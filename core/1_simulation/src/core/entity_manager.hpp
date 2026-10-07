#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <random>
#include "sovereign_boundary.hpp"
#include "blueprint_manager.hpp"
#include "did_crypto_generator.hpp"

namespace oasis {

enum class EntityType : uint8_t { NPC = 0, CITIZEN = 1 };
enum class EntityState : uint8_t { IDLE = 0, SEEKING_SERVICE = 1, WORKING = 2, RESTING = 3, DRIFTING = 4 };

struct Entity {
    uint16_t x, y, z;
    EntityType type;
    EntityState state;
    uint8_t temperature;
    uint8_t hydration;
    uint8_t trust;
    uint8_t fatigue;
    std::string did; // The Sovereign DID (Empty for NPCs)
};

class EntityManager {
public:
    EntityManager();
    void AddNPC(uint16_t x, uint16_t y, uint16_t z);
    void AddCitizen(uint16_t x, uint16_t y, uint16_t z);
    
    // Story 6.2 & 6.3: Tick now requires Blueprints for Layer 7 pathfinding and Crypto for onboarding captures
    void Tick(const SovereignBoundary& boundary, const BlueprintManager& blueprints, DIDCryptoGenerator& crypto);

    std::vector<Entity>& GetEntities() { return entities_; }
    const std::vector<Entity>& GetEntities() const { return entities_; }

private:
    std::vector<Entity> entities_;
    std::mt19937 rng_;
};

} // namespace oasis
