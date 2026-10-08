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

#pragma pack(push, 1)
struct TelemetryFrame {
    uint64_t timestamp_hlc;   // Hybrid Logical Clock timestamp
    uint32_t sensor_id;       // Virtual / Physical sensor register ID
    float    value;           // Measured / simulated physical value
    uint8_t  unit_enum;       // 0=Volts, 1=Amps, 2=DegC, 3=Liter/min, 4=MoisturePct
    uint8_t  quality_flags;   // Bit 0: Valid, Bit 1: Simulated (Oasis SITL), Bit 2: Anomaly
    uint16_t reserved;
    uint8_t  cryptographic_mac[12]; // Poly1305 MAC tag from hardware root
};
#pragma pack(pop)
static_assert(sizeof(TelemetryFrame) == 32, "TelemetryFrame must be exactly 32 bytes.");

struct alignas(CACHE_LINE_SIZE) SharedRingHeader {
    alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> write_index{0};
    alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> read_index{0};
    uint32_t capacity{RING_CAPACITY};
    uint32_t element_size{sizeof(TelemetryFrame)};
    uint32_t magic_signature{0x55484149}; // "UHAI"
    uint32_t version{1};
};

template <typename T, size_t Capacity = RING_CAPACITY>
class LockFreeSpscRing {
public:
    explicit LockFreeSpscRing(void* raw_shm_ptr)
        : header_(reinterpret_cast<SharedRingHeader*>(raw_shm_ptr)),
          buffer_(reinterpret_cast<T*>(static_cast<char*>(raw_shm_ptr) + sizeof(SharedRingHeader))) {}

    bool Push(const T& item) {
        const uint64_t w = header_->write_index.load(std::memory_order_relaxed);
        const uint64_t r = header_->read_index.load(std::memory_order_acquire);
        if ((w - r) >= Capacity) return false; // Buffer full
        buffer_[w & (Capacity - 1)] = item;
        header_->write_index.store(w + 1, std::memory_order_release);
        return true;
    }

    bool Pop(T& item) {
        const uint64_t r = header_->read_index.load(std::memory_order_relaxed);
        const uint64_t w = header_->write_index.load(std::memory_order_acquire);
        if (r == w) return false; // Buffer empty
        item = buffer_[r & (Capacity - 1)];
        header_->read_index.store(r + 1, std::memory_order_release);
        return true;
    }

    SharedRingHeader* GetHeader() { return header_; }
    T* GetBuffer() { return buffer_; }
private:
    SharedRingHeader* header_;
    T* buffer_;
};

} // namespace oasis::uhai

int main() {
    using namespace oasis::uhai;
    std::cout << "=== EMPIRICAL CHALLENGE: UHAI SPSC RING BUFFER ===" << std::endl;

    // 1. Inspect Struct Layout & Cache Line Alignment
    std::cout << "\n[1] Memory Layout & Cache Line Analysis:" << std::endl;
    std::cout << "sizeof(SharedRingHeader) = " << sizeof(SharedRingHeader) << " bytes" << std::endl;
    std::cout << "alignof(SharedRingHeader) = " << alignof(SharedRingHeader) << " bytes" << std::endl;
    std::cout << "offset(write_index) = " << offsetof(SharedRingHeader, write_index) << std::endl;
    std::cout << "offset(read_index)  = " << offsetof(SharedRingHeader, read_index) << std::endl;
    std::cout << "offset(capacity)    = " << offsetof(SharedRingHeader, capacity) << std::endl;
    std::cout << "offset(element_size)= " << offsetof(SharedRingHeader, element_size) << std::endl;
    std::cout << "offset(magic_sig)   = " << offsetof(SharedRingHeader, magic_signature) << std::endl;
    std::cout << "offset(version)     = " << offsetof(SharedRingHeader, version) << std::endl;

    size_t read_idx_cacheline = offsetof(SharedRingHeader, read_index) / 64;
    size_t capacity_cacheline = offsetof(SharedRingHeader, capacity) / 64;
    std::cout << "read_index cacheline: " << read_idx_cacheline 
              << ", capacity cacheline: " << capacity_cacheline << std::endl;
    if (read_idx_cacheline == capacity_cacheline) {
        std::cout << "-> DETECTED: capacity, element_size, magic_sig share cache line with read_index!" << std::endl;
    }

    // Check payload buffer cache line sharing
    size_t buffer_offset = sizeof(SharedRingHeader);
    std::cout << "Buffer start offset: " << buffer_offset << std::endl;
    std::cout << "sizeof(TelemetryFrame): " << sizeof(TelemetryFrame) << " bytes" << std::endl;
    std::cout << "Slot 0 byte range: [" << buffer_offset << " .. " << buffer_offset + sizeof(TelemetryFrame) - 1 << "]"
              << " (Cache line " << buffer_offset / 64 << ")" << std::endl;
    std::cout << "Slot 1 byte range: [" << buffer_offset + sizeof(TelemetryFrame) << " .. " << buffer_offset + 2*sizeof(TelemetryFrame) - 1 << "]"
              << " (Cache line " << (buffer_offset + sizeof(TelemetryFrame)) / 64 << ")" << std::endl;

    if ((buffer_offset / 64) == ((buffer_offset + sizeof(TelemetryFrame)) / 64)) {
        std::cout << "-> DETECTED: Adjacent slots 0 and 1 share the SAME 64-byte cache line! Producer writing slot 1 invalidates consumer reading slot 0!" << std::endl;
    }

    // 2. Concurrency & Contention Benchmark
    std::cout << "\n[2] Concurrency & Contention Benchmark (1,000,000 frames):" << std::endl;
    size_t total_shm_size = sizeof(SharedRingHeader) + RING_CAPACITY * sizeof(TelemetryFrame);
    std::vector<uint8_t> shm_storage(total_shm_size + 64, 0);
    // Align to 64 bytes
    void* raw_ptr = shm_storage.data();
    size_t space = shm_storage.size();
    void* aligned_shm = std::align(64, total_shm_size, raw_ptr, space);
    assert(aligned_shm != nullptr);

    LockFreeSpscRing<TelemetryFrame, RING_CAPACITY> ring(aligned_shm);

    constexpr uint64_t TOTAL_FRAMES = 1'000'000;
    std::atomic<bool> start_flag{false};
    std::atomic<uint64_t> dropped_frames{0};

    auto producer = [&]() {
        while (!start_flag.load(std::memory_order_acquire)) {}
        TelemetryFrame frame{};
        for (uint64_t i = 0; i < TOTAL_FRAMES; ++i) {
            frame.timestamp_hlc = i;
            frame.sensor_id = static_cast<uint32_t>(i % 32);
            frame.value = static_cast<float>(i);
            while (!ring.Push(frame)) {
                // Buffer full - spin/overrun
                dropped_frames.fetch_add(1, std::memory_order_relaxed);
                std::this_thread::yield();
            }
        }
    };

    uint64_t popped_frames = 0;
    uint64_t pop_underruns = 0;
    auto consumer = [&]() {
        while (!start_flag.load(std::memory_order_acquire)) {}
        TelemetryFrame frame{};
        while (popped_frames < TOTAL_FRAMES) {
            if (ring.Pop(frame)) {
                if (frame.timestamp_hlc != popped_frames) {
                    std::cerr << "DATA CORRUPTION DETECTED! Expected " << popped_frames << " got " << frame.timestamp_hlc << std::endl;
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
    std::cout << "Producer retry/overrun yields: " << dropped_frames.load() << std::endl;
    std::cout << "Consumer underrun yields: " << pop_underruns << std::endl;

    // 3. Overrun Stress Test at 60 Hz Telemetry Load
    std::cout << "\n[3] 60 Hz Simulated Telemetry Overrun Test (Consumer Hiccough):" << std::endl;
    // Simulate 50 sensors at 60 Hz = 3000 frames/sec.
    // If consumer freezes for 400ms (e.g. disk I/O, GC, or browser tab blur):
    ring.GetHeader()->write_index.store(0);
    ring.GetHeader()->read_index.store(0);

    // Producer sends 1500 frames during consumer freeze:
    uint64_t push_success = 0;
    uint64_t push_failed = 0;
    for (int i = 0; i < 1500; ++i) {
        TelemetryFrame f{};
        f.timestamp_hlc = i;
        if (ring.Push(f)) {
            push_success++;
        } else {
            push_failed++;
        }
    }
    std::cout << "Frames pushed: " << push_success << ", Frames dropped (Push failed): " << push_failed << std::endl;
    std::cout << "Capacity: " << RING_CAPACITY << std::endl;
    std::cout << "Lost frames percentage during 500ms freeze: " << (push_failed * 100.0 / 1500.0) << "%" << std::endl;
    std::cout << "Does SharedRingHeader record dropped count? " << "NO (No dropped_count field in struct)" << std::endl;

    return 0;
}
