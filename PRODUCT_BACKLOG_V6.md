# OASIS ENGINE PRODUCT BACKLOG V6.0 — The Playable Horizon Edition
**Document Identifier:** `OASIS-BACKLOG-V6.0`  
**Milestone:** Phase 4 Playable Horizon & Dual Sprint 11–14 Council Ratification (Round 2 Unanimous Sign-Off)  
**Classification:** Authoritative Technical Product Backlog & Sprint Execution Plan  
**Target Repositories:** `core/1_simulation/`, `docs/`, `scripts/`, `tools/sdd_compiler/`, `tests/`  
**Engine Baseline:** C++20 (`-std=c++20`) / WebGPU (WGSL) / Flecs ECS / UHAI IPC  
**Total Epics:** 12 Epics (`EPIC-1` through `EPIC-12`)  
**Total Articulated User Stories:** 51 User Stories (37 Foundational + 14 Playable Horizon)  
**Total Story Point Budget:** 343 Story Points (242 SP Foundational Epics 1–7 + 101 SP Playable Horizon Epics 8–12, plus Completed Tactical Sprints 5–10)  
**Sprint Schedule:** Sprints 1–10 (COMPLETED Baseline) + Dual Sprints 11–14 (Sprint 11: 35 SP, Sprint 12: 21 SP, Sprint 13: 23 SP, Sprint 14: 22 SP)  
**Status:** Council Approved (Round 2 Unanimous Consensus — All Vetoes Lifted)  
**Date of Enactment:** 2026-10-14  

---

## 1. Council Ratification & Unanimous Consensus

This Product Backlog V6.0 embodies the binding consensus of the Product Owner and the Oasis Technical & Design Council (`sovereign-stack-expert`, `game-designer`, `genre-gamer-expert`, `systems-software-engineer`, `quality-engineer`, `graphics-stack-engineer`, `build-engineer`), convened under the directive of the Horizon & Playability Council. Grounded in `docs/COLLECTIVE_GUIDING_PRINCIPLES.md`, `docs/OASIS_GDD.md` (V6.0), `docs/OASIS_ARCHITECTURE.md`, and `docs/BOOK_TEAM_DRIFT_REPORT.md`, this backlog preserves the mathematical and technical rigor of Epics 1–7, records Sprints 1–10 as the completed engineering baseline (with explicit post-Sprint 10 source-tree reconciliation notes), purges all deprecated adversarial framing, enforces strict Layer Adjacency over UHAI, and specifies Epics 8–12 across Dual Sprints 11–14 to transform Oasis into a tactile, long-horizon playable environment connected to the shared `Arcadia (server-01)` landscape.

### 1.1. Council Roll & Formal Ratification Statements

1. **Product Owner (Council Lead):**
   > *"Backlog V6.0 (Round 2) resolves every architectural, ludic, and systems invariant across our 12 Epics. In strict compliance with `docs/BOOK_TEAM_DRIFT_REPORT.md`, we have purged all 'Trojan Horse', 'parasitic', 'siphoning', and 'raid' framing in favor of cooperative **Macro Infrastructure Reclamation & Salvage Bridging**, thermodynamic load balancing, and legacy treaty/tariff compliance (`col-adversaryd` DES). Oasis is strictly an outward producer of `QUALITY_SIMULATED (0x01)` telemetry over the `L1b <-> L2` UHAI boundary. I ratify this backlog unconditionally."*

2. **Sovereign Stack Specialist (`sovereign-stack-expert`):**
   > *"All Layer Adjacency and Trust Ring invariants are now mathematically airtight. Oasis (Layer 1b) communicates exclusively with the `/dev/shm` UHAI ring (`L1b <-> L2 col-meshd SITL ingress/egress`) and never bypasses layers to talk directly to `col-storaged` (L3), `col-execd` (L4), `col-kmsd` (L5), `col-commonsd` (L6), or `col-adversaryd` (L7). Layer 3 (`col-storaged`) owns PoTW verification and Zero-Sum Mutual Credit Clearing (BCC); Layer 4 (`col-execd`) is our keyless, blind 10 Hz BPMN execution engine; and Layer 5 (`col-kmsd`) owns W3C DIDs, Ed25519/FROST keys, BBS+ credentials, and Trust Rings (`R6 -> R1`). Migrants arrive at `Ring R6 (Unverified Migrant)`, advance to `Ring R3 (Probationary Resident)` only after Layer 3 verifies $\ge 7$ days of sheltered homeostasis and $\ge 36.0\text{ MJ}$ ($10.0$ Exergy Credits) of PoTW sweat-equity, and reach `Ring R1 (Vouched Steward)` via a 2-of-3 FROST threshold ceremony (`SCN-TAU-GENESIS`). Furthermore, `Arcadia (server-01)` synchronizes via headless 24-byte `StateDeltaOp` cadastral records (`SS-EP9-001`). Veto lifted; I ratify unconditionally."*

3. **Game Designer & Genre Gamer Expert (`game-designer` & `genre-gamer-expert`):**
   > *"The Playable Horizon now delivers true Dwarf Fortress / RimWorld / Sims tactile depth without soft-lock traps. We have codified the **Day 1 Genesis Survival Camp Bootstrap** on Lot 402 (pre-placed 3×3 Canvas Bell Tent with `[Sleep Zone]`, camp woodstove + 12 cordwood, 12-meal Starter Ration Crate, 60 L Potable Water Tote, Campfire `[Recreation Zone]`, and Starter Tool Locker), a complete shelter construction & stockpile hauling loop (`[Build Wall: Cob / Urbanite]`, `[Install Door: Insulated Timber]`, `[Build Roof: Timber & Shingle]`, `[Build Hearth: Rocket Mass Heater]`), automatic/manual roof cutaway (`z_cutaway_level`), `F1–F4` Info-View shader overlays, Emergency Warp Brakes, a live Net Trajectory HUD Ticker, dramatic Migrant Arrival cards, and an interactive Blueprint Planning Phase during `EPOCH_SYNC_LOCKED`. Veto lifted; we ratify enthusiastically."*

4. **Systems Software & Graphics Engineer (`systems-software-engineer` & `graphics-stack-engineer`):**
   > *"Every struct, shader uniform, and loop boundary in Epics 8–12 aligns byte-for-byte with our post-Sprint 10 C++20 codebase (`core/1_simulation/src/`). We expand DDA raymarching and CPU raycasting bounds in `raymarch.wgsl`, `raymarch_wgsl.hpp`, and `raycaster.hpp` to `|x| <= 64, y in [0,32], z in [-64,64]` (`MAX_STEPS = 192`). We decouple `RenderWebGPUFrame()` and `EM_ASM(updateHUD(...))` from `g_ecs->progress()` so `100x` Time Warp sub-steps ECS ticks while rendering and pushing aggregated UHAI telemetry once per 60 Hz frame—preventing 1,024-slot SPSC ring overflow. We preserve the canonical 264-byte `SpatialIntentPayload` (`alignas(8)`), use dirty-flag cached $O(H)$ sky-column + 512-voxel BFS for `ThermalEnclosureSolver`, and enforce `-Wall -Wextra -Wpadded -Werror`. Veto lifted; we ratify without reservation."*

5. **Quality & Build Engineer (`quality-engineer` & `build-engineer`):**
   > *"Verification is now locked to concrete Catch2 and Playwright test targets. Section 6 and Epics 8–12 mandate 5 dedicated Catch2 suites in `CMakeLists.txt` (`test_arcball_and_designator.cpp`, `test_time_warp_and_throttling.cpp`, `test_chill_loop_homeostasis.cpp`, `test_migration_waves.cpp`, `test_arcadia_epoch_sync.cpp`) plus synthetic DOM/WebGPU input verification in `tests/verify_canvas.js`. The legacy 60-tick auto-exit is cleanly isolated behind `--headless-ci`. Veto lifted; we ratify unconditionally."*

### 1.2. Unanimous Consensus Declaration

```
════════════════════════════════════════════════════════════════════════════════════════
                        HORIZON & PLAYABILITY COUNCIL
                         ROUND 2 RATIFICATION LOG
════════════════════════════════════════════════════════════════════════════════════════
[VOTE] Product Owner (Council Lead)              : AYE (Unconditional)
[VOTE] Sovereign Stack Specialist                : AYE (Veto Lifted — Unconditional)
[VOTE] Game Designer & Genre Gamer Expert        : AYE (Veto Lifted — Unconditional)
[VOTE] Systems Software & Graphics Engineer      : AYE (Veto Lifted — Unconditional)
[VOTE] Quality & Build Engineer                  : AYE (Veto Lifted — Unconditional)

STATUS: Round 2 Unanimous Consensus Reached
DATE: 2026-10-14T23:45:00Z
ACTION: ENACTED AS CANONICAL PRODUCT BACKLOG V6.0 (THE PLAYABLE HORIZON EDITION)
════════════════════════════════════════════════════════════════════════════════════════
```

---

## 2. Executive Vision & Release Strategy

### 2.1. Strategic Product Vision
Oasis is the **Sovereign Resilience & Shadow Simulator** for physical-world cooperative infrastructure. It is not an escapist fantasy, an idle clicker, a transitionary real-world hardware orchestrator UI, or a malicious tool. It is an embodied, high-performance, local-first 3D shadow simulation game where every ludic mechanic models authentic physical survival competence and stress-tests cybernetic edge infrastructure via outward-only synthetic telemetry (`QUALITY_SIMULATED = 0x01`).

* **Epic 1 (D1): Dwarf Fortress Cognitive Core:** Deep psychological modeling replacing simplistic need bars. Cache-aligned psychology state, companion episodic memory ring buffer (32 slots $\times$ 24B), Lyapunov stress homeostasis, breakdown FSM, and Strange Moods with 72-hour artifact crafting.
* **Epic 2 (D2): The Sims Indirect Founder Management:** Intimate, embodied stewardship of Founder 01. Finite caloric metabolism (2,000–4,500 kcal/day), circadian fatigue, advisory intention queue, spontaneous personal whims, logistic moral rejection heuristics, 25% utility hysteresis, and 30-tick watchdog timers.
* **Epic 3 (D3): Cities: Skylines Macro Infrastructure Reclamation & Salvage Bridging:** Cooperative reclamation and salvage bridging of decommissioned 12kV substation transformers, municipal water mains, asphalt, and dark fiber across Suburban Sprawl, Dense Urban, and Permaculture biomes, solved via Successive Over-Relaxation (SOR, $\omega = 1.6$) electrical flow modeling and thermodynamic load balancing across 3 bootstrapping epochs.
* **Epic 4 (D4): Stochastic Boundary Interface & Sovereign Stack Multiplayer:** Inhomogeneous Poisson arrival process modeling external legacy pressure, attenuated by 3 sovereign pillars (autarky with Genesis Epsilon Guard $\epsilon = 0.001$, social NPC mutual aid, and peer proximity mesh). Multiplayer executed exclusively via Sovereign Stack (L2–L7) over the `L1b <-> L2` UHAI IPC boundary with speed-of-light distance bounding.
* **Epic 5 (SDD): Scenario Driven Development Dynamic Ingestion & In Silico Compiler:** Dynamic ingestion pipeline for the 24 canonical MVS scenarios (`docs/scenarios/*.md`), automated JSON-LD schema parsing, dynamic Flecs ECS data binding, spatial voxel palette allocation, Gibsonian affordance generation, upward compilation loop, and automated G1–G6 verification.
* **Epic 6 (Core): High-Performance Engine Core & WebGPU Amanatides & Woo Raymarcher:** C++20 Flecs ECS, hierarchical DDA raymarching in WebGPU WGSL, 7.40 ms active frame budget (10.60 ms hard cap, 60 FPS), rigid 256.00 MB WASM linear memory partition table (zero runtime allocations), and cross-platform build pipeline (CMake, Ninja, LLVM 18, emsdk 3.1.56, vcpkg).
* **Epic 7 (UHAI): Universal Human-Agent Interface Shared-Memory IPC & SITL Telemetry:** Lock-free SPSC shared memory ring buffer at `/dev/shm/uhai_telemetry.ring` (< 50 µs latency, 3 cache line header isolation), Emscripten browser adapter (`SharedArrayBuffer`), strict enforcement of 5 Anti-Bleed Invariants, and outward-only airgap isolation between `QUALITY_SIMULATED` and `QUALITY_HARDWARE_ROOT`.
* **Epic 8 (Horizon Pillar 1 & 2): Tactile 3D Navigation, POI Arcball Camera & Spatial Context Designator:** Dynamic Point-of-Interest (POI) spherical arcball camera (`Shift + Right Click` raycast origin with 3D wireframe reticle, spherical $(\theta, \phi)$ orbit, `Shift + Mouse Wheel` zoom $r$, and automatic/manual roof cutaway `z_cutaway_level`), Right-Click voxel evaluator & contextual action menu (including full wall/door/roof/hearth construction and prerequisite badges), and Left-Click 3D designation volume painter with stockpile hauling and 264-byte `SpatialIntentPayload` dispatch over `L1b -> L2` UHAI.
* **Epic 9 (Horizon Pillar 3): Biological & Psychological Homeostasis — The Chill Loop:** Dirty-flag cached $O(H)$ sky-column + 512-voxel BFS detection of enclosed thermally insulated rooms ($>18^\circ\text{C}$, with airtight passable `MAT_DOOR` voxels and `F1–F4` Info-View shader overlays) halting cold caloric burn and restoring `+45 Focus/hr` during sleep, the **Day 1 Genesis Survival Camp Bootstrap**, somatic food/water consumption affordances, designated Recreation Zones draining `Stress` (`-25%/hr` solo, `-40%/hr` social), and the 100% `Stress` involuntary `BURNOUT` state with anti-soft-lock safeguards.
* **Epic 10 (Horizon Pillar 4): Simulation Temporal Controls (Time Warp) & Runtime Hygiene:** HUD time-step multiplier controls (`[ || Pause ] [ > 1x ] [ >> 10x ] [ >>> 100x ]`) decoupling `g_ecs->progress()` sub-stepping from once-per-frame 60 Hz WebGPU rendering and aggregated UHAI telemetry emission, Emergency Warp Brakes, a live Net Trajectory HUD Ticker, logical-epoch `spdlog` throttling, and isolation of the 60-tick auto-exit behind `--headless-ci`.
* **Epic 11 (Horizon Pillar 5): Migration Waves & Legacy NPC Assimilation:** Solo Founder genesis followed by stochastic Layer 7 Migration Waves (`MigrationWaveArrivalEvent`, 32B `alignas(8)`) routed down the stack (`L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> L1b`); Legacy NPCs arrive at `Ring R6 (Unverified Migrant)` with fiat scarcity habits and high baseline `Stress`, advancing to `Ring R3 (Probationary Resident)` via Layer 3 PoTW verification ($\ge 7$ days sheltered homeostasis + $\ge 36.0\text{ MJ}$ sweat-equity) and to `Ring R1 (Vouched Steward)` via a Layer 5 2-of-3 FROST ceremony (`SCN-TAU-GENESIS`).
* **Epic 12 (Horizon Pillar 6): The Shared Virtual Landscape (`Arcadia server-01`) & Mandatory Milestone Epoch Sync:** Voluntary multiplayer landscape synchronization across `Arcadia (server-01)` consuming headless 24-byte `StateDeltaOp` cadastral records (`SS-EP9-001`) via `L3 -> L2 -> L1b` UHAI, paired with the Mandatory Milestone (End-of-Month Epoch Barrier at `EPOCH_BOUNDARY_TICKS = 864,000`) featuring a `SOLO_AUTARKIC` zero-peer fast-path, an Interactive Blueprint Planning Phase during `EPOCH_SYNC_LOCKED`, and a 3D Holographic Horizon Wave reveal upon Epoch reconciliation.

### 2.2. Complete 14-Sprint Release Roadmap (Sprints 1–10 Completed Baseline + Dual Sprints 11–14)

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 OASIS ENGINE 14-SPRINT MASTER ROADMAP (V6.0 HORIZON)                                  │
├───────────┬────────────┬──────────────────────────────┬───────────────────────────────────────────────────┬───────────┤
│ Sprint    │ Status     │ Focus Area                   │ Delivered Core Capabilities                       │ Velocity  │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 1**  │ COMPLETED  │ Core Engine & Physical UHAI  │ WebGPU DDA raymarcher foundation, 256MB WASM      │ 58 Points │
│           │            │ Foundations                  │ memory layout, /dev/shm SPSC ring (<50 µs),       │           │
│           │            │                              │ 32-bit voxels, basic facets, Founder embodiment.  │           │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 2**  │ COMPLETED  │ Cognitive Architecture &     │ 32-slot episodic memory ring, SDD dynamic         │ 62 Points │
│           │            │ Dynamic SDD Ingestion        │ ingestion pipeline (24 scenarios), SOR solver     │           │
│           │            │                              │ (ω=1.6), 12kV salvage bridging, moral rejection.  │           │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 3**  │ COMPLETED  │ Stress Homeostasis, Biomes   │ Dual-timescale Lyapunov damping, clinical recovery│ 60 Points │
│           │            │ & Stochastic Boundary        │ attractor, breakdown FSM, Suburban/Urban/Eco      │           │
│           │            │                              │ biomes, water/fiber reclamation, Poisson boundary.│           │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 4**  │ COMPLETED  │ Strange Moods, Terminal      │ 72-hour Strange Mood artifact crafting, Terminal  │ 62 Points │
│           │            │ Decoupling & SITL Harness    │ Decoupling Singularity (Omega), affordances,      │           │
│           │            │                              │ watchdog hysteresis, G1–G6 SITL digital twin.     │           │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 5**  │ COMPLETED  │ Physical Fabrication &       │ Tactical Override: 3D Amanatides & Woo mouse      │ Baseline  │
│           │            │ 3D Voxel Raycaster           │ raycaster (`raycaster.hpp`), voxel fabrication.   │ Override  │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 6**  │ COMPLETED  │ Mac Trackpad Panning &       │ Tactical Override: macOS trackpad/mouse 2D camera │ Baseline  │
│           │            │ Dynamic Voxel SSBO           │ panning, real-time dynamic WebGPU Voxel SSBO sync.│ Override  │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 7**  │ COMPLETED  │ Founder 3D Avatar &          │ Tactical Override: Embodied Founder 01 3D voxel   │ Baseline  │
│           │            │ HTML/CSS Vitals HUD          │ avatar rendering, reactive HTML/CSS Vitals HUD.   │ Override  │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 8**  │ COMPLETED  │ Ambient Thermal              │ Tactical Override: Ambient temperature cycle,     │ Baseline  │
│           │            │ Homeostasis Loop             │ thermodynamic exposure caloric multiplier penalty.│ Override  │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 9**  │ COMPLETED  │ Temporal Flow Contracts      │ Tactical Override: Continuous resource flow       │ Baseline  │
│           │            │                              │ contracts (`FlowContract`) across UHAI IPC.       │ Override  │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 10** │ COMPLETED  │ Layer 6 Companion App &      │ Tactical Override: `col-gatewayd` 8th bridge      │ Baseline  │
│           │            │ `col-gatewayd` Integration   │ daemon, Intent U-Turn Law, Companion UI sync.     │ Override  │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 11** │ PLANNED    │ Tactile 3D Navigation,       │ Epic 8 & Epic 10: POI Spherical Arcball Camera,   │ 35 Points │
│ (Dual)    │ (Horizon)  │ Designator & Time Warp       │ Roof Cutaway, Context Menu & Painter, Time Warp,  │           │
│           │            │                              │ Emergency Warp Brake, `--headless-ci` isolation.  │           │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 12** │ PLANNED    │ Biological & Psychological   │ Epic 9: Day 1 Genesis Camp Kit, cached flood-fill │ 21 Points │
│ (Dual)    │ (Horizon)  │ Homeostasis (The Chill Loop) │ (>18°C, `+45 Focus/hr`), `F1–F4` overlays, food/  │           │
│           │            │                              │ water intake, Recreation Zones, `BURNOUT` FSM.    │           │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 13** │ PLANNED    │ Migration Waves & Legacy     │ Epic 11: `L7->L2->L1b` Migration Wave ingress,    │ 23 Points │
│ (Dual)    │ (Horizon)  │ NPC Assimilation             │ multi-agent NPC spawner, scarcity regression,     │           │
│           │            │                              │ PoTW `R6->R3` & FROST `R3->R1` Trust onboarding.  │           │
├───────────┼────────────┼──────────────────────────────┼───────────────────────────────────────────────────┼───────────┤
│ **SP 14** │ PLANNED    │ Shared Landscape (`Arcadia`) │ Epic 12: `Arcadia (server-01)` 24B `StateDeltaOp` │ 22 Points │
│ (Dual)    │ (Horizon)  │ & Mandatory Epoch Sync       │ projection, Mandatory Milestone planning phase,   │           │
│           │            │                              │ `SOLO_AUTARKIC` fast-path, Horizon Wave reveal.   │           │
├───────────┴────────────┴──────────────────────────────┴───────────────────────────────────────────────────┼───────────┤
│ TOTAL ARTICULATED BACKLOG VELOCITY BUDGET (EPICS 1–12, 51 STORIES)                                        │ 343 Points│
└───────────────────────────────────────────────────────────────────────────────────────────────────────────┴───────────┘
```

> [!NOTE]
> **Post-Sprint 10 Engineering Reality & Source-Tree Reconciliation Note:**
> To prevent architectural assumptions from diverging from the live C++20 source tree in `core/1_simulation/src/` and `CMakeLists.txt`, engineers executing Dual Sprints 11–14 must account for the exact post-Sprint 10 implementation state:
> 1. **Unified `PsychologyComponent` (816 Bytes):** In `core/1_simulation/src/components/psychology.hpp`, the 64-byte psychological state (`AgentPsychologyComponent`) and 768-byte 32-slot episodic memory ring (`AgentMemoryRingComponent`) are currently unified into a single `alignas(64) PsychologyComponent` (`sizeof == 816` bytes: 48B core fields + 768B ring). Epics 8–12 extend or query this exact layout without introducing unaligned padding under `-Wpadded`.
> 2. **Fragment-Shader DDA & Inline WGSL Parity:** The active WebGPU renderer executes the Amanatides & Woo DDA raymarcher inside a full-screen fragment shader synced across both `core/1_simulation/shaders/raymarch.wgsl` and the inline C++ header `core/1_simulation/src/raymarch_wgsl.hpp`, paired with the CPU raycaster in `core/1_simulation/src/raycaster.hpp` and dynamic voxel storage in `DynamicVoxelStore` (`voxel_ssbo.hpp`). Currently, DDA bounds are clamped to `|x| <= 28, z in [-28, 28]` (`MAX_STEPS = 128`); **Sprint 11 (`STORY-8.1` & `STORY-8.2`) expands these bounds in lockstep across all three files to `|x| <= 64, y in [0, 32], z in [-64, 64]` (`MAX_STEPS = 192`).**
> 3. **Coupled Render/Tick Loop in `main.cpp`:** In the Sprint 10 baseline, `RenderWebGPUFrame()` and `EM_ASM(updateHUD(...))` are registered inside a Flecs system that runs on every `g_ecs->progress()` call, and `main.cpp` auto-exits at `ticks >= 60`. **Sprint 11 (`STORY-10.1` & `STORY-10.2`) decouples `RenderWebGPUFrame()` and DOM HUD updates from `g_ecs->progress()`** so `10x/100x` Time Warp sub-steps simulation ticks without re-rendering or overflowing the 1,024-slot UHAI SPSC ring buffer, and isolates the 60-tick exit behind `--headless-ci`.
> 4. **Compiled Sovereign Stack Daemons in `CMakeLists.txt`:** The root `CMakeLists.txt` currently builds 6 daemon targets (`col-telemetryd`, `col-meshd`, `col-execd`, `col-commonsd`, `col-gatewayd`, and `col-adversaryd`). Dedicated build targets for Layer 3 (`col-storaged`) and Layer 5 (`col-kmsd`) are added in lockstep with the Sovereign Stack V4.0 backlog during Dual Sprints 11–14.

---

## 3. Surgical Separation of Concerns & Anti-Bleed Invariants Manifest

Backlog V6.0 strictly codifies the surgical excision of all monolithic anti-patterns and bleeding interfaces between the Oasis Game Engine (Layer 1b Shadow Simulator) and the Sovereign Stack (Layers 1–7), including the mandatory **Outward-Only Telemetry Producer, Strict Layer Adjacency (`L1b <-> L2`), & Simulation Demarcation Laws** ratified in `docs/BOOK_TEAM_DRIFT_REPORT.md`:

> [!IMPORTANT]
> **Outward-Only Telemetry Producer, Layer Adjacency (`L1b <-> L2`), & Simulation Demarcation Invariants:**
> 1. **Outward Producer Only:** Oasis is strictly an **outward producer** of synthetic telemetry tagged `QUALITY_SIMULATED (0x01)` into the UHAI ring buffer for Layer 2–7 stress-testing. Oasis **never ingests live physical sensor telemetry** from real-world hardware (`QUALITY_HARDWARE_ROOT = 0x02`) to recalibrate the game simulation.
> 2. **Strict Layer Adjacency (`L1b <-> L2` Only):** Oasis (Layer 1b) interfaces **exclusively** with the `/dev/shm` UHAI shared-memory ring (`L1b <-> L2 col-meshd SITL ingress/egress`). Oasis **never** communicates directly with `col-storaged` (L3), `col-execd` (L4), `col-kmsd` (L5), `col-commonsd` (L6), or `col-adversaryd` (L7). All upward intents (`SpatialIntentPayload`, `EpochBarrierReachedPayload`) enter at `L2` and traverse the stack sequentially (`L1b -> L2 -> L3 -> L4 -> L5 -> L6 -> L7`), and all downward simulation events (`MigrationWaveArrivalEvent`, `StateDeltaOp`, `EpochReconciledAckPayload`) traverse sequentially downward to `L2` before Oasis reads them from UHAI (`L2 -> L1b`).
> 3. **Strict Context Demarcation:** Real and simulated contexts remain unmistakably distinct at all times. Oasis is a local-first shadow simulator and training ground—never a "transitionary UI" that silently turns into a real-world physical hardware orchestrator. Any synthetic packet tagged `QUALITY_SIMULATED (0x01)` is mathematically barred by `col-telemetryd`'s Hardware Airgap Filter from actuating physical relays, breakers, or solenoids.

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       OASIS ARCHITECTURAL BOUNDARY PURGE MANIFEST                                     │
├──────────────────────────────┬───────────────────────────────┬────────────────────────────────────────────────────────┤
│ Contaminated Subsystem       │ Purged Anti-Pattern           │ Backlog V6.0 Corrective Boundary & Invariant           │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `did_crypto_generator.cpp`   │ In-engine libsodium Ed25519   │ PURGED (Anti-Bleed Inv 2). Oasis holds only opaque     │
│                              │ key generation in C++ loop.   │ 32-byte `founder_did_handle`. Keys locked in `col-kmsd`.│
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `crdt_sync_engine.cpp`       │ In-engine libp2p/WebRTC       │ PURGED (Anti-Bleed Inv 3). Zero P2P sockets in engine. │
│                              │ Gossipsub networking.         │ Multiplayer state sync owned by `col-storaged` (L3)    │
│                              │                               │ and delivered to Oasis strictly via `L3 -> L2 -> L1b`. │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `bpmn_parser.cpp`            │ In-engine XML parsing of      │ PURGED (Anti-Bleed Inv 4). Oasis emits 264-byte binary │
│                              │ BPMN schemas via `pugixml`.   │ `SpatialIntentPayload` (`L1b->L2->L3->L4`); `col-execd`│
│                              │                               │ runs the keyless, blind 10 Hz BPMN VM (L4).            │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `Layer7DrainAccumulator`     │ In-engine fiat accounting and │ PURGED (Anti-Bleed Inv 5). Oasis simulates watts/joules│
│                              │ utility tariff debiting.      │ only; `col-adversaryd` (L7) models external tariffs.   │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ `iot_telemetry_bridge.cpp`   │ Direct MQTT / CoAP hardware   │ PURGED (Anti-Bleed Inv 1). Real hardware drivers       │
│                              │ actuation in game runtime.    │ belong exclusively to `col-telemetryd` (L1).           │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ Direct L1b -> L3/L4/L5/L6/L7 │ Bypassing Layer 2 to call     │ PURGED (Layer Adjacency Invariant). Oasis talks ONLY   │
│ Cross-Layer Coupling         │ upper-stack daemons directly. │ to the UHAI ring (`L1b <-> L2 col-meshd SITL bridge`). │
├──────────────────────────────┼───────────────────────────────┼────────────────────────────────────────────────────────┤
│ Synthetic Telemetry Bleed &  │ Simulated data driving real   │ PURGED (Airgap & Outward-Only Invariant). Oasis is an  │
│ Live Sensor Ingestion Drift  │ relays, or live sensors       │ outward-only producer of `QUALITY_SIMULATED` (0x01) and│
│                              │ recalibrating the game loop.  │ never ingests live physical sensor telemetry.          │
└──────────────────────────────┴───────────────────────────────┴────────────────────────────────────────────────────────┘
```

---

## 4. Epic Summary & Story Point Allocation

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                        EPIC SUMMARY & STORY POINT ALLOCATION                                          │
├─────────┬──────────────────────────────────────────────────────────────────┬─────────────┬──────────────┬──────┬──────┤
│ Epic    │ Epic Title                                                       │ Priority    │ Stories      │ SP   │ Spt  │
├─────────┼──────────────────────────────────────────────────────────────────┼─────────────┼──────────────┼──────┼──────┤
│ **E1**  │ Dwarf Fortress Cognitive Architecture & Deep Psychology (D1)     │ P0 Critical │ 5 Stories    │ 33   │ 1–4  │
│ **E2**  │ The Sims Indirect Founder Management & Agency Loop (D2)          │ P0 Critical │ 5 Stories    │ 32   │ 1–4  │
│ **E3**  │ Cities: Skylines Macro Infrastructure Reclamation & Salvage (D3) │ P1 High     │ 6 Stories    │ 42   │ 2–3  │
│ **E4**  │ Stochastic Boundary Interface & Sovereign Stack Multiplayer (D4) │ P1 High     │ 5 Stories    │ 31   │ 3–4  │
│ **E5**  │ Scenario Driven Development (SDD) Dynamic Ingestion & Compiler   │ P0 Critical │ 6 Stories    │ 40   │ 2, 4 │
│ **E6**  │ High-Performance Engine Core & WebGPU Hierarchical Raymarcher    │ P0 Critical │ 5 Stories    │ 34   │ 1, 4 │
│ **E7**  │ Universal Human-Agent Interface (UHAI) IPC & SITL Telemetry      │ P0 Critical │ 5 Stories    │ 30   │ 1, 4 │
│ **E8**  │ Tactile 3D Navigation, POI Arcball Camera & Spatial Designator   │ P0 Critical │ 3 Stories    │ 23   │ 11   │
│ **E9**  │ Biological & Psychological Homeostasis — The Chill Loop          │ P0 Critical │ 3 Stories    │ 21   │ 12   │
│ **E10** │ Simulation Temporal Controls (Time Warp) & Runtime Hygiene       │ P0 Critical │ 2 Stories    │ 12   │ 11   │
│ **E11** │ Migration Waves & Legacy NPC Assimilation                        │ P1 High     │ 3 Stories    │ 23   │ 13   │
│ **E12** │ Shared Virtual Landscape (`Arcadia server-01`) & Epoch Sync      │ P1 High     │ 3 Stories    │ 22   │ 14   │
├─────────┴──────────────────────────────────────────────────────────────────┴─────────────┼──────────────┼──────┴──────┤
│ TOTAL (12 Epics, 51 Articulated User Stories)                                            │ 51 Stories   │ 343 SP      │
└──────────────────────────────────────────────────────────────────────────────────────────┴──────────────┴─────────────┘
```

---

## 5. Detailed Epics & User Stories

### Epic 1: Dwarf Fortress Cognitive Architecture & Deep Psychology (DF Cognitive Core)
**Epic ID:** `EPIC-1` | **Total Estimate:** 33 Story Points | **Priority:** P0 Critical | **Baseline:** Completed (Sprints 1–4)  
**Focus:** Implement the cache-aligned 64-byte `AgentPsychologyComponent`, companion 768-byte `AgentMemoryRingComponent` (32 slots $\times$ 24B, unified in the 816-byte `PsychologyComponent` in `psychology.hpp`), Big-Five facets, 8 Core Values enum, active thought buffer, dual-timescale Lyapunov stress homeostasis with clinical recovery attractor, 4-tier breakdown FSM, Strange Moods with 72-hour artifact crafting, and spatial trauma flashbacks.

---

#### Story 1.1: 64-Byte Bounded Psychological State & Core Values ECS Component
* **ID:** `STORY-1.1` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G4
* **User Story:** *As an Oasis simulation agent, I want my personality facets, core philosophical values, and emotional state packed into a cache-aligned ECS component, so that hundreds of agents can evaluate emotional stimuli without causing CPU cache misses or heap allocations.*
* **Technical Tasks:**
  1. Define `PersonalityFacets` struct (`alignas(4)`, 16 bytes) containing 10 signed 8-bit facets (`assertiveness`, `anxiety`, `empathy`, `self_discipline`, `curiosity`, `pragmatism`, `stoicism`, `sociability`, `discordance`, `artistic_drive`, plus 6 padding bytes).
  2. Define `CoreValues` struct (`alignas(4)`, 16 bytes) containing 8 signed 8-bit values (`AUTONOMY`, `STEWARDSHIP`, `SOLIDARITY`, `INGENUITY`, `TENACITY`, `PRAGMATISM`, `TRANSPARENCY`, `BEAUTY`, plus 8 SIMD alignment padding bytes).
  3. Define `AgentPsychologyComponent` (`alignas(64)`, 64 bytes; or unified 816-byte `PsychologyComponent` when paired inline with the 768-byte memory ring) packing `PersonalityFacets` (16B), `CoreValues` (16B), `active_mood` (Fixed32, 4B), `cumulative_stress` (Fixed32, 4B), `short_term_focus` (uint8_t, 1B), `breakdown_state` (uint8_t, 1B), `active_thought_count` (uint8_t, 1B), `ring_head_index` (uint8_t, 1B), `flags` (uint8_t, 1B), and alignment padding bytes.
  4. Register component in Flecs ECS archetype tables and verify size and `alignof == 64` via `static_assert`.
  5. Implement deterministic initialization from split-stream PCG32 seeded by agent ID.
* **Acceptance Criteria:**
  * **Given** an entity is spawned in the Flecs ECS world,
  * **When** `AgentPsychologyComponent` / `PsychologyComponent` is attached,
  * **Then** cache-line alignment (`alignof == 64`) is verified at compile time via `static_assert`,
  * **And** accessing and mutating values executes in $< 5\text{ ns}$ without dynamic heap allocations,
  * **And** satisfies Quality Gate G4 (deterministic memory footprint).

---

#### Story 1.2: 768-Byte Companion Episodic Memory Ring & Salience Decay
* **ID:** `STORY-1.2` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G4
* **User Story:** *As an agent, I want high-valence sensory and social events recorded into a fixed 32-slot circular ring buffer with Ebbinghaus salience decay and permanent trauma protection, so that lived hardships and triumphs alter my long-term behavior without unbounded memory growth.*
* **Technical Tasks:**
  1. Define `EpisodicMemoryNode` struct (24 bytes packed, `alignas(8)`) containing `timestamp_ticks` (uint32_t, 4B), `event_type` (uint16_t, 2B), `subject_entity_id` (uint16_t, 2B), `emotional_valence` (Fixed16 Q8.8, 2B), `salience` (uint16_t, 2B), `voxel_x/y/z` (3 $\times$ uint16_t, 6B), and `flags` (uint16_t, 2B: Bit 0 `PERMANENT_MEMORY`, Bits 1-15 trigger count), plus 4 padding bytes for 8-byte alignment (yielding `sizeof(EpisodicMemoryNode) == 24` bytes).
  2. Implement `AgentMemoryRingComponent` (`alignas(8)`, exactly 768 bytes) storing an array of 32 `EpisodicMemoryNode` structs ($32 \times 24 = 768\text{ B}$).
  3. Implement diurnal Ebbinghaus salience decay running at midnight sleep boundaries: $S(t) = S_0 \cdot \exp(-\Delta t / \tau_{\text{decay}}) \cdot (1 + |\text{valence}| / V_{\max})$.
  4. Implement salience-priority ring replacement: nodes with `salience >= 80` receive the `PERMANENT_MEMORY` flag (`0x0001`). When the 32-slot ring wraps, the engine evicts the oldest entry with the lowest salience that lacks `PERMANENT_MEMORY`.
  5. Implement spatial hash flashback queries triggering when an agent moves within 4 decimeters of a trauma voxel coordinate, injecting a negative thought into the active thought buffer.
* **Acceptance Criteria:**
  * **Given** Founder 01 witnesses a legacy compliance enforcement event at voxel coordinate $(95, 32, 48)$ with salience 95,
  * **When** 50,000 simulation ticks elapse and 40 subsequent events occur,
  * **Then** the trauma memory node remains un-evicted in the ring buffer due to `PERMANENT_MEMORY` flag protection,
  * **And** walking within 4 decimeters of $(95, 32, 48)$ triggers a sensory flashback injecting `THOUGHT_COMPLIANCE_TRAUMA`, dropping focus by 30 points,
  * **And** `sizeof(AgentMemoryRingComponent) == 768` with zero heap allocations.

---

#### Story 1.3: Dual-Timescale Lyapunov Stress Homeostasis & Clinical Attractor
* **ID:** `STORY-1.3` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G4
* **User Story:** *As the simulation engine, I want to integrate active thought valence into a cumulative stress accumulator governed by dual-timescale Lyapunov damping with clinical recovery attractors, so that agents experience authentic psychological breakdowns without entering unrecoverable death spirals.*
* **Technical Tasks:**
  1. Implement $Q24.8$ fixed-point stress differential integration evaluating mood against resilience: $\frac{d\mathcal{S}}{dt} = \dot{S}_{\text{acute}} - \kappa_{\text{resilience}} \cdot (\text{Focus} + \text{MutualAid}) - \frac{1}{\tau_{\text{baseline}}} \tanh\left(\frac{\mathcal{S} - \mathcal{S}_{\text{baseline}}}{S_0}\right)$.
  2. Implement fast acute damping ($\tau_{\text{acute}} = 30\text{ s}$) modulating immediate emotional shocks (dropped wrench, cold soup).
  3. Implement slow chronic integration ($\tau_{\text{baseline}} = 7\text{ days}$) accumulating deep environmental hardship (winter freeze, partner illness).
  4. Implement non-linear restoring attractor $\tanh\left(\frac{\mathcal{S} - \mathcal{S}_{\text{baseline}}}{S_0}\right)$ with resilience damping from high focus and warm communal meals.
  5. Implement **Clinical Recovery Attractor**: when stress crosses $\mathcal{S} > 80,000$ (Severe Catatonia), peer agents autonomously trigger nursing behaviors (warm soup, blankets), enforcing a guaranteed restorative drift ($\Delta \mathcal{S}_{\text{clinical}} = -150\text{ units/hour}$) to break absorbing death spirals.
* **Acceptance Criteria:**
  * **Given** an agent experiences persistent negative stimuli pushing cumulative stress to 85,000,
  * **When** peers provide soup and shelter affordances over 48 in-game hours,
  * **Then** cumulative stress recovers below 50,000 via the clinical recovery attractor,
  * **And** stress values remain strictly clamped within $[-100,000, +100,000]$ in $Q24.8$ fixed-point arithmetic without numerical overflow.

---

#### Story 1.4: 4-Tier Breakdown FSM & Somatic Behavioral Invariants
* **ID:** `STORY-1.4` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G4
* **User Story:** *As an agent experiencing extreme stress, I want my behavioral state machine to transition through 4 distinct tiers of psychological breakdown, so that mental suffering manifests in observable somatic actions and task refusals.*
* **Technical Tasks:**
  1. Implement 4-tier Breakdown State Machine in Flecs ECS:
     * Tier 0: Normal ($\mathcal{S} < 25,000$).
     * Tier 1: Mild Irritability ($25,000 \le \mathcal{S} < 50,000$) — snaps in mess hall, refuses non-essential chore assignments.
     * Tier 2: Moderate Split State ($50,000 \le \mathcal{S} < 80,000$) — randomly splits into *Tantrum* (kicks inverters, smashes tools, damages voxels) or *Melancholy* (sits in dirt, stares blankly, ignores user queue).
     * Tier 3: Severe Catatonia ($\mathcal{S} \ge 80,000$) — complete motor shutdown, requires spoon-feeding (with $100\%$ normalized stress `STATE_BURNOUT` codified in `STORY-9.3`).
  2. Bind breakdown state transitions to agent animation state flags and somatic audio cues (muttering, heavy breathing, sobbing).
  3. Ensure agents in Tier 2/3 release all tool locks and fail active BPMN task leases cleanly.
* **Acceptance Criteria:**
  * **Given** an agent's cumulative stress reaches 55,000 with high `discordance`,
  * **When** the agent transitions to Tier 2 Tantrum,
  * **Then** the agent drops their current task, strikes an adjacent equipment voxel (inflicting 15 durability damage),
  * **And** advisory directives from the player are rejected with an audible frustration cue.

---

#### Story 1.5: Strange Moods & 72-Hour Masterwork Artifact Crafting
* **ID:** `STORY-1.5` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G1, G4
* **User Story:** *As an inspired artisan agent, I want to occasionally enter a manic "Strange Mood" and seize a workshop, demanding specific materials to craft a masterwork artifact within 72 hours, so that high creative drive produces transformative community assets.*
* **Technical Tasks:**
  1. Implement Strange Mood trigger evaluation: requires $\text{ArtisticDrive} \ge 70$, $\text{Autonomy} \ge 60$, $\text{Focus} \ge 180$, and random roll against creative potential.
  2. Implement Workshop Lockout: agent claims the nearest fabrication workshop (Scenario Alpha/Theta), barring peers from entry.
  3. Implement Material Demands Generator: agent requests 3-4 specific local materials (e.g., 4-gauge copper wire, seasoned white oak, biochar, beeswax).
  4. Implement 72-Hour Crafting Countdown:
     * Success (supplied within 72h): agent crafts a **Masterwork Artifact** (e.g., High-Efficiency MPPT Inverter, Hand-Wound LoRa Transceiver), granting $+20\%$ permanent parcel efficiency, $+15$ permanent Stoicism, and setting stress to 0.
     * Failure (timeout): agent collapses into severe breakdown or melancholia.
  5. Add Plumbob 2.0 prismatic iridescent shimmer visual shader effect during the mood.
* **Acceptance Criteria:**
  * **Given** an artisan agent enters a Strange Mood and claims the electronics bench,
  * **When** required copper wire and biochar materials are delivered within 72 in-game hours,
  * **Then** the agent produces an artifact entity with the `Masterwork` tag, parcel exergy efficiency gains $+20\%$,
  * **And** if materials are denied for 72 hours, the agent transitions to Catatonia and stress locks at $+100,000$.

---

### Epic 2: The Sims Indirect Founder Management & Agency Loop (The Sims Indirect Founder)
**Epic ID:** `EPIC-2` | **Total Estimate:** 32 Story Points | **Priority:** P0 Critical | **Baseline:** Completed (Sprints 1–4)  
**Focus:** Implement Founder 01 mortal embodiment (2,000–4,500 kcal/day, circadian fatigue), advisory intention queue, spontaneous whims, Gibsonian environmental affordance guidance, logistic moral rejection heuristics, 25% utility hysteresis, 30-tick progress watchdog with exponential backoff, and Trust Ring delegation.

---

#### Story 2.1: Founder 01 Mortal Embodiment & Caloric Fatigue ECS System
* **ID:** `STORY-2.1` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G1
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
* **ID:** `STORY-2.2` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G4
* **User Story:** *As Founder 01, I want chores designated on the Cadastral Slate to enter an advisory intention queue that I evaluate alongside my own spontaneous whims, so that I maintain authentic human agency.*
* **Technical Tasks:**
  1. Implement `AdvisoryIntentionQueue` component: holds up to 8 advisory tasks enqueued by the player from the Cadastral Slate or Spatial Context Designator.
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
* **ID:** `STORY-2.3` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G4
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
* **ID:** `STORY-2.4` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G4
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
* **ID:** `STORY-2.5` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G4
* **User Story:** *As the gameplay AI system, I want 25% utility hysteresis, atomic action commits, distance-adaptive TTL leases, and a 30-tick progress watchdog, so that agents never enter erratic decision flickers or pathfinding deadlocks.*
* **Technical Tasks:**
  1. Implement 25% Utility Hysteresis: agent executing task $A$ switches to candidate $B$ only if $U(B) \ge 1.25 \cdot U(A)$.
  2. Implement Atomic Action Commit: once physical execution (welding, sawing, pouring) begins, interruptions from non-critical whims are locked out until the phase finishes.
  3. Implement Distance-Adaptive TTL Lease with 15-tick heartbeat transit renewal: $\text{TTL}_{\text{init}} = \max\left(60, \left\lceil \frac{d}{\text{speed}} \right\rceil \times 1.5\right)\text{ ticks}$. Halting movement allows the lease to cleanly expire.
  4. Implement 30-Tick Progress Watchdog: if distance to goal fails to decrease over 30 consecutive ticks ($1.0\text{ s}$ at 30 Hz), transition task to `ACTION_BLOCKED`, step back 1 dm, release locks, and blacklist destination with exponential backoff ($30\text{s}, 60\text{s}, 120\text{s}, 300\text{s}$).
* **Acceptance Criteria:**
  * **Given** an agent pathfinding to a tool locker is blocked by an obstacle,
  * **When** distance to target fails to decrement for 30 consecutive ticks,
  * **Then** the watchdog fires, releasing the locker lease without deadlocking,
  * **And** the destination is blacklisted for 30 seconds while the agent picks an alternate affordance.

---

### Epic 3: Cities: Skylines Macro Infrastructure Reclamation & Salvage Bridging & Multi-Biome Simulation
**Epic ID:** `EPIC-3` | **Total Estimate:** 42 Story Points | **Priority:** P1 High | **Baseline:** Completed (Sprints 2–3)  
**Focus:** Implement macro infrastructure reclamation and salvage bridging (decommissioned 12kV substation transformers, municipal water mains, asphalt/urbanite, dark fiber conduits), transformer thermal load balancing and legacy treaty/tariff compliance counters (`col-adversaryd` DES), Successive Over-Relaxation (SOR, $\omega = 1.6$) electrical solver with virtual earth grounding, 3 biomes (Suburban, Urban, Permaculture), 128-chunk active sector visibility window, and the 3 Bootstrapping Epochs.

---

#### Story 3.1: Decommissioned 12kV Substation Transformer Salvage & Thermal Load Balancing
* **ID:** `STORY-3.1` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G1, G6
* **User Story:** *As Founder 01, I want to bridge decommissioned 12kV substation step-down transformers to our enclave microgrid while managing transformer winding thermal limits and legacy utility treaty/tariff compliance thresholds (`col-adversaryd` DES), so that we safely reclaim surplus grid exergy into local battery banks without thermal overload or tariff penalties.*
* **Technical Tasks:**
  1. Implement substation transformer salvage bridging interaction: requires insulated lugs, step-down coupling protection, and zero-load disconnect verification.
  2. Model exergy yield: delivers up to $7.2\text{ kW}$ electrical power into connected microgrid battery banks.
  3. Implement Transformer Thermal Load & Legacy Tariff Compliance Counter: drawing $> 30\text{ A}$ exceeds winding thermal dissipation ratings and contractual peak-demand treaty bounds; thermal/tariff counter increments $+1\%$ per 100 ticks; reaching $100\%$ triggers a legacy utility compliance tariff audit or protective breaker trip scheduled by `col-adversaryd` DES. Balancing load below $< 10\text{ A}$ safely cools windings and dissipates the counter at $-0.5\%$ per 100 ticks.
  4. Model arc flash hazard: improper ungrounded bridging during rain triggers an electrical fault, damaging equipment and injuring the agent.
* **Acceptance Criteria:**
  * **Given** Founder 01 draws $35\text{ A}$ across a salvaged transformer bridge for 10,000 simulation ticks,
  * **When** the thermal/tariff compliance counter increments past $100\%$,
  * **Then** a legacy utility compliance audit / breaker trip event is scheduled by Layer 7 (`col-adversaryd` DES),
  * **And** balancing current draw to $8\text{ A}$ cools the transformer and returns the compliance counter cleanly to $0\%$, satisfying Gate G1.

---

#### Story 3.2: Successive Over-Relaxation (SOR) Electrical Flow Solver
* **ID:** `STORY-3.2` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G1
* **User Story:** *As the electrical simulation system, I want an SOR flow solver with relaxation factor $\omega = 1.6$ and virtual earth grounding, so that high-contrast conductance networks converge in $< 20$ iterations without floating matrix singularities.*
* **Technical Tasks:**
  1. Formulate nodal admittance matrix equation: $\mathbf{G}\mathbf{V} = \mathbf{I}$ across grid lines, salvage transformer bridges, inverters, and battery banks.
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

#### Story 3.3: Municipal Water, Ductile Iron Reclamation & Dark Fiber Conduit Bridging
* **ID:** `STORY-3.3` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G1, G2
* **User Story:** *As Founder 01, I want to excavate 2 meters to install a pressure-regulated saddle bridge on a ductile iron water main, divert stormwater runoff into biochar reed beds, and splice decommissioned roadside dark fiber conduits, so that our settlement secures abundant water and feeds reclaimed optical bandwidth into Sovereign Stack Layer 2 `col-meshd` mesh routing.*
* **Technical Tasks:**
  1. Implement trench excavation through asphalt/dirt voxels to locate 4-inch ductile iron water mains.
  2. Implement pressure-regulated saddle valve installation affordance; provides regulated water flow ($35\text{ PSI}$, up to $25\text{ L/min}$).
  3. Implement hydraulic pressure balance monitoring: drawing $> 40\text{ L/min}$ exceeds municipal pressure-head stability bounds and triggers SCADA pressure-drop telemetry in `col-adversaryd` DES.
  4. Implement stormwater diversion swales routing street runoff through gravel, mycelium bio-filtration, and biochar reed beds (Scenario Mu) to remove heavy metals and hydrocarbons.
  5. Implement roadside dark fiber conduit reclamation and optical mechanical splicing affordance: locate subterranean fiber conduits in utility easements, execute low-loss optical splicing, and route simulated link capacity via UHAI into Sovereign Stack Layer 2 `col-meshd` mesh routing while maintaining optical return loss within carrier OTDR compliance thresholds.
* **Acceptance Criteria:**
  * **Given** a 4-inch water main voxel at depth $z = -20\text{ dm}$,
  * **When** Founder 01 installs a pressure-regulated saddle valve and opens flow at $20\text{ L/min}$,
  * **Then** domestic cisterns fill at $20\text{ L/min}$ within safe hydraulic pressure bounds,
  * **And** diverting untreated storm runoff into food crops without biochar filtration contaminates soil voxels,
  * **And** splicing dark fiber conduits establishes an optical data link reported to Layer 2 `col-meshd` with $< 1\text{ ms}$ UHAI serialization delay and clean OTDR return-loss compliance.

---

#### Story 3.4: Asphalt Jackhammering & Urbanite Concrete Quarrying
* **ID:** `STORY-3.4` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G1, G4
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
* **ID:** `STORY-3.5` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G1, G2
* **User Story:** *As the simulation engine, I want distinct biome profiles for Suburban Sprawl, Dense Urban Blocks, and Permaculture Eco-Villages, so that players face unique spatial envelopes, materials, and legacy regulatory environments.*
* **Technical Tasks:**
  1. Define Suburban Sprawl profile ($128 \times 128 \times 16\text{ dm}$): horizontal layout, overhead power lines, shallow water mains, turf grass, HOA covenant compliance assessments ($150/day tariff model).
  2. Define Dense Urban Block profile ($64 \times 64 \times 64\text{ dm}$): deep vertical canyons, high-voltage conduit vaults, steam tunnels, structural steel, municipal building code and thermal envelope audits.
  3. Define Permaculture Eco-Village profile ($96 \times 96 \times 32\text{ dm}$): contoured topography, keyline swales, cob, biochar, downstream watershed treaty allocations.
  4. Configure procedural terrain generator to instantiate chosen biome profile upon new game boot.
* **Acceptance Criteria:**
  * **Given** the user selects the Dense Urban Block biome,
  * **When** the simulation world initializes,
  * **Then** vertical height limit expands to 64 decimeters with steel/concrete voxel palettes,
  * **And** legacy regulatory pressure shifts from suburban HOA covenant assessments to urban thermal envelope audits.

---

#### Story 3.6: Spatial Chunk Memory & 128-Chunk Active Visibility Window
* **ID:** `STORY-3.6` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G1, G4
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
**Epic ID:** `EPIC-4` | **Total Estimate:** 31 Story Points | **Priority:** P1 High | **Baseline:** Completed (Sprints 3–4)  
**Focus:** Implement the Stochastic Boundary Interface via an inhomogeneous Poisson process, 3 attenuation pillars with Genesis Epsilon Guard ($\epsilon = 0.001$), multiplayer executed exclusively via Sovereign Stack (L2–L7) over the `L1b <-> L2` UHAI IPC boundary, cryptographic speed-of-light distance bounding, and isoperimetric perimeter consolidation.

---

#### Story 4.1: Inhomogeneous Poisson Arrival Process & Legacy Regimes
* **ID:** `STORY-4.1` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G2, G3
* **User Story:** *As the boundary simulation system, I want external legacy regulatory and environmental shocks to arrive according to an inhomogeneous Poisson process modulated by macro-economic regimes, so that external pressure feels unpredictable yet systematically responsive to enclave resilience.*
* **Technical Tasks:**
  1. Implement Poisson event generator: arrival intensity $\lambda(t) = \max\left(\lambda_{\min}, \lambda_{\mathcal{R}} \cdot \exp(-\Psi(t))\right)$.
  2. Define macro-economic regimes $\mathcal{R}(t)$: Baseline-Stability ($\lambda_0 = 0.005$), Regulatory-Scrutiny ($\lambda_0 = 0.02$), Tariff-Escalation ($\lambda_0 = 0.08$), Grid-Instability ($\lambda_0 = 0.25$).
  3. Enforce irreducible external shock floor: $\lambda_{\min} = 0.001\text{ s}^{-1}$ (approx. 1 event per 16.6 minutes), ensuring infrastructure resilience is continuously tested.
  4. Schedule stochastic boundary events (zoning audits, utility brownouts, tariff surcharges, watershed quality inspections) when the Poisson timer expires.
* **Acceptance Criteria:**
  * **Given** the node is in the Regulatory-Scrutiny regime with zero sovereign attenuation ($\Psi(t) = 0$),
  * **When** the simulation executes for 10,000 seconds,
  * **Then** boundary shock arrivals follow Poisson distribution with mean intensity $\lambda = 0.02\text{ s}^{-1}$,
  * **And** arrival rate never drops below the irreducible floor $\lambda_{\min} = 0.001\text{ s}^{-1}$.

---

#### Story 4.2: Three Attenuation Pillars & Genesis Epsilon Guard
* **ID:** `STORY-4.2` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G1, G4
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
  * **And** building 100% solar autarky, welcoming 5 trusted citizens, and meshing with 3 peer nodes reduces external shock frequency by $> 85\%$.

---

#### Story 4.3: Sovereign Stack Exclusive Multiplayer Protocol Matrix
* **ID:** `STORY-4.3` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 3 (Completed) | **Quality Gate:** G2, G3, G6
* **User Story:** *As an Oasis player, I want multiplayer interactions to route exclusively through the Sovereign Stack over the `L1b <-> L2` UHAI IPC boundary without any embedded P2P code in the game client, maintaining strict architectural separation across Layers 2 through 5.*
* **Technical Tasks:**
  1. Enforce Anti-Bleed Invariant 3 & Strict Layer Adjacency: verify zero libp2p, zero WebRTC sockets, and zero LoRa decoders in the Oasis engine binary, and ensure Oasis communicates exclusively with Layer 2 (`col-meshd` SITL ingress/egress) over the `/dev/shm` UHAI ring buffer.
  2. Codify the exact Sovereign Stack daemon layer responsibilities for multiplayer state and economic settlement:
     * **Layer 2 (`col-meshd`):** Reticulum Network Stack (RNS), LoRa, 802.11s mesh transport, speed-of-light physical distance bounding, and the `L1b <-> L2` UHAI SITL bridge.
     * **Layer 3 (`col-storaged`):** Automerge/Yrs CRDT state replication, Kademlia DHT, IPLD blockstore, Proof-of-Thermodynamic-Work (PoTW) verification, and **Zero-Sum Mutual Credit Clearing (BCC)**.
     * **Layer 4 (`col-execd`):** **Keyless, blind 10 Hz deterministic BPMN 2.0 state machine & saga execution engine** (executes multi-step workflow sagas without holding cryptographic keys).
     * **Layer 5 (`col-kmsd`):** **W3C DIDs (`did:peer`), Ed25519/FROST threshold keys, BBS+ Verifiable Credentials, and progressive Trust Rings ($R_6 \to R_1$)**.
  3. Implement game client multi-agent proxy entities and cadastral deltas synchronized downward from Layer 3 (`col-storaged`) via Layer 2 (`col-meshd`) over UHAI (`L3 -> L2 -> L1b`).
* **Acceptance Criteria:**
  * **Given** two Oasis clients running on separate local machines,
  * **When** peer mutual-credit trade and state replication occur,
  * **Then** Oasis exchanges frames solely with `col-meshd` (L2) over UHAI, `col-storaged` (L3) clears PoTW and zero-sum BCC balances, `col-execd` (L4) executes keyless BPMN sagas, and `col-kmsd` (L5) signs/verifies DIDs and Trust Rings,
  * **And** binary inspection of the Oasis client confirms zero embedded P2P, cryptographic key, or BPMN XML libraries, satisfying Anti-Bleed Invariants 1–5.

---

#### Story 4.4: Cryptographic Speed-of-Light Distance Bounding
* **ID:** `STORY-4.4` | **Points:** 5 SP | **Priority:** P1 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G2, G3
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
* **ID:** `STORY-4.5` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G2, G4
* **User Story:** *As neighboring allied settlements, I want merging our parcel borders to dissolve internal property lines and reduce exposed perimeter by 25%, establishing an open Commons Corridor that reduces external regulatory friction.*
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
  * **And** boundary friction $\kappa$ drops toward $0.05$, reducing external compliance audit frequency along that boundary by $95\%$.

---

### Epic 5: Scenario Driven Development (SDD) Dynamic Ingestion & In Silico Compiler (SDD Dynamic Ingestion & Compiler)
**Epic ID:** `EPIC-5` | **Total Estimate:** 40 Story Points | **Priority:** P0 Critical | **Baseline:** Completed (Sprints 2, 4)  
**Focus:** Implement the dynamic scenario ingestion pipeline for all 24 canonical MVS scenarios (`docs/scenarios/*.md`), JSON-LD schema parsing, dynamic Flecs ECS data binding, spatial voxel palette allocation, Gibsonian affordance generation, upward compilation loop, and automated G1–G6 verification harness.

---

#### Story 5.1: Dynamic Scenario Ingestion Pipeline & Schema Parser
* **ID:** `STORY-5.1` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G2, G4
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
* **ID:** `STORY-5.2` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G4
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
* **ID:** `STORY-5.3` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 2 (Completed) | **Quality Gate:** G1
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
* **ID:** `STORY-5.4` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G4
* **User Story:** *As Founder 01, I want scenario task bounties translated into environmental affordances and posted to the Cadastral Slate, so that community members can claim work autonomously.*
* **Technical Tasks:**
  1. Parse scenario JSON-LD `TaskSpecification` blocks and generate Gibsonian affordance trees in spatial grid buckets.
  2. Populate prerequisites (`min_skill`, `required_tools`), somatic costs ($\dot{C}, \dot{F}$), and psychological valence.
  3. Translate scenario bounties into 264-byte binary `SpatialIntentPayload` packets posted to the Cadastral Slate bulletin board.
  4. Allow autonomous NPCs to evaluate and claim open tasks during the 06:00 Morning Circle.
* **Acceptance Criteria:**
  * **Given** Scenario Delta (`delta_childcare.md`) posts a 4-hour childcare supervision bounty,
  * **When** the 06:00 Morning Circle convenes,
  * **Then** the task appears on the Cadastral Slate offering 1.0 VC/hr credit parity,
  * **And** an NPC with high `empathy` claims the affordance autonomously, freeing parents for construction.

---

#### Story 5.5: Upward Compilation Loop (Simulation to Bare-Metal Deployment)
* **ID:** `STORY-5.5` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G1–G6
* **User Story:** *As a community steward, I want policies and fixes stress-tested in Oasis simulation at 100x speed to compile upwards into production edge daemon configurations, so that in silico discoveries improve physical node operations.*
* **Technical Tasks:**
  1. Implement execution trace recorder capturing failure modes (e.g., childcare chore rotation collapse, inverter black-start overload).
  2. Implement policy refinement compiler: translate verified in-game workflow adjustments (e.g., rotation rebalance tokens, pre-charge delays) into canonical JSON-LD and BPMN 2.0 XML schemas via external compiler tooling (`tools/sdd_compiler/`).
  3. Export refined schemas byte-compatible with Sovereign Stack Layer 4 (`col-execd`) and Layer 6 (`col-commonsd`).
  4. Generate deployable daemon package configurations for physical edge nodes.
* **Acceptance Criteria:**
  * **Given** an inverter black-start sequence in Scenario Upsilon fails due to battery inrush current,
  * **When** the player tests and validates a 5-minute pre-charge delay policy in 100x simulation,
  * **Then** the SDD compiler exports the validated policy into `upsilon_shock.bpmn`,
  * **And** the compiled file deploys directly to `col-execd` on bare-metal hardware without schema errors.

---

#### Story 5.6: Automated Headless G1–G6 Quality Gate Verification Harness
* **ID:** `STORY-5.6` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G1–G6
* **User Story:** *As the QA engineer, I want an automated headless SITL test harness that executes all 24 canonical scenarios against Quality Gates G1 through G6, so that zero regressions or physical law violations reach production.*
* **Technical Tasks:**
  1. Implement headless test runner compiling with Catch2 3.5.3: `test_scenario_runner --all-scenarios`.
  2. Implement automated gate assertion checkers:
     * G1: Thermodynamic Conservation ($\pm 10\%$ physical bounds, SOR error $< 10^{-4}$).
     * G2: Offline Autonomy (100% WAN disconnected execution, zero external pings).
     * G3: Byzantine & Sybil Immunity (spoofed sensors rejected, Sybil rings $> 3$ hops zero credit).
     * G4: State Machine Invariants (zero hanging escrows, 30-tick watchdog, SAGAs).
     * G5: Legacy Carapace Fiat Ingestion (Stripe/fiat payloads compile cleanly to internal intents without KYC identity leakage).
     * G6: Terminal Decoupling & Outward-Only Airgap (complete severance verification in Scenario Omega; zero live telemetry ingestion; strict `L1b <-> L2` adjacency).
  3. Integrate test harness into CI/CD pipeline, failing builds if any scenario fails a gate.
* **Acceptance Criteria:**
  * **Given** all 24 canonical scenarios in `docs/scenarios/*.md`,
  * **When** `test_scenario_runner` executes in headless mode,
  * **Then** all 24 scenarios pass Gates G1 through G6 in $< 60\text{ seconds}$ total execution time,
  * **And** zero memory leaks, zero hanging escrows, and zero NaN values are detected.

---

### Epic 6: High-Performance Engine Core & WebGPU Amanatides & Woo Raymarcher (High-Performance Engine Core)
**Epic ID:** `EPIC-6` | **Total Estimate:** 34 Story Points | **Priority:** P0 Critical | **Baseline:** Completed (Sprints 1, 4)  
**Focus:** Implement C++20 Flecs ECS foundation, Amanatides & Woo DDA raymarcher in WebGPU WGSL, 7.40 ms frame budget (10.60 ms hard cap, 60 FPS), rigid 256.00 MB WASM linear memory partition table (zero runtime allocations), and cross-platform build pipeline (CMake, Ninja, LLVM 18, emsdk 3.1.56, vcpkg).

---

#### Story 6.1: Two-Level Hierarchical DDA Raymarcher in WGSL
* **ID:** `STORY-6.1` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G1
* **User Story:** *As an Oasis player, I want the volumetric voxel world rendered via an Amanatides & Woo 3D DDA shader in WebGPU WGSL, so that the engine sustains a locked 60 FPS in standard web browsers.*
* **Technical Tasks:**
  1. Implement Amanatides & Woo (1987) grid traversal algorithm in WebGPU WGSL shaders (`raymarch.wgsl` and inline `raymarch_wgsl.hpp`).
  2. Implement Two-Level Hierarchical DDA / coarse macro-chunk occupancy acceleration ($1\text{ bit per } 16 \times 16 \times 16$ block, 8 KB VRAM per sector).
  3. Accelerate ray traversal: primary rays skip empty air voids, reducing average ray steps from $\sim 45$ to $< 12$, and bounding worst-case traversal (`MAX_STEPS = 192` across expanded $[-64, 64]$ parcel bounds).
  4. Bound total lookups to execute full raymarching in $2.60\text{ ms} - 3.80\text{ ms}$.
* **Acceptance Criteria:**
  * **Given** an active sector rendered at $1920 \times 1080$ resolution,
  * **When** the DDA raymarcher executes in WebGPU,
  * **Then** traversal time is strictly $\le 3.80\text{ ms}$ on standard modern GPUs,
  * **And** primary ray steps average $< 12$ steps per pixel across open outdoor environments.

---

#### Story 6.2: 7.40 ms Active Frame Budget & Deferred PBR Pipeline
* **ID:** `STORY-6.2` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G1
* **User Story:** *As the graphics pipeline, I want the entire rendering pass structured within a 7.40 ms active frame budget (10.60 ms hard cap), so that 60 FPS is guaranteed with $> 36\%$ safety headroom.*
* **Technical Tasks:**
  1. Profile and enforce active frame phase allocations:
     * Phase 1: CPU Uniform Prep & Culling: $1.20\text{ ms}$
     * Phase 2: DDA Raymarch Traversal: $3.80\text{ ms}$
     * Phase 3: Two-Phase PBR & Emissive Lighting: $1.80\text{ ms}$
     * Phase 4: Cadastral CAD, Info-View Overlays & Fog of Legacy Pass: $0.60\text{ ms}$
     * **Total Active Frame Time:** $7.40\text{ ms}$ ($10.60\text{ ms}$ hard budget cap).
  2. Evaluate ambient occlusion, voxel emissive radiance (molten metal, hearths), and directional sunlight.
  3. Implement frame timer watchdog: if active frame exceeds $10.60\text{ ms}$, dynamically reduce ray step caps for distant peripheral chunks.
* **Acceptance Criteria:**
  * **Given** an intense simulation scene with active agents and multiple light sources,
  * **When** frame times are measured over 1,000 consecutive frames,
  * **Then** 99th percentile active frame time remains $\le 7.40\text{ ms}$,
  * **And** zero frames breach the $16.66\text{ ms}$ vsync threshold, sustaining a rock-solid 60 FPS.

---

#### Story 6.3: 256.00 MB WASM Linear Memory Partitioning (Zero Runtime Allocations)
* **ID:** `STORY-6.3` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G4
* **User Story:** *As the engine runtime, I want linear WebAssembly memory strictly partitioned into exactly 268,435,456 bytes (256.00 MB) with zero runtime heap allocations, so that the simulation runs indefinitely without memory fragmentation or browser OOM crashes.*
* **Technical Tasks:**
  1. Codify the static 256.00 MB WASM memory reconciliation table:
     * Partition 1: Active Sector Voxel Geometry (128 chunks @ 128 KB): $16.38\text{ MB}$
     * Partition 2: Voxel Macro-Chunk Grid (Coarse DDA): $32.00\text{ MB}$
     * Partition 3: Material Properties & Textures: $16.00\text{ MB}$
     * Partition 4: Flecs ECS World & Storage Tables: $32.00\text{ MB}$
     * Partition 5: Agent Psychology & Memory Rings ($816\text{B}$ unified `PsychologyComponent`): $8.00\text{ MB}$
     * Partition 6: UHAI Lock-Free Ring Buffers: $4.00\text{ MB}$
     * Partition 7: Audio Waveform & Somatic Cache: $32.00\text{ MB}$
     * Partition 8: Cadastral CAD & Mesh Buffers: $16.00\text{ MB}$
     * Partition 9: Emscripten Heap & Thread Stacks: $32.00\text{ MB}$
     * Partition 10: Reserved Safety Cushion (Staging Buffers): $67.62\text{ MB}$
     * **Total Static Sum:** Exactly $256.00\text{ MB}$ ($268,435,456\text{ bytes}$).
  2. Implement custom linear arena allocators for each partition.
  3. Install memory hooks asserting zero calls to `malloc()`, `calloc()`, or `operator new()` inside the simulation tick loop.
* **Acceptance Criteria:**
  * **Given** the Oasis WASM client running in Chrome/Firefox for 24 continuous hours,
  * **When** memory usage is monitored via browser devtools,
  * **Then** total linear memory remains pinned at exactly $256.00\text{ MB}$ ($0\text{ bytes}$ heap drift),
  * **And** running the simulation tick loop asserts zero runtime heap allocation violations.

---

#### Story 6.4: Cross-Platform Build Pipeline & Toolchain Manifest
* **ID:** `STORY-6.4` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G2
* **User Story:** *As a developer, I want a unified CMake 3.28+ build pipeline supporting native macOS (Homebrew), native Linux (APT), and WebAssembly (emsdk 3.1.56), so that identical code compiles cleanly across all platforms.*
* **Technical Tasks:**
  1. Author `CMakeLists.txt` enforcing `-std=c++20`, strict compiler warnings (`-Wall -Wextra -Wpadded -Werror -Wconversion -Wshadow`), and SIMD optimizations (`-march=armv8-a` / `-mavx2`).
  2. Configure `vcpkg.json` manifest mode for cross-platform dependencies: Flecs 3.2.11, GLM 1.0.1, spdlog 1.14.1, nlohmann-json 3.11.3, Catch2 3.5.3, SDL2 2.30.2.
  3. Configure Emscripten toolchain flags: `-sUSE_PTHREADS=1`, `-sUSE_WEBGPU=1`, `-sINITIAL_MEMORY=268435456`, `-sMAXIMUM_MEMORY=268435456`, `-sALLOW_MEMORY_GROWTH=0`, `-sEXPORTED_RUNTIME_METHODS=ccall,cwrap`.
  4. Validate builds on macOS Apple Silicon, Ubuntu 22.04 LTS, and WASM.
* **Acceptance Criteria:**
  * **Given** a clean clone of the repository,
  * **When** running `cmake -B build -G Ninja && cmake --build build` on macOS or Linux,
  * **Then** compilation completes with zero warnings and zero errors under `-Wall -Wextra -Wpadded -Werror`,
  * **And** compiling via `emcmake cmake -B build-wasm` produces valid `.wasm` and `.js` artifacts.

---

#### Story 6.5: Cadastral Axonometric Slate & Dual Viewport Camera System
* **ID:** `STORY-6.5` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G4
* **User Story:** *As a player, I want to seamlessly toggle between a tactile 3D perspective camera and an axonometric Cadastral Slate blueprint view, so that I can easily plan construction and monitor exergy networks.*
* **Technical Tasks:**
  1. Implement Dual Viewport System:
     * Camera Mode 1: 3D Perspective / POI Spherical Arcball Camera (tactile kinematics, tool swings, dynamic weather).
     * Camera Mode 2: Cadastral Axonometric Slate (orthographic isometric projection, blueprint vectors, exergy/water overlays).
  2. Implement smooth 400 ms cubic Hermite transition curve maintaining focal point alignment.
  3. Render Cadastral CAD line vectors and blueprint stamps using WebGPU primitive pipelines.
* **Acceptance Criteria:**
  * **Given** the player is in 3D perspective mode,
  * **When** the player presses `Tab` or clicks the Slate icon,
  * **Then** the camera transitions smoothly over 400 ms to the axonometric slate view without visual pop-in,
  * **And** blueprint construction overlays display exergy conduits and water pipes accurately.

---

### Epic 7: Universal Human-Agent Interface (UHAI) Shared-Memory IPC & SITL Telemetry (UHAI IPC & Telemetry)
**Epic ID:** `EPIC-7` | **Total Estimate:** 30 Story Points | **Priority:** P0 Critical | **Baseline:** Completed (Sprints 1, 4)  
**Focus:** Implement the lock-free Single-Producer Single-Consumer (SPSC) shared memory ring buffer at `/dev/shm/uhai_telemetry.ring` (< 50 µs latency, 3 cache line header isolation), Emscripten browser adapter (`SharedArrayBuffer`), strict enforcement of 5 Anti-Bleed Invariants and `L1b <-> L2` Layer Adjacency, and outward-only physical airgap isolation between `QUALITY_SIMULATED` and `QUALITY_HARDWARE_ROOT`.

---

#### Story 7.1: Lock-Free SPSC Shared Memory Ring Buffer in `/dev/shm`
* **ID:** `STORY-7.1` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G6
* **User Story:** *As the UHAI IPC subsystem, I want a lock-free SPSC shared memory ring buffer in `/dev/shm` delivering $< 50\ \mu\text{s}$ latency with 3-cache-line header isolation, so that the Oasis engine exchanges telemetry with Sovereign Stack Layer 2 (`col-meshd` SITL bridge) without lock contention.*
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
  * **Given** Oasis producing telemetry and the SITL bridge consuming at 1,000 Hz,
  * **When** IPC latency is measured over 100,000 samples,
  * **Then** 99.9th percentile latency is strictly $< 50\ \mu\text{s}$,
  * **And** cache-line separation eliminates false sharing, verifying zero CPU core cache stalls.

---

#### Story 7.2: Emscripten Browser Transport Adapter (`SharedArrayBuffer`)
* **ID:** `STORY-7.2` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G2, G6
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

#### Story 7.3: Enforcement of the 5 Anti-Bleed Invariants, `L1b <-> L2` Adjacency & Outward-Only Telemetry Manifest
* **ID:** `STORY-7.3` | **Points:** 5 SP | **Priority:** P0 | **Sprint:** Sprint 1 (Completed) | **Quality Gate:** G6
* **User Story:** *As the system architect, I want continuous static analysis and runtime assertion checks enforcing the Five Anti-Bleed Invariants, Strict Layer Adjacency (`L1b <-> L2`), and the Outward-Only Telemetry Producer Law, so that game code never couples with Sovereign Stack daemons or ingests live physical sensor telemetry.*
* **Technical Tasks:**
  1. Author static architecture linter rule asserting zero symbols from `col-*` daemons in the Oasis client binary (Anti-Bleed Inv 1) and verifying that Oasis IPC is restricted exclusively to the `/dev/shm` UHAI ring (`L1b <-> L2 col-meshd SITL bridge`).
  2. Assert zero Ed25519, FROST, or BBS+ private key representations in Oasis structs; verify Oasis holds only opaque 32-byte `founder_did_handle` (Anti-Bleed Inv 2).
  3. Assert zero libp2p, WebRTC, or LoRa symbols in Oasis (Anti-Bleed Inv 3).
  4. Assert zero XML parsing libraries (`pugixml`, `tinyxml`) in Oasis; verify spatial intents emit as canonical 264-byte binary `SpatialIntentPayload` frames (Anti-Bleed Inv 4).
  5. Assert zero fiat currency symbols (`$`, `USD`, `fiat_ledger`) in the simulation core; verify simulation handles pure physical exergy units (Anti-Bleed Inv 5).
  6. Assert that Oasis telemetry ring interfaces operate strictly as **outward producers** of `QUALITY_SIMULATED (0x01)` samples and contain zero code paths that ingest `QUALITY_HARDWARE_ROOT (0x02)` live physical sensor telemetry to recalibrate game state.
* **Acceptance Criteria:**
  * **Given** the compiled Oasis binary and source code,
  * **When** the architectural audit script `scripts/verify_anti_bleed.sh` runs,
  * **Then** all 5 Anti-Bleed Invariants, `L1b <-> L2` Layer Adjacency, and the Outward-Only Telemetry Producer check pass with zero violations,
  * **And** any pull request introducing bleeding dependencies or live sensor ingestion fails CI automatically.

---

#### Story 7.4: Hardware-in-the-Loop Isolation & Outward-Only Airgap Invariant
* **ID:** `STORY-7.4` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G6
* **User Story:** *As the edge security engineer, I want synthetic telemetry strictly tagged `QUALITY_SIMULATED` (0x01), produced outward-only by Oasis, and barred from physical hardware actuation, so that simulation bugs or testing runs can never actuate physical node hardware or blur simulation demarcation.*
* **Technical Tasks:**
  1. Define telemetry quality flags in `TelemetrySample`:
     * `QUALITY_SIMULATED = 0x01` (emitted outward by Oasis or SITL test harnesses).
     * `QUALITY_HARDWARE_ROOT = 0x02` (emitted by physical hardware secure elements).
  2. Implement hardware airgap filter in `col-telemetryd` (Layer 1): all physical relays, breakers, and solenoids strictly require `QUALITY_HARDWARE_ROOT` signed by Layer 5 keys and authorized via Layer 4 execution sagas.
  3. Verify that any packet tagged `QUALITY_SIMULATED` attempting to trigger a physical GPIO pin or Modbus register is immediately dropped and logged as a security violation, and verify that Oasis rejects any inward `QUALITY_HARDWARE_ROOT` calibration stream.
* **Acceptance Criteria:**
  * **Given** an Oasis SITL test harness injecting synthetic inverter trip commands tagged `QUALITY_SIMULATED`,
  * **When** the packet reaches `col-telemetryd`,
  * **Then** the hardware daemon rejects the actuation command,
  * **And** physical circuit breakers remain untouched, mathematically verifying the outward-only airgap invariant.

---

#### Story 7.5: Software-In-The-Loop (SITL) Headless Digital Twin Harness
* **ID:** `STORY-7.5` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Sprint 4 (Completed) | **Quality Gate:** G2, G6
* **User Story:** *As the QA engineer, I want to execute Oasis as a headless Software-In-The-Loop (SITL) digital twin paired with live Sovereign Stack daemons in local test environments, so that we can stress-test edge protocols prior to bare-metal deployment.*
* **Technical Tasks:**
  1. Implement headless execution mode: `./oasis_engine --headless --sitl --ticks 100000`.
  2. Connect headless engine over UHAI (`L1b <-> L2`) to external Sovereign Stack test daemons running in local Linux namespaces (`col-telemetryd` through `col-adversaryd`).
  3. Execute 100x accelerated clock stress-testing of microgrid black-starts (Scenario Upsilon) and trade clearances (Scenario Pi).
  4. Log state convergence, exergy conservation, and CRDT synchronization traces.
* **Acceptance Criteria:**
  * **Given** the Sovereign Stack daemons running in local test containers,
  * **When** Oasis runs in headless SITL mode for 100,000 ticks at 100x speed,
  * **Then** state synchronizes smoothly across UHAI IPC without deadlock,
  * **And** SITL run verifies system stability across all six Quality Gates prior to hardware flashing.

---

### Epic 8: Tactile 3D Navigation, POI Arcball Camera & Spatial Context Designator
**Epic ID:** `EPIC-8` | **Total Estimate:** 23 Story Points | **Priority:** P0 Critical | **Target Sprint:** Dual Sprint 11  
**Focus:** Replace abstract hotkeys and flat 2D-only camera panning with a dynamic Point-of-Interest (POI) Spherical Arcball Camera (`Shift + Right Click` raycast origin with 3D wireframe reticle, spherical $(\theta, \phi)$ orbit, `Shift + Mouse Wheel` radial zoom $r$, automatic/manual roof cutaway `z_cutaway_level`, and expanded $[-64, 64]$ DDA bounds) and a Dwarf Fortress / RimWorld-inspired tactile Spatial Designator (Right-Click voxel evaluator & contextual action menu with prerequisite badges + Left-Click 3D designation volume painter with stockpile hauling and canonical 264-byte `SpatialIntentPayload` dispatch over `L1b -> L2` UHAI).

---

#### Story 8.1: Dynamic Point-of-Interest (POI) Spherical Arcball Camera & Roof Cutaway
* **ID:** `STORY-8.1` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Dual Sprint 11 | **Quality Gate:** G1, G4
* **User Story:** *As an Oasis player navigating the 3D voxel enclave, I want `Shift + Right Click` to raycast the exact voxel clicked as a dynamic Point-of-Interest (POI) pivot highlighted by a 3D wireframe reticle, mouse drag to smoothly orbit azimuth ($\theta$) and elevation ($\phi$) around that voxel, `Shift + Mouse Wheel` to adjust radial distance ($r$), and automatic/manual roof cutaway (`z_cutaway_level`) so I can inspect and click inside enclosed rooms or survey the full $128 \times 128$ parcel.*
* **Technical Tasks:**
  1. Expand DDA raymarching and CPU raycasting spatial bounds in lockstep across **all three files**—`core/1_simulation/shaders/raymarch.wgsl`, `core/1_simulation/src/raymarch_wgsl.hpp`, and `core/1_simulation/src/raycaster.hpp`—from `|x| <= 28, z in [-28, 28]` (`MAX_STEPS = 128`) to **`|x| <= 64, y in [0, 32], z in [-64, 64]` (`MAX_STEPS = 192`)** so the entire $128 \times 128$ parcel is raycastable and renderable.
  2. Define `ArcballCameraComponent` (`alignas(32)`, 32 bytes, `-Wpadded` clean) in Flecs ECS storing `poi_origin` (`glm::vec3`, 12B), `azimuth_theta` (`float`, 4B, unbounded $[0, 2\pi)$), `elevation_phi` (`float`, 4B, strictly clamped to $[5.0^\circ, 85.0^\circ]$ / $[0.0873\text{ rad}, 1.4835\text{ rad}]$ to prevent gimbal flip and subterranean clipping), `radius_r` (`float`, 4B, strictly clamped to $[2.0\text{ m}, 150.0\text{ m}]$), `target_radius_r` (`float`, 4B), and `active_flags` (`uint32_t`, 4B).
  3. Implement `Shift + Right Click` initial press handler with **Ray-Miss No-Op Guard**: invoke `ScreenToWorldRaycast` (`raycaster.hpp`) from screen-space cursor coordinates $(u, v)$ to intersect the first non-air voxel $(x_p, y_p, z_p)$ below `z_cutaway_level`. **When `ScreenToWorldRaycast` returns `hit.hit == false` (clicking sky or out-of-bounds), `Shift + Right Click` is a strict no-op (preserving the existing `poi_origin` and never snapping the POI pivot to `{0, 0, 0}`).** On a valid hit (`hit.hit == true`), lock `poi_origin = vec3(x_p + 0.5f, y_p + 0.5f, z_p + 0.5f)` and render a **3D Wireframe POI Reticle** (`poi_reticle_pos` in `CameraUniforms`) around the pivot voxel during orbit/zoom.
  4. Implement spherical orbit kinematics on `Shift + Right Click` drag with **Shortest-Arc Angular Wrap**: map cursor delta $(\Delta x, \Delta y)$ to angular velocities $\Delta \theta = -k_{\text{orbit}} \cdot \Delta x$ and $\Delta \phi = k_{\text{orbit}} \cdot \Delta y$, using **shortest-arc angular wrap (`std::remainder(target_theta - current_theta, 2.0f * PI)`)** during azimuth $\theta$ interpolation so crossing the $0 / 2\pi$ seam never causes a $360^\circ$ camera spin, and compute eye position in world space:
     $$\mathbf{P}_{\text{eye}} = \mathbf{P}_{\text{POI}} + r \begin{bmatrix} \cos\phi \sin\theta \\ \sin\phi \\ \cos\phi \cos\theta \end{bmatrix}$$
     and implement `Shift + Mouse Wheel` logarithmic radial zoom $r_{\text{target}} = \text{clamp}(r \cdot \exp(-k_{\text{zoom}} \cdot \Delta w), 2.0, 150.0)$ with critically damped smoothing ($r(t + \Delta t) = \text{lerp}(r(t), r_{\text{target}}, 1 - \exp(-18.0 \cdot \Delta t))$) while preserving `Shift + Left Click` 2D planar panning (N, E, S, W).
  5. Implement **Automatic & Manual Roof/Wall Cutaway (`z_cutaway_level` uniform in `CameraUniforms`):**
     * Manual toggle via `PageUp` / `PageDown` (or `[` / `]`) steps `z_cutaway_level` vertically through $y \in [1, 32]$.
     * Automatic trigger: when `radius_r < 12.0f` and `poi_origin` lies inside an `ENCLOSED_INSULATED` room (or under `MAT_ROOF_SHINGLE`), automatically lower `z_cutaway_level` below the roof ceiling height so both the WGSL raymarcher (`raymarch.wgsl` / `raymarch_wgsl.hpp`) and CPU mouse raycaster (`raycaster.hpp`) cull roof voxels and allow direct viewing and clicking of interior floor voxels.
* **Acceptance Criteria:**
  * **Given** the player holds `Shift` and right-clicks on a voxel at $(52, 4, -48)$ inside an enclosed cabin (and subsequently `Shift + Right Clicks` on open sky where `hit.hit == false`),
  * **When** the player drags the mouse across the $0 / 2\pi$ azimuth boundary and scrolls `Shift + Mouse Wheel` to zoom in to $r = 8.0\text{ m}$ (or presses `PageDown`),
  * **Then** the sky click is a strict no-op (never snapping POI to `{0,0,0}`), the valid hit locks the 3D Wireframe POI Reticle to $(52.5, 4.5, -47.5)$, azimuth $\theta$ interpolates via `std::remainder(target_theta - current_theta, 2*PI)` without $360^\circ$ spin, pitch $\phi \in [5.0^\circ, 85.0^\circ]$ and $r \in [2.0\text{ m}, 150.0\text{ m}]$ remain strictly clamped, and `z_cutaway_level` slices away `MAT_ROOF_SHINGLE` so interior floors are clickable,
  * **And** verified by Catch2 suite `test_arcball_and_designator.cpp` and Playwright `scripts/verify_canvas.js` / `tests/verify_canvas.js`.

---

#### Story 8.2: Right-Click Spatial Voxel Evaluator, Full Shelter Context Menu & DOM/SDL2 Input Isolation
* **ID:** `STORY-8.2` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Dual Sprint 11 | **Quality Gate:** G4
* **User Story:** *As a player, I want an unmodified Right-Click on any 3D voxel to evaluate its material, structural state, and occupying entity and pop up a contextual action menu `#voxel-context-menu` with upfront `kcal`, `Focus`, and material/tool prerequisite badges—covering earthworks, hardware, hauling, and full wall/door/roof/hearth shelter construction—without browser context menus or click-through bugs.*
* **Technical Tasks:**
  1. Enforce **DOM & SDL2 Input Isolation**:
     * In `core/1_simulation/shell_minimal.html`, attach `event.preventDefault()` on `contextmenu` across `.oasis-viewport-container` and `#canvas` so browser right-click menus never appear.
     * In `core/1_simulation/src/main.cpp`, remove the legacy `SDL_BUTTON_RIGHT` `mousedown` blueprint-cycling handler so right-click is dedicated cleanly to `Shift + Right Click` Arcball orbit and unmodified `Right-Click` release (`< 5 px` drag threshold) Context Menu evaluation.
     * Enforce `pointer-events: auto` and `event.stopPropagation()` on `#voxel-context-menu`, `#time-warp-bar`, and `#epoch-reconcile-modal` so clicking HUD buttons never leaks a `Left-Click` raycast onto the 3D canvas behind them.
  2. Implement `SpatialVoxelEvaluator` service: on unmodified `Right-Click` release, execute a 3D DDA raycast (`raycaster.hpp`, respecting `z_cutaway_level`) to retrieve `VoxelHitResult { glm::ivec3 coord, uint16_t material_id, uint8_t flags, flecs::entity_t occupant_id }`.
  3. Define complete context resolution rules mapping `VoxelHitResult` to valid `ContextActionDescriptor` options so players can construct enclosed $>18^\circ\text{C}$ rooms from scratch:
     * **Dirt / Topsoil / Turf Voxel (`MAT_DIRT`, `MAT_TURF`):** `[Dig Swale]`, `[Plant Cover Crop]`, `[Designate Haul Zone]`, `[Build Wall: Cob / Urbanite]`
     * **Foundation / Wall / Perimeter Voxel (`MAT_FOUNDATION`, `MAT_URBANITE`, `MAT_COB`, `MAT_TIMBER_WALL`):** `[Build Wall: Cob / Urbanite]`, `[Install Door: Insulated Timber]`, `[Build Roof: Timber & Shingle]`, `[Build Hearth: Rocket Mass Heater]`, `[Install Hardware: Inverter]`, `[Erect Solar Blueprint]`
     * **Loose Item / Resource Voxel (`MAT_ITEM_CRATE`, `MAT_CORDWOOD`, `MAT_URBANITE_CHUNK`, `MAT_CLAY_COB_LUMP`):** `[Haul to Storage]`
     * **Interior Floor / Clearing Voxel (`MAT_TIMBER_FLOOR`, `MAT_CLEARING`, `MAT_BELL_TENT`):** `[Designate Sleep Zone]`, `[Designate Recreation Zone]`, `[Build Hearth: Rocket Mass Heater]`
     * **Active Designation / Blueprint Voxel:** `[Cancel Designation]`
  4. Render `#voxel-context-menu` anchored at screen-space coordinates $(u, v)$, displaying voxel telemetry (material name, temperature $^\circ\text{C}$, moisture $\%$, enclosure state) and **upfront cost & prerequisite badges** on every menu row (e.g., `[Build Wall: Cob] — 120 kcal | 8 Focus | Req: 2x Clay/Cob Lump`).
* **Acceptance Criteria:**
  * **Given** the player right-clicks on a Dirt or Foundation voxel in the 3D viewport,
  * **When** `SpatialVoxelEvaluator` resolves the hit,
  * **Then** the native browser context menu is suppressed, `#voxel-context-menu` renders with full shelter/earthwork/hardware options and upfront `kcal`, `Focus`, and material prerequisite badges,
  * **And** clicking an option inside `#voxel-context-menu` activates `DESIGNATOR_PAINT_MODE` without leaking a click event into the underlying SDL2 canvas (`test_arcball_and_designator.cpp` & `scripts/verify_canvas.js`).

---

#### Story 8.3: Left-Click Designation Painter, 256-Voxel Volume Cap, Stockpile Hauling & Canonical 264-Byte `SpatialIntentPayload` Dispatch
* **ID:** `STORY-8.3` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Dual Sprint 11 | **Quality Gate:** G1, G4, G6
* **User Story:** *As a player in `DESIGNATOR_PAINT_MODE`, I want Left-Click drag/click to paint 3D designation volumes (clamped to $\le 256$ voxels per drag) with Valid (Cyan/Amber) vs. Invalid (Crimson) WGSL previews, a floating cursor tally badge, physical resource drops & stockpile hauling, and canonical 264-byte `SpatialIntentPayload` dispatch across `L1b -> L2` UHAI, so that agents gather materials and execute construction without overflowing GPU or IPC buffers.*
* **Technical Tasks:**
  1. Implement `DesignationPainterSystem` with **256-Voxel Bounding-Box Volume Cap (`MAX_DESIGNATION_VOXELS = 256`)**: when in `DESIGNATOR_PAINT_MODE`, `Left-Click` press records $\mathbf{V}_{\text{start}} = (x_0, y_0, z_0)$ and drag updates $\mathbf{V}_{\text{end}} = (x_1, y_1, z_1)$, forming an axis-aligned 3D bounding box $\mathcal{B} = [\min(\mathbf{V}_{\text{start}}, \mathbf{V}_{\text{end}}), \max(\mathbf{V}_{\text{start}}, \mathbf{V}_{\text{end}})]$. **Strictly clamp total designated volume $|\mathcal{B}| \le 256\text{ voxels}$ per drag operation** (rendering oversized drag bounds in Crimson `[Volume Cap: Max 256 Voxels]`) so a single drag can never overflow the 1,024-slot `GpuDynamicVoxelBuffer` or `/uhai_intent` SPSC ring buffer.
  2. Render **Tactile Drag Previews & Cursor Tally Badge**:
     * In `DynamicVoxelStore` / WGSL, highlight valid target voxels inside $\mathcal{B}$ ($\le 256$ voxels) with pulsing **Cyan/Amber** holographic wireframes and invalid/obstructed/missing-material/over-cap voxels in **Crimson**.
     * Render a floating HTML/CSS cursor tally badge during drag displaying volume dimensions, voxel count ($N / 256$), total `kcal` & `Focus` cost, and required material count (e.g., `6x1x1 Swale (6/256) | 480 kcal | +6 Clay/Cob Lumps`).
     * Render a 3D floating radial/bar work-progress indicator above any voxel currently being excavated or constructed by an agent.
  3. Implement the **Physical Resource Drop & Stockpile Hauling Loop**:
     * Completing `[Dig Swale]` on Dirt spawns physical loose `MAT_CLAY_COB_LUMP` item entities; completing asphalt/concrete quarrying spawns `MAT_URBANITE_CHUNK` entities.
     * Painting `[Designate Haul Zone]` attaches a `StockpileZoneComponent` to the designated floor volume, which automatically generates background `[Haul to Storage]` jobs for loose items on the parcel.
     * Executing `[Build Wall: Cob / Urbanite]`, `[Install Door: Insulated Timber]`, `[Build Roof: Timber & Shingle]`, or `[Build Hearth: Rocket Mass Heater]` deducts the required materials from the nearest `StockpileZoneComponent` (or Starter Supply Crate) conserving mass (Gate G1).
  4. Preserve and populate the canonical **264-byte `SpatialIntentPayload` (`alignas(8)`)** wrapped inside the **320-byte `SpatialIntentSlot`** in `core/1_simulation/include/uhai_ring_buffer.hpp` (`static_assert(sizeof(SpatialIntentPayload) == 264)` and `static_assert(sizeof(SpatialIntentSlot) == 320)`), pushing the work order into Flecs ECS (`AdvisoryIntentionQueue` / `AffordanceBroker`) and emitting the frame across the `/dev/shm` UHAI ring buffer strictly via **`L1b -> L2` (`col-meshd` SITL ingress)** for upward routing (`L2 -> L3 -> L4 col-execd`).
* **Acceptance Criteria:**
  * **Given** the player selects `[Dig Swale]` and left-click drags a $4 \times 1 \times 1$ volume on Dirt (and attempts an oversized $20 \times 20 \times 1 = 400$ voxel drag),
  * **When** `DesignationPainterSystem` evaluates the drag volumes,
  * **Then** the 400-voxel drag is clamped/rejected at `MAX_DESIGNATION_VOXELS = 256` in Crimson without overflowing the 1,024-slot `GpuDynamicVoxelBuffer` or `/uhai_intent` ring, while the $4 \times 1 \times 1$ volume renders Cyan/Amber, emits a 264-byte `SpatialIntentPayload` (`L1b -> L2`), and drops `MAT_CLAY_COB_LUMP` items hauled to `StockpileZoneComponent` (`test_arcball_and_designator.cpp`).

---

### Epic 9: Biological & Psychological Homeostasis — The Chill Loop
**Epic ID:** `EPIC-9` | **Total Estimate:** 21 Story Points | **Priority:** P0 Critical | **Target Sprint:** Dual Sprint 12  
**Focus:** Counterbalance the thermodynamic and psychological drain loops by implementing the "Chill Loop" recovery ECS systems: dirty-flag cached 3D flood-fill enclosed thermal room detection (`MAX_BFS_VISITED = 512` voxels hard node cap, $>18^\circ\text{C}$, `+45 Focus/hr` sleep recovery, passable airtight `MAT_DOOR` voxels, and `F1–F4` Info-View shader overlays), the **Day 1 Genesis Survival Camp Bootstrap**, somatic food/water consumption affordances, designated Recreation Zones draining `Stress` (`-25%/hr` solo, `-40%/hr` social), and the involuntary `BURNOUT` state at 100% `Stress` with life-critical somatic preemption and anti-soft-lock recovery safeguards.

---

#### Story 9.1: Dirty-Flag Cached Thermal Room Flood-Fill (`MAX_BFS_VISITED = 512`), Passable Doors, `F1–F4` Overlays & Insulated Sleep Recovery (`+45 Focus/hr`)
* **ID:** `STORY-9.1` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Dual Sprint 12 | **Quality Gate:** G1, G4
* **User Story:** *As Founder 01 sleeping in a designated `[Sleep Zone]`, I want the engine to verify via a dirty-flag cached 3D flood-fill (`MAX_BFS_VISITED = 512` voxels hard node cap) whether I am inside a thermally insulated, enclosed voxel room (with passable doors and roof) at $>18.0^\circ\text{C}$, halting cold caloric burn ($\dot{C}_{\text{thermal}} = 0$), regenerating `+45 Focus/hr`, and providing `F1–F4` Info-View shader overlays so the player can immediately spot and patch any thermal leaks.*
* **Technical Tasks:**
  1. Implement **Dirty-Flag Cached `ThermalEnclosureSolver` with Hard Node Cap (`MAX_BFS_VISITED = 512`)**:
     * Maintain `bool room_enclosure_dirty` on `SleepZoneComponent` / `EnclosureCacheComponent`, set to `true` **only** when a voxel is mutated in `DynamicVoxelStore` or a new zone is placed.
     * When `room_enclosure_dirty == true`, first run an $O(H)$ vertical $+Y$ sky-column ray check from each zone floor voxel to verify an overhead `MAT_ROOF_SHINGLE` or `MAT_BELL_TENT` canopy exists; if open to sky, immediately mark `UNENCLOSED` and record the missing roof coordinate as a `leak_voxel`.
     * If roofed, execute a bounded 3D breadth-first flood-fill with a strict **`constexpr size_t MAX_BFS_VISITED = 512` visited-voxels hard node cap** (never an unbounded radius-32 $274\text{k}$-voxel search; uses a fixed 512-element static ring queue with zero heap allocations). If `visited_count > MAX_BFS_VISITED`, immediately terminate BFS and classify the volume as `UNENCLOSED` (open exterior or oversized hall). Treat `MAT_TIMBER_WALL`, `MAT_COB`, `MAT_URBANITE`, `MAT_ROOF_SHINGLE`, `MAT_BELL_TENT`, and **`MAT_DOOR` (`Insulated Timber Door`)** as airtight boundary barriers for thermal enclosure while keeping `MAT_DOOR` passable to agent A* pathfinding. Clear `room_enclosure_dirty = false` and cache the result.
  2. Compute interior room equilibrium temperature $T_{\text{room}}$ factoring wall thermal resistance ($R$-value) and interior heat sources (`MAT_ROCKET_HEATER`, camp stove, metabolic heat):
     $$\frac{dT_{\text{room}}}{dt} = \frac{\dot{Q}_{\text{heaters}} + \dot{Q}_{\text{metabolic}}}{C_{\text{thermal}}} - \frac{T_{\text{room}} - T_{\text{ambient}}}{R_{\text{envelope}} \cdot C_{\text{thermal}}}$$
  3. Standardize **`SleepRecoverySystem` Rates**: when an agent sleeps in a `[Sleep Zone]` inside an `ENCLOSED_INSULATED` room with $T_{\text{room}} > 18.0^\circ\text{C}$, set cold-exposure excess caloric burn to zero ($\dot{C}_{\text{thermal}} = 0$, clamping metabolic rate to $1.0\times$ basal rest burn $2,000\text{ kcal/day}$), regenerate `short_term_focus` at exactly **`+45 Focus/hr`** (clamped to $255$), and decay `circadian_sleep_debt` to $0$.
  4. Implement **`F1–F4` Info-View Shader Overlays** (`overlay_mode` uniform in `CameraUniforms` rendered in `raymarch.wgsl` / `raymarch_wgsl.hpp`):
     * **`F1` Thermal & Enclosure Overlay:** Renders enclosed $>18.0^\circ\text{C}$ interior volumes in warm emerald-green tint and pulses any unsealed gap / missing roof `leak_voxel` in bright icy-blue so players can instantly diagnose why a room isn't insulated.
     * **`F2` Exergy & Water Overlay:** Highlights electrical SOR conductance lines and water swale/cistern networks.
     * **`F3` Stockpile & Haul Overlay:** Highlights `StockpileZoneComponent` utilization and loose item locations.
     * **`F4` Stress & Trust Ring Overlay:** Colors agent Plumbobs and zone footprints by `Stress` ($0-100\%$) and Trust Ring tier ($R_6 \to R_1$).
* **Acceptance Criteria:**
  * **Given** outside ambient temperature is $2.0^\circ\text{C}$ and Founder 01 sleeps in a `[Sleep Zone]` inside a 4-walled room with an `Insulated Timber Door` (`MAT_DOOR`), a roof (`MAT_ROOF_SHINGLE`), and a rocket mass heater maintaining $T_{\text{room}} = 20.5^\circ\text{C}$,
  * **When** 4 in-game hours of sleep elapse,
  * **Then** `ThermalEnclosureSolver` executes only once on construction completion (`room_enclosure_dirty == false` on subsequent ticks) with hard cap `MAX_BFS_VISITED = 512`, Founder 01 walks freely through `MAT_DOOR` via A*, cold caloric penalty is halted ($\dot{C}_{\text{thermal}} = 0$), `Focus` regenerates at `+45 Focus/hr` ($+180$ Focus total), and pressing `F1` highlights the room in emerald-green (or pulses icy-blue on a breached wall voxel), verified by `test_chill_loop_homeostasis.cpp`.

---

#### Story 9.2: Day 1 Genesis Survival Camp Bootstrap & Somatic Caloric/Hydration Intake Affordances
* **ID:** `STORY-9.2` | **Points:** 6 SP | **Priority:** P0 | **Sprint:** Dual Sprint 12 | **Quality Gate:** G1, G4, G6
* **User Story:** *As Founder 01 starting on Lot 402 at `tick = 0`, I want a pre-placed **Day 1 Genesis Survival Camp Kit** providing a 3-day survival runway, and I want to autonomously or via player context designation drink from clean water totes/cisterns and eat from ration crates or the Commons Kitchen, restoring my `caloric_reserve` and `hydration` while emitting outward-only simulated telemetry over `L1b -> L2` UHAI.*
* **Technical Tasks:**
  1. Codify the **Day 1 Genesis Survival Camp Bootstrap** spawned deterministically on Lot 402 at `tick = 0` so Founder 01 never freezes or starves before building permanent masonry shelter:
     * **3×3 Canvas Bell Tent (`MAT_BELL_TENT`):** Pre-enclosed shelter containing a pre-designated `[Sleep Zone]` cot and a compact camp woodstove stocked with `12x MAT_CORDWOOD` (maintaining $>18.0^\circ\text{C}$ for the initial 72 in-game hours).
     * **Starter Ration Crate (`MAT_FOOD_STORAGE`):** Pre-stocked with **`12 Nutrient Ration Meals` ($9,600\text{ kcal}$ total, $800\text{ kcal/meal}$)**.
     * **Potable Water Tote (`MAT_CISTERN`):** Pre-stocked with **`60.0 L` of clean drinking water**.
     * **Campfire Ring (`[Recreation Zone]`):** Pre-designated adjacent to the Bell Tent for immediate Stress relief.
     * **Starter Tool Locker:** Contains pickaxe, shovel, wire strippers, and masonry trowel.
  2. Register `[Affordance: Drink Clean Water]` on water totes/cisterns (`MAT_CISTERN`, `MAT_WATER_TAP`) and `[Affordance: Eat Nutrient Meal]` on ration crates and Commons Kitchen ranges (`MAT_FOOD_STORAGE`, `MAT_INDUCTION_RANGE`).
  3. Implement autonomous self-care utility override in `SomaticStateSystem`: when `hydration < 35%` or `caloric_reserve < 1,200 kcal`, physiological survival utility overrides standard labor tasks. Drinking $1.0\text{ L}$ deducts $1.0\text{ L}$ from the water tote/cistern and restores $+40\%$ `hydration`; eating 1 meal deducts $1$ ration ($800\text{ kcal}$) from storage, restores $+800\text{ kcal}$ (`Q24.8`) to `caloric_reserve`, and grants `THOUGHT_WARM_MEAL` ($-10,000$ Stress units / $-10\%$).
  4. Emit outward-only `QUALITY_SIMULATED (0x01)` telemetry samples of water and caloric stock deltas strictly across the **`L1b -> L2` UHAI ring** (which `col-meshd` forwards to `L3 col-storaged` for `SCN-GAMMA-KITCH` accounting).
* **Acceptance Criteria:**
  * **Given** a new simulation boots at `tick = 0` on Lot 402,
  * **When** Founder 01 works through Day 1 and `caloric_reserve` drops below $1,200\text{ kcal}$ and `hydration` drops below $35\%$,
  * **Then** the Day 1 Genesis Camp Kit is present with $12$ meals ($9,600\text{ kcal}$), $60.0\text{ L}$ water, and a $>18^\circ\text{C}$ 3×3 Canvas Bell Tent,
  * **And** Founder 01 autonomously consumes $1.0\text{ L}$ water ($+40\%$ hydration) and $1$ meal ($+800\text{ kcal}$), decrementing camp stocks with exact mass conservation (Gate G1) and emitting `QUALITY_SIMULATED (0x01)` telemetry over `L1b -> L2` UHAI (`test_chill_loop_homeostasis.cpp`).

---

#### Story 9.3: Recreation Zones, Social Leisure & 100% Stress `BURNOUT` State with Life-Critical Somatic Preemption & Anti-Soft-Lock Safeguards
* **ID:** `STORY-9.3` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Dual Sprint 12 | **Quality Gate:** G1, G4
* **User Story:** *As a player managing agent mental health, I want `[Recreation Zone]` areas (campfires, reading corners, communal tables) to actively drain `Stress` (`-25%/hr` solo, `-40%/hr` social), and if `Stress` hits 100% (`100,000` units), I want the agent to enter an involuntary `STATE_BURNOUT` that drops labor tasks until `Stress < 40%` (`40,000` units)—while allowing life-critical hydration/caloric intake to preempt burnout so the agent never dies of thirst or hunger during recovery.*
* **Technical Tasks:**
  1. Implement `RecreationZoneComponent` attached via the Right-Click Spatial Designator (`[Designate Recreation Zone]`) and pre-attached to the Day 1 Campfire Ring.
  2. Standardize **Recreation Zone Stress Drainage Rates** in $Q24.8$ / normalized units ($100\% \equiv 100,000$ units):
     * **Solo Leisure** (reading, sitting by campfire): **`-25% Stress/hr` ($-25,000\text{ units/hr}$)**.
     * **Social Leisure** (conversing with $\ge 1$ agent within $3.0\text{ m}$ in a Recreation Zone): **`-40% Stress/hr` ($-40,000\text{ units/hr}$)**.
  3. Implement the **100% Stress `STATE_BURNOUT` FSM, Life-Critical Somatic Preemption & Anti-Soft-Lock Safeguards**:
     * When `cumulative_stress >= 100,000` ($100.0\%$), transition `breakdown_state` to `STATE_BURNOUT` (and trigger the Story 10.1 Emergency Warp Brake to `1x`).
     * Purge physical labor tasks from `AdvisoryIntentionQueue`, release held tool leases, and display HUD banner: `[BURNOUT: Agent refuses physical labor until Stress recovers below 40%]`.
     * **Life-Critical Somatic Preemption Rule:** Life-critical dehydration (**`hydration < 20%`**) or acute starvation (**`caloric_reserve < 500 kcal`**) **strictly preempts `STATE_BURNOUT`**, temporarily allowing the burned-out agent to walk to the nearest water tote/cistern or ration crate to drink/eat before resuming `STATE_BURNOUT` stress recovery—ensuring an agent in burnout never dies of thirst or hunger while waiting for stress decay.
     * **Anti-Soft-Lock Rule 1 (Zero-Labor UI Designation Always Allowed):** Painting metadata zones (`[Designate Sleep Zone]`, `[Designate Recreation Zone]`, `[Cancel Designation]`) requires zero agent labor and is **never** blocked while Founder 01 is in `STATE_BURNOUT`, allowing a player who forgot to place a zone to designate one immediately.
     * **Anti-Soft-Lock Rule 2 (`AutonomousSelfSooth` Fallback):** If an agent in `STATE_BURNOUT` cannot pathfind to any reachable `[Recreation Zone]` or `[Sleep Zone]`, the agent sits on the spot and enters `AutonomousSelfSooth` (deep breathing / resting on ground), draining Stress at a guaranteed fallback rate of **`-10% Stress/hr` ($-10,000\text{ units/hr}$)** so `STATE_BURNOUT` is mathematically guaranteed to recover below the **`< 40%` ($40,000$ units)** exit threshold within $\le 6$ in-game hours.
* **Acceptance Criteria:**
  * **Given** Founder 01's `Stress` reaches $100\%$ ($100,000$ units) while `hydration` drops to $18\%$ (`< 20%`) and `caloric_reserve` drops to $450\text{ kcal}$ (`< 500 kcal`),
  * **When** `STATE_BURNOUT` evaluates on the simulation tick,
  * **Then** Founder 01 drops all physical labor tasks, life-critical somatic preemption executes first so Founder 01 drinks water and eats a ration meal without dying, and Founder 01 then rests in `[Recreation Zone]` (or `AutonomousSelfSooth` at `-10%/hr`) until `Stress < 40%` ($40,000$ units) restores normal work capability (`test_chill_loop_homeostasis.cpp`).

---

### Epic 10: Simulation Temporal Controls (Time Warp) & Runtime Hygiene
**Epic ID:** `EPIC-10` | **Total Estimate:** 12 Story Points | **Priority:** P0 Critical | **Target Sprint:** Dual Sprint 11  
**Focus:** Decouple `g_ecs->progress()` logical tick sub-stepping from once-per-frame 60 Hz WebGPU rendering and DOM HUD updates via interactive Time-Warp HUD controls (`[ || Pause ] [ > 1x ] [ >> 10x ] [ >>> 100x ]`), aggregate UHAI telemetry pushes once per frame to prevent 1,024-slot SPSC ring overflow, add Emergency Warp Brakes and a live Net Trajectory HUD Ticker, throttle `spdlog` console output to logical epochs, and isolate the legacy 60-tick auto-exit behind `--headless-ci`.

---

#### Story 10.1: Decoupled Time-Step Multiplier (`Pause / 1x / 10x / 100x`), Aggregated UHAI Frame Push, Emergency Warp Brake & Net Trajectory Ticker
* **ID:** `STORY-10.1` | **Points:** 7 SP | **Priority:** P0 | **Sprint:** Dual Sprint 11 | **Quality Gate:** G1, G4, G6
* **User Story:** *As a player overseeing multi-day crop growth, rainwater collection, and shelter construction, I want interactive HUD controls (`[ || Pause ] [ > 1x ] [ >> 10x ] [ >>> 100x ]`) and shortcuts (`Space`, `1`, `2`, `3`) that sub-step simulation ticks without re-running WebGPU rendering or overflowing the UHAI ring buffer, paired with an automatic Emergency Warp Brake on crisis events and a live Net Trajectory HUD Ticker.*
* **Technical Tasks:**
  1. Define `SimulationClockState` struct in the engine core: `time_scale_multiplier` (`uint32_t`: `0` = Pause, `1` = 1x, `10` = 10x, `100` = 100x), `logical_tick_counter` (`uint64_t`), `fixed_dt_seconds` (`constexpr float = 0.1f`), and `epoch_barrier_clamped` (`bool`).
  2. **Extract `RenderWebGPUFrame()` and `EM_ASM(updateHUD(...))` Out of `g_ecs->progress()` (`main.cpp`):**
     * Remove the WebGPU render pass and JavaScript DOM HUD update from inside Flecs `ecs.system()` tick execution.
     * In the 60 Hz main frame callback (`main_loop_step`), execute `for (uint32_t step = 0; step < sub_steps; ++step) { g_ecs->progress(fixed_dt); }` where `sub_steps` is `0` (`Pause`), `1` (`1x`), `10` (`10x`), or `100` (`100x`, guarded by a frame-time spiral-of-death cap if CPU tick time exceeds $12.0\text{ ms}$).
     * Execute `RenderWebGPUFrame()` and `EM_ASM(updateHUD(...))` **strictly once per 60 Hz visual frame** after the sub-step loop completes. When `time_scale_multiplier == 0` (`Pause`), `g_ecs->progress()` executes `0` times while Arcball camera orbit, raycasting, and context menu designation remain 100% live at 60 FPS.
  3. **Aggregate UHAI Telemetry Pushes Once Per 60 Hz Frame:**
     * Accumulate simulated exergy, thermal, and somatic deltas across the $N \in \{1, 10, 100\}$ logical sub-steps and push a single aggregated batch of outward `QUALITY_SIMULATED (0x01)` `TelemetrySample` frames (tagged with `logical_tick_counter`) into `/dev/shm/uhai_telemetry.ring` (`L1b -> L2`) **once per 60 Hz frame**, mathematically preventing 1,024-slot SPSC ring buffer overflow during `100x` Time Warp.
  4. Implement **Emergency Warp Brake**: automatically downshift `time_scale_multiplier` from `10x` or `100x` to **`1x`** (with an audible alert chime and HUD flash) whenever:
     * A Layer 7 `MigrationWaveArrivalEvent` arrives at the perimeter,
     * Any agent enters `STATE_BURNOUT` ($100\%$ Stress) or Tier 2/3 Breakdown, or
     * Enclave potable water or caloric reserves drop below $< 24\text{ hours}$ of autonomy ($< 2.0\text{ L/agent}$ or $< 2,000\text{ kcal/agent}$).
  5. Render the **Live Net Trajectory HUD Ticker** adjacent to `#time-warp-bar` (`pointer-events: auto`, `event.stopPropagation()`), displaying real-time net resource rates and runway: `[⚡ +4.2 kWh/d | 💧 +18.0 L/d | 🍲 -1,800 kcal/d (4.8 Days Autonomy)]`.
* **Acceptance Criteria:**
  * **Given** the simulation is running at `100x` Time Warp (`sub_steps = 100`),
  * **When** 60 visual frames (1 wall-clock second = 6,000 logical ticks) execute,
  * **Then** `g_ecs->progress()` runs 6,000 logical ticks while `RenderWebGPUFrame()`, `updateHUD()`, and aggregated UHAI telemetry emission execute exactly 60 times (zero dropped frames in `SharedRingHeader::dropped_frames_counter`),
  * **And** triggering `STATE_BURNOUT` or a Migration Wave immediately downshifts Time Warp to `1x` via the Emergency Warp Brake (`test_time_warp_and_throttling.cpp`).

---

#### Story 10.2: Adaptive Log Throttling & Isolation of Hardcoded 60-Tick CI Exit
* **ID:** `STORY-10.2` | **Points:** 5 SP | **Priority:** P0 | **Sprint:** Dual Sprint 11 | **Quality Gate:** G2, G4
* **User Story:** *As a developer and player running Oasis interactively at 1x, 10x, or 100x speed, I want console logging throttled to logical epoch intervals rather than raw frames/ticks, and I want the legacy 60-tick auto-termination isolated behind an explicit `--headless-ci` flag, so that interactive sessions run indefinitely without terminal spam or premature exit after 1 second.*
* **Technical Tasks:**
  1. Refactor `main.cpp` CLI argument parser: remove unconditional termination at `ticks >= 60`; introduce `--headless-ci` (and `--max-ticks <N>`) CLI flags so that automated CTest/CI jobs exit deterministically at 60 ticks while default native (`./oasis_engine`) and WASM browser launches run indefinitely until user quit (`SDL_QUIT` / window close).
  2. Implement `LogicalEpochLogThrottle` wrapper around `spdlog`: suppress per-tick/per-frame `spdlog::info` telemetry dumps during 10x/100x time-warp, emitting periodic summary logs only once every $N_{\text{log}} = 300$ logical ticks (or immediately on state-transition edges such as `STATE_BURNOUT`, Emergency Warp Brake, Migration Wave arrival, or Epoch Barrier lock).
  3. Verify that running at `100x` time-warp produces $< 5\text{ KB/s}$ of stdout output and zero I/O blocking stalls on the simulation thread.
* **Acceptance Criteria:**
  * **Given** the player launches `./oasis_engine` natively without CLI flags and sets Time Warp to `100x`,
  * **When** the simulation runs past 60, 600, and 60,000 logical ticks,
  * **Then** the application does not auto-exit at tick 60, continues running smoothly until the window is closed, emits summary logs only once every 300 logical ticks,
  * **And** `./oasis_engine --headless-ci` exits cleanly at tick 60 with return code `0` (`test_time_warp_and_throttling.cpp`).

---

### Epic 11: Migration Waves & Legacy NPC Assimilation
**Epic ID:** `EPIC-11` | **Total Estimate:** 23 Story Points | **Priority:** P1 High | **Target Sprint:** Dual Sprint 13  
**Focus:** Model the demographic transition from solo Founder survival to a cooperative enclave while strictly respecting Sovereign Stack Layer Adjacency (`L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> L1b`), `SocialMutualAidComponent::MAX_NPCS = 16` population bounds, and Trust Ring cryptography: ingest 32-byte `MigrationWaveArrivalEvent` packets from UHAI (`L2 -> L1b`), spawn Legacy NPCs at **`Ring R6 (Unverified Migrant)`** with a 12h Pre-Wave Warning and Arrival Card UI, model scarcity regressions on unmet needs, and advance migrants from **`R6 -> R3 (Probationary Resident)`** via Layer 3 PoTW verification ($\ge 7$ days sheltered homeostasis + $\ge 36.0\text{ MJ}$ sweat-equity) and from **`R3 -> R1 (Vouched Steward)`** via a Layer 5 2-of-3 FROST ceremony (`SCN-TAU-GENESIS`).

---

#### Story 11.1: Layer 7 Migration Wave Ingress (`L7 -> ... -> L2 -> L1b`), `MAX_NPCS = 16` Population Cap & Dramatic Arrival UI
* **ID:** `STORY-11.1` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Dual Sprint 13 | **Quality Gate:** G2, G4, G6
* **User Story:** *As Founder 01 establishing an enclave after my 3-day solo genesis phase, I want periodic Migration Waves originating in Layer 7 (`col-adversaryd` DES) and routed sequentially down the Sovereign Stack (`L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> L1b`) over UHAI to trigger a 12-hour Pre-Wave Horizon Warning, engage the Emergency Warp Brake, and spawn Legacy NPCs at the parcel boundary initialized at `Ring R6 (Unverified Migrant)` within the hard `SocialMutualAidComponent::MAX_NPCS = 16` population cap.*
* **Technical Tasks:**
  1. Define the canonical 32-byte `MigrationWaveArrivalEvent` struct (`alignas(8)`, `-Wpadded` clean) wrapped inside a 64-byte cache-aligned `MigrationWaveSlot` in `core/1_simulation/include/uhai_ring_buffer.hpp`:
     * `uint64_t logical_tick` (8B)
     * `uint64_t deterministic_seed` (8B)
     * `uint32_t wave_id` (4B)
     * `int32_t legacy_distress_q16_16` (4B)
     * `uint8_t migrant_count` (1B, clamped $1..8$)
     * `uint8_t ingress_edge` (1B: North/East/South/West perimeter)
     * `uint8_t initial_trust_ring` (1B: always `6` = **`Ring R6 (Unverified Migrant)`**)
     * `uint8_t reserved_pad[5]` (5B explicit padding $\implies$ `sizeof(MigrationWaveArrivalEvent) == 32`).
  2. Enforce **Strict Layer Adjacency (`L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> L1b`) & `SocialMutualAidComponent::MAX_NPCS = 16` Hard Population Cap**:
     * `col-adversaryd` (L7) schedules the wave in DES; the event traverses downward through L6, L5, L4, L3, and L2 (`col-meshd`), which writes the `MigrationWaveSlot` into `/dev/shm` UHAI for Oasis (L1b) to consume after the 3-day solo genesis grace period (`T_GENESIS_SOLO = 86,400` ticks).
     * Enforce **`SocialMutualAidComponent::MAX_NPCS = 16` hard population cap per parcel**: when `active_npc_count + migrant_count > 16`, clamp immediate entity spawning to `16 - active_npc_count` and hold excess migrants in a bounded perimeter staging queue so multiple migration waves during `100x` Time Warp never overflow fixed-size `SocialMutualAidComponent` arrays or Partition 4/5 static memory.
  3. Extend `CameraUniforms` / `DynamicVoxelStore` (`voxel_ssbo.hpp` & `raymarch.wgsl` / `raymarch_wgsl.hpp`) with a fixed **`npc_avatars[16]` / `npc_avatars[8]` GPU buffer** encoding $(x, y, z, \text{state\_and\_ring\_flags})$ so the WebGPU DDA raymarcher renders active NPC voxel avatars alongside Founder 01 at 60 FPS with zero dynamic memory allocation.
  4. Implement **12-Hour Pre-Wave Horizon Warning & Migrant Arrival Card UI**:
     * 12 in-game hours before arrival, display a HUD horizon radar warning (`[⚠️ Horizon Alert: Legacy Migration Wave (3 Migrants) approaching North Perimeter — ETA 12h]`).
     * Upon arrival, trigger the Story 10.1 **Emergency Warp Brake** (downshifting `10x/100x` to `1x`), spawn the NPC entities (up to `MAX_NPCS = 16`) in Flecs ECS at the perimeter with `PsychologyComponent` (`cumulative_stress = 60,000`), `SomaticStateComponent`, and `LegacyMigrantComponent` (`trust_ring_tier = R6_UNVERIFIED_MIGRANT`, `sheltered_days = 0`, `potw_sweat_equity_mj = 0.0f`), and display an interactive **Migrant Arrival Card** featuring a **`[ 🎯 Snap Camera to Perimeter ]`** button that smoothly animates `poi_origin` to the newly arrived migrants.
* **Acceptance Criteria:**
  * **Given** Founder 01 has passed the 3-day solo genesis phase and multiple `MigrationWaveArrivalEvent` packets (`sizeof == 32`, `initial_trust_ring = 6`) arrive via `L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> L1b` UHAI while running at `100x` Time Warp,
  * **When** Oasis consumes the `MigrationWaveSlot` frames,
  * **Then** the Emergency Warp Brake downshifts Time Warp to `1x`, NPC entities spawn at **`Ring R6 (Unverified Migrant)`** while strictly respecting `SocialMutualAidComponent::MAX_NPCS = 16` without array overflow, and clicking `[ 🎯 Snap Camera to Perimeter ]` centers the Arcball camera on the migrants (`test_migration_waves.cpp`).

---

#### Story 11.2: Legacy Mindset Friction, Stress Cascades & Behavioral Regression
* **ID:** `STORY-11.2` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Dual Sprint 13 | **Quality Gate:** G1, G3, G4
* **User Story:** *As the simulation engine, I want unassimilated `Ring R6` Legacy NPCs to exhibit fiat scarcity mindsets that trigger Stress cascades and behavioral regressions (ration hoarding, tool lock conflicts, and drawing emergency microgrid reserve power) if the enclave fails to provide adequate enclosed shelter, clean water, and calories.*
* **Technical Tasks:**
  1. Define `LegacyMindsetState` flags inside `LegacyMigrantComponent`: `SCARCITY_ANXIETY`, `HOARDING_ACTIVE`, `CONFLICT_REGRESSION`, and `RESERVE_POWER_DRAW`.
  2. Implement `LegacyFrictionSystem` in Flecs ECS: when an unassimilated NPC (`trust_ring_tier == R6_UNVERIFIED_MIGRANT`) experiences unmet somatic needs (`caloric_reserve < 1,000 kcal`, `hydration < 30%`, or sleeping in an unenclosed/cold space $< 18.0^\circ\text{C}$), their `Stress` accumulates at $2.0\times$ nominal rate and radiates an acute social scarcity anxiety aura ($+5\text{ %/hr}$ Stress) to agents within $4.0\text{ m}$.
  3. Implement 3 deterministic Legacy Regression behaviors when an `R6` migrant's `Stress` crosses $75\%$ ($75,000$ units):
     * **Ration Hoarding:** Agent withdraws $3\times$ normal food rations from Commons Kitchen / ration storage into private inventory, accelerating shared stockpile depletion.
     * **Tool Lease Conflict:** Agent refuses to yield shared tool locks and interrupts adjacent workers, injecting `THOUGHT_LEGACY_ARGUMENT` ($+15,000$ Stress units).
     * **Emergency Microgrid Reserve Draw:** Agent plugs uninsulated resistance heaters into local drops and **draws emergency microgrid reserve power** ($+2.5\text{ kW}$ continuous drain), depleting battery reserves and reducing Pillar 1/3 attenuation $\Phi_{\text{infrastructure}}(t)$.
  4. Render a **4-Needs Migrant Assimilation Checklist Tooltip** when inspecting any `R6` or `R3` NPC (`[✅ Enclosed Shelter >18°C] [✅ Hydration >50%] [✅ Calories >1,500 kcal] [⏳ PoTW Sweat-Equity: 18.4 / 36.0 MJ]`) so the player always knows the exact cause of friction or progress toward assimilation.
* **Acceptance Criteria:**
  * **Given** 3 `Ring R6` Legacy NPCs arrive during a cold snap and lack enclosed $>18.0^\circ\text{C}$ shelter or rations,
  * **When** their `Stress` cascades above $75\%$,
  * **Then** the `R6` NPCs enter `HOARDING_ACTIVE` and `RESERVE_POWER_DRAW` regressions (withdrawing $3\times$ rations and drawing emergency microgrid reserve power), and inspecting an NPC displays the 4-Needs Assimilation Checklist pinpointing the shelter/caloric deficit (`test_migration_waves.cpp`).

---

#### Story 11.3: Two-Stage Trust Ring Onboarding (`R6 -> R3` via Layer 3 PoTW & `R3 -> R1` via Layer 5 FROST Ceremony)
* **ID:** `STORY-11.3` | **Points:** 7 SP | **Priority:** P1 | **Sprint:** Dual Sprint 13 | **Quality Gate:** G1, G3, G4, G6
* **User Story:** *As Founder 01, I want `Ring R6` migrants who receive sheltered homeostasis and contribute physical labor to have their outward `QUALITY_SIMULATED (0x01)` work/shelter telemetry verified by Layer 3 (`col-storaged`) to earn promotion to `Ring R3 (Probationary Resident)` via Layer 5 (`col-kmsd`), and after 30 days in `R3`, I want existing `Ring R1` stewards to vouch for them via a 2-of-3 FROST threshold ceremony (`SCN-TAU-GENESIS`) to promote them to `Ring R1 (Vouched Steward)`.*
* **Technical Tasks:**
  1. Remove any direct `L1b -> L5/L6` `CitizenOnboardingIntent` shortcut and forbid automatic Trust Ring promotion via passive eating/sleeping alone.
  2. **Stage 1 Promotion (`Ring R6 Unverified Migrant -> Ring R3 Probationary Resident` via Layer 3 PoTW & Layer 5 KMS):**
     * Allow `R6` migrants whose immediate survival needs are met (`T_room > 18°C` shelter, `hydration > 50%`, `caloric_reserve > 1,500 kcal`, `Stress < 50%`) to perform basic supervised manual labor (hauling loose items, digging swales, watering crops).
     * Oasis emits outward `QUALITY_SIMULATED (0x01)` telemetry samples of the migrant's daily sheltered homeostasis and physical Joule work output (`potw_sweat_equity_mj`) strictly across **`L1b -> L2 -> L3` (`col-storaged`)**.
     * Once **Layer 3 (`col-storaged`)** verifies **$\ge 7$ consecutive simulated days of sheltered homeostasis AND $\ge 36.0\text{ MJ}$ ($10.0$ Exergy Credits) of Proof-of-Thermodynamic-Work (PoTW) sweat-equity**, Layer 3 notifies Layer 5 (`col-kmsd` via `L3 -> L4 -> L5`), which issues a `TrustRingPromotionEvent` (`R6 -> R3`) routed downward (`L5 -> L4 -> L3 -> L2 -> L1b`) over UHAI.
     * Upon receiving `R6 -> R3`, Oasis updates the NPC to **`Ring R3 (Probationary Resident)`**, assigns an opaque 32-byte `resident_did_handle`, purges `SCARCITY_ANXIETY` flags, grants `+20 SOLIDARITY` and `+15 STEWARDSHIP`, and binds the resident to the shared `AdvisoryIntentionQueue` for full autonomous fabrication and construction tasks.
  3. **Stage 2 Promotion (`Ring R3 Probationary Resident -> Ring R1 Vouched Steward` via 2-of-3 FROST Ceremony `SCN-TAU-GENESIS`):**
     * After an `R3` resident completes $\ge 30$ simulated days of zero-regression cooperative service, unlock the **`[ 🔏 Initiate Steward Vouch Ceremony (2-of-3 FROST) ]`** action in the Companion App / Layer 6 UI (`POST /api/v1/l6/intent -> L5 col-kmsd`).
     * When Layer 5 (`col-kmsd`) completes the **2-of-3 FROST threshold multi-signature ceremony (`SCN-TAU-GENESIS`)** and emits `TrustRingPromotionEvent` (`R3 -> R1`) downward via `L5 -> L4 -> L3 -> L2 -> L1b`, the citizen ascends to **`Ring R1 (Vouched Steward)`**, unlocking workshop Strange Mood masterwork leadership and peer vouching privileges.
* **Acceptance Criteria:**
  * **Given** a `Ring R6` migrant is sheltered and fed for 7 consecutive days while performing passive rest ($0.0\text{ MJ}$ PoTW),
  * **When** Layer 3 evaluates promotion eligibility,
  * **Then** the migrant remains at `Ring R6` until they also complete $\ge 36.0\text{ MJ}$ ($10.0$ Exergy Credits) of verified PoTW sweat-equity, at which point Layer 3 triggers Layer 5 to emit the `R6 -> R3` promotion downward via `L5 -> L4 -> L3 -> L2 -> L1b` UHAI,
  * **And** promotion from `R3 -> R1` occurs only after $\ge 30$ simulated days in `R3` and completion of a Layer 5 2-of-3 FROST threshold signature ceremony (`SCN-TAU-GENESIS`), verified by `test_migration_waves.cpp`.

---

### Epic 12: The Shared Virtual Landscape (`Arcadia server-01`) & Mandatory Milestone Epoch Sync
**Epic ID:** `EPIC-12` | **Total Estimate:** 22 Story Points | **Priority:** P1 High | **Target Sprint:** Dual Sprint 14  
**Focus:** Scale Oasis from a local-first single-parcel simulation into a voluntary localized MMO across the shared virtual landscape `Arcadia (server-01)`, consuming headless 24-byte `StateDeltaOp` cadastral records (`SS-EP9-001`) projected downward via `L3 -> L2 -> L1b` UHAI, and solving the multi-player Time-Warp relativity problem via the **Mandatory Milestone (End-of-Month Epoch Barrier)** featuring a `SOLO_AUTARKIC` zero-peer fast-path, an Interactive Blueprint Planning Phase during `EPOCH_SYNC_LOCKED`, and a 3D Holographic Horizon Wave reveal.

---

#### Story 12.1: `Arcadia (server-01)` Cadastral `StateDeltaOp` Projection (`L3 -> L2 -> L1b`) & Local 3D Blueprint Mapping
* **ID:** `STORY-12.1` | **Points:** 8 SP | **Priority:** P1 | **Sprint:** Dual Sprint 14 | **Quality Gate:** G2, G3, G6
* **User Story:** *As an Oasis player opting into the voluntary `Arcadia (server-01)` shared landscape, I want headless 24-byte `StateDeltaOp` cadastral records replicated by Layer 3 (`col-storaged`) and delivered downward via Layer 2 (`col-meshd`) over the UHAI ring (`L3 -> L2 -> L1b`) to be mapped locally by Oasis into 3D neighbor parcel voxel structures, so that we collaboratively reshape a persistent regional world without leaking graphics/voxel internals into the Sovereign Stack wire protocol.*
* **Technical Tasks:**
  1. Enforce **Strict Wire-Protocol Separation (`SS-EP9-001`) & Layer Adjacency (`L3 -> L2 -> L1b`)**: remove all graphics/voxel-specific fields (`compressed_voxel_rle_offset`, `chunk_coord`, `structure_ecs_archetype_id`) from the Sovereign Stack wire payload. Instead, Oasis consumes the canonical headless **24-byte `StateDeltaOp` (`alignas(8)`, `-Wpadded` clean)** wrapped inside a 64-byte `StateDeltaSlot` in `uhai_ring_buffer.hpp`:
     * `uint64_t entity_uuid` (8B)
     * `uint64_t logical_tick` (8B)
     * `uint16_t spatial_parcel_id` (2B)
     * `uint16_t structure_type_id` (2B: e.g., `STRUCT_BIOSWALE`, `STRUCT_SOLAR_ARRAY`, `STRUCT_COB_CABIN`, `STRUCT_COMMONS_CORRIDOR`)
     * `int32_t exergy_delta_j` (4B $\implies$ `sizeof(StateDeltaOp) == 24`).
  2. Implement `ArcadiaProjectionSystem` in Oasis (Layer 1b): poll verified `StateDeltaOp` records from the `L2 -> L1b` UHAI ring buffer and deterministically stamp the corresponding 3D voxel blueprint (`structure_type_id`) onto the neighbor parcel's coordinates in `DynamicVoxelStore` / WebGPU `VoxelSSBO`.
  3. Preserve strict local-first voluntariness (Gate G2 & Anti-Bleed Invariant 3): if a player disconnects from `Arcadia (server-01)` or runs offline, local simulation continues unimpeded using the last cached regional snapshot with zero network socket calls inside the Oasis binary.
* **Acceptance Criteria:**
  * **Given** Player A on Parcel 402 and Player B on Parcel 403 are connected to `Arcadia (server-01)` via their local `col-meshd` (L2) and `col-storaged` (L3) daemons,
  * **When** Player B completes a solar array and bioswale, causing `col-storaged` (L3) to replicate 24-byte `StateDeltaOp` records downward via `L3 -> L2 -> L1b` UHAI,
  * **Then** `static_assert(sizeof(StateDeltaOp) == 24)` holds, `ArcadiaProjectionSystem` maps the `structure_type_id` records into 3D voxel structures on Parcel 403 in Player A's viewport, and `scripts/verify_anti_bleed.sh` confirms zero networking symbols in the Oasis binary (`test_arcadia_epoch_sync.cpp`).

---

#### Story 12.2: Mandatory Milestone (`EPOCH_BOUNDARY_TICKS`), `SOLO_AUTARKIC` Fast-Path & Interactive Blueprint Planning Phase
* **ID:** `STORY-12.2` | **Points:** 8 SP | **Priority:** P0 | **Sprint:** Dual Sprint 14 | **Quality Gate:** G2, G4, G6
* **User Story:** *As a player using `10x` or `100x` Time Warp on `Arcadia (server-01)`, I want Time Warp to smoothly ramp down approaching `EPOCH_BOUNDARY_TICKS = 864,000` (End of Month) and enter an **Interactive Blueprint Planning Phase (`EPOCH_SYNC_LOCKED`)** while waiting for `L1b -> L2 -> L3 -> L4` mesh reconciliation—with a 1-tick `SOLO_AUTARKIC` fast-path when playing with zero peers and a `[ Continue Offline ]` escape hatch so I am never trapped waiting on a stalled peer.*
* **Technical Tasks:**
  1. Define canonical monthly epoch duration: `EPOCH_BOUNDARY_TICKS = 864,000` logical ticks (30 simulated days at $28,800\text{ ticks/day}$, overrideable via `--epoch-ticks <N>` for SITL testing).
  2. Implement **Smooth Time-Warp Ramp-Down**: when `EPOCH_BOUNDARY_TICKS - logical_tick_counter <= 1,800` (final 3 simulated hours of the month), automatically step `100x -> 10x -> 1x` so the player never slams abruptly into the epoch wall.
  3. Implement **`EpochBarrierGovernor` & `SOLO_AUTARKIC` Zero-Peer Fast-Path**:
     * At `logical_tick_counter % EPOCH_BOUNDARY_TICKS == 0`, emit `EpochBarrierReachedPayload { uint32_t epoch_number, uint32_t active_peer_count, uint64_t logical_ticks, uint8_t state_digest[32] }` (`alignas(8)`, 48 bytes) across UHAI (`L1b -> L2 -> L3 -> L4`).
     * **`SOLO_AUTARKIC` Fast-Path (`active_peer_count == 0`):** If the player is running solo/offline with zero active mesh peers, self-reconcile locally in **$\le 1\text{ tick}$** and immediately unlock the Epoch Reconciliation Summary without locking Time-Warp controls.
     * **Multi-Peer `Arcadia` Sync (`active_peer_count > 0`):** Clamp `time_scale_multiplier = 0` (`EPOCH_SYNC_LOCKED`) until `col-storaged` (L3) and `col-execd` (L4) verify peer CRDT Merkle roots and `FlowContract` clearing channels and emit `EpochReconciledAckPayload` downward via `L4 -> L3 -> L2 -> L1b` UHAI.
  4. Make **`EPOCH_SYNC_LOCKED` an Interactive Blueprint Planning Phase**:
     * While waiting for `EpochReconciledAckPayload`, keep 60 FPS Arcball camera orbit, roof cutaway (`z_cutaway_level`), `F1–F4` Info-View overlays, and **Right-Click / Left-Click Spatial Designation Painting** 100% active so the player can inspect their enclave and pre-paint Month $N+1$ blueprints (staged into `AdvisoryIntentionQueue` for execution the moment Month $N+1$ unlocks).
     * Render a live **Peer Epoch Convergence Progress Bar** (`[Arcadia server-01 Sync: 3/4 Parcels at End of Month 1 — Parcel 403 at Day 28 (100x)...]`) and an immediate **`[ 🔌 Sever Mesh & Continue Offline (SOLO_AUTARKIC) ]`** button so a player is never held hostage by an AFK peer.
* **Acceptance Criteria:**
  * **Given** Player A reaches `EPOCH_BOUNDARY_TICKS = 864,000`,
  * **When** `active_peer_count == 0` (`SOLO_AUTARKIC`), the barrier self-reconciles in $\le 1\text{ tick}$ without locking controls; and **when** `active_peer_count > 0`, `time_scale_multiplier` clamps to `0` (`EPOCH_SYNC_LOCKED`) while allowing Player A to freely orbit the camera, toggle `F1–F4` overlays, pre-paint Month 2 designations, or click `[ Continue Offline ]` until `EpochReconciledAckPayload` arrives via `L4 -> L3 -> L2 -> L1b` UHAI (`test_arcadia_epoch_sync.cpp`).

---

#### Story 12.3: Epoch Reconciliation Summary, Temporal Flow Settlement View & 3D Holographic Horizon Wave
* **ID:** `STORY-12.3` | **Points:** 6 SP | **Priority:** P1 | **Sprint:** Dual Sprint 14 | **Quality Gate:** G1, G4, G6
* **User Story:** *As a player completing a Mandatory Milestone synchronization on `Arcadia (server-01)`, I want an `#epoch-reconcile-modal` summary displaying our monthly thermodynamic balances, settled `FlowContract` exchanges, and Trust Ring promotions, and when I click `[ Commence Month N+1 ]`, I want a sweeping 3D Holographic Horizon Wave in WebGPU to visually materialize neighbor parcel `StateDeltaOp` structures.*
* **Technical Tasks:**
  1. Implement `EpochReconciliationSummaryModal` (`#epoch-reconcile-modal` with `pointer-events: auto` and `event.stopPropagation()`), triggered upon receiving `EpochReconciledAckPayload` from `L2 -> L1b` UHAI (or immediately via the `SOLO_AUTARKIC` fast-path).
  2. Render 4 structured telemetry & settlement panels from the reconciled epoch snapshot:
     * **Thermodynamic & Exergy Ledger:** Total kWh generated (**solar vs. reclaimed/salvaged**), total water harvested (L), and enclave thermal enclosure efficiency (%).
     * **Temporal Flow Contract Settlement:** Net zero-sum mutual-credit clearing balances (BCC) and fulfilled continuous resource flows (`FlowContract` kWh/L/calories exchanged with `Arcadia` peers via Layer 3 `col-storaged`).
     * **Enclave Demography & Trust Rings:** Total residents promoted to `Ring R3 (Probationary Resident)` via PoTW and `Ring R1 (Vouched Steward)` via FROST, average enclave `Stress` and `Focus`, and Masterwork artifacts crafted.
     * **`Arcadia (server-01)` Regional Horizon:** Updated regional boundary friction $\kappa$, consolidated Commons Corridors, and upcoming Layer 7 macro-economic regime forecast.
  3. Implement the **3D Holographic Horizon Wave Reveal**: when the player clicks `[ Commence Month N+1 ]`, clear `epoch_barrier_clamped`, reset `time_scale_multiplier` to `1x`, and trigger an expanding radial cyan/gold shader wave (`horizon_wave_radius` uniform in `CameraUniforms`) across the WebGPU viewport that materializes all newly synced neighbor parcel `StateDeltaOp` structures across `Arcadia (server-01)`.
* **Acceptance Criteria:**
  * **Given** the End-of-Month Mandatory Milestone barrier receives `EpochReconciledAckPayload` for Month 1,
  * **When** `#epoch-reconcile-modal` renders and the player clicks `[ Commence Month 2 ]`,
  * **Then** the modal displays exact mass-and-energy-conserving monthly totals (**solar vs. reclaimed/salvaged** kWh, water L, `FlowContract` BCC settlements, and `R6 -> R3 -> R1` citizen promotions), and unlocking Month 2 sweeps the 3D Holographic Horizon Wave across the viewport to reveal neighbor parcel updates (`test_arcadia_epoch_sync.cpp` & `scripts/verify_canvas.js`).

---

## 6. Systematic Quality Gate Verification & Automated Test Target Matrix

Every user story and scenario across Epics 1–12 must satisfy the **Six Inviolable Quality Gates (G1–G6)** codified in `docs/COLLECTIVE_GUIDING_PRINCIPLES.md`, `docs/OASIS_GDD.md`, and `docs/BOOK_TEAM_DRIFT_REPORT.md`, verified by explicit Catch2 targets registered in `CMakeLists.txt` (both the 10 existing baseline Catch2 binaries and the 5 new Dual Sprint 11–14 Catch2 binaries) plus Playwright browser automation in `scripts/verify_canvas.js` / `tests/verify_canvas.js`:

```
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       SIX INVIOLABLE QUALITY GATES (G1–G6)                                            │
├───────┬───────────────────────────────┬───────────────────────────────────────────────────┬───────────────────────────┤
│ Gate  │ Domain                        │ Core Invariant Assertion & Verification Method    │ CMake Catch2 & Playwright │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────┼───────────────────────────┤
│ **G1**│ Thermodynamic Conservation    │ Mass/energy balances obey conservation within     │ `test_construction_caloric│
│       │ & Physical Bounds             │ ±10%. Enclosed room BFS (`MAX_BFS_VISITED = 512`),│ _burn`, `test_homeostasis_│
│       │                               │ Day 1 Camp Kit intake, and SOR residuals conserve.│ loop`, `test_chill_loop_  │
│       │                               │                                                   │ homeostasis`              │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────┼───────────────────────────┤
│ **G2**│ Offline Local-First Partition │ 100% deterministic execution with zero WAN pings. │ `test_boundary_math`,     │
│       │ & Privacy Resilience          │ `SOLO_AUTARKIC` zero-peer fast-path (<=1 tick) and│ `test_temporal_flow_mesh`,│
│       │                               │ `[Continue Offline]` escape hatch guarantee local │ `test_arcadia_epoch_sync` │
│       │                               │ autonomy.                                         │                           │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────┼───────────────────────────┤
│ **G3**│ Byzantine Detection &         │ Sensor spoofing, Sybil rings, and `Ring R6`       │ `test_adversary_des`,     │
│       │ Sybil Immunity                │ scarcity regressions bounded (`MAX_NPCS = 16`).   │ `test_migration_waves`    │
│       │                               │ `R6->R3` requires >=7d shelter + >=36.0 MJ PoTW;  │                           │
│       │                               │ `R3->R1` requires 2-of-3 FROST ceremony.          │                           │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────┼───────────────────────────┤
│ **G4**│ Resource & State Machine      │ Lyapunov damping, 256-voxel drag cap, ray-miss    │ `test_camera_ssbo`,       │
│       │ Invariants (Zero Escrow Hang) │ no-op, life-critical preemption during `BURNOUT`, │ `test_avatar_vitals`,     │
│       │                               │ `AutonomousSelfSooth`, decoupled Time-Warp loop,  │ `test_arcball_and_designa-│
│       │                               │ and Emergency Warp Brake.                         │ tor`, `test_time_warp_and_│
│       │                               │                                                   │ throttling`               │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────┼───────────────────────────┤
│ **G5**│ Legacy Carapace Fiat Ingestion│ External Web2 Stripe/fiat payloads compile cleanly│ `test_adversary_des`,     │
│       │ & Cooperative Conversion      │ to internal intents without KYC identity leakage; │ `test_col_gatewayd`       │
│       │                               │ strict Layer 6 Intent U-Turn Law enforced.        │                           │
├───────┼───────────────────────────────┼───────────────────────────────────────────────────┼───────────────────────────┤
│ **G6**│ SITL Hardware-in-the-Loop,    │ Zero embedded daemons in engine. Strict `L1b<->L2`│ `test_uhai_ring_buffer`,  │
│       │ Layer Adjacency & Airgap Gate │ UHAI adjacency. Outward-only `QUALITY_SIMULATED`  │ `test_airgap_filter`,     │
│       │                               │ (0x01) telemetry; zero live sensor ingestion.     │ `scripts/verify_canvas.js`│
└───────┴───────────────────────────────┴───────────────────────────────────────────────────┴───────────────────────────┘
```

### 6.1. Complete CMake Catch2 & Playwright Verification Target Registry (Sprints 1–14)

| Sprint / Phase | CMake Target / Script (`CMakeLists.txt` / `scripts/`) | Core Verified Invariants |
| :--- | :--- | :--- |
| **Baseline (SP 1–10)** | `test_uhai_ring_buffer`, `test_boundary_math`, `test_airgap_filter`, `test_construction_caloric_burn`, `test_adversary_des`, `test_camera_ssbo`, `test_avatar_vitals`, `test_homeostasis_loop`, `test_temporal_flow_mesh`, `test_col_gatewayd` | SPSC `/dev/shm` ring (<50 µs), Poisson boundary & Genesis Epsilon Guard, `QUALITY_SIMULATED` airgap drop, fabrication caloric burn, L7 DES scheduling, dynamic Voxel SSBO, avatar vitals HUD, ambient thermal loop, `FlowContract` mesh, and `col-gatewayd` Intent U-Turn. |
| **Dual Sprint 11** (`EPIC-8`) | `test_arcball_and_designator` (`test_arcball_and_designator.cpp`) & `scripts/verify_canvas.js` | `[-64, 64]` DDA bounds (`MAX_STEPS = 192`), POI Arcball $(\theta, \phi \in [5^\circ, 85^\circ], r \in [2, 150])$, ray-miss `hit.hit == false` strict no-op, shortest-arc `std::remainder` $\theta$ wrap, `z_cutaway_level` roof slicing, DOM `contextmenu` suppression, `MAX_DESIGNATION_VOXELS = 256` cap, stockpile hauling, and 264B `SpatialIntentPayload` / 320B `SpatialIntentSlot` `static_assert`s. |
| **Dual Sprint 11** (`EPIC-10`) | `test_time_warp_and_throttling` (`test_time_warp_and_throttling.cpp`) | Decoupled `g_ecs->progress()` sub-stepping ($N \in \{0, 1, 10, 100\}$) vs. single 60 Hz `RenderWebGPUFrame()` & aggregated UHAI push (zero ring overflow), Emergency Warp Brake downshift to `1x`, `--headless-ci` 60-tick isolation, and 300-tick `spdlog` throttling. |
| **Dual Sprint 12** (`EPIC-9`) | `test_chill_loop_homeostasis` (`test_chill_loop_homeostasis.cpp`) | Day 1 Genesis Camp Kit bootstrap ($12$ meals, $60\text{ L}$ water, 3×3 Bell Tent), dirty-flag cached $O(H)$ sky-column + `MAX_BFS_VISITED = 512` hard node cap in `ThermalEnclosureSolver`, passable airtight `MAT_DOOR`, `+45 Focus/hr` sleep recovery, `-25%/-40%` Recreation Stress drain, and `STATE_BURNOUT` life-critical somatic preemption (`hydration < 20%`, `caloric_reserve < 500 kcal`) + `AutonomousSelfSooth` (`-10%/hr`). |
| **Dual Sprint 13** (`EPIC-11`) | `test_migration_waves` (`test_migration_waves.cpp`) | 32B `MigrationWaveArrivalEvent` (`L7->...->L2->L1b`) `static_assert`, `SocialMutualAidComponent::MAX_NPCS = 16` population cap, `npc_avatars` GPU sync, `Ring R6` scarcity regressions (ration hoarding & emergency reserve power draw), Layer 3 PoTW verification ($\ge 7\text{d}$ shelter + $\ge 36.0\text{ MJ}$) for `R6 -> R3`, and Layer 5 2-of-3 FROST ceremony for `R3 -> R1`. |
| **Dual Sprint 14** (`EPIC-12`) | `test_arcadia_epoch_sync` (`test_arcadia_epoch_sync.cpp`) | Headless 24B `StateDeltaOp` (`L3->L2->L1b`) `static_assert` and local 3D voxel mapping, smooth warp ramp-down approaching `EPOCH_BOUNDARY_TICKS = 864,000`, `SOLO_AUTARKIC` $\le 1\text{ tick}$ fast-path, interactive blueprint planning during `EPOCH_SYNC_LOCKED`, and `[ Continue Offline ]` escape hatch. |

---

## 7. Definition of Ready (DoR) and Definition of Done (DoD)

### 7.1. Definition of Ready (DoR)
A User Story is considered **Ready for Sprint Commitment** only when:
1. Grounded in at least one canonical MVS Scenario (`docs/scenarios/*.md`), `HORIZON_ROADMAP.md` pillar, or explicit GDD V6.0 architectural directive.
2. Contains concrete, verifiable Given/When/Then acceptance criteria.
3. Explicitly assigned to a single Sprint with Story Point estimation agreed upon by the Council.
4. Mapped directly to at least one Quality Gate (G1 through G6) and a named Catch2 / Playwright verification target.
5. All external interface contracts (UHAI `L1b <-> L2` structs, Flecs ECS archetypes, WGSL uniforms) are statically sized with explicit padding (`-Wpadded` clean) and compliant with `docs/BOOK_TEAM_DRIFT_REPORT.md`.

### 7.2. Definition of Done (DoD)
A User Story is considered **Done** only when:
1. Implemented in C++20 / WGSL following strict memory constraints (zero heap allocations on tick) and synchronized across both `shaders/raymarch.wgsl` and `src/raymarch_wgsl.hpp`.
2. Compiles cleanly across native macOS, Linux, and WebAssembly targets with `-Wall -Wextra -Wpadded -Werror`.
3. All 10 baseline Catch2 suites, the 5 new Horizon Catch2 suites (`test_arcball_and_designator`, `test_time_warp_and_throttling`, `test_chill_loop_homeostasis`, `test_migration_waves`, `test_arcadia_epoch_sync`), and Playwright (`scripts/verify_canvas.js`) pass 100% in headless CI (`--headless-ci`).
4. Verified against all associated Quality Gates (G1–G6) without tolerance exceptions.
5. Passes the static Anti-Bleed Linter (`scripts/verify_anti_bleed.sh`) with zero boundary violations, strict `L1b <-> L2` UHAI adjacency, and zero inward live-telemetry ingestion paths.
6. Memory profiled under AddressSanitizer (ASan) and WebAssembly runtime, verifying zero memory leaks and strict 256.00 MB linear memory adherence.
