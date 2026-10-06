#include "thermodynamic_engine.hpp"

namespace oasis {

ThermodynamicEngine::ThermodynamicEngine(ChunkManager& chunk_mgr)
    : chunk_manager_(chunk_mgr), rng_(42), tick_count_(0) {} // Seeded for basic determinism

void ThermodynamicEngine::Tick(const SovereignBoundary& boundary) {
    auto& grid = chunk_manager_.GetGrid();
    if (grid.empty()) return;
    
    tick_count_++;
    std::uniform_int_distribution<int> chance(0, 100);
    std::uniform_int_distribution<int> temp_spike(50, 255);

    for (size_t i = 0; i < grid.size(); ++i) {
        Voxel& v = grid[i];
        int x, y, z;
        chunk_manager_.GetCoords(i, x, y, z);
        
        if (boundary.IsInside(x, y, z)) {
            // ==========================================
            // STRICT SIMULATION: Inside the Trust Ring
            // ==========================================
            if (v.temperature > 22) {
                v.temperature -= 1; // Cool down deterministically
            } else if (v.temperature < 22) {
                v.temperature += 1;
            }
            
            if (v.moisture > 0 && v.temperature > 50) {
                v.moisture -= 1; // High heat accelerates evaporation
            }
        } 
        else {
            // ==========================================
            // LEVEL OF DETAIL: The Fog of Legacy
            // ==========================================
            // We save massive CPU cycles by not running discrete flow logic.
            // Instead, the legacy world is subjected to probabilistic, chaotic pressure.
            
            // Low-frequency tick (e.g., only update 10% of the time) to simulate abstraction
            if (chance(rng_) < 10) {
                // Legacy Pressure: Sudden, chaotic heatwaves / entropy
                if (chance(rng_) < 5) {
                    v.temperature = temp_spike(rng_); 
                } else {
                    // Gradual, randomized decay
                    if (v.temperature > 0) v.temperature -= chance(rng_) % 5;
                    if (v.moisture > 0) v.moisture -= chance(rng_) % 3;
                }
            }
        }
    }
}

} // namespace oasis
