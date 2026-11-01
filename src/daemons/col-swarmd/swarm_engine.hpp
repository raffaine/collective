#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#if __has_include("stack/shared_stack_runtime.hpp")
#include "stack/shared_stack_runtime.hpp"
#elif __has_include("../../stack/shared_stack_runtime.hpp")
#include "../../stack/shared_stack_runtime.hpp"
#else
#include "../../../src/stack/shared_stack_runtime.hpp"
#endif

namespace oasis::swarm {

// ============================================================================
// Deterministic Integer PRNG (SplitMix64 — Zero IEEE-754 Floats)
// ============================================================================

struct alignas(8) SplitMix64 {
    uint64_t state{0x9E3779B97F4A7C15ULL};

    constexpr explicit SplitMix64(uint64_t seed = 0x9E3779B97F4A7C15ULL) noexcept
        : state(seed == 0ULL ? 0x9E3779B97F4A7C15ULL : seed) {}

    [[nodiscard]] constexpr uint64_t Next() noexcept {
        uint64_t z = (state += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30U)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27U)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31U);
    }

    [[nodiscard]] constexpr uint64_t NextBounded(uint64_t bound) noexcept {
        return (bound == 0ULL) ? 0ULL : (Next() % bound);
    }
};
static_assert(sizeof(SplitMix64) == 8, "SplitMix64 must be 8 bytes.");
static_assert(alignof(SplitMix64) == 8, "SplitMix64 must be 8-byte aligned.");

// ============================================================================
// Shared-Governance Peer Node Cluster Models (-Wpadded Clean, Zero Floats)
// ============================================================================

struct alignas(8) SwarmClusterNode {
    std::string cluster_id{};
    std::string title{};
    std::string specialty_domain{};
    uint64_t    cumulative_exergy_contributed_uj{0ULL};
    uint32_t    household_count{4U};
    uint32_t    tasks_claimed_count{0U};
    uint32_t    tasks_completed_count{0U};
    uint32_t    l6_requests_published{0U};
    uint32_t    l3_surplus_offers_posted{0U};
    [[maybe_unused]] uint8_t pad_[4]{0U};
};
static_assert(alignof(SwarmClusterNode) == 8, "SwarmClusterNode must be 8-byte aligned.");

struct alignas(8) SwarmState {
    std::vector<SwarmClusterNode> clusters{};
    SplitMix64                    rng{0xC011EC717E2026ULL};
    uint64_t                      last_stepped_tick{0ULL};
    uint64_t                      total_events_emitted{0ULL};
    uint64_t                      total_bcc_settled_uj{0ULL};
    uint32_t                      event_cadence_ticks{3U};
    uint32_t                      event_probability_bp{7000U}; // 7000 bp = 70% integer threshold
    uint32_t                      total_l6_needs_emitted{0U};
    uint32_t                      total_l3_offers_emitted{0U};

    explicit SwarmState(uint64_t seed = 0xC011EC717E2026ULL)
        : rng(seed) {
        InitializeDefaultClusters();
    }

    void InitializeDefaultClusters() {
        if (!clusters.empty()) {
            return;
        }
        {
            SwarmClusterNode north_oak{};
            north_oak.cluster_id = "cluster:north_oak_commons";
            north_oak.title = "North-Oak 5-Household Garden & Kitchen Cluster";
            north_oak.specialty_domain = "AGROECOLOGY_AND_SOIL";
            north_oak.household_count = 5U;
            clusters.push_back(north_oak);
        }
        {
            SwarmClusterNode creekside{};
            creekside.cluster_id = "cluster:creekside_guild";
            creekside.title = "Creekside 4-Garage Tool & Timber Cluster";
            creekside.specialty_domain = "JOINERY_AND_HEAVY_HAUL";
            creekside.household_count = 4U;
            clusters.push_back(creekside);
        }
        {
            SwarmClusterNode sunnyside{};
            sunnyside.cluster_id = "cluster:sunnyside_care_ring";
            sunnyside.title = "Sunnyside 6-Family Care & Hearth Cluster";
            sunnyside.specialty_domain = "HEARTH_TIFFINS_AND_CARE";
            sunnyside.household_count = 6U;
            clusters.push_back(sunnyside);
        }
    }

    [[nodiscard]] SwarmClusterNode* FindClusterMutable(std::string_view id) noexcept {
        for (auto& c : clusters) {
            if (c.cluster_id == id) {
                return &c;
            }
        }
        return clusters.empty() ? nullptr : &clusters.front();
    }
};
static_assert(alignof(SwarmState) == 8, "SwarmState must be 8-byte aligned.");

// ============================================================================
// Peer Cluster Affinity & Swarm Step Engine
// ============================================================================

namespace detail {

[[nodiscard]] inline size_t SelectClusterIndexForTask(
    const oasis::stack::L4TaskItem& task,
    SwarmState& swarm
) noexcept {
    if (swarm.clusters.size() < 3U) {
        return 0U;
    }
    // 75% domain-affinity routing, 25% SplitMix64 neighborhood load-sharing
    if (swarm.rng.NextBounded(100ULL) < 25ULL) {
        return static_cast<size_t>(swarm.rng.NextBounded(swarm.clusters.size()));
    }
    if (task.asset_id == "asset:garage_fab" ||
        task.haul_modality == oasis::stack::HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL) {
        return 1U; // cluster:creekside_guild
    }
    if (task.asset_id == "asset:pea_soup_hearth" ||
        task.haul_modality == oasis::stack::HaulModality::TIFFIN_CARRIER_HAUL) {
        return 2U; // cluster:sunnyside_care_ring
    }
    return 0U; // cluster:north_oak_commons
}

inline void EmitPeerClusterNeedEvent(
    oasis::stack::SharedStackRuntime& target_node,
    SwarmState& swarm,
    uint64_t tick
) {
    struct alignas(8) NeedTemplate {
        size_t      cluster_idx;
        const char* need_id;
        const char* summary;
        uint64_t    qty;
    };

    static constexpr NeedTemplate kTemplates[6] = {
        {2U, "need:pea_soup",             "Sunnyside Care Ring requested 4 Pea Soup Tiffins for evening distribution.", 4ULL},
        {0U, "need:thermophilic_compost", "North-Oak Commons requested 2 drums of Thermophilic Compost for pea bed top-dressing.", 2ULL},
        {1U, "need:raised_bed_frame",     "Creekside Guild requested 2 mortised Raised Bed Frames for terrace expansion.", 2ULL},
        {2U, "need:collagen_bone_broth",  "Sunnyside Care Ring requested 3 jars of 24h Collagen Bone Broth for postpartum hearth care.", 3ULL},
        {0U, "need:split_peas",           "North-Oak Commons requested 2 sacks of inoculated Split Peas for community seed swap.", 2ULL},
        {2U, "need:pasture_eggs",         "Sunnyside Care Ring requested 2 dozen Pasture Eggs for morning neighborhood breakfast.", 2ULL}
    };

    const size_t pick = (swarm.total_l6_needs_emitted == 0U)
                            ? 0U
                            : static_cast<size_t>(swarm.rng.NextBounded(6ULL));
    const NeedTemplate& tpl = kTemplates[pick];
    SwarmClusterNode& cluster = swarm.clusters[tpl.cluster_idx % swarm.clusters.size()];

    const oasis::stack::L6IntentRequest req = target_node.RequestNeed(
        tpl.need_id,
        oasis::stack::NeedTimingMode::SCHEDULED_FUTURE,
        target_node.CurrentSimDay() + 1U,
        static_cast<uint32_t>(tpl.qty),
        cluster.cluster_id
    );

    ++cluster.l6_requests_published;
    ++swarm.total_l6_needs_emitted;
    ++swarm.total_events_emitted;

    oasis::stack::L3MeshWeatherEvent ev{};
    ev.event_id = "sw_ev:" + std::to_string(swarm.total_events_emitted);
    ev.cluster_id = cluster.cluster_id;
    ev.cluster_title = cluster.title;
    ev.event_category = "L6_CLUSTER_NEED";
    ev.summary = tpl.summary;
    ev.exergy_delta_uj = req.exergy_budget_uj;
    ev.tick = tick;
    target_node.PushMeshWeatherEvent(ev);
}

inline void EmitPeerClusterSurplusOffer(
    oasis::stack::SharedStackRuntime& target_node,
    SwarmState& swarm,
    uint64_t tick
) {
    struct alignas(8) OfferTemplate {
        size_t      cluster_idx;
        const char* item_title;
        const char* summary;
        uint64_t    exergy_uj;
        uint64_t    qty;
    };

    static constexpr OfferTemplate kOffers[6] = {
        {1U, "Seasoned Salvaged Cedar Planks",           "Creekside Guild offering seasoned cedar planks via BCC.", 3200000000ULL, 6ULL},
        {0U, "Rhizobium Pea Inoculant & Mirepoix Seeds", "North-Oak Commons offering Rhizobium pea inoculant & heirloom mirepoix seeds via BCC.", 1400000000ULL, 4ULL},
        {2U, "Sterilized 304-Stainless Tiffin Carriers", "Sunnyside Care Ring offering sterilized 304-stainless tiffin carriers via BCC.", 1800000000ULL, 4ULL},
        {1U, "Screened Hardwood Pyrolysis Biochar",      "Creekside Guild offering screened hardwood pyrolysis biochar char-dust via BCC.", 2600000000ULL, 3ULL},
        {0U, "Winter Kale & Root Mirepoix Bundles",      "North-Oak Commons offering fresh winter kale & mirepoix root bundles via BCC.", 1900000000ULL, 5ULL},
        {2U, "Lacto-Fermented Kraut Garnish Crocks",     "Sunnyside Care Ring offering fermented cabbage kraut crocks (<4600 mpH) via BCC.", 1200000000ULL, 3ULL}
    };

    const size_t pick = (swarm.total_l3_offers_emitted == 0U)
                            ? 0U
                            : static_cast<size_t>(swarm.rng.NextBounded(6ULL));
    const OfferTemplate& tpl = kOffers[pick];
    SwarmClusterNode& cluster = swarm.clusters[tpl.cluster_idx % swarm.clusters.size()];

    ++cluster.l3_surplus_offers_posted;
    ++swarm.total_l3_offers_emitted;
    ++swarm.total_events_emitted;

    oasis::stack::L3SurplusOffer offer{};
    offer.offer_id = "offer:" + std::to_string(swarm.total_l3_offers_emitted);
    offer.cluster_id = cluster.cluster_id;
    offer.item_title = tpl.item_title;
    offer.exergy_value_uj = tpl.exergy_uj;
    offer.posted_tick = tick;
    offer.quantity_units = static_cast<uint32_t>(tpl.qty);
    offer.is_available = 1U;
    target_node.PushPeerSurplusOffer(offer);

    oasis::stack::L3MeshWeatherEvent ev{};
    ev.event_id = "sw_ev:" + std::to_string(swarm.total_events_emitted);
    ev.cluster_id = cluster.cluster_id;
    ev.cluster_title = cluster.title;
    ev.event_category = "L3_SURPLUS_OFFER";
    ev.summary = tpl.summary;
    ev.exergy_delta_uj = tpl.exergy_uj;
    ev.tick = tick;
    target_node.PushMeshWeatherEvent(ev);
}

} // namespace detail

inline void StepPeerClusterSwarm(
    oasis::stack::SharedStackRuntime& target_node,
    SwarmState& swarm,
    uint64_t tick
) {
    const uint64_t effective_tick = std::max(tick, target_node.CurrentTick() + 1ULL);
    swarm.last_stepped_tick = effective_tick;

    // Default Solo Steward 4-Node Personal Homestead Cluster Mode:
    // Defer external R1 neighborhood NPC swarm until explicitly enabled.
    if (!target_node.IsExternalR1SwarmEnabled()) {
        target_node.StepLocalHomesteadClusterHeartbeat(effective_tick);
        return;
    }

    swarm.InitializeDefaultClusters();
    target_node.AdvanceToTick(effective_tick);

    // 1. Collect ACTIVE_TOKEN R1 tasks to complete and R1 tasks to claim
    struct PendingAction {
        std::string task_id{};
        std::string task_title{};
        std::string cluster_id{};
        uint64_t    exergy_uj{0ULL};
        uint8_t     is_completion{0U};
        [[maybe_unused]] uint8_t pad_[7]{0U};
    };

    std::vector<PendingAction> actions;
    for (const auto& wf : target_node.GetWorkflows()) {
        for (const auto& task : wf.tasks) {
            if (task.bpmn_token_state != oasis::stack::BpmnTokenState::ACTIVE_TOKEN) {
                continue;
            }
            if (task.task_commitment_state == oasis::stack::TaskCommitmentState::CLAIMED_BY_PEER_CLUSTER &&
                effective_tick > task.last_state_change_tick) {
                PendingAction act{};
                act.task_id = task.task_id;
                act.task_title = task.title;
                act.cluster_id = task.claimed_by_cluster_id.empty()
                                     ? swarm.clusters.front().cluster_id
                                     : task.claimed_by_cluster_id;
                act.exergy_uj = task.exergy_cost_uj;
                act.is_completion = 1U;
                actions.push_back(act);
            } else if (task.task_commitment_state == oasis::stack::TaskCommitmentState::RELEASED_TO_R1_PEERS) {
                const size_t idx = detail::SelectClusterIndexForTask(task, swarm);
                PendingAction act{};
                act.task_id = task.task_id;
                act.task_title = task.title;
                act.cluster_id = swarm.clusters[idx].cluster_id;
                act.exergy_uj = task.exergy_cost_uj;
                act.is_completion = 0U;
                actions.push_back(act);
            }
        }
    }

    for (const auto& act : actions) {
        SwarmClusterNode* cluster = swarm.FindClusterMutable(act.cluster_id);
        const std::string cluster_title = (cluster != nullptr) ? cluster->title : act.cluster_id;

        if (act.is_completion != 0U) {
            if (target_node.CompleteTask(act.task_id, act.cluster_id)) {
                // Peer cluster executed thermodynamic work for target_node:
                // settle exact pairwise zero-sum BCC exergy credit (sum == 0 invariant preserved).
                (void)target_node.SettlePeerClusterBcc(
                    act.cluster_id,
                    -static_cast<int64_t>(act.exergy_uj),
                    true
                );
                if (cluster != nullptr) {
                    ++cluster->tasks_completed_count;
                    cluster->cumulative_exergy_contributed_uj += act.exergy_uj;
                }
                swarm.total_bcc_settled_uj += act.exergy_uj;
                ++swarm.total_events_emitted;

                oasis::stack::L3MeshWeatherEvent ev{};
                ev.event_id = "sw_ev:" + std::to_string(swarm.total_events_emitted);
                ev.cluster_id = act.cluster_id;
                ev.cluster_title = cluster_title;
                ev.event_category = "R1_TASK_COMPLETED";
                ev.summary = cluster_title + " completed R1 task '" + act.task_title +
                             "' and settled pairwise zero-sum BCC exergy credit.";
                ev.exergy_delta_uj = act.exergy_uj;
                ev.tick = effective_tick;
                target_node.PushMeshWeatherEvent(ev);
            }
        } else {
            if (target_node.ClaimTaskByPeerCluster(act.task_id, act.cluster_id)) {
                if (cluster != nullptr) {
                    ++cluster->tasks_claimed_count;
                }
                ++swarm.total_events_emitted;

                oasis::stack::L3MeshWeatherEvent ev{};
                ev.event_id = "sw_ev:" + std::to_string(swarm.total_events_emitted);
                ev.cluster_id = act.cluster_id;
                ev.cluster_title = cluster_title;
                ev.event_category = "R1_TASK_CLAIMED";
                ev.summary = cluster_title + " claimed R1 task '" + act.task_title + "' over Layer 3.";
                ev.exergy_delta_uj = act.exergy_uj;
                ev.tick = effective_tick;
                target_node.PushMeshWeatherEvent(ev);
            }
        }
    }

    // 2. Calmly paced semi-stochastic L6 cluster needs & L3 surplus offers
    const uint32_t cadence = (swarm.event_cadence_ticks == 0U) ? 1U : swarm.event_cadence_ticks;
    const bool due_by_cadence =
        (tick == 1ULL) ||
        ((effective_tick % static_cast<uint64_t>(cadence) == 0ULL) &&
         (swarm.rng.NextBounded(10000ULL) < static_cast<uint64_t>(swarm.event_probability_bp)));

    if (due_by_cadence) {
        if (swarm.total_l6_needs_emitted == 0U) {
            detail::EmitPeerClusterNeedEvent(target_node, swarm, effective_tick);
        } else if (swarm.total_l3_offers_emitted == 0U) {
            detail::EmitPeerClusterSurplusOffer(target_node, swarm, effective_tick);
        } else if (swarm.rng.NextBounded(2ULL) == 0ULL) {
            detail::EmitPeerClusterNeedEvent(target_node, swarm, effective_tick);
        } else {
            detail::EmitPeerClusterSurplusOffer(target_node, swarm, effective_tick);
        }
    }
}

} // namespace oasis::swarm

namespace oasis::daemons::swarmd {
using namespace oasis::swarm;
} // namespace oasis::daemons::swarmd

namespace oasis::stack {
using oasis::swarm::SplitMix64;
using oasis::swarm::SwarmClusterNode;
using oasis::swarm::SwarmState;
using oasis::swarm::StepPeerClusterSwarm;
} // namespace oasis::stack
