# Ledger Architecture Spec (Layer 3)

## 1. The Core Philosophy: Thermodynamics over Finance
Layer 3 is the economic heart of the Node, but it is **not a fiat accounting system**. It does not track USD, Euros, or abstract debt. It is a strict thermodynamic ledger.
It tracks the delta between `mesh:Exergy` generated (e.g., Solar) and `mesh:Exergy` consumed (e.g., Compute, Fabrication, Heat). 

The currency of the mesh (`mesh:ValueToken`) is a 1:1 reflection of physically verifiable surplus exergy. If the Node does not produce physical energy or biomass, no tokens can be minted. **This is the Thermodynamic Anti-Cheat.**

## 2. Networking Architecture (Local-First CRDTs)
The Collective must survive legacy internet failures (Scenario Upsilon). Therefore, Layer 3 **cannot** rely on centralized cloud databases (e.g., PostgreSQL on AWS) or global blockchain consensus (which is too slow and energy-intensive for local coordination).

**Technical Mandate:** 
- The state must be maintained via **Conflict-free Replicated Data Types (CRDTs)**.
- Synchronization between peers must occur via a decentralized P2P gossip protocol (e.g., libp2p Gossipsub).
- The state is "Local-First." Devices can go entirely offline, continue to log thermodynamic actions locally, and seamlessly merge their state branches with the Trust Ring when they reconnect via LoRaWAN or mesh Wi-Fi.

## 3. Escrow and Dispute (Scenario Psi)
Layer 3 handles the mathematical locking of resources. When an Orchestrator (Layer 4) begins a task, or when a Mediation Intent is filed, Layer 3 utilizes a `CRDTEscrowLock`. Because the ledger is a DAG (Directed Acyclic Graph) of CRDTs, this lock prevents double-spending of physical energy while still allowing offline nodes to operate their unlocked resources.

## 4. Directive for Network Agents
When developing the networking stack, agents must utilize existing Rust/C++ CRDT libraries (like Automerge or Yjs ports) rather than inventing a new consensus algorithm. The integration must be tightly bound to the `Voxel` memory payload defined in Layer 1b, ensuring that a state sync over the network is simply a delta-patch of the voxel array.
