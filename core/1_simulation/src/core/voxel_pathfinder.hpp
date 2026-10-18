#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

#include "core/fabrication.hpp"
#include "core/raycaster.hpp"

namespace oasis::pathfinding {

constexpr uint32_t MAT_LADDER_ID      = 32u;
constexpr uint32_t MAT_STAGE_FRAME_ID = 33u;
constexpr uint32_t MAT_MATURE_CROP_ID = 34u;

constexpr int PATH_GRID_MIN_X = -16;
constexpr int PATH_GRID_MAX_X = 32;
constexpr int PATH_GRID_MIN_Y = 1;
constexpr int PATH_GRID_MAX_Y = 6;
constexpr int PATH_GRID_MIN_Z = -4;
constexpr int PATH_GRID_MAX_Z = 20;

constexpr int PATH_GRID_NX = (PATH_GRID_MAX_X - PATH_GRID_MIN_X + 1); // 49
constexpr int PATH_GRID_NY = (PATH_GRID_MAX_Y - PATH_GRID_MIN_Y + 1); // 6
constexpr int PATH_GRID_NZ = (PATH_GRID_MAX_Z - PATH_GRID_MIN_Z + 1); // 25
constexpr size_t PATH_GRID_TOTAL_NODES =
    static_cast<size_t>(PATH_GRID_NX * PATH_GRID_NY * PATH_GRID_NZ); // 7,350

struct VoxelPath {
    std::vector<glm::ivec3> waypoints{};
    uint32_t current_idx{0};
    bool valid{false};
    glm::ivec3 target_goal{0};

    [[nodiscard]] bool empty() const noexcept { return waypoints.empty(); }
    [[nodiscard]] size_t size() const noexcept { return waypoints.size(); }
    [[nodiscard]] bool IsComplete() const noexcept {
        return !valid || current_idx >= waypoints.size();
    }
    [[nodiscard]] bool ContainsWaypoint(glm::ivec3 v) const noexcept {
        for (const auto& wp : waypoints) {
            if (wp == v) return true;
        }
        return false;
    }
};

using VoxelPathComponent = VoxelPath;

[[nodiscard]] inline bool IsRoofAccessLadderVoxel(int x, int y, int z) noexcept {
    const uint8_t dyn = oasis::graphics::GetDynamicVoxel(x, y, z);
    if (dyn == MAT_LADDER_ID) {
        return true;
    }
    if (x == 18 && z == 3 && y >= 1 && y <= 4) {
        // Exterior Roof Access Ladder on East House Wall (18, 1..4, 3) unless blocked by a solid dynamic block
        return (dyn == 0u || dyn == MAT_LADDER_ID || dyn == 26u || dyn == MAT_STAGE_FRAME_ID);
    }
    return oasis::graphics::GetVoxelMaterialAt(glm::ivec3(x, y, z)) == MAT_LADDER_ID;
}

[[nodiscard]] inline bool IsVoxelPassableForAgent(int x, int y, int z) noexcept {
    if (y < 1 || y > 32) {
        return false;
    }

    // Explicitly treat Node 0 Sovereign Homelab Server Rack at (12, 1..2, 6) as solid obstacle
    if (x == 12 && z == 6 && (y == 1 || y == 2)) {
        return false;
    }

    if (IsRoofAccessLadderVoxel(x, y, z)) {
        return true;
    }

    const uint32_t mat = oasis::graphics::GetVoxelMaterialAt(glm::ivec3(x, y, z));
    switch (mat) {
        case 0u:  // Air
        case 15u: // Insulated Timber Door (Front Door (9,1,6), Back Patio Door (17,1,6), etc.)
        case 18u: // Swale
        case 19u: // Cover Crop
        case 20u: // Haul / Stockpile Zone
        case 21u: // Sleep Zone
        case 22u: // Recreation Zone
        case 23u: // Bell Tent / Master Bed
        case 25u: // Thermal Leak Marker
        case 26u: // Blueprint Ghost
        case MAT_LADDER_ID:      // 32u: Exterior Roof Access Ladder
        case MAT_STAGE_FRAME_ID: // 33u: Construction Stage 1 Scaffolding Frame
        case MAT_MATURE_CROP_ID: // 34u: Mature Harvestable Crop
            return true;
        case 6u:  // Dark Shingle Roof surface (walkable on roof deck y >= 4)
        case 17u: // Insulated Timber Roof surface (walkable on roof deck y >= 4)
            return (y >= 4 && y <= 6);
        default:
            // Impassable solid obstacles:
            // Walls (5u, 13u, 14u), Window Glass (7u), Utility Poles (8u, 9u),
            // Fences (10u), Solar Panels (11u), Cisterns (12u), Hearths (16u),
            // Ration/Salvage Crate (24u), Server Rack (30u), Loose Items (31u)
            return false;
    }
}

[[nodiscard]] inline bool IsVoxelPassableForAgent(const glm::ivec3& pos) noexcept {
    return IsVoxelPassableForAgent(pos.x, pos.y, pos.z);
}

[[nodiscard]] inline bool IsSolidGroundSupportMaterial(uint32_t mat) noexcept {
    return (mat >= 1u && mat <= 4u) ||
           mat == 18u || mat == 19u ||
           mat == 20u || mat == 21u || mat == 22u ||
           mat == MAT_MATURE_CROP_ID;
}

[[nodiscard]] inline bool IsVoxelStandable(int x, int y, int z) noexcept {
    if (y < PATH_GRID_MIN_Y || y > PATH_GRID_MAX_Y) {
        return false;
    }
    if (!IsVoxelPassableForAgent(x, y, z)) {
        return false;
    }

    // 1. Exterior Roof Access Ladder (x == 18, z == 3, y in [1..4] or MAT_LADDER_ID)
    if (IsRoofAccessLadderVoxel(x, y, z)) {
        return true;
    }

    // 2. Ground Floor & Interior/Exterior Ground Level (y == 1)
    if (y == 1) {
        const uint32_t ground_mat = oasis::graphics::GetVoxelMaterialAt(glm::ivec3(x, 0, z));
        return IsSolidGroundSupportMaterial(ground_mat);
    }

    // 3. Lot 402 Roof Deck & Eave (y in [4..6])
    if (y >= 4 && y <= 6) {
        const uint32_t below_mat = oasis::graphics::GetVoxelMaterialAt(glm::ivec3(x, y - 1, z));
        if (below_mat == 6u || below_mat == 17u) {
            return true;
        }
        // Roof eave at (17, 4, 3) directly above East House Wall (5u at y == 3) connecting Ladder (18, 4, 3)
        if (y == 4 && below_mat == 5u && x == 17 && z == 3) {
            return true;
        }
        // Lower roof shingle deck at y == 4 (e.g. (16, 4, 3) where roof slab sits over hollow room y == 3)
        const uint32_t cur_mat = oasis::graphics::GetVoxelMaterialAt(glm::ivec3(x, y, z));
        const uint32_t proc_mat = oasis::graphics::GetProceduralVoxelMaterial(glm::ivec3(x, y, z));
        if (y == 4 && (cur_mat == 6u || cur_mat == 17u || proc_mat == 6u)) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] inline bool IsVoxelStandable(const glm::ivec3& pos) noexcept {
    return IsVoxelStandable(pos.x, pos.y, pos.z);
}

[[nodiscard]] inline bool InPathGridBounds(int x, int y, int z) noexcept {
    return x >= PATH_GRID_MIN_X && x <= PATH_GRID_MAX_X &&
           y >= PATH_GRID_MIN_Y && y <= PATH_GRID_MAX_Y &&
           z >= PATH_GRID_MIN_Z && z <= PATH_GRID_MAX_Z;
}

[[nodiscard]] inline uint16_t PackPathNodeIndex(int x, int y, int z) noexcept {
    const int ix = x - PATH_GRID_MIN_X;
    const int iy = y - PATH_GRID_MIN_Y;
    const int iz = z - PATH_GRID_MIN_Z;
    return static_cast<uint16_t>((iy * PATH_GRID_NZ + iz) * PATH_GRID_NX + ix);
}

[[nodiscard]] inline glm::ivec3 UnpackPathNodeIndex(uint16_t idx) noexcept {
    const int ix = static_cast<int>(idx) % PATH_GRID_NX;
    const int rem = static_cast<int>(idx) / PATH_GRID_NX;
    const int iz = rem % PATH_GRID_NZ;
    const int iy = rem / PATH_GRID_NZ;
    return glm::ivec3(
        ix + PATH_GRID_MIN_X,
        iy + PATH_GRID_MIN_Y,
        iz + PATH_GRID_MIN_Z
    );
}

[[nodiscard]] inline glm::ivec3 ResolveStandableGoalVoxel(glm::ivec3 start, glm::ivec3 target) noexcept {
    glm::ivec3 clamped_target(
        std::clamp(target.x, PATH_GRID_MIN_X, PATH_GRID_MAX_X),
        std::clamp(target.y, PATH_GRID_MIN_Y, PATH_GRID_MAX_Y),
        std::clamp(target.z, PATH_GRID_MIN_Z, PATH_GRID_MAX_Z)
    );

    if (IsVoxelStandable(clamped_target)) {
        return clamped_target;
    }

    constexpr glm::ivec3 kHorizDirs[4] = {
        { 1, 0,  0},
        {-1, 0,  0},
        { 0, 0,  1},
        { 0, 0, -1}
    };

    // 1. Check 4-connected neighbors at the same y first
    glm::ivec3 best_goal = clamped_target;
    float best_dist_to_start = std::numeric_limits<float>::max();
    bool found = false;

    for (const auto& d : kHorizDirs) {
        const glm::ivec3 nb = clamped_target + d;
        if (InPathGridBounds(nb.x, nb.y, nb.z) && IsVoxelStandable(nb)) {
            const glm::vec3 diff = glm::vec3(nb - start);
            const float d2 = glm::dot(diff, diff);
            if (d2 < best_dist_to_start) {
                best_dist_to_start = d2;
                best_goal = nb;
                found = true;
            }
        }
    }
    if (found) {
        return best_goal;
    }

    // 2. If target is on the roof (target.y >= 4), search nearby standable roof surface voxels
    if (target.y >= 4) {
        float best_score = std::numeric_limits<float>::max();
        for (int ny = 4; ny <= 6; ++ny) {
            for (int dz = -2; dz <= 2; ++dz) {
                for (int dx = -2; dx <= 2; ++dx) {
                    const glm::ivec3 nb(clamped_target.x + dx, ny, clamped_target.z + dz);
                    if (InPathGridBounds(nb.x, nb.y, nb.z) && IsVoxelStandable(nb)) {
                        const float dist_target = glm::length(glm::vec3(nb - clamped_target));
                        const float dist_start = glm::length(glm::vec3(nb - start));
                        const float score = dist_target * 10.0f + dist_start * 0.01f;
                        if (score < best_score) {
                            best_score = score;
                            best_goal = nb;
                            found = true;
                        }
                    }
                }
            }
        }
        if (found) {
            return best_goal;
        }
    }

    // 3. Fallback search in local 2-voxel radius at clamped_target.y
    float best_fallback_score = std::numeric_limits<float>::max();
    for (int dz = -2; dz <= 2; ++dz) {
        for (int dx = -2; dx <= 2; ++dx) {
            const glm::ivec3 nb(clamped_target.x + dx, clamped_target.y, clamped_target.z + dz);
            if (InPathGridBounds(nb.x, nb.y, nb.z) && IsVoxelStandable(nb)) {
                const float dist_target = glm::length(glm::vec3(nb - clamped_target));
                const float dist_start = glm::length(glm::vec3(nb - start));
                const float score = dist_target * 10.0f + dist_start * 0.01f;
                if (score < best_fallback_score) {
                    best_fallback_score = score;
                    best_goal = nb;
                }
            }
        }
    }
    return best_goal;
}

[[nodiscard]] inline VoxelPath ComputeVoxelPath(glm::ivec3 start, glm::ivec3 target) {
    VoxelPath result{};
    result.target_goal = target;

    glm::ivec3 start_node(
        std::clamp(start.x, PATH_GRID_MIN_X, PATH_GRID_MAX_X),
        std::clamp(start.y, PATH_GRID_MIN_Y, PATH_GRID_MAX_Y),
        std::clamp(start.z, PATH_GRID_MIN_Z, PATH_GRID_MAX_Z)
    );
    if (!IsVoxelStandable(start_node)) {
        start_node = ResolveStandableGoalVoxel(start_node, start_node);
    }

    const glm::ivec3 goal_node = ResolveStandableGoalVoxel(start_node, target);
    if (!InPathGridBounds(start_node.x, start_node.y, start_node.z) ||
        !InPathGridBounds(goal_node.x, goal_node.y, goal_node.z) ||
        !IsVoxelStandable(start_node) ||
        !IsVoxelStandable(goal_node)) {
        return result;
    }

    if (start_node == goal_node) {
        result.waypoints.push_back(goal_node);
        result.valid = true;
        return result;
    }

    // Fixed-size stack arrays for zero-heap-churn 3D A* search across 7,350 nodes
    constexpr uint16_t kInvalidIdx = 0xFFFFu;
    float g_score[PATH_GRID_TOTAL_NODES];
    uint16_t parent[PATH_GRID_TOTAL_NODES];
    uint8_t state_flag[PATH_GRID_TOTAL_NODES]{}; // 0 = unvisited, 1 = open, 2 = closed

    for (size_t i = 0; i < PATH_GRID_TOTAL_NODES; ++i) {
        g_score[i] = std::numeric_limits<float>::infinity();
        parent[i] = kInvalidIdx;
    }

    struct OpenEntry {
        uint16_t idx{0};
        float f{0.0f};
    };
    OpenEntry open_heap[PATH_GRID_TOTAL_NODES];
    size_t open_count = 0;

    auto heap_push = [&](uint16_t idx, float f) noexcept {
        size_t pos = open_count++;
        open_heap[pos] = {idx, f};
        while (pos > 0) {
            const size_t p = (pos - 1) / 2;
            if (open_heap[p].f <= open_heap[pos].f) break;
            std::swap(open_heap[p], open_heap[pos]);
            pos = p;
        }
    };

    auto heap_pop = [&]() noexcept -> uint16_t {
        const uint16_t top = open_heap[0].idx;
        open_heap[0] = open_heap[--open_count];
        size_t pos = 0;
        while (true) {
            size_t left = 2 * pos + 1;
            size_t right = 2 * pos + 2;
            size_t smallest = pos;
            if (left < open_count && open_heap[left].f < open_heap[smallest].f) {
                smallest = left;
            }
            if (right < open_count && open_heap[right].f < open_heap[smallest].f) {
                smallest = right;
            }
            if (smallest == pos) break;
            std::swap(open_heap[pos], open_heap[smallest]);
            pos = smallest;
        }
        return top;
    };

    auto heuristic = [&](const glm::ivec3& a) noexcept -> float {
        return static_cast<float>(
            std::abs(a.x - goal_node.x) +
            std::abs(a.y - goal_node.y) * 2 +
            std::abs(a.z - goal_node.z)
        );
    };

    const uint16_t start_idx = PackPathNodeIndex(start_node.x, start_node.y, start_node.z);
    const uint16_t goal_idx  = PackPathNodeIndex(goal_node.x, goal_node.y, goal_node.z);

    g_score[start_idx] = 0.0f;
    state_flag[start_idx] = 1u;
    heap_push(start_idx, heuristic(start_node));

    struct NeighborCandidate {
        glm::ivec3 pos{0};
        float cost{1.0f};
    };

    constexpr glm::ivec3 kHorizDirs[4] = {
        { 1, 0,  0},
        {-1, 0,  0},
        { 0, 0,  1},
        { 0, 0, -1}
    };

    bool reached = false;

    while (open_count > 0) {
        const uint16_t cur_idx = heap_pop();
        if (state_flag[cur_idx] == 2u) {
            continue;
        }
        state_flag[cur_idx] = 2u;

        if (cur_idx == goal_idx) {
            reached = true;
            break;
        }

        const glm::ivec3 cur = UnpackPathNodeIndex(cur_idx);
        const float cur_g = g_score[cur_idx];

        NeighborCandidate candidates[12];
        size_t cand_count = 0;

        // 1. 4-connected horizontal neighbors at the same y
        for (const auto& d : kHorizDirs) {
            const glm::ivec3 nb = cur + d;
            if (InPathGridBounds(nb.x, nb.y, nb.z) && IsVoxelStandable(nb)) {
                candidates[cand_count++] = {nb, 1.0f};
            }
        }

        // 2. Vertical transitions on the Exterior Roof Access Ladder (18, 1..4, 3)
        if (IsRoofAccessLadderVoxel(cur.x, cur.y, cur.z)) {
            for (int dy : {-1, 1}) {
                const glm::ivec3 nb(cur.x, cur.y + dy, cur.z);
                if (InPathGridBounds(nb.x, nb.y, nb.z) && IsVoxelStandable(nb)) {
                    candidates[cand_count++] = {nb, 1.0f};
                }
            }
        }

        // 3. Pitched roof tier transitions (y in [4..6]) on Lot 402 roof deck (x in [10..16], z in [3..9])
        if (cur.y >= 4 && cur.y <= 6 && cur.x >= 10 && cur.x <= 16 && cur.z >= 3 && cur.z <= 9) {
            for (const auto& d : kHorizDirs) {
                for (int dy : {-1, 1}) {
                    const glm::ivec3 nb(cur.x + d.x, cur.y + dy, cur.z + d.z);
                    if (nb.y >= 4 && nb.y <= 6 &&
                        nb.x >= 10 && nb.x <= 16 && nb.z >= 3 && nb.z <= 9 &&
                        InPathGridBounds(nb.x, nb.y, nb.z) && IsVoxelStandable(nb)) {
                        candidates[cand_count++] = {nb, 1.25f};
                    }
                }
            }
        }

        for (size_t i = 0; i < cand_count; ++i) {
            const glm::ivec3 nb = candidates[i].pos;
            const uint16_t nb_idx = PackPathNodeIndex(nb.x, nb.y, nb.z);
            if (state_flag[nb_idx] == 2u) {
                continue;
            }
            const float tentative_g = cur_g + candidates[i].cost;
            if (tentative_g < g_score[nb_idx]) {
                g_score[nb_idx] = tentative_g;
                parent[nb_idx] = cur_idx;
                state_flag[nb_idx] = 1u;
                heap_push(nb_idx, tentative_g + heuristic(nb));
            }
        }
    }

    if (!reached) {
        return result;
    }

    // Reconstruct path from goal_idx back to start_idx
    uint16_t trace_stack[PATH_GRID_TOTAL_NODES];
    size_t trace_len = 0;
    uint16_t curr = goal_idx;
    while (curr != kInvalidIdx && trace_len < PATH_GRID_TOTAL_NODES) {
        trace_stack[trace_len++] = curr;
        if (curr == start_idx) break;
        curr = parent[curr];
    }

    result.waypoints.reserve(trace_len);
    for (size_t i = trace_len; i > 0; --i) {
        result.waypoints.push_back(UnpackPathNodeIndex(trace_stack[i - 1]));
    }
    // Advance past start waypoint if path has > 1 node so agent immediately moves toward next tile
    result.current_idx = (result.waypoints.size() > 1) ? 1u : 0u;
    result.valid = true;
    return result;
}

[[nodiscard]] inline float ResolveSupportingSurfaceHeight(const glm::vec3& world_pos) noexcept {
    const int rx = static_cast<int>(std::round(world_pos.x));
    const int rz = static_cast<int>(std::round(world_pos.z));
    const int ry = std::clamp(static_cast<int>(std::round(world_pos.y)), 1, 6);

    if (IsRoofAccessLadderVoxel(rx, ry, rz)) {
        return std::clamp(world_pos.y, 1.0f, 4.0f);
    }

    if (world_pos.y >= 3.5f) {
        for (int check_y = std::min(6, static_cast<int>(std::ceil(world_pos.y))); check_y >= 4; --check_y) {
            if (IsVoxelStandable(rx, check_y, rz)) {
                return static_cast<float>(check_y);
            }
        }
    }

    return 1.0f;
}

inline bool StepAgentAlongVoxelPath(
    glm::vec3& io_pos,
    VoxelPath& path,
    float effective_move_speed,
    float dt = 1.0f / 60.0f
) noexcept {
    const float speed = std::max(0.1f, effective_move_speed);
    const float step_dist = speed * dt;

    if (!path.valid || path.current_idx >= path.waypoints.size()) {
        // Apply gravity when idle or path finished so agent never floats in mid-air
        const int rx = static_cast<int>(std::round(io_pos.x));
        const int rz = static_cast<int>(std::round(io_pos.z));
        const int ry = std::clamp(static_cast<int>(std::round(io_pos.y)), 1, 6);
        if (!IsRoofAccessLadderVoxel(rx, ry, rz)) {
            const float support_y = ResolveSupportingSurfaceHeight(io_pos);
            if (io_pos.y > support_y) {
                io_pos.y = std::max(support_y, io_pos.y - 9.8f * dt);
            } else if (io_pos.y < support_y) {
                io_pos.y = support_y;
            }
        }
        return true;
    }

    const glm::ivec3 wp = path.waypoints[path.current_idx];
    const glm::vec3 target_vec(
        static_cast<float>(wp.x),
        static_cast<float>(wp.y),
        static_cast<float>(wp.z)
    );

    const int cur_rx = static_cast<int>(std::round(io_pos.x));
    const int cur_rz = static_cast<int>(std::round(io_pos.z));
    const bool on_ladder =
        (wp.x == 18 && wp.z == 3) ||
        (cur_rx == 18 && cur_rz == 3 && io_pos.y >= 1.0f && io_pos.y <= 4.2f);

    if (on_ladder) {
        const glm::vec3 to_target = target_vec - io_pos;
        const float dist = glm::length(to_target);
        if (dist <= step_dist) {
            io_pos = target_vec;
            path.current_idx++;
        } else {
            io_pos += (to_target / dist) * step_dist;
        }
    } else {
        const glm::vec2 horiz_to(target_vec.x - io_pos.x, target_vec.z - io_pos.z);
        const float horiz_dist = glm::length(horiz_to);
        if (horiz_dist <= step_dist) {
            io_pos.x = target_vec.x;
            io_pos.z = target_vec.z;
            io_pos.y = target_vec.y;
            path.current_idx++;
        } else {
            const glm::vec2 dir = horiz_to / horiz_dist;
            io_pos.x += dir.x * step_dist;
            io_pos.z += dir.y * step_dist;

            // Apply gravity toward supporting surface height (1.0f on ground/interior, 4.0f..6.0f on roof)
            const float support_y = (wp.y >= 4 || io_pos.y >= 3.5f)
                ? static_cast<float>(wp.y)
                : 1.0f;
            if (io_pos.y > support_y) {
                io_pos.y = std::max(support_y, io_pos.y - 9.8f * dt);
            } else if (io_pos.y < support_y) {
                io_pos.y = std::min(support_y, io_pos.y + speed * dt);
            }
        }
    }

    return path.current_idx >= path.waypoints.size();
}

inline bool StepAgentAlongVoxelPath(
    oasis::fabrication::SpatialPositionComponent& pos_comp,
    VoxelPath& path,
    float effective_move_speed,
    float dt = 1.0f / 60.0f
) noexcept {
    return StepAgentAlongVoxelPath(pos_comp.pos, path, effective_move_speed, dt);
}

inline bool StepAgentAlongVoxelPath(
    oasis::fabrication::SpatialPositionComponent& pos_comp,
    VoxelPath& path,
    float dt = 1.0f / 60.0f
) noexcept {
    return StepAgentAlongVoxelPath(pos_comp.pos, path, pos_comp.move_speed, dt);
}

} // namespace oasis::pathfinding

namespace oasis {
    using oasis::pathfinding::MAT_LADDER_ID;
    using oasis::pathfinding::MAT_STAGE_FRAME_ID;
    using oasis::pathfinding::MAT_MATURE_CROP_ID;
    using oasis::pathfinding::VoxelPath;
    using oasis::pathfinding::VoxelPathComponent;
    using oasis::pathfinding::IsRoofAccessLadderVoxel;
    using oasis::pathfinding::IsVoxelPassableForAgent;
    using oasis::pathfinding::IsVoxelStandable;
    using oasis::pathfinding::ResolveStandableGoalVoxel;
    using oasis::pathfinding::ComputeVoxelPath;
    using oasis::pathfinding::ResolveSupportingSurfaceHeight;
    using oasis::pathfinding::StepAgentAlongVoxelPath;
} // namespace oasis
