#include <iostream>
#include <csignal>
#include <atomic>
#include <chrono>
#include <thread>
#include <string>
#include <cstring>
#include <cerrno>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <spdlog/spdlog.h>
#include "uhai/uhai_ring_buffer.hpp"

static std::atomic<bool> g_running{true};

static void SignalHandler(int signum) {
    (void)signum;
    g_running.store(false, std::memory_order_release);
}

int main(int argc, char* argv[]) {
    std::string shm_name = oasis::uhai::DEFAULT_SHM_NAME;
    uint64_t max_ticks = 0;
    int frequency_hz = 10;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--shm-name" && i + 1 < argc) {
            shm_name = argv[++i];
        } else if ((arg == "--ticks" || arg == "--max-ticks") && i + 1 < argc) {
            max_ticks = std::stoull(argv[++i]);
        } else if (arg == "--frequency" && i + 1 < argc) {
            frequency_hz = std::stoi(argv[++i]);
            if (frequency_hz <= 0 || frequency_hz > 1000) frequency_hz = 10;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: col-execd [options]\n"
                      << "  --shm-name <name>    Shared memory object name (default: "
                      << oasis::uhai::DEFAULT_SHM_NAME << ")\n"
                      << "  --max-ticks <N>      Exit cleanly after executing N deterministic ticks\n"
                      << "  --ticks <N>          Alias for --max-ticks\n"
                      << "  --frequency <Hz>     Tick frequency in Hz (default: 10)\n"
                      << "  --help               Display this help message\n";
            return 0;
        }
    }

    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    spdlog::info("[col-execd] Starting Sovereign Stack Layer 4 Deterministic Execution Daemon...");
    spdlog::info("[col-execd] Target POSIX shm: {} | Frequency: {} Hz", shm_name, frequency_hz);

    int fd = ::shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        spdlog::error("[col-execd] Failed to shm_open '{}': {} ({})",
                      shm_name, std::strerror(errno), errno);
        return 1;
    }

    struct stat sb{};
    if (::fstat(fd, &sb) == 0 && sb.st_size < static_cast<off_t>(oasis::uhai::TOTAL_SHM_SIZE)) {
        if (::ftruncate(fd, static_cast<off_t>(oasis::uhai::TOTAL_SHM_SIZE)) != 0) {
            spdlog::error("[col-execd] Failed to ftruncate shm to {} bytes: {}",
                          oasis::uhai::TOTAL_SHM_SIZE, std::strerror(errno));
            ::close(fd);
            return 1;
        }
    }

    void* raw_ptr = ::mmap(nullptr, oasis::uhai::TOTAL_SHM_SIZE,
                           PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (raw_ptr == MAP_FAILED) {
        spdlog::error("[col-execd] Failed to mmap shared memory: {}", std::strerror(errno));
        ::close(fd);
        return 1;
    }

    auto* header = reinterpret_cast<oasis::uhai::SharedRingHeader*>(raw_ptr);

    // If magic is not yet initialized (daemon launched before producer), initialize header
    if (header->magic_signature != oasis::uhai::UHAI_MAGIC) {
        spdlog::warn("[col-execd] Header magic uninitialized (0x{:08x}). Initializing default layout...",
                     header->magic_signature);
        header->write_index.store(0, std::memory_order_relaxed);
        header->dropped_frames.store(0, std::memory_order_relaxed);
        header->read_index.store(0, std::memory_order_relaxed);
        header->capacity = oasis::uhai::RING_CAPACITY;
        header->element_size = sizeof(oasis::uhai::TelemetrySlot);
        header->magic_signature = oasis::uhai::UHAI_MAGIC;
        header->version = oasis::uhai::UHAI_VERSION;
    }

    if (header->magic_signature != oasis::uhai::UHAI_MAGIC) {
        spdlog::critical("[col-execd] Invalid magic signature: 0x{:08x} (expected 0x{:08x})",
                         header->magic_signature, oasis::uhai::UHAI_MAGIC);
        ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
        ::close(fd);
        return 1;
    }

    if (header->version != oasis::uhai::UHAI_VERSION) {
        spdlog::critical("[col-execd] Unsupported UHAI version: {} (expected {})",
                         header->version, oasis::uhai::UHAI_VERSION);
        ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
        ::close(fd);
        return 1;
    }

    spdlog::info("[col-execd] Successfully attached to UHAI IPC ring buffer. Deterministic VM active.");

    oasis::uhai::LockFreeSpscRing<oasis::uhai::TelemetrySlot, oasis::uhai::RING_CAPACITY> ring(raw_ptr);

    using clock = std::chrono::steady_clock;
    const auto tick_interval = std::chrono::microseconds(1'000'000 / frequency_hz); // 100,000 us for 10 Hz
    auto next_tick_deadline = clock::now() + tick_interval;

    uint64_t l4_tick = 0;
    uint64_t total_telemetry_drained = 0;
    oasis::uhai::TelemetrySample latest_sample{};

    while (g_running.load(std::memory_order_relaxed)) {
        l4_tick++;

        // Drain available telemetry samples from UHAI buffer
        oasis::uhai::TelemetrySample sample{};
        bool drained_any = false;
        while (ring.Pop(sample)) {
            latest_sample = sample;
            total_telemetry_drained++;
            drained_any = true;
        }

        // Layer 4 BPMN VM deterministic execution step stub
        if ((max_ticks > 0 && max_ticks <= 50) || (l4_tick % 10 == 0) || drained_any) {
            spdlog::info("[col-execd] Tick: {:>6} ({} Hz) | Frame: {:>6} | FPS: {:>5.1f} | Stress: {:>4.2f} | Drained: {}",
                         l4_tick, frequency_hz, latest_sample.frame_count, latest_sample.fps,
                         latest_sample.founder_stress, total_telemetry_drained);
        }

        if (max_ticks > 0 && l4_tick >= max_ticks) {
            spdlog::info("[col-execd] Reached target tick count ({}). Initiating shutdown.", max_ticks);
            break;
        }

        // Drift-free scheduling via sleep_until
        std::this_thread::sleep_until(next_tick_deadline);
        next_tick_deadline += tick_interval;
        if (next_tick_deadline < clock::now()) {
            next_tick_deadline = clock::now() + tick_interval; // Drift guard
        }
    }

    spdlog::info("[col-execd] Completed {} ticks. Drained {} telemetry samples. Clean exit.",
                 l4_tick, total_telemetry_drained);

    ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
    ::close(fd);
    return 0;
}
