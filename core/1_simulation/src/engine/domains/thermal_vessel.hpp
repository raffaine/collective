#pragma once

#include <algorithm>
#include <cstdint>

namespace oasis::engine {

// Canonical pure-integer thermodynamic constants (milli-Kelvin, milli-Watts, milli-Joules, milliliters)
inline constexpr uint64_t kWaterSpecificHeatMjPerMlK     = 4184ULL;    // 4.184 J / (ml * K) = 4184 mJ / (ml * K)
inline constexpr uint64_t kDryVesselHeatCapacityMjPerK   = 850000ULL;  // 850 J / K empty stainless kettle shell
inline constexpr uint64_t kLatentHeatVaporizationMjPerMl = 2260000ULL; // 2260 J / ml latent heat of steam

inline constexpr uint32_t kAmbientDefaultTempMk          = 293150u;    // 20.00 C
inline constexpr uint32_t kEvaporationOnsetTempMk        = 323150u;    // 50.00 C
inline constexpr uint32_t kBigelowPasteurizationTempMk   = 347150u;    // 74.00 C reference lethality threshold
inline constexpr uint32_t kBoilingPointWaterMk           = 373150u;    // 100.00 C at 1 atm
inline constexpr uint32_t kMaxDryVesselSafetyTempMk      = 523150u;    // 250.00 C dry element safety limit
inline constexpr uint32_t kTargetPasteurizationHoldSec   = 15u;        // 15 s HTST hold at >= 347150 mK

inline constexpr uint32_t kConductanceLidClosedMwPerK    = 900u;       // 0.90 W/K lidded thermal loss
inline constexpr uint32_t kConductanceLidOpenMwPerK      = 2600u;      // 2.60 W/K unlidded convective/plume loss

struct alignas(8) ThermalVesselRegisters {
    uint32_t core_temp_mk{kAmbientDefaultTempMk}; // Liquid / vessel core temperature in milli-Kelvin
    uint32_t heater_power_mw{0u};                 // Active L2 heating element power in milli-Watts (mJ/s)
    uint32_t liquid_volume_ml{0u};                // Contained aqueous liquid volume in milliliters
    uint32_t pasteurization_hold_s{0u};           // Cumulative seconds held at core_temp_mk >= 347150 mK
    uint32_t target_cutoff_temp_mk{353150u};      // Thermostat cutoff temperature in mK (0 = disabled; default 80.0 C)
    uint8_t  lid_closed{1u};                      // 1 = lid sealed (low cooling/evap), 0 = open pot
    uint8_t  pasteurization_complete{0u};         // 1 once Bigelow pasteurization hold threshold achieved
    uint16_t evap_sub_ml_accum{0u};               // Sub-milliliter evaporation remainder (1/10000 ml units)
    int32_t  energy_sub_mj_accum{0};              // Sub-milli-Kelvin residual energy accumulator (mJ)
    uint32_t lethality_equivalent_ms{0u};         // Bigelow z=5K integrated equivalent hold time in ms
};
static_assert(sizeof(ThermalVesselRegisters) == 32, "ThermalVesselRegisters must be 32 bytes with zero implicit padding");

// Integer piecewise-exponential Bigelow lethality rate in equivalent-ms per real simulated second (z = 5,000 mK).
// At 347,150 mK (74.0 C): 1,000 ms/s (1.0x); at +5,000 mK (79.0 C): 10,000 ms/s (10.0x).
[[nodiscard]] constexpr uint32_t ComputeBigelowLethalityRateMsPerSec(uint32_t core_temp_mk) noexcept {
    if (core_temp_mk < kBigelowPasteurizationTempMk) {
        return 0u;
    }
    const uint32_t over_mk = core_temp_mk - kBigelowPasteurizationTempMk;
    const uint32_t decades = std::min(3u, over_mk / 5000u);
    const uint32_t rem_mk  = (decades >= 3u) ? 0u : (over_mk % 5000u);

    uint32_t decade_mult = 1u;
    for (uint32_t d = 0u; d < decades; ++d) {
        decade_mult *= 10u;
    }
    // Smooth 10^(rem / 5000) integer approximation across [0 .. 5000] mK -> [1000 .. 10000]
    // Using quadratic fit: 1000 + (800 * r)/5000 + (8200 * r^2)/25000000
    const uint64_t r = static_cast<uint64_t>(rem_mk);
    const uint64_t frac_rate = 1000ULL + ((800ULL * r) / 5000ULL) + ((8200ULL * r * r) / 25000000ULL);
    return static_cast<uint32_t>(static_cast<uint64_t>(decade_mult) * frac_rate);
}

namespace detail {

inline void StepThermalVesselSubchunk(
    ThermalVesselRegisters& vessel,
    uint32_t ambient_temp_mk,
    uint32_t chunk_dt_s
) noexcept {
    if (chunk_dt_s == 0u) {
        return;
    }

    // 1. Thermostat gating on heater element
    const bool thermostat_allows_heat = (vessel.target_cutoff_temp_mk == 0u) ||
                                        (vessel.core_temp_mk < vessel.target_cutoff_temp_mk);
    const uint64_t active_heater_mw = thermostat_allows_heat
        ? static_cast<uint64_t>(vessel.heater_power_mw)
        : 0ULL;

    // 2. Ambient Newtonian cooling (or warming if vessel is colder than ambient)
    const uint64_t conductance_mw_per_k = (vessel.lid_closed != 0u)
        ? static_cast<uint64_t>(kConductanceLidClosedMwPerK)
        : static_cast<uint64_t>(kConductanceLidOpenMwPerK);

    int64_t net_power_mw = static_cast<int64_t>(active_heater_mw);
    if (vessel.core_temp_mk >= ambient_temp_mk) {
        const uint64_t delta_mk = static_cast<uint64_t>(vessel.core_temp_mk - ambient_temp_mk);
        const uint64_t loss_mw  = (delta_mk * conductance_mw_per_k) / 1000ULL;
        net_power_mw -= static_cast<int64_t>(loss_mw);
    } else {
        const uint64_t delta_mk = static_cast<uint64_t>(ambient_temp_mk - vessel.core_temp_mk);
        const uint64_t gain_mw  = (delta_mk * conductance_mw_per_k) / 1000ULL;
        net_power_mw += static_cast<int64_t>(gain_mw);
    }

    // 3. Specific-heat temperature evolution: c_p = 4184 mJ / (ml * K)
    // Energy for 1 K (1000 mK) = C_mJ_per_K; Energy for 1 mK = C_mJ_per_K / 1000.
    const uint64_t heat_cap_mj_per_k = (vessel.liquid_volume_ml > 0u)
        ? (kWaterSpecificHeatMjPerMlK * static_cast<uint64_t>(vessel.liquid_volume_ml))
        : kDryVesselHeatCapacityMjPerK;

    const int64_t total_energy_mj = static_cast<int64_t>(vessel.energy_sub_mj_accum) +
                                    (net_power_mw * static_cast<int64_t>(chunk_dt_s));

    // Convert total_energy_mj * 1000 / heat_cap_mj_per_k -> delta_temp_mk while preserving sub-mK remainder
    const int64_t scaled_num = total_energy_mj * 1000LL;
    const int64_t cap_signed = static_cast<int64_t>(heat_cap_mj_per_k);
    const int64_t delta_mk   = scaled_num / cap_signed;
    const int64_t rem_scaled = scaled_num % cap_signed;
    vessel.energy_sub_mj_accum = static_cast<int32_t>(rem_scaled / 1000LL);

    int64_t next_temp_mk = static_cast<int64_t>(vessel.core_temp_mk) + delta_mk;
    if (next_temp_mk < 250000LL) {
        next_temp_mk = 250000LL;
    }

    // Clamp heating overshoot at thermostat cutoff when actively heating toward cutoff
    if (thermostat_allows_heat &&
        vessel.target_cutoff_temp_mk > 0u &&
        delta_mk > 0 &&
        next_temp_mk > static_cast<int64_t>(vessel.target_cutoff_temp_mk)) {
        next_temp_mk = static_cast<int64_t>(vessel.target_cutoff_temp_mk);
        vessel.energy_sub_mj_accum = 0;
    }

    // 4. Boiling phase-change clamp (373,150 mK) & convective evaporation
    if (vessel.liquid_volume_ml > 0u) {
        if (next_temp_mk > static_cast<int64_t>(kBoilingPointWaterMk)) {
            const uint64_t excess_mk = static_cast<uint64_t>(next_temp_mk - static_cast<int64_t>(kBoilingPointWaterMk));
            const uint64_t boil_energy_mj = (excess_mk * heat_cap_mj_per_k) / 1000ULL;
            const uint64_t boil_loss_sub_ml = (boil_energy_mj * 10000ULL) / kLatentHeatVaporizationMjPerMl;
            const uint64_t accum_sub = static_cast<uint64_t>(vessel.evap_sub_ml_accum) + boil_loss_sub_ml;
            const uint32_t boil_ml = static_cast<uint32_t>(accum_sub / 10000ULL);
            vessel.evap_sub_ml_accum = static_cast<uint16_t>(accum_sub % 10000ULL);
            vessel.liquid_volume_ml = (boil_ml >= vessel.liquid_volume_ml) ? 0u : (vessel.liquid_volume_ml - boil_ml);
            next_temp_mk = static_cast<int64_t>(kBoilingPointWaterMk);
        }

        vessel.core_temp_mk = static_cast<uint32_t>(next_temp_mk);

        // Sub-boiling surface evaporation above 50 C (85% condensed & returned when lid_closed == 1)
        if (vessel.core_temp_mk > kEvaporationOnsetTempMk && vessel.liquid_volume_ml > 0u) {
            const uint32_t hot_k = (vessel.core_temp_mk - kEvaporationOnsetTempMk) / 1000u;
            const uint32_t lid_factor = (vessel.lid_closed != 0u) ? 15u : 100u; // 1/10000 ml per s scaling
            const uint64_t evap_sub = static_cast<uint64_t>(vessel.evap_sub_ml_accum) +
                ((static_cast<uint64_t>(hot_k) * static_cast<uint64_t>(lid_factor) * static_cast<uint64_t>(chunk_dt_s)) / 5ULL);
            const uint32_t lost_ml = static_cast<uint32_t>(evap_sub / 10000ULL);
            vessel.evap_sub_ml_accum = static_cast<uint16_t>(evap_sub % 10000ULL);
            vessel.liquid_volume_ml = (lost_ml >= vessel.liquid_volume_ml) ? 0u : (vessel.liquid_volume_ml - lost_ml);
        }
    } else {
        vessel.core_temp_mk = static_cast<uint32_t>(
            std::min<int64_t>(next_temp_mk, static_cast<int64_t>(kMaxDryVesselSafetyTempMk))
        );
    }

    // 5. Bigelow lethality hold accumulation whenever liquid is held at core_temp_mk >= 347,150 mK (74.0 C)
    if (vessel.liquid_volume_ml > 0u && vessel.core_temp_mk >= kBigelowPasteurizationTempMk) {
        vessel.pasteurization_hold_s += chunk_dt_s;
        const uint64_t eq_ms = static_cast<uint64_t>(vessel.lethality_equivalent_ms) +
            (static_cast<uint64_t>(ComputeBigelowLethalityRateMsPerSec(vessel.core_temp_mk)) *
             static_cast<uint64_t>(chunk_dt_s));
        vessel.lethality_equivalent_ms = (eq_ms > 0xFFFFFFFFULL) ? 0xFFFFFFFFu : static_cast<uint32_t>(eq_ms);
        if (vessel.pasteurization_hold_s >= kTargetPasteurizationHoldSec ||
            vessel.lethality_equivalent_ms >= (kTargetPasteurizationHoldSec * 1000u)) {
            vessel.pasteurization_complete = 1u;
        }
    }
}

} // namespace detail

// Pure-integer thermodynamic vessel step (sub-steps larger intervals to preserve thermostat & Bigelow hold accuracy).
inline void StepThermalVessel(
    ThermalVesselRegisters& vessel,
    uint32_t ambient_temp_mk,
    uint32_t sim_dt_s
) noexcept {
    if (sim_dt_s == 0u) {
        return;
    }
    // Use 1-second sub-steps up to 600 s; cap total iterations to <= 600 for huge fast-forward intervals.
    const uint32_t max_sub_dt_s = (sim_dt_s <= 600u) ? 1u : ((sim_dt_s + 599u) / 600u);
    uint32_t remaining_s = sim_dt_s;
    while (remaining_s > 0u) {
        const uint32_t step_s = std::min(remaining_s, max_sub_dt_s);
        detail::StepThermalVesselSubchunk(vessel, ambient_temp_mk, step_s);
        remaining_s -= step_s;
    }
}

} // namespace oasis::engine
