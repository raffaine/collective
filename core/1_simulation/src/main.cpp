#include <iostream>
#include <memory>
#include "core/sdl2_abstraction.hpp"
#include "core/chunk_manager.hpp"
#include "core/thermodynamic_engine.hpp"
#include "core/entity_manager.hpp"
#include "core/sovereign_boundary.hpp"
#include "core/renderer.hpp"
#include "core/blueprint_manager.hpp"
#include "core/blueprint_exporter.hpp"
#include "core/did_crypto_generator.hpp"

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

    oasis::DIDCryptoGenerator crypto;
    oasis::SovereignBoundary boundary{16, 16, 16, 10};

    oasis::ChunkManager chunk_manager;
    chunk_manager.InitializeHeadless();
    oasis::ThermodynamicEngine thermo_engine(chunk_manager);
    
    // Initialize Blueprints
    oasis::BlueprintManager blueprints;
    blueprints.AddZone(oasis::Zone{16, 16, 16, 2, 1, 2, oasis::ZoneType::SERVICE_KIOSK});
    blueprints.AddZone(oasis::Zone{10, 16, 10, 4, 1, 4, oasis::ZoneType::SOLAR_MESH});
    
    std::cout << "\n--- REAL INTENT COMPILER START ---\n";
    std::cout << oasis::BlueprintExporter::ExportToBPMN(blueprints);
    std::cout << "--- REAL INTENT COMPILER END ---\n\n";

    oasis::EntityManager entity_manager;
    // Spawn a Swarm of NPCs in the Fog of Legacy!
    entity_manager.AddNPC(2, 16, 2);
    entity_manager.AddNPC(30, 16, 4);
    entity_manager.AddNPC(4, 16, 28);
    entity_manager.AddNPC(28, 16, 28);
    entity_manager.AddNPC(16, 16, 2);
    
    // Spawn one native citizen inside
    entity_manager.AddCitizen(14, 16, 14); 

    uint64_t ticks = 0;
    
    // Throttle logic ticks to 10 FPS so human eyes can watch the simulation unfold
    uint64_t last_tick_time = SDL_GetTicks64();

    app.Run([&]() {
        uint64_t current_time = SDL_GetTicks64();
        if (current_time - last_tick_time > 100) { // 10 ticks per second
            // Inject intense Heat into the Solar Mesh to watch Thermodynamics work!
            oasis::Voxel hot_v = chunk_manager.GetVoxel(12, 16, 12);
            hot_v.temperature = 255; 
            chunk_manager.SetVoxel(12, 16, 12, hot_v);
            
            thermo_engine.Tick(boundary);
            entity_manager.Tick(boundary, blueprints, crypto);
            ticks++;
            last_tick_time = current_time;
        }
    }, 
    [&](SDL_Renderer* sdl_renderer) {
        static oasis::Renderer renderer(sdl_renderer);
        renderer.Sync(chunk_manager, entity_manager);
        renderer.Render(boundary, blueprints, ticks);
    });

    return 0;
}
