#include <iostream>
#include <memory>
#include "core/sdl2_abstraction.hpp"
#include "core/chunk_manager.hpp"
#include "core/thermodynamic_engine.hpp"
#include "core/entity_manager.hpp"
#include "core/sovereign_boundary.hpp"
#include "core/renderer.hpp"

extern "C" {
    bool InjectIntent(const char* json_payload) {
        std::cout << "Received Intent: " << json_payload << "\n";
        return true; 
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Booting Oasis Engine (Layer 1b)...\n";
    
    oasis::SDL2Abstraction app("Oasis Engine: Sovereign Node", 800, 800);
    if (!app.Initialize()) return 1;

    oasis::SovereignBoundary boundary;
    boundary.center_x = 16;
    boundary.center_y = 16;
    boundary.center_z = 16;
    boundary.radius = 8; 

    oasis::ChunkManager chunk_manager;
    chunk_manager.InitializeHeadless();
    oasis::ThermodynamicEngine thermo_engine(chunk_manager);
    
    oasis::EntityManager entity_manager;
    entity_manager.AddCitizen(16, 16, 16); 
    entity_manager.AddNPC(4, 16, 4);

    uint64_t ticks = 0;

    std::cout << "Engine initialized. Sovereign Boundary active.\n";

    app.Run([&]() {
        thermo_engine.Tick(boundary);
        entity_manager.Tick(boundary);
        ticks++;
    }, 
    [&](SDL_Renderer* sdl_renderer) {
        // Initialize renderer on first frame
        static oasis::Renderer renderer(sdl_renderer);
        
        // 1. Flatten memory for GPU
        renderer.Sync(chunk_manager, entity_manager);
        
        // 2. Execute Render Pipeline (Native fallback or WebGPU)
        renderer.Render(boundary, ticks);
    });

    return 0;
}
