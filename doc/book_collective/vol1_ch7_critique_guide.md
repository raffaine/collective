# Volume 1, Chapter 7: 7-Expert Multi-Disciplinary Critique Guide
**Target File:** `doc/book_collective/chapters/07_layer_6_semantic.tex`

## Objective
Stress-test Chapter 7 ("Layer 6: The Semantic Layer"). Layer 6 is where people meet the Stack. It is the only layer where a machine (the local LLM) stands between humans and their own governance, so a mistranslation here can become law and a biased summary can steer a community. The chapter is currently the thinnest in the volume (~7.5 KB), while Chapters 5–6 route a great deal through it.

## Shared Context (all experts)
- **Strict Adjacency:** Layer 6 interfaces only with Layer 5 (below) and Layer 7 (above). It never reads the Ledger (L3), never talks to the Orchestrator (L4), and never touches hardware. Everything it knows about the physical world arrives through Layer 5, which receives it from Layer 4.
- **Duties assigned to Layer 6 by Chapters 5–6 (check coverage):**
  - *From L5 to people:* task offers to eligible Stewards; alerts, with an audience already decided by L5; explanations of decisions to the people they affect; proposals and rationale records for deliberation.
  - *From people to L5:* plain-language proposals (BPMN is not a precondition for participation; Layer 6 tooling helps translate intent into draft workflows); ballots; delegations; challenges and appeals; capacity and care-leave self-reports; testimony from hearings, including from non-members; authoring rationale records.
  - *Privacy commitments (Ch6):* pairwise per-council identifiers, selective disclosure, secret ballots, private delegation edges, private self-reports and care leave, no record of task refusals.
  - *Ch6 culture mechanisms that surface here:* pluggable decision procedures, in-person assemblies, Future Generations panels, the Protocol of Refusal, the right to informality, hearings and written responses for non-members.
  - Collisions with legacy law are surfaced through Layer 6 to Layer 7.
  - The Oasis governance UI is the same as the real Layer 6 UI.
- **Physical computing constraint:** LLMs on salvaged edge hardware face real memory and VRAM limits, latency, and an energy cost that, per Ch4/Ch5, must be accounted for like any other computation.
- **Ch1 commitments:** non-adversarial treatment of non-stewards (never NPCs), and privacy.
- **Tone:** this is a guide, not a narrative. Critique substance and precision.

## The Council
1. **Edge AI Systems Engineer:** feasibility of local LLMs on salvaged hardware (model size, quantization, VRAM, latency, energy per inference); model updates and provenance; retrieval-grounding vs. free generation; when *not* to use an LLM (e.g., safety-critical alerts should use deterministic, tested templates).
2. **Computational Linguist (Translation Fidelity):** the round trip from human intent to a structured, signed L5 payload and back. Ambiguity, confirmation by read-back before signing, multilingualism and dialects, ontology design, and keeping the generated and human-authored parts of a text distinguishable.
3. **HCI & Accessibility Designer:** calm technology; no dark patterns or engagement optimization; attention as a protected resource; non-screen and offline modes (voice, paper, in person); access for elderly, blind, low-literacy, and neurodivergent members; the shared Oasis UI.
4. **Deliberation & Epistemic Integrity Specialist:** the LLM as moderator or summarizer can steer deliberation. Preserve dissent in summaries; guard against persuasion and nudging and against the model becoming an epistemic authority; provide provenance and citations for every generated explanation; check the Agora design.
5. **Adversarial ML & Security Specialist:** prompt injection, especially from legacy content arriving via Layer 7; model and data poisoning; spoofed system messages; privacy of conversations and logs; model supply chain.
6. **Ethical & Cultural Guide:** whose language and metaphors the model encodes (legacy corpora carry legacy assumptions); oral tradition and story; whether the Protocol of Refusal extends to conversations a community never wants a machine to process; how non-stewards experience Layer 6.
7. **Sovereign Stack Architect:** flag adjacency violations (L6 touching L3/L4 or raw lower-layer data, or bypassing L5/L7); check coverage of every Layer 6 duty listed above; check consistency with Ch5–6 (privacy model, routing, terminology); check the Layer 6 → Layer 7 direction.

## Lead Author Mandate
- Synthesize all seven critiques. **Preserve depth and expand.** This chapter must carry its share of the volume.
- **Guide register:** clear `\subsection`/`\subsubsection` structure, tables where they aid reference, precise mechanisms, no narrative flourishes, no bold or numbered pseudo-headings.
- Cite only real, verifiable sources.
- Compile with `bibtex` + `pdflatex` (×2) using `BypassSandbox: true`; verify **0 errors and 0 undefined citations**; commit **only** the relevant files; write `vol1_ch7_final_status.md` listing design decisions for review.
