#pragma once
#include <vector>
#include <cstdint>
#include "sovereign_boundary.hpp"
#include <random>

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
};

class EntityManager {
public:
    EntityManager();
    void AddNPC(uint16_t x, uint16_t y, uint16_t z);
    void AddCitizen(uint16_t x, uint16_t y, uint16_t z);
    void Tick(const SovereignBoundary& boundary);

    std::vector<Entity>& GetEntities() { return entities_; }
    const std::vector<Entity>& GetEntities() const { return entities_; }

private:
    std::vector<Entity> entities_;
    std::mt19937 rng_;
};

} // namespace oasis
