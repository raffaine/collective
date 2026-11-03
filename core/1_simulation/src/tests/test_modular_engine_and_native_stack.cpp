#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "engine/clock/simulation_clock.hpp"
#include "engine/content/content_registry.hpp"
#include "engine/logistics/hauling_physics.hpp"
#include "engine/domains/soil_physics.hpp"
#include "engine/domains/thermal_vessel.hpp"
#include "engine/world/map_provider.hpp"
#include "engine/ecs/affordance_system.hpp"
#include "engine/ai/utility_autonomy.hpp"
#include "engine/l2_bridge/virtual_pin_bus.hpp"
#include "engine/l2_bridge/unmodeled_step_adapter.hpp"

#include "shared_stack_runtime.hpp"
#include "swarm_engine.hpp"

namespace {

[[nodiscard]] std::filesystem::path FindRepoAssetPath(std::string_view relative_from_repo_root) {
    const std::array<std::string_view, 6> prefixes = {
        "",
        "../",
        "../../",
        "../../../",
        "../../../../",
        "../../../../../"
    };
    for (const auto prefix : prefixes) {
        std::filesystem::path candidate = std::string(prefix) + std::string(relative_from_repo_root);
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
    }
    return std::filesystem::path(relative_from_repo_root);
}

[[nodiscard]] std::string ReadTextFile(const std::filesystem::path& p) {
    std::ifstream ifs(p, std::ios::in | std::ios::binary);
    if (!ifs.is_open()) {
        return {};
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

[[nodiscard]] bool HasAffordanceId(
    const std::vector<oasis::engine::AffordanceDef>& list,
    std::string_view affordance_id
) noexcept {
    for (const auto& a : list) {
        if (a.affordance_id == affordance_id) {
            return true;
        }
    }
    return false;
}

[[nodiscard]] const oasis::stack::L4WorkflowInstance* FindWorkflowById(
    const oasis::stack::SharedStackRuntime& runtime,
    std::string_view wf_id
) noexcept {
    for (const auto& wf : runtime.GetWorkflows()) {
        if (wf.workflow_id == wf_id) {
            return &wf;
        }
    }
    return nullptr;
}

[[nodiscard]] const oasis::stack::L4TaskItem* FindTaskById(
    const oasis::stack::SharedStackRuntime& runtime,
    std::string_view task_id
) noexcept {
    for (const auto& wf : runtime.GetWorkflows()) {
        for (const auto& t : wf.tasks) {
            if (t.task_id == task_id) {
                return &t;
            }
        }
    }
    return nullptr;
}

void VerifySuburbanLotStrataAndCleanSlate(
    oasis::engine::WorldGrid& grid,
    const std::vector<oasis::engine::PlacedEntitySeed>& entities
) {
    using namespace oasis::engine;

    // 1. Subsurface y = -2: undisturbed clay subsoil across parcel and street
    REQUIRE(grid.GetVoxel(0, -2, 5) == STRATA_CLAY_SUBSOIL);
    REQUIRE(grid.GetVoxel(12, -2, 6) == STRATA_CLAY_SUBSOIL);
    REQUIRE(grid.GetVoxel(20, -2, 8) == STRATA_CLAY_SUBSOIL);

    // 2. Subsurface y = -1: compacted gravel pad under road/driveway/house, concrete perimeter footings, yard topsoil
    REQUIRE(grid.GetVoxel(0, -1, 5) == STRATA_COMPACTED_GRAVEL_PAD);
    REQUIRE(grid.GetVoxel(7, -1, 2) == STRATA_COMPACTED_GRAVEL_PAD);
    REQUIRE(grid.GetVoxel(12, -1, 6) == STRATA_COMPACTED_GRAVEL_PAD);
    REQUIRE(grid.GetVoxel(9, -1, 2) == STRATA_CONCRETE_FOOTING_SLAB);
    REQUIRE(grid.GetVoxel(17, -1, 10) == STRATA_CONCRETE_FOOTING_SLAB);
    REQUIRE(grid.GetVoxel(8, -1, 10) == STRATA_CONCRETE_FOOTING_SLAB);
    REQUIRE(grid.GetVoxel(20, -1, 8) == STRATA_TOPSOIL_LOAM);

    // Paved vs unpaved path classification for wheeled hauling equipment
    REQUIRE(grid.IsPathPavedOrGraded(0, 5));
    REQUIRE(grid.IsPathPavedOrGraded(7, 2));
    REQUIRE_FALSE(grid.IsPathPavedOrGraded(20, 8));

    // 3. Clean-slate Day-1 backyard: zero pre-built garden beds, coops, kilns, or pre-installed L2 sensors
    for (int32_t z = 4; z <= 12; ++z) {
        for (int32_t x = 19; x <= 22; ++x) {
            REQUIRE(grid.GetVoxel(x, 1, z) == MAT_VOXEL_AIR);
        }
    }

    uint32_t pre_built_backyard_facilities = 0u;
    uint32_t pre_installed_l2_sensors      = 0u;
    bool     has_bucket                    = false;
    bool     has_watering_can              = false;
    bool     has_wheelbarrow               = false;
    bool     has_hand_dolly                = false;
    bool     has_tiffin_carrier            = false;

    for (const auto& e : entities) {
        if (e.has_l2_sensor) {
            ++pre_installed_l2_sensors;
        }
        if (e.archetype_id.find("garden_bed") != std::string::npos ||
            e.archetype_id.find("poultry_coop") != std::string::npos ||
            e.archetype_id.find("kiln") != std::string::npos ||
            e.archetype_id.find("compost") != std::string::npos) {
            ++pre_built_backyard_facilities;
        }
        if (e.initial_item_id == "bucket")         has_bucket = true;
        if (e.initial_item_id == "watering_can")   has_watering_can = true;
        if (e.initial_item_id == "wheelbarrow")    has_wheelbarrow = true;
        if (e.initial_item_id == "hand_dolly")     has_hand_dolly = true;
        if (e.initial_item_id == "tiffin_carrier") has_tiffin_carrier = true;
    }

    REQUIRE(pre_built_backyard_facilities == 0u);
    REQUIRE(pre_installed_l2_sensors == 0u);
    REQUIRE(has_bucket);
    REQUIRE(has_watering_can);
    REQUIRE(has_wheelbarrow);
    REQUIRE(has_hand_dolly);
    REQUIRE(has_tiffin_carrier);

    // 4. ExcavateTile yields GRANULAR_PARTICULATE spoil across topsoil, clay subsoil, and compacted gravel
    const ExcavationResult topsoil_spoil = grid.ExcavateTile(20, -1, 8);
    REQUIRE(topsoil_spoil.excavated);
    REQUIRE(topsoil_spoil.previous_material == STRATA_TOPSOIL_LOAM);
    REQUIRE(topsoil_spoil.yielded_item_id == "item.topsoil_loam");
    REQUIRE(topsoil_spoil.quantity_units == 110u);
    REQUIRE(topsoil_spoil.total_mass_g == 110000u);
    REQUIRE(topsoil_spoil.matter_phase == MatterPhase::GRANULAR_PARTICULATE);
    REQUIRE(topsoil_spoil.recommended_tool_class == HaulingToolClass::WHEELBARROW);
    REQUIRE(grid.GetVoxel(20, -1, 8) == MAT_VOXEL_AIR);

    const ExcavationResult clay_spoil = grid.ExcavateTile(20, -2, 8);
    REQUIRE(clay_spoil.excavated);
    REQUIRE(clay_spoil.previous_material == STRATA_CLAY_SUBSOIL);
    REQUIRE(clay_spoil.yielded_item_id == "item.clay_subsoil");
    REQUIRE(clay_spoil.quantity_units == 120u);
    REQUIRE(clay_spoil.total_mass_g == 120000u);
    REQUIRE(clay_spoil.matter_phase == MatterPhase::GRANULAR_PARTICULATE);
    REQUIRE(grid.GetVoxel(20, -2, 8) == MAT_VOXEL_AIR);

    const ExcavationResult gravel_spoil = grid.ExcavateTile(7, -1, 2);
    REQUIRE(gravel_spoil.excavated);
    REQUIRE(gravel_spoil.previous_material == STRATA_COMPACTED_GRAVEL_PAD);
    REQUIRE(gravel_spoil.yielded_item_id == "item.crushed_gravel");
    REQUIRE(gravel_spoil.quantity_units == 140u);
    REQUIRE(gravel_spoil.total_mass_g == 140000u);
    REQUIRE(gravel_spoil.matter_phase == MatterPhase::GRANULAR_PARTICULATE);
    REQUIRE(grid.GetVoxel(7, -1, 2) == MAT_VOXEL_AIR);

    // Poured structural concrete footings are non-excavatable by hand shovel
    const ExcavationResult footing_attempt = grid.ExcavateTile(9, -1, 2);
    REQUIRE_FALSE(footing_attempt.excavated);
    REQUIRE(grid.GetVoxel(9, -1, 2) == STRATA_CONCRETE_FOOTING_SLAB);
}

} // namespace

// =============================================================================
// PILLAR 1: Grounded SimulationClock & Calm Autonomy Dwell Pacing
// =============================================================================

TEST_CASE("Pillar 1: Grounded SimulationClock 12x pacing, warp snapping & calm 20-40 min dwell autonomy",
          "[modular_engine][clock][autonomy]") {
    using namespace oasis::engine;

    SECTION("SimulationClock enforces 12x sim-to-real ratio at 1x and snaps warp to {1, 2, 5, 10}") {
        SimulationClock clock{};
        REQUIRE(clock.BaseSimSecondsPerRealSecond() == 12u);
        REQUIRE(clock.WarpMultiplier() == 1u);
        REQUIRE(clock.EffectiveSimSecondsPerRealSecond() == 12u);
        REQUIRE(clock.FormatCalendar() == "Day 1, 08:00");
        REQUIRE(clock.CurrentCalendar().is_daylight == 1u);

        // 1 real second = 12 simulated seconds at 1x
        REQUIRE(clock.AdvanceRealDelta(1000u) == 12000u);
        REQUIRE(clock.FormatCalendar(true) == "Day 1, 08:00:12");

        // Remaining 59 real seconds => 1 full real minute = 12 simulated minutes
        REQUIRE(clock.AdvanceRealDelta(59000u) == 708000u);
        REQUIRE(clock.FormatCalendar() == "Day 1, 08:12");

        // 2 real hours (7,200,000 real ms) at 1x = 86,400,000 sim ms = 1 full 24h simulated day
        SimulationClock day_clock{kDefaultSimSecondsPerRealSecond, 0ULL};
        REQUIRE(day_clock.AdvanceRealDelta(7200000u) == static_cast<uint32_t>(kSimMsPerDay));
        REQUIRE(day_clock.CurrentCalendar().day == 2u);
        REQUIRE(day_clock.CurrentCalendar().hour == 0u);

        // Discrete warp snapping: {1, 2, 5, 10}
        REQUIRE(SimulationClock::SnapSupportedMultiplier(0u) == 1u);
        REQUIRE(SimulationClock::SnapSupportedMultiplier(1u) == 1u);
        REQUIRE(SimulationClock::SnapSupportedMultiplier(2u) == 2u);
        REQUIRE(SimulationClock::SnapSupportedMultiplier(3u) == 5u);
        REQUIRE(SimulationClock::SnapSupportedMultiplier(5u) == 5u);
        REQUIRE(SimulationClock::SnapSupportedMultiplier(6u) == 10u);
        REQUIRE(SimulationClock::SnapSupportedMultiplier(42u) == 10u);

        clock.SetWarpMultiplier(2u);
        REQUIRE(clock.WarpMultiplier() == 2u);
        REQUIRE(clock.EffectiveSimSecondsPerRealSecond() == 24u);
        REQUIRE(clock.AdvanceRealDelta(1000u) == 24000u);

        clock.SetWarpMultiplier(4u); // snaps to 5x
        REQUIRE(clock.WarpMultiplier() == 5u);
        REQUIRE(clock.EffectiveSimSecondsPerRealSecond() == 60u);
        REQUIRE(clock.AdvanceRealDelta(1000u) == 60000u);

        clock.SetWarpMultiplier(9u); // snaps to 10x
        REQUIRE(clock.WarpMultiplier() == 10u);
        REQUIRE(clock.EffectiveSimSecondsPerRealSecond() == 120u);
        REQUIRE(clock.AdvanceRealDelta(1000u) == 120000u);

        // Pause preserves configured multiplier while freezing simulated time
        clock.SetWarpSpeed(WarpSpeed::PAUSED);
        REQUIRE(clock.IsPaused());
        REQUIRE(clock.WarpMultiplier() == 0u);
        REQUIRE(clock.ConfiguredWarpMultiplier() == 10u);
        REQUIRE(clock.AdvanceRealDelta(5000u) == 0u);

        // Emergency downshift unpauses to 1x
        clock.EmergencyDownshiftTo1x();
        REQUIRE_FALSE(clock.IsPaused());
        REQUIRE(clock.WarpMultiplier() == 1u);

        // Background-tab frame clamp caps real delta at 250 ms (3000 sim ms at 1x)
        REQUIRE(clock.AdvanceClampedFrameDelta(10000u) == 3000u);
    }

    SECTION("UtilityAutonomySystem enforces 20-40 sim-minute minimum dwell and autonomous chicken egg-laying") {
        ContentRegistry registry{};
        registry.BootstrapCoreCatalog();

        AgentDrivesComponent human_drives{};
        human_drives.hunger_bp        = 8600u;
        human_drives.hydration_bp     = 8800u;
        human_drives.energy_bp        = 8500u;
        human_drives.leisure_bp       = 5200u;
        human_drives.social_bp        = 7400u;
        human_drives.introspection_bp = 6900u;

        AutonomyStateComponent human_state{};
        std::string thought;

        const AutonomousActivity initial_act = UtilityAutonomySystem::StepHumanAutonomy(
            human_drives, human_state, 1000u, &registry, &thought
        );
        REQUIRE(initial_act != AutonomousActivity::IDLE);
        REQUIRE(human_state.transition_count == 1u);
        REQUIRE(human_state.dwell_timer.duration_sim_ms >= kMinCalmDwellSimMs);
        REQUIRE(human_state.dwell_timer.duration_sim_ms <= kMaxCalmDwellSimMs);
        REQUIRE(human_state.dwell_timer.min_dwell_sim_ms == human_state.dwell_timer.duration_sim_ms);
        REQUIRE_FALSE(thought.empty());

        // Step 1,140 ticks of 1 sim-second (19 sim-minutes < 20 sim-minute minimum dwell):
        // even if another non-emergency drive dips lower, calm dwell strictly prevents frantic task switching!
        human_drives.social_bp = 3900u;
        for (uint32_t s = 0u; s < 1140u; ++s) {
            const AutonomousActivity cur = UtilityAutonomySystem::StepHumanAutonomy(
                human_drives, human_state, 1000u, &registry
            );
            REQUIRE(cur == initial_act);
            REQUIRE(human_state.transition_count == 1u);
        }

        // Advance past the remaining dwell window and verify clean transition to the next calm activity
        const uint32_t rem_ms = human_state.dwell_timer.RemainingSimMs();
        REQUIRE(rem_ms > 0u);
        (void)UtilityAutonomySystem::StepHumanAutonomy(human_drives, human_state, rem_ms + 1000u, &registry);
        REQUIRE(human_state.transition_count == 2u);
        REQUIRE(human_state.dwell_timer.duration_sim_ms >= kMinCalmDwellSimMs);
        REQUIRE(human_state.dwell_timer.duration_sim_ms <= kMaxCalmDwellSimMs);

        // Autonomous chicken foraging (20-35 sim-min dwell) and egg-laying into coop InventoryComponent
        AgentDrivesComponent hen_drives{};
        hen_drives.species      = AgentSpecies::CHICKEN;
        hen_drives.hunger_bp    = 8500u;
        hen_drives.hydration_bp = 8800u;
        hen_drives.energy_bp    = 8600u;
        hen_drives.egg_cycle_bp = 0u;

        AutonomyStateComponent hen_state{};
        InventoryComponent coop_inventory{};

        const AutonomousActivity hen_initial = UtilityAutonomySystem::StepChickenAutonomy(
            hen_drives, hen_state, coop_inventory, 1000u, "fresh_egg"
        );
        REQUIRE(hen_initial == AutonomousActivity::CHICKEN_FORAGING);
        REQUIRE(hen_state.dwell_timer.duration_sim_ms >= kChickenMinForageSimMs);
        REQUIRE(hen_state.dwell_timer.duration_sim_ms <= kChickenMaxForageSimMs);

        // Step 6 simulated hours in 1-sim-minute increments (~5 sim-hours matures 1 pasture egg)
        bool observed_laying_state = false;
        for (uint32_t m = 0u; m < 360u; ++m) {
            const AutonomousActivity hen_act = UtilityAutonomySystem::StepChickenAutonomy(
                hen_drives, hen_state, coop_inventory, kSimMsPerMinute, "fresh_egg"
            );
            if (hen_act == AutonomousActivity::CHICKEN_LAYING_EGG) {
                observed_laying_state = true;
            }
        }
        REQUIRE(observed_laying_state);
        REQUIRE(hen_state.eggs_laid_count >= 1u);
        REQUIRE(coop_inventory.CountItem("fresh_egg") == hen_state.eggs_laid_count);
    }
}

// =============================================================================
// PILLAR 2: Matter-Phase & Mechanical-Advantage Hauling Physics
// =============================================================================

TEST_CASE("Pillar 2: Matter-phase & mechanical-advantage hauling laws across LIQUID, GRANULAR, and heavy SOLID",
          "[modular_engine][hauling]") {
    using namespace oasis::engine;

    ContentRegistry registry{};
    registry.BootstrapCoreCatalog();

    // Register canonical pea soup liquid and exact heavy assemblies (74kg drum, 115kg coop, 800kg pallet)
    registry.RegisterItem(ItemDef{2800000ULL, "item.pea_soup", "Split-Pea Soup (500ml)", 540u, 500u, 650u, MatterPhase::LIQUID});
    registry.RegisterItem(ItemDef{6000000ULL, "item.compost_drum_74kg", "Rotating Compost Drum (74kg)", 74000u, 320000u, 0u, MatterPhase::DISCRETE_SOLID});
    registry.RegisterItem(ItemDef{9500000ULL, "item.poultry_coop_115kg", "Timber Poultry Coop (115kg)", 115000u, 480000u, 0u, MatterPhase::DISCRETE_SOLID});
    registry.RegisterItem(ItemDef{24000000ULL, "item.masonry_pallet_800kg", "Palletized Masonry (800kg)", 800000u, 580000u, 0u, MatterPhase::DISCRETE_SOLID});

    const HaulingToolDef* bare_hands     = registry.FindHaulingTool("bare_hands");
    const HaulingToolDef* bucket         = registry.FindHaulingTool("bucket");
    const HaulingToolDef* watering_can   = registry.FindHaulingTool("watering_can");
    const HaulingToolDef* tiffin_carrier = registry.FindHaulingTool("tiffin_carrier");
    const HaulingToolDef* wheelbarrow    = registry.FindHaulingTool("wheelbarrow");
    const HaulingToolDef* hand_dolly     = registry.FindHaulingTool("hand_dolly");
    const HaulingToolDef* cargo_etrike   = registry.FindHaulingTool("cargo_etrike");
    const HaulingToolDef* forklift       = registry.FindHaulingTool("forklift");

    REQUIRE(bare_hands != nullptr);
    REQUIRE(bucket != nullptr);
    REQUIRE(watering_can != nullptr);
    REQUIRE(tiffin_carrier != nullptr);
    REQUIRE(wheelbarrow != nullptr);
    REQUIRE(hand_dolly != nullptr);
    REQUIRE(cargo_etrike != nullptr);
    REQUIRE(forklift != nullptr);

    SECTION("LIQUID (water, pea soup): 0 ml bare-hand, succeeds with bucket / watering_can / tiffin_carrier") {
        const ItemDef* water    = registry.FindItem("water_potable");
        const ItemDef* pea_soup = registry.FindItem("item.pea_soup");
        REQUIRE(water != nullptr);
        REQUIRE(pea_soup != nullptr);

        const HaulEvaluation water_bare = EvaluateHaulPlan(*water, 10u, nullptr);
        REQUIRE(water_bare.feasibility == HaulFeasibility::IMPOSSIBLE_NEEDS_LIQUID_VESSEL);
        REQUIRE_FALSE(IsHaulPossible(water_bare.feasibility));
        REQUIRE(water_bare.max_volume_per_trip_ml == 0u);
        REQUIRE(water_bare.trips_required == 0u);

        const HaulEvaluation soup_bare = EvaluateHaulPlan(*pea_soup, 4u, bare_hands);
        REQUIRE(soup_bare.feasibility == HaulFeasibility::IMPOSSIBLE_NEEDS_LIQUID_VESSEL);
        REQUIRE(soup_bare.max_volume_per_trip_ml == 0u);

        // Non-liquid open-frame tools (e.g. hand_dolly) also cannot carry uncontained liquid
        const HaulEvaluation water_dolly = EvaluateHaulPlan(*water, 5u, hand_dolly);
        REQUIRE(water_dolly.feasibility == HaulFeasibility::IMPOSSIBLE_NEEDS_LIQUID_VESSEL);

        // Succeeds with bucket (12 L liquid cap), watering_can (10 L), and tiffin_carrier (3 L)
        const HaulEvaluation water_bucket = EvaluateHaulPlan(*water, 24u, bucket);
        REQUIRE(water_bucket.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(water_bucket.max_volume_per_trip_ml == 12000u);
        REQUIRE(water_bucket.trips_required == 2u);
        REQUIRE(water_bucket.effective_speed_bp == 9000u);

        const HaulEvaluation water_can = EvaluateHaulPlan(*water, 15u, watering_can);
        REQUIRE(water_can.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(water_can.max_volume_per_trip_ml == 10000u);
        REQUIRE(water_can.trips_required == 2u);
        REQUIRE(water_can.effective_speed_bp == 9500u);

        const HaulEvaluation soup_tiffin = EvaluateHaulPlan(*pea_soup, 6u, tiffin_carrier);
        REQUIRE(soup_tiffin.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(soup_tiffin.max_volume_per_trip_ml == 3000u);
        REQUIRE(soup_tiffin.trips_required == 1u);
        REQUIRE(soup_tiffin.effective_speed_bp == 10000u);
    }

    SECTION("GRANULAR_PARTICULATE (dirt, gravel, compost): 500 g bare-hand (3500 bp) vs 20 kg bucket (9000 bp) vs 120 kg wheelbarrow (10000 bp)") {
        const ItemDef* dirt    = registry.FindItem("excavated_dirt");
        const ItemDef* gravel  = registry.FindItem("crushed_gravel");
        const ItemDef* compost = registry.FindItem("compost_humus");
        REQUIRE(dirt != nullptr);
        REQUIRE(gravel != nullptr);
        REQUIRE(compost != nullptr);

        // 120 kg (120 units * 1,000 g) excavated dirt spoil
        const HaulEvaluation dirt_bare = EvaluateHaulPlan(*dirt, 120u, bare_hands);
        REQUIRE(dirt_bare.feasibility == HaulFeasibility::VERY_SLOW_BAREHAND_GRANULAR);
        REQUIRE(dirt_bare.max_payload_per_trip_g == 500u);
        REQUIRE(dirt_bare.effective_speed_bp == 3500u);
        REQUIRE(dirt_bare.trips_required == 240u);

        const HaulEvaluation dirt_bucket = EvaluateHaulPlan(*dirt, 120u, bucket);
        REQUIRE(dirt_bucket.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(dirt_bucket.max_payload_per_trip_g == 20000u);
        REQUIRE(dirt_bucket.effective_speed_bp == 9000u);
        REQUIRE(dirt_bucket.trips_required == 6u);

        const HaulEvaluation dirt_wheelbarrow = EvaluateHaulPlan(*dirt, 120u, wheelbarrow);
        REQUIRE(dirt_wheelbarrow.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(dirt_wheelbarrow.max_payload_per_trip_g == 120000u);
        REQUIRE(dirt_wheelbarrow.effective_speed_bp == 10000u);
        REQUIRE(dirt_wheelbarrow.trips_required == 1u);

        // Verify simulated round-trip haul duration scales dramatically across carrier classes
        const uint32_t ms_bare        = EstimateTotalHaulDurationSimMs(dirt_bare, 20u);
        const uint32_t ms_bucket      = EstimateTotalHaulDurationSimMs(dirt_bucket, 20u);
        const uint32_t ms_wheelbarrow = EstimateTotalHaulDurationSimMs(dirt_wheelbarrow, 20u);
        REQUIRE(ms_wheelbarrow < ms_bucket);
        REQUIRE(ms_bucket < ms_bare);

        // Gravel (75 * 1600g = 120 kg) & Compost (50 * 800g = 40 kg)
        REQUIRE(EvaluateHaulPlan(*gravel, 75u, wheelbarrow).trips_required == 1u);
        REQUIRE(EvaluateHaulPlan(*compost, 50u, bare_hands).trips_required == 80u);
        REQUIRE(EvaluateHaulPlan(*compost, 50u, bucket).trips_required == 2u);
    }

    SECTION("Heavy DISCRETE_SOLID (74 kg drum, 115 kg coop, 800 kg pallet): rejected bare-hand (>35 kg), hauled by mechanical carriers") {
        const ItemDef* drum_74kg    = registry.FindItem("item.compost_drum_74kg");
        const ItemDef* coop_115kg   = registry.FindItem("item.poultry_coop_115kg");
        const ItemDef* kiln_165kg   = registry.FindItem("cast_iron_kiln_core");
        const ItemDef* pallet_800kg = registry.FindItem("item.masonry_pallet_800kg");
        REQUIRE(drum_74kg != nullptr);
        REQUIRE(coop_115kg != nullptr);
        REQUIRE(kiln_165kg != nullptr);
        REQUIRE(pallet_800kg != nullptr);

        // All > 35 kg discrete assemblies are strictly rejected bare-handed
        REQUIRE(EvaluateHaulPlan(*drum_74kg, 1u, nullptr).feasibility == HaulFeasibility::IMPOSSIBLE_TOO_HEAVY_BAREHAND);
        REQUIRE(EvaluateHaulPlan(*coop_115kg, 1u, bare_hands).feasibility == HaulFeasibility::IMPOSSIBLE_TOO_HEAVY_BAREHAND);
        REQUIRE(EvaluateHaulPlan(*kiln_165kg, 1u, bare_hands).feasibility == HaulFeasibility::IMPOSSIBLE_TOO_HEAVY_BAREHAND);
        REQUIRE(EvaluateHaulPlan(*pallet_800kg, 1u, bare_hands).feasibility == HaulFeasibility::IMPOSSIBLE_TOO_HEAVY_BAREHAND);

        // 74 kg compost drum & 115 kg poultry coop fit wheelbarrow (120 kg cap, unpaved backyard ok) and dolly/e-trike/forklift
        const HaulEvaluation drum_wb = EvaluateHaulPlan(*drum_74kg, 1u, wheelbarrow, false);
        REQUIRE(drum_wb.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(drum_wb.trips_required == 1u);
        REQUIRE(drum_wb.effective_speed_bp == 10000u);

        const HaulEvaluation coop_wb = EvaluateHaulPlan(*coop_115kg, 1u, wheelbarrow, false);
        REQUIRE(coop_wb.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(coop_wb.trips_required == 1u);

        // Hand dolly requires paved/graded path, succeeds on concrete driveway (200 kg cap)
        REQUIRE(EvaluateHaulPlan(*coop_115kg, 1u, hand_dolly, false).feasibility ==
                HaulFeasibility::IMPOSSIBLE_NEEDS_PAVED_OR_GRADED_PATH);
        REQUIRE(EvaluateHaulPlan(*coop_115kg, 1u, hand_dolly, true).feasibility == HaulFeasibility::FEASIBLE);

        // 165 kg cast-iron kiln core exceeds wheelbarrow (120 kg), hauled cleanly by hand_dolly (200 kg) and cargo_etrike (250 kg)
        REQUIRE(EvaluateHaulPlan(*kiln_165kg, 1u, wheelbarrow, true).feasibility ==
                HaulFeasibility::IMPOSSIBLE_EXCEEDS_TOOL_CAPACITY);
        const HaulEvaluation kiln_dolly = EvaluateHaulPlan(*kiln_165kg, 1u, hand_dolly, true);
        REQUIRE(kiln_dolly.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(kiln_dolly.trips_required == 1u);
        REQUIRE(kiln_dolly.effective_speed_bp == 9000u);

        const HaulEvaluation kiln_etrike = EvaluateHaulPlan(*kiln_165kg, 1u, cargo_etrike, true);
        REQUIRE(kiln_etrike.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(kiln_etrike.trips_required == 1u);
        REQUIRE(kiln_etrike.effective_speed_bp == 13500u);

        // 800 kg pallet exceeds wheelbarrow, hand_dolly, and cargo_etrike, hauled cleanly by forklift (1500 kg cap)
        REQUIRE(EvaluateHaulPlan(*pallet_800kg, 1u, cargo_etrike, true).feasibility ==
                HaulFeasibility::IMPOSSIBLE_EXCEEDS_TOOL_CAPACITY);
        const HaulEvaluation pallet_forklift = EvaluateHaulPlan(*pallet_800kg, 1u, forklift, true);
        REQUIRE(pallet_forklift.feasibility == HaulFeasibility::FEASIBLE);
        REQUIRE(pallet_forklift.trips_required == 1u);
        REQUIRE(pallet_forklift.effective_speed_bp == 12000u);
    }
}

// =============================================================================
// PILLAR 3: Loadable Static & Procedural IMapProvider + Subsurface Earthworks
// =============================================================================

TEST_CASE("Pillar 3: StaticJsonMapProvider (suburban_lot_402.json) & ProceduralSuburbanEarthworksGenerator strata",
          "[modular_engine][world][strata]") {
    using namespace oasis::engine;

    SECTION("StaticJsonMapProvider loads suburban_lot_402.json with full subsurface strata & clean-slate Day-1 backyard") {
        const std::filesystem::path lot_json_path =
            FindRepoAssetPath("core/1_simulation/data/environments/suburban_lot_402.json");
        REQUIRE(std::filesystem::exists(lot_json_path));

        std::unique_ptr<IMapProvider> static_provider =
            std::make_unique<StaticJsonMapProvider>(lot_json_path.string());
        WorldGrid grid{};
        std::vector<PlacedEntitySeed> entities{};

        REQUIRE(static_provider->PopulateWorld(grid, entities));
        REQUIRE(entities.size() >= 15u);
        VerifySuburbanLotStrataAndCleanSlate(grid, entities);
    }

    SECTION("ProceduralSuburbanEarthworksGenerator builds matching civil-engineering strata & clean-slate Day-1 lot") {
        std::unique_ptr<IMapProvider> proc_provider =
            std::make_unique<ProceduralSuburbanEarthworksGenerator>(402);
        WorldGrid grid{};
        std::vector<PlacedEntitySeed> entities{};

        REQUIRE(proc_provider->PopulateWorld(grid, entities));
        REQUIRE(entities.size() >= 15u);
        VerifySuburbanLotStrataAndCleanSlate(grid, entities);
    }
}

// =============================================================================
// PILLAR 4: L2-Driven Physical Domains, Epistemic VirtualPinBus & UnmodeledStepAdapter
// =============================================================================

TEST_CASE("Pillar 4: Biochar/legume SoilPhysics, Bigelow ThermalVessel, epistemic VirtualPinBus & UnmodeledStepAdapter",
          "[modular_engine][domains][l2_bridge]") {
    using namespace oasis::engine;

    SECTION("SoilCellRegisters biochar/Terra Preta moisture & cold-snap heat retention + Rhizobium legume N-fixation") {
        SoilCellRegisters unamended{};
        unamended.moisture_vwc_bp   = 2600u;
        unamended.soil_temp_mk      = 291150u; // 18.0 C
        unamended.organic_carbon_bp = 200u;    // Degraded suburban mineral soil
        unamended.nitrogen_ppm      = 35u;
        unamended.ph_centi          = 620u;

        SoilCellRegisters terra_preta = unamended;
        AmendSoilWithBiochar(terra_preta, 20000u); // 20 kg charged biochar amendment
        REQUIRE(terra_preta.organic_carbon_bp > unamended.organic_carbon_bp);
        REQUIRE(terra_preta.ph_centi > unamended.ph_centi);
        REQUIRE(ComputeSoilFieldCapacityBp(terra_preta.organic_carbon_bp) >
                ComputeSoilFieldCapacityBp(unamended.organic_carbon_bp));

        // Irrigate both with 6.5 L (6500 ml) and expose to a 6.0 C (279150 mK) cold snap over 2 sim-hours (7200 s)
        StepSoilPhysics(unamended, 279150u, 6500u, 7200u);
        StepSoilPhysics(terra_preta, 279150u, 6500u, 7200u);

        REQUIRE(terra_preta.moisture_vwc_bp > unamended.moisture_vwc_bp);
        REQUIRE(terra_preta.soil_temp_mk > unamended.soil_temp_mk);

        // Symbiotic Rhizobium legume N-fixation increases mineral nitrogen_ppm while growing irrigated peas,
        // whereas an identical non-legume crop depletes mineral nitrogen_ppm
        terra_preta.moisture_vwc_bp  = 3800u;   // Irrigated Terra Preta root zone (250 L cell)
        terra_preta.crop_planted     = 1u;
        terra_preta.soil_temp_mk     = 293150u; // 20.0 C optimal nodulation
        SoilCellRegisters non_legume = terra_preta;
        non_legume.is_legume_fixing_n  = 0u;
        terra_preta.is_legume_fixing_n = 1u;

        const uint32_t n_before      = terra_preta.nitrogen_ppm;
        const uint32_t growth_before = terra_preta.crop_growth_bp;

        StepSoilPhysics(terra_preta, 293150u, 25000u, 3600u); // 1 sim-hour with 25 L drip irrigation
        StepSoilPhysics(non_legume, 293150u, 25000u, 3600u);

        REQUIRE(terra_preta.moisture_vwc_bp >= 1800u);
        REQUIRE(terra_preta.nitrogen_ppm > n_before);
        REQUIRE(non_legume.nitrogen_ppm < n_before);
        REQUIRE(terra_preta.nitrogen_ppm > non_legume.nitrogen_ppm);
        REQUIRE(terra_preta.crop_growth_bp > growth_before);
    }

    SECTION("ThermalVesselRegisters Bigelow pasteurization hold (>= 347150 mK)") {
        ThermalVesselRegisters kettle{};
        kettle.core_temp_mk          = 293150u;  // 20.0 C start
        kettle.liquid_volume_ml      = 4000u;    // 4.0 L split-pea soup / milk batch
        kettle.heater_power_mw       = 2400000u; // 2.4 kW induction hearth
        kettle.target_cutoff_temp_mk = 350150u;  // 77.0 C thermostat target (>= 347150 mK Bigelow threshold)
        kettle.lid_closed            = 1u;

        // Heat & hold for 10 simulated minutes (600 s)
        StepThermalVessel(kettle, 293150u, 600u);

        REQUIRE(kettle.core_temp_mk >= kBigelowPasteurizationTempMk);
        REQUIRE(kettle.core_temp_mk <= kettle.target_cutoff_temp_mk);
        REQUIRE(kettle.pasteurization_hold_s >= kTargetPasteurizationHoldSec);
        REQUIRE(kettle.lethality_equivalent_ms >= (kTargetPasteurizationHoldSec * 1000u));
        REQUIRE(kettle.pasteurization_complete == 1u);
    }

    SECTION("VirtualPinBus denies telemetry when has_l2_sensor == false, requiring InspectEntityManual, then actuates L2 pins") {
        ContentRegistry registry{};
        registry.BootstrapCoreCatalog();
        const EntityArchetypeDef* bed_arch = registry.FindArchetype("archetype.soil_garden_bed");
        REQUIRE(bed_arch != nullptr);

        SoilCellRegisters bed_soil{};
        bed_soil.moisture_vwc_bp   = 2450u;
        bed_soil.organic_carbon_bp = 3200u;
        bed_soil.nitrogen_ppm      = 54u;

        SensorAttachmentComponent sensor_attachment{};
        sensor_attachment.has_l2_sensor    = false;
        sensor_attachment.actuator_enabled = true;

        VirtualPinBus pin_bus{};
        constexpr uint64_t kBedUid = 40201ULL;
        pin_bus.BindSoilEntity(kBedUid, &sensor_attachment, &bed_soil);

        // Without L2 hardware attached, automated L2 reads & valve actuation are strictly denied!
        const SoilPinReadResult denied_read = pin_bus.ReadSoilEntity(kBedUid);
        REQUIRE(denied_read.status == PinAccessStatus::DENIED_NO_L2_SENSOR);
        REQUIRE(denied_read.registers.moisture_vwc_bp == 0u);
        REQUIRE(pin_bus.WriteEntityIrrigationValve(kBedUid, true) == PinAccessStatus::DENIED_NO_L2_SENSOR);

        // Steward must walk up and use manual diegetic InspectEntityManual
        const ManualInspectionReport manual_rep =
            InspectEntityManual(*bed_arch, nullptr, &bed_soil, nullptr, nullptr, &sensor_attachment);
        REQUIRE_FALSE(manual_rep.has_live_l2_telemetry);
        bool found_manual_moisture = false;
        for (const auto& prop : manual_rep.revealed_properties) {
            if (prop.key == "moisture_vwc_bp" && prop.value == 2450) {
                found_manual_moisture = true;
            }
        }
        REQUIRE(found_manual_moisture);

        // Attach L2 hardware (soil_probe_v1 + irrigation_valve_v1): telemetry & valve actuation now succeed!
        sensor_attachment.has_l2_sensor  = true;
        sensor_attachment.sensor_node_id = 1u;

        const SoilPinReadResult live_read = pin_bus.ReadSoilEntity(kBedUid);
        REQUIRE(live_read.status == PinAccessStatus::OK);
        REQUIRE(live_read.registers.moisture_vwc_bp == 2450u);

        REQUIRE(pin_bus.WriteEntityIrrigationValve(kBedUid, true) == PinAccessStatus::OK);
        REQUIRE(pin_bus.IsIrrigationValveOpen(kBedUid));

        // Stepping bound domains with open L2 drip valve injects water and raises VWC
        pin_bus.StepBoundDomains(293150u, 120u);
        REQUIRE(bed_soil.moisture_vwc_bp > 2450u);
    }

    SECTION("UnmodeledStepAdapter resolves Rhizobium slurry coating, <1400 bp pod moisture check & player observation prompts") {
        UnmodeledStepAdapter adapter{};

        // 1. Modeled physical domain step routes directly to soil_physics
        const StepAdapterResolution modeled = adapter.ResolveStep(
            "col://workflow/soil/irrigate_keyline_bed",
            {"moisture_vwc_bp", "irrigation_valve_open"}
        );
        REQUIRE(modeled.mode == ResolutionMode::MODELED_PHYSICAL_DOMAIN);
        REQUIRE(modeled.resolved_domain_id == "soil_physics");

        // 2. Unmodeled Rhizobium seed slurry coating -> default registers + timed station work
        StepAdapterResolution rhizobium_res = adapter.ResolveStep(
            "col://workflow/agronomy/rhizobium_seed_slurry_coating",
            {"inoculant_CFU_per_ml", "slurry_adhesion_bp"},
            180u,
            false
        );
        REQUIRE(rhizobium_res.mode == ResolutionMode::DEFAULT_VALUES_AND_TIMED_STATION_WORK);
        REQUIRE(rhizobium_res.default_registers.size() == 2u);
        REQUIRE(rhizobium_res.default_registers[0].register_key == "inoculant_CFU_per_ml");
        REQUIRE(rhizobium_res.default_registers[0].value == 100000000LL);
        REQUIRE(rhizobium_res.default_registers[1].register_key == "slurry_adhesion_bp");
        REQUIRE(rhizobium_res.default_registers[1].value == 9400LL);
        REQUIRE(rhizobium_res.station_timer.is_active == 1u);
        REQUIRE_FALSE(rhizobium_res.station_timer.Advance(90000u));
        REQUIRE(rhizobium_res.station_timer.Advance(90000u));
        REQUIRE(rhizobium_res.station_timer.is_completed == 1u);

        // 3. Unmodeled < 1400 bp pod dry-down moisture check -> default pod_moisture_bp < 1400
        const StepAdapterResolution pod_check_res = adapter.ResolveStep(
            "col://workflow/agronomy/shell_peas_drydown_test",
            {"pod_moisture_bp", "grain_moisture_bp"},
            240u,
            false
        );
        REQUIRE(pod_check_res.mode == ResolutionMode::DEFAULT_VALUES_AND_TIMED_STATION_WORK);
        REQUIRE(pod_check_res.default_registers.size() == 2u);
        REQUIRE(pod_check_res.default_registers[0].value < 1400LL);
        REQUIRE(pod_check_res.default_registers[1].value < 1400LL);

        // 4. Qualitative sensory check -> diegetic player observation prompt
        const StepAdapterResolution prompt_res = adapter.ResolveStep(
            "col://workflow/culinary/sensory_check_pea_soup_simmer",
            {"pod_moisture_bp"},
            120u,
            true
        );
        REQUIRE(prompt_res.mode == ResolutionMode::PLAYER_OBSERVATION_PROMPT);
        REQUIRE_FALSE(prompt_res.player_prompt_text.empty());

        PlayerObservationPrompt popped{};
        REQUIRE(adapter.PopNextObservationPrompt(popped));
        REQUIRE(popped.step_action_uri == "col://workflow/culinary/sensory_check_pea_soup_simmer");
    }
}

// =============================================================================
// PILLAR 5: Mod Bundle Hot-Loading (ContentRegistry::LoadModBundleFromJson)
// =============================================================================

TEST_CASE("Pillar 5: Community mod JSON bundle hot-loads custom item, archetype & affordance with zero C++ changes",
          "[modular_engine][modding][affordances]") {
    using namespace oasis::engine;

    ContentRegistry registry{};
    registry.BootstrapCoreCatalog();

    // Also verify on-disk core_catalog.json loads cleanly into ContentRegistry
    const std::filesystem::path core_catalog_path =
        FindRepoAssetPath("core/1_simulation/data/content/core_catalog.json");
    if (std::filesystem::exists(core_catalog_path)) {
        const std::string catalog_json = ReadTextFile(core_catalog_path);
        REQUIRE_FALSE(catalog_json.empty());
        REQUIRE(registry.LoadModBundleFromJson(catalog_json));
        REQUIRE(registry.FindItem("item.pea_soup") != nullptr);
    }

    // Synthetic post-release community Mycology Mod JSON bundle
    const std::string community_mod_json = R"JSON({
      "items": [
        {
          "item_id": "mod.item.oyster_mushroom_cluster",
          "display_name": "Fresh Pearl Oyster Mushroom Cluster",
          "matter_phase": "DISCRETE_SOLID",
          "unit_mass_g": 450,
          "unit_volume_ml": 900,
          "caloric_kcal": 165,
          "exergy_per_unit_uj": 690000
        }
      ],
      "affordances": [
        {
          "affordance_id": "mod.affordance.harvest_oyster_flush",
          "label": "Harvest Pearl Oyster Flush",
          "required_capability_tag": "cap.mycology_cultivation",
          "required_property_key": "mycelium_colonization_bp",
          "min_property_val": 10000,
          "max_property_val": 10000,
          "output_item_id": "mod.item.oyster_mushroom_cluster",
          "output_qty": 3,
          "duration_sim_s": 300,
          "target_property_mut_key": "mycelium_colonization_bp",
          "target_property_delta": -10000
        }
      ],
      "entity_archetypes": [
        {
          "archetype_id": "mod.archetype.mycelium_fruiting_rack",
          "display_name": "Hardwood Sawdust Mycelium Fruiting Rack",
          "look_description": "Humid cedar shelving stacked with colonized hardwood-and-coffee-chaff fruiting blocks.",
          "category_tag": "mycology",
          "voxel_material_id": 91,
          "is_passable": false,
          "is_airtight": false,
          "inspect_property_keys": ["mycelium_colonization_bp", "chamber_humidity_bp"],
          "affordance_ids": ["mod.affordance.harvest_oyster_flush"]
        }
      ]
    })JSON";

    REQUIRE(registry.LoadModBundleFromJson(community_mod_json));

    const ItemDef* mod_item = registry.FindItem("mod.item.oyster_mushroom_cluster");
    REQUIRE(mod_item != nullptr);
    REQUIRE(mod_item->unit_mass_g == 450u);
    REQUIRE(mod_item->caloric_kcal == 165u);

    const EntityArchetypeDef* mod_station = registry.FindArchetype("mod.archetype.mycelium_fruiting_rack");
    REQUIRE(mod_station != nullptr);

    PropertyBagComponent station_props{};
    station_props.Set("mycelium_colonization_bp", 6500, "bp");

    AgentCapabilitiesComponent steward_caps{};

    // Before colonization completes & without capability: universal Look + Inspect are present, harvest gated out
    const auto gated_affordances = QueryEntityAffordances(registry, *mod_station, &station_props, &steward_caps);
    REQUIRE(HasAffordanceId(gated_affordances, "affordance.look"));
    REQUIRE(HasAffordanceId(gated_affordances, "affordance.inspect"));
    REQUIRE_FALSE(HasAffordanceId(gated_affordances, "mod.affordance.harvest_oyster_flush"));

    // Once colonized to 10000 bp and steward holds cap.mycology_cultivation: custom mod affordance is live!
    station_props.Set("mycelium_colonization_bp", 10000, "bp");
    steward_caps.GrantCapability("cap.mycology_cultivation");

    const auto live_affordances = QueryEntityAffordances(registry, *mod_station, &station_props, &steward_caps);
    REQUIRE(HasAffordanceId(live_affordances, "affordance.look"));
    REQUIRE(HasAffordanceId(live_affordances, "affordance.inspect"));
    REQUIRE(HasAffordanceId(live_affordances, "mod.affordance.harvest_oyster_flush"));
}

// =============================================================================
// PILLAR 6: Solo 4-Node Personal Homestead Cluster, Strict BPMN Token Sequencing & Opt-In R1 Swarm
// =============================================================================

TEST_CASE("Pillar 6: Solo 4-Node Homestead Cluster, strict cross-node BPMN token & physical stage sequencing, and opt-in R1 BCC swarm",
          "[native_stack][bpmn_tokens][swarmd][bcc]") {
    using namespace oasis::stack;

    SharedStackRuntime runtime{};

    // 1. Solo Steward 4-Node Personal Homestead Cluster defaults (0 upfront workflows, external R1 swarm OFF)
    REQUIRE(runtime.GetClusterNodes().size() == 4u);
    REQUIRE(runtime.GetClusterNodes()[0].node_id == "node-01:office-coordinator");
    REQUIRE(runtime.GetClusterNodes()[1].node_id == "node-02:garage-fab");
    REQUIRE(runtime.GetClusterNodes()[2].node_id == "node-03:garden-agro");
    REQUIRE(runtime.GetClusterNodes()[3].node_id == "node-04:hearth-galley");
    REQUIRE_FALSE(runtime.IsExternalR1SwarmEnabled());
    REQUIRE(runtime.GetWorkflows().empty());
    REQUIRE(runtime.RaisedBedFramesBuilt() == 0u);
    REQUIRE(runtime.HarvestedSplitPeaBatches() == 0u);

    // Stepping col-swarmd in default Solo 4-Node Cluster mode performs local cluster heartbeats with ZERO external NPC interference
    oasis::swarm::SwarmState swarm{0xC011EC717E2026ULL};
    REQUIRE(swarm.clusters.size() == 3u);
    oasis::swarm::StepPeerClusterSwarm(runtime, swarm, runtime.CurrentTick() + 1ULL);
    REQUIRE(swarm.total_bcc_settled_uj == 0ULL);
    REQUIRE(swarm.total_events_emitted == 0ULL);
    REQUIRE(runtime.GetClusterNodes()[0].is_online == 1u);
    REQUIRE(runtime.CurrentTick() >= 2ULL);

    // 2. L6 Need search, L7 fallback request, and user-defined new mesh need
    const std::vector<L6NeedRecord> pea_matches = runtime.SearchNeeds("pea");
    REQUIRE(pea_matches.size() >= 2u);

    const int64_t initial_reserve_cents = runtime.GetLedger().household_reserve_cents;
    REQUIRE(initial_reserve_cents == 2000000); // $20,000.00 Genesis Reserve
    REQUIRE(runtime.GetLedger().monthly_legacy_bills_cents == 22000); // $220.00/mo baseline

    const L6IntentRequest fallback_req = runtime.RequestNeed("need:pea_soup", NeedTimingMode::NOW, 0u, 1u);
    REQUIRE(fallback_req.resolved_sourcing_mode == SourcingMode::L7_LEGACY_CHANNEL);
    REQUIRE(fallback_req.fulfilled == 1u);
    REQUIRE(runtime.GetLedger().household_reserve_cents < initial_reserve_cents);

    const L6NeedRecord custom_need = runtime.DefineNewMeshNeed(
        "Raw Apple Cider Vinegar",
        "Fermentation & Broth Acidifier",
        "L7 Specialty Grocer ($8.00/bottle)",
        1600000000ULL,
        800
    );
    REQUIRE(custom_need.is_custom_mesh_need == 1u);
    REQUIRE_FALSE(runtime.SearchNeeds("Vinegar").empty());

    // 3. Approving Pea Cultivation & Pea Soup spawns cross-node BPMN choreography:
    //    - wf:pea_cultivation on node-03:garden-agro starts BLOCKED_BY_PRECONDITION (no raised bed built yet!)
    //    - wf:fab_raised_bed is auto-requisitioned onto node-02:garage-fab with ACTIVE_TOKEN on fab_1
    //    - wf:pea_soup_hearth on node-04:hearth-galley starts BLOCKED_BY_PRECONDITION (no harvested peas yet!)
    REQUIRE(runtime.ApproveOrAdaptGovernanceDecision("dec:pea_cultivation", true));
    REQUIRE(runtime.ApproveOrAdaptGovernanceDecision("dec:pea_soup_hearth", true));

    const L4WorkflowInstance* fab_bed_wf = FindWorkflowById(runtime, "wf:fab_raised_bed");
    const L4WorkflowInstance* pea_wf     = FindWorkflowById(runtime, "wf:pea_cultivation");
    const L4WorkflowInstance* soup_wf    = FindWorkflowById(runtime, "wf:pea_soup_hearth");
    REQUIRE(fab_bed_wf != nullptr);
    REQUIRE(pea_wf != nullptr);
    REQUIRE(soup_wf != nullptr);
    REQUIRE(fab_bed_wf->assigned_node_id == "node-02:garage-fab");
    REQUIRE(pea_wf->assigned_node_id == "node-03:garden-agro");
    REQUIRE(soup_wf->assigned_node_id == "node-04:hearth-galley");

    const L4TaskItem* fab_1  = FindTaskById(runtime, "task:fab_1_mill_raised_bed");
    const L4TaskItem* pea_1  = FindTaskById(runtime, "task:pea_1_plow_amend");
    const L4TaskItem* pea_2  = FindTaskById(runtime, "task:pea_2_erect_trellis");
    const L4TaskItem* soup_1 = FindTaskById(runtime, "task:soup_1_gather_inputs");
    const L4TaskItem* soup_4 = FindTaskById(runtime, "task:soup_4_package_distribute_tiffins");
    REQUIRE(fab_1 != nullptr);
    REQUIRE(pea_1 != nullptr);
    REQUIRE(pea_2 != nullptr);
    REQUIRE(soup_1 != nullptr);
    REQUIRE(soup_4 != nullptr);

    REQUIRE(fab_1->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    REQUIRE(pea_1->bpmn_token_state == BpmnTokenState::BLOCKED_BY_PRECONDITION);
    REQUIRE(pea_2->bpmn_token_state == BpmnTokenState::WAITING_SEQUENCE_FLOW);
    REQUIRE(soup_1->bpmn_token_state == BpmnTokenState::BLOCKED_BY_PRECONDITION);
    REQUIRE(soup_4->bpmn_token_state == BpmnTokenState::WAITING_SEQUENCE_FLOW);

    // Out-of-order CommitTask / CompleteTask / ReleaseTaskToR1Peers on blocked or downstream steps MUST fail!
    REQUIRE_FALSE(runtime.CommitTask("task:pea_1_plow_amend"));
    REQUIRE_FALSE(runtime.CompleteTask("task:pea_1_plow_amend"));
    REQUIRE_FALSE(runtime.CommitTask("task:pea_2_erect_trellis"));
    REQUIRE_FALSE(runtime.CompleteTask("task:pea_3_rhizobium_sow"));
    REQUIRE_FALSE(runtime.CommitTask("task:soup_1_gather_inputs"));
    REQUIRE_FALSE(runtime.CompleteTask("task:soup_4_package_distribute_tiffins"));
    REQUIRE_FALSE(runtime.ReleaseTaskToR1Peers("task:fab_1_mill_raised_bed")); // External R1 swarm is disabled!

    // Local L5/L4 ratification now fulfills Pea Soup intents via R0_LOCAL_WORKFLOWS at $0 L7 fiat spend
    const int64_t reserve_after_ratification = runtime.GetLedger().household_reserve_cents;
    const L6IntentRequest local_req = runtime.RequestNeed("need:pea_soup", NeedTimingMode::NOW, 0u, 1u);
    REQUIRE(local_req.resolved_sourcing_mode == SourcingMode::R0_LOCAL_WORKFLOWS);
    REQUIRE(runtime.GetLedger().household_reserve_cents == reserve_after_ratification);

    // 4. L2 pin ingestion switches task:pea_4_irrigate_weed from MANUAL_HUMAN_OBSERVATION to AUTOMATED_L2_EXECUTION
    const L4TaskItem* irrigate_before = FindTaskById(runtime, "task:pea_4_irrigate_weed");
    REQUIRE(irrigate_before != nullptr);
    REQUIRE(irrigate_before->execution_modality == TaskExecutionModality::MANUAL_HUMAN_OBSERVATION);

    const std::string l2_resp_dry = runtime.IngestL2PinsPayload(
        R"({"soil_probe_v1":{"is_online":true,"moisture_bp":2900,"temperature_mk":291150,"nitrogen_ppm":52}})"
    );
    REQUIRE(l2_resp_dry.find("\"commanded_on\": true") != std::string::npos);

    const L4TaskItem* irrigate_after = FindTaskById(runtime, "task:pea_4_irrigate_weed");
    REQUIRE(irrigate_after != nullptr);
    REQUIRE(irrigate_after->execution_modality == TaskExecutionModality::AUTOMATED_L2_EXECUTION);

    const L2SensorRegister* valve_reg = runtime.GetL2TelemetryTable().FindRegister("irrigation_valve_v1");
    REQUIRE(valve_reg != nullptr);
    REQUIRE(valve_reg->is_online == 1u);
    REQUIRE(valve_reg->actuator_commanded_on == 1u);
    REQUIRE(valve_reg->actuator_level_bp == 10000u);

    (void)runtime.IngestL2PinsPayload(
        R"({"soil_probe_v1":{"is_online":true,"moisture_bp":4350,"temperature_mk":291150,"nitrogen_ppm":55}})"
    );
    valve_reg = runtime.GetL2TelemetryTable().FindRegister("irrigation_valve_v1");
    REQUIRE(valve_reg != nullptr);
    REQUIRE(valve_reg->actuator_commanded_on == 0u);
    REQUIRE(valve_reg->actuator_level_bp == 0u);

    // 5. Completing fab_1 on node-02 builds the raised bed frame and unblocks ONLY pea_1 on node-03!
    REQUIRE(runtime.CommitTask("task:fab_1_mill_raised_bed"));
    REQUIRE(runtime.CompleteTask("task:fab_1_mill_raised_bed"));
    REQUIRE(runtime.RaisedBedFramesBuilt() == 1u);

    pea_1 = FindTaskById(runtime, "task:pea_1_plow_amend");
    pea_2 = FindTaskById(runtime, "task:pea_2_erect_trellis");
    REQUIRE(pea_1->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    REQUIRE(pea_2->bpmn_token_state == BpmnTokenState::WAITING_SEQUENCE_FLOW);
    REQUIRE_FALSE(runtime.CompleteTask("task:pea_2_erect_trellis"));

    // Answer unmodeled micro-detail prompt on step 3 (Rhizobium inoculation) and step strictly pea_1 -> pea_5
    REQUIRE(runtime.AnswerUnmodeledMicroDetailPrompt(
        "task:pea_3_rhizobium_sow",
        "Dampened seeds and coated with Rhizobium slurry in shade before sowing 3.5 cm deep."
    ));
    REQUIRE(FindTaskById(runtime, "task:pea_3_rhizobium_sow")->unmodeled_prompt_resolved == 1u);

    REQUIRE(runtime.CompleteTask("task:pea_1_plow_amend"));
    REQUIRE(FindTaskById(runtime, "task:pea_2_erect_trellis")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    REQUIRE(runtime.CompleteTask("task:pea_2_erect_trellis"));
    REQUIRE(FindTaskById(runtime, "task:pea_3_rhizobium_sow")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    REQUIRE(runtime.CompleteTask("task:pea_3_rhizobium_sow"));

    // Right after sowing (0 elapsed seconds), harvest MUST be impossible even with L2 probe + valve online!
    // Step 4 enters WAITING_TIMER while L2 monitors biological growth across the 12h (43,200 s) cultivar cycle.
    REQUIRE(FindTaskById(runtime, "task:pea_4_irrigate_weed")->bpmn_token_state == BpmnTokenState::WAITING_TIMER);
    REQUIRE(FindTaskById(runtime, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::WAITING_SEQUENCE_FLOW);
    REQUIRE_FALSE(runtime.CompleteTask("task:pea_5_harvest_pods_roots"));

    // Advance 6 hours (21,600 s): mid-vegetative/flowering (~52%), still waiting on biological maturity
    runtime.AdvanceSimClockSeconds(21600ULL);
    REQUIRE(FindWorkflowById(runtime, "wf:pea_cultivation")->crop_growth_bp > 4500u);
    REQUIRE(FindWorkflowById(runtime, "wf:pea_cultivation")->crop_growth_bp < 9500u);
    REQUIRE(FindTaskById(runtime, "task:pea_4_irrigate_weed")->bpmn_token_state == BpmnTokenState::WAITING_TIMER);
    REQUIRE_FALSE(runtime.CompleteTask("task:pea_5_harvest_pods_roots"));

    // Advance remaining 6 hours (total 43,200 s = 12h): L2 auto-verifies >= 9500 bp maturity, completes Step 4, and unlocks Step 5 Harvest!
    runtime.AdvanceSimClockSeconds(21600ULL);
    REQUIRE(FindWorkflowById(runtime, "wf:pea_cultivation")->crop_growth_bp >= 9500u);
    REQUIRE(FindTaskById(runtime, "task:pea_4_irrigate_weed")->bpmn_token_state == BpmnTokenState::COMPLETED);

    // Before harvest (pea_5), Pea Soup step 1 on node-04 is STILL blocked!
    REQUIRE(FindTaskById(runtime, "task:soup_1_gather_inputs")->bpmn_token_state == BpmnTokenState::BLOCKED_BY_PRECONDITION);
    REQUIRE(FindTaskById(runtime, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    REQUIRE(runtime.CompleteTask("task:pea_5_harvest_pods_roots"));
    REQUIRE(runtime.HarvestedSplitPeaBatches() == 2u); // Chrono-Pod Virtual Pea yields +2 legume batches

    // Harvest immediately unblocks soup_1 on node-04:hearth-galley AND advances garden token to pea_6!
    const L4TaskItem* pea_6 = FindTaskById(runtime, "task:pea_6_shell_moisture_turn");
    REQUIRE(pea_6 != nullptr);
    REQUIRE(pea_6->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    REQUIRE(FindTaskById(runtime, "task:soup_1_gather_inputs")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    REQUIRE_FALSE(runtime.CompleteTask("task:soup_2_simmer_bone_broth"));

    REQUIRE(runtime.CompleteTask("task:pea_6_shell_moisture_turn"));
    REQUIRE(FindWorkflowById(runtime, "wf:pea_cultivation")->completed_cycles == 1u);

    REQUIRE(runtime.CompleteTask("task:soup_1_gather_inputs"));
    REQUIRE(runtime.CompleteTask("task:soup_2_simmer_bone_broth"));
    REQUIRE(runtime.CompleteTask("task:soup_3_simmer_pea_soup_74c"));
    REQUIRE(runtime.CompleteTask("task:soup_4_package_distribute_tiffins"));
    REQUIRE(FindWorkflowById(runtime, "wf:pea_soup_hearth")->completed_cycles == 1u);

    // 6. Verify 3D Physical Stage Preconditions in core_catalog.json (bed_stage 0..5 & empty grass plot)
    {
        using namespace oasis::engine;
        ContentRegistry catalog{};
        catalog.BootstrapCoreCatalog();
        const std::filesystem::path core_catalog_path =
            FindRepoAssetPath("core/1_simulation/data/content/core_catalog.json");
        REQUIRE(std::filesystem::exists(core_catalog_path));
        REQUIRE(catalog.LoadModBundleFromJson(ReadTextFile(core_catalog_path)));

        const EntityArchetypeDef* yard_plot  = catalog.FindArchetype("arch.yard_earthwork_plot");
        const EntityArchetypeDef* raised_bed = catalog.FindArchetype("arch.raised_garden_bed");
        REQUIRE(yard_plot != nullptr);
        REQUIRE(raised_bed != nullptr);

        AgentCapabilitiesComponent caps{};
        caps.GrantCapability("SOIL_CULTIVATION_BED");
        caps.GrantCapability("SOIL_EARTHWORKS");
        EquippedHaulingToolComponent wheelbarrow{"wheelbarrow", "ent.tool_wheelbarrow"};

        // Empty lawn plot ONLY offers Look + Inspect (no upfront digging or sowing on unbuilt grass!)
        const auto lawn_affs = QueryEntityAffordances(catalog, *yard_plot, nullptr, &caps, &wheelbarrow);
        REQUIRE(lawn_affs.size() == 2u);
        REQUIRE(HasAffordanceId(lawn_affs, "affordance.look"));
        REQUIRE(HasAffordanceId(lawn_affs, "affordance.inspect"));

        // Newly milled raised bed (bed_stage == 0): ONLY Broadfork & Amend is unlocked; trellis/sow/harvest hidden!
        PropertyBagComponent bed_props{};
        bed_props.Set("bed_stage", 0);
        bed_props.Set("l2_probe_installed", 0);
        bed_props.Set("l2_valve_installed", 0);

        const auto stage0_affs = QueryEntityAffordances(catalog, *raised_bed, &bed_props, &caps, &wheelbarrow);
        REQUIRE(HasAffordanceId(stage0_affs, "aff.plow_and_amend_bed"));
        REQUIRE_FALSE(HasAffordanceId(stage0_affs, "aff.erect_pea_trellis"));
        REQUIRE_FALSE(HasAffordanceId(stage0_affs, "aff.inoculate_and_sow_peas"));
        REQUIRE_FALSE(HasAffordanceId(stage0_affs, "aff.harvest_peas_and_roots"));
        REQUIRE_FALSE(HasAffordanceId(stage0_affs, "aff.shell_dry_and_turn_bed"));

        // After Broadfork & Amend advances bed_stage -> 1: Erect Trellis unlocks and Broadfork disappears!
        bed_props.Set("bed_stage", 1);
        const auto stage1_affs = QueryEntityAffordances(catalog, *raised_bed, &bed_props, &caps, &wheelbarrow);
        REQUIRE_FALSE(HasAffordanceId(stage1_affs, "aff.plow_and_amend_bed"));
        REQUIRE(HasAffordanceId(stage1_affs, "aff.erect_pea_trellis"));
        REQUIRE_FALSE(HasAffordanceId(stage1_affs, "aff.inoculate_and_sow_peas"));
    }

    // 7. Start a repeatable cycle + opt into External R1 Peer-Cluster Swarm -> pairwise zero-sum BCC clearing
    REQUIRE(runtime.StartNewWorkflowCycle("wf:pea_cultivation"));
    REQUIRE(FindTaskById(runtime, "task:pea_1_plow_amend")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);

    runtime.SetExternalR1SwarmEnabled(true);
    REQUIRE(runtime.IsExternalR1SwarmEnabled());
    REQUIRE_FALSE(runtime.ReleaseTaskToR1Peers("task:pea_3_rhizobium_sow")); // Not holding ACTIVE_TOKEN!
    REQUIRE(runtime.ReleaseTaskToR1Peers("task:pea_1_plow_amend"));

    oasis::swarm::StepPeerClusterSwarm(runtime, swarm, runtime.CurrentTick() + 1ULL);
    const L4TaskItem* claimed_pea = FindTaskById(runtime, "task:pea_1_plow_amend");
    REQUIRE(claimed_pea != nullptr);
    REQUIRE(claimed_pea->task_commitment_state == TaskCommitmentState::CLAIMED_BY_PEER_CLUSTER);
    REQUIRE_FALSE(claimed_pea->claimed_by_cluster_id.empty());

    for (uint32_t step = 0u; step < 5u; ++step) {
        oasis::swarm::StepPeerClusterSwarm(runtime, swarm, runtime.CurrentTick() + 1ULL);
    }

    claimed_pea = FindTaskById(runtime, "task:pea_1_plow_amend");
    REQUIRE(claimed_pea->task_commitment_state == TaskCommitmentState::COMPLETED);
    REQUIRE(FindTaskById(runtime, "task:pea_2_erect_trellis")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    REQUIRE(swarm.total_bcc_settled_uj == 800000000ULL);
    REQUIRE(swarm.total_events_emitted >= 2ULL);
    REQUIRE(runtime.GetLedger().VerifyPairwiseZeroSumInvariant());
    REQUIRE_FALSE(runtime.GetLedger().mesh_weather_feed.empty());

    uint32_t total_mutual_tasks = 0u;
    for (const auto& ch : runtime.GetLedger().peer_bcc_channels) {
        REQUIRE(ch.IsZeroSumValid());
        REQUIRE((ch.balance_local_to_peer_uj + ch.balance_peer_to_local_uj) == 0);
        total_mutual_tasks += ch.completed_mutual_tasks;
    }
    REQUIRE(total_mutual_tasks == 1u);

    // 8. Ratify dec:garage_l7_woodwork and complete +$180 (18,000 cents) L7 custom woodwork bill-offset order
    const int64_t reserve_before_woodwork = runtime.GetLedger().household_reserve_cents;
    REQUIRE(runtime.GetLedger().monthly_l7_offset_revenue_cents == 0);
    REQUIRE(runtime.ApproveOrAdaptGovernanceDecision("dec:garage_l7_woodwork", true));
    REQUIRE(FindTaskById(runtime, "task:fab_4_l7_custom_woodwork_order")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);

    REQUIRE(runtime.CompleteTask("task:fab_4_l7_custom_woodwork_order"));
    REQUIRE(runtime.GetLedger().monthly_l7_offset_revenue_cents == 18000);
    REQUIRE(runtime.GetLedger().household_reserve_cents == (reserve_before_woodwork + 18000));
    REQUIRE(runtime.GetLedger().completed_l7_offset_orders == 1u);
    REQUIRE(runtime.GetLedger().NetMonthlyCashflowCents() == -4000); // $180 offset against $220 legacy bills

    const std::string snapshot_json = runtime.SerializeFullNodeSnapshotJson();
    REQUIRE(snapshot_json.find("\"bcc_zero_sum_invariant_ok\": true") != std::string::npos);
    REQUIRE(snapshot_json.find("\"monthly_l7_offset_revenue_cents\": 18000") != std::string::npos);
    REQUIRE(snapshot_json.find("\"homestead_cluster_nodes\"") != std::string::npos);
    REQUIRE(snapshot_json.find("\"active_bpmn_tokens\"") != std::string::npos);
    REQUIRE(snapshot_json.find("\"l4_available_tasks\"") != std::string::npos);
    REQUIRE(snapshot_json.find("\"available_cultivars\"") != std::string::npos);
}

TEST_CASE("Pillar 7: Biological Timing, Manual BPMN WAITING_TIMER Loops & Parameterized Fabaceae Cultivars",
          "[modular][cultivar][timing][bpmn]") {
    using namespace oasis::stack;

    SECTION("Manual workflow requires periodic timed verification & watering loops before unlocking Harvest") {
        SharedStackRuntime runtime{};
        REQUIRE(runtime.GetAvailableCultivars().size() == 4u);
        REQUIRE(runtime.SelectedGardenCultivarId() == "cultivar:chrono_virtual_pea");

        // Approve Pea Cultivation (no L2 soil_probe_v1 installed -> strictly MANUAL_HUMAN_OBSERVATION!)
        REQUIRE(runtime.ApproveOrAdaptGovernanceDecision("dec:pea_cultivation", true));
        REQUIRE(runtime.CompleteTask("task:fab_1_mill_raised_bed"));
        REQUIRE(runtime.CompleteTask("task:pea_1_plow_amend"));
        REQUIRE(runtime.CompleteTask("task:pea_2_erect_trellis"));
        REQUIRE(runtime.CompleteTask("task:pea_3_rhizobium_sow"));

        const L4TaskItem* step4 = FindTaskById(runtime, "task:pea_4_irrigate_weed");
        const L4TaskItem* step5 = FindTaskById(runtime, "task:pea_5_harvest_pods_roots");
        REQUIRE(step4 != nullptr);
        REQUIRE(step5 != nullptr);
        REQUIRE(step4->execution_modality == TaskExecutionModality::MANUAL_HUMAN_OBSERVATION);
        REQUIRE(step4->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
        REQUIRE(step5->bpmn_token_state == BpmnTokenState::WAITING_SEQUENCE_FLOW);

        // Check #1 right after sowing waters the bed to optimal VWC, sees 2% growth (< 95%), and arms 3h WAITING_TIMER!
        REQUIRE(runtime.CompleteTask("task:pea_4_irrigate_weed"));
        step4 = FindTaskById(runtime, "task:pea_4_irrigate_weed");
        step5 = FindTaskById(runtime, "task:pea_5_harvest_pods_roots");
        REQUIRE(step4->bpmn_token_state == BpmnTokenState::WAITING_TIMER);
        REQUIRE(step4->timer_remaining_sim_s == 10800ULL);
        REQUIRE(step5->bpmn_token_state == BpmnTokenState::WAITING_SEQUENCE_FLOW);

        // Cannot bypass timer or harvest prematurely!
        REQUIRE_FALSE(runtime.CompleteTask("task:pea_4_irrigate_weed"));
        REQUIRE_FALSE(runtime.CommitTask("task:pea_4_irrigate_weed"));
        REQUIRE_FALSE(runtime.CompleteTask("task:pea_5_harvest_pods_roots"));

        const std::string timer_snap = runtime.SerializeFullNodeSnapshotJson();
        REQUIRE(timer_snap.find("\"WAITING_TIMER\"") != std::string::npos);

        // Advance 4 timed verification intervals of 3 hours (4 * 10,800 s = 43,200 s = 12h)
        for (uint32_t check = 0u; check < 4u; ++check) {
            runtime.AdvanceSimClockSeconds(10800ULL);
            step4 = FindTaskById(runtime, "task:pea_4_irrigate_weed");
            REQUIRE(step4->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
            // Even when biological growth reaches >= 95% on the 4th interval, without L2 sensors
            // Harvest stays gated until the Steward performs the manual verification check!
            REQUIRE(FindTaskById(runtime, "task:pea_5_harvest_pods_roots")->bpmn_token_state ==
                    BpmnTokenState::WAITING_SEQUENCE_FLOW);
            REQUIRE(runtime.CompleteTask("task:pea_4_irrigate_weed"));
        }

        // After the 4th timed check verifies >= 9500 bp maturity, Step 4 completes and Step 5 Harvest unlocks!
        REQUIRE(FindWorkflowById(runtime, "wf:pea_cultivation")->crop_growth_bp >= 9500u);
        REQUIRE(FindWorkflowById(runtime, "wf:pea_cultivation")->estimated_nitrogen_ppm > 48u); // Rhizobium N2 fixation!
        REQUIRE(FindTaskById(runtime, "task:pea_4_irrigate_weed")->bpmn_token_state == BpmnTokenState::COMPLETED);
        REQUIRE(FindTaskById(runtime, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
        REQUIRE(runtime.CompleteTask("task:pea_5_harvest_pods_roots"));
        REQUIRE(runtime.HarvestedSplitPeaBatches() == 2u);
        REQUIRE(runtime.HarvestedMirepoixBatches() == 1u);
    }

    SECTION("Parameterized cultivar input adapts workflow to Kentucky Wonder Pole Bean (warm-soil gate, N-draw & 10-day cycle)") {
        SharedStackRuntime runtime{};
        REQUIRE(runtime.SetGardenCultivar("cultivar:kentucky_wonder_bean"));
        REQUIRE(runtime.SelectedGardenCultivarId() == "cultivar:kentucky_wonder_bean");

        REQUIRE(runtime.ApproveOrAdaptGovernanceDecision("dec:pea_cultivation", true));
        const L4WorkflowInstance* wf = FindWorkflowById(runtime, "wf:pea_cultivation");
        REQUIRE(wf != nullptr);
        REQUIRE(wf->input_cultivar_id == "cultivar:kentucky_wonder_bean");
        REQUIRE(wf->species_binomial == "Phaseolus vulgaris");
        REQUIRE(wf->fixes_atmospheric_n2 == 0u);
        REQUIRE(wf->is_virtual_cultivar == 0u);
        REQUIRE(wf->maturity_sim_seconds == 864000u);       // 10 sim days
        REQUIRE(wf->verification_interval_sim_s == 43200u); // 12 sim hours

        // Verify workflow task titles & prompts dynamically adapted to Pole Bean specifications (2100 mm ridge, >=12C warm soil)
        REQUIRE(FindTaskById(runtime, "task:pea_2_erect_trellis")->title.find("2100 mm") != std::string::npos);
        REQUIRE(FindTaskById(runtime, "task:pea_3_rhizobium_sow")->title.find("Warm-Soil Check") != std::string::npos);

        REQUIRE(runtime.CompleteTask("task:fab_1_mill_raised_bed"));
        REQUIRE(runtime.CompleteTask("task:pea_1_plow_amend"));
        REQUIRE(runtime.CompleteTask("task:pea_2_erect_trellis"));
        REQUIRE(runtime.CompleteTask("task:pea_3_rhizobium_sow"));

        // Put L2 probe + valve online for closed-loop irrigation across the 10-day growth test
        (void)runtime.IngestL2PinsPayload(
            R"({"soil_probe_v1":{"is_online":true,"moisture_bp":4400,"temperature_mk":282150,"nitrogen_ppm":48}})"
        );

        // Cold spring soil (9 C = 282150 mK < 285150 mK Pole Bean floor): growth completely stalls at 200 bp!
        runtime.SetAmbientTempOverrideMk(282150U);
        runtime.AdvanceSimClockSeconds(86400ULL);
        wf = FindWorkflowById(runtime, "wf:pea_cultivation");
        REQUIRE(wf->crop_growth_bp == 200u);
        REQUIRE_FALSE(runtime.CompleteTask("task:pea_5_harvest_pods_roots"));

        // Warm summer soil (22 C = 295150 mK): Pole Bean grows across 10 days, consumes soil mineral N, and yields +4 batches!
        runtime.SetAmbientTempOverrideMk(295150U);
        runtime.AdvanceSimClockSeconds(864000ULL);
        wf = FindWorkflowById(runtime, "wf:pea_cultivation");
        REQUIRE(wf->crop_growth_bp >= 9500u);
        REQUIRE(wf->estimated_nitrogen_ppm < 48u); // Non-nodulating Phaseolus drew mineral N!
        REQUIRE(FindTaskById(runtime, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
        REQUIRE(runtime.CompleteTask("task:pea_5_harvest_pods_roots"));
        REQUIRE(runtime.HarvestedSplitPeaBatches() == 4u);
        REQUIRE(runtime.HarvestedMirepoixBatches() == 2u);
    }

    SECTION("3D SoilCellRegisters cultivar physics matches Fabaceae N-fixation vs. Pole Bean thermal & N-draw behavior") {
        using namespace oasis::engine;

        const CultivarPhysicsSpec& chrono_pea = ResolveCultivarByCode(kCultivarCodeChronoVirtualPea);
        const CultivarPhysicsSpec& pole_bean  = ResolveCultivarByCode(kCultivarCodeKentuckyWonderBean);

        SoilCellRegisters pea_soil{};
        pea_soil.moisture_vwc_bp   = 4200U;
        pea_soil.soil_temp_mk      = 282150U; // 9 C cool spring soil
        pea_soil.organic_carbon_bp = 4500U;
        pea_soil.nitrogen_ppm      = 45U;
        pea_soil.phosphorus_ppm    = 35U;
        pea_soil.potassium_ppm     = 140U;
        pea_soil.crop_growth_bp    = 200U;
        pea_soil.crop_planted      = 1U;
        for (uint32_t step = 0U; step < 12U; ++step) {
            pea_soil.moisture_vwc_bp = 4200U;
            StepCultivarSoilPhysics(pea_soil, chrono_pea, 282150U, 0U, 900U); // 12 * 900 s = 3h
        }
        REQUIRE(pea_soil.crop_growth_bp > 1000U); // Cool-season Pisum thrives at 9 C
        REQUIRE(pea_soil.nitrogen_ppm >= 45U);    // Rhizobium N2 fixation replenishes soil N

        SoilCellRegisters bean_soil{};
        bean_soil.moisture_vwc_bp   = 4400U;
        bean_soil.soil_temp_mk      = 282150U; // 9 C cold stall!
        bean_soil.organic_carbon_bp = 4500U;
        bean_soil.nitrogen_ppm      = 100U;    // Compost-amended raised bed
        bean_soil.phosphorus_ppm    = 115U;
        bean_soil.potassium_ppm     = 240U;
        bean_soil.crop_growth_bp    = 200U;
        bean_soil.crop_planted      = 1U;
        for (uint32_t step = 0U; step < 12U; ++step) {
            bean_soil.moisture_vwc_bp = 4400U;
            StepCultivarSoilPhysics(bean_soil, pole_bean, 282150U, 0U, 900U);
        }
        REQUIRE(bean_soil.crop_growth_bp == 200U);

        // Warm soil (24 C = 297150 mK) over 10 days (480 * 1800 s) matures Pole Bean and draws N-P-K
        bean_soil.soil_temp_mk = 297150U;
        for (uint32_t step = 0U; step < 480U; ++step) {
            if (bean_soil.moisture_vwc_bp < 3800U) {
                bean_soil.moisture_vwc_bp = 4400U;
            }
            StepCultivarSoilPhysics(bean_soil, pole_bean, 297150U, 0U, 1800U);
        }
        REQUIRE(bean_soil.crop_growth_bp >= 9500U);
        REQUIRE(bean_soil.nitrogen_ppm < 70U);
        REQUIRE(bean_soil.potassium_ppm < 150U);
    }
}

TEST_CASE("Pillar 8: Epistemic Skill-Tiered Agronomy Inspection, Guided BPMN Stressors/Remediation & Variable Harvest Outcomes",
          "[modular][agronomy][epistemic][bpmn][harvest]") {
    using namespace oasis::engine;
    using namespace oasis::stack;

    SECTION("Epistemic InspectGardenBedEpistemic masks N-P-K and soil thermal/hydraulic insulation for Novice without L2 probe") {
        ContentRegistry registry{};
        registry.BootstrapCoreCatalog();
        const std::string catalog_path = FindRepoAssetPath("core/1_simulation/data/content/core_catalog.json");
        REQUIRE_FALSE(catalog_path.empty());
        REQUIRE(registry.LoadModBundleFromJson(ReadTextFile(catalog_path)) > 0U);
        const EntityArchetypeDef* bed_arch = registry.FindArchetype("arch.raised_garden_bed");
        REQUIRE(bed_arch != nullptr);

        SoilCellRegisters soil{};
        soil.moisture_vwc_bp   = 4100U;
        soil.soil_temp_mk      = 291150U;
        soil.nitrogen_ppm      = 16U;   // Deficient N
        soil.phosphorus_ppm    = 14U;   // Deficient P
        soil.potassium_ppm     = 65U;   // Deficient K
        soil.organic_carbon_bp = 1200U; // Poor thermal insulation & hydraulic flow
        soil.ph_centi          = 640U;
        soil.crop_growth_bp    = 4800U;
        soil.crop_planted      = 1U;

        GardenBedHealthState health{};
        health.crop_health_bp        = 7400U;
        health.care_quality_score_bp = 7200U;
        health.uncertainty_band_bp   = 2800U;
        health.skill_tier            = AgronomySkillTier::NOVICE;
        health.true_issue            = GardenIssueCode::SOIL_INSULATION_FLOW;

        SensorAttachmentComponent no_l2{};
        no_l2.has_l2_sensor = 0U;

        const CultivarPhysicsSpec& chrono_pea = ResolveCultivarByCode(kCultivarCodeChronoVirtualPea);

        const auto HasPropKey = [](const ManualInspectionReport& rep, const char* key) -> bool {
            for (const auto& kv : rep.revealed_properties) {
                if (kv.key == key) return true;
            }
            return false;
        };

        // 1. Novice Steward without L2 probe: CANNOT identify N/P/K ppm or soil organic_carbon_bp insulation/flow!
        health.skill_tier = AgronomySkillTier::NOVICE;
        const ManualInspectionReport novice_rep = InspectGardenBedEpistemic(
            *bed_arch, soil, health, chrono_pea, &no_l2
        );
        REQUIRE_FALSE(novice_rep.has_live_l2_telemetry);
        REQUIRE(HasPropKey(novice_rep, "novice_plant_condition_tier"));
        REQUIRE(HasPropKey(novice_rep, "novice_surface_moisture_tier"));
        REQUIRE(HasPropKey(novice_rep, "crop_growth_bp"));
        REQUIRE_FALSE(HasPropKey(novice_rep, "moisture_vwc_bp"));
        REQUIRE_FALSE(HasPropKey(novice_rep, "nitrogen_ppm"));
        REQUIRE_FALSE(HasPropKey(novice_rep, "phosphorus_ppm"));
        REQUIRE_FALSE(HasPropKey(novice_rep, "potassium_ppm"));
        REQUIRE_FALSE(HasPropKey(novice_rep, "organic_carbon_bp"));
        REQUIRE_FALSE(HasPropKey(novice_rep, "ph_centi"));

        // 2. Experienced Steward (Tier 2) without L2 probe: diagnoses foliar N/P/K & 10cm soil insulation/flow!
        health.skill_tier = AgronomySkillTier::EXPERIENCED;
        const ManualInspectionReport exp_rep = InspectGardenBedEpistemic(
            *bed_arch, soil, health, chrono_pea, &no_l2
        );
        REQUIRE(HasPropKey(exp_rep, "nitrogen_ppm"));
        REQUIRE(HasPropKey(exp_rep, "phosphorus_ppm"));
        REQUIRE(HasPropKey(exp_rep, "potassium_ppm"));
        REQUIRE(HasPropKey(exp_rep, "organic_carbon_bp"));
        REQUIRE(HasPropKey(exp_rep, "ph_centi"));
    }

    SECTION("Novice BPMN misses hidden NUTRIENT_DEFICIENCY -> POOR_STUNTED harvest (1 batch); Experienced diagnoses & remediates -> BUMPER_ABUNDANT harvest (3 batches)") {
        // Part A: Novice Steward (Tier 1, ±28% uncertainty) with hidden NUTRIENT_DEFICIENCY
        SharedStackRuntime novice_rt{};
        REQUIRE(novice_rt.StewardAgronomySkillTier() == kStackSkillNovice);
        // Before L5 governance approval, injecting a stressor MUST fail and never spawn an unsolicited workflow!
        REQUIRE_FALSE(novice_rt.InjectGardenIssue("DROUGHT_WILTING"));
        REQUIRE(novice_rt.GetWorkflows().empty());

        REQUIRE(novice_rt.ApproveOrAdaptGovernanceDecision("dec:pea_cultivation", true));
        REQUIRE(novice_rt.CompleteTask("task:fab_1_mill_raised_bed"));
        REQUIRE(novice_rt.CompleteTask("task:pea_1_plow_amend"));
        REQUIRE(novice_rt.CompleteTask("task:pea_2_erect_trellis"));
        REQUIRE(novice_rt.CompleteTask("task:pea_3_rhizobium_sow"));

        const L4WorkflowInstance* wf_nov = FindWorkflowById(novice_rt, "wf:pea_cultivation");
        REQUIRE(wf_nov != nullptr);
        REQUIRE(wf_nov->workflow_role == WorkflowRole::PRIMARY_PRODUCTION);
        REQUIRE(wf_nov->agronomy_skill_tier == kStackSkillNovice);
        REQUIRE(wf_nov->diagnostic_uncertainty_bp == 2800U);
        // No remediation workflow exists when no stressor is diagnosed
        REQUIRE(FindWorkflowById(novice_rt, "wf:remediate_pea_cultivation") == nullptr);
        REQUIRE(FindTaskById(novice_rt, "task:pea_4r_remediate_issue") == nullptr);

        // Inject hidden NUTRIENT_DEFICIENCY: Novice cannot diagnose it without Tier 2 skill or L2 probe!
        REQUIRE(novice_rt.InjectGardenIssue("NUTRIENT_DEFICIENCY"));
        wf_nov = FindWorkflowById(novice_rt, "wf:pea_cultivation");
        REQUIRE(wf_nov->active_issue_code == kStackIssueNutrientDeficit);
        REQUIRE(wf_nov->diagnosed_issue_code == kStackIssueNone);
        // Because Novice cannot diagnose the hidden stressor, L5 does NOT instantiate wf:remediate_pea_cultivation!
        REQUIRE(FindWorkflowById(novice_rt, "wf:remediate_pea_cultivation") == nullptr);
        REQUIRE(FindTaskById(novice_rt, "task:pea_4r_remediate_issue") == nullptr);

        // Run the verification checks while the unaddressed nutrient deficiency degrades health & care quality
        REQUIRE(novice_rt.CompleteTask("task:pea_4_irrigate_weed"));
        for (uint32_t i = 0U; i < 6U; ++i) {
            novice_rt.AdvanceSimClockSeconds(10800ULL);
            if (FindTaskById(novice_rt, "task:pea_4_irrigate_weed")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN) {
                REQUIRE(novice_rt.CompleteTask("task:pea_4_irrigate_weed"));
            }
            if (FindTaskById(novice_rt, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN) {
                break;
            }
        }

        wf_nov = FindWorkflowById(novice_rt, "wf:pea_cultivation");
        REQUIRE(wf_nov->crop_health_bp < 5800U);
        REQUIRE(FindTaskById(novice_rt, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
        REQUIRE(novice_rt.CompleteTask("task:pea_5_harvest_pods_roots"));

        wf_nov = FindWorkflowById(novice_rt, "wf:pea_cultivation");
        REQUIRE(wf_nov->last_harvest_outcome_tier == kStackOutcomePoor);
        REQUIRE(novice_rt.HarvestedSplitPeaBatches() == 1U); // Stunted 0.5x yield (1 vs nominal 2)
        REQUIRE(novice_rt.HarvestedMirepoixBatches() == 0U);

        // Part B: Experienced Steward (Tier 2, ±9% uncertainty) diagnoses SOIL_INSULATION_FLOW, dynamically spawns wf:remediate_pea_cultivation, and achieves BUMPER_ABUNDANT harvest!
        SharedStackRuntime exp_rt{};
        REQUIRE(exp_rt.SetStewardAgronomySkillTier(kStackSkillExperienced));
        REQUIRE(exp_rt.StewardAgronomySkillTier() == kStackSkillExperienced);
        REQUIRE(exp_rt.ApproveOrAdaptGovernanceDecision("dec:pea_cultivation", true));
        REQUIRE(exp_rt.CompleteTask("task:fab_1_mill_raised_bed"));
        REQUIRE(exp_rt.CompleteTask("task:pea_1_plow_amend"));
        REQUIRE(exp_rt.CompleteTask("task:pea_2_erect_trellis"));
        REQUIRE(exp_rt.CompleteTask("task:pea_3_rhizobium_sow"));

        const L4WorkflowInstance* wf_exp = FindWorkflowById(exp_rt, "wf:pea_cultivation");
        REQUIRE(wf_exp != nullptr);
        REQUIRE(wf_exp->agronomy_skill_tier == kStackSkillExperienced);
        REQUIRE(wf_exp->diagnostic_uncertainty_bp == 900U);
        REQUIRE(FindWorkflowById(exp_rt, "wf:remediate_pea_cultivation") == nullptr);

        // Inject SOIL_INSULATION_FLOW: Experienced Steward diagnoses it and L5 dynamically instantiates wf:remediate_pea_cultivation!
        REQUIRE(exp_rt.InjectGardenIssue("SOIL_INSULATION_FLOW"));
        wf_exp = FindWorkflowById(exp_rt, "wf:pea_cultivation");
        REQUIRE(wf_exp->active_issue_code == kStackIssueSoilInsulation);
        REQUIRE(wf_exp->diagnosed_issue_code == kStackIssueSoilInsulation);
        REQUIRE(wf_exp->active_child_workflow_id == "wf:remediate_pea_cultivation");

        const L4WorkflowInstance* rem_wf = FindWorkflowById(exp_rt, "wf:remediate_pea_cultivation");
        REQUIRE(rem_wf != nullptr);
        REQUIRE(rem_wf->workflow_role == WorkflowRole::ISSUE_REMEDIATION);
        REQUIRE(rem_wf->parent_workflow_id == "wf:pea_cultivation");
        const L4TaskItem* rem_task = FindTaskById(exp_rt, "task:pea_4r_remediate_issue");
        REQUIRE(rem_task != nullptr);
        REQUIRE(rem_task->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);

        // Execute dynamic Remediation workflow to restore biochar-humus thermal insulation & hydraulic flow
        REQUIRE(exp_rt.CompleteTask("task:pea_4r_remediate_issue"));
        wf_exp = FindWorkflowById(exp_rt, "wf:pea_cultivation");
        REQUIRE(wf_exp->active_issue_code == kStackIssueNone);
        REQUIRE(wf_exp->diagnosed_issue_code == kStackIssueNone);
        REQUIRE(wf_exp->active_child_workflow_id.empty());
        REQUIRE(wf_exp->remediations_completed == 1U);
        REQUIRE(wf_exp->soil_organic_carbon_bp >= 4500U);
        // Once remediation completes, the dynamic child workflow is cleanly retired!
        REQUIRE(FindWorkflowById(exp_rt, "wf:remediate_pea_cultivation") == nullptr);

        // Complete the 4 timed verification checks with Experienced care
        REQUIRE(exp_rt.CompleteTask("task:pea_4_irrigate_weed"));
        for (uint32_t check = 0U; check < 4U; ++check) {
            exp_rt.AdvanceSimClockSeconds(10800ULL);
            REQUIRE(exp_rt.CompleteTask("task:pea_4_irrigate_weed"));
        }

        wf_exp = FindWorkflowById(exp_rt, "wf:pea_cultivation");
        REQUIRE(wf_exp->crop_health_bp >= 9000U);
        REQUIRE(wf_exp->care_quality_score_bp >= 9000U);
        REQUIRE(FindTaskById(exp_rt, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
        REQUIRE(exp_rt.CompleteTask("task:pea_5_harvest_pods_roots"));

        wf_exp = FindWorkflowById(exp_rt, "wf:pea_cultivation");
        REQUIRE(wf_exp->last_harvest_outcome_tier == kStackOutcomeBumper);
        REQUIRE(exp_rt.HarvestedSplitPeaBatches() == 3U); // Bumper Abundant yield (+3 vs nominal +2)!
        REQUIRE(exp_rt.HarvestedMirepoixBatches() == 2U); // Bumper Abundant mirepoix (+2 vs nominal +1)!
    }

    SECTION("L5 Governance Model dynamically instantiates INPUT_SOURCING workflow (wf:source_pea_soup_inputs) when Pea Soup Hearth is approved without local Pea Cultivation") {
        SharedStackRuntime soup_only_rt{};
        REQUIRE(soup_only_rt.GetWorkflows().empty());

        // Approve ONLY dec:pea_soup_hearth (without approving dec:pea_cultivation)
        REQUIRE(soup_only_rt.ApproveOrAdaptGovernanceDecision("dec:pea_soup_hearth", true));

        const L4WorkflowInstance* soup_wf = FindWorkflowById(soup_only_rt, "wf:pea_soup_hearth");
        const L4WorkflowInstance* src_wf  = FindWorkflowById(soup_only_rt, "wf:source_pea_soup_inputs");
        REQUIRE(soup_wf != nullptr);
        REQUIRE(src_wf != nullptr);
        REQUIRE(soup_wf->workflow_role == WorkflowRole::PRIMARY_PRODUCTION);
        REQUIRE(soup_wf->active_child_workflow_id == "wf:source_pea_soup_inputs");
        REQUIRE(src_wf->workflow_role == WorkflowRole::INPUT_SOURCING);
        REQUIRE(src_wf->parent_workflow_id == "wf:pea_soup_hearth");

        const L4TaskItem* soup_step1 = FindTaskById(soup_only_rt, "task:soup_1_gather_inputs");
        const L4TaskItem* src_step1  = FindTaskById(soup_only_rt, "task:source_1_procure_soup_inputs");
        REQUIRE(soup_step1 != nullptr);
        REQUIRE(src_step1 != nullptr);
        REQUIRE(soup_step1->bpmn_token_state == BpmnTokenState::BLOCKED_BY_PRECONDITION);
        REQUIRE(src_step1->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);

        // Complete the dynamic input sourcing workflow -> unblocks soup_step1 and retires wf:source_pea_soup_inputs!
        REQUIRE(soup_only_rt.CompleteTask("task:source_1_procure_soup_inputs"));
        REQUIRE(FindWorkflowById(soup_only_rt, "wf:source_pea_soup_inputs") == nullptr);
        soup_step1 = FindTaskById(soup_only_rt, "task:soup_1_gather_inputs");
        REQUIRE(soup_step1 != nullptr);
        REQUIRE(soup_step1->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    }

    SECTION("Unremediated CROP_FAILURE_BLIGHT collapses health to <= 20% -> CROP_FAILURE (0 yield) and resets Step 3 for re-sowing") {
        SharedStackRuntime fail_rt{};
        REQUIRE(fail_rt.ApproveOrAdaptGovernanceDecision("dec:pea_cultivation", true));
        REQUIRE(fail_rt.CompleteTask("task:fab_1_mill_raised_bed"));
        REQUIRE(fail_rt.CompleteTask("task:pea_1_plow_amend"));
        REQUIRE(fail_rt.CompleteTask("task:pea_2_erect_trellis"));
        REQUIRE(fail_rt.CompleteTask("task:pea_3_rhizobium_sow"));

        // Inject severe blight failure and advance time without executing 4R remediation
        REQUIRE(fail_rt.InjectGardenIssue("CROP_FAILURE_BLIGHT"));
        REQUIRE(FindWorkflowById(fail_rt, "wf:pea_cultivation")->diagnosed_issue_code == kStackIssueCropFailure);
        REQUIRE(FindWorkflowById(fail_rt, "wf:remediate_pea_cultivation") != nullptr);
        REQUIRE(FindTaskById(fail_rt, "task:pea_4r_remediate_issue")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);

        REQUIRE(fail_rt.CompleteTask("task:pea_4_irrigate_weed"));
        for (uint32_t i = 0U; i < 6U; ++i) {
            fail_rt.AdvanceSimClockSeconds(10800ULL);
            if (FindTaskById(fail_rt, "task:pea_4_irrigate_weed")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN) {
                REQUIRE(fail_rt.CompleteTask("task:pea_4_irrigate_weed"));
            }
            if (FindTaskById(fail_rt, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN) {
                break;
            }
        }

        const L4WorkflowInstance* wf_fail = FindWorkflowById(fail_rt, "wf:pea_cultivation");
        REQUIRE(wf_fail->crop_health_bp <= 2000U);
        REQUIRE(FindTaskById(fail_rt, "task:pea_5_harvest_pods_roots")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
        REQUIRE(fail_rt.CompleteTask("task:pea_5_harvest_pods_roots"));

        wf_fail = FindWorkflowById(fail_rt, "wf:pea_cultivation");
        REQUIRE(wf_fail->last_harvest_outcome_tier == kStackOutcomeFailure);
        REQUIRE(fail_rt.HarvestedSplitPeaBatches() == 0U);
        REQUIRE(fail_rt.HarvestedMirepoixBatches() == 0U);
        // Step 3 (Sow) is automatically re-armed as ACTIVE_TOKEN so the Steward can re-sow the cleared bed!
        REQUIRE(FindTaskById(fail_rt, "task:pea_3_rhizobium_sow")->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
    }
}

// =============================================================================
// PILLAR 9: Player-Decided Garden Plot Scope, Suburban Soil & Pest/Water Physics,
//           Strict Workflow Resource Gating, and Simulated L7 Green Amazon + Legacy Bank
// =============================================================================

TEST_CASE("Pillar 9: Player-Decided Garden Plot Scope, Suburban Soil & Pest/Water Physics, Strict Workflow Resource Gating, and Simulated L7 Green Amazon + Legacy Bank",
          "[modular_engine][native_stack][pillar9]") {
    using namespace oasis::stack;
    using namespace oasis::engine;

    SECTION("Player-decided physical Garden Plot Scope gates wf:pea_cultivation and scales L4 physical resource requirements & L5 governance targets") {
        SharedStackRuntime rt{};

        // Approve dec:pea_cultivation with await_plot_scope: true (as the UI does)
        rt.DispatchJsonCommand(
            "approve_decision",
            R"({"decision_id":"dec:pea_cultivation","approve":true,"await_plot_scope":true})"
        );

        const L4WorkflowInstance* pea_wf = FindWorkflowById(rt, "wf:pea_cultivation");
        REQUIRE(pea_wf != nullptr);
        REQUIRE(pea_wf->plot_scope_defined == 0U);

        // Step 1 of wf:pea_cultivation is BLOCKED_BY_PRECONDITION until the player decides where and how big the garden plot is!
        const L4TaskItem* step1 = FindTaskById(rt, "task:pea_1_plow_amend");
        REQUIRE(step1 != nullptr);
        REQUIRE(step1->bpmn_token_state == BpmnTokenState::BLOCKED_BY_PRECONDITION);
        REQUIRE(step1->precondition_blocker_note.find("Pending Physical Plot Scope") != std::string::npos);
        REQUIRE_FALSE(rt.CompleteTask("task:pea_1_plow_amend"));

        // Player decides a 2m x 3m (6 m2 -> scale_units = 3) plot at backyard coordinates (21, 1, 6)
        REQUIRE(rt.SetGardenPlotScope(21, 6, 2U, 3U));
        pea_wf = FindWorkflowById(rt, "wf:pea_cultivation");
        REQUIRE(pea_wf->plot_scope_defined == 1U);
        REQUIRE(pea_wf->plot_origin_x == 21);
        REQUIRE(pea_wf->plot_origin_z == 6);
        REQUIRE(pea_wf->plot_area_m2 == 6U);
        REQUIRE(pea_wf->plot_scale_units == 3U);

        // L4 task coordinates and required physical resource quantities are scaled by scale_units = 3!
        const L4TaskItem* fab_bed = FindTaskById(rt, "task:fab_1_mill_raised_bed");
        step1 = FindTaskById(rt, "task:pea_1_plow_amend");
        const L4TaskItem* step3 = FindTaskById(rt, "task:pea_3_rhizobium_sow");
        REQUIRE(fab_bed != nullptr);
        REQUIRE(step1 != nullptr);
        REQUIRE(step3 != nullptr);
        REQUIRE(step1->location_label.find("(21,1,6)") != std::string::npos);
        REQUIRE(fab_bed->required_input_item_id == "item:cedar_timber_plank");
        REQUIRE(fab_bed->required_input_qty == 12U); // 4 planks * 3 scale_units = 12 planks
        REQUIRE(step1->required_input_item_id == "item:organic_compost_bag");
        REQUIRE(step1->required_input_qty == 3U);    // 1 bag * 3 scale_units = 3 bags
        REQUIRE(step3->required_input_item_id == "item:pea_seed_packet");
        REQUIRE(step3->required_input_qty == 3U);    // 1 packet * 3 scale_units = 3 packets

        // Because starter timber stock is 4 (< 12 required), fab_bed is BLOCKED_BY_PRECONDITION on missing physical resources
        // and L5 dynamically spawns wf:source_garden_inputs!
        REQUIRE(rt.TimberPlanksStock() == 4U);
        REQUIRE(fab_bed->bpmn_token_state == BpmnTokenState::BLOCKED_BY_PRECONDITION);
        REQUIRE_FALSE(rt.CompleteTask("task:fab_1_mill_raised_bed"));
        REQUIRE(FindWorkflowById(rt, "wf:source_garden_inputs") != nullptr);

        // Player buys missing cedar planks, compost bags, and pea seed packets from Simulated L7 Green Amazon via Legacy Bank!
        const int64_t initial_bank_cents = rt.HouseholdReserveCents();
        REQUIRE(rt.BuyFromGreenAmazon("item:cedar_timber_plank", 4U));   // +8 planks (4 * $18.00 = $72.00) -> 12 planks total
        REQUIRE(rt.BuyFromGreenAmazon("item:organic_compost_bag", 1U));  // +2 bags ($14.00)   -> 4 bags total
        REQUIRE(rt.BuyFromGreenAmazon("item:pea_seed_packet", 1U));      // +2 packets ($12.00) -> 4 packets total
        REQUIRE(rt.HouseholdReserveCents() == initial_bank_cents - (7200 + 1400 + 1200));
        REQUIRE(rt.BankTransactions().size() >= 3U);
        REQUIRE(rt.TimberPlanksStock() == 12U);
        REQUIRE(rt.CompostAmendmentBags() == 4U);
        REQUIRE(rt.PeaSeedPackets() == 4U);

        // Now fab_bed is unblocked and consumes the 12 cedar planks!
        fab_bed = FindTaskById(rt, "task:fab_1_mill_raised_bed");
        REQUIRE(fab_bed->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
        REQUIRE(rt.CompleteTask("task:fab_1_mill_raised_bed"));
        REQUIRE(rt.TimberPlanksStock() == 0U);
        REQUIRE(rt.RaisedBedFramesBuilt() == 3U);

        // Suburban soil starts degraded (850 bp organic carbon, 14 ppm N); submitting observable soil color & texture calibrates Step 1!
        pea_wf = FindWorkflowById(rt, "wf:pea_cultivation");
        REQUIRE(pea_wf->soil_organic_carbon_bp == 850U);
        REQUIRE(pea_wf->estimated_nitrogen_ppm == 14U);
        REQUIRE(rt.SubmitSoilBaselineObservation("PALE_GREY_BROWN", "COMPACTED_CLAY_CLODS"));

        step1 = FindTaskById(rt, "task:pea_1_plow_amend");
        REQUIRE(step1->bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN);
        REQUIRE(rt.CompleteTask("task:pea_1_plow_amend"));
        REQUIRE(rt.CompostAmendmentBags() == 1U); // 4 - 3 = 1 bag remaining
        pea_wf = FindWorkflowById(rt, "wf:pea_cultivation");
        REQUIRE(pea_wf->soil_organic_carbon_bp == 3100U); // Calibrated (+700 bp over 2400 bp uncalibrated)
        REQUIRE(pea_wf->estimated_nitrogen_ppm == 54U);   // Calibrated (+12 ppm over 42 ppm uncalibrated)

        // Erect trellis (Step 2) and sow peas (Step 3 consumes 3 pea seed packets; cannot plant without seeds!)
        REQUIRE(rt.CompleteTask("task:pea_2_erect_trellis"));
        REQUIRE(rt.CompleteTask("task:pea_3_rhizobium_sow"));
        REQUIRE(rt.PeaSeedPackets() == 1U); // 4 - 3 = 1 packet remaining

        // Being too proactive and manually watering when soil is already moist (3600 bp + 10L -> >= 5400 bp)
        // causes OVERWATERED_ROOT_ROT and dynamically spawns wf:remediate_pea_cultivation!
        REQUIRE(rt.ProactiveManualWaterGarden(30000U));
        pea_wf = FindWorkflowById(rt, "wf:pea_cultivation");
        REQUIRE(pea_wf->estimated_moisture_bp >= 5400U);
        REQUIRE(pea_wf->active_issue_code == kStackIssueOverwatered);
        REQUIRE(pea_wf->diagnosed_issue_code == kStackIssueOverwatered);
        REQUIRE(FindWorkflowById(rt, "wf:remediate_pea_cultivation") != nullptr);

        // Remediate over-watering root rot via Step 4R
        REQUIRE(rt.CompleteTask("task:pea_4r_remediate_issue"));
        pea_wf = FindWorkflowById(rt, "wf:pea_cultivation");
        REQUIRE(pea_wf->active_issue_code == kStackIssueNone);
        REQUIRE(pea_wf->estimated_moisture_bp <= 4200U);
    }

    SECTION("Do-Nothing Baseline drains Legacy Bank via automatic Green Amazon grocery orders & monthly bills, while Green Amazon surplus sales offset bills") {
        SharedStackRuntime idle_rt{};
        REQUIRE(idle_rt.GetWorkflows().empty());
        const int64_t start_reserve = idle_rt.HouseholdReserveCents();
        REQUIRE(idle_rt.PantryGroceryMeals() == 6U);

        // Advance 30 simulated days (30 * 86400s) with the player doing nothing in L5/L4:
        // Steward eats 2 meals/day (60 meals total), auto-orders 9 packs of 6 meals ($36.00 each = $324.00) from Green Amazon,
        // and pays 1 monthly bill ($220.00) at Day 30 via Legacy Bank!
        for (uint32_t day = 0U; day < 30U; ++day) {
            idle_rt.AdvanceSimClockSeconds(86400ULL);
        }

        REQUIRE(idle_rt.MonthsBillsPaid() == 1U);
        REQUIRE(idle_rt.CumulativeL7BillsPaidCents() == 22000ULL);
        REQUIRE(idle_rt.CumulativeL7GrocerySpendCents() >= 28800ULL);
        REQUIRE(idle_rt.HouseholdReserveCents() < start_reserve - 50000LL);
        REQUIRE_FALSE(idle_rt.BankTransactions().empty());

        // Now verify selling surplus on Green Amazon deposits funds into Legacy Bank and offsets monthly bills
        SharedStackRuntime active_rt{};
        REQUIRE(active_rt.ApproveOrAdaptGovernanceDecision("dec:pea_soup_hearth", true));
        // Complete input sourcing & cook Pea Soup Tiffins
        REQUIRE(active_rt.CompleteTask("task:source_1_procure_soup_inputs"));
        REQUIRE(active_rt.CompleteTask("task:soup_1_gather_inputs"));
        REQUIRE(active_rt.CompleteTask("task:soup_2_simmer_bone_broth"));
        REQUIRE(active_rt.CompleteTask("task:soup_3_simmer_pea_soup_74c"));
        REQUIRE(active_rt.CompleteTask("task:soup_4_package_distribute_tiffins"));
        REQUIRE(active_rt.ServedPeaSoupTiffinBatches() >= 1U);

        const int64_t before_sale_cents = active_rt.HouseholdReserveCents();
        REQUIRE(active_rt.SellOnGreenAmazon("item:pea_soup_tiffin", 1U));
        REQUIRE(active_rt.HouseholdReserveCents() == before_sale_cents + 2800LL);

        // Verify JSON serialization of Legacy Bank and Green Amazon endpoints
        const std::string bank_json = active_rt.SerializeLegacyBankJson();
        const std::string ga_json   = active_rt.SerializeGreenAmazonCatalogJson();
        REQUIRE(bank_json.find("\"bank_account_id\":\"BANK-LOT402-CHECKING\"") != std::string::npos);
        REQUIRE(ga_json.find("\"marketplace_id\":\"L7-GREEN-AMAZON-REST-V1\"") != std::string::npos);
        REQUIRE(ga_json.find("\"item:pea_seed_packet\"") != std::string::npos);
    }
}


