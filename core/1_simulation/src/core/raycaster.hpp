#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>
#include <cmath>
#include <unordered_map>
#include <algorithm>

namespace oasis::graphics {

struct CameraPanState {
    glm::vec3 camera_offset{0.0f, 0.0f, 0.0f};
    uint32_t active_camera_mode{0}; // 0 = 3rd-Person Perspective, 1 = Cadastral Isometric
    uint32_t target_camera_mode{0};
    float transition_elapsed_sec{0.0f};
    bool is_transitioning{false};
};

inline glm::vec3 ComputeCameraPanDelta(
    uint32_t active_camera_mode,
    uint32_t target_camera_mode,
    float transition_elapsed_sec,
    bool is_transitioning,
    float delta_x,
    float delta_y,
    bool is_wheel
) noexcept {
    float t = 0.0f;
    if (is_transitioning) {
        float u = std::clamp(transition_elapsed_sec / 0.400f, 0.0f, 1.0f);
        float hermite_t = 3.0f * u * u - 2.0f * u * u * u;
        t = (target_camera_mode == 1) ? hermite_t : (1.0f - hermite_t);
    } else {
        t = (active_camera_mode == 1) ? 1.0f : 0.0f;
    }

    const glm::vec3 eye_persp(0.0001f, 2.0001f, -4.9999f);
    const glm::vec3 target_persp(0.0f, 0.0f, 0.0f);
    const glm::vec3 eye_iso(-16.0001f, 20.0001f, -16.0001f);
    const glm::vec3 target_iso(0.0001f, 0.0001f, 0.0001f);

    const glm::vec3 eye_cur = glm::mix(eye_persp, eye_iso, t);
    const glm::vec3 target_cur = glm::mix(target_persp, target_iso, t);
    const glm::vec3 look = target_cur - eye_cur;

    glm::vec3 fwd_ground(look.x, 0.0f, look.z);
    if (glm::length(fwd_ground) > 1e-4f) {
        fwd_ground = glm::normalize(fwd_ground);
    } else {
        fwd_ground = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    const glm::vec3 right_ground = glm::normalize(glm::cross(fwd_ground, glm::vec3(0.0f, 1.0f, 0.0f)));
    float scale = is_wheel ? glm::mix(0.025f, 0.040f, t) : glm::mix(0.020f, 0.045f, t);

    glm::vec3 pan_delta = (-delta_x * right_ground + delta_y * fwd_ground) * scale;
    pan_delta.y = 0.0f;
    return pan_delta;
}

inline void ApplyCameraPan(
    glm::vec3& offset,
    uint32_t active_camera_mode,
    uint32_t target_camera_mode,
    float transition_elapsed_sec,
    bool is_transitioning,
    float delta_x,
    float delta_y,
    bool is_wheel
) noexcept {
    glm::vec3 pan_delta = ComputeCameraPanDelta(
        active_camera_mode, target_camera_mode,
        transition_elapsed_sec, is_transitioning,
        delta_x, delta_y, is_wheel
    );
    offset += pan_delta;
    offset.y = 0.0f;
    offset.x = std::clamp(offset.x, -35.0f, 35.0f);
    offset.z = std::clamp(offset.z, -35.0f, 50.0f);
}

inline void ApplyCameraPan(CameraPanState& state, float delta_x, float delta_y, bool is_wheel) noexcept {
    ApplyCameraPan(state.camera_offset, state.active_camera_mode, state.target_camera_mode,
                   state.transition_elapsed_sec, state.is_transitioning, delta_x, delta_y, is_wheel);
}


struct RaycastHit {
    bool hit{false};
    glm::ivec3 voxel_pos{0};
    glm::ivec3 normal{0};
    glm::ivec3 placement_pos{0};
    uint32_t material_id{0};
    float distance{0.0f};
};

inline uint64_t PackVoxelCoord(int x, int y, int z) noexcept {
    const uint64_t ux = static_cast<uint64_t>(static_cast<uint32_t>(x));
    const uint64_t uy = static_cast<uint64_t>(static_cast<uint32_t>(y));
    const uint64_t uz = static_cast<uint64_t>(static_cast<uint32_t>(z));
    return (ux << 42) | ((uy & 0x1FFFFFULL) << 21) | (uz & 0x1FFFFFULL);
}

inline void UnpackVoxelCoord(uint64_t packed, int& x, int& y, int& z) noexcept {
    const int64_t sx = static_cast<int64_t>(packed) >> 42;
    x = static_cast<int>(sx);

    const uint32_t raw_y = static_cast<uint32_t>((packed >> 21) & 0x1FFFFFULL);
    y = (raw_y & (1U << 20)) ? static_cast<int>(raw_y | ~0x1FFFFFU) : static_cast<int>(raw_y);

    const uint32_t raw_z = static_cast<uint32_t>(packed & 0x1FFFFFULL);
    z = (raw_z & (1U << 20)) ? static_cast<int>(raw_z | ~0x1FFFFFU) : static_cast<int>(raw_z);
}

constexpr size_t MAX_GPU_DYNAMIC_VOXELS = 1024;

struct alignas(16) GpuDynamicVoxel {
    int32_t x{0};
    int32_t y{0};
    int32_t z{0};
    uint32_t material_id{0};
};
static_assert(sizeof(GpuDynamicVoxel) == 16, "GpuDynamicVoxel must be 16 bytes.");
static_assert(alignof(GpuDynamicVoxel) == 16, "GpuDynamicVoxel must be 16-byte aligned.");

struct alignas(16) GpuDynamicVoxelBuffer {
    uint32_t count{0};
    int32_t min_x{0};
    int32_t min_y{0};
    int32_t min_z{0};
    int32_t max_x{0};
    int32_t max_y{0};
    int32_t max_z{0};
    uint32_t pad{0};
    GpuDynamicVoxel voxels[MAX_GPU_DYNAMIC_VOXELS];
};
static_assert(sizeof(GpuDynamicVoxelBuffer) == 32 + MAX_GPU_DYNAMIC_VOXELS * 16, "GpuDynamicVoxelBuffer size mismatch.");
static_assert(alignof(GpuDynamicVoxelBuffer) == 16, "GpuDynamicVoxelBuffer must be 16-byte aligned.");


inline std::unordered_map<uint64_t, uint8_t>& GetDynamicVoxelStore() noexcept {
    static std::unordered_map<uint64_t, uint8_t> s_store;
    return s_store;
}

inline void SetDynamicVoxel(int x, int y, int z, uint8_t material_id) noexcept {
    const uint64_t key = PackVoxelCoord(x, y, z);
    if (material_id == 0u) {
        GetDynamicVoxelStore().erase(key);
    } else {
        GetDynamicVoxelStore()[key] = material_id;
    }
}

inline void SetDynamicVoxel(const glm::ivec3& pos, uint8_t material_id) noexcept {
    SetDynamicVoxel(pos.x, pos.y, pos.z, material_id);
}

inline uint8_t GetDynamicVoxel(int x, int y, int z) noexcept {
    auto& store = GetDynamicVoxelStore();
    auto it = store.find(PackVoxelCoord(x, y, z));
    if (it != store.end()) {
        return it->second;
    }
    return 0;
}

// Day 1 Survival Camp & Thermal Overlay Material IDs (STORY-9.1 / STORY-9.2)
constexpr uint32_t MAT_BELL_TENT = 23u;
constexpr uint32_t MAT_FOOD_STORAGE = 24u;
constexpr uint32_t MAT_LEAK_VOXEL_MARKER = 25u;
constexpr uint32_t MAT_SERVER_RACK = 30u;
constexpr uint32_t MAT_LADDER = 32u;
constexpr uint32_t MAT_STAGE_FRAME = 33u;
constexpr uint32_t MAT_MATURE_CROP = 34u;

inline uint32_t GetProceduralVoxelMaterial(const glm::ivec3& pos) noexcept {
    const int absX = std::abs(pos.x);
    const int lotZ = ((pos.z % 16) + 16) % 16;

    // Layer 0: Ground Plane & Road Infrastructure
    if (pos.y == 0) {
        if (absX <= 3) {
            if (pos.x == 0 && (pos.z % 4 >= 2 || pos.z % 4 <= -2)) {
                return 2u; // Yellow centerline
            }
            return 1u; // Asphalt
        }
        if (absX == 4 || absX == 5) {
            return 3u; // Sidewalk
        }
        if (absX >= 6 && absX <= 8 && lotZ >= 1 && lotZ <= 3) {
            return 3u; // Driveway
        }
        if (absX >= 6 && absX <= 64) {
            return 4u; // Parched lawn / interior floor / backyard loam
        }
    }

    // Layer 1-8: Electrical Grid Infrastructure (kept clear of Lot 402 foreground camera at z in [-2, 14])
    if (pos.y >= 1 && pos.y <= 8 && (pos.z < -2 || pos.z > 14)) {
        if (pos.x == 5 && lotZ == 0 && pos.y <= 7) {
            return 8u; // Utility timber pole
        }
        if (pos.x >= 4 && pos.x <= 6 && lotZ == 0 && pos.y == 7) {
            return 8u; // Crossarm
        }
        if (pos.x == 5 && lotZ == 1 && pos.y == 6) {
            return 9u; // Transformer
        }
        if (pos.x == 4 && pos.y == 7 && pos.z >= -10 && pos.z <= 40) {
            return 9u; // Overhead conductor wire
        }
    }

    // Layer 1-6: Hollow Suburban Residences (Lot 402 & neighbors)
    if (absX >= 9 && absX <= 17 && lotZ >= 2 && lotZ <= 10) {
        // Sloped gable roof (y in [4, 6])
        if (pos.y >= 4 && pos.y <= 6) {
            const int roof_inset = pos.y - 3;
            if (absX >= (9 + roof_inset) && absX <= (17 - roof_inset) &&
                lotZ >= (2 + roof_inset) && lotZ <= (10 - roof_inset)) {
                return 6u; // Dark shingle roof
            }
        }
        // Ground Floor Walls, Doors, Windows, Node 0 Server Rack, & Hollow Interior (y in [1, 3])
        if (pos.y >= 1 && pos.y <= 3) {
            // Front Door (absX == 9) & Back Patio Door (absX == 17) at lotZ == 6, y <= 2
            if ((absX == 9 || absX == 17) && lotZ == 6 && pos.y <= 2) {
                return 15u; // Insulated Timber Door (airtight & passable)
            }
            // Architectural Windows at y == 2
            if (pos.y == 2 &&
                (((absX == 9 || absX == 17) && (lotZ == 4 || lotZ == 8)) ||
                 ((lotZ == 2 || lotZ == 10) && (absX == 11 || absX == 15)))) {
                return 7u; // Window glass
            }
            // Exterior Perimeter Clapboard Walls
            if (absX == 9 || absX == 17 || lotZ == 2 || lotZ == 10) {
                return 5u; // Painted architectural clapboard siding
            }
            // Node 0 Sovereign Homelab Server Rack in Study (absX == 12, lotZ == 6, y <= 2)
            if (absX == 12 && lotZ == 6 && pos.y <= 2) {
                return 5u; // Rendered as Node 0 Server Rack in shader
            }
            // Hollow walkable interior returns 0u (Air)
        }
    }

    // Lot 402 Exterior Roof Access Ladder (x == 18, z == 3, y in [1, 4])
    if (pos.x == 18 && pos.z == 3 && pos.y >= 1 && pos.y <= 4) {
        return 32u; // MAT_LADDER: Exterior roof access ladder against back wall
    }

    // Layer 1: Property Boundary Fences & Lot 402 Fenced Backyard
    if (pos.y == 1) {
        if (pos.x > 0 && pos.z >= 1 && pos.z <= 15) {
            if ((pos.z == 1 || pos.z == 15) && pos.x >= 7 && pos.x <= 25) {
                return 10u; // Lot 402 side fences extending around backyard
            }
            if (pos.x == 25 && pos.z >= 1 && pos.z <= 15 && pos.z != 8) {
                return 10u; // Lot 402 rear backyard fence (with gate opening at z == 8)
            }
        } else if ((lotZ == 15 || lotZ == 1) && absX >= 7 && absX <= 20) {
            return 10u; // Neighbor boundary fences
        }
    }

    return 0u; // Air
}

inline uint32_t GetVoxelMaterialAt(const glm::ivec3& pos) noexcept {
    const uint8_t dyn = GetDynamicVoxel(pos.x, pos.y, pos.z);
    if (dyn != 0) {
        return dyn;
    }
    return GetProceduralVoxelMaterial(pos);
}

inline RaycastHit ScreenToWorldRaycast(
    int mouse_x, int mouse_y,
    uint32_t width, uint32_t height,
    const glm::mat4& inv_view_proj,
    const glm::vec4& camera_pos,
    const glm::vec4& viewport_params,
    int max_steps = 192,
    int z_cutaway_level = 32
) noexcept {
    RaycastHit result{};
    if (width == 0 || height == 0) return result;

    // 1. Screen space to WebGPU NDC coordinates
    const float ndc_x = (2.0f * static_cast<float>(mouse_x) / static_cast<float>(width)) - 1.0f;
    const float ndc_y = 1.0f - (2.0f * static_cast<float>(mouse_y) / static_cast<float>(height));

    // 2. Unproject near and far plane bounds
    const glm::vec4 near_h = inv_view_proj * glm::vec4(ndc_x, ndc_y, 0.0f, 1.0f);
    const glm::vec4 far_h  = inv_view_proj * glm::vec4(ndc_x, ndc_y, 1.0f, 1.0f);
    if (std::abs(near_h.w) < 1e-6f || std::abs(far_h.w) < 1e-6f) return result;

    const glm::vec3 p_near = glm::vec3(near_h) / near_h.w;
    const glm::vec3 p_far  = glm::vec3(far_h) / far_h.w;

    // 3. Perspective vs Cadastral Isometric Ray Blending
    const float t = viewport_params.w;
    const glm::vec3 ro_persp = glm::vec3(camera_pos);
    const glm::vec3 rd_persp = glm::normalize(p_far - ro_persp);
    const glm::vec3 ro_ortho = p_near;
    const glm::vec3 rd_ortho = glm::normalize(p_far - p_near);

    const glm::vec3 ro = glm::mix(ro_persp, ro_ortho, t);
    const glm::vec3 rd = glm::normalize(glm::mix(rd_persp, rd_ortho, t));

    // 4. Amanatides & Woo 3D DDA Traversal
    glm::ivec3 mapPos = glm::ivec3(glm::floor(ro));
    glm::ivec3 step(1);
    if (rd.x < 0.0f) step.x = -1;
    if (rd.y < 0.0f) step.y = -1;
    if (rd.z < 0.0f) step.z = -1;

    const glm::vec3 abs_rd = glm::max(glm::abs(rd), glm::vec3(1e-6f));
    const glm::vec3 deltaDist = glm::abs(glm::vec3(1.0f) / abs_rd);

    glm::vec3 sideDist(0.0f);
    sideDist.x = (step.x > 0) ? (static_cast<float>(mapPos.x + 1) - ro.x) * deltaDist.x : (ro.x - static_cast<float>(mapPos.x)) * deltaDist.x;
    sideDist.y = (step.y > 0) ? (static_cast<float>(mapPos.y + 1) - ro.y) * deltaDist.y : (ro.y - static_cast<float>(mapPos.y)) * deltaDist.y;
    sideDist.z = (step.z > 0) ? (static_cast<float>(mapPos.z + 1) - ro.z) * deltaDist.z : (ro.z - static_cast<float>(mapPos.z)) * deltaDist.z;

    glm::ivec3 hit_normal(0);
    float hit_dist = 0.0f;

    for (int i = 0; i < max_steps; ++i) {
        if (sideDist.x < sideDist.y) {
            if (sideDist.x < sideDist.z) {
                hit_dist = sideDist.x;
                sideDist.x += deltaDist.x;
                mapPos.x += step.x;
                hit_normal = glm::ivec3(-step.x, 0, 0);
            } else {
                hit_dist = sideDist.z;
                sideDist.z += deltaDist.z;
                mapPos.z += step.z;
                hit_normal = glm::ivec3(0, 0, -step.z);
            }
        } else {
            if (sideDist.y < sideDist.z) {
                hit_dist = sideDist.y;
                sideDist.y += deltaDist.y;
                mapPos.y += step.y;
                hit_normal = glm::ivec3(0, -step.y, 0);
            } else {
                hit_dist = sideDist.z;
                sideDist.z += deltaDist.z;
                mapPos.z += step.z;
                hit_normal = glm::ivec3(0, 0, -step.z);
            }
        }

        // Bounding volume check: y in [0, 32], z in [-64, 64], x in [-64, 64]
        if ((mapPos.y > 32 && step.y > 0) || (mapPos.y < 0 && step.y < 0) ||
            (mapPos.z > 64 && step.z > 0) || (mapPos.z < -64 && step.z < 0) ||
            (mapPos.x > 64 && step.x > 0) || (mapPos.x < -64 && step.x < 0)) {
            break;
        }

        const uint32_t mat = (mapPos.y >= z_cutaway_level) ? 0u : GetVoxelMaterialAt(mapPos);
        if (mat != 0) {
            result.hit = true;
            result.voxel_pos = mapPos;
            result.normal = hit_normal;
            result.placement_pos = mapPos + hit_normal;
            result.material_id = mat;
            result.distance = hit_dist;
            return result;
        }
    }
    return result;
}

} // namespace oasis::graphics
