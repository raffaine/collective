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
static int g_last_mouse_x = 0;
static int g_last_mouse_y = 0;
static bool g_shift_pressed = false;





#include "shaders/raymarch_wgsl.hpp"
#include "uhai/uhai_ring_buffer.hpp"
#include "core/stochastic_boundary.hpp"
#include "core/fabrication.hpp"
#include "core/homeostasis.hpp"
#include "core/raycaster.hpp"
#include "core/camera_uniforms.hpp"

namespace oasis {
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
    glm::vec4 viewport_params{800.0f, 600.0f, 800.0f / 600.0f, 0.0f}; // 16 bytes (offset 224), x=w, y=h, z=aspect, w=transition_t
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
    uint32_t active_camera_mode{0}; // 0 = 3rd-Person Perspective, 1 = Cadastral Isometric
    uint32_t target_camera_mode{0};
    float transition_elapsed_sec{0.0f};
    bool is_transitioning{false};
    glm::vec3 camera_offset{0.0f, 0.0f, 0.0f}; // Mac-friendly panning offset (ground plane locked)
    uint32_t width{800};
    uint32_t height{600};
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

    const float aspect = (state.height > 0) ? (static_cast<float>(state.width) / static_cast<float>(state.height)) : (800.0f / 600.0f);

    // Sync state.camera_offset with legacy globals if modified externally
    if (g_cam_offset_x != state.camera_offset.x || g_cam_offset_z != state.camera_offset.z) {
        state.camera_offset.x = g_cam_offset_x;
        state.camera_offset.z = g_cam_offset_z;
    }
    const glm::vec3 offset = state.camera_offset;

    // 1. Mode 0: 3rd-Person Follow Perspective Camera
    const glm::vec3 eye_persp = offset + glm::vec3(0.0001f, 2.0001f, -4.9999f);
    const glm::vec3 target_persp = offset + glm::vec3(0.0f, 0.0f, 0.0f);
    const glm::vec3 up_persp(0.0f, 1.0f, 0.0f);
    const glm::mat4 view_persp = glm::lookAt(eye_persp, target_persp, up_persp);
    const glm::mat4 proj_persp = glm::perspective(glm::radians(53.13f), aspect, 0.1f, 100.0f);
    const glm::mat4 inv_vp_persp = glm::inverse(proj_persp * view_persp);

    // 2. Mode 1: Cadastral Axonometric / Isometric Projection Camera
    const float ortho_size = 14.0f;
    const glm::mat4 proj_iso = glm::ortho(-ortho_size * aspect, ortho_size * aspect, -ortho_size, ortho_size, -50.0f, 50.0f);
    const glm::vec3 eye_iso = offset + glm::vec3(-16.0001f, 20.0001f, -16.0001f);
    const glm::vec3 target_iso = offset + glm::vec3(0.0001f, 0.0001f, 0.0001f);
    const glm::vec3 up_iso(0.0f, 1.0f, 0.0f);
    const glm::mat4 view_iso = glm::lookAt(eye_iso, target_iso, up_iso);
    const glm::mat4 inv_vp_iso = glm::inverse(proj_iso * view_iso);

    // Interpolate camera uniforms across transition parameter t
    state.camera_uniforms.view_matrix = (1.0f - t) * view_persp + t * view_iso;
    state.camera_uniforms.proj_matrix = (1.0f - t) * proj_persp + t * proj_iso;
    state.camera_uniforms.inv_view_proj = (1.0f - t) * inv_vp_persp + t * inv_vp_iso;
    state.camera_uniforms.camera_pos = glm::mix(glm::vec4(eye_persp, 0.0f), glm::vec4(eye_iso, 1.0f), t);
    state.camera_uniforms.camera_target = glm::mix(glm::vec4(target_persp, 1.0f), glm::vec4(target_iso, 1.0f), t);
    state.camera_uniforms.viewport_params = glm::vec4(static_cast<float>(state.width), static_cast<float>(state.height), aspect, t);
    // cursor_highlight is preserved across frame transitions
}

} // namespace graphics
} // namespace oasis

// Heap-allocated Flecs world to prevent stack use-after-free before async callbacks fire
static flecs::world* g_ecs = nullptr;

// Monotonic simulation frame counter
static uint64_t g_sim_tick = 0;

// Global UHAI Telemetry Channel (Producer mode)
static oasis::uhai::UhaiTelemetryChannel g_telemetry_channel(oasis::uhai::UhaiTelemetryChannel::ChannelMode::Producer);

// Write telemetry frame sample on each engine tick
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

static oasis::fabrication::InfrastructureType g_active_blueprint_type = oasis::fabrication::InfrastructureType::SOLAR_PANEL;

// Poll SDL input events: TAB camera toggle, Mac trackpad wheel/Shift-drag camera panning, blueprint placement
static void ProcessInputEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
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

            if (g_ecs) {
                auto r = g_ecs->entity("Renderer");
                if (r.has<oasis::graphics::RenderState>()) {
                    auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                    oasis::graphics::ApplyCameraPan(state_ref, wheel_x, -wheel_y, true);
                    r.modified<oasis::graphics::RenderState>();
                }
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            if (g_is_dragging) {
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
                            state_ref.camera_uniforms.viewport_params
                        );
                        if (hit.hit) {
                            state_ref.camera_uniforms.cursor_highlight = glm::vec4(glm::vec3(hit.voxel_pos), 1.0f);
                        } else {
                            state_ref.camera_uniforms.cursor_highlight = glm::vec4(0.0f);
                        }
                    }
                }
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            const SDL_Keymod mod = SDL_GetModState();
            const bool is_shift_down = ((mod & KMOD_SHIFT) != 0) || g_shift_pressed;

            if (event.button.button == SDL_BUTTON_MIDDLE ||
                (event.button.button == SDL_BUTTON_LEFT && is_shift_down)) {
                g_is_dragging = true;
                g_last_mouse_x = event.button.x;
                g_last_mouse_y = event.button.y;
            } else if (event.button.button == SDL_BUTTON_LEFT) {
                // Blueprint placement raycast (only when not Shift-panning)
                if (g_ecs) {
                    auto r = g_ecs->entity("Renderer");
                    auto founder = g_ecs->entity("Founder");
                    if (r.has<oasis::graphics::RenderState>() && founder.has<oasis::fabrication::FounderCommandComponent>()) {
                        auto& state_ref = r.get_mut<oasis::graphics::RenderState>();
                        auto hit = oasis::graphics::ScreenToWorldRaycast(
                            event.button.x, event.button.y,
                            state_ref.width, state_ref.height,
                            state_ref.camera_uniforms.inv_view_proj,
                            state_ref.camera_uniforms.camera_pos,
                            state_ref.camera_uniforms.viewport_params
                        );
                        if (hit.hit) {
                            glm::ivec3 target_pos = hit.placement_pos;
                            auto bp_type = g_active_blueprint_type;
                            if (hit.voxel_pos.y >= 4) {
                                bp_type = oasis::fabrication::InfrastructureType::SOLAR_PANEL;
                            }

                            auto bp_entity = g_ecs->entity()
                                .set<oasis::fabrication::BlueprintComponent>({
                                    bp_type,
                                    target_pos.x, target_pos.y, target_pos.z,
                                    100.0, 0.0, 500.0, false, 0.20f
                                });

                            auto& cmd = founder.get_mut<oasis::fabrication::FounderCommandComponent>();
                            cmd.state = oasis::fabrication::FounderActionState::NAVIGATING;
                            cmd.target_voxel = target_pos;
                            cmd.target_blueprint = bp_entity;

                            spdlog::info("[Oasis Input] Left click at ({},{}) -> Voxel hit ({},{},{}). Queued {} blueprint at ({},{},{}). Dispatching Founder.",
                                         event.button.x, event.button.y, hit.voxel_pos.x, hit.voxel_pos.y, hit.voxel_pos.z,
                                         (bp_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL ? "Solar Panel" : "Rainwater Catchment"),
                                         target_pos.x, target_pos.y, target_pos.z);
                        }
                    }
                }
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                g_active_blueprint_type = (g_active_blueprint_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL)
                    ? oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT
                    : oasis::fabrication::InfrastructureType::SOLAR_PANEL;
                spdlog::info("[Oasis Input] Right click: Toggled active blueprint type to {}",
                             (g_active_blueprint_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL ? "Solar Panel (Mat 11)" : "Rainwater Catchment (Mat 12)"));
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_MIDDLE || event.button.button == SDL_BUTTON_LEFT) {
                g_is_dragging = false;
            }
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_LEAVE || 
                event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                g_is_dragging = false;
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
            const float anim_time = static_cast<float>(state.frame_count) * (1.0f / 60.0f);
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

                        SetupPipeline(state);

                        state.is_initialized = true;
                        g_ecs->entity("Renderer").set<oasis::graphics::RenderState>(state);
                        spdlog::info("WebGPU initialization complete and RenderState updated in Flecs");
                    }
                });
        });
}

void EngineTick(void* arg) {
    if (g_ecs) {
        ProcessInputEvents();
        g_ecs->progress();
        TickTelemetry();
    }
}
#endif

int main() {
    spdlog::info("Oasis V4 Engine Starting...");
    spdlog::set_level(spdlog::level::info);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::error("SDL_Init Error: {}", SDL_GetError());
        return 1;
    }

    constexpr uint32_t kWidth = 800;
    constexpr uint32_t kHeight = 600;

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

    // Allocate Flecs world on the heap to prevent stack deallocation / UAF
    g_ecs = new flecs::world();
    g_ecs->set_target_fps(60.0f);

    // Register psychological components
    g_ecs->component<oasis::psychology::PersonalityFacets>();
    g_ecs->component<oasis::psychology::PsychologyComponent>();

    // Register fabrication and somatic components
    g_ecs->component<oasis::fabrication::SomaticStateComponent>();
    g_ecs->component<oasis::fabrication::SpatialPositionComponent>();
    g_ecs->component<oasis::fabrication::FounderCommandComponent>();
    g_ecs->component<oasis::fabrication::BlueprintComponent>();
    g_ecs->component<oasis::fabrication::VoxelPlacedComponent>();

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
    founder_pos.pos = glm::vec3(0.0f, 1.0f, 0.0f);
    founder_pos.move_speed = 3.0f;

    oasis::fabrication::FounderCommandComponent founder_cmd{};
    founder_cmd.state = oasis::fabrication::FounderActionState::IDLE;

    g_ecs->entity("Founder")
        .set<oasis::psychology::PersonalityFacets>(founder_psych.facets)
        .set<oasis::psychology::PsychologyComponent>(founder_psych)
        .set<oasis::fabrication::SomaticStateComponent>(founder_somatic)
        .set<oasis::fabrication::SpatialPositionComponent>(founder_pos)
        .set<oasis::fabrication::FounderCommandComponent>(founder_cmd);

    // Register FounderMovementSystem in flecs::OnUpdate phase
    g_ecs->system<oasis::fabrication::SpatialPositionComponent,
                  oasis::fabrication::FounderCommandComponent,
                  oasis::fabrication::SomaticStateComponent>("FounderMovementSystem")
        .kind(flecs::OnUpdate)
        .each([](oasis::fabrication::SpatialPositionComponent& pos,
                 oasis::fabrication::FounderCommandComponent& cmd,
                 oasis::fabrication::SomaticStateComponent& somatic) {
            if (cmd.state != oasis::fabrication::FounderActionState::NAVIGATING) return;

            glm::vec3 target_pos = glm::vec3(cmd.target_voxel.x, cmd.target_voxel.y, cmd.target_voxel.z);
            glm::vec3 to_target = target_pos - pos.pos;
            float dist = glm::length(to_target);

            constexpr float dt = 1.0f / 60.0f;

            if (dist > 1.5f) {
                glm::vec3 dir = to_target / dist;
                pos.pos += dir * pos.move_speed * dt;
                somatic.caloric_reserve = std::max(0.0, somatic.caloric_reserve - 0.10 * dt);
            } else {
                cmd.state = oasis::fabrication::FounderActionState::CONSTRUCTING;
                spdlog::info("[Oasis Movement] Founder arrived at construction site ({},{},{}). Starting fabrication.",
                             cmd.target_voxel.x, cmd.target_voxel.y, cmd.target_voxel.z);
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
            if (cmd.state != oasis::fabrication::FounderActionState::CONSTRUCTING) return;
            if (!cmd.target_blueprint.is_alive() || !cmd.target_blueprint.has<oasis::fabrication::BlueprintComponent>()) {
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
                return;
            }

            auto& bp = cmd.target_blueprint.get_mut<oasis::fabrication::BlueprintComponent>();
            if (bp.is_completed) {
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
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

            // Incremental fabrication progress
            constexpr float dt = 1.0f / 60.0f;
            constexpr double work_rate = 25.0;
            double delta_w = std::min(work_rate * dt, bp.required_work_ticks - bp.invested_work_ticks);
            double progress_fraction = (bp.required_work_ticks > 0.0) ? (delta_w / bp.required_work_ticks) : 1.0;

            double cal_burn = bp.caloric_cost_total * progress_fraction;
            float focus_drain = bp.focus_cost * static_cast<float>(progress_fraction);

            somatic.caloric_reserve = std::max(0.0, somatic.caloric_reserve - cal_burn);
            psych.focus = std::max(0.0f, psych.focus - focus_drain);
            somatic.focus = psych.focus;
            somatic.stamina = std::max(0.0f, somatic.stamina - 0.005f);
            bp.invested_work_ticks += delta_w;

            if (bp.invested_work_ticks >= bp.required_work_ticks) {
                bp.is_completed = true;
                cmd.state = oasis::fabrication::FounderActionState::IDLE;

                auto placed_type = bp.type;
                int px = bp.x, py = bp.y, pz = bp.z;
                cmd.target_blueprint.destruct();

                e.world().entity()
                    .set<oasis::fabrication::VoxelPlacedComponent>({placed_type, px, py, pz, g_sim_tick});

                uint8_t mat_id = (placed_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL) ? 11 : 12;
                oasis::graphics::SetDynamicVoxel(px, py, pz, mat_id);

                auto boundary = e.world().entity("SettlementBoundary");
                if (boundary.is_alive() && boundary.has<oasis::boundary::InfrastructureAutarkyComponent>()) {
                    auto& infra = boundary.get_mut<oasis::boundary::InfrastructureAutarkyComponent>();
                    if (placed_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL) {
                        infra.solar_stored_kwh += 5.0;
                    } else {
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
                             (placed_type == oasis::fabrication::InfrastructureType::SOLAR_PANEL ? "Solar Panel" : "Rainwater Catchment"),
                             px, py, pz, somatic.caloric_reserve, psych.focus);
            }
        });

    // Register FounderPsychologySystem in flecs::OnUpdate phase
    g_ecs->system<oasis::psychology::PsychologyComponent>("FounderPsychologySystem")
        .kind(flecs::OnUpdate)
        .each([](oasis::psychology::PsychologyComponent& psych) {
            g_sim_tick++;
            const uint64_t tick = g_sim_tick;

            bool recorded = false;
            oasis::psychology::EpisodicMemoryNode node{};
            node.timestamp_tick = tick;

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

            if (recorded) {
                uint32_t slot = psych.memory_ring.Push(node);
                spdlog::info("[Oasis Cognitive Core] Founder Memory: tick={} event={} salience={} stress={:.2f} (Slot {}/32, Permanent={})",
                             tick, node.description, node.salience, psych.stress, slot, psych.memory_ring.permanent_count);
            }

            // Periodic diagnostic memory summary every 60 ticks
            if (tick % 60 == 0) {
                spdlog::info("[Oasis Cognitive Core] Diagnostic Summary: tick={} | Total Recorded: {} | Active Slots: {}/32 | Permanent: {} | Stress: {:.2f} | Mood: {:.2f} | Focus: {:.2f}",
                             tick, psych.memory_ring.total_recorded, psych.memory_ring.count, psych.memory_ring.permanent_count,
                             psych.stress, psych.mood, psych.focus);
            }
        });

    // Register boundary components
    g_ecs->component<oasis::boundary::InfrastructureAutarkyComponent>();
    g_ecs->component<oasis::boundary::SocialMutualAidComponent>();
    g_ecs->component<oasis::boundary::PeerMeshComponent>();
    g_ecs->component<oasis::boundary::BoundaryInterfaceComponent>();

    // Instantiate SettlementBoundary entity with default components
    g_ecs->entity("SettlementBoundary")
        .set<oasis::boundary::InfrastructureAutarkyComponent>({})
        .set<oasis::boundary::SocialMutualAidComponent>({})
        .set<oasis::boundary::PeerMeshComponent>({})
        .set<oasis::boundary::BoundaryInterfaceComponent>({});

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

            if (g_sim_tick % 10 == 0 || g_sim_tick == 1) {
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
            float amb_temp = oasis::homeostasis::ResolveVoxelAmbientTemperature(pos.pos, g_sim_tick);
            oasis::homeostasis::StepHomeostasis(somatic, psych, amb_temp, dt);
        });

    // Register FounderVitalsHUDSystem in flecs::PostUpdate phase
    g_ecs->system<const oasis::fabrication::SomaticStateComponent,
                  const oasis::psychology::PsychologyComponent>("FounderVitalsHUDSystem")
        .kind(flecs::PostUpdate)
        .each([](flecs::entity e,
                 const oasis::fabrication::SomaticStateComponent& somatic,
                 const oasis::psychology::PsychologyComponent& psych) {
            if (e != e.world().entity("Founder")) return;
#ifdef __EMSCRIPTEN__
            EM_ASM({
                if (typeof updateHUD === 'function') {
                    updateHUD($0, $1, $2, $3);
                } else if (typeof window !== 'undefined' && typeof window.updateHUD === 'function') {
                    window.updateHUD($0, $1, $2, $3);
                }
            }, somatic.caloric_reserve, static_cast<double>(psych.stress), static_cast<double>(psych.focus), static_cast<double>(somatic.ambient_temp_c));
#endif
        });

    // Register Renderer entity with non-zero size RenderState
    g_ecs->component<oasis::graphics::RenderState>();
    oasis::graphics::RenderState initial_state{};
    initial_state.width = kWidth;
    initial_state.height = kHeight;
    g_ecs->entity("Renderer").set<oasis::graphics::RenderState>(initial_state);

#ifdef __EMSCRIPTEN__
    // Integrate WebGPU render pass into the Flecs system loop
    g_ecs->system<oasis::graphics::RenderState>("WebGPURenderPass")
        .kind(flecs::OnStore)
        .each([](oasis::graphics::RenderState& state) {
            RenderWebGPUFrame(state);
        });

    // Initiate modern asynchronous WebGPU adapter & device request flow
    InitWebGPUAsync(kWidth, kHeight);

    // Set up main loop via emscripten_set_main_loop_arg to tick g_ecs->progress()
    emscripten_set_main_loop_arg(EngineTick, g_ecs, 0, false);
#else
    g_ecs->system<oasis::graphics::RenderState>("NativeStubRenderPass")
        .kind(flecs::OnStore)
        .each([](oasis::graphics::RenderState& state) {
            oasis::graphics::UpdateCameraMatrices(state, 1.0f / 60.0f);
            state.frame_count++;
        });

    int ticks = 0;
    while (g_ecs->progress()) {
        ProcessInputEvents();
        TickTelemetry();
        ticks++;
        if (ticks >= 60) break;
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
