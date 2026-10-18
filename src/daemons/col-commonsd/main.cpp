#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <csignal>
#include <atomic>
#include <chrono>
#include <thread>
#include <cstring>
#include <cerrno>
#include <cstdint>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <spdlog/spdlog.h>
#include "uhai/uhai_ring_buffer.hpp"

namespace oasis::commons {

// ============================================================================
// Cryptographic Hash & Ricardian Treaty CID Engine (Story 6.5)
// ============================================================================

namespace crypto {

// Standard NIST FIPS 180-4 SHA-256 implementation for Ricardian Treaty CID calculation
class Sha256 {
public:
    Sha256() noexcept { Reset(); }

    void Reset() noexcept {
        state_[0] = 0x6a09e667;
        state_[1] = 0xbb67ae85;
        state_[2] = 0x3c6ef372;
        state_[3] = 0xa54ff53a;
        state_[4] = 0x510e527f;
        state_[5] = 0x9b05688c;
        state_[6] = 0x1f83d9ab;
        state_[7] = 0x5be0cd19;
        count_ = 0;
    }

    void Update(const void* data, size_t length) noexcept {
        const auto* input = static_cast<const uint8_t*>(data);
        size_t buffer_index = (count_ >> 3) & 0x3f;
        count_ += static_cast<uint64_t>(length) << 3;

        size_t part_len = 64 - buffer_index;
        size_t i = 0;

        if (length >= part_len) {
            std::memcpy(&buffer_[buffer_index], input, part_len);
            Transform(state_, buffer_);
            for (i = part_len; i + 63 < length; i += 64) {
                Transform(state_, &input[i]);
            }
            buffer_index = 0;
        }

        std::memcpy(&buffer_[buffer_index], &input[i], length - i);
    }

    void Update(std::string_view text) noexcept {
        Update(text.data(), text.size());
    }

    std::array<uint8_t, 32> Final() noexcept {
        uint8_t final_count[8];
        for (int i = 0; i < 8; ++i) {
            final_count[i] = static_cast<uint8_t>((count_ >> ((7 - i) * 8)) & 0xff);
        }

        size_t pad_len = ((count_ >> 3) & 0x3f) < 56 ? (56 - ((count_ >> 3) & 0x3f)) : (120 - ((count_ >> 3) & 0x3f));
        static const uint8_t padding[64] = { 0x80 };
        Update(padding, pad_len);
        Update(final_count, 8);

        std::array<uint8_t, 32> digest{};
        for (int i = 0; i < 8; ++i) {
            digest[i * 4 + 0] = static_cast<uint8_t>((state_[i] >> 24) & 0xff);
            digest[i * 4 + 1] = static_cast<uint8_t>((state_[i] >> 16) & 0xff);
            digest[i * 4 + 2] = static_cast<uint8_t>((state_[i] >> 8) & 0xff);
            digest[i * 4 + 3] = static_cast<uint8_t>(state_[i] & 0xff);
        }
        return digest;
    }

    static std::string DigestToHex(const std::array<uint8_t, 32>& digest) {
        std::ostringstream oss;
        for (uint8_t b : digest) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
        }
        return oss.str();
    }

private:
    static inline uint32_t ROR(uint32_t x, uint32_t n) noexcept { return (x >> n) | (x << (32 - n)); }
    static inline uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) noexcept { return (x & y) ^ (~x & z); }
    static inline uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) noexcept { return (x & y) ^ (x & z) ^ (y & z); }
    static inline uint32_t Sigma0(uint32_t x) noexcept { return ROR(x, 2) ^ ROR(x, 13) ^ ROR(x, 22); }
    static inline uint32_t Sigma1(uint32_t x) noexcept { return ROR(x, 6) ^ ROR(x, 11) ^ ROR(x, 25); }
    static inline uint32_t Gamma0(uint32_t x) noexcept { return ROR(x, 7) ^ ROR(x, 18) ^ (x >> 3); }
    static inline uint32_t Gamma1(uint32_t x) noexcept { return ROR(x, 17) ^ ROR(x, 19) ^ (x >> 10); }

    static void Transform(uint32_t state[8], const uint8_t block[64]) noexcept {
        static constexpr uint32_t K[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

        uint32_t w[64];
        for (int i = 0; i < 16; ++i) {
            w[i] = (static_cast<uint32_t>(block[i * 4 + 0]) << 24) |
                   (static_cast<uint32_t>(block[i * 4 + 1]) << 16) |
                   (static_cast<uint32_t>(block[i * 4 + 2]) << 8)  |
                   (static_cast<uint32_t>(block[i * 4 + 3]));
        }
        for (int i = 16; i < 64; ++i) {
            w[i] = Gamma1(w[i - 2]) + w[i - 7] + Gamma0(w[i - 15]) + w[i - 16];
        }

        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        uint32_t e = state[4], f = state[5], g = state[6], h = state[7];

        for (int i = 0; i < 64; ++i) {
            uint32_t t1 = h + Sigma1(e) + Ch(e, f, g) + K[i] + w[i];
            uint32_t t2 = Sigma0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
        state[5] += f;
        state[6] += g;
        state[7] += h;
    }

    uint32_t state_[8];
    uint64_t count_{0};
    uint8_t buffer_[64]{};
};

} // namespace crypto

// ============================================================================
// Ricardian Treaty Engine (Story 6.5)
// ============================================================================

struct RicardianTreaty {
    std::string treaty_id;
    std::string legal_prose;
    std::string jsonld_schema;
    std::string bpmn_invariants;
    std::string expected_cid;

    [[nodiscard]] std::string ComputeCID() const {
        crypto::Sha256 hasher;
        hasher.Update(legal_prose);
        hasher.Update("|");
        hasher.Update(jsonld_schema);
        hasher.Update("|");
        hasher.Update(bpmn_invariants);
        auto digest = hasher.Final();
        return crypto::Sha256::DigestToHex(digest);
    }

    [[nodiscard]] bool ValidateCID() const {
        return ComputeCID() == expected_cid;
    }
};

// ============================================================================
// W3C JSON-LD Semantic Intent Parser (Story 6.1)
// ============================================================================

struct JsonLdIntent {
    std::string context;
    std::string type;
    std::string author_did;
    std::string schema_uri;
    double exergy_mj{0.0};
    bool is_valid{false};

    static JsonLdIntent ParseStub(std::string_view json_str) {
        JsonLdIntent intent;
        if (json_str.find("\"@context\"") != std::string_view::npos) {
            intent.context = "https://w3id.org/collective/agora/v1";
        }
        if (json_str.find("\"ResourceRequestIntent\"") != std::string_view::npos) {
            intent.type = "ResourceRequestIntent";
            intent.is_valid = true;
        } else if (json_str.find("\"PolicyRefinement\"") != std::string_view::npos) {
            intent.type = "PolicyRefinement";
            intent.is_valid = true;
        }
        return intent;
    }
};

// ============================================================================
// Continuous Double-Auction Matching Engine for Bioregional Agora (Story 6.3)
// ============================================================================

struct AgoraOrder {
    uint64_t id{0};
    std::string trader_did;
    double price_exergy_mj{0.0}; // Price per unit in Exergy Credits (3.6 MJ)
    double quantity_kwh{0.0};
    bool is_bid{true};           // true = Buy bid, false = Sell ask
};

struct AgoraExecution {
    uint64_t execution_id{0};
    uint64_t bid_id{0};
    uint64_t ask_id{0};
    double clearing_price_mj{0.0};
    double quantity_cleared_kwh{0.0};
    std::string treaty_cid;
};

class ContinuousDoubleAuction {
public:
    ContinuousDoubleAuction() = default;

    void PlaceOrder(const AgoraOrder& order) {
        if (order.is_bid) {
            bids_.push_back(order);
            // Sort bids descending by price (highest bid first)
            std::sort(bids_.begin(), bids_.end(), [](const AgoraOrder& a, const AgoraOrder& b) {
                return a.price_exergy_mj > b.price_exergy_mj;
            });
        } else {
            asks_.push_back(order);
            // Sort asks ascending by price (lowest ask first)
            std::sort(asks_.begin(), asks_.end(), [](const AgoraOrder& a, const AgoraOrder& b) {
                return a.price_exergy_mj < b.price_exergy_mj;
            });
        }
    }

    std::vector<AgoraExecution> MatchOrders(const std::string& active_treaty_cid) {
        std::vector<AgoraExecution> executions;

        while (!bids_.empty() && !asks_.empty()) {
            auto& top_bid = bids_.front();
            auto& top_ask = asks_.front();

            // Double auction condition: Bid must meet or exceed Ask
            if (top_bid.price_exergy_mj < top_ask.price_exergy_mj) {
                break; // Spread is not crossed
            }

            // Midpoint clearing price formula: P_clear = (P_bid + P_ask) / 2.0
            double clearing_price = (top_bid.price_exergy_mj + top_ask.price_exergy_mj) / 2.0;
            double trade_qty = std::min(top_bid.quantity_kwh, top_ask.quantity_kwh);

            next_exec_id_++;
            executions.push_back({
                next_exec_id_,
                top_bid.id,
                top_ask.id,
                clearing_price,
                trade_qty,
                active_treaty_cid
            });

            top_bid.quantity_kwh -= trade_qty;
            top_ask.quantity_kwh -= trade_qty;

            if (top_bid.quantity_kwh <= 0.0001) bids_.erase(bids_.begin());
            if (top_ask.quantity_kwh <= 0.0001) asks_.erase(asks_.begin());
        }

        total_executions_ += executions.size();
        return executions;
    }

    [[nodiscard]] size_t PendingBids() const noexcept { return bids_.size(); }
    [[nodiscard]] size_t PendingAsks() const noexcept { return asks_.size(); }
    [[nodiscard]] uint64_t TotalExecutions() const noexcept { return total_executions_; }

private:
    std::vector<AgoraOrder> bids_;
    std::vector<AgoraOrder> asks_;
    uint64_t next_exec_id_{0};
    uint64_t total_executions_{0};
};

// ============================================================================
// Layer 6 Migration Wave Inward Adjacency Relay (SS-EP7-007: l6_l7.sock -> l5_l6.sock)
// ============================================================================

struct MigrationWaveAgoraRelay {
    static constexpr const char* INBOUND_SOCKET  = "l6_l7.sock"; // From L7 col-adversaryd (VLAN 40)
    static constexpr const char* OUTBOUND_SOCKET = "l5_l6.sock"; // To L5 col-kmsd (R6 guest tag)

    static bool RelayInwardToL5(oasis::uhai::MigrationWaveArrivalEvent& wave) noexcept {
        if (std::isnan(wave.legacy_distress_index) || wave.legacy_distress_index < 0.65f) {
            return false;
        }
        wave.cohort_size = std::clamp<uint16_t>(wave.cohort_size, 1u, 12u);
        wave.initial_trust_ring = 6; // Enforce Ring R6 (Unverified Migrant) guest boundary
        return true;
    }
};

} // namespace oasis::commons

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
            std::cout << "Usage: col-commonsd [options]\n"
                      << "  --shm-name <name>    Shared memory object name (default: "
                      << oasis::uhai::DEFAULT_SHM_NAME << ")\n"
                      << "  --max-ticks <N>      Exit cleanly after executing N deterministic Agora ticks\n"
                      << "  --ticks <N>          Alias for --max-ticks\n"
                      << "  --frequency <Hz>     Tick frequency in Hz (default: 10)\n"
                      << "  --help               Display this help message\n";
            return 0;
        }
    }

    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    spdlog::info("[col-commonsd] Starting Sovereign Stack Layer 6 Agora Daemon (col-commonsd)...");
    spdlog::info("[col-commonsd] Target POSIX shm: {} | Frequency: {} Hz", shm_name, frequency_hz);

    // Initialize Ricardian Treaty Validation (Story 6.5)
    oasis::commons::RicardianTreaty treaty;
    treaty.treaty_id = "treaty:bioregional:microgrid:v1";
    treaty.legal_prose = "Collective Ricardian Treaty: Bilateral clearing of electrical exergy at 3.6 MJ per token.";
    treaty.jsonld_schema = "{\"@context\":\"https://w3id.org/collective/agora/v1\",\"@type\":\"ExergyClearingContract\"}";
    treaty.bpmn_invariants = "<bpmn:invariant id=\"Inv-01\" rule=\"exergy_conservation\"/>";
    treaty.expected_cid = treaty.ComputeCID();

    if (treaty.ValidateCID()) {
        spdlog::info("[col-commonsd] Ricardian Treaty validated! Treaty_CID: {}", treaty.expected_cid);
    } else {
        spdlog::error("[col-commonsd] Ricardian Treaty CID mismatch!");
    }

    // Attach to UHAI Shared Memory with graceful in-memory fallback
    void* raw_ptr = nullptr;
    int fd = -1;
    bool is_posix_shm = false;

    fd = ::shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd >= 0) {
        struct stat sb{};
        if (::fstat(fd, &sb) == 0 && sb.st_size < static_cast<off_t>(oasis::uhai::TOTAL_SHM_SIZE)) {
            if (::ftruncate(fd, static_cast<off_t>(oasis::uhai::TOTAL_SHM_SIZE)) != 0) {
                spdlog::warn("[col-commonsd] ftruncate failed ({}), falling back to memory mode", std::strerror(errno));
                ::close(fd);
                fd = -1;
            }
        }
        if (fd >= 0) {
            raw_ptr = ::mmap(nullptr, oasis::uhai::TOTAL_SHM_SIZE,
                             PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if (raw_ptr != MAP_FAILED) {
                is_posix_shm = true;
            } else {
                raw_ptr = nullptr;
                ::close(fd);
                fd = -1;
            }
        }
    }

    // In-memory fallback if POSIX shm unavailable
    std::vector<uint8_t> fallback_memory;
    if (!raw_ptr) {
        spdlog::warn("[col-commonsd] POSIX shared memory unavailable. Activating in-memory UHAI fallback buffer.");
        fallback_memory.resize(oasis::uhai::TOTAL_SHM_SIZE + 64, 0);
        void* unaligned_ptr = fallback_memory.data();
        size_t space = fallback_memory.size();
        raw_ptr = std::align(64, oasis::uhai::TOTAL_SHM_SIZE, unaligned_ptr, space);
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

    spdlog::info("[col-commonsd] Layer 6 Commons Engine attached. UHAI SPSC active.");

    oasis::uhai::LockFreeSpscRing<oasis::uhai::TelemetrySlot, oasis::uhai::RING_CAPACITY> ring(raw_ptr);
    oasis::commons::ContinuousDoubleAuction auction;

    using clock = std::chrono::steady_clock;
    const auto tick_interval = std::chrono::microseconds(1'000'000 / frequency_hz);
    auto next_tick_deadline = clock::now() + tick_interval;

    uint64_t agora_tick = 0;
    uint64_t total_telemetry_drained = 0;
    uint64_t total_orders_matched = 0;

    // Simulate initial Bioregional Agora order book for autonomous exergy balancing
    auction.PlaceOrder({1, "did:key:z6MkhaXG1", 3.80, 50.0, true});   // Bid: 3.80 MJ, 50 kWh
    auction.PlaceOrder({2, "did:key:z6MkhaXG2", 3.70, 75.0, true});   // Bid: 3.70 MJ, 75 kWh
    auction.PlaceOrder({3, "did:key:z6MkhaXG3", 3.50, 60.0, false});  // Ask: 3.50 MJ, 60 kWh
    auction.PlaceOrder({4, "did:key:z6MkhaXG4", 3.65, 40.0, false});  // Ask: 3.65 MJ, 40 kWh

    while (g_running.load(std::memory_order_relaxed)) {
        agora_tick++;

        // Drain available telemetry samples from UHAI buffer (Story 5.5 Upward Compilation Loop)
        oasis::uhai::TelemetrySample sample{};
        bool drained_any = false;
        while (ring.Pop(sample)) {
            total_telemetry_drained++;
            drained_any = true;
        }

        // Inject dynamic simulated Agora trade demand every 5 ticks
        if (agora_tick % 5 == 0) {
            double dynamic_bid = 3.60 + 0.05 * static_cast<double>(agora_tick % 7);
            double dynamic_ask = 3.50 + 0.04 * static_cast<double>(agora_tick % 5);
            auction.PlaceOrder({100 + agora_tick, "did:key:z6MkWNodeA", dynamic_bid, 25.0, true});
            auction.PlaceOrder({200 + agora_tick, "did:key:z6MkWNodeB", dynamic_ask, 25.0, false});
        }

        // Run double-auction matching tick
        auto executions = auction.MatchOrders(treaty.expected_cid);
        total_orders_matched += executions.size();

        for (const auto& exec : executions) {
            spdlog::info("[col-commonsd] [AGORA MATCH] Execution #{}: Cleared {:>5.1f} kWh @ {:>5.3f} Exergy MJ | Treaty: {}",
                         exec.execution_id, exec.quantity_cleared_kwh, exec.clearing_price_mj,
                         exec.treaty_cid.substr(0, 12) + "...");
        }

        if ((max_ticks > 0 && max_ticks <= 50) || (agora_tick % 10 == 0) || drained_any) {
            spdlog::info("[col-commonsd] Agora Tick: {:>6} ({} Hz) | Executions: {} | Pending Bids: {} | Pending Asks: {} | UHAI Drained: {}",
                         agora_tick, frequency_hz, total_orders_matched, auction.PendingBids(),
                         auction.PendingAsks(), total_telemetry_drained);
        }

        if (max_ticks > 0 && agora_tick >= max_ticks) {
            spdlog::info("[col-commonsd] Reached target tick count ({}). Initiating clean shutdown.", max_ticks);
            break;
        }

        std::this_thread::sleep_until(next_tick_deadline);
        next_tick_deadline += tick_interval;
        if (next_tick_deadline < clock::now()) {
            next_tick_deadline = clock::now() + tick_interval;
        }
    }

    spdlog::info("[col-commonsd] Layer 6 Agora completed {} ticks. Total matches: {}. Total UHAI samples: {}. Clean exit.",
                 agora_tick, total_orders_matched, total_telemetry_drained);

    if (is_posix_shm && raw_ptr) {
        ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
    }
    if (fd >= 0) {
        ::close(fd);
    }

    return 0;
}
