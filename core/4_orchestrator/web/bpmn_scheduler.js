/**
 * Layer 4 BPMN 2.0 Workflow Designer, Filterable Database & Aggregated Diagnostics Engine
 * Sprint 14: Architectural Separation of L4 (Workflow Designer & DB), L5 (Governance & L4 Interface Binder), and L6 (Day-to-Day Calendar of Intents)
 *
 * Connects to Layer 5 Governance Manager, Layer 6 Citizen Companion & Oasis 3D Viewport via:
 *   - BroadcastChannel('oasis_companion_bus')
 *   - localStorage ('oasis_l4_workflow_db', 'oasis_l5_governance_bindings', 'oasis_companion_telemetry')
 */

(() => {
  'use strict';

  // =========================================================================
  // 1. Canonical 6-Workflow Fallback Database (Matches workflow_database.json)
  // =========================================================================
  const DEFAULT_WORKFLOWS = [
    {
      workflow_id: 'wf-fabrication-solar-v1',
      title: 'Roof Solar Microgrid Array Fabrication',
      intent_class: 'mesh:FabricationIntent',
      scenario: 'Scenario Alpha',
      status: 'ACTIVE_BOUND',
      version: '1.0.0',
      description: 'Routes a signed mesh:FabricationIntent to mount salvaged photovoltaic hardware onto the Lot 402 timber roof structure and interconnect with the 48V DC microgrid bus (+4.5 kWh/day).',
      l5_governance_interface: {
        policy_id: 'pol-solar-microgrid-alpha',
        required_vc: 'Solar_Grid_Apprentice_VC',
        max_dijkstra_hops: 2.0,
        zk_circuit: 'BBS_Plus_Selective_Disclosure_VC'
      },
      physics_check_params: {
        min_caloric_floor_kcal: 1800,
        min_focus_pct: 15,
        required_material_input: 'Solar PV Kit',
        required_material_qty: 1,
        water_reserve_min_l: 20.0
      },
      escrow_lock_params: {
        exergy_escrow_mj: 16.2,
        value_tokens_vt: 4.5,
        demurrage_exempt: false,
        fallback_on_failure: 'QueueUntilRested'
      },
      actuation_target: {
        oasis_task_id: 1,
        bounty_id: 'B-101',
        target_voxel: '(9, 2, 7) Roof South Array',
        expected_output_delta: '+4.5 kWh/day Solar Exergy (-1 Solar PV Kit)'
      },
      telemetry_aggregates: {
        executions_24h: 42,
        pass_rate_pct: 95.2,
        avg_latency_ms: 12.8,
        escrowed_mj: 16.2
      }
    },
    {
      workflow_id: 'wf-hydrology-swale-v1',
      title: 'Keyline Contour Swale & Catchment Excavation',
      intent_class: 'mesh:FabricationIntent',
      scenario: 'Scenario Beta',
      status: 'ACTIVE_BOUND',
      version: '1.0.0',
      description: 'Validates instream ecological water floor (F_eco >= 80%) and routes contour trenching to harvest +8.0 L/day rainwater catchment and +2 Cob earth thermal mass.',
      l5_governance_interface: {
        policy_id: 'pol-hydrology-earthworks-beta',
        required_vc: 'Hydro_Earthworks_VC',
        max_dijkstra_hops: 2.5,
        zk_circuit: 'BBS_Plus_Selective_Disclosure_VC'
      },
      physics_check_params: {
        min_caloric_floor_kcal: 1800,
        min_focus_pct: 20,
        required_material_input: 'None',
        required_material_qty: 0,
        water_reserve_min_l: 15.0
      },
      escrow_lock_params: {
        exergy_escrow_mj: 14.4,
        value_tokens_vt: 4.0,
        demurrage_exempt: false,
        fallback_on_failure: 'QueueUntilRested'
      },
      actuation_target: {
        oasis_task_id: 2,
        bounty_id: 'B-103',
        target_voxel: '(14, 0, 12) Backyard Keyline Contour',
        expected_output_delta: '+8.0 L/day Water Catchment & +2 Cob Lumps'
      },
      telemetry_aggregates: {
        executions_24h: 36,
        pass_rate_pct: 94.4,
        avg_latency_ms: 15.1,
        escrowed_mj: 14.4
      }
    },
    {
      workflow_id: 'wf-kitchen-gamma-v1',
      title: 'Sovereign Kitchen Harvest & Batch Thermal Cooking',
      intent_class: 'mesh:FabricationIntent',
      scenario: 'Scenario Gamma',
      status: 'ACTIVE_BOUND',
      version: '1.0.0',
      description: 'Coordinates garden produce harvest and thermal hearth batch cooking to replenish citizen caloric reserves (+800 kcal / 2 Sovereign Meals) with ZK allergen verification.',
      l5_governance_interface: {
        policy_id: 'pol-sovereign-kitchen-gamma',
        required_vc: 'Sovereign_Kitchen_Steward_VC',
        max_dijkstra_hops: 3.0,
        zk_circuit: 'ZK_Allergen_Pharmacopeia_Pass'
      },
      physics_check_params: {
        min_caloric_floor_kcal: 1200,
        min_focus_pct: 10,
        required_material_input: 'Raw Produce',
        required_material_qty: 2,
        water_reserve_min_l: 5.0
      },
      escrow_lock_params: {
        exergy_escrow_mj: 12.0,
        value_tokens_vt: 3.33,
        demurrage_exempt: true,
        fallback_on_failure: 'QueueUntilRested'
      },
      actuation_target: {
        oasis_task_id: 3,
        bounty_id: 'B-102',
        target_voxel: '(7, 1, 7) Sovereign Kitchen Hearth',
        expected_output_delta: '+2 Sovereign Meals (+800 kcal) (-2 Raw Produce)'
      },
      telemetry_aggregates: {
        executions_24h: 48,
        pass_rate_pct: 97.9,
        avg_latency_ms: 11.4,
        escrowed_mj: 12.0
      }
    },
    {
      workflow_id: 'wf-rest-lock-sigma-v1',
      title: 'Psychological Homeostasis & Temporal Bounty Shield',
      intent_class: 'mesh:RestIntent',
      scenario: 'Scenario Sigma',
      status: 'ACTIVE_BOUND',
      version: '1.0.0',
      description: 'Activates an inalienable psychological temporal lock preventing Layer 4 from routing discretionary work bounties to the issuer while restoring Focus (+65%) and thermal comfort.',
      l5_governance_interface: {
        policy_id: 'pol-citizen-rest-shield-sigma',
        required_vc: 'None (Inalienable Somatic Right)',
        max_dijkstra_hops: 999.0,
        zk_circuit: 'ZeroKnowledge_Somatic_Privacy_Shield'
      },
      physics_check_params: {
        min_caloric_floor_kcal: 800,
        min_focus_pct: 0,
        required_material_input: 'None',
        required_material_qty: 0,
        water_reserve_min_l: 2.0
      },
      escrow_lock_params: {
        exergy_escrow_mj: 0.0,
        value_tokens_vt: 0.0,
        demurrage_exempt: true,
        fallback_on_failure: 'QueueUntilRested'
      },
      actuation_target: {
        oasis_task_id: 3,
        bounty_id: 'REST-SIGMA',
        target_voxel: '(8, 1, 8) Thermal Enclosure Hearth Sanctuary',
        expected_output_delta: '+65% Focus Recovery & Work Queue Shield Active'
      },
      telemetry_aggregates: {
        executions_24h: 14,
        pass_rate_pct: 100.0,
        avg_latency_ms: 6.5,
        escrowed_mj: 0.0
      }
    },
    {
      workflow_id: 'wf-mediation-psi-template',
      title: 'Ostrom Graduated Sanction & Restitution Mediation',
      intent_class: 'mesh:MediationIntent',
      scenario: 'Scenario Psi',
      status: 'TEMPLATE',
      version: '1.0.0-template',
      description: 'Template workflow for polycentric conflict resolution, thermodynamic restitution calculation, and 2-of-3 FROST council review before social slashing (edgeWeight = -1.0).',
      l5_governance_interface: {
        policy_id: 'pol-ostrom-mediation-psi',
        required_vc: 'Mediation_Steward_VC',
        max_dijkstra_hops: 1.5,
        zk_circuit: 'FROST_2of3_Restitution_Quorum'
      },
      physics_check_params: {
        min_caloric_floor_kcal: 1400,
        min_focus_pct: 25,
        required_material_input: 'Cob Lump',
        required_material_qty: 1,
        water_reserve_min_l: 10.0
      },
      escrow_lock_params: {
        exergy_escrow_mj: 10.8,
        value_tokens_vt: 3.0,
        demurrage_exempt: true,
        fallback_on_failure: 'EscalateToL5Mediation'
      },
      actuation_target: {
        oasis_task_id: 4,
        bounty_id: 'MED-PSI',
        target_voxel: '(12, 1, 6) Cadastral Council Rack',
        expected_output_delta: 'Restitution Pact Settled & TrustGraphEdge Restored'
      },
      telemetry_aggregates: {
        executions_24h: 5,
        pass_rate_pct: 80.0,
        avg_latency_ms: 22.0,
        escrowed_mj: 10.8
      }
    },
    {
      workflow_id: 'wf-triage-chi-template',
      title: 'Zero-Knowledge Emergency Medical & Grid Triage Fast-Track',
      intent_class: 'mesh:TriageIntent',
      scenario: 'Scenario Chi/Upsilon',
      status: 'TEMPLATE',
      version: '1.0.0-template',
      description: 'High-priority Tier-1/Tier-2 emergency workflow triggered by human TriageIntent or L2 telemetry anomaly. Verifies ZK pharmacopeia constraints and preempts discretionary Tier-4 loads.',
      l5_governance_interface: {
        policy_id: 'pol-emergency-triage-chi',
        required_vc: 'ZK_Pharmacopeia_Medic_VC',
        max_dijkstra_hops: 3.0,
        zk_circuit: 'ZK_Allergen_Pharmacopeia_Proof'
      },
      physics_check_params: {
        min_caloric_floor_kcal: 800,
        min_focus_pct: 5,
        required_material_input: 'None',
        required_material_qty: 0,
        water_reserve_min_l: 10.0
      },
      escrow_lock_params: {
        exergy_escrow_mj: 0.0,
        value_tokens_vt: 0.0,
        demurrage_exempt: true,
        fallback_on_failure: 'EscalateToL5Mediation'
      },
      actuation_target: {
        oasis_task_id: 1,
        bounty_id: 'TRIAGE-CHI',
        target_voxel: '(12, 1, 6) Tier-2 Survival Bus Priority',
        expected_output_delta: 'Preempt Tier-4 Loads & Route Emergency Microgrid Reserve'
      },
      telemetry_aggregates: {
        executions_24h: 3,
        pass_rate_pct: 100.0,
        avg_latency_ms: 4.8,
        escrowed_mj: 0.0
      }
    }
  ];

  function buildBpmnStagesForWorkflow(wf) {
    const reqVc = wf.l5_governance_interface?.required_vc || 'Solar_Grid_Apprentice_VC';
    const maxHops = wf.l5_governance_interface?.max_dijkstra_hops ?? 2.5;
    const kcal = wf.physics_check_params?.min_caloric_floor_kcal ?? 800;
    const focus = wf.physics_check_params?.min_focus_pct ?? 20;
    const mat = wf.physics_check_params?.required_material_input || 'None';
    const mj = Number(wf.escrow_lock_params?.exergy_escrow_mj ?? 0).toFixed(1);
    const vt = (Number(mj) / 3.6).toFixed(2);
    const demExempt = Boolean(wf.escrow_lock_params?.demurrage_exempt);
    const fallback = wf.escrow_lock_params?.fallback_on_failure || 'QueueUntilRested';
    const targetVoxel = wf.actuation_target?.target_voxel || '(12, 1, 6)';
    const delta = wf.actuation_target?.expected_output_delta || 'Workflow Actuation Complete';

    return [
      {
        step: 1,
        id: 'StartEvent_L6_Ingest',
        bpmn_type: 'StartEvent',
        layer: 'L6',
        name: 'StartEvent (L6 Ingest)',
        expression: `intent.intentType == '${wf.intent_class}' && signature_ed25519_valid`
      },
      {
        step: 2,
        id: 'ExclusiveGateway_L5_TrustGate',
        bpmn_type: 'ExclusiveGateway',
        layer: 'L5',
        name: 'ExclusiveGateway (L5 Trust Gate)',
        expression: reqVc.startsWith('None')
          ? 'inalienable_somatic_right == true (All Rings R0..R6)'
          : `has_vc(issuerDid, '${reqVc}') && dijkstra_hops <= ${maxHops}`
      },
      {
        step: 3,
        id: 'ServiceTask_L2_ValidatePhysics',
        bpmn_type: 'ServiceTask',
        layer: 'L2/L1b',
        name: 'ServiceTask (L2/L1b Physics Check)',
        expression: `kcal >= ${kcal} && focus_pct >= ${focus} && material('${mat}')_ready`
      },
      {
        step: 4,
        id: 'ServiceTask_L3_EscrowLock',
        bpmn_type: 'ServiceTask',
        layer: 'L3',
        name: 'ServiceTask (L3 CRDT Escrow Lock)',
        expression: `lock_escrow(${mj} MJ, ${vt} VT, demurrage_exempt=${demExempt}, fallback='${fallback}')`
      },
      {
        step: 5,
        id: 'UserTask_L1b_SpatialActuation',
        bpmn_type: 'UserTask',
        layer: 'L2/L1b',
        name: 'UserTask (L2/L1b Actuation)',
        expression: `actuate_voxel(target='${targetVoxel}', delta='${delta}')`
      },
      {
        step: 6,
        id: 'EndEvent_L3_Settle',
        bpmn_type: 'EndEvent',
        layer: 'L3',
        name: 'EndEvent (L3 Settlement)',
        expression: `verify_provenance(0x01|0x02) && settle_crdt_ledger('${wf.workflow_id}')`
      }
    ];
  }

  // Ensure all default workflows have bpmn_stages populated
  DEFAULT_WORKFLOWS.forEach(wf => {
    wf.bpmn_stages = buildBpmnStagesForWorkflow(wf);
  });

  // =========================================================================
  // 2. Application State
  // =========================================================================
  const state = {
    workflows: JSON.parse(JSON.stringify(DEFAULT_WORKFLOWS)),
    selectedWorkflowId: 'wf-fabrication-solar-v1',
    activeStageStep: 1,
    filters: {
      intentClass: 'ALL',
      status: 'ALL',
      searchQuery: ''
    },
    diagnostics: {
      totalIntentsIngested: 148,
      activeJobQueueDepth: 4,
      l5GatePassCount: 139,
      l5GateRejectCount: 9,
      avgStageLatencyMs: 14.2,
      totalExergyEscrowedMj: 53.4
    }
  };

  // =========================================================================
  // 3. BroadcastChannel('oasis_companion_bus') & LocalStorage Persistence
  // =========================================================================
  let oasisBus = null;
  try {
    if (typeof BroadcastChannel !== 'undefined') {
      oasisBus = new BroadcastChannel('oasis_companion_bus');
      oasisBus.onmessage = (ev) => {
        if (!ev || !ev.data) return;
        if (ev.data.type === 'L5_GOVERNANCE_UPDATED') {
          appendLog(`[L5->L4 BUS] Received L5_GOVERNANCE_UPDATED policy binding table (${ev.data.bindings?.length || 5} policies bound).`, 'log-l5');
        } else if (ev.data.type === 'L6_CALENDAR_PUBLISHED') {
          state.diagnostics.totalIntentsIngested += (ev.data.intents?.length || 4);
          state.diagnostics.activeJobQueueDepth = ev.data.intents?.length || 4;
          renderDiagnosticsUI();
          appendLog(`[L6->L4 INGEST] Ingested ${ev.data.intents?.length || 4} signed Intents from Layer 6 Calendar of Intents into mesh:JobQueue.`, 'log-ok');
        }
      };
    }
  } catch (_e) {}

  function loadStoredWorkflows() {
    try {
      if (typeof localStorage !== 'undefined') {
        const raw = localStorage.getItem('oasis_l4_workflow_db');
        if (raw) {
          const parsed = JSON.parse(raw);
          const list = Array.isArray(parsed) ? parsed : (parsed.workflows || []);
          if (Array.isArray(list) && list.length > 0) {
            list.forEach(customWf => {
              if (!customWf || !customWf.workflow_id) return;
              if (!customWf.bpmn_stages) {
                customWf.bpmn_stages = buildBpmnStagesForWorkflow(customWf);
              }
              const existingIdx = state.workflows.findIndex(w => w.workflow_id === customWf.workflow_id);
              if (existingIdx >= 0) {
                state.workflows[existingIdx] = customWf;
              } else {
                state.workflows.push(customWf);
              }
            });
          }
        }
      }
    } catch (_e) {}
  }

  function persistWorkflowsToStorage() {
    try {
      if (typeof localStorage !== 'undefined') {
        localStorage.setItem('oasis_l4_workflow_db', JSON.stringify({
          schema_version: '1.4.0-sprint14',
          updated_at: Date.now(),
          workflows: state.workflows
        }));
      }
    } catch (_e) {}
  }

  function appendLog(message, cls = 'log-l4') {
    const logEl = document.getElementById('l4-terminal-log');
    if (!logEl) return;
    const row = document.createElement('div');
    row.className = cls;
    const ts = new Date().toTimeString().split(' ')[0];
    row.textContent = `[${ts}] ${message}`;
    logEl.appendChild(row);
    logEl.scrollTop = logEl.scrollHeight;
  }

  function showStatusBanner(msg) {
    const banner = document.getElementById('l4-status-banner');
    if (!banner) return;
    banner.textContent = msg;
    banner.style.display = 'block';
  }

  // =========================================================================
  // 4. Filterable BPMN Workflow Database Rendering
  // =========================================================================
  function getFilteredWorkflows() {
    return state.workflows.filter(wf => {
      if (state.filters.intentClass !== 'ALL' && wf.intent_class !== state.filters.intentClass) {
        return false;
      }
      if (state.filters.status !== 'ALL' && wf.status !== state.filters.status) {
        return false;
      }
      if (state.filters.searchQuery) {
        const q = state.filters.searchQuery.toLowerCase();
        const hay = [
          wf.workflow_id,
          wf.title,
          wf.intent_class,
          wf.scenario,
          wf.description,
          wf.l5_governance_interface?.required_vc || '',
          wf.physics_check_params?.required_material_input || ''
        ].join(' ').toLowerCase();
        if (!hay.includes(q)) return false;
      }
      return true;
    });
  }

  function renderWorkflowDatabaseGrid() {
    const grid = document.getElementById('workflow-db-grid');
    const countBadge = document.getElementById('db-count-badge');
    if (countBadge) countBadge.textContent = String(state.workflows.length);
    if (!grid) return;

    const filtered = getFilteredWorkflows();
    if (filtered.length === 0) {
      grid.innerHTML = `
        <div class="wf-card" style="grid-column: 1 / -1; text-align: center; cursor: default;">
          <div class="wf-title">No workflows match current filter criteria</div>
          <p class="wf-desc">Try resetting the Intent Class, Status, or Search Query filters above.</p>
        </div>
      `;
      return;
    }

    grid.innerHTML = filtered.map(wf => {
      const isSelected = wf.workflow_id === state.selectedWorkflowId;
      let statusCls = 'status-active-bound';
      if (wf.status === 'TEMPLATE') statusCls = 'status-template';
      else if (wf.status === 'SPECIALIZED_CUSTOM') statusCls = 'status-custom';

      const vc = wf.l5_governance_interface?.required_vc || 'None';
      const mj = Number(wf.escrow_lock_params?.exergy_escrow_mj ?? 0).toFixed(1);
      const vt = (Number(mj) / 3.6).toFixed(2);

      return `
        <div class="wf-card ${isSelected ? 'selected-wf' : ''}"
             data-wf-id="${wf.workflow_id}"
             data-intent-class="${wf.intent_class}"
             data-status="${wf.status}">
          <div>
            <div class="wf-card-top">
              <span class="wf-id-mono">${wf.workflow_id}</span>
              <span class="wf-status-badge ${statusCls}">${wf.status}</span>
            </div>
            <div class="wf-title">${wf.title}</div>
            <p class="wf-desc">${wf.description}</p>
          </div>
          <div class="wf-meta-strip">
            <span class="wf-tag">${wf.intent_class}</span>
            <span class="wf-tag">${wf.scenario}</span>
            <span class="wf-tag">VC: ${vc}</span>
            <span class="wf-tag">Escrow: ${mj} MJ (${vt} VT)</span>
          </div>
        </div>
      `;
    }).join('');

    grid.querySelectorAll('.wf-card[data-wf-id]').forEach(card => {
      card.addEventListener('click', () => {
        const wfId = card.getAttribute('data-wf-id');
        selectWorkflowForDesigner(wfId);
      });
    });
  }

  // =========================================================================
  // 5. BPMN 2.0 Specialization Studio & Live Compiler
  // =========================================================================
  function selectWorkflowForDesigner(wfId) {
    const wf = state.workflows.find(w => w.workflow_id === wfId) || state.workflows[0];
    if (!wf) return;
    state.selectedWorkflowId = wf.workflow_id;

    const pill = document.getElementById('designer-active-wf-pill');
    if (pill) pill.textContent = `Loaded: ${wf.workflow_id} [${wf.status}]`;

    // Populate Specialization Form Inputs
    const baseSlug = wf.workflow_id.replace(/-v\d+$/, '').replace(/-template$/, '');
    const setVal = (id, val) => {
      const el = document.getElementById(id);
      if (el) el.value = String(val);
    };

    setVal('spec-workflow-id', `${baseSlug}-custom-v2`);
    setVal('spec-workflow-title', `${wf.title} (Specialized)`);
    setVal('spec-intent-class', wf.intent_class || 'mesh:FabricationIntent');
    setVal('spec-scenario-label', `${wf.scenario} (Specialized)`);
    setVal('spec-min-kcal', wf.physics_check_params?.min_caloric_floor_kcal ?? 800);
    setVal('spec-min-focus', wf.physics_check_params?.min_focus_pct ?? 20);
    setVal('spec-material-input', wf.physics_check_params?.required_material_input || 'None');
    setVal('spec-escrow-mj', wf.escrow_lock_params?.exergy_escrow_mj ?? 16.2);
    setVal('spec-demurrage-exempt', String(Boolean(wf.escrow_lock_params?.demurrage_exempt)));
    setVal('spec-fallback-branch', wf.escrow_lock_params?.fallback_on_failure || 'QueueUntilRested');

    renderWorkflowDatabaseGrid();
    updateLiveDesignerCanvasAndJson();
    appendLog(`[L4-STUDIO] Loaded workflow '${wf.workflow_id}' (${wf.intent_class}) into BPMN 2.0 Specialization Studio.`, 'log-l4');
  }

  function buildSpecializedWorkflowFromForm() {
    const baseWf = state.workflows.find(w => w.workflow_id === state.selectedWorkflowId) || DEFAULT_WORKFLOWS[0];
    const getVal = (id, fallback) => {
      const el = document.getElementById(id);
      return el && el.value !== undefined && el.value !== '' ? el.value : fallback;
    };

    const wfId = getVal('spec-workflow-id', 'wf-specialized-custom-v2').trim();
    const title = getVal('spec-workflow-title', `${baseWf.title} (Specialized)`).trim();
    const intentClass = getVal('spec-intent-class', baseWf.intent_class);
    const scenario = getVal('spec-scenario-label', baseWf.scenario);
    const minKcal = Number(getVal('spec-min-kcal', 800));
    const minFocus = Number(getVal('spec-min-focus', 20));
    const materialInput = getVal('spec-material-input', 'None');
    const escrowMj = Number(getVal('spec-escrow-mj', 16.2));
    const demurrageExempt = getVal('spec-demurrage-exempt', 'false') === 'true';
    const fallbackBranch = getVal('spec-fallback-branch', 'QueueUntilRested');

    const specialized = {
      workflow_id: wfId,
      title: title,
      intent_class: intentClass,
      scenario: scenario,
      status: 'SPECIALIZED_CUSTOM',
      version: '2.0.0-custom',
      parent_template_id: baseWf.workflow_id,
      description: `Specialized from ${baseWf.workflow_id}: requires >= ${minKcal} kcal, >= ${minFocus}% Focus, Material=${materialInput}, Escrow=${escrowMj.toFixed(1)} MJ, Fallback=${fallbackBranch}.`,
      l5_governance_interface: {
        ...(baseWf.l5_governance_interface || {
          policy_id: 'pol-custom-l5',
          required_vc: 'Solar_Grid_Apprentice_VC',
          max_dijkstra_hops: 2.0,
          zk_circuit: 'BBS_Plus_Selective_Disclosure_VC'
        })
      },
      physics_check_params: {
        min_caloric_floor_kcal: minKcal,
        min_focus_pct: minFocus,
        required_material_input: materialInput,
        required_material_qty: materialInput === 'None' ? 0 : 1,
        water_reserve_min_l: baseWf.physics_check_params?.water_reserve_min_l ?? 10.0
      },
      escrow_lock_params: {
        exergy_escrow_mj: escrowMj,
        value_tokens_vt: Number((escrowMj / 3.6).toFixed(2)),
        demurrage_exempt: demurrageExempt,
        fallback_on_failure: fallbackBranch
      },
      actuation_target: {
        ...(baseWf.actuation_target || {
          oasis_task_id: 1,
          bounty_id: 'CUSTOM-L4',
          target_voxel: '(9, 2, 7)',
          expected_output_delta: 'Custom Specialized Actuation'
        })
      },
      telemetry_aggregates: {
        executions_24h: 1,
        pass_rate_pct: 100.0,
        avg_latency_ms: 11.0,
        escrowed_mj: escrowMj
      }
    };

    specialized.bpmn_stages = buildBpmnStagesForWorkflow(specialized);
    return specialized;
  }

  function updateLiveDesignerCanvasAndJson() {
    const previewWf = buildSpecializedWorkflowFromForm();

    // Update 6 stage cards in BPMN canvas
    previewWf.bpmn_stages.forEach(st => {
      const exprEl = document.getElementById(`expr-stage-${st.step}`);
      if (exprEl) exprEl.textContent = st.expression;
      const cardEl = document.getElementById(`designer-stage-${st.step}`);
      if (cardEl) {
        if (st.step === state.activeStageStep) {
          cardEl.classList.add('active-stage');
        } else {
          cardEl.classList.remove('active-stage');
        }
      }
    });

    const jsonPre = document.getElementById('bpmn-json-preview');
    if (jsonPre) {
      jsonPre.textContent = JSON.stringify(previewWf, null, 2);
    }
  }

  function saveSpecializedWorkflowToDatabase() {
    const newWf = buildSpecializedWorkflowFromForm();
    const existingIdx = state.workflows.findIndex(w => w.workflow_id === newWf.workflow_id);
    if (existingIdx >= 0) {
      state.workflows[existingIdx] = newWf;
    } else {
      state.workflows.push(newWf);
    }

    state.selectedWorkflowId = newWf.workflow_id;
    persistWorkflowsToStorage();

    // Broadcast L4_WORKFLOW_SAVED over BroadcastChannel('oasis_companion_bus')
    const busMsg = {
      type: 'L4_WORKFLOW_SAVED',
      workflow: newWf,
      workflows: state.workflows,
      timestamp: Date.now()
    };
    try {
      if (oasisBus) oasisBus.postMessage(busMsg);
    } catch (_e) {}

    renderWorkflowDatabaseGrid();
    updateLiveDesignerCanvasAndJson();
    renderDiagnosticsUI();

    showStatusBanner(
      `✅ Saved '${newWf.workflow_id}' (${newWf.intent_class}) to Layer 4 Workflow Database & published to Layer 5 Governance Manager!`
    );
    appendLog(
      `[L4-DB-SAVE] Saved specialized workflow '${newWf.workflow_id}' (minKcal=${newWf.physics_check_params.min_caloric_floor_kcal}, material='${newWf.physics_check_params.required_material_input}', escrow=${newWf.escrow_lock_params.exergy_escrow_mj} MJ) -> Broadcast L4_WORKFLOW_SAVED.`,
      'log-ok'
    );
    return newWf;
  }

  function exportBpmnJsonFile() {
    const wf = buildSpecializedWorkflowFromForm();
    const jsonStr = JSON.stringify(wf, null, 2);
    try {
      if (typeof Blob !== 'undefined' && typeof URL !== 'undefined' && URL.createObjectURL) {
        const blob = new Blob([jsonStr], { type: 'application/json' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = `${wf.workflow_id}.bpmn.json`;
        if (document.body) {
          document.body.appendChild(a);
          a.click();
          document.body.removeChild(a);
        }
        URL.revokeObjectURL(url);
      }
    } catch (_e) {}

    showStatusBanner(`📥 Exported ${wf.workflow_id}.bpmn.json (${jsonStr.length} bytes)`);
    appendLog(`[L4-EXPORT] Exported canonical BPMN 2.0 definition '${wf.workflow_id}.bpmn.json'.`, 'log-l3');
  }

  // =========================================================================
  // 6. Section 3: Aggregated Diagnostics UI & Optional Fetch
  // =========================================================================
  function renderDiagnosticsUI() {
    const totalEscrow = state.workflows.reduce((acc, w) => acc + Number(w.escrow_lock_params?.exergy_escrow_mj || 0), 0);
    const totalVt = totalEscrow / 3.6;

    const elIntents = document.getElementById('diag-total-intents');
    if (elIntents) elIntents.textContent = `${state.diagnostics.totalIntentsIngested} Intents`;

    const elQueue = document.getElementById('diag-jobqueue-depth');
    if (elQueue) elQueue.textContent = `${state.diagnostics.activeJobQueueDepth} Queued`;

    const elEscrow = document.getElementById('diag-total-escrow');
    if (elEscrow) elEscrow.textContent = `${totalEscrow.toFixed(1)} MJ (${totalVt.toFixed(2)} VT)`;
  }

  async function loadExternalWorkflowDatabaseIfServed() {
    const candidateUrls = ['./workflow_database.json', '../workflows/workflow_database.json'];
    for (const url of candidateUrls) {
      try {
        const res = await fetch(url);
        if (res && res.ok) {
          const data = await res.json();
          if (Array.isArray(data.workflows) && data.workflows.length > 0) {
            data.workflows.forEach(wf => {
              if (!wf.bpmn_stages) wf.bpmn_stages = buildBpmnStagesForWorkflow(wf);
              const idx = state.workflows.findIndex(existing => existing.workflow_id === wf.workflow_id);
              if (idx >= 0) state.workflows[idx] = wf;
              else state.workflows.push(wf);
            });
            loadStoredWorkflows();
            renderWorkflowDatabaseGrid();
            updateLiveDesignerCanvasAndJson();
            appendLog(`[L4-JSON] Loaded ${data.workflows.length} workflows from ${url}.`, 'log-l3');
            break;
          }
        }
      } catch (_e) {}
    }
  }

  // =========================================================================
  // 7. Event Listeners & Initialization
  // =========================================================================
  function initEvents() {
    // Database Filter Controls
    const elFilterClass = document.getElementById('filter-intent-class');
    if (elFilterClass) {
      elFilterClass.addEventListener('change', (e) => {
        state.filters.intentClass = e.target.value;
        renderWorkflowDatabaseGrid();
      });
    }

    const elFilterStatus = document.getElementById('filter-workflow-status');
    if (elFilterStatus) {
      elFilterStatus.addEventListener('change', (e) => {
        state.filters.status = e.target.value;
        renderWorkflowDatabaseGrid();
      });
    }

    const elFilterSearch = document.getElementById('filter-search-query');
    if (elFilterSearch) {
      elFilterSearch.addEventListener('input', (e) => {
        state.filters.searchQuery = e.target.value || '';
        renderWorkflowDatabaseGrid();
      });
    }

    const btnResetFilters = document.getElementById('btn-reset-filters');
    if (btnResetFilters) {
      btnResetFilters.addEventListener('click', () => {
        state.filters.intentClass = 'ALL';
        state.filters.status = 'ALL';
        state.filters.searchQuery = '';
        if (elFilterClass) elFilterClass.value = 'ALL';
        if (elFilterStatus) elFilterStatus.value = 'ALL';
        if (elFilterSearch) elFilterSearch.value = '';
        renderWorkflowDatabaseGrid();
      });
    }

    // BPMN Stage Card clicks
    document.querySelectorAll('#bpmn-designer-canvas .bpmn-node-card').forEach(card => {
      card.addEventListener('click', () => {
        state.activeStageStep = Number(card.getAttribute('data-stage')) || 1;
        updateLiveDesignerCanvasAndJson();
      });
    });

    // Specialization Studio Input Listeners
    const specInputIds = [
      'spec-workflow-id',
      'spec-workflow-title',
      'spec-intent-class',
      'spec-scenario-label',
      'spec-min-kcal',
      'spec-min-focus',
      'spec-material-input',
      'spec-escrow-mj',
      'spec-demurrage-exempt',
      'spec-fallback-branch'
    ];
    specInputIds.forEach(id => {
      const el = document.getElementById(id);
      if (el) {
        el.addEventListener('input', updateLiveDesignerCanvasAndJson);
        el.addEventListener('change', updateLiveDesignerCanvasAndJson);
      }
    });

    // Save Specialized Workflow Button
    const btnSave = document.getElementById('btn-save-specialized-workflow');
    if (btnSave) {
      btnSave.addEventListener('click', saveSpecializedWorkflowToDatabase);
    }

    // Export .bpmn.json Button
    const btnExport = document.getElementById('btn-export-bpmn-json');
    if (btnExport) {
      btnExport.addEventListener('click', exportBpmnJsonFile);
    }

    // Cross-tab storage listener
    if (typeof window !== 'undefined' && typeof window.addEventListener === 'function') {
      window.addEventListener('storage', (ev) => {
        if (ev && ev.key === 'oasis_l4_workflow_db') {
          loadStoredWorkflows();
          renderWorkflowDatabaseGrid();
        }
      });
    }
  }

  function init() {
    loadStoredWorkflows();
    persistWorkflowsToStorage();
    initEvents();
    renderWorkflowDatabaseGrid();
    selectWorkflowForDesigner(state.selectedWorkflowId);
    renderDiagnosticsUI();
    loadExternalWorkflowDatabaseIfServed();
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }

  // Expose API for automated testing and cross-layer inspection
  window.__COLLECTIVE_BPMN_SCHEDULER__ = {
    state,
    DEFAULT_WORKFLOWS,
    getFilteredWorkflows,
    selectWorkflowForDesigner,
    buildSpecializedWorkflowFromForm,
    saveSpecializedWorkflowToDatabase,
    exportBpmnJsonFile
  };
})();
