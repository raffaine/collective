#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "swarm_engine.hpp"

/**
 * =============================================================================
 * Sovereign Stack Layer 3/6 Peer-Cluster Swarm Daemon (col-swarmd)
 * =============================================================================
 *
 * Models 3 multi-household shared-governance peer node clusters over Layer 3:
 *   - cluster:north_oak_commons   (North-Oak 5-Household Garden & Kitchen Cluster)
 *   - cluster:creekside_guild     (Creekside 4-Garage Tool & Timber Cluster)
 *   - cluster:sunnyside_care_ring (Sunnyside 6-Family Care & Hearth Cluster)
 *
 * Uses strictly fixed-point integer math and SplitMix64 deterministic PRNG
 * (zero IEEE-754 floats) to publish L6 cluster need requests, post L3 surplus
 * offers, and claim/complete R1 tasks with zero-sum BCC exergy clearing.
 * =============================================================================
 */

namespace {

std::atomic<bool> g_running{true};

void SignalHandler(int signum) {
    (void)signum;
    g_running.store(false, std::memory_order_release);
}

void PrintHelp(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [options]\n"
              << "Options:\n"
              << "  --once                Execute a single deterministic swarm tick and exit\n"
              << "  --max-ticks <N>       Exit cleanly after executing N deterministic ticks\n"
              << "  --ticks <N>           Alias for --max-ticks\n"
              << "  --frequency <Hz>      Tick frequency in Hz (default: 2)\n"
              << "  --seed <uint64>       Deterministic SplitMix64 integer PRNG seed\n"
              << "  --snapshot-json       Print final node snapshot JSON on exit\n"
              << "  --help, -h            Display this help message\n";
}

} // namespace

int main(int argc, char* argv[]) {
    uint64_t max_ticks = 0ULL;
    int frequency_hz = 2;
    uint64_t seed = 0xC011EC717E2026ULL;
    bool print_snapshot = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintHelp(argv[0]);
            return 0;
        } else if (arg == "--once") {
            max_ticks = 1ULL;
        } else if ((arg == "--max-ticks" || arg == "--ticks") && (i + 1) < argc) {
            max_ticks = std::strtoull(argv[++i], nullptr, 10);
        } else if (arg == "--frequency" && (i + 1) < argc) {
            frequency_hz = std::atoi(argv[++i]);
            if (frequency_hz <= 0 || frequency_hz > 1000) {
                frequency_hz = 2;
            }
        } else if (arg == "--seed" && (i + 1) < argc) {
            seed = std::strtoull(argv[++i], nullptr, 10);
        } else if (arg == "--snapshot-json") {
            print_snapshot = true;
        } else {
            std::cerr << "[col-swarmd] Unknown argument: " << arg << "\n";
            PrintHelp(argv[0]);
            return 1;
        }
    }

    std::cout << std::unitbuf;
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    oasis::stack::SharedStackRuntime runtime{};
    oasis::swarm::SwarmState swarm{seed};

    std::cout << "[col-swarmd] Starting Peer-Cluster Neighborhood Swarm Daemon (frequency="
              << frequency_hz << "Hz, clusters=" << swarm.clusters.size()
              << ", jsonld=" << (runtime.BootstrappedFromJsonLd() ? "true" : "false") << ")\n";

    // Release one baseline R1 garage task in standalone daemon mode so peer-cluster BCC clearing is verified
    (void)runtime.ReleaseTaskToR1Peers("task:fab_1_mill_raised_bed");

    const auto tick_interval = std::chrono::microseconds(1'000'000 / frequency_hz);
    uint64_t tick = 0ULL;

    while (g_running.load(std::memory_order_acquire)) {
        ++tick;
        oasis::swarm::StepPeerClusterSwarm(runtime, swarm, tick);

        if (!runtime.GetLedger().VerifyPairwiseZeroSumInvariant()) {
            std::cerr << "[col-swarmd] FATAL: Pairwise BCC zero-sum invariant violated at tick " << tick << "!\n";
            return 1;
        }

        if (max_ticks > 0ULL && tick >= max_ticks) {
            break;
        }
        std::this_thread::sleep_for(tick_interval);
    }

    if (print_snapshot) {
        std::cout << runtime.SerializeFullNodeSnapshotJson();
    } else {
        std::cout << "[col-swarmd] Completed " << tick
                  << " deterministic swarm ticks (clusters=" << swarm.clusters.size()
                  << ", events=" << swarm.total_events_emitted
                  << ", bcc_settled_uj=" << swarm.total_bcc_settled_uj
                  << ", bcc_zero_sum="
                  << (runtime.GetLedger().VerifyPairwiseZeroSumInvariant() ? "ok" : "err")
                  << "). Clean exit.\n";
    }
    return 0;
}
