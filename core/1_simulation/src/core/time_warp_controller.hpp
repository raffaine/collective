#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>

namespace oasis::time {

constexpr double EMERGENCY_CALORIC_THRESHOLD_KCAL = 800.0;
constexpr float  EMERGENCY_HYDRATION_THRESHOLD    = 0.25f;
constexpr float  EMERGENCY_STRESS_THRESHOLD       = 0.95f;
constexpr uint32_t DEFAULT_LOG_INTERVAL_TICKS     = 300;

struct TimeWarpState {
    uint32_t multiplier{1}; // Discrete simulation speed multiplier in {0, 1, 2, 5, 10}
    uint64_t logical_tick{0};
    uint64_t last_log_tick{0};
    uint32_t log_interval_ticks{DEFAULT_LOG_INTERVAL_TICKS};
    bool emergency_brake_triggered{false};
    std::string emergency_reason{};
};

// Clamps/snaps requested speed multiplier to discrete {0, 1, 2, 5, 10} set
inline void SetTimeWarpMultiplier(TimeWarpState& state, uint32_t mult) noexcept {
    if (mult == 0) {
        state.multiplier = 0;
    } else if (mult <= 1) {
        state.multiplier = 1;
    } else if (mult <= 2) {
        state.multiplier = 2;
    } else if (mult <= 5) {
        state.multiplier = 5;
    } else {
        state.multiplier = 10;
    }
}

// Evaluates physiological, psychological, and boundary emergency thresholds.
// When multiplier > 1 and any threshold is breached, automatically downshifts to 1x.
inline bool EvaluateEmergencyWarpBrake(
    TimeWarpState& state,
    double caloric_reserve,
    float hydration,
    float stress,
    bool migration_wave_arriving = false
) {
    if (state.multiplier <= 1) {
        return false;
    }

    if (caloric_reserve < EMERGENCY_CALORIC_THRESHOLD_KCAL) {
        state.multiplier = 1;
        state.emergency_brake_triggered = true;
        state.emergency_reason = "CRITICAL_CALORIC_DEFICIT (< 800 kcal)";
        return true;
    }

    if (hydration < EMERGENCY_HYDRATION_THRESHOLD) {
        state.multiplier = 1;
        state.emergency_brake_triggered = true;
        state.emergency_reason = "CRITICAL_DEHYDRATION (< 0.25)";
        return true;
    }

    if (stress >= EMERGENCY_STRESS_THRESHOLD) {
        state.multiplier = 1;
        state.emergency_brake_triggered = true;
        state.emergency_reason = "IMMOBILIZING_PSYCHOLOGICAL_STRESS (>= 0.95)";
        return true;
    }

    if (migration_wave_arriving) {
        state.multiplier = 1;
        state.emergency_brake_triggered = true;
        state.emergency_reason = "MIGRATION_WAVE_ARRIVING";
        return true;
    }

    return false;
}

// Throttles periodic diagnostic logs during high-speed time warp (default every 300 ticks).
// Critical events and tick 1 always bypass throttling.
[[nodiscard]] inline bool ShouldEmitThrottledLog(
    TimeWarpState& state,
    uint64_t current_tick,
    bool is_critical = false
) noexcept {
    state.logical_tick = current_tick;
    if (is_critical) {
        return true;
    }
    if (current_tick == 1) {
        return true;
    }
    if (current_tick >= state.last_log_tick &&
        (current_tick - state.last_log_tick) >= state.log_interval_ticks) {
        state.last_log_tick = current_tick;
        return true;
    }
    return false;
}

struct EngineCliOptions {
    bool headless_ci{false};
    uint64_t max_ticks{60};
};

[[nodiscard]] inline EngineCliOptions ParseEngineCliOptions(int argc, const char* const* argv) noexcept {
    EngineCliOptions opts{};
    if (argc <= 0 || argv == nullptr) {
        return opts;
    }

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == nullptr) continue;
        std::string_view arg(argv[i]);
        if (arg == "--headless-ci") {
            opts.headless_ci = true;
        } else if (arg.starts_with("--max-ticks=")) {
            std::string_view num_str = arg.substr(std::string_view("--max-ticks=").size());
            uint64_t parsed = 0;
            for (char c : num_str) {
                if (c >= '0' && c <= '9') {
                    parsed = parsed * 10 + static_cast<uint64_t>(c - '0');
                }
            }
            if (parsed > 0) {
                opts.max_ticks = parsed;
            }
        }
    }

    return opts;
}

[[nodiscard]] inline EngineCliOptions ParseEngineCliOptions(int argc, char* argv[]) noexcept {
    return ParseEngineCliOptions(argc, static_cast<const char* const*>(argv));
}

} // namespace oasis::time

namespace oasis {
    using oasis::time::TimeWarpState;
    using oasis::time::SetTimeWarpMultiplier;
    using oasis::time::EvaluateEmergencyWarpBrake;
    using oasis::time::ShouldEmitThrottledLog;
    using oasis::time::EngineCliOptions;
    using oasis::time::ParseEngineCliOptions;
} // namespace oasis
