#pragma once

#include <glm/glm.hpp>
#include <flecs.h>
#include <cstdint>
#include <string>
#include <algorithm>

namespace oasis::fabrication {

enum class InfrastructureType : uint8_t {
    NONE = 0,
    SOLAR_PANEL = 1,        // Material 11: Photovoltaic collector
    RAINWATER_CATCHMENT = 2, // Material 12: Cistern / filter bed
    ROCKET_MASS_HEATER = 3,
    COMPOST_LATRINE = 4
};

enum class FounderActionState : uint8_t {
    IDLE = 0,
    NAVIGATING = 1,
    CONSTRUCTING = 2,
    RESTING = 3,
    EXHAUSTED = 4
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

    [[nodiscard]] glm::ivec3 target_voxel() const noexcept { return {x, y, z}; }
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

        // Advance work
        double work_delta = 1.0;
        bp.invested_work_ticks += work_delta;

        // Caloric expenditure: construction exertion burn
        double burn = somatic.construction_metabolic_rate;
        somatic.caloric_reserve -= burn;

        // Focus and stamina depletion
        somatic.focus = std::max(0.0f, somatic.focus - 0.002f);
        somatic.stamina = std::max(0.0f, somatic.stamina - 0.005f);

        // Check completion
        if (bp.invested_work_ticks >= bp.required_work_ticks) {
            bp.is_completed = true;
            completed_entities.push_back(e);
            placed_voxels.push_back({bp.type, bp.x, bp.y, bp.z, 0});
        }
    });

    for (size_t i = 0; i < completed_entities.size(); ++i) {
        completed_entities[i].destruct();
        ecs.entity().set<VoxelPlacedComponent>(placed_voxels[i]);
    }
}

} // namespace oasis::fabrication
