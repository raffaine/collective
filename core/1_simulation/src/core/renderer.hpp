#pragma once
#include <SDL2/SDL.h>
#include "buffer_sync.hpp"
#include "sovereign_boundary.hpp"
#include "blueprint_manager.hpp"

namespace oasis {

class Renderer {
public:
    Renderer(SDL_Renderer* sdl_renderer);
    
    void Sync(const ChunkManager& chunk_mgr, const EntityManager& entity_mgr);
    void Render(const SovereignBoundary& boundary, const BlueprintManager& blueprints, uint64_t tick_count);

private:
    SDL_Renderer* sdl_renderer_;
    BufferSync buffer_sync_;
    void RenderNativeFallback(const SovereignBoundary& boundary, const BlueprintManager& blueprints, uint64_t tick_count);
    static const char* wgsl_source_;
};

} // namespace oasis
