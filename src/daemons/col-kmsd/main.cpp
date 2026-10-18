#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "kms_assimilation.hpp"

/**
 * =============================================================================
 * Sovereign Stack Layer 5 KMS & Trust Ring Assimilation Daemon (col-kmsd)
 * =============================================================================
 *
 * Architecture & Responsibilities (SS-EP5-007, SS-EP7-007, EPIC-11):
 * - Consumes MigrationWaveArrivalEvent cohorts from /uhai_migration_waves and
 *   onboards newly arrived migrants into Ring R6 (Unverified Migrant) with an
 *   inviolable humanitarian survival floor B_t, 0.0 BCC credit limit, and 0.0
 *   L5 governance voting weight.
 * - Evaluates Layer 3 homeostasis (>= 7 consecutive sheltered days) AND Layer 4
 *   verified PoTW sweat equity (>= 36.0 MJ / 10.0 Exergy Credits) for R6 -> R3
 *   promotion (Probationary Resident, 25.0 EC BCC credit limit). Passive rest
 *   never promotes.
 * - Executes 2-of-3 FROST threshold signing ceremonies (SCN-TAU-GENESIS) by
 *   existing R1 Vouched Stewards after >= 30 consecutive days in R3 and 0
 *   unresolved mediations for R3 -> R1 promotion (Vouched Steward, BBS+
 *   credential, 1.0 voting weight).
 * - Emits TrustRingPromotionEvent downward via /uhai_trust_promotions.
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
              << "  --once                Execute a single assimilation verification cycle and exit\n"
              << "  --ticks <N>           Run for N ticks then terminate cleanly (0 = run forever)\n"
              << "  --max-ticks <N>       Alias for --ticks\n"
              << "  --shm-name <name>     UHAI trust promotion SHM name (default: "
              << oasis::uhai::DEFAULT_TRUST_PROMOTION_SHM_NAME << ")\n"
              << "  --help, -h            Display this help message\n";
}
} // namespace

int main(int argc, char* argv[]) {
    bool run_once = false;
    int max_ticks = 0;
    std::string promotion_shm_name = oasis::uhai::DEFAULT_TRUST_PROMOTION_SHM_NAME;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintHelp(argv[0]);
            return 0;
        } else if (arg == "--once") {
            run_once = true;
            max_ticks = 1;
        } else if ((arg == "--ticks" || arg == "--max-ticks") && i + 1 < argc) {
            max_ticks = std::atoi(argv[++i]);
        } else if (arg == "--shm-name" && i + 1 < argc) {
            promotion_shm_name = argv[++i];
        } else {
            std::cerr << "[col-kmsd] Unknown argument: " << arg << "\n";
            PrintHelp(argv[0]);
            return 1;
        }
    }

    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    oasis::uhai::UhaiMigrationWaveChannel wave_channel(
        oasis::uhai::UhaiMigrationWaveChannel::ChannelMode::Consumer,
        oasis::uhai::DEFAULT_MIGRATION_WAVE_SHM_NAME);
    oasis::uhai::UhaiTrustPromotionChannel promotion_channel(
        oasis::uhai::UhaiTrustPromotionChannel::ChannelMode::Producer,
        promotion_shm_name);

    collective::l5::KmsAssimilationEngine engine;

    if (run_once) {
        oasis::uhai::MigrationWaveArrivalEvent demo_wave{};
        demo_wave.wave_id = 1;
        demo_wave.timestamp_hlc = 1000;
        demo_wave.legacy_distress_index = 0.82f;
        demo_wave.cohort_size = 3;
        demo_wave.trigger_reason_code = static_cast<uint8_t>(
            oasis::uhai::MigrationTriggerReason::CLIMATE_SHOCK);
        demo_wave.initial_trust_ring = 6;

        auto cohort = engine.RegisterMigrantCohortFromWave(demo_wave);
        if (!cohort.empty()) {
            const uint64_t lead_did_hash = cohort.front().migrant_did_hash;
            engine.RecordLayer3HomeostasisAndPoTWAttestation(
                lead_did_hash, 7, 36.0f, 1007, &promotion_channel);
            engine.ExecuteFrostStewardVouchCeremony(
                lead_did_hash,
                30,
                {"did:peer:steward:alpha", "did:peer:steward:beta"},
                1037,
                &promotion_channel);
        }
        std::cout << "[col-kmsd] Completed single assimilation cycle (migrants="
                  << engine.GetAllMigrants().size()
                  << ", promotions=" << engine.GetEmittedPromotions().size() << ")\n";
        return 0;
    }

    int tick = 0;
    while (g_running.load(std::memory_order_acquire)) {
        while (auto wave_opt = wave_channel.Pop()) {
            engine.RegisterMigrantCohortFromWave(*wave_opt);
        }
        ++tick;
        if (max_ticks > 0 && tick >= max_ticks) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
