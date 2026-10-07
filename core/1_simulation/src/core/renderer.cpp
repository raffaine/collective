#include "renderer.hpp"
#include <random>

namespace oasis {

const char* Renderer::wgsl_source_ = R"(
// ... [WGSL Shader is kept internal for WASM compile] ...
)";

Renderer::Renderer(SDL_Renderer* sdl_renderer) : sdl_renderer_(sdl_renderer) {}

void Renderer::Sync(const ChunkManager& chunk_mgr, const EntityManager& entity_mgr) {
    buffer_sync_.Sync(chunk_mgr, entity_mgr);
}

void Renderer::Render(const SovereignBoundary& boundary, const BlueprintManager& blueprints, uint64_t tick_count) {
    RenderNativeFallback(boundary, blueprints, tick_count);
}

void Renderer::RenderNativeFallback(const SovereignBoundary& boundary, const BlueprintManager& blueprints, uint64_t tick_count) {
    const auto& voxels = buffer_sync_.GetVoxelBuffer();
    int cell_size = 800 / 32; // 25 pixels
    
    std::mt19937 rng(tick_count); // Static noise effect changes every tick
    std::uniform_int_distribution<int> noise(0, 100);

    // 1. Render Thermodynamic Voxel Grid
    for (int x = 0; x < 32; ++x) {
        for (int z = 0; z < 32; ++z) {
            int idx = x + (16 * 32) + (z * 32 * 32); // Center Y slice
            if (idx >= voxels.size()) continue;

            uint32_t data = voxels[idx].packed_data;
            uint8_t temp = (data >> 8) & 0xFF;

            SDL_Rect rect{ x * cell_size, z * cell_size, cell_size, cell_size };

            if (boundary.IsInside(x, 16, z)) {
                // Strict Managed Rendering (Heatmap: Cold=Blue, Hot=Red)
                SDL_SetRenderDrawColor(sdl_renderer_, temp, 0, 255 - temp, 255);
            } else {
                // Fog of Legacy Rendering (Noisy Haze)
                int n = noise(rng);
                // Mix the actual heat with static noise
                SDL_SetRenderDrawColor(sdl_renderer_, (temp/2) + n, n/2, n, 255);
            }
            SDL_RenderFillRect(sdl_renderer_, &rect);
        }
    }

    // 2. Render Blueprint Zones
    for (const auto& zone : blueprints.GetZones()) {
        if (zone.y != 16) continue; // Only draw on this slice
        SDL_Rect rect{ zone.x * cell_size, zone.z * cell_size, zone.w * cell_size, zone.d * cell_size };
        
        if (zone.type == ZoneType::SERVICE_KIOSK) {
            SDL_SetRenderDrawColor(sdl_renderer_, 0, 255, 255, 255); // Cyan Outline for Kiosk
        } else if (zone.type == ZoneType::SOLAR_MESH) {
            SDL_SetRenderDrawColor(sdl_renderer_, 255, 255, 0, 255); // Yellow Outline for Solar
        } else {
            SDL_SetRenderDrawColor(sdl_renderer_, 0, 255, 0, 255);
        }
        
        // Draw Hollow Box for zone
        SDL_RenderDrawRect(sdl_renderer_, &rect);
        // Draw inner border for thickness
        rect.x += 1; rect.y += 1; rect.w -= 2; rect.h -= 2;
        SDL_RenderDrawRect(sdl_renderer_, &rect);
    }

    // 3. Render Entities
    const auto& ents = buffer_sync_.GetEntityBuffer();
    for (const auto& e : ents) {
        if (e.y != 16) continue;
        
        SDL_Rect rect{ static_cast<int>(e.x) * cell_size + 6, static_cast<int>(e.z) * cell_size + 6, cell_size - 12, cell_size - 12 };
        
        if (e.type_and_state >= 1.0f) {
            // Citizen (Gold)
            SDL_SetRenderDrawColor(sdl_renderer_, 255, 215, 0, 255);
        } else {
            // NPC (Purple)
            SDL_SetRenderDrawColor(sdl_renderer_, 255, 0, 255, 255);
        }
        SDL_RenderFillRect(sdl_renderer_, &rect);
        
        // Black border around entity
        SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, 255);
        SDL_RenderDrawRect(sdl_renderer_, &rect);
    }
}

} // namespace oasis
