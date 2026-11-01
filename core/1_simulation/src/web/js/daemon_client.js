/* ============================================================================
 * Oasis Layer 1b <-> Native POSIX Stack Bridge (col-gatewayd :8090)
 * Zero fake hardcoded data. Polls GET /api/v1/snapshot every 1.5s, dispatches
 * POST /api/v1/command matching SharedStackRuntime::DispatchJsonCommand, and
 * bridges VirtualPinBus L2 sensors/actuators via POST /api/v1/l2_pins.
 * ============================================================================ */
(function () {
  'use strict';

  const DEFAULT_GATEWAY_URL = 'http://127.0.0.1:8090';
  const POLL_INTERVAL_MS = 1500;
  const FETCH_TIMEOUT_MS = 1400;

  function resolveGatewayBaseUrl() {
    try {
      const params = new URLSearchParams(window.location.search || '');
      const override = params.get('gateway');
      if (override && /^https?:\/\//i.test(override)) {
        return override.replace(/\/+$/, '');
      }
    } catch (_) {
      // Ignore URL parsing errors
    }
    if (typeof window.OASIS_GATEWAY_URL === 'string' && window.OASIS_GATEWAY_URL.trim()) {
      return window.OASIS_GATEWAY_URL.trim().replace(/\/+$/, '');
    }
    return DEFAULT_GATEWAY_URL;
  }

  function ensureEngineQueue() {
    if (!Array.isArray(window._oasisEngineCommandQueue)) {
      window._oasisEngineCommandQueue = [];
    }
    return window._oasisEngineCommandQueue;
  }

  const OasisDaemonClient = {
    baseUrl: resolveGatewayBaseUrl(),
    isOnline: false,
    latestSnapshot: null,
    lastError: null,
    lastAppliedActuators: {
      irrigation_valve_open: null,
      heater_power_mw: null
    },
    _subscribers: new Set(),
    _statusListeners: new Set(),
    _pollTimer: null,
    _pollInFlight: false,
    _l2SyncInFlight: false,
    _channel: null,
    _lastSyncedBpmnTokenSig: '',

    init() {
      ensureEngineQueue();
      if (typeof BroadcastChannel !== 'undefined' && !this._channel) {
        try {
          this._channel = new BroadcastChannel('oasis_daemon_sync');
          this._channel.onmessage = (ev) => {
            const msg = ev && ev.data;
            if (!msg || typeof msg !== 'object') return;
            if (msg.type === 'snapshot_updated' && msg.snapshot) {
              this._ingestSnapshot(msg.snapshot, false);
            } else if (msg.type === 'execute_committed_task' && msg.task) {
              if (window.OasisUI && typeof window.OasisUI.executeCommittedTaskObject === 'function') {
                window.OasisUI.executeCommittedTaskObject(msg.task);
              }
            }
          };
        } catch (_) {
          this._channel = null;
        }
      }

      this._renderConnectionBadges();
      this.pollOnce();

      if (!this._pollTimer) {
        this._pollTimer = window.setInterval(() => {
          this.pollOnce();
        }, POLL_INTERVAL_MS);
      }
    },

    subscribe(fn) {
      if (typeof fn !== 'function') return () => {};
      this._subscribers.add(fn);
      if (this.latestSnapshot) {
        try {
          fn(this.latestSnapshot, this.isOnline);
        } catch (err) {
          console.error('[OasisDaemonClient] Subscriber error:', err);
        }
      }
      return () => {
        this._subscribers.delete(fn);
      };
    },

    onStatusChange(fn) {
      if (typeof fn !== 'function') return () => {};
      this._statusListeners.add(fn);
      try {
        fn(this.isOnline, this.lastError);
      } catch (_) {}
      return () => {
        this._statusListeners.delete(fn);
      };
    },

    _syncBpmnTokensToEngine(active, tokens, cultivarId) {
      const normalized = Array.isArray(tokens) ? tokens : [];
      const cult = String(cultivarId || '');
      const sig = `${active ? '1' : '0'}:${cult}:${JSON.stringify(normalized)}`;
      if (this._lastSyncedBpmnTokenSig === sig) return;
      this._lastSyncedBpmnTokenSig = sig;

      const cmd = {
        type: 'sync_bpmn_tokens',
        action: 'sync_bpmn_tokens',
        active: Boolean(active),
        cultivar_id: cult,
        tokens: normalized
      };
      const queue = ensureEngineQueue();
      const existingIdx = queue.findIndex((item) => item && item.type === 'sync_bpmn_tokens');
      if (existingIdx >= 0) {
        queue[existingIdx] = cmd;
      } else {
        queue.push(cmd);
      }
    },

    _setOnlineState(online, errMsg) {
      const changed = this.isOnline !== online || this.lastError !== (errMsg || null);
      this.isOnline = online;
      this.lastError = errMsg || null;
      this._renderConnectionBadges();
      if (changed) {
        if (!online) {
          this._syncBpmnTokensToEngine(false, [], '');
        }
        this._statusListeners.forEach((fn) => {
          try {
            fn(this.isOnline, this.lastError);
          } catch (_) {}
        });
        if (!online) {
          this._subscribers.forEach((fn) => {
            try {
              fn(this.latestSnapshot, false);
            } catch (_) {}
          });
        }
      }
    },

    _renderConnectionBadges() {
      const portMatch = this.baseUrl.match(/:(\d+)$/);
      const portLabel = portMatch ? `:${portMatch[1]}` : ':8090';
      const r1SwarmOn = Boolean(this.latestSnapshot && this.latestSnapshot.external_r1_swarm_enabled);
      const onlineText = r1SwarmOn
        ? `🟢 4-Node Cluster + R1 Swarm (${portLabel})`
        : `🟢 4-Node Homestead Cluster (${portLabel})`;
      const text = this.isOnline
        ? onlineText
        : '🟠 Native Daemons Offline — Run ./scripts/run_native_daemons.sh start';

      const targets = new Set();
      const byId = document.getElementById('daemon-status-badge');
      if (byId) targets.add(byId);
      document.querySelectorAll('.daemon-status-badge').forEach((el) => targets.add(el));

      targets.forEach((el) => {
        el.textContent = text;
        el.classList.toggle('online', this.isOnline);
        el.classList.toggle('offline', !this.isOnline);
      });
    },

    _ingestSnapshot(snapshot, broadcastToTabs) {
      if (!snapshot || typeof snapshot !== 'object') return;
      this.latestSnapshot = snapshot;
      this._syncBpmnTokensToEngine(
        true,
        snapshot.active_bpmn_tokens || [],
        snapshot.selected_cultivar_id || ''
      );
      this._setOnlineState(true, null);
      this._renderConnectionBadges();

      this._subscribers.forEach((fn) => {
        try {
          fn(this.latestSnapshot, true);
        } catch (err) {
          console.error('[OasisDaemonClient] Snapshot subscriber failed:', err);
        }
      });

      if (broadcastToTabs && this._channel) {
        try {
          this._channel.postMessage({
            type: 'snapshot_updated',
            snapshot: this.latestSnapshot
          });
        } catch (_) {}
      }
    },

    async _fetchJson(path, options) {
      const controller = new AbortController();
      const timer = window.setTimeout(() => controller.abort(), FETCH_TIMEOUT_MS);
      try {
        const res = await fetch(`${this.baseUrl}${path}`, {
          ...options,
          signal: controller.signal,
          headers: {
            Accept: 'application/json',
            ...(options && options.headers ? options.headers : {})
          }
        });
        if (!res.ok) {
          throw new Error(`HTTP ${res.status}`);
        }
        return await res.json();
      } finally {
        clearTimeout(timer);
      }
    },

    async pollOnce() {
      if (this._pollInFlight) return this.latestSnapshot;
      this._pollInFlight = true;
      try {
        // Forward 3D sim clock & any active L2 virtual pins before/with snapshot poll so BPMN timers stay in lockstep
        const activePins = window.OasisUI && window.OasisUI.latestL2Pins;
        if (activePins && typeof activePins === 'object') {
          await this.syncL2Pins(activePins).catch(() => {});
        }

        const snap = await this._fetchJson('/api/v1/snapshot', { method: 'GET' });
        this._ingestSnapshot(snap, false);
        return snap;
      } catch (err) {
        this._setOnlineState(false, err && err.message ? err.message : 'Connection refused');
        return null;
      } finally {
        this._pollInFlight = false;
      }
    },

    async sendCommand(action, payload) {
      if (!action || typeof action !== 'string') {
        throw new Error('Command action is required');
      }
      try {
        const snap = await this._fetchJson('/api/v1/command', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            action: action,
            payload: payload || {}
          })
        });
        this._ingestSnapshot(snap, true);
        return snap;
      } catch (err) {
        this._setOnlineState(false, err && err.message ? err.message : 'Command failed');
        throw err;
      }
    },

    _buildL2PinPayload(rawPins) {
      if (!rawPins || typeof rawPins !== 'object') return null;

      // Support epistemic-safe sim_clock_seconds forwarding + structured register objects
      const payload = {};
      let hasData = false;

      if (typeof rawPins.sim_clock_seconds === 'number' && rawPins.sim_clock_seconds > 0) {
        payload.sim_clock_seconds = Math.round(rawPins.sim_clock_seconds);
        hasData = true;
      }

      if (rawPins.soil_probe_v1 && typeof rawPins.soil_probe_v1 === 'object') {
        payload.soil_probe_v1 = rawPins.soil_probe_v1;
        hasData = true;
      }
      if (rawPins.irrigation_valve_v1 && typeof rawPins.irrigation_valve_v1 === 'object') {
        payload.irrigation_valve_v1 = rawPins.irrigation_valve_v1;
        hasData = true;
      }
      if (rawPins.culinary_vessel_v1 && typeof rawPins.culinary_vessel_v1 === 'object') {
        payload.culinary_vessel_v1 = rawPins.culinary_vessel_v1;
        hasData = true;
      }

      const moistureBp = rawPins.soil_moisture_bp ?? rawPins.moisture_vwc_bp ?? rawPins.moisture_bp;
      const soilTempMk = rawPins.soil_temp_mk ?? rawPins.temperature_mk;
      if (moistureBp !== undefined && moistureBp !== null) {
        payload.soil_moisture_bp = Math.round(Number(moistureBp));
        payload.soil_probe_v1 = payload.soil_probe_v1 || {
          moisture_bp: Math.round(Number(moistureBp)),
          temperature_mk: Math.round(Number(soilTempMk ?? 293150)),
          is_online: true
        };
        hasData = true;
      }

      const vesselTempMk = rawPins.vessel_temp_mk ?? rawPins.core_temp_mk;
      const holdMs = rawPins.hold_duration_ms ??
        (rawPins.pasteurization_hold_s !== undefined
          ? Math.round(Number(rawPins.pasteurization_hold_s) * 1000)
          : undefined);
      if (vesselTempMk !== undefined && vesselTempMk !== null) {
        payload.vessel_temp_mk = Math.round(Number(vesselTempMk));
        if (holdMs !== undefined) {
          payload.hold_duration_ms = Math.round(Number(holdMs));
        }
        payload.culinary_vessel_v1 = payload.culinary_vessel_v1 || {
          temperature_mk: Math.round(Number(vesselTempMk)),
          hold_duration_ms: Math.round(Number(holdMs ?? 0)),
          is_online: true
        };
        hasData = true;
      }

      return hasData ? payload : null;
    },

    async syncL2Pins(rawPins) {
      if (this._l2SyncInFlight) return null;
      const payload = this._buildL2PinPayload(rawPins);
      if (!payload) return null;

      this._l2SyncInFlight = true;
      try {
        const res = await this._fetchJson('/api/v1/l2_pins', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(payload)
        });
        if (res && typeof res === 'object') {
          this._pushActuatorStatesToEngine(res);
        }
        return res;
      } finally {
        this._l2SyncInFlight = false;
      }
    },

    _pushActuatorStatesToEngine(pinResponse) {
      const valveObj = pinResponse.irrigation_valve_v1 ||
        (pinResponse.actuators && pinResponse.actuators.irrigation_valve_v1) ||
        null;
      const vesselObj = pinResponse.culinary_vessel_v1 ||
        (pinResponse.actuators && pinResponse.actuators.culinary_vessel_v1) ||
        null;

      const valveBp = valveObj
        ? Number(valveObj.valve_open_bp ?? valveObj.actuator_level_bp ?? (valveObj.commanded_on ? 10000 : 0))
        : 0;
      const irrigationValveOpen = Boolean(
        (valveObj && valveObj.commanded_on) || valveBp > 0
      );

      let heaterPowerMw = 0;
      if (typeof pinResponse.heater_power_mw === 'number') {
        heaterPowerMw = Math.round(pinResponse.heater_power_mw);
      } else if (vesselObj) {
        if (typeof vesselObj.heater_power_mw === 'number') {
          heaterPowerMw = Math.round(vesselObj.heater_power_mw);
        } else if (vesselObj.actuator_commanded_on || vesselObj.commanded_on) {
          const levelBp = Number(vesselObj.actuator_level_bp ?? 10000);
          heaterPowerMw = Math.round((levelBp / 10000) * 1800000);
        }
      }

      const prev = this.lastAppliedActuators;
      if (
        prev.irrigation_valve_open === irrigationValveOpen &&
        prev.heater_power_mw === heaterPowerMw
      ) {
        return;
      }

      this.lastAppliedActuators = {
        irrigation_valve_open: irrigationValveOpen,
        heater_power_mw: heaterPowerMw
      };

      const cmd = {
        type: 'l2_actuators',
        action: 'set_l2_actuators',
        irrigation_valve_open: irrigationValveOpen,
        irrigation_valve_bp: valveBp,
        heater_power_mw: heaterPowerMw
      };

      const queue = ensureEngineQueue();
      // Replace any unconsumed l2_actuators command so queue stays compact
      const existingIdx = queue.findIndex((item) => item && item.type === 'l2_actuators');
      if (existingIdx >= 0) {
        queue[existingIdx] = cmd;
      } else {
        queue.push(cmd);
      }
    },

    broadcastCommittedTaskExecution(task) {
      if (this._channel && task) {
        try {
          this._channel.postMessage({
            type: 'execute_committed_task',
            task: task
          });
        } catch (_) {}
      }
    }
  };

  window.OasisDaemonClient = OasisDaemonClient;

  document.addEventListener('DOMContentLoaded', () => {
    OasisDaemonClient.init();
  });
})();
