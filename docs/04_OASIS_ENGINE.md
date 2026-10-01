# Oasis Engine: The Shadow Simulator

Oasis is not a traditional video game. It is a high-performance, local-first physics simulator and the primary Trojan Horse used to bootstrap the physical infrastructure of The Collective. 

By simulating the thermodynamic realities of ecological succession, fluid dynamics, and material entropy, Oasis serves as the testbed for Layer 4 (BPMN Automation) and Layer 3 (Valuenomics). Once a Genesis Node is physically constructed, the Oasis engine transitions seamlessly from a video game into a "Shadow Simulator," consuming live telemetry from Layer 2 sensors to model and validate physical workflows before they actuate in the real world.

---

## 1. The C++20 Core & Data-Oriented Design

To simulate deep permaculture systems and fluid dynamics at 60 FPS, object-oriented programming overhead is abandoned in favor of strict Data-Oriented Design (DOD). The environment is represented as a massive, mutable voxel grid managed by a custom C++20 `ChunkManager`.

### The Voxel Memory Layout
Every voxel in the engine represents 1 cubic decimeter of physical space. To maximize CPU cache coherency and Vulkan memory bandwidth, a single voxel is packed into a strict 32-bit integer:

```cpp
// 32-bit Voxel Struct for Vulkan Compute Shaders & CRDT Sync
struct Voxel {
    uint8_t material_id;  // E.g., 0=Air, 1=Concrete, 2=Fungal_Loam, 3=PVC
    uint8_t moisture;     // Saturation capacitance (0-255)
    uint8_t temperature;  // Quantized localized thermal mass
    uint8_t metadata;     // Bitmask for systemic logic
    
    // Metadata Bitmask Breakdown:
    // Bit 0: Is_Legacy_Tethered (1 = Fiat Drain, 0 = Sovereign)
    // Bit 1: Is_Actuator (1 = BPMN Controlled, 0 = Static)
    // Bit 2: Is_Sensor (1 = Generating L2 Telemetry)
    // Bit 3-7: Structural stress / Flow vector data
};
