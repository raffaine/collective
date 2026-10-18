#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <glm/glm.hpp>
#include <flecs.h>
#include <cstdint>
#include <vector>

#include "core/voxel_pathfinder.hpp"
#include "core/fabrication.hpp"
#include "core/chill_loop.hpp"
#include "core/spatial_designator.hpp"
#include "core/raycaster.hpp"

using namespace oasis;
using namespace oasis::pathfinding;
using namespace oasis::fabrication;
using namespace oasis::chill;
using namespace oasis::designator;
using namespace oasis::graphics;

TEST_CASE("Test 1: 3D Voxel A* Doorway Routing & Wall Collision", "[sprint-13.6][pathfinding][astar]") {
    GetDynamicVoxelStore().clear();

    SECTION("Study (13, 1, 6) to Backyard (21, 1, 6) routes through Back Patio Door (17, 1, 6) and avoids walls/rack") {
        const glm::ivec3 start(13, 1, 6);
        const glm::ivec3 goal(21, 1, 6);

        const VoxelPath path = ComputeVoxelPath(start, goal);
        REQUIRE(path.valid);
        REQUIRE(!path.empty());
        CHECK(path.waypoints.front() == start);
        CHECK(path.waypoints.back() == goal);

        // Must route through the Back Patio Door at (17, 1, 6)
        CHECK(path.ContainsWaypoint(glm::ivec3(17, 1, 6)));

        // Zero waypoints may pass through solid walls (5u, 7u, 10u) or Node 0 Server Rack (12, 1, 6)
        for (const auto& wp : path.waypoints) {
            CHECK(wp != glm::ivec3(12, 1, 6));
            const uint32_t mat = GetVoxelMaterialAt(wp);
            CHECK(mat != 5u);
            CHECK(mat != 7u);
            CHECK(mat != 10u);
            CHECK(IsVoxelPassableForAgent(wp));
            CHECK(IsVoxelStandable(wp));
        }
    }

    SECTION("Study (13, 1, 6) to Driveway Salvage Crate (8, 1, 3) routes through Front Door (9, 1, 6)") {
        // Stamp the Salvage Crate at (8, 1, 3) as done in Genesis Camp bootstrap
        SetDynamicVoxel(glm::ivec3(8, 1, 3), static_cast<uint8_t>(MAT_RATION_CRATE));

        const glm::ivec3 start(13, 1, 6);
        const glm::ivec3 crate_target(8, 1, 3);

        const VoxelPath path = ComputeVoxelPath(start, crate_target);
        REQUIRE(path.valid);
        REQUIRE(!path.empty());

        // Must route through the Front Door at (9, 1, 6)
        CHECK(path.ContainsWaypoint(glm::ivec3(9, 1, 6)));

        // Goal waypoint must be adjacent to the solid salvage crate at (8, 1, 3)
        const glm::ivec3 final_wp = path.waypoints.back();
        const int manhattan_to_crate =
            std::abs(final_wp.x - crate_target.x) +
            std::abs(final_wp.y - crate_target.y) +
            std::abs(final_wp.z - crate_target.z);
        CHECK(manhattan_to_crate == 1);

        for (const auto& wp : path.waypoints) {
            CHECK(wp != glm::ivec3(12, 1, 6));
            const uint32_t mat = GetVoxelMaterialAt(wp);
            CHECK(mat != 5u);
            CHECK(mat != 7u);
            CHECK(mat != 10u);
        }

        GetDynamicVoxelStore().clear();
    }
}

TEST_CASE("Test 2: Exterior Roof Access Ladder (18, 1..4, 3) & Voxel Gravity", "[sprint-13.6][pathfinding][ladder][gravity]") {
    GetDynamicVoxelStore().clear();

    // 1. Verify Exterior Roof Access Ladder voxels (18, 1..4, 3) have material 32u (MAT_LADDER)
    for (int y = 1; y <= 4; ++y) {
        CHECK(GetVoxelMaterialAt(glm::ivec3(18, y, 3)) == 32u);
        CHECK(IsRoofAccessLadderVoxel(18, y, 3));
        CHECK(IsVoxelStandable(18, y, 3));
    }

    // 2. Compute path from Study (13, 1, 6) to Roof Solar Panel target (13, 5, 6)
    const glm::ivec3 start(13, 1, 6);
    const glm::ivec3 roof_target(13, 5, 6);
    VoxelPath path = ComputeVoxelPath(start, roof_target);

    REQUIRE(path.valid);
    REQUIRE(path.size() > 6);

    // Verify path climbs the Exterior Roof Access Ladder (18, 1..4, 3) and steps onto roof eave (17, 4, 3)
    CHECK(path.ContainsWaypoint(glm::ivec3(18, 1, 3)));
    CHECK(path.ContainsWaypoint(glm::ivec3(18, 2, 3)));
    CHECK(path.ContainsWaypoint(glm::ivec3(18, 3, 3)));
    CHECK(path.ContainsWaypoint(glm::ivec3(18, 4, 3)));
    CHECK(path.ContainsWaypoint(glm::ivec3(17, 4, 3)));
    CHECK(path.waypoints.back() == roof_target);

    // Verify ladder climb order is strictly ascending y = 1 -> 2 -> 3 -> 4
    int last_ladder_y = 0;
    for (const auto& wp : path.waypoints) {
        if (wp.x == 18 && wp.z == 3) {
            CHECK(wp.y > last_ladder_y);
            last_ladder_y = wp.y;
        }
    }
    CHECK(last_ladder_y == 4);

    // 3. Step an agent along the path via StepAgentAlongVoxelPath and verify grounded y physics
    glm::vec3 agent_pos(13.0f, 1.0f, 6.0f);
    bool reached_ladder = false;
    bool climbed_onto_roof = false;
    float max_y_observed = agent_pos.y;

    for (int tick = 0; tick < 2000 && !path.IsComplete(); ++tick) {
        const glm::ivec3 active_wp = path.waypoints[path.current_idx];
        StepAgentAlongVoxelPath(agent_pos, path, 4.0f, 1.0f / 60.0f);
        max_y_observed = std::max(max_y_observed, agent_pos.y);

        if (!reached_ladder && active_wp.x == 18 && active_wp.z == 3) {
            reached_ladder = true;
        }

        if (!reached_ladder) {
            // While walking across ground/interior floors, gravity keeps pos.y == 1.0f
            CHECK(agent_pos.y == Catch::Approx(1.0f).margin(1e-4f));
        } else if (active_wp.x == 18 && active_wp.z == 3) {
            // Smooth ascent on the ladder within [1.0, 4.0]
            CHECK(agent_pos.y >= 1.0f - 1e-4f);
            CHECK(agent_pos.y <= 4.0f + 1e-4f);
        } else if (active_wp.y >= 4 && agent_pos.y >= 3.95f) {
            climbed_onto_roof = true;
            CHECK(agent_pos.y >= 4.0f - 1e-3f);
        }
    }

    CHECK(path.IsComplete());
    CHECK(reached_ladder);
    CHECK(climbed_onto_roof);
    CHECK(max_y_observed >= 5.0f - 1e-3f);
    CHECK(agent_pos.y >= 4.0f);

    // 4. Verify idle gravity pulls an unsupported mid-air agent back down to y = 1.0f
    VoxelPath empty_path{};
    glm::vec3 floating_pos(13.0f, 2.8f, 6.0f);
    for (int i = 0; i < 60; ++i) {
        StepAgentAlongVoxelPath(floating_pos, empty_path, 3.0f, 1.0f / 60.0f);
    }
    CHECK(floating_pos.y == Catch::Approx(1.0f).margin(1e-4f));
}

TEST_CASE("Test 3: Resource-Gated Salvage Stockpile & Depletion Guard", "[sprint-13.6][fabrication][stockpile]") {
    GetDynamicVoxelStore().clear();
    flecs::world ecs;

    flecs::entity camp_entity = BootstrapGenesisSurvivalCamp(ecs);
    REQUIRE(camp_entity.is_alive());
    REQUIRE(camp_entity.has<GenesisCampStockComponent>());

    auto& stock = camp_entity.get_mut<GenesisCampStockComponent>();

    // Verify finite starter stock
    CHECK(stock.solar_kits_remaining == 2u);
    CHECK(stock.timber_planks_remaining == 6u);
    CHECK(stock.cob_earth_lumps == 4u);
    CHECK(stock.seed_packs_remaining == 6u);
    CHECK(stock.raw_produce_items == 2u);
    CHECK(stock.salvage_crate_pos == glm::ivec3(8, 1, 3));
    CHECK(GetVoxelMaterialAt(stock.salvage_crate_pos) == MAT_RATION_CRATE);

    // Verify 2 mature GardenCropComponent beds exist at (20, 1, 5) and (20, 1, 6)
    uint32_t mature_bed_count = 0;
    bool has_bed_20_1_5 = false;
    bool has_bed_20_1_6 = false;
    ecs.each([&](const GardenCropComponent& crop) {
        if (crop.is_mature) {
            mature_bed_count++;
            if (crop.pos() == glm::ivec3(20, 1, 5)) has_bed_20_1_5 = true;
            if (crop.pos() == glm::ivec3(20, 1, 6)) has_bed_20_1_6 = true;
        }
    });
    CHECK(mature_bed_count == 2u);
    CHECK(has_bed_20_1_5);
    CHECK(has_bed_20_1_6);
    CHECK(GetVoxelMaterialAt(glm::ivec3(20, 1, 5)) == 34u);
    CHECK(GetVoxelMaterialAt(glm::ivec3(20, 1, 6)) == 34u);

    // Verify ConsumeRequiredMaterialFromStock(stock, 1) succeeds twice (2 -> 1 -> 0) and fails on 3rd attempt
    REQUIRE(HasRequiredMaterialInStock(stock, MAT_REQ_SOLAR_PV_KIT));
    CHECK(ConsumeRequiredMaterialFromStock(stock, MAT_REQ_SOLAR_PV_KIT));
    CHECK(stock.solar_kits_remaining == 1u);

    REQUIRE(HasRequiredMaterialInStock(stock, MAT_REQ_SOLAR_PV_KIT));
    CHECK(ConsumeRequiredMaterialFromStock(stock, MAT_REQ_SOLAR_PV_KIT));
    CHECK(stock.solar_kits_remaining == 0u);

    // 3rd attempt must be blocked by depletion guard!
    CHECK_FALSE(HasRequiredMaterialInStock(stock, MAT_REQ_SOLAR_PV_KIT));
    CHECK_FALSE(ConsumeRequiredMaterialFromStock(stock, MAT_REQ_SOLAR_PV_KIT));
    CHECK(stock.solar_kits_remaining == 0u);

    // Verify refund restores 1 kit when a blueprint is cancelled before completion
    RefundRequiredMaterialToStock(stock, MAT_REQ_SOLAR_PV_KIT);
    CHECK(stock.solar_kits_remaining == 1u);
    CHECK(HasRequiredMaterialInStock(stock, MAT_REQ_SOLAR_PV_KIT));

    // Verify StepConstructionSystem blocks progress until materials_delivered == true
    auto founder = ecs.entity("Founder")
        .set<SomaticStateComponent>({2200.0, 2000.0 / 86400.0, 0.080, 1.0f, 1.0f, false, 0.90f, 37.0f, 22.0f});

    auto bp_entity = ecs.entity("GatedSolarBlueprint").set<BlueprintComponent>({
        InfrastructureType::SOLAR_PANEL,
        13, 5, 6,
        10.0, // required_work_ticks
        0.0,  // invested_work_ticks
        500.0,
        false,
        0.20f,
        0u,
        0u,
        0u,   // build_stage 0 (Ghost)
        true, // requires_material_haul
        false,// materials_delivered (not yet hauled!)
        MAT_REQ_SOLAR_PV_KIT
    });

    StepConstructionSystem(ecs, 1.0);
    CHECK(bp_entity.get<BlueprintComponent>().invested_work_ticks == Catch::Approx(0.0));

    // Deliver material to site -> Stage 1 (33u Scaffolding Frame)
    bp_entity.get_mut<BlueprintComponent>().materials_delivered = true;
    bp_entity.get_mut<BlueprintComponent>().build_stage = 1u;

    for (int i = 0; i < 5; ++i) {
        StepConstructionSystem(ecs, 1.0);
    }
    CHECK(bp_entity.get<BlueprintComponent>().invested_work_ticks == Catch::Approx(5.0));
    CHECK(bp_entity.get<BlueprintComponent>().build_stage == 2u);

    GetDynamicVoxelStore().clear();
}

TEST_CASE("Test 4: Scenario Gamma (The Sovereign Kitchen) Cooking & Consequential Metabolic Multipliers", "[sprint-13.6][kitchen][metabolism]") {
    GenesisCampStockComponent stock{};
    stock.raw_produce_items = 2u;
    stock.potable_water_liters = 60.0;
    stock.ration_meals_remaining = 12u;
    double solar_kwh = 1.50;

    // 1. Verify CookHomesteadMealAtKitchen consumes 1 raw_produce_items, 0.5 L water, and 0.25 kWh solar to produce +2 ration_meals_remaining
    REQUIRE(CookHomesteadMealAtKitchen(stock, solar_kwh));
    CHECK(stock.raw_produce_items == 1u);
    CHECK(stock.potable_water_liters == Catch::Approx(59.5));
    CHECK(solar_kwh == Catch::Approx(1.25));
    CHECK(stock.ration_meals_remaining == 14u);

    // Cook 2nd meal with remaining produce
    REQUIRE(CookHomesteadMealAtKitchen(stock, solar_kwh));
    CHECK(stock.raw_produce_items == 0u);
    CHECK(stock.potable_water_liters == Catch::Approx(59.0));
    CHECK(solar_kwh == Catch::Approx(1.00));
    CHECK(stock.ration_meals_remaining == 16u);

    // 3rd attempt fails cleanly when raw_produce_items == 0
    CHECK_FALSE(CookHomesteadMealAtKitchen(stock, solar_kwh));
    CHECK(stock.ration_meals_remaining == 16u);

    // 2. Verify EvaluateMetabolicSpeedMultiplier across Well-Fed, Hungry, Starving, and Dehydrated states
    SomaticStateComponent somatic{};

    // Well-fed (2000 kcal, 0.80 hydration) -> 1.0f
    somatic.caloric_reserve = 2000.0;
    somatic.hydration = 0.80f;
    CHECK(EvaluateMetabolicSpeedMultiplier(somatic) == Catch::Approx(1.0f));

    // Hungry (900 kcal, 0.80 hydration) -> 0.70f
    somatic.caloric_reserve = 900.0;
    somatic.hydration = 0.80f;
    CHECK(EvaluateMetabolicSpeedMultiplier(somatic) == Catch::Approx(0.70f));

    // Starving (400 kcal, 0.80 hydration) -> 0.40f
    somatic.caloric_reserve = 400.0;
    somatic.hydration = 0.80f;
    CHECK(EvaluateMetabolicSpeedMultiplier(somatic) == Catch::Approx(0.40f));

    // Dehydrated (2000 kcal, 0.10 hydration) -> 0.40f
    somatic.caloric_reserve = 2000.0;
    somatic.hydration = 0.10f;
    CHECK(EvaluateMetabolicSpeedMultiplier(somatic) == Catch::Approx(0.40f));

    // 3. Verify Spatial Designator Context Menus expose [Harvest Garden] on 34u and [Cook Meal (Scenario Γ)] on 24u
    RaycastHit crop_hit{true, {20, 1, 5}, {0, 1, 0}, {20, 2, 5}, 34u, 5.0f};
    const VoxelContextMenu crop_menu = EvaluateVoxelContext(crop_hit);
    CHECK(crop_menu.HasAction("[Harvest Garden]"));

    RaycastHit pantry_hit{true, {11, 1, 4}, {0, 1, 0}, {11, 2, 4}, 24u, 4.0f};
    const VoxelContextMenu pantry_menu = EvaluateVoxelContext(pantry_hit);
    CHECK(pantry_menu.HasAction("[Cook Meal (Scenario Γ)]"));
}
