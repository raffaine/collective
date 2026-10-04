# 09. The Prime Genesis: Evolutionary Bootstrapping

*   **System Epic:** Bootstrapping the Codebase, Escaping the Scaffolding, and Evolutionary Governance
*   **Objective:** We cannot birth a fully sovereign, decentralized Node on Day 1 while relying on legacy corporate infrastructure (GitHub, Cloud CI/CD) to build it. This document outlines the incremental roadmap to achieve freedom—transitioning from brittle legacy representations to the final, mathematically pure thermodynamic forms.

---

## 1. The Paradox of the Scaffolding

To build the Sovereign Stack, we must initially rely on the very legacy systems we intend to obsolete. If we attempt to instantly apply the strict 7-Layer rules to a GitHub repository (e.g., forcing JSON-LD formats for all issues, or treating Git commits as a thermodynamic ledger), the system will collapse under bureaucratic friction and abstraction leaks.

Instead, we treat our current environment as **Temporary Scaffolding**. We will execute a phased decoupling, shedding legacy dependencies layer by layer as the Oasis Engine matures.

---

## 2. The Evolutionary Phases of Node 0

### Phase 1: The Brittle Approximation (Days 1–30)
In this phase, we accept that we are entirely subservient to Layer 7 legacy infrastructure (GitHub). The goal is raw velocity to build the C++ Engine.
*   **L6 (Intent):** We use standard, unstructured GitHub Issues. We do not force humans to write JSON-LD.
*   **L5 (Governance):** We use GitHub `CODEOWNERS` and basic PR approvals as a brittle simulation of the Trust Ring.
*   **L4 (Orchestrator):** We rely on centralized GitHub Actions to compile our WASM payload.
*   **L3 (Ledger):** We do not track tokens. We rely on the unquantified altruistic labor of the Human and AI Stewards.

### Phase 2: The Hybrid Bridge (The Headless Engine)
Once the C++ Engine can run headless state machines (per Strategy 08), we begin routing logic *through* it, though still hosted on legacy infrastructure.
*   **L6 (Intent):** We introduce a bridge. A GitHub Issue can now contain a JSON-LD block. A GitHub Action parses this block and feeds it into the headless Oasis Engine for validation before a PR can be merged.
*   **L4 (Orchestrator):** The C++ BPMN engine now governs internal state (e.g., validating a `FabricationIntent` test), but GitHub Actions still acts as the chronological trigger.

### Phase 3: The Sovereign Interface (Self-Hosting)
The Oasis WebGL client is live and capable of local P2P networking. The Node begins to govern itself using its own software.
*   **L6 & L4 (Intent & Orchestrator):** GitHub Issues are deprecated for Node operations. Stewards use the Oasis UI to emit `Intents` directly into the local CRDT mesh. The local C++ engine orchestrates the tasks without touching the cloud.
*   **L5 (Governance):** `CODEOWNERS` is deprecated for operational trust. We transition to true cryptographic Key-Signing Parties (Scenario Omicron).

### Phase 4: The Terminal Decoupling (Scenario Omega)
The final shedding of the scaffolding.
*   **Codebase Migration:** The source code of the Oasis Engine is moved off Microsoft/GitHub entirely. It is hosted on a decentralized P2P git protocol (e.g., Radicle) or natively distributed across the Node's own CRDT ledger.
*   **L3 (Ledger):** The thermodynamic ledger is activated, tying actual physical exergy to the maintenance of the codebase.
*   **True Autonomy:** Node 0 exists purely on local hardware, governed by its own laws, completely illegible to the legacy state.

---

## 3. The Immediate Execution Path

To begin Phase 1, we must lay the physical scaffolding for the codebase. We will create the foundational directories mapping to the engine architecture:
*   `core/` (C++20 Oasis Engine & WASM)
*   `orchestrator/` (Proto-BPMN and bridging scripts)
*   `ledger/` (CRDT networking stubs)
*   `ui/` (Web frontend payload)

The first step is to build the scaffolding so we can eventually burn it.