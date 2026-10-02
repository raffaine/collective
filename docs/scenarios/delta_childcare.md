# Scenario Delta: Mutual Aid Childcare Pod

*   **Identifier:** `SCN-DELTA-CHILD`
*   **System Epic:** Trust Rings, Invisible Labor Valuation, and Decentralized Scheduling
*   **Primary Layers Tested:** L2 (Twin), L3 (Ledger), L4 (Orchestrator), L5 (Governance), L6 (Semantic), L7 (Proxy)
*   **Pass/Fail Metric:** Successful decentralized scheduling of care blocks respecting strict adult-to-child ratios; mathematical validation of care-work compensation via Value Tokens; zero legacy state childcare licensure violations.

---

## 1. Problem Statement & Legacy Failure

In the legacy economy, childcare is simultaneously one of the most expensive services a family can purchase and one of the lowest-paid professions in the market. The legacy state enforces rigid licensure laws that heavily capitalize corporate daycares, while atomizing the nuclear family.
Furthermore, informal care work (often performed by mothers or elders) is treated as "invisible labor" with zero recognized economic value. When parents try to form informal "co-ops," the scheduling logistics often collapse under human burnout (Scenario Sigma), and exchanging fiat money triggers state intervention for operating an "unlicensed daycare."

## 2. The Collective Workflow (7-Layer Traversal)

Scenario Delta utilizes a highly restricted **Trust Ring** to distribute childcare across a trusted pod of families. It mathematically recognizes care work as valuable negentropy (allowing other parents to perform high-exergy tasks) while legally shielding the pod from legacy state interference.

### Layer 7: The Legacy Proxy (Liability & Co-op Shielding)
*   **Regulatory Shield:** In many jurisdictions, caring for children from multiple families in exchange for fiat currency legally constitutes an "unlicensed daycare." Because the mesh utilizes internal Value Tokens (representing thermodynamic work, not fiat), the pod legally operates as a "Babysitting Co-op" or mutual aid group under the Social Purpose Corporation's (SPC) umbrella.
*   **Liability Wrapper:** The SPC holds a general umbrella liability insurance policy for activities on community grounds, protecting the individual host homes from ruinous legacy lawsuits.
*   **Fiat Procurement:** The SPC aggregates fiat to bulk-purchase physical supplies (diapers, organic snacks, craft materials) from legacy suppliers, distributing them to host homes.

### Layer 6: Semantic Intent
*   Parents emit a `CareIntent` (requesting care for their children for a specific time block).
*   Hosts emit a `CareOffering` (offering their time and physical space).
*   Both intents carry critical metadata: allergies, behavioral support needs, and age brackets.

### Layer 5: Policy & Web of Trust (The Trust Ring)
*   **Absolute Privacy:** Unlike Fabrication Bounties, Childcare intents are *not* broadcast to the general mesh. They are cryptographically locked to a specific `TrustRing` (e.g., "Duvall Pod Alpha"). Only authorized DIDs can decrypt and view the schedule.
*   **Competency & Background Gates:** To be added to the Trust Ring as a Caregiver, the user's decentralized identity must hold verified attestations (e.g., `Pediatric_CPR_L1`, `Background_Vouch`). Adding a new member to the pod requires an `n-of-m` multi-signature consensus from existing parents.

### Layer 4: Orchestration (Ratio & Conflict Engine)
*   The BPMN engine acts as the strict logistical scheduler.
*   **Ratio Enforcement:** The state machine mathematically prevents overbooking. If the Trust Ring policy sets a ratio of 1 Adult to 4 Children, the engine will block any `CareIntent` that attempts to push the host's active queue to 5 children, forcing a second adult to accept a `CareOffering` to unlock the slots.

### Layer 3: Ledger & Invisible Labor
*   Caregiving is thermodynamic labor. Keeping human children safe, fed, and emotionally regulated requires immense caloric and psychological exergy. 
*   The host is minted Value Tokens directly from the parents' escrow. This formally integrates care work into the local economy, allowing a parent who watches the pod's kids all week to use those earned tokens to "buy" 3D printed parts (Scenario Alpha) or meals (Scenario Gamma).

### Layer 2 & 1: Digital Twin & Physical Reality
*   **Telemetry (L2):** Secure physical handoffs are logged. NFC or BLE tags on children's backpacks register with the host's edge node upon arrival and departure, updating the BPMN state machine so parents have cryptographic proof of location. Environmental sensors monitor indoor CO2 levels in the playroom to ensure adequate ventilation.
*   **Physical (L1):** The living rooms, backyards, local parks, and the actual caloric and emotional labor of the caregivers.

---

```mermaid
graph TD
    subgraph Layer 7: Legacy Proxy
        SPC[Social Purpose Corporation]
        Insure[Umbrella Liability Insurance]
        SPC --- Insure
    end

    subgraph Layer 6: Intent
        Parent[Parent emits CareIntent]
        Host[Host emits CareOffering]
    end

    subgraph Layer 5: Policy & Trust Rings
        RingCheck{Are Both DIDs in <br>Duvall Pod Alpha?}
        RatioPolicy{Is Adult:Child <br>Ratio <= 1:4?}
    end

    subgraph Layer 4: Orchestrator
        Match[BPMN Scheduler Locks Slot]
        Wait[Wait for Handoff Event]
    end

    subgraph Layer 3: Ledger
        Escrow[Lock Value Tokens]
        Settle[Transfer Tokens to Host]
    end

    subgraph Layer 2: Twin (Edge Telemetry)
        NFC_In[BLE/NFC Tag: Drop-off]
        NFC_Out[BLE/NFC Tag: Pick-up]
    end

    %% Routing
    Parent --> RingCheck
    Host --> RingCheck
    RingCheck -->|Yes| RatioPolicy
    RingCheck -->|No| Block[Unauthorized]
    
    RatioPolicy -->|Valid| Match
    RatioPolicy -->|Exceeded| Queue[Hold for 2nd Adult]
    
    Match --> Escrow
    Escrow --> Wait
    
    Wait -.->|Listens for L2| NFC_In
    NFC_In --> NFC_Out
    NFC_Out --> Settle
    
    %% Legal Shielding
    Host -.->|Protected By| Insure
```

---

## 3. Oasis Engine Implementation Specification

In the Oasis C++ simulation, this scenario tests the engine's ability to handle private Trust Rings and multi-entity dependency logic:

*   **Trust Ring Cryptography:** The C++ core must implement a mock asymmetric encryption layer where intents are signed and encrypted against a `TrustRing` public key, ensuring unauthorized voxels/NPCs cannot observe the data.
*   **Spatial Occupancy Limits:** The `ChunkManager` must enforce dynamic entity caps on specific voxels. If an `NPC_CHILD` entity attempts to route into a `LIVING_ROOM` chunk, the engine must verify the `NPC_ADULT` count in that chunk satisfies the $1:4$ ratio.
*   **NFC Telemetry Simulation:** A child entity crossing the boundary of a registered Care Chunk triggers a simulated Layer 2 MQTT payload (`STATUS: CHECKED_IN`), advancing the BPMN state.

---

```json
{
  "@context": [
    "[https://schema.org/](https://schema.org/)",
    "[https://collective.network/ontology/v1/](https://collective.network/ontology/v1/)"
  ],
  "@type": "CareIntent",
  "identifier": "urn:uuid:a1b2c3d4-e5f6-7890-1234-56789abcdef0",
  "issuerDid": "did:mesh:node02:parent_sarah",
  "targetTrustRing": "did:mesh:ring:duvall_pod_alpha",
  "careRequirements": {
    "numberOfChildren": 2,
    "ageBrackets": ["Toddler", "SchoolAge"],
    "allergies": ["TreeNuts"],
    "specialInstructions": "Sam needs help with remote learning login at 10:00 AM."
  },
  "temporalVector": {
    "startTime": "2026-10-05T08:30:00Z",
    "endTime": "2026-10-05T14:30:00Z",
    "totalHours": 6.0
  },
  "escrowCapacity": {
    "maxValueTokens": "18.00"
  }
}
```

---

## 4. Acceptance Test Gates (Pass/Fail)

| Checkpoint | Validation Method | Pass Condition |
| :--- | :--- | :--- |
| **G1: Privacy Isolation** | Cryptographic View Check | A simulated user outside the defined `TrustRing` attempts to query the local network and receives zero data regarding child locations or schedules. |
| **G2: Strict Ratio Enforcement** | BPMN Queue Rejection | A `CareIntent` that would result in 5 children to 1 adult is mathematically rejected by the orchestrator and returned to the queue. |
| **G3: Secure Handoff** | L2 Telemetry Chain | Escrow Value Tokens are *only* transferred when the child's BLE/NFC tag registers a departure event from the host's local MQTT broker. |
| **G4: Multi-Sig Governance** | Trust Ring Modification | Adding a new caregiver to the pod requires cryptographic signatures from at least 3 existing parents before the network accepts their `CareOffering`. |
