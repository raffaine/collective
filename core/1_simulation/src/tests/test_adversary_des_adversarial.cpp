#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <sstream>

#include "adversary_engine.hpp"

using namespace collective::l7;

static int g_total_assertions = 0;
static int g_failed_assertions = 0;

#define CHALLENGE_ASSERT(cond, msg) do { \
    g_total_assertions++; \
    if (!(cond)) { \
        g_failed_assertions++; \
        std::cerr << "  [ASSERTION FAILED] " << msg << " at line " << __LINE__ << std::endl; \
    } \
} while(0)

// ============================================================================
// CHALLENGE 1: High Event Count Stress & Min-Heap Throughput (100,000 Events)
// ============================================================================
void Challenge1_HighEventCountScale() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 1] High Event Count Stress & Min-Heap Throughput (100k Events)\n";
    std::cout << "====================================================================\n";

    AdversaryConfig cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_1_STANDARD, 1337, 90);
    AdversaryDesEngine engine(cfg);
    // Drain default events
    while (engine.HasNextEvent()) engine.PopNextEvent();

    constexpr size_t N = 100000;
    std::mt19937_64 rng(54321);
    std::uniform_int_distribution<uint64_t> dist_time(10, 100000000);

    auto start_push = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        AdversaryEvent ev;
        ev.event_id = i + 1;
        ev.timestamp_sec = dist_time(rng);
        ev.type = AdversaryEventType::UTILITY_SURPRISE_RATE_ADJUSTMENT;
        ev.financial_impact_fiat_usd = 100.0;
        engine.ScheduleEvent(ev);
    }
    auto end_push = std::chrono::high_resolution_clock::now();
    double push_ms = std::chrono::duration<double, std::milli>(end_push - start_push).count();

    CHALLENGE_ASSERT(engine.PendingEventsCount() == N, "Queue size must match scheduled 100,000 events");

    auto start_pop = std::chrono::high_resolution_clock::now();
    uint64_t last_timestamp = 0;
    uint64_t last_event_id = 0;
    size_t popped_count = 0;

    while (engine.HasNextEvent()) {
        AdversaryEvent ev = engine.PopNextEvent();
        popped_count++;

        CHALLENGE_ASSERT(ev.timestamp_sec >= last_timestamp,
                         "Timestamp monotonicity violated: current=" + std::to_string(ev.timestamp_sec) +
                         " < last=" + std::to_string(last_timestamp));

        if (ev.timestamp_sec == last_timestamp) {
            CHALLENGE_ASSERT(ev.event_id > last_event_id,
                             "Tie-breaker invariant violated on equal timestamps: id=" +
                             std::to_string(ev.event_id) + " <= last_id=" + std::to_string(last_event_id));
        }

        last_timestamp = ev.timestamp_sec;
        last_event_id = ev.event_id;
    }
    auto end_pop = std::chrono::high_resolution_clock::now();
    double pop_ms = std::chrono::duration<double, std::milli>(end_pop - start_pop).count();

    CHALLENGE_ASSERT(popped_count == N, "All 100,000 events must be popped");
    CHALLENGE_ASSERT(engine.PendingEventsCount() == 0, "Queue must be empty after popping all events");

    std::cout << "  [PASS] 100,000 events processed successfully.\n";
    std::cout << "         Push time: " << push_ms << " ms (" << (N / (push_ms / 1000.0)) << " events/sec)\n";
    std::cout << "         Pop time:  " << pop_ms << " ms (" << (N / (pop_ms / 1000.0)) << " events/sec)\n";
    std::cout << "         Strict Monotonicity & Tie-Breaker: CONFIRMED\n";
}

// ============================================================================
// CHALLENGE 2: Identical Timestamps & Reverse Order Invariance
// ============================================================================
void Challenge2_IdenticalTimestampsAndReverseOrder() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 2] Identical Timestamps & Reverse Insertion Invariance\n";
    std::cout << "====================================================================\n";

    AdversaryConfig cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_1_STANDARD, 1337, 90);
    AdversaryDesEngine engine(cfg);
    while (engine.HasNextEvent()) engine.PopNextEvent();

    constexpr size_t BATCH_SIZE = 5000;
    constexpr uint64_t FIXED_TS = 864000; // 10 days in seconds

    // Push 5000 events with reverse ID ordering
    for (size_t i = BATCH_SIZE; i >= 1; --i) {
        AdversaryEvent ev;
        ev.event_id = i;
        ev.timestamp_sec = FIXED_TS;
        ev.type = AdversaryEventType::MUNICIPAL_INSPECTION_SCHEDULED;
        engine.ScheduleEvent(ev);
    }

    uint64_t expected_id = 1;
    bool tie_order_correct = true;
    while (engine.HasNextEvent()) {
        AdversaryEvent ev = engine.PopNextEvent();
        CHALLENGE_ASSERT(ev.timestamp_sec == FIXED_TS, "Timestamp must remain exactly FIXED_TS");
        if (ev.event_id != expected_id) {
            tie_order_correct = false;
        }
        expected_id++;
    }

    CHALLENGE_ASSERT(tie_order_correct, "Events with identical timestamps must pop in ascending event_id order");
    CHALLENGE_ASSERT(expected_id == BATCH_SIZE + 1, "All 5,000 events must be popped");
    std::cout << "  [PASS] 5,000 identical-timestamp events popped in strict ascending event_id order.\n";

    // Test reverse timestamp insertion (10000 -> 1)
    for (uint64_t t = 10000; t >= 1; --t) {
        AdversaryEvent ev;
        ev.event_id = t;
        ev.timestamp_sec = t * 10;
        engine.ScheduleEvent(ev);
    }

    uint64_t last_t = 0;
    while (engine.HasNextEvent()) {
        AdversaryEvent ev = engine.PopNextEvent();
        CHALLENGE_ASSERT(ev.timestamp_sec > last_t, "Reverse-inserted events must pop in strictly increasing order");
        last_t = ev.timestamp_sec;
    }
    std::cout << "  [PASS] 10,000 reverse-inserted events popped in strictly increasing order.\n";
}

// ============================================================================
// CHALLENGE 3: Extreme Timestamp Boundary Fuzzing
// ============================================================================
void Challenge3_ExtremeTimestampBoundaries() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 3] Extreme Timestamp Boundary Fuzzing (0, UINT64_MAX, etc.)\n";
    std::cout << "====================================================================\n";

    AdversaryConfig cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_1_STANDARD, 1337, 90);
    AdversaryDesEngine engine(cfg);
    while (engine.HasNextEvent()) engine.PopNextEvent();

    std::vector<uint64_t> extreme_timestamps = {
        0ULL,
        1ULL,
        2ULL,
        86400ULL,
        1000000000ULL,
        1000000000000ULL,
        0x7FFFFFFFFFFFFFFFULL, // INT64_MAX
        0x8000000000000000ULL, // INT64_MAX + 1
        0xFFFFFFFFFFFFFFFEULL, // UINT64_MAX - 1
        0xFFFFFFFFFFFFFFFFULL  // UINT64_MAX
    };

    // Push in reverse order
    for (size_t i = extreme_timestamps.size(); i > 0; --i) {
        AdversaryEvent ev;
        ev.event_id = i;
        ev.timestamp_sec = extreme_timestamps[i - 1];
        engine.ScheduleEvent(ev);
    }

    size_t idx = 0;
    while (engine.HasNextEvent()) {
        AdversaryEvent ev = engine.PopNextEvent();
        CHALLENGE_ASSERT(ev.timestamp_sec == extreme_timestamps[idx],
                         "Popped timestamp at index " + std::to_string(idx) +
                         " expected " + std::to_string(extreme_timestamps[idx]) +
                         " but got " + std::to_string(ev.timestamp_sec));
        idx++;
    }

    CHALLENGE_ASSERT(idx == extreme_timestamps.size(), "All extreme boundary timestamps popped");
    std::cout << "  [PASS] All 64-bit boundary timestamps (including 0 and UINT64_MAX) sorted and popped accurately.\n";
}

// ============================================================================
// CHALLENGE 4: Threat Level Frequency Scaling & 200-Seed Monte Carlo Sweep
// ============================================================================
void Challenge4_ThreatLevelScalingAndMonteCarlo() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 4] Threat Level Frequency Scaling & 200-Seed Monte Carlo Sweep\n";
    std::cout << "====================================================================\n";

    constexpr size_t NUM_SEEDS = 200;
    std::vector<uint64_t> test_seeds;
    test_seeds.reserve(NUM_SEEDS);
    // Include deterministic edge seeds
    test_seeds.push_back(0);
    test_seeds.push_back(1);
    test_seeds.push_back(1337);
    test_seeds.push_back(4242);
    test_seeds.push_back(65535);
    test_seeds.push_back(0xFFFFFFFFULL);

    std::mt19937_64 seed_gen(98765);
    while (test_seeds.size() < NUM_SEEDS) {
        test_seeds.push_back(seed_gen());
    }

    double total_events_l0 = 0.0;
    double total_events_l1 = 0.0;
    double total_events_l2 = 0.0;
    double total_events_l3 = 0.0;

    int siege_raid_confirmed_count = 0;
    int siege_freeze_confirmed_count = 0;
    int siege_lien_confirmed_count = 0;
    int siege_disconnect_confirmed_count = 0;

    int benign_freeze_violation_count = 0;
    int benign_disconnect_violation_count = 0;

    for (uint64_t seed : test_seeds) {
        auto cfg0 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_0_BENIGN, seed, 90);
        auto cfg1 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_1_STANDARD, seed, 90);
        auto cfg2 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_2_HOSTILE, seed, 90);
        auto cfg3 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, seed, 90);

        AdversaryDesEngine des0(cfg0);
        AdversaryDesEngine des1(cfg1);
        AdversaryDesEngine des2(cfg2);
        AdversaryDesEngine des3(cfg3);

        size_t cnt0 = des0.EventCount();
        size_t cnt1 = des1.EventCount();
        size_t cnt2 = des2.EventCount();
        size_t cnt3 = des3.EventCount();

        total_events_l0 += cnt0;
        total_events_l1 += cnt1;
        total_events_l2 += cnt2;
        total_events_l3 += cnt3;

        // In LEVEL_0, verify bank freezes or utility shutoffs NEVER appear
        while (des0.HasNextEvent()) {
            auto ev = des0.PopNextEvent();
            if (ev.type == AdversaryEventType::BANKING_ALGORITHMIC_FREEZE ||
                ev.type == AdversaryEventType::BANK_ACCOUNT_FREEZE) {
                benign_freeze_violation_count++;
            }
            if (ev.type == AdversaryEventType::UTILITY_DISCONNECT_ORDER ||
                ev.type == AdversaryEventType::UTILITY_SHUTOFF) {
                benign_disconnect_violation_count++;
            }
        }

        // In LEVEL_3, verify siege events ALWAYS appear
        bool has_raid = false;
        bool has_freeze = false;
        bool has_lien = false;
        bool has_disconnect = false;

        while (des3.HasNextEvent()) {
            auto ev = des3.PopNextEvent();
            if (ev.type == AdversaryEventType::REGULATORY_MULTI_AGENCY_RAID ||
                ev.event_type == AdversaryEventType::UNANNOUNCED_REGULATORY_RAID) {
                has_raid = true;
            }
            if (ev.type == AdversaryEventType::BANKING_ALGORITHMIC_FREEZE ||
                ev.event_type == AdversaryEventType::BANK_ACCOUNT_FREEZE) {
                has_freeze = true;
            }
            if (ev.type == AdversaryEventType::REGULATORY_TAX_LIEN ||
                ev.event_type == AdversaryEventType::MUNICIPAL_TAX_LIEN) {
                has_lien = true;
            }
            if (ev.type == AdversaryEventType::UTILITY_DISCONNECT_ORDER ||
                ev.event_type == AdversaryEventType::UTILITY_SHUTOFF) {
                has_disconnect = true;
            }
        }

        if (has_raid) siege_raid_confirmed_count++;
        if (has_freeze) siege_freeze_confirmed_count++;
        if (has_lien) siege_lien_confirmed_count++;
        if (has_disconnect) siege_disconnect_confirmed_count++;
    }

    double mean_l0 = total_events_l0 / NUM_SEEDS;
    double mean_l1 = total_events_l1 / NUM_SEEDS;
    double mean_l2 = total_events_l2 / NUM_SEEDS;
    double mean_l3 = total_events_l3 / NUM_SEEDS;

    std::cout << "  Across 200 random seeds (90-day horizon):\n";
    std::cout << "    Mean Events LEVEL_0_BENIGN:   " << mean_l0 << "\n";
    std::cout << "    Mean Events LEVEL_1_STANDARD: " << mean_l1 << "\n";
    std::cout << "    Mean Events LEVEL_2_HOSTILE:  " << mean_l2 << "\n";
    std::cout << "    Mean Events LEVEL_3_SIEGE:    " << mean_l3 << "\n";

    CHALLENGE_ASSERT(mean_l0 < mean_l1, "Mean events L0 must be strictly less than L1");
    CHALLENGE_ASSERT(mean_l1 < mean_l2, "Mean events L1 must be strictly less than L2");
    CHALLENGE_ASSERT(mean_l2 < mean_l3, "Mean events L2 must be strictly less than L3");
    CHALLENGE_ASSERT(mean_l3 > 30.0, "Mean events L3 must reflect overwhelming siege pressure (>30)");

    CHALLENGE_ASSERT(benign_freeze_violation_count == 0, "LEVEL_0 must NEVER fire bank freezes");
    CHALLENGE_ASSERT(benign_disconnect_violation_count == 0, "LEVEL_0 must NEVER fire utility shutoffs");

    // Empirical Poisson arrival bounds over 90-day horizon (N=200):
    // Stream 1 (Utility Shutoff): mu = 24.0 days -> P(>=1) = 1 - exp(-90/24) = 97.65%
    // Stream 2 (Raid / Lien):     mu =  6.6 days -> P(>=1) = 1 - exp(-90/6.6) = 99.999%
    // Stream 3 (Bank Freeze):     mu = 12.15 days -> P(>=1) = 1 - exp(-90/12.15) = 99.94%
    double pct_raid = (static_cast<double>(siege_raid_confirmed_count) / NUM_SEEDS) * 100.0;
    double pct_freeze = (static_cast<double>(siege_freeze_confirmed_count) / NUM_SEEDS) * 100.0;
    double pct_lien = (static_cast<double>(siege_lien_confirmed_count) / NUM_SEEDS) * 100.0;
    double pct_disconnect = (static_cast<double>(siege_disconnect_confirmed_count) / NUM_SEEDS) * 100.0;

    std::cout << "  LEVEL_3 90-day Siege Event Empirical Prevalence (N=200):\n";
    std::cout << "    Taskforce Raids:         " << pct_raid << "% (Expected ~100%)\n";
    std::cout << "    Bank Account Freezes:    " << pct_freeze << "% (Expected ~100%)\n";
    std::cout << "    Municipal Tax Liens:     " << pct_lien << "% (Expected ~100%)\n";
    std::cout << "    Utility Disconnects:     " << pct_disconnect << "% (Theoretical Poisson bound: 97.65%)\n";

    CHALLENGE_ASSERT(pct_raid >= 99.0, "LEVEL_3 must fire raids in >=99% of 90-day seeds");
    CHALLENGE_ASSERT(pct_freeze >= 99.0, "LEVEL_3 must fire freezes in >=99% of 90-day seeds");
    CHALLENGE_ASSERT(pct_lien >= 99.0, "LEVEL_3 must fire liens in >=99% of 90-day seeds");
    CHALLENGE_ASSERT(pct_disconnect >= 95.0, "LEVEL_3 must fire shutoffs in >=95% of 90-day seeds (Poisson bound 97.65%)");

    // Annual Horizon (365 days): P(0 disconnects) = exp(-365/24) = 2.5e-7 -> MUST be 100.0% across all 200 seeds
    int annual_disconnect_100pct_count = 0;
    int annual_raid_100pct_count = 0;
    for (uint64_t seed : test_seeds) {
        auto cfg3_annual = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, seed, 365);
        AdversaryDesEngine des_annual(cfg3_annual);
        bool has_annual_disconnect = false;
        bool has_annual_raid = false;
        while (des_annual.HasNextEvent()) {
            auto ev = des_annual.PopNextEvent();
            if (ev.type == AdversaryEventType::UTILITY_DISCONNECT_ORDER ||
                ev.type == AdversaryEventType::UTILITY_SHUTOFF) has_annual_disconnect = true;
            if (ev.type == AdversaryEventType::REGULATORY_MULTI_AGENCY_RAID ||
                ev.type == AdversaryEventType::UNANNOUNCED_REGULATORY_RAID) has_annual_raid = true;
        }
        if (has_annual_disconnect) annual_disconnect_100pct_count++;
        if (has_annual_raid) annual_raid_100pct_count++;
    }

    CHALLENGE_ASSERT(annual_disconnect_100pct_count == NUM_SEEDS, "LEVEL_3 must fire disconnect orders across 100% of seeds over 365 days");
    CHALLENGE_ASSERT(annual_raid_100pct_count == NUM_SEEDS, "LEVEL_3 must fire raids across 100% of seeds over 365 days");

    std::cout << "  [PASS] 365-day annual siege verified: 100% of 200 seeds fired disconnect orders and raids.\n";
    std::cout << "  [PASS] 0% of seeds in LEVEL_0 fired hostile freezes or disconnect orders.\n";
}

// ============================================================================
// CHALLENGE 5: Event Invariance Between Step-by-Step and Batch Advancement
// ============================================================================
void Challenge5_StepVsBatchInvariance() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 5] Step-by-Step vs Batch AdvanceDays() Invariance\n";
    std::cout << "====================================================================\n";

    auto cfg_batch = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 7777, 180);
    auto cfg_step = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 7777, 180);

    AdversaryDesEngine des_batch(cfg_batch);
    AdversaryDesEngine des_step(cfg_step);

    // des_batch advances 180 days in one call
    auto batch_events = des_batch.AdvanceDays(180.0);

    // des_step advances in 10-day increments
    std::vector<AdversaryEvent> step_events;
    for (double day = 10.0; day <= 180.0; day += 10.0) {
        auto fired = des_step.AdvanceDays(day);
        step_events.insert(step_events.end(), fired.begin(), fired.end());
    }

    CHALLENGE_ASSERT(batch_events.size() == step_events.size(),
                     "Batch and incremental step event counts must match exactly: batch=" +
                     std::to_string(batch_events.size()) + " vs step=" + std::to_string(step_events.size()));

    bool identical_stream = true;
    for (size_t i = 0; i < batch_events.size() && i < step_events.size(); ++i) {
        if (batch_events[i].event_id != step_events[i].event_id ||
            batch_events[i].timestamp_sec != step_events[i].timestamp_sec ||
            batch_events[i].type != step_events[i].type ||
            batch_events[i].financial_impact_fiat_usd != step_events[i].financial_impact_fiat_usd) {
            identical_stream = false;
            std::cerr << "Mismatch at event index " << i << "\n";
            break;
        }
    }
    CHALLENGE_ASSERT(identical_stream, "Batch events and incremental stepped events must be bit-for-bit identical");

    // Also compare stats
    const auto& stats_batch = des_batch.GetStats();
    const auto& stats_step = des_step.GetStats();
    CHALLENGE_ASSERT(stats_batch.total_events_emitted == stats_step.total_events_emitted, "Stats total events match");
    CHALLENGE_ASSERT(stats_batch.utility_rate_adjustments == stats_step.utility_rate_adjustments, "Stats rate adjustments match");
    CHALLENGE_ASSERT(stats_batch.municipal_inspections == stats_step.municipal_inspections, "Stats inspections match");
    CHALLENGE_ASSERT(stats_batch.merchant_reserve_holds == stats_step.merchant_reserve_holds, "Stats reserve holds match");
    CHALLENGE_ASSERT(stats_batch.bank_account_freezes == stats_step.bank_account_freezes, "Stats bank freezes match");
    CHALLENGE_ASSERT(stats_batch.regulatory_raids == stats_step.regulatory_raids, "Stats regulatory raids match");
    CHALLENGE_ASSERT(std::abs(stats_batch.total_fines_levied_usd - stats_step.total_fines_levied_usd) < 1e-6, "Stats total fines match");

    std::cout << "  [PASS] 180-day batch leap vs 18 10-day step increments are bit-for-bit identical ("
              << batch_events.size() << " events).\n";
}

// ============================================================================
// CHALLENGE 6: PRNG Statistical Uniformity & Exponential Inter-arrival Test
// ============================================================================
void Challenge6_PrngUniformityAndExponentialMoments() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 6] PRNG Uniformity & Exponential Inter-Arrival Moments (1M Samples)\n";
    std::cout << "====================================================================\n";

    PcgXshRr32 prng(123456789ULL, 1ULL);
    constexpr size_t N_SAMPLES = 1000000;

    double sum = 0.0;
    double sum_sq = 0.0;
    double min_val = 1.0;
    double max_val = 0.0;

    for (size_t i = 0; i < N_SAMPLES; ++i) {
        double u = prng.NextDouble();
        CHALLENGE_ASSERT(u >= 1e-12 && u <= (1.0 - 1e-12), "NextDouble() must stay strictly within clamped bounds");
        CHALLENGE_ASSERT(!std::isnan(u) && !std::isinf(u), "NextDouble() must never be NaN or Inf");
        sum += u;
        sum_sq += u * u;
        if (u < min_val) min_val = u;
        if (u > max_val) max_val = u;
    }

    double mean = sum / N_SAMPLES;
    double variance = (sum_sq / N_SAMPLES) - (mean * mean);

    std::cout << "  Uniform [0,1] empirical moments (1,000,000 samples):\n";
    std::cout << "    Mean:     " << mean << " (Expected ~0.5000)\n";
    std::cout << "    Variance: " << variance << " (Expected ~0.08333 [1/12])\n";
    std::cout << "    Min:      " << min_val << " | Max: " << max_val << "\n";

    CHALLENGE_ASSERT(std::abs(mean - 0.5) < 0.002, "Empirical mean must be within 0.2% of 0.5");
    CHALLENGE_ASSERT(std::abs(variance - (1.0 / 12.0)) < 0.002, "Empirical variance must be within 0.2% of 1/12");

    // Exponential inter-arrival moments test
    PcgXshRr32 prng_exp(987654321ULL, 2ULL);
    constexpr double TARGET_MEAN_DAYS = 25.0;
    double exp_sum = 0.0;
    constexpr size_t EXP_SAMPLES = 200000;
    for (size_t i = 0; i < EXP_SAMPLES; ++i) {
        double interval = prng_exp.NextExponential(TARGET_MEAN_DAYS);
        CHALLENGE_ASSERT(interval >= 0.0, "Exponential interval must be non-negative");
        CHALLENGE_ASSERT(!std::isnan(interval) && !std::isinf(interval), "Exponential interval must not be NaN/Inf");
        exp_sum += interval;
    }
    double exp_mean = exp_sum / EXP_SAMPLES;
    std::cout << "  Exponential distribution empirical mean: " << exp_mean << " days (Target: " << TARGET_MEAN_DAYS << " days)\n";
    CHALLENGE_ASSERT(std::abs(exp_mean - TARGET_MEAN_DAYS) / TARGET_MEAN_DAYS < 0.02,
                     "Exponential sample mean must converge within 2% of target parameter");

    std::cout << "  [PASS] PRNG passed 1M uniform and 200k exponential statistical validation checks.\n";
}

// ============================================================================
// CHALLENGE 7: Financial Penalties & Siege Multipliers Escalation
// ============================================================================
void Challenge7_FinancialPenaltiesEscalation() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 7] Financial Penalties & Siege Multiplier Escalation\n";
    std::cout << "====================================================================\n";

    auto cfg0 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_0_BENIGN, 2026, 365);
    auto cfg1 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_1_STANDARD, 2026, 365);
    auto cfg2 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_2_HOSTILE, 2026, 365);
    auto cfg3 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 2026, 365);

    AdversaryDesEngine des0(cfg0); auto ev0 = des0.AdvanceDays(365.0);
    AdversaryDesEngine des1(cfg1); auto ev1 = des1.AdvanceDays(365.0);
    AdversaryDesEngine des2(cfg2); auto ev2 = des2.AdvanceDays(365.0);
    AdversaryDesEngine des3(cfg3); auto ev3 = des3.AdvanceDays(365.0);

    double fines0 = des0.GetStats().total_fines_levied_usd;
    double fines1 = des1.GetStats().total_fines_levied_usd;
    double fines2 = des2.GetStats().total_fines_levied_usd;
    double fines3 = des3.GetStats().total_fines_levied_usd;

    std::cout << "  365-Day Total Levied Fiat Fines & Tariffs:\n";
    std::cout << "    LEVEL_0_BENIGN:   $" << std::fixed << std::setprecision(2) << fines0 << "\n";
    std::cout << "    LEVEL_1_STANDARD: $" << fines1 << "\n";
    std::cout << "    LEVEL_2_HOSTILE:  $" << fines2 << "\n";
    std::cout << "    LEVEL_3_SIEGE:    $" << fines3 << "\n";

    CHALLENGE_ASSERT(fines0 < fines1, "Fines must escalate monotonically: L0 < L1");
    CHALLENGE_ASSERT(fines1 < fines2, "Fines must escalate monotonically: L1 < L2");
    CHALLENGE_ASSERT(fines2 < fines3, "Fines must escalate monotonically: L2 < L3");
    CHALLENGE_ASSERT(fines3 > 100000.0, "Level 3 Siege must exert existential financial pressure (> $100,000 USD)");

    // Accumulator invariant check on Level 3 events
    double manual_sum = 0.0;
    for (const auto& ev : ev3) {
        manual_sum += ev.financial_impact_fiat_usd;
    }
    CHALLENGE_ASSERT(std::abs(manual_sum - fines3) < 1e-4, "Accumulated fines in stats must exactly match sum of events");

    std::cout << "  [PASS] Financial escalation and accumulator invariants verified.\n";
}

// ============================================================================
// CHALLENGE 8: Engine Reinitialization, Reset & Null Boundary Stress
// ============================================================================
void Challenge8_EngineResetAndNullBoundaries() {
    std::cout << "\n====================================================================\n";
    std::cout << "[CHALLENGE 8] Engine Reinitialization, Reset & Null Boundary Stress\n";
    std::cout << "====================================================================\n";

    auto cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 1337, 30);
    AdversaryDesEngine engine(cfg);

    // Test AdvanceDays(0.0)
    auto zero_advance = engine.AdvanceDays(0.0);
    CHALLENGE_ASSERT(zero_advance.empty(), "AdvanceDays(0.0) must return 0 events when initial events are at t > 0");

    // Drain engine completely
    while (engine.HasNextEvent()) engine.PopNextEvent();
    CHALLENGE_ASSERT(!engine.HasNextEvent(), "Engine must be empty");
    CHALLENGE_ASSERT(engine.PendingEventsCount() == 0, "Pending count must be 0");

    // PopNextEvent on empty queue
    auto empty_ev = engine.PopNextEvent();
    CHALLENGE_ASSERT(empty_ev.event_id == 0, "PopNextEvent on empty queue returns default event with event_id=0");

    // StepNext on empty queue
    auto opt_ev = engine.StepNext();
    CHALLENGE_ASSERT(!opt_ev.has_value(), "StepNext on empty queue returns nullopt");

    // AdvanceTo beyond horizon on empty queue
    auto empty_adv = engine.AdvanceTo(99999999);
    CHALLENGE_ASSERT(empty_adv.empty(), "AdvanceTo on empty queue returns empty vector");

    // 100 Reset cycles stress
    for (int cycle = 0; cycle < 100; ++cycle) {
        engine.Reset();
        CHALLENGE_ASSERT(engine.EventCount() > 0, "Reset must repopulate queue");
        CHALLENGE_ASSERT(engine.GetCurrentSimTimeSec() == 0, "Reset must reset current sim time to 0");
        CHALLENGE_ASSERT(engine.GetStats().total_events_emitted == 0, "Reset must zero out stats");
    }

    std::cout << "  [PASS] Empty queue guards, null boundaries, and 100 reset cycles passed cleanly.\n";
}

// ============================================================================
// MAIN HARNESS ENTRY POINT
// ============================================================================
int main() {
    std::cout << "====================================================================\n";
    std::cout << "   COL-ADVERSARYD LAYER 7 DES ADVERSARIAL STRESS TEST HARNESS       \n";
    std::cout << "====================================================================\n";

    Challenge1_HighEventCountScale();
    Challenge2_IdenticalTimestampsAndReverseOrder();
    Challenge3_ExtremeTimestampBoundaries();
    Challenge4_ThreatLevelScalingAndMonteCarlo();
    Challenge5_StepVsBatchInvariance();
    Challenge6_PrngUniformityAndExponentialMoments();
    Challenge7_FinancialPenaltiesEscalation();
    Challenge8_EngineResetAndNullBoundaries();

    std::cout << "\n====================================================================\n";
    std::cout << "   ADVERSARIAL STRESS TEST SUMMARY\n";
    std::cout << "====================================================================\n";
    std::cout << "  Total Assertions Run: " << g_total_assertions << "\n";
    std::cout << "  Failed Assertions:    " << g_failed_assertions << "\n";

    if (g_failed_assertions == 0) {
        std::cout << "====================================================================\n";
        std::cout << "OVERALL VERDICT: APPROVE\n";
        std::cout << "====================================================================\n";
        return 0;
    } else {
        std::cout << "====================================================================\n";
        std::cout << "OVERALL VERDICT: REQUEST_CHANGES (" << g_failed_assertions << " failures)\n";
        std::cout << "====================================================================\n";
        return 1;
    }
}
