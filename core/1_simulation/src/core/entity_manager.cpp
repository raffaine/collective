#include "entity_manager.hpp"

namespace oasis {

EntityManager::EntityManager() : rng_(1337) {}

void EntityManager::AddNPC(uint16_t x, uint16_t y, uint16_t z) {
    entities_.push_back(Entity{
        x, y, z, EntityType::NPC, EntityState::IDLE, 22, 100, 0, 0
    });
}

void EntityManager::AddCitizen(uint16_t x, uint16_t y, uint16_t z) {
    entities_.push_back(Entity{
        x, y, z, EntityType::CITIZEN, EntityState::WORKING, 22, 100, 255, 0
    });
}

void EntityManager::Tick(const SovereignBoundary& boundary) {
    std::uniform_int_distribution<int> move_chance(0, 100);

    for (auto& e : entities_) {
        bool in_boundary = boundary.IsInside(e.x, e.y, e.z);

        if (!in_boundary) {
            // ==========================================
            // BEHAVIORAL LoD: The Fog of Legacy
            // ==========================================
            e.state = EntityState::DRIFTING;
            
            // Legacy pressure abstraction: 
            // Randomly degrade stats due to unmanaged chaos
            if (move_chance(rng_) < 20) {
                if (e.hydration > 0) e.hydration -= 5;
                if (e.type == EntityType::CITIZEN && e.fatigue < 255) e.fatigue += 10;
                
                // Drift towards the Sovereign Boundary to seek stability
                if (e.x < boundary.center_x) e.x++; else if (e.x > boundary.center_x) e.x--;
                if (e.y < boundary.center_y) e.y++; else if (e.y > boundary.center_y) e.y--;
                if (e.z < boundary.center_z) e.z++; else if (e.z > boundary.center_z) e.z--;
            }
        } 
        else {
            // ==========================================
            // DISCRETE LOGIC: Inside the Trust Ring
            // ==========================================
            if (e.state == EntityState::DRIFTING) {
                e.state = (e.type == EntityType::NPC) ? EntityState::IDLE : EntityState::WORKING;
            }

            if (e.hydration > 0) e.hydration -= 1;
            
            if (e.type == EntityType::NPC) {
                if (e.state == EntityState::IDLE) {
                    if (e.hydration < 50) e.state = EntityState::SEEKING_SERVICE;
                } 
                else if (e.state == EntityState::SEEKING_SERVICE) {
                    e.hydration = 100;
                    e.state = EntityState::IDLE;
                    if (e.trust < 255) e.trust += 5;
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
