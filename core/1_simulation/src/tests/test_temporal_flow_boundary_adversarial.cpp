#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <optional>
#include <memory>
#include <limits>
#include <cmath>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include <flecs.h>
#include <glm/glm.hpp>

#include "uhai/uhai_ring_buffer.hpp"
#include "core/stochastic_boundary.hpp"
#include "core/fabrication.hpp"

using Catch::Matchers::WithinAbs;
using namespace oasis::uhai;
using oasis::flow::TemporalFlowContractComponent;

// =============================================================================
// ADVERSARIAL CHALLENGER SUITE: Temporal Flow Boundary Stress Tests
// Dual Sprint 9: Temporal Flow Contracts & col-meshd
// =============================================================================

TEST_CASE("Adversarial 1: 100x Ghost Agent High-Frequency Bursts vs 1x Simulation Clock", "[adversarial][burst][clock]") {
    SECTION("Sub-millisecond burst of 500 contracts cannot distort 1x simulation tick progression") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        constexpr size_t BURST_COUNT = 500;
        size_t push_successes = 0;

        const auto t_start = std::chrono::steady_clock::now();
        // 100x Ghost Agent emits 500 contracts in rapid burst
        for (uint64_t i = 1; i <= BURST_COUNT; ++i) {
            FlowContract c{};
            c.contract_id = i;
            c.sender_node_id = 0x1000 + (i % 8);
            c.consumer_node_id = 0x0001;
            c.start_logical_tick = 10 + (i % 20); // starts between tick 10 and 29
            c.end_logical_tick = c.start_logical_tick + 50; // duration: exactly 50 ticks
            c.rate_per_tick = 0.100; // 50 * 0.100 = 5.0 kWh each
            c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
            c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
            c.quality_flags = QualityFlags::QUALITY_SIMULATED;
            if (channel.Push(c)) {
                push_successes++;
            }
        }
        const auto t_end = std::chrono::steady_clock::now();
        const auto burst_micros = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();

        REQUIRE(push_successes == BURST_COUNT);
        // Raw SPSC push of 500 items into in-memory ring executes in < 5 milliseconds
        REQUIRE(burst_micros < 5000);

        TemporalFlowContractComponent flow_comp{};
        uint64_t local_sim_tick = 0;
        double solar_kwh = 0.0, rain_l = 0.0, cal_kcal = 0.0;

        // Drain contracts into engine at local tick 0 (Oasis engine frame 0)
        size_t drained = 0;
        size_t admitted = 0;
        size_t rejected_over_capacity = 0;
        FlowContract incoming{};

        while (channel.Pop(incoming)) {
            drained++;
            if (flow_comp.AddOrUpdateContract(incoming, local_sim_tick)) {
                admitted++;
            } else {
                rejected_over_capacity++;
            }
        }

        REQUIRE(drained == BURST_COUNT);
        REQUIRE(admitted == TemporalFlowContractComponent::MAX_CONTRACTS); // 64
        REQUIRE(rejected_over_capacity == BURST_COUNT - TemporalFlowContractComponent::MAX_CONTRACTS); // 436 rejected cleanly without crash

        // Simulation clock must progress tick-by-tick strictly at 1x local pace
        for (local_sim_tick = 0; local_sim_tick <= 100; ++local_sim_tick) {
            flow_comp.StepTick(local_sim_tick, solar_kwh, rain_l, cal_kcal);
        }

        // Each admitted contract had duration 50 ticks and rate 0.100 kWh/tick = 5.0 kWh
        // Total solar accrued must equal exactly 64 * 5.0 = 320.0 kWh
        REQUIRE_THAT(solar_kwh, WithinAbs(64.0 * 5.0, 1e-6));
        REQUIRE(flow_comp.active_count == 0); // all retired cleanly
    }

    SECTION("100x Burst packet timestamps / intervals do not cause temporal bleeding") {
        TemporalFlowContractComponent comp{};

        // Two contracts from 100x agent arriving in the same frame
        FlowContract c1{};
        c1.contract_id = 11;
        c1.start_logical_tick = 5;
        c1.end_logical_tick = 15; // 10 ticks @ 2.0 = 20.0 L
        c1.rate_per_tick = 2.0;
        c1.resource_type = static_cast<uint16_t>(FlowResourceType::WATER);

        FlowContract c2{};
        c2.contract_id = 12;
        c2.start_logical_tick = 15;
        c2.end_logical_tick = 25; // 10 ticks @ 3.0 = 30.0 L
        c2.rate_per_tick = 3.0;
        c2.resource_type = static_cast<uint16_t>(FlowResourceType::WATER);

        comp.AddOrUpdateContract(c1, 0);
        comp.AddOrUpdateContract(c2, 0);

        double solar = 0.0, rain = 0.0, cal = 0.0;

        // At tick 0..4: No accrual
        for (uint64_t t = 0; t < 5; ++t) comp.StepTick(t, solar, rain, cal);
        REQUIRE(rain == 0.0);

        // At tick 5..14: c1 active (10 ticks * 2.0 = 20.0 L)
        for (uint64_t t = 5; t < 15; ++t) comp.StepTick(t, solar, rain, cal);
        REQUIRE_THAT(rain, WithinAbs(20.0, 1e-6));

        // At tick 15..24: c2 active (10 ticks * 3.0 = 30.0 L)
        for (uint64_t t = 15; t < 25; ++t) comp.StepTick(t, solar, rain, cal);
        REQUIRE_THAT(rain, WithinAbs(50.0, 1e-6));

        // At tick 25+: Both retired
        comp.StepTick(25, solar, rain, cal);
        REQUIRE(comp.active_count == 0);
        REQUIRE_THAT(rain, WithinAbs(50.0, 1e-6));
    }
}

TEST_CASE("Adversarial 2: Ring Buffer Capacity Overflow Drops & Recovery", "[adversarial][ring][overflow]") {
    SECTION("Massive overflow burst of 2500 contracts into 1024 capacity ring") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        constexpr size_t TOTAL_PUSHES = 2500;
        size_t success_count = 0;
        size_t fail_count = 0;

        for (uint64_t i = 1; i <= TOTAL_PUSHES; ++i) {
            FlowContract c{};
            c.contract_id = i;
            c.rate_per_tick = static_cast<double>(i);
            if (channel.Push(c)) {
                success_count++;
            } else {
                fail_count++;
            }
        }

        // Capacity is 1024
        REQUIRE(success_count == 1024);
        REQUIRE(fail_count == 2500 - 1024); // 1476 dropped

        const auto* hdr = channel.GetHeader();
        REQUIRE(hdr != nullptr);
        REQUIRE(hdr->dropped_frames.load(std::memory_order_relaxed) == 1476);
        REQUIRE(channel.GetRing()->DroppedCount() == 1476);
        REQUIRE(channel.GetRing()->Size() == 1024);

        // Verify the 1024 accepted contracts are uncorrupted and in exact sequence
        for (uint64_t i = 1; i <= 1024; ++i) {
            FlowContract out{};
            REQUIRE(channel.Pop(out) == true);
            REQUIRE(out.contract_id == i);
            REQUIRE_THAT(out.rate_per_tick, WithinAbs(static_cast<double>(i), 1e-9));
        }

        // Buffer is now empty
        REQUIRE(channel.GetRing()->IsEmpty() == true);
        REQUIRE(channel.GetRing()->Size() == 0);
        FlowContract dummy{};
        REQUIRE_FALSE(channel.Pop(dummy));

        // Dropped count persists across pops
        REQUIRE(hdr->dropped_frames.load(std::memory_order_relaxed) == 1476);

        // Ring recovery: subsequent pushes succeed immediately
        for (uint64_t i = 3001; i <= 3500; ++i) {
            FlowContract c{};
            c.contract_id = i;
            REQUIRE(channel.Push(c) == true);
        }
        REQUIRE(channel.GetRing()->Size() == 500);

        for (uint64_t i = 3001; i <= 3500; ++i) {
            FlowContract out{};
            REQUIRE(channel.Pop(out) == true);
            REQUIRE(out.contract_id == i);
        }
        REQUIRE(channel.GetRing()->IsEmpty() == true);
    }
}

TEST_CASE("Adversarial 3: Abrupt Termination Edge Case Matrix", "[adversarial][termination][edge]") {
    SECTION("Edge 3A: Cancellation arriving BEFORE start tick produces 0.0 accrual and clean retirement") {
        TemporalFlowContractComponent comp{};

        FlowContract contract{};
        contract.contract_id = 701;
        contract.start_logical_tick = 200;
        contract.end_logical_tick = 800;
        contract.rate_per_tick = 2.50; // Total potential: 600 * 2.5 = 1500 kWh
        contract.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        contract.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        REQUIRE(comp.AddOrUpdateContract(contract, 0) == true);
        REQUIRE(comp.active_count == 1);

        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t < 50; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE(solar == 0.0);

        // At tick 50 (150 ticks before start tick 200), cancellation arrives!
        FlowContract cancel{};
        cancel.contract_id = 701;
        cancel.end_logical_tick = 50; // or 0
        cancel.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);

        REQUIRE(comp.AddOrUpdateContract(cancel, 50) == true);

        // Step tick 50: contract is retired immediately
        comp.StepTick(50, solar, rain, cal);
        REQUIRE(comp.active_count == 0);

        // Run all the way through tick 1000
        for (uint64_t t = 51; t <= 1000; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }

        REQUIRE_THAT(solar, WithinAbs(0.0, 1e-9));
        REQUIRE(comp.total_electricity_received == 0.0);
        REQUIRE(comp.active_count == 0);
    }

    SECTION("Edge 3B: Cancellation arriving at EXACT start tick produces 0.0 accrual") {
        TemporalFlowContractComponent comp{};

        FlowContract contract{};
        contract.contract_id = 702;
        contract.start_logical_tick = 100;
        contract.end_logical_tick = 500;
        contract.rate_per_tick = 5.0; // 400 * 5 = 2000 kcal
        contract.resource_type = static_cast<uint16_t>(FlowResourceType::CALORIES);
        contract.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        comp.AddOrUpdateContract(contract, 0);

        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t < 100; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE(cal == 0.0);

        // At exact start tick 100: cancellation arrives!
        FlowContract cancel{};
        cancel.contract_id = 702;
        cancel.start_logical_tick = 100;
        cancel.end_logical_tick = 100; // clamped to exact start
        cancel.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);

        comp.AddOrUpdateContract(cancel, 100);

        // Step tick 100: retired immediately without accrual
        comp.StepTick(100, solar, rain, cal);
        REQUIRE(comp.active_count == 0);
        REQUIRE_THAT(cal, WithinAbs(0.0, 1e-9));

        for (uint64_t t = 101; t <= 600; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }

        REQUIRE_THAT(cal, WithinAbs(0.0, 1e-9));
        REQUIRE(comp.total_calories_received == 0.0);
    }

    SECTION("Edge 3C: Late cancellation arriving AFTER contract end tick does not corrupt state") {
        TemporalFlowContractComponent comp{};

        FlowContract contract{};
        contract.contract_id = 703;
        contract.start_logical_tick = 10;
        contract.end_logical_tick = 30; // 20 ticks @ 1.0 = 20.0 L
        contract.rate_per_tick = 1.0;
        contract.resource_type = static_cast<uint16_t>(FlowResourceType::WATER);
        contract.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        comp.AddOrUpdateContract(contract, 0);

        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t <= 35; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }

        REQUIRE_THAT(rain, WithinAbs(20.0, 1e-6));
        REQUIRE(comp.active_count == 0); // naturally retired at tick 30

        // At tick 50 (after end tick 30), a stale cancellation arrives
        FlowContract stale_cancel{};
        stale_cancel.contract_id = 703;
        stale_cancel.end_logical_tick = 30;
        stale_cancel.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);

        // Contract is no longer active, and end_logical_tick <= current_tick (30 <= 50)
        // Must return false and NOT resurrect the contract
        REQUIRE_FALSE(comp.AddOrUpdateContract(stale_cancel, 50));
        REQUIRE(comp.active_count == 0);

        // Step through tick 100
        for (uint64_t t = 36; t <= 100; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }

        REQUIRE_THAT(rain, WithinAbs(20.0, 1e-6));
        REQUIRE(comp.active_count == 0);
    }

    SECTION("Edge 3D: Multiple redundant and conflicting cancellations on same contract") {
        TemporalFlowContractComponent comp{};

        FlowContract contract{};
        contract.contract_id = 704;
        contract.start_logical_tick = 100;
        contract.end_logical_tick = 1000;
        contract.rate_per_tick = 1.0;
        contract.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        contract.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        comp.AddOrUpdateContract(contract, 0);

        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t < 150; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE_THAT(solar, WithinAbs(50.0, 1e-6)); // 150 - 100 = 50 ticks * 1.0 = 50.0

        // Cancellation 1 at tick 150: claims end tick 300
        FlowContract c1{};
        c1.contract_id = 704;
        c1.end_logical_tick = 300;
        c1.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);
        REQUIRE(comp.AddOrUpdateContract(c1, 150));
        REQUIRE(comp.FindContract(704)->end_logical_tick == 300);

        // Advance to tick 180
        for (uint64_t t = 150; t < 180; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE_THAT(solar, WithinAbs(80.0, 1e-6)); // 80.0 accrued

        // Cancellation 2 at tick 180: sets earlier end tick 220
        FlowContract c2{};
        c2.contract_id = 704;
        c2.end_logical_tick = 220;
        c2.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);
        REQUIRE(comp.AddOrUpdateContract(c2, 180));
        REQUIRE(comp.FindContract(704)->end_logical_tick == 220);

        // Cancellation 3 at tick 180: attempts to extend end tick to 500 (adversarial extension!)
        // Invariant: termination end tick cannot be lengthened!
        FlowContract c3{};
        c3.contract_id = 704;
        c3.end_logical_tick = 500;
        c3.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);
        REQUIRE(comp.AddOrUpdateContract(c3, 180));
        // Must remain 220, NOT extended to 500!
        REQUIRE(comp.FindContract(704)->end_logical_tick == 220);

        // Step ticks [180..219]
        for (uint64_t t = 180; t < 220; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE_THAT(solar, WithinAbs(120.0, 1e-6)); // (220 - 100) * 1.0 = 120.0

        // Step tick 220: contract retired cleanly
        comp.StepTick(220, solar, rain, cal);
        REQUIRE(comp.active_count == 0);

        // Further ticks up to 1000: zero accrual
        for (uint64_t t = 221; t <= 1000; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE_THAT(solar, WithinAbs(120.0, 1e-6));
        REQUIRE(comp.total_electricity_received == 120.0);
    }
}

TEST_CASE("Adversarial 4: Boundary Value Analysis & Capacity Saturation", "[adversarial][boundary][limits]") {
    SECTION("Zero and negative flow rates") {
        TemporalFlowContractComponent comp{};

        FlowContract zero_c{};
        zero_c.contract_id = 801;
        zero_c.start_logical_tick = 10;
        zero_c.end_logical_tick = 100;
        zero_c.rate_per_tick = 0.0;
        zero_c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        zero_c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        REQUIRE(comp.AddOrUpdateContract(zero_c, 0));

        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t <= 120; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }

        REQUIRE(solar == 0.0);
        REQUIRE_FALSE(std::isnan(solar));
        REQUIRE_FALSE(std::isinf(solar));
        REQUIRE(comp.active_count == 0);
    }

    SECTION("Huge tick horizons (near 64-bit limits)") {
        TemporalFlowContractComponent comp{};

        constexpr uint64_t HUGE_START = 10'000'000'000'000ULL;
        constexpr uint64_t HUGE_END   = 10'000'000'000'100ULL;

        FlowContract huge_c{};
        huge_c.contract_id = 802;
        huge_c.start_logical_tick = HUGE_START;
        huge_c.end_logical_tick = HUGE_END; // 100 ticks @ 1.0 = 100.0 L
        huge_c.rate_per_tick = 1.0;
        huge_c.resource_type = static_cast<uint16_t>(FlowResourceType::WATER);
        huge_c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        REQUIRE(comp.AddOrUpdateContract(huge_c, HUGE_START - 100));

        double solar = 0.0, rain = 0.0, cal = 0.0;
        // Step before window
        comp.StepTick(HUGE_START - 1, solar, rain, cal);
        REQUIRE(rain == 0.0);

        // Step entire 100-tick window
        for (uint64_t t = HUGE_START; t < HUGE_END; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE_THAT(rain, WithinAbs(100.0, 1e-6));

        // Step expiration
        comp.StepTick(HUGE_END, solar, rain, cal);
        REQUIRE(comp.active_count == 0);
    }

    SECTION("Max Active Contracts (64) saturation, eviction, and slot recycling") {
        TemporalFlowContractComponent comp{};

        // Fill all 64 slots with staggered horizons
        for (uint64_t i = 1; i <= 64; ++i) {
            FlowContract c{};
            c.contract_id = i;
            c.start_logical_tick = 10;
            // Half expire at tick 30, half expire at tick 60
            c.end_logical_tick = (i <= 32) ? 30 : 60;
            c.rate_per_tick = 1.0;
            c.resource_type = static_cast<uint16_t>(FlowResourceType::CALORIES);
            c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
            REQUIRE(comp.AddOrUpdateContract(c, 0));
        }

        REQUIRE(comp.active_count == 64);

        // Overflow attempts 65..100 must be rejected
        for (uint64_t i = 65; i <= 100; ++i) {
            FlowContract c{};
            c.contract_id = i;
            c.start_logical_tick = 10;
            c.end_logical_tick = 60;
            c.rate_per_tick = 1.0;
            c.resource_type = static_cast<uint16_t>(FlowResourceType::CALORIES);
            c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
            REQUIRE_FALSE(comp.AddOrUpdateContract(c, 0));
        }
        REQUIRE(comp.active_count == 64);

        double solar = 0.0, rain = 0.0, cal = 0.0;

        // Step ticks [10..29]: all 64 active -> 20 ticks * 64 = 1280 kcal
        for (uint64_t t = 10; t < 30; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE_THAT(cal, WithinAbs(1280.0, 1e-6));
        REQUIRE(comp.active_count == 64);

        // Step tick 30: first 32 contracts expire!
        comp.StepTick(30, solar, rain, cal);
        REQUIRE(comp.active_count == 32);

        // Now we can add 32 new contracts into the freed slots!
        for (uint64_t i = 101; i <= 132; ++i) {
            FlowContract c{};
            c.contract_id = i;
            c.start_logical_tick = 35;
            c.end_logical_tick = 60;
            c.rate_per_tick = 2.0;
            c.resource_type = static_cast<uint16_t>(FlowResourceType::CALORIES);
            c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
            REQUIRE(comp.AddOrUpdateContract(c, 30));
        }
        REQUIRE(comp.active_count == 64);

        // 65th attempt is rejected again
        FlowContract rejected{};
        rejected.contract_id = 999;
        REQUIRE_FALSE(comp.AddOrUpdateContract(rejected, 30));
    }
}

TEST_CASE("Adversarial 5: LockFreeSpscRing 64-Bit Index Wrapping Past 2^32 and 2^64", "[adversarial][ring][wrap]") {
    SECTION("Crossing 32-bit uint32 boundary (0xFFFFFFFF -> 0x100000000)") {
        alignas(64) uint8_t raw_shm[TOTAL_FLOW_SHM_SIZE];
        std::memset(raw_shm, 0, sizeof(raw_shm));

        auto* hdr = reinterpret_cast<SharedRingHeader*>(raw_shm);
        constexpr uint64_t WRAP_32_START = 0x00000000FFFFFE00ULL; // 512 slots before 2^32
        hdr->write_index.store(WRAP_32_START, std::memory_order_relaxed);
        hdr->read_index.store(WRAP_32_START, std::memory_order_relaxed);
        hdr->capacity = RING_CAPACITY;
        hdr->element_size = sizeof(FlowContractSlot);
        hdr->magic_signature = UHAI_MAGIC;
        hdr->version = UHAI_VERSION;

        LockFreeSpscRing<FlowContractSlot, RING_CAPACITY> ring(raw_shm);

        constexpr size_t TEST_TRANSFERS = 2048; // spans across 2^32 boundary multiple times
        for (uint64_t i = 1; i <= TEST_TRANSFERS; ++i) {
            FlowContract in_c{};
            in_c.contract_id = 0xABCD0000ULL + i;
            in_c.rate_per_tick = static_cast<double>(i) * 0.5;

            FlowContractSlot in_slot{in_c};
            REQUIRE(ring.Push(in_slot) == true);

            FlowContractSlot out_slot{};
            REQUIRE(ring.Pop(out_slot) == true);
            REQUIRE(out_slot.contract.contract_id == 0xABCD0000ULL + i);
            REQUIRE_THAT(out_slot.contract.rate_per_tick, WithinAbs(static_cast<double>(i) * 0.5, 1e-9));
        }

        REQUIRE(hdr->write_index.load() == WRAP_32_START + TEST_TRANSFERS);
        REQUIRE(hdr->read_index.load() == WRAP_32_START + TEST_TRANSFERS);
        REQUIRE(hdr->dropped_frames.load() == 0);
        // Verify write index has crossed 2^32
        REQUIRE(hdr->write_index.load() > 0x100000000ULL);
    }

    SECTION("Crossing 64-bit uint64 wrap boundary (0xFFFFFFFFFFFFFFFF -> 0x0)") {
        alignas(64) uint8_t raw_shm[TOTAL_FLOW_SHM_SIZE];
        std::memset(raw_shm, 0, sizeof(raw_shm));

        auto* hdr = reinterpret_cast<SharedRingHeader*>(raw_shm);
        // Position at 256 items before UINT64_MAX wrap
        constexpr uint64_t WRAP_64_START = std::numeric_limits<uint64_t>::max() - 255;
        hdr->write_index.store(WRAP_64_START, std::memory_order_relaxed);
        hdr->read_index.store(WRAP_64_START, std::memory_order_relaxed);
        hdr->capacity = RING_CAPACITY;
        hdr->element_size = sizeof(FlowContractSlot);
        hdr->magic_signature = UHAI_MAGIC;
        hdr->version = UHAI_VERSION;

        LockFreeSpscRing<FlowContractSlot, RING_CAPACITY> ring(raw_shm);

        constexpr size_t TEST_TRANSFERS = 1024;
        for (uint64_t i = 1; i <= TEST_TRANSFERS; ++i) {
            FlowContract in_c{};
            in_c.contract_id = 0xCAFE0000ULL + i;
            in_c.rate_per_tick = static_cast<double>(i);

            FlowContractSlot in_slot{in_c};
            REQUIRE(ring.Push(in_slot) == true);

            FlowContractSlot out_slot{};
            REQUIRE(ring.Pop(out_slot) == true);
            REQUIRE(out_slot.contract.contract_id == 0xCAFE0000ULL + i);
            REQUIRE_THAT(out_slot.contract.rate_per_tick, WithinAbs(static_cast<double>(i), 1e-9));
        }

        // Verified that unsigned wrap arithmetic (w - r) functions seamlessly across 2^64
        const uint64_t expected_final_idx = WRAP_64_START + TEST_TRANSFERS; // wraps naturally modulo 2^64
        REQUIRE(hdr->write_index.load() == expected_final_idx);
        REQUIRE(hdr->read_index.load() == expected_final_idx);
        REQUIRE(hdr->dropped_frames.load() == 0);
    }
}

TEST_CASE("Adversarial 6: Multi-Threaded Concurrent High-Frequency Ghost Flooding & SPSC Backpressure", "[adversarial][concurrency][thread]") {
    SECTION("Concurrent ghost producer vs 1x sim consumer over 50,000 contracts with backpressure tracking") {
        alignas(64) uint8_t raw_shm[TOTAL_FLOW_SHM_SIZE];
        std::memset(raw_shm, 0, sizeof(raw_shm));

        auto* hdr = reinterpret_cast<SharedRingHeader*>(raw_shm);
        hdr->write_index.store(0, std::memory_order_relaxed);
        hdr->dropped_frames.store(0, std::memory_order_relaxed);
        hdr->read_index.store(0, std::memory_order_relaxed);
        hdr->capacity = RING_CAPACITY;
        hdr->element_size = sizeof(FlowContractSlot);
        hdr->magic_signature = UHAI_MAGIC;
        hdr->version = UHAI_VERSION;

        constexpr size_t TOTAL_PACKETS = 50'000;
        std::atomic<bool> producer_done{false};

        // Ghost producer thread running at maximum burst rate
        std::thread producer([&]() {
            LockFreeSpscRing<FlowContractSlot, RING_CAPACITY> ring(raw_shm);
            for (uint64_t i = 1; i <= TOTAL_PACKETS; ++i) {
                FlowContract c{};
                c.contract_id = i;
                c.sender_node_id = 0x100A;
                c.consumer_node_id = 0x0001;
                c.start_logical_tick = i;
                c.end_logical_tick = i + 100;
                c.rate_per_tick = 0.050;
                c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
                // Every 10th packet is an abrupt termination signal
                if (i % 10 == 0) {
                    c.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);
                    c.end_logical_tick = i; // abort
                } else {
                    c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
                }
                FlowContractSlot slot{c};

                while (!ring.Push(slot)) {
                    std::this_thread::yield();
                }
            }
            producer_done.store(true, std::memory_order_release);
        });

        // Local engine consumer thread draining packets
        size_t consumed_count = 0;
        size_t termination_count = 0;
        LockFreeSpscRing<FlowContractSlot, RING_CAPACITY> consumer_ring(raw_shm);

        while (!producer_done.load(std::memory_order_acquire) || !consumer_ring.IsEmpty()) {
            FlowContractSlot slot{};
            if (consumer_ring.Pop(slot)) {
                consumed_count++;
                if (slot.contract.status == static_cast<uint8_t>(FlowContractStatus::TERMINATED)) {
                    termination_count++;
                }
            } else {
                std::this_thread::yield();
            }
        }

        producer.join();

        // 100% of packets were transferred without loss
        REQUIRE(consumed_count == TOTAL_PACKETS);
        REQUIRE(termination_count == TOTAL_PACKETS / 10);
        REQUIRE(consumer_ring.IsEmpty() == true);
        // Note: hdr->dropped_frames records transient buffer-full retry events during backpressure
        // It demonstrates the atomic overrun accounting functions under heavy multi-threaded contention
        const uint64_t retried_pushes = hdr->dropped_frames.load(std::memory_order_relaxed);
        spdlog::info("[Adversarial 6] Successfully transferred 50,000 contracts ({} transient backpressure full events recorded)",
                     retried_pushes);
    }
}

TEST_CASE("Adversarial 7: Out-of-Order Resurrection & State Machine Robustness", "[adversarial][resurrection][state]") {
    SECTION("Out-of-order ACTIVE packet arriving after TERMINATED status") {
        TemporalFlowContractComponent comp{};

        // Contract 901 established: [100..500) @ 1.0 kWh/tick
        FlowContract initial{};
        initial.contract_id = 901;
        initial.start_logical_tick = 100;
        initial.end_logical_tick = 500;
        initial.rate_per_tick = 1.0;
        initial.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        initial.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
        comp.AddOrUpdateContract(initial, 0);

        // Run to tick 150 (accrues 50 kWh)
        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t < 150; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE_THAT(solar, WithinAbs(50.0, 1e-6));

        // Cancellation arrives at tick 150: terminates early at tick 200
        FlowContract cancel{};
        cancel.contract_id = 901;
        cancel.end_logical_tick = 200;
        cancel.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);
        comp.AddOrUpdateContract(cancel, 150);

        REQUIRE(comp.FindContract(901)->status == static_cast<uint8_t>(FlowContractStatus::TERMINATED));
        REQUIRE(comp.FindContract(901)->end_logical_tick == 200);

        // At tick 175: An out-of-order duplicate ACTIVE packet arrives (say network delay)
        // Trying to reset end_logical_tick back to 500!
        FlowContract late_active = initial;
        comp.AddOrUpdateContract(late_active, 175);

        // Observe current component behavior:
        // Does AddOrUpdateContract overwrite the contract?
        const auto* c = comp.FindContract(901);
        REQUIRE(c != nullptr);
        // Document whether current behavior permits resurrection or preserves TERMINATED status
        spdlog::info("[Adversarial 7] Contract 901 status after out-of-order ACTIVE arrival: status={}, end_tick={}",
                     c->status, c->end_logical_tick);
    }
}

TEST_CASE("Adversarial 8: Negative Rate Invariant & Autarky Math Boundary Vulnerability", "[adversarial][negative][math]") {
    SECTION("Negative rate contracts drain resources without floor clamping in StepTick") {
        TemporalFlowContractComponent comp{};

        FlowContract drain_c{};
        drain_c.contract_id = 999;
        drain_c.start_logical_tick = 10;
        drain_c.end_logical_tick = 20;
        drain_c.rate_per_tick = -5.0; // Negative flow rate (siphoning)
        drain_c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        drain_c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        REQUIRE(comp.AddOrUpdateContract(drain_c, 0));

        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t < 20; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }

        // Empirical finding: StepTick blindly adds rate_per_tick, allowing negative stored energy
        spdlog::warn("[Adversarial 8] Resource solar_kwh after negative rate: {}", solar);
        REQUIRE(solar == -50.0); // Drained into negative numbers!

        // If solar_stored is negative, test autarky ratio
        oasis::boundary::InfrastructureAutarkyComponent infra{};
        infra.solar_stored_kwh = solar; // -50.0
        double phi = infra.EvaluatePhi();
        spdlog::warn("[Adversarial 8] Infrastructure Autarky Phi with negative stored: {}", phi);
        // Autarky becomes negative
        REQUIRE(phi < 0.0);

        // Mathematical singularity check: if solar_stored == -GENESIS_EPSILON (-0.001)
        infra.solar_stored_kwh = -oasis::boundary::GENESIS_EPSILON;
        double ratio = infra.EvaluateAutarkyRatio(0.0);
        spdlog::warn("[Adversarial 8] Autarky ratio when solar_stored == -epsilon: {}", ratio);
        REQUIRE(std::isnan(ratio)); // NaN generated!
    }
}

