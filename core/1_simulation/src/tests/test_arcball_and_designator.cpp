#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <flecs.h>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

#include "core/arcball_camera.hpp"
#include "core/spatial_designator.hpp"
#include "core/raycaster.hpp"
#include "core/fabrication.hpp"
#include "uhai/uhai_ring_buffer.hpp"

using Catch::Approx;
using namespace oasis::graphics;
using namespace oasis::designator;
using namespace oasis::fabrication;
using namespace oasis::uhai;

// ============================================================================
// STORY-8.1: POI Lock, Strict Ray-Miss Guard, Gimbal-Lock Prevention & Cutaway
// ============================================================================

TEST_CASE("STORY-8.1: Arcball POI Lock & Strict Ray-Miss Guard", "[story-8.1][arcball]") {
    ArcballCameraState cam{};
    cam.poi_origin = glm::vec3(2.0f, 1.0f, 3.0f);
    cam.target_poi = glm::vec3(2.0f, 1.0f, 3.0f);
    cam.theta_rad = 0.45f;
    cam.target_theta_rad = 0.45f;
    cam.phi_rad = 0.72f;
    cam.target_phi_rad = 0.72f;
    cam.radius = 18.5f;
    cam.target_radius = 18.5f;
    cam.z_cutaway_level = ARCBALL_MAX_CUTAWAY_Z;

    SECTION("Sky raycast miss (hit.hit == false) leaves camera state 100% unchanged") {
        RaycastHit sky_miss{};
        sky_miss.hit = false;
        sky_miss.voxel_pos = glm::ivec3(99, 99, 99);

        const bool locked = LockPoiFromRaycast(cam, sky_miss);
        REQUIRE_FALSE(locked);
        REQUIRE_FALSE(cam.has_poi_lock);
        REQUIRE(cam.poi_origin == glm::vec3(2.0f, 1.0f, 3.0f));
        REQUIRE(cam.target_poi == glm::vec3(2.0f, 1.0f, 3.0f));
        REQUIRE(cam.theta_rad == Approx(0.45f));
        REQUIRE(cam.target_theta_rad == Approx(0.45f));
        REQUIRE(cam.phi_rad == Approx(0.72f));
        REQUIRE(cam.target_phi_rad == Approx(0.72f));
        REQUIRE(cam.radius == Approx(18.5f));
        REQUIRE(cam.target_radius == Approx(18.5f));
        REQUIRE(cam.z_cutaway_level == ARCBALL_MAX_CUTAWAY_Z);
    }

    SECTION("Valid raycast hit at building interior (12, 2, 6) locks target_poi and lowers cutaway to 3") {
        RaycastHit building_hit{};
        building_hit.hit = true;
        building_hit.voxel_pos = glm::ivec3(12, 2, 6);
        building_hit.material_id = 5u;

        const bool locked = LockPoiFromRaycast(cam, building_hit);
        REQUIRE(locked);
        REQUIRE(cam.has_poi_lock);
        REQUIRE(cam.target_poi == glm::vec3(12.0f, 2.0f, 6.0f));
        REQUIRE(cam.z_cutaway_level == ARCBALL_INTERIOR_CUTAWAY_Z);

        // Step damping to verify smooth interpolation toward target_poi without NaN
        for (int i = 0; i < 60; ++i) {
            StepArcballDamping(cam, 1.0f / 60.0f);
            REQUIRE_FALSE(std::isnan(cam.poi_origin.x));
            REQUIRE_FALSE(std::isnan(cam.poi_origin.y));
            REQUIRE_FALSE(std::isnan(cam.poi_origin.z));
        }
        REQUIRE(cam.poi_origin.x == Approx(12.0f).margin(1e-3f));
        REQUIRE(cam.poi_origin.y == Approx(2.0f).margin(1e-3f));
        REQUIRE(cam.poi_origin.z == Approx(6.0f).margin(1e-3f));
    }
}

TEST_CASE("STORY-8.1: Spherical Gimbal-Lock Prevention & Shortest-Arc Azimuth Wraparound", "[story-8.1][arcball]") {
    ArcballCameraState cam{};

    SECTION("Extreme vertical drags clamp phi strictly to [5 deg, 85 deg] with zero NaN or flipped up-vectors") {
        // Drag far below horizon
        OrbitArcball(cam, 0.0f, -500.0f);
        REQUIRE(cam.target_phi_rad == Approx(ARCBALL_MIN_PHI_RAD).margin(1e-6f));
        REQUIRE(cam.target_phi_rad >= 0.087266f);

        StepArcballDamping(cam, 1.0f);
        REQUIRE(cam.phi_rad == Approx(ARCBALL_MIN_PHI_RAD).margin(1e-5f));

        glm::vec3 eye_low = ComputeArcballEyePosition(cam);
        REQUIRE_FALSE(std::isnan(eye_low.x));
        REQUIRE_FALSE(std::isnan(eye_low.y));
        REQUIRE_FALSE(std::isnan(eye_low.z));

        glm::vec3 fwd_low = glm::normalize(cam.poi_origin - eye_low);
        glm::vec3 right_low = glm::cross(fwd_low, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 up_low = glm::cross(right_low, fwd_low);
        REQUIRE(glm::length(right_low) > 0.05f);
        REQUIRE(up_low.y > 0.0f);

        // Drag far above zenith
        OrbitArcball(cam, 0.0f, 500.0f);
        REQUIRE(cam.target_phi_rad == Approx(ARCBALL_MAX_PHI_RAD).margin(1e-6f));
        REQUIRE(cam.target_phi_rad <= 1.48353f);

        StepArcballDamping(cam, 1.0f);
        REQUIRE(cam.phi_rad == Approx(ARCBALL_MAX_PHI_RAD).margin(1e-5f));

        glm::vec3 eye_high = ComputeArcballEyePosition(cam);
        REQUIRE_FALSE(std::isnan(eye_high.x));
        REQUIRE_FALSE(std::isnan(eye_high.y));
        REQUIRE_FALSE(std::isnan(eye_high.z));

        glm::vec3 fwd_high = glm::normalize(cam.poi_origin - eye_high);
        glm::vec3 right_high = glm::cross(fwd_high, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 up_high = glm::cross(right_high, fwd_high);
        REQUIRE(glm::length(right_high) > 0.05f);
        REQUIRE(up_high.y > 0.0f);
    }

    SECTION("Azimuth theta wraps smoothly across +/-pi along the shortest angular arc") {
        cam.theta_rad = 3.10f;        // Just below +pi (~3.14159)
        cam.target_theta_rad = -3.10f; // Just above -pi (shortest arc is +0.08318 rad across +pi/-pi boundary)

        // Small damping step should move across the +/-pi seam rather than interpolating backward through 0
        StepArcballDamping(cam, 1.0f / 60.0f);
        // Distance to +/-pi seam must shrink, never jump toward 0
        REQUIRE(std::abs(cam.theta_rad) > 3.05f);
        REQUIRE_FALSE(std::isnan(cam.theta_rad));

        for (int i = 0; i < 60; ++i) {
            StepArcballDamping(cam, 1.0f / 60.0f);
        }
        const float final_diff = std::remainder(cam.target_theta_rad - cam.theta_rad, 2.0f * glm::pi<float>());
        REQUIRE(std::abs(final_diff) < 1e-4f);
    }
}

TEST_CASE("STORY-8.1: Radial Zoom, Roof Cutaway & Expanded 64x64x32 Raycast Bounds", "[story-8.1][raycast]") {
    ArcballCameraState cam{};
    cam.target_radius = 14.0f;
    cam.radius = 14.0f;

    SECTION("Zooming < 12.0f engages roof cutaway (z=3), zooming >= 12.0f restores z=32, and manual cutaway clamps to [1, 32]") {
        // Zoom in past 12.0f
        ZoomArcball(cam, 5.0f); // target_radius = 14.0 - 6.0 = 8.0f
        REQUIRE(cam.target_radius == Approx(8.0f));
        REQUIRE(cam.z_cutaway_level == ARCBALL_INTERIOR_CUTAWAY_Z);

        // Zoom out past 12.0f
        ZoomArcball(cam, -5.0f); // target_radius = 8.0 + 6.0 = 14.0f
        REQUIRE(cam.target_radius == Approx(14.0f));
        REQUIRE(cam.z_cutaway_level == ARCBALL_MAX_CUTAWAY_Z);

        // Manual cutaway adjustment clamps to [1, 32] and sets manual_cutaway_override
        AdjustManualCutaway(cam, -50);
        REQUIRE(cam.manual_cutaway_override);
        REQUIRE(cam.z_cutaway_level == ARCBALL_MIN_CUTAWAY_Z); // 1

        AdjustManualCutaway(cam, 100);
        REQUIRE(cam.z_cutaway_level == ARCBALL_MAX_CUTAWAY_Z); // 32
    }

    SECTION("ScreenToWorldRaycast with z_cutaway_level = 3 slices through roof voxels (y >= 3) to hit interior voxels") {
        GetDynamicVoxelStore().clear();

        // Camera positioned directly above suburban residence at (12.5, 18.0, 6.5) looking straight down at (12.5, 0.0, 6.5)
        const glm::vec3 eye(12.5f, 18.0f, 6.5f);
        const glm::vec3 center(12.5f, 0.0f, 6.5f);
        const glm::mat4 view = glm::lookAt(eye, center, glm::vec3(0.0f, 0.0f, -1.0f));
        const glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f);
        const glm::mat4 inv_vp = glm::inverse(proj * view);
        const glm::vec4 cam_pos(eye, 1.0f);
        const glm::vec4 viewport(800.0f, 800.0f, 1.0f, 0.0f);

        // 1. Without cutaway (z_cutaway_level = 32): hits roof (y >= 4, mat == 6)
        RaycastHit roof_hit = ScreenToWorldRaycast(400, 400, 800, 800, inv_vp, cam_pos, viewport, 192, 32);
        REQUIRE(roof_hit.hit);
        REQUIRE(roof_hit.voxel_pos.x == 12);
        REQUIRE(roof_hit.voxel_pos.z == 6);
        REQUIRE(roof_hit.voxel_pos.y >= 4);
        REQUIRE(roof_hit.material_id == 6u);

        // 2. With z_cutaway_level = 3: slices through roof (y >= 3) to hit interior structure (y == 2)
        RaycastHit cutaway_hit = ScreenToWorldRaycast(400, 400, 800, 800, inv_vp, cam_pos, viewport, 192, 3);
        REQUIRE(cutaway_hit.hit);
        REQUIRE(cutaway_hit.voxel_pos.x == 12);
        REQUIRE(cutaway_hit.voxel_pos.z == 6);
        REQUIRE(cutaway_hit.voxel_pos.y < 3);
        REQUIRE(cutaway_hit.voxel_pos.y == 2);

        // 3. With z_cutaway_level = 1: slices all the way down to ground/interior floor (y == 0, mat == 4)
        RaycastHit floor_hit = ScreenToWorldRaycast(400, 400, 800, 800, inv_vp, cam_pos, viewport, 192, 1);
        REQUIRE(floor_hit.hit);
        REQUIRE(floor_hit.voxel_pos == glm::ivec3(12, 0, 6));
        REQUIRE(floor_hit.material_id == 4u);
    }

    SECTION("Expanded 64x64x32 procedural world and raycast bounds reach outer perimeter") {
        REQUIRE(GetProceduralVoxelMaterial(glm::ivec3(60, 0, 60)) == 4u);
        REQUIRE(GetProceduralVoxelMaterial(glm::ivec3(-60, 0, -60)) == 4u);

        const glm::vec3 eye(58.5f, 10.0f, 58.5f);
        const glm::vec3 center(58.5f, 0.0f, 58.5f);
        const glm::mat4 view = glm::lookAt(eye, center, glm::vec3(0.0f, 0.0f, -1.0f));
        const glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f);
        const glm::mat4 inv_vp = glm::inverse(proj * view);

        RaycastHit outer_hit = ScreenToWorldRaycast(
            400, 400, 800, 800, inv_vp, glm::vec4(eye, 1.0f), glm::vec4(800.0f, 800.0f, 1.0f, 0.0f), 192, 32);
        REQUIRE(outer_hit.hit);
        REQUIRE(outer_hit.voxel_pos == glm::ivec3(58, 0, 58));
        REQUIRE(outer_hit.material_id == 4u);
    }
}

// ============================================================================
// STORY-8.2: Right-Click Voxel Context Evaluator
// ============================================================================

TEST_CASE("STORY-8.2: Right-Click Voxel Context Evaluator", "[story-8.2][context-menu]") {
    SECTION("Sky raycast miss returns closed empty menu") {
        RaycastHit miss{};
        miss.hit = false;
        VoxelContextMenu menu = EvaluateVoxelContext(miss, false);
        REQUIRE_FALSE(menu.is_open);
        REQUIRE(menu.empty());
        REQUIRE(menu.category == VoxelContextCategory::NONE);
    }

    SECTION("Dirt/Lawn voxel (mat == 4) returns earthworks, construction, and zone actions with upfront kcal & focus costs") {
        RaycastHit dirt_hit{};
        dirt_hit.hit = true;
        dirt_hit.voxel_pos = glm::ivec3(10, 0, 10);
        dirt_hit.material_id = 4u;

        VoxelContextMenu menu = EvaluateVoxelContext(dirt_hit, false);
        REQUIRE(menu.is_open);
        REQUIRE(menu.category == VoxelContextCategory::TERRAIN_DIRT);
        REQUIRE(menu.HasAction("[Dig Swale]"));
        REQUIRE(menu.HasAction("[Plant Cover Crop]"));
        REQUIRE(menu.HasAction("[Designate Haul Zone]"));
        REQUIRE(menu.HasAction("[Build Wall: Cob]"));
        REQUIRE(menu.HasAction("[Build Wall: Urbanite]"));
        REQUIRE(menu.HasAction("[Install Door: Insulated Timber]"));
        REQUIRE(menu.HasAction("[Build Hearth]"));
        REQUIRE(menu.HasAction("[Designate Sleep Zone]"));
        REQUIRE(menu.HasAction("[Designate Recreation Zone]"));
        REQUIRE_FALSE(menu.HasAction("[Cancel Designation]"));

        const auto* swale_spec = menu.FindAction("[Dig Swale]");
        REQUIRE(swale_spec != nullptr);
        REQUIRE(swale_spec->kcal_cost > 0.0);
        REQUIRE(swale_spec->focus_cost > 0.0f);

        const auto* haul_spec = menu.FindAction("[Designate Haul Zone]");
        REQUIRE(haul_spec != nullptr);
        REQUIRE(haul_spec->is_zero_labor_zone);
        REQUIRE(haul_spec->kcal_cost == Approx(0.0));
    }

    SECTION("Foundation/Roof voxels (mat == 1, 3, 5, 6) return hardware/roof/solar actions") {
        for (uint32_t mat : {1u, 3u, 5u, 6u}) {
            RaycastHit hit{};
            hit.hit = true;
            hit.voxel_pos = glm::ivec3(12, 4, 6);
            hit.material_id = mat;

            VoxelContextMenu menu = EvaluateVoxelContext(hit, false);
            REQUIRE(menu.is_open);
            REQUIRE(menu.category == VoxelContextCategory::FOUNDATION_ROOF);
            REQUIRE(menu.HasAction("[Install Hardware: Inverter]"));
            REQUIRE(menu.HasAction("[Erect Solar Blueprint]"));
            REQUIRE(menu.HasAction("[Build Roof]"));
            REQUIRE(menu.HasAction("[Build Wall: Cob]"));
        }
    }

    SECTION("Active blueprint at voxel includes [Cancel Designation] option") {
        RaycastHit bp_hit{};
        bp_hit.hit = true;
        bp_hit.voxel_pos = glm::ivec3(10, 0, 10);
        bp_hit.material_id = 4u;

        VoxelContextMenu menu = EvaluateVoxelContext(bp_hit, true);
        REQUIRE(menu.is_open);
        REQUIRE(menu.HasAction("[Cancel Designation]"));
    }
}

// ============================================================================
// STORY-8.3: Volume Cap, Blueprint Cancellation, Loose Item Hauling & SpatialIntentPayload
// ============================================================================

TEST_CASE("STORY-8.3: Designation Volume Cap, Loose Item Hauling & UHAI SpatialIntentPayload", "[story-8.3][designator]") {
    SECTION("ComputeDesignationVolume enforces MAX_DESIGNATION_VOXELS = 256 cap") {
        // 16 x 1 x 16 = 256 voxels -> valid
        DesignationVolume valid_vol = ComputeDesignationVolume(glm::ivec3(0, 0, 0), glm::ivec3(15, 0, 15));
        REQUIRE(valid_vol.total_voxels == 256);
        REQUIRE(valid_vol.is_valid);
        REQUIRE_FALSE(valid_vol.exceeds_cap);
        REQUIRE(valid_vol.voxels.size() == 256);

        // 17 x 1 x 16 = 272 voxels -> exceeds cap, invalid
        DesignationVolume over_vol = ComputeDesignationVolume(glm::ivec3(0, 0, 0), glm::ivec3(16, 0, 15));
        REQUIRE(over_vol.total_voxels == 272);
        REQUIRE(over_vol.exceeds_cap);
        REQUIRE_FALSE(over_vol.is_valid);
        REQUIRE(over_vol.voxels.empty());
    }

    SECTION("CancelBlueprintAtVoxel cleanly destroys uncompleted BlueprintComponent and refunds reserved materials") {
        flecs::world ecs;
        auto zone_ent = ecs.entity().set<StockpileZoneComponent>({8, 8, 10, 10, 0, 2u, 0u});
        auto bp_ent = ecs.entity().set<BlueprintComponent>({
            InfrastructureType::WALL_COB, 12, 1, 12, 100.0, 20.0, 220.0, false, 0.12f, 1u, 0u
        });

        REQUIRE(bp_ent.is_alive());
        const bool cancelled = CancelBlueprintAtVoxel(ecs, glm::ivec3(12, 1, 12));
        REQUIRE(cancelled);
        REQUIRE_FALSE(bp_ent.is_alive());

        const auto& zone = zone_ent.get<StockpileZoneComponent>();
        REQUIRE(zone.stored_cob_lumps == 3u); // 2 initial + 1 refunded
    }

    SECTION("Completing SWALE blueprint spawns LooseItemComponent (MAT_CLAY_COB_LUMP) and auto-hauls to StockpileZoneComponent") {
        flecs::world ecs;
        ecs.entity("Founder").set<SomaticStateComponent>({2500.0, 0.023, 0.080, 1.0f, 1.0f, false, 1.0f, 37.0f, 22.5f});
        auto stockpile = ecs.entity("HaulZone").set<StockpileZoneComponent>({6, 6, 8, 8, 0, 0u, 0u});

        ecs.entity("SwaleBlueprint").set<BlueprintComponent>({
            InfrastructureType::SWALE, 10, 0, 10, 5.0, 0.0, 180.0, false, 0.10f, 0u, 0u
        });

        for (int tick = 0; tick < 5; ++tick) {
            StepConstructionSystem(ecs, 1.0);
        }

        const auto& zone = stockpile.get<StockpileZoneComponent>();
        REQUIRE(zone.stored_cob_lumps == 1u);

        size_t loose_items_count = 0;
        ecs.each([&](const LooseItemComponent& item) {
            loose_items_count++;
            REQUIRE(item.material_id == MAT_CLAY_COB_LUMP);
            REQUIRE(item.is_hauled);
            REQUIRE(item.pos() == glm::ivec3(6, 0, 6));
        });
        REQUIRE(loose_items_count == 1);
    }

    SECTION("BuildSpatialIntentPayload produces 264-byte payload that round-trips through LockFreeSpscRing<SpatialIntentSlot>") {
        REQUIRE(sizeof(SpatialIntentPayload) == 264);
        REQUIRE(sizeof(SpatialIntentSlot) == 320);
        REQUIRE(alignof(SpatialIntentSlot) == 64);

        SpatialIntentPayload pkt = BuildSpatialIntentPayload(
            static_cast<uint16_t>(DesignatorActionId::DIG_SWALE),
            glm::ivec3(10, 0, 12),
            glm::ivec3(4, 1, 4),
            1728750000123456789ULL
        );

        REQUIRE(pkt.quality_flag == QUALITY_SIMULATED); // 0x01
        REQUIRE(pkt.intent_type == static_cast<uint16_t>(IntentType::SPATIAL_AFFORDANCE));
        REQUIRE(pkt.spatial_origin[0] == Approx(10.0f));
        REQUIRE(pkt.spatial_origin[1] == Approx(0.0f));
        REQUIRE(pkt.spatial_origin[2] == Approx(12.0f));
        REQUIRE(pkt.spatial_extents[0] == Approx(4.0f));
        REQUIRE(pkt.spatial_extents[1] == Approx(1.0f));
        REQUIRE(pkt.spatial_extents[2] == Approx(4.0f));
        REQUIRE(pkt.payload_bytes > 0);

        UhaiSpatialIntentChannel channel(UhaiSpatialIntentChannel::ChannelMode::VirtualInMemory, "/test_spatial_intent_s11");
        REQUIRE(channel.IsValid());
        REQUIRE(channel.Push(pkt));

        SpatialIntentPayload popped{};
        REQUIRE(channel.Pop(popped));
        REQUIRE(popped.timestamp_ns == 1728750000123456789ULL);
        REQUIRE(popped.quality_flag == 0x01);
        REQUIRE(std::memcmp(popped.intent_cid, pkt.intent_cid, 32) == 0);
        REQUIRE(std::string(popped.payload_jsonld) == std::string(pkt.payload_jsonld));
    }
}
