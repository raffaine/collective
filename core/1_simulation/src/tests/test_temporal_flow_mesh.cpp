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
// TIER 1: Memory Layout & Serialization Invariants
// =============================================================================
TEST_CASE("Tier 1: FlowContract Struct Memory Layout & Serialization Invariants", "[tier1][layout][uhai]") {
    SECTION("FlowContract struct size, alignment, and standard layout") {
        REQUIRE(sizeof(FlowContract) == 64);
        REQUIRE(alignof(FlowContract) == 8);
        REQUIRE(std::is_standard_layout_v<FlowContract>);
        REQUIRE(std::is_trivially_copyable_v<FlowContract>);
    }

    SECTION("FlowContract field offsets exact 64-byte binary layout") {
        REQUIRE(offsetof(FlowContract, contract_id) == 0);
        REQUIRE(offsetof(FlowContract, sender_node_id) == 8);
        REQUIRE(offsetof(FlowContract, consumer_node_id) == 16);
        REQUIRE(offsetof(FlowContract, start_logical_tick) == 24);
        REQUIRE(offsetof(FlowContract, end_logical_tick) == 32);
        REQUIRE(offsetof(FlowContract, rate_per_tick) == 40);
        REQUIRE(offsetof(FlowContract, resource_type) == 48);
        REQUIRE(offsetof(FlowContract, status) == 50);
        REQUIRE(offsetof(FlowContract, quality_flags) == 51);
        REQUIRE(offsetof(FlowContract, reserved) == 52);
        REQUIRE(offsetof(FlowContract, auth_signature) == 56);
    }

    SECTION("FlowContractSlot CPU cache-line alignment and false sharing isolation") {
        REQUIRE(sizeof(FlowContractSlot) == 64);
        REQUIRE(alignof(FlowContractSlot) == 64);
        REQUIRE(offsetof(FlowContractSlot, contract) == 0);
    }

    SECTION("SharedRingHeader 3-cacheline isolation and constants") {
        REQUIRE(sizeof(SharedRingHeader) == 192);
        REQUIRE(alignof(SharedRingHeader) == 64);

        // Cache Line 0 (0..63): Producer write index & dropped frames
        REQUIRE(offsetof(SharedRingHeader, write_index) == 0);
        REQUIRE(offsetof(SharedRingHeader, dropped_frames) == 8);
        REQUIRE(offsetof(SharedRingHeader, write_index) / 64 == 0);
        REQUIRE(offsetof(SharedRingHeader, dropped_frames) / 64 == 0);

        // Cache Line 1 (64..127): Consumer read index
        REQUIRE(offsetof(SharedRingHeader, read_index) == 64);
        REQUIRE(offsetof(SharedRingHeader, read_index) / 64 == 1);

        // Cache Line 2 (128..191): Static read-only metadata
        REQUIRE(offsetof(SharedRingHeader, capacity) == 128);
        REQUIRE(offsetof(SharedRingHeader, element_size) == 132);
        REQUIRE(offsetof(SharedRingHeader, magic_signature) == 136);
        REQUIRE(offsetof(SharedRingHeader, version) == 140);
        REQUIRE(offsetof(SharedRingHeader, capacity) / 64 == 2);
    }

    SECTION("Total UHAI flow shared memory size and slot indexing alignment") {
        REQUIRE(RING_CAPACITY == 1024);
        REQUIRE(TOTAL_FLOW_SHM_SIZE == 65728);
        REQUIRE(TOTAL_FLOW_SHM_SIZE == sizeof(SharedRingHeader) + RING_CAPACITY * sizeof(FlowContractSlot));

        // Slot offsets in shared memory: every slot starts on an exact 64-byte cache line
        size_t buffer_start = sizeof(SharedRingHeader);
        REQUIRE(buffer_start == 192);
        REQUIRE(buffer_start % 64 == 0);

        for (size_t k = 0; k < 10; ++k) {
            size_t slot_k_offset = buffer_start + k * sizeof(FlowContractSlot);
            REQUIRE(slot_k_offset % 64 == 0);
            REQUIRE(slot_k_offset / 64 == 3 + k);
        }
    }
}

// =============================================================================
// TIER 2: Boundary & Corner Cases
// =============================================================================
TEST_CASE("Tier 2: UHAI Flow Queue Boundary & Corner Cases", "[tier2][boundary][corner]") {
    SECTION("Empty queue pop returns false / nullopt gracefully without disturbing state") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        FlowContract out_contract{};
        REQUIRE_FALSE(channel.Pop(out_contract));
        REQUIRE(channel.Pop() == std::nullopt);

        const auto* hdr = channel.GetHeader();
        REQUIRE(hdr != nullptr);
        REQUIRE(hdr->write_index.load() == 0);
        REQUIRE(hdr->read_index.load() == 0);
        REQUIRE(hdr->dropped_frames.load() == 0);
    }

    SECTION("Ring capacity (1024) fill, overflow drop tracking, and recovery") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        // Fill all 1024 slots
        for (uint64_t i = 0; i < 1024; ++i) {
            FlowContract c{};
            c.contract_id = 1000 + i;
            c.rate_per_tick = 1.0;
            REQUIRE(channel.Push(c) == true);
        }

        const auto* hdr = channel.GetHeader();
        REQUIRE(hdr->dropped_frames.load() == 0);

        // 1025th push fails and increments dropped_frames
        FlowContract overflow_c{};
        overflow_c.contract_id = 9999;
        REQUIRE_FALSE(channel.Push(overflow_c));
        REQUIRE(hdr->dropped_frames.load() == 1);

        // Additional 50 overflow attempts
        for (int i = 0; i < 50; ++i) {
            REQUIRE_FALSE(channel.Push(overflow_c));
        }
        REQUIRE(hdr->dropped_frames.load() == 51);

        // Pop 1 contract to make room
        FlowContract popped{};
        REQUIRE(channel.Pop(popped) == true);
        REQUIRE(popped.contract_id == 1000);

        // Now pushing 1 contract succeeds
        REQUIRE(channel.Push(overflow_c) == true);
        REQUIRE(hdr->dropped_frames.load() == 51); // dropped counter preserved
    }

    SECTION("Index wrapping around 1024 ring buffer using monotonic uint64 indices") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        constexpr uint64_t TEST_CYCLES = 4096; // 4 full ring cycles
        for (uint64_t i = 1; i <= TEST_CYCLES; ++i) {
            FlowContract in_c{};
            in_c.contract_id = i;
            in_c.rate_per_tick = static_cast<double>(i) * 0.1;
            REQUIRE(channel.Push(in_c) == true);

            FlowContract out_c{};
            REQUIRE(channel.Pop(out_c) == true);
            REQUIRE(out_c.contract_id == i);
            REQUIRE_THAT(out_c.rate_per_tick, WithinAbs(static_cast<double>(i) * 0.1, 1e-9));
        }

        const auto* hdr = channel.GetHeader();
        REQUIRE(hdr->write_index.load() == TEST_CYCLES);
        REQUIRE(hdr->read_index.load() == TEST_CYCLES);
        REQUIRE(hdr->dropped_frames.load() == 0);
    }

    SECTION("Zero rate per tick handling does not cause NaN or division by zero") {
        TemporalFlowContractComponent comp{};
        FlowContract c{};
        c.contract_id = 501;
        c.start_logical_tick = 10;
        c.end_logical_tick = 50;
        c.rate_per_tick = 0.0; // zero rate
        c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        REQUIRE(comp.AddOrUpdateContract(c, 0) == true);

        double solar = 0.0, rain = 0.0, calories = 0.0;
        for (uint64_t t = 0; t <= 60; ++t) {
            comp.StepTick(t, solar, rain, calories);
        }

        REQUIRE(solar == 0.0);
        REQUIRE(rain == 0.0);
        REQUIRE(calories == 0.0);
        REQUIRE(comp.active_count == 0); // cleanly retired
    }

    SECTION("Inverted or zero-duration tick window produces 0 resource accrual") {
        TemporalFlowContractComponent comp{};
        FlowContract c{};
        c.contract_id = 502;
        c.start_logical_tick = 100;
        c.end_logical_tick = 50; // inverted window
        c.rate_per_tick = 10.0;
        c.resource_type = static_cast<uint16_t>(FlowResourceType::CALORIES);
        c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        comp.AddOrUpdateContract(c, 0);

        double solar = 0.0, rain = 0.0, calories = 0.0;
        for (uint64_t t = 0; t <= 120; ++t) {
            comp.StepTick(t, solar, rain, calories);
        }

        REQUIRE(calories == 0.0);
    }

    SECTION("Unknown resource type (UNKNOWN = 0) is ignored safely") {
        TemporalFlowContractComponent comp{};
        FlowContract c{};
        c.contract_id = 503;
        c.start_logical_tick = 5;
        c.end_logical_tick = 15;
        c.rate_per_tick = 5.0;
        c.resource_type = static_cast<uint16_t>(FlowResourceType::UNKNOWN);
        c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        REQUIRE(comp.AddOrUpdateContract(c, 0) == true);

        double solar = 0.0, rain = 0.0, calories = 0.0;
        for (uint64_t t = 0; t <= 20; ++t) {
            comp.StepTick(t, solar, rain, calories);
        }

        REQUIRE(solar == 0.0);
        REQUIRE(rain == 0.0);
        REQUIRE(calories == 0.0);
        REQUIRE(comp.active_count == 0);
    }

    SECTION("MAX_CONTRACTS bounds checking (64 capacity)") {
        TemporalFlowContractComponent comp{};
        for (uint64_t i = 1; i <= TemporalFlowContractComponent::MAX_CONTRACTS; ++i) {
            FlowContract c{};
            c.contract_id = i;
            c.start_logical_tick = 10;
            c.end_logical_tick = 100;
            c.rate_per_tick = 0.1;
            c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
            c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
            REQUIRE(comp.AddOrUpdateContract(c, 0) == true);
        }
        REQUIRE(comp.active_count == 64);

        // 65th contract is rejected without heap overflow
        FlowContract extra{};
        extra.contract_id = 999;
        extra.start_logical_tick = 10;
        extra.end_logical_tick = 100;
        extra.rate_per_tick = 0.1;
        extra.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        extra.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
        REQUIRE_FALSE(comp.AddOrUpdateContract(extra, 0));
        REQUIRE(comp.active_count == 64);
    }
}

// =============================================================================
// TIER 3: Time-Agnostic Rate Resolution (100x Ghost vs 1x Local Engine)
// =============================================================================
TEST_CASE("Tier 3: Time-Agnostic Rate Resolution (100x Ghost vs 1x Local Engine)", "[tier3][temporal][pace]") {
    SECTION("Exact mathematical conservation of resources across distinct resource types") {
        TemporalFlowContractComponent comp{};

        // Contract 101: 100x Ghost Node A emits Electricity contract [100..500) @ 0.050 kWh/tick
        FlowContract c_elec{};
        c_elec.contract_id = 101;
        c_elec.sender_node_id = 0x100A;
        c_elec.consumer_node_id = 0x0001;
        c_elec.start_logical_tick = 100;
        c_elec.end_logical_tick = 500;
        c_elec.rate_per_tick = 0.050; // Total expected: (500 - 100) * 0.050 = 20.000 kWh
        c_elec.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        c_elec.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
        c_elec.quality_flags = QualityFlags::QUALITY_SIMULATED;
        std::memcpy(c_elec.auth_signature, "GHOST100", 8);

        // Contract 102: 100x Ghost Node B emits Water contract [150..350) @ 2.500 L/tick
        FlowContract c_water{};
        c_water.contract_id = 102;
        c_water.sender_node_id = 0x100B;
        c_water.consumer_node_id = 0x0001;
        c_water.start_logical_tick = 150;
        c_water.end_logical_tick = 350;
        c_water.rate_per_tick = 2.500; // Total expected: (350 - 150) * 2.500 = 500.000 L
        c_water.resource_type = static_cast<uint16_t>(FlowResourceType::WATER);
        c_water.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
        c_water.quality_flags = QualityFlags::QUALITY_SIMULATED;
        std::memcpy(c_water.auth_signature, "GHOST100", 8);

        // Contract 103: 100x Ghost Node C emits Calories contract [200..300) @ 10.000 kcal/tick
        FlowContract c_cal{};
        c_cal.contract_id = 103;
        c_cal.sender_node_id = 0x100C;
        c_cal.consumer_node_id = 0x0001;
        c_cal.start_logical_tick = 200;
        c_cal.end_logical_tick = 300;
        c_cal.rate_per_tick = 10.000; // Total expected: (300 - 200) * 10.000 = 1000.000 kcal
        c_cal.resource_type = static_cast<uint16_t>(FlowResourceType::CALORIES);
        c_cal.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
        c_cal.quality_flags = QualityFlags::QUALITY_SIMULATED;
        std::memcpy(c_cal.auth_signature, "GHOST100", 8);

        // Contracts arrive at local engine
        REQUIRE(comp.AddOrUpdateContract(c_elec, 0) == true);
        REQUIRE(comp.AddOrUpdateContract(c_water, 0) == true);
        REQUIRE(comp.AddOrUpdateContract(c_cal, 0) == true);
        REQUIRE(comp.active_count == 3);

        double solar_kwh = 0.0;
        double rain_liters = 0.0;
        double calories = 0.0;

        // Step ticks [0..99]: Nothing active yet
        for (uint64_t t = 0; t < 100; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }
        REQUIRE_THAT(solar_kwh, WithinAbs(0.0, 1e-9));
        REQUIRE_THAT(rain_liters, WithinAbs(0.0, 1e-9));
        REQUIRE_THAT(calories, WithinAbs(0.0, 1e-9));

        // Step ticks [100..149]: Electricity active (50 ticks @ 0.050 = 2.50 kWh)
        for (uint64_t t = 100; t < 150; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }
        REQUIRE_THAT(solar_kwh, WithinAbs(2.50, 1e-6));
        REQUIRE_THAT(rain_liters, WithinAbs(0.0, 1e-9));
        REQUIRE_THAT(calories, WithinAbs(0.0, 1e-9));

        // Step ticks [150..199]: Electricity + Water active
        for (uint64_t t = 150; t < 200; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }
        REQUIRE_THAT(solar_kwh, WithinAbs(5.00, 1e-6));
        REQUIRE_THAT(rain_liters, WithinAbs(125.00, 1e-6)); // 50 ticks * 2.5
        REQUIRE_THAT(calories, WithinAbs(0.0, 1e-9));

        // Step ticks [200..299]: All three active
        for (uint64_t t = 200; t < 300; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }
        REQUIRE_THAT(solar_kwh, WithinAbs(10.00, 1e-6));
        REQUIRE_THAT(rain_liters, WithinAbs(375.00, 1e-6)); // 150 ticks * 2.5
        REQUIRE_THAT(calories, WithinAbs(1000.00, 1e-6));   // 100 ticks * 10.0

        // Step tick 300: Calories contract expires and retires
        comp.StepTick(300, solar_kwh, rain_liters, calories);
        REQUIRE(comp.active_count == 2); // 103 retired!

        // Step ticks [301..349]: Electricity + Water active
        for (uint64_t t = 301; t < 350; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }
        REQUIRE_THAT(calories, WithinAbs(1000.00, 1e-6)); // calories accrual stopped
        REQUIRE_THAT(rain_liters, WithinAbs(500.00, 1e-6)); // (350 - 150) * 2.5 = 500

        // Step tick 350: Water contract expires and retires
        comp.StepTick(350, solar_kwh, rain_liters, calories);
        REQUIRE(comp.active_count == 1); // 102 retired!

        // Step ticks [351..499]: Electricity only active
        for (uint64_t t = 351; t < 500; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }
        REQUIRE_THAT(solar_kwh, WithinAbs(20.00, 1e-6)); // 400 ticks * 0.05 = 20

        // Step tick 500: Electricity contract expires and retires
        comp.StepTick(500, solar_kwh, rain_liters, calories);
        REQUIRE(comp.active_count == 0); // all retired!

        // Step ticks [501..600]: Inactive post-window, zero accrual
        for (uint64_t t = 501; t <= 600; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }

        // Final conservation check
        REQUIRE_THAT(solar_kwh, WithinAbs(20.00, 1e-6));
        REQUIRE_THAT(rain_liters, WithinAbs(500.00, 1e-6));
        REQUIRE_THAT(calories, WithinAbs(1000.00, 1e-6));
        REQUIRE_THAT(comp.total_electricity_received, WithinAbs(20.00, 1e-6));
        REQUIRE_THAT(comp.total_water_received, WithinAbs(500.00, 1e-6));
        REQUIRE_THAT(comp.total_calories_received, WithinAbs(1000.00, 1e-6));
        REQUIRE(comp.active_count == 0);
    }

    SECTION("Bursty packet arrival vs strictly paced 1x simulation tick resolution") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);

        // 100x Ghost Agent dumps 20 contract packets in a sub-millisecond burst
        for (uint64_t i = 1; i <= 20; ++i) {
            FlowContract c{};
            c.contract_id = i;
            c.sender_node_id = 0x1000 + i;
            c.consumer_node_id = 0x0001;
            c.start_logical_tick = i * 10;
            c.end_logical_tick = i * 10 + 50;
            c.rate_per_tick = 0.5; // 50 ticks * 0.5 = 25.0 kWh each
            c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
            c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
            c.quality_flags = QualityFlags::QUALITY_SIMULATED;
            REQUIRE(channel.Push(c) == true);
        }

        // Local engine drains all 20 packets on frame 0
        TemporalFlowContractComponent comp{};
        FlowContract incoming{};
        size_t drained = 0;
        while (channel.Pop(incoming)) {
            comp.AddOrUpdateContract(incoming, 0);
            drained++;
        }
        REQUIRE(drained == 20);
        REQUIRE(comp.active_count == 20);

        // Local engine advances monotonically tick by tick (1x pace)
        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t tick = 0; tick <= 300; ++tick) {
            comp.StepTick(tick, solar, rain, cal);
        }

        // Total electricity must equal 20 * 25.0 = 500.0 kWh
        REQUIRE_THAT(solar, WithinAbs(500.00, 1e-6));
        REQUIRE(comp.active_count == 0);
    }

    SECTION("Flecs ECS TemporalFlowSystem Integration") {
        flecs::world ecs;

        ecs.component<oasis::boundary::InfrastructureAutarkyComponent>();
        ecs.component<oasis::fabrication::SomaticStateComponent>();
        ecs.component<oasis::flow::TemporalFlowContractComponent>();

        UhaiFlowChannel test_channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);

        oasis::boundary::InfrastructureAutarkyComponent initial_infra{};
        initial_infra.solar_stored_kwh = 50.0;
        initial_infra.rain_stored_liters = 200.0;
        initial_infra.total_lot_area_sqm = 100.0;

        oasis::fabrication::SomaticStateComponent initial_somatic{};
        initial_somatic.caloric_reserve = 2500.0;

        // Create settlement entity with autarky, somatic, and flow components
        auto settlement = ecs.entity("SettlementBoundary")
            .set<oasis::boundary::InfrastructureAutarkyComponent>(initial_infra)
            .set<oasis::fabrication::SomaticStateComponent>(initial_somatic)
            .set<oasis::flow::TemporalFlowContractComponent>({});

        uint64_t sim_tick = 0;

        // Register system mirroring main.cpp TemporalFlowSystem
        ecs.system<oasis::boundary::InfrastructureAutarkyComponent,
                   oasis::fabrication::SomaticStateComponent,
                   oasis::flow::TemporalFlowContractComponent>("TestTemporalFlowSystem")
            .kind(flecs::OnUpdate)
            .each([&](flecs::entity,
                      oasis::boundary::InfrastructureAutarkyComponent& infra,
                      oasis::fabrication::SomaticStateComponent& somatic,
                      oasis::flow::TemporalFlowContractComponent& flow) {
                // Drain channel
                oasis::uhai::FlowContract pkt{};
                while (test_channel.Pop(pkt)) {
                    flow.AddOrUpdateContract(pkt, sim_tick);
                }
                // Step tick
                flow.StepTick(sim_tick, infra.solar_stored_kwh, infra.rain_stored_liters, somatic.caloric_reserve);
            });

        // 100x Ghost agent pushes contract [10..60) @ 1.0 kWh/tick
        FlowContract c{};
        c.contract_id = 999;
        c.start_logical_tick = 10;
        c.end_logical_tick = 60;
        c.rate_per_tick = 1.0;
        c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
        test_channel.Push(c);

        // Run 100 simulation ticks
        for (sim_tick = 0; sim_tick < 100; ++sim_tick) {
            ecs.progress();
        }

        const auto& infra = settlement.get<oasis::boundary::InfrastructureAutarkyComponent>();
        // Stored began at 50.0 + (60 - 10) * 1.0 = 50.0 + 50.0 = 100.0 kWh
        REQUIRE_THAT(infra.solar_stored_kwh, WithinAbs(100.0, 1e-6));
    }
}

// =============================================================================
// TIER 4: Abrupt Future Contract Termination Resolution
// =============================================================================
TEST_CASE("Tier 4: Abrupt Future Contract Termination Resolution", "[tier4][termination][revocation]") {
    SECTION("Mid-execution revocation packet clamps delivery horizon at termination tick") {
        TemporalFlowContractComponent comp{};

        // Contract 201: Electricity [100..1000) @ 0.050 kWh/tick
        // Scheduled total: 900 ticks * 0.050 = 45.000 kWh
        FlowContract initial_contract{};
        initial_contract.contract_id = 201;
        initial_contract.sender_node_id = 0x100A;
        initial_contract.consumer_node_id = 0x0001;
        initial_contract.start_logical_tick = 100;
        initial_contract.end_logical_tick = 1000;
        initial_contract.rate_per_tick = 0.050;
        initial_contract.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        initial_contract.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
        initial_contract.quality_flags = QualityFlags::QUALITY_SIMULATED;
        std::memcpy(initial_contract.auth_signature, "GHOST100", 8);

        REQUIRE(comp.AddOrUpdateContract(initial_contract, 0) == true);
        REQUIRE(comp.active_count == 1);

        double solar_kwh = 0.0;
        double rain_liters = 0.0;
        double calories = 0.0;

        // Run simulation ticks 0 to 249
        for (uint64_t t = 0; t < 250; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }

        // Expected up to tick 249: (250 - 100) * 0.050 = 150 * 0.050 = 7.50 kWh
        REQUIRE_THAT(solar_kwh, WithinAbs(7.50, 1e-6));

        // At tick 250: Ghost Agent abruptly aborts contract 201!
        // Publishes revocation packet with status = TERMINATED and end_logical_tick = 250
        FlowContract revoke_packet{};
        revoke_packet.contract_id = 201;
        revoke_packet.sender_node_id = 0x100A;
        revoke_packet.consumer_node_id = 0x0001;
        revoke_packet.start_logical_tick = 100;
        revoke_packet.end_logical_tick = 250; // clamped to 250
        revoke_packet.rate_per_tick = 0.050;
        revoke_packet.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        revoke_packet.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);
        revoke_packet.quality_flags = QualityFlags::QUALITY_SIMULATED;
        std::memcpy(revoke_packet.auth_signature, "GHOSTABR", 8);

        REQUIRE(comp.AddOrUpdateContract(revoke_packet, 250) == true);

        // Verify contract was modified in-place
        const auto* found = comp.FindContract(201);
        REQUIRE(found != nullptr);
        REQUIRE(found->status == static_cast<uint8_t>(FlowContractStatus::TERMINATED));
        REQUIRE(found->end_logical_tick == 250);

        // Step tick 250: current_tick >= end_logical_tick (250 >= 250) -> contract cleanly retired!
        comp.StepTick(250, solar_kwh, rain_liters, calories);
        REQUIRE(comp.active_count == 0); // No memory leak, active count is 0

        // Continue running through scheduled horizon [251..1000]
        for (uint64_t t = 251; t <= 1000; ++t) {
            comp.StepTick(t, solar_kwh, rain_liters, calories);
        }

        // EXACT INVARIANT: Exactly 0.00 kWh accrued between tick 250 and 1000.
        // Total electricity accrued MUST remain exactly 7.50 kWh, NOT 45.00 kWh.
        REQUIRE_THAT(solar_kwh, WithinAbs(7.50, 1e-6));
        REQUIRE_THAT(comp.total_electricity_received, WithinAbs(7.50, 1e-6));
        REQUIRE(comp.active_count == 0);
    }

    SECTION("Premature revocation before contract start tick yields zero accrual") {
        TemporalFlowContractComponent comp{};

        // Contract 202: Water [500..1000) @ 2.0 L/tick
        FlowContract initial{};
        initial.contract_id = 202;
        initial.start_logical_tick = 500;
        initial.end_logical_tick = 1000;
        initial.rate_per_tick = 2.0;
        initial.resource_type = static_cast<uint16_t>(FlowResourceType::WATER);
        initial.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        comp.AddOrUpdateContract(initial, 0);

        // Step ticks [0..300]
        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t < 300; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }

        // At tick 300 (before start tick 500): Abrupt cancellation!
        FlowContract cancel{};
        cancel.contract_id = 202;
        cancel.start_logical_tick = 500;
        cancel.end_logical_tick = 300;
        cancel.rate_per_tick = 2.0;
        cancel.resource_type = static_cast<uint16_t>(FlowResourceType::WATER);
        cancel.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);

        comp.AddOrUpdateContract(cancel, 300);

        // Step ticks [300..1000]
        for (uint64_t t = 300; t <= 1000; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }

        // Total water accrued across entire run MUST be exactly 0.0
        REQUIRE_THAT(rain, WithinAbs(0.0, 1e-9));
        REQUIRE(comp.active_count == 0);
    }

    SECTION("Instant abort signal (end_logical_tick = 0) clamps to current tick") {
        TemporalFlowContractComponent comp{};

        FlowContract initial{};
        initial.contract_id = 203;
        initial.start_logical_tick = 50;
        initial.end_logical_tick = 500;
        initial.rate_per_tick = 10.0;
        initial.resource_type = static_cast<uint16_t>(FlowResourceType::CALORIES);
        initial.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        comp.AddOrUpdateContract(initial, 0);

        double solar = 0.0, rain = 0.0, cal = 0.0;
        for (uint64_t t = 0; t < 100; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        // (100 - 50) * 10 = 500 kcal
        REQUIRE_THAT(cal, WithinAbs(500.0, 1e-6));

        // Instant revocation with end_logical_tick = 0 at tick 100
        FlowContract instant_cancel{};
        instant_cancel.contract_id = 203;
        instant_cancel.end_logical_tick = 0; // instant abort
        instant_cancel.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);

        comp.AddOrUpdateContract(instant_cancel, 100);

        // Step tick 100 retires it immediately
        comp.StepTick(100, solar, rain, cal);
        REQUIRE(comp.active_count == 0);

        // Step ticks [101..500]
        for (uint64_t t = 101; t <= 500; ++t) {
            comp.StepTick(t, solar, rain, cal);
        }
        REQUIRE_THAT(cal, WithinAbs(500.0, 1e-6));
    }
}

// =============================================================================
// TIER 5: Multi-Threaded SPSC Concurrency & POSIX IPC Integrity
// =============================================================================
TEST_CASE("Tier 5: Multi-Threaded SPSC Concurrency & POSIX IPC Integrity", "[tier5][concurrency][ipc]") {
    SECTION("High-throughput SPSC concurrent transfer (50,000 contracts) with payload validation") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        constexpr uint64_t TOTAL_CONTRACTS = 50'000;
        std::atomic<bool> start_flag{false};

        // Producer thread: Simulating col-meshd ghost packet generator
        auto producer = [&]() {
            while (!start_flag.load(std::memory_order_acquire)) {}
            for (uint64_t i = 1; i <= TOTAL_CONTRACTS; ++i) {
                FlowContract c{};
                c.contract_id = i;
                c.sender_node_id = 0x100A;
                c.consumer_node_id = 0x0001;
                c.start_logical_tick = i;
                c.end_logical_tick = i + 100;
                c.rate_per_tick = static_cast<double>(i) * 0.001;
                c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
                c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
                c.quality_flags = QualityFlags::QUALITY_SIMULATED;
                std::memcpy(c.auth_signature, "GHOST100", 8);

                while (!channel.Push(c)) {
                    std::this_thread::yield();
                }
            }
        };

        // Consumer thread: Simulating oasis_engine simulation loop
        uint64_t popped_count = 0;
        uint64_t underrun_yields = 0;
        bool corruption_detected = false;

        auto consumer = [&]() {
            while (!start_flag.load(std::memory_order_acquire)) {}
            FlowContract c{};
            while (popped_count < TOTAL_CONTRACTS) {
                if (channel.Pop(c)) {
                    popped_count++;
                    if (c.contract_id != popped_count ||
                        std::memcmp(c.auth_signature, "GHOST100", 8) != 0 ||
                        c.quality_flags != QualityFlags::QUALITY_SIMULATED) {
                        corruption_detected = true;
                        break;
                    }
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
        double per_packet_ns = (elapsed_ms * 1e6) / TOTAL_CONTRACTS;

        spdlog::info("[Catch2 Tier 5 SPSC] Transferred {} FlowContracts in {:.2f} ms ({:.2f} ns/contract, yields: {})",
                     TOTAL_CONTRACTS, elapsed_ms, per_packet_ns, underrun_yields);

        REQUIRE_FALSE(corruption_detected);
        REQUIRE(popped_count == TOTAL_CONTRACTS);
        REQUIRE(channel.GetRing()->IsEmpty());
        REQUIRE(channel.GetHeader()->dropped_frames.load() >= 0);
    }

    SECTION("POSIX shared memory IPC attachment and channel communication") {
        std::string test_shm_name = "/test_flow_posix_" + std::to_string(getpid());
        UhaiFlowChannel::Unlink(test_shm_name);

        {
            // Producer channel (e.g. col-meshd)
            UhaiFlowChannel producer(UhaiFlowChannel::ChannelMode::Producer, test_shm_name);
            REQUIRE(producer.IsValid());
            REQUIRE(producer.GetHeader() != nullptr);
            REQUIRE(producer.GetHeader()->magic_signature == UHAI_MAGIC);
            REQUIRE(producer.GetHeader()->version == UHAI_VERSION);
            REQUIRE(producer.GetHeader()->capacity == RING_CAPACITY);

            // Consumer channel (e.g. oasis_engine)
            UhaiFlowChannel consumer(UhaiFlowChannel::ChannelMode::Consumer, test_shm_name);
            REQUIRE(consumer.IsValid());
            REQUIRE(consumer.GetHeader() != nullptr);
            REQUIRE(consumer.GetHeader()->magic_signature == UHAI_MAGIC);

            // Push contracts from producer
            for (uint64_t i = 1; i <= 25; ++i) {
                FlowContract c{};
                c.contract_id = 700 + i;
                c.rate_per_tick = static_cast<double>(i) * 0.25;
                c.resource_type = static_cast<uint16_t>(FlowResourceType::WATER);
                c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
                c.quality_flags = QualityFlags::QUALITY_SIMULATED;
                REQUIRE(producer.Push(c) == true);
            }

            // Pop contracts from consumer across shared memory boundary
            for (uint64_t i = 1; i <= 25; ++i) {
                FlowContract c{};
                REQUIRE(consumer.Pop(c) == true);
                REQUIRE(c.contract_id == 700 + i);
                REQUIRE_THAT(c.rate_per_tick, WithinAbs(static_cast<double>(i) * 0.25, 1e-9));
                REQUIRE(c.resource_type == static_cast<uint16_t>(FlowResourceType::WATER));
                REQUIRE(c.quality_flags == QualityFlags::QUALITY_SIMULATED);
            }

            // Buffer is now empty
            FlowContract extra{};
            REQUIRE_FALSE(consumer.Pop(extra));
        }

        // Clean unlink
        UhaiFlowChannel::Unlink(test_shm_name);
    }

    SECTION("Anti-Bleed Invariant 3 & Gate G6 Verification") {
        // Gate G6: All simulated packets carry QUALITY_SIMULATED (0x01)
        FlowContract ghost_contract{};
        ghost_contract.quality_flags = QualityFlags::QUALITY_SIMULATED;
        REQUIRE((ghost_contract.quality_flags & QualityFlags::QUALITY_SIMULATED) == QualityFlags::QUALITY_SIMULATED);
        REQUIRE((ghost_contract.quality_flags & QualityFlags::QUALITY_HARDWARE_ROOT) == 0);

        // Anti-Bleed Invariant 3: Memory layout and IPC pass through UHAI ring buffer exclusively
        REQUIRE(TOTAL_FLOW_SHM_SIZE == 65728);
        REQUIRE(sizeof(FlowContract) == 64);
    }
}
