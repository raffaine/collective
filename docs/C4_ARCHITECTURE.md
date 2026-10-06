# C4 Architecture Model: Oasis Engine

This document visually maps the architecture of the Oasis Engine across the Context (Level 1), Container (Level 2), and Component (Level 3) layers. It serves as the structural foundation for our Product Backlog and Sprint sequences.

## Level 1: System Context
The Oasis Engine operates as a bridge between the physical realities of the Sovereign Node and the semantic intent of the citizens.

```mermaid
C4Context
    title System Context diagram for Oasis Engine

    Person(player, "Citizen / Steward", "Interacts with the digital twin and issues semantic intents.")
    
    System(oasis, "Oasis Engine", "C++20/WASM simulation game and physical digital twin.")
    
    System_Ext(layer2, "Layer 2: IoT Sensor Mesh", "MQTT brokers feeding physical telemetry to the engine.")
    System_Ext(layer4, "Layer 4: Orchestrator", "BPMN execution engines that actuate real-world hardware.")
    System_Ext(ipfs, "Layer 6: Semantic Storage", "IPFS / Local CRDT Mesh for persistent knowledge artifacts.")

    Rel(player, oasis, "Plays simulation, designs blueprints, signs intents")
    Rel(layer2, oasis, "Provides physical voxel telemetry")
    Rel(oasis, layer4, "Emits verified BPMN schemas for actuation")
    Rel(oasis, ipfs, "Syncs decentralized state and bounties")
```

## Level 2: Container Diagram
The deployment architecture, demonstrating how the identical C++ core runs natively and in the browser.

```mermaid
C4Container
    title Container diagram for Oasis Engine

    Person(player, "Citizen / Steward", "Uses standard browser or desktop OS.")

    System_Boundary(oasis_boundary, "Oasis Engine") {
        Container(browser, "Web Application", "HTML/JS", "The browser-based shell serving the WASM binary.")
        Container(desktop, "Native Application", "C++ Binary", "The native desktop shell for high-performance development.")
        
        ContainerDb(localdb, "Local DB", "IndexedDB / OPFS", "Sandboxed local persistence for chunks and keys.")
        
        Container(cpp_core, "C++20 Engine Core", "WASM / Native Library", "The headless physics, voxel manager, and state machine.")
    }

    Rel(player, browser, "Plays via WebRTC/Canvas")
    Rel(player, desktop, "Plays natively")
    
    Rel(browser, cpp_core, "Emscripten Bindings (emcall)")
    Rel(desktop, cpp_core, "Direct memory access")
    
    Rel(cpp_core, localdb, "Persists CRDT state")
```

## Level 3: Component Diagram (C++ Core)
Zooming into the `C++20 Engine Core` container. This maps perfectly to our Epic and Story boundaries in the Product Backlog.

```mermaid
C4Component
    title Component diagram for the C++20 Engine Core

    Container_Boundary(core_boundary, "C++20 Engine Core") {
        Component(sdl2, "SDL2 Universal Abstraction", "C++", "Handles windowing, DOM event mapping, and WebGL context bindings.")
        Component(chunk_mgr, "ChunkManager", "C++", "DOD voxel memory layout and spatial querying.")
        Component(thermo_engine, "ThermodynamicEngine", "C++", "Calculates Exergy, heat transfer, and fluid decay via Tick().")
        Component(bpmn_parser, "BPMN XML Parser", "C++ / pugixml", "Ingests Layer 4 blueprints and mutates chunk states.")
        Component(crdt_sync, "CRDT Sync Engine", "C++ / automerge", "Tracks chunk deltas and serializes to IndexedDB/Network.")
        Component(crypto_did, "DID Crypto Generator", "C++ / libsodium", "Handles Ed25519 signing for local identity.")
        Component(webgpu, "WebGPU Renderer", "C++ / Dawn", "Ray-marches the ChunkManager state to the SDL2 context.")
    }

    Rel(sdl2, webgpu, "Provides render context")
    Rel(sdl2, chunk_mgr, "Passes input events")
    
    Rel(thermo_engine, chunk_mgr, "Reads/Writes voxel states")
    Rel(bpmn_parser, chunk_mgr, "Actuates workflows on voxels")
    
    Rel(crdt_sync, chunk_mgr, "Listens to voxel mutations (IChunkObserver)")
    Rel(crypto_did, bpmn_parser, "Signs automation intents")
    
    Rel(webgpu, chunk_mgr, "Copies voxel data to VRAM (IChunkBuffer)")
```
