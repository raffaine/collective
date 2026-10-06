#pragma once
#include <SDL2/SDL.h>
#include "buffer_sync.hpp"
#include "sovereign_boundary.hpp"

namespace oasis {

// Story 5.2 & 5.3: WebGPU WGSL Shader and Native Fallback
class Renderer {
public:
    Renderer(SDL_Renderer* sdl_renderer);
    
    // Updates internal VRAM arrays
    void Sync(const ChunkManager& chunk_mgr, const EntityManager& entity_mgr);
    
    // Renders the buffers (Native fallback uses SDL2, WASM targets WebGPU)
    void Render(const SovereignBoundary& boundary, uint64_t tick_count);

private:
    SDL_Renderer* sdl_renderer_;
    BufferSync buffer_sync_;

    void RenderNativeFallback(const SovereignBoundary& boundary, uint64_t tick_count);

    // The raw WGSL shader string for the WebGPU pipeline
    static const char* wgsl_source_;
};

} // namespace oasis
