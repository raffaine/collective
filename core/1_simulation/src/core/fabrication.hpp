#pragma once

#include <glm/glm.hpp>
#include <flecs.h>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace oasis::fabrication {

constexpr uint8_t MAT_CLAY_COB_LUMP  = 30;
constexpr uint8_t MAT_URBANITE_CHUNK = 31;

enum class InfrastructureType : uint8_t {
    NONE = 0,
    SOLAR_PANEL = 1,         // Material 11: Photovoltaic collector
    RAINWATER_CATCHMENT = 2, // Material 12: Cistern / filter bed
    ROCKET_MASS_HEATER = 3,
    COMPOST_LATRINE = 4,
    SWALE = 5,
    DIG_SWALE = 5,           // Alias for SWALE
    COVER_CROP = 6,
    PLANT_COVER_CROP = 6,    // Alias for COVER_CROP
    WALL_COB = 7,
    WALL_URBANITE = 8,
    DOOR_TIMBER = 9,
    ROOF_TIMBER = 10,
    SLEEP_ZONE = 11,
    RECREATION_ZONE = 12,
    HAUL_ZONE = 13,
    STOCKPILE_ZONE = 13      // Alias for HAUL_ZONE
};

constexpr uint8_t MAT_REQ_NONE         = 0u;
constexpr uint8_t MAT_REQ_SOLAR_PV_KIT = 1u;
constexpr uint8_t MAT_REQ_TIMBER_PLANK = 2u;
constexpr uint8_t MAT_REQ_COB_LUMP     = 3u;
constexpr uint8_t MAT_REQ_SEED_PACK    = 4u;

enum class FounderActionState : uint8_t {
    IDLE = 0,
    NAVIGATING = 1,
    CONSTRUCTING = 2,
    RESTING = 3,
    EXHAUSTED = 4,
    HAULING_TO_SOURCE = 5,
    HAULING_TO_SITE = 6,
    SEEKING_FOOD = 7,
    SEEKING_WATER = 8,
    HARVESTING_CROP = 9,
    COOKING_MEAL = 10
};

// Loose item spawned by earthworks / demolition (e.g. Clay/Cob Lump or Urbanite Chunk)
struct LooseItemComponent {
    uint8_t material_id{MAT_CLAY_COB_LUMP};
    int x{0};
    int y{0};
    int z{0};
    uint32_t quantity{1};
    bool is_hauled{false};
    bool is_reserved{false};

    [[nodiscard]] glm::ivec3 pos() const noexcept { return {x, y, z}; }
};

// Designated 1-voxel-high ground stockpile zone for loose items
struct StockpileZoneComponent {
    int min_x{0};
    int min_z{0};
    int max_x{0};
    int max_z{0};
    int y{0};
    uint32_t stored_cob_lumps{0};
    uint32_t stored_urbanite_chunks{0};

    [[nodiscard]] bool Contains(int vx, int vz) const noexcept {
        return vx >= min_x && vx <= max_x && vz >= min_z && vz <= max_z;
    }
};

// Metabolic and physiological reserves
struct SomaticStateComponent {
    double caloric_reserve{2500.0};             // Available metabolic reserve (kcal)
    double basal_metabolic_rate{2000.0 / 86400.0}; // ~0.0231 kcal/sec resting burn
    double construction_metabolic_rate{0.080};   // kcal/tick active construction burn
    float focus{1.0f};                          // [0.0 .. 1.0]
    float stamina{1.0f};                        // [0.0 .. 1.0]
    bool is_exhausted{false};
    float hydration{1.0f};                      // [0.0 .. 1.0]
    float body_temp_c{37.0f};                   // Core body temperature in °C
    float ambient_temp_c{22.5f};                // Ambient environment temperature in °C
    float hygiene{0.88f};                       // [0.0 .. 1.0] (restored by Bathroom Shower & Sink!)
    float bladder_comfort{0.90f};               // [0.0 .. 1.0] (restored by Bathroom Toilet!)
    float entertainment_comfort{0.85f};         // [0.0 .. 1.0] (restored by Living Room Couch / TV / VideoGame / Sound System!)
    float core_temp_c{37.0f};                   // Core body temperature in °C

    [[nodiscard]] double calories() const noexcept { return caloric_reserve; }
};

// Spatial placement and movement
struct SpatialPositionComponent {
    glm::vec3 pos{0.0f, 1.0f, 0.0f};     // Continuous world position
    glm::vec3 velocity{0.0f};
    float move_speed{3.0f};              // Traversal speed (units / sec)
};

// Player directive and active task handle
struct FounderCommandComponent {
    FounderActionState state{FounderActionState::IDLE};
    glm::ivec3 target_voxel{0};
    flecs::entity target_blueprint{};
};

// Queued physical construction site
struct BlueprintComponent {
    InfrastructureType type{InfrastructureType::NONE};
    int x{0};
    int y{0};
    int z{0};
    double required_work_ticks{100.0};   // Total work ticks required
    double invested_work_ticks{0.0};     // Accumulated work ticks
    double caloric_cost_total{500.0};    // Total kcal required
    bool is_completed{false};
    float focus_cost{0.20f};             // Focus drain upon completion
    uint32_t reserved_cob_lumps{0};      // Reserved MAT_CLAY_COB_LUMP count
    uint32_t reserved_urbanite_chunks{0};// Reserved MAT_URBANITE_CHUNK count
    uint8_t build_stage{0};              // 0=Ghost(26u), 1=Material Delivered/Frame(33u), 2=Wiring(50%+), 3=Done
    bool requires_material_haul{false};
    bool materials_delivered{true};      // Default true for legacy headless tests; set false in resource-gated blueprints
    uint8_t required_material_kind{0};   // 0=None, 1=Solar PV Kit, 2=Timber Plank, 3=Cob/Earth Lump, 4=Seed Pack

    [[nodiscard]] glm::ivec3 target_voxel() const noexcept { return {x, y, z}; }
};

// Backyard garden bed crop growth & ripening state
struct GardenCropComponent {
    int x{0};
    int y{1};
    int z{0};
    float growth_ticks{0.0f};
    float required_growth_ticks{300.0f};
    bool is_mature{false};

    [[nodiscard]] glm::ivec3 pos() const noexcept { return {x, y, z}; }
};

// Completed physical voxel structure
struct VoxelPlacedComponent {
    InfrastructureType type{InfrastructureType::NONE};
    int x{0};
    int y{0};
    int z{0};
    uint64_t completed_tick{0};

    [[nodiscard]] glm::ivec3 pos() const noexcept { return {x, y, z}; }
};

// Auto-hauls any unhauled LooseItemComponent entities to the first active StockpileZoneComponent
inline size_t HaulLooseItemsToStockpile(flecs::world& ecs) {
    flecs::entity target_stockpile{};
    ecs.each([&](flecs::entity se, StockpileZoneComponent&) {
        if (!target_stockpile.is_alive()) {
            target_stockpile = se;
        }
    });

    if (!target_stockpile.is_alive()) {
        return 0;
    }

    auto& zone = target_stockpile.get_mut<StockpileZoneComponent>();
    size_t hauled_count = 0;

    ecs.each([&](flecs::entity, LooseItemComponent& item) {
        if (item.is_hauled) return;
        if (item.material_id == MAT_CLAY_COB_LUMP) {
            zone.stored_cob_lumps += item.quantity;
        } else if (item.material_id == MAT_URBANITE_CHUNK) {
            zone.stored_urbanite_chunks += item.quantity;
        }
        item.x = zone.min_x;
        item.y = zone.y;
        item.z = zone.min_z;
        item.is_hauled = true;
        hauled_count++;
    });

    return hauled_count;
}

// Helper stepping function for headless tests and tick loops
inline void StepConstructionSystem(flecs::world& ecs, double dt = 1.0) {
    (void)dt;
    auto founder = ecs.entity("Founder");
    if (!founder.is_alive() || !founder.has<SomaticStateComponent>()) return;

    auto& somatic = founder.get_mut<SomaticStateComponent>();
    if (somatic.caloric_reserve <= 500.0) {
        somatic.is_exhausted = true;
        return; // Caloric exhaustion guard halts construction
    }

    std::vector<flecs::entity> completed_entities;
    std::vector<VoxelPlacedComponent> placed_voxels;

    ecs.each([&](flecs::entity e, BlueprintComponent& bp) {
        if (bp.is_completed || somatic.is_exhausted) return;
        if (bp.requires_material_haul && !bp.materials_delivered) return;

        // Advance work
        double work_delta = 1.0;
        bp.invested_work_ticks += work_delta;

        if (bp.invested_work_ticks >= 0.5 * bp.required_work_ticks) {
            bp.build_stage = 2;
        }

        // Caloric expenditure: construction exertion burn
        double burn = somatic.construction_metabolic_rate;
        somatic.caloric_reserve -= burn;

        // Focus and stamina depletion
        somatic.focus = std::max(0.0f, somatic.focus - 0.002f);
        somatic.stamina = std::max(0.0f, somatic.stamina - 0.005f);

        // Check completion
        if (bp.invested_work_ticks >= bp.required_work_ticks) {
            bp.build_stage = 3;
            bp.is_completed = true;
            completed_entities.push_back(e);
            placed_voxels.push_back({bp.type, bp.x, bp.y, bp.z, 0});
        }
    });

    for (size_t i = 0; i < completed_entities.size(); ++i) {
        completed_entities[i].destruct();
        const auto& placed = placed_voxels[i];
        ecs.entity().set<VoxelPlacedComponent>(placed);

        if (placed.type == InfrastructureType::SWALE) {
            ecs.entity().set<LooseItemComponent>({
                MAT_CLAY_COB_LUMP,
                placed.x,
                placed.y,
                placed.z,
                1u,
                false,
                false
            });
            HaulLooseItemsToStockpile(ecs);
        }
    }
}

} // namespace oasis::fabrication
