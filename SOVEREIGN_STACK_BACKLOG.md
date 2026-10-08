# THE SOVEREIGN STACK — PRODUCT BACKLOG (LAYERS 1–7 & VERIFICATION)
**Document Version:** 2.0.0-RELEASE  
**Status:** Complete, Ratified & Authoritative  
**Milestone:** Sovereign Stack Autonomous Protocols, Specifications, Runtimes & Layer 7 Adversarial Emulation  
**Target Environments:** Headless Edge Daemons (`collectived`, Linux ARM64 / RISC-V / x86_64, Microcontrollers / Zephyr RTOS) & WebAssembly Sandbox  
**Architectural Baseline:** Autonomous 7-Daemon Hierarchy, POSIX Process Isolation, Unix Domain Socket IPC, Zero Monolithic Threading, Volume 1 Canonical Fidelity  
**Consensus Status:** Unanimous Consensus Reached (5-Persona Architectural Council)

---

## 1. Executive Summary & Architectural Council Ratification

### 1.1. Council Ratification & The 5-Persona Unanimous Consensus
The Round 2 Architectural Council convened to overhaul the architectural baseline of the Sovereign Stack (`SOVEREIGN_STACK_BACKLOG.md`). Representing all five stakeholder disciplines:
1. **Product Owner / Orchestrator (`explorer_po_r2`):** Roadmap governance, infrastructure personas, value delivery pacing, and cross-stack acceptance criteria.
2. **Sovereign Stack Specialist (`explorer_sovereign_r2`):** Canonical fidelity to Volume 1 of the book (*The Sovereign Stack: A Blueprint for Cybernetic Ecology and Mutual Aid*, `doc/book_collective/`), Strict Adjacency, Proof-of-Thermodynamic-Work, demurrage, qualitative friction, and Scenario Omega.
3. **Quality Engineer (`explorer_qe_r2`):** Eradication of monolithic threading anti-patterns, POSIX process isolation, failure domain containment, cgroups resource enforcement, and consumer-driven contract testing.
4. **Stack Engineer (`explorer_stack_r2`):** Systems engineering specification of the 7 autonomous daemons (`col-telemetryd`, `col-meshd`, `col-storaged`, `col-execd`, `col-kmsd`, `col-commonsd`, `col-adversaryd`), Unix Domain Socket IPC, and hardware resource bounding.
5. **Oasis Engineer (`explorer_oasis_r2`):** Complete purge of client-side graphics rendering, volumetric world models, and viewport controls; strict demarcation of the Universal Human-Agent Interface (UHAI) / SITL bridge over IPC.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                     ROUND 2 ARCHITECTURAL COUNCIL 5-PERSONA UNANIMOUS CONSENSUS                  │
├──────────────────────────────┬────────────────────────────────┬──────────────────────────────────┤
│ Council Persona              │ Core Disciplinary Mandate      │ Key Ratified Contribution        │
├──────────────────────────────┼────────────────────────────────┼──────────────────────────────────┤
│ Product Owner (PO)           │ Delivery pacing & SoC          │ 8-Epic structure, infra personas │
│ Sovereign Stack Specialist   │ Volume 1 Book Canonical Tenets │ L1–L7 alignment, PoTW, demurrage │
│ Quality Engineer (QE)        │ Anti-monolith & failure bounds │ Process isolation, cgroups, SLOs │
│ Stack Engineer               │ Autonomous runtime daemons     │ 7 POSIX daemons, UDS IPC bus     │
│ Oasis Engineer               │ Boundary purity & game purge   │ Purge game mechanics, UHAI SITL  │
└──────────────────────────────┴────────────────────────────────┴──────────────────────────────────┘
```

The council formally confirms that **Unanimous Consensus Reached** has occurred across all technical, architectural, and systemic domains:
- **Eradication of Monolithic Game-Loop Threading:** All references to monolithic engine threads, in-process shared memory queues, and ludic frame rate ceilings are permanently eliminated.
- **Autonomous Layer Strategies:** Every layer (1 through 7) possesses its own autonomous computational daemon and independent, isolated storage engine.
- **Absolute Scope Demarcation:** Client-side graphics rendering loops, compute shaders, viewport modes, volumetric world data models, mortal embodiment physics, player vitality indicators, and psychological states belong exclusively to `PRODUCT_BACKLOG_V3.md` and are 100% purged from this backlog.
- **Headless Edge Autonomy:** The Sovereign Stack operates 24/7/365 as an autonomous, headless operating system on physical edge nodes (ARM64 Cortex-A72, Rockchip RK3588, RISC-V, ESP32-S3), completely independent of whether any graphical client is connected.
- **Universal Human-Agent Interface (UHAI):** Oasis interfaces with the Sovereign Stack strictly as an external client and Software-In-The-Loop (SITL) shadow simulator over versioned asynchronous IPC.

---

### 1.2. The Two Fatal Anti-Patterns of V1 & The V2 Decoupling Resolution
The V1 release of this backlog suffered from two catastrophic architectural flaws:
1. **Monolithic Threading Conflation:** Distributed networking, hardware telemetry, cryptographic proof verification, and municipal simulations were collapsed into arbitrary threads of a client-side game engine, subordinated to a rendering tick. If cryptographic pairing or a slow mock HTTP webhook stalled, physical battery telemetry was starved, risking hardware damage.
2. **Game Mechanic Infiltration:** Gameplay entities (such as volumetric space structures, mortal biological hunger data models, graphical views, and in-game citation callouts) were entangled with distributed protocol definitions.

**The V2 Resolution:**
V2 establishes strict Separation of Concerns (SoC). The Sovereign Stack is architected as an ensemble of seven independent system daemons managed by a lightweight supervisor. Communication occurs exclusively over asynchronous Unix Domain Sockets using versioned binary serialization (Cap'n Proto / Protobuf). Failure in higher layers cannot compromise lower-layer physical reflexes.

---

### 1.3. The 7-Layer Traversal Protocol ("No Layer Skipping") & Total Encapsulation
In accordance with Volume 1 Chapter 1 of the book (`doc/book_collective/`), the Sovereign Stack enforces two inviolable cybernetic laws:
1. **Strict Adjacency ("No Layer Skipping"):** Layer $N$ communicates only with Layer $N-1$ and Layer $N+1$. No semantic command (L6) may actuate a relay (L1) directly; no sensor reading (L2) may directly alter policy (L5). All intent descends through the full validation chain; all state ascends through the full verification chain.
2. **Total Encapsulation (Domain Isolation):** Each layer operates exclusively within its own domain. Lower layers are blind to semantics and fiat; higher layers never touch physical registers or voltage lines.

$$\text{Layer 7 (Legacy Proxy)} \longleftrightarrow \text{Layer 6 (Semantic Intent)} \longleftrightarrow \text{Layer 5 (Governance/WoT)} \longleftrightarrow \text{Layer 4 (Orchestrator)} \longleftrightarrow \text{Layer 3 (Ledger/PoTW)} \longleftrightarrow \text{Layer 2 (Digital Twin/Mesh)} \longleftrightarrow \text{Layer 1 (Physical Reality)}$$

---

### 1.4. Dual-Runtime Execution & Oasis Demarcation
The Sovereign Stack supports two deployment targets without code divergence:
1. **Physical Edge Production (`collectived`):** Native POSIX daemons running on Linux ARM64, RISC-V, or x86_64, interfacing with RS-485 Modbus charge controllers, SocketCAN battery management systems, Semtech SX1262 LoRa transceivers, and Microchip ATECC608A secure elements.
2. **Headless WebAssembly Sandbox (`sovereign_core.wasm`):** Each daemon compiles to an isolated Web Worker communicating over `MessageChannel` transferables, enabling deterministic headless simulations and browser-based edge nodes.

**Demarcation with Oasis:**
Oasis is an external 3D virtual environment and shadow simulator. When connected, Oasis simulates the physical environment, feeding synthetic sensor telemetry (`QUALITY_SIMULATED`) into Layer 2 via the UHAI SITL socket and receiving actuator commands. The Sovereign Stack executes its full cryptographic and thermodynamic pipeline completely unaware of 3D graphics.

---

## 2. System Architecture, Structural Invariants & Autonomous Layer Strategies

### 2.1. The Autonomous 7-Daemon Architecture & Process Isolation Matrix
Each layer of the Sovereign Stack operates as an independent POSIX process managed by `collective-supervisor`:

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              THE 7-DAEMON AUTONOMOUS RUNTIME ARCHITECTURE                              │
├───────┬───────────────────────────┬──────────────────────────────────────┬─────────────────────────────┤
│ LAYER │ DAEMON / PROCESS          │ AUTONOMOUS COMPUTATIONAL STRATEGY    │ AUTONOMOUS STORAGE STRATEGY │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L7**│ `col-adversaryd`          │ Priority-queue Discrete Event Sim    │ Isolated Adversarial SQLite │
│       │ (Legacy Proxy & Adversary)│ (DES); mock utilities, zoning, fiat  │ (Zero cross-DB FKs; shred)  │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L6**│ `col-commonsd`            │ JSON-LD Intent compiler; W3C PROV-O  │ Embedded RDF Quad Store     │
│       │ (Semantic Intent & Agora) │ knowledge graph; double-auction book │ (Oxigraph) / DuckDB graph   │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L5**│ `col-kmsd`                │ W3C DID resolver; BBS+ ZKP engine;   │ Encrypted SQLCipher vault   │
│       │ (Governance & Web of Trust│ FROST threshold crypto; EigenTrust   │ + Sparse Merkle Tree (SMT)  │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L4**│ `col-execd`               │ Deterministic 10 Hz BPMN 2.0 VM;     │ Durable Saga WAL journal +  │
│       │ (Orchestrator & Execution)│ Sandboxed Wasmtime; ecological floors│ Task state RocksDB store    │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L3**│ `col-storaged`            │ Automerge/Yrs CRDT engine; HLC causal│ Content-Addressed CAS (IPLD)│
│       │ (Thermodynamic Ledger)    │ ordering; PoTW negentropy minting    │ + RocksDB CRDT append-log   │
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L2**│ `col-meshd`               │ Reticulum Network Stack (RNS) / LoRa │ Persistent DTN spool ring + │
│       │ (Digital Twin & P2P Mesh) │ async reactor; UHAI telemetry bridge │ LMDB peerstore routing table│
├───────┼───────────────────────────┼──────────────────────────────────────┼─────────────────────────────┤
│ **L1**│ `col-telemetryd`          │ Linux epoll / RTOS HAL; RS485 Modbus,│ Flash NVRAM ring buffer +   │
│       │ (Physical Reality & HAL)  │ CAN-bus, hardware watchdog, ATECC608A│ ATECC608A secure EEPROM     │
└───────┴───────────────────────────┴──────────────────────────────────────┴─────────────────────────────┘
  SUPERVISOR & IPC:
  Managed by `collective-supervisor`. Inter-layer IPC via Unix Domain Sockets (`/run/collective/ipc/l[1-7].sock`).
```

---

### 2.2. Autonomous Computational & Storage Strategies (Detailed Matrix)

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              AUTONOMOUS COMPUTATIONAL & STORAGE SPECIFICATIONS                         │
├───────┬──────────────────┬────────────────────────────────┬──────────────────────┬─────────────────────┤
│ Layer │ Daemon           │ Concurrency & Event Loop       │ Storage Engine       │ State Format        │
├───────┼──────────────────┼────────────────────────────────┼──────────────────────┼─────────────────────┤
│ **L1**│ `col-telemetryd` │ Epoll reactor + timerfd;       │ Non-volatile flash   │ 24-byte binary      │
│       │                  │ SCHED_FIFO real-time priority  │ NVRAM ring buffer    │ TelemetrySample     │
├───────┼──────────────────┼────────────────────────────────┼──────────────────────┼─────────────────────┤
│ **L2**│ `col-meshd`      │ Async multi-bearer reactor;    │ Embedded LMDB +      │ Bounded DTN packet  │
│       │                  │ non-blocking socket multiplex  │ Append-only spool    │ spool (max 50 MB)   │
├───────┼──────────────────┼────────────────────────────────┼──────────────────────┼─────────────────────┤
│ **L3**│ `col-storaged`   │ 3-worker pipeline: Ingest,     │ RocksDB blockstore + │ IPLD CAR v2 blocks +│
│       │                  │ Crypto verify, Disk commit     │ Append-only WAL      │ Automerge delta ops │
├───────┼──────────────────┼────────────────────────────────┼──────────────────────┼─────────────────────┤
│ **L4**│ `col-execd`      │ Deterministic 10 Hz tick loop; │ RocksDB SMT store +  │ Wasm state root +   │
│       │                  │ metered Wasmtime fuel budget   │ Transaction WAL      │ BPMN token journal  │
├───────┼──────────────────┼────────────────────────────────┼──────────────────────┼─────────────────────┤
│ **L5**│ `col-kmsd`       │ Dedicated crypto worker pool;  │ SQLCipher (AES-256) +│ Sparse Merkle Tree  │
│       │                  │ mlockall() memory protection   │ In-memory WoT graph  │ + encrypted keys    │
├───────┼──────────────────┼────────────────────────────────┼──────────────────────┼─────────────────────┤
│ **L6**│ `col-commonsd`   │ Asynchronous intent compiler;  │ Oxigraph RDF store + │ W3C JSON-LD / OWL   │
│       │                  │ double-auction matching tick   │ SQLite order book    │ RDF Quads           │
├───────┼──────────────────┼────────────────────────────────┼──────────────────────┼─────────────────────┤
│ **L7**│ `col-adversaryd` │ Priority-queue DES event loop; │ Isolated SQLite      │ Municipal citation  │
│       │                  │ nice +10 background scheduling │ (zero cross-DB FKs)  │ & fiat ledger logs  │
└───────┴──────────────────┴────────────────────────────────┴──────────────────────┴─────────────────────┘
```

---

### 2.3. Inter-Process Communication (IPC) & Async Event Bus Architecture
All inter-daemon communication is conducted over Unix Domain Sockets (UDS) located in `/run/collective/ipc/`:

1. **Direct Bilateral Sockets:** Adjacent layers maintain dedicated bilateral sockets (`SOCK_SEQPACKET` or `SOCK_STREAM`):
   - `/run/collective/ipc/l1_l2.sock` (`col-telemetryd` $\longleftrightarrow$ `col-meshd`)
   - `/run/collective/ipc/l2_l3.sock` (`col-meshd` $\longleftrightarrow$ `col-storaged`)
   - `/run/collective/ipc/l3_l4.sock` (`col-storaged` $\longleftrightarrow$ `col-execd`)
   - `/run/collective/ipc/l4_l5.sock` (`col-execd` $\longleftrightarrow$ `col-kmsd`)
   - `/run/collective/ipc/l5_l6.sock` (`col-kmsd` $\longleftrightarrow$ `col-commonsd`)
   - `/run/collective/ipc/l6_l7.sock` (`col-commonsd` $\longleftrightarrow$ `col-adversaryd`)
2. **Broadcast Event Bus (`col-busd`):** A lightweight UDS message broker at `/run/collective/ipc/bus.sock` distributes asynchronous notifications (telemetry alarms, block confirmations, citation issuances).
3. **Serialization:**
   - High-throughput telemetry and state streams use **Cap'n Proto** or **Protobuf v3** zero-copy binary schemas.
   - Control plane RPC utilizes typed Request/Response envelopes over UDS.
4. **Bounded Backpressure:** Every socket sets `SO_SNDBUF` and `SO_RCVBUF` to 256 KB. Producers use non-blocking I/O (`O_NONBLOCK`). Telemetry drops non-critical frames when downstream queues are full; critical actuation commands employ bounded timeouts with circuit breakers.
5. **Strict Adjacency Enforcement:** The IPC supervisor enforces socket access controls (`chmod 0660`, user/group separation). `col-commonsd` (L6) possesses no file descriptor or socket path to `col-telemetryd` (L1). Downward commands must encapsulate a cryptographic chain of custody signed by each intermediary layer.

---

### 2.4. Universal Human-Agent Interface (UHAI) Specification
The Universal Human-Agent Interface (UHAI) standardizes the boundaries between the Sovereign Stack and external entities:

#### Channel 1: Sensory Ingress & Actuation Egress (Layer 2 / Digital Twin Boundary)
Standardized binary wire format (C++20):

```cpp
namespace collective::uhai {

enum class UnitType : uint8_t {
    WATTS          = 0,
    CELSIUS        = 1,
    LITERS_PER_MIN = 2,
    VOLTS          = 3,
    AMPERES        = 4,
    PRESSURE_KPA   = 5,
    PULSE_COUNT    = 6,
    KILOGRAMS      = 7
};

enum QualityFlags : uint8_t {
    QUALITY_OK             = 0x00,
    QUALITY_SIMULATED      = 0x01, // Emitted by external SITL (e.g. Oasis)
    QUALITY_HARDWARE_ROOT  = 0x02, // Hardware secure element verified
    QUALITY_OUT_OF_BOUNDS  = 0x04,
    QUALITY_SPOOF_SUSPECT  = 0x08  // Flagged by spatial/thermodynamic cross-check
};

// Exactly 24 bytes, 8-byte aligned: universal sensor wire format
struct alignas(8) TelemetrySample {
    uint64_t timestamp_hlc;      // Hybrid Logical Clock timestamp
    uint32_t channel_id;         // Telemetry register ID
    float    value;              // Calibrated physical quantity
    UnitType unit_type;          // Physical unit enum
    uint8_t  quality_flags;      // Provenance and validation flags
    uint16_t sensor_node_id;     // Originating node DID slot
};
static_assert(sizeof(TelemetrySample) == 24, "TelemetrySample must remain exactly 24 bytes.");

// Exactly 80 bytes, 8-byte aligned: universal actuation command
struct alignas(8) ActuatorCommand {
    uint64_t command_id;         // Monotonic command UUID
    uint32_t device_id;          // Actuator register ID (valve, relay, breaker)
    float    target_state;       // Setpoint duty cycle (0.0 - 1.0) or target level
    uint32_t duration_ms;        // Pulse width or timeout limit
    uint8_t  auth_signature[64]; // Ed25519 signature from Layer 4 execution key
};
static_assert(sizeof(ActuatorCommand) == 80, "ActuatorCommand must remain exactly 80 bytes.");

// Polymorphic bridge interface implemented by Hardware Drivers and SITL
class IDigitalTwinBridge {
public:
    virtual ~IDigitalTwinBridge() = default;
    virtual bool IngestTelemetry(const TelemetrySample& sample) = 0;
    virtual bool DispatchActuation(const ActuatorCommand& cmd) = 0;
    virtual bool ValidateThermodynamicPlausibility(const TelemetrySample& sample) = 0;
};

} // namespace collective::uhai
```

#### Channel 2: Semantic Intent & Observability Event Stream (Layer 5/6 Boundary)
- **Intent Ingress:** External clients (stewards, mobile PWAs, Oasis, CLI) submit declarative W3C JSON-LD intent envelopes signed with the steward's `did:key` over WebSocket or UDS.
- **Observability Egress:** The stack broadcasts structured domain events (`ValuenomicsMintedEvent`, `WorkTokenAvailableEvent`, `MunicipalCitationIssuedEvent`, `ScenarioOmegaDecoupledEvent`) across `/run/collective/ipc/bus.sock`. External graphical clients consume this stream to update their own visualizations.

---

### 2.5. Infrastructure Service Level Objectives (SLOs) Matrix
Replacing all legacy frame budgets, the Sovereign Stack is evaluated exclusively against distributed systems and infrastructure SLOs:

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
│ Daemon Crash Recovery Time Objective (RTO)      │ ≤ 500 ms                             │
│ Memory Leak Tolerance (Continuous 72-Hour Load) │ 0 bytes leaked (Valgrind / ASan)     │
│ Total Headless Stack Memory Ceiling (L1–L7 RSS) │ ≤ 320 MB total RSS on Linux ARM64    │
│ Layer 7 Memory Envelope                         │ ≤ 24 MB RSS                          │
│ Layer 7 Terminal Decoupling Execution Time      │ ≤ 100 ms total shutdown and wipe     │
└─────────────────────────────────────────────────┴──────────────────────────────────────┘
```

---

### 2.6. Strict Anti-Bleed Boundaries & Scope Rules (The 5 Scope Boundary Rules)
The codebase enforces five architectural boundary rules via CI linters (`scripts/lint_anti_bleed.sh`):

| Rule ID | Rule Name | Core Invariant | Enforced Verification |
| :---: | :--- | :--- | :--- |
| **SBR-1** | **The Headless Rule** | The Sovereign Stack compiles and executes 100% headlessly with zero graphics or windowing dependencies (`SDL2`, `WebGPU`, `Dawn`, `OpenGL`, `Vulkan`). | CI build passes in an environment without `DISPLAY`, `WAYLAND_DISPLAY`, or GPU drivers. |
| **SBR-2** | **The Frame Rate Rule** | Sovereign Stack specifications and runtimes contain zero references to display frame rates, frame budgets, or render loops (no frame rate ceilings, no display tick budgets, no `render_ms`). | CI grep linter fails if frame budgets appear in core stack modules. |
| **SBR-3** | **The Domain Entity Rule** | Sovereign Stack contains zero Oasis game entities (no game data structures, no player kinematics, no founder avatars, no hunger/hydration/fatigue states, no player vitality markers). | Static analysis ensures game structs exist only within `oasis/`. |
| **SBR-4** | **The Presentation Agnostic Rule** | Sovereign Stack never dictates visual styling or UI elements (no citation HUD banners, glowing overlays, or viewport angles). It emits structured data events only. | Code review gate rejecting visual rendering descriptions in L1–L7 specs. |
| **SBR-5** | **The Strict UHAI Rule** | All interaction between Oasis and the Sovereign Stack traverses the UHAI binary IPC protocol (L2) or W3C JSON-LD event streaming (L5/L6). | Zero direct memory sharing between graphics state and stack ledgers. |

---

## 3. Epic 1: Layer 1 — Physical Reality, Energy Harvesting & Hardware Root-of-Trust

### User Story 1.1: Multi-Tier Hardware Root-of-Trust & Chassis Tamper Zeroization Daemon
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-001  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want the edge node to initialize its cryptographic identity from a physical hardware secure element and immediately zeroize ephemeral keys upon chassis enclosure breach,  
**Benefit:** So that physical capture of an off-grid node by hostile actors cannot compromise the bioregional Web of Trust.

#### Technical Specifications & Architecture
- **Daemon:** `col-telemetryd` running with `CAP_SYS_RAWIO`.
- **Hardware Target:** Microchip ATECC608A / NXP SE050 via I2C (`/dev/i2c-1`) and tamper micro-switches via GPIO edge interrupt (`/dev/gpiochip0`).
- **Zeroization Pipeline:** Upon interrupt trigger, the daemon issues an atomic hardware command to zeroize volatile key slots within $\le 12\text{ }\mu\text{s}$, writes an immutable tamper flag to NVRAM, and issues `SIGTERM` to `col-kmsd`.

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
Scenario: Nominal boot with hardware root-of-trust
  Given an edge node equipped with an ATECC608A secure element
  When col-telemetryd initializes during system boot
  Then it retrieves the hardware-anchored did:key identifier without reading private key material into system RAM
  And it exposes ChassisStatus with tamper_circuit_closed = true over /run/collective/ipc/l1_l2.sock.

Scenario: Physical enclosure breach triggers key zeroization
  Given a running node with active cryptographic sessions
  When the physical chassis tamper circuit opens
  Then a hardware GPIO edge interrupt triggers zeroization in less than 12 microseconds
  And all volatile session keys in col-kmsd are overwritten with zeros
  And an immutable tamper event is committed to flash NVRAM before process termination.
```

#### Technical Tasks
- [ ] Implement Linux GPIO character device edge-detection listener in `col-telemetryd`.
- [ ] Implement I2C driver for Microchip ATECC608A utilizing PKCS#11 API.
- [ ] Implement zero-copy atomic zeroization hook communicating with `col-kmsd` over UDS.
- [ ] Add unit test verifying tamper ISR latency $\le 12\text{ }\mu\text{s}$ using virtual mock GPIO.

---

### User Story 1.2: Asynchronous RS-485 Modbus RTU Energy Telemetry & SocketCAN BMS Drivers
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-002  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-telemetryd` to continuously poll solar MPPT charge controllers and CAN-bus battery management systems without blocking,  
**Benefit:** So that real-time physical exergy generation and storage are precisely tracked for Proof-of-Thermodynamic-Work accounting.

#### Technical Specifications & Architecture
- **Protocols:** Modbus RTU over RS-485 (`/dev/ttyUSB0` via `termios2`) and CAN 2.0B via Linux SocketCAN (`can0`).
- **Concurrency:** Dedicated non-blocking epoll loop with POSIX `timerfd` sampling Modbus registers at 1 Hz and SocketCAN BMS frames at 10 Hz.
- **Zero-Allocation Hot Path:** Samples are packed directly into 24-byte `TelemetrySample` structs and written to a memory-mapped circular buffer.

```cpp
struct ModbusRegisterMap {
    static constexpr uint16_t REG_PV_VOLTAGE       = 0x3100; // 0.01 V
    static constexpr uint16_t REG_PV_CURRENT       = 0x3101; // 0.01 A
    static constexpr uint16_t REG_BATTERY_VOLTAGE   = 0x3104; // 0.01 V
    static constexpr uint16_t REG_BATTERY_CURRENT   = 0x3105; // 0.01 A
    static constexpr uint16_t REG_LOAD_POWER        = 0x310E; // 0.01 W
};
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Continuous Modbus RTU solar telemetry sampling
  Given an RS-485 bus connected to an EPEver Tracer MPPT solar charge controller
  When col-telemetryd executes its 1 Hz timerfd tick
  Then it queries registers 0x3100 through 0x310E asynchronously without blocking the reactor
  And it packs the calibrated Wattage and Voltage into 24-byte TelemetrySample structs
  And it streams the samples across /run/collective/ipc/l1_l2.sock with p99 latency <= 5.0 ms.

Scenario: CAN-bus LiFePO4 battery management alert
  Given a 16-cell LiFePO4 battery pack reporting over SocketCAN interface can0
  When any individual cell voltage exceeds 3.65 V or drops below 2.50 V
  Then col-telemetryd immediately emits a high-priority TelemetrySample with QUALITY_OUT_OF_BOUNDS set
  And it triggers local hardware protection contacts within 50 ms.
```

#### Technical Tasks
- [ ] Implement asynchronous Modbus RTU parser using non-blocking serial I/O.
- [ ] Implement SocketCAN frame reader listening on `can0` with raw frame filtering.
- [ ] Implement ring buffer writer with lock-free atomic head/tail pointers.
- [ ] Bench test on Raspberry Pi CM4 confirming CPU utilization $\le 1.5\%$ under continuous polling.

---

### User Story 1.3: Real-Time Thermodynamic Exergy & Thermal Sensor Telemetry Pipeline
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-003  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-telemetryd` to calculate instantaneous physical exergy flows from raw electrical, thermal, and fluid sensor inputs,  
**Benefit:** So that higher layers receive thermodynamically validated negentropy measurements rather than raw, uncalibrated numbers.

#### Technical Specifications & Architecture
- **Exergy Equation:** Implements real-time physical exergy rate calculation:
  $$\dot{E}x = \dot{W} + \dot{Q}\left(1 - \frac{T_0}{T}\right) + \dot{m}\left[\left(h - h_0\right) - T_0\left(s - s_0\right)\right]$$
  where $T_0$ is local ambient dead-state temperature, $T$ is reservoir temperature, and $\dot{W}$ is electrical power.
- **Sensors:** I2C ambient temperature/pressure (BME680), DS18B20 1-Wire thermal probes, and pulse-counter flow meters.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Thermodynamic exergy stream computation
  Given an electrical power input of 1200 W and a thermal water reservoir at 65°C with ambient temp at 15°C
  When col-telemetryd evaluates the exergy equation
  Then it computes thermal Carnot efficiency (1 - 288.15 / 338.15 = 0.1478)
  And it outputs an exergy sample reflecting 1200 W electrical plus Carnot-derated thermal Watts
  And it flags the sample with QUALITY_HARDWARE_ROOT.
```

#### Technical Tasks
- [ ] Implement Carnot efficiency and fluid enthalpy formulas in fixed-point or IEEE 754 math.
- [ ] Add 1-Wire Linux kernel driver integration (`/sys/bus/w1/devices/`).
- [ ] Add unit test verifying thermodynamic calculations against standard steam/water tables.

---

### User Story 1.4: Hardware Watchdog Supervision & Autonomous Low-Power Brownout Recovery
**Epic:** Epic 1 (Layer 1: Physical Reality)  
**Story ID:** SS-EP1-004  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want hardware watchdog integration and low-voltage emergency flush handlers,  
**Benefit:** So that an unrecoverable power loss or software deadlock automatically recovers the node without human intervention.

#### Technical Specifications & Architecture
- **Watchdog:** Interacts with `/dev/watchdog` with a 10-second timeout.
- **Brownout Sequence:** Monitored via ADC / BMS. When voltage drops below 11.2 V (LiFePO4 12V bank):
  1. Issues high-priority Brownout signal to all daemons via `/run/collective/ipc/bus.sock`.
  2. Each daemon flushes uncommitted WAL buffers to non-volatile storage within $\le 2.5\text{ ms}$.
  3. Node switches system power governor to low-frequency mode (`powersave`).

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Automatic hardware watchdog ping
  Given col-telemetryd running nominally
  When the internal event loop completes its timerfd cycle
  Then it writes the keep-alive byte to /dev/watchdog every 3 seconds
  And if the process deadlocks for > 10 seconds, the hardware supervisor resets the system.

Scenario: Low-voltage brownout emergency flush
  Given a battery pack discharging rapidly during prolonged overcast weather
  When the measured terminal voltage drops below 11.2 V
  Then col-telemetryd broadcasts a BrownoutAlert across the IPC event bus
  And all daemons flush WAL state to non-volatile storage in under 2.5 ms
  And the node enters deep survival sleep.
```

#### Technical Tasks
- [ ] Implement `/dev/watchdog` keepalive pinger with loop health verification.
- [ ] Implement system-wide brownout event broadcaster.
- [ ] Validate emergency flush duration on eMMC storage $\le 2.5\text{ ms}$.

---

## 4. Epic 2: Layer 2 — Delay-Tolerant Mesh, Reticulum Transport & Telemetry Ingress

### User Story 2.1: Reticulum Cryptographic Mesh Routing Engine & 128-Bit Addressing
**Epic:** Epic 2 (Layer 2: Digital Twin & Mesh)  
**Story ID:** SS-EP2-001  
**Persona:** As a Community Steward,  
**Goal:** I want `col-meshd` to route encrypted packets across off-grid peer-to-peer networks using 128-bit destination hashes without relying on IP addresses, ICANN DNS, or central servers,  
**Benefit:** So that bioregional nodes maintain autonomous communications even when municipal internet backbones are severed.

#### Technical Specifications & Architecture
- **Daemon:** `col-meshd` running an asynchronous multi-bearer reactor.
- **Addressing:** Reticulum Network Stack (RNS) destination hashing: 128-bit truncated SHA-256 of the destination public key.
- **Cryptography:** Curve25519 ECDH key exchange, ChaCha20-Poly1305 AEAD authenticated packet encryption.
- **Zero-IP Operation:** Operates over raw Ethernet frames, AX.25, LoRa KISS, and serial pipes.

```protobuf
syntax = "proto3";
package collective.l2;

message MeshPacket {
  bytes destination_hash = 1; // 16 bytes (128-bit)
  bytes sender_hash = 2;      // 16 bytes
  uint32_t hop_limit = 3;
  bytes payload = 4;          // AEAD encrypted payload
  bytes signature = 5;        // Ed25519 signature
}
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Air-gapped packet routing over 128-bit destination hash
  Given two off-grid sovereign nodes with zero IP connectivity
  When Node A sends a packet addressed to Node B's 128-bit destination hash
  Then col-meshd discovers the route via Reticulum announce beacons
  And encrypts the payload using ephemeral Curve25519 / ChaCha20-Poly1305 keys
  And delivers the packet across intermediate LoRa hops without IP routing.
```

#### Technical Tasks
- [ ] Implement Reticulum packet framing and 128-bit destination address parser.
- [ ] Implement Curve25519/ChaCha20-Poly1305 packet cryptor in C++20/Rust.
- [ ] Add route discovery table with hop-count metrics and announce cache.

---

### User Story 2.2: Multi-Bearer Transport Manager with Automatic LoRa / Wi-Fi Failover
**Epic:** Epic 2 (Layer 2: Digital Twin & Mesh)  
**Story ID:** SS-EP2-002  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-meshd` to dynamically route traffic across Semtech SX1262 LoRa, 802.11s Wi-Fi mesh, and physical sneakernet USB transports,  
**Benefit:** So that network partitions automatically degrade to low-bandwidth radio channels without dropping application packets.

#### Technical Specifications & Architecture
- **Bearer Interfaces:**
  - High-bandwidth bearer: 802.11s Wi-Fi mesh / Ethernet (QUIC transport, MTU 1280).
  - Low-power radio: Semtech SX1262 915 MHz LoRa via SPI (MTU 255 bytes, spread factor SF7–SF12).
  - Sneakernet bearer: Automated file export/import to removable block storage.
- **Failover Logic:** Dynamic link quality estimator (LQ) tracking packet loss and RSSI. Switches from Wi-Fi to LoRa in $\le 250\text{ ms}$ upon link drop.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Automatic bearer failover during Wi-Fi link disruption
  Given a dual-bearer node transmitting mesh state over 802.11s Wi-Fi
  When the Wi-Fi link experiences 100% packet loss for 3 consecutive intervals
  Then col-meshd automatically reroutes outbound packets through Semtech SX1262 LoRa
  And fragments payloads exceeding 255 bytes into indexed packet frames
  And resumes transmission with zero packet loss in the higher-layer queue.
```

#### Technical Tasks
- [ ] Implement SPI driver for Semtech SX1262 LoRa transceiver.
- [ ] Implement packet fragmentation and reassembly engine for 255-byte MTU limits.
- [ ] Implement dynamic bearer scoring and automatic failover state machine.

---

### User Story 2.3: Delay-Tolerant Store-and-Forward Packet Queue & LMDB Peerstore
**Epic:** Epic 2 (Layer 2: Digital Twin & Mesh)  
**Story ID:** SS-EP2-003  
**Persona:** As a Community Steward,  
**Goal:** I want `col-meshd` to buffer undelivered mesh packets in an append-only store-and-forward disk spool during long network partitions,  
**Benefit:** So that nodes isolated for days or weeks automatically synchronize when connectivity is re-established.

#### Technical Specifications & Architecture
- **Storage Engine:** Embedded Lightning Memory-Mapped Database (LMDB) at `/var/lib/collective/l2/peerstore.lmdb` for peer identities, plus append-only packet spool `/var/lib/collective/l2/spool.bin` (capped at 50 MB with cryptographic priority eviction).
- **TTL & Expiry:** Packets carry TTL timestamps; expired non-critical packets are evicted via LRU, while cryptographic ledger sync requests are retained until delivered.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Store-and-forward packet queuing during 72-hour network partition
  Given an isolated node disconnected from the bioregional mesh for 72 hours
  When higher layers generate 500 state update packets
  Then col-meshd persists the packets to the bounded LMDB delay spool
  And when a peer node comes within LoRa radio range, col-meshd opportunistically drains the spool
  And all 500 packets are delivered with verified cryptographic integrity.
```

#### Technical Tasks
- [ ] Implement LMDB-backed peerstore for storing peer public keys and capabilities.
- [ ] Implement priority-queued persistent disk spool with 50 MB hard ceiling.
- [ ] Add automated partition test asserting zero dropped packets over simulated link downtime.

---

### User Story 2.4: Universal Hardware Abstraction Interface (UHAI) & SITL Adapter
**Epic:** Epic 2 (Layer 2: Digital Twin & Mesh)  
**Story ID:** SS-EP2-004  
**Persona:** As an External Simulation Client (Oasis Engine),  
**Goal:** I want `col-meshd` to expose a Software-in-the-Loop (SITL) socket accepting virtual `TelemetrySample` frames and dispatching `ActuatorCommand` frames,  
**Benefit:** So that Oasis can act as a high-fidelity digital twin and shadow simulator for testing the Sovereign Stack without physical hardware.

#### Technical Specifications & Architecture
- **Socket:** `/run/collective/ipc/uhai_sitl.sock` (Unix Domain Socket, bidirectional binary stream).
- **Data Wire Format:** 24-byte `TelemetrySample` and 80-byte `ActuatorCommand` matching `collective::uhai` specification.
- **Quality Tagging:** Samples injected via SITL are tagged with `QUALITY_SIMULATED (0x01)`. The stack processes them identically to hardware samples while preserving provenance.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Oasis shadow simulator ingests virtual solar telemetry via SITL
  Given the Sovereign Stack running in simulation mode with col-meshd active
  When Oasis streams a 24-byte TelemetrySample with channel_id = 0x3100, value = 450.0 W, and QUALITY_SIMULATED
  Then col-meshd validates sample alignment and forwards it across /run/collective/ipc/l2_l3.sock
  And Layer 3 processes the exergy calculation with simulated provenance intact.

Scenario: Actuator command dispatch to virtual simulator
  Given a Layer 4 workflow issuing a command to open an irrigation valve
  When the actuation envelope reaches Layer 2
  Then col-meshd formats an 80-byte ActuatorCommand with valid Ed25519 signature
  And transmits it across the UHAI SITL socket to Oasis
  And Oasis confirms receipt and applies the valve state change.
```

#### Technical Tasks
- [ ] Implement UHAI SITL socket server in `col-meshd` with `SOCK_SEQPACKET`.
- [ ] Implement binary serialization matching `sizeof(TelemetrySample) == 24` and `sizeof(ActuatorCommand) == 80`.
- [ ] Implement SITL loopback test verifying roundtrip latency $\le 50\text{ }\mu\text{s}$.

---

## 5. Epic 3: Layer 3 — Thermodynamic Ledger, CRDT Replication & Valuenomics Minting

### User Story 3.1: Compact Delta-Based CRDT Engine & HLC Causal Replication
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-001  
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

### User Story 3.3: Proof-of-Thermodynamic-Work (PoTW) Minting & Demurrage Decay Engine
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-003  
**Persona:** As a Community Steward,  
**Goal:** I want `col-storaged` to mint Value Tokens strictly from verified physical exergy additions and apply continuous demurrage decay,  
**Benefit:** So that community wealth mirrors physical thermodynamics and speculative hoarding is mathematically prevented.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapter 4 (`04_layer_3_thermodynamic_ledger.tex`).
- **Minting Equation:**
  $$\Delta V = \int_{t_0}^{t_1} \left(\dot{E}x_{in} - \dot{E}x_{out}\right) \cdot \lambda_{ERC}(R) \, dt$$
  where $\lambda_{ERC}(R)$ is the Ecological Replacement Cost function approaching infinity as local resource extraction approaches ecological carrying capacity ($R \to R_{capacity}$).
- **Demurrage Decay:** Tokens decay continuously at rate $\delta_{asset}$ calibrated to the physical half-life of underlying storage assets (e.g., battery degradation, heat dissipation):
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
- [ ] Implement $\lambda_{ERC}(R)$ asymptotic pricing formula.
- [ ] Implement continuous demurrage calculation with daily decay reconciliation.

---

### User Story 3.4: Zero-Sum Bioregional Mutual Credit Clearing & Debt Loop Elimination
**Epic:** Epic 3 (Layer 3: Thermodynamic Ledger)  
**Story ID:** SS-EP3-004  
**Persona:** As a Community Steward,  
**Goal:** I want `col-storaged` to clear circular trade obligations using Johnson’s cycle-canceling algorithm,  
**Benefit:** So that debts between community members are settled without requiring fiat liquidity or external banking intermediaries.

#### Technical Specifications & Architecture
- **Mutual Credit Invariant:** $\sum_{i=1}^{N} Balance_i \equiv 0$ (Zero-sum bioregional credit).
- **Cycle Canceling:** Detects directed cycles in the credit graph ($A \to B \to C \to A$) and cancels min-capacity debt loops at periodic clearing intervals.

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
- [ ] Implement directed debt graph in memory using adjacency lists.
- [ ] Implement Johnson's cycle detection algorithm with $O((V+E)(c+1))$ complexity.
- [ ] Add unit test verifying debt cancellation across multi-party circular chains.

---

## 6. Epic 4: Layer 4 — Deterministic BPMN 2.0 Orchestrator & Ecological Floors

### User Story 4.1: Embedded Deterministic BPMN 2.0 Workflow VM & Sandboxed Wasm Runner
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-001  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-execd` to compile and deterministically execute BPMN 2.0 workflow state machines inside a metered WebAssembly sandbox,  
**Benefit:** So that automated municipal and ecological workflows execute identically across all nodes without non-deterministic drift.

#### Technical Specifications & Architecture
- **Daemon:** `col-execd` executing a deterministic 10 Hz logic tick.
- **Wasm Runtime:** Sandboxed Wasmtime / WAMR engine with:
  - Fuel/gas instruction metering preventing infinite loops.
  - NaN canonicalization forbidding non-deterministic floating-point math.
  - Time virtualization: system time is injected strictly from the block HLC timestamp.
  - Zero direct host system calls (`seccomp-bpf` isolation).

```protobuf
syntax = "proto3";
package collective.l4;

message WorkflowInstance {
  string workflow_id = 1;
  uint64_t instance_id = 2;
  string current_activity_id = 3;
  uint64_t fuel_remaining = 4;
  map<string, bytes> process_variables = 5;
  uint64_t start_time_hlc = 6;
}
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Deterministic BPMN workflow step execution
  Given a validated BPMN 2.0 microgrid load-shedding workflow definition
  When col-execd executes the current task step inside the Wasmtime sandbox
  Then it consumes instruction fuel proportionally
  And completes the state transition within the 10 Hz tick window (<= 100 ms)
  And produces a bit-exact state root hash across ARM64 and x86_64 architectures.
```

#### Technical Tasks
- [ ] Implement BPMN 2.0 XML schema compiler generating deterministic finite state machine bytecode.
- [ ] Configure Wasmtime runtime with fuel metering and canonical float NaN flags.
- [ ] Add cross-architecture execution test asserting bit-identical state roots.

---

### User Story 4.2: WorkToken Cryptographic Escrow & Physical Labor Allocation Lifecycle
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-002  
**Persona:** As a Community Steward,  
**Goal:** I want `col-execd` to dispatch physical maintenance tasks as cryptographic `WorkToken` escrows with caloric limits,  
**Benefit:** So that human physical labor is coordinated transparently without managerial hierarchy or exploitation.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapter 5 ("Freeing Work, Not the Worker").
- **Qualitative Friction:** Labor allocations enforce caloric burn ceilings ($E_{cal} \le 2,500\text{ kcal/day}$) and mandatory rest periods. Tasks carry cryptographic escrow deposits released upon multi-sig completion attestations.

```protobuf
syntax = "proto3";
package collective.l4;

message WorkToken {
  uint64_t token_id = 1;
  string task_description = 2;
  uint32_t estimated_caloric_burn = 3;
  uint64_t exergy_reward = 4;
  uint64_t escrow_deadline_hlc = 5;
  bytes assigned_steward_did = 6;
  enum State {
    AVAILABLE = 0;
    CLAIMED = 1;
    IN_PROGRESS = 2;
    ATTESTED = 3;
    SETTLED = 4;
    EXPIRED = 5;
  }
  State state = 7;
}
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: WorkToken claim and settlement lifecycle
  Given an available WorkToken for clearing irrigation ditch debris (estimated 800 kcal)
  When an authenticated steward claims the task using their did:key
  Then col-execd transitions the token to CLAIMED and locks the exergy reward in escrow
  When two peer stewards submit cryptographically signed completion attestations
  Then col-execd releases the escrowed reward to the steward's balance
  And advances the parent BPMN workflow to the next step.
```

#### Technical Tasks
- [ ] Implement WorkToken state machine with atomic transition locks.
- [ ] Implement caloric ceiling and labor friction validation rules.
- [ ] Add multi-signature attestation verifier for task completion.

---

### User Story 4.3: Ecological Floors & Actuation Envelopes with Efferent Handshake
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-003  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-execd` to issue hardware control commands strictly as bounded *Actuation Envelopes* and verify ecological circuit breakers,  
**Benefit:** So that no automated or human intent can drain an aquifer below replenishment levels or overload electrical transformers.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapters 3 and 5.
- **Actuation Envelope Schema:**
  - Setpoint value, deadband tolerance ($\pm \epsilon$), maximum ramp rate ($d/dt$), ceiling cutoff, exergy quota, expiry timestamp.
- **Efferent Handshake:** Layer 4 signs the envelope; Layer 3 logs the commitment; Layer 2 validates that the command does not violate hardcoded microcontroller edge reflexes before applying voltage.
- **Ecological Floor Circuit Breaker:** Hardcoded preconditions (minimum stream flow, maximum battery depth of discharge 80%, topsoil moisture thresholds) that abort actuation if violated.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Actuation command constrained by Actuation Envelope
  Given an automated irrigation workflow attempting to open a high-capacity pump
  When the calculated flow rate would exceed the aquifer recharge floor
  Then col-execd triggers the Ecological Floor circuit breaker
  And aborts the actuation envelope with ERR_ECOLOGICAL_FLOOR_BREACH
  And logs the moral dilemma event to the Layer 6 Agora without pulsing hardware.
```

#### Technical Tasks
- [ ] Implement Actuation Envelope data structure and signature verifier.
- [ ] Implement ecological floor invariant checks for water, energy, and soil bounds.
- [ ] Add SITL simulation test verifying pump cutoff when aquifer floor is breached.

---

### User Story 4.4: Task Dependency & Tool Reservation Finite State Machine
**Epic:** Epic 4 (Layer 4: Orchestrator & Execution)  
**Story ID:** SS-EP4-004  
**Persona:** As a Community Steward,  
**Goal:** I want `col-execd` to manage shared physical tool reservations and material prerequisites,  
**Benefit:** So that stewards are not dispatched to field tasks without necessary equipment or materials.

#### Technical Specifications & Architecture
- **Dependency Graph:** Directed Acyclic Graph (DAG) of task prerequisites (e.g. trenching requires excavator availability, PVC conduit inventory $\ge 50\text{ m}$, and electrical permit check).

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Task scheduling blocks on tool reservation conflict
  Given Task B requires the community solar crimping tool
  When Task A currently holds an active reservation on that tool
  Then col-execd keeps Task B in PENDING_DEPENDENCY state
  And immediately transitions Task B to AVAILABLE once Task A checks in the tool.
```

#### Technical Tasks
- [ ] Implement DAG dependency resolver with cycle detection.
- [ ] Implement physical tool check-out/check-in state machine.
- [ ] Add unit test verifying tool reservation release and cascading task unlock.

---

## 7. Epic 5: Layer 5 — Polycentric Governance, Pairwise DIDs & Web of Trust

### User Story 5.1: W3C Decentralized Identifiers (`did:key`, `did:peer`, `did:mesh`) & SQLCipher KMS
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-001  
**Persona:** As a Community Steward,  
**Goal:** I want `col-kmsd` to manage my decentralized identities and sign transactions from a local encrypted vault,  
**Benefit:** So that I interact with community councils without centralized identity providers, cloud authentication, or state ID cards.

#### Technical Specifications & Architecture
- **Daemon:** `col-kmsd` with memory locking (`mlockall`), core dumps disabled (`PR_SET_DUMPABLE = 0`), and isolated UNIX user permissions (`collective-kms`).
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

### User Story 5.2: W3C Verifiable Credentials, BBS+ ZKP Selective Disclosure & Sparse Merkle Tree
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-002  
**Persona:** As a Community Steward,  
**Goal:** I want to present BBS+ zero-knowledge credentials proving specific qualifications without revealing my real-world identity,  
**Benefit:** So that I can claim safety-critical tasks (e.g. high-voltage electrical work) while protecting my personal privacy.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapter 6.
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
**Persona:** As a Community Steward,  
**Goal:** I want my cryptographic identity to be recoverable through an $(m, n)$ Shamir social recovery ceremony,  
**Benefit:** So that losing my physical hardware device does not permanently disenfranchise me from the community commons.

#### Technical Specifications & Architecture
- **FROST Protocol:** Flexible Round-Optimized Schnorr Threshold signatures for multi-party council approvals.
- **Social Recovery:** $(3, 5)$ Shamir's Secret Sharing threshold scheme partitioning master recovery shards among trusted peer guardians.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Social key recovery ceremony restores lost identity
  Given a steward who lost their physical node key and holds 5 designated guardians
  When 3 of the 5 guardians sign a recovery challenge with their respective DIDs
  Then col-kmsd reconstructs the master secret in protected memory
  And re-binds the steward's reputation and standing to their new hardware key
  And revokes the old key across the bioregional Sparse Merkle Tree.
```

#### Technical Tasks
- [ ] Implement FROST $(t, n)$ threshold signature protocol.
- [ ] Implement Shamir's Secret Sharing ($3$-of-$5$) recovery ceremony state machine.
- [ ] Add recovery simulation test verifying state restoration without central servers.

---

### User Story 5.4: EigenTrust Web of Trust Graph Centrality & Sybil Botnet Pruning
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-004  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-kmsd` to calculate reputation centrality across the Web of Trust graph and prune Sybil botnet clusters,  
**Benefit:** So that malicious actors generating millions of synthetic identities cannot hijack commons governance.

#### Technical Specifications & Architecture
- **Algorithm:** EigenTrust power iteration over the local trust graph:
  $$\vec{t}^{(k+1)} = (1 - \alpha) C^T \vec{t}^{(k)} + \alpha \vec{p}$$
  where $C$ is the normalized trust matrix, $\vec{p}$ is the pre-trusted seed vector, and $\alpha = 0.15$.
- **Sybil Resistance:** Graph conductance and min-cut analysis detecting densely connected clusters with sparse exterior edges to authentic stewards.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Sybil botnet cluster detection and slashing
  Given a synthetic cluster of 500 bot accounts mutually endorsing each other
  When col-kmsd computes the EigenTrust power iteration
  Then the bot cluster's trust centrality collapses to near zero due to sparse connectivity to seed stewards
  And col-kmsd flags the cluster for governance review and suppresses their voting weights.
```

#### Technical Tasks
- [ ] Implement sparse-matrix EigenTrust power iteration solver in C++20.
- [ ] Implement graph cut conductance estimator for Sybil boundary detection.
- [ ] Add unit test evaluating 1,000-node synthetic graph convergence in $\le 50\text{ ms}$.

---

### User Story 5.5: Two-Chamber Polycentric Governance & Cascading Edge Slashing
**Epic:** Epic 5 (Layer 5: Governance & Web of Trust)  
**Story ID:** SS-EP5-005  
**Persona:** As a Community Steward,  
**Goal:** I want policy decisions evaluated through two distinct governance chambers,  
**Benefit:** So that survival baselines are guarded equally while technical decisions require demonstrated domain competence.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapter 6.
- **Two Chambers:**
  1. *Commons Chamber:* 1 person, 1 vote (equal Membership Standing). Governs baseline resource rights (water, emergency heat) and structural amendments.
  2. *Stewardship Chamber:* Weighted by domain-specific Reputational Wealth earned through completed tasks. Governs operational parameters and equipment allocation.
- **Decay & Care Leave:** Reputational Wealth decays with a multi-year half-life and is paused during private Care Leave.
- **Cascading Slashing:** If an authorized actor commits verifiable malice or negligence, the trust edge between endorser and actor is slashed proportionally.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Two-Chamber proposal ratification
  Given a proposal to upgrade the main water distribution pump
  When the proposal achieves majority approval in the Commons Chamber (1-person-1-vote)
  And achieves supermajority weighted approval in the Water Stewardship Chamber
  Then col-kmsd cryptographically ratifies the policy and passes it to Layer 4 for execution.
```

#### Technical Tasks
- [ ] Implement Two-Chamber vote tallying engine with quorum verification.
- [ ] Implement Reputational Wealth half-life decay and Care Leave pause mechanisms.
- [ ] Add cascading trust edge slashing calculation.

---

## 8. Epic 6: Layer 6 — Semantic Intent, Knowledge Graphs & The Agora Commons

### User Story 6.1: W3C JSON-LD Semantic Intent Parser & Multi-Layer Traversal Compiler
**Epic:** Epic 6 (Layer 6: Semantic Intent)  
**Story ID:** SS-EP6-001  
**Persona:** As a Community Steward,  
**Goal:** I want `col-commonsd` to parse human intents formatted as W3C JSON-LD and compile them down the 7-layer stack,  
**Benefit:** So that declarative community needs are deterministically translated into cryptographic policies and physical workflows.

#### Technical Specifications & Architecture
- **Daemon:** `col-commonsd` running an asynchronous intent compiler.
- **Ontology Standards:** W3C JSON-LD 1.1, Schema.org vocabularies, and PROV-O provenance ontology.
- **Compiler Chain:**
  $$\text{JSON-LD Intent} \xrightarrow{\text{L6}} \text{Policy Check (L5)} \xrightarrow{\text{L5}} \text{BPMN Workflow (L4)} \xrightarrow{\text{L4}} \text{Ledger Escrow (L3)} \xrightarrow{\text{L3}} \text{UHAI Envelopes (L2)}$$

```json
{
  "@context": [
    "https://www.w3.org/ns/did/v1",
    "https://collective.org/schemas/v1/intent.jsonld"
  ],
  "type": "ResourceReallocationIntent",
  "issuer": "did:key:z6MkuV8...steward01",
  "targetResource": "urn:resource:solar_array_east",
  "requestedExergy": 2500,
  "purpose": "EmergencyClinicRefrigeration",
  "proof": {
    "type": "Ed25519Signature2020",
    "signature": "..."
  }
}
```

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Valid JSON-LD intent compiles down the sovereign stack
  Given a signed JSON-LD ResourceReallocationIntent submitted by a certified clinic steward
  When col-commonsd parses the payload
  Then it validates schema conformance and passes it to Layer 5 for policy authorization
  And upon Layer 5 approval, Layer 4 generates the BPMN task tokens without layer skipping.
```

#### Technical Tasks
- [ ] Integrate lightweight JSON-LD 1.1 parser in C++20/Rust.
- [ ] Implement multi-layer downward intent compilation pipeline.
- [ ] Add negative test verifying rejection of malformed or unauthorized intents.

---

### User Story 6.2: Decentralized Knowledge Artifact Publishing & PROV-O Provenance Tracing
**Epic:** Epic 6 (Layer 6: Semantic Intent)  
**Story ID:** SS-EP6-002  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-commonsd` to publish physical blueprints and firmware manifests with W3C PROV-O provenance trails,  
**Benefit:** So that any node can independently verify the authorship, review history, and physical safety standards of deployed infrastructure.

#### Technical Specifications & Architecture
- **Knowledge Store:** Embedded RDF Quad Store (Oxigraph) at `/var/lib/collective/l6/commons.db`.
- **Provenance:** Every blueprint references contributing stewards, review council attestations, and raw sensor test logs via W3C PROV-O RDF triples (`prov:wasGeneratedBy`, `prov:wasAssociatedWith`).

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Provenance verification of open-source inverter firmware
  Given a firmware binary published with an associated PROV-O knowledge artifact
  When a node operator inspects the artifact
  Then col-commonsd traverses the RDF graph and verifies 3 independent council review signatures
  And links the firmware hash to verified physical test bench exergy logs.
```

#### Technical Tasks
- [ ] Integrate embedded Oxigraph RDF quad-store engine.
- [ ] Implement PROV-O graph constructor for published blueprints.
- [ ] Add SPARQL/JSON-LD query endpoint for provenance inspection.

---

### User Story 6.3: Continuous Double Auction Order Book & Resource Clearing Engine
**Epic:** Epic 6 (Layer 6: Semantic Intent)  
**Story ID:** SS-EP6-003  
**Persona:** As a Community Steward,  
**Goal:** I want `col-commonsd` to match bids and asks for surplus energy, water, and compute in a local continuous double auction,  
**Benefit:** So that community resources are allocated efficiently without price gouging or centralized broker fees.

#### Technical Specifications & Architecture
- **Auction Model:** Continuous Double Auction (CDA) order book with price-time priority. Bids and asks denominated in Valuenomics Exergy Credits.
- **Clearing Frequency:** Fixed 1-second matching tick executing in memory with orders committed to SQLite WAL.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Automated energy double auction clearing
  Given Node A offering 10 kWh of surplus solar power at 1.0 Credit/kWh
  And Node B bidding for 10 kWh of power at 1.0 Credit/kWh
  When the continuous double auction executes its 1-second tick
  Then col-commonsd matches the orders at 1.0 Credit/kWh
  And dispatches an execution intent to Layer 4 to schedule the physical power transfer.
```

#### Technical Tasks
- [ ] Implement in-memory continuous double auction order book in C++20.
- [ ] Implement market clearing engine with anti-wash trading validation.
- [ ] Add unit test verifying matching throughput $\ge 5,000\text{ orders/sec}$.

---

### User Story 6.4: High-Throughput UHAI Client Bridge & Headless IPC / RPC Gateway
**Epic:** Epic 6 (Layer 6: Semantic Intent)  
**Story ID:** SS-EP6-004  
**Persona:** As an External Simulation Client (Oasis Engine),  
**Goal:** I want `col-commonsd` to provide a WebSocket and IPC gateway streaming system observability events and ingesting steward intents,  
**Benefit:** So that graphical frontends, mobile PWAs, and the Oasis simulator visualize system state and submit actions without violating stack encapsulation.

#### Technical Specifications & Architecture
- **Gateways:**
  - Local IPC: `/run/collective/ipc/l6.sock` (Unix Domain Socket).
  - External Network: Localhost WebSocket / gRPC endpoint (`127.0.0.1:9050`) for browser PWAs and Oasis.
- **Event Streaming:** Publishes typed JSON/Protobuf domain events (`WorkTokenAvailable`, `ValuenomicsMinted`, `MunicipalCitationIssued`).

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Oasis frontend connects to L6 event stream
  Given the Sovereign Stack running headlessly on an edge server
  When Oasis connects to the local WebSocket gateway at 127.0.0.1:9050
  Then col-commonsd authenticates the client session
  And streams live domain events with p99 delivery latency <= 2.0 ms
  And ingests user intent envelopes without blocking internal layer daemons.
```

#### Technical Tasks
- [ ] Implement asynchronous WebSocket/gRPC server in `col-commonsd`.
- [ ] Implement event subscription filter matching topic wildcards.
- [ ] Add integration test verifying multi-client streaming under heavy load.

---

## 9. Epic 7: Layer 7 — Legacy Proxy, Adversarial Municipal Emulation & Scenario Omega Decoupling

### User Story 7.1: Discrete Event Simulation (DES) Engine & Configurable Threat Controller
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Adversary)  
**Story ID:** SS-EP7-001  
**Persona:** As an Infrastructure Engineer,  
**Goal:** I want `col-adversaryd` to simulate hostile external pressures using a priority-queue Discrete Event Simulation (DES) engine with configurable threat levels (0–3),  
**Benefit:** So that communities can stress-test their physical and financial resilience against real-world municipal warfare before deploying off-grid.

#### Technical Specifications & Architecture
- **Daemon:** `col-adversaryd` scheduled with `nice +10` background priority.
- **Simulation Engine:** Temporal event loop using a priority queue sorted by simulated calendar timestamps, driven by a seedable deterministic PRNG (PCG-XSH-RR).
- **Threat Levels:**
  - `LEVEL_0 (Benign/Subsidized)`: Favorable utility net-metering, minimal building inspections, low banking fees.
  - `LEVEL_1 (Standard Bureaucracy)`: Real-world utility rates, standard commercial bank fees, triennial municipal inspections.
  - `LEVEL_2 (Hostile Predation)`: Aggressive utility rate spikes (300%), building code compliance orders with 30-day cure periods, merchant payment holds.
  - `LEVEL_3 (Scorched Earth Municipal Siege)`: Utility disconnect threats, civil injunctions, tax liens, bank account freezes, unannounced multi-agency regulatory raids.

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
Scenario: Configurable threat level alters adversarial event frequency
  Given col-adversaryd configured with ThreatLevel = LEVEL_2_HOSTILE and seed = 1337
  When the simulation advances through 90 virtual calendar days
  Then the DES engine emits exactly 3 surprise utility rate adjustments, 2 building code inspections, and 1 merchant reserve hold
  And all events match the deterministic PRNG sequence.
```

#### Technical Tasks
- [ ] Implement priority-queue Discrete Event Simulation core with PCG-XSH-RR PRNG.
- [ ] Implement threat level parameter controller (Levels 0–3).
- [ ] Add unit test verifying bit-exact identical event sequences given identical random seeds.

---

### User Story 7.2: Fictional Utility Monopoly Emulation Suite
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Adversary)  
**Story ID:** SS-EP7-002  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-adversaryd` to emulate the predatory billing and curtailment tactics of three fictional US utility monopolies (*AmeriGrid*, *MetroPower*, *Keystone Gas & Electric*),  
**Benefit:** So that node microgrids learn to anticipate standby fees, demand charges, and grid-tie disconnects.

#### Technical Specifications & Architecture
- **Monopoly Behaviors:**
  - *AmeriGrid:* Imposes high standby/grid-access fees ($50/kW of installed solar), smart meter remote disconnects, and zero net-metering credits.
  - *MetroPower:* Enforces extreme peak-demand pricing ($1.85/kWh during 4 PM–9 PM) and punitive power-factor surcharges.
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

### User Story 7.3: Hostile Municipal Code & Zoning Enforcement 5-State Finite State Machine
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Adversary)  
**Story ID:** SS-EP7-003  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-adversaryd` to model municipal code enforcement through a 5-state finite state machine enforcing NFPA 855 battery limits, UPC greywater rules, and NEC electrical codes,  
**Benefit:** So that nodes navigate inspections, cure periods, and stop-work orders through legal defense mechanisms.

#### Technical Specifications & Architecture
- **5-State Enforcement FSM:**
  1. `STATE_INSPECTION_SCHEDULED`: Randomized or complaint-driven notice of inspection.
  2. `STATE_CITATION_ISSUED`: Formal violation notice with 30-day cure clock and civil fines.
  3. `STATE_STOP_WORK_ORDER`: Active red-tag freezing physical construction.
  4. `STATE_ABATEMENT_HEARING`: Formal municipal zoning board hearing.
  5. `STATE_LEGAL_SEVERANCE`: Civil injunction or property lien issued.
- **Regulatory Codes:**
  - *NFPA 855:* Stationary Energy Storage Systems (mandating 3-foot clearance from property lines and residential separation).
  - *Uniform Plumbing Code (UPC):* Prohibitions on gravity-fed unpermitted greywater systems.
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
- [ ] Implement 5-state municipal enforcement FSM with timer triggers.
- [ ] Implement code validation rulebooks for NFPA 855, UPC, and NEC.
- [ ] Add integration test verifying stop-work order freezing corresponding Layer 4 WorkTokens.

---

### User Story 7.4: Predatory Fiat Banking & Payment Gateway Mock Microservice
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Adversary)  
**Story ID:** SS-EP7-004  
**Persona:** As a Community Steward,  
**Goal:** I want `col-adversaryd` to simulate predatory fiat banking gateways (Stripe, Plaid, ACH) with realistic account freezes and reserves,  
**Benefit:** So that the community's legal umbrella anticipates cash-flow shocks and accelerates the untethering of capital.

#### Technical Specifications & Architecture
- **Mock Microservice:** Embedded HTTP server or direct IPC handler exposing mock Stripe / Plaid endpoints.
- **Predatory Behaviors:**
  - 20% rolling reserve holds lasting 180 days on new commercial accounts.
  - Algorithmic merchant account freezes triggered by rapid transaction volume surges.
  - 3.5% + $0.30 interchange fees and $15.00 dispute chargeback penalties.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Algorithmic payment gateway freeze simulation
  Given a community cooperative selling surplus agricultural produce via the legacy proxy
  When daily fiat revenue exceeds $2,500 across 3 consecutive days
  Then col-adversaryd triggers a mock Stripe algorithmic freeze holding 100% of fiat funds
  And emits an AccountFreezeEvent with a 14-day document demand notice
  And the sovereign stack maintains internal exergy trading with zero operational impact.
```

#### Technical Tasks
- [ ] Implement mock Stripe / Plaid JSON API responder with configurable failure injection.
- [ ] Implement rolling reserve and chargeback dispute logic.
- [ ] Add test asserting that internal Layer 3 ledger balances are 100% insulated from fiat freezes.

---

### User Story 7.5: Tactical Countermeasure Suite & Legal Defense Wrapper
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Adversary)  
**Story ID:** SS-EP7-005  
**Persona:** As a Community Steward,  
**Goal:** I want `col-adversaryd` to provide automated legal defense wrappers (Special Purpose Corporation appeals, Perpetual Purpose Trust easements, inverter cloaking),  
**Benefit:** So that the community shields its human stewards from personal regulatory liability while exhausting bureaucratic delays.

#### Technical Specifications & Architecture
- **Ablative Countermeasures:**
  - *SPC Administrative Appeal:* Generates procedural administrative appeal filings, extending citation cure periods by 60 days.
  - *Perpetual Purpose Trust (PPT) Easement:* Restructures land parcels into non-charitable trusts, asserting religious or agricultural exemptions.
  - *Inverter Grid-Zero Cloaking:* Commands inverters to match instantaneous household load within $\pm 5\text{ W}$, preventing net-metering backfeed detection on smart meters.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: SPC appeal delays municipal enforcement
  Given an outstanding Stop-Work Order on an unpermitted community greenhouse
  When the community legal proxy files an automated SPC Administrative Appeal
  Then col-adversaryd pauses the municipal enforcement clock for 60 virtual calendar days
  And unlocks temporary maintenance WorkTokens in Layer 4 during the appeal pendency.
```

#### Technical Tasks
- [ ] Implement legal document templating engine for SPC appeals and PPT easements.
- [ ] Implement Inverter Grid-Zero control algorithm matching household load within $\pm 5\text{ W}$.
- [ ] Add unit test verifying that filing an appeal halts the citation clock.

---

### User Story 7.6: Scenario Omega (Terminal Decoupling) Lifecycle Engine & Ablative Shredding
**Epic:** Epic 7 (Layer 7: Legacy Proxy & Adversary)  
**Story ID:** SS-EP7-006  
**Persona:** As a Sovereign Node Operator,  
**Goal:** I want `col-adversaryd` to execute the Scenario Omega Terminal Decoupling routine when the node reaches 100% autarky,  
**Benefit:** So that the legacy capitalist membrane is permanently severed, its data shredded, and the node operates perpetually on Layers 1 through 6.

#### Technical Specifications & Architecture
- **Volume 1 Reference:** Volume 1 Chapter 8 and Chapter 11 (Scenario Omega).
- **Autarky Criteria:** 100% self-sufficiency across energy generation, water harvesting, local food baseline, and maintenance tooling for 30 consecutive days.
- **Terminal Decoupling Sequence:**
  1. Issues formal corporate dissolution notice and closes fiat bank accounts.
  2. Disconnects physical grid-tie contactors permanently.
  3. Closes and unlinks `/run/collective/ipc/l7.sock`.
  4. Executes secure cryptographic wiping (`shred -u`) of `/var/lib/collective/l7/adversary.db`.
  5. Terminates `col-adversaryd` process (`SIGTERM` $\to$ `SIGKILL`).
  6. `collective-supervisor` permanently unregisters Layer 7. Layers 1–6 continue executing indefinitely.

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: Scenario Omega terminal decoupling sequence
  Given an edge node verifying 100% thermodynamic autarky for 30 consecutive days
  When the steward council confirms the Scenario Omega execution intent
  Then col-adversaryd disconnects external utility contactors
  And securely shreds its local database file in <= 100 ms
  And unlinks /run/collective/ipc/l7.sock and terminates
  And Layers 1 through 6 continue executing autonomously with zero memory leaks or crashes.
```

#### Technical Tasks
- [ ] Implement autarky metric evaluator querying Layers 1, 3, and 4.
- [ ] Implement atomic database shredding routine (`shred -u` / zeroization).
- [ ] Implement clean IPC socket unmount and supervisor deregistration.
- [ ] Add integration test verifying that Layers 1–6 run continuously following Layer 7 termination.

---

## 10. Epic 8: Cross-Stack Headless Verification, Chaos Engineering & Edge CI/CD Harness

### User Story 8.1: Headless Deterministic Multi-Node Test Runner & State Hash Parity
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-001  
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

### User Story 8.2: Scenario Rho Adversarial Full-Stack Attack & Fuzzing Suite
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-002  
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

### User Story 8.3: 72-Hour Network Partition & Delay-Tolerant Re-Sync Verifier
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-003  
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
- [ ] Implement bash/Python test script setting up `ip netns` and `tc netem` partitions.
- [ ] Implement state hash comparison probe verifying post-healing convergence.
- [ ] Add CI test gate ensuring memory usage does not exceed 320 MB RSS during partitions.

---

### User Story 8.4: Scenario Omega End-to-End Autarky Verification Gate
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-004  
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

### User Story 8.5: Continuous Anti-Bleed Linter, Memory Footprint & Resource Bounding CI Gate
**Epic:** Epic 8 (Cross-Stack Headless Verification)  
**Story ID:** SS-EP8-005  
**Persona:** As a Quality Engineer,  
**Goal:** I want a compile-time static analysis tool and CI linter enforcing the 5 Scope Boundary Rules (SBR-1 through SBR-5),  
**Benefit:** So that no developer or agent can inadvertently re-introduce game mechanics, monolithic threading, or layer-skipping dependencies.

#### Technical Specifications & Architecture
- **Script:** `scripts/lint_anti_bleed.sh` executing in CI:
  1. Forbids imports of `oasis/*`, `SDL2/*`, `webgpu/*`, `dawn/*` inside `src/core/` and `src/daemons/`.
  2. Forbids occurrences of game engine threading, frame budget ceilings, volumetric world types, player kinematics, and game UI markers in stack documentation and headers.
  3. Validates that Layers 1–4 contain zero references to fiat currencies or Layer 7 headers.
  4. Enforces compile-time static assertions on memory layouts:
     - `static_assert(sizeof(TelemetrySample) == 24)`
     - `static_assert(sizeof(ActuatorCommand) == 80)`
     - `static_assert(sizeof(StateDeltaOp) == 24)`

#### Acceptance Criteria (Given / When / Then)
```gherkin
Scenario: CI linter catches prohibited game mechanic import
  Given a pull request introducing client graphics or frame rate constraints into src/core/
  When scripts/lint_anti_bleed.sh runs in CI
  Then the build immediately fails with a descriptive boundary violation error
  And blocks merge until the code conforms to the Scope Boundary Rules.
```

#### Technical Tasks
- [ ] Implement `scripts/lint_anti_bleed.sh` using `ripgrep` and AST regexes.
- [ ] Add static assertions to core C++20 header files.
- [ ] Integrate linter as a mandatory blocking gate in GitHub Actions / local CI.

---

## 11. Quality Assurance Matrix, Definition of Done & Ratification Sign-Off

### 11.1. Infrastructure Definition of Done (DoD) — 10 Strict Invariants
A user story or pull request in the Sovereign Stack is marked **DONE** if and only if it satisfies all 10 invariants:
1. **Autonomous Daemon Conformance:** Operates within an autonomous daemon (`col-*d`) with its own event loop; zero coupling to monolithic thread pools or game loops.
2. **Dedicated Storage Strategy:** State is persisted to the layer's dedicated storage engine; zero unauthorized cross-database foreign keys.
3. **Strict Adjacency Enforced:** State transitions communicate exclusively with adjacent layers ($L_{N \pm 1}$) across Unix Domain Sockets; zero layer skipping.
4. **Hermetic Unit Test Coverage:** $\ge 90\%$ branch test coverage with 100% mocked hardware and network dependencies.
5. **Consumer-Driven Contract Testing:** All IPC interfaces verified against versioned Protobuf / Cap'n Proto schemas with zero breaking changes.
6. **Zero Graphics / Headless Compliance:** Compiles and executes cleanly without display servers, WebGPU, SDL2, or windowing libraries.
7. **Strict Memory & CPU Bounding:** Conforms to cgroups resource ceilings (total stack $\le 320\text{ MB}$ RSS; steady-state CPU $\le 8\%$ on ARM64 Cortex-A72).
8. **Crash Consistency & RTO $\le 500\text{ ms}$:** Recovers state from WAL upon abrupt `SIGKILL` in under 500 ms with zero corruption.
9. **Zero Memory Leaks:** 0 bytes leaked over 72 hours of continuous synthetic traffic verified via AddressSanitizer and Valgrind.
10. **Anti-Bleed Verification:** Passes `scripts/lint_anti_bleed.sh` with zero violations of Scope Boundary Rules SBR-1 through SBR-5.

---

### 11.2. Edge Deployment Hardware Qualification Matrix

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              EDGE HARDWARE DEPLOYMENT QUALIFICATION MATRIX                             │
├──────────────────────┬──────────────────────────────┬──────────────────────────┬───────────────────────┤
│ Target Platform      │ Hardware Specifications      │ Operating System         │ Primary Daemons Run   │
├──────────────────────┼──────────────────────────────┼──────────────────────────┼───────────────────────┤
│ **Tier 1: Master     │ Raspberry Pi CM4 / Rockchip  │ Alpine Linux / Debian 12 │ All 7 Daemons (L1–L7);│
│ Bioregional Hub**    │ RK3588 (4-8 Core ARM64, 4GB) │ (kernel 6.6+ PREEMPT_RT) │ Full node supervisor  │
├──────────────────────┼──────────────────────────────┼──────────────────────────┼───────────────────────┤
│ **Tier 2: Off-Grid   │ Allwinner H616 / BCM2711     │ Buildroot Minimal Linux  │ Daemons L1–L4;        │
│ Solar / Shed Node**  │ (Quad ARM64, 1GB RAM, eMMC)  │ (Read-only rootfs)       │ Headless microgrid hub│
├──────────────────────┼──────────────────────────────┼──────────────────────────┼───────────────────────┤
│ **Tier 3: Peripheral │ ESP32-S3 / STM32L4+          │ Zephyr RTOS / FreeRTOS   │ Bare-metal HAL (L1);  │
│ Sensor / Valve Tag** │ (Tensilica / ARM-M4, 512KB)  │ bare-metal C++20         │ Reticulum LoRa bridge │
└──────────────────────┴──────────────────────────────┴──────────────────────────┴───────────────────────┘
```

---

### 11.3. Architectural Council Ratification Sign-Off
The Round 2 Architectural Council hereby concludes its deliberations. All debate points have been resolved, all critiques incorporated, and all five stakeholder disciplines have verified the complete decoupling of the Sovereign Stack from client-side game mechanics.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                             ARCHITECTURAL COUNCIL RATIFICATION VOTE                              │
├────────────────────────────────┬───────────────────────────────┬─────────────────┬───────────────┤
│ Persona                        │ Document / Milestone          │ Disciplinary OK │ Final Ballot  │
├────────────────────────────────┼───────────────────────────────┼─────────────────┼───────────────┤
│ Product Owner (`explorer_po`)  │ Roadmap, Personas & Epics 1–8 │ APPROVED        │ UNANIMOUS YES │
│ Sovereign Specialist (`sov_r2`)│ Volume 1 Fidelity, L1–L7, PoTW│ APPROVED        │ UNANIMOUS YES │
│ Quality Engineer (`qe_r2`)     │ Anti-Monolith, Isolation, SLOs│ APPROVED        │ UNANIMOUS YES │
│ Stack Engineer (`stack_r2`)    │ 7 Daemons, UDS IPC, Bounded   │ APPROVED        │ UNANIMOUS YES │
│ Oasis Engineer (`oasis_r2`)    │ Purge Game Mechanics, UHAI   │ APPROVED        │ UNANIMOUS YES │
├────────────────────────────────┴───────────────────────────────┴─────────────────┴───────────────┤
│                                  # Unanimous Consensus Reached                                   │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

**Ratified:** 2026-10-07  
**Effective Baseline:** Version 2.0.0-RELEASE (`SOVEREIGN_STACK_BACKLOG.md`)  
**Signed by:**
- *Product Owner / Orchestrator (`explorer_po_r2`)*
- *Sovereign Stack Specialist (`explorer_sovereign_r2`)*
- *Quality Engineer (`explorer_qe_r2`)*
- *Stack Engineer (`explorer_stack_r2`)*
- *Oasis Engineer (`explorer_oasis_r2`)*
