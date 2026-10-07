# OASIS SIMULATION ENGINE — PRODUCT BACKLOG V3.0
**Document Version:** 3.0.0-RELEASE  
**Status:** Ratified & Authoritative  
**Milestone:** Oasis 3D Visual & Gameplay Reboot (Phase 1: Foundations & Genesis)  
**Target Environments:** Native Desktop (C++20 / SDL2 / Dawn WebGPU) & WebAssembly (WASM / WebGPU / Emscripten)  
**Architectural Baseline:** 7-Layer Sovereign Stack Model (Volume 1) & Data-Oriented Design (DOD)  

---

## 1. Executive Summary & The Oasis Engine Reboot Charter

### 1.1. The Reboot Rationale: Retiring Legacy Anti-Patterns
The Oasis Engine was conceived as a high-performance local-first simulator to bridge the psychological and material divide between legacy capitalist reliance and localized sovereign resilience. However, the initial technical implementation accumulated critical design and architectural compromises that constrained both performance and gameplay empathy. 

This Product Backlog formally enacts the **Oasis Engine Reboot Charter**, retiring four legacy anti-patterns:

1. **Retirement of the Orthogonal 2D Slice Renderer (`core/renderer.cpp`):**  
   The previous rendering pipeline collapsed a volumetric world into an isolated, single-plane 2D cross-section ($y=16$), rendering static color rectangles to an SDL2 window. This destroyed the spatial intuition required for 3D permaculture planning, sunlight angle calculation, multi-level construction, and vertical hydrological routing. In its place, the engine adopts a native **WebGPU screen-space 3D Digital Differential Analyzer (DDA) compute ray-marcher**.
2. **Retirement of the Omniscient "God-Mode Cursor":**  
   Legacy colony simulators detach the player from physical vulnerability, granting instant god-like omniscience and instant point-and-click terrain manipulation. Oasis permanently rejects this trope. Agency is embodied in **Founder 01**, a flesh-and-blood mortal steward bound to physical thermodynamic realities—caloric reserves, core body temperature, physical hydration, and altruism fatigue.
3. **Retirement of the Infinite Municipal Grid & Magical Blueprints:**  
   Traditional city builders hook infrastructure into invisible, infinite municipal grids funded by abstracted fiat tax pools. In Oasis, the simulation starts entangled in real **Layer 7 municipal fiat debt** (overhead power drop lines and sputtering water bibs draining bank reserves). Blueprints do not conjure physical voxels out of thin air; they compile into **Layer 4 BPMN 2.0 executable tasks** requiring physical tools, human labor, real calories, and physical material staging.
4. **Retirement of Heap-Indirected Entity Bloat (`core/entity_manager.hpp`):**  
   The initial C++ `Entity` struct contained dynamic `std::string did` identifiers and unaligned scalar fields, swelling individual entity footprint to over 40 bytes with heap indirection and failing Catch2 unit tests (`sizeof(Entity) <= 32`). This destroyed L1 CPU cache coherence. The reboot establishes a contiguous, cache-packed **12-byte `EntityPhysics` Data-Oriented Design (DOD)** struct, isolating cold cryptographic identities into an out-of-band indexed store.

```
       ┌────────────────────────────────────────────────────────┐
       │             LEGACY OASIS ENGINE (DEPRECATED)           │
       │  - 2D Orthogonal Slice Renderer (y=16 plane only)      │
       │  - Omniscient God-Mode Cursor (Zero physical stakes)   │
       │  - Magical Blueprints (Instant instant voxel mutation) │
       │  - Heap-Indirected Entity Struct (40+ bytes, std::str) │
       └───────────────────────────┬────────────────────────────┘
                                   │
                         PERMANENT REBOOT PASS
                                   │
                                   ▼
       ┌────────────────────────────────────────────────────────┐
       │              OASIS REBOOT ENGINE V3.0 (RATIFIED)       │
       │  - WebGPU Screen-Space 3D DDA Compute Ray-Marching     │
       │  - Dual-Mode Founding Steward (Mortal Embodiment)      │
       │  - Non-Magical Blueprints Compiling to Layer 4 BPMN 2.0│
       │  - Contiguous 12-Byte DOD EntityPhysics (L1-Optimized) │
       └────────────────────────────────────────────────────────┘
```

### 1.2. The Sovereign Trojan Horse Vision
Oasis is not an escapist video game. It is a high-performance simulation engine designed to serve as a **Trojan Horse for physical-world sovereign resilience**. Every ecological, logistical, and architectural system successfully designed within the simulation compiles deterministically through the 7-Layer Sovereign Stack:

$$\text{Layer 6 (Semantic Lens)} \longrightarrow \text{Layer 4 (BPMN 2.0 Orchestration)} \longrightarrow \text{Layer 3 (Ledger/CRDT)} \longrightarrow \text{Layer 2 (IoT Digital Twin)} \longrightarrow \text{Layer 1 (Physical Reality)}$$

Once a physical community node is established, Oasis transitions seamlessly from an interactive game into a real-time **Shadow Simulator**. In-game valve toggles actuate physical edge relays over MQTT/CoAP (Layer 2), material trades mint cryptographic Value Tokens on local ledgers (Layer 3), and communal mutual aid schedules execute as signed BPMN workflow tokens.

---

## 2. Core Architectural Pillars & Design Principles

### 2.1. The Three Gameplay Design Pillars
The reboot synthesizes the design philosophies of three simulation landmarks, adapting their mechanics to sovereign permaculture realities:

* **Pillar 1: Dwarf Fortress (Thermodynamics & Emergent Social Friction):**  
  Matter is strictly conserved across a 32-bit voxel grid. Heat diffuses through solid mass, fluids permeate porous loam, and thermal mass protects or endangers biological entities. Socially, physical hardship generates psychological trauma and altruism fatigue. A ruined harvest does not vanish into a generic deficit; it strains Trust Rings, creates social friction, and forces the community to choose between mutual aid and catastrophic burnout.
* **Pillar 2: The Sims (Daily Agency & Visual Empathy):**  
  Complex sociological modeling is conveyed through intuitive, personal human stakes rather than sterile spreadsheets. Founder 01 and onboarding citizens communicate physical limits through unmistakable visual telegraphs: visible breath condensation in cold environments, slumped fatigue animations, stomach-growling audio cues, and an iconic **Plumbob Vitality Indicator** hovering in 3D space whose color shifts from emerald green to pulsing crimson as needs criticalize.
* **Pillar 3: Cities: Skylines (Permaculture Macro-Flows & Closed Loops):**  
  Permaculture is treated as serious macroscopic infrastructure. Rather than relying on an invisible municipal umbilical cord, players manage closed-loop material cycles: biochar pyrolysis, solar Exergy capture and routing, greywater bioswale filtration, and composting thermophilic piles.

### 2.2. The Spatial Envelope: Lot 402 Genesis Node
At engine boot ($t=0$), the active physical simulation volume is strictly bounded to **Lot 402: The Brownfield Depot**:
- **Grid Dimensions:** $3 \times 3 \times 2$ chunks ($96 \times 96 \times 64$ voxels).
- **Physical Scale:** 1 decimeter ($0.1\text{m}$) per voxel. Total parcel volume: $9.6\text{m} \times 9.6\text{m} \times 6.4\text{m}$.
- **Memory Footprint:** 18 chunks $\times$ 128 KB = **2,359,296 bytes (2.359 MB)**. This fits completely within modern CPU L3 cache and GPU VRAM, guaranteeing sub-10ms allocation on cold boot.
- **Physical Landscape:** 60% impervious degraded asphalt slabs (`material_id = 1`), 30% compacted clay soil (`material_id = 4`), and 10% shelter structures (a derelict cinderblock shed and camper van). Compacted asphalt produces zero moisture capacitance, channeling initial stormwater into municipal gutters rather than groundwater infiltration.
- **Layer 7 Municipal Entanglement:** Boundary voxels interface with legacy municipal grids:
  - Overhead 240V utility line tether at $(x=95, y=32, z=48)$.
  - Corroded cast-iron water bib tether at $(x=0, y=16, z=48)$.
  - Active fiat drain: Consuming power or water decrements the player's $\$1,450.00$ legacy escrow account. When the reserve reaches zero, utility cutoff occurs, accelerating municipal code enforcement scrutiny.

### 2.3. Player Embodiment: The Dual-Mode Founding Steward
Player interaction is mediated through **Founder 01**, alternating between two perspectives via a smooth 400ms camera interpolation:
1. **Direct Mode (Micro / Kinematic 3rd-Person):**  
   Over-the-shoulder 3rd-person camera tracking Founder 01. Direct WASD movement with a discrete swept-AABB kinematic controller capable of negotiating 2-decimeter step-ups over broken asphalt. Contextual physical actions include shoveling compost, swinging pickaxes to break asphalt, and lighting biochar fires.
2. **Cadastral Mode (Macro / The Steward Slate):**  
   The camera interpolates into a 45-degree orthographic axonometric projection framing the entire $9.6\text{m} \times 9.6\text{m}$ parcel. The render style transitions to a desaturated architectural CAD blueprint aesthetic, overlaying Layer 6 semantic visualizers: thermodynamic heatmaps, hydraulic flow arrows, and pulsating neon magenta Layer 7 fiat drain boundary markers.

```
       [Direct Mode: 3rd-Person View] ◄──────── 400ms ────────► [Cadastral Mode: Axonometric CAD]
       - Over-the-shoulder tracking            Cubic Hermite     - 45° isometric surveyor angle
       - WASD swept-AABB kinematics              Blend Curve     - Architectural desaturated palette
       - Physical calorie & tool labor                           - Semantic heat/water/fiat overlays
       - Plumbob vitality diamond                                - BPMN blueprint drafting
```

### 2.4. The Fog of Legacy: Volumetric Thermodynamic LoD Boundary
Traditional Level of Detail (LoD) algorithms discard geometric fidelity to save GPU cycles. In Oasis, LoD is an intentional **thermodynamic and social boundary**:
- Voxels within the $96 \times 96 \times 64$ envelope execute deterministic 32-bit physical and metabolic updates.
- Space beyond the envelope is not simulated discretely. It is rendered as a volumetric atmospheric haze of urban smog, distant highway glare, and sirens, acting as a statistical thermodynamic heat and fluid sink.
- As the player expands their Sovereign Trust Ring and onboards adjacent parcels, the Fog of Legacy recedes, converting unmanaged probabilistic noise into deterministic physical truth.

---

## 3. High-Performance Memory & Data Layouts (C++20 Specifications)

### 3.1. The Strict 32-Bit Voxel Layout
Every voxel represents exactly $1\text{ dm}^3$ of matter. Memory layout is strictly aligned to 4 bytes:

```cpp
namespace oasis {

struct alignas(4) Voxel {
    uint8_t material_id;  // 0=Air, 1=Asphalt, 2=Fungal Loam, 3=PVC, 4=Clay, 5=Biochar, 6=Water, 7=Rubble
    uint8_t moisture;     // Moisture capacitance (0 = bone dry, 255 = saturated)
    uint8_t temperature;  // Quantized thermal mass (0 = -20°C, 128 = 20°C, 255 = 100°C+)
    uint8_t metadata;     // Systemic logic bitmask
};
static_assert(sizeof(Voxel) == 4, "Voxel must be exactly 32 bits (4 bytes) for memory alignment.");

// Metadata Bitmask Definitions
enum VoxelMetadataBits : uint8_t {
    META_LEGACY_TETHERED = 1 << 0, // Bit 0: Layer 7 municipal fiat drain active
    META_ACTUATOR        = 1 << 1, // Bit 1: Layer 4 BPMN controlled valve / relay
    META_SENSOR          = 1 << 2, // Bit 2: Layer 2 IoT Digital Twin telemetry source
    META_STRESS_MASK     = 0xF8    // Bits 3-7: Structural load stress / hydraulic routing
};

} // namespace oasis
```

### 3.2. The Contiguous 12-Byte DOD `EntityPhysics` Struct
To maximize CPU L1 cache utilization and eliminate heap allocations during the 60 Hz simulation tick, the dynamic state of all active characters is packed into a 12-byte contiguous struct:

```cpp
namespace oasis {

enum class EntityType : uint8_t {
    NPC = 0,
    CITIZEN = 1,
    FOUNDER = 2
};

enum class EntityState : uint8_t {
    IDLE = 0,
    MOVING = 1,
    WORKING = 2,
    RESTING = 3,
    EXHAUSTED = 4,
    DRIFTING = 5
};

struct alignas(4) EntityPhysics {
    uint16_t x;           // 2 bytes: X position in decimeters (0 to 65,535)
    uint16_t y;           // 2 bytes: Y elevation in decimeters
    uint16_t z;           // 2 bytes: Z position in decimeters
    uint8_t  entity_type; // 1 byte: 0=NPC, 1=Citizen, 2=Founder
    uint8_t  state;       // 1 byte: EntityState FSM
    uint8_t  temperature; // 1 byte: Quantized body temp (30.0°C to 42.0°C)
    uint8_t  hydration;   // 1 byte: Water reserves (0 to 255)
    uint8_t  calories;    // 1 byte: Caloric reserve (0 to 255, scaling 0 to 3,000 kcal)
    uint8_t  fatigue;     // 1 byte: Physical/cognitive exhaustion (0 to 255)
};
static_assert(sizeof(EntityPhysics) == 12, "EntityPhysics must be exactly 12 bytes.");

} // namespace oasis
```

- **Cache Line Efficiency:** A standard 64-byte L1 cache line holds $64 / 12 \approx 5.33$ contiguous entities.
- **L1 Cache Residency:** A pre-allocated pool of 1,024 active entities occupies $1,024 \times 12\text{ bytes} = 12.288\text{ KB}$, fitting entirely within the 32 KB or 48 KB L1 data cache of modern processors.

### 3.3. Parallel Hot `EntityKinematics` Buffer (Structure of Arrays)
Sub-voxel floating-point coordinates and velocities for moving entities are decoupled from metabolic logic:

```cpp
namespace oasis {

struct EntityKinematics {
    float sub_x, sub_y, sub_z; // Continuous position in decimeters
    float vel_x, vel_y, vel_z; // Velocity vector in dm/s
    bool  is_grounded;         // Voxel contact flag
};

} // namespace oasis
```

### 3.4. Decoupled Cold `SovereignIdentityStore`
Cold cryptographic data (Ed25519 public keys, Decentralized Identifiers, Web of Trust scores, and token balances) is segregated into an out-of-band store indexed by `uint16_t entity_id`:

```cpp
namespace oasis {

struct SovereignIdentity {
    std::array<uint8_t, 32> ed25519_pubkey; // Raw Ed25519 public key
    uint8_t  trust_ring;                    // Ring 0=Founder, Ring 1=Citizen, Ring 2=Peer
    uint8_t  reputation_score;              // Web of Trust weight (0 to 255)
    uint16_t assigned_bpmn_task;           // Currently claimed WorkToken ID
    uint32_t token_balance;                // Layer 3 Value Tokens (Proof of Stewardship)
    char     did_uri[48];                  // "did:oasis:genesis:<hash>"
};

} // namespace oasis
```

### 3.5. Operational CRDT Delta Struct (`VoxelMutationOp`)
Multiplayer and distributed state synchronization uses compact 24-byte operational mutations:

```cpp
namespace oasis {

struct alignas(8) VoxelMutationOp {
    uint64_t logical_clock;     // Hybrid Logical Clock (HLC)
    uint32_t voxel_index;       // Linear offset in Genesis envelope (0 to 589,823)
    Voxel    old_value;         // 4 bytes: State prior to mutation
    Voxel    new_value;         // 4 bytes: State post mutation
    uint16_t author_entity_id;  // Founder 01 or Citizen ID
    uint16_t signature_slot;    // Slot index in the signed cryptographic delta pool
};
static_assert(sizeof(VoxelMutationOp) == 24, "VoxelMutationOp must be exactly 24 bytes.");

} // namespace oasis
```

### 3.6. Camera Uniforms Buffer (`CameraUniforms`)
```cpp
namespace oasis {

struct alignas(16) CameraUniforms {
    glm::mat4 inv_view_proj;    // 64 bytes: Screen-to-world ray unprojection matrix
    glm::vec4 camera_pos;       // 16 bytes: Camera position in world space
    glm::vec4 view_params;      // 16 bytes: x=FOV, y=OrthoScale, z=BlendProgress, w=RenderMode
    glm::vec2 resolution;       // 8 bytes: Viewport dimensions (e.g., 1280, 720)
    float     time_seconds;     // 4 bytes: Running simulation time
    uint32_t  active_lens;      // 4 bytes: 0=Physical, 1=Thermal, 2=Hydraulic, 3=Cadastral
};
static_assert(sizeof(CameraUniforms) == 112, "CameraUniforms alignment verified.");

} // namespace oasis
```

---

## 4. WebGPU WGSL Ray-Marching & Two-Phase Deferred Pipeline

### 4.1. Two-Phase Deferred Architecture
To eliminate GPU warp and wavefront execution divergence, ray evaluation is separated into two decoupled compute passes:
1. **Phase 1: 3D DDA Spatial Traversal:**  
   Rays step through the $96 \times 96 \times 64$ voxel volume using Amanatides & Woo integer arithmetic. Fast-path traversal branches only on `material_id != 0`. Traversal records hit voxel index, normal direction, and distance.
2. **Phase 2: Deferred PBR & Lens Evaluation:**  
   Rays that hit a voxel evaluate material properties from an indexed uniform palette (`array<MaterialProperties, 16>`). PBR specular reflection, moisture darkening, thermal glow, and neon magenta fiat tethers are computed uniformly without divergence. Rays missing the parcel sample procedural volumetric fog.

```
[Screen Pixel (x, y)]
         │
         ▼
[Phase 1: 3D DDA Ray Traversal] ──(Integer steps through 96x96x64 grid)──┐
  - Ray-Box AABB Intersection                                            │
  - Amanatides & Woo DDA Step                                            │
  - Fetch 32-bit voxel word: if (material != 0) break;                   │
         │                                                               │
         ├─────────────────────────────────────────┐                     │
         ▼ (Voxel Hit)                             ▼ (Miss / Exited Envelope)
[Phase 2: Deferred Shading]             [Volumetric Fog of Legacy]       │
  - Read material_palette[id]             - Ray-march 3D Perlin noise    │
  - Modulate roughness via moisture       - Sample distant highway haze  │
  - Blend thermal heatmap or L7 magenta   - Return moody industrial smog │
         │                                         │                     │
         └────────────────────┬────────────────────┘                     │
                              ▼                                          │
                     [Output Render Target] <────────────────────────────┘
```

### 4.2. WGSL Material Palette Uniform Definition
```wgsl
struct MaterialProperties {
    base_color: vec4<f32>,
    roughness: f32,
    metallic: f32,
    porosity: f32,
    padding: f32,
};

@group(0) @binding(0) var<uniform> camera: CameraUniforms;
@group(0) @binding(1) var<uniform> material_palette: array<MaterialProperties, 16>;
@group(0) @binding(2) var voxel_grid: texture_3d<u32>;
```

### 4.3. Visual Shading Lenses
The rendering pipeline supports four real-time semantic lenses via `active_lens`:
- **Lens 0 (Physical Reality):** Standard PBR shading. Surface roughness drops toward $0.05$ as `moisture > 128`, producing mirror-like specular puddles reflecting skybox ambient light.
- **Lens 1 (Thermodynamic Heatmap):** Surfaces are false-color shaded based on `temperature`. Freezing voxels ($T < 80$) show blue-white frost crystals. Superheated biochar voxels ($T > 180$) pulse orange with screen-space heat refraction.
- **Lens 2 (Hydraulic Flow):** Permeability and moisture saturation arrows rendered over soil. Impervious asphalt voxels highlight runoff vectors towards municipal gutters.
- **Lens 3 (Cadastral CAD Overlay):** Monochromatic high-contrast architectural render with neon magenta bounding boxes on `META_LEGACY_TETHERED` utility drops, displaying floating real-time kilowatt and gallon consumption gauges.

---

## 5. Comprehensive Epics Specification

---

### Epic 1: The 3D Voxel Genesis Node & WebGPU Rendering Core

#### 1.1. Executive Scope & Strategic Objective
Construct the core spatial foundation for the Oasis reboot: an 18-chunk simulation volume ($96 \times 96 \times 64$ voxels, 2.359 MB) representing Lot 402, rendered via a high-performance WebGPU screen-space 3D DDA compute ray-marching pipeline running at 60 FPS on both native SDL2/Dawn and browser WASM/WebGPU. Implement two-phase deferred PBR material shading, 400ms smooth dual-mode camera interpolation, and dirty-chunk bitmask buffer synchronization.

#### 1.2. Persona Value Propositions
- **Expert Game Designer (3D Sims / DF / Skylines):** Eliminates the flat 2D slice, providing an immersive, tactile 3D world where soil visibly darkens with moisture, puddles form on cracked asphalt, and switching to Cadastral mode provides an intuitive architectural surveyor perspective.
- **Collective Architect (7-Layer Sovereign Stack):** Grounds Layer 1 physical space in strict 32-bit voxel conservation. Boundary utility drops visually display Layer 7 municipal fiat drains with neon magenta wireframes.
- **Senior C++ Game Architect (Engine Core & DOD):** Replaces dynamic polygon rasterization with an integer 3D DDA ray-marcher. Utilizes 32-bit dirty chunk bitmasks to restrict GPU buffer uploads to $< 128\text{ KB/frame}$, completing compute rendering within $4.80\text{ ms}$.

#### 1.3. User Stories & Formal Acceptance Criteria

##### Story 1.1: Multi-Chunk Allocation & Spatial Indexing for Lot 402
- **Description:** Upgrade `ChunkManager` to allocate an array of $3 \times 3 \times 2 = 18$ contiguous $32^3$ chunks representing Lot 402 ($96 \times 96 \times 64$ voxels, 2.359 MB) with $O(1)$ coordinate resolution.
- **Given:** An uninitialized `ChunkManager`.
- **When:** `InitializeLot402()` is called.
- **Then:** Exactly 18 contiguous chunks are allocated in a single heap block of 2,359,296 bytes; coordinate query `GetVoxel(95, 63, 95)` maps to chunk index 17 and local voxel offset 32,767 in $< 10\text{ ns}$; out-of-bounds queries throw `std::out_of_range`.

##### Story 1.2: Screen-Space 3D DDA Compute Ray-Marching Shader (WGSL)
- **Description:** Implement a WebGPU compute shader executing Amanatides & Woo fast 3D voxel traversal through a $1280 \times 720$ screen grid.
- **Given:** A bound 3D voxel storage texture of $96 \times 96 \times 64$ voxels and screen dimensions $1280 \times 720$.
- **When:** The compute shader dispatches with workgroups of $8 \times 8 \times 1$.
- **Then:** Rays traverse the volume in integer grid steps with a hard ceiling of 180 steps per ray; rays exiting the envelope sample procedural Volumetric Fog of Legacy noise; the full compute pass executes in $\le 4.80\text{ ms}$.

##### Story 1.3: Two-Phase Deferred Material Palette & PBR Lens Shading
- **Description:** Implement Phase 2 deferred shading sampling a uniform palette of 16 materials, modulating roughness via moisture and emissive color via temperature.
- **Given:** A primary ray hitting a voxel with `material_id = 1` (Asphalt) and `moisture = 200`.
- **When:** The deferred shading pass evaluates surface properties.
- **Then:** Material albedo is darkened by 30%, surface roughness drops to 0.05, producing sharp specular reflections; boundary voxels with `META_LEGACY_TETHERED` render a pulsating neon magenta wireframe at 1.0 Hz; voxels with `temperature > 160` emit glowing thermal refraction.

##### Story 1.4: Dual-Mode Camera Interpolation & Cadastral Projection
- **Description:** Implement camera uniform updates supporting smooth 400ms transition between 3rd-person perspective and orthographic axonometric surveyor view.
- **Given:** The camera is in Direct 3rd-Person mode tracking Founder 01.
- **When:** The player toggles to Cadastral mode.
- **Then:** Camera projection and view matrices interpolate over exactly 400ms using a cubic Hermite curve (`smoothstep`), settling into a 45° orthographic axonometric projection with desaturated CAD coloring and Layer 6 semantic overlays without frame stutter.

##### Story 1.5: Dirty Chunk Bitmask Buffer Synchronization
- **Description:** Implement a 32-bit dirty chunk bitmask in `BufferSync` to upload only mutated $32^3$ chunks to the GPU per frame.
- **Given:** A simulation tick where 12 voxels mutate exclusively within chunk 2 and chunk 5.
- **When:** `BufferSync::Sync()` executes.
- **Then:** `dirty_chunk_mask` evaluates to `(1 << 2) | (1 << 5)`, only 256 KB of buffer data is copied via `wgpuQueueWriteBuffer`, and GPU synchronization completes in $\le 0.35\text{ ms}$.

#### 1.4. Technical Architecture & Concrete Implementation Tasks
- **C++20 Implementation Tasks:**
  1. Refactor `core/1_simulation/src/core/chunk_manager.hpp` and `.cpp` to manage an array of 18 contiguous $32^3$ chunks with bitmask dirty tracking.
  2. Implement `core/1_simulation/src/core/camera_manager.hpp` handling cubic Hermite camera interpolation between Direct 3rd-Person and Cadastral Axonometric modes.
  3. Implement `core/1_simulation/src/core/webgpu_renderer.hpp` wrapping `wgpu::Device`, `wgpu::Queue`, compute pipelines, and bind groups.
  4. Implement `core/1_simulation/src/core/buffer_sync.hpp` updating GPU storage buffers strictly via `dirty_chunk_mask`.
- **WGSL Shader Tasks:**
  1. Author `core/1_simulation/src/shaders/dda_raymarch.wgsl`: Amanatides & Woo 3D integer traversal, AABB clipping, Fog of Legacy sampling.
  2. Author `core/1_simulation/src/shaders/deferred_pbr.wgsl`: Uniform palette lookup (`array<MaterialProperties, 16>`), moisture specular modulation, thermal heatmap, and neon magenta wireframes.

#### 1.5. Definition of Done (DoD)
- `tests/epic1_tests.cpp` compiles and passes 100% in Catch2.
- WebGPU compute pipelines compile on native Dawn and Emscripten WASM with zero validation errors.
- Total memory footprint for the 18 chunks is verified at exactly 2,359,296 bytes.
- Frame compute pass benchmarks at $\le 4.80\text{ ms}$ at $1280 \times 720$ resolution.

---

### Epic 2: The Founding Steward & Kinematic Physical Simulation

#### 2.1. Executive Scope & Strategic Objective
Implement the mortal embodiment of Founder 01 and sovereign citizens using a cache-contiguous 12-byte `EntityPhysics` data structure, a smooth 60 Hz swept-AABB kinematic controller with 2-decimeter step-up capability over complex voxel terrain, Sims-style metabolic telegraphing (calories, fatigue, hydration, Plumbob vitality diamond), and Dwarf Fortress-style physical thermal mass and labor exertion.

#### 2.2. Persona Value Propositions
- **Expert Game Designer (3D Sims / DF / Skylines):** Gives the player an empathetic, vulnerable avatar whose daily survival matters. Replaces spreadsheet menus with visual empathy: shivering breath condensation in the cold, slumped posture when hungry, slow tool swings when exhausted, and an iconic floating Plumbob diamond that shifts color to convey need states.
- **Collective Architect (7-Layer Sovereign Stack):** Anchors character labor into Layer 1 caloric physics. Proof of Stewardship is grounded in genuine thermodynamic work—constructing permaculture features requires real calories and time. Separates physical embodiment from cold cryptographic identity.
- **Senior C++ Game Architect (Engine Core & DOD):** Replaces the 40-byte heap-indirected `Entity` struct with a 12-byte DOD `EntityPhysics` struct, immediately fixing failing Catch2 tests (`sizeof(Entity) <= 32`). Pre-allocates 1,024 entities into 12.3 KB of L1 cache, executing the 60 Hz kinematic controller in $< 0.05\text{ ms}$.

#### 2.3. User Stories & Formal Acceptance Criteria

##### Story 2.1: 12-Byte Contiguous DOD `EntityPhysics` & Cache Locality
- **Description:** Replace bloated entity struct with a 12-byte cache-aligned struct, decoupling identity and string data into cold storage.
- **Given:** The `oasis::EntityPhysics` struct definition.
- **When:** Compiled with C++20.
- **Then:** `sizeof(EntityPhysics)` is verified as exactly 12 bytes via compile-time `static_assert`, fitting 5.3 entities per 64-byte L1 cache line, and pre-allocated buffer of 1,024 entities consumes $\le 12.3\text{ KB}$.

##### Story 2.2: Discrete Swept-AABB Kinematic Controller with 2-Decimeter Step-Up
- **Description:** Implement a 60 Hz kinematic character controller with swept-AABB collision and 2-decimeter step-up height for cracked asphalt and curbs.
- **Given:** Founder 01 bounding box of $5 \times 3 \times 18$ decimeters moving horizontally toward a 1-dm curb or 2-dm asphalt slab.
- **When:** Forward horizontal movement detects a foot collision but head clearance at $y + \text{step} + 18$ is unobstructed.
- **Then:** The character smoothly ascends the step within 100ms without loss of horizontal forward velocity; obstacles $\ge 3\text{ dm}$ cause a standard Cartesian slide; gravity applies at $98\text{ dm/s}^2$.

##### Story 2.3: Sims-Style Metabolic Needs & Visual Empathy Telegraphing
- **Description:** Implement 60 Hz metabolic decay and visual status telegraphs (calories, fatigue, hydration, Plumbob vitality).
- **Given:** Founder 01 performing continuous physical labor.
- **When:** `calories` drops below 50 ($< 600\text{ kcal}$).
- **Then:** Sprint is disabled, walk velocity decreases by 35%, stomach-growling audio cues trigger, and the Plumbob vitality diamond shifts from Emerald Green to Amber. When `fatigue` hits 255, the character transitions to `EntityState::EXHAUSTED` and collapses.

##### Story 2.4: Dwarf Fortress Thermal Mass & Environmental Hypothermia
- **Description:** Implement bidirectional thermal exchange between entity bodies and surrounding voxels.
- **Given:** Ambient temperature of $0^\circ\text{C}$ ($T=80$) and uninsulated surroundings.
- **When:** Founder 01 remains outside unshielded for 300 logic ticks.
- **Then:** Body temperature drops below 35.5°C ($T < 80$), triggering visible breath condensation particles, shivering animation, and doubling baseline caloric consumption. Moving within 2 decimeters of a burning biochar voxel ($T > 200$) warms core body temperature toward 37°C ($T=128$).

##### Story 2.5: Physical Tool Actuation & Voxel Harvesting
- **Description:** Implement physical tool interaction (shoveling loam, breaking asphalt, turning valves) with caloric deduction and voxel mutation.
- **Given:** Founder 01 wielding a pickaxe facing a degraded asphalt slab (`material_id = 1`).
- **When:** The player holds primary action for 3 seconds.
- **Then:** 45 kcal are deducted from `calories`, `fatigue` increases by 10, the target voxel mutates to rubble (`material_id = 7`), and a dirty chunk update is emitted.

#### 2.4. Technical Architecture & Concrete Implementation Tasks
- **C++20 Implementation Tasks:**
  1. Refactor `core/1_simulation/src/core/entity_manager.hpp` and `.cpp` to replace `Entity` with `EntityPhysics` and parallel `EntityKinematics`.
  2. Implement `core/1_simulation/src/core/kinematic_controller.hpp` executing swept-AABB collision detection with 2-decimeter step-up elevation.
  3. Implement `core/1_simulation/src/core/metabolic_system.hpp` evaluating caloric decay, hydration loss, and fatigue accumulation at 60 Hz.
  4. Implement `core/1_simulation/src/core/thermal_exchange.hpp` simulating Fourier heat conduction between entity bounding boxes and adjacent voxels.
  5. Implement `core/1_simulation/src/core/plumbob_renderer.hpp` generating 3D vitality diamond mesh primitives hovering at $(x, y+22, z)$ with color interpolation.

#### 2.5. Definition of Done (DoD)
- `tests/epic2_tests.cpp` passes 100% in Catch2, confirming `sizeof(EntityPhysics) == 12`.
- Kinematic step-up unit tests confirm smooth ascent on 1-dm and 2-dm steps, and blockage on 3-dm walls.
- Metabolic tests verify caloric burn rates and exhaustion state transitions.
- Kinematic controller benchmarked at $\le 0.25\text{ ms}$ for 64 active entities.

---

### Epic 3: Sovereign Systems & 7-Layer Integration Mesh

#### 3.1. Executive Scope & Strategic Objective
Implement the 7-Layer Sovereign Stack integration mesh in C++20, linking Layer 7 municipal fiat drain accumulators, Layer 2 IoT Digital Twin telemetry ingress/egress via lock-free ring buffers, Layer 3 24-byte CRDT operational synchronization over Gossipsub, Layer 4 non-magical BPMN 2.0 blueprint compilation and WorkToken execution, and decoupled Layer 5/cold Ed25519 DID cryptographic identity management.

#### 3.2. Persona Value Propositions
- **Expert Game Designer (3D Sims / DF / Skylines):** Creates systemic tension and rewarding macro-goals. The municipal fiat drain creates a countdown to off-grid survival. Non-magical blueprints turn construction into satisfying logistical execution where citizens haul materials and burn calories.
- **Collective Architect (7-Layer Sovereign Stack):** Realizes Volume 1 of the Sovereign Stack. Enforces the Traversal Protocol: L6 blueprints compile to L4 BPMN, settling on L3 ledger and actuating on L2/L1. Sovereign DIDs ensure verifiable community membership.
- **Senior C++ Game Architect (Engine Core & DOD):** Enforces asynchronous queue isolation. Isolates heavy cryptographic operations and network sockets to worker threads via lock-free SPSC ring buffers, guaranteeing that Layer 2-7 processing consumes $\le 0.30\text{ ms}$ of the frame budget.

#### 3.3. User Stories & Formal Acceptance Criteria

##### Story 3.1: Layer 7 Municipal Tether & Fiat Drain Accumulator
- **Description:** Implement `Layer7DrainAccumulator` calculating power and water draw from boundary tether voxels and deducting costs from fiat escrow.
- **Given:** Lot 402 with $\$1,450.00$ fiat escrow and tethered voxels at $(95, 32, 48)$ and $(0, 16, 48)$.
- **When:** 3,600 logic ticks elapse with camp equipment drawing power and water.
- **Then:** Escrow is decremented by exact integer rate formula:
  $$\text{Cost} = \frac{\text{WattTicks} \times \text{ElectricRate}}{3,600,000} + \frac{\text{WaterTicks} \times \text{WaterRate}}{36,000}$$
  If escrow hits $\$0.00$, `MunicipalCutoffEvent` triggers, cutting power/water and accelerating municipal code enforcement alerts.

##### Story 3.2: Decoupled Cold `SovereignIdentityStore` & Trust Rings
- **Description:** Create out-of-band identity store managing Ed25519 public keys, DIDs, Trust Rings, and token balances indexed by `entity_id`.
- **Given:** An NPC whose trust reaches 255 through mutual aid interactions.
- **When:** `PromoteToCitizen(entity_id)` is invoked.
- **Then:** An Ed25519 keypair and `did:oasis:...` URI are generated and stored in `SovereignIdentityStore`, `EntityPhysics.entity_type` updates to `CITIZEN`, and zero dynamic heap allocations occur in the hot physics loop.

##### Story 3.3: Layer 4 Non-Magical BPMN 2.0 Compiler & Work Token Dispatch
- **Description:** Compile Cadastral blueprints via `pugixml` into BPMN 2.0 XML tasks that emit physical `WorkToken` instances requiring tools, materials, and calories.
- **Given:** A player placing a biochar trench blueprint in Cadastral mode.
- **When:** The blueprint is committed.
- **Then:** `pugixml` generates a valid `Process_Oasis_Infrastructure` BPMN schema, creates `WorkToken` instances requiring 450 kcal of labor, and enqueues them for assignment; voxels mutate only after work tokens are physically completed.

##### Story 3.4: Layer 3 CRDT Synchronization Engine & Operational Deltas
- **Description:** Implement 24-byte `VoxelMutationOp` replication with Hybrid Logical Clock (HLC) and Last-Write-Wins (LWW) conflict resolution.
- **Given:** Two distributed nodes modifying adjacent voxels concurrently.
- **When:** Signed 24-byte `VoxelMutationOp` packets are exchanged over Gossipsub.
- **Then:** Both nodes resolve conflicts deterministically using HLC timestamp and author DID hash tie-breakers, converging to identical voxel memory state with zero divergences.

##### Story 3.5: Layer 2 IoT Digital Twin Telemetry & Hardware Actuator Queue
- **Description:** Implement lock-free SPSC telemetry ingress and actuator validation egress.
- **Given:** An incoming MQTT sensor packet reporting ambient temperature $= 18^\circ\text{C}$ for a voxel marked with `META_SENSOR`.
- **When:** The packet is drained from the SPSC ingress buffer during the 10 Hz logic tick.
- **Then:** The target voxel updates its `temperature` byte within $\le 100\text{ ms}$; actuator commands are thermodynamically validated before emitting hardware GPIO actuation packets.

#### 3.4. Technical Architecture & Concrete Implementation Tasks
- **C++20 Implementation Tasks:**
  1. Implement `core/7_proxy/layer7_drain_accumulator.hpp` and `.cpp` calculating utility consumption and escrow balance.
  2. Implement `core/3_ledger/sovereign_identity_store.hpp` storing Ed25519 keys, DIDs, Trust Rings, and token balances indexed by `uint16_t`.
  3. Implement `core/4_orchestrator/bpmn_compiler.hpp` converting Cadastral spatial blueprints into BPMN 2.0 XML and executable `WorkToken` queues using `pugixml`.
  4. Implement `core/3_ledger/crdt_sync_engine.hpp` managing ring buffers of 24-byte `VoxelMutationOp` deltas with HLC timestamps.
  5. Implement `core/2_twin/iot_telemetry_bridge.hpp` with lock-free SPSC ring buffers for MQTT/CoAP telemetry ingress and actuator egress.

#### 3.5. Definition of Done (DoD)
- `tests/epic3_tests.cpp` passes 100% in Catch2.
- `sizeof(VoxelMutationOp) == 24` verified at compile time via `static_assert`.
- Lock-free SPSC ring buffers stress-tested with 1,000,000 concurrent ops with zero data corruption under ThreadSanitizer (TSan).
- Combined Layer 2-7 processing consumes $\le 0.30\text{ ms}$ on the main engine thread.

---

### Epic 4: Scenario Acceptance & Performance Verification Harness

#### 4.1. Executive Scope & Strategic Objective
Establish an automated, deterministic verification harness, CI test runner, and microsecond-level profiling suite that enforces memory limits ($< 30\text{ MB}$ WASM heap), 60 FPS frame budgets ($\le 8.75\text{ ms}$ engine frame), bit-exact deterministic replay, and full scenario validation across Scenario Alpha (Cold Boot to First Harvest) and Scenario Sigma (Altruism Fatigue & Citizen Burnout).

#### 4.2. Persona Value Propositions
- **Expert Game Designer (3D Sims / DF / Skylines):** Verifies that systemic game mechanics produce intended emergent behavior: that survival is challenging but fair, and that mutual aid is sociologically necessary to prevent burnout.
- **Collective Architect (7-Layer Sovereign Stack):** Proves the mathematical determinism of Layers 1-4. Guarantees that two nodes running the same signed intent sequence reach identical state, providing the bedrock for decentralized consensus.
- **Senior C++ Game Architect (Engine Core & DOD):** Provides automated guards against performance regressions, heap bloat, memory leaks, and frame drops. Enforces strict microsecond telemetry across all 9 pipeline stages.

#### 4.3. User Stories & Formal Acceptance Criteria

##### Story 4.1: Deterministic Simulation Headless Verification Runner
- **Description:** Build `oasis_headless_runner` executing 1,000 deterministic ticks without GPU/windowing and verifying state hashes.
- **Given:** The `oasis_headless_runner` binary.
- **When:** Executed with `--ticks 1000 --seed 42`.
- **Then:** Completes 1,000 ticks in $\le 2.0\text{ seconds}$ on CPU, outputting an identical SHA-256 state hash across runs with zero floating-point divergence.

##### Story 4.2: 60 FPS Profiling & Frame Budget Telemetry Harness
- **Description:** Implement high-resolution microsecond instrumentation across all 9 pipeline stages with budget violation alerts.
- **Given:** Engine rendering at $1280 \times 720$ under SDL2/Dawn and WebGPU.
- **When:** Profiled over 600 consecutive frames.
- **Then:** Frame time ceiling is $\le 16.67\text{ ms}$ (60 FPS), average engine frame time is $\le 8.75\text{ ms}$, and no subsystem exceeds its budget over 3 consecutive frames.

##### Story 4.3: Memory Bounds & L1 Cache Residency Verifier
- **Description:** Implement automated test suite validating struct sizes, cache line packing, and total heap usage under AddressSanitizer.
- **Given:** Engine initialized with 18 chunks and 1,024 entities under ASan.
- **When:** Memory audit executes.
- **Then:** Verifies `sizeof(Voxel) == 4`, `sizeof(EntityPhysics) == 12`, `sizeof(VoxelMutationOp) == 24`, total active heap $\le 30\text{ Megabytes}$, and zero dynamic allocations occur during logic/render ticks.

##### Story 4.4: Scenario Alpha (Cold Boot to First Harvest) End-to-End Test
- **Description:** Automated integration test verifying full gameplay progression from Genesis boot to first permaculture harvest.
- **Given:** Lot 402 initialized at $t=0$ with Founder 01.
- **When:** Simulated for 36,000 logic ticks (1 in-game day) executing water catchment, asphalt removal, and crop planting.
- **Then:** Founder 01 survives with calories $> 500$, core temperature $\ge 36.0^\circ\text{C}$, completes first harvest, and terminates municipal fiat drain.

##### Story 4.5: Scenario Sigma (Altruism Fatigue & Citizen Onboarding) Verification
- **Description:** Automated integration test verifying mutual aid mechanics and altruism fatigue burnout prevention.
- **Given:** Founder 01 managing Lot 402 alone.
- **When:** Labor is performed solo for 12 in-game hours, fatigue reaches 255 (burnout); when 2 NPCs are onboarded into Citizens with DIDs and share work tokens.
- **Then:** Founder 01 fatigue stabilizes at $\le 100$, and community productivity increases by $\ge 150\%$.

#### 4.4. Technical Architecture & Concrete Implementation Tasks
- **C++20 Implementation Tasks:**
  1. Implement `core/1_simulation/src/tools/headless_runner.cpp` with tick/seed arguments and SHA-256 state hashing.
  2. Implement `core/1_simulation/src/core/frame_profiler.hpp` with RAII `ScopedTimer` macros measuring all 9 pipeline stages.
  3. Implement `core/1_simulation/src/tests/memory_verifier_test.cpp` asserting struct sizes, alignments, and heap limits under ASan.
  4. Implement `core/1_simulation/src/tests/scenario_alpha_test.cpp` Catch2 integration suite.
  5. Implement `core/1_simulation/src/tests/scenario_sigma_test.cpp` Catch2 integration suite.

#### 4.5. Definition of Done (DoD)
- `tests/scenario_alpha_test.cpp` and `tests/scenario_sigma_test.cpp` pass 100% in Catch2.
- Headless runner executes 1,000 ticks in $\le 2.0\text{ s}$ with matching deterministic state hash.
- Profiling telemetry confirms $\le 8.75\text{ ms}$ average frame time at 60 FPS.
- Memory audit confirms total heap $< 30\text{ MB}$ with zero leaks under ASan/Leaks.

---

## 6. Real-Time 60 FPS Frame Budget Allocation (8.75 ms Target)

To guarantee a locked 60 FPS across both native desktop and browser WASM environments, the engine enforces a strict **8.75 ms frame budget**, leaving **47.5% safety headroom** (equivalent to 114 FPS capability) against the 16.67 ms VSync deadline:

| Stage | Subsystem | Execution Rate | Budget | Headroom & Architectural Rationale |
| :---: | :--- | :---: | :---: | :--- |
| **1** | **Network Ingress & CRDT Sync** | 10 Hz (amortized) | **0.15 ms** | Drain max 64 ops from SPSC ring buffer into memory without heap allocation. |
| **2** | **Voxel Thermodynamics** | 10 Hz (amortized) | **0.80 ms** | 18 chunks ($96 \times 96 \times 64$) thermal diffusion and moisture percolation. |
| **3** | **Kinematic Character Controller** | 60 Hz | **0.25 ms** | Swept-AABB collision and 2-decimeter step-up for Founder 01 and active NPCs. |
| **4** | **Entity DOD & Needs Simulation** | 60 Hz | **0.20 ms** | 12-byte contiguous `EntityPhysics` metabolic decay executed in L1 cache. |
| **5** | **Layer 7 Fiat & BPMN Orchestration** | 10 Hz (amortized) | **0.15 ms** | Integer watt/gallon accumulation and BPMN work token progress evaluation. |
| **6** | **Dirty GPU Buffer Synchronization** | 60 Hz | **0.35 ms** | Uploads only modified $32^3$ chunks via `dirty_chunk_mask` ($< 128\text{ KB/frame}$). |
| **7** | **WebGPU 3D DDA Compute Pass** | 60 Hz | **4.80 ms** | $1280 \times 720$ screen-space rays stepping through 18-chunk envelope (max 180 DDA steps). |
| **8** | **Deferred PBR & Lens Shading** | 60 Hz | **1.20 ms** | Palette lookup, moisture specular puddles, thermal heatmap, and neon magenta tethers. |
| **9** | **UI, Plumbob & Composite Presentation**| 60 Hz | **0.85 ms** | Plumbob vitality diamond rendering, HUD metabolic meters, Cadastral CAD grid overlay. |
| **Σ** | **TOTAL ENGINE FRAME TIME** | **60 FPS** | **8.75 ms** | **47.5% Safety Headroom** (7.92 ms margin under 16.67 ms deadline). |

### WASM Sandbox Memory Footprint Analysis
```
Voxel Storage Grid (18 chunks @ 128 KB)    :   2.359 MB
EntityPhysics Pool (1,024 @ 12 bytes)      :   0.012 MB
EntityKinematics Pool (1,024 @ 28 bytes)   :   0.028 MB
SovereignIdentityStore (1,024 slots)       :   0.090 MB
CRDT Delta Ring Buffer (4,096 @ 24 bytes)  :   0.098 MB
BPMN Work Queues & Blueprints              :   0.128 MB
Camera Uniforms & WebGPU Framebuffers      :  16.000 MB
Static Engine Data & Code Section          :   6.283 MB
─────────────────────────────────────────────────────────
TOTAL ESTIMATED WASM HEAP FOOTPRINT        :  25.000 MB (1.25% of 2 GB browser limit)
```

---

## 7. Dependency Matrix & Phased 4-Sprint Implementation Roadmap

### 7.1. Sprint Dependency Graph
```
┌────────────────────────────────────────────────────────────────────────┐
│ Sprint 1: Memory Foundation & Spatial Envelope (Weeks 1-2)             │
│ - Story 1.1: Multi-Chunk Allocation for Lot 402                        │
│ - Story 1.5: Dirty Chunk Bitmask Buffer Synchronization                │
│ - Story 4.3: Memory Bounds & L1 Cache Residency Verifier               │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│ Sprint 2: Kinematic Embodiment & 3D WebGPU Core (Weeks 3-4)            │
│ - Story 2.1: 12-Byte DOD EntityPhysics Struct                          │
│ - Story 2.2: Swept-AABB Controller & 2-dm Step-Up Height               │
│ - Story 1.2: WGSL 3D DDA Compute Ray-Marching Shader                  │
│ - Story 1.3: Two-Phase Deferred PBR Material Palette                   │
│ - Story 1.4: Dual-Mode Camera Interpolation (400ms)                    │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│ Sprint 3: Living Systems & Sovereign Mesh (Weeks 5-6)                  │
│ - Story 2.3: Sims Metabolic Needs & Plumbob Vitality Indicator        │
│ - Story 2.4: Dwarf Fortress Thermal Mass & Environmental Hypothermia   │
│ - Story 2.5: Physical Tool Actuation & Voxel Harvesting               │
│ - Story 3.1: Layer 7 Municipal Fiat Drain Accumulator                 │
│ - Story 3.2: Decoupled Cold SovereignIdentityStore & Trust Rings       │
│ - Story 3.3: Layer 4 BPMN 2.0 Work Token Compiler                     │
│ - Story 3.4: Layer 3 CRDT Delta Gossip Synchronization                │
│ - Story 3.5: Layer 2 IoT Digital Twin Telemetry Queue                 │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│ Sprint 4: Scenario Acceptance & Verification (Weeks 7-8)               │
│ - Story 4.1: Deterministic Headless Verification Runner               │
│ - Story 4.2: 60 FPS Profiling & Frame Budget Telemetry                 │
│ - Story 4.4: Scenario Alpha (Boot to Harvest) End-to-End Test         │
│ - Story 4.5: Scenario Sigma (Altruism Fatigue) Verification           │
└────────────────────────────────────────────────────────────────────────┘
```

### 7.2. Sprint Breakdown & Milestones
- **Sprint 1: Memory Foundation & Spatial Envelope (Weeks 1–2):**  
  *Deliverable:* Stable multi-chunk allocation (18 chunks, 2.359 MB), dirty chunk bitmask tracking, and Catch2 memory validation under ASan.
- **Sprint 2: Kinematic Embodiment & 3D WebGPU Core (Weeks 3–4):**  
  *Deliverable:* 12-byte `EntityPhysics` struct passing Catch2 tests, swept-AABB kinematic controller with 2-dm step-up, functional WebGPU 3D DDA compute ray-marcher, and 400ms camera interpolation.
- **Sprint 3: Living Systems & Sovereign Mesh (Weeks 5–6):**  
  *Deliverable:* Sims-style metabolic decay, Plumbob vitality indicator, DF thermal exchange, pickaxe harvesting, Layer 7 fiat drain, decoupled Ed25519 DID store, BPMN 2.0 compiler, and CRDT synchronization.
- **Sprint 4: Scenario Acceptance & Verification (Weeks 7–8):**  
  *Deliverable:* Deterministic headless runner, microsecond profiling telemetry, and end-to-end passing integration tests for Scenario Alpha (Boot to Harvest) and Scenario Sigma (Altruism Fatigue).

---

## 8. Universal Definition of Done (DoD)

Every user story across all Epics must satisfy the following six criteria prior to being marked Complete:

1. **Compilation & Warning Hygiene:** Code compiles cleanly with C++20 using `-Wall -Wextra -Wpedantic -Werror` on Clang/GCC and `/W4 /WX` on MSVC, plus Emscripten WASM. Zero compiler warnings.
2. **Memory Alignment & Cache Residency:** All hot structs (`Voxel`, `EntityPhysics`, `VoxelMutationOp`) must satisfy compile-time `static_assert` size and alignment checks. Zero heap allocations in per-frame tick or render loops.
3. **Automated Catch2 Coverage:** All corresponding unit and integration tests compile and pass with 100% assertions in `core/1_simulation/src/tests/`. No skipped or disabled tests.
4. **Profiling Compliance:** All pipeline stages execute within their allocated budgets on the profiling harness ($\le 16.67\text{ ms}$ ceiling, $\le 8.75\text{ ms}$ baseline).
5. **Architectural Decoupling:** Strict traversal protocol maintained. Zero layer-skipping. Cold identity and external I/O strictly isolated to worker threads via lock-free SPSC ring buffers.
6. **Integrity & Genuine State Execution:** All implementations maintain genuine state and produce real physical/systemic behavior. No hardcoded return values, facade implementations, or hollow mocks.
