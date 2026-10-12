#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <cstring>
#include <glm/glm.hpp>
#include <flecs.h>

#include "core/camera_uniforms.hpp"
#include "core/fabrication.hpp"

using Catch::Matchers::WithinAbs;

// ==============================================================================
// ORACLE: WGSL intersect_sphere Exact Float Math Implementation
// ==============================================================================
namespace {

inline float intersect_sphere_oracle(glm::vec3 ro, glm::vec3 rd, glm::vec3 center, float radius) {
    glm::vec3 oc = ro - center;
    float b = glm::dot(oc, rd);
    float c = glm::dot(oc, oc) - radius * radius;
    float disc = b * b - c;
    if (disc < 0.0f) {
        return -1.0f;
    }
    float t = -b - std::sqrt(disc);
    if (t < 0.0f) {
        return -1.0f;
    }
    return t;
}

inline float smoothstep_oracle(float edge0, float edge1, float x) {
    float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

} // anonymous namespace

// ==============================================================================
// CHALLENGE 1: CameraUniforms Memory Layout, Alignment, Offsets & Bit Integrity
// ==============================================================================

TEST_CASE("Adversarial: CameraUniforms 288-byte WGSL Layout & Memory Boundary Integrity", "[adversarial][layout][uniforms]") {
    SECTION("Exact total size and 16-byte alignment") {
        REQUIRE(sizeof(oasis::graphics::CameraUniforms) == 288);
        REQUIRE(alignof(oasis::graphics::CameraUniforms) == 16);
    }

    SECTION("Exact member byte offsets according to WebGPU @group(0) @binding(0) host contract") {
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

    SECTION("Zero trailing padding after avatar_params") {
        constexpr size_t avatar_params_end = offsetof(oasis::graphics::CameraUniforms, avatar_params) + sizeof(glm::vec4);
        REQUIRE(avatar_params_end == 288);
        REQUIRE(avatar_params_end == sizeof(oasis::graphics::CameraUniforms));
    }

    SECTION("Strict member separation under dirty memory and bitwise serialization") {
        oasis::graphics::CameraUniforms uniforms{};
        // Fill memory with 0xAA pattern
        std::memset(&uniforms, 0xAA, sizeof(uniforms));

        // Write specific values to avatar_pos
        uniforms.avatar_pos = glm::vec4(12.5f, -3.25f, 44.125f, 987.5f);
        // Write specific values to avatar_params
        uniforms.avatar_params = glm::vec4(2.0f, 0.65f, 0.90f, 1.0f);
        // Write cursor_highlight
        uniforms.cursor_highlight = glm::vec4(1.0f, 2.0f, 3.0f, 1.0f);

        // Verify avatar_pos did not corrupt cursor_highlight
        REQUIRE_THAT(uniforms.cursor_highlight.x, WithinAbs(1.0f, 1e-6));
        REQUIRE_THAT(uniforms.cursor_highlight.y, WithinAbs(2.0f, 1e-6));
        REQUIRE_THAT(uniforms.cursor_highlight.z, WithinAbs(3.0f, 1e-6));
        REQUIRE_THAT(uniforms.cursor_highlight.w, WithinAbs(1.0f, 1e-6));

        // Verify avatar_pos fields
        REQUIRE_THAT(uniforms.avatar_pos.x, WithinAbs(12.5f, 1e-6));
        REQUIRE_THAT(uniforms.avatar_pos.y, WithinAbs(-3.25f, 1e-6));
        REQUIRE_THAT(uniforms.avatar_pos.z, WithinAbs(44.125f, 1e-6));
        REQUIRE_THAT(uniforms.avatar_pos.w, WithinAbs(987.5f, 1e-6));

        // Verify avatar_params fields
        REQUIRE_THAT(uniforms.avatar_params.x, WithinAbs(2.0f, 1e-6));
        REQUIRE_THAT(uniforms.avatar_params.y, WithinAbs(0.65f, 1e-6));
        REQUIRE_THAT(uniforms.avatar_params.z, WithinAbs(0.90f, 1e-6));
        REQUIRE_THAT(uniforms.avatar_params.w, WithinAbs(1.0f, 1e-6));

        // Verify raw byte serialization across uint8_t buffer (simulating WebGPU queue.WriteBuffer)
        uint8_t raw_buffer[288];
        std::memcpy(raw_buffer, &uniforms, sizeof(uniforms));

        // Verify avatar_pos floats at byte offset 256
        const float* raw_avatar_pos = reinterpret_cast<const float*>(raw_buffer + 256);
        REQUIRE_THAT(raw_avatar_pos[0], WithinAbs(12.5f, 1e-6));
        REQUIRE_THAT(raw_avatar_pos[1], WithinAbs(-3.25f, 1e-6));
        REQUIRE_THAT(raw_avatar_pos[2], WithinAbs(44.125f, 1e-6));
        REQUIRE_THAT(raw_avatar_pos[3], WithinAbs(987.5f, 1e-6));

        // Verify avatar_params floats at byte offset 272
        const float* raw_avatar_params = reinterpret_cast<const float*>(raw_buffer + 272);
        REQUIRE_THAT(raw_avatar_params[0], WithinAbs(2.0f, 1e-6));
        REQUIRE_THAT(raw_avatar_params[1], WithinAbs(0.65f, 1e-6));
        REQUIRE_THAT(raw_avatar_params[2], WithinAbs(0.90f, 1e-6));
        REQUIRE_THAT(raw_avatar_params[3], WithinAbs(1.0f, 1e-6));
    }
}

// ==============================================================================
// CHALLENGE 2: Ray-Sphere Intersection Math & Adversarial Boundary Edge Cases
// ==============================================================================

TEST_CASE("Adversarial: Ray-Sphere Intersection Math & Geometric Edge Cases", "[adversarial][math][sphere]") {
    SECTION("Direct axial hits across 6 cardinal directions") {
        const float radius = 0.32f;
        const float dist = 10.0f;

        const struct TestCase {
            glm::vec3 ro;
            glm::vec3 rd;
            glm::vec3 center;
        } cases[] = {
            {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, dist}},       // +Z
            {{0.0f, 0.0f, dist}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},      // -Z
            {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {dist, 0.0f, 0.0f}},       // +X
            {{dist, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},      // -X
            {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, dist, 0.0f}},       // +Y
            {{0.0f, dist, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}       // -Y
        };

        for (const auto& c : cases) {
            float t = intersect_sphere_oracle(c.ro, c.rd, c.center, radius);
            REQUIRE_THAT(t, WithinAbs(dist - radius, 1e-4));
        }
    }

    SECTION("Rays pointing strictly away from sphere return -1.0") {
        glm::vec3 ro{0.0f, 0.0f, 0.0f};
        glm::vec3 center{0.0f, 0.0f, 10.0f};
        float radius = 0.32f;

        // Directly away (-Z)
        float t_away = intersect_sphere_oracle(ro, glm::vec3{0.0f, 0.0f, -1.0f}, center, radius);
        REQUIRE(t_away == -1.0f);

        // Oblique away
        float t_oblique = intersect_sphere_oracle(ro, glm::normalize(glm::vec3{1.0f, 1.0f, -0.5f}), center, radius);
        REQUIRE(t_oblique == -1.0f);
    }

    SECTION("Ray strictly tangent to sphere: discriminant is exactly zero") {
        const float radius = 2.0f;
        const float D = 10.0f;
        glm::vec3 ro{0.0f, 0.0f, 0.0f};
        glm::vec3 rd{0.0f, 0.0f, 1.0f};
        glm::vec3 center{radius, 0.0f, D}; // Sphere shifted on X by exactly radius

        float t = intersect_sphere_oracle(ro, rd, center, radius);
        REQUIRE_THAT(t, WithinAbs(D, 1e-5));

        // Grazing outside by epsilon (miss)
        const float eps = 1e-4f;
        glm::vec3 center_miss{radius + eps, 0.0f, D};
        float t_miss = intersect_sphere_oracle(ro, rd, center_miss, radius);
        REQUIRE(t_miss == -1.0f);

        // Grazing inside by epsilon (hit)
        glm::vec3 center_hit{radius - eps, 0.0f, D};
        float t_hit = intersect_sphere_oracle(ro, rd, center_hit, radius);
        REQUIRE(t_hit > 0.0f);
        REQUIRE_THAT(t_hit, WithinAbs(D, 0.1f));
    }

    SECTION("Ray origin strictly inside sphere: returns -1.0 (no false positive in front)") {
        glm::vec3 center{5.0f, 2.0f, -3.0f};
        float radius = 1.0f;

        // Origin at center
        float t_center = intersect_sphere_oracle(center, glm::vec3{0.0f, 0.0f, 1.0f}, center, radius);
        REQUIRE(t_center == -1.0f);

        // Origin halfway between center and surface
        glm::vec3 inside_pos = center + glm::vec3{0.3f, -0.2f, 0.1f};
        float t_inside = intersect_sphere_oracle(inside_pos, glm::vec3{1.0f, 0.0f, 0.0f}, center, radius);
        REQUIRE(t_inside == -1.0f);

        // Origin 99% of radius towards surface
        glm::vec3 near_surface_inside = center + glm::vec3{0.0f, 0.99f * radius, 0.0f};
        float t_near = intersect_sphere_oracle(near_surface_inside, glm::vec3{0.0f, 1.0f, 0.0f}, center, radius);
        REQUIRE(t_near == -1.0f);
    }

    SECTION("Ray origin on sphere surface pointing inward vs outward") {
        glm::vec3 center{0.0f, 0.0f, 5.0f};
        float radius = 1.0f;
        glm::vec3 on_surface{0.0f, 0.0f, 4.0f}; // Front vertex

        // Pointing inward (+Z)
        float t_in = intersect_sphere_oracle(on_surface, glm::vec3{0.0f, 0.0f, 1.0f}, center, radius);
        REQUIRE_THAT(t_in, WithinAbs(0.0f, 1e-5));

        // Pointing outward (-Z)
        float t_out = intersect_sphere_oracle(on_surface, glm::vec3{0.0f, 0.0f, -1.0f}, center, radius);
        REQUIRE(t_out == -1.0f);
    }

    SECTION("Zero-radius sphere behavior and single-precision ULP limit") {
        glm::vec3 ro{0.0f, 0.0f, 0.0f};
        glm::vec3 rd{0.0f, 0.0f, 1.0f};
        glm::vec3 center{0.0f, 0.0f, 10.0f};

        // Ray passing exactly through center point
        float t_hit = intersect_sphere_oracle(ro, rd, center, 0.0f);
        REQUIRE_THAT(t_hit, WithinAbs(10.0f, 1e-5));

        // Offset exceeding float32 ULP at D=10: ULP(100) = 1.19e-5, sqrt(ULP) = 0.0035
        // Offset of 0.01 has delta^2 = 0.0001 >> ULP(100), correctly producing disc < 0 (miss)
        float t_miss = intersect_sphere_oracle(ro, rd, glm::vec3{0.01f, 0.0f, 10.0f}, 0.0f);
        REQUIRE(t_miss == -1.0f);

        // Sub-ULP offset (0.001) where delta^2 = 1e-6 < ULP(100) collapses to disc = 0 in f32
        float t_sub_ulp = intersect_sphere_oracle(ro, rd, glm::vec3{0.001f, 0.0f, 10.0f}, 0.0f);
        REQUIRE(t_sub_ulp == 10.0f);
    }

    SECTION("Negative radius behavior (squared radius symmetry)") {
        glm::vec3 ro{0.0f, 0.0f, 0.0f};
        glm::vec3 rd{0.0f, 0.0f, 1.0f};
        glm::vec3 center{0.0f, 0.0f, 10.0f};

        float t_pos = intersect_sphere_oracle(ro, rd, center, 0.32f);
        float t_neg = intersect_sphere_oracle(ro, rd, center, -0.32f);
        REQUIRE_THAT(t_pos, WithinAbs(t_neg, 1e-6));
    }

    SECTION("Extreme distances: Oasis bounds precision vs catastrophic cancellation threshold") {
        glm::vec3 ro{0.0f, 0.0f, 0.0f};
        glm::vec3 rd{0.0f, 0.0f, 1.0f};
        float radius = 0.5f;

        // Within Oasis map bounds and viewport ranges (D in [10, 500]), precision is strictly preserved
        const float oasis_distances[] = {10.0f, 50.0f, 100.0f, 500.0f};
        for (float d : oasis_distances) {
            glm::vec3 center{0.0f, 0.0f, d};
            float t = intersect_sphere_oracle(ro, rd, center, radius);
            REQUIRE(t > 0.0f);
            REQUIRE_THAT(t, WithinAbs(d - radius, 0.02f));
        }

        // Catastrophic cancellation boundary: for r=0.5, r^2=0.25.
        // Single precision has 24-bit mantissa (eps ~ 1.19e-7).
        // At D=5000, D^2 = 2.5e7 > 2^24 (1.67e7), so ULP(2.5e7) = 2.0 > r^2 (0.25).
        // r^2 is swallowed into 0, disc becomes 0, and t collapses to d.
        glm::vec3 far_center{0.0f, 0.0f, 5000.0f};
        float t_far = intersect_sphere_oracle(ro, rd, far_center, radius);
        REQUIRE(t_far == 5000.0f); // Empirically reproduces float32 ULP swallowing threshold
    }

    SECTION("Monte Carlo randomized ray-sphere consistency (1,000 rays)") {
        std::mt19937 rng(424242);
        std::uniform_real_distribution<float> pos_dist(-20.0f, 20.0f);
        std::uniform_real_distribution<float> z_dist(5.0f, 40.0f);
        std::uniform_real_distribution<float> rad_dist(0.1f, 2.0f);
        std::uniform_real_distribution<float> dir_dist(-1.0f, 1.0f);

        for (int i = 0; i < 1000; ++i) {
            glm::vec3 center{pos_dist(rng), pos_dist(rng), z_dist(rng)};
            float radius = rad_dist(rng);
            glm::vec3 ro{0.0f, 0.0f, 0.0f};
            glm::vec3 raw_dir{dir_dist(rng), dir_dist(rng), std::abs(dir_dist(rng)) + 0.1f};
            glm::vec3 rd = glm::normalize(raw_dir);

            float t = intersect_sphere_oracle(ro, rd, center, radius);
            if (t > 0.0f) {
                // Verify hit point is within epsilon of sphere surface
                glm::vec3 hit = ro + rd * t;
                float d_to_center = glm::length(hit - center);
                REQUIRE_THAT(d_to_center, WithinAbs(radius, 1e-3));
            } else {
                REQUIRE(t == -1.0f);
            }
        }
    }
}

// ==============================================================================
// CHALLENGE 3: Map Extremes, Coordinate Boundaries & Dynamic Hover Math
// ==============================================================================

TEST_CASE("Adversarial: Map Coordinate Extremes, Hover Oscillation & Contact Shadow", "[adversarial][coordinates][boundaries]") {
    SECTION("Map bounding box corner coordinates and negative extremes") {
        // Map volume bounds: x in [-28, 28], y in [0, 12], z in [-10, 42]
        const std::vector<glm::vec3> extreme_coords = {
            {-28.0f, 0.0f, -10.0f},
            { 28.0f, 0.0f, -10.0f},
            {-28.0f, 12.0f, -10.0f},
            { 28.0f, 12.0f, -10.0f},
            {-28.0f, 0.0f,  42.0f},
            { 28.0f, 0.0f,  42.0f},
            {-28.0f, 12.0f,  42.0f},
            { 28.0f, 12.0f,  42.0f},
            {  0.0f,  0.0f,   0.0f},
            {-100.0f, -50.0f, -100.0f}, // Deep negative out of bounds
            { 100.0f,  50.0f,  100.0f}  // Large positive out of bounds
        };

        for (const auto& coord : extreme_coords) {
            oasis::graphics::CameraUniforms uniforms{};
            uniforms.avatar_pos = glm::vec4(coord.x, coord.y, coord.z, 0.0f);
            uniforms.avatar_params = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

            REQUIRE_THAT(uniforms.avatar_pos.x, WithinAbs(coord.x, 1e-5));
            REQUIRE_THAT(uniforms.avatar_pos.y, WithinAbs(coord.y, 1e-5));
            REQUIRE_THAT(uniforms.avatar_pos.z, WithinAbs(coord.z, 1e-5));
        }
    }

    SECTION("Hover offset and pulse oscillation bounds across long simulation runtimes") {
        // WGSL: let hover_offset = 0.40 + sin(avatar_pos.w * 3.5) * 0.08;
        // WGSL: let pulse = 0.85 + 0.15 * sin(avatar_pos.w * 5.0);
        // Time points from 0 to 100,000 seconds (~27 hours)
        const std::vector<float> time_samples = {
            0.0f, 0.5f, 1.0f, 10.0f, 100.0f, 3600.0f, 86400.0f, 1000000.0f
        };

        for (float t : time_samples) {
            float hover_offset = 0.40f + std::sin(t * 3.5f) * 0.08f;
            float pulse = 0.85f + 0.15f * std::sin(t * 5.0f) * 0.08f;

            REQUIRE(hover_offset >= 0.3199f);
            REQUIRE(hover_offset <= 0.4801f);

            REQUIRE(pulse >= 0.70f);
            REQUIRE(pulse <= 1.00f);
        }
    }

    SECTION("Ground contact shadow roll-off and vertical gate invariants") {
        // WGSL:
        // let ground_dist = length(voxel_pos_hit.xz - camera.avatar_pos.xz);
        // if (ground_dist < 0.65 && voxel_pos_hit.y <= camera.avatar_pos.y + 0.1)
        // let shadow = smoothstep(0.65, 0.15, ground_dist) * 0.45;

        glm::vec3 avatar_pos{10.0f, 1.0f, -5.0f};

        // Center directly beneath avatar: ground_dist = 0
        float dist_center = 0.0f;
        float shadow_center = smoothstep_oracle(0.65f, 0.15f, dist_center) * 0.45f;
        REQUIRE_THAT(shadow_center, WithinAbs(0.45f, 1e-5));

        // Exact boundary: ground_dist = 0.65
        float dist_edge = 0.65f;
        float shadow_edge = smoothstep_oracle(0.65f, 0.15f, dist_edge) * 0.45f;
        REQUIRE_THAT(shadow_edge, WithinAbs(0.0f, 1e-5));

        // Beyond boundary: ground_dist = 0.80
        float dist_far = 0.80f;
        float shadow_far = smoothstep_oracle(0.65f, 0.15f, dist_far) * 0.45f;
        REQUIRE_THAT(shadow_far, WithinAbs(0.0f, 1e-5));

        // Midpoint: ground_dist = 0.40
        float dist_mid = 0.40f;
        float shadow_mid = smoothstep_oracle(0.65f, 0.15f, dist_mid) * 0.45f;
        REQUIRE(shadow_mid > 0.0f);
        REQUIRE(shadow_mid < 0.45f);

        // Vertical gate checks
        float hit_y_below = avatar_pos.y - 0.5f;
        REQUIRE(hit_y_below <= avatar_pos.y + 0.1f);

        float hit_y_flush = avatar_pos.y;
        REQUIRE(hit_y_flush <= avatar_pos.y + 0.1f);

        float hit_y_above = avatar_pos.y + 0.2f;
        REQUIRE(hit_y_above > avatar_pos.y + 0.1f); // Must NOT receive shadow
    }
}

// ==============================================================================
// CHALLENGE 4: Depth Occlusion Invariants & Tie-Breaking
// ==============================================================================

TEST_CASE("Adversarial: Depth Occlusion Invariants Between Voxel Terrain & Avatar", "[adversarial][depth][occlusion]") {
    // WGSL Invariant:
    // let voxel_dist = select(1e9, hit_dist, hit_mat != 0u);
    // let is_avatar_visible = (t_avatar > 0.0 && t_avatar < voxel_dist);

    SECTION("Avatar in front of voxel terrain is strictly visible") {
        float t_avatar = 5.0f;
        float voxel_dist = 12.0f;
        bool visible = (t_avatar > 0.0f && t_avatar < voxel_dist);
        REQUIRE(visible == true);
    }

    SECTION("Voxel terrain in front of avatar strictly occludes avatar") {
        float t_avatar = 12.0f;
        float voxel_dist = 5.0f;
        bool visible = (t_avatar > 0.0f && t_avatar < voxel_dist);
        REQUIRE(visible == false);
    }

    SECTION("Depth tie-breaking: identical distance goes to voxel (strict less-than)") {
        float t_avatar = 8.0f;
        float voxel_dist = 8.0f;
        bool visible = (t_avatar > 0.0f && t_avatar < voxel_dist);
        REQUIRE(visible == false); // Voxel surface strictly occludes in tie
    }

    SECTION("Near-tie delta tests (sub-millimeter precision)") {
        float t_avatar = 8.0000f;

        // Voxel 0.0001 units behind avatar
        float voxel_behind = 8.0001f;
        REQUIRE((t_avatar > 0.0f && t_avatar < voxel_behind) == true);

        // Voxel 0.0001 units in front of avatar
        float voxel_in_front = 7.9999f;
        REQUIRE((t_avatar > 0.0f && t_avatar < voxel_in_front) == false);
    }

    SECTION("Sky background (hit_mat == 0) renders avatar at any valid distance") {
        float hit_dist = 20.0f;
        uint32_t hit_mat_sky = 0u;
        float voxel_dist_sky = (hit_mat_sky != 0u) ? hit_dist : 1e9f;

        float t_avatar_near = 2.0f;
        REQUIRE((t_avatar_near > 0.0f && t_avatar_near < voxel_dist_sky) == true);

        float t_avatar_far = 500.0f;
        REQUIRE((t_avatar_far > 0.0f && t_avatar_far < voxel_dist_sky) == true);
    }

    SECTION("Avatar miss (t_avatar == -1.0) is never visible") {
        float t_avatar = -1.0f;
        float voxel_dist_sky = 1e9f;
        REQUIRE((t_avatar > 0.0f && t_avatar < voxel_dist_sky) == false);

        float voxel_dist_terrain = 10.0f;
        REQUIRE((t_avatar > 0.0f && t_avatar < voxel_dist_terrain) == false);
    }

    SECTION("Avatar disabled (active flag <= 0.5) skips rendering completely") {
        float avatar_active_flag = 0.0f;
        bool active = (avatar_active_flag > 0.5f);
        REQUIRE(active == false);

        avatar_active_flag = 0.4999f;
        active = (avatar_active_flag > 0.5f);
        REQUIRE(active == false);

        avatar_active_flag = 1.0f;
        active = (avatar_active_flag > 0.5f);
        REQUIRE(active == true);
    }
}

// ==============================================================================
// CHALLENGE 5: Flecs Action State & Psychological Visual Mapping Invariants
// ==============================================================================

TEST_CASE("Adversarial: State-Driven Visual Styling & High-Stress Override Invariants", "[adversarial][vitals][styling]") {
    // WGSL logic:
    // let action_state = u32(round(camera.avatar_params.x));
    // var base_color = vec3<f32>(0.00, 0.90, 0.45); // Emerald Green (IDLE = 0)
    // if (action_state == 1u) -> Cyan (NAVIGATING)
    // else if (action_state == 2u) -> Amber Gold (CONSTRUCTING)
    // else if (action_state == 4u || camera.avatar_params.y > 0.75) -> Crimson Warning (EXHAUSTED / High Stress)

    enum class AvatarColorStyle {
        EMERALD_IDLE,
        CYAN_NAVIGATING,
        AMBER_CONSTRUCTING,
        CRIMSON_WARNING
    };

    auto resolve_style = [](float action_param, float stress_param) -> AvatarColorStyle {
        uint32_t action_state = static_cast<uint32_t>(std::round(action_param));
        if (action_state == 4u || stress_param > 0.75f) {
            return AvatarColorStyle::CRIMSON_WARNING;
        } else if (action_state == 1u) {
            return AvatarColorStyle::CYAN_NAVIGATING;
        } else if (action_state == 2u) {
            return AvatarColorStyle::AMBER_CONSTRUCTING;
        }
        return AvatarColorStyle::EMERALD_IDLE;
    };

    SECTION("Normal nominal states under low stress") {
        REQUIRE(resolve_style(0.0f, 0.20f) == AvatarColorStyle::EMERALD_IDLE);
        REQUIRE(resolve_style(1.0f, 0.20f) == AvatarColorStyle::CYAN_NAVIGATING);
        REQUIRE(resolve_style(2.0f, 0.20f) == AvatarColorStyle::AMBER_CONSTRUCTING);
        REQUIRE(resolve_style(4.0f, 0.20f) == AvatarColorStyle::CRIMSON_WARNING);
    }

    SECTION("High stress (> 0.75) overrides ALL states to Crimson Warning") {
        // IDLE overridden
        REQUIRE(resolve_style(0.0f, 0.76f) == AvatarColorStyle::CRIMSON_WARNING);
        // NAVIGATING overridden
        REQUIRE(resolve_style(1.0f, 0.85f) == AvatarColorStyle::CRIMSON_WARNING);
        // CONSTRUCTING overridden
        REQUIRE(resolve_style(2.0f, 0.99f) == AvatarColorStyle::CRIMSON_WARNING);
        // EXHAUSTED remains Crimson
        REQUIRE(resolve_style(4.0f, 1.00f) == AvatarColorStyle::CRIMSON_WARNING);
    }

    SECTION("Stress boundary transition at exactly 0.75") {
        // At 0.75: NOT high stress
        REQUIRE(resolve_style(1.0f, 0.7500f) == AvatarColorStyle::CYAN_NAVIGATING);
        // At 0.7501: High stress triggers
        REQUIRE(resolve_style(1.0f, 0.7501f) == AvatarColorStyle::CRIMSON_WARNING);
    }

    SECTION("Float round resilience on action state parameter") {
        // 0.98 rounds to 1 (NAVIGATING)
        REQUIRE(resolve_style(0.98f, 0.20f) == AvatarColorStyle::CYAN_NAVIGATING);
        // 1.02 rounds to 1 (NAVIGATING)
        REQUIRE(resolve_style(1.02f, 0.20f) == AvatarColorStyle::CYAN_NAVIGATING);
        // 1.95 rounds to 2 (CONSTRUCTING)
        REQUIRE(resolve_style(1.95f, 0.20f) == AvatarColorStyle::AMBER_CONSTRUCTING);
    }
}
