#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <string>

#include "core/chill_loop.hpp"

namespace oasis::orchestrator {

// ============================================================================
// PILLAR 1: Asynchronous Node Proxies (Semi-Stochastic Single-Player Multiplayer)
// ============================================================================

struct AsyncNodeProxy {
    uint32_t node_id{1};
    std::string did{"did:mesh:node01:maya"};
    std::string steward_name{"Maya (Node 01 - Lot 404)"};
    std::string role_label{"Biochar & Kiln Steward"};
    uint8_t trust_ring{1}; // R1 or R3
    double exergy_balance_kwh{-4.5}; // Negative = deficit!
    double water_balance_l{12.0};
    double caloric_balance_kcal{400.0};
    int32_t cob_biochar_balance{4};
    uint16_t active_bounty_task_id{1}; // 1=Roof Solar, 2=Backyard Swale, 3=Harvest&Cook, 4=Salvage Exchange
    std::string active_bounty_title{"BPMN #B-101: Supply +4.5 kWh Roof Solar to Lot 404 Kiln"};
    std::string required_l5_vc{"Solar_Grid_Apprentice_VC"};
    uint32_t reward_value_tokens{15};
    bool bounty_fulfilled{false};
};

struct CollectiveNetworkState {
    std::array<AsyncNodeProxy, 3> peer_nodes{{
        AsyncNodeProxy{
            1u,
            "did:mesh:node01:maya",
            "Maya (Node 01 - Lot 404)",
            "Biochar & Kiln Steward",
            1u,
            -4.5,
            12.0,
            400.0,
            4,
            1u,
            "BPMN #B-101: Supply +4.5 kWh Roof Solar to Lot 404 Kiln",
            "Solar_Grid_Apprentice_VC",
            15u,
            false
        },
        AsyncNodeProxy{
            2u,
            "did:mesh:node02:kael",
            "Kael (Node 02 - Lot 406)",
            "Hydro & Seed Bank Steward",
            1u,
            6.0,
            -8.0,
            650.0,
            2,
            2u,
            "BPMN #B-102: Dig +8.0 L/d Backyard Swale Catchment for Lot 406",
            "Hydro_Earthworks_VC",
            12u,
            false
        },
        AsyncNodeProxy{
            3u,
            "did:mesh:node03:soren",
            "Soren (Node 03 - Lot 408)",
            "Microgrid Forge Steward",
            3u,
            9.5,
            14.0,
            -1600.0,
            3,
            3u,
            "BPMN #B-103: Harvest & Cook 2 Sovereign Meals (+1,600 kcal) for Lot 408",
            "Sovereign_Kitchen_Steward_VC",
            20u,
            false
        }
    }};
    uint32_t collective_value_tokens{25}; // Player's Layer 3 mutual-credit ValueTokens
    std::string storage_mode{"GitHub/JSON Legacy Snapshot Bridge -> Horizon: L3 CRDT"};
};

[[nodiscard]] inline CollectiveNetworkState CreateDefaultCollectiveNetwork() {
    return CollectiveNetworkState{};
}

// Semi-stochastically updates peer node balances over time (deterministic function of sim_tick)
// so the broader server's production and deficits feel alive and consistent.
inline void StepAsyncNodeProxies(CollectiveNetworkState& net, uint64_t sim_tick) noexcept {
    const double phase = static_cast<double>(sim_tick % 28800ULL) / 28800.0;
    const double wave_a = std::sin(phase * 6.28318530718);
    const double wave_b = std::cos(phase * 6.28318530718);

    // Node 01: Maya (Lot 404 — Biochar & Kiln)
    auto& maya = net.peer_nodes[0];
    if (!maya.bounty_fulfilled) {
        maya.exergy_balance_kwh = -4.5 + 0.35 * wave_a;
    } else {
        maya.exergy_balance_kwh = 2.4 + 0.35 * wave_a;
    }
    maya.water_balance_l = 12.0 + 1.2 * wave_b;
    maya.caloric_balance_kcal = 400.0 + 45.0 * wave_a;

    // Node 02: Kael (Lot 406 — Hydro & Seed Bank)
    auto& kael = net.peer_nodes[1];
    kael.exergy_balance_kwh = 6.0 + 0.40 * wave_b;
    if (!kael.bounty_fulfilled) {
        kael.water_balance_l = -8.0 + 0.45 * wave_a;
    } else {
        kael.water_balance_l = 4.5 + 0.45 * wave_a;
    }
    kael.caloric_balance_kcal = 650.0 + 50.0 * wave_b;

    // Node 03: Soren (Lot 408 — Microgrid Forge)
    auto& soren = net.peer_nodes[2];
    soren.exergy_balance_kwh = 9.5 + 0.60 * wave_a;
    soren.water_balance_l = 14.0 + 0.80 * wave_b;
    if (!soren.bounty_fulfilled) {
        soren.caloric_balance_kcal = -1600.0 + 75.0 * wave_b;
    } else {
        soren.caloric_balance_kcal = 520.0 + 60.0 * wave_b;
    }
}

// Marks the matching peer Node Proxy's bounty as fulfilled, awards reward_value_tokens to
// net.collective_value_tokens, and delivers the peer node's reciprocal surplus into stock.
inline bool FulfillBpmnBountyForTask(
    CollectiveNetworkState& net,
    uint16_t completed_task_id,
    oasis::chill::GenesisCampStockComponent& stock
) noexcept {
    for (auto& node : net.peer_nodes) {
        if (node.active_bounty_task_id == completed_task_id && !node.bounty_fulfilled) {
            node.bounty_fulfilled = true;
            net.collective_value_tokens += node.reward_value_tokens;

            if (completed_task_id == 1u) {
                // Maya (Node 01): +2 Cob/Biochar lumps & resolves solar deficit
                node.exergy_balance_kwh = 2.4;
                stock.cob_earth_lumps += 2u;
            } else if (completed_task_id == 2u) {
                // Kael (Node 02): +2 Seed Packs & resolves swale catchment deficit
                node.water_balance_l = 4.5;
                stock.seed_packs_remaining += 2u;
            } else if (completed_task_id == 3u) {
                // Soren (Node 03): +1 Solar PV Kit & resolves kitchen meal deficit
                node.caloric_balance_kcal = 520.0;
                stock.solar_kits_remaining += 1u;
            }
            return true;
        }
    }
    return false;
}

inline bool FulfillBpmnBountyForTask(
    CollectiveNetworkState& net,
    uint16_t completed_task_id
) noexcept {
    oasis::chill::GenesisCampStockComponent dummy_stock{};
    return FulfillBpmnBountyForTask(net, completed_task_id, dummy_stock);
}

// ============================================================================
// PILLAR 3: BPMN 2.0 State Machine & Multi-Day Temporal Workflow Scheduler
// ============================================================================

enum class BpmnStage : uint8_t {
    REJECTED         = 0,
    L6_INGEST        = 1,
    L5_TRUST_GATE    = 2,
    L2_PHYSICS_CHECK = 3,
    L3_ESCROW_LOCK   = 4,
    L1B_ACTUATION    = 5,
    L3_SETTLED       = 6
};

[[nodiscard]] constexpr const char* FormatBpmnStageBadge(BpmnStage stage) noexcept {
    switch (stage) {
        case BpmnStage::REJECTED:         return "[REJECTED]";
        case BpmnStage::L6_INGEST:        return "[L6 INGEST]";
        case BpmnStage::L5_TRUST_GATE:    return "[L5 GATE]";
        case BpmnStage::L2_PHYSICS_CHECK: return "[L2 PHYSICS]";
        case BpmnStage::L3_ESCROW_LOCK:   return "[L5 LOCKED]";
        case BpmnStage::L1B_ACTUATION:    return "[L1B EXECUTING]";
        case BpmnStage::L3_SETTLED:       return "[L3 SETTLED]";
        default:                          return "[L6 INGEST]";
    }
}

// Per core/6_intent/INTENT_SPEC.md Section 4 (Intent Lifecycles), citizens in Layer 6 see
// Intent Lifecycle states ([PENDING], [REJECTED], [ESCROWED], [ACTUATING], [FULFILLED]).
[[nodiscard]] constexpr const char* FormatIntentLifecycleBadge(BpmnStage stage) noexcept {
    switch (stage) {
        case BpmnStage::REJECTED:         return "[REJECTED]";
        case BpmnStage::L6_INGEST:
        case BpmnStage::L5_TRUST_GATE:
        case BpmnStage::L2_PHYSICS_CHECK: return "[PENDING]";
        case BpmnStage::L3_ESCROW_LOCK:   return "[ESCROWED]";
        case BpmnStage::L1B_ACTUATION:    return "[ACTUATING]";
        case BpmnStage::L3_SETTLED:       return "[FULFILLED]";
        default:                          return "[PENDING]";
    }
}

struct ScheduledWorkflowSlot {
    uint32_t scheduled_day{1}; // Day 1..7
    uint16_t task_id{2}; // 1=Roof Solar, 2=Backyard Swale, 3=Harvest & Cook, 4=Requisition Kits, 5=RestIntent
    std::string task_label{"Dig Backyard Rain Swale"};
    std::string required_l5_vc{"Hydro_Earthworks_VC"};
    bool l5_gate_locked{false};
    bool l3_escrow_locked{false};
    bool dispatched_to_founder{false};
    bool completed{false};
    BpmnStage bpmn_stage{BpmnStage::L6_INGEST};
};

struct TemporalScheduleState {
    std::array<ScheduledWorkflowSlot, 4> days{{
        ScheduledWorkflowSlot{
            1u,
            2u,
            "Dig Backyard Rain Swale",
            "Hydro_Earthworks_VC",
            true,
            true,
            false,
            false,
            BpmnStage::L3_ESCROW_LOCK
        },
        ScheduledWorkflowSlot{
            2u,
            1u,
            "Mount Roof Solar Panel",
            "Solar_Grid_Apprentice_VC",
            true,
            true,
            false,
            false,
            BpmnStage::L3_ESCROW_LOCK
        },
        ScheduledWorkflowSlot{
            3u,
            3u,
            "Harvest Garden & Cook 2 Meals",
            "Sovereign_Kitchen_Steward_VC",
            true,
            true,
            false,
            false,
            BpmnStage::L3_ESCROW_LOCK
        },
        ScheduledWorkflowSlot{
            4u,
            4u,
            "Settle L3 Escrow & Requisition +2 PV Kits",
            "Solar_Grid_Apprentice_VC",
            true,
            true,
            false,
            false,
            BpmnStage::L3_ESCROW_LOCK
        }
    }};
    bool auto_dispatch_enabled{true};
};

[[nodiscard]] inline TemporalScheduleState CreateDefaultTemporalSchedule() {
    return TemporalScheduleState{};
}

// Matches formatSimClock in shell_minimal.html:
// Day 1 starts at 06:00 (360 minutes from midnight); 20 ticks = 1 simulated minute; 1440 minutes = 1 day.
[[nodiscard]] constexpr uint32_t ComputeSimDayFromTick(uint64_t sim_tick) noexcept {
    const uint32_t total_minutes = 360u + static_cast<uint32_t>(sim_tick / 20ULL);
    return (total_minutes / 1440u) + 1u;
}

inline bool LockScheduleL5Gates(TemporalScheduleState& sched, uint8_t founder_trust_ring = 1) noexcept {
    if (founder_trust_ring > 3u) {
        for (auto& slot : sched.days) {
            if (!slot.completed && !slot.dispatched_to_founder) {
                slot.l5_gate_locked = false;
                slot.l3_escrow_locked = false;
                slot.bpmn_stage = BpmnStage::L5_TRUST_GATE;
            }
        }
        return false;
    }

    for (auto& slot : sched.days) {
        if (!slot.completed) {
            slot.l5_gate_locked = true;
            slot.l3_escrow_locked = true;
            if (!slot.dispatched_to_founder) {
                slot.bpmn_stage = BpmnStage::L3_ESCROW_LOCK;
            }
        }
    }
    return true;
}

[[nodiscard]] inline uint16_t StepTemporalSchedule(
    TemporalScheduleState& sched,
    uint64_t sim_tick,
    bool founder_is_idle
) noexcept {
    if (!sched.auto_dispatch_enabled || !founder_is_idle) {
        return 0u;
    }

    const uint32_t current_day = ComputeSimDayFromTick(sim_tick);
    for (auto& slot : sched.days) {
        if (!slot.completed &&
            !slot.dispatched_to_founder &&
            slot.l5_gate_locked &&
            slot.scheduled_day <= current_day) {
            slot.dispatched_to_founder = true;
            slot.bpmn_stage = BpmnStage::L1B_ACTUATION;
            return slot.task_id;
        }
    }
    return 0u;
}

inline bool UpdateScheduleSlotTask(
    TemporalScheduleState& sched,
    uint32_t day_index_1_based,
    uint16_t new_task_id
) {
    if (day_index_1_based < 1u || day_index_1_based > sched.days.size()) {
        return false;
    }
    if (new_task_id < 1u || new_task_id > 5u) {
        return false;
    }

    auto& slot = sched.days[day_index_1_based - 1u];
    slot.scheduled_day = day_index_1_based;
    slot.task_id = new_task_id;

    switch (new_task_id) {
        case 1u:
            slot.task_label = "Mount Roof Solar Panel";
            slot.required_l5_vc = "Solar_Grid_Apprentice_VC";
            break;
        case 2u:
            slot.task_label = "Dig Backyard Rain Swale";
            slot.required_l5_vc = "Hydro_Earthworks_VC";
            break;
        case 3u:
            slot.task_label = "Harvest Garden & Cook 2 Meals";
            slot.required_l5_vc = "Sovereign_Kitchen_Steward_VC";
            break;
        case 4u:
            slot.task_label = "Settle L3 Escrow & Requisition +2 PV Kits";
            slot.required_l5_vc = "Solar_Grid_Apprentice_VC";
            break;
        case 5u:
        default:
            slot.task_label = "mesh:RestIntent — Patio Hearth Rest & Recovery";
            slot.required_l5_vc = "None_Inalienable_Rest_Right";
            break;
    }

    slot.l5_gate_locked = true;
    slot.l3_escrow_locked = true;
    slot.dispatched_to_founder = false;
    slot.completed = false;
    slot.bpmn_stage = BpmnStage::L3_ESCROW_LOCK;
    return true;
}

inline bool CompleteScheduledTask(
    TemporalScheduleState& sched,
    uint16_t completed_task_id
) noexcept {
    // First prefer a slot that was already dispatched to the Founder
    for (auto& slot : sched.days) {
        if (!slot.completed && slot.dispatched_to_founder && slot.task_id == completed_task_id) {
            slot.completed = true;
            slot.bpmn_stage = BpmnStage::L3_SETTLED;
            return true;
        }
    }
    // Otherwise mark the earliest uncompleted slot matching completed_task_id
    for (auto& slot : sched.days) {
        if (!slot.completed && slot.task_id == completed_task_id) {
            slot.dispatched_to_founder = true;
            slot.completed = true;
            slot.bpmn_stage = BpmnStage::L3_SETTLED;
            return true;
        }
    }
    return false;
}

} // namespace oasis::orchestrator

namespace oasis {
    using oasis::orchestrator::AsyncNodeProxy;
    using oasis::orchestrator::CollectiveNetworkState;
    using oasis::orchestrator::CreateDefaultCollectiveNetwork;
    using oasis::orchestrator::StepAsyncNodeProxies;
    using oasis::orchestrator::FulfillBpmnBountyForTask;
    using oasis::orchestrator::BpmnStage;
    using oasis::orchestrator::FormatBpmnStageBadge;
    using oasis::orchestrator::FormatIntentLifecycleBadge;
    using oasis::orchestrator::ScheduledWorkflowSlot;
    using oasis::orchestrator::TemporalScheduleState;
    using oasis::orchestrator::CreateDefaultTemporalSchedule;
    using oasis::orchestrator::ComputeSimDayFromTick;
    using oasis::orchestrator::LockScheduleL5Gates;
    using oasis::orchestrator::StepTemporalSchedule;
    using oasis::orchestrator::UpdateScheduleSlotTask;
    using oasis::orchestrator::CompleteScheduledTask;
} // namespace oasis
