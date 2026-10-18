#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <glm/glm.hpp>
#include <flecs.h>
#include <cmath>
#include <cstdint>

#include "core/chill_loop.hpp"
#include "core/homeostasis.hpp"
#include "core/fabrication.hpp"
#include "core/raycaster.hpp"
#include "core/spatial_designator.hpp"
#include "core/time_warp_controller.hpp"

using Catch::Approx;
using namespace oasis::chill;
using namespace oasis::homeostasis;
using namespace oasis::fabrication;
using namespace oasis::graphics;
using namespace oasis::designator;
using namespace oasis::time;

namespace {
struct MockPsychComponent {
    float stress{0.25f};
    float mood{0.50f};
    float focus{0.20f};
};
} // namespace

// ============================================================================
// STORY-9.1: ThermalEnclosureSolver, Passable Doors, Dirty-Flag Cache & Sleep
// ============================================================================

TEST_CASE("STORY-9.1: ThermalEnclosureSolver O(H) Sky Check, 512-Node Cap, Passable Doors & Dirty-Flag Cache", "[story-9.1][enclosure]") {
    GetDynamicVoxelStore().clear();

    SECTION("Unroofed volume immediately fails O(H) vertical sky-column check") {
        GetDynamicVoxelStore().clear();
        EnclosureCacheComponent cache{};
        // Open lawn at (25, 1, 25) has no overhead roof
        const EnclosureStatus status = EvaluateThermalEnclosure(cache, glm::ivec3(25, 1, 25), 4.0f);
        REQUIRE(status == EnclosureStatus::UNENCLOSED);
        REQUIRE(cache.has_leak_voxel);
        REQUIRE_FALSE(cache.room_enclosure_dirty);
        REQUIRE(cache.interior_temp_c == Approx(4.0f));
    }

    SECTION("Unwalled canopy terminates at MAX_BFS_VISITED = 512 hard node cap with zero heap allocations") {
        GetDynamicVoxelStore().clear();
        // Place a broad roof canopy at y = 5 over x in [20, 40], z in [20, 40] (21x21x4 = 1,764 air voxels beneath)
        for (int x = 20; x <= 40; ++x) {
            for (int z = 20; z <= 40; ++z) {
                SetDynamicVoxel(x, 5, z, 17u); // Insulated Timber Roof
            }
        }

        EnclosureCacheComponent cache{};
        const EnclosureStatus status = EvaluateThermalEnclosure(cache, glm::ivec3(30, 1, 30), 5.0f);
        REQUIRE(status == EnclosureStatus::UNENCLOSED);
        REQUIRE(cache.has_leak_voxel);
        REQUIRE(cache.enclosed_voxel_count == MAX_BFS_VISITED);
        REQUIRE_FALSE(cache.room_enclosure_dirty);
    }

    SECTION("Enclosed 4-walled room with roof and MAT_DOOR_TIMBER (15u) resolves to ENCLOSED_INSULATED and is passable") {
        GetDynamicVoxelStore().clear();

        // Verify MAT_DOOR_TIMBER (15u) is both an airtight thermal barrier and passable for pathfinding
        REQUIRE(IsAirtightBarrierMaterial(MAT_DOOR_TIMBER));
        REQUIRE(IsPassableForPathfinding(MAT_DOOR_TIMBER));
        REQUIRE_FALSE(IsPassableForPathfinding(13u)); // Cob wall is not passable

        // Build a 5x5x3 room at x in [20, 24], z in [20, 24], y in [1, 3], roof at y = 4
        for (int x = 20; x <= 24; ++x) {
            for (int z = 20; z <= 24; ++z) {
                SetDynamicVoxel(x, 4, z, 17u); // Roof
                for (int y = 1; y <= 3; ++y) {
                    const bool is_perimeter = (x == 20 || x == 24 || z == 20 || z == 24);
                    if (is_perimeter) {
                        SetDynamicVoxel(x, y, z, 13u); // Cob wall
                    }
                }
            }
        }
        // Install an Insulated Timber Door at (22, 1, 20) and (22, 2, 20), plus a Hearth at (23, 1, 23)
        SetDynamicVoxel(22, 1, 20, static_cast<uint8_t>(MAT_DOOR_TIMBER));
        SetDynamicVoxel(22, 2, 20, static_cast<uint8_t>(MAT_DOOR_TIMBER));
        SetDynamicVoxel(23, 1, 23, 16u); // Hearth / Rocket Mass Heater

        EnclosureCacheComponent cache{};
        REQUIRE(cache.room_enclosure_dirty);

        const EnclosureStatus status = EvaluateThermalEnclosure(cache, glm::ivec3(22, 1, 22), 2.0f);
        REQUIRE(status == EnclosureStatus::ENCLOSED_INSULATED);
        REQUIRE_FALSE(cache.has_leak_voxel);
        REQUIRE(cache.has_heat_source);
        REQUIRE(cache.interior_temp_c > 18.0f);
        REQUIRE_FALSE(cache.room_enclosure_dirty);

        // Dirty-flag short-circuit verification: punch a hole in the wall WITHOUT marking dirty -> cached status remains
        SetDynamicVoxel(24, 1, 22, 0u);
        const EnclosureStatus cached_status = EvaluateThermalEnclosure(cache, glm::ivec3(22, 1, 22), 2.0f);
        REQUIRE(cached_status == EnclosureStatus::ENCLOSED_INSULATED);

        // Now mark dirty on voxel change -> re-evaluates and detects the breach!
        MarkEnclosureDirtyOnVoxelChange(cache);
        REQUIRE(cache.room_enclosure_dirty);
        const EnclosureStatus breached_status = EvaluateThermalEnclosure(cache, glm::ivec3(22, 1, 22), 2.0f);
        REQUIRE(breached_status == EnclosureStatus::UNENCLOSED);
        REQUIRE(cache.has_leak_voxel);
    }
}

TEST_CASE("STORY-9.1: Insulated Sleep Recovery (+45 Focus/hr & Zero Cold-Stress Penalty)", "[story-9.1][sleep]") {
    EnclosureCacheComponent enclosure{};
    enclosure.status = EnclosureStatus::ENCLOSED_INSULATED;
    enclosure.interior_temp_c = 21.5f;
    enclosure.has_heat_source = true;
    enclosure.room_enclosure_dirty = false;

    SomaticStateComponent somatic{};
    somatic.caloric_reserve = 2000.0;
    somatic.focus = 0.20f;
    somatic.stamina = 0.25f;
    somatic.is_exhausted = true;

    MockPsychComponent psych{};
    psych.focus = 0.20f;

    // Sleep 4 simulated hours inside insulated shelter while outdoor ambient is 2.0 C
    StepInsulatedSleepRecovery(somatic, psych, enclosure, 4.0f);

    // Zero cold excess penalty: only basal 4 * (2000 / 24) = 333.333 kcal burned
    const double expected_cal = 2000.0 - 4.0 * (2000.0 / 24.0);
    REQUIRE(somatic.caloric_reserve == Approx(expected_cal).margin(1e-4));

    // Focus regenerates at +45 Focus/hr on 0..255 scale = +4 * (45 / 255)
    const float expected_focus = 0.20f + 4.0f * (45.0f / 255.0f);
    REQUIRE(psych.focus == Approx(expected_focus).margin(1e-5f));
    REQUIRE(somatic.focus == Approx(expected_focus).margin(1e-5f));
    REQUIRE_FALSE(somatic.is_exhausted);
}

// ============================================================================
// STORY-9.2: Day 1 Genesis Survival Camp Bootstrap & Autonomous Intake
// ============================================================================

TEST_CASE("STORY-9.2: Day 1 Genesis Survival Camp Bootstrap & Autonomous Somatic Intake", "[story-9.2][genesis-camp]") {
    GetDynamicVoxelStore().clear();
    flecs::world ecs;

    auto camp_entity = BootstrapGenesisSurvivalCamp(ecs);
    REQUIRE(camp_entity.is_alive());
    REQUIRE(camp_entity.has<GenesisCampStockComponent>());

    auto& camp = camp_entity.get_mut<GenesisCampStockComponent>();
    REQUIRE(camp.ration_meals_remaining == 12u);
    REQUIRE(camp.total_ration_kcal() == Approx(9600.0));
    REQUIRE(camp.potable_water_liters == Approx(60.0));
    REQUIRE(camp.cordwood_remaining == 12u);
    REQUIRE(camp.bell_tent_active);
    REQUIRE(camp.campfire_active);
    REQUIRE(camp.tool_locker_present);

    // Verify dynamic voxels placed by BootstrapGenesisSurvivalCamp
    REQUIRE(GetDynamicVoxel(camp.tent_pos.x, camp.tent_pos.y, camp.tent_pos.z) == oasis::chill::MAT_BELL_TENT);
    REQUIRE(GetDynamicVoxel(camp.ration_crate_pos.x, camp.ration_crate_pos.y, camp.ration_crate_pos.z) == oasis::chill::MAT_RATION_CRATE);
    REQUIRE(GetDynamicVoxel(camp.water_tote_pos.x, camp.water_tote_pos.y, camp.water_tote_pos.z) == 12u);
    REQUIRE(GetDynamicVoxel(camp.campfire_pos.x, camp.campfire_pos.y, camp.campfire_pos.z) == oasis::chill::MAT_RECREATION_ZONE);

    // Autonomous Water & Ration Intake when hydration < 0.35f and caloric_reserve < 1200.0 kcal
    SomaticStateComponent somatic{};
    somatic.hydration = 0.30f;
    somatic.caloric_reserve = 1100.0;
    MockPsychComponent psych{};
    psych.stress = 0.50f;
    psych.focus = 0.80f;

    bool telemetry_emitted = false;
    const bool consumed = StepSomaticIntake(somatic, psych, camp, telemetry_emitted);
    REQUIRE(consumed);
    REQUIRE(telemetry_emitted);
    REQUIRE(camp.potable_water_liters == Approx(59.0));
    REQUIRE(somatic.hydration == Approx(0.70f));
    REQUIRE(camp.ration_meals_remaining == 11u);
    REQUIRE(somatic.caloric_reserve == Approx(1900.0));
    REQUIRE(psych.stress == Approx(0.40f)); // -0.10f THOUGHT_WARM_MEAL relief
}

// ============================================================================
// STORY-9.3: Recreation Zones & STATE_BURNOUT Anti-Soft-Lock Safeguards
// ============================================================================

TEST_CASE("STORY-9.3: Recreation Stress Drainage, STATE_BURNOUT Preemption & Anti-Soft-Lock", "[story-9.3][burnout]") {
    GenesisCampStockComponent camp{};

    SECTION("Solo leisure in Recreation Zone drains -25% Stress/hr; Social leisure drains -40% Stress/hr") {
        SomaticStateComponent somatic{};
        BurnoutStateComponent burnout{};

        MockPsychComponent psych_solo{0.80f, 0.40f, 0.80f};
        StepRecreationAndBurnout(somatic, psych_solo, burnout, camp, true, false, true, 1.0f, nullptr);
        REQUIRE(psych_solo.stress == Approx(0.55f));

        MockPsychComponent psych_social{0.80f, 0.40f, 0.80f};
        StepRecreationAndBurnout(somatic, psych_social, burnout, camp, true, true, true, 1.0f, nullptr);
        REQUIRE(psych_social.stress == Approx(0.40f));
    }

    SECTION("Reaching stress >= 1.0f enters STATE_BURNOUT and auto-downshifts TimeWarpState from 100x to 1x") {
        SomaticStateComponent somatic{};
        MockPsychComponent psych{1.0f, 0.20f, 0.50f};
        BurnoutStateComponent burnout{};
        TimeWarpState warp{};
        SetTimeWarpMultiplier(warp, 100);

        StepRecreationAndBurnout(somatic, psych, burnout, camp, false, false, true, 0.0f, &warp);
        REQUIRE(burnout.is_burned_out);
        REQUIRE(warp.multiplier == 1);
        REQUIRE(warp.emergency_brake_triggered);
    }

    SECTION("Life-Critical Somatic Preemption: burned-out agent with critical thirst/hunger drinks and eats without dying") {
        SomaticStateComponent somatic{};
        somatic.hydration = 0.15f;       // < 0.20f life-critical threshold
        somatic.caloric_reserve = 450.0; // < 500.0 kcal life-critical threshold
        MockPsychComponent psych{1.0f, 0.20f, 0.50f};
        BurnoutStateComponent burnout{};
        burnout.is_burned_out = true;

        StepRecreationAndBurnout(somatic, psych, burnout, camp, false, false, true, 0.0f, nullptr);
        REQUIRE(burnout.life_critical_preemption_active);
        REQUIRE(somatic.hydration == Approx(0.55f));
        REQUIRE(somatic.caloric_reserve == Approx(1250.0));
        REQUIRE(camp.potable_water_liters == Approx(59.0));
        REQUIRE(camp.ration_meals_remaining == 11u);
    }

    SECTION("Anti-Soft-Lock Safeguards: zero-labor zone designations allowed during burnout & self-soothe exits burnout") {
        BurnoutStateComponent burnout{};
        burnout.is_burned_out = true;

        DesignatorToolSpec rec_zone_spec{};
        rec_zone_spec.name = "[Designate Recreation Zone]";
        rec_zone_spec.is_zero_labor_zone = true;

        DesignatorToolSpec cob_wall_spec{};
        cob_wall_spec.name = "[Build Wall: Cob]";
        cob_wall_spec.is_zero_labor_zone = false;

        REQUIRE(CanExecuteDesignatorAction(rec_zone_spec, burnout));
        REQUIRE_FALSE(CanExecuteDesignatorAction(cob_wall_spec, burnout));

        // Autonomous self-soothe when no Recreation Zone is reachable (-10% Stress/hr until < 0.40f)
        SomaticStateComponent somatic{};
        MockPsychComponent psych{1.0f, 0.20f, 0.50f};

        StepRecreationAndBurnout(somatic, psych, burnout, camp, false, false, false, 1.0f, nullptr);
        REQUIRE(burnout.autonomous_self_sooth_active);
        REQUIRE(psych.stress == Approx(0.90f));
        REQUIRE(burnout.is_burned_out);

        // Advance 5.5 more simulated hours of self-soothing -> stress drops to 0.35f (< 0.40f) and exits burnout
        StepRecreationAndBurnout(somatic, psych, burnout, camp, false, false, false, 5.5f, nullptr);
        REQUIRE(psych.stress == Approx(0.35f));
        REQUIRE_FALSE(burnout.is_burned_out);
        REQUIRE(CanExecuteDesignatorAction(cob_wall_spec, burnout));
    }
}

// ============================================================================
// STORY-10.3 / BUG-11.2: Canonical 28,800-Tick Diurnal Pacing Verification
// ============================================================================

TEST_CASE("STORY-10.3 (BUG-11.2): Canonical 1x Diurnal Pacing (28,800 Ticks/Day)", "[story-10.3][pacing]") {
    REQUIRE(TICKS_PER_SIM_HOUR == 1200ULL);
    REQUIRE(TICKS_PER_SIM_DAY == 28800ULL);
    REQUIRE(CANONICAL_DIURNAL_CYCLE_TICKS == 28800ULL);

    // 300 ticks (5 seconds at 60 Hz 1x) must advance only 1.04167% of a simulated day (15 sim minutes)
    const double day_fraction_300_ticks = 300.0 / static_cast<double>(CANONICAL_DIURNAL_CYCLE_TICKS);
    REQUIRE(day_fraction_300_ticks == Approx(0.0104166667).margin(1e-6));

    const float temp_t0 = CalculateDiurnalTemperature(0, CANONICAL_DIURNAL_CYCLE_TICKS);
    const float temp_t300 = CalculateDiurnalTemperature(300, CANONICAL_DIURNAL_CYCLE_TICKS);
    // Over 5 real seconds at 1x, temperature shifts gently (< 1.0 C), never jumping to noon (35.0 C)
    REQUIRE(std::abs(temp_t300 - temp_t0) < 1.0f);
}
