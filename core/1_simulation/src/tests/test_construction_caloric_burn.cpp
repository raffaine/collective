#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <flecs.h>
#include <spdlog/spdlog.h>
#include <cmath>
#include "core/fabrication.hpp"

using Catch::Matchers::WithinAbs;

TEST_CASE("Founder Caloric Burn During Construction in Flecs ECS", "[oasis][ecs][fabrication][calories]") {
    flecs::world ecs;

    // Register components
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<oasis::fabrication::BlueprintComponent>();
    ecs.component<oasis::fabrication::VoxelPlacedComponent>();

    SECTION("Founder initializes with healthy baseline somatic reserves") {
        auto founder = ecs.entity("Founder")
            .set<oasis::fabrication::SomaticStateComponent>({3000.0, 0.023, 0.080, 1.0f, 1.0f, false});

        REQUIRE(founder.has<oasis::fabrication::SomaticStateComponent>());
        const auto& somatic = founder.get<oasis::fabrication::SomaticStateComponent>();
        REQUIRE(somatic.caloric_reserve == 3000.0);
        REQUIRE(somatic.focus == 1.0f);
        REQUIRE_FALSE(somatic.is_exhausted);
    }

    SECTION("Active construction strictly depletes calories monotonically") {
        auto founder = ecs.entity("Founder")
            .set<oasis::fabrication::SomaticStateComponent>({3000.0, 0.023, 0.080, 1.0f, 1.0f, false});

        auto bp = ecs.entity("SolarBlueprint")
            .set<oasis::fabrication::BlueprintComponent>({
                oasis::fabrication::InfrastructureType::SOLAR_PANEL,
                10, 0, 10, 50.0, 0.0, 400.0, false
            });

        double prev_calories = 3000.0;
        for (int tick = 1; tick <= 25; ++tick) {
            oasis::fabrication::StepConstructionSystem(ecs, 1.0);
            const auto& somatic = founder.get<oasis::fabrication::SomaticStateComponent>();
            REQUIRE(somatic.caloric_reserve < prev_calories);
            REQUIRE(somatic.focus < 1.0f);
            prev_calories = somatic.caloric_reserve;
        }

        const auto& somatic = founder.get<oasis::fabrication::SomaticStateComponent>();
        // 25 ticks * 0.080 kcal = 2.0 kcal burn
        REQUIRE_THAT(somatic.caloric_reserve, WithinAbs(3000.0 - 2.0, 1e-4));
        REQUIRE(somatic.focus < 1.0f);
    }

    SECTION("Blueprint transitions to completed voxel state upon work fulfillment") {
        auto founder = ecs.entity("Founder")
            .set<oasis::fabrication::SomaticStateComponent>({3000.0, 0.023, 0.080, 1.0f, 1.0f, false});

        auto bp = ecs.entity("RainwaterBlueprint")
            .set<oasis::fabrication::BlueprintComponent>({
                oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT,
                5, 0, 5, 20.0, 0.0, 200.0, false
            });

        for (int tick = 1; tick <= 20; ++tick) {
            oasis::fabrication::StepConstructionSystem(ecs, 1.0);
        }

        // Blueprint entity should be transformed/destructed
        REQUIRE_FALSE(bp.is_alive());

        // Completed voxel entity should now exist
        bool voxel_found = false;
        ecs.each([&](flecs::entity e, const oasis::fabrication::VoxelPlacedComponent& v) {
            if (v.type == oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT && v.x == 5 && v.z == 5) {
                voxel_found = true;
            }
        });
        REQUIRE(voxel_found);
    }

    SECTION("Caloric starvation guard halts construction when reserve is depleted") {
        // Founder at starvation threshold (500 kcal)
        auto founder = ecs.entity("Founder")
            .set<oasis::fabrication::SomaticStateComponent>({500.0, 0.023, 0.080, 0.2f, 0.1f, false});

        auto bp = ecs.entity("SolarBlueprint")
            .set<oasis::fabrication::BlueprintComponent>({
                oasis::fabrication::InfrastructureType::SOLAR_PANEL,
                10, 0, 10, 50.0, 0.0, 400.0, false
            });

        oasis::fabrication::StepConstructionSystem(ecs, 1.0);

        const auto& somatic = founder.get<oasis::fabrication::SomaticStateComponent>();
        REQUIRE(somatic.is_exhausted);
        REQUIRE(somatic.caloric_reserve == 500.0); // No burn occurred because work was blocked

        const auto& blueprint = bp.get<oasis::fabrication::BlueprintComponent>();
        REQUIRE(blueprint.invested_work_ticks == 0.0); // Construction did not advance
    }

    SECTION("Direct Flecs world system registration matches Founder without flecs::entity in template pack") {
        ecs.component<oasis::fabrication::SpatialPositionComponent>();
        ecs.component<oasis::fabrication::FounderCommandComponent>();

        auto founder = ecs.entity("Founder")
            .set<oasis::fabrication::SpatialPositionComponent>({glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), 3.0f})
            .set<oasis::fabrication::FounderCommandComponent>({oasis::fabrication::FounderActionState::CONSTRUCTING, glm::ivec3(10, 0, 10), {}})
            .set<oasis::fabrication::SomaticStateComponent>({3000.0, 0.023, 0.080, 1.0f, 1.0f, false});

        bool system_matched = false;
        flecs::entity matched_entity{};
        ecs.system<oasis::fabrication::SpatialPositionComponent,
                   oasis::fabrication::FounderCommandComponent,
                   oasis::fabrication::SomaticStateComponent>("TestDirectConstructionSystem")
            .kind(flecs::OnUpdate)
            .each([&](flecs::entity e,
                      oasis::fabrication::SpatialPositionComponent& pos,
                      oasis::fabrication::FounderCommandComponent& cmd,
                      oasis::fabrication::SomaticStateComponent& somatic) {
                system_matched = true;
                matched_entity = e;
            });

        ecs.progress();
        REQUIRE(system_matched);
        REQUIRE(matched_entity == founder);
    }
}
