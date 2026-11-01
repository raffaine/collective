/* ============================================================================
 * Oasis Layer 6 — 4-Tab Commons Companion App (web/js/companion_l6.js)
 * Shared 100% identically between:
 *  1) In-Game L6 Companion Drawer (#l6-companion-drawer > #l6-companion-root)
 *  2) Standalone Browser Tab (web/companion/index.html > #l6-companion-root)
 * Driven 100% by live native POSIX daemons via col-gatewayd (:8090).
 * Zero fake hardcoded telemetry, needs, workflows, or balances.
 * ============================================================================ */
(function () {
  'use strict';

  const SOURCING_OPTIONS = [
    { value: 'R0_LOCAL_WORKFLOWS', label: 'R0 Local Homestead Workflows ($0)' },
    { value: 'R1_MESH_PEERS', label: 'R1 Trust-Ring Peer Clusters (BCC)' },
    { value: 'L7_LEGACY_CHANNEL', label: 'L7 Legacy Fiat Fallback' }
  ];

  const DISTRIBUTION_OPTIONS = [
    { value: 'R0_HOUSEHOLD', label: 'R0 Household Self-Provision' },
    { value: 'R1_TRUST_RING_COMMONS', label: 'R1 Trust-Ring Commons Sharing' },
    { value: 'COMMONS_PLUS_L7_BILL_OFFSET', label: 'Commons + L7 Zero-Bill Offset' }
  ];

  const RING_OPTIONS = [
    { value: 'R0_STEWARD_ONLY', label: 'R0 Self (Steward Only)' },
    { value: 'R1_PEER_CLUSTERS', label: 'R1 Peer Clusters Swarm' }
  ];

  const HAUL_LABELS = {
    NONE: '',
    GRANULAR_HAUL: '🧺 Granular Tote',
    BUCKET_LIQUID_HAUL: '🪣 10L Liquid Bucket',
    WHEELBARROW_DOLLY_HEAVY_HAUL: '🛒 Wheelbarrow / Dolly',
    TIFFIN_CARRIER_HAUL: '🍱 Stainless Tiffin'
  };

  function escapeHtml(str) {
    return String(str ?? '')
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;');
  }

  function formatExergyMj(uj) {
    const val = Number(uj || 0) / 1e9;
    return `${val.toFixed(2)} MJ`;
  }

  function formatSignedExergyMj(uj) {
    const val = Number(uj || 0) / 1e9;
    return `${val >= 0 ? '+' : ''}${val.toFixed(2)} MJ`;
  }

  function formatUsdCents(cents, showPlus) {
    const n = Number(cents || 0) / 100;
    const sign = n < 0 ? '-' : (showPlus && n > 0 ? '+' : '');
    return `${sign}$${Math.abs(n).toFixed(2)}`;
  }

  function sourcingPillClass(mode) {
    if (mode === 'R0_LOCAL_WORKFLOWS') return 'emerald';
    if (mode === 'R1_MESH_PEERS') return 'cyan';
    return 'amber';
  }

  function humanizeEnum(val) {
    return String(val || '')
      .replace(/_/g, ' ')
      .toLowerCase()
      .replace(/\b\w/g, (c) => c.toUpperCase())
      .replace(/\bR0\b/g, 'R0')
      .replace(/\bR1\b/g, 'R1')
      .replace(/\bL7\b/g, 'L7')
      .replace(/\bL2\b/g, 'L2');
  }

  function renderSelectOptions(options, selectedVal) {
    return options
      .map((opt) => {
        const sel = opt.value === selectedVal ? 'selected' : '';
        return `<option value="${escapeHtml(opt.value)}" ${sel}>${escapeHtml(opt.label)}</option>`;
      })
      .join('');
  }

  function hasFocusedInputInside(container) {
    if (!container || !document.activeElement) return false;
    const tag = document.activeElement.tagName;
    if (tag !== 'INPUT' && tag !== 'SELECT' && tag !== 'TEXTAREA') return false;
    return container.contains(document.activeElement);
  }

  const OasisCompanionL6 = {
    activeTab: 'needs',
    isStandalone: false,
    searchQuery: '',
    defineFormOpen: false,
    decisionDrafts: {},
    assetDrafts: {},
    promptDrafts: {},
    needScheduleDays: {},
    needQuantities: {},
    _sectionHashes: {},
    _toastTimer: null,

    init() {
      const root = document.getElementById('l6-companion-root');
      if (!root) return;

      this.isStandalone = document.body.classList.contains('l6-standalone-body') ||
        !document.getElementById('l6-companion-drawer');

      this._mountShell(root);

      if (window.OasisDaemonClient) {
        window.OasisDaemonClient.subscribe((snapshot, isOnline) => {
          this.renderSnapshot(snapshot, isOnline);
        });
      }
    },

    _mountShell(root) {
      const standaloneUrl = this.isStandalone ? 'index.html' : 'web/companion/index.html';
      const headerRight = this.isStandalone
        ? `<span class="daemon-badge offline daemon-status-badge">Connecting to :8090...</span>`
        : `
          <a href="${standaloneUrl}" target="_blank" rel="noopener" class="hud-btn" style="text-decoration:none;">↗ Tab</a>
          <button type="button" class="hud-btn" id="l6-close-drawer-btn" title="Close Companion Drawer">✕</button>
        `;

      root.innerHTML = `
        <div class="l6-shell-root">
          <div class="l6-header">
            <div>
              <div class="l6-header-title">📱 L6 COMMONS COMPANION</div>
              <div id="l6-node-submeta" style="font-family:var(--font-mono);font-size:8.8px;color:var(--text-muted);margin-top:2px;">
                Awaiting col-gatewayd (:8090) snapshot...
              </div>
            </div>
            <div class="l6-header-meta">
              ${headerRight}
            </div>
          </div>

          <div class="l6-tab-bar" role="tablist">
            <button type="button" class="l6-tab-btn active" data-l6-tab="needs">🔍 Needs &amp; Requests</button>
            <button type="button" class="l6-tab-btn" data-l6-tab="decisions" id="l6-tab-btn-decisions">⚖️ L5 Decisions</button>
            <button type="button" class="l6-tab-btn" data-l6-tab="workflows">📋 L4 Workflows</button>
            <button type="button" class="l6-tab-btn" data-l6-tab="node">🏛️ Cluster, L3 &amp; Bills</button>
          </div>

          <div id="l6-cmd-toast" class="l6-status-toast hidden"></div>

          <div class="l6-tab-viewport">
            <div id="l6-offline-banner" class="l6-offline-banner hidden">
              <div style="font-weight:800;margin-bottom:4px;">🟠 Native POSIX Daemons Offline (:8090)</div>
              <div>Start the 6 native daemons to populate live L2–L7 state:</div>
              <code style="display:block;margin:6px 0;padding:5px 7px;background:rgba(0,0,0,0.45);border-radius:4px;color:#a7f3d0;">./scripts/run_native_daemons.sh start</code>
              <button type="button" class="l6-btn amber" id="l6-retry-conn-btn">↻ Retry Connection Now</button>
            </div>

            <!-- TAB 1: NEEDS & REQUESTS -->
            <div class="l6-tab-panel active" id="l6-panel-needs">
              <div class="l6-card" id="l6-search-bar-box">
                <div style="display:flex;gap:6px;align-items:center;">
                  <input
                    type="text"
                    id="l6-need-search-input"
                    class="l6-input"
                    placeholder="Search semantic mesh needs (e.g. pea soup, compost, eggs, joinery)..."
                  />
                  <button type="button" class="l6-btn secondary" id="l6-need-search-btn">Search</button>
                  <button type="button" class="l6-btn" id="l6-toggle-define-btn" style="white-space:nowrap;">+ Define Need</button>
                </div>
              </div>

              <div class="l6-card emerald hidden" id="l6-define-need-card">
                <div class="l6-card-header">
                  <span class="l6-card-title">✨ Define New Semantic Need for Mesh</span>
                  <button type="button" class="hud-btn" id="l6-close-define-btn">✕</button>
                </div>
                <div class="l6-card-desc">
                  Registers a semantic JSON-LD need in <code>col-commonsd</code>, binds L7 fallback &amp; L3 exergy budget, instantiates an initial L5 asset model, and emits an L5 Governance Decision.
                </div>
                <div class="l6-form-grid">
                  <div>
                    <label class="l6-kv-label" for="def-need-title">Need Title</label>
                    <input type="text" id="def-need-title" class="l6-input" placeholder="e.g. Neighborhood Herbal Tea" />
                  </div>
                  <div>
                    <label class="l6-kv-label" for="def-need-cat">Category</label>
                    <input type="text" id="def-need-cat" class="l6-input" value="Community Agroecology" />
                  </div>
                  <div>
                    <label class="l6-kv-label" for="def-need-exergy">Exergy Budget (MJ)</label>
                    <input type="number" id="def-need-exergy" class="l6-input" value="2.5" min="0.1" step="0.1" />
                  </div>
                  <div>
                    <label class="l6-kv-label" for="def-need-l7cost">L7 Legacy Unit Cost ($ USD)</label>
                    <input type="number" id="def-need-l7cost" class="l6-input" value="12.00" min="0" step="0.50" />
                  </div>
                  <div>
                    <label class="l6-kv-label" for="def-need-l7label">L7 Legacy Channel</label>
                    <input type="text" id="def-need-l7label" class="l6-input" value="L7 Retail Specialty Grocer" />
                  </div>
                  <div>
                    <label class="l6-kv-label" for="def-need-locallabel">Local Stewarded Channel</label>
                    <input type="text" id="def-need-locallabel" class="l6-input" value="Local Herb Drying Rack & Hearth ($0.00)" />
                  </div>
                </div>
                <div class="l6-btn-row">
                  <button type="button" class="l6-btn" id="l6-submit-define-need-btn">🚀 Register Need &amp; Trigger L5 Decision</button>
                </div>
              </div>

              <div class="l6-section-label">Known Mesh Needs (col-commonsd · pea_to_soup_needs.jsonld)</div>
              <div id="l6-needs-list-container"></div>

              <div class="l6-section-label">Active &amp; Fulfilled L6 Requests</div>
              <div id="l6-requests-list-container"></div>
            </div>

            <!-- TAB 2: L5 DECISIONS -->
            <div class="l6-tab-panel" id="l6-panel-decisions">
              <div class="l6-section-label">Pending L5 Governance Proposals (col-kmsd)</div>
              <div id="l6-decisions-pending-container"></div>

              <div class="l6-section-label">Stewarded L5 Assets &amp; Live Policy Controls</div>
              <div id="l6-assets-list-container"></div>
            </div>

            <!-- TAB 3: L4 WORKFLOWS & TASKS -->
            <div class="l6-tab-panel" id="l6-panel-workflows">
              <div class="l6-section-label">BPMN 2.0 Token-Sequenced Workflows &amp; Cross-Node Choreography (col-execd)</div>
              <div id="l6-workflows-list-container"></div>
            </div>

            <!-- TAB 4: CLUSTER, L3 & BILLS -->
            <div class="l6-tab-panel" id="l6-panel-node">
              <div class="l6-section-label">Lot 402 Personal Homestead Cluster (4 Sovereign Nodes · R0)</div>
              <div id="l6-cluster-nodes-container"></div>

              <div class="l6-section-label">L7 Monthly Bill-Offset &amp; Household Reserve (col-adversaryd)</div>
              <div id="l6-l7-bills-container"></div>

              <div class="l6-section-label" id="l6-l3-bcc-section-label">L3 Inter-Node Choreography &amp; Exergy Ledger</div>
              <div id="l6-l3-bcc-container"></div>

              <div class="l6-section-label" id="l6-l3-weather-section-label">Live L3 Inter-Node Choreography &amp; Mesh Event Feed</div>
              <div id="l6-l3-weather-container"></div>

              <div class="l6-section-label">Active L2 Hardware &amp; Virtual Pin Registers (col-telemetryd)</div>
              <div id="l6-l2-registers-container"></div>
            </div>
          </div>

          <div class="l6-footer-links">
            <span>Gateway RPC: <code>http://127.0.0.1:8090/api/v1</code></span>
            <span>Zero Fake Telemetry · Live POSIX Stack</span>
          </div>
        </div>
      `;

      this._bindShellEvents(root);
    },

    _bindShellEvents(root) {
      root.querySelectorAll('[data-l6-tab]').forEach((btn) => {
        btn.addEventListener('click', () => {
          this.switchTab(btn.getAttribute('data-l6-tab'));
        });
      });

      const closeBtn = document.getElementById('l6-close-drawer-btn');
      if (closeBtn) {
        closeBtn.addEventListener('click', () => {
          if (window.OasisUI && typeof window.OasisUI.toggleCompanionDrawer === 'function') {
            window.OasisUI.toggleCompanionDrawer(false);
          }
        });
      }

      const retryBtn = document.getElementById('l6-retry-conn-btn');
      if (retryBtn) {
        retryBtn.addEventListener('click', () => {
          if (window.OasisDaemonClient) window.OasisDaemonClient.pollOnce();
        });
      }

      const searchInput = document.getElementById('l6-need-search-input');
      const searchBtn = document.getElementById('l6-need-search-btn');
      const runSearch = async () => {
        this.searchQuery = searchInput ? searchInput.value.trim() : '';
        this._sectionHashes.needs = '';
        if (window.OasisDaemonClient && window.OasisDaemonClient.isOnline) {
          try {
            await window.OasisDaemonClient.sendCommand('search_needs', { query: this.searchQuery });
          } catch (_) {}
        } else if (window.OasisDaemonClient && window.OasisDaemonClient.latestSnapshot) {
          this.renderSnapshot(window.OasisDaemonClient.latestSnapshot, window.OasisDaemonClient.isOnline);
        }
      };

      if (searchBtn) searchBtn.addEventListener('click', runSearch);
      if (searchInput) {
        searchInput.addEventListener('input', () => {
          this.searchQuery = searchInput.value.trim();
          this._sectionHashes.needs = '';
          if (window.OasisDaemonClient && window.OasisDaemonClient.latestSnapshot) {
            this._renderNeedsTab(window.OasisDaemonClient.latestSnapshot);
          }
        });
        searchInput.addEventListener('keydown', (ev) => {
          if (ev.key === 'Enter') runSearch();
        });
      }

      const toggleDefBtn = document.getElementById('l6-toggle-define-btn');
      const closeDefBtn = document.getElementById('l6-close-define-btn');
      const defCard = document.getElementById('l6-define-need-card');
      const setDefineOpen = (open) => {
        this.defineFormOpen = open;
        if (defCard) defCard.classList.toggle('hidden', !open);
      };
      if (toggleDefBtn) toggleDefBtn.addEventListener('click', () => setDefineOpen(!this.defineFormOpen));
      if (closeDefBtn) closeDefBtn.addEventListener('click', () => setDefineOpen(false));

      const submitDefBtn = document.getElementById('l6-submit-define-need-btn');
      if (submitDefBtn) {
        submitDefBtn.addEventListener('click', async () => {
          const titleEl = document.getElementById('def-need-title');
          const catEl = document.getElementById('def-need-cat');
          const exEl = document.getElementById('def-need-exergy');
          const costEl = document.getElementById('def-need-l7cost');
          const l7El = document.getElementById('def-need-l7label');
          const locEl = document.getElementById('def-need-locallabel');

          const title = titleEl ? titleEl.value.trim() : '';
          if (!title) {
            this.showToast('Enter a Need Title first.', true);
            return;
          }

          const mj = Math.max(0.1, Number(exEl ? exEl.value : 2.5) || 2.5);
          const usd = Math.max(0, Number(costEl ? costEl.value : 12.0) || 12.0);

          await this.dispatchDaemonCommand(
            'define_need',
            {
              title: title,
              category: (catEl && catEl.value.trim()) || 'Community Defined Commons',
              l7_channel_label: (l7El && l7El.value.trim()) || 'L7 Legacy Supply Channel',
              local_channel_label: (locEl && locEl.value.trim()) || 'Local L5 Stewarded Workflow ($0.00)',
              exergy_cost_uj: Math.round(mj * 1e9),
              l7_unit_cost_cents: Math.round(usd * 100)
            },
            `Defined "${title}" in col-commonsd and emitted a new L5 Governance Decision!`
          );

          if (titleEl) titleEl.value = '';
          setDefineOpen(false);
        });
      }
    },

    switchTab(tabId) {
      if (!tabId) return;
      this.activeTab = tabId;
      document.querySelectorAll('[data-l6-tab]').forEach((btn) => {
        btn.classList.toggle('active', btn.getAttribute('data-l6-tab') === tabId);
      });
      ['needs', 'decisions', 'workflows', 'node'].forEach((id) => {
        const panel = document.getElementById(`l6-panel-${id}`);
        if (panel) panel.classList.toggle('active', id === tabId);
      });
      if (window.OasisAudio) window.OasisAudio.playClick();
    },

    showToast(msg, isWarning) {
      const toast = document.getElementById('l6-cmd-toast');
      if (!toast) return;
      toast.textContent = msg;
      toast.style.borderColor = isWarning ? 'var(--border-amber)' : 'var(--border-emerald)';
      toast.style.background = isWarning ? 'rgba(120, 53, 15, 0.88)' : 'rgba(6, 78, 59, 0.88)';
      toast.classList.remove('hidden');
      if (this._toastTimer) clearTimeout(this._toastTimer);
      this._toastTimer = window.setTimeout(() => {
        toast.classList.add('hidden');
      }, 4800);
    },

    async dispatchDaemonCommand(action, payload, successToast) {
      if (!window.OasisDaemonClient) return;
      try {
        await window.OasisDaemonClient.sendCommand(action, payload);
        if (window.OasisAudio) {
          if (action === 'approve_decision' || action === 'define_need') {
            window.OasisAudio.playApprove();
          } else {
            window.OasisAudio.playClick();
          }
        }
        if (successToast) {
          this.showToast(successToast, false);
        }
      } catch (err) {
        this.showToast(`Daemon command "${action}" failed — ensure ./scripts/run_native_daemons.sh start is running.`, true);
      }
    },

    renderSnapshot(snapshot, isOnline) {
      const offlineBanner = document.getElementById('l6-offline-banner');
      if (offlineBanner) {
        offlineBanner.classList.toggle('hidden', Boolean(isOnline));
      }

      const submeta = document.getElementById('l6-node-submeta');
      if (!snapshot) {
        if (submeta) {
          submeta.textContent = 'Offline — No fake data loaded. Start col-gatewayd on :8090.';
        }
        return;
      }

      if (submeta) {
        const modeTag = snapshot.external_r1_swarm_enabled
          ? '4-Node Cluster + External R1 Swarm'
          : 'Solo Steward 4-Node Cluster (R0)';
        const srcTag = snapshot.bootstrapped_from_jsonld ? 'JSON-LD Live' : (snapshot.bootstrap_source || 'Native');
        submeta.textContent = `${modeTag} · Sim Day ${snapshot.sim_day ?? 1} · Tick ${snapshot.tick ?? 0} · ${srcTag}`;
      }

      const pendingCount = (snapshot.l6_pending_decisions || []).filter(
        (d) => !d.status || d.status === 'PENDING'
      ).length;
      const decTabBtn = document.getElementById('l6-tab-btn-decisions');
      if (decTabBtn) {
        decTabBtn.textContent = pendingCount > 0
          ? `⚖️ L5 Decisions (${pendingCount})`
          : '⚖️ L5 Decisions';
      }

      this._renderNeedsTab(snapshot);
      this._renderDecisionsTab(snapshot);
      this._renderWorkflowsTab(snapshot);
      this._renderNodeSwarmBillsTab(snapshot);
    },

    /* ------------------------------------------------------------------------
     * TAB 1: Needs & Requests
     * ------------------------------------------------------------------------ */
    _renderNeedsTab(snapshot) {
      const needsContainer = document.getElementById('l6-needs-list-container');
      const reqContainer = document.getElementById('l6-requests-list-container');
      if (!needsContainer || !reqContainer) return;

      const allNeeds = Array.isArray(snapshot.l6_needs) ? snapshot.l6_needs : [];
      const q = this.searchQuery.toLowerCase();
      const filteredNeeds = q
        ? allNeeds.filter((n) =>
            String(n.title || '').toLowerCase().includes(q) ||
            String(n.category || '').toLowerCase().includes(q) ||
            String(n.description || '').toLowerCase().includes(q) ||
            String(n.need_id || '').toLowerCase().includes(q)
          )
        : allNeeds;

      const simDay = Number(snapshot.sim_day || 1);
      const needsHash = JSON.stringify({ q, simDay, list: filteredNeeds });

      if (!hasFocusedInputInside(needsContainer) && this._sectionHashes.needs !== needsHash) {
        this._sectionHashes.needs = needsHash;

        if (filteredNeeds.length === 0) {
          needsContainer.innerHTML = `<div class="l6-card"><div class="l6-card-desc">No matching mesh needs found${q ? ` for "${escapeHtml(this.searchQuery)}"` : ''}. Use <strong>+ Define Need</strong> above to register a new semantic need.</div></div>`;
        } else {
          needsContainer.innerHTML = filteredNeeds.map((need) => {
            const pillColor = sourcingPillClass(need.active_sourcing_mode);
            const qtyVal = this.needQuantities[need.need_id] ?? 1;
            const dueVal = this.needScheduleDays[need.need_id] ?? (simDay + 2);

            return `
              <div class="l6-card ${pillColor === 'emerald' ? 'emerald' : ''}">
                <div class="l6-card-header">
                  <div>
                    <div class="l6-card-title">${escapeHtml(need.title)}</div>
                    <div style="font-family:var(--font-mono);font-size:9px;color:var(--text-muted);">${escapeHtml(need.category)} · <code>${escapeHtml(need.need_id)}</code></div>
                  </div>
                  <span class="l6-pill ${pillColor}">${escapeHtml(humanizeEnum(need.active_sourcing_mode))}</span>
                </div>
                <div class="l6-card-desc">${escapeHtml(need.description || '')}</div>
                <div class="l6-meta-row">
                  <span class="l6-pill cyan">⚡ ${formatExergyMj(need.exergy_cost_uj)}</span>
                  <span class="l6-pill amber">L7 Fallback: ${formatUsdCents(need.l7_unit_cost_cents)}</span>
                  ${need.is_r0_baseline_care ? '<span class="l6-pill emerald">🛡️ R0 Baseline Care</span>' : ''}
                  ${need.is_custom_mesh_need ? '<span class="l6-pill violet">✨ Custom Mesh Need</span>' : ''}
                  ${need.request_count > 0 ? `<span class="l6-pill">Requested ×${need.request_count}</span>` : ''}
                </div>
                <div style="font-family:var(--font-mono);font-size:9px;color:var(--text-muted);margin-bottom:6px;">
                  Local: ${escapeHtml(need.local_channel_label || '—')} · ${escapeHtml(need.last_status_note || '')}
                </div>
                <div class="l6-btn-row">
                  <label style="font-family:var(--font-mono);font-size:9px;color:var(--text-muted);">
                    Qty
                    <input
                      type="number"
                      class="l6-input"
                      data-need-qty="${escapeHtml(need.need_id)}"
                      value="${qtyVal}"
                      min="1"
                      max="50"
                      style="width:52px;display:inline-block;padding:3px 5px;margin-left:3px;"
                    />
                  </label>
                  <button type="button" class="l6-btn" data-req-now="${escapeHtml(need.need_id)}">⚡ Request Now</button>
                  <label style="font-family:var(--font-mono);font-size:9px;color:var(--text-muted);margin-left:4px;">
                    Day
                    <input
                      type="number"
                      class="l6-input"
                      data-need-day="${escapeHtml(need.need_id)}"
                      value="${dueVal}"
                      min="${simDay}"
                      style="width:54px;display:inline-block;padding:3px 5px;margin-left:3px;"
                    />
                  </label>
                  <button type="button" class="l6-btn secondary" data-req-sched="${escapeHtml(need.need_id)}">📅 Schedule Future Day</button>
                </div>
              </div>
            `;
          }).join('');

          needsContainer.querySelectorAll('[data-need-qty]').forEach((inp) => {
            inp.addEventListener('change', () => {
              this.needQuantities[inp.getAttribute('data-need-qty')] = Math.max(1, Number(inp.value) || 1);
            });
          });

          needsContainer.querySelectorAll('[data-need-day]').forEach((inp) => {
            inp.addEventListener('change', () => {
              this.needScheduleDays[inp.getAttribute('data-need-day')] = Math.max(1, Number(inp.value) || (simDay + 1));
            });
          });

          needsContainer.querySelectorAll('[data-req-now]').forEach((btn) => {
            btn.addEventListener('click', () => {
              const needId = btn.getAttribute('data-req-now');
              const qtyInput = needsContainer.querySelector(`[data-need-qty="${CSS.escape(needId)}"]`);
              const qty = Math.max(1, Number(qtyInput ? qtyInput.value : 1) || 1);
              this.dispatchDaemonCommand(
                'request_need',
                {
                  need_id: needId,
                  timing_mode: 'NOW',
                  due_sim_day: simDay,
                  quantity_units: qty,
                  requester_did: snapshot.node_did || 'did:mesh:node01:steward'
                },
                `Requested ${needId} (×${qty}) for immediate fulfillment.`
              );
            });
          });

          needsContainer.querySelectorAll('[data-req-sched]').forEach((btn) => {
            btn.addEventListener('click', () => {
              const needId = btn.getAttribute('data-req-sched');
              const qtyInput = needsContainer.querySelector(`[data-need-qty="${CSS.escape(needId)}"]`);
              const dayInput = needsContainer.querySelector(`[data-need-day="${CSS.escape(needId)}"]`);
              const qty = Math.max(1, Number(qtyInput ? qtyInput.value : 1) || 1);
              const dueDay = Math.max(simDay, Number(dayInput ? dayInput.value : simDay + 2) || (simDay + 2));
              this.dispatchDaemonCommand(
                'request_need',
                {
                  need_id: needId,
                  timing_mode: 'SCHEDULED_FUTURE',
                  due_sim_day: dueDay,
                  quantity_units: qty,
                  requester_did: snapshot.node_did || 'did:mesh:node01:steward'
                },
                `Scheduled ${needId} (×${qty}) for Sim Day ${dueDay}.`
              );
            });
          });
        }
      }

      const requests = Array.isArray(snapshot.l6_requests) ? snapshot.l6_requests.slice().reverse() : [];
      const reqHash = JSON.stringify(requests);
      if (this._sectionHashes.requests !== reqHash) {
        this._sectionHashes.requests = reqHash;
        if (requests.length === 0) {
          reqContainer.innerHTML = '<div class="l6-card"><div class="l6-card-desc">No L6 requests submitted yet this session.</div></div>';
        } else {
          reqContainer.innerHTML = `<div class="l6-feed-list">${requests.slice(0, 12).map((req) => `
            <div class="l6-feed-item">
              <div style="display:flex;justify-content:space-between;gap:6px;">
                <strong>${escapeHtml(req.need_title || req.need_id)} ×${req.quantity_units}</strong>
                <span class="l6-pill ${sourcingPillClass(req.resolved_sourcing_mode)}">${escapeHtml(humanizeEnum(req.resolved_sourcing_mode))}</span>
              </div>
              <div style="margin-top:2px;color:var(--text-muted);">
                ${req.timing_mode === 'SCHEDULED_FUTURE' ? `Scheduled Day ${req.due_sim_day}` : 'Immediate'} · Budget ${formatExergyMj(req.exergy_budget_uj)} · ${req.fulfilled ? '✅ Fulfilled' : '⏳ Queued'}
              </div>
              ${req.fulfillment_note ? `<div style="margin-top:2px;">${escapeHtml(req.fulfillment_note)}</div>` : ''}
            </div>
          `).join('')}</div>`;
        }
      }
    },

    /* ------------------------------------------------------------------------
     * TAB 2: L5 Governance Decisions & Stewarded Assets
     * ------------------------------------------------------------------------ */
    _renderDecisionsTab(snapshot) {
      const pendingContainer = document.getElementById('l6-decisions-pending-container');
      const assetsContainer = document.getElementById('l6-assets-list-container');
      if (!pendingContainer || !assetsContainer) return;

      const cultivars = Array.isArray(snapshot.available_cultivars) ? snapshot.available_cultivars : [];
      const selectedCultivarId = snapshot.selected_cultivar_id || (cultivars[0] && cultivars[0].cultivar_id) || 'cultivar:chrono_virtual_pea';
      const findCultivarSpec = (cid) => cultivars.find((c) => c.cultivar_id === cid) || cultivars[0] || null;
      const peaWorkflow = (Array.isArray(snapshot.l4_workflows) ? snapshot.l4_workflows : []).find(
        (w) => w.workflow_id === 'wf:pea_cultivation'
      );
      const cropCurrentlySown = Boolean(peaWorkflow && peaWorkflow.crop_sown);

      const decisions = Array.isArray(snapshot.l6_pending_decisions) ? snapshot.l6_pending_decisions : [];
      const pending = decisions.filter((d) => !d.status || d.status === 'PENDING');
      const decHash = JSON.stringify({ pending, selectedCultivarId, cultivarsCount: cultivars.length });

      if (!hasFocusedInputInside(pendingContainer) && this._sectionHashes.decisions !== decHash) {
        this._sectionHashes.decisions = decHash;

        if (pending.length === 0) {
          pendingContainer.innerHTML = '<div class="l6-card emerald"><div class="l6-card-desc">✅ All L5 governance proposals have been reviewed. Define a new need or adapt any asset policy below.</div></div>';
        } else {
          pendingContainer.innerHTML = pending.map((dec) => {
            const draft = this.decisionDrafts[dec.decision_id] || {
              sourcing_mode: dec.proposed_sourcing_mode || 'R0_LOCAL_WORKFLOWS',
              distribution_policy: dec.proposed_distribution_policy || 'R1_TRUST_RING_COMMONS',
              task_release_ring: dec.proposed_task_release_ring || 'R0_STEWARD_ONLY',
              cultivar_id: selectedCultivarId
            };
            if (!draft.cultivar_id) draft.cultivar_id = selectedCultivarId;
            this.decisionDrafts[dec.decision_id] = draft;

            const isGardenDecision =
              dec.governed_asset_id === 'asset:backyard_pea_garden' ||
              dec.decision_id === 'dec:pea_cultivation';
            const chosenSpec = isGardenDecision ? findCultivarSpec(draft.cultivar_id) : null;

            const cultivarInputHtml = (isGardenDecision && cultivars.length > 0) ? `
              <div style="margin-top:4px;padding:7px 8px;border-radius:5px;background:rgba(6,78,59,0.28);border:1px solid rgba(52,211,153,0.36);">
                <label class="l6-kv-label" style="color:var(--emerald);">🌱 Workflow Input Parameter — Fabaceae Variety (<code>cultivar_id</code>)</label>
                <select class="l6-select" style="margin-top:3px;" data-dec-cultivar="${escapeHtml(dec.decision_id)}">
                  ${cultivars.map((c) => `
                    <option value="${escapeHtml(c.cultivar_id)}" ${c.cultivar_id === draft.cultivar_id ? 'selected' : ''}>
                      ${escapeHtml(c.title)} (${escapeHtml(c.maturity_label)} cycle · ${escapeHtml(c.verification_interval_label)} checks${c.is_virtual_sim_tuned ? ' · Sim-Tuned' : ''})
                    </option>
                  `).join('')}
                </select>
                ${chosenSpec ? `
                  <div class="l6-meta-row" style="margin-top:5px;margin-bottom:2px;">
                    <span class="l6-pill ${chosenSpec.is_virtual_sim_tuned ? 'violet' : 'emerald'}">
                      ${chosenSpec.is_virtual_sim_tuned ? '⚡ Virtual Sim-Tuned' : `🌿 ${escapeHtml(chosenSpec.species_binomial)}`}
                    </span>
                    <span class="l6-pill cyan">⏱️ Maturity: ${escapeHtml(chosenSpec.maturity_label)}</span>
                    <span class="l6-pill amber">🔍 Check Every ${escapeHtml(chosenSpec.verification_interval_label)}</span>
                    <span class="l6-pill">${chosenSpec.fixes_atmospheric_n2 ? '🦠 Rhizobium N₂-Fixing' : '🧪 Draws Soil Mineral N'}</span>
                    <span class="l6-pill">🌡️ ≥${((chosenSpec.min_growth_temp_mk - 273150) / 1000).toFixed(0)}°C</span>
                    <span class="l6-pill emerald">🧺 +${chosenSpec.harvest_legume_batches} Legume / +${chosenSpec.harvest_mirepoix_batches} Root</span>
                  </div>
                  <div style="font-family:var(--font-mono);font-size:8.6px;color:var(--text-secondary);">
                    ${escapeHtml(chosenSpec.agronomic_notes || '')}
                  </div>
                ` : ''}
              </div>
            ` : '';

            return `
              <div class="l6-card amber">
                <div class="l6-card-header">
                  <div>
                    <div class="l6-card-title">⚖️ ${escapeHtml(dec.title)}</div>
                    <div style="font-family:var(--font-mono);font-size:9px;color:var(--text-muted);">
                      Asset: <code>${escapeHtml(dec.governed_asset_id)}</code> · Decision <code>${escapeHtml(dec.decision_id)}</code>
                    </div>
                  </div>
                  <span class="l6-pill amber">PENDING</span>
                </div>
                <div class="l6-card-desc">${escapeHtml(dec.rationale || '')}</div>
                <div class="l6-meta-row">
                  <span class="l6-pill cyan">⚡ Saves ${formatExergyMj(dec.estimated_exergy_savings_uj)}</span>
                  <span class="l6-pill emerald">💵 Offsets ${formatUsdCents(dec.estimated_monthly_l7_savings_cents)}/mo</span>
                </div>

                <div class="l6-form-grid" style="grid-template-columns:1fr;">
                  ${cultivarInputHtml}
                  <div>
                    <label class="l6-kv-label">Sourcing Mode</label>
                    <select class="l6-select" data-dec-id="${escapeHtml(dec.decision_id)}" data-dec-field="sourcing_mode">
                      ${renderSelectOptions(SOURCING_OPTIONS, draft.sourcing_mode)}
                    </select>
                  </div>
                  <div>
                    <label class="l6-kv-label">Distribution &amp; Zero-Bill Policy</label>
                    <select class="l6-select" data-dec-id="${escapeHtml(dec.decision_id)}" data-dec-field="distribution_policy">
                      ${renderSelectOptions(DISTRIBUTION_OPTIONS, draft.distribution_policy)}
                    </select>
                  </div>
                  <div>
                    <label class="l6-kv-label">Task Release Ring</label>
                    <select class="l6-select" data-dec-id="${escapeHtml(dec.decision_id)}" data-dec-field="task_release_ring">
                      ${renderSelectOptions(RING_OPTIONS, draft.task_release_ring)}
                    </select>
                  </div>
                </div>

                <div class="l6-btn-row">
                  <button type="button" class="l6-btn" data-approve-dec="${escapeHtml(dec.decision_id)}">✅ Approve &amp; Instantiate L4 Workflows</button>
                  <button type="button" class="l6-btn secondary" data-adapt-dec="${escapeHtml(dec.decision_id)}">🛠️ Adapt Policy</button>
                  <button type="button" class="l6-btn amber" data-defer-dec="${escapeHtml(dec.decision_id)}">⏸ Defer</button>
                </div>
              </div>
            `;
          }).join('');

          pendingContainer.querySelectorAll('[data-dec-id]').forEach((sel) => {
            sel.addEventListener('change', () => {
              const id = sel.getAttribute('data-dec-id');
              const field = sel.getAttribute('data-dec-field');
              if (!this.decisionDrafts[id]) this.decisionDrafts[id] = {};
              this.decisionDrafts[id][field] = sel.value;
            });
          });

          pendingContainer.querySelectorAll('[data-dec-cultivar]').forEach((sel) => {
            sel.addEventListener('change', () => {
              const id = sel.getAttribute('data-dec-cultivar');
              if (!this.decisionDrafts[id]) this.decisionDrafts[id] = {};
              this.decisionDrafts[id].cultivar_id = sel.value;
              this.dispatchDaemonCommand(
                'set_cultivar',
                { cultivar_id: sel.value },
                `Selected garden workflow input cultivar: ${sel.value}.`
              );
            });
          });

          pendingContainer.querySelectorAll('[data-approve-dec]').forEach((btn) => {
            btn.addEventListener('click', () => {
              const id = btn.getAttribute('data-approve-dec');
              const draft = this.decisionDrafts[id] || {};
              const payload = {
                decision_id: id,
                approve: true,
                sourcing_mode: draft.sourcing_mode,
                distribution_policy: draft.distribution_policy,
                task_release_ring: draft.task_release_ring
              };
              if (draft.cultivar_id) {
                payload.cultivar_id = draft.cultivar_id;
              }
              this.dispatchDaemonCommand(
                'approve_decision',
                payload,
                `Approved ${id} and instantiated L4 BPMN workflows!`
              );
            });
          });

          pendingContainer.querySelectorAll('[data-adapt-dec]').forEach((btn) => {
            btn.addEventListener('click', () => {
              const id = btn.getAttribute('data-adapt-dec');
              const draft = this.decisionDrafts[id] || {};
              const payload = {
                decision_id: id,
                sourcing_mode: draft.sourcing_mode,
                distribution_policy: draft.distribution_policy,
                task_release_ring: draft.task_release_ring
              };
              if (draft.cultivar_id) {
                payload.cultivar_id = draft.cultivar_id;
              }
              this.dispatchDaemonCommand(
                'adapt_policy',
                payload,
                `Adapted governance policy for ${id}.`
              );
            });
          });

          pendingContainer.querySelectorAll('[data-defer-dec]').forEach((btn) => {
            btn.addEventListener('click', () => {
              const id = btn.getAttribute('data-defer-dec');
              this.dispatchDaemonCommand(
                'approve_decision',
                { decision_id: id, approve: false },
                `Deferred governance proposal ${id}.`
              );
            });
          });
        }
      }

      const assets = Array.isArray(snapshot.l5_governed_assets) ? snapshot.l5_governed_assets : [];
      const assetsHash = JSON.stringify({ assets, selectedCultivarId, cropCurrentlySown });
      if (!hasFocusedInputInside(assetsContainer) && this._sectionHashes.assets !== assetsHash) {
        this._sectionHashes.assets = assetsHash;
        assetsContainer.innerHTML = assets.map((asset) => {
          const draft = this.assetDrafts[asset.asset_id] || {
            sourcing_mode: asset.sourcing_mode,
            distribution_policy: asset.distribution_policy,
            task_release_ring: asset.task_release_ring
          };
          this.assetDrafts[asset.asset_id] = draft;
          const isGardenAsset =
            asset.asset_id === 'asset:pea_cultivation' ||
            asset.asset_id === 'asset:backyard_pea_garden';

          return `
            <div class="l6-card ${asset.local_workflows_instantiated ? 'emerald' : ''}">
              <div class="l6-card-header">
                <div>
                  <div class="l6-card-title">${escapeHtml(asset.title)}</div>
                  <div style="font-family:var(--font-mono);font-size:9px;color:var(--text-muted);">
                    <code>${escapeHtml(asset.asset_id)}</code> · ${escapeHtml(asset.governance_badge || '')}
                  </div>
                </div>
                <span class="l6-pill ${sourcingPillClass(asset.sourcing_mode)}">${escapeHtml(humanizeEnum(asset.sourcing_mode))}</span>
              </div>
              <div class="l6-meta-row">
                <span class="l6-pill cyan">Offset Potential: ${formatUsdCents(asset.monthly_l7_offset_potential_cents)}/mo</span>
                <span class="l6-pill">Maint: ${formatExergyMj(asset.maintenance_exergy_uj)}</span>
                <span class="l6-pill ${asset.local_workflows_instantiated ? 'emerald' : 'amber'}">
                  ${asset.local_workflows_instantiated ? '⚙️ L4 Workflows Active' : '⏸ Awaiting Approval'}
                </span>
              </div>
              ${(isGardenAsset && cultivars.length > 0) ? `
                <div style="margin-bottom:6px;">
                  <label class="l6-kv-label">🌱 Active Legume Cultivar Input ${cropCurrentlySown ? '(Locked while sown crop grows)' : '(Switchable prior to sowing)'}</label>
                  <select class="l6-select" style="margin-top:2px;" data-asset-cultivar="${escapeHtml(asset.asset_id)}" ${cropCurrentlySown ? 'disabled' : ''}>
                    ${cultivars.map((c) => `
                      <option value="${escapeHtml(c.cultivar_id)}" ${c.cultivar_id === selectedCultivarId ? 'selected' : ''}>
                        ${escapeHtml(c.title)} · ${escapeHtml(c.maturity_label)} cycle · ${escapeHtml(c.verification_interval_label)} checks
                      </option>
                    `).join('')}
                  </select>
                </div>
              ` : ''}
              <div class="l6-form-grid" style="grid-template-columns:1fr 1fr 1fr;">
                <select class="l6-select" data-asset-id="${escapeHtml(asset.asset_id)}" data-asset-field="sourcing_mode" title="Sourcing Mode">
                  ${renderSelectOptions(SOURCING_OPTIONS, asset.sourcing_mode)}
                </select>
                <select class="l6-select" data-asset-id="${escapeHtml(asset.asset_id)}" data-asset-field="distribution_policy" title="Distribution Policy">
                  ${renderSelectOptions(DISTRIBUTION_OPTIONS, asset.distribution_policy)}
                </select>
                <select class="l6-select" data-asset-id="${escapeHtml(asset.asset_id)}" data-asset-field="task_release_ring" title="Task Release Ring">
                  ${renderSelectOptions(RING_OPTIONS, asset.task_release_ring)}
                </select>
              </div>
              <div class="l6-btn-row">
                <button type="button" class="l6-btn secondary" data-save-asset="${escapeHtml(asset.asset_id)}">⚙️ Update Asset Policy</button>
              </div>
            </div>
          `;
        }).join('');

        assetsContainer.querySelectorAll('[data-asset-cultivar]').forEach((sel) => {
          sel.addEventListener('change', () => {
            this.dispatchDaemonCommand(
              'set_cultivar',
              { cultivar_id: sel.value },
              `Updated garden workflow cultivar input to ${sel.value}.`
            );
          });
        });

        assetsContainer.querySelectorAll('[data-asset-id]').forEach((sel) => {
          sel.addEventListener('change', () => {
            const id = sel.getAttribute('data-asset-id');
            const field = sel.getAttribute('data-asset-field');
            if (!this.assetDrafts[id]) this.assetDrafts[id] = {};
            this.assetDrafts[id][field] = sel.value;
          });
        });

        assetsContainer.querySelectorAll('[data-save-asset]').forEach((btn) => {
          btn.addEventListener('click', () => {
            const id = btn.getAttribute('data-save-asset');
            const rowSelects = assetsContainer.querySelectorAll(`[data-asset-id="${CSS.escape(id)}"]`);
            const payload = { asset_id: id };
            rowSelects.forEach((s) => {
              payload[s.getAttribute('data-asset-field')] = s.value;
            });
            const cultSel = assetsContainer.querySelector(`[data-asset-cultivar="${CSS.escape(id)}"]`);
            if (cultSel && !cultSel.disabled) {
              payload.cultivar_id = cultSel.value;
            }
            this.dispatchDaemonCommand('adapt_policy', payload, `Updated L5 governance policy for ${id}.`);
          });
        });
      }
    },

    /* ------------------------------------------------------------------------
     * TAB 3: L4 Workflows & Tasks (col-execd · Strict BPMN Token & Timer Sequencing)
     * ------------------------------------------------------------------------ */
    _renderWorkflowsTab(snapshot) {
      const container = document.getElementById('l6-workflows-list-container');
      if (!container) return;

      const workflows = Array.isArray(snapshot.l4_workflows) ? snapshot.l4_workflows : [];
      const cultivars = Array.isArray(snapshot.available_cultivars) ? snapshot.available_cultivars : [];
      const selectedCultivarId = snapshot.selected_cultivar_id || '';
      const r1SwarmEnabled = Boolean(snapshot.external_r1_swarm_enabled);
      const wfHash = JSON.stringify({ r1SwarmEnabled, selectedCultivarId, workflows });
      if (hasFocusedInputInside(container) || this._sectionHashes.workflows === wfHash) {
        return;
      }
      this._sectionHashes.workflows = wfHash;

      if (workflows.length === 0) {
        container.innerHTML = `
          <div class="l6-card">
            <div class="l6-card-title" style="margin-bottom:4px;">🌱 Clean Day-1 BPMN Slate — 0 Unsolicited Workflows</div>
            <div class="l6-card-desc">
              No L4 workflows are dumped upfront. Approve an L5 Governance Proposal in <strong>⚖️ L5 Decisions</strong> (for example, <em>Backyard Keyline Pea &amp; Mirepoix Garden</em> with your chosen <em>Fabaceae</em> cultivar) to spawn its BPMN workflow — cross-node preconditions will automatically requisition prerequisite Garage fabrication on <code>node-02:garage-fab</code>!
            </div>
          </div>
        `;
        return;
      }

      const tasksIndex = new Map();
      const workflowRank = (wf) => {
        const wTasks = Array.isArray(wf.tasks) ? wf.tasks : [];
        if (wTasks.some((t) => t.bpmn_token_state === 'ACTIVE_TOKEN')) return 0;
        if (wTasks.some((t) => t.bpmn_token_state === 'WAITING_TIMER')) return 1;
        if (wTasks.length > 0 && wTasks.every((t) => t.task_commitment_state === 'COMPLETED')) return 3;
        return 2;
      };
      const sortedWorkflows = workflows.slice().sort((a, b) => workflowRank(a) - workflowRank(b));

      container.innerHTML = sortedWorkflows.map((wf) => {
        const tasks = Array.isArray(wf.tasks) ? wf.tasks : [];
        tasks.forEach((t) => tasksIndex.set(t.task_id, t));
        const allCompleted = tasks.length > 0 && tasks.every((t) => t.task_commitment_state === 'COMPLETED');
        const isGardenWf = wf.workflow_id === 'wf:pea_cultivation';

        let gardenPhenologyBannerHtml = '';
        if (isGardenWf) {
          const activeCultId = wf.input_cultivar_id || selectedCultivarId || 'cultivar:chrono_virtual_pea';
          const cSpec = cultivars.find((c) => c.cultivar_id === activeCultId) || null;
          const growthPct = Math.min(100, Math.max(0, Number(wf.crop_growth_bp || 0) / 100));
          const maturePct = Number(wf.harvest_mature_bp || 9500) / 100;
          const moistPct = (Number(wf.estimated_moisture_bp || 0) / 100).toFixed(1);
          const nPpm = Number(wf.estimated_nitrogen_ppm || 0);
          const canSwitchCultivar = !wf.crop_sown || allCompleted;

          gardenPhenologyBannerHtml = `
            <div style="margin:6px 0 8px 0;padding:8px 9px;border-radius:6px;background:rgba(6,46,38,0.45);border:1px solid rgba(52,211,153,0.38);">
              <div style="display:flex;justify-content:space-between;align-items:center;gap:8px;flex-wrap:wrap;margin-bottom:5px;">
                <div style="font-family:var(--font-mono);font-size:9.5px;font-weight:800;color:var(--emerald);">
                  🌱 Cultivar Input: ${escapeHtml(wf.input_cultivar_title || (cSpec && cSpec.title) || activeCultId)}
                  ${wf.species_binomial ? `<span style="font-weight:400;color:var(--text-muted);">(${escapeHtml(wf.species_binomial)})</span>` : ''}
                </div>
                <span class="l6-pill ${wf.crop_growth_bp >= (wf.harvest_mature_bp || 9500) ? 'emerald' : (wf.crop_sown ? 'amber' : 'cyan')}">
                  ${escapeHtml(wf.phenology_stage_label || 'Unsown Bed')} · ${growthPct.toFixed(0)}% / ${maturePct.toFixed(0)}% Mature
                </span>
              </div>

              ${(canSwitchCultivar && cultivars.length > 0) ? `
                <div style="margin-bottom:6px;">
                  <select class="l6-select" data-wf-cultivar-select="${escapeHtml(wf.workflow_id)}" title="Select Fabaceae variety for this workflow cycle">
                    ${cultivars.map((c) => `
                      <option value="${escapeHtml(c.cultivar_id)}" ${c.cultivar_id === activeCultId ? 'selected' : ''}>
                        ${escapeHtml(c.title)} (${escapeHtml(c.maturity_label)} cycle · ${escapeHtml(c.verification_interval_label)} checks${c.is_virtual_sim_tuned ? ' · Virtual Sim-Tuned' : ''})
                      </option>
                    `).join('')}
                  </select>
                </div>
              ` : ''}

              <div class="progress-track" style="height:6px;margin-bottom:6px;">
                <div class="progress-fill" style="width:${growthPct.toFixed(1)}%;"></div>
              </div>

              <div class="l6-meta-row" style="margin-bottom:0;">
                ${wf.is_virtual_cultivar ? '<span class="l6-pill violet">⚡ Virtual Sim-Tuned Physics</span>' : ''}
                <span class="l6-pill cyan">💧 Est. VWC: ${moistPct}%</span>
                <span class="l6-pill">${wf.fixes_atmospheric_n2 ? `🦠 Rhizobium N₂-Fixing (Soil N: ${nPpm} ppm)` : `🧪 Mineral-N Draw (Soil N: ${nPpm} ppm)`}</span>
                <span class="l6-pill">🔍 Manual Checks: ${wf.verification_checks_completed ?? 0}</span>
                ${Number(wf.auto_l2_checks_completed || 0) > 0 ? `<span class="l6-pill cyan">🤖 L2 Auto Checks: ${wf.auto_l2_checks_completed}</span>` : ''}
              </div>
            </div>
          `;
        }

        const tasksHtml = tasks.map((task) => {
          const isAutoL2 = task.execution_modality === 'AUTOMATED_L2_EXECUTION';
          const haulLabel = HAUL_LABELS[task.haul_modality] || '';
          const state = task.task_commitment_state || 'UNASSIGNED';
          const tokenState = task.bpmn_token_state || (state === 'COMPLETED' ? 'COMPLETED' : 'ACTIVE_TOKEN');

          let tokenBadgeHtml = '';
          let rowStyle = '';
          if (tokenState === 'COMPLETED') {
            tokenBadgeHtml = '<span class="l6-pill emerald">✓ COMPLETED</span>';
            rowStyle = 'opacity:0.78;border-left:3px solid rgba(16,185,129,0.55);';
          } else if (tokenState === 'ACTIVE_TOKEN') {
            const claimSuffix = task.claimed_by_cluster_id ? ` · ${escapeHtml(task.claimed_by_cluster_id)}` : '';
            const stateSuffix = state !== 'UNASSIGNED' ? ` · ${escapeHtml(humanizeEnum(state))}` : '';
            tokenBadgeHtml = `<span class="l6-pill emerald">🟢 ACTIVE BPMN TOKEN${stateSuffix}${claimSuffix}</span>`;
            rowStyle = 'border-left:3px solid #10b981;background:rgba(16,185,129,0.08);';
          } else if (tokenState === 'WAITING_TIMER') {
            const remLabel = task.timer_remaining_label || `${task.timer_remaining_sim_s || 0}s`;
            tokenBadgeHtml = `<span class="l6-pill amber">⏳ BPMN TIMER (${escapeHtml(remLabel)})</span>`;
            rowStyle = 'border-left:3px solid #fbbf24;background:rgba(251,191,36,0.09);';
          } else if (tokenState === 'BLOCKED_BY_PRECONDITION') {
            tokenBadgeHtml = '<span class="l6-pill amber">🔒 BLOCKED BY PRECONDITION</span>';
            rowStyle = 'border-left:3px solid #f59e0b;background:rgba(245,158,11,0.06);opacity:0.92;';
          } else {
            tokenBadgeHtml = '<span class="l6-pill">⏳ QUEUED</span>';
            rowStyle = 'opacity:0.62;';
          }

          const sensorDetails = [];
          if (task.l2_sensor_register_id) sensorDetails.push(`Sensor: ${task.l2_sensor_register_id}`);
          if (task.l2_actuator_register_id) sensorDetails.push(`Actuator: ${task.l2_actuator_register_id}`);
          if (task.target_temp_min_mk > 0) {
            sensorDetails.push(`Target ≥ ${(task.target_temp_min_mk / 1000 - 273.15).toFixed(1)}°C`);
          }
          if (task.target_moisture_max_bp > 0) {
            sensorDetails.push(`Moisture ≤ ${(task.target_moisture_max_bp / 100).toFixed(0)}%`);
          }

          let blockerHtml = '';
          if (tokenState === 'WAITING_TIMER') {
            blockerHtml = `
              <div style="margin-top:5px;padding:5px 7px;border-radius:4px;background:rgba(120,53,15,0.40);border:1px dashed rgba(251,191,36,0.48);font-family:var(--font-mono);font-size:8.8px;color:#fde68a;">
                ⏳ ${escapeHtml(task.precondition_blocker_note || `BPMN IntermediateCatchEvent timer active — next verification check unlocks in ${task.timer_remaining_label || 'scheduled interval'}.`)}
              </div>
            `;
          } else if (tokenState === 'BLOCKED_BY_PRECONDITION' && task.precondition_blocker_note) {
            blockerHtml = `
              <div style="margin-top:5px;padding:5px 7px;border-radius:4px;background:rgba(120,53,15,0.35);border:1px solid rgba(251,191,36,0.35);font-family:var(--font-mono);font-size:8.8px;color:#fde68a;">
                🔗 ${escapeHtml(task.precondition_blocker_note)}
              </div>
            `;
          } else if (tokenState === 'WAITING_SEQUENCE_FLOW' && task.precondition_blocker_note) {
            blockerHtml = `
              <div style="margin-top:4px;font-family:var(--font-mono);font-size:8.6px;color:var(--text-muted);">
                ↳ ${escapeHtml(task.precondition_blocker_note)}
              </div>
            `;
          }

          let unmodeledHtml = '';
          if (task.has_unmodeled_micro_detail) {
            if (task.unmodeled_prompt_resolved) {
              unmodeledHtml = `
                <div style="margin-top:5px;">
                  <span class="l6-pill violet">🧩 Unmodeled Step Resolved: ${escapeHtml(task.unmodeled_prompt_answer || 'Safe Heuristic Default')}</span>
                </div>
              `;
            } else if (tokenState === 'ACTIVE_TOKEN') {
              const draftAns = this.promptDrafts[task.task_id] || '';
              unmodeledHtml = `
                <div class="l6-prompt-box">
                  <div style="font-weight:800;margin-bottom:3px;">🧩 UnmodeledStepAdapter Prompt</div>
                  <div style="margin-bottom:5px;">${escapeHtml(task.unmodeled_prompt_question || 'Specify physical micro-step heuristic or accept safe default:')}</div>
                  <div style="display:flex;gap:5px;">
                    <input
                      type="text"
                      class="l6-input"
                      data-prompt-input="${escapeHtml(task.task_id)}"
                      value="${escapeHtml(draftAns)}"
                      placeholder="Enter steward micro-detail or click Default..."
                    />
                    <button type="button" class="l6-btn amber" data-prompt-submit="${escapeHtml(task.task_id)}">Answer</button>
                    <button type="button" class="l6-btn secondary" data-prompt-default="${escapeHtml(task.task_id)}">Default</button>
                  </div>
                </div>
              `;
            }
          }

          // Strict BPMN gating: ONLY the ACTIVE_TOKEN step can be committed, executed, or released!
          const actions = [];
          if (tokenState === 'ACTIVE_TOKEN' && state !== 'COMPLETED') {
            if (state !== 'COMMITTED_BY_PLAYER') {
              actions.push(`<button type="button" class="l6-btn" data-commit-task="${escapeHtml(task.task_id)}">📌 Commit to Me</button>`);
            }
            if (state === 'COMMITTED_BY_PLAYER') {
              actions.push(`<button type="button" class="l6-btn" data-exec-oasis-task="${escapeHtml(task.task_id)}">▶ Execute in Oasis</button>`);
            }
            if (r1SwarmEnabled && state !== 'RELEASED_TO_R1_PEERS' && state !== 'CLAIMED_BY_PEER_CLUSTER') {
              actions.push(`<button type="button" class="l6-btn secondary" data-release-task="${escapeHtml(task.task_id)}">🌐 Release to R1 Peer Swarm</button>`);
            }
          }

          const hasNumericPrefix = /^\d+\./.test(String(task.title || '').trim());
          return `
            <div class="l6-task-row" style="${rowStyle}">
              <div style="display:flex;justify-content:space-between;align-items:flex-start;gap:6px;">
                <div style="font-family:var(--font-mono);font-size:10px;font-weight:800;color:var(--text-primary);">
                  ${(!hasNumericPrefix && task.step_index) ? `Step ${task.step_index}: ` : ''}${escapeHtml(task.title)}
                </div>
                ${tokenBadgeHtml}
              </div>

              <div class="l6-meta-row" style="margin-top:4px;margin-bottom:2px;">
                ${task.assigned_node_id ? `<span class="l6-pill cyan">🖥️ ${escapeHtml(task.assigned_node_id)}</span>` : ''}
                <span class="l6-pill ${isAutoL2 ? 'cyan' : ''}">
                  ${isAutoL2 ? '🤖 L2 Sensor-Automated' : '🖐️ Manual Steward Action'}
                </span>
                ${haulLabel ? `<span class="l6-pill amber">${escapeHtml(haulLabel)}</span>` : ''}
                ${task.location_label ? `<span class="l6-pill">📍 ${escapeHtml(task.location_label)}</span>` : ''}
                <span class="l6-pill">⚡ ${formatExergyMj(task.exergy_cost_uj)}</span>
                ${task.l7_revenue_offset_cents > 0 ? `<span class="l6-pill emerald">💵 ${formatUsdCents(task.l7_revenue_offset_cents, true)} L7 Offset</span>` : ''}
              </div>

              ${sensorDetails.length > 0 ? `<div style="font-family:var(--font-mono);font-size:8.8px;color:var(--cyan);margin-top:2px;">${escapeHtml(sensorDetails.join(' · '))}</div>` : ''}
              ${blockerHtml}
              ${unmodeledHtml}
              ${actions.length > 0 ? `<div class="l6-btn-row">${actions.join('')}</div>` : ''}
            </div>
          `;
        }).join('');

        return `
          <div class="l6-card ${wf.is_active ? 'emerald' : ''}">
            <div class="l6-card-header">
              <div>
                <div class="l6-card-title">⚙️ ${escapeHtml(wf.title)}</div>
                <div style="font-family:var(--font-mono);font-size:9px;color:var(--text-muted);">
                  Node <code>${escapeHtml(wf.assigned_node_id || 'node-01:office-coordinator')}</code> · Workflow <code>${escapeHtml(wf.workflow_id)}</code> · Asset <code>${escapeHtml(wf.asset_id)}</code>
                </div>
              </div>
              <div style="display:flex;align-items:center;gap:5px;">
                <span class="l6-pill ${allCompleted ? 'emerald' : (wf.is_active ? 'cyan' : '')}">
                  ${allCompleted ? 'CYCLE COMPLETE' : (wf.is_active ? 'BPMN ACTIVE' : 'PAUSED')} · Cycles: ${wf.completed_cycles ?? 0}
                </span>
                ${allCompleted ? `<button type="button" class="l6-btn secondary" data-start-wf-cycle="${escapeHtml(wf.workflow_id)}">🔄 Start Next Cycle</button>` : ''}
              </div>
            </div>
            ${gardenPhenologyBannerHtml}
            ${tasksHtml}
          </div>
        `;
      }).join('');

      container.querySelectorAll('[data-wf-cultivar-select]').forEach((sel) => {
        sel.addEventListener('change', () => {
          this.dispatchDaemonCommand(
            'set_cultivar',
            { cultivar_id: sel.value },
            `Parameterized Legume Cultivation workflow for ${sel.value}.`
          );
        });
      });

      container.querySelectorAll('[data-start-wf-cycle]').forEach((btn) => {
        btn.addEventListener('click', () => {
          const wfId = btn.getAttribute('data-start-wf-cycle');
          const cultSel = container.querySelector(`[data-wf-cultivar-select="${CSS.escape(wfId)}"]`);
          const payload = { workflow_id: wfId };
          if (cultSel) {
            payload.cultivar_id = cultSel.value;
          }
          this.dispatchDaemonCommand('start_workflow_cycle', payload, `Started a fresh BPMN token cycle for ${wfId}.`);
        });
      });

      container.querySelectorAll('[data-commit-task]').forEach((btn) => {
        btn.addEventListener('click', () => {
          const taskId = btn.getAttribute('data-commit-task');
          this.dispatchDaemonCommand('commit_task', { task_id: taskId }, `Committed active BPMN step ${taskId} to your Top-Right HUD checklist.`);
        });
      });

      container.querySelectorAll('[data-release-task]').forEach((btn) => {
        btn.addEventListener('click', () => {
          const taskId = btn.getAttribute('data-release-task');
          this.dispatchDaemonCommand('release_task_r1', { task_id: taskId }, `Released ${taskId} to External R1 Peer Clusters Swarm.`);
        });
      });

      container.querySelectorAll('[data-exec-oasis-task]').forEach((btn) => {
        btn.addEventListener('click', () => {
          const taskId = btn.getAttribute('data-exec-oasis-task');
          const task = tasksIndex.get(taskId);
          if (!task) return;
          if (window.OasisUI && typeof window.OasisUI.executeCommittedTaskObject === 'function') {
            window.OasisUI.executeCommittedTaskObject(task);
            this.showToast(`Dispatched "${task.title}" to Oasis 3D Steward.`, false);
          } else if (window.OasisDaemonClient) {
            window.OasisDaemonClient.broadcastCommittedTaskExecution(task);
            this.showToast(`Sent "${task.title}" to open Oasis 3D tab via BroadcastChannel.`, false);
          }
        });
      });

      container.querySelectorAll('[data-prompt-input]').forEach((inp) => {
        inp.addEventListener('input', () => {
          this.promptDrafts[inp.getAttribute('data-prompt-input')] = inp.value;
        });
      });

      container.querySelectorAll('[data-prompt-submit]').forEach((btn) => {
        btn.addEventListener('click', () => {
          const taskId = btn.getAttribute('data-prompt-submit');
          const inp = container.querySelector(`[data-prompt-input="${CSS.escape(taskId)}"]`);
          const answer = (inp && inp.value.trim()) || 'Verified tactile check & standard homestead practice';
          delete this.promptDrafts[taskId];
          this.dispatchDaemonCommand('answer_unmodeled_prompt', { task_id: taskId, answer: answer }, `Recorded micro-detail heuristic for ${taskId}.`);
        });
      });

      container.querySelectorAll('[data-prompt-default]').forEach((btn) => {
        btn.addEventListener('click', () => {
          const taskId = btn.getAttribute('data-prompt-default');
          delete this.promptDrafts[taskId];
          this.dispatchDaemonCommand(
            'answer_unmodeled_prompt',
            { task_id: taskId, answer: 'Standard commons ecological default applied' },
            `Applied safe default heuristic for ${taskId}.`
          );
        });
      });
    },

    /* ------------------------------------------------------------------------
     * TAB 4: 4-Node Personal Homestead Cluster, L3 Ledger & L7 Bill-Offset
     * ------------------------------------------------------------------------ */
    _renderNodeSwarmBillsTab(snapshot) {
      const clusterEl = document.getElementById('l6-cluster-nodes-container');
      const billsEl = document.getElementById('l6-l7-bills-container');
      const bccEl = document.getElementById('l6-l3-bcc-container');
      const weatherEl = document.getElementById('l6-l3-weather-container');
      const l2El = document.getElementById('l6-l2-registers-container');
      if (!billsEl || !bccEl || !weatherEl || !l2El) return;

      const nodeHash = JSON.stringify({
        mode: snapshot.cluster_mode,
        r1: snapshot.external_r1_swarm_enabled,
        phys: snapshot.homestead_physical_state,
        nodes: snapshot.homestead_cluster_nodes,
        l7: snapshot.l7_bill_offset_ledger,
        l3: snapshot.l3_ledger,
        l2: snapshot.l2_telemetry
      });
      if (this._sectionHashes.node === nodeHash) return;
      this._sectionHashes.node = nodeHash;

      const r1Enabled = Boolean(snapshot.external_r1_swarm_enabled);
      const phys = snapshot.homestead_physical_state || {};
      const clusterNodes = Array.isArray(snapshot.homestead_cluster_nodes)
        ? snapshot.homestead_cluster_nodes
        : [];

      const bccLabelEl = document.getElementById('l6-l3-bcc-section-label');
      if (bccLabelEl) {
        bccLabelEl.textContent = r1Enabled
          ? 'L3 Exergy & External R1 Pairwise BCC Peer Clusters (col-swarmd)'
          : 'L3 Inter-Node Choreography & Exergy Ledger (Solo 4-Node Mode — External R1 NPC Swarm Deferred)';
      }

      // 0. 4-Node Personal Homestead Cluster Topology (Lot 402 R0 Cluster)
      if (clusterEl) {
        clusterEl.innerHTML = `
          <div class="l6-card emerald">
            <div class="l6-card-header">
              <div>
                <div class="l6-card-title">🏡 ${r1Enabled ? '4-Node Homestead Cluster + External R1 Swarm' : 'Solo Steward 4-Node Personal Homestead Cluster'}</div>
                <div style="font-family:var(--font-mono);font-size:9px;color:var(--text-muted);">
                  One Steward coordinating 4 room-scoped Sovereign Nodes across Lot 402 (${clusterNodes.length || 4} active R0 nodes · ${r1Enabled ? '3 external R1 peer clusters online' : '0 external NPC clusters active'})
                </div>
              </div>
              <button type="button" class="l6-btn ${r1Enabled ? 'amber' : 'secondary'}" id="l6-toggle-r1-swarm-btn">
                ${r1Enabled ? '🏡 Return to Solo 4-Node Mode' : '🌐 Enable External R1 Peer Swarm'}
              </button>
            </div>

            <div class="l6-meta-row">
              <span class="l6-pill ${Number(phys.raised_bed_frames_built || 0) > 0 ? 'emerald' : ''}">🪵 Beds Milled: ${phys.raised_bed_frames_built ?? 0}</span>
              <span class="l6-pill ${Number(phys.compost_kiln_units_built || 0) > 0 ? 'emerald' : ''}">♻️ Compost/Kiln: ${phys.compost_kiln_units_built ?? 0}</span>
              <span class="l6-pill ${Number(phys.poultry_coops_built || 0) > 0 ? 'emerald' : ''}">🐔 Coops: ${phys.poultry_coops_built ?? 0}</span>
              <span class="l6-pill ${Number(phys.harvested_split_pea_batches || 0) > 0 ? 'cyan' : ''}">🌱 Pea Harvests: ${phys.harvested_split_pea_batches ?? 0}</span>
              <span class="l6-pill ${Number(phys.simmered_bone_broth_batches || 0) > 0 ? 'cyan' : ''}">🦴 Broth: ${phys.simmered_bone_broth_batches ?? 0}</span>
              <span class="l6-pill ${Number(phys.served_pea_soup_tiffin_batches || 0) > 0 ? 'emerald' : ''}">🍲 Soup Tiffins: ${phys.served_pea_soup_tiffin_batches ?? 0}</span>
            </div>

            ${clusterNodes.map((cn) => {
              const tokSummary = String(cn.active_token_summary || 'Idle');
              const isTokActive = tokSummary.startsWith('ACTIVE TOKEN:');
              const isTokBlocked = tokSummary.startsWith('Blocked:');
              const pillCls = isTokActive ? 'emerald' : (isTokBlocked ? 'amber' : '');
              const zoneText = cn.zone_label || cn.host_location_label || '';
              const daemonsText = cn.daemons_hosted || cn.hosted_daemons || '';
              return `
                <div class="l6-task-row" style="${isTokActive ? 'border-left:3px solid #10b981;' : (isTokBlocked ? 'border-left:3px solid #f59e0b;' : '')}">
                  <div style="display:flex;justify-content:space-between;align-items:flex-start;gap:6px;">
                    <div>
                      <strong style="font-family:var(--font-mono);font-size:10px;">🖥️ ${escapeHtml(cn.title)}</strong>
                      <div style="font-family:var(--font-mono);font-size:8.6px;color:var(--text-muted);">
                        <code>${escapeHtml(cn.node_id)}</code>${zoneText ? ` · 📍 ${escapeHtml(zoneText)}` : ''}${daemonsText ? ` · <code>${escapeHtml(daemonsText)}</code>` : ''}
                      </div>
                    </div>
                    <span class="l6-pill cyan">⚡ ${formatExergyMj(cn.accounted_exergy_uj)}</span>
                  </div>
                  <div style="margin-top:4px;display:flex;flex-wrap:wrap;gap:4px;align-items:center;">
                    <span class="l6-pill ${pillCls}">${escapeHtml(tokSummary)}</span>
                    <span class="l6-pill">✓ Tasks: ${cn.completed_task_count ?? 0}</span>
                    <span class="l6-pill ${Number(cn.online_l2_pin_count || 0) > 0 ? 'cyan' : ''}">📡 L2 Pins: ${cn.online_l2_pin_count ?? 0}</span>
                  </div>
                  <div style="font-family:var(--font-mono);font-size:8.6px;color:var(--text-secondary);margin-top:3px;">
                    ${escapeHtml(cn.inventory_summary || '')}
                  </div>
                </div>
              `;
            }).join('')}
          </div>
        `;

        const toggleSwarmBtn = document.getElementById('l6-toggle-r1-swarm-btn');
        if (toggleSwarmBtn) {
          toggleSwarmBtn.addEventListener('click', () => {
            this.dispatchDaemonCommand(
              'set_external_swarm',
              { enabled: !r1Enabled },
              !r1Enabled
                ? 'Enabled External R1 Neighborhood Peer Swarm alongside your 4-node cluster.'
                : 'Returned to Solo Steward 4-Node Personal Homestead Cluster mode.'
            );
          });
        }
      }

      // 1. L7 Monthly Bill-Offset Ledger (strictly inside Companion App!)
      const l7 = snapshot.l7_bill_offset_ledger || {};
      const depPct = ((Number(l7.dependency_ratio_bp || 0)) / 100).toFixed(1);
      const repPct = ((Number(l7.steward_reputation_bp || 0)) / 100).toFixed(1);

      billsEl.innerHTML = `
        <div class="l6-card ${l7.zero_net_bill_achieved ? 'emerald' : ''}">
          <div class="l6-card-header">
            <span class="l6-card-title">🏦 L7 Carapace &amp; Monthly Bill-Offset Accounting</span>
            <span class="l6-pill ${l7.zero_net_bill_achieved ? 'emerald' : 'amber'}">
              ${l7.zero_net_bill_achieved ? '🏆 ZERO-NET BILL ACHIEVED' : `L7 Dependency: ${depPct}%`}
            </span>
          </div>
          <div class="l6-kv-grid">
            <div class="l6-kv-box">
              <div class="l6-kv-label">Household Fiat Reserve</div>
              <div class="l6-kv-val">${formatUsdCents(l7.household_reserve_cents)}</div>
            </div>
            <div class="l6-kv-box">
              <div class="l6-kv-label">Net Monthly Cashflow</div>
              <div class="l6-kv-val" style="color:${Number(l7.net_monthly_cashflow_cents || 0) >= 0 ? 'var(--emerald)' : 'var(--amber)'};">
                ${formatUsdCents(l7.net_monthly_cashflow_cents, true)}/mo
              </div>
            </div>
            <div class="l6-kv-box">
              <div class="l6-kv-label">Legacy Monthly Bills</div>
              <div class="l6-kv-val">${formatUsdCents(l7.monthly_legacy_bills_cents)}/mo</div>
            </div>
            <div class="l6-kv-box">
              <div class="l6-kv-label">Commons L7 Offset Revenue</div>
              <div class="l6-kv-val" style="color:var(--emerald);">${formatUsdCents(l7.monthly_l7_offset_revenue_cents, true)}/mo</div>
            </div>
          </div>
          <div class="l6-meta-row" style="margin-bottom:0;">
            <span class="l6-pill ${l7.r0_baseline_enrolled ? 'emerald' : ''}">
              ${l7.r0_baseline_enrolled ? '🛡️ R0 Baseline Enrolled' : 'R0 Baseline Pending'}
            </span>
            <span class="l6-pill cyan">Offset Orders: ${l7.completed_l7_offset_orders ?? 0}</span>
            <span class="l6-pill">Steward Reputation: ${repPct}%</span>
          </div>
        </div>
      `;

      // 2. L3 Exergy & Pairwise BCC Peer Clusters
      const l3 = snapshot.l3_ledger || {};
      const channels = Array.isArray(l3.pairwise_bcc_channels) ? l3.pairwise_bcc_channels : [];
      const offers = Array.isArray(l3.peer_surplus_offers) ? l3.peer_surplus_offers : [];

      bccEl.innerHTML = `
        <div class="l6-card">
          <div class="l6-card-header">
            <span class="l6-card-title">⚡ Total Accounted Cluster Exergy: ${formatExergyMj(l3.total_exergy_accounted_uj)}</span>
            <span class="l6-pill ${l3.bcc_zero_sum_invariant_ok ? 'emerald' : 'amber'}">
              ${l3.bcc_zero_sum_invariant_ok ? '✅ Zero-Sum Invariant OK' : '⚠️ Invariant Check'}
            </span>
          </div>
          ${!r1Enabled ? `
            <div class="l6-card-desc" style="margin-bottom:0;">
              Running strictly on your <strong>Solo Steward 4-Node Personal Homestead Cluster</strong> (<code>node-01</code> Office Coordinator ↔ <code>node-02</code> Garage Fab ↔ <code>node-03</code> Garden Agro ↔ <code>node-04</code> Kitchen Hearth). External R1 neighborhood NPC swarm traffic is paused until you opt in above.
            </div>
          ` : channels.map((ch) => `
            <div class="l6-task-row">
              <div style="display:flex;justify-content:space-between;align-items:center;">
                <strong style="font-family:var(--font-mono);font-size:10px;">🏘️ ${escapeHtml(ch.peer_cluster_title)} (${ch.household_count} households)</strong>
                <span class="l6-pill cyan">${formatSignedExergyMj(ch.balance_local_to_peer_uj)}</span>
              </div>
              <div style="font-family:var(--font-mono);font-size:8.8px;color:var(--text-muted);margin-top:3px;">
                Cluster <code>${escapeHtml(ch.peer_cluster_id)}</code> · Cleared ${formatExergyMj(ch.total_cleared_volume_uj)} · Limit ${formatExergyMj(ch.credit_limit_uj)} · Mutual Tasks: ${ch.completed_mutual_tasks}
              </div>
            </div>
          `).join('')}

          ${r1Enabled && offers.length > 0 ? `
            <div class="l6-section-label" style="margin-top:8px;">Active R1 Peer Surplus Offers</div>
            <div class="l6-meta-row" style="margin-bottom:0;">
              ${offers.slice(0, 6).map((of) => `
                <span class="l6-pill emerald">🎁 ${escapeHtml(of.item_title)} ×${of.quantity_units} (${escapeHtml(of.cluster_id)} · ${formatExergyMj(of.exergy_value_uj)})</span>
              `).join('')}
            </div>
          ` : ''}
        </div>
      `;

      // 3. L3 Inter-Node Choreography & Mesh Event Feed
      const weather = Array.isArray(l3.mesh_weather_feed) ? l3.mesh_weather_feed : [];
      weatherEl.innerHTML = weather.length === 0
        ? '<div class="l6-card"><div class="l6-card-desc">Awaiting inter-node BPMN choreography events across Lot 402...</div></div>'
        : `<div class="l6-feed-list">${weather.slice(0, 14).map((ev) => `
            <div class="l6-feed-item">
              <div style="display:flex;justify-content:space-between;gap:6px;">
                <strong>[Tick ${ev.tick}] ${escapeHtml(ev.cluster_title || ev.cluster_id)}</strong>
                <span class="l6-pill cyan">${escapeHtml(ev.event_category || 'CLUSTER')} · ${formatSignedExergyMj(ev.exergy_delta_uj)}</span>
              </div>
              <div style="margin-top:2px;">${escapeHtml(ev.summary)}</div>
            </div>
          `).join('')}</div>`;

      // 4. Active L2 Sensor & Actuator Registers
      const l2 = snapshot.l2_telemetry || {};
      const regs = Array.isArray(l2.registers) ? l2.registers : [];
      l2El.innerHTML = regs.length === 0
        ? '<div class="l6-card"><div class="l6-card-desc">No active L2 registers reported by col-telemetryd.</div></div>'
        : `<div class="l6-card">${regs.map((reg) => {
            const readings = [];
            if (reg.temperature_mk > 0) {
              readings.push(`Temp ${(reg.temperature_mk / 1000 - 273.15).toFixed(1)}°C`);
            }
            if (reg.moisture_bp > 0 || String(reg.hardware_modality).includes('SOIL')) {
              readings.push(`Moisture ${(Number(reg.moisture_bp || 0) / 100).toFixed(1)}% VWC`);
            }
            if (reg.concentration_ppm > 0) {
              readings.push(`${reg.concentration_ppm} ppm`);
            }
            if (reg.hold_duration_ms > 0) {
              readings.push(`Hold ${(reg.hold_duration_ms / 1000).toFixed(0)}s`);
            }
            if (reg.is_actuator) {
              readings.push(`Actuator: ${reg.actuator_commanded_on ? 'ON' : 'OFF'} (${(Number(reg.actuator_level_bp || 0) / 100).toFixed(0)}%)`);
            }

            return `
              <div class="l6-task-row">
                <div style="display:flex;justify-content:space-between;align-items:center;">
                  <strong style="font-family:var(--font-mono);font-size:10px;">📡 <code>${escapeHtml(reg.register_id)}</code></strong>
                  <span class="l6-pill ${reg.is_online ? 'emerald' : 'amber'}">${reg.is_online ? 'ONLINE' : 'OFFLINE'}</span>
                </div>
                <div style="font-family:var(--font-mono);font-size:8.8px;color:var(--text-secondary);margin-top:3px;">
                  ${escapeHtml(reg.hardware_modality || 'L2_PIN')} · ${escapeHtml(readings.join(' · ') || 'Nominal')}
                </div>
              </div>
            `;
          }).join('')}</div>`;
    }
  };

  window.OasisCompanionL6 = OasisCompanionL6;

  document.addEventListener('DOMContentLoaded', () => {
    OasisCompanionL6.init();
  });
})();
