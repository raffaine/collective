#pragma once
#include "chunk_manager.hpp"
#include "entity_manager.hpp"
#include <vector>
#include <cstring>

namespace oasis {

// Flattened structures identical to WGSL storage buffers
struct GpuVoxel {
    uint32_t packed_data; // bytes: id, temperature, moisture, flags
};

struct GpuEntity {
    float x, y, z;
    float type_and_state; // Encoded for shader parsing
};

// Story 5.1: IBufferSync Layer
class BufferSync {
public:
    void Sync(const ChunkManager& chunk_mgr, const EntityManager& entity_mgr) {
        // Flatten Voxel Grid
        const auto& grid = chunk_mgr.GetGrid();
        voxel_buffer_.resize(grid.size());
        std::memcpy(voxel_buffer_.data(), grid.data(), grid.size() * sizeof(uint32_t));

        // Flatten Entities
        const auto& ents = entity_mgr.GetEntities();
        entity_buffer_.clear();
        for (const auto& e : ents) {
            float t_s = static_cast<float>(e.type) + (static_cast<float>(e.state) * 0.1f);
            entity_buffer_.push_back({(float)e.x, (float)e.y, (float)e.z, t_s});
        }
    }

    const std::vector<GpuVoxel>& GetVoxelBuffer() const { return voxel_buffer_; }
    const std::vector<GpuEntity>& GetEntityBuffer() const { return entity_buffer_; }

private:
    std::vector<GpuVoxel> voxel_buffer_;
    std::vector<GpuEntity> entity_buffer_;
};

} // namespace oasis
