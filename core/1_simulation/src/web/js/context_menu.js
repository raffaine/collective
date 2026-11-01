/* ============================================================================
 * Oasis Layer 1b — Spatial Context Menu (web/js/context_menu.js)
 * Zero hardcoded (vx, vy, vz) coordinates! Renders whatever C++ OasisApplication
 * passes to window.OasisUI.onContextMenuData(jsonStr):
 *  - Header: Entity/Tile Name, coordinates, subsurface strata summary (y=-1...)
 *  - Universal epistemic actions always at top: [ 👁️ Look ] & [ 🔍 Inspect ]
 *  - Capability- & Hauling-Filtered Affordances below divider
 * ============================================================================ */
(function () {
  'use strict';

  const TOOL_LABELS = {
    bucket_10l: '🪣 Uses Bucket',
    BUCKET_LIQUID_HAUL: '🪣 Uses Bucket',
     LIQUID_VESSEL: '🪣 Uses Liquid Vessel',
    wheelbarrow_80l: '🛒 Uses Wheelbarrow',
    WHEELBARROW_DOLLY_HEAVY_HAUL: '🛒 Uses Wheelbarrow',
    hand_dolly_150kg: '🛞 Uses Hand Dolly',
    harvest_basket: '🧺 Uses Harvest Basket',
    GRANULAR_HAUL: '🧺 Uses Granular Tote',
    tiffin_carrier: '🍱 Uses Tiffin Carrier',
    TIFFIN_CARRIER_HAUL: '🍱 Uses Tiffin Carrier'
  };

  function escapeHtml(str) {
    return String(str ?? '')
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;');
  }

  function formatDurationLabel(aff) {
    if (aff.duration_label) return String(aff.duration_label);
    if (typeof aff.duration_sim_min === 'number' && aff.duration_sim_min > 0) {
      return `${Math.round(aff.duration_sim_min)} sim-min`;
    }
    if (typeof aff.duration_sim_s === 'number' && aff.duration_sim_s > 0) {
      if (aff.duration_sim_s >= 60) {
        const mins = Math.round((aff.duration_sim_s / 60) * 10) / 10;
        return `${mins} sim-min`;
      }
      return `${Math.round(aff.duration_sim_s)} sim-s`;
    }
    return 'Instant';
  }

  function formatCapabilityTag(tag) {
    if (!tag) return '';
    return String(tag)
      .replace(/^cap\./i, '')
      .replace(/_/g, ' ')
      .replace(/\b\w/g, (ch) => ch.toUpperCase());
  }

  function resolveHaulBadge(aff) {
    if (aff.hauling_badge) return String(aff.hauling_badge);
    const key = aff.required_hauling_tool || aff.hauling_tool_id || aff.haul_modality || '';
    if (!key || key === 'NONE') return '';
    return TOOL_LABELS[key] || `🧰 Uses ${formatCapabilityTag(key)}`;
  }

  function extractCoords(data) {
    const src = data.coords || data.position || data.voxel || data;
    const x = src.x ?? src.vx ?? null;
    const y = src.y ?? src.vy ?? null;
    const z = src.z ?? src.vz ?? null;
    return { x, y, z };
  }

  function formatStrataSummary(data) {
    if (typeof data.strata_summary === 'string' && data.strata_summary.trim()) {
      return data.strata_summary.trim();
    }
    if (typeof data.subsurface_strata === 'string' && data.subsurface_strata.trim()) {
      return data.subsurface_strata.trim();
    }
    if (Array.isArray(data.strata) && data.strata.length > 0) {
      return data.strata
        .map((layer) => {
          if (typeof layer === 'string') return layer;
          const depth = layer.y !== undefined ? `y=${layer.y}: ` : '';
          return `${depth}${layer.label || layer.material || layer.name || 'Subsoil'}`;
        })
        .join(' / ');
    }
    return '';
  }

  const OasisContextMenu = {
    currentData: null,
    lastPointerInViewport: { x: 220, y: 150 },

    init() {
      const viewport = document.querySelector('.oasis-viewport-container');
      const canvas = document.getElementById('canvas');

      if (canvas) {
        canvas.addEventListener('contextmenu', (ev) => {
          ev.preventDefault();
        });
      }

      if (viewport) {
        viewport.addEventListener('pointerdown', (ev) => {
          const rect = viewport.getBoundingClientRect();
          this.lastPointerInViewport = {
            x: Math.round(ev.clientX - rect.left),
            y: Math.round(ev.clientY - rect.top)
          };
          const menuEl = document.getElementById('oasis-context-menu');
          if (menuEl && !menuEl.classList.contains('hidden') && !menuEl.contains(ev.target)) {
            this.close();
          }
        });
      }

      document.addEventListener('keydown', (ev) => {
        if (ev.key === 'Escape') {
          this.close();
        }
      });

      // Attach onContextMenuData onto window.OasisUI
      if (!window.OasisUI) {
        window.OasisUI = {};
      }
      window.OasisUI.onContextMenuData = (jsonOrObj) => {
        this.openWithData(jsonOrObj);
      };
      window.OasisUI.closeContextMenu = () => {
        this.close();
      };
    },

    close() {
      const menuEl = document.getElementById('oasis-context-menu');
      if (menuEl) {
        menuEl.classList.add('hidden');
      }
      this.currentData = null;
    },

    openWithData(jsonOrObj) {
      let data = jsonOrObj;
      if (typeof jsonOrObj === 'string') {
        if (!jsonOrObj.trim()) return;
        try {
          data = JSON.parse(jsonOrObj);
        } catch (err) {
          console.warn('[OasisContextMenu] Invalid JSON:', err);
          return;
        }
      }
      if (!data || typeof data !== 'object') return;
      if (data.close === true || data.visible === false) {
        this.close();
        return;
      }

      this.currentData = data;
      const menuEl = document.getElementById('oasis-context-menu');
      if (!menuEl) return;

      const title =
        data.display_name ||
        data.entity_name ||
        data.title ||
        data.tile_name ||
        data.archetype_id ||
        'Parcel Surface';
      const coords = extractCoords(data);
      const coordLabel =
        coords.x !== null && coords.y !== null && coords.z !== null
          ? `(${coords.x}, ${coords.y}, ${coords.z})`
          : '';
      const strataSummary = formatStrataSummary(data);

      const rawAffordances = Array.isArray(data.affordances) ? data.affordances : [];
      const isLookId = (id) => id === 'affordance.look' || id === 'aff.look';
      const isInspectId = (id) => id === 'affordance.inspect' || id === 'aff.inspect';
      const lookAff = rawAffordances.find((a) => isLookId(a.affordance_id || a.id));
      const inspectAff = rawAffordances.find((a) => isInspectId(a.affordance_id || a.id));

      const lookText =
        data.look_description ||
        (lookAff && lookAff.description) ||
        'A quiet corner of the homestead parcel.';

      const inspectMinLabel = inspectAff ? formatDurationLabel(inspectAff) : '0.5 sim-min';

      const domainAffordances = rawAffordances.filter((a) => {
        const id = a.affordance_id || a.id || '';
        return !isLookId(id) && !isInspectId(id);
      });

      const affordanceButtonsHtml = domainAffordances.length === 0
        ? '<div class="committed-empty">No physical interventions unlocked for current tools & capabilities. Use Look or Inspect above.</div>'
        : domainAffordances.map((aff, idx) => {
            const affId = aff.affordance_id || aff.id || `aff_${idx}`;
            const label = aff.label || aff.title || affId;
            const dur = formatDurationLabel(aff);
            const cap = formatCapabilityTag(aff.required_capability_tag || aff.required_capability);
            const haul = resolveHaulBadge(aff);
            const disabledReason = aff.disabled_reason || aff.locked_reason || (aff.enabled === false ? 'Requirements unmet' : '');
            const isDisabled = Boolean(disabledReason) || aff.enabled === false;

            const badges = [];
            if (cap) badges.push(`<span class="hud-tag">🛠️ ${escapeHtml(cap)}</span>`);
            if (haul) badges.push(`<span class="hud-tag l2-active">${escapeHtml(haul)}</span>`);
            if (disabledReason) {
              badges.push(`<span class="hud-tag" style="border-color:rgba(251,191,36,0.55);color:#fde68a;">🔒 ${escapeHtml(disabledReason)}</span>`);
            }

            return `
              <button
                type="button"
                class="ctx-aff-btn"
                data-aff-idx="${idx}"
                ${isDisabled ? 'disabled' : ''}
              >
                <div class="ctx-aff-top">
                  <span>${escapeHtml(label)}</span>
                  <span style="color:var(--text-muted);font-size:9px;">⏱ ${escapeHtml(dur)}</span>
                </div>
                ${badges.length > 0 ? `<div class="ctx-aff-badges">${badges.join('')}</div>` : ''}
              </button>
            `;
          }).join('');

      menuEl.innerHTML = `
        <div class="ctx-header">
          <div class="ctx-header-top">
            <span class="ctx-title">${escapeHtml(title)}</span>
            <span class="ctx-coords">${escapeHtml(coordLabel)}</span>
          </div>
          ${strataSummary ? `<div class="ctx-strata">🪨 ${escapeHtml(strataSummary)}</div>` : ''}
        </div>

        <div class="ctx-epistemic-actions">
          <button type="button" class="hud-btn" id="ctx-btn-look" title="Immediate visual glance">
            👁️ Look
          </button>
          <button type="button" class="hud-btn" id="ctx-btn-inspect" title="Walk over and observe physical properties (${escapeHtml(inspectMinLabel)})">
            🔍 Inspect (Walk &amp; Observe)
          </button>
        </div>

        <div id="ctx-epistemic-box" class="ctx-epistemic-box hidden"></div>

        <div class="ctx-divider">Available Affordances</div>
        <div class="ctx-affordance-list">
          ${affordanceButtonsHtml}
        </div>
      `;

      // Position inside viewport bounds
      this._positionMenu(menuEl, data);
      menuEl.classList.remove('hidden');

      if (window.OasisAudio) window.OasisAudio.playClick();

      // Bind universal [ 👁️ Look ]
      const lookBtn = document.getElementById('ctx-btn-look');
      const epistemicBox = document.getElementById('ctx-epistemic-box');
      if (lookBtn && epistemicBox) {
        lookBtn.addEventListener('click', () => {
          epistemicBox.textContent = lookText;
          epistemicBox.classList.remove('hidden');
          if (window.OasisAudio) window.OasisAudio.playClick();
        });
      }

      // Bind universal [ 🔍 Inspect (Walk & Observe) ]
      const inspectBtn = document.getElementById('ctx-btn-inspect');
      if (inspectBtn) {
        inspectBtn.addEventListener('click', () => {
          this._dispatchAffordance('affordance.inspect', data, coords);
          if (window.OasisAudio) window.OasisAudio.playInspect();
          this.close();
        });
      }

      // Bind domain affordances
      menuEl.querySelectorAll('[data-aff-idx]').forEach((btn) => {
        btn.addEventListener('click', () => {
          const idx = Number(btn.getAttribute('data-aff-idx'));
          const aff = domainAffordances[idx];
          if (!aff) return;
          const affId = aff.affordance_id || aff.id;
          this._playAffordanceSound(affId);
          this._dispatchAffordance(affId, data, coords, aff);
          this.close();
        });
      });
    },

    _positionMenu(menuEl, data) {
      const viewport = document.querySelector('.oasis-viewport-container');
      const vw = viewport ? viewport.clientWidth : 1024;
      const vh = viewport ? viewport.clientHeight : 720;

      const rawX = data.screen_x ?? data.mouse_x ?? data.client_x ?? this.lastPointerInViewport.x;
      const rawY = data.screen_y ?? data.mouse_y ?? data.client_y ?? this.lastPointerInViewport.y;

      const menuW = 302;
      const menuH = 320;
      const clampedX = Math.max(10, Math.min(vw - menuW - 10, Number(rawX) + 8));
      const clampedY = Math.max(10, Math.min(vh - menuH - 10, Number(rawY) + 8));

      menuEl.style.left = `${clampedX}px`;
      menuEl.style.top = `${clampedY}px`;
    },

    _dispatchAffordance(affordanceId, data, coords, affObj) {
      if (!window.OasisUI || typeof window.OasisUI.enqueueEngineCommand !== 'function') return;
      const cmd = {
        type: 'execute_affordance',
        action: 'execute_affordance',
        affordance_id: affordanceId,
        entity_id: data.entity_id ?? data.entity_uid ?? null,
        archetype_id: data.archetype_id || ''
      };
      if (coords.x !== null) cmd.x = coords.x;
      if (coords.y !== null) cmd.y = coords.y;
      if (coords.z !== null) cmd.z = coords.z;
      if (affObj && affObj.oasis_action_code !== undefined) {
        cmd.oasis_action_code = affObj.oasis_action_code;
      }
      window.OasisUI.enqueueEngineCommand(cmd);
    },

    _playAffordanceSound(affId) {
      if (!window.OasisAudio) return;
      const id = String(affId || '').toLowerCase();
      if (id.includes('water') || id.includes('irrig')) {
        window.OasisAudio.playWater();
      } else if (id.includes('harvest') || id.includes('collect') || id.includes('egg')) {
        window.OasisAudio.playHarvest();
      } else if (id.includes('cook') || id.includes('simmer') || id.includes('soup')) {
        window.OasisAudio.playCook();
      } else {
        window.OasisAudio.playStamp();
      }
    }
  };

  window.OasisContextMenu = OasisContextMenu;

  document.addEventListener('DOMContentLoaded', () => {
    OasisContextMenu.init();
  });
})();
