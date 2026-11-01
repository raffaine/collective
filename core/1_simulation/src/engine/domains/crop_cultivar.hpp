#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>

#include "soil_physics.hpp"

namespace oasis::engine {

inline constexpr uint8_t kCultivarCodeChronoVirtualPea     = 0u;
inline constexpr uint8_t kCultivarCodeLittleMarvelPea      = 1u;
inline constexpr uint8_t kCultivarCodeAldermanTelephonePea = 2u;
inline constexpr uint8_t kCultivarCodeKentuckyWonderBean   = 3u;
inline constexpr uint8_t kCultivarCodeUnset                = 0xFFu;

enum class CropPhenologyStage : uint8_t {
    FALLOW               = 0u,
    SOWN_GERMINATING     = 1u,
    VEGETATIVE_CLIMBING  = 2u,
    FLOWERING_POD_FILL   = 3u,
    MATURE_HARVEST_READY = 4u
};

// Pure-integer fixed-point cultivar specification (64 bytes exact, zero implicit padding).
// Parameterizes both SoilPhysics biological growth/resource uptake and L4 BPMN timer intervals.
struct alignas(8) CultivarPhysicsSpec {
    uint32_t maturity_sim_seconds{43200u};        // Ideal 1.0x growth duration from 0 -> 10000 bp
    uint32_t verification_interval_sim_s{10800u}; // BPMN IntermediateCatchEvent timer cadence
    uint32_t min_growth_temp_mk{277150u};         // Lower thermal cutoff (mK)
    uint32_t opt_low_temp_mk{285150u};            // Optimal thermal plateau start (mK)
    uint32_t opt_high_temp_mk{302150u};           // Optimal thermal plateau end (mK)
    uint32_t max_growth_temp_mk{313150u};         // Heat-stress cutoff (mK)
    uint32_t target_min_vwc_bp{3000u};            // Moisture verification trigger threshold (bp)
    uint32_t target_opt_vwc_bp{4200u};            // Target post-irrigation root-zone VWC (bp)
    uint32_t target_max_vwc_bp{4900u};            // Upper optimal moisture bound before hypoxia taper (bp)
    uint32_t irrigation_dose_ml{4500u};           // Per-actuation water dose (ml)
    uint32_t et_multiplier_bp{15000u};            // Evapotranspiration rate multiplier (10000 = 1.0x)
    uint32_t n_fixation_rate_bp{14000u};          // Rhizobium N2 fixation rate multiplier (10000 = 1.0x)
    uint32_t harvest_mature_bp{9500u};            // Maturity threshold to unlock Harvest step (bp)
    uint16_t trellis_ridge_height_mm{1500u};      // Required A-frame ridge height (mm)
    uint16_t sowing_depth_mm{30u};                // Seed placement depth (mm)
    uint8_t  n_draw_per_band_ppm{0u};             // Mineral N draw per 250 bp biomass band (ppm)
    uint8_t  p_draw_per_band_ppm{1u};             // Available P draw per 250 bp biomass band (ppm)
    uint8_t  k_draw_per_band_ppm{1u};             // Exchangeable K draw per 250 bp biomass band (ppm)
    uint8_t  fixes_atmospheric_n2{1u};            // 1 = active Rhizobium symbiosis, 0 = soil-N consumer
    uint8_t  is_virtual_sim_tuned{1u};            // 1 = game-tuned virtual physics, 0 = heritage agronomy
    uint8_t  harvest_legume_batches{2u};          // Harvested dry/fresh legume batches per bed cycle
    uint8_t  harvest_mirepoix_batches{1u};        // Companion understory mirepoix batches per bed cycle
    uint8_t  cultivar_code{kCultivarCodeChronoVirtualPea};
};
static_assert(sizeof(CultivarPhysicsSpec) == 64, "CultivarPhysicsSpec must be 64 bytes with zero implicit padding");

[[nodiscard]] constexpr CultivarPhysicsSpec ResolveCultivarByCode(uint8_t code) noexcept {
    if (code == kCultivarCodeLittleMarvelPea) {
        return CultivarPhysicsSpec{
            172800u, 21600u,
            277150u, 283150u, 295150u, 305150u,
            3200u, 4100u, 4700u, 4500u,
            10000u, 10000u, 9500u,
            900u, 35u,
            0u, 1u, 2u, 1u, 0u, 2u, 2u,
            kCultivarCodeLittleMarvelPea
        };
    }
    if (code == kCultivarCodeAldermanTelephonePea) {
        return CultivarPhysicsSpec{
            604800u, 43200u,
            277150u, 286150u, 297150u, 306150u,
            3400u, 4300u, 4900u, 5500u,
            11000u, 13000u, 9500u,
            1850u, 40u,
            0u, 2u, 2u, 1u, 0u, 3u, 2u,
            kCultivarCodeAldermanTelephonePea
        };
    }
    if (code == kCultivarCodeKentuckyWonderBean) {
        return CultivarPhysicsSpec{
            864000u, 43200u,
            285150u, 291150u, 303150u, 311150u,
            3600u, 4500u, 5100u, 6000u,
            12500u, 0u, 9500u,
            1950u, 25u,
            1u, 2u, 3u, 0u, 0u, 4u, 2u,
            kCultivarCodeKentuckyWonderBean
        };
    }
    // Default: Chrono-Pod Virtual Pea (Pisum virtualis — Sim-Tuned 12h cycle, 3h verification checks)
    return CultivarPhysicsSpec{};
}

[[nodiscard]] constexpr uint8_t ResolveCultivarCodeFromId(std::string_view cultivar_id) noexcept {
    if (cultivar_id == "cultivar:little_marvel_pea" || cultivar_id == "little_marvel_pea") {
        return kCultivarCodeLittleMarvelPea;
    }
    if (cultivar_id == "cultivar:alderman_telephone_pea" || cultivar_id == "alderman_telephone_pea") {
        return kCultivarCodeAldermanTelephonePea;
    }
    if (cultivar_id == "cultivar:kentucky_wonder_bean" || cultivar_id == "kentucky_wonder_bean") {
        return kCultivarCodeKentuckyWonderBean;
    }
    return kCultivarCodeChronoVirtualPea;
}

[[nodiscard]] constexpr std::string_view CultivarIdFromCode(uint8_t code) noexcept {
    switch (code) {
        case kCultivarCodeLittleMarvelPea:      return "cultivar:little_marvel_pea";
        case kCultivarCodeAldermanTelephonePea: return "cultivar:alderman_telephone_pea";
        case kCultivarCodeKentuckyWonderBean:   return "cultivar:kentucky_wonder_bean";
        default:                                return "cultivar:chrono_virtual_pea";
    }
}

[[nodiscard]] constexpr std::string_view CultivarDisplayNameFromCode(uint8_t code) noexcept {
    switch (code) {
        case kCultivarCodeLittleMarvelPea:      return "Little Marvel Early Garden Pea";
        case kCultivarCodeAldermanTelephonePea: return "Alderman Tall Climbing Split Pea";
        case kCultivarCodeKentuckyWonderBean:   return "Kentucky Wonder Heritage Pole Bean";
        default:                                return "Chrono-Pod Virtual Pea";
    }
}

[[nodiscard]] constexpr CropPhenologyStage EvaluateCropPhenologyStage(
    uint32_t crop_growth_bp,
    uint8_t  crop_planted,
    uint32_t harvest_mature_bp = 9500u
) noexcept {
    if (crop_planted == 0u) {
        return CropPhenologyStage::FALLOW;
    }
    if (crop_growth_bp >= harvest_mature_bp) {
        return CropPhenologyStage::MATURE_HARVEST_READY;
    }
    if (crop_growth_bp >= 6800u) {
        return CropPhenologyStage::FLOWERING_POD_FILL;
    }
    if (crop_growth_bp >= 2500u) {
        return CropPhenologyStage::VEGETATIVE_CLIMBING;
    }
    return CropPhenologyStage::SOWN_GERMINATING;
}

[[nodiscard]] constexpr std::string_view PhenologyStageLabel(CropPhenologyStage stage) noexcept {
    switch (stage) {
        case CropPhenologyStage::FALLOW:               return "Fallow";
        case CropPhenologyStage::SOWN_GERMINATING:     return "Germinating Radicle & Shoots";
        case CropPhenologyStage::VEGETATIVE_CLIMBING:  return "Vegetative Tendril Climb";
        case CropPhenologyStage::FLOWERING_POD_FILL:   return "Flowering & Pod Fill";
        case CropPhenologyStage::MATURE_HARVEST_READY: return "Mature Pods Ready for Harvest";
    }
    return "Fallow";
}

[[nodiscard]] constexpr uint32_t EvaluateCultivarTemperatureFactorBp(
    uint32_t soil_temp_mk,
    const CultivarPhysicsSpec& cultivar
) noexcept {
    if (soil_temp_mk <= cultivar.min_growth_temp_mk || soil_temp_mk >= cultivar.max_growth_temp_mk) {
        return 0u;
    }
    if (soil_temp_mk < cultivar.opt_low_temp_mk) {
        const uint32_t span = std::max(1u, cultivar.opt_low_temp_mk - cultivar.min_growth_temp_mk);
        return ((soil_temp_mk - cultivar.min_growth_temp_mk) * 10000u) / span;
    }
    if (soil_temp_mk <= cultivar.opt_high_temp_mk) {
        return 10000u;
    }
    const uint32_t span = std::max(1u, cultivar.max_growth_temp_mk - cultivar.opt_high_temp_mk);
    return ((cultivar.max_growth_temp_mk - soil_temp_mk) * 10000u) / span;
}

[[nodiscard]] constexpr uint32_t EvaluateCultivarMoistureFactorBp(
    uint32_t moisture_vwc_bp,
    uint32_t field_capacity_bp,
    const CultivarPhysicsSpec& cultivar
) noexcept {
    if (moisture_vwc_bp <= kSoilWiltingPointBp || moisture_vwc_bp >= kSoilWaterloggedPointBp) {
        return 0u;
    }
    const uint32_t opt_low  = std::max(kSoilWiltingPointBp + 200u, cultivar.target_min_vwc_bp);
    const uint32_t opt_high = std::max(opt_low + 400u, std::max(cultivar.target_max_vwc_bp, field_capacity_bp + 600u));
    if (moisture_vwc_bp < opt_low) {
        return ((moisture_vwc_bp - kSoilWiltingPointBp) * 10000u) / (opt_low - kSoilWiltingPointBp);
    }
    if (moisture_vwc_bp <= opt_high) {
        return 10000u;
    }
    return ((kSoilWaterloggedPointBp - moisture_vwc_bp) * 10000u) /
           std::max(1u, kSoilWaterloggedPointBp - opt_high);
}

// Cultivar-parameterized pure-integer soil & crop growth step.
inline void StepCultivarSoilPhysics(
    SoilCellRegisters& soil,
    const CultivarPhysicsSpec& cultivar,
    uint32_t ambient_temp_mk,
    uint32_t added_water_ml,
    uint32_t sim_dt_s
) noexcept {
    soil.moisture_vwc_bp    = std::min(soil.moisture_vwc_bp, kSoilMaxVwcBp);
    soil.organic_carbon_bp  = std::min(soil.organic_carbon_bp, 10000u);
    soil.crop_growth_bp     = std::min(soil.crop_growth_bp, kCropMatureBp);
    soil.is_legume_fixing_n = (soil.crop_planted != 0u && cultivar.fixes_atmospheric_n2 != 0u) ? 1u : 0u;

    // 1. Infiltration of newly added water (1 bp VWC = 25 ml across a 250,000 ml root-zone cell)
    if (added_water_ml > 0u) {
        const uint32_t add_bp   = added_water_ml / 25u;
        const uint32_t rem_ml   = added_water_ml % 25u;
        const uint32_t sub_sum  = static_cast<uint32_t>(soil.moisture_sub_accum) + (rem_ml * 400u);
        const uint32_t carry_bp = sub_sum / 10000u;
        soil.moisture_sub_accum = static_cast<uint16_t>(sub_sum % 10000u);
        soil.moisture_vwc_bp    = std::min(kSoilMaxVwcBp, soil.moisture_vwc_bp + add_bp + carry_bp);
    }

    if (sim_dt_s == 0u) {
        return;
    }

    const uint32_t field_capacity_bp = ComputeSoilFieldCapacityBp(soil.organic_carbon_bp);

    // 2. Gravity drainage above field capacity + cultivar-scaled evapotranspiration
    if (soil.moisture_vwc_bp > field_capacity_bp) {
        const uint32_t excess_bp   = soil.moisture_vwc_bp - field_capacity_bp;
        const uint32_t drain_tau_s = 1200u + ((soil.organic_carbon_bp * 1800u) / 10000u);
        const uint64_t drain_raw   = (static_cast<uint64_t>(excess_bp) * static_cast<uint64_t>(sim_dt_s)) /
                                     static_cast<uint64_t>(drain_tau_s);
        const uint32_t drained_bp  = static_cast<uint32_t>(
            std::min<uint64_t>(excess_bp, std::max<uint64_t>(1ULL, drain_raw))
        );
        soil.moisture_vwc_bp -= drained_bp;
    }

    const uint32_t warmth_k = (ambient_temp_mk > kFreezingTempMk)
        ? ((ambient_temp_mk - kFreezingTempMk) / 1000u)
        : 0u;
    const uint32_t plant_transpiration = (soil.crop_planted != 0u)
        ? static_cast<uint32_t>((14ULL * static_cast<uint64_t>(cultivar.et_multiplier_bp)) / 10000ULL)
        : 0u;
    const uint32_t base_et_sub_per_s = 8u + (warmth_k * 2u) + plant_transpiration;
    const uint32_t retention_mult_bp = 10000u - ((soil.organic_carbon_bp * 5500u) / 10000u);
    const uint64_t et_sub_total = (static_cast<uint64_t>(base_et_sub_per_s) *
                                   static_cast<uint64_t>(retention_mult_bp) *
                                   static_cast<uint64_t>(sim_dt_s)) / 10000ULL;
    const uint64_t et_whole_bp = et_sub_total / 1000u;
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
    const uint32_t tau_s        = ComputeSoilThermalTauSeconds(soil.organic_carbon_bp, soil.moisture_vwc_bp);

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

    // 4. Cultivar-parameterized Rhizobium N2 fixation
    if (soil.is_legume_fixing_n != 0u &&
        cultivar.n_fixation_rate_bp > 0u &&
        soil.moisture_vwc_bp >= 1200u &&
        soil.soil_temp_mk >= cultivar.min_growth_temp_mk &&
        soil.crop_growth_bp < kCropMatureBp) {
        const uint32_t temp_factor_bp = EvaluateCultivarTemperatureFactorBp(soil.soil_temp_mk, cultivar);
        const uint64_t scaled_rate = (static_cast<uint64_t>(temp_factor_bp) *
                                      static_cast<uint64_t>(cultivar.n_fixation_rate_bp)) / 10000ULL;
        const uint64_t fixed_sub = static_cast<uint64_t>(soil.nitrogen_sub_accum) +
            ((static_cast<uint64_t>(sim_dt_s) * scaled_rate) / 600ULL);
        const uint32_t gained_ppm = static_cast<uint32_t>(fixed_sub / 10000ULL);
        soil.nitrogen_sub_accum   = static_cast<uint16_t>(fixed_sub % 10000ULL);
        soil.nitrogen_ppm         = std::min(260u, soil.nitrogen_ppm + gained_ppm);
    }

    // 5. Cultivar-parameterized phenological growth & N-P-K stoichiometry
    if (soil.crop_planted != 0u && soil.crop_growth_bp < kCropMatureBp) {
        const uint32_t f_moist = EvaluateCultivarMoistureFactorBp(soil.moisture_vwc_bp, field_capacity_bp, cultivar);
        const uint32_t f_temp  = EvaluateCultivarTemperatureFactorBp(soil.soil_temp_mk, cultivar);
        const uint32_t f_npk   = EvaluateNutrientAndPhFactorBp(soil);

        const uint64_t combined_bp = (static_cast<uint64_t>(f_moist) *
                                      static_cast<uint64_t>(f_temp) *
                                      static_cast<uint64_t>(f_npk)) / 100000000ULL;

        const uint32_t maturity_s = std::max(60u, cultivar.maturity_sim_seconds);
        const uint64_t raw_sub_growth = static_cast<uint64_t>(soil.growth_sub_accum) +
            ((static_cast<uint64_t>(sim_dt_s) * combined_bp * 10000ULL) /
             static_cast<uint64_t>(maturity_s));

        const uint32_t prev_growth_bp  = soil.crop_growth_bp;
        const uint32_t delta_growth_bp = static_cast<uint32_t>(
            std::min<uint64_t>(kCropMatureBp - prev_growth_bp, raw_sub_growth / 10000ULL)
        );
        soil.growth_sub_accum = static_cast<uint16_t>(raw_sub_growth % 10000ULL);
        soil.crop_growth_bp   = prev_growth_bp + delta_growth_bp;

        // Cultivar-specific stoichiometric N-P-K draw every 250 bp (2.5%) of new biomass crossed
        const uint32_t prev_band = prev_growth_bp / 250u;
        const uint32_t next_band = soil.crop_growth_bp / 250u;
        if (next_band > prev_band) {
            const uint32_t bands  = next_band - prev_band;
            const uint32_t n_draw = bands * static_cast<uint32_t>(cultivar.n_draw_per_band_ppm);
            const uint32_t p_draw = bands * static_cast<uint32_t>(cultivar.p_draw_per_band_ppm);
            const uint32_t k_draw = bands * static_cast<uint32_t>(cultivar.k_draw_per_band_ppm);
            soil.nitrogen_ppm   = (soil.nitrogen_ppm > n_draw) ? (soil.nitrogen_ppm - n_draw) : 0u;
            soil.phosphorus_ppm = (soil.phosphorus_ppm > p_draw) ? (soil.phosphorus_ppm - p_draw) : 0u;
            soil.potassium_ppm  = (soil.potassium_ppm > k_draw) ? (soil.potassium_ppm - k_draw) : 0u;
        }
    }
}

} // namespace oasis::engine
