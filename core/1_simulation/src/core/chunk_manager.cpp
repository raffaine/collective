#include "chunk_manager.hpp"

namespace oasis {

ChunkManager::ChunkManager() {}

void ChunkManager::InitializeHeadless() {
    grid_.assign(CHUNK_VOLUME, Voxel{0, 0, 0, 0});
}

void ChunkManager::SetVoxel(int x, int y, int z, const Voxel& v) {
    int index = GetIndex(x, y, z);
    Voxel old_v = grid_[index];
    grid_[index] = v;
    
    for (auto* obs : observers_) {
        obs->OnVoxelMutated(x, y, z, old_v, v);
    }
}

Voxel ChunkManager::GetVoxel(int x, int y, int z) const {
    if (grid_.empty()) throw std::runtime_error("Grid not initialized");
    return grid_[GetIndex(x, y, z)];
}

void ChunkManager::AddObserver(IChunkObserver* observer) {
    observers_.push_back(observer);
}

} // namespace oasis
