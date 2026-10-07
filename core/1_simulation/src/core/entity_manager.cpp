#include "entity_manager.hpp"
#include <iostream>

namespace oasis {

EntityManager::EntityManager() : rng_(1337) {}

void EntityManager::AddNPC(uint16_t x, uint16_t y, uint16_t z) {
    entities_.push_back(Entity{
        x, y, z, EntityType::NPC, EntityState::IDLE, 22, 100, 0, 0, ""
    });
}

void EntityManager::AddCitizen(uint16_t x, uint16_t y, uint16_t z) {
    entities_.push_back(Entity{
        x, y, z, EntityType::CITIZEN, EntityState::WORKING, 22, 100, 255, 0, "did:oasis:genesis"
    });
}

void EntityManager::Tick(const SovereignBoundary& boundary, const BlueprintManager& blueprints, DIDCryptoGenerator& crypto) {
    std::uniform_int_distribution<int> move_chance(0, 100);

    for (auto& e : entities_) {
        bool in_boundary = boundary.IsInside(e.x, e.y, e.z);

        if (!in_boundary) {
            e.state = EntityState::DRIFTING;
            
            // Fog of Legacy Pressure
            if (move_chance(rng_) < 20) {
                if (e.hydration > 0) e.hydration -= 5;
                if (e.type == EntityType::CITIZEN && e.fatigue < 255) e.fatigue += 10;
                
                // Story 6.2: Pathfind to the nearest Layer 7 Kiosk (or center if none exists)
                int tx = boundary.center_x, ty = boundary.center_y, tz = boundary.center_z;
                blueprints.FindNearest(ZoneType::SERVICE_KIOSK, e.x, e.y, e.z, tx, ty, tz);

                if (e.x < tx) e.x++; else if (e.x > tx) e.x--;
                if (e.y < ty) e.y++; else if (e.y > ty) e.y--;
                if (e.z < tz) e.z++; else if (e.z > tz) e.z--;
            }
        } 
        else {
            if (e.state == EntityState::DRIFTING) {
                e.state = (e.type == EntityType::NPC) ? EntityState::IDLE : EntityState::WORKING;
            }

            if (e.hydration > 0) e.hydration -= 1;
            
            if (e.type == EntityType::NPC) {
                if (e.state == EntityState::IDLE) {
                    if (e.hydration < 50) e.state = EntityState::SEEKING_SERVICE;
                } 
                else if (e.state == EntityState::SEEKING_SERVICE) {
                    int tx, ty, tz;
                    if (blueprints.FindNearest(ZoneType::SERVICE_KIOSK, e.x, e.y, e.z, tx, ty, tz)) {
                        // Move towards the kiosk within the boundary
                        if (e.x != tx || e.y != ty || e.z != tz) {
                            if (e.x < tx) e.x++; else if (e.x > tx) e.x--;
                            if (e.y < ty) e.y++; else if (e.y > ty) e.y--;
                            if (e.z < tz) e.z++; else if (e.z > tz) e.z--;
                        } else {
                            // At the kiosk! Consume service.
                            e.hydration = 100;
                            e.state = EntityState::IDLE;
                            if (e.trust < 255) {
                                e.trust += 5;
                            }
                            
                            // Story 6.3: The Capture & Onboarding
                            if (e.trust >= 255) {
                                e.type = EntityType::CITIZEN;
                                e.state = EntityState::WORKING;
                                e.did = oasis::DIDCryptoGenerator::GenerateDIDKey();
                                std::cout << "ONBOARDING EVENT: NPC Captured! Issued DID: " << e.did << "\n";
                            }
                        }
                    }
                }
            } 
            else if (e.type == EntityType::CITIZEN) {
                if (e.state == EntityState::WORKING) {
                    if (e.fatigue < 255) e.fatigue += 10;
                    if (e.fatigue >= 200) e.state = EntityState::RESTING;
                } 
                else if (e.state == EntityState::RESTING) {
                    if (e.fatigue > 0) {
                        if (e.fatigue >= 20) e.fatigue -= 20;
                        else e.fatigue = 0;
                    }
                    if (e.fatigue == 0) e.state = EntityState::WORKING;
                }
            }
        }
    }
}

} // namespace oasis
