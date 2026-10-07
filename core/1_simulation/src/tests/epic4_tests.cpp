#include <catch2/catch_test_macros.hpp>
#include "../core/chunk_manager.hpp"
#include "../core/thermodynamic_engine.hpp"
#include "../core/entity_manager.hpp"
#include "../core/sovereign_boundary.hpp"
#include "../core/blueprint_manager.hpp"
#include "../core/did_crypto_generator.hpp"

using namespace oasis;

TEST_CASE("Story 4.1: Thermodynamic LoD (Spatial Fog)", "[lod][thermo]") {
    ChunkManager chunk;
    chunk.InitializeHeadless();
    ThermodynamicEngine thermo(chunk);
    SovereignBoundary boundary{16, 16, 16, 5};
    
    Voxel v_in{1, 100, 240, 0};
    chunk.SetVoxel(16, 16, 16, v_in);
    
    Voxel v_out{1, 100, 240, 0};
    chunk.SetVoxel(0, 0, 0, v_out);
    
    thermo.Tick(boundary);
    
    Voxel updated_in = chunk.GetVoxel(16, 16, 16);
    Voxel updated_out = chunk.GetVoxel(0, 0, 0);
    
    REQUIRE(updated_in.temperature == 239);
    REQUIRE(updated_in.moisture == 99);
    
    bool outside_is_chaotic = (updated_out.temperature != 239) || (updated_out.moisture != 99);
    REQUIRE(outside_is_chaotic);
}

TEST_CASE("Story 4.2 & 4.3: Behavioral LoD & Legacy Pressure", "[lod][ecology]") {
    EntityManager em;
    SovereignBoundary boundary{16, 16, 16, 5};
    BlueprintManager bm;
    DIDCryptoGenerator cg;
    
    em.AddNPC(16, 16, 16);
    em.AddNPC(0, 0, 0);
    
    auto& entities = em.GetEntities();
    
    for(int i = 0; i < 51; i++) em.Tick(boundary, bm, cg);
    
    REQUIRE(entities[0].state == EntityState::SEEKING_SERVICE);
    REQUIRE(entities[0].x == 16); 
    
    REQUIRE(entities[1].state == EntityState::DRIFTING);
    REQUIRE(entities[1].x > 0);
    REQUIRE(entities[1].y > 0);
    REQUIRE(entities[1].z > 0);
}
