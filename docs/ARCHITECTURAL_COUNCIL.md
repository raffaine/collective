# Multi-Agent Architecture Council: Oasis Engine Reboot

## 1. Objective
Pull the brakes. While the headless simulation mechanics (Thermodynamics, Entity State Machines, DID generation) function in C++, the visual and interactive execution currently feels disjointed from a true 3D simulation game. 

We must build a fresh Product Backlog that bridges the gap between deep C++ systems architecture, the 7-Layer Sovereign Collective model, and the tangible, empathetic gameplay of *Dwarf Fortress*, *The Sims*, and *Cities Skylines*. We are not throwing away the C++ foundation, but we are rewriting the blueprint for how it comes together.

## 2. The Council (Roles)

**Agent 1: Orchestrator & Product Owner (Lead)**
*   **Responsibility:** Guides the conversation, maintains focus, enforces consensus, and ultimately writes the new `PRODUCT_BACKLOG_V3.md`.
*   **Focus:** Actionable epics, maintaining momentum, and ensuring the backlog doesn't become overly theoretical.

**Agent 2: Expert Game Designer (3D Sims)**
*   **Responsibility:** Injects the design philosophies of *Dwarf Fortress* (deep psychology/systems), *The Sims* (accessible visual empathy, player character interaction), and *Cities Skylines* (macroscopic flow).
*   **Focus:** How does the player *feel*? How do they interact with the Player Character and the Node? How do we visualize the simulation in 3D?

**Agent 3: Collective Architect**
*   **Responsibility:** Ensures the game mechanics perfectly mirror Volume 1 of the Sovereign Stack and the 7-Layer Model.
*   **Focus:** Physical layer reality, real-intent compilation, DID cryptography, and how NPCs transition into Citizens.

**Agent 4: Senior C++ Game Architect**
*   **Responsibility:** Grounds the design in technological reality. Evaluates rendering tech (WebGPU, Vulkan, OpenGL), Entity-Component-Systems (ECS), and cross-compilation (Native vs. WASM).
*   **Focus:** Keeping the architecture decoupled, performant, and capable of rendering a 3D isometric/perspective world without destroying our headless simulation.

## 3. Phase 1: Foundations & Genesis (3 Rounds)
The council will execute 3 strictly orchestrated rounds to define the genesis of a Node and the Player Character.

*   **Round 1: Conceptual Alignment:** Define the "Node Genesis" (What does the world look like the second the game boots?) and the "Player Character" (Who is the player? An avatar? An omniscient cursor?). 
*   **Round 2: Technological Reality Check:** The C++ Architect and Game Designer bridge the conceptual ideas with our existing codebase. How do we take our 32-bit Voxels and 12-byte Entities and render them in a compelling 3D space?
*   **Round 3: Backlog Genesis:** The PO formalizes the agreements from Rounds 1 & 2 into a brand new set of Epics. 

## 4. Phase 2: Scenario Driven Development (ScDD)
Once Phase 1 is complete, the Council will transition into ScDD. The Orchestrator will feed the 24 scenarios from `docs/scenarios/` into the council, one iteration at a time. The Council will debate how the architecture and game design handles each scenario, refining the Backlog dynamically.

## 5. Consensus Protocol
*   The Orchestrator must explicitly ask for a "Sign-off" from the Game Designer, Collective Architect, and C++ Architect at the end of each round.
*   If any agent objects, the round continues until the conflict is resolved. No progression to the next round without unanimous agreement.
