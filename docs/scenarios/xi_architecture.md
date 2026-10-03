# Scenario Xi: Algorithmic Architecture

*   **Identifier:** `SCN-XI-ARCH`
*   **System Epic:** Parametric Design, Structural Simulation, and Decentralized Construction
*   **Primary Layers Tested:** L1 (Physical), L2 (Twin), L3 (Ledger), L4 (Orchestrator), L5 (Governance), L6 (Semantic), L7 (Proxy)
*   **Pass/Fail Metric:** Successful procedural generation of a load-bearing structure optimized for local material inventory; verifiable structural integrity simulation prior to physical assembly; successful legacy permit ingestion.

---

## 1. Problem Statement & Legacy Failure

Legacy construction relies on high-entropy, globalized supply chains (Portland cement, structural steel, mass-produced dimensional lumber) that ignore local microclimates and thermodynamic efficiency. Furthermore, architectural design is gatekept by expensive legacy firms, resulting in standardized, culturally sterile suburban sprawl.
When a community attempts to build alternative structures (e.g., rammed earth, reciprocal timber roofs, geodesic domes), they hit a bureaucratic wall. Legacy municipal zoning and building departments demand standardized engineering stamps, forcing builders back into the extractive fiat economy.

## 2. The Collective Workflow (7-Layer Traversal)

Scenario Xi uses open-source, parametric algorithms to generate structural blueprints dynamically. It takes the exact inventory of the Node (e.g., "We have 40 irregular Alder logs and 200 3D-printed PETG brackets") and computes a structurally sound, custom geometry that can be assembled by decentralized labor.

### Layer 7: The Legacy Proxy (The PE Stamp & Permit Shield)
*   **The Fiat Bridge (Engineering):** Municipalities do not accept "the algorithm said it was safe." The Social Purpose Corporation (SPC) acts as the legacy contractor. It takes the generated parametric CAD file and uses a small amount of fiat to hire a legacy Professional Engineer (PE) to review and stamp the plans.
*   **Permit Insulation:** The SPC submits the stamped plans to the legacy building department, absorbing all permit fees and zoning hearings, legally shielding the decentralized labor force from state interference.

### Layer 6: Semantic Intent
*   A Citizen or Steward emits a `ParametricBuildIntent` (e.g., requesting a 150 sq-ft greenhouse or a community pavilion).
*   The intent defines the spatial bounding box, the algorithmic archetype (e.g., "Zome", "Geodesic", "Timber-Frame"), and the load requirements (e.g., "Must withstand 40psf snow load").

### Layer 5: Policy & Web of Trust (The Safety & Zoning Gate)
*   **Structural Integrity Gate:** Layer 5 routes the intent through a local digital twin simulation. If the algorithm detects the structure will collapse under the bioregion's historical wind or snow loads, it mathematically rejects the design.
*   **Aesthetic & Zoning Consensus:** If the structure is in a shared commons, Layer 5 requires a multi-sig consensus from surrounding neighbors to ensure the design does not violate local solar-rights (blocking a neighbor's solar panels) or aesthetic budgets.

### Layer 4: Orchestration (BOM & Sweat Equity Routing)
*   The BPMN engine breaks the approved algorithm down into a massive Bill of Materials (BOM) and labor sequence.
*   It routes material bounties across the mesh: requesting Scenarios Alpha and Theta to fabricate the structural brackets, and Scenario Eta to release the seasoned timber.
*   It schedules "Barn Raising" events, breaking the assembly into modular tasks.

### Layer 3: Ledger (Sweat Equity)
*   Legacy construction requires massive upfront fiat loans (mortgages). In the mesh, the structure is capitalized via thermodynamic "Sweat Equity."
*   Citizens who show up to physically assemble the structure, or who provide the raw materials from their local hoppers, are minted Value Tokens or granted fractional usage rights to the completed structure.

### Layer 2 & 1: Digital Twin & Physical Reality
*   **Telemetry (L2):** During construction, LiDAR or drone photogrammetry scans the physical progress, overlaying it against the digital twin to detect millimeter-level deviations before the next layer is added.
*   **Physical (L1):** Earth, timber, hardware, localized physical labor, and the finalized shelter.

---

```mermaid
graph TD
    subgraph Layer 7: Legacy Proxy
        PE[Legacy Professional Engineer]
        Gov[Municipal Building Dept]
        SPC[Social Purpose Corporation]
    end

    subgraph Layer 6: Intent
        Citizen[Citizen / Steward]
        Intent[Emits ParametricBuildIntent]
    end

    subgraph Layer 5: Policy
        Physics{L2 Structural <br> Integrity Simulation}
        Zoning{Check Solar Rights <br> & Trust Ring Consensus}
    end

    subgraph Layer 4: Orchestrator
        BPMN[Generate BOM & Barn Raising Schedule]
        SubBounty[Route Sub-tasks to <br> Printers & Forestry]
    end

    subgraph Layer 3: Ledger
        Mint[Mint Value Tokens <br> for Sweat Equity / Labor]
    end

    subgraph Layer 2 & 1: Physical Reality
        Blueprint[L2: Phantom Voxel Projection]
        Action[L1: Physical Assembly]
        LiDAR[L2: Drone Verification]
    end

    %% Legacy Flow
    SPC -->|Pays Fiat for Stamp| PE
    PE -->|Stamps CAD| SPC
    SPC -->|Submits Permit| Gov
    Gov -->|Approves| BPMN

    %% Sovereign Flow
    Citizen --> Intent
    Intent --> Physics
    Physics -->|Safe| Zoning
    Physics -->|Fails Load Test| Reject[Modify Algorithm]
    
    Zoning -->|Approved| SPC
    Zoning -->|Conflicts| Reject
    
    BPMN --> SubBounty
    BPMN --> Blueprint
    
    Blueprint --> Action
    Action --> LiDAR
    LiDAR -.->|Deviation Check| Blueprint
    
    Action --> Mint
```

---

## 3. Oasis Engine Implementation Specification

In the C++ Oasis engine, this scenario tests the procedural generation and physics mechanics of the voxel grid:

*   **Structural Load Physics:** The C++ core must implement a gravity and load-bearing simulation. Voxels possess a `load_bearing_capacity` and `mass`. If a player entity attempts to build a roof without adequate support columns, the accumulated mass must exceed the structural limits of the base voxels, triggering a `COLLAPSE_EVENT`.
*   **Procedural Blueprinting:** The engine must be able to read a `ParametricBuildIntent` JSON, scan the player's available inventory, and dynamically place phantom "blueprint" voxels in the world space, guiding the player where to place physical materials.
*   **BOM Calculation:** The engine must accurately sum the required voxels to complete the phantom blueprint and deduct them from the local chunk inventory as they are placed.

---

```json
{
  "@context": [
    "[https://schema.org/](https://schema.org/)",
    "[https://collective.network/ontology/v1/](https://collective.network/ontology/v1/)"
  ],
  "@type": "ParametricBuildIntent",
  "identifier": "urn:uuid:1a2b3c4d-5e6f-7a8b-9c0d-1e2f3a4b5c6d",
  "issuerDid": "did:mesh:node04:architect_sara",
  "designParameters": {
    "archetype": "Geodesic_Dome_V3",
    "boundingVolumeCubicMeters": 45.0,
    "targetLocation": {
      "chunkCoordinates": [12, 5, 8],
      "gisPolygon": "ipfs://QmSitePolygonHash..."
    }
  },
  "structuralConstraints": {
    "snowLoadPsf": 40.0,
    "windLoadMph": 85.0,
    "seismicCategory": "D"
  },
  "materialInventory": {
    "primaryStruts": "Coppiced_Alder_Logs",
    "primaryHubs": "PETG_3D_Printed_Joints"
  },
  "legacyCompliance": {
    "peStampRequired": true,
    "municipalPermitRequired": true,
    "allocatedFiatBudget": "1200.00"
  }
}
```

---

## 4. Acceptance Test Gates (Pass/Fail)

| Checkpoint | Validation Method | Pass Condition |
| :--- | :--- | :--- |
| **G1: Structural Integrity Sim** | Physics Engine Load Test | Attempting to execute a blueprint with a roof span exceeding the mathematical sheer strength of the supporting voxel material triggers a programmatic `DESIGN_REJECTED` state. |
| **G2: Material Orchestration** | BOM Generation | The BPMN engine successfully parses a complex geometric design into exact sub-bounties for 3D printed joints and timber lengths. |
| **G3: Solar Rights Validation** | Voxel Raycasting | The system rejects a build intent if the proposed bounding box casts a shadow (simulated via raycasting from the local sun vector) over a voxel marked as `SOLAR_ARRAY` on an adjacent property. |
| **G4: The Permit Bridge** | Layer 7 Aggregation | The system successfully suspends the physical assembly workflow until a simulated Layer 7 manual override (`PE_STAMP_RECEIVED`) is triggered by the SPC admin. |
