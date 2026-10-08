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

// Procedural voxel query: returns 0u for empty air, or material ID > 0u
fn get_voxel_material(pos: vec3<i32>) -> u32 {
    // 1. Checkered floor at y == 0
    if (pos.y == 0) {
        if (abs(pos.x) <= 12 && pos.z >= -6 && pos.z <= 16) {
            let check = ((pos.x + pos.z) & 1);
            if (check == 0) {
                return 1u; // Slate tile
            } else {
                return 2u; // Sandstone tile
            }
        }
    }

    // 2. Central floating monolith structure:
    // Hovering at y in [1, 2], centered on x in [-1, 1], z in [0, 2]
    if (pos.y >= 1 && pos.y <= 2 && abs(pos.x) <= 1 && pos.z >= 0 && pos.z <= 2) {
        return 3u; // Vibrant amber / gold monolith
    }

    // 3. Four corner decorative pillars at y in [1, 3]
    if (pos.y >= 1 && pos.y <= 3) {
        if ((pos.x == -3 || pos.x == 3) && (pos.z == -1 || pos.z == 5)) {
            return 4u; // Teal pillar
        }
    }

    return 0u; // Air
}

fn get_sky_color(rd: vec3<f32>) -> vec3<f32> {
    let sky_top = vec3<f32>(0.15, 0.35, 0.65);
    let sky_horizon = vec3<f32>(0.55, 0.70, 0.85);
    let t = clamp(rd.y * 0.5 + 0.5, 0.0, 1.0);
    return mix(sky_horizon, sky_top, t);
}

@fragment
fn fs_main(in : VertexOutput) -> @location(0) vec4<f32> {
    // Aspect ratio correction (800x600 viewport)
    let aspect = 800.0 / 600.0;
    let ndc = in.uv * 2.0 - vec2<f32>(1.0, 1.0);
    let screen_p = vec2<f32>(ndc.x * aspect, ndc.y);

    // Fixed virtual camera setup:
    // Origin at (0.0, 2.0, -5.0) with tiny epsilon offset to avoid integer grid boundary singularities
    let ro = vec3<f32>(0.0001, 2.0001, -4.9999);
    let cam_target = vec3<f32>(0.0, 0.0, 0.0);

    let cam_fwd = normalize(cam_target - ro);
    let world_up = vec3<f32>(0.0, 1.0, 0.0);
    let cam_right = normalize(cross(world_up, cam_fwd));
    let cam_up = cross(cam_fwd, cam_right);

    // Camera ray direction with 1.5 focal length (~53 deg vertical FOV)
    let focal_length = 1.5;
    let rd = normalize(screen_p.x * cam_right + screen_p.y * cam_up + focal_length * cam_fwd);

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

    const MAX_STEPS: i32 = 96;
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

        // Bounding space early exit: avoid wasted ray steps outside populated scene volume
        if ((mapPos.y > 4 && step.y > 0) || (mapPos.y < 0 && step.y < 0) || (mapPos.z > 18 && step.z > 0)) {
            break;
        }

        let mat = get_voxel_material(mapPos);
        if (mat != 0u) {
            hit_mat = mat;
            break;
        }
    }

    // Sky miss
    if (hit_mat == 0u) {
        return vec4<f32>(get_sky_color(rd), 1.0);
    }

    // Voxel hit material coloring
    var base_color: vec3<f32>;
    if (hit_mat == 1u) {
        base_color = vec3<f32>(0.35, 0.38, 0.42); // Slate floor tile
    } else if (hit_mat == 2u) {
        base_color = vec3<f32>(0.60, 0.55, 0.48); // Sandstone floor tile
    } else if (hit_mat == 3u) {
        base_color = vec3<f32>(0.92, 0.68, 0.22); // Amber gold monolith
    } else if (hit_mat == 4u) {
        base_color = vec3<f32>(0.22, 0.78, 0.68); // Teal pillar
    } else {
        base_color = vec3<f32>(0.50, 0.50, 0.50);
    }

    // Directional light from upper-right-front
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

    let lit_color = base_color * lighting * face_tint;

    // Distance fog blend into horizon
    let sky = get_sky_color(rd);
    let fog = 1.0 - exp(-hit_dist * 0.035);
    let final_color = mix(lit_color, sky, fog);

    return vec4<f32>(final_color, 1.0);
}
