#pragma once
#include <cstdint>

namespace oasis {

// Story 1.1: The strict 32-bit compressed Voxel struct
struct Voxel {
    uint8_t material_id;  // E.g., 0=Air, 15=FAB_NODE
    uint8_t moisture;     // Saturation capacitance (0-255)
    uint8_t temperature;  // Quantized localized thermal mass
    uint8_t metadata;     // Bitmask for systemic logic
};

// Compile-time check to guarantee strict 32-bit DOD packing
static_assert(sizeof(Voxel) == 4, "Voxel must be exactly 32 bits (4 bytes) to ensure cache coherency.");

} // namespace oasis
