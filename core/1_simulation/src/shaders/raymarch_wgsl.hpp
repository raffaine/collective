#pragma once

namespace oasis::shaders {

inline constexpr const char* RAYMARCH_WGSL = R"wgsl(
struct CameraUniforms {
    view_matrix : mat4x4<f32>,
    proj_matrix : mat4x4<f32>,
    inv_view_proj : mat4x4<f32>,
    camera_pos : vec4<f32>,
    camera_target : vec4<f32>,
    viewport_params : vec4<f32>,
    cursor_highlight : vec4<f32>,
    avatar_pos : vec4<f32>,
    avatar_params : vec4<f32>,
};

@group(0) @binding(0)
var<uniform> camera : CameraUniforms;

struct DynamicVoxel {
    pos : vec3<i32>,
    material_id : u32,
};

struct DynamicVoxelStore {
    count : u32,
    min_x : i32,
    min_y : i32,
    min_z : i32,
    max_x : i32,
    max_y : i32,
    max_z : i32,
    _pad : u32,
    voxels : array<DynamicVoxel, 1024>,
};

@group(0) @binding(1)
var<storage, read> dynamic_voxels : DynamicVoxelStore;

fn get_dynamic_voxel_material(pos : vec3<i32>) -> u32 {
    let count = dynamic_voxels.count;
    if (count == 0u) {
        return 0u;
    }
    // Fast AABB early rejection
    if (pos.x < dynamic_voxels.min_x || pos.x > dynamic_voxels.max_x ||
        pos.y < dynamic_voxels.min_y || pos.y > dynamic_voxels.max_y ||
        pos.z < dynamic_voxels.min_z || pos.z > dynamic_voxels.max_z) {
        return 0u;
    }
    let limit = min(count, 1024u);
    for (var i = 0u; i < limit; i = i + 1u) {
        let v = dynamic_voxels.voxels[i];
        if (v.pos.x == pos.x && v.pos.y == pos.y && v.pos.z == pos.z) {
            return v.material_id;
        }
    }
    return 0u;
}

struct VertexOutput {
    @builtin(position) position : vec4<f32>,
    @location(0) uv : vec2<f32>,
};

@vertex
fn vs_main(@builtin(vertex_index) vertex_index : u32) -> VertexOutput {
    var out : VertexOutput;
    
    // Explicit 3-vertex oversized triangle covering [-1, 1] on both axes
    var pos = array<vec2<f32>, 3>(
        vec2<f32>(-1.0, -1.0),
        vec2<f32>( 3.0, -1.0),
        vec2<f32>(-1.0,  3.0)
    );
    
    let p = pos[vertex_index];
    out.position = vec4<f32>(p.x, p.y, 0.0, 1.0);
    
    // Map NDC [-1, 1] to UV [0, 1] across the visible viewport
    out.uv = p * 0.5 + vec2<f32>(0.5, 0.5);
    return out;
}

// Procedural Suburban Sprawl Legacy Biome ("Lot 402 & Cul-de-sac")
// Returns 0u for empty air, or 1u..10u for distinct procedural materials
fn get_voxel_material(pos: vec3<i32>) -> u32 {
    let dyn_mat = get_dynamic_voxel_material(pos);
    if (dyn_mat != 0u) {
        return dyn_mat;
    }

    let absX = abs(pos.x);
    let lotZ = (pos.z % 16 + 16) % 16; // 16-voxel periodic residential lot pitch

    // --- Layer 0: Ground Plane & Road Infrastructure ---
    if (pos.y == 0) {
        // Central Asphalt Roadway (|x| <= 3)
        if (absX <= 3) {
            // Intermittent yellow double centerline
            if (pos.x == 0 && (pos.z % 4 >= 2 || pos.z % 4 <= -2)) {
                return 2u; // Material 2: Yellow centerline (#d4af37)
            }
            return 1u; // Material 1: Asphalt roadway (#222428)
        }
        // Concrete Curbs & Sidewalks (|x| in [4, 5])
        if (absX == 4 || absX == 5) {
            return 3u; // Material 3: Concrete sidewalk/curbs (#8c8e90)
        }
        // Concrete driveways leading to garages
        if (absX >= 6 && absX <= 8 && lotZ >= 1 && lotZ <= 3) {
            return 3u; // Material 3: Concrete driveway
        }
        // Parched lawn / dead suburban grass (|x| in [6, 24])
        if (absX >= 6 && absX <= 24) {
            return 4u; // Material 4: Parched lawn / dead grass (#7a7258)
        }
    }

    // --- Layer 1-8: Legacy Electrical Grid Infrastructure ---
    if (pos.y >= 1 && pos.y <= 8) {
        // Wooden utility poles along curb line (x == 5, at lot boundaries lotZ == 0)
        if (pos.x == 5 && lotZ == 0 && pos.y <= 7) {
            return 8u; // Material 8: Utility timber pole (#3e2723)
        }
        // Crossarm at pole top (y == 7, x in [4, 6], lotZ == 0)
        if (pos.x >= 4 && pos.x <= 6 && lotZ == 0 && pos.y == 7) {
            return 8u; // Utility timber crossarm (#3e2723)
        }
        // Pole-mounted distribution transformer (x == 5, lotZ == 1, y == 6)
        if (pos.x == 5 && lotZ == 1 && pos.y == 6) {
            return 9u; // Material 9: Transformer / grid hardware (#9e9e9e)
        }
        // Longitudinal overhead power line wire (x == 4, y == 7, z in [-10, 40])
        if (pos.x == 4 && pos.y == 7 && pos.z >= -10 && pos.z <= 40) {
            return 9u; // Material 9: Conductor wire / grid hardware (#9e9e9e)
        }
    }

    // --- Layer 1-6: Suburban Residences (Lot 402 & neighbors) ---
    if (absX >= 9 && absX <= 17 && lotZ >= 2 && lotZ <= 10) {
        // Front reflective windows facing the street (at facade absX == 9, lotZ in {4, 8}, y == 2)
        if (absX == 9 && (lotZ == 4 || lotZ == 8) && pos.y == 2) {
            return 7u; // Material 7: Window glass (#3d5a80)
        }
        // Ground & second floor walls (y in [1, 3])
        if (pos.y >= 1 && pos.y <= 3) {
            return 5u; // Material 5: Weathered wood siding (#5c6b73)
        }
        // Sloped gable roof (y in [4, 6])
        if (pos.y >= 4 && pos.y <= 6) {
            let roof_inset = pos.y - 3;
            if (absX >= (9 + roof_inset) && absX <= (17 - roof_inset) &&
                lotZ >= (2 + roof_inset) && lotZ <= (10 - roof_inset)) {
                return 6u; // Material 6: Dark shingle roof (#2b2d42)
            }
        }
    }

    // --- Layer 1: Property Boundary Fences ---
    if (pos.y == 1 && (lotZ == 15 || lotZ == 1) && absX >= 7 && absX <= 20) {
        return 10u; // Material 10: Boundary fences (#6d4c41)
    }

    return 0u; // Air
}

fn get_sky_color(rd: vec3<f32>) -> vec3<f32> {
    let sky_top = vec3<f32>(0.15, 0.35, 0.65);
    let sky_horizon = vec3<f32>(0.55, 0.70, 0.85);
    let t = clamp(rd.y * 0.5 + 0.5, 0.0, 1.0);
    return mix(sky_horizon, sky_top, t);
}

fn intersect_sphere(ro: vec3<f32>, rd: vec3<f32>, center: vec3<f32>, radius: f32) -> f32 {
    let oc = ro - center;
    let b = dot(oc, rd);
    let c = dot(oc, oc) - radius * radius;
    let disc = b * b - c;
    if (disc < 0.0) {
        return -1.0;
    }
    let t = -b - sqrt(disc);
    if (t < 0.0) {
        return -1.0;
    }
    return t;
}

@fragment
fn fs_main(in : VertexOutput) -> @location(0) vec4<f32> {
    let aspect = camera.viewport_params.z;
    let ndc = in.uv * 2.0 - vec2<f32>(1.0, 1.0);

    // Unproject near plane (z=0.0) and far plane (z=1.0) into world space
    let near_h = camera.inv_view_proj * vec4<f32>(ndc.x, ndc.y, 0.0, 1.0);
    let far_h  = camera.inv_view_proj * vec4<f32>(ndc.x, ndc.y, 1.0, 1.0);
    let p_near = near_h.xyz / near_h.w;
    let p_far  = far_h.xyz / far_h.w;

    // Transition parameter t: 0.0 = 3rd-Person Perspective, 1.0 = Cadastral Isometric
    let t = camera.viewport_params.w;

    // Perspective ray origin (camera eye) and direction towards unprojected far plane
    let ro_persp = camera.camera_pos.xyz;
    let rd_persp = normalize(p_far - ro_persp);

    // Cadastral/Orthographic ray origin (near plane position) and parallel ray direction
    let ro_ortho = p_near;
    let rd_ortho = normalize(p_far - p_near);

    // Blend ray origin and ray direction using transition parameter t
    var ro = mix(ro_persp, ro_ortho, t);
    var rd = normalize(mix(rd_persp, rd_ortho, t));

    // Amanatides & Woo 3D DDA Initialization
    var mapPos = vec3<i32>(floor(ro));

    var step = vec3<i32>(1, 1, 1);
    if (rd.x < 0.0) { step.x = -1; }
    if (rd.y < 0.0) { step.y = -1; }
    if (rd.z < 0.0) { step.z = -1; }

    // Delta distance per axis
    let deltaDist = abs(vec3<f32>(1.0) / max(abs(rd), vec3<f32>(1e-6)));

    // Side distance to first boundary
    var sideDist = vec3<f32>(0.0);
    if (step.x > 0) {
        sideDist.x = (f32(mapPos.x + 1) - ro.x) * deltaDist.x;
    } else {
        sideDist.x = (ro.x - f32(mapPos.x)) * deltaDist.x;
    }
    if (step.y > 0) {
        sideDist.y = (f32(mapPos.y + 1) - ro.y) * deltaDist.y;
    } else {
        sideDist.y = (ro.y - f32(mapPos.y)) * deltaDist.y;
    }
    if (step.z > 0) {
        sideDist.z = (f32(mapPos.z + 1) - ro.z) * deltaDist.z;
    } else {
        sideDist.z = (ro.z - f32(mapPos.z)) * deltaDist.z;
    }

    var hit_dist: f32 = 0.0;
    var hit_normal = vec3<f32>(0.0);
    var hit_mat: u32 = 0u;

    const MAX_STEPS: i32 = 128;
    for (var i = 0; i < MAX_STEPS; i++) {
        // Step along axis with minimum distance to next voxel boundary
        if (sideDist.x < sideDist.y) {
            if (sideDist.x < sideDist.z) {
                hit_dist = sideDist.x;
                sideDist.x += deltaDist.x;
                mapPos.x += step.x;
                hit_normal = vec3<f32>(-f32(step.x), 0.0, 0.0);
            } else {
                hit_dist = sideDist.z;
                sideDist.z += deltaDist.z;
                mapPos.z += step.z;
                hit_normal = vec3<f32>(0.0, 0.0, -f32(step.z));
            }
        } else {
            if (sideDist.y < sideDist.z) {
                hit_dist = sideDist.y;
                sideDist.y += deltaDist.y;
                mapPos.y += step.y;
                hit_normal = vec3<f32>(0.0, -f32(step.y), 0.0);
            } else {
                hit_dist = sideDist.z;
                sideDist.z += deltaDist.z;
                mapPos.z += step.z;
                hit_normal = vec3<f32>(0.0, 0.0, -f32(step.z));
            }
        }

        // Bounding space early exit: volume y in [0, 12], z in [-10, 42], |x| <= 28
        if ((mapPos.y > 12 && step.y > 0) || (mapPos.y < 0 && step.y < 0) ||
            (mapPos.z > 42 && step.z > 0) || (mapPos.z < -10 && step.z < 0) ||
            (mapPos.x > 28 && step.x > 0) || (mapPos.x < -28 && step.x < 0)) {
            break;
        }

        let mat = get_voxel_material(mapPos);
        if (mat != 0u) {
            hit_mat = mat;
            break;
        }
    }

    let voxel_dist = select(1e9, hit_dist, hit_mat != 0u);

    // Founder Avatar: Hovering Plumbob Sphere
    if (camera.avatar_params.w > 0.5) {
        let hover_offset = 0.40 + sin(camera.avatar_pos.w * 3.5) * 0.08;
        let avatar_center = camera.avatar_pos.xyz + vec3<f32>(0.0, hover_offset, 0.0);
        let avatar_radius = 0.32;

        let t_avatar = intersect_sphere(ro, rd, avatar_center, avatar_radius);

        if (t_avatar > 0.0 && t_avatar < voxel_dist) {
            let hit_pos = ro + rd * t_avatar;
            let norm = normalize(hit_pos - avatar_center);
            let view_dir = -rd;
            let light_dir = normalize(vec3<f32>(0.5, 0.8, -0.4));

            let diff = max(dot(norm, light_dir), 0.0);
            let view_dot = max(dot(norm, view_dir), 0.0);
            let fresnel = pow(1.0 - view_dot, 2.5);

            // State-driven Sims Plumbob color:
            // 0 = IDLE (Emerald #00e676)
            // 1 = NAVIGATING (Cyan #00b0ff)
            // 2 = CONSTRUCTING (Amber #ffd600)
            // 4 = EXHAUSTED / High Stress (Crimson #ff1744)
            let action_state = u32(round(camera.avatar_params.x));
            var base_color = vec3<f32>(0.00, 0.90, 0.45); // Emerald Green
            var glow_color = vec3<f32>(0.35, 1.00, 0.65);

            if (action_state == 1u) {
                base_color = vec3<f32>(0.00, 0.65, 0.95); // Cyan
                glow_color = vec3<f32>(0.40, 0.85, 1.00);
            } else if (action_state == 2u) {
                base_color = vec3<f32>(0.95, 0.75, 0.05); // Amber Gold
                glow_color = vec3<f32>(1.00, 0.90, 0.40);
            } else if (action_state == 4u || camera.avatar_params.y > 0.75) {
                base_color = vec3<f32>(0.95, 0.10, 0.15); // Crimson Warning
                glow_color = vec3<f32>(1.00, 0.40, 0.45);
            }

            let pulse = 0.85 + 0.15 * sin(camera.avatar_pos.w * 5.0);
            let lit_avatar = base_color * (0.35 + 0.65 * diff) + glow_color * pulse * (fresnel * 0.85 + 0.25);

            let sky = get_sky_color(rd);
            let fog = 1.0 - exp(-t_avatar * 0.035);
            let final_avatar = mix(lit_avatar, sky, fog);
            return vec4<f32>(final_avatar, 1.0);
        }
    }

    // Sky miss
    if (hit_mat == 0u) {
        return vec4<f32>(get_sky_color(rd), 1.0);
    }

    // 10 distinct procedural materials for Suburban Sprawl biome
    var base_color: vec3<f32>;
    if (hit_mat == 1u) {
        base_color = vec3<f32>(0.133, 0.141, 0.157); // Material 1: Asphalt roadway (#222428)
    } else if (hit_mat == 2u) {
        base_color = vec3<f32>(0.831, 0.686, 0.216); // Material 2: Yellow centerline (#d4af37)
    } else if (hit_mat == 3u) {
        base_color = vec3<f32>(0.549, 0.557, 0.565); // Material 3: Concrete sidewalk/curbs (#8c8e90)
    } else if (hit_mat == 4u) {
        base_color = vec3<f32>(0.478, 0.447, 0.345); // Material 4: Parched lawn / dead grass (#7a7258)
    } else if (hit_mat == 5u) {
        base_color = vec3<f32>(0.361, 0.420, 0.451); // Material 5: Weathered wood siding (#5c6b73)
    } else if (hit_mat == 6u) {
        base_color = vec3<f32>(0.169, 0.176, 0.259); // Material 6: Dark shingle roofs (#2b2d42)
    } else if (hit_mat == 7u) {
        base_color = vec3<f32>(0.239, 0.353, 0.502); // Material 7: Window glass (#3d5a80)
    } else if (hit_mat == 8u) {
        base_color = vec3<f32>(0.243, 0.153, 0.137); // Material 8: Utility timber poles (#3e2723)
    } else if (hit_mat == 9u) {
        base_color = vec3<f32>(0.620, 0.620, 0.620); // Material 9: Transformers / grid hardware (#9e9e9e)
    } else if (hit_mat == 10u) {
        base_color = vec3<f32>(0.427, 0.298, 0.255); // Material 10: Boundary fences (#6d4c41)
    } else if (hit_mat == 11u) {
        base_color = vec3<f32>(0.102, 0.137, 0.494); // Material 11: Solar Panel photovoltaic (#1a237e)
    } else if (hit_mat == 12u) {
        base_color = vec3<f32>(0.000, 0.412, 0.361); // Material 12: Rainwater Catchment cistern (#00695c)
    } else {
        base_color = vec3<f32>(0.50, 0.50, 0.50);
    }

    // Directional lighting from upper-right-front
    let light_dir = normalize(vec3<f32>(0.5, 0.8, -0.4));
    let diff = max(dot(hit_normal, light_dir), 0.0);
    let ambient = 0.32;
    let lighting = ambient + 0.68 * diff;

    // Face orientation shading
    var face_tint = 1.0;
    if (hit_normal.y > 0.5) {
        face_tint = 1.15; // Top face highlight
    } else if (hit_normal.y < -0.5) {
        face_tint = 0.60; // Bottom face shadow
    } else if (abs(hit_normal.x) > 0.5) {
        face_tint = 0.85; // X-axis side faces
    } else {
        face_tint = 0.95; // Z-axis front/back faces
    }

    var lit_color = base_color * lighting * face_tint;

    // Contact shadow cast by Founder avatar onto terrain
    if (camera.avatar_params.w > 0.5) {
        let voxel_pos_hit = ro + rd * hit_dist;
        let ground_dist = length(voxel_pos_hit.xz - camera.avatar_pos.xz);
        if (ground_dist < 0.65 && voxel_pos_hit.y <= camera.avatar_pos.y + 0.1) {
            let shadow_factor = smoothstep(0.65, 0.15, ground_dist) * 0.45;
            lit_color = lit_color * (1.0 - shadow_factor);
        }
    }

    // Holographic cursor selection highlight when active
    if (camera.cursor_highlight.w > 0.5) {
        let cursor_voxel = vec3<i32>(floor(camera.cursor_highlight.xyz + 0.5));
        if (all(mapPos == cursor_voxel)) {
            lit_color = mix(lit_color, vec3<f32>(0.2, 0.9, 1.0), 0.55);
        }
    }

    // Distance fog blend into horizon
    let sky = get_sky_color(rd);
    let fog = 1.0 - exp(-hit_dist * 0.035);
    let final_color = mix(lit_color, sky, fog);

    return vec4<f32>(final_color, 1.0);
}
)wgsl";

} // namespace oasis::shaders
