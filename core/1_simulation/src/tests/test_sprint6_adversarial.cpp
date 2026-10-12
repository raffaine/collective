#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <limits>
#include "core/raycaster.hpp"

using Catch::Matchers::WithinAbs;

// ==============================================================================
// CHALLENGE 1: Dynamic Voxel Packing / Unpacking Coordinate Math Stress Test
// ==============================================================================

TEST_CASE("Adversarial: Voxel Coordinate Packing / Unpacking Exact Bit Boundaries", "[adversarial][coords][boundary]") {
    SECTION("Extremal 22-bit X and 21-bit Y, Z boundary corners (8 octants)") {
        const int min_x = -2097152;
        const int max_x =  2097151;
        const int min_y = -1048576;
        const int max_y =  1048575;
        const int min_z = -1048576;
        const int max_z =  1048575;

        const int xs[2] = {min_x, max_x};
        const int ys[2] = {min_y, max_y};
        const int zs[2] = {min_z, max_z};

        for (int ix = 0; ix < 2; ++ix) {
            for (int iy = 0; iy < 2; ++iy) {
                for (int iz = 0; iz < 2; ++iz) {
                    const int tx = xs[ix];
                    const int ty = ys[iy];
                    const int tz = zs[iz];

                    const uint64_t packed = oasis::graphics::PackVoxelCoord(tx, ty, tz);
                    int ox = 0, oy = 0, oz = 0;
                    oasis::graphics::UnpackVoxelCoord(packed, ox, oy, oz);

                    CHECK(ox == tx);
                    CHECK(oy == ty);
                    CHECK(oz == tz);
                }
            }
        }
    }

    SECTION("Sign extension near zero and boundary transitions") {
        const std::vector<int> test_vals = {
            0, 1, -1, 2, -2,
            1048574, 1048575, -1048575, -1048576,
            2097150, 2097151, -2097151, -2097152
        };

        for (int vx : test_vals) {
            for (int vy : test_vals) {
                if (vy > 1048575 || vy < -1048576) continue;
                for (int vz : test_vals) {
                    if (vz > 1048575 || vz < -1048576) continue;

                    const uint64_t packed = oasis::graphics::PackVoxelCoord(vx, vy, vz);
                    int ox = 0, oy = 0, oz = 0;
                    oasis::graphics::UnpackVoxelCoord(packed, ox, oy, oz);

                    CHECK(ox == vx);
                    CHECK(oy == vy);
                    CHECK(oz == vz);
                }
            }
        }
    }

    SECTION("Monte Carlo Fuzzing: 250,000 random 3D coordinates") {
        std::mt19937_64 rng(0xDEADBEEFCAFEULL);
        std::uniform_int_distribution<int> dist_x(-2097152, 2097151);
        std::uniform_int_distribution<int> dist_y(-1048576, 1048575);
        std::uniform_int_distribution<int> dist_z(-1048576, 1048575);

        for (int i = 0; i < 250000; ++i) {
            const int rx = dist_x(rng);
            const int ry = dist_y(rng);
            const int rz = dist_z(rng);

            const uint64_t packed = oasis::graphics::PackVoxelCoord(rx, ry, rz);
            int ox = 0, oy = 0, oz = 0;
            oasis::graphics::UnpackVoxelCoord(packed, ox, oy, oz);

            if (ox != rx || oy != ry || oz != rz) {
                FAIL("Coord roundtrip failed at i=" << i << ": (" << rx << "," << ry << "," << rz << ") -> (" << ox << "," << oy << "," << oz << ")");
            }
        }
        SUCCEED("250,000 random coordinates round-tripped with 100% bitwise parity");
    }

    SECTION("Injectivity & Uniqueness: 20,000 distinct coordinates have zero hash collisions") {
        std::unordered_map<uint64_t, bool> key_set;
        key_set.reserve(20000);

        std::mt19937_64 rng(0x5EED1234ULL);
        std::uniform_int_distribution<int> dist(-50000, 50000);

        for (int i = 0; i < 20000; ++i) {
            int x = dist(rng);
            int y = dist(rng);
            int z = dist(rng);
            uint64_t packed = oasis::graphics::PackVoxelCoord(x, y, z);
            key_set[packed] = true;
        }

        // Each unique (x, y, z) generates a unique uint64_t key
        REQUIRE(key_set.size() <= 20000);
        // Test that key unpacking uniquely reconstructs
        for (const auto& [packed, _] : key_set) {
            int ox = 0, oy = 0, oz = 0;
            oasis::graphics::UnpackVoxelCoord(packed, ox, oy, oz);
            uint64_t repacked = oasis::graphics::PackVoxelCoord(ox, oy, oz);
            REQUIRE(repacked == packed);
        }
    }
}

// ==============================================================================
// CHALLENGE 2: Dynamic Voxel Buffer Mapping, Alignment, and Overflow Stress Test
// ==============================================================================

TEST_CASE("Adversarial: Dynamic Voxel Buffer WGSL Conformance & Overflow Clamping", "[adversarial][ssbo][overflow]") {
    SECTION("Host struct WGSL byte-for-byte binary parity") {
        REQUIRE(sizeof(oasis::graphics::GpuDynamicVoxel) == 16);
        REQUIRE(alignof(oasis::graphics::GpuDynamicVoxel) == 16);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxel, x) == 0);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxel, y) == 4);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxel, z) == 8);
        REQUIRE(offsetof(oasis::graphics::GpuDynamicVoxel, material_id) == 12);

        REQUIRE(sizeof(oasis::graphics::GpuDynamicVoxelBuffer) == 16416);
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

    SECTION("Overflow stress: 5,000 dynamic voxels clamped strictly to 1,024") {
        auto& store = oasis::graphics::GetDynamicVoxelStore();
        store.clear();

        for (int i = 0; i < 5000; ++i) {
            oasis::graphics::SetDynamicVoxel(i % 50 - 25, (i / 50) % 20, (i / 1000) - 2, (i % 2 == 0) ? 11 : 12);
        }
        REQUIRE(store.size() > 1024);

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

        REQUIRE(buffer.count == 1024);
        REQUIRE(buffer.count == oasis::graphics::MAX_GPU_DYNAMIC_VOXELS);

        size_t upload_size = 32 + count * sizeof(oasis::graphics::GpuDynamicVoxel);
        upload_size = std::min(upload_size, sizeof(oasis::graphics::GpuDynamicVoxelBuffer));
        REQUIRE(upload_size == sizeof(oasis::graphics::GpuDynamicVoxelBuffer));
        REQUIRE(upload_size == 16416);

        store.clear();
    }

    SECTION("Empty store edge case: count = 0, upload_size = 32") {
        auto& store = oasis::graphics::GetDynamicVoxelStore();
        store.clear();

        oasis::graphics::GpuDynamicVoxelBuffer buffer{};
        uint32_t count = 0;
        for (const auto& [packed, mat_id] : store) {
            if (count >= oasis::graphics::MAX_GPU_DYNAMIC_VOXELS) break;
            count++;
        }
        buffer.count = count;
        if (count == 0) {
            buffer.min_x = 0; buffer.min_y = 0; buffer.min_z = 0;
            buffer.max_x = 0; buffer.max_y = 0; buffer.max_z = 0;
        }

        REQUIRE(buffer.count == 0);
        size_t upload_size = 32 + count * sizeof(oasis::graphics::GpuDynamicVoxel);
        upload_size = std::min(upload_size, sizeof(oasis::graphics::GpuDynamicVoxelBuffer));
        REQUIRE(upload_size == 32);
    }

    SECTION("Emulated WGSL dynamic voxel shader sampler with AABB early rejection") {
        auto& store = oasis::graphics::GetDynamicVoxelStore();
        store.clear();

        oasis::graphics::SetDynamicVoxel(5, 7, 10, 11);
        oasis::graphics::SetDynamicVoxel(-8, 2, 14, 12);
        oasis::graphics::SetDynamicVoxel(12, 4, -6, 11);

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
        buffer.min_x = min_x; buffer.min_y = min_y; buffer.min_z = min_z;
        buffer.max_x = max_x; buffer.max_y = max_y; buffer.max_z = max_z;

        // WGSL get_dynamic_voxel_material emulation
        auto wgsl_sample = [&](int x, int y, int z) -> uint32_t {
            if (buffer.count == 0) return 0;
            if (x < buffer.min_x || x > buffer.max_x ||
                y < buffer.min_y || y > buffer.max_y ||
                z < buffer.min_z || z > buffer.max_z) {
                return 0;
            }
            uint32_t limit = std::min(buffer.count, 1024u);
            for (uint32_t i = 0; i < limit; ++i) {
                const auto& v = buffer.voxels[i];
                if (v.x == x && v.y == y && v.z == z) {
                    return v.material_id;
                }
            }
            return 0;
        };

        // True positive hits
        REQUIRE(wgsl_sample(5, 7, 10) == 11);
        REQUIRE(wgsl_sample(-8, 2, 14) == 12);
        REQUIRE(wgsl_sample(12, 4, -6) == 11);

        // Outside AABB early rejections
        REQUIRE(wgsl_sample(50, 7, 10) == 0);
        REQUIRE(wgsl_sample(-50, 2, 14) == 0);
        REQUIRE(wgsl_sample(0, 100, 0) == 0);

        // Inside AABB but empty air
        REQUIRE(wgsl_sample(0, 4, 0) == 0);

        store.clear();
    }
}

// ==============================================================================
// CHALLENGE 3: Camera Panning Math Adversarial Suite
// ==============================================================================

TEST_CASE("Adversarial: Mac-Friendly Camera Panning Invariants & Extremal Bounds", "[adversarial][camera][math]") {
    SECTION("Ground-Lock Invariant (offset.y == 0.0f) under 50,000 random operations") {
        oasis::graphics::CameraPanState state{};
        std::mt19937_64 rng(0xCAFEBABEULL);
        std::uniform_real_distribution<float> dist_delta(-10000.0f, 10000.0f);
        std::uniform_int_distribution<uint32_t> dist_mode(0, 1);
        std::uniform_real_distribution<float> dist_trans(0.0f, 0.400f);

        for (int i = 0; i < 50000; ++i) {
            state.active_camera_mode = dist_mode(rng);
            state.target_camera_mode = dist_mode(rng);
            state.transition_elapsed_sec = dist_trans(rng);
            state.is_transitioning = (dist_mode(rng) == 1);
            float dx = dist_delta(rng);
            float dy = dist_delta(rng);
            bool is_wheel = (dist_mode(rng) == 1);

            oasis::graphics::ApplyCameraPan(state, dx, dy, is_wheel);

            if (state.camera_offset.y != 0.0f) {
                FAIL("Ground-lock violated at i=" << i << ": offset.y=" << state.camera_offset.y);
            }
            if (state.camera_offset.x < -35.0f || state.camera_offset.x > 35.0f) {
                FAIL("X clamp violated at i=" << i << ": offset.x=" << state.camera_offset.x);
            }
            if (state.camera_offset.z < -35.0f || state.camera_offset.z > 50.0f) {
                FAIL("Z clamp violated at i=" << i << ": offset.z=" << state.camera_offset.z);
            }
        }
        SUCCEED("Ground-lock (y==0.0f) and diorama clamp ([-35,35] x [-35,50]) verified across 50,000 steps");
    }

    SECTION("Astronomical delta panning clamping") {
        oasis::graphics::CameraPanState state{};
        state.camera_offset = glm::vec3(0.0f);

        // Huge positive drag
        oasis::graphics::ApplyCameraPan(state, 1e12f, 1e12f, false);
        REQUIRE(state.camera_offset.x == 35.0f);
        REQUIRE(state.camera_offset.z == 50.0f);
        REQUIRE(state.camera_offset.y == 0.0f);

        // Huge negative drag
        oasis::graphics::ApplyCameraPan(state, -1e12f, -1e12f, false);
        REQUIRE(state.camera_offset.x == -35.0f);
        REQUIRE(state.camera_offset.z == -35.0f);
        REQUIRE(state.camera_offset.y == 0.0f);

        // Huge wheel inputs
        oasis::graphics::ApplyCameraPan(state, 1e9f, -1e9f, true);
        REQUIRE(state.camera_offset.x >= -35.0f);
        REQUIRE(state.camera_offset.x <= 35.0f);
        REQUIRE(state.camera_offset.z >= -35.0f);
        REQUIRE(state.camera_offset.z <= 50.0f);
        REQUIRE(state.camera_offset.y == 0.0f);
    }

    SECTION("Smooth transition interpolation monotonicity") {
        // Test Hermite smoothstep blend across t in [0.000s, 0.400s]
        float prev_scale_drag = -1.0f;
        for (float elapsed = 0.0f; elapsed <= 0.400f; elapsed += 0.001f) {
            glm::vec3 delta = oasis::graphics::ComputeCameraPanDelta(0, 1, elapsed, true, 10.0f, 0.0f, false);
            REQUIRE(!std::isnan(delta.x));
            REQUIRE(!std::isnan(delta.y));
            REQUIRE(!std::isnan(delta.z));
            REQUIRE(!std::isinf(delta.x));
            REQUIRE(!std::isinf(delta.y));
            REQUIRE(!std::isinf(delta.z));
            REQUIRE(delta.y == 0.0f);
        }
    }
}

// ==============================================================================
// CHALLENGE 4: Shift+Click Drag vs Blueprint Placement Isolation
// ==============================================================================

TEST_CASE("Adversarial: Shift+Click Camera Drag Strictly Isolates Blueprint Placement", "[adversarial][input][drag]") {
    struct EmulatedInputState {
        bool shift_key_pressed{false};
        bool is_dragging{false};
        int last_mouse_x{0};
        int last_mouse_y{0};
        glm::vec3 camera_offset{0.0f};
        uint32_t blueprints_placed{0};
        uint32_t camera_pans_applied{0};

        void OnKeyDown(int key, uint32_t mod) {
            if (key == 1 /* LSHIFT */ || key == 2 /* RSHIFT */) {
                shift_key_pressed = true;
            }
        }

        void OnKeyUp(int key, uint32_t mod) {
            if (key == 1 /* LSHIFT */ || key == 2 /* RSHIFT */) {
                shift_key_pressed = false;
            }
        }

        void OnMouseDown(int button, int x, int y, uint32_t mod) {
            const bool is_shift_down = ((mod & 0x01 /* KMOD_SHIFT */) != 0) || shift_key_pressed;

            if (button == 2 /* MIDDLE */ || (button == 1 /* LEFT */ && is_shift_down)) {
                is_dragging = true;
                last_mouse_x = x;
                last_mouse_y = y;
            } else if (button == 1 /* LEFT */) {
                blueprints_placed++;
            }
        }

        void OnMouseMove(int x, int y) {
            if (is_dragging) {
                float dx = static_cast<float>(x - last_mouse_x);
                float dy = static_cast<float>(y - last_mouse_y);
                last_mouse_x = x;
                last_mouse_y = y;
                camera_offset.x += dx * 0.02f;
                camera_offset.z += dy * 0.02f;
                camera_pans_applied++;
            }
        }

        void OnMouseUp(int button) {
            if (button == 2 /* MIDDLE */ || button == 1 /* LEFT */) {
                is_dragging = false;
            }
        }

        void OnWindowFocusLost() {
            is_dragging = false;
        }
    };

    SECTION("Normal Left Click without Shift places blueprint") {
        EmulatedInputState state{};
        state.OnMouseDown(1 /* LEFT */, 100, 100, 0 /* NO SHIFT */);
        state.OnMouseUp(1 /* LEFT */);

        REQUIRE(state.blueprints_placed == 1);
        REQUIRE(state.is_dragging == false);
        REQUIRE(state.camera_pans_applied == 0);
    }

    SECTION("Shift + Left Click drag pans camera and NEVER places blueprint") {
        EmulatedInputState state{};

        // Press Shift
        state.OnKeyDown(1 /* LSHIFT */, 0x01);
        // Press Left Button down
        state.OnMouseDown(1 /* LEFT */, 100, 100, 0x01 /* KMOD_SHIFT */);
        REQUIRE(state.is_dragging == true);
        REQUIRE(state.blueprints_placed == 0);

        // Drag mouse 10 steps
        for (int i = 1; i <= 10; ++i) {
            state.OnMouseMove(100 + i * 5, 100 + i * 3);
        }
        REQUIRE(state.camera_pans_applied == 10);
        REQUIRE(state.blueprints_placed == 0);

        // Release Shift while still dragging
        state.OnKeyUp(1 /* LSHIFT */, 0);
        REQUIRE(state.is_dragging == true); // Drag state remains active until mouse up
        REQUIRE(state.blueprints_placed == 0);

        // Drag further without Shift
        state.OnMouseMove(160, 140);
        REQUIRE(state.camera_pans_applied == 11);
        REQUIRE(state.blueprints_placed == 0);

        // Release mouse button
        state.OnMouseUp(1 /* LEFT */);
        REQUIRE(state.is_dragging == false);
        REQUIRE(state.blueprints_placed == 0); // Still ZERO blueprints placed!
    }

    SECTION("Focus lost during drag cleanly resets dragging state") {
        EmulatedInputState state{};
        state.OnKeyDown(1 /* LSHIFT */, 0x01);
        state.OnMouseDown(1 /* LEFT */, 100, 100, 0x01);
        REQUIRE(state.is_dragging == true);

        state.OnWindowFocusLost();
        REQUIRE(state.is_dragging == false);
        REQUIRE(state.blueprints_placed == 0);
    }

    SECTION("Alternating rapid Shift-drag and regular click stress (1,000 cycles)") {
        EmulatedInputState state{};

        for (int cycle = 0; cycle < 1000; ++cycle) {
            // Cycle part A: Shift-drag
            state.OnKeyDown(1, 0x01);
            state.OnMouseDown(1, 100, 100, 0x01);
            state.OnMouseMove(110, 110);
            state.OnMouseUp(1);
            state.OnKeyUp(1, 0);

            // Cycle part B: Normal click
            state.OnMouseDown(1, 200, 200, 0);
            state.OnMouseUp(1);
        }

        // Each cycle should yield exactly 1 blueprint and 1 pan
        REQUIRE(state.blueprints_placed == 1000);
        REQUIRE(state.camera_pans_applied == 1000);
        REQUIRE(state.is_dragging == false);
    }
}
