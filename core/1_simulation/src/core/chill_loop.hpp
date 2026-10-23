#pragma once

#include <glm/glm.hpp>
#include <flecs.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include "core/fabrication.hpp"
#include "core/homeostasis.hpp"
#include "core/raycaster.hpp"
#include "core/spatial_designator.hpp"
#include "core/time_warp_controller.hpp"

namespace oasis::chill {

// Canonical Pacing Constants (1,200 ticks = 1 simulated hour; 28,800 ticks = 1 simulated day)
constexpr uint64_t TICKS_PER_SIM_HOUR = 1200ULL;
constexpr uint64_t TICKS_PER_SIM_DAY  = 28800ULL;
constexpr size_t   MAX_BFS_VISITED    = 512;

// Material IDs for Chill Loop & Survival Camp
constexpr uint32_t MAT_TILLED_BED      = 19u;
constexpr uint32_t MAT_DOOR_TIMBER     = 15u;
constexpr uint32_t MAT_HAUL_ZONE       = 20u;
constexpr uint32_t MAT_SLEEP_ZONE      = 21u;
constexpr uint32_t MAT_RECREATION_ZONE = 22u;
constexpr uint32_t MAT_BELL_TENT       = 23u;
constexpr uint32_t MAT_RATION_CRATE    = 24u;
constexpr uint32_t MAT_ROOF_LADDER     = 32u;
constexpr uint32_t MAT_STAGE_FRAME     = 33u;
constexpr uint32_t MAT_MATURE_GARDEN   = 34u;

// +45 Focus/hr on the 0..255 scale (~ +0.17647f / hr on [0.0..1.0])
constexpr float FOCUS_RECOVERY_PER_SIM_HOUR = 45.0f / 255.0f;

// ============================================================================
// STORY-9.1: Dirty-Flag Cached ThermalEnclosureSolver, Passable Doors & Sleep
// ============================================================================

enum class EnclosureStatus : uint8_t {
    UNENCLOSED           = 0,
    ENCLOSED_UNINSULATED = 1,
    ENCLOSED_INSULATED   = 2
};

struct EnclosureCacheComponent {
    bool room_enclosure_dirty{true};
    EnclosureStatus status{EnclosureStatus::UNENCLOSED};
    glm::ivec3 leak_voxel{0};
    bool has_leak_voxel{false};
    uint32_t enclosed_voxel_count{0};
    float interior_temp_c{22.0f};
    bool has_heat_source{false};
};

// Returns true for structural walls, roofs, Bell Tent, and Insulated Timber Doors (MAT_DOOR_TIMBER = 15)
[[nodiscard]] constexpr bool IsAirtightBarrierMaterial(uint32_t mat_id) noexcept {
    switch (mat_id) {
        case 5u:  // Weathered wood siding
        case 6u:  // Dark shingle roof
        case 7u:  // Window glass
        case 13u: // Cob wall
        case 14u: // Urbanite wall
        case 15u: // MAT_DOOR_TIMBER (Insulated Timber Door)
        case 16u: // Rocket Mass Heater / Hearth
        case 17u: // Insulated Timber Roof
        case 23u: // MAT_BELL_TENT
            return true;
        default:
            return false;
    }
}

// Returns true for air, zones, Bell Tent, AND MAT_DOOR_TIMBER (15u) so agents pathfind through doors
// while the room remains thermally sealed.
[[nodiscard]] constexpr bool IsPassableForPathfinding(uint32_t mat_id) noexcept {
    switch (mat_id) {
        case 0u:  // Air
        case 15u: // MAT_DOOR_TIMBER (passable door!)
        case 18u: // Swale
        case 19u: // Cover Crop
        case 20u: // Haul / Stockpile Zone
        case 21u: // Sleep Zone
        case 22u: // Recreation Zone
        case 23u: // Bell Tent
        case 25u: // Leak Marker
        case 26u: // Blueprint Ghost
        case 32u: // Exterior Roof Access Ladder
        case 33u: // Construction Stage Frame
        case 34u: // Mature Crop
        case 120u: // Studio Workstation Pod (Scenario Zero Homelab)
            return true;
        default:
            return false;
    }
}

inline void MarkEnclosureDirtyOnVoxelChange(EnclosureCacheComponent& cache) noexcept {
    cache.room_enclosure_dirty = true;
}

[[nodiscard]] inline bool HasOverheadRoofBarrier(int x, int y, int z) noexcept {
    for (int check_y = y + 1; check_y <= 16; ++check_y) {
        const uint32_t mat = oasis::graphics::GetVoxelMaterialAt(glm::ivec3(x, check_y, z));
        if (IsAirtightBarrierMaterial(mat)) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] inline bool IsFlankedByWalls(const glm::ivec3& pos) noexcept {
    const bool x_walls =
        IsAirtightBarrierMaterial(oasis::graphics::GetVoxelMaterialAt(pos + glm::ivec3(1, 0, 0))) &&
        IsAirtightBarrierMaterial(oasis::graphics::GetVoxelMaterialAt(pos + glm::ivec3(-1, 0, 0)));
    const bool z_walls =
        IsAirtightBarrierMaterial(oasis::graphics::GetVoxelMaterialAt(pos + glm::ivec3(0, 0, 1))) &&
        IsAirtightBarrierMaterial(oasis::graphics::GetVoxelMaterialAt(pos + glm::ivec3(0, 0, -1)));
    return x_walls || z_walls;
}

inline EnclosureStatus EvaluateThermalEnclosure(
    EnclosureCacheComponent& cache,
    glm::ivec3 seed_pos,
    float outdoor_ambient_c = 10.0f
) noexcept {
    auto update_cached_temperature = [&]() noexcept {
        if (cache.status == EnclosureStatus::ENCLOSED_INSULATED) {
            if (cache.has_heat_source) {
                cache.interior_temp_c = std::max(21.5f, 22.0f + 0.15f * (outdoor_ambient_c - 22.0f));
            } else {
                cache.interior_temp_c = std::max(19.5f, 21.0f + 0.20f * (outdoor_ambient_c - 21.0f));
            }
        } else if (cache.status == EnclosureStatus::ENCLOSED_UNINSULATED) {
            cache.interior_temp_c = 18.0f + 0.40f * (outdoor_ambient_c - 18.0f);
        } else {
            cache.interior_temp_c = outdoor_ambient_c;
        }
    };

    // Dirty-Flag Short-Circuit: skip BFS when room topology has not changed
    if (!cache.room_enclosure_dirty) {
        update_cached_temperature();
        return cache.status;
    }

    // Special case: seed_pos is directly inside a Bell Tent voxel
    const uint32_t seed_mat = oasis::graphics::GetVoxelMaterialAt(seed_pos);
    if (seed_mat == MAT_BELL_TENT) {
        cache.status = EnclosureStatus::ENCLOSED_INSULATED;
        cache.has_leak_voxel = false;
        cache.leak_voxel = glm::ivec3(0);
        cache.enclosed_voxel_count = 1;
        cache.has_heat_source = true;
        cache.room_enclosure_dirty = false;
        update_cached_temperature();
        return cache.status;
    }

    // 1. O(H) vertical +Y sky-column ray check from seed_pos
    if (!HasOverheadRoofBarrier(seed_pos.x, seed_pos.y, seed_pos.z)) {
        cache.status = EnclosureStatus::UNENCLOSED;
        cache.has_leak_voxel = true;
        cache.leak_voxel = glm::ivec3(seed_pos.x, seed_pos.y + 3, seed_pos.z);
        cache.enclosed_voxel_count = 0;
        cache.has_heat_source = false;
        cache.room_enclosure_dirty = false;
        update_cached_temperature();
        return cache.status;
    }

    // 2. Bounded 3D BFS using fixed-size stack arrays (MAX_BFS_VISITED = 512, zero heap allocations)
    glm::ivec3 queue[MAX_BFS_VISITED]{};
    uint64_t visited[MAX_BFS_VISITED]{};
    size_t head = 0;
    size_t tail = 0;
    size_t visited_count = 0;
    bool found_heat_source = false;

    auto is_visited = [&](uint64_t packed) noexcept -> bool {
        for (size_t i = 0; i < visited_count; ++i) {
            if (visited[i] == packed) return true;
        }
        return false;
    };

    const uint64_t seed_packed = oasis::graphics::PackVoxelCoord(seed_pos.x, seed_pos.y, seed_pos.z);
    queue[tail++] = seed_pos;
    visited[visited_count++] = seed_packed;

    constexpr glm::ivec3 kDirs[6] = {
        { 1,  0,  0},
        {-1,  0,  0},
        { 0,  0,  1},
        { 0,  0, -1},
        { 0,  1,  0},
        { 0, -1,  0}
    };

    while (head < tail) {
        const glm::ivec3 cur = queue[head++];

        for (const auto& d : kDirs) {
            const glm::ivec3 nb = cur + d;
            if (nb.y <= 0) {
                continue; // Ground plane (y <= 0) acts as solid floor barrier
            }

            const uint32_t nb_mat = oasis::graphics::GetVoxelMaterialAt(nb);
            if (nb_mat == 16u || nb_mat == MAT_BELL_TENT) {
                found_heat_source = true;
            }

            if (IsAirtightBarrierMaterial(nb_mat)) {
                continue; // Sealed wall, closed door (15u), or roof stops expansion
            }

            // Check if the open neighbor column is covered by an overhead roof
            if (!HasOverheadRoofBarrier(nb.x, nb.y, nb.z)) {
                cache.status = EnclosureStatus::UNENCLOSED;
                cache.has_leak_voxel = true;
                cache.leak_voxel = (d.y == 0 && IsFlankedByWalls(cur)) ? cur : nb;
                cache.enclosed_voxel_count = static_cast<uint32_t>(visited_count);
                cache.has_heat_source = found_heat_source;
                cache.room_enclosure_dirty = false;
                update_cached_temperature();
                return cache.status;
            }

            const uint64_t nb_packed = oasis::graphics::PackVoxelCoord(nb.x, nb.y, nb.z);
            if (is_visited(nb_packed)) {
                continue;
            }

            if (visited_count >= MAX_BFS_VISITED) {
                cache.status = EnclosureStatus::UNENCLOSED;
                cache.has_leak_voxel = true;
                cache.leak_voxel = nb;
                cache.enclosed_voxel_count = static_cast<uint32_t>(visited_count);
                cache.has_heat_source = found_heat_source;
                cache.room_enclosure_dirty = false;
                update_cached_temperature();
                return cache.status;
            }

            visited[visited_count++] = nb_packed;
            queue[tail++] = nb;
        }
    }

    // 3. Frontier terminated cleanly within airtight walls, passable doors, and roof
    cache.status = EnclosureStatus::ENCLOSED_INSULATED;
    cache.has_leak_voxel = false;
    cache.leak_voxel = glm::ivec3(0);
    cache.enclosed_voxel_count = static_cast<uint32_t>(visited_count);
    cache.has_heat_source = found_heat_source;
    cache.room_enclosure_dirty = false;
    update_cached_temperature();
    return cache.status;
}

template <typename PsychComponentT>
inline void StepInsulatedSleepRecovery(
    oasis::fabrication::SomaticStateComponent& somatic,
    PsychComponentT& psych,
    const EnclosureCacheComponent& enclosure,
    float sim_hours_elapsed
) noexcept {
    if (sim_hours_elapsed <= 0.0f) {
        return;
    }

    constexpr double kBasalKcalPerSimHour = 2000.0 / 24.0; // ~83.33 kcal/hr

    if (enclosure.status == EnclosureStatus::ENCLOSED_INSULATED && enclosure.interior_temp_c > 18.0f) {
        somatic.ambient_temp_c = enclosure.interior_temp_c;
        // Zero cold excess caloric burn inside insulated shelter
        somatic.caloric_reserve = std::max(0.0, somatic.caloric_reserve - kBasalKcalPerSimHour * static_cast<double>(sim_hours_elapsed));

        // +45 Focus/hr on 0..255 scale (+45/255 per hour on [0.0..1.0])
        const float focus_gain = FOCUS_RECOVERY_PER_SIM_HOUR * sim_hours_elapsed;
        psych.focus = std::clamp(psych.focus + focus_gain, 0.0f, 1.0f);
        somatic.focus = psych.focus;
        somatic.stamina = std::clamp(somatic.stamina + 0.25f * sim_hours_elapsed, 0.0f, 1.0f);

        if (somatic.caloric_reserve > 500.0 && psych.focus > 0.05f) {
            somatic.is_exhausted = false;
        }
    } else {
        somatic.ambient_temp_c = enclosure.interior_temp_c;
        const auto eval = oasis::homeostasis::EvaluateHomeostasis(enclosure.interior_temp_c);
        const double burn = kBasalKcalPerSimHour * eval.burn_multiplier * static_cast<double>(sim_hours_elapsed);
        somatic.caloric_reserve = std::max(0.0, somatic.caloric_reserve - burn);
    }
}

template <typename PsychComponentT>
inline void StepInsulatedSleepRecovery(
    oasis::fabrication::SomaticStateComponent& somatic,
    PsychComponentT& psych,
    EnclosureStatus status,
    uint64_t recovery_ticks_scale
) noexcept {
    if (status == EnclosureStatus::ENCLOSED_INSULATED) {
        somatic.ambient_temp_c = 22.0f;
        const float sim_hours_step =
            (static_cast<float>(recovery_ticks_scale) / static_cast<float>(TICKS_PER_SIM_HOUR)) * (1.0f / 60.0f);
        const float focus_gain = FOCUS_RECOVERY_PER_SIM_HOUR * sim_hours_step;
        psych.focus = std::clamp(psych.focus + focus_gain, 0.0f, 1.0f);
        somatic.focus = psych.focus;
        somatic.stamina = std::clamp(somatic.stamina + 0.25f * sim_hours_step, 0.0f, 1.0f);
        if (somatic.caloric_reserve > 500.0 && psych.focus > 0.05f) {
            somatic.is_exhausted = false;
        }
    }
}

// ============================================================================
// STORY-9.2: Day 1 Genesis Survival Camp Bootstrap & Autonomous Intake
// ============================================================================

struct GenesisCampStockComponent {
    uint32_t ration_meals_remaining{12}; // 12 meals * 800 kcal = 9,600 kcal (4 days)
    double potable_water_liters{60.0};   // 60 L IBC tote (15 days)
    uint32_t cordwood_remaining{12};     // 12 bundles
    bool bell_tent_active{true};
    bool campfire_active{true};
    bool tool_locker_present{true};
    glm::ivec3 tent_pos{11, 1, 8};          // Master Bed inside Lot 402 House bedroom
    glm::ivec3 water_tote_pos{14, 1, 4};    // Kitchen Filtered Water Tap / Cistern inside Lot 402 House
    glm::ivec3 ration_crate_pos{11, 1, 4};  // Kitchen Pantry & Counter inside Lot 402 House
    glm::ivec3 campfire_pos{19, 1, 6};      // Backyard Patio Hearth / Firepit outside back door
    glm::ivec3 salvage_crate_pos{8, 1, 3};  // Driveway Salvage & Hardware Crate
    uint32_t solar_kits_remaining{2};       // Finite starter PV Kits (enough for 2 Solar Panels!)
    uint32_t timber_planks_remaining{6};    // Timber for Cisterns/Doors/Roof
    uint32_t cob_earth_lumps{4};            // Clay/Cob lumps (Swales yield +2!)
    uint32_t seed_packs_remaining{6};       // Seeds for backyard cover/food crops
    uint32_t raw_produce_items{2};          // Harvested raw backyard produce for Scenario Gamma Kitchen

    [[nodiscard]] double total_ration_kcal() const noexcept {
        return static_cast<double>(ration_meals_remaining) * 800.0;
    }
};

[[nodiscard]] inline bool HasRequiredMaterialInStock(
    const GenesisCampStockComponent& stock,
    uint8_t kind
) noexcept {
    switch (kind) {
        case 0u: return true;
        case 1u: return stock.solar_kits_remaining >= 1u;
        case 2u: return stock.timber_planks_remaining >= 1u;
        case 3u: return stock.cob_earth_lumps >= 1u;
        case 4u: return stock.seed_packs_remaining >= 1u;
        default: return true;
    }
}

inline bool ConsumeRequiredMaterialFromStock(
    GenesisCampStockComponent& stock,
    uint8_t kind
) noexcept {
    switch (kind) {
        case 0u:
            return true;
        case 1u:
            if (stock.solar_kits_remaining == 0u) return false;
            stock.solar_kits_remaining -= 1u;
            return true;
        case 2u:
            if (stock.timber_planks_remaining == 0u) return false;
            stock.timber_planks_remaining -= 1u;
            return true;
        case 3u:
            if (stock.cob_earth_lumps == 0u) return false;
            stock.cob_earth_lumps -= 1u;
            return true;
        case 4u:
            if (stock.seed_packs_remaining == 0u) return false;
            stock.seed_packs_remaining -= 1u;
            return true;
        default:
            return true;
    }
}

inline void RefundRequiredMaterialToStock(
    GenesisCampStockComponent& stock,
    uint8_t kind
) noexcept {
    switch (kind) {
        case 1u: stock.solar_kits_remaining += 1u; break;
        case 2u: stock.timber_planks_remaining += 1u; break;
        case 3u: stock.cob_earth_lumps += 1u; break;
        case 4u: stock.seed_packs_remaining += 1u; break;
        default: break;
    }
}

[[nodiscard]] inline const char* FormatMaterialKindName(uint8_t kind) noexcept {
    switch (kind) {
        case 1u: return "Solar PV Kit";
        case 2u: return "Timber Plank";
        case 3u: return "Cob/Earth Lump";
        case 4u: return "Seed Pack";
        default: return "Material";
    }
}

[[nodiscard]] inline float EvaluateMetabolicSpeedMultiplier(
    const oasis::fabrication::SomaticStateComponent& somatic
) noexcept {
    if (somatic.caloric_reserve <= 500.0 || somatic.hydration < 0.15f) {
        return 0.40f; // STARVING / DEHYDRATED (heavy construction halted)
    }
    if (somatic.caloric_reserve < 1100.0 || somatic.hydration < 0.35f) {
        return 0.70f; // HUNGRY / THIRSTY
    }
    return 1.0f;      // WELL_FED
}

[[nodiscard]] inline const char* FormatMetabolicStateLabel(
    const oasis::fabrication::SomaticStateComponent& somatic
) noexcept {
    const float mult = EvaluateMetabolicSpeedMultiplier(somatic);
    if (mult <= 0.45f) {
        return "STARVING/DEHYDRATED (0.4x)";
    }
    if (mult <= 0.75f) {
        return "HUNGRY/THIRSTY (0.7x)";
    }
    return "WELL-FED (1.0x)";
}

inline bool CookHomesteadMealAtKitchen(
    GenesisCampStockComponent& camp,
    double& io_solar_kwh
) noexcept {
    if (camp.raw_produce_items < 1u || camp.potable_water_liters < 0.5) {
        return false;
    }
    camp.raw_produce_items -= 1u;
    camp.potable_water_liters = std::max(0.0, camp.potable_water_liters - 0.5);
    if (io_solar_kwh >= 0.25) {
        io_solar_kwh -= 0.25;
    } else {
        io_solar_kwh = 0.0;
    }
    camp.ration_meals_remaining += 2u; // +1,600 kcal of prepared homestead meals
    return true;
}

inline bool CookHomesteadMealAtKitchen(GenesisCampStockComponent& camp) noexcept {
    double dummy_solar_kwh = 1.0;
    return CookHomesteadMealAtKitchen(camp, dummy_solar_kwh);
}

template <typename PsychComponentT>
inline bool CookHomesteadMealAtKitchen(
    oasis::fabrication::SomaticStateComponent& somatic,
    PsychComponentT& psych,
    GenesisCampStockComponent& camp
) noexcept {
    if (camp.raw_produce_items < 1u || camp.potable_water_liters < 0.5) {
        return false;
    }
    camp.raw_produce_items -= 1u;
    camp.potable_water_liters = std::max(0.0, camp.potable_water_liters - 0.5);
    camp.ration_meals_remaining += 2u;
    somatic.caloric_reserve = std::min(2500.0, somatic.caloric_reserve + 800.0);
    somatic.hydration = std::min(1.0f, somatic.hydration + 0.20f);
    psych.mood = std::min(1.0f, psych.mood + 0.15f);
    psych.stress = std::max(0.0f, psych.stress - 0.08f);
    return true;
}

inline flecs::entity BootstrapGenesisSurvivalCamp(flecs::world& ecs) {
    ecs.component<GenesisCampStockComponent>();
    ecs.component<EnclosureCacheComponent>();
    ecs.component<oasis::fabrication::GardenCropComponent>();

    GenesisCampStockComponent camp{};
    oasis::graphics::SetDynamicVoxel(camp.tent_pos, static_cast<uint8_t>(MAT_BELL_TENT));
    oasis::graphics::SetDynamicVoxel(camp.ration_crate_pos, static_cast<uint8_t>(MAT_RATION_CRATE));
    oasis::graphics::SetDynamicVoxel(camp.water_tote_pos, 12u); // Cistern / Water Tote
    oasis::graphics::SetDynamicVoxel(camp.campfire_pos, static_cast<uint8_t>(MAT_RECREATION_ZONE));
    oasis::graphics::SetDynamicVoxel(camp.tent_pos + glm::ivec3(0, 0, 1), static_cast<uint8_t>(MAT_SLEEP_ZONE));

    // Stamp Driveway Salvage & Hardware Crate at (8, 1, 3) (24u)
    oasis::graphics::SetDynamicVoxel(camp.salvage_crate_pos, static_cast<uint8_t>(MAT_RATION_CRATE));

    // Stamp 2 starter Mature Backyard Garden Beds (34u MAT_MATURE_CROP) at (20, 1, 5) and (20, 1, 6)
    oasis::graphics::SetDynamicVoxel(glm::ivec3(20, 1, 5), 34u);
    oasis::graphics::SetDynamicVoxel(glm::ivec3(20, 1, 6), 34u);
    ecs.entity().set<oasis::fabrication::GardenCropComponent>({20, 1, 5, 300.0f, 300.0f, true});
    ecs.entity().set<oasis::fabrication::GardenCropComponent>({20, 1, 6, 300.0f, 300.0f, true});

    return ecs.entity("GenesisCamp").set<GenesisCampStockComponent>(camp);
}

inline flecs::entity BootstrapCleanSuburbanGenesisLot(flecs::world& ecs) {
    ecs.component<GenesisCampStockComponent>();
    ecs.component<EnclosureCacheComponent>();
    ecs.component<oasis::fabrication::GardenCropComponent>();

    GenesisCampStockComponent camp{};
    camp.solar_kits_remaining = 0u;
    camp.raw_produce_items = 0u;
    camp.tent_pos = {15, 1, 8};          // Bedroom Bed + Mattress + Nightstand + Wardrobe
    camp.ration_crate_pos = {11, 1, 4};  // Kitchen Fridge / Oven+Stove / Dining Table area
    camp.water_tote_pos = {12, 1, 4};    // Kitchen Sink & Counter
    camp.salvage_crate_pos = {10, 1, 9}; // Garage Workbench & Storage Unit access inside house floor plan

    // Clean-slate 6-room furnished suburban house & backyard lawn:
    // Do NOT stamp driveway salvage crates, water totes, or ration crates into the dynamic voxel store!
    oasis::graphics::GetDynamicVoxelStore().clear();

    std::vector<flecs::entity> existing_crops;
    ecs.each([&](flecs::entity ce, oasis::fabrication::GardenCropComponent&) {
        existing_crops.push_back(ce);
    });
    for (auto& ce : existing_crops) {
        if (ce.is_alive()) {
            ce.destruct();
        }
    }

    return ecs.entity("GenesisCamp").set<GenesisCampStockComponent>(camp);
}

template <typename PsychComponentT>
inline bool StepSomaticIntake(
    oasis::fabrication::SomaticStateComponent& somatic,
    PsychComponentT& psych,
    GenesisCampStockComponent& camp,
    bool& out_telemetry_emitted
) noexcept {
    out_telemetry_emitted = false;

    // Autonomous Water Intake: hydration < 0.35 -> consume 1.0 L -> +0.40 hydration
    if (somatic.hydration < 0.35f && camp.potable_water_liters >= 1.0) {
        camp.potable_water_liters -= 1.0;
        somatic.hydration = std::min(1.0f, somatic.hydration + 0.40f);
        out_telemetry_emitted = true;
    }

    // Autonomous Ration Intake: caloric_reserve < 1200.0 -> consume 1 meal -> +800 kcal & -0.10 stress (THOUGHT_WARM_MEAL)
    if (somatic.caloric_reserve < 1200.0 && camp.ration_meals_remaining >= 1u) {
        camp.ration_meals_remaining -= 1u;
        somatic.caloric_reserve = std::min(2500.0, somatic.caloric_reserve + 800.0);
        psych.stress = std::max(0.0f, psych.stress - 0.10f);
        if (somatic.caloric_reserve > 500.0 && psych.focus > 0.05f) {
            somatic.is_exhausted = false;
        }
        out_telemetry_emitted = true;
    }

    return out_telemetry_emitted;
}

template <typename PsychComponentT>
inline bool StepSomaticIntake(
    oasis::fabrication::SomaticStateComponent& somatic,
    PsychComponentT& psych,
    GenesisCampStockComponent& camp
) noexcept {
    bool emitted = false;
    return StepSomaticIntake(somatic, psych, camp, emitted);
}

// ============================================================================
// STORY-9.3: Recreation Zones & STATE_BURNOUT with Life-Critical Preemption
// ============================================================================

struct BurnoutStateComponent {
    bool is_burned_out{false};
    bool life_critical_preemption_active{false};
    bool autonomous_self_sooth_active{false};
};

// Zero-labor zone designations are NEVER blocked by STATE_BURNOUT (Anti-Soft-Lock)
[[nodiscard]] inline bool CanExecuteDesignatorAction(
    const oasis::designator::DesignatorToolSpec& spec,
    const BurnoutStateComponent& burnout
) noexcept {
    if (spec.is_zero_labor_zone) {
        return true;
    }
    return !burnout.is_burned_out;
}

template <typename PsychComponentT>
inline void StepRecreationAndBurnout(
    oasis::fabrication::SomaticStateComponent& somatic,
    PsychComponentT& psych,
    BurnoutStateComponent& burnout,
    GenesisCampStockComponent& camp,
    bool in_recreation_zone,
    bool has_social_companion,
    bool has_reachable_recreation_zone,
    float sim_hours_elapsed,
    oasis::time::TimeWarpState* warp_state = nullptr
) noexcept {
    // 1. Burnout Entry (stress >= 1.0f) + Emergency Warp Auto-Downshift to 1x
    if (psych.stress >= 1.0f) {
        burnout.is_burned_out = true;
        if (warp_state != nullptr && warp_state->multiplier > 1) {
            oasis::time::EvaluateEmergencyWarpBrake(
                *warp_state,
                somatic.caloric_reserve,
                somatic.hydration,
                psych.stress,
                false
            );
        }
    }

    // 2. Life-Critical Somatic Preemption (hydration < 0.20f || caloric_reserve < 500.0 kcal)
    // Preempts STATE_BURNOUT so a burned-out agent drinks water and eats rations instead of dying!
    if (somatic.hydration < 0.20f || somatic.caloric_reserve < 500.0) {
        burnout.life_critical_preemption_active = true;
        bool telemetry_emitted = false;
        StepSomaticIntake(somatic, psych, camp, telemetry_emitted);
    } else {
        burnout.life_critical_preemption_active = false;
    }

    // 3. Stress Drainage (-40%/hr social recreation, -25%/hr solo recreation, -10%/hr self-soothe fallback)
    if (sim_hours_elapsed > 0.0f) {
        if (in_recreation_zone) {
            burnout.autonomous_self_sooth_active = false;
            const float drain_per_hr = has_social_companion ? 0.40f : 0.25f;
            psych.stress = std::max(0.0f, psych.stress - drain_per_hr * sim_hours_elapsed);
            psych.mood = std::min(1.0f, psych.mood + 0.15f * sim_hours_elapsed);
        } else if (burnout.is_burned_out && !has_reachable_recreation_zone) {
            burnout.autonomous_self_sooth_active = true;
            psych.stress = std::max(0.0f, psych.stress - 0.10f * sim_hours_elapsed);
        }
    }

    // 4. Burnout Exit Hysteresis (stress < 0.40f)
    if (burnout.is_burned_out && psych.stress < 0.40f) {
        burnout.is_burned_out = false;
        burnout.autonomous_self_sooth_active = false;
    }
}

} // namespace oasis::chill

namespace oasis::homeostasis {
    constexpr double BASAL_KCAL_PER_SEC = 2000.0 / 86400.0;
    constexpr float  BASAL_HYDRATION_DRAIN_PER_SEC = 1.0f / 86400.0f;
    using oasis::chill::TICKS_PER_SIM_HOUR;
    using oasis::chill::TICKS_PER_SIM_DAY;
    using oasis::chill::MAX_BFS_VISITED;
    using oasis::chill::EnclosureStatus;
    using oasis::chill::EnclosureCacheComponent;
    using oasis::chill::IsAirtightBarrierMaterial;
    using oasis::chill::IsPassableForPathfinding;
    using oasis::chill::MarkEnclosureDirtyOnVoxelChange;
    using oasis::chill::EvaluateThermalEnclosure;
    using oasis::chill::StepInsulatedSleepRecovery;
    using oasis::chill::GenesisCampStockComponent;
    using oasis::chill::HasRequiredMaterialInStock;
    using oasis::chill::ConsumeRequiredMaterialFromStock;
    using oasis::chill::RefundRequiredMaterialToStock;
    using oasis::chill::FormatMaterialKindName;
    using oasis::chill::EvaluateMetabolicSpeedMultiplier;
    using oasis::chill::FormatMetabolicStateLabel;
    using oasis::chill::CookHomesteadMealAtKitchen;
    using oasis::chill::BootstrapGenesisSurvivalCamp;
    using oasis::chill::BootstrapCleanSuburbanGenesisLot;
    using oasis::chill::StepSomaticIntake;
    using oasis::chill::BurnoutStateComponent;
    using oasis::chill::CanExecuteDesignatorAction;
    using oasis::chill::StepRecreationAndBurnout;
} // namespace oasis::homeostasis

namespace oasis::graphics {
    constexpr int MIN_CUTAWAY_Z = 1;
    constexpr int MAX_CUTAWAY_Z = 32;
} // namespace oasis::graphics


namespace oasis::fabrication {
    using oasis::chill::MAT_DOOR_TIMBER;
    using oasis::chill::MAT_HAUL_ZONE;
    using oasis::chill::MAT_SLEEP_ZONE;
    using oasis::chill::MAT_RECREATION_ZONE;
    using oasis::chill::MAT_BELL_TENT;
    using oasis::chill::MAT_RATION_CRATE;
    using oasis::chill::EnclosureStatus;
    using oasis::chill::EnclosureCacheComponent;
    using oasis::chill::IsAirtightBarrierMaterial;
    using oasis::chill::IsPassableForPathfinding;
    using oasis::chill::EvaluateThermalEnclosure;
    using oasis::chill::StepInsulatedSleepRecovery;
    using oasis::chill::GenesisCampStockComponent;
    using oasis::chill::HasRequiredMaterialInStock;
    using oasis::chill::ConsumeRequiredMaterialFromStock;
    using oasis::chill::RefundRequiredMaterialToStock;
    using oasis::chill::FormatMaterialKindName;
    using oasis::chill::EvaluateMetabolicSpeedMultiplier;
    using oasis::chill::FormatMetabolicStateLabel;
    using oasis::chill::CookHomesteadMealAtKitchen;
    using oasis::chill::BootstrapGenesisSurvivalCamp;
    using oasis::chill::BootstrapCleanSuburbanGenesisLot;
    using oasis::chill::StepSomaticIntake;
    using oasis::chill::BurnoutStateComponent;
    using oasis::chill::CanExecuteDesignatorAction;
    using oasis::chill::StepRecreationAndBurnout;
} // namespace oasis::fabrication

namespace oasis {
    using oasis::chill::TICKS_PER_SIM_HOUR;
    using oasis::chill::TICKS_PER_SIM_DAY;
    using oasis::chill::MAX_BFS_VISITED;
    using oasis::chill::MAT_DOOR_TIMBER;
    using oasis::chill::MAT_HAUL_ZONE;
    using oasis::chill::MAT_SLEEP_ZONE;
    using oasis::chill::MAT_RECREATION_ZONE;
    using oasis::chill::MAT_BELL_TENT;
    using oasis::chill::MAT_RATION_CRATE;
    using oasis::chill::EnclosureStatus;
    using oasis::chill::EnclosureCacheComponent;
    using oasis::chill::IsAirtightBarrierMaterial;
    using oasis::chill::IsPassableForPathfinding;
    using oasis::chill::MarkEnclosureDirtyOnVoxelChange;
    using oasis::chill::EvaluateThermalEnclosure;
    using oasis::chill::StepInsulatedSleepRecovery;
    using oasis::chill::GenesisCampStockComponent;
    using oasis::chill::HasRequiredMaterialInStock;
    using oasis::chill::ConsumeRequiredMaterialFromStock;
    using oasis::chill::RefundRequiredMaterialToStock;
    using oasis::chill::FormatMaterialKindName;
    using oasis::chill::EvaluateMetabolicSpeedMultiplier;
    using oasis::chill::FormatMetabolicStateLabel;
    using oasis::chill::CookHomesteadMealAtKitchen;
    using oasis::chill::BootstrapGenesisSurvivalCamp;
    using oasis::chill::BootstrapCleanSuburbanGenesisLot;
    using oasis::chill::StepSomaticIntake;
    using oasis::chill::BurnoutStateComponent;
    using oasis::chill::CanExecuteDesignatorAction;
    using oasis::chill::StepRecreationAndBurnout;
} // namespace oasis
