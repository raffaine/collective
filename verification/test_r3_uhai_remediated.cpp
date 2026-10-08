#include <iostream>
#include <atomic>
#include <cstdint>
#include <cstddef>
#include <chrono>
#include <thread>
#include <vector>
#include <cassert>

namespace oasis::uhai {

constexpr size_t CACHE_LINE_SIZE = 64;
constexpr size_t RING_CAPACITY = 1024; // Power of 2

#pragma pack(push, 8)
struct alignas(8) TelemetrySample {
    uint64_t timestamp_hlc;     // Monotonic Hybrid Logical Clock (Layer 3)
    uint32_t sensor_register;   // Virtual or physical hardware register ID
    float    value;             // Measured / simulated physical quantity
    uint64_t flags;             // Bit 0: Valid, Bit 1: Simulated (SITL), Bits 2-7: Quality flags
};
#pragma pack(pop)
static_assert(sizeof(TelemetrySample) == 24, "TelemetrySample must be exactly 24 bytes.");

#pragma pack(push, 8)
struct alignas(8) ActuatorCommand {
    uint64_t cmd_id;            // Monotonic command UUID
    uint16_t actuator_id;       // Actuator register ID (valve, relay, breaker)
    uint16_t cmd_type;          // Command opcode / type enum
    uint32_t payload_len;       // Active payload length (<= 48 bytes if payload mode)
    uint8_t  auth_signature[64];// Ed25519 signature from Layer 4 execution key
};
#pragma pack(pop)
static_assert(sizeof(ActuatorCommand) == 80, "ActuatorCommand must be exactly 80 bytes.");

struct alignas(CACHE_LINE_SIZE) TelemetrySlot {
    TelemetrySample sample;
    uint8_t pad[40];            // Pads slot to exactly 64 bytes (1 cache line)
};
static_assert(sizeof(TelemetrySlot) == 64, "TelemetrySlot must be exactly 64 bytes.");

struct alignas(CACHE_LINE_SIZE) ActuatorSlot {
    ActuatorCommand cmd;
    uint8_t pad[48];            // Pads slot to exactly 128 bytes (2 cache lines)
};
static_assert(sizeof(ActuatorSlot) == 128, "ActuatorSlot must be exactly 128 bytes.");

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
    uint32_t magic_signature{0x55484149}; // "UHAI"
    uint32_t version{1};
    uint8_t pad2[48];
};
static_assert(sizeof(SharedRingHeader) == 192, "SharedRingHeader must occupy exactly 3 cache lines (192 bytes).");

template <typename SlotT, size_t Capacity = RING_CAPACITY>
class LockFreeSpscRing {
public:
    explicit LockFreeSpscRing(void* raw_shm_ptr)
        : header_(reinterpret_cast<SharedRingHeader*>(raw_shm_ptr)),
          buffer_(reinterpret_cast<SlotT*>(static_cast<char*>(raw_shm_ptr) + sizeof(SharedRingHeader))) {}

    template <typename ItemT>
    bool Push(const ItemT& item) {
        const uint64_t w = header_->write_index.load(std::memory_order_relaxed);
        const uint64_t r = header_->read_index.load(std::memory_order_acquire);
        if ((w - r) >= Capacity) {
            header_->dropped_frames.fetch_add(1, std::memory_order_relaxed);
            return false; // Buffer full; overrun recorded atomically
        }
        buffer_[w & (Capacity - 1)].sample = item;
        header_->write_index.store(w + 1, std::memory_order_release);
        return true;
    }

    template <typename ItemT>
    bool Pop(ItemT& item) {
        const uint64_t r = header_->read_index.load(std::memory_order_relaxed);
        const uint64_t w = header_->write_index.load(std::memory_order_acquire);
        if (r == w) return false; // Buffer empty
        item = buffer_[r & (Capacity - 1)].sample;
        header_->read_index.store(r + 1, std::memory_order_release);
        return true;
    }

    SharedRingHeader* GetHeader() { return header_; }
    SlotT* GetBuffer() { return buffer_; }
private:
    SharedRingHeader* header_;
    SlotT* buffer_;
};

} // namespace oasis::uhai

int main() {
    using namespace oasis::uhai;
    std::cout << "=== EMPIRICAL CHALLENGE: REMEDIATED UHAI SPSC RING BUFFER ===" << std::endl;

    // 1. Inspect Struct Layout & Cache Line Alignment
    std::cout << "\n[1] Memory Layout & Cache Line Analysis:" << std::endl;
    std::cout << "sizeof(SharedRingHeader) = " << sizeof(SharedRingHeader) << " bytes" << std::endl;
    std::cout << "alignof(SharedRingHeader) = " << alignof(SharedRingHeader) << " bytes" << std::endl;
    std::cout << "offset(write_index)   = " << offsetof(SharedRingHeader, write_index) << std::endl;
    std::cout << "offset(dropped_frames)= " << offsetof(SharedRingHeader, dropped_frames) << std::endl;
    std::cout << "offset(read_index)    = " << offsetof(SharedRingHeader, read_index) << std::endl;
    std::cout << "offset(capacity)      = " << offsetof(SharedRingHeader, capacity) << std::endl;
    std::cout << "offset(element_size)  = " << offsetof(SharedRingHeader, element_size) << std::endl;
    std::cout << "offset(magic_sig)     = " << offsetof(SharedRingHeader, magic_signature) << std::endl;
    std::cout << "offset(version)       = " << offsetof(SharedRingHeader, version) << std::endl;

    size_t write_idx_cl = offsetof(SharedRingHeader, write_index) / 64;
    size_t dropped_cl   = offsetof(SharedRingHeader, dropped_frames) / 64;
    size_t read_idx_cl  = offsetof(SharedRingHeader, read_index) / 64;
    size_t capacity_cl  = offsetof(SharedRingHeader, capacity) / 64;

    std::cout << "write_index cacheline:   " << write_idx_cl << std::endl;
    std::cout << "dropped_frames cacheline:" << dropped_cl << std::endl;
    std::cout << "read_index cacheline:    " << read_idx_cl << std::endl;
    std::cout << "capacity cacheline:      " << capacity_cl << std::endl;

    assert(write_idx_cl == 0);
    assert(dropped_cl == 0);
    assert(read_idx_cl == 1);
    assert(capacity_cl == 2);
    std::cout << "-> CONFIRMED: Strict 3-cache-line header isolation verified! No false sharing between producer (CL0), consumer (CL1), and config (CL2)." << std::endl;

    // Check payload buffer cache line sharing
    size_t buffer_offset = sizeof(SharedRingHeader);
    std::cout << "Buffer start offset: " << buffer_offset << std::endl;
    std::cout << "sizeof(TelemetrySlot): " << sizeof(TelemetrySlot) << " bytes" << std::endl;
    size_t slot0_cl = buffer_offset / 64;
    size_t slot1_cl = (buffer_offset + sizeof(TelemetrySlot)) / 64;
    std::cout << "Slot 0 Cache Line: " << slot0_cl << " | Slot 1 Cache Line: " << slot1_cl << std::endl;
    assert(slot0_cl != slot1_cl);
    std::cout << "-> CONFIRMED: Adjacent slots occupy distinct 64-byte cache lines! Zero slot-level false sharing." << std::endl;

    // 2. Concurrency & Contention Benchmark
    std::cout << "\n[2] Concurrency & Contention Benchmark (1,000,000 frames):" << std::endl;
    size_t total_shm_size = sizeof(SharedRingHeader) + RING_CAPACITY * sizeof(TelemetrySlot);
    std::vector<uint8_t> shm_storage(total_shm_size + 64, 0);
    void* raw_ptr = shm_storage.data();
    size_t space = shm_storage.size();
    void* aligned_shm = std::align(64, total_shm_size, raw_ptr, space);
    assert(aligned_shm != nullptr);

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned_shm);

    constexpr uint64_t TOTAL_FRAMES = 1'000'000;
    std::atomic<bool> start_flag{false};

    auto producer = [&]() {
        while (!start_flag.load(std::memory_order_acquire)) {}
        TelemetrySample sample{};
        for (uint64_t i = 0; i < TOTAL_FRAMES; ++i) {
            sample.timestamp_hlc = i;
            sample.sensor_register = static_cast<uint32_t>(i % 32);
            sample.value = static_cast<float>(i);
            sample.flags = 1;
            while (!ring.Push(sample)) {
                std::this_thread::yield();
            }
        }
    };

    uint64_t popped_frames = 0;
    uint64_t pop_underruns = 0;
    auto consumer = [&]() {
        while (!start_flag.load(std::memory_order_acquire)) {}
        TelemetrySample sample{};
        while (popped_frames < TOTAL_FRAMES) {
            if (ring.Pop(sample)) {
                if (sample.timestamp_hlc != popped_frames) {
                    std::cerr << "DATA CORRUPTION DETECTED! Expected " << popped_frames << " got " << sample.timestamp_hlc << std::endl;
                    exit(1);
                }
                popped_frames++;
            } else {
                pop_underruns++;
                std::this_thread::yield();
            }
        }
    };

    auto t0 = std::chrono::high_resolution_clock::now();
    std::thread t_prod(producer);
    std::thread t_cons(consumer);
    start_flag.store(true, std::memory_order_release);

    t_prod.join();
    t_cons.join();
    auto t1 = std::chrono::high_resolution_clock::now();

    double elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    double per_frame_ns = (elapsed_ms * 1e6) / TOTAL_FRAMES;
    std::cout << "Transferred " << TOTAL_FRAMES << " frames in " << elapsed_ms << " ms" << std::endl;
    std::cout << "Mean transfer latency per frame: " << per_frame_ns << " ns" << std::endl;
    std::cout << "Consumer underrun yields: " << pop_underruns << std::endl;

    // 3. Overrun Stress Test at 60 Hz Telemetry Load
    std::cout << "\n[3] Simulated Telemetry Overrun Test (Consumer Freeze):" << std::endl;
    ring.GetHeader()->write_index.store(0);
    ring.GetHeader()->read_index.store(0);
    ring.GetHeader()->dropped_frames.store(0);

    // Producer attempts 1500 pushes into a 1024-capacity buffer while consumer is frozen
    uint64_t push_success = 0;
    uint64_t push_failed = 0;
    for (int i = 0; i < 1500; ++i) {
        TelemetrySample s{};
        s.timestamp_hlc = i;
        if (ring.Push(s)) {
            push_success++;
        } else {
            push_failed++;
        }
    }
    uint64_t recorded_drops = ring.GetHeader()->dropped_frames.load(std::memory_order_relaxed);
    std::cout << "Frames pushed: " << push_success << ", Frames dropped (Push failed): " << push_failed << std::endl;
    std::cout << "SharedRingHeader recorded dropped_frames: " << recorded_drops << std::endl;
    assert(push_success == 1024);
    assert(push_failed == 476);
    assert(recorded_drops == 476);
    std::cout << "-> CONFIRMED: Overrun drops recorded atomically with 100% accuracy (" << recorded_drops << " drops)!" << std::endl;

    std::cout << "\nALL UHAI SPSC RING BUFFER EMPIRICAL TESTS PASSED [PASS]" << std::endl;
    return 0;
}
