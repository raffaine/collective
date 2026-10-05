# Orchestrator Architecture Spec (Layer 4)

## 1. The Decentralized Traffic Cop
Layer 4 is the active consciousness of the Node. It does not store physical reality (Layer 1b) nor does it store economic state (Layer 3). It is purely a **State Machine Router**. 

When a human or an automated sensor emits a Semantic Intent (Layer 6), the Orchestrator picks it up, validates it, and routes it through a standardized workflow graph.

## 2. Technical Mandate: BPMN 2.0 State Machines
To prevent spaghetti code and hardcoded logic, all routing must be represented as **BPMN (Business Process Model and Notation)** XML or JSON-equivalent graphs. 
The Orchestrator engine (whether written in C++, Python, or Go) acts as a headless BPMN runner. 

Why BPMN?
- It provides a standardized visual graph (Start Events, Exclusive Gateways, Tasks, End Events).
- It allows Stewards to visually audit the "rules" of the Node without knowing how to read C++.
- It makes the system completely declarative.

## 3. The 7-Layer Workflow Sequence
When Layer 4 processes an Intent (e.g., `FabricationIntent`), it executes a strict sequence:
1.  **Ingest (L6):** Parse the JSON-LD Intent.
2.  **Gate (L5):** Query the Trust Ring. Does this DID have the `SkillCredential` to operate the 3D printer? If not, route to a `RejectionEvent`.
3.  **Validate Physics (L1b & L2):** Query the Digital Twin and the C++ engine. Is the printer online? Is there enough thermal Exergy in the battery bank?
4.  **Escrow (L3):** Lock the required `ValueTokens` via the CRDT ledger.
5.  **Actuate (L2):** Emit an `ActuatorCommand` to start the physical printer.
6.  **Settle (L3):** Upon receiving a success `TelemetryStream` from the printer, burn the tokens and finalize the thermodynamic ledger.

## 4. Directive for Orchestrator Agents
Agents building this layer must avoid writing domain-specific business logic (e.g., `if (intent == "Fabrication")`). Instead, they must build a generic execution loop that reads a `.bpmn` graph file, steps through the nodes, and calls the appropriate interfaces for Layers 2, 3, and 5.
