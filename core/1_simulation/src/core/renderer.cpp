#include "renderer.hpp"
#include <random>

namespace oasis {

const char* Renderer::wgsl_source_ = R"(
struct Voxel {
    data: u32,
};
struct Entity {
    pos: vec3<f32>,
    type_and_state: f32,
};
@group(0) @binding(0) var<storage, read> voxels: array<Voxel>;
@group(0) @binding(1) var<storage, read> entities: array<Entity>;

struct Uniforms {
    boundary_center: vec3<f32>,
    boundary_radius: f32,
    time: f32,
};
@group(0) @binding(2) var<uniform> uniforms: Uniforms;

@vertex
fn vs_main(@builtin(vertex_index) VertexIndex : u32) -> @builtin(position) vec4<f32> {
    var pos = array<vec2<f32>, 4>(
        vec2<f32>(-1.0, 1.0), vec2<f32>(-1.0, -1.0),
        vec2<f32>(1.0, 1.0), vec2<f32>(1.0, -1.0)
    );
    return vec4<f32>(pos[VertexIndex], 0.0, 1.0);
}

fn hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(12.9898, 78.233))) * 43758.5453);
}

@fragment
fn fs_main(@builtin(position) coord: vec4<f32>) -> @location(0) vec4<f32> {
    let grid_x = u32(coord.x / 25.0);
    let grid_z = u32(coord.y / 25.0);
    
    if (grid_x >= 32u || grid_z >= 32u) { return vec4<f32>(0.0, 0.0, 0.0, 1.0); }

    let idx = grid_x + (16u * 32u) + (grid_z * 32u * 32u);
    let v = voxels[idx].data;
    let temp = f32((v >> 8u) & 255u);
    
    let dx = f32(grid_x) - uniforms.boundary_center.x;
    let dz = f32(grid_z) - uniforms.boundary_center.z;
    let dist = sqrt(dx*dx + dz*dz);
    
    // Thermodynamics: Cold=Blue, Hot=Red
    var color = vec3<f32>(temp / 255.0, 0.0, 1.0 - (temp / 255.0)); 
    
    if (dist > uniforms.boundary_radius) {
        // FOG OF LEGACY
        let noise = hash(coord.xy + uniforms.time);
        color = mix(color, vec3<f32>(0.2, 0.2, 0.2), noise * 0.9);
    }

    return vec4<f32>(color, 1.0);
}
)";

Renderer::Renderer(SDL_Renderer* sdl_renderer) : sdl_renderer_(sdl_renderer) {}

void Renderer::Sync(const ChunkManager& chunk_mgr, const EntityManager& entity_mgr) {
    buffer_sync_.Sync(chunk_mgr, entity_mgr);
}

void Renderer::Render(const SovereignBoundary& boundary, uint64_t tick_count) {
    // Currently executing the Native Fallback to avoid the heavy Dawn toolchain locally.
    // The WGSL string above is passed to the Emscripten WebGPU compiler in WASM mode.
    RenderNativeFallback(boundary, tick_count);
}

void Renderer::RenderNativeFallback(const SovereignBoundary& boundary, uint64_t tick_count) {
    const auto& voxels = buffer_sync_.GetVoxelBuffer();
    int cell_size = 800 / 32; // Fit 32x32 grid into 800x800 window
    
    std::mt19937 rng(tick_count); // Noise for the Fog
    std::uniform_int_distribution<int> noise(0, 100);

    for (int x = 0; x < 32; ++x) {
        for (int z = 0; z < 32; ++z) {
            int idx = x + (16 * 32) + (z * 32 * 32); // Y=16 slice
            if (idx >= voxels.size()) continue;

            uint32_t data = voxels[idx].packed_data;
            uint8_t temp = (data >> 8) & 0xFF;

            SDL_Rect rect{ x * cell_size, z * cell_size, cell_size, cell_size };

            if (boundary.IsInside(x, 16, z)) {
                // Strict Managed Rendering
                SDL_SetRenderDrawColor(sdl_renderer_, temp, 0, 255 - temp, 255);
            } else {
                // Fog of Legacy Rendering (Noisy Haze)
                int n = noise(rng);
                SDL_SetRenderDrawColor(sdl_renderer_, n, n, n, 255);
            }
            SDL_RenderFillRect(sdl_renderer_, &rect);
        }
    }

    // Render Entities
    const auto& ents = buffer_sync_.GetEntityBuffer();
    for (const auto& e : ents) {
        if (e.y != 16) continue; // Only draw on this slice
        
        SDL_Rect rect{ static_cast<int>(e.x) * cell_size + 4, static_cast<int>(e.z) * cell_size + 4, cell_size - 8, cell_size - 8 };
        
        if (e.type_and_state >= 1.0f) {
            // Citizen (Gold)
            SDL_SetRenderDrawColor(sdl_renderer_, 255, 215, 0, 255);
        } else {
            // NPC (Purple)
            SDL_SetRenderDrawColor(sdl_renderer_, 128, 0, 128, 255);
        }
        SDL_RenderFillRect(sdl_renderer_, &rect);
    }
}

} // namespace oasis
