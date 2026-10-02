# Scenario Theta: Neighborhood Micro-Foundry

*   **Identifier:** `SCN-THETA-FOUNDRY`
*   **System Epic:** Heavy Exergy Fabrication, Decentralized Metallurgy, and Batch Processing
*   **Primary Layers Tested:** L1 (Physical), L2 (Twin), L3 (Ledger), L4 (Orchestrator), L5 (Governance), L6 (Semantic), L7 (Proxy)
*   **Pass/Fail Metric:** Safe, localized phase-change of scrap metal into functional cast components; strict automated batching to prevent exergy waste; zero violations of municipal fire or emissions codes.

---

## 1. Problem Statement & Legacy Failure

The legacy recycling system is fundamentally broken. Citizens engage in "wish-cycling," throwing aluminum and steel into municipal bins where it is often shipped to other continents burning heavy bunker fuel, or sent straight to landfills due to sorting costs. Meanwhile, if a local citizen needs a custom metal bracket or gear, they must order virgin metal products forged in massive, centralized industrial centers, incurring massive carbon drag.
Bringing metallurgy back to the neighborhood scale solves this, but introduces severe localized risks: extreme thermal hazards ($>700^\circ\text{C}$), toxic fumes from impure scrap, and massive electrical/fuel exergy requirements that make single-item casting thermodynamically disastrous.

## 2. The Collective Workflow (7-Layer Traversal)

Scenario Theta establishes a localized, closed-loop metal economy. It incentivizes the community to mine their own waste stream (scrap aluminum/copper) and uses strict mathematical batching to make localized smelting thermodynamically viable.

### Layer 7: The Legacy Proxy (Fire Codes & Artisan Fiat)
*   **Regulatory Shield:** A neighborhood foundry triggers aggressive municipal fire codes and zoning laws. The Social Purpose Corporation (SPC) leases a specific, industrially-zoned or appropriately permitted outbuilding. The SPC holds the high-liability commercial insurance policies necessary to protect the Node from bankruptcy in the event of an accident.
*   **Trojan Casting (Fiat Ingestion):** To pay for the crucible replacements, PPE, and legacy utility costs, the foundry takes on artisan commercial work (e.g., casting custom bronze plaques or bespoke architectural hardware) for the Web2 legacy market. Legacy customers pay fiat via Stripe; the mesh citizens do the work for Value Tokens.

### Layer 6: Semantic Intent
*   Citizens emit a `ScrapDeposit` (bringing clean, sorted aluminum cans or broken extrusions to the foundry).
*   Engineers emit a `CastingBounty` (requesting a specific CAD geometry to be cast in sand or lost-PLA, specifying the required alloy).

### Layer 5: Policy & Web of Trust (The Safety & Emissions Gate)
*   **Extreme Competency Gating:** A 3D printer can be run by a novice. A foundry cannot. Layer 5 absolutely restricts access to the kiln. Only Stewards holding a `Foundry_Safety_L3` and `First_Aid_Burn` Verifiable Credential can accept a casting bounty.
*   **Emissions Budget:** If the local air quality index (AQI) is poor, or if the time is outside the designated industrial noise window, Layer 5 policy suspends all foundry operations to maintain peace with legacy neighbors.

### Layer 4: Orchestration (The Batching Engine)
*   **Thermal Aggregation:** Heating a crucible to $750^\circ\text{C}$ costs immense exergy. The BPMN engine will *not* actuate the kiln for a single 100-gram part. It pools `CastingBounty` requests over days or weeks. Only when the queue reaches $85\%$ of the crucible's volumetric capacity does the orchestrator release the job to a Steward.

### Layer 3: Ledger (The Waste-to-Value Loop)
*   **Mining the Suburbs:** Citizens are minted Value Tokens for bringing verified, sorted scrap metal (weighed and spectrographically checked if possible). This turns household waste into local currency.
*   **Exergy Escrow:** The requester of a cast part locks tokens covering the immense thermodynamic cost of the kiln's electrical draw or biochar fuel consumption.

### Layer 2 & 1: Digital Twin & Physical Reality
*   **Telemetry (L2):** Type-K thermocouples inside the kiln broadcast exact temperatures via MQTT. Air quality sensors (VOC and PM2.5) monitor the exhaust ventilation. If toxic off-gassing from impure scrap spikes, L2 cuts the induction power via an automated relay.
*   **Physical (L1):** Electric induction kilns, graphite crucibles, Petrobond (green sand), safety visors, leather aprons, and liquid metal.

---

```mermaid
graph TD
    subgraph Layer 7: Legacy Proxy
        SPC[Social Purpose Corporation]
        FireMarshal[Municipal Fire / EPA]
        FiatClient[Legacy Artisan Client]
    end

    subgraph Layer 6: Intent
        Scrapper[Citizen emits ScrapDeposit]
        Requester[Citizen emits CastingBounty]
    end

    subgraph Layer 5: Policy
        CredCheck{Does Steward Hold <br> Foundry_Safety_L3?}
        AQICheck{Is Local Air Quality <br> & Time Window Safe?}
    end

    subgraph Layer 4: Orchestrator
        BatchPool[BPMN Holds Jobs]
        VolumeCheck{Is Crucible <br> >85% Full?}
    end

    subgraph Layer 3: Ledger
        Mint[Mint Tokens for Scrap]
        Escrow[Lock Tokens for Exergy/Heat]
    end

    subgraph Layer 2: Twin (Telemetry)
        TempSensor[Type-K Thermocouple]
        VOCSensor[Exhaust PM2.5 / VOC Monitor]
    end

    %% Legacy Flow
    SPC -.->|Insurance & Zoning| FireMarshal
    FiatClient -->|Pays USD| SPC
    SPC -->|Translates to Casting Bounty| BatchPool

    %% Sovereign Flow
    Scrapper --> Mint
    Mint -->|Adds Raw Material| BatchPool
    Requester --> BatchPool
    
    BatchPool --> VolumeCheck
    VolumeCheck -->|No| Wait[Hold in Queue]
    VolumeCheck -->|Yes| AQICheck
    
    AQICheck -->|Clear| CredCheck
    AQICheck -->|Violation| Suspend[Suspend Operations]
    
    CredCheck -->|Approved| Escrow
    Escrow --> L1[Layer 1: Kiln Actuation]
    
    L1 --> TempSensor
    L1 --> VOCSensor
    VOCSensor -->|Spike Detected| Emergency[L2 Relay Cutoff]
    TempSensor -->|Phase Change Reached| Cast[Pour & Cool]
```

---

## 3. Oasis Engine Implementation Specification

In the C++ Oasis engine, this scenario pushes the thermodynamic cellular automata to its limits:

*   **Extreme Heat Diffusion:** The `ChunkManager` must handle temperatures exceeding $1000^\circ\text{C}$. If the `FOUNDRY_KILN` voxel reaches operating temp, the engine must calculate thermal bleed into adjacent voxels. If the adjacent voxels lack high `thermal_resistance` (e.g., a `WOOD_WALL` voxel), they must burst into `FIRE` voxels, penalizing the player for poor spatial design.
*   **Phase Change Logic:** A `SOLID_ALUMINUM` item entity inside the kiln voxel must track its specific heat capacity. Once it crosses $660^\circ\text{C}$, the engine transitions its state to `LIQUID_ALUMINUM`, unlocking the `POUR` action for the player.
*   **Batch Queuing:** The BPMN parser must successfully implement the `Parallel Gateway` and `Conditional Event` logic, holding workflow execution in a suspended state until the `target_mass_grams` variable is satisfied.

---

```json
{
  "@context": [
    "[https://schema.org/](https://schema.org/)",
    "[https://collective.network/ontology/v1/](https://collective.network/ontology/v1/)"
  ],
  "@type": "CastingBounty",
  "identifier": "urn:uuid:f0e1d2c3-b4a5-6789-0123-456789abcdef",
  "issuerDid": "did:mesh:node04:engineer_tom",
  "metallurgyProfile": {
    "targetAlloy": "Aluminum_A356",
    "massGrams": 450,
    "moldType": "Petrobond_GreenSand",
    "patternUri": "ipfs://QmPatternGeometryData..."
  },
  "processConstraints": {
    "requiresDegassing": true,
    "maxPorosityPercentage": 2.0
  },
  "batchingTolerance": {
    "maxWaitTimeHours": 336,
    "allowMixingWithFiatJobs": true
  },
  "escrowCapacity": {
    "maxValueTokens": "45.00"
  }
}
```

---

## 4. Acceptance Test Gates (Pass/Fail)

| Checkpoint | Validation Method | Pass Condition |
| :--- | :--- | :--- |
| **G1: Thermodynamic Batching** | Orchestrator Queue Test | A `CastingBounty` for 50g of aluminum remains suspended in the BPMN queue; the kiln voxel cannot be actuated until the queue exceeds the 2kg crucible threshold. |
| **G2: Thermal Bleed & Fire Code** | Voxel Adjacent Checks | Operating the kiln adjacent to flammable voxels triggers a catastrophic fire state; operating it within a designated `FIREBRICK` zone remains thermally stable. |
| **G3: L2 Emission Cutoff** | Simulated Sensor Spike | Injecting a high VOC payload into the simulated exhaust sensor instantly triggers a hardware relay cutoff, aborting the BPMN state machine to `FAILED_EMISSIONS`. |
| **G4: Competency Lockout** | Trust Ring Vouching | A player entity without the `Foundry_Safety_L3` credential is mathematically barred from triggering the `POUR` action on a liquid metal voxel. |
