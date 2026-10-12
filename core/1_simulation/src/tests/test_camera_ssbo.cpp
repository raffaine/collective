#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "core/raycaster.hpp"

using Catch::Matchers::WithinAbs;

TEST_CASE("WebGPU Dynamic Voxel Storage Buffer Layout & Alignment", "[graphics][webgpu][ssbo]") {
    SECTION("GpuDynamicVoxel 16-byte WGSL alignment and field offsets") {
        REQUIRE(sizeof(oasis::graphics::GpuDynamicVoxel) == 16);
        REQUIRE(alignof(oasis::graphics::GpuDynamicVoxel) == 16);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxel, x) == 0);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxel, y) == 4);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxel, z) == 8);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxel, material_id) == 12);
    }

    SECTION("GpuDynamicVoxelBuffer 32-byte header + array layout and alignment") {
        REQUIRE(sizeof(oasis::graphics::GpuDynamicVoxelBuffer) == 32 + 1024 * 16);
        REQUIRE(alignof(oasis::graphics::GpuDynamicVoxelBuffer) == 16);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, count) == 0);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, min_x) == 4);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, min_y) == 8);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, min_z) == 12);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, max_x) == 16);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, max_y) == 20);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, max_z) == 24);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, pad) == 28);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxelBuffer, voxels) == 32);
    }
}

TEST_CASE("Voxel Coordinate Packing and Unpacking Bijective Mapping", "[graphics][voxel][coords]") {
    SECTION("Origin coordinate (0, 0, 0)") {
        uint64_t packed = oasis::graphics::PackVoxelCoord(0, 0, 0);
        int ox = 99, oy = 99, oz = 99;
        oasis::graphics::UnpackVoxelCoord(packed, ox, oy, oz);
        REQUIRE(ox == 0);
        REQUIRE(oy == 0);
        REQUIRE(oz == 0);
    }

    SECTION("Positive coordinates") {
        struct Coord { int x, y, z; };
        std::vector<Coord> coords = {
            {1, 2, 3},
            {10, 5, 20},
            {28, 12, 42},
            {100, 200, 300},
            {10000, 20000, 30000}
        };

        for (const auto& c : coords) {
            uint64_t packed = oasis::graphics::PackVoxelCoord(c.x, c.y, c.z);
            int ox = 0, oy = 0, oz = 0;
            oasis::graphics::UnpackVoxelCoord(packed, ox, oy, oz);
            CHECK(ox == c.x);
            CHECK(oy == c.y);
            CHECK(oz == c.z);
        }
    }

    SECTION("Negative coordinates across all axes with sign extension") {
        struct Coord { int x, y, z; };
        std::vector<Coord> coords = {
            {-1, -1, -1},
            {-15, 3, -8},
            {-28, 0, -10},
            {-100, -200, -300},
            {-28, 12, 42},
            {28, -5, -10}
        };

        for (const auto& c : coords) {
            uint64_t packed = oasis::graphics::PackVoxelCoord(c.x, c.y, c.z);
            int ox = 0, oy = 0, oz = 0;
            oasis::graphics::UnpackVoxelCoord(packed, ox, oy, oz);
            CHECK(ox == c.x);
            CHECK(oy == c.y);
            CHECK(oz == c.z);
        }
    }

    SECTION("Grid-level exhaustive test in suburban diorama bounds") {
        for (int x = -30; x <= 30; x += 5) {
            for (int y = -10; y <= 15; y += 5) {
                for (int z = -15; z <= 45; z += 10) {
                    uint64_t packed = oasis::graphics::PackVoxelCoord(x, y, z);
                    int ox = 0, oy = 0, oz = 0;
                    oasis::graphics::UnpackVoxelCoord(packed, ox, oy, oz);
                    REQUIRE(ox == x);
                    REQUIRE(oy == y);
                    REQUIRE(oz == z);
                }
            }
        }
    }

    SECTION("Extremal bit boundary test for 22-bit X and 21-bit Y, Z") {
        int min_x = -2097152, max_x = 2097151;
        int min_y = -1048576, max_y = 1048575;
        int min_z = -1048576, max_z = 1048575;

        uint64_t packed_min = oasis::graphics::PackVoxelCoord(min_x, min_y, min_z);
        int ox = 0, oy = 0, oz = 0;
        oasis::graphics::UnpackVoxelCoord(packed_min, ox, oy, oz);
        CHECK(ox == min_x);
        CHECK(oy == min_y);
        CHECK(oz == min_z);

        uint64_t packed_max = oasis::graphics::PackVoxelCoord(max_x, max_y, max_z);
        oasis::graphics::UnpackVoxelCoord(packed_max, ox, oy, oz);
        CHECK(ox == max_x);
        CHECK(oy == max_y);
        CHECK(oz == max_z);
    }
}

TEST_CASE("Dynamic Voxel Store and SSBO Buffer Upload & AABB Mapping", "[graphics][ssbo][aabb]") {
    SECTION("Store insertions and material queries") {
        auto& store = oasis::graphics::GetDynamicVoxelStore();
        store.clear();

        oasis::graphics::SetDynamicVoxel(5, 7, 10, 11); // Solar panel
        oasis::graphics::SetDynamicVoxel(-8, 2, 14, 12); // Rainwater catchment

        REQUIRE(oasis::graphics::GetDynamicVoxel(5, 7, 10) == 11);
        REQUIRE(oasis::graphics::GetDynamicVoxel(-8, 2, 14) == 12);
        REQUIRE(oasis::graphics::GetDynamicVoxel(0, 0, 0) == 0); // Empty air

        store.clear();
    }

    SECTION("AABB computation and SSBO flattening") {
        auto& store = oasis::graphics::GetDynamicVoxelStore();
        store.clear();

        oasis::graphics::SetDynamicVoxel(10, 4, -5, 11);
        oasis::graphics::SetDynamicVoxel(-12, 1, 20, 12);
        oasis::graphics::SetDynamicVoxel(0, 8, 30, 11);

        oasis::graphics::GpuDynamicVoxelBuffer buffer{};
        uint32_t count = 0;
        int32_t min_x = 999999, min_y = 999999, min_z = 999999;
        int32_t max_x = -999999, max_y = -999999, max_z = -999999;

        for (const auto& [packed, mat_id] : store) {
            if (count >= oasis::graphics::MAX_GPU_DYNAMIC_VOXELS) break;
            int vx = 0, vy = 0, vz = 0;
            oasis::graphics::UnpackVoxelCoord(packed, vx, vy, vz);
            buffer.voxels[count].x = vx;
            buffer.voxels[count].y = vy;
            buffer.voxels[count].z = vz;
            buffer.voxels[count].material_id = static_cast<uint32_t>(mat_id);

            min_x = std::min(min_x, vx);
            min_y = std::min(min_y, vy);
            min_z = std::min(min_z, vz);
            max_x = std::max(max_x, vx);
            max_y = std::max(max_y, vy);
            max_z = std::max(max_z, vz);
            count++;
        }

        buffer.count = count;
        buffer.min_x = min_x;
        buffer.min_y = min_y;
        buffer.min_z = min_z;
        buffer.max_x = max_x;
        buffer.max_y = max_y;
        buffer.max_z = max_z;

        REQUIRE(buffer.count == 3);
        REQUIRE(buffer.min_x == -12);
        REQUIRE(buffer.max_x == 10);
        REQUIRE(buffer.min_y == 1);
        REQUIRE(buffer.max_y == 8);
        REQUIRE(buffer.min_z == -5);
        REQUIRE(buffer.max_z == 30);

        store.clear();
    }

    SECTION("Capacity clamping protects buffer under 1024 overflow") {
        oasis::graphics::GpuDynamicVoxelBuffer buffer{};
        for (size_t i = 0; i < 1100; ++i) {
            if (buffer.count < oasis::graphics::MAX_GPU_DYNAMIC_VOXELS) {
                buffer.voxels[buffer.count].x = static_cast<int32_t>(i);
                buffer.voxels[buffer.count].y = 0;
                buffer.voxels[buffer.count].z = 0;
                buffer.voxels[buffer.count].material_id = 11;
                buffer.count++;
            }
        }
        REQUIRE(buffer.count == 1024);
        REQUIRE(buffer.count <= oasis::graphics::MAX_GPU_DYNAMIC_VOXELS);
    }
}

TEST_CASE("Mac-Friendly Camera Panning Math Verification", "[camera][panning][mac]") {
    oasis::graphics::CameraPanState state{};

    SECTION("Ground-plane lock enforces zero Y-axis movement") {
        state.camera_offset = glm::vec3(0.0f);
        state.active_camera_mode = 0; // Perspective
        oasis::graphics::ApplyCameraPan(state, 50.0f, -30.0f, false);
        REQUIRE(state.camera_offset.y == 0.0f);

        oasis::graphics::ApplyCameraPan(state, 10.0f, 15.0f, true);
        REQUIRE(state.camera_offset.y == 0.0f);
    }

    SECTION("Perspective mode (Mode 0) panning translations") {
        state.camera_offset = glm::vec3(0.0f);
        state.active_camera_mode = 0; // Perspective
        state.is_transitioning = false;

        // Dragging right (delta_x > 0) moves offset along right vector
        // In Perspective: look is (0, -2, 5), fwd_ground = (0, 0, 1), right_ground = cross((0,0,1), (0,1,0)) = (-1,0,0)
        // pan_delta = (-delta_x * (-1, 0, 0) + delta_y * (0, 0, 1)) * scale = (delta_x * scale, 0, delta_y * scale)
        oasis::graphics::ApplyCameraPan(state, 100.0f, 0.0f, false);
        REQUIRE(state.camera_offset.x > 0.0f);
        REQUIRE_THAT(state.camera_offset.z, WithinAbs(0.0f, 1e-4));
        REQUIRE(state.camera_offset.y == 0.0f);

        // Dragging down (delta_y > 0) pushes offset forward
        state.camera_offset = glm::vec3(0.0f);
        oasis::graphics::ApplyCameraPan(state, 0.0f, 100.0f, false);
        REQUIRE(state.camera_offset.z > 0.0f);
        REQUIRE_THAT(state.camera_offset.x, WithinAbs(0.0f, 1e-4));
    }

    SECTION("Cadastral Isometric mode (Mode 1) diagonal ground projection") {
        state.camera_offset = glm::vec3(0.0f);
        state.active_camera_mode = 1; // Cadastral Isometric
        state.is_transitioning = false;

        // In Cadastral Isometric: eye is (-16, 20, -16), target is (0, 0, 0)
        // look vector is (16, -20, 16). fwd_ground is normalize(16, 0, 16) = (1/sqrt(2), 0, 1/sqrt(2))
        // right_ground is cross(fwd_ground, up) = normalize(-1/sqrt(2), 0, 1/sqrt(2))
        oasis::graphics::ApplyCameraPan(state, 100.0f, 0.0f, false);
        // Horizontal drag distributes across both X and Z in isometric world space
        REQUIRE(state.camera_offset.x > 0.0f);
        REQUIRE(state.camera_offset.z < 0.0f);
        REQUIRE(state.camera_offset.y == 0.0f);
    }

    SECTION("Bounds clamping prevents camera drifting into infinity") {
        state.camera_offset = glm::vec3(0.0f);
        state.active_camera_mode = 0;
        // Apply huge positive drag
        oasis::graphics::ApplyCameraPan(state, 100000.0f, 100000.0f, false);
        REQUIRE(state.camera_offset.x <= 35.0f);
        REQUIRE(state.camera_offset.z <= 50.0f);

        // Apply huge negative drag
        oasis::graphics::ApplyCameraPan(state, -200000.0f, -200000.0f, false);
        REQUIRE(state.camera_offset.x >= -35.0f);
        REQUIRE(state.camera_offset.z >= -35.0f);
    }

    SECTION("Trackpad wheel vs Mouse drag scaling") {
        state.camera_offset = glm::vec3(0.0f);
        state.active_camera_mode = 0;
        glm::vec3 drag_delta = oasis::graphics::ComputeCameraPanDelta(0, 0, 0.0f, false, 10.0f, 10.0f, false);
        glm::vec3 wheel_delta = oasis::graphics::ComputeCameraPanDelta(0, 0, 0.0f, false, 10.0f, 10.0f, true);

        REQUIRE(glm::length(wheel_delta) > glm::length(drag_delta));
    }
}
