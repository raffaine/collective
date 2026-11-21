# Scenario Kappa: Debt-Free Polymathic Guild Apprenticeship & Unlinkable Mastery (`SCN-KAPPA-APPRENTICE`)

* **Identifier:** `SCN-KAPPA-APPRENTICE`
* **Horizontal Wave:** Wave C (Care, Kinship, Apprenticeship & Decommodified Shelter)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Kappa governs **how people learn practical crafts, acquire hazardous operational competencies, mentor one another, and earn verifiable credentials (`T1` through `T4`) across every guild in The Collective**. While `Scenario Iota` cultivates open scientific inquiry and general foundational literacy, Scenario Kappa bridges knowledge into **embodied, psychomotor preparedness and safe physical stewardship**.

Its scope encompasses:
* **Debt-Free Polymathic Guild Apprenticeships:** Enabling any member to master woodworking, CNC machining (`Alpha`), clinical triage (`Chi`), induction metallurgy (`Theta`), agroforestry (`Eta`), or software engineering (`Zero`) with zero tuition debt and full $B_{\mathrm{baseline}}$ living support.
* **$R_0$-Private Mastery Tracking (`BKT`):** Using local integer Bayesian Knowledge Tracing so a learner's mistakes, retries, and pace remain $100\%$ private on their own device.
* **Hardware-Interlocked Supervised Practice & Unlinkable `BBS+` Tool Unlocks:** Pairing encrypted mentor proximity tethers during hands-on training with $< 50\,\mathrm{ms}$ zero-knowledge safety credentials that unlock hazardous machinery without tracking who used what tool when.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy education and credentialing market (Layer 7), skill acquisition is distorted by four structural pathologies:

1. **Predatory Student Debt & Credential Inflation:** Universities and trade schools charge tens of thousands of dollars in tuition—trapping young adults in decades of debt servitude before they can touch a lathe, clinical bench, or design workstation.
2. **Surveillance Proctoring & High-Stakes Exam Anxiety:** Legacy schools grade learners on curve-ranked, surveillance-proctored multiple-choice tests that punish early mistakes, induce chronic anxiety, and fail to measure whether a person can actually operate a saw or sterilize a bioreactor safely under real conditions.
3. **Siloed Career Hyper-Specialization:** Workers are forced into narrow, lifelong occupational boxes ("you are either a software engineer or a carpenter, never both"), stifling human polymathy and leaving communities brittle when specialists are unavailable.
4. **Bureaucratic Gatekeeping & Nepotism:** Guilds and licensing boards often restrict entry to protect artificial wage scarcity or favor insiders' relatives rather than welcoming eager learners.

---

## 3. Wide Feature Horizon

Scenario Kappa replaces tuition debt and surveillance testing with five convivial learning loops:

1. **Four-Tier Somatic Competency Progression (`T1` $\to$ `T4`):**
   * *`T1_Supervised_Novice`:* Completed interactive hazard rehearsal (`Oasis` Layer 1b sandbox); cleared to operate equipment in `ARMED_SUPERVISED` mode within a $\le 2000\,\mathrm{mm}$ ($2.0\,\mathrm{m}$) encrypted BLE proximity tether of a mentor.
   * *`T2_Autonomous_Practitioner`:* Reached $\ge 9500\,\mathrm{bp}$ ($95\%$) private Bayesian Knowledge Tracing (`BKT`) mastery and passed a physical bench checkout; cleared for solo unsupervised operation (`ZeroKnowledgeToolUnlockIntent`).
   * *`T3_Guild_Steward`:* Qualified to calibrate, repair, and sign off on safety-critical batch releases (`Gamma` retorts, `Chi` HPLC assays, `Theta` furnace pours).
   * *`T4_Master_Mentor` (and `T4_Emeritus_Sage` in `Scenario Phi`):* Qualified to tether novices, author `GuildPreparednessSyllabus` modules, and sit on Capstone panels.
2. **100% $R_0$-Private Integer Bayesian Knowledge Tracing (`BKT`):** As an apprentice practices in `Oasis` hazard simulations and mentored bench sessions, their personal workbook updates mastery probability $P(L_t)$ in 64-bit integer basis points (`0..10000 bp`). **All mistakes, failed quiz attempts, and learning curves stay strictly inside volatile/local $R_0$ storage**—the guild only ever sees the final signed readiness proof once $P(L_t) \ge 9500\,\mathrm{bp}$.
3. **Nepotism-Proof 3-of-3 Capstone Attestation:** Awarding a `T2+` credential requires a simultaneous hardware sign-off (`SociologicalCapstoneAttestation`) by a 3-person panel whose members share **zero $R_0$ kinship covenant (`Scenario Omicron`)** with the candidate.
4. **Unlinkable `BBS+` Zero-Knowledge Tool Interlocks:** Once certified, the steward's hardware card presents a blinded `BBS+` proof at any shop saw (`Alpha`), furnace (`Theta`), or clinical cabinet (`Chi`), unlocking the contactor in $< 50\,\mathrm{ms}$ without logging the steward's personal identity on a central surveillance database.
5. **Recency Decay & Gentle Refresher Drills (with `Sigma` Care-Leave Freeze):** High-hazard credentials (`T2/T3` on high-voltage switchgear, chainsaws, or trauma triage) include a natural recency timer; if a steward hasn't practiced a rare emergency skill in 6 months, their slate schedules a calm 15-minute `Oasis` or physical refresher drill (automatically frozen during `Scenario Sigma` Care Leave).

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`ApprenticeshipEnrollmentIntent`:** Pairing a learner with a `GuildPreparednessSyllabus` and matching them with available `T4` mentors.
* **`SociologicalCapstoneAttestation`:** The 3-of-3 household-independent panel co-sign certifying that an apprentice has demonstrated physical safety, craft tolerance, and ecological care at $\ge 9500\,\mathrm{bp}$.
* **`ZeroKnowledgeToolUnlockIntent`:** Presenting an unlinkable `BBS+` capability proof at a Layer 2 machine interlock to energize equipment in $< 50\,\mathrm{ms}$.
* **`MentorshipFellowshipOffer` (paired with `Scenario Sigma` / `Pi`):** Priority learning fellowships automatically published when a guild triggers an Anti-Martyrdom concentration alert ($> 4000\,\mathrm{bp}$) or a `MeshGrowthBeacon`.

### B. Semantic Representations & Deterministic Read-Backs
* **Declarative `GuildPreparednessSyllabus` Graphs:** Links `Scenario Iota` theoretical foundations, `Oasis` (`0x01`) interactive failure-mode simulations (e.g., kickback prevention, chemical spill neutralization), and mentored physical bench hours into a self-describing skill tree.
* **Mandatory Capstone & Safety Read-Back:** Before the 3-of-3 Capstone Panel co-signs a `SociologicalCapstoneAttestation`, their slates present a **Deterministic Read-Back** confirming: (1) local `BKT` mastery $\ge 9500\,\mathrm{bp}$, (2) zero `Omicron` household overlap between examiners and candidate, and (3) the exact machine classes and recency windows unlocked by the credential.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **$R_0$-Private Apprenticeship Workbook** | Learner's Personal Slate (`kappa:learner`, `r0:local_only`) | Tracks personal `BKT` progress across woodworking, cooking, coding, or botany; zero public grades, zero rankings, and zero telemetry of mistakes. |
| **Interactive Hazard Drill & Oasis Rehearsal Lens** | Workstation / VR / Tablet (`kappa:drill`) | Lets novices safely experience and recover from simulated lathe binds, autoclave over-pressures, or microgrid black-starts (`0x01`) before touching live hardware. |
| **Mentor-Apprentice BLE Tether Badge** | Shop-Floor Wearable / Pocket Token (`tool:unlock`) | Screenless hardware beacon: keeps a novice's machine in `ARMED_SUPERVISED` mode as long as the mentor is within $\le 2000\,\mathrm{mm}$; smoothly pauses the spindle if the mentor steps away. |
| **3-of-3 Capstone Co-Sign Slate** | Guild Bench (`gov:ratify`, `kappa:examiner`) | Displays the physical tolerance checklist for the candidate's test piece and accepts simultaneous NFC taps from three independent examiners. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Across every scale, Layer 5 governance in Scenario Kappa is anchored in the single unified voting weight formula scoped strictly to **Node Membership ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)\right)$$
Scenario Kappa is the canonical showcase of **Constitutional Pillar II (`collective:WorkCapabilityMatrix`, `D3`, `D7`)**: a newcomer enters on Day 0 with $R_{\mathrm{apprentice}}(0) = 0$ yet holds full civic equality $V_{\mathrm{apprentice}}(0) = B + 0 = 1$ ($10{,}000\,\mathrm{bp}$), earning $R_i(t) > 0$ through Tier-0 unsupervised starter tasks and Tier-1 mentor co-signed dual-key tasks (including building **Stage II Right-to-Repair Hybrid Retrofit Kits**).

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ learner/operator}$):**
   * *Context:* A solo homesteader learning a new craft (e.g., solar battery wiring, pressure canning, or timber joinery) using the node's offline syllabus and `Oasis` rehearsal sandbox.
   * *Governance Model:* **Self-paced safety rehearsal & remote/visiting checkout.** The solo steward uses the `R0-Private Workbook` and `Oasis` hazard drills to build mastery at their own pace; for high-hazard equipment unlocks, they can complete their Capstone checkout during a visiting `Scenario Tau` Pathway 3 artisan round or at a neighboring node's weekend guild bee.
2. **Single / Core Mentor Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ apprentices/borrowers}$):**
   * *Context:* One master carpenter, cook, or machinist mentoring $3\text{–}10$ neighborhood learners and gating access to a shared garage shop ($N_p > 1$).
   * *Governance Model:*
     * **Multiplied Mentorship Without Hovering:** Once an apprentice reaches `T1` via offline drills, the $\le 2000\,\mathrm{mm}$ BLE tether lets the mentor supervise $2\text{–}3$ novices at adjacent benches safely; once they reach `T2`, the unlinkable `BBS+` badge interlock handles all future tool checkouts automatically without bothering the mentor.
     * **Mentor Recognition & Anti-Hoarding Incentives:** In Layer 5, a master steward's Reputational Wealth ($\mathcal{R}$) ceiling and guild standing grow **when they successfully mentor new `T2/T3` peers**—structurally rewarding knowledge sharing and making solitary skill-hoarding counterproductive.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ mentors}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* Polycentric guilds across a settlement and watershed ($N_s = 20\text{–}200+$ mentors across 25 scenarios).
   * *Governance Model:*
     * **Anti-Nepotism 3-of-3 Panel Invariant:** Enforces household-independent Capstone panels across households/pods so credentials remain trusted across the entire $R_1 \dots R_5$ federation.
     * **Polymathic Cross-Guild Mobility:** Because survival is unconditionally guaranteed ($B_{\mathrm{baseline}} = 10{,}000\,\mathrm{bp}$) and tuition is zero, a steward can spend mornings baking in `Scenario Gamma`, afternoons studying luthiery in `Scenario Lambda`/`Alpha`, and winters learning botanical chemistry in `Scenario Eta`/`Chi`.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. External Trade-School & Clinical Licensure Portability:** Translating internal `Kappa` bench hours and `SociologicalCapstoneAttestation` proofs into state-recognized journeyman electrician, plumber, EMT (`Scenario Chi`), or PE engineering credits.<br>**B. Importing Reference Textbooks & Calibration Standards:** Acquiring specialized technical manuals or NIST-traceable gauge blocks not yet in the mesh. | • **One-Way Credential Export:** `col-adversaryd` compiles state-compliant apprenticeship hour logs under a registered **State Apprenticeship Sponsor / Cooperative Trade School** wrapper when a member wants a Layer 7 license (e.g., to sign building permits in `Scenario Xi`).<br>• **Immediate Commons Ingestion:** Any imported technical reference is digitized into the `Scenario Iota` Alexandria Commons so no second node ever has to buy it again. |
| **2. Purely Defensive Reach into Layer 7** | **A. OSHA / Insurer Safety Demonstration:** Proving to municipal inspectors or cooperative insurers (`Scenario Epsilon`) that shared workshops enforce stricter machine safety than commercial factories.<br>**B. Learner Privacy Shield (`Mundane Semantic Camouflage`):** Protecting internal skill distributions and apprentice identities from external labor brokers or state registries. | • **Cryptographic Zero-Knowledge Safety Audit:** Demonstrates hardware $< 10\,\mathrm{ms}$ `LOTO` and `BBS+` interlock compliance to insurers without revealing which individual stewards operated which machines.<br>• **Private Association Vocational Exemption:** Frames internal guild mentorship under **UUNAA Section 6** non-commercial member education. |

---

## 7. Horizon Council v3.2 Architectural Binding (*The Architecture of Boundless Time* — D1–D8)

| Horizon Council Dimension | Scenario Kappa (`SCN-KAPPA-APPRENTICESHIP` / `kappa_apprenticeship`) Binding |
| :--- | :--- |
| **Node Membership Regime ($N_s$) & Unified Voice $V_i(t) = 1 + R_i(t)$** | $N_s = 2 \to 20$ mentors & apprentices ($R_{\mathrm{mentor}} > 0$ paired with $R_{\mathrm{apprentice}}(0) = 0$, $B=1$ civic floor guaranteed from Day 0). |
| **Primary Layer 6 Lens & Need-Capture Mode** | ***The Workshop Lens*** (Pillar II Tier-0 unsupervised starter tasks & Tier-1 mentor co-signed dual-key tasks). |
| **Enveloping Node Hypervisor Lease (`cB`, `mW`, `mK`, `mm`)** | **Training Bay Envelope** (`7,500 cB`, hardware interlock power lease `mW`, $\le 2,000\,\mathrm{mm}$ BLE mentor tether). |
| **L4 3-Tier Cascade & Stage I$\to$III Hardware Retrofit Archetype** | Canonical showcase of **Pillar II (`collective:WorkCapabilityMatrix`)**: solves the cold-start $R_i=0$ onboarding paradox via mentor co-signed **Stage II Right-to-Repair** retrofit tasks. |
