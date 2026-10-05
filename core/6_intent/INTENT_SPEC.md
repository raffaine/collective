# Semantic Intent Architecture Spec (Layer 6)

## 1. The Expression of Sovereign Will
Layer 6 is the human-computer interface boundary. The Node does not respond to button clicks or REST API `POST` requests. It responds exclusively to **Semantic Intents**. 

An Intent is a formalized, cryptographically signed declaration of a human's (or an automated L2 sensor's) desire to alter the state of the Node. It is the bridge between human psychology and the Orchestrator's (Layer 4) state machine.

## 2. Anatomy of an Intent
Every Intent must adhere to the W3C JSON-LD schemas defined in `ontology_intents.jsonld` and must contain the following cryptographic structure:

1.  **`issuerDid`:** The Decentralized Identifier of the citizen making the request.
2.  **`intentType`:** The class of action requested (e.g., `mesh:FabricationIntent`, `mesh:RestIntent`, `mesh:TriageIntent`).
3.  **`payload`:** The context-specific data. (e.g., for a FabricationIntent, this contains the IPFS hash of the 3D model and the required material).
4.  **`cryptographicSignature`:** The entire JSON payload must be signed by the private key corresponding to the `issuerDid`. This guarantees non-repudiation.

## 3. The Decoupled UI (Dumb Terminals)
The WebGL/React frontend (`ui/` folder) contains no business logic. It is merely an "Intent Builder." 
The UI's only job is to provide a friendly interface for a citizen to construct a JSON-LD Intent, sign it locally in their browser using their private key, and drop it into the Orchestrator's `JobQueue` (mempool). 

Because the UI is fully decoupled, citizens can bypass it entirely and submit Intents via command-line scripts or automated home-assistant daemons.

## 4. Intent Lifecycles
When an Intent is broadcast, it follows a strict lifecycle:
- **`PENDING`:** Dropped into the mempool.
- **`REJECTED`:** The Orchestrator (Layer 4) checked Layer 5 and found the signature invalid, or the user lacked the required `SkillCredential`.
- **`ESCROWED`:** The Orchestrator verified physics (Layer 1b) and locked the necessary ValueTokens (Layer 3).
- **`FULFILLED`:** The physical actuation (Layer 2) succeeded, and the ledger settled.

## 5. Directive for Scribe Agents
Agents building this layer must ensure that every new Scenario mapped in the documentation has a corresponding JSON-LD schema defined here. If a user wants to request an action that does not exist in the schema, the Orchestrator will blindly reject it. The schema is the absolute vocabulary of the Node.
