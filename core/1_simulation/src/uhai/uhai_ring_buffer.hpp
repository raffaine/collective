#pragma once

#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#ifndef __EMSCRIPTEN__
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace oasis::uhai {

// Telemetry Sample Quality Flags (Story 7.4 Hardware-in-the-Loop Isolation)
namespace QualityFlags {
    constexpr uint8_t QUALITY_SIMULATED     = 0x01; // Emitted by external SITL (e.g. Oasis)
    constexpr uint8_t QUALITY_HARDWARE_ROOT = 0x02; // Hardware secure element (ATECC608A) verified
    constexpr uint8_t QUALITY_OUT_OF_BOUNDS = 0x04; // Physical transducer limits breached
    constexpr uint8_t QUALITY_SPOOF_SUSPECT = 0x08; // Flagged by spatial/thermodynamic cross-check
}
constexpr uint8_t QUALITY_SIMULATED     = QualityFlags::QUALITY_SIMULATED;
constexpr uint8_t QUALITY_HARDWARE_ROOT = QualityFlags::QUALITY_HARDWARE_ROOT;

constexpr size_t CACHE_LINE_SIZE = 64;
constexpr size_t RING_CAPACITY = 1024; // Power of 2
constexpr uint32_t UHAI_MAGIC = 0x55484149; // "UHAI" (0x55484149)
constexpr uint32_t UHAI_VERSION = 1;
constexpr const char* DEFAULT_SHM_NAME = "/uhai_telemetry";

#pragma pack(push, 8)
struct alignas(8) TelemetrySample {
    uint64_t frame_count{0};        // Monotonic engine simulation/render frame count (8 bytes)
    uint64_t timestamp_ns{0};       // Nanosecond timestamp (HLC or steady_clock) (8 bytes)
    float    fps{0.0f};             // Current frame rate (4 bytes)
    float    founder_stress{0.0f};  // Founder psychological stress accumulator (4 bytes)
    uint32_t memory_slot_count{0};  // Founder episodic memory ring buffer count (4 bytes)
    uint8_t  biome_id{0};           // Active legacy biome: 0=Suburban, 1=Urban, 2=Permaculture (1 byte)
    union {
        uint8_t quality_flags{0};   // Bitfield flags (legacy/plural name)
        uint8_t quality_flag;       // Singular name (Story 7.4)
    };
    uint16_t reserved{0};           // Struct padding for strict 8-byte alignment (2 bytes)
};
#pragma pack(pop)
static_assert(sizeof(TelemetrySample) == 32, "TelemetrySample must be exactly 32 bytes.");

// Story 5.5: Upward Compilation Loop Intent Data Structures (Anti-Bleed Invariant 4)
enum class IntentType : uint16_t {
    UNKNOWN             = 0,
    SPATIAL_AFFORDANCE  = 1, // Cadastral blueprint placement / chore affordance
    POLICY_REFINEMENT   = 2, // In silico verified policy export (Story 5.5)
    WORK_TOKEN_DISPATCH = 3, // Layer 4 BPMN VM work token
    RESOURCE_REBALANCE  = 4  // Microgrid/cistern dispatch rebalance
};

#pragma pack(push, 8)
struct alignas(8) SpatialIntentPayload {
    uint8_t  intent_cid[32]{};     // SHA256 / CID digest of JSON-LD schema or treaty specification (32 bytes)
    uint8_t  author_did[32]{};     // Opaque 32-byte founder DID handle (Anti-Bleed Inv 2) (32 bytes)
    uint64_t timestamp_ns{0};      // Nanosecond epoch timestamp (8 bytes)
    uint16_t intent_type{0};       // IntentType enum (2 bytes)
    uint8_t  quality_flag{0x01};   // QUALITY_SIMULATED (0x01) or QUALITY_HARDWARE_ROOT (0x02) (1 byte)
    uint8_t  status{0};            // Lifecycle status: 0=Draft, 1=Validated, 2=Compiled (1 byte)
    uint32_t payload_bytes{0};     // Byte length of inline JSON-LD payload (4 bytes)
    float    spatial_origin[3]{};  // 3D Voxel coordinates (x, y, z) for Cadastral anchor (12 bytes)
    float    spatial_extents[3]{}; // Voxel bounding box extents (dx, dy, dz) (12 bytes)
    char     schema_uri[32]{};     // JSON-LD schema or target URI (32 bytes)
    char     payload_jsonld[128]{};// Compact canonical JSON-LD intent payload (128 bytes)
};
#pragma pack(pop)
static_assert(sizeof(SpatialIntentPayload) == 264, "SpatialIntentPayload must be exactly 264 bytes.");

struct alignas(CACHE_LINE_SIZE) TelemetrySlot {
    TelemetrySample sample;
    uint8_t pad[32]; // Pads 32 bytes to 64 bytes (1 CPU cache line)
};
static_assert(sizeof(TelemetrySlot) == 64, "TelemetrySlot must be exactly 64 bytes.");

// Dual Sprint 9: Temporal Flow Contracts (Story 2.4, Anti-Bleed Invariant 3)
enum class FlowResourceType : uint16_t {
    UNKNOWN        = 0,
    ELECTRICITY    = 1, // kWh per tick (accrues to solar_stored_kwh)
    WATER          = 2, // Liters per tick (accrues to rain_stored_liters)
    CALORIES       = 3, // kcal per tick (accrues to somatic caloric_reserve)
    DATA_BANDWIDTH = 4  // Routing exergy
};

enum class FlowContractStatus : uint8_t {
    ACTIVE     = 0, // Contract is active / fulfilling
    TERMINATED = 1, // Revoked / cancelled early by sender
    COMPLETED  = 2  // Completed at end_logical_tick
};

#pragma pack(push, 8)
struct alignas(8) FlowContract {
    uint64_t contract_id{0};          // 8 bytes
    uint64_t sender_node_id{0};       // 8 bytes
    uint64_t consumer_node_id{0};     // 8 bytes
    uint64_t start_logical_tick{0};   // 8 bytes
    uint64_t end_logical_tick{0};     // 8 bytes
    double   rate_per_tick{0.0};      // 8 bytes
    uint16_t resource_type{0};        // 2 bytes
    uint8_t  status{0};               // 1 byte
    uint8_t  quality_flags{0x01};     // 1 byte (0x01 = QUALITY_SIMULATED)
    uint32_t reserved{0};             // 4 bytes
    uint8_t  auth_signature[8]{0};    // 8 bytes
};
#pragma pack(pop)
static_assert(sizeof(FlowContract) == 64, "FlowContract must be exactly 64 bytes.");

struct alignas(CACHE_LINE_SIZE) FlowContractSlot {
    FlowContract contract;
};
static_assert(sizeof(FlowContractSlot) == 64, "FlowContractSlot must be exactly 64 bytes.");

struct alignas(CACHE_LINE_SIZE) SharedRingHeader {
    // Cache Line 0 (offset 0..63): Producer write state & overrun accounting
    alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> write_index{0};
    std::atomic<uint64_t> dropped_frames{0}; // Atomic counter for overrun telemetry drops
    uint8_t pad0[48];

    // Cache Line 1 (offset 64..127): Consumer read state
    alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> read_index{0};
    uint8_t pad1[56];

    // Cache Line 2 (offset 128..191): Static read-only configuration
    alignas(CACHE_LINE_SIZE) uint32_t capacity{RING_CAPACITY};
    uint32_t element_size{sizeof(TelemetrySlot)};
    uint32_t magic_signature{UHAI_MAGIC}; // "UHAI"
    uint32_t version{UHAI_VERSION};
    uint8_t pad2[48];
};
static_assert(sizeof(SharedRingHeader) == 192, "SharedRingHeader must occupy exactly 3 cache lines (192 bytes).");

constexpr size_t TOTAL_SHM_SIZE = sizeof(SharedRingHeader) + RING_CAPACITY * sizeof(TelemetrySlot);
static_assert(TOTAL_SHM_SIZE == 65728, "Total UHAI shared memory size must be 65728 bytes.");

constexpr const char* DEFAULT_FLOW_SHM_NAME = "/uhai_flow";
constexpr size_t TOTAL_FLOW_SHM_SIZE = sizeof(SharedRingHeader) + RING_CAPACITY * sizeof(FlowContractSlot);
static_assert(TOTAL_FLOW_SHM_SIZE == 65728, "Total UHAI flow SHM size must be 65728 bytes.");

template <typename SlotT = TelemetrySlot, size_t Capacity = RING_CAPACITY>
class LockFreeSpscRing {
    static_assert((Capacity > 0) && ((Capacity & (Capacity - 1)) == 0),
                  "Capacity must be a power of 2 for bitwise masking");
public:
    explicit LockFreeSpscRing(void* raw_shm_ptr)
        : header_(reinterpret_cast<SharedRingHeader*>(raw_shm_ptr)),
          buffer_(reinterpret_cast<SlotT*>(static_cast<char*>(raw_shm_ptr) + sizeof(SharedRingHeader))) {}

    // Disable copy and assignment semantics
    LockFreeSpscRing(const LockFreeSpscRing&) = delete;
    LockFreeSpscRing& operator=(const LockFreeSpscRing&) = delete;
    LockFreeSpscRing(LockFreeSpscRing&&) = default;
    LockFreeSpscRing& operator=(LockFreeSpscRing&&) = default;

    template <typename ItemT>
    bool Push(const ItemT& item) {
        const uint64_t w = header_->write_index.load(std::memory_order_relaxed);
        const uint64_t r = header_->read_index.load(std::memory_order_acquire);
        if ((w - r) >= Capacity) {
            header_->dropped_frames.fetch_add(1, std::memory_order_relaxed);
            return false; // Buffer full; overrun recorded atomically
        }

        if constexpr (requires { buffer_[0].sample = item; }) {
            buffer_[w & (Capacity - 1)].sample = item;
        } else if constexpr (requires { buffer_[0].contract = item; }) {
            buffer_[w & (Capacity - 1)].contract = item;
        } else {
            buffer_[w & (Capacity - 1)] = item;
        }

        header_->write_index.store(w + 1, std::memory_order_release);
        return true;
    }

    template <typename ItemT>
    bool push(const ItemT& item) {
        return Push(item);
    }

    template <typename ItemT>
    bool Pop(ItemT& item) {
        const uint64_t r = header_->read_index.load(std::memory_order_relaxed);
        const uint64_t w = header_->write_index.load(std::memory_order_acquire);
        if (r == w) {
            return false; // Buffer empty; underrun handled gracefully
        }

        if constexpr (requires { item = buffer_[0].sample; }) {
            item = buffer_[r & (Capacity - 1)].sample;
        } else if constexpr (requires { item = buffer_[0].contract; }) {
            item = buffer_[r & (Capacity - 1)].contract;
        } else {
            item = buffer_[r & (Capacity - 1)];
        }

        header_->read_index.store(r + 1, std::memory_order_release);
        return true;
    }

    template <typename ItemT>
    bool pop(ItemT& item) {
        return Pop(item);
    }

    template <typename T = SlotT>
    requires requires(T s) { s.sample; }
    std::optional<TelemetrySample> Pop() {
        TelemetrySample s;
        if (Pop(s)) {
            return s;
        }
        return std::nullopt;
    }

    template <typename T = SlotT>
    requires requires(T s) { s.sample; }
    std::optional<TelemetrySample> pop() {
        return Pop();
    }

    template <typename T = SlotT>
    requires requires(T s) { s.contract; }
    std::optional<FlowContract> Pop() {
        FlowContract c;
        if (Pop(c)) {
            return c;
        }
        return std::nullopt;
    }

    template <typename T = SlotT>
    requires requires(T s) { s.contract; }
    std::optional<FlowContract> pop() {
        return Pop();
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return header_->read_index.load(std::memory_order_relaxed) ==
               header_->write_index.load(std::memory_order_relaxed);
    }

    [[nodiscard]] uint64_t Size() const noexcept {
        const uint64_t w = header_->write_index.load(std::memory_order_relaxed);
        const uint64_t r = header_->read_index.load(std::memory_order_relaxed);
        return (w >= r) ? (w - r) : 0;
    }

    [[nodiscard]] uint64_t DroppedCount() const noexcept {
        return header_->dropped_frames.load(std::memory_order_relaxed);
    }

    [[nodiscard]] SharedRingHeader* GetHeader() noexcept { return header_; }
    [[nodiscard]] const SharedRingHeader* GetHeader() const noexcept { return header_; }
    [[nodiscard]] SlotT* GetBuffer() noexcept { return buffer_; }
    [[nodiscard]] const SlotT* GetBuffer() const noexcept { return buffer_; }

private:
    SharedRingHeader* header_{nullptr};
    SlotT* buffer_{nullptr};
};

class UhaiTelemetryChannel {
public:
    enum class ChannelMode {
        Producer,
        Consumer,
        VirtualInMemory
    };

    explicit UhaiTelemetryChannel(ChannelMode mode = ChannelMode::Producer,
                                  const std::string& shm_name = DEFAULT_SHM_NAME)
        : mode_(mode), shm_name_(shm_name) {
        Initialize();
    }

    ~UhaiTelemetryChannel() {
        Close();
    }

    UhaiTelemetryChannel(const UhaiTelemetryChannel&) = delete;
    UhaiTelemetryChannel& operator=(const UhaiTelemetryChannel&) = delete;

    UhaiTelemetryChannel(UhaiTelemetryChannel&& other) noexcept
        : mode_(other.mode_),
          shm_name_(std::move(other.shm_name_)),
          raw_ptr_(other.raw_ptr_),
          shm_size_(other.shm_size_),
          fd_(other.fd_),
          is_posix_shm_(other.is_posix_shm_),
          ring_(std::move(other.ring_)) {
        other.raw_ptr_ = nullptr;
        other.shm_size_ = 0;
        other.fd_ = -1;
        other.is_posix_shm_ = false;
    }

    UhaiTelemetryChannel& operator=(UhaiTelemetryChannel&& other) noexcept {
        if (this != &other) {
            Close();
            mode_ = other.mode_;
            shm_name_ = std::move(other.shm_name_);
            raw_ptr_ = other.raw_ptr_;
            shm_size_ = other.shm_size_;
            fd_ = other.fd_;
            is_posix_shm_ = other.is_posix_shm_;
            ring_ = std::move(other.ring_);

            other.raw_ptr_ = nullptr;
            other.shm_size_ = 0;
            other.fd_ = -1;
            other.is_posix_shm_ = false;
        }
        return *this;
    }

    [[nodiscard]] bool IsValid() const noexcept {
        return raw_ptr_ != nullptr && ring_ != nullptr;
    }

    bool Push(const TelemetrySample& sample) {
        if (!IsValid()) return false;
        return ring_->Push(sample);
    }

    bool Pop(TelemetrySample& sample) {
        if (!IsValid()) return false;
        return ring_->Pop(sample);
    }

    [[nodiscard]] SharedRingHeader* GetHeader() noexcept {
        return ring_ ? ring_->GetHeader() : nullptr;
    }

    [[nodiscard]] const SharedRingHeader* GetHeader() const noexcept {
        return ring_ ? ring_->GetHeader() : nullptr;
    }

    [[nodiscard]] LockFreeSpscRing<TelemetrySlot, RING_CAPACITY>* GetRing() noexcept {
        return ring_.get();
    }

    void Close() {
#ifndef __EMSCRIPTEN__
        if (is_posix_shm_ && raw_ptr_) {
            ::munmap(raw_ptr_, shm_size_);
            raw_ptr_ = nullptr;
        }
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
#endif
        if (!is_posix_shm_ && raw_ptr_) {
            std::free(raw_ptr_);
            raw_ptr_ = nullptr;
        }
        ring_.reset();
        shm_size_ = 0;
        is_posix_shm_ = false;
    }

    static bool Unlink(const std::string& shm_name = DEFAULT_SHM_NAME) {
#ifndef __EMSCRIPTEN__
        return (::shm_unlink(shm_name.c_str()) == 0);
#else
        (void)shm_name;
        return true;
#endif
    }

private:
    void Initialize() {
        shm_size_ = TOTAL_SHM_SIZE;

#ifndef __EMSCRIPTEN__
        if (mode_ != ChannelMode::VirtualInMemory) {
            int flags = (mode_ == ChannelMode::Producer) ? (O_CREAT | O_RDWR) : (O_CREAT | O_RDWR);
            fd_ = ::shm_open(shm_name_.c_str(), flags, 0666);
            if (fd_ >= 0) {
                // Ensure size on newly created shm
                struct stat sb;
                if (::fstat(fd_, &sb) == 0 && sb.st_size < static_cast<off_t>(shm_size_)) {
                    if (::ftruncate(fd_, static_cast<off_t>(shm_size_)) != 0) {
                        ::close(fd_);
                        fd_ = -1;
                    }
                }
            }

            if (fd_ >= 0) {
                void* ptr = ::mmap(nullptr, shm_size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
                if (ptr != MAP_FAILED) {
                    raw_ptr_ = ptr;
                    is_posix_shm_ = true;

                    auto* hdr = reinterpret_cast<SharedRingHeader*>(raw_ptr_);
                    if (mode_ == ChannelMode::Producer) {
                        if (hdr->magic_signature != UHAI_MAGIC || hdr->version != UHAI_VERSION) {
                            hdr->write_index.store(0, std::memory_order_relaxed);
                            hdr->dropped_frames.store(0, std::memory_order_relaxed);
                            hdr->read_index.store(0, std::memory_order_relaxed);
                            hdr->capacity = RING_CAPACITY;
                            hdr->element_size = sizeof(TelemetrySlot);
                            hdr->magic_signature = UHAI_MAGIC;
                            hdr->version = UHAI_VERSION;
                        }
                    }
                    ring_ = std::make_unique<LockFreeSpscRing<TelemetrySlot, RING_CAPACITY>>(raw_ptr_);
                    return;
                } else {
                    ::close(fd_);
                    fd_ = -1;
                }
            }
        }
#endif

        // In-memory fallback (WASM or POSIX failure fallback)
        void* ptr = nullptr;
        if (posix_memalign(&ptr, CACHE_LINE_SIZE, shm_size_) == 0 && ptr != nullptr) {
            std::memset(ptr, 0, shm_size_);
            raw_ptr_ = ptr;
            is_posix_shm_ = false;

            auto* hdr = reinterpret_cast<SharedRingHeader*>(raw_ptr_);
            hdr->write_index.store(0, std::memory_order_relaxed);
            hdr->dropped_frames.store(0, std::memory_order_relaxed);
            hdr->read_index.store(0, std::memory_order_relaxed);
            hdr->capacity = RING_CAPACITY;
            hdr->element_size = sizeof(TelemetrySlot);
            hdr->magic_signature = UHAI_MAGIC;
            hdr->version = UHAI_VERSION;

            ring_ = std::make_unique<LockFreeSpscRing<TelemetrySlot, RING_CAPACITY>>(raw_ptr_);
        }
    }

    ChannelMode mode_{ChannelMode::Producer};
    std::string shm_name_{DEFAULT_SHM_NAME};
    void* raw_ptr_{nullptr};
    size_t shm_size_{0};
    int fd_{-1};
    bool is_posix_shm_{false};
    std::unique_ptr<LockFreeSpscRing<TelemetrySlot, RING_CAPACITY>> ring_{nullptr};
};

template <typename SlotT = FlowContractSlot, size_t Capacity = RING_CAPACITY>
class UhaiChannel {
public:
    enum class ChannelMode {
        Producer,
        Consumer,
        VirtualInMemory
    };

    static const char* DefaultShmName() {
        if constexpr (std::is_same_v<SlotT, TelemetrySlot>) {
            return DEFAULT_SHM_NAME;
        } else {
            return DEFAULT_FLOW_SHM_NAME;
        }
    }

    explicit UhaiChannel(ChannelMode mode = ChannelMode::Consumer)
        : UhaiChannel(mode, DefaultShmName()) {}

    UhaiChannel(ChannelMode mode, const std::string& shm_name)
        : mode_(mode), shm_name_(shm_name) {
        Initialize();
    }

    ~UhaiChannel() {
        Close();
    }

    UhaiChannel(const UhaiChannel&) = delete;
    UhaiChannel& operator=(const UhaiChannel&) = delete;

    UhaiChannel(UhaiChannel&& other) noexcept
        : mode_(other.mode_),
          shm_name_(std::move(other.shm_name_)),
          raw_ptr_(other.raw_ptr_),
          shm_size_(other.shm_size_),
          fd_(other.fd_),
          is_posix_shm_(other.is_posix_shm_),
          ring_(std::move(other.ring_)) {
        other.raw_ptr_ = nullptr;
        other.shm_size_ = 0;
        other.fd_ = -1;
        other.is_posix_shm_ = false;
    }

    UhaiChannel& operator=(UhaiChannel&& other) noexcept {
        if (this != &other) {
            Close();
            mode_ = other.mode_;
            shm_name_ = std::move(other.shm_name_);
            raw_ptr_ = other.raw_ptr_;
            shm_size_ = other.shm_size_;
            fd_ = other.fd_;
            is_posix_shm_ = other.is_posix_shm_;
            ring_ = std::move(other.ring_);

            other.raw_ptr_ = nullptr;
            other.shm_size_ = 0;
            other.fd_ = -1;
            other.is_posix_shm_ = false;
        }
        return *this;
    }

    [[nodiscard]] bool IsValid() const noexcept {
        return raw_ptr_ != nullptr && ring_ != nullptr;
    }

    template <typename ItemT>
    bool Push(const ItemT& item) {
        if (!IsValid()) return false;
        return ring_->Push(item);
    }

    template <typename ItemT>
    bool push(const ItemT& item) {
        return Push(item);
    }

    template <typename ItemT>
    bool Pop(ItemT& item) {
        if (!IsValid()) return false;
        return ring_->Pop(item);
    }

    template <typename ItemT>
    bool pop(ItemT& item) {
        return Pop(item);
    }

    template <typename T = SlotT>
    requires requires(T s) { s.contract; }
    std::optional<FlowContract> Pop() {
        if (!IsValid()) return std::nullopt;
        return ring_->Pop();
    }

    template <typename T = SlotT>
    requires requires(T s) { s.contract; }
    std::optional<FlowContract> pop() {
        return Pop();
    }

    template <typename T = SlotT>
    requires requires(T s) { s.sample; }
    std::optional<TelemetrySample> Pop() {
        if (!IsValid()) return std::nullopt;
        return ring_->Pop();
    }

    template <typename T = SlotT>
    requires requires(T s) { s.sample; }
    std::optional<TelemetrySample> pop() {
        return Pop();
    }

    [[nodiscard]] SharedRingHeader* GetHeader() noexcept {
        return ring_ ? ring_->GetHeader() : nullptr;
    }

    [[nodiscard]] const SharedRingHeader* GetHeader() const noexcept {
        return ring_ ? ring_->GetHeader() : nullptr;
    }

    [[nodiscard]] LockFreeSpscRing<SlotT, Capacity>* GetRing() noexcept {
        return ring_.get();
    }

    void Close() {
#ifndef __EMSCRIPTEN__
        if (is_posix_shm_ && raw_ptr_) {
            ::munmap(raw_ptr_, shm_size_);
            raw_ptr_ = nullptr;
        }
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
#endif
        if (!is_posix_shm_ && raw_ptr_) {
            std::free(raw_ptr_);
            raw_ptr_ = nullptr;
        }
        ring_.reset();
        shm_size_ = 0;
        is_posix_shm_ = false;
    }

    static bool Unlink(const std::string& shm_name = DefaultShmName()) {
#ifndef __EMSCRIPTEN__
        return (::shm_unlink(shm_name.c_str()) == 0);
#else
        (void)shm_name;
        return true;
#endif
    }

private:
    void Initialize() {
        shm_size_ = sizeof(SharedRingHeader) + Capacity * sizeof(SlotT);

#ifndef __EMSCRIPTEN__
        if (mode_ != ChannelMode::VirtualInMemory) {
            int flags = (mode_ == ChannelMode::Producer) ? (O_CREAT | O_RDWR) : (O_CREAT | O_RDWR);
            fd_ = ::shm_open(shm_name_.c_str(), flags, 0666);
            if (fd_ >= 0) {
                struct stat sb;
                if (::fstat(fd_, &sb) == 0 && sb.st_size < static_cast<off_t>(shm_size_)) {
                    if (::ftruncate(fd_, static_cast<off_t>(shm_size_)) != 0) {
                        ::close(fd_);
                        fd_ = -1;
                    }
                }
            }

            if (fd_ >= 0) {
                void* ptr = ::mmap(nullptr, shm_size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
                if (ptr != MAP_FAILED) {
                    raw_ptr_ = ptr;
                    is_posix_shm_ = true;

                    auto* hdr = reinterpret_cast<SharedRingHeader*>(raw_ptr_);
                    if (hdr->magic_signature != UHAI_MAGIC || hdr->version != UHAI_VERSION) {
                        hdr->write_index.store(0, std::memory_order_relaxed);
                        hdr->dropped_frames.store(0, std::memory_order_relaxed);
                        hdr->read_index.store(0, std::memory_order_relaxed);
                        hdr->capacity = static_cast<uint32_t>(Capacity);
                        hdr->element_size = static_cast<uint32_t>(sizeof(SlotT));
                        hdr->magic_signature = UHAI_MAGIC;
                        hdr->version = UHAI_VERSION;
                    }
                    ring_ = std::make_unique<LockFreeSpscRing<SlotT, Capacity>>(raw_ptr_);
                    return;
                } else {
                    ::close(fd_);
                    fd_ = -1;
                }
            }
        }
#endif

        // In-memory fallback (WASM or POSIX failure fallback)
        void* ptr = nullptr;
        if (posix_memalign(&ptr, CACHE_LINE_SIZE, shm_size_) == 0 && ptr != nullptr) {
            std::memset(ptr, 0, shm_size_);
            raw_ptr_ = ptr;
            is_posix_shm_ = false;

            auto* hdr = reinterpret_cast<SharedRingHeader*>(raw_ptr_);
            hdr->write_index.store(0, std::memory_order_relaxed);
            hdr->dropped_frames.store(0, std::memory_order_relaxed);
            hdr->read_index.store(0, std::memory_order_relaxed);
            hdr->capacity = static_cast<uint32_t>(Capacity);
            hdr->element_size = static_cast<uint32_t>(sizeof(SlotT));
            hdr->magic_signature = UHAI_MAGIC;
            hdr->version = UHAI_VERSION;

            ring_ = std::make_unique<LockFreeSpscRing<SlotT, Capacity>>(raw_ptr_);
        }
    }

    ChannelMode mode_{ChannelMode::Producer};
    std::string shm_name_{DefaultShmName()};
    void* raw_ptr_{nullptr};
    size_t shm_size_{0};
    int fd_{-1};
    bool is_posix_shm_{false};
    std::unique_ptr<LockFreeSpscRing<SlotT, Capacity>> ring_{nullptr};
};

using UhaiFlowChannel = UhaiChannel<FlowContractSlot, RING_CAPACITY>;

} // namespace oasis::uhai

namespace oasis::flow {

struct TemporalFlowContractComponent {
    static constexpr size_t MAX_CONTRACTS = 64;
    oasis::uhai::FlowContract contracts[MAX_CONTRACTS]{};
    size_t active_count{0};
    double total_electricity_received{0.0};
    double total_water_received{0.0};
    double total_calories_received{0.0};

    bool AddOrUpdateContract(const oasis::uhai::FlowContract& incoming, uint64_t current_tick = 0) {
        if (std::isnan(incoming.rate_per_tick) || std::isinf(incoming.rate_per_tick) || incoming.rate_per_tick < 0.0) {
            return false;
        }

        for (size_t i = 0; i < active_count; ++i) {
            if (contracts[i].contract_id == incoming.contract_id) {
                // Prevent out-of-order resurrection of terminated contract
                if (contracts[i].status == static_cast<uint8_t>(oasis::uhai::FlowContractStatus::TERMINATED)) {
                    if (incoming.status != static_cast<uint8_t>(oasis::uhai::FlowContractStatus::TERMINATED)) {
                        return false; // Reject resurrection to ACTIVE
                    }
                    if (incoming.end_logical_tick > 0 && incoming.end_logical_tick < contracts[i].end_logical_tick) {
                        contracts[i].end_logical_tick = incoming.end_logical_tick;
                    } else if (incoming.end_logical_tick == 0 || incoming.end_logical_tick <= current_tick) {
                        contracts[i].end_logical_tick = current_tick;
                    }
                    return true;
                }

                if (incoming.status == static_cast<uint8_t>(oasis::uhai::FlowContractStatus::TERMINATED)) {
                    contracts[i].status = incoming.status;
                    if (incoming.end_logical_tick > 0 && incoming.end_logical_tick < contracts[i].end_logical_tick) {
                        contracts[i].end_logical_tick = incoming.end_logical_tick;
                    } else {
                        // Clamp end tick immediately to current_tick
                        contracts[i].end_logical_tick = current_tick;
                    }
                } else {
                    contracts[i] = incoming;
                }
                return true;
            }
        }
        if (active_count < MAX_CONTRACTS) {
            if (incoming.status == static_cast<uint8_t>(oasis::uhai::FlowContractStatus::TERMINATED)) {
                if (incoming.end_logical_tick > current_tick) {
                    contracts[active_count++] = incoming;
                    return true;
                }
                return false;
            } else if (incoming.status != static_cast<uint8_t>(oasis::uhai::FlowContractStatus::COMPLETED)) {
                contracts[active_count++] = incoming;
                return true;
            }
        }
        return false;
    }

    void StepTick(uint64_t current_tick, double& out_solar_kwh, double& out_rain_liters, double& out_calories) {
        for (size_t i = 0; i < active_count; ) {
            auto& c = contracts[i];
            if (current_tick >= c.end_logical_tick ||
                (c.status == static_cast<uint8_t>(oasis::uhai::FlowContractStatus::TERMINATED) && current_tick >= c.end_logical_tick)) {
                // Expired or reached termination tick: retire without leak
                contracts[i] = contracts[--active_count];
                continue;
            }

            if (current_tick >= c.start_logical_tick && current_tick < c.end_logical_tick) {
                if (c.resource_type == static_cast<uint16_t>(oasis::uhai::FlowResourceType::ELECTRICITY)) {
                    out_solar_kwh += c.rate_per_tick;
                    total_electricity_received += c.rate_per_tick;
                } else if (c.resource_type == static_cast<uint16_t>(oasis::uhai::FlowResourceType::WATER)) {
                    out_rain_liters += c.rate_per_tick;
                    total_water_received += c.rate_per_tick;
                } else if (c.resource_type == static_cast<uint16_t>(oasis::uhai::FlowResourceType::CALORIES)) {
                    out_calories += c.rate_per_tick;
                    total_calories_received += c.rate_per_tick;
                }
            }
            ++i;
        }
    }

    [[nodiscard]] const oasis::uhai::FlowContract* FindContract(uint64_t contract_id) const noexcept {
        for (size_t i = 0; i < active_count; ++i) {
            if (contracts[i].contract_id == contract_id) {
                return &contracts[i];
            }
        }
        return nullptr;
    }
};

} // namespace oasis::flow

namespace oasis::uhai {
    using oasis::flow::TemporalFlowContractComponent;
} // namespace oasis::uhai

// Alias namespace for backwards compatibility
namespace uhai {
    using oasis::uhai::CACHE_LINE_SIZE;
    using oasis::uhai::RING_CAPACITY;
    using oasis::uhai::UHAI_MAGIC;
    using oasis::uhai::UHAI_VERSION;
    using oasis::uhai::DEFAULT_SHM_NAME;
    using oasis::uhai::TOTAL_SHM_SIZE;
    using oasis::uhai::QUALITY_SIMULATED;
    using oasis::uhai::QUALITY_HARDWARE_ROOT;
    using oasis::uhai::TelemetrySample;
    using oasis::uhai::TelemetrySlot;
    using oasis::uhai::SharedRingHeader;
    using oasis::uhai::IntentType;
    using oasis::uhai::SpatialIntentPayload;
    using oasis::uhai::LockFreeSpscRing;
    using oasis::uhai::UhaiTelemetryChannel;

    using oasis::uhai::FlowResourceType;
    using oasis::uhai::FlowContractStatus;
    using oasis::uhai::FlowContract;
    using oasis::uhai::FlowContractSlot;
    using oasis::uhai::DEFAULT_FLOW_SHM_NAME;
    using oasis::uhai::TOTAL_FLOW_SHM_SIZE;
    using oasis::uhai::UhaiChannel;
    using oasis::uhai::UhaiFlowChannel;
    using oasis::uhai::TemporalFlowContractComponent;

    template <typename T = TelemetrySample, size_t Capacity = RING_CAPACITY>
    using SPSCRingBuffer = oasis::uhai::LockFreeSpscRing<TelemetrySlot, Capacity>;
} // namespace uhai
