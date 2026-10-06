#pragma once
#include "voxel.hpp"
#include <string>

namespace oasis {

// Story 2.1: Observer interface for systems (CRDT/Renderer) tracking voxel state changes
class IChunkObserver {
public:
    virtual ~IChunkObserver() = default;
    virtual void OnVoxelMutated(int x, int y, int z, const Voxel& old_v, const Voxel& new_v) = 0;
};

// Story 2.4: Actuator interface allowing parsed BPMN intents to mutate physical state
class IActuator {
public:
    virtual ~IActuator() = default;
    // Execute a semantic intent against the physical layer
    virtual bool Actuate(const std::string& task_id, float value) = 0;
};

} // namespace oasis
