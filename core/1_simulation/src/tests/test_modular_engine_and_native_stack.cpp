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
// PILLAR 6: Native Stack Runtime & col-swarmd Peer-Cluster Swarm
// =============================================================================

TEST_CASE("Pillar 6: SharedStackRuntime L6/L5/L4/L3/L2/L7 lifecycle & col-swarmd zero-sum BCC peer-cluster swarm",
          "[native_stack][swarmd][bcc]") {
    using namespace oasis::stack;

    SharedStackRuntime runtime{};

    // 1. L6 Need search, L7 fallback request, and user-defined new mesh need
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

    // 2. L5 polycentric governance approval spawns 6-step Pea Cultivation & 4-step Pea Soup L4 lifecycles
    REQUIRE(FindWorkflowById(runtime, "wf:pea_cultivation") == nullptr);
    REQUIRE(FindWorkflowById(runtime, "wf:pea_soup_hearth") == nullptr);

    REQUIRE(runtime.ApproveOrAdaptGovernanceDecision("dec:pea_cultivation", true));
    REQUIRE(runtime.ApproveOrAdaptGovernanceDecision("dec:pea_soup_hearth", true));

    const L4WorkflowInstance* pea_wf  = FindWorkflowById(runtime, "wf:pea_cultivation");
    const L4WorkflowInstance* soup_wf = FindWorkflowById(runtime, "wf:pea_soup_hearth");
    REQUIRE(pea_wf != nullptr);
    REQUIRE(soup_wf != nullptr);
    REQUIRE(pea_wf->tasks.size() == 6u);
    REQUIRE(soup_wf->tasks.size() == 4u);

    // Local L5/L4 sourcing now fulfills Pea Soup at $0 L7 fiat spend
    const int64_t reserve_after_ratification = runtime.GetLedger().household_reserve_cents;
    const L6IntentRequest local_req = runtime.RequestNeed("need:pea_soup", NeedTimingMode::NOW, 0u, 1u);
    REQUIRE(local_req.resolved_sourcing_mode == SourcingMode::R0_LOCAL_WORKFLOWS);
    REQUIRE(runtime.GetLedger().household_reserve_cents == reserve_after_ratification);

    // Answer unmodeled micro-detail prompt on step 3 (Rhizobium inoculation)
    REQUIRE(runtime.AnswerUnmodeledMicroDetailPrompt(
        "task:pea_3_rhizobium_sow",
        "Dampened seeds and coated with Rhizobium slurry in shade before sowing 3.5 cm deep."
    ));
    const L4TaskItem* rhizobium_task = FindTaskById(runtime, "task:pea_3_rhizobium_sow");
    REQUIRE(rhizobium_task != nullptr);
    REQUIRE(rhizobium_task->unmodeled_prompt_resolved == 1u);

    // 3. L2 pin ingestion switches task:pea_4_irrigate_weed from MANUAL_HUMAN_OBSERVATION to AUTOMATED_L2_EXECUTION
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

    // Once moisture reaches >= 4200 bp, closed-loop L2 control closes irrigation_valve_v1
    (void)runtime.IngestL2PinsPayload(
        R"({"soil_probe_v1":{"is_online":true,"moisture_bp":4350,"temperature_mk":291150,"nitrogen_ppm":55}})"
    );
    valve_reg = runtime.GetL2TelemetryTable().FindRegister("irrigation_valve_v1");
    REQUIRE(valve_reg != nullptr);
    REQUIRE(valve_reg->actuator_commanded_on == 0u);
    REQUIRE(valve_reg->actuator_level_bp == 0u);

    // 4. R1 task claiming & pairwise zero-sum BCC exergy clearing by col-swarmd peer clusters
    REQUIRE(runtime.ReleaseTaskToR1Peers("task:pea_1_plow_amend"));
    REQUIRE(runtime.ReleaseTaskToR1Peers("task:soup_4_package_distribute_tiffins"));

    oasis::swarm::SwarmState swarm{0xC011EC717E2026ULL};
    REQUIRE(swarm.clusters.size() == 3u);

    // First swarm step claims released R1 tasks
    oasis::swarm::StepPeerClusterSwarm(runtime, swarm, runtime.CurrentTick() + 1ULL);
    const L4TaskItem* claimed_pea  = FindTaskById(runtime, "task:pea_1_plow_amend");
    const L4TaskItem* claimed_soup = FindTaskById(runtime, "task:soup_4_package_distribute_tiffins");
    REQUIRE(claimed_pea != nullptr);
    REQUIRE(claimed_soup != nullptr);
    REQUIRE(claimed_pea->task_commitment_state == TaskCommitmentState::CLAIMED_BY_PEER_CLUSTER);
    REQUIRE(claimed_soup->task_commitment_state == TaskCommitmentState::CLAIMED_BY_PEER_CLUSTER);
    REQUIRE_FALSE(claimed_pea->claimed_by_cluster_id.empty());

    // Subsequent swarm steps complete claimed R1 tasks, settle zero-sum BCC exergy, and emit L6/L3 peer events
    for (uint32_t step = 0u; step < 5u; ++step) {
        oasis::swarm::StepPeerClusterSwarm(runtime, swarm, runtime.CurrentTick() + 1ULL);
    }

    claimed_pea  = FindTaskById(runtime, "task:pea_1_plow_amend");
    claimed_soup = FindTaskById(runtime, "task:soup_4_package_distribute_tiffins");
    REQUIRE(claimed_pea->task_commitment_state == TaskCommitmentState::COMPLETED);
    REQUIRE(claimed_soup->task_commitment_state == TaskCommitmentState::COMPLETED);
    REQUIRE(swarm.total_bcc_settled_uj == (800000000ULL + 500000000ULL));
    REQUIRE(swarm.total_events_emitted >= 4ULL);
    REQUIRE(runtime.GetLedger().VerifyPairwiseZeroSumInvariant());
    REQUIRE_FALSE(runtime.GetLedger().mesh_weather_feed.empty());

    uint32_t total_mutual_tasks = 0u;
    for (const auto& ch : runtime.GetLedger().peer_bcc_channels) {
        REQUIRE(ch.IsZeroSumValid());
        REQUIRE((ch.balance_local_to_peer_uj + ch.balance_peer_to_local_uj) == 0);
        total_mutual_tasks += ch.completed_mutual_tasks;
    }
    REQUIRE(total_mutual_tasks == 2u);

    // 5. +$180 (18,000 cents) L7 custom woodwork bill-offset accounting
    const int64_t reserve_before_woodwork = runtime.GetLedger().household_reserve_cents;
    REQUIRE(runtime.GetLedger().monthly_l7_offset_revenue_cents == 0);

    REQUIRE(runtime.CompleteTask("task:fab_4_l7_custom_woodwork_order"));
    REQUIRE(runtime.GetLedger().monthly_l7_offset_revenue_cents == 18000);
    REQUIRE(runtime.GetLedger().household_reserve_cents == (reserve_before_woodwork + 18000));
    REQUIRE(runtime.GetLedger().completed_l7_offset_orders == 1u);
    REQUIRE(runtime.GetLedger().NetMonthlyCashflowCents() == -4000); // $180 offset against $220 legacy bills

    const std::string snapshot_json = runtime.SerializeFullNodeSnapshotJson();
    REQUIRE(snapshot_json.find("\"bcc_zero_sum_invariant_ok\": true") != std::string::npos);
    REQUIRE(snapshot_json.find("\"monthly_l7_offset_revenue_cents\": 18000") != std::string::npos);
}
