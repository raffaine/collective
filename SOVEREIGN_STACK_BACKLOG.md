# THE SOVEREIGN STACK — PRODUCT BACKLOG (LAYERS 1–7)
**Document Version:** 1.0.0-RELEASE  
**Status:** Complete, Ratified & Authoritative  
**Milestone:** Sovereign Stack Protocols, Specifications, Runtimes & Layer 7 Adversarial Emulation  
**Target Environments:** Native Desktop / Edge Daemons (C++20 / Linux / ARM64 / RISC-V) & WebAssembly (WASM / WebGPU / Emscripten)  
**Architectural Baseline:** 7-Layer Sovereign Stack Model, Dual-Runtime Paradigm & Data-Oriented Design (DOD)  
**Consensus Status:** Unanimous Consensus Reached (Internal Council Triad)

---

## 1. Executive Summary & Architectural Council Charter

### 1.1. Architectural Council Charter & Council Triad Verdict
The Architectural Council—composed of the **Product Owner (`explorer_po_r1`)**, the **Software Architect (`explorer_arch_r1`)**, and the **Sovereign Stack Expert (`explorer_sovereign_r1`)**—has concluded an exhaustive 3-round technical debate defining the architectural, cryptographic, thermodynamic, and gameplay specifications for the Sovereign Stack.

```
       ┌─────────────────────────────────────────────────────────────┐
       │             ARCHITECTURAL COUNCIL TRIAD CONSENSUS           │
       ├──────────────────────────────┬──────────────────────────────┤
       │ Product Owner (PO)           │ Gameplay loop, player agency,│
       │                              │ systemic pacing, DoD gates   │
       ├──────────────────────────────┼──────────────────────────────┤
       │ Software Architect (SA)      │ C++20/WASM dual runtime, UHAI│
       │                              │ memory packing, frame budget │
       ├──────────────────────────────┼──────────────────────────────┤
       │ Sovereign Stack Expert (SSE) │ Cryptography, L1-L7 specs,   │
       │                              │ mesh routing, L7 adversary   │
       └──────────────────────────────┴──────────────────────────────┘
```

The council formally confirms that **Unanimous Consensus Reached** has occurred across all technical, economic, and systemic domains:
1. **Decoupled Cryptographic Worker:** All Ed25519, BLS12-381, and zero-knowledge proof verifications are isolated to background worker threads via lock-free Single-Producer Single-Consumer (SPSC) ring buffers, ensuring zero frame drops on the main simulation thread.
2. **Dual-Runtime Execution Core:** A shared C++20 static library (`liboasis_core`) compiles both to WebAssembly/WebGPU for client-side Oasis simulation and to POSIX-compliant native binaries for headless Collective edge node daemons (`collectived`).
3. **Layer 7 Adversarial Engine:** Fictional US utility monopolies (*AmeriGrid*, *MetroPower*, *Keystone Gas & Electric*), municipal zoning codes (NFPA 855, UPC, NEC), and predatory fiat banking APIs (*Stripe*, *Plaid*, ACH) are fully emulated as a configurable Discrete Event Simulation (DES) queue, driving the player through the "Funnel to Freedom."
4. **The Ablative Shield & Terminal Decoupling:** Layer 7 is architected as an ablative legal/fiat membrane (Social Purpose Corporations, Perpetual Purpose Trusts, Trojan commercial services) that is permanently unmounted and zeroized when a node achieves 100% thermodynamic autarky (Scenario Omega).

---

### 1.2. The 7-Layer Traversal Protocol
The Sovereign Stack enforces a strict, hierarchical traversal protocol (`docs/01_ARCHITECTURE.md`). In accordance with the **Strict Traversal Principle ("No Layer Skipping")**, state mutations and player intents must traverse the layers deterministically:

$$\text{Layer 6 (Semantic Lens)} \longrightarrow \text{Layer 5 (Policy/WoT)} \longrightarrow \text{Layer 4 (Orchestration)} \longrightarrow \text{Layer 3 (Ledger/CRDT)} \longrightarrow \text{Layer 2 (Digital Twin)} \longrightarrow \text{Layer 1 (Physical Reality)}$$

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       THE 7-LAYER SOVEREIGN STACK                                      │
├───────┬─────────────────────────┬──────────────────────────────────────────┬───────────────────────────┤
│ LAYER │ TECHNICAL DESIGNATION   │ OASIS SIMULATED RUNTIME (WASM / GAME)    │ PHYSICAL EDGE NODE DAEMON │
├───────┼─────────────────────────┼──────────────────────────────────────────┼───────────────────────────┤
│ **7** │ **Legacy Proxy &        │ Discrete Event Simulation (DES) Queue:   │ Ablative Shield Gateway:  │
│       │ Adversarial Membrane**  │ - Fictional Utility Monopolies (3 firms) │ - Stripe / Plaid Webhooks │
│       │                         │ - Municipal Code Enforcement FSM         │ - Automated SPC Filings   │
│       │                         │ - Adversary Levels 0–3 Configuration     │ - Terminal Decoupling     │
├───────┼─────────────────────────┼──────────────────────────────────────────┼───────────────────────────┤
│ **6** │ **Semantic & Intent     │ Cadastral Blueprint Slate:               │ Sovereign Web App / CLI:  │
│       │ Lens**                  │ - 45° Axonometric CAD overlay            │ - Local-First PWA client  │
│       │                         │ - JSON-LD Intent generation              │ - W3C JSON-LD Intent sign │
│       │                         │ - In-memory Ed25519 signing              │ - IPFS Content Addressing │
├───────┼─────────────────────────┼──────────────────────────────────────────┼───────────────────────────┤
│ **5** │ **Policy & Web of       │ In-Memory SovereignIdentityStore:        │ W3C DID / VC Validator:   │
│       │ Trust**                 │ - Trust Ring adjacency graph             │ - did:key / did:mesh      │
│       │                         │ - Dijkstra trust distance calculation    │ - Arkworks ZK-SNARK engine│
│       │                         │ - Social slashing & Sybil cluster damp   │ - Dynamic edge slashing   │
├───────┼─────────────────────────┼──────────────────────────────────────────┼───────────────────────────┤
│ **4** │ **Orchestration & State │ Embedded pugixml BPMN 2.0 Runner:        │ Headless BPMN Daemon:     │
│       │ Automation**            │ - Deterministic 10 Hz Logic Tick         │ - wasmtime Workflow Engine│
│       │                         │ - WorkToken physical task queues         │ - Physical task dispatch  │
│       │                         │ - Caloric burn / tool wear evaluation    │ - Cryptographic escrow    │
├───────┼─────────────────────────┼──────────────────────────────────────────┼───────────────────────────┤
│ **3** │ **Ledger & CRDT State   │ Operational 24-Byte Delta Mesh:          │ libp2p Gossipsub Daemon:  │
│       │ Replication**           │ - VoxelMutationOp ring buffers           │ - libp2p (QUIC/TCP/LoRa)  │
│       │                         │ - Kulkarni-Demir HLC causal ordering     │ - Automerge/Yrs CRDT store│
│       │                         │ - WebRTC Gossipsub simulation            │ - Proof of Stewardship   │
├───────┼─────────────────────────┼──────────────────────────────────────────┼───────────────────────────┤
│ **2** │ **Digital Twin & Edge   │ Software-in-the-Loop (SITL) Bridge:      │ Hardware Telemetry Daemon:│
│       │ Telemetry**             │ - Virtual IoT sensor generator           │ - Modbus RTU / CAN-bus    │
│       │                         │ - Lock-free SPSC telemetry queue         │ - ATECC608A Secure Element│
│       │                         │ - Thermodynamic plausibility filter      │ - GPIO relay actuation    │
├───────┼─────────────────────────┼──────────────────────────────────────────┼───────────────────────────┤
│ **1** │ **Physical Reality &    │ 32-Bit Quantized Voxel Envelope:         │ Physical Thermodynamics:  │
│       │ Thermodynamics**        │ - 96x96x64 Envelope (Lot 402 Genesis)    │ - Solar MPPT, LiFePO4 BESS│
│       │                         │ - Fourier heat & Darcy fluid equations   │ - Biomass pyrolysis retorts│
│       │                         │ - 12-byte EntityPhysics mortal bodies    │ - Human steward labor     │
└───────┴─────────────────────────┴──────────────────────────────────────────┴───────────────────────────┘
```

---

### 1.3. The Dual-Runtime Execution Paradigm
To guarantee that the Oasis game engine functions as an authentic **Shadow Simulator** for physical nodes, both execution environments execute the identical C++20 core logic:

1. **Oasis Simulated Runtime (Client-Side WASM / Desktop):**
   - Compiles via Emscripten to WebAssembly (`oasis_core.wasm`) and native desktop shells (SDL2 / Dawn WebGPU).
   - Executes within a sandboxed browser environment with direct WebGPU screen-space 3D DDA compute ray-marching.
   - Binds to the **Universal Hardware Abstraction Interface (UHAI)** using Software-in-the-Loop (SITL) simulated sensors.
   - Maintains an in-memory Discrete Event Simulation (DES) bus for Layer 7 municipal friction.

2. **Collective Edge Node Daemon Runtime (`collectived`):**
   - Compiles natively for POSIX targets (Linux ARM64 / RISC-V) running on low-power hardware (Raspberry Pi CM4, Rockchip RK3588, ESP32-S3).
   - Executes headless background daemons interfacing with real RS485 Modbus charge controllers, CAN-bus battery management systems, Semtech SX1262 LoRa transceivers, and hardware secure elements (ATECC608A).
   - Connects to real-world fiat gateways via isolated background webhooks, and executes automated legal filings through the Social Purpose Corporation (SPC) wrapper.

---

### 1.4. The Sovereign Trojan Horse & Scenario Omega Horizon
Oasis is engineered to resolve the **Dual Horizon**:
- **Ludic Horizon:** A compelling, tactile survival management game combining the thermodynamic detail of *Dwarf Fortress*, the emotional agency and metabolic vulnerability of *The Sims*, and the macro-infrastructure orchestration of *Cities: Skylines*.
- **Sovereign Horizon:** A real-world onboarding curriculum and deployment engine. Every blueprint designed in-game compiles into real-world BPMN 2.0 workflows; every token minted represents verified thermodynamic negentropy; every legal wrapper models genuine corporate protection.
- **Scenario Omega (Terminal Decoupling):** The culmination of the node lifecycle. When a node achieves 100% autarky across energy, water, nutrition, and fabrication, Layer 7 is unmounted. The physical utility drop line is severed, fiat bank accounts are zeroized into physical capital, and the node operates perpetually on Layers 1 through 6.

---

## 2. Architectural Foundations & Structural Constraints

### 2.1. Universal Hardware Abstraction Interface (UHAI)
The Universal Hardware Abstraction Interface (UHAI) is the polymorphic boundary isolating higher-level consensus, orchestration, and simulation logic from the physical I/O transport.

```cpp
namespace collective::uhai {

enum class UnitType : uint8_t {
    WATTS          = 0,
    CELSIUS        = 1,
    LITERS_PER_MIN = 2,
    VOLTS          = 3,
    AMPERES        = 4,
    PRESSURE_KPA   = 5,
    PULSE_COUNT    = 6
};

enum QualityFlags : uint8_t {
    QUALITY_OK             = 0x00,
    QUALITY_SIMULATED      = 0x01,
    QUALITY_HARDWARE_ROOT  = 0x02,
    QUALITY_OUT_OF_BOUNDS  = 0x04,
    QUALITY_SPOOF_SUSPECT  = 0x08
};

struct alignas(8) TelemetrySample {
    uint64_t timestamp_hlc;      // Hybrid Logical Clock timestamp
    uint32_t channel_id;         // Unique sensor register ID
    float    value;              // Calibrated engineering unit
    UnitType unit_type;          // Physical quantity
    uint8_t  quality_flags;      // Bitmask of validation flags
    uint16_t sensor_node_id;     // Originating DID slot or physical address
};
static_assert(sizeof(TelemetrySample) == 24, "TelemetrySample must be 24 bytes.");

struct alignas(8) ActuatorCommand {
    uint64_t command_id;         // Monotonic command UUID
    uint32_t device_id;          // Physical or virtual actuator register
    float    target_state;       // Target duty cycle, valve angle, or switch state
    uint32_t duration_ms;        // Pulse width or timeout
    uint8_t  auth_signature[64]; // Ed25519 signature from Layer 4 BPMN execution key
};
static_assert(sizeof(ActuatorCommand) == 80, "ActuatorCommand must be 80 bytes.");

class IDigitalTwinBridge {
public:
    virtual ~IDigitalTwinBridge() = default;
    virtual bool IngestTelemetry(const TelemetrySample& sample) = 0;
    virtual bool DispatchActuation(const ActuatorCommand& cmd) = 0;
    virtual bool ValidateThermodynamicPlausibility(const TelemetrySample& sample) = 0;
};

} // namespace collective::uhai
```

---

### 2.2. Memory Layout Constraints & Data-Oriented Design (DOD)
To ensure absolute L1 CPU cache residency, predictable memory bandwidth, and zero dynamic heap allocation in the hot simulation loops, all core entities adhere to strict struct packing and compile-time static assertions:

#### 1. 32-Bit Packed Voxel Struct (4 Bytes)
```cpp
namespace oasis {
struct alignas(4) Voxel {
    uint8_t material_id;  // 0=Air, 1=Asphalt, 2=Fungal Loam, 3=PVC, 4=Clay, 5=Biochar, 6=Water, 7=Rubble
    uint8_t moisture;     // 0=Bone Dry, 255=Fully Saturated (Hydraulic Percolation)
    uint8_t temperature;  // 0=-20°C, 128=20°C, 255=100°C+ (Fourier Heat Diffusion)
    uint8_t metadata;     // Bit 0: LegacyTethered, Bit 1: Actuator, Bit 2: Sensor, Bits 3-7: Stress/Flow
};
static_assert(sizeof(Voxel) == 4, "Voxel struct must remain exactly 4 bytes.");
}
```
*Spatial Envelope:* Divided into $32 \times 32 \times 32$ chunks ($131,072\text{ bytes}$ per chunk). Genesis Lot 402 is modeled as an envelope of $3 \times 3 \times 2 = 18$ contiguous chunks ($96 \times 96 \times 64$ voxels), requiring exactly **$2,359,296\text{ bytes}$ ($2.359\text{ MB}$)** of pre-allocated contiguous memory.

#### 2. 12-Byte Contiguous EntityPhysics Struct (L1 Cache Resident)
```cpp
namespace oasis {
struct alignas(4) EntityPhysics {
    uint16_t x, y, z;      // Decimeter coordinates (0.1m resolution within envelope)
    uint8_t  entity_type; // 0=NPC, 1=Citizen, 2=Founder 01
    uint8_t  state;       // FSM State (Idle, Walking, Working, Exhausted, Hypothermic)
    uint8_t  temperature; // Quantized core body temp (30.0°C to 42.0°C)
    uint8_t  hydration;   // 0 to 255 (metabolic dehydration threshold < 50)
    uint8_t  calories;    // 0 to 255 (scales linearly to 3,000 kcal)
    uint8_t  fatigue;     // 0 to 255 (Altruism Fatigue / sleep deprivation)
};
static_assert(sizeof(EntityPhysics) == 12, "EntityPhysics must remain exactly 12 bytes.");
}
```
*Cache Footprint:* 1,024 active entities consume only **$12.288\text{ KB}$**, fitting entirely within the L1 data cache of any modern CPU core. Cold data (DIDs, public keys, social graph indices) is stored out-of-band in `SovereignIdentityStore`.

#### 3. 24-Byte Operational CRDT Delta Struct (`VoxelMutationOp`)
```cpp
namespace collective::crdt {
struct alignas(8) VoxelMutationOp {
    uint64_t logical_clock;     // Hybrid Logical Clock (HLC)
    uint32_t voxel_index;       // Spatial linear offset (0 to 589,823)
    Voxel    old_value;         // 4 bytes: State before mutation (inverse rollback log)
    Voxel    new_value;         // 4 bytes: State after mutation
    uint16_t author_entity_id;  // Author DID slot index
    uint16_t signature_slot;    // Slot index in signed cryptographic delta pool
};
static_assert(sizeof(VoxelMutationOp) == 24, "VoxelMutationOp must remain exactly 24 bytes.");
}
```

---

### 2.3. Engine Frame Budget & Performance Ceiling (60 FPS / 8.75 ms)
The Oasis simulation engine maintains a non-negotiable **60 FPS** frame rate, enforcing an **$8.75\text{ ms}$** total frame processing ceiling. The per-subsystem frame budget is strictly enforced across three execution threads:

```
┌────────────────────────────────────────────────────────────────────────────────────────┐
│                        60 FPS / 8.75 ms ENGINE FRAME BUDGET CEILING                    │
├───────────────────────────────────────────────────────────────────┬────────────────────┤
│ Subsystem / Processing Phase                                      │ Allocated Budget   │
├───────────────────────────────────────────────────────────────────┼────────────────────┤
│ Layer 1 & Layer 2: Kinematics, Thermal/Fluid Diffusion, UHAI SITL │ 0.30 ms            │
│ Layer 3 & Layer 5: CRDT Ingress Drain, HLC Order, WoT Trust Ring  │ 0.15 ms            │
│ Layer 4: Deterministic BPMN 2.0 Logic Tick & Task Queues         │ 0.15 ms            │
│ Layer 7: Adversarial Municipal Simulation & Utility DES Queue    │ 0.15 ms            │
│ WebGPU Screen-Space 3D DDA Compute Ray-Marching & G-Buffer Pass   │ 8.00 ms            │
├───────────────────────────────────────────────────────────────────┼────────────────────┤
│ TOTAL FRAME EXECUTION TIME                                        │ 8.75 ms            │
└───────────────────────────────────────────────────────────────────┴────────────────────┘
```

#### Threading Model & Lock-Free Architecture
- **Thread 1 (Main Simulation & Render Thread):** Executes the 60 Hz kinematic controller, 10 Hz voxel physics (amortized), WebGPU 3D ray-marching, and drains local lock-free SPSC queues. Zero dynamic heap allocation (`malloc`, `new`) permitted in the tick loop.
- **Thread 2 (Asynchronous Mesh & CRDT Worker):** Handles libp2p Gossipsub network I/O, WebRTC data channels, Reticulum packet frames, and disk persistence to OPFS/LMDB.
- **Thread 3 (Cryptographic & Layer 7 Worker):** Executes Ed25519 signature verifications, BBS+ zero-knowledge proofs, mock Stripe/Plaid HTTP webhooks, and BPMN XML compilation without stalling the render loop.

---

### 2.4. Anti-Bleed Containment Boundary & Static Analysis
To preserve the sovereignty and mathematical purity of the underlying physical and consensus layers, the codebase enforces an ironclad **Anti-Bleed Containment Boundary**:

```
[Layer 7: Legacy Proxy / Fiat / Zoning]
                  │
                  ▼  (Strict Firewall: Attestation Envelopes & BPMN Parameters ONLY)
────────────────────────────────────────────────────────────────────────────
[Layers 1–4: Physics, UHAI, CRDT Ledgers, BPMN Workflows]
(INVIOLABLE: Zero references to fiat currency, zoning codes, or legacy IDs)
```

1. **Naming & Variable Firewall:** Layers 1 through 4 source code must *never* contain variables named `fiat_value`, `usd_price`, `sales_tax`, `zoning_violation`, or `utility_bill`.
2. **Compile-Time Linter Enforcement:** Continuous Integration runs an automated grep verification script (`scripts/lint_anti_bleed.sh`) that halts the build if any source file under `core/1_physics/`, `core/2_uhai/`, `core/3_crdt/`, or `core/4_bpmn/` imports Layer 7 headers or contains fiat tokens.
3. **Ablative Self-Liquidation:** Layer 7 exposes the `ITerminalDecoupling` interface. When invoked, Layer 7 releases all memory allocations, shuts down HTTP mock daemons, and completely decouples from the running engine.

---

## 3. Epic 1: Physical Mesh, Compute & Low-Power Hardware Abstraction (Layers 1 & 2)

### Epic Overview
- **Strategic Scope:** Deliver the low-level physical compute, energy harvesting, sensor telemetry, and delay-tolerant mesh transport layers. This epic establishes the Universal Hardware Abstraction Interface (UHAI) connecting both the Oasis simulation and physical edge nodes, implements drivers for RS485 Modbus MPPT controllers and LiFePO4 CAN-bus BMS systems, and deploys multi-bearer transport over Reticulum (RNS), Semtech SX1262 LoRa, and 802.11s Wi-Fi.
- **Architectural Layers:** Layer 1 (Physical Reality / Hardware) & Layer 2 (Digital Twin / Hardware Abstraction).
- **Target Environments:** Oasis C++20 / WASM SITL and Physical ARM64 / ESP32-S3 / Linux hardware.

---

### Story 1.1: Multi-Tier Compute Node Architecture & Secure Hardware Root-of-Trust
- **User Story:** As an edge node deployment engineer, I want a standardized three-tier hardware compute specification paired with a cryptographic secure element, so that physical nodes operate autonomously off-grid and defend their private keys against physical chassis tampering.
- **Technical Context:** Physical nodes run across three tiers: Tier 1 Hub Node (Raspberry Pi CM4 or Rockchip RK3588 with NVMe and Microchip ATECC608A secure element), Tier 2 Field Node (ESP32-S3 running Zephyr RTOS/Rust with isolated RS485 and CAN transceivers), and Tier 3 Sentinel Node (STM32L4/nRF52840 low-power MCU with SX1262 LoRa). The ATECC608A provides hardware TRNG, root Ed25519 identity key storage, and instant key zeroization upon enclosure breach.

#### Gherkin Scenarios
```gherkin
Scenario: Autonomous boot and cryptographic attestation on Tier 1 Hub Node
  Given a Raspberry Pi CM4 node with an attached ATECC608A secure element on I2C address 0x60
  When the collectived system service initializes during boot
  Then the system verifies the boot partition signature via Secure Boot
  And extracts the node's public key "did:key" without exposing the private key
  And initializes the hardware watchdog timer with a 10-second timeout.

Scenario: Physical enclosure tamper detection and key zeroization
  Given a Tier 1 Hub Node operating in "Hostile Territory" mode
  When the chassis microswitch detects an enclosure breach or internal photodiode detects light
  Then the ATECC608A triggers a hardware zeroization of ephemeral session keys within 12 microseconds
  And writes a tamper attestation record to non-volatile flash
  And halts all active network interfaces.
```

#### Technical Tasks
- [ ] Implement Linux I2C driver abstraction for Microchip ATECC608A / OPTIGA Trust M secure elements (`hardware/sec_element.cpp`).
- [ ] Configure hardware watchdog daemon (`watchdogd`) with dual-stage software task supervision.
- [ ] Implement GPIO chassis intrusion interrupt handler with microsecond zeroization callback.
- [ ] Build Zephyr RTOS / Rust Embedded HAL firmware template for ESP32-S3 field nodes (`firmware/esp32_field_node/`).

#### Interfaces & Data Structures
```cpp
namespace collective::hardware {
struct TamperStatus {
    bool chassis_breached;
    bool light_detected;
    uint64_t tamper_timestamp_posix;
    uint32_t zeroization_latency_us;
};

class ISecureElement {
public:
    virtual ~ISecureElement() = default;
    virtual bool GetPublicKey(std::array<uint8_t, 32>& out_pubkey) = 0;
    virtual bool SignDigest(const std::array<uint8_t, 32>& digest, std::array<uint8_t, 64>& out_sig) = 0;
    virtual bool ZeroizeSessionKeys() = 0;
};
}
```

#### Definition of Done
- Complete unit tests passing on simulated hardware and physical Raspberry Pi CM4.
- Zeroization latency verified at $\le 12\text{ }\mu\text{s}$ under oscilloscope measurement.
- Memory leak free under ASan; zero dynamic allocations in telemetry loops.

---

### Story 1.2: RS485 Modbus RTU Energy Telemetry & LiFePO4 CAN-Bus BMS Drivers
- **User Story:** As an off-grid microgrid operator, I want an asynchronous RS485 Modbus and CAN-bus driver suite, so that solar generation and battery cell metrics are continuously ingested into Layer 2 without blocking engine ticks.
- **Technical Context:** Off-grid power relies on EPEver Tracer-AN / Victron MPPT charge controllers and JK-BMS / Daly Smart BMS units. The driver polls registers over RS485 (9600 baud, 8N1) and CAN 2.0B (115200 baud) at 1 Hz, parsing array voltage ($V_{pv}$), charging current ($I_{pv}$), cell millivolt delta ($\Delta V_{cell} \le 15\text{ mV}$), and pack State-of-Charge (SoC) into `TelemetrySample` structs.

#### Gherkin Scenarios
```gherkin
Scenario: Ingestion of solar MPPT telemetry via RS485 Modbus RTU
  Given an RS485 Modbus bus with an EPEver MPPT controller at slave address 0x01
  When the Modbus driver polls input register 0x3100 (PV Array Voltage) and 0x3101 (PV Current)
  Then raw binary responses are validated against Modbus CRC-16
  And converted into a 24-byte TelemetrySample with UnitType::VOLTS and UnitType::AMPERES
  And pushed into the lock-free SPSC telemetry queue in less than 5 milliseconds.

Scenario: BMS cell voltage imbalance alert and relay protection
  Given an active LiFePO4 battery pack with 16 prismatic cells connected via CAN-bus
  When cell 7 voltage drops to 2.80V while cell 12 sits at 3.35V (delta > 500mV)
  Then the BMS driver flags the TelemetrySample with QUALITY_OUT_OF_BOUNDS
  And issues an emergency actuation command to disengage the low-voltage discharge MOSFET.
```

#### Technical Tasks
- [ ] Implement non-blocking POSIX serial driver for RS485 Modbus RTU with CRC-16 table lookup (`drivers/modbus_rtu.cpp`).
- [ ] Implement SocketCAN interface parser for JK-BMS / Daly BMS telemetries (`drivers/can_bms.cpp`).
- [ ] Construct lock-free SPSC ring buffer connecting driver threads to the UHAI engine bridge.
- [ ] Implement simulated MPPT and BMS telemetry generator for the Oasis SITL WASM client.

#### Interfaces & Data Structures
```cpp
namespace collective::drivers {
struct BmsTelemetryFrame {
    uint16_t cell_voltages_mv[16];
    int16_t  temperatures_c[4];
    uint32_t pack_voltage_mv;
    int32_t  current_ma;
    uint8_t  state_of_charge_pct;
    uint8_t  alarm_flags;
};
}
```

#### Definition of Done
- Sustained 1 Hz polling across RS485 and CAN-bus with zero lost frames over 24 hours.
- Ingestion latency from wire to SPSC ring buffer $\le 5\text{ ms}$.
- Simulated SITL driver produces bit-exact identical telemetry structures in browser WASM.

---

### Story 1.3: Reticulum Cryptographic Mesh Routing Engine
- **User Story:** As an autonomous community member, I want an infrastructure-free, cryptographically addressed mesh routing engine, so that nodes communicate and replicate state across delay-tolerant links without relying on IP addresses, DNS, or central servers.
- **Technical Context:** The Reticulum Network Stack (RNS) is the core delay-tolerant transport engine. Destinations are 128-bit truncated SHA-256 hashes of Ed25519/X25519 public keys. Payloads are encrypted point-to-point via Curve25519 ephemeral ECDH and ChaCha20-Poly1305. Packet frames support store-and-forward caching for intermittent links.

#### Gherkin Scenarios
```gherkin
Scenario: Point-to-point Reticulum packet transmission over zero-infrastructure mesh
  Given Node Alpha and Node Beta with established 128-bit Reticulum destination hashes
  When Node Alpha transmits an encrypted CRDT delta packet to Node Beta across an unaddressed mesh link
  Then the packet is authenticated with Curve25519 ephemeral ECDH and ChaCha20-Poly1305
  And Node Beta decrypts the payload without requiring IP routing or central name resolution.

Scenario: Delay-tolerant store-and-forward packet delivery during mesh partition
  Given Node Gamma separated from Node Delta by an active network partition
  When Node Gamma dispatches a Reticulum packet addressed to Node Delta
  Then intermediary Node Epsilon caches the packet in its non-volatile delay-tolerant buffer
  And forwards the packet to Node Delta upon link re-establishment without data corruption.
```

#### Technical Tasks
- [ ] Implement 128-bit destination hash generator and routing table in C++20 (`reticulum/destination.cpp`).
- [ ] Implement packet serialization, framing, and ChaCha20-Poly1305 AEAD envelope wrapping (`reticulum/packet.cpp`).
- [ ] Build delay-tolerant store-and-forward caching queue with LRU cache eviction and TTL expiration.
- [ ] Integrate Reticulum packet dispatcher into the dual-runtime `ISovereignBus`.

#### Interfaces & Data Structures
```cpp
namespace collective::reticulum {
struct ReticulumPacketHeader {
    uint8_t  flags;                     // Header type, propagation flags
    uint8_t  hops;                      // Hop counter
    uint8_t  destination_hash[16];      // 128-bit truncated SHA-256 hash
    uint8_t  context_type;              // 0=Telemetry, 1=CRDT Delta, 2=Message
};
}
```

#### Definition of Done
- Multi-hop routing verified across 5 simulated nodes with zero packet drop in nominal conditions.
- Store-and-forward cache verified to preserve packet integrity over 72-hour simulated partitions.
- Header serialization strictly packed with zero padding bytes.

---

### Story 1.4: Multi-Bearer Transport Manager & Semtech SX1262 LoRa Radio Interface
- **User Story:** As a mesh communications engineer, I want an adaptive transport manager coordinating LoRa, 802.11s Wi-Fi, and physical sneakernet bearers, so that network traffic automatically routes over the optimal medium and fails over instantaneously during jamming or hardware outages.
- **Technical Context:** The multi-bearer manager abstracts physical channels: Semtech SX1262 LoRa (915 MHz US / 868 MHz EU at SF7–SF12), 802.11s Wi-Fi mesh (B.A.T.M.A.N.-adv), and physical USB/BLE sneakernet tokens. It routes high-throughput traffic (blueprints, binary blobs) over Wi-Fi, falling over to LoRa for critical CRDT deltas and telemetry in $< 500\text{ ms}$ upon link degradation.

#### Gherkin Scenarios
```gherkin
Scenario: Automatic failover from Wi-Fi mesh to SX1262 LoRa during RF jamming
  Given two nodes communicating over an active 802.11s Wi-Fi mesh connection
  When Wi-Fi packet loss exceeds 80% or RSSI drops below -88 dBm due to interference
  Then the multi-bearer manager switches transport to the Semtech SX1262 915 MHz LoRa link within 500 ms
  And compresses outgoing CRDT deltas to fit within 256-byte LoRa packet frames
  And causal message delivery order is strictly preserved.

Scenario: Sneakernet store-and-forward token import
  Given a mobile steward node with a cryptographically signed USB sneakernet token
  When the token is plugged into an isolated Tier 1 Hub Node
  Then the transport manager reads the bundled Reticulum packet bundles
  And validates authorization signatures before injecting them into the local CRDT ingress queue.
```

#### Technical Tasks
- [ ] Implement SPI driver for Semtech SX1262 LoRa transceiver supporting CAD and duty cycle management (`drivers/sx1262.cpp`).
- [ ] Implement link quality monitor evaluating RSSI, SNR, and packet loss ratios.
- [ ] Build adaptive dynamic bearer router with fallback priority matrix (`transport/multi_bearer.cpp`).
- [ ] Implement sneakernet directory watcher and cryptographic bundle importer.

#### Interfaces & Data Structures
```cpp
namespace collective::transport {
enum class BearerType : uint8_t {
    WIFI_80211S = 0,
    LORA_SX1262 = 1,
    ETHERNET    = 2,
    SNEAKERNET  = 3
};

class IMultiBearerManager {
public:
    virtual ~IMultiBearerManager() = default;
    virtual bool SendPacket(const uint8_t* data, size_t len, BearerType preferred) = 0;
    virtual void RegisterBearerFailoverCallback(std::function<void(BearerType, BearerType)> cb) = 0;
};
}
```

#### Definition of Done
- Dynamic link failover executes in $\le 500\text{ ms}$ under simulated Wi-Fi packet drop.
- LoRa driver operates within 1% regional duty cycle regulations.
- Causal ordering maintained across mixed-bearer round-trips.

---

### Story 1.5: Universal Hardware Abstraction Interface (UHAI) & SITL Simulation Bridge
- **User Story:** As a software engineer developing the Oasis client, I want a unified UHAI bridge with thermodynamic sanity validation and brownout protection, so that the identical simulation code runs seamlessly against virtual voxels in WASM and physical sensors on edge nodes.
- **Technical Context:** UHAI provides `IDigitalTwinBridge`, `ITelemetryIngress`, and `IActuationEgress`. In Oasis, SITL samples the 32-bit voxel grid; on edge nodes, it reads Modbus and GPIO. Crucially, it incorporates a thermodynamic anti-cheat engine: if sensor samples violate physical laws (e.g. temperature rise without electrical draw or heat source), the sample is flagged `QUALITY_SPOOF_SUSPECT`. It also triggers a brownout fail-safe when $V_{bus} \le 10.8\text{ V}$, flushing CRDT deltas to disk within $2.5\text{ ms}$.

#### Gherkin Scenarios
```gherkin
Scenario: Software-in-the-Loop telemetry sampling from 32-bit voxel grid in Oasis
  Given an in-game solar panel voxel at coordinate (45, 12, 10) with metadata flag META_SENSOR
  When the Oasis simulation tick executes at 10 Hz
  Then the SITL bridge calculates solar irradiance and pushes a TelemetrySample into the UHAI queue
  And the sample is marked with QUALITY_SIMULATED and ingested by Layer 3 minting.

Scenario: Thermodynamic anti-cheat anomaly detection (Scenario Rho)
  Given a remote sensor reporting an induction furnace temperature of 450°C
  When the upstream CT current clamp reports 0.0 Amperes of electrical draw
  Then the UHAI sanity validator flags the sample as QUALITY_SPOOF_SUSPECT
  And suspends Proof of Stewardship token minting for that channel.

Scenario: Early brownout interrupt and non-volatile flush
  Given a physical node operating on battery power
  When the DC bus voltage drops to 10.8V or lower
  Then a high-priority brownout interrupt triggers
  And all pending CRDT state deltas are flushed to non-volatile flash memory within 2.5 milliseconds
  And all actuators are locked in safe-park state before deep sleep.
```

#### Technical Tasks
- [ ] Implement `IDigitalTwinBridge` polymorphic interfaces and lock-free SPSC queues (`uhai/digital_twin.cpp`).
- [ ] Build SITL voxel grid sampler translating voxel states to `TelemetrySample` packets.
- [ ] Implement multi-sensor cross-validation rules for thermodynamic anti-cheat detection (`uhai/anti_cheat.cpp`).
- [ ] Implement early brownout voltage monitoring and atomic $2.5\text{ ms}$ emergency flash flush.

#### Interfaces & Data Structures
```cpp
namespace collective::uhai {
class IUhaiBridge : public IDigitalTwinBridge {
public:
    virtual bool IngestTelemetry(const TelemetrySample& sample) override = 0;
    virtual bool DispatchActuation(const ActuatorCommand& cmd) override = 0;
    virtual bool ValidateThermodynamicPlausibility(const TelemetrySample& sample) override = 0;
    virtual void TriggerEmergencyFlush() = 0;
};
}
```

#### Definition of Done
- SITL bridge runs in browser WASM with zero heap allocation per tick.
- Thermodynamic anomaly correctly identified in automated Catch2 unit tests.
- Emergency flush completes in $< 2.5\text{ ms}$ under simulated brownout triggers.

---

## 4. Epic 2: Sovereign Identity, Cryptographic Trust & Local Storage Engine (Layers 3 & 4)

### Epic Overview
- **Strategic Scope:** Implement the self-sovereign identity, decentralized cryptographic verification, and local-first data availability layers. Provide W3C DID document management (`did:key`, `did:peer`, `did:mesh`), Ed25519/BLS12-381 cryptographic suites, W3C Verifiable Credentials with BBS+ zero-knowledge selective disclosure, FROST threshold social recovery, and local-first SQLite/CRDT storage with IPFS/IPLD content addressing and Reed-Solomon erasure coding.
- **Architectural Layers:** Layer 3 (Identity & Cryptography) & Layer 4 (Storage & Data Availability).
- **Target Environments:** Oasis C++20 / WASM (OPFS / WebWorker) and Physical Edge Daemons (Linux POSIX / LMDB / cr-sqlite).

---

### Story 2.1: W3C Decentralized Identifiers & Cryptographic Primitive Suites
- **User Story:** As a sovereign citizen, I want to create and manage cryptographically self-certifying Decentralized Identifiers (`did:key`, `did:peer`, `did:mesh`) backed by Ed25519 and BLS12-381 keypairs, so that all my actions, work orders, and transactions are authenticated without relying on centralized certificate authorities or state registries.
- **Technical Context:** Implements W3C DID Core 1.0 specifications. Supports disposable `did:key` for session actors, pairwise `did:peer` for encrypted bilateral communication channels, and bioregional `did:mesh` anchored to local CRDT identity stores. Key operations utilize Ed25519 (EdDSA over Curve25519 with SHA-512), X25519 ECDH key agreement, and BLS12-381 pairing curves for threshold signature aggregation. All signature verification on the main engine loop is non-blocking, delegating to Thread 3 via SPSC ring buffers.

#### Gherkin Scenarios
```gherkin
Scenario: Ephemeral session identity creation using did:key
  Given an unauthenticated guest terminal in Oasis
  When the user initiates a session
  Then a new Ed25519 keypair is generated via the hardware TRNG or WebCrypto API
  And a deterministic "did:key" URI is derived using multicodec prefix 0xed01
  And signature generation executes in less than 50 microseconds.

Scenario: Asynchronous cryptographic verification offloaded from main thread
  Given 100 incoming signed transactions from remote mesh peers
  When the transactions enter the engine's cryptographic ingress pipeline
  Then the signatures are pushed into an SPSC ring buffer for background verification on Thread 3
  And the main simulation frame continues executing at 60 FPS without dropping frames
  And verified state transitions are committed optimistically.
```

#### Technical Tasks
- [ ] Implement RFC 8032 compliant Ed25519 signature generation and verification (`crypto/ed25519.cpp`).
- [ ] Implement BLS12-381 curve operations and signature aggregation (`crypto/bls12_381.cpp`).
- [ ] Implement W3C DID document parser and resolver for `did:key`, `did:peer`, and `did:mesh`.
- [ ] Build lock-free SPSC cryptographic verification queue between main thread and Thread 3.

#### Interfaces & Data Structures
```cpp
namespace collective::crypto {
struct DidDocument {
    char     did_uri[64];
    uint8_t  public_key[32];     // Ed25519 or BLS12-381 public key
    uint8_t  key_type;           // 0=Ed25519, 1=BLS12_381, 2=X25519
    uint32_t created_tick;
    uint32_t expires_tick;
};

class ICryptoEngine {
public:
    virtual ~ICryptoEngine() = default;
    virtual bool Sign(const uint8_t* msg, size_t len, const uint8_t* privkey, uint8_t* out_sig) = 0;
    virtual bool Verify(const uint8_t* msg, size_t len, const uint8_t* pubkey, const uint8_t* sig) = 0;
    virtual bool AggregateSignatures(const uint8_t** sigs, size_t count, uint8_t* out_agg_sig) = 0;
};
}
```

#### Definition of Done
- Passes 100% of RFC 8032 and RFC 8439 test vectors.
- Single Ed25519 verification benchmarks $\le 50\text{ }\mu\text{s}$ on ARM64 and x86_64.
- Memory leak free under ASan; zero dynamic allocation on hot verification paths.

---

### Story 2.2: W3C Verifiable Credentials & BBS+ Zero-Knowledge Selective Disclosure
- **User Story:** As an apprentice steward, I want to present verifiable competency credentials (e.g. electrical wiring certification, medical training) with BBS+ zero-knowledge proofs, so that I can claim specialized BPMN tasks without disclosing my real-world identity or unnecessary personal data.
- **Technical Context:** Implements W3C Verifiable Credentials v2.0 JSON-LD payloads signed with `Ed25519Signature2020` or BBS+ signatures over BLS12-381. BBS+ allows selective disclosure of individual credential claims: a steward can prove they possess a valid master electrician credential issued by a trusted node, without revealing the credential subject's name, issue date, or other attributes to unauthorized peers.

#### Gherkin Scenarios
```gherkin
Scenario: Issuance of Verifiable Competency Credential by Trust Ring 0
  Given Founder 01 holding Trust Ring 0 administrative signing keys
  When Apprentice Dave completes the "Off-Grid Solar Inverter Wiring" BPMN assessment
  Then Founder 01 signs a W3C Verifiable Credential using BBS+ signatures
  And the credential schema conforms to "https://schema.collective.org/v1/StewardCompetency"
  And the credential is saved to Dave's local wallet.

Scenario: Zero-Knowledge selective disclosure of high-voltage wiring competency
  Given Dave claiming an advanced electrical WorkToken requiring certification
  When the BPMN engine requests proof of competency
  Then Dave generates a BBS+ zero-knowledge selective disclosure proof revealing only "Competency: HighVoltage"
  And hides his real-world identity, registration date, and issuing mentor DID
  And the engine verifies the proof cryptographically in less than 15 milliseconds.
```

#### Technical Tasks
- [ ] Implement W3C VC v2.0 JSON-LD canonicalization and linked data proof verifier (`identity/vc_engine.cpp`).
- [ ] Implement BBS+ multi-message signature scheme over BLS12-381 pairing curves (`crypto/bbs_plus.cpp`).
- [ ] Build in-memory credential storage and selective disclosure query evaluator.
- [ ] Integrate VC validation into Layer 4 BPMN task eligibility checkers.

#### Interfaces & Data Structures
```cpp
namespace collective::identity {
struct BBSProofRequest {
    const char* credential_schema_uri;
    uint32_t    disclosed_attribute_mask; // Bitmask of attributes to reveal
};

class IVcEngine {
public:
    virtual ~IVcEngine() = default;
    virtual bool IssueCredential(const char* jsonld_payload, uint8_t* out_signature) = 0;
    virtual bool GenerateZkProof(const char* vc_jsonld, const BBSProofRequest& req, std::string& out_proof) = 0;
    virtual bool VerifyZkProof(const std::string& proof_jsonld, const BBSProofRequest& req) = 0;
};
}
```

#### Definition of Done
- Verified against W3C VC Test Suite compliance cases.
- BBS+ ZKP proof generation completes in $\le 15\text{ ms}$; verification in $\le 5\text{ ms}$.
- Zero private attributes leaked across network packets during selective disclosure runs.

---

### Story 2.3: Distributed Social Recovery Engine via FROST Threshold Signatures
- **User Story:** As an off-grid community member, I want to protect my master cryptographic identity using a 3-of-5 FROST threshold social recovery scheme across my Trust Ring, so that I can recover my account if my physical device is destroyed without ever reconstructing my private key in a single vulnerable location.
- **Technical Context:** Traditional seed phrases are brittle in emergency conditions. This story implements FROST (Flexible Round-Optimized Schnorr Threshold) over Curve25519. A user's root signing authority is divided into $n=5$ secret polynomial shares distributed across trusted guardians in Trust Ring 1. In a recovery event, any $k=3$ guardians participate in a 2-round threshold signing session to authorize a key-rotation transaction to the user's new device, without ever reconstructing the private master key on any single node. Supports FIDO2 / WebAuthn hardware tokens for local device authorization.

#### Gherkin Scenarios
```gherkin
Scenario: Initializing a 3-of-5 FROST threshold guardian quorum
  Given a citizen establishing their sovereign identity
  When they configure social recovery across 5 selected Trust Ring 1 guardians
  Then FROST Distributed Key Generation (DKG) executes over encrypted peer-to-peer channels
  And each guardian receives their private secret share
  And the master public key is derived without any single entity holding the root private key.

Scenario: Successful 2-round threshold social key rotation after hardware loss
  Given a citizen who lost their primary hardware device
  When they submit a key-rotation request to 3 of their 5 designated guardians
  Then Round 1 commitments are exchanged and Round 2 threshold signature shares are computed
  And a valid master authorization signature is constructed
  And the user's DID document is rotated to their new public key on the local mesh.
```

#### Technical Tasks
- [ ] Implement FROST DKG (Distributed Key Generation) protocol over Curve25519 (`crypto/frost_dkg.cpp`).
- [ ] Implement Round 1 and Round 2 threshold signature aggregation routines (`crypto/frost_sign.cpp`).
- [ ] Build P2P guardian coordination protocol over libp2p Gossipsub.
- [ ] Implement WebAuthn / FIDO2 hardware token adapter for local wallet unlock.

#### Interfaces & Data Structures
```cpp
namespace collective::crypto {
struct FrostRound1Commitment {
    uint8_t guardian_index;
    uint8_t hiding_nonce_commitment[32];
    uint8_t binding_nonce_commitment[32];
};

struct FrostRound2Share {
    uint8_t guardian_index;
    uint8_t response_share[32];
};

class IFrostRecoveryManager {
public:
    virtual ~IFrostRecoveryManager() = default;
    virtual bool InitiateRecovery(const char* did_uri, const uint8_t* new_pubkey) = 0;
    virtual bool AggregateRecoverySignature(const FrostRound2Share* shares, size_t count, uint8_t* out_sig) = 0;
};
}
```

#### Definition of Done
- Successfully simulates 3-of-5 recovery across 5 independent nodes with zero secret reconstruction.
- 2-round coordination completes in $\le 500\text{ ms}$ over simulated network mesh.
- Cryptographic security verified against rogue-key and man-in-the-middle attacks.

---

### Story 2.4: Local-First `cr-sqlite` Engine & OPFS WASM Storage Architecture
- **User Story:** As an Oasis engine and edge node developer, I want a unified local-first relational database with Conflict-free Replicated Relations (`cr-sqlite`), running over Origin Private File System (OPFS) in the browser and native POSIX on edge nodes, so that all local state is instantaneously queryable, persistent, and automatically synchronizes when connected to peers.
- **Technical Context:** Embeds SQLite with `cr-sqlite` extensions. In WebAssembly, it compiles with the OPFS synchronous VFS, providing direct file-handle block access with performance comparable to native NVMe SSDs ($> 50\text{ MB/s}$ throughput), eliminating IndexedDB serialization overhead. On native edge nodes, it runs standard SQLite with WAL mode. Tables are declared as CRR (Conflict-free Replicated Relations), automatically tracking row- and column-level mutations via local causal clocks and generating binary delta changesets for P2P synchronization.

#### Gherkin Scenarios
```gherkin
Scenario: Synchronous OPFS block write during browser WASM execution
  Given the Oasis engine running inside a WebAssembly canvas
  When the engine persists a chunk containing 32,768 modified voxels
  Then the cr-sqlite storage engine writes blocks directly to the OPFS VFS handle
  And total write latency completes in less than 10 milliseconds
  And data survives full browser tab refreshes without corruption.

Scenario: Bidirectional delta synchronization across peer SQLite databases
  Given Node Alpha and Node Beta with locally diverging cr-sqlite databases
  When Node Alpha establishes a P2P data channel with Node Beta
  Then both nodes exchange Merkle search tree state vectors
  And apply binary delta changesets without generating SQL conflicts or primary key collisions.
```

#### Technical Tasks
- [ ] Configure SQLite Emscripten build with OPFS synchronous VFS and WASM pthread support (`storage/opfs_vfs.cpp`).
- [ ] Integrate `cr-sqlite` extension with CRR table schema declarations (`storage/cr_sqlite.cpp`).
- [ ] Implement binary delta changeset serializer and deserializer over libp2p and WebRTC streams.
- [ ] Build automated disk quota and pruning manager for browser sandboxes ($< 30\text{ MB}$ footprint).

#### Interfaces & Data Structures
```cpp
namespace collective::storage {
struct DbChangeset {
    uint64_t min_hlc;
    uint64_t max_hlc;
    size_t   payload_bytes;
    uint8_t  binary_payload[4096];
};

class ILocalStorageEngine {
public:
    virtual ~ILocalStorageEngine() = default;
    virtual bool ExecuteQuery(const char* sql) = 0;
    virtual bool ExtractChangeset(uint64_t since_hlc, DbChangeset& out_changes) = 0;
    virtual bool ApplyChangeset(const DbChangeset& changes) = 0;
};
}
```

#### Definition of Done
- 10,000 queries execute in $\le 50\text{ ms}$ in both native desktop and browser WASM environments.
- Zero data corruption across 1,000 simulated browser tab crash/reload cycles.
- Binary changesets sync bidirectionally with bit-exact convergence across 3 peers.

---

### Story 2.5: IPLD Content-Addressing & Reed-Solomon Bioregional Erasure Coding
- **User Story:** As an archivist of sovereign knowledge, I want all blueprints, CAD models, legal trust deeds, and media assets to be content-addressed via CIDv1 and protected with $k=4, m=4$ Reed-Solomon erasure coding, so that critical community knowledge survives catastrophic loss of multiple physical nodes.
- **Technical Context:** Immutable data is addressed by Content Identifiers (CIDv1) using BLAKE3 and SHA-256 multihashes and IPLD `dag-cbor` formatting. Large files ($> 1\text{ MB}$) are split into $k=4$ data shards and $m=4$ parity shards (8 total shards) using Galois Field $GF(2^8)$ Reed-Solomon erasure coding. Any 4 of the 8 shards can completely reconstruct the original document. Shards are distributed across distinct physical nodes with periodic Proof-of-Retrievability (PoR) challenges.

#### Gherkin Scenarios
```gherkin
Scenario: Content-addressing a CAD blueprint artifact using CIDv1
  Given a 15 MB CAD blueprint of a community solar microgrid
  When the artifact is saved to the local IPFS/IPLD storage subsystem
  Then the file is hashed using BLAKE3 and assigned an immutable CIDv1 identifier
  And an IPLD dag-cbor metadata manifest is generated linking all component sub-blocks.

Scenario: Reconstruction of a legal trust deed after catastrophic loss of 4 nodes
  Given a critical PPT Trust Deed split into 4 data shards and 4 parity shards across 8 nodes
  When a fire or municipal raid permanently destroys 4 of the 8 nodes
  Then the remaining 4 nodes exchange their erasure shards
  And the Reed-Solomon decoder reconstructs the original document bit-for-bit in less than 50 milliseconds.
```

#### Technical Tasks
- [ ] Implement CIDv1 string generator and IPLD `dag-cbor` parser (`storage/ipld_cid.cpp`).
- [ ] Implement Galois Field $GF(2^8)$ Reed-Solomon $k=4, m=4$ codec (`storage/reed_solomon.cpp`).
- [ ] Build decentralized shard distribution protocol over libp2p Bitswap / Gossipsub.
- [ ] Implement randomized Merkle-path Proof-of-Retrievability challenge daemon.

#### Interfaces & Data Structures
```cpp
namespace collective::storage {
struct ErasureShard {
    uint8_t shard_index;        // 0 to 7
    uint8_t shard_type;         // 0=Data, 1=Parity
    uint32_t shard_size_bytes;
    std::array<uint8_t, 32> shard_merkle_root;
};

class IErasureCodec {
public:
    virtual ~IErasureCodec() = default;
    virtual bool Encode(const uint8_t* src, size_t len, uint8_t** out_shards, size_t* out_shard_len) = 0;
    virtual bool Decode(const uint8_t** in_shards, const uint8_t* shard_mask, size_t shard_len, uint8_t* out_data) = 0;
};
}
```

#### Definition of Done
- Reconstructs 10 MB files with exactly 4 missing shards with bit-exact hash verification.
- Decoding benchmark $\le 50\text{ ms}$ on ARM64 hardware.
- Proof-of-Retrievability verifies Merkle-tree inclusion in $< 1\text{ ms}$.

---

## 5. Epic 3: Asynchronous Consensus, CRDT Replication & Thermodynamic Valuenomics (Layers 5 & 6)

### Epic Overview
- **Strategic Scope:** Implement the state machine replication, asynchronous consensus, and thermodynamic valuenomics engines. Build the 24-byte `VoxelMutationOp` delta replication pipeline over libp2p Gossipsub, Kulkarni-Demir Hybrid Logical Clock ordering, EigenTrust Web of Trust graph centrality scoring, thermodynamic Proof of Stewardship minting ($\Delta V$), and the Ripple/Sardex mutual credit bilateral clearing engine.
- **Architectural Layers:** Layer 5 (State Replication & Policy) & Layer 6 (Semantic & Valuenomics).
- **Target Environments:** Oasis C++20 / WebGPU Client and Physical Collective Edge Daemons.

---

### Story 3.1: 24-Byte `VoxelMutationOp` CRDT Replication & HLC Engine
- **User Story:** As an Oasis engine programmer, I want a compact 24-byte CRDT mutation struct governed by Hybrid Logical Clocks, so that multi-node state replication consumes minimal mesh bandwidth and resolves concurrent edits deterministically without centralized servers.
- **Technical Context:** State replication is achieved through contiguous 24-byte `VoxelMutationOp` structs. Temporal ordering uses the Kulkarni-Demir Hybrid Logical Clock $(l_i, c_i)$, guaranteeing strict causal ordering across disconnected nodes. Concurrent conflicting edits to the same voxel coordinate are resolved deterministically using Last-Write-Wins (LWW) with author DID slot as the tie-breaker. The old voxel value is stored within the struct, functioning as an instantaneous inverse rollback log for optimistic local execution.

#### Gherkin Scenarios
```gherkin
Scenario: Deterministic resolution of concurrent voxel placement conflict
  Given Node Alpha and Node Beta simultaneously placing different materials at voxel coordinate (10, 5, 2)
  When both nodes broadcast their 24-byte VoxelMutationOp records across the mesh
  Then both nodes evaluate the Hybrid Logical Clock timestamps
  And apply the deterministic Last-Write-Wins rule with DID slot tie-breaker
  And arrive at bit-exact identical voxel states with zero divergence.

Scenario: Rollback of optimistic local edit upon receiving causally prior mutation
  Given the local engine optimistically mutating voxel index 45000 at tick 120
  When a remote mutation arrives with an earlier HLC timestamp from an authorized peer
  Then the engine applies the 24-byte inverse log (old_value) to roll back the local voxel
  And commits the remote mutation in less than 2 microseconds.
```

#### Technical Tasks
- [ ] Implement Kulkarni-Demir Hybrid Logical Clock algorithm in C++20 (`crdt/hlc.cpp`).
- [ ] Implement 24-byte `VoxelMutationOp` binary serialization and ring buffer dispatcher (`crdt/voxel_delta.cpp`).
- [ ] Build Last-Write-Wins deterministic conflict resolution engine with DID tie-breakers.
- [ ] Integrate delta exchange into libp2p Gossipsub and WebRTC data channel streams.

#### Interfaces & Data Structures
```cpp
namespace collective::crdt {
class ICrdtDeltaEngine {
public:
    virtual ~ICrdtDeltaEngine() = default;
    virtual bool EmitMutation(uint32_t voxel_idx, Voxel old_v, Voxel new_v, uint16_t author_id) = 0;
    virtual bool IngestDelta(const VoxelMutationOp& delta) = 0;
    virtual uint64_t GetCurrentHlc() = 0;
    virtual void RollbackOperation(const VoxelMutationOp& delta) = 0;
};
}
```

#### Definition of Done
- `static_assert(sizeof(VoxelMutationOp) == 24)` strictly enforced at compile time.
- 1,000 out-of-order mutations merged in $\le 2.0\text{ ms}$ with zero state divergence across 5 simulated nodes.
- Zero dynamic memory allocations during delta generation and ingestion.

---

### Story 3.2: EigenTrust Web of Trust Centrality & Sybil Botnet Pruning
- **User Story:** As a governance coordinator, I want a Web of Trust reputation engine based on EigenTrust power iteration with graph clustering penalties, so that community consensus weights reflect real-world mutual aid rather than financial wealth, while isolating Sybil attack botnets.
- **Technical Context:** Access control and voting weights are calculated from the normalized trust matrix $C_{ij} \in [0, 1]$ via power iteration: $\vec{t}^{(k+1)} = C^T \vec{t}^{(k)}$. To resist Sybil attacks, the engine calculates the local clustering coefficient of peer clusters: tightly-coupled cliques lacking diversified inbound trust edges from Trust Ring 0 are dampened by $> 90\%$. If a peer attacks the network, a `SlashingAttestation` cascades along trust edges, severing the path. A `SafeHarborSeverance` mechanism allows clean, non-reputational edge dissolution in cases of interpersonal conflict.

#### Gherkin Scenarios
```gherkin
Scenario: Power iteration convergence across 1,024-node Trust Ring graph
  Given a Web of Trust graph containing 1,024 nodes and 8,192 trust edges
  When the EigenTrust power iteration algorithm executes
  Then the normalized reputation vector converges within 15 iterations
  And total calculation time completes in less than 5 milliseconds.

Scenario: Sybil botnet detection and reputation dampening (Scenario Rho)
  Given a rogue cluster of 50 bot nodes that fully cross-sign each other's credentials
  When the bot cluster submits an intent to override communal resource policy
  Then the graph clustering coefficient algorithm flags the dense insular topology
  And dampens the bot cluster's aggregate trust score by over 90%
  And the malicious intent is rejected by Layer 5 policy.
```

#### Technical Tasks
- [ ] Implement sparse matrix power iteration for EigenTrust calculation (`policy/eigentrust.cpp`).
- [ ] Implement graph clustering coefficient evaluation for Sybil cluster identification (`policy/sybil_filter.cpp`).
- [ ] Build cascading `SlashingAttestation` propagator and `SafeHarborSeverance` edge unloader.
- [ ] Integrate trust weights into Layer 4 BPMN task claiming authorization gates.

#### Interfaces & Data Structures
```cpp
namespace collective::policy {
struct TrustEdge {
    uint16_t source_did_slot;
    uint16_t target_did_slot;
    uint8_t  weight;            // 0 to 255
    uint32_t last_validated_tick;
};

class ITrustGraphEngine {
public:
    virtual ~ITrustGraphEngine() = default;
    virtual void AddTrustEdge(const TrustEdge& edge) = 0;
    virtual void ComputeGlobalTrustVector(float* out_trust_scores, size_t node_count) = 0;
    virtual void SlashPeer(uint16_t target_slot, uint8_t severity) = 0;
};
}
```

#### Definition of Done
- Calculates reputation across 1,024 nodes in $\le 5.0\text{ ms}$.
- Sybil cluster reputation dampened by $\ge 90\%$ compared to honest nodes.
- Unit tests verify convergence across disconnected graph components.

---

### Story 3.3: Thermodynamic Proof of Stewardship Minting Engine
- **User Story:** As an ecological steward, I want Value Tokens to be minted strictly through physical negentropy added to the local biome, so that the community currency represents genuine thermodynamic wealth without fiat inflation or speculative crypto mining.
- **Technical Context:** Tokens (`mesh:ValueToken`) are minted strictly via the thermodynamic exergy integral:
$$\Delta V = \int_{t_0}^{t_1} \left( \Phi_{in}(t) - \Phi_{out}(t) \right) \cdot \lambda_{ERC}(t) \, dt$$
Where $\Phi_{in}(t)$ is verified exergy entering the node (solar kWh generated, filtered rainwater liters, biochar kilograms sequestered), $\Phi_{out}(t)$ is energy consumed or degraded into waste heat, and $\lambda_{ERC}(t)$ is the **Ecological Replacement Cost** asymptote:
$$\lambda_{ERC} = \lambda_{base} \cdot \left( \frac{1}{1 - \left( \frac{R_{current}}{R_{capacity}} \right)^\gamma} \right)$$
If resource extraction approaches carrying capacity ($R_{current} \to R_{capacity}$), $\lambda_{ERC} \to \infty$, making extraction economically impossible and triggering automated hardware cutoffs.

#### Gherkin Scenarios
```gherkin
Scenario: Minting Value Tokens from verified off-grid solar generation
  Given an off-grid solar array generating 5.0 kWh of verified exergy over 3,600 ticks
  When the Layer 3 minting cycle executes at the conclusion of the billing window
  Then exactly Delta V Value Tokens are minted into the steward's CRDT wallet
  And the transaction is cryptographically signed by the local node's UHAI telemetry key.

Scenario: Ecological Replacement Cost asymptote throttling resource extraction
  Given a local aquifer with capacity 100,000 liters currently depleted to 95,000 liters
  When a water extraction intent is submitted to Layer 4 orchestration
  Then lambda_ERC recalculates along the asymptotic curve, multiplying extraction cost by 20x
  And automated irrigation valves throttle flow rate to prevent ecological collapse.
```

#### Technical Tasks
- [ ] Implement fixed-point numerical integration engine for $\Delta V$ minting calculation (`valuenomics/minting.cpp`).
- [ ] Implement dynamic $\lambda_{ERC}$ asymptotic curve calculator with configurable exponent $\gamma$ (`valuenomics/erc_curve.cpp`).
- [ ] Connect UHAI `TelemetrySample` streams directly to the exergy input accumulator.
- [ ] Implement automated actuator throttling hooks when $\lambda_{ERC}$ exceeds safety thresholds.

#### Interfaces & Data Structures
```cpp
namespace collective::valuenomics {
struct ExergyTelemetry {
    float solar_joules_harvested;
    float water_liters_purified;
    float biochar_kg_sequestered;
    float waste_heat_joules_dissipated;
};

class IValuenomicsEngine {
public:
    virtual ~IValuenomicsEngine() = default;
    virtual uint32_t ComputeMintableTokens(const ExergyTelemetry& telem, float carrying_capacity_ratio) = 0;
    virtual float CalculateErcMultiplier(float r_current, float r_capacity, float gamma) = 0;
};
}
```

#### Definition of Done
- Numerical integration maintains $\pm 0.001\%$ precision across 3,600 tick test vectors.
- Asymptotic pricing prevents 100% extraction in automated Catch2 unit tests.
- Value Token transactions balance conservation laws across all peer nodes.

---

### Story 3.4: Bioregional Mutual Credit Clearing & Debt Loop Elimination
- **User Story:** As a community producer, I want to trade goods and services with neighboring nodes using a zero-sum mutual credit clearing engine, so that we can conduct commerce without fiat cash while automatically eliminating circular debts.
- **Technical Context:** Implements a bilateral mutual credit ledger inspired by the Sardex and Ripple architectures. Citizens operate with credit limits established by their Web of Trust score. The system is strictly zero-sum across the Trust Ring ($\sum B_i = 0$). To prevent gross debt bloat, the engine runs an asynchronous cycle-detection algorithm (Johnson's algorithm) to detect and cancel circular credit loops ($A \to B \to C \to A$), reducing total outstanding credit obligations without requiring any currency settlement.

#### Gherkin Scenarios
```gherkin
Scenario: Zero-sum bilateral mutual credit transaction between two stewards
  Given Steward Alice with balance 0.0 and Steward Bob with balance 0.0
  When Alice purchases 20 kg of fresh tomatoes from Bob for 50 Mutual Credit units
  Then Alice's balance updates to -50.0 and Bob's balance updates to +50.0
  And the sum of all balances across the Trust Ring remains exactly 0.0.

Scenario: Automated multilateral debt loop elimination via Johnson's algorithm
  Given Alice owes Bob 30 credits, Bob owes Charlie 30 credits, and Charlie owes Alice 30 credits
  When the mutual credit clearing daemon executes its periodic optimization pass
  Then the 3-node cycle is detected and cleared automatically
  And Alice, Bob, and Charlie's balances are all reset to 0.0 with signed settlement receipts.
```

#### Technical Tasks
- [ ] Implement zero-sum bipartite mutual credit ledger (`valuenomics/mutual_credit.cpp`).
- [ ] Implement Johnson's elementary cycle-finding algorithm for debt loop cancellation (`valuenomics/cycle_clearing.cpp`).
- [ ] Enforce Trust Ring credit limit boundaries based on EigenTrust reputation scores.
- [ ] Generate cryptographically signed settlement receipts for cleared credit cycles.

#### Interfaces & Data Structures
```cpp
namespace collective::valuenomics {
struct CreditTransaction {
    uint16_t debtor_did_slot;
    uint16_t creditor_did_slot;
    uint32_t amount_units;
    uint64_t timestamp_hlc;
    uint8_t  debtor_sig[64];
};

class IMutualCreditEngine {
public:
    virtual ~IMutualCreditEngine() = default;
    virtual bool PostTransaction(const CreditTransaction& tx) = 0;
    virtual size_t ClearDebtCycles() = 0;
    virtual int64_t GetBalance(uint16_t did_slot) const = 0;
};
}
```

#### Definition of Done
- Resolves 10-node circular debt loops in $\le 1.0\text{ ms}$.
- Conservation check $\sum B_i = 0$ holds across 100,000 random transaction test vectors.
- Credit limit bounds strictly enforced; transactions exceeding limits rejected.

---

### Story 3.5: Combinatorial Resource Barter & Autonomous Double Auctions
- **User Story:** As an autonomous workshop coordinator, I want an automated double-auction matching engine for physical tools and raw materials, so that idle tools (e.g. 3D printers, tractors, CNC mills) are automatically scheduled to fulfill community work orders with minimal transport distance.
- **Technical Context:** Citizens broadcast signed JSON-LD Knowledge Artifacts declaring resource offerings (e.g. 5 hours of CNC spindle time) and material bounties (e.g. 10 kg of recycled PET filament needed). The engine runs a local combinatorial double-auction solver that matches buyers and sellers, optimizing for Pareto-efficient resource utilization and shortest physical transport distance across the mesh.

#### Gherkin Scenarios
```gherkin
Scenario: Combinatorial matching of 3D printer capacity and filament bounty
  Given a workshop node advertising 12 hours of idle 3D printing capacity
  And a nearby citizen advertising a bounty for 4 structural brackets with PET filament provided
  When the double-auction matching engine runs its periodic cycle
  Then the offer and bounty are paired Pareto-optimally
  And an executable Layer 4 BPMN WorkToken is generated and dispatched to the workshop.

Scenario: Spatial-aware barter matching minimizing transport energy
  Given two identical offers of 50 kg firewood located 0.5 km and 12 km away respectively
  When an off-grid kitchen requests firewood
  Then the auction engine weights the bids by physical mesh distance
  And awards the contract to the 0.5 km provider to minimize transport exergy dissipation.
```

#### Technical Tasks
- [ ] Implement JSON-LD Knowledge Artifact schema parser for bounties and offers (`valuenomics/knowledge_artifact.cpp`).
- [ ] Build combinatorial double-auction solver with spatial distance weighting (`valuenomics/double_auction.cpp`).
- [ ] Implement automatic compilation of matched auction pairs into Layer 4 BPMN work orders.
- [ ] Build auction status dashboard for the Oasis Cadastral Lens.

#### Interfaces & Data Structures
```cpp
namespace collective::valuenomics {
struct BarterOffer {
    uint32_t resource_type_id;
    float    quantity;
    float    min_unit_price;
    uint16_t location_x, location_y;
    uint16_t provider_did_slot;
};

class IDoubleAuctionEngine {
public:
    virtual ~IDoubleAuctionEngine() = default;
    virtual void SubmitOffer(const BarterOffer& offer) = 0;
    virtual size_t SolveMarketClearing() = 0;
};
}
```

#### Definition of Done
- Market clearing solves 500 simultaneous bids/offers in $\le 10\text{ ms}$.
- Spatial distance weighting correctly minimizes transport energy.
- All matches compile directly into valid BPMN 2.0 work queues.

---

## 6. Epic 4: Layer 7 Adversarial Municipal Constraints & Fiat Emulation Engine (Dedicated Comprehensive Epic)

### Epic Overview
- **Strategic Scope:** Deliver the comprehensive, dedicated Layer 7 Adversarial Municipal Constraints and Fiat Emulation Engine. This engine provides the indispensable systemic tension for Oasis, modeling the aggressive regulatory, financial, and legal warfare exerted by legacy state apparatuses and utility monopolies against emerging sovereign communities. It models three predatory US utility monopolies (*AmeriGrid*, *MetroPower*, *Keystone Gas & Electric*), four hostile municipal enforcement regimes (NFPA 855 battery limits, UPC greywater penalties, NEC 690/705 solar red-tags, nuisance abatement swatting), a 5-state municipal code enforcement FSM, predatory fiat banking APIs (*Stripe*, *Plaid*, ACH) with rolling reserves and KYC freezes, configurable Adversary Levels (0 to 3), and symmetric tactical countermeasures leading to Scenario Omega (Terminal Decoupling).
- **Architectural Layer:** Layer 7 (Legacy Proxy, Legal Shield & Adversarial Membrane).
- **Target Environments:** Oasis C++20 / WASM DES Engine (in-engine client) and Collective Ablative Shield Daemons (real-world POSIX sidecar).

```
┌────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              LAYER 7 ADVERSARIAL EMULATION ENGINE ARCHITECTURE                         │
├────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                        │
│   ┌────────────────────────┐      ┌─────────────────────────┐      ┌───────────────────────────────┐   │
│   │  Pluggable US Utility   │      │ Municipal Code Enforce  │      │ Mock Banking / Fiat Gateways  │   │
│   │  Company Models        │      │ State Machine (FSM)     │      │ - Mock Stripe API             │   │
│   │  - AmeriGrid Electric  │      │ - Inspector Patrols     │      │ - Mock Plaid API              │   │
│   │  - MetroPower Utility  │      │ - Notice of Violations  │      │ - Municipal e-Permit Portal   │   │
│   │  - Keystone Gas & Elec │      │ - Stop-Work Injunctions │      │ - Real-Time Webhook Engine    │   │
│   └───────────┬────────────┘      └────────────┬────────────┘      └───────────────┬───────────────┘   │
│               │                                │                                   │                   │
│               └─────────────────────────┐      │      ┌────────────────────────────┘                   │
│                                         ▼      ▼      ▼                                                │
│                          ┌──────────────────────────────────────────────┐                              │
│                          │    Discrete Event Simulation (DES) Bus       │                              │
│                          │    - Priority Event Queue (Tick-Aligned)     │                              │
│                          │    - Adversary Level Configurator (0 to 3)   │                              │
│                          │    - Deterministic PCG-XSH-RR Seed Engine    │                              │
│                          └──────────────────────┬───────────────────────┘                              │
│                                                 │                                                      │
│                                                 ▼                                                      │
│                          ┌──────────────────────────────────────────────┐                              │
│                          │   Ablative Translation Firewall & Escrow     │                              │
│                          │   - Converts Exergy Drains -> Fiat Invoices  │                              │
│                          │   - Auto-generates SPC Board Resolutions     │                              │
│                          │   - Terminal Decoupling Kill-Switch          │                              │
│                          └──────────────────────┬───────────────────────┘                              │
│                                                 │                                                      │
└─────────────────────────────────────────────────┼──────────────────────────────────────────────────────┘
                                                  │ (Non-leaking state transitions)
                                                  ▼
                          ┌──────────────────────────────────────────────┐
                          │    Layer 5 / Layer 4 Orchestration Bus       │
                          │    (Boundary Voxel Bitmask Updates)          │
                          └──────────────────────────────────────────────┘
```

---

### 6.1. The Configurable Adversary Level Matrix (Threat Levels 0–3)
To balance accessibility and rigorous survival gameplay, the Layer 7 emulation engine is governed by a global runtime parameter `adversary_level` ($0 \le L \le 3$):

| Level | Title | Utility Monopoly Behavior | Municipal & Zoning Behavior | Fiat Banking Behavior | In-Game Purpose & Calibration |
| :---: | :--- | :--- | :--- | :--- | :--- |
| **0** | **Sandbox / Permaculture Demo** | Fixed $\$0.12/\text{kWh}$, 1:1 net-metering, zero standby fees. | Inspections disabled; building permits auto-approved instantly at $\$0$ fee. | Instant clearing, 0% fees, no holds, no KYC freezes. | Engine benchmarking, educational permaculture design, creative sandbox. |
| **1** | **Mild Bureaucracy** | Predictable rates (3% inflation), standard TOU peak $\$0.28/\text{kWh}$. | Routine inspections, 30-day permit delays, $\$250$ fees, clear checklists. | Stripe standard $2.9\% + \$0.30$, 48h KYC review, 5% rolling reserves on disputes. | Balanced management experience (*Cities: Skylines* style). |
| **2** | **Hostile Municipal Siege** *(Default Canon)* | Net-metering banned, TOU peak $\$0.65/\text{kWh}$, 1 Hz AMI smart-meter sniffing, $\$65$ fixed fee. | NFPA 855 battery limits enforced, greywater diversion fines, anonymous neighbor nuisance swatting. | 25% 180-day rolling reserve holds, automated KYB trust freezes, $\$35$ overdraft fees. | The canonical intended Oasis experience (*Dwarf Fortress* tension). Drives Sovereign Stack adoption. |
| **3** | **Maximum State Capture / Omega** | Rule 21 lockouts, $\$5,000$ standby fee, remote smart-meter cutoffs, grid defection bans. | Emergency Red-Tag Condemnation Notices, sheriff inspection raids, bioswale nuisance fines. | 100% operating account freeze, deplatforming from commercial processors (MCC 6051). | Hardcore survival crisis: requires complete physical off-grid severance and pure mesh autonomy. |

---

### Story 4.1: Discrete Event Simulation (DES) Bus & Configurable Adversary Controller
- **User Story:** As an Oasis engine designer, I want an asynchronous Discrete Event Simulation (DES) priority queue running at an amortized 1 Hz tick within the 0.15 ms frame budget, so that external municipal, utility, and banking events execute deterministically and scale according to Adversary Levels 0 through 3.
- **Technical Context:** The `AdversaryEngine` manages a priority queue of scheduled events sorted by execution tick. To guarantee 100% deterministic replay for headless CI testing, stochastic events (neighbor complaints, random audits) use the PCG-XSH-RR 64/32 pseudorandom algorithm initialized with an explicit seed. Layer 7 consumes $\le 0.15\text{ ms}$ of frame time and zero heap memory in its update loop.

#### Gherkin Scenarios
```gherkin
Scenario: Deterministic event dispatch under Adversary Level 2
  Given an engine initialized with seed 42 and adversary_level set to 2
  When 10,000 simulation ticks execute
  Then exactly 3 utility billing events, 1 code complaint, and 1 Stripe reserve hold fire at predictable ticks
  And the state hash matches bit-for-bit across multiple execution runs.

Scenario: Dynamic runtime scaling from Level 0 to Level 3
  Given a simulation running in Level 0 Sandbox mode with zero municipal pressure
  When the player or scenario harness adjusts adversary_level to 3 (Maximum State Capture)
  Then the DES event bus immediately schedules an emergency AMI inspection visit
  And escalates utility peak pricing tariffs within the next billing cycle.
```

#### Technical Tasks
- [ ] Implement priority queue DES event bus with tick-aligned dispatch (`layer7/des_bus.cpp`).
- [ ] Implement deterministic PCG-XSH-RR pseudorandom number generator (`layer7/prng.cpp`).
- [ ] Build global `AdversaryEngine` controller managing runtime difficulty levels 0 to 3.
- [ ] Enforce strict 0.15 ms frame budget ceiling via RAII timer profilers.

#### Interfaces & Data Structures
```cpp
namespace oasis::layer7 {
enum class AdversaryEventCode : uint16_t {
    UTILITY_BILL_GENERATED      = 0x0100,
    UTILITY_PAYMENT_DEFAULTED   = 0x0101,
    UTILITY_PHYSICAL_DISCONNECT = 0x0102,
    CODE_VIOLATION_COMPLAINT    = 0x0200,
    INSPECTOR_SITE_VISIT        = 0x0201,
    NOTICE_OF_VIOLATION_ISSUED  = 0x0202,
    STOP_WORK_ORDER_POSTED      = 0x0203,
    COURT_INJUNCTION_HEARING    = 0x0204,
    MUNICIPAL_ABATEMENT_ORDER   = 0x0205,
    FIAT_RESERVE_HOLD_TRIGGERED = 0x0300,
    FIAT_ACCOUNT_FROZEN_KYC     = 0x0301
};

struct alignas(8) AdversaryEvent {
    uint64_t tick_timestamp;
    AdversaryEventCode event_code;
    uint16_t target_chunk_id;
    uint32_t monetary_cents;
    char     entity_descriptor[32];
};

class IAdversaryEngine {
public:
    virtual ~IAdversaryEngine() = default;
    virtual void SetAdversaryLevel(uint8_t level) = 0;
    virtual void ScheduleEvent(const AdversaryEvent& event) = 0;
    virtual void Tick(uint64_t current_tick) = 0;
};
}
```

#### Definition of Done
- DES bus runs in $\le 0.10\text{ ms}$ per tick with zero dynamic memory allocation.
- 10,000-tick runs with identical seeds produce identical bit-exact state hashes.
- All 11 `AdversaryEventCode` types handled deterministically.

---

### Story 4.2: Fictional Utility Monopoly Suite (AmeriGrid, MetroPower, Keystone)
- **User Story:** As an Oasis player, I want realistic emulation of three predatory US utility monopolies with time-of-use tariffs, smart-meter surveillance, and standby fees, so that relying on legacy utility infrastructure creates visceral financial pressure driving me to build off-grid solar and microgrids.
- **Technical Context:** Implements rate algorithms and adversarial mechanics for three utility monopolies:
1. **AmeriGrid (Electric Utility):** $\$45.00/\text{month}$ standing fee; off-peak $\$0.14/\text{kWh}$, peak (4–9 PM) $\$0.68/\text{kWh}$; $\$0.05/\text{kWh}$ solar backfeed interconnection penalty; $\$120/\text{month}$ "Standby Defection Fee" if breaker is pulled without a $\$1,500$ permit; automated physical disconnect if escrow hits zero.
2. **MetroPower (Municipal Water & Power):** 1 Hz AMI smart-meter telemetry sniffing running load disaggregation (detects unpermitted battery cycling and inter-lot power sharing); $\$65/\text{month}$ fixed stranded-asset charge; cross-property conduit injunctions.
3. **Keystone Gas & Electric:** Mandatory gas connection building code clause; $\$450$ meter pull fee; $\$35/\text{month}$ line maintenance standby fee; pipeline proximity easement injunctions.

#### Gherkin Scenarios
```gherkin
Scenario: AmeriGrid peak TOU pricing surge during evening hours
  Given Lot 402 connected to the AmeriGrid 240V drop line consuming 3.5 kW
  When the in-game clock reaches 4:00 PM (peak tariff window)
  Then the electricity billing rate quadruples from $0.14/kWh to $0.68/kWh
  And the fiat escrow drain rate increases proportionally
  And the Cadastral view tints the power conduit pulsing neon magenta.

Scenario: MetroPower AMI surveillance detecting unpermitted battery cycling
  Given a player running an unshielded hybrid inverter cycling 15 kWh of batteries daily
  When MetroPower's 1 Hz AMI smart-meter analytics detect harmonic power-factor signatures
  Then an unpermitted generation alert is flagged
  And an inspector site visit is automatically scheduled within 7 in-game days.

Scenario: AmeriGrid automated remote utility disconnect upon escrow depletion
  Given a player whose fiat escrow balance reaches exactly $0.00
  When the 15-day grace period expires
  Then AmeriGrid issues an automated remote disconnect command
  And the boundary voxel at (95, 32, 48) clears its META_LEGACY_TETHERED bit
  And all camp appliances lose grid power instantly.
```

#### Technical Tasks
- [ ] Implement `AmeriGridSimulator` with 24-hour TOU curves and standby defection fees (`layer7/amerigrid.cpp`).
- [ ] Implement `MetroPowerSimulator` with 1 Hz AMI load disaggregation and conduit injunction logic (`layer7/metropower.cpp`).
- [ ] Implement `KeystoneGasSimulator` with mandatory connection codes and meter pull fees (`layer7/keystone.cpp`).
- [ ] Connect utility bill totals to Layer 7 SPC fiat escrow account drains.

#### Interfaces & Data Structures
```cpp
namespace oasis::layer7 {
struct UtilityTariffProfile {
    uint32_t monthly_standing_cents;
    uint32_t off_peak_mwh_cents;
    uint32_t on_peak_mwh_cents;
    uint32_t solar_backfeed_penalty_mwh_cents;
    uint32_t defection_standby_cents;
};

class IUtilityMonopoly {
public:
    virtual ~IUtilityMonopoly() = default;
    virtual uint32_t ComputeMonthlyBill(float kwh_consumed, float kwh_exported, bool is_connected) = 0;
    virtual bool EvaluateAmiHarmonics(float current_draw_amps, float power_factor) = 0;
};
}
```

#### Definition of Done
- Billing calculations accurate to the integer cent across 30-day simulated cycles.
- AMI harmonics algorithm flags unshielded battery cycling with $> 95\%$ accuracy.
- Automated disconnect correctly clears voxel `META_LEGACY_TETHERED` flag.

---

### Story 4.3: Hostile Municipal Codes & Zoning Enforcement FSM
- **User Story:** As an Oasis player, I want an authentic municipal code enforcement engine simulating building inspectors, fire marshals, and health department citations, so that my off-grid construction must navigate real-world regulatory hurdles like NFPA 855 battery limits and greywater plumbing codes.
- **Technical Context:** Implements four hostile municipal enforcement regimes governed by a 5-State Finite State Machine:
1. **NFPA 855 Battery Storage Limits:** Prohibits $> 20\text{ kWh}$ of indoor Lithium batteries without commercial sprinklers, blast venting, and a $\$2,500$ engineering review; outdoor units must maintain 3-foot property line setbacks.
2. **Uniform Plumbing Code (UPC Chapter 15):** Fines unpermitted rainwater plumbing cross-connections without certified backflow preventers; fines standing greywater on mulch basins ($> 24\text{ hours}$); assesses impervious surface runoff taxes.
3. **National Electrical Code (NEC 690/705):** Mandates PE structural stamps for roof solar; requires exterior rapid shutdown switches; enforces IEEE 1547 anti-islanding.
4. **Nuisance Abatement Swatting:** Hostile neighbors file complaints over 3D printer noise, duck ponds, biochar kiln smoke, or camper van dwelling.

```
┌────────────────────────────────────────────────────────────────────────────────────────┐
│                   MUNICIPAL CODE ENFORCEMENT STATE MACHINE (FSM)                       │
└────────────────────────────────────────────────────────────────────────────────────────┘

 [STATE 0: UNNOTICED]
         │
         │ (Trigger: Notice Level > 100 OR Visible Smoke/Noise OR Neighbor Complaint)
         ▼
 [STATE 1: ANONYMOUS CODE COMPLAINT FILED]
         │
         │ (Timer: 3 to 7 Days elapsed)
         ▼
 [STATE 2: INSPECTOR SITE VISIT SCHEDULED]
         │
         ├─────────────────────────────────────────┐
         │ (Founder presents approved L7 permit    │ (No permit OR Steward denies access)
         ▼  or disguises feature)                  ▼
 [STATE 0: COMPLIANT]                      [STATE 3: NOTICE OF VIOLATION (NOV) POSTED]
                                                   │
                                                   │ (Timer: 14 Days to Cure or Escrow Payment)
                                                   ▼
                                           [STATE 4: STOP-WORK ORDER & DAILY FINES]
                                                   │
                                                   │ (Timer: 30 Days default)
                                                   ▼
                                           [STATE 5: COURT INJUNCTION & SHERIFF ABATEMENT]
```

#### Gherkin Scenarios
```gherkin
Scenario: Fire Marshal red-tagging indoor battery storage exceeding NFPA 855 limits
  Given a player installing 28 kWh of LiFePO4 batteries inside the Lot 402 shed
  When the Municipal Fire Marshal conducts an unannounced inspection
  Then a Notice of Violation (State 3) is issued under NFPA 855 indoor storage limits
  And a $250/day fine begins draining the SPC escrow
  And a neon orange citation banner renders over the shed in Cadastral view.

Scenario: Stop-Work Order blocking Layer 4 BPMN construction tasks
  Given a Notice of Violation that has remained unaddressed for 14 in-game days
  When the FSM transitions to State 4 (Stop-Work Order Posted)
  Then Layer 5 policy emits a coordination lock on the targeted voxel coordinates
  And citizens are mathematically blocked from claiming BPMN WorkTokens for that structure.

Scenario: County Sheriff abatement raid demolishing unpermitted structure
  Given an active Stop-Work Order that has defaulted for 30 consecutive days
  When the municipal court issues a Warrant of Abatement (State 5)
  Then municipal contractor NPCs enter Lot 402 with heavy equipment
  And demolish the target voxels, replacing them with rubble
  And assess a $3,500 demolition lien against the parcel escrow.
```

#### Technical Tasks
- [ ] Implement the 5-State Municipal Code Enforcement FSM (`layer7/code_fsm.cpp`).
- [ ] Build player "Notice Level" accumulator tracking smoke, noise, solar glare, and unpermitted structures.
- [ ] Implement NFPA 855, UPC Chapter 15, and NEC 690/705 violation rules (`layer7/municipal_rules.cpp`).
- [ ] Connect Stop-Work orders to Layer 4 BPMN task eligibility locks.

#### Interfaces & Data Structures
```cpp
namespace oasis::layer7 {
enum class FsmViolationState : uint8_t {
    UNNOTICED           = 0,
    COMPLAINT_FILED     = 1,
    INSPECTION_SCHEDULED= 2,
    NOV_POSTED          = 3,
    STOP_WORK_ORDER     = 4,
    ABATEMENT_EXECUTED  = 5
};

struct MunicipalCitation {
    uint32_t citation_id;
    FsmViolationState current_state;
    uint32_t daily_fine_cents;
    uint64_t cure_deadline_tick;
    uint16_t target_voxel_chunk;
    char     code_reference[32]; // "NFPA 855 Table 12.2"
};

class IMunicipalCodeEnforcer {
public:
    virtual ~IMunicipalCodeEnforcer() = default;
    virtual void AccumulateNotice(float visibility_delta) = 0;
    virtual FsmViolationState EvaluateStructure(uint16_t chunk_id) = 0;
    virtual bool CureViolation(uint32_t citation_id) = 0;
};
}
```

#### Definition of Done
- 5-state transitions verified with deterministic timers in automated Catch2 tests.
- Stop-Work orders mathematically halt BPMN task assignment.
- Notice Level correctly decays when structures are shielded or disguised.

---

### Story 4.4: Adversarial Fiat Banking & Payment Gateway Mock APIs
- **User Story:** As an Oasis player selling goods to the legacy economy, I want an authentic emulation of predatory fiat payment processors (Stripe, Plaid, ACH), so that algorithmic reserve holds, KYC freezes, and fee clawbacks teach me why centralized banking cannot be trusted for long-term community survival.
- **Technical Context:** Implements an embedded HTTP micro-service mocking Stripe, Plaid, and ACH payment rails. When the community sells "Trojan" goods (3D printed parts, honey, produce) to legacy customers, the gateway deducts $3.5\% + \$0.30$ processing friction. If monthly transaction volume grows by $> 40\%$, it triggers a **25% rolling reserve hold** locked for 180 days. When the Perpetual Purpose Trust (PPT) is submitted for business verification, the compliance algorithm flags the lack of human beneficial owners and executes a **100% account freeze** for 14–30 business days, bouncing automated utility payments and levying $\$35$ overdraft fees.

#### Gherkin Scenarios
```gherkin
Scenario: Stripe algorithmic 25% rolling reserve hold on rapid volume growth
  Given the Genesis Node earning $1,200/month selling 3D printed brackets via Stripe
  When gross revenue grows by more than 40% in a 30-day window
  Then the mock Stripe gateway flags the account as "Elevated Chargeback Risk"
  And locks 25% of gross revenue in a 180-day rolling escrow reserve
  And reduces operating cash flow available for property tax and utility bills.

Scenario: Complete fiat account freeze upon automated PPT trust beneficial ownership review
  Given the node submitting its Perpetual Purpose Trust charter to First National Bank for KYB
  When the automated compliance bot detects 0% human equity ownership
  Then the bank initiates a 100% account freeze pending manual legal compliance
  And automated utility bill debits bounce with HTTP 400 INSUFFICIENT_FUNDS
  And the bank levies a $35 overdraft fee and $15 ACH return penalty.
```

#### Technical Tasks
- [ ] Build embedded C++ HTTP micro-service mocking Stripe REST endpoints (`layer7/mock_stripe.cpp`).
- [ ] Build mock Plaid balance and ACH transfer simulation (`layer7/mock_plaid.cpp`).
- [ ] Implement algorithmic rolling reserve calculator and KYC freeze event generator.
- [ ] Implement ACH fee penalty accumulator and bounce notification webhooks.

#### Interfaces & Data Structures
```cpp
namespace oasis::layer7 {
struct FiatAccountBalance {
    int64_t available_cents;
    int64_t locked_reserve_cents;
    bool    is_frozen_kyc;
    uint32_t pending_ach_returns_count;
};

class IMockFiatGateway {
public:
    virtual ~IMockFiatGateway() = default;
    virtual bool ProcessCardCharge(uint32_t amount_cents, const char* memo) = 0;
    virtual bool ExecuteAchTransfer(int32_t amount_cents, const char* routing) = 0;
    virtual FiatAccountBalance GetAccountStatus() const = 0;
};
}
```

#### Definition of Done
- Endpoints respond in $\le 2\text{ ms}$ over loopback HTTP sockets.
- Rolling reserve holds trigger accurately when volume thresholds are crossed.
- KYC freezes successfully simulate downstream utility payment defaults.

---

### Story 4.5: The Tactical Countermeasure Suite & Ablative Shield
- **User Story:** As an Oasis player under municipal and financial siege, I want to deploy symmetric legal, cryptographic, and technical countermeasures, so that I can shield my emerging community from hostile state actions while preparing for complete off-grid autonomy.
- **Technical Context:** The player is equipped with symmetric countermeasures:
1. **Social Purpose Corporation (SPC) Administrative Appeals:** Structured under Washington State RCW 23B.25, the SPC files formal legal appeals that stay code enforcement fines for 30 in-game days.
2. **Perpetual Purpose Trust (PPT) Sovereign Land Easements:** Submits agricultural and educational trust deed filings that exempt greenhouses and cisterns from municipal setbacks.
3. **Sub-Panel Inverter Load Cloaking:** Installs inductive chokes and peak-shaving batteries that smooth power-draw harmonics, reducing MetroPower AMI detection probability by $> 85\%$.
4. **Modular NFPA 855 Outdoor Blast Enclosures:** Constructs external, 3-foot setback battery sheds equipped with automated Stat-X potassium aerosol fire suppression, resolving fire marshal red-tags.
5. **Defensive Patent License (DPL):** Wraps open-source fabrication blueprints in a DPL, preventing corporate patent litigation.

#### Gherkin Scenarios
```gherkin
Scenario: SPC administrative appeal suspending municipal code enforcement fines
  Given an active Stop-Work Order and daily fines on an unpermitted community kitchen
  When Founder 01 files an SPC Administrative Hearing Appeal through the Cadastral Slate
  Then all municipal daily fines are suspended for 30 in-game days
  And the player gains a window to bring the kitchen into compliance or enclose it.

Scenario: Sub-panel inverter load-cloaking masking battery cycling from AMI surveillance
  Given a node vulnerable to MetroPower 1 Hz smart-meter harmonics detection
  When the player installs an inductive choke and active power-factor correction filter
  Then power draw harmonics are smoothed to mimic standard resistive heating elements
  And the probability of AMI surveillance detection drops by over 85%.

Scenario: Modular outdoor blast shed clearing NFPA 855 battery citations
  Given an active Notice of Violation for indoor lithium-ion battery storage
  When Founder 01 relocates the batteries into a modular outdoor blast enclosure with Stat-X fire suppression
  Then the Fire Marshal inspection clears the violation
  And the citation is dismissed without further fines.
```

#### Technical Tasks
- [ ] Implement SPC administrative appeal workflow and 30-day stay of enforcement (`layer7/spc_shield.cpp`).
- [ ] Implement PPT land easement filing generator and setback exemption logic (`layer7/ppt_easement.cpp`).
- [ ] Implement sub-panel inverter load smoothing reducing AMI detection probability (`layer7/inverter_cloak.cpp`).
- [ ] Implement modular NFPA 855 blast enclosure construction blueprint and fire suppression clearance.

#### Interfaces & Data Structures
```cpp
namespace oasis::layer7 {
struct LegalDefenseFiling {
    uint32_t filing_id;
    uint8_t  filing_type;        // 0=SPC Appeal, 1=PPT Easement, 2=DPL License
    uint32_t fee_cents;
    uint32_t enforcement_stay_ticks;
};

class IAblativeShield {
public:
    virtual ~IAblativeShield() = default;
    virtual bool SubmitLegalFiling(const LegalDefenseFiling& filing) = 0;
    virtual void DeployInverterCloaking(bool enabled) = 0;
    virtual bool VerifyBlastEnclosureCompliance(uint16_t chunk_id) = 0;
};
}
```

#### Definition of Done
- Legal filings stay fines for exactly the defined tick duration.
- Load cloaking reduces AMI inspection trigger rate by $\ge 85\%$ in monte-carlo test runs.
- Blast enclosures resolve NFPA 855 violations 100% of the time.

---

### Story 4.6: Scenario Omega (Terminal Decoupling) Lifecycle Engine
- **User Story:** As an Oasis player who has achieved 100% off-grid thermodynamic self-sufficiency, I want to execute the Terminal Decoupling sequence, permanently severing legacy grid lines, zeroizing fiat bank accounts, and unmounting Layer 7 from engine memory, so that our community survives in pure sovereignty.
- **Technical Context:** Implements the `ITerminalDecoupling` lifecycle engine. When the community reaches the **Sovereignty Threshold** (100% energy, water, calorie, and tool autonomy for 30 consecutive days):
1. The SPC converts its remaining fiat escrow into physical raw materials (copper wire, seeds, steel) until the balance reaches exactly **$\$0.00$**.
2. Founder 01 physically shears the 240V utility line and municipal water bib at $(x=95, y=32, z=48)$.
3. The mock fiat gateway HTTP worker threads are halted and joined.
4. The `Layer7DrainAccumulator` and all Layer 7 data structures are completely unmounted and zeroized from engine memory.
5. The engine transitions to `sovereignty_state = FULLY_DECOUPLED`, removing all fiat HUD elements and maintaining 60 FPS on pure Layers 1–6.

#### Gherkin Scenarios
```gherkin
Scenario: Successful execution of Scenario Omega Terminal Decoupling
  Given a community node maintaining 100% thermodynamic autarky for 30 consecutive days
  When Founder 01 commits the Terminal Decoupling intent at the parcel breaker box
  Then SPC fiat reserves are liquidated into physical copper and seeds, reaching exactly $0.00
  And the 240V utility drop line is physically severed
  And the Layer 7 software module unmounts from engine memory
  And all fiat indicators vanish from the HUD, unlocking the "Scenario Omega: Sovereign Autarky" victory state.

Scenario: Verification of Anti-Bleed boundary after Layer 7 unmounting
  Given an engine that has successfully executed Terminal Decoupling
  When the simulation continues running for 10,000 subsequent ticks
  Then zero null pointer dereferences or memory leaks occur
  And the engine executes at a full 60 FPS utilizing only Layers 1 through 6.
```

#### Technical Tasks
- [ ] Implement `ITerminalDecoupling` interface and autarky threshold evaluation (`layer7/terminal_decoupling.cpp`).
- [ ] Implement physical breaker cut logic clearing boundary voxel flags.
- [ ] Build graceful unmounting and deallocation sequence for Layer 7 worker threads and DES queues.
- [ ] Implement HUD transition pruning fiat widgets and activating sovereign visual shaders.

#### Interfaces & Data Structures
```cpp
namespace oasis::layer7 {
class ITerminalDecoupling {
public:
    virtual ~ITerminalDecoupling() = default;
    virtual bool EvaluateSovereigntyThreshold() const = 0;
    virtual void ExecuteTerminalDecoupling() = 0;
    virtual bool IsFullyDecoupled() const = 0;
};
}
```

#### Definition of Done
- Complete test run transitions from Stage 0 brownfield to Stage 3 Terminal Decoupling with zero memory leaks.
- Layer 7 successfully deallocates without affecting Layers 1–6 execution.
- 60 FPS maintained during and after decoupling sequence.

---

## 7. Epic 5: Sovereign Stack Verification Harness, Adversarial Attack Suite & Scenario Rho Gate

### Epic Overview
- **Strategic Scope:** Construct an automated, headless verification harness, CI testing runner, and adversarial fuzzing suite that validates full-stack integrity across Layers 1 through 7. Implement automated integration tests for Scenario Rho (The Adversarial Mesh: Sybil attacks, sensor spoofing, network partitions, double-spending, and legal subpoenas) and Scenario Omega (Terminal Decoupling), guaranteeing mathematical determinism, zero state divergence, sub-10ms frame budgets, and compliance with the 30 MB WASM heap boundary.
- **Architectural Scope:** Cross-Stack Full Integration (Layers 1–7) & Continuous Quality Gate.
- **Target Environments:** Headless Linux CI (Catch2 / ASan / UBSan / Valgrind) and Headless WASM Node Runner (`node --experimental-wasm-threads`).

---

### Story 5.1: Headless Deterministic Multi-Node Test Runner
- **User Story:** As a CI/CD infrastructure engineer, I want a headless test runner (`oasis_adversarial_runner`) capable of executing 10,000 deterministic ticks across 5 simulated nodes with seedable PRNG, so that all gameplay, CRDT sync, and municipal adversarial events can be verified in automated pipelines without requiring a graphical display.
- **Technical Context:** Builds `oasis_adversarial_runner`, a lightweight C++20 CLI utility that links against `liboasis_core.a`. It initializes an arbitrary number of virtual nodes, attaches simulated SITL hardware buses and DES municipal queues, steps the simulation at 1,000+ ticks/second on CPU, and computes a SHA-256 hash of the complete voxel and ledger memory space every 1,000 ticks. Two runs with identical seeds must produce bit-exact identical hashes across x86_64, ARM64, and WASM runtimes.

#### Gherkin Scenarios
```gherkin
Scenario: Deterministic headless simulation replay across 10,000 ticks
  Given the headless runner initialized with seed 1337 and 5 simulated peer nodes
  When 10,000 logic ticks execute without graphics rendering
  Then all nodes reach identical voxel and ledger states
  And the final SHA-256 state hash matches the benchmark golden master bit-for-bit
  And total execution time completes in less than 5.0 seconds.

Scenario: Cross-architecture state hash parity (x86_64 vs ARM64 vs WASM)
  Given state dumps recorded from native x86_64, native Apple Silicon ARM64, and Node.js WebAssembly
  When the verification tool compares the voxel grids and CRDT mutation logs
  Then zero byte divergences are detected
  And all floating-point thermodynamic calculations demonstrate exact parity.
```

#### Technical Tasks
- [ ] Build headless CLI test runner binary (`tools/oasis_adversarial_runner.cpp`).
- [ ] Implement SHA-256 state hash aggregator over active chunk memory and CRDT ledgers (`tools/state_hasher.cpp`).
- [ ] Implement virtual peer mesh interconnect for in-memory multi-node testing.
- [ ] Configure GitHub Actions / local CI workflows running regression replays on every commit.

#### Interfaces & Data Structures
```cpp
namespace oasis::tools {
struct SimulationRunSummary {
    uint64_t total_ticks_executed;
    uint32_t active_entities_count;
    uint32_t total_crdt_deltas_synced;
    uint32_t adversary_events_fired;
    std::array<uint8_t, 32> final_state_hash;
    double   elapsed_wallclock_seconds;
};

class IHeadlessRunner {
public:
    virtual ~IHeadlessRunner() = default;
    virtual void Initialize(uint64_t seed, uint8_t node_count, uint8_t adversary_level) = 0;
    virtual SimulationRunSummary RunTicks(uint64_t tick_count) = 0;
};
}
```

#### Definition of Done
- 10,000 ticks execute in $\le 5.0\text{ seconds}$ on standard modern hardware.
- Bit-exact state hashes match across x86_64, ARM64, and Emscripten WASM.
- Zero memory leaks detected under AddressSanitizer.

---

### Story 5.2: Scenario Rho Adversarial Full-Stack Attack Suite
- **User Story:** As a security auditor, I want an automated adversarial attack suite executing simultaneous threats across Layers 1 through 7 (Scenario Rho), so that we prove the sovereign stack survives physical sensor spoofing, network jamming, CRDT double-spending, Sybil botnets, and fiat banking freezes.
- **Technical Context:** Implements the canonical **Scenario Rho (The Adversarial Mesh)** stress test suite. The harness simultaneously unleashes five coordinated attack vectors:
1. **Layer 1 & 2:** Physical sensor spoofing (fake furnace heat without current draw).
2. **Layer 2:** RF jamming and high packet-loss injection on mesh links.
3. **Layer 3:** Concurrent double-spend attempts across partitioned peers.
4. **Layer 5:** Sybil clustering (50 coordinated fake DIDs attempting voting hijack).
5. **Layer 7:** Immediate 100% fiat account freeze and municipal stop-work orders.
The test asserts that the stack detects the anomaly, slashes attackers, isolates Sybils, resolves double-spends deterministically, and maintains 100% community survival on internal mutual credit.

#### Gherkin Scenarios
```gherkin
Scenario: Coordinated full-stack attack defense under Scenario Rho
  Given a 10-node cluster operating under Adversary Level 3 conditions
  When the Scenario Rho harness simultaneously injects sensor spoofing, Sybil clusters, and a 100% fiat freeze
  Then UHAI flags the spoofed sensor and halts invalid token minting within 2 ticks
  And Layer 5 EigenTrust dampens the Sybil cluster's voting power by over 90%
  And the community seamlessly pivots to internal mutual credit clearing without dropping 60 FPS
  And the test harness passes 100% of security assertions.

Scenario: Automated social slashing of double-spending rogue peer
  Given a rogue node attempting to spend the same Value Token concurrently with two disconnected merchants
  When the network partition heals and the conflicting CRDT deltas are gossiped
  Then the Kulkarni-Demir HLC and deterministic tie-breaker reject the double-spend
  And a SlashingAttestation cascades across the Web of Trust
  And the rogue peer's trust edges are automatically severed.
```

#### Technical Tasks
- [ ] Build automated Scenario Rho attack injector harness (`tests/scenario_rho_suite.cpp`).
- [ ] Implement synthetic packet corruption and RF jamming simulators in Layer 2.
- [ ] Build automated double-spend CRDT delta generator.
- [ ] Implement end-to-end assertions verifying zero funds lost and zero unhandled exceptions.

#### Interfaces & Data Structures
```cpp
namespace collective::tests {
struct RhoAttackVectorConfig {
    bool enable_sensor_spoofing;
    bool enable_sybil_cluster;
    bool enable_network_jamming;
    bool enable_crdt_double_spend;
    bool enable_fiat_freeze;
};

class IScenarioRhoTester {
public:
    virtual ~IScenarioRhoTester() = default;
    virtual void ConfigureAttacks(const RhoAttackVectorConfig& cfg) = 0;
    virtual bool ExecuteAttackScenario(uint64_t duration_ticks) = 0;
    virtual bool VerifySystemIntegrity() = 0;
};
}
```

#### Definition of Done
- Sensor spoofing flagged within $\le 2\text{ ticks}$.
- Double-spends resolved with 100% mathematical consistency.
- Sybil cluster dampened by $> 90\%$; all assertions pass in CI.

---

### Story 5.3: 72-Hour Network Partition & Delay-Tolerant Re-Sync Verifier
- **User Story:** As an off-grid network engineer, I want an automated test simulating a 72-hour network partition across 3 isolated node clusters with 10,000 concurrent mutations, so that I can verify that when the WAN heals, all nodes converge on an identical state without losing data or creating merge conflicts.
- **Technical Context:** Simulates a prolonged physical disruption (storm or infrastructure outage) where three clusters (Lot 402, Community Workshop, Farm Outpost) operate independently for 72 simulated hours. Each cluster generates thousands of local voxel edits, BPMN task completions, and mutual credit trades. When the network partition heals, nodes exchange CRDT deltas over low-bandwidth simulated LoRa and Wi-Fi links. The test asserts that state convergence completes rapidly and state hashes become bit-for-bit identical.

#### Gherkin Scenarios
```gherkin
Scenario: Complete state convergence after 72-hour network partition
  Given three clusters of nodes partitioned into isolated network islands for 72 simulated hours
  And each cluster executing over 3,000 independent voxel and ledger mutations
  When the network partition heals and mesh connectivity is restored
  Then nodes exchange cr-sqlite changesets and 24-byte VoxelMutationOp streams
  And all three clusters achieve bit-exact state parity in less than 3.5 seconds
  And zero manual conflict-resolution prompts are generated.
```

#### Technical Tasks
- [ ] Implement simulated network partition and latency injection proxy (`tests/partition_proxy.cpp`).
- [ ] Generate 10,000 concurrent, overlapping mutations across 3 virtual clusters.
- [ ] Implement convergence verification assert checking chunk hashes and wallet balances.
- [ ] Measure and optimize synchronization bandwidth over simulated 21.8 kbps LoRa links.

#### Interfaces & Data Structures
```cpp
namespace collective::tests {
struct PartitionTestReport {
    uint32_t ops_generated_cluster_a;
    uint32_t ops_generated_cluster_b;
    uint32_t ops_generated_cluster_c;
    uint32_t total_conflicts_resolved;
    double   convergence_duration_ms;
    bool     bit_exact_parity_achieved;
};
}
```

#### Definition of Done
- State convergence completes in $\le 3.5\text{ seconds}$ on CPU.
- 100% bit-exact state parity verified across all 3 clusters.
- Peak re-sync bandwidth fits within delay-tolerant LoRa/Wi-Fi constraints.

---

### Story 5.4: Scenario Omega (Terminal Decoupling) End-to-End Autarky Verification Gate
- **User Story:** As an architectural auditor, I want an automated integration test executing the complete player progression arc from Stage 0 brownfield boot to Stage 3 Terminal Decoupling (Scenario Omega), so that we verify that Layer 7 can be completely unmounted while the engine continues running at 60 FPS on pure sovereign protocols.
- **Technical Context:** Validates the entire macro-lifecycle of Oasis. The test boots Lot 402 with $\$1,450.00$ in fiat escrow, simulates the construction of 5 kW off-grid PV, 15 kWh battery storage, rainwater harvesting, and onboarding of 3 citizens. It then advances time through municipal inspections and Stripe holds until 100% thermodynamic autarky is attained. It commits the `TerminalDecouplingIntent`, asserts that fiat reserves reach $\$0.00$, severs utility lines, unmounts Layer 7, and asserts that the engine maintains 60 FPS for 5,000 subsequent ticks using only Layers 1–6.

#### Gherkin Scenarios
```gherkin
Scenario: End-to-end lifecycle execution through Terminal Decoupling
  Given a simulation running from Genesis Stage 0 on Lot 402
  When off-grid infrastructure achieves 100% thermodynamic autarky for 30 consecutive days
  And Founder 01 executes the Terminal Decoupling command
  Then the SPC fiat treasury liquidates to exactly $0.00
  And the 240V utility drop line is severed
  And Layer 7 unmounts from engine memory with zero memory leaks
  And the simulation maintains a steady 60 FPS for 5,000 subsequent ticks.
```

#### Technical Tasks
- [ ] Build end-to-end integration test runner for the complete 4-stage progression arc (`tests/scenario_omega_suite.cpp`).
- [ ] Validate automated liquidation of fiat escrow into physical inventory.
- [ ] Verify clean memory deallocation of Layer 7 DES queues and HTTP threads.
- [ ] Measure frame times before, during, and after decoupling to verify zero FPS degradation.

#### Interfaces & Data Structures
```cpp
namespace collective::tests {
struct OmegaVerificationReport {
    bool autarky_threshold_met;
    int64_t final_fiat_balance_cents;
    bool layer7_unmounted_cleanly;
    double min_fps_post_decoupling;
    size_t memory_freed_bytes;
};
}
```

#### Definition of Done
- Full progression arc executes autonomously in $\le 15.0\text{ seconds}$ in headless mode.
- Final fiat balance is asserted to be exactly $\$0.00$.
- Post-decoupling frame rate never drops below 60 FPS ($8.75\text{ ms}$ budget).

---

### Story 5.5: Continuous Memory, Cache & Performance Profiling CI Gate
- **User Story:** As the lead software architect, I want continuous compile-time `static_assert` verifications, ASan/TSan memory profiling, and frame budget timers enforced in CI, so that no commit can degrade the 8.75 ms frame ceiling or exceed the 30 MB WebAssembly memory boundary.
- **Technical Context:** Configures a strict CI quality gate enforcing the architectural constraints of Section 2:
1. `static_assert(sizeof(Voxel) == 4)`
2. `static_assert(sizeof(EntityPhysics) == 12)`
3. `static_assert(sizeof(VoxelMutationOp) == 24)`
4. `static_assert(sizeof(TelemetrySample) == 24)`
Runs Valgrind Massif and AddressSanitizer to guarantee zero memory leaks and heap allocations during frame loops. Instruments RAII microsecond profilers asserting that Layer 1–2 consumes $\le 0.30\text{ ms}$, Layer 3–5 $\le 0.15\text{ ms}$, Layer 4 $\le 0.15\text{ ms}$, and Layer 7 $\le 0.15\text{ ms}$. Verifies that total WASM heap usage remains strictly under **$30\text{ Megabytes}$**.

#### Gherkin Scenarios
```gherkin
Scenario: Compile-time struct layout and alignment verification
  Given the C++20 compiler compiling the Oasis core codebase
  When static assertions evaluate struct sizes and alignments
  Then Voxel is exactly 4 bytes, EntityPhysics is exactly 12 bytes, and VoxelMutationOp is exactly 24 bytes
  And the build succeeds with zero compiler warnings under -Werror.

Scenario: Profiling frame budget compliance under heavy simulation load
  Given 1,024 active entities and 500 remote CRDT ops arriving per tick
  When the engine ticks 1,000 times under continuous profiling
  Then Layer 1 through 7 processing time strictly adheres to the 0.75 ms CPU allocation
  And total WASM memory consumption remains strictly under 30 Megabytes.
```

#### Technical Tasks
- [ ] Add static assertions for all core data structures across all header files.
- [ ] Build automated CI workflow executing tests under AddressSanitizer and ThreadSanitizer.
- [ ] Implement RAII sub-millisecond CPU frame profiler with automated threshold alerts (`core/profiler.hpp`).
- [ ] Build WASM memory consumption monitor asserting total heap $\le 30\text{ MB}$.

#### Interfaces & Data Structures
```cpp
namespace oasis::perf {
struct FrameBudgetProfile {
    float l1_l2_physics_ms;
    float l3_l5_crdt_trust_ms;
    float l4_bpmn_tick_ms;
    float l7_adversary_ms;
    float webgpu_render_ms;
    float total_frame_ms;
};

class IPerformanceMonitor {
public:
    virtual ~IPerformanceMonitor() = default;
    virtual void RecordFrame(const FrameBudgetProfile& profile) = 0;
    virtual bool AssertBudgetCompliance() const = 0;
};
}
```

#### Definition of Done
- 100% of compile-time static asserts pass.
- ASan, UBSan, and TSan report zero errors in full test runs.
- Frame budget strictly respects $\le 8.75\text{ ms}$ total frame ceiling ($\le 0.75\text{ ms}$ CPU simulation).
- WASM heap strictly $\le 30\text{ MB}$.

---

## 8. Quality Assurance Matrix, Definition of Done & Consensus Attestation

### 8.1. Project-Wide Definition of Done (DoD)
To ensure production-grade engineering excellence, every user story within `SOVEREIGN_STACK_BACKLOG.md` must satisfy the following non-negotiable criteria before being marked Complete:

1. **Dual-Runtime Verification:** Code compiles and executes identically on native desktop C++20 (SDL2 / Dawn WebGPU / Linux POSIX) and browser WebAssembly (Emscripten / WebGPU / OPFS).
2. **Deterministic Reproducibility:** Simulation runs produce bit-exact identical SHA-256 state hashes given the same seed and input sequence across all supported architectures.
3. **Strict Frame Budget Adherence:** Total simulation processing respects the allocated per-subsystem frame budgets, maintaining 60 FPS ($\le 8.75\text{ ms}$ frame ceiling; $\le 0.75\text{ ms}$ CPU logic budget; $\le 0.15\text{ ms}$ Layer 7 allocation).
4. **Zero Heap Allocation in Hot Loops:** No dynamic memory allocation (`malloc`, `new`, `std::vector` resizing) occurs during per-frame simulation and rendering passes.
5. **Memory Envelope Compliance:** Total memory footprint in browser WebAssembly sandboxes remains strictly under **$30\text{ Megabytes}$**.
6. **Anti-Bleed Containment Verified:** Automated CI static analysis confirms that zero Layer 7 fiat variables, zoning terms, or legacy identifiers contaminate Layers 1 through 4.
7. **Authentic Implementations (No Facades):** All cryptographic operations, CRDT replications, thermodynamic calculations, and municipal state machines maintain genuine, verifiable internal state.
8. **Automated Test Coverage:** All stories include comprehensive Catch2 unit and integration tests passing with 100% assertions in continuous integration pipelines.
9. **Tactile UX Telegraphs:** Every underlying protocol state transition (utility rate hike, zoning citation, token minting, fatigue threshold) is paired with an intuitive visual or audio feedback affordance in the user interface.

---

### 8.2. Scenario Traceability Matrix
The user stories defined in this backlog directly implement and validate the foundational scenarios ratified in `docs/scenarios/`:

| Scenario ID | Scenario Title | Primary Backlog Epics & User Stories | Core Mechanical Validation |
| :---: | :--- | :--- | :--- |
| **Scenario Alpha** | The Fabrication Commons | Epic 1 (Story 1.1), Epic 3 (Story 3.5) | Tool sharing, G-code execution, double-auction material matching. |
| **Scenario Gamma** | The Commons Kitchen | Epic 1 (Story 1.2), Epic 3 (Story 3.3, 3.4) | Caloric budgeting, shared food preparation, mutual credit barter. |
| **Scenario Iota** | The Seed Library & DPL | Epic 2 (Story 2.5), Epic 4 (Story 4.5) | Defensive Patent Licenses, open-source germplasm preservation. |
| **Scenario Tau** | Steward Onboarding | Epic 2 (Story 2.1, 2.2), Epic 3 (Story 3.2) | Campfire key-signing, W3C DIDs, Trust Ring 1 expansion, BBS+ VCs. |
| **Scenario Sigma** | Altruism Fatigue & Burnout | Epic 1 (Story 1.1), Epic 3 (Story 3.2, 3.4) | Metabolic fatigue limit (255), labor sharing via BPMN WorkTokens. |
| **Scenario Rho** | The Adversarial Mesh | Epic 1 (Story 1.5), Epic 3 (Story 3.1, 3.2), Epic 5 (Story 5.2) | Sensor spoofing, Sybil clusters, 72h partition merge, double-spends. |
| **Scenario Omega** | Terminal Decoupling | Epic 4 (Story 4.5, 4.6), Epic 5 (Story 5.4) | 100% autarky, SPC liquidation, grid severance, Layer 7 unmounting. |

---

### 8.3. Architectural Council Sign-Off & Attestation

We, the members of the Architectural Council, hereby attest that this Product Backlog represents the definitive, exhaustive, and production-grade specification for the Sovereign Stack and Layer 7 Adversarial Municipal Emulation.

By unanimous agreement, all technical tensions regarding dual-runtime execution, cryptographic friction vs 60 FPS responsiveness, adversarial pacing, and anti-bleed memory isolation have been resolved.

**Sign-off:**
- **Product Owner (`explorer_po_r1`):** *Approved. Pacing, embodiment, and gameplay tension rigorously grounded.*
- **Software Architect (`explorer_arch_r1`):** *Approved. Struct alignments, UHAI interfaces, and 8.75 ms frame budget strictly guaranteed.*
- **Sovereign Stack Expert (`explorer_sovereign_r1`):** *Approved. Cryptographic autonomy, delay-tolerant mesh, and Layer 7 adversarial threat modeling fully specified.*

**Unanimous Consensus Reached.**
