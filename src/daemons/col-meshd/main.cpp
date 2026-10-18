#include <iostream>
#include <csignal>
#include <atomic>
#include <chrono>
#include <thread>
#include <string>
#include <cstring>
#include <cerrno>
#include <vector>
#include <queue>
#include <memory>
#include <iomanip>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <spdlog/spdlog.h>
#include "uhai/uhai_ring_buffer.hpp"

/**
 * =============================================================================
 * Sovereign Stack Layer 2/3 Cybernetic Mesh Daemon (col-meshd)
 * =============================================================================
 *
 * Architecture & Responsibilities (PROJECT.md & doc/book_collective/):
 * - Standalone POSIX daemon mediating between cybernetic mesh bearers (LoRa,
 *   802.11s Wi-Fi, Reticulum RNS, WireGuard) and local UHAI IPC shared memory.
 * - Enforces Anti-Bleed Invariant 3: Oasis simulation engine contains zero
 *   raw network sockets (<sys/socket.h>, libp2p, WebRTC, LoRa decoders). All
 *   multiplayer mesh traffic enters the engine strictly via the lock-free
 *   UHAI SPSC shared-memory ring buffer (/uhai_flow).
 * - Enforces Quality Gate G6: Synthetic SITL / ghost packets are strictly
 *   tagged with QUALITY_SIMULATED = 0x01.
 * - Provides drift-free POSIX execution loop via std::this_thread::sleep_until.
 * - Maps POSIX shared memory with automatic posix_memalign fallback.
 *
 * Architecture Diagram:
 * +-----------------------------------------------------------------+
 * |                     col-meshd POSIX Daemon                      |
 * |                                                                 |
 * |  [libp2p Gossipsub / Reticulum RNS]  <-- Extension Point        |
 * |                 |                                               |
 * |                 v                                               |
 * |          IMeshTransport                                         |
 * |                 |                                               |
 * |        +--------+--------+                                      |
 * |        |                 |                                      |
 * |  GhostMeshTransport  Libp2pMeshTransport (Future Production)    |
 * |        |                 |                                      |
 * |        +--------+--------+                                      |
 * |                 |                                               |
 * |                 v                                               |
 * |     LockFreeSpscRing<FlowContractSlot, 1024>                    |
 * |     (/dev/shm/uhai_flow | posix_memalign fallback)              |
 * +-----------------+-----------------------------------------------+
 *                   | Zero-Copy SPSC Ring Buffer
 *                   v
 * +-----------------+-----------------------------------------------+
 * |               Oasis Engine (core/1_simulation/src)              |
 * |  - TemporalFlowSystem (flecs::OnUpdate)                         |
 * |  - 1x sim_tick decoupled resource accretion                     |
 * |  - Zero network sockets (Anti-Bleed Invariant 3)                |
 * +-----------------------------------------------------------------+
 * =============================================================================
 */

static std::atomic<bool> g_running{true};

static void SignalHandler(int signum) {
    (void)signum;
    g_running.store(false, std::memory_order_release);
}

/**
 * =============================================================================
 * Pluggable Mesh Transport Abstraction (IMeshTransport)
 * =============================================================================
 * Decouples physical/virtual mesh transport from the UHAI shared-memory push
 * path. Allows transparent swapping between GhostMeshTransport (simulation)
 * and Libp2pMeshTransport (production Gossipsub/Reticulum).
 */
class IMeshTransport {
public:
    virtual ~IMeshTransport() = default;

    /// Initialize the transport layer (bind sockets, subscribe to topics, etc.)
    virtual void Initialize() = 0;

    /// Poll for inbound FlowContract packets received from mesh peers
    virtual bool PollInbound(oasis::uhai::FlowContract& out_contract) = 0;

    /// Transmit outbound FlowContract packet over the mesh network
    virtual bool SendOutbound(const oasis::uhai::FlowContract& in_contract) = 0;
};

/**
 * =============================================================================
 * Ghost Mesh Transport (GhostMeshTransport)
 * =============================================================================
 * Simulates a remote peer node running accelerated forward projections at 100x
 * speed (VSM System 4 Forward Projection, doc/book_collective/ Chapter 3).
 *
 * Emulates:
 * 1. Asynchronous generation of FlowContract packets for electricity and water.
 * 2. Quality Gate G6 compliance: all packets carry QUALITY_SIMULATED (0x01).
 * 3. Abrupt future contract termination: at a configurable abort tick,
 *    publishes a cancellation packet (status = TERMINATED) to verify that
 *    the consumer clamps resource flow without corruption or lingering leak.
 */
class GhostMeshTransport : public IMeshTransport {
public:
    GhostMeshTransport(uint32_t ghost_agents = 1, uint64_t abort_tick = 0, uint32_t frequency_hz = 10)
        : ghost_agents_(ghost_agents), abort_tick_(abort_tick), frequency_hz_(frequency_hz) {}

    void Initialize() override {
        spdlog::info("[GhostMeshTransport] Initializing 100x Ghost Agent Mesh Simulator...");
        spdlog::info("[GhostMeshTransport] Ghost agents: {} | Abort tick: {} | Frequency: {} Hz",
                     ghost_agents_, abort_tick_, frequency_hz_);
    }

    /// Advance simulation clock and produce scheduled ghost contract events
    void Step(uint64_t daemon_tick) {
        current_tick_ = daemon_tick;

        // At tick 1: Ghost Agent 1 (Node 0x100A) issues an active electricity flow contract
        if (current_tick_ == 1 && !electricity_contract_emitted_) {
            electricity_contract_emitted_ = true;
            oasis::uhai::FlowContract contract{};
            contract.contract_id = electricity_contract_id_;
            contract.sender_node_id = 0x100A; // Ghost Node A DID handle
            contract.consumer_node_id = 0x0001; // Local human node
            contract.start_logical_tick = 50;
            contract.end_logical_tick = 500;
            contract.rate_per_tick = 0.050; // 0.05 kWh per logical tick
            contract.resource_type = static_cast<uint16_t>(oasis::uhai::FlowResourceType::ELECTRICITY);
            contract.status = static_cast<uint8_t>(oasis::uhai::FlowContractStatus::ACTIVE);
            contract.quality_flags = oasis::uhai::QualityFlags::QUALITY_SIMULATED; // Gate G6
            contract.reserved = 0;
            std::memcpy(contract.auth_signature, "GHOST100", 8);

            inbound_queue_.push(contract);
            spdlog::info("[GhostMeshTransport] [100x Ghost Node 0x100A] Emitted ELECTRICITY contract ID: {} "
                         "([50..500) @ 0.050 kWh/tick)", contract.contract_id);
        }

        // At tick 2: If >= 2 agents, Ghost Agent 2 (Node 0x100B) issues a water flow contract
        if (current_tick_ == 2 && ghost_agents_ >= 2 && !water_contract_emitted_) {
            water_contract_emitted_ = true;
            oasis::uhai::FlowContract contract{};
            contract.contract_id = water_contract_id_;
            contract.sender_node_id = 0x100B; // Ghost Node B DID handle
            contract.consumer_node_id = 0x0001;
            contract.start_logical_tick = 100;
            contract.end_logical_tick = 600;
            contract.rate_per_tick = 2.500; // 2.50 Liters per logical tick
            contract.resource_type = static_cast<uint16_t>(oasis::uhai::FlowResourceType::WATER);
            contract.status = static_cast<uint8_t>(oasis::uhai::FlowContractStatus::ACTIVE);
            contract.quality_flags = oasis::uhai::QualityFlags::QUALITY_SIMULATED; // Gate G6
            contract.reserved = 0;
            std::memcpy(contract.auth_signature, "GHOST100", 8);

            inbound_queue_.push(contract);
            spdlog::info("[GhostMeshTransport] [100x Ghost Node 0x100B] Emitted WATER contract ID: {} "
                         "([100..600) @ 2.500 L/tick)", contract.contract_id);
        }

        // At tick 3: If >= 3 agents, Ghost Agent 3 (Node 0x100C) issues a calories flow contract
        if (current_tick_ == 3 && ghost_agents_ >= 3 && !calories_contract_emitted_) {
            calories_contract_emitted_ = true;
            oasis::uhai::FlowContract contract{};
            contract.contract_id = calories_contract_id_;
            contract.sender_node_id = 0x100C; // Ghost Node C DID handle
            contract.consumer_node_id = 0x0001;
            contract.start_logical_tick = 150;
            contract.end_logical_tick = 450;
            contract.rate_per_tick = 10.000; // 10 kcal per logical tick
            contract.resource_type = static_cast<uint16_t>(oasis::uhai::FlowResourceType::CALORIES);
            contract.status = static_cast<uint8_t>(oasis::uhai::FlowContractStatus::ACTIVE);
            contract.quality_flags = oasis::uhai::QualityFlags::QUALITY_SIMULATED; // Gate G6
            contract.reserved = 0;
            std::memcpy(contract.auth_signature, "GHOST100", 8);

            inbound_queue_.push(contract);
            spdlog::info("[GhostMeshTransport] [100x Ghost Node 0x100C] Emitted CALORIES contract ID: {} "
                         "([150..450) @ 10.000 kcal/tick)", contract.contract_id);
        }

        // Abrupt future contract termination trigger:
        // When current_tick_ reaches abort_tick_, publish an abrupt cancellation
        // of contract 101 (electricity), clamping its end_logical_tick to abort_tick_
        // and setting status = TERMINATED.
        if (abort_tick_ > 0 && current_tick_ >= abort_tick_ && !abort_emitted_) {
            abort_emitted_ = true;
            oasis::uhai::FlowContract contract{};
            contract.contract_id = electricity_contract_id_;
            contract.sender_node_id = 0x100A;
            contract.consumer_node_id = 0x0001;
            contract.start_logical_tick = 50;
            contract.end_logical_tick = abort_tick_; // Clamped delivery horizon
            contract.rate_per_tick = 0.050;
            contract.resource_type = static_cast<uint16_t>(oasis::uhai::FlowResourceType::ELECTRICITY);
            contract.status = static_cast<uint8_t>(oasis::uhai::FlowContractStatus::TERMINATED); // Revoked early
            contract.quality_flags = oasis::uhai::QualityFlags::QUALITY_SIMULATED;
            contract.reserved = 0;
            std::memcpy(contract.auth_signature, "GHOSTABR", 8);

            inbound_queue_.push(contract);
            spdlog::warn("[GhostMeshTransport] [ABRUPT TERMINATION EVENT] Ghost Agent 0x100A terminated Contract ID: {} "
                         "early at logical tick {} (status=TERMINATED)", contract.contract_id, abort_tick_);
        }
    }

    bool PollInbound(oasis::uhai::FlowContract& out_contract) override {
        if (!inbound_queue_.empty()) {
            out_contract = inbound_queue_.front();
            inbound_queue_.pop();
            return true;
        }
        return false;
    }

    bool SendOutbound(const oasis::uhai::FlowContract& in_contract) override {
        spdlog::info("[GhostMeshTransport] Outbound mesh packet transmitted: ID: {}, Recipient: 0x{:04x}, Status: {}",
                     in_contract.contract_id, in_contract.consumer_node_id, in_contract.status);
        return true;
    }

    [[nodiscard]] bool HasPendingPackets() const noexcept {
        return !inbound_queue_.empty();
    }

    /// Bridge an inward-relayed MigrationWaveArrivalEvent from l2_l3.sock (col-storaged)
    /// into /uhai_migration_waves with QUALITY_SIMULATED = 0x01 (SS-EP7-007)
    static bool RelayMigrationWaveToUhai(
        const oasis::uhai::MigrationWaveArrivalEvent& in_wave,
        oasis::uhai::UhaiMigrationWaveChannel& out_channel) {
        if (std::isnan(in_wave.legacy_distress_index) || in_wave.legacy_distress_index < 0.65f) {
            return false;
        }
        oasis::uhai::MigrationWaveArrivalEvent sanitized = in_wave;
        sanitized.cohort_size = std::clamp<uint16_t>(sanitized.cohort_size, 1u, 12u);
        sanitized.initial_trust_ring = 6; // Ring R6: Unverified Migrant
        return out_channel.Push(sanitized);
    }

private:
    uint32_t ghost_agents_{1};
    uint64_t abort_tick_{0};
    uint32_t frequency_hz_{10};
    uint64_t current_tick_{0};

    bool electricity_contract_emitted_{false};
    bool water_contract_emitted_{false};
    bool calories_contract_emitted_{false};
    bool abort_emitted_{false};

    uint64_t electricity_contract_id_{101};
    uint64_t water_contract_id_{102};
    uint64_t calories_contract_id_{103};

    std::queue<oasis::uhai::FlowContract> inbound_queue_;
};

/**
 * =============================================================================
 * EXTENSION POINT: libp2p Gossipsub Mesh Transport Adapter
 * =============================================================================
 * In future production sprints, Libp2pMeshTransport implements IMeshTransport:
 *
 * class Libp2pMeshTransport : public IMeshTransport {
 *     std::shared_ptr<libp2p::Host> host_;
 *     std::shared_ptr<libp2p::protocol::gossipsub::Gossipsub> gossipsub_;
 *     libp2p::protocol::gossipsub::TopicId flow_topic_{"/collective/mesh/1.0.0/flows"};
 *
 * public:
 *     void Initialize() override {
 *         // 1. Initialize libp2p Host with Secio / Noise crypto channel.
 *         // 2. Bind physical radio bearers (SX1262 LoRa, 802.11s Wi-Fi mesh).
 *         // 3. Subscribe to gossipsub topic "/collective/mesh/1.0.0/flows":
 *         //    gossipsub_->subscribe(flow_topic_, [this](const Message& msg) {
 *         //        oasis::uhai::FlowContract contract;
 *         //        if (DecodeFlowContract(msg.data, contract)) {
 *         //            inbound_queue_.push(contract);
 *         //        }
 *         //    });
 *     }
 *
 *     bool PollInbound(oasis::uhai::FlowContract& out_contract) override {
 *         // Pop deserialized FlowContract decoded from Gossipsub message payload
 *     }
 *
 *     bool SendOutbound(const oasis::uhai::FlowContract& in_contract) override {
 *         // Serialize FlowContract and call gossipsub_->publish(flow_topic_, data)
 *     }
 * };
 *
 * This architecture guarantees zero game engine modifications when upgrading
 * from simulated ghost transport to live cryptographic P2P mesh transport.
 * =============================================================================
 */

int main(int argc, char* argv[]) {
    std::string shm_name = oasis::uhai::DEFAULT_FLOW_SHM_NAME;
    std::string migration_shm_name = oasis::uhai::DEFAULT_MIGRATION_WAVE_SHM_NAME;
    uint64_t max_ticks = 0;
    int frequency_hz = 10;
    uint32_t ghost_agents = 1;
    uint64_t abort_tick = 0;

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
        } else if (arg == "--ghost-agents" && i + 1 < argc) {
            ghost_agents = static_cast<uint32_t>(std::stoul(argv[++i]));
            if (ghost_agents == 0) ghost_agents = 1;
            if (ghost_agents > 32) ghost_agents = 32;
        } else if (arg == "--abort-tick" && i + 1 < argc) {
            abort_tick = std::stoull(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: col-meshd [options]\n"
                      << "  --shm-name <name>      Shared memory object name (default: "
                      << oasis::uhai::DEFAULT_FLOW_SHM_NAME << ")\n"
                      << "  --migration-shm <name> Migration wave SHM channel (default: "
                      << oasis::uhai::DEFAULT_MIGRATION_WAVE_SHM_NAME << ")\n"
                      << "  --frequency <Hz>       Tick frequency in Hz (default: 10)\n"
                      << "  --ticks <N>            Exit cleanly after executing N ticks (0 = run indefinitely)\n"
                      << "  --max-ticks <N>        Alias for --ticks\n"
                      << "  --ghost-agents <N>     Number of 100x ghost agents to simulate (default: 1, max: 32)\n"
                      << "  --abort-tick <N>       Logical tick at which to publish abrupt contract termination (default: 0)\n"
                      << "  --help, -h             Display this help message\n";
            return 0;
        }
    }

    // Register POSIX signal handlers
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    spdlog::info("[col-meshd] Starting Sovereign Stack Layer 2/3 Mesh Daemon...");
    spdlog::info("[col-meshd] Target POSIX shm: '{}' | Migration shm: '{}' | Frequency: {} Hz | Ghost agents: {} | Abort tick: {}",
                 shm_name, migration_shm_name, frequency_hz, ghost_agents, abort_tick);

    oasis::uhai::UhaiMigrationWaveChannel migration_wave_channel(
        oasis::uhai::UhaiMigrationWaveChannel::ChannelMode::Producer, migration_shm_name);

    const size_t shm_size = oasis::uhai::TOTAL_FLOW_SHM_SIZE;
    int fd = -1;
    void* raw_ptr = nullptr;
    bool is_posix_shm = false;

    // 1. Attempt POSIX shared memory creation/opening
    fd = ::shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd >= 0) {
        struct stat sb{};
        if (::fstat(fd, &sb) == 0 && sb.st_size < static_cast<off_t>(shm_size)) {
            if (::ftruncate(fd, static_cast<off_t>(shm_size)) != 0) {
                spdlog::warn("[col-meshd] Failed to ftruncate shm to {} bytes: {}. Falling back to in-memory buffer.",
                             shm_size, std::strerror(errno));
                ::close(fd);
                fd = -1;
            }
        }
        if (fd >= 0) {
            raw_ptr = ::mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if (raw_ptr != MAP_FAILED) {
                is_posix_shm = true;
            } else {
                spdlog::warn("[col-meshd] Failed to mmap POSIX shm: {}. Falling back to in-memory buffer.",
                             std::strerror(errno));
                raw_ptr = nullptr;
                ::close(fd);
                fd = -1;
            }
        }
    } else {
        spdlog::warn("[col-meshd] Failed to shm_open '{}': {}. Falling back to in-memory buffer.",
                     shm_name, std::strerror(errno));
    }

    // 2. Aligned in-memory fallback via posix_memalign if POSIX shm is unavailable
    if (!raw_ptr) {
        spdlog::warn("[col-meshd] POSIX shm unavailable; activating in-memory UHAI fallback via posix_memalign.");
        void* aligned_ptr = nullptr;
        if (::posix_memalign(&aligned_ptr, oasis::uhai::CACHE_LINE_SIZE, shm_size) != 0 || aligned_ptr == nullptr) {
            spdlog::critical("[col-meshd] posix_memalign failed to allocate {} bytes: {}",
                             shm_size, std::strerror(errno));
            return 1;
        }
        std::memset(aligned_ptr, 0, shm_size);
        raw_ptr = aligned_ptr;
        is_posix_shm = false;
    }

    auto* header = reinterpret_cast<oasis::uhai::SharedRingHeader*>(raw_ptr);

    // Initialize default header layout if magic signature uninitialized
    if (header->magic_signature != oasis::uhai::UHAI_MAGIC) {
        spdlog::warn("[col-meshd] Header magic uninitialized (0x{:08x}). Initializing default layout...",
                     header->magic_signature);
        header->write_index.store(0, std::memory_order_relaxed);
        header->dropped_frames.store(0, std::memory_order_relaxed);
        header->read_index.store(0, std::memory_order_relaxed);
        header->capacity = oasis::uhai::RING_CAPACITY;
        header->element_size = sizeof(oasis::uhai::FlowContractSlot);
        header->magic_signature = oasis::uhai::UHAI_MAGIC;
        header->version = oasis::uhai::UHAI_VERSION;
    }

    // Assert header invariants
    if (header->magic_signature != oasis::uhai::UHAI_MAGIC) {
        spdlog::critical("[col-meshd] Invalid magic signature: 0x{:08x} (expected 0x{:08x})",
                         header->magic_signature, oasis::uhai::UHAI_MAGIC);
        if (is_posix_shm) {
            ::munmap(raw_ptr, shm_size);
            if (fd >= 0) ::close(fd);
        } else {
            std::free(raw_ptr);
        }
        return 1;
    }

    if (header->version != oasis::uhai::UHAI_VERSION) {
        spdlog::critical("[col-meshd] Unsupported UHAI version: {} (expected {})",
                         header->version, oasis::uhai::UHAI_VERSION);
        if (is_posix_shm) {
            ::munmap(raw_ptr, shm_size);
            if (fd >= 0) ::close(fd);
        } else {
            std::free(raw_ptr);
        }
        return 1;
    }

    spdlog::info("[col-meshd] Successfully attached to UHAI flow ring! ShmMode: {}, Capacity: {}, ElementSize: {}B",
                 is_posix_shm ? "POSIX_SHM" : "IN_MEMORY_FALLBACK", header->capacity, header->element_size);

    oasis::uhai::LockFreeSpscRing<oasis::uhai::FlowContractSlot, oasis::uhai::RING_CAPACITY> ring(raw_ptr);

    // Instantiate and initialize transport
    auto ghost_transport = std::make_unique<GhostMeshTransport>(ghost_agents, abort_tick, frequency_hz);
    ghost_transport->Initialize();

    using clock = std::chrono::steady_clock;
    const auto tick_interval = std::chrono::microseconds(1'000'000 / frequency_hz);
    auto next_tick_deadline = clock::now() + tick_interval;

    uint64_t daemon_tick = 0;
    uint64_t published_contracts = 0;

    spdlog::info("[col-meshd] Drift-free POSIX execution loop started (target: {} Hz).", frequency_hz);

    while (g_running.load(std::memory_order_relaxed)) {
        daemon_tick++;

        // Step ghost transport simulation
        ghost_transport->Step(daemon_tick);

        // Drain inbound packets from mesh transport and publish into UHAI ring buffer
        oasis::uhai::FlowContract contract{};
        while (ghost_transport->PollInbound(contract)) {
            oasis::uhai::FlowContractSlot slot{contract};
            if (ring.Push(slot)) {
                published_contracts++;
                const char* status_str = (contract.status == static_cast<uint8_t>(oasis::uhai::FlowContractStatus::ACTIVE)) ? "ACTIVE" :
                                         (contract.status == static_cast<uint8_t>(oasis::uhai::FlowContractStatus::TERMINATED)) ? "TERMINATED" : "COMPLETED";
                spdlog::info("[col-meshd] Published Mesh FlowContract: ID: {:>4} | Sender: 0x{:04x} | Resource: {} | "
                             "Rate: {:>6.3f}/tick | Window: [{:>4}..{:>4}) | Status: {} | Quality: 0x{:02x}",
                             contract.contract_id, contract.sender_node_id, contract.resource_type,
                             contract.rate_per_tick, contract.start_logical_tick, contract.end_logical_tick,
                             status_str, contract.quality_flags);
            } else {
                spdlog::warn("[col-meshd] Flow ring buffer full! Contract ID: {} dropped.", contract.contract_id);
            }
        }

        if ((max_ticks > 0 && max_ticks <= 50) || (daemon_tick % 20 == 0)) {
            spdlog::info("[col-meshd] Tick: {:>6} ({} Hz) | Published Contracts: {:>4} | MigrationSHM: {} | Dropped: {}",
                         daemon_tick, frequency_hz, published_contracts,
                         migration_wave_channel.IsValid() ? "READY" : "OFFLINE",
                         header->dropped_frames.load(std::memory_order_relaxed));
        }

        if (max_ticks > 0 && daemon_tick >= max_ticks) {
            spdlog::info("[col-meshd] Reached target tick count ({}). Initiating shutdown.", max_ticks);
            break;
        }

        // Drift-free scheduling via sleep_until
        std::this_thread::sleep_until(next_tick_deadline);
        next_tick_deadline += tick_interval;
        if (next_tick_deadline < clock::now()) {
            next_tick_deadline = clock::now() + tick_interval; // Drift guard
        }
    }

    spdlog::info("[col-meshd] Shutdown complete. Executed {} ticks, published {} flow contracts. Clean exit.",
                 daemon_tick, published_contracts);

    // Clean unmapping and file descriptor closure
    if (is_posix_shm) {
        ::munmap(raw_ptr, shm_size);
        if (fd >= 0) ::close(fd);
    } else {
        std::free(raw_ptr);
    }

    return 0;
}
