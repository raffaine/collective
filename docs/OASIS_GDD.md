# OASIS GAME DESIGN DOCUMENT (GDD)
**Document Identifier:** `OASIS-GDD-V6.0`  
**Milestone:** Horizon & Playability Council Ratification (The Horizon & Playability Edition)  
**Classification:** Authoritative Game Design Specification & SITL Shadow Simulator Blueprint  
**Engine Baseline:** C++20 (`-std=c++20`) / WebGPU (WGSL) / Flecs ECS / UHAI IPC  
**Status:** Council Approved (Unanimous Consensus Reached)  
**Date of Enactment:** 2026-10-14  

---

## Horizon & Playability Council Ratification & Roll

This document embodies the synthesized, legally and architecturally binding consensus of the Oasis GDD & Strategic Realignment Council. Building upon `docs/COLLECTIVE_GUIDING_PRINCIPLES.md` and `docs/BOOK_TEAM_DRIFT_REPORT.md`, Version 6.0 establishes **The Six Horizon Playability Pillars**, enforces the **Companion App Interaction Boundary (V2.0 Law)**, codifies strict **Outward-Only Telemetry Demarcation (`QUALITY_SIMULATED = 0x01`)**, and grounds **Scenario Driven Development (SDD)** as the primary cybernetic engine driving emergent gameplay within the Oasis Simulation Layer.

### Council Roll & Formal Ratification Statements

1. **Expert Game Designer (Oasis GDD Council Lead):**  
   > *"Modern video games have become extractive cognitive pacifiers: Skinner boxes with colored motive bars, god-mode cursors, and sanitized corporate fantasy worlds. Oasis stands as the Shadow Simulator and Cooperative Bioregional Onboarding Engine. By fusing the historical psychological trauma of Dwarf Fortress, the empathetic, indirect agency of The Sims, and the macroscopic infrastructure reclamation of Cities: Skylines onto a local-first WebGPU 3D voxel engine, we build a simulation where every ludic mechanic compiles into real-world stewardship competence. In V6.0, we introduce the Six Horizon Playability Pillars—tactile Spatial Context Menus, Orbital POI Camera kinematics, Psychological Homeostasis ('The Chill Loop'), Temporal Time-Warp controls, Legacy NPC Migration Waves, and the shared `Arcadia (server-01)` MMO mesh synchronized by Mandatory Epoch Milestones. I ratify this V6.0 specification unconditionally as our foundational ludic constitution."*

2. **Gamer Expert (Dwarf Fortress / The Sims / Cities: Skylines Specialist):**  
   > *"Gamers possess zero tolerance for fake mechanics or clunky controls. If an agent cannot recover from freezing cold in an insulated room above $18^\circ\text{C}$, if selecting voxel actions requires memorizing arcane keyboard shortcuts instead of tactile right-click spatial menus, or if 3D navigation lacks a true Point-of-Interest Arcball camera, playability suffers. In V6.0, we pair our 64-byte bounded Dwarf Fortress psychology, Sims-style indirect intention queues, and Successive Over-Relaxation (SOR) electrical physics with the 'Chill Loop' recovery systems, involuntary 100% Stress Burnout states, Legacy NPC assimilation waves, and $1\times\text{–}100\times$ Time Warp. Players will experience authentic emergent drama with razor-sharp strategy UX. I ratify this document with utmost enthusiasm."*

3. **Sovereign Stack Specialist:**  
   > *"The boundary between simulation and bare-metal reality must remain sacred and unmistakable. In full alignment with `docs/BOOK_TEAM_DRIFT_REPORT.md`, V6.0 eradicates any narrative ambiguity: Oasis is strictly an outward producer of `QUALITY_SIMULATED (0x01)` telemetry into the Sovereign Stack for Software-In-The-Loop (SITL) stress-testing, and never ingests live physical telemetry to recalibrate the simulator. Furthermore, V6.0 ratifies the V2.0 Companion App Interaction Boundary Law: Layer 1 (Oasis Desktop) governs all 3D spatial actuation and voxel designations, whereas Layer 6 (Mobile Companion App via `col-gatewayd`) serves as the exclusive Agora/societal interface following the `L6 -> L5 -> L4 -> [yield] -> L5 -> L6 -> L7` Intent U-Turn, with Layers 2–5 and 7 acting strictly as read-only cybernetic diagnostics. I ratify this specification unconditionally."*

4. **Oasis C++ Engine Architect:**  
   > *"Our technical commitments are absolute. The Oasis engine operates within a strict 256.00 MB WASM linear memory partition with zero heap allocations on the simulation tick. Rendering is executed by an Amanatides & Woo 3D DDA compute raymarcher in WebGPU WGSL at a locked 60 FPS ($< 10.60\text{ ms}$ frame budget), now completely decoupled from the variable simulation timestep ($\Delta t_{\text{sim}}$) to support $1\times$, $10\times$, and $100\times$ Time Warp with deterministic log throttling. DDA raycasting directly powers our dynamic POI Orbital Camera and Spatial Context Menu evaluator, while Sprint 9 Temporal `FlowContract` ring buffers and Mandatory Milestone Epoch barriers keep multi-node `Arcadia (server-01)` state causally consistent. I ratify this specification."*

5. **Quality Engineer (Automation & Gatekeeper):**  
   > *"An unverified design is merely wishful thinking. This V6.0 GDD enforces Six Inviolable Quality Gates (G1 through G6) spanning thermodynamic conservation, offline local-first resilience, Byzantine and Sybil immunity, psychological homeostasis & anti-burnout recovery, cooperative bioregional pedagogical equivalence, and outward-only `0x01` hardware airgap isolation. All 24 canonical MVS scenarios and all 6 Horizon Playability Pillars must pass automated headless simulation harnesses before feature completion is declared. I ratify this specification."*

---

### Unanimous Consensus Declaration

```
════════════════════════════════════════════════════════════════════════════════════════
                      HORIZON & PLAYABILITY COUNCIL (V6.0)
                                RATIFICATION LOG
════════════════════════════════════════════════════════════════════════════════════════
[VOTE] Expert Game Designer (Lead)     : AYE (Unconditional)
[VOTE] Gamer Expert (DF/Sims/Cities)   : AYE (Unconditional)
[VOTE] Sovereign Stack Specialist      : AYE (Unconditional)
[VOTE] Oasis C++ Engine Architect      : AYE (Unconditional)
[VOTE] Quality Engineer                : AYE (Unconditional)

STATUS: Unanimous Consensus Reached
DATE: 2026-10-14T23:30:00Z
ACTION: ENACTED AS CANONICAL GAME DESIGN SPECIFICATION (V6.0 — HORIZON EDITION)
════════════════════════════════════════════════════════════════════════════════════════
```

---

## 1. Executive Vision & Foundational Philosophy

### 1.1. The Shadow Simulator & Cooperative Bioregional Onboarding Engine
Oasis is not an escapist digital distraction, an idle clicker, or an idealized greenfield sandbox. It is the **Shadow Simulator** and **Cooperative Bioregional Onboarding Engine** for physical-world sovereign resilience.

Modern game design reinforces comfortable, detached fallacies that disarm the human intellect:
* **The Sims Fallacy (Mechanical Puppetry):** Human consciousness is reduced to six colored motive bars. Grief, existential terror, and community betrayal are instantly remedied by thirty seconds of microwave pizza or a television program. Characters have no historical memory, no lived trauma, and no ethical conviction.
* **The Cities: Skylines Fallacy (The Magic Greenfield):** Settlements sprout ex nihilo upon pristine, virgin greenfields. Infinite electrical power and pure potable water flow from the edges of the map in exchange for abstract fiat currency, supported by a benevolent, invisible municipal state.
* **The Colony Sim Fallacy (The Authoritarian God-Cursor):** In titles like *RimWorld* or *Banished*, the player is an omnipotent floating despot. A single click forces an exhausted, freezing mortal pawn to march into lethal hypothermia, industrial radiation, or suicidal combat without friction, moral hesitation, or psychological collapse.

Oasis rejects every one of these fallacies. The real Anthropocene offers no pristine greenfield, no infallible municipal utility, and no obedient digital pawns. Humanity exists within the aging, fragile infrastructure of late-stage industrial civilization: cracked suburban asphalt, overloaded 12kV distribution lines, decaying cast-iron water mains, rigid municipal zoning codes, and volatile inflationary fiat tariffs.

Oasis simulates the authentic, gritty, and exhilarating physics of **cooperative bioregional sovereignty**:
1. **Metabolic Scavenging & Infrastructure Reclamation:** You do not begin with an off-grid permaculture haven; you start as an economically vulnerable enclave **structurally bridging and reclaiming** materials and utility flows from aging legacy infrastructure while building regenerative capacity.
2. **Embodied Indirect Stewardship:** You do not command obedient pawns; you guide **Founder 01**, a mortal human steward with circadian fatigue, thermal homeostasis needs, ethical convictions, and an emergent psychological inner life, who must later welcome and assimilate **Migration Waves** of stressed Legacy NPCs.
3. **Progressive Autarky & Voluntary Mesh Federation:** You do not win through imperial map painting; you systematically **achieve thermodynamic autarky**, regenerate depleted soil, form local Web-of-Trust mutual-credit circles, and voluntarily federate with peers across shared virtual landscapes like **`Arcadia (server-01)`**.

Every gameplay system in Oasis acts as a **cybernetic compiler**: player mastery over insulated room envelopes, rocket mass heaters, photovoltaic charge controllers, keyline swales, biochar filtration, and horizontal governance mirrors the exact engineering blueprints required to steward autonomous physical nodes in the real world.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 THE FOUR HERITAGE PILLARS OF OASIS                               │
├────────────────────────────────┬────────────────────────────────┬────────────────────────────────┤
│ D1: Dwarf Fortress Psychology  │ D2: The Sims Indirect Founder  │ D3: Metabolic Reclamation      │
│ - 64-byte bounded ECS state    │ - Mortal Founder 01 embodiment │ - 12kV utility bridging        │
│ - 32-slot episodic memory ring │ - Spatial Designator menus     │ - Ductile iron water tapping   │
│ - Big-Five facets & 8 Values   │ - Chill Loop & 100% Burnout    │ - Urbanite & rebar quarrying   │
│ - Lyapunov stress homeostasis  │ - Logistic moral rejection     │ - Suburban, Urban & Eco biomes │
│ - Breakdowns & Strange Moods   │ - Legacy NPC Wave assimilation │ - Dependent -> Sovereign arc   │
├────────────────────────────────┴────────────────────────────────┴────────────────────────────────┤
│ D4: Sovereign Stack Multiplayer, Temporal Flow Contracts & Stochastic Boundary Interface         │
│ - Local-first deterministic simulation scaling to voluntary MMO (`Arcadia server-01`)            │
│ - Mandatory Milestone (End-of-Month Epoch Sync) resolving 1x–100x Time-Warp relativity           │
│ - Stochastic Boundary Interface: Good-faith DES (`col-adversaryd`) inhomogeneous Poisson shocks  │
│ - 3 Attenuation Pillars: Physical autarky, social NPC integration, cryptographic mesh proximity  │
│ - Outward-only `QUALITY_SIMULATED (0x01)` telemetry production over UHAI IPC ring buffers        │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

### 1.2. Stafford Beer's Viable System Model (VSM) Synthesis
As ratified in `docs/COLLECTIVE_GUIDING_PRINCIPLES.md`, Oasis does not exist in isolation. It serves as **System 4 (Intelligence, Simulation, and Adaptation)** within Stafford Beer’s Viable System Model:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                         VIABLE SYSTEM MODEL (VSM) CYBERNETIC INTEGRATION                         │
├─────────┬───────────────────────────────┬────────────────────────────────────────────────────────┤
│ VSM Tier│ Cybernetic Function           │ Concrete Implementation                                │
├─────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ System 5│ Identity, Policy & Ethos      │ Sovereign Stack L5 (`col-kmsd` DIDs, BBS+ ZKP policies)│
│         │ Constitutional Invariants     │ & Layer 6 (`col-commonsd` JSON-LD Constitutions)       │
├─────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ System 4│ Intelligence & Simulation     │ OASIS ENGINE (Layer 1b Flecs ECS / WebGPU DDA)         │
│         │ Environmental Exploration     │ Shadow Simulator, Cooperative Bioregional Onboarding,  │
│         │ Future Shock Rehearsal        │ Outward SITL Stress-Testing, Dynamic Scenario Lab      │
├─────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ System 3│ Control, Operations & Audit   │ Sovereign Stack L4 Orchestration (`col-execd` BPMN VM) │
│         │ Internal Resource Balance     │ & Layer 3 Ledger (`col-storaged` PoTW / CRDT)          │
├─────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ System 2│ Anti-Oscillation & Damping    │ Sovereign Stack L2 Mesh (`col-meshd` Reticulum RNS)    │
│         │ Coordination Channels         │ & UHAI IPC SPSC Shared Memory Ring Buffer              │
├─────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ System 1│ Operational Autonomous Units  │ Sovereign Stack L1 Physical Plant (`col-telemetryd`)   │
│         │ Primary Metabolic Work        │ Power Electronics, Sensors, Relays, Batteries          │
└─────────┴───────────────────────────────┴────────────────────────────────────────────────────────┘
```

Oasis provides the high-variety exploratory sandbox where catastrophic failure costs zero human lives and zero physical kilowatt-hours. In Oasis, communities stress-test microgrid black-starts, childcare co-op schedules, and mutual-credit clearing networks at $1\times$, $10\times$, and $100\times$ real-time clock speeds. When a governance or engineering policy proves stable in Oasis, it compiles upwards into production daemon configurations for real-world deployment.

### 1.3. Strict Simulation Demarcation & Outward-Only Telemetry Law (`BOOK_TEAM_DRIFT_REPORT.md` Alignment)
To prevent narrative and architectural drift between the C++ engine and the broader Sovereign Stack documentation, Oasis enforces three absolute cybernetic demarcation invariants:

1. **Outward-Only Telemetry Production (`QUALITY_SIMULATED = 0x01`):**  
   Oasis (Layer 1b) is strictly an **outward producer** of synthetic Layer 2 telemetry into the Sovereign Stack (Layers 2–7) for Software-In-The-Loop (SITL) stress-testing. Oasis **never ingests live physical telemetry** from real-world hardware sensors to recalibrate the simulation. Every telemetry packet emitted by Oasis across the UHAI ring buffer is stamped with the hardware airgap flag `quality_flag = 0x01` (`QUALITY_SIMULATED`). The Sovereign Stack's Hardware Airgap Filter mathematically drops any `0x01` packet from actuating physical relays, inverters, or valves.
2. **Unmistakable Context Demarcation (Shadow Simulator, Not Transitionary UI):**  
   Oasis is a dedicated **Shadow Simulator**, never a "game that transitions into a real-world Orchestrator." Blurring simulated gameplay with live physical actuation violates fundamental UHAI safety invariants. Simulated contexts (`Layer 1b`) and physical orchestration contexts (`Layer 1a` / `Layer 6`) remain permanently airgapped and unmistakably distinct.
3. **Cooperative & Structural Modeling (Rejection of Malicious/Parasitic Framing):**  
   The Sovereign Stack and Oasis are built upon *willing voluntariness*, structural resilience, and cooperative mutual aid—never malicious subversion or "parasitic" exploitation. The Layer 7 Adversary daemon (`col-adversaryd`) is engineered on **Discrete Event Simulation (DES)** to model legacy municipal, regulatory, and utility systems in structural good faith, allowing communities to rehearse supply-chain shocks, tariff shifts, and migration waves constructively.

---

## 2. Deep Scenario Driven Development (SDD) Engine Architecture

### 2.1. Dynamic Scenario Ingestion Pipeline
Scenario Driven Development (SDD) is the architectural compiler connecting human socio-ecological requirements directly into both the simulation client (Oasis) and edge daemon infrastructure (The Sovereign Stack). 

Rather than hardcoding gameplay mechanics as bespoke C++ functions or brittle script branches, Oasis incorporates a **Dynamic Scenario Ingestion Pipeline**:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                         DYNAMIC SCENARIO INGESTION PIPELINE (SDD)                                │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│   CANONICAL SCENARIO SPECIFICATION (`docs/scenarios/*.md` or packaged `.jsonld`)                 │
│   ├── Section 1: Metadata & Cryptographic Hashes                                                 │
│   ├── Section 2: Problem Statement & Thermodynamic Bounds                                        │
│   ├── Section 3: Machine-Readable Schemas (Bounty JSON-LD & Attestation JSON)                    │
│   ├── Section 4: Oasis Engine Blueprint (Voxel IDs, Flecs ECS Components, Affordances)           │
│   └── Section 5: Acceptance Test Gates (G1–G6 Invariants)                                        │
│                                           │                                                      │
│                                           ▼                                                      │
│                   ┌───────────────────────────────────────────────┐                              │
│                   │      OASIS SCENARIO COMPILER & INGESTION      │                              │
│                   │        (WASM / Native Zero-Copy Parser)       │                              │
│                   └───────────────────────┬───────────────────────┘                              │
│                                           │                                                      │
│         ┌─────────────────────────────────┼─────────────────────────────────┐                    │
│         ▼                                 ▼                                 ▼                    │
│  ┌──────────────┐                 ┌───────────────┐                 ┌──────────────┐             │
│  │  FLECS ECS   │                 │ SPATIAL VOXEL │                 │  GIBSONIAN   │             │
│  │  COMPONENTS  │                 │   CHUNK TAGS  │                 │  AFFORDANCE  │             │
│  │ (Data Pods)  │                 │ (Material IDs)│                 │    TREES     │             │
│  └──────────────┘                 └───────────────┘                 └──────────────┘             │
│         │                                 │                                 │                    │
│         └─────────────────────────────────┼─────────────────────────────────┘                    │
│                                           ▼                                                      │
│                          ┌─────────────────────────────────┐                                     │
│                          │    SIMULATION RUNTIME WORLD     │                                     │
│                          │  - Autonomous NPC Evaluation    │                                     │
│                          │  - Cadastral Slate Blueprints   │                                     │
│                          │  - Dynamic Task Bounties        │                                     │
│                          │  - Headless SITL Regression     │                                     │
│                          └─────────────────────────────────┘                                     │
│                                                                                                  │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

#### Pipeline Ingestion Stages:
1. **Schema Validation & Hash Registration:** At boot or scenario package load, the engine validates the scenario's canonical identifier (e.g., `SCN-ALPHA-FAB`), verifying integrity against the known IPLD Content Identifier (CID).
2. **Flecs ECS Component Synthesis:** The scenario defines packed, zero-heap data structures. The engine registers these as dynamic Flecs component IDs using the Flecs C API/C++ wrapper, binding them to archetypes without engine recompilation.
3. **Spatial Voxel Palette Tagging:** The scenario registers unique voxel `material_id` allocations (e.g., `material_id = 15` for `HARDWARE_FAB_NODE`, `material_id = 45` for `WOOD_CORD`). Physical parameters (density $\rho$, thermal conductivity $k$, electrical conductivity $\sigma$, moisture index $\mu$, combustibility) are loaded into the GPU voxel material uniform buffer.
4. **Gibsonian Affordance Tree Generation:** Environmental interaction nodes are instantiated into the parcel's spatial hash grid. Objects declare:
   * Required tools and skill prerequisites (`min_skill_level`, `required_tool_flag`).
   * Physiological costs (caloric burn rate $\dot{C}$, focus drain rate $\dot{F}$, hydration loss).
   * Somatic and psychological outcomes (injected transient thoughts, valence, comfort fields).
5. **Dynamic Bounty & Task Publishing:** Scenario bounties (derived from JSON-LD `TaskSpecification`) are translated into binary `SpatialIntentPayload` packets posted to the Cadastral Slate and community bulletin board for autonomous claiming.

### 2.2. Emergent Gameplay Dynamics Without Hardcoded Logic
A fatal flaw of traditional game engines is mechanical sclerosis: adding a "fabrication shop" or "childcare pod" requires modifying player controllers, crafting menus, and AI trees.

In Oasis, SDD achieves **zero-hardcoded mechanical emergence** via cybernetic principles:
* **Ashby's Law of Requisite Variety ($\mathcal{V}_{\text{player}} \ge \mathcal{V}_{\text{threat}}$):** The engine provides an open set of thermodynamic primitives (exergy, mass, thermal transfer, emotional valence, spatial adjacency). A scenario merely instantiates a new configuration of these primitives.
* **The Affordance Broker:** Agents do not possess hardcoded scripts like `GoToChildcare()`. Instead, a childcare pod emits affordances: `[Somatic Need: Child Supervision]`, `[Affordance: Comfort Nursery]`, `[Mutual Credit Reward: 1.0 VC/hr]`. An agent with high `Empathy` and available time evaluates the affordance utility, claiming the task autonomously.
* **Non-Linear Feedback Cascades:** If the micro-foundry (Scenario Theta) runs during a solar deficit, the SOR electrical solver detects low bus voltage, tripping battery under-voltage protection. The kitchen induction cooktop (Scenario Gamma) loses power, delaying dinner. Hungry citizens suffer low focus and negative thoughts, leading to an argument in the evening circle. No single script orchestrated this chain; it emerged naturally from thermodynamic and psychological interdependencies.

### 2.3. The Upwards Compilation Loop
When an Oasis player or headless simulation test suite discovers a failure mode—such as a mutual-aid childcare rotation collapsing due to asymmetrical chore distribution (Scenario Delta) or an inverter fire during a black-start (Scenario Upsilon)—the scenario's test harness records the execution trace. 

Because Oasis schemas are byte-compatible with the Sovereign Stack's Layer 4 BPMN VM and Layer 6 JSON-LD intent engines, remediations developed in the game (e.g., adding an automated rotation rebalance token or a 5-minute pre-charge delay) are exported directly into canonical scenario files and deployed to physical edge nodes.

---

## 3. Comprehensive MVS Scenario Mapping Across the 5 Developmental Acts

The Minimum Viable Society (MVS) comprises 24 canonical scenarios organized into Five Developmental Acts. Below is the authoritative specification mapping each scenario into its ludic mechanics, Founder affordances, voxel interactions, psychological impacts, and verification gates.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                             THE 5 DEVELOPMENTAL ACTS OF THE MVS                                  │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ ACT I: METABOLIC FOUNDATION (Scenarios 1–4)                                                      │
│ Tools, transport, caloric nourishment, and generational care. Survival baseline.                 │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ ACT II: SPATIAL & CULTURAL CONTINUITY (Scenarios 5–11)                                           │
│ Workplaces, lodging, forestry, metallurgy, decentralized science, education, and the arts.       │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ ACT III: MATERIAL CLOSURE & CIRCULARITY (Scenarios 12–16)                                        │
│ Pyrolysis/biochar, circular textiles, architecture, kinship security, and sovereign guilds.     │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ ACT IV: HUMAN FABRIC & CARE (Scenarios 17–21)                                                    │
│ Maintenance commons, steward genesis, eldercare, paramedic response, and restorative justice.   │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ ACT V: SHOCKS, RESILIENCE & DECOUPLING (Scenarios 22–24)                                         │
│ Adversarial defense, ecological black-start shocks, and terminal decoupling singularity.         │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

### 3.1. Act I: Metabolic Foundation

#### Scenario 1: `alpha_fabrication.md` (`SCN-ALPHA-FAB`) — Distributed Physical Fabrication
* **Ludic Mechanics:** Scavenging raw PETG and aluminum scrap; managing shared 3D printer queues and CNC router toolwear; calibrating print beds to prevent spaghetti failures.
* **Founder Affordances:** `[Calibrate Nozzle]`, `[Slice Blueprint]`, `[Pick Up Tool from Locker]`, `[Service Linear Rails]`.
* **Voxel Interactions:** Voxel `material_id = 15` (`FAB_NODE`) and `16` (`FILAMENT_SPOOL`). Extruder heats adjacent air voxels ($+15^\circ\text{C}$ in $1\text{ dm}$ radius).
* **DF Psychology:** Crafting a masterwork tool grants $+30$ Mood (`THOUGHT_CRAFTED_MASTERWORK`); printer nozzle clogging triggers $+15$ Stress and frustration thoughts.
* **Test Gates:** **G1 (Thermo):** Extrusion energy balances within $\pm 10\%$ of thermodynamic melt enthalpy. **G5 (Onboarding):** Stripe-funded filament purchase compiles to local mesh bounty.

#### Scenario 2: `beta_transit.md` (`SCN-BETA-TRANS`) — Decentralized Transit & Logistics
* **Ludic Mechanics:** Maintaining an electric cargo van; balancing battery state-of-charge against cargo weight; coordinating multi-stop carpool routes across suburban sprawl.
* **Founder Affordances:** `[Inspect Tire Pressure]`, `[Plug In Level-2 EVSE]`, `[Load Cargo Pallet]`, `[Keyless Bluetooth Vehicle Checkout]`.
* **Voxel Interactions:** Vehicle occupies a dynamic $32\times 16\times 16\text{ dm}$ bounding box; tires compress gravel/dirt voxels into packed roadbed over repeated transits.
* **DF Psychology:** Solitary commuting in gridlock causes $+25$ Stress; conversational banter during shared carpool grants $+20$ Mood (`THOUGHT_KINSHIP_TRANSIT`).
* **Test Gates:** **G1 (Utilization):** Empty-seat passenger ratio exceeds $75\%$. **G2 (Offline):** Keyless BLE vehicle handoff completes $100\%$ offline without central servers.

#### Scenario 3: `gamma_kitchen.md` (`SCN-GAMMA-KITCH`) — Commons Kitchen & Caloric Routing
* **Ludic Mechanics:** Managing wholesale dry goods storage; batch-cooking root-vegetable stew on induction ranges; preserving seasonal yields via fermentation and canning.
* **Founder Affordances:** `[Chop Root Vegetables]`, `[Stir Stew Pot]`, `[Sterilize Mason Jars]`, `[Ring Dinner Bell]`.
* **Voxel Interactions:** `material_id = 22` (`COLD_STORAGE_CHEST`) and `23` (`INDUCTION_RANGE`). Stew simmering emits volumetric steam particles and a pleasant aroma field ($r = 8\text{ m}$).
* **DF Psychology:** Eating a hot, nutritious meal in a clean hall grants $+40$ Mood (`THOUGHT_FEAST_WITH_COMMONS`); spoiled food creates severe disgust and lingering food-poisoning trauma.
* **Test Gates:** **G1 (Caloric):** Zero food waste ($> 98\%$ utilization of perishables). **G4 (HACCP):** Cold-storage temperature rise above $4^\circ\text{C}$ triggers emergency preservation alert.

#### Scenario 4: `delta_childcare.md` (`SCN-DELTA-CHILD`) — Mutual Aid Childcare Pod
* **Ludic Mechanics:** Establishing child-safe play zones; scheduling rotating parent caregiver shifts; valuing invisible reproductive labor equally with technical construction.
* **Founder Affordances:** `[Read Story to Pod]`, `[Prepare Afternoon Snack]`, `[Check Boundary Fence]`, `[Hand Off Caregiver Token]`.
* **Voxel Interactions:** Children entities possess small collision bounds ($4\times 4\times 10\text{ dm}$); hazard voxels (exposed wiring, open water tanks) trigger visual alert badges.
* **DF Psychology:** Parents relieved of constant childcare gain $+50$ Mood (`THOUGHT_UNBURDENED_MIND`) and focus recovery; caregiver burnout occurs if shifts exceed 4 hours.
* **Test Gates:** **G2 (Privacy):** Zero facial recognition or camera telemetry; strictly cryptographically blinded presence tokens. **G5 (Parity):** Childcare hours credit 1:1 against skilled carpentry in the mutual credit ledger.

---

### 3.2. Act II: Spatial & Cultural Continuity

#### Scenario 5: `epsilon_coworking.md` (`SCN-EPSILON-COWORK`) — Sovereign Third Place & Facade
* **Ludic Mechanics:** Managing shared hot-desks and high-speed Wi-Fi; operating a compliant public coffee shop facade to generate legacy cash while hosting sovereign assemblies.
* **Founder Affordances:** `[Brew Espresso]`, `[Reserve Standing Desk]`, `[Wipe Down Whiteboard]`, `[Switch Router to Mesh WAN]`.
* **Voxel Interactions:** Workstations provide ergonomic seating affordances; acoustic absorption panels (`material_id = 31`) dampen acoustic ray reflections.
* **DF Psychology:** Social interaction with intellectual peers cures cabin fever ($+25$ Mood); noisy disruptions cause focus loss and irritability.
* **Test Gates:** **G3 (Partition):** External fiber cutoff smoothly fails over to local community intranet mesh. **G5 (Facade):** Commercial fiat revenue shields interior commons from tax audits.

#### Scenario 6: `zeta_lodging.md` (`SCN-ZETA-LODGE`) — Micro-Lodging Mesh
* **Ludic Mechanics:** Retrofitting spare rooms into guest micro-cabins; hosting traveling sovereign builders; managing thermal comfort and laundry turnaround.
* **Founder Affordances:** `[Change Bed Linens]`, `[Restock Firewood Box]`, `[Issue NFC Room Key]`, `[Welcome Guest at Gate]`.
* **Voxel Interactions:** Insulated wall voxels (`R-value \ge 24`) prevent winter heat loss to exterior chunks; room door latches respond to NFC proximity.
* **DF Psychology:** Welcoming trustworthy travelers creates positive hospitality thoughts (`THOUGHT_HOSPITALITY_BOND`); untrustworthy guests trigger vigilance and mild anxiety.
* **Test Gates:** **G2 (Offline Key):** Keyless cryptographic NFC lock opens with local Ed25519 signature verification without WAN internet. **G6 (Thermal):** Guest cabins remain $> 18^\circ\text{C}$ during $-10^\circ\text{C}$ exterior freeze.

#### Scenario 7: `eta_coppice.md` (`SCN-ETA-COPPICE`) — Coppice Agroforestry & Biomass
* **Ludic Mechanics:** Rotational felling of hazel and willow stools on 7-year cycles; stacking and drying cordwood; monitoring bio-acoustic bird biodiversity.
* **Founder Affordances:** `[Prune Coppice Stool]`, `[Split Cordwood]`, `[Stack Firewood Rick]`, `[Check Moisture Meter]`.
* **Voxel Interactions:** Voxel `material_id = 45` (`WOOD_CORD`) undergoes progressive drying ticks ($\mu$ drops from $50\%$ to $< 18\%$).
* **DF Psychology:** Rhythmic hand-splitting wood provides cathartic physical satisfaction ($+20$ Mood, stress reduction); excessive clearcutting triggers environmental grief.
* **Test Gates:** **G1 (Regen):** Annual biomass harvest is strictly less than annual biological regrowth rate. **G4 (Combustion):** Cordwood cannot be loaded into stoves until moisture is under $18\%$.

#### Scenario 8: `theta_foundry.md` (`SCN-THETA-FOUNDRY`) — Neighborhood Micro-Foundry
* **Ludic Mechanics:** Melting salvaged aluminum cans and copper scrap in an electric induction crucible; pouring sand castings for replacement machine gears; scheduling pours during solar peaks.
* **Founder Affordances:** `[Charge Crucible with Scrap]`, `[Don Heat Shield & Leather Apron]`, `[Skim Slag]`, `[Pour Molten Metal into Sand Flask]`.
* **Voxel Interactions:** Molten metal voxels glow with high emissive radiance ($1200^\circ\text{C}$); radiant heat dissipates into adjacent voxels via 3D thermal conduction.
* **DF Psychology:** Watching glowing liquid bronze instills creative awe ($+40$ Mood, `THOUGHT_MASTER_METALLURGY`); near-miss burn hazards cause acute fear and panic.
* **Test Gates:** **G1 (Heat Balance):** Melts are interlocked to solar exergy surplus ($P_{\text{solar}} > 12\text{ kW}$). **G3 (Safety):** Emergency thermal dump valve trips in $< 50\text{ ms}$ upon coolant failure.

#### Scenario 9: `iota_epistemic.md` (`SCN-IOTA-EPISTEMIC`) — Epistemic Mesh & Open R&D
* **Ludic Mechanics:** Conducting soil biology microscopy and plant tissue assays; documenting open-hardware schematics; publishing findings to local IPFS repositories.
* **Founder Affordances:** `[Calibrate Microscope]`, `[Inoculate Agar Petri Dish]`, `[Export KiCad Schematic]`, `[Pin Dataset to Node IPFS]`.
* **Voxel Interactions:** Lab benches require stable horizontal surfaces and clean room airflow filters (`material_id = 52`).
* **DF Psychology:** Scientific discovery yields intense eureka ecstasy ($+70$ Mood, `THOUGHT_SCIENTIFIC_BREAKTHROUGH`); repeated assay failures cause frustration and imposter syndrome.
* **Test Gates:** **G2 (Provenance):** Raw experimental logs signed by local DID and pinned to IPLD blockstore. **G4 (Open):** All blueprints released under CERN-OHL-S open hardware licenses.

#### Scenario 10: `kappa_apprenticeship.md` (`SCN-KAPPA-APPRENTICE`) — Apprenticeship & Skill Transfer
* **Ludic Mechanics:** Pairing novice characters with master craftspeople; unlocking advanced construction blueprints through cooperative labor; awarding verifiable skill credentials.
* **Founder Affordances:** `[Demonstrate Dovetail Joint]`, `[Observe Master Electrician]`, `[Grade Apprentice Test Piece]`, `[Issue BBS+ Skill Credential]`.
* **Voxel Interactions:** Dual-operator workstations require two character collision boxes simultaneously present within $1.5\text{ m}$.
* **DF Psychology:** Teaching an apprentice gives deep elder pride ($+35$ Mood); apprentices feel reverence and rapid focus growth when shadowed by a patient mentor.
* **Test Gates:** **G2 (ZKP):** Skill credentials prove competence without exposing past failure logs. **G5 (Multi-sig):** Masterwork blueprints require 3 independent peer attestations.

#### Scenario 11: `lambda_cultural.md` (`SCN-LAMBDA-CULTURE`) — Cultural Commons & Acoustic Budget
* **Ludic Mechanics:** Staging acoustic folk concerts and theatrical performances; setting decibel budgets to prevent municipal noise citations; decorating communal halls.
* **Founder Affordances:** `[Tune Acoustic Guitar]`, `[Hang Stage Drapery]`, `[Adjust Ambient Lighting]`, `[Dance in Communal Circle]`.
* **Voxel Interactions:** Sound emission voxels propagate acoustic waves; foliage and cob walls attenuate sound according to material absorption coefficients.
* **DF Psychology:** Collective music and dancing produce massive collective effervescence ($+60$ Mood, rapid reduction of community stress accumulators).
* **Test Gates:** **G1 (Acoustic):** Sound levels at parcel perimeter stay below $55\text{ dBA}$ curfew limit. **G4 (Morale):** Community stress levels drop by at least $25\%$ following cultural events.

---

### 3.3. Act III: Material Closure & Circularity

#### Scenario 12: `mu_extraction.md` (`SCN-MU-BIOCHAR`) — Regenerative Biochar & Biodigestion
* **Ludic Mechanics:** Pyrolyzing agricultural waste into high-surface-area biochar; capturing methane from anaerobic biodigesters; inoculating biochar with compost tea.
* **Founder Affordances:** `[Pack Retort Kiln]`, `[Ignite Pyrolysis Burner]`, `[Monitor Methane Flare Valve]`, `[Charge Biochar in Compost Slurry]`.
* **Voxel Interactions:** Retort kiln operates at $550^\circ\text{C}$; charged biochar applied to soil voxels permanently increases water retention ($+35\%$) and nitrogen storage.
* **DF Psychology:** Disgust from handling manure transforms into profound agronomic pride upon observing dark, rich terra preta soil.
* **Test Gates:** **G1 (Sequestration):** Verifiable carbon sequestration exceeds $2.5\text{ kg } CO_2e$ per kg biochar. **G4 (Safety):** Digester pressure relief valve auto-triggers flare upon overpressure.

#### Scenario 13: `nu_fashion.md` (`SCN-NU-TEXTILES`) — Circular Textiles & Mending
* **Ludic Mechanics:** Salvaging discarded municipal textiles; spinning wool and plant fibers; repairing work clothes; shredding scraps into cellulose building insulation.
* **Founder Affordances:** `[Treadle Sewing Machine]`, `[Darn Woolen Socks]`, `[Card Raw Fleece]`, `[Shred Denim into Insulation Batts]`.
* **Voxel Interactions:** Damaged character clothing displays visible tear decals; repaired clothing increases thermal insulation properties.
* **DF Psychology:** Wearing sturdy, hand-mended clothing gives warm comfort and grounded dignity ($+25$ Mood); shivering in ragged clothing spikes anxiety.
* **Test Gates:** **G1 (Circularity):** Zero virgin synthetic polymers used; $100\%$ recycled or natural fibers. **G6 (Thermal):** Scrap cellulose insulation achieves verified $R-3.5/\text{inch}$ rating.

#### Scenario 14: `xi_architecture.md` (`SCN-XI-ARCH`) — Algorithmic Architecture & Barn-Raising
* **Ludic Mechanics:** Generating timber-frame structures with parametric load-bearing checks; coordinating multi-person barn-raising events; retrofitting leaky drywall with cob.
* **Founder Affordances:** `[Erect Heavy Timber Bent]`, `[Drive Oak Peg]`, `[Apply Cob Plaster]`, `[Sight Total Station Theodolite]`.
* **Voxel Interactions:** Voxels evaluate vertical and shear load paths; unsupported spans exceed structural limits and collapse dynamically if overloaded.
* **DF Psychology:** Collective construction creates intense social bonding (`THOUGHT_COMMUNAL_BARN_RAISING`, $+50$ Mood); witnessing a structural collapse induces severe shock.
* **Test Gates:** **G1 (Structural):** Load paths satisfy $1.5\times$ engineering safety factor. **G5 (Building Code):** Structure footprint stays within municipal permit-exempt square footage limits.

#### Scenario 15: `omicron_kinship.md` (`SCN-OMICRON-KINSHIP`) — Kinship Protocol & Trust Rings
* **Ludic Mechanics:** Establishing Web-of-Trust rings ($R_0 \to R_6$: `Ring R0` Core, `Ring R1` Vouched Stewards, `Ring R3` Probationary Residents, `Ring R6` Unverified Migrants); vouching for members via 2-of-3 FROST ceremonies; defending against Sybil attacks.
* **Founder Affordances:** `[Conduct Trust Vouching Circle]`, `[Review Vouch Request]`, `[Sign Mutual Credit Covenant]`, `[Initiate Social Slashing Dispute]`.
* **Voxel Interactions:** Physical proximity is validated via in-person NFC tap interactions between character entities.
* **DF Psychology:** Belonging to a tight-knit kinship ring provides an impenetrable psychological buffer against external stress; betrayal by a vouched peer causes acute grief.
* **Test Gates:** **G2 (Sybil):** Nodes $> 3$ hops from root in the trust graph receive zero credit line. **G3 (Slashing):** Malicious defection automatically slashes the voucher's reputation score by $50\%$.

#### Scenario 16: `pi_guild.md` (`SCN-PI-GUILD`) — Sovereign Guild & Legal Shield
* **Ludic Mechanics:** Forming a shared services guild; pooling high-value fabrication tools; invoicing external legacy clients through a legal Employer-of-Record (EOR) cooperative shell.
* **Founder Affordances:** `[Approve Collective Invoice]`, `[Disburse Guild Dividend]`, `[Vote on Tool Purchase]`, `[Sign EOR Legal Shield Pact]`.
* **Voxel Interactions:** Guild hall vault stores high-precision tooling behind multi-signature electronic locks.
* **DF Psychology:** Professional solidarity and shared economic safety eliminate wage-slavery terror ($+40$ Mood, permanent reduction of baseline anxiety).
* **Test Gates:** **G5 (Shielding):** Commercial revenue routed through cooperative EOR incurs zero unshielded tax liability. **G6 (Reserves):** At least $30\%$ of guild surplus is retained in physical tools.

---

### 3.4. Act IV: Human Fabric & Care

#### Scenario 17: `sigma_burnout.md` (`SCN-SIGMA-MAINTENANCE`) — Maintenance Commons & Anti-Burnout
* **Ludic Mechanics:** Managing invisible labor (compost turning, filter cleaning, tool sharpening, floor sweeping); dynamic scaling of chore bounties as tasks sit uncompleted; preventing founder burnout.
* **Founder Affordances:** `[Clean Greywater Filter]`, `[Oil Chainsaw Bar]`, `[Sweep Mess Hall]`, `[Enforce Mandatory Rest Day]`.
* **Voxel Interactions:** Equipment voxels accumulate dust and rust layers over simulation ticks; uncleaned voxels suffer operational efficiency penalties.
* **DF Psychology:** Resentment festers when one agent performs all unglamorous chores (`THOUGHT_MARTYRDOM_RESENTMENT`); clean, well-maintained surroundings confer calm focus ($+25$).
* **Test Gates:** **G1 (Entropy):** Chore bounty rewards scale exponentially with unfulfilled tick count. **G4 (Equity):** No single character performs more than $25\%$ of total maintenance labor.

#### Scenario 18: `tau_genesis.md` (`SCN-TAU-GENESIS`) — Steward Genesis & Onboarding
* **Ludic Mechanics:** Character creation and ideological grounding; generating local cryptographic DID keypairs on air-gapped hardware; inventorying personal tools brought to the commons.
* **Founder Affordances:** `[Generate Ed25519 Identity]`, `[Declare Tool Inventory]`, `[Sign Genesis Covenant]`, `[Select Founder Whims]`.
* **Voxel Interactions:** Founder 01 places their first shelter (canvas bell tent, woodstove) on the unregenerated parcel.
* **DF Psychology:** Excitement and solemn purpose of a fresh beginning ($+30$ Mood); trepidation regarding legacy severing.
* **Test Gates:** **G2 (Airgap):** Private keys generated locally in secure storage; zero network leakage. **G5 (Cold Start):** Initial mutual credit line collateralized against declared physical tools.

#### Scenario 19: `phi_elder.md` (`SCN-PHI-ELDERCARE`) — The Elder Commons & Multi-Gen Living
* **Ludic Mechanics:** Designing accessible, single-story living spaces; supporting aging community members; harvesting elder agronomic and craft wisdom before memory loss; tool bequest ceremonies.
* **Founder Affordances:** `[Sit with Elder by Hearth]`, `[Record Oral History]`, `[Deliver Warm Meal Tray]`, `[Accept Master Tool Bequest]`.
* **Voxel Interactions:** Ramped entrances and grab-bars (`material_id = 61`) allow smooth navigation for characters with reduced mobility.
* **DF Psychology:** Elders feel profound peace and continued purpose ($+50$ Mood); community experiences deep grounding from living history and grief upon an elder's passing.
* **Test Gates:** **G2 (Dignity):** Fall-detection radar preserves elder privacy without video surveillance. **G6 (Inheritance):** Deceased elder’s tools transfer into the public tool library without probate probate seizures.

#### Scenario 20: `chi_paramedic.md` (`SCN-CHI-PARAMEDIC`) — Paramedic Mesh & Health Sovereignty
* **Ludic Mechanics:** Rapid response to acute trauma (falls, burns, deep lacerations); maintaining an off-grid trauma kit and cold-chain medications; dispatching community first responders.
* **Founder Affordances:** `[Apply Tourniquet]`, `[Suture Laceration]`, `[Administer Herbal Salve]`, `[Broadcast Medical Mesh Beacon]`.
* **Voxel Interactions:** Clinic bay provides sterile, climate-controlled voxel volumes; trauma kits must be replenished with sterilized bandages.
* **DF Psychology:** Terror and adrenaline during acute injury; profound relief upon medic arrival ($+60$ Mood); long-term survival trauma requires convalescent care.
* **Test Gates:** **G2 (Response Time):** Local paramedic response time $< 3\text{ minutes}$, beating legacy 911 EMS. **G4 (Cold Chain):** Medication refrigerator temperature excursions trigger audible alarms within 60 seconds.

#### Scenario 21: `psi_mediation.md` (`SCN-PSI-MEDIATION`) — Restorative Justice & Conflict Resolution
* **Ludic Mechanics:** De-escalating interpersonal grievances and resource disputes; convening restorative justice circles; agreeing upon restitution work pacts without state police.
* **Founder Affordances:** `[Call Restorative Circle]`, `[Speak from Heart with Talking Stick]`, `[Propose Restitution Labor]`, `[Embrace in Forgiveness]`.
* **Voxel Interactions:** Neutral meeting circle space surrounded by circular cob seating.
* **DF Psychology:** Simmering resentment decays into catharsis and tears of reconciliation; unresolved grudges lead to factional schisms and tantrums.
* **Test Gates:** **G3 (Confidentiality):** Mediation transcripts encrypted locally; only the signed restitution pact is stored. **G5 (Restitution):** Harm is repaired through physical labor for the victim, not cash fines.

---

### 3.5. Act V: Shocks, Resilience & Decoupling

#### Scenario 22: `rho_adversarial.md` (`SCN-RHO-ADVERSARY`) — Adversarial Defense & Threat Modeling
* **Ludic Mechanics:** Defending against hostile municipal code enforcement raids, predatory zoning citations, utility shutoffs, and drone surveillance; deploying optical and acoustic shielding.
* **Founder Affordances:** `[Deploy Camouflage Netting]`, `[Verify Legal 501(d) Shield]`, `[Zeroize Compromised Keys]`, `[Record Code Officer with Bodycam]`.
* **Voxel Interactions:** Boundary visual blockers reduce parcel visibility from the public roadway, suppressing legacy inspection triggers.
* **DF Psychology:** Paranoia and hyper-vigilance under active scrutiny; solidarity and courage in standing firm against corrupt municipal officials.
* **Test Gates:** **G3 (Tamper):** Physical breach of server enclosure zeroes volatile memory in $< 10\text{ ms}$. **G4 (Immunity):** Sybil packet floods on mesh radio are dropped at the physical layer without CPU lockup.

#### Scenario 23: `upsilon_shock.md` (`SCN-UPSILON-SHOCK`) — Ecological Shock & Black-Start Resilience
* **Ludic Mechanics:** Surviving a catastrophic ice storm and prolonged regional power grid blackout; executing a cold black-start of the local microgrid; sharing emergency heat and soup with frozen legacy neighbors.
* **Founder Affordances:** `[Switch Microgrid to Island Mode]`, `[Clear Heavy Snow from PV Array]`, `[Fire Up Main Rocket Heater]`, `[Open Warming Shelter Doors]`.
* **Voxel Interactions:** Heavy snow voxels accumulate on roofs and solar panels; freezing temperatures penetrate uninsulated walls, testing thermal mass.
* **DF Psychology:** Survival dread transformed into triumphant solidarity as lights stay on while the surrounding city goes pitch black.
* **Test Gates:** **G1 (Autarky):** Microgrid maintains critical heating and life-support loads for 72 hours of total grid blackout. **G2 (Black-Start):** Inverters bootstrap successfully from battery storage without external AC reference.

#### Scenario 24: `omega_decoupling.md` (`SCN-OMEGA-DECOUPLING`) — Terminal Decoupling Singularity
* **Ludic Mechanics:** Reaching the sovereign autarky threshold ($\mathcal{S}_{\text{autarky}} \ge 1.25$); cutting the physical high-voltage grid drop and water meter; dissolving the legacy corporate shell into a Perpetual Purpose Trust; dissolving the Fog of Legacy into a verdant commons corridor.
* **Founder Affordances:** `[Physically Sever Legacy Utility Drop]`, `[Smash Municipal Water Meter]`, `[Dissolve Corporate Shell Paperwork]`, `[Celebrate Emancipation Day]`.
* **Voxel Interactions:** The high-voltage connection wire is permanently deleted; the property boundary fence opens into an uninterrupted bioregional commons corridor.
* **DF Psychology:** Complete, transcendent emancipation from wage servitude and debt anxiety (`THOUGHT_SOVEREIGN_SINGULARITY`, $+100$ permanent Mood buff, zero baseline stress).
* **Test Gates:** **G1 (Zero Net External Exergy):** Node produces $125\%$ of its annual energy and caloric needs. **G6 (Terminal Independence):** All legacy fiat HUD counters are permanently deactivated; the simulation transitions into a pure thermodynamic negentropy economy.

---

## 4. The Four Heritage Pillars of Oasis

### 4.1. Directive 1: Dwarf Fortress Psychology & Cognitive Core

#### 4.1.1. The Failure of Sims-Style Need Bars
Conventional simulation games reduce human beings to shallow mechanical meters:
1. **Zero Temporal Depth (Instant Amnesia):** An agent whose fun bar reaches $100\%$ is immediately cheerful, even if their closest companion froze to death thirty seconds prior.
2. **Zero Value Friction:** An agent will perform any task if basic motive bars are satisfied, completely lacking philosophical conviction or moral boundaries.
3. **Linear State Machines:** Agents act as mechanical dolls responding to immediate biological triggers rather than complex humans shaped by lived trauma and triumph.

Oasis adopts the psychological architecture of *Dwarf Fortress*, grounding characters in **Personality Facets**, **8 Core Values**, **Transient Thoughts**, **Dual-Timescale Lyapunov Stress Homeostasis**, and a **32-Slot Bounded Episodic Memory Ring**.

#### 4.1.2. The Deep Psychology Data Model
Every agent’s psychological state is packed into a cache-aligned, 64-byte ECS component with zero runtime heap allocation:

```cpp
namespace oasis::psychology {

// Bit-exact fixed-point types for cross-platform simulation determinism
using Fixed8  = int8_t;   // Quantized scalar [-100 to +100]
using Fixed16 = int16_t;  // Q8.8 fixed-point [-128.00 to +127.99]
using Fixed32 = int32_t;  // Q24.8 fixed-point [-8,388,608.00 to +8,388,607.99]

// 1. Personality Facets (Big-Five OCEAN + DF Facet Extensions, 16 bytes)
struct alignas(4) PersonalityFacets {
    Fixed8 assertiveness;       // Propensity to command and direct others
    Fixed8 anxiety;             // Sensitivity to external stressors and hazards
    Fixed8 empathy;             // Altruistic drive and compassion for peer suffering
    Fixed8 self_discipline;     // Capacity to sustain labor through fatigue
    Fixed8 curiosity;           // Drive to explore and invent blueprints
    Fixed8 pragmatism;          // Willingness to compromise ideals for survival
    Fixed8 stoicism;            // Buffering against acute emotional shocks
    Fixed8 sociability;         // Need for communal contact vs solitary retreat
    Fixed8 discordance;         // Propensity for friction, skepticism, and debate
    Fixed8 artistic_drive;      // Need for beauty, craft, and aesthetic expression
    Fixed8 padding[6];          // Cache-line alignment padding
};
static_assert(sizeof(PersonalityFacets) == 16, "PersonalityFacets must be exactly 16 bytes.");

// 2. Core Values (8 Inviolable Philosophical Convictions, 16 bytes)
struct alignas(4) CoreValues {
    Fixed8 autonomy;            // Rejection of servitude vs comfort in external rules
    Fixed8 stewardship;         // Regenerative ecology vs extractive exploitation
    Fixed8 solidarity;          // Mutual aid obligation vs individual hoarding
    Fixed8 ingenuity;           // Open-source fabrication vs passive consumption
    Fixed8 tenacity;            // Perseverance through winter hardship
    Fixed8 pragmatism;          // Compromising perfection for immediate survival
    Fixed8 transparency;        // Reverence for truth vs secrecy
    Fixed8 beauty;              // Need for craft and dignity vs barren utility
    Fixed8 padding[8];          // 16-byte SIMD alignment padding
};
static_assert(sizeof(CoreValues) == 16, "CoreValues must be exactly 16 bytes.");

// 3. Transient Thought (Active emotional stimulus, 12 bytes)
struct alignas(4) Thought {
    uint16_t thought_id;        // Categorical ID (e.g., THOUGHT_COLD_RAIN, THOUGHT_COMMUNAL_FEAST)
    Fixed16  valence;           // Emotional impact (Q8.8 fixed-point)
    uint32_t timestamp_ticks;   // Tick when thought occurred
    uint16_t duration_ticks;    // Lifespan of thought in active buffer
    uint16_t source_entity_id;  // Instigating entity or 0
};
static_assert(sizeof(Thought) == 12, "Thought must be exactly 12 bytes.");

// 4. Episodic Memory Node (Historical event in 32-slot ring, 24 bytes packed)
struct alignas(8) EpisodicMemoryNode {
    uint32_t timestamp_ticks;   // Simulation tick of event occurrence
    uint16_t event_type;        // Categorical event (MEM_CODE_RAID, MEM_FIRST_KILOWATT, etc.)
    uint16_t subject_entity_id; // Related actor (peer, inspector, or 0)
    Fixed16  emotional_valence; // Consolidated emotional weight (Q8.8)
    uint16_t salience;          // Salience score [0..65535] determining retention
    uint16_t voxel_x;           // Spatial coordinate of event occurrence
    uint16_t voxel_y;
    uint16_t voxel_z;
    uint16_t flags;             // Bit 0: PERMANENT_MEMORY (salience >= 80), Bits 1-15: Trigger count
};
static_assert(sizeof(EpisodicMemoryNode) == 24, "EpisodicMemoryNode must be exactly 24 bytes.");

// 5. Total 64-Byte Bounded Psychological State Component
struct alignas(64) AgentPsychologyComponent {
    PersonalityFacets facets;   // 16 bytes
    CoreValues        values;   // 16 bytes
    Fixed32 active_mood;        // 4 bytes: Fast emotional balance [-10,000..+10,000]
    Fixed32 cumulative_stress;  // 4 bytes: Lyapunov stress accumulator [-100,000..+100,000]
    uint8_t short_term_focus;   // 1 byte: Operational clarity [0..255]
    uint8_t breakdown_state;    // 1 byte: 0=Normal, 1=Irritable, 2=Tantrum, 3=Melancholy, 4=Catatonia
    uint8_t active_thought_count;// 1 byte: Number of active thoughts in buffer
    uint8_t ring_head_index;    // 1 byte: Head pointer for 32-slot memory ring
    uint8_t flags;              // 1 byte: Bit 0=StrangeMood, Bit 1=ClinicalRecovery
    uint8_t padding[19];        // Padded to exactly 64 bytes (1 cache line)
};
static_assert(sizeof(AgentPsychologyComponent) == 64, "AgentPsychologyComponent must be exactly 64 bytes.");

} // namespace oasis::psychology
```

#### 4.1.3. The Cognitive Triad & Dual-Timescale Lyapunov Stress Homeostasis
Every agent operates under a three-tier cognitive state machine:
* **Short-Term Focus ($0 \dots 255$):** Immediate operational clarity. High focus ($\ge 200$) yields $+25\%$ crafting speed and masterwork artifact chance. Low focus ($\le 50$) causes tool drops, miswiring, and construction accidents.
* **Active Mood ($\mathcal{M}(t) \in [-10,000 \dots +10,000]$):** Fast emotional balance computed at $1\text{ Hz}$ from active thoughts $\mathcal{T}(t)$:
  $$\mathcal{M}(t) = \sum_{i \in \mathcal{T}(t)} \text{valence}_i \cdot \left( 1.0 + \frac{\text{FacetImpact}_i + \text{ValueImpact}_i}{100.0} \right)$$
* **Cumulative Stress ($\mathcal{S}(t) \in [-100,000 \dots +100,000]$):** Slow-moving hysteretic accumulator governing breakdown states.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                           DUAL-TIMESCALE LYAPUNOV STRESS HOMEOSTASIS                             │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│   Fast Acute Shocks (τ_acute = 30s)             Slow Chronic Integration (τ_baseline = 7 days)   │
│   [Cold Rain, Dropped Wrench, Soup Burn]        [Winter Freeze, Raid Trauma, Isolation]         │
│                 │                                             │                                  │
│                 ▼                                             ▼                                  │
│   ┌───────────────────────────┐                 ┌───────────────────────────┐                    │
│   │   S_acute Drift Equation  │                 │ S_baseline Integrator     │                    │
│   └─────────────┬─────────────┘                 └─────────────┬─────────────┘                    │
│                 │                                             │                                  │
│                 └───────────────────────┬─────────────────────┘                                  │
│                                         ▼                                                        │
│                    ┌──────────────────────────────────────────┐                                  │
│                    │     NON-LINEAR RESTORING ATTRACTOR       │                                  │
│                    │   - Resilience Dampers (Focus + Kinship) │                                  │
│                    │   - Non-linear Tanh Restoring Function   │                                  │
│                    └────────────────────┬─────────────────────┘                                  │
│                                         │                                                        │
│                                         ▼                                                        │
│                  [Stress > 80,000: CLINICAL RECOVERY ATTRACTOR]                                  │
│                  - Autonomous peer nursing (warm soup, blankets)                                 │
│                  - Guaranteed restorative drift (-150 units/hr)                                  │
│                  - Prevents Catatonia Death Spirals                                              │
│                                                                                                  │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

The differential stress equation is formulated as:
$$\frac{d\mathcal{S}}{dt} = \dot{S}_{\text{acute}} - \kappa_{\text{resilience}} \cdot (\text{Focus} + \text{MutualAid}) - \frac{1}{\tau_{\text{baseline}}} \tanh\left(\frac{\mathcal{S} - \mathcal{S}_{\text{baseline}}}{S_0}\right)$$

Where:
* Fast acute damping ($\tau_{\text{acute}} = 30\text{ s}$):
  $$\dot{S}_{\text{acute}} = \begin{cases} -\frac{1}{\tau_{\text{acute}}} (\mathcal{M}(t) - \mathcal{M}_{\text{threshold}}), & \text{if } \mathcal{M}(t) > \mathcal{M}_{\text{threshold}} \\ +\frac{1}{\tau_{\text{acute}}} (\mathcal{M}_{\text{threshold}} - \mathcal{M}(t)) \left(1.0 + \frac{\text{Anxiety}}{100.0}\right), & \text{if } \mathcal{M}(t) < \mathcal{M}_{\text{threshold}} \end{cases}$$
* **Clinical Recovery Attractor:** When stress crosses into severe breakdown ($\mathcal{S} > 80,000$), unmitigated feedback spirals are arrested. Peers autonomously initiate nursing behaviors, enforcing a guaranteed restorative drift ($\Delta \mathcal{S}_{\text{clinical}} = -150\text{ units/hour}$) that breaks permanent catatonia loops.

#### 4.1.4. Breakdown State Machine & Strange Moods
When cumulative stress crosses critical thresholds, the cognitive state transitions:
* **Stress $< 25,000$:** Normal Operation.
* **Stress $25,000 \dots 50,000$ (Mild Breakdown — Irritable):** Snaps at peers in mess hall; refuses non-essential chore assignments.
* **Stress $50,000 \dots 80,000$ (Moderate Breakdown — Split State):**
  * *Tantrum:* Smashes tools, kicks inverters, damages adjacent voxels.
  * *Melancholy:* Sits in dirt, stares blankly, ignores user intention queue.
* **Stress $> 80,000$ (Severe Breakdown — Catatonia):** Complete biological shutdown; requires peer spoon-feeding and blanket care.

##### The Creative Burst ("Strange Mood"):
When an agent with $\text{ArtisticDrive} \ge 70$, $\text{Autonomy} \ge 60$, and $\text{Focus} \ge 180$ experiences an acute creative spike, they enter a **Strange Mood**:
1. **Trance State:** The agent claims a workshop, locking out peers.
2. **Material Demands:** Demands specific salvaged materials (e.g., 4-gauge copper wire, white oak, biochar, beeswax).
3. **Masterwork Artifact:** If supplied within 72 in-game hours, they fabricate a Masterwork Artifact (e.g., ultra-efficient MPPT charge controller, hand-wound LoRa transceiver) conferring permanent $+20\%$ parcel efficiency, $+15$ permanent Stoicism, and setting stress to zero.
4. **Tragic Collapse:** If materials are denied, the agent collapses into suicidal melancholia or berserk fury.

#### 4.1.5. 32-Slot Episodic Memory Ring Buffer & Sensory Flashbacks
Every agent owns a 32-slot circular ring buffer storing `EpisodicMemoryNode` entries:
* **Ebbinghaus Decay Curve:** Low-salience memories decay on diurnal sleep cycles:
  $$S(t) = S_0 \cdot \exp\left(-\frac{\Delta t}{\tau_{\text{decay}}}\right) \cdot \left(1 + \frac{|\text{valence}|}{V_{\text{max}}}\right)$$
* **Salience-Priority Ring Replacement:** High-salience events ($S(t) \ge 80$) receive the `PERMANENT_MEMORY` flag. When the buffer is full, the engine evicts the oldest entry with lowest salience that lacks the `PERMANENT_MEMORY` flag. Root cause traumas are permanently immune to FIFO eviction.
* **Spatial Sensory Flashbacks:** Walking within $4\text{ decimeters}$ of a voxel where an intense trauma occurred (e.g., coordinate $(95, 32, 48)$ during a police raid) triggers a spatial hash query:
  > *"Founder 01 remembers the violent raid of Autumn Year 1. A cold dread settles over them."*
  A negative thought is injected, and focus plummets by 30 points.

---

### 4.2. Directive 2: The Sims Indirect Founder Management

#### 4.2.1. Rejection of the RTS God-Mode Cursor
In conventional base builders, the player acts as a digital dictator commanding pawns to perform lethal tasks with zero friction. Oasis rejects this authoritarian paradigm. You are not a god hovering over Lot 402; you are the **Guiding Steward** of **Founder 01**, an embodied, mortal human being.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                   INDIRECT STEWARDSHIP LOOP                                      │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│   PLAYER (Cadastral Slate / HUD)                               FOUNDER 01 (Autonomous Agent)     │
│   ┌───────────────────────────┐                               ┌──────────────────────────────┐   │
│   │ Designates Blueprints     │                               │ Evaluates Caloric Stamina    │   │
│   │ Enqueues Advisory Chores  │──[Advisory Intention Queue]──►│ Checks Moral Value Alignment │   │
│   │ Honors Personal Whims     │                               │ Decides: Execute or Refuse   │   │
│   └───────────────────────────┘                               └──────────────┬───────────────┘   │
│                 ▲                                                            │                   │
│                 │                                                            │ Environmental     │
│                 │               Gibsonian Affordances                        ▼ Feedback          │
│                 └───────────[Heat, Cooked Stew, Clean Water, Tools]──────────┘                   │
│                                                                                                  │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

#### 4.2.2. Founder 01 Embodiment & Advisory Intention Queue
* **Caloric & Fatigue Limits:** Founder 01 burns $2,000\text{ kcal/day}$ at rest, up to $4,500\text{ kcal/day}$ breaking asphalt or felling timber. Physical exhaustion depletes focus and accelerates stress accumulation.
* **Advisory Intention Queue:** Chores queued on the Cadastral Slate are advisory suggestions. Founder 01 evaluates tasks through an internal utility function.
* **Whims & Creative Aspirations:** Founder 01 spontaneously generates personal aspirations (e.g., *"Wants to build a solar food dehydrator"*, *"Craves solitary reading by the woodstove"*). Fulfilling whims restores $+20$ Focus and purges $-15$ Stress.
* **Gibsonian Environmental Affordances:** The player influences Founder 01 primarily by altering the physical environment. A warm rocket heater naturally attracts shivering characters; simmering stew gathers the settlement into the communal hall.

#### 4.2.3. Autonomous Moral Rejection Heuristics
Founder 01 will refuse directives that violate their core ethical convictions:

```
                      [Player Directive Issued on Slate]
                                       │
                                       ▼
                         ┌───────────────────────────┐
                         │ G1: Stress Breakdown Check│───[Stress > 50,000]───► [REJECT: Tantrum/Melancholy]
                         └─────────────┬─────────────┘
                                       │ Passed
                                       ▼
                         ┌───────────────────────────┐
                         │  G2: Physiological Gating │───[Hydration < 10% or]──► [DEFER: Drops tool, rushes]
                         │      (Thirst, Exhaustion) │   [Fatigue > 90%     ]    [to well or bed first     ]
                         └─────────────┬─────────────┘
                                       │ Passed
                                       ▼
                         ┌───────────────────────────┐
                         │  G3: Core Value Alignment │───[Moral Conflict    ]──► [FLAT REFUSAL: Moral      ]
                         │      Gating Heuristic     │   [ΔValue > Threshold]    [Protest Telegraph        ]
                         └─────────────┬─────────────┘
                                       │ Passed
                                       ▼
                         [DIRECTIVE ACCEPTED & QUEUED]
```

##### Logistic Moral Rejection Curve:
The probability of rejecting an unethical directive follows a logistic sigmoid curve:
$$P(\text{Rejection}) = \frac{1}{1 + \exp\left(-k \cdot (\text{MoralConflict} + \text{StressDelta} - \text{Trust} - \theta)\right)}$$

Where:
* $\text{MoralConflict} = \sum (\text{DirectiveImpact}_i \cdot \text{CoreValues}_i)$. Ordering Founder 01 to dump lead-acid battery acid into the aquifer produces catastrophic conflict against $\text{Stewardship} = +85$.
* **Visual Telegraph:** Founder 01 drops the tool, turns toward the camera, and the Plumbob 2.0 flashes jagged crimson:
  > *"Founder 01 refuses: 'I will not poison the soil we eat from.' Focus dropped by 20."*

#### 4.2.4. Anti-Lockup & Anti-Thrashing Guarantees
To prevent AI livelocks and erratic whim oscillation:
1. **25% Utility Hysteresis ($\Theta_{\text{hysteresis}} = 0.25 \cdot U_{\text{max}}$):** An agent executing a task switches to a candidate chore only if candidate utility exceeds current utility by at least $25\%$.
2. **Atomic Action Commit:** Once physical execution (welding, digging, cooking) begins, it cannot be interrupted by non-critical whims until the action phase finishes.
3. **Distance-Adaptive TTL Lease & Heartbeat Transit Renewal:** Tools and beds are reserved with a distance-scaled Time-To-Live:
   $$\text{TTL}_{\text{init}} = \max\left(60, \left\lceil \frac{d(\text{agent}, \text{target})}{\text{speed}} \right\rceil \times 1.5\right)\text{ ticks}$$
   Continuous forward motion emits a heartbeat lease renewal every 15 ticks. Halting movement allows the lease to expire cleanly without resource lockup.
4. **30-Tick Progress Watchdog with Exponential Backoff:** If distance to a goal fails to decrease over 30 consecutive ticks, the task transitions to `ACTION_BLOCKED`. The agent steps back $1\text{ dm}$, releases all locks, and blacklists the destination with exponential backoff:
   $$\text{BlacklistDuration}(k) \in \{ 30\text{ s}, 60\text{ s}, 120\text{ s}, 300\text{ s} \} \quad (900, 1800, 3600, 9000\text{ ticks at } 30\text{ Hz})$$

#### 4.2.5. Community Scaling: Expansion via Leadership & Delegation
As Lot 402 grows into an ecovillage, the player **never gains god-mode control over incoming NPCs**:
* **The Morning Circle (06:00):** Founder 01 rings the bell; open BPMN WorkTokens are displayed on the bulletin board.
* **Voluntary Task Claiming:** NPCs evaluate open tokens against their facets and skills.
* **Trust Rings ($R_0 \to R_6$):** Founder 01 delegates critical infrastructure (inverters, seed vaults) only to citizens in **Trust Ring R1 (Vouched Stewards)**. Commanding a **Trust Ring R6 (Unverified Migrant)** or **Ring R3 (Probationary Resident)** newcomer without trust causes friction and token refusal.

---

### 4.3. Directive 3: Cities: Skylines Macro Infrastructure Reclamation & Bridging

#### 4.3.1. The Omnipresent Legacy Reality
Traditional city builders isolate the player within an empty paradise where utilities flow infinitely from map borders. In Oasis, the player exists within the aging, fragile reality of late-stage industrial infrastructure.

Our core macroscopic gameplay loop is **"Metabolic Scavenging & Infrastructure Reclamation"**: you survive by structurally bridging, reclaiming, intercepting, and upcycling the decaying material and utility flows of the legacy world while progressively building regenerative autarky.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              THE LEGACY INFRASTRUCTURE RECLAMATION MATRIX                        │
├─────────────────────────┬───────────────────────────────┬────────────────────────────────────────┤
│ Target Legacy System    │ Bridging / Reclamation Method │ Operational Hazard / Regulatory Friction│
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ High-Voltage Power Grid │ Inductive line couplers, peak │ Arc flash hazard, transformer overload,│
│ (12kV / 240V drops)     │ shaving bridges, PV buffer    │ smart-meter phase imbalance alert      │
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Municipal Water & Sewer │ Saddle-valve branch junctions,│ Water pressure drop alert, backflow    │
│ (Cast-iron mains)       │ stormwater bioswale catchment │ contamination, municipal shutoff order │
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Asphalt, Concrete &     │ Depaving driveways, cutting   │ Municipal zoning citations, slab shear │
│ Structural Rebar        │ salvaged rebar, crushing rock │ collapse, silica & coal-tar dust hazard│
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Fiber & Copper Telecom  │ Roadside terminal bridging,   │ Carrier line fault detection, ISP      │
│ (Roadside punch blocks) │ dark-fiber community uplinks  │ signal sweep, unshielded traffic audit │
└─────────────────────────┴───────────────────────────────┴────────────────────────────────────────┘
```

#### 4.3.2. Detailed Utility Bridging & Material Reclamation Mechanics

##### 1. Power Grid Bridging & Smart Meter Anomaly Counter:
* **The Bridge:** Founder 01 couples an auxiliary transfer bridge to a 240V utility drop to stabilize early-stage microgrid battery banks before sufficient solar capacity is erected.
* **Exergy Yield:** Delivers up to $7.2\text{ kW}$ directly into local LFP battery storage.
* **Smart Meter Anomaly Counter:** Drawing $> 30\text{ A}$ unbuffered creates a localized thermal signature and grid-phase imbalance. The utility's smart anomaly counter (modeled by Layer 7 `col-adversaryd` DES) rises by $+1\%$ per 100 ticks. At $100\%$, a municipal inspection crew accompanied by code enforcement audits the parcel. Drawing $< 10\text{ A}$ safely dissipates the counter at $-0.5\%$ per 100 ticks.
* **Successive Over-Relaxation (SOR) Electrical Flow Solver:**
  Current propagation across 12kV distribution lines and 240V drops is modeled via **SOR ($\omega = 1.6$)**:
  $$\mathbf{G}\mathbf{V} = \mathbf{I}$$
  Because low-impedance copper distribution lines ($R \approx 0.005\ \Omega, g \approx 200\text{ S}$) connect directly to high-impedance local battery drops ($R \approx 8.0\ \Omega, g \approx 0.125\text{ S}$), extreme conductance contrast stalls unrelaxed solvers. SOR contracts spectral error rapidly, converging in $< 20$ iterations ($\epsilon < 10^{-4}$). To prevent singular floating matrices ($\det(\mathbf{G}) = 0$) during islanding, the microgrid incorporates a virtual earth ground node ($g_{\text{virtual\_earth}} = 10^{-6}\text{ S}$), backed by a direct Sparse Cholesky fallback ($< 0.12\text{ ms}$) during topology splices.

##### 2. Municipal Water Interception & Bioswale Catchment:
* **The Reclamation:** Excavating $2\text{ m}$ to service a 4-inch ductile iron main or grading keyline bioswales to harvest sheet-flow stormwater runoff.
* **Exergy Yield:** Pressurized water and gravity-fed retention ponds for cover crops and domestic living.
* **Operational Hazard:** Sudden municipal pressure drops alert SCADA systems. Storm runoff interception carries road oils and heavy metals that require biochar filtration beds and mycelium bio-filtration before potable or agricultural use.

##### 3. Asphalt Depaving, Concrete & Rebar Quarrying (Urbanite):
* **The Reclamation:** Jackhammering impervious asphalt driveways and parking lots to expose living subsoil; cutting salvaged steel rebar with oxy-acetylene torches.
* **Exergy Yield:** Produces **Urbanite** (reclaimed concrete blocks) for insulated building foundations, rocket mass heaters, and terrace retaining walls, while restoring soil permeability.
* **Operational Hazard:** Generates silica dust and bitumen fumes; unmasked workers suffer rapid lung irritation thoughts and physical fatigue.

##### 4. Fiber & Copper Telecom Bridging:
* **The Reclamation:** Bridging roadside terminal blocks or lighting community dark-fiber links.
* **Exergy Yield:** Synchronizes open-hardware CAD blueprints, firmware, and meteorological datasets into the local Layer 6 semantic cache.
* **Operational Hazard:** Unencrypted legacy traffic triggers automated ISP rate-limiting or regulatory audits.

#### 4.3.3. Multi-Environment Biome Profiles
Oasis simulates three radically distinct biomes:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                   MULTI-BIOME SPECIFICATION MATRIX                               │
├───────────────────┬──────────────────────────┬──────────────────────────┬────────────────────────┤
│ Characteristic    │ Suburban Sprawl          │ Dense Urban Block        │ Permaculture Eco-Node  │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Spatial Envelope  │ Sprawling Horizontal     │ Deep Vertical Canyons    │ Contoured Topography   │
│                   │ (128x128x16 dm)          │ (64x64x64 dm)            │ (96x96x32 dm)          │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Legacy Tethers    │ Overhead power lines,    │ High-voltage conduits,   │ Scavenged storm drains,│
│ (Layer 7 DES)     │ shallow water mains,     │ steam tunnels, subway    │ boundary fence lines,  │
│                   │ residential meters       │ vaults, fiat property tax│ off-grid well taps     │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Primary Materials │ Asphalt, Pine Siding,    │ Structural Steel,        │ Fungal Loam, Cob,      │
│                   │ Drywall, Turf Grass      │ Reinforced Concrete      │ Biochar, Bamboo, Clay  │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Hydrology Loop    │ Lawn runoff, impervious  │ Sump pumps, stormwater   │ Keyline swales, ponds, │
│                   │ asphalt gutter routing   │ retention vaults, sewage │ greywater reed beds    │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Reclamation Vector│ Bridging grid drops &    │ Capturing waste heat,    │ Restoring dead topsoil,│
│                   │ depaving suburban lots   │ upcycling structural rebar│ harvesting rain runoff │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Regulatory Focus  │ HOA bylaws, drone lawn   │ Building inspectors,     │ Downstream senior water│
│ (DES Modeling)    │ inspections ($150/day)   │ FLIR thermal sweeps      │ rights enforcement     │
└───────────────────┴──────────────────────────┴──────────────────────────┴────────────────────────┘
```

#### 4.3.4. Spatial Chunk Memory & 128-Chunk Active Visibility Window
To render expansive biomes without exceeding WASM's 256.00 MB linear memory ceiling, the engine maintains a **128-chunk active sector visibility window** ($128 \times 128\text{ KB} = 16.38\text{ MB}$). A full neighborhood sector ($8 \times 8 \times 4$ chunks = 256 chunks) is simulated hierarchically: the core 128 chunks closest to Founder 01 execute full-fidelity thermodynamics, hydrology, and voxel physics, while outer chunks utilize compressed macro-block representations.

#### 4.3.5. The Macro Progression Arc: Three Epochs of Sovereignty
The journey of Lot 402 spans three historical epochs:
1. **Epoch 1: Legacy Utility Dependency:** High reliance on municipal utilities. Rapid fiat escrow drain ($\$1,450$ starting balance). Vulnerability to tariff rate hikes, supply-chain bottlenecks, and smart-meter audits.
2. **Epoch 2: Metabolic Scavenging & Regenerative Transition (Symbiotic Dual-Membrane):** Bridging auxiliary power, harvesting rainwater in keyline swales, depaving driveways into cover-crop food forests, quarrying urbanite for insulated rooms, and establishing compost toilets and greywater reed beds.
3. **Epoch 3: Sovereign Severance (Singularity):** Physical utility drops voluntarily disconnected. Solar microgrid and rain catchment achieve $\ge 125\%$ autarky. The municipal meter stops spinning, and monthly fiat drain drops to zero.

---

### 4.4. Directive 4: Sovereign Stack Multiplayer & Stochastic Boundary Interface

#### 4.4.1. Local-First Simulation Philosophy & Voluntary MMO Scaling
Oasis rejects extractive, fragile cloud-server dependencies while enabling voluntary communal federation:
* **100% Local-First Execution:** The simulation executes deterministically on the user's local hardware with zero mandatory cloud lock-in. World voxel state, character psychology, and event logs reside in local storage (SQLite/LMDB or browser OPFS).
* **Total Offline Resilience:** If WAN connectivity drops, your Oasis simulation continues running deterministically without interruption.
* **Voluntary Shared Landscapes (`Arcadia server-01`):** Based strictly on *willing voluntariness*, local nodes can federate over the Sovereign Stack mesh (Layers 2–5) to co-inhabit persistent shared virtual landscapes such as **`Arcadia (server-01)`**, synchronized via Sprint 9 Temporal `FlowContract` streams and Mandatory Milestone Epoch barriers (detailed in Section 6.6).

#### 4.4.2. The Stochastic Boundary Interface & Good-Faith DES Modeling
The perimeter of the simulated parcel is not an invisible wall. It is an active **Stochastic Boundary Interface** driven by Layer 7 (`col-adversaryd`), which uses **Discrete Event Simulation (DES)** to model the structural, economic, and demographic pressures of the surrounding legacy fiat world in good faith:

```
                  ┌──────────────────────────────────────────────────┐
                  │       THE LEGACY WORLD (`col-adversaryd` DES)    │
                  │   - Tariff shifts, tax assessments, code audits  │
                  │   - Grid brownouts, drought shocks, NPC waves    │
                  └─────────────────────────┬────────────────────────┘
                                            │
                                            ▼  Stochastic DES Pressure & Migration Vectors
                  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                  ░░░░░░░░░░░ STOCHASTIC BOUNDARY INTERFACE ░░░░░░░░░░
                  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                            │
                                            ▼  Filtered & Attenuated Stress / Assimilation
                  ┌──────────────────────────────────────────────────┐
                  │           SOVEREIGN OASIS SANCTUARY              │
                  │   - Regenerated living soil & micro-grid exergy  │
                  │   - Insulated rooms (>18°C) & Recreation Zones   │
                  │   - Assimilated Legacy NPCs & Arcadia Mesh Ties  │
                  └──────────────────────────────────────────────────┘
```

##### Inhomogeneous Poisson Arrival Process:
External legacy pressure shocks and migration events arrive according to an inhomogeneous Poisson process with time-varying intensity $\lambda(t)$:
$$\lambda(t) = \max\left(\lambda_{\min}, \lambda_{\mathcal{R}} \cdot \exp(-\Psi(t))\right)$$

Where:
* $\lambda_{\mathcal{R}}$ is the baseline arrival intensity governed by current regulatory/macroeconomic regime $\mathcal{R}(t) \in \{\text{Sub-Radar}, \text{Under-Scrutiny}, \text{Regulatory-Friction}, \text{Systemic-Crisis}\}$.
* $\lambda_{\min} = 0.001\text{ s}^{-1}$ is the **irreducible legacy friction floor** (approx. 1 event per 16.6 minutes), ensuring off-grid resilience is continuously verified.
* $\Psi(t)$ is the **Sovereign Attenuation Potential** driven by the three attenuation pillars.

#### 4.4.3. The Three Attenuation Pillars
$$\Psi(t) = \alpha \cdot \Phi_{\text{infrastructure}}(t) + \beta \cdot \Phi_{\text{social}}(t) + \gamma \cdot \tanh\left(\delta \cdot \Phi_{\text{mesh}}(t)\right)$$

1. **Physical Self-Sufficiency ($\Phi_{\text{infrastructure}}$) & Genesis Epsilon Guard:**
   $$\Phi_{\text{infrastructure}}(t) = \frac{1}{3} \left[ \min\left(1.0, \frac{E_{\text{solar\_stored}} + \epsilon}{E_{\text{consumed}} + \epsilon}\right) + \min\left(1.0, \frac{W_{\text{rain\_stored}} + \epsilon}{W_{\text{consumed}} + \epsilon}\right) + \frac{A_{\text{depaved\_loam}}}{A_{\text{total\_lot}}} \right]$$
   At Day 1 start, consumption is zero ($E_{\text{consumed}} = 0, W_{\text{consumed}} = 0$). To prevent catastrophic $0/0 \implies \text{NaN}$ singularities, all evaluations include the **Genesis Autarky Epsilon Guard** ($\epsilon = 0.001\text{ kWh/L}$):
   $$\eta = \frac{E_{\text{self}} + \epsilon}{E_{\text{legacy}} + E_{\text{self}} + \epsilon}$$
2. **Social NPC Assimilation ($\Phi_{\text{social}}$):**
   $$\Phi_{\text{social}}(t) = \ln\left(1 + \sum_{k=1}^{N_{\text{agents}}} \frac{\text{Trust}_k \cdot \text{MutualAidHours}_k}{100}\right)$$
   Welcoming Legacy NPC Migration Waves with warm shelter, clean water, and caloric security—and integrating them into Trust Ring 1—transforms stressed newcomers into capable stewards who strengthen the enclave.
3. **Peer Proximity Mesh Ties ($\Phi_{\text{mesh}}$) & Bounded Weighting:**
   $$\Phi_{\text{mesh}}(t) = \frac{1}{|\mathcal{P}|_{\max}} \sum_{j \in \mathcal{P}} \frac{\text{TrustScore}_{ij}}{1 + \left(\frac{d_{ij}}{d_0}\right)^2}$$
   Where $\mathcal{P}$ is the set of mesh peer nodes connected via Reticulum radio or `Arcadia (server-01)` relays, $d_{ij}$ is distance (km), and $d_0 = 1.0\text{ km}$. Bounded via $\tanh(\delta \cdot \Phi_{\text{mesh}}) \in [0, 1.0)$, preventing IEEE-754 floating-point underflow.

#### 4.4.4. Sovereign Stack Exclusive Multiplayer Protocol Matrix
Oasis contains zero custom P2P networking code. All multiplayer coordination routes through the Sovereign Stack via the UHAI IPC boundary, with strict layer separation:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                D4 MULTIPLAYER LAYER ALLOCATION MATRIX                            │
├───────┬──────────────────────────────┬───────────────────────────────────────────────────────────┤
│ LAYER │ SOVEREIGN STACK SUBSYSTEM    │ MULTIPLAYER FUNCTIONALITY & PROTOCOL SPECIFICATION        │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L2**│ Physical/Mesh Radio &        │ Reticulum Network Stack (RNS), LoRa (SX1262), 802.11s     │
│       │ Delay-Tolerant Transport     │ Wi-Fi Mesh; Cryptographic Distance Bounding PHY; UHAI IPC │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L3**│ Distributed State Ledger,    │ Kademlia DHT, Sphinx Onion Routing, libp2p Gossipsub;     │
│       │ PoTW & Mutual Credit Clearing│ Automerge/Yrs CRDT (`StateDeltaOp` 24B cadastral records);│
│       │ (`col-storaged`)             │ **Zero-Sum Mutual Credit Clearing (BCC / Johnson's Cycle- │
│       │                              │ Canceling)**, **Exponential Demurrage**, & `FlowContract` │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L4**│ Keyless Deterministic BPMN   │ **Keyless Deterministic 10 Hz Wasmtime BPMN Execution     │
│       │ Execution Engine             │ Engine**; 5-Tier Priority Queue, instruction-fuel metering│
│       │ (`col-execd`)                │ ecological floor enforcement, `WorkToken` & `[YIELD]`     │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L5**│ Cryptographic Identity, VCs  │ **W3C DID Core (`did:key`, `did:peer`)**, **Ed25519 /     │
│       │ & FROST Governance Engine    │ FROST Threshold Keypairs** (`mlockall()`), **BBS+ ZKP     │
│       │ (`col-kmsd`)                 │ Verifiable Credentials**, **Trust Rings ($R_0 \to R_6$)** │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L7**│ Legacy World DES Emulation & │ Good-Faith Discrete Event Simulation (`col-adversaryd`);  │
│       │ Migration Wave Generators    │ Tariff Modeling, Code Audits, `MigrationWaveArrivalEvent` │
└───────┴──────────────────────────────┴───────────────────────────────────────────────────────────┘
```

#### 4.4.5. Cryptographic Geo-Proximity & Boundary Perimeter Consolidation
Physical distance between nodes is verified via **cryptographic speed-of-light distance bounding**:
$$d_{AB} \le \frac{c \cdot (\Delta t - t_d)}{2}$$

Where $c \approx 3 \times 10^8\text{ m/s}$, $\Delta t$ is round-trip time, and $t_d \le 1.0\text{ ns}$ is hardware turnaround.

##### Isoperimetric Perimeter Consolidation:
Two independent square parcels of area $A$ have a perimeter of $8\sqrt{A}$. When merged along a shared property edge, their interior border dissolves, reducing exposed perimeter to $6\sqrt{A}$—an immediate **$25\%$ reduction in exposed boundary surface area**.

##### Bounded Boundary Friction Coefficient ($\kappa$):
$$\kappa = \kappa_{\text{base}} \cdot \prod_{j \in \text{Peers}} \max\left(0.0, 1 - \alpha \cdot \frac{R_j}{\max(d_j, d_{\min})}\right) \cdot (1 - \beta \cdot P_{\text{buffer}}) \cdot \left(1 - \tanh\left(\gamma \cdot N_{\text{citizens}}\right)\right)$$
Strictly bounded to $\kappa \in [0.05, 1.0]$. The bounded term $\left(1 - \tanh(\gamma \cdot N_{\text{citizens}})\right)$ eliminates negative friction coefficient sign-inversion bugs. As $\kappa$ drops to $0.05$, external regulatory friction drops by $95\%$, dissolving the Fog of Legacy along the shared line into an open **Commons Corridor**.

---

## 5. Inviolable Architectural & Anti-Bleed Invariants

### 5.1. The 6 Anti-Bleed & Airgap Invariants
To preserve strict Separation of Concerns (SoC) and hardware safety, Oasis enforces six inviolable architectural boundaries:
1. **Zero Embedded Daemons:** Oasis never compiles, links, or embeds Sovereign Stack daemons (`col-telemetryd` through `col-adversaryd`).
2. **Zero In-Engine Private Keys:** Oasis never touches raw Ed25519, FROST, or BBS+ private keys. Keys reside exclusively in `col-kmsd` (Layer 5) under `mlockall()` memory protection. Oasis holds only an opaque 32-byte `author_did` handle.
3. **Zero Custom P2P Networking:** Oasis contains zero libp2p, zero WebRTC sockets, zero LoRa packet decoders, and zero P2P mesh code. All multiplayer routes through Layer 2 (`col-meshd`) and Layer 3 (`col-storaged`).
4. **Zero In-Engine BPMN XML Parsers:** Oasis emits canonical 264-byte binary `SpatialIntentPayload` structs; Layer 4 (`col-execd` Keyless Deterministic 10 Hz Wasmtime BPMN Engine) executes BPMN 2.0 workflows, enforces ecological floors, and returns `WorkToken` packets.
5. **Zero In-Engine Fiat Accounting:** Oasis simulates physical exergy (watts, liters, joules); Layer 7 (`col-adversaryd`) models fiat tariffs and municipal citations via DES.
6. **Outward-Only Simulated Telemetry (`QUALITY_SIMULATED = 0x01`):** Oasis is strictly an **outward producer** of synthetic 32-byte `TelemetrySample` frames into the Sovereign Stack for SITL stress-testing and **never ingests live physical telemetry** to recalibrate the simulator. Every outbound telemetry frame is stamped with `quality_flag = 0x01` (`QUALITY_SIMULATED`), guaranteeing that Sprint 4's Hardware Airgap Filter mathematically drops simulated packets before they can ever actuate physical hardware.

### 5.2. 256.00 MB WASM Linear Memory Partitioning
The WebAssembly runtime memory is strictly partitioned to 268,435,456 bytes ($256.00\text{ MB}$):

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                         256.00 MB WASM LINEAR MEMORY RECONCILIATION                              │
├───────────────────────────────────┬───────────────┬──────────────────────────────────────────────┤
│ Memory Subsystem Partition        │ Allocation    │ Purpose & Static Data Structures             │
├───────────────────────────────────┼───────────────┼──────────────────────────────────────────────┤
│ 1. Active Sector Voxel Geometry   │ 16.38 MB      │ 128 chunks @ 128 KB (32³ voxels, 4B/voxel)   │
│ 2. Voxel Macro-Chunk Grid         │ 32.00 MB      │ Two-level coarse DDA acceleration grid       │
│ 3. Material Properties & Textures │ 16.00 MB      │ Static WGSL material parameter uniforms      │
│ 4. Flecs ECS World & Storage      │ 32.00 MB      │ Packed component tables (1,024B/agent)       │
│ 5. Agent Psychology & Memory Rings│ 8.00 MB       │ 64B psychology + 32x24B episodic ring buffers│
│ 6. UHAI Lock-Free Ring Buffers    │ 4.00 MB       │ 64-byte aligned SPSC telemetry & intents     │
│ 7. Audio Waveform & Somatic Cache │ 32.00 MB      │ Procedural sound clips, acoustic impulse     │
│ 8. Cadastral CAD & Mesh Buffers   │ 16.00 MB      │ Blueprint lines, UI meshes, Plumbob geometry │
│ 9. Emscripten Heap & Stack        │ 32.00 MB      │ Thread stacks, libc runtime, C++ runtime     │
│ 10. Reserved Safety Cushion       │ 67.62 MB      │ WebGPU staging buffers, dynamic scratch space│
├───────────────────────────────────┼───────────────┼──────────────────────────────────────────────┤
│ TOTAL ARITHMETIC RECONCILIATION   │ 256.00 MB     │ Exactly 268,435,456 bytes (Zero runtime leak)│
└───────────────────────────────────┴───────────────┴──────────────────────────────────────────────┘
```

Zero heap allocation (`malloc`/`new`) occurs on the simulation tick.

### 5.3. WebGPU Amanatides & Woo 3D DDA Compute Raymarcher Pipeline
* **Screen-Space Raymarcher:** Renders a full-screen triangle in vertex shader, executing Amanatides & Woo voxel traversal in the fragment shader.
* **Hierarchical Two-Level DDA:** A coarse macro-chunk occupancy grid ($1\text{ bit per } 16\times 16\times 16$ block, 8 KB VRAM per sector) reduces primary ray steps from $\sim 45$ to $< 12$, worst-case from 180 to $\le 36$, and texture lookups from $> 93\text{M}$ to $< 24\text{M}$ per frame. Raymarching completes in $2.60\text{ ms} - 3.80\text{ ms}$.
* **Frame Time Budget (60 FPS / 16.66 ms):**
  * CPU Uniform Prep: $1.20\text{ ms}$
  * DDA Compute Raymarch: $3.80\text{ ms}$
  * Two-Phase Deferred PBR: $1.80\text{ ms}$
  * Cadastral CAD / Fog / `F1–F4` Info-View Pass: $0.60\text{ ms}$
  * **Total Active Frame Time:** $7.40\text{ ms}$ ($10.60\text{ ms}$ hard budget cap), leaving $> 36\%$ safety headroom.

### 5.4. UHAI IPC Bridge & Canonical Binary Struct Layouts (`uhai_ring_buffer.hpp`)
* **High-Frequency Outward Data Plane:** POSIX SPSC shared memory ring buffer (`/dev/shm/uhai_telemetry.ring`, $< 50\ \mu\text{s}$ latency) carrying outward 32-byte `TelemetrySample` payloads (packed inside 64-byte cache-aligned `TelemetrySlot` lines) stamped with `QUALITY_SIMULATED (0x01)` from Layer 1b (Oasis) into Layer 2/3 daemons.
* **Cache-Line Isolation:** `SharedRingHeader` occupies 3 cache lines (192 bytes): Cache line 0 (producer write index & dropped frames counter); Cache line 1 (consumer read index); Cache line 2 (static config). Total SHM size is 65,728 bytes.
* **Asynchronous Control Plane:** Unix Domain Sockets (`/var/run/collective/uhai.sock`) and lock-free SPSC rings transporting 264-byte `SpatialIntentPayload` designations, 64-byte Sprint 9 `FlowContract` packets, BPMN `WorkToken` packets, and BBS+ verifiable credentials.

```cpp
namespace oasis::uhai {

// Canonical 32-Byte Outward Telemetry Sample (`uhai_ring_buffer.hpp`)
#pragma pack(push, 8)
struct alignas(8) TelemetrySample {
    uint64_t frame_count{0};        // 8B: Monotonic engine simulation/render frame count
    uint64_t timestamp_ns{0};       // 8B: Nanosecond timestamp (HLC or steady_clock)
    float    fps{0.0f};             // 4B: Current WebGPU render frame rate (60.0 Hz target)
    float    founder_stress{0.0f};  // 4B: Founder psychological stress accumulator [0..100,000]
    uint32_t memory_slot_count{0};  // 4B: Active slots in 32-slot episodic memory ring
    uint8_t  biome_id{0};           // 1B: 0=Suburban, 1=Urban, 2=Permaculture
    union {
        uint8_t quality_flags{0x01};// 1B: Strictly 0x01 (QUALITY_SIMULATED)
        uint8_t quality_flag;
    };
    uint16_t reserved{0};           // 2B: Padding for strict 8-byte alignment
};
#pragma pack(pop)
static_assert(sizeof(TelemetrySample) == 32, "TelemetrySample must be exactly 32 bytes.");

// Canonical 264-Byte Spatial Intent Payload (`uhai_ring_buffer.hpp`)
#pragma pack(push, 8)
struct alignas(8) SpatialIntentPayload {
    uint8_t  intent_cid[32]{};      // 32B: SHA256 / CID digest of JSON-LD schema or treaty spec
    uint8_t  author_did[32]{};      // 32B: Opaque 32-byte founder DID handle (Anti-Bleed Inv 2)
    uint64_t timestamp_ns{0};       // 8B:  Nanosecond epoch timestamp
    uint16_t intent_type{0};        // 2B:  IntentType enum (1=SPATIAL_AFFORDANCE, etc.)
    uint8_t  quality_flag{0x01};    // 1B:  Strictly 0x01 (QUALITY_SIMULATED)
    uint8_t  status{0};             // 1B:  0=Draft, 1=Validated, 2=Compiled
    uint32_t payload_bytes{0};      // 4B:  Byte length of inline JSON-LD payload
    float    spatial_origin[3]{};   // 12B: 3D Voxel coordinates (x, y, z) for Cadastral anchor
    float    spatial_extents[3]{};  // 12B: Voxel bounding box extents (dx, dy, dz)
    char     schema_uri[32]{};      // 32B: JSON-LD schema or target URI (e.g., "oasis:dig_swale")
    char     payload_jsonld[128]{}; // 128B: Compact canonical JSON-LD intent parameters
};
#pragma pack(pop)
static_assert(sizeof(SpatialIntentPayload) == 264, "SpatialIntentPayload must be exactly 264 bytes.");

} // namespace oasis::uhai
```

### 5.5. The Sovereign Companion App Interaction Boundary & Intent U-Turn (V2.0 Law)
To eliminate UI bloat and preserve strict cybernetic isolation between 3D spatial simulation and societal governance, Version 6.0 codifies the **Dual-Surface Interaction Boundary Law (V2.0 Law)**:

1. **Layer 1 (Oasis WebGPU Desktop Client — Spatial Actuation Surface):**  
   The Oasis desktop client is the **exclusive human interface for physical-world spatial actuation**. All 3D Orbital/Arcball camera navigation, voxel raycasting, tactile Right-Click/Left-Click spatial designations (`[Dig Swale]`, `[Build Wall: Cob / Urbanite]`, `[Install Hardware: Inverter]`, `[Designate Sleep/Recreation Zone]`), and somatic character stewardship occur strictly inside Layer 1.
2. **Layer 6 (Mobile Companion App via `col-gatewayd` on `TLS 1.3 HTTPS/WSS :8443 (VLAN 30)` — Exclusive Societal/Agora Interface):**  
   The Mobile Companion App—connected exclusively over mTLS/TLS 1.3 **`HTTPS/WSS :8443` on isolated `VLAN 30`** to `col-gatewayd`—is the **exclusive human interface for societal, economic, and Agora interactions** within The Collective (`POST /api/v1/l6/intent`), designed under the pure commons postulate that Layer 7 (the legacy fiat world) does not exist inside the Agora.
3. **Read-Only "Pop-the-Hood" Cybernetic Diagnostics (Layers 2–5 & Layer 7):**  
   Within the Companion App, inspection panels for **Layers 2, 3, 4, 5, and 7** are **strictly read-only "pop-the-hood" cybernetic telemetry views** (inspecting Reticulum RNS link SNR, Layer 3 BCC mutual-credit clearings and CRDT Merkle DAGs, Layer 4 Wasmtime BPMN token traces, Layer 5 FROST/DID key handles and Trust Rings $R_0 \to R_6$, and Layer 7 `col-adversaryd` DES boundary pressure). Users never mutate Layers 2–5 or Layer 7 directly from diagnostic views; every societal mutation must enter through Layer 6 and traverse the canonical **Intent U-Turn**:
   * **Downward Execution (`L6 -> L5 -> L4 -> L3 -> L2 -> L1 UHAI`):** Societal intents submitted at Layer 6 (`POST /api/v1/l6/intent`) pass to Layer 5 (`col-kmsd`) for DID/FROST signature verification and Trust Ring authorization, compile into deterministic BPMN tokens in Layer 4 (`col-execd`), **settle exergy and mutual-credit balances in Layer 3 (`col-storaged`)**, and actuate downward through **`L2 -> L1 (UHAI)`**.
   * **Upward Yield (`[YIELD] -> L5 -> L6 -> L7`):** When Layer 4 hits a human task gate, moral conflict, or covenant milestone (`[YIELD]`), it yields upward to **`L5: Governance & FROST Policy Evaluation (col-kmsd)`** (ratified by stewards via `POST /api/v1/l6/intent -> L5`), which yields upward to **`L6: Mundane Semantic Camouflage (col-commonsd)`**, and finally to **`L7: Legacy Bridge & DES (col-adversaryd)`** for external regulatory/tariff translation.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                 THE V2.0 COMPANION APP INTERACTION BOUNDARY & INTENT U-TURN                      │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│  [SURFACE A: LAYER 1 OASIS DESKTOP CLIENT]         [SURFACE B: LAYER 6 MOBILE COMPANION APP]     │
│  ┌───────────────────────────────────────┐         ┌──────────────────────────────────────────┐  │
│  │ - 3D WebGPU Voxel World & Arcball Cam │         │ - Connected via `TLS 1.3 HTTPS/WSS :8443`│  │
│  │ - Right-Click Spatial Designator      │         │   on isolated `VLAN 30` (`col-gatewayd`) │  │
│  │ - Physical Actuation & Chill Loop     │         │ - Exclusive Societal / Agora Interface   │  │
│  │ - Emits 32B `TelemetrySample (0x01)`  │         │ - Read-Only Diagnostics for L2–L5 & L7   │  │
│  └───────────────────┬───────────────────┘         └────────────────────┬─────────────────────┘  │
│                      │                                                  │                        │
│                      │ Outward `0x01` Telemetry                         │ `POST /api/v1/l6/intent`
│                      ▼                                                  ▼                        │
│              ┌───────────────┐                                  ┌───────────────────────────┐    │
│              │ L1b -> L2/L3  │                                  │ L6: Agora & Commons Engine│    │
│              │ Outward Only  │                                  │     (`col-commonsd`)      │    │
│              │ (`0x01` Flag) │                                  └─────────────┬─────────────┘    │
│              └───────────────┘                                                │ 1. Auth & Sign   │
│                                                                               ▼                  │
│                                                                 ┌───────────────────────────┐    │
│                                                                 │ L5: DID, FROST & Trust    │    │
│                                                                 │  Rings R0..R6 (`col-kmsd`)│    │
│                                                                 └─────────────┬─────────────┘    │
│                                                                               │ 2. Compile BPMN  │
│                                                                               ▼                  │
│   ┌──────────────────────────────────────────────────────────┐  ┌───────────────────────────┐    │
│   │ Downward Ledger Settlement & Physical/Sim Actuation:     │  │ L4: Keyless 10Hz Wasmtime │    │
│   │ L4 ──► L3 (`col-storaged` Exergy & BCC Mutual Credit)    │◄─┤   BPMN VM (`col-execd`)   │    │
│   │    ──► L2 (`col-meshd`) ──► L1 (UHAI Ring Actuation)     │  └─────────────┬─────────────┘    │
│   └──────────────────────────────────────────────────────────┘                │                  │
│                                                                         [ Y I E L D ]            │
│                                                                    (Human Gate / Exception)      │
│                                                                               │                  │
│                                                                               ▼                  │
│                                                                 ┌───────────────────────────┐    │
│                                                                 │ L5: Governance & FROST    │    │
│                                                                 │ Policy Eval (`col-kmsd`)  │    │
│                                                                 │ (Ratified via L6 -> L5)   │    │
│                                                                 └─────────────┬─────────────┘    │
│                                                                               │ 3. Upward Camo   │
│                                                                               ▼                  │
│                                                                 ┌───────────────────────────┐    │
│                                                                 │ L6: Mundane Semantic      │    │
│                                                                 │ Camouflage (`col-commonsd`)    │
│                                                                 └─────────────┬─────────────┘    │
│                                                                               │ 4. DES Bridge    │
│                                                                               ▼                  │
│                                                                 ┌───────────────────────────┐    │
│                                                                 │ L7: Legacy Bridge & DES   │    │
│                                                                 │    (`col-adversaryd`)     │    │
│                                                                 └───────────────────────────┘    │
│                                                                                                  │
│     CANONICAL INTENT U-TURN:  L6 ──► L5 ──► L4 (──► L3 ──► L2 ──► L1)                            │
│                                             └─► [YIELD] ──► L5 ──► L6 ──► L7                     │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## 6. The Six Horizon Playability Pillars (V6.0 Specification)

To transform Oasis from an uncompromising thermodynamic simulation into a deeply tactile, playable, and responsive strategy experience on par with *Dwarf Fortress*, *The Sims*, and *Cities: Skylines*, Version 6.0 establishes **The Six Horizon Playability Pillars**, anchored to unified numerical simulation constants across the C++ engine and Sovereign Stack.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                     UNIFIED V6.0 SIMULATION & HOMEOSTASIS CONSTANTS TABLE                        │
├─────────────────────────────────────────┬────────────────────────────────────────────────────────┤
│ Parameter / Invariant                   │ Canonical Value & Mathematical Specification           │
├─────────────────────────────────────────┼────────────────────────────────────────────────────────┤
│ **Base Simulation Tick Rate ($1\times$)**│ **`30 Hz` fixed step** ($\Delta t_{\text{fixed}} = 33.333\text{ ms}$, $28,800\text{ ticks/day}$)│
├─────────────────────────────────────────┼────────────────────────────────────────────────────────┤
│ **Mandatory Milestone Epoch Length**    │ **`EPOCH_BOUNDARY_TICKS = 864,000` ticks** ($30\text{ simulated days}$)│
├─────────────────────────────────────────┼────────────────────────────────────────────────────────┤
│ **Insulated Room Sleep Recovery**       │ $T_{\text{room}} > 18.0^\circ\text{C}$ enclosed: **`+45 Focus/hr`**, $\dot{C}_{\text{thermal}} = 0\text{ kcal/hr}$│
├─────────────────────────────────────────┼────────────────────────────────────────────────────────┤
│ **Recreation Zone Stress Drainage**     │ **`-25%/hr`** ($-25,000/\text{hr}$) solo; **`-40%/hr`** ($-40,000/\text{hr}$) social; **`-10%/hr`** `AutonomousSelfSooth`│
├─────────────────────────────────────────┼────────────────────────────────────────────────────────┤
│ **Burnout Entry / Exit Hysteresis**     │ Entry at **`100%` Stress** ($100,000$); Exit below **`< 40%` Stress** ($40,000$)│
├─────────────────────────────────────────┼────────────────────────────────────────────────────────┤
│ **Migrant Trust Ring Progression**      │ **`R6 (Unverified Migrant)`** $\xrightarrow{7\text{d} + 36\text{ MJ PoTW}}$ **`R3`** $\xrightarrow{30\text{d} + \text{2-of-3 FROST}}$ **`R1`**│
└─────────────────────────────────────────┴────────────────────────────────────────────────────────┘
```

---

### 6.1. Pillar 1: Orbital / Arcball Camera Controls (Dynamic POI Spherical Kinematics)

#### 6.1.1. Playability Rationale
While `Shift + Left Click (Drag)` provides rapid 2D planar panning across the cardinal axes (North, East, South, West), navigating complex 3D voxel structures—such as multi-story urban canyons, subterranean root cellars, and interior rocket-heater flues—requires an intuitive 3D orbit camera anchored to the exact geometry the player is inspecting, complete with automatic roof/wall cutaway so interior rooms are never occluded.

#### 6.1.2. Interaction, Reticle & Cutaway Kinematics
1. **Dynamic Point-of-Interest (POI) Lock (`Shift + Right Click`) & 3D Wireframe Reticle:**  
   On the rising edge of `Shift + Right Mouse Button`, the engine casts a screen-space ray $\mathbf{R}(t) = \mathbf{O}_{\text{cam}} + t \cdot \hat{\mathbf{d}}_{\text{mouse}}$ through the Amanatides & Woo 3D DDA voxel grid. The intersected voxel center $\mathbf{P}_{\text{POI}} = (x_{\text{vox}} + 0.5, y_{\text{vox}} + 0.5, z_{\text{vox}} + 0.5)$ becomes the spherical orbit origin, immediately highlighted in-world by a **3D Wireframe POI Reticle** (pulsing cyan $1.02\times$ bounding cube).
2. **Spherical Orbit (`Shift + Right Drag`):**  
   Dragging the cursor by screen-space deltas $(\Delta x_{\text{px}}, \Delta y_{\text{px}})$ updates the azimuth angle $\theta \in [0, 2\pi)$ and clamped elevation pitch angle $\phi \in [5.0^\circ, 85.0^\circ]$ ($[0.0873\text{ rad}, 1.4835\text{ rad}]$, preventing both ground-plane clipping and zenith gimbal lock):
   $$\theta_{t+1} = (\theta_t - s_\theta \cdot \Delta x_{\text{px}}) \bmod 2\pi, \qquad \phi_{t+1} = \text{clamp}\left(\phi_t + s_\phi \cdot \Delta y_{\text{px}},\, 5.0^\circ,\, 85.0^\circ\right)$$
3. **Radial Zoom (`Shift + Mouse Wheel`):**  
   Scrolling the mouse wheel while holding `Shift` scales the radial distance $r \in [r_{\min}, r_{\max}]$ ($r_{\min} = 1.5\text{ m}$ for intimate interior room inspection, $r_{\max} = 180.0\text{ m}$ for macro enclave overview):
   $$r_{t+1} = \text{clamp}\left(r_t \cdot (1.0 - \kappa_{\text{zoom}} \cdot \Delta w_{\text{wheel}}),\, 1.5\text{ m},\, 180.0\text{ m}\right)$$
4. **Automatic & Manual Roof/Wall Cutaway (`z_cutaway_level` WGSL Uniform):**  
   To prevent roofs and upper floors from blocking the player's view of interior furniture, sleeping zones, and agents, the DDA raymarcher uniform buffer exposes `z_cutaway_level`:
   * **Automatic Interior Cutaway:** Whenever the locked POI voxel $\mathbf{P}_{\text{POI}}$ resides inside an enclosed room (`RoomEnclosureID > 0`) OR radial zoom $r < 12.0\text{ m}$, `z_cutaway_level` automatically clamps to $y_{\text{POI}} + 2\text{ voxels}$, slicing away overhead roof voxels and camera-facing front walls along the ray path while preserving floor-level wall silhouettes.
   * **Manual Slice Override (`PageUp` / `PageDown`):** Players can manually step `z_cutaway_level` up or down voxel-by-voxel (*Dwarf Fortress* Z-slice style) at any zoom distance.
5. **Cartesian Camera Reconstruction:**  
   The eye position $\mathbf{O}_{\text{cam}}$ is reconstructed each render frame (60 Hz) around $\mathbf{P}_{\text{POI}}$ with exponential critically damped smoothing ($\alpha_{\text{cam}} = 1 - \exp(-18.0 \cdot \Delta t_{\text{render}})$):
   $$\mathbf{O}_{\text{cam}} = \mathbf{P}_{\text{POI}} + \begin{bmatrix} r \cos(\phi) \cos(\theta) \\ r \sin(\phi) \\ r \cos(\phi) \sin(\theta) \end{bmatrix}$$

#### 6.1.3. C++20 Orbital Camera Component Layout
```cpp
namespace oasis::camera {

struct alignas(32) OrbitalCameraState {
    float    poi_origin[3];         // 12B: Locked voxel POI world coordinates (x, y, z)
    float    radial_distance;       // 4B:  Distance r from POI [1.5m .. 180.0m]
    float    azimuth_theta;         // 4B:  Horizontal orbit angle [0 .. 2*pi]
    float    elevation_phi;         // 4B:  Vertical pitch angle [5.0 deg (0.0873 rad) .. 85.0 deg (1.4835 rad)]
    uint16_t locked_voxel[3];       // 6B:  Discrete voxel coordinate (vx, vy, vz) for 3D Wireframe Reticle
    uint8_t  z_cutaway_level;       // 1B:  Active WGSL Z-slice height (auto-clamped when r < 12m or indoor POI)
    uint8_t  flags;                 // 1B:  Bit 0: POI Reticle Active, Bit 1: Auto-Cutaway, Bit 2: Manual Z-Slice
};
static_assert(sizeof(OrbitalCameraState) == 32, "OrbitalCameraState must be 32 bytes.");

} // namespace oasis::camera
```

---

### 6.2. Pillar 2: Spatial Context Menus (The Designator & `SpatialIntentPayload`)

#### 6.2.1. Playability Rationale
Memorizing dozens of abstract keyboard hotkeys alienates players and obscures the rich affordances of the voxel world. Inspired by *Dwarf Fortress* and *RimWorld*, Oasis implements **The Designator**—a tactile, two-step spatial interaction model driven by `Right-Click (Evaluate)` and `Left-Click (Execute)`, complete with upfront metabolic cost badges, Cyan/Crimson validity previews, and full structural enclosure construction so players can build insulated rooms from the ground up.

#### 6.2.2. Tactile Two-Step Interaction & Material Loop
1. **Right-Click (Evaluate Voxel & Open Context Menu with Upfront Cost Badges):**  
   Right-clicking without `Shift` casts a DDA ray into the 3D voxel grid, inspects the target voxel's `material_id`, structural support state, and spatial tags, and opens a tactile **Spatial Context Menu** anchored to the 3D block. Every menu item displays upfront **Metabolic & Material Badges** (`[Cost: kcal | Focus | Material]`):
   * **Dirt / Topsoil / Clay Voxel (`MAT_SOIL`, `MAT_LOAM`, `MAT_CLAY`):**
     * `[Dig Swale]` — (`120 kcal | 15 Focus / vox`) Excavates contoured keyline water trench and **automatically drops loose `MAT_CLAY_COB_LUMP` / `MAT_URBANITE_CHUNK` resource items** onto adjacent voxels.
     * `[Plant Cover Crop]` — (`45 kcal | 10 Focus / vox`) Seeds nitrogen-fixing clover/vetch/daikon radish.
     * `[Designate Haul Zone]` — (`0 kcal | Paint-Only`) Creates a `StockpileZoneComponent` that automatically pulls dropped `MAT_CLAY_COB_LUMP`, `MAT_URBANITE_CHUNK`, `MAT_WOOD_CORD`, and harvest crates via autonomous hauling tasks.
     * `[Designate Sleep/Recreation Zone]` — (`0 kcal | Paint-Only`) Marks a sleeping or leisure zone (remains $100\%$ usable even during `STATE_BURNOUT`).
   * **Foundation / Structural Voxel (`MAT_URBANITE_SLAB`, `MAT_TIMBER_FLOOR`, `MAT_COB`, `MAT_SOIL` perimeter):**
     * `[Build Wall: Cob / Urbanite]` — (`180 kcal | 20 Focus | 2x MAT_CLAY_COB_LUMP or MAT_URBANITE_CHUNK`) Erects high-thermal-mass structural wall ($R\text{-value} = 24$).
     * `[Install Door: Insulated Timber]` — (`140 kcal | 25 Focus | 2x MAT_WOOD_CORD`) Installs a `MAT_DOOR` voxel (passable by agents for pathfinding, airtight $R\text{-value} = 12$ for thermal enclosure).
     * `[Build Roof: Timber & Shingle]` — (`160 kcal | 25 Focus | 2x MAT_WOOD_CORD`) Seals overhead room envelope against rain and winter heat loss.
     * `[Build Hearth: Rocket Mass Heater]` — (`350 kcal | 45 Focus | 4x MAT_URBANITE_CHUNK + 2x MAT_CLAY_COB_LUMP`) Constructs a high-efficiency thermal mass heater that heats enclosed rooms above $18.0^\circ\text{C}$.
     * `[Install Hardware: Inverter]` — (`250 kcal | 60 Focus | 1x Salvaged Inverter`) Mounts hybrid MPPT/split-phase inverter node.
     * `[Erect Solar Blueprint]` — (`220 kcal | 50 Focus | 1x PV Panel Array`) Places tilted photovoltaic rack assembly.
     * `[Designate Sleep/Recreation Zone]` — (`0 kcal | Paint-Only`) Defines indoor insulated dormitory or hearth/reading lounge.
   * **Loose Item / Salvage Voxel (`MAT_CLAY_COB_LUMP`, `MAT_URBANITE_CHUNK`, `MAT_SCRAP_METAL`, `MAT_WOOD_CORD`):**
     * `[Haul to Storage]` — (`35 kcal | 5 Focus`) Enqueues immediate transport of loose item to the nearest `StockpileZoneComponent`.
   * **Active Designation Voxel (Any Voxel with Pending Intent):**
     * `[Cancel Designation]` — (`0 kcal | Instant`) Aborts pending designation, refunds reserved material locks, and clears holographic overlay.
2. **Left-Click (Cyan/Crimson Volume Preview & `SpatialIntentPayload` Dispatch):**  
   Once an action is selected from the context menu, dragging with `Left-Click` renders a live volumetric bounding box $(x_{\min}..x_{\max}, y_{\min}..y_{\max}, z_{\min}..z_{\max})$:
   * **Valid Placement (Translucent Holographic Cyan):** Structural support, terrain grade, and required stockpile materials are satisfied.
   * **Invalid Placement (Pulsing Holographic Crimson):** Missing structural foundation, obstructed voxel, or insufficient material in `StockpileZoneComponent` (displays tooltip explaining the exact blocker).
   Releasing `Left-Click` on a valid volume instantiates a `VoxelDesignationComponent` in Flecs ECS and serializes a canonical **264-byte `SpatialIntentPayload`** (`alignas(8)`, defined in Section 5.4 and `uhai_ring_buffer.hpp`) onto the UHAI ring buffer.

---

### 6.3. Pillar 3: Psychological & Biological Homeostasis (The Chill Loop & Day 1 Genesis Camp Kit)

#### 6.3.1. Playability Rationale & The Day 1 Genesis Survival Camp Kit
In early prototypes (Sprints 1–5), dropping Founder 01 onto an empty, freezing parcel with zero shelter or leisure affordances caused an inescapable Day 1 hypothermia and burnout death spiral before a single room could be built. Version 6.0 resolves this by pairing **The Chill Loop** recovery ECS systems with the **Day 1 Genesis Survival Camp Kit** (`SCN-TAU-GENESIS`).

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                         DAY 1 GENESIS SURVIVAL CAMP KIT (LOT 402 SPAWN)                          │
├──────────────────────────────┬───────────────────────────────────────────────────────────────────┤
│ Pre-Spawned Asset            │ Thermodynamic & Gameplay Function at `Tick 0`                     │
├──────────────────────────────┼───────────────────────────────────────────────────────────────────┤
│ **1. 3×3 Canvas Bell Tent +  │ Pre-enclosed (`MAT_DOOR` flap + insulated canvas) starter shelter │
│    Woodstove + 12 Cordwood** │ with `[Sleep Zone]`; maintains $>18.0^\circ\text{C}$ for 72h.     │
├──────────────────────────────┼───────────────────────────────────────────────────────────────────┤
│ **2. 60 L Potable Water Tote**│ Provides 15–20 days of clean solo drinking water while keyline    │
│                              │ swales and biochar filtration beds are excavated.                 │
├──────────────────────────────┼───────────────────────────────────────────────────────────────────┤
│ **3. 12-Meal Ration Crate**  │ $24,000\text{ kcal}$ high-density dry rations preventing starvation│
│                              │ while cover crops and the Commons Kitchen are established.        │
├──────────────────────────────┼───────────────────────────────────────────────────────────────────┤
│ **4. Stone Ring Campfire**   │ Pre-designated outdoor `[Recreation Zone]` providing immediate    │
│    (`RECREATION_ZONE`)       │ **`-25%/hr` solo** (`-40%/hr` social) Stress drainage on Day 1.   │
├──────────────────────────────┼───────────────────────────────────────────────────────────────────┤
│ **5. Genesis Tool Locker**   │ Contains Mattock (`[Dig Swale]`), Trowel (`[Build Wall: Cob]`),   │
│                              │ Timber Saw, Inductive Line Coupler, and Ed25519 DID Handle.       │
└──────────────────────────────┴───────────────────────────────────────────────────────────────────┘
```

#### 6.3.2. Mathematical Specification of the Recovery ECS Systems

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                     PSYCHOLOGICAL & BIOLOGICAL HOMEOSTASIS ("THE CHILL LOOP")                    │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│   [THERMAL & WORK STRESSORS]                            [THE CHILL LOOP RECOVERY SYSTEMS]        │
│   - Ambient Cold (<10°C) / Heat (>32°C)                 - Enclosed Insulated Room (>18°C Sleep)  │
│   - Heavy Excavation / Asphalt Depaving                 - Ration Crate / Commons Kitchen + Water │
│   - Uninsulated Exposure & Hunger                       - Recreation Zones & Self-Sooth Fallback │
│                 │                                                       │                        │
│                 ▼                                                       ▼                        │
│   ┌───────────────────────────┐                           ┌───────────────────────────────────┐  │
│   │ +Caloric Burn Penalty     │◄─────── Counteracts ──────│ Halts Thermal Kcal Burn (0 kcal/h)│  │
│   │ +Cumulative Stress Drift  │         & Restores        │ +45 Focus/hr Insulated Sleep Regen│  │
│   │ -Short-Term Focus Drain   │                           │ -25%/hr Solo | -40%/hr Social     │  │
│   └─────────────┬─────────────┘                           │ -10%/hr AutonomousSelfSooth       │  │
│                 │                                         └───────────────────────────────────┘  │
│                 ▼  If Unmitigated (Stress reaches 100% / 100,000 units)                          │
│   ┌───────────────────────────────────────────────────────────────────────────────────────────┐  │
│   │                     INVOLUNTARY BURNOUT STATE (`STATE_BURNOUT`)                           │  │
│   │  - Emergency Warp Brake auto-downshifts `10x/100x -> 1x` immediately                      │  │
│   │  - Agent drops all active tools and aborts current physical `SpatialIntentPayload`        │  │
│   │  - Paint-only `[Designate Sleep/Recreation Zone]` remains 100% usable by player           │  │
│   │  - Clears only when Stress drains below hysteresis exit threshold (< 40% / 40,000 units)  │  │
│   └───────────────────────────────────────────────────────────────────────────────────────────┘  │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

1. **Dirty-Flag Cached `ThermalEnclosureSolver` & `MAT_DOOR` Semantics:**  
   * **Passable-Yet-Airtight `MAT_DOOR` Voxels:** Agents pathfind seamlessly through `MAT_DOOR` voxels (`cost = 1.0`), whereas the 3D flood-fill `ThermalEnclosureSolver` treats closed `MAT_DOOR` voxels as airtight insulated barriers ($R\text{-value} \ge 12$).
   * **Zero-Cost Dirty-Flag Caching:** Flood-fill enclosure evaluation runs **only** when a structural voxel (`MAT_COB`, `MAT_URBANITE_SLAB`, `MAT_DOOR`, `MAT_ROOF`) is added or removed (`chunk_topology_dirty = true`), caching the resulting `RoomEnclosureID` and leak coordinates so steady-state 30 Hz ticks incur **$0.00\text{ ms}$** flood-fill overhead.
   * **`F1–F4` Info-View Shader Overlays:** Pressing `F1` activates the **Thermal Envelope Shader Overlay**, rendering enclosed $>18.0^\circ\text{C}$ rooms in warm amber and highlighting unsealed roof/wall gap voxels in **pulsing icy-blue** so players can immediately spot and patch drafts (`F2` = SOR Electrical Bus; `F3` = Keyline Hydrology & Moisture; `F4` = Stress, Trust Rings $R_0 \to R_6$ & Designations).
2. **Insulated Sleep Recovery (`RoomHomeostasisSystem`):**  
   When an agent sleeps inside an enclosed room with interior ambient temperature $T_{\text{room}} > 18.0^\circ\text{C}$ ($291.15\text{ K}$):
   * **Thermal Caloric Penalty Halted:** $\dot{C}_{\text{thermal}} = 0.0\text{ kcal/hr}$ (metabolic burn locks to basal resting $\dot{C}_{\text{basal}} = 70\text{ kcal/hr}$).
   * **Canonical Focus Regeneration (`+45 Focus/hr`):**
     $$\frac{dF}{dt} = \begin{cases} +45.0\text{ Focus/hr}, & \text{if } T_{\text{room}} > 18.0^\circ\text{C} \text{ and Enclosed} \\ +12.0\text{ Focus/hr}, & \text{otherwise (Uninsulated / Cold Exposure)} \end{cases}$$
3. **Recreation Zones & Three-Tier Active Stress Drainage (`LeisureChillSystem`):**  
   Cumulative `Stress` ($\mathcal{S} \in [0, 100,000]$, where $100,000 = 100\%$) drains according to three canonical recovery tiers:
   $$\left(\frac{d\mathcal{S}}{dt}\right)_{\text{chill}} = \begin{cases} -40,000\text{ units/hr } (-40\%/\text{hr}), & \text{in } \text{RECREATION\_ZONE} \text{ with } N_{\text{peers}} \ge 1 \text{ (Social Hearth/Music)} \\ -25,000\text{ units/hr } (-25\%/\text{hr}), & \text{in } \text{RECREATION\_ZONE} \text{ solo (Campfire / Reading)} \\ -10,000\text{ units/hr } (-10\%/\text{hr}), & \text{via } \texttt{AutonomousSelfSooth} \text{ (In-Place Breathing/Sitting Fallback)} \end{cases}$$
   If no `RECREATION_ZONE` is reachable or designated, a stressed agent automatically invokes `AutonomousSelfSooth` in-place (`-10%/hr`), guaranteeing that the simulation can never permanently deadlock even if the campfire goes out.
4. **The 100% Stress Involuntary Burnout State (`STATE_BURNOUT`) & `< 40%` Hysteresis Exit:**  
   When cumulative `Stress` reaches $100\%$ ($\mathcal{S} \ge 100,000$), the agent enters `STATE_BURNOUT`:
   * The **Emergency Warp Auto-Downshift Brake** immediately drops Time Warp from `10x`/`100x` to `1x` (`30 Hz`).
   * The agent drops all held tools and aborts active physical labor.
   * **Paint-Only Designation Exemption:** While physical build/dig orders are rejected until recovery, zero-kcal paint-only designations (`[Designate Sleep/Recreation Zone]`, `[Cancel Designation]`) remain **$100\%$ usable by the player** so they can always designate a recovery zone during an active Burnout.
   * **Hysteresis Exit (`< 40%` Stress):** The agent exits `STATE_BURNOUT` and resumes normal task evaluation only after `Stress` drains strictly below **$40\%$ ($\mathcal{S} < 40,000$ units)**.

---

### 6.4. Pillar 4: Standardized Simulation Temporal Controls (`Pause / 1x / 2x / 5x / 10x`, Auto-Downshift Brake & `FlowContract` Harmonization)

#### 6.4.1. Playability Rationale
Binding simulation time 1:1 to the 60 FPS render loop makes multi-day ecological and thermodynamic processes—such as biochar pyrolysis, waiting for rainwater to fill a keyline swale, curing cob walls, or crop maturation—tedious, while extreme multipliers ($100\times$) create tick-rate instabilities and force artificial epoch walls. Oasis decouples the **`30 Hz` base simulation step** from the **locked `60 Hz` WebGPU render loop** via **Standardized Simulation Temporal Controls (`0x, 1x, 2x, 5x, 10x`)**.

#### 6.4.2. Standardized HUD Controls (`0x / 1x / 2x / 5x / 10x`), Live Net Trajectory Ticker & Auto-Downshift Brake
The top-right Cadastral HUD provides five standardized temporal state controls ($100\times$ is deprecated) paired with a live **Net Metabolic Trajectory Ticker**:
* **Live Net Trajectory HUD Ticker:** Continuously displays **`[ Net: +4.2 kWh/d | +18.0 L/d | -450 kcal/d ]`** (color-coded Emerald for surplus, Amber for <48h reserve, Crimson for <24h deficit) so the player knows their exact survival runway before accelerating time.
* **`[ || Pause ]` (`Space` / `0`):** $M_{\text{warp}} = 0\times$ ($0\text{ Hz}$ simulation, $60\text{ Hz}$ WebGPU rendering). Orbital Camera, `F1–F4` overlays, Spatial Context Designator, and the Temporal Workflow Scheduler remain $100\%$ interactive for planning.
* **`[ > 1x ]` (`Key 1`):** $M_{\text{warp}} = 1\times$. Canonical **`30 Hz` fixed simulation step** ($\Delta t_{\text{fixed}} = 33.333\text{ ms}$, $28,800\text{ logical ticks/day}$).
* **`[ >> 2x ]` (`Key 2`):** $M_{\text{warp}} = 2\times$ (**`60 Hz`** logical step rate; $2\times$ sub-stepping of `g_ecs->progress()` per frame).
* **`[ >>> 5x ]` (`Key 3`):** $M_{\text{warp}} = 5\times$ (**`150 Hz`** logical step rate; $5\times$ sub-stepping of `g_ecs->progress()` per frame).
* **`[ >>>> 10x ]` (`Key 4`):** $M_{\text{warp}} = 10\times$ (**`300 Hz`** logical step rate; $10\times$ sub-stepping of `g_ecs->progress()` per frame).

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│           STANDARDIZED 30Hz TIME-WARP ENGINE (`0x, 1x, 2x, 5x, 10x`) & FLOW HARMONIZATION        │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│  [WebGPU Render Loop: Locked 60 FPS (16.66 ms)]   [Flecs Simulation: 30 Hz Base * M_warp]        │
│  - Orbital Camera, Cutaway & DDA Raymarcher       - M_warp in { 0x, 1x, 2x, 5x, 10x(300Hz) }     │
│  - Right-Click Designator & Net Trajectory HUD    - Sub-steps `g_ecs->progress(33.33ms)`         │
│  - Emits 1 aggregated 32B `TelemetrySample`       - Emergency Auto-Downshift Brake -> `1x`       │
│    per 60 Hz frame boundary to UHAI ring          - Wall-clock Log Decimator (max 2 Hz stdout)   │
│                                                                  │                               │
│                                                                  ▼                               │
│                                       ┌──────────────────────────────────────────────────────┐   │
│                                       │      UHAI SPSC RING BUFFER & SPRINT 9 HARMONIZER     │   │
│                                       │  - Steps `TemporalFlowContractComponent::StepTick()` │   │
│                                       │    on every logical tick (28,800 ticks/day)          │   │
│                                       │  - Steps Asynchronous Node Proxies & Temporal        │   │
│                                       │    Workflow Schedule slots deterministically          │   │
│                                       └──────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

#### 6.4.3. Emergency Warp Auto-Downshift Brake & Frame-Boundary UHAI Aggregation
1. **Emergency Warp Auto-Downshift Brake (`2x/5x/10x -> 1x`):**  
   To prevent players from accidentally fast-forwarding into a lethal catastrophe at $2\times$, $5\times$, or $10\times$, the engine automatically disengages time-warp and drops immediately to **`1x` (`30 Hz`)** with a resonant acoustic warning chime whenever any of the following triggers fire:
   * **Trigger A (Migration Wave Arrival):** A `MigrationWaveArrivalEvent` crosses the parcel boundary.
   * **Trigger B (Burnout or Regression):** Founder 01 or any NPC enters `STATE_BURNOUT` ($100\%$ Stress) or a Legacy Behavioral Regression (`HOARDING`, `CONFLICT`, `MESH_DEGRADATION`).
   * **Trigger C (Critical Survival Reserve `< 24h`):** Potable water or caloric reserves fall below $24\text{ simulated hours}$ ($28,800\text{ ticks}$) of consumption at current metabolic burn rates.
2. **Frame-Boundary UHAI Telemetry Aggregation & Log Decimation:**  
   During $2\times$, $5\times$, and $10\times$ sub-stepping, `g_ecs->progress(dt_fixed)` advances deterministically without flooding `/dev/shm/uhai_telemetry.ring`. Instead, Oasis emits **exactly one aggregated 32-byte `TelemetrySample` per 60 Hz render frame boundary**, and throttles stdout logs via a $500\text{ ms}$ wall-clock token bucket.
3. **Sprint 9 Temporal `FlowContract` Harmonization:**  
   Each logical tick ($28,800\text{ ticks/day}$) invokes `TemporalFlowContractComponent::StepTick(current_tick, ...)`, integrating `rate_per_tick` across `FlowContract` slots so total conserved exergy $\sum \text{rate\_per\_tick} \times \Delta \text{ticks}$ is bit-exact across $1\times$, $2\times$, $5\times$, and $10\times$ warp speeds.

---

### 6.5. Pillar 5: Migration Waves & Legacy NPCs (Adjacency Routing & `R6 -> R3 -> R1` Pipeline)

#### 6.5.1. Playability Rationale & Strict Layer Adjacency (`L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> L1b`)
A solitary survivor loop plateaus once basic shelter and calories are secured. To drive mid- and late-game emergent drama, Layer 7 (`col-adversaryd` DES) periodically generates **Migration Waves** in response to macroscopic legacy crises.

In strict compliance with Sovereign Stack layer adjacency, `MigrationWaveArrivalEvent` **never** jumps directly from Layer 7 to Layer 1b; it traverses adjacent daemon sockets downward:
$$\text{L7 (`col-adversaryd`)} \longrightarrow \text{L6 (`col-commonsd`)} \longrightarrow \text{L5 (`col-kmsd` assigns `Ring R6` DID)} \longrightarrow \text{L4 (`col-execd`)} \longrightarrow \text{L3 (`col-storaged`)} \longrightarrow \text{L2 (`col-meshd`)} \longrightarrow \text{L1b (Oasis UHAI Ring)}$$

#### 6.5.2. Dramatic Arrival UX & Legacy Migrant Profile
1. **12-Hour Pre-Wave Horizon Warning (`-14,400 Ticks`):**  
   Exactly $12\text{ simulated hours}$ ($14,400\text{ ticks}$ at $30\text{ Hz}$) before a wave reaches the gate, a visual dust plume / signal flare appears on the parcel horizon outside the Fog of Legacy, accompanied by a HUD early-warning banner:
   > `"[HORIZON ALERT] Migration Wave detected: 2 Displaced Legacy Citizens approaching South Gate in 12h 00m. Prepare insulated beds (>18°C), water, and rations."`
2. **Arrival Auto-Downshift, Camera Snap & Migrant Dossier Card:**  
   At arrival (`Tick T_arrival`), the Emergency Warp Brake auto-downshifts simulation speed to `1x` (`30 Hz`), a one-click `[Snap Camera to Gate]` pill appears, and a **Migrant Arrival Card** presents each newcomer's psychological facets, skills, displacement trauma memory, high baseline `Stress` ($\mathcal{S}_{\text{init}} \in [45,000 \dots 65,000]$), entrenched Fiat Mindset score, and initial cryptographic assignment to **`Trust Ring R6 (Unverified Migrant)`**.
3. **4-Needs Assimilation Checklist UI:**  
   Selecting any migrant displays a live **4-Needs Assimilation Checklist** in their Cognitive Dossier:
   * `[ ] 1. Insulated Sleeping Quarters (> 18.0°C Enclosed)`
   * `[ ] 2. Clean Potable Water Access (>= 3.0 L/day)`
   * `[ ] 3. Nourished Caloric Security (>= 2,200 kcal/day)`
   * `[ ] 4. Recreation Zone Leisure Access (Stress < 40%)`

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│           MIGRATION WAVE ADJACENCY & TWO-STAGE PoTW + FROST ASSIMILATION (`SS-EP5-007`)          │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│  [L7 `col-adversaryd` DES] ──► L6 ──► L5 ──► L4 ──► L3 ──► L2 ──► [L1b Oasis UHAI Ring]          │
│  (12h Horizon Warning -> Arrival Auto-Downshift to 1x -> Migrant Arrival Card)                   │
│                                                │                                                 │
│                                                ▼                                                 │
│        ┌───────────────────────────────────────────────────────────────────────────────┐         │
│        │ STAGE 0: `Trust Ring R6 (Unverified Migrant)`                                 │         │
│        │ - Baseline Stress: 45%–65% | Fiat Mindset Active | Zero Grid/Vault Access     │         │
│        └───────────────────┬───────────────────────────────────────┬───────────────────┘         │
│                            │                                       │                             │
│     [4-Needs Checklist Met: >18°C Bed, Water,               [Infrastructure Overwhelmed:         │
│      Calories & Chill Loop + Sweat-Equity Labor]             Cold, Hungry, Unhoused Shock]       │
│                            │                                       │                             │
│                            ▼                                       ▼                             │
│        ┌───────────────────────────────────────┐   ┌───────────────────────────────────────┐     │
│        │ STAGE 1 (`R6 -> R3` Promotion):       │   │ LEGACY BEHAVIORAL REGRESSION PATH     │     │
│        │ **`Trust Ring R3 (Probationary)`**    │   │ - Stress cascades to 100% (Burnout)   │     │
│        │ - Requires L3 (`col-storaged`) proof: │   │ - `HOARDING`: Locks food/tools        │     │
│        │   >= 7 consecutive sheltered days AND │   │ - `CONFLICT`: Mess hall brawls (-45)  │     │
│        │   >= 36.0 MJ (10.0 Exergy Credits)    │   │ - `MESH_DEGRADATION`: Unauthorized    │     │
│        │   of verified PoTW sweat-equity work  │   │   grid draw / relay sabotage          │     │
│        └───────────────────┬───────────────────┘   └───────────────────────────────────────┘     │
│                            │                                                                     │
│                            ▼  >= 30 Simulated Days in R3 + Zero Active Ostrom Sanctions          │
│        ┌───────────────────────────────────────────────────────────────────────────────┐         │
│        │ STAGE 2 (`R3 -> R1` Promotion via `SS-EP5-007` & `SCN-TAU-GENESIS`):          │         │
│        │ **`Trust Ring R1 (Vouched Steward)`**                                         │         │
│        │ - Requires **2-of-3 FROST Threshold Multi-Signature Ceremony** initiated by   │         │
│        │   existing `Ring R1` stewards via Layer 6 (`POST /api/v1/l6/intent -> L5`)    │         │
│        │ - Unlocks full inverter, seed vault, and mutual-credit co-stewardship         │         │
│        └───────────────────────────────────────────────────────────────────────────────┘         │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

#### 6.5.3. Two-Stage PoTW + FROST Web-of-Trust Graduation Pipeline (`SS-EP5-007`)
Migrants **never** jump directly from unverified arrival to full infrastructure stewardship. Graduation follows the two-stage cryptographic protocol enforced jointly by Layer 3 (`col-storaged`) and Layer 5 (`col-kmsd`):
1. **Stage 1 (`Trust Ring R6 [Unverified Migrant] -> Trust Ring R3 [Probationary Resident]`):**  
   Requires Layer 3 (`col-storaged`) cryptographic attestation of:
   * $\ge 7$ consecutive simulated days ($201,600\text{ ticks}$) of sheltered homeostasis (all 4 Assimilation Checklist needs satisfied, zero regression events), **AND**
   * $\ge 36.0\text{ MJ}$ (**$10.0\text{ Exergy Credits}$**) of verified Proof-of-Thermodynamic-Work (PoTW) sweat-equity labor contributed to enclave designations (digging swales, hauling cob, tending crops).
2. **Stage 2 (`Trust Ring R3 [Probationary Resident] -> Trust Ring R1 [Vouched Steward]`):**  
   Requires:
   * $\ge 30$ simulated days ($864,000\text{ ticks}$, 1 full Epoch) in `Trust Ring R3` with **zero active Ostrom graduated sanctions**, **AND**
   * A **2-of-3 FROST Threshold Multi-Signature Vouching Ceremony (`SCN-TAU-GENESIS`)** initiated by existing `Ring R1` stewards via the Layer 6 Companion App (`POST /api/v1/l6/intent -> L5 col-kmsd`).

#### 6.5.4. C++20 Migrant Assimilation Component Layout
```cpp
namespace oasis::migration {

enum class TrustRingLevel : uint8_t {
    R0_FOUNDER_CORE         = 0, // Genesis Founder / airgapped root steward
    R1_VOUCHED_STEWARD      = 1, // 2-of-3 FROST vouched steward (full infra authority)
    R2_FEDERATED_ALLY       = 2, // Neighboring Arcadia (server-01) peer steward
    R3_PROBATIONARY_RESIDENT= 3, // >= 7d sheltered + >= 36.0 MJ (10 Exergy) PoTW verified
    R6_UNVERIFIED_MIGRANT   = 6  // Newly arrived Layer 7 DES migration wave NPC
};

enum class RegressionState : uint8_t {
    NONE                = 0,
    HOARDING            = 1, // Seizes and locks communal calories/tools
    CONFLICT            = 2, // Initiates arguments/brawls, spikes peer stress
    MESH_DEGRADATION    = 3  // Disrupts microgrid / mesh relays
};

struct alignas(32) MigrantAssimilationComponent {
    uint64_t arrival_tick;              // 8B: Simulation tick when migrant wave arrived
    float    potw_sweat_equity_mj;      // 4B: Verified PoTW work contributed (target >= 36.0 MJ)
    uint32_t sheltered_homeostasis_ticks;// 4B: Consecutive ticks with 4-Needs Checklist met (target >= 201,600)
    uint32_t r3_probationary_ticks;     // 4B: Ticks spent in Ring R3 without sanction (target >= 864,000)
    int16_t  fiat_mindset_index;        // 2B: [1000 = Pure Fiat Hoarder .. 0 = Mutualist Steward]
    uint16_t wave_id;                   // 2B: Originating Layer 7 DES migration wave ID
    TrustRingLevel trust_ring;          // 1B: Starts at R6_UNVERIFIED_MIGRANT (6) -> R3 (3) -> R1 (1)
    RegressionState regression;         // 1B: Active legacy regression behavior
    uint8_t  four_needs_checklist_mask; // 1B: Bit 0: Bed(>18C), Bit 1: Water, Bit 2: Kcal, Bit 3: Leisure
    uint8_t  frost_vouch_signatures;    // 1B: Count of Ring R1 FROST signatures collected (target >= 2 of 3)
    uint8_t  active_ostrom_sanctions;   // 1B: Must be 0 for R3 -> R1 promotion
    uint8_t  padding[3];                // 3B: Alignment to exactly 32 bytes
};
static_assert(sizeof(MigrantAssimilationComponent) == 32, "MigrantAssimilationComponent must be 32 bytes.");

} // namespace oasis::migration
```

---

### 6.6. Pillar 6: The Asynchronous Node Proxy Architecture (`Arcadia server-01`), BPMN-Driven Gameplay Loops & Temporal Workflow Scheduling

#### 6.6.1. Playability Rationale & The Asynchronous Node Proxy Shift
Real-time, fully synchronized lockstep multiplayer (and hard-wall epoch barriers that freeze the simulation) is **deprecated**. Oasis utilizes a **semi-stochastic single-player architecture** with **Asynchronous Node Proxies**:
* When a player connects to `Arcadia (server-01)`, they operate in their own localized instance (`Node 0 — Lot 402`), while every other human player on the server is represented in their world as an **Autonomous NPC Node Proxy** (`Node 01 — Lot 404 Maya`, `Node 02 — Lot 406 Kael`, `Node 03 — Lot 408 Soren`).
* The configuration parameters, production outputs, and thermodynamic states of these NPC Node Proxies update dynamically based on what real players are doing in their respective instances—providing a deeply populated, consistent world driven by real human exergy without real-time latency, collision-resolution nightmares, or forced epoch-freeze walls.
* **Storage Horizon (Phase 1 Legacy Bridge $\to$ Phase 2 Layer 3 CRDT Mesh):**
  * **Phase 1 (Current Implementation):** Leverages GitHub and static JSON snapshot feeds (`core/3_ledger/node_proxies.json`) to synchronize peer Node Proxy configurations, thermodynamic production rates, and resource deficits across instances.
  * **Phase 2 (Horizon):** Transitions seamlessly to **Layer 3 (`core/3_ledger/LEDGER_SPEC.md`, `col-storaged`)** local-first CRDT distributed storage (`StateDeltaOp`, `CRDTEscrowLock`, and `mesh:ValueToken` 1:1 surplus exergy reflection) replicated over Layer 2 (`col-meshd`).

#### 6.6.2. Dual Control & Strict Layer Role Separation: Layer 6 Day-to-Day Citizen Intent Calendar vs. Layer 4 Workflow Designer & Layer 5 Governance Manager
People (and simulated agents commanded by the player in the game) use **Layer 6 (`core/6_intent/INTENT_SPEC.md`) exclusively** in their day-to-day usage. Layer 4 and Layer 5 are not day-to-day task lists; they are the underlying workflow orchestration and governance systems that citizens configure to support their Layer 6 usage:

1. **In-Game & Citizen Companion Day-to-Day Interface — Layer 6 Intent Slate & Calendar of Intents (`shell_minimal.html` & `core/1_simulation/src/web/companion/`):**  
   In-game (`[ 📱 L6 INTENTS & CALENDAR ]`) and in the Layer 6 Citizen Companion App (`#deck-l6`), the player interacts strictly with **Layer 6**. Peer Node Proxy resource deficits appear as **Layer 6 Collective Intent Bounties** (`mesh:FabricationIntent`, `mesh:RestIntent`, `mesh:MediationIntent`, `mesh:TriageIntent`). Players plan ahead using the **Layer 6 Calendar of Intents (Days 1–4, Morning & Afternoon Slots)**, observing each scheduled intent progress through canonical Layer 6 lifecycle states (`[PENDING]` $\to$ `[ESCROWED]` $\to$ `[ACTUATING]` $\to$ `[FULFILLED]` / `[REJECTED]`).
2. **Layer 4 Companion App — BPMN 2.0 Workflow Designer, Filterable Database & Aggregated Diagnostics (`core/4_orchestrator/web/index.html`):**  
   A dedicated **BPMN 2.0 Workflow Designer** that provides: (a) access to the existing `.bpmn.json` workflow database (`core/4_orchestrator/workflows/workflow_database.json`) with **applicable filtering** (by Layer 6 Intent Class, Status/Origin, and Scenario/Search), (b) a **Workflow Specialization Studio** for customizing templates (e.g., tuning `L2/L1b Physics Check` thresholds, `L3 Escrow` parameters, and fallback routes) and saving specialized workflows back to the database, and (c) **aggregated Layer 4 diagnostic telemetry** (intents ingested, `JobQueue` mempool depth, stage latencies, and gateway pass/rejection rates).
3. **Layer 5 Companion App — Governance Systems Manager & L5$\leftrightarrow$L4 Workflow Interface Binder (`core/5_governance/web/index.html`):**  
   A dedicated **Governance Systems Manager** for configuring W3C DID Web-of-Trust edges (`mesh:TrustGraphEdge` with weights in `[-1.0, 1.0]`), Dijkstra hop-distance caps, `mesh:SkillCredential` VCs, and `mesh:SafeHarborSeverance` ZK blind spots (`core/5_governance/trust_gates.json`), and **establishing the associated Layer 4 workflows (`mesh:BPMNProcess`) that each governance system uses as its L4 execution interface**.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│     LAYER SEPARATION: L6 CITIZEN INTENT CALENDAR + L5 GOVERNANCE MANAGER + L4 WORKFLOW DESIGNER  │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│  [Layer 4 Workflow Designer (`core/4_orchestrator/web/`)]                                        │
│  - Filterable `.bpmn.json` Workflow Database (`workflow_database.json`)                          │
│  - Template Specialization Studio + Aggregated L4 Diagnostics (Mempool, Latencies, Pass Rates)   │
│                         ▲                                                                        │
│                         │ L5 <-> L4 Workflow Interface Binding (`mesh:BPMNProcess`)              │
│                         ▼                                                                        │
│  [Layer 5 Governance Manager (`core/5_governance/web/`)]                                         │
│  - Manages W3C DID `mesh:TrustGraphEdge`, Dijkstra Hop Caps, `SkillCredential` & `SafeHarbor`    │
│  - Establishes which Layer 4 `.bpmn.json` Workflow each Governance Rule invokes                  │
│                         ▲                                                                        │
│                         │ Supports Day-to-Day Citizen Intents (`PENDING -> ESCROWED -> FULFILLED`)│
│                         ▼                                                                        │
│  [In-Game & Citizen Companion: Layer 6 Intent Slate & Calendar (`core/6_intent/`)]               │
│  - Exclusive Day-to-Day Citizen UI (`[ 📱 L6 INTENTS & CALENDAR ]` in HUD & Companion `#deck-l6`)│
│  - Emits `mesh:FabricationIntent` / `mesh:RestIntent` & schedules Days 1..4 Calendar of Intents  │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

#### 6.6.3. C++20 `AsyncNodeProxy`, `CollectiveBpmnBounty` & `TemporalScheduleState` (`core/bpmn_scheduler.hpp`)
```cpp
namespace oasis::orchestrator {

struct AsyncNodeProxy {
    uint32_t node_id{1};
    const char* callsign{"Node 01 (Lot 404 — Maya)"};
    const char* did_uri{"did:mesh:node01:maya"};
    uint8_t trust_ring{2};               // R2 Federated Neighbor
    double net_kwh_per_day{1.8};         // Positive = surplus, Negative = deficit
    double net_water_l_per_day{-12.0};   // Negative triggers BPMN Hydrological Bounty
    double net_kcal_per_day{200.0};      // Negative triggers BPMN Scenario Gamma Bounty
    uint16_t active_bounty_action_id{10};// Mapped to DesignatorActionId (10=Cistern, 11=Solar, 14=Cook)
    bool bounty_fulfilled{false};
};

struct ScheduledWorkflowSlot {
    uint32_t day_index{1};               // Day 1..5
    uint8_t  slot_of_day{0};             // 0 = Morning (06:00-14:00), 1 = Afternoon (14:00-22:00)
    uint16_t planned_action_id{0};       // DesignatorActionId (0 = Unscheduled, 1=Swale, 11=Solar, 13=Harvest, 14=Cook)
    bool     l5_gate_locked{false};      // True once Layer 5 Trust Gate + L3 Escrow Lock are reserved in advance
    bool     dispatched_to_sim{false};   // True once simulation clock reaches (day_index, slot_of_day)
    bool     completed{false};
};

} // namespace oasis::orchestrator
```

---

## 7. Core Gameplay Loops & Systems Progression

### 7.1. The Daily Diurnal Simulation Rhythm (`28,800 Ticks/Day` at `30 Hz`)
```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                         THE DAILY SIMULATION RHYTHM (28,800 TICKS @ 30 Hz)                       │
├─────────────────────┬────────────────────────────────────────────────────────────────────────────┤
│ Time Window         │ Gameplay Phase & Systems Activity                                          │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **06:00 — Dawn**    │ **The Morning Circle:** Bell rings; Founder 01 convenes community. Open    │
│                     │ BPMN WorkTokens & Spatial Designations are posted; citizens claim tasks.   │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **07:00 — 12:00**   │ **Morning Production Sprint:** High focus period. Heavy physical labor     │
│                     │ (swale digging dropping cob/urbanite, wall/roof building, solar mounting). │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **12:00 — Midday**  │ **Communal Nourishment:** Shared lunch from Ration Crate / Commons Kitchen;│
│                     │ potable water hydration; `Ring R6 -> R3` migrant assimilation check-ins.   │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **13:00 — 17:00**   │ **Afternoon Maintenance & Craft:** Precision crafting (rocket mass heater  │
│                     │ masonry, inverter wiring, cover crop seeding, hauling to stockpiles).      │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **17:00 — 20:00**   │ **Evening Chill Loop & Governance:** Recreation Zone leisure (`-25%/hr`    │
│                     │ solo, `-40%/hr` social); L6 Companion App FROST vouching (`:8443 VLAN 30`).│
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **20:00 — 06:00**   │ **Insulated Night Sleep & Memory Consolidation:** Sleeping in $>18.0^\circ\text{C}$│
│                     │ enclosed rooms halts thermal burn ($0\text{ kcal/hr}$) & restores `+45 Focus/hr`.│
└─────────────────────┴────────────────────────────────────────────────────────────────────────────┘
```

### 7.2. Seasonal Dynamics, Migration Waves & `864,000`-Tick Epoch Milestones
* **Spring (The Thaw):** Bootstrapping from the **Day 1 Genesis Survival Camp Kit**; digging keyline swales to harvest clay/cob and rainwater; first 12h Pre-Wave Horizon Warning announces `Trust Ring R6` migrants.
* **Summer (The Heat & Drought):** Solar exergy at maximum ($> 8.0\text{ kWh/m}^2/\text{day}$); high evaporation demands water conservation; graduating `R6` migrants to `R3 (Probationary Resident)` via $\ge 7\text{d}$ shelter + $\ge 36.0\text{ MJ}$ PoTW.
* **Autumn (The Harvest & Enclosure):** Using `F1` Thermal Overlay to patch pulsing icy-blue thermal leaks before frost; completing 2-of-3 FROST ceremonies (`SCN-TAU-GENESIS`) to promote `R3` residents into `Ring R1 (Vouched Stewards)`.
* **Winter (The Deep Freeze & Survival):** Low solar yield; rocket mass heaters keep cob dormitories $> 18.0^\circ\text{C}$ (`+45 Focus/hr`); hearth Recreation Zones (`-40%/hr` social stress drain) prevent $100\%$ Stress `STATE_BURNOUT`, while `864,000`-tick Mandatory Milestone Epoch syncs reconcile `Arcadia (server-01)` via Layer 3 BCC and Layer 5 FROST commits.

---

## 8. HUD, UI/UX Design & Sensory Telemetry

### 8.1. Dual Viewport, Orbital Cutaway & Spatial Designator System
Oasis features a seamless camera and spatial interaction architecture:
* **Dynamic POI Orbital / Arcball View (`Shift + Right Click`):** Locks onto any clicked voxel with a **3D Wireframe POI Reticle**, orbits smoothly in clamped spherical coordinates $(\theta, \phi \in [5^\circ, 85^\circ])$, scales radial zoom $r$ via `Shift + Mouse Wheel`, and automatically slices roofs/walls via `z_cutaway_level` when indoors or $r < 12.0\text{ m}$.
* **Tactile Spatial Context Menus (The Designator):** `Right-Click` evaluates any voxel and opens its contextual menu with upfront `kcal`/`Focus`/material badges (including `[Build Wall: Cob / Urbanite]`, `[Install Door: Insulated Timber]`, `[Build Roof]`, `[Build Hearth: Rocket Mass Heater]`, `[Cancel Designation]`); `Left-Click` renders Cyan (valid) / Crimson (invalid) drag previews and dispatches a 264-byte `SpatialIntentPayload`.
* **`F1–F4` Info-View Shader Overlays:** `F1` Thermal Envelope (pulsing icy-blue leak highlights & warm amber $>18.0^\circ\text{C}$ rooms), `F2` SOR Electrical Bus, `F3` Keyline Hydrology, `F4` Stress & Trust Rings ($R_0 \to R_6$).
* **Cadastral Temporal Bar & Live Net Trajectory Ticker:** Displays `[ || Pause ] [ > 1x (30Hz) ] [ >> 10x ] [ >>> 100x ]`, the **Emergency Warp Auto-Downshift Brake** indicator, the live **`[ ±kWh/d | ±L/d | ±kcal/d ]`** Net Trajectory Ticker, and the `864,000`-tick Epoch Milestone progress bar.
* **Companion App Surface Separation (V2.0 Law):** All societal/Agora workflows live exclusively on the Layer 6 Mobile Companion App (`col-gatewayd` on **`TLS 1.3 HTTPS/WSS :8443, VLAN 30`**), keeping the Oasis desktop HUD clean, spatial, and diegetic.

### 8.2. The Volumetric Plumbob 2.0
Hovering over Founder 01’s head is a 3D volumetric faceted gemstone—**The Plumbob 2.0**:
* **Calm Emerald Pulse (0.5 Hz):** High focus ($\ge 180$), low stress ($< 20,000$), thermal homeostasis satisfied ($>18.0^\circ\text{C}$ rest).
* **Warm Amber Flicker (1.5 Hz):** Strained focus ($50 \dots 100$), moderate stress, thermal caloric penalty active.
* **Jagged Crimson Strobing (4.0 Hz):** Acute breakdown, $100\%$ Stress **Involuntary Burnout** (`STATE_BURNOUT`, clears at $< 40\%$ Stress), legacy regression, or moral defiance.
* **Prismatic Iridescent Shimmer:** Strange Mood / Creative Burst in progress.

### 8.3. Cognitive Dossier, 4-Needs Assimilation Checklist & Narrative Stream
Selecting any character opens their **Cognitive Dossier**, including the **4-Needs Assimilation Checklist** and `R6 -> R3 -> R1` PoTW/FROST tracker for migrants:
> *"Founder 01 feels deeply rested (+45 Focus/hr) after sleeping in a warm cob room above 20°C. She felt quiet pride while digging the north keyline swale yesterday, and her stress melted away (-40%/hr) while sharing stories by the campfire. She is reviewing a 2-of-3 FROST vouching ceremony on the Companion App to promote Marcus from Trust Ring R3 to Ring R1 Steward."*

### 8.4. Volumetric Fog of Legacy, Pre-Wave Horizon Flares & 3D Epoch Wave Reveal
Outside the player’s active parcel boundary, the world is shrouded in the **Fog of Legacy**. Twelve simulated hours before a Migration Wave arrives, a **Pre-Wave Horizon Flare** illuminates the boundary approach. At the end of each `864,000`-tick month, lifting the Mandatory Milestone barrier triggers a **3D Holographic Horizon Wave** that sweeps across `Arcadia (server-01)`, revealing neighboring peers' newly reconciled structures and expanding verdant **Commons Corridors**.

---

## 9. The Six Inviolable Quality Gates (G1–G6)

To prevent simulation drift and guarantee pedagogical and safety validity, every scenario and Horizon Playability Pillar in Oasis must satisfy Six Inviolable Quality Gates:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                               THE SIX INVIOLABLE QUALITY GATES (G1–G6)                           │
├───────┬───────────────────────────────┬──────────────────────────────────────────────────────────┤
│ Gate  │ Domain                        │ Acceptance Criteria & Validation Method                  │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G1**│ Thermodynamic, Exergy &       │ Energy and mass balances obey conservation laws across   │
│       │ `864,000`-Tick Epoch Gate     │ 30 Hz (1x–100x) Time Warp and Layer 3 BCC / Demurrage    │
│       │                               │ Epoch Syncs. SOR solver error < 1e-4. Zero free exergy.  │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G2**│ Offline Local-First (`SOLO_   │ `SOLO_AUTARKIC` Zero-Peer Fast-Path reconciles in <= 1   │
│       │ AUTARKIC`) & Arcadia CRDT Gate│ tick offline. Voluntary `Arcadia (server-01)` MMO syncs  │
│       │                               │ 24B `StateDeltaOp` CRDTs with `SS-EP9-003` Grace Window. │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G3**│ Byzantine, Sybil & `R6->R3->R1`│ Migration waves traverse `L7->L6->L5->L4->L3->L2->L1b`. │
│       │ PoTW + FROST Assimilation Gate│ Migrants start in `Ring R6`; `R6->R3` requires 7d + 36MJ │
│       │                               │ PoTW; `R3->R1` requires 30d + 2-of-3 L5 FROST ceremony.  │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G4**│ Psychological Homeostasis &   │ >18.0°C enclosed sleep halts thermal burn & restores     │
│       │ Chill Loop Anti-Burnout Gate  │ `+45 Focus/hr`. Leisure drains `-25%/-40%/-10%/hr` Stress│
│       │                               │ 100% Burnout auto-downshifts to 1x & exits at `< 40%`.   │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G5**│ Cooperative Bioregional       │ Day 1 Genesis Camp Kit prevents cold-start deadlock.     │
│       │ Onboarding & Designator Gate  │ Spatial Context Designator emits canonical 264-byte      │
│       │                               │ `SpatialIntentPayload` & supports full room enclosure.   │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G6**│ Outward-Only `0x01` Airgap &  │ Oasis emits 32B `TelemetrySample (0x01)` outward only &  │
│       │ V2.0 `:8443 VLAN 30` Gate     │ never ingests live telemetry. Companion App uses TLS 1.3 │
│       │                               │ `:8443 (VLAN 30)` & `L6->L5->L4->L3->L2->L1` / `[YIELD]`.│
└───────┴───────────────────────────────┴──────────────────────────────────────────────────────────┘
```

---

## 10. Audio & Narrative Themes

### 10.1. Acoustic Landscape & Somatic Cues
* **The Living Hearth vs. The Stressed Metropolis:** Inside Lot 402, soundscapes feature the gentle bubbling of rainwater swales, the crackle of seasoned oak in insulated rocket heaters, acoustic guitars in Recreation Zones, and rhythmic hand sawing. Outside the perimeter hums the industrial drone of 60 Hz high-voltage transformers, highway traffic, and distant sirens modeled by Layer 7 DES.
* **Somatic & Temporal Audio Feedback:** Cold, uninsulated characters shiver audibly; the **Emergency Warp Auto-Downshift Brake** sounds a warm brass singing-bowl chime when dropping from $100\times$ to $1\times$; agents entering $100\%$ Stress Burnout let out an exhausted sigh as their tools clatter to the ground; harmonious community meals and evening fire-pit sessions produce warm, restorative vocal murmurs.

### 10.2. Thematic Narrative Tone
Oasis is serious, cooperative, grounded, and defiantly constructive. In full alignment with `docs/BOOK_TEAM_DRIFT_REPORT.md`, it rejects malicious or "parasitic" framing in favor of structural resilience, good-faith modeling, and *willing voluntariness*. It honors the dignity of physical labor, the necessity of psychological rest and warmth ("The Chill Loop"), the compassionate assimilation of displaced neighbors (`R6 -> R3 -> R1`), and the transformative power of organized mutual aid.

---

## 11. Conclusion & Handoff Directives

With the unanimous Round 2 ratification of `OASIS-GDD-V6.0 (The Horizon & Playability Edition)`, all five Council domains are aligned without veto:
1. **The Six Horizon Playability Pillars (`HORIZON_ROADMAP.md`):** Orbital/Arcball POI Camera with 3D Wireframe Reticle & Roof Cutaway, Tactile Spatial Context Designator (emitting 264-byte `SpatialIntentPayload` with full wall/door/roof/hearth enclosure actions), Psychological & Biological Homeostasis (Day 1 Genesis Camp Kit, `+45 Focus/hr` insulated $>18.0^\circ\text{C}$ sleep, `-25%/-40%/-10%/hr` Stress drainage, and $100\% \to <40\%$ Burnout hysteresis), `30 Hz` Simulation Temporal Controls ($1\times\text{–}100\times$ Time Warp with Emergency Auto-Downshift Brake and Live Net Trajectory Ticker), Legacy NPC Migration Waves (`L7->...->L1b` adjacency and `R6 -> R3 -> R1` PoTW + 2-of-3 FROST graduation), and the Shared Virtual Landscape (`Arcadia server-01`) governed by `864,000`-tick Mandatory Milestone Epoch Synchronization (`SS-EP9-003` Straggler Grace Window, Dormant Epoch Escrow, and `SOLO_AUTARKIC` $\le 1\text{-tick}$ fast-path).
2. **Narrative & Safety Demarcation (`docs/BOOK_TEAM_DRIFT_REPORT.md`):** Cooperative bioregional onboarding terminology, strict outward-only 32-byte `TelemetrySample` (`QUALITY_SIMULATED = 0x01`) production, zero ingestion of live physical telemetry, and unmistakable demarcation between the Shadow Simulator (Layer 1b) and physical orchestration.
3. **The V2.0 Companion App Interaction Boundary Law:** Layer 1 (Oasis Desktop) as the exclusive surface for 3D spatial actuation, and Layer 6 (Mobile Companion App via `col-gatewayd` on **`TLS 1.3 HTTPS/WSS :8443, VLAN 30`**) as the exclusive Agora/societal interface with read-only diagnostic visibility into Layers 2–5 and 7 following the canonical `L6 -> L5 -> L4 -> L3 -> L2 -> L1 (UHAI)` downward execution and `[YIELD] -> L5 -> L6 -> L7` upward governance trajectory.

