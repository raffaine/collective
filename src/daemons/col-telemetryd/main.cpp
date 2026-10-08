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
    uint64_t max_frames = 0;
    int timeout_ms = -1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--shm-name" && i + 1 < argc) {
            shm_name = argv[++i];
        } else if (arg == "--max-frames" && i + 1 < argc) {
            max_frames = std::stoull(argv[++i]);
        } else if (arg == "--timeout-ms" && i + 1 < argc) {
            timeout_ms = std::stoi(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: col-telemetryd [options]\n"
                      << "  --shm-name <name>    Shared memory object name (default: "
                      << oasis::uhai::DEFAULT_SHM_NAME << ")\n"
                      << "  --max-frames <N>     Exit after draining N frames\n"
                      << "  --timeout-ms <ms>    Exit if idle/inactive for ms milliseconds\n"
                      << "  --help               Display this help message\n";
            return 0;
        }
    }

    // Register signal handlers for clean shutdown
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    spdlog::info("[col-telemetryd] Starting Sovereign Stack Telemetry Daemon...");
    spdlog::info("[col-telemetryd] Target POSIX shm: {}", shm_name);

    // Open or create POSIX shared memory object
    int fd = ::shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        spdlog::error("[col-telemetryd] Failed to shm_open '{}': {} ({})",
                      shm_name, std::strerror(errno), errno);
        return 1;
    }

    struct stat sb{};
    if (::fstat(fd, &sb) == 0 && sb.st_size < static_cast<off_t>(oasis::uhai::TOTAL_SHM_SIZE)) {
        if (::ftruncate(fd, static_cast<off_t>(oasis::uhai::TOTAL_SHM_SIZE)) != 0) {
            spdlog::error("[col-telemetryd] Failed to ftruncate shm to {} bytes: {}",
                          oasis::uhai::TOTAL_SHM_SIZE, std::strerror(errno));
            ::close(fd);
            return 1;
        }
    }

    void* raw_ptr = ::mmap(nullptr, oasis::uhai::TOTAL_SHM_SIZE,
                           PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (raw_ptr == MAP_FAILED) {
        spdlog::error("[col-telemetryd] Failed to mmap shared memory: {}", std::strerror(errno));
        ::close(fd);
        return 1;
    }

    auto* header = reinterpret_cast<oasis::uhai::SharedRingHeader*>(raw_ptr);

    // If magic is not yet initialized (daemon launched before producer), initialize header
    if (header->magic_signature != oasis::uhai::UHAI_MAGIC) {
        spdlog::warn("[col-telemetryd] Header magic uninitialized (0x{:08x}). Initializing default layout...",
                     header->magic_signature);
        header->write_index.store(0, std::memory_order_relaxed);
        header->dropped_frames.store(0, std::memory_order_relaxed);
        header->read_index.store(0, std::memory_order_relaxed);
        header->capacity = oasis::uhai::RING_CAPACITY;
        header->element_size = sizeof(oasis::uhai::TelemetrySlot);
        header->magic_signature = oasis::uhai::UHAI_MAGIC;
        header->version = oasis::uhai::UHAI_VERSION;
    }

    // Validate header invariants
    if (header->magic_signature != oasis::uhai::UHAI_MAGIC) {
        spdlog::critical("[col-telemetryd] Invalid magic signature: 0x{:08x} (expected 0x{:08x})",
                         header->magic_signature, oasis::uhai::UHAI_MAGIC);
        ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
        ::close(fd);
        return 1;
    }

    if (header->version != oasis::uhai::UHAI_VERSION) {
        spdlog::critical("[col-telemetryd] Unsupported UHAI version: {} (expected {})",
                         header->version, oasis::uhai::UHAI_VERSION);
        ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
        ::close(fd);
        return 1;
    }

    spdlog::info("[col-telemetryd] Successfully attached! Capacity: {}, ElementSize: {}B, Version: {}",
                 header->capacity, header->element_size, header->version);

    oasis::uhai::LockFreeSpscRing<oasis::uhai::TelemetrySlot, oasis::uhai::RING_CAPACITY> ring(raw_ptr);

    uint64_t total_drained = 0;
    auto last_drain_time = std::chrono::steady_clock::now();
    auto start_time = std::chrono::steady_clock::now();

    oasis::uhai::TelemetrySample sample{};

    // Non-blocking drain loop
    while (g_running.load(std::memory_order_relaxed)) {
        bool drained_any = false;
        while (ring.Pop(sample)) {
            drained_any = true;
            total_drained++;
            last_drain_time = std::chrono::steady_clock::now();

            spdlog::info("[col-telemetryd] Frame: {:>6} | FPS: {:>5.1f} | Stress: {:>4.2f} | "
                         "Memories: {:>2} | Biome: {} | Quality: 0x{:02x} | TS: {} ns",
                         sample.frame_count, sample.fps, sample.founder_stress,
                         sample.memory_slot_count, sample.biome_id, sample.quality_flags,
                         sample.timestamp_ns);

            if (max_frames > 0 && total_drained >= max_frames) {
                spdlog::info("[col-telemetryd] Reached target frame count ({})", max_frames);
                g_running.store(false, std::memory_order_relaxed);
                break;
            }
        }

        if (!drained_any) {
            if (timeout_ms > 0) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_drain_time).count();
                if (elapsed >= timeout_ms && total_drained > 0) {
                    spdlog::info("[col-telemetryd] Inactivity timeout ({} ms) reached", timeout_ms);
                    break;
                }
                auto total_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
                if (total_elapsed >= timeout_ms && total_drained == 0) {
                    spdlog::info("[col-telemetryd] Startup timeout ({} ms) reached", timeout_ms);
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    uint64_t dropped = header->dropped_frames.load(std::memory_order_relaxed);
    spdlog::info("[col-telemetryd] Drained {} total samples. Header recorded dropped frames: {}",
                 total_drained, dropped);
    spdlog::info("[col-telemetryd] Clean shutdown complete.");

    ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
    ::close(fd);
    return 0;
}
