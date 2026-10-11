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
// 4. Discrete Event Simulation Priority Queue Engine (Story 7.1)
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

    void Reset() {
        while (!queue_.empty()) queue_.pop();
        current_sim_time_sec_ = 0;
        next_event_id_ = 1;
        stats_ = {};
        fsm_state_ = MunicipalFsmState::STATE_IDLE;
        ScheduleInitialStreams(static_cast<double>(config_.simulation_days));
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

        return fired_events;
    }

    // Step to the next due event
    std::optional<AdversaryEvent> StepNext() {
        if (queue_.empty()) return std::nullopt;
        AdversaryEvent ev = queue_.top();
        queue_.pop();
        current_sim_time_sec_ = ev.timestamp_sec;
        ProcessEvent(ev);
        return ev;
    }

    AdversaryEvent PopNextEvent() {
        if (queue_.empty()) return {};
        AdversaryEvent ev = queue_.top();
        queue_.pop();
        current_sim_time_sec_ = ev.timestamp_sec;
        ProcessEvent(ev);
        return ev;
    }

    // Convenience method to advance virtual calendar days
    std::vector<AdversaryEvent> AdvanceDays(double virtual_days) {
        uint64_t target_sec = static_cast<uint64_t>(virtual_days * 86400.0);
        return AdvanceTo(target_sec);
    }

private:
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
    std::priority_queue<AdversaryEvent, std::vector<AdversaryEvent>, std::greater<AdversaryEvent>> queue_;
    DesEngineStats stats_{};
    MunicipalFsmState fsm_state_{MunicipalFsmState::STATE_IDLE};
};

} // namespace collective::l7

namespace oasis::adversary {
    using collective::l7::ThreatLevel;
    using collective::l7::AdversaryConfig;
    using collective::l7::AdversaryEventType;
    using collective::l7::MunicipalFsmState;
    using collective::l7::AdversaryEvent;
    using collective::l7::PcgXshRr32;
    using collective::l7::DesEngineStats;
    using collective::l7::AdversaryDesEngine;
} // namespace oasis::adversary
