#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <spdlog/spdlog.h>

#include "uhai/uhai_ring_buffer.hpp"
#include "airgap_filter.hpp"

using namespace oasis::uhai;
using namespace oasis::airgap;

TEST_CASE("Test 1: Constant definitions & bitwise orthogonality", "[airgap][constants]") {
    SECTION("Quality flag constant values and definitions") {
        REQUIRE(QUALITY_SIMULATED == 0x01);
        REQUIRE(QualityFlags::QUALITY_SIMULATED == 0x01);

        REQUIRE(QUALITY_HARDWARE_ROOT == 0x02);
        REQUIRE(QualityFlags::QUALITY_HARDWARE_ROOT == 0x02);

        REQUIRE(QualityFlags::QUALITY_OUT_OF_BOUNDS == 0x04);
        REQUIRE(QualityFlags::QUALITY_SPOOF_SUSPECT == 0x08);
    }

    SECTION("Bitwise orthogonality across distinct bit planes") {
        // Strict bitwise orthogonality between simulated SITL and hardware root
        REQUIRE((QUALITY_SIMULATED & QUALITY_HARDWARE_ROOT) == 0);
        REQUIRE((0x01 & 0x02) == 0);

        // Orthogonality across all defined quality flag bits
        REQUIRE((QUALITY_SIMULATED & QualityFlags::QUALITY_OUT_OF_BOUNDS) == 0);
        REQUIRE((QUALITY_SIMULATED & QualityFlags::QUALITY_SPOOF_SUSPECT) == 0);
        REQUIRE((QUALITY_HARDWARE_ROOT & QualityFlags::QUALITY_OUT_OF_BOUNDS) == 0);
        REQUIRE((QUALITY_HARDWARE_ROOT & QualityFlags::QUALITY_SPOOF_SUSPECT) == 0);
        REQUIRE((QualityFlags::QUALITY_OUT_OF_BOUNDS & QualityFlags::QUALITY_SPOOF_SUSPECT) == 0);

        // Single-bit power-of-two invariants
        REQUIRE((QUALITY_SIMULATED & (QUALITY_SIMULATED - 1)) == 0);
        REQUIRE((QUALITY_HARDWARE_ROOT & (QUALITY_HARDWARE_ROOT - 1)) == 0);
    }

    SECTION("AirgapDecision enum variants and string mappings") {
        REQUIRE(static_cast<uint8_t>(AirgapDecision::ACCEPTED_HARDWARE_ROOT) == 0);
        REQUIRE(static_cast<uint8_t>(AirgapDecision::DROPPED_SIMULATED_SECURITY_VIOLATION) == 1);
        REQUIRE(static_cast<uint8_t>(AirgapDecision::DROPPED_INVALID_FLAG) == 2);

        REQUIRE(HardwareAirgapFilter::DecisionToString(AirgapDecision::ACCEPTED_HARDWARE_ROOT) == "ACCEPTED_HARDWARE_ROOT");
        REQUIRE(HardwareAirgapFilter::DecisionToString(AirgapDecision::DROPPED_SIMULATED_SECURITY_VIOLATION) == "DROPPED_SIMULATED_SECURITY_VIOLATION");
        REQUIRE(HardwareAirgapFilter::DecisionToString(AirgapDecision::DROPPED_INVALID_FLAG) == "DROPPED_INVALID_FLAG");
    }
}

TEST_CASE("Test 2: Exhaustive 256-state mathematical rejection predicate across all 256 possible uint8_t values", "[airgap][predicate][exhaustive]") {
    HardwareAirgapFilter filter;
    uint32_t accepted_count = 0;
    uint32_t rejected_count = 0;
    uint32_t simulated_violation_count = 0;
    uint32_t invalid_flag_count = 0;

    for (int state = 0; state <= 255; ++state) {
        const uint8_t quality = static_cast<uint8_t>(state);
        const AirgapDecision decision = filter.Evaluate(quality);
        const bool authorized = (decision == AirgapDecision::ACCEPTED_HARDWARE_ROOT);

        if (quality == QUALITY_HARDWARE_ROOT) {
            REQUIRE(authorized == true);
            REQUIRE(decision == AirgapDecision::ACCEPTED_HARDWARE_ROOT);
            accepted_count++;
        } else {
            REQUIRE(authorized == false);
            REQUIRE(decision != AirgapDecision::ACCEPTED_HARDWARE_ROOT);
            rejected_count++;

            if (quality == QUALITY_SIMULATED) {
                REQUIRE(decision == AirgapDecision::DROPPED_SIMULATED_SECURITY_VIOLATION);
                simulated_violation_count++;
            } else {
                REQUIRE(decision == AirgapDecision::DROPPED_INVALID_FLAG);
                invalid_flag_count++;
            }
        }
    }

    // Mathematical predicate invariants: exactly 1 authorized, exactly 255 rejected
    REQUIRE(accepted_count == 1);
    REQUIRE(rejected_count == 255);
    REQUIRE(simulated_violation_count == 1);
    REQUIRE(invalid_flag_count == 254);

    // Verify filter internal accounting counters across the 256 evaluated states
    REQUIRE(filter.GetTotalInspected() == 256);
    REQUIRE(filter.GetAcceptedHardware() == 1);
    REQUIRE(filter.GetDroppedSimulated() == 1);
    REQUIRE(filter.GetDroppedInvalid() == 254);

    // Specific boundary cases explicitly verified via IsAuthorized
    HardwareAirgapFilter boundary_filter;
    REQUIRE_FALSE(boundary_filter.IsAuthorized(0x00)); // Uninitialized
    REQUIRE_FALSE(boundary_filter.IsAuthorized(0x01)); // Simulated
    REQUIRE(boundary_filter.IsAuthorized(0x02));       // Hardware root
    REQUIRE_FALSE(boundary_filter.IsAuthorized(0x03)); // Simulated | Hardware root collision
    REQUIRE_FALSE(boundary_filter.IsAuthorized(0xFF)); // All bits set

    // Verify identical behavior through TelemetrySample member overloads
    HardwareAirgapFilter sample_filter;
    for (int state = 0; state <= 255; ++state) {
        const uint8_t quality = static_cast<uint8_t>(state);

        TelemetrySample sample_plural{};
        sample_plural.quality_flags = quality;
        REQUIRE(sample_filter.IsAuthorized(sample_plural) == (quality == QUALITY_HARDWARE_ROOT));

        TelemetrySample sample_singular{};
        sample_singular.quality_flag = quality;
        REQUIRE(sample_filter.IsAuthorized(sample_singular) == (quality == QUALITY_HARDWARE_ROOT));
    }
    REQUIRE(sample_filter.GetAcceptedHardware() == 2);
    REQUIRE(sample_filter.GetTotalInspected() == 512);
}

TEST_CASE("Test 3: Adversarial spoofing and collision protection (testing 0x03 = 0x01 | 0x02)", "[airgap][adversarial][security]") {
    HardwareAirgapFilter filter;

    SECTION("Bitwise collision 0x03 = QUALITY_SIMULATED | QUALITY_HARDWARE_ROOT") {
        constexpr uint8_t collision_flag = QUALITY_SIMULATED | QUALITY_HARDWARE_ROOT;
        REQUIRE(collision_flag == 0x03);

        // Must NOT be authorized under any circumstances
        const AirgapDecision decision = filter.Evaluate(collision_flag);
        REQUIRE(decision == AirgapDecision::DROPPED_INVALID_FLAG);
        REQUIRE(decision != AirgapDecision::ACCEPTED_HARDWARE_ROOT);
        REQUIRE(filter.GetAcceptedHardware() == 0);
        REQUIRE(filter.GetDroppedInvalid() == 1);
        REQUIRE(filter.GetTotalInspected() == 1);

        HardwareAirgapFilter is_auth_check;
        REQUIRE_FALSE(is_auth_check.IsAuthorized(collision_flag));
        REQUIRE(is_auth_check.GetAcceptedHardware() == 0);
        REQUIRE(is_auth_check.GetDroppedInvalid() == 1);
    }

    SECTION("Composite adversarial collision vectors") {
        filter.Reset();
        // Set of vectors combining simulated bit with hardware root bit and other bits
        const std::vector<uint8_t> collision_vectors = {
            0x03,                         // 0x01 | 0x02
            0x07,                         // 0x01 | 0x02 | 0x04
            0x0B,                         // 0x01 | 0x02 | 0x08
            0x0F,                         // 0x01 | 0x02 | 0x04 | 0x08
            static_cast<uint8_t>(0x03 | 0x10),
            static_cast<uint8_t>(0x03 | 0x20),
            static_cast<uint8_t>(0x03 | 0x40),
            static_cast<uint8_t>(0x03 | 0x80),
            0xFF                          // All bits set
        };

        for (uint8_t vector : collision_vectors) {
            const AirgapDecision decision = filter.Evaluate(vector);
            REQUIRE(decision != AirgapDecision::ACCEPTED_HARDWARE_ROOT);
            REQUIRE(decision == AirgapDecision::DROPPED_INVALID_FLAG);
        }

        REQUIRE(filter.GetAcceptedHardware() == 0);
        REQUIRE(filter.GetDroppedInvalid() == collision_vectors.size());
        REQUIRE(filter.GetTotalInspected() == collision_vectors.size());
    }

    SECTION("Tracking spoofed violations across adversarial telemetry stream") {
        filter.Reset();
        uint64_t spoofed_detected = 0;
        constexpr uint32_t STREAM_SIZE = 1000;

        for (uint32_t i = 0; i < STREAM_SIZE; ++i) {
            TelemetrySample s{};
            s.frame_count = i;
            // Generate synthetic spoof with collision bits plus pseudorandom noise
            uint8_t noise = static_cast<uint8_t>((i * 17 + 31) & 0xFC);
            s.quality_flag = static_cast<uint8_t>(0x03 | noise);

            if ((s.quality_flag & QUALITY_SIMULATED) && (s.quality_flag & QUALITY_HARDWARE_ROOT)) {
                spoofed_detected++;
            }

            const AirgapDecision dec = filter.Evaluate(s);
            REQUIRE(dec != AirgapDecision::ACCEPTED_HARDWARE_ROOT);
            REQUIRE(dec == AirgapDecision::DROPPED_INVALID_FLAG);
        }

        REQUIRE(spoofed_detected == STREAM_SIZE);
        REQUIRE(filter.GetAcceptedHardware() == 0);
        REQUIRE(filter.GetDroppedInvalid() == STREAM_SIZE);
        REQUIRE(filter.GetTotalInspected() == STREAM_SIZE);
    }
}

TEST_CASE("Test 4: SPSC Ring Buffer queue drainage and packet partitioning", "[airgap][drainage][spsc]") {
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

    constexpr uint64_t TOTAL_PACKETS = 1000;
    constexpr uint64_t HALF_PACKETS = 500;

    // Push 500 simulated packets interleaved with 500 hardware root packets
    for (uint64_t i = 0; i < TOTAL_PACKETS; ++i) {
        TelemetrySample s{};
        s.frame_count = i;
        s.timestamp_ns = 1'000'000'000ULL + i * 16'666'666ULL;
        s.fps = 60.0f;
        s.founder_stress = 0.25f;
        s.memory_slot_count = static_cast<uint32_t>(i % 16);
        s.biome_id = 0;
        // Even indices: QUALITY_SIMULATED (0x01)
        // Odd indices:  QUALITY_HARDWARE_ROOT (0x02)
        s.quality_flag = (i % 2 == 0) ? QUALITY_SIMULATED : QUALITY_HARDWARE_ROOT;

        const bool pushed = ring.Push(s);
        REQUIRE(pushed == true);
    }

    REQUIRE(ring.Size() == TOTAL_PACKETS);
    REQUIRE_FALSE(ring.IsEmpty());

    // Drain queue through HardwareAirgapFilter and verify mathematical partitioning
    HardwareAirgapFilter filter;
    std::vector<TelemetrySample> accepted_hardware_packets;
    std::vector<TelemetrySample> dropped_simulated_packets;

    TelemetrySample popped{};
    while (ring.Pop(popped)) {
        if (filter.IsAuthorized(popped)) {
            accepted_hardware_packets.push_back(popped);
        } else {
            dropped_simulated_packets.push_back(popped);
        }
    }

    REQUIRE(ring.IsEmpty());
    REQUIRE(accepted_hardware_packets.size() == HALF_PACKETS);
    REQUIRE(dropped_simulated_packets.size() == HALF_PACKETS);

    // Verify filter internal statistics
    REQUIRE(filter.GetTotalInspected() == TOTAL_PACKETS);
    REQUIRE(filter.GetAcceptedHardware() == HALF_PACKETS);
    REQUIRE(filter.GetDroppedSimulated() == HALF_PACKETS);
    REQUIRE(filter.GetDroppedInvalid() == 0);

    // Verify zero simulated leakage in accepted partition
    for (const auto& sample : accepted_hardware_packets) {
        REQUIRE(sample.quality_flag == QUALITY_HARDWARE_ROOT);
        REQUIRE(sample.quality_flag != QUALITY_SIMULATED);
        REQUIRE((sample.frame_count % 2) == 1); // Odd frames only
    }

    // Verify dropped partition contains strictly simulated packets
    for (const auto& sample : dropped_simulated_packets) {
        REQUIRE(sample.quality_flag == QUALITY_SIMULATED);
        REQUIRE(sample.quality_flag != QUALITY_HARDWARE_ROOT);
        REQUIRE((sample.frame_count % 2) == 0); // Even frames only
    }
}

TEST_CASE("Test 5: Circuit breaker actuation protection under SITL simulation overload", "[airgap][actuation][sitl_overload]") {
    // Mock Physical Hardware Actuator
    struct MockPhysicalActuator {
        uint64_t trip_commands_received{0};
        uint64_t physical_relay_engagements{0};
        bool breaker_closed{true};

        void TripBreaker() {
            trip_commands_received++;
            physical_relay_engagements++;
            breaker_closed = false;
        }

        void Reset() {
            breaker_closed = true;
        }
    };

    MockPhysicalActuator actuator;
    HardwareAirgapFilter filter;

    REQUIRE(actuator.trip_commands_received == 0);
    REQUIRE(actuator.physical_relay_engagements == 0);
    REQUIRE(actuator.breaker_closed == true);

    // Simulate emergency SITL failure storm: 10,000 synthetic failure packets
    constexpr uint64_t SITL_OVERLOAD_COUNT = 10'000;
    for (uint64_t i = 0; i < SITL_OVERLOAD_COUNT; ++i) {
        TelemetrySample synthetic_emergency{};
        synthetic_emergency.frame_count = i;
        synthetic_emergency.timestamp_ns = 2'000'000'000ULL + i * 1'000'000ULL;
        synthetic_emergency.fps = 0.0f;               // Simulation engine stalled
        synthetic_emergency.founder_stress = 1.0f;    // Catastrophic stress alarm
        synthetic_emergency.biome_id = 0;
        synthetic_emergency.quality_flag = QUALITY_SIMULATED; // 0x01: Tagged as SITL synthetic

        // If airgap filter authorizes, physical breaker trips
        if (filter.IsAuthorized(synthetic_emergency)) {
            actuator.TripBreaker();
        }
    }

    // Rigorously assert that zero simulated packets actuated physical hardware
    REQUIRE(actuator.trip_commands_received == 0);
    REQUIRE(actuator.physical_relay_engagements == 0);
    REQUIRE(actuator.breaker_closed == true);
    REQUIRE(filter.GetDroppedSimulated() == SITL_OVERLOAD_COUNT);
    REQUIRE(filter.GetAcceptedHardware() == 0);

    // Now inject genuine hardware root emergency packets
    constexpr uint64_t GENUINE_HARDWARE_PACKETS = 5;
    for (uint64_t i = 0; i < GENUINE_HARDWARE_PACKETS; ++i) {
        TelemetrySample genuine_hardware{};
        genuine_hardware.frame_count = SITL_OVERLOAD_COUNT + i;
        genuine_hardware.timestamp_ns = 3'000'000'000ULL + i * 1'000'000ULL;
        genuine_hardware.fps = 60.0f;
        genuine_hardware.founder_stress = 0.95f;      // High stress alarm from physical sensor
        genuine_hardware.biome_id = 1;
        genuine_hardware.quality_flag = QUALITY_HARDWARE_ROOT; // 0x02: Hardware secure element signed

        if (filter.IsAuthorized(genuine_hardware)) {
            actuator.TripBreaker();
        }
    }

    // Verify that authentic hardware root packets did actuate physical hardware
    REQUIRE(actuator.trip_commands_received == GENUINE_HARDWARE_PACKETS);
    REQUIRE(actuator.physical_relay_engagements == GENUINE_HARDWARE_PACKETS);
    REQUIRE(actuator.breaker_closed == false);
    REQUIRE(filter.GetAcceptedHardware() == GENUINE_HARDWARE_PACKETS);
    REQUIRE(filter.GetTotalInspected() == SITL_OVERLOAD_COUNT + GENUINE_HARDWARE_PACKETS);
}

TEST_CASE("Test 6: Multi-threaded concurrent producer-consumer stress testing", "[airgap][concurrency][stress]") {
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

    constexpr uint64_t TOTAL_STRESS_FRAMES = 100'000;
    // 80,000 simulated packets, 20,000 hardware root packets
    constexpr uint64_t EXPECTED_HARDWARE_COUNT = 20'000;
    constexpr uint64_t EXPECTED_SIMULATED_COUNT = 80'000;

    std::atomic<bool> start_flag{false};
    std::atomic<bool> producer_done{false};

    std::atomic<uint64_t> hardware_accepted_atomic{0};
    std::atomic<uint64_t> simulated_dropped_atomic{0};
    std::atomic<uint64_t> leakage_violations_atomic{0};
    std::atomic<uint64_t> total_consumed_atomic{0};

    HardwareAirgapFilter filter;

    auto producer_thread = std::thread([&]() {
        while (!start_flag.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        TelemetrySample sample{};
        for (uint64_t i = 0; i < TOTAL_STRESS_FRAMES; ++i) {
            sample.frame_count = i;
            sample.timestamp_ns = i * 1'000ULL;
            sample.fps = 60.0f;
            sample.founder_stress = 0.5f;
            sample.memory_slot_count = static_cast<uint32_t>(i % 32);
            sample.biome_id = 0;

            // Pattern: every 5th packet is hardware root (0x02), others simulated (0x01)
            sample.quality_flag = (i % 5 == 0) ? QUALITY_HARDWARE_ROOT : QUALITY_SIMULATED;

            while (!ring.Push(sample)) {
                std::this_thread::yield();
            }
        }
        producer_done.store(true, std::memory_order_release);
    });

    auto consumer_thread = std::thread([&]() {
        while (!start_flag.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        TelemetrySample sample{};
        while (true) {
            if (ring.Pop(sample)) {
                total_consumed_atomic.fetch_add(1, std::memory_order_relaxed);

                if (filter.IsAuthorized(sample)) {
                    hardware_accepted_atomic.fetch_add(1, std::memory_order_relaxed);
                    if (sample.quality_flag != QUALITY_HARDWARE_ROOT) {
                        leakage_violations_atomic.fetch_add(1, std::memory_order_relaxed);
                    }
                } else {
                    simulated_dropped_atomic.fetch_add(1, std::memory_order_relaxed);
                    if (sample.quality_flag == QUALITY_HARDWARE_ROOT) {
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

    // Start concurrent execution
    start_flag.store(true, std::memory_order_release);

    producer_thread.join();
    consumer_thread.join();

    // Verify all thread invariants on main thread
    REQUIRE(leakage_violations_atomic.load() == 0);
    REQUIRE(total_consumed_atomic.load() == TOTAL_STRESS_FRAMES);
    REQUIRE(hardware_accepted_atomic.load() == EXPECTED_HARDWARE_COUNT);
    REQUIRE(simulated_dropped_atomic.load() == EXPECTED_SIMULATED_COUNT);

    REQUIRE(filter.GetTotalInspected() == TOTAL_STRESS_FRAMES);
    REQUIRE(filter.GetAcceptedHardware() == EXPECTED_HARDWARE_COUNT);
    REQUIRE(filter.GetDroppedSimulated() == EXPECTED_SIMULATED_COUNT);
    REQUIRE(filter.GetDroppedInvalid() == 0);

    REQUIRE(ring.IsEmpty());
}
