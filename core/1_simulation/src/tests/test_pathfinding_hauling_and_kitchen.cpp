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

TEST_CASE("Test 5: Sprint 18 Scenario Alpha + Gamma Backyard Pea Farming, Poultry Husbandry, Bone Broth & Communal Pea Soup", "[sprint-18][gamma][alpha][husbandry][peasoup]") {
    GenesisCampStockComponent stock{};
    stock.backyard_coop_built = false;
    stock.pea_trellis_built = false;
    stock.timber_planks_remaining = 6u;
    stock.potable_water_liters = 60.0;
    stock.ration_meals_remaining = 12u;
    stock.split_peas_and_beans = 4u;
    stock.root_mirepoix_veggies = 2u;
    stock.bsfl_larvae_and_scraps = 3u;
    stock.bone_ash_calcium_grit = 0u;
    stock.flock_hens_count = 4u;
    stock.fresh_eggs_count = 2u;
    stock.chicken_meat_portions = 0u;
    stock.poultry_broth_bones = 1u;
    stock.bone_broth_jars = 1u;
    stock.communal_pea_soup_tiffins = 0u;
    stock.community_meals_served_total = 0u;
    stock.biochar_kiln_char_batches = 0u;
    stock.terra_preta_batches_charged = 0u;

    double solar_kwh = 5.0;
    uint32_t reputation_bp = 100u;
    uint32_t value_tokens = 25u;

    // 1. Scenario Alpha (1) Garage Fabrication: Build Backyard Chicken Coop & Keyline Pea/Bean Trellis
    CHECK_FALSE(stock.backyard_coop_built);
    CHECK_FALSE(stock.pea_trellis_built);
    REQUIRE(BuildOrUpgradeCoopAndTrellisInGarage(stock));
    CHECK(stock.backyard_coop_built);
    CHECK(stock.pea_trellis_built);
    CHECK(stock.timber_planks_remaining == 5u);
    CHECK(stock.flock_hens_count == 5u);

    // 2. Scenario Gamma (3) Backyard Pea/Bean & Root Mirepoix Harvest
    const uint32_t peas_before_harvest = stock.split_peas_and_beans;
    const uint32_t roots_before_harvest = stock.root_mirepoix_veggies;
    const uint32_t scraps_before_harvest = stock.bsfl_larvae_and_scraps;
    REQUIRE(HarvestPeaAndRootGarden(stock));
    CHECK(stock.split_peas_and_beans == peas_before_harvest + 3u);
    CHECK(stock.root_mirepoix_veggies == roots_before_harvest + 2u);
    CHECK(stock.bsfl_larvae_and_scraps == scraps_before_harvest + 2u);

    // 3. Trophic Non-Competition Poultry Husbandry: Tend Backyard Flock & Collect Eggs / Heritage Meat / Bones
    const uint32_t human_peas_before_tend = stock.split_peas_and_beans;
    const uint32_t human_meals_before_tend = stock.ration_meals_remaining;
    const uint32_t scraps_before_tend = stock.bsfl_larvae_and_scraps;
    const uint32_t eggs_before_tend = stock.fresh_eggs_count;
    const uint32_t meat_before_tend = stock.chicken_meat_portions;
    const uint32_t bones_before_tend = stock.poultry_broth_bones;
    REQUIRE(TendBackyardFlockAndCollectEggs(stock));
    // Must consume BSFL/scraps and NEVER touch human-grade split peas/beans or ration meals
    CHECK(stock.bsfl_larvae_and_scraps == scraps_before_tend - 1u);
    CHECK(stock.split_peas_and_beans == human_peas_before_tend);
    CHECK(stock.ration_meals_remaining == human_meals_before_tend);
    CHECK(stock.fresh_eggs_count == eggs_before_tend + 4u);
    CHECK(stock.chicken_meat_portions == meat_before_tend + 1u);
    CHECK(stock.poultry_broth_bones == bones_before_tend + 1u);

    // 4. Closed-Loop Calcium & Collagen Cycle: Simmer Collagen Bone Broth
    const uint32_t bones_before_broth = stock.poultry_broth_bones;
    const uint32_t broth_before = stock.bone_broth_jars;
    const uint32_t grit_before = stock.bone_ash_calcium_grit;
    REQUIRE(SimmerCollagenBoneBroth(stock, solar_kwh));
    CHECK(stock.poultry_broth_bones == bones_before_broth - 1u);
    CHECK(stock.bone_broth_jars == broth_before + 3u);
    CHECK(stock.bone_ash_calcium_grit == grit_before + 2u);
    CHECK(solar_kwh == Catch::Approx(4.70));

    // 5. Cook Communal Pea Soup & Heritage Poultry/Egg Potage Batch
    const uint32_t peas_before_soup = stock.split_peas_and_beans;
    const uint32_t roots_before_soup = stock.root_mirepoix_veggies;
    const uint32_t broth_before_soup = stock.bone_broth_jars;
    const uint32_t meat_before_soup = stock.chicken_meat_portions;
    const uint32_t meals_before_soup = stock.ration_meals_remaining;
    REQUIRE(CookCommunalPeaSoupBatch(stock, solar_kwh));
    CHECK(stock.split_peas_and_beans == peas_before_soup - 1u);
    CHECK(stock.root_mirepoix_veggies == roots_before_soup - 1u);
    CHECK(stock.bone_broth_jars == broth_before_soup - 1u);
    CHECK(stock.chicken_meat_portions == meat_before_soup - 1u);
    CHECK(stock.communal_pea_soup_tiffins == 4u);
    CHECK(stock.ration_meals_remaining == meals_before_soup + 3u);
    CHECK(solar_kwh == Catch::Approx(4.35));

    // 6. Serve Communal Pea Soup to Neighborhood (Ring R0 Baseline Hospitality & L3 Bounty)
    REQUIRE(ServeCommunalPeaSoupToNeighborhood(stock, reputation_bp, value_tokens));
    CHECK(stock.communal_pea_soup_tiffins == 0u);
    CHECK(stock.community_meals_served_total == 4u);
    CHECK(reputation_bp == 500u);
    CHECK(value_tokens == 40u);

    // 7. Scenario Mu (12): TLUD Biochar Kiln, 55°C Thermophilic Composting Drum, Struvite Recovery & Living Terra Preta Soil
    CHECK(stock.biochar_kiln_char_batches == 0u);
    CHECK(stock.terra_preta_batches_charged == 0u);
    const uint32_t peas_before_mu = stock.split_peas_and_beans;
    const uint32_t roots_before_mu = stock.root_mirepoix_veggies;
    REQUIRE(RunScenarioMuBiocharAndThermophilicCompost(stock));
    CHECK(stock.biochar_kiln_char_batches == 1u);
    CHECK(stock.thermophilic_compost_temp_mk >= 328150u);
    CHECK(stock.struvite_phosphorus_recovery_bp >= 9000u);
    CHECK(stock.terra_preta_soil_moisture_bp >= 4000u);
    CHECK(stock.terra_preta_batches_charged == 1u);
    CHECK(stock.split_peas_and_beans == peas_before_mu + 1u);
    CHECK(stock.root_mirepoix_veggies == roots_before_mu + 1u);
}

TEST_CASE("Test 6: Sprint 19 Clean-Slate Backyard, 1-to-5 Biological Feel Tiers & Granular L4 Lifecycle Workflows", "[sprint-19][feel][clean-slate][workflows]") {
    flecs::world ecs;
    auto camp_entity = BootstrapCleanSuburbanGenesisLot(ecs);
    REQUIRE(camp_entity.is_alive());
    auto& stock = camp_entity.get_mut<GenesisCampStockComponent>();

    // 1. Clean-slate backyard on Day 1: no garden beds, coop, kiln, or compost drum built yet
    CHECK_FALSE(stock.garden_bed_built);
    CHECK_FALSE(stock.garden_planted);
    CHECK_FALSE(stock.backyard_coop_built);
    CHECK_FALSE(stock.compost_drum_built);
    CHECK_FALSE(stock.biochar_kiln_built);
    CHECK(GetDynamicVoxel(20, 1, 5) == 0u);
    CHECK(GetDynamicVoxel(22, 1, 11) == 0u);
    CHECK(GetDynamicVoxel(23, 1, 8) == 0u);
    CHECK(GetDynamicVoxel(23, 1, 9) == 0u);

    // 2. 1-to-5 Biological Feel Tier mapping (epistemic privacy without L2 biometric sensor)
    SomaticVitalsComponent vitals{};
    vitals.caloric_stamina_kcal = 2100.0f;
    vitals.fatigue_factor = 0.15f;
    vitals.hydration_pct = 0.85f;
    vitals.focus_pool = 0.82f;
    vitals.stress_accumulator = 0.12f;
    vitals.entertainment_comfort = 0.80f;
    vitals.social_comfort = 0.78f;
    const BiologicalFeelTiers feel_thriving = ComputeBiologicalFeelTiers(vitals, 0.78f, false, false);
    CHECK(feel_thriving.hunger_tier >= 4u);
    CHECK(feel_thriving.energy_tier >= 4u);
    CHECK(feel_thriving.focus_tier >= 4u);
    CHECK(feel_thriving.mood_tier >= 4u);
    CHECK(feel_thriving.social_tier >= 4u);
    CHECK(feel_thriving.l2_biometric_sensor_installed == 0u);

    vitals.caloric_stamina_kcal = 300.0f;
    vitals.fatigue_factor = 0.90f;
    vitals.hydration_pct = 0.15f;
    vitals.focus_pool = 0.10f;
    vitals.stress_accumulator = 0.92f;
    vitals.entertainment_comfort = 0.10f;
    const BiologicalFeelTiers feel_depleted = ComputeBiologicalFeelTiers(vitals, 0.10f, false, false);
    CHECK(feel_depleted.hunger_tier == 1u);
    CHECK(feel_depleted.energy_tier == 1u);
    CHECK(feel_depleted.focus_tier == 1u);
    CHECK(feel_depleted.mood_tier == 1u);
    CHECK(feel_depleted.social_tier == 1u);

    // 3. Granular L4 Gardening Lifecycle: Mill Bed in Garage -> Sow Peas -> Manual Observation -> Cleanup
    REQUIRE(FabricateGardenBedKitInGarage(stock));
    CHECK(stock.garden_bed_built);
    CHECK(GetDynamicVoxel(20, 1, 5) == static_cast<uint8_t>(MAT_COVER_CROP));
    CHECK(GetDynamicVoxel(20, 1, 6) == static_cast<uint8_t>(MAT_COVER_CROP));

    REQUIRE(PlantBackyardPeaAndRootCrop(stock));
    CHECK(stock.garden_planted);
    CHECK(GetDynamicVoxel(20, 1, 4) == static_cast<uint8_t>(oasis::chill::MAT_MATURE_CROP));
    CHECK(GetDynamicVoxel(20, 1, 5) == static_cast<uint8_t>(oasis::chill::MAT_MATURE_CROP));

    const uint32_t peas_before_obs = stock.split_peas_and_beans;
    REQUIRE(ObserveAndTendGardenManually(stock));
    CHECK(stock.split_peas_and_beans == peas_before_obs + 1u);

    const uint32_t scraps_before_clean = stock.bsfl_larvae_and_scraps;
    REQUIRE(CleanupAndTurnGardenBed(stock));
    CHECK(stock.bsfl_larvae_and_scraps >= scraps_before_clean + 2u);
    CHECK(GetDynamicVoxel(20, 1, 4) == static_cast<uint8_t>(MAT_COVER_CROP));

    // 4. Build Poultry Coop & Biochar/Compost Drum into DynamicVoxelStore
    REQUIRE(BuildOrUpgradeCoopAndTrellisInGarage(stock));
    CHECK(stock.backyard_coop_built);
    CHECK(GetDynamicVoxel(22, 1, 11) == static_cast<uint8_t>(MAT_POULTRY_COOP));
    CHECK(GetDynamicVoxel(21, 1, 11) == static_cast<uint8_t>(MAT_CHICKEN_HEN));

    REQUIRE(RunScenarioMuBiocharAndThermophilicCompost(stock));
    CHECK(stock.compost_drum_built);
    CHECK(stock.biochar_kiln_built);
    CHECK(GetDynamicVoxel(23, 1, 8) == static_cast<uint8_t>(MAT_MU_BIOCHAR_KILN));
    CHECK(GetDynamicVoxel(23, 1, 9) == static_cast<uint8_t>(MAT_MU_COMPOST_DRUM));
}
