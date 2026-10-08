#pragma once

#include <atomic>
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
    uint8_t  quality_flags{0};      // Bit 0: Valid, Bit 1: Simulated (SITL) (1 byte)
    uint16_t reserved{0};           // Struct padding for strict 8-byte alignment (2 bytes)
};
#pragma pack(pop)
static_assert(sizeof(TelemetrySample) == 32, "TelemetrySample must be exactly 32 bytes.");

struct alignas(CACHE_LINE_SIZE) TelemetrySlot {
    TelemetrySample sample;
    uint8_t pad[32]; // Pads 32 bytes to 64 bytes (1 CPU cache line)
};
static_assert(sizeof(TelemetrySlot) == 64, "TelemetrySlot must be exactly 64 bytes.");

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

    std::optional<TelemetrySample> Pop() {
        TelemetrySample s;
        if (Pop(s)) {
            return s;
        }
        return std::nullopt;
    }

    std::optional<TelemetrySample> pop() {
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

} // namespace oasis::uhai

// Alias namespace for backwards compatibility
namespace uhai {
    using oasis::uhai::CACHE_LINE_SIZE;
    using oasis::uhai::RING_CAPACITY;
    using oasis::uhai::UHAI_MAGIC;
    using oasis::uhai::UHAI_VERSION;
    using oasis::uhai::DEFAULT_SHM_NAME;
    using oasis::uhai::TOTAL_SHM_SIZE;
    using oasis::uhai::TelemetrySample;
    using oasis::uhai::TelemetrySlot;
    using oasis::uhai::SharedRingHeader;
    using oasis::uhai::LockFreeSpscRing;
    using oasis::uhai::UhaiTelemetryChannel;

    template <typename T = TelemetrySample, size_t Capacity = RING_CAPACITY>
    using SPSCRingBuffer = oasis::uhai::LockFreeSpscRing<TelemetrySlot, Capacity>;
} // namespace uhai
