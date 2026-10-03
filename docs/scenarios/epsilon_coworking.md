# Scenario Epsilon: Sovereign Third Place (Coworking)

*   **Identifier:** `SCN-EPSILON-COWORK`
*   **System Epic:** Decentralized Workspace, Resource Multiplexing, and Commercial Facade
*   **Primary Layers Tested:** L2 (Twin), L3 (Ledger), L4 (Orchestrator), L5 (Governance), L6 (Semantic), L7 (Proxy)
*   **Pass/Fail Metric:** Autonomous spatial booking and access control without human front-desk staff; successful fractional billing of power/bandwidth; seamless ingestion of fiat from legacy non-members to subsidize the lease.

---

## 1. Problem Statement & Legacy Failure

The "Third Place" (spaces outside of home and work where communities gather) has been entirely financialized. Remote workers and creators are forced to choose between the psychological isolation of working from home, buying a $7 coffee every two hours to justify occupying a cafe table, or paying exorbitant monthly fiat subscriptions to centralized corporate coworking spaces (e.g., WeWork) that extract massive profit margins.

## 2. The Collective Workflow (7-Layer Traversal)

Scenario Epsilon transforms underutilized physical spaces (a retrofitted neighborhood garage, an empty retail storefront, or a community hall) into an autonomous, shared workspace. It operates a dual-economy: extracting fiat from the legacy world to pay state liabilities, while operating on thermodynamic Value Tokens for the local mesh.

### Layer 7: The Legacy Proxy (The Commercial Facade)
*   **Property & ISP Shield:** The Social Purpose Corporation (SPC) holds the commercial lease or property deed and pays the legacy fiber-optic ISP and municipal utility bills.
*   **Trojan Coworking (Fiat Ingestion):** To fund these legacy tethers, the space presents a standard commercial facade to the public. Legacy remote workers can book a desk via a standard Web2 website and pay a $25/day fiat drop-in rate via Stripe. The SPC absorbs this fiat, effectively making the physical space "free" for the sovereign citizens of the Node to use.

### Layer 6: Semantic Intent
*   Citizens emit a `WorkspaceIntent` requesting specific spatial resources (e.g., a standing desk, a soundproof podcast booth, high-bandwidth routing) for a specific time block.
*   The space itself continuously broadcasts a `SpatialOffering` detailing current occupancy and available assets.

### Layer 5: Policy & Web of Trust
*   **The Etiquette Gate:** Shared spaces degrade quickly without accountability (the tragedy of the commons). Layer 5 enforces a reputational stake. If a user leaves the podcast booth a mess or violates the acoustic budget (talking loudly in the quiet zone), local Stewards flag their DID. Future `WorkspaceIntents` from that user will require a massive Value Token collateral lock, or be rejected entirely.

### Layer 4: Orchestration (Spatial Multiplexing)
*   The BPMN engine acts as the invisible front desk manager.
*   It handles conflict resolution, ensuring a single physical desk cannot be double-booked. Once a booking is confirmed, it queues the Layer 2 actuation sequence for the user's arrival window.

### Layer 3: Ledger & Fractional Exergy Billing
*   Unlike legacy subscriptions, mesh citizens pay purely for the thermodynamic and spatial footprint they consume.
*   Value Tokens are escrowed based on time, but final settlement is adjusted by Layer 2 telemetry (e.g., if the user plugged in a 1000W rendering rig vs. a 15W laptop, their exergy deduction scales accordingly).

### Layer 2 & 1: Digital Twin & Physical Reality
*   **Telemetry & Actuation (L2):** The mesh utilizes local MQTT. When the user arrives, their mobile device authenticates via BLE (Bluetooth Low Energy) to a localized smart lock, granting physical entry. Smart plugs at the assigned desk activate power. The local router provisions a Wi-Fi VLAN specific to their DID.
*   **Physical (L1):** Desks, ergonomic chairs, HVAC, photons, and localized acoustic paneling.

---

```mermaid
graph TD
    subgraph Layer 7: Legacy Market
        Normie[Legacy Remote Worker]
        Stripe[Fiat Gateway]
        SPC[Social Purpose Corporation]
        Landlord[Legacy Landlord / Utilities]
    end

    subgraph Layer 6: Intent
        Citizen[Mesh Citizen]
        Intent[Emits WorkspaceIntent]
    end

    subgraph Layer 5: Policy
        RepCheck{Check DID Reputation <br> & Etiquette History}
    end

    subgraph Layer 4: Orchestrator
        BPMN[Calendar & Spatial Multiplexer]
    end

    subgraph Layer 3: Ledger
        Escrow[Lock Value Tokens]
        Settle[Fractional Settlement <br> based on Exergy]
    end

    subgraph Layer 2: Twin (Edge Actuation)
        Door[BLE Smart Lock]
        Power[Smart Plug Actuation]
        Network[VLAN Provisioning]
    end

    %% Legacy Flow
    Normie -->|Books via Web2| Stripe
    Stripe -->|Pays USD| SPC
    SPC -->|Pays Rent| Landlord
    SPC -->|Blocks out schedule| BPMN

    %% Sovereign Flow
    Citizen --> Intent
    Intent --> RepCheck
    RepCheck -->|Approved| BPMN
    RepCheck -->|Poor Rep| Block[Require High Collateral]
    
    BPMN --> Escrow
    Escrow --> Door
    Door -->|Proximity Auth| Power
    Power --> Network
    
    Network -.->|End of Session| Settle
```

---

## 3. Oasis Engine Implementation Specification

In the C++ Oasis engine, spatial multiplexing requires rigorous voxel occupancy logic:

*   **Zone Definition:** The `ChunkManager` must support "Zone" metadata, grouping multiple voxels into a `SOUNDPROOF_BOOTH` or `HOT_DESK`.
*   **BLE Actuation Sim:** The orchestrator must simulate a proximity handshake. A player entity moving into the chunk adjacent to a locked door voxel must broadcast a valid cryptographic token to transition the door voxel's state to `UNLOCKED`.
*   **Exergy Drain:** The engine tracks the `temperature` and `electrical_load` of the desk voxel while occupied, correctly debiting the simulated user's CRDT wallet per tick.

---

```json
{
  "@context": [
    "[https://schema.org/](https://schema.org/)",
    "[https://collective.network/ontology/v1/](https://collective.network/ontology/v1/)"
  ],
  "@type": "WorkspaceIntent",
  "identifier": "urn:uuid:d3f4a9c2-88bb-4e1a-9f01-7c3d2e1f4a5b",
  "issuerDid": "did:mesh:node04:creator_jules",
  "targetFacility": "did:mesh:node04:space:duvall_commons",
  "resourceRequirements": {
    "zoneType": "DeepWork",
    "assets": [
      "StandingDesk",
      "Ethernet_1Gbps",
      "Monitor_4K"
    ],
    "acousticTolerance": "Silent"
  },
  "temporalVector": {
    "startTime": "2026-10-06T09:00:00Z",
    "endTime": "2026-10-06T13:00:00Z",
    "totalHours": 4.0
  },
  "escrowCapacity": {
    "maxValueTokens": "6.50"
  }
}
```

---

## 4. Acceptance Test Gates (Pass/Fail)

| Checkpoint | Validation Method | Pass Condition |
| :--- | :--- | :--- |
| **G1: Autonomous Access** | Layer 2 Actuation | The door latch only actuates if the simulated user's DID has an active, escrow-funded booking for the current time window. |
| **G2: Offline Authentication** | Internet Severance Test | The BLE door handshake and local Wi-Fi provisioning succeed even if the Node's upstream legacy ISP connection is severed. |
| **G3: Fractional Billing** | Energy Telemetry | The final ledger settlement accurately reflects the variable power draw recorded by the desk's smart plug during the session. |
| **G4: The Fiat Subsidy** | Layer 7 Aggregation | A mock Web2 fiat booking successfully blocks out physical availability on the internal mesh calendar, and the fiat revenue is routed to the SPC's simulated treasury. |
