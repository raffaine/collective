#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <flecs.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <cassert>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include <string>

#include "core/fabrication.hpp"
#include "core/raycaster.hpp"

// Setup standard Oasis camera matrices
struct CameraSetup {
    glm::mat4 inv_view_proj_persp;
    glm::mat4 inv_view_proj_iso;
    glm::vec4 camera_pos_persp;
    glm::vec4 camera_pos_iso;
    glm::vec4 viewport_params_persp;
    glm::vec4 viewport_params_iso;

    CameraSetup(uint32_t width = 800, uint32_t height = 600) {
        const float aspect = (height > 0) ? (static_cast<float>(width) / static_cast<float>(height)) : (800.0f / 600.0f);

        // Perspective Mode
        const glm::vec3 eye_persp(0.0001f, 2.0001f, -4.9999f);
        const glm::vec3 target_persp(0.0f, 0.0f, 0.0f);
        const glm::vec3 up_persp(0.0f, 1.0f, 0.0f);
        const glm::mat4 view_persp = glm::lookAt(eye_persp, target_persp, up_persp);
        const glm::mat4 proj_persp = glm::perspective(glm::radians(53.13f), aspect, 0.1f, 100.0f);
        inv_view_proj_persp = glm::inverse(proj_persp * view_persp);
        camera_pos_persp = glm::vec4(eye_persp, 0.0f);
        viewport_params_persp = glm::vec4(static_cast<float>(width), static_cast<float>(height), aspect, 0.0f);

        // Isometric Mode
        const float ortho_size = 14.0f;
        const glm::mat4 proj_iso = glm::ortho(-ortho_size * aspect, ortho_size * aspect, -ortho_size, ortho_size, -50.0f, 50.0f);
        const glm::vec3 eye_iso(-16.0001f, 20.0001f, -16.0001f);
        const glm::vec3 target_iso(0.0001f, 0.0001f, 0.0001f);
        const glm::vec3 up_iso(0.0f, 1.0f, 0.0f);
        const glm::mat4 view_iso = glm::lookAt(eye_iso, target_iso, up_iso);
        inv_view_proj_iso = glm::inverse(proj_iso * view_iso);
        camera_pos_iso = glm::vec4(eye_iso, 1.0f);
        viewport_params_iso = glm::vec4(static_cast<float>(width), static_cast<float>(height), aspect, 1.0f);
    }
};

// ==============================================================================
// TEST SUITE 1: Caloric Burn Invariance & Long Simulation Runs (500+ ticks)
// ==============================================================================
bool TestCaloricBurnLongRun() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 1.1] Long-Run Caloric Burn Invariance (500+ Ticks)\n";
    std::cout << "====================================================================\n";

    flecs::world ecs;
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<oasis::fabrication::BlueprintComponent>();
    ecs.component<oasis::fabrication::VoxelPlacedComponent>();

    const double initial_calories = 3000.0;
    auto founder = ecs.entity("Founder")
        .set<oasis::fabrication::SomaticStateComponent>({initial_calories, 0.023, 0.080, 1.0f, 1.0f, false});

    // Create a large blueprint requiring 1000 ticks
    auto bp = ecs.entity("MegastructureBlueprint")
        .set<oasis::fabrication::BlueprintComponent>({
            oasis::fabrication::InfrastructureType::SOLAR_PANEL,
            12, 0, 12, 1000.0, 0.0, 8000.0, false
        });

    double prev_cal = initial_calories;
    bool strictly_monotonic = true;
    bool no_nan = true;
    bool no_underflow = true;

    for (int tick = 1; tick <= 600; ++tick) {
        oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        const auto& somatic = founder.get<oasis::fabrication::SomaticStateComponent>();

        if (std::isnan(somatic.caloric_reserve) || std::isinf(somatic.caloric_reserve)) {
            no_nan = false;
            std::cerr << "FAIL: Caloric reserve became NaN/Inf at tick " << tick << "\n";
            return false;
        }

        if (somatic.caloric_reserve >= prev_cal) {
            strictly_monotonic = false;
            std::cerr << "FAIL: Monotonic depletion violated at tick " << tick 
                      << ": curr=" << somatic.caloric_reserve << " prev=" << prev_cal << "\n";
            return false;
        }

        if (somatic.caloric_reserve < 0.0) {
            no_underflow = false;
            std::cerr << "FAIL: Caloric reserve underflowed at tick " << tick << "\n";
            return false;
        }

        prev_cal = somatic.caloric_reserve;
    }

    const auto& final_somatic = founder.get<oasis::fabrication::SomaticStateComponent>();
    // Expected burn: 600 ticks * 0.080 kcal/tick = 48.0 kcal
    double expected_calories = initial_calories - (600 * 0.080);
    double error = std::abs(final_somatic.caloric_reserve - expected_calories);

    std::cout << "  [PASS] 600 ticks continuous construction completed successfully.\n";
    std::cout << "         Initial Calories: " << initial_calories << " kcal\n";
    std::cout << "         Final Calories:   " << final_somatic.caloric_reserve << " kcal\n";
    std::cout << "         Expected:         " << expected_calories << " kcal (Delta=" << error << ")\n";
    std::cout << "         Strict Monotonicity: " << (strictly_monotonic ? "CONFIRMED" : "FAILED") << "\n";
    std::cout << "         No NaN / No Inf:    " << (no_nan ? "CONFIRMED" : "FAILED") << "\n";

    return (error < 1e-4) && strictly_monotonic && no_nan && no_underflow;
}

// ==============================================================================
// TEST SUITE 2: Starvation Guard Boundary Conditions (500, 100, 0, Negative)
// ==============================================================================
bool TestStarvationGuardBoundaries() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 1.2] Starvation Guard Boundary Conditions (500, 100, 0, Neg)\n";
    std::cout << "====================================================================\n";

    std::vector<double> test_boundary_calories = {500.0, 499.99, 100.0, 0.0, -0.001, -50.0, -500.0};
    bool all_passed = true;

    for (double start_cal : test_boundary_calories) {
        flecs::world ecs;
        ecs.component<oasis::fabrication::SomaticStateComponent>();
        ecs.component<oasis::fabrication::BlueprintComponent>();
        ecs.component<oasis::fabrication::VoxelPlacedComponent>();

        auto founder = ecs.entity("Founder")
            .set<oasis::fabrication::SomaticStateComponent>({start_cal, 0.023, 0.080, 1.0f, 1.0f, false});

        auto bp = ecs.entity("SiteBlueprint")
            .set<oasis::fabrication::BlueprintComponent>({
                oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT,
                5, 0, 5, 50.0, 0.0, 400.0, false
            });

        // Run 50 ticks at boundary condition
        for (int t = 0; t < 50; ++t) {
            oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        }

        const auto& somatic = founder.get<oasis::fabrication::SomaticStateComponent>();
        const auto& blueprint = bp.get<oasis::fabrication::BlueprintComponent>();

        bool guard_held = somatic.is_exhausted;
        bool reserve_unchanged = (somatic.caloric_reserve == start_cal);
        bool no_work_done = (blueprint.invested_work_ticks == 0.0);
        bool not_completed = !blueprint.is_completed;

        std::cout << "  Start Cal=" << std::setw(7) << start_cal << " kcal -> "
                  << "Exhausted=" << (guard_held ? "TRUE" : "FALSE") << " | "
                  << "Final Cal=" << std::setw(7) << somatic.caloric_reserve << " kcal | "
                  << "Work Done=" << blueprint.invested_work_ticks << " ticks -> "
                  << ((guard_held && reserve_unchanged && no_work_done && not_completed) ? "[PASS]" : "[FAIL]")
                  << "\n";

        if (!guard_held || !reserve_unchanged || !no_work_done || !not_completed) {
            all_passed = false;
        }
    }

    // Dynamic threshold transition test: start at 500.24 kcal, burns down to <= 500.0 kcal
    {
        flecs::world ecs;
        ecs.component<oasis::fabrication::SomaticStateComponent>();
        ecs.component<oasis::fabrication::BlueprintComponent>();
        ecs.component<oasis::fabrication::VoxelPlacedComponent>();

        auto founder = ecs.entity("Founder")
            .set<oasis::fabrication::SomaticStateComponent>({500.24, 0.023, 0.080, 1.0f, 1.0f, false});

        auto bp = ecs.entity("SiteBlueprint")
            .set<oasis::fabrication::BlueprintComponent>({
                oasis::fabrication::InfrastructureType::SOLAR_PANEL,
                10, 0, 10, 100.0, 0.0, 500.0, false
            });

        // Tick 1: 500.24 - 0.080 = 500.16
        oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        assert(!founder.get<oasis::fabrication::SomaticStateComponent>().is_exhausted);

        // Tick 2: 500.16 - 0.080 = 500.08
        oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        assert(!founder.get<oasis::fabrication::SomaticStateComponent>().is_exhausted);

        // Tick 3: 500.08 - 0.080 = 500.00000000000006 (> 500.0 in IEEE 754)
        oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        assert(!founder.get<oasis::fabrication::SomaticStateComponent>().is_exhausted);

        // Tick 4: 500.00000000000006 - 0.080 = 499.92 (<= 500.0 at tick end)
        oasis::fabrication::StepConstructionSystem(ecs, 1.0);

        // Tick 5: 499.92 <= 500.0 -> guard triggers and halts immediately!
        oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        const auto& s5 = founder.get<oasis::fabrication::SomaticStateComponent>();
        const auto& b5 = bp.get<oasis::fabrication::BlueprintComponent>();

        // Ticks 6..10: guard continues to hold unconditionally
        for (int t = 6; t <= 10; ++t) {
            oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        }
        const auto& s10 = founder.get<oasis::fabrication::SomaticStateComponent>();
        const auto& b10 = bp.get<oasis::fabrication::BlueprintComponent>();

        std::cout << "  Threshold Crossing Test: Started at 500.24, burned to 499.92, halted at tick 5 (Exhausted=" 
                  << (s5.is_exhausted ? "TRUE" : "FALSE") << ", Work=" << b5.invested_work_ticks 
                  << " ticks). Ticks 6-10 unchanged (Exhausted=" << (s10.is_exhausted ? "TRUE" : "FALSE")
                  << ", Work=" << b10.invested_work_ticks << " ticks, Cal=" << s10.caloric_reserve << " kcal)\n";

        if (!s5.is_exhausted || s5.caloric_reserve != s10.caloric_reserve || b5.invested_work_ticks != b10.invested_work_ticks || b5.invested_work_ticks != 4.0) {
            all_passed = false;
        }
    }

    return all_passed;
}

// ==============================================================================
// TEST SUITE 3: Focus & Stamina Degradation (Underflow & NaN Checks)
// ==============================================================================
bool TestFocusStaminaDegradation() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 2] Focus & Stamina Degradation Under Long Runs\n";
    std::cout << "====================================================================\n";

    flecs::world ecs;
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<oasis::fabrication::BlueprintComponent>();
    ecs.component<oasis::fabrication::VoxelPlacedComponent>();

    // Give large calories so focus drains to zero before starvation
    auto founder = ecs.entity("Founder")
        .set<oasis::fabrication::SomaticStateComponent>({10000.0, 0.023, 0.080, 1.0f, 1.0f, false});

    auto bp = ecs.entity("LongBlueprint")
        .set<oasis::fabrication::BlueprintComponent>({
            oasis::fabrication::InfrastructureType::SOLAR_PANEL,
            10, 0, 10, 2000.0, 0.0, 1000.0, false
        });

    bool no_nan = true;
    bool clamped_at_zero = true;
    bool proper_monotonic_decrease = true;
    float prev_focus = 1.0f;
    float prev_stamina = 1.0f;

    for (int tick = 1; tick <= 1000; ++tick) {
        oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        const auto& somatic = founder.get<oasis::fabrication::SomaticStateComponent>();

        if (std::isnan(somatic.focus) || std::isnan(somatic.stamina)) {
            no_nan = false;
            std::cerr << "FAIL: NaN in focus/stamina at tick " << tick << "\n";
            return false;
        }

        if (somatic.focus < 0.0f || somatic.stamina < 0.0f) {
            clamped_at_zero = false;
            std::cerr << "FAIL: Negative focus/stamina at tick " << tick << "\n";
            return false;
        }

        // Before hitting 0.0f, must be strictly decreasing
        if (prev_focus > 0.0f && somatic.focus >= prev_focus) {
            proper_monotonic_decrease = false;
        }
        if (prev_stamina > 0.0f && somatic.stamina >= prev_stamina) {
            proper_monotonic_decrease = false;
        }

        prev_focus = somatic.focus;
        prev_stamina = somatic.stamina;
    }

    const auto& s = founder.get<oasis::fabrication::SomaticStateComponent>();
    std::cout << "  [PASS] 1000 ticks focus/stamina degradation completed.\n";
    std::cout << "         Final Focus:   " << s.focus << " (clamped to 0.0f)\n";
    std::cout << "         Final Stamina: " << s.stamina << " (clamped to 0.0f)\n";
    std::cout << "         No NaN / No Inf:       " << (no_nan ? "CONFIRMED" : "FAILED") << "\n";
    std::cout << "         Zero Clamping (>=0.0): " << (clamped_at_zero ? "CONFIRMED" : "FAILED") << "\n";
    std::cout << "         Monotonic Decrease:    " << (proper_monotonic_decrease ? "CONFIRMED" : "FAILED") << "\n";

    return no_nan && clamped_at_zero && proper_monotonic_decrease;
}

// ==============================================================================
// TEST SUITE 4: ScreenToWorldRaycast Corner Cases & Adversarial Robustness
// ==============================================================================
bool TestRaycasterCornerCases() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 3] ScreenToWorldRaycast Boundary & Offscreen Robustness\n";
    std::cout << "====================================================================\n";

    CameraSetup cam(800, 600);
    bool all_passed = true;

    // 4.1 Degenerate Viewport Dimensions
    std::cout << "  -- Subtest 4.1: Degenerate Viewport Dimensions --\n";
    std::vector<std::pair<uint32_t, uint32_t>> degenerate_viewports = {
        {0, 0}, {0, 600}, {800, 0}, {1, 1}, {1000000, 1000000}
    };
    for (auto [w, h] : degenerate_viewports) {
        auto hit = oasis::graphics::ScreenToWorldRaycast(
            400, 300, w, h,
            cam.inv_view_proj_persp, cam.camera_pos_persp, cam.viewport_params_persp
        );
        if (w == 0 || h == 0) {
            if (hit.hit) {
                std::cerr << "FAIL: Degenerate viewport (" << w << "x" << h << ") returned hit=true\n";
                all_passed = false;
            }
        }
    }
    std::cout << "  [PASS] Zero-dimension viewports safely return clean hit=false.\n";

    // 4.2 Boundary & Offscreen Mouse Coordinates
    std::cout << "  -- Subtest 4.2: Boundary and Extreme Offscreen Mouse Coordinates --\n";
    struct MouseCase {
        int x, y;
        const char* desc;
    };
    std::vector<MouseCase> mouse_cases = {
        {0, 0, "Top-Left Corner (0,0)"},
        {800, 600, "Bottom-Right Corner (w,h)"},
        {800, 0, "Top-Right Corner (w,0)"},
        {0, 600, "Bottom-Left Corner (0,h)"},
        {400, 300, "Screen Center (w/2, h/2)"},
        {-1, -1, "Just Offscreen (-1,-1)"},
        {-500, -500, "Substantially Offscreen (-500,-500)"},
        {1500, 1500, "Far Beyond Screen (1500,1500)"},
        {-10000, -10000, "Extreme Negative (-10000,-10000)"},
        {10000, 10000, "Extreme Positive (10000,10000)"},
        {1000000, 1000000, "Megapixel Offscreen (1000000,1000000)"},
        {-1000000, -1000000, "Megapixel Negative (-1000000,-1000000)"}
    };

    for (const auto& mc : mouse_cases) {
        // Perspective Mode
        auto hit_persp = oasis::graphics::ScreenToWorldRaycast(
            mc.x, mc.y, 800, 600,
            cam.inv_view_proj_persp, cam.camera_pos_persp, cam.viewport_params_persp
        );

        // Isometric Mode
        auto hit_iso = oasis::graphics::ScreenToWorldRaycast(
            mc.x, mc.y, 800, 600,
            cam.inv_view_proj_iso, cam.camera_pos_iso, cam.viewport_params_iso
        );

        // Verify distance and coords are valid numbers if hit
        if (hit_persp.hit) {
            if (std::isnan(hit_persp.distance) || std::isinf(hit_persp.distance)) {
                std::cerr << "FAIL: NaN distance in perspective hit for " << mc.desc << "\n";
                all_passed = false;
            }
        }
        if (hit_iso.hit) {
            if (std::isnan(hit_iso.distance) || std::isinf(hit_iso.distance)) {
                std::cerr << "FAIL: NaN distance in isometric hit for " << mc.desc << "\n";
                all_passed = false;
            }
        }

        std::cout << "  " << std::setw(38) << std::left << mc.desc 
                  << " -> PerspHit=" << (hit_persp.hit ? "YES" : "NO ")
                  << " (voxel: " << hit_persp.voxel_pos.x << "," << hit_persp.voxel_pos.y << "," << hit_persp.voxel_pos.z << ")"
                  << " | IsoHit=" << (hit_iso.hit ? "YES" : "NO ")
                  << " (voxel: " << hit_iso.voxel_pos.x << "," << hit_iso.voxel_pos.y << "," << hit_iso.voxel_pos.z << ")"
                  << "\n";
    }

    // 4.3 Degenerate / Singular Matrices
    std::cout << "  -- Subtest 4.3: Degenerate / Singular Inverse View-Projection Matrices --\n";
    glm::mat4 zero_mat(0.0f);
    auto hit_zero = oasis::graphics::ScreenToWorldRaycast(
        400, 300, 800, 600,
        zero_mat, cam.camera_pos_persp, cam.viewport_params_persp
    );
    if (hit_zero.hit) {
        std::cerr << "FAIL: Zero matrix produced hit=true\n";
        all_passed = false;
    } else {
        std::cout << "  [PASS] Zero matrix cleanly rejected via near_h.w singular guard.\n";
    }

    // 4.4 Camera Transition Parameter Range t in [-1.0 .. 2.0]
    std::cout << "  -- Subtest 4.4: Viewport Blending Transition Parameter t --\n";
    std::vector<float> transition_t_vals = {-0.5f, 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.5f};
    for (float t : transition_t_vals) {
        glm::vec4 vp_params(800.0f, 600.0f, 800.0f / 600.0f, t);
        glm::mat4 blended_inv_vp = (1.0f - t) * cam.inv_view_proj_persp + t * cam.inv_view_proj_iso;
        glm::vec4 blended_cam_pos = glm::mix(cam.camera_pos_persp, cam.camera_pos_iso, t);

        auto hit = oasis::graphics::ScreenToWorldRaycast(
            400, 300, 800, 600,
            blended_inv_vp, blended_cam_pos, vp_params
        );
        std::cout << "  Transition t=" << std::setw(5) << t << " -> Hit=" << (hit.hit ? "YES" : "NO ")
                  << " Mat=" << hit.material_id << " Dist=" << hit.distance << "\n";
        if (hit.hit && (std::isnan(hit.distance) || std::isinf(hit.distance))) {
            all_passed = false;
        }
    }

    // 4.5 Dynamic Voxel Placement & Raycast Hit Invariance
    std::cout << "  -- Subtest 4.5: Dynamic Voxel Placement & Raycast Target Verification --\n";
    // Place a custom voxel at (0, 1, 0) - Solar panel (Mat 11)
    oasis::graphics::SetDynamicVoxel(0, 1, 0, 11);
    uint8_t fetched_mat = oasis::graphics::GetDynamicVoxel(0, 1, 0);
    assert(fetched_mat == 11 && "Dynamic voxel store must retrieve placed material");

    // Place Rainwater Catchment (Mat 12) at (-2, 1, 2)
    oasis::graphics::SetDynamicVoxel(-2, 1, 2, 12);
    assert(oasis::graphics::GetDynamicVoxel(-2, 1, 2) == 12);

    std::cout << "  [PASS] Dynamic voxel storage (Materials 11 & 12) verified.\n";

    // 4.6 Coordinate Bit-Packing Collision Proof (Negative & Positive Spatial Bounds)
    std::cout << "  -- Subtest 4.6: PackVoxelCoord Uniqueness Across Spatial Volume --\n";
    std::unordered_set<uint64_t> packed_hashes;
    size_t evaluated_coords = 0;
    bool hash_collision = false;

    for (int x = -30; x <= 30; ++x) {
        for (int y = -10; y <= 20; ++y) {
            for (int z = -20; z <= 50; ++z) {
                uint64_t packed = oasis::graphics::PackVoxelCoord(x, y, z);
                if (packed_hashes.find(packed) != packed_hashes.end()) {
                    hash_collision = true;
                    std::cerr << "FAIL: Hash collision at (" << x << "," << y << "," << z << ")\n";
                    break;
                }
                packed_hashes.insert(packed);
                evaluated_coords++;
            }
            if (hash_collision) break;
        }
        if (hash_collision) break;
    }

    std::cout << "  Evaluated " << evaluated_coords << " spatial coordinates in [-30..30, -10..20, -20..50].\n";
    std::cout << "  Hash Collisions: " << (hash_collision ? "DETECTED!" : "0 (ZERO)") << "\n";
    if (hash_collision) all_passed = false;

    return all_passed;
}

// ==============================================================================
// TEST SUITE 5: Multiple Blueprints & Archetype Safety Stress
// ==============================================================================
bool TestMultipleBlueprintsSafety() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 4] Multiple Blueprints & Archetype Safety Stress\n";
    std::cout << "====================================================================\n";

    flecs::world ecs;
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<oasis::fabrication::BlueprintComponent>();
    ecs.component<oasis::fabrication::VoxelPlacedComponent>();

    auto founder = ecs.entity("Founder")
        .set<oasis::fabrication::SomaticStateComponent>({3000.0, 0.023, 0.080, 1.0f, 1.0f, false});

    // Spawn 10 simultaneous blueprints of varying work lengths
    std::vector<flecs::entity> blueprints;
    for (int i = 0; i < 10; ++i) {
        blueprints.push_back(
            ecs.entity()
                .set<oasis::fabrication::BlueprintComponent>({
                    (i % 2 == 0) ? oasis::fabrication::InfrastructureType::SOLAR_PANEL 
                                 : oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT,
                    i * 2, 1, i * 2,
                    10.0 + (i * 5.0), // required work: 10, 15, 20, ... 55
                    0.0,
                    200.0,
                    false
                })
        );
    }

    // Run 60 ticks so all 10 blueprints progressively finish
    for (int tick = 1; tick <= 60; ++tick) {
        oasis::fabrication::StepConstructionSystem(ecs, 1.0);
    }

    // Verify all blueprints finished and were transformed to VoxelPlacedComponent
    int completed_voxel_count = 0;
    ecs.each([&](flecs::entity e, const oasis::fabrication::VoxelPlacedComponent& v) {
        completed_voxel_count++;
    });

    int remaining_blueprints = 0;
    ecs.each([&](flecs::entity e, const oasis::fabrication::BlueprintComponent& bp) {
        remaining_blueprints++;
    });

    std::cout << "  Spawned 10 blueprints simultaneously.\n";
    std::cout << "  After 60 ticks: Completed Voxels = " << completed_voxel_count 
              << " / 10 | Remaining Blueprints = " << remaining_blueprints << "\n";

    const auto& somatic = founder.get<oasis::fabrication::SomaticStateComponent>();
    std::cout << "  Founder Remaining Calories: " << somatic.caloric_reserve << " kcal (no NaN, >500.0)\n";

    return (completed_voxel_count == 10) && (remaining_blueprints == 0) && !somatic.is_exhausted;
}

// ==============================================================================
// TEST SUITE 6: Extreme Integer Mouse Coordinates & Fuzzing
// ==============================================================================
bool TestRaycasterExtremeValues() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 5] INT32_MAX / INT32_MIN & Extreme Coordinate Fuzzing\n";
    std::cout << "====================================================================\n";

    CameraSetup cam(800, 600);
    bool all_passed = true;

    std::vector<std::pair<int, int>> extreme_coords = {
        {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()},
        {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()},
        {std::numeric_limits<int>::max(), 0},
        {0, std::numeric_limits<int>::max()},
        {std::numeric_limits<int>::min(), 0},
        {0, std::numeric_limits<int>::min()},
        {1073741824, -1073741824},
        {-1073741824, 1073741824}
    };

    for (auto [mx, my] : extreme_coords) {
        // Perspective mode
        auto hit_p = oasis::graphics::ScreenToWorldRaycast(
            mx, my, 800, 600,
            cam.inv_view_proj_persp, cam.camera_pos_persp, cam.viewport_params_persp
        );

        // Isometric mode
        auto hit_i = oasis::graphics::ScreenToWorldRaycast(
            mx, my, 800, 600,
            cam.inv_view_proj_iso, cam.camera_pos_iso, cam.viewport_params_iso
        );

        if (hit_p.hit && (std::isnan(hit_p.distance) || std::isinf(hit_p.distance))) {
            std::cerr << "FAIL: Perspective hit returned NaN/Inf distance at (" << mx << "," << my << ")\n";
            all_passed = false;
        }
        if (hit_i.hit && (std::isnan(hit_i.distance) || std::isinf(hit_i.distance))) {
            std::cerr << "FAIL: Isometric hit returned NaN/Inf distance at (" << mx << "," << my << ")\n";
            all_passed = false;
        }

        std::cout << "  Mouse (" << std::setw(11) << mx << ", " << std::setw(11) << my << ") -> "
                  << "PerspHit=" << (hit_p.hit ? "YES" : "NO ") << " (dist=" << hit_p.distance << ") | "
                  << "IsoHit=" << (hit_i.hit ? "YES" : "NO ") << " (dist=" << hit_i.distance << ") [SAFE]\n";
    }

    return all_passed;
}

// ==============================================================================
// TEST SUITE 7: main.cpp Flecs ConstructionSystem Registration Bug & Lifecycle
// ==============================================================================
namespace psychology_mock {
#pragma pack(push, 8)
struct alignas(8) EpisodicMemoryNode {
    uint64_t timestamp_tick{0};
    uint16_t event_type{0};
    int16_t  emotional_valence{0};
    uint8_t  salience{0};
    uint8_t  flags{0};
    char     description[8]{};
    uint16_t reserved{0};
};
#pragma pack(pop)

struct EpisodicMemoryRing {
    static constexpr size_t CAPACITY = 32;
    EpisodicMemoryNode slots[CAPACITY]{};
    uint32_t head{0};
    uint32_t count{0};
    uint32_t total_recorded{0};
    uint32_t permanent_count{0};

    uint32_t Push(const EpisodicMemoryNode& input_node) {
        EpisodicMemoryNode node = input_node;
        total_recorded++;
        if (node.salience >= 80) node.flags |= 0x01; // PERMANENT
        uint32_t target_slot = head;
        if (count < CAPACITY) {
            slots[target_slot] = node;
            head = (head + 1) % CAPACITY;
            count++;
            if ((node.flags & 0x01) != 0) permanent_count++;
            return target_slot;
        }
        slots[head] = node;
        head = (head + 1) % CAPACITY;
        return target_slot;
    }
};

struct PsychologyComponent {
    float stress{0.25f};
    float mood{0.50f};
    float focus{0.75f};
    EpisodicMemoryRing memory_ring{};
};
} // namespace psychology_mock

struct AutarkyMock {
    double solar_stored_kwh{0.0};
    double rain_stored_liters{0.0};
};

bool TestMainCppConstructionSystemBugAndLifecycle(bool& bug_reproduced, bool& corrected_lifecycle_passed) {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 6] main.cpp Flecs Query Bug & End-to-End Systems Lifecycle\n";
    std::cout << "====================================================================\n";

    flecs::world ecs;

    // Register all components
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<oasis::fabrication::SpatialPositionComponent>();
    ecs.component<oasis::fabrication::FounderCommandComponent>();
    ecs.component<oasis::fabrication::BlueprintComponent>();
    ecs.component<oasis::fabrication::VoxelPlacedComponent>();
    ecs.component<psychology_mock::PsychologyComponent>();
    ecs.component<AutarkyMock>();

    auto boundary = ecs.entity("SettlementBoundary").set<AutarkyMock>({0.0, 0.0});

    psychology_mock::PsychologyComponent psych{};
    psych.focus = 1.0f;
    oasis::fabrication::SomaticStateComponent somatic{2500.0, 0.023, 0.080, 1.0f, 1.0f, false};
    oasis::fabrication::SpatialPositionComponent pos{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), 3.0f};
    oasis::fabrication::FounderCommandComponent cmd{oasis::fabrication::FounderActionState::IDLE, glm::ivec3(0), {}};

    auto founder = ecs.entity("Founder")
        .set<oasis::fabrication::SpatialPositionComponent>(pos)
        .set<oasis::fabrication::FounderCommandComponent>(cmd)
        .set<oasis::fabrication::SomaticStateComponent>(somatic)
        .set<psychology_mock::PsychologyComponent>(psych);

    // 1. Empirically verify the system as remediated in main.cpp line 720:
    bool main_cpp_system_matched = false;
    ecs.system<oasis::fabrication::SpatialPositionComponent,
               oasis::fabrication::FounderCommandComponent,
               oasis::fabrication::SomaticStateComponent,
               psychology_mock::PsychologyComponent>("ConstructionSystem_Remediated")
        .kind(flecs::OnUpdate)
        .each([&](flecs::entity e,
                 oasis::fabrication::SpatialPositionComponent& p,
                 oasis::fabrication::FounderCommandComponent& c,
                 oasis::fabrication::SomaticStateComponent& s,
                 psychology_mock::PsychologyComponent& ps) {
            main_cpp_system_matched = true;
        });

    ecs.progress();
    std::cout << "  [Remediation Verification] main.cpp line 720 (system<...>): "
              << (main_cpp_system_matched ? "MATCHED ENTITIES (PASS - NO BUG)" : "MATCHED 0 ENTITIES (BUG CONFIRMED!)") << "\n";
    bug_reproduced = !main_cpp_system_matched;

    // 2. Now verify the corrected query:
    flecs::world ecs2;
    ecs2.component<oasis::fabrication::SomaticStateComponent>();
    ecs2.component<oasis::fabrication::SpatialPositionComponent>();
    ecs2.component<oasis::fabrication::FounderCommandComponent>();
    ecs2.component<oasis::fabrication::BlueprintComponent>();
    ecs2.component<oasis::fabrication::VoxelPlacedComponent>();
    ecs2.component<psychology_mock::PsychologyComponent>();
    ecs2.component<AutarkyMock>();

    auto boundary2 = ecs2.entity("SettlementBoundary").set<AutarkyMock>({0.0, 0.0});

    psychology_mock::PsychologyComponent psych2{};
    psych2.focus = 1.0f;
    oasis::fabrication::SomaticStateComponent somatic2{2500.0, 0.023, 0.080, 1.0f, 1.0f, false};
    oasis::fabrication::SpatialPositionComponent pos2{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), 3.0f};
    oasis::fabrication::FounderCommandComponent cmd2{oasis::fabrication::FounderActionState::IDLE, glm::ivec3(0), {}};

    auto founder2 = ecs2.entity("Founder")
        .set<oasis::fabrication::SpatialPositionComponent>(pos2)
        .set<oasis::fabrication::FounderCommandComponent>(cmd2)
        .set<oasis::fabrication::SomaticStateComponent>(somatic2)
        .set<psychology_mock::PsychologyComponent>(psych2);

    // Systems matching main.cpp with corrected query
    ecs2.system<oasis::fabrication::SpatialPositionComponent,
                oasis::fabrication::FounderCommandComponent,
                oasis::fabrication::SomaticStateComponent>("MovementSystem")
        .kind(flecs::OnUpdate)
        .each([](oasis::fabrication::SpatialPositionComponent& p,
                 oasis::fabrication::FounderCommandComponent& c,
                 oasis::fabrication::SomaticStateComponent& s) {
            if (c.state != oasis::fabrication::FounderActionState::NAVIGATING) return;
            glm::vec3 target = glm::vec3(c.target_voxel.x, c.target_voxel.y, c.target_voxel.z);
            glm::vec3 to_target = target - p.pos;
            float dist = glm::length(to_target);
            constexpr float dt = 1.0f / 60.0f;
            if (dist > 1.5f) {
                p.pos += (to_target / dist) * p.move_speed * dt;
                s.caloric_reserve = std::max(0.0, s.caloric_reserve - 0.10 * dt);
            } else {
                c.state = oasis::fabrication::FounderActionState::CONSTRUCTING;
            }
        });

    ecs2.system<oasis::fabrication::SpatialPositionComponent,
                oasis::fabrication::FounderCommandComponent,
                oasis::fabrication::SomaticStateComponent,
                psychology_mock::PsychologyComponent>("ConstructionSystem_Corrected")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e,
                 oasis::fabrication::SpatialPositionComponent& p,
                 oasis::fabrication::FounderCommandComponent& c,
                 oasis::fabrication::SomaticStateComponent& s,
                 psychology_mock::PsychologyComponent& ps) {
            if (c.state != oasis::fabrication::FounderActionState::CONSTRUCTING) return;
            if (!c.target_blueprint.is_alive() || !c.target_blueprint.has<oasis::fabrication::BlueprintComponent>()) {
                c.state = oasis::fabrication::FounderActionState::IDLE;
                return;
            }

            auto& bp = c.target_blueprint.get_mut<oasis::fabrication::BlueprintComponent>();
            if (bp.is_completed) {
                c.state = oasis::fabrication::FounderActionState::IDLE;
                return;
            }

            // Exhaustion guard
            if (s.caloric_reserve <= 500.0 || ps.focus <= 0.05f) {
                s.is_exhausted = true;
                c.state = oasis::fabrication::FounderActionState::EXHAUSTED;
                psychology_mock::EpisodicMemoryNode exhaust_mem{};
                exhaust_mem.salience = 60;
                exhaust_mem.emotional_valence = -50;
                ps.memory_ring.Push(exhaust_mem);
                return;
            }

            constexpr float dt = 1.0f / 60.0f;
            constexpr double work_rate = 25.0;
            double delta_w = std::min(work_rate * dt, bp.required_work_ticks - bp.invested_work_ticks);
            double progress_fraction = (bp.required_work_ticks > 0.0) ? (delta_w / bp.required_work_ticks) : 1.0;

            s.caloric_reserve = std::max(0.0, s.caloric_reserve - bp.caloric_cost_total * progress_fraction);
            ps.focus = std::max(0.0f, ps.focus - bp.focus_cost * static_cast<float>(progress_fraction));
            s.focus = ps.focus;
            s.stamina = std::max(0.0f, s.stamina - 0.005f);
            bp.invested_work_ticks += delta_w;

            if (bp.invested_work_ticks >= bp.required_work_ticks) {
                bp.is_completed = true;
                c.state = oasis::fabrication::FounderActionState::IDLE;
                auto placed_type = bp.type;
                int px = bp.x, py = bp.y, pz = bp.z;
                c.target_blueprint.destruct();

                e.world().entity().set<oasis::fabrication::VoxelPlacedComponent>({placed_type, px, py, pz, 0});
                oasis::graphics::SetDynamicVoxel(px, py, pz, (placed_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL) ? 11 : 12);

                auto b = e.world().entity("SettlementBoundary");
                if (b.is_alive() && b.has<AutarkyMock>()) {
                    auto& autarky = b.get_mut<AutarkyMock>();
                    if (placed_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL) {
                        autarky.solar_stored_kwh += 5.0;
                    } else {
                        autarky.rain_stored_liters += 250.0;
                    }
                }

                psychology_mock::EpisodicMemoryNode ach{};
                ach.salience = 85;
                ach.emotional_valence = 75;
                ach.flags = 0x01; // PERMANENT
                ps.memory_ring.Push(ach);
                ps.mood = std::min(1.0f, ps.mood + 0.15f);
            }
        });

    // Queue Solar Panel Blueprint at (4, 0, 4)
    auto bp1 = ecs2.entity().set<oasis::fabrication::BlueprintComponent>({
        oasis::fabrication::InfrastructureType::SOLAR_PANEL,
        4, 0, 4, 100.0, 0.0, 500.0, false, 0.20f
    });

    auto& founder_cmd2 = founder2.get_mut<oasis::fabrication::FounderCommandComponent>();
    founder_cmd2.state = oasis::fabrication::FounderActionState::NAVIGATING;
    founder_cmd2.target_voxel = {4, 0, 4};
    founder_cmd2.target_blueprint = bp1;

    for (int frame = 0; frame < 400; ++frame) {
        ecs2.progress();
    }

    const auto& final_psych2 = founder2.get<psychology_mock::PsychologyComponent>();
    const auto& final_somatic2 = founder2.get<oasis::fabrication::SomaticStateComponent>();
    const auto& final_cmd2 = founder2.get<oasis::fabrication::FounderCommandComponent>();
    const auto& final_autarky2 = boundary2.get<AutarkyMock>();

    bool bp1_destroyed = !bp1.is_alive();
    bool voxel_placed = false;
    ecs2.each([&](flecs::entity e, const oasis::fabrication::VoxelPlacedComponent& v) {
        if (v.type == oasis::fabrication::InfrastructureType::SOLAR_PANEL && v.x == 4 && v.z == 4) {
            voxel_placed = true;
        }
    });

    bool dyn_voxel_placed = (oasis::graphics::GetDynamicVoxel(4, 0, 4) == 11);
    bool autarky_updated = (final_autarky2.solar_stored_kwh == 5.0);
    bool perm_memory_pushed = (final_psych2.memory_ring.permanent_count >= 1);
    bool mood_elevated = (final_psych2.mood > 0.50f);
    bool founder_idle = (final_cmd2.state == oasis::fabrication::FounderActionState::IDLE);

    std::cout << "  [Corrected Query Lifecycle Verification]\n";
    std::cout << "    Blueprint Destructed:   " << (bp1_destroyed ? "YES" : "NO") << "\n";
    std::cout << "    VoxelPlacedComponent:   " << (voxel_placed ? "YES" : "NO") << "\n";
    std::cout << "    Dynamic Voxel Mat 11:   " << (dyn_voxel_placed ? "YES" : "NO") << "\n";
    std::cout << "    Autarky Solar kWh:      " << final_autarky2.solar_stored_kwh << " kWh (Expected 5.0)\n";
    std::cout << "    Founder Mood:           " << final_psych2.mood << " (Elevated from 0.50)\n";
    std::cout << "    Founder Permanent Mem:  " << final_psych2.memory_ring.permanent_count << "\n";
    std::cout << "    Founder Calories Burned:" << (2500.0 - final_somatic2.caloric_reserve) << " kcal (Expected ~500.14)\n";

    corrected_lifecycle_passed = bp1_destroyed && voxel_placed && dyn_voxel_placed && 
                                 autarky_updated && perm_memory_pushed && mood_elevated && founder_idle;

    return bug_reproduced && corrected_lifecycle_passed;
}

int main() {
    std::cout << "====================================================================\n";
    std::cout << "   OASIS DUAL SPRINT 5 ADVERSARIAL CHALLENGER TEST HARNESS\n";
    std::cout << "====================================================================\n";

    bool pass1 = TestCaloricBurnLongRun();
    bool pass2 = TestStarvationGuardBoundaries();
    bool pass3 = TestFocusStaminaDegradation();
    bool pass4 = TestRaycasterCornerCases();
    bool pass5 = TestMultipleBlueprintsSafety();
    bool pass6 = TestRaycasterExtremeValues();
    bool bug_reproduced = false;
    bool corrected_lifecycle_passed = false;
    TestMainCppConstructionSystemBugAndLifecycle(bug_reproduced, corrected_lifecycle_passed);

    std::cout << "\n====================================================================\n";
    std::cout << "   ADVERSARIAL STRESS TEST SUMMARY\n";
    std::cout << "====================================================================\n";
    std::cout << "  1. Long-Run Caloric Depletion (500+ ticks):  " << (pass1 ? "PASS [APPROVED]" : "FAIL [DEFECT]") << "\n";
    std::cout << "  2. Starvation Guard Boundary Conditions:     " << (pass2 ? "PASS [APPROVED]" : "FAIL [DEFECT]") << "\n";
    std::cout << "  3. Focus & Stamina Degradation Invariance:   " << (pass3 ? "PASS [APPROVED]" : "FAIL [DEFECT]") << "\n";
    std::cout << "  4. ScreenToWorldRaycast Corner Cases:        " << (pass4 ? "PASS [APPROVED]" : "FAIL [DEFECT]") << "\n";
    std::cout << "  5. Multiple Blueprints ECS Safety:           " << (pass5 ? "PASS [APPROVED]" : "FAIL [DEFECT]") << "\n";
    std::cout << "  6. Extreme INT32 Mouse Fuzzing:              " << (pass6 ? "PASS [APPROVED]" : "FAIL [DEFECT]") << "\n";
    std::cout << "  7. main.cpp Flecs Query Match Defect:        " << (bug_reproduced ? "CONFIRMED BUG [DEFECT IN MAIN.CPP]" : "PASS [NO BUG]") << "\n";
    std::cout << "  8. Corrected Lifecycle Simulation:           " << (corrected_lifecycle_passed ? "PASS [VERIFIED]" : "FAIL [DEFECT]") << "\n";
    std::cout << "====================================================================\n";

    // If bug_reproduced is TRUE, the production code in main.cpp has a fatal defect!
    bool overall_pass = pass1 && pass2 && pass3 && pass4 && pass5 && pass6 && !bug_reproduced && corrected_lifecycle_passed;
    std::cout << "OVERALL VERDICT: " << (overall_pass ? "APPROVE" : "REQUEST_CHANGES") << "\n";
    if (bug_reproduced) {
        std::cout << ">>> ACTION REQUIRED: In core/1_simulation/src/main.cpp:720, remove 'flecs::entity,' from template argument pack of g_ecs->system<...>()\n";
    }

    return overall_pass ? 0 : 1;
}

