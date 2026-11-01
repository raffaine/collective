#pragma once

#include <algorithm>
#include <cstdint>

#include "../content/content_registry.hpp"

namespace oasis::engine {

// Physical human & equipment hauling thresholds (all integer grams / milliliters / basis points)
inline constexpr uint32_t kBarehandLiquidMaxMl            = 0u;
inline constexpr uint32_t kBarehandGranularHandfulMassG   = 500u;
inline constexpr uint32_t kBarehandGranularSpeedBp        = 3500u;
inline constexpr uint32_t kBarehandComfortSolidMassG      = 20000u;
inline constexpr uint32_t kBarehandMaxSolidLiftMassG      = 35000u;
inline constexpr uint32_t kBarehandMinHeavySpeedBp        = 5500u;
inline constexpr uint32_t kNominalWalkSpeedBp             = 10000u;

inline constexpr uint32_t kBucketMaxGranularMassG         = 20000u;
inline constexpr uint32_t kBucketDefaultSpeedBp           = 9000u;
inline constexpr uint32_t kWheelbarrowMaxMassG            = 120000u;
inline constexpr uint32_t kWheelbarrowDefaultSpeedBp      = 10000u;
inline constexpr uint32_t kHandDollyMaxMassG              = 200000u;
inline constexpr uint32_t kCargoEtrikeMaxMassG            = 250000u;
inline constexpr uint32_t kForkliftMaxMassG               = 1500000u;

enum class HaulFeasibility : uint8_t {
    FEASIBLE                              = 0,
    VERY_SLOW_BAREHAND_GRANULAR           = 1,
    IMPOSSIBLE_NEEDS_LIQUID_VESSEL        = 2,
    IMPOSSIBLE_TOO_HEAVY_BAREHAND         = 3,
    IMPOSSIBLE_EXCEEDS_TOOL_CAPACITY      = 4,
    IMPOSSIBLE_NEEDS_PAVED_OR_GRADED_PATH = 5
};

[[nodiscard]] constexpr bool IsHaulPossible(HaulFeasibility feasibility) noexcept {
    return feasibility == HaulFeasibility::FEASIBLE ||
           feasibility == HaulFeasibility::VERY_SLOW_BAREHAND_GRANULAR;
}

struct alignas(8) HaulEvaluation {
    uint32_t         trips_required{0u};
    uint32_t         effective_speed_bp{0u};
    uint32_t         max_payload_per_trip_g{0u};
    uint32_t         max_volume_per_trip_ml{0u};
    HaulFeasibility  feasibility{HaulFeasibility::FEASIBLE};
    HaulingToolClass recommended_tool_class{HaulingToolClass::NONE_BARE_HANDS};
    uint8_t          pad_[6]{0u, 0u, 0u, 0u, 0u, 0u};
};
static_assert(sizeof(HaulEvaluation) == 24, "HaulEvaluation must be 24 bytes with zero implicit padding");

[[nodiscard]] constexpr uint32_t CeilDivU64ToU32(uint64_t numerator, uint64_t denominator) noexcept {
    if (numerator == 0ULL || denominator == 0ULL) {
        return 0u;
    }
    const uint64_t q = (numerator + denominator - 1ULL) / denominator;
    return (q > 0xFFFFFFFFULL) ? 0xFFFFFFFFu : static_cast<uint32_t>(q);
}

[[nodiscard]] constexpr HaulingToolClass RecommendToolForDiscreteMass(
    uint32_t unit_mass_g,
    uint64_t total_mass_g,
    bool path_is_paved
) noexcept {
    const uint64_t governing_mass = std::max(static_cast<uint64_t>(unit_mass_g), total_mass_g);
    if (unit_mass_g <= kBarehandComfortSolidMassG && governing_mass <= kBarehandComfortSolidMassG) {
        return HaulingToolClass::NONE_BARE_HANDS;
    }
    if (unit_mass_g <= kWheelbarrowMaxMassG && (!path_is_paved || governing_mass <= kWheelbarrowMaxMassG)) {
        return HaulingToolClass::WHEELBARROW;
    }
    if (unit_mass_g <= kHandDollyMaxMassG) {
        return HaulingToolClass::HAND_DOLLY;
    }
    if (unit_mass_g <= kCargoEtrikeMaxMassG) {
        return HaulingToolClass::CARGO_ETRIKE;
    }
    return HaulingToolClass::FORKLIFT;
}

[[nodiscard]] constexpr HaulingToolClass RecommendToolForGranularMass(uint64_t total_mass_g) noexcept {
    if (total_mass_g <= static_cast<uint64_t>(kBucketMaxGranularMassG)) {
        return HaulingToolClass::GRANULAR_BUCKET_OR_SACK;
    }
    return HaulingToolClass::WHEELBARROW;
}

[[nodiscard]] constexpr HaulingToolClass RecommendToolForLiquidVolume(
    uint64_t total_volume_ml,
    bool path_is_paved
) noexcept {
    if (total_volume_ml > 40000ULL && path_is_paved) {
        return HaulingToolClass::CARGO_ETRIKE;
    }
    return HaulingToolClass::LIQUID_VESSEL;
}

[[nodiscard]] constexpr uint32_t ComputeBarehandSolidSpeedBp(uint32_t payload_mass_g) noexcept {
    if (payload_mass_g <= kBarehandComfortSolidMassG) {
        return kNominalWalkSpeedBp;
    }
    if (payload_mass_g >= kBarehandMaxSolidLiftMassG) {
        return kBarehandMinHeavySpeedBp;
    }
    const uint32_t excess_g = payload_mass_g - kBarehandComfortSolidMassG;
    const uint32_t span_g   = kBarehandMaxSolidLiftMassG - kBarehandComfortSolidMassG; // 15,000 g
    const uint32_t drop_bp  = (excess_g * (kNominalWalkSpeedBp - kBarehandMinHeavySpeedBp)) / span_g;
    return kNominalWalkSpeedBp - drop_bp;
}

// Evaluates physical feasibility, trip count, effective speed, and recommended carrier class for an item haul.
[[nodiscard]] constexpr HaulEvaluation EvaluateHaulPlan(
    const ItemDef& item,
    uint32_t quantity,
    const HaulingToolDef* equipped_tool,
    bool path_is_paved = true
) noexcept {
    HaulEvaluation out{};
    if (quantity == 0u) {
        out.feasibility            = HaulFeasibility::FEASIBLE;
        out.trips_required         = 0u;
        out.effective_speed_bp     = kNominalWalkSpeedBp;
        out.recommended_tool_class = HaulingToolClass::NONE_BARE_HANDS;
        return out;
    }

    const bool is_bare_hands = (equipped_tool == nullptr) ||
                               (equipped_tool->tool_class == HaulingToolClass::NONE_BARE_HANDS);

    if (!is_bare_hands && equipped_tool->requires_paved_or_graded_path && !path_is_paved) {
        out.feasibility        = HaulFeasibility::IMPOSSIBLE_NEEDS_PAVED_OR_GRADED_PATH;
        out.trips_required     = 0u;
        out.effective_speed_bp = 0u;
        out.recommended_tool_class = (item.matter_phase == MatterPhase::LIQUID)
            ? HaulingToolClass::LIQUID_VESSEL
            : HaulingToolClass::WHEELBARROW;
        return out;
    }

    const uint32_t safe_unit_mass_g = std::max(1u, item.unit_mass_g);
    const uint32_t safe_unit_vol_ml = std::max(1u, item.unit_volume_ml);
    const uint64_t total_mass_g     = static_cast<uint64_t>(safe_unit_mass_g) * static_cast<uint64_t>(quantity);
    const uint64_t total_vol_ml     = static_cast<uint64_t>(safe_unit_vol_ml) * static_cast<uint64_t>(quantity);

    // Law 1: LIQUID — 0 ml bare-handed capacity; strictly requires a liquid-capable vessel or tank.
    if (item.matter_phase == MatterPhase::LIQUID) {
        out.recommended_tool_class = RecommendToolForLiquidVolume(total_vol_ml, path_is_paved);
        if (is_bare_hands || equipped_tool->max_liquid_volume_ml == kBarehandLiquidMaxMl) {
            out.feasibility            = HaulFeasibility::IMPOSSIBLE_NEEDS_LIQUID_VESSEL;
            out.trips_required         = 0u;
            out.effective_speed_bp     = 0u;
            out.max_payload_per_trip_g = 0u;
            out.max_volume_per_trip_ml = 0u;
            return out;
        }

        const uint64_t vol_cap_by_mass_ml = (equipped_tool->max_payload_mass_g > 0u)
            ? ((static_cast<uint64_t>(equipped_tool->max_payload_mass_g) * static_cast<uint64_t>(safe_unit_vol_ml)) /
               static_cast<uint64_t>(safe_unit_mass_g))
            : static_cast<uint64_t>(equipped_tool->max_liquid_volume_ml);
        const uint64_t trip_vol_ml = std::max<uint64_t>(
            1ULL,
            std::min<uint64_t>(equipped_tool->max_liquid_volume_ml, vol_cap_by_mass_ml)
        );

        out.feasibility            = HaulFeasibility::FEASIBLE;
        out.trips_required         = CeilDivU64ToU32(total_vol_ml, trip_vol_ml);
        out.effective_speed_bp     = equipped_tool->speed_multiplier_bp;
        out.max_volume_per_trip_ml = static_cast<uint32_t>(trip_vol_ml);
        out.max_payload_per_trip_g = static_cast<uint32_t>(
            (trip_vol_ml * static_cast<uint64_t>(safe_unit_mass_g)) / static_cast<uint64_t>(safe_unit_vol_ml)
        );
        return out;
    }

    // Law 2: GRANULAR_PARTICULATE — divisible particulate matter (500 g handful bare-handed at 3500 bp).
    if (item.matter_phase == MatterPhase::GRANULAR_PARTICULATE) {
        out.recommended_tool_class = RecommendToolForGranularMass(total_mass_g);
        if (is_bare_hands) {
            out.feasibility            = HaulFeasibility::VERY_SLOW_BAREHAND_GRANULAR;
            out.trips_required         = CeilDivU64ToU32(total_mass_g, kBarehandGranularHandfulMassG);
            out.effective_speed_bp     = kBarehandGranularSpeedBp;
            out.max_payload_per_trip_g = kBarehandGranularHandfulMassG;
            return out;
        }

        if (equipped_tool->max_granular_mass_g == 0u) {
            out.feasibility            = HaulFeasibility::VERY_SLOW_BAREHAND_GRANULAR;
            out.trips_required         = CeilDivU64ToU32(total_mass_g, kBarehandGranularHandfulMassG);
            out.effective_speed_bp     = kBarehandGranularSpeedBp;
            out.max_payload_per_trip_g = kBarehandGranularHandfulMassG;
            return out;
        }

        const uint32_t trip_mass_g = (equipped_tool->max_payload_mass_g > 0u)
            ? std::min(equipped_tool->max_granular_mass_g, equipped_tool->max_payload_mass_g)
            : equipped_tool->max_granular_mass_g;

        out.feasibility            = HaulFeasibility::FEASIBLE;
        out.trips_required         = CeilDivU64ToU32(total_mass_g, std::max(1u, trip_mass_g));
        out.effective_speed_bp     = equipped_tool->speed_multiplier_bp;
        out.max_payload_per_trip_g = trip_mass_g;
        return out;
    }

    // Law 3: DISCRETE_SOLID — indivisible solid units; bare-handed human lift max 35,000 g (slowed > 20,000 g).
    out.recommended_tool_class = RecommendToolForDiscreteMass(safe_unit_mass_g, total_mass_g, path_is_paved);
    if (is_bare_hands) {
        if (safe_unit_mass_g > kBarehandMaxSolidLiftMassG) {
            out.feasibility            = HaulFeasibility::IMPOSSIBLE_TOO_HEAVY_BAREHAND;
            out.trips_required         = 0u;
            out.effective_speed_bp     = 0u;
            out.max_payload_per_trip_g = 0u;
            return out;
        }

        const uint32_t units_per_trip = (safe_unit_mass_g > kBarehandComfortSolidMassG)
            ? 1u
            : std::max(1u, kBarehandComfortSolidMassG / safe_unit_mass_g);
        const uint32_t trip_mass_g = units_per_trip * safe_unit_mass_g;

        out.feasibility            = HaulFeasibility::FEASIBLE;
        out.trips_required         = CeilDivU64ToU32(quantity, units_per_trip);
        out.effective_speed_bp     = ComputeBarehandSolidSpeedBp(trip_mass_g);
        out.max_payload_per_trip_g = trip_mass_g;
        return out;
    }

    if (safe_unit_mass_g > equipped_tool->max_payload_mass_g) {
        out.feasibility            = HaulFeasibility::IMPOSSIBLE_EXCEEDS_TOOL_CAPACITY;
        out.trips_required         = 0u;
        out.effective_speed_bp     = 0u;
        out.max_payload_per_trip_g = 0u;
        return out;
    }

    const uint32_t units_per_trip = std::max(1u, equipped_tool->max_payload_mass_g / safe_unit_mass_g);
    out.feasibility            = HaulFeasibility::FEASIBLE;
    out.trips_required         = CeilDivU64ToU32(quantity, units_per_trip);
    out.effective_speed_bp     = equipped_tool->speed_multiplier_bp;
    out.max_payload_per_trip_g = units_per_trip * safe_unit_mass_g;
    return out;
}

// Estimates round-trip simulated haul duration in milliseconds for distance_voxels (1 voxel = 1 meter).
[[nodiscard]] constexpr uint32_t EstimateTotalHaulDurationSimMs(
    const HaulEvaluation& eval,
    uint32_t one_way_distance_voxels,
    uint32_t base_walk_speed_mm_per_s = 1400u
) noexcept {
    if (!IsHaulPossible(eval.feasibility) || eval.trips_required == 0u || eval.effective_speed_bp == 0u) {
        return 0u;
    }
    const uint64_t eff_speed_mm_s = std::max<uint64_t>(
        100ULL,
        (static_cast<uint64_t>(base_walk_speed_mm_per_s) * static_cast<uint64_t>(eval.effective_speed_bp)) / 10000ULL
    );
    // Outbound loaded + return unloaded (2 * distance * 1000 mm) plus 4s load/unload handling per trip
    const uint64_t round_trip_mm = 2ULL * static_cast<uint64_t>(std::max(1u, one_way_distance_voxels)) * 1000ULL;
    const uint64_t per_trip_ms   = ((round_trip_mm * 1000ULL) / eff_speed_mm_s) + 4000ULL;
    const uint64_t total_ms      = per_trip_ms * static_cast<uint64_t>(eval.trips_required);
    return (total_ms > 0xFFFFFFFFULL) ? 0xFFFFFFFFu : static_cast<uint32_t>(total_ms);
}

} // namespace oasis::engine
