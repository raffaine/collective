# Bootstrap Strategy: The AI-Human Studio

Building a 7-layer thermodynamic operating system simultaneously with a C++20 voxel game engine requires a highly compressed, asymmetrical development model. Traditional software studios scale by adding human headcount, which introduces massive communication overhead and fiat burn rates. 

The Collective bootstraps itself using an **AI-Human Studio Model**. In this paradigm, Large Language Models (LLMs) act as highly specialized, parallelized execution agents, while the Human acts as the sole cryptographic authority, thermodynamic verifier, and architectural governor. 

Because Oasis and The Collective are a synergistic-metamorphic whole, every line of C++ code written for the game engine and every JSON schema formulated by the agents directly compiles the infrastructure for the physical world.

---

## 1. The AI Agent Panorama (Execution & Orchestration)

To pulverize the workload of Scenario-Driven Development (SDD), the studio deploys specific AI personas. These agents do not cross boundaries; they are strictly siloed by their layer responsibilities to prevent architectural bleed.

*   **The Architect (Layers 1, 2, & Engine Core):** Dedicated entirely to the C++20 engine, Vulkan compute shaders, and WebAssembly (WASM) translation. Responsible for the Data-Oriented Design (DOD) of the voxel chunks and ensuring the simulation runs deterministically at 60 FPS in a local browser.
*   **The Orchestrator (Layers 3 & 4):** Writes the complex state machines. Responsible for generating W3C-compliant BPMN 2.0 XML files, CRDT network sync logic, and defining the Thermodynamic Triangulation math for the ledger.
*   **The Semantic Scribe (Layers 5 & 6):** Translates physical reality into human intent. Responsible for drafting the JSON-LD schemas (Knowledge Artifacts, Verifiable Credentials, Trust Rings) and the UI/UX scaffolding that allows human players to interact with the system without seeing raw code.
*   **The Legal Proxy (Layer 7):** Formulates the legacy interfaces. Drafts the Social Purpose Corporation charters, Perpetual Purpose Trust deeds, and localized municipal compliance protocols for physical deployment (e.g., zoning overrides for Duvall/Cascadia environments).

---

## 2. Scenario-Driven Development (SDD) Pipeline

Development is strictly horizontal, slicing through all 7 layers scenario by scenario (Alpha through Omega). The AI-Human studio executes this loop:

1.  **Scenario Selection:** The Human pulls a scenario from the `03_SCENARIOS.md` dashboard (e.g., Scenario Alpha: The Fabrication Commons).
2.  **Agent Parallel Execution:**
    *   *The Architect* mocks up the C++ `FabricationNode` struct.
    *   *The Orchestrator* writes the BPMN logic for printer queuing.
    *   *The Semantic Scribe* drafts the `FabricationBounty` JSON-LD schema.
3.  **Synthesis:** The agents compile their outputs into a single, cohesive Pull Request within the `arch-v2-genesis` branch.

---

## 3. Human Supervision Gates (The Meatspace API)

AI agents hallucinate, drift from constraints, and do not possess physical bodies. They cannot know if a voxel moisture model accurately reflects the water retention of fungal loam. The Human provides the ultimate ground-truth friction. 

No AI code or schema becomes canon until it passes these strict supervision gates:

### Gate 1: The Architectural Freeze
The Human sets the immutable laws of the system. The agents cannot alter the 7-Layer stack, they cannot introduce fiat dependency into Layer 3, and they cannot bypass the physical Level of Detail (LoD) requirements. If an agent suggests a centralized database to solve a sync issue, the Human rejects the payload and forces a CRDT rewrite.

### Gate 2: Thermodynamic & Hardware Verification
The game engine must perfectly simulate the physical hardware it will eventually govern. When the agents draft the BPMN workflow for 3D printing, the Human tests the logic against reality. If the game's simulated logic fails to accurately predict the filament consumption and time-to-completion of a physical Bambu Lab A1 Mini printing a multi-color part via AMS, the engine logic is rejected and recalibrated.

### Gate 3: Cryptographic Sign-Off
AI agents cannot hold trust. They cannot sign Verifiable Credentials. The Human is the sole cryptographic anchor for the Genesis Node. The agents may format the perfect JSON-LD `SkillAttestation` for a local teenager, but the payload is inert until the Human physically reviews the work and applies their private key signature.

---

## 4. Metamorphosis: From Studio to System

This bootstrap strategy contains a deliberate phase transition. 

In the beginning, the AI agents are building a game (Oasis). But as the scenarios progress, the schemas they generate—like the JSON-LD payload for a local rideshare or the BPMN file for a rainwater loop—are cached in the repository. 

When the human player finishes testing a system in the Oasis simulation, they do not need to rewrite the software for the real world. They simply point the Oasis engine's Layer 2 MQTT broker at their local hardware. The game studio dissolves, and the 7-layer operating system for The Collective is born.
