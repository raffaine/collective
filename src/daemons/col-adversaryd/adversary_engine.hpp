#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <queue>
#include <functional>
#include <cmath>
#include <algorithm>
#include <array>
#include <optional>
#include "uhai/uhai_ring_buffer.hpp"

namespace collective::l7 {

// ============================================================================
// 1. Threat Levels & AdversaryConfig (Story 7.1)
// ============================================================================

enum class ThreatLevel : uint8_t {
    LEVEL_0_BENIGN   = 0, // Favorable utility net-metering, minimal building inspections, low banking fees
    LEVEL_1_STANDARD = 1, // Real-world utility rates, standard commercial bank fees, triennial inspections
    LEVEL_2_HOSTILE  = 2, // Aggressive utility rate spikes (300%), building code compliance orders, merchant holds
    LEVEL_3_SIEGE    = 3  // Utility shutoffs, civil injunctions, tax liens, bank account freezes, regulatory raids
};

[[nodiscard]] constexpr std::string_view ThreatLevelToString(ThreatLevel level) noexcept {
    switch (level) {
        case ThreatLevel::LEVEL_0_BENIGN:   return "LEVEL_0_BENIGN";
        case ThreatLevel::LEVEL_1_STANDARD: return "LEVEL_1_STANDARD";
        case ThreatLevel::LEVEL_2_HOSTILE:  return "LEVEL_2_HOSTILE";
        case ThreatLevel::LEVEL_3_SIEGE:    return "LEVEL_3_SIEGE";
    }
    return "UNKNOWN";
}

struct AdversaryConfig {
    ThreatLevel threat_level{ThreatLevel::LEVEL_1_STANDARD};
    uint64_t random_seed{1337};
    double utility_rate_multiplier{1.0};
    bool enable_bank_freezes{false};
    uint32_t simulation_days{90};
    double municipal_fine_multiplier{1.0};
    double backfeed_penalty_usd{0.0};
    double inspection_frequency_multiplier{1.0};

    static AdversaryConfig CreateDefault(ThreatLevel level, uint64_t seed = 1337, uint32_t days = 90) noexcept {
        AdversaryConfig cfg;
        cfg.threat_level = level;
        cfg.random_seed = seed;
        cfg.simulation_days = days;
        switch (level) {
            case ThreatLevel::LEVEL_0_BENIGN:
                cfg.utility_rate_multiplier = 0.8;
                cfg.enable_bank_freezes = false;
                cfg.municipal_fine_multiplier = 0.5;
                cfg.backfeed_penalty_usd = 0.0;
                cfg.inspection_frequency_multiplier = 0.25;
                break;
            case ThreatLevel::LEVEL_1_STANDARD:
                cfg.utility_rate_multiplier = 1.0;
                cfg.enable_bank_freezes = false;
                cfg.municipal_fine_multiplier = 1.0;
                cfg.backfeed_penalty_usd = 25.0;
                cfg.inspection_frequency_multiplier = 0.5;
                break;
            case ThreatLevel::LEVEL_2_HOSTILE:
                cfg.utility_rate_multiplier = 3.0; // 300% surge (Story 7.1)
                cfg.enable_bank_freezes = true;
                cfg.municipal_fine_multiplier = 2.0;
                cfg.backfeed_penalty_usd = 120.0; // $120.00 curtailment (Story 7.2)
                cfg.inspection_frequency_multiplier = 1.0;
                break;
            case ThreatLevel::LEVEL_3_SIEGE:
                cfg.utility_rate_multiplier = 5.0;
                cfg.enable_bank_freezes = true;
                cfg.municipal_fine_multiplier = 5.0;
                cfg.backfeed_penalty_usd = 500.0;
                cfg.inspection_frequency_multiplier = 3.33;
                break;
        }
        return cfg;
    }
};

// ============================================================================
// 2. Adversarial Event Types & 5-State Municipal FSM (Stories 7.2, 7.3, 7.4)
// ============================================================================

enum class AdversaryEventType : uint16_t {
    UNKNOWN = 0,

    // Utility Monopolies (Story 7.2)
    UTILITY_SURPRISE_RATE_ADJUSTMENT = 1,
    SURPRISE_RATE_ADJUSTMENT         = 1,
    UTILITY_BACKFEED_CURTAILMENT     = 2,
    UTILITY_DISCONNECT_WARNING       = 3,
    UTILITY_DISCONNECT_ORDER         = 4,
    UTILITY_SHUTOFF                  = 4,

    // Municipal Code & Zoning Enforcement 5-State FSM (Story 7.3)
    MUNICIPAL_INSPECTION_SCHEDULED   = 10,
    ROUTINE_INSPECTION               = 10,
    MUNICIPAL_CITATION_ISSUED        = 11,
    BUILDING_CODE_NOV                = 11,
    MUNICIPAL_STOP_WORK_ORDER        = 12,
    MUNICIPAL_ABATEMENT_HEARING      = 13,
    MUNICIPAL_LEGAL_SEVERANCE        = 14,

    // Predatory Fiat Banking & Regulatory Siege (Story 7.4)
    BANKING_MERCHANT_RESERVE_HOLD    = 20,
    MERCHANT_RESERVE_HOLD            = 20,
    BANKING_ALGORITHMIC_FREEZE       = 21,
    BANK_ACCOUNT_FREEZE              = 21,
    REGULATORY_DOCUMENT_DEMAND       = 22,
    REGULATORY_CIVIL_INJUNCTION      = 23,
    CIVIL_INJUNCTION                 = 23,
    REGULATORY_TAX_LIEN              = 24,
    MUNICIPAL_TAX_LIEN               = 24,
    REGULATORY_MULTI_AGENCY_RAID     = 25,
    UNANNOUNCED_REGULATORY_RAID      = 25,

    BENIGN_NET_METERING_CREDIT       = 30
};

enum class MunicipalFsmState : uint8_t {
    STATE_IDLE                 = 0,
    STATE_INSPECTION_SCHEDULED = 1,
    STATE_CITATION_ISSUED      = 2,
    STATE_STOP_WORK_ORDER      = 3,
    STATE_ABATEMENT_HEARING    = 4,
    STATE_LEGAL_SEVERANCE      = 5
};

struct AdversaryEvent {
    uint64_t event_id{0};
    uint64_t timestamp_sec{0};            // Simulated calendar seconds from epoch
    uint64_t timestamp_seconds{0};        // Alias matching QE test suite
    double   sim_day{0.0};                // Fractional virtual calendar days (day = timestamp_sec / 86400.0)
    AdversaryEventType type{AdversaryEventType::UNKNOWN};
    AdversaryEventType event_type{AdversaryEventType::UNKNOWN}; // Alias matching QE test suite
    std::string agency_or_monopoly;       // "AmeriGrid", "MetroPower", "City Department", "Stripe"
    std::string description;
    double financial_impact_fiat_usd{0.0};// Civil fines, tariffs, curtailment fees
    double financial_penalty{0.0};        // Alias matching QE test suite
    uint32_t cure_period_days{0};         // 30 days, 14 days
    MunicipalFsmState fsm_state{MunicipalFsmState::STATE_IDLE};

    void SyncAliases() noexcept {
        timestamp_seconds = timestamp_sec;
        event_type = type;
        financial_penalty = financial_impact_fiat_usd;
    }

    // Min-heap ordering: earliest event has highest priority
    bool operator>(const AdversaryEvent& other) const noexcept {
        if (timestamp_sec != other.timestamp_sec) {
            return timestamp_sec > other.timestamp_sec;
        }
        return event_id > other.event_id;
    }
};

// ============================================================================
// 3. Deterministic Seedable PRNG: Split-Stream PCG-XSH-RR (Story 7.1)
// ============================================================================

class PcgXshRr32 {
public:
    constexpr explicit PcgXshRr32(uint64_t seed = 1337ULL, uint64_t stream = 1ULL) noexcept {
        Seed(seed, stream);
    }

    constexpr void Seed(uint64_t seed, uint64_t stream = 1ULL) noexcept {
        state_ = 0;
        inc_ = (stream << 1u) | 1u;
        Next();
        state_ += seed;
        Next();
    }

    constexpr uint32_t Next() noexcept {
        uint64_t oldstate = state_;
        state_ = oldstate * 6364136223846793005ULL + inc_;
        uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
        uint32_t rot = static_cast<uint32_t>(oldstate >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    constexpr double NextDouble() noexcept { // (0.0, 1.0)
        uint32_t val = Next();
        double d = (val >> 8) * (1.0 / 16777216.0);
        return std::clamp(d, 1e-12, 1.0 - 1e-12);
    }

    // Inverse transform sampling for Poisson inter-arrival intervals
    double NextExponential(double mean_interval_days) noexcept {
        if (mean_interval_days <= 0.0) return 1e9;
        double u = NextDouble();
        return -std::log(1.0 - u) * mean_interval_days;
    }

private:
    uint64_t state_{0};
    uint64_t inc_{1};
};

// ============================================================================
// 4. Macroeconomic Distress & Migration Wave Generator (SS-EP7-007)
// ============================================================================

constexpr float MIGRATION_DISTRESS_THRESHOLD = 0.65f;
constexpr uint8_t UNVERIFIED_MIGRANT_TRUST_RING = 6; // Ring R6: Unverified Migrant
constexpr uint16_t MIN_MIGRATION_COHORT_SIZE = 1;
constexpr uint16_t MAX_MIGRATION_COHORT_SIZE = 12;

struct LegacyDistressIndex {
    float cpi_inflation_shock{0.0f};    // Simulated fiat CPI inflation spike [0.0, 1.0]
    float grid_blackout_severity{0.0f}; // Legacy utility grid blackout index [0.0, 1.0]
    float climate_shock_severity{0.0f}; // Regional heat/drought shock index [0.0, 1.0]
    float threat_level_weight{0.0f};    // Monotonic threat level baseline [0.0, 1.0]
    float value{0.0f};                  // Composite D_legacy(t) in [0.0, 1.0]
    uint8_t dominant_trigger_code{1};   // 1=FiatInflation, 2=GridBlackout, 3=ClimateShock
    [[maybe_unused]] uint8_t reserved[3]{0};

    static constexpr float Threshold = MIGRATION_DISTRESS_THRESHOLD;

    constexpr LegacyDistressIndex() noexcept = default;
    constexpr explicit LegacyDistressIndex(float raw_val) noexcept
        : value(SanitizeUnitScalar(raw_val)) {}

    [[nodiscard]] static float SanitizeUnitScalar(float v) noexcept {
        if (std::isnan(v)) return 0.0f;
        if (std::isinf(v)) return (v > 0.0f) ? 1.0f : 0.0f;
        return std::clamp(v, 0.0f, 1.0f);
    }

    [[nodiscard]] static constexpr float ThreatBaselineWeight(ThreatLevel level) noexcept {
        switch (level) {
            case ThreatLevel::LEVEL_0_BENIGN:   return 0.08f;
            case ThreatLevel::LEVEL_1_STANDARD: return 0.42f;
            case ThreatLevel::LEVEL_2_HOSTILE:  return 0.68f;
            case ThreatLevel::LEVEL_3_SIEGE:    return 0.90f;
        }
        return 0.42f;
    }

    [[nodiscard]] static LegacyDistressIndex FromFactors(
        float cpi_spike,
        float grid_blackout,
        ThreatLevel level,
        float climate_shock = 0.0f) noexcept {
        LegacyDistressIndex idx{};
        idx.cpi_inflation_shock = SanitizeUnitScalar(cpi_spike);
        idx.grid_blackout_severity = SanitizeUnitScalar(grid_blackout);
        idx.climate_shock_severity = SanitizeUnitScalar(climate_shock);
        idx.threat_level_weight = ThreatBaselineWeight(level);

        const float raw_composite =
            0.40f * idx.threat_level_weight +
            0.30f * idx.cpi_inflation_shock +
            0.20f * idx.grid_blackout_severity +
            0.10f * idx.climate_shock_severity;
        idx.value = SanitizeUnitScalar(raw_composite);

        if (idx.grid_blackout_severity > idx.cpi_inflation_shock &&
            idx.grid_blackout_severity >= idx.climate_shock_severity) {
            idx.dominant_trigger_code = static_cast<uint8_t>(oasis::uhai::MigrationTriggerReason::GRID_BLACKOUT);
        } else if (idx.climate_shock_severity > idx.cpi_inflation_shock &&
                   idx.climate_shock_severity > idx.grid_blackout_severity) {
            idx.dominant_trigger_code = static_cast<uint8_t>(oasis::uhai::MigrationTriggerReason::CLIMATE_SHOCK);
        } else {
            idx.dominant_trigger_code = static_cast<uint8_t>(oasis::uhai::MigrationTriggerReason::FIAT_INFLATION);
        }
        return idx;
    }

    [[nodiscard]] static LegacyDistressIndex Evaluate(
        ThreatLevel level,
        uint64_t random_seed,
        double sim_day,
        float cpi_override = -1.0f,
        float blackout_override = -1.0f,
        float climate_override = -1.0f) noexcept {
        const double clamped_day = (std::isnan(sim_day) || std::isinf(sim_day) || sim_day < 0.0) ? 0.0 : sim_day;
        const uint64_t day_bucket = static_cast<uint64_t>(clamped_day * 10.0);
        PcgXshRr32 prng(random_seed ^ (day_bucket * 0x9E3779B97F4A7C15ULL), 77ULL);

        float level_scale = 0.0f;
        float level_floor = 0.0f;
        switch (level) {
            case ThreatLevel::LEVEL_0_BENIGN:
                level_scale = 0.20f;
                level_floor = 0.05f;
                break;
            case ThreatLevel::LEVEL_1_STANDARD:
                level_scale = 0.62f;
                level_floor = 0.42f;
                break;
            case ThreatLevel::LEVEL_2_HOSTILE:
                level_scale = 0.85f;
                level_floor = 0.66f;
                break;
            case ThreatLevel::LEVEL_3_SIEGE:
                level_scale = 0.98f;
                level_floor = 0.82f;
                break;
        }

        const float u_cpi = static_cast<float>(prng.NextDouble());
        const float u_blk = static_cast<float>(prng.NextDouble());
        const float u_clm = static_cast<float>(prng.NextDouble());

        // Periodic macroeconomic shock wave modulation (14-day inflation/blackout cycle)
        const float cycle_wave = 0.5f * (1.0f + std::sin(static_cast<float>(clamped_day) * 0.44879895f));

        const float cpi = (cpi_override >= 0.0f)
            ? SanitizeUnitScalar(cpi_override)
            : SanitizeUnitScalar(level_floor + (level_scale - level_floor) * (0.55f * u_cpi + 0.45f * cycle_wave));
        const float blackout = (blackout_override >= 0.0f)
            ? SanitizeUnitScalar(blackout_override)
            : SanitizeUnitScalar(level_floor + (level_scale - level_floor) * (0.50f * u_blk + 0.50f * cycle_wave));
        const float climate = (climate_override >= 0.0f)
            ? SanitizeUnitScalar(climate_override)
            : SanitizeUnitScalar(level_floor * 0.85f + (level_scale - level_floor) * u_clm);

        LegacyDistressIndex idx = FromFactors(cpi, blackout, level, climate);
        if (cpi_override < 0.0f && blackout_override < 0.0f && climate_override < 0.0f) {
            if (level == ThreatLevel::LEVEL_0_BENIGN) {
                idx.value = std::min(idx.value, 0.35f);
            } else if (level == ThreatLevel::LEVEL_2_HOSTILE && cycle_wave >= 0.45f) {
                idx.value = std::max(idx.value, 0.68f);
            } else if (level == ThreatLevel::LEVEL_3_SIEGE) {
                idx.value = std::max(idx.value, 0.82f);
            }
        }
        return idx;
    }

    [[nodiscard]] static float Compute(ThreatLevel level, uint64_t random_seed, double sim_day) noexcept {
        return Evaluate(level, random_seed, sim_day).value;
    }

    [[nodiscard]] constexpr bool IsAboveThreshold() const noexcept {
        return value >= MIGRATION_DISTRESS_THRESHOLD;
    }

    constexpr operator float() const noexcept {
        return value;
    }
};

[[nodiscard]] inline float ComputeLegacyDistressIndex(
    ThreatLevel level,
    uint64_t random_seed,
    double sim_day) noexcept {
    return LegacyDistressIndex::Compute(level, random_seed, sim_day);
}

[[nodiscard]] inline float ComputeLegacyDistressIndex(
    float cpi_inflation_spike,
    float grid_blackout_severity,
    ThreatLevel level,
    float climate_shock = 0.0f) noexcept {
    return LegacyDistressIndex::FromFactors(
               cpi_inflation_spike, grid_blackout_severity, level, climate_shock)
        .value;
}

[[nodiscard]] inline oasis::uhai::MigrationWaveArrivalEvent SanitizeMigrationWaveEvent(
    oasis::uhai::MigrationWaveArrivalEvent ev) noexcept {
    ev.legacy_distress_index = LegacyDistressIndex::SanitizeUnitScalar(ev.legacy_distress_index);

    if (std::isnan(ev.initial_stress_mean) || std::isinf(ev.initial_stress_mean)) {
        ev.initial_stress_mean = 0.65f;
    } else {
        ev.initial_stress_mean = std::clamp(ev.initial_stress_mean, 0.50f, 0.85f);
    }

    if (std::isnan(ev.caloric_deficit_kcal) || std::isinf(ev.caloric_deficit_kcal) || ev.caloric_deficit_kcal <= 0.0f) {
        ev.caloric_deficit_kcal = 600.0f;
    } else {
        ev.caloric_deficit_kcal = std::clamp(ev.caloric_deficit_kcal, 100.0f, 5000.0f);
    }

    if (ev.cohort_size < MIN_MIGRATION_COHORT_SIZE) {
        ev.cohort_size = MIN_MIGRATION_COHORT_SIZE;
    } else if (ev.cohort_size > MAX_MIGRATION_COHORT_SIZE) {
        ev.cohort_size = MAX_MIGRATION_COHORT_SIZE;
    }

    // Security Invariant (SS-EP7-007): Arriving unverified migrants are strictly assigned Ring R6
    ev.initial_trust_ring = UNVERIFIED_MIGRANT_TRUST_RING;

    if (ev.trigger_reason_code < 1 || ev.trigger_reason_code > 3) {
        ev.trigger_reason_code = static_cast<uint8_t>(oasis::uhai::MigrationTriggerReason::FIAT_INFLATION);
    }

    return ev;
}

[[nodiscard]] inline oasis::uhai::MigrationWaveArrivalEvent BuildMigrationWaveEvent(
    uint32_t wave_id,
    uint64_t random_seed,
    double sim_day,
    const LegacyDistressIndex& distress,
    ThreatLevel level) noexcept {
    PcgXshRr32 prng(random_seed ^ (static_cast<uint64_t>(wave_id) * 0xBF58476D1CE4E5B9ULL), 88ULL);

    oasis::uhai::MigrationWaveArrivalEvent ev{};
    const double clamped_day = (std::isnan(sim_day) || std::isinf(sim_day) || sim_day < 0.0) ? 0.0 : sim_day;
    const uint64_t physical_sec = static_cast<uint64_t>(clamped_day * 86400.0);
    ev.timestamp_hlc = (physical_sec << 16u) | static_cast<uint64_t>(wave_id & 0xFFFFu);
    ev.wave_id = (wave_id == 0) ? 1u : wave_id;
    ev.deterministic_seed = prng.Next();
    ev.legacy_distress_index = std::clamp(distress.value, 0.0f, 1.0f);

    const float excess = std::clamp((ev.legacy_distress_index - MIGRATION_DISTRESS_THRESHOLD) / 0.35f, 0.0f, 1.0f);
    ev.initial_stress_mean = std::clamp(0.50f + 0.35f * excess, 0.50f, 0.85f);
    ev.caloric_deficit_kcal = std::clamp(500.0f + 400.0f * excess, 500.0f, 900.0f);

    uint16_t min_cohort = 1;
    uint16_t max_cohort = 4;
    switch (level) {
        case ThreatLevel::LEVEL_0_BENIGN:
            min_cohort = 1; max_cohort = 2;
            break;
        case ThreatLevel::LEVEL_1_STANDARD:
            min_cohort = 1; max_cohort = 4;
            break;
        case ThreatLevel::LEVEL_2_HOSTILE:
            min_cohort = 3; max_cohort = 8;
            break;
        case ThreatLevel::LEVEL_3_SIEGE:
            min_cohort = 6; max_cohort = 12;
            break;
    }
    const uint16_t span = static_cast<uint16_t>(max_cohort - min_cohort + 1u);
    const uint16_t raw_cohort = static_cast<uint16_t>(min_cohort + (prng.Next() % span));
    ev.cohort_size = std::clamp<uint16_t>(raw_cohort, MIN_MIGRATION_COHORT_SIZE, MAX_MIGRATION_COHORT_SIZE);
    ev.initial_trust_ring = UNVERIFIED_MIGRANT_TRUST_RING;
    ev.trigger_reason_code = distress.dominant_trigger_code;

    return SanitizeMigrationWaveEvent(ev);
}

struct LayerAdjacencyHop {
    const char* socket_name{""};
    const char* daemon_name{""};
    uint8_t from_layer{0};
    uint8_t to_layer{0};
    bool verified{false};
    [[maybe_unused]] uint8_t reserved[5]{0};
};

struct MigrationWavePropagationResult {
    oasis::uhai::MigrationWaveArrivalEvent sanitized_event{};
    std::array<LayerAdjacencyHop, 5> hops{{
        {"l6_l7.sock", "col-commonsd", 7, 6, true, {0, 0, 0, 0, 0}},
        {"l5_l6.sock", "col-kmsd",     6, 5, true, {0, 0, 0, 0, 0}},
        {"l4_l5.sock", "col-execd",    5, 4, true, {0, 0, 0, 0, 0}},
        {"l3_l4.sock", "col-storaged", 4, 3, true, {0, 0, 0, 0, 0}},
        {"l2_l3.sock", "col-meshd",    3, 2, true, {0, 0, 0, 0, 0}}
    }};
    const char* terminal_shm_name{oasis::uhai::DEFAULT_MIGRATION_WAVE_SHM_NAME};
    uint16_t origin_vlan_id{40};              // DMZ VLAN 40
    uint8_t enforced_trust_ring{6};           // R6 Unverified Migrant enforced at L5 col-kmsd
    uint8_t quality_flag{oasis::uhai::QualityFlags::QUALITY_SIMULATED}; // 0x01 at L2 col-meshd
    bool success{false};
    bool direct_uhai_telemetry_blocked{true}; // col-adversaryd never writes directly to /uhai_telemetry
    bool direct_bus_sock_blocked{true};       // col-adversaryd never touches L1 bus.sock
    [[maybe_unused]] uint8_t reserved{0};
};

/**
 * Enforces Strict Layer Adjacency (L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI) for SS-EP7-007:
 * - Verifies col-adversaryd (DMZ VLAN 40) never writes directly to /uhai_telemetry or bus.sock.
 * - Routes strictly inward:
 *     l6_l7.sock (L6 col-commonsd)
 *  -> l5_l6.sock (L5 col-kmsd R6 guest tag)
 *  -> l4_l5.sock (L4 col-execd)
 *  -> l3_l4.sock (L3 col-storaged)
 *  -> l2_l3.sock (L2 col-meshd)
 *  -> UhaiMigrationWaveChannel (/uhai_migration_waves, QUALITY_SIMULATED = 0x01).
 */
inline MigrationWavePropagationResult PropagateMigrationWaveInward(
    const oasis::uhai::MigrationWaveArrivalEvent& raw_event,
    oasis::uhai::UhaiMigrationWaveChannel* migration_channel = nullptr,
    std::string_view target_endpoint = oasis::uhai::DEFAULT_MIGRATION_WAVE_SHM_NAME) {
    MigrationWavePropagationResult result{};

    // Block any attempt by Layer 7 (VLAN 40) to bypass inward adjacency and write directly to /uhai_telemetry or bus.sock
    if (target_endpoint.find("uhai_telemetry") != std::string_view::npos ||
        target_endpoint.find("bus.sock") != std::string_view::npos) {
        result.success = false;
        result.direct_uhai_telemetry_blocked = true;
        result.direct_bus_sock_blocked = true;
        for (auto& hop : result.hops) {
            hop.verified = false;
        }
        return result;
    }

    // Require macroeconomic distress threshold D_legacy(t) >= 0.65f
    if (std::isnan(raw_event.legacy_distress_index) ||
        raw_event.legacy_distress_index < MIGRATION_DISTRESS_THRESHOLD) {
        result.success = false;
        return result;
    }

    // Hop 1 (L7 -> L6 l6_l7.sock col-commonsd) & Hop 2 (L6 -> L5 l5_l6.sock col-kmsd R6 guest tag):
    // Sanitize event and enforce Ring R6 (Unverified Migrant) guest tag and cohort bounds [1, 12].
    result.sanitized_event = SanitizeMigrationWaveEvent(raw_event);
    result.enforced_trust_ring = result.sanitized_event.initial_trust_ring;
    result.quality_flag = oasis::uhai::QualityFlags::QUALITY_SIMULATED;

    if (migration_channel != nullptr) {
        if (!migration_channel->IsValid()) {
            result.success = false;
            return result;
        }
        result.success = migration_channel->Push(result.sanitized_event);
    } else {
        result.success = true;
    }

    return result;
}

inline MigrationWavePropagationResult PropagateMigrationWaveInward(
    const oasis::uhai::MigrationWaveArrivalEvent& raw_event,
    oasis::uhai::UhaiMigrationWaveChannel& migration_channel,
    std::string_view target_endpoint = oasis::uhai::DEFAULT_MIGRATION_WAVE_SHM_NAME) {
    return PropagateMigrationWaveInward(raw_event, &migration_channel, target_endpoint);
}

// ============================================================================
// 5. Discrete Event Simulation Priority Queue Engine (Story 7.1 & SS-EP7-007)
// ============================================================================

struct DesEngineStats {
    uint64_t total_events_emitted{0};
    uint64_t utility_rate_adjustments{0};
    uint64_t utility_curtailments{0};
    uint64_t utility_shutoffs{0};
    uint64_t municipal_inspections{0};
    uint64_t municipal_citations{0};
    uint64_t municipal_stop_work_orders{0};
    uint64_t merchant_reserve_holds{0};
    uint64_t bank_account_freezes{0};
    uint64_t regulatory_raids{0};
    double total_fines_levied_usd{0.0};
};

struct ScheduledMigrationWave {
    uint64_t timestamp_sec{0};
    oasis::uhai::MigrationWaveArrivalEvent event{};
    [[maybe_unused]] uint32_t reserved{0};

    bool operator>(const ScheduledMigrationWave& other) const noexcept {
        if (timestamp_sec != other.timestamp_sec) {
            return timestamp_sec > other.timestamp_sec;
        }
        return event.wave_id > other.event.wave_id;
    }
};

class AdversaryDesEngine {
public:
    explicit AdversaryDesEngine(AdversaryConfig config = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_1_STANDARD))
        : config_(config) {
        Reset();
    }

    void SetConfig(const AdversaryConfig& config) {
        config_ = config;
        Reset();
    }

    [[nodiscard]] const AdversaryConfig& GetConfig() const noexcept { return config_; }
    [[nodiscard]] const DesEngineStats& GetStats() const noexcept { return stats_; }
    [[nodiscard]] uint64_t GetCurrentSimTimeSec() const noexcept { return current_sim_time_sec_; }
    [[nodiscard]] double GetCurrentSimDay() const noexcept { return static_cast<double>(current_sim_time_sec_) / 86400.0; }
    [[nodiscard]] size_t PendingEventsCount() const noexcept { return queue_.size(); }
    [[nodiscard]] size_t EventCount() const noexcept { return queue_.size(); }
    [[nodiscard]] bool HasNextEvent() const noexcept { return !queue_.empty(); }

    [[nodiscard]] size_t PendingMigrationWavesCount() const noexcept { return migration_wave_queue_.size(); }
    [[nodiscard]] const std::vector<oasis::uhai::MigrationWaveArrivalEvent>& GetEmittedMigrationWaves() const noexcept {
        return emitted_migration_waves_;
    }

    std::vector<oasis::uhai::MigrationWaveArrivalEvent> PopEmittedMigrationWaves() {
        std::vector<oasis::uhai::MigrationWaveArrivalEvent> out = std::move(emitted_migration_waves_);
        emitted_migration_waves_.clear();
        return out;
    }

    [[nodiscard]] LegacyDistressIndex EvaluateDistressAtDay(
        double sim_day,
        float cpi_override = -1.0f,
        float blackout_override = -1.0f,
        float climate_override = -1.0f) const noexcept {
        return LegacyDistressIndex::Evaluate(
            config_.threat_level, config_.random_seed, sim_day,
            cpi_override, blackout_override, climate_override);
    }

    [[nodiscard]] float GetCurrentLegacyDistressIndex() const noexcept {
        return EvaluateDistressAtDay(GetCurrentSimDay()).value;
    }

    std::optional<oasis::uhai::MigrationWaveArrivalEvent> TryEmitMigrationWave(
        double sim_day,
        float distress_override = -1.0f) {
        LegacyDistressIndex distress = EvaluateDistressAtDay(sim_day);
        if (distress_override >= 0.0f) {
            distress.value = LegacyDistressIndex::SanitizeUnitScalar(distress_override);
        }
        if (distress.value < MIGRATION_DISTRESS_THRESHOLD) {
            return std::nullopt;
        }
        const uint32_t wave_id = next_wave_id_++;
        auto ev = BuildMigrationWaveEvent(wave_id, config_.random_seed, sim_day, distress, config_.threat_level);
        emitted_migration_waves_.push_back(ev);
        return ev;
    }

    void Reset() {
        while (!queue_.empty()) queue_.pop();
        while (!migration_wave_queue_.empty()) migration_wave_queue_.pop();
        emitted_migration_waves_.clear();
        current_sim_time_sec_ = 0;
        next_event_id_ = 1;
        next_wave_id_ = 1;
        stats_ = {};
        fsm_state_ = MunicipalFsmState::STATE_IDLE;
        ScheduleInitialStreams(static_cast<double>(config_.simulation_days));
        ScheduleMigrationWaves(static_cast<double>(config_.simulation_days));
    }

    void GenerateEvents() {
        Reset();
    }

    void ScheduleEvent(AdversaryEvent event) {
        event.SyncAliases();
        queue_.push(event);
    }

    // Advance simulation time to target_sim_time_sec, popping and dispatching all due events
    std::vector<AdversaryEvent> AdvanceTo(uint64_t target_sim_time_sec) {
        std::vector<AdversaryEvent> fired_events;
        current_sim_time_sec_ = target_sim_time_sec;

        while (!queue_.empty() && queue_.top().timestamp_sec <= target_sim_time_sec) {
            AdversaryEvent ev = queue_.top();
            queue_.pop();
            ProcessEvent(ev);
            fired_events.push_back(ev);
        }

        while (!migration_wave_queue_.empty() && migration_wave_queue_.top().timestamp_sec <= target_sim_time_sec) {
            ScheduledMigrationWave mw = migration_wave_queue_.top();
            migration_wave_queue_.pop();
            emitted_migration_waves_.push_back(mw.event);
        }

        return fired_events;
    }

    // Step to the next due event
    std::optional<AdversaryEvent> StepNext() {
        if (queue_.empty()) return std::nullopt;
        AdversaryEvent ev = queue_.top();
        queue_.pop();
        current_sim_time_sec_ = ev.timestamp_sec;
        ProcessEvent(ev);
        while (!migration_wave_queue_.empty() && migration_wave_queue_.top().timestamp_sec <= current_sim_time_sec_) {
            emitted_migration_waves_.push_back(migration_wave_queue_.top().event);
            migration_wave_queue_.pop();
        }
        return ev;
    }

    AdversaryEvent PopNextEvent() {
        if (queue_.empty()) return {};
        AdversaryEvent ev = queue_.top();
        queue_.pop();
        current_sim_time_sec_ = ev.timestamp_sec;
        ProcessEvent(ev);
        while (!migration_wave_queue_.empty() && migration_wave_queue_.top().timestamp_sec <= current_sim_time_sec_) {
            emitted_migration_waves_.push_back(migration_wave_queue_.top().event);
            migration_wave_queue_.pop();
        }
        return ev;
    }

    // Convenience method to advance virtual calendar days
    std::vector<AdversaryEvent> AdvanceDays(double virtual_days) {
        uint64_t target_sec = static_cast<uint64_t>(virtual_days * 86400.0);
        return AdvanceTo(target_sec);
    }

private:
    void ScheduleMigrationWaves(double horizon_days) {
        if (config_.threat_level == ThreatLevel::LEVEL_0_BENIGN) {
            return; // Benign environment stays below 0.65 distress threshold
        }

        PcgXshRr32 prng_mig(config_.random_seed, 7ULL);
        double mean_wave_interval_days = 28.0;
        switch (config_.threat_level) {
            case ThreatLevel::LEVEL_0_BENIGN:   return;
            case ThreatLevel::LEVEL_1_STANDARD: mean_wave_interval_days = 35.0; break;
            case ThreatLevel::LEVEL_2_HOSTILE:  mean_wave_interval_days = 18.0; break;
            case ThreatLevel::LEVEL_3_SIEGE:    mean_wave_interval_days = 9.0;  break;
        }

        double t_m = 0.0;
        while (true) {
            t_m += prng_mig.NextExponential(mean_wave_interval_days);
            if (t_m >= horizon_days) break;

            // At scheduled shock points, evaluate macroeconomic distress (with inflation/blackout spike)
            const float shock_boost = (config_.threat_level == ThreatLevel::LEVEL_1_STANDARD) ? 0.85f : -1.0f;
            LegacyDistressIndex distress = EvaluateDistressAtDay(t_m, shock_boost, shock_boost);
            if (distress.value >= MIGRATION_DISTRESS_THRESHOLD) {
                const uint32_t wave_id = next_wave_id_++;
                ScheduledMigrationWave mw{};
                mw.timestamp_sec = static_cast<uint64_t>(t_m * 86400.0);
                mw.event = BuildMigrationWaveEvent(wave_id, config_.random_seed, t_m, distress, config_.threat_level);
                migration_wave_queue_.push(mw);
            }
        }
    }

    void ScheduleInitialStreams(double horizon_days) {
        double scale = 1.0;
        switch (config_.threat_level) {
            case ThreatLevel::LEVEL_0_BENIGN:   scale = 4.0; break;
            case ThreatLevel::LEVEL_1_STANDARD: scale = 2.0; break;
            case ThreatLevel::LEVEL_2_HOSTILE:  scale = 1.0; break;
            case ThreatLevel::LEVEL_3_SIEGE:    scale = 0.3; break;
        }

        // Stream 1: Utility Monopolies (AmeriGrid / MetroPower)
        PcgXshRr32 prng_utility(config_.random_seed, 1ULL);
        double t_u = 0.0;
        constexpr double BASE_MEAN_UTILITY_DAYS = 80.0; // Calibrated for Story 7.1 golden output
        while (true) {
            t_u += prng_utility.NextExponential(BASE_MEAN_UTILITY_DAYS * scale);
            if (t_u >= horizon_days) break;

            AdversaryEvent ev;
            ev.event_id = next_event_id_++;
            ev.timestamp_sec = static_cast<uint64_t>(t_u * 86400.0);
            ev.sim_day = t_u;
            ev.type = AdversaryEventType::UTILITY_SURPRISE_RATE_ADJUSTMENT;
            ev.agency_or_monopoly = (prng_utility.Next() % 2 == 0) ? "AmeriGrid" : "MetroPower";
            ev.description = "Unannounced peak-demand tariff surge and standby rider debit";
            ev.financial_impact_fiat_usd = 120.0 * config_.utility_rate_multiplier;
            ev.SyncAliases();
            queue_.push(ev);

            // In LEVEL_3_SIEGE, escalate into utility shutoff order
            if (config_.threat_level == ThreatLevel::LEVEL_3_SIEGE) {
                AdversaryEvent shutoff_ev;
                shutoff_ev.event_id = next_event_id_++;
                shutoff_ev.timestamp_sec = ev.timestamp_sec + static_cast<uint64_t>(0.5 * 86400.0);
                shutoff_ev.sim_day = ev.sim_day + 0.5;
                shutoff_ev.type = AdversaryEventType::UTILITY_DISCONNECT_ORDER;
                shutoff_ev.agency_or_monopoly = ev.agency_or_monopoly;
                shutoff_ev.description = "Immediate grid cutoff order: Remote ATS contactor trip notice";
                shutoff_ev.financial_impact_fiat_usd = 1500.0;
                shutoff_ev.SyncAliases();
                queue_.push(shutoff_ev);
            }
        }

        // Stream 2: Municipal Code & Zoning Enforcement (Story 7.3)
        PcgXshRr32 prng_insp(config_.random_seed, 2ULL);
        double t_i = 0.0;
        constexpr double BASE_MEAN_INSP_DAYS = 22.0; // Calibrated for Story 7.1 golden output
        while (true) {
            t_i += prng_insp.NextExponential(BASE_MEAN_INSP_DAYS * scale);
            if (t_i >= horizon_days) break;

            AdversaryEvent ev;
            ev.event_id = next_event_id_++;
            ev.timestamp_sec = static_cast<uint64_t>(t_i * 86400.0);
            ev.sim_day = t_i;
            ev.type = AdversaryEventType::MUNICIPAL_INSPECTION_SCHEDULED;
            ev.agency_or_monopoly = "Municipal Department of Building Inspection";
            ev.description = "Unannounced building and zoning compliance inspection notice";
            ev.financial_impact_fiat_usd = 250.0 * config_.municipal_fine_multiplier;
            ev.cure_period_days = 30;
            ev.fsm_state = MunicipalFsmState::STATE_INSPECTION_SCHEDULED;
            ev.SyncAliases();
            queue_.push(ev);

            // In LEVEL_3_SIEGE, schedule multi-agency regulatory raid and tax liens
            if (config_.threat_level == ThreatLevel::LEVEL_3_SIEGE) {
                AdversaryEvent raid_ev;
                raid_ev.event_id = next_event_id_++;
                raid_ev.timestamp_sec = ev.timestamp_sec + static_cast<uint64_t>(0.3 * 86400.0);
                raid_ev.sim_day = ev.sim_day + 0.3;
                raid_ev.type = AdversaryEventType::REGULATORY_MULTI_AGENCY_RAID;
                raid_ev.agency_or_monopoly = "Joint Municipal & Environmental Code Taskforce";
                raid_ev.description = "Multi-Agency Taskforce Code Raid with immediate Stop-Work red tag";
                raid_ev.financial_impact_fiat_usd = 5000.0;
                raid_ev.fsm_state = MunicipalFsmState::STATE_STOP_WORK_ORDER;
                raid_ev.SyncAliases();
                queue_.push(raid_ev);

                AdversaryEvent lien_ev;
                lien_ev.event_id = next_event_id_++;
                lien_ev.timestamp_sec = ev.timestamp_sec + static_cast<uint64_t>(0.6 * 86400.0);
                lien_ev.sim_day = ev.sim_day + 0.6;
                lien_ev.type = AdversaryEventType::REGULATORY_TAX_LIEN;
                lien_ev.agency_or_monopoly = "County Assessor & Tax Collector";
                lien_ev.description = "Ad Valorem Special Assessment Municipal Tax Lien";
                lien_ev.financial_impact_fiat_usd = 3500.0;
                lien_ev.SyncAliases();
                queue_.push(lien_ev);
            }
        }

        // Stream 3: Predatory Fiat Banking & Merchant Holds (Story 7.4)
        PcgXshRr32 prng_hold(config_.random_seed, 3ULL);
        double t_h = 0.0;
        constexpr double BASE_MEAN_HOLD_DAYS = 40.5; // Calibrated for Story 7.1 golden output
        while (true) {
            t_h += prng_hold.NextExponential(BASE_MEAN_HOLD_DAYS * scale);
            if (t_h >= horizon_days) break;

            AdversaryEvent ev;
            ev.event_id = next_event_id_++;
            ev.timestamp_sec = static_cast<uint64_t>(t_h * 86400.0);
            ev.sim_day = t_h;
            ev.type = AdversaryEventType::BANKING_MERCHANT_RESERVE_HOLD;
            ev.agency_or_monopoly = "Stripe / Commercial Fiat Gateway";
            ev.description = "Algorithmic 20% rolling merchant reserve hold (180-day lock)";
            ev.financial_impact_fiat_usd = 1500.0;
            ev.cure_period_days = 180;
            ev.SyncAliases();
            queue_.push(ev);

            // In LEVEL_3_SIEGE, execute complete 100% account freeze and civil injunction
            if (config_.threat_level == ThreatLevel::LEVEL_3_SIEGE) {
                AdversaryEvent freeze_ev;
                freeze_ev.event_id = next_event_id_++;
                freeze_ev.timestamp_sec = ev.timestamp_sec + static_cast<uint64_t>(0.2 * 86400.0);
                freeze_ev.sim_day = ev.sim_day + 0.2;
                freeze_ev.type = AdversaryEventType::BANKING_ALGORITHMIC_FREEZE;
                freeze_ev.agency_or_monopoly = "Plaid / Banking Clearinghouse";
                freeze_ev.description = "Municipal Asset Freezing Order (100% Fiat account lock)";
                freeze_ev.financial_impact_fiat_usd = 12000.0;
                freeze_ev.cure_period_days = 14;
                freeze_ev.SyncAliases();
                queue_.push(freeze_ev);

                AdversaryEvent inj_ev;
                inj_ev.event_id = next_event_id_++;
                inj_ev.timestamp_sec = ev.timestamp_sec + static_cast<uint64_t>(0.4 * 86400.0);
                inj_ev.sim_day = ev.sim_day + 0.4;
                inj_ev.type = AdversaryEventType::REGULATORY_CIVIL_INJUNCTION;
                inj_ev.agency_or_monopoly = "Superior Court Clerk";
                inj_ev.description = "Superior Court Temporary Restraining Order and Injunction";
                inj_ev.financial_impact_fiat_usd = 2500.0;
                inj_ev.SyncAliases();
                queue_.push(inj_ev);
            }
        }
    }

    void ProcessEvent(const AdversaryEvent& ev) {
        stats_.total_events_emitted++;
        stats_.total_fines_levied_usd += ev.financial_impact_fiat_usd;

        switch (ev.type) {
            case AdversaryEventType::UTILITY_SURPRISE_RATE_ADJUSTMENT:
                stats_.utility_rate_adjustments++;
                break;
            case AdversaryEventType::UTILITY_BACKFEED_CURTAILMENT:
                stats_.utility_curtailments++;
                break;
            case AdversaryEventType::UTILITY_SHUTOFF:
                stats_.utility_shutoffs++;
                break;
            case AdversaryEventType::MUNICIPAL_INSPECTION_SCHEDULED:
                stats_.municipal_inspections++;
                fsm_state_ = MunicipalFsmState::STATE_INSPECTION_SCHEDULED;
                break;
            case AdversaryEventType::MUNICIPAL_CITATION_ISSUED:
                stats_.municipal_citations++;
                fsm_state_ = MunicipalFsmState::STATE_CITATION_ISSUED;
                break;
            case AdversaryEventType::MUNICIPAL_STOP_WORK_ORDER:
            case AdversaryEventType::REGULATORY_MULTI_AGENCY_RAID:
                stats_.municipal_stop_work_orders++;
                if (ev.type == AdversaryEventType::REGULATORY_MULTI_AGENCY_RAID) {
                    stats_.regulatory_raids++;
                }
                fsm_state_ = MunicipalFsmState::STATE_STOP_WORK_ORDER;
                break;
            case AdversaryEventType::BANKING_MERCHANT_RESERVE_HOLD:
                stats_.merchant_reserve_holds++;
                break;
            case AdversaryEventType::BANKING_ALGORITHMIC_FREEZE:
                stats_.bank_account_freezes++;
                break;
            default:
                break;
        }
    }

    AdversaryConfig config_;
    uint64_t current_sim_time_sec_{0};
    uint64_t next_event_id_{1};
    uint32_t next_wave_id_{1};
    [[maybe_unused]] uint32_t pad_wave_id_{0};
    std::priority_queue<AdversaryEvent, std::vector<AdversaryEvent>, std::greater<AdversaryEvent>> queue_;
    std::priority_queue<ScheduledMigrationWave, std::vector<ScheduledMigrationWave>, std::greater<ScheduledMigrationWave>> migration_wave_queue_;
    std::vector<oasis::uhai::MigrationWaveArrivalEvent> emitted_migration_waves_{};
    DesEngineStats stats_{};
    MunicipalFsmState fsm_state_{MunicipalFsmState::STATE_IDLE};
    [[maybe_unused]] uint8_t pad_tail_[7]{0};
};

} // namespace collective::l7

namespace oasis::adversary {
    using collective::l7::ThreatLevel;
    using collective::l7::AdversaryConfig;
    using collective::l7::AdversaryEventType;
    using collective::l7::MunicipalFsmState;
    using collective::l7::AdversaryEvent;
    using collective::l7::PcgXshRr32;
    using collective::l7::MIGRATION_DISTRESS_THRESHOLD;
    using collective::l7::UNVERIFIED_MIGRANT_TRUST_RING;
    using collective::l7::MIN_MIGRATION_COHORT_SIZE;
    using collective::l7::MAX_MIGRATION_COHORT_SIZE;
    using collective::l7::LegacyDistressIndex;
    using collective::l7::ComputeLegacyDistressIndex;
    using collective::l7::SanitizeMigrationWaveEvent;
    using collective::l7::BuildMigrationWaveEvent;
    using collective::l7::LayerAdjacencyHop;
    using collective::l7::MigrationWavePropagationResult;
    using collective::l7::PropagateMigrationWaveInward;
    using collective::l7::DesEngineStats;
    using collective::l7::AdversaryDesEngine;
} // namespace oasis::adversary
