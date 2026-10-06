#include <catch2/catch_test_macros.hpp>
#include "../core/chunk_manager.hpp"
#include "../core/thermodynamic_engine.hpp"
#include "../core/entity_manager.hpp"
#include "../core/sovereign_boundary.hpp"

using namespace oasis;

TEST_CASE("Story 4.1: Thermodynamic LoD (Spatial Fog)", "[lod][thermo]") {
    ChunkManager chunk;
    chunk.InitializeHeadless();
    ThermodynamicEngine thermo(chunk);
    
    SovereignBoundary boundary{16, 16, 16, 5};
    
    // Voxel inside boundary
    Voxel v_in{1, 100, 240, 0};
    chunk.SetVoxel(16, 16, 16, v_in);
    
    // Voxel outside boundary (Fog of Legacy)
    Voxel v_out{1, 100, 240, 0};
    chunk.SetVoxel(0, 0, 0, v_out);
    
    // Tick 1 time.
    thermo.Tick(boundary);
    
    Voxel updated_in = chunk.GetVoxel(16, 16, 16);
    Voxel updated_out = chunk.GetVoxel(0, 0, 0);
    
    // Inside is strictly deterministic: MUST cool by exactly 1 degree, and evaporate 1 moisture
    REQUIRE(updated_in.temperature == 239);
    REQUIRE(updated_in.moisture == 99);
    
    // Outside is probabilistic. It most likely didn't tick at all (only 10% chance), 
    // or if it did, it decayed by a random mod amount or spiked.
    // We just verify it does not equal the strict deterministic result.
    bool outside_is_chaotic = (updated_out.temperature != 239) || (updated_out.moisture != 99);
    REQUIRE(outside_is_chaotic);
}

TEST_CASE("Story 4.2 & 4.3: Behavioral LoD & Legacy Pressure", "[lod][ecology]") {
    EntityManager em;
    SovereignBoundary boundary{16, 16, 16, 5};
    
    // NPC inside boundary
    em.AddNPC(16, 16, 16);
    // NPC outside boundary
    em.AddNPC(0, 0, 0);
    
    auto& entities = em.GetEntities();
    REQUIRE(entities[0].state == EntityState::IDLE);
    REQUIRE(entities[1].state == EntityState::IDLE);
    
    // Tick enough times to trigger hydration drop and movement chance
    for(int i = 0; i < 51; i++) em.Tick(boundary);
    
    // Entity 0 (Inside) should be acting deterministically: 
    // Hydration dropped 50 times -> state transitions to SEEKING_SERVICE
    REQUIRE(entities[0].state == EntityState::SEEKING_SERVICE);
    REQUIRE(entities[0].x == 16); // Has not drifted
    
    // Entity 1 (Outside) is in the Fog. It should be DRIFTING.
    REQUIRE(entities[1].state == EntityState::DRIFTING);
    // And due to macro-stats, it should have drifted towards the center (> 0)
    REQUIRE(entities[1].x > 0);
    REQUIRE(entities[1].y > 0);
    REQUIRE(entities[1].z > 0);
}
