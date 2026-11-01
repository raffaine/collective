#pragma once

#include <algorithm>
#include <cstdint>

namespace oasis::engine {

// Canonical soil constants (100% pure integer fixed-point: basis points, ppm, milli-Kelvin)
inline constexpr uint32_t kSoilMaxVwcBp                = 10000u;
inline constexpr uint32_t kSoilMinFieldCapacityBp      = 2500u; // Unamended degraded mineral soil (25.00% VWC)
inline constexpr uint32_t kSoilMaxFieldCapacityBp      = 4800u; // Biochar-rich Terra Preta (48.00% VWC)
inline constexpr uint32_t kSoilRootZoneVolumeMl        = 250000u; // 1.0m x 1.0m x 0.25m root zone = 250 L
inline constexpr uint32_t kSoilWiltingPointBp          = 600u;
inline constexpr uint32_t kSoilWaterloggedPointBp      = 9200u;

inline constexpr uint32_t kFreezingTempMk              = 273150u; // 0.00 C
inline constexpr uint32_t kCropMinGrowthTempMk         = 277150u; // 4.00 C
inline constexpr uint32_t kCropOptLowTempMk            = 288150u; // 15.00 C
inline constexpr uint32_t kCropOptHighTempMk           = 301150u; // 28.00 C
inline constexpr uint32_t kCropMaxGrowthTempMk         = 313150u; // 40.00 C
inline constexpr uint32_t kColdSnapThresholdMk         = 285150u; // 12.00 C

inline constexpr uint32_t kCropMatureBp                = 10000u;
inline constexpr uint32_t kNominalMaturitySimSeconds   = 43200u;  // 12 sim-hours under ideal 1.0x conditions

struct alignas(8) SoilCellRegisters {
    uint32_t moisture_vwc_bp{2800u};     // Volumetric Water Content [0 .. 10000] bp
    uint32_t soil_temp_mk{291150u};      // Root-zone temperature in milli-Kelvin (18.0 C default)
    uint32_t nitrogen_ppm{45u};          // Plant-available mineral N (NO3- + NH4+) in ppm
    uint32_t phosphorus_ppm{35u};        // Available P in ppm
    uint32_t potassium_ppm{160u};        // Exchangeable K in ppm
    uint32_t organic_carbon_bp{1500u};   // Organic carbon / Terra Preta biochar fraction [0 .. 10000] bp
    uint32_t ph_centi{650u};             // Soil pH * 100 (650 = pH 6.50)
    uint32_t crop_growth_bp{0u};         // Vegetative -> reproductive maturity [0 .. 10000] bp
    uint8_t  is_legume_fixing_n{0u};     // 1 when Rhizobium-nodulated legume actively fixes N2
    uint8_t  crop_planted{1u};           // 1 when seeded/planted, 0 when fallow
    uint16_t growth_sub_accum{0u};       // Sub-basis-point remainder accumulator for time-step invariance
    uint16_t nitrogen_sub_accum{0u};     // Sub-ppm remainder accumulator for N fixation / uptake
    uint16_t moisture_sub_accum{0u};     // Sub-basis-point remainder accumulator for drip & ET
};
static_assert(sizeof(SoilCellRegisters) == 40, "SoilCellRegisters must be 40 bytes with zero implicit padding");

// Biochar / Terra Preta amendment increases field water-holding capacity from 2500 bp to 4800 bp.
[[nodiscard]] constexpr uint32_t ComputeSoilFieldCapacityBp(uint32_t organic_carbon_bp) noexcept {
    const uint32_t clamped_oc = std::min(organic_carbon_bp, 10000u);
    const uint32_t span_bp    = kSoilMaxFieldCapacityBp - kSoilMinFieldCapacityBp; // 2300 bp
    return kSoilMinFieldCapacityBp + ((clamped_oc * span_bp) / 10000u);
}

// Thermal relaxation time constant (seconds): higher organic carbon + moisture buffers against cold snaps.
[[nodiscard]] constexpr uint32_t ComputeSoilThermalTauSeconds(
    uint32_t organic_carbon_bp,
    uint32_t moisture_vwc_bp
) noexcept {
    const uint32_t clamped_oc  = std::min(organic_carbon_bp, 10000u);
    const uint32_t clamped_vwc = std::min(moisture_vwc_bp, 10000u);
    return 3600u + ((clamped_oc * 10800u) / 10000u) + ((clamped_vwc * 3600u) / 10000u);
}

[[nodiscard]] constexpr uint32_t EvaluateMoistureGrowthFactorBp(
    uint32_t moisture_vwc_bp,
    uint32_t field_capacity_bp
) noexcept {
    if (moisture_vwc_bp <= kSoilWiltingPointBp || moisture_vwc_bp >= kSoilWaterloggedPointBp) {
        return 0u;
    }
    const uint32_t opt_low  = 1800u;
    const uint32_t opt_high = std::max(4500u, field_capacity_bp + 800u);
    if (moisture_vwc_bp < opt_low) {
        return ((moisture_vwc_bp - kSoilWiltingPointBp) * 10000u) / (opt_low - kSoilWiltingPointBp);
    }
    if (moisture_vwc_bp <= opt_high) {
        return 10000u;
    }
    return ((kSoilWaterloggedPointBp - moisture_vwc_bp) * 10000u) / (kSoilWaterloggedPointBp - opt_high);
}

[[nodiscard]] constexpr uint32_t EvaluateTemperatureGrowthFactorBp(uint32_t soil_temp_mk) noexcept {
    if (soil_temp_mk <= kCropMinGrowthTempMk || soil_temp_mk >= kCropMaxGrowthTempMk) {
        return 0u;
    }
    if (soil_temp_mk < kCropOptLowTempMk) {
        return ((soil_temp_mk - kCropMinGrowthTempMk) * 10000u) / (kCropOptLowTempMk - kCropMinGrowthTempMk);
    }
    if (soil_temp_mk <= kCropOptHighTempMk) {
        return 10000u;
    }
    return ((kCropMaxGrowthTempMk - soil_temp_mk) * 10000u) / (kCropMaxGrowthTempMk - kCropOptHighTempMk);
}

[[nodiscard]] constexpr uint32_t EvaluateNutrientAndPhFactorBp(const SoilCellRegisters& soil) noexcept {
    if (soil.ph_centi <= 420u || soil.ph_centi >= 880u) {
        return 0u;
    }
    uint32_t ph_bp = 10000u;
    if (soil.ph_centi < 580u) {
        ph_bp = ((soil.ph_centi - 420u) * 10000u) / 160u;
    } else if (soil.ph_centi > 730u) {
        ph_bp = ((880u - soil.ph_centi) * 10000u) / 150u;
    }

    // Legumes fix atmospheric N2 via Rhizobium symbiosis, reducing reliance on pre-existing mineral N.
    const uint32_t effective_n = (soil.is_legume_fixing_n != 0u)
        ? std::max(soil.nitrogen_ppm, 55u)
        : soil.nitrogen_ppm;

    const uint32_t n_bp = std::min(10000u, (effective_n * 10000u) / 60u);
    const uint32_t p_bp = std::min(10000u, (soil.phosphorus_ppm * 10000u) / 30u);
    const uint32_t k_bp = std::min(10000u, (soil.potassium_ppm * 10000u) / 120u);

    const uint32_t npk_min_bp = std::min({n_bp, p_bp, k_bp});
    return static_cast<uint32_t>((static_cast<uint64_t>(npk_min_bp) * static_cast<uint64_t>(ph_bp)) / 10000ULL);
}

inline void AmendSoilWithBiochar(SoilCellRegisters& soil, uint32_t biochar_mass_g) noexcept {
    // 1,000 g of charged biochar incorporated into 250 L root zone boosts organic_carbon_bp by +400 bp and buffers pH toward 660.
    const uint32_t delta_oc_bp = (biochar_mass_g * 400u) / 1000u;
    soil.organic_carbon_bp = std::min(10000u, soil.organic_carbon_bp + delta_oc_bp);
    soil.potassium_ppm = std::min(600u, soil.potassium_ppm + (biochar_mass_g / 250u));
    if (soil.ph_centi < 660u) {
        soil.ph_centi = std::min(660u, soil.ph_centi + std::min(40u, biochar_mass_g / 250u));
    }
}

inline void AmendSoilWithCompost(SoilCellRegisters& soil, uint32_t compost_mass_g) noexcept {
    soil.organic_carbon_bp = std::min(10000u, soil.organic_carbon_bp + ((compost_mass_g * 180u) / 1000u));
    soil.nitrogen_ppm      = std::min(400u, soil.nitrogen_ppm + ((compost_mass_g * 12u) / 1000u));
    soil.phosphorus_ppm    = std::min(300u, soil.phosphorus_ppm + ((compost_mass_g * 8u) / 1000u));
    soil.potassium_ppm     = std::min(600u, soil.potassium_ppm + ((compost_mass_g * 15u) / 1000u));
}

// Pure-integer soil physics step driven by ambient environment and L2 irrigation / amendment registers.
inline void StepSoilPhysics(
    SoilCellRegisters& soil,
    uint32_t ambient_temp_mk,
    uint32_t added_water_ml,
    uint32_t sim_dt_s
) noexcept {
    soil.moisture_vwc_bp   = std::min(soil.moisture_vwc_bp, kSoilMaxVwcBp);
    soil.organic_carbon_bp = std::min(soil.organic_carbon_bp, 10000u);
    soil.crop_growth_bp    = std::min(soil.crop_growth_bp, kCropMatureBp);

    // 1. Infiltration of newly added water (1 bp VWC = 25 ml across a 250,000 ml root-zone cell)
    if (added_water_ml > 0u) {
        const uint32_t add_bp = added_water_ml / 25u;
        const uint32_t rem_ml = added_water_ml % 25u;
        const uint32_t sub_sum = static_cast<uint32_t>(soil.moisture_sub_accum) + (rem_ml * 400u);
        const uint32_t carry_bp = sub_sum / 10000u;
        soil.moisture_sub_accum = static_cast<uint16_t>(sub_sum % 10000u);
        soil.moisture_vwc_bp = std::min(kSoilMaxVwcBp, soil.moisture_vwc_bp + add_bp + carry_bp);
    }

    if (sim_dt_s == 0u) {
        return;
    }

    const uint32_t field_capacity_bp = ComputeSoilFieldCapacityBp(soil.organic_carbon_bp);

    // 2. Gravity drainage when saturated above field capacity + surface evapotranspiration
    if (soil.moisture_vwc_bp > field_capacity_bp) {
        const uint32_t excess_bp = soil.moisture_vwc_bp - field_capacity_bp;
        const uint32_t drain_tau_s = 1200u + ((soil.organic_carbon_bp * 1800u) / 10000u);
        const uint64_t drain_raw = (static_cast<uint64_t>(excess_bp) * static_cast<uint64_t>(sim_dt_s)) /
                                   static_cast<uint64_t>(drain_tau_s);
        const uint32_t drained_bp = static_cast<uint32_t>(std::min<uint64_t>(excess_bp, std::max<uint64_t>(1ULL, drain_raw)));
        soil.moisture_vwc_bp -= drained_bp;
    }

    // Evapotranspiration scales with warmth above freezing; biochar micropores cut evaporative loss by up to 55%.
    const uint32_t warmth_k = (ambient_temp_mk > kFreezingTempMk)
        ? ((ambient_temp_mk - kFreezingTempMk) / 1000u)
        : 0u;
    const uint32_t base_et_sub_per_s = 8u + (warmth_k * 2u) + ((soil.crop_planted != 0u) ? 10u : 0u); // sub-bp (1/10000 bp) per s
    const uint32_t retention_mult_bp = 10000u - ((soil.organic_carbon_bp * 5500u) / 10000u);
    const uint64_t et_sub_total = (static_cast<uint64_t>(base_et_sub_per_s) *
                                   static_cast<uint64_t>(retention_mult_bp) *
                                   static_cast<uint64_t>(sim_dt_s)) / 10000ULL;
    const uint64_t et_whole_bp = et_sub_total / 100u;
    if (et_whole_bp >= static_cast<uint64_t>(soil.moisture_vwc_bp)) {
        soil.moisture_vwc_bp = 0u;
    } else {
        soil.moisture_vwc_bp -= static_cast<uint32_t>(et_whole_bp);
    }

    // 3. Soil thermal relaxation with biochar dark-earth cold-snap heat retention
    const uint32_t cold_snap_cushion_mk = (ambient_temp_mk < kColdSnapThresholdMk)
        ? ((soil.organic_carbon_bp * 2000u) / 10000u)
        : ((soil.organic_carbon_bp * 400u) / 10000u);
    const uint32_t eq_target_mk = ambient_temp_mk + cold_snap_cushion_mk;
    const uint32_t tau_s = ComputeSoilThermalTauSeconds(soil.organic_carbon_bp, soil.moisture_vwc_bp);

    if (soil.soil_temp_mk < eq_target_mk) {
        const uint32_t diff_mk = eq_target_mk - soil.soil_temp_mk;
        const uint64_t step_mk = (static_cast<uint64_t>(diff_mk) * static_cast<uint64_t>(sim_dt_s)) /
                                 static_cast<uint64_t>(tau_s + sim_dt_s);
        soil.soil_temp_mk += static_cast<uint32_t>(std::min<uint64_t>(diff_mk, step_mk));
    } else if (soil.soil_temp_mk > eq_target_mk) {
        const uint32_t diff_mk = soil.soil_temp_mk - eq_target_mk;
        const uint64_t step_mk = (static_cast<uint64_t>(diff_mk) * static_cast<uint64_t>(sim_dt_s)) /
                                 static_cast<uint64_t>(tau_s + sim_dt_s);
        soil.soil_temp_mk -= static_cast<uint32_t>(std::min<uint64_t>(diff_mk, step_mk));
    }

    // 4. Symbiotic Rhizobium legume nitrogen fixation (+nitrogen_ppm while growing nodulated peas)
    if (soil.is_legume_fixing_n != 0u &&
        soil.moisture_vwc_bp >= 1200u &&
        soil.soil_temp_mk >= kCropMinGrowthTempMk &&
        soil.crop_growth_bp < kCropMatureBp) {
        const uint32_t temp_factor_bp = EvaluateTemperatureGrowthFactorBp(soil.soil_temp_mk);
        // At optimal temperature: +1 ppm N every 600 sim-seconds (6 ppm / sim-hour) up to 260 ppm ceiling
        const uint64_t fixed_sub = static_cast<uint64_t>(soil.nitrogen_sub_accum) +
            ((static_cast<uint64_t>(sim_dt_s) * static_cast<uint64_t>(temp_factor_bp)) / 600ULL);
        const uint32_t gained_ppm = static_cast<uint32_t>(fixed_sub / 10000ULL);
        soil.nitrogen_sub_accum = static_cast<uint16_t>(fixed_sub % 10000ULL);
        soil.nitrogen_ppm = std::min(260u, soil.nitrogen_ppm + gained_ppm);
    }

    // 5. Crop growth & N-P-K uptake as a joint function of moisture, temperature, pH, and nutrients
    if (soil.crop_planted != 0u && soil.crop_growth_bp < kCropMatureBp) {
        const uint32_t f_moist = EvaluateMoistureGrowthFactorBp(soil.moisture_vwc_bp, field_capacity_bp);
        const uint32_t f_temp  = EvaluateTemperatureGrowthFactorBp(soil.soil_temp_mk);
        const uint32_t f_npk   = EvaluateNutrientAndPhFactorBp(soil);

        const uint64_t combined_bp = (static_cast<uint64_t>(f_moist) *
                                      static_cast<uint64_t>(f_temp) *
                                      static_cast<uint64_t>(f_npk)) / 100000000ULL;

        // 10,000 sub-units = 1 bp of crop_growth_bp; ideal 10,000 bp maturity in kNominalMaturitySimSeconds (43,200 s)
        const uint64_t raw_sub_growth = static_cast<uint64_t>(soil.growth_sub_accum) +
            ((static_cast<uint64_t>(sim_dt_s) * combined_bp * 10000ULL) /
             static_cast<uint64_t>(kNominalMaturitySimSeconds));

        const uint32_t prev_growth_bp = soil.crop_growth_bp;
        const uint32_t delta_growth_bp = static_cast<uint32_t>(
            std::min<uint64_t>(kCropMatureBp - prev_growth_bp, raw_sub_growth / 10000ULL)
        );
        soil.growth_sub_accum = static_cast<uint16_t>(raw_sub_growth % 10000ULL);
        soil.crop_growth_bp   = prev_growth_bp + delta_growth_bp;

        // Stoichiometric nutrient uptake every 250 bp (2.5%) of new biomass crossed
        const uint32_t prev_band = prev_growth_bp / 250u;
        const uint32_t next_band = soil.crop_growth_bp / 250u;
        if (next_band > prev_band) {
            const uint32_t bands = next_band - prev_band;
            if (soil.is_legume_fixing_n == 0u) {
                soil.nitrogen_ppm = (soil.nitrogen_ppm > bands) ? (soil.nitrogen_ppm - bands) : 0u;
            }
            const uint32_t p_draw = (bands + 1u) / 2u;
            soil.phosphorus_ppm = (soil.phosphorus_ppm > p_draw) ? (soil.phosphorus_ppm - p_draw) : 0u;
            soil.potassium_ppm  = (soil.potassium_ppm > bands) ? (soil.potassium_ppm - bands) : 0u;
        }
    }
}

} // namespace oasis::engine
