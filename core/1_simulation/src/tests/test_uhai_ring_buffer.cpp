#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <cstdint>
#include <cstddef>
#include <unistd.h>
#include <spdlog/spdlog.h>

#include "uhai/uhai_ring_buffer.hpp"

using namespace oasis::uhai;

TEST_CASE("UHAI Memory Layout and Cacheline Isolation", "[layout]") {
    SECTION("Struct sizes and alignments") {
        REQUIRE(sizeof(SharedRingHeader) == 192);
        REQUIRE(alignof(SharedRingHeader) == 64);

        REQUIRE(sizeof(TelemetrySample) == 32);
        REQUIRE(alignof(TelemetrySample) == 8);

        REQUIRE(sizeof(TelemetrySlot) == 64);
        REQUIRE(alignof(TelemetrySlot) == 64);

        REQUIRE(TOTAL_SHM_SIZE == 65728);
    }

    SECTION("SharedRingHeader 3-cacheline isolation") {
        // Cache Line 0 (offset 0..63): Producer write state & dropped frames
        REQUIRE(offsetof(SharedRingHeader, write_index) == 0);
        REQUIRE(offsetof(SharedRingHeader, dropped_frames) == 8);
        REQUIRE(offsetof(SharedRingHeader, write_index) / 64 == 0);
        REQUIRE(offsetof(SharedRingHeader, dropped_frames) / 64 == 0);

        // Cache Line 1 (offset 64..127): Consumer read state
        REQUIRE(offsetof(SharedRingHeader, read_index) == 64);
        REQUIRE(offsetof(SharedRingHeader, read_index) / 64 == 1);

        // Cache Line 2 (offset 128..191): Static configuration & metadata
        REQUIRE(offsetof(SharedRingHeader, capacity) == 128);
        REQUIRE(offsetof(SharedRingHeader, element_size) == 132);
        REQUIRE(offsetof(SharedRingHeader, magic_signature) == 136);
        REQUIRE(offsetof(SharedRingHeader, version) == 140);
        REQUIRE(offsetof(SharedRingHeader, capacity) / 64 == 2);
        REQUIRE(offsetof(SharedRingHeader, element_size) / 64 == 2);
        REQUIRE(offsetof(SharedRingHeader, magic_signature) / 64 == 2);
        REQUIRE(offsetof(SharedRingHeader, version) / 64 == 2);
    }

    SECTION("Payload slot cacheline isolation") {
        size_t buffer_start = sizeof(SharedRingHeader);
        REQUIRE(buffer_start == 192);
        REQUIRE(buffer_start % 64 == 0);

        size_t slot0_offset = buffer_start;
        size_t slot1_offset = buffer_start + sizeof(TelemetrySlot);
        size_t slot2_offset = buffer_start + 2 * sizeof(TelemetrySlot);

        REQUIRE(slot0_offset / 64 == 3);
        REQUIRE(slot1_offset / 64 == 4);
        REQUIRE(slot2_offset / 64 == 5);

        // Adjacent slots occupy distinct 64-byte cache lines
        REQUIRE(slot0_offset / 64 != slot1_offset / 64);
        REQUIRE(slot1_offset / 64 != slot2_offset / 64);
    }
}

TEST_CASE("UHAI SPSC FIFO Ordering and Data Integrity", "[fifo]") {
    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    REQUIRE(aligned != nullptr);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);
    REQUIRE(ring.IsEmpty());

    constexpr uint64_t PUSH_COUNT = 500;
    for (uint64_t i = 0; i < PUSH_COUNT; ++i) {
        TelemetrySample s{};
        s.frame_count = i;
        s.timestamp_ns = 1'000'000'000ULL + i;
        s.fps = 60.0f + static_cast<float>(i % 10);
        s.founder_stress = 0.1f * static_cast<float>(i % 5);
        s.memory_slot_count = static_cast<uint32_t>(i % 32);
        s.biome_id = static_cast<uint8_t>(i % 3);
        s.quality_flags = 0x01;
        s.reserved = 0;

        REQUIRE(ring.Push(s) == true);
    }

    REQUIRE(ring.Size() == PUSH_COUNT);
    REQUIRE(!ring.IsEmpty());

    for (uint64_t i = 0; i < PUSH_COUNT; ++i) {
        TelemetrySample s{};
        REQUIRE(ring.Pop(s) == true);
        REQUIRE(s.frame_count == i);
        REQUIRE(s.timestamp_ns == 1'000'000'000ULL + i);
        REQUIRE(s.fps == 60.0f + static_cast<float>(i % 10));
        REQUIRE(s.memory_slot_count == (i % 32));
        REQUIRE(s.biome_id == (i % 3));
        REQUIRE(s.quality_flags == 0x01);
    }

    REQUIRE(ring.IsEmpty());
    TelemetrySample extra{};
    REQUIRE(ring.Pop(extra) == false);

    // Test power-of-two index wraparound beyond capacity
    for (int cycle = 0; cycle < 3; ++cycle) {
        for (uint64_t i = 0; i < 700; ++i) {
            TelemetrySample s{};
            s.frame_count = cycle * 1000 + i;
            REQUIRE(ring.Push(s) == true);
        }
        for (uint64_t i = 0; i < 700; ++i) {
            TelemetrySample s{};
            REQUIRE(ring.Pop(s) == true);
            REQUIRE(s.frame_count == cycle * 1000 + i);
        }
        REQUIRE(ring.IsEmpty());
    }
}

TEST_CASE("UHAI SPSC Multi-Threaded Concurrency (1,000,000 frames)", "[concurrency]") {
    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    REQUIRE(aligned != nullptr);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    constexpr uint64_t TOTAL_FRAMES = 1'000'000;
    std::atomic<bool> start_flag{false};

    auto producer = [&]() {
        while (!start_flag.load(std::memory_order_acquire)) {}
        TelemetrySample s{};
        for (uint64_t i = 0; i < TOTAL_FRAMES; ++i) {
            s.frame_count = i;
            s.timestamp_ns = i * 1000;
            s.fps = 60.0f;
            s.founder_stress = 0.5f;
            s.memory_slot_count = static_cast<uint32_t>(i % 32);
            s.biome_id = 0;
            s.quality_flags = 1;
            s.reserved = 0;

            while (!ring.Push(s)) {
                std::this_thread::yield();
            }
        }
    };

    uint64_t popped_count = 0;
    uint64_t underrun_yields = 0;
    bool corruption_detected = false;

    auto consumer = [&]() {
        while (!start_flag.load(std::memory_order_acquire)) {}
        TelemetrySample s{};
        while (popped_count < TOTAL_FRAMES) {
            if (ring.Pop(s)) {
                if (s.frame_count != popped_count || s.timestamp_ns != popped_count * 1000) {
                    corruption_detected = true;
                    break;
                }
                popped_count++;
            } else {
                underrun_yields++;
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

    spdlog::info("[Catch2 Concurrency] Transferred {} frames in {:.2f} ms ({:.2f} ns/frame, underruns: {}, overrun attempts: {})",
                 TOTAL_FRAMES, elapsed_ms, per_frame_ns, underrun_yields, ring.DroppedCount());

    REQUIRE_FALSE(corruption_detected);
    REQUIRE(popped_count == TOTAL_FRAMES);
    REQUIRE(ring.IsEmpty());
}

TEST_CASE("UHAI Buffer Overrun Accounting and Dropped Frames", "[overrun]") {
    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    REQUIRE(aligned != nullptr);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    // Consumer is paused. Producer attempts 1500 pushes into 1024-capacity buffer.
    uint64_t accepted = 0;
    uint64_t rejected = 0;

    for (uint64_t i = 0; i < 1500; ++i) {
        TelemetrySample s{};
        s.frame_count = i;
        if (ring.Push(s)) {
            accepted++;
        } else {
            rejected++;
        }
    }

    REQUIRE(accepted == 1024);
    REQUIRE(rejected == 476);
    REQUIRE(ring.DroppedCount() == 476);
    REQUIRE(ring.Size() == 1024);

    // Verify all 1024 accepted frames can be popped in order
    for (uint64_t i = 0; i < 1024; ++i) {
        TelemetrySample s{};
        REQUIRE(ring.Pop(s) == true);
        REQUIRE(s.frame_count == i);
    }

    REQUIRE(ring.IsEmpty());
    TelemetrySample extra{};
    REQUIRE(ring.Pop(extra) == false);

    // Buffer now has space again; verify normal push succeeds
    TelemetrySample new_sample{};
    new_sample.frame_count = 9999;
    REQUIRE(ring.Push(new_sample) == true);
    REQUIRE(ring.Pop(extra) == true);
    REQUIRE(extra.frame_count == 9999);
}

TEST_CASE("UHAI Buffer Underrun Handling", "[underrun]") {
    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    REQUIRE(aligned != nullptr);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    REQUIRE(ring.IsEmpty());

    TelemetrySample sample{};
    // Pop on empty buffer returns false gracefully
    REQUIRE(ring.Pop(sample) == false);
    REQUIRE(ring.pop() == std::nullopt);
    REQUIRE(ring.DroppedCount() == 0);
    REQUIRE(ring.Size() == 0);

    // Indexes should be undisturbed
    REQUIRE(header->write_index.load() == 0);
    REQUIRE(header->read_index.load() == 0);

    // Push 1 item, pop 1 item
    sample.frame_count = 42;
    REQUIRE(ring.Push(sample) == true);
    REQUIRE_FALSE(ring.IsEmpty());
    REQUIRE(ring.Size() == 1);

    TelemetrySample out{};
    REQUIRE(ring.Pop(out) == true);
    REQUIRE(out.frame_count == 42);
    REQUIRE(ring.IsEmpty());

    // Underrun again
    REQUIRE(ring.Pop(out) == false);
}

TEST_CASE("UHAI POSIX Shared Memory Attachment and Channel IPC", "[shm][channel]") {
    std::string test_shm_name = "/test_uhai_ch_" + std::to_string(getpid());
    UhaiTelemetryChannel::Unlink(test_shm_name);

    {
        // Producer channel
        UhaiTelemetryChannel producer(UhaiTelemetryChannel::ChannelMode::Producer, test_shm_name);
        REQUIRE(producer.IsValid());
        REQUIRE(producer.GetHeader() != nullptr);
        REQUIRE(producer.GetHeader()->magic_signature == UHAI_MAGIC);
        REQUIRE(producer.GetHeader()->version == UHAI_VERSION);
        REQUIRE(producer.GetHeader()->capacity == RING_CAPACITY);

        // Consumer channel attaching to same shm
        UhaiTelemetryChannel consumer(UhaiTelemetryChannel::ChannelMode::Consumer, test_shm_name);
        REQUIRE(consumer.IsValid());
        REQUIRE(consumer.GetHeader() != nullptr);
        REQUIRE(consumer.GetHeader()->magic_signature == UHAI_MAGIC);

        // Push 10 samples from producer
        for (uint64_t i = 1; i <= 10; ++i) {
            TelemetrySample s{};
            s.frame_count = i;
            s.fps = 60.0f;
            REQUIRE(producer.Push(s) == true);
        }

        // Pop 10 samples from consumer
        for (uint64_t i = 1; i <= 10; ++i) {
            TelemetrySample s{};
            REQUIRE(consumer.Pop(s) == true);
            REQUIRE(s.frame_count == i);
            REQUIRE(s.fps == 60.0f);
        }

        // Ensure consumer ring is now empty
        TelemetrySample extra{};
        REQUIRE(consumer.Pop(extra) == false);
    }

    UhaiTelemetryChannel::Unlink(test_shm_name);
}
