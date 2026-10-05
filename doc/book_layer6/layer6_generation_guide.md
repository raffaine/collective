# Layer 6 Interfaces & Experience: Architecture Generation Guide

## Core Mandate
This document governs the authoring of `doc/book_layer6/`. Layer 6 is the Human-Machine Boundary—the physical and digital interfaces where citizens actually interact with the Sovereign Stack (The Oasis Engine). This is not about Web2 React apps; this is about **Local-First software, Conflict-Free Replicated Data Types (CRDTs), Spatial Interfaces (AR/E-ink), and Cognitive Load Management**.

**Rule 1: Core Technology Chapters.** The book must be structured around core mechanisms rather than a catalog of scenarios. 
**Rule 2: The Physical Reality of Interfaces.** User Interfaces consume electricity. The core chapters MUST detail the thermodynamic constraints of rendering UI: OLEDs vs. ambient E-ink, GPU power for AR overlays, and the battery constraints of mobile devices running local mesh nodes.
**Rule 3: The Scenario Appendix.** The 25 Sovereign Scenarios (Scenario 0 through Omega) will be moved to the Appendix. They serve as an exhaustive reference proving that the core UI/UX technologies can handle every possible interaction.
**Rule 4: The Iterative Feedback Loop.** If a scenario appendix (e.g., Scenario Omega - Complete Decoupling) uncovers a missing interface mechanic (like offline mesh-syncing UI patterns), the agent MUST halt, invent the solution, and **refine the Core Chapters** to implement it before finishing the appendix.

## Required Core Chapter Layout (To Be Authored)
*   **Chapter 1: Local-First Synchronization \& CRDTs** (Bypassing centralized cloud servers. How user devices maintain state locally and sync opportunistically via the offline mesh).
*   **Chapter 2: Spatial \& Ambient Interfaces** (Interfacing with physical environments. Utilizing low-power E-ink signage for communal resources, and AR overlays for spatial policy zoning).
*   **Chapter 3: Cognitive Load \& Algorithmic Triage** (How the system shields humans from information overload. Designing the "Centaur" interface for human-agent collaboration without notification fatigue).
*   **Chapter 4: Thermodynamic Rendering \& Hardware Limits** (The physical cost of pixels. Dynamically degrading UIs from rich graphics to raw text based on microgrid power availability).

## Authoring Guidelines for Each Appendix
When writing the 25 scenario appendices, you must strictly follow this internal structure:
1.  **The Legacy Paradigm & Its Failures:** How does the current world do this? (e.g., cloud-dependent apps, addictive notification loops, dark patterns).
2.  **The Incremental Automation Pathway:** Transitioning from manual human data entry (Phase 1) to Ambient/AR assistance (Phase 2), to fully ephemeral, zero-click interactions (Phase 3).
3.  **The Human Boundary (Limits of Automation):** Exactly where the interface must force a human to pause, think, and manually confirm an action (Friction as a feature).
4.  **Interface Mechanics (The "How"):** Deep dive into the CRDT data structures, the UI rendering states, and the physical interaction models (voice, haptic, visual). If writing this section reveals a gap in the core architecture, trigger **Rule 4 (The Iterative Feedback Loop)**.
