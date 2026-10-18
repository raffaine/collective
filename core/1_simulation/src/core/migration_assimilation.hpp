#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <flecs.h>

#include "uhai/uhai_ring_buffer.hpp"
#include "core/fabrication.hpp"
#include "core/homeostasis.hpp"
#include "core/chill_loop.hpp"
#include "core/raycaster.hpp"
#include "core/stochastic_boundary.hpp"
#include "core/time_warp_controller.hpp"

#ifndef OASIS_PSYCHOLOGY_DEFINED
#define OASIS_PSYCHOLOGY_DEFINED
namespace oasis::psychology {

struct PersonalityFacets {
    float openness{0.8f};
    float conscientiousness{0.9f};
    float extraversion{0.6f};
    float agreeableness{0.7f};
    float neuroticism{0.3f};
};

// Memory Event Type IDs
enum class MemoryEventType : uint16_t {
    UNKNOWN = 0,
    GENESIS_AWAKENING = 1,
    MUNICIPAL_CODE_RAID = 2,
    SIPHON_POWER_GRID = 3,
    FIRST_KILOWATT_GENERATED = 4,
    WATER_MAIN_TAPPED = 5,
    SOLAR_INVERTER_REPAIR = 6,
    TOOL_DROPPED = 7,
    COMMUNAL_FEAST = 8,
    STRANGE_MOOD_MASTERWORK = 9,
    FREEZING_WINTER_NIGHT = 10,
    HOA_VIOLATION_NOTICE = 11
};

// Memory Bitfield Flags
namespace MemoryFlags {
    constexpr uint8_t NONE             = 0x00;
    constexpr uint8_t PERMANENT_MEMORY = 0x01; // salience >= 80 root trauma / achievement
    constexpr uint8_t TRAUMA           = 0x02; // Negative valence severe event
    constexpr uint8_t EUPHORIA         = 0x04; // Positive valence major breakthrough
}

#pragma pack(push, 8)
struct alignas(8) EpisodicMemoryNode {
    uint64_t timestamp_tick{0};     // Tick when event occurred (8 bytes)
    uint16_t event_type{0};          // MemoryEventType enum (2 bytes)
    int16_t  emotional_valence{0};   // Emotional valence [-100..+100] (2 bytes)
    uint8_t  salience{0};            // Salience score [0..100] (1 byte)
    uint8_t  flags{0};               // Bitfield flags (1 byte)
    char     description[8]{};       // Short label (8 bytes)
    uint16_t reserved{0};            // Alignment padding (2 bytes)
};
#pragma pack(pop)
static_assert(sizeof(EpisodicMemoryNode) == 24, "EpisodicMemoryNode must be exactly 24 bytes packed");

struct EpisodicMemoryRing {
    static constexpr size_t CAPACITY = 32;

    EpisodicMemoryNode slots[CAPACITY]{};
    uint32_t head{0};
    uint32_t count{0};
    uint32_t total_recorded{0};
    uint32_t permanent_count{0};

    // Push with DF trauma-protection eviction
    uint32_t Push(const EpisodicMemoryNode& input_node) {
        EpisodicMemoryNode node = input_node;
        total_recorded++;

        if (node.salience >= 80) {
            node.flags |= MemoryFlags::PERMANENT_MEMORY;
        }

        uint32_t target_slot = head;

        if (count < CAPACITY) {
            target_slot = head;
            slots[target_slot] = node;
            head = (head + 1) % CAPACITY;
            count++;
            if ((node.flags & MemoryFlags::PERMANENT_MEMORY) != 0) {
                permanent_count++;
            }
            return target_slot;
        }

        int32_t evict_idx = -1;
        uint32_t min_salience = 0xFFFFFFFF;

        for (size_t i = 0; i < CAPACITY; ++i) {
            if ((slots[i].flags & MemoryFlags::PERMANENT_MEMORY) == 0 && slots[i].salience < min_salience) {
                min_salience = slots[i].salience;
                evict_idx = static_cast<int32_t>(i);
            }
        }

        if (evict_idx != -1) {
            target_slot = static_cast<uint32_t>(evict_idx);
            slots[target_slot] = node;
            if ((node.flags & MemoryFlags::PERMANENT_MEMORY) != 0) {
                permanent_count++;
            }
            return target_slot;
        }

        target_slot = head;
        const bool prev_was_permanent = (slots[head].flags & MemoryFlags::PERMANENT_MEMORY) != 0;
        slots[head] = node;
        head = (head + 1) % CAPACITY;
        const bool new_is_permanent = (node.flags & MemoryFlags::PERMANENT_MEMORY) != 0;
        if (!prev_was_permanent && new_is_permanent) {
            permanent_count++;
        } else if (prev_was_permanent && !new_is_permanent) {
            if (permanent_count > 0) permanent_count--;
        }
        return target_slot;
    }
};

struct PsychologyComponent {
    PersonalityFacets facets{0.8f, 0.9f, 0.6f, 0.7f, 0.3f};
    float stress{0.25f};
    float mood{0.50f};
    float focus{0.75f};
    EpisodicMemoryRing memory_ring{};
};

} // namespace oasis::psychology
#endif // OASIS_PSYCHOLOGY_DEFINED

namespace oasis::migration {

// ============================================================================
// EPIC-11 (STORY-11.1, STORY-11.2, STORY-11.3): Migration Waves, Legacy NPC
// Friction & Progressive Trust Ring (R6 -> R3 -> R1) Assimilation
// ============================================================================

constexpr uint32_t MAX_PARCEL_NPCS             = 16u;      // Matches SocialMutualAidComponent::MAX_NPCS = 16
constexpr uint64_t SOLO_GENESIS_GRACE_TICKS    = 86400ULL; // 3 simulated days at 28,800 ticks/day
constexpr uint64_t PRE_WAVE_WARNING_LEAD_TICKS = 14400ULL; // 12 simulated hours at 1,200 ticks/hr
constexpr float    REQUIRED_POTW_MJ_FOR_R3     = 36.0f;    // 10.0 Exergy Credits (36.0 MJ)
constexpr uint32_t REQUIRED_SHELTER_DAYS_FOR_R3= 7u;
constexpr uint32_t REQUIRED_R3_DAYS_FOR_R1     = 30u;

// Dynamic Voxel Material IDs for NPC avatars in DynamicVoxelStore
constexpr uint8_t MAT_MIGRANT_R6  = 27u;
constexpr uint8_t MAT_RESIDENT_R3 = 28u;
constexpr uint8_t MAT_STEWARD_R1  = 29u;

enum LegacyMindsetFlags : uint8_t {
    MINDSET_NONE        = 0,
    SCARCITY_ANXIETY    = 1 << 0,
    HOARDING_ACTIVE     = 1 << 1,
    CONFLICT_REGRESSION = 1 << 2,
    RESERVE_POWER_DRAW  = 1 << 3
};

struct AssimilationChecklist {
    bool enclosed_shelter_ok{false}; // > 18.0°C
    bool hydration_ok{false};        // > 50%
    bool calories_ok{false};         // > 1500 kcal
    float potw_sweat_equity_mj{0.0f};// target 36.0 MJ
    uint32_t sheltered_days{0};      // target 7 days
    uint8_t current_ring{6};

    [[nodiscard]] bool all_survival_needs_met() const noexcept {
        return enclosed_shelter_ok && hydration_ok && calories_ok;
    }
};

struct LegacyMigrantComponent {
    uint32_t npc_index{0};
    uint32_t wave_id{0};
    uint64_t did_hash{0};
    uint8_t trust_ring_tier{6}; // 6 = R6 Unverified Migrant, 3 = R3 Probationary Resident, 1 = R1 Vouched Steward
    uint8_t mindset_flags{SCARCITY_ANXIETY};
    uint32_t sheltered_days_consecutive{0};
    uint32_t days_in_r3_consecutive{0};
    float potw_sweat_equity_mj{0.0f};
    glm::ivec3 last_stamped_voxel{-999, -999, -999};
    glm::vec3 patrol_target{7.0f, 1.0f, 10.0f};
    AssimilationChecklist checklist{};
};

struct MigrationStagingQueue {
    static constexpr size_t MAX_STAGED_COHORTS = 32;
    std::vector<oasis::uhai::MigrationWaveArrivalEvent> staged_waves{};
    uint32_t staged_overflow_migrants{0};

    void StageOverflow(const oasis::uhai::MigrationWaveArrivalEvent& wave, uint32_t excess_count) {
        if (excess_count == 0) return;
        staged_overflow_migrants += excess_count;
        if (staged_waves.size() < MAX_STAGED_COHORTS) {
            oasis::uhai::MigrationWaveArrivalEvent copy = wave;
            copy.cohort_size = static_cast<uint16_t>(std::min<uint32_t>(excess_count, 65535u));
            staged_waves.push_back(copy);
        }
    }

    void Clear() noexcept {
        staged_waves.clear();
        staged_overflow_migrants = 0;
    }
};

[[nodiscard]] constexpr bool IsMigrantAvatarVoxelMaterial(uint32_t mat_id) noexcept {
    return mat_id == MAT_MIGRANT_R6 || mat_id == MAT_RESIDENT_R3 || mat_id == MAT_STEWARD_R1;
}

[[nodiscard]] constexpr uint8_t GetMigrantVoxelMaterialForRing(uint8_t trust_ring_tier) noexcept {
    if (trust_ring_tier <= 1u) {
        return MAT_STEWARD_R1;
    }
    if (trust_ring_tier <= 3u) {
        return MAT_RESIDENT_R3;
    }
    return MAT_MIGRANT_R6;
}

[[nodiscard]] inline uint32_t CountActiveMigrantNpcs(flecs::world& ecs) {
    ecs.component<LegacyMigrantComponent>();
    uint32_t count = 0;
    ecs.each([&](const LegacyMigrantComponent&) {
        count++;
    });
    return count;
}

template <typename PsychComponentT = oasis::psychology::PsychologyComponent>
[[nodiscard]] inline AssimilationChecklist EvaluateMigrantChecklist(
    const oasis::fabrication::SomaticStateComponent& som,
    const PsychComponentT& psy,
    const LegacyMigrantComponent& mig,
    float ambient_or_room_temp_c
) noexcept {
    (void)psy;
    AssimilationChecklist cl{};
    cl.enclosed_shelter_ok = (ambient_or_room_temp_c > 18.0f);
    cl.hydration_ok = (som.hydration > 0.50f);
    cl.calories_ok = (som.caloric_reserve > 1500.0);
    cl.potw_sweat_equity_mj = mig.potw_sweat_equity_mj;
    cl.sheltered_days = mig.sheltered_days_consecutive;
    cl.current_ring = mig.trust_ring_tier;
    return cl;
}

inline uint32_t SpawnMigrationWaveInOasis(
    flecs::world& ecs,
    const oasis::uhai::MigrationWaveArrivalEvent& wave,
    MigrationStagingQueue& staging,
    oasis::time::TimeWarpState& warp
) {
    // 1. Immediately engage Story 10.1 Emergency Warp Brake (downshifts 10x/100x -> 1x)
    oasis::time::EvaluateEmergencyWarpBrake(warp, 2000.0, 0.8f, 0.3f, /*migration_wave_arriving=*/true);

    ecs.component<LegacyMigrantComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<oasis::psychology::PsychologyComponent>();
    ecs.component<oasis::fabrication::SpatialPositionComponent>();
    ecs.component<oasis::boundary::SocialMutualAidComponent>();
    ecs.component<oasis::boundary::InfrastructureAutarkyComponent>();

    // 2. Enforce MAX_PARCEL_NPCS = 16 hard population cap
    const uint32_t current_count = CountActiveMigrantNpcs(ecs);
    const uint32_t available_slots = (current_count < MAX_PARCEL_NPCS) ? (MAX_PARCEL_NPCS - current_count) : 0u;
    const uint32_t requested = static_cast<uint32_t>(wave.cohort_size);
    const uint32_t to_spawn = std::min<uint32_t>(requested, available_slots);
    const uint32_t excess = requested - to_spawn;

    if (excess > 0u) {
        staging.StageOverflow(wave, excess);
    }

    for (uint32_t i = 0; i < to_spawn; ++i) {
        const uint32_t idx = current_count + i;
        const int vx = static_cast<int>(4u + (idx % 2u));
        const int vy = 1;
        const int vz = static_cast<int>(2u + idx * 4u);

        LegacyMigrantComponent mig{};
        mig.npc_index = idx;
        mig.wave_id = wave.wave_id;
        mig.did_hash = (static_cast<uint64_t>(wave.deterministic_seed) << 32) ^
                       static_cast<uint64_t>(idx + 1u) ^ 0x9E3779B97F4A7C15ULL;
        mig.trust_ring_tier = 6u; // Always Ring R6 (Unverified Migrant) on arrival
        mig.mindset_flags = SCARCITY_ANXIETY;
        mig.sheltered_days_consecutive = 0u;
        mig.days_in_r3_consecutive = 0u;
        mig.potw_sweat_equity_mj = 0.0f;
        mig.last_stamped_voxel = glm::ivec3(vx, vy, vz);
        mig.patrol_target = glm::vec3(static_cast<float>(vx), 1.0f, (idx % 2u == 0u) ? 22.0f : -10.0f);

        oasis::fabrication::SomaticStateComponent som{};
        som.caloric_reserve = std::clamp(2200.0 - static_cast<double>(wave.caloric_deficit_kcal), 900.0, 2200.0);
        som.hydration = 0.65f;
        som.focus = 0.70f;
        som.stamina = 0.80f;
        som.ambient_temp_c = 20.0f;

        oasis::psychology::PsychologyComponent psy{};
        psy.stress = std::clamp(wave.initial_stress_mean, 0.45f, 0.85f);
        psy.focus = 0.70f;
        psy.mood = 0.35f;
        if (psy.stress >= 0.75f) {
            mig.mindset_flags |= (HOARDING_ACTIVE | CONFLICT_REGRESSION | RESERVE_POWER_DRAW);
        }

        mig.checklist = EvaluateMigrantChecklist(som, psy, mig, som.ambient_temp_c);

        oasis::fabrication::SpatialPositionComponent pos{};
        pos.pos = glm::vec3(static_cast<float>(vx), static_cast<float>(vy), static_cast<float>(vz));
        pos.move_speed = 2.2f;

        if (oasis::graphics::GetDynamicVoxel(vx, vy, vz) == 0u ||
            IsMigrantAvatarVoxelMaterial(oasis::graphics::GetDynamicVoxel(vx, vy, vz))) {
            oasis::graphics::SetDynamicVoxel(vx, vy, vz, MAT_MIGRANT_R6);
        }

        ecs.entity()
            .set<LegacyMigrantComponent>(mig)
            .set<oasis::fabrication::SomaticStateComponent>(som)
            .set<oasis::psychology::PsychologyComponent>(psy)
            .set<oasis::fabrication::SpatialPositionComponent>(pos);
    }

    // 3. Sync SettlementBoundary's SocialMutualAidComponent count (clamped to 16)
    auto boundary = ecs.entity("SettlementBoundary");
    if (!boundary.is_alive()) {
        boundary = ecs.entity("SettlementBoundary")
            .set<oasis::boundary::SocialMutualAidComponent>({})
            .set<oasis::boundary::InfrastructureAutarkyComponent>({});
    } else if (!boundary.has<oasis::boundary::SocialMutualAidComponent>()) {
        boundary.set<oasis::boundary::SocialMutualAidComponent>({});
    }

    auto& social = boundary.get_mut<oasis::boundary::SocialMutualAidComponent>();
    const size_t new_total = std::min<size_t>(MAX_PARCEL_NPCS, static_cast<size_t>(current_count + to_spawn));
    for (size_t b_idx = current_count; b_idx < new_total; ++b_idx) {
        social.bonds[b_idx].trust = 15.0; // Initial R6 probationary trust
        social.bonds[b_idx].hours_aid = 1.0;
    }
    social.count = new_total;

    return to_spawn;
}

inline void StepLegacyFrictionSystem(flecs::world& ecs, float sim_hours_dt) {
    if (sim_hours_dt < 0.0f) {
        return;
    }

    oasis::chill::GenesisCampStockComponent* camp_stock = nullptr;
    auto camp = ecs.entity("GenesisCamp");
    if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
        camp_stock = &camp.get_mut<oasis::chill::GenesisCampStockComponent>();
    }

    oasis::boundary::InfrastructureAutarkyComponent* boundary_infra = nullptr;
    oasis::boundary::SocialMutualAidComponent* boundary_social = nullptr;
    auto boundary = ecs.entity("SettlementBoundary");
    if (boundary.is_alive()) {
        if (boundary.has<oasis::boundary::InfrastructureAutarkyComponent>()) {
            boundary_infra = &boundary.get_mut<oasis::boundary::InfrastructureAutarkyComponent>();
        }
        if (boundary.has<oasis::boundary::SocialMutualAidComponent>()) {
            boundary_social = &boundary.get_mut<oasis::boundary::SocialMutualAidComponent>();
        }
    }

    auto founder = ecs.entity("Founder");

    ecs.each([&](flecs::entity npc_ent,
                 LegacyMigrantComponent& mig,
                 oasis::fabrication::SomaticStateComponent& som,
                 oasis::psychology::PsychologyComponent& psy,
                 oasis::fabrication::SpatialPositionComponent& pos) {
        mig.checklist = EvaluateMigrantChecklist(som, psy, mig, som.ambient_temp_c);

        if (mig.trust_ring_tier == 6u) {
            const bool acute_unmet_needs =
                (!mig.checklist.enclosed_shelter_ok || som.hydration < 0.30f || som.caloric_reserve < 1000.0);
            const bool all_needs_met =
                (mig.checklist.enclosed_shelter_ok && mig.checklist.hydration_ok && mig.checklist.calories_ok);

            if (acute_unmet_needs || !all_needs_met) {
                mig.mindset_flags |= SCARCITY_ANXIETY;
                // Stress accumulates at 2.0x nominal rate (+0.12f * sim_hours_dt)
                psy.stress = std::clamp(psy.stress + 0.12f * sim_hours_dt, 0.0f, 1.0f);

                // Radiate scarcity anxiety (+0.05f * sim_hours_dt) to Founder / other agents within 4.0m
                ecs.each([&](flecs::entity other_ent,
                             oasis::psychology::PsychologyComponent& other_psy,
                             const oasis::fabrication::SpatialPositionComponent& other_pos) {
                    if (other_ent == npc_ent) return;
                    if (glm::length(other_pos.pos - pos.pos) <= 4.0f) {
                        other_psy.stress = std::clamp(other_psy.stress + 0.05f * sim_hours_dt, 0.0f, 1.0f);
                    }
                });
                if (founder.is_alive() && founder != npc_ent &&
                    founder.has<oasis::psychology::PsychologyComponent>() &&
                    !founder.has<oasis::fabrication::SpatialPositionComponent>()) {
                    auto& f_psy = founder.get_mut<oasis::psychology::PsychologyComponent>();
                    f_psy.stress = std::clamp(f_psy.stress + 0.05f * sim_hours_dt, 0.0f, 1.0f);
                }
            } else {
                // Survival needs ARE met: stress decays (-0.15f * sim_hours_dt), clears regression flags,
                // and the migrant performs supervised cooperative work (+1.5f MJ/hr PoTW sweat-equity)
                psy.stress = std::clamp(psy.stress - 0.15f * sim_hours_dt, 0.0f, 1.0f);
                mig.mindset_flags &= static_cast<uint8_t>(~(HOARDING_ACTIVE | CONFLICT_REGRESSION | RESERVE_POWER_DRAW));
                if (psy.stress < 0.35f) {
                    mig.mindset_flags &= static_cast<uint8_t>(~SCARCITY_ANXIETY);
                }
                mig.potw_sweat_equity_mj += 1.5f * sim_hours_dt;
            }

            // Evaluate 75% Stress Legacy Regression threshold
            if (psy.stress >= 0.75f) {
                mig.mindset_flags |= (SCARCITY_ANXIETY | HOARDING_ACTIVE | CONFLICT_REGRESSION | RESERVE_POWER_DRAW);

                // 1. Ration Hoarding: withdraws 3x rations (3 meals instead of 1) when hungry (caloric_reserve < 1400.0)
                if (som.caloric_reserve < 1400.0 && camp_stock != nullptr && camp_stock->ration_meals_remaining > 0u) {
                    const uint32_t hoarded_meals = std::min<uint32_t>(3u, camp_stock->ration_meals_remaining);
                    camp_stock->ration_meals_remaining -= hoarded_meals;
                    som.caloric_reserve = std::min(2500.0, som.caloric_reserve + 800.0 * static_cast<double>(hoarded_meals));
                }

                // 2. Tool Conflict: injects +0.15f * sim_hours_dt stress to nearby workers
                bool injected_nearby = false;
                ecs.each([&](flecs::entity other_ent,
                             oasis::psychology::PsychologyComponent& other_psy,
                             const oasis::fabrication::SpatialPositionComponent& other_pos) {
                    if (other_ent == npc_ent) return;
                    if (glm::length(other_pos.pos - pos.pos) <= 4.0f) {
                        other_psy.stress = std::clamp(other_psy.stress + 0.15f * sim_hours_dt, 0.0f, 1.0f);
                        injected_nearby = true;
                    }
                });
                if (!injected_nearby && founder.is_alive() && founder != npc_ent &&
                    founder.has<oasis::psychology::PsychologyComponent>()) {
                    auto& f_psy = founder.get_mut<oasis::psychology::PsychologyComponent>();
                    f_psy.stress = std::clamp(f_psy.stress + 0.15f * sim_hours_dt, 0.0f, 1.0f);
                }

                // 3. Emergency Microgrid Reserve Draw: drains 2.5 * sim_hours_dt kWh from solar_stored_kwh (clamped >= 0.0)
                if (boundary_infra != nullptr) {
                    const double drain_kwh = static_cast<double>(2.5f * sim_hours_dt);
                    boundary_infra->solar_stored_kwh = std::max(0.0, boundary_infra->solar_stored_kwh - drain_kwh);
                    boundary_infra->solar_consumed_kwh += drain_kwh;
                }
            }
        } else {
            // R3 Probationary Resident or R1 Vouched Steward: zero legacy regressions & cooperative mutual aid
            mig.mindset_flags = MINDSET_NONE;
            psy.stress = std::clamp(psy.stress - 0.10f * sim_hours_dt, 0.0f, 1.0f);
            mig.potw_sweat_equity_mj += 2.0f * sim_hours_dt;

            if (boundary_social != nullptr && mig.npc_index < oasis::boundary::SocialMutualAidComponent::MAX_NPCS) {
                boundary_social->bonds[mig.npc_index].trust = (mig.trust_ring_tier == 1u) ? 95.0 : 65.0;
                boundary_social->bonds[mig.npc_index].hours_aid += static_cast<double>(sim_hours_dt);
            }
        }

        mig.checklist = EvaluateMigrantChecklist(som, psy, mig, som.ambient_temp_c);
    });
}

template <typename PsychComponentT = oasis::psychology::PsychologyComponent>
inline bool TryPromoteMigrantR6ToR3(
    LegacyMigrantComponent& mig,
    PsychComponentT& psy
) noexcept {
    if (mig.trust_ring_tier != 6u) {
        return false;
    }
    if (mig.sheltered_days_consecutive < REQUIRED_SHELTER_DAYS_FOR_R3) {
        return false;
    }
    if (mig.potw_sweat_equity_mj < REQUIRED_POTW_MJ_FOR_R3) {
        return false;
    }
    if ((mig.mindset_flags & HOARDING_ACTIVE) != 0u) {
        return false;
    }

    mig.trust_ring_tier = 3u;
    mig.mindset_flags = MINDSET_NONE;
    mig.checklist.current_ring = 3u;
    mig.checklist.sheltered_days = mig.sheltered_days_consecutive;
    mig.checklist.potw_sweat_equity_mj = mig.potw_sweat_equity_mj;

    psy.stress = std::max(0.0f, psy.stress - 0.20f);
    psy.mood = std::min(1.0f, psy.mood + 0.20f);
    return true;
}

template <typename PsychComponentT = oasis::psychology::PsychologyComponent>
inline bool TryPromoteMigrantR3ToR1(
    LegacyMigrantComponent& mig,
    PsychComponentT& psy,
    uint8_t frost_signers
) noexcept {
    if (mig.trust_ring_tier != 3u) {
        return false;
    }
    if (mig.days_in_r3_consecutive < REQUIRED_R3_DAYS_FOR_R1) {
        return false;
    }
    if (frost_signers < 2u) {
        return false;
    }

    mig.trust_ring_tier = 1u;
    mig.mindset_flags = MINDSET_NONE;
    mig.checklist.current_ring = 1u;

    psy.stress = std::max(0.0f, psy.stress - 0.15f);
    psy.mood = 0.95f;
    return true;
}

// Synchronizes active NPC positions into DynamicVoxelStore (MAT_MIGRANT_R6 = 27u,
// MAT_RESIDENT_R3 = 28u, MAT_STEWARD_R1 = 29u) using a 2-pass clear-then-stamp
// so departing migrants never erase a co-located migrant's voxel.
inline void SyncMigrantAvatarVoxels(flecs::world& ecs) {
    // Pass 1: Clear previous stamped voxels for all migrants
    ecs.each([&](LegacyMigrantComponent& mig,
                 const oasis::fabrication::SpatialPositionComponent&) {
        if (mig.last_stamped_voxel.y >= 1) {
            const uint8_t prev_mat = oasis::graphics::GetDynamicVoxel(
                mig.last_stamped_voxel.x, mig.last_stamped_voxel.y, mig.last_stamped_voxel.z
            );
            if (IsMigrantAvatarVoxelMaterial(prev_mat)) {
                oasis::graphics::SetDynamicVoxel(
                    mig.last_stamped_voxel.x, mig.last_stamped_voxel.y, mig.last_stamped_voxel.z, 0u
                );
            }
        }
    });

    // Pass 2: Stamp each migrant's current voxel (or an adjacent unoccupied offset if two migrants share a cell)
    static constexpr int kOffsets[9][2] = {
        {0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
    };
    ecs.each([&](LegacyMigrantComponent& mig,
                 const oasis::fabrication::SpatialPositionComponent& pos) {
        const glm::ivec3 base_voxel(
            static_cast<int>(std::round(pos.pos.x)),
            std::max(1, static_cast<int>(std::round(pos.pos.y))),
            static_cast<int>(std::round(pos.pos.z))
        );
        const uint8_t target_mat = GetMigrantVoxelMaterialForRing(mig.trust_ring_tier);

        for (const auto& off : kOffsets) {
            const glm::ivec3 candidate(base_voxel.x + off[0], base_voxel.y, base_voxel.z + off[1]);
            const uint8_t existing_mat = oasis::graphics::GetDynamicVoxel(candidate.x, candidate.y, candidate.z);
            if (existing_mat == 0u) {
                oasis::graphics::SetDynamicVoxel(candidate.x, candidate.y, candidate.z, target_mat);
                mig.last_stamped_voxel = candidate;
                break;
            }
        }
    });
}

enum class Layer7CommentaryEvent : uint8_t {
    AMBIENT_WAVE = 0,
    ROOF_SOLAR_PANEL = 1,
    BACKYARD_SWALE = 2,
    COOKED_HOMESTEAD_MEAL = 3,
    SALVAGE_CRATE_HAUL = 4,
    RAINWATER_CISTERN = 5,
    COURIER_REQUISITION_DROP = 6
};

[[nodiscard]] inline const char* GetLayer7NeighborMilestoneRemark(Layer7CommentaryEvent ev) noexcept {
    switch (ev) {
        case Layer7CommentaryEvent::ROOF_SOLAR_PANEL:
            return "Neighbor (L7) glances up at your roof: 'Solar panels spinning already? Smart move with grid rates lately!'";
        case Layer7CommentaryEvent::BACKYARD_SWALE:
            return "Neighbor (L7) peers over the fence: 'Digging a rain swale back there? Smart—this cul-de-sac floods every autumn storm.'";
        case Layer7CommentaryEvent::COOKED_HOMESTEAD_MEAL:
            return "Neighbor (L7) pauses on the sidewalk: 'Something smells amazing from your kitchen! Nothing beats fresh backyard produce.'";
        case Layer7CommentaryEvent::SALVAGE_CRATE_HAUL:
            return "Neighbor (L7) waves from the sidewalk: 'Hauling those garage hardware crates? Respect the DIY homestead hustle.'";
        case Layer7CommentaryEvent::RAINWATER_CISTERN:
            return "Neighbor (L7) nods toward your eaves: 'Rain barrel hooked up! Free soft water for the garden all summer.'";
        case Layer7CommentaryEvent::COURIER_REQUISITION_DROP:
            return "Courier (L7) drops a crate at (8,1,3): 'Delivered +2 Salvaged Solar PV Kits & +2 Seed Packs for your homestead!'";
        case Layer7CommentaryEvent::AMBIENT_WAVE:
        default:
            return "Neighbor (L7) nods politely from the sidewalk: 'Morning! Beautiful day on the cul-de-sac.'";
    }
}

inline bool RequisitionSalvageKitsViaLayer7(oasis::chill::GenesisCampStockComponent& stock) noexcept {
    if (stock.ration_meals_remaining < 2u) {
        return false;
    }
    stock.ration_meals_remaining -= 2u;
    stock.solar_kits_remaining += 2u;
    stock.seed_packs_remaining += 2u;
    return true;
}

} // namespace oasis::migration

namespace oasis::graphics {
    using oasis::migration::MAT_MIGRANT_R6;
    using oasis::migration::MAT_RESIDENT_R3;
    using oasis::migration::MAT_STEWARD_R1;
} // namespace oasis::graphics

namespace oasis {
    using oasis::migration::MAX_PARCEL_NPCS;
    using oasis::migration::SOLO_GENESIS_GRACE_TICKS;
    using oasis::migration::PRE_WAVE_WARNING_LEAD_TICKS;
    using oasis::migration::REQUIRED_POTW_MJ_FOR_R3;
    using oasis::migration::REQUIRED_SHELTER_DAYS_FOR_R3;
    using oasis::migration::REQUIRED_R3_DAYS_FOR_R1;
    using oasis::migration::MAT_MIGRANT_R6;
    using oasis::migration::MAT_RESIDENT_R3;
    using oasis::migration::MAT_STEWARD_R1;
    using oasis::migration::LegacyMindsetFlags;
    using oasis::migration::AssimilationChecklist;
    using oasis::migration::LegacyMigrantComponent;
    using oasis::migration::MigrationStagingQueue;
    using oasis::migration::CountActiveMigrantNpcs;
    using oasis::migration::SpawnMigrationWaveInOasis;
    using oasis::migration::EvaluateMigrantChecklist;
    using oasis::migration::StepLegacyFrictionSystem;
    using oasis::migration::TryPromoteMigrantR6ToR3;
    using oasis::migration::TryPromoteMigrantR3ToR1;
    using oasis::migration::SyncMigrantAvatarVoxels;
    using oasis::migration::Layer7CommentaryEvent;
    using oasis::migration::GetLayer7NeighborMilestoneRemark;
    using oasis::migration::RequisitionSalvageKitsViaLayer7;
} // namespace oasis
