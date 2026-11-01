#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>

namespace oasis::engine {

// Grounded, calm pacing: 1 real second = 12 simulated seconds at 1x warp
// (1 real minute = 12 simulated minutes; 2 real hours = 1 full 24h simulated day).
inline constexpr uint32_t default_sim_seconds_per_real_second = 12u;
inline constexpr uint32_t kDefaultSimSecondsPerRealSecond     = default_sim_seconds_per_real_second;

inline constexpr uint32_t kSimMsPerSecond      = 1000u;
inline constexpr uint32_t kSimMsPerMinute      = 60u * kSimMsPerSecond;
inline constexpr uint32_t kSimMsPerHour        = 60u * kSimMsPerMinute;
inline constexpr uint64_t kSimMsPerDay         = 24ULL * static_cast<uint64_t>(kSimMsPerHour);
inline constexpr uint32_t kDefaultFrameClampMs = 250u;

enum class WarpSpeed : uint8_t {
    PAUSED = 0,
    X1     = 1,
    X2     = 2,
    X5     = 5,
    X10    = 10
};

struct alignas(8) CalendarTimestamp {
    uint32_t day{1u};        // 1-indexed simulated calendar day (Day 1, Day 2, ...)
    uint8_t  hour{8u};       // [0 .. 23]
    uint8_t  minute{0u};     // [0 .. 59]
    uint8_t  second{0u};     // [0 .. 59]
    uint8_t  is_daylight{1u}; // 1 when hour in [6 .. 19], 0 at night
};
static_assert(sizeof(CalendarTimestamp) == 8, "CalendarTimestamp must be 8 bytes with zero implicit padding");

struct alignas(8) ActivityTimer {
    uint32_t duration_sim_ms{0u};
    uint32_t elapsed_sim_ms{0u};
    uint32_t min_dwell_sim_ms{0u};
    uint8_t  is_active{0u};
    uint8_t  is_completed{0u};
    uint8_t  pad_[2]{0u, 0u};

    constexpr void Start(uint32_t duration_ms, uint32_t min_dwell_ms = 0u) noexcept {
        duration_sim_ms  = duration_ms;
        elapsed_sim_ms   = 0u;
        min_dwell_sim_ms = min_dwell_ms;
        is_active        = 1u;
        is_completed     = (duration_ms == 0u && min_dwell_ms == 0u) ? 1u : 0u;
    }

    constexpr void Reset() noexcept {
        duration_sim_ms  = 0u;
        elapsed_sim_ms   = 0u;
        min_dwell_sim_ms = 0u;
        is_active        = 0u;
        is_completed     = 0u;
    }

    // Advances the activity timer by sim_dt_ms; returns true once both duration and min_dwell are satisfied.
    constexpr bool Advance(uint32_t sim_dt_ms) noexcept {
        if (is_active == 0u) {
            return false;
        }
        const uint64_t next = static_cast<uint64_t>(elapsed_sim_ms) + static_cast<uint64_t>(sim_dt_ms);
        elapsed_sim_ms = (next > static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()))
            ? std::numeric_limits<uint32_t>::max()
            : static_cast<uint32_t>(next);

        const uint32_t required_ms = std::max(duration_sim_ms, min_dwell_sim_ms);
        if (elapsed_sim_ms >= required_ms) {
            is_completed = 1u;
            return true;
        }
        return false;
    }

    [[nodiscard]] constexpr bool CanInterrupt() const noexcept {
        return (is_active == 0u) || (elapsed_sim_ms >= min_dwell_sim_ms);
    }

    [[nodiscard]] constexpr uint32_t RemainingSimMs() const noexcept {
        if (is_active == 0u) {
            return 0u;
        }
        const uint32_t target_ms = std::max(duration_sim_ms, min_dwell_sim_ms);
        return (elapsed_sim_ms >= target_ms) ? 0u : (target_ms - elapsed_sim_ms);
    }

    [[nodiscard]] constexpr uint32_t ProgressBasisPoints() const noexcept {
        if (is_active == 0u) {
            return 0u;
        }
        const uint32_t target_ms = std::max(duration_sim_ms, min_dwell_sim_ms);
        if (target_ms == 0u || elapsed_sim_ms >= target_ms) {
            return 10000u;
        }
        return static_cast<uint32_t>((static_cast<uint64_t>(elapsed_sim_ms) * 10000ULL) / target_ms);
    }
};
static_assert(sizeof(ActivityTimer) == 16, "ActivityTimer must be 16 bytes with zero implicit padding");

class alignas(8) SimulationClock {
public:
    constexpr SimulationClock() noexcept = default;

    constexpr explicit SimulationClock(
        uint32_t sim_seconds_per_real_second,
        uint64_t initial_sim_ms = 8ULL * static_cast<uint64_t>(kSimMsPerHour)
    ) noexcept
        : total_sim_ms_(initial_sim_ms),
          total_real_ms_(0ULL),
          step_count_(0ULL),
          sim_seconds_per_real_second_(sim_seconds_per_real_second == 0u
              ? default_sim_seconds_per_real_second
              : sim_seconds_per_real_second),
          warp_multiplier_(1u),
          paused_(false) {}

    // Snaps an integer warp request to the discrete supported set {1, 2, 5, 10}.
    [[nodiscard]] static constexpr uint8_t SnapSupportedMultiplier(uint32_t requested) noexcept {
        if (requested <= 1u) {
            return 1u;
        }
        if (requested <= 2u) {
            return 2u;
        }
        if (requested <= 5u) {
            return 5u;
        }
        return 10u;
    }

    // Sets warp multiplier in {1, 2, 5, 10}; passing 0 engages pause while preserving the last active multiplier.
    constexpr void SetWarpMultiplier(uint32_t requested_multiplier) noexcept {
        if (requested_multiplier == 0u) {
            paused_ = true;
            return;
        }
        paused_ = false;
        warp_multiplier_ = SnapSupportedMultiplier(requested_multiplier);
    }

    constexpr void SetWarpSpeed(WarpSpeed speed) noexcept {
        SetWarpMultiplier(static_cast<uint32_t>(speed));
    }

    constexpr void SetPaused(bool paused) noexcept {
        paused_ = paused;
    }

    constexpr void TogglePause() noexcept {
        paused_ = !paused_;
    }

    constexpr void EmergencyDownshiftTo1x() noexcept {
        paused_ = false;
        warp_multiplier_ = 1u;
    }

    [[nodiscard]] constexpr bool IsPaused() const noexcept {
        return paused_;
    }

    [[nodiscard]] constexpr uint8_t WarpMultiplier() const noexcept {
        return paused_ ? 0u : warp_multiplier_;
    }

    [[nodiscard]] constexpr uint8_t ConfiguredWarpMultiplier() const noexcept {
        return warp_multiplier_;
    }

    [[nodiscard]] constexpr uint32_t BaseSimSecondsPerRealSecond() const noexcept {
        return sim_seconds_per_real_second_;
    }

    [[nodiscard]] constexpr uint32_t EffectiveSimSecondsPerRealSecond() const noexcept {
        return paused_ ? 0u : (sim_seconds_per_real_second_ * static_cast<uint32_t>(warp_multiplier_));
    }

    // Advances simulation clock by real_dt_ms and returns elapsed simulated milliseconds (sim_dt_ms).
    constexpr uint32_t AdvanceRealDelta(uint32_t real_dt_ms) noexcept {
        total_real_ms_ += static_cast<uint64_t>(real_dt_ms);
        ++step_count_;
        if (paused_ || real_dt_ms == 0u) {
            return 0u;
        }

        const uint64_t raw_sim_ms = static_cast<uint64_t>(real_dt_ms) *
            static_cast<uint64_t>(sim_seconds_per_real_second_) *
            static_cast<uint64_t>(warp_multiplier_);

        const uint32_t clamped_sim_ms = (raw_sim_ms > static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()))
            ? std::numeric_limits<uint32_t>::max()
            : static_cast<uint32_t>(raw_sim_ms);

        total_sim_ms_ += static_cast<uint64_t>(clamped_sim_ms);
        return clamped_sim_ms;
    }

    // Frame-loop convenience wrapper that clamps abnormally long real frame deltas (e.g., backgrounded tab).
    constexpr uint32_t AdvanceClampedFrameDelta(
        uint32_t real_frame_dt_ms,
        uint32_t max_real_dt_ms = kDefaultFrameClampMs
    ) noexcept {
        return AdvanceRealDelta(std::min(real_frame_dt_ms, max_real_dt_ms));
    }

    [[nodiscard]] constexpr uint64_t TotalSimMs() const noexcept {
        return total_sim_ms_;
    }

    [[nodiscard]] constexpr uint64_t TotalSimSeconds() const noexcept {
        return total_sim_ms_ / static_cast<uint64_t>(kSimMsPerSecond);
    }

    [[nodiscard]] constexpr uint64_t TotalRealMs() const noexcept {
        return total_real_ms_;
    }

    [[nodiscard]] constexpr uint64_t StepCount() const noexcept {
        return step_count_;
    }

    [[nodiscard]] static constexpr CalendarTimestamp BreakDownSimMs(uint64_t sim_ms) noexcept {
        CalendarTimestamp out{};
        out.day = static_cast<uint32_t>(sim_ms / kSimMsPerDay) + 1u;
        const uint64_t rem_day_ms = sim_ms % kSimMsPerDay;
        out.hour   = static_cast<uint8_t>(rem_day_ms / static_cast<uint64_t>(kSimMsPerHour));
        const uint64_t rem_hr_ms = rem_day_ms % static_cast<uint64_t>(kSimMsPerHour);
        out.minute = static_cast<uint8_t>(rem_hr_ms / static_cast<uint64_t>(kSimMsPerMinute));
        const uint64_t rem_min_ms = rem_hr_ms % static_cast<uint64_t>(kSimMsPerMinute);
        out.second = static_cast<uint8_t>(rem_min_ms / static_cast<uint64_t>(kSimMsPerSecond));
        out.is_daylight = (out.hour >= 6u && out.hour < 20u) ? 1u : 0u;
        return out;
    }

    [[nodiscard]] constexpr CalendarTimestamp CurrentCalendar() const noexcept {
        return BreakDownSimMs(total_sim_ms_);
    }

    [[nodiscard]] inline std::string FormatCalendar(bool include_seconds = false) const {
        const CalendarTimestamp cal = CurrentCalendar();
        std::array<char, 48> buf{};
        int written = 0;
        if (include_seconds) {
            written = std::snprintf(
                buf.data(), buf.size(), "Day %u, %02u:%02u:%02u",
                cal.day, static_cast<unsigned>(cal.hour),
                static_cast<unsigned>(cal.minute), static_cast<unsigned>(cal.second)
            );
        } else {
            written = std::snprintf(
                buf.data(), buf.size(), "Day %u, %02u:%02u",
                cal.day, static_cast<unsigned>(cal.hour),
                static_cast<unsigned>(cal.minute)
            );
        }
        return (written > 0) ? std::string(buf.data()) : std::string("Day 1, 08:00");
    }

    // Pure-integer diurnal ambient temperature curve in milli-Kelvin (trough at 04:00, peak at 16:00).
    [[nodiscard]] constexpr uint32_t AmbientTempMilliKelvin(
        uint32_t night_min_mk = 285150u,
        uint32_t day_max_mk = 299150u
    ) const noexcept {
        if (day_max_mk <= night_min_mk) {
            return night_min_mk;
        }
        const uint32_t span_mk = day_max_mk - night_min_mk;
        const uint64_t rem_day_ms = total_sim_ms_ % kSimMsPerDay;
        const uint64_t trough_ms = 4ULL * static_cast<uint64_t>(kSimMsPerHour);
        const uint64_t shifted_ms = (rem_day_ms + kSimMsPerDay - trough_ms) % kSimMsPerDay;
        const uint64_t half_day_ms = kSimMsPerDay / 2ULL;
        const uint64_t dist_ms = (shifted_ms <= half_day_ms)
            ? shifted_ms
            : (kSimMsPerDay - shifted_ms);
        // Smooth S-curve 3t^2 - 2t^3 in [0 .. 10000] basis points
        const uint64_t t_bp = (dist_ms * 10000ULL) / half_day_ms;
        const uint64_t smooth_bp = (t_bp * t_bp * (30000ULL - 2ULL * t_bp)) / 100000000ULL;
        return night_min_mk + static_cast<uint32_t>((static_cast<uint64_t>(span_mk) * smooth_bp) / 10000ULL);
    }

private:
    uint64_t total_sim_ms_{8ULL * static_cast<uint64_t>(kSimMsPerHour)};
    uint64_t total_real_ms_{0ULL};
    uint64_t step_count_{0ULL};
    uint32_t sim_seconds_per_real_second_{default_sim_seconds_per_real_second};
    uint8_t  warp_multiplier_{1u};
    bool     paused_{false};
    [[maybe_unused]] uint8_t pad_[2]{0u, 0u};
};
static_assert(sizeof(SimulationClock) == 32, "SimulationClock must be 32 bytes with zero implicit padding");

} // namespace oasis::engine
