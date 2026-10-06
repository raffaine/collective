#pragma once
#include "interfaces.hpp"
#include <vector>
#include <iostream>

namespace oasis {

// Represents a single voxel mutation for sync over WebRTC/LocalDB
struct VoxelDelta {
    int x, y, z;
    Voxel new_state;
};

// Story 2.5: CRDT Sync Engine Shell
class CRDTSyncEngine : public IChunkObserver {
public:
    CRDTSyncEngine() = default;
    
    // Implements IChunkObserver
    void OnVoxelMutated(int x, int y, int z, const Voxel& old_v, const Voxel& new_v) override {
        // In Epic 2, we just record the delta. 
        // Epic 3/5 will hook this into y-crdt / automerge.
        pending_deltas_.push_back({x, y, z, new_v});
    }

    size_t GetPendingDeltaCount() const { return pending_deltas_.size(); }
    
    void Flush() {
        pending_deltas_.clear();
    }

private:
    std::vector<VoxelDelta> pending_deltas_;
};

} // namespace oasis
