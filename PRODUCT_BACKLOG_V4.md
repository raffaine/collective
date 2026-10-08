# OASIS ENGINE PRODUCT BACKLOG V4.0
**Document ID:** `OASIS-BACKLOG-V4.0`  
**Milestone:** Round 3 Architectural Council Ratification  
**Status:** Council Approved (Unanimous Consensus Ratified)  
**Total Epics:** 6 Epics  
**Total Stories:** 31 Articulated Stories  
**Total Estimation:** 195 Story Points (4 Sprints: SP1: 47, SP2: 52, SP3: 47, SP4: 49 SP)  
**Target Repositories:** `core/1_simulation/`, `docs/`, `scripts/`  

---

## 1. Vision & Release Strategy

### 1.1. Executive Vision
The Oasis Game Engine Backlog Version 4.0 translates the ratified **Game Design Document (`docs/OASIS_GDD.md`)** and **Engine Architecture Specification (`docs/OASIS_ARCHITECTURE.md`)** into an actionable, sprint-ready product roadmap. 

Oasis is the **Trojan Horse Simulator** for physical-world sovereign resilience:
* **Epic 1 (D1): Dwarf Fortress Cognitive Core:** Deep psychological modeling (thoughts, bounded memory ring buffers, core values, focus, stress, and breakdown states) replaces simplistic need bars.
* **Epic 2 (D2): The Sims Indirect Founder Management:** Intimate stewardship of Founder 01 via whims, affordances, and autonomous moral rejection heuristics.
* **Epic 3 (D3): Cities Skylines Macro Leeching:** Systematic scavenging of decaying 12kV grid lines, water mains, and asphalt across Suburban Sprawl, Dense Urban, and Eco-Village biomes, solved via robust Poisson/SOR flow mechanics.
* **Epic 4 (D4): Stochastic Boundary Interface:** Inhomogeneous Poisson process modeling legacy pressure, attenuated by local autarky, NPC mutual aid, and peer proximity ties.
* **Epic 5 (D4 + Architecture): Sovereign Stack UHAI Integration:** Zero-copy shared memory IPC (< 50 µs latency) connecting Oasis to external Sovereign Stack daemons without embedded networking.
* **Epic 6 (Architecture): High-Performance Engine Core & WebGPU Pipeline:** C++20 Flecs ECS, screen-space 3D DDA compute ray-marching at 60 FPS, fixed-point math, and multi-platform compilation (Homebrew, emsdk 3.1.56, vcpkg).

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                OASIS ENGINE REBOOT ROADMAP (4 SPRINTS)                           │
├──────────┬─────────────────────────────┬──────────────────────────────────────────┬──────────────┤
│ Sprint   │ Focus Area                  │ Delivered Capabilities                   │ Velocity     │
├──────────┼─────────────────────────────┼──────────────────────────────────────────┼──────────────┤
│ **SP 1** │ Foundations & Physical Core │ WebGPU DDA ray-marcher, UHAI ring buffer,│ 47 Points    │
│          │                             │ 32-bit voxels, basic facets, tethers     │              │
├──────────┼─────────────────────────────┼──────────────────────────────────────────┼──────────────┤
│ **SP 2** │ Cognitive Depth & Leeching  │ 32-slot memory ring buffer, utility AI,  │ 52 Points    │
│          │                             │ urbanite deconstruction, flow solver     │              │
├──────────┼─────────────────────────────┼──────────────────────────────────────────┼──────────────┤
│ **SP 3** │ Stress, Biomes & Federation │ Stress breakdown FSM, multi-biomes,      │ 47 Points    │
│          │                             │ P2P mesh discovery, border softening     │              │
├──────────┼─────────────────────────────┼──────────────────────────────────────────┼──────────────┤
│ **SP 4** │ Strange Moods & Verification│ Masterwork artifacts, Commons corridors, │ 49 Points    │
│          │                             │ BPMN intent export, headless test harness│              │
├──────────┴─────────────────────────────┴──────────────────────────────────────────┼──────────────┤
│ TOTAL BACKLOG VELOCITY                                                            │ 195 Points   │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## 2. Anti-Bleed Purge: Surgical Separation of Concerns

Backlog V4 explicitly documents the surgical excision of all monolithic anti-patterns identified in V1, V2, and V3:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                             OASIS ARCHITECTURAL BOUNDARY PURGE MANIFEST                          │
├──────────────────────────────┬───────────────────────────────┬───────────────────────────────────┤
│ Contaminated Subsystem       │ Purged Anti-Pattern           │ Backlog V4 Corrective Boundary    │
├──────────────────────────────┼───────────────────────────────┼───────────────────────────────────┤
│ `did_crypto_generator.cpp`   │ In-engine libsodium Ed25519   │ PURGED. Oasis holds only opaque   │
│                              │ key generation in C++ loop.   │ 32-byte `founder_did_handle` (L5).│
├──────────────────────────────┼───────────────────────────────┼───────────────────────────────────┤
│ `crdt_sync_engine.cpp`       │ In-engine libp2p/WebRTC       │ PURGED. All multi-node state sync │
│                              │ Gossipsub networking.         │ handled by `col-storaged` (L3).   │
├──────────────────────────────┼───────────────────────────────┼───────────────────────────────────┤
│ `bpmn_parser.cpp`            │ In-engine XML parsing of      │ PURGED. Oasis exports spatial     │
│                              │ BPMN schemas via `pugixml`.   │ intents; `col-execd` runs VM (L4).│
├──────────────────────────────┼───────────────────────────────┼───────────────────────────────────┤
│ `Layer7DrainAccumulator`     │ In-engine fiat accounting and │ PURGED. Oasis simulates watts;    │
│                              │ utility tariff debiting.      │ `col-adversaryd` bills fiat (L7). │
├──────────────────────────────┼───────────────────────────────┼───────────────────────────────────┤
│ `iot_telemetry_bridge.cpp`   │ Direct MQTT / CoAP hardware   │ PURGED. Real hardware drivers     │
│                              │ actuation in game runtime.    │ belong to `col-telemetryd` (L1).  │
├──────────────────────────────┼───────────────────────────────┼───────────────────────────────────┤
│ Monolithic Game Loop         │ Hardcoding Sovereign layers   │ PURGED. Decoupled POSIX daemons   │
│                              │ into engine threads (T1, T3). │ communicating over UHAI IPC.      │
└──────────────────────────────┴───────────────────────────────┴───────────────────────────────────┘
```

---

## 3. Epics & User Stories (31 Articulated Stories)

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    EPIC SUMMARY & POINT ALLOCATION                               │
├────────┬────────────────────────────────────────────────────────┬─────────────┬──────────────────┤
│ Epic   │ Epic Title                                             │ Priority    │ Total Estimate   │
├────────┼────────────────────────────────────────────────────────┼─────────────┼──────────────────┤
│ **E1** │ Deep Psychology & Episodic Memory (DF Cognitive Core)  │ P0 Critical │ 29 Story Points  │
│ **E2** │ Indirect Founder Management & Agency Loop (The Sims)   │ P0 Critical │ 34 Story Points  │
│ **E3** │ Macro-Infrastructure Leeching & Multi-Biome Simulation │ P1 High     │ 47 Story Points  │
│ **E4** │ Boundary Interface & Stochastic Legacy Pressure        │ P1 High     │ 26 Story Points  │
│ **E5** │ Sovereign Stack UHAI Integration & Peer Federation     │ P1 High     │ 31 Story Points  │
│ **E6** │ High-Performance Engine Core & WebGPU Pipeline         │ P0 Critical │ 28 Story Points  │
├────────┴────────────────────────────────────────────────────────┴─────────────┼──────────────────┤
│ TOTAL (6 Epics, 31 Stories)                                                    │ 195 Story Points │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

---

### Epic 1: Deep Psychology & Episodic Memory (DF Cognitive Core)
**Total Estimate:** 29 Story Points | **Priority:** P0 Critical  
**Focus:** Implement the bounded 1,024-byte ECS cognitive state, 32-slot episodic memory ring buffer with permanent trauma protection, Big-Five + DF facets, 8 core values enum, focus, Q24.8 stress integration with dual-timescale Lyapunov damping, breakdown states, and deterministic PRNG.

#### Story 1.1: Core Personality Facets & Core Values ECS Component
* **ID:** `STORY-1.1` | **Points:** 5 SP | **Priority:** P0 | **Sprint:** Sprint 1
* **User Story:** *As an Oasis simulation agent, I want my personality facets and core philosophical values represented in compact, cache-aligned ECS structs, so that hundreds of agents can evaluate emotional stimuli without causing CPU cache misses or heap allocations.*
* **Technical Tasks:**
  1. Define `PersonalityFacets` struct (`alignas(4)`, 16 bytes) containing 10 signed 8-bit facets (`assertiveness`, `anxiety`, `empathy`, `self_discipline`, `curiosity`, `pragmatism`, `stoicism`, `sociability`, `discordance`, `artistic_drive`).
  2. Define `CoreValues` struct (`alignas(4)`, 16 bytes) containing exactly 8 signed 8-bit values enum/array (`AUTONOMY`, `STEWARDSHIP`, `SOLIDARITY`, `INGENUITY`, `TENACITY`, `PRAGMATISM`, `TRANSPARENCY`, `BEAUTY`) matching `docs/OASIS_GDD.md` Section 2.2 and `docs/OASIS_ARCHITECTURE.md` Section 3.2.3.
  3. Register components in Flecs archetype tables with contiguous array layout.
  4. Implement deterministic initialization from split-stream PCG32 seeded by agent ID.
* **Acceptance Criteria:**
  * **Given** an entity is spawned in the simulation world,
  * **When** `PersonalityFacets` and `CoreValues` components are attached,
  * **Then** `sizeof(PersonalityFacets) == 16` and `sizeof(CoreValues) == 16`,
  * **And** mutating a facet executes in $< 5\text{ ns}$ without dynamic heap allocations.

#### Story 1.2: 32-Slot Episodic Memory Ring Buffer & Salience Decay
* **ID:** `STORY-1.2` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 2
* **User Story:** *As an agent, I want high-valence sensory and social events recorded into a fixed 32-slot circular ring buffer with Ebbinghaus salience decay and permanent trauma protection, so that past hardships and triumphs alter my long-term behavior without unbounded memory growth.*
* **Technical Tasks:**
  1. Implement `EpisodicMemoryNode` struct (20 bytes packed, `alignas(4)`) holding timestamp, categorical event ID, subject entity ID, fixed-point valence, salience, 3D voxel coordinates, and `uint8_t flags` bitfield including `PERMANENT_MEMORY` (`0x01`).
  2. Implement `AgentEpisodicMemoryRing` component managing exactly 32 contiguous slots (640 bytes packed per agent).
  3. Implement Ebbinghaus salience decay running at diurnal sleep boundaries: $S_{k+1} = S_k - (S_k \gg 4)$. Protect root traumas with salience $> 80$ or `PERMANENT_MEMORY` flag from circular FIFO ring eviction before consolidation.
  4. Implement spatial hash query triggering sensory flashbacks when an agent moves within 4 decimeters of a trauma voxel.
* **Acceptance Criteria:**
  * **Given** Founder 01 witnesses a code enforcement raid at coordinate $(95, 32, 48)$,
  * **When** Founder 01 walks within 4 decimeters of that coordinate 10,000 ticks later,
  * **Then** the spatial query activates `EpisodicMemoryNode`, injecting a negative thought into the active thought buffer,
  * **And** total memory per agent for episodic records remains strictly capped at 640 bytes packed (within the 1,024-byte per-agent cap).

#### Story 1.3: Chronic Stress Accumulator & Altruism Fatigue FSM
* **ID:** `STORY-1.3` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 3
* **User Story:** *As the simulation engine, I want to integrate active thought valence into a cumulative stress accumulator governed by dual-timescale Lyapunov emotional damping with clinical recovery attractors, so that agents experience authentic psychological breakdowns and altruism fatigue without entering unrecoverable death spirals.*
* **Technical Tasks:**
  1. Implement $Q24.8$ fixed-point stress differential integration (range $[-100,000 \dots +100,000]$): $\frac{d(\text{Stress})}{dt}$ evaluating mood against resilience.
  2. Implement Dual-Timescale Lyapunov Damping ($\tau_{\text{acute}} = 30\text{s}$, $\tau_{\text{baseline}} = 7\text{ days}$) modulating stress relief through physical affordances (warmth, shared food, acoustic music), coupled with an autonomous clinical recovery attractor ($\Delta\text{Stress}_{\text{clinical}} = -150\text{ units/hour}$) to break absorbing Catatonia death spirals.
  3. Implement 4-tier Breakdown FSM: Normal ($<25k$), Mild Irritability ($>25k$), Tantrum / Melancholy ($>50k$), Catatonia / Berserk ($>80k$).
  4. Unit test assert stress recovery below 40,000 within 48 game hours when affordances are provided, and verified attractor recovery from Catatonia.
* **Acceptance Criteria:**
  * **Given** an agent experiences persistent negative thoughts over 72 in-game hours,
  * **When** cumulative stress exceeds 50,000,
  * **Then** the agent enters a Breakdown State (Tantrum: destroys nearby equipment; Melancholy: refuses all work directives),
  * **And** stress values remain strictly clamped within $[-100,000, +100,000]$ in $Q24.8$ fixed-point arithmetic.

#### Story 1.4: Focus System & Cognitive Distraction Penalties
* **ID:** `STORY-1.4` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 3
* **User Story:** *As an agent, I want my operational focus to fluctuate between 0 and 255 based on physical comfort and emotional preoccupations, so that distressed agents make physical fabrication mistakes.*
* **Technical Tasks:**
  1. Implement `Focus` state variable ($0 \dots 255$) in `AgentPsychologyComponent`.
  2. Link focus depletion to physical cold, dehydration, calorie deficits, and unfulfilled whims.
  3. Apply focus efficiency multiplier to fabrication and construction systems ($\text{Speed} \propto \frac{\text{Focus}}{255}$).
  4. Introduce 25% error and miswiring probability for high-precision tasks when $\text{Focus} \le 50$.
* **Acceptance Criteria:**
  * **Given** Founder 01 has $\text{Focus} = 35$,
  * **When** attempting to wire an off-grid electrical inverter,
  * **Then** task completion duration increases by 100%, and the action has a 25% chance to blow a fuse or drop the tool.

#### Story 1.5: Natural-Language Cognitive Dossier Generator
* **ID:** `STORY-1.5` | **Points:** 3 SP | **Priority:** P2 | **Sprint:** Sprint 4
* **User Story:** *As a player, I want to inspect any citizen to read an empathetic, grammatically cohesive natural-language summary of their internal thoughts and values, so that I understand their mind without reading spreadsheets.*
* **Technical Tasks:**
  1. Implement rule-based narrative string assembler in `oasis::ui::DossierView`.
  2. Map top 3 dominant values, most salient episodic memory, and current mood to natural English templates.
  3. Render dossier inside ImGui / WebGPU UI pane on agent selection.
* **Acceptance Criteria:**
  * **Given** an agent is selected with high anxiety, recent trauma, and ecological values,
  * **When** the player opens the dossier pane,
  * **Then** the UI renders a cohesive narrative paragraph (e.g., *"Founder 01 feels deeply unsettled. She is haunted by the raid..."*),
  * **And** formatting executes without memory allocation inside the frame tick.

---

### Epic 2: Indirect Founder Management & Agency Loop (The Sims)
**Total Estimate:** 34 Story Points | **Priority:** P0 Critical  
**Focus:** Indirect guidance scheme focused on Founder 01, advisory intention queue, whims, environmental affordances, autonomous moral rejection heuristics, anti-lockup guarantees, and community delegation.

#### Story 2.1: Embodied Mortal Founder 01 Kinematics & Caloric Bounds
* **ID:** `STORY-2.1` | **Points:** 5 SP | **Priority:** P0 | **Sprint:** Sprint 1
* **User Story:** *As Founder 01, I exist as an embodied physical entity bound to caloric expenditure, hydration, and physical fatigue, so that the player cannot treat me as an omniscient RTS cursor.*
* **Technical Tasks:**
  1. Attach `EntityKinematics` (24 bytes) and `PhysiologicalState` (16 bytes) components to Founder 01 entity.
  2. Implement caloric burn rate system (2,000 kcal baseline, up to 4,500 kcal breaking asphalt).
  3. Enforce involuntary resting when fatigue reaches 100%, forcing sleep or sitting.
* **Acceptance Criteria:**
  * **Given** Founder 01 performs heavy labor (breaking asphalt),
  * **When** 2,500 active calories are expended without food intake,
  * **Then** physical fatigue forces the character to stop, drop tools, and seek nourishment.

#### Story 2.2: Advisory Intention Queue & Deliberative Utility AI
* **ID:** `STORY-2.2` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 2
* **User Story:** *As Founder 01, I want to evaluate player-queued blueprint chores against my personal values, focus, and energy, so that I maintain human agency rather than mindlessly executing clicks.*
* **Technical Tasks:**
  1. Implement `IntentionQueue` component holding up to 8 player-suggested task proposals.
  2. Implement utility scoring function: $U(\text{Task}) = \text{Priority} + \text{Alignment} + \text{Focus} - \text{StressCost} - \text{Fatigue}$.
  3. Implement 25% utility hysteresis threshold ($\Theta_{\text{hysteresis}} = 0.25 \cdot U_{\text{max}}$) to prevent whim thrashing.
  4. Implement distance-adaptive lease TTL ($\text{TTL}_{\text{init}} = \max(60, \lceil \frac{d}{\text{speed}} \rceil \times 1.5)$) with transit heartbeat renewal every 15 ticks and 30-tick `ProgressWatchdog` detecting action blockages and triggering the unstick recovery reflex.
  5. Implement exponential backoff on unfulfillable affordance blacklisting ($30\text{s}, 60\text{s}, 120\text{s}, 300\text{s}$) to eliminate 150-tick periodic livelocks.
* **Acceptance Criteria:**
  * **Given** the player queues a construction chore on the Cadastral Slate,
  * **When** Founder 01 finishes her current action,
  * **Then** she evaluates the chore via utility scoring and accepts or defers it based on current focus and fatigue,
  * **And** distance-adaptive lease TTL with heartbeat renewal prevents premature lease loss during transit,
  * **And** zero task oscillation occurs between two tasks of similar utility.

#### Story 2.3: Spontaneous Founder Whims & Inspirations Loop
* **ID:** `STORY-2.3` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Sprint 3
* **User Story:** *As Founder 01, I want to spontaneously generate personal desires and craft aspirations, so that honoring my individual human needs creates an engaging gameplay loop of mutual respect.*
* **Technical Tasks:**
  1. Implement whim generator sampling from active personality facets and unfulfilled interests.
  2. Display active whims on the Steward Slate HUD (e.g., *"Wants to build solar dehydrator"*).
  3. Hook whim completion into psychological state: awards $+20$ Focus and purges $-15$ Stress.
* **Acceptance Criteria:**
  * **Given** Founder 01 enters a rested, contemplative state,
  * **When** a whim timer expires,
  * **Then** a whim is generated and displayed on the HUD,
  * **And** completing the whim successfully restores 20 points of focus and decrements stress.

#### Story 2.4: Autonomous Moral Gating & Directive Rejection Heuristic
* **ID:** `STORY-2.4` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Sprint 3
* **User Story:** *As Founder 01, I want to evaluate player directives against an autonomous moral gating function, so that I refuse orders that violate my core values or exceed stress breakdown thresholds.*
* **Technical Tasks:**
  1. Implement logistic sigmoid rejection heuristic: $P(\text{Rejection}) = \frac{1}{1 + e^{-k(\text{MoralConflict} + \text{StressDelta} - \text{Trust})}}$.
  2. Calculate `MoralConflict` against `CoreValues` (e.g., dumping battery acid vs `ecological_harmony`).
  3. Implement visual and audio refusal telegraph over the Plumbob (*"Founder 01 refuses: 'I will not poison the soil...'"*).
* **Acceptance Criteria:**
  * **Given** Founder 01 has `CoreValues.ecological_harmony = +80` and `Stress = 60,000`,
  * **When** the player issues a directive to dump lead-acid battery electrolyte into the aquifer,
  * **Then** the directive is rejected with an audible refusal and in-world telegraph,
  * **And** Founder 01 drops focus by 20 points and refuses further non-essential tasks.

#### Story 2.5: Volumetric Plumbob 2.0 Vitality Indicator
* **ID:** `STORY-2.5` | **Points:** 5 SP | **Priority:** P2 | **Sprint:** Sprint 4
* **User Story:** *As a player, I want to gauge Founder 01's combined physiological and psychological status via a 3D chromatic gemstone hovering above her head, so that I can read her condition at a glance.*
* **Technical Tasks:**
  1. Create WebGPU mesh instancing shader for faceted Plumbob geometry.
  2. Interpolate emissive palette: Emerald green (grounded), Amber flicker (strained), Crimson strobe (breakdown), Iridescent shimmer (Strange Mood).
  3. Modulate pulse frequency to heart rate and stress level ($0.5\text{ Hz}$ to $4.0\text{ Hz}$).
* **Acceptance Criteria:**
  * **Given** Founder 01 transitions from grounded into acute breakdown,
  * **When** the frame renders in WebGPU,
  * **Then** the Plumbob shader shifts from calm emerald pulse to jagged crimson strobing at 4 Hz.

---

### Epic 3: Macro-Infrastructure Leeching & Multi-Biome Simulation (Cities Skylines Macro)
**Total Estimate:** 47 Story Points | **Priority:** P1 High  
**Focus:** Omnipresent decaying legacy infrastructure, siphoning mechanics (power, water, sewage, rebar/asphalt, fiber), 3 distinct biomes, linear-segment BVH spatial indexing, SOR/Cholesky flow solvers with ground reference nodes, 128-chunk active sector visibility, and closed-loop permaculture logistics.

#### Story 3.1: Entangled Legacy Utility Tethers & Escrow Fiat Drain
* **ID:** `STORY-3.1` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 1
* **User Story:** *As the simulation engine, I want to simulate physical connections to legacy overhead power and municipal water mains that drain fiat escrow, establishing initial survival stakes within a 128-chunk active sector visibility window.*
* **Technical Tasks:**
  1. Implement `LegacyTetherComponent` tracking connected overhead 240V drop lines and water bibs within the 128-chunk active sector window ($16.38\text{ MB}$).
  2. Track kilowatt-hours and liters drawn, streaming telemetry to UHAI IPC.
  3. Implement smart meter anomaly dissipation rate ($-0.5\% / 100\text{t}$ when current $<10\text{A}$) to prevent false-positive disconnects on transient noise.
  4. Trigger utility shutoff event when escrow reaches zero or smart meter tamper detection fires.
* **Acceptance Criteria:**
  * **Given** the simulation boots at $t=0$,
  * **When** power or water is consumed by appliances,
  * **Then** physical consumption accumulates, and UHAI streams telemetry to Layer 7 `col-adversaryd`.

#### Story 3.2: Urbanite Quarrying & Asphalt Deconstruction Loop
* **ID:** `STORY-3.2` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 2
* **User Story:** *As Founder 01, I want to break up impervious asphalt and concrete slabs to harvest Urbanite chunks, converting toxic surfaces into permeable loam and thermal retaining walls.*
* **Technical Tasks:**
  1. Add deconstruction interaction for asphalt voxels (`material_id = 1`).
  2. Convert broken asphalt voxels into compacted clay/loam.
  3. Yield 4 units of `Urbanite` inventory per voxel; apply silica dust and fatigue penalty to workers without respirators.
* **Acceptance Criteria:**
  * **Given** an asphalt road voxel,
  * **When** deconstructed with a pickaxe or jackhammer,
  * **Then** the voxel transforms into loam, yielding 4 Urbanite blocks,
  * **And** parcel stormwater infiltration capacity increases.

#### Story 3.3: Copper, Conduit & Rebar Scavenging Engine
* **ID:** `STORY-3.3` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Sprint 2
* **User Story:** *As an agent, I want to strip copper wiring, galvanized pipe, and rebar from derelict structures, providing essential fabrication raw materials without fiat purchases.*
* **Technical Tasks:**
  1. Implement scavenging interaction on derelict cinderblock structures and camper shells.
  2. Extract high-purity copper conduit, structural steel rebar, and electric motors into inventory.
  3. Update voxel damage states and structural integrity masks.
* **Acceptance Criteria:**
  * **Given** a derelict structure on Lot 402,
  * **When** an agent executes a scavenging work token,
  * **Then** raw copper and rebar materials are added to the commons inventory.

#### Story 3.4: Suburban, Urban & Eco-Village Biome Procedural Generators
* **ID:** `STORY-3.4` | **Points:** 10 SP | **Priority:** P1 | **Sprint:** Sprint 3
* **User Story:** *As the terrain generator, I want to instantiate three distinct biomes (Suburban Cul-de-sac, Urban Industrial Block, Permaculture Eco-Village) with authentic legacy infrastructure layouts within a 128-chunk active sector visibility window.*
* **Technical Tasks:**
  1. Implement procedural terrain generator supporting 3 biomes:
     - Suburban Sprawl: cul-de-sacs, driveways, dead lawns, overhead split-phase drops.
     - Urban Block: concrete canyons, steam tunnels, flat roofs with load limits ($250\text{ kg/m}^2$).
     - Eco-Village: keyline watershed contours, degraded orchards, natural streams.
  2. Enforce the 128-chunk active sector window ($16.38\text{ MB}$ within the 256 MB WASM memory budget).
  3. Build Linear-Segment BVH for conduit queries ($O(\log N)$ spatial search).
* **Acceptance Criteria:**
  * **Given** a biome selection at node genesis,
  * **When** world generation runs,
  * **Then** terrain chunks and legacy conduit BVH populate deterministically from seed in $< 500\text{ ms}$.

#### Story 3.5: Closed-Loop Permaculture Logistics (Biochar, Exergy, Greywater)
* **ID:** `STORY-3.5` | **Points:** 8 SP | **Priority:** P2 | **Sprint:** Sprint 4
* **User Story:** *As the engine, I want to simulate closed-loop resource transformations (pyrolysis biochar production, greywater filtration, solar thermal storage), so that off-grid severance is achievable.*
* **Technical Tasks:**
  1. Implement biochar pyrolysis kiln thermodynamic simulation (biomass + heat $\to$ biochar + syn-gas).
  2. Implement greywater bioswale filtration model: contaminated runoff passes through sand, gravel, and biochar to produce clean irrigation water.
  3. Implement rocket mass heater thermal storage bench absorbing exhaust heat and radiating it into living spaces.
* **Acceptance Criteria:**
  * **Given** a functional biochar kiln and greywater bioswale,
  * **When** waste streams pass through them,
  * **Then** soil fertility increases by $+30\%$ and water contamination drops to potable levels.

#### Story 3.6: Infrastructure Flow Solver (SOR & Ground Reference Node)
* **ID:** `STORY-3.6` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 2
* **User Story:** *As the simulation engine, I want to solve electrical and hydraulic network flow equations using Successive Over-Relaxation (SOR) with a virtual ground reference node and Sparse Cholesky fallback, so that power and fluid networks converge in $<20$ iterations without floating singular matrices when islanded from legacy infrastructure.*
* **Technical Tasks:**
  1. Formulate linear conductance matrix $\mathbf{G} \mathbf{v} = \mathbf{i}$ for active electrical and pipe networks.
  2. Implement virtual earth reference node ($g_{\text{virtual\_earth}} = 10^{-6}\text{ S}$) tied to soil voxels, eliminating floating singular matrices during Epoch 3 Sovereign Severance.
  3. Implement Successive Over-Relaxation (SOR with relaxation factor $\omega = 1.6$), achieving convergence in $< 20$ iterations.
  4. Implement Sparse Cholesky decomposition fallback when condition number $\kappa(\mathbf{G}) > 10^6$.
  5. Connect legacy feeder lines to slack bus at nominal $7,200\text{ V}$.
* **Acceptance Criteria:**
  * **Given** an islanded off-grid microgrid disconnected from the municipal grid,
  * **When** the SOR flow solver evaluates node voltages across 128 chunks,
  * **Then** the solver converges in $< 20$ iterations with zero singular matrix exceptions,
  * **And** node voltage residual remains strictly below $10^{-4}\text{ V}$.

---

### Epic 4: Boundary Interface & Stochastic Legacy Pressure
**Total Estimate:** 26 Story Points | **Priority:** P1 High  
**Focus:** Local-first simulation philosophy, stochastic boundary interface (inhomogeneous Poisson process for legacy pressure), 3 attenuation pillars, volumetric Fog of Legacy shader, and municipal code encounters.

#### Story 4.1: Stochastic Poisson Boundary Event Generator
* **ID:** `STORY-4.1` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 1
* **User Story:** *As the simulation engine, I want to generate probabilistic external legacy pressure events at the parcel perimeter using an inhomogeneous Poisson process with genesis epsilon guards and an irreducible threat floor, so that the player experiences external fiat pressure without division-by-zero artifacts.*
* **Technical Tasks:**
  1. Implement `PoissonEventGenerator` seeded by PCG32 deterministic PRNG.
  2. Implement genesis autarky epsilon guard $\eta = \frac{E_{\text{self}} + \epsilon}{E_{\text{legacy}} + E_{\text{self}} + \epsilon}$ ($\epsilon = 0.001\text{ kWh/L}$) preventing 0/0 NaN division when both self and legacy energy draw are zero at node genesis.
  3. Model arrival intensity with irreducible threat floor: $\lambda(t) = \max\left(\lambda_{\min},\, \lambda_{\mathcal{R}} \cdot \exp(-\Psi(t))\right)$, where $\lambda_{\min} = 0.001\text{ s}^{-1}$.
  4. Implement 4-state Markov regime generator matrix $\mathbf{Q}$ (Sub-Radar, Under-Scrutiny, Adversarial, Active-Siege).
  5. Dispatch boundary shock events (rate hikes, inspection warnings, smog plumes).
* **Acceptance Criteria:**
  * **Given** an active simulation running at 1 Hz boundary tick,
  * **When** the Poisson generator evaluates event arrival,
  * **Then** event arrival frequencies match the theoretical Poisson distribution within $5\%$ tolerance over 100,000 ticks,
  * **And** arrival intensity never drops below the irreducible floor $\lambda_{\min} = 0.001\text{ s}^{-1}$.

#### Story 4.2: Boundary Attenuation Math (Autarky, NPCs & Mesh Ties)
* **ID:** `STORY-4.2` | **Points:** 5 SP | **Priority:** P0 | **Sprint:** Sprint 2
* **User Story:** *As the engine, I want to attenuate boundary pressure intensity based on physical self-sufficiency, onboarded NPC integration, and peer proximity mesh ties bounded by hyperbolic tangent scaling.*
* **Technical Tasks:**
  1. Calculate Sovereign Attenuation Potential: $\Psi(t) = \alpha \Phi_{\text{infrastructure}} + \beta \Phi_{\text{social}} + \gamma \tanh(\delta \cdot \Phi_{\text{mesh}})$.
  2. Implement physical autarky ratio $\Phi_{\text{infrastructure}}$ (solar stored, rainwater, depaved area).
  3. Implement social integration potential $\Phi_{\text{social}}$ based on citizen trust and mutual aid hours.
  4. Consume verified peer proximity mesh ties $\Phi_{\text{mesh}}$ from Sovereign Stack UHAI events, strictly bounded via $\tanh(\delta \cdot \Phi_{\text{mesh}}) \in [0, 1.0)$ to prevent infinite boundary collapse.
* **Acceptance Criteria:**
  * **Given** a parcel achieves 100% off-grid solar, integrates 5 trusted citizens, and connects to 2 peer nodes,
  * **When** $\Psi(t)$ is evaluated,
  * **Then** the boundary event arrival rate $\lambda(t)$ is reduced by at least $75\%$,
  * **And** unbounded mesh spam cannot drive attenuation beyond the theoretical asymptotic limit.

#### Story 4.3: Volumetric Fog of Legacy WebGPU Shader
* **ID:** `STORY-4.3` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 4
* **User Story:** *As a player, I want to visually perceive the boundary pressure as an atmospheric smog with flashing sirens that physically recedes as my sovereignty grows.*
* **Technical Tasks:**
  1. Implement volumetric ray-marched fog pass in WGSL outside the active lot envelope.
  2. Modulate fog density and color based on boundary friction coefficient $\kappa$.
  3. Dynamically carve out cleared corridors along borders where peer proximity ties are verified.
* **Acceptance Criteria:**
  * **Given** the 3D scene is rendering,
  * **When** boundary friction $\kappa$ drops from 1.0 to 0.2,
  * **Then** the volumetric fog visually recedes by up to 16 voxels beyond the property line.

#### Story 4.4: Municipal Code Enforcement & Inspection Encounters
* **ID:** `STORY-4.4` | **Points:** 5 SP | **Priority:** P2 | **Sprint:** Sprint 4
* **User Story:** *As the simulation, I want to trigger adversarial encounters when municipal code inspectors breach the perimeter, testing the community's legal and social resilience.*
* **Technical Tasks:**
  1. Spawn municipal inspector NPC at perimeter upon `ZONING_INSPECTOR_DISPATCHED` event.
  2. Provide player interaction choices via Founder 01: mediate, present trade guild credentials, or camouflage unpermitted builds.
  3. Resolve citations with legal tokens or fines; apply stress penalty to community.
* **Acceptance Criteria:**
  * **Given** an inspector arrives at the gate,
  * **When** Founder 01 presents a valid guild credential or successfully camouflages the battery bank,
  * **Then** the citation is dismissed without parcel seizure or fine.

#### Story 4.5: Boundary Corridor Merging & Isoperimetric Friction Dissolution
* **ID:** `STORY-4.5` | **Points:** 3 SP | **Priority:** P2 | **Sprint:** Sprint 4
* **User Story:** *As the simulation engine, when two adjacent player parcels establish verified physical proximity ties, I want to merge their boundary perimeters and eliminate the internal dividing interface, reducing exposed hostile perimeter by at least 25% according to isoperimetric scaling and clamping friction $\kappa \in [0.05, 1.0]$.*
* **Technical Tasks:**
  1. Detect adjacent bounding envelopes when proximity proof is confirmed over UHAI.
  2. Dissolve collision boundaries and hostile legacy shock generators along the shared border.
  3. Calculate consolidated boundary friction coefficient $\kappa = \kappa_{\text{base}} \cdot (1 - \tanh(\gamma \cdot N_{\text{citizens}}))$ strictly clamped to $[0.05, 1.0]$ with isoperimetric scaling.
* **Acceptance Criteria:**
  * **Given** two neighboring nodes with adjacent borders,
  * **When** proximity verification confirms connection,
  * **Then** the shared dividing wall is replaced with an open Commons corridor,
  * **And** total exposed hostile boundary perimeter decreases by at least 25%,
  * **And** friction coefficient $\kappa$ never drops below the $0.05$ boundary minimum.

---

### Epic 5: Sovereign Stack UHAI Integration & Peer-Proximity Federation
**Total Estimate:** 31 Story Points | **Priority:** P1 High  
**Focus:** Lock-free SPSC POSIX shared memory ring buffers (< 50 µs latency), Unix domain sockets, Web Worker SAB, FlatBuffers/POD serialization, speed-of-light distance bounding proofs, and BPMN intent compilation.

#### Story 5.1: Zero-Copy UHAI Shared Memory Ring Buffer (Native & WASM)
* **ID:** `STORY-5.1` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 1
* **User Story:** *As the Oasis engine, I want to communicate high-frequency telemetry and actuation with Sovereign Stack daemons via a lock-free SPSC shared memory ring buffer with strict 64-byte cache line isolation and an atomic dropped frames counter, achieving sub-50µs latency without thread blocking.*
* **Technical Tasks:**
  1. Implement `LockFreeSpscRing` backed by POSIX `shm_open()` and `mmap()` on native desktop, and `SharedArrayBuffer` with WebAssembly threads (`-pthread -sSHARED_MEMORY=1 -sPTHREAD_POOL_SIZE=4`) in browser.
  2. Implement `SharedRingHeader` (192 bytes = 3 cache lines: Cache Line 0 with `write_index` and atomic `dropped_frames`; Cache Line 1 with `read_index`; Cache Line 2 with static configuration `capacity`, `element_size`, `magic_sig`, `version`).
  3. Enforce slot padding: `TelemetrySlot` padded to 64 bytes (`alignas(64)`) and `ActuatorSlot` padded to 128 bytes (`alignas(64)`) to eliminate adjacent element false sharing.
  4. Fuzz test with 1,000,000 randomized packets across 2 threads asserting zero corruption.
* **Acceptance Criteria:**
  * **Given** high-frequency telemetry streaming at 100 Hz,
  * **When** frames pass across the UHAI boundary,
  * **Then** round-trip latency measures strictly $< 50\ \mu\text{s}$ natively,
  * **And** zero false sharing occurs between read and write pointers across separate cache lines,
  * **And** zero dynamic heap allocations occur.

#### Story 5.2: Cryptographic DID Handle & BBS+ Credential Bridge
* **ID:** `STORY-5.2` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 4
* **User Story:** *As Founder 01, I want to reference an opaque 32-byte DID handle stored in `AgentIdentityComponent` resolved by `col-kmsd` (Layer 5) over UHAI IPC, utilizing 24-byte telemetry and 80-byte actuator wire frames, so that I possess verifiable identity without embedding private keys in the game.*
* **Technical Tasks:**
  1. Define `founder_did_handle` (32 bytes) in dedicated `AgentIdentityComponent` rather than bloating psychology components.
  2. Implement canonical 24-byte `TelemetrySample` (`alignas(8)`) and 80-byte `ActuatorCommand` (`alignas(8)`) wire formats matching `SOVEREIGN_STACK_BACKLOG.md`.
  3. Implement asynchronous UHAI RPC requesting BBS+ verifiable credential issuance from `col-kmsd`.
  4. Store verified credential attestations in local component cache.
* **Acceptance Criteria:**
  * **Given** Founder 01 completes an advanced solar installation,
  * **When** credential issuance is requested,
  * **Then** `col-kmsd` signs the credential over UHAI and returns an opaque confirmation handle,
  * **And** zero raw cryptographic private keys exist within the Oasis engine runtime.

#### Story 5.3: P2P Mesh Discovery & Border Softening Protocol
* **ID:** `STORY-5.3` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Sprint 3
* **User Story:** *As the simulation engine, I want to receive verified peer discovery events from Reticulum mesh (`col-meshd`), softening the shared boundary into a collaborative Commons corridor over POSIX domain sockets and emsdk pthread workers.*
* **Technical Tasks:**
  1. Listen for `ProximityTieEstablishedEvent` over UHAI Unix domain socket or WASM shared memory worker bridge.
  2. Parse verified speed-of-light distance bounding ceiling ($d \le \frac{c(\Delta t - t_d)}{2}$).
  3. Eliminate hostile boundary event generation along the shared parcel border.
* **Acceptance Criteria:**
  * **Given** two Oasis nodes operate within physical radio range,
  * **When** speed-of-light distance bounding verifies $d \le 100\text{ m}$,
  * **Then** the shared boundary interface dissolves along the common border.

#### Story 5.4: Cross-Border Commons Resource Routing (Energy & Swale)
* **ID:** `STORY-5.4` | **Points:** 5 SP | **Priority:** P2 | **Sprint:** Sprint 4
* **User Story:** *As two allied neighboring nodes, we want to route surplus solar electricity and clean stormwater overflow across our shared property line, earning mutual aid credits on the Layer 3 ledger.*
* **Technical Tasks:**
  1. Implement inter-node physical conduit connections in the linear-segment BVH.
  2. Stream cross-border kilowatt-hours and water liters over UHAI to `col-storaged`.
  3. Receive and display Layer 5 mutual credit settlement confirmations.
* **Acceptance Criteria:**
  * **Given** Node A has surplus solar and Node B needs power,
  * **When** an inter-tie cable is energized,
  * **Then** power flows across the boundary in the physical simulation, and `col-storaged` mints mutual aid credits.

#### Story 5.5: Real-World Intent Compilation (BPMN 2.0 Export to `col-execd`)
* **ID:** `STORY-5.5` | **Points:** 5 SP | **Priority:** P2 | **Sprint:** Sprint 4
* **User Story:** *As the player, when I finalize an optimized permaculture or microgrid blueprint, I want to export the spatial design as a validated BPMN 2.0 process executed by `col-execd`.*
* **Technical Tasks:**
  1. Implement `ExportSpatialIntent()` generating binary `SpatialIntentPayload` from Cadastral blueprint.
  2. Transmit payload over UHAI socket to `col-execd` (Layer 4).
  3. Receive validated BPMN XML confirmation and executable `WorkToken` IDs.
* **Acceptance Criteria:**
  * **Given** an optimized water catchment blueprint on Lot 402,
  * **When** the player clicks "Compile to Reality",
  * **Then** Oasis exports the intent over UHAI, and `col-execd` confirms BPMN compilation.

---

### Epic 6: High-Performance Engine Core & WebGPU Pipeline
**Total Estimate:** 28 Story Points | **Priority:** P0 Critical  
**Focus:** C++20 data-oriented architecture, multi-platform toolchain procurement (Debian/Ubuntu, Fedora, macOS, emsdk 3.1.56), fixed-point math ($Q24.8$ and $Q32.32$), screen-space 3D DDA compute ray-marching at 60 FPS, 256.00 MB WASM linear memory bounds, dual-viewport camera interpolation, Cadastral CAD blueprint shaders, and automated headless CLI test runners.

#### Story 6.1: Screen-Space 3D DDA Compute Ray-Marcher
* **ID:** `STORY-6.1` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 1
* **User Story:** *As the rendering pipeline, I want to march rays through the 3D voxel chunk volume in a WebGPU compute shader at 60 FPS using Hierarchical Two-Level DDA and accurate distance tracking, providing tactile volumetric lighting and material depth within the frame budget.*
* **Technical Tasks:**
  1. Author WGSL compute shader executing Amanatides & Woo 3D DDA traversal with explicit `hit.distance = t_min;` recording and initialization `hit.distance = 0.0;` and `hit.normal = -sign(ray_dir);`.
  2. Implement Hierarchical Two-Level DDA (macro-chunk empty space skipping via coarse 16-dm occupancy grid) bounding GPU lookup throughput to $< 4.6\text{ Giga-steps/s}$ to guarantee meeting the 5.20 ms frame budget at 1080p 60 FPS.
  3. Implement AABB bounding box entry test and voxel material sampling.
  4. Enforce maximum frame time cap of $\le 5.20\text{ ms}$ for ray-marching compute pass.
* **Acceptance Criteria:**
  * **Given** an active world of $3 \times 3 \times 2$ chunks (128 active chunks in visibility sector),
  * **When** rendered at $1920 \times 1080$ resolution,
  * **Then** ray-marching compute pass executes in $\le 5.20\text{ ms}$ and overall frame render time remains strictly below $10.60\text{ ms}$ (60 FPS),
  * **And** ray hits accurately record distance and surface normals without empty-space traversal stalls.

#### Story 6.2: Dual Viewport Smooth Camera Interpolation
* **ID:** `STORY-6.2` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 2
* **User Story:** *As a player, I want to toggle between 3rd-person over-the-shoulder view and Cadastral Axonometric Blueprint view with a 400ms smooth Hermite interpolation curve.*
* **Technical Tasks:**
  1. Implement camera controller supporting Perspective and Orthographic projection matrices.
  2. Implement Hermite curve interpolation for position, yaw, pitch, and field-of-view on keypress (Tab).
  3. Ensure zero camera jitter or clipping during transition.
* **Acceptance Criteria:**
  * **Given** the player presses Tab,
  * **When** camera transition executes,
  * **Then** viewport blends smoothly over exactly 400ms without frame drops.

#### Story 6.3: Cadastral Blueprint CAD Shader & Thermal Overlays
* **ID:** `STORY-6.3` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 2
* **User Story:** *As a player in Cadastral Mode, I want the world rendered in a desaturated architectural blueprint aesthetic with thermodynamic isotherms and utility tethers overlaid.*
* **Technical Tasks:**
  1. Author post-process blueprint shader converting voxel colors into blueprint cyan and white line-art.
  2. Render thermal heatmaps as colored isothermal contours on voxel surfaces.
  3. Highlight high-voltage power conduits and municipal pipes in neon magenta lines.
* **Acceptance Criteria:**
  * **Given** Cadastral Mode is active,
  * **When** thermal overlay is enabled,
  * **Then** voxel temperatures display as clear isotherms, and legacy grid tethers pulse distinctly.

#### Story 6.4: Cross-Platform Build Toolchain, Deterministic Math & SDL2 Shell
* **ID:** `STORY-6.4` | **Points:** 5 SP | **Priority:** P0 | **Sprint:** Sprint 1
* **User Story:** *As a developer and player, I want multi-platform host procurement scripts (Debian/Ubuntu, Fedora, macOS, emsdk), deterministic fixed-point math ($Q24.8$ and $Q32.32$), and unified SDL2 input handling, ensuring identical simulation results across all platforms.*
* **Technical Tasks:**
  1. Specify Linux host package procurement commands for Debian/Ubuntu (`apt-get install -y cmake ninja-build clang-17 libsdl2-dev pkg-config ccache`) and Fedora (`dnf install -y cmake ninja-build clang SDL2-devel pkgconf ccache`).
  2. Maintain sanitized `vcpkg.json` (specifying `flecs 3.2.11`, removing purged `libsodium` and `pugixml`), configure `arm64-linux` triplet, and update `scripts/build_native.sh` architecture detection (`aarch64`).
  3. Implement deterministic fixed-point math library: $Q24.8$ `Fixed32` (range $[-8,388,608 \dots +8,388,607]$) and $Q32.32$ `Fixed64` with saturating arithmetic overloads.
  4. Implement SDL2 event polling loop on desktop and Emscripten virtual SDL2 on web.
  5. Implement screen-to-world raycast for voxel selection and tool cursor highlighting.
* **Acceptance Criteria:**
  * **Given** a clean build on macOS (ARM64/x86_64) or Linux (x86_64/arm64),
  * **When** compilation executes via `scripts/build_native.sh`,
  * **Then** the build succeeds cleanly with zero compiler warnings or missing dependency errors,
  * **And** fixed-point math operations produce identical bit-exact results between native and WASM builds,
  * **And** screen-to-voxel raycast executes within $0.1\text{ ms}$.

#### Story 6.5: Automated Headless CLI Runner & Visual Regression Harness
* **ID:** `STORY-6.5` | **Points:** 5 SP | **Priority:** P2 | **Sprint:** Sprint 4
* **User Story:** *As the CI test harness, I want a standalone `oasis_headless` CLI executable that runs simulation ticks at $\ge 5,000$ ticks/second, enforces the 256.00 MB WASM linear memory budget audit and 1,024-byte per-agent cap, and captures headless WebGPU frame buffers for golden image diffing.*
* **Technical Tasks:**
  1. Compile standalone `oasis_headless` binary stripping SDL2 windowing dependencies.
  2. Enforce 256.00 MB WASM linear memory partition table audit (allocating exactly 256.00 MB across 10 static arenas: Voxel Chunks 16.38 MB, Entity ECS 32.00 MB, Physics & BVH 16.00 MB, Thermo & Fluid 32.00 MB, Psychology & Intention 8.00 MB, UHAI Rings 4.00 MB, WebGPU Staging 32.00 MB, WASM Stack & Binary 16.00 MB, OPFS Cache 32.00 MB, Heap/Contingency Reserve 67.62 MB).
  3. Enforce strict 1,024-byte per-agent contiguous memory footprint cap.
  4. Configure emsdk flags: `-sASYNCIFY_IMPORTS=['emscripten_sleep','oasis_yield_tick']` and support W3C JSPI (`-sJSPI=1`) for elimination of Asyncify unwinding overhead.
  5. Benchmark throughput asserting $\ge 5,000$ ticks/s on native and $\ge 1,500$ ticks/s in Node.js WASM.
  6. Capture off-screen WebGPU frame buffer and compute pixel hash against golden reference.
* **Acceptance Criteria:**
  * **Given** a 100,000 tick stress test run via `oasis_headless`,
  * **When** execution completes,
  * **Then** execution throughput exceeds 5,000 ticks/sec per core with zero assertions or memory leaks,
  * **And** total WASM linear memory footprint strictly adheres to the 256.00 MB ceiling.

---

## 4. Comprehensive Traceability Matrix

This matrix maps every gameplay mechanic from the **Game Design Document (`docs/OASIS_GDD.md`)** to its architectural component in **`docs/OASIS_ARCHITECTURE.md`**, its actionable story in **`PRODUCT_BACKLOG_V4.md`**, and its verification test suite:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                           COMPREHENSIVE TRACEABILITY MATRIX                                                  │
├────┬────────────────────────────┬──────────────────────────┬────────────────────────────┬───────────────────────────────────┤
│ ID │ GDD Requirement            │ Architecture Module      │ Backlog V4 Story           │ Verification Test Suite           │
│    │ (`docs/OASIS_GDD.md`)      │ (`docs/OASIS_ARCH.md`)   │ (`PRODUCT_BACKLOG_V4.md`)  │ (`tests/`)                        │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D1 │ Personality Facets &       │ `psychology/`            │ **Story 1.1** (5 SP, SP1)  │ `tests/psychology_tests.cpp`      │
│    │ Core Values Component      │ `PersonalityFacets`      │ Core Facets & Values       │ - Assert `sizeof == 16` (both)    │
│    │                            │ `CoreValues` 16B structs │                            │ - 8 core values enum check        │
│    │                            │                          │                            │ - Benchmark mutation $<5$ ns      │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D1 │ 32-Slot Episodic Memory    │ `psychology/`            │ **Story 1.2** (8 SP, SP2)  │ `tests/psychology_tests.cpp`      │
│    │ Ring & Flashbacks          │ `EpisodicMemoryNode`     │ 32-Slot Memory Ring &      │ - Invariant: Ring cap 32 (640B)   │
│    │                            │ Ebbinghaus Salience decay│ Salience Decay             │ - `PERMANENT_MEMORY` protection   │
│    │                            │ 20B packed node layout   │                            │ - Flashback distance trigger test │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D1 │ Stress Accumulation &      │ `psychology/`            │ **Story 1.3** (8 SP, SP3)  │ `tests/psychology_tests.cpp`      │
│    │ Breakdown State Machine    │ Q24.8 Differential FSM   │ Chronic Stress Accumulator │ - Assert stress clamped [-100k,+1]│
│    │                            │ Dual Lyapunov Damping    │ & Altruism Fatigue FSM     │ - Attractor breaks Catatonia      │
│    │                            │ Clinical attractor       │                            │ - Homeostasis recovery in 48h     │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D1 │ Focus System & Cognitive   │ `psychology/`            │ **Story 1.4** (5 SP, SP3)  │ `tests/psychology_tests.cpp`      │
│    │ Distraction Penalties      │ Focus state [0..255]     │ Focus System &             │ - Assert fabrication slowdown     │
│    │                            │ Efficiency multiplier    │ Distraction Penalties      │ - 25% miswiring probability check │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D1 │ Natural-Language Cognitive │ `ui/`                    │ **Story 1.5** (3 SP, SP4)  │ `tests/ui_tests.cpp`              │
│    │ Dossier Generator          │ `DossierView`            │ Natural-Language Cognitive │ - Validate narrative grammar      │
│    │                            │ Narrative templating     │ Dossier Generator          │ - Zero frame allocation check     │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D2 │ Mortal Founder 01 Embodied │ `kinematics/`            │ **Story 2.1** (5 SP, SP1)  │ `tests/autonomy_tests.cpp`        │
│    │ Kinematics & Caloric Cap   │ `EntityKinematics`       │ Embodied Mortal Founder 01 │ - Caloric burn rate test          │
│    │                            │ `PhysiologicalState`     │ Kinematics & Caloric Bounds│ - Fatigue rest reflex validation  │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D2 │ Advisory Intention Queue & │ `autonomy/`              │ **Story 2.2** (8 SP, SP2)  │ `tests/autonomy_tests.cpp`        │
│    │ Deliberative Utility AI    │ Deliberative Utility AI  │ Advisory Intention Queue & │ - Assert 25% hysteresis margin    │
│    │                            │ 30-tick ProgressWatchdog │ Deliberative Utility AI    │ - Adaptive lease TTL & heartbeat  │
│    │                            │ Distance-adaptive lease  │                            │ - Affordance exponential backoff  │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D2 │ Spontaneous Founder Whims  │ `autonomy/`              │ **Story 2.3** (8 SP, SP3)  │ `tests/autonomy_tests.cpp`        │
│    │ & Inspirations Loop        │ Whim generator           │ Spontaneous Founder Whims  │ - Focus +20 reward assert         │
│    │                            │ Psychological reward link│ & Inspirations Loop        │ - Stress -15 purge validation     │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D2 │ Moral Gating & Directive   │ `autonomy/`              │ **Story 2.4** (8 SP, SP3)  │ `tests/autonomy_tests.cpp`        │
│    │ Rejection Heuristic        │ Sigmoid gating function  │ Autonomous Moral Gating &  │ - Ecological conflict test        │
│    │                            │ In-world Plumbob cues    │ Directive Rejection        │ - Directive refusal telegraph test│
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D2 │ Volumetric Plumbob 2.0     │ `renderer/`              │ **Story 2.5** (5 SP, SP4)  │ `tests/renderer_tests.cpp`        │
│    │ Vitality Indicator         │ Instanced Plumbob mesh   │ Volumetric Plumbob 2.0     │ - Shader color transition check   │
│    │                            │ Chromatic pulse shader   │ Vitality Indicator         │ - Pulse frequency mapping test    │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D3 │ Legacy Utility Tethers &   │ `infrastructure/`        │ **Story 3.1** (8 SP, SP1)  │ `tests/infrastructure_tests.cpp`  │
│    │ Escrow Fiat Drain          │ `LegacyTetherComponent`  │ Entangled Legacy Utility   │ - Escrow debit rate validation    │
│    │                            │ 128-chunk sector window  │ Tethers & Escrow Drain     │ - Smart meter dissipation rate    │
│    │                            │ Telemetry export to UHAI │                            │ - Utility shutoff trigger assert  │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D3 │ Urbanite Quarrying &       │ `voxels/`                │ **Story 3.2** (8 SP, SP2)  │ `tests/voxel_tests.cpp`           │
│    │ Asphalt Deconstruction     │ Asphalt voxel mutation   │ Urbanite Quarrying &       │ - Voxel transition to loam        │
│    │                            │ Urbanite inventory yield │ Asphalt Deconstruction     │ - Soil infiltration capacity test │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D3 │ Copper, Conduit & Rebar    │ `infrastructure/`        │ **Story 3.3** (8 SP, SP2)  │ `tests/infrastructure_tests.cpp`  │
│    │ Scavenging Engine          │ Scavenging interactions  │ Copper, Conduit & Rebar    │ - Derelict yield verification     │
│    │                            │ Structural integrity mask│ Scavenging Engine          │ - Material conservation check     │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D3 │ Multi-Biome Procedural     │ `world/`                 │ **Story 3.4** (10 SP, SP3) │ `tests/world_tests.cpp`           │
│    │ Generators (3 Biomes)      │ Suburban/Urban/Eco biomes│ Multi-Biome Procedural     │ - Deterministic seed generation   │
│    │                            │ Linear-Segment BVH       │ Generators (3 Biomes)      │ - 128-chunk active sector window  │
│    │                            │ 128-chunk active sector  │                            │ - BVH query speed $\le 3.5\ \mu$s │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D3 │ Closed-Loop Permaculture   │ `thermodynamics/`        │ **Story 3.5** (8 SP, SP4)  │ `tests/thermo_tests.cpp`          │
│    │ Logistics (Biochar, Exergy)│ Biochar pyrolysis model  │ Closed-Loop Permaculture   │ - Pyrolysis yield validation      │
│    │                            │ Bioswale filtration      │ Logistics                  │ - Potable water threshold assert  │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D3 │ Infrastructure Flow Solver │ `infrastructure/`        │ **Story 3.6** (5 SP, SP2)  │ `tests/infrastructure_tests.cpp`  │
│    │ (SOR & Ground Reference)   │ LinearConductanceMatrix  │ Infrastructure Flow Solver │ - SOR convergence in $<20$ iter   │
│    │                            │ SOR $\omega=1.6$, Cholesky│ (SOR & Ground Node)        │ - Virtual earth singular check    │
│    │                            │ Virtual earth ground node│                            │ - Slack bus 7,200V tie assert     │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Stochastic Poisson Legacy  │ `boundary/`              │ **Story 4.1** (8 SP, SP1)  │ `tests/boundary_tests.cpp`        │
│    │ Pressure Generator         │ `PoissonEventGenerator`  │ Stochastic Poisson Legacy  │ - Arrival frequency check ($5\%$) │
│    │                            │ 4-state Markov regime Q  │ Pressure Generator         │ - Genesis autarky epsilon guard   │
│    │                            │ Threat floor $\lambda_m$ │                            │ - Floor $\lambda_{\min}=0.001$/s  │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Boundary Attenuation Math  │ `boundary/`              │ **Story 4.2** (5 SP, SP2)  │ `tests/boundary_tests.cpp`        │
│    │ (Autarky, NPCs, Mesh Ties) │ Potential $\Psi(t)$ calc │ Boundary Attenuation Math  │ - Assert 75% rate reduction       │
│    │                            │ Bounded $\tanh$ mesh tie │ (Autarky, NPCs, Mesh Ties) │ - Mesh tie saturation test        │
│    │                            │ Triad attenuation inputs │                            │ - Formula convergence test        │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Volumetric Fog of Legacy   │ `renderer/`              │ **Story 4.3** (5 SP, SP4)  │ `tests/renderer_tests.cpp`        │
│    │ WebGPU Shader Pass         │ Atmospheric WGSL shader  │ Volumetric Fog of Legacy   │ - Fog density modulation test     │
│    │                            │ Boundary friction $\kappa$│ WebGPU Shader Pass         │ - Visual recession 16 voxels check│
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Municipal Code Enforcement │ `boundary/`              │ **Story 4.4** (5 SP, SP4)  │ `tests/boundary_tests.cpp`        │
│    │ & Inspection Encounters    │ Inspector NPC spawning   │ Municipal Code Enforcement │ - Inspector dialogue choices      │
│    │                            │ Citation resolution FSM  │ & Inspection Encounters    │ - Guild credential dismissal test │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Boundary Corridor Merging  │ `boundary/`              │ **Story 4.5** (3 SP, SP4)  │ `tests/boundary_tests.cpp`        │
│    │ & Isoperimetric Dissolution│ Shared envelope detection│ Boundary Corridor Merging  │ - Assert 25% perimeter reduction  │
│    │                            │ Perimeter consolidation  │ & Isoperimetric Dissolution│ - Shared border collision removal │
│    │                            │ Friction $\kappa \ge 0.05$│                            │ - Friction bound $[0.05, 1.0]$    │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Lock-Free SPSC Shared      │ `uhai/`                  │ **Story 5.1** (8 SP, SP1)  │ `tests/uhai_tests.cpp`            │
│    │ Memory Ring Buffer (<50µs) │ POSIX shm / WebWorker SAB│ Zero-Copy UHAI Shared      │ - Round-trip latency $<50\ \mu$s  │
│    │                            │ 192B header (3 lines)    │ Memory Ring Buffer         │ - 64B cache line isolation test   │
│    │                            │ 64B/128B padded slots    │                            │ - `dropped_frames` counter test   │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Cryptographic DID Handle & │ `uhai/`                  │ **Story 5.2** (5 SP, SP4)  │ `tests/uhai_tests.cpp`            │
│    │ BBS+ Credential Bridge     │ `AgentIdentityComponent` │ Cryptographic DID Handle & │ - Zero private key assert         │
│    │                            │ 24B telemetry / 80B cmd  │ BBS+ Credential Bridge     │ - 24B/80B wire alignment check    │
│    │                            │ UHAI RPC to `col-kmsd`   │                            │ - BBS+ credential issuance RPC    │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ P2P Mesh Discovery &       │ `uhai/`                  │ **Story 5.3** (8 SP, SP3)  │ `tests/boundary_tests.cpp`        │
│    │ Border Softening Protocol  │ Reticulum mesh events    │ P2P Mesh Discovery &       │ - Speed-of-light formula check    │
│    │                            │ Distance bounding verify │ Border Softening Protocol  │ - Border friction collapse test   │
│    │                            │ emsdk pthread bridge     │                            │ - Worker shared memory sync test  │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Cross-Border Commons       │ `infrastructure/`        │ **Story 5.4** (5 SP, SP4)  │ `tests/infrastructure_tests.cpp`  │
│    │ Resource Routing           │ Inter-tie conduit BVH    │ Cross-Border Commons       │ - Multi-node exergy flow test     │
│    │                            │ UHAI ledger mint event   │ Resource Routing           │ - Mutual credit balance check     │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D4 │ Real-World BPMN Intent     │ `uhai/`                  │ **Story 5.5** (5 SP, SP4)  │ `tests/uhai_tests.cpp`            │
│    │ Compilation (`col-execd`)  │ `SpatialIntentPayload`   │ Real-World Intent          │ - Intent serialization test       │
│    │                            │ UHAI dispatch to L4 VM   │ Compilation (BPMN Export)  │ - `col-execd` confirmation assert │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D3 │ Screen-Space 3D DDA        │ `renderer/`              │ **Story 6.1** (8 SP, SP1)  │ `tests/renderer_tests.cpp`        │
│    │ Compute Raymarcher (60FPS) │ Amanatides & Woo DDA     │ Screen-Space 3D DDA        │ - Compute pass $\le 5.20$ ms      │
│    │                            │ Hierarchical 2-level DDA │ Compute Raymarcher         │ - Frame time $\le 10.60$ ms       │
│    │                            │ WGSL compute shader pass │                            │ - `hit.distance = t_min` assert   │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D2 │ Dual Viewport Camera       │ `renderer/`              │ **Story 6.2** (5 SP, SP2)  │ `tests/renderer_tests.cpp`        │
│    │ Interpolation (400ms)      │ Dual projection matrix   │ Dual Viewport Smooth       │ - 400ms Hermite blend timing test │
│    │                            │ Hermite blend curve      │ Camera Interpolation       │ - Zero clipping / jitter assert   │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ D3 │ Cadastral Blueprint CAD    │ `renderer/`              │ **Story 6.3** (5 SP, SP2)  │ `tests/renderer_tests.cpp`        │
│    │ Shader & Thermal Overlays  │ Desaturated CAD pass     │ Cadastral Blueprint CAD    │ - Thermal isotherm rendering check│
│    │                            │ Isotherm & conduit lines │ Shader & Thermal Overlays  │ - Magenta tether pulse test       │
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ Ar │ Cross-Platform Toolchain,  │ `core/`                  │ **Story 6.4** (5 SP, SP1)  │ `tests/core_tests.cpp`            │
│    │ Math & SDL2 Input Shell    │ Debian/Fedora/macOS pkg  │ Cross-Platform Toolchain,  │ - Bit-exact Fixed32/64 math test  │
│    │                            │ Fixed32/Fixed64 library  │ Deterministic Math & SDL2  │ - Multi-platform clean build check│
│    │                            │ SDL2 event polling loop  │                            │ - Screen-to-voxel raycast $<0.1$ms│
├────┼────────────────────────────┼──────────────────────────┼────────────────────────────┼───────────────────────────────────┤
│ Ar │ Automated Headless CLI &   │ `testing/`               │ **Story 6.5** (5 SP, SP4)  │ `tests/headless_tests.cpp`        │
│    │ 256MB WASM Budget Audit    │ `oasis_headless` runner  │ Automated Headless CLI &   │ - CLI throughput $\ge 5000$ tick/s│
│    │                            │ 256.00 MB arena budget   │ Visual Regression Harness  │ - 256.00 MB arena budget audit    │
│    │                            │ Golden image frame diff  │                            │ - 1,024B per-agent cap assert     │
└────┴────────────────────────────┴──────────────────────────┴────────────────────────────┴───────────────────────────────────┘
```

---

## 5. Definition of Done (DoD) & Acceptance Invariants

A user story in Backlog V4 is considered **Done** only when all of the following invariants are verified:
1. **Compilation:** Compiles cleanly with zero warnings (`-Wall -Wextra -Werror -std=c++20`) across Native macOS (Apple Silicon/Intel), Linux x86_64, Linux arm64, and WebAssembly (`emsdk 3.1.56` with `-pthread -sSHARED_MEMORY=1`).
2. **Determinism:** Bit-exact reproducibility confirmed across 10,000 ticks between native and WASM builds using Q24.8 `Fixed32` and Q32.32 `Fixed64` fixed-point arithmetic and seeded split-stream PCG32 PRNG.
3. **Memory Budget:** Zero heap allocations (`malloc`, `new`) during the 30 Hz simulation tick; total memory per agent remains strictly $\le 1,024\text{ bytes}$ contiguous; WASM linear memory remains strictly bounded within the 256.00 MB ceiling.
4. **Performance:** Simulation tick compute remains $\le 8.00\text{ ms}$ (leaving $>75\%$ CPU idle); WebGPU render frame remains $\le 10.60\text{ ms}$ (60 FPS), with ray-marching pass bounded to $\le 5.20\text{ ms}$ via Hierarchical Two-Level DDA.
5. **SoC Boundary:** Zero embedded Sovereign Stack daemons, zero in-engine cryptographic private keys, zero custom P2P networking, zero in-engine fiat balance tracking.
6. **Test Coverage:** Comprehensive Catch2 unit and regression tests written and passing in CI with 100% assertions green.

