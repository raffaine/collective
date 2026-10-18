#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "core/raycaster.hpp"

namespace oasis::graphics {

// Pitch clamp bounds: [5 degrees, 85 degrees] in radians to prevent gimbal lock at 0 deg / 90 deg
constexpr float ARCBALL_MIN_PHI_RAD = glm::radians(5.0f);   // ~0.08726646f
constexpr float ARCBALL_MAX_PHI_RAD = glm::radians(85.0f);  // ~1.48352986f

// Radial zoom distance bounds (meters / voxels)
constexpr float ARCBALL_MIN_RADIUS = 4.0f;
constexpr float ARCBALL_MAX_RADIUS = 120.0f;

// Roof cutaway bounds
constexpr int ARCBALL_MIN_CUTAWAY_Z = 1;
constexpr int ARCBALL_MAX_CUTAWAY_Z = 32;
constexpr int ARCBALL_INTERIOR_CUTAWAY_Z = 3;

struct ArcballCameraState {
    glm::vec3 poi_origin{0.0f, 0.0f, 0.0f};
    glm::vec3 target_poi{0.0f, 0.0f, 0.0f};
    bool has_poi_lock{false};

    // Spherical coordinates around POI
    float theta_rad{0.0f};
    float target_theta_rad{0.0f};
    float phi_rad{0.65f};
    float target_phi_rad{0.65f};
    float radius{14.0f};
    float target_radius{14.0f};

    // Roof cutaway level and manual override state
    int z_cutaway_level{ARCBALL_MAX_CUTAWAY_Z};
    bool manual_cutaway_override{false};
};

// Strict Ray-Miss Guard: if !hit.hit, returns false and leaves cam 100% unchanged.
// If hit.hit, locks target_poi to hit.voxel_pos, sets has_poi_lock = true, and
// auto-lowers z_cutaway_level = 3 when locking onto an interior/ground building voxel.
inline bool LockPoiFromRaycast(ArcballCameraState& cam, const RaycastHit& hit) noexcept {
    if (!hit.hit) {
        return false;
    }

    cam.target_poi = glm::vec3(hit.voxel_pos);
    cam.has_poi_lock = true;

    const int abs_x = std::abs(hit.voxel_pos.x);
    if (hit.voxel_pos.y <= 3 && abs_x >= 9 && abs_x <= 17) {
        cam.z_cutaway_level = ARCBALL_INTERIOR_CUTAWAY_Z;
    }

    return true;
}

// Updates target_theta_rad wrapped into [-pi, pi] via std::remainder and
// clamps target_phi_rad strictly to [5 deg, 85 deg] to prevent gimbal lock.
inline void OrbitArcball(ArcballCameraState& cam, float delta_x, float delta_y) noexcept {
    constexpr float kTwoPi = 2.0f * glm::pi<float>();
    cam.target_theta_rad = std::remainder(cam.target_theta_rad + delta_x, kTwoPi);
    cam.target_phi_rad = std::clamp(cam.target_phi_rad + delta_y, ARCBALL_MIN_PHI_RAD, ARCBALL_MAX_PHI_RAD);
}

// Updates target_radius clamped to [4.0, 120.0] and auto-adjusts roof cutaway
// unless manual_cutaway_override is active.
inline void ZoomArcball(ArcballCameraState& cam, float wheel_delta) noexcept {
    cam.target_radius = std::clamp(cam.target_radius - wheel_delta * 1.2f, ARCBALL_MIN_RADIUS, ARCBALL_MAX_RADIUS);
    if (!cam.manual_cutaway_override) {
        cam.z_cutaway_level = (cam.target_radius < 12.0f) ? ARCBALL_INTERIOR_CUTAWAY_Z : ARCBALL_MAX_CUTAWAY_Z;
    }
}

// Engages manual cutaway override and clamps z_cutaway_level to [1, 32].
inline void AdjustManualCutaway(ArcballCameraState& cam, int delta_z) noexcept {
    cam.manual_cutaway_override = true;
    cam.z_cutaway_level = std::clamp(cam.z_cutaway_level + delta_z, ARCBALL_MIN_CUTAWAY_Z, ARCBALL_MAX_CUTAWAY_Z);
}

// Exponential damping (alpha = 1 - exp(-12 * dt_render)) with shortest-arc angular interpolation.
inline void StepArcballDamping(ArcballCameraState& cam, float dt_render) noexcept {
    if (dt_render <= 0.0f) {
        return;
    }

    constexpr float kTwoPi = 2.0f * glm::pi<float>();
    const float alpha = std::clamp(1.0f - std::exp(-12.0f * dt_render), 0.0f, 1.0f);

    const float dtheta = std::remainder(cam.target_theta_rad - cam.theta_rad, kTwoPi);
    cam.theta_rad = std::remainder(cam.theta_rad + dtheta * alpha, kTwoPi);

    cam.phi_rad = std::clamp(
        cam.phi_rad + (cam.target_phi_rad - cam.phi_rad) * alpha,
        ARCBALL_MIN_PHI_RAD,
        ARCBALL_MAX_PHI_RAD
    );

    cam.radius = std::clamp(
        cam.radius + (cam.target_radius - cam.radius) * alpha,
        ARCBALL_MIN_RADIUS,
        ARCBALL_MAX_RADIUS
    );

    cam.poi_origin = cam.poi_origin + (cam.target_poi - cam.poi_origin) * alpha;
}

// Computes the 3D world-space camera eye position from spherical coordinates around poi_origin.
[[nodiscard]] inline glm::vec3 ComputeArcballEyePosition(const ArcballCameraState& cam) noexcept {
    const float cos_phi = std::cos(cam.phi_rad);
    const float sin_phi = std::sin(cam.phi_rad);
    const float sin_theta = std::sin(cam.theta_rad);
    const float cos_theta = std::cos(cam.theta_rad);

    return glm::vec3(
        cam.poi_origin.x + cam.radius * cos_phi * sin_theta,
        cam.poi_origin.y + cam.radius * sin_phi,
        cam.poi_origin.z - cam.radius * cos_phi * cos_theta
    );
}

// Translates target_poi (and poi_origin) along the camera's horizontal ground plane (XZ).
inline void PanArcballPoi(ArcballCameraState& cam, float right_step, float fwd_step) noexcept {
    const float sin_theta = std::sin(cam.theta_rad);
    const float cos_theta = std::cos(cam.theta_rad);
    const glm::vec3 fwd_ground(-sin_theta, 0.0f, cos_theta);
    const glm::vec3 right_ground = glm::cross(fwd_ground, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::vec3 delta = right_ground * right_step + fwd_ground * fwd_step;

    cam.target_poi.x = std::clamp(cam.target_poi.x + delta.x, -45.0f, 45.0f);
    cam.target_poi.z = std::clamp(cam.target_poi.z + delta.z, -45.0f, 50.0f);
}

} // namespace oasis::graphics
