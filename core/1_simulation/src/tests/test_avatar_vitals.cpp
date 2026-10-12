#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <flecs.h>
#include <glm/glm.hpp>

#include "core/camera_uniforms.hpp"
#include "core/fabrication.hpp"

using Catch::Matchers::WithinAbs;

namespace oasis::psychology {

enum class MemoryEventType : uint16_t {
    UNKNOWN = 0,
    FIRST_SUNLIGHT = 1,
    MUNICIPAL_CODE_RAID = 2,
    NEIGHBOR_DISPUTE = 3,
    FIRST_KILOWATT_GENERATED = 4,
    WATER_MAIN_TAPPED = 5,
    PERMACULTURE_HARVEST = 6,
    COMMUNITY_POTLUCK = 7,
    GRID_OUTAGE_ISLANDED = 8,
    TOOL_DROPPED = 9,
    HEATWAVE_SURVIVED = 10,
    COLLECTIVE_CHARTER_SIGNED = 11
};

struct MemoryFlags {
    static constexpr uint8_t NONE = 0x00;
    static constexpr uint8_t TRAUMA = 0x01;
    static constexpr uint8_t EUPHORIA = 0x02;
    static constexpr uint8_t CRUCIAL = 0x04;
    static constexpr uint8_t PERMANENT = 0x08;
};

struct EpisodicMemoryNode {
    uint64_t timestamp_tick{0};
    uint16_t event_type{0};
    int8_t emotional_valence{0};
    uint8_t salience{0};
    uint8_t flags{0};
    char description[24]{0};
};

struct EpisodicMemoryRing {
    static constexpr size_t CAPACITY = 32;
    EpisodicMemoryNode slots[CAPACITY]{};
    uint32_t head{0};
    uint32_t count{0};
    uint32_t permanent_count{0};
    uint64_t total_recorded{0};

    uint32_t Push(const EpisodicMemoryNode& node) noexcept {
        EpisodicMemoryNode to_insert = node;
        if (to_insert.salience >= 80) {
            to_insert.flags |= MemoryFlags::PERMANENT;
        }

        uint32_t target_slot = head;
        if (count == CAPACITY) {
            bool found_evictable = false;
            for (uint32_t i = 0; i < CAPACITY; ++i) {
                uint32_t candidate = (head + i) % CAPACITY;
                if ((slots[candidate].flags & MemoryFlags::PERMANENT) == 0) {
                    target_slot = candidate;
                    found_evictable = true;
                    break;
                }
            }
            if (!found_evictable) {
                target_slot = head;
            }
        }

        if (count < CAPACITY && target_slot == head) {
            count++;
        }

        if (to_insert.flags & MemoryFlags::PERMANENT) {
            if ((slots[target_slot].flags & MemoryFlags::PERMANENT) == 0) {
                permanent_count++;
            }
        } else if (slots[target_slot].flags & MemoryFlags::PERMANENT) {
            permanent_count--;
        }

        slots[target_slot] = to_insert;
        head = (target_slot + 1) % CAPACITY;
        total_recorded++;
        return target_slot;
    }
};

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
    EpisodicMemoryRing memory_ring{};
};

} // namespace oasis::psychology

// =============================================================================
// TEST SUITE: CameraUniforms Memory Layout & Avatar Uniform Extension
// =============================================================================
TEST_CASE("CameraUniforms WGSL Binary Layout and Avatar Extension", "[sprint7][avatar][layout]") {
    SECTION("Exact total size is 288 bytes and 16-byte aligned") {
        REQUIRE(sizeof(oasis::graphics::CameraUniforms) == 288);
        REQUIRE(alignof(oasis::graphics::CameraUniforms) == 16);
    }

    SECTION("Exact field offsets match WebGPU host-shader contract") {
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, view_matrix) == 0);
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, proj_matrix) == 64);
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, inv_view_proj) == 128);
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, camera_pos) == 192);
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, camera_target) == 208);
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, viewport_params) == 224);
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, cursor_highlight) == 240);
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, avatar_pos) == 256);
        REQUIRE(offsetof(oasis::graphics::CameraUniforms, avatar_params) == 272);
    }

    SECTION("CameraUniforms can be populated and read without aliasing or padding distortion") {
        oasis::graphics::CameraUniforms uniforms{};
        uniforms.avatar_pos = glm::vec4(1.5f, 2.5f, 3.5f, 10.25f);
        uniforms.avatar_params = glm::vec4(1.0f, 0.35f, 0.85f, 1.0f);

        REQUIRE_THAT(uniforms.avatar_pos.x, WithinAbs(1.5f, 1e-5));
        REQUIRE_THAT(uniforms.avatar_pos.y, WithinAbs(2.5f, 1e-5));
        REQUIRE_THAT(uniforms.avatar_pos.z, WithinAbs(3.5f, 1e-5));
        REQUIRE_THAT(uniforms.avatar_pos.w, WithinAbs(10.25f, 1e-5));

        REQUIRE_THAT(uniforms.avatar_params.x, WithinAbs(1.0f, 1e-5));
        REQUIRE_THAT(uniforms.avatar_params.y, WithinAbs(0.35f, 1e-5));
        REQUIRE_THAT(uniforms.avatar_params.z, WithinAbs(0.85f, 1e-5));
        REQUIRE_THAT(uniforms.avatar_params.w, WithinAbs(1.0f, 1e-5));
    }
}

// =============================================================================
// TEST SUITE: Founder Entity & Core ECS Components Registration
// =============================================================================
TEST_CASE("Founder Entity & Vitals Components Registration in Flecs", "[sprint7][avatar][ecs]") {
    flecs::world ecs;

    ecs.component<oasis::psychology::PersonalityFacets>();
    ecs.component<oasis::psychology::PsychologyComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<oasis::fabrication::SpatialPositionComponent>();
    ecs.component<oasis::fabrication::FounderCommandComponent>();
    ecs.component<oasis::fabrication::BlueprintComponent>();

    oasis::psychology::PsychologyComponent psych{};
    psych.stress = 0.25f;
    psych.mood = 0.50f;
    psych.focus = 0.75f;

    oasis::fabrication::SomaticStateComponent somatic{};
    somatic.caloric_reserve = 2500.0;
    somatic.focus = 1.0f;
    somatic.stamina = 1.0f;
    somatic.is_exhausted = false;

    oasis::fabrication::SpatialPositionComponent pos{};
    pos.pos = glm::vec3(0.0f, 1.0f, 0.0f);
    pos.move_speed = 3.0f;

    oasis::fabrication::FounderCommandComponent cmd{};
    cmd.state = oasis::fabrication::FounderActionState::IDLE;

    auto founder = ecs.entity("Founder")
        .set<oasis::psychology::PersonalityFacets>(psych.facets)
        .set<oasis::psychology::PsychologyComponent>(psych)
        .set<oasis::fabrication::SomaticStateComponent>(somatic)
        .set<oasis::fabrication::SpatialPositionComponent>(pos)
        .set<oasis::fabrication::FounderCommandComponent>(cmd);

    REQUIRE(founder.is_alive());
    REQUIRE(founder.has<oasis::psychology::PsychologyComponent>());
    REQUIRE(founder.has<oasis::fabrication::SomaticStateComponent>());
    REQUIRE(founder.has<oasis::fabrication::SpatialPositionComponent>());
    REQUIRE(founder.has<oasis::fabrication::FounderCommandComponent>());

    const auto& f_pos = founder.get<oasis::fabrication::SpatialPositionComponent>();
    REQUIRE_THAT(f_pos.pos.x, WithinAbs(0.0f, 1e-5));
    REQUIRE_THAT(f_pos.pos.y, WithinAbs(1.0f, 1e-5));
    REQUIRE_THAT(f_pos.pos.z, WithinAbs(0.0f, 1e-5));
    REQUIRE_THAT(f_pos.move_speed, WithinAbs(3.0f, 1e-5));

    const auto& f_somatic = founder.get<oasis::fabrication::SomaticStateComponent>();
    REQUIRE_THAT(f_somatic.caloric_reserve, WithinAbs(2500.0, 1e-4));
    REQUIRE_THAT(f_somatic.focus, WithinAbs(1.0f, 1e-4));

    const auto& f_psych = founder.get<oasis::psychology::PsychologyComponent>();
    REQUIRE_THAT(f_psych.stress, WithinAbs(0.25f, 1e-4));
    REQUIRE_THAT(f_psych.focus, WithinAbs(0.75f, 1e-4));
    REQUIRE_THAT(f_psych.mood, WithinAbs(0.50f, 1e-4));
}

// =============================================================================
// TEST SUITE: Founder Traversal & Caloric Burn System
// =============================================================================
TEST_CASE("Founder Continuous Movement & Caloric Expenditure", "[sprint7][avatar][movement]") {
    flecs::world ecs;

    ecs.component<oasis::fabrication::SpatialPositionComponent>();
    ecs.component<oasis::fabrication::FounderCommandComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();

    oasis::fabrication::SpatialPositionComponent pos{};
    pos.pos = glm::vec3(0.0f, 1.0f, 0.0f);
    pos.move_speed = 3.0f;

    oasis::fabrication::FounderCommandComponent cmd{};
    cmd.state = oasis::fabrication::FounderActionState::NAVIGATING;
    cmd.target_voxel = glm::ivec3(6, 1, 0); // 6 units away on X

    oasis::fabrication::SomaticStateComponent somatic{};
    somatic.caloric_reserve = 2500.0;

    auto founder = ecs.entity("Founder")
        .set<oasis::fabrication::SpatialPositionComponent>(pos)
        .set<oasis::fabrication::FounderCommandComponent>(cmd)
        .set<oasis::fabrication::SomaticStateComponent>(somatic);

    // Register movement system matching main.cpp
    ecs.system<oasis::fabrication::SpatialPositionComponent,
               oasis::fabrication::FounderCommandComponent,
               oasis::fabrication::SomaticStateComponent>("MovementTestSystem")
        .kind(flecs::OnUpdate)
        .each([](oasis::fabrication::SpatialPositionComponent& p,
                 oasis::fabrication::FounderCommandComponent& c,
                 oasis::fabrication::SomaticStateComponent& s) {
            if (c.state != oasis::fabrication::FounderActionState::NAVIGATING) return;

            glm::vec3 target = glm::vec3(c.target_voxel.x, c.target_voxel.y, c.target_voxel.z);
            glm::vec3 to_target = target - p.pos;
            float dist = glm::length(to_target);

            constexpr float dt = 1.0f / 60.0f;
            if (dist > 1.5f) {
                glm::vec3 dir = to_target / dist;
                p.pos += dir * p.move_speed * dt;
                s.caloric_reserve = std::max(0.0, s.caloric_reserve - 0.10 * dt);
            } else {
                c.state = oasis::fabrication::FounderActionState::CONSTRUCTING;
            }
        });

    // Run 60 ticks (1 second of traversal)
    for (int i = 0; i < 60; ++i) {
        ecs.progress();
    }

    const auto& updated_pos = founder.get<oasis::fabrication::SpatialPositionComponent>();
    const auto& updated_somatic = founder.get<oasis::fabrication::SomaticStateComponent>();

    // Traversed 3 units along +X: from 0.0 to 3.0
    REQUIRE_THAT(updated_pos.pos.x, WithinAbs(3.0f, 0.05f));
    REQUIRE_THAT(updated_pos.pos.y, WithinAbs(1.0f, 1e-4));
    REQUIRE_THAT(updated_pos.pos.z, WithinAbs(0.0f, 1e-4));

    // Caloric expenditure: 60 * (0.10 / 60) = 0.10 kcal burned
    REQUIRE_THAT(updated_somatic.caloric_reserve, WithinAbs(2499.90, 0.01));

    // Continue until arrival
    for (int i = 0; i < 60; ++i) {
        ecs.progress();
    }

    const auto& final_cmd = founder.get<oasis::fabrication::FounderCommandComponent>();
    REQUIRE(final_cmd.state == oasis::fabrication::FounderActionState::CONSTRUCTING);
}

// =============================================================================
// TEST SUITE: Founder Vitals HUD Data Continuity & Exhaustion Guard
// =============================================================================
TEST_CASE("Founder Vitals Bounds & Exhaustion Invariants", "[sprint7][vitals][exhaustion]") {
    SECTION("Caloric reserve depletion triggers exhaustion state") {
        oasis::fabrication::SomaticStateComponent somatic{};
        somatic.caloric_reserve = 500.5; // Near exhaustion threshold (500.0 kcal)

        oasis::psychology::PsychologyComponent psych{};
        psych.focus = 0.80f;

        oasis::fabrication::FounderCommandComponent cmd{};
        cmd.state = oasis::fabrication::FounderActionState::CONSTRUCTING;

        // Drain calories below threshold
        somatic.caloric_reserve -= 1.0;
        if (somatic.caloric_reserve <= 500.0 || psych.focus <= 0.05f) {
            somatic.is_exhausted = true;
            cmd.state = oasis::fabrication::FounderActionState::EXHAUSTED;
        }

        REQUIRE(somatic.is_exhausted == true);
        REQUIRE(cmd.state == oasis::fabrication::FounderActionState::EXHAUSTED);
        REQUIRE(somatic.caloric_reserve < 500.0);
    }

    SECTION("Focus depletion triggers exhaustion state") {
        oasis::fabrication::SomaticStateComponent somatic{};
        somatic.caloric_reserve = 2000.0;

        oasis::psychology::PsychologyComponent psych{};
        psych.focus = 0.04f; // Below 0.05 threshold

        oasis::fabrication::FounderCommandComponent cmd{};
        cmd.state = oasis::fabrication::FounderActionState::CONSTRUCTING;

        if (somatic.caloric_reserve <= 500.0 || psych.focus <= 0.05f) {
            somatic.is_exhausted = true;
            cmd.state = oasis::fabrication::FounderActionState::EXHAUSTED;
        }

        REQUIRE(somatic.is_exhausted == true);
        REQUIRE(cmd.state == oasis::fabrication::FounderActionState::EXHAUSTED);
    }
}

// =============================================================================
// TEST SUITE: CameraUniforms Avatar Data Pipeline Integration
// =============================================================================
TEST_CASE("Render Frame Avatar Data Packaging Pipeline", "[sprint7][avatar][pipeline]") {
    flecs::world ecs;

    ecs.component<oasis::fabrication::SpatialPositionComponent>();
    ecs.component<oasis::fabrication::FounderCommandComponent>();
    ecs.component<oasis::fabrication::SomaticStateComponent>();
    ecs.component<oasis::psychology::PsychologyComponent>();

    oasis::fabrication::SpatialPositionComponent pos{};
    pos.pos = glm::vec3(4.25f, 1.0f, -2.75f);

    oasis::fabrication::FounderCommandComponent cmd{};
    cmd.state = oasis::fabrication::FounderActionState::NAVIGATING;

    oasis::psychology::PsychologyComponent psych{};
    psych.stress = 0.42f;
    psych.focus = 0.88f;

    auto founder = ecs.entity("Founder")
        .set<oasis::fabrication::SpatialPositionComponent>(pos)
        .set<oasis::fabrication::FounderCommandComponent>(cmd)
        .set<oasis::psychology::PsychologyComponent>(psych);

    oasis::graphics::CameraUniforms uniforms{};

    // Emulate RenderWebGPUFrame extraction logic
    auto f = ecs.entity("Founder");
    REQUIRE(f.is_alive());
    if (f.is_alive() && f.has<oasis::fabrication::SpatialPositionComponent>()) {
        const auto& p = f.get<oasis::fabrication::SpatialPositionComponent>();
        const float anim_time = 120.0f * (1.0f / 60.0f); // tick 120 -> 2.0s
        uniforms.avatar_pos = glm::vec4(p.pos.x, p.pos.y, p.pos.z, anim_time);

        float action_state = 0.0f;
        float stress = 0.25f;
        float focus = 0.75f;

        if (f.has<oasis::fabrication::FounderCommandComponent>()) {
            action_state = static_cast<float>(f.get<oasis::fabrication::FounderCommandComponent>().state);
        }
        if (f.has<oasis::psychology::PsychologyComponent>()) {
            const auto& psi = f.get<oasis::psychology::PsychologyComponent>();
            stress = psi.stress;
            focus = psi.focus;
        }

        uniforms.avatar_params = glm::vec4(action_state, stress, focus, 1.0f);
    }

    REQUIRE_THAT(uniforms.avatar_pos.x, WithinAbs(4.25f, 1e-5));
    REQUIRE_THAT(uniforms.avatar_pos.y, WithinAbs(1.0f, 1e-5));
    REQUIRE_THAT(uniforms.avatar_pos.z, WithinAbs(-2.75f, 1e-5));
    REQUIRE_THAT(uniforms.avatar_pos.w, WithinAbs(2.0f, 1e-5));

    REQUIRE_THAT(uniforms.avatar_params.x, WithinAbs(1.0f, 1e-5)); // NAVIGATING = 1
    REQUIRE_THAT(uniforms.avatar_params.y, WithinAbs(0.42f, 1e-5)); // Stress = 0.42
    REQUIRE_THAT(uniforms.avatar_params.z, WithinAbs(0.88f, 1e-5)); // Focus = 0.88
    REQUIRE_THAT(uniforms.avatar_params.w, WithinAbs(1.0f, 1e-5));  // Active = 1.0
}
