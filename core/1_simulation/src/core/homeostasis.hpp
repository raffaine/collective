#pragma once

#include <cmath>
#include <algorithm>
#include <cstdint>
#include <glm/glm.hpp>
#include <flecs.h>
#include "core/fabrication.hpp"

namespace oasis::homeostasis {

// Temperature thresholds (°C)
constexpr float GOLDILOCKS_MIN_TEMP_C = 15.0f;
constexpr float GOLDILOCKS_MAX_TEMP_C = 30.0f;
constexpr float MEAN_AMBIENT_TEMP_C   = 22.5f;
constexpr float DIURNAL_AMPLITUDE_C   = 12.5f; // Night min: 10.0°C, Day max: 35.0°C
constexpr uint64_t DIURNAL_CYCLE_TICKS = 1200; // 20s at 60 Hz

// Metabolic burn & focus drain constants
constexpr double BASELINE_RESTING_BURN_RATE = 0.050;   // kcal/sec resting basal burn
constexpr double COLD_BURN_COEFF            = 0.080;   // +8% burn per °C below 15°C
constexpr double HOT_BURN_COEFF             = 0.060;   // +6% burn per °C above 30°C

constexpr float COLD_FOCUS_DRAIN_PER_SEC    = 0.0020f; // focus drain per °C below 15°C per sec
constexpr float HOT_FOCUS_DRAIN_PER_SEC     = 0.0015f; // focus drain per °C above 30°C per sec

struct HomeostasisEvaluation {
    double burn_multiplier{1.0};
    float focus_drain_rate{0.0f}; // per sec
    bool in_goldilocks{true};
    bool is_cold_stress{false};
    bool is_heat_stress{false};
};

inline float CalculateDiurnalTemperature(uint64_t sim_tick, uint64_t cycle_period = DIURNAL_CYCLE_TICKS) noexcept {
    if (cycle_period == 0) cycle_period = DIURNAL_CYCLE_TICKS;
    const float phase = static_cast<float>(sim_tick % cycle_period) / static_cast<float>(cycle_period);
    // phase 0.0: dawn (22.5°C), 0.25: noon (35.0°C), 0.5: dusk (22.5°C), 0.75: night (10.0°C)
    constexpr float kTwoPi = 6.28318530717958647692f;
    const float angle = kTwoPi * phase;
    return MEAN_AMBIENT_TEMP_C + DIURNAL_AMPLITUDE_C * std::sin(angle);
}

inline bool IsSheltered(const glm::vec3& world_pos) noexcept {
    const int absX = std::abs(static_cast<int>(std::floor(world_pos.x)));
    const int lotZ = ((static_cast<int>(std::floor(world_pos.z)) % 16) + 16) % 16;
    const int vy   = static_cast<int>(std::floor(world_pos.y));
    return (absX >= 9 && absX <= 17 && lotZ >= 2 && lotZ <= 10 && vy >= 1 && vy <= 3);
}

inline float ResolveVoxelAmbientTemperature(const glm::vec3& world_pos, uint64_t sim_tick, uint64_t cycle_period = DIURNAL_CYCLE_TICKS) noexcept {
    float base_temp = CalculateDiurnalTemperature(sim_tick, cycle_period);
    if (IsSheltered(world_pos)) {
        // 70% thermal moderation towards 22.0°C
        base_temp = 22.0f + 0.3f * (base_temp - 22.0f);
    }
    return base_temp;
}

inline HomeostasisEvaluation EvaluateHomeostasis(float ambient_temp_c) noexcept {
    HomeostasisEvaluation eval{};
    if (ambient_temp_c < GOLDILOCKS_MIN_TEMP_C) {
        const double delta = static_cast<double>(GOLDILOCKS_MIN_TEMP_C - ambient_temp_c);
        eval.burn_multiplier = 1.0 + COLD_BURN_COEFF * delta;
        eval.focus_drain_rate = static_cast<float>(COLD_FOCUS_DRAIN_PER_SEC * delta);
        eval.in_goldilocks = false;
        eval.is_cold_stress = true;
    } else if (ambient_temp_c > GOLDILOCKS_MAX_TEMP_C) {
        const double delta = static_cast<double>(ambient_temp_c - GOLDILOCKS_MAX_TEMP_C);
        eval.burn_multiplier = 1.0 + HOT_BURN_COEFF * delta;
        eval.focus_drain_rate = static_cast<float>(HOT_FOCUS_DRAIN_PER_SEC * delta);
        eval.in_goldilocks = false;
        eval.is_heat_stress = true;
    } else {
        eval.burn_multiplier = 1.0;
        eval.focus_drain_rate = 0.0f;
        eval.in_goldilocks = true;
    }
    return eval;
}

template <typename PsychComponentType>
inline void StepHomeostasis(
    oasis::fabrication::SomaticStateComponent& somatic,
    PsychComponentType& psych,
    float ambient_temp_c,
    float dt,
    double base_burn_rate = BASELINE_RESTING_BURN_RATE
) noexcept {
    somatic.ambient_temp_c = ambient_temp_c;
    const auto eval = EvaluateHomeostasis(ambient_temp_c);

    // Continuous metabolic expenditure
    const double effective_burn = base_burn_rate * eval.burn_multiplier;
    const double cal_delta = effective_burn * static_cast<double>(dt);
    somatic.caloric_reserve = std::max(0.0, somatic.caloric_reserve - cal_delta);

    // Thermal focus drain
    if (eval.focus_drain_rate > 0.0f) {
        const float focus_delta = eval.focus_drain_rate * dt;
        psych.focus = std::max(0.0f, psych.focus - focus_delta);
        somatic.focus = psych.focus;
    }

    if (somatic.caloric_reserve <= 500.0 || psych.focus <= 0.05f) {
        somatic.is_exhausted = true;
    }
}

} // namespace oasis::homeostasis
