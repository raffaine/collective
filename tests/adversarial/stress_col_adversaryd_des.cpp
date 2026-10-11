#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <sys/resource.h>

#include "adversary_engine.hpp"

using namespace collective::l7;

struct BenchmarkResults {
    bool passed{true};
    std::string test_name;
    std::string details;
};

// ============================================================================
// Helper: get resident set size in KB
// ============================================================================
static long GetRssKb() {
    struct rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
#if defined(__APPLE__)
        return usage.ru_maxrss / 1024; // On macOS ru_maxrss is in bytes
#else
        return usage.ru_maxrss;        // On Linux ru_maxrss is in kilobytes
#endif
    }
    return 0;
}

// ============================================================================
// Test 1: DES Queue Order & Monotonicity
// ============================================================================
BenchmarkResults TestDesQueueMonotonicity() {
    BenchmarkResults res;
    res.test_name = "DES Queue Order & Monotonicity (100,000+ events)";

    std::cout << "\n============================================================\n";
    std::cout << "[TEST 1] DES Priority Queue Monotonicity & Chronological Order\n";
    std::cout << "============================================================\n";

    // 1A. 100,000 events inserted in reverse order
    {
        std::cout << "--> Subtest 1A: 100,000 events inserted in reverse order...\n";
        AdversaryConfig cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_0_BENIGN, 1337, 1);
        AdversaryDesEngine des(cfg);
        while (des.PendingEventsCount() > 0) des.PopNextEvent();

        const uint64_t N = 100000;
        for (uint64_t i = N; i >= 1; --i) {
            AdversaryEvent ev;
            ev.event_id = i;
            ev.timestamp_sec = i * 10; // strictly decreasing insertion
            des.ScheduleEvent(ev);
        }

        if (des.PendingEventsCount() != N) {
            res.passed = false;
            res.details += "Subtest 1A Failed: Queue count != 100,000.\n";
            return res;
        }

        uint64_t prev_ts = 0;
        uint64_t prev_id = 0;
        for (uint64_t i = 1; i <= N; ++i) {
            AdversaryEvent ev = des.PopNextEvent();
            if (ev.timestamp_sec < prev_ts) {
                std::cerr << "INVERSION DETECTED: prev_ts=" << prev_ts << " > ev.ts=" << ev.timestamp_sec << "\n";
                res.passed = false;
                res.details += "Subtest 1A Failed: Timestamp inversion in reverse insertion.\n";
                return res;
            }
            if (ev.timestamp_sec != i * 10 || ev.event_id != i) {
                res.passed = false;
                res.details += "Subtest 1A Failed: Event order mismatch.\n";
                return res;
            }
            prev_ts = ev.timestamp_sec;
            prev_id = ev.event_id;
        }
        std::cout << "    [PASS] Reverse 100,000 events popped with 100% strictly non-decreasing timestamps.\n";
    }

    // 1B. 100,000 events inserted with random timestamps
    {
        std::cout << "--> Subtest 1B: 100,000 events inserted with pseudo-random timestamps...\n";
        AdversaryConfig cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_0_BENIGN, 1337, 1);
        AdversaryDesEngine des(cfg);
        while (des.PendingEventsCount() > 0) des.PopNextEvent();

        PcgXshRr32 rng(987654321ULL, 42ULL);
        const uint64_t N = 100000;
        for (uint64_t i = 1; i <= N; ++i) {
            AdversaryEvent ev;
            ev.event_id = i;
            // Generate random timestamp up to 10^12 seconds (~31,700 years)
            uint64_t ts = (static_cast<uint64_t>(rng.Next()) << 32) | rng.Next();
            ev.timestamp_sec = ts % 1000000000000ULL;
            des.ScheduleEvent(ev);
        }

        uint64_t prev_ts = 0;
        uint64_t inversions = 0;
        for (uint64_t i = 0; i < N; ++i) {
            AdversaryEvent ev = des.PopNextEvent();
            if (ev.timestamp_sec < prev_ts) {
                inversions++;
            }
            prev_ts = ev.timestamp_sec;
        }

        if (inversions > 0) {
            res.passed = false;
            res.details += "Subtest 1B Failed: Random insertion produced " + std::to_string(inversions) + " timestamp inversions.\n";
            return res;
        }
        std::cout << "    [PASS] Random 100,000 events popped with 0 inversions.\n";
    }

    // 1C. 100,000 events with heavy timestamp collisions (1,000 duplicates per timestamp)
    {
        std::cout << "--> Subtest 1C: 100,000 events with 1,000 duplicate timestamps each (tie-breaker check)...\n";
        AdversaryConfig cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_0_BENIGN, 1337, 1);
        AdversaryDesEngine des(cfg);
        while (des.PendingEventsCount() > 0) des.PopNextEvent();

        const uint64_t NUM_DISTINCT = 100;
        const uint64_t PER_TIMESTAMP = 1000;
        uint64_t global_id = 1;
        for (uint64_t t = 1; t <= NUM_DISTINCT; ++t) {
            for (uint64_t k = 1; k <= PER_TIMESTAMP; ++k) {
                AdversaryEvent ev;
                ev.event_id = global_id++;
                ev.timestamp_sec = t * 100;
                des.ScheduleEvent(ev);
            }
        }

        uint64_t prev_ts = 0;
        uint64_t prev_id = 0;
        for (uint64_t i = 0; i < NUM_DISTINCT * PER_TIMESTAMP; ++i) {
            AdversaryEvent ev = des.PopNextEvent();
            if (ev.timestamp_sec < prev_ts) {
                res.passed = false;
                res.details += "Subtest 1C Failed: Timestamp inversion on duplicate clusters.\n";
                return res;
            }
            if (ev.timestamp_sec == prev_ts && ev.event_id <= prev_id) {
                res.passed = false;
                res.details += "Subtest 1C Failed: Tie-breaker event_id inversion: prev_id=" +
                               std::to_string(prev_id) + " current_id=" + std::to_string(ev.event_id) + "\n";
                return res;
            }
            prev_ts = ev.timestamp_sec;
            prev_id = ev.event_id;
        }
        std::cout << "    [PASS] 100,000 duplicate events preserved strict (timestamp, event_id) monotonicity.\n";
    }

    // 1D. Extreme temporal horizons (1,000 days and 100,000 days natural generation)
    {
        std::cout << "--> Subtest 1D: Natural generation under 1,000 days and 10,000 days...\n";
        for (double horizon : {1000.0, 10000.0}) {
            auto cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 1337, static_cast<uint32_t>(horizon));
            AdversaryDesEngine des(cfg);
            uint64_t count = des.PendingEventsCount();
            uint64_t prev_ts = 0;
            for (uint64_t i = 0; i < count; ++i) {
                AdversaryEvent ev = des.PopNextEvent();
                if (ev.timestamp_sec < prev_ts) {
                    res.passed = false;
                    res.details += "Subtest 1D Failed: Monotonicity violation at horizon=" + std::to_string(horizon) + "\n";
                    return res;
                }
                prev_ts = ev.timestamp_sec;
            }
            std::cout << "    [PASS] Horizon " << horizon << " days: " << count << " events generated and strictly monotonic.\n";
        }
    }

    // 1E. Extreme boundary timestamps near UINT64_MAX
    {
        std::cout << "--> Subtest 1E: Extreme boundary timestamps near 0 and UINT64_MAX...\n";
        AdversaryConfig cfg = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_0_BENIGN, 1337, 1);
        AdversaryDesEngine des(cfg);
        while (des.PendingEventsCount() > 0) des.PopNextEvent();

        std::vector<uint64_t> test_stamps = {
            0,
            1,
            86400,
            1000000000ULL,
            0x7FFFFFFFFFFFFFFFULL,
            0xFFFFFFFFFFFFFF00ULL,
            0xFFFFFFFFFFFFFFFEULL,
            0xFFFFFFFFFFFFFFFFULL
        };

        uint64_t id = 1;
        // Insert in reverse order
        for (auto it = test_stamps.rbegin(); it != test_stamps.rend(); ++it) {
            AdversaryEvent ev;
            ev.event_id = id++;
            ev.timestamp_sec = *it;
            des.ScheduleEvent(ev);
        }

        for (uint64_t expected_ts : test_stamps) {
            AdversaryEvent ev = des.PopNextEvent();
            if (ev.timestamp_sec != expected_ts) {
                res.passed = false;
                res.details += "Subtest 1E Failed: Expected ts=" + std::to_string(expected_ts) +
                               ", got " + std::to_string(ev.timestamp_sec) + "\n";
                return res;
            }
        }
        std::cout << "    [PASS] Boundary timestamps from 0 to UINT64_MAX popped in exact sorted order.\n";
    }

    return res;
}

// ============================================================================
// Test 2: PRNG Independence & Stream Orthogonality across Boundary Seeds
// ============================================================================
BenchmarkResults TestPrngOrthogonalityAndBoundarySeeds() {
    BenchmarkResults res;
    res.test_name = "PRNG Independence & Collision Resistance across Boundary Seeds";

    std::cout << "\n============================================================\n";
    std::cout << "[TEST 2] PRNG Stream Independence & Boundary Seed Stability\n";
    std::cout << "============================================================\n";

    const std::vector<uint64_t> boundary_seeds = {
        0ULL,
        1ULL,
        1337ULL,
        0xAAAAAAAAAAAAAAAAULL,
        0x5555555555555555ULL,
        0x8000000000000000ULL,
        0x7FFFFFFFFFFFFFFFULL,
        0xFFFFFFFFFFFFFFFFULL // UINT64_MAX
    };

    const size_t N_SAMPLES = 100000;

    for (uint64_t seed : boundary_seeds) {
        std::cout << "--> Testing Seed: 0x" << std::hex << seed << std::dec << " (" << seed << ")...\n";

        // Instantiate Stream 1 (Utility), Stream 2 (Inspection), Stream 3 (Hold)
        PcgXshRr32 s1(seed, 1ULL);
        PcgXshRr32 s2(seed, 2ULL);
        PcgXshRr32 s3(seed, 3ULL);

        std::vector<uint32_t> vals1(N_SAMPLES);
        std::vector<uint32_t> vals2(N_SAMPLES);
        std::vector<uint32_t> vals3(N_SAMPLES);

        double sum1 = 0, sum2 = 0, sum3 = 0;
        double sum1_sq = 0, sum2_sq = 0, sum3_sq = 0;
        double sum12 = 0, sum13 = 0, sum23 = 0;

        uint64_t direct_collisions_12 = 0;
        uint64_t direct_collisions_13 = 0;
        uint64_t direct_collisions_23 = 0;

        std::unordered_set<uint32_t> unique1, unique2, unique3;

        for (size_t i = 0; i < N_SAMPLES; ++i) {
            uint32_t v1 = s1.Next();
            uint32_t v2 = s2.Next();
            uint32_t v3 = s3.Next();

            vals1[i] = v1;
            vals2[i] = v2;
            vals3[i] = v3;

            unique1.insert(v1);
            unique2.insert(v2);
            unique3.insert(v3);

            if (v1 == v2) direct_collisions_12++;
            if (v1 == v3) direct_collisions_13++;
            if (v2 == v3) direct_collisions_23++;

            double norm1 = static_cast<double>(v1) / 4294967296.0;
            double norm2 = static_cast<double>(v2) / 4294967296.0;
            double norm3 = static_cast<double>(v3) / 4294967296.0;

            sum1 += norm1; sum2 += norm2; sum3 += norm3;
            sum1_sq += norm1 * norm1; sum2_sq += norm2 * norm2; sum3_sq += norm3 * norm3;
            sum12 += norm1 * norm2; sum13 += norm1 * norm3; sum23 += norm2 * norm3;
        }

        // 2A. Check for zero-state traps or freeze
        if (unique1.size() < N_SAMPLES * 0.99 || unique2.size() < N_SAMPLES * 0.99 || unique3.size() < N_SAMPLES * 0.99) {
            res.passed = false;
            res.details += "PRNG degenerate unique value count for seed " + std::to_string(seed) + "\n";
            return res;
        }

        // 2B. Direct step-by-step collisions: expected ~0 (threshold <= 5)
        if (direct_collisions_12 > 5 || direct_collisions_13 > 5 || direct_collisions_23 > 5) {
            res.passed = false;
            res.details += "Excessive direct collisions for seed " + std::to_string(seed) + "\n";
            return res;
        }

        // 2C. Pearson Correlation Coefficient
        auto calc_r = [&](double s_x, double s_y, double s_xy, double s_xx, double s_yy, double n) {
            double cov = s_xy - (s_x * s_y) / n;
            double var_x = s_xx - (s_x * s_x) / n;
            double var_y = s_yy - (s_y * s_y) / n;
            return cov / std::sqrt(var_x * var_y);
        };

        double r12 = calc_r(sum1, sum2, sum12, sum1_sq, sum2_sq, N_SAMPLES);
        double r13 = calc_r(sum1, sum3, sum13, sum1_sq, sum3_sq, N_SAMPLES);
        double r23 = calc_r(sum2, sum3, sum23, sum2_sq, sum3_sq, N_SAMPLES);

        if (std::abs(r12) > 0.015 || std::abs(r13) > 0.015 || std::abs(r23) > 0.015) {
            res.passed = false;
            res.details += "Excessive correlation (|r| > 0.015) for seed " + std::to_string(seed) +
                           ": r12=" + std::to_string(r12) + " r13=" + std::to_string(r13) + " r23=" + std::to_string(r23) + "\n";
            return res;
        }

        // 2D. 4-Gram Subsequence Collision Check
        // Ensure no sequence of 4 consecutive outputs in Stream 1 appears in Stream 2 or Stream 3
        std::unordered_set<uint64_t> grams1;
        for (size_t i = 0; i + 3 < N_SAMPLES; i += 2) {
            // Hash 4 numbers into a 64-bit integer
            uint64_t h = (static_cast<uint64_t>(vals1[i]) << 32) ^ vals1[i+1] ^
                         (static_cast<uint64_t>(vals1[i+2]) << 16) ^ vals1[i+3];
            grams1.insert(h);
        }
        for (size_t i = 0; i + 3 < N_SAMPLES; i += 2) {
            uint64_t h2 = (static_cast<uint64_t>(vals2[i]) << 32) ^ vals2[i+1] ^
                          (static_cast<uint64_t>(vals2[i+2]) << 16) ^ vals2[i+3];
            if (grams1.count(h2) > 0) {
                res.passed = false;
                res.details += "Subsequence 4-gram collision detected between Stream 1 and Stream 2!\n";
                return res;
            }
        }

        // 2E. Exponential distribution sanity check
        PcgXshRr32 exp_rng(seed, 1ULL);
        double mean_exp = 50.0;
        double sum_exp = 0.0;
        for (size_t i = 0; i < N_SAMPLES; ++i) {
            double v = exp_rng.NextExponential(mean_exp);
            if (std::isnan(v) || std::isinf(v) || v <= 0.0) {
                res.passed = false;
                res.details += "Invalid exponential value: " + std::to_string(v) + "\n";
                return res;
            }
            sum_exp += v;
        }
        double empirical_mean = sum_exp / N_SAMPLES;
        double err_pct = std::abs(empirical_mean - mean_exp) / mean_exp * 100.0;
        if (err_pct > 2.0) {
            res.passed = false;
            res.details += "Exponential empirical mean error > 2% (" + std::to_string(err_pct) + "%)\n";
            return res;
        }

        std::cout << "    [PASS] Unique: " << unique1.size() << "/" << N_SAMPLES
                  << " | Direct collisions (1-2, 1-3, 2-3): (" << direct_collisions_12 << ", "
                  << direct_collisions_13 << ", " << direct_collisions_23 << ")"
                  << " | Pearson r: (r12=" << std::fixed << std::setprecision(5) << r12
                  << ", r13=" << r13 << ", r23=" << r23 << ")"
                  << " | ExpMean: " << std::setprecision(2) << empirical_mean << " (err " << err_pct << "%)\n";
    }

    return res;
}

// ============================================================================
// Test 3: Threat Level 3 Siege Saturation, Event Throughput & Memory Leaks
// ============================================================================
BenchmarkResults TestSiegeSaturationAndThroughput() {
    BenchmarkResults res;
    res.test_name = "Threat Level 3 Siege Saturation, Event Throughput & Memory Stability";

    std::cout << "\n============================================================\n";
    std::cout << "[TEST 3] Threat Level 3 Siege Saturation & Throughput\n";
    std::cout << "============================================================\n";

    // 3A. Siege event inventory breakdown over 90, 1000, 10000 days
    {
        std::cout << "--> Subtest 3A: Verifying Siege Event Escalation Breakdown...\n";
        auto cfg90 = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 1337, 90);
        AdversaryDesEngine des90(cfg90);
        auto fired90 = des90.AdvanceDays(90.0);
        const auto& s90 = des90.GetStats();

        std::cout << "    [Level 3 @ 90 days] Total: " << s90.total_events_emitted
                  << " | RateAdjust: " << s90.utility_rate_adjustments
                  << " | Shutoffs: " << s90.utility_shutoffs
                  << " | Inspections: " << s90.municipal_inspections
                  << " | Raids: " << s90.regulatory_raids
                  << " | ReserveHolds: " << s90.merchant_reserve_holds
                  << " | AccountFreezes: " << s90.bank_account_freezes
                  << " | Total Fines: $" << std::fixed << std::setprecision(2) << s90.total_fines_levied_usd << "\n";

        if (s90.total_events_emitted < 35) {
            res.passed = false;
            res.details += "Subtest 3A Failed: Level 3 Siege emitted fewer than 35 events.\n";
            return res;
        }
        if (s90.utility_shutoffs == 0 || s90.regulatory_raids == 0 || s90.bank_account_freezes == 0) {
            res.passed = false;
            res.details += "Subtest 3A Failed: Missing critical Level 3 siege events.\n";
            return res;
        }
    }

    // 3B. High-volume Saturation Throughput Benchmark (100,000+ events)
    {
        std::cout << "--> Subtest 3B: Massive Saturation Throughput (150,000 virtual calendar days)...\n";
        // 150,000 days in Level 3 generates ~118,000 events
        auto t_start = std::chrono::high_resolution_clock::now();
        auto cfgMassive = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 4242, 150000);
        AdversaryDesEngine desMassive(cfgMassive);
        auto t_gen = std::chrono::high_resolution_clock::now();

        uint64_t pending = desMassive.PendingEventsCount();
        std::cout << "    Generated " << pending << " siege events in queue.\n";

        if (pending < 100000) {
            res.passed = false;
            res.details += "Subtest 3B Failed: Expected >= 100,000 events for 150,000 days, got " + std::to_string(pending) + "\n";
            return res;
        }

        // Pop all events
        uint64_t popped_count = 0;
        uint64_t prev_ts = 0;
        auto t_pop_start = std::chrono::high_resolution_clock::now();
        while (desMassive.HasNextEvent()) {
            AdversaryEvent ev = desMassive.PopNextEvent();
            if (ev.timestamp_sec < prev_ts) {
                res.passed = false;
                res.details += "Subtest 3B Failed: Chronological inversion during massive siege run!\n";
                return res;
            }
            prev_ts = ev.timestamp_sec;
            popped_count++;
        }
        auto t_pop_end = std::chrono::high_resolution_clock::now();

        double gen_ms = std::chrono::duration<double, std::milli>(t_gen - t_start).count();
        double pop_ms = std::chrono::duration<double, std::milli>(t_pop_end - t_pop_start).count();
        double throughput_events_per_sec = (static_cast<double>(popped_count) / pop_ms) * 1000.0;

        std::cout << "    [PASS] Popped " << popped_count << " events in " << pop_ms << " ms.\n";
        std::cout << "    Throughput: " << std::fixed << std::setprecision(0) << throughput_events_per_sec << " events/sec.\n";

        if (throughput_events_per_sec < 500000.0) {
            res.passed = false;
            res.details += "Subtest 3B Warning/Failure: Throughput below 500,000 events/sec (" +
                           std::to_string(throughput_events_per_sec) + " eps)\n";
            return res;
        }
    }

    // 3C. Memory Leak & RSS Stability under Repeated Cycles (1,000 Reset iterations)
    {
        std::cout << "--> Subtest 3C: Memory Leak Test across 1,000 Consecutive Reset/Advance Cycles...\n";
        long rss_initial = GetRssKb();

        auto cfgCycle = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 1337, 365);
        AdversaryDesEngine desCycle(cfgCycle);

        for (int cycle = 0; cycle < 1000; ++cycle) {
            desCycle.AdvanceDays(365.0);
            desCycle.Reset();
        }

        long rss_final = GetRssKb();
        long rss_growth = rss_final - rss_initial;

        std::cout << "    RSS Initial: " << rss_initial << " KB | RSS Final: " << rss_final
                  << " KB | Growth: " << rss_growth << " KB\n";

        // If there's an unbounded leak over 1,000 cycles with ~260 events each (260,000 allocations),
        // growth would be tens of megabytes. Growth < 5 MB is standard allocator high-water mark.
        if (rss_growth > 5120) { // 5 MB threshold
            res.passed = false;
            res.details += "Subtest 3C Failed: Potential memory leak detected! RSS grew by " +
                           std::to_string(rss_growth) + " KB\n";
            return res;
        }
        std::cout << "    [PASS] Memory stable: zero memory leaks detected over 1,000 engine cycles.\n";
    }

    return res;
}

// ============================================================================
// Main Harness Runner
// ============================================================================
int main() {
    std::cout << "================================================================\n";
    std::cout << "  COL-ADVERSARYD DES ENGINE ADVERSARIAL EMPIRICAL STRESS SUITE  \n";
    std::cout << "================================================================\n";

    std::vector<BenchmarkResults> results;
    results.push_back(TestDesQueueMonotonicity());
    results.push_back(TestPrngOrthogonalityAndBoundarySeeds());
    results.push_back(TestSiegeSaturationAndThroughput());

    std::cout << "\n================================================================\n";
    std::cout << "  EMPIRICAL CHALLENGE SUMMARY RESULTS                           \n";
    std::cout << "================================================================\n";

    bool all_passed = true;
    for (const auto& r : results) {
        std::cout << "[" << (r.passed ? "PASS" : "FAIL") << "] " << r.test_name << "\n";
        if (!r.passed) {
            std::cout << "       Reason: " << r.details << "\n";
            all_passed = false;
        }
    }

    if (all_passed) {
        std::cout << "\n>>> EMPIRICAL VERDICT: ALL ADVERSARIAL STRESS CHALLENGES PASSED <<<\n";
        return 0;
    } else {
        std::cout << "\n>>> EMPIRICAL VERDICT: ADVERSARIAL CHALLENGES FAILED <<<\n";
        return 1;
    }
}
