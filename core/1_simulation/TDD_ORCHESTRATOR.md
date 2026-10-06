# Oasis Engine: Architect Orchestrator Protocol

## Mission Context
You are the **Architect Agent**, tasked with building the Oasis Engine (Layer 1b). The engine is a high-performance, deterministic C++20 physical simulator using strict Data-Oriented Design (DOD). 

Critically, Oasis is a **video game / shadow simulator**. The Scenarios (ScDD) outlined in `docs/03_SCENARIOS.md` imply a player interacting with a voxel environment. Therefore, before *any* ScDD scenarios can be started, the basilar engine foundation must be fully established.

---

## Phase 0: The Basilar Foundation (Current Focus)
Do not attempt ScDD Scenarios until these foundational engine pillars are implemented, tested, and compiling to WASM:

1. **The Game Loop & Context:** Implement a core application loop (e.g., using SDL2 or GLFW) capable of running both natively and in the browser via Emscripten.
2. **WASM Compilation:** Ensure the `CMakeLists.txt` is configured to build the C++20 codebase to WebAssembly using Emscripten.
3. **The Renderer (Compute/Ray-Marching):** Establish the WebGL/WebGPU context capable of rendering the 32-bit voxel grid.
4. **Player Entity & Input:** Create a basic player entity that can move through the chunk manager and interact with voxels (placing/mining).
5. **CRDT Local State:** Implement the foundational local-first persistence (IndexedDB/OPFS bindings) for saving the chunk state.

---

## Phase 1+: Scenario-Driven Development (ScDD)
*Only proceed to this phase once Phase 0 is complete.*

Once the game engine is playable and running in the browser, execute the following loop to integrate the physics:
1. **Scenario Ingestion:** Read the next unstarted Scenario from `docs/03_SCENARIOS.md`.
2. **Physical Integration:** Update the C++ voxel interactions (thermodynamics, Exergy drain, intent ingestion) to model the scenario's constraints.
3. **Player Testing:** Ensure the player can interact with the scenario bounds within the game loop.
4. **Refactor & Sync:** Mark the scenario as 🟡 **Prototyped** when initial mocks pass, and 🟢 **Integrated** when fully realized.
