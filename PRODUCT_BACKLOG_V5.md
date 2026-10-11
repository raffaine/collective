# OASIS ENGINE PRODUCT BACKLOG V5.0
**Document Identifier:** `OASIS-BACKLOG-V5.0`  
**Milestone:** Phase 3 Strategic Realignment Council Ratification  
**Classification:** Authoritative Technical Product Backlog & Sprint Execution Plan  
**Target Repositories:** `core/1_simulation/`, `docs/`, `scripts/`, `tools/sdd_compiler/`  
**Engine Baseline:** C++20 (`-std=c++20`) / WebGPU (WGSL) / Flecs ECS / UHAI IPC  
**Total Epics:** 7 Epics  
**Total Articulated User Stories:** 37 User Stories  
**Total Story Point Budget:** 242 Story Points  
**Sprint Schedule:** 4 Sprints (Sprint 1: 58 SP, Sprint 2: 62 SP, Sprint 3: 60 SP, Sprint 4: 62 SP)  
**Status:** Council Approved (Unanimous Consensus Reached)  
**Date of Enactment:** 2026-10-10  

---

## 1. Council Ratification & Unanimous Consensus

This Product Backlog embodies the binding consensus of the Product Owner and the Oasis Technical Team (Graphics Stack Engineer, Platform/Gameplay Systems Engineer, Build Engineer, Quality Engineer, Sovereign Stack Specialist), convened under the directive of the Strategic Realignment Council. Grounded in `docs/COLLECTIVE_GUIDING_PRINCIPLES.md`, `docs/OASIS_GDD.md` (V5.0), and `docs/OASIS_ARCHITECTURE.md`, this backlog translates Scenario Driven Development (SDD), 3D WebGPU compute raymarching, and Dwarf Fortress cognitive psychology into an executable 4-sprint roadmap.

### 1.1. Council Roll & Formal Ratification Statements

1. **Product Owner (Council Lead):**
   > *"The fatal error of ambitious software projects is architectural drift between simulation games and real-world infrastructure. Backlog V5 solves this permanently. By establishing Scenario Driven Development (SDD) as our single cybernetic compiler across all 24 canonical MVS scenarios, every story in this backlog compiles directly into both Flecs ECS client gameplay and edge daemon specifications. We balance 242 Story Points across four balanced sprints of 58–62 points each, delivering an uncompromising Trojan Horse Simulator that teaches authentic physical resilience. I ratify this backlog unconditionally."*

2. **Graphics Stack Engineer:**
   > *"The visual foundation of Oasis is locked to mathematical reality. Rendering a 3D volumetric voxel world in the browser at 60 FPS without rasterization hacks requires rigorous shader discipline. In Epic 6, we commit to a two-level hierarchical DDA raymarcher in WebGPU WGSL with coarse macro-chunk occupancy acceleration that limits primary ray steps to under 12 and completes traversal in 3.80 ms. Combined with our two-phase deferred PBR and Cadastral CAD passes, active frame time is strictly bounded to 7.40 ms against our 10.60 ms hard budget cap. I ratify this backlog without reservation."*

3. **Platform/Gameplay Systems Engineer:**
   > *"We have systematically purged simplistic Sims motive bars and omnipotent god cursors. In Epic 1 and Epic 2, we implement the 64-byte `AgentPsychologyComponent` and 768-byte episodic memory ring buffer with permanent trauma retention, Lyapunov stress homeostasis, and the 4-tier breakdown FSM. In Epic 2, Founder 01 is embodied with finite calories and circadian fatigue, evaluating advisory orders via logistic moral rejection curves with 25% utility hysteresis and 30-tick watchdog protection. In Epic 3, our Successive Over-Relaxation (SOR) electrical flow solver converges in under 20 iterations with virtual earth grounding. Every gameplay mechanic is deterministic, cache-aligned, and zero-heap. I ratify this backlog enthusiastically."*

4. **Build Engineer:**
   > *"Our deterministic toolchain is airtight. Native builds compile under LLVM/Clang 18 with CMake 3.28.3 and Ninja, while the browser target compiles via Emscripten SDK (emsdk 3.1.56) with `emdawnwebgpu`, `pthreads`, and `SharedArrayBuffer` support. WebAssembly linear memory is reconciled down to the exact byte: a rigid 256.00 MB (268,435,456 bytes) static partition with zero runtime heap allocations on the simulation tick. Every dependency is locked via Homebrew, APT, and `vcpkg.json`. I ratify this backlog."*

5. **Quality Engineer (Automation & Gatekeeper):**
   > *"Software without automated verification is merely an aspiration. Backlog V5 formally integrates the Six Inviolable Quality Gates (G1 through G6) directly into the acceptance criteria of every user story. We enforce thermodynamic conservation (G1), WAN-disconnected offline autonomy (G2), Byzantine/Sybil immunity (G3), zero hanging escrow sagas (G4), Trojan fiat ingestion (G5), and terminal decoupling verification (G6). All 24 canonical scenarios must pass automated headless SITL regression suites before deployment. I ratify this backlog."*

6. **Sovereign Stack Specialist:**
   > *"The boundary between the Oasis client and the Sovereign Stack is permanently sealed. In Epic 4 and Epic 7, we enforce the Five Anti-Bleed Invariants: zero embedded daemons, zero in-engine private keys, zero custom P2P networking, zero BPMN XML parsers, and zero fiat accounting. The Universal Human-Agent Interface (UHAI) connects the engine to the 7-daemon stack via a lock-free SPSC shared-memory ring buffer in `/dev/shm` delivering sub-50 µs latency with 3-cache-line header isolation. Furthermore, synthetic telemetry tagged `QUALITY_SIMULATED` is mathematically barred from triggering physical hardware actuation. I ratify this backlog unconditionally."*

### 1.2. Unanimous Consensus Declaration

```
════════════════════════════════════════════════════════════════════════════════════════
                        STRATEGIC REALIGNMENT COUNCIL
                               RATIFICATION LOG
════════════════════════════════════════════════════════════════════════════════════════
[VOTE] Product Owner (Council Lead)        : AYE (Unconditional)
[VOTE] Graphics Stack Engineer             : AYE (Unconditional)
[VOTE] Platform/Gameplay Systems Engineer  : AYE (Unconditional)
[VOTE] Build Engineer                      : AYE (Unconditional)
[VOTE] Quality Engineer                    : AYE (Unconditional)
[VOTE] Sovereign Stack Specialist          : AYE (Unconditional)

STATUS: Unanimous Consensus Reached
DATE: 2026-10-10T23:35:00Z
ACTION: ENACTED AS CANONICAL PRODUCT BACKLOG V5.0
════════════════════════════════════════════════════════════════════════════════════════
```

---

## 2. Executive Vision & Release Strategy

### 2.1. Strategic Product Vision
Oasis is the **Trojan Horse Simulator** for physical-world sovereign resilience. It is not an escapist fantasy, an idle clicker, or an idealized greenfield sandbox. It is an embodied, high-performance, local-first 3D simulation game where every ludic mechanic compiles directly into physical survival competence and cybernetic edge infrastructure.

* **Epic 1 (D1): Dwarf Fortress Cognitive Core:** Deep psychological modeling replacing simplistic need bars. 64-byte cache-aligned psychology state, 768-byte companion memory ring buffer (32 slots $\times$ 24B), Lyapunov stress homeostasis, breakdown FSM, and Strange Moods with 72-hour artifact crafting.
* **Epic 2 (D2): The Sims Indirect Founder Management:** Intimate, embodied stewardship of Founder 01. Finite caloric metabolism (2,000–4,500 kcal/day), circadian fatigue, advisory intention queue, spontaneous personal whims, logistic moral rejection heuristics, 25% utility hysteresis, and 30-tick watchdog timers.
* **Epic 3 (D3): Cities: Skylines Macro Infrastructure Leeching:** Systematic scavenging of decaying 12kV grid lines, municipal water mains, asphalt, and dark fiber across Suburban Sprawl, Dense Urban, and Permaculture biomes, solved via Successive Over-Relaxation (SOR, $\omega = 1.6$) electrical flow modeling across 3 bootstrapping epochs.
* **Epic 4 (D4): Stochastic Boundary Interface & Sovereign Stack Multiplayer:** Inhomogeneous Poisson arrival process modeling legacy pressure, attenuated by 3 sovereign pillars (autarky with Genesis Epsilon Guard $\epsilon = 0.001$, social NPC mutual aid, and peer proximity mesh). Multiplayer executed exclusively via Sovereign Stack (L2–L7) over UHAI IPC with speed-of-light distance bounding.
* **Epic 5 (SDD): Scenario Driven Development Dynamic Ingestion & In Silico Compiler:** Dynamic ingestion pipeline for the 24 canonical MVS scenarios (`docs/scenarios/*.md`), automated JSON-LD schema parsing, dynamic Flecs ECS data binding, spatial voxel palette allocation, Gibsonian affordance generation, upward compilation loop, and automated G1–G6 verification.
* **Epic 6 (Core): High-Performance Engine Core & WebGPU Amanatides & Woo Raymarcher:** C++20 Flecs ECS, two-level hierarchical DDA compute raymarcher in WebGPU WGSL, 7.40 ms active frame budget (10.60 ms hard cap, 60 FPS), rigid 256.00 MB WASM linear memory partition table (zero runtime allocations), and cross-platform build pipeline (CMake, Ninja, LLVM 18, emsdk 3.1.56, vcpkg).
* **Epic 7 (UHAI): Universal Human-Agent Interface Shared-Memory IPC & SITL Telemetry:** Lock-free SPSC shared memory ring buffer at `/dev/shm/uhai_telemetry.ring` (< 50 µs latency, 3 cache line header isolation), Emscripten browser adapter (`SharedArrayBuffer`), strict enforcement of 5 Anti-Bleed Invariants, and physical airgap isolation between `QUALITY_SIMULATED` and `QUALITY_HARDWARE_ROOT`.

### 2.2. Four-Sprint Release Roadmap

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       OASIS ENGINE REBOOT ROADMAP (4 SPRINTS)                                         │
├──────────┬─────────────────────────────────┬───────────────────────────────────────────────────────────┬──────────────┤
│ Sprint   │ Focus Area                      │ Delivered Core Capabilities                               │ Velocity     │
├──────────┼─────────────────────────────────┼───────────────────────────────────────────────────────────┼──────────────┤
│ **SP 1** │ Core Engine & Physical UHAI     │ WebGPU hierarchical DDA compute raymarcher, 256 MB WASM  │ 58 Points    │
│          │ Foundations                     │ memory layout, /dev/shm SPSC ring buffer (<50 µs), 32-bit │              │
│          │                                 │ voxels, basic facets, and Founder 01 mortal embodiment.   │              │
├──────────┼─────────────────────────────────┼───────────────────────────────────────────────────────────┼──────────────┤
│ **SP 2** │ Cognitive Architecture &        │ 32-slot episodic memory ring (768B), SDD dynamic ingestion│ 62 Points    │
│          │ Dynamic SDD Ingestion           │ pipeline for 24 scenarios, SOR flow solver (ω=1.6), 12kV  │              │
│          │                                 │ grid siphoning, advisory queue, and moral rejection curve.│              │
├──────────┼─────────────────────────────────┼───────────────────────────────────────────────────────────┼──────────────┤
│ **SP 3** │ Stress Homeostasis, Multi-Biome │ Dual-timescale Lyapunov damping, clinical recovery        │ 60 Points    │
│          │ & Stochastic Boundary Interface │ attractor, breakdown FSM, Suburban/Urban/Eco biomes,      │              │
│          │                                 │ water/fiber tapping, Poisson boundary, mesh P2P.          │              │
├──────────┼─────────────────────────────────┼───────────────────────────────────────────────────────────┼──────────────┤
│ **SP 4** │ Strange Moods, Terminal         │ 72-hour Strange Mood artifact crafting, Terminal Decoupling│ 62 Points    │
│          │ Decoupling & SITL Verification  │ Singularity (Omega), affordances, watchdog hysteresis,    │              │
│          │                                 │ automated G1–G6 verification harness, and SITL digital twin│              │
├──────────┴─────────────────────────────────┴───────────────────────────────────────────────────────────┼──────────────┤
│ TOTAL BACKLOG VELOCITY BUDGET                                                                         │ 242 Points   │
└───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## 3. Surgical Separation of Concerns & Anti-Bleed Invariants Manifest

Backlog V5 strictly codifies the surgical excision of all monolithic anti-patterns and bleeding interfaces between the Oasis Game Engine (Layer 1b) and the Sovereign Stack (Layers 1–7):

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       OASIS ARCHITECTURAL BOUNDARY PURGE MANIFEST                                     │
├──────────────────────────────┬───────────────────────────────┬────────────────────────────────────────────────────────┤
│ Contaminated Subsystem       │ Purged Anti-Pattern           │ Backlog V5 Corrective Boundary & Invariant             │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `did_crypto_generator.cpp`   │ In-engine libsodium Ed25519   │ PURGED (Anti-Bleed Inv 2). Oasis holds only opaque     │
│                              │ key generation in C++ loop.   │ 32-byte `founder_did_handle`. Keys locked in `col-kmsd`.│
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `crdt_sync_engine.cpp`       │ In-engine libp2p/WebRTC       │ PURGED (Anti-Bleed Inv 3). Zero P2P sockets in engine. │
│                              │ Gossipsub networking.         │ Multiplayer state sync handled by `col-storaged` (L3). │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `bpmn_parser.cpp`            │ In-engine XML parsing of      │ PURGED (Anti-Bleed Inv 4). Oasis emits binary          │
│                              │ BPMN schemas via `pugixml`.   │ `SpatialIntentPayload`; `col-execd` runs BPMN VM (L4). │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `Layer7DrainAccumulator`     │ In-engine fiat accounting and │ PURGED (Anti-Bleed Inv 5). Oasis simulates watts;      │
│                              │ utility tariff debiting.      │ `col-adversaryd` (L7) computes external fiat tariffs.  │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `iot_telemetry_bridge.cpp`   │ Direct MQTT / CoAP hardware   │ PURGED (Anti-Bleed Inv 1). Real hardware drivers       │
│                              │ actuation in game runtime.    │ belong exclusively to `col-telemetryd` (L1).           │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ Monolithic Game Loop         │ Hardcoding Sovereign layers   │ PURGED (Anti-Bleed Inv 1). Decoupled POSIX daemons     │
│                              │ into engine threads (T1, T3). │ communicating over lock-free UHAI shared-memory IPC.   │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ Synthetic Telemetry Bleed    │ Simulated sensor data driving │ PURGED (Airgap Invariant). Synthetic data tagged       │
│                              │ real physical plant relays.   │ `QUALITY_SIMULATED` (0x01) barred from hardware plant. │
└──────────────────────────────┴───────────────────────────────┴────────────────────────────────────────────────────────┘
```

---

## 4. Epic Summary & Story Point Allocation

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                            EPIC SUMMARY & POINT ALLOCATION                                            │
├────────┬────────────────────────────────────────────────────────────────────────┬─────────────┬───────────────┬───────┤
│ Epic   │ Epic Title                                                             │ Priority    │ Stories Count │ Points│
├────────┼────────────────────────────────────────────────────────────────────────┼─────────────┼───────────────┼───────┤
│ **E1** │ Dwarf Fortress Cognitive Architecture & Deep Psychology (D1)           │ P0 Critical │ 5 Stories     │ 33 SP │
│ **E2** │ The Sims Indirect Founder Management & Agency Loop (D2)                │ P0 Critical │ 5 Stories     │ 32 SP │
│ **E3** │ Cities: Skylines Macro Infrastructure Leeching & Multi-Biome (D3)      │ P1 High     │ 6 Stories     │ 42 SP │
│ **E4** │ Stochastic Boundary Interface & Sovereign Stack Multiplayer (D4)       │ P1 High     │ 5 Stories     │ 31 SP │
│ **E5** │ Scenario Driven Development (SDD) Dynamic Ingestion & Compiler         │ P0 Critical │ 6 Stories     │ 40 SP │
│ **E6** │ High-Performance Engine Core & WebGPU Amanatides & Woo Raymarcher      │ P0 Critical │ 5 Stories     │ 34 SP │
│ **E7** │ Universal Human-Agent Interface (UHAI) IPC & SITL Telemetry            │ P0 Critical │ 5 Stories     │ 30 SP │
├────────┴────────────────────────────────────────────────────────────────────────┴─────────────┼───────────────┼───────┤
│ TOTAL (7 Epics, 37 Articulated Stories)                                                       │ 37 Stories    │ 242 SP│
└───────────────────────────────────────────────────────────────────────────────────────────────────────────────┴───────┘
```

---

## 5. Detailed Epics & User Stories

### Epic 1: Dwarf Fortress Cognitive Architecture & Deep Psychology (DF Cognitive Core)
**Epic ID:** `EPIC-1` | **Total Estimate:** 33 Story Points | **Priority:** P0 Critical  
**Focus:** Implement the cache-aligned 64-byte `AgentPsychologyComponent`, companion 768-byte `AgentMemoryRingComponent` (32 slots $\times$ 24B), Big-Five facets, 8 Core Values enum, active thought buffer, dual-timescale Lyapunov stress homeostasis with clinical recovery attractor, 4-tier breakdown FSM, Strange Moods with 72-hour artifact crafting, and spatial trauma flashbacks.

---

#### Story 1.1: 64-Byte Bounded Psychological State & Core Values ECS Component
* **ID:** `STORY-1.1` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G4
* **User Story:** *As an Oasis simulation agent, I want my personality facets, core philosophical values, and emotional state packed into a single 64-byte cache-aligned ECS component, so that hundreds of agents can evaluate emotional stimuli without causing CPU cache misses or heap allocations.*
* **Technical Tasks:**
  1. Define `PersonalityFacets` struct (`alignas(4)`, 16 bytes) containing 10 signed 8-bit facets (`assertiveness`, `anxiety`, `empathy`, `self_discipline`, `curiosity`, `pragmatism`, `stoicism`, `sociability`, `discordance`, `artistic_drive`, plus 6 padding bytes).
  2. Define `CoreValues` struct (`alignas(4)`, 16 bytes) containing 8 signed 8-bit values (`AUTONOMY`, `STEWARDSHIP`, `SOLIDARITY`, `INGENUITY`, `TENACITY`, `PRAGMATISM`, `TRANSPARENCY`, `BEAUTY`, plus 8 SIMD alignment padding bytes).
  3. Define `AgentPsychologyComponent` (`alignas(64)`, exactly 64 bytes) packing `PersonalityFacets` (16B), `CoreValues` (16B), `active_mood` (Fixed32, 4B), `cumulative_stress` (Fixed32, 4B), `short_term_focus` (uint8_t, 1B), `breakdown_state` (uint8_t, 1B), `active_thought_count` (uint8_t, 1B), `ring_head_index` (uint8_t, 1B), `flags` (uint8_t, 1B), and 19 padding bytes.
  4. Register component in Flecs ECS archetype tables and verify `sizeof(AgentPsychologyComponent) == 64` via `static_assert`.
  5. Implement deterministic initialization from split-stream PCG32 seeded by agent ID.
* **Acceptance Criteria:**
  * **Given** an entity is spawned in the Flecs ECS world,
  * **When** `AgentPsychologyComponent` is attached,
  * **Then** `sizeof(AgentPsychologyComponent) == 64` and `alignof(AgentPsychologyComponent) == 64`,
  * **And** accessing and mutating values executes in $< 5\text{ ns}$ without dynamic heap allocations,
  * **And** satisfies Quality Gate G4 (deterministic memory footprint).

---

#### Story 1.2: 768-Byte Companion Episodic Memory Ring & Salience Decay
* **ID:** `STORY-1.2` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G4
* **User Story:** *As an agent, I want high-valence sensory and social events recorded into a fixed 32-slot circular ring buffer with Ebbinghaus salience decay and permanent trauma protection, so that lived hardships and triumphs alter my long-term behavior without unbounded memory growth.*
* **Technical Tasks:**
  1. Define `EpisodicMemoryNode` struct (24 bytes packed, `alignas(8)`) containing `timestamp_ticks` (uint32_t, 4B), `event_type` (uint16_t, 2B), `subject_entity_id` (uint16_t, 2B), `emotional_valence` (Fixed16 Q8.8, 2B), `salience` (uint16_t, 2B), `voxel_x/y/z` (3 $\times$ uint16_t, 6B), and `flags` (uint16_t, 2B: Bit 0 `PERMANENT_MEMORY`, Bits 1-15 trigger count), plus 4 padding bytes for 8-byte alignment (yielding sizeof(EpisodicMemoryNode) == 24 bytes).
  2. Implement `AgentMemoryRingComponent` (`alignas(8)`, exactly 768 bytes) storing an array of 32 `EpisodicMemoryNode` structs ($32 \times 24 = 768\text{ B}$).
  3. Implement diurnal Ebbinghaus salience decay running at midnight sleep boundaries: $S(t) = S_0 \cdot \exp(-\Delta t / \tau_{\text{decay}}) \cdot (1 + |\text{valence}| / V_{\max})$.
  4. Implement salience-priority ring replacement: nodes with `salience >= 80` receive the `PERMANENT_MEMORY` flag (`0x0001`). When the 32-slot ring wraps, the engine evicts the oldest entry with the lowest salience that lacks `PERMANENT_MEMORY`.
  5. Implement spatial hash flashback queries triggering when an agent moves within 4 decimeters of a trauma voxel coordinate, injecting a negative thought into the active thought buffer.
* **Acceptance Criteria:**
  * **Given** Founder 01 witnesses a code enforcement raid at voxel coordinate $(95, 32, 48)$ with salience 95,
  * **When** 50,000 simulation ticks elapse and 40 subsequent events occur,
  * **Then** the trauma memory node remains un-evicted in the ring buffer due to `PERMANENT_MEMORY` flag protection,
  * **And** walking within 4 decimeters of $(95, 32, 48)$ triggers a sensory flashback injecting `THOUGHT_RAID_TRAUMA`, dropping focus by 30 points,
  * **And** `sizeof(AgentMemoryRingComponent) == 768` with zero heap allocations.

---

#### Story 1.3: Dual-Timescale Lyapunov Stress Homeostasis & Clinical Attractor
* **ID:** `STORY-1.3` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 3 | **Quality Gate:** G4
* **User Story:** *As the simulation engine, I want to integrate active thought valence into a cumulative stress accumulator governed by dual-timescale Lyapunov damping with clinical recovery attractors, so that agents experience authentic psychological breakdowns without entering unrecoverable death spirals.*
* **Technical Tasks:**
  1. Implement $Q24.8$ fixed-point stress differential integration evaluating mood against resilience: $\frac{d\mathcal{S}}{dt} = \dot{S}_{\text{acute}} - \kappa_{\text{resilience}} \cdot (\text{Focus} + \text{MutualAid}) - \frac{1}{\tau_{\text{baseline}}} \tanh\left(\frac{\mathcal{S} - \mathcal{S}_{\text{baseline}}}{S_0}\right)$.
  2. Implement fast acute damping ($\tau_{\text{acute}} = 30\text{ s}$) modulating immediate emotional shocks (dropped wrench, cold soup).
  3. Implement slow chronic integration ($\tau_{\text{baseline}} = 7\text{ days}$) accumulating deep environmental hardship (winter freeze, partner death).
  4. Implement non-linear restoring attractor $\tanh\left(\frac{\mathcal{S} - \mathcal{S}_{\text{baseline}}}{S_0}\right)$ with resilience damping from high focus and warm communal meals.
  5. Implement **Clinical Recovery Attractor**: when stress crosses $\mathcal{S} > 80,000$ (Severe Catatonia), peer agents autonomously trigger nursing behaviors (warm soup, blankets), enforcing a guaranteed restorative drift ($\Delta \mathcal{S}_{\text{clinical}} = -150\text{ units/hour}$) to break absorbing death spirals.
* **Acceptance Criteria:**
  * **Given** an agent experiences persistent negative stimuli pushing cumulative stress to 85,000,
  * **When** peers provide soup and shelter affordances over 48 in-game hours,
  * **Then** cumulative stress recovers below 50,000 via the clinical recovery attractor,
  * **And** stress values remain strictly clamped within $[-100,000, +100,000]$ in $Q24.8$ fixed-point arithmetic without numerical overflow.

---

#### Story 1.4: 4-Tier Breakdown FSM & Somatic Behavioral Invariants
* **ID:** `STORY-1.4` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 3 | **Quality Gate:** G4
* **User Story:** *As an agent experiencing extreme stress, I want my behavioral state machine to transition through 4 distinct tiers of psychological breakdown, so that mental suffering manifests in observable somatic actions and task refusals.*
* **Technical Tasks:**
  1. Implement 4-tier Breakdown State Machine in Flecs ECS:
     * Tier 0: Normal ($\mathcal{S} < 25,000$).
     * Tier 1: Mild Irritability ($25,000 \le \mathcal{S} < 50,000$) — snaps in mess hall, refuses non-essential chore assignments.
     * Tier 2: Moderate Split State ($50,000 \le \mathcal{S} < 80,000$) — randomly splits into *Tantrum* (kicks inverters, smashes tools, damages voxels) or *Melancholy* (sits in dirt, stares blankly, ignores user queue).
     * Tier 3: Severe Catatonia ($\mathcal{S} \ge 80,000$) — complete motor shutdown, requires spoon-feeding.
  2. Bind breakdown state transitions to agent animation state flags and somatic audio cues (muttering, heavy breathing, sobbing).
  3. Ensure agents in Tier 2/3 release all tool locks and fail active BPMN task leases cleanly.
* **Acceptance Criteria:**
  * **Given** an agent's cumulative stress reaches 55,000 with high `discordance`,
  * **When** the agent transitions to Tier 2 Tantrum,
  * **Then** the agent drops their current task, attacks an adjacent equipment voxel (inflicting 15 durability damage),
  * **And** advisory directives from the player are rejected with an audible frustration cue.

---

#### Story 1.5: Strange Moods & 72-Hour Masterwork Artifact Crafting
* **ID:** `STORY-1.5` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Sprint 4 | **Quality Gate:** G1, G4
* **User Story:** *As an inspired artisan agent, I want to occasionally enter a manic "Strange Mood" and seize a workshop, demanding specific materials to craft a masterwork artifact within 72 hours, so that high creative drive produces transformative community assets.*
* **Technical Tasks:**
  1. Implement Strange Mood trigger evaluation: requires $\text{ArtisticDrive} \ge 70$, $\text{Autonomy} \ge 60$, $\text{Focus} \ge 180$, and random roll against creative potential.
  2. Implement Workshop Lockout: agent claims the nearest fabrication workshop (Scenario Alpha/Theta), barring peers from entry.
  3. Implement Material Demands Generator: agent requests 3-4 specific local materials (e.g., 4-gauge copper wire, seasoned white oak, biochar, beeswax).
  4. Implement 72-Hour Crafting Countdown:
     * Success (supplied within 72h): agent crafts a **Masterwork Artifact** (e.g., High-Efficiency MPPT Inverter, Hand-Wound LoRa Transceiver), granting $+20\%$ permanent parcel efficiency, $+15$ permanent Stoicism, and setting stress to 0.
     * Failure (timeout): agent collapses into berserk fury or permanent suicidal melancholia.
  5. Add Plumbob 2.0 prismatic iridescent shimmer visual shader effect during the mood.
* **Acceptance Criteria:**
  * **Given** an artisan agent enters a Strange Mood and claims the electronics bench,
  * **When** required copper wire and biochar materials are delivered within 72 in-game hours,
  * **Then** the agent produces an artifact entity with the `Masterwork` tag, parcel exergy efficiency gains $+20\%$,
  * **And** if materials are denied for 72 hours, the agent transitions to Catatonia/Berserk and stress locks at $+100,000$.

---

### Epic 2: The Sims Indirect Founder Management & Agency Loop (The Sims Indirect Founder)
**Epic ID:** `EPIC-2` | **Total Estimate:** 32 Story Points | **Priority:** P0 Critical  
**Focus:** Implement Founder 01 mortal embodiment (2,000–4,500 kcal/day, circadian fatigue), advisory intention queue, spontaneous whims, Gibsonian environmental affordance guidance, logistic moral rejection heuristics, 25% utility hysteresis, 30-tick progress watchdog with exponential backoff, and Trust Ring delegation.

---

#### Story 2.1: Founder 01 Mortal Embodiment & Caloric Fatigue ECS System
* **ID:** `STORY-2.1` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G1
* **User Story:** *As Founder 01, I want my physical stamina, caloric burn (2,000–4,500 kcal/day), hydration, and circadian sleep debt modeled as physical constraints, so that players must care for my physical survival rather than treating me as an immortal puppet.*
* **Technical Tasks:**
  1. Implement `SomaticStateComponent` in Flecs ECS: caloric reserve (Q24.8 kcal), hydration (0..100%), stamina (0..255), body temperature ($^\circ\text{C}$), circadian sleep debt (ticks).
  2. Implement basal metabolic burn rate: $2,000\text{ kcal/day}$ at rest, scaling dynamically up to $4,500\text{ kcal/day}$ when swinging pickaxes, hauling urbanite, or digging bioswales.
  3. Implement circadian fatigue integration: accumulating sleep debt imposes progressive focus penalties and accelerates stress drift.
  4. Connect hydration depletion to environmental temperature and physical exertion rates.
* **Acceptance Criteria:**
  * **Given** Founder 01 spends 6 in-game hours jackhammering asphalt at $32^\circ\text{C}$,
  * **When** somatic ticks integrate exertion,
  * **Then** caloric burn rate increases to $4,200\text{ kcal/day}$ equivalent, hydration drops by $60\%$,
  * **And** stamina exhaustion forces the agent to drop tools and seek water/rest affordances, satisfying Gate G1.

---

#### Story 2.2: Advisory Intention Queue & Spontaneous Personal Whims
* **ID:** `STORY-2.2` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G4
* **User Story:** *As Founder 01, I want chores designated on the Cadastral Slate to enter an advisory intention queue that I evaluate alongside my own spontaneous whims, so that I maintain authentic human agency.*
* **Technical Tasks:**
  1. Implement `AdvisoryIntentionQueue` component: holds up to 8 advisory tasks enqueued by the player from the Cadastral Slate.
  2. Implement Spontaneous Whim Generator: agent evaluates high personality facets and current surroundings to generate personal aspirations (e.g., *"Wants to build a solar food dehydrator"*, *"Craves solitary reading by woodstove"*).
  3. Implement Whim Fulfillment Rewards: executing a personal whim restores $+20$ Focus and purges $-15$ Stress from the accumulator.
  4. Implement Cadastral Slate UI interface allowing players to pin, cancel, or reorder advisory chores.
* **Acceptance Criteria:**
  * **Given** the player queues 3 trench-digging chores on the Cadastral Slate,
  * **When** Founder 01 generates a whim to brew warm herbal tea,
  * **Then** Founder 01 interleaves the tea-brewing whim based on utility score,
  * **And** completing the whim confers $+20$ Focus before resuming the advisory digging queue.

---

#### Story 2.3: Gibsonian Environmental Affordance Broker
* **ID:** `STORY-2.3` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G4
* **User Story:** *As the simulation engine, I want environmental objects to emit Gibsonian affordance trees into the spatial grid, so that agents navigate and interact with their world through natural physical cues rather than hardcoded scripts.*
* **Technical Tasks:**
  1. Implement `AffordanceBroker` system in Flecs ECS indexing world affordances into spatial hash buckets ($8 \times 8 \times 8\text{ dm}$).
  2. Define `AffordanceDescriptor` struct: required tools (`tool_flag`), minimum skill (`min_skill`), caloric cost rate ($\dot{C}$), focus drain rate ($\dot{F}$), emotional valence outcome, and spatial interaction transform.
  3. Attach affordances to physical voxels and equipment (e.g., rocket mass heaters emit warmth affordances, mess hall pots emit stew eating affordances).
  4. Implement autonomous utility scoring: agents evaluate nearby affordances based on personality facets, current fatigue, and core values.
* **Acceptance Criteria:**
  * **Given** outside ambient temperature drops to $-5^\circ\text{C}$ and Founder 01 is shivering,
  * **When** the rocket mass heater at coordinate $(20, 10, 15)$ is fired,
  * **Then** the heater emits `[Affordance: Warmth]` across a 6-meter radius,
  * **And** Founder 01 autonomously navigates toward the heater without direct player commands.

---

#### Story 2.4: Logistic Moral Rejection Heuristics & Visual Plumbob Telegraph
* **ID:** `STORY-2.4` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G4
* **User Story:** *As Founder 01, I want to morally refuse player directives that violate my core philosophical convictions following a logistic rejection curve, telegraphing my refusal through the Plumbob 2.0, so that I cannot be commanded to commit unethical acts.*
* **Technical Tasks:**
  1. Implement 3-tier Directive Gating:
     * Tier 1: Breakdown Check ($\mathcal{S} > 50,000 \implies$ reject).
     * Tier 2: Physiological Gating (hydration $< 10\%$ or fatigue $> 90\% \implies$ defer to biological survival).
     * Tier 3: Core Value Alignment Gating.
  2. Implement Logistic Moral Rejection Curve:
     $$P(\text{Rejection}) = \frac{1}{1 + \exp\left(-k \cdot (\text{MoralConflict} + \text{StressDelta} - \text{Trust} - \theta)\right)}$$
     where $\text{MoralConflict} = \sum (\text{DirectiveImpact}_i \cdot \text{CoreValues}_i)$.
  3. Implement Visual & Narrative Telegraph: on refusal, Founder 01 drops the tool, turns toward the camera, and the Plumbob 2.0 flashes jagged crimson strobing (4.0 Hz); narrative toast displays refusal rationale (e.g., *"Founder 01 refuses: 'I will not poison the soil we eat from.'"*).
* **Acceptance Criteria:**
  * **Given** Founder 01 possesses `STEWARDSHIP = +85` and `SOLIDARITY = +70`,
  * **When** the player issues a directive to dump toxic battery runoff into the common well,
  * **Then** $P(\text{Rejection}) > 0.99$, Founder 01 drops the bucket, Plumbob 2.0 strobes crimson,
  * **And** the directive is purged from the queue with an explicit ethical refusal message.

---

#### Story 2.5: Anti-Lockup Hysteresis, Action Commit & 30-Tick Watchdog
* **ID:** `STORY-2.5` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 4 | **Quality Gate:** G4
* **User Story:** *As the gameplay AI system, I want 25% utility hysteresis, atomic action commits, distance-adaptive TTL leases, and a 30-tick progress watchdog, so that agents never enter erratic decision flickers or pathfinding deadlocks.*
* **Technical Tasks:**
  1. Implement 25% Utility Hysteresis: agent executing task $A$ switches to candidate $B$ only if $U(B) \ge 1.25 \cdot U(A)$.
  2. Implement Atomic Action Commit: once physical execution (welding, sawing, pouring) begins, interruptions from non-critical whims are locked out until the phase finishes.
  3. Implement Distance-Adaptive TTL Lease with 15-tick heartbeat transit renewal: $\text{TTL}_{\text{init}} = \max\left(60, \left\lceil \frac{d}{\text{speed}} \right\rceil \times 1.5\right)\text{ ticks}$. Halting movement allows the lease to cleanly expire.
  4. Implement 30-Tick Progress Watchdog: if distance to goal fails to decrease over 30 consecutive ticks ($1.0\text{ s}$ at 30 Hz), transition task to `ACTION_BLOCKED`, step back 1 dm, release locks, and blacklist destination with exponential backoff ($30\text{s}, 60\text{s}, 120\text{s}, 300\text{s}$).
* **Acceptance Criteria:**
  * **Given** an agent pathfinding to a tool locker is blocked by a closed door,
  * **When** distance to target fails to decrement for 30 consecutive ticks,
  * **Then** the watchdog fires, releasing the locker lease without deadlocking,
  * **And** the destination is blacklisted for 30 seconds while the agent picks an alternate affordance.

---

### Epic 3: Cities: Skylines Macro Infrastructure Leeching & Multi-Biome Simulation (Macro Infrastructure Leeching)
**Epic ID:** `EPIC-3` | **Total Estimate:** 42 Story Points | **Priority:** P1 High  
**Focus:** Implement macro infrastructure leeching (12kV grid lines, water mains, asphalt/urbanite, dark fiber), smart meter anomaly counters, Successive Over-Relaxation (SOR, $\omega = 1.6$) electrical solver with virtual earth grounding, 3 biomes (Suburban, Urban, Permaculture), 128-chunk active sector visibility window, and the 3 Bootstrapping Epochs.

---

#### Story 3.1: 12kV Power Grid Siphoning & Smart Meter Anomaly Counter
* **ID:** `STORY-3.1` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G1, G6
* **User Story:** *As Founder 01, I want to climb utility poles at night to attach inductive line clamps to a 240V drop, siphoning power while managing the utility company's smart meter anomaly counter, so that I can charge pirate battery banks without triggering municipal code raids.*
* **Technical Tasks:**
  1. Implement utility pole voxel attachment interaction: requires wire strippers, insulated clamps, and nocturnal darkness.
  2. Model exergy yield: delivers up to $7.2\text{ kW}$ electrical power into connected battery banks.
  3. Implement Smart Meter Anomaly Counter: drawing $> 30\text{ A}$ creates thermal signatures; anomaly counter increments $+1\%$ per 100 ticks; reaching $100\%$ triggers an armed municipal inspection raid. Drawing $< 10\text{ A}$ safely dissipates the counter at $-0.5\%$ per 100 ticks.
  4. Model arc flash hazard: improper installation during rain triggers an electrical explosion, destroying equipment and injuring the agent.
* **Acceptance Criteria:**
  * **Given** Founder 01 siphons $35\text{ A}$ from an overhead drop for 10,000 simulation ticks,
  * **When** anomaly counter increments past $100\%$,
  * **Then** an adversarial inspection event is scheduled by Layer 7,
  * **And** throttling current draw to $8\text{ A}$ dissipates the anomaly counter cleanly back to $0\%$, satisfying Gate G1.

---

#### Story 3.2: Successive Over-Relaxation (SOR) Electrical Flow Solver
* **ID:** `STORY-3.2` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G1
* **User Story:** *As the electrical simulation system, I want an SOR flow solver with relaxation factor $\omega = 1.6$ and virtual earth grounding, so that high-contrast conductance networks converge in $< 20$ iterations without floating matrix singularities.*
* **Technical Tasks:**
  1. Formulate nodal admittance matrix equation: $\mathbf{G}\mathbf{V} = \mathbf{I}$ across grid lines, pirate clamps, inverters, and battery banks.
  2. Implement Successive Over-Relaxation (SOR) algorithm with optimal relaxation factor $\omega = 1.6$:
     $$V_i^{(k+1)} = (1 - \omega) V_i^{(k)} + \frac{\omega}{G_{ii}} \left( I_i - \sum_{j < i} G_{ij} V_j^{(k+1)} - \sum_{j > i} G_{ij} V_j^{(k)} \right)$$
  3. Handle extreme conductance contrast between low-impedance copper lines ($R \approx 0.005\ \Omega, g \approx 200\text{ S}$) and high-impedance battery drops ($R \approx 8.0\ \Omega, g \approx 0.125\text{ S}$).
  4. Implement virtual earth ground node ($g_{\text{virtual\_earth}} = 10^{-6}\text{ S}$) to guarantee $\det(\mathbf{G}) \neq 0$ during islanding and off-grid transitions.
  5. Implement direct Sparse Cholesky fallback ($< 0.12\text{ ms}$) if SOR fails to converge within 50 iterations.
* **Acceptance Criteria:**
  * **Given** an electrical network with 120 nodes and conductance contrast ratio of $1,600:1$,
  * **When** the SOR solver executes with $\omega = 1.6$,
  * **Then** voltage residuals converge below $\epsilon = 10^{-4}$ in $< 20$ iterations (execution time $< 0.45\text{ ms}$),
  * **And** disconnecting the external grid leaves the virtual ground active with zero matrix singularity crashes.

---

#### Story 3.3: Municipal Water, Ductile Iron Tapping & Telecom Dark Fiber Scavenging
* **ID:** `STORY-3.3` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 3 | **Quality Gate:** G1, G2
* **User Story:** *As Founder 01, I want to excavate 2 meters to clamp a saddle tap to a ductile iron water main, divert stormwater runoff into biochar reed beds, and splice roadside dark fiber conduits, so that our settlement secures abundant water and feeds scavenged high-speed optical bandwidth directly into Sovereign Stack Layer 2 `col-meshd` mesh routing.*
* **Technical Tasks:**
  1. Implement trench excavation through asphalt/dirt voxels to locate 4-inch ductile iron water mains.
  2. Implement hot-tap saddle valve installation affordance; provides pressurized water flow ($35\text{ PSI}$, up to $25\text{ L/min}$).
  3. Implement pressure drop detection: tapping $> 40\text{ L/min}$ triggers municipal SCADA pressure alerts.
  4. Implement stormwater diversion swales routing street runoff through gravel, mycelium bio-filtration, and biochar reed beds (Scenario Mu) to remove heavy metals and hydrocarbons.
  5. Implement roadside telecom dark fiber tapping and optical mechanical splicing affordance: locate subterranean fiber conduits in utility trenches, execute non-destructive optical tapping, and route scavenged packets directly into Sovereign Stack Layer 2 `col-meshd` mesh routing daemon for encrypted inter-node transport and Layer 6 semantic caching, while actively monitoring for carrier OTDR backscatter reflection spikes to evade ISP line fault sweeps.
* **Acceptance Criteria:**
  * **Given** a 4-inch water main voxel at depth $z = -20\text{ dm}$,
  * **When** Founder 01 clamps a saddle tap and opens the valve,
  * **Then** domestic cisterns fill at $20\text{ L/min}$ with zero SCADA alarm trigger,
  * **And** diverting untreated storm runoff into food crops without biochar filtration contaminates soil voxels,
  * **And** tapping dark fiber conduits establishes an optical data link routed into Sovereign Stack Layer 2 `col-meshd` with $< 1\text{ ms}$ packet serialization delay without triggering carrier OTDR reflection alerts.

---

#### Story 3.4: Asphalt Jackhammering & Urbanite Concrete Quarrying
* **ID:** `STORY-3.4` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 3 | **Quality Gate:** G1, G4
* **User Story:** *As an agent, I want to jackhammer degraded asphalt parking lots and cut reinforced concrete rebar to quarry "Urbanite" stone, so that we obtain sturdy foundation materials without purchasing commercial masonry.*
* **Technical Tasks:**
  1. Implement jackhammer and pickaxe quarrying mechanics on asphalt (`material_id = 8`) and concrete (`material_id = 9`) voxels.
  2. Generate **Urbanite** inventory items (broken concrete blocks, $25\text{ kg}$ per chunk) and scrap rebar steel.
  3. Model physical fatigue and somatic hazards: quarrying without dust masks generates silica dust and bitumen fumes, injecting lung irritation thoughts and accelerating fatigue.
  4. Allow Urbanite to be laid as foundation footings, rocket stove thermal mass, and retaining wall voxels.
* **Acceptance Criteria:**
  * **Given** an asphalt road chunk adjacent to Lot 402,
  * **When** Founder 01 operates an electric jackhammer for 4 in-game hours,
  * **Then** 8 road voxels are converted to depaved living soil, yielding 24 Urbanite blocks,
  * **And** if unmasked, Founder 01 incurs the `THOUGHT_SILICA_COUGH` negative thought, dropping focus by 15.

---

#### Story 3.5: Multi-Biome Simulation Profiles (Suburban, Urban, Permaculture)
* **ID:** `STORY-3.5` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Sprint 3 | **Quality Gate:** G1, G2
* **User Story:** *As the simulation engine, I want distinct biome profiles for Suburban Sprawl, Dense Urban Blocks, and Permaculture Eco-Villages, so that players face unique spatial envelopes, materials, and adversarial pressures.*
* **Technical Tasks:**
  1. Define Suburban Sprawl profile ($128 \times 128 \times 16\text{ dm}$): horizontal layout, overhead power lines, shallow water mains, turf grass, HOA drone inspections ($150/day fines).
  2. Define Dense Urban Block profile ($64 \times 64 \times 64\text{ dm}$): deep vertical canyons, high-voltage conduit vaults, steam tunnels, structural steel, building inspectors, FLIR thermal sweeps.
  3. Define Permaculture Eco-Village profile ($96 \times 96 \times 32\text{ dm}$): contoured topography, keyline swales, cob, biochar, downstream senior water rights enforcement.
  4. Configure procedural terrain generator to instantiate chosen biome profile upon new game boot.
* **Acceptance Criteria:**
  * **Given** the user selects the Dense Urban Block biome,
  * **When** the simulation world initializes,
  * **Then** vertical height limit expands to 64 decimeters with steel/concrete voxel palettes,
  * **And** adversarial pressure shifts from HOA lawn citations to municipal FLIR thermal sweeps.

---

#### Story 3.6: Spatial Chunk Memory & 128-Chunk Active Visibility Window
* **ID:** `STORY-3.6` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 3 | **Quality Gate:** G1, G4
* **User Story:** *As the memory management system, I want a 128-chunk active sector visibility window ($16.38\text{ MB}$) with hierarchical coarse simulation for outer chunks, so that expansive neighborhoods simulate smoothly within WASM's 256.00 MB ceiling.*
* **Technical Tasks:**
  1. Allocate active sector geometry: exactly 128 chunks @ 128 KB ($32^3$ voxels $\times$ 4B/voxel) = $16.38\text{ MB}$.
  2. Implement distance-sorted chunk paging centered on Founder 01: core 128 chunks execute full-fidelity thermodynamics, moisture diffusion, and voxel physics.
  3. Outer chunks in the $8 \times 8 \times 4$ sector (remaining 128 chunks) simulate using compressed macro-block representations (aggregate temperature, moisture, exergy balance).
  4. Enforce zero runtime allocations during chunk streaming via pre-allocated chunk ring pools.
* **Acceptance Criteria:**
  * **Given** Founder 01 moves across a biome boundary,
  * **When** chunk streaming pages new terrain,
  * **Then** 128 active chunks remain resident in linear memory ($16.38\text{ MB}$),
  * **And** total WASM linear memory usage remains strictly bounded within the allocated 256.00 MB partition.

---

### Epic 4: Stochastic Boundary Interface & Sovereign Stack Multiplayer (Stochastic Boundary & Multiplayer)
**Epic ID:** `EPIC-4` | **Total Estimate:** 31 Story Points | **Priority:** P1 High  
**Focus:** Implement the Stochastic Boundary Interface via an inhomogeneous Poisson process, 3 attenuation pillars with Genesis Epsilon Guard ($\epsilon = 0.001$), multiplayer executed exclusively via Sovereign Stack (L2–L7) over UHAI IPC, cryptographic speed-of-light distance bounding, and isoperimetric perimeter consolidation.

---

#### Story 4.1: Inhomogeneous Poisson Arrival Process & Legacy Regimes
* **ID:** `STORY-4.1` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Sprint 3 | **Quality Gate:** G2, G3
* **User Story:** *As the boundary simulation system, I want hostile legacy shocks to arrive according to an inhomogeneous Poisson process modulated by geopolitical regimes, so that external pressure feels unpredictable yet systematically responsive to player actions.*
* **Technical Tasks:**
  1. Implement Poisson event generator: arrival intensity $\lambda(t) = \max\left(\lambda_{\min}, \lambda_{\mathcal{R}} \cdot \exp(-\Psi(t))\right)$.
  2. Define geopolitical regimes $\mathcal{R}(t)$: Sub-Radar ($\lambda_0 = 0.005$), Under-Scrutiny ($\lambda_0 = 0.02$), Adversarial ($\lambda_0 = 0.08$), Active-Siege ($\lambda_0 = 0.25$).
  3. Enforce irreducible legacy threat floor: $\lambda_{\min} = 0.001\text{ s}^{-1}$ (approx. 1 event per 16.6 minutes), ensuring off-grid vigilance is never extinguished.
  4. Schedule stochastic boundary events (code raids, utility shutoffs, tax liens, EPA water inspections) when the Poisson timer expires.
* **Acceptance Criteria:**
  * **Given** the node is in the Under-Scrutiny regime with zero sovereign attenuation ($\Psi(t) = 0$),
  * **When** the simulation executes for 10,000 seconds,
  * **Then** boundary shock arrivals follow Poisson distribution with mean intensity $\lambda = 0.02\text{ s}^{-1}$,
  * **And** arrival rate never drops below the irreducible floor $\lambda_{\min} = 0.001\text{ s}^{-1}$.

---

#### Story 4.2: Three Attenuation Pillars & Genesis Epsilon Guard
* **ID:** `STORY-4.2` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Sprint 3 | **Quality Gate:** G1, G4
* **User Story:** *As Founder 01, I want our physical self-sufficiency, social NPC bonds, and peer mesh connections to exponentially attenuate boundary pressure shocks, with Genesis Epsilon guards preventing division-by-zero crashes on Day 1.*
* **Technical Tasks:**
  1. Implement Sovereign Attenuation Potential:
     $$\Psi(t) = \alpha \cdot \Phi_{\text{infrastructure}}(t) + \beta \cdot \Phi_{\text{social}}(t) + \gamma \cdot \tanh(\delta \cdot \Phi_{\text{mesh}}(t))$$
  2. Implement Pillar 1 (Infrastructure) with Genesis Epsilon Guard ($\epsilon = 0.001\text{ kWh/L}$):
     $$\Phi_{\text{infrastructure}}(t) = \frac{1}{3} \left[ \min\left(1.0, \frac{E_{\text{solar}} + \epsilon}{E_{\text{consumed}} + \epsilon}\right) + \min\left(1.0, \frac{W_{\text{rain}} + \epsilon}{W_{\text{consumed}} + \epsilon}\right) + \frac{A_{\text{depaved}}}{A_{\text{total}}} \right]$$
     Preventing $0/0 \implies \text{NaN}$ singularities on Day 1 when consumption is zero.
  3. Implement Pillar 2 (Social NPC Integration):
     $$\Phi_{\text{social}}(t) = \ln\left(1 + \sum_{k=1}^{N} \frac{\text{Trust}_k \cdot \text{Hours}_k}{100}\right)$$
  4. Implement Pillar 3 (Peer Mesh Proximity) with bounded $\tanh$ weighting:
     $$\Phi_{\text{mesh}}(t) = \frac{1}{|\mathcal{P}|_{\max}} \sum_{j \in \mathcal{P}} \frac{\text{TrustScore}_{ij}}{1 + (d_{ij}/d_0)^2}$$
* **Acceptance Criteria:**
  * **Given** a new game spawns on Day 1 with 0 kWh solar stored and 0 kWh consumed,
  * **When** $\Phi_{\text{infrastructure}}$ is evaluated,
  * **Then** the Genesis Epsilon Guard yields $\eta = 1.0$ without NaN or floating-point exception,
  * **And** building 100% solar autarky, welcoming 5 trusted citizens, and meshing with 3 peer nodes reduces hostile arrival frequency by $> 85\%$.

---

#### Story 4.3: Sovereign Stack Exclusive Multiplayer Protocol Matrix
* **ID:** `STORY-4.3` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 3 | **Quality Gate:** G2, G3, G6
* **User Story:** *As an Oasis player, I want multiplayer interactions to route exclusively through the Sovereign Stack over UHAI IPC without any embedded P2P code in the game client, maintaining strict architectural separation.*
* **Technical Tasks:**
  1. Enforce Anti-Bleed Invariant 3: verify zero libp2p, zero WebRTC sockets, zero LoRa decoders in the Oasis engine binary.
  2. Route multiplayer packet dispatch through Sovereign Stack Layer 2 (`col-meshd`) and Layer 3 (`col-storaged`) via UHAI IPC:
     * L2: Reticulum Network Stack (RNS), LoRa, 802.11s mesh transport.
     * L3: Automerge/Yrs CRDT state replication, Kademlia DHT, IPLD blockstore.
     * L4: W3C DIDs (`did:peer`), BBS+ Verifiable Credentials.
     * L5: Zero-sum mutual credit clearing (BCC) and smart covenants.
  3. Implement game client multi-agent proxy entities synchronized from Layer 3 CRDT state snapshots.
* **Acceptance Criteria:**
  * **Given** two Oasis clients running on separate local machines,
  * **When** peer trade and chat occur,
  * **Then** all network packets route through the external `col-meshd` daemon,
  * **And** binary inspection of the Oasis client confirms zero embedded P2P or networking libraries, satisfying Anti-Bleed Invariant 3.

---

#### Story 4.4: Cryptographic Speed-of-Light Distance Bounding
* **ID:** `STORY-4.4` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 4 | **Quality Gate:** G2, G3
* **User Story:** *As the peer discovery protocol, I want physical distance between settlement nodes verified via cryptographic speed-of-light distance bounding, so that remote GPS-spoofing attackers cannot falsify local peer proximity.*
* **Technical Tasks:**
  1. Implement cryptographic challenge-response distance bounding protocol:
     $$d_{AB} \le \frac{c \cdot (\Delta t - t_d)}{2}$$
     where $c \approx 3 \times 10^8\text{ m/s}$, $\Delta t$ is round-trip time, and $t_d \le 1.0\text{ ns}$ is hardware turnaround.
  2. Require high-precision timestamping via PHY/MAC hardware layer in `col-meshd` (Layer 2).
  3. Reject peer mesh proximity claims if round-trip latency implies physical distance $> 50\text{ km}$.
* **Acceptance Criteria:**
  * **Given** an attacker attempts to spoof a neighbor node using a WAN VPN with $40\text{ ms}$ latency,
  * **When** the distance bounding challenge executes,
  * **Then** the calculated distance exceeds $6,000\text{ km}$,
  * **And** the node is rejected from the local proximity mesh and granted zero local credit line, satisfying Gate G3.

---

#### Story 4.5: Isoperimetric Perimeter Consolidation & Boundary Friction
* **ID:** `STORY-4.5` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 4 | **Quality Gate:** G2, G4
* **User Story:** *As neighboring allied settlements, I want merging our parcel borders to dissolve internal property lines and reduce exposed perimeter by 25%, establishing an open Commons Corridor that suppresses hostile municipal raids.*
* **Technical Tasks:**
  1. Implement Isoperimetric Perimeter Consolidation: when two adjacent square parcels of area $A$ federate along a shared boundary edge, dissolve the interior fence line, reducing total exposed perimeter from $8\sqrt{A}$ to $6\sqrt{A}$ (an immediate $25\%$ reduction).
  2. Implement Bounded Boundary Friction Coefficient ($\kappa \in [0.05, 1.0]$):
     $$\kappa = \kappa_{\text{base}} \cdot \prod_{j} \max\left(0, 1 - \alpha \frac{R_j}{\max(d_j, d_{\min})}\right) \cdot (1 - \beta P_{\text{buffer}}) \cdot (1 - \tanh(\gamma N_{\text{citizens}}))$$
  3. Ensure the term $(1 - \tanh(\gamma N_{\text{citizens}}))$ prevents negative friction sign-inversion bugs.
  4. Dissolve the volumetric Fog of Legacy along the consolidated boundary into a verdant **Commons Corridor**.
* **Acceptance Criteria:**
  * **Given** Lot 402 and Lot 403 establish a ratified mutual-aid covenant,
  * **When** perimeter consolidation executes,
  * **Then** the interior dividing fence is deleted, exposed perimeter drops by $25\%$,
  * **And** boundary friction $\kappa$ drops toward $0.05$, reducing municipal raid frequency along that boundary by $95\%$.

---

### Epic 5: Scenario Driven Development (SDD) Dynamic Ingestion & In Silico Compiler (SDD Dynamic Ingestion & Compiler)
**Epic ID:** `EPIC-5` | **Total Estimate:** 40 Story Points | **Priority:** P0 Critical  
**Focus:** Implement the dynamic scenario ingestion pipeline for all 24 canonical MVS scenarios (`docs/scenarios/*.md`), JSON-LD schema parsing, dynamic Flecs ECS data binding, spatial voxel palette allocation, Gibsonian affordance generation, upward compilation loop, and automated G1–G6 verification harness.

---

#### Story 5.1: Dynamic Scenario Ingestion Pipeline & Schema Parser
* **ID:** `STORY-5.1` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G2, G4
* **User Story:** *As the Oasis simulation engine, I want to dynamically ingest scenario packages (`docs/scenarios/*.md` or `.jsonld`) at runtime without C++ recompilation, so that new survival scenarios load seamlessly into the game.*
* **Technical Tasks:**
  1. Implement zero-copy scenario parser in C++20 reading markdown frontmatter and embedded JSON-LD schema blocks (`bounty.jsonld`, `attestation.json`).
  2. Validate scenario cryptographic hash against known IPLD Content Identifier (CID).
  3. Parse Section 1 (Metadata), Section 2 (Problem & Thermodynamic Bounds), Section 3 (Schemas), Section 4 (Engine Blueprint), and Section 5 (Acceptance Test Gates).
  4. Instantiate scenario descriptor into memory without dynamic heap fragmentation.
* **Acceptance Criteria:**
  * **Given** `docs/scenarios/alpha_fabrication.md` is loaded by the engine,
  * **When** `ScenarioIngestionPipeline::load_scenario()` executes,
  * **Then** the scenario is validated against its CID hash in $< 15\text{ ms}$,
  * **And** schema definitions for `FabricationBounty` are parsed with zero memory leaks.

---

#### Story 5.2: Dynamic Flecs ECS Component Synthesis & Archetype Registration
* **ID:** `STORY-5.2` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G4
* **User Story:** *As the ECS runtime, I want scenario data pods registered as dynamic Flecs component IDs and bound to entity archetypes at runtime, so that scenarios introduce custom state without modifying engine source code.*
* **Technical Tasks:**
  1. Use the Flecs C API dynamic component reflection to register scenario-defined packed structs (e.g., `PrinterComponent`, `VehicleComponent`, `ColdStorageComponent`).
  2. Ensure all dynamically synthesized components are packed with strict 4-byte or 8-byte alignment and zero pointer indirection.
  3. Bind synthesized components to target entity archetypes (e.g., tool lockers, cooktops, delivery vans).
  4. Verify that archetype table queries operate at full native DOD iteration speed ($< 2\text{ ns}$ per entity iteration).
* **Acceptance Criteria:**
  * **Given** Scenario Beta (`beta_transit.md`) declares `VehicleComponent` (32 bytes),
  * **When** the scenario is ingested at runtime,
  * **Then** Flecs registers `flecs::entity component = ecs.component<VehicleComponent>()`,
  * **And** vehicle entities queryable via `ecs.each<VehicleComponent>()` execute at native ECS iteration speed.

---

#### Story 5.3: Spatial Voxel Palette Allocation & Material Uniform Buffering
* **ID:** `STORY-5.3` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 2 | **Quality Gate:** G1
* **User Story:** *As the graphics and physics subsystems, I want scenario-defined materials assigned unique voxel `material_id` tags with physical properties uploaded to GPU uniform buffers, so that new materials render and conduct heat correctly.*
* **Technical Tasks:**
  1. Map scenario material definitions to 32-bit voxel palette indices:
     * Mat 15: `FAB_NODE`, Mat 16: `FILAMENT_SPOOL` (Scenario Alpha)
     * Mat 22: `COLD_STORAGE`, Mat 23: `INDUCTION_RANGE` (Scenario Gamma)
     * Mat 45: `WOOD_CORD` (Scenario Eta)
     * Mat 52: `CLEAN_BENCH` (Scenario Iota)
  2. Load physical parameters (density $\rho$, thermal conductivity $k$, electrical conductivity $\sigma$, moisture index $\mu$, combustibility) into the GPU `VoxelMaterialUniformBuffer`.
  3. Ensure voxel physics systems (SOR electrical flow, thermal diffusion) look up parameters from uniform arrays without branching.
* **Acceptance Criteria:**
  * **Given** cordwood voxels (`material_id = 45`) are stacked near a rocket stove,
  * **When** thermal diffusion ticks execute,
  * **Then** the wood heats up according to $k = 0.13\text{ W/m}\cdot\text{K}$ and moisture dries from $50\%$ to $< 18\%$,
  * **And** WebGPU shaders render the wood texture accurately via material ID palette lookup.

---

#### Story 5.4: Gibsonian Affordance Tree Generation & Cadastral Task Publishing
* **ID:** `STORY-5.4` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 4 | **Quality Gate:** G4
* **User Story:** *As Founder 01, I want scenario task bounties translated into environmental affordances and posted to the Cadastral Slate, so that community members can claim work autonomously.*
* **Technical Tasks:**
  1. Parse scenario JSON-LD `TaskSpecification` blocks and generate Gibsonian affordance trees in spatial grid buckets.
  2. Populate prerequisites (`min_skill`, `required_tools`), somatic costs ($\dot{C}, \dot{F}$), and psychological valence.
  3. Translate scenario bounties into binary `SpatialIntentPayload` packets posted to the Cadastral Slate bulletin board.
  4. Allow autonomous NPCs to evaluate and claim open tasks during the 06:00 Morning Circle.
* **Acceptance Criteria:**
  * **Given** Scenario Delta (`delta_childcare.md`) posts a 4-hour childcare supervision bounty,
  * **When** the 06:00 Morning Circle convenes,
  * **Then** the task appears on the Cadastral Slate offering 1.0 VC/hr credit parity,
  * **And** an NPC with high `empathy` claims the affordance autonomously, freeing parents for construction.

---

#### Story 5.5: Upward Compilation Loop (Simulation to Bare-Metal Deployment)
* **ID:** `STORY-5.5` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Sprint 4 | **Quality Gate:** G1–G6
* **User Story:** *As a community steward, I want policies and fixes stress-tested in Oasis simulation at 100x speed to compile upwards into production edge daemon configurations, so that in silico discoveries improve physical node operations.*
* **Technical Tasks:**
  1. Implement execution trace recorder capturing failure modes (e.g., childcare chore rotation collapse, inverter black-start overload).
  2. Implement policy refinement compiler: translate verified in-game workflow adjustments (e.g., rotation rebalance tokens, pre-charge delays) into canonical JSON-LD and BPMN 2.0 XML schemas.
  3. Export refined schemas byte-compatible with Sovereign Stack Layer 4 (`col-execd`) and Layer 6 (`col-commonsd`).
  4. Generate deployable daemon package configurations for physical edge nodes.
* **Acceptance Criteria:**
  * **Given** an inverter black-start sequence in Scenario Upsilon fails due to battery inrush current,
  * **When** the player tests and validates a 5-minute pre-charge delay policy in 100x simulation,
  * **Then** the engine exports the validated policy into `upsilon_shock.bpmn`,
  * **And** the compiled file deploys directly to `col-execd` on bare-metal hardware without schema errors.

---

#### Story 5.6: Automated Headless G1–G6 Quality Gate Verification Harness
* **ID:** `STORY-5.6` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 4 | **Quality Gate:** G1–G6
* **User Story:** *As the QA engineer, I want an automated headless SITL test harness that executes all 24 canonical scenarios against Quality Gates G1 through G6, so that zero regressions or physical law violations reach production.*
* **Technical Tasks:**
  1. Implement headless test runner compiling with Catch2 3.5.3: `test_scenario_runner --all-scenarios`.
  2. Implement automated gate assertion checkers:
     * G1: Thermodynamic Conservation ($\pm 10\%$ physical bounds, SOR error $< 10^{-4}$).
     * G2: Offline Autonomy (100% WAN disconnected execution, zero external pings).
     * G3: Byzantine & Sybil Immunity (spoofed sensors rejected, Sybil rings $> 3$ hops zero credit).
     * G4: State Machine Invariants (zero hanging escrows, 30-tick watchdog, SAGAs).
     * G5: Trojan Fiat Ingestion (Stripe payloads compile without identity leakage).
     * G6: Terminal Decoupling (complete severance verification in Scenario Omega).
  3. Integrate test harness into CI/CD pipeline, failing builds if any scenario fails a gate.
* **Acceptance Criteria:**
  * **Given** all 24 canonical scenarios in `docs/scenarios/*.md`,
  * **When** `test_scenario_runner` executes in headless mode,
  * **Then** all 24 scenarios pass Gates G1 through G6 in $< 60\text{ seconds}$ total execution time,
  * **And** zero memory leaks, zero hanging escrows, and zero NaN values are detected.

---

### Epic 6: High-Performance Engine Core & WebGPU Amanatides & Woo Raymarcher (High-Performance Engine Core)
**Epic ID:** `EPIC-6` | **Total Estimate:** 34 Story Points | **Priority:** P0 Critical  
**Focus:** Implement C++20 Flecs ECS foundation, two-level hierarchical DDA compute raymarcher in WebGPU WGSL, 7.40 ms frame budget (10.60 ms hard cap, 60 FPS), rigid 256.00 MB WASM linear memory partition table (zero runtime allocations), and cross-platform build pipeline (CMake, Ninja, LLVM 18, emsdk 3.1.56, vcpkg).

---

#### Story 6.1: Two-Level Hierarchical DDA Compute Raymarcher in WGSL
* **ID:** `STORY-6.1` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G1
* **User Story:** *As an Oasis player, I want the volumetric voxel world rendered via an Amanatides & Woo 3D DDA compute shader accelerated by a coarse macro-chunk grid, so that the engine sustains a locked 60 FPS in standard web browsers.*
* **Technical Tasks:**
  1. Implement Amanatides & Woo (1987) grid traversal algorithm in WebGPU WGSL compute shaders.
  2. Implement Two-Level Hierarchical DDA: generate a coarse macro-chunk occupancy grid ($1\text{ bit per } 16 \times 16 \times 16$ block, 8 KB VRAM per sector).
  3. Accelerate ray traversal: primary rays skip empty $16^3$ air voids, reducing average ray steps from $\sim 45$ to $< 12$, and worst-case steps from 180 to $\le 36$.
  4. Bound total texture lookups to $< 24\text{M}$ per frame, executing full raymarching in $2.60\text{ ms} - 3.80\text{ ms}$.
* **Acceptance Criteria:**
  * **Given** a 128-chunk active sector rendered at $1920 \times 1080$ resolution,
  * **When** the DDA compute raymarcher executes in WebGPU,
  * **Then** compute traversal time is strictly $\le 3.80\text{ ms}$ on standard modern GPUs,
  * **And** primary ray steps average $< 12$ steps per pixel across open outdoor environments.

---

#### Story 6.2: 7.40 ms Active Frame Budget & Deferred PBR Pipeline
* **ID:** `STORY-6.2` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G1
* **User Story:** *As the graphics pipeline, I want the entire rendering pass structured within a 7.40 ms active frame budget (10.60 ms hard cap), so that 60 FPS is guaranteed with $> 36\%$ safety headroom.*
* **Technical Tasks:**
  1. Profile and enforce active frame phase allocations:
     * Phase 1: CPU Uniform Prep & Culling: $1.20\text{ ms}$
     * Phase 2: DDA Compute Raymarch: $3.80\text{ ms}$
     * Phase 3: Two-Phase Deferred PBR Lighting: $1.80\text{ ms}$
     * Phase 4: Cadastral CAD & Fog of Legacy Pass: $0.60\text{ ms}$
     * **Total Active Frame Time:** $7.40\text{ ms}$ ($10.60\text{ ms}$ hard budget cap).
  2. Implement two-phase deferred PBR shader: evaluate ambient occlusion, voxel emissive radiance (molten metal, hearths), and directional sunlight in G-buffer screen space.
  3. Implement frame timer watchdog: if active frame exceeds $10.60\text{ ms}$, dynamically reduce ray step caps for distant peripheral chunks.
* **Acceptance Criteria:**
  * **Given** an intense simulation scene with 50 active agents and multiple light sources,
  * **When** frame times are measured over 1,000 consecutive frames,
  * **Then** 99th percentile active frame time remains $\le 7.40\text{ ms}$,
  * **And** zero frames breach the $16.66\text{ ms}$ vsync threshold, sustaining a rock-solid 60 FPS.

---

#### Story 6.3: 256.00 MB WASM Linear Memory Partitioning (Zero Runtime Allocations)
* **ID:** `STORY-6.3` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G4
* **User Story:** *As the engine runtime, I want linear WebAssembly memory strictly partitioned into exactly 268,435,456 bytes (256.00 MB) with zero runtime heap allocations, so that the simulation runs indefinitely without memory fragmentation or browser OOM crashes.*
* **Technical Tasks:**
  1. Codify the static 256.00 MB WASM memory reconciliation table:
     * Partition 1: Active Sector Voxel Geometry (128 chunks @ 128 KB): $16.38\text{ MB}$
     * Partition 2: Voxel Macro-Chunk Grid (Coarse DDA): $32.00\text{ MB}$
     * Partition 3: Material Properties & Textures: $16.00\text{ MB}$
     * Partition 4: Flecs ECS World & Storage Tables: $32.00\text{ MB}$
     * Partition 5: Agent Psychology & Memory Rings ($64\text{B} + 768\text{B}$): $8.00\text{ MB}$
     * Partition 6: UHAI Lock-Free Ring Buffers: $4.00\text{ MB}$
     * Partition 7: Audio Waveform & Somatic Cache: $32.00\text{ MB}$
     * Partition 8: Cadastral CAD & Mesh Buffers: $16.00\text{ MB}$
     * Partition 9: Emscripten Heap & Thread Stacks: $32.00\text{ MB}$
     * Partition 10: Reserved Safety Cushion (Staging Buffers): $67.62\text{ MB}$
     * **Total Static Sum:** Exactly $256.00\text{ MB}$ ($268,435,456\text{ bytes}$).
  2. Implement custom linear arena allocators for each partition.
  3. Install memory hooks asserting zero calls to `malloc()`, `calloc()`, or `operator new()` inside the 30 Hz simulation tick loop.
* **Acceptance Criteria:**
  * **Given** the Oasis WASM client running in Chrome/Firefox for 24 continuous hours,
  * **When** memory usage is monitored via browser devtools,
  * **Then** total linear memory remains pinned at exactly $256.00\text{ MB}$ ($0\text{ bytes}$ heap drift),
  * **And** running the simulation tick loop asserts zero runtime heap allocation violations.

---

#### Story 6.4: Cross-Platform Build Pipeline & Toolchain Manifest
* **ID:** `STORY-6.4` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G2
* **User Story:** *As a developer, I want a unified CMake 3.28+ build pipeline supporting native macOS (Homebrew), native Linux (APT), and WebAssembly (emsdk 3.1.56), so that identical code compiles cleanly across all platforms.*
* **Technical Tasks:**
  1. Author `CMakeLists.txt` enforcing `-std=c++20`, strict compiler warnings (`-Wall -Wextra -Werror -Wconversion -Wshadow`), and SIMD optimizations (`-march=armv8-a` / `-mavx2`).
  2. Configure `vcpkg.json` manifest mode for cross-platform dependencies: Flecs 3.2.11, GLM 1.0.1, spdlog 1.14.1, nlohmann-json 3.11.3, Catch2 3.5.3, SDL2 2.30.2.
  3. Configure Emscripten toolchain flags: `-sUSE_PTHREADS=1`, `-sUSE_WEBGPU=1`, `-sINITIAL_MEMORY=268435456`, `-sMAXIMUM_MEMORY=268435456`, `-sALLOW_MEMORY_GROWTH=0`, `-sEXPORTED_RUNTIME_METHODS=ccall,cwrap`.
  4. Validate builds on macOS Apple Silicon, Ubuntu 22.04 LTS, and WASM.
* **Acceptance Criteria:**
  * **Given** a clean clone of the repository,
  * **When** running `cmake -B build -G Ninja && cmake --build build` on macOS or Linux,
  * **Then** compilation completes with zero warnings and zero errors,
  * **And** compiling via `emcmake cmake -B build-wasm` produces valid `.wasm` and `.js` artifacts.

---

#### Story 6.5: Cadastral Axonometric Slate & Dual Viewport Camera System
* **ID:** `STORY-6.5` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 4 | **Quality Gate:** G4
* **User Story:** *As a player, I want to seamlessly toggle between a tactile 3rd-person founder camera and an axonometric Cadastral Slate blueprint view, so that I can easily plan construction and monitor exergy networks.*
* **Technical Tasks:**
  1. Implement Dual Viewport System:
     * Camera Mode 1: 3rd-Person Follow Camera (over-the-shoulder, tactile kinematics, tool swings, dynamic weather).
     * Camera Mode 2: Cadastral Axonometric Slate (orthographic isometric projection, blueprint vectors, exergy/water overlays, threat vectors).
  2. Implement smooth 400 ms cubic Hermite transition curve maintaining focal point alignment.
  3. Render Cadastral CAD line vectors and blueprint stamps using WebGPU line primitive pipelines.
* **Acceptance Criteria:**
  * **Given** the player is in 3rd-person follow mode,
  * **When** the player presses `Tab` or clicks the Slate icon,
  * **Then** the camera transitions smoothly over 400 ms to the axonometric slate view without visual pop-in,
  * **And** blueprint construction overlays display exergy conduits and water pipes accurately.

---

### Epic 7: Universal Human-Agent Interface (UHAI) Shared-Memory IPC & SITL Telemetry (UHAI IPC & Telemetry)
**Epic ID:** `EPIC-7` | **Total Estimate:** 30 Story Points | **Priority:** P0 Critical  
**Focus:** Implement the lock-free Single-Producer Single-Consumer (SPSC) shared memory ring buffer at `/dev/shm/uhai_telemetry.ring` (< 50 µs latency, 3 cache line header isolation), Emscripten browser adapter (`SharedArrayBuffer`), strict enforcement of 5 Anti-Bleed Invariants, and physical airgap isolation between `QUALITY_SIMULATED` and `QUALITY_HARDWARE_ROOT`.

---

#### Story 7.1: Lock-Free SPSC Shared Memory Ring Buffer in `/dev/shm`
* **ID:** `STORY-7.1` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G6
* **User Story:** *As the UHAI IPC subsystem, I want a lock-free SPSC shared memory ring buffer in `/dev/shm` delivering $< 50\ \mu\text{s}$ latency with 3-cache-line header isolation, so that the Oasis engine exchanges telemetry with Sovereign Stack daemons without lock contention.*
* **Technical Tasks:**
  1. Define `SharedRingHeader` struct occupying exactly 3 cache lines (192 bytes):
     * Cache line 0: `producer_write_index` (atomic uint32_t) & `dropped_frames_counter`.
     * Cache line 1: `consumer_read_index` (atomic uint32_t).
     * Cache line 2: Static ring configuration (slot count, magic number, protocol version).
  2. Define `TelemetrySlot` struct padded to 64 bytes (1 cache line), holding a 32-byte packed `TelemetrySample` struct (timestamp, sensor ID, Q16.16 measurement, quality flag).
  3. Pre-allocate 1,024 slots ($1,024 \times 64 = 65,536\text{ B}$). Total SHM file size: exactly 65,728 bytes.
  4. Create `/dev/shm/uhai_telemetry.ring` with POSIX `0660` permissions.
  5. Implement lock-free atomic push and pop operations using acquire-release memory order semantics.
* **Acceptance Criteria:**
  * **Given** Oasis producing telemetry and `col-telemetryd` consuming at 1,000 Hz,
  * **When** IPC latency is measured over 100,000 samples,
  * **Then** 99.9th percentile latency is strictly $< 50\ \mu\text{s}$,
  * **And** cache-line separation eliminates false sharing, verifying zero CPU core cache stalls.

---

#### Story 7.2: Emscripten Browser Transport Adapter (`SharedArrayBuffer`)
* **ID:** `STORY-7.2` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G2, G6
* **User Story:** *As the browser client, I want the UHAI ring buffer mapped to `SharedArrayBuffer` using background Web Workers with `Atomics.wait()` and `Atomics.notify()`, so that telemetry streaming never introduces UI jitter on the main thread.*
* **Technical Tasks:**
  1. Implement Emscripten bridge mapping `/dev/shm` memory semantics to `SharedArrayBuffer` in WebAssembly.
  2. Spawn a dedicated background Web Worker for UHAI IPC transport.
  3. Use JavaScript `Atomics.wait()` and `Atomics.notify()` for low-latency thread coordination without busy-waiting.
  4. Stream telemetry samples to/from the main WebAssembly thread via zero-copy shared memory views.
* **Acceptance Criteria:**
  * **Given** Oasis running in a web browser with `Cross-Origin-Opener-Policy: same-origin` and `Cross-Origin-Embedder-Policy: require-corp`,
  * **When** UHAI telemetry streams at 100 Hz,
  * **Then** zero garbage collection spikes or main-thread frame drops occur,
  * **And** message latency remains strictly $< 1.0\text{ ms}$ inside the browser sandbox.

---

#### Story 7.3: Enforcement of the 5 Anti-Bleed Invariants Manifest
* **ID:** `STORY-7.3` | **Points:** 5 SP | **Priority:** P0 | **Sprint:** Sprint 1 | **Quality Gate:** G6
* **User Story:** *As the system architect, I want continuous static analysis and runtime assertion checks enforcing the Five Anti-Bleed Invariants, so that game code never couples with Sovereign Stack daemons.*
* **Technical Tasks:**
  1. Author static architecture linter rule asserting zero symbols from `col-*` daemons in the Oasis client binary (Anti-Bleed Inv 1).
  2. Assert zero Ed25519 or BBS+ private key representations in Oasis structs; verify Oasis holds only opaque 32-byte `founder_did_handle` (Anti-Bleed Inv 2).
  3. Assert zero libp2p, WebRTC, or LoRa symbols in Oasis (Anti-Bleed Inv 3).
  4. Assert zero XML parsing libraries (`pugixml`, `tinyxml`) in Oasis; verify spatial intents emit as binary `SpatialIntentPayload` (Anti-Bleed Inv 4).
  5. Assert zero fiat currency symbols (`$`, `USD`, `fiat_ledger`) in the simulation core; verify simulation handles pure physical exergy units (Anti-Bleed Inv 5).
* **Acceptance Criteria:**
  * **Given** the compiled Oasis binary and source code,
  * **When** the architectural audit script `scripts/verify_anti_bleed.sh` runs,
  * **Then** all 5 invariants pass with zero violations,
  * **And** any pull request introducing bleeding dependencies fails CI automatically.

---

#### Story 7.4: Hardware-in-the-Loop Isolation & Airgap Invariant
* **ID:** `STORY-7.4` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 4 | **Quality Gate:** G6
* **User Story:** *As the edge security engineer, I want synthetic telemetry strictly tagged `QUALITY_SIMULATED` (0x01) and barred from physical hardware actuation, so that simulation bugs or testing hacks can never damage physical node hardware.*
* **Technical Tasks:**
  1. Define telemetry quality flags in `TelemetrySample`:
     * `QUALITY_SIMULATED = 0x01` (emitted by Oasis or SITL test harnesses).
     * `QUALITY_HARDWARE_ROOT = 0x02` (emitted by physical hardware secure elements).
  2. Implement hardware airgap filter in `col-telemetryd` (Layer 1): all physical relays, breakers, and solenoids strictly require `QUALITY_HARDWARE_ROOT` signed by Layer 4 execution keys.
  3. Verify that any packet tagged `QUALITY_SIMULATED` attempting to trigger a physical GPIO pin or Modbus register is immediately dropped and logged as a security violation.
* **Acceptance Criteria:**
  * **Given** an Oasis SITL test harness injecting synthetic inverter trip commands tagged `QUALITY_SIMULATED`,
  * **When** the packet reaches `col-telemetryd`,
  * **Then** the hardware daemon rejects the actuation command,
  * **And** physical circuit breakers remain untouched, mathematically verifying the airgap invariant.

---

#### Story 7.5: Software-In-The-Loop (SITL) Headless Digital Twin Harness
* **ID:** `STORY-7.5` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 4 | **Quality Gate:** G2, G6
* **User Story:** *As the QA engineer, I want to execute Oasis as a headless Software-In-The-Loop (SITL) digital twin paired with live Sovereign Stack daemons in local test environments, so that we can stress-test edge protocols prior to bare-metal deployment.*
* **Technical Tasks:**
  1. Implement headless execution mode: `./oasis_engine --headless --sitl --ticks 100000`.
  2. Connect headless engine to external Sovereign Stack test daemons running in local Linux namespaces (`col-telemetryd` through `col-adversaryd`).
  3. Execute 100x accelerated clock stress-testing of microgrid black-starts (Scenario Upsilon) and trade clearances (Scenario Pi).
  4. Log state convergence, exergy conservation, and CRDT synchronization traces.
* **Acceptance Criteria:**
  * **Given** the 7 Sovereign Stack daemons running in local test containers,
  * **When** Oasis runs in headless SITL mode for 100,000 ticks at 100x speed,
  * **Then** state synchronizes smoothly across UHAI IPC without deadlock,
  * **And** SITL run verifies system stability across all six Quality Gates prior to hardware flashing.

---

## 6. Systematic Quality Gate Verification & Test Harness

Every user story and scenario must satisfy the **Six Inviolable Quality Gates (G1–G6)** codified in `docs/COLLECTIVE_GUIDING_PRINCIPLES.md` and `docs/OASIS_GDD.md`:

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       SIX INVIOLABLE QUALITY GATES (G1–G6)                                            │
├───────┬───────────────────────────────┬───────────────────────────────────────────────────────────┬───────────────────┤
│ Gate  │ Domain                        │ Core Invariant Assertion & Verification Method            │ Automated Harness │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────────────┼───────────────────┤
│ **G1**│ Thermodynamic Conservation    │ Mass and energy balances obey physical conservation laws  │ Catch2 Test:      │
│       │ & Physical Bounds             │ within ±10%. Power draw matches physical enthalpy and     │ `test_thermo.cpp` │
│       │                               │ Joule dissipation. SOR solver residuals < 1e-4.           │                   │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────────────┼───────────────────┤
│ **G2**│ Offline Local-First Partition │ 100% deterministic execution with zero WAN connectivity.  │ Catch2 Test:      │
│       │ & Privacy Resilience          │ Zero central server pings. BLE/NFC checkouts and speed-of-│ `test_offline.cpp`│
│       │                               │ light distance bounding execute purely on local hardware. │                   │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────────────┼───────────────────┤
│ **G3**│ Byzantine Detection &         │ Sensor spoofing, Sybil identity rings, and compromised    │ Catch2 Test:      │
│       │ Sybil Immunity                │ keys fail safely. Sybil nodes > 3 hops receive 0 credit.  │ `test_byzantine`  │
│       │                               │ Malicious defection slashes voucher reputation by 50%.    │                   │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────────────┼───────────────────┤
│ **G4**│ Resource & State Machine      │ Dual-timescale Lyapunov damping prevents infinite stress  │ Catch2 Test:      │
│       │ Invariants (Zero Escrow Hang) │ death spirals. Clinical recovery attractor triggers at    │ `test_fsm_saga`   │
│       │                               │ S > 80k. 30-tick watchdog with exponential backoff.       │                   │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────────────┼───────────────────┤
│ **G5**│ Trojan Fiat Ingestion &       │ External Web2 Stripe/fiat payloads compile cleanly to     │ Catch2 Test:      │
│       │ Legal Carapace Conversion     │ internal intents without KYC identity leakage. In-game    │ `test_trojan_l7`  │
│       │                               │ blueprints match open-hardware specs (CERN-OHL).          │                   │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────────────┼───────────────────┤
│ **G6**│ SITL Hardware-in-the-Loop &   │ Zero embedded daemons in engine. Synthetic telemetry tagged│ Catch2 Test:     │
│       │ Anti-Bleed Invariant Gate     │ QUALITY_SIMULATED cannot actuate real physical hardware.  │ `test_airgap.cpp` │
│       │                               │ UHAI SPSC ring buffer latency < 50 µs in /dev/shm.        │                   │
└───────┴───────────────────────────────┴───────────────────────────────────────────────────────────┴───────────────────┘
```

---

## 7. Definition of Ready (DoR) and Definition of Done (DoD)

### 7.1. Definition of Ready (DoR)
A User Story is considered **Ready for Sprint Commitment** only when:
1. Grounded in at least one canonical MVS Scenario (`docs/scenarios/*.md`) or explicit GDD V5.0 architectural directive.
2. Contains concrete, verifiable Given/When/Then acceptance criteria.
3. Explicitly assigned to a single Sprint with Story Point estimation agreed upon by the Council.
4. Mapped directly to at least one Quality Gate (G1 through G6).
5. All external interface contracts (UHAI, Flecs ECS archetypes, WGSL uniforms) are statically defined.

### 7.2. Definition of Done (DoD)
A User Story is considered **Done** only when:
1. Implemented in C++20 / WGSL following strict memory constraints (zero heap allocations on tick).
2. Compiles cleanly across native macOS, Linux, and WebAssembly targets with `-Wall -Wextra -Werror`.
3. Unit and regression tests written in Catch2 3.5.3 pass 100% in headless CI.
4. Verified against all associated Quality Gates (G1–G6) without tolerance exceptions.
5. Passes the static Anti-Bleed Linter (`scripts/verify_anti_bleed.sh`) with zero boundary violations.
6. Memory profiled under AddressSanitizer (ASan) and WebAssembly runtime, verifying zero memory leaks and strict 256.00 MB linear memory adherence.
