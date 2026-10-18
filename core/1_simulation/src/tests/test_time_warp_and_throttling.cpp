#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <glm/glm.hpp>
#include <cmath>
#include <cstdint>

#include "core/time_warp_controller.hpp"
#include "core/homeostasis.hpp"
#include "core/fabrication.hpp"
#include "core/bpmn_scheduler.hpp"
#include "uhai/uhai_ring_buffer.hpp"

using Catch::Approx;
using namespace oasis::time;
using namespace oasis::homeostasis;
using namespace oasis::fabrication;
using namespace oasis::orchestrator;
using namespace oasis::uhai;

namespace {
struct MockPsychState {
    float focus{1.0f};
    float stress{0.05f};
};
} // namespace

// ============================================================================
// STORY-10.1: Standardized Time-Warp Multiplier Clamping ({0, 1, 2, 5, 10})
// ============================================================================

TEST_CASE("STORY-10.1: Time-Warp Multiplier Clamping & Sub-Step Determinism", "[story-10.1][time-warp]") {
    TimeWarpState warp{};
    REQUIRE(warp.multiplier == 1);

    SECTION("SetTimeWarpMultiplier snaps strictly to discrete {0, 1, 2, 5, 10} set") {
        SetTimeWarpMultiplier(warp, 0);
        REQUIRE(warp.multiplier == 0);

        SetTimeWarpMultiplier(warp, 1);
        REQUIRE(warp.multiplier == 1);

        SetTimeWarpMultiplier(warp, 2);
        REQUIRE(warp.multiplier == 2);

        SetTimeWarpMultiplier(warp, 3);
        REQUIRE(warp.multiplier == 5);

        SetTimeWarpMultiplier(warp, 5);
        REQUIRE(warp.multiplier == 5);

        SetTimeWarpMultiplier(warp, 10);
        REQUIRE(warp.multiplier == 10);

        SetTimeWarpMultiplier(warp, 42);
        REQUIRE(warp.multiplier == 10);

        SetTimeWarpMultiplier(warp, 100);
        REQUIRE(warp.multiplier == 10);
    }

    SECTION("1,000 logical ticks at 10x (100 frames x 10 sub-steps) matches 1,000 frames at 1x with zero NaN/Inf") {
        constexpr float kFixedDt = 1.0f / 60.0f;
        const glm::vec3 world_pos(4.0f, 1.0f, 4.0f); // Un-sheltered outdoor position

        // Run A: 1,000 frames at 1x
        SomaticStateComponent somatic_1x{};
        MockPsychState psych_1x{};
        uint64_t tick_1x = 0;

        for (int frame = 0; frame < 1000; ++frame) {
            tick_1x++;
            const float ambient = ResolveVoxelAmbientTemperature(world_pos, tick_1x);
            StepHomeostasis(somatic_1x, psych_1x, ambient, kFixedDt);
            REQUIRE_FALSE(std::isnan(somatic_1x.caloric_reserve));
            REQUIRE_FALSE(std::isinf(somatic_1x.caloric_reserve));
            REQUIRE_FALSE(std::isnan(somatic_1x.focus));
        }

        // Run B: 100 frames at 10x (1,000 sub-steps total)
        SomaticStateComponent somatic_10x{};
        MockPsychState psych_10x{};
        uint64_t tick_10x = 0;

        for (int frame = 0; frame < 100; ++frame) {
            for (int step = 0; step < 10; ++step) {
                tick_10x++;
                const float ambient = ResolveVoxelAmbientTemperature(world_pos, tick_10x);
                StepHomeostasis(somatic_10x, psych_10x, ambient, kFixedDt);
                REQUIRE_FALSE(std::isnan(somatic_10x.caloric_reserve));
                REQUIRE_FALSE(std::isinf(somatic_10x.caloric_reserve));
                REQUIRE_FALSE(std::isnan(somatic_10x.focus));
            }
        }

        REQUIRE(tick_1x == 1000);
        REQUIRE(tick_10x == 1000);
        REQUIRE(somatic_10x.caloric_reserve == Approx(somatic_1x.caloric_reserve).margin(1e-12));
        REQUIRE(somatic_10x.focus == Approx(somatic_1x.focus).margin(1e-7f));
        REQUIRE(somatic_10x.ambient_temp_c == Approx(somatic_1x.ambient_temp_c).margin(1e-7f));
    }
}

TEST_CASE("STORY-10.1: Once-Per-Frame UHAI Telemetry Aggregation at 10x", "[story-10.1][telemetry]") {
    UhaiTelemetryChannel channel(UhaiTelemetryChannel::ChannelMode::VirtualInMemory, "/test_time_warp_uhai_s14");
    REQUIRE(channel.IsValid());
    auto* ring = channel.GetRing();
    REQUIRE(ring != nullptr);

    TimeWarpState warp{};
    SetTimeWarpMultiplier(warp, 10);
    REQUIRE(warp.multiplier == 10);

    SomaticStateComponent somatic{};
    MockPsychState psych{};
    uint64_t logical_tick = 0;

    // Simulate 60 render frames at 10x (600 logical ticks)
    // Aggregation invariant: push exactly 1 TelemetrySample per 60 Hz render frame (after sub-step loop)
    for (uint64_t render_frame = 1; render_frame <= 60; ++render_frame) {
        for (uint32_t step = 0; step < warp.multiplier; ++step) {
            logical_tick++;
            const float ambient = CalculateDiurnalTemperature(logical_tick);
            StepHomeostasis(somatic, psych, ambient, 1.0f / 60.0f);
        }

        TelemetrySample sample{};
        sample.frame_count = logical_tick;
        sample.timestamp_ns = render_frame * 16'666'667ULL;
        sample.fps = 60.0f;
        sample.founder_stress = psych.stress;
        sample.memory_slot_count = 16;
        sample.biome_id = 0;
        sample.quality_flag = QUALITY_SIMULATED;

        REQUIRE(channel.Push(sample));
    }

    REQUIRE(logical_tick == 600);
    REQUIRE(ring->Size() == 60);
    REQUIRE(ring->DroppedCount() == 0);
}

TEST_CASE("STORY-10.1: Emergency Warp Auto-Downshift Brake", "[story-10.1][emergency-brake]") {
    SECTION("Nominal vitals at 10x do not trigger emergency brake") {
        TimeWarpState warp{};
        SetTimeWarpMultiplier(warp, 10);
        REQUIRE_FALSE(EvaluateEmergencyWarpBrake(warp, 1800.0, 0.85f, 0.20f, false));
        REQUIRE(warp.multiplier == 10);
        REQUIRE_FALSE(warp.emergency_brake_triggered);
    }

    SECTION("Caloric deficit (< 800 kcal) downshifts 10x -> 1x immediately") {
        TimeWarpState warp{};
        SetTimeWarpMultiplier(warp, 10);
        REQUIRE(EvaluateEmergencyWarpBrake(warp, 799.5, 0.85f, 0.20f, false));
        REQUIRE(warp.multiplier == 1);
        REQUIRE(warp.emergency_brake_triggered);
        REQUIRE_FALSE(warp.emergency_reason.empty());
    }

    SECTION("Dehydration (< 0.25) downshifts 5x -> 1x immediately") {
        TimeWarpState warp{};
        SetTimeWarpMultiplier(warp, 5);
        REQUIRE(EvaluateEmergencyWarpBrake(warp, 2000.0, 0.24f, 0.20f, false));
        REQUIRE(warp.multiplier == 1);
        REQUIRE(warp.emergency_brake_triggered);
    }

    SECTION("Immobilizing psychological stress (>= 0.95) downshifts 10x -> 1x immediately") {
        TimeWarpState warp{};
        SetTimeWarpMultiplier(warp, 10);
        REQUIRE(EvaluateEmergencyWarpBrake(warp, 2000.0, 0.85f, 0.95f, false));
        REQUIRE(warp.multiplier == 1);
        REQUIRE(warp.emergency_brake_triggered);
    }

    SECTION("Arriving migration wave downshifts 10x -> 1x immediately") {
        TimeWarpState warp{};
        SetTimeWarpMultiplier(warp, 10);
        REQUIRE(EvaluateEmergencyWarpBrake(warp, 2000.0, 0.85f, 0.20f, true));
        REQUIRE(warp.multiplier == 1);
        REQUIRE(warp.emergency_brake_triggered);
    }
}

// ============================================================================
// STORY-10.2: Adaptive Log Throttling & --headless-ci CLI Isolation
// ============================================================================

TEST_CASE("STORY-10.2: Adaptive Log Throttling & --headless-ci CLI Isolation", "[story-10.2][cli]") {
    SECTION("ShouldEmitThrottledLog emits at tick 1 and every 300 ticks, bypassing only on critical events") {
        TimeWarpState warp{};
        REQUIRE(ShouldEmitThrottledLog(warp, 1, false));
        REQUIRE_FALSE(ShouldEmitThrottledLog(warp, 2, false));
        REQUIRE_FALSE(ShouldEmitThrottledLog(warp, 150, false));
        REQUIRE_FALSE(ShouldEmitThrottledLog(warp, 299, false));
        REQUIRE(ShouldEmitThrottledLog(warp, 300, false));
        REQUIRE_FALSE(ShouldEmitThrottledLog(warp, 301, false));
        REQUIRE_FALSE(ShouldEmitThrottledLog(warp, 599, false));
        REQUIRE(ShouldEmitThrottledLog(warp, 600, false));

        // Critical event always emits regardless of tick interval
        REQUIRE(ShouldEmitThrottledLog(warp, 605, true));
    }

    SECTION("ParseEngineCliOptions defaults headless_ci to false unless --headless-ci is passed") {
        const char* default_argv[] = {"oasis_engine"};
        EngineCliOptions default_opts = ParseEngineCliOptions(1, default_argv);
        REQUIRE_FALSE(default_opts.headless_ci);
        REQUIRE(default_opts.max_ticks == 60);

        const char* ci_argv[] = {"oasis_engine", "--headless-ci", "--max-ticks=120"};
        EngineCliOptions ci_opts = ParseEngineCliOptions(3, ci_argv);
        REQUIRE(ci_opts.headless_ci);
        REQUIRE(ci_opts.max_ticks == 120);
    }
}

// ============================================================================
// SPRINT 14: Asynchronous Node Proxies, BPMN Bounties & Temporal Scheduling
// ============================================================================

TEST_CASE("SPRINT-14: Asynchronous Node Proxies & BPMN Bounty Fulfillment", "[sprint-14][bpmn-bounty]") {
    CollectiveNetworkState net = CreateDefaultCollectiveNetwork();
    oasis::chill::GenesisCampStockComponent stock{};
    REQUIRE(net.peer_nodes.size() == 3);
    REQUIRE(net.collective_value_tokens == 25u);

    // Step async node proxies and verify deficits exist before bounty fulfillment
    StepAsyncNodeProxies(net, 1200ULL);
    REQUIRE(net.peer_nodes[0].exergy_balance_kwh < 0.0);
    REQUIRE(net.peer_nodes[1].water_balance_l < 0.0);
    REQUIRE(net.peer_nodes[2].caloric_balance_kcal < 0.0);

    // Fulfill Task 1 (Roof Solar for Maya Node 01 - Lot 404)
    const uint32_t initial_cob = stock.cob_earth_lumps;
    REQUIRE(FulfillBpmnBountyForTask(net, 1u, stock));
    REQUIRE(net.peer_nodes[0].bounty_fulfilled);
    REQUIRE(net.peer_nodes[0].exergy_balance_kwh > 0.0);
    REQUIRE(net.collective_value_tokens == 40u); // 25 + 15 VT
    REQUIRE(stock.cob_earth_lumps == initial_cob + 2u);

    // Fulfill Task 2 (Backyard Swale for Kael Node 02 - Lot 406)
    const uint32_t initial_seeds = stock.seed_packs_remaining;
    REQUIRE(FulfillBpmnBountyForTask(net, 2u, stock));
    REQUIRE(net.peer_nodes[1].bounty_fulfilled);
    REQUIRE(net.peer_nodes[1].water_balance_l > 0.0);
    REQUIRE(net.collective_value_tokens == 52u); // 40 + 12 VT
    REQUIRE(stock.seed_packs_remaining == initial_seeds + 2u);

    // Fulfill Task 3 (Sovereign Kitchen Meals for Soren Node 03 - Lot 408)
    const uint32_t initial_kits = stock.solar_kits_remaining;
    REQUIRE(FulfillBpmnBountyForTask(net, 3u, stock));
    REQUIRE(net.peer_nodes[2].bounty_fulfilled);
    REQUIRE(net.peer_nodes[2].caloric_balance_kcal > 0.0);
    REQUIRE(net.collective_value_tokens == 72u); // 52 + 20 VT
    REQUIRE(stock.solar_kits_remaining == initial_kits + 1u);
}

TEST_CASE("SPRINT-14: Multi-Day Temporal Workflow Scheduler & Advance L5 Gate Locking", "[sprint-14][temporal-scheduler]") {
    TemporalScheduleState sched = CreateDefaultTemporalSchedule();
    REQUIRE(sched.days.size() == 4);

    // Update Day 1 to Task 3 (Harvest & Cook) and Day 2 to Task 1 (Mount Roof Solar)
    REQUIRE(UpdateScheduleSlotTask(sched, 1u, 3u));
    REQUIRE(UpdateScheduleSlotTask(sched, 2u, 1u));
    REQUIRE(sched.days[0].task_id == 3u);
    REQUIRE(sched.days[1].task_id == 1u);

    // Lock L5 gates in advance for a Ring R1 Steward
    REQUIRE(LockScheduleL5Gates(sched, 1u));
    REQUIRE(sched.days[0].l5_gate_locked);
    REQUIRE(sched.days[0].bpmn_stage == BpmnStage::L3_ESCROW_LOCK);

    // At Day 1 (sim_tick = 100), StepTemporalSchedule dispatches Day 1's task (3u) when Founder is idle
    REQUIRE(ComputeSimDayFromTick(100ULL) == 1u);
    const uint16_t dispatched_d1 = StepTemporalSchedule(sched, 100ULL, true);
    REQUIRE(dispatched_d1 == 3u);
    REQUIRE(sched.days[0].dispatched_to_founder);
    REQUIRE(sched.days[0].bpmn_stage == BpmnStage::L1B_ACTUATION);

    // Completing Task 3 transitions Day 1 slot to L3_SETTLED
    REQUIRE(CompleteScheduledTask(sched, 3u));
    REQUIRE(sched.days[0].completed);
    REQUIRE(sched.days[0].bpmn_stage == BpmnStage::L3_SETTLED);

    // Advance clock to Day 2 (sim_tick = 22000 -> total_minutes = 360 + 1100 = 1460 -> Day 2)
    REQUIRE(ComputeSimDayFromTick(22000ULL) == 2u);
    const uint16_t dispatched_d2 = StepTemporalSchedule(sched, 22000ULL, true);
    REQUIRE(dispatched_d2 == 1u);
    REQUIRE(sched.days[1].dispatched_to_founder);
    REQUIRE(sched.days[1].bpmn_stage == BpmnStage::L1B_ACTUATION);

    // Verify canonical Layer 6 Intent Lifecycle badges and Task 5 (mesh:RestIntent) scheduling
    REQUIRE(std::string(FormatIntentLifecycleBadge(BpmnStage::L6_INGEST)) == "[PENDING]");
    REQUIRE(std::string(FormatIntentLifecycleBadge(BpmnStage::L3_ESCROW_LOCK)) == "[ESCROWED]");
    REQUIRE(std::string(FormatIntentLifecycleBadge(BpmnStage::L1B_ACTUATION)) == "[ACTUATING]");
    REQUIRE(std::string(FormatIntentLifecycleBadge(BpmnStage::L3_SETTLED)) == "[FULFILLED]");
    REQUIRE(std::string(FormatIntentLifecycleBadge(BpmnStage::REJECTED)) == "[REJECTED]");

    REQUIRE(UpdateScheduleSlotTask(sched, 4u, 5u));
    REQUIRE(sched.days[3].task_id == 5u);
    REQUIRE(std::string(sched.days[3].required_l5_vc) == "None_Inalienable_Rest_Right");
}

