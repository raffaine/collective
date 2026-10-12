#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <vector>
#include <limits>
#include <flecs.h>
#include <glm/glm.hpp>

#include "core/fabrication.hpp"
#include "core/homeostasis.hpp"

// ANSI Color Codes for reporting
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

struct MockPsychologyComponent {
    float stress{0.25f};
    float mood{0.50f};
    float focus{1.0f};
};

static int g_total_assertions = 0;
static int g_failed_assertions = 0;

#define ADVERSARIAL_CHECK(cond, msg) do { \
    g_total_assertions++; \
    if (!(cond)) { \
        g_failed_assertions++; \
        std::cerr << COLOR_RED << "  [FAIL] " << COLOR_RESET << msg \
                  << " (Line " << __LINE__ << "): " << #cond << std::endl; \
    } \
} while(0)

#define ADVERSARIAL_REQUIRE(cond, msg) do { \
    ADVERSARIAL_CHECK(cond, msg); \
    if (!(cond)) return false; \
} while(0)

// =============================================================================
// CHALLENGE 1: Boundary Temperatures & Goldilocks Precision
// =============================================================================
bool Challenge1_BoundaryTemperatures() {
    std::cout << COLOR_BOLD << "\n--- [CHALLENGE 1] Boundary Temperatures & Goldilocks Precision ---" << COLOR_RESET << std::endl;

    // Test 1.1: Exact Goldilocks Boundaries
    // 15.0°C and 30.0°C should be exactly in Goldilocks
    auto eval_min = oasis::homeostasis::EvaluateHomeostasis(15.000f);
    ADVERSARIAL_CHECK(eval_min.in_goldilocks == true, "15.000°C must be in Goldilocks");
    ADVERSARIAL_CHECK(std::abs(eval_min.burn_multiplier - 1.0) < 1e-6, "15.000°C multiplier must be 1.0");
    ADVERSARIAL_CHECK(eval_min.focus_drain_rate == 0.0f, "15.000°C focus drain must be 0.0");
    ADVERSARIAL_CHECK(eval_min.is_cold_stress == false, "15.000°C must not be cold stress");

    auto eval_max = oasis::homeostasis::EvaluateHomeostasis(30.000f);
    ADVERSARIAL_CHECK(eval_max.in_goldilocks == true, "30.000°C must be in Goldilocks");
    ADVERSARIAL_CHECK(std::abs(eval_max.burn_multiplier - 1.0) < 1e-6, "30.000°C multiplier must be 1.0");
    ADVERSARIAL_CHECK(eval_max.focus_drain_rate == 0.0f, "30.000°C focus drain must be 0.0");
    ADVERSARIAL_CHECK(eval_max.is_heat_stress == false, "30.000°C must not be heat stress");

    // Test 1.2: Boundary Epsilon Sub-15°C (14.999°C vs 15.000°C)
    auto eval_14_999 = oasis::homeostasis::EvaluateHomeostasis(14.999f);
    ADVERSARIAL_CHECK(eval_14_999.in_goldilocks == false, "14.999°C must trigger cold stress");
    ADVERSARIAL_CHECK(eval_14_999.is_cold_stress == true, "14.999°C is_cold_stress must be true");
    ADVERSARIAL_CHECK(eval_14_999.burn_multiplier > 1.0, "14.999°C multiplier > 1.0");
    ADVERSARIAL_CHECK(eval_14_999.focus_drain_rate > 0.0f, "14.999°C focus drain > 0.0");

    // Jump continuity check across 15.0°C
    double jump_mult_15 = std::abs(eval_14_999.burn_multiplier - eval_min.burn_multiplier);
    float jump_drain_15 = std::abs(eval_14_999.focus_drain_rate - eval_min.focus_drain_rate);
    ADVERSARIAL_CHECK(jump_mult_15 < 1e-3, "Multiplier jump across 15°C must be infinitesimal (< 0.001)");
    ADVERSARIAL_CHECK(jump_drain_15 < 1e-4f, "Focus drain jump across 15°C must be infinitesimal (< 0.0001)");

    // Test 1.3: Boundary Epsilon Super-30°C (30.000°C vs 30.001°C)
    auto eval_30_001 = oasis::homeostasis::EvaluateHomeostasis(30.001f);
    ADVERSARIAL_CHECK(eval_30_001.in_goldilocks == false, "30.001°C must trigger heat stress");
    ADVERSARIAL_CHECK(eval_30_001.is_heat_stress == true, "30.001°C is_heat_stress must be true");
    ADVERSARIAL_CHECK(eval_30_001.burn_multiplier > 1.0, "30.001°C multiplier > 1.0");
    ADVERSARIAL_CHECK(eval_30_001.focus_drain_rate > 0.0f, "30.001°C focus drain > 0.0");

    double jump_mult_30 = std::abs(eval_30_001.burn_multiplier - eval_max.burn_multiplier);
    float jump_drain_30 = std::abs(eval_30_001.focus_drain_rate - eval_max.focus_drain_rate);
    ADVERSARIAL_CHECK(jump_mult_30 < 1e-3, "Multiplier jump across 30°C must be infinitesimal (< 0.001)");
    ADVERSARIAL_CHECK(jump_drain_30 < 1e-4f, "Focus drain jump across 30°C must be infinitesimal (< 0.0001)");

    // Test 1.4: Dense scan across entire Goldilocks range [15.0 .. 30.0] with 0.01 step
    for (int i = 1500; i <= 3000; ++i) {
        float t = i / 100.0f;
        auto ev = oasis::homeostasis::EvaluateHomeostasis(t);
        ADVERSARIAL_CHECK(ev.in_goldilocks == true, "Dense scan in Goldilocks");
        ADVERSARIAL_CHECK(ev.burn_multiplier == 1.0, "Multiplier strictly 1.0");
        ADVERSARIAL_CHECK(ev.focus_drain_rate == 0.0f, "Focus drain strictly 0.0");
    }

    std::cout << COLOR_GREEN << "  [PASS] Challenge 1: Boundary temperatures and C0 continuity verified." << COLOR_RESET << std::endl;
    return true;
}

// =============================================================================
// CHALLENGE 2: Extreme Freezing and Scorching Temperatures
// =============================================================================
bool Challenge2_ExtremeTemperatures() {
    std::cout << COLOR_BOLD << "\n--- [CHALLENGE 2] Extreme Freezing (-50°C, -273.15°C) & Scorching (+60°C, +100°C) ---" << COLOR_RESET << std::endl;

    // Test 2.1: Freezing at -50.0°C
    // Delta = 15.0 - (-50.0) = 65.0. Multiplier = 1.0 + 0.08 * 65 = 6.20. Drain = 0.0020 * 65 = 0.130/s
    auto eval_neg50 = oasis::homeostasis::EvaluateHomeostasis(-50.0f);
    ADVERSARIAL_CHECK(std::abs(eval_neg50.burn_multiplier - 6.20) < 1e-5, "-50°C burn multiplier = 6.20");
    ADVERSARIAL_CHECK(std::abs(eval_neg50.focus_drain_rate - 0.130f) < 1e-5f, "-50°C focus drain rate = 0.130/s");
    ADVERSARIAL_CHECK(eval_neg50.is_cold_stress == true, "-50°C cold stress flag");

    // Test 2.2: Absolute Zero at -273.15°C
    // Delta = 15.0 - (-273.15) = 288.15. Multiplier = 1.0 + 0.08 * 288.15 = 24.052. Drain = 0.0020 * 288.15 = 0.5763/s
    auto eval_abszero = oasis::homeostasis::EvaluateHomeostasis(-273.15f);
    ADVERSARIAL_CHECK(std::abs(eval_abszero.burn_multiplier - 24.052) < 1e-3, "-273.15°C burn multiplier = 24.052");
    ADVERSARIAL_CHECK(std::abs(eval_abszero.focus_drain_rate - 0.5763f) < 1e-3f, "-273.15°C focus drain rate = 0.5763/s");
    ADVERSARIAL_CHECK(std::isfinite(eval_abszero.burn_multiplier), "Multiplier finite at absolute zero");

    // Test 2.3: Scorching at +60.0°C
    // Delta = 60.0 - 30.0 = 30.0. Multiplier = 1.0 + 0.06 * 30 = 2.80. Drain = 0.0015 * 30 = 0.045/s
    auto eval_pos60 = oasis::homeostasis::EvaluateHomeostasis(60.0f);
    ADVERSARIAL_CHECK(std::abs(eval_pos60.burn_multiplier - 2.80) < 1e-5, "+60°C burn multiplier = 2.80");
    ADVERSARIAL_CHECK(std::abs(eval_pos60.focus_drain_rate - 0.045f) < 1e-5f, "+60°C focus drain rate = 0.045/s");
    ADVERSARIAL_CHECK(eval_pos60.is_heat_stress == true, "+60°C heat stress flag");

    // Test 2.4: Scorching at +100.0°C (Boiling water)
    // Delta = 100.0 - 30.0 = 70.0. Multiplier = 1.0 + 0.06 * 70 = 5.20. Drain = 0.0015 * 70 = 0.105/s
    auto eval_pos100 = oasis::homeostasis::EvaluateHomeostasis(100.0f);
    ADVERSARIAL_CHECK(std::abs(eval_pos100.burn_multiplier - 5.20) < 1e-5, "+100°C burn multiplier = 5.20");
    ADVERSARIAL_CHECK(std::abs(eval_pos100.focus_drain_rate - 0.105f) < 1e-5f, "+100°C focus drain rate = 0.105/s");

    // Test 2.5: Monotonicity across wide range [-100°C .. 100°C]
    // Cold monotonicity: T decreasing -> Multiplier strictly increasing
    double prev_cold_m = 1.0;
    for (float t = 14.0f; t >= -100.0f; t -= 5.0f) {
        auto ev = oasis::homeostasis::EvaluateHomeostasis(t);
        ADVERSARIAL_CHECK(ev.burn_multiplier > prev_cold_m, "Cold multiplier strictly increasing as T drops");
        prev_cold_m = ev.burn_multiplier;
    }
    // Heat monotonicity: T increasing -> Multiplier strictly increasing
    double prev_hot_m = 1.0;
    for (float t = 31.0f; t <= 100.0f; t += 5.0f) {
        auto ev = oasis::homeostasis::EvaluateHomeostasis(t);
        ADVERSARIAL_CHECK(ev.burn_multiplier > prev_hot_m, "Heat multiplier strictly increasing as T rises");
        prev_hot_m = ev.burn_multiplier;
    }

    std::cout << COLOR_GREEN << "  [PASS] Challenge 2: Extreme temperatures (-50°C, -273.15°C, +60°C, +100°C) verified." << COLOR_RESET << std::endl;
    return true;
}

// =============================================================================
// CHALLENGE 3: Continuous Long Simulation Ticks (1,000,000 Ticks)
// =============================================================================
bool Challenge3_ContinuousLongTicks() {
    std::cout << COLOR_BOLD << "\n--- [CHALLENGE 3] Continuous Long Simulation (1,000,000 Ticks & Diurnal Continuity) ---" << COLOR_RESET << std::endl;

    constexpr uint64_t total_ticks = 1000000; // 833.3 diurnal cycles
    constexpr uint64_t period = 1200;

    float min_temp = 999.0f;
    float max_temp = -999.0f;
    double sum_temp = 0.0;

    for (uint64_t tick = 0; tick < total_ticks; ++tick) {
        float temp = oasis::homeostasis::CalculateDiurnalTemperature(tick, period);

        if (temp < min_temp) min_temp = temp;
        if (temp > max_temp) max_temp = temp;
        sum_temp += temp;

        // Invariant: temp must strictly stay within [10.0°C - epsilon, 35.0°C + epsilon]
        if (temp < 10.0f - 1e-4f || temp > 35.0f + 1e-4f) {
            ADVERSARIAL_REQUIRE(false, "Diurnal temperature out of bounds [10°C, 35°C]");
        }
    }

    double mean_temp = sum_temp / static_cast<double>(total_ticks);
    ADVERSARIAL_CHECK(std::abs(min_temp - 10.0f) < 1e-3f, "Minimum diurnal temp must reach 10.0°C");
    ADVERSARIAL_CHECK(std::abs(max_temp - 35.0f) < 1e-3f, "Maximum diurnal temp must reach 35.0°C");
    ADVERSARIAL_CHECK(std::abs(mean_temp - 22.5) < 0.01, "Mean diurnal temp over 1M ticks must equal 22.5°C");

    // Test cycle period == 0 guard
    float fallback_temp = oasis::homeostasis::CalculateDiurnalTemperature(300, 0);
    ADVERSARIAL_CHECK(std::abs(fallback_temp - 35.0f) < 1e-4f, "cycle_period == 0 safely falls back to DIURNAL_CYCLE_TICKS");

    std::cout << COLOR_GREEN << "  [PASS] Challenge 3: 1,000,000 ticks diurnal continuity, bounds [10°C, 35°C], mean 22.5°C verified." << COLOR_RESET << std::endl;
    return true;
}

// =============================================================================
// CHALLENGE 4: Caloric Clamping to 0.0 kcal & Underflow Resistance
// =============================================================================
bool Challenge4_CaloricClamping() {
    std::cout << COLOR_BOLD << "\n--- [CHALLENGE 4] Caloric Clamping to 0.0 kcal & Underflow Resistance ---" << COLOR_RESET << std::endl;

    oasis::fabrication::SomaticStateComponent somatic{};
    somatic.caloric_reserve = 100.0; // Start low
    somatic.is_exhausted = false;
    MockPsychologyComponent psych{};
    psych.focus = 1.0f;

    // Run at -50°C cold (burn rate = 0.05 * 6.20 = 0.31 kcal/s).
    // In ~323 seconds, calories reach 0.0.
    constexpr float dt = 1.0f / 60.0f;
    bool reached_zero = false;
    uint64_t zero_tick = 0;

    for (uint64_t tick = 0; tick < 100000; ++tick) {
        oasis::homeostasis::StepHomeostasis(somatic, psych, -50.0f, dt);

        ADVERSARIAL_CHECK(somatic.caloric_reserve >= 0.0, "Caloric reserve must NEVER be negative");
        ADVERSARIAL_CHECK(!std::isnan(somatic.caloric_reserve), "Caloric reserve must not be NaN");

        if (somatic.caloric_reserve == 0.0 && !reached_zero) {
            reached_zero = true;
            zero_tick = tick;
        }

        if (reached_zero) {
            ADVERSARIAL_CHECK(somatic.caloric_reserve == 0.0, "Caloric reserve must remain exactly 0.0 once depleted");
            ADVERSARIAL_CHECK(somatic.is_exhausted == true, "Must be flagged exhausted");
        }
    }

    ADVERSARIAL_CHECK(reached_zero == true, "Must reach 0.0 kcal under sustained extreme cold");
    std::cout << "  [INFO] Reached 0.0 kcal at tick " << zero_tick << ", maintained 0.0 for " << (100000 - zero_tick) << " subsequent ticks." << std::endl;

    // Adversarial corrupt initial state: negative starting calories
    oasis::fabrication::SomaticStateComponent corrupt_somatic{};
    corrupt_somatic.caloric_reserve = -500.0;
    MockPsychologyComponent corrupt_psych{};
    oasis::homeostasis::StepHomeostasis(corrupt_somatic, corrupt_psych, 22.5f, 1.0f);
    ADVERSARIAL_CHECK(corrupt_somatic.caloric_reserve >= 0.0, "Negative corrupt reserve clamped to >= 0.0");
    ADVERSARIAL_CHECK(corrupt_somatic.caloric_reserve == 0.0, "Negative corrupt reserve clamped to exactly 0.0");

    std::cout << COLOR_GREEN << "  [PASS] Challenge 4: Caloric clamping to strictly 0.0 kcal verified without underflow." << COLOR_RESET << std::endl;
    return true;
}

// =============================================================================
// CHALLENGE 5: Focus Drain Dynamics & Bounds Clamping [0.0, 1.0]
// =============================================================================
bool Challenge5_FocusDrainClamping() {
    std::cout << COLOR_BOLD << "\n--- [CHALLENGE 5] Focus Drain Dynamics & Clamping [0.0, 1.0] ---" << COLOR_RESET << std::endl;

    oasis::fabrication::SomaticStateComponent somatic{};
    somatic.caloric_reserve = 50000.0; // Plenty of calories so caloric exhaustion doesn't trigger first
    somatic.is_exhausted = false;
    MockPsychologyComponent psych{};
    psych.focus = 1.0f;
    somatic.focus = 1.0f;

    // At -10.0°C cold, Delta = 25.0°C. Focus drain rate = 0.0020 * 25 = 0.050 / s.
    // In 20.0 seconds (1200 ticks at 60 Hz), focus drains from 1.0 to 0.0.
    constexpr float dt = 1.0f / 60.0f;
    bool hit_exhaustion_threshold = false;
    bool reached_zero = false;

    float prev_focus = 1.0f;
    for (uint64_t tick = 0; tick < 2400; ++tick) {
        oasis::homeostasis::StepHomeostasis(somatic, psych, -10.0f, dt);

        ADVERSARIAL_CHECK(psych.focus <= prev_focus, "Focus must be monotonically non-increasing under cold stress");
        ADVERSARIAL_CHECK(psych.focus >= 0.0f, "Focus must never be negative");
        ADVERSARIAL_CHECK(somatic.focus == psych.focus, "Somatic focus must stay synchronized with psychology focus");

        if (psych.focus <= 0.05f && !hit_exhaustion_threshold) {
            hit_exhaustion_threshold = true;
            ADVERSARIAL_CHECK(somatic.is_exhausted == true, "Exhaustion must trigger when focus <= 0.05");
        }

        if (psych.focus == 0.0f && !reached_zero) {
            reached_zero = true;
            // Drained to 0 in approximately 20 seconds = 1200 ticks
            ADVERSARIAL_CHECK(tick >= 1190 && tick <= 1210, "Focus must reach 0.0 at exactly ~1200 ticks (20s)");
        }

        if (reached_zero) {
            ADVERSARIAL_CHECK(psych.focus == 0.0f, "Focus must clamp at exactly 0.0f without underflow");
            ADVERSARIAL_CHECK(somatic.focus == 0.0f, "Somatic focus must clamp at exactly 0.0f");
        }
        prev_focus = psych.focus;
    }

    ADVERSARIAL_CHECK(hit_exhaustion_threshold == true, "Hit focus exhaustion threshold (<= 0.05)");
    ADVERSARIAL_CHECK(reached_zero == true, "Focus drained and clamped cleanly at 0.0");

    std::cout << COLOR_GREEN << "  [PASS] Challenge 5: Focus drain dynamics and clean 0.0f clamping verified." << COLOR_RESET << std::endl;
    return true;
}

// =============================================================================
// CHALLENGE 6: Microclimate Shelter Spatial Invariance & Moderation
// =============================================================================
bool Challenge6_MicroclimateShelter() {
    std::cout << COLOR_BOLD << "\n--- [CHALLENGE 6] Microclimate Shelter Resolution & Thermal Moderation ---" << COLOR_RESET << std::endl;

    // Shelter spatial bounds:
    // absX in [9..17], lotZ in [2..10], vy in [1..3]

    // Test 6.1: Positive and Negative X symmetry
    glm::vec3 pos_east{12.0f, 2.0f, 4.0f};
    glm::vec3 pos_west{-12.0f, 2.0f, 4.0f};
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered(pos_east) == true, "East residence is sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered(pos_west) == true, "West residence is sheltered (X symmetry)");

    // Test 6.2: X boundary conditions
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({9.0f, 2.0f, 4.0f}) == true, "x = 9.0 (min bound) sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({17.99f, 2.0f, 4.0f}) == true, "x = 17.99 (max bound) sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({8.99f, 2.0f, 4.0f}) == false, "x = 8.99 (outside) not sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({18.01f, 2.0f, 4.0f}) == false, "x = 18.01 (outside) not sheltered");

    // Test 6.3: Y height bounds (vy in [1..3])
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 1.0f, 4.0f}) == true, "y = 1.0 (ground floor) sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 3.99f, 4.0f}) == true, "y = 3.99 (top floor) sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 0.99f, 4.0f}) == false, "y = 0.99 (underground) not sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 4.01f, 4.0f}) == false, "y = 4.01 (roof/sky) not sheltered");

    // Test 6.4: Z chunk periodicity (lotZ = (z % 16 + 16) % 16 in [2..10])
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 2.0f, 2.0f}) == true, "z = 2.0 sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 2.0f, 10.99f}) == true, "z = 10.99 sheltered");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 2.0f, 1.99f}) == false, "z = 1.99 outside");
    ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 2.0f, 11.01f}) == false, "z = 11.01 outside");

    // Across multiple chunk repeats along Z: z + 16*k
    for (int k = -5; k <= 5; ++k) {
        float z = 5.0f + 16.0f * k;
        ADVERSARIAL_CHECK(oasis::homeostasis::IsSheltered({12.0f, 2.0f, z}) == true, "Modular Z repeat sheltered");
    }

    // Test 6.5: Complete 24h diurnal protection inside shelter
    // Sheltered temp: 22.0 + 0.3 * (base - 22.0)
    // Base ranges [10.0 .. 35.0].
    // At 10.0°C -> 22.0 + 0.3 * (-12.0) = 18.4°C
    // At 35.0°C -> 22.0 + 0.3 * (13.0)  = 25.9°C
    for (uint64_t tick = 0; tick < 1200; ++tick) {
        float indoor_temp = oasis::homeostasis::ResolveVoxelAmbientTemperature(pos_east, tick, 1200);
        ADVERSARIAL_CHECK(indoor_temp >= 18.39f && indoor_temp <= 25.91f, "Indoor temp strictly bounded in [18.4, 25.9]");
        ADVERSARIAL_CHECK(indoor_temp >= oasis::homeostasis::GOLDILOCKS_MIN_TEMP_C, "Indoor temp strictly >= 15°C Goldilocks min");
        ADVERSARIAL_CHECK(indoor_temp <= oasis::homeostasis::GOLDILOCKS_MAX_TEMP_C, "Indoor temp strictly <= 30°C Goldilocks max");

        auto eval = oasis::homeostasis::EvaluateHomeostasis(indoor_temp);
        ADVERSARIAL_CHECK(eval.in_goldilocks == true, "Indoor Founder is NEVER stressed across entire 24h cycle");
        ADVERSARIAL_CHECK(eval.burn_multiplier == 1.0, "Indoor Founder burn multiplier strictly 1.0");
        ADVERSARIAL_CHECK(eval.focus_drain_rate == 0.0f, "Indoor Founder focus drain strictly 0.0");
    }

    std::cout << COLOR_GREEN << "  [PASS] Challenge 6: Microclimate shelter spatial bounds and 100% Goldilocks protection verified." << COLOR_RESET << std::endl;
    return true;
}

// =============================================================================
// CHALLENGE 7: Flecs ECS System Multi-Entity Isolation & System Integrity
// =============================================================================
bool Challenge7_FlecsECSIsolation() {
    std::cout << COLOR_BOLD << "\n--- [CHALLENGE 7] Flecs ECS Multi-Entity Isolation & Execution Integrity ---" << COLOR_RESET << std::endl;

    flecs::world ecs;

    ecs.component<oasis::fabrication::SpatialPositionComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<MockPsychologyComponent>();

    // Entity 1: Founder (in freezing outdoor cold, x=0, y=1, z=0, tick=900 -> 10.0°C)
    oasis::fabrication::SpatialPositionComponent founder_pos{{0.0f, 1.0f, 0.0f}};
    oasis::fabrication::SomaticStateComponent founder_somatic{};
    founder_somatic.caloric_reserve = 2500.0;
    founder_somatic.focus = 1.0f;
    MockPsychologyComponent founder_psych{};
    founder_psych.focus = 1.0f;

    auto founder = ecs.entity("Founder")
        .set<oasis::fabrication::SpatialPositionComponent>(founder_pos)
        .set<oasis::fabrication::SomaticStateComponent>(founder_somatic)
        .set<MockPsychologyComponent>(founder_psych);

    // Entity 2: Non-Founder Settler NPC (same components, but named SettlerNPC)
    oasis::fabrication::SpatialPositionComponent npc_pos{{0.0f, 1.0f, 0.0f}};
    oasis::fabrication::SomaticStateComponent npc_somatic{};
    npc_somatic.caloric_reserve = 2500.0;
    npc_somatic.focus = 1.0f;
    MockPsychologyComponent npc_psych{};
    npc_psych.focus = 1.0f;

    auto settler_npc = ecs.entity("SettlerNPC")
        .set<oasis::fabrication::SpatialPositionComponent>(npc_pos)
        .set<oasis::fabrication::SomaticStateComponent>(npc_somatic)
        .set<MockPsychologyComponent>(npc_psych);

    // Register FounderHomeostasisSystem matching main.cpp logic
    static uint64_t sim_tick = 900; // Midnight: outdoor temp = 10.0°C
    ecs.system<oasis::fabrication::SpatialPositionComponent,
               oasis::fabrication::SomaticStateComponent,
               MockPsychologyComponent>("TestFounderHomeostasisSystem")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e,
                 oasis::fabrication::SpatialPositionComponent& pos,
                 oasis::fabrication::SomaticStateComponent& somatic,
                 MockPsychologyComponent& psych) {
            // Must strictly filter to Founder entity only!
            if (e != e.world().entity("Founder")) return;
            constexpr float dt = 1.0f / 60.0f;
            float amb_temp = oasis::homeostasis::ResolveVoxelAmbientTemperature(pos.pos, sim_tick);
            oasis::homeostasis::StepHomeostasis(somatic, psych, amb_temp, dt);
        });

    // Run for 600 ticks (10 seconds)
    for (int i = 0; i < 600; ++i) {
        ecs.progress();
    }

    const auto& f_som = founder.get<oasis::fabrication::SomaticStateComponent>();
    const auto& f_psy = founder.get<MockPsychologyComponent>();
    const auto& n_som = settler_npc.get<oasis::fabrication::SomaticStateComponent>();
    const auto& n_psy = settler_npc.get<MockPsychologyComponent>();

    // Founder was in 10°C cold:
    // Delta = 5.0, Multiplier = 1.40. Burn = 0.05 * 1.40 * 10 = 0.70 kcal
    ADVERSARIAL_CHECK(std::abs((2500.0 - f_som.caloric_reserve) - 0.70) < 1e-3, "Founder burned 0.70 kcal at 10°C");
    // Focus drain = 0.010 / s * 10 = 0.10 -> focus = 0.90
    ADVERSARIAL_CHECK(std::abs(f_psy.focus - 0.90f) < 1e-3f, "Founder focus drained to 0.90");

    // SettlerNPC was NOT targeted: reserves and focus MUST BE UNTOUCHED!
    ADVERSARIAL_CHECK(n_som.caloric_reserve == 2500.0, "Settler NPC caloric reserve completely untouched");
    ADVERSARIAL_CHECK(n_psy.focus == 1.0f, "Settler NPC focus completely untouched");

    std::cout << COLOR_GREEN << "  [PASS] Challenge 7: Flecs ECS Founder entity filtering and component safety verified." << COLOR_RESET << std::endl;
    return true;
}

// =============================================================================
// CHALLENGE 8: IEEE 754 Floating-Point Corruption & Denormals
// =============================================================================
bool Challenge8_FloatingPointSanitization() {
    std::cout << COLOR_BOLD << "\n--- [CHALLENGE 8] IEEE 754 Edge Cases (NaN, +/-Inf, Subnormals) ---" << COLOR_RESET << std::endl;

    // Test 8.1: NaN ambient temperature
    float nan_temp = std::numeric_limits<float>::quiet_NaN();
    auto eval_nan = oasis::homeostasis::EvaluateHomeostasis(nan_temp);
    // When NaN is compared with < 15 or > 30, both are false, falls back to Goldilocks
    ADVERSARIAL_CHECK(eval_nan.in_goldilocks == true, "NaN falls back safely to Goldilocks");
    ADVERSARIAL_CHECK(eval_nan.burn_multiplier == 1.0, "NaN multiplier is 1.0");
    ADVERSARIAL_CHECK(eval_nan.focus_drain_rate == 0.0f, "NaN focus drain is 0.0");

    // Test 8.2: +Infinity ambient temperature
    float pos_inf = std::numeric_limits<float>::infinity();
    auto eval_pos_inf = oasis::homeostasis::EvaluateHomeostasis(pos_inf);
    ADVERSARIAL_CHECK(eval_pos_inf.is_heat_stress == true, "+Inf triggers heat stress");
    ADVERSARIAL_CHECK(std::isinf(eval_pos_inf.burn_multiplier), "+Inf multiplier is Inf");

    oasis::fabrication::SomaticStateComponent inf_somatic{};
    inf_somatic.caloric_reserve = 2500.0;
    MockPsychologyComponent inf_psych{};
    inf_psych.focus = 1.0f;
    oasis::homeostasis::StepHomeostasis(inf_somatic, inf_psych, pos_inf, 1.0f);
    // std::max(0.0, 2500 - inf) should clamp cleanly to 0.0!
    ADVERSARIAL_CHECK(inf_somatic.caloric_reserve == 0.0, "+Inf step clamps caloric reserve safely to 0.0");
    ADVERSARIAL_CHECK(inf_psych.focus == 0.0f, "+Inf step clamps focus safely to 0.0f");
    ADVERSARIAL_CHECK(inf_somatic.is_exhausted == true, "+Inf flags exhausted");

    // Test 8.3: -Infinity ambient temperature
    float neg_inf = -std::numeric_limits<float>::infinity();
    auto eval_neg_inf = oasis::homeostasis::EvaluateHomeostasis(neg_inf);
    ADVERSARIAL_CHECK(eval_neg_inf.is_cold_stress == true, "-Inf triggers cold stress");
    oasis::fabrication::SomaticStateComponent neg_inf_somatic{};
    neg_inf_somatic.caloric_reserve = 2500.0;
    MockPsychologyComponent neg_inf_psych{};
    neg_inf_psych.focus = 1.0f;
    oasis::homeostasis::StepHomeostasis(neg_inf_somatic, neg_inf_psych, neg_inf, 1.0f);
    ADVERSARIAL_CHECK(neg_inf_somatic.caloric_reserve == 0.0, "-Inf step clamps caloric reserve safely to 0.0");
    ADVERSARIAL_CHECK(neg_inf_psych.focus == 0.0f, "-Inf step clamps focus safely to 0.0f");

    // Test 8.4: Subnormal (denormal) temperature: 1e-40f
    float subnorm = 1e-40f;
    auto eval_subnorm = oasis::homeostasis::EvaluateHomeostasis(subnorm);
    ADVERSARIAL_CHECK(std::isfinite(eval_subnorm.burn_multiplier), "Subnormal multiplier is finite");
    ADVERSARIAL_CHECK(std::isfinite(eval_subnorm.focus_drain_rate), "Subnormal focus drain is finite");

    std::cout << COLOR_GREEN << "  [PASS] Challenge 8: IEEE 754 edge-cases (NaN, +/-Inf, Subnormals) sanitized safely." << COLOR_RESET << std::endl;
    return true;
}

int main() {
    std::cout << COLOR_BOLD << "================================================================================" << COLOR_RESET << std::endl;
    std::cout << COLOR_BOLD << "=== EMPIRICAL CHALLENGER 1: HOMEOSTASIS LOOP ADVERSARIAL STRESS TEST SUITE ===" << COLOR_RESET << std::endl;
    std::cout << COLOR_BOLD << "================================================================================" << COLOR_RESET << std::endl;

    bool pass1 = Challenge1_BoundaryTemperatures();
    bool pass2 = Challenge2_ExtremeTemperatures();
    bool pass3 = Challenge3_ContinuousLongTicks();
    bool pass4 = Challenge4_CaloricClamping();
    bool pass5 = Challenge5_FocusDrainClamping();
    bool pass6 = Challenge6_MicroclimateShelter();
    bool pass7 = Challenge7_FlecsECSIsolation();
    bool pass8 = Challenge8_FloatingPointSanitization();

    std::cout << COLOR_BOLD << "\n================================================================================" << COLOR_RESET << std::endl;
    std::cout << "SUMMARY: " << g_total_assertions << " assertions evaluated across 8 challenge suites." << std::endl;
    if (g_failed_assertions == 0 && pass1 && pass2 && pass3 && pass4 && pass5 && pass6 && pass7 && pass8) {
        std::cout << COLOR_GREEN << COLOR_BOLD << "RESULT: 100% ADVERSARIAL TESTS PASSED (0 FAILURES)" << COLOR_RESET << std::endl;
        std::cout << COLOR_BOLD << "================================================================================" << COLOR_RESET << std::endl;
        return 0;
    } else {
        std::cout << COLOR_RED << COLOR_BOLD << "RESULT: " << g_failed_assertions << " ASSERTION FAILURES DETECTED!" << COLOR_RESET << std::endl;
        std::cout << COLOR_BOLD << "================================================================================" << COLOR_RESET << std::endl;
        return 1;
    }
}
