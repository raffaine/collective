# Volume 1, Chapter 6: 6-Expert Multi-Disciplinary Critique Guide
**Target File:** `doc/book_collective/chapters/06_layer_5_governance.tex`

## Objective
Stress-test Chapter 6 ("Layer 5: Governance") from six directions. Layer 5 is the only **meta-recursive** layer in the Stack. It governs three things at once:
1. **The layer below it:** it ratifies, migrates, and drains Layer 4 workflows, and sets the policies the Orchestrator enforces.
2. **Its own domain:** identity (the Web of Trust), Reputational Wealth, delegation, councils, and policy.
3. **Itself:** the rules by which governance rules are made, amended, and challenged.

A flaw here spreads into every other layer, and a flaw in self-amendment can make all the other flaws permanent.

## Shared Context (all experts)
- **Strict Adjacency:** Layer 5 interfaces only with Layer 4 (below) and Layer 6 (above). It sets policy and legitimacy but never executes. It never touches the Ledger (Layer 3) or hardware directly; physical facts reach it only through Layer 4.
- **Settled in Chapters 3–5 (do not re-argue; check that Ch6 is consistent with these):**
  - Layer 5 owns identity, skill attestations, and personal capacity. Layer 4 is structurally blind to identity, so it matches no one and surveils no one.
  - Task intents go L4 → L5 (eligibility via the Web of Trust) → L6 (rendered in human language). Alerts follow the same path, with L5 deciding who must be told.
  - Completed work earns **Reputational Wealth**: local to its Node, non-transferable, and with influence that radiates outward. Exergy cannot be hoarded because of demurrage (Ch4); there are no bounties and no minting by people.
  - Layer 5 ratifies: **ecological floors**, the **priority order** under contention, the **unconditional survival baseline**, **emergency playbooks** (with post-hoc review), workflow **migrations and drains**, and the **human capacity ceiling**, which is built from consensual self-reports under Qualitative Friction.
  - Floor-vs-floor conflicts (e.g., ecology vs. human survival) are escalated by L4 to L5 for **human** decision.
  - New workflows are simulated in Oasis, deliberated, and ratified, then must also pass the L4 compiler's safety checks.
- **Ch1 commitments:** non-adversarial treatment of non-stewards (never NPCs), and privacy.
- **Tone:** this is a guide, not a narrative. Critique substance and precision. Narrative use should be to provide additional understanding to complex interactions. Leveraging reference material can be used but not for mere brevity in presentation but as available premises taken.

## The Council

### 1. Political Scientist / Commons Scholar
Polycentricity and Ostrom's design principles for long-enduring commons: clear boundaries, congruence with local conditions, collective-choice arrangements, monitoring, graduated sanctions, conflict-resolution mechanisms, recognition of the right to organize, and nested enterprises. Look for routes to capture by a faction, an oligarchy of the most active, or delegation chains. Check how councils nest across Nodes and bioregions.

### 2. Legal & Constitutional Expert
Legitimacy and self-amendment. What is the community's "rule of recognition"? Which rules are entrenched, and with what supermajority or cooling-off period can they change? How are decisions challenged or appealed, and how are minorities protected from majorities? What due process applies to sanctions or loss of attestations? How does governance interface with legacy law (Volume/Layer 7 territory; flag but don't redesign)? Specifically address the bootstrapping and self-amendment paradoxes.

### 3. Cryptographer / Identity Specialist
The Web of Trust, Verifiable Credentials, Sybil resistance (consistent with Ch4's hardware root of trust and physical web of trust), key loss and recovery, coercion-resistant and private voting, and how reputation decays. Check whether the "automated vote tallying" and "cryptographic scopes" are technically sound and whether they leak more than they should.

### 4. Psychologist / Conflict Mediator (the individual)
Deliberation fatigue, the tyranny of the most vocal, power dynamics, shame and exclusion, how decisions are explained to the people they affect, restorative rather than punitive conflict resolution, and protecting individuals from social coercion disguised as consensus. Governance must not become a second job that only the energetic can afford.

### 5. Ethical & Cultural Guide (the collective)
The **non-individual** dimension of human nature, complementing Expert 4. Look at shared meaning, ritual, story, and tradition as the substance that legitimacy actually rests on, alongside procedure. Cover intergenerational duty to ancestors and descendants, collective memory, and the sacred or non-quantifiable (what a community refuses to put a number on). Check for cultural plurality across Nodes, so that one protocol doesn't impose one culture or one style of consensus. Check for kinship and affection-based bonds, and for the relationship with non-stewards and neighbouring legacy communities. Ensure Layer 5 is not reduced to vote-counting machinery: a community governs through who it is as well as what it decides.

### 6. Sovereign Stack Architect (Adjacency & Consistency Enforcer)
Flag every place where Layer 5 executes, touches the Ledger or hardware, or bypasses Layer 4 or Layer 6. Flag responsibilities assigned to Layer 5 in Chapters 3–5 (listed above) that Chapter 6 omits or contradicts. Check that the meta-recursion is architecturally coherent: how a change to governance rules propagates, and what Layer 4 does with in-flight workflows when policy changes. Ensure that the reality of storage and computation for the layer's needs are established.

## Lead Author Mandate
- Synthesize all six critiques into one coherent chapter. **Preserve depth**: keep valuable existing material and expand where the council finds gaps.
- **Guide register, not narrative.** Clear `\subsection`/`\subsubsection` structure, precise definitions and mechanisms, and prose only where explanation is needed. No rhetorical flourishes, and no bold or numbered pseudo-headings.
- Make the three levels of meta-recursion explicit (governing L4, governing its own domain, governing itself), each with its own mechanisms and safeguards.
- Cite real, verifiable sources where claims rest on prior work (e.g., Ostrom is already in `collective.bib`; add others such as Hart, Elster, or Mansbridge only if accurate).
- Compile with `bibtex` + `pdflatex` (×2) using `BypassSandbox: true`. Verify **0 LaTeX errors and 0 undefined citations**. Commit **only** the relevant files. Write `vol1_ch6_final_status.md`, including any design decisions the user should review.
