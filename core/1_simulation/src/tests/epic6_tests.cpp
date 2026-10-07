#include <catch2/catch_test_macros.hpp>
#include "../core/entity_manager.hpp"
#include "../core/blueprint_manager.hpp"
#include "../core/blueprint_exporter.hpp"
#include "../core/sovereign_boundary.hpp"
#include "../core/did_crypto_generator.hpp"

using namespace oasis;

TEST_CASE("Story 6.2 & 6.3: Layer 7 Capture and Onboarding", "[capture][ecology]") {
    EntityManager em;
    BlueprintManager blueprints;
    DIDCryptoGenerator crypto;
    SovereignBoundary boundary{16, 16, 16, 100}; // Large boundary to test pure discrete logic

    // Place a Service Kiosk at (20, 16, 20)
    blueprints.AddZone(Zone{20, 16, 20, 1, 1, 1, ZoneType::SERVICE_KIOSK});
    
    // Spawn an NPC at (18, 16, 18)
    em.AddNPC(18, 16, 18);
    auto& entities = em.GetEntities();
    
    // 50 ticks to drop hydration to <50, putting them in SEEKING_SERVICE state
    for(int i=0; i<51; i++) em.Tick(boundary, blueprints, crypto);
    
    REQUIRE(entities[0].state == EntityState::SEEKING_SERVICE);
    
    // They should now pathfind to (20, 16, 20) and consume the service.
    // It takes 2 steps (dx=2, dz=2 => 2 ticks for x, 2 ticks for z? Actually Tick modifies x,y,z simultaneously)
    // So 2 ticks to arrive. 1 tick to consume and reset hydration, incrementing trust by 5.
    for(int i=0; i<3; i++) em.Tick(boundary, blueprints, crypto);
    
    REQUIRE(entities[0].x == 20);
    REQUIRE(entities[0].z == 20);
    REQUIRE(entities[0].hydration == 100);
    REQUIRE(entities[0].trust == 5);
    REQUIRE(entities[0].state == EntityState::IDLE);
    
    // Fast forward! If we force their trust to 250, then simulate the next service consumption...
    entities[0].trust = 250;
    
    // Drain hydration again
    for(int i=0; i<51; i++) em.Tick(boundary, blueprints, crypto);
    
    // They are already at the kiosk coordinates, so 1 more tick consumes it immediately
    em.Tick(boundary, blueprints, crypto);
    
    // Trust hit 255. Onboarding should have occurred!
    REQUIRE(entities[0].type == EntityType::CITIZEN);
    REQUIRE(entities[0].state == EntityState::WORKING);
    // They should now possess a cryptographic DID
    REQUIRE(entities[0].did.find("did:key:") == 0);
}

TEST_CASE("Story 6.4: Real Intent BPMN Compiler", "[compiler][xml]") {
    BlueprintManager blueprints;
    blueprints.AddZone(Zone{10, 0, 10, 5, 5, 5, ZoneType::SOLAR_MESH});
    blueprints.AddZone(Zone{20, 0, 20, 2, 2, 2, ZoneType::SERVICE_KIOSK});

    std::string xml = BlueprintExporter::ExportToBPMN(blueprints);
    
    // Validate it generated BPMN 2.0 definitions
    REQUIRE(xml.find("<bpmn2:definitions") != std::string::npos);
    REQUIRE(xml.find("id=\"Process_Oasis_Infrastructure\"") != std::string::npos);
    // Validate it compiled the zones into tasks
    REQUIRE(xml.find("name=\"SOLAR_EXERGY_ROUTING\"") != std::string::npos);
    REQUIRE(xml.find("name=\"LAYER7_SERVICE_KIOSK\"") != std::string::npos);
    // Validate spatial coordinates were embedded
    REQUIRE(xml.find("oasis:x=\"10\"") != std::string::npos);
}
