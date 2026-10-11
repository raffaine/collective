# Sovereign Stack Agent-as-Judge Evaluation Rubric & Specification

## Purpose
This document defines the authoritative 6-criterion evaluation rubric and prompting specification for the **Independent Sovereign Stack Agent-as-Judge**. It provides invariant rules, scoring mechanics, and the JSON output schema used to evaluate all 24 scenario markdown files in `docs/scenarios/`.

---

## 1. Evaluation Invariants & Rubric

| Criterion ID | Dimension | Mandatory Invariants to Verify in Scenario |
| :--- | :--- | :--- |
| **CRIT-1_BCC** | Bilateral Clearing Channels (Layer 3) | 1. Explicitly documents Layer 3 pairwise Bilateral Clearing Channels ($BCC$).<br>2. Cites or formulates channel zero-sum conservation: $B_{u \to v}(t) + B_{v \to u}(t) = 0$.<br>3. Defines mutual-aid asymmetric credit window $[-C_{u \to v}, +C_{v \to u}]$.<br>4. Rejects planetary/global ledgers, universal tokens, and AMMs.<br>5. Specifies non-monetary settlement mechanisms: reciprocal physical delivery, stewardship labor, or Commons Jubilee. |
| **CRIT-2_HTLC** | Multi-Hop Atomic Routing | 1. Documents multi-hop resource routing ($A \to B \to C$) using chained Hashed Timelock Contracts (HTLCs): $\langle H(S), \Delta t, Q, u, v \rangle$.<br>2. Specifies sensor-verified two-phase cryptographic delivery receipts: $\sigma_{\mathrm{delivery}} = \mathrm{Sign}_{K_{\mathrm{receiver}}}(\mathrm{Hash}(\dots))$.<br>3. Proves atomic execution: revelation of preimage $S$ triggers cascade clearing; timeout expiry triggers deterministic rollback. |
| **CRIT-3_RINGS** | 7-Tier Concentric Trust Rings | 1. Explicitly defines or structures interactions across the 7 concentric rings:<br>&nbsp;&nbsp;&nbsp;&nbsp;• $R_0$: Hearth (Domestic sanctuary & intimate care)<br>&nbsp;&nbsp;&nbsp;&nbsp;• $R_1$: Settlement (Immediate intentional neighborhood / hamlet)<br>&nbsp;&nbsp;&nbsp;&nbsp;• $R_2$: Commons (Shared local resource pool)<br>&nbsp;&nbsp;&nbsp;&nbsp;• $R_3$: Watershed (Ecological hydrology & local valley)<br>&nbsp;&nbsp;&nbsp;&nbsp;• $R_4$: Bioregion (Cooperative regional federations)<br>&nbsp;&nbsp;&nbsp;&nbsp;• $R_5$: Continental (Macro-scale intertie exchanges)<br>&nbsp;&nbsp;&nbsp;&nbsp;• $R_6$: Ecumene (Planetary biosphere & human species)<br>2. Implements dynamic outward cascading request matching ($R_0 \to R_1 \dots \to R_6$).<br>3. Incorporates physical and social friction attenuation factors. |
| **CRIT-4_BPVP** | Bioregional Path-Vector Protocol (BPVP) | 1. Formulates Layer 5 Capability Advertisements $\mathcal{A} = \langle \mathrm{Resource}, Q_{\mathrm{avail}}, \mathcal{P}, \tau_{\mathrm{valid}}, \mathrm{Cert} \rangle$.<br>2. Enforces export surplus floor: $Q_{\mathrm{avail}} = \max(0, Q_{\mathrm{inventory}} - F_{\mathrm{eco}} - B_{\mathrm{baseline}}) \cdot \mathbf{1}_{\mathrm{export}}$.<br>3. Implements path-vector $\mathcal{P}$ with loop prevention (rejects routing if local Node $u \in \mathcal{P}$).<br>4. Accounts for thermodynamic path attenuation $\mathcal{C}(\mathcal{P})$ and effective delivered exergy $\mathcal{E}_{\mathrm{eff}}(\mathcal{P})$ with constitutional efficiency floor $\eta_{\mathrm{route}} \ge \eta_{\mathrm{min}}$. |
| **CRIT-5_GOV** | Strict Layer 5 Polycentric Governance | 1. Enforces autopoietic Cellular Autonomy: local node retains absolute sovereignty over internal resource allocation ("the parts" hold precedence over "the whole").<br>2. Defines Protocol of Refusal: sacred, intimate, and non-commercial assets are firewalled from export advertisements.<br>3. Supports Autonomous Transit Veto (`No-Transit` attribute) for biosecurity or ecological defense.<br>4. Formalizes non-monetary Ricardian Treaties ($\mathrm{Treaty\_CID} = \mathrm{SHA256}(\dots)$) through boundary sockets (e.g. `/run/collective/ipc/l4_l5.sock`).<br>5. Guarantees Unconditional Right of Exit without forfeiture of bodily or domestic survival assets. |
| **CRIT-6_CONS** | 5-Perspective Council Consensus | 1. Contains dedicated section: `## Architectural Council Ratification & 5-Perspective Synthesis`.<br>2. Verifies distinct, substantive contributions from all 5 personas:<br>&nbsp;&nbsp;&nbsp;&nbsp;• **Game Designer** (DOD ECS mechanics, DF psychology, Sim-style indirect control)<br>&nbsp;&nbsp;&nbsp;&nbsp;• **Game Engineer** (C++20 harness, Flecs components, 10 Hz determinism)<br>&nbsp;&nbsp;&nbsp;&nbsp;• **Sovereign Stack Specialist** (7-layer SoC, zero layer skipping, UHAI IPC)<br>&nbsp;&nbsp;&nbsp;&nbsp;• **Solarpunk Thinker** (Bioregional regeneration, closed loops, convivial limits)<br>&nbsp;&nbsp;&nbsp;&nbsp;• **Scenario Specialist** (Domain-specific technical mechanics)<br>3. Contains explicit declaration confirming unanimous consensus (e.g. "ratified by unanimous consensus"). |

---

## 2. Scoring & Verdict Mechanics

1. **Criterion Level:**
   - Every individual criterion is strictly binary: `PASS` or `FAIL`.
   - To achieve `PASS`, the scenario text must contain explicit, verifiable statements addressing all mandatory invariants for that criterion.
   - For every `PASS`, the evaluation must cite exact line numbers and provide verbatim quotes from the target file.
   - If any required invariant is missing, vague, or contradicted, the criterion is marked `FAIL` with a clear explanation of what is absent.

2. **Overall Verdict:**
   - A scenario receives an overall `verdict: "PASS"` **if and only if** all 6 criteria receive `status: "PASS"` (score: `6/6`).
   - If even one criterion fails, the overall verdict is `verdict: "FAIL"`.

---

## 3. Structured JSON Output Schema

When the Agent-as-Judge evaluates a scenario, it must produce valid JSON matching the following schema:

```json
{
  "$schema": "https://json-schema.org/draft/2020-12/schema",
  "title": "ScenarioJudgeVerdict",
  "type": "object",
  "required": [
    "scenario_file",
    "scenario_id",
    "verdict",
    "score",
    "criteria",
    "identified_violations"
  ],
  "properties": {
    "scenario_file": { "type": "string" },
    "scenario_id": { "type": "string" },
    "verdict": { "type": "string", "enum": ["PASS", "FAIL"] },
    "score": { "type": "string", "pattern": "^[0-6]/6$" },
    "criteria": {
      "type": "object",
      "required": [
        "CRIT-1_BCC",
        "CRIT-2_HTLC",
        "CRIT-3_RINGS",
        "CRIT-4_BPVP",
        "CRIT-5_GOV",
        "CRIT-6_CONS"
      ],
      "properties": {
        "CRIT-1_BCC": {
          "type": "object",
          "required": ["status", "line_numbers", "quote", "justification"],
          "properties": {
            "status": { "type": "string", "enum": ["PASS", "FAIL"] },
            "line_numbers": { "type": "array", "items": { "type": "integer" } },
            "quote": { "type": "string" },
            "justification": { "type": "string" }
          }
        },
        "CRIT-2_HTLC": {
          "type": "object",
          "required": ["status", "line_numbers", "quote", "justification"],
          "properties": {
            "status": { "type": "string", "enum": ["PASS", "FAIL"] },
            "line_numbers": { "type": "array", "items": { "type": "integer" } },
            "quote": { "type": "string" },
            "justification": { "type": "string" }
          }
        },
        "CRIT-3_RINGS": {
          "type": "object",
          "required": ["status", "line_numbers", "quote", "justification"],
          "properties": {
            "status": { "type": "string", "enum": ["PASS", "FAIL"] },
            "line_numbers": { "type": "array", "items": { "type": "integer" } },
            "quote": { "type": "string" },
            "justification": { "type": "string" }
          }
        },
        "CRIT-4_BPVP": {
          "type": "object",
          "required": ["status", "line_numbers", "quote", "justification"],
          "properties": {
            "status": { "type": "string", "enum": ["PASS", "FAIL"] },
            "line_numbers": { "type": "array", "items": { "type": "integer" } },
            "quote": { "type": "string" },
            "justification": { "type": "string" }
          }
        },
        "CRIT-5_GOV": {
          "type": "object",
          "required": ["status", "line_numbers", "quote", "justification"],
          "properties": {
            "status": { "type": "string", "enum": ["PASS", "FAIL"] },
            "line_numbers": { "type": "array", "items": { "type": "integer" } },
            "quote": { "type": "string" },
            "justification": { "type": "string" }
          }
        },
        "CRIT-6_CONS": {
          "type": "object",
          "required": ["status", "line_numbers", "quote", "justification"],
          "properties": {
            "status": { "type": "string", "enum": ["PASS", "FAIL"] },
            "line_numbers": { "type": "array", "items": { "type": "integer" } },
            "quote": { "type": "string" },
            "justification": { "type": "string" }
          }
        }
      }
    },
    "identified_violations": {
      "type": "array",
      "items": { "type": "string" }
    }
  }
}
```

---

## 4. Evaluation Prompt Template

When dispatching an Agent-as-Judge review, provide the following prompt:

```markdown
You are the Independent Sovereign Stack Agent-as-Judge.
Your mandate is to rigorously evaluate the assigned scenario file against Chapters 2–8 of The Sovereign Stack using the rubric defined in `verification/scenarios/agent_as_judge_rubric.md`.

Target Scenario: {scenario_file} ({scenario_id})

INSTRUCTIONS:
1. Thoroughly read the scenario file and evaluate each of the 6 criteria (CRIT-1 through CRIT-6).
2. Each criterion must be marked PASS or FAIL.
3. For every PASS, provide:
   - "line_numbers": list of exact line numbers where the invariants are satisfied.
   - "quote": verbatim excerpt from the file proving compliance.
   - "justification": concise explanation of how the excerpt satisfies all mandatory invariants.
4. For any FAIL, provide:
   - "line_numbers": empty list or line numbers of nearest deficient attempt.
   - "quote": empty string or excerpt showing the gap/contradiction.
   - "justification": precise statement of which invariants are missing or violated.
5. If any single criterion is FAIL, the overall "verdict" MUST be "FAIL". "verdict" can only be "PASS" if score is "6/6".
6. Output ONLY valid JSON conforming to the schema in `verification/scenarios/agent_as_judge_rubric.md`.
```
