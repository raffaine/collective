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

// =============================================================================
// Skill-Tiered Agronomy Diagnostics, Biotic/Edaphic Stressors & Variable Yield
// =============================================================================

enum class AgronomySkillTier : uint8_t {
    NOVICE       = 1u, // Coarse visual check only; reports soil color/texture, cannot read N/P/K chemistry or 10cm thermal flow
    EXPERIENCED  = 2u, // Identifies foliar nutrient chlorosis, soil crumb insulation/drainage, and early pests
    L2_AUTOMATED = 3u  // Direct FDR soil_probe_v1 + irrigation_valve_v1 closed-loop register telemetry
};

enum class GardenIssueCode : uint8_t {
    NONE                 = 0u,
    DROUGHT_WILTING      = 1u, // Root-zone VWC < 2300 bp causing turgor loss & stomatal closure (neglected timed watering)
    WEED_COMPETITION     = 2u, // Bindweed/crabgrass siphoning water & mineral N-K
    PEST_APHID_BLIGHT    = 3u, // Pea aphids / foliar blight attacking shoots & stipules (high risk in monoculture without hens)
    NUTRIENT_DEFICIENCY  = 4u, // Mineral N/P/K depletion causing chlorosis (invisible to Novice!)
    SOIL_INSULATION_FLOW = 5u, // Low organic carbon (< 1800 bp) from degraded suburban soil (invisible to Novice!)
    CROP_FAILURE_BLIGHT  = 6u, // Severe unaddressed stress collapse (health <= 2000 bp) -> requires clearing & re-sow
    OVERWATERED_ROOT_ROT = 7u  // Root-zone VWC >= 5400 bp from overly proactive manual watering -> hypoxia & root rot
};

// Degraded suburban soil observation (color & texture for Novice Tier 1; tilth/compaction for Experienced Tier 2)
enum class SoilColorObservation : uint8_t {
    UNOBSERVED       = 0u,
    PALE_GREY_BROWN  = 1u, // Typical leached suburban turf subsoil
    YELLOW_CLAY_SUB  = 2u, // Developer earthworks compacted clay fill
    DARK_HUMUS_LOAM  = 3u  // Amended biochar-compost loam
};

enum class SoilTextureObservation : uint8_t {
    UNOBSERVED           = 0u,
    COMPACTED_CLAY_CLODS = 1u, // Heavy developer earthworks compaction + shallow grass roots
    SANDY_LEACHED_GRIT   = 2u, // Fast-draining low-retention fill
    CRUMBLY_AGGREGATE    = 3u  // Well-aggregatedfriable garden tilth
};

enum class HarvestOutcomeTier : uint8_t {
    CROP_FAILURE     = 0u, // 0x yield — dead/blighted vines must be cleared and re-sown
    POOR_STUNTED     = 1u, // ~0.5x yield — neglected checks, weeds/pests, or unaddressed nutrient/insulation stress
    STANDARD_NOMINAL = 2u, // 1.0x nominal cultivar yield — attentive care
    BUMPER_ABUNDANT  = 3u  // ~1.35x-1.5x yield (+1 legume & +1 mirepoix batch) — high-skill or L2 care
};

struct alignas(8) GardenBedHealthState {
    uint32_t          crop_health_bp{8800u};        // Biological vigor [0 .. 10000] bp
    uint32_t          care_quality_score_bp{8000u}; // Cumulative care score [0 .. 10000] bp
    uint32_t          weed_pressure_bp{0u};         // Competing weed biomass [0 .. 10000] bp
    uint32_t          pest_pressure_bp{0u};         // Aphid / foliar blight pressure [0 .. 10000] bp
    uint16_t          uncertainty_band_bp{2800u};   // Epistemic outcome uncertainty [150 .. 2800] bp
    uint16_t          remediations_completed{0u};   // Count of resolved BPMN exception tasks
    GardenIssueCode   true_issue{GardenIssueCode::NONE};
    GardenIssueCode   diagnosed_issue{GardenIssueCode::NONE};
    AgronomySkillTier skill_tier{AgronomySkillTier::NOVICE};
    uint8_t           pad_{0u};
};
static_assert(sizeof(GardenBedHealthState) == 24, "GardenBedHealthState must be 24 bytes with zero implicit padding");

// Initializes a raw suburban backyard plot prior to bed amendment:
// years of shallow grass monoculture over developer-compacted earthworks subsoil leaves poor organic carbon & N-P-K.
[[nodiscard]] constexpr SoilCellRegisters MakeDegradedSuburbanSoilBaseline(
    uint32_t ambient_temp_mk = 288150u
) noexcept {
    SoilCellRegisters s{};
    s.moisture_vwc_bp   = 2900u;
    s.soil_temp_mk      = (ambient_temp_mk > 250000u) ? ambient_temp_mk : 288150u; // 15.0 C default
    s.organic_carbon_bp = 850u;    // 0.85% organic C (poor suburban turf subsoil; below 1800 bp insulation threshold!)
    s.nitrogen_ppm      = 14u;     // Depleted mineral N
    s.phosphorus_ppm    = 15u;     // Depleted available P
    s.potassium_ppm     = 82u;     // Depleted exchangeable K
    s.ph_centi          = 585u;    // Slightly acidic compacted clay-loam (5.85 pH)
    return s;
}

// Applies initial Step 1 soil amendment adjusted by player/agent soil observation & skill curve:
// - Without observation (blind default assumption on suburban trash soil): partial amendment (organic_carbon_bp ~ 1550 bp,
//   leaving a residual SOIL_INSULATION_FLOW / NUTRIENT_DEFICIENCY vulnerability).
// - Novice (Tier 1) reporting observable Soil Color (PALE_GREY_BROWN / YELLOW_CLAY_SUB) + Texture (COMPACTED_CLAY_CLODS):
//   L4 workflow infers heavy suburban earthworks degradation from color/texture and prescribes extra humus (+1050 bp C, +22 ppm N).
// - Experienced (Tier 2) or L2 Automated (Tier 3): calibrated biochar + thermophilic compost prescription fully restores tilth.
inline void ApplySkillCalibratedSuburbanSoilAmendment(
    SoilCellRegisters&     soil,
    AgronomySkillTier      skill_tier,
    SoilColorObservation   color_obs   = SoilColorObservation::PALE_GREY_BROWN,
    SoilTextureObservation texture_obs = SoilTextureObservation::COMPACTED_CLAY_CLODS,
    uint32_t               compost_bags_applied = 1u
) noexcept {
    const uint32_t bag_mult = std::min<uint32_t>(4u, std::max<uint32_t>(1u, compost_bags_applied));
    // Baseline workflow assumption
    AmendSoilWithCompost(soil, 1800u * bag_mult);
    soil.nitrogen_ppm   = std::min(300u, soil.nitrogen_ppm + (12u * bag_mult));
    soil.phosphorus_ppm = std::min(250u, soil.phosphorus_ppm + (8u * bag_mult));
    soil.potassium_ppm  = std::min(500u, soil.potassium_ppm + (22u * bag_mult));

    // Novice color + texture inference adjustment
    if (color_obs == SoilColorObservation::PALE_GREY_BROWN ||
        color_obs == SoilColorObservation::YELLOW_CLAY_SUB ||
        texture_obs == SoilTextureObservation::COMPACTED_CLAY_CLODS ||
        texture_obs == SoilTextureObservation::SANDY_LEACHED_GRIT) {
        soil.organic_carbon_bp = std::min(10000u, soil.organic_carbon_bp + 650u);
        soil.nitrogen_ppm      = std::min(300u, soil.nitrogen_ppm + 10u);
        soil.phosphorus_ppm    = std::min(250u, soil.phosphorus_ppm + 6u);
        soil.potassium_ppm     = std::min(500u, soil.potassium_ppm + 15u);
    }

    // Experienced / L2 calibrated biochar + compost prescription
    if (skill_tier == AgronomySkillTier::EXPERIENCED || skill_tier == AgronomySkillTier::L2_AUTOMATED) {
        AmendSoilWithBiochar(soil, 2500u);
        soil.nitrogen_ppm   = std::max(soil.nitrogen_ppm, 48u);
        soil.phosphorus_ppm = std::max(soil.phosphorus_ppm, 34u);
        soil.potassium_ppm  = std::max(soil.potassium_ppm, 145u);
        soil.ph_centi       = 650u;
    }
}

inline void ApplySkillCalibratedSuburbanSoilAmendment(
    SoilCellRegisters&    soil,
    GardenBedHealthState& health,
    AgronomySkillTier     skill_tier
) noexcept {
    health.skill_tier = skill_tier;
    ApplySkillCalibratedSuburbanSoilAmendment(soil, skill_tier);
    if (skill_tier >= AgronomySkillTier::EXPERIENCED) {
        health.care_quality_score_bp = static_cast<uint16_t>(
            std::min<uint32_t>(9800u, static_cast<uint32_t>(health.care_quality_score_bp) + 400u));
    }
}

inline void ApplySkillCalibratedSuburbanSoilAmendment(
    SoilCellRegisters&    soil,
    GardenBedHealthState& health,
    bool                  has_l2_sensor = false
) noexcept {
    const AgronomySkillTier eff_tier =
        has_l2_sensor ? AgronomySkillTier::L2_AUTOMATED : health.skill_tier;
    ApplySkillCalibratedSuburbanSoilAmendment(soil, health, eff_tier);
}

struct alignas(8) HarvestOutcomeResult {
    uint32_t           legume_batches{0u};
    uint32_t           mirepoix_batches{0u};
    uint16_t           min_expected_legumes{0u};
    uint16_t           max_expected_legumes{0u};
    uint16_t           uncertainty_bp{0u};
    HarvestOutcomeTier outcome_tier{HarvestOutcomeTier::STANDARD_NOMINAL};
    uint8_t            pad_{0u};
};
static_assert(sizeof(HarvestOutcomeResult) == 16, "HarvestOutcomeResult must be 16 bytes with zero implicit padding");

[[nodiscard]] constexpr uint16_t DiagnosticUncertaintyBpForSkill(AgronomySkillTier tier) noexcept {
    switch (tier) {
        case AgronomySkillTier::L2_AUTOMATED: return 150u;  // +-1.5% closed-loop sensor precision
        case AgronomySkillTier::EXPERIENCED:  return 900u;  // +-9.0% experienced foliar/tilth diagnosis
        case AgronomySkillTier::NOVICE:       return 2800u; // +-28.0% coarse novice surface guesswork
    }
    return 2800u;
}

[[nodiscard]] constexpr uint32_t DiagnosticConfidenceBpForSkill(AgronomySkillTier tier) noexcept {
    switch (tier) {
        case AgronomySkillTier::L2_AUTOMATED: return 9850u;
        case AgronomySkillTier::EXPERIENCED:  return 8600u;
        case AgronomySkillTier::NOVICE:       return 5500u;
    }
    return 5500u;
}

[[nodiscard]] constexpr std::string_view GardenIssueCodeId(GardenIssueCode code) noexcept {
    switch (code) {
        case GardenIssueCode::DROUGHT_WILTING:      return "DROUGHT_WILTING";
        case GardenIssueCode::WEED_COMPETITION:     return "WEED_COMPETITION";
        case GardenIssueCode::PEST_APHID_BLIGHT:    return "PEST_APHID_BLIGHT";
        case GardenIssueCode::NUTRIENT_DEFICIENCY:  return "NUTRIENT_DEFICIENCY";
        case GardenIssueCode::SOIL_INSULATION_FLOW: return "SOIL_INSULATION_FLOW";
        case GardenIssueCode::CROP_FAILURE_BLIGHT:  return "CROP_FAILURE_BLIGHT";
        case GardenIssueCode::OVERWATERED_ROOT_ROT: return "OVERWATERED_ROOT_ROT";
        case GardenIssueCode::NONE:                 return "NONE";
    }
    return "NONE";
}

[[nodiscard]] constexpr std::string_view GardenIssueDisplayName(GardenIssueCode code) noexcept {
    switch (code) {
        case GardenIssueCode::DROUGHT_WILTING:
            return "Drought Stress & Root-Zone Wilting (Neglected Timed Watering)";
        case GardenIssueCode::WEED_COMPETITION:
            return "Competing Bindweed & Suburban Crabgrass Infestation";
        case GardenIssueCode::PEST_APHID_BLIGHT:
            return "Monoculture Pea Aphid Colony & Foliar Blight";
        case GardenIssueCode::NUTRIENT_DEFICIENCY:
            return "Suburban Soil N-P-K Deficiency (Foliar Chlorosis)";
        case GardenIssueCode::SOIL_INSULATION_FLOW:
            return "Poor Suburban Soil Insulation & Water/Nutrient Flow (Low Biochar Carbon)";
        case GardenIssueCode::CROP_FAILURE_BLIGHT:
            return "Crop Failure — Severe Wilt / Blight Collapse (Re-Sow Required)";
        case GardenIssueCode::OVERWATERED_ROOT_ROT:
            return "Over-Watered Root Rot & Hypoxia (Excessive Proactive Watering)";
        case GardenIssueCode::NONE:
            return "Healthy Canopy & Balanced Root Zone";
    }
    return "Healthy Canopy & Balanced Root Zone";
}

[[nodiscard]] constexpr std::string_view HarvestOutcomeTierLabel(HarvestOutcomeTier tier) noexcept {
    switch (tier) {
        case HarvestOutcomeTier::CROP_FAILURE:     return "Crop Failure (0x Yield — Re-Sow Required)";
        case HarvestOutcomeTier::POOR_STUNTED:     return "Poor / Stunted Harvest (~0.5x Yield)";
        case HarvestOutcomeTier::STANDARD_NOMINAL: return "Standard Healthy Harvest (1.0x Nominal Yield)";
        case HarvestOutcomeTier::BUMPER_ABUNDANT:  return "Bumper Certifiable Harvest (~1.4x Abundant Yield)";
    }
    return "Standard Healthy Harvest (1.0x Nominal Yield)";
}

// Evaluates physical ground-truth stressors in the garden bed.
[[nodiscard]] constexpr GardenIssueCode EvaluateGardenBedTrueIssue(
    const SoilCellRegisters& soil,
    const CultivarPhysicsSpec& cultivar,
    uint32_t crop_health_bp,
    uint32_t weed_pressure_bp,
    uint32_t pest_pressure_bp
) noexcept {
    if (soil.crop_planted == 0u) {
        return GardenIssueCode::NONE;
    }
    if (crop_health_bp <= 2000u) {
        return GardenIssueCode::CROP_FAILURE_BLIGHT;
    }
    if (soil.moisture_vwc_bp >= 5400u) {
        return GardenIssueCode::OVERWATERED_ROOT_ROT;
    }
    if (soil.moisture_vwc_bp < 2300u) {
        return GardenIssueCode::DROUGHT_WILTING;
    }
    if (pest_pressure_bp >= 3200u) {
        return GardenIssueCode::PEST_APHID_BLIGHT;
    }
    if (weed_pressure_bp >= 3200u) {
        return GardenIssueCode::WEED_COMPETITION;
    }
    // Soil structure & thermal/hydraulic insulation bottleneck: low organic_carbon_bp (< 1800 bp)
    // fails to insulate roots in cool weather or retain capillary water/nutrient flow
    if (soil.organic_carbon_bp < 1800u &&
        (soil.soil_temp_mk < cultivar.opt_low_temp_mk || soil.moisture_vwc_bp < cultivar.target_min_vwc_bp + 200u)) {
        return GardenIssueCode::SOIL_INSULATION_FLOW;
    }
    // Individual nutrient deficiency: mineral N draw in non-nodulating cultivars or P/K depletion
    const uint32_t min_n = (cultivar.fixes_atmospheric_n2 != 0u) ? 18u : 32u;
    if (soil.nitrogen_ppm < min_n || soil.phosphorus_ppm < 18u || soil.potassium_ppm < 90u) {
        return GardenIssueCode::NUTRIENT_DEFICIENCY;
    }
    return GardenIssueCode::NONE;
}

// Epistemic Skill Filter:
// - NOVICE (Tier 1) can recognize a dead/diseased crop (CROP_FAILURE_BLIGHT, PEST_APHID_BLIGHT),
//   obvious surface weeds (WEED_COMPETITION), severe wilting (DROUGHT_WILTING), or pooling mud (OVERWATERED_ROOT_ROT),
//   but CANNOT identify individual nutrient deficiencies (NUTRIENT_DEFICIENCY) or
//   soil thermal insulation / water-nutrient flow issues (SOIL_INSULATION_FLOW)!
// - EXPERIENCED (Tier 2) and L2_AUTOMATED (Tier 3) accurately identify all 7 issue classes.
[[nodiscard]] constexpr GardenIssueCode FilterIssueByEpistemicSkill(
    GardenIssueCode   true_issue,
    AgronomySkillTier skill_tier
) noexcept {
    if (skill_tier == AgronomySkillTier::NOVICE) {
        if (true_issue == GardenIssueCode::NUTRIENT_DEFICIENCY ||
            true_issue == GardenIssueCode::SOIL_INSULATION_FLOW) {
            return GardenIssueCode::NONE; // Novice sees pale/slow plants but cannot diagnose soil/nutrient root cause!
        }
    }
    return true_issue;
}

inline void StepGardenBedHealthAndStressors(
    SoilCellRegisters&         soil,
    GardenBedHealthState&      health,
    const CultivarPhysicsSpec& cultivar,
    uint32_t                   sim_dt_s,
    uint8_t                    polyculture_crop_count = 1u,
    uint8_t                    foraging_hens_present  = 0u
) noexcept {
    health.uncertainty_band_bp = DiagnosticUncertaintyBpForSkill(health.skill_tier);
    if (soil.crop_planted == 0u || sim_dt_s == 0u) {
        return;
    }

    // 1. Natural suburban turf-weed emergence in warm moist topsoil when unweeded (~35 bp/hr)
    if (soil.moisture_vwc_bp >= 2500u && soil.soil_temp_mk >= 283150u && health.weed_pressure_bp < 10000u) {
        const uint32_t weed_gain = static_cast<uint32_t>((35ULL * static_cast<uint64_t>(sim_dt_s)) / 3600ULL);
        health.weed_pressure_bp = std::min(10000u, health.weed_pressure_bp + weed_gain);
    }

    // 2. Ecological pest emergence: high risk in single-crop monoculture surrounded by suburban grass;
    //    reduced by companion polyculture diversity and foraging heritage hens!
    if (soil.crop_growth_bp >= 1500u && soil.soil_temp_mk >= 284150u && health.pest_pressure_bp < 10000u) {
        uint32_t base_pest_bp_per_hr = (polyculture_crop_count <= 1u) ? 75u : 28u;
        if (foraging_hens_present != 0u) {
            base_pest_bp_per_hr = base_pest_bp_per_hr / 3u;
        }
        const uint32_t pest_gain = static_cast<uint32_t>(
            (static_cast<uint64_t>(base_pest_bp_per_hr) * static_cast<uint64_t>(sim_dt_s)) / 3600ULL
        );
        health.pest_pressure_bp = std::min(10000u, health.pest_pressure_bp + pest_gain);
    }

    // Competing weeds siphon soil moisture and mineral nitrogen
    if (health.weed_pressure_bp >= 3200u) {
        const uint32_t siphon_bp = static_cast<uint32_t>((45ULL * static_cast<uint64_t>(sim_dt_s)) / 3600ULL);
        soil.moisture_vwc_bp = (soil.moisture_vwc_bp > 700u + siphon_bp) ? (soil.moisture_vwc_bp - siphon_bp) : 700u;
    }

    health.true_issue      = EvaluateGardenBedTrueIssue(soil, cultivar, health.crop_health_bp, health.weed_pressure_bp, health.pest_pressure_bp);
    health.diagnosed_issue = FilterIssueByEpistemicSkill(health.true_issue, health.skill_tier);

    // Health & care score drift based on active unremediated stressors
    if (health.true_issue != GardenIssueCode::NONE) {
        uint32_t loss_per_hr_bp = 110u;
        if (health.true_issue == GardenIssueCode::DROUGHT_WILTING ||
            health.true_issue == GardenIssueCode::PEST_APHID_BLIGHT ||
            health.true_issue == GardenIssueCode::OVERWATERED_ROOT_ROT) {
            loss_per_hr_bp = 180u;
        }
        const uint32_t delta_loss = static_cast<uint32_t>(
            (static_cast<uint64_t>(loss_per_hr_bp) * static_cast<uint64_t>(sim_dt_s)) / 3600ULL
        );
        health.crop_health_bp = (health.crop_health_bp > delta_loss) ? (health.crop_health_bp - delta_loss) : 0u;
        const uint32_t care_loss = delta_loss / 2u;
        health.care_quality_score_bp = (health.care_quality_score_bp > care_loss)
            ? (health.care_quality_score_bp - care_loss)
            : 0u;
        if (health.crop_health_bp <= 2000u) {
            health.true_issue      = GardenIssueCode::CROP_FAILURE_BLIGHT;
            health.diagnosed_issue = GardenIssueCode::CROP_FAILURE_BLIGHT; // Even a novice recognizes a dead/blighted crop!
        }
    } else if (soil.moisture_vwc_bp >= cultivar.target_min_vwc_bp &&
               soil.moisture_vwc_bp <= 5100u &&
               soil.soil_temp_mk >= cultivar.min_growth_temp_mk &&
               health.crop_health_bp < 9600u) {
        const uint32_t recovery = static_cast<uint32_t>((40ULL * static_cast<uint64_t>(sim_dt_s)) / 3600ULL);
        health.crop_health_bp = std::min(9600u, health.crop_health_bp + recovery);
    }
}

inline void ApplyGardenRemediationAction(
    SoilCellRegisters&         soil,
    GardenBedHealthState&      health,
    const CultivarPhysicsSpec& cultivar,
    GardenIssueCode            target_issue
) noexcept {
    const GardenIssueCode effective = (target_issue != GardenIssueCode::NONE) ? target_issue : health.true_issue;
    switch (effective) {
        case GardenIssueCode::DROUGHT_WILTING:
            soil.moisture_vwc_bp   = std::max(soil.moisture_vwc_bp, cultivar.target_opt_vwc_bp + 200u);
            soil.organic_carbon_bp = std::min(10000u, soil.organic_carbon_bp + 300u); // Mulch retention boost
            break;
        case GardenIssueCode::OVERWATERED_ROOT_ROT:
            // Aerate waterlogged root zone & incorporate porous biochar drainage
            soil.moisture_vwc_bp   = cultivar.target_opt_vwc_bp;
            soil.organic_carbon_bp = std::min(10000u, soil.organic_carbon_bp + 400u);
            break;
        case GardenIssueCode::WEED_COMPETITION:
            health.weed_pressure_bp = 0u;
            break;
        case GardenIssueCode::PEST_APHID_BLIGHT:
            health.pest_pressure_bp = 0u;
            break;
        case GardenIssueCode::NUTRIENT_DEFICIENCY:
            AmendSoilWithCompost(soil, 4000u);
            soil.nitrogen_ppm   = std::min(300u, soil.nitrogen_ppm + 35u);
            soil.phosphorus_ppm = std::min(250u, soil.phosphorus_ppm + 20u);
            soil.potassium_ppm  = std::min(500u, soil.potassium_ppm + 45u);
            break;
        case GardenIssueCode::SOIL_INSULATION_FLOW:
            AmendSoilWithBiochar(soil, 4500u); // +1800 bp organic_carbon_bp thermal & hydraulic buffer
            soil.moisture_vwc_bp = std::max(soil.moisture_vwc_bp, cultivar.target_opt_vwc_bp);
            break;
        case GardenIssueCode::CROP_FAILURE_BLIGHT:
            // Clear dead/blighted crop and reset bed for re-sowing
            soil.crop_growth_bp          = 200u;
            soil.moisture_vwc_bp         = cultivar.target_opt_vwc_bp;
            health.weed_pressure_bp      = 0u;
            health.pest_pressure_bp      = 0u;
            health.crop_health_bp        = 8200u;
            health.care_quality_score_bp = 7500u;
            break;
        case GardenIssueCode::NONE:
            break;
    }
    if (effective != GardenIssueCode::CROP_FAILURE_BLIGHT && effective != GardenIssueCode::NONE) {
        health.crop_health_bp        = std::min(10000u, health.crop_health_bp + 1100u);
        health.care_quality_score_bp = std::min(10000u, health.care_quality_score_bp + 750u);
    }
    ++health.remediations_completed;
    health.true_issue      = EvaluateGardenBedTrueIssue(soil, cultivar, health.crop_health_bp, health.weed_pressure_bp, health.pest_pressure_bp);
    health.diagnosed_issue = FilterIssueByEpistemicSkill(health.true_issue, health.skill_tier);
}

[[nodiscard]] constexpr HarvestOutcomeResult EvaluateVariableHarvestYield(
    const CultivarPhysicsSpec& cultivar,
    uint32_t                   crop_health_bp,
    uint32_t                   care_quality_score_bp,
    uint16_t                   uncertainty_band_bp
) noexcept {
    HarvestOutcomeResult res{};
    res.uncertainty_bp = uncertainty_band_bp;

    const uint32_t nom_leg  = std::max<uint32_t>(1u, cultivar.harvest_legume_batches);
    const uint32_t nom_root = std::max<uint32_t>(1u, cultivar.harvest_mirepoix_batches);

    if (crop_health_bp <= 2000u) {
        res.outcome_tier         = HarvestOutcomeTier::CROP_FAILURE;
        res.legume_batches       = 0u;
        res.mirepoix_batches     = 0u;
        res.min_expected_legumes = 0u;
        res.max_expected_legumes = (uncertainty_band_bp >= 2000u) ? 1u : 0u;
        return res;
    }

    const uint32_t composite_bp = (crop_health_bp * 55u + care_quality_score_bp * 45u) / 100u;

    if (composite_bp < 6200u || crop_health_bp < 5800u) {
        res.outcome_tier     = HarvestOutcomeTier::POOR_STUNTED;
        res.legume_batches   = std::max<uint32_t>(1u, nom_leg / 2u);
        res.mirepoix_batches = (nom_root > 1u) ? (nom_root - 1u) : 0u;
    } else if (composite_bp >= 9050u && crop_health_bp >= 9000u && care_quality_score_bp >= 9000u) {
        res.outcome_tier     = HarvestOutcomeTier::BUMPER_ABUNDANT;
        res.legume_batches   = nom_leg + 1u;
        res.mirepoix_batches = nom_root + 1u;
    } else {
        res.outcome_tier     = HarvestOutcomeTier::STANDARD_NOMINAL;
        res.legume_batches   = nom_leg;
        res.mirepoix_batches = nom_root;
    }

    // Epistemic expected yield range before harvest reflects skill uncertainty band
    if (uncertainty_band_bp >= 2000u) {
        // Novice: wide expected range (e.g. 1..nom+1)
        res.min_expected_legumes = static_cast<uint16_t>((res.legume_batches > 1u) ? (res.legume_batches - 1u) : 0u);
        res.max_expected_legumes = static_cast<uint16_t>(nom_leg + 1u);
    } else if (uncertainty_band_bp >= 600u) {
        // Experienced: tight expected range
        res.min_expected_legumes = static_cast<uint16_t>(std::max<uint32_t>(1u, res.legume_batches));
        res.max_expected_legumes = static_cast<uint16_t>(std::min<uint32_t>(nom_leg + 1u, res.legume_batches + 1u));
    } else {
        // L2 Automated: deterministic pinpoint expectation
        res.min_expected_legumes = static_cast<uint16_t>(res.legume_batches);
        res.max_expected_legumes = static_cast<uint16_t>(res.legume_batches);
    }

    return res;
}

} // namespace oasis::engine

