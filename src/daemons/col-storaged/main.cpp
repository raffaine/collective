#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "storaged_engine.hpp"

/**
 * =============================================================================
 * Sovereign Stack Layer 3 Content-Addressed Merkle DAG & BCC Daemon (col-storaged)
 * =============================================================================
 *
 * Enforces:
 * - Content-addressed Merkle DAG & SBOM root verification (>= 3 builder consensus)
 * - Pairwise Bilateral Clearing Channel (BCC) zero-sum invariant:
 *     (balance_u_to_v_uj + balance_v_to_u_uj) == 0
 * - Constitutional Non-Commensurability Invariant:
 *     AttemptTokenizeBaselineCare() unconditionally returns false
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
              << "  --once                Execute a single Layer 3 ledger verification cycle and exit\n"
              << "  --max-ticks <N>       Exit cleanly after executing N deterministic ticks\n"
              << "  --ticks <N>           Alias for --max-ticks\n"
              << "  --frequency <Hz>      Tick frequency in Hz (default: 10)\n"
              << "  --help, -h            Display this help message\n";
}
} // namespace

int main(int argc, char* argv[]) {
    uint64_t max_ticks = 0;
    int frequency_hz = 10;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintHelp(argv[0]);
            return 0;
        } else if (arg == "--once") {
            max_ticks = 1;
        } else if ((arg == "--max-ticks" || arg == "--ticks") && i + 1 < argc) {
            max_ticks = std::stoull(argv[++i]);
        } else if (arg == "--frequency" && i + 1 < argc) {
            frequency_hz = std::stoi(argv[++i]);
            if (frequency_hz <= 0 || frequency_hz > 1000) {
                frequency_hz = 10;
            }
        }
    }

    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    oasis::daemons::storaged::StoragedEngine engine;
    oasis::daemons::storaged::PairwiseBccChannel channel{};
    channel.peer_did_hash = 0x9988776655443322ULL;

    // Verify Constitutional Non-Commensurability Invariant on startup
    if (oasis::daemons::storaged::AttemptTokenizeBaselineCare(1U)) {
        std::cerr << "[col-storaged] FATAL: Baseline care tokenization invariant violated!\n";
        return 1;
    }

    oasis::daemons::storaged::MerkleArtifactRecord genesis_artifact{};
    genesis_artifact.artifact_cid_hash = 0xCAFEBABE00000001ULL;
    genesis_artifact.sbom_merkle_root_hash = 0xE3B0C44298FC1C14ULL;
    genesis_artifact.agreed_sha256_prefix = 0x9F86D081884C7D65ULL;
    genesis_artifact.builder_consensus_count = 3U;
    genesis_artifact.asset_category = 6U;
    (void)engine.CommitMerkleArtifact(genesis_artifact);

    const auto tick_interval = std::chrono::microseconds(1'000'000 / frequency_hz);
    uint64_t tick = 0;

    while (g_running.load(std::memory_order_acquire)) {
        ++tick;
        (void)oasis::daemons::storaged::SettlePairwiseBccTransfer(channel, 3600000ULL);

        if (max_ticks > 0 && tick >= max_ticks) {
            break;
        }
        std::this_thread::sleep_for(tick_interval);
    }

    std::cout << "[col-storaged] Completed " << tick
              << " deterministic ticks (artifacts=" << engine.GetArtifactCount()
              << ", bcc_zero_sum=" << ((channel.balance_u_to_v_uj + channel.balance_v_to_u_uj) == 0 ? "ok" : "err")
              << "). Clean exit.\n";
    return 0;
}
