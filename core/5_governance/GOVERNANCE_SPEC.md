# Governance & Trust Architecture Spec (Layer 5)

## 1. The Decentralized Immune System
Layer 5 is the mathematical immune system of the Node. It does not dictate what people *want* to do (Layer 6) or how it physically happens (Layer 1b). It dictates **who is cryptographically permitted** to do it.

The Collective relies on high-friction, high-trust proximity rather than global anonymity. The Trust Ring is a mathematically weighted social graph.

## 2. Technical Mandates: DIDs & Verifiable Credentials
Layer 5 must strictly adhere to W3C Decentralized Identifiers (DIDs) and Verifiable Credentials (VCs).
- **Identity:** Every Steward is represented by a `did:key` or `did:mesh`. There are no centralized user tables or passwords.
- **Skills (Scenario Kappa):** Competency is not self-declared. It is issued as a cryptographic VC. If Alice wants to operate the forge (Layer 1b), Layer 4 checks Layer 5 to see if she holds a `Foundry_Apprentice` VC signed by a recognized Master.

## 3. Graph Theory & Edge Weights
Trust is not binary; it decays over distance. 
- **Vouching (Omicron):** When Bob vouches for Charlie, he creates a directional `TrustGraphEdge`. If Charlie acts maliciously and is slashed, Bob's reputation is also mathematically penalized.
- **Social Slashing (Rho):** If a Steward attacks the network or violates the Core Agreements, a consensus threshold of edges flips to negative (-1.0). The graph automatically severs them. The Orchestrator (Layer 4) is mathematically incapable of routing resources to a slashed DID.

## 4. Zero-Knowledge Privacy & Safe Harbor
Total transparency is a surveillance state. Layer 5 must implement Zero-Knowledge (ZK) Proofs for intimate data.
- **The Pharmacopeia (Chi):** A medic can use a ZK-proof to verify a patient is not allergic to a compound without decrypting their entire medical ledger.
- **Safe Harbor Severance (Omicron):** In cases of abuse, a victim can invoke a `SafeHarborSeverance`. Layer 5 enforces this as a cryptographic blind spot. The Orchestrator (Layer 4) will never route the abuser and the victim to the same physical location (e.g., Scenario Epsilon Coworking), and the abuser is mathematically prevented from seeing the victim's telemetry.

## 5. Directive for Governance Agents
Agents implementing this layer must not build standard Web2 RBAC (Role-Based Access Control). Do not use simple boolean permissions. All permissions must be calculated dynamically based on graph distance (e.g., Dijkstra's algorithm running across the Trust Ring edges).
