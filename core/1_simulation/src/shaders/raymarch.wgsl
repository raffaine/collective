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

// Procedural Suburban Sprawl & Hollow Lot 402 Homestead ("Node 0 Study, Roof & Backyard")
// Strictly synchronized 1-to-1 with GetProceduralVoxelMaterial in raycaster.hpp
fn get_voxel_material(pos: vec3<i32>) -> u32 {
    let dyn_mat = get_dynamic_voxel_material(pos);
    if (dyn_mat != 0u) {
        return dyn_mat;
    }

    let absX = abs(pos.x);
    let lotZ = (pos.z % 16 + 16) % 16; // 16-voxel periodic residential lot pitch

    // --- Layer 0: Ground Plane, Road Infrastructure, Interior Floor & Backyard ---
    if (pos.y == 0) {
        // Central Asphalt Roadway (|x| <= 3)
        if (absX <= 3) {
            // Intermittent yellow double centerline
            if (pos.x == 0 && (pos.z % 4 >= 2 || pos.z % 4 <= -2)) {
                return 2u; // Material 2: Yellow centerline
            }
            return 1u; // Material 1: Asphalt roadway
        }
        // Concrete Curbs & Sidewalks (|x| in [4, 5])
        if (absX == 4 || absX == 5) {
            return 3u; // Material 3: Concrete sidewalk/curbs
        }
        // Concrete driveways leading to garages
        if (absX >= 6 && absX <= 8 && lotZ >= 1 && lotZ <= 3) {
            return 3u; // Material 3: Concrete driveway
        }
        // Lawn / Interior Oak Floor / Backyard Loam (|x| in [6, 64])
        if (absX >= 6 && absX <= 64) {
            return 4u; // Material 4: Lawn / interior floor / backyard loam
        }
    }

    // --- Layer 1-8: Legacy Electrical Grid Infrastructure (kept clear of Lot 402 foreground camera at z in [-2, 14]) ---
    if (pos.y >= 1 && pos.y <= 8 && (pos.z < -2 || pos.z > 14)) {
        // Wooden utility poles along curb line (x == 5, at lot boundaries lotZ == 0)
        if (pos.x == 5 && lotZ == 0 && pos.y <= 7) {
            return 8u; // Material 8: Utility timber pole
        }
        // Crossarm at pole top (y == 7, x in [4, 6], lotZ == 0)
        if (pos.x >= 4 && pos.x <= 6 && lotZ == 0 && pos.y == 7) {
            return 8u; // Material 8: Utility timber crossarm
        }
        // Pole-mounted distribution transformer (x == 5, lotZ == 1, y == 6)
        if (pos.x == 5 && lotZ == 1 && pos.y == 6) {
            return 9u; // Material 9: Transformer / grid hardware
        }
        // Longitudinal overhead power line wire (x == 4, y == 7, z in [-10, 40])
        if (pos.x == 4 && pos.y == 7 && pos.z >= -10 && pos.z <= 40) {
            return 9u; // Material 9: Conductor wire / grid hardware
        }
    }

    // --- Layer 1-6: Hollow Suburban Residences (Lot 402 & neighbors) ---
    if (absX >= 9 && absX <= 17 && lotZ >= 2 && lotZ <= 10) {
        // Sloped gable roof (y in [4, 6])
        if (pos.y >= 4 && pos.y <= 6) {
            let roof_inset = pos.y - 3;
            if (absX >= (9 + roof_inset) && absX <= (17 - roof_inset) &&
                lotZ >= (2 + roof_inset) && lotZ <= (10 - roof_inset)) {
                return 6u; // Material 6: Architectural shingle roof
            }
        }
        // Ground Floor Walls, Doors, Windows, Node 0 Server Rack, & Hollow Interior (y in [1, 3])
        if (pos.y >= 1 && pos.y <= 3) {
            // Front Door (absX == 9) & Back Patio Door (absX == 17) at lotZ == 6, y <= 2
            if ((absX == 9 || absX == 17) && lotZ == 6 && pos.y <= 2) {
                return 15u; // Material 15: Insulated Timber Door (airtight & passable)
            }
            // Architectural Windows at y == 2
            if (pos.y == 2 &&
                (((absX == 9 || absX == 17) && (lotZ == 4 || lotZ == 8)) ||
                 ((lotZ == 2 || lotZ == 10) && (absX == 11 || absX == 15)))) {
                return 7u; // Material 7: Window glass
            }
            // Exterior Perimeter Clapboard Walls
            if (absX == 9 || absX == 17 || lotZ == 2 || lotZ == 10) {
                return 5u; // Material 5: Painted architectural clapboard siding
            }
            // Node 0 Sovereign Homelab Server Rack in Study (absX == 12, lotZ == 6, y <= 2)
            if (absX == 12 && lotZ == 6 && pos.y <= 2) {
                return 5u; // Shaded as Node 0 Homelab Server Rack
            }
            // Hollow walkable interior returns 0u (Air)
        }
    }

    // --- Lot 402 Exterior Roof Access Ladder (x == 18, z == 3, y in [1, 4]) ---
    if (pos.x == 18 && pos.z == 3 && pos.y >= 1 && pos.y <= 4) {
        return 32u; // Material 32: Exterior roof access ladder against back wall
    }

    // --- Layer 1: Property Boundary Fences & Lot 402 Fenced Backyard ---
    if (pos.y == 1) {
        if (pos.x > 0 && pos.z >= 1 && pos.z <= 15) {
            if ((pos.z == 1 || pos.z == 15) && pos.x >= 7 && pos.x <= 25) {
                return 10u; // Lot 402 side fences extending around backyard
            }
            if (pos.x == 25 && pos.z >= 1 && pos.z <= 15 && pos.z != 8) {
                return 10u; // Lot 402 rear backyard fence (with gate opening at z == 8)
            }
        } else if ((lotZ == 15 || lotZ == 1) && absX >= 7 && absX <= 20) {
            return 10u; // Neighbor boundary fences
        }
    }

    return 0u; // Air
}

fn get_sky_color(rd: vec3<f32>) -> vec3<f32> {
    let diurnal_phase = sin(camera.avatar_pos.w * 0.01308997) * 0.5 + 0.5;
    let day_top = vec3<f32>(0.14, 0.36, 0.68);
    let dusk_top = vec3<f32>(0.08, 0.14, 0.32);
    let day_horizon = vec3<f32>(0.62, 0.78, 0.92);
    let dusk_horizon = vec3<f32>(0.76, 0.48, 0.36);
    let sky_top = mix(dusk_top, day_top, diurnal_phase);
    let sky_horizon = mix(dusk_horizon, day_horizon, diurnal_phase);
    let t = clamp(rd.y * 0.5 + 0.5, 0.0, 1.0);
    let sun_dir = normalize(vec3<f32>(0.5, 0.8, -0.4));
    let sun_glow = pow(max(dot(rd, sun_dir), 0.0), 24.0) * 0.35 * diurnal_phase;
    return mix(sky_horizon, sky_top, t) + vec3<f32>(1.0, 0.92, 0.75) * sun_glow;
}

fn aces_tonemap(x: vec3<f32>) -> vec3<f32> {
    let a = 2.51;
    let b = 0.03;
    let c = 2.43;
    let d = 0.59;
    let e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), vec3<f32>(0.0), vec3<f32>(1.0));
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

fn intersect_capsule(ro: vec3<f32>, rd: vec3<f32>, pa: vec3<f32>, pb: vec3<f32>, r: f32) -> f32 {
    let ba = pb - pa;
    let oa = ro - pa;
    let baba = dot(ba, ba);
    let bard = dot(ba, rd);
    let baoa = dot(ba, oa);
    let rdoa = dot(rd, oa);
    let oaoa = dot(oa, oa);
    let a = baba - bard * bard;
    let b = baba * rdoa - baoa * bard;
    let c = baba * oaoa - baoa * baoa - r * r * baba;
    let h = b * b - a * c;
    if (h >= 0.0 && abs(a) > 1e-6) {
        let t = (-b - sqrt(h)) / a;
        let y = baoa + t * bard;
        if (y > 0.0 && y < baba && t > 0.0) {
            return t;
        }
        let oc = select(ro - pb, oa, y <= 0.0);
        let b2 = dot(rd, oc);
        let c2 = dot(oc, oc) - r * r;
        let h2 = b2 * b2 - c2;
        if (h2 > 0.0) {
            let t_cap = -b2 - sqrt(h2);
            if (t_cap > 0.0) {
                return t_cap;
            }
        }
    }
    return -1.0;
}

fn capsule_normal(pos: vec3<f32>, pa: vec3<f32>, pb: vec3<f32>) -> vec3<f32> {
    let ba = pb - pa;
    let pa_pos = pos - pa;
    let h = clamp(dot(pa_pos, ba) / max(dot(ba, ba), 1e-6), 0.0, 1.0);
    return normalize(pa_pos - ba * h);
}

// Sub-voxel vertical slab height (1.0 = full 1m block; < 0.99 = low ground feature)
fn get_voxel_max_local_y(mat: u32) -> f32 {
    if (mat == 18u) { return 0.05; } // Sunken Swale Trench rim
    if (mat == 20u || mat == 21u || mat == 22u || mat == 25u) { return 0.06; } // Ground zone / stockpile / threshold
    if (mat == 26u) { return 0.14; } // Blueprint ghost stake
    if (mat == 19u) { return 0.18; } // Tilled Garden Bed / Cover Crop
    if (mat == 34u) { return 0.38; } // Mature Harvestable Crop Bush
    if (mat == 33u) { return 0.55; } // Stage 1 Assembly Frame
    return 1.0;
}

// 16-Step Directional Sun Shadow Ray (respects roof cutaway & translucent window glass)
fn trace_sun_shadow(origin: vec3<f32>, sun_dir: vec3<f32>, z_cutaway: i32) -> f32 {
    var sPos = vec3<i32>(floor(origin));
    var sStep = vec3<i32>(1, 1, 1);
    if (sun_dir.x < 0.0) { sStep.x = -1; }
    if (sun_dir.y < 0.0) { sStep.y = -1; }
    if (sun_dir.z < 0.0) { sStep.z = -1; }

    let sDelta = abs(vec3<f32>(1.0) / max(abs(sun_dir), vec3<f32>(1e-6)));
    var sSide = vec3<f32>(0.0);
    if (sStep.x > 0) { sSide.x = (f32(sPos.x + 1) - origin.x) * sDelta.x; }
    else { sSide.x = (origin.x - f32(sPos.x)) * sDelta.x; }
    if (sStep.y > 0) { sSide.y = (f32(sPos.y + 1) - origin.y) * sDelta.y; }
    else { sSide.y = (origin.y - f32(sPos.y)) * sDelta.y; }
    if (sStep.z > 0) { sSide.z = (f32(sPos.z + 1) - origin.z) * sDelta.z; }
    else { sSide.z = (origin.z - f32(sPos.z)) * sDelta.z; }

    var shadow_atten: f32 = 1.0;
    for (var s = 0; s < 16; s++) {
        if (sSide.x < sSide.y) {
            if (sSide.x < sSide.z) { sSide.x += sDelta.x; sPos.x += sStep.x; }
            else { sSide.z += sDelta.z; sPos.z += sStep.z; }
        } else {
            if (sSide.y < sSide.z) { sSide.y += sDelta.y; sPos.y += sStep.y; }
            else { sSide.z += sDelta.z; sPos.z += sStep.z; }
        }
        if (sPos.y >= z_cutaway || sPos.y > 16 || sPos.y < 0) {
            break;
        }
        let sm = get_voxel_material(sPos);
        if (sm != 0u && sm < 26u && get_voxel_max_local_y(sm) >= 0.99) {
            if (sm == 7u) {
                shadow_atten = min(shadow_atten, 0.72); // Sunbeams through window glass
            } else {
                return 0.34; // Soft directional sun shadow
            }
        }
    }
    return shadow_atten;
}

@fragment
fn fs_main(in : VertexOutput) -> @location(0) vec4<f32> {
    let aspect = camera.viewport_params.z;
    let ndc = in.uv * 2.0 - vec2<f32>(1.0, 1.0);

    // Unpack F1-F4 Info-View overlay_mode from integer tens digit of camera.camera_pos.w
    let cam_w_pos = max(camera.camera_pos.w, 0.0);
    let overlay_mode: u32 = u32(floor(cam_w_pos / 10.0));
    let poi_reticle_val: f32 = f32(u32(floor(cam_w_pos + 0.5)) % 10u);

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

    var dda_step = vec3<i32>(1, 1, 1);
    if (rd.x < 0.0) { dda_step.x = -1; }
    if (rd.y < 0.0) { dda_step.y = -1; }
    if (rd.z < 0.0) { dda_step.z = -1; }

    // Delta distance per axis
    let deltaDist = abs(vec3<f32>(1.0) / max(abs(rd), vec3<f32>(1e-6)));

    // Side distance to first boundary
    var sideDist = vec3<f32>(0.0);
    if (dda_step.x > 0) {
        sideDist.x = (f32(mapPos.x + 1) - ro.x) * deltaDist.x;
    } else {
        sideDist.x = (ro.x - f32(mapPos.x)) * deltaDist.x;
    }
    if (dda_step.y > 0) {
        sideDist.y = (f32(mapPos.y + 1) - ro.y) * deltaDist.y;
    } else {
        sideDist.y = (ro.y - f32(mapPos.y)) * deltaDist.y;
    }
    if (dda_step.z > 0) {
        sideDist.z = (f32(mapPos.z + 1) - ro.z) * deltaDist.z;
    } else {
        sideDist.z = (ro.z - f32(mapPos.z)) * deltaDist.z;
    }

    var hit_dist: f32 = 0.0;
    var hit_normal = vec3<f32>(0.0);
    var hit_mat: u32 = 0u;
    var npc_subpart: u32 = 0u; // 0u = voxel, 1u = NPC body capsule, 2u = NPC head sphere

    // Roof / Wall Cutaway level encoded in camera.camera_target.w when in (1.5, 31.5)
    let cutaway_active = camera.camera_target.w > 1.5 && camera.camera_target.w < 31.5;
    let z_cutaway_level = select(32, i32(floor(camera.camera_target.w + 0.5)), cutaway_active);

    const MAX_STEPS: i32 = 192;
    for (var i = 0; i < MAX_STEPS; i++) {
        // Step along axis with minimum distance to next voxel boundary
        if (sideDist.x < sideDist.y) {
            if (sideDist.x < sideDist.z) {
                hit_dist = sideDist.x;
                sideDist.x += deltaDist.x;
                mapPos.x += dda_step.x;
                hit_normal = vec3<f32>(-f32(dda_step.x), 0.0, 0.0);
            } else {
                hit_dist = sideDist.z;
                sideDist.z += deltaDist.z;
                mapPos.z += dda_step.z;
                hit_normal = vec3<f32>(0.0, 0.0, -f32(dda_step.z));
            }
        } else {
            if (sideDist.y < sideDist.z) {
                hit_dist = sideDist.y;
                sideDist.y += deltaDist.y;
                mapPos.y += dda_step.y;
                hit_normal = vec3<f32>(0.0, -f32(dda_step.y), 0.0);
            } else {
                hit_dist = sideDist.z;
                sideDist.z += deltaDist.z;
                mapPos.z += dda_step.z;
                hit_normal = vec3<f32>(0.0, 0.0, -f32(dda_step.z));
            }
        }

        // Bounding space early exit: volume y in [0, 32], z in [-64, 64], |x| <= 64
        if ((mapPos.y > 32 && dda_step.y > 0) || (mapPos.y < 0 && dda_step.y < 0) ||
            (mapPos.z > 64 && dda_step.z > 0) || (mapPos.z < -64 && dda_step.z < 0) ||
            (mapPos.x > 64 && dda_step.x > 0) || (mapPos.x < -64 && dda_step.x < 0)) {
            break;
        }

        var mat: u32 = 0u;
        if (mapPos.y < z_cutaway_level) {
            mat = get_voxel_material(mapPos);
        }
        if (mat >= 27u && mat <= 29u) {
            // Sub-voxel 3D humanoid intersection for sidewalk/enclave NPCs
            let npc_base = vec3<f32>(f32(mapPos.x) + 0.5, f32(mapPos.y), f32(mapPos.z) + 0.5);
            let npc_pa = npc_base + vec3<f32>(0.0, 0.12, 0.0);
            let npc_pb = npc_base + vec3<f32>(0.0, 0.62, 0.0);
            let npc_head_c = npc_base + vec3<f32>(0.0, 0.80, 0.0);
            let t_npc_body = intersect_capsule(ro, rd, npc_pa, npc_pb, 0.20);
            let t_npc_head = intersect_sphere(ro, rd, npc_head_c, 0.15);

            var t_npc: f32 = 1e9;
            var part_npc: u32 = 0u;
            if (t_npc_body > 0.0 && t_npc_body < t_npc) {
                t_npc = t_npc_body;
                part_npc = 1u;
            }
            if (t_npc_head > 0.0 && t_npc_head < t_npc) {
                t_npc = t_npc_head;
                part_npc = 2u;
            }
            if (part_npc != 0u) {
                hit_dist = t_npc;
                hit_mat = mat;
                npc_subpart = part_npc;
                let p_npc = ro + rd * t_npc;
                if (part_npc == 1u) {
                    hit_normal = capsule_normal(p_npc, npc_pa, npc_pb);
                } else {
                    hit_normal = normalize(p_npc - npc_head_c);
                }
                break;
            }
            // Ray missed sub-voxel NPC capsule/head -> continue DDA through cell!
        } else if (mat != 0u) {
            let max_h = get_voxel_max_local_y(mat);
            if (max_h < 0.99) {
                let bmin = vec3<f32>(f32(mapPos.x), f32(mapPos.y), f32(mapPos.z));
                let bmax = vec3<f32>(f32(mapPos.x) + 1.0, f32(mapPos.y) + max_h, f32(mapPos.z) + 1.0);
                let inv_rd = vec3<f32>(1.0) / select(rd, vec3<f32>(1e-6), abs(rd) < vec3<f32>(1e-6));
                let t0s = (bmin - ro) * inv_rd;
                let t1s = (bmax - ro) * inv_rd;
                let tsmaller = min(t0s, t1s);
                let tbigger  = max(t0s, t1s);
                let t_slab_enter = max(tsmaller.x, max(tsmaller.y, tsmaller.z));
                let t_slab_exit  = min(tbigger.x, min(tbigger.y, tbigger.z));
                if (t_slab_enter <= t_slab_exit && t_slab_exit > 0.0) {
                    hit_dist = max(0.0, t_slab_enter);
                    hit_mat = mat;
                    let p_slab = ro + rd * hit_dist;
                    if (p_slab.y >= bmax.y - 0.005) {
                        hit_normal = vec3<f32>(0.0, 1.0, 0.0);
                    }
                    break;
                }
                // Ray passed through empty air above low ground slab -> continue DDA!
            } else {
                hit_mat = mat;
                break;
            }
        }
    }

    let voxel_dist = select(1e9, hit_dist, hit_mat != 0u);

    // Founder Avatar: Embodied 3D Colonist (Body Capsule + Head Sphere + Carried Crate) + Overhead Plumbob Beacon
    if (camera.avatar_params.w > 0.5) {
        let is_carrying = camera.avatar_pos.w >= 10.0;
        let plumbob_state = select(camera.avatar_pos.w, camera.avatar_pos.w - 10.0, is_carrying);
        let anim_time = plumbob_state;
        let body_pa = camera.avatar_pos.xyz + vec3<f32>(0.0, -0.50, 0.0);
        let body_pb = camera.avatar_pos.xyz + vec3<f32>(0.0,  0.35, 0.0);
        let head_center = camera.avatar_pos.xyz + vec3<f32>(0.0, 0.55, 0.0);
        let crate_center = camera.avatar_pos.xyz + vec3<f32>(0.24, 0.05, 0.20);
        let plumbob_offset = 1.15 + sin(anim_time * 3.5) * 0.10;
        let plumbob_center = camera.avatar_pos.xyz + vec3<f32>(0.0, plumbob_offset, 0.0);

        let t_body = intersect_capsule(ro, rd, body_pa, body_pb, 0.26);
        let t_head = intersect_sphere(ro, rd, head_center, 0.20);
        let t_crate = select(-1.0, intersect_sphere(ro, rd, crate_center, 0.19), is_carrying);
        let t_plumbob = intersect_sphere(ro, rd, plumbob_center, 0.22);

        var t_avatar: f32 = 1e9;
        var avatar_part: u32 = 0u; // 1u = colonist body, 2u = colonist head, 3u = overhead plumbob, 4u = carried crate/bundle
        if (t_body > 0.0 && t_body < t_avatar) {
            t_avatar = t_body;
            avatar_part = 1u;
        }
        if (t_head > 0.0 && t_head < t_avatar) {
            t_avatar = t_head;
            avatar_part = 2u;
        }
        if (t_crate > 0.0 && t_crate < t_avatar) {
            t_avatar = t_crate;
            avatar_part = 4u;
        }
        if (t_plumbob > 0.0 && t_plumbob < t_avatar) {
            t_avatar = t_plumbob;
            avatar_part = 3u;
        }

        if (avatar_part != 0u && t_avatar < voxel_dist) {
            let hit_pos = ro + rd * t_avatar;
            var norm = vec3<f32>(0.0, 1.0, 0.0);
            if (avatar_part == 1u) {
                norm = capsule_normal(hit_pos, body_pa, body_pb);
            } else if (avatar_part == 2u) {
                norm = normalize(hit_pos - head_center);
            } else if (avatar_part == 4u) {
                norm = normalize(hit_pos - crate_center);
            } else {
                norm = normalize(hit_pos - plumbob_center);
            }

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
            let param_state = u32(round(camera.avatar_params.x));
            let pb_state_u32 = u32(round(clamp(plumbob_state, 0.0, 4.0)));
            let action_state = select(param_state, pb_state_u32, is_carrying && plumbob_state <= 4.5);
            var plumbob_base = vec3<f32>(0.00, 0.90, 0.45); // Emerald Green
            var plumbob_glow = vec3<f32>(0.35, 1.00, 0.65);

            if (action_state == 1u) {
                plumbob_base = vec3<f32>(0.00, 0.65, 0.95); // Cyan
                plumbob_glow = vec3<f32>(0.40, 0.85, 1.00);
            } else if (action_state == 2u) {
                plumbob_base = vec3<f32>(0.95, 0.75, 0.05); // Amber Gold
                plumbob_glow = vec3<f32>(1.00, 0.90, 0.40);
            } else if (action_state == 4u || camera.avatar_params.y > 0.75) {
                plumbob_base = vec3<f32>(0.95, 0.10, 0.15); // Crimson Warning
                plumbob_glow = vec3<f32>(1.00, 0.40, 0.45);
            }

            if (overlay_mode == 4u) {
                let stress_t = clamp(camera.avatar_params.y, 0.0, 1.0);
                plumbob_base = mix(vec3<f32>(0.00, 0.95, 0.50), vec3<f32>(1.00, 0.10, 0.20), stress_t);
                plumbob_glow = mix(vec3<f32>(0.40, 1.00, 0.75), vec3<f32>(1.00, 0.35, 0.45), stress_t);
            }

            let half_v = normalize(light_dir + view_dir);
            let spec_av = pow(max(dot(norm, half_v), 0.0), 32.0);

            var lit_avatar = vec3<f32>(0.0);
            if (avatar_part == 1u) {
                // 3D Colonist Body: Solarpunk indigo/ochre workwear with utility belt accent
                let rel_y = hit_pos.y - camera.avatar_pos.y;
                let is_belt = abs(rel_y - 0.02) < 0.07;
                let workwear_col = select(vec3<f32>(0.20, 0.54, 0.72), vec3<f32>(0.85, 0.58, 0.18), is_belt);
                lit_avatar = workwear_col * (0.38 + 0.62 * diff) + vec3<f32>(0.9, 0.95, 1.0) * (spec_av * 0.25) + plumbob_glow * (fresnel * 0.25);
            } else if (avatar_part == 2u) {
                // 3D Colonist Head: Warm skin/visor tone
                let head_col = vec3<f32>(0.92, 0.78, 0.62);
                lit_avatar = head_col * (0.42 + 0.58 * diff) + vec3<f32>(0.95, 0.90, 0.80) * (fresnel * 0.22 + spec_av * 0.18);
            } else if (avatar_part == 4u) {
                // Carried Supply Crate / Produce Bundle: Warm salvaged hardware brass/crate ochre with dark strap bands
                let rel_crate = hit_pos - crate_center;
                let strap_band = abs(rel_crate.x) < 0.035 || abs(rel_crate.z) < 0.035 || abs(rel_crate.y) < 0.028;
                let crate_col = select(vec3<f32>(0.85, 0.58, 0.22), vec3<f32>(0.24, 0.18, 0.12), strap_band);
                lit_avatar = crate_col * (0.38 + 0.62 * diff) + vec3<f32>(1.0, 0.92, 0.72) * (spec_av * 0.32);
            } else {
                // Overhead Plumbob Beacon
                let pulse = 0.85 + 0.15 * sin(anim_time * 5.0);
                lit_avatar = plumbob_base * (0.35 + 0.65 * diff) + plumbob_glow * pulse * (fresnel * 0.85 + 0.28 + spec_av * 0.45);
            }

            let sky = get_sky_color(rd);
            let fog = 1.0 - exp(-t_avatar * 0.022);
            let final_avatar = aces_tonemap(mix(lit_avatar, sky, fog) * 1.08);
            return vec4<f32>(final_avatar, 1.0);
        }
    }

    // Sky miss
    if (hit_mat == 0u) {
        return vec4<f32>(get_sky_color(rd), 1.0);
    }

    let hit_pos = ro + rd * hit_dist;
    let hit_frac = fract(hit_pos);
    let lotZ_hit = (mapPos.z % 16 + 16) % 16;
    let absX_hit = abs(mapPos.x);

    // Stylized PBR Diorama Materials (albedo, roughness, metallic, emissive)
    var base_color: vec3<f32> = vec3<f32>(0.5);
    var roughness: f32 = 0.65;
    var metallic: f32 = 0.04;
    var emissive: vec3<f32> = vec3<f32>(0.0);

    if (hit_mat == 1u) {
        // Material 1: Asphalt roadway with subtle aggregate micro-grain
        let agg = sin(hit_pos.x * 18.0) * cos(hit_pos.z * 18.0) * 0.015;
        base_color = vec3<f32>(0.15, 0.16, 0.18) + vec3<f32>(agg);
        roughness = 0.78;
    } else if (hit_mat == 2u) {
        // Material 2: Reflective yellow road centerline
        base_color = vec3<f32>(0.92, 0.73, 0.18);
        roughness = 0.35;
    } else if (hit_mat == 3u) {
        // Material 3: Brushed concrete sidewalk & driveway with expansion joints
        let joint = select(0.0, 1.0, hit_frac.x < 0.035 || hit_frac.z < 0.035);
        base_color = mix(vec3<f32>(0.64, 0.66, 0.68), vec3<f32>(0.48, 0.50, 0.52), joint * 0.65);
        roughness = 0.62;
    } else if (hit_mat == 4u) {
        // Material 4: Context-aware Ground (Interior Oak Floor / Flagstone Patio / Backyard Garden Loam / Front Lawn)
        if (absX_hit >= 9 && absX_hit <= 17 && lotZ_hit >= 2 && lotZ_hit <= 10) {
            // Warm Polished Oak Hardwood Flooring inside House
            let plank_seam = select(0.0, 1.0, fract(hit_pos.x * 3.0) < 0.06 || fract(hit_pos.z * 0.75) < 0.04);
            let grain = sin(hit_pos.z * 14.0 + floor(hit_pos.x * 3.0) * 2.3) * 0.025;
            base_color = mix(vec3<f32>(0.58, 0.40, 0.24) + vec3<f32>(grain), vec3<f32>(0.36, 0.24, 0.14), plank_seam * 0.65);
            roughness = 0.26;
            metallic = 0.06;
        } else if (mapPos.x >= 18 && mapPos.x <= 19 && mapPos.z >= 4 && mapPos.z <= 8) {
            // Backyard Flagstone Patio Pavers behind Back Door
            let paver_seam = select(0.0, 1.0, fract(hit_pos.x * 2.0) < 0.06 || fract(hit_pos.z * 2.0) < 0.06);
            base_color = mix(vec3<f32>(0.62, 0.60, 0.56), vec3<f32>(0.42, 0.40, 0.38), paver_seam * 0.65);
            roughness = 0.50;
        } else if (mapPos.x >= 18 && mapPos.x <= 25 && mapPos.z >= 1 && mapPos.z <= 15) {
            // Lot 402 Fenced Backyard Cultivable Turf & Rich Garden Loam
            let turf_var = sin(hit_pos.x * 5.0) * cos(hit_pos.z * 5.0) * 0.03;
            base_color = vec3<f32>(0.30, 0.52, 0.25) + vec3<f32>(turf_var);
            roughness = 0.70;
        } else {
            // Suburban Front Lawn & Neighbor Yards
            let lawn_var = sin(hit_pos.x * 4.0) * cos(hit_pos.z * 4.0) * 0.025;
            base_color = vec3<f32>(0.43, 0.49, 0.32) + vec3<f32>(lawn_var);
            roughness = 0.76;
        }
    } else if (hit_mat == 5u || hit_mat == 30u) {
        if ((absX_hit == 12 && lotZ_hit == 6 && mapPos.y <= 2) || hit_mat == 30u) {
            // Node 0 Sovereign Homelab Server Rack inside Study
            let blade_slot = fract(hit_pos.y * 6.0);
            let is_led = blade_slot > 0.35 && blade_slot < 0.58 && (hit_frac.x > 0.18 && hit_frac.x < 0.82);
            let led_blink = 0.55 + 0.45 * sin(camera.avatar_pos.w * 6.0 + floor(hit_pos.y * 6.0) * 1.9);
            let led_col = mix(vec3<f32>(0.0, 0.95, 0.65), vec3<f32>(0.10, 0.82, 1.00), fract(floor(hit_pos.y * 6.0) * 0.37));
            base_color = vec3<f32>(0.10, 0.13, 0.17);
            roughness = 0.18;
            metallic = 0.85;
            if (is_led) {
                emissive = led_col * led_blink * 0.95;
            }
        } else {
            // Crisp Architectural Painted Clapboard Siding with Corner Trim
            let clapboard_rib = fract(hit_pos.y * 4.0);
            let rib_shadow = smoothstep(0.0, 0.18, clapboard_rib);
            let is_corner_trim = (absX_hit == 9 || absX_hit == 17) && (lotZ_hit == 2 || lotZ_hit == 10);
            let siding_base = select(vec3<f32>(0.76, 0.79, 0.82) * (0.86 + 0.14 * rib_shadow), vec3<f32>(0.90, 0.91, 0.92), is_corner_trim);
            base_color = siding_base;
            roughness = 0.46;
        }
    } else if (hit_mat == 6u) {
        // Material 6: Architectural Slate/Terracotta Roof Shingles with Course Overlap
        let course_z = fract(hit_pos.z * 3.0);
        let course_x = fract(hit_pos.x * 2.0 + floor(hit_pos.z * 3.0) * 0.5);
        let shingle_line = select(1.0, 0.82, course_z < 0.10 || course_x < 0.06);
        base_color = vec3<f32>(0.23, 0.26, 0.34) * shingle_line;
        roughness = 0.52;
        metallic = 0.10;
    } else if (hit_mat == 7u) {
        // Material 7: Glossy Architectural Window Glass with White Mullions & Warm Study Glow
        let mullion = hit_frac.x < 0.08 || hit_frac.x > 0.92 || hit_frac.y < 0.08 || hit_frac.y > 0.92 || abs(hit_frac.x - 0.5) < 0.035;
        base_color = select(vec3<f32>(0.20, 0.38, 0.54), vec3<f32>(0.92, 0.93, 0.94), mullion);
        roughness = select(0.06, 0.40, mullion);
        metallic = select(0.88, 0.05, mullion);
        if (!mullion) {
            emissive = vec3<f32>(0.20, 0.15, 0.07);
        }
    } else if (hit_mat == 8u) {
        // Material 8: Utility timber poles
        base_color = vec3<f32>(0.30, 0.19, 0.15);
        roughness = 0.72;
    } else if (hit_mat == 9u) {
        // Material 9: Transformers / grid hardware
        base_color = vec3<f32>(0.68, 0.70, 0.72);
        roughness = 0.30;
        metallic = 0.80;
    } else if (hit_mat == 10u) {
        // Material 10: Western Red Cedar Privacy Fence with Vertical Slat Seams
        let slat = select(1.0, 0.80, fract((hit_pos.x + hit_pos.z) * 4.0) < 0.09);
        base_color = vec3<f32>(0.54, 0.36, 0.22) * slat;
        roughness = 0.60;
    } else if (hit_mat == 11u) {
        // Material 11: Monocrystalline Solar Panel (photovoltaic blue with silver busbar grid)
        let pv_grid = select(0.0, 1.0, fract(hit_pos.x * 4.0) < 0.07 || fract(hit_pos.z * 4.0) < 0.07);
        let pv_frame = select(0.0, 1.0, hit_frac.x < 0.05 || hit_frac.x > 0.95 || hit_frac.z < 0.05 || hit_frac.z > 0.95);
        base_color = mix(vec3<f32>(0.08, 0.22, 0.62), vec3<f32>(0.55, 0.78, 0.96), pv_grid * 0.45);
        base_color = mix(base_color, vec3<f32>(0.82, 0.86, 0.90), pv_frame * 0.80);
        roughness = 0.12;
        metallic = 0.88;
    } else if (hit_mat == 12u) {
        // Material 12: Rainwater Cistern / Water Tote (glossy aqua tank with level ribs)
        let rib = select(1.0, 0.88, fract(hit_pos.y * 4.0) < 0.12);
        base_color = vec3<f32>(0.10, 0.72, 0.88) * rib;
        roughness = 0.16;
        metallic = 0.30;
    } else if (hit_mat == 13u) {
        // Material 13: Cob Wall (warm earthen cob)
        base_color = vec3<f32>(0.65, 0.50, 0.37);
        roughness = 0.72;
    } else if (hit_mat == 14u) {
        // Material 14: Urbanite Wall (reclaimed concrete slab)
        base_color = vec3<f32>(0.46, 0.50, 0.53);
        roughness = 0.64;
    } else if (hit_mat == 15u) {
        // Material 15: Insulated Timber Door (warm cedar door with architectural frame & handle accent)
        let door_frame = select(0.0, 1.0, hit_frac.x < 0.10 || hit_frac.x > 0.90 || hit_frac.z < 0.10 || hit_frac.z > 0.90);
        base_color = mix(vec3<f32>(0.58, 0.35, 0.16), vec3<f32>(0.36, 0.20, 0.08), door_frame * 0.50);
        roughness = 0.42;
    } else if (hit_mat == 16u) {
        // Material 16: Rocket Mass Heater / Hearth (glowing terracotta hearth with warm ember pulse)
        let ember_pulse = 0.88 + 0.12 * sin(camera.avatar_pos.w * 4.5);
        base_color = vec3<f32>(0.86, 0.28, 0.10) * ember_pulse;
        roughness = 0.45;
        emissive = vec3<f32>(0.45, 0.12, 0.02) * ember_pulse;
    } else if (hit_mat == 17u) {
        // Material 17: Insulated Timber Roof (dark timber roof beams)
        base_color = vec3<f32>(0.32, 0.25, 0.21);
        roughness = 0.55;
    } else if (hit_mat == 18u) {
        // Material 18: Sunken Swale Trench (recessed earthen berm rim + shimmering rainwater pool)
        let in_trench = abs(hit_frac.x - 0.5) < 0.36 && abs(hit_frac.z - 0.5) < 0.36;
        let in_water = abs(hit_frac.x - 0.5) < 0.24 && abs(hit_frac.z - 0.5) < 0.24;
        let swale_shimmer = 0.90 + 0.10 * sin(camera.avatar_pos.w * 3.5 + hit_pos.x * 4.0 + hit_pos.z * 4.0);
        if (in_water) {
            base_color = vec3<f32>(0.08, 0.38, 0.52) * swale_shimmer;
            roughness = 0.12;
            metallic = 0.45;
            emissive = vec3<f32>(0.02, 0.09, 0.14) * swale_shimmer;
        } else if (in_trench) {
            // Dark recessed earthen trench slope illusion
            base_color = vec3<f32>(0.14, 0.11, 0.08);
            roughness = 0.72;
        } else {
            // Contoured low earth berm rim
            base_color = vec3<f32>(0.28, 0.22, 0.15);
            roughness = 0.68;
        }
    } else if (hit_mat == 19u) {
        // Material 19: Low-Profile Cover Crop / Tilled Garden Rows (0.18m raised bed with dark soil furrows)
        let furrow = sin(hit_pos.z * 18.85);
        let is_foliage = furrow > -0.15 && abs(hit_frac.x - 0.5) < 0.42;
        base_color = select(vec3<f32>(0.22, 0.16, 0.11), vec3<f32>(0.28, 0.58, 0.18) * (0.90 + 0.10 * furrow), is_foliage);
        roughness = select(0.75, 0.55, is_foliage);
    } else if (hit_mat == 20u) {
        // Material 20: Haul Stockpile Zone (cyan cadastral grid border)
        let haul_border = select(0.0, 1.0, hit_frac.x < 0.10 || hit_frac.x > 0.90 || hit_frac.z < 0.10 || hit_frac.z > 0.90);
        base_color = mix(vec3<f32>(0.05, 0.52, 0.62), vec3<f32>(0.00, 0.72, 0.82), 0.45 + 0.55 * haul_border);
        roughness = 0.40;
    } else if (hit_mat == 21u) {
        // Material 21: Sleep Zone (indigo thermal bedding)
        base_color = vec3<f32>(0.36, 0.44, 0.78);
        roughness = 0.55;
    } else if (hit_mat == 22u) {
        // Material 22: Recreation Zone / Patio Hearth (warm amber hearth/campfire glow)
        let camp_pulse = 0.88 + 0.12 * sin(camera.avatar_pos.w * 5.5);
        base_color = vec3<f32>(1.00, 0.58, 0.05) * camp_pulse;
        roughness = 0.40;
        emissive = vec3<f32>(0.42, 0.20, 0.0) * camp_pulse;
    } else if (hit_mat == 23u) {
        // Material 23: Master Bed / Shelter inside House (quilted indigo & warm linen bedding)
        let pillow = hit_frac.x < 0.28;
        base_color = select(vec3<f32>(0.28, 0.42, 0.68), vec3<f32>(0.90, 0.86, 0.78), pillow);
        roughness = 0.52;
    } else if (hit_mat == 24u) {
        // Material 24: Kitchen Pantry / Ration Crate (warm timber cabinet with brass trim)
        let crate_trim = select(1.0, 0.78, hit_frac.x < 0.08 || hit_frac.x > 0.92 || hit_frac.z < 0.08 || hit_frac.z > 0.92);
        base_color = vec3<f32>(0.60, 0.41, 0.22) * crate_trim;
        roughness = 0.48;
    } else if (hit_mat == 25u) {
        // Material 25: Thermal Leak Marker (pulsing icy-blue breach beacon)
        let leak_beacon = 0.78 + 0.22 * sin(camera.avatar_pos.w * 7.0);
        base_color = vec3<f32>(0.20, 0.85, 1.00) * leak_beacon;
        emissive = vec3<f32>(0.10, 0.45, 0.60) * leak_beacon;
    } else if (hit_mat == 26u) {
        // Material 26: Queued Blueprint Ghost (high-visibility pulsing cyan/emerald wireframe construction blueprint)
        let bp_edge_x = min(hit_frac.x, 1.0 - hit_frac.x);
        let bp_edge_y = min(hit_frac.y, 1.0 - hit_frac.y);
        let bp_edge_z = min(hit_frac.z, 1.0 - hit_frac.z);
        let bp_wire = min(max(bp_edge_x, bp_edge_y), min(max(bp_edge_y, bp_edge_z), max(bp_edge_x, bp_edge_z)));
        let bp_pulse = 0.70 + 0.30 * sin(camera.avatar_pos.w * 6.0);
        base_color = select(vec3<f32>(0.15, 0.90, 0.95) * (0.55 * bp_pulse), vec3<f32>(0.35, 1.00, 0.90), bp_wire < 0.11);
        emissive = base_color * 0.65;
    } else if (hit_mat == 27u) {
        // Material 27: MAT_MIGRANT_R6 (Sub-voxel 3D Passerby / Layer 7 Neighbor — warm slate/amber jacket + head)
        base_color = select(vec3<f32>(0.76, 0.46, 0.20), vec3<f32>(0.90, 0.75, 0.60), npc_subpart == 2u);
        roughness = 0.48;
    } else if (hit_mat == 28u) {
        // Material 28: MAT_RESIDENT_R3 (Sub-voxel 3D Resident — teal/cyan cooperative jacket + head)
        base_color = select(vec3<f32>(0.14, 0.62, 0.78), vec3<f32>(0.92, 0.78, 0.62), npc_subpart == 2u);
        roughness = 0.45;
    } else if (hit_mat == 29u) {
        // Material 29: MAT_STEWARD_R1 (Sub-voxel 3D Steward — emerald/gold tunic + head)
        base_color = select(vec3<f32>(0.14, 0.72, 0.45), vec3<f32>(0.94, 0.80, 0.64), npc_subpart == 2u);
        roughness = 0.42;
    } else if (hit_mat == 32u) {
        // Material 32: MAT_LADDER (Brushed galvanized steel / cedar roof access ladder with side rails and rungs)
        let rung = step(0.72, fract(hit_pos.y * 4.0));
        let rail = step(0.30, abs(hit_frac.z - 0.5));
        let is_frame = max(rung, rail);
        base_color = mix(vec3<f32>(0.18, 0.20, 0.22), vec3<f32>(0.76, 0.78, 0.82), is_frame);
        roughness = mix(0.68, 0.24, is_frame);
        metallic = mix(0.15, 0.82, is_frame);
    } else if (hit_mat == 33u) {
        // Material 33: MAT_STAGE_FRAME (Active multi-stage construction scaffolding & hardware mounting frame)
        let tr_x = min(hit_frac.x, 1.0 - hit_frac.x);
        let tr_y = min(hit_frac.y, 1.0 - hit_frac.y);
        let tr_z = min(hit_frac.z, 1.0 - hit_frac.z);
        let truss_edge = min(max(tr_x, tr_y), min(max(tr_y, tr_z), max(tr_x, tr_z))) < 0.14;
        let stage_pulse = 0.5 + 0.5 * sin(camera.avatar_pos.w * 6.5);
        let status_glow = mix(vec3<f32>(0.98, 0.72, 0.12), vec3<f32>(0.15, 0.95, 0.58), stage_pulse);
        base_color = select(vec3<f32>(0.42, 0.34, 0.22), vec3<f32>(0.78, 0.58, 0.26), truss_edge);
        roughness = 0.36;
        metallic = 0.45;
        emissive = status_glow * select(0.18, 0.55, truss_edge);
    } else if (hit_mat == 34u) {
        // Material 34: MAT_MATURE_CROP (Ripe, harvest-ready backyard garden bed with golden/warm-orange crop heads)
        let row_pat = 0.88 + 0.12 * sin(hit_pos.z * 12.0);
        let crop_clusters = step(0.35, sin(hit_pos.x * 16.0) * cos(hit_pos.z * 16.0));
        let is_top_crown = hit_normal.y > 0.5 || hit_frac.y > 0.72;
        let foliage_col = vec3<f32>(0.36, 0.58, 0.16) * row_pat;
        let ripe_head_col = vec3<f32>(0.95, 0.72, 0.15);
        base_color = select(foliage_col, mix(foliage_col, ripe_head_col, 0.65 + 0.35 * crop_clusters), is_top_crown);
        roughness = 0.48;
        emissive = select(vec3<f32>(0.0), vec3<f32>(0.18, 0.12, 0.02), is_top_crown);
    } else {
        base_color = vec3<f32>(0.50, 0.50, 0.50);
    }

    // Stylized PBR Lighting: Directional Sun + 16-Step Shadow + Hemisphere Bounce + GGX/Blinn-Phong Specular
    let light_dir = normalize(vec3<f32>(0.5, 0.8, -0.4));
    let view_dir = -rd;
    let half_vec = normalize(light_dir + view_dir);

    // Subtle voxel-edge micro-bevel & crease AO on architectural blocks
    var edge_ao: f32 = 1.0;
    if (hit_mat < 26u && mapPos.y >= 1) {
        let ex = min(hit_frac.x, 1.0 - hit_frac.x);
        let ey = min(hit_frac.y, 1.0 - hit_frac.y);
        let ez = min(hit_frac.z, 1.0 - hit_frac.z);
        let min_e = min(max(ex, ey), min(max(ey, ez), max(ex, ez)));
        edge_ao = smoothstep(0.0, 0.06, min_e) * 0.14 + 0.86;
        // Ground-wall contact ambient occlusion at base of y == 1 walls
        if (mapPos.y == 1 && abs(hit_normal.y) < 0.5 && hit_frac.y < 0.15) {
            edge_ao = edge_ao * mix(0.68, 1.0, hit_frac.y / 0.15);
        }
    }

    let sun_shadow = select(trace_sun_shadow(hit_pos + hit_normal * 0.03, light_dir, z_cutaway_level), 1.0, hit_mat >= 26u);
    let n_dot_l = max(dot(hit_normal, light_dir), 0.0);
    let hemi_t = hit_normal.y * 0.5 + 0.5;
    let hemi_ambient = mix(vec3<f32>(0.24, 0.22, 0.20), vec3<f32>(0.42, 0.48, 0.56), hemi_t);

    let shininess = mix(96.0, 8.0, roughness);
    let spec_pow = pow(max(dot(hit_normal, half_vec), 0.0), shininess);
    let spec_strength = mix(0.04, 0.75, 1.0 - roughness) * (0.35 + 0.65 * metallic);
    let spec_light = vec3<f32>(1.0, 0.96, 0.88) * spec_pow * spec_strength * sun_shadow;

    // Fresnel sky reflection on glossy architectural glass, solar panels, and water
    let n_dot_v = max(dot(hit_normal, view_dir), 0.0);
    let fresnel_refl = pow(1.0 - n_dot_v, 4.0) * (1.0 - roughness);
    let refl_sky = get_sky_color(reflect(rd, hit_normal)) * fresnel_refl * (0.25 + 0.55 * metallic);

    var lit_color = (base_color * (hemi_ambient + vec3<f32>(0.92, 0.89, 0.82) * n_dot_l * sun_shadow) + spec_light + refl_sky) * edge_ao + emissive;

    // F1-F4 Info-View Shader Overlays (STORY-9.1)
    if (overlay_mode == 1u) {
        // F1 Thermal & Enclosure Overlay (16u = Hearth, 17u = Timber Roof, 21u = Sleep Zone, 22u = Recreation Zone)
        let leak_pulse = 0.65 + 0.35 * sin(camera.avatar_pos.w * 8.0);
        if (hit_mat == 25u || hit_mat == 7u) {
            lit_color = mix(lit_color, vec3<f32>(0.20, 0.85, 1.00), leak_pulse);
        } else if (hit_mat == 16u || hit_mat == 22u) {
            lit_color = mix(lit_color, vec3<f32>(1.00, 0.50, 0.10), 0.75);
        } else if (hit_mat == 23u || hit_mat == 21u || hit_mat == 5u || hit_mat == 6u ||
                   hit_mat == 13u || hit_mat == 14u || hit_mat == 15u || hit_mat == 17u) {
            lit_color = mix(lit_color, vec3<f32>(0.15, 0.92, 0.55), 0.65);
        } else {
            lit_color = lit_color * 0.45;
        }
    } else if (overlay_mode == 2u) {
        // F2 Exergy & Water Overlay
        if (hit_mat == 11u || hit_mat == 8u || hit_mat == 9u || (hit_mat == 5u && absX_hit == 12 && lotZ_hit == 6)) {
            lit_color = mix(lit_color, vec3<f32>(1.00, 0.85, 0.15), 0.70);
        } else if (hit_mat == 12u || hit_mat == 18u || hit_mat == 19u || hit_mat == 34u) {
            lit_color = mix(lit_color, vec3<f32>(0.00, 0.92, 1.00), 0.70);
        } else {
            lit_color = lit_color * 0.45;
        }
    } else if (overlay_mode == 3u) {
        // F3 Stockpile & Haul Overlay (20u = Haul Stockpile Zone, 24u = Ration Crate, 33u = Stage Frame, 34u = Mature Crop)
        if (hit_mat == 20u || hit_mat == 24u || hit_mat == 33u || hit_mat == 34u) {
            lit_color = mix(lit_color, vec3<f32>(0.00, 0.98, 0.75), 0.75);
        } else {
            lit_color = lit_color * 0.45;
        }
    } else if (overlay_mode == 4u) {
        // F4 Stress & Trust Ring Overlay (27u = R6 Crimson/Amber, 28u = R3 Cyan, 29u = R1 Emerald/Gold)
        let stress_lvl = clamp(camera.avatar_params.y, 0.0, 1.0);
        let stress_col = mix(vec3<f32>(0.10, 0.95, 0.55), vec3<f32>(1.00, 0.15, 0.25), stress_lvl);
        if (hit_mat == 27u) {
            lit_color = mix(lit_color, vec3<f32>(1.0, 0.30, 0.15), 0.85);
        } else if (hit_mat == 28u) {
            lit_color = mix(lit_color, vec3<f32>(0.10, 0.85, 1.0), 0.85);
        } else if (hit_mat == 29u) {
            lit_color = mix(lit_color, vec3<f32>(0.20, 0.95, 0.45), 0.85);
        } else if (hit_mat == 21u || hit_mat == 22u || hit_mat == 20u || hit_mat == 23u) {
            lit_color = mix(lit_color, stress_col, 0.70);
        }
        let hit_world = ro + rd * hit_dist;
        let ring_d = abs(length(hit_world.xz - camera.avatar_pos.xz) - 1.5);
        if (ring_d < 0.18 && mapPos.y == 0) {
            lit_color = mix(lit_color, stress_col, 0.85);
        }
    }

    // Roof / Wall Cutaway top-face architectural cross-section cap
    if (cutaway_active && mapPos.y == i32(floor(camera.camera_target.w + 0.5)) - 1 && hit_normal.y > 0.5) {
        lit_color = mix(lit_color, vec3<f32>(0.88, 0.72, 0.45), 0.55);
    }

    // Contact shadow cast by Founder avatar onto terrain/floor
    if (camera.avatar_params.w > 0.5) {
        let voxel_pos_hit = ro + rd * hit_dist;
        let ground_dist = length(voxel_pos_hit.xz - camera.avatar_pos.xz);
        if (ground_dist < 0.65 && voxel_pos_hit.y <= camera.avatar_pos.y + 0.1) {
            let shadow_factor = smoothstep(0.65, 0.15, ground_dist) * 0.45;
            lit_color = lit_color * (1.0 - shadow_factor);
        }
    }

    // 3D Wireframe POI Reticle when camera.camera_pos.w >= 2.0 (or ones digit >= 2.0)
    if (poi_reticle_val >= 2.0 || (overlay_mode == 0u && camera.camera_pos.w >= 2.0)) {
        let poi_voxel = vec3<i32>(floor(camera.camera_target.xyz + 0.5));
        if (all(mapPos == poi_voxel) || (mapPos.x == poi_voxel.x && mapPos.z == poi_voxel.z && mapPos.y == 0)) {
            let hit_p = ro + rd * hit_dist;
            let frac_p = fract(hit_p);
            let edge_dist = min(min(frac_p.x, 1.0 - frac_p.x), min(frac_p.z, 1.0 - frac_p.z));
            let poi_pulse = 0.5 + 0.5 * sin(camera.avatar_pos.w * 6.0);
            let reticle_col = mix(vec3<f32>(1.0, 0.84, 0.0), vec3<f32>(0.0, 0.95, 1.0), poi_pulse);
            let wire_factor = select(0.35, 0.90, edge_dist < 0.12);
            lit_color = mix(lit_color, reticle_col, wire_factor);
        }
    }

    // Holographic cursor selection & blueprint drag preview highlight when active
    if (camera.cursor_highlight.w > 0.5) {
        let cursor_voxel = vec3<i32>(floor(camera.cursor_highlight.xyz + 0.5));
        if (all(mapPos == cursor_voxel)) {
            if (camera.cursor_highlight.w < 1.5) {
                lit_color = mix(lit_color, vec3<f32>(0.2, 0.9, 1.0), 0.55);
            } else if (camera.cursor_highlight.w < 2.5) {
                let bp_pulse = 0.55 + 0.20 * sin(camera.avatar_pos.w * 6.0);
                lit_color = mix(lit_color, vec3<f32>(0.0, 0.95, 0.85), bp_pulse);
            } else {
                let err_pulse = 0.60 + 0.25 * sin(camera.avatar_pos.w * 8.0);
                lit_color = mix(lit_color, vec3<f32>(1.0, 0.15, 0.25), err_pulse);
            }
        }
    }

    // Distance fog blend into horizon & ACES filmic tone mapping
    let sky = get_sky_color(rd);
    let fog = 1.0 - exp(-hit_dist * 0.022);
    let toned_color = aces_tonemap(lit_color * 1.08);
    let final_color = mix(toned_color, sky, fog);

    return vec4<f32>(final_color, 1.0);
}
