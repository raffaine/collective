/* ============================================================================
 * Oasis Layer 1b — Minimal Epistemic In-Game HUD (web/js/hud_feel.js)
 * Enforces strict epistemic privacy:
 *  - Top-Left: 1-to-5 Biological Feel Strip (5 tactile pips + qualitative word;
 *    zero exact kcal/hydration/bank-balance numbers unless l2_biometric_installed)
 *  - Top-Right: Daemon status badge, Sim Clock, Time Warp, Roof/Map/Audio toggles,
 *    and My Committed Tasks Checklist from live col-gatewayd (:8090)
 *  - Bottom-Center: Steward Inner Thought, Hauling Tool badge & Progress Ribbon
 *  - Bottom-Right: Live Pending L6 Decision Pill & L6 Companion Drawer toggle
 * ============================================================================ */
(function () {
  'use strict';

  const FEEL_AXES = [
    {
      key: 'hunger',
      label: 'Hunger',
      bpKeys: ['hunger_bp'],
      words: ['Famished', 'Peckish', 'Steady', 'Nourished', 'Well-Fed']
    },
    {
      key: 'energy',
      label: 'Energy',
      bpKeys: ['energy_bp'],
      words: ['Exhausted', 'Drowsy', 'Rested', 'Energetic', 'Vibrant']
    },
    {
      key: 'focus',
      label: 'Focus',
      bpKeys: ['introspection_bp', 'focus_bp'],
      words: ['Scattered', 'Drifting', 'Attentive', 'Grounded', 'Immersed']
    },
    {
      key: 'mood',
      label: 'Mood',
      bpKeys: ['leisure_bp', 'mood_bp'],
      words: ['Strained', 'Uneasy', 'Calm', 'Content', 'Serene']
    },
    {
      key: 'social',
      label: 'Social',
      bpKeys: ['social_bp'],
      words: ['Isolated', 'Quiet', 'Connected', 'Warm', 'Kinship']
    }
  ];

  const HAUL_BADGES = {
    BUCKET_LIQUID_HAUL: '🪣 Bucket',
    WHEELBARROW_DOLLY_HEAVY_HAUL: '🛒 Wheelbarrow / Dolly',
    GRANULAR_HAUL: '🧺 Granular Tote',
    TIFFIN_CARRIER_HAUL: '🍱 Tiffin Carrier',
    bucket_10l: '🪣 10L Bucket',
    wheelbarrow_80l: '🛒 Wheelbarrow',
    hand_dolly_150kg: '🛞 Hand Dolly',
    harvest_basket: '🧺 Harvest Basket',
    tiffin_carrier: '🍱 Tiffin Carrier'
  };

  function ensureCommandQueue() {
    if (!Array.isArray(window._oasisEngineCommandQueue)) {
      window._oasisEngineCommandQueue = [];
    }
    return window._oasisEngineCommandQueue;
  }

  function clampTier(val) {
    const n = Math.round(Number(val));
    if (Number.isNaN(n)) return 4;
    return Math.max(1, Math.min(5, n));
  }

  function bpToTier(bp) {
    const v = Number(bp);
    if (Number.isNaN(v)) return 4;
    if (v < 2000) return 1;
    if (v < 4000) return 2;
    if (v < 6500) return 3;
    if (v < 8500) return 4;
    return 5;
  }

  function formatPropertyEntry(entry) {
    if (!entry || !entry.key) return '';
    const k = String(entry.key);
    const v = Number(entry.value);
    if (k === 'moisture_vwc_bp') return `Moisture ${(v / 100).toFixed(1)}% VWC`;
    if (k === 'soil_temp_mk') return `Soil Temp ${(v / 1000 - 273.15).toFixed(1)} °C`;
    if (k === 'core_temp_mk') return `Vessel Temp ${(v / 1000 - 273.15).toFixed(1)} °C`;
    if (k === 'ph_centi') return `pH ${(v / 100).toFixed(2)}`;
    if (k === 'organic_carbon_bp') return `Organic C ${(v / 100).toFixed(1)}%`;
    if (k === 'crop_growth_bp') return `Growth ${(v / 100).toFixed(0)}%`;
    if (k === 'nitrogen_ppm') return `N ${v} ppm`;
    if (k === 'phosphorus_ppm') return `P ${v} ppm`;
    if (k === 'potassium_ppm') return `K ${v} ppm`;
    if (k === 'liquid_volume_ml') return `Volume ${(v / 1000).toFixed(2)} L`;
    if (k === 'pasteurization_hold_s') return `Hold ${v}s`;
    return `${k}: ${entry.value}${entry.unit_hint ? ' ' + entry.unit_hint : ''}`;
  }

  function escapeHtml(str) {
    return String(str ?? '')
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;');
  }

  const OasisUI = {
    latestFrame: null,
    latestL2Pins: null,
    timeWarp: 1,
    roofCutaway: true,
    mapMode: 'static',
    drawerOpen: false,
    availablePage: 0,
    availablePageSize: 2,
    availableTotalPages: 1,
    committedTasksById: new Map(),
    executingTaskState: null, // { taskId, sawEngineProgress, startedAtMs }
    _completedSentIds: new Set(),
    _inspectionTimer: null,

    init() {
      ensureCommandQueue();
      this._buildFeelStripSkeleton();
      this._bindControls();

      if (window.OasisDaemonClient) {
        window.OasisDaemonClient.subscribe((snapshot, isOnline) => {
          this._onDaemonSnapshot(snapshot, isOnline);
        });
      }
    },

    _buildFeelStripSkeleton() {
      const container = document.getElementById('hud-feel-strip');
      if (!container) return;
      container.innerHTML = FEEL_AXES.map((axis) => {
        const pips = [1, 2, 3, 4, 5]
          .map((idx) => `<span class="feel-pip ${idx <= 4 ? 'filled tier-4' : ''}" data-pip="${idx}"></span>`)
          .join('');
        return `
          <div class="feel-row" id="feel-row-${axis.key}">
            <span class="feel-name">${axis.label}</span>
            <span class="feel-pips">${pips}</span>
            <span class="feel-word tier-4">${axis.words[3]}</span>
          </div>
        `;
      }).join('');
    },

    _bindControls() {
      document.querySelectorAll('[data-warp]').forEach((btn) => {
        btn.addEventListener('click', () => {
          const mult = Number(btn.getAttribute('data-warp'));
          this.setTimeWarp(mult);
        });
      });

      const roofBtn = document.getElementById('btn-roof-toggle');
      if (roofBtn) {
        roofBtn.addEventListener('click', () => this.toggleRoof());
      }

      const mapBtn = document.getElementById('btn-map-toggle');
      if (mapBtn) {
        mapBtn.addEventListener('click', () => this.toggleMapMode());
      }

      const audioBtn = document.getElementById('btn-audio-toggle');
      if (audioBtn) {
        audioBtn.addEventListener('click', () => {
          if (window.OasisAudio) window.OasisAudio.toggle();
        });
      }

      const prevAvailBtn = document.getElementById('btn-avail-prev');
      if (prevAvailBtn) {
        prevAvailBtn.addEventListener('click', () => {
          if (this.availablePage > 0) {
            this.availablePage -= 1;
            if (window.OasisAudio) window.OasisAudio.playClick();
            if (window.OasisDaemonClient && window.OasisDaemonClient.latestSnapshot) {
              this._renderAvailableTasksCard(
                window.OasisDaemonClient.latestSnapshot,
                window.OasisDaemonClient.isOnline
              );
            }
          }
        });
      }

      const nextAvailBtn = document.getElementById('btn-avail-next');
      if (nextAvailBtn) {
        nextAvailBtn.addEventListener('click', () => {
          if (this.availablePage + 1 < this.availableTotalPages) {
            this.availablePage += 1;
            if (window.OasisAudio) window.OasisAudio.playClick();
            if (window.OasisDaemonClient && window.OasisDaemonClient.latestSnapshot) {
              this._renderAvailableTasksCard(
                window.OasisDaemonClient.latestSnapshot,
                window.OasisDaemonClient.isOnline
              );
            }
          }
        });
      }

      const pillBtn = document.getElementById('hud-decision-pill');
      if (pillBtn) {
        pillBtn.addEventListener('click', () => {
          this.toggleCompanionDrawer(true, 'decisions');
        });
      }

      const drawerBtn = document.getElementById('btn-companion-toggle');
      if (drawerBtn) {
        drawerBtn.addEventListener('click', () => {
          this.toggleCompanionDrawer(!this.drawerOpen);
        });
      }
    },

    enqueueEngineCommand(cmdObj) {
      if (!cmdObj || typeof cmdObj !== 'object') return;
      const queue = ensureCommandQueue();
      queue.push(cmdObj);
    },

    popEngineCommandJson() {
      const queue = ensureCommandQueue();
      if (queue.length === 0) return '';
      return JSON.stringify(queue.shift());
    },

    setTimeWarp(mult) {
      const m = Number(mult);
      if (Number.isNaN(m)) return;
      this.timeWarp = m;
      document.querySelectorAll('[data-warp]').forEach((btn) => {
        const btnVal = Number(btn.getAttribute('data-warp'));
        btn.classList.toggle('active', btnVal === m);
      });
      if (window.OasisAudio) {
        window.OasisAudio.setTimeWarp(m);
        window.OasisAudio.playClick();
      }
      this.enqueueEngineCommand({
        type: 'set_time_warp',
        action: 'set_time_warp',
        multiplier: m
      });
    },

    toggleRoof() {
      this.roofCutaway = !this.roofCutaway;
      const btn = document.getElementById('btn-roof-toggle');
      if (btn) {
        btn.classList.toggle('active', this.roofCutaway);
        btn.textContent = this.roofCutaway ? '🏠 Roof: Cut' : '🏠 Roof: Full';
      }
      if (window.OasisAudio) window.OasisAudio.playClick();
      this.enqueueEngineCommand({
        type: 'toggle_roof',
        action: 'toggle_roof',
        roof_cutaway: this.roofCutaway
      });
    },

    toggleMapMode() {
      this.mapMode = this.mapMode === 'static' ? 'procedural' : 'static';
      const btn = document.getElementById('btn-map-toggle');
      if (btn) {
        btn.textContent = this.mapMode === 'static'
          ? '🗺️ Map: Static'
          : '🗺️ Map: Procedural';
        btn.classList.toggle('active', this.mapMode === 'procedural');
      }
      if (window.OasisAudio) window.OasisAudio.playClick();
      this.enqueueEngineCommand({
        type: 'set_map_mode',
        action: 'set_map_mode',
        mode: this.mapMode
      });
    },

    toggleCompanionDrawer(forceOpen, tabId) {
      this.drawerOpen = typeof forceOpen === 'boolean' ? forceOpen : !this.drawerOpen;
      const drawer = document.getElementById('l6-companion-drawer');
      if (drawer) {
        drawer.classList.toggle('hidden', !this.drawerOpen);
      }
      if (this.drawerOpen && tabId && window.OasisCompanionL6) {
        window.OasisCompanionL6.switchTab(tabId);
      }
      if (window.OasisAudio) window.OasisAudio.playClick();
    },

    onEngineFrame(jsonOrObj) {
      let frame = jsonOrObj;
      if (typeof jsonOrObj === 'string') {
        if (!jsonOrObj.trim()) return;
        try {
          frame = JSON.parse(jsonOrObj);
        } catch (err) {
          console.warn('[OasisUI] Malformed engine frame JSON:', err);
          return;
        }
      }
      if (!frame || typeof frame !== 'object') return;
      this.latestFrame = frame;

      // 1. Capture active virtual L2 pins for col-gatewayd (:8090) forwarding
      const pins = frame.l2_pins || frame.virtual_pins || frame.latest_l2_pins || null;
      if (pins && typeof pins === 'object') {
        this.latestL2Pins = pins;
      }

      // 2. Render 1-to-5 Biological Feel Strip (strict epistemic privacy)
      this._renderFeelStrip(frame);

      // 3. Render Sim Clock & sync audio day/night
      this._renderClock(frame);

      // 4. Render Steward Inner Thought & Calm Activity Progress Ribbon
      this._renderActivityRibbon(frame);

      // 5. Render diegetic inspection report if emitted this frame
      if (frame.inspection_report) {
        this.onInspectionReport(frame.inspection_report);
      }

      // 6. Check L4 committed task completion lifecycle
      this._checkCommittedTaskLifecycle(frame);
    },

    _resolveAxisFeel(frame, axis) {
      const feelObj = frame.feel || frame.biological_feel || {};
      const drivesObj = frame.drives || frame;

      let tier = null;
      let customWord = null;

      const direct = feelObj[axis.key] ?? frame[`${axis.key}_tier`];
      if (direct !== undefined && direct !== null) {
        if (typeof direct === 'object') {
          tier = clampTier(direct.tier ?? direct.level ?? direct.pips ?? 4);
          if (direct.label || direct.word) customWord = String(direct.label || direct.word);
        } else {
          tier = clampTier(direct);
        }
      }

      if (tier === null) {
        for (const bpKey of axis.bpKeys) {
          if (drivesObj[bpKey] !== undefined && drivesObj[bpKey] !== null) {
            tier = bpToTier(drivesObj[bpKey]);
            break;
          }
        }
      }

      if (tier === null && typeof frame[axis.key] === 'number') {
        const val = frame[axis.key];
        tier = val <= 1.0 ? bpToTier(val * 10000) : (val <= 5 ? clampTier(val) : bpToTier(val));
      }

      if (tier === null) tier = 4;

      const explicitLabel = feelObj[`${axis.key}_label`] || frame[`${axis.key}_label`];
      const word = customWord || (explicitLabel ? String(explicitLabel) : axis.words[tier - 1]);
      return { tier, word };
    },

    _renderFeelStrip(frame) {
      FEEL_AXES.forEach((axis) => {
        const row = document.getElementById(`feel-row-${axis.key}`);
        if (!row) return;
        const { tier, word } = this._resolveAxisFeel(frame, axis);

        row.querySelectorAll('.feel-pip').forEach((pipEl) => {
          const idx = Number(pipEl.getAttribute('data-pip'));
          pipEl.className = idx <= tier
            ? `feel-pip filled tier-${tier}`
            : 'feel-pip';
        });

        const wordEl = row.querySelector('.feel-word');
        if (wordEl) {
          wordEl.textContent = word;
          wordEl.className = `feel-word tier-${tier}`;
        }
      });

      const biometricInstalled = Boolean(frame.l2_biometric_installed);
      const tagEl = document.getElementById('hud-feel-epistemic-tag');
      if (tagEl) {
        tagEl.textContent = biometricInstalled ? 'L2 BIOMETRIC' : 'QUALITATIVE';
        tagEl.classList.toggle('l2-active', biometricInstalled);
      }

      const bioBox = document.getElementById('hud-l2-biometric-box');
      if (bioBox) {
        if (biometricInstalled && frame.biometric_telemetry) {
          const bt = frame.biometric_telemetry;
          const parts = [];
          if (bt.heart_rate_bpm !== undefined) parts.push(`HR ${bt.heart_rate_bpm} bpm`);
          if (bt.kcal_reserve !== undefined) parts.push(`Energy ${bt.kcal_reserve} kcal`);
          if (bt.hydration_pct !== undefined) parts.push(`Hydration ${bt.hydration_pct}%`);
          bioBox.textContent = parts.join(' · ') || 'L2 Biometric Cuff Active';
          bioBox.classList.remove('hidden');
        } else {
          bioBox.textContent = '';
          bioBox.classList.add('hidden');
        }
      }
    },

    _renderClock(frame) {
      const clockEl = document.getElementById('hud-sim-clock');
      if (!clockEl) return;

      if (frame.clock_label) {
        clockEl.textContent = String(frame.clock_label);
      } else if (
        frame.sim_day !== undefined ||
        frame.day !== undefined ||
        frame.sim_time_ms !== undefined
      ) {
        let day = Number(frame.sim_day ?? frame.day ?? 1);
        let hour = Number(frame.sim_hour ?? frame.hour ?? 8);
        let minute = Number(frame.sim_minute ?? frame.minute ?? 0);

        if (frame.sim_time_ms !== undefined && frame.sim_hour === undefined) {
          const totalMin = Math.floor(Number(frame.sim_time_ms) / 60000);
          day = Math.floor(totalMin / 1440) + 1;
          hour = Math.floor((totalMin % 1440) / 60);
          minute = totalMin % 60;
        }

        const hh = String(Math.max(0, Math.min(23, Math.floor(hour)))).padStart(2, '0');
        const mm = String(Math.max(0, Math.min(59, Math.floor(minute)))).padStart(2, '0');
        clockEl.textContent = `Day ${day} · ${hh}:${mm}`;
        if (window.OasisAudio) window.OasisAudio.setDayPhase(hour);
      }

      if (typeof frame.time_warp === 'number' && frame.time_warp !== this.timeWarp) {
        this.timeWarp = frame.time_warp;
        document.querySelectorAll('[data-warp]').forEach((btn) => {
          btn.classList.toggle('active', Number(btn.getAttribute('data-warp')) === this.timeWarp);
        });
      }
    },

    _renderActivityRibbon(frame) {
      const thoughtEl = document.getElementById('hud-steward-thought');
      if (thoughtEl) {
        const thought = frame.thought || frame.steward_thought || frame.inner_thought;
        if (thought) {
          thoughtEl.textContent = `“${thought}”`;
        }
      }

      const actLabelEl = document.getElementById('hud-activity-label');
      if (actLabelEl) {
        const label = frame.activity_label || frame.current_activity_label || frame.activity;
        if (label) {
          actLabelEl.textContent = String(label);
        }
      }

      const haulBadgeEl = document.getElementById('hud-hauling-badge');
      if (haulBadgeEl) {
        const toolKey = frame.equipped_tool_id || frame.hauling_tool || frame.haul_modality || '';
        const toolLabel = frame.equipped_tool_label || HAUL_BADGES[toolKey] || (toolKey && toolKey !== 'NONE' ? `🧰 ${toolKey}` : '');
        if (toolLabel) {
          haulBadgeEl.textContent = toolLabel;
          haulBadgeEl.classList.remove('hidden');
        } else {
          haulBadgeEl.classList.add('hidden');
        }
      }

      let pct = 0;
      if (typeof frame.activity_progress === 'number') {
        pct = frame.activity_progress <= 1.0 ? frame.activity_progress * 100 : frame.activity_progress;
      } else if (typeof frame.progress_bp === 'number') {
        pct = frame.progress_bp / 100;
      }
      pct = Math.max(0, Math.min(100, pct));

      const fillEl = document.getElementById('hud-activity-progress-fill');
      if (fillEl) {
        fillEl.style.width = `${pct.toFixed(1)}%`;
      }

      const remEl = document.getElementById('hud-activity-remaining');
      if (remEl) {
        if (typeof frame.remaining_sim_min === 'number' && frame.remaining_sim_min > 0) {
          remEl.textContent = `${Math.ceil(frame.remaining_sim_min)}m left`;
        } else if (typeof frame.remaining_sim_s === 'number' && frame.remaining_sim_s > 0) {
          const mins = Math.ceil(frame.remaining_sim_s / 60);
          remEl.textContent = mins > 1 ? `${mins}m left` : `${Math.ceil(frame.remaining_sim_s)}s`;
        } else if (pct > 0 && pct < 100) {
          remEl.textContent = `${Math.round(pct)}%`;
        } else {
          remEl.textContent = 'Calm';
        }
      }
    },

    onInspectionReport(reportOrJson) {
      let rep = reportOrJson;
      if (typeof reportOrJson === 'string') {
        try {
          rep = JSON.parse(reportOrJson);
        } catch (_) {
          rep = { headline: 'Inspection', look_description: reportOrJson };
        }
      }
      if (!rep || typeof rep !== 'object') return;

      const toast = document.getElementById('hud-inspection-toast');
      if (!toast) return;

      const props = Array.isArray(rep.revealed_properties)
        ? rep.revealed_properties.map(formatPropertyEntry).filter(Boolean)
        : [];
      const inv = Array.isArray(rep.revealed_inventory)
        ? rep.revealed_inventory.map((s) => `${s.item_id} ×${s.quantity}`)
        : [];

      const details = [...props, ...inv].join(' · ');
      const l2Tag = rep.has_live_l2_telemetry ? ' [L2 Live]' : ' [Manual Observation]';
      const title = rep.headline ? `${rep.headline}${l2Tag}: ` : 'Observed: ';
      const body = details || rep.summary || rep.look_description || 'No anomalous readings.';

      toast.textContent = `🔍 ${title}${body}`;
      toast.classList.remove('hidden');

      if (window.OasisAudio) window.OasisAudio.playInspect();
      if (this._inspectionTimer) clearTimeout(this._inspectionTimer);
      this._inspectionTimer = window.setTimeout(() => {
        toast.classList.add('hidden');
      }, 14000);
    },

    _renderAvailableTasksCard(snapshot, isOnline) {
      const availCountEl = document.getElementById('hud-available-count');
      const availPageEl = document.getElementById('hud-available-page');
      const prevBtn = document.getElementById('btn-avail-prev');
      const nextBtn = document.getElementById('btn-avail-next');
      const availListEl = document.getElementById('hud-available-list');
      if (!availListEl) return;

      if (!isOnline || !snapshot) {
        if (availCountEl) availCountEl.textContent = '0';
        if (availPageEl) availPageEl.textContent = '1/1';
        if (prevBtn) prevBtn.disabled = true;
        if (nextBtn) nextBtn.disabled = true;
        availListEl.innerHTML = '<div class="committed-empty">Start native POSIX daemons (./scripts/run_native_daemons.sh start) to browse available BPMN tasks.</div>';
        return;
      }

      const wfMap = new Map();
      if (Array.isArray(snapshot.l4_workflows)) {
        snapshot.l4_workflows.forEach((wf) => {
          if (wf && wf.workflow_id) wfMap.set(wf.workflow_id, wf);
        });
      }

      let available = Array.isArray(snapshot.l4_available_tasks)
        ? snapshot.l4_available_tasks.slice()
        : [];
      if (available.length === 0 && Array.isArray(snapshot.l4_workflows)) {
        snapshot.l4_workflows.forEach((wf) => {
          (wf.tasks || []).forEach((t) => {
            if (
              t &&
              t.bpmn_token_state === 'ACTIVE_TOKEN' &&
              t.task_commitment_state === 'UNASSIGNED'
            ) {
              available.push(t);
            }
          });
        });
      }

      let waitingTimers = Array.isArray(snapshot.l4_waiting_timers)
        ? snapshot.l4_waiting_timers.slice()
        : [];
      if (waitingTimers.length === 0 && Array.isArray(snapshot.l4_workflows)) {
        snapshot.l4_workflows.forEach((wf) => {
          (wf.tasks || []).forEach((t) => {
            if (t && t.bpmn_token_state === 'WAITING_TIMER') {
              waitingTimers.push(t);
            }
          });
        });
      }

      const entries = [
        ...available.map((task) => ({ kind: 'available', task })),
        ...waitingTimers.map((task) => ({ kind: 'timer', task }))
      ];

      if (availCountEl) {
        availCountEl.textContent = waitingTimers.length > 0
          ? `${available.length} · ${waitingTimers.length}⏳`
          : String(available.length);
      }

      this.availableTotalPages = Math.max(1, Math.ceil(entries.length / this.availablePageSize));
      if (this.availablePage >= this.availableTotalPages) {
        this.availablePage = this.availableTotalPages - 1;
      }
      if (this.availablePage < 0) {
        this.availablePage = 0;
      }

      if (availPageEl) {
        availPageEl.textContent = `${this.availablePage + 1}/${this.availableTotalPages}`;
      }
      if (prevBtn) prevBtn.disabled = this.availablePage <= 0;
      if (nextBtn) nextBtn.disabled = this.availablePage + 1 >= this.availableTotalPages;

      if (entries.length === 0) {
        const hasWorkflows = Array.isArray(snapshot.l4_workflows) && snapshot.l4_workflows.length > 0;
        availListEl.innerHTML = hasWorkflows
          ? '<div class="committed-empty">All active BPMN tokens are committed or completed. Execute your committed task below or start a new cycle in L6 Companion.</div>'
          : '<div class="committed-empty">0 unsolicited tasks on Day 1. Approve an L5 Decision (🔔 below) to unlock its first BPMN step.</div>';
        return;
      }

      const startIdx = this.availablePage * this.availablePageSize;
      const pageEntries = entries.slice(startIdx, startIdx + this.availablePageSize);

      availListEl.innerHTML = pageEntries.map((entry) => {
        const task = entry.task;
        if (entry.kind === 'timer') {
          const wf = wfMap.get(task.workflow_id) || {};
          const cultTitle = wf.input_cultivar_title || 'Legume Crop';
          const growthPct = Math.round(Number(wf.crop_growth_bp || 0) / 100);
          const remLabel = task.timer_remaining_label || `${task.timer_remaining_sim_s || 0}s`;
          const stageLabel = wf.phenology_stage_label || 'Growing';
          return `
            <div class="waiting-timer-item">
              <div class="committed-item-info">
                <div class="committed-item-title" style="color:var(--amber);" title="${escapeHtml(task.title)}">
                  ⏳ ${escapeHtml(task.title)}
                </div>
                <div class="committed-item-meta" style="color:#fde68a;">
                  ${escapeHtml(cultTitle)} · ${growthPct}% (${escapeHtml(stageLabel)}) · Next in ${escapeHtml(remLabel)}
                </div>
              </div>
              <span class="hud-tag" style="border-color:var(--border-amber);color:var(--amber);">⏱ ${escapeHtml(remLabel)}</span>
            </div>
          `;
        }

        const haulTag = HAUL_BADGES[task.haul_modality] || '';
        const metaParts = [];
        if (task.assigned_node_id) metaParts.push(task.assigned_node_id);
        if (task.location_label) metaParts.push(task.location_label);
        if (haulTag) metaParts.push(haulTag);

        return `
          <div class="available-item">
            <div class="committed-item-info">
              <div class="committed-item-title" title="${escapeHtml(task.title)}">${escapeHtml(task.title)}</div>
              <div class="committed-item-meta">${escapeHtml(metaParts.join(' · ') || task.workflow_id || 'Homestead')}</div>
            </div>
            <button type="button" class="hud-btn" data-hud-commit-task="${escapeHtml(task.task_id)}" title="Commit active BPMN token to My Committed Tasks">📌 Commit</button>
          </div>
        `;
      }).join('');

      availListEl.querySelectorAll('[data-hud-commit-task]').forEach((btn) => {
        btn.addEventListener('click', async () => {
          const taskId = btn.getAttribute('data-hud-commit-task');
          if (!taskId || !window.OasisDaemonClient) return;
          btn.disabled = true;
          if (window.OasisAudio) window.OasisAudio.playClick();
          try {
            await window.OasisDaemonClient.sendCommand('commit_task', { task_id: taskId });
          } catch (err) {
            btn.disabled = false;
            console.warn('[OasisUI] Failed to commit available task:', err);
          }
        });
      });
    },

    _onDaemonSnapshot(snapshot, isOnline) {
      const pillBtn = document.getElementById('hud-decision-pill');
      const countBadge = document.getElementById('hud-committed-count');
      const listEl = document.getElementById('hud-committed-list');

      // Render Available Tasks & BPMN Timers window directly above My Committed Tasks
      this._renderAvailableTasksCard(snapshot, isOnline);

      if (!isOnline || !snapshot) {
        if (pillBtn) {
          pillBtn.textContent = '🟠 L6 Daemons Offline (:8090)';
          pillBtn.classList.remove('all-clear');
        }
        if (listEl && this.committedTasksById.size === 0) {
          listEl.innerHTML = '<div class="committed-empty">Start native POSIX daemons (./scripts/run_native_daemons.sh start) to sync committed L4 tasks.</div>';
        }
        return;
      }

      // 1. Update Pending L6 Decision Pill
      const decisions = Array.isArray(snapshot.l6_pending_decisions)
        ? snapshot.l6_pending_decisions
        : [];
      const pending = decisions.filter((d) => !d.status || d.status === 'PENDING');
      if (pillBtn) {
        if (pending.length > 0) {
          pillBtn.textContent = `🔔 ${pending.length} Decision${pending.length === 1 ? '' : 's'} Pending in L6 Companion`;
          pillBtn.classList.remove('all-clear');
        } else {
          pillBtn.textContent = '✅ 0 Decisions Pending in L6 Companion';
          pillBtn.classList.add('all-clear');
        }
      }

      // 2. Clear completion cache for any workflow tasks reset or re-armed by a BPMN timer
      if (Array.isArray(snapshot.l4_workflows)) {
        snapshot.l4_workflows.forEach((wf) => {
          (wf.tasks || []).forEach((t) => {
            if (t && t.task_id && t.task_commitment_state !== 'COMPLETED') {
              this._completedSentIds.delete(t.task_id);
            }
          });
        });
      }

      // 3. Populate My Committed Tasks Checklist from COMMITTED_BY_PLAYER tasks
      let committed = Array.isArray(snapshot.l4_committed_tasks)
        ? snapshot.l4_committed_tasks.filter((t) => t.task_commitment_state === 'COMMITTED_BY_PLAYER')
        : [];

      if (committed.length === 0 && Array.isArray(snapshot.l4_workflows)) {
        snapshot.l4_workflows.forEach((wf) => {
          (wf.tasks || []).forEach((t) => {
            if (t.task_commitment_state === 'COMMITTED_BY_PLAYER') {
              committed.push(t);
            }
          });
        });
      }

      this.committedTasksById.clear();
      committed.forEach((t) => this.committedTasksById.set(t.task_id, t));

      if (this.executingTaskState && !this.committedTasksById.has(this.executingTaskState.taskId)) {
        this.executingTaskState = null;
      }

      if (countBadge) {
        countBadge.textContent = String(committed.length);
      }

      if (!listEl) return;
      if (committed.length === 0) {
        listEl.innerHTML = '<div class="committed-empty">No tasks committed to you. Click 📌 Commit in Available Tasks above or in L6 Companion.</div>';
        return;
      }

      listEl.innerHTML = committed.map((task) => {
        const isExecuting = this.executingTaskState && this.executingTaskState.taskId === task.task_id;
        const hasActiveToken = !task.bpmn_token_state || task.bpmn_token_state === 'ACTIVE_TOKEN';
        const haulTag = HAUL_BADGES[task.haul_modality] || '';
        const metaParts = [];
        if (task.assigned_node_id) metaParts.push(task.assigned_node_id);
        if (task.location_label) metaParts.push(task.location_label);
        if (haulTag) metaParts.push(haulTag);

        let actionBtn = '<span class="hud-tag">⏳ Queued</span>';
        if (isExecuting) {
          actionBtn = `<button type="button" class="hud-btn active" data-finish-task="${escapeHtml(task.task_id)}" title="Confirm completion with col-gatewayd">✓ Complete</button>`;
        } else if (hasActiveToken) {
          actionBtn = `<button type="button" class="hud-btn" data-exec-task="${escapeHtml(task.task_id)}">▶ Execute</button>`;
        }

        return `
          <div class="committed-item">
            <div class="committed-item-info">
              <div class="committed-item-title" title="${escapeHtml(task.title)}">${escapeHtml(task.title)}</div>
              <div class="committed-item-meta">${escapeHtml(metaParts.join(' · ') || task.workflow_id || 'Homestead')}</div>
            </div>
            ${actionBtn}
          </div>
        `;
      }).join('');

      listEl.querySelectorAll('[data-exec-task]').forEach((btn) => {
        btn.addEventListener('click', () => {
          this.executeCommittedTask(btn.getAttribute('data-exec-task'));
        });
      });

      listEl.querySelectorAll('[data-finish-task]').forEach((btn) => {
        btn.addEventListener('click', () => {
          this.completeCommittedTaskInDaemon(btn.getAttribute('data-finish-task'));
        });
      });
    },

    executeCommittedTask(taskId) {
      const task = this.committedTasksById.get(taskId);
      if (!task) return;
      this.executeCommittedTaskObject(task);
    },

    executeCommittedTaskObject(task) {
      if (!task || !task.task_id) return;
      this.executingTaskState = {
        taskId: task.task_id,
        sawEngineProgress: false,
        startedAtMs: Date.now()
      };

      if (window.OasisAudio) window.OasisAudio.playStamp();

      this.enqueueEngineCommand({
        type: 'execute_l4_task',
        action: 'execute_l4_task',
        task_id: task.task_id,
        workflow_id: task.workflow_id || '',
        asset_id: task.asset_id || '',
        oasis_action_code: Number(task.oasis_action_code || 0),
        haul_modality: task.haul_modality || 'NONE',
        title: task.title || '',
        location_label: task.location_label || ''
      });

      const actLabelEl = document.getElementById('hud-activity-label');
      if (actLabelEl && task.title) {
        actLabelEl.textContent = `Executing: ${task.title}`;
      }

      if (window.OasisDaemonClient && window.OasisDaemonClient.latestSnapshot) {
        this._onDaemonSnapshot(window.OasisDaemonClient.latestSnapshot, window.OasisDaemonClient.isOnline);
      }
    },

    _checkCommittedTaskLifecycle(frame) {
      const explicitDoneId =
        frame.completed_l4_task_id ||
        frame.completed_task_id ||
        frame.last_completed_task_id ||
        null;

      if (explicitDoneId && !this._completedSentIds.has(explicitDoneId)) {
        this.completeCommittedTaskInDaemon(String(explicitDoneId));
        return;
      }

      if (!this.executingTaskState) return;
      const activeId = frame.active_l4_task_id || frame.executing_task_id || null;
      const pct = typeof frame.activity_progress === 'number'
        ? (frame.activity_progress <= 1.0 ? frame.activity_progress : frame.activity_progress / 100)
        : 0;

      if (activeId === this.executingTaskState.taskId || (pct > 0.02 && pct < 0.99)) {
        this.executingTaskState.sawEngineProgress = true;
      } else if (
        this.executingTaskState.sawEngineProgress &&
        (pct >= 0.99 || !activeId) &&
        Date.now() - this.executingTaskState.startedAtMs > 600
      ) {
        this.completeCommittedTaskInDaemon(this.executingTaskState.taskId);
      }
    },

    async completeCommittedTaskInDaemon(taskId) {
      if (!taskId || this._completedSentIds.has(taskId)) return;
      this._completedSentIds.add(taskId);
      if (this.executingTaskState && this.executingTaskState.taskId === taskId) {
        this.executingTaskState = null;
      }
      if (window.OasisAudio) window.OasisAudio.playApprove();

      if (window.OasisDaemonClient) {
        const payload = { task_id: taskId };
        if (this.latestFrame) {
          if (
            typeof this.latestFrame.observed_garden_growth_bp === 'number' &&
            this.latestFrame.observed_garden_growth_bp > 0
          ) {
            payload.observed_growth_bp = Math.round(this.latestFrame.observed_garden_growth_bp);
          }
          if (
            typeof this.latestFrame.observed_garden_moisture_bp === 'number' &&
            this.latestFrame.observed_garden_moisture_bp > 0
          ) {
            payload.observed_moisture_bp = Math.round(this.latestFrame.observed_garden_moisture_bp);
          }
        }
        try {
          await window.OasisDaemonClient.sendCommand('complete_task', payload);
        } catch (err) {
          this._completedSentIds.delete(taskId);
          console.warn('[OasisUI] Failed to report complete_task to col-gatewayd:', err);
        }
      }
    }
  };

  window.OasisUI = OasisUI;

  document.addEventListener('DOMContentLoaded', () => {
    OasisUI.init();
  });
})();
