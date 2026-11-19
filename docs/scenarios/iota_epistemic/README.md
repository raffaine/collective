# Scenario Iota: Epistemic Open Science, Metrology Replication & Alexandria Commons (`SCN-IOTA-EPISTEMIC`)

* **Identifier:** `SCN-IOTA-EPISTEMIC`
* **Horizontal Wave:** Wave E (Third Place, Epistemic R&D, Culture & Polycentric Guilds)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Iota governs **how The Collective discovers, verifies, synthesizes, and distributes empirical truth and foundational knowledge across generations**. Together with `Scenario Kappa` (somatic trade mastery) and `Scenario Lambda` (arts, music, and philosophy), Scenario Iota forms the **STEAM & Preparedness Triad**.

Its scope spans three interlocking pillars:
* **Lifelong Multi-Age Pedagogical Inquiry:** Replacing standardized factory schooling with curiosity-driven Inquiry Circles, Socratic mentorship, interactive `Oasis` physics/ecology sandboxes, and hands-on field/lab investigation connected to every real guild of the settlement.
* **Distributed Citizen Science & $N \ge 3$ Empirical Replication:** Pre-registered experimental protocols, vibration-isolated metrology calibration ($\le 50\,\mathrm{bp}$ drift gate), and multi-node replication that directly updates shared `NodeFacilityBlueprint` and ontological parameters—giving **strict equal publication parity to null and negative results**.
* **The Offline Alexandria Commons & Physical Bindery:** Hosting a censorship-resistant local mirror of human knowledge (books, journals, schematics, musical scores, and open datasets) paired with on-demand archival paper printing and bookbinding (`Scenario Nu`/`Eta`).

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy academic, educational, and publishing institutions (Layer 7), the pursuit and transmission of knowledge are crippled by four structural pathologies:

1. **The Replication Crisis & Null-Result File-Drawer Bias:** Academic journals and corporate R&D labs reward flashy positive claims while burying null or negative experiments—causing researchers worldwide to waste billions of joules and years of labor repeating undocumented dead ends.
2. **Publisher Paywalls & Corporate IP Silos:** Publicly funded scientific papers, agricultural trials, and medical breakthroughs are locked behind predatory journal paywalls or corporate trade secrets where the farmers, paramedics, and engineers who need them cannot read them.
3. **Alienated Factory Schooling:** Young learners are segregated by narrow birth-year cohorts in windowless classrooms to memorize disconnected abstractions for standardized tests, severed from living ecosystems (`Scenario Eta`), workshops (`Scenario Alpha`), and elders (`Scenario Phi`).
4. **Uncalibrated Citizen-Science Noise:** Amateur environmental or agricultural logs often rely on uncalibrated consumer sensors whose drift invalidates longitudinal comparisons across valleys.

---

## 3. Wide Feature Horizon

Scenario Iota replaces paywalled academia and factory schooling with five epistemic capabilities:

1. **Pre-Registered Epistemic Lab Notebooks & Metrology Drift Gates ($\le 50\,\mathrm{bp}$):** Before any agricultural trial (`Gamma`/`Eta`), alloy test (`Theta`), or clinical assay (`Chi`) begins, researchers publish an `EpistemicResearchAndCurriculumIntent` pre-registering their hypothesis, control variables, and biosafety guardrails. Layer 6 refuses to admit experimental sensor streams into a replication set if the instrument's calibration drift exceeds **$50\,\mathrm{bp}$ ($0.5\%$)** against the node's metrology reference standards—automatically spawning a calibration task instead of polluting the dataset.
2. **$N \ge 3$ Multi-Node Replication & Negative-Result Parity:** When $N \ge 3$ independent nodes across different microclimates replicate an experiment at $p < 100\,\mathrm{bp}$ ($1\%$), `col-commonsd` compiles a **Deterministic Read-Back** proposing a calibrated update to the corresponding `https://w3id.org/collective/v1/` ontological parameter and `NodeFacilityBlueprint` catalog. Crucially, **disproven hypotheses and null results ($p \ge 100\,\mathrm{bp}$) are canonized with identical prominence** so no sister node ever repeats a failed trial blindly.
3. **Multi-Age Inquiry Circles & `Oasis` Interactive Physics Sandboxes:** Following early childhood wonder in `Scenario Delta`, learners explore mathematics, thermodynamics, biology, history, and ethics through real settlement projects and interactive 3D `Oasis` sandboxes (`Scenario Zero`)—shrinking down to watch mycorrhizal nutrient exchange or tuning a virtual PID controller before stepping into `Scenario Kappa` bench apprenticeships.
4. **The Offline Alexandria Mesh Library & Rag-Paper Bindery:** Every node maintains an offline content-addressed mirror (`IPFS`/`IPLD` synced via `Scenario Beta` fiber and DTN cargo-trike sneakernet) of millions of open-access texts, standards, and schematics, paired with a physical printshop and rag-paper bookbindery that produces durable, screenless field manuals and textbooks.
5. **Hard Biosafety & Dual-Use `Protocol of Refusal`:** The Epistemic Guild and Commons Chamber enforce an absolute ethical veto blocking gain-of-function pathogen research, ecological gene-drive release, or weaponizable chemical/nuclear experimentation.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`EpistemicResearchAndCurriculumIntent`:** Pre-registering a scientific experiment, agronomic field trial, or multi-age Inquiry Circle syllabus with explicit metrology drift bounds ($\le 50\,\mathrm{bp}$) and biosafety declarations.
* **`ReplicationDatasetAttestation`:** Submitting hardware-signed experimental observations (positive, null, or negative) from an independent node toward an open $N \ge 3$ replication cohort.
* **`OntologicalParameterCalibrationProposal`:** Proposing an empirical update to a shared blueprint or ontology constant once $N \ge 3$ independent replications converge.
* **`AlexandriaArchivalPrintIntent`:** Printing and binding a durable physical book, wet-lab protocol manual, or musical score at the local bindery.

### B. Semantic Representations & Deterministic Read-Backs
* **Self-Calibrating Ontological Grounding (`https://w3id.org/collective/v1/`):** Unlike static software schemas whose constants are guessed once by programmers, Layer 6's ontological parameters (e.g., `soil_vwc_retention_baseline_bp`, coppice regrowth yields, solar kiln drying curves) link directly to the CIDs of the $N \ge 3$ `ReplicationDatasetAttestation` graphs that verified them.
* **Mandatory Calibration & Replication Read-Back:** No empirical trial automatically overwrites a node's active blueprints. When $N \ge 3$ replications complete, `col-commonsd` presents a **Deterministic Read-Back** to the relevant guild stewards showing the old parameter value, the newly replicated value across $N$ nodes, the sensor calibration proofs ($\le 50\,\mathrm{bp}$), and any null-result counter-evidence before hardware signature.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Pre-Registered Epistemic Lab Notebook** | Wet-Lab / Field Slate (`iota:researcher`, `crdt:sync`) | Pairs directly with analytical balances, pH meters, and spectrometers; automatically checks instrument calibration drift ($\le 50\,\mathrm{bp}$) before unlocking data entry and treats null results as first-class discoveries. |
| **Multi-Node Replication & Ontology Workbench** | Guild Research Desk (`iota:canonizer`, `gov:ratify`) | Visualizes side-by-side replication curves from $N \ge 3$ sister nodes across the watershed and compiles the pre-sign Deterministic Read-Back for blueprint parameter updates. |
| **Alexandria Offline Reader & Socratic Tutor** | E-Ink Reader / Laptop (`crdt:read`, `r0:local_only`) | Distraction-free search across the entire offline Alexandria library, optionally assisted by a local, grounded open-weight Socratic tutor that cites exact textbook/paper paragraphs and keeps student questions $100\%$ private in $R_0$. |
| **Archival Rag-Paper Field Manual & Lab Ledger** | Physical Bindery / Wet-Lab Bench | Screenless acid-free linen/hemp paper notebooks stamped with QR graph anchors for wet, muddy, or high-solvent environments where tablets shouldn't go. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ learner/researcher}$):**
   * *Context:* A solo homesteader running soil/water assays in a home kitchen lab and studying from the local Alexandria mirror.
   * *Governance Model:* **Full access to planetary knowledge & peer replication invitation.** The solo researcher has the entire offline Alexandria Commons at their fingertips, can log pre-registered backyard cultivar trials, and can broadcast a lightweight replication request inviting two neighboring homesteads ($N \ge 3$) to test the same soil amendment in their gardens.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ inquiry circle}$):**
   * *Context:* One or two rotating scholar-stewards / elders (`Scenario Phi`) facilitating a neighborhood multi-age Inquiry Circle, reading room, and citizen-science bench for $6\text{–}20$ youth and adults ($N_p > 1$).
   * *Governance Model:*
     * **Grade-Free, Curiosity-Driven Portfolio:** Learners are never ranked against each other or subjected to coercive grading; completed inquiries and replicated experiments live in the learner's own portfolio and bridge directly into `Scenario Kappa` hands-on trade checkouts when they want to use hazardous tools.
     * **Shared Metrology Calibration:** The neighborhood lab steward maintains the reference weights, buffer solutions, and thermometer bath so all $N_p$ citizen scientists' instruments stay within the $\le 50\,\mathrm{bp}$ drift gate.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A federated Epistemic & Metrology Guild ($N_s = 15\text{–}100+$ scientists, librarians, teachers, and metrologists) operating wet-labs, astronomical/ecological observatories, metrology vaults, and print binderies across $R_2 \dots R_5$.
   * *Governance Model:*
     * **Cross-Node Replication Before Canonization ($N \ge 3$):** Prevents any single charismatic researcher or node from pushing unverified "miracle methods" into the bioregional blueprint catalog; only findings replicated across $\ge 3$ independent nodes with $\le 50\,\mathrm{bp}$ sensor accuracy can update shared defaults.
     * **Bicameral Biosafety & Ethics Veto:** Any experiment involving live biological agents, watershed hydrology, or human clinical protocols (`Scenario Chi`) requires explicit approval from an independent Ethics & Biosafety Circle and remains subject to immediate Commons Chamber veto.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Scientific Literature, Patent & Dataset Ingestion:** Pulling newly published open-access journals, geological surveys, and technical standards from the global internet into the Alexandria Commons.<br>**B. Traceable Metrology Standards & Analytical Reagent GPO:** Procuring NIST-traceable calibration weights, reference thermocouples, or analytical-grade HPLC standards (`Scenario Chi`) not yet synthesized locally. | • **One-Way Detox & Permanent Offline Archiving:** Ingested literature is stripped of publisher tracking scripts across `l6_l7.sock`, indexed into content-addressed `IPFS`/`IPLD` bundles, and replicated across all nodes via `Scenario Beta` DTN sneakernet.<br>• **Primary-Standard Cascade:** A single imported NIST reference standard in an $R_4$ metrology vault is used to calibrate hundreds of locally fabricated secondary working standards (`Scenario Alpha`/`Theta`), driving external metrology dependency toward zero. |
| **2. Purely Defensive Reach into Layer 7** | **A. Defensive Prior-Art Publication (Patent-Troll Destruction):** Preventing external corporations from patenting open-hardware tools, botanical extracts, or cultivar genetics discovered inside The Collective.<br>**B. Homeschool / Alternative-School Statutory Shielding:** Protecting multi-age Inquiry Circles from truancy or rigid state curriculum enforcement. | • **Timestamped Defensive Prior Art:** `col-commonsd` automatically publishes cryptographic hashes and technical disclosures of all newly canonized `Iota`/`Alpha`/`Chi` breakthroughs to public Layer 7 prior-art registries (`Planetary Lighthouse`), establishing legally binding prior art worldwide.<br>• **Umbrella Private School / Cooperative Education Wrapper:** Files routine statutory homeschool/micro-school equivalence notices while keeping individual learner portfolios $100\%$ private inside $R_0$. |
