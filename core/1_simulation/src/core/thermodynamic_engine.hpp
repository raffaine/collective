#pragma once
#include "chunk_manager.hpp"
#include "sovereign_boundary.hpp"
#include <random>

namespace oasis {

class ThermodynamicEngine {
public:
    ThermodynamicEngine(ChunkManager& chunk_mgr);
    
    // Simulates one frame of physics, applying LoD based on the boundary
    void Tick(const SovereignBoundary& boundary);

private:
    ChunkManager& chunk_manager_;
    std::mt19937 rng_;
    uint64_t tick_count_;
};

} // namespace oasis
