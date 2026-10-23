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

// Helper: True only for Player Lot 402 (keeps neighbor houses sealed in fog without interior props or cutaway)
fn is_player_lot_402(pos: vec3<i32>) -> bool {
    return pos.x >= 4 && pos.x <= 26 && pos.z >= 0 && pos.z <= 15;
}

// Visual-only Stylized Low-Poly Diorama Trees, Shrubs & Curbside Mailbox
// Placed on suburban lots completely clear of house footprints, doors, walkways, and Lot 402 garage
fn get_decorative_prop_material(pos: vec3<i32>) -> u32 {
    if (pos.y < 1 || pos.y > 3) {
        return 0u;
    }
    let absX = abs(pos.x);
    let lotZ = (pos.z % 16 + 16) % 16;

    // Low-Poly Trees (35u = Tapered Cedar Trunk at y == 1, 36u = Faceted Octahedron Crown at y == 2..3)
    let is_tree_xz = (absX == 7 && lotZ == 14 && !is_player_lot_402(pos)) ||
                     (pos.x == 7 && pos.z == 14) ||
                     (pos.x == 23 && (lotZ == 3 || lotZ == 13)) ||
                     (pos.x == -21 && (lotZ == 4 || lotZ == 12));
    if (is_tree_xz) {
        if (pos.y == 1) {
            return 35u; // MAT_TREE_TRUNK
        }
        if (pos.y == 2 || pos.y == 3) {
            return 36u; // MAT_TREE_CROWN
        }
    }

    // Low-Poly Front Garden Shrubs (38u) flanking front lawn (kept clear of Lot 402 Garage at z in [10..14])
    if (pos.y == 1 && absX == 8) {
        if (is_player_lot_402(pos)) {
            if (pos.z == 4 || pos.z == 8) {
                return 38u; // MAT_SHRUB flanking Lot 402 front windows
            }
        } else if (lotZ == 11 || lotZ == 12) {
            return 38u; // MAT_SHRUB on neighbor lots
        }
    }

    // Low-Poly Curbside Mailbox (37u) by Lot 402 front driveway/walkway
    if (pos.y == 1 && pos.x == 6 && lotZ == 4) {
        return 37u; // MAT_MAILBOX
    }

    return 0u;
}

// Procedural Suburban Sprawl & Furnished 6-Room Lot 402 Homestead + Attached Garage
fn get_voxel_material(pos: vec3<i32>) -> u32 {
    let lot402 = is_player_lot_402(pos);
    let dyn_mat = get_dynamic_voxel_material(pos);
    if (dyn_mat != 0u) {
        if (lot402) {
            // Suppress ugly front-driveway crate at (8, 1, 3) so driveway is 100% clean concrete
            if (pos.x == 8 && pos.y == 1 && pos.z == 3 && dyn_mat == 24u) {
                return 0u;
            }
            // Remap legacy interior bootstrap markers to proper 6-room furniture appliances
            if (pos.x == 11 && pos.y == 1 && (pos.z == 3 || pos.z == 4) && dyn_mat == 24u) {
                return 41u; // MAT_OVEN_STOVE
            }
            if (pos.x == 11 && pos.y == 1 && pos.z == 8 && dyn_mat == 23u) {
                return 0u; // Open Living Space rug between Couch and TV (Bed is in Bedroom at 15..16, 1, 9)
            }
            if (pos.x == 11 && pos.y == 1 && pos.z == 9 && dyn_mat == 21u) {
                return 49u; // MAT_TV_MEDIA_SET
            }
            if (pos.x == 14 && pos.y == 1 && pos.z == 4 && dyn_mat == 12u) {
                return 0u; // Keep Office floor clear of outdoor water tank
            }
        }
        return dyn_mat;
    }

    let absX = abs(pos.x);
    let lotZ = (pos.z % 16 + 16) % 16; // 16-voxel periodic residential lot pitch

    // --- Layer 0: Ground Plane, Road Infrastructure, Interior Floor & Backyard ---
    if (pos.y == 0) {
        // Central Asphalt Roadway (|x| <= 3)
        if (absX <= 3) {
            if (pos.x == 0 && (pos.z % 4 >= 2 || pos.z % 4 <= -2)) {
                return 2u; // Material 2: Yellow centerline
            }
            return 1u; // Material 1: Asphalt roadway
        }
        // Concrete Curbs & Sidewalks (|x| in [4, 5])
        if (absX == 4 || absX == 5) {
            return 3u; // Material 3: Concrete sidewalk/curbs
        }
        // Concrete driveways leading to garages (including Lot 402 garage apron at z in [11, 12])
        if ((absX >= 6 && absX <= 8 && lotZ >= 1 && lotZ <= 3) ||
            (lot402 && pos.x >= 6 && pos.x <= 7 && (pos.z == 11 || pos.z == 12))) {
            return 3u; // Material 3: Concrete driveway
        }
        // Lawn / Interior Room Floors / Garage Slab / Backyard Loam (|x| in [6, 64])
        if (absX >= 6 && absX <= 64) {
            return 4u; // Material 4: Context-shaded ground & room floors
        }
    }

    // --- Layer 1-8: Legacy Electrical Grid Infrastructure (kept clear of Lot 402 foreground camera at z in [-2, 14]) ---
    if (pos.y >= 1 && pos.y <= 8 && (pos.z < -2 || pos.z > 14)) {
        if (pos.x == 5 && lotZ == 0 && pos.y <= 7) {
            return 8u; // Material 8: Utility timber pole
        }
        if (pos.x >= 4 && pos.x <= 6 && lotZ == 0 && pos.y == 7) {
            return 8u; // Material 8: Utility timber crossarm
        }
        if (pos.x == 5 && lotZ == 1 && pos.y == 6) {
            return 9u; // Material 9: Transformer / grid hardware
        }
        if (pos.x == 4 && pos.y == 7 && pos.z >= -10 && pos.z <= 40) {
            return 9u; // Material 9: Conductor wire / grid hardware
        }
    }

    // --- Lot 402 Attached Garage Wing (x in [8..12], z in [10..14], y in [1..4]) ---
    if (lot402 && pos.x >= 8 && pos.x <= 12 && pos.z >= 10 && pos.z <= 14) {
        if (pos.z >= 11 || pos.x == 8) {
            if (pos.y == 4) {
                return 6u; // Material 6: Garage pitched/flat roof
            }
            if (pos.y >= 1 && pos.y <= 3) {
                // Roll-Up Sectional Garage Door facing street at x == 8, z in [11, 12], y <= 2
                if (pos.x == 8 && (pos.z == 11 || pos.z == 12) && pos.y <= 2) {
                    return 57u; // MAT_GARAGE_DOOR
                }
                // Garage Exterior Clapboard Walls
                if (pos.x == 8 || pos.x == 12 || pos.z == 14) {
                    return 5u; // Material 5: Clapboard siding
                }
                // Room 6 — Garage Interior Furniture along back wall z == 13 (y == 1)
                if (pos.y == 1 && pos.z == 13) {
                    if (pos.x == 9)  { return 54u; } // MAT_GARAGE_WORKBENCH
                    if (pos.x == 10) { return 55u; } // MAT_TOOL_WALL
                    if (pos.x == 11) { return 56u; } // MAT_GARAGE_STORAGE
                }
                return 0u; // Walkable open garage interior
            }
        }
    }

    // --- Layer 1-6: Suburban Residences (Lot 402 6-Room House & Sealed Neighbor Shells) ---
    if (absX >= 9 && absX <= 17 && lotZ >= 2 && lotZ <= 10) {
        // Sloped gable roof (y in [4, 6])
        if (pos.y >= 4 && pos.y <= 6) {
            let roof_inset = pos.y - 3;
            if (absX >= (9 + roof_inset) && absX <= (17 - roof_inset) &&
                lotZ >= (2 + roof_inset) && lotZ <= (10 - roof_inset)) {
                return 6u; // Material 6: Architectural shingle roof
            }
        }
        // Ground Floor Walls, Doors, Windows, & Lot 402 6-Room Interior Furniture (y in [1, 3])
        if (pos.y >= 1 && pos.y <= 3) {
            // Front Door (absX == 9) & Back Patio Door (absX == 17) at lotZ == 6, y <= 2
            if ((absX == 9 || absX == 17) && lotZ == 6 && pos.y <= 2) {
                return 15u; // Material 15: Insulated Timber Door (airtight & passable)
            }
            // Connecting interior doorway between Lot 402 House and Attached Garage at (10, 1..2, 10)
            if (lot402 && pos.x == 10 && pos.z == 10 && pos.y <= 2) {
                return 0u; // Open interior archway into Garage
            }
            // Architectural Windows at y == 2 (skip (11, 2, 10) on Lot 402 where Garage attaches)
            if (pos.y == 2 &&
                (((absX == 9 || absX == 17) && (lotZ == 4 || lotZ == 8)) ||
                 ((lotZ == 2 || lotZ == 10) && (absX == 11 || absX == 15) && !(lot402 && pos.x == 11 && pos.z == 10)))) {
                return 7u; // Material 7: Window glass
            }
            // Exterior Perimeter Clapboard Walls
            if (absX == 9 || absX == 17 || lotZ == 2 || lotZ == 10) {
                return 5u; // Material 5: Painted architectural clapboard siding
            }

            // Sculpted Low-Poly 3D Furniture & Appliances ONLY inside Player Lot 402 (y == 1)
            // Central walking corridors z == 6 (x=10..16) and x == 13 (z=4..9) stay 100% open air (0u)
            if (lot402 && pos.y == 1) {
                // Room 1 — Kitchen + Dining (x in [10..12], z in [3..5])
                if (pos.x == 10 && pos.z == 3) { return 40u; } // MAT_FRIDGE
                if (pos.x == 11 && (pos.z == 3 || pos.z == 4)) { return 41u; } // MAT_OVEN_STOVE
                if (pos.x == 12 && pos.z == 3) { return 42u; } // MAT_KITCHEN_CABINET
                if (pos.x == 10 && pos.z == 5) { return 43u; } // MAT_DINING_TABLE_CHAIR

                // Room 2 — Office (x in [13..14], z in [3..5])
                if (pos.x == 13 && pos.z == 3) { return 120u; } // MAT_OFFICE_DESK_PC
                if (pos.x == 14 && pos.z == 3) { return 44u; }  // MAT_OFFICE_STORAGE

                // Room 3 — Bathroom (x in [15..16], z in [3..5])
                if (pos.x == 16 && pos.z == 3) { return 45u; } // MAT_SHOWER
                if (pos.x == 16 && pos.z == 4) { return 46u; } // MAT_TOILET
                if (pos.x == 15 && pos.z == 3) { return 47u; } // MAT_BATH_SINK_CABINET

                // Room 4 — Living Space (x in [10..12], z in [7..9])
                if ((pos.x == 10 || pos.x == 11) && pos.z == 7) { return 48u; } // MAT_COUCH
                if (pos.x == 11 && pos.z == 9) { return 49u; } // MAT_TV_MEDIA_SET
                if ((pos.x == 10 || pos.x == 12) && pos.z == 9) { return 50u; } // MAT_SOUNDSYSTEM

                // Room 5 — Bedroom (x in [14..16], z in [7..9])
                if ((pos.x == 15 || pos.x == 16) && pos.z == 9) { return 23u; } // MAT_BED
                if (pos.x == 16 && pos.z == 8) { return 51u; } // MAT_NIGHTSTAND
                if (pos.x == 14 && pos.z == 9) { return 52u; } // MAT_WARDROBE
                if (pos.x == 14 && pos.z == 8) { return 53u; } // MAT_MIRROR
            }
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

    let deco_mat = get_decorative_prop_material(pos);
    if (deco_mat != 0u) {
        return deco_mat;
    }

    return 0u; // Air
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

// Smooth 45-degree Pitched Hip/Gable Low-Poly Roof Intersection inside DDA cell [mapPos, mapPos + 1]
fn intersect_sloped_roof_voxel(ro: vec3<f32>, rd: vec3<f32>, mapPos: vec3<i32>) -> vec4<f32> {
    let cell_min = vec3<f32>(f32(mapPos.x), f32(mapPos.y), f32(mapPos.z));
    let lro = ro - cell_min;
    let inv_rd = vec3<f32>(1.0) / select(rd, vec3<f32>(1e-6), abs(rd) < vec3<f32>(1e-6));
    let t0s = (vec3<f32>(0.0) - lro) * inv_rd;
    let t1s = (vec3<f32>(1.0) - lro) * inv_rd;
    let tsmaller = min(t0s, t1s);
    let tbigger  = max(t0s, t1s);
    let t_cell_enter = max(tsmaller.x, max(tsmaller.y, tsmaller.z));
    let t_cell_exit  = min(tbigger.x, min(tbigger.y, tbigger.z));
    if (t_cell_enter > t_cell_exit || t_cell_exit <= 0.0) {
        return vec4<f32>(0.0, 1.0, 0.0, -1.0);
    }

    let absX = abs(mapPos.x);
    let lotZ = (mapPos.z % 16 + 16) % 16;
    if (mapPos.y >= 4 && mapPos.y <= 6 && absX >= 9 && absX <= 17 && lotZ >= 2 && lotZ <= 10) {
        let house_cx = select(-13.5, 13.5, mapPos.x > 0);
        let house_cz = f32(mapPos.z - lotZ) + 6.5;
        let pro = vec3<f32>(ro.x - house_cx, ro.y - 4.0, ro.z - house_cz);

        var t_pyr_enter: f32 = -1e9;
        var t_pyr_exit: f32  =  1e9;
        var n_pyr = vec3<f32>(0.0, 1.0, 0.0);

        for (var p_idx = 0; p_idx < 6; p_idx++) {
            var n_p = vec3<f32>(0.0, 1.0, 0.0);
            var d_lim: f32 = 3.50;
            if (p_idx == 0) { n_p = vec3<f32>( 1.0, 1.0, 0.0); d_lim = 3.50; }
            else if (p_idx == 1) { n_p = vec3<f32>(-1.0, 1.0, 0.0); d_lim = 3.50; }
            else if (p_idx == 2) { n_p = vec3<f32>(0.0, 1.0,  1.0); d_lim = 3.50; }
            else if (p_idx == 3) { n_p = vec3<f32>(0.0, 1.0, -1.0); d_lim = 3.50; }
            else if (p_idx == 4) { n_p = vec3<f32>(0.0, 1.0,  0.0); d_lim = 2.85; }
            else { n_p = vec3<f32>(0.0, -1.0, 0.0); d_lim = 0.0; }

            let denom = dot(n_p, rd);
            let dist = d_lim - dot(n_p, pro);
            if (abs(denom) < 1e-6) {
                if (dist < 0.0) {
                    return vec4<f32>(0.0, 1.0, 0.0, -1.0);
                }
            } else {
                let t_c = dist / denom;
                if (denom < 0.0) {
                    if (t_c > t_pyr_enter) {
                        t_pyr_enter = t_c;
                        n_pyr = normalize(n_p);
                    }
                } else {
                    t_pyr_exit = min(t_pyr_exit, t_c);
                }
            }
        }

        if (t_pyr_enter > t_pyr_exit || t_pyr_exit <= 0.0) {
            return vec4<f32>(0.0, 1.0, 0.0, -1.0);
        }
        let t_overlap_enter = max(t_cell_enter, t_pyr_enter);
        let t_overlap_exit  = min(t_cell_exit, t_pyr_exit);
        if (t_overlap_enter > t_overlap_exit || t_overlap_exit <= 0.0) {
            return vec4<f32>(0.0, 1.0, 0.0, -1.0);
        }
        return vec4<f32>(n_pyr, max(0.001, t_overlap_enter));
    }

    var n_enter = vec3<f32>(0.0, 1.0, 0.0);
    if (tsmaller.x >= tsmaller.y && tsmaller.x >= tsmaller.z) {
        n_enter = vec3<f32>(-sign(rd.x), 0.0, 0.0);
    } else if (tsmaller.y >= tsmaller.x && tsmaller.y >= tsmaller.z) {
        n_enter = vec3<f32>(0.0, -sign(rd.y), 0.0);
    } else {
        n_enter = vec3<f32>(0.0, 0.0, -sign(rd.z));
    }
    return vec4<f32>(n_enter, max(0.001, t_cell_enter));
}

// Sculpted Sub-Voxel Low-Poly Props, 6-Room Furniture/Appliances, Doors/Windows & Faceted Trees
fn intersect_subvoxel_prop(ro: vec3<f32>, rd: vec3<f32>, mapPos: vec3<i32>, mat: u32) -> vec4<f32> {
    if (mat == 6u || mat == 17u) {
        return intersect_sloped_roof_voxel(ro, rd, mapPos);
    }

    let cell_min = vec3<f32>(f32(mapPos.x), f32(mapPos.y), f32(mapPos.z));
    let lro = ro - cell_min;
    let absX = abs(mapPos.x);
    let lotZ = (mapPos.z % 16 + 16) % 16;

    var best_hit = vec4<f32>(0.0, 1.0, 0.0, 1e9);

    if (mat == 10u) {
        let post = intersect_aabb_normal(lro, rd, vec3<f32>(0.37, 0.0, 0.37), vec3<f32>(0.63, 0.92, 0.63));
        if (post.w > 0.0 && post.w < best_hit.w) { best_hit = post; }

        let runs_z = (mapPos.x == 25);
        let runs_x = (lotZ == 1 || lotZ == 15 || !runs_z);
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
        let on_x_wall = (absX == 9 || absX == 17);
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
        let on_x_wall = (absX == 9 || absX == 17);
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
        let gdoor = intersect_aabb_normal(lro, rd, vec3<f32>(0.26, 0.0, 0.0), vec3<f32>(0.68, 1.0, 1.0));
        if (gdoor.w > 0.0 && gdoor.w < best_hit.w) { best_hit = gdoor; }
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
        // Room 5 — Bedroom Bed + Mattress + Headboard + Plump Pillows
        let mattress = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.08), vec3<f32>(0.98, 0.45, 0.94));
        if (mattress.w > 0.0 && mattress.w < best_hit.w) { best_hit = mattress; }
        let headboard = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.45, 0.76), vec3<f32>(0.98, 0.84, 0.94));
        if (headboard.w > 0.0 && headboard.w < best_hit.w) { best_hit = headboard; }
        let pillow = intersect_aabb_normal(lro, rd, vec3<f32>(0.14, 0.45, 0.52), vec3<f32>(0.86, 0.58, 0.76));
        if (pillow.w > 0.0 && pillow.w < best_hit.w) { best_hit = pillow; }
    } else if (mat == 24u || mat == 41u) {
        // Room 1 — Low-Poly Oven + Stove (0.84m counter height + simmering pot on top burner)
        let oven = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.0, 0.08), vec3<f32>(0.96, 0.82, 0.88));
        if (oven.w > 0.0 && oven.w < best_hit.w) { best_hit = oven; }
        let pot = intersect_aabb_normal(lro, rd, vec3<f32>(0.26, 0.82, 0.28), vec3<f32>(0.64, 0.98, 0.66));
        if (pot.w > 0.0 && pot.w < best_hit.w) { best_hit = pot; }
    } else if (mat == 40u) {
        // Room 1 — Tall Brushed Stainless/Mint Refrigerator + Vertical Handles
        let body = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.0, 0.08), vec3<f32>(0.92, 0.96, 0.86));
        if (body.w > 0.0 && body.w < best_hit.w) { best_hit = body; }
        let handle = intersect_aabb_normal(lro, rd, vec3<f32>(0.16, 0.26, 0.86), vec3<f32>(0.24, 0.88, 0.94));
        if (handle.w > 0.0 && handle.w < best_hit.w) { best_hit = handle; }
    } else if (mat == 42u) {
        // Room 1 — Kitchen Cabinets & Counter with Butcher-Block Top + Recessed Sink + Chrome Faucet
        let cab = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.08), vec3<f32>(0.98, 0.82, 0.88));
        if (cab.w > 0.0 && cab.w < best_hit.w) { best_hit = cab; }
        let faucet = intersect_aabb_normal(lro, rd, vec3<f32>(0.44, 0.82, 0.14), vec3<f32>(0.56, 0.98, 0.36));
        if (faucet.w > 0.0 && faucet.w < best_hit.w) { best_hit = faucet; }
    } else if (mat == 43u) {
        // Room 1 — Dining Table + Tucked Chair + Dinner Plate
        let top = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.44, 0.14), vec3<f32>(0.92, 0.52, 0.88));
        if (top.w > 0.0 && top.w < best_hit.w) { best_hit = top; }
        let ped = intersect_aabb_normal(lro, rd, vec3<f32>(0.36, 0.0, 0.38), vec3<f32>(0.64, 0.44, 0.64));
        if (ped.w > 0.0 && ped.w < best_hit.w) { best_hit = ped; }
        let chair = intersect_aabb_normal(lro, rd, vec3<f32>(0.24, 0.0, 0.02), vec3<f32>(0.76, 0.76, 0.22));
        if (chair.w > 0.0 && chair.w < best_hit.w) { best_hit = chair; }
        let plate = intersect_aabb_normal(lro, rd, vec3<f32>(0.34, 0.52, 0.38), vec3<f32>(0.66, 0.56, 0.68));
        if (plate.w > 0.0 && plate.w < best_hit.w) { best_hit = plate; }
    } else if (mat == 44u) {
        // Room 2 — Office Bookshelf & Storage Cabinet
        let shelf = intersect_aabb_normal(lro, rd, vec3<f32>(0.06, 0.0, 0.08), vec3<f32>(0.94, 0.94, 0.62));
        if (shelf.w > 0.0 && shelf.w < best_hit.w) { best_hit = shelf; }
    } else if (mat == 45u) {
        // Room 3 — Shower Stall (Ceramic Base Tray + Frosted Glass Enclosure + Chrome Showerhead)
        let tray = intersect_aabb_normal(lro, rd, vec3<f32>(0.06, 0.0, 0.06), vec3<f32>(0.94, 0.14, 0.94));
        if (tray.w > 0.0 && tray.w < best_hit.w) { best_hit = tray; }
        let glass = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.14, 0.08), vec3<f32>(0.92, 0.96, 0.92));
        if (glass.w > 0.0 && glass.w < best_hit.w) { best_hit = glass; }
    } else if (mat == 46u) {
        // Room 3 — Sculpted White Porcelain Toilet (Bowl + Rear Tank)
        let bowl = intersect_aabb_normal(lro, rd, vec3<f32>(0.20, 0.0, 0.18), vec3<f32>(0.80, 0.42, 0.78));
        if (bowl.w > 0.0 && bowl.w < best_hit.w) { best_hit = bowl; }
        let tank = intersect_aabb_normal(lro, rd, vec3<f32>(0.54, 0.42, 0.20), vec3<f32>(0.88, 0.80, 0.76));
        if (tank.w > 0.0 && tank.w < best_hit.w) { best_hit = tank; }
    } else if (mat == 47u) {
        // Room 3 — Bathroom Sink + Vanity Cabinet + Wall Mirror
        let vanity = intersect_aabb_normal(lro, rd, vec3<f32>(0.12, 0.0, 0.08), vec3<f32>(0.88, 0.68, 0.68));
        if (vanity.w > 0.0 && vanity.w < best_hit.w) { best_hit = vanity; }
        let mirror = intersect_aabb_normal(lro, rd, vec3<f32>(0.18, 0.68, 0.06), vec3<f32>(0.82, 0.96, 0.18));
        if (mirror.w > 0.0 && mirror.w < best_hit.w) { best_hit = mirror; }
    } else if (mat == 48u) {
        // Room 4 — Cozy Low-Poly Couch / Sofa (Seat Cushions + Backrest + Side Armrests)
        let seat = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.10), vec3<f32>(0.98, 0.38, 0.86));
        if (seat.w > 0.0 && seat.w < best_hit.w) { best_hit = seat; }
        let back = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.38, 0.08), vec3<f32>(0.98, 0.74, 0.32));
        if (back.w > 0.0 && back.w < best_hit.w) { best_hit = back; }
        if (mapPos.x == 10) {
            let arm_l = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.38, 0.10), vec3<f32>(0.22, 0.56, 0.86));
            if (arm_l.w > 0.0 && arm_l.w < best_hit.w) { best_hit = arm_l; }
        } else {
            let arm_r = intersect_aabb_normal(lro, rd, vec3<f32>(0.78, 0.38, 0.10), vec3<f32>(0.98, 0.56, 0.86));
            if (arm_r.w > 0.0 && arm_r.w < best_hit.w) { best_hit = arm_r; }
        }
    } else if (mat == 49u) {
        // Room 4 — TV on Media Furniture + VideoGame Console
        let stand = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.32), vec3<f32>(0.98, 0.36, 0.92));
        if (stand.w > 0.0 && stand.w < best_hit.w) { best_hit = stand; }
        let tv = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.38, 0.55), vec3<f32>(0.92, 0.88, 0.73));
        if (tv.w > 0.0 && tv.w < best_hit.w) { best_hit = tv; }
        let console_box = intersect_aabb_normal(lro, rd, vec3<f32>(0.62, 0.36, 0.34), vec3<f32>(0.88, 0.46, 0.52));
        if (console_box.w > 0.0 && console_box.w < best_hit.w) { best_hit = console_box; }
    } else if (mat == 50u) {
        // Room 4 — Sound System Tower Speakers
        let spk = intersect_aabb_normal(lro, rd, vec3<f32>(0.22, 0.0, 0.36), vec3<f32>(0.78, 0.80, 0.88));
        if (spk.w > 0.0 && spk.w < best_hit.w) { best_hit = spk; }
    } else if (mat == 51u) {
        // Room 5 — Nightstand + Glowing Bedside Lamp
        let tbl = intersect_aabb_normal(lro, rd, vec3<f32>(0.20, 0.0, 0.20), vec3<f32>(0.80, 0.46, 0.80));
        if (tbl.w > 0.0 && tbl.w < best_hit.w) { best_hit = tbl; }
        let shade = intersect_aabb_normal(lro, rd, vec3<f32>(0.30, 0.46, 0.30), vec3<f32>(0.70, 0.78, 0.70));
        if (shade.w > 0.0 && shade.w < best_hit.w) { best_hit = shade; }
    } else if (mat == 52u) {
        // Room 5 — Walnut Wardrobe / Dresser
        let ward = intersect_aabb_normal(lro, rd, vec3<f32>(0.08, 0.0, 0.16), vec3<f32>(0.92, 0.94, 0.92));
        if (ward.w > 0.0 && ward.w < best_hit.w) { best_hit = ward; }
    } else if (mat == 53u) {
        // Room 5 — Full-Length Standing Mirror
        let mir = intersect_aabb_normal(lro, rd, vec3<f32>(0.16, 0.0, 0.38), vec3<f32>(0.84, 0.92, 0.58));
        if (mir.w > 0.0 && mir.w < best_hit.w) { best_hit = mir; }
    } else if (mat == 54u) {
        // Room 6 — Garage Workbench + Red Machinist Vise
        let bench = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.0, 0.12), vec3<f32>(0.96, 0.76, 0.90));
        if (bench.w > 0.0 && bench.w < best_hit.w) { best_hit = bench; }
        let vise = intersect_aabb_normal(lro, rd, vec3<f32>(0.20, 0.76, 0.16), vec3<f32>(0.48, 0.92, 0.44));
        if (vise.w > 0.0 && vise.w < best_hit.w) { best_hit = vise; }
    } else if (mat == 55u) {
        // Room 6 — Tool-Hanging Pegboard Wall + Bench
        let bench = intersect_aabb_normal(lro, rd, vec3<f32>(0.02, 0.0, 0.14), vec3<f32>(0.98, 0.74, 0.90));
        if (bench.w > 0.0 && bench.w < best_hit.w) { best_hit = bench; }
        let peg = intersect_aabb_normal(lro, rd, vec3<f32>(0.04, 0.50, 0.72), vec3<f32>(0.96, 0.98, 0.94));
        if (peg.w > 0.0 && peg.w < best_hit.w) { best_hit = peg; }
    } else if (mat == 56u) {
        // Room 6 — Heavy-Duty Garage Storage Rack
        let rack = intersect_aabb_normal(lro, rd, vec3<f32>(0.06, 0.0, 0.12), vec3<f32>(0.94, 0.95, 0.90));
        if (rack.w > 0.0 && rack.w < best_hit.w) { best_hit = rack; }
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
        // Room 2 — Office Desk + PC Tower + Dual Low-Poly Monitors + Swivel Office Chair
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

// Sub-voxel vertical slab height (1.0 = full 1m block; < 0.99 = low ground feature)
fn get_voxel_max_local_y(mat: u32) -> f32 {
    if (mat == 18u) { return 0.05; }
    if (mat == 20u || mat == 21u || mat == 22u || mat == 25u) { return 0.06; }
    if (mat == 26u) { return 0.14; }
    if (mat == 19u) { return 0.18; }
    if (mat == 34u) { return 0.38; }
    if (mat == 33u) { return 0.55; }
    return 1.0;
}

// 16-Step Directional Sun Shadow Ray (respects Lot 402 roof cutaway while keeping neighbor roofs sealed)
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
        let s_cutaway = select(32, z_cutaway, is_player_lot_402(sPos));
        if (sPos.y >= s_cutaway || sPos.y > 16 || sPos.y < 0) {
            break;
        }
        let sm = get_voxel_material(sPos);
        if (sm == 35u || sm == 36u) {
            shadow_atten = min(shadow_atten, 0.52);
        } else if (sm != 0u && sm < 26u && get_voxel_max_local_y(sm) >= 0.99) {
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
        let local_cutaway = select(32, z_cutaway_level, is_player_lot_402(mapPos));
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
                       mat == 23u || mat == 24u || mat == 32u || (mat >= 35u && mat <= 38u) ||
                       (mat >= 40u && mat <= 57u) || mat == 120u || mat == 122u) {
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
    let hit_lot402 = is_player_lot_402(mapPos);

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
        // Material 4: Context-aware Ground (Lot 402 6-Room Differentiated Floors + Threshold Strips / Garage Slab / Patio / Backyard Lawn)
        if (hit_lot402 && mapPos.x >= 8 && mapPos.x <= 12 && mapPos.z >= 11 && mapPos.z <= 14) {
            // Room 6 — Garage: Smooth Epoxy-Grey Workshop Concrete Slab
            let slab_joint = select(0.0, 1.0, fract(hit_pos.x * 0.5) < 0.025 || fract(hit_pos.z * 0.5) < 0.025);
            base_color = mix(vec3<f32>(0.54, 0.57, 0.60), vec3<f32>(0.38, 0.40, 0.43), slab_joint * 0.55);
            roughness = 0.32;
            metallic = 0.10;
        } else if (absX_hit >= 9 && absX_hit <= 17 && lotZ_hit >= 2 && lotZ_hit <= 10) {
            // Crisp low-poly dark oak interior room partition threshold strips on Lot 402
            let is_part_x = hit_lot402 && ((abs(hit_pos.x - 13.0) < 0.045 && hit_pos.z >= 3.0 && hit_pos.z <= 6.0) ||
                                           (abs(hit_pos.x - 15.0) < 0.045 && hit_pos.z >= 3.0 && hit_pos.z <= 6.0) ||
                                           (abs(hit_pos.x - 13.5) < 0.045 && hit_pos.z >= 6.8 && hit_pos.z <= 10.0));
            let is_part_z = hit_lot402 && ((abs(hit_pos.z - 5.95) < 0.045 || abs(hit_pos.z - 6.95) < 0.045) &&
                                           hit_pos.x >= 9.8 && hit_pos.x <= 17.0);
            if (is_part_x || is_part_z) {
                base_color = vec3<f32>(0.34, 0.22, 0.12);
                roughness = 0.26;
            } else if (hit_lot402 && mapPos.x >= 10 && mapPos.x <= 12 && mapPos.z >= 3 && mapPos.z <= 5) {
                // Room 1 — Kitchen + Dining: Warm Terracotta & Cream Checkerboard Ceramic Tile
                let grout = select(0.0, 1.0, fract(hit_pos.x * 2.0) < 0.07 || fract(hit_pos.z * 2.0) < 0.07);
                let chk = (i32(floor(hit_pos.x * 2.0)) + i32(floor(hit_pos.z * 2.0))) & 1;
                let tile_c = select(vec3<f32>(0.88, 0.78, 0.66), vec3<f32>(0.76, 0.44, 0.30), chk == 0);
                base_color = mix(tile_c, vec3<f32>(0.48, 0.42, 0.36), grout * 0.65);
                roughness = 0.20;
                metallic = 0.08;
            } else if (hit_lot402 && mapPos.x >= 15 && mapPos.x <= 16 && mapPos.z >= 3 && mapPos.z <= 5) {
                // Room 3 — Bathroom: Clean Aqua-White Ceramic Bathroom Tile
                let grout = select(0.0, 1.0, fract(hit_pos.x * 3.0) < 0.08 || fract(hit_pos.z * 3.0) < 0.08);
                let b_chk = (i32(floor(hit_pos.x * 3.0)) + i32(floor(hit_pos.z * 3.0))) & 1;
                let aqua_tile = select(vec3<f32>(0.88, 0.95, 0.96), vec3<f32>(0.68, 0.88, 0.90), b_chk == 0);
                base_color = mix(aqua_tile, vec3<f32>(0.56, 0.68, 0.72), grout * 0.55);
                roughness = 0.14;
                metallic = 0.12;
            } else {
                // Warm Honey-Oak Hardwood Planks (Office, Hallway, Living Space & Bedroom)
                let plank_seam = select(0.0, 1.0, fract(hit_pos.x * 3.0) < 0.06 || fract(hit_pos.z * 0.75) < 0.04);
                let grain = sin(hit_pos.z * 14.0 + floor(hit_pos.x * 3.0) * 2.3) * 0.025;
                var oak_col = mix(vec3<f32>(0.68, 0.48, 0.28) + vec3<f32>(grain), vec3<f32>(0.44, 0.29, 0.16), plank_seam * 0.60);
                roughness = 0.28;
                metallic = 0.06;
                // Cozy woven area rug in Living Space (under Couch & TV)
                if (hit_lot402 && hit_pos.x >= 10.15 && hit_pos.x <= 12.75 && hit_pos.z >= 7.25 && hit_pos.z <= 9.25) {
                    let rug_border = hit_pos.x < 10.32 || hit_pos.x > 12.58 || hit_pos.z < 7.42 || hit_pos.z > 9.08;
                    oak_col = select(vec3<f32>(0.76, 0.36, 0.24), vec3<f32>(0.90, 0.78, 0.56), rug_border);
                    roughness = 0.68;
                }
                // Soft woven area rug in Bedroom
                if (hit_lot402 && hit_pos.x >= 14.55 && hit_pos.x <= 16.75 && hit_pos.z >= 7.35 && hit_pos.z <= 9.35) {
                    let b_border = hit_pos.x < 14.70 || hit_pos.x > 16.60 || hit_pos.z < 7.50 || hit_pos.z > 9.20;
                    oak_col = select(vec3<f32>(0.34, 0.48, 0.64), vec3<f32>(0.84, 0.86, 0.90), b_border);
                    roughness = 0.68;
                }
                base_color = oak_col;
            }
        } else if (mapPos.x >= 18 && mapPos.x <= 19 && mapPos.z >= 4 && mapPos.z <= 8) {
            let paver_seam = select(0.0, 1.0, fract(hit_pos.x * 2.0) < 0.06 || fract(hit_pos.z * 2.0) < 0.06);
            base_color = mix(vec3<f32>(0.74, 0.70, 0.64), vec3<f32>(0.52, 0.48, 0.44), paver_seam * 0.60);
            roughness = 0.48;
        } else if (mapPos.x >= 18 && mapPos.x <= 25 && mapPos.z >= 1 && mapPos.z <= 15) {
            let checker = f32((mapPos.x + mapPos.z) & 1) * 0.028;
            let turf_var = sin(hit_pos.x * 5.0) * cos(hit_pos.z * 5.0) * 0.025;
            base_color = vec3<f32>(0.32, 0.60, 0.26) + vec3<f32>(checker + turf_var);
            roughness = 0.68;
        } else {
            let stripe = sin(hit_pos.x * 1.5708) * 0.022;
            let lawn_var = sin(hit_pos.x * 4.0) * cos(hit_pos.z * 4.0) * 0.020;
            base_color = vec3<f32>(0.36, 0.63, 0.29) + vec3<f32>(stripe + lawn_var);
            roughness = 0.72;
        }
    } else if (hit_mat == 5u || hit_mat == 30u) {
        if (hit_mat == 30u) {
            // Compact Home-Office Router / NAS Unit
            base_color = vec3<f32>(0.22, 0.26, 0.32);
            roughness = 0.28;
            metallic = 0.65;
        } else {
            // Warm Cream/Ivory Architectural Clapboard Siding with Crisp White Corner Trim & Baseboard Plinth
            let clapboard_rib = fract(hit_pos.y * 4.0);
            let rib_shadow = smoothstep(0.0, 0.18, clapboard_rib);
            let is_corner_trim = ((absX_hit == 9 || absX_hit == 17) && (lotZ_hit == 2 || lotZ_hit == 10)) ||
                                 (hit_lot402 && (mapPos.x == 8 || mapPos.x == 12) && mapPos.z == 14);
            let is_plinth = mapPos.y == 1 && hit_frac.y < 0.18;
            let wall_cream = select(vec3<f32>(0.92, 0.89, 0.83), vec3<f32>(0.86, 0.89, 0.84), mapPos.x < 0);
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
        let roof_palette = select(vec3<f32>(0.72, 0.34, 0.24), vec3<f32>(0.28, 0.36, 0.48), hit_lot402);
        base_color = roof_palette * shingle_line;
        roughness = 0.50;
        metallic = 0.08;
    } else if (hit_mat == 7u) {
        let is_sill_or_frame = hit_frac.y < 0.15 || hit_frac.y > 0.86 || hit_frac.x < 0.10 || hit_frac.x > 0.90 || hit_frac.z < 0.10 || hit_frac.z > 0.90 || abs(hit_frac.x - 0.5) < 0.04 || abs(hit_frac.z - 0.5) < 0.04;
        let pane_col = select(vec3<f32>(0.72, 0.58, 0.38), vec3<f32>(0.24, 0.46, 0.64), hit_lot402);
        base_color = select(pane_col, vec3<f32>(0.95, 0.94, 0.91), is_sill_or_frame);
        roughness = select(0.14, 0.42, is_sill_or_frame);
        metallic = select(0.75, 0.05, is_sill_or_frame);
        if (!is_sill_or_frame) {
            emissive = select(vec3<f32>(0.34, 0.24, 0.11), vec3<f32>(0.22, 0.16, 0.08), hit_lot402);
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
        let furrow = sin(hit_pos.z * 18.85);
        let is_foliage = furrow > -0.15 && abs(hit_frac.x - 0.5) < 0.42;
        base_color = select(vec3<f32>(0.28, 0.20, 0.14), vec3<f32>(0.32, 0.64, 0.22) * (0.90 + 0.10 * furrow), is_foliage);
        roughness = select(0.75, 0.52, is_foliage);
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
        // Room 1 — MAT_OVEN_STOVE: Brushed Stainless Stove + Dark Glass Oven Window + 4 Glowing Burners + Stockpot
        let is_pot = hit_frac.y > 0.825;
        let is_cooktop = hit_frac.y >= 0.78 && !is_pot;
        let is_oven_glass = hit_frac.z > 0.85 && hit_frac.y > 0.18 && hit_frac.y < 0.64 && hit_frac.x > 0.16 && hit_frac.x < 0.84;
        if (is_pot) {
            let is_broth = hit_frac.y > 0.95;
            base_color = select(vec3<f32>(0.78, 0.80, 0.84), vec3<f32>(0.94, 0.68, 0.20), is_broth);
            roughness = 0.22;
            metallic = select(0.85, 0.15, is_broth);
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
        // Room 6 — MAT_GARAGE_WORKBENCH: Steel Frame + Butcher-Block Top + Red Machinist Vise
        if (hit_frac.y > 0.765) {
            base_color = vec3<f32>(0.86, 0.20, 0.18);
            roughness = 0.28;
            metallic = 0.65;
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
        let row_pat = 0.88 + 0.12 * sin(hit_pos.z * 12.0);
        let crop_clusters = step(0.35, sin(hit_pos.x * 16.0) * cos(hit_pos.z * 16.0));
        let is_top_crown = hit_normal.y > 0.5 || hit_frac.y > 0.72;
        let foliage_col = vec3<f32>(0.36, 0.62, 0.18) * row_pat;
        let ripe_head_col = vec3<f32>(0.96, 0.74, 0.16);
        base_color = select(foliage_col, mix(foliage_col, ripe_head_col, 0.65 + 0.35 * crop_clusters), is_top_crown);
        roughness = 0.48;
        emissive = select(vec3<f32>(0.0), vec3<f32>(0.18, 0.12, 0.02), is_top_crown);
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
    if (hit_mat < 26u && hit_mat != 6u && hit_mat != 17u && mapPos.y >= 1) {
        let ex = min(hit_frac.x, 1.0 - hit_frac.x);
        let ey = min(hit_frac.y, 1.0 - hit_frac.y);
        let ez = min(hit_frac.z, 1.0 - hit_frac.z);
        let min_e = min(max(ex, ey), min(max(ey, ez), max(ex, ez)));
        edge_ao = smoothstep(0.0, 0.05, min_e) * 0.10 + 0.90;
        if (mapPos.y == 1 && abs(hit_normal.y) < 0.5 && hit_frac.y < 0.15) {
            edge_ao = edge_ao * mix(0.74, 1.0, hit_frac.y / 0.15);
        }
    }

    let sun_shadow = select(trace_sun_shadow(hit_pos + hit_normal * 0.03, light_dir, z_cutaway_level), 1.0, (hit_mat >= 26u && hit_mat <= 57u) || hit_mat == 6u || hit_mat == 17u);
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
        } else if (hit_mat == 16u || hit_mat == 22u || hit_mat == 41u) {
            lit_color = mix(lit_color, vec3<f32>(1.00, 0.50, 0.10), 0.75);
        } else if (hit_mat == 23u || hit_mat == 21u || hit_mat == 5u || hit_mat == 6u ||
                   hit_mat == 13u || hit_mat == 14u || hit_mat == 15u || hit_mat == 17u || hit_mat == 57u) {
            lit_color = mix(lit_color, vec3<f32>(0.15, 0.92, 0.55), 0.65);
        } else {
            lit_color = lit_color * 0.45;
        }
    } else if (overlay_mode == 2u) {
        if (hit_mat == 11u || hit_mat == 8u || hit_mat == 9u || hit_mat == 120u || hit_mat == 49u) {
            lit_color = mix(lit_color, vec3<f32>(1.00, 0.85, 0.15), 0.70);
        } else if (hit_mat == 12u || hit_mat == 18u || hit_mat == 19u || hit_mat == 34u ||
                   hit_mat == 42u || hit_mat == 45u || hit_mat == 46u || hit_mat == 47u) {
            lit_color = mix(lit_color, vec3<f32>(0.00, 0.92, 1.00), 0.70);
        } else {
            lit_color = lit_color * 0.45;
        }
    } else if (overlay_mode == 3u) {
        if (hit_mat == 20u || hit_mat == 24u || hit_mat == 33u || hit_mat == 34u ||
            (hit_mat >= 40u && hit_mat <= 44u) || (hit_mat >= 54u && hit_mat <= 56u)) {
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

    // Roof / Wall Cutaway top-face architectural cross-section cap (ONLY on Player Lot 402!)
    if (cutaway_active && hit_lot402 && mapPos.y == i32(floor(camera.camera_target.w + 0.5)) - 1 && hit_normal.y > 0.5) {
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

    // Distance fog blend into horizon & Neighbor Fog Envelope (Lot 402 stays crystal-clear)
    let sky = get_sky_color(rd);
    var fog = 1.0 - exp(-hit_dist * 0.0075);
    if (!hit_lot402 && abs(mapPos.x) >= 6) {
        let neighbor_fog = clamp(fog + 0.42, 0.0, 0.86);
        fog = neighbor_fog;
    }
    let toned_color = aces_tonemap(lit_color * 1.12);
    let final_color = mix(toned_color, sky, fog);

    return vec4<f32>(final_color, 1.0);
}
