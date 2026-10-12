/**
 * The Collective Companion App — Frontend Application Controller
 * Milestone 3: Web Dashboard for Sovereign Stack POSIX Daemons & Gateway
 * Conforms to: COMPANION_APP_ARCHITECTURE.md & PROJECT.md
 */

(() => {
  'use strict';

  // =========================================================================
  // Configuration & Constants
  // =========================================================================
  const CONFIG = {
    // Determine gateway API base: if served from same origin (e.g. gateway port 8443),
    // use relative path; otherwise target localhost:8443
    apiBase: (window.location.port === '8443') ? '' : 'http://localhost:8443',
    pollIntervalMs: 2000,
    requestTimeoutMs: 1500,
    localDid: 'did:peer:z6MkhaXgBZDvotDkL5257faiz4YzPXWy2nd6Vd4u'
  };

  // Layer metadata definition for navigation and dock styling
  const LAYERS = {
    l2: { name: 'L2: HW', title: 'L2: HARDWARE CYBERNETICS', mode: 'READ-ONLY DIAGNOSTICS', color: '#10b981' },
    l3: { name: 'L3: LEDGER', title: 'L3: THERMODYNAMIC LEDGER', mode: 'READ-ONLY DIAGNOSTICS', color: '#06b6d4' },
    l4: { name: 'L4: EXEC', title: 'L4: WASMTIME EXECUTION', mode: 'READ-ONLY DETERMINISTIC', color: '#6366f1' },
    l5: { name: 'L5: GOV', title: 'L5: GOVERNANCE BRAIN', mode: 'READ-ONLY CONSENSUS', color: '#a855f7' },
    l6: { name: 'L6: INTENT', title: 'L6: SEMANTIC INTENT & AGORA', mode: 'INTERACTIVE CIVIC BOUNDARY', color: '#f59e0b' },
    l7: { name: 'L7: PROXY', title: 'L7: CARAPACE & PROXY', mode: 'READ-ONLY MONITOR', color: '#64748b' }
  };

  // =========================================================================
  // Application State
  // =========================================================================
  const state = {
    activeDeck: 'deck-hub', // 'deck-hub' | 'deck-l2' | ... | 'deck-l7'
    activeLayerKey: null,   // null | 'l2' | 'l3' | 'l4' | 'l5' | 'l6' | 'l7'
    isOnline: false,        // True if col-gatewayd responded
    isSimulation: true,     // Default to simulation mode until daemon responds
    provenance: '0x02',     // '0x02' (Root) or '0x01' (SITL)
    orderSide: 'BUY',
    hlcClock: 1782390441.0,
    vitals: {
      socPercent: 88,
      busVoltage: 48.24,
      netFluxKw: 3.42,
      cisternLiters: 8450,
      cisternPercent: 84.5,
      fuelRemaining: 4200,
      fuelMax: 10000,
      kalmanDm2: 0.042
    },
    orderbook: {
      bids: [
        { qty: 40.0, price: 3.50 },
        { qty: 25.0, price: 3.45 },
        { qty: 100.0, price: 3.30 }
      ],
      asks: [
        { price: 3.60, qty: 15.0 },
        { price: 3.65, qty: 30.0 },
        { price: 3.80, qty: 80.0 }
      ]
    },
    chores: {
      '8492': { claimed: false, title: '5x PETG Gears for Aquaponics Valve', escrow: 12.5 },
      '8495': { claimed: false, title: 'Clean Solar PV Array Strings 1-4', escrow: 8.0 },
      '8501': { claimed: false, title: 'Inspect Swale Infiltration Berm', escrow: 5.0 }
    }
  };

  // =========================================================================
  // DOM Cache
  // =========================================================================
  const dom = {
    activeDeckTitle: document.getElementById('active-deck-title'),
    activeModeTag: document.getElementById('active-mode-tag'),
    badgeProvenance: document.getElementById('badge-provenance'),
    provenanceText: document.getElementById('provenance-text'),
    badgeConnection: document.getElementById('badge-connection'),
    connectionText: document.getElementById('connection-text'),
    vitalSoc: document.getElementById('vital-soc'),
    vitalFlux: document.getElementById('vital-flux'),
    vitalCistern: document.getElementById('vital-cistern'),
    vitalHlc: document.getElementById('vital-hlc'),
    hubMetricSoc: document.getElementById('hub-metric-soc'),
    hubMetricFlux: document.getElementById('hub-metric-flux'),
    hubMetricH2o: document.getElementById('hub-metric-h2o'),
    btnNavHub: document.getElementById('btn-nav-hub'),
    btnEnterL6: document.getElementById('btn-enter-l6'),
    dockTabs: document.querySelectorAll('.dock-tab'),
    toastContainer: document.getElementById('toast-container'),
    // L2 elements
    l2Voltage: document.getElementById('l2-val-voltage'),
    l2Current: document.getElementById('l2-val-current'),
    l2Power: document.getElementById('l2-val-power'),
    l2Eff: document.getElementById('l2-val-eff'),
    l2Dm2: document.getElementById('l2-val-dm2'),
    l2FluxWave: document.getElementById('l2-flux-wave'),
    // L4 elements
    l4FuelText: document.getElementById('l4-fuel-text'),
    l4FuelBar: document.getElementById('l4-fuel-bar'),
    // L6 elements
    cdaSpread: document.getElementById('cda-spread'),
    cdaMidpoint: document.getElementById('cda-midpoint'),
    tableBids: document.getElementById('table-bids')?.querySelector('tbody'),
    tableAsks: document.getElementById('table-asks')?.querySelector('tbody'),
    formAgoraOrder: document.getElementById('form-agora-order'),
    btnSideBuy: document.getElementById('btn-side-buy'),
    btnSideSell: document.getElementById('btn-side-sell'),
    orderQty: document.getElementById('order-qty'),
    orderPrice: document.getElementById('order-price'),
    formIntentBuilder: document.getElementById('form-intent-builder'),
    intentType: document.getElementById('intent-type'),
    intentPrompt: document.getElementById('intent-prompt'),
    readbackPreview: document.getElementById('readback-preview'),
    intentTerminalLog: document.getElementById('intent-terminal-log')
  };

  // =========================================================================
  // Layer Navigation & Dock Switcher
  // =========================================================================

  /**
   * Switches the active deck view and updates dock indicators.
   * @param {string} target 'hub' or 'l2'...'l7'
   */
  function navigateTo(target) {
    const isHub = (target === 'hub' || target === 'deck-hub');
    const targetDeckId = isHub ? 'deck-hub' : (target.startsWith('deck-') ? target : `deck-${target}`);
    const layerKey = isHub ? null : target.replace('deck-', '');

    // Hide all decks and show the target deck
    document.querySelectorAll('.deck').forEach(deck => {
      if (deck.id === targetDeckId) {
        deck.classList.remove('deck-hidden');
        deck.classList.add('deck-visible');
      } else {
        deck.classList.add('deck-hidden');
        deck.classList.remove('deck-visible');
      }
    });

    state.activeDeck = targetDeckId;
    state.activeLayerKey = layerKey;

    // Update navigation breadcrumbs
    if (isHub) {
      dom.activeDeckTitle.textContent = 'MAIN NODE OVERVIEW';
      dom.activeModeTag.textContent = 'READ-ONLY OVERVIEW';
      dom.btnNavHub.style.borderColor = 'var(--color-border-focus)';
    } else if (LAYERS[layerKey]) {
      const info = LAYERS[layerKey];
      dom.activeDeckTitle.textContent = info.title;
      dom.activeModeTag.textContent = info.mode;
      dom.btnNavHub.style.borderColor = 'var(--color-border-subtle)';
    }

    // Update universal bottom dock tab text: [►L6: INTENT◄] pointer notation
    dom.dockTabs.forEach(tab => {
      const tabLayer = tab.getAttribute('data-layer');
      const labelSpan = tab.querySelector('.dock-label');
      if (tabLayer === layerKey) {
        tab.classList.add('active');
        if (labelSpan && LAYERS[tabLayer]) {
          labelSpan.textContent = `[►${LAYERS[tabLayer].name}◄]`;
        }
      } else {
        tab.classList.remove('active');
        if (labelSpan && LAYERS[tabLayer]) {
          labelSpan.textContent = `[ ${LAYERS[tabLayer].name} ]`;
        }
      }
    });

    // Scroll container to top smoothly
    window.scrollTo({ top: 0, behavior: 'smooth' });
  }

  // =========================================================================
  // Provenance Root vs SITL Switcher
  // =========================================================================

  function toggleProvenance() {
    if (state.provenance === '0x02') {
      state.provenance = '0x01';
      dom.badgeProvenance.classList.add('sitl-mode');
      dom.provenanceText.textContent = '0x01 SITL';
      showToast('Switched to SITL Simulated Forward Projection (0x01)', 'warning');
      logTerminal('Provenance set to 0x01 QUALITY_SIMULATED (Oasis SITL).');
    } else {
      state.provenance = '0x02';
      dom.badgeProvenance.classList.remove('sitl-mode');
      dom.provenanceText.textContent = '0x02 ROOT';
      showToast('Switched to ATECC608A Hardware Root of Trust (0x02)', 'success');
      logTerminal('Provenance set to 0x02 QUALITY_HARDWARE_ROOT (ATECC608A).');
    }
  }

  // =========================================================================
  // Gateway Communication & Diagnostic Polling
  // =========================================================================

  /**
   * Fetches latest layer diagnostics from col-gatewayd or falls back to simulation.
   */
  async function pollDiagnostics() {
    state.hlcClock += 1.0;
    dom.vitalHlc.textContent = state.hlcClock.toFixed(2);

    try {
      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), CONFIG.requestTimeoutMs);

      const response = await fetch(`${CONFIG.apiBase}/api/v1/layers/summary`, {
        signal: controller.signal
      });
      clearTimeout(timeoutId);

      if (response.ok) {
        const payload = await response.json();
        handleGatewayPayload(payload);
        setConnectionStatus(true);
        return;
      }
    } catch (_err) {
      // Gateway unreachable or timed out -> run local simulation step
    }

    // Offline / simulation fallback
    setConnectionStatus(false);
    runLocalSimulationStep();
  }

  /**
   * Updates state & UI from actual col-gatewayd payload.
   */
  function handleGatewayPayload(payload) {
    if (payload.l2) {
      if (payload.l2.voltage_v) state.vitals.busVoltage = payload.l2.voltage_v;
      if (payload.l2.power_w) state.vitals.netFluxKw = payload.l2.power_w / 1000.0;
      if (payload.l2.kalman_dm2) state.vitals.kalmanDm2 = payload.l2.kalman_dm2;
    }
    if (payload.l4) {
      if (payload.l4.fuel_remaining) state.vitals.fuelRemaining = payload.l4.fuel_remaining;
    }
    if (payload.l6 && payload.l6.orderbook) {
      if (payload.l6.orderbook.bids) state.orderbook.bids = payload.l6.orderbook.bids;
      if (payload.l6.orderbook.asks) state.orderbook.asks = payload.l6.orderbook.asks;
      renderOrderBook();
    }
    updateVitalsUI();
  }

  /**
   * Updates UI status badges for online / simulation fallback.
   */
  function setConnectionStatus(online) {
    if (state.isOnline === online) return;
    state.isOnline = online;
    state.isSimulation = !online;

    const dot = dom.badgeConnection.querySelector('.status-indicator-dot');
    if (online) {
      dot.className = 'status-indicator-dot online';
      dom.connectionText.textContent = '● ONLINE';
      dom.badgeConnection.title = `Connected to col-gatewayd at ${CONFIG.apiBase}`;
    } else {
      dot.className = 'status-indicator-dot simulation';
      dom.connectionText.textContent = '⚡ SIMULATION';
      dom.badgeConnection.title = 'Offline mode: local dynamic cybernetic simulation active';
    }
  }

  /**
   * Dynamic cybernetic simulation step ensuring genuine live state behavior.
   */
  function runLocalSimulationStep() {
    // Gentle natural drift in solar generation & microgrid voltage
    const jitter = (Math.random() - 0.48) * 0.04;
    state.vitals.busVoltage = Math.max(47.5, Math.min(49.2, state.vitals.busVoltage + jitter));
    state.vitals.netFluxKw = Math.max(2.8, Math.min(4.2, state.vitals.netFluxKw + jitter * 2));
    
    // Kalman residual oscillation
    state.vitals.kalmanDm2 = +(0.038 + Math.random() * 0.008).toFixed(3);

    // Consume and recycle Wasmtime execution fuel (deterministic loop)
    state.vitals.fuelRemaining -= 150;
    if (state.vitals.fuelRemaining < 1000) {
      state.vitals.fuelRemaining = 9800; // Reset tick cycle
    }

    updateVitalsUI();
    updateSparkline();
  }

  /**
   * Updates all DOM readouts from current vitals state.
   */
  function updateVitalsUI() {
    const v = state.vitals;
    dom.vitalSoc.textContent = `${v.socPercent}%`;
    dom.vitalFlux.textContent = `${v.netFluxKw >= 0 ? '+' : ''}${v.netFluxKw.toFixed(2)} kW`;
    dom.vitalCistern.textContent = `${v.cisternLiters.toLocaleString()} L`;

    if (dom.hubMetricSoc) dom.hubMetricSoc.textContent = `${v.socPercent}%`;
    if (dom.hubMetricFlux) dom.hubMetricFlux.textContent = `+${v.netFluxKw.toFixed(2)} kW`;
    if (dom.hubMetricH2o) dom.hubMetricH2o.textContent = `${v.cisternLiters.toLocaleString()} L`;

    if (dom.l2Voltage) dom.l2Voltage.textContent = `${v.busVoltage.toFixed(2)} V DC`;
    if (dom.l2Current) dom.l2Current.textContent = `+${(v.netFluxKw * 1000 / v.busVoltage).toFixed(1)} A`;
    if (dom.l2Power) dom.l2Power.textContent = `${Math.round(v.netFluxKw * 1000).toLocaleString()} W`;
    if (dom.l2Dm2) dom.l2Dm2.textContent = `${v.kalmanDm2} σ`;

    if (dom.l4FuelText && dom.l4FuelBar) {
      dom.l4FuelText.textContent = `${v.fuelRemaining.toLocaleString()} / ${v.fuelMax.toLocaleString()} instructions`;
      const pct = Math.round((v.fuelRemaining / v.fuelMax) * 100);
      dom.l4FuelBar.style.width = `${pct}%`;
    }
  }

  /**
   * Generates dynamic path coordinates for the L2 sparkline wave.
   */
  function updateSparkline() {
    if (!dom.l2FluxWave) return;
    const y1 = Math.round(40 + (Math.random() - 0.5) * 18);
    const y2 = Math.round(30 + (Math.random() - 0.5) * 15);
    const y3 = Math.round(25 + (Math.random() - 0.5) * 20);
    const y4 = Math.round(35 + (Math.random() - 0.5) * 16);
    dom.l2FluxWave.setAttribute('d', `M 0,50 Q 50,${y1} 100,${y2} T 200,${y3} T 300,${y4} T 400,${y2}`);
  }

  // =========================================================================
  // Strict Layer 6 Societal Intent Dispatch Boundary (POST /api/v1/l6/intent)
  // =========================================================================

  /**
   * The sole mutating dispatch function in the companion application.
   * Dispatches signed JSON-LD / Agora payloads exclusively to POST /api/v1/l6/intent.
   * Diagnostic layers (L2, L3, L4, L5, L7) NEVER have mutating dispatch calls!
   *
   * @param {string} intentType E.g. 'AGORA_ORDER', 'ResourceRequestIntent', 'CHORE_CLAIM'
   * @param {object} payload Intent specific payload
   */
  async function dispatchL6Intent(intentType, payload) {
    const timestampNs = Date.now() * 1000000;
    const intentObject = {
      intent_type: intentType,
      author_did: CONFIG.localDid,
      provenance: state.provenance,
      timestamp_ns: timestampNs,
      payload: payload
    };

    logTerminal(`[DISPATCH] Packaging ${intentType} with DID ${CONFIG.localDid.substring(0, 16)}...`);

    try {
      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), CONFIG.requestTimeoutMs);

      const res = await fetch(`${CONFIG.apiBase}/api/v1/l6/intent`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(intentObject),
        signal: controller.signal
      });
      clearTimeout(timeoutId);

      if (res.ok) {
        const ack = await res.json();
        const cid = ack.intent_cid || `bafyreib${Math.random().toString(36).substring(2, 10)}`;
        showToast(`Intent Dispatched & Ratified: ${cid}`, 'success');
        logTerminal(`[ACK 200] Intent Accepted by col-gatewayd. CID: ${cid}`, 'ack');
        pollDiagnostics();
        return;
      } else {
        const errJson = await res.json().catch(() => ({}));
        showToast(`Gateway Rejected Intent: ${errJson.message || res.statusText}`, 'error');
        logTerminal(`[ERR ${res.status}] ${errJson.message || 'Intent rejected'}`, 'err');
        return;
      }
    } catch (_networkErr) {
      // Offline fallback: handle intent in local simulation mode
      handleLocalSimulationIntent(intentObject);
    }
  }

  /**
   * Simulates immediate local settlement when daemon is offline,
   * keeping the UI fully interactive and testable.
   */
  function handleLocalSimulationIntent(intent) {
    const syntheticCid = `bafyreib${Math.random().toString(36).substring(2, 10)}`;
    logTerminal(`[SIMULATION ACK] POST /api/v1/l6/intent accepted locally. Synthetic CID: ${syntheticCid}`, 'ack');
    showToast(`[SIMULATED ACK] Intent Dispatched: ${syntheticCid}`, 'success');

    if (intent.intent_type === 'AGORA_ORDER') {
      const { side, quantity_kwh, price_exergy_mj } = intent.payload;
      if (side === 'BUY') {
        state.orderbook.bids.unshift({ qty: quantity_kwh, price: price_exergy_mj });
        state.orderbook.bids.sort((a, b) => b.price - a.price);
        if (state.orderbook.bids.length > 5) state.orderbook.bids.pop();
      } else {
        state.orderbook.asks.unshift({ price: price_exergy_mj, qty: quantity_kwh });
        state.orderbook.asks.sort((a, b) => a.price - b.price);
        if (state.orderbook.asks.length > 5) state.orderbook.asks.pop();
      }
      renderOrderBook();
    } else if (intent.intent_type === 'CHORE_CLAIM') {
      const choreId = intent.payload.chore_id;
      if (state.chores[choreId]) {
        state.chores[choreId].claimed = true;
        const choreCard = document.getElementById(`chore-${choreId}`);
        if (choreCard) {
          const btn = choreCard.querySelector('.btn-claim-chore');
          if (btn) {
            btn.disabled = true;
            btn.style.opacity = '0.6';
            btn.innerHTML = '<span>[ CLAIMED — PENDING L5 APPROVAL ]</span>';
          }
        }
      }
    }
  }

  // =========================================================================
  // Layer 6 UI Components & Form Handlers
  // =========================================================================

  /**
   * Renders the CDA orderbook table rows and recalculates midpoint.
   */
  function renderOrderBook() {
    if (!dom.tableBids || !dom.tableAsks) return;

    dom.tableBids.innerHTML = state.orderbook.bids.map(b => `
      <tr>
        <td>${b.qty.toFixed(1)}</td>
        <td class="text-emerald">${b.price.toFixed(2)}</td>
      </tr>
    `).join('');

    dom.tableAsks.innerHTML = state.orderbook.asks.map(a => `
      <tr>
        <td class="text-amber">${a.price.toFixed(2)}</td>
        <td>${a.qty.toFixed(1)}</td>
      </tr>
    `).join('');

    const bestBid = state.orderbook.bids[0]?.price || 3.50;
    const bestAsk = state.orderbook.asks[0]?.price || 3.60;
    const spread = Math.max(0, bestAsk - bestBid);
    const midpoint = (bestBid + bestAsk) / 2.0;

    if (dom.cdaSpread) dom.cdaSpread.textContent = `${spread.toFixed(2)} MJ`;
    if (dom.cdaMidpoint) dom.cdaMidpoint.textContent = `${midpoint.toFixed(2)} MJ/kWh`;
  }

  /**
   * Limit Order Form Submission handler.
   */
  function handleOrderFormSubmit(e) {
    e.preventDefault();
    const qty = parseFloat(dom.orderQty ? dom.orderQty.value : 0);
    const price = parseFloat(dom.orderPrice ? dom.orderPrice.value : 0);

    if (isNaN(qty) || qty <= 0 || isNaN(price) || price <= 0) {
      showToast('Please enter valid positive quantity and price.', 'warning');
      return;
    }

    dispatchL6Intent('AGORA_ORDER', {
      side: state.orderSide,
      quantity_kwh: qty,
      price_exergy_mj: price
    });
  }

  /**
   * Semantic Intent Form Submission handler.
   */
  function handleIntentFormSubmit(e) {
    e.preventDefault();
    const type = dom.intentType ? dom.intentType.value : 'ResourceRequestIntent';
    const prompt = dom.intentPrompt ? dom.intentPrompt.value.trim() : '';

    if (!prompt) {
      showToast('Please specify your natural language intent.', 'warning');
      return;
    }

    dispatchL6Intent(type, {
      prompt: prompt,
      readback: dom.readbackPreview ? dom.readbackPreview.textContent.trim() : '',
      compiled_schema: {
        '@type': type,
        'creator': CONFIG.localDid,
        'actionStatus': 'DRAFT_AUTHORIZED'
      }
    });
  }

  /**
   * Updates deterministic read-back preview on input changes.
   */
  function updateReadbackPreview() {
    if (!dom.readbackPreview || !dom.intentPrompt) return;
    const prompt = (dom.intentPrompt.value || '').trim();
    const type = dom.intentType ? dom.intentType.value : 'ResourceRequestIntent';

    if (type === 'ResourceRequestIntent') {
      dom.readbackPreview.textContent = prompt
        ? `Deterministic Translation: Authorize transfer matching "${prompt}" evaluated against current microgrid bus telemetry and continuous double-auction clearing midpoint.`
        : 'Awaiting prompt specification...';
    } else if (type === 'PolicyRefinement') {
      dom.readbackPreview.textContent = `Policy Rule: Refine bioregional ecological margin thresholds per intent: "${prompt}".`;
    } else if (type === 'TreatyRatificationIntent') {
      dom.readbackPreview.textContent = `Ricardian Covenant: Ratify mutual credit agreement: "${prompt}" with Ed25519 cryptographic hash verification.`;
    } else if (type === 'ChoreBountyIntent') {
      dom.readbackPreview.textContent = `Chore Escrow: Authorize PoTW escrow lock for task: "${prompt}".`;
    }
  }

  /**
   * Quick Template Selector.
   */
  function handleQuickTemplate(templateKey) {
    if (templateKey === 'solar') {
      dom.intentType.value = 'ResourceRequestIntent';
      dom.intentPrompt.value = 'Transfer 15 kWh surplus solar to Ridge Cistern if battery > 85%';
    } else if (templateKey === 'water') {
      dom.intentType.value = 'PolicyRefinement';
      dom.intentPrompt.value = 'Enforce instream ecological water floor 80% before enabling agricultural irrigation flume.';
    } else if (templateKey === 'treaty') {
      dom.intentType.value = 'TreatyRatificationIntent';
      dom.intentPrompt.value = 'Ratify seasonal gravity water sharing treaty with Valley Hearth up to 150 MJ bilateral credit.';
    }
    updateReadbackPreview();
  }

  /**
   * Chore Bounty Claim handler.
   */
  function handleChoreClaim(button) {
    const choreId = button.getAttribute('data-chore-id');
    const choreTitle = button.getAttribute('data-chore-title');
    const escrow = parseFloat(button.getAttribute('data-escrow'));

    dispatchL6Intent('CHORE_CLAIM', {
      chore_id: choreId,
      task_title: choreTitle,
      escrow_mj: escrow,
      claimant_did: CONFIG.localDid
    });
  }

  // =========================================================================
  // Logging & Toast Notifications
  // =========================================================================

  function logTerminal(msg, category = 'dispatch') {
    if (!dom.intentTerminalLog || !dom.intentTerminalLog.appendChild) return;
    const entry = document.createElement('div');
    const timeStr = new Date().toTimeString().split(' ')[0];
    entry.className = `log-entry log-${category}`;
    entry.textContent = `[${timeStr}] ${msg}`;
    dom.intentTerminalLog.appendChild(entry);
    dom.intentTerminalLog.scrollTop = dom.intentTerminalLog.scrollHeight;
  }

  function showToast(message, type = 'success') {
    if (!dom.toastContainer || !dom.toastContainer.appendChild) return;
    const toast = document.createElement('div');
    toast.className = `toast toast-${type}`;
    toast.innerHTML = `
      <span>${message}</span>
      <span style="opacity: 0.6; cursor: pointer;">✕</span>
    `;
    const closeBtn = toast.querySelector ? toast.querySelector('span:last-child') : null;
    if (closeBtn) closeBtn.onclick = () => (toast.remove ? toast.remove() : null);
    dom.toastContainer.appendChild(toast);

    setTimeout(() => {
      toast.style.opacity = '0';
      toast.style.transform = 'translateY(10px)';
      toast.style.transition = 'all 0.3s ease';
      setTimeout(() => toast.remove(), 300);
    }, 3500);
  }

  // =========================================================================
  // Boundary Protection Attestation: Verify Read-Only Diagnostic Decks
  // =========================================================================

  /**
   * Architectural invariant check: confirms that no mutating controls
   * exist in Layer 2, Layer 3, Layer 4, Layer 5, or Layer 7 decks.
   */
  function verifyDiagnosticBoundaries() {
    const diagnosticDecks = ['deck-l2', 'deck-l3', 'deck-l4', 'deck-l5', 'deck-l7'];
    let violationCount = 0;

    diagnosticDecks.forEach(deckId => {
      const deckEl = document.getElementById(deckId);
      if (!deckEl) return;
      // Find all buttons or forms
      const forms = deckEl.querySelectorAll('form');
      const actionButtons = deckEl.querySelectorAll('button:not([data-jump])');
      if (forms.length > 0) {
        console.error(`Boundary violation: Form found in read-only deck ${deckId}`);
        violationCount++;
      }
      if (actionButtons.length > 0) {
        console.error(`Boundary violation: Action button found in read-only deck ${deckId}`);
        violationCount++;
      }
    });

    if (violationCount === 0) {
      console.log('✓ Invariant 7 passed: All diagnostic decks (L2, L3, L4, L5, L7) are 100% read-only.');
    } else {
      console.warn(`! Invariant 7 warning: ${violationCount} interactive elements detected in diagnostic decks.`);
    }
  }

  // =========================================================================
  // Event Binding & App Initialization
  // =========================================================================

  function initEventListeners() {
    // Top Provenance Badge Switcher
    if (dom.badgeProvenance) {
      dom.badgeProvenance.addEventListener('click', toggleProvenance);
    }

    // Navigation Dock Tabs
    dom.dockTabs.forEach(tab => {
      tab.addEventListener('click', () => {
        const layer = tab.getAttribute('data-layer');
        navigateTo(layer);
      });
    });

    // Home / Hub navigation button
    if (dom.btnNavHub) {
      dom.btnNavHub.addEventListener('click', () => navigateTo('hub'));
    }

    // CTA Button to enter L6
    if (dom.btnEnterL6) {
      dom.btnEnterL6.addEventListener('click', () => navigateTo('l6'));
    }

    // "Pop the Hood" Buttons on Hub Deck
    document.querySelectorAll('[data-jump]').forEach(btn => {
      btn.addEventListener('click', (e) => {
        const target = e.currentTarget.getAttribute('data-jump');
        navigateTo(target);
      });
    });

    // Layer 7-matrix item clicks
    document.querySelectorAll('[data-goto]').forEach(item => {
      item.addEventListener('click', (e) => {
        const target = e.currentTarget.getAttribute('data-goto');
        navigateTo(target);
      });
    });

    // Limit Order Side Toggle (BUY / SELL)
    if (dom.btnSideBuy && dom.btnSideSell) {
      dom.btnSideBuy.addEventListener('click', () => {
        state.orderSide = 'BUY';
        dom.btnSideBuy.classList.add('active');
        dom.btnSideSell.classList.remove('active');
      });
      dom.btnSideSell.addEventListener('click', () => {
        state.orderSide = 'SELL';
        dom.btnSideSell.classList.add('active');
        dom.btnSideBuy.classList.remove('active');
      });
    }

    // Order Form Submit
    if (dom.formAgoraOrder) {
      dom.formAgoraOrder.addEventListener('submit', handleOrderFormSubmit);
    }

    // Intent Builder Submit
    if (dom.formIntentBuilder) {
      dom.formIntentBuilder.addEventListener('submit', handleIntentFormSubmit);
    }

    // Prompt & Type Change handlers for read-back preview
    if (dom.intentPrompt) {
      dom.intentPrompt.addEventListener('input', updateReadbackPreview);
    }
    if (dom.intentType) {
      dom.intentType.addEventListener('change', updateReadbackPreview);
    }

    // Quick Prompt Chips
    document.querySelectorAll('.btn-quick-chip').forEach(chip => {
      chip.addEventListener('click', (e) => {
        const template = e.currentTarget.getAttribute('data-template');
        handleQuickTemplate(template);
      });
    });

    // Chore Claim Buttons
    document.querySelectorAll('.btn-claim-chore').forEach(btn => {
      btn.addEventListener('click', (e) => {
        handleChoreClaim(e.currentTarget);
      });
    });

    // Keyboard navigation (Strict Adjacency Left/Right arrows)
    window.addEventListener('keydown', (e) => {
      const layerSeq = ['l2', 'l3', 'l4', 'l5', 'l6', 'l7'];
      if (state.activeLayerKey) {
        const idx = layerSeq.indexOf(state.activeLayerKey);
        if (e.key === 'ArrowRight' && idx < layerSeq.length - 1) {
          navigateTo(layerSeq[idx + 1]);
        } else if (e.key === 'ArrowLeft' && idx > 0) {
          navigateTo(layerSeq[idx - 1]);
        } else if (e.key === 'Escape') {
          navigateTo('hub');
        }
      }
    });
  }

  // =========================================================================
  // Bootstrap Application
  // =========================================================================

  function initApp() {
    initEventListeners();
    renderOrderBook();
    updateVitalsUI();
    updateReadbackPreview();
    verifyDiagnosticBoundaries();

    // Initial navigation to Hub
    navigateTo('hub');

    // Start diagnostic polling loop
    pollDiagnostics();
    setInterval(pollDiagnostics, CONFIG.pollIntervalMs);

    logTerminal('Application initialized. Simulation engine active.');
  }

  // Launch when DOM is ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initApp);
  } else {
    initApp();
  }

  // Export state and handlers for automated testing suites
  window.__COLLECTIVE_COMPANION__ = {
    state,
    navigateTo,
    toggleProvenance,
    dispatchL6Intent,
    pollDiagnostics,
    verifyDiagnosticBoundaries
  };

})();
