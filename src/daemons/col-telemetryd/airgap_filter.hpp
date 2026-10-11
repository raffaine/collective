#pragma once

#include <cstdint>
#include <string_view>
#include "uhai/uhai_ring_buffer.hpp"

namespace oasis::airgap {

/**
 * @brief Discrete evaluation decisions produced by the Hardware Airgap Filter.
 * Story 7.4 (Hardware Airgap Filter & SITL Isolation)
 */
enum class AirgapDecision : uint8_t {
    ACCEPTED_HARDWARE_ROOT = 0,               // 0x02: Authentic hardware root packet passed to HAL
    DROPPED_SIMULATED_SECURITY_VIOLATION = 1, // 0x01: Synthetic simulation packet barred from hardware
    DROPPED_INVALID_FLAG = 2                  // Any other value: Unrecognized/corrupted quality flags
};

/**
 * @brief Telemetry inspection accounting statistics.
 */
struct AirgapFilterStats {
    uint64_t total_inspected{0};
    uint64_t accepted_hardware{0};
    uint64_t dropped_simulated{0};
    uint64_t dropped_invalid{0};
};

/**
 * @brief Story 7.4: Hardware Airgap Filter enforcing strict SITL vs Physical isolation.
 * Guarantees that in-silico simulation frames (0x01) never trigger physical actuation relays.
 */
class HardwareAirgapFilter {
public:
    constexpr HardwareAirgapFilter() noexcept = default;

    /**
     * @brief Evaluate a TelemetrySample struct against the airgap security policy.
     */
    AirgapDecision Evaluate(const oasis::uhai::TelemetrySample& sample) noexcept {
        return Evaluate(sample.quality_flags);
    }

    /**
     * @brief Evaluate a raw quality_flags byte against the airgap security policy.
     */
    AirgapDecision Evaluate(uint8_t quality_flag) noexcept {
        stats_.total_inspected++;

        if (quality_flag == oasis::uhai::QUALITY_HARDWARE_ROOT) {
            stats_.accepted_hardware++;
            return AirgapDecision::ACCEPTED_HARDWARE_ROOT;
        } else if (quality_flag == oasis::uhai::QUALITY_SIMULATED) {
            stats_.dropped_simulated++;
            return AirgapDecision::DROPPED_SIMULATED_SECURITY_VIOLATION;
        } else {
            stats_.dropped_invalid++;
            return AirgapDecision::DROPPED_INVALID_FLAG;
        }
    }

    /**
     * @brief Convenience predicate returning true if the sample is authorized for hardware actuation.
     */
    [[nodiscard]] bool IsAuthorized(uint8_t quality_flag) noexcept {
        return Evaluate(quality_flag) == AirgapDecision::ACCEPTED_HARDWARE_ROOT;
    }

    [[nodiscard]] bool IsAuthorized(const oasis::uhai::TelemetrySample& sample) noexcept {
        return Evaluate(sample) == AirgapDecision::ACCEPTED_HARDWARE_ROOT;
    }

    /**
     * @brief Return human-readable string representation of a decision.
     */
    [[nodiscard]] static constexpr std::string_view DecisionToString(AirgapDecision decision) noexcept {
        switch (decision) {
            case AirgapDecision::ACCEPTED_HARDWARE_ROOT:
                return "ACCEPTED_HARDWARE_ROOT";
            case AirgapDecision::DROPPED_SIMULATED_SECURITY_VIOLATION:
                return "DROPPED_SIMULATED_SECURITY_VIOLATION";
            case AirgapDecision::DROPPED_INVALID_FLAG:
            default:
                return "DROPPED_INVALID_FLAG";
        }
    }

    [[nodiscard]] const AirgapFilterStats& GetStats() const noexcept { return stats_; }
    [[nodiscard]] uint64_t GetTotalInspected() const noexcept { return stats_.total_inspected; }
    [[nodiscard]] uint64_t GetAcceptedHardware() const noexcept { return stats_.accepted_hardware; }
    [[nodiscard]] uint64_t GetDroppedSimulated() const noexcept { return stats_.dropped_simulated; }
    [[nodiscard]] uint64_t GetDroppedInvalid() const noexcept { return stats_.dropped_invalid; }

    void Reset() noexcept { stats_ = {}; }

private:
    AirgapFilterStats stats_{};
};

} // namespace oasis::airgap

namespace oasis::uhai {
    using oasis::airgap::AirgapDecision;
    using oasis::airgap::AirgapFilterStats;
    using oasis::airgap::HardwareAirgapFilter;
} // namespace oasis::uhai

namespace uhai {
    using oasis::airgap::AirgapDecision;
    using oasis::airgap::AirgapFilterStats;
    using oasis::airgap::HardwareAirgapFilter;
} // namespace uhai
