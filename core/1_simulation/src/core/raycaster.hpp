#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <cmath>
#include <unordered_map>
#include <algorithm>

namespace oasis::graphics {

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

inline std::unordered_map<uint64_t, uint8_t>& GetDynamicVoxelStore() noexcept {
    static std::unordered_map<uint64_t, uint8_t> s_store;
    return s_store;
}

inline void SetDynamicVoxel(int x, int y, int z, uint8_t material_id) noexcept {
    GetDynamicVoxelStore()[PackVoxelCoord(x, y, z)] = material_id;
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
        if (absX >= 6 && absX <= 24) {
            return 4u; // Parched lawn
        }
    }

    // Layer 1-8: Electrical Grid Infrastructure
    if (pos.y >= 1 && pos.y <= 8) {
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
            return 9u; // Conductor wire
        }
    }

    // Layer 1-6: Suburban Residences
    if (absX >= 9 && absX <= 17 && lotZ >= 2 && lotZ <= 10) {
        if (absX == 9 && (lotZ == 4 || lotZ == 8) && pos.y == 2) {
            return 7u; // Window glass
        }
        if (pos.y >= 1 && pos.y <= 3) {
            return 5u; // Weathered wood siding
        }
        if (pos.y >= 4 && pos.y <= 6) {
            const int roof_inset = pos.y - 3;
            if (absX >= (9 + roof_inset) && absX <= (17 - roof_inset) &&
                lotZ >= (2 + roof_inset) && lotZ <= (10 - roof_inset)) {
                return 6u; // Dark shingle roof
            }
        }
    }

    // Layer 1: Property Boundary Fences
    if (pos.y == 1 && (lotZ == 15 || lotZ == 1) && absX >= 7 && absX <= 20) {
        return 10u; // Boundary fences
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
    int max_steps = 128
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

        // Bounding volume check
        if ((mapPos.y > 12 && step.y > 0) || (mapPos.y < 0 && step.y < 0) ||
            (mapPos.z > 42 && step.z > 0) || (mapPos.z < -10 && step.z < 0) ||
            (mapPos.x > 28 && step.x > 0) || (mapPos.x < -28 && step.x < 0)) {
            break;
        }

        const uint32_t mat = GetVoxelMaterialAt(mapPos);
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
