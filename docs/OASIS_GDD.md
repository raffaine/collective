# OASIS GAME DESIGN DOCUMENT (GDD)
**Document Identifier:** `OASIS-GDD-V5.0`  
**Milestone:** Strategic Realignment Council Ratification (Scenario Driven Development Realignment)  
**Classification:** Authoritative Game Design Specification & Digital Twin Blueprint  
**Engine Baseline:** C++20 (`-std=c++20`) / WebGPU (WGSL) / Flecs ECS / UHAI IPC  
**Status:** Council Approved (Unanimous Consensus Reached)  
**Date of Enactment:** 2026-10-10  

---

## Strategic Realignment Council Ratification & Roll

This document embodies the synthesized, legally and architecturally binding consensus of the Strategic Realignment Council. Convened following the ratification of `docs/COLLECTIVE_GUIDING_PRINCIPLES.md`, the Council's mandate is to establish **Scenario Driven Development (SDD)** as the primary cybernetic engine driving emergent gameplay, dynamic scenario ingestion, and physical-world protocol compilation within the Oasis Simulation Layer.

### Council Roll & Formal Ratification Statements

1. **Game Designer (Council Lead):**  
   > *"Modern video games have become extractive cognitive pacifiers: Skinner boxes with colored motive bars, god-mode cursors, and sanitized corporate fantasy worlds. Oasis stands as the Trojan Horse Simulator. By fusing the historical psychological trauma of Dwarf Fortress, the empathetic, indirect agency of The Sims, and the macroscopic infrastructure siphoning of Cities: Skylines onto a local-first WebGPU 3D voxel engine, we build a game where every ludic mechanic compiles into real-world survival competence. Scenario Driven Development (SDD) is our breakthrough: rather than hardcoding static quests or bloated branching scripts, the 24 canonical MVS scenarios ingest dynamically as Flecs ECS components, spatial voxel tags, and Gibsonian affordance trees. I ratify this document unconditionally as our foundational ludic constitution."*

2. **Gamer Expert (Dwarf Fortress / The Sims / Cities: Skylines Specialist):**  
   > *"Gamers possess zero tolerance for fake mechanics. If an agent does not remember their frostbitten toes from a winter power outage, if an intention queue behaves like an obedient RTS puppet instead of a stressed human with moral boundaries, or if an electrical grid does not obey real impedance and phase balance, the simulation fails. In this GDD, we have achieved true fidelity: 64-byte bounded Dwarf Fortress psychology with Lyapunov stress homeostasis and permanent trauma retention; Sims-style indirect intention queues with logistic moral rejection curves; and Cities: Skylines macro-infrastructure leeching solved via Successive Over-Relaxation (SOR). By grounding all 24 scenarios across the 5 Developmental Acts into these three pillars, players will experience authentic emergent drama. I ratify this document with utmost enthusiasm."*

3. **Sovereign Stack Architect:**  
   > *"The boundary between simulation and bare-metal reality must remain sacred. The Five Anti-Bleed Invariants are maintained without compromise: Oasis contains zero embedded daemons, zero in-engine private keys, zero custom P2P networking, zero BPMN XML parsers, and zero fiat ledgers. Through the Universal Human-Agent Interface (UHAI) lock-free shared memory ring buffer, Oasis serves as Stafford Beer's System 4 Intelligence Vanguard—a Software-In-The-Loop (SITL) digital twin that stress-tests edge protocols in silico before physical bringup. SDD bridges the dual architecture perfectly: a single markdown scenario compiles downwards to game voxels and upwards to POSIX daemons. I ratify this specification unconditionally."*

4. **Tech Engineer:**  
   > *"Our technical commitments are absolute. The Oasis engine operates within a strict 256.00 MB WASM linear memory partition with zero heap allocations on the simulation tick. Rendering is executed by an Amanatides & Woo 3D DDA compute raymarcher in WebGPU WGSL, accelerated by a two-level hierarchical macro-chunk occupancy grid that guarantees 60 FPS well within our 10.60 ms frame budget. The SOR electrical flow solver converges in under 20 iterations with virtual earth grounding, and the UHAI data plane delivers sub-50 µs IPC latency over cache-aligned SPSC ring buffers in `/dev/shm`. Every gameplay system described herein is implementable, testable, and mathematically bounded. I ratify this specification."*

5. **Quality Engineer (Automation & Gatekeeper):**  
   > *"An unverified design is merely wishful thinking. This GDD establishes Six Inviolable Quality Gates (G1 through G6) spanning thermodynamic conservation, offline local-first resilience, Byzantine and Sybil immunity, psychological anti-livelock equilibrium, pedagogical Trojan Horse equivalence, and hardware-in-the-loop isolation. All 24 canonical scenarios must pass automated headless simulation harnesses before feature completion is declared. Cowboy coding and manual testing are prohibited. I ratify this specification."*

---

### Unanimous Consensus Declaration

```
════════════════════════════════════════════════════════════════════════════════════════
                        STRATEGIC REALIGNMENT COUNCIL
                               RATIFICATION LOG
════════════════════════════════════════════════════════════════════════════════════════
[VOTE] Game Designer (Lead)            : AYE (Unconditional)
[VOTE] Gamer Expert (DF/Sims/Cities)   : AYE (Unconditional)
[VOTE] Sovereign Stack Architect       : AYE (Unconditional)
[VOTE] Tech Engineer                   : AYE (Unconditional)
[VOTE] Quality Engineer                : AYE (Unconditional)

STATUS: Unanimous Consensus Reached
DATE: 2026-10-10T23:30:00Z
ACTION: ENACTED AS CANONICAL GAME DESIGN SPECIFICATION (V5.0)
════════════════════════════════════════════════════════════════════════════════════════
```

---

## 1. Executive Vision & Foundational Philosophy

### 1.1. The Trojan Horse Simulator
Oasis is not an escapist digital distraction, an idle clicker, or an idealized greenfield sandbox. It is the **Trojan Horse Simulator** for physical-world sovereign resilience.

Modern game design reinforces comfortable, detached fallacies that disarm the human intellect:
* **The Sims Fallacy (Mechanical Puppetry):** Human consciousness is reduced to six colored motive bars. Grief, existential terror, and community betrayal are instantly remedied by thirty seconds of microwave pizza or a television program. Characters have no historical memory, no lived trauma, and no ethical conviction.
* **The Cities: Skylines Fallacy (The Magic Greenfield):** Settlements sprout ex nihilo upon pristine, virgin greenfields. Infinite electrical power and pure potable water flow from the edges of the map in exchange for abstract fiat currency, supported by a benevolent, invisible municipal state.
* **The Colony Sim Fallacy (The Authoritarian God-Cursor):** In titles like *RimWorld* or *Banished*, the player is an omnipotent floating despot. A single click forces an exhausted, freezing mortal pawn to march into lethal hypothermia, industrial radiation, or suicidal combat without friction, moral hesitation, or psychological collapse.

Oasis rejects every one of these fallacies. The real Anthropocene offers no pristine greenfield, no benevolent municipal utility, and no obedient digital pawns. Humanity exists within the crumbling, predatory infrastructure of late-stage industrial civilization: cracked suburban asphalt, failing 12kV grid lines, lead-corroded water mains, arbitrary municipal code enforcement, and surging inflationary fiat bills.

Oasis simulates the authentic, gritty, and exhilarating physics of **emancipatory sovereignty**:
1. **Parasitic Scavenging:** You do not begin with an off-grid permaculture haven; you start as an economically vulnerable enclave **leeching parasitically** from decaying legacy infrastructure.
2. **Embodied Indirect Stewardship:** You do not command obedient pawns; you guide **Founder 01**, a mortal human steward with circadian fatigue, ethical convictions, and an emergent psychological inner life.
3. **Progressive Severance & Mesh Federation:** You do not win through imperial map painting; you systematically **sever the legacy umbilical cord**, regenerate depleted soil, form local Web-of-Trust mutual aid circles, and mesh with physical peers to dissolve the hostile legacy frontier.

Every gameplay system in Oasis acts as a **cybernetic compiler**: player mastery over thermal mass, rocket mass heaters, photovoltaic charge controllers, biochar filtration, and horizontal governance mirrors the exact engineering blueprints required to construct autonomous physical nodes in the physical world.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 THE FOUR HERITAGE PILLARS OF OASIS                               │
├────────────────────────────────┬────────────────────────────────┬────────────────────────────────┤
│ D1: Dwarf Fortress Psychology  │ D2: The Sims Indirect Founder  │ D3: Macro Legacy Leeching      │
│ - 64-byte bounded ECS state    │ - Mortal Founder 01 embodiment │ - 12kV grid siphoning & clamps │
│ - 32-slot episodic memory ring │ - Advisory intention queue     │ - Ductile iron water tapping   │
│ - Big-Five facets & 8 Values   │ - Spontaneous whims & desires  │ - Urbanite & rebar quarrying   │
│ - Lyapunov stress homeostasis  │ - Logistic moral rejection     │ - Suburban, Urban & Eco biomes │
│ - Breakdowns & Strange Moods   │ - Trust Ring social delegation │ - Parasitic -> Sovereign arc   │
├────────────────────────────────┴────────────────────────────────┴────────────────────────────────┤
│ D4: Sovereign Stack Multiplayer & Stochastic Boundary Interface                                  │
│ - Local-first deterministic simulation (zero centralized cloud servers)                          │
│ - Stochastic Boundary Interface: Inhomogeneous Poisson process for legacy pressure shocks       │
│ - 3 Attenuation Pillars: Physical autarky, social NPC integration, cryptographic mesh proximity  │
│ - Multiplayer executed EXCLUSIVELY via Sovereign Stack (Layers 2–7) over UHAI IPC               │
│ - Cryptographic speed-of-light geo-proximity proofs & shared boundary perimeter consolidation     │
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
│         │ Environmental Exploration     │ Shadow Simulator, Trojan Horse Onboarding,             │
│         │ Future Shock Rehearsal        │ SITL Stress-Testing, Dynamic Scenario Ingestion Lab    │
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

Oasis provides the high-variety exploratory sandbox where catastrophic failure costs zero human lives and zero physical kilowatt-hours. In Oasis, communities stress-test microgrid black-starts, childcare co-op schedules, and mutual-credit clearing networks at $10\times$ to $1000\times$ real-time clock speeds. When a governance or engineering policy proves stable in Oasis, it compiles upwards into production daemon configurations for real-world deployment.

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
* **Test Gates:** **G1 (Thermo):** Extrusion energy balances within $\pm 10\%$ of thermodynamic melt enthalpy. **G5 (Trojan):** Stripe-funded filament purchase compiles to local mesh bounty.

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
* **Ludic Mechanics:** Establishing Web-of-Trust rings (Ring 0 Core, Ring 1 Stewards, Ring 2 Guests); vouching for new members with personal reputation; defending against Sybil attacks.
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
* **Trust Rings:** Founder 01 delegates critical infrastructure (inverters, seed vaults) only to citizens in **Ring 1**. Commanding a **Ring 2** newcomer without trust causes friction and token refusal.

---

### 4.3. Directive 3: Cities: Skylines Macro Infrastructure Leeching

#### 4.3.1. The Omnipresent Legacy Reality
Traditional city builders isolate the player within an empty paradise where utilities flow infinitely from map borders. In Oasis, the player exists within the decaying, predatory reality of late-stage industrial infrastructure. 

Our core macroscopic gameplay loop is **"Leeching the Legacy"**: you survive by parasitically tapping, siphoning, intercepting, and scavenging the dying infrastructure of the old world.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 THE LEGACY INFRASTRUCTURE MATRIX                                 │
├─────────────────────────┬───────────────────────────────┬────────────────────────────────────────┤
│ Target Legacy System    │ Siphoning / Scavenging Method │ Catastrophic Failure / Retaliation Risk│
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ High-Voltage Power Grid │ Inductive line clamps, meter  │ Arc flash explosion, transformer fire, │
│ (12kV / 240V drops)     │ bypasses, pirate solar inject │ smart-meter automated tamper alarm     │
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Municipal Water & Sewer │ Hot-tap saddle valves on cast │ Water pressure drop alert, backflow    │
│ (Cast-iron mains)       │ iron, storm runoff diversion  │ contamination, municipal cutoff squad  │
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Asphalt, Concrete &     │ Jackhammering roads, cutting  │ Heavy fine citations, structural slab  │
│ Structural Rebar        │ bridge rebar, crushing rubble │ collapse, toxic coal-tar runoff        │
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Fiber & Copper Telecom  │ Inductive telephone taps,     │ Carrier line fault detection, FCC/ISP  │
│ (Roadside punch blocks) │ splicing roadside fiber lines │ signal sweep, cellular triangulation   │
└─────────────────────────┴───────────────────────────────┴────────────────────────────────────────┘
```

#### 4.3.2. Detailed Siphoning Mechanics

##### 1. Power Grid Siphoning & Smart Meter Anomaly Counter:
* **The Siphon:** Founder 01 climbs a utility pole at night to attach inductive line clamps to a 240V drop before the municipal meter.
* **Exergy Yield:** Delivers up to $7.2\text{ kW}$ directly into pirate battery banks.
* **Smart Meter Anomaly Counter:** Drawing $> 30\text{ A}$ creates a localized thermal signature and grid-phase imbalance. The utility's smart anomaly counter rises by $+1\%$ per 100 ticks. At $100\%$, an armed inspection crew accompanied by code enforcement raids the parcel. Drawing $< 10\text{ A}$ safely dissipates the counter at $-0.5\%$ per 100 ticks.
* **Successive Over-Relaxation (SOR) Electrical Flow Solver:**
  Current propagation across tapped 12kV distribution lines and 240V drops is modeled via **SOR ($\omega = 1.6$)**:
  $$\mathbf{G}\mathbf{V} = \mathbf{I}$$
  Because low-impedance copper distribution lines ($R \approx 0.005\ \Omega, g \approx 200\text{ S}$) connect directly to high-impedance pirate battery drops ($R \approx 8.0\ \Omega, g \approx 0.125\text{ S}$), extreme conductance contrast stalls unrelaxed solvers. SOR contracts spectral error rapidly, converging in $< 20$ iterations ($\epsilon < 10^{-4}$). To prevent singular floating matrices ($\det(\mathbf{G}) = 0$) during islanding, the microgrid incorporates a virtual earth ground node ($g_{\text{virtual\_earth}} = 10^{-6}\text{ S}$), backed by a direct Sparse Cholesky fallback ($< 0.12\text{ ms}$) during topology splices.

##### 2. Municipal Water & Sewage Interception:
* **The Siphon:** Excavating $2\text{ m}$ to expose a 4-inch ductile iron municipal main and clamping a stealth saddle tap with a manual ball valve.
* **Exergy Yield:** Abundant pressurized water for crops and domestic living.
* **Retaliation Risk:** Sudden pressure drops alert municipal SCADA systems. Storm runoff siphoning brings heavy metals and oils that require biochar filtration beds and mycelium bio-filtration.

##### 3. Asphalt, Concrete & Rebar Quarrying (Urbanite):
* **The Siphon:** Jackhammering degraded asphalt streets and parking lots; cutting high-tensile steel rebar with oxy-acetylene torches.
* **Exergy Yield:** Produces **Urbanite** (broken concrete blocks) for building foundations, rocket stoves, and retaining walls.
* **Retaliation Risk:** Generates toxic silica dust and bitumen fumes; unmasked workers suffer rapid lung irritation thoughts and physical fatigue.

##### 4. Fiber & Copper Telecom Tapping:
* **The Siphon:** Splicing roadside punch blocks or stripping dark fiber lines.
* **Exergy Yield:** Downloads open-source CAD blueprints, firmware, and weather data into the local Layer 6 semantic cache.
* **Retaliation Risk:** Unencrypted legacy traffic triggers municipal surveillance sweeps.

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
│ (Layer 7 Pressure)│ shallow water mains,     │ steam tunnels, subway    │ boundary fence lines,  │
│                   │ residential meters       │ vaults, fiat property tax│ off-grid well taps     │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Primary Materials │ Asphalt, Pine Siding,    │ Structural Steel,        │ Fungal Loam, Cob,      │
│                   │ Drywall, Turf Grass      │ Reinforced Concrete      │ Biochar, Bamboo, Clay  │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Hydrology Loop    │ Lawn runoff, impervious  │ Sump pumps, stormwater   │ Keyline swales, ponds, │
│                   │ asphalt gutter routing   │ retention vaults, sewage │ greywater reed beds    │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Leeching Vector   │ Tapping grid lines for   │ Siphoning waste heat,    │ Restoring dead topsoil,│
│                   │ microgrid battery charge │ scavenging structural rebar│ capturing runoff water │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Adversarial Focus │ HOA bylaws, drone lawn   │ Building inspectors,     │ Downstream senior water│
│                   │ inspections ($150/day)   │ FLIR thermal sweeps      │ rights enforcement     │
└───────────────────┴──────────────────────────┴──────────────────────────┴────────────────────────┘
```

#### 4.3.4. Spatial Chunk Memory & 128-Chunk Active Visibility Window
To render expansive biomes without exceeding WASM's 256.00 MB linear memory ceiling, the engine maintains a **128-chunk active sector visibility window** ($128 \times 128\text{ KB} = 16.38\text{ MB}$). A full neighborhood sector ($8 \times 8 \times 4$ chunks = 256 chunks) is simulated hierarchically: the core 128 chunks closest to Founder 01 execute full-fidelity thermodynamics, hydrology, and voxel physics, while outer chunks utilize compressed macro-block representations.

#### 4.3.5. The Macro Progression Arc: Three Epochs of Sovereignty
The journey of Lot 402 spans three historical epochs:
1. **Epoch 1: Parasitic Entanglement:** Complete dependency on legacy utilities. Rapid fiat escrow drain ($1,450 starting balance). Constant vulnerability to utility rate hikes and smart-meter surveillance.
2. **Epoch 2: Systematic Scavenging (Symbiotic Dual-Membrane):** Siphoning power, tapping water, depaving driveways into food forests, quarrying urbanite, establishing compost toilets and greywater bioswales.
3. **Epoch 3: Sovereign Severance (Singularity):** Physical utility drops cut. Solar microgrid and rain catchment achieve $100\%$ autarky. The municipal meter stops spinning. Monthly fiat drain drops to zero.

---

### 4.4. Directive 4: Sovereign Stack Multiplayer & Stochastic Boundary Interface

#### 4.4.1. Local-First Simulation Philosophy
Oasis rejects the extractive, fragile cloud-server architecture:
* **Zero Centralized Game Servers:** The simulation executes $100\%$ locally on the user's hardware.
* **Zero Cloud Lock-in:** World voxel state, character psychology, and event logs reside in local storage (SQLite/LMDB or browser OPFS).
* **Total Offline Resilience:** If the global internet goes down, your Oasis simulation continues running deterministically without interruption.

#### 4.4.2. The Stochastic Boundary Interface
The perimeter of the simulated parcel is not an invisible wall. It is an active **Stochastic Boundary Interface** modeling the pressure of the unmanaged legacy fiat world:

```
                  ┌──────────────────────────────────────────────────┐
                  │          THE UNMANAGED LEGACY FIAT WORLD         │
                  │   - Evictions, tax liens, police code raids      │
                  │   - Utility shutoffs, EPA fines, market shocks   │
                  └─────────────────────────┬────────────────────────┘
                                            │
                                            ▼  Stochastic Pressure Vectors
                  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                  ░░░░░░░░░░░ STOCHASTIC BOUNDARY INTERFACE ░░░░░░░░░░
                  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                            │
                                            ▼  Filtered & Attenuated Stress
                  ┌──────────────────────────────────────────────────┐
                  │           SOVEREIGN OASIS SANCTUARY              │
                  │   - Regenerated living soil & micro-grid exergy  │
                  │   - Integrated, healed citizens & low stress     │
                  │   - Physical P2P Proximity Mesh to Peer Nodes    │
                  └──────────────────────────────────────────────────┘
```

##### Inhomogeneous Poisson Arrival Process:
Hostile legacy shocks arrive according to an inhomogeneous Poisson process with time-varying intensity $\lambda(t)$:
$$\lambda(t) = \max\left(\lambda_{\min}, \lambda_{\mathcal{R}} \cdot \exp(-\Psi(t))\right)$$

Where:
* $\lambda_{\mathcal{R}}$ is the baseline arrival intensity governed by current geopolitical regime $\mathcal{R}(t) \in \{\text{Sub-Radar}, \text{Under-Scrutiny}, \text{Adversarial}, \text{Active-Siege}\}$.
* $\lambda_{\min} = 0.001\text{ s}^{-1}$ is the **irreducible legacy threat floor** (approx. 1 event per 16.6 minutes), ensuring off-grid vigilance is never extinguished.
* $\Psi(t)$ is the **Sovereign Attenuation Potential** driven by the three attenuation pillars.

#### 4.4.3. The Three Attenuation Pillars
$$\Psi(t) = \alpha \cdot \Phi_{\text{infrastructure}}(t) + \beta \cdot \Phi_{\text{social}}(t) + \gamma \cdot \tanh\left(\delta \cdot \Phi_{\text{mesh}}(t)\right)$$

1. **Physical Self-Sufficiency ($\Phi_{\text{infrastructure}}$) & Genesis Epsilon Guard:**
   $$\Phi_{\text{infrastructure}}(t) = \frac{1}{3} \left[ \min\left(1.0, \frac{E_{\text{solar\_stored}} + \epsilon}{E_{\text{consumed}} + \epsilon}\right) + \min\left(1.0, \frac{W_{\text{rain\_stored}} + \epsilon}{W_{\text{consumed}} + \epsilon}\right) + \frac{A_{\text{depaved\_loam}}}{A_{\text{total\_lot}}} \right]$$
   At Day 1 start, consumption is zero ($E_{\text{consumed}} = 0, W_{\text{consumed}} = 0$). To prevent catastrophic $0/0 \implies \text{NaN}$ singularities, all evaluations include the **Genesis Autarky Epsilon Guard** ($\epsilon = 0.001\text{ kWh/L}$):
   $$\eta = \frac{E_{\text{self}} + \epsilon}{E_{\text{legacy}} + E_{\text{self}} + \epsilon}$$
2. **Social NPC Integration ($\Phi_{\text{social}}$):**
   $$\Phi_{\text{social}}(t) = \ln\left(1 + \sum_{k=1}^{N_{\text{agents}}} \frac{\text{Trust}_k \cdot \text{MutualAidHours}_k}{100}\right)$$
   Welcoming refugees and integrating them into Trust Ring 1 transforms them into vigilant defenders who patrol the perimeter.
3. **Peer Proximity Mesh Ties ($\Phi_{\text{mesh}}$) & Bounded Weighting:**
   $$\Phi_{\text{mesh}}(t) = \frac{1}{|\mathcal{P}|_{\max}} \sum_{j \in \mathcal{P}} \frac{\text{TrustScore}_{ij}}{1 + \left(\frac{d_{ij}}{d_0}\right)^2}$$
   Where $\mathcal{P}$ is the set of mesh peer nodes connected via Reticulum radio, $d_{ij}$ is distance (km), and $d_0 = 1.0\text{ km}$. Bounded via $\tanh(\delta \cdot \Phi_{\text{mesh}}) \in [0, 1.0)$, preventing IEEE-754 floating-point underflow.

#### 4.4.4. Sovereign Stack Exclusive Multiplayer Protocol Matrix
Oasis contains zero custom P2P networking code. All multiplayer coordination routes through the Sovereign Stack via the UHAI IPC boundary:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                D4 MULTIPLAYER LAYER ALLOCATION MATRIX                            │
├───────┬──────────────────────────────┬───────────────────────────────────────────────────────────┤
│ LAYER │ SOVEREIGN STACK SUBSYSTEM    │ MULTIPLAYER FUNCTIONALITY & PROTOCOL SPECIFICATION        │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L2**│ Physical/Mesh Radio &        │ Reticulum Network Stack (RNS), LoRa (SX1262), 802.11s     │
│       │ Delay-Tolerant Transport     │ Wi-Fi Mesh; Cryptographic Distance Bounding PHY           │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L3**│ Distributed State Ledger &   │ Kademlia DHT, Sphinx Onion Routing, libp2p Gossipsub;     │
│       │ Encrypted Gossip Pub/Sub     │ Automerge/Yrs CRDT state replication, IPLD blockstore     │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L4**│ Identity, Credentials &      │ W3C DID Core (`did:key`, `did:peer`), Ed25519 Keypairs;   │
│       │ Collaborative Orchestration  │ Verifiable Credentials (VCs); Metered WASM BPMN 2.0 VM    │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L5**│ Mutual Credit, Barter &      │ Zero-Sum Mutual Credit Clearing (Hawala/Ripple loops);     │
│       │ Smart Covenants              │ Bilateral/Multilateral Smart Covenants; Two-Chamber WoT   │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L7**│ Adversarial Municipal        │ Discrete Event Simulation (`col-adversaryd`); Municipal   │
│       │ Emulation & Boundary Drivers │ Code Enforcement, Zoning Raids, Utility Monopoly Strikes │
└───────┴──────────────────────────────┴───────────────────────────────────────────────────────────┘
```

#### 4.4.5. Cryptographic Geo-Proximity & Boundary Perimeter Consolidation
Physical distance between nodes is verified via **cryptographic speed-of-light distance bounding**:
$$d_{AB} \le \frac{c \cdot (\Delta t - t_d)}{2}$$

Where $c \approx 3 \times 10^8\text{ m/s}$, $\Delta t$ is round-trip time, and $t_d \le 1.0\text{ ns}$ is hardware turnaround.

##### Isoperimetric Perimeter Consolidation:
Two independent square parcels of area $A$ have a perimeter of $8\sqrt{A}$. When merged along a shared property edge, their interior border dissolves, reducing exposed perimeter to $6\sqrt{A}$—an immediate **$25\%$ reduction in hostile surface area**.

##### Bounded Boundary Friction Coefficient ($\kappa$):
$$\kappa = \kappa_{\text{base}} \cdot \prod_{j \in \text{Peers}} \max\left(0.0, 1 - \alpha \cdot \frac{R_j}{\max(d_j, d_{\min})}\right) \cdot (1 - \beta \cdot P_{\text{buffer}}) \cdot \left(1 - \tanh\left(\gamma \cdot N_{\text{citizens}}\right)\right)$$
Strictly bounded to $\kappa \in [0.05, 1.0]$. The bounded term $\left(1 - \tanh(\gamma \cdot N_{\text{citizens}})\right)$ eliminates negative friction coefficient sign-inversion bugs. As $\kappa$ drops to $0.05$, municipal raid frequency drops by $95\%$, dissolving the Fog of Legacy along the shared line into an open **Commons Corridor**.

---

## 5. Inviolable Architectural & Anti-Bleed Invariants

### 5.1. The 5 Anti-Bleed Invariants
To preserve strict Separation of Concerns (SoC), Oasis enforces five inviolable architectural boundaries:
1. **Zero Embedded Daemons:** Oasis never compiles, links, or embeds Sovereign Stack daemons (`col-telemetryd` through `col-adversaryd`).
2. **Zero In-Engine Private Keys:** Oasis never touches raw Ed25519 or BBS+ private keys. Keys reside exclusively in `col-kmsd` (Layer 5) under `mlockall()` memory protection. Oasis holds only an opaque 32-byte `founder_did_handle`.
3. **Zero Custom P2P Networking:** Oasis contains zero libp2p, zero WebRTC sockets, zero LoRa packet decoders, and zero P2P mesh code. All multiplayer routes through Layer 2 (`col-meshd`) and Layer 3 (`col-storaged`).
4. **Zero In-Engine BPMN XML Parsers:** Oasis emits compact binary `SpatialIntentPayload` structs; Layer 4 (`col-execd`) compiles BPMN 2.0 schemas, enforces ecological floors, and returns `WorkToken` packets.
5. **Zero In-Engine Fiat Accounting:** Oasis simulates physical exergy (watts, liters, joules); Layer 7 (`col-adversaryd`) calculates fiat tariffs and municipal citations.

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
  * Cadastral CAD / Fog Pass: $0.60\text{ ms}$
  * **Total Active Frame Time:** $7.40\text{ ms}$ ($10.60\text{ ms}$ hard budget cap), leaving $> 36\%$ safety headroom.

### 5.4. UHAI IPC Bridge & Shared Memory Data Plane
* **High-Frequency Data Plane:** POSIX SPSC shared memory ring buffer (`/dev/shm/uhai_telemetry.ring`, $< 50\ \mu\text{s}$ latency).
* **Cache-Line Isolation:** `SharedRingHeader` occupies 3 cache lines (192 bytes): Cache line 0 (producer write index & dropped frames counter); Cache line 1 (consumer read index); Cache line 2 (static config). Slots padded to 64 bytes (1 cache line), total SHM size 65,728 bytes.
* **Asynchronous Control Plane:** Unix Domain Sockets (`/var/run/collective/uhai.sock`) transporting FlatBuffers RPC, BPMN tokens, and BBS+ verifiable credentials.

---

## 6. Core Gameplay Loops & Systems Progression

### 6.1. The Daily Diurnal Simulation Rhythm
```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                   THE DAILY SIMULATION RHYTHM                                    │
├─────────────────────┬────────────────────────────────────────────────────────────────────────────┤
│ Time Window         │ Gameplay Phase & Systems Activity                                          │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **06:00 — Dawn**    │ **The Morning Circle:** Bell rings; Founder 01 convenes community. Open    │
│                     │ BPMN WorkTokens are posted on the slate; citizens claim tasks.             │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **07:00 — 12:00**   │ **Morning Production Sprint:** High focus period. Heavy physical labor     │
│                     │ (asphalt quarrying, solar mounting, ditch excavation, keyline trenching).  │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **12:00 — Midday**  │ **Communal Nourishment:** Shared lunch in the mess tent. Thermal resting.  │
│                     │ Need satiation; social thought exchange; hydration replenishment.          │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **13:00 — 17:00**   │ **Afternoon Maintenance & Craft:** Precision crafting (circuit wiring,     │
│                     │ seed sorting, biochar inoculation, tool sharpening, whim execution).       │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **17:00 — 20:00**   │ **Evening Governance & Social Bonding:** Dinner around the rocket heater;  │
│                     │ storytelling, acoustic music, reviewing boundary threat reports.           │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **20:00 — 06:00**   │ **Night Rest & Memory Consolidation:** Sleep cycle. Diurnal memory decay;  │
│                     │ episodic ring buffer consolidation into core beliefs; stress damping.      │
└─────────────────────┴────────────────────────────────────────────────────────────────────────────┘
```

### 6.2. Seasonal Dynamics & Ecological Friction
* **Spring (The Thaw):** Mud challenges transportation; stormwater swales fill; code enforcement begins property inspections.
* **Summer (The Heat & Drought):** Solar exergy at maximum ($> 8.0\text{ kWh/m}^2/\text{day}$); high evaporation demands water conservation; transformer overheat risks.
* **Autumn (The Harvest & Scavenging):** Intensive canning, drying, and root cellar storage; final push to scavenge structural timber and rebar before snow.
* **Winter (The Deep Freeze & Survival):** Low solar yield; high heating exergy demand; pipe burst hazards; social solidarity tested against altruism fatigue.

---

## 7. HUD, UI/UX Design & Sensory Telemetry

### 7.1. Dual Viewport System
Oasis features a seamless dual camera architecture:
* **Direct 3rd-Person View:** Over-the-shoulder perspective; tactile kinematics, tool swings, facial expressions, and dynamic weather.
* **Cadastral Axonometric Slate:** Orthographic blueprint view; exergy and water flow overlays, boundary threat vectors, and architectural planning.
* **Transition:** Smooth 400ms Hermite curve preserves spatial orientation.

### 7.2. The Volumetric Plumbob 2.0
Hovering over Founder 01’s head is a 3D volumetric faceted gemstone—**The Plumbob 2.0**:
* **Calm Emerald Pulse (0.5 Hz):** High focus ($\ge 180$), low stress ($< 20,000$).
* **Warm Amber Flicker (1.5 Hz):** Strained focus ($50 \dots 100$), moderate stress, physiological fatigue.
* **Jagged Crimson Strobing (4.0 Hz):** Acute breakdown, tantrum, melancholy, or moral defiance.
* **Prismatic Iridescent Shimmer:** Strange Mood / Creative Burst in progress.

### 7.3. Cognitive Dossier & Empathetic Narrative Stream
Selecting any character opens their **Cognitive Dossier**, presented as an intimate narrative:
> *"Founder 01 feels deeply unsettled. She is haunted by the memory of the municipal code raid at the south gate last Autumn. She felt quiet pride while laying cob around the rocket stove yesterday. She values ecological harmony above all else and is frustrated by Adam's reluctance to share salvaged copper wire. Her thoughts are scattered."*

### 7.4. Volumetric Fog of Legacy & Commons Corridors
Outside the player’s active parcel boundary, the world is shrouded in the **Fog of Legacy**—a dense, volumetric atmospheric smog illuminated by distant emergency sirens, surveillance drones, and flickering sodium-vapor streetlights. As the player regenerates soil and establishes peer proximity ties, the fog recedes, revealing clean skies and verdant **Commons Corridors**.

---

## 8. The Six Inviolable Quality Gates (G1–G6)

To prevent simulation drift and guarantee pedagogical validity, every scenario and feature in Oasis must satisfy Six Inviolable Quality Gates:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                               THE SIX INVIOLABLE QUALITY GATES (G1–G6)                           │
├───────┬───────────────────────────────┬──────────────────────────────────────────────────────────┤
│ Gate  │ Domain                        │ Acceptance Criteria & Validation Method                  │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G1**│ Thermodynamic & Exergy        │ Energy and mass balances obey conservation laws (1st/2nd │
│       │ Conservation Gate             │ laws). Power draw matches physical enthalpy and Joule    │
│       │                               │ losses. Zero free-energy exploits. SOR solver error < 1e-4│
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G2**│ Offline Local-First Partition │ 100% deterministic execution with zero WAN connectivity. │
│       │ & Privacy Gate                │ Zero central server pings. BLE/NFC checkouts and peer    │
│       │                               │ distance bounding execute purely on local hardware.       │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G3**│ Byzantine & Sybil Immunity    │ Malicious mesh packet floods dropped at physical layer.  │
│       │ Gate                          │ Sybil nodes > 3 hops receive zero mutual credit. Social  │
│       │                               │ defection slashes voucher reputation by exactly 50%.     │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G4**│ Psychological Stability &     │ Dual-timescale Lyapunov damping prevents infinite stress │
│       │ Anti-Livelock Gate            │ death spirals. Clinical recovery attractor triggers at   │
│       │                               │ stress > 80k. 30-tick watchdog with exponential backoff. │
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G5**│ Trojan Horse Pedagogical      │ In-game blueprints match real-world open hardware specs  │
│       │ Validity Gate                 │ (CERN-OHL, permaculture keyline math). Ludic skills      │
│       │                               │ compile directly to physical node stewardship blueprints.│
├───────┼───────────────────────────────┼──────────────────────────────────────────────────────────┤
│ **G6**│ SITL Hardware-in-the-Loop &   │ Zero embedded daemons in engine. Synthetic telemetry tagged│
│       │ Anti-Bleed Invariant Gate     │ QUALITY_SIMULATED cannot actuate real physical hardware. │
│       │                               │ UHAI SPSC ring buffer latency < 50 µs in /dev/shm.       │
└───────┴───────────────────────────────┴──────────────────────────────────────────────────────────┘
```

---

## 9. Audio & Narrative Themes

### 9.1. Acoustic Landscape & Somatic Cues
* **The Living Hearth vs. The Dying Machine:** Inside Lot 402, soundscapes feature the gentle bubbling of bio-swales, the crackle of seasoned oak in rocket stoves, acoustic guitars, and rhythmic hand sawing. Outside the perimeter hums the oppressive, industrial drone of 60 Hz high-voltage transformers, diesel engines, and police sirens.
* **Somatic Audio Feedback:** Fatigued characters breathe heavily; high-stress agents mutter under their breath or slam tools with excessive force; harmonious community meals produce warm, resonant vocal murmurs.

### 9.2. Thematic Narrative Tone
Oasis is serious, grounded, and defiantly optimistic. It avoids both cynical post-apocalyptic despair and naive utopian fantasy. It honors the dignity of physical labor, the reality of human psychological friction, and the transformative power of organized mutual aid.

---

## 10. Conclusion & Handoff Directives

With the unanimous ratification of `OASIS-GDD-V5.0`, the game design specification for Oasis is fully realigned with **Scenario Driven Development (SDD)**. The engine is established as the interactive vanguard and Software-In-The-Loop digital twin of The Collective, grounded in Dwarf Fortress psychology, Sims indirect agency, Cities: Skylines infrastructure leeching, and Sovereign Stack local-first multiplayer.

This document serves as the authoritative blueprint for:
1. **Phase 3 (`PRODUCT_BACKLOG_V5.md`):** Direct translation of these SDD mechanics into actionable engineering epics and user stories.
2. **Phase 4 (`SOVEREIGN_STACK_BACKLOG.md` V3):** Alignment of the 7-layer headless daemon stack to support dynamic scenario ingestion and UHAI IPC contracts without violating domain isolation.
