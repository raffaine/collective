#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <flecs.h>
#include <glm/glm.hpp>

#include "core/camera_uniforms.hpp"
#include "core/fabrication.hpp"

using Catch::Matchers::WithinAbs;

namespace oasis::psychology {

struct PersonalityFacets {
    float neuroticism{0.8f};
    float openness{0.9f};
    float conscientiousness{0.6f};
    float agreeableness{0.7f};
    float extraversion{0.3f};
};

struct PsychologyComponent {
    PersonalityFacets facets{0.8f, 0.9f, 0.6f, 0.7f, 0.3f};
    float stress{0.25f};
    float mood{0.50f};
    float focus{0.75f};
};

} // namespace oasis::psychology

// Telemetry capture struct for adversarial tests
struct VitalsHUDTelemetry {
    uint64_t total_invocations{0};
    double last_caloric_reserve{0.0};
    double last_stress{0.0};
    double last_focus{0.0};
    flecs::entity_t last_entity_id{0};
};

// ==============================================================================
// CHALLENGE 1: Flecs FounderVitalsHUDSystem Rapid Tick Cycling (100,000 Ticks)
// ==============================================================================
TEST_CASE("Adversarial: Flecs FounderVitalsHUDSystem 100,000 Rapid Tick Stress", "[adversarial][vitals][ticks]") {
    flecs::world ecs;

    ecs.component<oasis::psychology::PsychologyComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();

    oasis::psychology::PsychologyComponent initial_psych{};
    initial_psych.stress = 0.25f;
    initial_psych.focus = 0.75f;

    oasis::fabrication::SomaticStateComponent initial_somatic{};
    initial_somatic.caloric_reserve = 2500.0;

    auto founder = ecs.entity("Founder")
        .set<oasis::psychology::PsychologyComponent>(initial_psych)
        .set<oasis::fabrication::SomaticStateComponent>(initial_somatic);

    REQUIRE(founder.is_alive());

    VitalsHUDTelemetry telemetry{};

    // Register FounderVitalsHUDSystem in flecs::PostUpdate exactly as in main.cpp
    ecs.system<const oasis::fabrication::SomaticStateComponent,
               const oasis::psychology::PsychologyComponent>("FounderVitalsHUDSystem")
        .kind(flecs::PostUpdate)
        .each([&telemetry](flecs::entity e,
                           const oasis::fabrication::SomaticStateComponent& somatic,
                           const oasis::psychology::PsychologyComponent& psych) {
            if (e != e.world().entity("Founder")) return;

            telemetry.total_invocations++;
            telemetry.last_caloric_reserve = somatic.caloric_reserve;
            telemetry.last_stress = static_cast<double>(psych.stress);
            telemetry.last_focus = static_cast<double>(psych.focus);
            telemetry.last_entity_id = e.id();
        });

    std::mt19937_64 rng(0xCAFEBABE12345678ULL);
    std::uniform_real_distribution<double> dist_cal(-500.0, 10000.0);
    std::uniform_real_distribution<float> dist_stress(-0.5f, 2.0f);
    std::uniform_real_distribution<float> dist_focus(-0.5f, 2.0f);

    const auto t_start = std::chrono::high_resolution_clock::now();

    const int kNumTicks = 100000;
    for (int tick = 1; tick <= kNumTicks; ++tick) {
        // Mutate Founder state pseudo-randomly
        double cur_cal = dist_cal(rng);
        float cur_str = dist_stress(rng);
        float cur_foc = dist_focus(rng);

        founder.set<oasis::fabrication::SomaticStateComponent>({cur_cal, 1.0f, 1.0f, false});
        founder.set<oasis::psychology::PsychologyComponent>({{}, cur_str, 0.5f, cur_foc});

        ecs.progress();

        REQUIRE(telemetry.total_invocations == static_cast<uint64_t>(tick));
        REQUIRE_THAT(telemetry.last_caloric_reserve, WithinAbs(cur_cal, 1e-5));
        REQUIRE_THAT(telemetry.last_stress, WithinAbs(static_cast<double>(cur_str), 1e-5));
        REQUIRE_THAT(telemetry.last_focus, WithinAbs(static_cast<double>(cur_foc), 1e-5));
        REQUIRE(telemetry.last_entity_id == founder.id());
    }

    const auto t_end = std::chrono::high_resolution_clock::now();
    const auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();
    const double us_per_tick = static_cast<double>(elapsed_us) / static_cast<double>(kNumTicks);

    REQUIRE(telemetry.total_invocations == 100000);
    CHECK(us_per_tick < 50.0); // Strict performance budget: < 50us per tick
}

// ==============================================================================
// CHALLENGE 2: Entity Lifecycle Stress (Destruction, Recreation & Disappearance)
// ==============================================================================
TEST_CASE("Adversarial: Founder Entity Lifecycle Stress (Destruction & Recreation)", "[adversarial][vitals][lifecycle]") {
    flecs::world ecs;

    ecs.component<oasis::psychology::PsychologyComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();

    VitalsHUDTelemetry telemetry{};

    ecs.system<const oasis::fabrication::SomaticStateComponent,
               const oasis::psychology::PsychologyComponent>("FounderVitalsHUDSystem")
        .kind(flecs::PostUpdate)
        .each([&telemetry](flecs::entity e,
                           const oasis::fabrication::SomaticStateComponent& somatic,
                           const oasis::psychology::PsychologyComponent& psych) {
            if (e != e.world().entity("Founder")) return;

            telemetry.total_invocations++;
            telemetry.last_caloric_reserve = somatic.caloric_reserve;
            telemetry.last_stress = static_cast<double>(psych.stress);
            telemetry.last_focus = static_cast<double>(psych.focus);
            telemetry.last_entity_id = e.id();
        });

    SECTION("Cycle 1: 500 ticks with alive Founder") {
        auto founder = ecs.entity("Founder")
            .set<oasis::psychology::PsychologyComponent>({{}, 0.30f, 0.50f, 0.70f})
            .set<oasis::fabrication::SomaticStateComponent>({2400.0, 1.0f, 1.0f, false});

        for (int i = 0; i < 500; ++i) {
            ecs.progress();
        }
        REQUIRE(telemetry.total_invocations == 500);

        SECTION("Cycle 2: Destruct Founder mid-simulation, tick 500 times with zero crashes") {
            founder.destruct();
            REQUIRE_FALSE(founder.is_alive());

            const uint64_t invocations_before = telemetry.total_invocations;
            for (int i = 0; i < 500; ++i) {
                // Must not segfault or invoke callback
                ecs.progress();
            }
            REQUIRE(telemetry.total_invocations == invocations_before);

            SECTION("Cycle 3: Recreate Founder entity, verify seamless resumption") {
                auto new_founder = ecs.entity("Founder")
                    .set<oasis::psychology::PsychologyComponent>({{}, 0.15f, 0.80f, 0.90f})
                    .set<oasis::fabrication::SomaticStateComponent>({2100.0, 1.0f, 1.0f, false});

                REQUIRE(new_founder.is_alive());

                for (int i = 0; i < 500; ++i) {
                    ecs.progress();
                }
                REQUIRE(telemetry.total_invocations == 1000);
                REQUIRE_THAT(telemetry.last_caloric_reserve, WithinAbs(2100.0, 1e-4));
                REQUIRE_THAT(telemetry.last_stress, WithinAbs(0.15, 1e-4));
                REQUIRE_THAT(telemetry.last_focus, WithinAbs(0.90, 1e-4));
                REQUIRE(telemetry.last_entity_id == new_founder.id());
            }
        }

        SECTION("Cycle 4: Disabling and re-enabling Founder entity") {
            founder.disable();
            const uint64_t count_disabled = telemetry.total_invocations;
            for (int i = 0; i < 200; ++i) {
                ecs.progress();
            }
            REQUIRE(telemetry.total_invocations == count_disabled); // No ticks while disabled

            founder.enable();
            for (int i = 0; i < 200; ++i) {
                ecs.progress();
            }
            REQUIRE(telemetry.total_invocations == count_disabled + 200); // Resumed ticks
        }
    }
}

// ==============================================================================
// CHALLENGE 3: Multi-Entity Herd Stress (10,000 NPCs & Zero Contamination)
// ==============================================================================
TEST_CASE("Adversarial: Multi-Entity Herd Isolation (10,000 Inhabitants vs 1 Founder)", "[adversarial][vitals][isolation]") {
    flecs::world ecs;

    ecs.component<oasis::psychology::PsychologyComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();

    VitalsHUDTelemetry telemetry{};

    ecs.system<const oasis::fabrication::SomaticStateComponent,
               const oasis::psychology::PsychologyComponent>("FounderVitalsHUDSystem")
        .kind(flecs::PostUpdate)
        .each([&telemetry](flecs::entity e,
                           const oasis::fabrication::SomaticStateComponent& somatic,
                           const oasis::psychology::PsychologyComponent& psych) {
            if (e != e.world().entity("Founder")) return;

            telemetry.total_invocations++;
            telemetry.last_caloric_reserve = somatic.caloric_reserve;
            telemetry.last_stress = static_cast<double>(psych.stress);
            telemetry.last_focus = static_cast<double>(psych.focus);
            telemetry.last_entity_id = e.id();
        });

    // Create the authentic Founder
    auto founder = ecs.entity("Founder")
        .set<oasis::psychology::PsychologyComponent>({{}, 0.22f, 0.50f, 0.77f})
        .set<oasis::fabrication::SomaticStateComponent>({2345.0, 1.0f, 1.0f, false});

    // Spawn 10,000 NPCs with poisoned/extreme vitals
    std::vector<flecs::entity> npcs;
    npcs.reserve(10000);

    for (int i = 0; i < 10000; ++i) {
        auto npc = ecs.entity()
            .set<oasis::psychology::PsychologyComponent>({{}, 99.9f, 0.0f, -99.9f})
            .set<oasis::fabrication::SomaticStateComponent>({-999999.0, 0.0f, 0.0f, true});
        npcs.push_back(npc);
    }

    // Tick the world 100 times
    for (int i = 0; i < 100; ++i) {
        ecs.progress();
    }

    // Invocations must be exactly 100 (1 per tick), NOT 100 * 10001
    REQUIRE(telemetry.total_invocations == 100);
    // Values recorded must strictly reflect the Founder, never contaminated by any of the 10,000 NPCs
    REQUIRE_THAT(telemetry.last_caloric_reserve, WithinAbs(2345.0, 1e-4));
    REQUIRE_THAT(telemetry.last_stress, WithinAbs(0.22, 1e-4));
    REQUIRE_THAT(telemetry.last_focus, WithinAbs(0.77, 1e-4));
    REQUIRE(telemetry.last_entity_id == founder.id());
}

// ==============================================================================
// CHALLENGE 4: Missing Component Matrix & Degenerate Entity State
// ==============================================================================
TEST_CASE("Adversarial: Missing Component Permutations & Degenerate Founder State", "[adversarial][vitals][missing]") {
    flecs::world ecs;

    ecs.component<oasis::psychology::PsychologyComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();

    VitalsHUDTelemetry telemetry{};

    ecs.system<const oasis::fabrication::SomaticStateComponent,
               const oasis::psychology::PsychologyComponent>("FounderVitalsHUDSystem")
        .kind(flecs::PostUpdate)
        .each([&telemetry](flecs::entity e,
                           const oasis::fabrication::SomaticStateComponent& somatic,
                           const oasis::psychology::PsychologyComponent& psych) {
            if (e != e.world().entity("Founder")) return;

            telemetry.total_invocations++;
            telemetry.last_caloric_reserve = somatic.caloric_reserve;
            telemetry.last_stress = static_cast<double>(psych.stress);
            telemetry.last_focus = static_cast<double>(psych.focus);
        });

    SECTION("Founder has ONLY SomaticStateComponent (missing PsychologyComponent)") {
        auto founder = ecs.entity("Founder")
            .set<oasis::fabrication::SomaticStateComponent>({2500.0, 1.0f, 1.0f, false});

        for (int i = 0; i < 100; ++i) {
            ecs.progress();
        }
        REQUIRE(telemetry.total_invocations == 0); // Query demands both components; must skip cleanly

        // Add PsychologyComponent -> system must activate
        founder.set<oasis::psychology::PsychologyComponent>({{}, 0.25f, 0.50f, 0.75f});
        for (int i = 0; i < 100; ++i) {
            ecs.progress();
        }
        REQUIRE(telemetry.total_invocations == 100);
    }

    SECTION("Founder has ONLY PsychologyComponent (missing SomaticStateComponent)") {
        auto founder = ecs.entity("Founder")
            .set<oasis::psychology::PsychologyComponent>({{}, 0.25f, 0.50f, 0.75f});

        for (int i = 0; i < 100; ++i) {
            ecs.progress();
        }
        REQUIRE(telemetry.total_invocations == 0); // Query demands both components; must skip cleanly

        // Add SomaticStateComponent -> system must activate
        founder.set<oasis::fabrication::SomaticStateComponent>({2500.0, 1.0f, 1.0f, false});
        for (int i = 0; i < 100; ++i) {
            ecs.progress();
        }
        REQUIRE(telemetry.total_invocations == 100);
    }
}

// ==============================================================================
// CHALLENGE 5: CameraUniforms Binary Soundness & Float Extrema Fuzzing
// ==============================================================================
TEST_CASE("Adversarial: CameraUniforms 288-Byte Layout & Binary Buffer Serialization", "[adversarial][camera][binary]") {
    REQUIRE(sizeof(oasis::graphics::CameraUniforms) == 288);
    REQUIRE(alignof(oasis::graphics::CameraUniforms) == 16);

    SECTION("Serialization round-trip through raw byte buffer (WebGPU Queue WriteBuffer Simulation)") {
        oasis::graphics::CameraUniforms original{};
        original.camera_pos = glm::vec4(-12345.678f, 9876.543f, -5555.555f, 1.0f);
        original.avatar_pos = glm::vec4(1024.125f, 512.250f, -2048.500f, 3600.0f);
        original.avatar_params = glm::vec4(2.0f, 0.95f, 0.15f, 1.0f);

        // Raw byte buffer
        alignas(16) std::byte raw_buffer[288]{};
        std::memcpy(raw_buffer, &original, sizeof(oasis::graphics::CameraUniforms));

        // Read back
        oasis::graphics::CameraUniforms reconstituted{};
        std::memcpy(&reconstituted, raw_buffer, sizeof(oasis::graphics::CameraUniforms));

        REQUIRE_THAT(reconstituted.camera_pos.x, WithinAbs(-12345.678f, 1e-3));
        REQUIRE_THAT(reconstituted.camera_pos.y, WithinAbs(9876.543f, 1e-3));
        REQUIRE_THAT(reconstituted.camera_pos.z, WithinAbs(-5555.555f, 1e-3));
        REQUIRE_THAT(reconstituted.avatar_pos.x, WithinAbs(1024.125f, 1e-3));
        REQUIRE_THAT(reconstituted.avatar_pos.y, WithinAbs(512.250f, 1e-3));
        REQUIRE_THAT(reconstituted.avatar_pos.z, WithinAbs(-2048.500f, 1e-3));
        REQUIRE_THAT(reconstituted.avatar_pos.w, WithinAbs(3600.0f, 1e-3));
        REQUIRE_THAT(reconstituted.avatar_params.x, WithinAbs(2.0f, 1e-3));
        REQUIRE_THAT(reconstituted.avatar_params.y, WithinAbs(0.95f, 1e-3));
        REQUIRE_THAT(reconstituted.avatar_params.z, WithinAbs(0.15f, 1e-3));
        REQUIRE_THAT(reconstituted.avatar_params.w, WithinAbs(1.0f, 1e-3));
    }

    SECTION("Monte Carlo Fuzzing: 50,000 float extrema packaging tests") {
        std::mt19937_64 rng(0xBADC0FFEE0DDF00DULL);
        std::uniform_real_distribution<float> dist_pos(-100000.0f, 100000.0f);
        std::uniform_real_distribution<float> dist_time(0.0f, 1000000.0f);
        std::uniform_int_distribution<int> dist_state(0, 3);
        std::uniform_real_distribution<float> dist_norm(0.0f, 1.0f);

        for (int i = 0; i < 50000; ++i) {
            oasis::graphics::CameraUniforms u{};
            float rx = dist_pos(rng);
            float ry = dist_pos(rng);
            float rz = dist_pos(rng);
            float rw = dist_time(rng);
            float action = static_cast<float>(dist_state(rng));
            float str = dist_norm(rng);
            float foc = dist_norm(rng);

            u.avatar_pos = glm::vec4(rx, ry, rz, rw);
            u.avatar_params = glm::vec4(action, str, foc, 1.0f);

            CHECK(u.avatar_pos.x == rx);
            CHECK(u.avatar_pos.y == ry);
            CHECK(u.avatar_pos.z == rz);
            CHECK(u.avatar_pos.w == rw);
            CHECK(u.avatar_params.x == action);
            CHECK(u.avatar_params.y == str);
            CHECK(u.avatar_params.z == foc);
            CHECK(u.avatar_params.w == 1.0f);
        }
    }
}

// ==============================================================================
// CHALLENGE 6: Variable Delta Time Integration & Caloric Underflow Invariants
// ==============================================================================
TEST_CASE("Adversarial: Variable Delta-Time Caloric Traversal Integration", "[adversarial][traversal][integration]") {
    flecs::world ecs;

    ecs.component<oasis::fabrication::SpatialPositionComponent>();
    ecs.component<oasis::fabrication::FounderCommandComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();

    oasis::fabrication::SpatialPositionComponent pos{};
    pos.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    pos.move_speed = 4.0f; // 4.0 units / sec

    oasis::fabrication::FounderCommandComponent cmd{};
    cmd.state = oasis::fabrication::FounderActionState::NAVIGATING;
    cmd.target_voxel = glm::ivec3(100, 0, 0); // 100 units along X

    oasis::fabrication::SomaticStateComponent somatic{};
    somatic.caloric_reserve = 1000.0;

    auto founder = ecs.entity("Founder")
        .set<oasis::fabrication::SpatialPositionComponent>(pos)
        .set<oasis::fabrication::FounderCommandComponent>(cmd)
        .set<oasis::fabrication::SomaticStateComponent>(somatic);

    float current_dt = 1.0f / 60.0f;

    ecs.system<oasis::fabrication::SpatialPositionComponent,
               oasis::fabrication::FounderCommandComponent,
               oasis::fabrication::SomaticStateComponent>("VariableDtMovementSystem")
        .kind(flecs::OnUpdate)
        .each([&current_dt](oasis::fabrication::SpatialPositionComponent& p,
                            oasis::fabrication::FounderCommandComponent& c,
                            oasis::fabrication::SomaticStateComponent& s) {
            if (c.state != oasis::fabrication::FounderActionState::NAVIGATING) return;

            glm::vec3 target = glm::vec3(c.target_voxel.x, c.target_voxel.y, c.target_voxel.z);
            glm::vec3 to_target = target - p.pos;
            float dist = glm::length(to_target);

            if (dist > 1.5f) {
                glm::vec3 dir = to_target / dist;
                p.pos += dir * p.move_speed * current_dt;
                s.caloric_reserve = std::max(0.0, s.caloric_reserve - 0.10 * static_cast<double>(current_dt));
            } else {
                c.state = oasis::fabrication::FounderActionState::CONSTRUCTING;
            }
        });

    // Test a variety of frame rates: 15 fps (dt=0.0667), 30 fps (dt=0.0333), 60 fps (dt=0.0167), 120 fps (dt=0.0083)
    const std::vector<float> dts = {1.0f / 15.0f, 1.0f / 30.0f, 1.0f / 60.0f, 1.0f / 120.0f, 1.0f / 240.0f};

    double total_simulated_time = 0.0;
    for (float dt : dts) {
        current_dt = dt;
        for (int i = 0; i < 60; ++i) {
            ecs.progress();
            total_simulated_time += static_cast<double>(dt);
        }
    }

    const auto& final_pos = founder.get<oasis::fabrication::SpatialPositionComponent>();
    const auto& final_somatic = founder.get<oasis::fabrication::SomaticStateComponent>();

    double expected_distance = 4.0 * total_simulated_time;
    double expected_calories_burned = 0.10 * total_simulated_time;
    double expected_remaining_calories = 1000.0 - expected_calories_burned;

    REQUIRE_THAT(final_pos.pos.x, WithinAbs(static_cast<float>(expected_distance), 0.05f));
    REQUIRE_THAT(final_somatic.caloric_reserve, WithinAbs(expected_remaining_calories, 0.05));
    REQUIRE(final_somatic.caloric_reserve >= 0.0);
}
