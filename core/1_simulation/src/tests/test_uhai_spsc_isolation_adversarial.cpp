#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cassert>
#include <set>
#include <mutex>
#include "uhai/uhai_ring_buffer.hpp"

using namespace oasis::uhai;

// Verification Test 1: SPSC 1:1 Isolation (Producer -> Consumer)
bool VerifySingleProducerSingleConsumer() {
    std::cout << "\n[TEST 1] Single-Producer Single-Consumer (1:1) Invariant Verification..." << std::endl;
    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    assert(aligned != nullptr);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    constexpr uint64_t TOTAL_ITEMS = 200'000;
    std::atomic<bool> producer_done{false};
    std::atomic<uint64_t> received_count{0};
    std::vector<uint64_t> received_frames;
    received_frames.reserve(TOTAL_ITEMS);

    auto start_time = std::chrono::steady_clock::now();

    // Single Producer thread
    std::thread producer([&]() {
        for (uint64_t i = 1; i <= TOTAL_ITEMS; ++i) {
            TelemetrySample s{};
            s.frame_count = i;
            s.timestamp_ns = 1000 + i;
            s.quality_flags = QUALITY_HARDWARE_ROOT;

            while (!ring.Push(s)) {
                std::this_thread::yield();
            }
        }
        producer_done.store(true, std::memory_order_release);
    });

    // Single Consumer thread
    std::thread consumer([&]() {
        while (!producer_done.load(std::memory_order_acquire) || !ring.IsEmpty()) {
            TelemetrySample s{};
            if (ring.Pop(s)) {
                received_frames.push_back(s.frame_count);
                received_count.fetch_add(1, std::memory_order_relaxed);
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "  Transferred " << received_count.load() << " / " << TOTAL_ITEMS << " frames in " << elapsed_ms << " ms." << std::endl;

    if (received_count.load() != TOTAL_ITEMS) {
        std::cerr << "  FAIL: Received count mismatch: " << received_count.load() << " != " << TOTAL_ITEMS << std::endl;
        return false;
    }

    // Verify strictly monotonic ordering and zero loss/duplicates
    for (uint64_t i = 0; i < TOTAL_ITEMS; ++i) {
        if (received_frames[i] != i + 1) {
            std::cerr << "  FAIL: Out of order frame at index " << i << ": expected " << (i + 1) << ", got " << received_frames[i] << std::endl;
            return false;
        }
    }

    std::cout << "  PASS: SPSC 1:1 invariant held perfectly: 0 drops, 0 duplicates, strict FIFO ordering." << std::endl;
    return true;
}

// Verification Test 2: Multi-Consumer Race Condition Vulnerability Demonstration (1 Producer, 2 Consumers)
bool VerifyMultiConsumerContention() {
    std::cout << "\n[TEST 2] Multi-Consumer (1:2) Contention Failure Demonstration..." << std::endl;
    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    assert(aligned != nullptr);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    constexpr uint64_t TOTAL_ITEMS = 50'000;
    std::atomic<bool> producer_done{false};
    std::vector<uint64_t> consumer_a_frames;
    std::vector<uint64_t> consumer_b_frames;
    consumer_a_frames.reserve(TOTAL_ITEMS);
    consumer_b_frames.reserve(TOTAL_ITEMS);

    std::thread producer([&]() {
        for (uint64_t i = 1; i <= TOTAL_ITEMS; ++i) {
            TelemetrySample s{};
            s.frame_count = i;
            s.quality_flags = QUALITY_HARDWARE_ROOT;

            while (!ring.Push(s)) {
                std::this_thread::yield();
            }
        }
        producer_done.store(true, std::memory_order_release);
    });

    // Consumer A (representing col-execd)
    std::thread consumer_a([&]() {
        while (!producer_done.load(std::memory_order_acquire) || !ring.IsEmpty()) {
            TelemetrySample s{};
            if (ring.Pop(s)) {
                consumer_a_frames.push_back(s.frame_count);
            } else {
                std::this_thread::yield();
            }
        }
    });

    // Consumer B (representing uncoordinated col-gatewayd reading same ring)
    std::thread consumer_b([&]() {
        while (!producer_done.load(std::memory_order_acquire) || !ring.IsEmpty()) {
            TelemetrySample s{};
            if (ring.Pop(s)) {
                consumer_b_frames.push_back(s.frame_count);
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer_a.join();
    consumer_b.join();

    std::cout << "  Consumer A read: " << consumer_a_frames.size() << " frames" << std::endl;
    std::cout << "  Consumer B read: " << consumer_b_frames.size() << " frames" << std::endl;
    uint64_t total_popped = consumer_a_frames.size() + consumer_b_frames.size();
    std::cout << "  Total frames popped by both: " << total_popped << " (Produced: " << TOTAL_ITEMS << ")" << std::endl;

    // Check for duplicate frames consumed by both threads due to lack of CAS
    std::set<uint64_t> set_a(consumer_a_frames.begin(), consumer_a_frames.end());
    uint64_t duplicates = 0;
    for (uint64_t f : consumer_b_frames) {
        if (set_a.count(f)) {
            duplicates++;
        }
    }
    std::cout << "  Detected duplicate frames read by both consumers: " << duplicates << std::endl;

    bool contention_observed = (duplicates > 0) || (total_popped != TOTAL_ITEMS);
    if (contention_observed) {
        std::cout << "  CONFIRMED ARCHITECTURAL LAW: LockFreeSpscRing without CAS cannot support multi-consumer!" << std::endl;
        std::cout << "  This validates the companion architecture's rule: Dedicated point-to-point rings are MANDATORY." << std::endl;
    } else {
        std::cout << "  Note: Contention was not caught in this specific run." << std::endl;
    }

    return contention_observed;
}

// Verification Test 3: Dedicated Point-to-Point Rings Architecture Isolation
// Verifies that dual dedicated rings (/uhai_telemetry_execd and /uhai_telemetry_gateway)
// achieve 100% throughput with 0 duplicates, 0 drops, and complete isolation!
bool VerifyDedicatedRingsIsolation() {
    std::cout << "\n[TEST 3] Architecture Solution: Dedicated Point-to-Point Rings Isolation..." << std::endl;
    std::vector<uint8_t> mem_execd(TOTAL_SHM_SIZE + 64, 0);
    std::vector<uint8_t> mem_gateway(TOTAL_SHM_SIZE + 64, 0);

    size_t space1 = mem_execd.size();
    void* raw1 = mem_execd.data();
    void* aligned1 = std::align(64, TOTAL_SHM_SIZE, raw1, space1);

    size_t space2 = mem_gateway.size();
    void* raw2 = mem_gateway.data();
    void* aligned2 = std::align(64, TOTAL_SHM_SIZE, raw2, space2);

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring_execd(aligned1);
    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring_gateway(aligned2);

    constexpr uint64_t TOTAL_ITEMS = 100'000;
    std::atomic<bool> producer_done{false};
    std::vector<uint64_t> execd_frames;
    std::vector<uint64_t> gateway_frames;
    execd_frames.reserve(TOTAL_ITEMS);
    gateway_frames.reserve(TOTAL_ITEMS);

    // Producer distributes to both dedicated rings
    std::thread producer([&]() {
        for (uint64_t i = 1; i <= TOTAL_ITEMS; ++i) {
            TelemetrySample s{};
            s.frame_count = i;
            s.quality_flags = QUALITY_HARDWARE_ROOT;

            while (!ring_execd.Push(s)) {
                std::this_thread::yield();
            }
            while (!ring_gateway.Push(s)) {
                std::this_thread::yield();
            }
        }
        producer_done.store(true, std::memory_order_release);
    });

    // col-execd consumer (reading /uhai_telemetry_execd exclusively)
    std::thread execd_consumer([&]() {
        while (!producer_done.load(std::memory_order_acquire) || !ring_execd.IsEmpty()) {
            TelemetrySample s{};
            if (ring_execd.Pop(s)) {
                execd_frames.push_back(s.frame_count);
            } else {
                std::this_thread::yield();
            }
        }
    });

    // col-gatewayd consumer (reading /uhai_telemetry_gateway exclusively)
    std::thread gateway_consumer([&]() {
        while (!producer_done.load(std::memory_order_acquire) || !ring_gateway.IsEmpty()) {
            TelemetrySample s{};
            if (ring_gateway.Pop(s)) {
                gateway_frames.push_back(s.frame_count);
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    execd_consumer.join();
    gateway_consumer.join();

    std::cout << "  col-execd received: " << execd_frames.size() << " / " << TOTAL_ITEMS << std::endl;
    std::cout << "  col-gatewayd received: " << gateway_frames.size() << " / " << TOTAL_ITEMS << std::endl;

    assert(execd_frames.size() == TOTAL_ITEMS);
    assert(gateway_frames.size() == TOTAL_ITEMS);

    for (uint64_t i = 0; i < TOTAL_ITEMS; ++i) {
        assert(execd_frames[i] == i + 1);
        assert(gateway_frames[i] == i + 1);
    }

    std::cout << "  PASS: Dedicated rings achieve complete isolation, 100% delivery, and zero cursor contention!" << std::endl;
    return true;
}

int main() {
    std::cout << "===========================================================" << std::endl;
    std::cout << "   UHAI SPSC RING BUFFER ISOLATION ADVERSARIAL HARNESS     " << std::endl;
    std::cout << "===========================================================" << std::endl;

    bool t1 = VerifySingleProducerSingleConsumer();
    bool t2 = VerifyMultiConsumerContention();
    bool t3 = VerifyDedicatedRingsIsolation();

    std::cout << "\n===========================================================" << std::endl;
    std::cout << "Summary: T1(SPSC 1:1)=" << (t1 ? "PASS" : "FAIL")
              << " T2(Contention Proven)=" << (t2 ? "PASS" : "FAIL")
              << " T3(Dedicated Isolation)=" << (t3 ? "PASS" : "FAIL") << std::endl;
    std::cout << "===========================================================" << std::endl;

    return (t1 && t2 && t3) ? 0 : 1;
}
