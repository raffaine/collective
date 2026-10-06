#pragma once
#include <vector>
#include "voxel.hpp"
#include "interfaces.hpp"

namespace oasis {

const int CHUNK_SIZE = 32;
const int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

class ChunkManager {
public:
    ChunkManager();
    
    void InitializeHeadless();

    void AddObserver(IChunkObserver* observer);

    void SetVoxel(int x, int y, int z, const Voxel& v);
    Voxel GetVoxel(int x, int y, int z) const;

    inline int GetIndex(int x, int y, int z) const {
        return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE);
    }
    
    inline void GetCoords(int index, int& x, int& y, int& z) const {
        x = index % CHUNK_SIZE;
        y = (index / CHUNK_SIZE) % CHUNK_SIZE;
        z = index / (CHUNK_SIZE * CHUNK_SIZE);
    }

    std::vector<Voxel>& GetGrid() { return grid_; }
    const std::vector<Voxel>& GetGrid() const { return grid_; }

private:
    std::vector<Voxel> grid_;
    std::vector<IChunkObserver*> observers_;
};

} // namespace oasis
