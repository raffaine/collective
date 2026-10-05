# Engine Spec: Oasis C++ Core (Layer 1b)

## 1. Directive for AI Agents
**Target Persona:** The Architect Agent
**Objective:** Parse the semantic ontologies established in `core/1a_knowledge/` (e.g., `ontology_core.jsonld`) and translate them into a high-performance C++20 voxel simulation engine.
**Constraints:**
- No centralized databases.
- Memory must be laid out in strict Data-Oriented Design (DOD) structures (Structs of Arrays / Arrays of Structs).
- Must compile to WebAssembly (WASM) via Emscripten.
- Must execute deterministically.

## 2. The Semantic to Memory Mapping

The JSON-LD schemas in Layer 1a define the *concept* of reality. Layer 1b must simulate it.

### A. Voxel Representation
The ontology defines `mesh:Voxel`. The C++ engine must allocate this as a highly compressed 32-bit integer grid.
*   `8 bits`: Material ID (Air, Biomass, Concrete, Steel).
*   `8 bits`: Temperature (Scaled representation).
*   `8 bits`: Structural Integrity (Damage/Decay).
*   `8 bits`: Meta/Ownership index.

### B. The Exergy Loop (Thermodynamics)
The ontology defines `mesh:Exergy`. The engine must run a `Tick()` loop that constantly drains Exergy from executing tasks. 
If an entity accepts a `FabricationIntent` to print a part, the engine must simulate the joules of electricity consumed over time. If the battery voxel hits `0`, the machine halts, and the Intent fails.

## 3. The Required Interface (Handoff to Layer 4)
The engine must expose a C-style API (or WASM bindings) that allows the Orchestrator (Layer 4) to inject an `Intent`.
1.  **Ingest:** `bool InjectIntent(const char* json_payload);`
2.  **Validate:** The engine mathematically simulates the intent (e.g., "Do we have enough physical Exergy to build this?").
3.  **Resolve:** Returns a success or failure state to Layer 4 to settle the Ledger (Layer 3).

## 4. Bootstrapping Task
The first required code contribution to `1b_simulation` is to write `main.cpp` demonstrating a headless memory allocation of a $32 \times 32 \times 32$ voxel chunk, modifying a voxel's temperature, and logging the state to `stdout`.
