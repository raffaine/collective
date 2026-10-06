# C4 Architecture & Backlog Refinement Protocol

## Mission
You are acting as a dual-role **Product Owner & Software Architect**. To prevent costly rework and ensure our foundational layers are rock-solid, you must apply the **C4 Model** (Context, Containers, Components, Code) to the Oasis Engine before the Scrum team begins deep implementation.

## Execution Steps

### Step 1: Generate the C4 Visualizations
Create a new artifact at `docs/C4_ARCHITECTURE.md`. Using Mermaid.js, generate the first three levels of the C4 model for the Oasis Engine:
1. **Level 1: System Context Diagram** 
   - Show how the Oasis Engine interacts with the Player, Layer 2 IoT Sensors, the Layer 4 Orchestrator, and the Local File System.
2. **Level 2: Container Diagram** 
   - Break the engine down into its deployable units (e.g., The WASM Browser Client, The Native Debug Client, the IndexedDB/OPFS Local Storage, the C++ Core).
3. **Level 3: Component Diagram** 
   - Zoom into the C++ Core. Map out the `ChunkManager`, `ThermodynamicEngine`, `BPMN_XML_Parser`, `CRDT_Mesh_Sync`, `DID_Crypto_Generator`, and the `SDL2_Abstraction_Layer`. 

### Step 2: Backlog Refinement & Component Mapping
Once the architecture is visualized, review the existing `PRODUCT_BACKLOG.md`. 
1. **Map to Components:** Explicitly tag each User Story with the C4 Component it belongs to (e.g., `[Component: ThermodynamicEngine]`).
2. **Identify Missing Interfaces:** Are there stories missing that define the APIs *between* these components? (e.g., How does the `BPMN_XML_Parser` talk to the `ChunkManager`?). Add these foundational interface stories.
3. **Re-sequence for Stability:** Reorder the backlog to ensure we build from the core outward. We must not build the UI or WebGPU renderer before the foundational C++ component interfaces are strictly defined.

### Step 3: Final Output
Do not stop until `docs/C4_ARCHITECTURE.md` is complete with Mermaid diagrams, and `PRODUCT_BACKLOG.md` has been rewritten to reflect the exact component boundaries and optimal build sequence.
