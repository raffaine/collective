#pragma once

#include <glm/glm.hpp>
#include <cstddef>

namespace oasis::graphics {

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
static_assert(sizeof(CameraUniforms) == 288, "CameraUniforms must be exactly 288 bytes.");
static_assert(alignof(CameraUniforms) == 16, "CameraUniforms must be 16-byte aligned.");
#endif

} // namespace oasis::graphics
