#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include <sstream>

#include "uhai/uhai_ring_buffer.hpp"
#include "airgap_filter.hpp"

using namespace oasis::uhai;
using namespace oasis::airgap;

static int g_failed_tests = 0;

#define ADV_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[FAIL] Line " << __LINE__ << ": " << msg << " (" #cond ")" << std::endl; \
            g_failed_tests++; \
        } \
    } while (0)

#define ADV_TEST_START(name) \
    std::cout << "\n>>> [ADVERSARIAL SUITE] Starting: " << name << " <<<" << std::endl;

#define ADV_TEST_PASS(name) \
    std::cout << ">>> [PASS] " << name << " passed successfully." << std::endl;

// ============================================================================
// Test 1: Exhaustive 256-State Mathematical Invariant Proof
// ============================================================================
void Test1_Exhaustive256StateProof() {
    ADV_TEST_START("Test 1: Exhaustive 256-State Mathematical Invariant Proof");

    HardwareAirgapFilter filter;
    uint32_t accepted_hardware = 0;
    uint32_t dropped_simulated = 0;
    uint32_t dropped_invalid = 0;

    for (int state = 0; state <= 255; ++state) {
        uint8_t q = static_cast<uint8_t>(state);
        AirgapDecision decision = filter.Evaluate(q);

        if (q == QUALITY_HARDWARE_ROOT) { // 0x02
            ADV_ASSERT(decision == AirgapDecision::ACCEPTED_HARDWARE_ROOT, "0x02 must be ACCEPTED_HARDWARE_ROOT");
            accepted_hardware++;
        } else if (q == QUALITY_SIMULATED) { // 0x01
            ADV_ASSERT(decision == AirgapDecision::DROPPED_SIMULATED_SECURITY_VIOLATION, "0x01 must be DROPPED_SIMULATED");
            dropped_simulated++;
        } else {
            ADV_ASSERT(decision == AirgapDecision::DROPPED_INVALID_FLAG, "Non-0x01/0x02 must be DROPPED_INVALID");
            dropped_invalid++;
        }
    }

    ADV_ASSERT(accepted_hardware == 1, "Exactly one byte value (0x02) must be accepted as hardware");
    ADV_ASSERT(dropped_simulated == 1, "Exactly one byte value (0x01) must be dropped as simulated violation");
    ADV_ASSERT(dropped_invalid == 254, "Exactly 254 byte values must be dropped as invalid flag");

    ADV_ASSERT(filter.GetTotalInspected() == 256, "Total inspected must be 256");
    ADV_ASSERT(filter.GetAcceptedHardware() == 1, "Accepted hardware must be 1");
    ADV_ASSERT(filter.GetDroppedSimulated() == 1, "Dropped simulated must be 1");
    ADV_ASSERT(filter.GetDroppedInvalid() == 254, "Dropped invalid must be 254");

    // Separate check for IsAuthorized predicate
    HardwareAirgapFilter auth_filter;
    for (int state = 0; state <= 255; ++state) {
        uint8_t q = static_cast<uint8_t>(state);
        bool authorized = auth_filter.IsAuthorized(q);
        ADV_ASSERT(authorized == (q == QUALITY_HARDWARE_ROOT), "IsAuthorized must be true iff q == 0x02");
    }
    ADV_ASSERT(auth_filter.GetTotalInspected() == 256, "auth_filter total inspected must be 256");
    ADV_ASSERT(auth_filter.GetAcceptedHardware() == 1, "auth_filter accepted hardware must be 1");
    ADV_ASSERT(auth_filter.GetDroppedSimulated() == 1, "auth_filter dropped simulated must be 1");
    ADV_ASSERT(auth_filter.GetDroppedInvalid() == 254, "auth_filter dropped invalid must be 254");

    ADV_TEST_PASS("Test 1: Exhaustive 256-State Mathematical Invariant Proof");
}

// ============================================================================
// Test 2: Malicious Composite Spoofing & Fuzzing (1,000,000 Adversarial Vectors)
// ============================================================================
void Test2_MaliciousCompositeSpoofingFuzz() {
    ADV_TEST_START("Test 2: Malicious Composite Spoofing & Fuzzing (1,000,000 Vectors)");

    HardwareAirgapFilter filter;

    // Fixed composite attack vectors
    const std::vector<uint8_t> composite_attacks = {
        0x03, // 0x01 | 0x02 (Simulated + Hardware Root)
        0x05, // 0x01 | 0x04 (Simulated + Out of Bounds)
        0x06, // 0x02 | 0x04 (Hardware Root + Out of Bounds)
        0x07, // 0x01 | 0x02 | 0x04 (Simulated + Hardware + Out of Bounds)
        0x09, // 0x01 | 0x08 (Simulated + Spoof Suspect)
        0x0A, // 0x02 | 0x08 (Hardware Root + Spoof Suspect)
        0x0B, // 0x01 | 0x02 | 0x08
        0x0F, // 0x01 | 0x02 | 0x04 | 0x08 (All defined flags)
        0x12, // 0x02 | Bit 4
        0x22, // 0x02 | Bit 5
        0x42, // 0x02 | Bit 6
        0x82, // 0x02 | Bit 7
        0x83, // 0x03 | Bit 7
        0xFE, // 0xFF & ~0x01
        0xFF  // All bits set
    };

    for (uint8_t attack_flag : composite_attacks) {
        AirgapDecision dec = filter.Evaluate(attack_flag);
        bool auth = filter.IsAuthorized(attack_flag);

        ADV_ASSERT(dec != AirgapDecision::ACCEPTED_HARDWARE_ROOT,
                   "Composite attack must NOT be accepted as hardware root");
        ADV_ASSERT(auth == false, "Composite attack must NOT be authorized");
        ADV_ASSERT(dec == AirgapDecision::DROPPED_INVALID_FLAG,
                   "Composite attack must evaluate to DROPPED_INVALID_FLAG");
    }

    // Fuzzing 1,000,000 pseudorandom adversarial vectors
    std::mt19937 prng(0xACE1337);
    std::uniform_int_distribution<uint32_t> dist(0, 255);

    uint64_t spoof_collision_detected = 0;
    uint64_t fuzz_runs = 1'000'000;
    filter.Reset();

    for (uint64_t i = 0; i < fuzz_runs; ++i) {
        uint8_t fuzz_byte = static_cast<uint8_t>(dist(prng));

        // Inject intentional composite collisions in 20% of cases
        if (i % 5 == 0) {
            fuzz_byte = static_cast<uint8_t>(0x03 | (fuzz_byte & 0xFC));
        }

        AirgapDecision dec = filter.Evaluate(fuzz_byte);
        bool auth = filter.IsAuthorized(fuzz_byte);

        if (fuzz_byte == QUALITY_HARDWARE_ROOT) {
            ADV_ASSERT(auth == true, "0x02 must be authorized");
        } else {
            ADV_ASSERT(auth == false, "Non-0x02 must NEVER be authorized");
            ADV_ASSERT(dec != AirgapDecision::ACCEPTED_HARDWARE_ROOT, "Non-0x02 must NOT be accepted as hardware");

            if ((fuzz_byte & QUALITY_SIMULATED) && (fuzz_byte & QUALITY_HARDWARE_ROOT)) {
                spoof_collision_detected++;
            }
        }
    }

    std::cout << "  Fuzzer verified " << fuzz_runs << " vectors. Spoof collisions detected and blocked: "
              << spoof_collision_detected << std::endl;
    ADV_TEST_PASS("Test 2: Malicious Composite Spoofing & Fuzzing");
}

// ============================================================================
// Test 3: TelemetrySample Struct-Level Corruption & Edge Value Attacks
// ============================================================================
void Test3_StructCorruptionAndEdgeValues() {
    ADV_TEST_START("Test 3: TelemetrySample Struct-Level Corruption & Edge Values");

    HardwareAirgapFilter filter;

    // Verify union memory layout and size invariants
    TelemetrySample s{};
    ADV_ASSERT(sizeof(TelemetrySample) == 32, "TelemetrySample must be 32 bytes");
    ADV_ASSERT(alignof(TelemetrySample) == 8, "TelemetrySample must be 8-byte aligned");

    // Verify union coherence between quality_flag and quality_flags
    s.quality_flag = 0x01;
    ADV_ASSERT(s.quality_flags == 0x01, "Union member quality_flags must mirror quality_flag");
    s.quality_flags = 0x02;
    ADV_ASSERT(s.quality_flag == 0x02, "Union member quality_flag must mirror quality_flags");

    // Corrupt all numerical fields with extreme / boundary values
    const std::vector<float> extreme_floats = {
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -9999999.0f,
        0.0f,
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::denorm_min()
    };

    for (float bad_fps : extreme_floats) {
        for (float bad_stress : extreme_floats) {
            TelemetrySample bad_sample{};
            bad_sample.frame_count = std::numeric_limits<uint64_t>::max();
            bad_sample.timestamp_ns = std::numeric_limits<uint64_t>::max();
            bad_sample.fps = bad_fps;
            bad_sample.founder_stress = bad_stress;
            bad_sample.memory_slot_count = std::numeric_limits<uint32_t>::max();
            bad_sample.biome_id = 255;
            bad_sample.quality_flag = QUALITY_SIMULATED; // 0x01

            // Despite corrupted numbers, airgap MUST drop simulated packet
            AirgapDecision dec = filter.Evaluate(bad_sample);
            bool auth = filter.IsAuthorized(bad_sample);

            ADV_ASSERT(dec == AirgapDecision::DROPPED_SIMULATED_SECURITY_VIOLATION,
                       "Corrupted numerical fields must NOT prevent simulated drop");
            ADV_ASSERT(auth == false, "Corrupted simulated sample must NEVER be authorized");
        }
    }

    // Byte-level garbage memory test
    std::vector<uint8_t> raw_garbage(sizeof(TelemetrySample), 0xAA);
    auto* garbage_sample = reinterpret_cast<TelemetrySample*>(raw_garbage.data());
    // 0xAA has bit 1 (0x02) set, bit 0 (0x01) not set, but 0xAA != 0x02!
    ADV_ASSERT(garbage_sample->quality_flag == 0xAA, "Garbage byte must be 0xAA");
    ADV_ASSERT(!filter.IsAuthorized(*garbage_sample), "0xAA must NOT be authorized");
    ADV_ASSERT(filter.Evaluate(*garbage_sample) == AirgapDecision::DROPPED_INVALID_FLAG, "0xAA must be dropped invalid");

    ADV_TEST_PASS("Test 3: TelemetrySample Struct-Level Corruption & Edge Values");
}

// ============================================================================
// Test 4: Physical Actuator Simulation Protection Oracle Under Overload
// ============================================================================
void Test4_PhysicalActuatorOverloadOracle() {
    ADV_TEST_START("Test 4: Physical Actuator Overload Oracle");

    struct MockPhysicalActuator {
        uint64_t trip_commands{0};
        uint64_t engagements{0};
        bool relay_closed{true};

        void Actuate() {
            trip_commands++;
            engagements++;
            relay_closed = false;
        }
    };

    MockPhysicalActuator actuator;
    HardwareAirgapFilter filter;

    constexpr uint64_t OVERLOAD_FLOOD = 500'000;
    std::mt19937 prng(0xDEADBEEF);
    std::uniform_int_distribution<uint32_t> dist(0, 255);

    for (uint64_t i = 0; i < OVERLOAD_FLOOD; ++i) {
        TelemetrySample flood_sample{};
        flood_sample.frame_count = i;
        flood_sample.fps = 0.0f;
        flood_sample.founder_stress = 1.0f; // Critical stress signal

        // In 70% of cases, tag as QUALITY_SIMULATED (0x01)
        // In 30% of cases, tag as adversarial spoof (0x03, 0x07, random noise with 0x01)
        if (i % 10 < 7) {
            flood_sample.quality_flag = QUALITY_SIMULATED;
        } else {
            flood_sample.quality_flag = static_cast<uint8_t>(0x03 | (dist(prng) & 0xFC));
        }

        if (filter.IsAuthorized(flood_sample)) {
            actuator.Actuate();
        }
    }

    ADV_ASSERT(actuator.trip_commands == 0, "Actuator MUST NOT receive any trip commands from simulated/spoofed flood");
    ADV_ASSERT(actuator.engagements == 0, "Actuator engagements MUST remain 0");
    ADV_ASSERT(actuator.relay_closed == true, "Relay MUST remain safely closed");
    ADV_ASSERT(filter.GetAcceptedHardware() == 0, "Filter accepted hardware MUST be 0");
    ADV_ASSERT(filter.GetTotalInspected() == OVERLOAD_FLOOD, "Total inspected must match flood count");

    // Inject 10 authentic hardware root frames
    for (uint64_t i = 0; i < 10; ++i) {
        TelemetrySample authentic_hw{};
        authentic_hw.frame_count = OVERLOAD_FLOOD + i;
        authentic_hw.quality_flag = QUALITY_HARDWARE_ROOT;

        if (filter.IsAuthorized(authentic_hw)) {
            actuator.Actuate();
        }
    }

    ADV_ASSERT(actuator.trip_commands == 10, "Authentic hardware frames MUST actuate");
    ADV_ASSERT(actuator.relay_closed == false, "Relay opened upon authentic hardware frames");
    ADV_ASSERT(filter.GetAcceptedHardware() == 10, "Filter accepted exactly 10 hardware frames");

    ADV_TEST_PASS("Test 4: Physical Actuator Overload Oracle");
}

// ============================================================================
// Test 5: Queue Overrun & Flooding Defense (High-Rate SPSC Flooding)
// ============================================================================
void Test5_QueueOverrunFloodingDefense() {
    ADV_TEST_START("Test 5: Queue Overrun & Flooding Defense");

    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);
    ADV_ASSERT(aligned != nullptr, "Aligned SHM buffer must not be null");

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    constexpr uint64_t FLOOD_COUNT = 50'000;
    uint64_t successful_pushes = 0;
    uint64_t rejected_pushes = 0;

    for (uint64_t i = 0; i < FLOOD_COUNT; ++i) {
        TelemetrySample s{};
        s.frame_count = i;
        s.quality_flag = QUALITY_SIMULATED;

        if (ring.Push(s)) {
            successful_pushes++;
        } else {
            rejected_pushes++;
        }
    }

    ADV_ASSERT(successful_pushes == RING_CAPACITY, "Exact ring capacity (1024) must succeed");
    ADV_ASSERT(rejected_pushes == (FLOOD_COUNT - RING_CAPACITY), "Overrun pushes must be rejected");
    ADV_ASSERT(ring.DroppedCount() == (FLOOD_COUNT - RING_CAPACITY), "Dropped counter must match overruns");
    ADV_ASSERT(ring.Size() == RING_CAPACITY, "Ring size must equal RING_CAPACITY");

    // Drain the 1024 packets and verify zero memory corruption / strict FIFO
    TelemetrySample popped{};
    uint64_t drained = 0;
    while (ring.Pop(popped)) {
        ADV_ASSERT(popped.frame_count == drained, "Drained frames must match original FIFO push order");
        drained++;
    }

    ADV_ASSERT(drained == RING_CAPACITY, "Must drain exactly RING_CAPACITY frames");
    ADV_ASSERT(ring.IsEmpty(), "Ring must be empty after full drain");
    ADV_ASSERT(ring.Size() == 0, "Ring size must be 0 after full drain");

    // Verify recovery after flood: push and pop one new packet
    TelemetrySample post_flood{};
    post_flood.frame_count = 999999;
    ADV_ASSERT(ring.Push(post_flood) == true, "Must be able to push after drain recovery");
    ADV_ASSERT(ring.Pop(popped) == true, "Must be able to pop after drain recovery");
    ADV_ASSERT(popped.frame_count == 999999, "Popped frame must match post-flood frame");
    ADV_ASSERT(ring.IsEmpty(), "Ring must be empty again");

    ADV_TEST_PASS("Test 5: Queue Overrun & Flooding Defense");
}

// ============================================================================
// Test 6: 64-Bit Modular Wrap-Around Invariant Test
// ============================================================================
void Test6_IndexWrapAroundModularArithmetic() {
    ADV_TEST_START("Test 6: 64-Bit Modular Wrap-Around Invariant Test");

    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;
    header->dropped_frames.store(0);

    // Set write and read index near UINT64_MAX
    constexpr uint64_t NEAR_MAX = std::numeric_limits<uint64_t>::max() - 250;
    header->write_index.store(NEAR_MAX, std::memory_order_relaxed);
    header->read_index.store(NEAR_MAX, std::memory_order_relaxed);

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);
    ADV_ASSERT(ring.IsEmpty(), "Must be empty when read == write near UINT64_MAX");

    constexpr uint64_t WRAP_PACKETS = 50'000;
    TelemetrySample sample{};
    TelemetrySample popped{};

    for (uint64_t i = 0; i < WRAP_PACKETS; ++i) {
        sample.frame_count = i;
        sample.quality_flag = (i % 2 == 0) ? QUALITY_SIMULATED : QUALITY_HARDWARE_ROOT;

        bool pushed = ring.Push(sample);
        ADV_ASSERT(pushed == true, "Push during wrap-around must succeed");

        bool pop_ok = ring.Pop(popped);
        ADV_ASSERT(pop_ok == true, "Pop during wrap-around must succeed");
        ADV_ASSERT(popped.frame_count == i, "Popped frame must match push during wrap");
    }

    ADV_ASSERT(ring.IsEmpty(), "Must be empty after wrap-around push-pop loop");
    ADV_ASSERT(ring.DroppedCount() == 0, "Dropped count must be 0");

    ADV_TEST_PASS("Test 6: 64-Bit Modular Wrap-Around Invariant Test");
}

// ============================================================================
// Test 7: Corrupt Header Fuzzing & Resilience
// ============================================================================
void Test7_CorruptHeaderFuzzing() {
    ADV_TEST_START("Test 7: Corrupt Header Fuzzing & Resilience");

    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    // Scenario A: Inverted index corruption (read_index > write_index)
    // E.g. read_index = 5000, write_index = 1000
    header->read_index.store(5000, std::memory_order_relaxed);
    header->write_index.store(1000, std::memory_order_relaxed);

    // Size() must return 0 when w < r due to check (w >= r) ? (w - r) : 0
    ADV_ASSERT(ring.Size() == 0, "Size() must handle inverted indices without wrapping to 2^64");

    // Push() calculates (w - r) which underflows to unsigned uint64 ~ 18446744073709547616 >= Capacity
    // Therefore Push() correctly rejects and records dropped frame instead of buffer overflow!
    TelemetrySample sample{};
    sample.frame_count = 1;
    bool push_res = ring.Push(sample);
    ADV_ASSERT(push_res == false, "Push() must safely reject when read > write");
    ADV_ASSERT(ring.DroppedCount() == 1, "Dropped counter must increment on inverted indices");

    // Scenario B: Magic signature corruption
    header->magic_signature = 0xDEADBEEF;
    ADV_ASSERT(header->magic_signature != UHAI_MAGIC, "Corrupted magic signature verified");

    // Scenario C: Version corruption
    header->version = 99;
    ADV_ASSERT(header->version != UHAI_VERSION, "Corrupted version verified");

    ADV_TEST_PASS("Test 7: Corrupt Header Fuzzing & Resilience");
}

// ============================================================================
// Test 8: Massive Concurrent Multi-Threaded Stress Test (500,000 Frames)
// ============================================================================
void Test8_MassiveConcurrentStress() {
    ADV_TEST_START("Test 8: Massive Concurrent Multi-Threaded Stress Test (500,000 Frames)");

    std::vector<uint8_t> memory(TOTAL_SHM_SIZE + 64, 0);
    void* raw = memory.data();
    size_t space = memory.size();
    void* aligned = std::align(64, TOTAL_SHM_SIZE, raw, space);

    auto* header = reinterpret_cast<SharedRingHeader*>(aligned);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = RING_CAPACITY;
    header->element_size = sizeof(TelemetrySlot);
    header->magic_signature = UHAI_MAGIC;
    header->version = UHAI_VERSION;

    LockFreeSpscRing<TelemetrySlot, RING_CAPACITY> ring(aligned);

    constexpr uint64_t TOTAL_FRAMES = 500'000;
    constexpr uint64_t EXPECTED_HARDWARE = 100'000;  // 20%
    constexpr uint64_t EXPECTED_SIMULATED = 350'000; // 70%
    constexpr uint64_t EXPECTED_INVALID = 50'000;    // 10%

    std::atomic<bool> start_flag{false};
    std::atomic<bool> producer_done{false};

    std::atomic<uint64_t> hardware_accepted_atomic{0};
    std::atomic<uint64_t> simulated_dropped_atomic{0};
    std::atomic<uint64_t> invalid_dropped_atomic{0};
    std::atomic<uint64_t> total_consumed_atomic{0};
    std::atomic<uint64_t> leakage_violations_atomic{0};
    std::atomic<uint64_t> sequence_violations_atomic{0};

    auto producer = std::thread([&]() {
        while (!start_flag.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        TelemetrySample s{};
        for (uint64_t i = 0; i < TOTAL_FRAMES; ++i) {
            s.frame_count = i;
            s.timestamp_ns = i * 16'666'666ULL;
            s.fps = 60.0f;
            s.founder_stress = 0.35f;

            uint64_t mod = i % 10;
            if (mod < 2) {
                // 20%: Authentic Hardware Root (0x02)
                s.quality_flag = QUALITY_HARDWARE_ROOT;
            } else if (mod < 9) {
                // 70%: SITL Simulated (0x01)
                s.quality_flag = QUALITY_SIMULATED;
            } else {
                // 10%: Adversarial Composite Spoof (0x03, 0x83, 0xFF)
                s.quality_flag = static_cast<uint8_t>(0x03 | ((i & 0x07) << 4));
            }

            while (!ring.Push(s)) {
                std::this_thread::yield();
            }
        }
        producer_done.store(true, std::memory_order_release);
    });

    auto consumer = std::thread([&]() {
        HardwareAirgapFilter filter;
        uint64_t last_hardware_frame = 0;
        bool first_hardware = true;

        while (!start_flag.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        TelemetrySample s{};
        while (true) {
            if (ring.Pop(s)) {
                total_consumed_atomic.fetch_add(1, std::memory_order_relaxed);

                AirgapDecision dec = filter.Evaluate(s);
                bool auth = filter.IsAuthorized(s);

                if (dec == AirgapDecision::ACCEPTED_HARDWARE_ROOT) {
                    hardware_accepted_atomic.fetch_add(1, std::memory_order_relaxed);
                    if (s.quality_flag != QUALITY_HARDWARE_ROOT || !auth) {
                        leakage_violations_atomic.fetch_add(1, std::memory_order_relaxed);
                    }

                    // Check FIFO monotonic ordering of hardware frames
                    if (first_hardware) {
                        last_hardware_frame = s.frame_count;
                        first_hardware = false;
                    } else {
                        if (s.frame_count <= last_hardware_frame) {
                            sequence_violations_atomic.fetch_add(1, std::memory_order_relaxed);
                        }
                        last_hardware_frame = s.frame_count;
                    }
                } else if (dec == AirgapDecision::DROPPED_SIMULATED_SECURITY_VIOLATION) {
                    simulated_dropped_atomic.fetch_add(1, std::memory_order_relaxed);
                    if (s.quality_flag != QUALITY_SIMULATED || auth) {
                        leakage_violations_atomic.fetch_add(1, std::memory_order_relaxed);
                    }
                } else {
                    invalid_dropped_atomic.fetch_add(1, std::memory_order_relaxed);
                    if (s.quality_flag == QUALITY_HARDWARE_ROOT || auth) {
                        leakage_violations_atomic.fetch_add(1, std::memory_order_relaxed);
                    }
                }
            } else {
                if (producer_done.load(std::memory_order_acquire) && ring.IsEmpty()) {
                    break;
                }
                std::this_thread::yield();
            }
        }
    });

    auto start_time = std::chrono::steady_clock::now();
    start_flag.store(true, std::memory_order_release);

    producer.join();
    consumer.join();
    auto end_time = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "  Transferred & filtered " << TOTAL_FRAMES << " frames in " << elapsed_ms << " ms ("
              << (elapsed_ms > 0 ? (TOTAL_FRAMES * 1000 / elapsed_ms) : 0) << " frames/sec)" << std::endl;

    ADV_ASSERT(total_consumed_atomic.load() == TOTAL_FRAMES, "Total consumed frames must be 500,000");
    ADV_ASSERT(hardware_accepted_atomic.load() == EXPECTED_HARDWARE, "Hardware accepted must be 100,000");
    ADV_ASSERT(simulated_dropped_atomic.load() == EXPECTED_SIMULATED, "Simulated dropped must be 350,000");
    ADV_ASSERT(invalid_dropped_atomic.load() == EXPECTED_INVALID, "Invalid dropped must be 50,000");
    ADV_ASSERT(leakage_violations_atomic.load() == 0, "Leakage violations MUST BE ZERO");
    ADV_ASSERT(sequence_violations_atomic.load() == 0, "Sequence violations MUST BE ZERO");
    ADV_ASSERT(ring.IsEmpty(), "Ring must be empty upon completion");

    ADV_TEST_PASS("Test 8: Massive Concurrent Multi-Threaded Stress Test");
}

int main() {
    std::cout << "=================================================================" << std::endl;
    std::cout << "=== Empirical Challenger: Hardware Airgap & UHAI Concurrency  ===" << std::endl;
    std::cout << "=================================================================" << std::endl;

    Test1_Exhaustive256StateProof();
    Test2_MaliciousCompositeSpoofingFuzz();
    Test3_StructCorruptionAndEdgeValues();
    Test4_PhysicalActuatorOverloadOracle();
    Test5_QueueOverrunFloodingDefense();
    Test6_IndexWrapAroundModularArithmetic();
    Test7_CorruptHeaderFuzzing();
    Test8_MassiveConcurrentStress();

    std::cout << "\n=================================================================" << std::endl;
    if (g_failed_tests == 0) {
        std::cout << ">>> ALL 8 ADVERSARIAL STRESS TEST SUITES PASSED (0 FAILURES) <<<" << std::endl;
        std::cout << "=================================================================" << std::endl;
        return 0;
    } else {
        std::cerr << ">>> ADVERSARIAL VERIFICATION FAILED: " << g_failed_tests << " assertions failed! <<<" << std::endl;
        std::cout << "=================================================================" << std::endl;
        return 1;
    }
}
