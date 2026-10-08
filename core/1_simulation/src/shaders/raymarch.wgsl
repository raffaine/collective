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
            return 8u; // Material 8: Utility timber crossarm (#3e2723)
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

    let lit_color = base_color * lighting * face_tint;

    // Distance fog blend into horizon
    let sky = get_sky_color(rd);
    let fog = 1.0 - exp(-hit_dist * 0.035);
    let final_color = mix(lit_color, sky, fog);

    return vec4<f32>(final_color, 1.0);
}
