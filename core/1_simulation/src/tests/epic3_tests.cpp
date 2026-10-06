#include <catch2/catch_test_macros.hpp>
#include "../core/entity_manager.hpp"

using namespace oasis;

TEST_CASE("Story 3.1: DOD Entity Packing", "[ecology][memory]") {
    REQUIRE(sizeof(Entity) <= 12);
}

TEST_CASE("Story 3.2: NPC vs Citizen State Machine", "[ecology][logic]") {
    EntityManager em;
    SovereignBoundary boundary;
    boundary.radius = 100;
    
    em.AddNPC(16, 16, 16);
    REQUIRE(em.GetEntities()[0].type == EntityType::NPC);
    REQUIRE(em.GetEntities()[0].trust == 0);
    
    for(int i=0; i<51; i++) em.Tick(boundary);
    REQUIRE(em.GetEntities()[0].state == EntityState::SEEKING_SERVICE);
    
    em.Tick(boundary);
    REQUIRE(em.GetEntities()[0].state == EntityState::IDLE);
    REQUIRE(em.GetEntities()[0].trust == 5);
    REQUIRE(em.GetEntities()[0].hydration == 100);

    em.AddCitizen(10, 10, 10);
    size_t cit_idx = 1;
    REQUIRE(em.GetEntities()[cit_idx].type == EntityType::CITIZEN);
    REQUIRE(em.GetEntities()[cit_idx].state == EntityState::WORKING);
    
    for(int i=0; i<20; i++) em.Tick(boundary);
    REQUIRE(em.GetEntities()[cit_idx].state == EntityState::RESTING);
    
    for(int i=0; i<10; i++) em.Tick(boundary);
    REQUIRE(em.GetEntities()[cit_idx].state == EntityState::WORKING);
}
