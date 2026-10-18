/**
 * Layer 5 Governance Systems Manager & L4 Workflow Interface Binder
 * Sprint 14: Mathematical Immune System (GOVERNANCE_SPEC.md)
 *
 * Manages W3C DIDs, directional TrustGraphEdge weights, BBS+ SkillCredential VCs,
 * SafeHarborSeverance ZK blind spots, and establishes the Layer 4 BPMN Workflow
 * interfaces bound to each Governance Policy.
 */

(() => {
  'use strict';

  // =========================================================================
  // 1. Canonical Layer 4 Workflows (Fallback + Synced with oasis_l4_workflow_db)
  // =========================================================================
  const BASE_L4_WORKFLOWS = [
    { workflow_id: 'wf-fabrication-solar-v1', title: 'Roof Solar Microgrid Array Fabrication', intent_class: 'mesh:FabricationIntent', status: 'ACTIVE_BOUND' },
    { workflow_id: 'wf-hydrology-swale-v1', title: 'Keyline Contour Swale & Catchment Excavation', intent_class: 'mesh:FabricationIntent', status: 'ACTIVE_BOUND' },
    { workflow_id: 'wf-kitchen-gamma-v1', title: 'Sovereign Kitchen Harvest & Batch Thermal Cooking', intent_class: 'mesh:FabricationIntent', status: 'ACTIVE_BOUND' },
    { workflow_id: 'wf-rest-lock-sigma-v1', title: 'Psychological Homeostasis & Temporal Bounty Shield', intent_class: 'mesh:RestIntent', status: 'ACTIVE_BOUND' },
    { workflow_id: 'wf-mediation-psi-template', title: 'Ostrom Graduated Sanction & Restitution Mediation', intent_class: 'mesh:MediationIntent', status: 'TEMPLATE' },
    { workflow_id: 'wf-triage-chi-template', title: 'Zero-Knowledge Emergency Medical & Grid Triage Fast-Track', intent_class: 'mesh:TriageIntent', status: 'TEMPLATE' }
  ];

  const DEFAULT_POLICIES = [
    {
      policy_id: 'pol-solar-microgrid-alpha',
      domain_label: 'Microgrid & Solar Fabrication Policy',
      intent_class: 'mesh:FabricationIntent / Solar',
      required_vc: 'Solar_Grid_Apprentice_VC',
      max_dijkstra_hops: 2.0,
      bound_workflow_id: 'wf-fabrication-solar-v1'
    },
    {
      policy_id: 'pol-hydrology-earthworks-beta',
      domain_label: 'Hydrology & Earthworks Policy',
      intent_class: 'mesh:FabricationIntent / Swale & Cistern',
      required_vc: 'Hydro_Earthworks_VC',
      max_dijkstra_hops: 2.5,
      bound_workflow_id: 'wf-hydrology-swale-v1'
    },
    {
      policy_id: 'pol-sovereign-kitchen-gamma',
      domain_label: 'Sovereign Kitchen Scenario Γ Policy',
      intent_class: 'mesh:FabricationIntent / Culinary',
      required_vc: 'Sovereign_Kitchen_Steward_VC',
      max_dijkstra_hops: 3.0,
      bound_workflow_id: 'wf-kitchen-gamma-v1'
    },
    {
      policy_id: 'pol-citizen-rest-shield-sigma',
      domain_label: 'Citizen Rest & Burnout Shield Policy',
      intent_class: 'mesh:RestIntent / Scenario Σ',
      required_vc: 'None (Inalienable Somatic Right)',
      max_dijkstra_hops: '∞ (All Rings R0–R6)',
      bound_workflow_id: 'wf-rest-lock-sigma-v1'
    },
    {
      policy_id: 'pol-ostrom-mediation-psi',
      domain_label: 'Ostrom Conflict Mediation & Social Slashing',
      intent_class: 'mesh:MediationIntent / Scenario Ψ',
      required_vc: 'Mediation_Steward_VC',
      max_dijkstra_hops: 1.5,
      bound_workflow_id: 'wf-mediation-psi-template'
    }
  ];

  // =========================================================================
  // 2. State
  // =========================================================================
  const state = {
    availableWorkflows: JSON.parse(JSON.stringify(BASE_L4_WORKFLOWS)),
    policies: JSON.parse(JSON.stringify(DEFAULT_POLICIES)),
    dijkstraEvaluations: 148,
    trustEdges: [
      {
        source_did: 'did:mesh:node00:founder',
        target_did: 'did:mesh:node01:maya',
        edge_type: 'TrustGraphEdge_Vouch',
        weight: 1.0,
        dijkstra_cost: 1.0,
        ring: 'R1',
        status: 'ACTIVE_VOUCHED'
      },
      {
        source_did: 'did:mesh:node00:founder',
        target_did: 'did:mesh:node02:kael',
        edge_type: 'TrustGraphEdge_Vouch',
        weight: 1.0,
        dijkstra_cost: 1.0,
        ring: 'R1',
        status: 'ACTIVE_VOUCHED'
      },
      {
        source_did: 'did:mesh:node01:maya',
        target_did: 'did:mesh:node03:soren',
        edge_type: 'TrustGraphEdge_ProbationaryVouch',
        weight: 0.85,
        dijkstra_cost: 1.85,
        ring: 'R3',
        status: 'ACTIVE_PROBATIONARY'
      }
    ]
  };

  // =========================================================================
  // 3. BroadcastChannel('oasis_companion_bus') & LocalStorage Sync
  // =========================================================================
  let oasisBus = null;
  try {
    if (typeof BroadcastChannel !== 'undefined') {
      oasisBus = new BroadcastChannel('oasis_companion_bus');
      oasisBus.onmessage = (ev) => {
        if (!ev || !ev.data) return;
        if (ev.data.type === 'L4_WORKFLOW_SAVED') {
          syncAvailableWorkflowsFromL4();
          populatePolicyWorkflowSelectors();
          appendLog(
            `[L4->L5 BUS] Synced new specialized Layer 4 Workflow '${ev.data.workflow?.workflow_id}' into L5 Policy Binding selectors!`,
            'log-ok'
          );
        }
      };
    }
  } catch (_e) {}

  function appendLog(msg, cls = 'log-l5') {
    const logEl = document.getElementById('l5-terminal-log');
    if (!logEl) return;
    const row = document.createElement('div');
    row.className = cls;
    const ts = new Date().toTimeString().split(' ')[0];
    row.textContent = `[${ts}] ${msg}`;
    logEl.appendChild(row);
    logEl.scrollTop = logEl.scrollHeight;
  }

  function showStatusBanner(msg) {
    const banner = document.getElementById('l5-status-banner');
    if (!banner) return;
    banner.textContent = msg;
    banner.style.display = 'block';
  }

  function syncAvailableWorkflowsFromL4() {
    try {
      if (typeof localStorage !== 'undefined') {
        const raw = localStorage.getItem('oasis_l4_workflow_db');
        if (raw) {
          const parsed = JSON.parse(raw);
          const list = Array.isArray(parsed) ? parsed : (parsed.workflows || []);
          list.forEach(wf => {
            if (!wf || !wf.workflow_id) return;
            const idx = state.availableWorkflows.findIndex(w => w.workflow_id === wf.workflow_id);
            if (idx >= 0) {
              state.availableWorkflows[idx] = wf;
            } else {
              state.availableWorkflows.push(wf);
            }
          });
        }
      }
    } catch (_e) {}

    const pill = document.getElementById('l5-available-wf-pill');
    if (pill) {
      pill.textContent = `Available L4 Workflows: ${state.availableWorkflows.length} (Synced with L4 Database)`;
    }
  }

  function loadStoredBindings() {
    try {
      if (typeof localStorage !== 'undefined') {
        const raw = localStorage.getItem('oasis_l5_governance_bindings');
        if (raw) {
          const parsed = JSON.parse(raw);
          const list = Array.isArray(parsed) ? parsed : (parsed.bindings || []);
          if (Array.isArray(list) && list.length > 0) {
            list.forEach(savedPol => {
              const target = state.policies.find(p => p.policy_id === savedPol.policy_id);
              if (target) {
                if (savedPol.required_vc) target.required_vc = savedPol.required_vc;
                if (savedPol.max_dijkstra_hops !== undefined) target.max_dijkstra_hops = savedPol.max_dijkstra_hops;
                if (savedPol.bound_workflow_id) target.bound_workflow_id = savedPol.bound_workflow_id;
              }
            });
          }
        }
      }
    } catch (_e) {}
  }

  function populatePolicyWorkflowSelectors() {
    state.policies.forEach(pol => {
      const sel = document.getElementById(`bind-wf-${pol.policy_id}`);
      if (!sel) return;

      const optionsHtml = state.availableWorkflows.map(wf => {
        const selectedAttr = (wf.workflow_id === pol.bound_workflow_id) ? ' selected' : '';
        const badge = wf.status === 'SPECIALIZED_CUSTOM' ? ' [CUSTOM]' : '';
        return `<option value="${wf.workflow_id}"${selectedAttr}>${wf.workflow_id} (${wf.title}${badge})</option>`;
      }).join('');

      sel.innerHTML = optionsHtml;
      sel.value = pol.bound_workflow_id;
    });
  }

  // =========================================================================
  // 4. Ratify L5 Governance Policies & Bind L4 Workflows
  // =========================================================================
  function ratifyGovernanceBindings() {
    state.policies.forEach(pol => {
      const vcSel = document.querySelector(`.policy-vc-select[data-policy-id="${pol.policy_id}"]`);
      const hopsIn = document.querySelector(`.policy-hops-input[data-policy-id="${pol.policy_id}"]`);
      const wfSel = document.getElementById(`bind-wf-${pol.policy_id}`);

      if (vcSel && vcSel.value) pol.required_vc = vcSel.value;
      if (hopsIn && hopsIn.value !== undefined && hopsIn.value !== '') {
        pol.max_dijkstra_hops = isNaN(Number(hopsIn.value)) ? hopsIn.value : Number(hopsIn.value);
      }
      if (wfSel && wfSel.value) pol.bound_workflow_id = wfSel.value;
    });

    const payload = {
      type: 'L5_GOVERNANCE_UPDATED',
      schema_version: '1.4.0-sprint14',
      rbac_prohibited: true,
      bindings: state.policies,
      timestamp: Date.now()
    };

    try {
      if (typeof localStorage !== 'undefined') {
        localStorage.setItem('oasis_l5_governance_bindings', JSON.stringify(payload));
      }
    } catch (_e) {}

    try {
      if (oasisBus) oasisBus.postMessage(payload);
    } catch (_e) {}

    const summary = state.policies.map(p => `${p.policy_id}->${p.bound_workflow_id}`).join(', ');
    showStatusBanner(
      `🛡️ Ratified 5 Layer 5 Governance Policies & Bound Layer 4 Workflow Interfaces (${summary})!`
    );
    appendLog(
      `[L5-RATIFY] Persisted L5<->L4 Workflow Binding Table to localStorage('oasis_l5_governance_bindings') & broadcast L5_GOVERNANCE_UPDATED.`,
      'log-ok'
    );
    return payload;
  }

  // =========================================================================
  // 5. Section 2: Interactive Trust Ring Graph Simulation
  // =========================================================================
  function simulateVouchingOmicron() {
    const sorenEdge = state.trustEdges[2];
    sorenEdge.edge_type = 'TrustGraphEdge_Vouch';
    sorenEdge.weight = 1.0;
    sorenEdge.dijkstra_cost = 1.0;
    sorenEdge.ring = 'R1';
    sorenEdge.status = 'ACTIVE_VOUCHED_R1';

    state.dijkstraEvaluations += 1;
    const card = document.getElementById('edge-card-2');
    const metrics = document.getElementById('edge-soren-metrics');
    if (card) card.classList.remove('slashed-edge', 'safeharbor-edge');
    if (metrics) {
      metrics.innerHTML = `
        Type: <code>TrustGraphEdge_Vouch</code><br>
        edgeWeight: <strong style="color:var(--text-emerald);">+1.00</strong> │ Dijkstra Cost: <code>1.0 hops</code> (Ring R1)<br>
        Ceremony: <code>SCN-OMICRON-2OF3-FROST-VOUCH</code> [ACTIVE_VOUCHED_R1]
      `;
    }
    const evalEl = document.getElementById('l5-kpi-dijkstra-evals');
    if (evalEl) evalEl.textContent = `${state.dijkstraEvaluations} Evaluations`;

    showStatusBanner('🤝 Scenario Omicron Vouch Verified: did:mesh:node03:soren promoted to Ring R1 (edgeWeight = +1.00, 1.0 hops)!');
    appendLog('[L5-OMICRON] Directional TrustGraphEdge did:mesh:node01:maya -> did:mesh:node03:soren updated to edgeWeight = +1.00 (Ring R1).', 'log-ok');
  }

  function simulateSocialSlashingRho() {
    const sorenEdge = state.trustEdges[2];
    sorenEdge.edge_type = 'TrustGraphEdge_Slashed';
    sorenEdge.weight = -1.0;
    sorenEdge.dijkstra_cost = Infinity;
    sorenEdge.ring = 'SLASHED';
    sorenEdge.status = 'SEVERED_OSTROM_SANCTION';

    state.dijkstraEvaluations += 1;
    const card = document.getElementById('edge-card-2');
    const metrics = document.getElementById('edge-soren-metrics');
    if (card) {
      card.classList.remove('safeharbor-edge');
      card.classList.add('slashed-edge');
    }
    if (metrics) {
      metrics.innerHTML = `
        Type: <code>TrustGraphEdge_Slashed (Scenario Rho)</code><br>
        edgeWeight: <strong style="color:var(--text-rose);">-1.00 [SLASHED]</strong> │ Dijkstra Cost: <code>∞ (SEVERED)</code><br>
        Status: <code>OSTROM_GRADUATED_SANCTION</code> [L4 ROUTING BLOCKED]
      `;
    }
    const evalEl = document.getElementById('l5-kpi-dijkstra-evals');
    if (evalEl) evalEl.textContent = `${state.dijkstraEvaluations} Evaluations`;

    showStatusBanner('⚡ Scenario Rho Social Slashing Applied: edgeWeight = -1.00! Layer 4 Orchestrator is mathematically blocked from routing to slashed DID.');
    appendLog('[L5-RHO-SLASH] Consensus threshold flipped edgeWeight to -1.00. Dijkstra path = ∞. Layer 4 routing severed.', 'log-slash');
  }

  function simulateSafeHarborSeverance() {
    const sorenEdge = state.trustEdges[2];
    sorenEdge.edge_type = 'mesh:SafeHarborSeverance';
    sorenEdge.weight = -1.0;
    sorenEdge.dijkstra_cost = Infinity;
    sorenEdge.status = 'ZK_SAFE_HARBOR_BLIND_SPOT';

    const card = document.getElementById('edge-card-2');
    const metrics = document.getElementById('edge-soren-metrics');
    if (card) {
      card.classList.remove('slashed-edge');
      card.classList.add('safeharbor-edge');
    }
    if (metrics) {
      metrics.innerHTML = `
        Type: <code>mesh:SafeHarborSeverance (Scenario Omicron)</code><br>
        edgeWeight: <strong style="color:var(--text-amber);">-1.00 [ZK BLIND SPOT]</strong> │ Co-Location: <code>PROHIBITED</code><br>
        Circuit: <code>SafeHarborSeverance_ZeroKnowledge_Proximity_Filter</code> [ACTIVE]
      `;
    }
    const zkEl = document.getElementById('l5-kpi-zk-status');
    if (zkEl) zkEl.textContent = '1 Active SafeHarbor ZK Filter';

    showStatusBanner('🕶️ SafeHarborSeverance Invoked (Scenario Omicron): Cryptographic blind spot active; Layer 4 prohibited from co-routing DIDs.');
    appendLog('[L5-SAFE-HARBOR] Enforced mesh:SafeHarborSeverance ZK blind spot (edgeWeight = -1.0, telemetry masked, co-location blocked).', 'log-warn');
  }

  // =========================================================================
  // 6. Optional Fetch of trust_gates.json
  // =========================================================================
  async function loadTrustGatesJsonIfServed() {
    const urls = ['./trust_gates.json', '../trust_gates.json'];
    for (const url of urls) {
      try {
        const res = await fetch(url);
        if (res && res.ok) {
          const data = await res.json();
          const vcCount = Object.keys(data.verifiable_credential_catalog || {}).length;
          appendLog(`[L5-JSON] Verified ${url} (${vcCount} VCs, algorithm: ${data.permission_engine?.algorithm}).`, 'log-l5');
          break;
        }
      } catch (_e) {}
    }
  }

  // =========================================================================
  // 7. Initialization & Event Listeners
  // =========================================================================
  function initEvents() {
    const btnRatify = document.getElementById('btn-ratify-l5-bindings');
    if (btnRatify) {
      btnRatify.addEventListener('click', ratifyGovernanceBindings);
    }

    const btnVouch = document.getElementById('btn-sim-vouch');
    if (btnVouch) {
      btnVouch.addEventListener('click', simulateVouchingOmicron);
    }

    const btnSlash = document.getElementById('btn-sim-slash');
    if (btnSlash) {
      btnSlash.addEventListener('click', simulateSocialSlashingRho);
    }

    const btnSafeHarbor = document.getElementById('btn-sim-safe-harbor');
    if (btnSafeHarbor) {
      btnSafeHarbor.addEventListener('click', simulateSafeHarborSeverance);
    }

    if (typeof window !== 'undefined' && typeof window.addEventListener === 'function') {
      window.addEventListener('storage', (ev) => {
        if (ev && ev.key === 'oasis_l4_workflow_db') {
          syncAvailableWorkflowsFromL4();
          populatePolicyWorkflowSelectors();
        }
      });
    }
  }

  function init() {
    syncAvailableWorkflowsFromL4();
    loadStoredBindings();
    populatePolicyWorkflowSelectors();
    initEvents();
    loadTrustGatesJsonIfServed();
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }

  window.__COLLECTIVE_GOVERNANCE_MANAGER__ = {
    state,
    syncAvailableWorkflowsFromL4,
    populatePolicyWorkflowSelectors,
    ratifyGovernanceBindings,
    simulateVouchingOmicron,
    simulateSocialSlashingRho,
    simulateSafeHarborSeverance
  };
})();
