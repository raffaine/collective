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
```

Chunks are allocated in $32 \times 32 \times 32$ blocks. If a voxel's `Is_Legacy_Tethered` bit is flagged (e.g., a municipal water pipe), the engine automatically subtracts fiat value from the player's Layer 7 proxy reserves during the simulation loop.

---

## 2. Vulkan Compute & Statistical LoD

Oasis relies on compute shaders (written in GLSL/HLSL and cross-compiled via SPIR-V) to handle both rendering and the physical Level of Detail (LoD) simulation.

*   **Ray-Marched Rendering:** The engine bypasses traditional polygon rasterization. The Vulkan backend ray-marches the voxel grid directly, allowing for real-time visualization of soil degradation, moisture gradients, and heat maps without generating millions of triangulated meshes.
*   **Thermodynamic Fluid Dynamics:** Compute shaders calculate the collision and routing of rainwater. Rain hitting a concrete voxel (`material_id = 1`) routes to a legacy storm drain boundary condition. Rain hitting a permaculture voxel (`material_id = 2`) triggers an absorption algorithm based on the `moisture` byte.
*   **Statistical LoD:** Distant biomes are not simulated on a per-voxel basis. The engine abstracts them into probabilistic state models (e.g., "Forest Chunk 04 has a 20% moisture decay rate"), only collapsing the waveform into physical voxels when the player or a Layer 2 sensor directly observes them.

---

## 3. The WebAssembly (WASM) Sandbox

Oasis is a local-first application. There are no central game servers. 

The C++20 core is compiled down to WebAssembly (WASM) using Emscripten. This allows the heavy physical simulation and Vulkan/WebGL graphics to run sandboxed inside any modern web browser or lightweight desktop wrapper. 
*   **Data Sovereignty:** The player's map, ledger, and blueprints live entirely in an IndexedDB/OPFS (Origin Private File System) local database. 
*   **Embedded Logic:** Oasis does not hardcode its gameplay rules. The WASM binary embeds a lightweight XML parser. When a player designs an automated system, the engine generates Layer 4 BPMN 2.0 XML, feeds it to the parser, and executes the state machine locally to update the voxel grid.

---

## 4. Decentralized Multiplayer (CRDTs & Gossipsub)

If the database is local, multiplayer is achieved purely through state synchronization. 

When citizens collaborate on a community build within Oasis, their local WASM clients sync structural changes using **Conflict-free Replicated Data Types (CRDTs)**. 
*   Instead of sending "Voxel at X,Y,Z is now Wood," the client sends a cryptographically signed operational delta.
*   These deltas are broadcast across the mesh using **libp2p Gossipsub** (routed over WebRTC in the browser, or raw TCP/LoRa for physical edge nodes).
*   This perfectly prototypes the Layer 3 Ledger. If the networking code can successfully resolve a conflict between two players trying to place a block in the same voxel simultaneously without a central server, it can safely manage the real-world Value Token ledger.
