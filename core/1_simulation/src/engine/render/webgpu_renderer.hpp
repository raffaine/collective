#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif
#include <SDL2/SDL.h>
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <webgpu/webgpu_cpp.h>
#endif

#include "../../core/arcball_camera.hpp"
#include "../../core/camera_uniforms.hpp"
#include "../../shaders/raymarch_wgsl.hpp"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include "../world/map_provider.hpp"

namespace oasis::engine::render {

inline constexpr int32_t  kChunkOriginX   = -16;
inline constexpr int32_t  kChunkOriginY   = -2;
inline constexpr int32_t  kChunkOriginZ   = -8;
inline constexpr uint32_t kChunkSizeX     = 64u;
inline constexpr uint32_t kChunkSizeY     = 16u;
inline constexpr uint32_t kChunkSizeZ     = 32u;
inline constexpr size_t   kChunkCellCount =
    static_cast<size_t>(kChunkSizeX) *
    static_cast<size_t>(kChunkSizeY) *
    static_cast<size_t>(kChunkSizeZ); // 32,768 cells

// Matches @group(0) @binding(2) var<storage, read> world_chunk : WorldChunkBuffer in raymarch.wgsl
struct alignas(16) WorldChunkGpuBuffer {
    uint32_t is_loaded{1u};
    int32_t  origin_x{kChunkOriginX};
    int32_t  origin_y{kChunkOriginY};
    int32_t  origin_z{kChunkOriginZ};
    uint32_t size_x{kChunkSizeX};
    uint32_t size_y{kChunkSizeY};
    uint32_t size_z{kChunkSizeZ};
    uint32_t flags{1u}; // bit 0: enable distant neighbor horizon silhouettes outside loaded chunk
    uint32_t cells[kChunkCellCount]{};
};
static_assert(sizeof(WorldChunkGpuBuffer) == 131104u, "WorldChunkGpuBuffer must be 131,104 bytes.");

// Matches @group(0) @binding(1) var<storage, read> dynamic_voxels : DynamicVoxelStore in raymarch.wgsl
using DynamicVoxelStoreGpu = oasis::graphics::GpuDynamicVoxelBuffer;
static_assert(sizeof(DynamicVoxelStoreGpu) == 16416u, "DynamicVoxelStoreGpu must be 16,416 bytes.");
static_assert(sizeof(oasis::graphics::CameraUniforms) == 288u, "CameraUniforms must be 288 bytes.");

struct alignas(8) WorldRaycastHit {
    int32_t  vx{0};
    int32_t  vy{0};
    int32_t  vz{0};
    int32_t  place_x{0};
    int32_t  place_y{0};
    int32_t  place_z{0};
    int32_t  normal_x{0};
    int32_t  normal_y{1};
    int32_t  normal_z{0};
    uint16_t material_id{0u};
    bool     hit{false};
    uint8_t  pad_[1]{0u};
};
static_assert(sizeof(WorldRaycastHit) == 40u, "WorldRaycastHit must be 40 bytes.");

class alignas(16) WebGpuRenderer {
public:
    WebGpuRenderer() noexcept {
        // Elevated colony-sim boot framing overlooking Lot 402 6-room home (cutaway Z=3), garage & backyard
        arcball_.has_poi_lock            = true;
        arcball_.poi_origin              = glm::vec3(14.5f, 1.0f, 6.5f);
        arcball_.target_poi              = glm::vec3(14.5f, 1.0f, 6.5f);
        arcball_.theta_rad               = -0.62f;
        arcball_.target_theta_rad        = -0.62f;
        arcball_.phi_rad                 = 0.72f;
        arcball_.target_phi_rad          = 0.72f;
        arcball_.radius                  = 18.0f;
        arcball_.target_radius           = 18.0f;
        arcball_.z_cutaway_level         = 3;
        arcball_.manual_cutaway_override = true;
    }

    ~WebGpuRenderer() {
        Shutdown();
    }

    WebGpuRenderer(const WebGpuRenderer&)            = delete;
    WebGpuRenderer& operator=(const WebGpuRenderer&) = delete;

    bool Initialize(uint32_t width, uint32_t height, bool headless_ci) {
        width_  = (width > 0u) ? width : 1024u;
        height_ = (height > 0u) ? height : 720u;

        if (headless_ci) {
            gpu_initialized_ = true;
            UpdateCameraMatrices(1.0f / 60.0f);
            return true;
        }

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            spdlog::warn("[Oasis Renderer] SDL_Init(SDL_INIT_VIDEO) failed ({}); continuing headless.", SDL_GetError());
            gpu_initialized_ = true;
            UpdateCameraMatrices(1.0f / 60.0f);
            return true;
        }
        sdl_initialized_ = true;

        sdl_window_ = SDL_CreateWindow(
            "Oasis V4",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            static_cast<int>(width_),
            static_cast<int>(height_),
            SDL_WINDOW_SHOWN
        );
        if (sdl_window_ == nullptr) {
            spdlog::error("[Oasis Renderer] SDL_CreateWindow failed: {}", SDL_GetError());
            SDL_Quit();
            sdl_initialized_ = false;
            return false;
        }

        UpdateCameraMatrices(1.0f / 60.0f);

#ifdef __EMSCRIPTEN__
        InitWebGpuAsync();
#else
        gpu_initialized_ = true;
#endif
        return true;
    }

    void Shutdown() noexcept {
#ifndef __EMSCRIPTEN__
        if (sdl_window_ != nullptr) {
            SDL_DestroyWindow(sdl_window_);
            sdl_window_ = nullptr;
        }
        if (sdl_initialized_) {
            SDL_Quit();
            sdl_initialized_ = false;
        }
#endif
    }

    // Populates WorldChunkGpuBuffer directly from WorldGrid and marks it dirty for immediate GPU upload.
    void SyncWorldChunkFromGrid(const WorldGrid& grid, uint32_t chunk_flags = 1u) noexcept {
        world_chunk_.is_loaded = 1u;
        world_chunk_.origin_x  = kChunkOriginX;
        world_chunk_.origin_y  = kChunkOriginY;
        world_chunk_.origin_z  = kChunkOriginZ;
        world_chunk_.size_x    = kChunkSizeX;
        world_chunk_.size_y    = kChunkSizeY;
        world_chunk_.size_z    = kChunkSizeZ;
        world_chunk_.flags     = chunk_flags;

        for (uint32_t ly = 0u; ly < kChunkSizeY; ++ly) {
            const int32_t wy = kChunkOriginY + static_cast<int32_t>(ly);
            for (uint32_t lz = 0u; lz < kChunkSizeZ; ++lz) {
                const int32_t wz = kChunkOriginZ + static_cast<int32_t>(lz);
                const size_t row_base =
                    (static_cast<size_t>(ly) * static_cast<size_t>(kChunkSizeZ) + static_cast<size_t>(lz)) *
                    static_cast<size_t>(kChunkSizeX);
                for (uint32_t lx = 0u; lx < kChunkSizeX; ++lx) {
                    const int32_t wx = kChunkOriginX + static_cast<int32_t>(lx);
                    world_chunk_.cells[row_base + static_cast<size_t>(lx)] =
                        static_cast<uint32_t>(grid.GetVoxel(wx, wy, wz));
                }
            }
        }
        ++chunk_sync_generation_;
        world_chunk_dirty_ = true;

#ifdef __EMSCRIPTEN__
        if (gpu_initialized_ && queue_ != nullptr && world_chunk_buffer_ != nullptr) {
            queue_.WriteBuffer(world_chunk_buffer_, 0, &world_chunk_, sizeof(WorldChunkGpuBuffer));
            world_chunk_dirty_ = false;
        }
#endif
    }

    void ClearDynamicVoxels() noexcept {
        dynamic_voxels_.count = 0u;
        dynamic_voxels_.min_x = 0;
        dynamic_voxels_.min_y = 0;
        dynamic_voxels_.min_z = 0;
        dynamic_voxels_.max_x = 0;
        dynamic_voxels_.max_y = 0;
        dynamic_voxels_.max_z = 0;
    }

    void PushDynamicVoxel(int32_t x, int32_t y, int32_t z, uint32_t material_id) noexcept {
        if (material_id == 0u || dynamic_voxels_.count >= oasis::graphics::MAX_GPU_DYNAMIC_VOXELS) {
            return;
        }
        const uint32_t idx = dynamic_voxels_.count;
        dynamic_voxels_.voxels[idx].x           = x;
        dynamic_voxels_.voxels[idx].y           = y;
        dynamic_voxels_.voxels[idx].z           = z;
        dynamic_voxels_.voxels[idx].material_id = material_id;
        if (idx == 0u) {
            dynamic_voxels_.min_x = x;
            dynamic_voxels_.min_y = y;
            dynamic_voxels_.min_z = z;
            dynamic_voxels_.max_x = x;
            dynamic_voxels_.max_y = y;
            dynamic_voxels_.max_z = z;
        } else {
            dynamic_voxels_.min_x = std::min(dynamic_voxels_.min_x, x);
            dynamic_voxels_.min_y = std::min(dynamic_voxels_.min_y, y);
            dynamic_voxels_.min_z = std::min(dynamic_voxels_.min_z, z);
            dynamic_voxels_.max_x = std::max(dynamic_voxels_.max_x, x);
            dynamic_voxels_.max_y = std::max(dynamic_voxels_.max_y, y);
            dynamic_voxels_.max_z = std::max(dynamic_voxels_.max_z, z);
        }
        dynamic_voxels_.count = idx + 1u;
    }

    void SetAvatarState(
        const glm::vec3& world_pos,
        float            anim_time_sec,
        bool             is_carrying,
        float            action_state_code,
        float            stress_norm,
        float            focus_norm
    ) noexcept {
        const float encoded_anim = anim_time_sec + (is_carrying ? 10.0f : 0.0f);
        camera_uniforms_.avatar_pos    = glm::vec4(world_pos, encoded_anim);
        camera_uniforms_.avatar_params = glm::vec4(action_state_code, stress_norm, focus_norm, 1.0f);
    }

    void SetCursorHighlight(int32_t vx, int32_t vy, int32_t vz, bool active) noexcept {
        if (active) {
            camera_uniforms_.cursor_highlight = glm::vec4(
                static_cast<float>(vx),
                static_cast<float>(vy),
                static_cast<float>(vz),
                1.0f
            );
        } else {
            camera_uniforms_.cursor_highlight = glm::vec4(0.0f);
        }
    }

    void ToggleCameraProjectionMode() noexcept {
        target_camera_mode_     = (active_camera_mode_ == 0u) ? 1u : 0u;
        is_transitioning_       = true;
        transition_elapsed_sec_ = 0.0f;
    }

    void Orbit(float delta_theta_rad, float delta_phi_rad) noexcept {
        oasis::graphics::OrbitArcball(arcball_, delta_theta_rad, delta_phi_rad);
    }

    void Zoom(float scroll_delta) noexcept {
        oasis::graphics::ZoomArcball(arcball_, scroll_delta);
    }

    void PanMouseOrWheel(float delta_x, float delta_y, bool is_wheel) noexcept {
        const float scale = is_wheel ? 0.035f : 0.030f;
        oasis::graphics::PanArcballPoi(arcball_, -delta_x * scale, delta_y * scale);
        camera_offset_.x = std::clamp(camera_offset_.x - delta_x * scale, -45.0f, 45.0f);
        camera_offset_.z = std::clamp(camera_offset_.z + delta_y * scale, -45.0f, 50.0f);
    }

    void PanByKey(float right_units, float fwd_units) noexcept {
        oasis::graphics::PanArcballPoi(arcball_, right_units, fwd_units);
        camera_offset_.x = std::clamp(camera_offset_.x + right_units, -45.0f, 45.0f);
        camera_offset_.z = std::clamp(camera_offset_.z + fwd_units, -45.0f, 50.0f);
    }

    void LockPoi(int32_t vx, int32_t vy, int32_t vz) noexcept {
        arcball_.target_poi   = glm::vec3(static_cast<float>(vx), static_cast<float>(vy), static_cast<float>(vz));
        arcball_.has_poi_lock = true;
        if (vy <= 3 && vx >= 8 && vx <= 17 && vz >= 2 && vz <= 14) {
            arcball_.z_cutaway_level = oasis::graphics::ARCBALL_INTERIOR_CUTAWAY_Z;
        }
    }

    void AdjustRoofCutaway(int32_t delta_z) noexcept {
        oasis::graphics::AdjustManualCutaway(arcball_, static_cast<int>(delta_z));
    }

    void SetRoofCutawayLevel(int32_t z_level) noexcept {
        arcball_.z_cutaway_level = std::clamp(
            static_cast<int>(z_level),
            oasis::graphics::ARCBALL_MIN_CUTAWAY_Z,
            oasis::graphics::ARCBALL_MAX_CUTAWAY_Z
        );
        arcball_.manual_cutaway_override = true;
    }

    void ToggleRoofCutaway() noexcept {
        if (arcball_.z_cutaway_level <= oasis::graphics::ARCBALL_INTERIOR_CUTAWAY_Z) {
            SetRoofCutawayLevel(oasis::graphics::ARCBALL_MAX_CUTAWAY_Z);
        } else {
            SetRoofCutawayLevel(oasis::graphics::ARCBALL_INTERIOR_CUTAWAY_Z);
        }
    }

    void SetOverlayMode(uint32_t mode) noexcept {
        overlay_mode_ = mode;
    }

    [[nodiscard]] uint32_t OverlayMode() const noexcept { return overlay_mode_; }
    [[nodiscard]] int32_t  CutawayLevel() const noexcept { return static_cast<int32_t>(arcball_.z_cutaway_level); }
    [[nodiscard]] uint32_t Width() const noexcept { return width_; }
    [[nodiscard]] uint32_t Height() const noexcept { return height_; }
    [[nodiscard]] uint64_t FrameCount() const noexcept { return frame_count_; }
    [[nodiscard]] uint64_t ChunkSyncGeneration() const noexcept { return chunk_sync_generation_; }
    [[nodiscard]] const WorldChunkGpuBuffer& WorldChunk() const noexcept { return world_chunk_; }
    [[nodiscard]] const oasis::graphics::CameraUniforms& Uniforms() const noexcept { return camera_uniforms_; }

    void UpdateCameraMatrices(float dt_sec) noexcept {
        if (is_transitioning_) {
            transition_elapsed_sec_ += dt_sec;
            if (transition_elapsed_sec_ >= 0.400f) {
                transition_elapsed_sec_ = 0.400f;
                is_transitioning_       = false;
                active_camera_mode_     = target_camera_mode_;
            }
        }

        oasis::graphics::StepArcballDamping(arcball_, dt_sec);

        float t = 0.0f;
        if (is_transitioning_) {
            const float u         = std::clamp(transition_elapsed_sec_ / 0.400f, 0.0f, 1.0f);
            const float hermite_t = 3.0f * u * u - 2.0f * u * u * u;
            t                     = (target_camera_mode_ == 1u) ? hermite_t : (1.0f - hermite_t);
        } else {
            t = (active_camera_mode_ == 1u) ? 1.0f : 0.0f;
        }

        const float aspect = (height_ > 0u)
                                 ? (static_cast<float>(width_) / static_cast<float>(height_))
                                 : (1024.0f / 720.0f);

        // Mode 0: Spherical Arcball Perspective
        glm::vec3 eye_persp    = camera_offset_ + glm::vec3(0.0001f, 2.0001f, -4.9999f);
        glm::vec3 target_persp = camera_offset_;
        if (arcball_.has_poi_lock) {
            eye_persp    = oasis::graphics::ComputeArcballEyePosition(arcball_);
            target_persp = arcball_.poi_origin;
        }
        const glm::mat4 view_persp   = glm::lookAt(eye_persp, target_persp, glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 proj_persp   = glm::perspective(glm::radians(53.13f), aspect, 0.1f, 120.0f);
        const glm::mat4 inv_vp_persp = glm::inverse(proj_persp * view_persp);

        // Mode 1: Cadastral Axonometric / Isometric
        const float     ortho_size = arcball_.radius;
        const glm::mat4 proj_iso   = glm::ortho(
            -ortho_size * aspect, ortho_size * aspect,
            -ortho_size, ortho_size,
            -60.0f, 60.0f
        );
        const glm::vec3 iso_center = arcball_.has_poi_lock
                                         ? glm::vec3(arcball_.poi_origin.x, 0.0f, arcball_.poi_origin.z)
                                         : camera_offset_;
        const glm::vec3 eye_iso    = iso_center + glm::vec3(-16.0001f, 20.0001f, -16.0001f);
        const glm::vec3 target_iso = iso_center + glm::vec3(0.0001f, 0.0001f, 0.0001f);
        const glm::mat4 view_iso   = glm::lookAt(eye_iso, target_iso, glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 inv_vp_iso = glm::inverse(proj_iso * view_iso);

        const float cutaway_z      = static_cast<float>(arcball_.z_cutaway_level);
        const float encoded_mode_w = static_cast<float>(overlay_mode_ * 10u) + (arcball_.has_poi_lock ? 2.0f : t);

        camera_uniforms_.view_matrix   = (1.0f - t) * view_persp + t * view_iso;
        camera_uniforms_.proj_matrix   = (1.0f - t) * proj_persp + t * proj_iso;
        camera_uniforms_.inv_view_proj = (1.0f - t) * inv_vp_persp + t * inv_vp_iso;
        camera_uniforms_.camera_pos    = glm::vec4(glm::mix(eye_persp, eye_iso, t), encoded_mode_w);
        camera_uniforms_.camera_target = glm::mix(glm::vec4(target_persp, cutaway_z), glm::vec4(target_iso, cutaway_z), t);
        camera_uniforms_.viewport_params = glm::vec4(
            static_cast<float>(width_),
            static_cast<float>(height_),
            aspect,
            t
        );
    }

    // Map-agnostic 3D Amanatides-Woo DDA raycast against WorldGrid (y >= 0, respecting z_cutaway_level).
    [[nodiscard]] WorldRaycastHit RaycastWorldGrid(
        int32_t          mouse_x,
        int32_t          mouse_y,
        const WorldGrid& grid,
        int32_t          max_steps = 220
    ) const noexcept {
        WorldRaycastHit result{};
        if (width_ == 0u || height_ == 0u) {
            return result;
        }

        const float ndc_x = (2.0f * (static_cast<float>(mouse_x) + 0.5f)) / static_cast<float>(width_) - 1.0f;
        const float ndc_y = 1.0f - (2.0f * (static_cast<float>(mouse_y) + 0.5f)) / static_cast<float>(height_);

        const glm::vec4 near_clip(ndc_x, ndc_y, 0.0f, 1.0f);
        const glm::vec4 far_clip(ndc_x, ndc_y, 1.0f, 1.0f);

        const glm::vec4 near_world = camera_uniforms_.inv_view_proj * near_clip;
        const glm::vec4 far_world  = camera_uniforms_.inv_view_proj * far_clip;
        if (std::abs(near_world.w) < 1e-6f || std::abs(far_world.w) < 1e-6f) {
            return result;
        }
        const glm::vec3 p_near = glm::vec3(near_world) / near_world.w;
        const glm::vec3 p_far  = glm::vec3(far_world) / far_world.w;

        const float     t        = camera_uniforms_.viewport_params.w;
        const glm::vec3 ro_persp = glm::vec3(camera_uniforms_.camera_pos);
        const glm::vec3 rd_persp = glm::normalize(p_far - ro_persp);
        const glm::vec3 ro_ortho = p_near;
        const glm::vec3 rd_ortho = glm::normalize(p_far - p_near);

        glm::vec3       ro = glm::mix(ro_persp, ro_ortho, t);
        const glm::vec3 rd = glm::normalize(glm::mix(rd_persp, rd_ortho, t));

        // Advance ray into bounded volume y <= 15 if camera starts high above parcel
        if (ro.y > 15.5f && rd.y < -1e-5f) {
            const float t_top = (15.5f - ro.y) / rd.y;
            if (t_top > 0.0f) {
                ro += rd * t_top;
            }
        }

        int32_t map_x = static_cast<int32_t>(std::floor(ro.x));
        int32_t map_y = static_cast<int32_t>(std::floor(ro.y));
        int32_t map_z = static_cast<int32_t>(std::floor(ro.z));

        const int32_t step_x = (rd.x >= 0.0f) ? 1 : -1;
        const int32_t step_y = (rd.y >= 0.0f) ? 1 : -1;
        const int32_t step_z = (rd.z >= 0.0f) ? 1 : -1;

        const float delta_x = (std::abs(rd.x) > 1e-6f) ? std::abs(1.0f / rd.x) : 1e30f;
        const float delta_y = (std::abs(rd.y) > 1e-6f) ? std::abs(1.0f / rd.y) : 1e30f;
        const float delta_z = (std::abs(rd.z) > 1e-6f) ? std::abs(1.0f / rd.z) : 1e30f;

        float side_x = (rd.x >= 0.0f)
                           ? (static_cast<float>(map_x + 1) - ro.x) * delta_x
                           : (ro.x - static_cast<float>(map_x)) * delta_x;
        float side_y = (rd.y >= 0.0f)
                           ? (static_cast<float>(map_y + 1) - ro.y) * delta_y
                           : (ro.y - static_cast<float>(map_y)) * delta_y;
        float side_z = (rd.z >= 0.0f)
                           ? (static_cast<float>(map_z + 1) - ro.z) * delta_z
                           : (ro.z - static_cast<float>(map_z)) * delta_z;

        int32_t last_nx = 0;
        int32_t last_ny = 1;
        int32_t last_nz = 0;

        const int32_t cutaway_y = static_cast<int32_t>(arcball_.z_cutaway_level);

        for (int32_t i = 0; i < max_steps; ++i) {
            if (map_y < 0 && step_y < 0) {
                break;
            }

            if (map_y >= 0 && map_y < cutaway_y) {
                uint16_t mat = LookupDynamicVoxel(map_x, map_y, map_z);
                if (mat == 0u) {
                    mat = grid.GetVoxel(map_x, map_y, map_z);
                }
                if (mat != 0u) {
                    result.hit         = true;
                    result.vx          = map_x;
                    result.vy          = map_y;
                    result.vz          = map_z;
                    result.material_id = mat;
                    result.normal_x    = last_nx;
                    result.normal_y    = last_ny;
                    result.normal_z    = last_nz;
                    result.place_x     = map_x + last_nx;
                    result.place_y     = std::max<int32_t>(1, map_y + last_ny);
                    result.place_z     = map_z + last_nz;
                    return result;
                }
            }

            if (side_x < side_y) {
                if (side_x < side_z) {
                    side_x += delta_x;
                    map_x += step_x;
                    last_nx = -step_x;
                    last_ny = 0;
                    last_nz = 0;
                } else {
                    side_z += delta_z;
                    map_z += step_z;
                    last_nx = 0;
                    last_ny = 0;
                    last_nz = -step_z;
                }
            } else {
                if (side_y < side_z) {
                    side_y += delta_y;
                    map_y += step_y;
                    last_nx = 0;
                    last_ny = -step_y;
                    last_nz = 0;
                } else {
                    side_z += delta_z;
                    map_z += step_z;
                    last_nx = 0;
                    last_ny = 0;
                    last_nz = -step_z;
                }
            }
        }
        return result;
    }

    void RenderFrame(float dt_sec) {
        UpdateCameraMatrices(dt_sec);

#ifdef __EMSCRIPTEN__
        if (!gpu_initialized_ || device_ == nullptr || pipeline_ == nullptr ||
            surface_ == nullptr || bind_group_ == nullptr) {
            ++frame_count_;
            return;
        }

        queue_.WriteBuffer(camera_buffer_, 0, &camera_uniforms_, sizeof(oasis::graphics::CameraUniforms));

        const size_t dyn_upload_bytes = std::min(
            static_cast<size_t>(32u) + static_cast<size_t>(dynamic_voxels_.count) * sizeof(oasis::graphics::GpuDynamicVoxel),
            sizeof(DynamicVoxelStoreGpu)
        );
        queue_.WriteBuffer(dynamic_voxel_buffer_, 0, &dynamic_voxels_, dyn_upload_bytes);

        if (world_chunk_dirty_) {
            queue_.WriteBuffer(world_chunk_buffer_, 0, &world_chunk_, sizeof(WorldChunkGpuBuffer));
            world_chunk_dirty_ = false;
        }

        wgpu::SurfaceTexture surface_tex{};
        surface_.GetCurrentTexture(&surface_tex);
        if (surface_tex.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal &&
            surface_tex.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal) {
            return;
        }
        if (!surface_tex.texture) {
            return;
        }

        wgpu::TextureView backbuffer = surface_tex.texture.CreateView();
        if (!backbuffer) {
            return;
        }

        wgpu::RenderPassColorAttachment color_attachment{};
        color_attachment.view       = backbuffer;
        color_attachment.clearValue = {clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]};
        color_attachment.loadOp     = wgpu::LoadOp::Clear;
        color_attachment.storeOp    = wgpu::StoreOp::Store;

        wgpu::RenderPassDescriptor pass_desc{};
        pass_desc.colorAttachmentCount = 1;
        pass_desc.colorAttachments     = &color_attachment;

        wgpu::CommandEncoder    encoder = device_.CreateCommandEncoder();
        wgpu::RenderPassEncoder pass    = encoder.BeginRenderPass(&pass_desc);
        pass.SetPipeline(pipeline_);
        pass.SetBindGroup(0, bind_group_);
        pass.Draw(3, 1, 0, 0);
        pass.End();

        wgpu::CommandBuffer commands = encoder.Finish();
        queue_.Submit(1, &commands);
#endif
        ++frame_count_;
    }

private:
    [[nodiscard]] uint16_t LookupDynamicVoxel(int32_t x, int32_t y, int32_t z) const noexcept {
        if (dynamic_voxels_.count == 0u) {
            return 0u;
        }
        if (x < dynamic_voxels_.min_x || x > dynamic_voxels_.max_x ||
            y < dynamic_voxels_.min_y || y > dynamic_voxels_.max_y ||
            z < dynamic_voxels_.min_z || z > dynamic_voxels_.max_z) {
            return 0u;
        }
        const uint32_t limit =
            std::min(dynamic_voxels_.count, static_cast<uint32_t>(oasis::graphics::MAX_GPU_DYNAMIC_VOXELS));
        for (uint32_t i = 0u; i < limit; ++i) {
            const auto& v = dynamic_voxels_.voxels[i];
            if (v.x == x && v.y == y && v.z == z) {
                return static_cast<uint16_t>(v.material_id & 0xFFFFu);
            }
        }
        return 0u;
    }

#ifdef __EMSCRIPTEN__
    void SetupPipelineAndBuffers() {
        const std::string_view shader_src = oasis::shaders::RAYMARCH_WGSL;

        wgpu::ShaderSourceWGSL wgsl_desc{};
        wgsl_desc.code = wgpu::StringView{shader_src.data(), shader_src.size()};

        wgpu::ShaderModuleDescriptor sm_desc{};
        sm_desc.nextInChain              = &wgsl_desc;
        wgpu::ShaderModule shader_module = device_.CreateShaderModule(&sm_desc);

        // Explicit 3-binding layout matching raymarch.wgsl:
        // @binding(0): CameraUniforms (Uniform, Vertex | Fragment)
        // @binding(1): DynamicVoxelStore (ReadOnlyStorage, Fragment)
        // @binding(2): WorldChunkBuffer (ReadOnlyStorage, Fragment)
        std::array<wgpu::BindGroupLayoutEntry, 3> bgl_entries{};
        bgl_entries[0].binding               = 0;
        bgl_entries[0].visibility            = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
        bgl_entries[0].buffer.type           = wgpu::BufferBindingType::Uniform;
        bgl_entries[0].buffer.minBindingSize = sizeof(oasis::graphics::CameraUniforms);

        bgl_entries[1].binding               = 1;
        bgl_entries[1].visibility            = wgpu::ShaderStage::Fragment;
        bgl_entries[1].buffer.type           = wgpu::BufferBindingType::ReadOnlyStorage;
        bgl_entries[1].buffer.minBindingSize = sizeof(DynamicVoxelStoreGpu);

        bgl_entries[2].binding               = 2;
        bgl_entries[2].visibility            = wgpu::ShaderStage::Fragment;
        bgl_entries[2].buffer.type           = wgpu::BufferBindingType::ReadOnlyStorage;
        bgl_entries[2].buffer.minBindingSize = sizeof(WorldChunkGpuBuffer);

        wgpu::BindGroupLayoutDescriptor bgl_desc{};
        bgl_desc.entryCount = bgl_entries.size();
        bgl_desc.entries    = bgl_entries.data();
        bind_group_layout_  = device_.CreateBindGroupLayout(&bgl_desc);

        wgpu::PipelineLayoutDescriptor pl_desc{};
        pl_desc.bindGroupLayoutCount         = 1;
        pl_desc.bindGroupLayouts             = &bind_group_layout_;
        wgpu::PipelineLayout pipeline_layout = device_.CreatePipelineLayout(&pl_desc);

        wgpu::ColorTargetState color_target{};
        color_target.format    = format_;
        color_target.writeMask = wgpu::ColorWriteMask::All;

        wgpu::FragmentState frag_state{};
        frag_state.module      = shader_module;
        frag_state.entryPoint  = wgpu::StringView{"fs_main"};
        frag_state.targetCount = 1;
        frag_state.targets     = &color_target;

        wgpu::PrimitiveState prim_state{};
        prim_state.topology  = wgpu::PrimitiveTopology::TriangleList;
        prim_state.cullMode  = wgpu::CullMode::None;
        prim_state.frontFace = wgpu::FrontFace::CCW;

        wgpu::RenderPipelineDescriptor rp_desc{};
        rp_desc.layout              = pipeline_layout;
        rp_desc.vertex.module       = shader_module;
        rp_desc.vertex.entryPoint   = wgpu::StringView{"vs_main"};
        rp_desc.fragment            = &frag_state;
        rp_desc.primitive           = prim_state;
        rp_desc.multisample.count   = 1;
        rp_desc.multisample.mask    = 0xFFFFFFFFu;
        pipeline_                   = device_.CreateRenderPipeline(&rp_desc);

        wgpu::BufferDescriptor cam_buf_desc{};
        cam_buf_desc.label = wgpu::StringView{"CameraUniformBuffer"};
        cam_buf_desc.size  = sizeof(oasis::graphics::CameraUniforms);
        cam_buf_desc.usage = wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst;
        camera_buffer_     = device_.CreateBuffer(&cam_buf_desc);

        wgpu::BufferDescriptor dyn_buf_desc{};
        dyn_buf_desc.label    = wgpu::StringView{"DynamicVoxelStorageBuffer"};
        dyn_buf_desc.size     = sizeof(DynamicVoxelStoreGpu);
        dyn_buf_desc.usage    = wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopyDst;
        dynamic_voxel_buffer_ = device_.CreateBuffer(&dyn_buf_desc);

        wgpu::BufferDescriptor chunk_buf_desc{};
        chunk_buf_desc.label = wgpu::StringView{"WorldChunkStorageBuffer"};
        chunk_buf_desc.size  = sizeof(WorldChunkGpuBuffer);
        chunk_buf_desc.usage = wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopyDst;
        world_chunk_buffer_  = device_.CreateBuffer(&chunk_buf_desc);

        std::array<wgpu::BindGroupEntry, 3> bg_entries{};
        bg_entries[0].binding = 0;
        bg_entries[0].buffer  = camera_buffer_;
        bg_entries[0].offset  = 0;
        bg_entries[0].size    = sizeof(oasis::graphics::CameraUniforms);

        bg_entries[1].binding = 1;
        bg_entries[1].buffer  = dynamic_voxel_buffer_;
        bg_entries[1].offset  = 0;
        bg_entries[1].size    = sizeof(DynamicVoxelStoreGpu);

        bg_entries[2].binding = 2;
        bg_entries[2].buffer  = world_chunk_buffer_;
        bg_entries[2].offset  = 0;
        bg_entries[2].size    = sizeof(WorldChunkGpuBuffer);

        wgpu::BindGroupDescriptor bg_desc{};
        bg_desc.layout     = bind_group_layout_;
        bg_desc.entryCount = bg_entries.size();
        bg_desc.entries    = bg_entries.data();
        bind_group_        = device_.CreateBindGroup(&bg_desc);

        queue_.WriteBuffer(world_chunk_buffer_, 0, &world_chunk_, sizeof(WorldChunkGpuBuffer));
        world_chunk_dirty_ = false;
    }

    void InitWebGpuAsync() {
        const int webgpu_ready = EM_ASM_INT({
            if (typeof window !== 'undefined' && window.location &&
                (window.location.hostname === '0.0.0.0' || window.location.hostname === '[::]' || window.location.hostname === '::')) {
                var targetUrl = window.location.protocol + '//localhost:' + window.location.port +
                                window.location.pathname + window.location.search + window.location.hash;
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
            spdlog::error("[Oasis WebGPU] WebGPU unavailable (status={}). Open via http://localhost in a WebGPU browser.", webgpu_ready);
            return;
        }

        wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector canvas_desc{};
        canvas_desc.selector = wgpu::StringView{"#canvas"};

        wgpu::SurfaceDescriptor surface_desc{};
        surface_desc.nextInChain = &canvas_desc;

        instance_ = wgpu::CreateInstance();
        surface_  = instance_.CreateSurface(&surface_desc);

        wgpu::RequestAdapterOptions adapter_opts{};
        adapter_opts.compatibleSurface = surface_;

        instance_.RequestAdapter(
            &adapter_opts,
            wgpu::CallbackMode::AllowSpontaneous,
            [this](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView msg) {
                if (status != wgpu::RequestAdapterStatus::Success) {
                    spdlog::error(
                        "[Oasis WebGPU] Adapter request failed: {}",
                        msg.data ? std::string_view(msg.data, msg.length) : "unknown"
                    );
                    return;
                }
                adapter_ = adapter;

                wgpu::DeviceDescriptor dev_desc{};
                dev_desc.SetUncapturedErrorCallback(
                    [](const wgpu::Device&, wgpu::ErrorType err_type, wgpu::StringView err_msg) {
                        spdlog::error(
                            "[Oasis WebGPU] Device error ({}): {}",
                            static_cast<int>(err_type),
                            err_msg.data ? std::string_view(err_msg.data, err_msg.length) : "unknown"
                        );
                    }
                );

                adapter_.RequestDevice(
                    &dev_desc,
                    wgpu::CallbackMode::AllowSpontaneous,
                    [this](wgpu::RequestDeviceStatus dev_status, wgpu::Device device, wgpu::StringView dev_msg) {
                        if (dev_status != wgpu::RequestDeviceStatus::Success) {
                            spdlog::error(
                                "[Oasis WebGPU] Device request failed: {}",
                                dev_msg.data ? std::string_view(dev_msg.data, dev_msg.length) : "unknown"
                            );
                            return;
                        }
                        device_ = device;
                        queue_  = device_.GetQueue();

                        wgpu::SurfaceCapabilities caps{};
                        surface_.GetCapabilities(adapter_, &caps);
                        format_ = (caps.formatCount > 0u) ? caps.formats[0] : wgpu::TextureFormat::BGRA8Unorm;

                        wgpu::SurfaceConfiguration cfg{};
                        cfg.device      = device_;
                        cfg.format      = format_;
                        cfg.usage       = wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc;
                        cfg.width       = width_;
                        cfg.height      = height_;
                        cfg.alphaMode   = wgpu::CompositeAlphaMode::Auto;
                        cfg.presentMode = wgpu::PresentMode::Fifo;
                        surface_.Configure(&cfg);

                        SetupPipelineAndBuffers();
                        gpu_initialized_ = true;
                        spdlog::info("[Oasis WebGPU] 3-binding WorldChunk pipeline initialized ({}x{}).", width_, height_);
                    }
                );
            }
        );
    }
#endif

    // 16-byte aligned GPU payload mirrors (offset 0 .. 147,808)
    oasis::graphics::CameraUniforms camera_uniforms_{};
    DynamicVoxelStoreGpu            dynamic_voxels_{};
    WorldChunkGpuBuffer             world_chunk_{};

    // 16-byte block: monotonic counters
    uint64_t frame_count_{0ULL};
    uint64_t chunk_sync_generation_{0ULL};

    // Pointer / handle block (exact multiple of 16 bytes on both 64-bit native and 32-bit wasm32)
    SDL_Window* sdl_window_{nullptr};
#ifdef __EMSCRIPTEN__
    wgpu::Instance        instance_{nullptr};
    wgpu::Adapter         adapter_{nullptr};
    wgpu::Device          device_{nullptr};
    wgpu::Queue           queue_{nullptr};
    wgpu::Surface         surface_{nullptr};
    wgpu::RenderPipeline  pipeline_{nullptr};
    wgpu::Buffer          camera_buffer_{nullptr};
    wgpu::Buffer          dynamic_voxel_buffer_{nullptr};
    wgpu::Buffer          world_chunk_buffer_{nullptr};
    wgpu::BindGroupLayout bind_group_layout_{nullptr};
    wgpu::BindGroup       bind_group_{nullptr};
#else
    [[maybe_unused]] uint64_t native_ptr_pad_{0ULL};
#endif

    // 112-byte camera & viewport block (60 + 12 + 16 + 24 = 112 bytes)
    oasis::graphics::ArcballCameraState  arcball_{};
    glm::vec3                            camera_offset_{0.0f, 0.0f, 0.0f};
    [[maybe_unused]] float               clear_color_[4]{0.05f, 0.07f, 0.11f, 1.0f};
    uint32_t                             width_{1024u};
    uint32_t                             height_{720u};
    uint32_t                             active_camera_mode_{0u};
    uint32_t                             target_camera_mode_{0u};
    uint32_t                             overlay_mode_{0u};
    float                                transition_elapsed_sec_{0.0f};

#ifdef __EMSCRIPTEN__
    wgpu::TextureFormat                  format_{wgpu::TextureFormat::Undefined};
    [[maybe_unused]] uint32_t            em_format_pad_[3]{0u, 0u, 0u};
#endif

    // 16-byte boolean & tail alignment block
    bool                     sdl_initialized_{false};
    bool                     gpu_initialized_{false};
    bool                     is_transitioning_{false};
    bool                     world_chunk_dirty_{true};
    [[maybe_unused]] uint8_t tail_pad_[12]{};
};

} // namespace oasis::engine::render
