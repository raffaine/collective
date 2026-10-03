# Schemas: Semantic Intent and Cryptographic Trust

Layer 6 (Semantic) and Layer 5 (Policy) operate exclusively on structured, machine-readable data. In The Collective, human intent cannot be a vague text string; it must be compiled into an actionable graph that the Layer 4 Orchestrator can parse, execute, and verify.

We utilize **JSON-LD** (JavaScript Object Notation for Linked Data) coupled with W3C standards (like DIDs and Verifiable Credentials). This ontology bridges the gap between subjective human relationships and deterministic state machines, completely eliminating the need for centralized relational databases.

---

## 1. Knowledge Artifacts (The Intent Protocol)

A Knowledge Artifact is a formalized broadcast of intent on the Gossipsub mesh. It represents a bounty, a resource offering, or a civic proposal. 

By standardizing these requests, the Oasis engine and physical nodes can automatically calculate thermodynamic routing without human middlemen.

**Example: A Fabrication Commons Bounty (Scenario Alpha)**
When a citizen needs a physical part, they broadcast this payload. The Layer 4 orchestrator parses the `material` and `thermodynamicReward` to match it with a local idle 3D printer.

```json
{
  "@context": [
    "[https://schema.org/](https://schema.org/)", 
    "[https://collective.network/ontology/v1/](https://collective.network/ontology/v1/)"
  ],
  "@type": "FabricationBounty",
  "identifier": "bounty-genesis-8492",
  "name": "Print 5x Interlocking Gears for Aquaponics Valve",
  "issuedBy": "did:mesh:node04:steward77",
  "requirements": {
    "@type": "MaterialSpec",
    "material": "PETG",
    "color": "UV-Resistant Black",
    "weightGrams": 450,
    "modelPayload": "ipfs://QmYwAPJzv5CZsnA625s3Xf2..."
  },
  "thermodynamicReward": {
    "@type": "ValueToken",
    "amount": "12.5",
    "escrowStatus": "Locked"
  },
  "deadline": "2026-10-05T18:00:00Z"
}
```
## 2. Verifiable Credentials (The Competency Protocol)

To replace legacy university degrees and centralized licensing boards (Scenario Kappa), The Collective uses the W3C Verifiable Credentials (VC) Data Model.

A skill is not a self-declared string on a profile; it is a cryptographic attestation signed by a vetted peer who physically inspected the thermodynamic output of the student's labor.

**Example: The Apprenticeship Attestation**
This credential proves a 14-year-old successfully engineered a local DC grid. It is stored in their sovereign wallet and can be verified offline by any node's public key registry.

```json
{
  "@context": [
    "[https://www.w3.org/2018/credentials/v1](https://www.w3.org/2018/credentials/v1)",
    "[https://collective.network/ontology/v1/](https://collective.network/ontology/v1/)"
  ],
  "id": "urn:uuid:3978344f-8596-4c3a-a978-8fcaba3903c5",
  "type": ["VerifiableCredential", "SkillAttestation"],
  "issuer": "did:mesh:node12:mentor01",
  "issuanceDate": "2026-10-01T12:00:00Z",
  "credentialSubject": {
    "id": "did:mesh:node04:apprentice02",
    "skill": {
      "type": "EngineeringCompetency",
      "domain": "Electrical",
      "name": "Low-Voltage DC Solar Wiring",
      "level": "L1-Practical",
      "proofOfWork": "ipfs://QmProofOfActuation..."
    }
  },
  "proof": {
    "type": "Ed25519Signature2020",
    "created": "2026-10-01T12:05:00Z",
    "verificationMethod": "did:mesh:node12:mentor01#keys-1",
    "proofValue": "z58Dfd9j...[cryptographic signature]...8xK"
  }
}
```

## 3. Trust Rings (The Governance Protocol)

In Scenarios Delta (Childcare Pods) and Omicron (Kinship Protocols), citizens pool resources and coordinate schedules. A Trust Ring is a JSON-LD definition of a polycentric governance boundary.

It defines exactly which DIDs (Decentralized Identifiers) are in the pod, what resources they share, and the mathematical consensus threshold required to execute a Layer 4 BPMN workflow (like unlocking the communal tool library).

**Example: A Mutual Aid Childcare Pod**

```json
{
  "@context": "[https://collective.network/ontology/v1/](https://collective.network/ontology/v1/)",
  "@type": "TrustRing",
  "identifier": "ring-childcare-alpha",
  "name": "Duvall Mutual Aid Childcare Pod Alpha",
  "governanceModel": "MultiSignature",
  "consensusThreshold": "3-of-5",
  "members": [
    {
      "id": "did:mesh:node01:parentA",
      "role": "Steward",
      "requiredAttestations": ["Pediatric_CPR", "Background_Vouch"]
    },
    {
      "id": "did:mesh:node02:parentB",
      "role": "Steward",
      "requiredAttestations": ["Pediatric_CPR", "Background_Vouch"]
    }
  ],
  "sharedResources": [
    "urn:mesh:resource:physical:node02_backyard",
    "urn:mesh:resource:ledger:pod_time_bank"
  ]
}
```

## Integration with Layer 4

These JSON-LD schemas are the absolute boundary of Layer 6 and Layer 5. Once a Trust Ring reaches consensus on a Knowledge Artifact, the payload is serialized and injected as the variables state into the Layer 4 WASM BPMN executor. The Orchestrator strips away the semantics, looks only at the raw data, and begins chronologically actuating the edge devices at Layer 2.
