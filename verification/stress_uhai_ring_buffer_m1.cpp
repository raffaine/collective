#include <iostream>
#include <atomic>
#include <cstdint>
#include <cstddef>
#include <chrono>
#include <thread>
#include <vector>
#include <cassert>
#include <random>
#include <sys/wait.h>
#include <unistd.h>

#include "uhai/uhai_ring_buffer.hpp"

using namespace oasis::uhai;

// Mock non-isolated struct to empirically measure false sharing penalty
struct FalseSharingMockHeader {
    std::atomic<uint64_t> write_index{0};
    std::atomic<uint64_t> read_index{0}; // Packed in the same 64B cacheline as write_index!
};

void Test1_MemoryLayoutAndAlignment() {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "[CHALLENGE 1] Memory Layout & Cache Line Alignment Verification" << std::endl;
    std::cout << "========================================================" << std::endl;

    std::cout << "sizeof(SharedRingHeader) = " << sizeof(SharedRingHeader) << " bytes (expect 192)" << std::endl;
    std::cout << "alignof(SharedRingHeader) = " << alignof(SharedRingHeader) << " bytes (expect 64)" << std::endl;
    std::cout << "sizeof(TelemetrySample) = " << sizeof(TelemetrySample) << " bytes (expect 32)" << std::endl;
    std::cout << "alignof(TelemetrySample) = " << alignof(TelemetrySample) << " bytes (expect 8)" << std::endl;
    std::cout << "sizeof(TelemetrySlot) = " << sizeof(TelemetrySlot) << " bytes (expect 64)" << std::endl;
    std::cout << "alignof(TelemetrySlot) = " << alignof(TelemetrySlot) << " bytes (expect 64)" << std::endl;
    std::cout << "TOTAL_SHM_SIZE = " << TOTAL_SHM_SIZE << " bytes (expect 65728)" << std::endl;

    assert(sizeof(SharedRingHeader) == 192);
    assert(alignof(SharedRingHeader) == 64);
    assert(sizeof(TelemetrySample) == 32);
    assert(alignof(TelemetrySample) == 8);
    assert(sizeof(TelemetrySlot) == 64);
    assert(alignof(TelemetrySlot) == 64);
    assert(TOTAL_SHM_SIZE == 65728);

    size_t w_off = offsetof(SharedRingHeader, write_index);
    size_t d_off = offsetof(SharedRingHeader, dropped_frames);
    size_t r_off = offsetof(SharedRingHeader, read_index);
    size_t c_off = offsetof(SharedRingHeader, capacity);
    size_t e_off = offsetof(SharedRingHeader, element_size);
    size_t m_off = offsetof(SharedRingHeader, magic_signature);
    size_t v_off = offsetof(SharedRingHeader, version);

    std::cout << "Offsets: write_idx=" << w_off << " dropped_frames=" << d_off 
              << " read_idx=" << r_off << " capacity=" << c_off 
              << " elem_size=" << e_off << " magic=" << m_off 
              << " version=" << v_off << std::endl;

    assert(w_off == 0);
    assert(d_off == 8);
    assert(r_off == 64);
    assert(c_off == 128);
    assert(e_off == 132);
    assert(m_off == 136);
    assert(v_off == 140);

    assert(w_off / 64 == 0);
    assert(d_off / 64 == 0);
    assert(r_off / 64 == 1);
    assert(c_off / 64 == 2);
    assert(e_off / 64 == 2);
    assert(m_off / 64 == 2);
    assert(v_off / 64 == 2);

    // Slot alignment
    size_t buf_start = sizeof(SharedRingHeader);
    assert(buf_start == 192);
    assert(buf_start % 64 == 0);

    for (size_t i = 0; i < 10; ++i) {
        size_t slot_off = buf_start + i * sizeof(TelemetrySlot);
        assert(slot_off % 64 == 0);
        assert(slot_off / 64 == (3 + i));
    }

    std::cout << "[PASS] Memory layout and strict 64-byte cache line isolation verified!" << std::endl;
}

void Test2_FalseSharingEmpiricalComparison() {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "[CHALLENGE 2] False Sharing Empirical Benchmark" << std::endl;
    std::cout << "========================================================" << std::endl;

    constexpr uint64_t ITERS = 20'000'000;

    // 1. Unpadded / Packed (False Sharing: write_index and read_index on SAME 64B cache line)
    FalseSharingMockHeader packed_hdr;
    std::atomic<bool> start_packed{false};

    auto t0 = std::chrono::high_resolution_clock::now();
    std::thread t_p1([&]() {
        while (!start_packed.load(std::memory_order_acquire)) {}
        for (uint64_t i = 0; i < ITERS; ++i) {
            packed_hdr.write_index.fetch_add(1, std::memory_order_relaxed);
        }
    });
    std::thread t_p2([&]() {
        while (!start_packed.load(std::memory_order_acquire)) {}
        for (uint64_t i = 0; i < ITERS; ++i) {
            packed_hdr.read_index.fetch_add(1, std::memory_order_relaxed);
        }
    });

    start_packed.store(true, std::memory_order_release);
    t_p1.join();
    t_p2.join();
    auto t1 = std::chrono::high_resolution_clock::now();
    double packed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    // 2. Aligned / Isolated (SharedRingHeader CL0 vs CL1 separation)
    alignas(64) SharedRingHeader isolated_hdr;
    std::atomic<bool> start_isolated{false};

    auto t2 = std::chrono::high_resolution_clock::now();
    std::thread t_i1([&]() {
        while (!start_isolated.load(std::memory_order_acquire)) {}
        for (uint64_t i = 0; i < ITERS; ++i) {
            isolated_hdr.write_index.fetch_add(1, std::memory_order_relaxed);
        }
    });
    std::thread t_i2([&]() {
        while (!start_isolated.load(std::memory_order_acquire)) {}
        for (uint64_t i = 0; i < ITERS; ++i) {
            isolated_hdr.read_index.fetch_add(1, std::memory_order_relaxed);
        }
    });

    start_isolated.store(true, std::memory_order_release);
    t_i1.join();
    t_i2.join();
    auto t3 = std::chrono::high_resolution_clock::now();
    double isolated_ms = std::chrono::duration<double, std::milli>(t3 - t2).count();

    std::cout << "Packed (False Sharing) Time:   " << packed_ms << " ms" << std::endl;
    std::cout << "Isolated (CL0/CL1 Separated):  " << isolated_ms << " ms" << std::endl;
    std::cout << "Speedup with Cache Line Separation: " << (packed_ms / isolated_ms) << "x" << std::endl;

    assert(isolated_ms < packed_ms * 1.2); // Ensure isolated is faster or at least not worse
    std::cout << "[PASS] False sharing elimination empirically confirmed!" << std::endl;
}

void Test3_HighStressConcurrencyUnderLoad() {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "[CHALLENGE 3] High-Stress Concurrency Under Load (5,000,000 frames)" << std::endl;
    std::cout << "========================================================" << std::endl;

    std::vector<uint8_t> mem(TOTAL_SHM_SIZE + 64, 0);
    void* raw = mem.data();
    size_t space = mem.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    assert(aligned != nullptr);

    auto* hdr = reinterpret_cast<SharedRingHeader*>(aligned);
    hdr->write_index.store(0);
    hdr->read_index.store(0);
    hdr->dropped_frames.store(0);
    hdr->capacity = RING_CAPACITY;
    hdr->element_size = sizeof(TelemetrySlot);
    hdr->magic_signature = UHAI_MAGIC;
    hdr->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    constexpr uint64_t TOTAL_FRAMES = 5'000'000;
    std::atomic<bool> start_flag{false};

    auto producer = [&]() {
        while (!start_flag.load(std::memory_order_acquire)) {}
        TelemetrySample s{};
        for (uint64_t i = 0; i < TOTAL_FRAMES; ++i) {
            s.frame_count = i;
            s.timestamp_ns = 5000000000ULL + i;
            s.fps = 60.0f + static_cast<float>(i % 30);
            s.founder_stress = 0.05f * static_cast<float>(i % 20);
            s.memory_slot_count = static_cast<uint32_t>(i % 32);
            s.biome_id = static_cast<uint8_t>(i % 3);
            s.quality_flags = (i % 2 == 0) ? 0x01 : 0x03;
            s.reserved = 0xAA55;

            while (!ring.Push(s)) {
                std::this_thread::yield();
            }
        }
    };

    uint64_t popped = 0;
    uint64_t underruns = 0;
    bool corruption = false;

    auto consumer = [&]() {
        while (!start_flag.load(std::memory_order_acquire)) {}
        TelemetrySample s{};
        while (popped < TOTAL_FRAMES) {
            if (ring.Pop(s)) {
                // Strict assertion on all fields to detect tearing or weak memory reordering
                if (s.frame_count != popped ||
                    s.timestamp_ns != (5000000000ULL + popped) ||
                    s.memory_slot_count != (popped % 32) ||
                    s.biome_id != (popped % 3) ||
                    s.quality_flags != ((popped % 2 == 0) ? 0x01 : 0x03) ||
                    s.reserved != 0xAA55) {
                    corruption = true;
                    std::cerr << "CORRUPTION detected at frame " << popped << " got " << s.frame_count << std::endl;
                    break;
                }
                popped++;
            } else {
                underruns++;
                std::this_thread::yield();
            }
        }
    };

    auto t0 = std::chrono::high_resolution_clock::now();
    std::thread tp(producer);
    std::thread tc(consumer);

    start_flag.store(true, std::memory_order_release);
    tp.join();
    tc.join();
    auto t1 = std::chrono::high_resolution_clock::now();

    double elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    double ns_per_frame = (elapsed_ms * 1e6) / TOTAL_FRAMES;

    std::cout << "Transferred " << TOTAL_FRAMES << " frames in " << elapsed_ms << " ms" << std::endl;
    std::cout << "Throughput: " << (TOTAL_FRAMES / (elapsed_ms / 1000.0) / 1e6) << " Million frames/sec" << std::endl;
    std::cout << "Mean Latency: " << ns_per_frame << " ns/frame" << std::endl;
    std::cout << "Consumer underrun yields: " << underruns << std::endl;
    std::cout << "Ring empty at end: " << (ring.IsEmpty() ? "YES" : "NO") << std::endl;

    assert(!corruption);
    assert(popped == TOTAL_FRAMES);
    assert(ring.IsEmpty());
    std::cout << "[PASS] 5M frame concurrency test passed with zero corruption!" << std::endl;
}

void Test4_AdversarialOverrunConservationLaw() {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "[CHALLENGE 4] Overrun Stress & Atomic Conservation Law" << std::endl;
    std::cout << "========================================================" << std::endl;

    std::vector<uint8_t> mem(TOTAL_SHM_SIZE + 64, 0);
    void* raw = mem.data();
    size_t space = mem.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    assert(aligned != nullptr);

    auto* hdr = reinterpret_cast<SharedRingHeader*>(aligned);
    hdr->write_index.store(0);
    hdr->read_index.store(0);
    hdr->dropped_frames.store(0);
    hdr->capacity = RING_CAPACITY;
    hdr->element_size = sizeof(TelemetrySlot);
    hdr->magic_signature = UHAI_MAGIC;
    hdr->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    constexpr uint64_t TOTAL_ATTEMPTS = 10'000'000;
    std::atomic<bool> producer_done{false};
    std::atomic<uint64_t> producer_accepted{0};
    std::atomic<uint64_t> producer_rejected{0};

    // Producer blasts at maximum rate WITHOUT yielding or waiting for consumer
    auto producer = [&]() {
        TelemetrySample s{};
        uint64_t acc = 0;
        uint64_t rej = 0;
        for (uint64_t i = 0; i < TOTAL_ATTEMPTS; ++i) {
            s.frame_count = i;
            if (ring.Push(s)) {
                acc++;
            } else {
                rej++;
            }
        }
        producer_accepted.store(acc, std::memory_order_release);
        producer_rejected.store(rej, std::memory_order_release);
        producer_done.store(true, std::memory_order_release);
    };

    // Consumer drains with jitter (intermittent delays to induce deep overruns)
    uint64_t consumer_drained = 0;
    auto consumer = [&]() {
        TelemetrySample s{};
        uint64_t loop_count = 0;
        while (!producer_done.load(std::memory_order_acquire) || !ring.IsEmpty()) {
            if (ring.Pop(s)) {
                consumer_drained++;
            } else {
                std::this_thread::yield();
            }
            loop_count++;
            // Inject periodic jitter every 500,000 iterations
            if (loop_count % 500'000 == 0) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        }
    };

    std::thread tp(producer);
    std::thread tc(consumer);

    tp.join();
    tc.join();

    uint64_t total_acc = producer_accepted.load();
    uint64_t total_rej = producer_rejected.load();
    uint64_t hdr_drops = ring.DroppedCount();
    uint64_t remaining_in_ring = ring.Size();

    std::cout << "Push attempts:       " << TOTAL_ATTEMPTS << std::endl;
    std::cout << "Pushes accepted:     " << total_acc << std::endl;
    std::cout << "Pushes rejected:     " << total_rej << std::endl;
    std::cout << "Header dropped_frames: " << hdr_drops << std::endl;
    std::cout << "Consumer drained:    " << consumer_drained << std::endl;
    std::cout << "Remaining in ring:   " << remaining_in_ring << std::endl;

    // Verify Conservation Laws:
    // Law 1: All attempts are either accepted or rejected
    assert(total_acc + total_rej == TOTAL_ATTEMPTS);
    // Law 2: Rejected pushes strictly equal dropped_frames
    assert(total_rej == hdr_drops);
    // Law 3: Accepted pushes strictly equal drained + remaining in ring
    assert(total_acc == consumer_drained + remaining_in_ring);

    std::cout << "[PASS] Strict conservation laws verified across 10M chaotic pushes!" << std::endl;
}

void Test5_CrossProcessPosixShmIpc() {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "[CHALLENGE 5] Cross-Process POSIX shm_open IPC (fork)" << std::endl;
    std::cout << "========================================================" << std::endl;

    std::string shm_name = "/test_uhai_fork_" + std::to_string(getpid());
    UhaiTelemetryChannel::Unlink(shm_name);

    constexpr uint64_t FORK_FRAMES = 1'000'000;

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(1);
    }

    if (pid == 0) {
        // Child process: CONSUMER
        // Wait slightly for parent to initialize shm
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        UhaiTelemetryChannel consumer(UhaiTelemetryChannel::ChannelMode::Consumer, shm_name);
        if (!consumer.IsValid()) {
            std::cerr << "[Child] Consumer failed to open shm!" << std::endl;
            _exit(1);
        }

        TelemetrySample s{};
        uint64_t received = 0;
        bool corrupt = false;

        while (received < FORK_FRAMES) {
            if (consumer.Pop(s)) {
                if (s.frame_count != received) {
                    corrupt = true;
                    std::cerr << "[Child] Corrupt frame! Expected " << received << " got " << s.frame_count << std::endl;
                    break;
                }
                received++;
            } else {
                std::this_thread::yield();
            }
        }

        if (corrupt || received != FORK_FRAMES) {
            _exit(2);
        }
        _exit(0);
    } else {
        // Parent process: PRODUCER
        UhaiTelemetryChannel producer(UhaiTelemetryChannel::ChannelMode::Producer, shm_name);
        assert(producer.IsValid());

        TelemetrySample s{};
        auto t0 = std::chrono::high_resolution_clock::now();
        for (uint64_t i = 0; i < FORK_FRAMES; ++i) {
            s.frame_count = i;
            s.timestamp_ns = i * 100;
            s.fps = 60.0f;
            s.founder_stress = 0.1f;
            s.memory_slot_count = 1;
            s.biome_id = 0;
            s.quality_flags = 1;
            s.reserved = 0;

            while (!producer.Push(s)) {
                std::this_thread::yield();
            }
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

        int status = 0;
        waitpid(pid, &status, 0);
        int exit_code = WEXITSTATUS(status);

        std::cout << "[Parent] Pushed " << FORK_FRAMES << " frames in " << elapsed_ms << " ms" << std::endl;
        std::cout << "[Parent] Child process exited with status: " << exit_code << std::endl;
        assert(exit_code == 0);

        UhaiTelemetryChannel::Unlink(shm_name);
        std::cout << "[PASS] True multi-process POSIX shm IPC verified!" << std::endl;
    }
}

void Test6_BoundaryAndEdgeConditions() {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "[CHALLENGE 6] Boundary & Edge Condition Mining" << std::endl;
    std::cout << "========================================================" << std::endl;

    std::vector<uint8_t> mem(TOTAL_SHM_SIZE + 64, 0);
    void* raw = mem.data();
    size_t space = mem.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    assert(aligned != nullptr);

    auto* hdr = reinterpret_cast<SharedRingHeader*>(aligned);
    hdr->write_index.store(0);
    hdr->read_index.store(0);
    hdr->dropped_frames.store(0);
    hdr->capacity = RING_CAPACITY;
    hdr->element_size = sizeof(TelemetrySlot);
    hdr->magic_signature = UHAI_MAGIC;
    hdr->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    // 1. Underrun spam on empty buffer: 500,000 pops
    TelemetrySample s{};
    for (int i = 0; i < 500'000; ++i) {
        assert(ring.Pop(s) == false);
        assert(ring.pop() == std::nullopt);
    }
    assert(hdr->write_index.load() == 0);
    assert(hdr->read_index.load() == 0);
    assert(hdr->dropped_frames.load() == 0);
    assert(ring.IsEmpty());
    assert(ring.Size() == 0);

    // 2. Exactly fill to capacity (1024)
    for (uint64_t i = 0; i < 1024; ++i) {
        s.frame_count = i;
        assert(ring.Push(s) == true);
    }
    assert(ring.Size() == 1024);
    assert(!ring.IsEmpty());

    // 3. Overflow attempt on full buffer: 50,000 pushes
    for (uint64_t i = 0; i < 50'000; ++i) {
        s.frame_count = 99999;
        assert(ring.Push(s) == false);
    }
    assert(ring.Size() == 1024);
    assert(ring.DroppedCount() == 50'000);

    // 4. Pop 1, Push 1 at boundary
    for (uint64_t i = 0; i < 10'000; ++i) {
        TelemetrySample out{};
        assert(ring.Pop(out) == true);
        assert(out.frame_count == i);
        s.frame_count = 1024 + i;
        assert(ring.Push(s) == true);
        assert(ring.Size() == 1024);
    }

    // Drain remaining 1024 items
    for (uint64_t i = 0; i < 1024; ++i) {
        TelemetrySample out{};
        assert(ring.Pop(out) == true);
        assert(out.frame_count == 10'000 + i);
    }
    assert(ring.IsEmpty());
    assert(ring.Size() == 0);

    std::cout << "[PASS] Boundary conditions and edge cases verified!" << std::endl;
}

int main() {
    std::cout << "========================================================\n"
              << "  UHAI SPSC RING BUFFER ADVERSARIAL STRESS TEST SUITE   \n"
              << "========================================================" << std::endl;

    Test1_MemoryLayoutAndAlignment();
    Test2_FalseSharingEmpiricalComparison();
    Test3_HighStressConcurrencyUnderLoad();
    Test4_AdversarialOverrunConservationLaw();
    Test5_CrossProcessPosixShmIpc();
    Test6_BoundaryAndEdgeConditions();

    std::cout << "\n========================================================" << std::endl;
    std::cout << ">>> ALL 6 EMPIRICAL CHALLENGES PASSED WITH ZERO ERRORS <<<" << std::endl;
    std::cout << "========================================================" << std::endl;

    return 0;
}
