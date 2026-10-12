#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <flecs.h>
#include <glm/glm.hpp>

#include "core/fabrication.hpp"
#include "core/homeostasis.hpp"

using Catch::Matchers::WithinAbs;

namespace {

struct MockPsychologyComponent {
    float stress{0.25f};
    float mood{0.50f};
    float focus{1.0f};
};

} // namespace

// =============================================================================
// SECTION 1: Pure Mathematical Invariants of the Homeostasis Evaluation
// =============================================================================
TEST_CASE("Homeostasis Evaluation Mathematical Invariants", "[homeostasis][math][invariants]") {
    SECTION("Goldilocks Zone (15.0°C <= T <= 30.0°C) has unit multiplier and zero focus drain") {
        const float test_temps[] = {15.0f, 18.0f, 22.0f, 22.5f, 25.0f, 28.0f, 30.0f};
        for (float temp : test_temps) {
            auto eval = oasis::homeostasis::EvaluateHomeostasis(temp);
            REQUIRE_THAT(eval.burn_multiplier, WithinAbs(1.0, 1e-6));
            REQUIRE_THAT(eval.focus_drain_rate, WithinAbs(0.0f, 1e-6f));
            REQUIRE(eval.in_goldilocks == true);
            REQUIRE(eval.is_cold_stress == false);
            REQUIRE(eval.is_heat_stress == false);
        }
    }

    SECTION("Sub-15°C Cold Stress strictly accelerates burn and drains focus") {
        // Delta = 15.0 - T. Multiplier = 1.0 + 0.08 * Delta. Focus drain = 0.0020 * Delta
        // At 10.0°C: Delta = 5.0 -> Multiplier = 1.40, Drain = 0.010/s
        auto eval_10 = oasis::homeostasis::EvaluateHomeostasis(10.0f);
        REQUIRE_THAT(eval_10.burn_multiplier, WithinAbs(1.40, 1e-6));
        REQUIRE_THAT(eval_10.focus_drain_rate, WithinAbs(0.010f, 1e-6f));
        REQUIRE(eval_10.in_goldilocks == false);
        REQUIRE(eval_10.is_cold_stress == true);
        REQUIRE(eval_10.is_heat_stress == false);

        // At 5.0°C: Delta = 10.0 -> Multiplier = 1.80, Drain = 0.020/s
        auto eval_5 = oasis::homeostasis::EvaluateHomeostasis(5.0f);
        REQUIRE_THAT(eval_5.burn_multiplier, WithinAbs(1.80, 1e-6));
        REQUIRE_THAT(eval_5.focus_drain_rate, WithinAbs(0.020f, 1e-6f));
        REQUIRE(eval_5.is_cold_stress == true);

        // At 0.0°C: Delta = 15.0 -> Multiplier = 2.20, Drain = 0.030/s
        auto eval_0 = oasis::homeostasis::EvaluateHomeostasis(0.0f);
        REQUIRE_THAT(eval_0.burn_multiplier, WithinAbs(2.20, 1e-6));
        REQUIRE_THAT(eval_0.focus_drain_rate, WithinAbs(0.030f, 1e-6f));
        REQUIRE(eval_0.is_cold_stress == true);

        // At -10.0°C: Delta = 25.0 -> Multiplier = 3.00, Drain = 0.050/s
        auto eval_neg10 = oasis::homeostasis::EvaluateHomeostasis(-10.0f);
        REQUIRE_THAT(eval_neg10.burn_multiplier, WithinAbs(3.00, 1e-6));
        REQUIRE_THAT(eval_neg10.focus_drain_rate, WithinAbs(0.050f, 1e-6f));
        REQUIRE(eval_neg10.is_cold_stress == true);
    }

    SECTION("Cold burn multiplier is strictly monotonic with decreasing temperature") {
        float last_temp = 14.9f;
        double last_multiplier = oasis::homeostasis::EvaluateHomeostasis(last_temp).burn_multiplier;
        float last_drain = oasis::homeostasis::EvaluateHomeostasis(last_temp).focus_drain_rate;

        for (float temp = 14.0f; temp >= -20.0f; temp -= 1.0f) {
            auto eval = oasis::homeostasis::EvaluateHomeostasis(temp);
            REQUIRE(eval.burn_multiplier > last_multiplier);
            REQUIRE(eval.focus_drain_rate > last_drain);
            last_multiplier = eval.burn_multiplier;
            last_drain = eval.focus_drain_rate;
        }
    }

    SECTION("Super-30°C Heat Stress accelerates burn and drains focus") {
        // Delta = T - 30.0. Multiplier = 1.0 + 0.06 * Delta. Focus drain = 0.0015 * Delta
        // At 35.0°C: Delta = 5.0 -> Multiplier = 1.30, Drain = 0.0075/s
        auto eval_35 = oasis::homeostasis::EvaluateHomeostasis(35.0f);
        REQUIRE_THAT(eval_35.burn_multiplier, WithinAbs(1.30, 1e-6));
        REQUIRE_THAT(eval_35.focus_drain_rate, WithinAbs(0.0075f, 1e-6f));
        REQUIRE(eval_35.in_goldilocks == false);
        REQUIRE(eval_35.is_cold_stress == false);
        REQUIRE(eval_35.is_heat_stress == true);

        // At 40.0°C: Delta = 10.0 -> Multiplier = 1.60, Drain = 0.015/s
        auto eval_40 = oasis::homeostasis::EvaluateHomeostasis(40.0f);
        REQUIRE_THAT(eval_40.burn_multiplier, WithinAbs(1.60, 1e-6));
        REQUIRE_THAT(eval_40.focus_drain_rate, WithinAbs(0.015f, 1e-6f));
        REQUIRE(eval_40.is_heat_stress == true);
    }
}

// =============================================================================
// SECTION 2: Diurnal Cycle and Microclimate Shelter Verification
// =============================================================================
TEST_CASE("Diurnal Temperature Model and Shelter Microclimate", "[homeostasis][diurnal][microclimate]") {
    SECTION("Diurnal Cycle traverses exactly between 10.0°C and 35.0°C") {
        constexpr uint64_t cycle = 1200;

        // Tick 0: Dawn (phase 0.0) -> 22.5°C
        float temp_dawn = oasis::homeostasis::CalculateDiurnalTemperature(0, cycle);
        REQUIRE_THAT(temp_dawn, WithinAbs(22.5f, 1e-4f));

        // Tick 300: Noon (phase 0.25) -> 35.0°C (maximum peak)
        float temp_noon = oasis::homeostasis::CalculateDiurnalTemperature(300, cycle);
        REQUIRE_THAT(temp_noon, WithinAbs(35.0f, 1e-4f));

        // Tick 600: Dusk (phase 0.50) -> 22.5°C
        float temp_dusk = oasis::homeostasis::CalculateDiurnalTemperature(600, cycle);
        REQUIRE_THAT(temp_dusk, WithinAbs(22.5f, 1e-4f));

        // Tick 900: Midnight (phase 0.75) -> 10.0°C (minimum trough)
        float temp_night = oasis::homeostasis::CalculateDiurnalTemperature(900, cycle);
        REQUIRE_THAT(temp_night, WithinAbs(10.0f, 1e-4f));

        // Periodicity wraps
        float temp_cycle_end = oasis::homeostasis::CalculateDiurnalTemperature(cycle, cycle);
        REQUIRE_THAT(temp_cycle_end, WithinAbs(temp_dawn, 1e-4f));
    }

    SECTION("Residential shelter moderates ambient temperature towards comfort") {
        // Outdoors at midnight: position (0, 1, 0)
        glm::vec3 outdoor_pos{0.0f, 1.0f, 0.0f};
        REQUIRE(oasis::homeostasis::IsSheltered(outdoor_pos) == false);
        float outdoor_temp = oasis::homeostasis::ResolveVoxelAmbientTemperature(outdoor_pos, 900, 1200);
        REQUIRE_THAT(outdoor_temp, WithinAbs(10.0f, 1e-4f));

        // Indoors inside residence (absX=12 in [9..17], lotZ=4 in [2..10], y=2 in [1..3])
        glm::vec3 indoor_pos{12.0f, 2.0f, 4.0f};
        REQUIRE(oasis::homeostasis::IsSheltered(indoor_pos) == true);
        float indoor_temp = oasis::homeostasis::ResolveVoxelAmbientTemperature(indoor_pos, 900, 1200);
        // Base = 10.0°C, moderated = 22.0 + 0.3 * (10.0 - 22.0) = 22.0 - 3.6 = 18.4°C
        REQUIRE_THAT(indoor_temp, WithinAbs(18.4f, 1e-4f));
        // Inside shelter, temperature is protected in the Goldilocks zone!
        REQUIRE(indoor_temp >= oasis::homeostasis::GOLDILOCKS_MIN_TEMP_C);
        REQUIRE(indoor_temp <= oasis::homeostasis::GOLDILOCKS_MAX_TEMP_C);
    }
}

// =============================================================================
// SECTION 3: Flecs ECS Dual-Founder Comparative Metabolic Burn
// =============================================================================
TEST_CASE("Flecs ECS Comparative Homeostasis Burn", "[homeostasis][flecs][comparative]") {
    flecs::world ecs;

    // Register components
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<MockPsychologyComponent>();

    // Instantiate FounderWarm (22.5°C Goldilocks)
    oasis::fabrication::SomaticStateComponent warm_somatic{};
    warm_somatic.caloric_reserve = 2500.0;
    warm_somatic.ambient_temp_c = 22.5f;
    warm_somatic.focus = 1.0f;
    MockPsychologyComponent warm_psych{};
    warm_psych.focus = 1.0f;

    auto founder_warm = ecs.entity("FounderWarm")
        .set<oasis::fabrication::SomaticStateComponent>(warm_somatic)
        .set<MockPsychologyComponent>(warm_psych);

    // Instantiate FounderCold (5.0°C Sub-15°C Cold Stress)
    oasis::fabrication::SomaticStateComponent cold_somatic{};
    cold_somatic.caloric_reserve = 2500.0;
    cold_somatic.ambient_temp_c = 5.0f;
    cold_somatic.focus = 1.0f;
    MockPsychologyComponent cold_psych{};
    cold_psych.focus = 1.0f;

    auto founder_cold = ecs.entity("FounderCold")
        .set<oasis::fabrication::SomaticStateComponent>(cold_somatic)
        .set<MockPsychologyComponent>(cold_psych);

    // Register Homeostasis System
    ecs.system<oasis::fabrication::SomaticStateComponent, MockPsychologyComponent>("TestHomeostasisSystem")
        .kind(flecs::OnUpdate)
        .each([](oasis::fabrication::SomaticStateComponent& somatic, MockPsychologyComponent& psych) {
            constexpr float dt = 1.0f / 60.0f;
            oasis::homeostasis::StepHomeostasis(somatic, psych, somatic.ambient_temp_c, dt);
        });

    // Advance 600 ticks = 10.0 seconds simulated time
    constexpr int total_ticks = 600;
    constexpr float total_time = total_ticks * (1.0f / 60.0f); // 10.0 sec
    for (int i = 0; i < total_ticks; ++i) {
        ecs.progress();
    }

    const auto& post_warm_somatic = founder_warm.get<oasis::fabrication::SomaticStateComponent>();
    const auto& post_warm_psych = founder_warm.get<MockPsychologyComponent>();
    const auto& post_cold_somatic = founder_cold.get<oasis::fabrication::SomaticStateComponent>();
    const auto& post_cold_psych = founder_cold.get<MockPsychologyComponent>();

    // Baseline burn rate = 0.050 kcal/sec
    // Warm: Multiplier = 1.0. Burn in 10s = 0.050 * 1.0 * 10 = 0.500 kcal
    const double warm_burn = 2500.0 - post_warm_somatic.caloric_reserve;
    REQUIRE_THAT(warm_burn, WithinAbs(0.500, 1e-4));

    // Cold at 5.0°C: Delta = 10.0°C, Multiplier = 1.0 + 0.08 * 10 = 1.80
    // Cold: Burn in 10s = 0.050 * 1.80 * 10 = 0.900 kcal
    const double cold_burn = 2500.0 - post_cold_somatic.caloric_reserve;
    REQUIRE_THAT(cold_burn, WithinAbs(0.900, 1e-4));

    // Exact acceleration ratio is mathematically M = 1.80
    const double ratio = cold_burn / warm_burn;
    REQUIRE_THAT(ratio, WithinAbs(1.80, 1e-4));

    // Warm focus remains intact at 1.0
    REQUIRE_THAT(post_warm_psych.focus, WithinAbs(1.0f, 1e-4f));
    REQUIRE_THAT(post_warm_somatic.focus, WithinAbs(1.0f, 1e-4f));

    // Cold focus drained at 0.020 / sec * 10s = 0.20
    REQUIRE_THAT(post_cold_psych.focus, WithinAbs(0.80f, 1e-4f));
    REQUIRE_THAT(post_cold_somatic.focus, WithinAbs(0.80f, 1e-4f));

    REQUIRE(post_cold_somatic.caloric_reserve < post_warm_somatic.caloric_reserve);
    REQUIRE(post_cold_psych.focus < post_warm_psych.focus);
}

// =============================================================================
// SECTION 4: Exhaustion Guard and Boundary Clamping
// =============================================================================
TEST_CASE("Homeostasis Exhaustion Guard and Boundary Clamping", "[homeostasis][bounds][exhaustion]") {
    SECTION("Caloric exhaustion triggers when reserves drop to or below 500 kcal") {
        oasis::fabrication::SomaticStateComponent somatic{};
        somatic.caloric_reserve = 500.5;
        somatic.is_exhausted = false;
        MockPsychologyComponent psych{};
        psych.focus = 1.0f;

        // In 5.0°C cold, burn is 0.090 kcal/s. Over 10 seconds, burn is 0.90 kcal -> drops to 499.6 kcal
        oasis::homeostasis::StepHomeostasis(somatic, psych, 5.0f, 10.0f);

        REQUIRE(somatic.caloric_reserve < 500.0);
        REQUIRE(somatic.is_exhausted == true);
    }

    SECTION("Focus exhaustion triggers when focus drops to or below 0.05") {
        oasis::fabrication::SomaticStateComponent somatic{};
        somatic.caloric_reserve = 2500.0;
        somatic.focus = 0.06f;
        somatic.is_exhausted = false;
        MockPsychologyComponent psych{};
        psych.focus = 0.06f;

        // In severe cold (-10.0°C), drain rate is 0.050/s. Over 0.5 seconds, drain is 0.025 -> drops to 0.035
        oasis::homeostasis::StepHomeostasis(somatic, psych, -10.0f, 0.5f);

        REQUIRE(psych.focus <= 0.05f);
        REQUIRE(somatic.is_exhausted == true);
    }

    SECTION("Reserves clamp cleanly at 0.0 kcal without negative underflow") {
        oasis::fabrication::SomaticStateComponent somatic{};
        somatic.caloric_reserve = 0.01;
        MockPsychologyComponent psych{};

        // Stepping 100 seconds in freezing cold
        oasis::homeostasis::StepHomeostasis(somatic, psych, -50.0f, 100.0f);

        REQUIRE(somatic.caloric_reserve >= 0.0);
        REQUIRE_THAT(somatic.caloric_reserve, WithinAbs(0.0, 1e-9));
        REQUIRE(psych.focus >= 0.0f);
        REQUIRE_THAT(psych.focus, WithinAbs(0.0f, 1e-9f));
    }

    SECTION("Extreme temperatures produce finite numbers with no NaNs or infinities") {
        const float extreme_temps[] = {-273.15f, -100.0f, 0.0f, 100.0f, 500.0f};
        for (float temp : extreme_temps) {
            auto eval = oasis::homeostasis::EvaluateHomeostasis(temp);
            REQUIRE(std::isfinite(eval.burn_multiplier));
            REQUIRE(std::isfinite(eval.focus_drain_rate));
            REQUIRE(eval.burn_multiplier > 0.0);
            REQUIRE(eval.focus_drain_rate >= 0.0f);

            oasis::fabrication::SomaticStateComponent s{};
            MockPsychologyComponent p{};
            oasis::homeostasis::StepHomeostasis(s, p, temp, 1.0f);

            REQUIRE(std::isfinite(s.caloric_reserve));
            REQUIRE(std::isfinite(p.focus));
            REQUIRE(s.caloric_reserve >= 0.0);
            REQUIRE(p.focus >= 0.0f);
        }
    }
}
