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
#include <sys/wait.h>
#include <signal.h>
#include <spdlog/spdlog.h>

#include "uhai/uhai_ring_buffer.hpp"

using Catch::Matchers::WithinAbs;
using namespace oasis::uhai;
using oasis::flow::TemporalFlowContractComponent;

// =============================================================================
// CHALLENGE 1: SPSC Overrun Accounting & Spin-Push Flakiness Reproduction
// =============================================================================
TEST_CASE("Adversarial: SPSC Overrun Accounting & Spin-Push Flakiness Reproduction", "[adversarial][spsc][accounting]") {
    SECTION("Empirical proof that spin-push retry loop inflates dropped_frames despite 100% delivery") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        constexpr uint64_t TOTAL_CONTRACTS = 20'000;
        std::atomic<bool> start_flag{false};
        std::atomic<uint64_t> spin_retries{0};

        // Producer runs fast and spin-retries with yield when buffer (1024) is full
        auto producer = [&]() {
            while (!start_flag.load(std::memory_order_acquire)) {}
            for (uint64_t i = 1; i <= TOTAL_CONTRACTS; ++i) {
                FlowContract c{};
                c.contract_id = i;
                c.sender_node_id = 0x100A;
                c.consumer_node_id = 0x0001;
                c.start_logical_tick = i;
                c.end_logical_tick = i + 100;
                c.rate_per_tick = 0.05;
                c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
                c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
                c.quality_flags = QualityFlags::QUALITY_SIMULATED;
                std::memcpy(c.auth_signature, "GHOST100", 8);

                while (!channel.Push(c)) {
                    spin_retries.fetch_add(1, std::memory_order_relaxed);
                    std::this_thread::yield();
                }
            }
        };

        // Consumer deliberately lags slightly by batching or sleeping tiny intervals
        uint64_t popped_count = 0;
        bool corruption = false;

        auto consumer = [&]() {
            while (!start_flag.load(std::memory_order_acquire)) {}
            FlowContract c{};
            while (popped_count < TOTAL_CONTRACTS) {
                if (channel.Pop(c)) {
                    popped_count++;
                    if (c.contract_id != popped_count) {
                        corruption = true;
                        break;
                    }
                } else {
                    std::this_thread::yield();
                }
            }
        };

        std::thread t_prod(producer);
        std::thread t_cons(consumer);
        start_flag.store(true, std::memory_order_release);

        t_prod.join();
        t_cons.join();

        REQUIRE_FALSE(corruption);
        REQUIRE(popped_count == TOTAL_CONTRACTS);

        const auto dropped = channel.GetHeader()->dropped_frames.load(std::memory_order_relaxed);
        spdlog::info("[Adversarial Challenge 1] Transferred {}/{} contracts. Spin retries: {}, Ring dropped_frames: {}",
                     popped_count, TOTAL_CONTRACTS, spin_retries.load(), dropped);

        // Crucial empirical finding:
        // Because Push() increments dropped_frames on EVERY full buffer event:
        // if spin_retries > 0, dropped_frames == spin_retries > 0!
        // This proves why Catch2 Tier 5 REQUIRE(dropped_frames == 0) flakes under system scheduling noise!
        CHECK(dropped == spin_retries.load());
    }

    SECTION("Exact dropped frames accounting under sustained producer overrun without retries") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        constexpr uint64_t BURST_ATTEMPTS = 5000;
        uint64_t successful_pushes = 0;
        uint64_t rejected_pushes = 0;

        // Push 5,000 contracts with zero consumer activity
        for (uint64_t i = 1; i <= BURST_ATTEMPTS; ++i) {
            FlowContract c{};
            c.contract_id = i;
            c.rate_per_tick = 1.0;
            if (channel.Push(c)) {
                successful_pushes++;
            } else {
                rejected_pushes++;
            }
        }

        // Exact invariants:
        // 1. Exactly RING_CAPACITY (1024) succeeded
        REQUIRE(successful_pushes == RING_CAPACITY);
        // 2. Exactly BURST_ATTEMPTS - RING_CAPACITY (3976) rejected
        REQUIRE(rejected_pushes == BURST_ATTEMPTS - RING_CAPACITY);
        // 3. Header dropped_frames exactly equals rejected_pushes
        REQUIRE(channel.GetHeader()->dropped_frames.load() == rejected_pushes);
        REQUIRE(channel.GetRing()->Size() == RING_CAPACITY);

        // Now drain all 1024 contracts and verify zero corruption and strict FIFO order
        for (uint64_t i = 1; i <= RING_CAPACITY; ++i) {
            FlowContract out{};
            REQUIRE(channel.Pop(out));
            REQUIRE(out.contract_id == i);
        }

        // Buffer is now completely empty
        REQUIRE(channel.GetRing()->IsEmpty());
        REQUIRE(channel.GetRing()->Size() == 0);
        FlowContract extra{};
        REQUIRE_FALSE(channel.Pop(extra));

        // Pushing another full batch of 1024 now succeeds cleanly
        for (uint64_t i = 1; i <= RING_CAPACITY; ++i) {
            FlowContract c{};
            c.contract_id = 90000 + i;
            REQUIRE(channel.Push(c));
        }
        REQUIRE(channel.GetRing()->Size() == RING_CAPACITY);
        // dropped_frames counter remains untouched
        REQUIRE(channel.GetHeader()->dropped_frames.load() == rejected_pushes);
    }
}

// =============================================================================
// CHALLENGE 2: High-Contention Multi-Threaded SPSC Concurrency (1,000,000 Contracts)
// =============================================================================
TEST_CASE("Adversarial: SPSC Stress Test Under Extreme Saturated Contention (1,000,000 Contracts)", "[adversarial][spsc][million]") {
    SECTION("1,000,000 contracts transferred with zero data races, zero payload corruption, and strict ordering") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        constexpr uint64_t TOTAL_CONTRACTS = 1'000'000;
        std::atomic<bool> start_flag{false};
        std::atomic<bool> producer_done{false};

        auto producer = [&]() {
            while (!start_flag.load(std::memory_order_acquire)) {}
            for (uint64_t i = 1; i <= TOTAL_CONTRACTS; ++i) {
                FlowContract c{};
                c.contract_id = i;
                c.sender_node_id = 0xCAFE0000 | (i & 0xFFFF);
                c.consumer_node_id = 0x0001;
                c.start_logical_tick = i;
                c.end_logical_tick = i + 500;
                c.rate_per_tick = static_cast<double>(i) * 0.0001;
                c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
                c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
                c.quality_flags = QualityFlags::QUALITY_SIMULATED;
                std::memcpy(c.auth_signature, "ADVERSAR", 8);

                while (!channel.Push(c)) {
                    // Backpressure
                    std::this_thread::yield();
                }
            }
            producer_done.store(true, std::memory_order_release);
        };

        uint64_t consumed_count = 0;
        uint64_t corruption_errors = 0;

        auto consumer = [&]() {
            while (!start_flag.load(std::memory_order_acquire)) {}
            FlowContract c{};
            while (consumed_count < TOTAL_CONTRACTS) {
                if (channel.Pop(c)) {
                    consumed_count++;
                    if (c.contract_id != consumed_count ||
                        c.sender_node_id != (0xCAFE0000 | (consumed_count & 0xFFFF)) ||
                        c.consumer_node_id != 0x0001 ||
                        c.start_logical_tick != consumed_count ||
                        c.end_logical_tick != consumed_count + 500 ||
                        std::memcmp(c.auth_signature, "ADVERSAR", 8) != 0 ||
                        c.quality_flags != QualityFlags::QUALITY_SIMULATED) {
                        corruption_errors++;
                    }
                } else {
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

        double elapsed_sec = std::chrono::duration<double>(t1 - t0).count();
        double throughput = static_cast<double>(TOTAL_CONTRACTS) / elapsed_sec;

        spdlog::info("[Adversarial Challenge 2] Transferred {} contracts in {:.3f} s ({:.1f} contracts/s, {:.1f} ns/contract)",
                     TOTAL_CONTRACTS, elapsed_sec, throughput, (elapsed_sec * 1e9) / TOTAL_CONTRACTS);

        REQUIRE(corruption_errors == 0);
        REQUIRE(consumed_count == TOTAL_CONTRACTS);
        REQUIRE(channel.GetRing()->IsEmpty());
    }
}

// =============================================================================
// CHALLENGE 3: Abrupt Termination Semantics Flaws in TemporalFlowContractComponent
// =============================================================================
TEST_CASE("Adversarial: Abrupt Future Termination Semantics & Edge Cases", "[adversarial][termination][flaws]") {
    SECTION("Flaw 1: StepTick ignores status == TERMINATED on directly inserted terminated contracts") {
        TemporalFlowContractComponent comp{};

        // Contract inserted with status = TERMINATED and future end_logical_tick
        FlowContract terminated_contract{};
        terminated_contract.contract_id = 901;
        terminated_contract.start_logical_tick = 10;
        terminated_contract.end_logical_tick = 50;
        terminated_contract.rate_per_tick = 2.0;
        terminated_contract.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        terminated_contract.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);

        // AddOrUpdateContract accepts it if incoming.end_logical_tick > current_tick
        bool added = comp.AddOrUpdateContract(terminated_contract, 0);
        REQUIRE(added);
        REQUIRE(comp.active_count == 1);

        double solar = 0.0, rain = 0.0, calories = 0.0;
        // Step tick 20: current_tick is in [10..50)
        comp.StepTick(20, solar, rain, calories);

        // VULNERABILITY DEMONSTRATION:
        // Even though status == TERMINATED, StepTick does NOT check status and accrues solar power!
        // This confirms that StepTick relies exclusively on end_logical_tick, not status!
        spdlog::warn("[Adversarial Challenge 3] Solar accrued on contract with status=TERMINATED: {} kWh", solar);
        CHECK(solar == 2.0); // Documents the semantic behavior
    }

    SECTION("Flaw 2: Revocation packet with end_logical_tick >= original end does not clamp delivery") {
        TemporalFlowContractComponent comp{};

        FlowContract initial{};
        initial.contract_id = 902;
        initial.start_logical_tick = 10;
        initial.end_logical_tick = 100;
        initial.rate_per_tick = 1.0;
        initial.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        initial.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        comp.AddOrUpdateContract(initial, 0);

        // Step ticks [0..20)
        double solar = 0.0, rain = 0.0, calories = 0.0;
        for (uint64_t t = 0; t < 20; ++t) {
            comp.StepTick(t, solar, rain, calories);
        }
        REQUIRE_THAT(solar, WithinAbs(10.0, 1e-9)); // 10 ticks in [10..20)

        // At tick 20, sender sends revocation with status = TERMINATED,
        // but sender left end_logical_tick = 100 (the original scheduled end)
        FlowContract revoke = initial;
        revoke.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);
        revoke.end_logical_tick = 100; // Not strictly less than 100, and not 0, and not <= current_tick (20)

        comp.AddOrUpdateContract(revoke, 20);

        // Inspect contract state: status is TERMINATED and end_logical_tick is clamped to current_tick (20)!
        const auto* c = comp.FindContract(902);
        REQUIRE(c != nullptr);
        REQUIRE(c->status == static_cast<uint8_t>(FlowContractStatus::TERMINATED));
        // end_logical_tick was clamped to current_tick (20)!
        CHECK(c->end_logical_tick == 20);

        // Step ticks [20..50)
        for (uint64_t t = 20; t < 50; ++t) {
            comp.StepTick(t, solar, rain, calories);
        }

        // HARDENED BEHAVIOR CONFIRMATION:
        // Resource stopped accruing after clamped revocation!
        spdlog::info("[Adversarial Challenge 3] Solar accrued after clamped revocation: {} kWh (expected 10.0, got {})",
                     solar, solar);
        CHECK(solar == 10.0);
    }

    SECTION("Flaw 3: Out-of-order packet arrival resurrects TERMINATED contract to ACTIVE") {
        TemporalFlowContractComponent comp{};

        // Out-of-order scenario: Revocation arrives first at tick 10
        FlowContract term{};
        term.contract_id = 903;
        term.start_logical_tick = 5;
        term.end_logical_tick = 15;
        term.rate_per_tick = 3.0;
        term.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        term.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);

        comp.AddOrUpdateContract(term, 10);
        REQUIRE(comp.active_count == 1);
        REQUIRE(comp.contracts[0].status == static_cast<uint8_t>(FlowContractStatus::TERMINATED));

        // Delayed initial ACTIVE packet arrives at tick 11
        FlowContract active{};
        active.contract_id = 903;
        active.start_logical_tick = 5;
        active.end_logical_tick = 500;
        active.rate_per_tick = 3.0;
        active.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
        active.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);

        comp.AddOrUpdateContract(active, 11);

        // Anti-resurrection guard rejects resurrection to ACTIVE; remains TERMINATED
        REQUIRE(comp.contracts[0].status == static_cast<uint8_t>(FlowContractStatus::TERMINATED));
        REQUIRE(comp.contracts[0].end_logical_tick == 15);
    }
}

// =============================================================================
// CHALLENGE 4: Rapid Emit & Abrupt Future Termination Contention Harness
// =============================================================================
TEST_CASE("Adversarial: Concurrent Rapid Emission & Abrupt Future Termination Under Contention", "[adversarial][concurrency][revocation_stress]") {
    SECTION("Concurrent producer emitting 5,000 contracts with 1,000 asynchronous revocations vs engine consumer") {
        UhaiFlowChannel channel(UhaiFlowChannel::ChannelMode::VirtualInMemory);
        REQUIRE(channel.IsValid());

        constexpr uint64_t CONTRACT_COUNT = 5'000;
        constexpr uint64_t REVOCATIONS_COUNT = 1'000;
        std::atomic<bool> start_flag{false};
        std::atomic<bool> producer_done{false};

        // Producer emits contracts and interleaved revocations
        auto producer = [&]() {
            while (!start_flag.load(std::memory_order_acquire)) {}

            for (uint64_t i = 1; i <= CONTRACT_COUNT; ++i) {
                // Emit active contract
                FlowContract c{};
                c.contract_id = i;
                c.sender_node_id = 0x100A;
                c.consumer_node_id = 0x0001;
                c.start_logical_tick = i;
                c.end_logical_tick = i + 200;
                c.rate_per_tick = 0.010;
                c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
                c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
                c.quality_flags = QualityFlags::QUALITY_SIMULATED;
                std::memcpy(c.auth_signature, "GHOST100", 8);

                while (!channel.Push(c)) {
                    std::this_thread::yield();
                }

                // Interleave abrupt revocation for a previously emitted contract
                if (i % 5 == 0 && i >= 10) {
                    uint64_t target_id = i - 5;
                    FlowContract revoke{};
                    revoke.contract_id = target_id;
                    revoke.sender_node_id = 0x100A;
                    revoke.consumer_node_id = 0x0001;
                    revoke.start_logical_tick = target_id;
                    revoke.end_logical_tick = target_id + 50; // Abruptly cut horizon from +200 to +50
                    revoke.rate_per_tick = 0.010;
                    revoke.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
                    revoke.status = static_cast<uint8_t>(FlowContractStatus::TERMINATED);
                    revoke.quality_flags = QualityFlags::QUALITY_SIMULATED;
                    std::memcpy(revoke.auth_signature, "GHOSTABR", 8);

                    while (!channel.Push(revoke)) {
                        std::this_thread::yield();
                    }
                }
            }
            producer_done.store(true, std::memory_order_release);
        };

        // Consumer simulation engine running tick-by-tick
        TemporalFlowContractComponent flow_comp{};
        double total_solar = 0.0;
        double total_rain = 0.0;
        double total_cal = 0.0;
        uint64_t max_active_observed = 0;
        uint64_t engine_tick = 0;

        auto consumer = [&]() {
            while (!start_flag.load(std::memory_order_acquire)) {}

            while (!producer_done.load(std::memory_order_acquire) || !channel.GetRing()->IsEmpty()) {
                engine_tick++;

                // 1. Drain incoming contracts from ring buffer
                FlowContract incoming{};
                while (channel.Pop(incoming)) {
                    flow_comp.AddOrUpdateContract(incoming, engine_tick);
                }

                if (flow_comp.active_count > max_active_observed) {
                    max_active_observed = flow_comp.active_count;
                }

                // 2. Step simulation tick
                flow_comp.StepTick(engine_tick, total_solar, total_rain, total_cal);

                // Small yield to let producer make progress
                if (engine_tick % 10 == 0) {
                    std::this_thread::yield();
                }
            }

            // Drain any remaining active contracts to completion
            while (flow_comp.active_count > 0 && engine_tick < 20'000) {
                engine_tick++;
                flow_comp.StepTick(engine_tick, total_solar, total_rain, total_cal);
            }
        };

        std::thread t_prod(producer);
        std::thread t_cons(consumer);

        start_flag.store(true, std::memory_order_release);
        t_prod.join();
        t_cons.join();

        spdlog::info("[Adversarial Challenge 4] Stress test finished at tick {}. Max concurrent active contracts: {} (limit: {}). Total solar: {:.3f} kWh",
                     engine_tick, max_active_observed, TemporalFlowContractComponent::MAX_CONTRACTS, total_solar);

        // Verification invariants:
        // 1. Active contracts never exceeded array bounds (MAX_CONTRACTS = 64)
        REQUIRE(max_active_observed <= TemporalFlowContractComponent::MAX_CONTRACTS);
        // 2. All contracts eventually retired without lingering memory leak
        REQUIRE(flow_comp.active_count == 0);
        // 3. Finite, non-negative, non-NaN resource accumulation
        REQUIRE(total_solar > 0.0);
        REQUIRE_FALSE(std::isnan(total_solar));
        REQUIRE_FALSE(std::isinf(total_solar));
    }
}

// =============================================================================
// CHALLENGE 5: POSIX Multi-Process Crash & Abrupt Exit Survivability
// =============================================================================
TEST_CASE("Adversarial: Multi-Process POSIX IPC Abrupt Termination & Recovery", "[adversarial][posix][crash]") {
    SECTION("Abrupt SIGKILL of child producer during active writes leaves shared ring buffer safe and recoverable") {
        std::string crash_shm_name = "/test_uhai_crash_" + std::to_string(getpid());
        UhaiFlowChannel::Unlink(crash_shm_name);

        // Create POSIX shared memory from parent
        {
            UhaiFlowChannel parent_channel(UhaiFlowChannel::ChannelMode::Consumer, crash_shm_name);
            REQUIRE(parent_channel.IsValid());

            pid_t pid = fork();
            REQUIRE(pid >= 0);

            if (pid == 0) {
                // Child process: aggressively write contracts to shared memory
                UhaiFlowChannel child_producer(UhaiFlowChannel::ChannelMode::Producer, crash_shm_name);
                if (!child_producer.IsValid()) {
                    _exit(1);
                }

                uint64_t counter = 1;
                while (true) {
                    FlowContract c{};
                    c.contract_id = counter++;
                    c.sender_node_id = 0xBEEF;
                    c.rate_per_tick = 1.0;
                    c.resource_type = static_cast<uint16_t>(FlowResourceType::ELECTRICITY);
                    c.status = static_cast<uint8_t>(FlowContractStatus::ACTIVE);
                    c.quality_flags = QualityFlags::QUALITY_SIMULATED;
                    std::memcpy(c.auth_signature, "CHILDPRD", 8);

                    child_producer.Push(c);
                }
                _exit(0);
            }

            // Parent process: consume some contracts
            uint64_t parent_consumed = 0;
            FlowContract c{};
            while (parent_consumed < 500) {
                if (parent_channel.Pop(c)) {
                    parent_consumed++;
                    REQUIRE(c.sender_node_id == 0xBEEF);
                    REQUIRE(std::memcmp(c.auth_signature, "CHILDPRD", 8) == 0);
                } else {
                    std::this_thread::yield();
                }
            }

            // Abruptly kill the producer child process mid-write with SIGKILL!
            kill(pid, SIGKILL);
            int status = 0;
            waitpid(pid, &status, 0);

            // Verify parent process did not crash, can still safely read all remaining valid queued packets
            while (parent_channel.Pop(c)) {
                parent_consumed++;
                REQUIRE(c.sender_node_id == 0xBEEF);
                REQUIRE(std::memcmp(c.auth_signature, "CHILDPRD", 8) == 0);
            }

            // Invariants after crash:
            // 1. write_index and read_index are consistent
            const auto* hdr = parent_channel.GetHeader();
            REQUIRE(hdr != nullptr);
            REQUIRE(hdr->read_index.load() <= hdr->write_index.load());
            REQUIRE(parent_channel.GetRing()->IsEmpty());

            spdlog::info("[Adversarial Challenge 5] Successfully survived abrupt SIGKILL of producer child process. Consumed: {}", parent_consumed);
        }

        // Clean unlink
        UhaiFlowChannel::Unlink(crash_shm_name);
    }
}
