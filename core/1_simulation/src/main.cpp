#include <flecs.h>
#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <string_view>
#include <cstring>
#include <chrono>
#include <algorithm>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <webgpu/webgpu_cpp.h>
#endif

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static float g_cam_offset_x = 0.0f;
static float g_cam_offset_z = 0.0f;
static bool g_is_dragging = false;
static bool g_is_orbiting_arcball = false;
static bool g_is_painting_volume = false;
static int g_last_mouse_x = 0;
static int g_last_mouse_y = 0;
static bool g_shift_pressed = false;
static bool g_running = true;
static bool g_is_headless_ci = false;
static uint32_t g_overlay_mode = 0; // 0 = Normal, 1 = F1 Moisture, 2 = F2 Solar, 3 = F3 Thermal, 4 = F4 Stress
static uint16_t g_active_designator_action_id = 1; // Default: [1:Backyard Swale] (matches shell_minimal.html)
static uint16_t& g_active_designator_action = g_active_designator_action_id;

#include "shaders/raymarch_wgsl.hpp"
#include "uhai/uhai_ring_buffer.hpp"
#include "core/stochastic_boundary.hpp"
#include "core/fabrication.hpp"
#include "core/homeostasis.hpp"
#include "core/chill_loop.hpp"
#include "core/raycaster.hpp"
#include "core/camera_uniforms.hpp"
#include "core/arcball_camera.hpp"
#include "core/spatial_designator.hpp"
#include "core/time_warp_controller.hpp"
#include "core/migration_assimilation.hpp"
#include "core/voxel_pathfinder.hpp"
#include "core/bpmn_scheduler.hpp"

// Global Arcball Camera State (STORY-8.1)
static oasis::graphics::ArcballCameraState g_arcball_camera{};

namespace oasis {
#ifndef OASIS_PSYCHOLOGY_DEFINED
#define OASIS_PSYCHOLOGY_DEFINED
namespace psychology {

struct PersonalityFacets {
    float openness{0.8f};
    float conscientiousness{0.9f};
    float extraversion{0.6f};
    float agreeableness{0.7f};
    float neuroticism{0.3f};
};

// Memory Event Type IDs
enum class MemoryEventType : uint16_t {
    UNKNOWN = 0,
    GENESIS_AWAKENING = 1,
    MUNICIPAL_CODE_RAID = 2,
    SIPHON_POWER_GRID = 3,
    FIRST_KILOWATT_GENERATED = 4,
    WATER_MAIN_TAPPED = 5,
    SOLAR_INVERTER_REPAIR = 6,
    TOOL_DROPPED = 7,
    COMMUNAL_FEAST = 8,
    STRANGE_MOOD_MASTERWORK = 9,
    FREEZING_WINTER_NIGHT = 10,
    HOA_VIOLATION_NOTICE = 11
};

// Memory Bitfield Flags
namespace MemoryFlags {
    constexpr uint8_t NONE             = 0x00;
    constexpr uint8_t PERMANENT_MEMORY = 0x01; // salience >= 80 root trauma / achievement
    constexpr uint8_t TRAUMA           = 0x02; // Negative valence severe event
    constexpr uint8_t EUPHORIA         = 0x04; // Positive valence major breakthrough
}

#pragma pack(push, 8)
struct alignas(8) EpisodicMemoryNode {
    uint64_t timestamp_tick{0};     // Tick when event occurred (8 bytes)
    uint16_t event_type{0};          // MemoryEventType enum (2 bytes)
    int16_t  emotional_valence{0};   // Emotional valence [-100..+100] (2 bytes)
    uint8_t  salience{0};            // Salience score [0..100] (1 byte)
    uint8_t  flags{0};               // Bitfield flags (1 byte)
    char     description[8]{};       // Short label (8 bytes)
    uint16_t reserved{0};            // Alignment padding (2 bytes)
};
#pragma pack(pop)
static_assert(sizeof(EpisodicMemoryNode) == 24, "EpisodicMemoryNode must be exactly 24 bytes packed");

struct EpisodicMemoryRing {
    static constexpr size_t CAPACITY = 32;

    EpisodicMemoryNode slots[CAPACITY]{};
    uint32_t head{0};
    uint32_t count{0};
    uint32_t total_recorded{0};
    uint32_t permanent_count{0};

    // Push with DF trauma-protection eviction
    uint32_t Push(const EpisodicMemoryNode& input_node) {
        EpisodicMemoryNode node = input_node;
        total_recorded++;

        // When node.salience >= 80, sets PERMANENT_MEMORY flag
        if (node.salience >= 80) {
            node.flags |= MemoryFlags::PERMANENT_MEMORY;
        }

        uint32_t target_slot = head;

        if (count < CAPACITY) {
            target_slot = head;
            slots[target_slot] = node;
            head = (head + 1) % CAPACITY;
            count++;
            if ((node.flags & MemoryFlags::PERMANENT_MEMORY) != 0) {
                permanent_count++;
            }
            return target_slot;
        }

        // Buffer full (count == CAPACITY): DF trauma protection!
        // Find lowest-salience slot that is NOT flagged PERMANENT_MEMORY
        int32_t evict_idx = -1;
        uint32_t min_salience = 0xFFFFFFFF;

        for (size_t i = 0; i < CAPACITY; ++i) {
            if ((slots[i].flags & MemoryFlags::PERMANENT_MEMORY) == 0 && slots[i].salience < min_salience) {
                min_salience = slots[i].salience;
                evict_idx = static_cast<int32_t>(i);
            }
        }

        if (evict_idx != -1) {
            target_slot = static_cast<uint32_t>(evict_idx);
            slots[target_slot] = node;
            if ((node.flags & MemoryFlags::PERMANENT_MEMORY) != 0) {
                permanent_count++;
            }
            return target_slot;
        }

        // If all are permanent, overwrite head
        target_slot = head;
        const bool prev_was_permanent = (slots[head].flags & MemoryFlags::PERMANENT_MEMORY) != 0;
        slots[head] = node;
        head = (head + 1) % CAPACITY;
        const bool new_is_permanent = (node.flags & MemoryFlags::PERMANENT_MEMORY) != 0;
        if (!prev_was_permanent && new_is_permanent) {
            permanent_count++;
        } else if (prev_was_permanent && !new_is_permanent) {
            if (permanent_count > 0) permanent_count--;
        }
        return target_slot;
    }
};

struct PsychologyComponent {
    PersonalityFacets facets{0.8f, 0.9f, 0.6f, 0.7f, 0.3f};
    float stress{0.25f};
    float mood{0.50f};
    float focus{0.75f};
    EpisodicMemoryRing memory_ring{};
};

} // namespace psychology
#endif // OASIS_PSYCHOLOGY_DEFINED

namespace graphics {

#ifndef OASIS_CAMERA_UNIFORMS_DEFINED
#define OASIS_CAMERA_UNIFORMS_DEFINED
#pragma pack(push, 16)
struct alignas(16) CameraUniforms {
    glm::mat4 view_matrix{1.0f};       // 64 bytes (offset 0)
    glm::mat4 proj_matrix{1.0f};       // 64 bytes (offset 64)
    glm::mat4 inv_view_proj{1.0f};     // 64 bytes (offset 128)
    glm::vec4 camera_pos{0.0001f, 2.0001f, -4.9999f, 0.0f}; // 16 bytes (offset 192), xyz = pos, w = mode
    glm::vec4 camera_target{0.0f, 0.0f, 0.0f, 1.0f};         // 16 bytes (offset 208), xyz = target, w = zoom
    glm::vec4 viewport_params{1024.0f, 720.0f, 1024.0f / 720.0f, 0.0f}; // 16 bytes (offset 224), x=w, y=h, z=aspect, w=transition_t
    glm::vec4 cursor_highlight{0.0f};  // 16 bytes (offset 240), xyz = cursor voxel pos, w = active flag (>0.5)
    glm::vec4 avatar_pos{0.0f, 1.0f, 0.0f, 0.0f}; // 16 bytes (offset 256), xyz = Founder 3D pos, w = animation time (seconds)
    glm::vec4 avatar_params{0.0f};     // 16 bytes (offset 272), x = action_state, y = stress, z = focus, w = active flag (>0.5)
};
#pragma pack(pop)
#endif
static_assert(sizeof(CameraUniforms) == 288, "CameraUniforms must be exactly 288 bytes.");
static_assert(alignof(CameraUniforms) == 16, "CameraUniforms must be 16-byte aligned.");

struct RenderState {
#ifdef __EMSCRIPTEN__
    wgpu::Instance instance{nullptr};
    wgpu::Adapter adapter{nullptr};
    wgpu::Device device{nullptr};
    wgpu::Queue queue{nullptr};
    wgpu::Surface surface{nullptr};
    wgpu::RenderPipeline pipeline{nullptr};
    wgpu::TextureFormat format{wgpu::TextureFormat::Undefined};
    wgpu::Buffer camera_buffer{nullptr};
    wgpu::Buffer voxel_storage_buffer{nullptr};
    wgpu::BindGroupLayout bind_group_layout{nullptr};
    wgpu::BindGroup camera_bind_group{nullptr};
#else
    void* device{nullptr};
    void* queue{nullptr};
    void* surface{nullptr};
    void* pipeline{nullptr};
    void* camera_buffer{nullptr};
    void* voxel_storage_buffer{nullptr};
    void* bind_group_layout{nullptr};
    void* camera_bind_group{nullptr};
#endif
    CameraUniforms camera_uniforms{};
    oasis::graphics::GpuDynamicVoxelBuffer voxel_data{};
    oasis::graphics::ArcballCameraState arcball{};
    uint32_t active_camera_mode{0}; // 0 = 3rd-Person Perspective, 1 = Cadastral Isometric
    uint32_t target_camera_mode{0};
    float transition_elapsed_sec{0.0f};
    bool is_transitioning{false};
    glm::vec3 camera_offset{0.0f, 0.0f, 0.0f}; // Mac-friendly panning offset (ground plane locked)
    uint32_t width{1024};
    uint32_t height{720};
    bool is_initialized{false};
    uint64_t frame_count{0};
    float clear_color[4]{0.05f, 0.05f, 0.1f, 1.0f};

    bool is_ready() const {
#ifdef __EMSCRIPTEN__
        return is_initialized && device != nullptr && pipeline != nullptr && surface != nullptr && camera_bind_group != nullptr && voxel_storage_buffer != nullptr;
#else
        return is_initialized;
#endif
    }
};

inline void ApplyCameraPan(RenderState& state, float delta_x, float delta_y, bool is_wheel) {
    oasis::graphics::ApplyCameraPan(state.camera_offset, state.active_camera_mode, state.target_camera_mode,
                                   state.transition_elapsed_sec, state.is_transitioning, delta_x, delta_y, is_wheel);
    g_cam_offset_x = state.camera_offset.x;
    g_cam_offset_z = state.camera_offset.z;

    if (state.active_camera_mode == 0 && state.arcball.has_poi_lock) {
        const float scale = is_wheel ? 0.035f : 0.030f;
        oasis::graphics::PanArcballPoi(state.arcball, -delta_x * scale, delta_y * scale);
    } else {
        const glm::vec3 pan_delta = oasis::graphics::ComputeCameraPanDelta(
            state.active_camera_mode, state.target_camera_mode,
            state.transition_elapsed_sec, state.is_transitioning,
            delta_x, delta_y, is_wheel
        );
        state.arcball.target_poi.x = std::clamp(state.arcball.target_poi.x + pan_delta.x, -45.0f, 45.0f);
        state.arcball.target_poi.z = std::clamp(state.arcball.target_poi.z + pan_delta.z, -45.0f, 50.0f);
    }
    g_arcball_camera.target_poi = state.arcball.target_poi;
}

inline void PanCameraByKey(RenderState& state, float right_units, float fwd_units) {
    oasis::graphics::PanArcballPoi(state.arcball, right_units, fwd_units);
    g_arcball_camera.target_poi = state.arcball.target_poi;
    state.camera_offset.x = std::clamp(state.camera_offset.x + right_units, -45.0f, 45.0f);
    state.camera_offset.z = std::clamp(state.camera_offset.z + fwd_units, -45.0f, 50.0f);
    g_cam_offset_x = state.camera_offset.x;
    g_cam_offset_z = state.camera_offset.z;
}

inline void UpdateCameraMatrices(RenderState& state, float dt_sec) {
    if (state.is_transitioning) {
        state.transition_elapsed_sec += dt_sec;
        if (state.transition_elapsed_sec >= 0.400f) {
            state.transition_elapsed_sec = 0.400f;
            state.is_transitioning = false;
            state.active_camera_mode = state.target_camera_mode;
        }
    }

    oasis::graphics::StepArcballDamping(state.arcball, dt_sec);
    oasis::graphics::StepArcballDamping(g_arcball_camera, dt_sec);

    float t = 0.0f;
    if (state.is_transitioning) {
        float u = std::clamp(state.transition_elapsed_sec / 0.400f, 0.0f, 1.0f);
        float hermite_t = 3.0f * u * u - 2.0f * u * u * u;
        if (state.target_camera_mode == 1) {
            t = hermite_t;
        } else {
            t = 1.0f - hermite_t;
        }
    } else {
        t = (state.active_camera_mode == 1) ? 1.0f : 0.0f;
    }

    const float aspect = (state.height > 0) ? (static_cast<float>(state.width) / static_cast<float>(state.height)) : (1024.0f / 720.0f);

    // Sync state.camera_offset with legacy globals if modified externally
    if (g_cam_offset_x != state.camera_offset.x || g_cam_offset_z != state.camera_offset.z) {
        state.camera_offset.x = g_cam_offset_x;
        state.camera_offset.z = g_cam_offset_z;
    }
    const glm::vec3 offset = state.camera_offset;

    // 1. Mode 0: 3rd-Person Follow / Dynamic POI Spherical Arcball Perspective Camera
    glm::vec3 eye_persp = offset + glm::vec3(0.0001f, 2.0001f, -4.9999f);
    glm::vec3 target_persp = offset + glm::vec3(0.0f, 0.0f, 0.0f);
    if (state.arcball.has_poi_lock) {
        eye_persp = oasis::graphics::ComputeArcballEyePosition(state.arcball);
        target_persp = state.arcball.poi_origin;
    }
    const glm::vec3 up_persp(0.0f, 1.0f, 0.0f);
    const glm::mat4 view_persp = glm::lookAt(eye_persp, target_persp, up_persp);
    const glm::mat4 proj_persp = glm::perspective(glm::radians(53.13f), aspect, 0.1f, 100.0f);
    const glm::mat4 inv_vp_persp = glm::inverse(proj_persp * view_persp);

    // 2. Mode 1: Cadastral Axonometric / Isometric Projection Camera
    const float ortho_size = state.arcball.radius;
    const glm::mat4 proj_iso = glm::ortho(-ortho_size * aspect, ortho_size * aspect, -ortho_size, ortho_size, -50.0f, 50.0f);
    const glm::vec3 iso_center = state.arcball.has_poi_lock
        ? glm::vec3(state.arcball.poi_origin.x, 0.0f, state.arcball.poi_origin.z)
        : offset;
    const glm::vec3 eye_iso = iso_center + glm::vec3(-16.0001f, 20.0001f, -16.0001f);
    const glm::vec3 target_iso = iso_center + glm::vec3(0.0001f, 0.0001f, 0.0001f);
    const glm::vec3 up_iso(0.0f, 1.0f, 0.0f);
    const glm::mat4 view_iso = glm::lookAt(eye_iso, target_iso, up_iso);
    const glm::mat4 inv_vp_iso = glm::inverse(proj_iso * view_iso);

    // Interpolate camera uniforms across transition parameter t
    const float cutaway_z = static_cast<float>(state.arcball.z_cutaway_level);
    const float encoded_mode_w = static_cast<float>(g_overlay_mode * 10u) + (state.arcball.has_poi_lock ? 2.0f : t);
    state.camera_uniforms.view_matrix = (1.0f - t) * view_persp + t * view_iso;
    state.camera_uniforms.proj_matrix = (1.0f - t) * proj_persp + t * proj_iso;
    state.camera_uniforms.inv_view_proj = (1.0f - t) * inv_vp_persp + t * inv_vp_iso;
    state.camera_uniforms.camera_pos = glm::vec4(glm::mix(eye_persp, eye_iso, t), encoded_mode_w);
    state.camera_uniforms.camera_target = glm::mix(glm::vec4(target_persp, cutaway_z), glm::vec4(target_iso, cutaway_z), t);
    state.camera_uniforms.viewport_params = glm::vec4(static_cast<float>(state.width), static_cast<float>(state.height), aspect, t);
    // cursor_highlight is preserved across frame transitions
}

} // namespace graphics
} // namespace oasis

// Heap-allocated Flecs world to prevent stack use-after-free before async callbacks fire
static flecs::world* g_ecs = nullptr;

// Monotonic simulation frame counter
static uint64_t g_sim_tick = 0;

// Global Time Warp State (STORY-10.1 & STORY-10.2)
static oasis::time::TimeWarpState g_time_warp{};

// Global Spatial Designator State (STORY-8.2 & STORY-8.3)
static oasis::designator::VoxelContextMenu g_active_context_menu{};
static oasis::graphics::RaycastHit g_last_context_hit{};
static glm::ivec3 g_paint_start_voxel{0};
static glm::ivec3 g_paint_end_voxel{0};
static glm::ivec3 g_paint_start_clicked_voxel{0};
static oasis::uhai::SpatialIntentPayload g_last_spatial_intent{};

// Global UHAI Telemetry Channel (Producer mode)
static oasis::uhai::UhaiTelemetryChannel g_telemetry_channel(oasis::uhai::UhaiTelemetryChannel::ChannelMode::Producer);

// Global UHAI Flow Channel (Consumer mode) - Dual Sprint 9 Temporal Flow
static oasis::uhai::UhaiFlowChannel g_flow_channel(oasis::uhai::UhaiFlowChannel::ChannelMode::Consumer);

// Global UHAI Migration Wave & Trust Ring Promotion Channels (Consumer mode) - Dual Sprint 13 (EPIC-11)
static oasis::uhai::UhaiMigrationWaveChannel g_migration_wave_channel(oasis::uhai::UhaiMigrationWaveChannel::ChannelMode::Consumer);
static oasis::uhai::UhaiTrustPromotionChannel g_trust_promotion_channel(oasis::uhai::UhaiTrustPromotionChannel::ChannelMode::Consumer);
static oasis::migration::MigrationStagingQueue g_migration_staging{};
static uint32_t g_manual_wave_seq = 100u;

// Sprint 13.5 & 13.7: Lot 402 Homestead Quest Progress, Layer 7 Reactive Commentary & Designator Guard State
static bool g_quest_has_garden = false;
static bool g_quest_has_roof_solar_or_cistern = false;
static bool g_quest_has_l7_grace = false;
static uint64_t g_last_l7_grace_tick = 0ULL;
static uint32_t g_l7_remark_seq = 0u;
static std::string g_latest_l7_neighbor_quote =
    "Sidewalk quiet — Layer 7 neighbors strolling along the front cul-de-sac.";
static std::string& g_last_neighbor_remark = g_latest_l7_neighbor_quote;
static std::string g_last_designator_warning{};
static uint64_t g_designator_warning_until_tick = 0ULL;

// Sprint 14: Asynchronous Node Proxies (Pillar 1) & Multi-Day BPMN Temporal Scheduler (Pillar 3)
static oasis::orchestrator::CollectiveNetworkState g_collective_net = oasis::orchestrator::CreateDefaultCollectiveNetwork();
static oasis::orchestrator::TemporalScheduleState g_temporal_schedule = []() {
    auto sched = oasis::orchestrator::CreateDefaultTemporalSchedule();
    sched.auto_dispatch_enabled = false;
    return sched;
}();

static void SetLayer7NeighborMilestoneRemark(
    flecs::world& ecs,
    oasis::migration::Layer7CommentaryEvent ev
) {
    g_last_l7_grace_tick = g_sim_tick;
    g_quest_has_l7_grace = true;
    g_l7_remark_seq++;
    g_last_neighbor_remark = oasis::migration::GetLayer7NeighborMilestoneRemark(ev);

    auto founder = ecs.entity("Founder");
    if (founder.is_alive() && founder.has<oasis::psychology::PsychologyComponent>()) {
        auto& psy = founder.get_mut<oasis::psychology::PsychologyComponent>();
        psy.stress = std::max(0.0f, psy.stress - 0.01f);
    }
    spdlog::info("[Oasis Layer 7] {}", g_last_neighbor_remark);
}

static void TriggerLayer7MinimumGrace(flecs::world& ecs, bool force_manual) {
    if (!force_manual && g_last_l7_grace_tick > 0ULL && (g_sim_tick < g_last_l7_grace_tick + 600ULL)) {
        return;
    }
    g_last_l7_grace_tick = g_sim_tick;
    g_quest_has_l7_grace = true;
    g_l7_remark_seq++;

    if (g_quest_has_roof_solar_or_cistern && (!g_quest_has_garden || (g_l7_remark_seq % 2u == 1u))) {
        g_last_neighbor_remark = oasis::migration::GetLayer7NeighborMilestoneRemark(
            oasis::migration::Layer7CommentaryEvent::ROOF_SOLAR_PANEL
        );
    } else if (g_quest_has_garden) {
        g_last_neighbor_remark =
            "Neighbor (L7) waves briefly: 'Backyard garden's coming along nicely! Gotta run to work.'";
    } else {
        g_last_neighbor_remark = oasis::migration::GetLayer7NeighborMilestoneRemark(
            oasis::migration::Layer7CommentaryEvent::AMBIENT_WAVE
        );
    }

    auto founder = ecs.entity("Founder");
    if (founder.is_alive() && founder.has<oasis::psychology::PsychologyComponent>()) {
        auto& psy = founder.get_mut<oasis::psychology::PsychologyComponent>();
        psy.stress = std::max(0.0f, psy.stress - 0.01f);
    }
    spdlog::info("[Oasis Layer 7] {}", g_last_neighbor_remark);
}

// Write telemetry frame sample strictly once per 60 Hz render frame
static void TickTelemetry() {
    auto now = std::chrono::steady_clock::now();
    uint64_t current_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    static auto s_last_frame_time = now;
    auto frame_delta_us = std::chrono::duration_cast<std::chrono::microseconds>(now - s_last_frame_time).count();
    s_last_frame_time = now;
    float calculated_fps = (frame_delta_us > 0) ? (1000000.0f / static_cast<float>(frame_delta_us)) : 60.0f;
    if (calculated_fps > 300.0f || calculated_fps < 1.0f) {
        calculated_fps = 60.0f;
    }

    float founder_stress = 0.25f;
    uint32_t memory_slot_count = 0;
    if (g_ecs) {
        auto founder = g_ecs->entity("Founder");
        if (founder.has<oasis::psychology::PsychologyComponent>()) {
            const auto& psych = founder.get<oasis::psychology::PsychologyComponent>();
            founder_stress = psych.stress;
            memory_slot_count = psych.memory_ring.count;
        }
    }

    oasis::uhai::TelemetrySample sample{};
    sample.frame_count = g_sim_tick;
    sample.timestamp_ns = current_time_ns;
    sample.fps = calculated_fps;
    sample.founder_stress = founder_stress;
    sample.memory_slot_count = memory_slot_count;
    sample.biome_id = 1; // 1 = Suburban Sprawl ("Lot 402 & Cul-de-sac")
    sample.quality_flag = oasis::uhai::QUALITY_SIMULATED; // 0x01

    g_telemetry_channel.Push(sample);
}

static oasis::fabrication::InfrastructureType g_active_blueprint_type = oasis::fabrication::InfrastructureType::SWALE;
static bool g_founder_is_carrying = false;
static oasis::pathfinding::VoxelPath g_founder_voxel_path{};
static std::string g_blocked_blueprint_status_note{};

static void DispatchFounderPathTo(flecs::entity founder, glm::ivec3 target_voxel) {
    if (!founder.is_alive() || !founder.has<oasis::fabrication::SpatialPositionComponent>()) {
        return;
    }
    const auto& pos = founder.get<oasis::fabrication::SpatialPositionComponent>().pos;
    const glm::ivec3 start_voxel(
        static_cast<int>(std::round(pos.x)),
        std::clamp(static_cast<int>(std::round(pos.y)), 1, 6),
        static_cast<int>(std::round(pos.z))
    );
    g_founder_voxel_path = oasis::pathfinding::ComputeVoxelPath(start_voxel, target_voxel);
    founder.set<oasis::pathfinding::VoxelPathComponent>(g_founder_voxel_path);
}

static void DispatchFounderPathTo(
    flecs::world& ecs,
    glm::ivec3 target_voxel,
    oasis::fabrication::FounderActionState new_state,
    flecs::entity target_bp = flecs::entity::null()
) {
    auto founder = ecs.entity("Founder");
    if (!founder.is_alive() || !founder.has<oasis::fabrication::FounderCommandComponent>()) {
        return;
    }
    auto& cmd = founder.get_mut<oasis::fabrication::FounderCommandComponent>();
    cmd.state = new_state;
    cmd.target_voxel = target_voxel;
    if (target_bp.is_alive()) {
        cmd.target_blueprint = target_bp;
    }
    DispatchFounderPathTo(founder, target_voxel);
}

static bool TriggerHarvestGardenAction(flecs::world& ecs, const glm::ivec3* preferred_voxel = nullptr) {
    auto founder = ecs.entity("Founder");
    if (!founder.is_alive() ||
        !founder.has<oasis::fabrication::FounderCommandComponent>() ||
        !founder.has<oasis::fabrication::SpatialPositionComponent>()) {
        return false;
    }
    const auto& fpos = founder.get<oasis::fabrication::SpatialPositionComponent>().pos;

    flecs::entity best_crop{};
    glm::ivec3 best_crop_pos{20, 1, 5};
    float best_score = 1e30f;

    ecs.each([&](flecs::entity ce, oasis::fabrication::GardenCropComponent& crop) {
        if (preferred_voxel != nullptr &&
            crop.x == preferred_voxel->x && crop.z == preferred_voxel->z) {
            best_crop = ce;
            best_crop_pos = crop.pos();
            best_score = -1000.0f;
            return;
        }
        if (crop.is_mature) {
            const float d2 = glm::dot(glm::vec3(crop.pos()) - fpos, glm::vec3(crop.pos()) - fpos);
            if (d2 < best_score) {
                best_score = d2;
                best_crop = ce;
                best_crop_pos = crop.pos();
            }
        }
    });

    if (!best_crop.is_alive()) {
        // Fallback: if player manually clicked Harvest Garden before crop finished ripening, pick closest garden bed
        ecs.each([&](flecs::entity ce, oasis::fabrication::GardenCropComponent& crop) {
            const float d2 = glm::dot(glm::vec3(crop.pos()) - fpos, glm::vec3(crop.pos()) - fpos);
            if (d2 < best_score) {
                best_score = d2;
                best_crop = ce;
                best_crop_pos = crop.pos();
            }
        });
    }

    if (!best_crop.is_alive()) {
        return false;
    }

    auto& cmd = founder.get_mut<oasis::fabrication::FounderCommandComponent>();
    g_founder_is_carrying = false;
    cmd.state = oasis::fabrication::FounderActionState::HARVESTING_CROP;
    cmd.target_voxel = best_crop_pos;
    DispatchFounderPathTo(founder, best_crop_pos);
    spdlog::info("[Oasis Garden] Founder dispatched to harvest backyard garden crop at ({},{},{})",
                 best_crop_pos.x, best_crop_pos.y, best_crop_pos.z);
    return true;
}

static bool TriggerCookKitchenAction(flecs::world& ecs) {
    auto founder = ecs.entity("Founder");
    auto camp = ecs.entity("GenesisCamp");
    if (!founder.is_alive() || !founder.has<oasis::fabrication::FounderCommandComponent>() ||
        !camp.is_alive() || !camp.has<oasis::chill::GenesisCampStockComponent>()) {
        return false;
    }

    const auto& stock = camp.get<oasis::chill::GenesisCampStockComponent>();
    if (stock.raw_produce_items == 0u) {
        // If no raw produce in stock yet, harvest a backyard garden bed first (which transitions into COOKING_MEAL!)
        if (TriggerHarvestGardenAction(ecs, nullptr)) {
            return true;
        }
        return false;
    }

    auto& cmd = founder.get_mut<oasis::fabrication::FounderCommandComponent>();
    g_founder_is_carrying = false;
    cmd.state = oasis::fabrication::FounderActionState::COOKING_MEAL;
    cmd.target_voxel = stock.ration_crate_pos;
    DispatchFounderPathTo(founder, stock.ration_crate_pos);
    spdlog::info("[Oasis Kitchen] Founder dispatched to Kitchen Counter (11,1,4) to cook Homestead Meal (Scenario Gamma)");
    return true;
}

static oasis::fabrication::InfrastructureType MapActionIdToBlueprintType(
    uint16_t action_id,
    oasis::fabrication::InfrastructureType fallback = oasis::fabrication::InfrastructureType::WALL_COB
) {
    using oasis::designator::DesignatorActionId;
    using oasis::fabrication::InfrastructureType;
    switch (action_id) {
        case static_cast<uint16_t>(DesignatorActionId::DIG_SWALE):
            return InfrastructureType::SWALE;
        case static_cast<uint16_t>(DesignatorActionId::PLANT_COVER_CROP):
            return InfrastructureType::COVER_CROP;
        case static_cast<uint16_t>(DesignatorActionId::DESIGNATE_HAUL_ZONE):
            return InfrastructureType::HAUL_ZONE;
        case static_cast<uint16_t>(DesignatorActionId::BUILD_WALL_COB):
            return InfrastructureType::WALL_COB;
        case static_cast<uint16_t>(DesignatorActionId::BUILD_WALL_URBANITE):
            return InfrastructureType::WALL_URBANITE;
        case static_cast<uint16_t>(DesignatorActionId::INSTALL_DOOR_TIMBER):
            return InfrastructureType::DOOR_TIMBER;
        case static_cast<uint16_t>(DesignatorActionId::BUILD_HEARTH):
            return InfrastructureType::ROCKET_MASS_HEATER;
        case static_cast<uint16_t>(DesignatorActionId::DESIGNATE_SLEEP_ZONE):
            return InfrastructureType::SLEEP_ZONE;
        case static_cast<uint16_t>(DesignatorActionId::DESIGNATE_RECREATION_ZONE):
            return InfrastructureType::RECREATION_ZONE;
        case static_cast<uint16_t>(DesignatorActionId::INSTALL_INVERTER):
            return InfrastructureType::RAINWATER_CATCHMENT;
        case static_cast<uint16_t>(DesignatorActionId::ERECT_SOLAR_BLUEPRINT):
            return InfrastructureType::SOLAR_PANEL;
        case static_cast<uint16_t>(DesignatorActionId::BUILD_ROOF):
            return InfrastructureType::ROOF_TIMBER;
        case static_cast<uint16_t>(DesignatorActionId::HARVEST_GARDEN):
            return InfrastructureType::COVER_CROP;
        case static_cast<uint16_t>(DesignatorActionId::COOK_MEAL_KITCHEN):
        case static_cast<uint16_t>(DesignatorActionId::CANCEL_DESIGNATION):
            return InfrastructureType::NONE;
        default:
            return fallback;
    }
}

static bool IsZeroLaborActionId(uint16_t action_id) {
    using oasis::designator::DesignatorActionId;
    if (action_id == 13 || action_id == 14) return true;
    const auto id = static_cast<DesignatorActionId>(action_id);
    return id == DesignatorActionId::DESIGNATE_HAUL_ZONE ||
           id == DesignatorActionId::DESIGNATE_SLEEP_ZONE ||
           id == DesignatorActionId::DESIGNATE_RECREATION_ZONE ||
           id == DesignatorActionId::CANCEL_DESIGNATION;
}

static const char* FormatBlueprintTypeName(oasis::fabrication::InfrastructureType type) {
    using oasis::fabrication::InfrastructureType;
    switch (type) {
        case InfrastructureType::SWALE:               return "Swale";
        case InfrastructureType::COVER_CROP:          return "Cover Crop";
        case InfrastructureType::HAUL_ZONE:           return "Haul Zone";
        case InfrastructureType::WALL_COB:            return "Wall: Cob";
        case InfrastructureType::WALL_URBANITE:       return "Wall: Urbanite";
        case InfrastructureType::DOOR_TIMBER:         return "Door: Timber";
        case InfrastructureType::ROCKET_MASS_HEATER:  return "Hearth";
        case InfrastructureType::SLEEP_ZONE:          return "Sleep Zone";
        case InfrastructureType::RECREATION_ZONE:     return "Recreation Zone";
        case InfrastructureType::RAINWATER_CATCHMENT: return "Cistern / Inverter";
        case InfrastructureType::SOLAR_PANEL:         return "Solar Panel";
        case InfrastructureType::ROOF_TIMBER:         return "Roof: Timber";
        default:                                      return "Structure";
    }
}

static bool PlaceDesignationAtVoxel(
    flecs::world& ecs,
    glm::ivec3 clicked_voxel,
    glm::ivec3 placement_voxel,
    uint16_t action_id
) {
    auto founder = ecs.entity("Founder");

    // 1. Cancel Designation (action_id == 99)
    if (action_id == 99) {
        oasis::designator::CancelBlueprintAtVoxel(ecs, clicked_voxel);
        oasis::designator::CancelBlueprintAtVoxel(ecs, placement_voxel);
        if (clicked_voxel.y >= 1) {
            oasis::graphics::SetDynamicVoxel(clicked_voxel.x, clicked_voxel.y, clicked_voxel.z, 0u);
        }
        if (placement_voxel.y >= 1) {
            const uint8_t dyn_p = oasis::graphics::GetDynamicVoxel(placement_voxel.x, placement_voxel.y, placement_voxel.z);
            if (dyn_p == oasis::designator::MAT_BLUEPRINT_GHOST || dyn_p == 33u) {
                oasis::graphics::SetDynamicVoxel(placement_voxel.x, placement_voxel.y, placement_voxel.z, 0u);
            }
        }
        if (founder.is_alive() && founder.has<oasis::chill::EnclosureCacheComponent>()) {
            oasis::chill::MarkEnclosureDirtyOnVoxelChange(founder.get_mut<oasis::chill::EnclosureCacheComponent>());
        }
        spdlog::info("[Oasis Designator] Cancelled designation / cleared voxel at ({},{},{})",
                     clicked_voxel.x, clicked_voxel.y, clicked_voxel.z);
        return true;
    }

    // 1b. Harvest Backyard Garden (action_id == 13) & Scenario Gamma Cook Meal at Kitchen (action_id == 14)
    if (action_id == 13) {
        return TriggerHarvestGardenAction(ecs, &clicked_voxel);
    }
    if (action_id == 14) {
        return TriggerCookKitchenAction(ecs);
    }

    // 2. Zero-Labor Zones (action_id == 3, 8, 9 — Haul, Sleep, Recreation): complete instantly!
    if (action_id == 3 || action_id == 8 || action_id == 9) {
        const glm::ivec3 pos = (clicked_voxel.y == 0)
            ? glm::ivec3(clicked_voxel.x, 1, clicked_voxel.z)
            : clicked_voxel;
        const uint8_t mat_id = (action_id == 3) ? 20u : ((action_id == 8) ? 21u : 22u);
        oasis::graphics::SetDynamicVoxel(pos.x, pos.y, pos.z, mat_id);

        if (action_id == 3) {
            ecs.entity().set<oasis::fabrication::StockpileZoneComponent>({
                pos.x, pos.z, pos.x, pos.z, pos.y, 0u, 0u
            });
            oasis::fabrication::HaulLooseItemsToStockpile(ecs);
        }
        if (founder.is_alive() && founder.has<oasis::chill::EnclosureCacheComponent>()) {
            oasis::chill::MarkEnclosureDirtyOnVoxelChange(founder.get_mut<oasis::chill::EnclosureCacheComponent>());
        }
        spdlog::info("[Oasis Designator] Instant zero-labor zone (mat={}) placed at ({},{},{})",
                     static_cast<unsigned>(mat_id), pos.x, pos.y, pos.z);
        return true;
    }

    // 3. Queued Construction & Earthworks (action_id == 1, 2, 4, 5, 6, 7, 10, 11, 12)
    const uint16_t effective_action_id = (action_id == 0)
        ? ((clicked_voxel.y >= 4) ? 11u : 4u)
        : action_id;
    const auto spec = oasis::designator::GetDesignatorToolSpecById(effective_action_id);

    if (founder.is_alive() && founder.has<oasis::chill::BurnoutStateComponent>()) {
        const auto& burnout = founder.get<oasis::chill::BurnoutStateComponent>();
        if (!oasis::chill::CanExecuteDesignatorAction(spec, burnout)) {
            spdlog::warn("[Oasis Burnout Guard] Founder is in Cognitive Burnout! Productive fabrication action refused.");
            return false;
        }
    }

    // Guard 1 — Block Outdoor Earthworks ([1:Swale] & [2:Garden]) Indoors, on Roofs, or on Pavement
    if (effective_action_id == 1 || effective_action_id == 2) {
        const uint32_t ground_mat = oasis::graphics::GetProceduralVoxelMaterial(
            glm::ivec3(clicked_voxel.x, 0, clicked_voxel.z)
        );
        const uint32_t above_proc_mat = oasis::graphics::GetProceduralVoxelMaterial(
            glm::ivec3(clicked_voxel.x, 1, clicked_voxel.z)
        );
        if (oasis::designator::IsInsideLot402HouseInterior(clicked_voxel.x, clicked_voxel.y, clicked_voxel.z) ||
            clicked_voxel.y >= 2 ||
            ground_mat != 4u ||
            above_proc_mat != 0u) {
            g_last_designator_warning =
                "Cannot dig Swale/Garden indoors or on pavement! Click Backyard Soil (x:18..24).";
            g_designator_warning_until_tick = g_sim_tick + 180ULL;
            spdlog::warn("[Oasis Designator Guard] {}", g_last_designator_warning);
            return false;
        }
    }

    glm::ivec3 pos = (effective_action_id == 1 || effective_action_id == 2)
        ? glm::ivec3(clicked_voxel.x, 1, clicked_voxel.z)
        : ((clicked_voxel.y == 0) ? glm::ivec3(clicked_voxel.x, 1, clicked_voxel.z) : placement_voxel);
    pos.y = std::clamp(pos.y, 1, 6);

    // Prevent stacking a Swale on top of an already-completed Swale at y == 1
    if (effective_action_id == 1 && oasis::graphics::GetDynamicVoxel(pos.x, 1, pos.z) == 18u) {
        return false;
    }

    // Guard 2 — Snap [11:Roof Solar] to the House Roof When Clicked on the House Footprint
    const int abs_px = std::abs(pos.x);
    const int local_pz = ((pos.z % 16) + 16) % 16;
    if (effective_action_id == 11 &&
        abs_px >= 9 && abs_px <= 17 &&
        local_pz >= 2 && local_pz <= 10) {
        pos.y = std::max(pos.y, 4);
        while (pos.y < 6 && oasis::graphics::GetProceduralVoxelMaterial(glm::ivec3(pos.x, pos.y + 1, pos.z)) == 6u) {
            pos.y++;
        }
        g_arcball_camera.z_cutaway_level = oasis::graphics::MAX_CUTAWAY_Z;
        g_arcball_camera.manual_cutaway_override = false;
        auto r_ent = ecs.entity("Renderer");
        if (r_ent.is_alive() && r_ent.has<oasis::graphics::RenderState>()) {
            auto& state_ref = r_ent.get_mut<oasis::graphics::RenderState>();
            state_ref.arcball.z_cutaway_level = oasis::graphics::MAX_CUTAWAY_Z;
            state_ref.arcball.manual_cutaway_override = false;
            r_ent.modified<oasis::graphics::RenderState>();
        }
    } else if (pos.y >= 4) {
        while (pos.y < 6 && oasis::graphics::GetProceduralVoxelMaterial(glm::ivec3(pos.x, pos.y + 1, pos.z)) == 6u) {
            pos.y++;
        }
        g_arcball_camera.manual_cutaway_override = false;
        auto r_ent = ecs.entity("Renderer");
        if (r_ent.is_alive() && r_ent.has<oasis::graphics::RenderState>()) {
            r_ent.get_mut<oasis::graphics::RenderState>().arcball.manual_cutaway_override = false;
        }
    }

    // Avoid duplicate blueprints at the same pos
    bool duplicate_exists = false;
    ecs.each([&](oasis::fabrication::BlueprintComponent& existing_bp) {
        if (!existing_bp.is_completed &&
            existing_bp.x == pos.x &&
            existing_bp.y == pos.y &&
            existing_bp.z == pos.z) {
            duplicate_exists = true;
        }
    });
    if (duplicate_exists) {
        return false;
    }

    g_last_designator_warning.clear();
    g_designator_warning_until_tick = 0ULL;

    // Configure realistic work ticks & resource-gated material hauling requirements
    double req_work_ticks = 180.0;
    bool requires_haul = true;
    bool delivered = false;
    uint8_t req_mat_kind = 0u;

    switch (spec.infra_type) {
        case oasis::fabrication::InfrastructureType::SOLAR_PANEL:
            req_work_ticks = 240.0; // 4.0s at 1x
            requires_haul = true;
            delivered = false;
            req_mat_kind = 1u;      // Solar PV Kit
            SetLayer7NeighborMilestoneRemark(
                ecs, oasis::migration::Layer7CommentaryEvent::ROOF_SOLAR_PANEL
            );
            break;
        case oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT:
        case oasis::fabrication::InfrastructureType::DOOR_TIMBER:
        case oasis::fabrication::InfrastructureType::ROOF_TIMBER:
            req_work_ticks = 180.0; // 3.0s at 1x
            requires_haul = true;
            delivered = false;
            req_mat_kind = 2u;      // Timber Plank
            break;
        case oasis::fabrication::InfrastructureType::WALL_COB:
        case oasis::fabrication::InfrastructureType::WALL_URBANITE:
        case oasis::fabrication::InfrastructureType::ROCKET_MASS_HEATER:
            req_work_ticks = 180.0; // 3.0s at 1x
            requires_haul = true;
            delivered = false;
            req_mat_kind = 3u;      // Cob/Earth Lump
            break;
        case oasis::fabrication::InfrastructureType::SWALE:
            req_work_ticks = 150.0; // 2.5s at 1x (yields +2 cob_earth_lumps on completion!)
            requires_haul = false;
            delivered = true;
            req_mat_kind = 0u;
            break;
        case oasis::fabrication::InfrastructureType::COVER_CROP:
            req_work_ticks = 120.0; // 2.0s at 1x
            requires_haul = true;
            delivered = false;
            req_mat_kind = 4u;      // Seed Pack
            break;
        default:
            req_work_ticks = 180.0;
            requires_haul = false;
            delivered = true;
            req_mat_kind = 0u;
            break;
    }

    // Immediately stamp Stage 0 holographic blueprint ghost block (26u)
    oasis::graphics::SetDynamicVoxel(pos.x, pos.y, pos.z, oasis::designator::MAT_BLUEPRINT_GHOST);

    oasis::fabrication::BlueprintComponent bp_comp{};
    bp_comp.type = spec.infra_type;
    bp_comp.x = pos.x;
    bp_comp.y = pos.y;
    bp_comp.z = pos.z;
    bp_comp.required_work_ticks = req_work_ticks;
    bp_comp.invested_work_ticks = 0.0;
    bp_comp.caloric_cost_total = spec.kcal_cost;
    bp_comp.is_completed = false;
    bp_comp.focus_cost = spec.focus_cost;
    bp_comp.build_stage = 0u;
    bp_comp.requires_material_haul = requires_haul;
    bp_comp.materials_delivered = delivered;
    bp_comp.required_material_kind = req_mat_kind;

    ecs.entity().set<oasis::fabrication::BlueprintComponent>(bp_comp);

    spdlog::info("[Oasis Designator] Queued blueprint {} (action={}, work_ticks={:.0f}, mat_kind={}) at ({},{},{}) with holographic ghost (Mat 26)",
                 spec.name, effective_action_id, req_work_ticks, static_cast<unsigned>(req_mat_kind), pos.x, pos.y, pos.z);
    return true;
}

static bool DispatchOrchestratorTaskById(flecs::world& ecs, uint16_t task_id) {
    auto is_voxel_occupied_by_bp_or_dyn = [&](glm::ivec3 p) -> bool {
        if (oasis::graphics::GetDynamicVoxel(p.x, p.y, p.z) != 0u) {
            return true;
        }
        bool has_bp = false;
        ecs.each([&](const oasis::fabrication::BlueprintComponent& bp) {
            if (!bp.is_completed && bp.x == p.x && bp.y == p.y && bp.z == p.z) {
                has_bp = true;
            }
        });
        return has_bp;
    };

    if (task_id == 1u) {
        // Task alpha: Build Roof Solar
        static constexpr glm::ivec3 kRoofSolarSlots[] = {
            {13, 6, 6}, {14, 6, 6}, {12, 6, 6},
            {13, 6, 5}, {14, 6, 5}, {12, 6, 5}
        };
        for (const auto& slot : kRoofSolarSlots) {
            glm::ivec3 check_pos = slot;
            while (check_pos.y < 6 &&
                   oasis::graphics::GetProceduralVoxelMaterial(glm::ivec3(check_pos.x, check_pos.y + 1, check_pos.z)) == 6u) {
                check_pos.y++;
            }
            if (!is_voxel_occupied_by_bp_or_dyn(check_pos)) {
                return PlaceDesignationAtVoxel(ecs, slot, slot, 11u);
            }
        }
        return false;
    } else if (task_id == 2u) {
        // Task beta: Dig Backyard Swale
        static constexpr glm::ivec3 kBackyardSwaleSlots[] = {
            {21, 0, 6}, {21, 0, 7}, {21, 0, 8},
            {22, 0, 6}, {22, 0, 7}, {22, 0, 8}
        };
        for (const auto& slot : kBackyardSwaleSlots) {
            const glm::ivec3 surf_pos(slot.x, 1, slot.z);
            if (!is_voxel_occupied_by_bp_or_dyn(surf_pos)) {
                return PlaceDesignationAtVoxel(ecs, slot, surf_pos, 1u);
            }
        }
        return false;
    } else if (task_id == 3u) {
        // Task gamma: Harvest & Cook Meal
        if (TriggerHarvestGardenAction(ecs, nullptr)) {
            return true;
        }
        return TriggerCookKitchenAction(ecs);
    } else if (task_id == 4u) {
        // Requisition +2 Solar PV Kits & +2 Seed Packs via L6/L7 Exchange (costs 2 Ration Meals)
        auto camp = ecs.entity("GenesisCamp");
        if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
            auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
            if (oasis::migration::RequisitionSalvageKitsViaLayer7(stock)) {
                g_blocked_blueprint_status_note.clear();
                oasis::orchestrator::CompleteScheduledTask(g_temporal_schedule, 4u);
                SetLayer7NeighborMilestoneRemark(
                    ecs, oasis::migration::Layer7CommentaryEvent::COURIER_REQUISITION_DROP
                );
                spdlog::info("[Oasis Companion Bridge] Requisitioned +2 Solar PV Kits & +2 Seed Packs via L6/L7 Exchange (-2 Ration Meals).");
                return true;
            } else {
                g_last_designator_warning =
                    "Need at least 2 Ration Meals in Kitchen Pantry to requisition L7 Salvage Crate!";
                g_designator_warning_until_tick = g_sim_tick + 180ULL;
            }
        }
        return false;
    } else if (task_id == 5u) {
        // Task 5: mesh:RestIntent — Patio Hearth Rest & Recovery (Sigma)
        auto founder = ecs.entity("Founder");
        if (founder.is_alive() && founder.has<oasis::fabrication::FounderCommandComponent>()) {
            if (founder.has<oasis::psychology::PsychologyComponent>()) {
                auto& psych = founder.get_mut<oasis::psychology::PsychologyComponent>();
                psych.stress = std::max(0.0f, psych.stress - 0.15f);
                psych.focus = std::min(1.0f, psych.focus + 0.25f);
            }
            if (founder.has<oasis::fabrication::SomaticStateComponent>()) {
                auto& som = founder.get_mut<oasis::fabrication::SomaticStateComponent>();
                som.focus = std::min(1.0f, som.focus + 0.25f);
                som.is_exhausted = false;
            }
            const glm::ivec3 hearth_goal(19, 1, 9);
            g_founder_is_carrying = false;
            DispatchFounderPathTo(ecs, hearth_goal, oasis::fabrication::FounderActionState::RESTING);
            oasis::orchestrator::CompleteScheduledTask(g_temporal_schedule, 5u);
            spdlog::info("[Oasis Layer 6 Intent] Dispatched mesh:RestIntent to Backyard Patio Hearth (19,1,9) (-0.15 Stress, +0.25 Focus).");
            return true;
        }
        return false;
    }
    return false;
}

// Poll SDL input events: TAB camera toggle, Shift+Right-Click POI Arcball Orbit, Shift+Wheel Zoom,
// PageUp/PageDown roof cutaway, Right-Click context evaluator, Left-Click volume painter & SpatialIntentPayload
static void ProcessInputEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            g_running = false;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_TAB) {
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        state_ref.target_camera_mode = (state_ref.active_camera_mode == 0) ? 1 : 0;
                        state_ref.is_transitioning = true;
                        state_ref.transition_elapsed_sec = 0.0f;
                        spdlog::info("[Oasis Viewport System] SDLK_TAB pressed: Toggling camera mode to {} (400ms transition initiated)",
                                     (state_ref.target_camera_mode == 1) ? "Cadastral Isometric" : "3rd-Person Perspective");
                        r.modified<oasis::graphics::RenderState>();
                    }
                }
            } else if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                g_shift_pressed = true;
            } else if (event.key.keysym.sym == SDLK_PAGEUP || event.key.keysym.sym == SDLK_PAGEDOWN) {
                const int dz = (event.key.keysym.sym == SDLK_PAGEUP) ? +1 : -1;
                oasis::graphics::AdjustManualCutaway(g_arcball_camera, dz);
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        oasis::graphics::AdjustManualCutaway(state_ref.arcball, dz);
                        r.modified<oasis::graphics::RenderState>();
                    }
                }
            } else if (event.key.keysym.sym == SDLK_PLUS ||
                       event.key.keysym.sym == SDLK_EQUALS ||
                       event.key.keysym.sym == SDLK_KP_PLUS) {
                oasis::graphics::ZoomArcball(g_arcball_camera, 2.0f);
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        oasis::graphics::ZoomArcball(state_ref.arcball, 2.0f);
                        r.modified<oasis::graphics::RenderState>();
                    }
                }
            } else if (event.key.keysym.sym == SDLK_MINUS ||
                       event.key.keysym.sym == SDLK_KP_MINUS) {
                oasis::graphics::ZoomArcball(g_arcball_camera, -2.0f);
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        oasis::graphics::ZoomArcball(state_ref.arcball, -2.0f);
                        r.modified<oasis::graphics::RenderState>();
                    }
                }
            } else if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP ||
                       event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN ||
                       event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT ||
                       event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) {
                float fwd = 0.0f;
                float right = 0.0f;
                if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP) fwd += 1.0f;
                if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN) fwd -= 1.0f;
                if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) right += 1.0f;
                if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT) right -= 1.0f;
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        oasis::graphics::PanCameraByKey(state_ref, right, fwd);
                        r.modified<oasis::graphics::RenderState>();
                    }
                }
            } else if (event.key.keysym.sym == SDLK_0) {
                oasis::time::SetTimeWarpMultiplier(g_time_warp, 0);
            } else if (event.key.keysym.sym == SDLK_1) {
                oasis::time::SetTimeWarpMultiplier(g_time_warp, 1);
            } else if (event.key.keysym.sym == SDLK_2) {
                oasis::time::SetTimeWarpMultiplier(g_time_warp, 2);
            } else if (event.key.keysym.sym == SDLK_3) {
                oasis::time::SetTimeWarpMultiplier(g_time_warp, 5);
            } else if (event.key.keysym.sym == SDLK_4) {
                oasis::time::SetTimeWarpMultiplier(g_time_warp, 10);
            } else if (event.key.keysym.sym == SDLK_F1) {
                g_overlay_mode = (g_overlay_mode == 1u) ? 0u : 1u;
            } else if (event.key.keysym.sym == SDLK_F2) {
                g_overlay_mode = (g_overlay_mode == 2u) ? 0u : 2u;
            } else if (event.key.keysym.sym == SDLK_F3) {
                g_overlay_mode = (g_overlay_mode == 3u) ? 0u : 3u;
            } else if (event.key.keysym.sym == SDLK_F4) {
                g_overlay_mode = (g_overlay_mode == 4u) ? 0u : 4u;
            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                g_overlay_mode = 0u;
            }
        } else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                g_shift_pressed = false;
            }
        } else if (event.type == SDL_MOUSEWHEEL) {
            float wheel_x = (event.wheel.preciseX != 0.0f) ? event.wheel.preciseX : static_cast<float>(event.wheel.x);
            float wheel_y = (event.wheel.preciseY != 0.0f) ? event.wheel.preciseY : static_cast<float>(event.wheel.y);

            if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                wheel_x = -wheel_x;
                wheel_y = -wheel_y;
            }

            const SDL_Keymod mod = SDL_GetModState();
            const bool is_zoom_mod = ((mod & (KMOD_SHIFT | KMOD_ALT | KMOD_CTRL | KMOD_GUI)) != 0) || g_shift_pressed;

            if (g_ecs) {
                auto r = g_ecs->entity("Renderer");
                if (r.has<oasis::graphics::RenderState>()) {
                    auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                    if (is_zoom_mod) {
                        oasis::graphics::ZoomArcball(g_arcball_camera, wheel_y);
                        oasis::graphics::ZoomArcball(state_ref.arcball, wheel_y);
                    } else {
                        oasis::graphics::ApplyCameraPan(state_ref, wheel_x, -wheel_y, true);
                    }
                    r.modified<oasis::graphics::RenderState>();
                }
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            if (g_is_orbiting_arcball) {
                float dx = static_cast<float>(event.motion.x - g_last_mouse_x);
                float dy = static_cast<float>(event.motion.y - g_last_mouse_y);
                g_last_mouse_x = event.motion.x;
                g_last_mouse_y = event.motion.y;

                oasis::graphics::OrbitArcball(g_arcball_camera, dx * 0.008f, dy * 0.008f);
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        oasis::graphics::OrbitArcball(state_ref.arcball, dx * 0.008f, dy * 0.008f);
                        r.modified<oasis::graphics::RenderState>();
                    }
                }
            } else if (g_is_dragging) {
                float dx = static_cast<float>(event.motion.x - g_last_mouse_x);
                float dy = static_cast<float>(event.motion.y - g_last_mouse_y);
                g_last_mouse_x = event.motion.x;
                g_last_mouse_y = event.motion.y;

                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        oasis::graphics::ApplyCameraPan(state_ref, dx, dy, false);
                        state_ref.camera_uniforms.cursor_highlight = glm::vec4(0.0f); // suppress during drag
                        r.modified<oasis::graphics::RenderState>();
                    }
                }
            } else {
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        auto hit = oasis::graphics::ScreenToWorldRaycast(
                            event.motion.x, event.motion.y,
                            state_ref.width, state_ref.height,
                            state_ref.camera_uniforms.inv_view_proj,
                            state_ref.camera_uniforms.camera_pos,
                            state_ref.camera_uniforms.viewport_params,
                            192,
                            state_ref.arcball.z_cutaway_level
                        );
                        if (hit.hit) {
                            state_ref.camera_uniforms.cursor_highlight = glm::vec4(glm::vec3(hit.voxel_pos), 1.0f);
                            if (g_is_painting_volume) {
                                g_paint_end_voxel = hit.placement_pos;
                            }
                        } else {
                            state_ref.camera_uniforms.cursor_highlight = glm::vec4(0.0f);
                        }
                    }
                }
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            const SDL_Keymod mod = SDL_GetModState();
            const bool is_shift_down = ((mod & KMOD_SHIFT) != 0) || g_shift_pressed;

            if (event.button.button == SDL_BUTTON_RIGHT && is_shift_down) {
                g_is_orbiting_arcball = true;
                g_last_mouse_x = event.button.x;
                g_last_mouse_y = event.button.y;
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        auto hit = oasis::graphics::ScreenToWorldRaycast(
                            event.button.x, event.button.y,
                            state_ref.width, state_ref.height,
                            state_ref.camera_uniforms.inv_view_proj,
                            state_ref.camera_uniforms.camera_pos,
                            state_ref.camera_uniforms.viewport_params,
                            192,
                            state_ref.arcball.z_cutaway_level
                        );
                        // Strict Ray-Miss Guard: sky clicks leave POI 100% unchanged
                        oasis::graphics::LockPoiFromRaycast(g_arcball_camera, hit);
                        oasis::graphics::LockPoiFromRaycast(state_ref.arcball, hit);
                        r.modified<oasis::graphics::RenderState>();
                    }
                }
            } else if (event.button.button == SDL_BUTTON_MIDDLE ||
                       (event.button.button == SDL_BUTTON_LEFT && is_shift_down)) {
                g_is_dragging = true;
                g_last_mouse_x = event.button.x;
                g_last_mouse_y = event.button.y;
            } else if (event.button.button == SDL_BUTTON_LEFT) {
                // Spatial designator left-click placement / volume painting
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        auto hit = oasis::graphics::ScreenToWorldRaycast(
                            event.button.x, event.button.y,
                            state_ref.width, state_ref.height,
                            state_ref.camera_uniforms.inv_view_proj,
                            state_ref.camera_uniforms.camera_pos,
                            state_ref.camera_uniforms.viewport_params,
                            192,
                            state_ref.arcball.z_cutaway_level
                        );
                        if (hit.hit) {
                            g_is_painting_volume = true;
                            g_paint_start_voxel = hit.placement_pos;
                            g_paint_end_voxel = hit.placement_pos;
                            g_paint_start_clicked_voxel = hit.voxel_pos;

                            if (PlaceDesignationAtVoxel(*g_ecs, hit.voxel_pos, hit.placement_pos, g_active_designator_action_id)) {
                                auto vol = oasis::designator::ComputeDesignationVolume(g_paint_start_voxel, g_paint_end_voxel);
                                auto now_ns = static_cast<uint64_t>(
                                    std::chrono::duration_cast<std::chrono::nanoseconds>(
                                        std::chrono::steady_clock::now().time_since_epoch()
                                    ).count()
                                );
                                g_last_spatial_intent = oasis::designator::BuildSpatialIntentPayload(
                                    g_active_designator_action_id,
                                    vol.min_bound,
                                    vol.extents,
                                    now_ns
                                );
                            }
                        }
                    }
                }
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    if (r.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        auto hit = oasis::graphics::ScreenToWorldRaycast(
                            event.button.x, event.button.y,
                            state_ref.width, state_ref.height,
                            state_ref.camera_uniforms.inv_view_proj,
                            state_ref.camera_uniforms.camera_pos,
                            state_ref.camera_uniforms.viewport_params,
                            192,
                            state_ref.arcball.z_cutaway_level
                        );
                        g_last_context_hit = hit;
                        bool has_bp = false;
                        if (hit.hit) {
                            g_ecs->each([&](oasis::fabrication::BlueprintComponent& bp) {
                                if (!bp.is_completed &&
                                    bp.x == hit.voxel_pos.x &&
                                    bp.y == hit.voxel_pos.y &&
                                    bp.z == hit.voxel_pos.z) {
                                    has_bp = true;
                                }
                            });
#ifdef __EMSCRIPTEN__
                            EM_ASM({
                                if (typeof window !== 'undefined' && typeof window.openVoxelContextMenu === 'function') {
                                    window.openVoxelContextMenu($0, $1, $2, $3, $4, $5);
                                }
                            }, event.button.x, event.button.y, hit.voxel_pos.x, hit.voxel_pos.y, hit.voxel_pos.z, static_cast<int>(hit.material_id));
#endif
                        }
                        g_active_context_menu = oasis::designator::EvaluateVoxelContext(hit, has_bp);
                    }
                }
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (g_is_painting_volume && g_paint_end_voxel != g_paint_start_voxel && g_ecs) {
                    auto vol = oasis::designator::ComputeDesignationVolume(g_paint_start_voxel, g_paint_end_voxel);
                    if (vol.is_valid) {
                        for (const auto& v : vol.voxels) {
                            PlaceDesignationAtVoxel(
                                *g_ecs,
                                glm::ivec3(v.x, g_paint_start_clicked_voxel.y, v.z),
                                v,
                                g_active_designator_action_id
                            );
                        }
                        auto now_ns = static_cast<uint64_t>(
                            std::chrono::duration_cast<std::chrono::nanoseconds>(
                                std::chrono::steady_clock::now().time_since_epoch()
                            ).count()
                        );
                        g_last_spatial_intent = oasis::designator::BuildSpatialIntentPayload(
                            g_active_designator_action_id,
                            vol.min_bound,
                            vol.extents,
                            now_ns
                        );
                    }
                }
                g_is_dragging = false;
                g_is_painting_volume = false;
            } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                g_is_dragging = false;
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                g_is_orbiting_arcball = false;
            }
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_LEAVE || 
                event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                g_is_dragging = false;
                g_is_orbiting_arcball = false;
                g_is_painting_volume = false;
            }
        }
    }
}

#ifdef __EMSCRIPTEN__
void SetupPipeline(oasis::graphics::RenderState& state) {
    const std::string_view shaderCode = oasis::shaders::RAYMARCH_WGSL;

    wgpu::ShaderSourceWGSL wgslDesc{};
    wgslDesc.code = wgpu::StringView{shaderCode.data(), shaderCode.size()};

    wgpu::ShaderModuleDescriptor shaderModuleDesc{};
    shaderModuleDesc.nextInChain = &wgslDesc;
    wgpu::ShaderModule shaderModule = state.device.CreateShaderModule(&shaderModuleDesc);

    // Bind group layout with 2 entries:
    // Binding 0: CameraUniforms (Uniform, Fragment | Vertex)
    // Binding 1: DynamicVoxels (ReadOnlyStorage, Fragment)
    wgpu::BindGroupLayoutEntry bglEntries[2]{};
    bglEntries[0].binding = 0;
    bglEntries[0].visibility = wgpu::ShaderStage::Fragment | wgpu::ShaderStage::Vertex;
    bglEntries[0].buffer.type = wgpu::BufferBindingType::Uniform;
    bglEntries[0].buffer.minBindingSize = sizeof(oasis::graphics::CameraUniforms);

    bglEntries[1].binding = 1;
    bglEntries[1].visibility = wgpu::ShaderStage::Fragment;
    bglEntries[1].buffer.type = wgpu::BufferBindingType::ReadOnlyStorage;
    bglEntries[1].buffer.minBindingSize = sizeof(oasis::graphics::GpuDynamicVoxelBuffer);

    wgpu::BindGroupLayoutDescriptor bglDesc{};
    bglDesc.entryCount = 2;
    bglDesc.entries = bglEntries;
    state.bind_group_layout = state.device.CreateBindGroupLayout(&bglDesc);

    wgpu::PipelineLayoutDescriptor plDesc{};
    plDesc.bindGroupLayoutCount = 1;
    plDesc.bindGroupLayouts = &state.bind_group_layout;
    wgpu::PipelineLayout pipelineLayout = state.device.CreatePipelineLayout(&plDesc);

    wgpu::ColorTargetState colorTarget{};
    colorTarget.format = state.format;
    colorTarget.writeMask = wgpu::ColorWriteMask::All;

    wgpu::FragmentState fragmentState{};
    fragmentState.module = shaderModule;
    fragmentState.entryPoint = wgpu::StringView{"fs_main"};
    fragmentState.targetCount = 1;
    fragmentState.targets = &colorTarget;

    wgpu::PrimitiveState primitiveState{};
    primitiveState.topology = wgpu::PrimitiveTopology::TriangleList;
    primitiveState.cullMode = wgpu::CullMode::None; // Never cull full-screen quad!
    primitiveState.frontFace = wgpu::FrontFace::CCW;

    wgpu::RenderPipelineDescriptor pipelineDesc{};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.vertex.module = shaderModule;
    pipelineDesc.vertex.entryPoint = wgpu::StringView{"vs_main"};
    pipelineDesc.fragment = &fragmentState;
    pipelineDesc.primitive = primitiveState;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = 0xFFFFFFFF;

    state.pipeline = state.device.CreateRenderPipeline(&pipelineDesc);

    // Create camera_buffer (Uniform | CopyDst, size 256 bytes)
    wgpu::BufferDescriptor bufDesc{};
    bufDesc.label = wgpu::StringView{"CameraUniformBuffer"};
    bufDesc.size = sizeof(oasis::graphics::CameraUniforms);
    bufDesc.usage = wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst;
    state.camera_buffer = state.device.CreateBuffer(&bufDesc);

    // Create voxel_storage_buffer (Storage | CopyDst)
    wgpu::BufferDescriptor voxelBufDesc{};
    voxelBufDesc.label = wgpu::StringView{"DynamicVoxelStorageBuffer"};
    voxelBufDesc.size = sizeof(oasis::graphics::GpuDynamicVoxelBuffer);
    voxelBufDesc.usage = wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopyDst;
    state.voxel_storage_buffer = state.device.CreateBuffer(&voxelBufDesc);

    // Create camera_bind_group bound at index 0 and 1
    wgpu::BindGroupEntry bgEntries[2]{};
    bgEntries[0].binding = 0;
    bgEntries[0].buffer = state.camera_buffer;
    bgEntries[0].offset = 0;
    bgEntries[0].size = sizeof(oasis::graphics::CameraUniforms);

    bgEntries[1].binding = 1;
    bgEntries[1].buffer = state.voxel_storage_buffer;
    bgEntries[1].offset = 0;
    bgEntries[1].size = sizeof(oasis::graphics::GpuDynamicVoxelBuffer);

    wgpu::BindGroupDescriptor bgDesc{};
    bgDesc.layout = state.bind_group_layout;
    bgDesc.entryCount = 2;
    bgDesc.entries = bgEntries;
    state.camera_bind_group = state.device.CreateBindGroup(&bgDesc);

    // Initial camera uniform write
    oasis::graphics::UpdateCameraMatrices(state, 0.0f);
    state.queue.WriteBuffer(state.camera_buffer, 0, &state.camera_uniforms, sizeof(oasis::graphics::CameraUniforms));

    // Initial dynamic voxel buffer write
    state.queue.WriteBuffer(state.voxel_storage_buffer, 0, &state.voxel_data, sizeof(oasis::graphics::GpuDynamicVoxelBuffer));

    spdlog::info("WebGPU Render Pipeline and Camera Bind Group created successfully with cullMode = None and Dynamic Voxel SSBO");
}

void RenderWebGPUFrame(oasis::graphics::RenderState& state) {
    if (!state.is_ready()) return;

    // Update and upload camera uniform buffer each frame
    oasis::graphics::UpdateCameraMatrices(state, 1.0f / 60.0f);

    // Extract Founder avatar position and somatic/psych state from Flecs ECS
    if (g_ecs) {
        auto founder = g_ecs->entity("Founder");
        if (founder.is_alive() && founder.has<oasis::fabrication::SpatialPositionComponent>()) {
            const auto& pos = founder.get<oasis::fabrication::SpatialPositionComponent>();
            const float base_anim_time = std::fmod(static_cast<float>(g_sim_tick) * (1.0f / 60.0f), 9.0f);
            const float anim_time = base_anim_time + (g_founder_is_carrying ? 10.0f : 0.0f);
            state.camera_uniforms.avatar_pos = glm::vec4(pos.pos.x, pos.pos.y, pos.pos.z, anim_time);

            float action_state = 0.0f;
            float stress = 0.25f;
            float focus = 0.75f;

            if (founder.has<oasis::fabrication::FounderCommandComponent>()) {
                action_state = static_cast<float>(founder.get<oasis::fabrication::FounderCommandComponent>().state);
            }
            if (founder.has<oasis::psychology::PsychologyComponent>()) {
                const auto& psych = founder.get<oasis::psychology::PsychologyComponent>();
                stress = psych.stress;
                focus = psych.focus;
            }

            state.camera_uniforms.avatar_params = glm::vec4(action_state, stress, focus, 1.0f);
        }
    }

    state.queue.WriteBuffer(state.camera_buffer, 0, &state.camera_uniforms, sizeof(oasis::graphics::CameraUniforms));

    // Sync and upload dynamic voxels to WebGPU SSBO
    const auto& store = oasis::graphics::GetDynamicVoxelStore();
    uint32_t count = 0;
    int32_t min_x = 999999, min_y = 999999, min_z = 999999;
    int32_t max_x = -999999, max_y = -999999, max_z = -999999;

    for (const auto& [packed, mat_id] : store) {
        if (count >= oasis::graphics::MAX_GPU_DYNAMIC_VOXELS) break;
        int vx = 0, vy = 0, vz = 0;
        oasis::graphics::UnpackVoxelCoord(packed, vx, vy, vz);
        state.voxel_data.voxels[count].x = vx;
        state.voxel_data.voxels[count].y = vy;
        state.voxel_data.voxels[count].z = vz;
        state.voxel_data.voxels[count].material_id = static_cast<uint32_t>(mat_id);

        min_x = std::min(min_x, vx);
        min_y = std::min(min_y, vy);
        min_z = std::min(min_z, vz);
        max_x = std::max(max_x, vx);
        max_y = std::max(max_y, vy);
        max_z = std::max(max_z, vz);
        count++;
    }

    state.voxel_data.count = count;
    if (count > 0) {
        state.voxel_data.min_x = min_x;
        state.voxel_data.min_y = min_y;
        state.voxel_data.min_z = min_z;
        state.voxel_data.max_x = max_x;
        state.voxel_data.max_y = max_y;
        state.voxel_data.max_z = max_z;
    } else {
        state.voxel_data.min_x = 0;
        state.voxel_data.min_y = 0;
        state.voxel_data.min_z = 0;
        state.voxel_data.max_x = 0;
        state.voxel_data.max_y = 0;
        state.voxel_data.max_z = 0;
    }

    size_t upload_size = 32 + count * sizeof(oasis::graphics::GpuDynamicVoxel);
    upload_size = std::min(upload_size, sizeof(oasis::graphics::GpuDynamicVoxelBuffer));
    state.queue.WriteBuffer(state.voxel_storage_buffer, 0, &state.voxel_data, upload_size);

    wgpu::SurfaceTexture surfaceTexture;
    state.surface.GetCurrentTexture(&surfaceTexture);
    if (surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal &&
        surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal) {
        return;
    }
    if (!surfaceTexture.texture) return;

    wgpu::TextureView backbuffer = surfaceTexture.texture.CreateView();
    if (!backbuffer) return;

    wgpu::RenderPassColorAttachment colorAttachment{};
    colorAttachment.view = backbuffer;
    colorAttachment.clearValue = {
        state.clear_color[0],
        state.clear_color[1],
        state.clear_color[2],
        state.clear_color[3]
    };
    colorAttachment.loadOp = wgpu::LoadOp::Clear;
    colorAttachment.storeOp = wgpu::StoreOp::Store;

    wgpu::RenderPassDescriptor renderPassDesc{};
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &colorAttachment;

    wgpu::CommandEncoder encoder = state.device.CreateCommandEncoder();
    wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderPassDesc);
    pass.SetPipeline(state.pipeline);
    pass.SetBindGroup(0, state.camera_bind_group);
    pass.Draw(3, 1, 0, 0); // 3 vertices, 1 instance (full-screen triangle)
    pass.End();

    wgpu::CommandBuffer commands = encoder.Finish();
    state.queue.Submit(1, &commands);

    state.frame_count++;
}

void InitWebGPUAsync(uint32_t width, uint32_t height) {
    const int webgpu_ready = EM_ASM_INT({
        if (typeof window !== 'undefined' && window.location &&
            (window.location.hostname === '0.0.0.0' || window.location.hostname === '[::]' || window.location.hostname === '::')) {
            var targetUrl = window.location.protocol + '//localhost:' + window.location.port +
                            window.location.pathname + window.location.search + window.location.hash;
            console.warn('[Oasis WebGPU] Non-secure hostname ' + window.location.hostname + ' disables WebGPU. Redirecting to ' + targetUrl);
            window.location.replace(targetUrl);
            return -2;
        }
        if (typeof navigator === 'undefined' || !('gpu' in navigator) || !navigator.gpu) {
            return 0;
        }
        var c = document.querySelector('#canvas');
        if (!c || !c.getContext('webgpu')) {
            return -1;
        }
        return 1;
    });

    if (webgpu_ready <= 0) {
        spdlog::error("[Oasis WebGPU] WebGPU context unavailable (status={}). Ensure you open http://localhost:8080/oasis_engine.html in Google Chrome or Edge with WebGPU enabled.", webgpu_ready);
        return;
    }

    wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector canvasDesc{};
    canvasDesc.selector = wgpu::StringView{"#canvas"};

    wgpu::SurfaceDescriptor surfaceDesc{};
    surfaceDesc.nextInChain = &canvasDesc;

    wgpu::Instance instance = wgpu::CreateInstance();
    wgpu::Surface surface = instance.CreateSurface(&surfaceDesc);

    wgpu::RequestAdapterOptions adapterOpts{};
    adapterOpts.compatibleSurface = surface;

    instance.RequestAdapter(&adapterOpts, wgpu::CallbackMode::AllowSpontaneous,
        [instance, surface, width, height](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message) {
            if (status != wgpu::RequestAdapterStatus::Success) {
                spdlog::error("Failed to acquire WebGPU adapter: {}", 
                              message.data ? std::string_view(message.data, message.length) : "Unknown");
                return;
            }

            wgpu::DeviceDescriptor deviceDesc{};
            deviceDesc.SetUncapturedErrorCallback(
                [](const wgpu::Device&, wgpu::ErrorType errorType, wgpu::StringView msg) {
                    spdlog::error("WebGPU Device Error (type {}): {}", 
                                  static_cast<int>(errorType), 
                                  msg.data ? std::string_view(msg.data, msg.length) : "Unknown");
                });

            adapter.RequestDevice(&deviceDesc, wgpu::CallbackMode::AllowSpontaneous,
                [instance, surface, adapter, width, height](wgpu::RequestDeviceStatus devStatus, wgpu::Device device, wgpu::StringView devMessage) {
                    if (devStatus != wgpu::RequestDeviceStatus::Success) {
                        spdlog::error("Failed to acquire WebGPU device: {}", 
                                      devMessage.data ? std::string_view(devMessage.data, devMessage.length) : "Unknown");
                        return;
                    }

                    wgpu::Queue queue = device.GetQueue();

                    // Query surface capabilities for preferred format
                    wgpu::SurfaceCapabilities capabilities{};
                    surface.GetCapabilities(adapter, &capabilities);
                    wgpu::TextureFormat preferredFormat = (capabilities.formatCount > 0)
                        ? capabilities.formats[0]
                        : wgpu::TextureFormat::BGRA8Unorm;

                    spdlog::info("Configuring WebGPU surface with format: {}", static_cast<int>(preferredFormat));

                    wgpu::SurfaceConfiguration config{};
                    config.device = device;
                    config.format = preferredFormat;
                    config.usage = wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc;
                    config.width = width;
                    config.height = height;
                    config.alphaMode = wgpu::CompositeAlphaMode::Auto;
                    config.presentMode = wgpu::PresentMode::Fifo;
                    surface.Configure(&config);

                    // Update RenderState component in Flecs
                    if (g_ecs) {
                        oasis::graphics::RenderState state{};
                        state.instance = instance;
                        state.adapter = adapter;
                        state.device = device;
                        state.queue = queue;
                        state.surface = surface;
                        state.format = preferredFormat;
                        state.width = width;
                        state.height = height;
                        state.arcball = g_arcball_camera;

                        SetupPipeline(state);

                        state.is_initialized = true;
                        g_ecs->entity("Renderer").set<oasis::graphics::RenderState>(state);
                        spdlog::info("WebGPU initialization complete and RenderState updated in Flecs");
                    }
                });
        });
}

void EngineTick(void* arg) {
    (void)arg;
    if (g_ecs) {
        // Poll pending JS UI commands (Time Warp, Context Action ID, Kitchen Action, Direct Placement, Roof Cutaway, Overlay Mode)
        const int pending_warp = EM_ASM_INT({
            if (typeof window !== 'undefined' && typeof window._oasisPendingTimeWarp === 'number' && window._oasisPendingTimeWarp >= 0) {
                var w = window._oasisPendingTimeWarp;
                window._oasisPendingTimeWarp = -1;
                return w;
            }
            return -1;
        });
        if (pending_warp >= 0) {
            oasis::time::SetTimeWarpMultiplier(g_time_warp, static_cast<uint32_t>(pending_warp));
        }

        const int pending_kitchen_action = EM_ASM_INT({
            if (typeof window !== 'undefined' && typeof window._oasisPendingKitchenAction === 'number' && window._oasisPendingKitchenAction > 0) {
                var ka = window._oasisPendingKitchenAction;
                window._oasisPendingKitchenAction = 0;
                return ka;
            }
            return 0;
        });
        if (pending_kitchen_action == 13) {
            TriggerHarvestGardenAction(*g_ecs, nullptr);
        } else if (pending_kitchen_action == 14) {
            TriggerCookKitchenAction(*g_ecs);
        }

        const int pending_action = EM_ASM_INT({
            if (typeof window !== 'undefined' && typeof window._oasisPendingActionId === 'number' && window._oasisPendingActionId >= 0) {
                var a = window._oasisPendingActionId;
                window._oasisPendingActionId = -1;
                return a;
            }
            return -1;
        });
        if (pending_action >= 0) {
            if (pending_action == 13) {
                TriggerHarvestGardenAction(*g_ecs, nullptr);
            } else if (pending_action == 14) {
                TriggerCookKitchenAction(*g_ecs);
            } else {
                g_active_designator_action_id = static_cast<uint16_t>(pending_action);
                g_active_blueprint_type = MapActionIdToBlueprintType(g_active_designator_action_id, g_active_blueprint_type);
                if (g_active_designator_action_id == 11u) {
                    g_arcball_camera.z_cutaway_level = oasis::graphics::MAX_CUTAWAY_Z;
                    g_arcball_camera.manual_cutaway_override = false;
                    auto r_ent = g_ecs->entity("Renderer");
                    if (r_ent.is_alive() && r_ent.has<oasis::graphics::RenderState>()) {
                        auto& state_ref = r_ent.get_mut<oasis::graphics::RenderState>();
                        state_ref.arcball.z_cutaway_level = oasis::graphics::MAX_CUTAWAY_Z;
                        state_ref.arcball.manual_cutaway_override = false;
                        r_ent.modified<oasis::graphics::RenderState>();
                    }
                }
            }
        }

        const int pending_toggle_roof = EM_ASM_INT({
            if (typeof window !== 'undefined' && (window._oasisToggleRoofCutaway === 1 || window._oasisToggleRoofCutaway === true)) {
                window._oasisToggleRoofCutaway = 0;
                return 1;
            }
            return 0;
        });
        if (pending_toggle_roof == 1) {
            const int next_z = (g_arcball_camera.z_cutaway_level <= 3) ? oasis::graphics::MAX_CUTAWAY_Z : 3;
            g_arcball_camera.z_cutaway_level = next_z;
            g_arcball_camera.manual_cutaway_override = (next_z < oasis::graphics::MAX_CUTAWAY_Z);
            auto r_ent = g_ecs->entity("Renderer");
            if (r_ent.is_alive() && r_ent.has<oasis::graphics::RenderState>()) {
                auto& state_ref = r_ent.get_mut<oasis::graphics::RenderState>();
                state_ref.arcball.z_cutaway_level = next_z;
                state_ref.arcball.manual_cutaway_override = (next_z < oasis::graphics::MAX_CUTAWAY_Z);
                r_ent.modified<oasis::graphics::RenderState>();
            }
            spdlog::info("[Oasis Camera] Toggled Roof Cutaway level to z_cutaway_level={}", next_z);
        }

        const int pending_wave_neighbor = EM_ASM_INT({
            if (typeof window !== 'undefined' && (window._oasisWaveNeighborRequested === 1 || window._oasisWaveNeighborRequested === true)) {
                window._oasisWaveNeighborRequested = 0;
                return 1;
            }
            return 0;
        });
        if (pending_wave_neighbor == 1) {
            TriggerLayer7MinimumGrace(*g_ecs, true);
        }

        const int has_direct_placement = EM_ASM_INT({
            if (typeof window !== 'undefined' &&
                window._oasisPendingDirectPlacement &&
                window._oasisPendingDirectPlacement.pending === 1) {
                return 1;
            }
            return 0;
        });
        if (has_direct_placement == 1) {
            const int dp_x = EM_ASM_INT({ return window._oasisPendingDirectPlacement.x | 0; });
            const int dp_y = EM_ASM_INT({ return window._oasisPendingDirectPlacement.y | 0; });
            const int dp_z = EM_ASM_INT({ return window._oasisPendingDirectPlacement.z | 0; });
            const int dp_action = EM_ASM_INT({
                var a = window._oasisPendingDirectPlacement.actionId | 0;
                window._oasisPendingDirectPlacement.pending = 0;
                return a;
            });
            if (dp_action > 0) {
                if (dp_action != 13 && dp_action != 14 && dp_action != 99) {
                    g_active_designator_action_id = static_cast<uint16_t>(dp_action);
                    g_active_designator_action = static_cast<uint16_t>(dp_action);
                    g_active_blueprint_type = MapActionIdToBlueprintType(g_active_designator_action_id, g_active_blueprint_type);
                }
                const glm::ivec3 clicked_v(dp_x, dp_y, dp_z);
                const glm::ivec3 placement_v = (g_last_context_hit.hit && g_last_context_hit.voxel_pos == clicked_v)
                    ? g_last_context_hit.placement_pos
                    : ((dp_y == 0) ? glm::ivec3(dp_x, 1, dp_z) : glm::ivec3(dp_x, dp_y + 1, dp_z));
                PlaceDesignationAtVoxel(*g_ecs, clicked_v, placement_v, static_cast<uint16_t>(dp_action));
            }
        }

        // Sprint 13.7 & Sprint 14: Companion / BPMN Bounty Live Task Bridge (_oasisPendingCompanionTask)
        const int pending_companion_task = EM_ASM_INT({
            if (typeof window !== 'undefined' &&
                typeof window._oasisPendingCompanionTask === 'number' &&
                window._oasisPendingCompanionTask > 0) {
                var t = window._oasisPendingCompanionTask | 0;
                window._oasisPendingCompanionTask = 0;
                return t;
            }
            return 0;
        });
        if (pending_companion_task > 0) {
            DispatchOrchestratorTaskById(*g_ecs, static_cast<uint16_t>(pending_companion_task));
        }

        // Sprint 14: Multi-Day Temporal Workflow Schedule Slot Customization (_oasisPendingScheduleUpdate)
        const int has_schedule_update = EM_ASM_INT({
            if (typeof window !== 'undefined' &&
                window._oasisPendingScheduleUpdate &&
                window._oasisPendingScheduleUpdate.pending === 1) {
                return 1;
            }
            return 0;
        });
        if (has_schedule_update == 1) {
            const int sched_day = EM_ASM_INT({ return window._oasisPendingScheduleUpdate.day | 0; });
            const int sched_task = EM_ASM_INT({
                var tid = window._oasisPendingScheduleUpdate.taskId | 0;
                window._oasisPendingScheduleUpdate.pending = 0;
                return tid;
            });
            if (oasis::orchestrator::UpdateScheduleSlotTask(
                    g_temporal_schedule,
                    static_cast<uint32_t>(sched_day),
                    static_cast<uint16_t>(sched_task))) {
                spdlog::info("[Oasis L4/L5 BPMN Scheduler] Updated Day {} workflow slot to Task #{} ({})",
                             sched_day, sched_task,
                             g_temporal_schedule.days[static_cast<size_t>(sched_day - 1)].task_label);
            }
        }

        // Sprint 14: Lock L5 Gates & Commit / Toggle Auto-Run Schedule (_oasisPendingScheduleCommit)
        const int pending_schedule_commit = EM_ASM_INT({
            if (typeof window !== 'undefined' &&
                typeof window._oasisPendingScheduleCommit === 'number' &&
                window._oasisPendingScheduleCommit > 0) {
                var c = window._oasisPendingScheduleCommit | 0;
                window._oasisPendingScheduleCommit = 0;
                return c;
            }
            return 0;
        });
        if (pending_schedule_commit > 0) {
            oasis::orchestrator::LockScheduleL5Gates(g_temporal_schedule, 1u);
            if (pending_schedule_commit == 1) {
                g_temporal_schedule.auto_dispatch_enabled = true;
            } else if (pending_schedule_commit == 2) {
                g_temporal_schedule.auto_dispatch_enabled = !g_temporal_schedule.auto_dispatch_enabled;
            }
            spdlog::info("[Oasis L4/L5 BPMN Scheduler] L5 Trust Gates locked & Auto-Dispatch set to {}",
                         g_temporal_schedule.auto_dispatch_enabled);
        }

        // Step Pillar 1 Asynchronous Node Proxies & Pillar 3 Temporal Schedule
        oasis::orchestrator::StepAsyncNodeProxies(g_collective_net, g_sim_tick);
        {
            auto founder_sched = g_ecs->entity("Founder");
            bool founder_idle_for_sched = false;
            if (founder_sched.is_alive() && founder_sched.has<oasis::fabrication::FounderCommandComponent>()) {
                const auto& fcmd = founder_sched.get<oasis::fabrication::FounderCommandComponent>();
                founder_idle_for_sched = (fcmd.state == oasis::fabrication::FounderActionState::IDLE);
            }
            if (founder_idle_for_sched) {
                g_ecs->each([&](const oasis::fabrication::BlueprintComponent& bp) {
                    if (!bp.is_completed) {
                        founder_idle_for_sched = false;
                    }
                });
            }
            const uint16_t auto_task_id = oasis::orchestrator::StepTemporalSchedule(
                g_temporal_schedule, g_sim_tick, founder_idle_for_sched
            );
            if (auto_task_id > 0u) {
                DispatchOrchestratorTaskById(*g_ecs, auto_task_id);
                spdlog::info("[Oasis L4/L5 BPMN Scheduler] Auto-dispatched scheduled Day {} Task #{} to Founder!",
                             oasis::orchestrator::ComputeSimDayFromTick(g_sim_tick), auto_task_id);
            }
        }

        const int pending_cutaway = EM_ASM_INT({
            if (typeof window !== 'undefined' && typeof window._oasisPendingCutawayLevel === 'number' && window._oasisPendingCutawayLevel >= 0) {
                var c = window._oasisPendingCutawayLevel;
                window._oasisPendingCutawayLevel = -1;
                return c;
            }
            return -1;
        });
        if (pending_cutaway >= 0) {
            const int clamped_z = std::clamp(pending_cutaway, oasis::graphics::MIN_CUTAWAY_Z, oasis::graphics::MAX_CUTAWAY_Z);
            g_arcball_camera.z_cutaway_level = clamped_z;
            g_arcball_camera.manual_cutaway_override = (clamped_z < oasis::graphics::MAX_CUTAWAY_Z);
            auto r_ent = g_ecs->entity("Renderer");
            if (r_ent.is_alive() && r_ent.has<oasis::graphics::RenderState>()) {
                auto& state_ref = r_ent.get_mut<oasis::graphics::RenderState>();
                state_ref.arcball.z_cutaway_level = clamped_z;
                state_ref.arcball.manual_cutaway_override = (clamped_z < oasis::graphics::MAX_CUTAWAY_Z);
                r_ent.modified<oasis::graphics::RenderState>();
            }
        }

        const int pending_overlay = EM_ASM_INT({
            if (typeof window !== 'undefined' && typeof window._oasisPendingOverlayMode === 'number' && window._oasisPendingOverlayMode >= 0) {
                var m = window._oasisPendingOverlayMode;
                window._oasisPendingOverlayMode = -1;
                return m;
            }
            return -1;
        });
        if (pending_overlay >= 0) {
            g_overlay_mode = static_cast<uint32_t>(std::clamp(pending_overlay, 0, 4));
        }

        // EPIC-11 (STORY-11.1, 11.2, 11.3): Poll interactive JS triggers from shell_minimal.html
        const int pending_trigger_wave = EM_ASM_INT({
            if (typeof window !== 'undefined' && (window._oasisPendingTriggerWave === 1 || window._oasisPendingTriggerWave === true)) {
                window._oasisPendingTriggerWave = 0;
                return 1;
            }
            return 0;
        });
        if (pending_trigger_wave == 1) {
            oasis::uhai::MigrationWaveArrivalEvent manual_wave{};
            manual_wave.timestamp_hlc = g_sim_tick;
            manual_wave.wave_id = ++g_manual_wave_seq;
            manual_wave.deterministic_seed = 4200u + g_manual_wave_seq;
            manual_wave.legacy_distress_index = 0.78f;
            manual_wave.initial_stress_mean = 0.60f;
            manual_wave.caloric_deficit_kcal = 500.0f;
            manual_wave.cohort_size = 3u;
            manual_wave.initial_trust_ring = 6u;
            manual_wave.trigger_reason_code = 1u;
            const uint32_t spawned = oasis::migration::SpawnMigrationWaveInOasis(
                *g_ecs, manual_wave, g_migration_staging, g_time_warp
            );
            oasis::migration::SyncMigrantAvatarVoxels(*g_ecs);
            spdlog::info("[Oasis Migration] Manual Migration Wave #{} spawned {} R6 migrants (staged overflow={})",
                         manual_wave.wave_id, spawned, g_migration_staging.staged_overflow_migrants);
        }

        const int pending_snap_migrants = EM_ASM_INT({
            if (typeof window !== 'undefined' && (window._oasisPendingSnapMigrants === 1 || window._oasisPendingSnapMigrants === true)) {
                window._oasisPendingSnapMigrants = 0;
                return 1;
            }
            return 0;
        });
        if (pending_snap_migrants == 1) {
            glm::vec3 snap_pos(4.5f, 1.0f, 6.0f);
            glm::vec3 sum_pos(0.0f);
            uint32_t mig_cnt = 0;
            g_ecs->each([&](const oasis::migration::LegacyMigrantComponent&,
                            const oasis::fabrication::SpatialPositionComponent& m_pos) {
                sum_pos += m_pos.pos;
                mig_cnt++;
            });
            if (mig_cnt > 0u) {
                snap_pos = sum_pos / static_cast<float>(mig_cnt);
            }
            g_arcball_camera.has_poi_lock = true;
            g_arcball_camera.target_poi = snap_pos;
            g_arcball_camera.poi_origin = snap_pos;
            auto r_ent = g_ecs->entity("Renderer");
            if (r_ent.is_alive() && r_ent.has<oasis::graphics::RenderState>()) {
                auto& state_ref = r_ent.get_mut<oasis::graphics::RenderState>();
                state_ref.arcball.has_poi_lock = true;
                state_ref.arcball.target_poi = snap_pos;
                state_ref.arcball.poi_origin = snap_pos;
                r_ent.modified<oasis::graphics::RenderState>();
            }
            spdlog::info("[Oasis Camera] Snapped Arcball POI to Sidewalk Neighbor centroid at ({:.1f}, {:.1f}, {:.1f})",
                         snap_pos.x, snap_pos.y, snap_pos.z);
        }

        const int pending_promote_r3 = EM_ASM_INT({
            if (typeof window !== 'undefined' && (window._oasisPendingPromoteR3 === 1 || window._oasisPendingPromoteR3 === true)) {
                window._oasisPendingPromoteR3 = 0;
                return 1;
            }
            return 0;
        });
        if (pending_promote_r3 == 1) {
            uint32_t promoted_count = 0;
            g_ecs->each([&](oasis::migration::LegacyMigrantComponent& mig,
                            oasis::psychology::PsychologyComponent& m_psy) {
                if (mig.trust_ring_tier == 6u) {
                    mig.sheltered_days_consecutive = std::max(mig.sheltered_days_consecutive, 7u);
                    mig.potw_sweat_equity_mj = std::max(mig.potw_sweat_equity_mj, 36.0f);
                    mig.mindset_flags &= static_cast<uint8_t>(~oasis::migration::HOARDING_ACTIVE);
                    if (oasis::migration::TryPromoteMigrantR6ToR3(mig, m_psy)) {
                        promoted_count++;
                    }
                }
            });
            oasis::migration::SyncMigrantAvatarVoxels(*g_ecs);
            spdlog::info("[Oasis Trust Ring] Promoted {} R6 migrants to R3 Probationary Resident via PoTW attestation", promoted_count);
        }

        const int pending_vouch_r1 = EM_ASM_INT({
            if (typeof window !== 'undefined' && (window._oasisPendingVouchR1 === 1 || window._oasisPendingVouchR1 === true)) {
                window._oasisPendingVouchR1 = 0;
                return 1;
            }
            return 0;
        });
        if (pending_vouch_r1 == 1) {
            uint32_t vouched_count = 0;
            g_ecs->each([&](oasis::migration::LegacyMigrantComponent& mig,
                            oasis::psychology::PsychologyComponent& m_psy) {
                if (mig.trust_ring_tier == 6u) {
                    mig.sheltered_days_consecutive = std::max(mig.sheltered_days_consecutive, 7u);
                    mig.potw_sweat_equity_mj = std::max(mig.potw_sweat_equity_mj, 36.0f);
                    mig.mindset_flags &= static_cast<uint8_t>(~oasis::migration::HOARDING_ACTIVE);
                    oasis::migration::TryPromoteMigrantR6ToR3(mig, m_psy);
                }
                if (mig.trust_ring_tier == 3u) {
                    mig.days_in_r3_consecutive = std::max(mig.days_in_r3_consecutive, 30u);
                    if (oasis::migration::TryPromoteMigrantR3ToR1(mig, m_psy, 2u)) {
                        vouched_count++;
                    }
                }
            });
            oasis::migration::SyncMigrantAvatarVoxels(*g_ecs);
            spdlog::info("[Oasis Trust Ring] Vouched {} R3 residents to R1 Steward via 2-of-3 FROST ceremony", vouched_count);
        }

        ProcessInputEvents();

        auto founder = g_ecs->entity("Founder");
        if (founder.is_alive() &&
            founder.has<oasis::fabrication::SomaticStateComponent>() &&
            founder.has<oasis::psychology::PsychologyComponent>()) {
            const auto& som = founder.get<oasis::fabrication::SomaticStateComponent>();
            const auto& psy = founder.get<oasis::psychology::PsychologyComponent>();
            if (oasis::time::EvaluateEmergencyWarpBrake(g_time_warp, som.caloric_reserve, som.hydration, psy.stress, false)) {
                spdlog::warn("[Oasis Time Warp] Emergency Auto-Downshift to 1x triggered: {}", g_time_warp.emergency_reason);
            }
        }

        for (uint32_t step = 0; step < g_time_warp.multiplier; ++step) {
            g_ecs->progress();
            if (founder.is_alive() &&
                founder.has<oasis::fabrication::SomaticStateComponent>() &&
                founder.has<oasis::psychology::PsychologyComponent>()) {
                const auto& som = founder.get<oasis::fabrication::SomaticStateComponent>();
                const auto& psy = founder.get<oasis::psychology::PsychologyComponent>();
                if (oasis::time::EvaluateEmergencyWarpBrake(g_time_warp, som.caloric_reserve, som.hydration, psy.stress, false)) {
                    spdlog::warn("[Oasis Time Warp] Mid-frame Emergency Auto-Downshift to 1x triggered: {}", g_time_warp.emergency_reason);
                    break;
                }
            }
        }

        // Auto-lift roof cutaway when Founder climbs onto the roof (pos.pos.y >= 3.2f)
        auto r = g_ecs->entity("Renderer");
        if (r.is_alive() && r.has<oasis::graphics::RenderState>()) {
            auto& state = r.get_mut<oasis::graphics::RenderState>();
            if (founder.is_alive() && founder.has<oasis::fabrication::SpatialPositionComponent>()) {
                const auto& fpos = founder.get<oasis::fabrication::SpatialPositionComponent>();
                if (fpos.pos.y >= 3.2f && !state.arcball.manual_cutaway_override) {
                    state.arcball.z_cutaway_level = 32;
                    g_arcball_camera.z_cutaway_level = 32;
                }
            }
            // Strictly once per 60 Hz frame: execute WebGPU render pass, push aggregated telemetry, and update DOM HUD
            RenderWebGPUFrame(state);
        }
        TickTelemetry();

        if (founder.is_alive() &&
            founder.has<oasis::fabrication::SomaticStateComponent>() &&
            founder.has<oasis::psychology::PsychologyComponent>()) {
            const auto& som = founder.get<oasis::fabrication::SomaticStateComponent>();
            const auto& psy = founder.get<oasis::psychology::PsychologyComponent>();
            bool is_burned_out = false;
            if (founder.has<oasis::chill::BurnoutStateComponent>()) {
                is_burned_out = founder.get<oasis::chill::BurnoutStateComponent>().is_burned_out;
            }

            // Count active solar panels, cisterns/swales, and cover/mature crops for real Net Trajectory HUD
            uint32_t solar_panel_count = 0;
            uint32_t water_collector_count = 0;
            uint32_t cover_crop_count = 0;
            uint32_t swale_count = 0;
            for (const auto& [packed, mat_id] : oasis::graphics::GetDynamicVoxelStore()) {
                (void)packed;
                if (mat_id == 11u) {
                    solar_panel_count++;
                } else if (mat_id == 12u) {
                    water_collector_count++;
                } else if (mat_id == 18u) {
                    water_collector_count++;
                    swale_count++;
                } else if (mat_id == 19u || mat_id == 34u) {
                    cover_crop_count++;
                }
            }

            if (swale_count > 0u || cover_crop_count > 0u) {
                g_quest_has_garden = true;
            }
            if (solar_panel_count > 0u || (water_collector_count - swale_count) > 1u) {
                g_quest_has_roof_solar_or_cistern = true;
            }

            const auto eval_h = oasis::homeostasis::EvaluateHomeostasis(som.ambient_temp_c);
            const double net_kcal_day = -2000.0 * eval_h.burn_multiplier + static_cast<double>(cover_crop_count) * 300.0;
            const double net_water_day = -3.0 + static_cast<double>(water_collector_count) * 8.0 + static_cast<double>(cover_crop_count) * 2.0;
            double net_kwh_day = static_cast<double>(solar_panel_count) * 4.5;
            auto boundary = g_ecs->entity("SettlementBoundary");
            if (boundary.is_alive() && boundary.has<oasis::boundary::InfrastructureAutarkyComponent>()) {
                net_kwh_day = std::max(net_kwh_day, boundary.get<oasis::boundary::InfrastructureAutarkyComponent>().solar_stored_kwh);
            }
            const double net_stress_hr = is_burned_out ? -25.0 : (eval_h.in_goldilocks ? -1.5 : 3.5);

            EM_ASM({
                if (typeof updateHUD === 'function') {
                    updateHUD($0, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
                } else if (typeof window !== 'undefined' && typeof window.updateHUD === 'function') {
                    window.updateHUD($0, $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
                }
            }, som.caloric_reserve,
               static_cast<double>(psy.stress),
               static_cast<double>(psy.focus),
               static_cast<double>(som.ambient_temp_c),
               static_cast<int>(g_time_warp.multiplier),
               g_arcball_camera.z_cutaway_level,
               net_kcal_day,
               net_water_day,
               net_kwh_day,
               net_stress_hr,
               static_cast<double>(g_sim_tick),
               is_burned_out ? 1 : 0);

            // Live Founder Activity Status & Camp Stock HUD Update
            uint32_t queued_bp_count = 0;
            g_ecs->each([&](const oasis::fabrication::BlueprintComponent& bp) {
                if (!bp.is_completed) {
                    queued_bp_count++;
                }
            });

            uint32_t rations = 12;
            double water_l = 60.0;
            uint32_t solar_kits = 2;
            uint32_t timber_planks = 6;
            uint32_t cob_lumps = 4;
            uint32_t raw_produce = 2;
            auto camp = g_ecs->entity("GenesisCamp");
            if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                const auto& stock = camp.get<oasis::chill::GenesisCampStockComponent>();
                rations = stock.ration_meals_remaining;
                water_l = stock.potable_water_liters;
                solar_kits = stock.solar_kits_remaining;
                timber_planks = stock.timber_planks_remaining;
                cob_lumps = stock.cob_earth_lumps;
                raw_produce = stock.raw_produce_items;
            }

            const char* metabolic_state_str = oasis::chill::FormatMetabolicStateLabel(som);
            const auto active_spec = oasis::designator::GetDesignatorToolSpecById(g_active_designator_action_id);
            std::string status_str = "Idle in Lot 402 House (Node 0 Online)";
            glm::vec3 f_world_pos(13.0f, 1.0f, 6.0f);
            if (founder.has<oasis::fabrication::SpatialPositionComponent>()) {
                f_world_pos = founder.get<oasis::fabrication::SpatialPositionComponent>().pos;
            }
            const bool climbing_ladder =
                (static_cast<int>(std::round(f_world_pos.x)) == 18 &&
                 static_cast<int>(std::round(f_world_pos.z)) == 3 &&
                 f_world_pos.y >= 1.15f);

            if (g_sim_tick < g_designator_warning_until_tick && !g_last_designator_warning.empty()) {
                status_str = g_last_designator_warning;
            } else if (is_burned_out) {
                status_str = "Burned Out — Recovering by Backyard Patio Hearth (-Stress)";
            } else if (climbing_ladder) {
                char buf[128]{};
                std::snprintf(buf, sizeof(buf), "Climbing Roof Ladder (18,%d,3)",
                              static_cast<int>(std::round(f_world_pos.y)));
                status_str = buf;
            } else if (founder.has<oasis::fabrication::FounderCommandComponent>()) {
                const auto& cmd = founder.get<oasis::fabrication::FounderCommandComponent>();
                if (cmd.state == oasis::fabrication::FounderActionState::HAULING_TO_SOURCE &&
                    cmd.target_blueprint.is_alive() &&
                    cmd.target_blueprint.has<oasis::fabrication::BlueprintComponent>()) {
                    const auto& bp = cmd.target_blueprint.get<oasis::fabrication::BlueprintComponent>();
                    char buf[160]{};
                    std::snprintf(buf, sizeof(buf), "Hauling %s from Driveway (8,1,3)",
                                  oasis::chill::FormatMaterialKindName(bp.required_material_kind));
                    status_str = buf;
                } else if (cmd.state == oasis::fabrication::FounderActionState::HAULING_TO_SITE &&
                           cmd.target_blueprint.is_alive() &&
                           cmd.target_blueprint.has<oasis::fabrication::BlueprintComponent>()) {
                    const auto& bp = cmd.target_blueprint.get<oasis::fabrication::BlueprintComponent>();
                    char buf[160]{};
                    std::snprintf(buf, sizeof(buf), "Carrying %s to %s (%d,%d,%d)",
                                  oasis::chill::FormatMaterialKindName(bp.required_material_kind),
                                  FormatBlueprintTypeName(bp.type), bp.x, bp.y, bp.z);
                    status_str = buf;
                } else if (cmd.state == oasis::fabrication::FounderActionState::CONSTRUCTING &&
                           cmd.target_blueprint.is_alive() &&
                           cmd.target_blueprint.has<oasis::fabrication::BlueprintComponent>()) {
                    const auto& bp = cmd.target_blueprint.get<oasis::fabrication::BlueprintComponent>();
                    const int pct = static_cast<int>(std::clamp(
                        (bp.invested_work_ticks / std::max(1.0, bp.required_work_ticks)) * 100.0,
                        0.0, 100.0
                    ));
                    const unsigned stage_num = static_cast<unsigned>(std::max<uint8_t>(1u, bp.build_stage));
                    char buf[160]{};
                    std::snprintf(buf, sizeof(buf), "Assembling %s (Stage %u/3 - %d%%) at (%d,%d,%d)",
                                  FormatBlueprintTypeName(bp.type), stage_num, pct, bp.x, bp.y, bp.z);
                    status_str = buf;
                } else if (cmd.state == oasis::fabrication::FounderActionState::NAVIGATING) {
                    char buf[128]{};
                    std::snprintf(buf, sizeof(buf), "Walking to Blueprint (%d,%d,%d)",
                                  cmd.target_voxel.x, cmd.target_voxel.y, cmd.target_voxel.z);
                    status_str = buf;
                } else if (cmd.state == oasis::fabrication::FounderActionState::SEEKING_WATER) {
                    status_str = "Walking to Kitchen Water Filter (14,1,4) to Drink";
                } else if (cmd.state == oasis::fabrication::FounderActionState::SEEKING_FOOD) {
                    status_str = "Walking to Kitchen Pantry (11,1,4) to Eat Meal";
                } else if (cmd.state == oasis::fabrication::FounderActionState::HARVESTING_CROP) {
                    char buf[128]{};
                    std::snprintf(buf, sizeof(buf), "Harvesting Backyard Garden Crop at (%d,%d,%d)",
                                  cmd.target_voxel.x, cmd.target_voxel.y, cmd.target_voxel.z);
                    status_str = buf;
                } else if (cmd.state == oasis::fabrication::FounderActionState::COOKING_MEAL) {
                    status_str = "Cooking Homestead Meal at Kitchen (Scenario Γ)";
                } else if (cmd.state == oasis::fabrication::FounderActionState::RESTING ||
                           cmd.state == oasis::fabrication::FounderActionState::EXHAUSTED) {
                    if (cmd.target_voxel == glm::ivec3(19, 1, 9)) {
                        const float dist_hearth = glm::length(f_world_pos - glm::vec3(19.0f, 1.0f, 9.0f));
                        status_str = (dist_hearth <= 2.5f)
                            ? "Resting by Backyard Patio Hearth (-Stress, +Focus)"
                            : "Walking to Backyard Patio Hearth (19,1,9) to Rest";
                    } else {
                        const float dist_tent = glm::length(f_world_pos - glm::vec3(11.0f, 1.0f, 8.0f));
                        status_str = (dist_tent <= 2.5f)
                            ? "Resting in Lot 402 Bedroom (+Focus)"
                            : "Walking to Bedroom (11,1,8) to Rest";
                    }
                } else if (cmd.state == oasis::fabrication::FounderActionState::IDLE &&
                           !g_blocked_blueprint_status_note.empty()) {
                    status_str = g_blocked_blueprint_status_note;
                }
            }

            EM_ASM({
                if (typeof window !== 'undefined' && typeof window._oasisSetFounderStatus === 'function') {
                    window._oasisSetFounderStatus(
                        UTF8ToString($0),
                        $1,
                        $2,
                        $3,
                        UTF8ToString($4),
                        $5,
                        $6,
                        $7,
                        $8,
                        UTF8ToString($9)
                    );
                }
            }, status_str.c_str(),
               static_cast<int>(rations),
               water_l,
               static_cast<int>(queued_bp_count),
               active_spec.name.c_str(),
               static_cast<int>(solar_kits),
               static_cast<int>(timber_planks),
               static_cast<int>(cob_lumps),
               static_cast<int>(raw_produce),
               metabolic_state_str);

            // Sprint 13.5: Homestead Quest HUD Sync
            EM_ASM({
                if (typeof window !== 'undefined' && typeof window.updateHomesteadQuestHUD === 'function') {
                    window.updateHomesteadQuestHUD(Boolean($0), Boolean($1), Boolean($2), $3, UTF8ToString($4));
                }
            }, g_quest_has_garden ? 1 : 0,
               g_quest_has_roof_solar_or_cistern ? 1 : 0,
               g_quest_has_l7_grace ? 1 : 0,
               g_arcball_camera.z_cutaway_level,
               g_latest_l7_neighbor_quote.c_str());

            // Enclave Demographics & 4-Needs Migrant Assimilation Checklist HUD Update (EPIC-11)
            uint32_t total_npcs = 0;
            uint32_t r6_count = 0;
            uint32_t r3_count = 0;
            uint32_t r1_count = 0;
            bool shelter_ok = true;
            bool hydration_ok = true;
            bool calories_ok = true;
            float max_potw_mj = 0.0f;
            uint32_t max_shelter_days = 0;
            uint32_t max_r3_days = 0;
            uint8_t combined_mindset_flags = 0;
            float sum_migrant_stress = 0.0f;
            bool sampled_first = false;

            g_ecs->each([&](const oasis::migration::LegacyMigrantComponent& mig,
                            const oasis::fabrication::SomaticStateComponent& m_som,
                            const oasis::psychology::PsychologyComponent& m_psy) {
                (void)m_som;
                total_npcs++;
                if (mig.trust_ring_tier == 6u) {
                    r6_count++;
                } else if (mig.trust_ring_tier == 3u) {
                    r3_count++;
                } else if (mig.trust_ring_tier <= 1u) {
                    r1_count++;
                }
                if (!sampled_first || mig.trust_ring_tier == 6u) {
                    shelter_ok = mig.checklist.enclosed_shelter_ok;
                    hydration_ok = mig.checklist.hydration_ok;
                    calories_ok = mig.checklist.calories_ok;
                    sampled_first = true;
                }
                max_potw_mj = std::max(max_potw_mj, mig.potw_sweat_equity_mj);
                max_shelter_days = std::max(max_shelter_days, mig.sheltered_days_consecutive);
                max_r3_days = std::max(max_r3_days, mig.days_in_r3_consecutive);
                combined_mindset_flags |= mig.mindset_flags;
                sum_migrant_stress += m_psy.stress;
            });

            const double avg_migrant_stress = (total_npcs > 0u)
                ? static_cast<double>(sum_migrant_stress / static_cast<float>(total_npcs))
                : 0.0;

            const bool pre_wave_window = (g_migration_staging.staged_overflow_migrants > 0u);

            EM_ASM({
                if (typeof window !== 'undefined') {
                    var stateObj = new Object();
                    stateObj.totalNpcs = $0;
                    stateObj.npcCount = $0;
                    stateObj.maxNpcs = 16;
                    stateObj.r6Count = $1;
                    stateObj.r3Count = $2;
                    stateObj.r1Count = $3;
                    stateObj.stagedOverflow = $4;
                    stateObj.shelterOk = Boolean($5);
                    stateObj.hydrationOk = Boolean($6);
                    stateObj.caloriesOk = Boolean($7);
                    stateObj.potwMj = Number($8);
                    stateObj.shelteredDays = $9;
                    stateObj.r3Days = $10;
                    stateObj.mindsetFlags = $11;
                    stateObj.avgStress = Number($12);
                    var warnStr = ($13 !== 0 && $4 > 0)
                        ? ("⚠️ HORIZON ALERT: " + $4 + " Staged Migrants Holding at Sidewalk Perimeter (Parcel Cap 16/16)")
                        : "";
                    var leadStr = ($0 > 0)
                        ? ("Layer 7 Sidewalk Neighbors (" + $0 + "/16 NPCs | Minimum Grace Only — Zero Task Assistance)")
                        : "Sidewalk Quiet (Layer 7 Cul-de-sac)";
                    var regStr = (($11 & 14) !== 0)
                        ? "⚠️ LEGACY REGRESSION: Ration Hoarding (3x) & Reserve Power Draw (-2.5 kW)!"
                        : "";
                    stateObj.warningText = warnStr;
                    stateObj.leadTitle = leadStr;
                    stateObj.regressionText = regStr;
                    window._oasisLastMigrantHUD = stateObj;
                    var fn = window.updateMigrantHUD || window._oasisUpdateMigrantHUD;
                    if (typeof fn === 'function') {
                        fn($0, 16, $1, $2, $3, warnStr, leadStr, $5, $6, $7, $8, regStr);
                    }
                }
            }, static_cast<int>(total_npcs),
               static_cast<int>(r6_count),
               static_cast<int>(r3_count),
               static_cast<int>(r1_count),
               static_cast<int>(g_migration_staging.staged_overflow_migrants),
               shelter_ok ? 1 : 0,
               hydration_ok ? 1 : 0,
               calories_ok ? 1 : 0,
               static_cast<double>(max_potw_mj),
               static_cast<int>(max_shelter_days),
               static_cast<int>(max_r3_days),
               static_cast<int>(combined_mindset_flags),
               avg_migrant_stress,
               pre_wave_window ? 1 : 0);

            // Sprint 14: Sync Asynchronous Node Proxies & Multi-Day BPMN Temporal Schedule HUD
            // Note: Emscripten EM_ASM supports at most 16 arguments ($0..$15), so split into two calls:
            EM_ASM({
                if (typeof window !== 'undefined') {
                    window._oasisBpmnProxyCache = Array.of($0, $1, $2, $3, $4, $5, $6);
                }
            }, static_cast<int>(g_collective_net.collective_value_tokens),
               g_collective_net.peer_nodes[0].exergy_balance_kwh,
               g_collective_net.peer_nodes[0].bounty_fulfilled ? 1 : 0,
               g_collective_net.peer_nodes[1].water_balance_l,
               g_collective_net.peer_nodes[1].bounty_fulfilled ? 1 : 0,
               g_collective_net.peer_nodes[2].caloric_balance_kcal,
               g_collective_net.peer_nodes[2].bounty_fulfilled ? 1 : 0);

            EM_ASM({
                if (typeof window !== 'undefined' && typeof window._oasisUpdateBpmnHUD === 'function') {
                    var c = window._oasisBpmnProxyCache || Array.of(25, -4.5, 0, -8.0, 0, -1600.0, 0);
                    window._oasisUpdateBpmnHUD(
                        c[0],
                        c[1], c[2],
                        c[3], c[4],
                        c[5], c[6],
                        $0, $1,
                        $2, $3,
                        $4, $5,
                        $6, $7,
                        $8, $9
                    );
                }
            }, static_cast<int>(g_temporal_schedule.days[0].task_id),
               static_cast<int>(g_temporal_schedule.days[0].bpmn_stage),
               static_cast<int>(g_temporal_schedule.days[1].task_id),
               static_cast<int>(g_temporal_schedule.days[1].bpmn_stage),
               static_cast<int>(g_temporal_schedule.days[2].task_id),
               static_cast<int>(g_temporal_schedule.days[2].bpmn_stage),
               static_cast<int>(g_temporal_schedule.days[3].task_id),
               static_cast<int>(g_temporal_schedule.days[3].bpmn_stage),
               g_temporal_schedule.auto_dispatch_enabled ? 1 : 0,
               static_cast<int>(oasis::orchestrator::ComputeSimDayFromTick(g_sim_tick)));
        }
    }
}
#endif

int main(int argc, char* argv[]) {
    const auto cli_opts = oasis::time::ParseEngineCliOptions(argc, argv);
    const char* env_headless = std::getenv("OASIS_HEADLESS_CI");
    const bool is_headless_ci = cli_opts.headless_ci || (env_headless != nullptr && std::string_view(env_headless) == "1");
    g_is_headless_ci = is_headless_ci;

    spdlog::info("Oasis V4 Engine Starting... (headless_ci={})", is_headless_ci);
    spdlog::set_level(spdlog::level::info);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::error("SDL_Init Error: {}", SDL_GetError());
        return 1;
    }

    constexpr uint32_t kWidth = 1024;
    constexpr uint32_t kHeight = 720;

    SDL_Window* window = SDL_CreateWindow(
        "Oasis V4",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kWidth, kHeight,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        spdlog::error("SDL_CreateWindow Error: {}", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialize elevated colony-sim boot camera overlooking Lot 402 House (cutaway Z=3) & Backyard
    g_arcball_camera.has_poi_lock = true;
    g_arcball_camera.poi_origin = glm::vec3(14.5f, 1.0f, 6.5f);
    g_arcball_camera.target_poi = glm::vec3(14.5f, 1.0f, 6.5f);
    g_arcball_camera.theta_rad = -0.62f;
    g_arcball_camera.target_theta_rad = -0.62f;
    g_arcball_camera.phi_rad = 0.72f;
    g_arcball_camera.target_phi_rad = 0.72f;
    g_arcball_camera.radius = 18.0f;
    g_arcball_camera.target_radius = 18.0f;
    g_arcball_camera.z_cutaway_level = 3;
    g_arcball_camera.manual_cutaway_override = true;

    // Allocate Flecs world on the heap to prevent stack deallocation / UAF
    g_ecs = new flecs::world();
    g_ecs->set_target_fps(60.0f);

    // Register psychological components
    g_ecs->component<oasis::psychology::PersonalityFacets>();
    g_ecs->component<oasis::psychology::PsychologyComponent>();

    // Register fabrication, somatic, loose item, stockpile, and EPIC-9 Chill Loop components
    g_ecs->component<oasis::fabrication::SomaticStateComponent>();
    g_ecs->component<oasis::fabrication::SpatialPositionComponent>();
    g_ecs->component<oasis::fabrication::FounderCommandComponent>();
    g_ecs->component<oasis::fabrication::BlueprintComponent>();
    g_ecs->component<oasis::fabrication::GardenCropComponent>();
    g_ecs->component<oasis::pathfinding::VoxelPathComponent>();
    g_ecs->component<oasis::fabrication::VoxelPlacedComponent>();
    g_ecs->component<oasis::fabrication::LooseItemComponent>();
    g_ecs->component<oasis::fabrication::StockpileZoneComponent>();
    g_ecs->component<oasis::chill::EnclosureCacheComponent>();
    g_ecs->component<oasis::chill::GenesisCampStockComponent>();
    g_ecs->component<oasis::chill::BurnoutStateComponent>();
    g_ecs->component<oasis::migration::LegacyMigrantComponent>();

    // Seed Lot 402 Home Fixtures & Backyard Patio Hearth (STORY-9.2)
    oasis::chill::BootstrapGenesisSurvivalCamp(*g_ecs);

    oasis::psychology::PsychologyComponent founder_psych{};
    founder_psych.facets = {0.8f, 0.9f, 0.6f, 0.7f, 0.3f};
    founder_psych.stress = 0.25f;
    founder_psych.mood = 0.50f;
    founder_psych.focus = 0.75f;

    // Seed initial Genesis Awakening formative memory (tick 0)
    oasis::psychology::EpisodicMemoryNode m0{};
    m0.timestamp_tick = 0;
    m0.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::GENESIS_AWAKENING);
    m0.emotional_valence = 40;
    m0.salience = 65;
    m0.flags = oasis::psychology::MemoryFlags::NONE;
    std::strncpy(m0.description, "Genesis", sizeof(m0.description) - 1);
    uint32_t initial_slot = founder_psych.memory_ring.Push(m0);

    spdlog::info("[Oasis Cognitive Core] Founder Memory: tick={} event={} salience={} stress={:.2f} (Slot {}/32, Permanent={})",
                 0ULL, m0.description, m0.salience, founder_psych.stress, initial_slot, founder_psych.memory_ring.permanent_count);

    oasis::fabrication::SomaticStateComponent founder_somatic{};
    founder_somatic.caloric_reserve = 2500.0;
    founder_somatic.focus = 1.0f;
    founder_somatic.stamina = 1.0f;
    founder_somatic.is_exhausted = false;

    oasis::fabrication::SpatialPositionComponent founder_pos{};
    founder_pos.pos = glm::vec3(13.0f, 1.0f, 6.0f); // Inside Lot 402 Study right beside Node 0 Server Rack (12, 1, 6)
    founder_pos.move_speed = 3.0f;

    oasis::fabrication::FounderCommandComponent founder_cmd{};
    founder_cmd.state = oasis::fabrication::FounderActionState::IDLE;

    g_ecs->entity("Founder")
        .set<oasis::psychology::PersonalityFacets>(founder_psych.facets)
        .set<oasis::psychology::PsychologyComponent>(founder_psych)
        .set<oasis::fabrication::SomaticStateComponent>(founder_somatic)
        .set<oasis::fabrication::SpatialPositionComponent>(founder_pos)
        .set<oasis::fabrication::FounderCommandComponent>(founder_cmd)
        .set<oasis::pathfinding::VoxelPathComponent>({})
        .set<oasis::chill::EnclosureCacheComponent>({})
        .set<oasis::chill::BurnoutStateComponent>({});

    // Spawn 2 ambient Layer 7 Sidewalk Neighbors strolling along the front sidewalk (x = 4..5)
    if (!is_headless_ci) {
        oasis::uhai::MigrationWaveArrivalEvent l7_neighbors{};
        l7_neighbors.timestamp_hlc = 0ULL;
        l7_neighbors.wave_id = 1u;
        l7_neighbors.deterministic_seed = 402u;
        l7_neighbors.legacy_distress_index = 0.25f;
        l7_neighbors.initial_stress_mean = 0.45f;
        l7_neighbors.caloric_deficit_kcal = 0.0f;
        l7_neighbors.cohort_size = 2u;
        l7_neighbors.initial_trust_ring = 6u;
        l7_neighbors.trigger_reason_code = 0u;
        oasis::migration::SpawnMigrationWaveInOasis(*g_ecs, l7_neighbors, g_migration_staging, g_time_warp);
        oasis::migration::SyncMigrantAvatarVoxels(*g_ecs);
    }

    // Register FounderMovementSystem in flecs::OnUpdate phase
    g_ecs->system<oasis::fabrication::SpatialPositionComponent,
                  oasis::fabrication::FounderCommandComponent,
                  oasis::fabrication::SomaticStateComponent>("FounderMovementSystem")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e,
                 oasis::fabrication::SpatialPositionComponent& pos,
                 oasis::fabrication::FounderCommandComponent& cmd,
                 oasis::fabrication::SomaticStateComponent& somatic) {
            if (cmd.state != oasis::fabrication::FounderActionState::NAVIGATING &&
                cmd.state != oasis::fabrication::FounderActionState::HAULING_TO_SOURCE &&
                cmd.state != oasis::fabrication::FounderActionState::HAULING_TO_SITE &&
                cmd.state != oasis::fabrication::FounderActionState::SEEKING_FOOD &&
                cmd.state != oasis::fabrication::FounderActionState::SEEKING_WATER &&
                cmd.state != oasis::fabrication::FounderActionState::HARVESTING_CROP &&
                cmd.state != oasis::fabrication::FounderActionState::COOKING_MEAL) {
                return;
            }

            constexpr float dt = 1.0f / 60.0f;

            // Headless CI 60-tick path preservation
            if (g_is_headless_ci) {
                glm::vec3 target_pos = glm::vec3(static_cast<float>(cmd.target_voxel.x), pos.pos.y, static_cast<float>(cmd.target_voxel.z));
                glm::vec3 to_target = target_pos - pos.pos;
                float dist = glm::length(to_target);
                if (dist > 1.5f) {
                    glm::vec3 dir = to_target / dist;
                    pos.pos += dir * pos.move_speed * dt;
                    pos.pos.y = 1.0f;
                    somatic.caloric_reserve = std::max(0.0, somatic.caloric_reserve - 0.10 * dt);
                } else {
                    cmd.state = oasis::fabrication::FounderActionState::CONSTRUCTING;
                    spdlog::info("[Oasis Movement] Founder arrived at construction site ({},{},{}). Starting fabrication.",
                                 cmd.target_voxel.x, cmd.target_voxel.y, cmd.target_voxel.z);
                }
                return;
            }

            const float metabolic_mult = oasis::chill::EvaluateMetabolicSpeedMultiplier(somatic);
            const float effective_speed = pos.move_speed * metabolic_mult;

            if (!e.has<oasis::pathfinding::VoxelPathComponent>()) {
                e.set<oasis::pathfinding::VoxelPathComponent>({});
            }
            auto& path = e.get_mut<oasis::pathfinding::VoxelPathComponent>();

            if (path.waypoints.empty() || path.target_goal != cmd.target_voxel) {
                const glm::ivec3 start_vox(
                    static_cast<int>(std::round(pos.pos.x)),
                    std::clamp(static_cast<int>(std::round(pos.pos.y)), 1, 6),
                    static_cast<int>(std::round(pos.pos.z))
                );
                path = oasis::pathfinding::ComputeVoxelPath(start_vox, cmd.target_voxel);
            }

            const bool arrived = oasis::pathfinding::StepAgentAlongVoxelPath(
                pos.pos, path, effective_speed, dt
            );
            somatic.caloric_reserve = std::max(0.0, somatic.caloric_reserve - 0.10 * dt);
            if (!arrived) {
                return;
            }

            auto world = e.world();
            auto camp = world.entity("GenesisCamp");

            if (cmd.state == oasis::fabrication::FounderActionState::HAULING_TO_SOURCE) {
                if (cmd.target_blueprint.is_alive() &&
                    cmd.target_blueprint.has<oasis::fabrication::BlueprintComponent>() &&
                    camp.is_alive() &&
                    camp.has<oasis::chill::GenesisCampStockComponent>()) {
                    auto& bp = cmd.target_blueprint.get_mut<oasis::fabrication::BlueprintComponent>();
                    auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
                    if (oasis::chill::ConsumeRequiredMaterialFromStock(stock, bp.required_material_kind)) {
                        g_founder_is_carrying = true;
                        cmd.state = oasis::fabrication::FounderActionState::HAULING_TO_SITE;
                        cmd.target_voxel = glm::ivec3(bp.x, bp.y, bp.z);
                        const glm::ivec3 start_vox(
                            static_cast<int>(std::round(pos.pos.x)),
                            std::clamp(static_cast<int>(std::round(pos.pos.y)), 1, 6),
                            static_cast<int>(std::round(pos.pos.z))
                        );
                        path = oasis::pathfinding::ComputeVoxelPath(start_vox, cmd.target_voxel);
                        SetLayer7NeighborMilestoneRemark(
                            world, oasis::migration::Layer7CommentaryEvent::SALVAGE_CRATE_HAUL
                        );
                        spdlog::info("[Oasis Hauling] Picked up {} from Salvage Crate. Hauling to ({},{},{}).",
                                     oasis::chill::FormatMaterialKindName(bp.required_material_kind),
                                     bp.x, bp.y, bp.z);
                    } else {
                        g_founder_is_carrying = false;
                        cmd.state = oasis::fabrication::FounderActionState::IDLE;
                    }
                } else {
                    g_founder_is_carrying = false;
                    cmd.state = oasis::fabrication::FounderActionState::IDLE;
                }
            } else if (cmd.state == oasis::fabrication::FounderActionState::HAULING_TO_SITE) {
                g_founder_is_carrying = false;
                if (cmd.target_blueprint.is_alive() &&
                    cmd.target_blueprint.has<oasis::fabrication::BlueprintComponent>()) {
                    auto& bp = cmd.target_blueprint.get_mut<oasis::fabrication::BlueprintComponent>();
                    bp.materials_delivered = true;
                    bp.build_stage = std::max<uint8_t>(bp.build_stage, 1u);
                    cmd.state = oasis::fabrication::FounderActionState::CONSTRUCTING;
                    spdlog::info("[Oasis Hauling] Delivered materials to ({},{},{}). Starting Stage 1 fabrication.",
                                 bp.x, bp.y, bp.z);
                } else {
                    cmd.state = oasis::fabrication::FounderActionState::IDLE;
                }
            } else if (cmd.state == oasis::fabrication::FounderActionState::SEEKING_WATER) {
                if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                    auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
                    if (stock.potable_water_liters > 0.0) {
                        const double drink_l = std::min(1.5, stock.potable_water_liters);
                        stock.potable_water_liters -= drink_l;
                        somatic.hydration = std::min(1.0f, somatic.hydration + 0.50f);
                        spdlog::info("[Oasis Kitchen] Founder drank {:.1f}L filtered water at Kitchen Sink (Hydration={:.0f}%).",
                                     drink_l, somatic.hydration * 100.0f);
                    }
                }
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
            } else if (cmd.state == oasis::fabrication::FounderActionState::SEEKING_FOOD) {
                if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>() &&
                    e.has<oasis::psychology::PsychologyComponent>()) {
                    auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
                    auto& psych = e.get_mut<oasis::psychology::PsychologyComponent>();
                    if (!oasis::chill::CookHomesteadMealAtKitchen(somatic, psych, stock)) {
                        if (stock.ration_meals_remaining > 0u) {
                            stock.ration_meals_remaining -= 1u;
                            somatic.caloric_reserve = std::min(2500.0, somatic.caloric_reserve + 800.0);
                            psych.mood = std::min(1.0f, psych.mood + 0.08f);
                            psych.stress = std::max(0.0f, psych.stress - 0.04f);
                            spdlog::info("[Oasis Kitchen] Founder ate 800 kcal ration meal at Kitchen Pantry.");
                        }
                    } else {
                        SetLayer7NeighborMilestoneRemark(
                            world, oasis::migration::Layer7CommentaryEvent::COOKED_HOMESTEAD_MEAL
                        );
                        oasis::orchestrator::CompleteScheduledTask(g_temporal_schedule, 3u);
                        if (oasis::orchestrator::FulfillBpmnBountyForTask(g_collective_net, 3u, stock)) {
                            g_last_neighbor_remark =
                                "Node 03 (Soren @ Lot 408) via L4 BPMN: 'Warm Sovereign Kitchen meals received! Settled +20 L3 ValueTokens & sent +1 Solar PV Kit.'";
                        }
                        spdlog::info("[Oasis Sovereign Kitchen] Cooked Homestead Meal (+800 kcal, +20% Hydration, +0.15 Mood).");
                    }
                }
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
            } else if (cmd.state == oasis::fabrication::FounderActionState::COOKING_MEAL) {
                g_founder_is_carrying = false;
                if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>() &&
                    e.has<oasis::psychology::PsychologyComponent>()) {
                    auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
                    auto& psych = e.get_mut<oasis::psychology::PsychologyComponent>();
                    if (oasis::chill::CookHomesteadMealAtKitchen(somatic, psych, stock)) {
                        SetLayer7NeighborMilestoneRemark(
                            world, oasis::migration::Layer7CommentaryEvent::COOKED_HOMESTEAD_MEAL
                        );
                        oasis::orchestrator::CompleteScheduledTask(g_temporal_schedule, 3u);
                        if (oasis::orchestrator::FulfillBpmnBountyForTask(g_collective_net, 3u, stock)) {
                            g_last_neighbor_remark =
                                "Node 03 (Soren @ Lot 408) via L4 BPMN: 'Warm Sovereign Kitchen meals received! Settled +20 L3 ValueTokens & sent +1 Solar PV Kit.'";
                        }
                        spdlog::info("[Oasis Sovereign Kitchen] Cooked Homestead Meal (+2 Meals, +800 kcal, +20% Hydration, +0.15 Mood).");
                    }
                }
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
            } else if (cmd.state == oasis::fabrication::FounderActionState::HARVESTING_CROP) {
                bool harvested = false;
                world.each([&](flecs::entity, oasis::fabrication::GardenCropComponent& crop) {
                    if (!harvested && crop.is_mature &&
                        (crop.pos() == cmd.target_voxel ||
                         glm::length(glm::vec3(crop.pos()) - pos.pos) <= 2.5f)) {
                        if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                            auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
                            stock.raw_produce_items += 2;
                            stock.seed_packs_remaining += 1;
                        }
                        crop.growth_ticks = 0.0f;
                        crop.is_mature = false;
                        oasis::graphics::SetDynamicVoxel(crop.x, crop.y, crop.z, oasis::chill::MAT_TILLED_BED);
                        harvested = true;
                        spdlog::info("[Oasis Sovereign Kitchen] Harvested Mature Garden Bed at ({},{},{}): +2 Produce, +1 Seed Pack.",
                                     crop.x, crop.y, crop.z);
                    }
                });
                if (!harvested &&
                    oasis::graphics::GetVoxelMaterialAt(cmd.target_voxel) == oasis::chill::MAT_MATURE_GARDEN) {
                    if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                        auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
                        stock.raw_produce_items += 2;
                        stock.seed_packs_remaining += 1;
                    }
                    oasis::graphics::SetDynamicVoxel(cmd.target_voxel.x, cmd.target_voxel.y, cmd.target_voxel.z, oasis::chill::MAT_TILLED_BED);
                    harvested = true;
                }
                if (harvested && camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                    const auto& stock = camp.get<oasis::chill::GenesisCampStockComponent>();
                    g_founder_is_carrying = true;
                    cmd.state = oasis::fabrication::FounderActionState::COOKING_MEAL;
                    cmd.target_voxel = stock.ration_crate_pos;
                    const glm::ivec3 start_vox(
                        static_cast<int>(std::round(pos.pos.x)),
                        std::clamp(static_cast<int>(std::round(pos.pos.y)), 1, 6),
                        static_cast<int>(std::round(pos.pos.z))
                    );
                    path = oasis::pathfinding::ComputeVoxelPath(start_vox, cmd.target_voxel);
                    spdlog::info("[Oasis Sovereign Kitchen] Hauling harvested produce to Kitchen Counter (11,1,4) to cook Homestead Meals.");
                } else {
                    cmd.state = oasis::fabrication::FounderActionState::IDLE;
                }
            } else if (cmd.state == oasis::fabrication::FounderActionState::NAVIGATING) {
                if (cmd.target_blueprint.is_alive() &&
                    cmd.target_blueprint.has<oasis::fabrication::BlueprintComponent>()) {
                    const auto& bp = cmd.target_blueprint.get<oasis::fabrication::BlueprintComponent>();
                    if (!bp.requires_material_haul || bp.materials_delivered) {
                        cmd.state = oasis::fabrication::FounderActionState::CONSTRUCTING;
                        spdlog::info("[Oasis Movement] Founder arrived at construction site ({},{},{}). Starting fabrication.",
                                      cmd.target_voxel.x, cmd.target_voxel.y, cmd.target_voxel.z);
                    } else {
                        cmd.state = oasis::fabrication::FounderActionState::IDLE;
                    }
                } else {
                    cmd.state = oasis::fabrication::FounderActionState::IDLE;
                }
            }
        });

    // Register ConstructionSystem in flecs::OnUpdate phase
    g_ecs->system<oasis::fabrication::SpatialPositionComponent,
                  oasis::fabrication::FounderCommandComponent,
                  oasis::fabrication::SomaticStateComponent,
                  oasis::psychology::PsychologyComponent>("ConstructionSystem")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e,
                 oasis::fabrication::SpatialPositionComponent& pos,
                 oasis::fabrication::FounderCommandComponent& cmd,
                 oasis::fabrication::SomaticStateComponent& somatic,
                 oasis::psychology::PsychologyComponent& psych) {
            const bool is_burned_out = e.has<oasis::chill::BurnoutStateComponent>() &&
                                       e.get<oasis::chill::BurnoutStateComponent>().is_burned_out;

            auto world = e.world();
            auto camp = world.entity("GenesisCamp");

            auto try_dispatch_next_blueprint = [&]() {
                if (is_burned_out || somatic.is_exhausted || somatic.caloric_reserve <= 500.0 || psych.focus <= 0.10f) {
                    return;
                }
                flecs::entity nearest_bp{};
                float best_dist_sq = 1e30f;
                glm::ivec3 best_pos{0};
                bool blocked_found = false;
                uint8_t blocked_mat_kind = 0u;

                world.each([&](flecs::entity bp_ent, oasis::fabrication::BlueprintComponent& candidate_bp) {
                    if (!candidate_bp.is_completed) {
                        if (!g_is_headless_ci &&
                            candidate_bp.requires_material_haul &&
                            !candidate_bp.materials_delivered &&
                            camp.is_alive() &&
                            camp.has<oasis::chill::GenesisCampStockComponent>()) {
                            const auto& stock = camp.get<oasis::chill::GenesisCampStockComponent>();
                            if (!oasis::chill::HasRequiredMaterialInStock(stock, candidate_bp.required_material_kind)) {
                                blocked_found = true;
                                blocked_mat_kind = candidate_bp.required_material_kind;
                                return;
                            }
                        }
                        const glm::vec3 bp_vec(
                            static_cast<float>(candidate_bp.x),
                            static_cast<float>(candidate_bp.y),
                            static_cast<float>(candidate_bp.z)
                        );
                        const glm::vec3 diff = bp_vec - pos.pos;
                        const float d2 = glm::dot(diff, diff);
                        if (d2 < best_dist_sq) {
                            best_dist_sq = d2;
                            nearest_bp = bp_ent;
                            best_pos = glm::ivec3(candidate_bp.x, candidate_bp.y, candidate_bp.z);
                        }
                    }
                });

                if (nearest_bp.is_alive()) {
                    g_blocked_blueprint_status_note.clear();
                    cmd.target_blueprint = nearest_bp;
                    const auto& bp_ref = nearest_bp.get<oasis::fabrication::BlueprintComponent>();
                    if (!g_is_headless_ci && bp_ref.requires_material_haul && !bp_ref.materials_delivered &&
                        camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                        const auto& stock = camp.get<oasis::chill::GenesisCampStockComponent>();
                        DispatchFounderPathTo(
                            world,
                            stock.salvage_crate_pos,
                            oasis::fabrication::FounderActionState::HAULING_TO_SOURCE,
                            nearest_bp
                        );
                    } else if (g_is_headless_ci) {
                        cmd.state = oasis::fabrication::FounderActionState::NAVIGATING;
                        cmd.target_voxel = best_pos;
                    } else {
                        DispatchFounderPathTo(
                            world,
                            best_pos,
                            oasis::fabrication::FounderActionState::NAVIGATING,
                            nearest_bp
                        );
                    }
                } else if (blocked_found) {
                    char buf[160]{};
                    std::snprintf(
                        buf,
                        sizeof(buf),
                        "Awaiting %s (0 in Salvage Crate)",
                        oasis::chill::FormatMaterialKindName(blocked_mat_kind)
                    );
                    g_blocked_blueprint_status_note = buf;
                } else {
                    g_blocked_blueprint_status_note.clear();
                }
            };

            if (cmd.state == oasis::fabrication::FounderActionState::IDLE) {
                try_dispatch_next_blueprint();
                return;
            }

            if (cmd.state != oasis::fabrication::FounderActionState::CONSTRUCTING) return;
            if (!cmd.target_blueprint.is_alive() || !cmd.target_blueprint.has<oasis::fabrication::BlueprintComponent>()) {
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
                try_dispatch_next_blueprint();
                return;
            }

            auto& bp = cmd.target_blueprint.get_mut<oasis::fabrication::BlueprintComponent>();
            if (bp.is_completed) {
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
                try_dispatch_next_blueprint();
                return;
            }

            // Guard against constructing an un-delivered resource-gated blueprint in interactive mode
            if (!g_is_headless_ci && bp.requires_material_haul && !bp.materials_delivered) {
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
                try_dispatch_next_blueprint();
                return;
            }

            // Caloric exhaustion guard (<= 500.0 kcal or focus <= 0.05)
            if (somatic.caloric_reserve <= 500.0 || psych.focus <= 0.05f) {
                somatic.is_exhausted = true;
                cmd.state = oasis::fabrication::FounderActionState::EXHAUSTED;
                spdlog::warn("[Oasis Fabrication] Founder exhausted! Calories={:.1f}, Focus={:.2f}. Halting construction.",
                             somatic.caloric_reserve, psych.focus);
                oasis::psychology::EpisodicMemoryNode exhaust_mem{};
                exhaust_mem.timestamp_tick = g_sim_tick;
                exhaust_mem.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::TOOL_DROPPED);
                exhaust_mem.salience = 60;
                exhaust_mem.emotional_valence = -50;
                std::strncpy(exhaust_mem.description, "Exhaust", sizeof(exhaust_mem.description) - 1);
                psych.memory_ring.Push(exhaust_mem);
                return;
            }

            const float metabolic_mult = g_is_headless_ci
                ? 1.0f
                : oasis::chill::EvaluateMetabolicSpeedMultiplier(somatic);

            // Incremental fabrication progress scaled by metabolic multiplier
            constexpr float dt = 1.0f / 60.0f;
            const double work_rate = 60.0 * static_cast<double>(metabolic_mult);
            double delta_w = std::min(work_rate * dt, bp.required_work_ticks - bp.invested_work_ticks);
            double progress_fraction = (bp.required_work_ticks > 0.0) ? (delta_w / bp.required_work_ticks) : 1.0;

            double cal_burn = bp.caloric_cost_total * progress_fraction;
            float focus_drain = bp.focus_cost * static_cast<float>(progress_fraction);

            somatic.caloric_reserve = std::max(0.0, somatic.caloric_reserve - cal_burn);
            psych.focus = std::max(0.0f, psych.focus - focus_drain);
            somatic.focus = psych.focus;
            somatic.stamina = std::max(0.0f, somatic.stamina - 0.005f);
            bp.invested_work_ticks += delta_w;

            // Multi-Stage Visual Progression: Stage 1 (<50%) -> Stage 2 (>=50% Timber Scaffolding Frame 33u for structures)
            if (!g_is_headless_ci &&
                bp.required_work_ticks > 0.0 &&
                bp.invested_work_ticks >= 0.5 * bp.required_work_ticks &&
                bp.build_stage < 2u) {
                bp.build_stage = 2u;
                if (bp.type != oasis::fabrication::InfrastructureType::SWALE &&
                    bp.type != oasis::fabrication::InfrastructureType::COVER_CROP) {
                    oasis::graphics::SetDynamicVoxel(bp.x, bp.y, bp.z, oasis::chill::MAT_STAGE_FRAME);
                }
            }

            if (bp.invested_work_ticks >= bp.required_work_ticks) {
                bp.is_completed = true;
                bp.build_stage = 3u;
                g_founder_is_carrying = false;
                cmd.state = oasis::fabrication::FounderActionState::IDLE;

                auto placed_type = bp.type;
                int px = bp.x, py = bp.y, pz = bp.z;
                cmd.target_blueprint.destruct();

                world.entity()
                    .set<oasis::fabrication::VoxelPlacedComponent>({placed_type, px, py, pz, g_sim_tick});

                uint8_t mat_id = 12;
                switch (placed_type) {
                    case oasis::fabrication::InfrastructureType::SOLAR_PANEL:         mat_id = 11; break;
                    case oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT: mat_id = 12; break;
                    case oasis::fabrication::InfrastructureType::WALL_COB:            mat_id = 13; break;
                    case oasis::fabrication::InfrastructureType::WALL_URBANITE:       mat_id = 14; break;
                    case oasis::fabrication::InfrastructureType::DOOR_TIMBER:         mat_id = 15; break;
                    case oasis::fabrication::InfrastructureType::ROCKET_MASS_HEATER:  mat_id = 16; break;
                    case oasis::fabrication::InfrastructureType::ROOF_TIMBER:         mat_id = 17; break;
                    case oasis::fabrication::InfrastructureType::SWALE:               mat_id = 18; break;
                    case oasis::fabrication::InfrastructureType::COVER_CROP:          mat_id = 19; break;
                    case oasis::fabrication::InfrastructureType::HAUL_ZONE:           mat_id = 20; break;
                    case oasis::fabrication::InfrastructureType::SLEEP_ZONE:          mat_id = 21; break;
                    case oasis::fabrication::InfrastructureType::RECREATION_ZONE:     mat_id = 22; break;
                    default:                                                          mat_id = 12; break;
                }
                oasis::graphics::SetDynamicVoxel(px, py, pz, mat_id);

                if (e.has<oasis::chill::EnclosureCacheComponent>()) {
                    oasis::chill::MarkEnclosureDirtyOnVoxelChange(e.get_mut<oasis::chill::EnclosureCacheComponent>());
                }

                if (placed_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL) {
                    SetLayer7NeighborMilestoneRemark(
                        world, oasis::migration::Layer7CommentaryEvent::ROOF_SOLAR_PANEL
                    );
                    oasis::orchestrator::CompleteScheduledTask(g_temporal_schedule, 1u);
                    if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                        auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
                        if (oasis::orchestrator::FulfillBpmnBountyForTask(g_collective_net, 1u, stock)) {
                            g_last_neighbor_remark =
                                "Node 01 (Maya @ Lot 404) via L4 BPMN: 'Received +4.5 kWh solar exergy! Settled +15 L3 ValueTokens & sent +2 Biochar/Cob to your driveway.'";
                        }
                    }
                } else if (placed_type == oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT) {
                    SetLayer7NeighborMilestoneRemark(
                        world, oasis::migration::Layer7CommentaryEvent::RAINWATER_CISTERN
                    );
                } else if (placed_type == oasis::fabrication::InfrastructureType::SWALE) {
                    SetLayer7NeighborMilestoneRemark(
                        world, oasis::migration::Layer7CommentaryEvent::BACKYARD_SWALE
                    );
                    oasis::orchestrator::CompleteScheduledTask(g_temporal_schedule, 2u);
                    world.entity().set<oasis::fabrication::LooseItemComponent>({
                        oasis::fabrication::MAT_CLAY_COB_LUMP,
                        px, py, pz, 1u, false, false
                    });
                    oasis::fabrication::HaulLooseItemsToStockpile(world);
                    if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                        auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();
                        stock.cob_earth_lumps += 2;
                        if (oasis::orchestrator::FulfillBpmnBountyForTask(g_collective_net, 2u, stock)) {
                            g_last_neighbor_remark =
                                "Node 02 (Kael @ Lot 406) via L4 BPMN: 'Swale catchment online (+8.0 L/d)! Settled +12 L3 ValueTokens & sent +2 Heirloom Seed Packs.'";
                        }
                        spdlog::info("[Oasis Fabrication] Swale excavation yielded +2 Cob Earth Lumps (Total: {})",
                                     stock.cob_earth_lumps);
                    }
                } else if (placed_type == oasis::fabrication::InfrastructureType::COVER_CROP) {
                    oasis::fabrication::GardenCropComponent crop{};
                    crop.x = px;
                    crop.y = py;
                    crop.z = pz;
                    crop.growth_ticks = 0.0f;
                    crop.required_growth_ticks = 300.0f;
                    crop.is_mature = false;
                    world.entity().set<oasis::fabrication::GardenCropComponent>(crop);
                }

                auto boundary = world.entity("SettlementBoundary");
                if (boundary.is_alive() && boundary.has<oasis::boundary::InfrastructureAutarkyComponent>()) {
                    auto& infra = boundary.get_mut<oasis::boundary::InfrastructureAutarkyComponent>();
                    if (placed_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL) {
                        infra.solar_stored_kwh += 4.5;
                    } else if (placed_type == oasis::fabrication::InfrastructureType::SWALE) {
                        infra.rain_stored_liters += 100.0;
                        infra.depaved_area_sqm += 1.0;
                    } else if (placed_type == oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT) {
                        infra.rain_stored_liters += 250.0;
                    }
                }

                oasis::psychology::EpisodicMemoryNode ach_mem{};
                ach_mem.timestamp_tick = g_sim_tick;
                ach_mem.salience = 85; // PERMANENT memory!
                ach_mem.emotional_valence = 75;
                ach_mem.flags = oasis::psychology::MemoryFlags::EUPHORIA;
                if (placed_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL) {
                    ach_mem.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::FIRST_KILOWATT_GENERATED);
                    std::strncpy(ach_mem.description, "SolarFab", sizeof(ach_mem.description) - 1);
                } else {
                    ach_mem.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::WATER_MAIN_TAPPED);
                    std::strncpy(ach_mem.description, "RainFab", sizeof(ach_mem.description) - 1);
                }
                psych.memory_ring.Push(ach_mem);
                psych.mood = std::min(1.0f, psych.mood + 0.15f);

                spdlog::info("[Oasis Fabrication] Infrastructure {} completed at ({},{},{})! Remaining Calories={:.1f}, Focus={:.2f}",
                             FormatBlueprintTypeName(placed_type),
                             px, py, pz, somatic.caloric_reserve, psych.focus);

                // Immediately dispatch next queued blueprint if Founder is ready
                try_dispatch_next_blueprint();
            }
        });

    // Register FounderPsychologySystem in flecs::OnUpdate phase
    g_ecs->system<oasis::psychology::PsychologyComponent>("FounderPsychologySystem")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e, oasis::psychology::PsychologyComponent& psych) {
            if (e != e.world().entity("Founder")) return;
            g_sim_tick++;
            const uint64_t tick = g_sim_tick;

            bool recorded = false;
            oasis::psychology::EpisodicMemoryNode node{};
            node.timestamp_tick = tick;

            // BUG-11.2a Fix: Guard synthetic demo memory script behind g_is_headless_ci
            // so interactive WebGPU sessions do not suffer unprovoked HOA/Freeze stress spirals.
            if (g_is_headless_ci) {
                if (tick == 1) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::MUNICIPAL_CODE_RAID);
                    node.emotional_valence = -90;
                    node.salience = 95; // salience >= 80 -> sets PERMANENT_MEMORY
                    node.flags = oasis::psychology::MemoryFlags::TRAUMA;
                    std::strncpy(node.description, "Raid", sizeof(node.description) - 1);
                    psych.stress = 0.65f;
                    psych.mood = 0.20f;
                    recorded = true;
                } else if (tick == 10) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::SIPHON_POWER_GRID);
                    node.emotional_valence = 50;
                    node.salience = 75;
                    std::strncpy(node.description, "Power", sizeof(node.description) - 1);
                    psych.stress = 0.55f;
                    psych.mood = 0.40f;
                    recorded = true;
                } else if (tick == 20) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::WATER_MAIN_TAPPED);
                    node.emotional_valence = 45;
                    node.salience = 65;
                    std::strncpy(node.description, "Water", sizeof(node.description) - 1);
                    psych.stress = 0.45f;
                    psych.mood = 0.50f;
                    recorded = true;
                } else if (tick == 25) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::FIRST_KILOWATT_GENERATED);
                    node.emotional_valence = 85;
                    node.salience = 90; // salience >= 80 -> sets PERMANENT_MEMORY
                    node.flags = oasis::psychology::MemoryFlags::EUPHORIA;
                    std::strncpy(node.description, "Solar", sizeof(node.description) - 1);
                    psych.stress = 0.30f;
                    psych.mood = 0.70f;
                    recorded = true;
                } else if (tick == 35) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::SOLAR_INVERTER_REPAIR);
                    node.emotional_valence = 30;
                    node.salience = 55;
                    std::strncpy(node.description, "Invert", sizeof(node.description) - 1);
                    psych.stress = 0.28f;
                    psych.mood = 0.72f;
                    recorded = true;
                } else if (tick == 45) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::TOOL_DROPPED);
                    node.emotional_valence = -15;
                    node.salience = 25;
                    std::strncpy(node.description, "Tool", sizeof(node.description) - 1);
                    psych.stress = 0.32f;
                    psych.mood = 0.68f;
                    recorded = true;
                } else if (tick == 55) {
                    node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::COMMUNAL_FEAST);
                    node.emotional_valence = 80;
                    node.salience = 85; // salience >= 80 -> sets PERMANENT_MEMORY
                    node.flags = oasis::psychology::MemoryFlags::EUPHORIA;
                    std::strncpy(node.description, "Feast", sizeof(node.description) - 1);
                    psych.stress = 0.22f;
                    psych.mood = 0.85f;
                    recorded = true;
                } else if (tick > 60 && (tick % 15 == 0)) {
                    uint32_t cycle = (tick / 15) % 5;
                    if (cycle == 0) {
                        node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::HOA_VIOLATION_NOTICE);
                        node.emotional_valence = -60;
                        node.salience = 82; // salience >= 80 -> sets PERMANENT_MEMORY
                        std::strncpy(node.description, "HOA", sizeof(node.description) - 1);
                        psych.stress = std::min(1.0f, psych.stress + 0.15f);
                    } else if (cycle == 1) {
                        node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::STRANGE_MOOD_MASTERWORK);
                        node.emotional_valence = 95;
                        node.salience = 88; // salience >= 80 -> sets PERMANENT_MEMORY
                        node.flags = oasis::psychology::MemoryFlags::EUPHORIA;
                        std::strncpy(node.description, "Master", sizeof(node.description) - 1);
                        psych.mood = std::min(1.0f, psych.mood + 0.20f);
                    } else if (cycle == 2) {
                        node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::FREEZING_WINTER_NIGHT);
                        node.emotional_valence = -40;
                        node.salience = 70;
                        std::strncpy(node.description, "Freeze", sizeof(node.description) - 1);
                        psych.stress = std::min(1.0f, psych.stress + 0.08f);
                    } else if (cycle == 3) {
                        node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::TOOL_DROPPED);
                        node.emotional_valence = -10;
                        node.salience = 20;
                        std::strncpy(node.description, "DropTool", sizeof(node.description) - 1);
                    } else {
                        node.event_type = static_cast<uint16_t>(oasis::psychology::MemoryEventType::SIPHON_POWER_GRID);
                        node.emotional_valence = 40;
                        node.salience = 60;
                        std::strncpy(node.description, "TapGrid", sizeof(node.description) - 1);
                        psych.stress = std::max(0.1f, psych.stress - 0.05f);
                    }
                    recorded = true;
                }
            }

            if (recorded) {
                uint32_t slot = psych.memory_ring.Push(node);
                const bool is_critical = (node.salience >= 80) || (tick <= 60);
                if (oasis::time::ShouldEmitThrottledLog(g_time_warp, tick, is_critical)) {
                    spdlog::info("[Oasis Cognitive Core] Founder Memory: tick={} event={} salience={} stress={:.2f} (Slot {}/32, Permanent={})",
                                 tick, node.description, node.salience, psych.stress, slot, psych.memory_ring.permanent_count);
                }
            }

            // Periodic diagnostic memory summary throttled via ShouldEmitThrottledLog
            if (tick == 60 || oasis::time::ShouldEmitThrottledLog(g_time_warp, tick, false)) {
                spdlog::info("[Oasis Cognitive Core] Diagnostic Summary: tick={} | Total Recorded: {} | Active Slots: {}/32 | Permanent: {} | Stress: {:.2f} | Mood: {:.2f} | Focus: {:.2f}",
                             tick, psych.memory_ring.total_recorded, psych.memory_ring.count, psych.memory_ring.permanent_count,
                             psych.stress, psych.mood, psych.focus);
            }
        });

    // Register boundary and flow components
    g_ecs->component<oasis::boundary::InfrastructureAutarkyComponent>();
    g_ecs->component<oasis::boundary::SocialMutualAidComponent>();
    g_ecs->component<oasis::boundary::PeerMeshComponent>();
    g_ecs->component<oasis::boundary::BoundaryInterfaceComponent>();
    g_ecs->component<oasis::flow::TemporalFlowContractComponent>();

    // Instantiate SettlementBoundary entity with default components
    g_ecs->entity("SettlementBoundary")
        .set<oasis::boundary::InfrastructureAutarkyComponent>({})
        .set<oasis::fabrication::SomaticStateComponent>({})
        .set<oasis::boundary::SocialMutualAidComponent>({})
        .set<oasis::boundary::PeerMeshComponent>({})
        .set<oasis::boundary::BoundaryInterfaceComponent>({})
        .set<oasis::flow::TemporalFlowContractComponent>({});

    // Register TemporalFlowSystem in flecs::OnUpdate phase (Dual Sprint 9 Anti-Bleed Invariant 3)
    // Consumes FlowContract packets from UHAI shared memory at 1x local sim_tick pace,
    // completely decoupling simulation time from remote agents running at 100x wall-clock speed.
    g_ecs->system<oasis::boundary::InfrastructureAutarkyComponent,
                  oasis::fabrication::SomaticStateComponent,
                  oasis::flow::TemporalFlowContractComponent>("TemporalFlowSystem")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e,
                 oasis::boundary::InfrastructureAutarkyComponent& infra,
                 oasis::fabrication::SomaticStateComponent& somatic,
                 oasis::flow::TemporalFlowContractComponent& flow) {
            // 1. Drain incoming FlowContract packets from UHAI flow ring
            oasis::uhai::FlowContract incoming{};
            while (g_flow_channel.Pop(incoming)) {
                flow.AddOrUpdateContract(incoming, g_sim_tick);
            }

            // 2. Resolve active contracts against current local tick g_sim_tick
            const double prev_cal = somatic.caloric_reserve;
            flow.StepTick(g_sim_tick, infra.solar_stored_kwh, infra.rain_stored_liters, somatic.caloric_reserve);
            const double net_flow_delta = somatic.caloric_reserve - prev_cal;

            // Only add non-zero incoming flow contract delta to Founder entity (never overwrite Founder's live caloric burn!)
            if (net_flow_delta != 0.0) {
                auto founder = e.world().entity("Founder");
                if (founder.is_alive() && founder != e && founder.has<oasis::fabrication::SomaticStateComponent>()) {
                    auto& founder_som = founder.get_mut<oasis::fabrication::SomaticStateComponent>();
                    founder_som.caloric_reserve = std::min(2500.0, founder_som.caloric_reserve + net_flow_delta);
                }
            }
        });

    // Register BoundaryInterfaceSystem in flecs::OnUpdate phase
    g_ecs->system<oasis::boundary::BoundaryInterfaceComponent,
                  const oasis::boundary::InfrastructureAutarkyComponent,
                  const oasis::boundary::SocialMutualAidComponent,
                  const oasis::boundary::PeerMeshComponent>("BoundaryInterfaceSystem")
        .kind(flecs::OnUpdate)
        .each([](oasis::boundary::BoundaryInterfaceComponent& b,
                 const oasis::boundary::InfrastructureAutarkyComponent& infra,
                 const oasis::boundary::SocialMutualAidComponent& social,
                 const oasis::boundary::PeerMeshComponent& mesh) {
            const double phi_infra = infra.EvaluatePhi();
            const double phi_social = social.EvaluatePhi();
            const double phi_mesh = mesh.EvaluatePhi();

            b.psi = oasis::boundary::CalculateSovereignAttenuation(
                phi_infra, phi_social, phi_mesh, b.alpha, b.beta, b.gamma, b.delta);

            b.lambda = oasis::boundary::CalculatePoissonArrivalIntensity(
                b.regime, b.psi, oasis::boundary::IRREDUCIBLE_THREAT_FLOOR);

            static oasis::time::TimeWarpState s_boundary_log_throttle{};
            if (oasis::time::ShouldEmitThrottledLog(s_boundary_log_throttle, g_sim_tick, false)) {
                spdlog::info("[Oasis Boundary Interface] Poisson arrival intensity lambda={:.6f} s^-1 (regime={}, Psi={:.4f}, Phi_infra={:.4f}, floor={:.4f})",
                             b.lambda, static_cast<int>(b.regime), b.psi, phi_infra,
                             oasis::boundary::IRREDUCIBLE_THREAT_FLOOR);
            }

            constexpr double dt = 1.0 / 60.0;
            b.countdown_timer -= dt;
            b.time_to_next_shock = b.countdown_timer;
            if (b.countdown_timer <= 0.0) {
                b.total_shocks++;
                switch (b.total_shocks % 4) {
                    case 1: b.last_shock_type = oasis::boundary::BoundaryShockType::MunicipalCodeRaid; break;
                    case 2: b.last_shock_type = oasis::boundary::BoundaryShockType::UtilityShutoff; break;
                    case 3: b.last_shock_type = oasis::boundary::BoundaryShockType::TaxLien; break;
                    case 0: b.last_shock_type = oasis::boundary::BoundaryShockType::EpaWaterInspection; break;
                }
                b.last_shock = b.last_shock_type;
                b.countdown_timer = (b.lambda > 0.0) ? (1.0 / b.lambda) : 50.0;
                b.time_to_next_shock = b.countdown_timer;
                spdlog::info("[Oasis Boundary Interface] Boundary shock event triggered: count={} lambda={:.6f} s^-1",
                             b.total_shocks, b.lambda);
            }
        });

    // Register FounderHomeostasisSystem in flecs::OnUpdate phase
    g_ecs->system<oasis::fabrication::SpatialPositionComponent,
                  oasis::fabrication::SomaticStateComponent,
                  oasis::psychology::PsychologyComponent>("FounderHomeostasisSystem")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e,
                 oasis::fabrication::SpatialPositionComponent& pos,
                 oasis::fabrication::SomaticStateComponent& somatic,
                 oasis::psychology::PsychologyComponent& psych) {
            if (e != e.world().entity("Founder")) return;
            constexpr float dt = 1.0f / 60.0f;
            constexpr float sim_hours_per_tick = 1.0f / static_cast<float>(oasis::chill::TICKS_PER_SIM_HOUR);

            // BUG-11.2b Fix: Pass canonical 28,800-tick diurnal cycle period (TICKS_PER_SIM_DAY)
            float amb_temp = oasis::homeostasis::ResolveVoxelAmbientTemperature(
                pos.pos, g_sim_tick, oasis::chill::TICKS_PER_SIM_DAY
            );

            const glm::ivec3 start_voxel(
                static_cast<int>(std::round(pos.pos.x)),
                static_cast<int>(std::round(pos.pos.y)),
                static_cast<int>(std::round(pos.pos.z))
            );

            // STORY-9.1: Dirty-Flagged 512-Voxel BFS Thermal Enclosure & Insulated Sleep
            oasis::chill::EnclosureStatus enc_status = oasis::chill::EnclosureStatus::UNENCLOSED;
            if (e.has<oasis::chill::EnclosureCacheComponent>()) {
                auto& enc_cache = e.get_mut<oasis::chill::EnclosureCacheComponent>();
                enc_status = oasis::chill::EvaluateThermalEnclosure(
                    enc_cache, start_voxel, amb_temp
                );
                if (enc_status != oasis::chill::EnclosureStatus::UNENCLOSED) {
                    amb_temp = enc_cache.interior_temp_c;
                }
            }

            // Indoor comfort moderation around Lot 402 Master Bedroom (11, 1, 8)
            const glm::vec3 tent_pos(11.0f, 1.0f, 8.0f);
            const float dist_to_tent = glm::length(tent_pos - pos.pos);
            if (!g_is_headless_ci && dist_to_tent <= 4.5f && enc_status == oasis::chill::EnclosureStatus::UNENCLOSED) {
                amb_temp = std::clamp(amb_temp, 18.0f, 26.0f);
            }

            oasis::homeostasis::StepHomeostasis(somatic, psych, amb_temp, dt);

            // STORY-9.2, STORY-9.3 & Sprint 13.6 Scenario Gamma Sovereign Kitchen Loop
            if (!g_is_headless_ci) {
                auto world = e.world();

                // Tick all planted GardenCropComponent entities toward maturity (300 ticks = 5s at 1x)
                world.each([&](oasis::fabrication::GardenCropComponent& crop) {
                    if (!crop.is_mature) {
                        crop.growth_ticks += 1.0f;
                        if (crop.growth_ticks >= crop.required_growth_ticks) {
                            crop.growth_ticks = crop.required_growth_ticks;
                            crop.is_mature = true;
                            oasis::graphics::SetDynamicVoxel(
                                crop.x, crop.y, crop.z, oasis::chill::MAT_MATURE_GARDEN
                            );
                        }
                    }
                });

                somatic.hydration = std::max(0.0f, somatic.hydration - oasis::homeostasis::BASAL_HYDRATION_DRAIN_PER_SEC * dt);

                auto camp = world.entity("GenesisCamp");
                if (camp.is_alive() && camp.has<oasis::chill::GenesisCampStockComponent>()) {
                    auto& stock = camp.get_mut<oasis::chill::GenesisCampStockComponent>();

                    const glm::vec3 water_filter_f(
                        static_cast<float>(stock.water_tote_pos.x),
                        static_cast<float>(stock.water_tote_pos.y),
                        static_cast<float>(stock.water_tote_pos.z)
                    );
                    const glm::vec3 pantry_f(
                        static_cast<float>(stock.ration_crate_pos.x),
                        static_cast<float>(stock.ration_crate_pos.y),
                        static_cast<float>(stock.ration_crate_pos.z)
                    );
                    const float dist_to_water = glm::length(water_filter_f - pos.pos);
                    const float dist_to_pantry = glm::length(pantry_f - pos.pos);

                    // Grounded Somatic Intake: only consume immediately when standing beside the Kitchen fixture,
                    // otherwise dispatch physical A* navigation when Founder is IDLE.
                    if (somatic.hydration < 0.45f && stock.potable_water_liters > 0.0) {
                        if (dist_to_water <= 2.5f) {
                            const double drink_l = std::min(1.5, stock.potable_water_liters);
                            stock.potable_water_liters -= drink_l;
                            somatic.hydration = std::min(1.0f, somatic.hydration + 0.50f);
                        } else if (e.has<oasis::fabrication::FounderCommandComponent>() &&
                                   e.get<oasis::fabrication::FounderCommandComponent>().state ==
                                       oasis::fabrication::FounderActionState::IDLE) {
                            DispatchFounderPathTo(
                                world,
                                stock.water_tote_pos,
                                oasis::fabrication::FounderActionState::SEEKING_WATER
                            );
                        }
                    } else if (somatic.caloric_reserve < 1200.0) {
                        if (dist_to_pantry <= 2.5f) {
                            if (!oasis::chill::CookHomesteadMealAtKitchen(somatic, psych, stock)) {
                                if (stock.ration_meals_remaining > 0u) {
                                    stock.ration_meals_remaining -= 1u;
                                    somatic.caloric_reserve = std::min(2500.0, somatic.caloric_reserve + 800.0);
                                    psych.mood = std::min(1.0f, psych.mood + 0.08f);
                                    psych.stress = std::max(0.0f, psych.stress - 0.04f);
                                }
                            }
                        } else if (e.has<oasis::fabrication::FounderCommandComponent>() &&
                                   e.get<oasis::fabrication::FounderCommandComponent>().state ==
                                       oasis::fabrication::FounderActionState::IDLE) {
                            if (stock.raw_produce_items >= 1u && stock.potable_water_liters >= 1.0) {
                                DispatchFounderPathTo(
                                    world,
                                    stock.ration_crate_pos,
                                    oasis::fabrication::FounderActionState::COOKING_MEAL
                                );
                            } else if (stock.ration_meals_remaining > 0u) {
                                DispatchFounderPathTo(
                                    world,
                                    stock.ration_crate_pos,
                                    oasis::fabrication::FounderActionState::SEEKING_FOOD
                                );
                            } else {
                                TriggerHarvestGardenAction(world, nullptr);
                            }
                        }
                    }

                    if (e.has<oasis::chill::BurnoutStateComponent>()) {
                        auto& burnout = e.get_mut<oasis::chill::BurnoutStateComponent>();
                        oasis::chill::StepRecreationAndBurnout(
                            somatic, psych, burnout, stock,
                            stock.campfire_active && psych.stress > 0.35f,
                            false,
                            stock.campfire_active,
                            sim_hours_per_tick,
                            &g_time_warp
                        );
                    }
                }

                if (e.has<oasis::fabrication::FounderCommandComponent>()) {
                    auto& cmd = e.get_mut<oasis::fabrication::FounderCommandComponent>();

                    // Autonomous Bedroom Rest trigger when Focus drops below 25% or Founder is Exhausted
                    if (psych.focus < 0.25f && cmd.state == oasis::fabrication::FounderActionState::IDLE) {
                        cmd.state = oasis::fabrication::FounderActionState::RESTING;
                    }

                    if (cmd.state == oasis::fabrication::FounderActionState::RESTING ||
                        cmd.state == oasis::fabrication::FounderActionState::EXHAUSTED) {
                        const uint32_t cur_mat = oasis::graphics::GetVoxelMaterialAt(start_voxel);
                        const bool is_hearth_goal = (cmd.target_voxel == glm::ivec3(19, 1, 9));
                        const glm::ivec3 rest_goal = is_hearth_goal ? glm::ivec3(19, 1, 9) : glm::ivec3(11, 1, 8);
                        const float dist_to_rest_goal = glm::length(glm::vec3(rest_goal) - pos.pos);
                        const bool at_shelter = is_hearth_goal
                            ? (dist_to_rest_goal <= 2.5f)
                            : ((dist_to_tent <= 2.5f) ||
                               (enc_status == oasis::chill::EnclosureStatus::ENCLOSED_INSULATED) ||
                               (cur_mat == oasis::chill::MAT_BELL_TENT) ||
                               (cur_mat == oasis::chill::MAT_SLEEP_ZONE));

                        if (!at_shelter && dist_to_rest_goal > 0.01f) {
                            if (!e.has<oasis::pathfinding::VoxelPathComponent>()) {
                                e.set<oasis::pathfinding::VoxelPathComponent>({});
                            }
                            auto& path = e.get_mut<oasis::pathfinding::VoxelPathComponent>();
                            if (path.waypoints.empty() || path.target_goal != rest_goal) {
                                path = oasis::pathfinding::ComputeVoxelPath(start_voxel, rest_goal);
                            }
                            const float metabolic_mult = oasis::chill::EvaluateMetabolicSpeedMultiplier(somatic);
                            oasis::pathfinding::StepAgentAlongVoxelPath(
                                pos.pos, path, pos.move_speed * metabolic_mult, dt
                            );
                        } else {
                            oasis::chill::StepInsulatedSleepRecovery(
                                somatic,
                                psych,
                                oasis::chill::EnclosureStatus::ENCLOSED_INSULATED,
                                600ULL
                            );
                            if (is_hearth_goal) {
                                psych.stress = std::max(0.0f, psych.stress - 0.002f);
                            }
                            if (psych.focus >= 0.85f && somatic.caloric_reserve > 500.0) {
                                somatic.is_exhausted = false;
                                cmd.state = oasis::fabrication::FounderActionState::IDLE;
                            }
                        }
                    } else if (enc_status != oasis::chill::EnclosureStatus::UNENCLOSED &&
                               e.has<oasis::chill::EnclosureCacheComponent>() &&
                               cmd.state == oasis::fabrication::FounderActionState::IDLE) {
                        const auto& enc_cache = e.get<oasis::chill::EnclosureCacheComponent>();
                        oasis::chill::StepInsulatedSleepRecovery(somatic, psych, enc_cache, sim_hours_per_tick);
                    }
                }
            }
        });

    // Register MigrationAndAssimilationSystem in flecs::OnUpdate phase (Layer 7 Sidewalk Neighbors & Enclave)
    g_ecs->system<oasis::boundary::BoundaryInterfaceComponent>("MigrationAndAssimilationSystem")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity e, oasis::boundary::BoundaryInterfaceComponent&) {
            auto world = e.world();

            // 1. Drain incoming MigrationWaveArrivalEvent packets from UHAI channel
            oasis::uhai::MigrationWaveArrivalEvent incoming_wave{};
            while (g_migration_wave_channel.Pop(incoming_wave)) {
                if (!g_is_headless_ci && g_sim_tick >= oasis::migration::SOLO_GENESIS_GRACE_TICKS) {
                    oasis::migration::SpawnMigrationWaveInOasis(
                        world, incoming_wave, g_migration_staging, g_time_warp
                    );
                } else {
                    g_migration_staging.StageOverflow(incoming_wave, incoming_wave.cohort_size);
                }
            }

            // 2. Spawn staged waves once past Solo Genesis Grace Period if slots are available
            if (!g_is_headless_ci &&
                g_sim_tick >= oasis::migration::SOLO_GENESIS_GRACE_TICKS &&
                !g_migration_staging.staged_waves.empty() &&
                oasis::migration::CountActiveMigrantNpcs(world) < oasis::migration::MAX_PARCEL_NPCS) {
                oasis::uhai::MigrationWaveArrivalEvent front_wave = g_migration_staging.staged_waves.front();
                g_migration_staging.staged_waves.erase(g_migration_staging.staged_waves.begin());
                if (g_migration_staging.staged_overflow_migrants >= front_wave.cohort_size) {
                    g_migration_staging.staged_overflow_migrants -= front_wave.cohort_size;
                } else {
                    g_migration_staging.staged_overflow_migrants = 0;
                }
                oasis::migration::SpawnMigrationWaveInOasis(
                    world, front_wave, g_migration_staging, g_time_warp
                );
            }

            // 3. Drain incoming TrustRingPromotionEvent packets from UHAI channel
            oasis::uhai::TrustRingPromotionEvent promo_ev{};
            while (g_trust_promotion_channel.Pop(promo_ev)) {
                world.each([&](oasis::migration::LegacyMigrantComponent& mig,
                               oasis::psychology::PsychologyComponent& npc_psych) {
                    if (promo_ev.migrant_did_hash == 0 || mig.did_hash == promo_ev.migrant_did_hash) {
                        if (promo_ev.to_ring == 3u) {
                            oasis::migration::TryPromoteMigrantR6ToR3(mig, npc_psych);
                        } else if (promo_ev.to_ring <= 1u) {
                            oasis::migration::TryPromoteMigrantR3ToR1(
                                mig, npc_psych, std::max<uint8_t>(2u, promo_ev.frost_quorum_signers)
                            );
                        }
                    }
                });
            }

            // 4. Step Layer 7 Sidewalk Neighbor Patrol & Minimum Grace Acknowledgment (Zero Task Assistance)
            constexpr float dt = 1.0f / 60.0f;
            glm::vec3 founder_world_pos(13.0f, 1.0f, 6.0f);
            auto founder_ent = world.entity("Founder");
            if (founder_ent.is_alive() && founder_ent.has<oasis::fabrication::SpatialPositionComponent>()) {
                founder_world_pos = founder_ent.get<oasis::fabrication::SpatialPositionComponent>().pos;
            }

            uint32_t npc_idx = 0;
            world.each([&](oasis::migration::LegacyMigrantComponent& mig,
                           oasis::fabrication::SpatialPositionComponent& npc_pos) {
                // Layer 7 neighbors stay strictly on the public front sidewalk (x = 4..5) and NEVER help on tasks
                const float lane_x = 4.0f + static_cast<float>(npc_idx % 2u);
                const bool walk_north = (((g_sim_tick / 540ULL) + npc_idx) % 2ULL) == 0ULL;
                const float goal_z = walk_north ? 18.0f : -4.0f;
                const glm::vec3 desired_pos(lane_x, 1.0f, goal_z);

                const glm::vec3 to_goal = desired_pos - npc_pos.pos;
                const float dist = glm::length(to_goal);
                if (dist > 0.35f) {
                    npc_pos.pos += (to_goal / dist) * (1.25f * dt);
                }

                // Minimum Grace Acknowledgment when passing Lot 402 front walkway (z in [4, 10])
                if (!g_is_headless_ci &&
                    npc_pos.pos.z >= 4.0f && npc_pos.pos.z <= 10.0f &&
                    glm::length(founder_world_pos - npc_pos.pos) <= 11.0f) {
                    TriggerLayer7MinimumGrace(world, false);
                }

                if (g_sim_tick > 0 && (g_sim_tick % oasis::chill::TICKS_PER_SIM_DAY == 0)) {
                    mig.sheltered_days_consecutive++;
                    if (mig.trust_ring_tier == 3u) {
                        mig.days_in_r3_consecutive++;
                    }
                }
                npc_idx++;
            });

            // 5. Synchronize Sidewalk NPC sub-voxel avatars (MAT_MIGRANT_R6=27, MAT_RESIDENT_R3=28, MAT_STEWARD_R1=29)
            if (npc_idx > 0) {
                oasis::migration::SyncMigrantAvatarVoxels(world);
            }
        });

    // Register FounderVitalsHUDSystem in flecs::PostUpdate phase
    // Note: WebGPU DOM updateHUD() is invoked strictly once per 60 Hz frame in EngineTick()
    g_ecs->system<const oasis::fabrication::SomaticStateComponent,
                  const oasis::psychology::PsychologyComponent>("FounderVitalsHUDSystem")
        .kind(flecs::PostUpdate)
        .each([](flecs::entity e,
                 const oasis::fabrication::SomaticStateComponent& somatic,
                 const oasis::psychology::PsychologyComponent& psych) {
            if (e != e.world().entity("Founder")) return;
            (void)somatic;
            (void)psych;
        });

    // Register Renderer entity with non-zero size RenderState
    g_ecs->component<oasis::graphics::RenderState>();
    oasis::graphics::RenderState initial_state{};
    initial_state.width = kWidth;
    initial_state.height = kHeight;
    initial_state.arcball = g_arcball_camera;
    g_ecs->entity("Renderer").set<oasis::graphics::RenderState>(initial_state);

#ifdef __EMSCRIPTEN__
    // Initiate modern asynchronous WebGPU adapter & device request flow
    InitWebGPUAsync(kWidth, kHeight);

    // Set up main loop via emscripten_set_main_loop_arg (decoupled 60 Hz RenderWebGPUFrame + sub-stepped g_ecs->progress())
    emscripten_set_main_loop_arg(EngineTick, g_ecs, 0, false);
#else
    uint64_t ticks = 0;
    while (g_running) {
        ProcessInputEvents();
        if (!g_running) break;

        auto founder = g_ecs->entity("Founder");
        if (founder.is_alive() &&
            founder.has<oasis::fabrication::SomaticStateComponent>() &&
            founder.has<oasis::psychology::PsychologyComponent>()) {
            const auto& som = founder.get<oasis::fabrication::SomaticStateComponent>();
            const auto& psy = founder.get<oasis::psychology::PsychologyComponent>();
            if (oasis::time::EvaluateEmergencyWarpBrake(g_time_warp, som.caloric_reserve, som.hydration, psy.stress, false)) {
                spdlog::warn("[Oasis Time Warp] Emergency Auto-Downshift to 1x triggered: {}", g_time_warp.emergency_reason);
            }
        }

        bool ecs_alive = true;
        for (uint32_t step = 0; step < g_time_warp.multiplier; ++step) {
            if (!g_ecs->progress()) {
                ecs_alive = false;
                break;
            }
            if (founder.is_alive() &&
                founder.has<oasis::fabrication::SomaticStateComponent>() &&
                founder.has<oasis::psychology::PsychologyComponent>()) {
                const auto& som = founder.get<oasis::fabrication::SomaticStateComponent>();
                const auto& psy = founder.get<oasis::psychology::PsychologyComponent>();
                if (oasis::time::EvaluateEmergencyWarpBrake(g_time_warp, som.caloric_reserve, som.hydration, psy.stress, false)) {
                    spdlog::warn("[Oasis Time Warp] Mid-frame Emergency Auto-Downshift to 1x triggered: {}", g_time_warp.emergency_reason);
                    break;
                }
            }
        }
        if (!ecs_alive) break;

        // Strictly once per 60 Hz frame: update camera/render state & push aggregated telemetry
        auto r = g_ecs->entity("Renderer");
        if (r.is_alive() && r.has<oasis::graphics::RenderState>()) {
            auto& state = r.get_mut<oasis::graphics::RenderState>();
            oasis::graphics::UpdateCameraMatrices(state, 1.0f / 60.0f);
            state.frame_count++;
        }
        TickTelemetry();

        ticks++;
        if (is_headless_ci && ticks >= cli_opts.max_ticks) {
            break;
        }
    }
#endif

#ifndef __EMSCRIPTEN__
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete g_ecs;
    g_ecs = nullptr;
#endif

    return 0;
}
