# THE SOVEREIGN STACK — PRODUCT BACKLOG (LAYERS 1–7, GATEWAY & VERIFICATION)
**Document Version:** 4.0.0-RELEASE (Companion V2.0, Migration Waves & Arcadia Epoch Sync Edition)  
**Status:** Complete, Ratified & Authoritative  
**Milestone:** Sovereign Stack Autonomous Protocols, Companion App V2.0 Semantic Gateway (`col-gatewayd`), Intent U-Turn Enforcement, Legacy Migration Waves & `Arcadia (server-01)` Mandatory Milestone Epoch Synchronization  
**Target Environments:** Headless Edge Daemons (`collectived`, Linux ARM64 / RISC-V / x86_64, Microcontrollers / Zephyr RTOS) & WebAssembly Sandbox  
**Architectural Baseline:** Autonomous 8-Daemon Hierarchy, POSIX Process Isolation, Unix Domain Socket IPC, Strict Intent U-Turn (Zero `l4_l7.sock`), Zero Monolithic Threading, Scenario Driven Development (SDD) Alignment, Volume 1 Canonical Fidelity & `BOOK_TEAM_DRIFT_REPORT.md` Compliance  
**Consensus Status:** Unanimous Consensus Reached (6-Persona Horizon & Strategic Realignment Council)

---

## 1. Executive Summary & Strategic Realignment Council Ratification

### 1.1. Council Ratification & Unanimous Consensus
The Strategic Realignment & Horizon Council convened to upgrade the authoritative architectural baseline of the Sovereign Stack (`SOVEREIGN_STACK_BACKLOG.md`) from Version 3.0.0-RELEASE to **Version 4.0.0-RELEASE**, reconciling Sprints 1–10 and integrating the Companion App V2.0 architecture (`docs/COMPANION_APP_ARCHITECTURE.md`), the narrative drift remediations (`docs/BOOK_TEAM_DRIFT_REPORT.md`), and the 6 Horizon Pillars (`HORIZON_ROADMAP.md` / `PRODUCT_BACKLOG_V6.md`). Representing all stakeholder disciplines:

1. **Product Owner / Orchestrator (`explorer_po_v4`):** Delivery roadmap governance across Sprints 1–10 (completed baseline) and Dual Sprints 11–14 (Horizon roadmap), epic-level value delivery, cross-stack acceptance criteria, and synchronization between the 24 canonical SDD scenarios, the 6 Horizon Pillars, and the underlying daemon architecture.
2. **Sovereign Stack Specialist (`explorer_sovereign_v4`):** Canonical fidelity to Volume 1 of the book (*The Sovereign Stack: A Blueprint for Cybernetic Ecology and Mutual Aid*, `doc/book_collective/`) and `docs/BOOK_TEAM_DRIFT_REPORT.md`: Strict Adjacency, Proof-of-Thermodynamic-Work (PoTW) negentropy minting, continuous physical demurrage, Bilateral Clearing Channels (BCC), Bioregional Path-Vector Protocol (BPVP / FRRP), Cooperative Tripartite Legal Carapace (BMAA, Subchapter T SSAs, UCEA Easements), Systemic Legal Blinding, and the Strict Intent U-Turn doctrine.
3. **Quality Engineer (`explorer_qe_v4`):** Eradication of monolithic threading anti-patterns, POSIX process isolation, failure domain containment, cgroups resource enforcement, Six Inviolable Quality Gates (G1–G6), consumer-driven contract testing across all IPC sockets, and deterministic cross-peer Epoch Merkle reconciliation.
4. **Stack Engineer (`explorer_stack_v4`):** Systems engineering specification of the 8 autonomous POSIX daemons (`col-telemetryd`, `col-meshd`, `col-storaged`, `col-execd`, `col-kmsd`, `col-commonsd`, `col-adversaryd`, and `col-gatewayd`), Unix Domain Socket IPC bus, dedicated 1:1 lock-free SPSC ring buffers, Wasmtime metered execution, Automerge/Yrs CRDT reconciliation, and HLC-paced `FlowContract` epoch barriers.
5. **Oasis Boundary Architect (`explorer_oasis_v4`):** Total excision of client-side graphics rendering, voxel raymarching, orbital camera controllers, and gameplay psychology from the stack; strict demarcation of Oasis as a non-transitionary SITL shadow simulator and strictly an **outward producer** of simulated telemetry (`QUALITY_SIMULATED = 0x01`) that never ingests live physical telemetry; and enforcement of the physical hardware airgap.

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                      STRATEGIC REALIGNMENT & HORIZON COUNCIL UNANIMOUS CONSENSUS MATRIX (V4.0)                         │
├──────────────────────────────┬────────────────────────────────┬────────────────────────────────────────────────────────┤
│ Council Persona              │ Core Disciplinary Mandate      │ Key Ratified Contribution                              │
├──────────────────────────────┼────────────────────────────────┼────────────────────────────────────────────────────────┤
│ Product Owner (PO)           │ Delivery pacing & Horizon plan │ 9 Epics, 50 Stories, 358 SP, Sprints 1–14 roadmap      │
│ Sovereign Stack Specialist   │ Book Fidelity & Drift Align    │ PoTW, ERC, BCC, BPVP, Cooperative Carapace, U-Turn     │
│ Quality Engineer (QE)        │ Anti-monolith & Quality Gates  │ Six Inviolable Gates (G1–G6), cgroups, Epoch Parity    │
│ Stack Engineer               │ Autonomous runtime daemons     │ 8 POSIX daemons (+col-gatewayd), UDS IPC, HLC Barrier  │
│ Oasis Boundary Architect     │ Boundary purity & airgap check │ Outward-only SITL producer (0x01), non-transitionary   │
└──────────────────────────────┴────────────────────────────────┴────────────────────────────────────────────────────────┘
```

The council formally confirms that **Unanimous Consensus Reached** has occurred across all technical, architectural, and systemic domains:
- **Unanimous Consensus Reached on Zero Monolithic Threading:** All Sovereign Stack layers operate as independent, decoupled POSIX daemons or isolated Web Workers, with zero reliance on client engine game loops or monolithic threading pools.
- **Unanimous Consensus Reached on Pure Scope Isolation & Non-Transitionary Simulation Demarcation (`BOOK_TEAM_DRIFT_REPORT.md`):** Zero Oasis visual or gameplay mechanics (rendering passes, POI arcball camera rigs, spatial context menus, DDA voxel algorithms, psychological stress/burnout bars) exist within the Sovereign Stack codebase. Oasis is strictly a non-transitionary Software-In-The-Loop (SITL) shadow simulator—never a "game that transitions into a real-world orchestrator."
- **Unanimous Consensus Reached on Outward-Only Simulated Telemetry & Strict Physical Airgap:** Oasis is strictly an **outward producer** of simulated Layer 2 telemetry (`QUALITY_SIMULATED = 0x01`) for stress-testing downstream protocols and **never ingests live physical telemetry**. Telemetry marked `QUALITY_SIMULATED` (`0x01`) can never, under any circumstance, actuate real physical edge relays or breakers, which strictly demand `QUALITY_HARDWARE_ROOT` (`0x02`) signed by hardware secure elements and Layer 4 execution keys.
- **Unanimous Consensus Reached on Cooperative Legal Carapace Terminology:** All deprecated adversarial or subversive framing flagged in `docs/BOOK_TEAM_DRIFT_REPORT.md` is permanently stripped across the specification. The Sovereign Stack is a cooperative, structural, and legal mutual-aid architecture; `col-adversaryd` models external utility, municipal, and demographic stresses in good faith via Discrete Event Simulation (DES).
- **Unanimous Consensus Reached on Companion V2.0 Gateway & Strict Intent U-Turn:** `col-gatewayd` exposes human-authored intent submission exclusively at Layer 6 (`POST /api/v1/l6/intent`), while Layers 2, 3, 4, 5, and 7 on the gateway diagnostic hood are strictly read-only (`403 Forbidden` / `405 Method Not Allowed` on write attempts), enforcing the Intent U-Turn (`Human -> L6 -> L5 -> L4 -> [Upward Yield] -> L5 -> L6 -> L7`) with zero `l4_l7.sock`.
- **Unanimous Consensus Reached on Migration Waves & `Arcadia (server-01)` Epoch Synchronization:** Layer 7 schedules macroscopic demographic migration waves (`SS-EP7-007`) assimilated via Layer 5 progressive Trust Ring ($R_6 \to R_1$) onboarding (`SS-EP5-007`), while Epic 9 (`SS-EP9`) synchronizes voluntary shared landscape deltas and bounds HLC `FlowContract` time-warp ($1\times$ to $100\times$) at monthly **Mandatory Milestone Barriers**.

---

### 1.2. The Evolution: V1 Conflation $\to$ V2 Decoupling $\to$ V3 SDD Realignment $\to$ V4 Companion V2.0 & Arcadia Epoch Sync
The Sovereign Stack specification has evolved through four distinct evolutionary stages:

1. **Version 1.0 (The Monolithic Conflation):** The initial draft erroneously collapsed distributed networking, hardware telemetry, cryptographic proofs, and municipal simulations into arbitrary threads of a client-side game engine ("Thread 1 rendering", "Thread 3 running Layer 3"). This created fatal coupling: if cryptographic pairing stalled, battery safety telemetry starved.
2. **Version 2.0 (The Daemon Decoupling):** V2 corrected the threading flaw by establishing seven autonomous POSIX daemons communicating over Unix Domain Sockets (UDS), introducing the Universal Human-Agent Interface (UHAI) and purging client graphics.
3. **Version 3.0 (The SDD & Cybernetic Realignment):** V3 elevated the decoupled stack to execute the full civilizational requirements of Scenario Driven Development (SDD):
   - **Full 24-Scenario Hardware & Runtime Matrix:** Direct mapping of all 24 canonical scenarios across the 5 Developmental Acts (Metabolic Foundation, Spatial/Cultural, Material Closure, Human Fabric, Shocks/Decoupling) into specific hardware drivers, CRDT ledgers, BPMN workflows, and legal wrappers.
   - **Mathematical Rigor & Formulations:** Complete formalization of Proof-of-Thermodynamic-Work ($\Delta V = \int (\Phi_{in} - \Phi_{out})\lambda_{ERC} dt$), the Asymptotic ERC Firewall ($\lambda_{ERC} \to \infty$), Pairwise Bilateral Clearing Channels (BCC: $B_{u \to v}(t) + B_{v \to u}(t) = 0$), and Bioregional Path-Vector Protocol (BPVP / FRRP) across Concentric Trust Rings ($R_0 \to R_6$).
   - **Tripartite Legal Chassis & Systemic Blinding:** Operationalization of Layer 7 (`col-adversaryd`) as a dual-membrane cooperative legal carapace utilizing Bilateral Mutual-Aid Agreements (BMAA under UUNAA § 6), Subchapter T SSAs (26 U.S.C. §§ 1381–1388), and UCEA Conservation Easements, backed by structural blinding (Cellular Independence, Mundane NAICS Classification, IEEE 1547 net-meter profile standardization, and Subpoena Immunity via Data Non-Existence).
   - **Six Inviolable Quality Gates (G1–G6):** Rigorous, verifiable gates establishing mathematical and structural thresholds required prior to deploying any scenario or workflow to bare-metal hardware.
4. **Version 4.0 (Companion V2.0, Migration Waves & Arcadia Epoch Sync Edition — Current Baseline):** V4 integrates the 8th daemon (`col-gatewayd`), remediates narrative drift (`docs/BOOK_TEAM_DRIFT_REPORT.md`), and builds the backend protocols for the 6 Horizon Pillars (`HORIZON_ROADMAP.md`):
   - **Companion V2.0 Semantic Gateway (`col-gatewayd`) & Strict Intent U-Turn (`SS-EP6-006`):** Adds the VLAN 30 TLS 1.3 / WSS gateway on port 8443. Enforces that member interactions enter **exclusively at Layer 6** (`POST /api/v1/l6/intent`), which assumes Layer 7 does not exist internally, while Layers 2, 3, 4, 5, and 7 exposed on the gateway diagnostic hood remain strictly **read-only** (`403 Forbidden` / `405 Method Not Allowed` on any write attempt). Enforces the **Intent U-Turn** (`Human -> L6 -> L5 -> L4 -> [Upward Yield] -> L5 -> L6 -> L7`) with zero `l4_l7.sock`.
   - **Demographic Shocks & Progressive Trust Ring ($R_6 \to R_1$) Assimilation (`SS-EP7-007` & `SS-EP5-007`):** Expands `col-adversaryd` to generate macroscopic `MigrationWaveArrivalEvent` packets during legacy inflation or grid failures, and expands `col-kmsd` to manage probationary pairwise DIDs (`Ring R6: Unverified Migrant` $\to$ `Ring R3: Probationary Resident` $\to$ `Ring R1: Vouched Steward`) as shelter, water, and caloric mutual-aid milestones are attested in Layer 3.
   - **Shared Virtual Landscape (`Arcadia server-01`) & Mandatory Milestone Epoch Synchronization (`SS-EP9-001` to `SS-EP9-003`):** Introduces Epic 9 across `col-meshd`, `col-storaged`, and `col-execd` to relay voluntary coarse terrain/infrastructure CRDT deltas, pace `FlowContract` streams by HLC logical ticks across $1\times$–$100\times$ local time-warp multipliers, and enforce a hard cryptographic **Mandatory Milestone Barrier** at `EPOCH_BOUNDARY_TICKS` (e.g., 1 simulated month) with delay-tolerant Merkle reconciliation and straggler quorum grace windows.

---

### 1.3. Strict Separation of Concerns (SoC) & Anti-Bleed Invariants Manifest
The Sovereign Stack enforces five strict architectural boundary rules, validated via continuous static analysis and CI gating (`scripts/lint_anti_bleed.sh`):

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       SOVEREIGN STACK ARCHITECTURAL ANTI-BLEED MANIFEST                                │
├──────────┬───────────────────────────────┬─────────────────────────────────────────────────────────────────────────────┤
│ Rule ID  │ Invariant Name                │ Architectural Rule & Enforcement Mechanism                                  │
├──────────┼───────────────────────────────┼─────────────────────────────────────────────────────────────────────────────┤
│ **SBR-1**│ **The Headless Rule**         │ The Sovereign Stack compiles and executes 100% headlessly with zero graphics│
│          │                               │ or windowing dependencies (`SDL2`, `SDL3`, `WebGPU`, `Dawn`, `Vulkan`).     │
│          │                               │ Enforcement: CI headless build with no display/GPU packages installed.      │
├──────────┼───────────────────────────────┼─────────────────────────────────────────────────────────────────────────────┤
│ **SBR-2**│ **The Frame Rate Rule**       │ Zero references to display frame rates, tick budgets, or render loops       │
│          │                               │ exist in stack runtimes (no frame ceilings, no `render_ms`, no vsync).      │
│          │                               │ Enforcement: Grep linter halts CI if render/frame terminology appears.     │
├──────────┼───────────────────────────────┼─────────────────────────────────────────────────────────────────────────────┤
│ **SBR-3**│ **The Domain Entity Rule**    │ Zero Oasis gameplay entities (founder avatars, DF psychological stress bars,│
│          │                               │ Sims hunger/hygiene stats, voxel chunk geometry, arcball cameras) exist in  │
│          │                               │ stack daemons. Enforcement: Clang-tidy AST check keeping game structs out.  │
├──────────┼───────────────────────────────┼─────────────────────────────────────────────────────────────────────────────┤
│ **SBR-4**│ **The Presentation Agnostic** │ Daemons emit structured binary/JSON-LD data events only. Zero styling,      │
│          │                               │ UI banners, HUD alerts, or 3D coordinate transformations are computed.      │
│          │                               │ Enforcement: Socket schema review rejects visual representation fields.     │
├──────────┼───────────────────────────────┼─────────────────────────────────────────────────────────────────────────────┤
│ **SBR-5**│ **The Airgap & Outward-Only** │ All interaction with Oasis occurs across the UHAI IPC boundary. Oasis is a  │
│          │ **SITL Producer Rule**        │ non-transitionary shadow simulator and strictly an **outward producer** of  │
│          │                               │ simulated telemetry (`QUALITY_SIMULATED = 0x01`) for SITL testing; it       │
│          │                               │ **never ingests live physical telemetry**. `QUALITY_SIMULATED` packets can  │
│          │                               │ never energize physical edge relays, which require `QUALITY_HARDWARE_ROOT`  │
│          │                               │ (`0x02`). Enforcement: Kernel eBPF filter & L1 HAL hardware interlock.      │
└──────────┴───────────────────────────────┴─────────────────────────────────────────────────────────────────────────────┘
```

#### Strict Adjacency Protocol ("No Layer Skipping") & The Strict Intent U-Turn
In accordance with Volume 1 Chapter 1 of the book (`doc/book_collective/`) and `docs/COMPANION_APP_ARCHITECTURE.md` (V2.0), inter-layer communication strictly follows adjacent traversal:
$$\text{L7 (Carapace)} \longleftrightarrow \text{L6 (Intent)} \longleftrightarrow \text{L5 (Governance)} \longleftrightarrow \text{L4 (Orchestrator)} \longleftrightarrow \text{L3 (Ledger)} \longleftrightarrow \text{L2 (Mesh)} \longleftrightarrow \text{L1 (Ground)}$$

- **Downward Adjacency:** Plain language needs (L6) cannot directly actuate water valves (L1). Requests compile down through L5 governance verification, L4 task scheduling, L3 exergy reservation, L2 routing, and L1 hardware safety interlocks. Each intermediate layer possesses full authority to veto the command.
- **Upward Adjacency:** Physical sensor signals (L1) cannot directly alter governance policies (L5). Telemetry is digitized in L2, causally ordered and verified in L3, orchestrated into state updates in L4, and only then surfaced as governance context in L5.
- **The Strict Intent U-Turn & Read-Only Cybernetic Hood (`col-gatewayd` on VLAN 30 Port `8443`):**
  - `col-gatewayd` serves the Companion App on **VLAN 30 exclusively over TLS 1.3 HTTPS/WSS on port `8443`** (`https://collective.local:8443` / `wss://collective.local:8443/ws/v1`; never unencrypted HTTP `8080`). Member mutations enter **exclusively at Layer 6** (`POST /api/v1/l6/intent`), which assumes Layer 7 does not exist internally. Layers 2, 3, 4, 5, and 7 exposed on the `col-gatewayd` diagnostic hood are strictly **read-only** (`403 Forbidden` / `405 Method Not Allowed` on any write attempt, superseding any direct `/api/v1/l5/*` POST mutations).
  - Layer 4 (`col-execd`) is a deterministic, blind Wasmtime sandbox with **zero `l4_l7.sock` connection**. Whenever a Layer 4 `FlowContract` or workflow requires human governance ratification or external legacy interaction, it executes the **Intent U-Turn**:
    $$\text{Downward Intent \& Ratification:} \quad \text{Human} \xrightarrow[\text{POST /api/v1/l6/intent}]{\text{col-gatewayd (:8443)}} \text{L6 (\texttt{col-commonsd})} \xrightarrow{\text{l5\_l6.sock}} \text{L5 (\texttt{col-kmsd})} \xrightarrow{\text{l4\_l5.sock}} \text{L4 (\texttt{col-execd})}$$
    $$\text{Upward Yield:} \quad \text{L4 (\texttt{col-execd})} \xrightarrow[\text{YIELD\_UPWARD\_L5}]{\text{l4\_l5.sock}} \text{L5 (Hold / Notify via WSS :8443)} \xrightarrow{\text{l5\_l6.sock}} \text{L6 (Camouflage Translate)} \xrightarrow{\text{l6\_l7.sock}} \text{L7 (\texttt{col-adversaryd})}$$
- **Deterministic Boundary:** Generative AI / LLM models are strictly confined to Layers 5 and 6 as non-authoritative advisory drafting tools. Layers 1 through 4 are 100% mathematically deterministic.

---

### 1.4. Universal Human-Agent Interface (UHAI) Specification

The Universal Human-Agent Interface (UHAI) establishes the canonical binary IPC boundary (`core/1_simulation/src/uhai/uhai_ring_buffer.hpp`) between the Sovereign Stack daemons and external consumers (including physical hardware buses, headless CLI runners, steward mobile apps via `col-gatewayd`, and the outward-only Oasis SITL shadow simulator). All UHAI wire structs are compiled under `-Wpadded -Werror` with explicit padding and `alignas(64)` cache-line isolation to prevent false sharing across cores.

#### Binary Wire Format (C++20 Standard Layout — `uhai_ring_buffer.hpp`)
```cpp
namespace collective::uhai {

inline constexpr uint32_t UHAI_MAGIC          = 0x55484149; // "UHAI"
inline constexpr uint16_t UHAI_SCHEMA_VERSION = 1;
inline constexpr uint32_t RING_CAPACITY       = 1024;       // Power-of-2 slot count

enum QualityFlags : uint8_t {
    QUALITY_OK             = 0x00,
    QUALITY_SIMULATED      = 0x01, // Emitted outward-only by external SITL (e.g. Oasis)
    QUALITY_HARDWARE_ROOT  = 0x02, // Hardware secure element (ATECC608A) verified
    QUALITY_OUT_OF_BOUNDS  = 0x04, // Physical transducer limits breached
    QUALITY_SPOOF_SUSPECT  = 0x08  // Flagged by spatial/thermodynamic cross-check
};

// 1. TelemetrySample: Exactly 32 bytes (alignas(8)), wrapped in a 64-byte cache-line TelemetrySlot
struct alignas(8) TelemetrySample {
    uint64_t frame_count;        // 8B: Monotonic frame / tick counter
    uint64_t timestamp_ns;       // 8B: HLC / monotonic nanosecond timestamp
    float    fps;                // 4B: Sample rate or SITL frame pacing metric
    float    founder_stress;     // 4B: Aggregate physiological / homeostatic stress [0.0 - 1.0]
    uint32_t memory_slot_count;  // 4B: Active ledger / memory slot count
    uint8_t  biome_id;           // 1B: Bioregional zone identifier
    union {
        uint8_t quality_flags;   // 1B: Provenance & security validation flags (QUALITY_*)
        uint8_t quality_flag;
    };
    uint16_t reserved;           // 2B: Explicit alignment padding (zero implicit padding)
};
static_assert(sizeof(TelemetrySample) == 32, "TelemetrySample must remain exactly 32 bytes.");

struct alignas(64) TelemetrySlot {
    TelemetrySample sample;      // 32B payload
    uint8_t         pad[32];     // 32B explicit cache-line padding
};
static_assert(sizeof(TelemetrySlot) == 64, "TelemetrySlot must occupy exactly 1 cache line (64 bytes).");

// 2. SpatialIntentPayload: Exactly 264 bytes (alignas(8)), wrapped in a 320-byte (5-cache-line) SpatialIntentSlot
struct alignas(8) SpatialIntentPayload {
    uint64_t intent_id;          // 8B: Monotonic intent identifier
    uint64_t timestamp_ns;       // 8B: HLC nanosecond timestamp
    float    world_x;            // 4B: Cadastral X coordinate (meters)
    float    world_y;            // 4B: Cadastral Y elevation (meters)
    float    world_z;            // 4B: Cadastral Z coordinate (meters)
    uint8_t  intent_type;        // 1B: SpatialIntentType enum
    uint8_t  quality_flags;      // 1B: Provenance bitmask
    uint16_t reserved;           // 2B: Explicit alignment padding
    char     json_ld_payload[232]; // 232B: Null-terminated W3C JSON-LD fragment
};
static_assert(sizeof(SpatialIntentPayload) == 264, "SpatialIntentPayload must remain 264 bytes.");

struct alignas(64) SpatialIntentSlot {
    SpatialIntentPayload payload; // 264B payload
    uint8_t              pad[56]; // 56B explicit padding -> 320 bytes (5 cache lines)
};
static_assert(sizeof(SpatialIntentSlot) == 320, "SpatialIntentSlot must be 320 bytes.");

// 3. FlowContract: Exactly 64 bytes (alignas(8)), wrapped in a 64-byte FlowContractSlot
struct alignas(8) FlowContract {
    uint64_t contract_id;        // 8B: Unique bilateral FlowContract UUID
    uint64_t sender_node_id;     // 8B: Originating peer slot on Arcadia (server-01)
    uint64_t consumer_node_id;   // 8B: Counterparty peer slot on Arcadia (server-01)
    uint64_t start_logical_tick; // 8B: Starting HLC logical tick
    uint64_t end_logical_tick;   // 8B: Ending HLC logical tick (bounded at EPOCH_BOUNDARY_TICKS)
    double   rate_per_tick;      // 8B: Continuous exergy/resource flow rate per logical tick
    uint16_t resource_type;      // 2B: FlowResourceType enum (Exergy, Water, Caloric, Biomass)
    uint8_t  status;             // 1B: FlowContractStatus (Proposed, Active, Settled, Yielded)
    uint8_t  quality_flags;      // 1B: Provenance bitmask (QUALITY_SIMULATED = 0x01)
    uint32_t reserved;           // 4B: Explicit alignment padding
    uint8_t  auth_signature[8];  // 8B: Truncated capability MAC / Ed25519 tag
};
static_assert(sizeof(FlowContract) == 64, "FlowContract must remain exactly 64 bytes.");

struct alignas(64) FlowContractSlot {
    FlowContract contract;       // 64B payload (exactly 1 cache line)
};
static_assert(sizeof(FlowContractSlot) == 64, "FlowContractSlot must occupy 64 bytes.");

// 4. ActuatorCommand: Exactly 80 bytes (alignas(8)) universal hardware actuator envelope
struct alignas(8) ActuatorCommand {
    uint64_t command_id;         // 8B: Monotonic command UUID
    uint32_t device_id;          // 4B: Actuator register ID (valve, contactor, motor)
    float    target_state;       // 4B: Setpoint duty cycle (0.0 - 1.0) or target level
    uint32_t duration_ms;        // 4B: Pulse width or timeout limit (watchdog safety)
    uint32_t reserved;           // 4B: Explicit alignment padding
    uint8_t  auth_signature[56]; // 56B: Hardware capability signature envelope
};
static_assert(sizeof(ActuatorCommand) == 80, "ActuatorCommand must remain 80 bytes.");

// 5. SharedRingHeader: Exactly 192 bytes (3 cache lines, alignas(64)) SPSC lock-free ring header
struct alignas(64) SharedRingHeader {
    // Cache Line 0: Immutable Ring Metadata (64 bytes)
    uint32_t magic;              // 0x55484149 ("UHAI")
    uint16_t schema_version;     // 1
    uint16_t reserved_flags;     // 0
    uint32_t capacity;           // 1024 slots
    uint32_t slot_size;          // 64 bytes
    uint8_t  pad0[48];

    // Cache Line 1: Producer-Owned Atomic Write Cursor (64 bytes)
    alignas(64) std::atomic<uint64_t> write_index;
    uint8_t  pad1[56];

    // Cache Line 2: Consumer-Owned Atomic Read Cursor (64 bytes)
    alignas(64) std::atomic<uint64_t> read_index;
    uint8_t  pad2[56];
};
static_assert(sizeof(SharedRingHeader) == 192, "SharedRingHeader must be 192 bytes (3 cache lines).");

inline constexpr size_t TOTAL_SHM_SIZE =
    sizeof(SharedRingHeader) + (RING_CAPACITY * sizeof(TelemetrySlot));
static_assert(TOTAL_SHM_SIZE == 65728, "Total SHM mapping must be exactly 65,728 bytes.");

} // namespace collective::uhai
```

---

### 1.5. Infrastructure Service Level Objectives (SLOs) Matrix

Replacing all legacy frame budgets, the Sovereign Stack is evaluated exclusively against distributed systems and headless edge performance SLOs:

```
┌────────────────────────────────────────────────────────────────────────────────────────┐
│               SOVEREIGN STACK INFRASTRUCTURE SERVICE LEVEL OBJECTIVES (SLOs)           │
├─────────────────────────────────────────────────┬──────────────────────────────────────┤
│ Metric Designation                              │ Strict Performance Threshold         │
├─────────────────────────────────────────────────┼──────────────────────────────────────┤
│ Telemetry Ingest Latency (Hardware to L2 Queue) │ p99 ≤ 5.0 ms                         │
│ CRDT Delta Reconciliation Latency (1,000 ops)   │ p99 ≤ 10.0 ms                        │
│ Cryptographic Verification Throughput (ARM64)   │ ≥ 2,500 Ed25519 sigs/sec             │
│ BBS+ Zero-Knowledge Proof Verification Latency  │ p99 ≤ 15.0 ms                        │
│ Orchestration Logic Tick Drift (10 Hz Tick)     │ ≤ 0.5 ms maximum jitter              │
│ Local IPC Roundtrip Overhead (Unix Domain Sock) │ p99 ≤ 50 µs                          │
│ Companion Gateway (`col-gatewayd`) WSS Push     │ p99 ≤ 10.0 ms (TLS 1.3 over VLAN 30) │
│ `Arcadia (server-01)` Epoch Barrier Settlement  │ p99 ≤ 2.5 s across active quorum     │
│ Daemon Crash Recovery Time Objective (RTO)      │ ≤ 500 ms                             │
│ Memory Leak Tolerance (Continuous 72-Hour Load) │ 0 bytes leaked (Valgrind / ASan)     │
│ Total Headless Stack Memory Ceiling (8 Daemons) │ ≤ 320 MB total RSS on Linux ARM64    │
│ Layer 7 Memory Envelope                         │ ≤ 24 MB RSS                          │
│ Layer 7 Terminal Decoupling Execution Time      │ ≤ 100 ms total shutdown and wipe     │
└─────────────────────────────────────────────────┴──────────────────────────────────────┘
```

---

## 2. SDD Scenario Alignment Matrix (24 Scenarios Across 5 Developmental Acts)

The 24 canonical SDD scenarios from `docs/scenarios/*.md` represent the comprehensive operational verification matrix of the Sovereign Stack. Each scenario exercises the full 7-layer hierarchy:

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              THE 24 CANONICAL SDD SCENARIOS ACROSS THE 5 DEVELOPMENTAL ACTS                            │
├────┬─────────────────┬──────┬──────────────────────────────┬──────────────────────────────┬────────────────────────────┤
│ #  │ Scenario ID     │ Act  │ Layer 1 Physical Transducers │ Layer 3 Ledger & Valuenomics │ Layer 5/6 Intent & Policy  │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 1  │ `SCN-ALPHA-FAB` │ I    │ CNC Spindle, 3D hotend, ATECC│ Tool checkout CRDT, melt Wh  │ `FabricationBounty.jsonld`,│
│    │ Fabrication     │      │ power meter, safety interlock│ minting, depreciation debit  │ PE Machinist BBS+ VC       │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 2  │ `SCN-BETA-TRANS`│ I    │ CAN-bus BMS, BLE offline NFC,│ Arrhenius battery wear CRDT, │ Transit route request,     │
│    │ Transit Mesh    │      │ motor controller shunt meter │ seat-km mutual clearing      │ BBS+ Driver VC, zero-broker│
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 3  │ `SCN-GAMMA-KITCH`│ I   │ Induction cooktop, cold chain│ Caloric negentropy minting,  │ `MealBounty.jsonld`,       │
│    │ Commons Kitchen │      │ PT100 probe, greywater valve │ food preservation ledger     │ Food Safety Steward VC     │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 4  │ `SCN-DELTA-CHILD`│ I   │ Smart magnetic latches, zero │ Care-hour credit (1:1 with   │ `ChildcareIntent.jsonld`,  │
│    │ Childcare Pod   │      │ optical cameras, CO2 monitor │ engineering labor), guardian │ Trust Ring R1 pairing multi│
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 5  │ `SCN-EPSILON-COW`│ II  │ HVAC damper relays, power    │ Desk-hour exergy allocation, │ Guest ephemeral voucher,   │
│    │ Third Place     │      │ sub-meter, ambient light lux │ microgrid surplus sharing    │ Bioregional Commons charter│
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 6  │ `SCN-ZETA-LODGE`│ II   │ Electronic strike locks, NFC │ Thermal mass / water reserve │ WoT guest vouching,        │
│    │ Micro-Lodging   │      │ door reader, ultrasonic flow │ ledger, seasonal credit pool │ Emergency shelter pact     │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 7  │ `SCN-ETA-COPPICE`│ II  │ Kiln thermocouples, moisture │ Carbon sequestration CRDT,   │ `CordwoodBatch.jsonld`,    │
│    │ Coppice Carbon  │      │ pin probes, load cell scale  │ cordwood harvest quota ledger│ Bioregional Forestry VC    │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 8  │ `SCN-THETA-FOUND`│ II  │ Optical pyrometers, induction│ Heavy exergy escrow, solar   │ Master Pyrometallurgy VC,  │
│    │ Micro-Foundry   │      │ furnace relays, thermal dump │ surplus lock ($P > 15\text{kW}$)│ OSHA safety compliance wrap│
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 9  │ `SCN-IOTA-EPIST`│ II   │ PCR thermal cycler, optical  │ Proof-of-Reproducibility CAS,│ Blind referee review ring, │
│    │ Epistemic Mesh  │      │ spectrophotometer, centrifuge│ IPLD open science blockstore │ CERN-OHL-S open patent wrap│
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 10 │ `SCN-KAPPA-APPR`│ II   │ Machine interlock lockout,   │ Mentor/student split ledger, │ Progressive mastery BBS+ VC│
│    │ Apprenticeship  │      │ torque/speed sensor loggers  │ foundational sweat equity    │ `ApprenticeshipPact.jsonld`│
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 11 │ `SCN-LAMBDA-CULT`│ II  │ Type-1 Class-1 decibel meter,│ Cultural morale negentropy,  │ Acoustic zoning policy,    │
│    │ Cultural Commons│      │ PA system power interlock    │ commons gathering ledger     │ Municipal noise defense    │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 12 │ `SCN-MU-BIOCHAR`│ III  │ Retort pyrometer, gas flare  │ $\ge 2.0\text{kg CO}_2e/\text{kg}$ biochar,│ EPA agricultural exemption,│
│    │ Biochar Sink    │      │ thermocouple, weighbridge    │ soil carbon addition credit  │ Carbon removal cert schema │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 13 │ `SCN-NU-TEXTILE`│ III  │ CNC laser fabric cutter, loom│ Mending life-extension ledger│ Circular textile steward VC│
│    │ Circular Textile│      │ servo encoder, shredder motor│ recycled fiber weight escrow │ Thrift salvage bill of sale│
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 14 │ `SCN-XI-ARCH`   │ III  │ CNC timber joinery router,   │ Embodied carbon ledger,      │ Algorithmic frame schema,  │
│    │ Timber Framing  │      │ beam deflection strain gauges│ structural safety factor 1.5x│ PE structural attestation  │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 15 │ `SCN-OMICRON-KIN`│ III │ Air-gapped NFC fobs, biometric│ EigenTrust graph CRDT, Trust │ Concentric Trust Rings     │
│    │ Kinship Trust   │      │ zero-knowledge hardware token│ Ring threshold escrow        │ $R_0 \to R_6$, Sybil slash │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 16 │ `SCN-PI-GUILD`  │ III  │ Guild hall door strikers,    │ Multi-sig guild treasury,    │ Sovereign Guild Charter,   │
│    │ Sovereign Guild │      │ tool locker telemetry access │ mutual defense legal escrow  │ EOR / LLC liability shield │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 17 │ `SCN-SIGMA-MAINT`│ IV  │ Accelerometers, dust sensors,│ Dynamic bounty scaling CRDT, │ Automated chore allocator, │
│    │ Maintenance     │      │ vibration FFT transducers    │ anti-burnout duty balancer   │ max 25% burden cap per user│
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 18 │ `SCN-TAU-GENESIS`│ IV  │ ATECC608A key generation HAL,│ Soulbound genesis credit,    │ In-person Ed25519 ceremony,│
│    │ Steward Genesis │      │ hardware entropy source TRNG │ physical tool seed escrow    │ 501(d) club member roll    │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 19 │ `SCN-PHI-ELDER` │ IV  │ 60 GHz mmWave fall radar, zero│ Wisdom archive IPLD CAS,     │ Advance medical directive, │
│    │ Elder Commons   │      │ cameras, environmental temps │ eldercare mutual credit fund │ Probate avoidance trust    │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 20 │ `SCN-CHI-PARAMED`│ IV  │ AED battery monitor, vaccine │ Emergency supply reservation,│ Priority mesh preemption,  │
│    │ Paramedic Mesh  │      │ fridge PT100 cold chain probe│ mutual aid triage ledger     │ Paramedic BBS+ credential  │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 21 │ `SCN-PSI-MEDIAT`│ IV  │ Mediation room privacy lock, │ Restitution work bounty CRDT,│ Restorative Justice Treaty,│
│    │ Restorative Just│      │ audio recording scrambler    │ debt reconciliation ledger   │ Grievance transcript shred │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 22 │ `SCN-RHO-ADVERS`│ V   │ Chassis tamper microswitches,│ Byzantine fault detector,    │ Threshold emergency re-key,│
│    │ Adversarial Def │      │ optical enclosure photodiode │ tamper event proof ledger    │ Zeroization within 1 ms    │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 23 │ `SCN-UPSILON-SHK`│ V   │ Motorized microgrid breaker, │ 72-hour emergency water/kWh  │ Disaster governance policy,│
│    │ Ecological Shock│      │ inverter black-start contacts│ survival allocation ledger   │ FEMA emergency liaison     │
├────┼─────────────────┼──────┼──────────────────────────────┼──────────────────────────────┼────────────────────────────┤
│ 24 │ `SCN-OMEGA-DECP`│ V   │ ATS permanent physical grid  │ 100% pure negentropy currency│ Permanent corporate dissol,│
│    │ Terminal Decoup │      │ disconnect interlock contact │ Viability $\mathcal{S}\ge1.25$│ Shred all Layer 7 data     │
└────┴─────────────────┴──────┴──────────────────────────────┴──────────────────────────────┴────────────────────────────┘
```

---

## 3. Autonomous 8-Daemon POSIX Architecture & IPC Bus

### 3.1. Daemon Process Topology
Each layer of the Sovereign Stack—plus the dedicated Layer 6 Semantic HTTP/WebSocket Gateway (`col-gatewayd`)—runs as an independent, isolated POSIX process supervised by `collective-supervisor`:

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              THE AUTONOMOUS 8-DAEMON SYSTEM ARCHITECTURE                               │
├───────┬───────────────────────────┬──────────────────────────────────────┬─────────────────────────────┤
│ LAYER │ DAEMON EXECUTABLE         │ COMPUTATIONAL LOOP & RUNTIME ENGINE  │ DEDICATED STORAGE BACKEND   │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **GW**│ `col-gatewayd`            │ TLS 1.3 / WSS 10 Hz Multiplexor      │ Ephemeral `/dev/shm` pairing│
│       │ (L6 Semantic Gateway:8443)│ Exclusive L6 write; L2-L5/L7 RO hood │ + Dedicated SPSC readers    │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L7**│ `col-adversaryd`          │ Priority-Queue Discrete Event Sim    │ Isolated SQLite (SQLCipher) │
│       │ (Cooperative Carapace/DES)│ (DES); utilities, zoning, migrations │ (Zero cross-DB FKs; shred)  │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L6**│ `col-commonsd`            │ W3C JSON-LD compiler; W3C PROV-O     │ Embedded RDF Quad Store     │
│       │ (Semantic Intent & Agora) │ knowledge graph; double-auction book │ (Oxigraph) / DuckDB graph   │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L5**│ `col-kmsd`                │ W3C DID resolver; BBS+ ZKP engine;   │ Encrypted SQLCipher vault   │
│       │ (Governance & Web of Trust│ FROST threshold; R6->R1 onboarding   │ + Sparse Merkle Tree (SMT)  │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L4**│ `col-execd`               │ Deterministic 10 Hz BPMN 2.0 VM;     │ Durable Saga WAL journal +  │
│       │ (Orchestrator & Execution)│ Blind Wasmtime; Intent U-Turn yield  │ Task state RocksDB store    │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L3**│ `col-storaged`            │ Automerge/Yrs CRDT engine; HLC causal│ Content-Addressed CAS (IPLD)│
│       │ (Thermodynamic Ledger)    │ ordering; PoTW; Epoch Merkle Barrier │ + RocksDB CRDT append-log   │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L2**│ `col-meshd`               │ Reticulum Network Stack (RNS) / LoRa │ Persistent DTN spool ring + │
│       │ (Digital Twin & P2P Mesh) │ async reactor; Arcadia delta relay   │ LMDB peerstore routing table│
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L1**│ `col-telemetryd`          │ Linux epoll / RTOS HAL; RS485 Modbus,│ Flash NVRAM ring buffer +   │
│       │ (Physical Reality & HAL)  │ CAN-bus, hardware watchdog, ATECC608A│ ATECC608A secure EEPROM     │
└───────┴───────────────────────────┴──────────────────────────────────────┴─────────────────────────────┘
```

### 3.2. Inter-Process Communication (IPC) Socket Architecture & Strict Intent U-Turn
All inter-daemon communication occurs over dedicated Unix Domain Sockets located in `/run/collective/ipc/` and isolated 1:1 Single-Producer Single-Consumer (`LockFreeSpscRing`) shared-memory rings mapped with the 192-byte `SharedRingHeader` (`RING_CAPACITY = 1024`, `TOTAL_SHM_SIZE = 65728` for 64-byte slots):

1. **Bilateral Adjacency Sockets (Strict Adjacency — Zero `l4_l7.sock`):**
   - `/run/collective/ipc/l1_l2.sock` (`col-telemetryd` $\longleftrightarrow$ `col-meshd`)
   - `/run/collective/ipc/l2_l3.sock` (`col-meshd` $\longleftrightarrow$ `col-storaged`)
   - `/run/collective/ipc/l3_l4.sock` (`col-storaged` $\longleftrightarrow$ `col-execd`)
   - `/run/collective/ipc/l4_l5.sock` (`col-execd` $\longleftrightarrow$ `col-kmsd` — includes `YIELD_UPWARD_L5` U-Turn channel)
   - `/run/collective/ipc/l5_l6.sock` (`col-kmsd` $\longleftrightarrow$ `col-commonsd`)
   - `/run/collective/ipc/l6_l7.sock` (`col-commonsd` $\longleftrightarrow$ `col-adversaryd`)
   - `/run/collective/ipc/gw_l6.sock` (`col-gatewayd` $\longrightarrow$ `col-commonsd` — exclusive write path for `POST /api/v1/l6/intent`)
   - *Topological Prohibition:* There is **NO `l4_l7.sock`**. `col-execd` executes inside a restricted Linux network/AppArmor sandbox (`PrivateNetwork=yes`) that aborts on any socket call outside `l3_l4.sock` and `l4_l5.sock`. Furthermore, `col-adversaryd` (isolated in DMZ VLAN 40) connects **only** to `/run/collective/ipc/l6_l7.sock` and never accesses `bus.sock` or `/dev/shm/uhai_*` directly; any Layer 7 event (such as `MigrationWaveArrivalEvent`) propagates strictly inward across adjacent layers (`L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI`).
2. **Dedicated 1:1 SPSC Shared Memory Rings (Zero Reader Contention):**
   - `/dev/shm/uhai_telemetry` (`TOTAL_SHM_SIZE = 65728`, bridging Layer 2 `col-meshd` and Layer 1b SITL / Oasis with `QUALITY_SIMULATED = 0x01`).
   - `/dev/shm/uhai_telemetry_execd` and `/dev/shm/uhai_flow_execd` (consumed exclusively by `col-execd`).
   - `/dev/shm/uhai_telemetry_gateway` and `/dev/shm/uhai_flow_gateway` (consumed exclusively by `col-gatewayd` on port `8443`).
3. **Broadcast Event Bus (`col-busd`):** `/run/collective/ipc/bus.sock` broadcasts non-destructive read-only copies of internal VLAN 10/20 domain events (telemetry anomalies, block confirmations, epoch barrier alerts, scenario completions) to `col-gatewayd` and subscribed internal daemons (excluding DMZ VLAN 40 `col-adversaryd`).
4. **Buffer Management & Backpressure:** Every socket sets `SO_SNDBUF = 262144` and `SO_RCVBUF = 262144` (256 KB). All sockets operate in non-blocking mode (`O_NONBLOCK`). Telemetry drops non-critical frames under backpressure; critical actuator commands enforce circuit breakers.
5. **Permissions:** Socket permissions are enforced strictly via POSIX groups (`collective-ipc`, `chmod 0660`). Layer 6 and `col-gatewayd` have zero direct write access to Layer 1 (`col-telemetryd`) or Layer 4 (`col-execd`).

---

## 4. Epic Summary, Story Points & Sprint Allocations

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       EPIC SUMMARY & SPRINT ALLOCATION MATRIX (V4.0)                                   │
├────────┬────────────────────────────────────────────────────────────────────────┬─────────────┬───────────────┬────────┤
│ Epic   │ Epic Title                                                             │ Priority    │ Stories Count │ Points │
├────────┼────────────────────────────────────────────────────────────────────────┼─────────────┼───────────────┼────────┤
│ **E1** │ Layer 1: Physical Reality, Energy Harvesting & Hardware Root-of-Trust  │ P0 Critical │ 5 Stories     │ 35 SP  │
│ **E2** │ Layer 2: Delay-Tolerant Mesh, Reticulum Transport & Telemetry Ingress  │ P0 Critical │ 5 Stories     │ 34 SP  │
│ **E3** │ Layer 3: Thermodynamic Ledger, CRDT Replication & Valuenomics Minting  │ P0 Critical │ 6 Stories     │ 44 SP  │
│ **E4** │ Layer 4: Deterministic BPMN 2.0 Orchestrator & Ecological Floors       │ P0 Critical │ 5 Stories     │ 36 SP  │
│ **E5** │ Layer 5: Polycentric Governance, Pairwise DIDs & Trust Ring Onboarding │ P0 Critical │ 7 Stories     │ 50 SP  │
│ **E6** │ Layer 6: Semantic Intent, Agora Commons & Companion Gateway (`:8443`)  │ P0 Critical │ 6 Stories     │ 41 SP  │
│ **E7** │ Layer 7: Legacy Proxy, Cooperative Legal Carapace & Migration Waves    │ P0 Critical │ 7 Stories     │ 53 SP  │
│ **E8** │ Cross-Stack Headless Verification, Chaos Engineering & Quality Gates   │ P0 Critical │ 6 Stories     │ 41 SP  │
│ **E9** │ Shared Virtual Landscape (`Arcadia server-01`) & Epoch Synchronization │ P0 Critical │ 3 Stories     │ 24 SP  │
├────────┴────────────────────────────────────────────────────────────────────────┴─────────────┼───────────────┼────────┤
│ TOTAL (9 Epics, 50 Fully Articulated Stories)                                                 │ 50 Stories    │ 358 SP │
└───────────────────────────────────────────────────────────────────────────────────────────────┴───────────────┴────────┘
```

### Sprint Delivery Pacing (Sprints 1–10 Completed Baseline & Dual Sprints 11–14 Horizon Roadmap)

#### Phase I: Core Sovereign Stack & Tactical "Flesh & Fabrication" Baseline (Sprints 1–10 — COMPLETED)
- **Sprint 1 (52 SP — Completed):** Epics 1 & 2 Core Daemons (`col-telemetryd`, `col-meshd`), UHAI IPC wire protocol, ATECC608A HAL, Reticulum LoRa mesh.
- **Sprint 2 (54 SP — Completed):** Epic 3 Core Ledger (`col-storaged`), RocksDB append-log, Automerge/Yrs CRDT, PoTW negentropy minting, Asymptotic ERC Firewall.
- **Sprint 3 (52 SP — Completed):** Epic 4 & 5 Deterministic Execution (`col-execd`, `col-kmsd`), 10 Hz BPMN VM, Wasmtime fuel sandbox, W3C DIDs, BBS+ ZKP credentials.
- **Sprint 4 (50 SP — Completed):** Epic 5 Federation & Epic 6 Agora (`col-kmsd`, `col-commonsd`), Bioregional Path-Vector Protocol (BPVP), Bilateral Clearing Channels, JSON-LD intent compiler, Oxigraph RDF.
- **Sprint 5 (52 SP — Completed):** Epic 7 Cooperative Legal Carapace (`col-adversaryd`), Discrete Event Sim, Tripartite Legal Chassis, Mock Fiat APIs, Systemic Legal Blinding, plus Dual Sprint 5 10 Hz POSIX Lock-Free UHAI IPC integration.
- **Sprint 6 (50 SP — Completed):** Epic 8 Full-Stack Verification & Quality Gates (G1–G6), Scenario Rho fuzzing, 72-hour partition resilience, Scenario Omega terminal decoupling certification, and `BOOK_TEAM_DRIFT_REPORT.md` alignment.
- **Dual Sprints 7–10 (Tactical "Flesh & Fabrication" Overrides — Completed):** Dynamic infrastructure work-order ingestion in `col-execd`, thermal & somatic telemetry grounding in `col-telemetryd`/`col-storaged`, Temporal `FlowContract` SPSC ring buffers (`/uhai_flow`), and initial Companion App V2.0 Gateway (`col-gatewayd` on port 8443).

#### Phase II: Horizon & Playable Environment Roadmap (Dual Sprints 11–14 — ACTIVE)
- **Dual Sprint 11 (8 SP Stack + Oasis Tactile UX — Tactile Mastery, Time Control & Layer 6 Gateway Hardening):**
  - *Stack Deliverables:* **User Story 6.6 (`SS-EP6-006`)** — `col-gatewayd` Read-Only Cybernetic Hood (`403`/`405` on L2/L3/L4/L5/L7 writes), exclusive Layer 6 intent ingress (`POST /api/v1/l6/intent`), Strict Intent U-Turn enforcement, and UHAI `SpatialIntentPayload` & `TimeScaleState` synchronization with `col-execd`.
- **Dual Sprint 12 (Homeostatic Grounding — The Chill Loop & Shelter/Kitchen Telemetry):**
  - *Stack Deliverables:* Microgrid enclosed-room thermal telemetry ($>18^\circ\text{C}$ shelter boundary verification in `SCN-ZETA-LODGE`) and Commons Kitchen caloric/hydration reserve accounting (`SCN-GAMMA-KITCH`) across `col-telemetryd`, `col-storaged`, and `col-commonsd`.
- **Dual Sprint 13 (16 SP Stack — Migration Waves & Legacy NPC Trust Ring Assimilation):**
  - *Stack Deliverables:* **User Story 7.7 (`SS-EP7-007`)** — Layer 7 Demographic Shock & Legacy Migration Wave Generator in `col-adversaryd`; **User Story 5.7 (`SS-EP5-007`)** — Progressive Migrant Onboarding & Trust Ring ($R_6 \to R_1$) Assimilation Protocol in `col-kmsd`.
- **Dual Sprint 14 (24 SP Stack — `Arcadia (server-01)` Shared Virtual Landscape & Mandatory Milestone Epoch Sync):**
  - *Stack Deliverables:* **Epic 9 (`SS-EP9`)** — **User Story 9.1 (`SS-EP9-001`)** Voluntary Shared Landscape Delta Relay (`col-meshd` & `col-storaged`), **User Story 9.2 (`SS-EP9-002`)** Temporal Flow Contract HLC Pacing & Mandatory Milestone Barrier (`col-execd` & `col-storaged`), and **User Story 9.3 (`SS-EP9-003`)** Asynchronous Epoch Merkle Reconciliation & Straggler Quorum Grace Protocol.

---

## 5. Epic 1: Layer 1 — Physical Reality, Energy Harvesting & Hardware Root-of-Trust (`col-telemetryd`)

### User Story 1.1: Multi-Tier Hardware Root-of-Trust & Chassis Tamper Zeroization Daemon
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-001  
**Points:** 8 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-telemetryd` to initialize its cryptographic identity directly from a hardware secure element (Microchip ATECC608A) and immediately zeroize ephemeral keys upon physical chassis breach,  
**Benefit:** So that physical capture or theft of an off-grid node by hostile actors cannot compromise the bioregional Web of Trust or forge telemetry proofs.

#### Technical Specifications & Architecture
- **Daemon:** `col-telemetryd` running with Linux capabilities `CAP_SYS_RAWIO` and real-time scheduling priority `SCHED_FIFO` (priority 50).
- **Hardware Integration:** Communicates with Microchip ATECC608A over I2C (`/dev/i2c-1`) using the CryptoAuthLib HAL. Primary node DID seed is burned into Slot 0 (locked, non-exportable).
- **Tamper Circuit:** Chassis enclosure microswitch wired to an interrupt-driven GPIO line (`/dev/gpiochip0`, pin 17).
- **Zeroization Pipeline:** On interrupt trigger, kernel ISR signals daemon to issue atomic hardware command zeroizing volatile keys in $\le 12\text{ }\mu\text{s}$, writes an immutable tamper flag to NVRAM, and sends `SIGTERM` to `col-kmsd`.

```protobuf
syntax = "proto3";
package collective.l1;

message ChassisStatus {
  bool enclosure_locked = 1;
  bool tamper_circuit_closed = 2;
  uint32_t boot_counter = 3;
  bytes root_public_key = 4;
  uint64_t uptime_seconds = 5;
}

message ZeroizeKeysResponse {
  bool zeroization_confirmed = 1;
  uint64_t timestamp_hlc = 2;
  bytes hardware_attestation = 3;
}
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Physical enclosure tamper triggers immediate zeroization
  Given an operational edge node with locked ATECC608A Slot 0
  When the physical chassis tamper switch is tripped (circuit opened)
  Then col-telemetryd executes key zeroization within <= 12 microseconds
  And writes an immutable tamper record to non-volatile EEPROM
  And all subsequent cryptographic operations return EPERM until in-person re-keying ceremony.
```

#### Technical Tasks
- [ ] Implement ATECC608A I2C driver integration using Microchip CryptoAuthLib.
- [ ] Configure GPIO edge interrupt handler in Linux sysfs/libgpiod with debouncing ($\le 2\text{ ms}$).
- [ ] Implement atomic zeroization routine and panic state transition.
- [ ] Add hardware test fixture asserting sub-millisecond zeroization upon signal trip.

---

### User Story 1.2: Asynchronous RS-485 Modbus RTU Energy Telemetry & SocketCAN BMS Drivers
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-002  
**Points:** 8 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As a Microgrid Electrical Steward,  
**Goal:** I want `col-telemetryd` to poll solar charge controllers via RS-485 Modbus RTU and battery management systems via SocketCAN asynchronously,  
**Benefit:** So that real-time voltage, current, state-of-charge, and temperature telemetry are sampled at 10 Hz without blocking system execution.

#### Technical Specifications & Architecture
- **Reactor:** Linux `epoll` reactor monitoring non-blocking serial file descriptors (`/dev/ttyUSB0`) and SocketCAN network interface (`can0`).
- **Protocols Supported:**
  - Modbus RTU (libmodbus asynchronous wrapper) polling Victron SmartSolar MPPT and Schneider Conext inverters.
  - SocketCAN (J1939 CAN 2.0B) polling Daly / Seplos LiFePO4 battery management systems at 250 kbps.
- **Telemetry Packing:** Ingests raw registers, applies scaling factors, validates physical boundaries, and packs into 32-byte `TelemetrySample` structs (wrapped in 64-byte cache-line-isolated `TelemetrySlot` ring entries).

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: High-frequency Modbus and CAN telemetry sampling
  Given a 48V LiFePO4 battery bank and 5 kW solar array connected via CAN and RS-485
  When col-telemetryd executes its 10 Hz epoll loop for 60 seconds
  Then it records exactly 600 samples per register without dropped frames
  And p99 telemetry ingest latency from serial read to IPC emission is <= 5.0 ms
  And corrupted CRC checksum frames are quarantined with QUALITY_OUT_OF_BOUNDS.
```

#### Technical Tasks
- [ ] Implement asynchronous epoll reactor for RS-485 serial ports with custom timeout timers.
- [ ] Implement SocketCAN frame reader and J1939 PGN parser for battery pack metrics.
- [ ] Add CRC16 and CAN frame validation tests with simulated bit errors.

---

### User Story 1.3: Hardware Actuator Drivers & Safety Interlocks for the 24 Canonical Scenarios
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-003  
**Points:** 8 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As an Edge Infrastructure Engineer,  
**Goal:** I want `col-telemetryd` to implement dedicated hardware actuator drivers and fail-safe safety interlocks for all 24 canonical SDD scenarios,  
**Benefit:** So that physical equipment (CNC spindles, kiln valves, microgrid breakers, medical refrigeration) operates safely and shuts down immediately upon sensor anomaly.

#### Technical Specifications & Architecture
- **Hardware Drivers Configured:**
  - `SCN-ALPHA-FAB` (CNC Spindle Relays, Stepper Enable Pins)
  - `SCN-GAMMA-KITCH` (Induction Cooktop Power Limiters, Greywater Valves)
  - `SCN-DELTA-CHILD` (Magnetic Door Strikes, Zero Camera Hardware)
  - `SCN-THETA-FOUNDRY` (Induction Furnace Contactors, Emergency Thermal Dump Relays $\le 50\text{ ms}$)
  - `SCN-ETA-COPPICE` & `SCN-MU-BIOCHAR` (Pyrolysis Retort Thermocouple Valves, Gas Flares)
  - `SCN-CHI-PARAMEDIC` (Cold Chain Vaccine Compressor Contactors, AED Battery Monitors)
  - `SCN-UPSILON-SHOCK` & `SCN-OMEGA-DECOUPLING` (Motorized ATS Transfer Switches, Grid Contactor Disconnects)
- **Hard Safety Interlocks:** Independent firmware watchdog and thermal cutoff relays wired in series with software logic. If thermal threshold is breached, physical power is cut regardless of software state.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Micro-foundry thermal dump triggered on overheating
  Given col-telemetryd controlling induction furnace contactors for SCN-THETA-FOUNDRY
  When optical pyrometer reads temperature exceeding 1,450 deg C
  Then hardware safety interlock fires within <= 50 ms cutting main contactor power
  And emits an emergency ActuationInterlockTriggered event to Layer 2
  And requires explicit steward physical reset before contactor can re-energize.
```

#### Technical Tasks
- [ ] Implement GPIO relay driver matrix using Linux libgpiod with pulse-width safety clamps.
- [ ] Implement automated scenario actuator driver profiles for all 24 canonical SDD scenarios.
- [ ] Add hardware loopback testing verifying contactor trip latency $\le 50\text{ ms}$.

---

### User Story 1.4: Real-Time Thermodynamic Exergy & Thermal Transducer Telemetry Pipeline
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-004  
**Points:** 5 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As a Cybernetics Theorist,  
**Goal:** I want `col-telemetryd` to compute real-time exergy flows ($\Phi_{in}, \Phi_{out}$) from raw physical sensor transducers,  
**Benefit:** So that the thermodynamic quality of energy (useful work potential, second law efficiency) is passed directly to the ledger rather than raw scalar kilowatt-hours.

#### Technical Specifications & Architecture
- **Transducers:** PT100/PT1000 RTD temperature sensors (SPI interface via MAX31865), mass flow meters (pulse counter GPIO), and DC/AC power meters.
- **Exergy Formula:** For thermal fluid streams, computes flow exergy:
  $$ex = \dot{m} \cdot \left[ (h - h_0) - T_0 (s - s_0) \right]$$
  where $T_0$ is ambient reference temperature ($293.15\text{ K}$), $h$ is enthalpy, and $s$ is entropy.
- **Wire Tagging:** Ingests samples, sets `unit_type = WATTS`, and populates `quality_flags = QUALITY_HARDWARE_ROOT`.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Solar thermal exergy computation
  Given a water flow rate of 10 L/min entering at 20 deg C and exiting at 60 deg C
  When col-telemetryd evaluates the exergy equation with ambient T0 = 20 deg C
  Then it calculates net thermal exergy rate of 342.1 Watts (versus 27.9 kW raw enthalpy)
  And packages the result into a verified TelemetrySample with QUALITY_HARDWARE_ROOT.
```

#### Technical Tasks
- [ ] Implement MAX31865 RTD SPI driver with fault detection (open/short circuit).
- [ ] Implement thermodynamic exergy library in C++20 with polynomial water steam tables.
- [ ] Add unit test verifying Carnot and flow exergy limits across temperature spans.

---

### User Story 1.5: Hardware Watchdog Supervision, Brownout Recovery & Strict Physical Airgap Interlock
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-005  
**Points:** 6 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As a Quality Engineer,  
**Goal:** I want `col-telemetryd` to supervise an external hardware watchdog timer, handle brownouts gracefully, and enforce the physical airgap barring simulated telemetry from edge relays,  
**Benefit:** So that edge nodes recover autonomously from power starvation and synthetic simulator commands can never actuate physical machinery.

#### Technical Specifications & Architecture
- **Watchdog:** Serviced via `/dev/watchdog` (Linux watchdog API) with an 8-second timeout. If the 10 Hz reactor thread stalls for $> 6$ seconds, the hardware resets the SoC.
- **Brownout Recovery:** Monitors DC bus voltage. If $V_{bus} < 44.0\text{ V}$ (for 48V LiFePO4), initiates orderly shutdown of non-critical daemons, leaves L1/L2 in ultra-low-power sleep ($< 500\text{ mW}$), and auto-boots when $V_{bus} > 51.2\text{ V}$.
- **Strict Airgap Interlock:** Ingress filter asserts that incoming `ActuatorCommand` packets must originate from Layer 4 execution keys and target physical relays only when accompanying telemetry carries `QUALITY_HARDWARE_ROOT`. Packets flagged `QUALITY_SIMULATED` are physically dropped at socket ingress.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Simulated telemetry command rejected at hardware airgap
  Given an external SITL simulator emitting ActuatorCommand targeting physical relay 0x05
  When the incoming packet payload carries quality_flags = QUALITY_SIMULATED (0x01)
  Then col-telemetryd drops the command immediately with EACCES
  And logs an AirgapSecurityBreachAttemptEvent to the system journal
  And physical relay 0x05 remains completely unenergized.
```

#### Technical Tasks
- [ ] Implement `/dev/watchdog` keep-alive pinger in the reactor event loop.
- [ ] Implement brownout state machine with hysteretic voltage recovery thresholds.
- [ ] Implement strict airgap validation filter rejecting `QUALITY_SIMULATED` packets.

---

## 6. Epic 2: Layer 2 — Delay-Tolerant Mesh, Reticulum Transport & Telemetry Ingress (`col-meshd`)

### User Story 2.1: Reticulum Cryptographic Mesh Routing Engine & 128-Bit Addressing
**Epic:** Epic 2 (Layer 2: Delay-Tolerant Mesh)  
**Story ID:** SS-EP2-001  
**Points:** 8 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As a Mesh Infrastructure Steward,  
**Goal:** I want `col-meshd` to route telemetry and governance envelopes over the Reticulum Network Stack (RNS) using 128-bit truncated SHA-256 destination hashes,  
**Benefit:** So that nodes communicate securely over untrusted ad-hoc radio links without central IP address assignment, domain name servers, or telecommunication providers.

#### Technical Specifications & Architecture
- **Routing Engine:** Embedded C++20 / Rust implementation of the Reticulum Protocol.
- **Addressing:** Destination addresses derived as $\mathrm{Hash}_{128}(K_{\mathrm{identity}} \parallel \text{"collective.node"})$.
- **Packet Structure:** 500-byte maximum MTU optimized for low-bandwidth LoRa links. Packets encrypted using ephemeral Curve25519 ECDH + AES-256-CBC with HMAC-SHA256 authentication.
- **Routing Table:** Next-hop distance-vector table maintaining hop count, roundtrip latency, and SNR per destination.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Ad-hoc packet routing across 3-hop mesh
  Given Node A, Node B, and Node C in a linear mesh topology (A <-> B <-> C)
  When Node A sends a 128-byte encrypted state update to Node C's Reticulum destination hash
  Then Node B forwards the packet without inspecting payload contents
  And Node C decrypts and authenticates the packet within <= 250 ms
  And zero IP routing or DNS lookups are performed.
```

#### Technical Tasks
- [ ] Implement Reticulum packet header serializer and deserializer.
- [ ] Implement Curve25519 ECDH ephemeral key exchange and AES-256-CBC cipher pipeline.
- [ ] Add multi-hop routing test harness simulating packet forwarding across 5 nodes.

---

### User Story 2.2: Multi-Bearer Transport Manager with Automatic LoRa / 802.11s / Ethernet Failover
**Epic:** Epic 2 (Layer 2: Delay-Tolerant Mesh)  
**Story ID:** SS-EP2-002  
**Points:** 7 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As an Edge Infrastructure Engineer,  
**Goal:** I want `col-meshd` to multiplex traffic seamlessly across Semtech SX1262 LoRa, 802.11s Wi-Fi mesh, and wired dark fiber,  
**Benefit:** So that nodes utilize high-bandwidth links when available and automatically fall back to long-range radio when fiber cuts occur.

#### Technical Specifications & Architecture
- **Physical Bearers:**
  - Semtech SX1262 SPI LoRa transceiver (915 MHz US / 868 MHz EU, spreading factor SF7–SF10, bandwidth 125 kHz).
  - Linux `wlan0` configured in 802.11s mesh mode with mesh ID `collective-mesh`.
  - Wired Gigabit Ethernet (`eth0` / dark fiber).
- **Metric Controller:** Evaluates link quality every 1 second based on Packet Error Rate (PER) and RSSI. Routes bulk data over 802.11s/Ethernet; duplicates critical alarms across LoRa and Ethernet simultaneously.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Automatic bearer failover during fiber disconnection
  Given an active link streaming state updates over Gigabit Ethernet
  When the Ethernet cable is physically severed
  Then col-meshd detects carrier loss within <= 100 ms
  And shifts routing to Semtech SX1262 LoRa without dropping inflight transactions
  And rate-limits subsequent frame emissions to match LoRa link capacity (max 5.4 kbps).
```

#### Technical Tasks
- [ ] Implement SX1262 SPI driver with CAD (Channel Activity Detection).
- [ ] Implement Linux netlink socket monitor tracking 802.11s and Ethernet link states.
- [ ] Add failover test simulating sudden cable detachment under active load.

---

### User Story 2.3: Delay-Tolerant Store-and-Forward Packet Queue (50 MB Ring) & LMDB Peerstore
**Epic:** Epic 2 (Layer 2: Delay-Tolerant Mesh)  
**Story ID:** SS-EP2-003  
**Points:** 6 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As an Off-Grid Node Steward,  
**Goal:** I want `col-meshd` to store outbound packets in a persistent 50 MB spool when peers are unreachable,  
**Benefit:** So that mobile data mules or intermittent radio connections guarantee eventual delivery of state without packet loss.

#### Technical Specifications & Architecture
- **Spool Storage:** Persistent ring buffer at `/var/lib/collective/l2/dtn_spool.dat` capped strictly at 50 MB. Oldest non-critical packets are evicted with an alert if capacity is exceeded.
- **Peerstore:** Embedded Lightning Memory-Mapped Database (LMDB) storing peer public keys, Reticulum destination hashes, last seen timestamps, and link performance metrics.
- **Delivery Receipts:** Implements Custody Transfer Protocol where receiving nodes send cryptographically signed receipts before the sender removes the packet from its spool.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Delay-Tolerant Networking store-and-forward delivery
  Given Node A attempting to send 5 MB of telemetry to Node B while Node B is offline
  When Node A enqueues packets into col-meshd
  Then packets are persisted to the 50 MB DTN spool in LMDB
  When Node B reconnects 12 hours later
  Then Node A automatically flushes the spooled packets in causal order
  And both nodes confirm delivery with signed receipts.
```

#### Technical Tasks
- [ ] Implement bounded 50 MB memory-mapped spool ring buffer.
- [ ] Integrate LMDB key-value store for Reticulum peer metadata.
- [ ] Add unit test verifying FIFO spool persistence across sudden daemon restarts.

---

### User Story 2.4: UHAI Asynchronous SITL Adapter & Lock-Free IPC Ring Buffer
**Epic:** Epic 2 (Layer 2: Delay-Tolerant Mesh)  
**Story ID:** SS-EP2-004  
**Points:** 7 SP | **Sprint:** 1 | **Priority:** P0 Critical  
**Persona:** As a Systems Simulation Engineer,  
**Goal:** I want `col-meshd` to expose a lock-free shared-memory ring buffer and Unix Domain Socket for external Software-In-The-Loop (SITL) simulator telemetry ingress,  
**Benefit:** So that external simulators (e.g. Oasis) stream high-throughput synthetic sensor telemetry with sub-50 microsecond roundtrip latency.

#### Technical Specifications & Architecture
- **Ingress Channels:**
  - Shared memory file `/dev/shm/uhai_telemetry` (`TOTAL_SHM_SIZE = 65728`) mapped with the 192-byte `SharedRingHeader` (`RING_CAPACITY = 1024`) and 64-byte `TelemetrySlot` entries for zero-copy read/write (`core/1_simulation/src/uhai/uhai_ring_buffer.hpp`).
  - Unix Domain Socket `/run/collective/ipc/sitl.sock` (`SOCK_SEQPACKET`).
- **Telemetry Validation:** Checks 32-byte `TelemetrySample` (`alignas(8)`) and 64-byte `TelemetrySlot` (`alignas(64)`) layout invariants (`-Wpadded -Werror`). Enforces `quality_flags = QUALITY_SIMULATED` on all incoming SITL frames. Forwards validated frames to Layer 3 via `/run/collective/ipc/l2_l3.sock`.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: SITL high-frequency synthetic telemetry stream
  Given the Oasis engine connected to /run/collective/ipc/sitl.sock
  When Oasis streams 10,000 synthetic TelemetrySample structs at 1,000 samples/sec
  Then col-meshd ingests all samples with p99 latency <= 50 microseconds
  And all samples are tagged with QUALITY_SIMULATED before forwarding to Layer 3
  And zero memory allocations or buffer overflows occur.
```

#### Technical Tasks
- [ ] Implement lock-free circular ring buffer with atomic head/tail pointers in C++20.
- [ ] Implement `SOCK_SEQPACKET` socket server handling SITL connections.
- [ ] Add latency benchmark test asserting $< 50\text{ }\mu\text{s}$ roundtrip on ARM64.

---

### User Story 2.5: Telecom Dark Fiber Ingress, Carrier Intertie & Sub-Mesh Bridging
**Epic:** Epic 2 (Layer 2: Delay-Tolerant Mesh)  
**Story ID:** SS-EP2-005  
**Points:** 6 SP | **Sprint:** 1 | **Priority:** P1 High  
**Persona:** As a Bioregional Network Architect,  
**Goal:** I want `col-meshd` to interface with telecom dark fiber pairs and commercial transit backbones through encrypted WireGuard tunnels,  
**Benefit:** So that distant valley sub-meshes bridge together over leased legacy fiber without exposing internal routing topology.

#### Technical Specifications & Architecture
- **Carrier Interface:** Integrates with Linux WireGuard kernel module (`wg0`) running ChaCha20-Poly1305.
- **Traffic Shaping:** Enforces constant-bitrate (CBR) packet padding and dummy traffic generation. External observers analyzing fiber traffic see only constant-size encrypted frames at constant intervals, defeating traffic analysis attacks.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Dark fiber bridge connects two disconnected mountain valleys
  Given Valley A mesh and Valley B mesh separated by 50 km of legacy dark fiber
  When col-meshd establishes a WireGuard carrier tunnel over the dark fiber
  Then both valleys route Reticulum packets as a unified Layer 2 mesh
  And dummy traffic padding obscures bursty communication patterns.
```

#### Technical Tasks
- [ ] Implement WireGuard control plane manager in C++.
- [ ] Implement constant-bitrate traffic padding generator for carrier links.
- [ ] Add integration test verifying sub-mesh bridging across emulated WAN latency.

---

## 7. Epic 3: Layer 3 — Thermodynamic Ledger, CRDT Replication & Valuenomics Minting (`col-storaged`)

### User Story 3.1: Compact Delta-Based CRDT Engine (Automerge/Yrs) & HLC Causal Replication
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-001  
**Points:** 8 SP | **Sprint:** 2 | **Priority:** P0 Critical  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-storaged` to synchronize bioregional ledger state using compact delta-based Conflict-free Replicated Data Types (CRDTs) ordered by Hybrid Logical Clocks (HLC),  
**Benefit:** So that concurrent, offline edits across disconnected nodes merge deterministically without merge conflicts or central coordinators.

#### Technical Specifications & Architecture
- **Daemon:** `col-storaged` running delta CRDT replication (Automerge / Yrs engine).
- **Causality:** Kulkarni-Demir Hybrid Logical Clock (HLC) combining physical monotonic timestamps with logical sequence counters to guarantee strict partial ordering across distributed nodes.
- **State Delta Wire Format:** 24-byte aligned `StateDeltaOp` struct supporting generalized registers, sets, and counter mutations:

```cpp
namespace collective::crdt {

enum class OpType : uint8_t {
    REGISTER_SET = 0,
    COUNTER_ADD  = 1,
    SET_INSERT   = 2,
    SET_REMOVE   = 3
};

struct alignas(8) StateDeltaOp {
    uint64_t hlc_timestamp;      // Logical clock ordering
    uint32_t entity_id;          // Subject entity (account, resource, task)
    uint16_t property_id;        // Target property slot
    OpType   op_type;            // Operation enum
    uint8_t  padding;            // Strict 8-byte alignment
    uint64_t value_payload;      // Delta value or content hash reference
};
static_assert(sizeof(StateDeltaOp) == 24, "StateDeltaOp must remain exactly 24 bytes.");

} // namespace collective::crdt
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Concurrent offline CRDT mutations merge deterministically
  Given Node A and Node B partitioned for 24 hours
  When Node A increments resource inventory by 50 units at HLC T1
  And Node B decrements the same resource by 20 units at HLC T2
  When network connectivity is restored and delta ops are exchanged
  Then both nodes converge to an identical final state (+30 units)
  And p99 reconciliation latency for 1,000 operations is <= 10.0 ms.
```

#### Technical Tasks
- [ ] Implement Kulkarni-Demir Hybrid Logical Clock with drift clamping ($\le 500\text{ ms}$).
- [ ] Implement delta-based CRDT state reconciler for 24-byte `StateDeltaOp` structs.
- [ ] Add fuzz test executing 100,000 concurrent shuffled mutations asserting mathematical convergence.

---

### User Story 3.2: Content-Addressed IPLD / RocksDB Storage Engine with Erasure Coding
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-002  
**Points:** 7 SP | **Sprint:** 2 | **Priority:** P0 Critical  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-storaged` to store immutable blueprints, firmware images, and governance artifacts in an IPLD Merkle-DAG backed by RocksDB and Reed-Solomon erasure coding,  
**Benefit:** So that critical community assets are permanently addressable by CID and survive physical disk sector corruption.

#### Technical Specifications & Architecture
- **Storage Backend:** RocksDB LSM-tree at `/var/lib/collective/l3/blocks/` storing raw Content-Addressed Archives (CAR v2).
- **Addressing:** CID v1 (multihash BLAKE3, raw multicodec 0x55, base32 encoding).
- **Resilience:** Reed-Solomon $(8, 4)$ erasure coding partitioning chunks across available storage devices.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Immutable content retrieval via Content Identifier (CID)
  Given a 5 MB permaculture water catchment blueprint committed to col-storaged
  When the client queries the block store using its BLAKE3 CID
  Then the block store streams the verified byte payload with cryptographic inclusion proof
  And if 2 of the 12 erasure coded shards are corrupted, the payload reconstructs perfectly.
```

#### Technical Tasks
- [ ] Integrate embedded RocksDB with snappy compression and bloom filter optimizations.
- [ ] Implement IPLD CAR v2 parser and CID v1 multihash generator.
- [ ] Implement Reed-Solomon $(8, 4)$ chunk shard encoder and reconstructor.

---

### User Story 3.3: Proof-of-Thermodynamic-Work (PoTW) Negentropy Minting & Demurrage Decay Engine
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-003  
**Points:** 8 SP | **Sprint:** 2 | **Priority:** P0 Critical  
**Persona:** As a Community Steward,  
**Goal:** I want `col-storaged` to mint Exergy Credits strictly from verified physical negentropy additions and apply continuous physical demurrage,  
**Benefit:** So that community currency mirrors physical thermodynamics and speculative wealth hoarding is mathematically prevented.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapter 4 (`04_layer_3_thermodynamic_ledger.tex`) and `docs/02_VALUENOMICS.md`.
- **Minting Equation:**
  $$\Delta V = \int_{t_0}^{t_1} \left(\Phi_{in}(t) - \Phi_{out}(t)\right) \cdot \lambda_{ERC}(t) \, dt$$
  where $\Phi_{in}(t)$ is useful exergy delivered, $\Phi_{out}(t)$ is exergy lost or extracted, and $\lambda_{ERC}(t)$ is the Ecological Replacement Cost multiplier.
- **Canonical Unit:** $1.0\text{ Unit} \equiv 3.6\text{ MJ } (1.0\text{ kWh net exergy}) \equiv 2.0\text{ kg } \mathrm{CO}_2\text{ sequestered}$.
- **Demurrage Decay:** Ledger balances decay continuously based on the physical degradation of underlying stored assets (battery wear, grain drying, food shelf-life):
  $$V(t) = V_0 \cdot e^{-\delta_{asset}(t - t_0)}$$

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Proof-of-Thermodynamic-Work minting from verified solar exergy
  Given verified telemetry samples documenting 50 kWh of net solar exergy generated into the microgrid
  When col-storaged executes the minting evaluation
  Then it verifies hardware signatures and spatial cross-checks from adjacent nodes
  And mints exactly Delta V Exergy Credits to the stewardship pool
  And rejects any attempt to mint tokens from speculative trades or hash calculations.

Scenario: Continuous demurrage decay on stored exergy credits
  Given a balance of 1,000 Exergy Credits backed by electrochemical battery storage
  When 30 simulated days elapse with demurrage rate delta = 0.005/day
  Then the active ledger balance decays exponentially to 860.7 Exergy Credits
  And decayed credits are recycled to the bioregional ecological maintenance fund.
```

#### Technical Tasks
- [ ] Implement numerical integration loop for incoming exergy telemetry streams.
- [ ] Implement continuous demurrage calculation with daily decay reconciliation.
- [ ] Add unit test verifying mathematical precision of continuous exponential decay.

---

### User Story 3.4: Asymptotic Ecological Replacement Cost (ERC) Dynamic Firewall
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-004  
**Points:** 7 SP | **Sprint:** 2 | **Priority:** P0 Critical  
**Persona:** As an Ecological Resource Custodian,  
**Goal:** I want `col-storaged` to dynamically escalate the resource extraction multiplier $\lambda_{ERC}$ toward infinity as extraction approaches bioregional carrying capacity,  
**Benefit:** So that physical resource depletion (groundwater drawdown, timber cutting, soil carbon depletion) is stopped by protocol before ecological collapse occurs.

#### Technical Specifications & Architecture
- **Asymptotic Formula:**
  $$\lambda_{ERC}(R) = \lambda_{base} \cdot \left( \frac{1}{1 - \left( \frac{R_{current}}{R_{capacity}} \right)^\gamma} \right)$$
  where $R_{current}$ is current extracted volume, $R_{capacity}$ is ecological threshold, and $\gamma \ge 2.0$ determines curve steepness.
- **Firewall Trigger:** When $\lambda_{ERC}(R) > 1,000 \cdot \lambda_{base}$, `col-storaged` refuses all debit authorizations, halting downstream Layer 4 actuation tasks.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Aquifer over-extraction triggers asymptotic firewall
  Given an aquifer with capacity 100,000 liters and current drawdown at 98,500 liters
  When a water extraction request for 1,000 liters is submitted to col-storaged
  Then lambda_ERC spikes asymptotically exceeding the system threshold
  And col-storaged refuses the credit debit authorization
  And halts water pump actuation before ecological carrying capacity is breached.
```

#### Technical Tasks
- [ ] Implement asymptotic pricing function $\lambda_{ERC}(R)$ with configurable thresholds.
- [ ] Implement hard rejection circuit when extraction ratio $R_{curr} / R_{cap} \ge 0.99$.
- [ ] Add test validating mathematical curve progression and rejection behavior.

---

### User Story 3.5: Pairwise Bilateral Clearing Channels (BCC) & Bioregional Commons Jubilee
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-005  
**Points:** 8 SP | **Sprint:** 2 | **Priority:** P0 Critical  
**Persona:** As a Bioregional Settlement Steward,  
**Goal:** I want `col-storaged` to clear trade obligations across nodes using Pairwise Bilateral Clearing Channels (BCC) without a global token or master blockchain,  
**Benefit:** So that nodes settle inter-node trade peer-to-peer, eliminate circular debt loops, and execute periodic Commons Jubilees to prevent systemic debt peonage.

#### Technical Specifications & Architecture
- **Zero-Sum Conservation Invariant:** Between any two nodes $u$ and $v$:
  $$B_{u \to v}(t) + B_{v \to u}(t) = 0 \quad \text{within credit window } [-C_{u \to v}, +C_{v \to u}]$$
- **Cycle Canceling:** Implements Johnson's cycle-canceling algorithm to prune directed loops in the bioregional credit graph ($A \to B \to C \to A$).
- **Commons Jubilee:** If structural crises lock a channel beyond its credit window, the bioregional council can ratify a Jubilee transaction resetting bilateral balances to zero without bankruptcy litigation.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Automated clearing of circular debt loop
  Given Steward A owes Steward B 100 Credits, B owes C 100 Credits, and C owes A 100 Credits
  When col-storaged executes the cycle-canceling clearing pass
  Then it detects the directed cycle A -> B -> C -> A
  And cancels all three debts to 0 Credits
  And maintains the invariant sum(Balances) == 0 with zero net loss to any steward.
```

#### Technical Tasks
- [ ] Implement Pairwise Bilateral Clearing Channel state machine in C++20.
- [ ] Implement Johnson's cycle detection algorithm with $O((V+E)(c+1))$ complexity.
- [ ] Implement Commons Jubilee council transaction handler.

---

### User Story 3.6: Pre-Partition Cryptographic Sharding & Meshed Spatial Reasoning
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-006  
**Points:** 6 SP | **Sprint:** 2 | **Priority:** P1 High  
**Persona:** As a Distributed Systems Engineer,  
**Goal:** I want `col-storaged` to shard asset state roots to local physical microgrid zones and validate incoming sensor claims against spatial thermodynamic laws,  
**Benefit:** So that partitioned mesh islands continue operating without double-spending, and spoofed sensor readings are detected and quarantined.

#### Technical Specifications & Architecture
- **State Sharding:** Asset state roots are localized to physical microgrid domains $\mathcal{S}_{\mathrm{local}}(u)$. Operations touching local assets do not require wide-area consensus.
- **Meshed Spatial Reasoning:** Resolves the Oracle Problem. Cross-checks reported power generation against solar irradiance and adjacent node readings. If a deep borehole pump claims high water output with zero inverter current draw, the claim is flagged `QUALITY_SPOOF_SUSPECT` and quarantined.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Impossible sensor reading quarantined by spatial reasoning
  Given a 3 kW solar array claiming 25 kW power output at midnight
  When the telemetry sample is ingested by col-storaged
  Then the spatial reasoning validator detects physical law violation
  And flags the sample as QUALITY_SPOOF_SUSPECT
  And refuses to mint Exergy Credits for the fraudulent claim.
```

#### Technical Tasks
- [ ] Implement localized cryptographic state trie sharding per node.
- [ ] Implement spatial correlation engine cross-validating solar, wind, and battery physics.
- [ ] Add unit test verifying that fraudulent sensor injections are quarantined.

---

## 8. Epic 4: Layer 4 — Deterministic BPMN 2.0 Orchestrator & Ecological Floors (`col-execd`)

### User Story 4.1: Embedded Deterministic BPMN 2.0 Workflow VM & Sandboxed Wasm Runner
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-001  
**Points:** 8 SP | **Sprint:** 3 | **Priority:** P0 Critical  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-execd` to compile and deterministically execute BPMN 2.0 workflow state machines inside a metered WebAssembly sandbox,  
**Benefit:** So that automated municipal and ecological workflows execute identically across all nodes without non-deterministic drift.

#### Technical Specifications & Architecture
- **Daemon:** `col-execd` executing a deterministic 10 Hz logic tick.
- **Wasm Runtime:** Sandboxed Wasmtime / WAMR engine with:
  - Fuel/gas instruction metering preventing infinite loops.
  - NaN canonicalization forbidding non-deterministic floating-point math.
  - Time virtualization: system time is injected strictly from the block HLC timestamp.
- **BPMN Parser:** Compiles BPMN 2.0 XML activity graphs (ServiceTasks, ExclusiveGateways, BoundaryEvents) into bytecode transitions.

```xml
<?xml version="1.0" encoding="UTF-8"?>
<definitions xmlns="http://www.omg.org/spec/BPMN/20100524/MODEL" targetNamespace="http://collective.org/bpmn">
  <process id="IrrigationCycle" isExecutable="true">
    <startEvent id="Start_SoilMoistureLow"/>
    <sequenceFlow id="f1" sourceRef="Start_SoilMoistureLow" targetRef="CheckWaterReserve"/>
    <serviceTask id="CheckWaterReserve" name="Query Layer 3 Water Balances"/>
    <sequenceFlow id="f2" sourceRef="CheckWaterReserve" targetRef="Gateway_ReserveAdequate"/>
    <exclusiveGateway id="Gateway_ReserveAdequate" name="Reserve > Floor?"/>
    <sequenceFlow id="f3_yes" sourceRef="Gateway_ReserveAdequate" targetRef="ActuateValve">
      <conditionExpression>reserves &gt; 5000</conditionExpression>
    </sequenceFlow>
    <sequenceFlow id="f3_no" sourceRef="Gateway_ReserveAdequate" targetRef="End_Abort"/>
    <serviceTask id="ActuateValve" name="Emit Layer 1 Valve Pulse"/>
    <sequenceFlow id="f4" sourceRef="ActuateValve" targetRef="End_Success"/>
    <endEvent id="End_Success"/>
    <endEvent id="End_Abort"/>
  </process>
</definitions>
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Deterministic BPMN workflow execution
  Given the IrrigationCycle BPMN workflow compiled into col-execd
  When soil moisture falls below 20% and water reserves exceed 5,000 liters
  Then col-execd advances the execution token to ActuateValve
  And emits an ActuatorCommand pulse of 60 seconds
  And consumes exactly 42,100 Wasm instructions (fuel) deterministically.
```

#### Technical Tasks
- [ ] Integrate Wasmtime C API with instruction fuel metering.
- [ ] Implement BPMN 2.0 XML parsing pipeline compiling to state graph.
- [ ] Add unit test verifying that running out of fuel terminates execution with an error.

---

### User Story 4.2: Multi-Tier Priority Hierarchy Scheduler & Inviolable Ecological Floors
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-002  
**Points:** 7 SP | **Sprint:** 3 | **Priority:** P0 Critical  
**Persona:** As an Ecological Infrastructure Custodian,  
**Goal:** I want `col-execd` to enforce the strict six-tier priority scheduling hierarchy,  
**Benefit:** So that survival baselines and ecological floors are inviolably protected during resource deficits.

#### Technical Specifications & Architecture
- **Scheduling Order:**
  $$F_t \text{ (Ecological Floors)} \longrightarrow B_t \text{ (Basic Needs)} \longrightarrow C_t \text{ (Commons)} \longrightarrow O_t \text{ (Ordinary)} \longrightarrow E_t \text{ (Enhancements)} \longrightarrow R_t \text{ (Export)}$$
- **Resource Shedding:** If exergy forecast indicates a deficit, $R_t$ (export) is shed first, followed by $E_t$ and $O_t$. $F_t$ (minimum river flow, battery preservation, soil moisture) can never be shed under any circumstance.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Automated load shedding during microgrid energy deficit
  Given a severe storm dropping solar generation by 80%
  When col-execd evaluates total microgrid load against available reserves
  Then it immediately sheds Layer 7 commercial export (Rt) and workshop power (Et)
  While maintaining 100% uninterrupted power to battery thermal management (Ft) and medical refrigeration (Bt).
```

#### Technical Tasks
- [ ] Implement priority hierarchy queue in C++20.
- [ ] Implement automated shedding evaluator querying Layer 3 exergy reserves.
- [ ] Add test asserting that $F_t$ tasks are never dropped under deficit conditions.

---

### User Story 4.3: Cryptographically Bounded 80-Byte `ActuatorCommand` Dispatch
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-003  
**Points:** 7 SP | **Sprint:** 3 | **Priority:** P0 Critical  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-execd` to sign and dispatch bounded 80-byte `ActuatorCommand` envelopes to Layer 1,  
**Benefit:** So that actuators execute strictly with verified Layer 4 cryptographic authorization, bounded duration pulses, and zero runaway mechanical movement.

#### Technical Specifications & Architecture
- **Envelope Structure:** Conforms to `sizeof(ActuatorCommand) == 80` (64-byte Ed25519 signature, 8-byte command ID, 4-byte device ID, 4-byte duration, target state float).
- **Signing:** Layer 4 holds the private Ed25519 execution key in locked memory. Layer 1 verifies the signature against Layer 4's public key before pulsing the physical relay.
- **Pulse Width Clamping:** All commands enforce a hardware safety duration (`duration_ms <= 60000`). If duration exceeds 60 seconds, Layer 1 rejects the command.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Actuator command signing and execution pulse
  Given an authorized irrigation workflow step in col-execd
  When col-execd dispatches an ActuatorCommand to open water valve 0x12 for 15,000 ms
  Then the command carries a valid 64-byte Ed25519 signature
  And Layer 1 verifies the signature and pulses the valve for exactly 15,000 ms
  And automatically resets the valve to closed state upon timeout.
```

#### Technical Tasks
- [ ] Implement Ed25519 signature generator using libsodium / ed25519-dalek.
- [ ] Implement binary command envelope builder matching 80-byte alignment.
- [ ] Add unit test verifying rejection of commands with invalid signatures.

---

### User Story 4.4: Saga Transaction WAL with Automatic Compensatory Rollbacks
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-004  
**Points:** 8 SP | **Sprint:** 3 | **Priority:** P0 Critical  
**Persona:** As a Quality Engineer,  
**Goal:** I want `col-execd` to record multi-step distributed operations in a Saga Write-Ahead Log (WAL) with automated compensatory rollbacks upon failure,  
**Benefit:** So that complex multi-layer tasks (e.g. allocating exergy, locking physical tools, updating inventory) never leave the node in an inconsistent half-completed state.

#### Technical Specifications & Architecture
- **Storage:** Durable append-only WAL at `/var/lib/collective/l4/saga_wal.log`.
- **Saga Pattern:** Every forward step $S_i$ defines a compensating backward step $C_i$. If step $k$ fails (e.g. tool locker fails to open), the Saga coordinator executes $C_{k-1}, \dots, C_1$ in reverse order to unwind previously committed credits and locks.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Saga compensatory rollback upon physical hardware failure
  Given a 3-step saga: 1) Reserve 10 Exergy Credits, 2) Unlock CNC Tool Locker, 3) Dispatch Task Token
  When Step 1 succeeds, but Step 2 fails due to a jammed locker solenoid
  Then col-execd halts forward execution
  And executes Step 1's compensation action: releasing the 10 Exergy Credits back to the steward
  And marks the Saga transaction FAILED with zero orphaned state.
```

#### Technical Tasks
- [ ] Implement durable Saga WAL journal in C++20 with fsync guarantees.
- [ ] Implement forward and backward compensation step state machine.
- [ ] Add fault injection test simulating crash between steps asserting clean rollback.

---

### User Story 4.5: Automated SDD Scenario Workflow Engine Executing Canonical Acts I–V Workflows
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-005  
**Points:** 6 SP | **Sprint:** 3 | **Priority:** P0 Critical  
**Persona:** As a Scenario Architect,  
**Goal:** I want `col-execd` to load and execute pre-compiled BPMN workflows for all 24 canonical SDD scenarios across Acts I through V,  
**Benefit:** So that community operations (apprentice certifications, biochar burns, emergency triage, terminal decoupling) run through verified, automated cybernetic workflows.

#### Technical Specifications & Architecture
- **Workflows Packaged:**
  - `act1_fabrication.bpmn`, `act1_transit_mesh.bpmn`, `act1_kitchen.bpmn`, `act1_childcare.bpmn`
  - `act2_coworking.bpmn`, `act2_lodging.bpmn`, `act2_coppice.bpmn`, `act2_foundry.bpmn`, `act2_epistemic.bpmn`, `act2_apprenticeship.bpmn`, `act2_cultural.bpmn`
  - `act3_biochar.bpmn`, `act3_textiles.bpmn`, `act3_timber_frame.bpmn`, `act3_kinship.bpmn`, `act3_guild.bpmn`
  - `act4_maintenance.bpmn`, `act4_genesis.bpmn`, `act4_eldercare.bpmn`, `act4_paramedic.bpmn`, `act4_mediation.bpmn`
  - `act5_adversarial_def.bpmn`, `act5_microgrid_shock.bpmn`, `act5_terminal_decoupling.bpmn`
- **Execution Interface:** Workflows are loaded at boot from `/etc/collective/workflows/` and subscribed to domain triggers from Layer 6.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Execution of SCN-CHI-PARAMEDIC emergency workflow
  Given the act4_paramedic.bpmn workflow loaded in col-execd
  When an EmergencyMedicalAlert trigger is received from Layer 2
  Then col-execd advances the paramedic workflow: 1) Preempts non-critical mesh traffic,
  2) Queries nearest AED telemetry, 3) Broadcasts location to all Trust Ring R1 stewards
  And completes execution in <= 50 ms.
```

#### Technical Tasks
- [ ] Implement automated XML workflow loader and validator.
- [ ] Package and verify BPMN definitions for all 24 canonical SDD scenarios.
- [ ] Add integration test triggering and validating workflow completion.

---

## 9. Epic 5: Layer 5 — Polycentric Governance, Pairwise DIDs & Web of Trust (`col-kmsd`)

### User Story 5.1: W3C Decentralized Identifiers (`did:key`, `did:peer`, `did:mesh`) & SQLCipher KMS
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-001  
**Points:** 7 SP | **Sprint:** 3 | **Priority:** P0 Critical  
**Persona:** As a Community Steward,  
**Goal:** I want `col-kmsd` to manage my decentralized identities and sign transactions from a local encrypted vault,  
**Benefit:** So that I interact with community councils without centralized identity providers, cloud authentication, or state ID cards.

#### Technical Specifications & Architecture
- **Daemon:** `col-kmsd` running with memory locking (`mlockall`), core dumps disabled (`PR_SET_DUMPABLE = 0`), and isolated UNIX user permissions (`collective-kms`).
- **DID Methods:**
  - `did:key`: W3C DID Core 1.0 based on Ed25519 and Curve25519 public keys.
  - `did:peer`: Bilateral, pairwise peer channels preventing cross-council surveillance correlation.
  - `did:mesh`: Bioregional identities anchored to the Layer 3 state trie.
- **Storage:** SQLCipher AES-256 encrypted database at `/var/lib/collective/l5/vault.enc` with Argon2id key derivation.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Pairwise DID derivation for council privacy
  Given a steward participating in both the Water Council and the Energy Council
  When col-kmsd generates session credentials
  Then it derives unique pairwise did:peer identifiers for each council
  And mathematical correlation between the steward's actions across councils is impossible
  And private keys are zeroized in RAM immediately after signing.
```

#### Technical Tasks
- [ ] Implement W3C `did:key` and `did:peer` specification parsers and encoders.
- [ ] Integrate SQLCipher encrypted vault with Argon2id password hashing.
- [ ] Add unit test verifying zero trace of private keys in core dumps or memory pages.

---

### User Story 5.2: BBS+ ZKP Selective Disclosure Credentials & Sparse Merkle Tree Revocation Accumulator
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-002  
**Points:** 8 SP | **Sprint:** 3 | **Priority:** P0 Critical  
**Persona:** As a Community Steward,  
**Goal:** I want to present BBS+ zero-knowledge credentials proving specific qualifications without revealing my real-world identity,  
**Benefit:** So that I can claim safety-critical tasks (e.g. high-voltage electrical work) while protecting my personal privacy.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapter 6 (`06_layer_5_governance.tex`).
- **Cryptography:** BBS+ multi-message signatures over BLS12-381 pairing-friendly elliptic curve. Supports zero-knowledge selective disclosure proofs.
- **Revocation:** Cryptographic Sparse Merkle Tree (SMT) accumulator maintaining non-membership proofs for revoked credentials.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Zero-knowledge selective disclosure of electrical certification
  Given a Verifiable Credential containing {name: "Alice", role: "Electrician", cert_level: "HighVoltage", id: "9872"}
  When Alice submits a proof to claim a 480V battery wiring task
  Then col-kmsd generates a BBS+ zero-knowledge proof disclosing only {cert_level: "HighVoltage"}
  And the verifying council verifies the signature and SMT non-revocation status in <= 15.0 ms
  And Alice's name and identity remain completely hidden.
```

#### Technical Tasks
- [ ] Implement BBS+ signature generation and selective disclosure verifier using Arkworks/pairing.
- [ ] Implement 256-bit Sparse Merkle Tree (SMT) for cryptographic revocation checks.
- [ ] Add performance benchmark asserting BBS+ verification time $\le 15.0\text{ ms}$ on ARM64 Cortex-A72.

---

### User Story 5.3: FROST Threshold Multi-Signatures & Shamir Social Key Recovery
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-003  
**Points:** 7 SP | **Sprint:** 3 | **Priority:** P0 Critical  
**Persona:** As a Community Steward,  
**Goal:** I want my cryptographic identity to be recoverable through an $(m, n)$ Shamir social recovery ceremony,  
**Benefit:** So that losing a physical key or hardware device does not destroy my identity or reputation in the community.

#### Technical Specifications & Architecture
- **FROST Protocol:** Flexible Round-Optimized Schnorr Threshold signatures allowing $m$-of-$n$ signers to produce a standard single Ed25519 signature over two rounds.
- **Social Recovery:** Master identity key is split into $n$ Shamir secret shards distributed to trusted guardians. Reconstructing $m$ shards in an in-person ceremony re-binds the identity to a new hardware key.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Social key recovery re-binds lost hardware key
  Given a steward with a 3-of-5 social recovery configuration who loses their physical phone
  When 3 designated community guardians present their recovery shards in person
  Then col-kmsd reconstructs the master secret and binds it to a new hardware key
  And all previous historical reputation and Soulbound credentials transfer seamlessly.
```

#### Technical Tasks
- [ ] Implement FROST threshold signature protocol in C++/Rust.
- [ ] Implement Shamir Secret Sharing $(m, n)$ splitting and reconstruction.
- [ ] Add unit test verifying identity restoration with $m$ shards and failure with $m-1$ shards.

---

### User Story 5.4: Bioregional Path-Vector Protocol (BPVP / FRRP) across Trust Rings ($R_0 \to R_6$)
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-004  
**Points:** 8 SP | **Sprint:** 4 | **Priority:** P0 Critical  
**Persona:** As a Bioregional Routing Engineer,  
**Goal:** I want `col-kmsd` to execute the Bioregional Path-Vector Protocol (BPVP / FRRP) across Concentric Trust Rings,  
**Benefit:** So that physical resources (exergy, water, timber) are routed efficiently across autonomous nodes with provable loop-freedom and thermodynamic path attenuation.

#### Technical Specifications & Architecture
- **Capability Advertisement:**
  $$\mathcal{A} = \big\langle \mathrm{adv\_id}, \, v_{\mathrm{orig}}, \, \mathcal{T}_{\mathrm{res}}, \, \mathbf{u}, \, Q_{\mathrm{avail}}, \, [\tau_{\mathrm{start}}, \tau_{\mathrm{end}}], \, \mathcal{P}, \, \mathcal{C}, \, H_{\mathrm{ttl}}, \, \sigma \big\rangle$$
- **Loop-Freedom Invariant:** Uses Ring-Path Vector $\mathcal{P} = [(v_k, r_k), \dots, (v_0, r_0)]$. If recipient node $w$ is already present in $\mathcal{P}$, announcement is dropped instantly:
  $$\mathrm{LoopCheck}(w, \mathcal{P}) = \begin{cases} \mathrm{DISCARD}, & \text{if } \exists (v_i, r_i) \in \mathcal{P} \text{ such that } v_i = w \\ \mathrm{ACCEPT}, & \text{otherwise} \end{cases}$$
- **Delivered Effective Exergy:**
  $$\mathcal{E}_{\mathrm{eff}}(\mathcal{P}) = \mathcal{E}_0 \prod_{k=1}^n (1 - \lambda_k) - \sum_{k=1}^n \mathcal{E}_{\mathrm{transport}}(e_k)$$
  If net route efficiency $\eta_{\mathrm{route}}(\mathcal{P}) < \eta_{\mathrm{min}}$ ($0.60$), the route is rejected.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: BPVP loop detection discards circulating advertisement
  Given an advertisement path vector P = [(NodeC, R2), (NodeB, R1), (NodeA, R0)]
  When Node A receives this advertisement
  Then col-kmsd detects Node A in the path vector
  And immediately discards the advertisement, preventing routing loops in O(D) time.
```

#### Technical Tasks
- [ ] Implement BPVP packet format and Ring-Path Vector $\mathcal{P}$ parser.
- [ ] Implement $O(D)$ loop detection and path attenuation calculation $\mathcal{C}(\mathcal{P})$.
- [ ] Add test validating route discard when delivery efficiency $\eta_{\mathrm{route}} < 0.60$.

---

### User Story 5.5: Dynamic Request Cascading ($R_0 \to R_4$) & Constitutional Sovereignty Firewalls
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-005  
**Points:** 6 SP | **Sprint:** 4 | **Priority:** P0 Critical  
**Persona:** As a Bioregional Governance Steward,  
**Goal:** I want `col-kmsd` to cascade resource requests progressively across 7 concentric Trust Rings while enforcing local sovereignty firewalls,  
**Benefit:** So that local needs are fulfilled locally before seeking external resources, and external demands never breach local survival baselines.

#### Technical Specifications & Architecture
- **Concentric Trust Rings:**
  $$R_0 / R_1 \text{ (Hearth/Hamlet)} \longrightarrow R_2 \text{ (Commons } \le 5\text{km)} \longrightarrow R_3 \text{ (Watershed } \le 30\text{km)} \longrightarrow R_4 \text{ (Bioregion } \le 150\text{km)}$$
- **Constitutional Sovereignty Firewalls:** Guarantees that external trade or transit requests cannot violate local ecological floors ($F_{\mathrm{eco}}$) or survival baselines ($B_{\mathrm{baseline}}$). Local nodes maintain an absolute Autonomous Transit Veto (`No-Transit`) over sensitive pedestrian and ecological glades.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Local survival floor blocks external export request
  Given a severe winter freeze where local heating reserves are near baseline B_baseline
  When a high-value resource request arrives from a node in Trust Ring R3
  Then col-kmsd's Constitutional Sovereignty Firewall automatically rejects the request
  And local heating reserves remain 100% protected for resident survival.
```

#### Technical Tasks
- [ ] Implement Trust Ring request cascading state machine ($R_0 \to R_4$).
- [ ] Implement Constitutional Sovereignty Firewall filter blocking baseline breaches.
- [ ] Add unit test verifying Autonomous Transit Veto drops transit routes.

---

### User Story 5.6: Reputational Wealth Exponential Decay & Logarithmic Anti-Oligopoly Capping
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-006  
**Points:** 6 SP | **Sprint:** 4 | **Priority:** P1 High  
**Persona:** As a Social Cybernetics Theorist,  
**Goal:** I want `col-kmsd` to apply continuous exponential decay and logarithmic capping to reputational standing ($\mathcal{W}$),  
**Benefit:** So that governance power requires active, ongoing community stewardship and historical founders cannot entrench themselves as permanent oligarchs.

#### Technical Specifications & Architecture
- **Exponential Decay:**
  $$\mathcal{W}(t) = \mathcal{W}_0 \cdot 2^{-\frac{t - t_0}{\tau_{\mathrm{half}}}}$$
  with calibrated half-life $\tau_{\mathrm{half}} = 180\text{ days}$.
- **Sublinear Logarithmic Cap:** To prevent extreme influence disparities, effective voting weight $\mathcal{V}$ is capped logarithmically:
  $$\mathcal{V}(\mathcal{W}) = \mathcal{V}_{\mathrm{base}} + \alpha \cdot \ln(1 + \beta \cdot \mathcal{W})$$
  ensuring that a steward with 1,000x raw points has only $\le 4\times$ the voting influence of an active novice.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Founder reputation decays during 6-month inactivity
  Given a founder with 1,000 Reputational Wealth points who ceases active stewardship
  When 180 simulated days elapse with tau_half = 180 days
  Then col-kmsd decays the raw score to exactly 500 points
  And logarithmic voting weight decreases smoothly according to the sublinear cap.
```

#### Technical Tasks
- [ ] Implement continuous exponential decay calculation for reputation scores.
- [ ] Implement logarithmic voting weight transformation $\mathcal{V}(\mathcal{W})$.
- [ ] Add unit test verifying mathematical bounds and anti-oligopoly guarantees.

---

### User Story 5.7: Progressive Migrant Onboarding & Trust Ring ($R_6 \to R_1$) Assimilation Protocol
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-007  
**Points:** 8 SP | **Sprint:** Dual Sprint 13 | **Priority:** P0 Critical  
**Persona:** As a Community Steward & Membership Guardian,  
**Goal:** I want `col-kmsd` to manage probationary pairwise DIDs (`Ring R6: Unverified Migrant` $\to$ `Ring R3: Probationary Resident` $\to$ `Ring R1: Vouched Steward`) as thermodynamic mutual-aid milestones (shelter, potable water, caloric security, and cooperative work contributions) are attested in Layer 3,  
**Benefit:** So that newcomers arriving during Legacy Migration Waves (`SS-EP7-007`) are safely assimilated into the mutual-credit commonwealth without exposing the enclave to Sybil governance capture or credit-pool exhaustion.

#### Technical Specifications & Architecture
- **SDD & Horizon Alignment:** `SCN-TAU-GENESIS`, `SCN-OMICRON-KIN`, `SCN-ZETA-LODGE`, and Horizon Pillar 5 (*Migration Waves & Legacy NPCs*).
- **Strict Layer Adjacency Propagation (`L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI`):**
  - When `col-adversaryd` (L7, isolated in DMZ VLAN 40) emits a `MigrationWaveArrivalEvent` over `/run/collective/ipc/l6_l7.sock`, `col-commonsd` (L6) compiles a humanitarian intake intent and forwards it over `/run/collective/ipc/l5_l6.sock` to `col-kmsd` (L5).
  - `col-kmsd` initializes the newcomer's ephemeral `Ring R6` guest credential and forwards the authorized survival baseline intent over `/run/collective/ipc/l4_l5.sock` to `col-execd` (L4) $\to$ `/run/collective/ipc/l3_l4.sock` to `col-storaged` (L3) $\to$ `/run/collective/ipc/l2_l3.sock` to `col-meshd` (L2) $\to$ UHAI shared-memory ring (`/dev/shm/uhai_telemetry`, `QUALITY_SIMULATED = 0x01`) for Layer 1b (Oasis) consumption. Zero layers are ever skipped.
- **Probationary Trust Ring State Machine ($R_6 \to R_3 \to R_1$):**
  1. **`Ring R6 (Unverified Migrant)`:** Upon receiving the intake intent from Layer 6 (`col-commonsd`) over `/run/collective/ipc/l5_l6.sock`, `col-kmsd` provisions an ephemeral, rate-limited `did:peer` credential in `Ring R6`. The newcomer receives unconditional humanitarian access to survival baselines ($B_t$: emergency shelter thermal envelope $>18^\circ\text{C}$, potable water, and daily caloric rations from `SCN-GAMMA-KITCH`), but holds **zero** Bilateral Clearing Channel (BCC) credit overdraft authority ($C_{u \to v} = 0$) and zero governance voting weight ($\mathcal{V} = 0$).
  2. **`Ring R3 (Probationary Resident)`:** Transitioned automatically when Layer 3 (`col-storaged`, relayed via Layer 4 `col-execd`) attests $\ge 7\text{ consecutive days}$ of verified homeostatic stability (enclosed shelter assignment + caloric/water allocation) and $\ge 36.0\text{ MJ}$ ($10.0\text{ Exergy Credits}$) of verified cooperative sweat-equity contributions (`bounty.jsonld` completions). Unlocks bounded local BCC mutual credit ($[-25.0, +25.0]\text{ Exergy Credits}$) and apprentice task claiming (`SCN-KAPPA-APPR`).
  3. **`Ring R1 (Vouched Steward)`:** Requires cryptographic co-signing by a $2\text{-of-}3$ FROST threshold quorum of existing `Ring R1` stewards (`SCN-TAU-GENESIS` ceremony) after $\ge 30\text{ simulated days}$ in `Ring R3` with zero unresolved restorative justice mediations (`SCN-PSI-MEDIAT`). Issues a full BBS+ Steward Verifiable Credential and unlocks full governance participation.
- **Regression & Sybil Safeguard:** If enclave infrastructure fails to meet basic shelter/caloric baselines during a demographic shock and a probationary `R6`/`R3` identity triggers resource hoarding or conflict mediation events, `col-kmsd` freezes credit expansion and routes a `RestorativeTriageIntent` upward to Layer 6 (`col-commonsd`) without revoking basic humanitarian survival floors ($B_t$).
- **Build & Test Scaffolding (`core/1_simulation/src/CMakeLists.txt`):** Scaffolds standalone C++20 daemon target `col_kmsd` (`col-kmsd`) alongside the 6 existing daemon targets (`col_telemetryd`, `col_execd`, `col_commonsd`, `col_adversaryd`, `col_meshd`, `col_gatewayd`) and `col_storaged`, plus Catch2 verification target `test_migration_waves.cpp`.

```protobuf
syntax = "proto3";
package collective.l5;

enum AssimilationRing {
  RING_R6_UNVERIFIED_MIGRANT = 0;
  RING_R3_PROBATIONARY_RESIDENT = 1;
  RING_R1_VOUCHED_STEWARD = 2;
}

message MigrantAssimilationState {
  string migrant_pairwise_did = 1;
  AssimilationRing current_ring = 2;
  double verified_sweat_equity_mj = 3;
  uint32_t sheltered_days_consecutive = 4;
  repeated string vouching_steward_dids = 5;
  uint64_t last_transition_hlc = 6;
}
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Progressive migrant onboarding from Ring R6 to Ring R3 and Ring R1 across strict layer adjacency
  Given a MigrationWaveArrivalEvent emitted by col-adversaryd (L7) over l6_l7.sock to col-commonsd (L6) and forwarded over l5_l6.sock to col-kmsd (L5)
  Then col-kmsd registers the newly arrived migrant in Ring R6 (Unverified Migrant) with unconditional access to Bt survival rations and shelter but 0.0 Exergy Credit overdraft and 0 voting weight
  And propagates the Ring R6 onboarding state inward via l4_l5.sock -> l3_l4.sock -> l2_l3.sock -> /dev/shm/uhai_telemetry (QUALITY_SIMULATED = 0x01)
  When Layer 3 attests 7 consecutive days of sheltered homeostasis and 36.0 MJ of completed work bounties
  Then col-kmsd transitions the migrant's pairwise DID to Ring R3 (Probationary Resident) with a 25.0 Exergy Credit BCC window
  When 2-of-3 existing Ring R1 stewards co-sign a FROST genesis attestation after 30 days
  Then col-kmsd issues a BBS+ Steward Credential promoting the resident to Ring R1 (Vouched Steward).
```

#### Technical Tasks
- [ ] Scaffold standalone CMake executable target `col_kmsd` (`col-kmsd`) in `core/1_simulation/src/CMakeLists.txt` alongside the 6 existing daemons (`col_telemetryd`, `col_execd`, `col_commonsd`, `col_adversaryd`, `col_meshd`, `col_gatewayd`).
- [ ] Implement `MigrantAssimilationState` FSM in `col-kmsd` persisted in the encrypted SQLCipher vault, enforcing strict `L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI` adjacency.
- [ ] Implement Layer 3 attestation listener (via `l3_l4.sock` $\to$ `l4_l5.sock`) verifying shelter (`SCN-ZETA-LODGE`), kitchen (`SCN-GAMMA-KITCH`), and sweat-equity (`bounty.jsonld`) milestones.
- [ ] Implement $2\text{-of-}3$ FROST steward vouching ceremony promoting `Ring R3` to `Ring R1`.
- [ ] Implement Catch2 test suite `test_migration_waves.cpp` asserting zero Sybil voting power for `Ring R6` arrivals, $B_t$ humanitarian floor guarantees, and full `L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI` traversal.

---

## 10. Epic 6: Layer 6 — Semantic Intent, Knowledge Graphs, The Agora Commons & Companion Gateway (`col-commonsd` & `col-gatewayd`)

### User Story 6.1: Semantic Intent Parser, W3C JSON-LD Compiler & Calm Technology Read-Back
**Epic:** Epic 6 (Layer 6: Semantic Intent & Agora)  
**Story ID:** SS-EP6-001  
**Points:** 7 SP | **Sprint:** 4 | **Priority:** P0 Critical  
**Persona:** As a Human Steward,  
**Goal:** I want `col-commonsd` to parse my natural language requests into structured W3C JSON-LD schemas and require plain-language confirmation before signing,  
**Benefit:** So that AI assists me in drafting actions without hallucinating parameters or executing unapproved commands.

#### Technical Specifications & Architecture
- **Daemon:** `col-commonsd` exposing WebSocket and UDS endpoints.
- **Calm Technology Boundary:** Edge LLMs draft JSON-LD envelopes, but the daemon enforces **Plain Language Read-Back**: verbatim plain-text summary displayed to the human steward. The command is only signed with the steward's `did:key` upon explicit physical confirmation.

```json
{
  "@context": [
    "https://www.w3.org/ns/activitystreams",
    "https://collective.org/ns/intent/v1"
  ],
  "type": "ResourceRequestIntent",
  "actor": "did:key:z6MkhaXgBZDvotDkL5257faiztiGiC2QtKLGpbnnEGta2doK",
  "resourceType": "PotableWater",
  "quantity": {
    "value": 500.0,
    "unit": "Liters"
  },
  "maxExergyCredits": 25.0,
  "requiredBy": "2026-10-15T18:00:00Z"
}
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Intent compilation and plain language confirmation
  Given a steward speaking "Order 500 liters of drinking water for the south greenhouse"
  When col-commonsd compiles the input into a ResourceRequestIntent JSON-LD envelope
  Then it generates a plain-language summary: "Confirm transfer of 25.0 Credits for 500L water"
  And requires explicit steward cryptographic sign-off before forwarding to Layer 5.
```

#### Technical Tasks
- [ ] Implement W3C JSON-LD schema compiler and validator in C++/Rust.
- [ ] Implement plain-language read-back text generator.
- [ ] Add test asserting that unconfirmed drafts expire without reaching Layer 5.

---

### User Story 6.2: Embedded W3C PROV-O Provenance Engine & Oxigraph RDF Quad Store
**Epic:** Epic 6 (Layer 6: Semantic Intent & Agora)  
**Story ID:** SS-EP6-002  
**Points:** 7 SP | **Sprint:** 4 | **Priority:** P0 Critical  
**Persona:** As a Bioregional Auditor,  
**Goal:** I want `col-commonsd` to track the full lineage of every physical good, task, and transaction using W3C PROV-O stored in an embedded Oxigraph RDF quad store,  
**Benefit:** So that materials, tools, and food batches possess an immutable, queryable provenance graph.

#### Technical Specifications & Architecture
- **Storage:** Embedded Oxigraph RDF quad store at `/var/lib/collective/l6/prov.oxigraph` supporting SPARQL 1.1 queries.
- **Ontology:** W3C PROV-O (`prov:wasGeneratedBy`, `prov:wasDerivedFrom`, `prov:wasAssociatedWith`). Every finished good (e.g. biochar, lumber, meal) links to raw materials, energy inputs, and steward DIDs.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Querying provenance graph for organic biochar batch
  Given a batch of biochar registered in col-commonsd with PROV-O metadata
  When an auditor queries via SPARQL for the originating coppice wood batch and kiln telemetry
  Then Oxigraph returns the complete derivation graph including ATECC608A signed kiln temperatures
  And confirms that zero uncertified feedstock was mixed into the batch.
```

#### Technical Tasks
- [ ] Integrate Oxigraph embedded RDF store in C++ via Rust FFI.
- [ ] Implement W3C PROV-O graph generator for completed tasks.
- [ ] Add SPARQL query test asserting graph completeness across 10 linked derivation steps.

---

### User Story 6.3: Continuous Double-Auction Matching Engine for Bioregional Agora
**Epic:** Epic 6 (Layer 6: Semantic Intent & Agora)  
**Story ID:** SS-EP6-003  
**Points:** 7 SP | **Sprint:** 4 | **Priority:** P0 Critical  
**Persona:** As a Local Producer,  
**Goal:** I want `col-commonsd` to match bids and offers for physical goods and labor using a continuous double-auction order book,  
**Benefit:** So that community resources are allocated transparently without middlemen, speculation, or centralized trading fees.

#### Technical Specifications & Architecture
- **Engine:** Continuous double-auction with price-time priority matching.
- **Clearing Price:** Clears at the midpoint of matching limit orders ($P_{\mathrm{clear}} = \frac{P_{\mathrm{bid}} + P_{\mathrm{ask}}}{2}$).
- **Currency:** Bids and asks denominated strictly in Exergy Credits ($3.6\text{ MJ}$). Zero fiat trading allowed in Layer 6.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Double-auction clears matched bids and asks
  Given a bid for 10 kg of flour at 5.0 Exergy Credits and an ask at 4.0 Exergy Credits
  When col-commonsd executes its matching tick
  Then the order clears at 4.5 Exergy Credits for 10 kg of flour
  And generates matching execution intents sent to Layer 4 for physical locker release.
```

#### Technical Tasks
- [ ] Implement in-memory order book with price-time priority queue.
- [ ] Implement midpoint execution matching algorithm.
- [ ] Add unit test validating fair execution across simultaneous bids and asks.

---

### User Story 6.4: SDD Scenario Bounty Compiler (`bounty.jsonld`) & Dynamic Sweat-Equity Registry
**Epic:** Epic 6 (Layer 6: Semantic Intent & Agora)  
**Story ID:** SS-EP6-004  
**Points:** 6 SP | **Sprint:** 4 | **Priority:** P0 Critical  
**Persona:** As a Community Organizer,  
**Goal:** I want `col-commonsd` to compile work bounties for all 24 canonical SDD scenarios into standard `bounty.jsonld` schemas with dynamic sweat-equity scaling,  
**Benefit:** So that essential but unglamorous chores (composting, kitchen cleaning, maintenance) dynamically increase in credit value until claimed.

#### Technical Specifications & Architecture
- **Schema:** Standardized `bounty.jsonld` defining task requirements, location, safety VCs, and reward credits.
- **Dynamic Bounty Scaling:**
  $$\mathrm{Reward}(t) = \mathrm{Reward}_0 \cdot \left(1 + \kappa \cdot \Delta t_{\mathrm{unclaimed}}\right)$$
  Chore bounties escalate exponentially until claimed, preventing task starvation (`SCN-SIGMA-MAINTENANCE`).
- **Sweat Equity:** Volunteer foundational hours log into the Proto-Ledger care registry, establishing Reputational Wealth at settlement ignition (`SCN-TAU-GENESIS`).

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Maintenance chore reward escalates when unclaimed
  Given an unassigned composting chore with base reward 2.0 Exergy Credits
  When the chore sits unclaimed for 48 hours
  Then col-commonsd escalates the reward dynamically to 4.8 Exergy Credits
  When a steward claims and completes the chore, the reward is credited and reset to base.
```

#### Technical Tasks
- [ ] Implement `bounty.jsonld` schema validator and task registry.
- [ ] Implement dynamic reward escalation formula with anti-gaming caps.
- [ ] Add unit test verifying sweat-equity registration in Proto-Ledger.

---

### User Story 6.5: Ricardian Treaty Engine ($\mathrm{Treaty\_CID}$) Binding Prose, Schema & BPMN Invariants
**Epic:** Epic 6 (Layer 6: Semantic Intent & Agora)  
**Story ID:** SS-EP6-005  
**Points:** 6 SP | **Sprint:** 4 | **Priority:** P0 Critical  
**Persona:** As an Inter-Node Diplomat,  
**Goal:** I want `col-commonsd` to compile multilateral agreements into Ricardian Treaties ($\mathrm{Treaty\_CID}$) cryptographically binding legal prose, JSON-LD schemas, and BPMN invariants,  
**Benefit:** So that agreements between autonomous nodes are readable by humans and strictly executable by machines without discrepancy.

#### Technical Specifications & Architecture
- **Ricardian Structure:** Three-part bundle:
  1. Human-readable markdown legal text.
  2. Machine-readable JSON-LD parameters (quotas, deadlines, DID signatories).
  3. Deterministic BPMN 2.0 XML workflow governing automated execution.
- **Digest:** All three components are hashed together into an immutable 32-byte BLAKE3 digest ($\mathrm{Treaty\_CID}$). Modifying a single character of prose or a single parameter breaks the hash.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Ricardian treaty cryptographic verification
  Given a bilateral watershed treaty binding Node A and Node B
  When both nodes sign the treaty, col-commonsd computes Treaty_CID
  Then both nodes verify that prose, JSON-LD parameters, and BPMN XML match the digest
  And any discrepancy between human text and machine code fails signature verification.
```

#### Technical Tasks
- [ ] Implement Ricardian treaty bundler combining markdown, JSON-LD, and BPMN XML.
- [ ] Implement BLAKE3 multi-part digest generator for $\mathrm{Treaty\_CID}$.
- [ ] Add test asserting that tampering with legal prose invalidates machine execution.

---

### User Story 6.6: Layer 6 Semantic Gateway (`col-gatewayd`), Read-Only Cybernetic Hood & Strict Intent U-Turn Enforcement
**Epic:** Epic 6 (Layer 6: Semantic Intent, Agora & Companion Gateway)  
**Story ID:** SS-EP6-006  
**Points:** 8 SP | **Sprint:** Dual Sprint 11 | **Priority:** P0 Critical  
**Persona:** As a Sovereign Node Steward using the Cadastral Slate Companion App,  
**Goal:** I want `col-gatewayd` to terminate TLS 1.3 HTTPS/WSS exclusively on port `8443` on VLAN 30, accept signed human intent and governance ratification mutations exclusively at Layer 6 (`POST /api/v1/l6/intent`), enforce strictly read-only diagnostic endpoints across Layers 2, 3, 4, 5, and 7, and guarantee the Strict Intent U-Turn across the daemon mesh,  
**Benefit:** So that human authorship and `FlowContract` ratifications enter cleanly through Layer 6 semantic translation while preserving Layer 4's blind sandbox, Layer 5's governance supremacy, and total isolation between Layer 4 and Layer 7.

#### Technical Specifications & Architecture
- **Reference Specification:** `docs/COMPANION_APP_ARCHITECTURE.md` (V2.0 Authoritative Sovereign Stack Specification).
- **Daemon & Network (`VLAN 30`, TLS 1.3 Port `8443`):** `col-gatewayd` listens on VLAN 30 (`https://collective.local:8443` and `wss://collective.local:8443/ws/v1`, TLS 1.3 + mDNS `_collective._tcp.local`; unencrypted HTTP port `8080` is permanently disabled), reading dedicated 1:1 SPSC shared-memory rings (`/dev/shm/uhai_telemetry_gateway`, `/dev/shm/uhai_flow_gateway`) mapped with the 192-byte `SharedRingHeader` (`TOTAL_SHM_SIZE = 65728`) to eliminate consumer cursor contention with `col-execd`.
- **Exclusive Layer 6 Write Ingress (`POST /api/v1/l6/intent`):**
  - All member interactions—including Agora order placement, spatial work-order designations (`SpatialIntentPayload`), bounty claims, treaty signatures, and human governance ratifications of held Layer 4 `FlowContract` / `[upward-yield]` requests—enter **exclusively at Layer 6** (`POST /api/v1/l6/intent`), authenticated with a Web Crypto Ed25519 signature (`X-Collective-Signature`) and monotonic sequence header (`X-Collective-Sequence`).
  - Layer 6 (`col-commonsd`) strictly assumes Layer 7 does not exist internally.
- **Read-Only Cybernetic Hood (Layers 2, 3, 4, 5 & 7 — Superseding Direct `/api/v1/l5/*` Writes):**
  - `GET /api/v1/l2/telemetry`, `GET /api/v1/l3/ledger`, `GET /api/v1/l4/diagnostics`, `GET /api/v1/l5/governance`, and `GET /api/v1/l7/carapace` (plus 10 Hz `wss://collective.local:8443/ws/v1` streams) are **strictly read-only diagnostic inspection endpoints**.
  - Any `POST`, `PUT`, `PATCH`, or `DELETE` request targeting `/api/v1/l2/*`, `/api/v1/l3/*`, `/api/v1/l4/*`, `/api/v1/l5/*`, or `/api/v1/l7/*` is immediately rejected by `col-gatewayd` with HTTP `403 Forbidden` / `405 Method Not Allowed`.
- **The Strict Intent U-Turn & Human Ratification Loop (Zero `l4_l7.sock`):**
  1. **Downward Intent Flow:** `Human (Web Crypto)` $\to$ `col-gatewayd (:8443)` (`POST /api/v1/l6/intent`) $\to$ `L6 (col-commonsd)` $\to$ `L5 (col-kmsd Governance Validation)` $\to$ `L4 (col-execd Sandboxed Execution)`.
  2. **Upward Yield & Human Ratification U-Turn:** When a deterministic workflow or `FlowContract` in `L4 (col-execd)` requires human steward ratification or external legacy interaction, it **NEVER** communicates with `L7 (col-adversaryd)`. Instead, `L4` emits a `YIELD_UPWARD_L5` token (`FlowContractStatus::Yielded`) over `/run/collective/ipc/l4_l5.sock` to `L5 (col-kmsd)`. `L5` holds the contract, streams a read-only notification to the Companion App over `wss://collective.local:8443/ws/v1`, and awaits the steward's signed ratification via `POST /api/v1/l6/intent` (`Human -> L6 -> L5`). Once `L6 (col-commonsd)` validates and forwards the signed approval inward to `L5 (col-kmsd)` over `/run/collective/ipc/l5_l6.sock`, `L5` either releases the held `L4` contract (`L5 -> L4`) or, for external statutory/utility actions, yields upward over `/run/collective/ipc/l5_l6.sock` $\to$ `L6 (col-commonsd)` (which applies the Mundane Semantic Camouflage Filter) $\to$ `/run/collective/ipc/l6_l7.sock` $\to$ `L7 (col-adversaryd)` (`Human -> L6 -> L5 -> L4 -> [yield] -> L5 -> L6 -> L7`).

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Read-only Cybernetic Hood rejects direct mutation attempts on Layers 2, 3, 4, 5, and 7
  Given an authenticated Cadastral Slate session connected to col-gatewayd via TLS 1.3 on port 8443
  When a client sends a POST, PUT, PATCH, or DELETE request to /api/v1/l4/diagnostics, /api/v1/l5/governance, or /api/v1/l7/carapace
  Then col-gatewayd immediately rejects the request with HTTP 403 Forbidden or 405 Method Not Allowed
  And logs an ImmutableHoodViolationAttempt to the security journal
  And only signed W3C JSON-LD payloads sent to POST /api/v1/l6/intent are forwarded to col-commonsd.

Scenario: Strict Intent U-Turn and L6->L5 human ratification when Layer 4 yields a FlowContract or legacy action
  Given a steward submitting a signed intent via POST /api/v1/l6/intent on port 8443 that compiles down through L6 -> L5 -> L4
  When col-execd (L4) reaches a workflow step or FlowContract requiring steward ratification or external statutory filing action
  Then col-execd never opens a socket to Layer 7 (zero l4_l7.sock exists)
  And col-execd yields upward (YIELD_UPWARD_L5) over l4_l5.sock to col-kmsd (L5), which notifies the Companion App over WSS :8443
  When the steward ratifies the held action by submitting a signed approval to POST /api/v1/l6/intent
  Then col-commonsd (L6) validates and forwards the ratification inward over l5_l6.sock to col-kmsd (L5)
  And col-kmsd releases the held Layer 4 contract (or yields upward via l5_l6.sock -> col-commonsd -> l6_l7.sock -> col-adversaryd for external legacy filings).
```

#### Technical Tasks
- [ ] Implement `col-gatewayd` TLS 1.3 HTTPS/WSS server on VLAN 30 port `8443` with `/dev/shm/uhai_telemetry_gateway` and `/dev/shm/uhai_flow_gateway` SPSC readers (`SharedRingHeader`, `TOTAL_SHM_SIZE = 65728`).
- [ ] Implement strict HTTP method router enforcing `403 Forbidden` / `405 Method Not Allowed` on any non-`GET`/`HEAD` request to `/api/v1/l2/*`, `/api/v1/l3/*`, `/api/v1/l4/*`, `/api/v1/l5/*`, and `/api/v1/l7/*`.
- [ ] Implement `POST /api/v1/l6/intent` Web Crypto Ed25519 signature verifier and `/run/collective/ipc/gw_l6.sock` forwarder supporting both new intents and `L6 -> L5` governance ratifications of yielded `FlowContract` records.
- [ ] Implement `YIELD_UPWARD_L5` state machine (`Human -> L6 -> L5 -> L4 -> [yield] -> L5 -> L6 -> L7`) and add CI test asserting zero `l4_l7.sock` descriptor existence.

---

## 11. Epic 7: Layer 7 — Legacy Proxy, Cooperative Legal Carapace & Demographic Shock Simulation (`col-adversaryd`)

### User Story 7.1: Discrete Event Simulation (DES) Priority-Queue Engine & Configurable Stress Controller (Levels 0–3)
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Cooperative Legal Carapace)  
**Story ID:** SS-EP7-001  
**Points:** 8 SP | **Sprint:** 5 | **Priority:** P0 Critical  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-adversaryd` to model external legacy system conditions in good faith using a priority-queue Discrete Event Simulation (DES) engine with configurable stress levels (0–3),  
**Benefit:** So that cooperative communities can stress-test their physical, legal, and financial resilience against real-world regulatory, tariff, and macroeconomic pressures before deploying off-grid.

#### Technical Specifications & Architecture
- **Daemon:** `col-adversaryd` scheduled with `nice +10` background priority (`BOOK_TEAM_DRIFT_REPORT.md` compliant: cooperative, structural, and legal carapace framing; zero malicious or parasitic mechanics).
- **Simulation Engine:** Temporal event loop using a priority queue sorted by simulated calendar timestamps, driven by a seedable deterministic PRNG (PCG-XSH-RR).
- **Stress Levels:**
  - `LEVEL_0 (Benign/Subsidized)`: Favorable utility net-metering, minimal building inspections, low banking fees.
  - `LEVEL_1 (Standard Bureaucracy)`: Real-world utility rates, standard commercial bank fees, triennial municipal inspections.
  - `LEVEL_2 (Acute Regulatory & Tariff Friction)`: Aggressive utility rate spikes (300%), building code compliance orders with 30-day cure periods, merchant payment holds.
  - `LEVEL_3 (Severe Macroeconomic & Municipal Crisis)`: Utility disconnect notices, civil injunctions, tax liens, bank account freezes, unannounced multi-agency regulatory audits.

```protobuf
syntax = "proto3";
package collective.l7;

message AdversaryConfig {
  enum ThreatLevel {
    LEVEL_0_BENIGN = 0;
    LEVEL_1_STANDARD = 1;
    LEVEL_2_HOSTILE = 2;
    LEVEL_3_SIEGE = 3;
  }
  ThreatLevel threat_level = 1;
  uint64_t random_seed = 2;
  double utility_rate_multiplier = 3;
  bool enable_bank_freezes = 4;
}
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Configurable stress level alters external event frequency deterministically
  Given col-adversaryd configured with ThreatLevel = LEVEL_2_HOSTILE and seed = 1337
  When the simulation advances through 90 virtual calendar days
  Then the DES engine emits exactly 3 surprise utility rate adjustments, 2 building code inspections, and 1 merchant reserve hold
  And all events match the deterministic PRNG sequence.
```

#### Technical Tasks
- [ ] Implement priority-queue Discrete Event Simulation core with PCG-XSH-RR PRNG.
- [ ] Implement stress level parameter controller (Levels 0–3).
- [ ] Add unit test verifying bit-exact identical event sequences given identical random seeds.

---

### User Story 7.2: Legacy Utility Tariff & Curtailment Emulation (*AmeriGrid*, *MetroPower*, *Keystone Gas & Electric*)
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Cooperative Legal Carapace)  
**Story ID:** SS-EP7-002  
**Points:** 7 SP | **Sprint:** 5 | **Priority:** P0 Critical  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-adversaryd` to emulate the tariff structures, standby fees, and curtailment policies of three fictional regional utility providers (*AmeriGrid*, *MetroPower*, *Keystone Gas & Electric*),  
**Benefit:** So that node microgrids learn to anticipate standby fees, peak demand charges, and grid-tie disconnects structurally.

#### Technical Specifications & Architecture
- **Utility Tariff Profiles:**
  - *AmeriGrid:* Imposes high standby/grid-access fees (\$50/kW of installed solar), smart meter remote disconnects, and zero net-metering credits.
  - *MetroPower:* Enforces extreme peak-demand pricing (\$1.85/kWh during 4 PM–9 PM) and power-factor surcharges.
  - *Keystone Gas & Electric:* Mandatory fixed delivery charges and natural gas pipeline maintenance riders regardless of zero consumption.
- **Storage:** Historical billing statements logged to isolated SQLite database at `/var/lib/collective/l7/adversary.db`.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: AmeriGrid peak demand surge and grid-tie curtailment
  Given an edge node connected to the simulated AmeriGrid utility provider under LEVEL_2
  When the node's solar inverter attempts to backfeed surplus power during peak sun
  Then col-adversaryd issues an unannounced grid backfeed curtailment penalty of $120.00
  And logs an OutstandingUtilityBillRecord to the isolated L7 database
  And emits a UtilityDisconnectionWarningEvent over the IPC event bus.
```

#### Technical Tasks
- [ ] Implement billing calculator models for AmeriGrid, MetroPower, and Keystone Gas & Electric.
- [ ] Implement smart meter remote disconnect and curtailment simulation hooks.
- [ ] Add schema for historical utility billing ledger in SQLite.

---

### User Story 7.3: Municipal Code & Zoning Compliance 5-State Finite State Machine (NFPA 855, UPC, NEC)
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Cooperative Legal Carapace)  
**Story ID:** SS-EP7-003  
**Points:** 8 SP | **Sprint:** 5 | **Priority:** P0 Critical  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-adversaryd` to model municipal code compliance through a 5-state finite state machine enforcing NFPA 855 battery limits, UPC greywater rules, and NEC electrical codes,  
**Benefit:** So that nodes navigate inspections, cure periods, and stop-work orders through cooperative legal compliance and physical engineering remedies.

#### Technical Specifications & Architecture
- **5-State Compliance FSM:**
  1. `STATE_INSPECTION_SCHEDULED`: Randomized or complaint-driven notice of inspection.
  2. `STATE_CITATION_ISSUED`: Formal violation notice with 30-day cure clock and civil fines.
  3. `STATE_STOP_WORK_ORDER`: Active red-tag freezing physical construction.
  4. `STATE_ABATEMENT_HEARING`: Formal municipal zoning board hearing.
  5. `STATE_LEGAL_SEVERANCE`: Civil injunction or property lien issued.
- **Regulatory Codes:**
  - *NFPA 855:* Stationary Energy Storage Systems (mandating 3-foot clearance from property lines and residential separation).
  - *Uniform Plumbing Code (UPC):* Permitting requirements on gravity-fed greywater systems.
  - *National Electrical Code (NEC):* Section 690 rapid shutdown and certified contractor requirements.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: NFPA 855 battery distance violation citation lifecycle
  Given a node installing a 30 kWh battery bank within 2 feet of a property boundary
  When col-adversaryd schedules an unannounced municipal building inspection
  Then the FSM transitions to STATE_CITATION_ISSUED and issues an NFPA 855 citation with a 30-day cure period
  If the node operator does not resolve the violation or file a legal appeal within 30 days
  Then the FSM transitions to STATE_STOP_WORK_ORDER and locks affected Layer 4 construction WorkTokens.
```

#### Technical Tasks
- [ ] Implement 5-state municipal compliance FSM with timer triggers.
- [ ] Implement code validation rulebooks for NFPA 855, UPC, and NEC.
- [ ] Add integration test verifying stop-work order freezing corresponding Layer 4 WorkTokens.

---

### User Story 7.4: Legacy Fiat Banking Gateway, Stripe/Plaid Mock Microservice & Reserve Holds
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Cooperative Legal Carapace)  
**Story ID:** SS-EP7-004  
**Points:** 7 SP | **Sprint:** 5 | **Priority:** P0 Critical  
**Persona:** As a Community Steward,  
**Goal:** I want `col-adversaryd` to simulate legacy commercial banking gateways (Stripe, Plaid, ACH) with realistic algorithmic account holds and rolling reserves,  
**Benefit:** So that the community's cooperative legal carapace anticipates cash-flow shocks and insulates internal mutual-credit operations from external banking friction.

#### Technical Specifications & Architecture
- **Mock Microservice:** Embedded HTTP server or direct IPC handler exposing mock Stripe / Plaid endpoints.
- **Simulated Banking Friction:**
  - 20% rolling reserve holds lasting 180 days on new cooperative accounts.
  - Algorithmic merchant account holds triggered by rapid transaction volume surges.
  - 3.5% + \$0.30 interchange fees and \$15.00 dispute chargeback penalties.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Algorithmic payment gateway reserve hold simulation
  Given a community cooperative selling surplus agricultural produce via the legacy proxy
  When daily fiat revenue exceeds $2,500 across 3 consecutive days
  Then col-adversaryd triggers a mock Stripe algorithmic hold retaining 100% of external fiat funds
  And emits an AccountFreezeEvent with a 14-day document demand notice
  And the sovereign stack maintains internal exergy trading with zero operational impact.
```

#### Technical Tasks
- [ ] Implement mock Stripe / Plaid JSON API responder with configurable failure injection.
- [ ] Implement rolling reserve and chargeback dispute logic.
- [ ] Add test asserting that internal Layer 3 ledger balances are 100% insulated from fiat holds.

---

### User Story 7.5: Tripartite Inter-Node Legal Chassis & Anti-Consolidation Firewalls
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Cooperative Legal Carapace)  
**Story ID:** SS-EP7-005  
**Points:** 8 SP | **Sprint:** 5 | **Priority:** P0 Critical  
**Persona:** As a Legal Engineering Specialist,  
**Goal:** I want `col-adversaryd` to wrap inter-node trade in the Tripartite Legal Chassis and enforce Five Anti-Consolidation Firewalls,  
**Benefit:** So that physical resource sharing between nodes is legally recognized as non-taxable cooperative mutual aid, and single-enterprise liability piercing across nodes is prevented.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapter 8 (`08_layer_7_legacy.tex`).
- **Tripartite Legal Chassis:**
  1. *Bilateral Mutual-Aid Agreements (BMAA):* Under Uniform Unincorporated Nonprofit Association Act (UUNAA § 6) and Good Samaritan statutes (gratuitous bailments, UCC Article 2 excluded).
  2. *Subchapter T Shared Services Agreements (SSAs):* Under 26 U.S.C. §§ 1381–1388 (cost-pooling, zero net taxable margin, exemption from commercial carrier licenses under 49 U.S.C. § 13505).
  3. *Federated Land Trust Utility Easements:* Under Uniform Conservation Easement Act (UCEA) (real property servitudes running with the land, zero recurring commercial lease excise tax).
- **Five Anti-Consolidation Firewalls:** Severed boards of directors, separate EINs and bank accounts, absolute ban on cross-collateralization, written arm's-length SSAs, and statutory partnership disclaimers defeating "alter ego" lawsuits.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Inter-node timber trade shielded under Subchapter T SSA
  Given Node A shipping 5 tons of milled cedar timber to Node B across public highways
  When state highway patrol or tax auditor inspects the transport manifest
  Then col-adversaryd generates an automated Subchapter T SSA cost-reimbursement manifest
  And confirms zero commercial brokerage or barter tax liability under 26 U.S.C. § 6045
  And defeats commercial carrier license requirements under 49 U.S.C. § 13505.
```

#### Technical Tasks
- [ ] Implement automated legal manifest generator for BMAA and Subchapter T SSAs.
- [ ] Implement Five Anti-Consolidation Firewall validation rules in database schemas.
- [ ] Add legal audit test asserting that inter-node transfers carry valid tripartite chassis tags.

---

### User Story 7.6: Systemic Legal Blinding, Telemetry Standardization, Subpoena Immunity & Scenario Omega Decoupling
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Cooperative Legal Carapace)  
**Story ID:** SS-EP7-006  
**Points:** 7 SP | **Sprint:** 5 | **Priority:** P0 Critical  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-adversaryd` to enforce Systemic Legal Blinding, standardize external utility telemetry, maintain Subpoena Immunity via Data Non-Existence, and execute Scenario Omega terminal decoupling,  
**Benefit:** So that external reporting remains structurally standard, subpoenas yield zero internal member records, and full thermodynamic autarky cleanly retires the legacy membrane.

#### Technical Specifications & Architecture
- **Systemic Blinding:**
  - *Cellular Independence:* A federated network of independent local co-ops with standard NAICS classifications (1114 agricultural greenhouses, 5415 IT facilities, 2382 electrical contractors).
  - *Telemetry Standardization:* Sanitizes external utility reporting to standard IEEE 1547 net-meter profiles; domestic sub-circuits and battery cell logs remain strictly inside VLAN 10/20 and are never replicated to VLAN 40.
  - *Subpoena Immunity via Data Non-Existence:* Layer 7 holds zero DIDs, cryptographic keys, voting records, or exergy balances (*NAACP v. Alabama* defense).
- **Residual Export Ceiling:**
  $$R_t = \max\left(0, \, A_t - F_t - B_t - C_t - O_t - M_t\right)$$
- **Scenario Omega Decoupling:** Sustaining $\mathcal{S}_{\mathrm{viability}} \ge 1.25$ for 30 consecutive days triggers permanent disconnection:
  1. Trips motorized ATS breaker, permanently disconnecting grid interties.
  2. Closes and unlinks `/run/collective/ipc/l6_l7.sock`.
  3. Cryptographically shreds `/var/lib/collective/l7/adversary.db` via `shred -u`.
  4. Unregisters process from `collective-supervisor`. Layers 1–6 execute perpetually.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Subpoena immunity via data non-existence
  Given a state regulatory subpoena demanding production of internal member identity keys
  When the legal compliance officer queries Layer 7 (col-adversaryd)
  Then col-adversaryd produces statutory corporate filings and payroll logs
  And truthfully certifies that member DIDs, private keys, and exergy ledgers do not exist in L7 storage.

Scenario: Scenario Omega terminal decoupling sequence
  Given an edge node verifying 100% thermodynamic autarky (S_viability >= 1.25) for 30 consecutive days
  When the steward council confirms the Scenario Omega execution intent
  Then col-adversaryd permanently disconnects external utility contactors
  And securely shreds its local database file in <= 100 ms
  And unlinks /run/collective/ipc/l6_l7.sock and terminates
  And Layers 1 through 6 continue executing autonomously with zero memory leaks or crashes.
```

#### Technical Tasks
- [ ] Implement IEEE 1547 net-meter profile sanitizer at VLAN 40 DMZ boundary.
- [ ] Implement Residual Export Ceiling calculator in C++20.
- [ ] Implement atomic cryptographic shredding routine (`shred -u` / memory wipe).
- [ ] Add integration test verifying that Layers 1–6 continue executing following Layer 7 termination.

---

### User Story 7.7: Layer 7 Demographic Shock & Legacy Migration Wave Generator
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Cooperative Legal Carapace)  
**Story ID:** SS-EP7-007  
**Points:** 8 SP | **Sprint:** Dual Sprint 13 | **Priority:** P0 Critical  
**Persona:** As a Bioregional Resilience Planner,  
**Goal:** I want `col-adversaryd`'s Discrete Event Simulation (DES) engine to schedule macroscopic demographic migration waves triggered by simulated external fiat inflation, housing displacement, and regional grid failure events, emitting `MigrationWaveArrivalEvent` packets strictly inward across the Layer 6 boundary (`/run/collective/ipc/l6_l7.sock`),  
**Benefit:** So that sovereign enclaves can stress-test their physical shelter capacity (`SCN-ZETA-LODGE`), commons kitchen caloric reserves (`SCN-GAMMA-KITCH`), and progressive Trust Ring ($R_6 \to R_1$) onboarding protocols (`SS-EP5-007`) under realistic population shocks without violating DMZ VLAN 40 isolation or layer adjacency.

#### Technical Specifications & Architecture
- **SDD & Horizon Alignment:** `SCN-ZETA-LODGE`, `SCN-GAMMA-KITCH`, `SCN-UPSILON-SHOCK`, and Horizon Pillar 5 (*Migration Waves & Legacy NPCs*).
- **Macroeconomic Trigger Model in `col-adversaryd`:**
  - The DES priority queue tracks a regional **Legacy Distress Index** $\mathcal{D}_{\mathrm{legacy}}(t) \in [0.0, 1.0]$ driven by simulated fiat CPI inflation spikes, regional utility blackouts (*AmeriGrid* / *MetroPower* load shedding), and Stress Level settings (`LEVEL_1` through `LEVEL_3`).
  - When $\mathcal{D}_{\mathrm{legacy}}(t)$ crosses configurable thresholds (or at deterministic PRNG intervals seeded by `random_seed`), `col-adversaryd` schedules a `MigrationWaveArrivalEvent` containing cohort count $N_{\mathrm{migrants}} \in [1, 12]$, baseline physiological caloric/water deficit metadata, initial stress index, and probationary trust ring tag (`RING_R6_UNVERIFIED_MIGRANT`).
- **Strict Layer Adjacency Boundary Propagation (`L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI`):**
  - Because `col-adversaryd` is isolated in DMZ VLAN 40, it **never** accesses `/run/collective/ipc/bus.sock`, `/dev/shm/uhai_*`, `col-kmsd`, or Oasis directly.
  - Instead, `col-adversaryd` (L7) emits `MigrationWaveArrivalEvent` **exclusively** inward to Layer 6 (`col-commonsd`) over `/run/collective/ipc/l6_l7.sock` $\to$ Layer 6 compiles the humanitarian intake intent and forwards over `/run/collective/ipc/l5_l6.sock` to Layer 5 (`col-kmsd`, which initializes ephemeral `Ring R6` guest credentials per `SS-EP5-007`) $\to$ Layer 4 (`col-execd` over `/run/collective/ipc/l4_l5.sock`, scheduling baseline $B_t$ shelter/ration workflows) $\to$ Layer 3 (`col-storaged` over `/run/collective/ipc/l3_l4.sock`, logging provisional homeostatic reserves) $\to$ Layer 2 (`col-meshd` over `/run/collective/ipc/l2_l3.sock`) $\to$ UHAI shared-memory ring (`/dev/shm/uhai_telemetry`, `QUALITY_SIMULATED = 0x01`) for Layer 1b (Oasis) consumption.

```cpp
namespace collective::uhai {

// 32-byte payload (alignas(8)) with zero implicit compiler padding (-Wpadded -Werror)
struct alignas(8) MigrationWaveArrivalEvent {
    uint64_t timestamp_hlc;          // 8B: HLC timestamp of wave arrival
    uint32_t wave_id;                // 4B: Monotonic wave sequence identifier
    uint32_t deterministic_seed;     // 4B: PRNG sub-seed for reproducible cohort traits
    float    legacy_distress_index;  // 4B: Macro distress scalar [0.0 - 1.0]
    float    initial_stress_mean;    // 4B: Baseline cohort stress scalar [0.50 - 0.85]
    float    caloric_deficit_kcal;   // 4B: Immediate per-capita caloric need upon arrival
    uint16_t cohort_size;            // 2B: Number of arriving migrants (e.g., 1..12)
    uint8_t  initial_trust_ring;     // 1B: Defaults to 6 (Ring R6: Unverified Migrant)
    uint8_t  trigger_reason_code;    // 1B: 1=FiatInflation, 2=GridBlackout, 3=ClimateShock
};
static_assert(sizeof(MigrationWaveArrivalEvent) == 32,
              "MigrationWaveArrivalEvent must remain exactly 32 bytes.");

// 64-byte cache-line-isolated UHAI / IPC slot wrapper (alignas(64))
struct alignas(64) MigrationWaveSlot {
    MigrationWaveArrivalEvent event; // 32B payload
    uint8_t                   pad[32]; // 32B explicit cache-line padding
};
static_assert(sizeof(MigrationWaveSlot) == 64,
              "MigrationWaveSlot must occupy exactly 1 cache line (64 bytes).");

} // namespace collective::uhai
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Regional grid failure and fiat inflation spike trigger a deterministic Migration Wave across strict layer adjacency
  Given col-adversaryd running in DMZ VLAN 40 with ThreatLevel = LEVEL_2_HOSTILE and random_seed = 42
  When a simulated regional utility blackout and fiat inflation event pushes Legacy Distress Index >= 0.65
  Then col-adversaryd schedules and emits a 32-byte MigrationWaveArrivalEvent (wrapped in a 64-byte MigrationWaveSlot) with initial_trust_ring = 6 (R6)
  And dispatches the event exclusively over l6_l7.sock to col-commonsd (L6) with zero direct access to bus.sock or /dev/shm/uhai_telemetry
  And the event propagates strictly inward across L6 (col-commonsd) -> L5 (col-kmsd, provisioning Ring R6 DIDs per SS-EP5-007) -> L4 (col-execd) -> L3 (col-storaged) -> L2 (col-meshd) -> UHAI (/dev/shm/uhai_telemetry, QUALITY_SIMULATED = 0x01) for Layer 1b SITL consumption.
```

#### Technical Tasks
- [ ] Implement `LegacyDistressIndex` accumulator and stochastic wave scheduler inside `col-adversaryd`'s DES priority queue.
- [ ] Define and serialize the 32-byte `MigrationWaveArrivalEvent` (`alignas(8)`) and 64-byte `MigrationWaveSlot` (`alignas(64)`) in `core/1_simulation/src/uhai/uhai_ring_buffer.hpp` verified under `-Wpadded -Werror`.
- [ ] Wire strict inward adjacency handlers across `l6_l7.sock` (`col-commonsd`) $\to$ `l5_l6.sock` (`col-kmsd` `SS-EP5-007`) $\to$ `l4_l5.sock` (`col-execd`) $\to$ `l3_l4.sock` (`col-storaged`) $\to$ `l2_l3.sock` (`col-meshd`) $\to$ `/dev/shm/uhai_telemetry`.
- [ ] Add deterministic Catch2 unit and integration tests in `test_migration_waves.cpp` verifying bit-exact `MigrationWaveArrivalEvent` emission for a fixed PRNG seed and full `L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI` traversal.

---

## 12. Epic 8: Cross-Stack Headless Verification, Chaos Engineering & Six Inviolable Quality Gates (G1–G6)

### User Story 8.1: Headless Deterministic Multi-Node Test Runner & Cross-Platform State Hash Parity
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-001  
**Points:** 7 SP | **Sprint:** 6 | **Priority:** P0 Critical  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want a high-speed headless CLI test harness (`col-test-runner`) executing multi-node topologies across thousands of ticks,  
**Benefit:** So that distributed protocol behavior and cross-platform state hash parity are validated without graphics or manual testing.

#### Technical Specifications & Architecture
- **Harness:** `tools/sovereign_runner.cpp` executing virtual nodes in a single process or multi-process test harness.
- **Parity Check:** Compares SHA-256 state root hashes of Layer 3 ledgers and Layer 4 state tries across Linux ARM64, Linux x86_64, and Wasm runtimes.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Bit-exact cross-platform state hash parity
  Given a 10-node simulated cluster executing 10,000 deterministic ticks under identical random seeds
  When the test harness computes the final Layer 3 Merkle root on ARM64 and x86_64
  Then both architectures output identical 32-byte SHA-256 hashes
  And execution throughput exceeds 1,000 ticks/second on a single CPU core.
```

#### Technical Tasks
- [ ] Implement headless multi-node test harness in C++20.
- [ ] Add SHA-256 Merkle root accumulator across ledger accounts.
- [ ] Configure CI workflow testing cross-platform parity on ARM64 and x86_64.

---

### User Story 8.2: Automated Six Inviolable Quality Gates (G1–G6) Verification Test Suite
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-002  
**Points:** 8 SP | **Sprint:** 6 | **Priority:** P0 Critical  
**Persona:** As a Quality Engineer,  
**Goal:** I want an automated test suite verifying compliance with the Six Inviolable Quality Gates (G1–G6) across all 24 canonical SDD scenarios,  
**Benefit:** So that no code or workflow can be deployed to physical edge nodes without passing all mathematical, physical, and legal invariants.

#### Technical Specifications & Architecture
- **The Six Quality Gates:**
  - **G1: Thermodynamic Conservation & Physical Grounding:** Verifies $\Delta E \ge \text{work}$, mass/energy balance, and ATECC608A hardware signature.
  - **G2: Asynchronous Delay-Tolerant Convergence:** Verifies that partitioned nodes merge deterministically within $\le 5.0\text{ seconds}$ post-healing.
  - **G3: Provable Ecological & Resource Ceilings:** Verifies that $\lambda_{ERC} \to \infty$ prevents over-extraction and preserves $F_{\mathrm{eco}}$ floors.
  - **G4: Zero-Knowledge & Web of Trust Boundary:** Verifies BBS+ selective disclosure, pairwise DIDs, and $O(D)$ loop-free BPVP routing.
  - **G5: Tripartite Legal Chassis & Subpoena Immunity:** Verifies that inter-node transfers carry BMAA/SSA tags and Layer 7 holds zero internal member data.
  - **G6: Strict Physical Airgap & Anti-Bleed Purity:** Verifies that `QUALITY_SIMULATED` frames are dropped at edge relays and zero game mechanics bleed into daemons.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Automated Six Quality Gates validation on release candidate
  Given the complete Sovereign Stack test harness running against the 24 canonical scenarios
  When scripts/verify_quality_gates.sh executes in CI
  Then Gates G1 through G6 return 100% PASS across all 24 scenarios
  And any single gate failure blocks binary release generation.
```

#### Technical Tasks
- [ ] Implement `scripts/verify_quality_gates.sh` automated test orchestrator.
- [ ] Implement specific verification probes for Gates G1 through G6.
- [ ] Integrate Quality Gates as mandatory blocking step in edge CI/CD pipeline.

---

### User Story 8.3: Scenario Rho Adversarial Full-Stack Attack, Fuzzing & RF Jamming Suite
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-003  
**Points:** 7 SP | **Sprint:** 6 | **Priority:** P0 Critical  
**Persona:** As a Quality Engineer,  
**Goal:** I want an automated adversarial stress suite injecting sensor spoofing, RF jamming, double-spends, and Sybil attacks simultaneously,  
**Benefit:** So that vulnerabilities and protocol edge cases are identified and eliminated before live bioregional deployment.

#### Technical Specifications & Architecture
- **Attack Vectors:**
  1. *Sensor Spoofing:* Injects impossible thermodynamic values (e.g. 50 kW from a 3 kW solar array).
  2. *RF Jamming:* Simulates 99% packet drop and arbitrary delay over LoRa channels.
  3. *Double-Spend Attack:* Attempts concurrent exergy credit spends across partitioned nodes.
  4. *Sybil Injection:* Floods mesh with 1,000 synthetic DIDs attempting governance capture.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Scenario Rho full-stack defense verification
  Given an active 5-node cluster subjected to simultaneous Scenario Rho attack vectors
  When the test suite executes for 60 minutes
  Then sensor spoofing is quarantined by spatial cross-checking without halting the ledger
  And double-spends are deterministically rejected upon partition healing
  And zero crashes, assertion failures, or memory corruptions occur.
```

#### Technical Tasks
- [ ] Implement synthetic fault injection framework for IPC socket streams.
- [ ] Implement LibFuzzer / AFL++ harnesses for all UDS socket input decoders.
- [ ] Add continuous CI stress test running Scenario Rho for 30 minutes per build.

---

### User Story 8.4: 72-Hour Network Partition & Netem Delay-Tolerant Resynchronization Test
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-004  
**Points:** 6 SP | **Sprint:** 6 | **Priority:** P0 Critical  
**Persona:** As a Quality Engineer,  
**Goal:** I want an automated test harness simulating a 72-hour mesh network partition using Linux network namespaces and netem,  
**Benefit:** So that delay-tolerant packet queues and CRDT reconciliation are rigorously validated against real-world radio blackouts.

#### Technical Specifications & Architecture
- **Environment:** Isolated Linux network namespaces (`ip netns`) with traffic control packet loss (`tc netem loss 100%`).
- **Verification:** Nodes operate autonomously offline for 72 simulated hours; upon link restoration, all nodes reconcile to identical state within $\le 5.0\text{ seconds}$.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: 72-hour network partition recovery
  Given two nodes partitioned via netem with 100% packet loss for 72 simulated hours
  When each node executes 500 local tasks and exergy minting transactions
  When netem restores the link with 200 ms latency and 5% jitter
  Then col-meshd drains the persistent DTN spool without buffer overrun
  And col-storaged reconciles all CRDT deltas to identical SHA-256 state hashes in <= 5.0 seconds.
```

#### Technical Tasks
- [ ] Implement test script setting up `ip netns` and `tc netem` partitions.
- [ ] Implement state hash comparison probe verifying post-healing convergence.
- [ ] Add CI test gate ensuring memory usage does not exceed 320 MB RSS during partitions.

---

### User Story 8.5: Scenario Omega End-to-End Autarky Verification Gate & Memory Leak Audit
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-005  
**Points:** 6 SP | **Sprint:** 6 | **Priority:** P0 Critical  
**Persona:** As a Quality Engineer,  
**Goal:** I want an automated end-to-end test asserting that Scenario Omega unmounts Layer 7 cleanly without degrading Layers 1 through 6,  
**Benefit:** So that the permanent decoupling of the legacy membrane is proven crash-safe and leak-free.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Clean unmount and memory leak verification during Scenario Omega
  Given a fully integrated 7-layer node executing under AddressSanitizer (ASan) and Valgrind
  When the autarky threshold triggers Scenario Omega terminal decoupling
  Then col-adversaryd terminates within 100 ms and /var/lib/collective/l7/adversary.db is zeroized
  And Layers 1 through 6 continue processing telemetry, mesh traffic, and workflows for 1,000 additional ticks
  And Valgrind confirms exactly 0 bytes leaked and 0 file descriptor leaks.
```

#### Technical Tasks
- [ ] Implement automated ASan/Valgrind test script executing Scenario Omega.
- [ ] Add assertions checking open file descriptors via `/proc/<pid>/fd/`.
- [ ] Add CI gate enforcing zero memory leaks on terminal decoupling.

---

### User Story 8.6: Continuous Anti-Bleed CI Linter (`scripts/lint_anti_bleed.sh`) & cgroups Enforcement
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-006  
**Points:** 7 SP | **Sprint:** 6 | **Priority:** P0 Critical  
**Persona:** As a Quality Engineer,  
**Goal:** I want a compile-time static analysis tool and CI linter enforcing the Scope Boundary Rules (SBR-1 through SBR-5), `-Wpadded -Werror` cache-line isolation, and Linux cgroups memory ceilings,  
**Benefit:** So that no developer or agent can inadvertently re-introduce game mechanics, monolithic threading, false-sharing memory layouts, or unbounded memory leaks.

#### Technical Specifications & Architecture
- **Script:** `scripts/lint_anti_bleed.sh` executing in CI:
  1. Forbids imports of `oasis/*`, `SDL2/*`, `SDL3/*`, `webgpu/*`, `dawn/*` inside `src/core/` and `src/daemons/`.
  2. Forbids occurrences of game engine threading, frame budget ceilings, volumetric world types, and player kinematics in stack documentation and headers, as well as deprecated adversarial lore terminology flagged in `docs/BOOK_TEAM_DRIFT_REPORT.md`.
  3. Validates compile-time static assertions and `-Wpadded -Werror` cache-line-isolated memory layouts in `core/1_simulation/src/uhai/uhai_ring_buffer.hpp`:
     - `static_assert(sizeof(TelemetrySample) == 32)` & `static_assert(sizeof(TelemetrySlot) == 64)`
     - `static_assert(sizeof(SharedRingHeader) == 192)` & `static_assert(TOTAL_SHM_SIZE == 65728)`
     - `static_assert(sizeof(SpatialIntentPayload) == 264)` & `static_assert(sizeof(SpatialIntentSlot) == 320)`
     - `static_assert(sizeof(FlowContract) == 64)` & `static_assert(sizeof(FlowContractSlot) == 64)`
     - `static_assert(sizeof(MigrationWaveArrivalEvent) == 32)` & `static_assert(sizeof(MigrationWaveSlot) == 64)`
     - `static_assert(sizeof(ActuatorCommand) == 80)` & `static_assert(sizeof(StateDeltaOp) == 24)`
  4. Enforces Linux systemd / cgroups ceilings: total 8-daemon stack $\le 320\text{ MB}$ RSS, Layer 7 $\le 24\text{ MB}$ RSS, and zero `l4_l7.sock` descriptor existence.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: CI linter catches prohibited game mechanic import or UHAI struct padding drift
  Given a pull request introducing client graphics or breaking -Wpadded -Werror alignment in uhai_ring_buffer.hpp
  When scripts/lint_anti_bleed.sh and the CMake build run in CI
  Then the build immediately fails with a descriptive boundary or static_assert violation error
  And blocks merge until the code conforms to the Scope Boundary Rules and cache-line layout invariants.
```

#### Technical Tasks
- [ ] Implement `scripts/lint_anti_bleed.sh` using `ripgrep` and AST regexes.
- [ ] Verify all `static_assert` and `alignas(64)` slot invariants in `core/1_simulation/src/uhai/uhai_ring_buffer.hpp` under `-Wpadded -Werror`.
- [ ] Configure cgroups v2 resource ceiling profiles for all 8 systemd units (`col-telemetryd`, `col-meshd`, `col-storaged`, `col-execd`, `col-kmsd`, `col-commonsd`, `col-adversaryd`, `col-gatewayd`).

---

## 13. Epic 9: Asynchronous Node Proxy Federation, Standardized Temporal Scaling (`1x/2x/5x/10x`) & Layer 4/5 BPMN Workflow Scheduling (`col-meshd`, `col-storaged`, `col-execd`, `col-kmsd`)

### User Story 9.1: Asynchronous Node Proxy State Synchronization & Phase 1/2 Storage Horizon (`col-meshd` & `col-storaged`)
**Epic:** Epic 9 (Asynchronous Node Proxy Federation & Layer 4/5 BPMN Orchestration)  
**Story ID:** SS-EP9-001  
**Points:** 8 SP | **Sprint:** Dual Sprint 14 | **Priority:** P0 Critical  
**Persona:** As a Federated Enclave Steward on `Arcadia (server-01)`,  
**Goal:** I want real-time lockstep multiplayer deprecated in favor of an **Asynchronous Node Proxy architecture** where every peer player on the server is represented locally as an autonomous NPC Node Proxy whose configuration parameters, production outputs, and thermodynamic states synchronize asynchronously (via Phase 1 GitHub/static JSON feeds `core/3_ledger/node_proxies.json` and Phase 2 Layer 3 `col-storaged` CRDT `StateDeltaOp` replication),  
**Benefit:** So that players inhabit a deeply populated, consistent world driven by real human exergy without real-time latency, collision-resolution nightmares, or hard-wall epoch freezes.

#### Technical Specifications & Architecture
- **SDD & Horizon Alignment:** `SCN-ETA-COPPICE`, `SCN-XI-ARCH`, `SCN-BETA-TRANS`, `core/3_ledger/LEDGER_SPEC.md`, and Sprint 14 Pillar 1 (*The Asynchronous Node Proxy*).
- **Two-Phase Storage Horizon (Phase 1 Legacy Bridge $\to$ Phase 2 Layer 3 CRDT Distributed Storage):**
  - **Phase 1 (Immediate Bridge):** Peer Node Proxy configurations, thermodynamic trajectories (`net_kwh_per_day`, `net_water_l_per_day`, `net_kcal_per_day`), W3C DIDs (`did:mesh:*`), and resource deficits are published and ingested via `core/3_ledger/node_proxies.json` (leveraging GitHub / static JSON hosting from the legacy world).
  - **Phase 2 (Sovereign Horizon):** Transitions seamlessly to **Layer 3 (`col-storaged`, `core/3_ledger/LEDGER_SPEC.md`)** local-first CRDT distributed storage (`StateDeltaOp`, `CRDTEscrowLock`, and `mesh:ValueToken` 1:1 surplus exergy reflection) replicated over Layer 2 (`col-meshd`).
- **Headless Coarse Cadastral CRDT Deltas (SBR-1 & SBR-3 Compliant):**
  - Zero GPU meshes, shader uniforms, or camera matrices ever cross `l2_l3.sock`; SITL clients (Oasis) independently project peer Node Proxy parameters into local autonomous NPC Node Proxies.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Asynchronous Node Proxy synchronization without lockstep multiplayer stalls
  Given Node 0 operating in its localized instance alongside Asynchronous Node Proxies Node 01 (Lot 404), Node 02 (Lot 406), and Node 03 (Lot 408)
  When peer Node Proxy snapshots update via core/3_ledger/node_proxies.json (Phase 1) or 24-byte StateDeltaOp CRDT records (Phase 2)
  Then Node 0's local simulation updates the production outputs and resource deficits of those autonomous NPC Node Proxies deterministically
  And zero real-time multiplayer lockstep barriers or rendering symbols enter col-meshd or col-storaged.
```

---

### User Story 9.2: Standardized Temporal Mechanics (`1x, 2x, 5x, 10x`) & HLC `FlowContract` Pacing (`col-execd` & `col-storaged`)
**Epic:** Epic 9 (Asynchronous Node Proxy Federation & Layer 4/5 BPMN Orchestration)  
**Story ID:** SS-EP9-002  
**Points:** 7 SP | **Sprint:** Dual Sprint 14 | **Priority:** P0 Critical  
**Persona:** As a Distributed Simulation & Systems Architect,  
**Goal:** I want simulation time-scaling multipliers strictly standardized to **`Pause (0x)`, `1x`, `2x`, `5x`, and `10x`** (deprecating `100x` and hard epoch freeze walls), while `col-execd` and `col-storaged` integrate canonical 64-byte `FlowContract` streams over HLC logical ticks,  
**Benefit:** So that players can smoothly fast-forward through long thermodynamic processes (biochar pyrolysis, crop maturation, rainwater catchment) with predictable tick-rate calculations and bit-exact exergy conservation.

#### Technical Specifications & Architecture
- **Standardized Multipliers (`{0, 1, 2, 5, 10}`):**
  - `SetTimeWarpMultiplier` clamps strictly to `{0, 1, 2, 5, 10}`.
  - All bilateral resource flows (`FlowContract` in `uhai_ring_buffer.hpp`) and Asynchronous Node Proxy trajectories integrate over logical HLC ticks $\tau_{\mathrm{logical}}$, producing identical conserved thermodynamic totals across `1x`, `2x`, `5x`, and `10x`.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Standardized 1x, 2x, 5x, and 10x time scaling preserves bit-exact thermodynamic integrals
  Given a node running local simulation across 1x, 2x, 5x, and 10x multipliers
  When 1,000 logical ticks of homeostasis, FlowContract integration, and Asynchronous Node Proxy stepping execute
  Then SetTimeWarpMultiplier strictly enforces {0, 1, 2, 5, 10}
  And the conserved caloric, water, and exergy integrals at 2x, 5x, and 10x match 1x bit-for-bit.
```

---

### User Story 9.3: Layer 4 BPMN 2.0 Demand Routing, Advance Layer 5 Gate Locking & Multi-Day Temporal Workflow Scheduler (`col-execd`, `col-kmsd`, `col-storaged`)
**Epic:** Epic 9 (Asynchronous Node Proxy Federation & Layer 4/5 BPMN Orchestration)  
**Story ID:** SS-EP9-003  
**Points:** 7 SP | **Sprint:** Dual Sprint 14 | **Priority:** P0 Critical  
**Persona:** As a Dual-Control Enclave Steward (`Node 0`),  
**Goal:** I want Layer 4 (`core/4_orchestrator/ORCHESTRATOR_SPEC.md`, `col-execd`) to route resource deficits from peer NPC Node Proxies into my actionable task log as high-priority **Collective BPMN Bounties**, and I want a **Multi-Day Temporal Workflow Scheduler (`core/4_orchestrator/web/`)** that locks **Layer 5 Governance gates (`core/5_governance/GOVERNANCE_SPEC.md`, `col-kmsd`)** and **Layer 3 `CRDTEscrowLock` (`core/3_ledger/LEDGER_SPEC.md`, `col-storaged`)** in advance across Days 1–5,  
**Benefit:** So that I experience true dual control—managing my physical homestead in the 3D simulation while orchestrating multi-day workflows and fulfilling communal demands in *The Collective*.

#### Technical Specifications & Architecture
- **Canonical Spec Alignment:**
  - **Layer 3 (`core/3_ledger/LEDGER_SPEC.md`):** `mesh:ValueToken` is a strict 1:1 reflection of surplus `mesh:Exergy` (`1.0 VT == 1.0 kWh`); `CRDTEscrowLock` reserves physical capacity for scheduled BPMN workflows.
  - **Layer 4 (`core/4_orchestrator/ORCHESTRATOR_SPEC.md`):** Declarative BPMN 2.0 state machine runner (`core/4_orchestrator/workflows/collective_homestead_bounties.bpmn.json`) executing the 6-step sequence: `1. Ingest (L6) -> 2. Gate (L5) -> 3. Validate Physics (L1b & L2) -> 4. Escrow (L3) -> 5. Actuate (L2/L1b) -> 6. Settle (L3)`.
  - **Layer 5 (`core/5_governance/GOVERNANCE_SPEC.md`):** Evaluates W3C DIDs (`did:key` / `did:mesh`), Verifiable Credentials (`SkillCredential`), and weighted Trust Ring edges (`core/5_governance/trust_gates.json`).
- **Layer 4/5 Companion App (`core/4_orchestrator/web/index.html` & `bpmn_scheduler.js`):**
  - Provides the interactive BPMN 2.0 workflow graph, Asynchronous Node Proxy deficit monitor, and 5-Day Temporal Workflow Scheduler grid synchronized with the 3D simulation.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Peer Node Proxy resource deficit routes as a BPMN Bounty and settles ValueTokens upon physical completion
  Given Node 02 (Lot 406) reports a -1.6 kWh/day electrical deficit and Node 03 (Lot 408) reports a -1,200 kcal/day caloric deficit
  When the Layer 4 BPMN orchestrator evaluates collective_homestead_bounties.bpmn.json and locks Layer 5 gates
  Then high-priority Collective Bounties populate Node 0's actionable task log
  And when Node 0 completes the corresponding physical task or scheduled Day/Slot workflow, the BPMN state machine transitions to SETTLE (L3) and credits mesh:ValueToken (1:1 kWh) and Layer 5 Trust weight.
```

---

## 14. Quality Assurance Matrix, Definition of Ready, Definition of Done & Ratification Sign-Off

### 14.1. Infrastructure Definition of Ready (DoR) — 6 Criteria
A user story in the Sovereign Stack is marked **READY** for sprint backlog ingestion if and only if it satisfies all 6 criteria:
1. **Mathematical / Protocol Formulation:** All equations (minting, decay, routing, clearing, HLC epoch pacing) are explicitly stated with defined units and variable constraints.
2. **Interface Alignment:** Inter-layer IPC messages and binary wire formats (Cap'n Proto / Protobuf / C++20 standard layout) are defined with explicit byte alignments.
3. **Hardware / OS Specificity:** Target hardware registers, serial baud rates, Linux kernel subsystems (`epoll`, `netlink`, `cgroups`), or RTOS APIs are specified.
4. **Hermetic Test Scenario:** A Gherkin acceptance scenario (Given / When / Then) is articulated with quantitative pass/fail thresholds.
5. **Anti-Bleed & Drift Boundary Conformance:** Verified free of client-side graphics rendering, game loop threads, Oasis gameplay semantics, and deprecated adversarial lore terminology flagged in `docs/BOOK_TEAM_DRIFT_REPORT.md`.
6. **SDD & Horizon Scenario Tracing:** Direct link to one of the 24 canonical SDD scenarios in `docs/scenarios/` or the 6 Horizon Pillars in `HORIZON_ROADMAP.md` establishing end-to-end provenance.

---

### 14.2. Infrastructure Definition of Done (DoD) — 10 Strict Invariants
A user story or pull request in the Sovereign Stack is marked **DONE** if and only if it satisfies all 10 invariants:
1. **Autonomous Daemon Conformance:** Operates within an autonomous daemon (`col-*d`) with its own event loop; zero coupling to monolithic thread pools or game loops.
2. **Dedicated Storage Strategy:** State is persisted to the layer's dedicated storage engine; zero unauthorized cross-database foreign keys.
3. **Strict Adjacency & Intent U-Turn Enforced:** State transitions communicate exclusively with adjacent layers ($L_{N \pm 1}$) across Unix Domain Sockets; zero `l4_l7.sock`; external human intents enter exclusively via Layer 6 (`POST /api/v1/l6/intent`).
4. **Hermetic Unit Test Coverage:** $\ge 90\%$ branch test coverage with 100% mocked hardware and network dependencies.
5. **Consumer-Driven Contract Testing:** All IPC interfaces verified against versioned Protobuf / Cap'n Proto / C++20 wire schemas with zero breaking changes.
6. **Zero Graphics / Headless Compliance:** Compiles and executes cleanly without display servers, WebGPU, SDL2/SDL3, or windowing libraries.
7. **Strict Memory & CPU Bounding:** Conforms to cgroups resource ceilings (total 8-daemon stack $\le 320\text{ MB}$ RSS; steady-state CPU $\le 8\%$ on ARM64 Cortex-A72).
8. **Crash Consistency & RTO $\le 500\text{ ms}$:** Recovers state from WAL upon abrupt `SIGKILL` in under 500 ms with zero corruption.
9. **Zero Memory Leaks:** 0 bytes leaked over 72 hours of continuous synthetic traffic verified via AddressSanitizer and Valgrind.
10. **Anti-Bleed & Outward-Only SITL Verification:** Passes `scripts/lint_anti_bleed.sh` with zero violations of Scope Boundary Rules SBR-1 through SBR-5, confirming Oasis is strictly an outward-only producer of `QUALITY_SIMULATED (0x01)` telemetry.

---

### 14.3. Edge Hardware Deployment Qualification Matrix

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              EDGE HARDWARE DEPLOYMENT QUALIFICATION MATRIX                             │
├──────────────────────┬──────────────────────────────┬──────────────────────────┬───────────────────────┤
│ Target Platform      │ Hardware Specifications      │ Operating System         │ Primary Daemons Run   │
├──────────────────────┼──────────────────────────────┼──────────────────────────┼───────────────────────┤
│ **Tier 1: Master     │ Raspberry Pi CM4 / Rockchip  │ Alpine Linux / Debian 12 │ All 8 Daemons (L1–L7  │
│ Bioregional Hub**    │ RK3588 (4-8 Core ARM64, 4GB) │ (kernel 6.6+ PREEMPT_RT) │ + `col-gatewayd`:8443)│
├──────────────────────┼──────────────────────────────┼──────────────────────────┼───────────────────────┤
│ **Tier 2: Off-Grid   │ Allwinner H616 / BCM2711     │ Buildroot Minimal Linux  │ Daemons L1–L4;        │
│ Solar / Shed Node**  │ (Quad ARM64, 1GB RAM, eMMC)  │ (Read-only rootfs)       │ Headless microgrid hub│
├──────────────────────┼──────────────────────────────┼──────────────────────────┼───────────────────────┤
│ **Tier 3: Peripheral │ ESP32-S3 / STM32L4+          │ Zephyr RTOS / FreeRTOS   │ Bare-metal HAL (L1);  │
│ Sensor / Valve Tag** │ (Tensilica / ARM-M4, 512KB)  │ bare-metal C++20         │ Reticulum LoRa bridge │
└──────────────────────┴──────────────────────────────┴──────────────────────────┴───────────────────────┘
```

---

### 14.4. Six Inviolable Quality Gates (G1–G6) Acceptance Specification

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                SIX INVIOLABLE QUALITY GATES SPECIFICATION                              │
├──────┬───────────────────────────────┬──────────────────────────────────┬──────────────────────────────┤
│ Gate │ Gate Title & Focus            │ Inviolable Verification Rule     │ Automated Gate Test Method   │
├──────┼───────────────────────────────┼──────────────────────────────────┼──────────────────────────────┤
│ **G1**│ **Thermodynamic Grounding**   │ Work tokens minted only with net │ Mass/energy balance assert:  │
│      │ (Physical Conservation)       │ exergy addition & ATECC608A sig  │ Delta E >= work performed    │
├──────┼───────────────────────────────┼──────────────────────────────────┼──────────────────────────────┤
│ **G2**│ **Asynchronous Convergence**  │ Partitioned nodes & Epoch walls  │ 72-hr netem + Epoch Barrier  │
│      │ (Delay-Tolerant & Epoch Sync) │ merge deterministically via CRDT │ converges to identical hash  │
├──────┼───────────────────────────────┼──────────────────────────────────┼──────────────────────────────┤
│ **G3**│ **Ecological Ceilings**       │ Extraction halted by protocol    │ lambda_ERC -> inf blocks     │
│      │ (Carrying Capacity Defense)   │ before carrying capacity breach  │ Layer 4 task execution       │
├──────┼───────────────────────────────┼──────────────────────────────────┼──────────────────────────────┤
│ **G4**│ **Web of Trust & U-Turn**     │ Pairwise DIDs, R6->R1 onboarding │ BBS+ ZKP <= 15 ms; zero      │
│      │ (Governance Supremacy)        │ & strict Intent U-Turn enforced  │ l4_l7.sock; L2-L5/L7 RO hood │
├──────┼───────────────────────────────┼──────────────────────────────────┼──────────────────────────────┤
│ **G5**│ **Cooperative Legal Shield**  │ Inter-node trade wrapped in BMAA │ L7 database inspection: zero │
│      │ (Subpoena Immunity)           │ and SSAs; L7 holds zero DIDs     │ internal keys/ballots exist  │
├──────┼───────────────────────────────┼──────────────────────────────────┼──────────────────────────────┤
│ **G6**│ **Airgap & Outward-Only SITL**│ Oasis is outward-only producer   │ Simulated packets (0x01)     │
│      │ (Domain Isolation)            │ (0x01); cannot drive edge relays │ dropped at HAL; linter zero  │
└──────┴───────────────────────────────┴──────────────────────────────────┴──────────────────────────────┘
```

---

### 14.5. Strategic Realignment & Horizon Council Ratification Sign-Off
The Strategic Realignment & Horizon Council concludes its deliberations for **Version 4.0.0-RELEASE**. All Sprints 1–10 baseline capabilities, Companion App V2.0 (`col-gatewayd`) Intent U-Turn invariants, `BOOK_TEAM_DRIFT_REPORT.md` cooperative carapace alignments, Legacy Migration Wave onboarding protocols (`SS-EP7-007`, `SS-EP5-007`), and `Arcadia (server-01)` Mandatory Milestone Epoch Synchronization protocols (`SS-EP9-001` through `SS-EP9-003`) have been ratified across all stakeholder disciplines.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                        ARCHITECTURAL COUNCIL RATIFICATION VOTE (V4.0.0-RELEASE)                  │
├────────────────────────────────┬───────────────────────────────┬─────────────────┬───────────────┤
│ Persona                        │ Document / Milestone          │ Disciplinary OK │ Final Ballot  │
├────────────────────────────────┼───────────────────────────────┼─────────────────┼───────────────┤
│ Product Owner (`explorer_po_v4`) 9 Epics, 50 Stories, Sprints  │ APPROVED        │ UNANIMOUS YES │
│ Sovereign Specialist (`sov_v4`)  Book Fidelity, U-Turn, R6->R1 │ APPROVED        │ UNANIMOUS YES │
│ Quality Engineer (`qe_v4`)       Gates G1–G6, Epoch Parity     │ APPROVED        │ UNANIMOUS YES │
│ Stack Engineer (`stack_v4`)      8 Daemons (+gatewayd), HLC    │ APPROVED        │ UNANIMOUS YES │
│ Oasis Architect (`oasis_v4`)     Outward-Only SITL (0x01), SoC │ APPROVED        │ UNANIMOUS YES │
├────────────────────────────────┴───────────────────────────────┴─────────────────┴───────────────┤
│                                  # Unanimous Consensus Reached                                   │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

**Ratified:** 2026-10-14  
**Effective Baseline:** Version 4.0.0-RELEASE (`SOVEREIGN_STACK_BACKLOG.md`)  
**Signed by:**
- *Product Owner / Orchestrator (`explorer_po_v4`)*
- *Sovereign Stack Specialist (`explorer_sovereign_v4`)*
- *Quality Engineer (`explorer_qe_v4`)*
- *Stack Engineer (`explorer_stack_v4`)*
- *Oasis Boundary Architect (`explorer_oasis_v4`)*



---

## 15. V3.1 Architectural Realignment — Autopoietic Self-Hosting (`SCN-ZERO-SOFTWARE`), Dual-Fidelity L6 Blueprints, $R_0$ Baseline & \$20k Genesis Procurement Pipeline

In alignment with `docs/OASIS_GDD.md` (Section 12) and `PRODUCT_BACKLOG_V6.md` (Section 8), Version 4.0.0-RELEASE incorporates the four constitutional laws of the **V3.1 Architectural Realignment**:

### 15.1. Law 1 — Scenario Zero (`SCN-ZERO-SOFTWARE`) Autopoietic Self-Hosting
* **Canonical Manifest (`core/6_intent/blueprints/scn_zero_daemons_manifest.jsonld`):** The 8 Sovereign Stack daemons (`col-telemetryd`, `col-meshd`, `col-storaged`, `col-execd`, `col-kmsd`, `col-commonsd`, `col-gatewayd`, `col-adversaryd`) and `oasis_engine` are governed, built, audited, and canary-tested by the stack itself.
* **Constitutional Gate Invariants:** Disassembly and CI gates enforce `floating_point_opcodes_detected == 0` across deterministic Layer 2–5 paths, `layer_skipping_imports_detected == 0`, zero `l4_l7.sock`, 3-node reproducible SHA-256 binary consensus (`x86_64`, `ARM64`, `RISC-V`), and a mandatory `10,000`-tick Oasis shadow-twin canary soak before Layer 5 deployment ratification.

### 15.2. Law 2 — Feeding Oasis Feeds The Collective (Dual-Fidelity Real Blueprints)
* **Unified Layer 6 Blueprint Artifacts (`core/6_intent/blueprints/*.jsonld`):** Each blueprint in `col-commonsd` (`bp_alpha_stage1_solar_array.jsonld`, `bp_zero_stage1_homelab_workstation.jsonld`, `bp_gamma_stage1_hearth_and_garden.jsonld`) pairs real-world physical BOMs, fixed-point SI sensor bounds, and Layer 5 legal/acoustic policies (`<= 550 cB` workshop, `< 380 cB` homelab, `>= 347150 mK` Bigelow culinary lethality) with an `oasisSimulationProjection` (`MACRO_ABSTRACTED`, `MULTI_STAGE_SUBSET`, `FULL_STOICHIOMETRIC_HIL`).

### 15.3. Law 3 — The Constitutional Baseline Right to Exist ($R_0 / B_{\mathrm{baseline}}$)
* **Unconditional Survival Floor (`col-kmsd` & `col-storaged`):** Layer 5 and Layer 3 guarantee every person's unconditional right to exist—providing humble, nourishing **pea soup and bean stew**, a **warm shower and heating**, **sanitation**, and **emergency / failing-health care** with zero token or fiat pricing (`domesticCareTokenMinting == 0`), enabling households to immediately scrap legacy grocery and baseline utility bills.
* **Open Knowledge & Reputational Wealth ($\mathcal{R}$):** All Layer 6 Agora blueprints and knowledge are open from Day 1, paired with transparent visibility into learning (`Scenario Kappa`) and Layer 4 workflows where contributors earn decaying, sublinearly capped ($\le 800\,\mathrm{bp}$) **Reputational Wealth ($\mathcal{R}$)** to govern collective work in Layer 5 and progressively elevate $B_{\mathrm{baseline}}$.

### 15.4. Law 4 — Alpha Release Genesis & The Intent U-Turn Procurement Pipeline
* **Suburban Genesis State (`2,000,000` Integer USD Cents / $D_0 = 10000\,\mathrm{bp}$):** Day-1 Lot 402 initializes as an ordinary suburban house with a `$20,000.00` (`2,000,000` cents) Layer 7 reserve managed under `col-adversaryd` and $D_0 = 10000\,\mathrm{bp}$.
* **Strict Intent U-Turn Procurement (`L6 -> L5 -> L4 -> [Yield] -> L5 -> L6 -> L7`):** Commissioning any station (`142,000` cents Stage-1 Solar Array, `35,000` cents Stage-1 Homelab Workstation, `87,000` cents Stage-1 Hearth & Garden) traverses `L6 -> L5 -> L4`; when `col-execd` detects missing irreducible leaf components in `R1–R3`, it yields upward (`YIELD_UPWARD_L5 -> L6 -> L7`) so `col-adversaryd` executes the GPO fallback purchase from the `$20,000` reserve without ever creating a direct `l4_l7.sock` bypass.
