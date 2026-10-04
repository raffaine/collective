# Knowledge Representation Spec (Layer 1a)

## 1. Directive for Semantic Scribe Agents
**Objective:** Define the complete cryptographic and semantic reality of the Node. Layer 1a is the sole source of truth for "What exists" and "What actions can be taken."
**Constraints:**
- All definitions must be valid JSON-LD 1.1.
- Schemas must be modular and heavily leverage existing W3C namespaces (`schema.org`, `w3id.org/security/v2`) where possible, avoiding reinventing standard primitives.
- Validation must eventually utilize SHACL (Shapes Constraint Language) so Layer 4 Orchestrator can definitively accept or reject an intent before passing it to Layer 1b.

## 2. Core Domains of Knowledge

The knowledge base is split into three primary ontologies to prevent a monolith:
1.  **Core / Physical (`ontology_core.jsonld`):** The baseline physical reality. Voxels, Exergy, Hardware, Citizens.
2.  **Intents (`ontology_intents.jsonld`):** The semantic verbs of the system. A formalized request to alter the physical or social state (e.g., Fabrication, Triage, Mediation).
3.  **Trust & Identity (`ontology_trust.jsonld`):** The cryptographic relationships. DIDs (Decentralized Identifiers), Verifiable Credentials (VCs), and Graph Edges (vouches and slashes).

## 3. The SHACL Validation Pipeline
Before a `GenesisIntent` or `FabricationIntent` is routed to the Engine (1b) or Orchestrator (4), it must be mathematically validated against a SHACL shape. 
*   **Example Rule:** A `MediationIntent` (Scenario Psi) *must* include a `targetDid` and a `severityLevel`. If it lacks these, the schema parser rejects it at the edge.

## 4. Binding to Layer 2 (Digital Twin)
The knowledge representations must contain fields to anchor them to physical telemetry. A `3D_Printer` defined in 1a must have a `telemetryCapable` flag that signals Layer 2 to listen for an MQTT data stream mapping to that specific physical object.
