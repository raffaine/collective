# The Collective: A Decentralized Operating System for Physical Sovereignty

The Collective is not a blockchain project; it is a localized, thermodynamic operating system. It routes physical resources, ecological stewardship, and human labor across a decentralized peer-to-peer mesh, systematically severing dependency on legacy corporate and municipal infrastructure (The Legacy Tether).

To bootstrap this reality, we are building a dual-interface **Trojan Horse**:
1. **Oasis (Layer 1):** A local-first, WASM-compiled C++20 voxel simulation. It acts as the physical sandbox where you designate zones, build infrastructure, and manage biological needs.
2. **The Companion App (Layer 6):** A mobile-responsive web dashboard connecting to the native POSIX daemons (The Sovereign Stack). It handles the societal layer—governance, Agora trading, and diagnostic telemetry—strictly airgapped from the 3D simulation.

## The 7-Layer Architecture
The system enforces a strict separation of concerns, mapping human intent down to physical thermodynamics without centralized servers. The Companion App enforces the **Intent U-Turn**: Human action enters at Layer 6, pushes *down* to Layer 5 (Governance) and Layer 4 (Execution), and only yields *up* to Layer 7 (Legacy Proxy) if necessary.

*   **Layer 7 (The Legacy Proxy):** The boundary interface for fiat ingestion and legacy shock monitoring.
*   **Layer 6 (The Semantic Layer):** The primary entry point. Human intent, Agora UI, and plain-language translation.
*   **Layer 5 (The Governance Layer):** Mutual credit limits, polycentric treaties, and the consensus brain.
*   **Layer 4 (The Execution Layer):** Deterministic BPMN execution sandbox and diagnostic readouts.
*   **Layer 3 (The Ledger Layer):** Content-Addressed CRDTs and asynchronous peer-to-mesh networking (`col-meshd`).
*   **Layer 2 (The Digital Twin):** Edge telemetry, MQTT brokers, IoT actuators, and UHAI shared memory.
*   **Layer 1 (The Physical Reality):** The Oasis Engine. Soil, water, joules, and human caloric labor.

## The Alpha Release & Usage
The project is approaching its first playable Alpha. The architecture is split between the native POSIX daemons and the WebAssembly game client.

**For Developers:**
A unified launch script is provided to compile the C++ engine and spin up the local HTTP servers:
```bash
./scripts/start_dev.sh
```
- **Oasis (Layer 1)** will be available at `http://localhost:8080`
- **Companion App (Layer 6)** will be available at `http://localhost:8081`

**For Alpha Testers:**
We package distributable tarballs containing the compiled Native and WASM binaries:
```bash
./scripts/build_release.sh
```
This generates `release/collective_alpha.tar.gz` which testers can run locally with zero build dependencies.

## The Horizon (Upcoming Features)
We are actively executing against the Horizon Roadmap to deepen the simulation before the Alpha launch:
- **Migration Waves & Legacy NPCs:** Assimilating legacy citizens into the mutual credit economy.
- **Arcball Spatial Designation:** Tactile right-click context menus and orbital 3D camera controls.
- **Psychological Homeostasis:** Balancing brutal thermodynamics with restorative "Chill Loops" for the Founder.
- **Simulation Time-Warp:** Decoupling simulation speed (1x, 10x, 100x) while asynchronously resolving Temporal Flow Contracts across the mesh.

## Valuenomics & SDD
Value Tokens are not fiat. They are cryptographic representations of localized negentropy. The engine is built using **Scenario-Driven Development (SDD)**, tested against 24 socio-ecological stress tests to guarantee infrastructural resilience against the Legacy Tether.
