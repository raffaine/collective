#pragma once

namespace oasis {
namespace shaders {

constexpr const char* RAYMARCH_WGSL = R"WGSL(struct CameraUniforms {
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

struct WorldChunkBuffer {
    is_loaded : u32,      // 1u when populated by IMapProvider (Static JSON or Procedural Generator)
    origin_x  : i32,      // e.g. -16
    origin_y  : i32,      // e.g. -2 (includes subsurface strata y = -2..-1!)
    origin_z  : i32,      // e.g. -8
    size_x    : u32,      // 64
    size_y    : u32,      // 16 (covers y in [-2..13])
    size_z    : u32,      // 32 (covers z in [-8..23])
    flags     : u32,      // bit 0: enable distant neighbor fog silhouettes outside loaded chunk
    cells     : array<u32, 32768>, // 64 * 16 * 32 = 32,768 O(1) voxel material IDs
};

@group(0) @binding(2)
var<storage, read> world_chunk : WorldChunkBuffer;

fn is_inside_loaded_chunk(pos: vec3<i32>) -> bool {
    if (world_chunk.is_loaded == 0u) {
        return false;
    }
    let lx = pos.x - world_chunk.origin_x;
    let ly = pos.y - world_chunk.origin_y;
    let lz = pos.z - world_chunk.origin_z;
    return lx >= 0 && lx < i32(world_chunk.size_x) &&
           ly >= 0 && ly < i32(world_chunk.size_y) &&
           lz >= 0 && lz < i32(world_chunk.size_z);
}

fn is_inside_loaded_chunk_xz(pos: vec3<i32>) -> bool {
    if (world_chunk.is_loaded == 0u) {
        return false;
    }
    let lx = pos.x - world_chunk.origin_x;
    let lz = pos.z - world_chunk.origin_z;
    return lx >= 0 && lx < i32(world_chunk.size_x) &&
           lz >= 0 && lz < i32(world_chunk.size_z);
}

fn get_loaded_chunk_material(pos: vec3<i32>) -> u32 {
    let lx = u32(pos.x - world_chunk.origin_x);
    let ly = u32(pos.y - world_chunk.origin_y);
    let lz = u32(pos.z - world_chunk.origin_z);
    let idx = (ly * world_chunk.size_z + lz) * world_chunk.size_x + lx;
    if (idx >= 32768u) {
        return 0u;
    }
    return world_chunk.cells[idx] & 0xFFu;
}

fn is_playable_chunk_cell(pos: vec3<i32>) -> bool {
    if (world_chunk.is_loaded != 0u) {
        return is_inside_loaded_chunk_xz(pos);
    }
    return pos.x >= 4 && pos.x <= 26 && pos.z >= 0 && pos.z <= 15;
}

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

// Generic Sealed Exterior Neighbor Silhouettes for Distant Horizon Fog (outside loaded WorldChunkBuffer)
fn get_distant_neighbor_silhouette_material(pos: vec3<i32>) -> u32 {
    let absX = abs(pos.x);
    if (absX > 64) {
        return 0u;
    }
    let lotZ = (pos.z % 16 + 16) % 16;

    // Subsurface Horizon Continuity outside loaded chunk
    if (pos.y == -2) {
        return 64u; // STRATA_CLAY_SUBSOIL
    }
    if (pos.y == -1) {
        if (absX <= 5) {
            return 65u; // STRATA_COMPACTED_GRAVEL_PAD
        }
        return 67u; // STRATA_TOPSOIL_LOAM
    }

    // Layer 0: Street, Sidewalks, Driveways & Distant Neighbor Lawn
    if (pos.y == 0) {
        if (absX <= 3) {
            if (pos.x == 0 && (pos.z % 4 >= 2 || pos.z % 4 <= -2)) {
                return 2u; // Yellow centerline
            }
            return 1u; // Asphalt roadway
        }
        if (absX == 4 || absX == 5) {
            return 3u; // Concrete sidewalk/curbs
        }
        if (absX >= 6 && absX <= 8 && lotZ >= 1 && lotZ <= 3) {
            return 3u; // Concrete driveway
        }
        return 4u; // Neighbor lawn ground
    }

    // Distant Electrical Grid Infrastructure in Fog
    if (pos.y >= 1 && pos.y <= 8 && (pos.z < -2 || pos.z > 14)) {
        if (pos.x == 5 && lotZ == 0 && pos.y <= 7) {
            return 8u;
        }
        if (pos.x >= 4 && pos.x <= 6 && lotZ == 0 && pos.y == 7) {
            return 8u;
        }
        if (pos.x == 5 && lotZ == 1 && pos.y == 6) {
            return 9u;
        }
        if (pos.x == 4 && pos.y == 7 && pos.z >= -10 && pos.z <= 40) {
            return 9u;
        }
    }

    // Generic Sealed Exterior Neighbor House Shells (zero interior furniture)
    if (absX >= 9 && absX <= 17 && lotZ >= 2 && lotZ <= 10) {
        if (pos.y >= 4 && pos.y <= 6) {
            let roof_inset = pos.y - 3;
            if (absX >= (9 + roof_inset) && absX <= (17 - roof_inset) &&
                lotZ >= (2 + roof_inset) && lotZ <= (10 - roof_inset)) {
                return 6u;
            }
        }
        if (pos.y >= 1 && pos.y <= 3) {
            if ((absX == 9 || absX == 17) && lotZ == 6 && pos.y <= 2) {
                return 15u;
            }
            if (pos.y == 2 &&
                (((absX == 9 || absX == 17) && (lotZ == 4 || lotZ == 8)) ||
                 ((lotZ == 2 || lotZ == 10) && (absX == 11 || absX == 15)))) {
                return 7u;
            }
            if (absX == 9 || absX == 17 || lotZ == 2 || lotZ == 10) {
                return 5u;
            }
        }
    }

    // Distant Neighbor Boundary Fences & Trees/Shrubs
    if (pos.y == 1 && (lotZ == 15 || lotZ == 1) && absX >= 7 && absX <= 20) {
        return 10u;
    }
    if (pos.y >= 1 && pos.y <= 3) {
        let is_tree_xz = (absX == 7 && lotZ == 14) ||
                         (absX == 21 && (lotZ == 4 || lotZ == 12));
        if (is_tree_xz) {
            if (pos.y == 1) { return 35u; }
            return 36u;
        }
        if (pos.y == 1 && absX == 8 && (lotZ == 11 || lotZ == 12)) {
            return 38u;
        }
    }

    return 0u;
}

// Map-Agnostic O(1) Voxel Material Lookup via WorldChunkBuffer + DynamicVoxelStore
fn get_voxel_material(pos: vec3<i32>) -> u32 {
    let dyn_mat = get_dynamic_voxel_material(pos);
    if (dyn_mat != 0u) {
        return dyn_mat;
    }

    if (is_inside_loaded_chunk(pos)) {
        return get_loaded_chunk_material(pos);
    }

    if (world_chunk.is_loaded != 0u) {
        if (is_inside_loaded_chunk_xz(pos) || (world_chunk.flags & 1u) == 0u) {
            return 0u;
        }
    }

    return get_distant_neighbor_silhouette_material(pos);
}

fn get_sky_color(rd: vec3<f32>) -> vec3<f32> {
    let diurnal_phase = sin(camera.avatar_pos.w * 0.01308997) * 0.5 + 0.5;
    let day_top = vec3<f32>(0.18, 0.44, 0.78);
    let dusk_top = vec3<f32>(0.10, 0.18, 0.38);
    let day_horizon = vec3<f32>(0.72, 0.85, 0.96);
    let dusk_horizon = vec3<f32>(0.84, 0.54, 0.40);
    let sky_top = mix(dusk_top, day_top, diurnal_phase);
    let sky_horizon = mix(dusk_horizon, day_horizon, diurnal_phase);
    let t = clamp(rd.y * 0.5 + 0.5, 0.0, 1.0);
    let sun_dir = normalize(vec3<f32>(0.5, 0.8, -0.4));
    let sun_glow = pow(max(dot(rd, sun_dir), 0.0), 24.0) * 0.38 * diurnal_phase;
    return mix(sky_horizon, sky_top, t) + vec3<f32>(1.0, 0.94, 0.78) * sun_glow;
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

// Analytic Ray-AABB intersection returning vec4(normal.xyz, t_hit) where t_hit < 0.0 means miss
fn intersect_aabb_normal(ro: vec3<f32>, rd: vec3<f32>, bmin: vec3<f32>, bmax: vec3<f32>) -> vec4<f32> {
    let inv_rd = vec3<f32>(1.0) / select(rd, vec3<f32>(1e-6), abs(rd) < vec3<f32>(1e-6));
    let t0s = (bmin - ro) * inv_rd;
    let t1s = (bmax - ro) * inv_rd;
    let tsmaller = min(t0s, t1s);
    let tbigger  = max(t0s, t1s);
    let t_enter = max(tsmaller.x, max(tsmaller.y, tsmaller.z));
    let t_exit  = min(tbigger.x, min(tbigger.y, tbigger.z));
    if (t_enter > t_exit || t_exit <= 0.0) {
        return vec4<f32>(0.0, 1.0, 0.0, -1.0);
    }
    var n = vec3<f32>(0.0, 1.0, 0.0);
    if (tsmaller.x >= tsmaller.y && tsmaller.x >= tsmaller.z) {
        n = vec3<f32>(-sign(rd.x), 0.0, 0.0);
    } else if (tsmaller.y >= tsmaller.x && tsmaller.y >= tsmaller.z) {
        n = vec3<f32>(0.0, -sign(rd.y), 0.0);
    } else {
        n = vec3<f32>(0.0, 0.0, -sign(rd.z));
    }
    return vec4<f32>(n, max(0.001, t_enter));
}

// Exact Ray Intersection with a True 8-Faceted Low-Poly Octahedron (|x-cx|/rx + |y-cy|/ry + |z-cz|/rz <= 1.0)
fn intersect_octahedron(ro: vec3<f32>, rd: vec3<f32>, center: vec3<f32>, rad: vec3<f32>) -> vec4<f32> {
    let inv_rad = vec3<f32>(1.0) / max(rad, vec3<f32>(1e-5));
    let p = (ro - center) * inv_rad;
    let d = rd * inv_rad;

    var t_enter: f32 = -1e9;
    var t_exit: f32 = 1e9;
    var n_enter = vec3<f32>(0.0, 1.0, 0.0);

    let signs = array<vec3<f32>, 4>(
        vec3<f32>( 1.0,  1.0,  1.0),
        vec3<f32>(-1.0,  1.0,  1.0),
        vec3<f32>( 1.0, -1.0,  1.0),
        vec3<f32>( 1.0,  1.0, -1.0)
    );

    for (var k = 0; k < 4; k = k + 1) {
        let s = signs[k];
        let o_k = dot(s, p);
        let v_k = dot(s, d);
        if (abs(v_k) < 1e-6) {
            if (abs(o_k) > 1.0) {
                return vec4<f32>(0.0, 1.0, 0.0, -1.0);
            }
        } else {
            let t1 = (-1.0 - o_k) / v_k;
            let t2 = ( 1.0 - o_k) / v_k;
            let t_in = min(t1, t2);
            let t_out = max(t1, t2);
            if (t_in > t_enter) {
                t_enter = t_in;
                let face_s = select(s, -s, v_k > 0.0);
                n_enter = normalize(face_s * inv_rad);
            }
            t_exit = min(t_exit, t_out);
        }
    }

    if (t_enter > t_exit || t_exit <= 0.0) {
        return vec4<f32>(0.0, 1.0, 0.0, -1.0);
    }
    return vec4<f32>(n_enter, max(0.001, t_enter));
}

fn is_roof_or_wall_neighbor(mat: u32) -> bool {
    return mat == 6u || mat == 17u || mat == 5u || mat == 13u || mat == 14u;
}

fn is_wall_or_opening_neighbor(mat: u32) -> bool {
    return mat == 5u || mat == 7u || mat == 13u || mat == 14u || mat == 15u || mat == 57u;
}

// Data-Driven Smooth 45-degree Pitched Hip/Gable Low-Poly Roof Intersection inside DDA cell [mapPos, mapPos + 1]
fn intersect_sloped_roof_voxel(ro: vec3<f32>, rd: vec3<f32>, mapPos: vec3<i32>) -> vec4<f32> {
    let cell_min = vec3<f32>(f32(mapPos.x), f32(mapPos.y), f32(mapPos.z));
    let lro = ro - cell_min;
    let inv_rd = vec3<f32>(1.0) / select(rd, vec3<f32>(1e-6), abs(rd) < vec3<f32>(1e-6));
    let t0s = (vec3<f32>(0.0) - lro) * inv_rd;
    let t1s = (vec3<f32>(1.0) - lro) * inv_rd;
    let tsmaller = min(t0s, t1s);
    let tbigger  = max(t0s, t1s);
    var t_enter = max(tsmaller.x, max(tsmaller.y, tsmaller.z));
    var t_exit  = min(tbigger.x, min(tbigger.y, tbigger.z));
    if (t_enter > t_exit || t_exit <= 0.0) {
        return vec4<f32>(0.0, 1.0, 0.0, -1.0);
    }

    var n_enter = vec3<f32>(0.0, 1.0, 0.0);
    if (tsmaller.x >= tsmaller.y && tsmaller.x >= tsmaller.z) {
        n_enter = vec3<f32>(-sign(rd.x), 0.0, 0.0);
    } else if (tsmaller.y >= tsmaller.x && tsmaller.y >= tsmaller.z) {
        n_enter = vec3<f32>(0.0, -sign(rd.y), 0.0);
    } else {
        n_enter = vec3<f32>(0.0, 0.0, -sign(rd.z));
    }

    // Check neighbor cells (-x, +x, -z, +z) to determine which side(s) slope at 45 degrees
    let open_nx = !is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>(-1, 0, 0))) &&
                  !is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>(-1, 1, 0)));
    let open_px = !is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>( 1, 0, 0))) &&
                  !is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>( 1, 1, 0)));
    let open_nz = !is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>( 0, 0, -1))) &&
                  !is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>( 0, 1, -1)));
    let open_pz = !is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>( 0, 0,  1))) &&
                  !is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>( 0, 1,  1)));

    if (open_nx) {
        let n_p = vec3<f32>(-1.0, 1.0, 0.0);
        let denom = dot(n_p, rd);
        let dist = 0.0 - dot(n_p, lro);
        if (abs(denom) < 1e-6) {
            if (dist < 0.0) { return vec4<f32>(0.0, 1.0, 0.0, -1.0); }
        } else {
            let t_c = dist / denom;
            if (denom < 0.0) {
                if (t_c > t_enter) { t_enter = t_c; n_enter = normalize(n_p); }
            } else {
                t_exit = min(t_exit, t_c);
            }
        }
    }
    if (open_px) {
        let n_p = vec3<f32>(1.0, 1.0, 0.0);
        let denom = dot(n_p, rd);
        let dist = 1.0 - dot(n_p, lro);
        if (abs(denom) < 1e-6) {
            if (dist < 0.0) { return vec4<f32>(0.0, 1.0, 0.0, -1.0); }
        } else {
            let t_c = dist / denom;
            if (denom < 0.0) {
                if (t_c > t_enter) { t_enter = t_c; n_enter = normalize(n_p); }
            } else {
                t_exit = min(t_exit, t_c);
            }
        }
    }
    if (open_nz) {
        let n_p = vec3<f32>(0.0, 1.0, -1.0);
        let denom = dot(n_p, rd);
        let dist = 0.0 - dot(n_p, lro);
        if (abs(denom) < 1e-6) {
            if (dist < 0.0) { return vec4<f32>(0.0, 1.0, 0.0, -1.0); }
        } else {
            let t_c = dist / denom;
            if (denom < 0.0) {
                if (t_c > t_enter) { t_enter = t_c; n_enter = normalize(n_p); }
            } else {
                t_exit = min(t_exit, t_c);
            }
        }
    }
    if (open_pz) {
        let n_p = vec3<f32>(0.0, 1.0, 1.0);
        let denom = dot(n_p, rd);
        let dist = 1.0 - dot(n_p, lro);
        if (abs(denom) < 1e-6) {
            if (dist < 0.0) { return vec4<f32>(0.0, 1.0, 0.0, -1.0); }
        } else {
            let t_c = dist / denom;
            if (denom < 0.0) {
                if (t_c > t_enter) { t_enter = t_c; n_enter = normalize(n_p); }
            } else {
                t_exit = min(t_exit, t_c);
            }
        }
    }

    if (t_enter > t_exit || t_exit <= 0.0) {
        return vec4<f32>(0.0, 1.0, 0.0, -1.0);
    }
    return vec4<f32>(n_enter, max(0.001, t_enter));
}

// Sculpted Sub-Voxel Low-Poly Props, Furniture/Appliances, Hauling Tools, Doors/Windows & Faceted Trees
fn intersect_subvoxel_prop(ro: vec3<f32>, rd: vec3<f32>, mapPos: vec3<i32>, mat: u32) -> vec4<f32> {
    if (mat == 6u || mat == 17u) {
        return intersect_sloped_roof_voxel(ro, rd, mapPos);
    }

    let cell_min = vec3<f32>(f32(mapPos.x), f32(mapPos.y), f32(mapPos.z));
    let lro = ro - cell_min;

    var best_hit = vec4<f32>(0.0, 1.0, 0.0, 1e9);

    if (mat == 10u) {
        let post = intersect_aabb_normal(lro, rd, vec3<f32>(0.37, 0.0, 0.37), vec3<f32>(0.63, 0.92, 0.63));
        if (post.w > 0.0 && post.w < best_hit.w) { best_hit = post; }

        let conn_x = (get_voxel_material(mapPos + vec3<i32>(-1, 0, 0)) == 10u) ||
                     (get_voxel_material(mapPos + vec3<i32>( 1, 0, 0)) == 10u);
        let conn_z = (get_voxel_material(mapPos + vec3<i32>( 0, 0, -1)) == 10u) ||
                     (get_voxel_material(mapPos + vec3<i32>( 0, 0,  1)) == 10u);
        let runs_x = conn_x || !conn_z;
        let runs_z = conn_z;
        if (runs_x) {
            let rail_x1 = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.26, 0.44), vec3<f32>(1.0, 0.40, 0.56));
            if (rail_x1.w > 0.0 && rail_x1.w < best_hit.w) { best_hit = rail_x1; }
            let rail_x2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.60, 0.44), vec3<f32>(1.0, 0.74, 0.56));
            if (rail_x2.w > 0.0 && rail_x2.w < best_hit.w) { best_hit = rail_x2; }
        }
        if (runs_z) {
            let rail_z1 = intersect_aabb_normal(lro, rd, vec3<f32>(0.44, 0.26, 0.0), vec3<f32>(0.56, 0.40, 1.0));
            if (rail_z1.w > 0.0 && rail_z1.w < best_hit.w) { best_hit = rail_z1; }
            let rail_z2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.44, 0.60, 0.0), vec3<f32>(0.56, 0.74, 1.0));
            if (rail_z2.w > 0.0 && rail_z2.w < best_hit.w) { best_hit = rail_z2; }
        }
    } else if (mat == 8u) {
        if (mapPos.x == 5) {
            let pole = intersect_aabb_normal(lro, rd, vec3<f32>(0.38, 0.0, 0.38), vec3<f32>(0.62, 1.0, 0.62));
            if (pole.w > 0.0 && pole.w < best_hit.w) { best_hit = pole; }
        }
        if (mapPos.y == 7) {
            let arm = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.72, 0.38), vec3<f32>(1.0, 0.94, 0.62));
            if (arm.w > 0.0 && arm.w < best_hit.w) { best_hit = arm; }
        }
    } else if (mat == 9u) {
        if (mapPos.y == 7) {
            let wire = intersect_aabb_normal(lro, rd, vec3<f32>(0.45, 0.80, 0.0), vec3<f32>(0.55, 0.88, 1.0));
            if (wire.w > 0.0 && wire.w < best_hit.w) { best_hit = wire; }
        } else {
            let drum = intersect_aabb_normal(lro, rd, vec3<f32>(0.24, 0.15, 0.24), vec3<f32>(0.76, 0.88, 0.76));
            if (drum.w > 0.0 && drum.w < best_hit.w) { best_hit = drum; }
        }
    } else if (mat == 7u) {
        let wall_z = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(0, 0, -1))) ||
                     is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(0, 0,  1)));
        let wall_x = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(-1, 0, 0))) ||
                     is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>( 1, 0, 0)));
        let on_x_wall = wall_z && !wall_x;
        if (on_x_wall) {
            let pane = intersect_aabb_normal(lro, rd, vec3<f32>(0.32, 0.0, 0.0), vec3<f32>(0.68, 1.0, 1.0));
            if (pane.w > 0.0 && pane.w < best_hit.w) { best_hit = pane; }
            let sill = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.0, 0.0), vec3<f32>(0.92, 0.14, 1.0));
            if (sill.w > 0.0 && sill.w < best_hit.w) { best_hit = sill; }
            let lintel = intersect_aabb_normal(lro, rd, vec3<f32>(0.14, 0.88, 0.0), vec3<f32>(0.86, 1.0, 1.0));
            if (lintel.w > 0.0 && lintel.w < best_hit.w) { best_hit = lintel; }
        } else {
            let pane = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.0, 0.32), vec3<f32>(1.0, 1.0, 0.68));
            if (pane.w > 0.0 && pane.w < best_hit.w) { best_hit = pane; }
            let sill = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.0, 0.08), vec3<f32>(1.0, 0.14, 0.92));
            if (sill.w > 0.0 && sill.w < best_hit.w) { best_hit = sill; }
            let lintel = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.88, 0.14), vec3<f32>(1.0, 1.0, 0.86));
            if (lintel.w > 0.0 && lintel.w < best_hit.w) { best_hit = lintel; }
        }
    } else if (mat == 15u) {
        let wall_z = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(0, 0, -1))) ||
                     is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(0, 0,  1)));
        let wall_x = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(-1, 0, 0))) ||
                     is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>( 1, 0, 0)));
        let on_x_wall = wall_z && !wall_x;
        if (on_x_wall) {
            let door = intersect_aabb_normal(lro, rd, vec3<f32>(0.30, 0.0, 0.0), vec3<f32>(0.70, 1.0, 1.0));
            if (door.w > 0.0 && door.w < best_hit.w) { best_hit = door; }
            let jamb1 = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.0, 0.0), vec3<f32>(0.88, 1.0, 0.10));
            if (jamb1.w > 0.0 && jamb1.w < best_hit.w) { best_hit = jamb1; }
            let jamb2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.0, 0.90), vec3<f32>(0.88, 1.0, 1.0));
            if (jamb2.w > 0.0 && jamb2.w < best_hit.w) { best_hit = jamb2; }
        } else {
            let door = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.0, 0.30), vec3<f32>(1.0, 1.0, 0.70));
            if (door.w > 0.0 && door.w < best_hit.w) { best_hit = door; }
            let jamb1 = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.0, 0.12), vec3<f32>(0.10, 1.0, 0.88));
            if (jamb1.w > 0.0 && jamb1.w < best_hit.w) { best_hit = jamb1; }
            let jamb2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.90, 0.0, 0.12), vec3<f32>(1.0, 1.0, 0.88));
            if (jamb2.w > 0.0 && jamb2.w < best_hit.w) { best_hit = jamb2; }
        }
    } else if (mat == 57u) {
        // MAT_GARAGE_DOOR: Ribbed White/Slate Sectional Roll-Up Garage Door
        let wall_z = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(0, 0, -1))) ||
                     is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(0, 0,  1)));
        let wall_x = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(-1, 0, 0))) ||
                     is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>( 1, 0, 0)));
        let on_x_wall = !wall_x || wall_z;
        if (on_x_wall) {
            let gdoor = intersect_aabb_normal(lro, rd, vec3<f32>(0.26, 0.0, 0.0), vec3<f32>(0.68, 1.0, 1.0));
            if (gdoor.w > 0.0 && gdoor.w < best_hit.w) { best_hit = gdoor; }
        } else {
            let gdoor = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.0, 0.26), vec3<f32>(1.0, 1.0, 0.68));
            if (gdoor.w > 0.0 && gdoor.w < best_hit.w) { best_hit = gdoor; }
        }
    } else if (mat == 11u) {
        let rack = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.0, 0.08), vec3<f32>(0.92, 0.14, 0.92));
        if (rack.w > 0.0 && rack.w < best_hit.w) { best_hit = rack; }
        let strut = intersect_aabb_normal(lro, rd, vec3<f32>(0.36, 0.14, 0.36), vec3<f32>(0.64, 0.42, 0.64));
        if (strut.w > 0.0 && strut.w < best_hit.w) { best_hit = strut; }

        let inv_rd = vec3<f32>(1.0) / select(rd, vec3<f32>(1e-6), abs(rd) < vec3<f32>(1e-6));
        let t0s = (vec3<f32>(0.04, 0.16, 0.04) - lro) * inv_rd;
        let t1s = (vec3<f32>(0.96, 0.76, 0.96) - lro) * inv_rd;
        let tsm = min(t0s, t1s);
        let tbg = max(t0s, t1s);
        var te = max(tsm.x, max(tsm.y, tsm.z));
        var tx = min(tbg.x, min(tbg.y, tbg.z));
        var ne = vec3<f32>(0.0, 1.0, 0.0);
        if (tsm.x >= tsm.y && tsm.x >= tsm.z) { ne = vec3<f32>(-sign(rd.x), 0.0, 0.0); }
        else if (tsm.y >= tsm.x && tsm.y >= tsm.z) { ne = vec3<f32>(0.0, -sign(rd.y), 0.0); }
        else { ne = vec3<f32>(0.0, 0.0, -sign(rd.z)); }

        let n_pv = normalize(vec3<f32>(0.0, 0.86, -0.50));
        let d_pv = dot(n_pv, rd);
        let o_pv = dot(n_pv, lro - vec3<f32>(0.5, 0.46, 0.5));
        if (abs(d_pv) > 1e-6) {
            let t_top = ( 0.05 - o_pv) / d_pv;
            let t_bot = (-0.05 - o_pv) / d_pv;
            let tin = min(t_top, t_bot);
            let tout = max(t_top, t_bot);
            if (tin > te) {
                te = tin;
                ne = select(-n_pv, n_pv, d_pv < 0.0);
            }
            tx = min(tx, tout);
        } else if (abs(o_pv) > 0.05) {
            te = 1e9;
        }
        if (te <= tx && tx > 0.0) {
            let t_hit_pv = max(0.001, te);
            if (t_hit_pv < best_hit.w) {
                best_hit = vec4<f32>(ne, t_hit_pv);
            }
        }
    } else if (mat == 12u) {
        let tank = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.0, 0.12), vec3<f32>(0.88, 0.88, 0.88));
        if (tank.w > 0.0 && tank.w < best_hit.w) { best_hit = tank; }
        let lid = intersect_aabb_normal(lro, rd, vec3<f32>(0.28, 0.88, 0.28), vec3<f32>(0.72, 0.96, 0.72));
        if (lid.w > 0.0 && lid.w < best_hit.w) { best_hit = lid; }
    } else if (mat == 16u) {
        let stove = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.0, 0.08), vec3<f32>(0.92, 0.52, 0.92));
        if (stove.w > 0.0 && stove.w < best_hit.w) { best_hit = stove; }
        let riser = intersect_aabb_normal(lro, rd, vec3<f32>(0.30, 0.52, 0.30), vec3<f32>(0.70, 1.0, 0.70));
        if (riser.w > 0.0 && riser.w < best_hit.w) { best_hit = riser; }
    } else if (mat == 23u) {
        // Bedroom Bed + Mattress + Headboard + Plump Pillows
        let mattress = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.08), vec3<f32>(0.98, 0.45, 0.94));
        if (mattress.w > 0.0 && mattress.w < best_hit.w) { best_hit = mattress; }
        let headboard = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.45, 0.76), vec3<f32>(0.98, 0.84, 0.94));
        if (headboard.w > 0.0 && headboard.w < best_hit.w) { best_hit = headboard; }
        let pillow = intersect_aabb_normal(lro, rd, vec3<f32>(0.14, 0.45, 0.52), vec3<f32>(0.86, 0.58, 0.76));
        if (pillow.w > 0.0 && pillow.w < best_hit.w) { best_hit = pillow; }
    } else if (mat == 24u || mat == 41u) {
        // Low-Poly Oven + Stove (0.82m counter height + Communal Pea Soup & Bone Broth Stockpot on top burner)
        let oven = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.0, 0.08), vec3<f32>(0.96, 0.82, 0.88));
        if (oven.w > 0.0 && oven.w < best_hit.w) { best_hit = oven; }
        let pot = intersect_aabb_normal(lro, rd, vec3<f32>(0.22, 0.82, 0.24), vec3<f32>(0.70, 0.99, 0.72));
        if (pot.w > 0.0 && pot.w < best_hit.w) { best_hit = pot; }
    } else if (mat == 40u) {
        // Tall Brushed Stainless/Mint Refrigerator + Vertical Handles
        let body = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.0, 0.08), vec3<f32>(0.92, 0.96, 0.86));
        if (body.w > 0.0 && body.w < best_hit.w) { best_hit = body; }
        let handle = intersect_aabb_normal(lro, rd, vec3<f32>(0.16, 0.26, 0.86), vec3<f32>(0.24, 0.88, 0.94));
        if (handle.w > 0.0 && handle.w < best_hit.w) { best_hit = handle; }
    } else if (mat == 42u) {
        // Kitchen Cabinets & Counter with Butcher-Block Top + Recessed Sink + Chrome Faucet
        let cab = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.08), vec3<f32>(0.98, 0.82, 0.88));
        if (cab.w > 0.0 && cab.w < best_hit.w) { best_hit = cab; }
        let faucet = intersect_aabb_normal(lro, rd, vec3<f32>(0.44, 0.82, 0.14), vec3<f32>(0.56, 0.98, 0.36));
        if (faucet.w > 0.0 && faucet.w < best_hit.w) { best_hit = faucet; }
    } else if (mat == 43u) {
        // Dining Table + Tucked Chair + Dinner Plate
        let top = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.44, 0.14), vec3<f32>(0.92, 0.52, 0.88));
        if (top.w > 0.0 && top.w < best_hit.w) { best_hit = top; }
        let ped = intersect_aabb_normal(lro, rd, vec3<f32>(0.36, 0.0, 0.38), vec3<f32>(0.64, 0.44, 0.64));
        if (ped.w > 0.0 && ped.w < best_hit.w) { best_hit = ped; }
        let chair = intersect_aabb_normal(lro, rd, vec3<f32>(0.24, 0.0, 0.02), vec3<f32>(0.76, 0.76, 0.22));
        if (chair.w > 0.0 && chair.w < best_hit.w) { best_hit = chair; }
        let plate = intersect_aabb_normal(lro, rd, vec3<f32>(0.34, 0.52, 0.38), vec3<f32>(0.66, 0.56, 0.68));
        if (plate.w > 0.0 && plate.w < best_hit.w) { best_hit = plate; }
    } else if (mat == 44u) {
        // Office Bookshelf & Storage Cabinet
        let shelf = intersect_aabb_normal(lro, rd, vec3<f32>(0.06, 0.0, 0.08), vec3<f32>(0.94, 0.94, 0.62));
        if (shelf.w > 0.0 && shelf.w < best_hit.w) { best_hit = shelf; }
    } else if (mat == 45u) {
        // Shower Stall (Ceramic Base Tray + Frosted Glass Enclosure + Chrome Showerhead)
        let tray = intersect_aabb_normal(lro, rd, vec3<f32>(0.06, 0.0, 0.06), vec3<f32>(0.94, 0.14, 0.94));
        if (tray.w > 0.0 && tray.w < best_hit.w) { best_hit = tray; }
        let glass = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.14, 0.08), vec3<f32>(0.92, 0.96, 0.92));
        if (glass.w > 0.0 && glass.w < best_hit.w) { best_hit = glass; }
    } else if (mat == 46u) {
        // Sculpted White Porcelain Toilet (Bowl + Rear Tank)
        let bowl = intersect_aabb_normal(lro, rd, vec3<f32>(0.20, 0.0, 0.18), vec3<f32>(0.80, 0.42, 0.78));
        if (bowl.w > 0.0 && bowl.w < best_hit.w) { best_hit = bowl; }
        let tank = intersect_aabb_normal(lro, rd, vec3<f32>(0.54, 0.42, 0.20), vec3<f32>(0.88, 0.80, 0.76));
        if (tank.w > 0.0 && tank.w < best_hit.w) { best_hit = tank; }
    } else if (mat == 47u) {
        // Bathroom Sink + Vanity Cabinet + Wall Mirror
        let vanity = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.0, 0.08), vec3<f32>(0.88, 0.68, 0.68));
        if (vanity.w > 0.0 && vanity.w < best_hit.w) { best_hit = vanity; }
        let mirror = intersect_aabb_normal(lro, rd, vec3<f32>(0.18, 0.68, 0.06), vec3<f32>(0.82, 0.96, 0.18));
        if (mirror.w > 0.0 && mirror.w < best_hit.w) { best_hit = mirror; }
    } else if (mat == 48u) {
        // Cozy Low-Poly Couch / Sofa (Seat Cushions + Backrest + Data-Driven End Armrests)
        let seat = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.10), vec3<f32>(0.98, 0.38, 0.86));
        if (seat.w > 0.0 && seat.w < best_hit.w) { best_hit = seat; }
        let back = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.38, 0.08), vec3<f32>(0.98, 0.74, 0.32));
        if (back.w > 0.0 && back.w < best_hit.w) { best_hit = back; }
        if (get_voxel_material(mapPos + vec3<i32>(-1, 0, 0)) != 48u) {
            let arm_l = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.38, 0.10), vec3<f32>(0.22, 0.56, 0.86));
            if (arm_l.w > 0.0 && arm_l.w < best_hit.w) { best_hit = arm_l; }
        }
        if (get_voxel_material(mapPos + vec3<i32>( 1, 0, 0)) != 48u) {
            let arm_r = intersect_aabb_normal(lro, rd, vec3<f32>(0.78, 0.38, 0.10), vec3<f32>(0.98, 0.56, 0.86));
            if (arm_r.w > 0.0 && arm_r.w < best_hit.w) { best_hit = arm_r; }
        }
    } else if (mat == 49u) {
        // TV on Media Furniture + VideoGame Console
        let stand = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.32), vec3<f32>(0.98, 0.36, 0.92));
        if (stand.w > 0.0 && stand.w < best_hit.w) { best_hit = stand; }
        let tv = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.38, 0.55), vec3<f32>(0.92, 0.88, 0.73));
        if (tv.w > 0.0 && tv.w < best_hit.w) { best_hit = tv; }
        let console_box = intersect_aabb_normal(lro, rd, vec3<f32>(0.62, 0.36, 0.34), vec3<f32>(0.88, 0.46, 0.52));
        if (console_box.w > 0.0 && console_box.w < best_hit.w) { best_hit = console_box; }
    } else if (mat == 50u) {
        // Sound System Tower Speakers
        let spk = intersect_aabb_normal(lro, rd, vec3<f32>(0.22, 0.0, 0.36), vec3<f32>(0.78, 0.80, 0.88));
        if (spk.w > 0.0 && spk.w < best_hit.w) { best_hit = spk; }
    } else if (mat == 51u) {
        // Nightstand + Glowing Bedside Lamp
        let tbl = intersect_aabb_normal(lro, rd, vec3<f32>(0.20, 0.0, 0.20), vec3<f32>(0.80, 0.46, 0.80));
        if (tbl.w > 0.0 && tbl.w < best_hit.w) { best_hit = tbl; }
        let shade = intersect_aabb_normal(lro, rd, vec3<f32>(0.30, 0.46, 0.30), vec3<f32>(0.70, 0.78, 0.70));
        if (shade.w > 0.0 && shade.w < best_hit.w) { best_hit = shade; }
    } else if (mat == 52u) {
        // Walnut Wardrobe / Dresser
        let ward = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.0, 0.16), vec3<f32>(0.92, 0.94, 0.92));
        if (ward.w > 0.0 && ward.w < best_hit.w) { best_hit = ward; }
    } else if (mat == 53u) {
        // Full-Length Standing Mirror
        let mir = intersect_aabb_normal(lro, rd, vec3<f32>(0.16, 0.0, 0.38), vec3<f32>(0.84, 0.92, 0.58));
        if (mir.w > 0.0 && mir.w < best_hit.w) { best_hit = mir; }
    } else if (mat == 54u) {
        // Garage Workbench + Red Machinist Vise + Brass/Copper Tool Caddy
        let bench = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.0, 0.12), vec3<f32>(0.96, 0.76, 0.90));
        if (bench.w > 0.0 && bench.w < best_hit.w) { best_hit = bench; }
        let vise = intersect_aabb_normal(lro, rd, vec3<f32>(0.18, 0.76, 0.16), vec3<f32>(0.48, 0.94, 0.46));
        if (vise.w > 0.0 && vise.w < best_hit.w) { best_hit = vise; }
        let toolbox = intersect_aabb_normal(lro, rd, vec3<f32>(0.60, 0.76, 0.46), vec3<f32>(0.90, 0.90, 0.82));
        if (toolbox.w > 0.0 && toolbox.w < best_hit.w) { best_hit = toolbox; }
    } else if (mat == 55u) {
        // Tool-Hanging Pegboard Wall + Bench
        let bench = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.14), vec3<f32>(0.98, 0.74, 0.90));
        if (bench.w > 0.0 && bench.w < best_hit.w) { best_hit = bench; }
        let peg = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.50, 0.72), vec3<f32>(0.96, 0.98, 0.94));
        if (peg.w > 0.0 && peg.w < best_hit.w) { best_hit = peg; }
    } else if (mat == 56u) {
        // Heavy-Duty Garage Storage Rack
        let rack = intersect_aabb_normal(lro, rd, vec3<f32>(0.06, 0.0, 0.12), vec3<f32>(0.94, 0.95, 0.90));
        if (rack.w > 0.0 && rack.w < best_hit.w) { best_hit = rack; }
    } else if (mat == 19u || mat == 34u) {
        // Backyard Raised Cedar Planter Bed (y in [0.0..0.24]) + A-Frame Pea/Bean Trellis & Root Veggies
        let planter = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.0, 0.04), vec3<f32>(0.96, 0.24, 0.96));
        if (planter.w > 0.0 && planter.w < best_hit.w) { best_hit = planter; }
        if (mat == 19u) {
            let sprouts = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.24, 0.10), vec3<f32>(0.88, 0.34, 0.90));
            if (sprouts.w > 0.0 && sprouts.w < best_hit.w) { best_hit = sprouts; }
        } else {
            let root_row = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.24, 0.10), vec3<f32>(0.88, 0.38, 0.90));
            if (root_row.w > 0.0 && root_row.w < best_hit.w) { best_hit = root_row; }
            let trellis_low = intersect_aabb_normal(lro, rd, vec3<f32>(0.24, 0.38, 0.08), vec3<f32>(0.76, 0.58, 0.92));
            if (trellis_low.w > 0.0 && trellis_low.w < best_hit.w) { best_hit = trellis_low; }
            let trellis_top = intersect_aabb_normal(lro, rd, vec3<f32>(0.35, 0.58, 0.06), vec3<f32>(0.65, 0.78, 0.94));
            if (trellis_top.w > 0.0 && trellis_top.w < best_hit.w) { best_hit = trellis_top; }
        }
    } else if (mat == 58u) {
        // MAT_POULTRY_COOP: Elevated Timber Chicken Coop & BSFL Bioconversion Feeder
        let is_feeder_half = (get_voxel_material(mapPos + vec3<i32>(0, 0, -1)) == 58u);
        if (!is_feeder_half) {
            let legs_f = intersect_aabb_normal(lro, rd, vec3<f32>(0.18, 0.0, 0.12), vec3<f32>(0.86, 0.18, 0.26));
            if (legs_f.w > 0.0 && legs_f.w < best_hit.w) { best_hit = legs_f; }
            let legs_b = intersect_aabb_normal(lro, rd, vec3<f32>(0.18, 0.0, 0.74), vec3<f32>(0.86, 0.18, 0.88));
            if (legs_b.w > 0.0 && legs_b.w < best_hit.w) { best_hit = legs_b; }
            let coop_body = intersect_aabb_normal(lro, rd, vec3<f32>(0.18, 0.18, 0.10), vec3<f32>(0.88, 0.76, 0.90));
            if (coop_body.w > 0.0 && coop_body.w < best_hit.w) { best_hit = coop_body; }
            let roof_eaves = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.74, 0.06), vec3<f32>(0.94, 0.86, 0.94));
            if (roof_eaves.w > 0.0 && roof_eaves.w < best_hit.w) { best_hit = roof_eaves; }
            let roof_peak = intersect_aabb_normal(lro, rd, vec3<f32>(0.30, 0.86, 0.06), vec3<f32>(0.76, 0.96, 0.94));
            if (roof_peak.w > 0.0 && roof_peak.w < best_hit.w) { best_hit = roof_peak; }
            let nest_box = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.28, 0.22), vec3<f32>(0.28, 0.56, 0.78));
            if (nest_box.w > 0.0 && nest_box.w < best_hit.w) { best_hit = nest_box; }
            let ramp = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.36), vec3<f32>(0.24, 0.22, 0.64));
            if (ramp.w > 0.0 && ramp.w < best_hit.w) { best_hit = ramp; }
        } else {
            let trough = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.0, 0.14), vec3<f32>(0.92, 0.18, 0.86));
            if (trough.w > 0.0 && trough.w < best_hit.w) { best_hit = trough; }
            let hopper = intersect_aabb_normal(lro, rd, vec3<f32>(0.42, 0.18, 0.18), vec3<f32>(0.90, 0.46, 0.82));
            if (hopper.w > 0.0 && hopper.w < best_hit.w) { best_hit = hopper; }
            let waterer = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.18, 0.24), vec3<f32>(0.38, 0.38, 0.72));
            if (waterer.w > 0.0 && waterer.w < best_hit.w) { best_hit = waterer; }
        }
    } else if (mat == 59u) {
        // MAT_CHICKEN_HEN: 2 Animated Low-Poly 3D Heritage Hens per cell in the Backyard Poultry Run
        let t_hen = camera.avatar_pos.w;
        let wx1 = sin(t_hen * 1.3 + f32(mapPos.x * 5 + mapPos.z * 3)) * 0.14;
        let wz1 = cos(t_hen * 0.9 + f32(mapPos.z * 7)) * 0.14;
        let off1 = vec3<f32>(wx1, 0.0, wz1);
        let off2 = vec3<f32>(-wz1 * 0.8, 0.0, wx1 * 0.8);
        let peck = abs(sin(t_hen * 6.0 + f32(mapPos.x * 3 + mapPos.z * 5))) * 0.12;
        let peck2 = abs(cos(t_hen * 5.2 + f32(mapPos.x * 7 + mapPos.z * 3))) * 0.11;

        let hen_legs1 = intersect_aabb_normal(lro, rd, vec3<f32>(0.29, 0.0, 0.32) + off1, vec3<f32>(0.41, 0.08, 0.44) + off1);
        if (hen_legs1.w > 0.0 && hen_legs1.w < best_hit.w) { best_hit = hen_legs1; }
        let hen_body = intersect_aabb_normal(lro, rd, vec3<f32>(0.22, 0.08, 0.25) + off1, vec3<f32>(0.48, 0.32, 0.53) + off1);
        if (hen_body.w > 0.0 && hen_body.w < best_hit.w) { best_hit = hen_body; }
        let hen_tail = intersect_aabb_normal(lro, rd, vec3<f32>(0.25, 0.22, 0.49) + off1, vec3<f32>(0.45, 0.38, 0.62) + off1);
        if (hen_tail.w > 0.0 && hen_tail.w < best_hit.w) { best_hit = hen_tail; }
        let h1_y = 0.24 - peck;
        let hen_head = intersect_aabb_normal(lro, rd, vec3<f32>(0.26, h1_y, 0.22) + off1, vec3<f32>(0.44, h1_y + 0.15, 0.35) + off1);
        if (hen_head.w > 0.0 && hen_head.w < best_hit.w) { best_hit = hen_head; }
        let hen_comb = intersect_aabb_normal(lro, rd, vec3<f32>(0.32, h1_y + 0.15, 0.24) + off1, vec3<f32>(0.38, h1_y + 0.20, 0.33) + off1);
        if (hen_comb.w > 0.0 && hen_comb.w < best_hit.w) { best_hit = hen_comb; }

        let hen_legs2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.62, 0.0, 0.58) + off2, vec3<f32>(0.74, 0.08, 0.70) + off2);
        if (hen_legs2.w > 0.0 && hen_legs2.w < best_hit.w) { best_hit = hen_legs2; }
        let hen_body2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.55, 0.08, 0.50) + off2, vec3<f32>(0.80, 0.30, 0.76) + off2);
        if (hen_body2.w > 0.0 && hen_body2.w < best_hit.w) { best_hit = hen_body2; }
        let hen_tail2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.58, 0.20, 0.70) + off2, vec3<f32>(0.76, 0.36, 0.80) + off2);
        if (hen_tail2.w > 0.0 && hen_tail2.w < best_hit.w) { best_hit = hen_tail2; }
        let h2_y = 0.23 - peck2;
        let hen_head2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.59, h2_y, 0.40) + off2, vec3<f32>(0.77, h2_y + 0.14, 0.54) + off2);
        if (hen_head2.w > 0.0 && hen_head2.w < best_hit.w) { best_hit = hen_head2; }
        let hen_comb2 = intersect_aabb_normal(lro, rd, vec3<f32>(0.65, h2_y + 0.14, 0.42) + off2, vec3<f32>(0.71, h2_y + 0.19, 0.51) + off2);
        if (hen_comb2.w > 0.0 && hen_comb2.w < best_hit.w) { best_hit = hen_comb2; }
    } else if (mat == 60u) {
        // MAT_MU_BIOCHAR_KILN: Scenario Mu Smokeless TLUD / Flame-Cap Pyrolysis Biochar Kiln
        let retort = intersect_aabb_normal(lro, rd, vec3<f32>(0.18, 0.0, 0.18), vec3<f32>(0.82, 0.76, 0.82));
        if (retort.w > 0.0 && retort.w < best_hit.w) { best_hit = retort; }
        let collar = intersect_aabb_normal(lro, rd, vec3<f32>(0.32, 0.76, 0.32), vec3<f32>(0.68, 0.98, 0.68));
        if (collar.w > 0.0 && collar.w < best_hit.w) { best_hit = collar; }
    } else if (mat == 61u || mat == 62u) {
        // MAT_MU_COMPOST_DRUM: Scenario Mu Insulated Thermophilic Composting Drum & Struvite Carboy
        let stand = intersect_aabb_normal(lro, rd, vec3<f32>(0.14, 0.0, 0.42), vec3<f32>(0.86, 0.24, 0.68));
        if (stand.w > 0.0 && stand.w < best_hit.w) { best_hit = stand; }
        let drum = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.22, 0.14), vec3<f32>(0.88, 0.78, 0.86));
        if (drum.w > 0.0 && drum.w < best_hit.w) { best_hit = drum; }
        let carboy = intersect_aabb_normal(lro, rd, vec3<f32>(0.14, 0.0, 0.14), vec3<f32>(0.42, 0.48, 0.42));
        if (carboy.w > 0.0 && carboy.w < best_hit.w) { best_hit = carboy; }
    } else if (mat == 70u) {
        // MAT_TOOL_WHEELBARROW: Low-poly Garden Wheelbarrow (front dark-rubber wheel + emerald steel tray tub + wooden push handles & rear legs)
        let wheel = intersect_aabb_normal(lro, rd, vec3<f32>(0.38, 0.0, 0.10), vec3<f32>(0.62, 0.28, 0.34));
        if (wheel.w > 0.0 && wheel.w < best_hit.w) { best_hit = wheel; }
        let tray = intersect_aabb_normal(lro, rd, vec3<f32>(0.20, 0.22, 0.24), vec3<f32>(0.80, 0.56, 0.78));
        if (tray.w > 0.0 && tray.w < best_hit.w) { best_hit = tray; }
        let handles_legs = intersect_aabb_normal(lro, rd, vec3<f32>(0.24, 0.0, 0.72), vec3<f32>(0.76, 0.48, 0.92));
        if (handles_legs.w > 0.0 && handles_legs.w < best_hit.w) { best_hit = handles_legs; }
    } else if (mat == 71u) {
        // MAT_TOOL_BUCKET_CAN: Low-poly Galvanized Steel Bucket & Copper-Spout Watering Can
        let bucket = intersect_aabb_normal(lro, rd, vec3<f32>(0.16, 0.0, 0.24), vec3<f32>(0.52, 0.40, 0.60));
        if (bucket.w > 0.0 && bucket.w < best_hit.w) { best_hit = bucket; }
        let wcan = intersect_aabb_normal(lro, rd, vec3<f32>(0.54, 0.0, 0.30), vec3<f32>(0.86, 0.38, 0.68));
        if (wcan.w > 0.0 && wcan.w < best_hit.w) { best_hit = wcan; }
        let spout = intersect_aabb_normal(lro, rd, vec3<f32>(0.65, 0.18, 0.16), vec3<f32>(0.75, 0.42, 0.30));
        if (spout.w > 0.0 && spout.w < best_hit.w) { best_hit = spout; }
    } else if (mat == 72u) {
        // MAT_TOOL_HAND_DOLLY: Low-poly Heavy-Duty Red Steel Hand Truck / Dolly
        let toe_wheels = intersect_aabb_normal(lro, rd, vec3<f32>(0.22, 0.0, 0.20), vec3<f32>(0.78, 0.16, 0.74));
        if (toe_wheels.w > 0.0 && toe_wheels.w < best_hit.w) { best_hit = toe_wheels; }
        let l_frame = intersect_aabb_normal(lro, rd, vec3<f32>(0.26, 0.16, 0.60), vec3<f32>(0.74, 0.92, 0.76));
        if (l_frame.w > 0.0 && l_frame.w < best_hit.w) { best_hit = l_frame; }
    } else if (mat == 32u) {
        let ladder = intersect_aabb_normal(lro, rd, vec3<f32>(0.0, 0.0, 0.18), vec3<f32>(0.26, 1.0, 0.82));
        if (ladder.w > 0.0 && ladder.w < best_hit.w) { best_hit = ladder; }
    } else if (mat == 35u) {
        let trunk = intersect_aabb_normal(lro, rd, vec3<f32>(0.36, 0.0, 0.36), vec3<f32>(0.64, 1.0, 0.64));
        if (trunk.w > 0.0 && trunk.w < best_hit.w) { best_hit = trunk; }
    } else if (mat == 36u) {
        if (mapPos.y == 2) {
            let tier1 = intersect_octahedron(lro, rd, vec3<f32>(0.5, 0.46, 0.5), vec3<f32>(0.72, 0.76, 0.72));
            if (tier1.w > 0.0 && tier1.w < best_hit.w) { best_hit = tier1; }
            let tier2 = intersect_octahedron(lro, rd, vec3<f32>(0.5, 0.82, 0.5), vec3<f32>(0.58, 0.54, 0.58));
            if (tier2.w > 0.0 && tier2.w < best_hit.w) { best_hit = tier2; }
        } else {
            let crown_top = intersect_octahedron(lro, rd, vec3<f32>(0.5, 0.28, 0.5), vec3<f32>(0.54, 0.68, 0.54));
            if (crown_top.w > 0.0 && crown_top.w < best_hit.w) { best_hit = crown_top; }
        }
    } else if (mat == 37u) {
        let post = intersect_aabb_normal(lro, rd, vec3<f32>(0.44, 0.0, 0.44), vec3<f32>(0.56, 0.62, 0.56));
        if (post.w > 0.0 && post.w < best_hit.w) { best_hit = post; }
        let box_head = intersect_aabb_normal(lro, rd, vec3<f32>(0.30, 0.62, 0.34), vec3<f32>(0.70, 0.86, 0.66));
        if (box_head.w > 0.0 && box_head.w < best_hit.w) { best_hit = box_head; }
    } else if (mat == 38u) {
        let shrub = intersect_octahedron(lro, rd, vec3<f32>(0.5, 0.28, 0.5), vec3<f32>(0.46, 0.38, 0.46));
        if (shrub.w > 0.0 && shrub.w < best_hit.w) { best_hit = shrub; }
    } else if (mat == 120u || mat == 122u) {
        // Office Desk + PC Tower + Dual Low-Poly Monitors + Swivel Office Chair
        let desk = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.42, 0.08), vec3<f32>(0.96, 0.50, 0.70));
        if (desk.w > 0.0 && desk.w < best_hit.w) { best_hit = desk; }
        let pc_legs = intersect_aabb_normal(lro, rd, vec3<f32>(0.06, 0.0, 0.10), vec3<f32>(0.94, 0.42, 0.66));
        if (pc_legs.w > 0.0 && pc_legs.w < best_hit.w) { best_hit = pc_legs; }
        let monitor = intersect_aabb_normal(lro, rd, vec3<f32>(0.14, 0.50, 0.14), vec3<f32>(0.86, 0.88, 0.32));
        if (monitor.w > 0.0 && monitor.w < best_hit.w) { best_hit = monitor; }
        let chair = intersect_aabb_normal(lro, rd, vec3<f32>(0.28, 0.0, 0.66), vec3<f32>(0.72, 0.68, 0.96));
        if (chair.w > 0.0 && chair.w < best_hit.w) { best_hit = chair; }
    }

    if (best_hit.w > 1e8) {
        return vec4<f32>(0.0, 1.0, 0.0, -1.0);
    }
    return best_hit;
}

// Sub-voxel vertical slab height (1.0 = full 1m block or sub-voxel prop; < 0.99 = low flat ground slab)
fn get_voxel_max_local_y(mat: u32) -> f32 {
    if (mat == 18u) { return 0.05; }
    if (mat == 20u || mat == 21u || mat == 22u || mat == 25u) { return 0.06; }
    if (mat == 26u) { return 0.14; }
    if (mat == 33u) { return 0.55; }
    return 1.0;
}

// 16-Step Directional Sun Shadow Ray (respects active map chunk roof cutaway while keeping distant neighbor roofs sealed)
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

    let min_shadow_y = select(-2, min(-2, world_chunk.origin_y), world_chunk.is_loaded != 0u);
    var shadow_atten: f32 = 1.0;
    for (var s = 0; s < 16; s++) {
        if (sSide.x < sSide.y) {
            if (sSide.x < sSide.z) { sSide.x += sDelta.x; sPos.x += sStep.x; }
            else { sSide.z += sDelta.z; sPos.z += sStep.z; }
        } else {
            if (sSide.y < sSide.z) { sSide.y += sDelta.y; sPos.y += sStep.y; }
            else { sSide.z += sDelta.z; sPos.z += sStep.z; }
        }
        let s_cutaway = select(32, z_cutaway, is_playable_chunk_cell(sPos));
        if (sPos.y >= s_cutaway || sPos.y > 16 || sPos.y < min_shadow_y) {
            break;
        }
        let sm = get_voxel_material(sPos);
        if (sm == 35u || sm == 36u || sm == 34u || sm == 58u || sm == 60u || sm == 62u || (sm >= 70u && sm <= 72u)) {
            shadow_atten = min(shadow_atten, 0.52);
        } else if (sm != 0u && sm != 19u && (sm < 26u || sm == 57u || (sm >= 64u && sm <= 68u)) && get_voxel_max_local_y(sm) >= 0.99) {
            if (sm == 7u || sm == 10u) {
                shadow_atten = min(shadow_atten, 0.72);
            } else {
                return 0.36;
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
    var npc_subpart: u32 = 0u; // 0u = voxel, 1u = NPC body/legs, 2u = NPC head sphere

    // Roof / Wall Cutaway level encoded in camera.camera_target.w when in (1.5, 31.5)
    let cutaway_active = camera.camera_target.w > 1.5 && camera.camera_target.w < 31.5;
    let z_cutaway_level = select(32, i32(floor(camera.camera_target.w + 0.5)), cutaway_active);
    let min_voxel_y = select(-2, min(-2, world_chunk.origin_y), world_chunk.is_loaded != 0u);

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

        // Bounding space early exit: volume y in [min_voxel_y, 32], z in [-64, 64], |x| <= 64
        if ((mapPos.y > 32 && dda_step.y > 0) || (mapPos.y < min_voxel_y && dda_step.y < 0) ||
            (mapPos.z > 64 && dda_step.z > 0) || (mapPos.z < -64 && dda_step.z < 0) ||
            (mapPos.x > 64 && dda_step.x > 0) || (mapPos.x < -64 && dda_step.x < 0)) {
            break;
        }

        var mat: u32 = 0u;
        let local_cutaway = select(32, z_cutaway_level, is_playable_chunk_cell(mapPos));
        if (mapPos.y < local_cutaway) {
            mat = get_voxel_material(mapPos);
        }
        if (mat >= 27u && mat <= 29u) {
            // Sub-voxel articulated 3D humanoid intersection for sidewalk/enclave NPCs
            let npc_base = vec3<f32>(f32(mapPos.x) + 0.5, f32(mapPos.y), f32(mapPos.z) + 0.5);
            let npc_stride = sin(camera.avatar_pos.w * 5.0 + f32(mapPos.z)) * 0.05;
            let leg_l_a = npc_base + vec3<f32>(-0.07, 0.04,  npc_stride);
            let leg_l_b = npc_base + vec3<f32>(-0.07, 0.34,  0.0);
            let leg_r_a = npc_base + vec3<f32>( 0.07, 0.04, -npc_stride);
            let leg_r_b = npc_base + vec3<f32>( 0.07, 0.34,  0.0);
            let npc_pa = npc_base + vec3<f32>(0.0, 0.32, 0.0);
            let npc_pb = npc_base + vec3<f32>(0.0, 0.62, 0.0);
            let npc_head_c = npc_base + vec3<f32>(0.0, 0.80, 0.0);

            let t_leg_l = intersect_capsule(ro, rd, leg_l_a, leg_l_b, 0.065);
            let t_leg_r = intersect_capsule(ro, rd, leg_r_a, leg_r_b, 0.065);
            let t_npc_body = intersect_capsule(ro, rd, npc_pa, npc_pb, 0.16);
            let t_npc_head = intersect_sphere(ro, rd, npc_head_c, 0.15);

            var t_npc: f32 = 1e9;
            var part_npc: u32 = 0u;
            var n_npc = vec3<f32>(0.0, 1.0, 0.0);
            if (t_leg_l > 0.0 && t_leg_l < t_npc) {
                t_npc = t_leg_l;
                part_npc = 1u;
                n_npc = capsule_normal(ro + rd * t_leg_l, leg_l_a, leg_l_b);
            }
            if (t_leg_r > 0.0 && t_leg_r < t_npc) {
                t_npc = t_leg_r;
                part_npc = 1u;
                n_npc = capsule_normal(ro + rd * t_leg_r, leg_r_a, leg_r_b);
            }
            if (t_npc_body > 0.0 && t_npc_body < t_npc) {
                t_npc = t_npc_body;
                part_npc = 1u;
                n_npc = capsule_normal(ro + rd * t_npc_body, npc_pa, npc_pb);
            }
            if (t_npc_head > 0.0 && t_npc_head < t_npc) {
                t_npc = t_npc_head;
                part_npc = 2u;
                n_npc = normalize((ro + rd * t_npc_head) - npc_head_c);
            }
            if (part_npc != 0u) {
                hit_dist = t_npc;
                hit_mat = mat;
                npc_subpart = part_npc;
                hit_normal = n_npc;
                break;
            }
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
            } else if (mat == 6u || mat == 7u || mat == 8u || mat == 9u || mat == 10u ||
                       mat == 11u || mat == 12u || mat == 15u || mat == 16u || mat == 17u ||
                       mat == 19u || mat == 23u || mat == 24u || mat == 32u || (mat >= 34u && mat <= 38u) ||
                       (mat >= 40u && mat <= 62u) || (mat >= 70u && mat <= 72u) || mat == 120u || mat == 122u) {
                let sub_hit = intersect_subvoxel_prop(ro, rd, mapPos, mat);
                if (sub_hit.w > 0.0) {
                    hit_dist = sub_hit.w;
                    hit_normal = sub_hit.xyz;
                    hit_mat = mat;
                    break;
                }
            } else {
                hit_mat = mat;
                break;
            }
        }
    }

    let voxel_dist = select(1e9, hit_dist, hit_mat != 0u);

    // Founder Avatar: Articulated Low-Poly 3D Colonist (2 Legs + Torso Tunic + Backpack + 2 Arms + Head/Cap) + Faceted Octahedron Plumbob Crystal
    if (camera.avatar_params.w > 0.5) {
        let is_carrying = camera.avatar_pos.w >= 10.0;
        let plumbob_state = select(camera.avatar_pos.w, camera.avatar_pos.w - 10.0, is_carrying);
        let anim_time = plumbob_state;
        let walk_swing = sin(anim_time * 8.0) * 0.08;

        let leg_l_pa = camera.avatar_pos.xyz + vec3<f32>(-0.10, -0.52,  walk_swing);
        let leg_l_pb = camera.avatar_pos.xyz + vec3<f32>(-0.10, -0.10,  0.0);
        let leg_r_pa = camera.avatar_pos.xyz + vec3<f32>( 0.10, -0.52, -walk_swing);
        let leg_r_pb = camera.avatar_pos.xyz + vec3<f32>( 0.10, -0.10,  0.0);
        let body_pa  = camera.avatar_pos.xyz + vec3<f32>( 0.0,  -0.12,  0.0);
        let body_pb  = camera.avatar_pos.xyz + vec3<f32>( 0.0,   0.34,  0.0);
        let arm_l_pa = camera.avatar_pos.xyz + vec3<f32>(-0.24,  0.28,  0.0);
        let arm_l_pb = camera.avatar_pos.xyz + vec3<f32>(-0.26, -0.04, -walk_swing * 0.8);
        let arm_r_pa = camera.avatar_pos.xyz + vec3<f32>( 0.24,  0.28,  0.0);
        let arm_r_pb = camera.avatar_pos.xyz + vec3<f32>( 0.26, -0.04,  walk_swing * 0.8);
        let pack_min = camera.avatar_pos.xyz + vec3<f32>(-0.15, -0.02, -0.28);
        let pack_max = camera.avatar_pos.xyz + vec3<f32>( 0.15,  0.30, -0.10);

        let head_center = camera.avatar_pos.xyz + vec3<f32>(0.0, 0.56, 0.0);
        let crate_center = camera.avatar_pos.xyz + vec3<f32>(0.24, 0.06, 0.22);
        let plumbob_offset = 1.16 + sin(anim_time * 3.5) * 0.10;
        let plumbob_center = camera.avatar_pos.xyz + vec3<f32>(0.0, plumbob_offset, 0.0);

        let t_leg_l = intersect_capsule(ro, rd, leg_l_pa, leg_l_pb, 0.09);
        let t_leg_r = intersect_capsule(ro, rd, leg_r_pa, leg_r_pb, 0.09);
        let t_body  = intersect_capsule(ro, rd, body_pa, body_pb, 0.21);
        let t_arm_l = intersect_capsule(ro, rd, arm_l_pa, arm_l_pb, 0.07);
        let t_arm_r = intersect_capsule(ro, rd, arm_r_pa, arm_r_pb, 0.07);
        let pack_hit = intersect_aabb_normal(ro, rd, pack_min, pack_max);
        let t_head  = intersect_sphere(ro, rd, head_center, 0.19);
        let crate_hit = intersect_aabb_normal(ro, rd, crate_center - vec3<f32>(0.16, 0.14, 0.16), crate_center + vec3<f32>(0.16, 0.14, 0.16));
        let t_crate = select(-1.0, crate_hit.w, is_carrying);
        let plumbob_hit = intersect_octahedron(ro, rd, plumbob_center, vec3<f32>(0.20, 0.30, 0.20));

        var t_avatar: f32 = 1e9;
        var avatar_part: u32 = 0u;
        var av_norm = vec3<f32>(0.0, 1.0, 0.0);

        if (t_leg_l > 0.0 && t_leg_l < t_avatar) {
            t_avatar = t_leg_l; avatar_part = 1u; av_norm = capsule_normal(ro + rd * t_leg_l, leg_l_pa, leg_l_pb);
        }
        if (t_leg_r > 0.0 && t_leg_r < t_avatar) {
            t_avatar = t_leg_r; avatar_part = 1u; av_norm = capsule_normal(ro + rd * t_leg_r, leg_r_pa, leg_r_pb);
        }
        if (t_body > 0.0 && t_body < t_avatar) {
            t_avatar = t_body; avatar_part = 1u; av_norm = capsule_normal(ro + rd * t_body, body_pa, body_pb);
        }
        if (t_arm_l > 0.0 && t_arm_l < t_avatar) {
            t_avatar = t_arm_l; avatar_part = 1u; av_norm = capsule_normal(ro + rd * t_arm_l, arm_l_pa, arm_l_pb);
        }
        if (t_arm_r > 0.0 && t_arm_r < t_avatar) {
            t_avatar = t_arm_r; avatar_part = 1u; av_norm = capsule_normal(ro + rd * t_arm_r, arm_r_pa, arm_r_pb);
        }
        if (pack_hit.w > 0.0 && pack_hit.w < t_avatar) {
            t_avatar = pack_hit.w; avatar_part = 1u; av_norm = pack_hit.xyz;
        }
        if (t_head > 0.0 && t_head < t_avatar) {
            t_avatar = t_head; avatar_part = 2u; av_norm = normalize((ro + rd * t_head) - head_center);
        }
        if (t_crate > 0.0 && t_crate < t_avatar) {
            t_avatar = t_crate; avatar_part = 4u; av_norm = crate_hit.xyz;
        }
        if (plumbob_hit.w > 0.0 && plumbob_hit.w < t_avatar) {
            t_avatar = plumbob_hit.w; avatar_part = 3u; av_norm = plumbob_hit.xyz;
        }

        if (avatar_part != 0u && t_avatar < voxel_dist) {
            let hit_pos = ro + rd * t_avatar;
            let norm = av_norm;

            let view_dir = -rd;
            let light_dir = normalize(vec3<f32>(0.5, 0.8, -0.4));

            let raw_diff = max(dot(norm, light_dir), 0.0);
            let diff = mix(raw_diff, floor(raw_diff * 4.0 + 0.5) / 4.0, 0.35);
            let view_dot = max(dot(norm, view_dir), 0.0);
            let fresnel = pow(1.0 - view_dot, 2.5);

            let param_state = u32(round(camera.avatar_params.x));
            let pb_state_u32 = u32(round(clamp(plumbob_state, 0.0, 4.0)));
            let action_state = select(param_state, pb_state_u32, is_carrying && plumbob_state <= 4.5);
            var plumbob_base = vec3<f32>(0.00, 0.90, 0.45);
            var plumbob_glow = vec3<f32>(0.35, 1.00, 0.65);

            if (action_state == 1u) {
                plumbob_base = vec3<f32>(0.00, 0.65, 0.95);
                plumbob_glow = vec3<f32>(0.40, 0.85, 1.00);
            } else if (action_state == 2u) {
                plumbob_base = vec3<f32>(0.95, 0.75, 0.05);
                plumbob_glow = vec3<f32>(1.00, 0.90, 0.40);
            } else if (action_state == 4u || camera.avatar_params.y > 0.75) {
                plumbob_base = vec3<f32>(0.95, 0.10, 0.15);
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
                let rel_y = hit_pos.y - camera.avatar_pos.y;
                let rel_z = hit_pos.z - camera.avatar_pos.z;
                let is_belt = abs(rel_y + 0.08) < 0.05;
                let is_legs = rel_y < -0.13;
                let is_pack = rel_z < -0.11 && rel_y > -0.05;
                var workwear_col = vec3<f32>(0.18, 0.56, 0.74);
                if (is_pack) {
                    workwear_col = vec3<f32>(0.52, 0.44, 0.26);
                } else if (is_belt) {
                    workwear_col = vec3<f32>(0.88, 0.60, 0.18);
                } else if (is_legs) {
                    workwear_col = vec3<f32>(0.18, 0.24, 0.34);
                }
                lit_avatar = workwear_col * (0.40 + 0.60 * diff) + vec3<f32>(0.95, 0.96, 1.0) * (spec_av * 0.22) + plumbob_glow * (fresnel * 0.22);
            } else if (avatar_part == 2u) {
                let rel_head = hit_pos - head_center;
                let is_hair = rel_head.y > 0.05 || (rel_head.z < -0.04 && rel_head.y > -0.04);
                let head_col = select(vec3<f32>(0.94, 0.80, 0.66), vec3<f32>(0.22, 0.15, 0.11), is_hair);
                lit_avatar = head_col * (0.44 + 0.56 * diff) + vec3<f32>(0.95, 0.90, 0.80) * (fresnel * 0.20 + spec_av * 0.16);
            } else if (avatar_part == 4u) {
                let rel_crate = hit_pos - crate_center;
                let strap_band = abs(rel_crate.x) < 0.035 || abs(rel_crate.z) < 0.035 || abs(rel_crate.y) < 0.028;
                let crate_col = select(vec3<f32>(0.86, 0.60, 0.24), vec3<f32>(0.24, 0.18, 0.12), strap_band);
                lit_avatar = crate_col * (0.40 + 0.60 * diff) + vec3<f32>(1.0, 0.92, 0.72) * (spec_av * 0.32);
            } else {
                let pulse = 0.85 + 0.15 * sin(anim_time * 5.0);
                let facet_sparkle = pow(max(dot(norm, half_v), 0.0), 48.0);
                lit_avatar = plumbob_base * (0.32 + 0.68 * raw_diff) + plumbob_glow * pulse * (fresnel * 0.75 + 0.25 + facet_sparkle * 0.65);
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
    let cell_origin = vec3<f32>(f32(mapPos.x), f32(mapPos.y), f32(mapPos.z));
    let hit_frac = clamp(hit_pos - cell_origin, vec3<f32>(0.0), vec3<f32>(1.0));
    let lotZ_hit = (mapPos.z % 16 + 16) % 16;
    let absX_hit = abs(mapPos.x);
    let in_playable_chunk = is_playable_chunk_cell(mapPos);

    // Stylized Low-Poly Diorama Materials (albedo, roughness, metallic, emissive)
    var base_color: vec3<f32> = vec3<f32>(0.5);
    var roughness: f32 = 0.65;
    var metallic: f32 = 0.04;
    var emissive: vec3<f32> = vec3<f32>(0.0);

    if (hit_mat == 1u) {
        let agg = sin(hit_pos.x * 18.0) * cos(hit_pos.z * 18.0) * 0.012;
        base_color = vec3<f32>(0.20, 0.22, 0.26) + vec3<f32>(agg);
        roughness = 0.76;
    } else if (hit_mat == 2u) {
        base_color = vec3<f32>(0.96, 0.76, 0.18);
        roughness = 0.35;
    } else if (hit_mat == 3u) {
        let joint = select(0.0, 1.0, hit_frac.x < 0.035 || hit_frac.z < 0.035);
        base_color = mix(vec3<f32>(0.78, 0.77, 0.74), vec3<f32>(0.58, 0.57, 0.54), joint * 0.60);
        roughness = 0.60;
    } else if (hit_mat == 4u) {
        // Material 4: Data-Driven Context-Aware Ground (Workshop Slab / Interior Tile & Oak Floors / Planter Soil / Poultry Litter / Lawn Turf)
        let above_1 = get_voxel_material(mapPos + vec3<i32>(0, 1, 0));
        let n_nx = get_voxel_material(mapPos + vec3<i32>(-1, 1, 0));
        let n_px = get_voxel_material(mapPos + vec3<i32>( 1, 1, 0));
        let n_nz = get_voxel_material(mapPos + vec3<i32>( 0, 1, -1));
        let n_pz = get_voxel_material(mapPos + vec3<i32>( 0, 1,  1));
        let has_roof_above = is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>(0, 4, 0))) ||
                             is_roof_or_wall_neighbor(get_voxel_material(mapPos + vec3<i32>(0, 3, 0)));
        let is_interior_slab = has_roof_above || is_wall_or_opening_neighbor(above_1) ||
                               (above_1 >= 40u && above_1 <= 56u) || above_1 == 23u || above_1 == 120u;

        if (above_1 == 19u || above_1 == 34u) {
            // Cultivated garden bed dark biochar-loam soil base
            base_color = vec3<f32>(0.26, 0.19, 0.13);
            roughness = 0.74;
        } else if (above_1 == 58u || above_1 == 59u || n_nz == 58u || n_pz == 58u) {
            // Poultry Run: Deep-Litter Biochar & Golden Straw Pasture Bedding
            let straw_tex = sin(hit_pos.x * 16.0 + cos(hit_pos.z * 14.0) * 2.5) * 0.04;
            let biochar_fleck = step(0.86, sin(hit_pos.x * 24.0) * cos(hit_pos.z * 24.0));
            base_color = mix(vec3<f32>(0.62, 0.54, 0.32) + vec3<f32>(straw_tex), vec3<f32>(0.24, 0.20, 0.16), biochar_fleck * 0.45);
            roughness = 0.74;
        } else if (is_interior_slab) {
            let n_nz2 = get_voxel_material(mapPos + vec3<i32>(0, 1, -2));
            let n_pz2 = get_voxel_material(mapPos + vec3<i32>(0, 1,  2));
            let n_px_nz = get_voxel_material(mapPos + vec3<i32>(1, 1, -1));
            let is_workshop = (above_1 >= 54u && above_1 <= 57u) || (above_1 >= 70u && above_1 <= 72u) ||
                              (n_pz >= 54u && n_pz <= 57u) || (n_nx == 57u) ||
                              ((n_pz2 >= 54u && n_pz2 <= 56u) && !is_wall_or_opening_neighbor(n_pz));
            let is_kitchen = (above_1 == 24u || (above_1 >= 40u && above_1 <= 43u)) ||
                             (n_nz == 24u || (n_nz >= 40u && n_nz <= 42u)) ||
                             (n_nx == 43u) ||
                             ((n_nz2 == 24u || (n_nz2 >= 40u && n_nz2 <= 42u)) && n_nz == 0u);
            let is_bath = (above_1 >= 45u && above_1 <= 47u) ||
                          (n_nz >= 45u && n_nz <= 47u) ||
                          (n_px >= 45u && n_px <= 47u) ||
                          (n_px_nz >= 45u && n_px_nz <= 47u);

            if (is_workshop) {
                // Workshop / Attached Garage: Smooth Troweled Epoxy-Grey Concrete Slab
                let slab_joint = select(0.0, 1.0, fract(hit_pos.x * 0.5) < 0.025 || fract(hit_pos.z * 0.5) < 0.025);
                base_color = mix(vec3<f32>(0.54, 0.57, 0.60), vec3<f32>(0.38, 0.40, 0.43), slab_joint * 0.55);
                roughness = 0.32;
                metallic = 0.10;
            } else if (is_kitchen) {
                // Kitchen & Hearth: Warm Terracotta & Cream Checkerboard Ceramic Quarry Tile
                let grout = select(0.0, 1.0, fract(hit_pos.x * 2.0) < 0.07 || fract(hit_pos.z * 2.0) < 0.07);
                let chk = (i32(floor(hit_pos.x * 2.0)) + i32(floor(hit_pos.z * 2.0))) & 1;
                let tile_c = select(vec3<f32>(0.88, 0.78, 0.66), vec3<f32>(0.76, 0.44, 0.30), chk == 0);
                base_color = mix(tile_c, vec3<f32>(0.48, 0.42, 0.36), grout * 0.65);
                roughness = 0.20;
                metallic = 0.08;
            } else if (is_bath) {
                // Bathroom & Sanitation: Clean Aqua-White Waterproof Porcelain Tile
                let grout = select(0.0, 1.0, fract(hit_pos.x * 3.0) < 0.08 || fract(hit_pos.z * 3.0) < 0.08);
                let b_chk = (i32(floor(hit_pos.x * 3.0)) + i32(floor(hit_pos.z * 3.0))) & 1;
                let aqua_tile = select(vec3<f32>(0.88, 0.95, 0.96), vec3<f32>(0.68, 0.88, 0.90), b_chk == 0);
                base_color = mix(aqua_tile, vec3<f32>(0.56, 0.68, 0.72), grout * 0.55);
                roughness = 0.14;
                metallic = 0.12;
            } else {
                // Warm Honey-Oak Hardwood Planks (Study, Circulation Hallways, Living Space & Bedroom)
                let plank_seam = select(0.0, 1.0, fract(hit_pos.x * 3.0) < 0.06 || fract(hit_pos.z * 0.75) < 0.04);
                let grain = sin(hit_pos.z * 14.0 + floor(hit_pos.x * 3.0) * 2.3) * 0.025;
                var oak_col = mix(vec3<f32>(0.68, 0.48, 0.28) + vec3<f32>(grain), vec3<f32>(0.44, 0.29, 0.16), plank_seam * 0.60);
                roughness = 0.28;
                metallic = 0.06;
                if (above_1 == 48u || n_nz == 48u) {
                    let rug_border = hit_frac.x < 0.12 || hit_frac.x > 0.88 || hit_frac.z < 0.12 || hit_frac.z > 0.88;
                    oak_col = select(vec3<f32>(0.76, 0.36, 0.24), vec3<f32>(0.90, 0.78, 0.56), rug_border);
                    roughness = 0.68;
                } else if (above_1 == 23u || n_pz == 23u) {
                    let b_border = hit_frac.x < 0.12 || hit_frac.x > 0.88 || hit_frac.z < 0.12 || hit_frac.z > 0.88;
                    oak_col = select(vec3<f32>(0.34, 0.48, 0.64), vec3<f32>(0.84, 0.86, 0.90), b_border);
                    roughness = 0.68;
                }
                base_color = oak_col;
            }
        } else {
            // Exterior Lawn Turf over Sandy-Loam Topsoil Horizon
            let checker = f32((mapPos.x + mapPos.z) & 1) * 0.022;
            let stripe = sin(hit_pos.x * 1.5708) * 0.018;
            let lawn_var = sin(hit_pos.x * 4.5) * cos(hit_pos.z * 4.5) * 0.022;
            base_color = vec3<f32>(0.34, 0.62, 0.27) + vec3<f32>(checker + stripe + lawn_var);
            roughness = 0.70;
        }
    } else if (hit_mat == 5u || hit_mat == 30u) {
        if (hit_mat == 30u) {
            // Compact Home-Office Router / NAS Unit
            base_color = vec3<f32>(0.22, 0.26, 0.32);
            roughness = 0.28;
            metallic = 0.65;
        } else {
            // Warm Cream/Ivory Architectural Clapboard Siding with Data-Driven Corner Trim & Baseboard Plinth
            let clapboard_rib = fract(hit_pos.y * 4.0);
            let rib_shadow = smoothstep(0.0, 0.18, clapboard_rib);
            let wx_n = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>(-1, 0, 0)));
            let wx_p = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>( 1, 0, 0)));
            let wz_n = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>( 0, 0, -1)));
            let wz_p = is_wall_or_opening_neighbor(get_voxel_material(mapPos + vec3<i32>( 0, 0,  1)));
            let is_corner_trim = !(wx_n && wx_p) && !(wz_n && wz_p) && ((wx_n || wx_p) && (wz_n || wz_p));
            let is_plinth = mapPos.y == 1 && hit_frac.y < 0.18;
            let wall_cream = select(vec3<f32>(0.92, 0.89, 0.83), vec3<f32>(0.86, 0.89, 0.84), !in_playable_chunk && mapPos.x < 0);
            var siding_base = select(wall_cream * (0.90 + 0.10 * rib_shadow), vec3<f32>(0.97, 0.96, 0.93), is_corner_trim);
            if (is_plinth && !is_corner_trim) {
                siding_base = vec3<f32>(0.62, 0.59, 0.55);
            }
            base_color = siding_base;
            roughness = 0.48;
        }
    } else if (hit_mat == 6u) {
        let course_y = fract(hit_pos.y * 3.0);
        let shingle_line = select(1.0, 0.90, course_y < 0.10);
        let roof_palette = select(vec3<f32>(0.72, 0.34, 0.24), vec3<f32>(0.28, 0.36, 0.48), in_playable_chunk);
        base_color = roof_palette * shingle_line;
        roughness = 0.50;
        metallic = 0.08;
    } else if (hit_mat == 7u) {
        let is_sill_or_frame = hit_frac.y < 0.15 || hit_frac.y > 0.86 || hit_frac.x < 0.10 || hit_frac.x > 0.90 || hit_frac.z < 0.10 || hit_frac.z > 0.90 || abs(hit_frac.x - 0.5) < 0.04 || abs(hit_frac.z - 0.5) < 0.04;
        let pane_col = select(vec3<f32>(0.72, 0.58, 0.38), vec3<f32>(0.24, 0.46, 0.64), in_playable_chunk);
        base_color = select(pane_col, vec3<f32>(0.95, 0.94, 0.91), is_sill_or_frame);
        roughness = select(0.14, 0.42, is_sill_or_frame);
        metallic = select(0.75, 0.05, is_sill_or_frame);
        if (!is_sill_or_frame) {
            emissive = select(vec3<f32>(0.34, 0.24, 0.11), vec3<f32>(0.22, 0.16, 0.08), in_playable_chunk);
        }
    } else if (hit_mat == 8u) {
        base_color = vec3<f32>(0.44, 0.28, 0.18);
        roughness = 0.68;
    } else if (hit_mat == 9u) {
        base_color = vec3<f32>(0.68, 0.72, 0.75);
        roughness = 0.30;
        metallic = 0.80;
    } else if (hit_mat == 10u) {
        let post_cap = hit_frac.y > 0.84;
        base_color = select(vec3<f32>(0.64, 0.43, 0.24), vec3<f32>(0.72, 0.50, 0.30), post_cap);
        roughness = 0.58;
    } else if (hit_mat == 11u) {
        let is_rack = hit_frac.y < 0.18;
        let pv_grid = select(0.0, 1.0, fract(hit_pos.x * 4.0) < 0.07 || fract(hit_pos.z * 4.0) < 0.07);
        let pv_cell = mix(vec3<f32>(0.10, 0.26, 0.66), vec3<f32>(0.60, 0.82, 0.98), pv_grid * 0.45);
        base_color = select(pv_cell, vec3<f32>(0.78, 0.82, 0.86), is_rack);
        roughness = select(0.12, 0.35, is_rack);
        metallic = select(0.88, 0.75, is_rack);
    } else if (hit_mat == 12u) {
        let rib = select(1.0, 0.88, fract(hit_pos.y * 4.0) < 0.12);
        base_color = vec3<f32>(0.14, 0.74, 0.88) * rib;
        roughness = 0.20;
        metallic = 0.28;
    } else if (hit_mat == 13u) {
        base_color = vec3<f32>(0.74, 0.56, 0.40);
        roughness = 0.68;
    } else if (hit_mat == 14u) {
        base_color = vec3<f32>(0.54, 0.57, 0.60);
        roughness = 0.62;
    } else if (hit_mat == 15u) {
        let is_jamb = hit_frac.z < 0.11 || hit_frac.z > 0.89 || hit_frac.x < 0.11 || hit_frac.x > 0.89;
        let is_handle = abs(hit_frac.y - 0.48) < 0.05 && (abs(hit_frac.z - 0.78) < 0.06 || abs(hit_frac.x - 0.78) < 0.06);
        base_color = select(vec3<f32>(0.64, 0.39, 0.19), vec3<f32>(0.94, 0.93, 0.90), is_jamb);
        if (is_handle) {
            base_color = vec3<f32>(0.92, 0.76, 0.24);
            metallic = 0.85;
        }
        roughness = 0.40;
    } else if (hit_mat == 16u) {
        let ember_pulse = 0.88 + 0.12 * sin(camera.avatar_pos.w * 4.5);
        let is_riser = hit_frac.y > 0.53;
        base_color = select(vec3<f32>(0.86, 0.32, 0.14) * ember_pulse, vec3<f32>(0.26, 0.24, 0.23), is_riser);
        roughness = select(0.45, 0.32, is_riser);
        metallic = select(0.10, 0.70, is_riser);
        emissive = select(vec3<f32>(0.42, 0.12, 0.02) * ember_pulse, vec3<f32>(0.06, 0.02, 0.0), is_riser);
    } else if (hit_mat == 17u) {
        base_color = vec3<f32>(0.48, 0.34, 0.24);
        roughness = 0.52;
    } else if (hit_mat == 18u) {
        let in_trench = abs(hit_frac.x - 0.5) < 0.36 && abs(hit_frac.z - 0.5) < 0.36;
        let in_water = abs(hit_frac.x - 0.5) < 0.24 && abs(hit_frac.z - 0.5) < 0.24;
        let swale_shimmer = 0.90 + 0.10 * sin(camera.avatar_pos.w * 3.5 + hit_pos.x * 4.0 + hit_pos.z * 4.0);
        if (in_water) {
            base_color = vec3<f32>(0.12, 0.48, 0.64) * swale_shimmer;
            roughness = 0.12;
            metallic = 0.45;
            emissive = vec3<f32>(0.02, 0.09, 0.14) * swale_shimmer;
        } else if (in_trench) {
            base_color = vec3<f32>(0.22, 0.17, 0.12);
            roughness = 0.72;
        } else {
            base_color = vec3<f32>(0.36, 0.28, 0.19);
            roughness = 0.68;
        }
    } else if (hit_mat == 19u) {
        // MAT_COVER_CROP: Raised Cedar Planter Bed (y in [0.0..0.24]) + Dark Biochar-Loam + Sprouting Cover Crop
        let is_cedar_rim = hit_frac.y < 0.235 && (hit_frac.x < 0.12 || hit_frac.x > 0.88 || hit_frac.z < 0.12 || hit_frac.z > 0.88);
        if (is_cedar_rim) {
            let plank_seam = abs(hit_frac.y - 0.12) < 0.018;
            base_color = select(vec3<f32>(0.68, 0.42, 0.22), vec3<f32>(0.46, 0.28, 0.14), plank_seam);
            roughness = 0.50;
        } else {
            let furrow = sin(hit_pos.z * 18.85);
            let is_foliage = hit_frac.y >= 0.235 && furrow > -0.15;
            base_color = select(vec3<f32>(0.22, 0.16, 0.11), vec3<f32>(0.32, 0.66, 0.24) * (0.90 + 0.10 * furrow), is_foliage);
            roughness = select(0.76, 0.50, is_foliage);
        }
    } else if (hit_mat == 20u) {
        let haul_border = select(0.0, 1.0, hit_frac.x < 0.10 || hit_frac.x > 0.90 || hit_frac.z < 0.10 || hit_frac.z > 0.90);
        base_color = mix(vec3<f32>(0.08, 0.56, 0.66), vec3<f32>(0.00, 0.76, 0.86), 0.45 + 0.55 * haul_border);
        roughness = 0.40;
    } else if (hit_mat == 21u) {
        base_color = vec3<f32>(0.38, 0.48, 0.82);
        roughness = 0.55;
    } else if (hit_mat == 22u) {
        let camp_pulse = 0.88 + 0.12 * sin(camera.avatar_pos.w * 5.5);
        base_color = vec3<f32>(1.00, 0.60, 0.08) * camp_pulse;
        roughness = 0.40;
        emissive = vec3<f32>(0.42, 0.20, 0.0) * camp_pulse;
    } else if (hit_mat == 23u) {
        // Room 5 — Bedroom Bed + Mattress + Pillows + Indigo Duvet
        let is_headboard = hit_frac.z > 0.75 && hit_frac.y > 0.44;
        let is_pillow = hit_frac.z >= 0.50 && hit_frac.z <= 0.75 && hit_frac.y > 0.44;
        let is_frame = hit_frac.y < 0.16;
        if (is_headboard || is_frame) {
            base_color = vec3<f32>(0.56, 0.37, 0.20);
        } else if (is_pillow) {
            base_color = vec3<f32>(0.96, 0.94, 0.88);
        } else {
            base_color = vec3<f32>(0.26, 0.44, 0.72);
        }
        roughness = 0.48;
    } else if (hit_mat == 24u || hit_mat == 41u) {
        // Room 1 — MAT_OVEN_STOVE: Brushed Stainless Stove + 4 Glowing Burners + Simmering Communal Pea Soup & Bone Broth Stockpot
        let is_pot = hit_frac.y > 0.825;
        let is_cooktop = hit_frac.y >= 0.78 && !is_pot;
        let is_oven_glass = hit_frac.z > 0.85 && hit_frac.y > 0.18 && hit_frac.y < 0.64 && hit_frac.x > 0.16 && hit_frac.x < 0.84;
        if (is_pot) {
            let pot_top = hit_frac.y > 0.96 || (hit_normal.y > 0.5 && hit_frac.y > 0.92);
            let in_soup = pot_top && hit_frac.x > 0.26 && hit_frac.x < 0.66 && hit_frac.z > 0.28 && hit_frac.z < 0.68;
            if (in_soup) {
                let simmer_swirl = 0.5 + 0.5 * sin(camera.avatar_pos.w * 5.0 + hit_pos.x * 18.0 + hit_pos.z * 16.0);
                let soup_col = mix(vec3<f32>(0.34, 0.76, 0.24), vec3<f32>(0.96, 0.76, 0.20), simmer_swirl * 0.65);
                base_color = soup_col;
                roughness = 0.14;
                metallic = 0.10;
                emissive = soup_col * 0.38;
            } else {
                let is_stainless_rim = hit_frac.y > 0.93;
                base_color = select(vec3<f32>(0.86, 0.52, 0.30), vec3<f32>(0.84, 0.88, 0.92), is_stainless_rim);
                roughness = 0.18;
                metallic = 0.88;
            }
        } else if (is_cooktop) {
            let b_dist = length(fract(hit_frac.xz * 2.0) - vec2<f32>(0.5));
            let burner_ring = abs(b_dist - 0.26) < 0.055;
            base_color = select(vec3<f32>(0.14, 0.16, 0.18), vec3<f32>(1.0, 0.36, 0.10), burner_ring);
            roughness = 0.18;
            if (burner_ring) {
                emissive = vec3<f32>(0.85, 0.24, 0.05);
            }
        } else if (is_oven_glass) {
            base_color = vec3<f32>(0.24, 0.16, 0.10);
            roughness = 0.12;
            emissive = vec3<f32>(0.36, 0.16, 0.04);
        } else {
            base_color = vec3<f32>(0.76, 0.79, 0.82);
            roughness = 0.26;
            metallic = 0.68;
        }
    } else if (hit_mat == 40u) {
        // Room 1 — MAT_FRIDGE: Tall Brushed Mint/Stainless Refrigerator with Freezer Seam & Vertical Handle
        let is_seam = abs(hit_frac.y - 0.62) < 0.018;
        let is_handle = hit_frac.z > 0.85;
        base_color = select(vec3<f32>(0.78, 0.88, 0.85), vec3<f32>(0.90, 0.92, 0.95), is_handle);
        if (is_seam) {
            base_color = vec3<f32>(0.36, 0.42, 0.44);
        }
        roughness = 0.22;
        metallic = select(0.55, 0.88, is_handle);
    } else if (hit_mat == 42u) {
        // Room 1 — MAT_KITCHEN_CABINET: Honey-Oak Cabinet + Butcher-Block Counter + Recessed Stainless Sink & Faucet
        let is_faucet = hit_frac.y > 0.825;
        let is_counter = hit_frac.y >= 0.74 && !is_faucet;
        let in_sink = is_counter && abs(hit_frac.x - 0.50) < 0.28 && abs(hit_frac.z - 0.48) < 0.24;
        if (is_faucet) {
            base_color = vec3<f32>(0.86, 0.88, 0.92);
            roughness = 0.15;
            metallic = 0.90;
        } else if (in_sink) {
            base_color = vec3<f32>(0.54, 0.76, 0.84);
            roughness = 0.16;
            metallic = 0.75;
        } else if (is_counter) {
            base_color = vec3<f32>(0.82, 0.66, 0.46);
            roughness = 0.30;
        } else {
            let door_seam = abs(hit_frac.x - 0.5) < 0.025 || hit_frac.y < 0.10;
            base_color = select(vec3<f32>(0.62, 0.42, 0.24), vec3<f32>(0.42, 0.28, 0.15), door_seam);
            roughness = 0.42;
        }
    } else if (hit_mat == 43u) {
        // Room 1 — MAT_DINING_TABLE_CHAIR: Warm Walnut Dining Table + Chair + Ceramic Dinner Plate
        let is_plate = hit_frac.y > 0.52 && hit_frac.z > 0.34;
        base_color = select(vec3<f32>(0.64, 0.43, 0.25), vec3<f32>(0.96, 0.95, 0.90), is_plate);
        roughness = select(0.36, 0.16, is_plate);
    } else if (hit_mat == 44u) {
        // Room 2 — MAT_OFFICE_STORAGE: Wooden Bookshelf with Colorful Low-Poly Book Spines
        let shelf_frame = fract(hit_pos.y * 3.5) < 0.18 || hit_frac.x < 0.12 || hit_frac.x > 0.88 || hit_frac.z < 0.16;
        let book_idx = floor(hit_pos.x * 9.0 + floor(hit_pos.y * 3.5) * 3.0);
        let book_hue = fract(book_idx * 0.29);
        var book_col = vec3<f32>(0.82, 0.32, 0.26);
        if (book_hue > 0.66) { book_col = vec3<f32>(0.22, 0.58, 0.74); }
        else if (book_hue > 0.33) { book_col = vec3<f32>(0.86, 0.70, 0.24); }
        base_color = select(book_col, vec3<f32>(0.52, 0.34, 0.20), shelf_frame);
        roughness = 0.42;
    } else if (hit_mat == 45u) {
        // Room 3 — MAT_SHOWER: White Ceramic Shower Tray + Frosted Aqua Glass + Chrome Showerhead
        let is_tray = hit_frac.y < 0.15;
        let is_chrome = hit_frac.y > 0.86 || abs(hit_frac.x - 0.10) < 0.03 || abs(hit_frac.z - 0.10) < 0.03;
        if (is_tray) {
            base_color = vec3<f32>(0.94, 0.96, 0.98);
            roughness = 0.18;
        } else if (is_chrome) {
            base_color = vec3<f32>(0.84, 0.88, 0.92);
            roughness = 0.14;
            metallic = 0.88;
        } else {
            base_color = vec3<f32>(0.68, 0.90, 0.95);
            roughness = 0.10;
            metallic = 0.45;
        }
    } else if (hit_mat == 46u) {
        // Room 3 — MAT_TOILET: Sculpted White Vitreous China Toilet + Chrome Flush Button
        let is_flush = hit_frac.y > 0.77 && hit_frac.x > 0.64;
        base_color = select(vec3<f32>(0.95, 0.96, 0.98), vec3<f32>(0.82, 0.86, 0.90), is_flush);
        roughness = 0.12;
        metallic = select(0.10, 0.88, is_flush);
    } else if (hit_mat == 47u) {
        // Room 3 — MAT_BATH_SINK_CABINET: Wooden Vanity + White Ceramic Basin + Reflective Wall Mirror
        if (hit_frac.y > 0.69) {
            base_color = vec3<f32>(0.78, 0.90, 0.96);
            roughness = 0.05;
            metallic = 0.92;
        } else if (hit_frac.y > 0.55) {
            base_color = vec3<f32>(0.94, 0.96, 0.98);
            roughness = 0.14;
        } else {
            base_color = vec3<f32>(0.58, 0.39, 0.22);
            roughness = 0.40;
        }
    } else if (hit_mat == 48u) {
        // Room 4 — MAT_COUCH: Cozy Terracotta/Amber Upholstered Living Room Sofa
        let tuft = select(1.0, 0.88, abs(hit_frac.x - 0.5) < 0.03 || abs(hit_frac.y - 0.38) < 0.025);
        base_color = select(vec3<f32>(0.78, 0.42, 0.28) * tuft, vec3<f32>(0.44, 0.28, 0.16), hit_frac.y < 0.08);
        roughness = 0.62;
    } else if (hit_mat == 49u) {
        // Room 4 — MAT_TV_MEDIA_SET: Oak Media Console + Widescreen TV + VideoGame Console
        if (hit_frac.y >= 0.38 && hit_frac.z >= 0.54) {
            let in_screen = hit_frac.x > 0.13 && hit_frac.x < 0.87 && hit_frac.y > 0.43 && hit_frac.y < 0.84 && hit_frac.z < 0.62;
            if (in_screen) {
                let game_sky = mix(vec3<f32>(0.16, 0.78, 0.96), vec3<f32>(0.22, 0.88, 0.48), step(hit_frac.y, 0.58));
                base_color = game_sky;
                roughness = 0.10;
                emissive = game_sky * 0.75;
            } else {
                base_color = vec3<f32>(0.12, 0.14, 0.17);
                roughness = 0.22;
            }
        } else if (hit_frac.y >= 0.36 && hit_frac.x > 0.60) {
            // Compact VideoGame Console with glowing neon LED strip
            let console_led = abs(hit_frac.y - 0.41) < 0.02;
            base_color = select(vec3<f32>(0.90, 0.92, 0.95), vec3<f32>(0.0, 0.96, 0.65), console_led);
            if (console_led) {
                emissive = vec3<f32>(0.0, 0.85, 0.55);
            }
            roughness = 0.20;
        } else {
            base_color = vec3<f32>(0.60, 0.40, 0.22);
            roughness = 0.36;
        }
    } else if (hit_mat == 50u) {
        // Room 4 — MAT_SOUNDSYSTEM: Acoustic Walnut Tower Speakers + Dual Silver Cones
        let front_face = hit_frac.z < 0.42;
        let cone1 = length(vec2<f32>(hit_frac.x - 0.5, hit_frac.y - 0.30)) < 0.15;
        let cone2 = length(vec2<f32>(hit_frac.x - 0.5, hit_frac.y - 0.58)) < 0.10;
        if (front_face && (cone1 || cone2)) {
            base_color = vec3<f32>(0.84, 0.86, 0.90);
            roughness = 0.18;
            metallic = 0.85;
        } else if (front_face) {
            base_color = vec3<f32>(0.16, 0.18, 0.22);
            roughness = 0.55;
        } else {
            base_color = vec3<f32>(0.50, 0.32, 0.18);
            roughness = 0.38;
        }
    } else if (hit_mat == 51u) {
        // Room 5 — MAT_NIGHTSTAND: Wooden Bedside Table + Warm Glowing Lamp
        let is_lamp = hit_frac.y > 0.54;
        base_color = select(vec3<f32>(0.56, 0.37, 0.20), vec3<f32>(0.98, 0.88, 0.64), is_lamp);
        roughness = select(0.40, 0.25, is_lamp);
        if (is_lamp) {
            emissive = vec3<f32>(0.78, 0.54, 0.20);
        }
    } else if (hit_mat == 52u) {
        // Room 5 — MAT_WARDROBE: Tall Walnut Wardrobe Closet & Dresser Drawers
        let seam = abs(hit_frac.x - 0.5) < 0.025 || abs(hit_frac.y - 0.32) < 0.02;
        let handle = abs(hit_frac.x - 0.5) < 0.08 && abs(hit_frac.y - 0.58) < 0.04;
        base_color = select(vec3<f32>(0.50, 0.32, 0.18), vec3<f32>(0.34, 0.21, 0.11), seam);
        if (handle) {
            base_color = vec3<f32>(0.90, 0.74, 0.26);
            metallic = 0.85;
        }
        roughness = 0.38;
    } else if (hit_mat == 53u) {
        // Room 5 — MAT_MIRROR: Full-Length Standing Mirror in Oak Frame
        let is_frame = hit_frac.x < 0.24 || hit_frac.x > 0.76 || hit_frac.y < 0.08 || hit_frac.y > 0.84;
        base_color = select(vec3<f32>(0.76, 0.88, 0.96), vec3<f32>(0.58, 0.38, 0.20), is_frame);
        roughness = select(0.04, 0.40, is_frame);
        metallic = select(0.95, 0.06, is_frame);
    } else if (hit_mat == 54u) {
        // Room 6 — MAT_GARAGE_WORKBENCH: Steel Frame + Butcher-Block Top + Red Machinist Vise + Brass/Copper Tool Caddy
        if (hit_frac.y > 0.765) {
            let is_tool_caddy = hit_frac.x > 0.55;
            base_color = select(vec3<f32>(0.86, 0.20, 0.18), vec3<f32>(0.88, 0.64, 0.24), is_tool_caddy);
            roughness = 0.24;
            metallic = select(0.68, 0.82, is_tool_caddy);
        } else if (hit_frac.y > 0.64) {
            base_color = vec3<f32>(0.80, 0.62, 0.38);
            roughness = 0.34;
        } else {
            base_color = vec3<f32>(0.24, 0.27, 0.30);
            roughness = 0.36;
            metallic = 0.72;
        }
    } else if (hit_mat == 55u) {
        // Room 6 — MAT_TOOL_WALL: Workbench + Pegboard Tool Wall with Metallic Tool Silhouettes
        if (hit_frac.y > 0.74) {
            let tool_sil = step(0.55, sin(hit_pos.x * 20.0)) * step(0.25, cos(hit_pos.y * 16.0));
            base_color = select(vec3<f32>(0.36, 0.30, 0.24), vec3<f32>(0.84, 0.88, 0.92), tool_sil > 0.5);
            roughness = select(0.52, 0.18, tool_sil > 0.5);
            metallic = select(0.10, 0.88, tool_sil > 0.5);
        } else {
            base_color = select(vec3<f32>(0.24, 0.27, 0.30), vec3<f32>(0.80, 0.62, 0.38), hit_frac.y > 0.62);
            roughness = 0.36;
        }
    } else if (hit_mat == 56u) {
        // Room 6 — MAT_GARAGE_STORAGE: Heavy-Duty Shelving Rack + Colored Organizer Bins
        let rack_frame = fract(hit_pos.y * 3.0) < 0.16 || hit_frac.x < 0.14 || hit_frac.x > 0.86;
        let bin_col = select(vec3<f32>(0.20, 0.56, 0.86), vec3<f32>(0.92, 0.68, 0.16), hit_frac.x > 0.5);
        base_color = select(bin_col, vec3<f32>(0.26, 0.29, 0.33), rack_frame);
        roughness = 0.34;
        metallic = select(0.15, 0.78, rack_frame);
    } else if (hit_mat == 57u) {
        // Room 6 — MAT_GARAGE_DOOR: Ribbed White/Slate Sectional Roll-Up Garage Door
        let panel_groove = fract(hit_pos.y * 4.0) < 0.12 || fract(hit_pos.z * 2.0) < 0.06;
        let is_lite = mapPos.y == 2 && hit_frac.y > 0.55 && fract(hit_pos.z * 2.0) > 0.18 && fract(hit_pos.z * 2.0) < 0.82;
        base_color = select(vec3<f32>(0.92, 0.93, 0.95), vec3<f32>(0.72, 0.75, 0.79), panel_groove);
        if (is_lite) {
            base_color = vec3<f32>(0.34, 0.52, 0.68);
        }
        roughness = select(0.32, 0.12, is_lite);
        metallic = 0.25;
    } else if (hit_mat == 58u) {
        // MAT_POULTRY_COOP: Elevated Timber Chicken Coop & BSFL Bioconversion Feeder + Waterer (data-driven orientation)
        let is_feeder_half = (get_voxel_material(mapPos + vec3<i32>(0, 0, -1)) == 58u);
        if (!is_feeder_half) {
            if (hit_frac.y > 0.735) {
                let roof_rib = fract(hit_pos.z * 6.0) < 0.16;
                base_color = select(vec3<f32>(0.24, 0.48, 0.30), vec3<f32>(0.18, 0.36, 0.22), roof_rib);
                roughness = 0.42;
            } else if (hit_frac.x < 0.19 && hit_frac.y >= 0.27 && hit_frac.y <= 0.57) {
                let in_nest = hit_frac.y > 0.33 && hit_frac.y < 0.50 && hit_frac.z > 0.28 && hit_frac.z < 0.72;
                let egg_cell = fract(hit_frac.z * 5.0);
                let is_egg = in_nest && egg_cell > 0.22 && egg_cell < 0.78 && hit_frac.y > 0.37;
                if (is_egg) {
                    base_color = select(vec3<f32>(0.96, 0.94, 0.88), vec3<f32>(0.88, 0.66, 0.44), hit_frac.z > 0.5);
                    roughness = 0.22;
                } else if (in_nest) {
                    base_color = vec3<f32>(0.84, 0.70, 0.32);
                    roughness = 0.68;
                } else {
                    base_color = vec3<f32>(0.72, 0.42, 0.22);
                    roughness = 0.48;
                }
            } else if (hit_frac.x < 0.25 && hit_frac.y < 0.23) {
                let cleat = fract(hit_frac.x * 10.0) < 0.24;
                base_color = select(vec3<f32>(0.76, 0.54, 0.28), vec3<f32>(0.50, 0.32, 0.16), cleat);
                roughness = 0.52;
            } else if (hit_frac.y < 0.185) {
                base_color = vec3<f32>(0.48, 0.30, 0.16);
                roughness = 0.58;
            } else {
                let is_door = hit_frac.x < 0.22 && hit_frac.z > 0.38 && hit_frac.z < 0.62 && hit_frac.y < 0.46;
                let is_trim = hit_frac.z < 0.15 || hit_frac.z > 0.85 || hit_frac.y > 0.70;
                let plank = fract(hit_pos.y * 6.0) < 0.14;
                var wall_c = select(vec3<f32>(0.78, 0.44, 0.22), vec3<f32>(0.60, 0.32, 0.15), plank);
                if (is_trim) { wall_c = vec3<f32>(0.94, 0.90, 0.82); }
                if (is_door) { wall_c = vec3<f32>(0.20, 0.14, 0.10); }
                base_color = wall_c;
                roughness = 0.46;
            }
        } else {
            if (hit_frac.y >= 0.18 && hit_frac.x > 0.40) {
                let is_lid = hit_frac.y > 0.41;
                let is_band = abs(hit_frac.y - 0.31) < 0.025;
                base_color = select(vec3<f32>(0.18, 0.58, 0.32), vec3<f32>(0.18, 0.22, 0.20), is_lid);
                if (is_band) { base_color = vec3<f32>(0.94, 0.74, 0.18); }
                roughness = 0.34;
            } else if (hit_frac.y >= 0.18) {
                let is_base_lip = hit_frac.y < 0.23;
                base_color = select(vec3<f32>(0.28, 0.70, 0.90), vec3<f32>(0.88, 0.20, 0.18), is_base_lip);
                roughness = 0.20;
            } else {
                let in_feed = hit_frac.y > 0.13 && abs(hit_frac.z - 0.5) < 0.24 && hit_frac.x > 0.14 && hit_frac.x < 0.86;
                base_color = select(vec3<f32>(0.74, 0.78, 0.82), vec3<f32>(0.86, 0.72, 0.34), in_feed);
                roughness = select(0.26, 0.62, in_feed);
                metallic = select(0.78, 0.08, in_feed);
            }
        }
    } else if (hit_mat == 59u) {
        // MAT_CHICKEN_HEN: Animated 3D Heritage Hens (Speckled Buff Orpington, Rhode Island Red, Sussex Pearl-White)
        let t_hen = camera.avatar_pos.w;
        let wx1 = sin(t_hen * 1.3 + f32(mapPos.x * 5 + mapPos.z * 3)) * 0.14;
        let wz1 = cos(t_hen * 0.9 + f32(mapPos.z * 7)) * 0.14;
        let h1_center = vec2<f32>(0.35 + wx1, 0.38 + wz1);
        let h2_center = vec2<f32>(0.68 - wz1 * 0.8, 0.63 + wx1 * 0.8);
        let is_hen2 = length(hit_frac.xz - h2_center) < length(hit_frac.xz - h1_center);
        let hen_local_xz = hit_frac.xz - select(vec2<f32>(wx1, wz1), vec2<f32>(-wz1 * 0.8, wx1 * 0.8), is_hen2);
        let peck_val = select(
            abs(sin(t_hen * 6.0 + f32(mapPos.x * 3 + mapPos.z * 5))) * 0.12,
            abs(cos(t_hen * 5.2 + f32(mapPos.x * 7 + mapPos.z * 3))) * 0.11,
            is_hen2
        );
        let head_top = select(0.39, 0.37, is_hen2) - peck_val;
        let in_head_z = select(hen_local_xz.y < 0.36, hen_local_xz.y < 0.55 && hen_local_xz.x > 0.56, is_hen2);
        let is_comb = (in_head_z && hit_frac.y > head_top - 0.01) || hit_frac.y > 0.365;
        let is_beak_or_legs = hit_frac.y < 0.085 || (in_head_z && select(hen_local_xz.y < 0.25, hen_local_xz.y < 0.44, is_hen2) && hit_frac.y < head_top - 0.04);

        let breed_id = (u32(mapPos.x + mapPos.z * 2) + select(0u, 1u, is_hen2)) % 3u;
        var plumage = vec3<f32>(0.88, 0.64, 0.28); // Speckled Buff Orpington golden-ochre
        if (breed_id == 1u) {
            plumage = vec3<f32>(0.68, 0.26, 0.16); // Rhode Island Red warm chestnut
        } else if (breed_id == 2u) {
            plumage = vec3<f32>(0.94, 0.92, 0.88); // Light Sussex pearl-white
        }
        let speckle = step(0.65, sin(hit_pos.x * 28.0) * cos(hit_pos.y * 28.0 + hit_pos.z * 28.0));
        plumage = plumage * (0.92 + 0.08 * speckle);

        if (is_comb) {
            base_color = vec3<f32>(0.92, 0.14, 0.16);
            roughness = 0.38;
        } else if (is_beak_or_legs) {
            base_color = vec3<f32>(0.96, 0.68, 0.14);
            roughness = 0.34;
        } else {
            base_color = plumage;
            roughness = 0.54;
        }
    } else if (hit_mat == 60u) {
        // MAT_MU_BIOCHAR_KILN: Scenario Mu Smokeless TLUD / Flame-Cap Pyrolysis Biochar Kiln
        let flame_pulse = 0.86 + 0.14 * sin(camera.avatar_pos.w * 6.5);
        let is_flame_cap = hit_frac.y > 0.72;
        let is_biochar_base = hit_frac.y < 0.18;
        let barrel_rib = abs(hit_frac.y - 0.38) < 0.025 || abs(hit_frac.y - 0.62) < 0.025;
        if (is_flame_cap) {
            let in_collar_flame = hit_frac.y > 0.88 || (hit_normal.y > 0.5 && length(hit_frac.xz - vec2<f32>(0.5)) < 0.20);
            base_color = select(vec3<f32>(0.24, 0.22, 0.20), vec3<f32>(1.00, 0.52, 0.12) * flame_pulse, in_collar_flame);
            roughness = select(0.36, 0.18, in_collar_flame);
            metallic = select(0.72, 0.10, in_collar_flame);
            emissive = select(vec3<f32>(0.28, 0.10, 0.02), vec3<f32>(0.88, 0.38, 0.06) * flame_pulse, in_collar_flame);
        } else if (is_biochar_base) {
            base_color = vec3<f32>(0.10, 0.10, 0.11);
            roughness = 0.82;
        } else {
            base_color = select(vec3<f32>(0.20, 0.22, 0.25), vec3<f32>(0.36, 0.38, 0.42), barrel_rib);
            roughness = 0.38;
            metallic = 0.76;
        }
    } else if (hit_mat == 61u || hit_mat == 62u) {
        // MAT_MU_COMPOST_DRUM: Scenario Mu Insulated Thermophilic Composting Drum & Translucent-Amber Struvite Carboy
        let is_carboy = hit_frac.x < 0.43 && hit_frac.z < 0.43 && hit_frac.y < 0.49;
        let is_stand = hit_frac.y < 0.23 && !is_carboy;
        if (is_carboy) {
            let is_crystal_bed = hit_frac.y < 0.16;
            let is_cap = hit_frac.y > 0.42;
            base_color = select(vec3<f32>(0.88, 0.58, 0.18), vec3<f32>(0.94, 0.90, 0.76), is_crystal_bed);
            if (is_cap) { base_color = vec3<f32>(0.76, 0.36, 0.22); }
            roughness = 0.16;
            metallic = 0.25;
            emissive = vec3<f32>(0.14, 0.08, 0.02);
        } else if (is_stand) {
            base_color = vec3<f32>(0.38, 0.28, 0.20);
            roughness = 0.52;
        } else {
            let drum_band = abs(hit_frac.x - 0.5) < 0.045 || abs(hit_frac.y - 0.50) < 0.03;
            base_color = select(vec3<f32>(0.22, 0.50, 0.30), vec3<f32>(0.74, 0.42, 0.24), drum_band);
            roughness = 0.40;
        }
    } else if (hit_mat == 64u) {
        // STRATA_CLAY_SUBSOIL: Dense terracotta-ochre clay subsoil horizon with sedimentary banding
        let horizon_band = sin(hit_pos.y * 14.0 + sin(hit_pos.x * 3.5 + hit_pos.z * 3.5) * 0.8) * 0.045;
        let clay_mottle = sin(hit_pos.x * 11.0) * cos(hit_pos.z * 11.0) * 0.025;
        base_color = vec3<f32>(0.58, 0.33, 0.20) + vec3<f32>(horizon_band + clay_mottle, (horizon_band + clay_mottle) * 0.65, 0.0);
        roughness = 0.78;
    } else if (hit_mat == 65u) {
        // STRATA_COMPACTED_GRAVEL_PAD: Engineered crushed-stone capillary-break grading aggregate
        let pebble_a = sin(hit_pos.x * 26.0 + hit_pos.y * 19.0) * cos(hit_pos.z * 26.0 - hit_pos.y * 17.0);
        let pebble_b = step(0.45, sin(hit_pos.x * 37.0) * cos(hit_pos.z * 39.0));
        let gravel_base = mix(vec3<f32>(0.46, 0.48, 0.50), vec3<f32>(0.62, 0.63, 0.65), pebble_b * 0.65);
        base_color = gravel_base + vec3<f32>(pebble_a * 0.045);
        roughness = 0.80;
    } else if (hit_mat == 66u) {
        // STRATA_CONCRETE_FOOTING_SLAB: Poured reinforced structural concrete perimeter foundation footing
        let form_seam = select(0.0, 1.0, hit_frac.x < 0.04 || hit_frac.z < 0.04 || abs(hit_frac.y - 0.5) < 0.025);
        let pour_agg = sin(hit_pos.x * 20.0) * cos(hit_pos.z * 20.0) * 0.018;
        base_color = mix(vec3<f32>(0.64, 0.65, 0.63) + vec3<f32>(pour_agg), vec3<f32>(0.45, 0.46, 0.45), form_seam * 0.60);
        roughness = 0.58;
        metallic = 0.06;
    } else if (hit_mat == 67u) {
        // STRATA_TOPSOIL_LOAM: Dark nutrient-rich sandy-loam topsoil horizon
        let loam_crumb = sin(hit_pos.x * 22.0 + hit_pos.y * 15.0) * cos(hit_pos.z * 22.0) * 0.028;
        let sand_fleck = step(0.78, sin(hit_pos.x * 31.0) * cos(hit_pos.z * 29.0 + hit_pos.y * 23.0));
        base_color = mix(vec3<f32>(0.24, 0.18, 0.13) + vec3<f32>(loam_crumb), vec3<f32>(0.38, 0.30, 0.20), sand_fleck * 0.35);
        roughness = 0.82;
    } else if (hit_mat == 68u) {
        // STRATA_UTILITY_TRENCH: Bedding gravel utility trench with potable water & electrical conduit runs
        let in_water_pipe = abs(hit_frac.z - 0.36) < 0.11 && hit_frac.y > 0.34 && hit_frac.y < 0.66;
        let in_elec_conduit = abs(hit_frac.z - 0.66) < 0.09 && hit_frac.y > 0.38 && hit_frac.y < 0.64;
        if (in_water_pipe) {
            base_color = vec3<f32>(0.16, 0.52, 0.84);
            roughness = 0.26;
            metallic = 0.25;
        } else if (in_elec_conduit) {
            base_color = vec3<f32>(0.92, 0.44, 0.12);
            roughness = 0.28;
            metallic = 0.20;
        } else {
            let bedding_speck = sin(hit_pos.x * 28.0) * cos(hit_pos.z * 28.0) * 0.035;
            base_color = vec3<f32>(0.42, 0.43, 0.44) + vec3<f32>(bedding_speck);
            roughness = 0.78;
        }
    } else if (hit_mat == 70u) {
        // MAT_TOOL_WHEELBARROW: Front dark-rubber wheel + emerald powder-coated steel tray + ash-wood handles & rear legs
        let is_wheel = hit_frac.z < 0.238 || (hit_frac.z < 0.345 && hit_frac.y < 0.218);
        let is_handles_legs = !is_wheel && (hit_frac.z > 0.782 || hit_frac.y < 0.218);
        if (is_wheel) {
            let is_hub = abs(hit_frac.y - 0.14) < 0.055 && abs(hit_frac.z - 0.22) < 0.055;
            base_color = select(vec3<f32>(0.15, 0.16, 0.18), vec3<f32>(0.78, 0.82, 0.86), is_hub);
            roughness = select(0.68, 0.22, is_hub);
            metallic = select(0.05, 0.84, is_hub);
        } else if (is_handles_legs) {
            let is_ash_handle = hit_frac.y > 0.28;
            let is_grip = is_ash_handle && hit_frac.z > 0.87;
            base_color = select(vec3<f32>(0.22, 0.25, 0.28), vec3<f32>(0.76, 0.56, 0.34), is_ash_handle);
            if (is_grip) {
                base_color = vec3<f32>(0.74, 0.22, 0.18);
            }
            roughness = select(0.36, 0.42, is_ash_handle);
            metallic = select(0.68, 0.06, is_ash_handle);
        } else {
            let is_rim = hit_frac.y > 0.51;
            base_color = select(vec3<f32>(0.14, 0.46, 0.28), vec3<f32>(0.22, 0.58, 0.38), is_rim);
            roughness = 0.28;
            metallic = 0.48;
        }
    } else if (hit_mat == 71u) {
        // MAT_TOOL_BUCKET_CAN: Brushed galvanized zinc bucket with water shimmer + sage watering can with copper spout
        if (hit_frac.x < 0.53) {
            let in_water = hit_normal.y > 0.5 && hit_frac.y > 0.36 &&
                           hit_frac.x > 0.19 && hit_frac.x < 0.49 &&
                           hit_frac.z > 0.27 && hit_frac.z < 0.57;
            if (in_water) {
                let shimmer = 0.90 + 0.10 * sin(camera.avatar_pos.w * 4.5 + hit_pos.x * 16.0 + hit_pos.z * 16.0);
                base_color = vec3<f32>(0.22, 0.68, 0.86) * shimmer;
                roughness = 0.10;
                metallic = 0.35;
                emissive = vec3<f32>(0.03, 0.11, 0.16) * shimmer;
            } else {
                let zinc_rib = abs(hit_frac.y - 0.14) < 0.018 || abs(hit_frac.y - 0.28) < 0.018 || hit_frac.y > 0.37;
                let spangle = sin(hit_pos.x * 32.0 + hit_pos.y * 28.0) * cos(hit_pos.z * 32.0) * 0.03;
                base_color = select(vec3<f32>(0.72, 0.76, 0.80) + vec3<f32>(spangle), vec3<f32>(0.84, 0.88, 0.92), zinc_rib);
                roughness = 0.24;
                metallic = 0.84;
            }
        } else {
            let is_copper_spout = hit_frac.z < 0.302 || hit_frac.y > 0.382;
            base_color = select(vec3<f32>(0.38, 0.62, 0.44), vec3<f32>(0.88, 0.52, 0.28), is_copper_spout);
            roughness = select(0.30, 0.18, is_copper_spout);
            metallic = select(0.32, 0.88, is_copper_spout);
        }
    } else if (hit_mat == 72u) {
        // MAT_TOOL_HAND_DOLLY: Gunmetal steel toe-plate + rear rubber wheels + upright crimson-red tubular steel L-frame
        if (hit_frac.y <= 0.162) {
            let is_rear_wheel = hit_frac.z > 0.54 && (hit_frac.x < 0.32 || hit_frac.x > 0.68);
            base_color = select(vec3<f32>(0.58, 0.62, 0.66), vec3<f32>(0.15, 0.16, 0.18), is_rear_wheel);
            roughness = select(0.28, 0.68, is_rear_wheel);
            metallic = select(0.82, 0.05, is_rear_wheel);
        } else {
            let is_top_grip = hit_frac.y > 0.85;
            let cross_strap = abs(fract(hit_frac.y * 4.0) - 0.5) < 0.08;
            let frame_red = select(vec3<f32>(0.84, 0.18, 0.16), vec3<f32>(0.72, 0.14, 0.13), cross_strap);
            base_color = select(frame_red, vec3<f32>(0.18, 0.20, 0.22), is_top_grip);
            roughness = select(0.26, 0.52, is_top_grip);
            metallic = select(0.52, 0.10, is_top_grip);
        }
    } else if (hit_mat == 25u) {
        let leak_beacon = 0.78 + 0.22 * sin(camera.avatar_pos.w * 7.0);
        base_color = vec3<f32>(0.20, 0.85, 1.00) * leak_beacon;
        emissive = vec3<f32>(0.10, 0.45, 0.60) * leak_beacon;
    } else if (hit_mat == 26u) {
        let bp_edge_x = min(hit_frac.x, 1.0 - hit_frac.x);
        let bp_edge_y = min(hit_frac.y, 1.0 - hit_frac.y);
        let bp_edge_z = min(hit_frac.z, 1.0 - hit_frac.z);
        let bp_wire = min(max(bp_edge_x, bp_edge_y), min(max(bp_edge_y, bp_edge_z), max(bp_edge_x, bp_edge_z)));
        let bp_pulse = 0.70 + 0.30 * sin(camera.avatar_pos.w * 6.0);
        base_color = select(vec3<f32>(0.15, 0.90, 0.95) * (0.55 * bp_pulse), vec3<f32>(0.35, 1.00, 0.90), bp_wire < 0.11);
        emissive = base_color * 0.65;
    } else if (hit_mat == 27u) {
        let is_legs = hit_frac.y < 0.34;
        let is_hair = npc_subpart == 2u && hit_frac.y > 0.84;
        let body_c = select(vec3<f32>(0.80, 0.48, 0.22), vec3<f32>(0.22, 0.26, 0.34), is_legs);
        let head_c = select(vec3<f32>(0.92, 0.77, 0.62), vec3<f32>(0.24, 0.16, 0.12), is_hair);
        base_color = select(body_c, head_c, npc_subpart == 2u);
        roughness = 0.46;
    } else if (hit_mat == 28u) {
        let is_legs = hit_frac.y < 0.34;
        let is_hair = npc_subpart == 2u && hit_frac.y > 0.84;
        let body_c = select(vec3<f32>(0.16, 0.66, 0.82), vec3<f32>(0.20, 0.25, 0.34), is_legs);
        let head_c = select(vec3<f32>(0.94, 0.80, 0.64), vec3<f32>(0.20, 0.14, 0.10), is_hair);
        base_color = select(body_c, head_c, npc_subpart == 2u);
        roughness = 0.44;
    } else if (hit_mat == 29u) {
        let is_legs = hit_frac.y < 0.34;
        let is_hair = npc_subpart == 2u && hit_frac.y > 0.84;
        let body_c = select(vec3<f32>(0.16, 0.74, 0.48), vec3<f32>(0.22, 0.28, 0.26), is_legs);
        let head_c = select(vec3<f32>(0.95, 0.82, 0.66), vec3<f32>(0.26, 0.18, 0.12), is_hair);
        base_color = select(body_c, head_c, npc_subpart == 2u);
        roughness = 0.42;
    } else if (hit_mat == 32u) {
        let rung = step(0.72, fract(hit_pos.y * 4.0));
        let rail = step(0.24, abs(hit_frac.z - 0.5));
        let is_frame = max(rung, rail);
        base_color = mix(vec3<f32>(0.48, 0.32, 0.18), vec3<f32>(0.80, 0.82, 0.86), is_frame);
        roughness = mix(0.58, 0.26, is_frame);
        metallic = mix(0.15, 0.78, is_frame);
    } else if (hit_mat == 33u) {
        let tr_x = min(hit_frac.x, 1.0 - hit_frac.x);
        let tr_y = min(hit_frac.y, 1.0 - hit_frac.y);
        let tr_z = min(hit_frac.z, 1.0 - hit_frac.z);
        let truss_edge = min(max(tr_x, tr_y), min(max(tr_y, tr_z), max(tr_x, tr_z))) < 0.14;
        let stage_pulse = 0.5 + 0.5 * sin(camera.avatar_pos.w * 6.5);
        let status_glow = mix(vec3<f32>(0.98, 0.72, 0.12), vec3<f32>(0.15, 0.95, 0.58), stage_pulse);
        base_color = select(vec3<f32>(0.46, 0.36, 0.24), vec3<f32>(0.82, 0.62, 0.28), truss_edge);
        roughness = 0.36;
        metallic = 0.45;
        emissive = status_glow * select(0.18, 0.55, truss_edge);
    } else if (hit_mat == 34u) {
        // MAT_MATURE_CROP: Raised Cedar Planter Bed (y in [0.0..0.24]) + Carrot/Root Veggie Crowns + A-Frame Pea/Bean Trellis
        let is_cedar_rim = hit_frac.y < 0.235 && (hit_frac.x < 0.12 || hit_frac.x > 0.88 || hit_frac.z < 0.12 || hit_frac.z > 0.88);
        if (is_cedar_rim) {
            let plank_seam = abs(hit_frac.y - 0.12) < 0.018;
            base_color = select(vec3<f32>(0.68, 0.42, 0.22), vec3<f32>(0.46, 0.28, 0.14), plank_seam);
            roughness = 0.50;
        } else if (hit_frac.y < 0.235) {
            base_color = vec3<f32>(0.20, 0.15, 0.10);
            roughness = 0.76;
        } else if (hit_frac.y < 0.385) {
            let root_spot = sin(hit_pos.z * 16.0) * cos(hit_pos.x * 14.0);
            let is_carrot = root_spot > 0.35;
            base_color = select(vec3<f32>(0.28, 0.64, 0.20), vec3<f32>(0.96, 0.48, 0.12), is_carrot);
            roughness = 0.48;
        } else {
            let is_trellis_wood = hit_frac.z < 0.14 || hit_frac.z > 0.86 || hit_frac.y > 0.74 || abs(fract(hit_pos.z * 3.0) - 0.5) < 0.07;
            let pea_pod = step(0.45, sin(hit_pos.y * 22.0) * cos(hit_pos.z * 18.0));
            let vine_col = mix(vec3<f32>(0.24, 0.66, 0.22), vec3<f32>(0.58, 0.90, 0.24), pea_pod * 0.75);
            base_color = select(vine_col, vec3<f32>(0.72, 0.48, 0.26), is_trellis_wood);
            roughness = 0.46;
            if (!is_trellis_wood && pea_pod > 0.5) {
                emissive = vec3<f32>(0.08, 0.14, 0.02);
            }
        }
    } else if (hit_mat == 35u) {
        base_color = vec3<f32>(0.48, 0.30, 0.18);
        roughness = 0.68;
    } else if (hit_mat == 36u) {
        let crown_tier = select(0.0, 1.0, mapPos.y == 3 || hit_frac.y > 0.68);
        let tree_tint = fract(f32(absX_hit * 3 + lotZ_hit * 7) * 0.17);
        let emerald_leaf = mix(vec3<f32>(0.24, 0.62, 0.28), vec3<f32>(0.38, 0.72, 0.26), crown_tier * 0.65 + tree_tint * 0.35);
        base_color = emerald_leaf;
        roughness = 0.52;
    } else if (hit_mat == 37u) {
        let is_box = hit_frac.y > 0.61;
        let is_flag = hit_frac.y > 0.72 && hit_frac.z > 0.52 && hit_frac.x > 0.62;
        base_color = select(vec3<f32>(0.54, 0.36, 0.20), vec3<f32>(0.24, 0.38, 0.58), is_box);
        if (is_flag) {
            base_color = vec3<f32>(0.94, 0.20, 0.18);
        }
        roughness = select(0.58, 0.32, is_box);
        metallic = select(0.05, 0.55, is_box);
    } else if (hit_mat == 38u) {
        base_color = select(vec3<f32>(0.26, 0.58, 0.25), vec3<f32>(0.42, 0.70, 0.28), hit_normal.y > 0.25);
        roughness = 0.54;
    } else if (hit_mat == 120u) {
        // Room 2 — MAT_OFFICE_DESK_PC: Wooden Desk + PC Tower + Dual Glowing Monitors + Swivel Chair
        let is_monitor = hit_frac.y > 0.52 && hit_frac.z < 0.34;
        let is_chair = hit_frac.z > 0.65;
        let is_desk_slab = hit_frac.y >= 0.41 && hit_frac.y <= 0.52 && !is_chair;
        if (is_monitor) {
            let dual_gap = abs(hit_frac.x - 0.5) < 0.03;
            let mon_glow = vec3<f32>(0.10, 0.88, 0.78);
            base_color = select(mon_glow, vec3<f32>(0.14, 0.16, 0.20), dual_gap);
            roughness = 0.12;
            if (!dual_gap) {
                emissive = mon_glow * 0.65;
            }
        } else if (is_chair) {
            base_color = vec3<f32>(0.22, 0.46, 0.56);
            roughness = 0.45;
        } else if (is_desk_slab) {
            base_color = vec3<f32>(0.68, 0.48, 0.28);
            roughness = 0.32;
        } else {
            let pwr_led = hit_frac.x < 0.26 && abs(hit_frac.y - 0.32) < 0.03 && hit_frac.z > 0.55;
            base_color = select(vec3<f32>(0.18, 0.22, 0.26), vec3<f32>(0.0, 0.95, 0.75), pwr_led);
            roughness = 0.30;
            metallic = 0.60;
            if (pwr_led) {
                emissive = vec3<f32>(0.0, 0.85, 0.65);
            }
        }
    } else if (hit_mat == 122u) {
        let trace_grid = step(0.84, fract(hit_pos.x * 12.0)) + step(0.84, fract(hit_pos.z * 12.0));
        let is_gold_trace = clamp(trace_grid, 0.0, 1.0);
        let pcb_emerald = vec3<f32>(0.14, 0.46, 0.30);
        let gold_trace = vec3<f32>(0.94, 0.78, 0.24);
        base_color = mix(pcb_emerald, gold_trace, is_gold_trace * 0.45);
        roughness = mix(0.38, 0.18, is_gold_trace);
        metallic = mix(0.25, 0.88, is_gold_trace);
        emissive = vec3<f32>(0.04, 0.22, 0.14) + gold_trace * (is_gold_trace * 0.18);
    } else {
        base_color = vec3<f32>(0.50, 0.50, 0.50);
    }

    // Stylized Low-Poly Diorama Lighting: Quantized Facet Diffuse + Warm Golden Rim Light + 16-Step Sun Shadow
    let light_dir = normalize(vec3<f32>(0.5, 0.8, -0.4));
    let view_dir = -rd;
    let half_vec = normalize(light_dir + view_dir);

    var edge_ao: f32 = 1.0;
    if (((hit_mat < 26u && mapPos.y >= 1) || (hit_mat >= 64u && hit_mat <= 68u)) && hit_mat != 6u && hit_mat != 17u) {
        let ex = min(hit_frac.x, 1.0 - hit_frac.x);
        let ey = min(hit_frac.y, 1.0 - hit_frac.y);
        let ez = min(hit_frac.z, 1.0 - hit_frac.z);
        let min_e = min(max(ex, ey), min(max(ey, ez), max(ex, ez)));
        edge_ao = smoothstep(0.0, 0.05, min_e) * 0.10 + 0.90;
        if (mapPos.y == 1 && abs(hit_normal.y) < 0.5 && hit_frac.y < 0.15) {
            edge_ao = edge_ao * mix(0.74, 1.0, hit_frac.y / 0.15);
        }
    }

    let sun_shadow = select(trace_sun_shadow(hit_pos + hit_normal * 0.03, light_dir, z_cutaway_level), 1.0, (hit_mat >= 26u && hit_mat <= 62u) || (hit_mat >= 70u && hit_mat <= 72u) || hit_mat == 6u || hit_mat == 17u || hit_mat == 19u);
    let n_dot_l = max(dot(hit_normal, light_dir), 0.0);
    let stylized_diff = mix(n_dot_l, floor(n_dot_l * 4.0 + 0.5) / 4.0, 0.35);
    let hemi_t = hit_normal.y * 0.5 + 0.5;
    let hemi_ambient = mix(vec3<f32>(0.28, 0.25, 0.22), vec3<f32>(0.46, 0.52, 0.60), hemi_t);

    let shininess = mix(96.0, 8.0, roughness);
    let spec_pow = pow(max(dot(hit_normal, half_vec), 0.0), shininess);
    let spec_strength = mix(0.04, 0.72, 1.0 - roughness) * (0.35 + 0.65 * metallic);
    let spec_light = vec3<f32>(1.0, 0.96, 0.88) * spec_pow * spec_strength * sun_shadow;

    let n_dot_v = max(dot(hit_normal, view_dir), 0.0);
    let fresnel_refl = pow(1.0 - n_dot_v, 4.0) * (1.0 - roughness);
    let refl_sky = get_sky_color(reflect(rd, hit_normal)) * fresnel_refl * (0.25 + 0.55 * metallic);
    let rim_light = vec3<f32>(1.0, 0.90, 0.70) * pow(1.0 - n_dot_v, 3.0) * 0.18 * sun_shadow;

    var lit_color = (base_color * (hemi_ambient + vec3<f32>(0.96, 0.92, 0.84) * stylized_diff * sun_shadow) + spec_light + refl_sky + rim_light) * edge_ao + emissive;

    // F1-F4 Info-View Shader Overlays (STORY-9.1)
    if (overlay_mode == 1u) {
        let leak_pulse = 0.65 + 0.35 * sin(camera.avatar_pos.w * 8.0);
        if (hit_mat == 25u || hit_mat == 7u) {
            lit_color = mix(lit_color, vec3<f32>(0.20, 0.85, 1.00), leak_pulse);
        } else if (hit_mat == 16u || hit_mat == 22u || hit_mat == 41u || hit_mat == 60u) {
            lit_color = mix(lit_color, vec3<f32>(1.00, 0.50, 0.10), 0.75);
        } else if (hit_mat == 23u || hit_mat == 21u || hit_mat == 5u || hit_mat == 6u ||
                   hit_mat == 13u || hit_mat == 14u || hit_mat == 15u || hit_mat == 17u ||
                   hit_mat == 57u || hit_mat == 66u) {
            lit_color = mix(lit_color, vec3<f32>(0.15, 0.92, 0.55), 0.65);
        } else {
            lit_color = lit_color * 0.45;
        }
    } else if (overlay_mode == 2u) {
        if (hit_mat == 11u || hit_mat == 8u || hit_mat == 9u || hit_mat == 120u || hit_mat == 49u) {
            lit_color = mix(lit_color, vec3<f32>(1.00, 0.85, 0.15), 0.70);
        } else if (hit_mat == 12u || hit_mat == 18u || hit_mat == 19u || hit_mat == 34u ||
                   hit_mat == 42u || hit_mat == 45u || hit_mat == 46u || hit_mat == 47u ||
                   (hit_mat >= 58u && hit_mat <= 62u) || hit_mat == 68u || hit_mat == 71u) {
            lit_color = mix(lit_color, vec3<f32>(0.00, 0.92, 1.00), 0.70);
        } else {
            lit_color = lit_color * 0.45;
        }
    } else if (overlay_mode == 3u) {
        if (hit_mat == 19u || hit_mat == 20u || hit_mat == 24u || hit_mat == 33u || hit_mat == 34u ||
            (hit_mat >= 40u && hit_mat <= 44u) || (hit_mat >= 54u && hit_mat <= 62u) ||
            hit_mat == 67u || (hit_mat >= 70u && hit_mat <= 72u)) {
            lit_color = mix(lit_color, vec3<f32>(0.00, 0.98, 0.75), 0.75);
        } else {
            lit_color = lit_color * 0.45;
        }
    } else if (overlay_mode == 4u) {
        let stress_lvl = clamp(camera.avatar_params.y, 0.0, 1.0);
        let stress_col = mix(vec3<f32>(0.10, 0.95, 0.55), vec3<f32>(1.00, 0.15, 0.25), stress_lvl);
        if (hit_mat == 27u) {
            lit_color = mix(lit_color, vec3<f32>(1.0, 0.30, 0.15), 0.85);
        } else if (hit_mat == 28u) {
            lit_color = mix(lit_color, vec3<f32>(0.10, 0.85, 1.0), 0.85);
        } else if (hit_mat == 29u) {
            lit_color = mix(lit_color, vec3<f32>(0.20, 0.95, 0.45), 0.85);
        } else if (hit_mat == 21u || hit_mat == 22u || hit_mat == 20u || hit_mat == 23u ||
                   (hit_mat >= 48u && hit_mat <= 51u)) {
            lit_color = mix(lit_color, stress_col, 0.70);
        }
        let hit_world = ro + rd * hit_dist;
        let ring_d = abs(length(hit_world.xz - camera.avatar_pos.xz) - 1.5);
        if (ring_d < 0.18 && mapPos.y == 0) {
            lit_color = mix(lit_color, stress_col, 0.85);
        }
    }

    // Roof / Wall Cutaway top-face architectural cross-section cap (active loaded map chunk only)
    if (cutaway_active && in_playable_chunk && mapPos.y == i32(floor(camera.camera_target.w + 0.5)) - 1 && hit_normal.y > 0.5) {
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

    // Distance fog blend into horizon & Neighbor Fog Envelope (active loaded map chunk stays crystal-clear)
    let sky = get_sky_color(rd);
    var fog = 1.0 - exp(-hit_dist * 0.0075);
    if (!in_playable_chunk && abs(mapPos.x) >= 6) {
        let neighbor_fog = clamp(fog + 0.42, 0.0, 0.86);
        fog = neighbor_fog;
    }
    let toned_color = aces_tonemap(lit_color * 1.12);
    let final_color = mix(toned_color, sky, fog);

    return vec4<f32>(final_color, 1.0);
}
)WGSL";

} // namespace shaders
} // namespace oasis
