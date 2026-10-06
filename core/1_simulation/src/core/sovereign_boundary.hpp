#pragma once

namespace oasis {

// Story 4.1 & 4.2: Defines the managed territory vs. The Fog of Legacy
struct SovereignBoundary {
    int center_x = 16;
    int center_y = 16;
    int center_z = 16;
    int radius = 8; // Voxel distance of managed trust

    // Simple Manhattan distance for grid efficiency
    inline bool IsInside(int x, int y, int z) const {
        int dx = (x > center_x) ? (x - center_x) : (center_x - x);
        int dy = (y > center_y) ? (y - center_y) : (center_y - y);
        int dz = (z > center_z) ? (z - center_z) : (center_z - z);
        return (dx + dy + dz) <= radius;
    }
};

} // namespace oasis
