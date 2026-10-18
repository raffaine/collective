#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <csignal>
#include <atomic>
#include <chrono>
#include <thread>
#include <cstring>
#include <cerrno>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <spdlog/spdlog.h>
#include "uhai/uhai_ring_buffer.hpp"
#include "adversary_engine.hpp"

static std::atomic<bool> g_running{true};

static void SignalHandler(int signum) {
    (void)signum;
    g_running.store(false, std::memory_order_release);
}

int main(int argc, char* argv[]) {
    std::string shm_name = oasis::uhai::DEFAULT_SHM_NAME;
    std::string migration_shm_name = oasis::uhai::DEFAULT_MIGRATION_WAVE_SHM_NAME;
    uint64_t max_ticks = 0;
    int frequency_hz = 10;
    uint8_t threat_level_raw = 1; // Default LEVEL_1_STANDARD
    uint64_t seed = 1337;
    double sim_days_batch = 0.0;  // > 0 triggers accelerated batch simulation

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--shm-name" && i + 1 < argc) {
            shm_name = argv[++i];
        } else if (arg == "--migration-shm" && i + 1 < argc) {
            migration_shm_name = argv[++i];
        } else if ((arg == "--ticks" || arg == "--max-ticks") && i + 1 < argc) {
            max_ticks = std::stoull(argv[++i]);
        } else if (arg == "--frequency" && i + 1 < argc) {
            frequency_hz = std::stoi(argv[++i]);
            if (frequency_hz <= 0 || frequency_hz > 1000) frequency_hz = 10;
        } else if (arg == "--threat-level" && i + 1 < argc) {
            threat_level_raw = static_cast<uint8_t>(std::stoi(argv[++i]));
            if (threat_level_raw > 3) threat_level_raw = 3;
        } else if (arg == "--seed" && i + 1 < argc) {
            seed = std::stoull(argv[++i]);
        } else if ((arg == "--days" || arg == "--sim-days") && i + 1 < argc) {
            sim_days_batch = std::stod(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: col-adversaryd [options]\n"
                      << "  --shm-name <name>      Shared memory object name (default: " << oasis::uhai::DEFAULT_SHM_NAME << ")\n"
                      << "  --migration-shm <name> Migration wave SHM channel (default: " << oasis::uhai::DEFAULT_MIGRATION_WAVE_SHM_NAME << ")\n"
                      << "  --max-ticks <N>        Exit cleanly after executing N real-time ticks\n"
                      << "  --ticks <N>            Alias for --max-ticks\n"
                      << "  --frequency <Hz>       Tick frequency in Hz (default: 10)\n"
                      << "  --threat-level <0-3>   Threat Level: 0=BENIGN, 1=STANDARD, 2=HOSTILE, 3=SIEGE (default: 1)\n"
                      << "  --seed <uint64>        Deterministic PRNG seed (default: 1337)\n"
                      << "  --days <N>             Run accelerated batch simulation for N virtual calendar days and exit\n"
                      << "  --help                 Display this help message\n";
            return 0;
        }
    }

    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    auto level = static_cast<collective::l7::ThreatLevel>(threat_level_raw);
    auto config = collective::l7::AdversaryConfig::CreateDefault(level, seed, 90);
    collective::l7::AdversaryDesEngine des_engine(config);
    oasis::uhai::UhaiMigrationWaveChannel migration_channel(
        oasis::uhai::UhaiMigrationWaveChannel::ChannelMode::Producer, migration_shm_name);

    spdlog::info("[col-adversaryd] Starting Sovereign Stack Layer 7 Adversarial Proxy Daemon (DMZ VLAN 40)...");
    spdlog::info("[col-adversaryd] Threat Level: {} (Level {}) | PRNG Seed: {} | Migration SHM: {}",
                 collective::l7::ThreatLevelToString(level), threat_level_raw, seed, migration_shm_name);

    // Accelerated batch mode (e.g. CLI or verification run)
    if (sim_days_batch > 0.0) {
        spdlog::info("[col-adversaryd] Executing accelerated batch DES across {:.1f} virtual calendar days...", sim_days_batch);
        auto fired = des_engine.AdvanceDays(sim_days_batch);
        for (const auto& ev : fired) {
            spdlog::info("[col-adversaryd] [SIM EVENT #{} @ Day {:>5.2f}] Agency: {:<20} | Type: {:<4} | Impact: ${:>7.2f} | {}",
                         ev.event_id, ev.sim_day, ev.agency_or_monopoly,
                         static_cast<int>(ev.type), ev.financial_impact_fiat_usd, ev.description);
        }
        auto migration_waves = des_engine.PopEmittedMigrationWaves();
        uint64_t waves_relayed = 0;
        for (const auto& wave : migration_waves) {
            auto relay = collective::l7::PropagateMigrationWaveInward(wave, migration_channel, migration_shm_name);
            if (relay.success) {
                waves_relayed++;
                spdlog::info("[col-adversaryd] [MIGRATION WAVE #{} @ HLC {}] Distress: {:.2f} | Cohort: {} | Ring: R{} | Reason: {} | Relayed L7->L6->L5->L4->L3->L2->{}",
                             relay.sanitized_event.wave_id, relay.sanitized_event.timestamp_hlc,
                             relay.sanitized_event.legacy_distress_index, relay.sanitized_event.cohort_size,
                             relay.sanitized_event.initial_trust_ring, relay.sanitized_event.trigger_reason_code,
                             relay.terminal_shm_name);
            }
        }
        const auto& stats = des_engine.GetStats();
        spdlog::info("[col-adversaryd] Batch complete! Fired: {} events (RateAdjust: {}, Insp: {}, ReserveHold: {}, Shutoffs: {}, MigrationWaves: {})",
                     stats.total_events_emitted, stats.utility_rate_adjustments, stats.municipal_inspections,
                     stats.merchant_reserve_holds, stats.utility_shutoffs, waves_relayed);
        spdlog::info("[col-adversaryd] Total simulated fiat fines/tariffs levied: ${:.2f}", stats.total_fines_levied_usd);
        return 0;
    }

    // Attach to UHAI Shared Memory (read-only telemetry drain; Layer 7 DMZ VLAN 40 never writes to /uhai_telemetry)
    void* raw_ptr = nullptr;
    int fd = -1;
    bool is_posix_shm = false;

    fd = ::shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd >= 0) {
        struct stat sb{};
        if (::fstat(fd, &sb) == 0 && sb.st_size < static_cast<off_t>(oasis::uhai::TOTAL_SHM_SIZE)) {
            if (::ftruncate(fd, static_cast<off_t>(oasis::uhai::TOTAL_SHM_SIZE)) != 0) {
                spdlog::warn("[col-adversaryd] ftruncate failed, using in-memory buffer: {}", std::strerror(errno));
                ::close(fd);
                fd = -1;
            }
        }
        if (fd >= 0) {
            raw_ptr = ::mmap(nullptr, oasis::uhai::TOTAL_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if (raw_ptr != MAP_FAILED) {
                is_posix_shm = true;
            } else {
                raw_ptr = nullptr;
                ::close(fd);
                fd = -1;
            }
        }
    }

    std::vector<uint8_t> fallback_memory;
    if (!raw_ptr) {
        spdlog::warn("[col-adversaryd] POSIX shm unavailable; activating in-memory UHAI fallback.");
        fallback_memory.resize(oasis::uhai::TOTAL_SHM_SIZE + 64, 0);
        void* unaligned = fallback_memory.data();
        size_t space = fallback_memory.size();
        raw_ptr = std::align(64, oasis::uhai::TOTAL_SHM_SIZE, unaligned, space);
    }

    auto* header = reinterpret_cast<oasis::uhai::SharedRingHeader*>(raw_ptr);
    if (header->magic_signature != oasis::uhai::UHAI_MAGIC) {
        header->write_index.store(0, std::memory_order_relaxed);
        header->dropped_frames.store(0, std::memory_order_relaxed);
        header->read_index.store(0, std::memory_order_relaxed);
        header->capacity = oasis::uhai::RING_CAPACITY;
        header->element_size = sizeof(oasis::uhai::TelemetrySlot);
        header->magic_signature = oasis::uhai::UHAI_MAGIC;
        header->version = oasis::uhai::UHAI_VERSION;
    }

    spdlog::info("[col-adversaryd] UHAI IPC ring attached. DES & Migration Wave engine active.");

    oasis::uhai::LockFreeSpscRing<oasis::uhai::TelemetrySlot, oasis::uhai::RING_CAPACITY> ring(raw_ptr);

    using clock = std::chrono::steady_clock;
    const auto tick_interval = std::chrono::microseconds(1'000'000 / frequency_hz);
    auto next_tick_deadline = clock::now() + tick_interval;

    uint64_t adv_tick = 0;
    uint64_t total_telemetry_drained = 0;
    uint64_t total_migration_waves_emitted = 0;
    constexpr double SIM_DAYS_PER_REAL_SECOND = 1.0; // 1 real second = 1 virtual calendar day

    while (g_running.load(std::memory_order_relaxed)) {
        adv_tick++;

        // Drain telemetry from UHAI buffer
        oasis::uhai::TelemetrySample sample{};
        bool drained_any = false;
        while (ring.Pop(sample)) {
            total_telemetry_drained++;
            drained_any = true;
        }

        // Advance DES virtual time
        double virtual_days_elapsed = (static_cast<double>(adv_tick) / static_cast<double>(frequency_hz)) * SIM_DAYS_PER_REAL_SECOND;
        auto fired = des_engine.AdvanceDays(virtual_days_elapsed);

        for (const auto& ev : fired) {
            spdlog::warn("[col-adversaryd] [ADVERSARY EVENT #{} @ Day {:>5.2f}] Agency: {} | Impact: ${:.2f} | {}",
                         ev.event_id, ev.sim_day, ev.agency_or_monopoly, ev.financial_impact_fiat_usd, ev.description);
        }

        // Dispatch any scheduled MigrationWaveArrivalEvents strictly inward (L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI)
        auto migration_waves = des_engine.PopEmittedMigrationWaves();
        for (const auto& wave : migration_waves) {
            auto relay = collective::l7::PropagateMigrationWaveInward(wave, migration_channel, migration_shm_name);
            if (relay.success) {
                total_migration_waves_emitted++;
                spdlog::warn("[col-adversaryd] [MIGRATION WAVE #{} @ Day {:>5.2f}] Distress: {:.2f} | Cohort: {} | Ring: R{} | Relayed inward L7->L6->L5->L4->L3->L2->{}",
                             relay.sanitized_event.wave_id, des_engine.GetCurrentSimDay(),
                             relay.sanitized_event.legacy_distress_index, relay.sanitized_event.cohort_size,
                             relay.sanitized_event.initial_trust_ring, relay.terminal_shm_name);
            }
        }

        if ((max_ticks > 0 && max_ticks <= 50) || (adv_tick % 10 == 0) || !fired.empty() || !migration_waves.empty() || drained_any) {
            spdlog::info("[col-adversaryd] Tick: {:>6} ({} Hz) | SimDay: {:>5.2f} | Distress: {:.2f} | EventsFired: {} | MigrationWaves: {} | Pending: {} | Drained: {}",
                         adv_tick, frequency_hz, des_engine.GetCurrentSimDay(),
                         des_engine.GetCurrentLegacyDistressIndex(),
                         des_engine.GetStats().total_events_emitted, total_migration_waves_emitted,
                         des_engine.PendingEventsCount(), total_telemetry_drained);
        }

        if (max_ticks > 0 && adv_tick >= max_ticks) {
            spdlog::info("[col-adversaryd] Reached target tick count ({}). Initiating clean shutdown.", max_ticks);
            break;
        }

        std::this_thread::sleep_until(next_tick_deadline);
        next_tick_deadline += tick_interval;
        if (next_tick_deadline < clock::now()) {
            next_tick_deadline = clock::now() + tick_interval;
        }
    }

    const auto& stats = des_engine.GetStats();
    spdlog::info("[col-adversaryd] Layer 7 completed {} ticks. Total events fired: {}. Migration waves: {}. Total fines levied: ${:.2f}. Clean exit.",
                 adv_tick, stats.total_events_emitted, total_migration_waves_emitted, stats.total_fines_levied_usd);

    if (is_posix_shm && raw_ptr) {
        ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
    }
    if (fd >= 0) {
        ::close(fd);
    }

    return 0;
}
