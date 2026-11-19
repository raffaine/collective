# Scenario Chi: Zero-LLM Clinical Triage, Primary Care & Assay-Gated Pharmacopoeia (`SCN-CHI-PARAMEDIC`)

* **Identifier:** `SCN-CHI-PARAMEDIC`
* **Horizontal Wave:** Wave F (Health, Resilience, Elder Care, Justice & Terminal Decoupling)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Chi governs **how the settlement protects human health across every way our bodies and minds can fail us—and how it closes the loop on analytical, human-consumption pharmaceutical and botanical chemistry**.

Its scope encompasses two tightly coupled pillars:
* **Six-Modality Sovereign Healthcare (`domestic_care_token_minting == 0`):**
  1. *Acute Emergency Trauma & Cardiac Triage* (sub-$500\,\mathrm{ms}$ zero-LLM pager dispatch, $< 180\,\mathrm{s}$ neighborhood medic arrival).
  2. *Primary Care, Preventive Medicine, Dentistry & Midwifery* (paired with `Scenario Delta`).
  3. *Chronic Disease Management & Disability Support* (paired with `Scenario Tau` assistive permanence).
  4. *Non-Carceral Mental Health & Peer-Respite Sanctuary* (zero police dispatch; calm de-escalation suites).
  5. *Bodily, Reproductive & Gender-Affirming Sovereignty* (protected under `Scenario Omicron`'s Anti-Moralism Firewall).
  6. *Seamless Tertiary Hospital / Surgical Escalation* (clean `HL7/FHIR` handoff packets when advanced Layer 7 surgical/ICU care is required).
* **Assay-Gated Human-Consumption Chemistry & Open Pharmacopoeia:** Synthesizing and compounding WHO Essential Medicines (`Open-Insulin`, antibiotics, analgesics, oral rehydration salts, anesthetics, and standardized `Scenario Eta` botanicals) locked behind mandatory dual `HPLC` / `GC-MS` analytical purity gates ($\ge 9950\,\mathrm{bp}$ / $99.5\%$ purity and verified endotoxin bounds).

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy for-profit healthcare and pharmaceutical monopolies (Layer 7), human vulnerability is exploited through four systemic failures:

1. **Medical Bankruptcy & Artificial Drug Rationing:** Pharmaceutical cartels charge hundreds of dollars for vials of insulin, epinephrine auto-injectors, or asthma inhalers that cost pennies in raw precursors to synthesize, while insurance networks deny care or bankrupt families after an ambulance ride or emergency surgery.
2. **Dangerous "AI Triage Chatbot" Hallucinations:** Tech companies increasingly insert stochastic Large Language Models (`LLMs`) into clinical intake and triage funnels, where a single hallucinated dosage or delayed ambulance dispatch during sepsis, stroke, or anaphylaxis can be fatal.
3. **Carceral Police Response to Mental-Health Crises:** Calling emergency services for a neighbor experiencing acute psychiatric distress or panic routinely brings armed police rather than trained peer-respite medics—escalating care crises into violence or incarceration.
4. **Unverified "Grey-Market" Compounding vs. Total Import Dependence:** Off-grid groups that attempt DIY medicine without analytical chemistry instrumentation (`HPLC`, `GC-MS`, Limulus endotoxin assays) risk lethal contamination or dosing errors, while communities that rely $100\%$ on distant commercial pharmacies face collapse during supply-chain shocks (`Scenario Upsilon`).

---

## 3. Wide Feature Horizon

Scenario Chi replaces both corporate medical extortion and unsafe DIY guesswork with five clinical and chemical loops:

1. **Zero-LLM Deterministic Trauma Dispatch (`< 500 ms` Alert, `< 180 s` Bedside Arrival):** Pulling a physical red wall cord or pressing a tactile emergency fob bypasses $100\%$ of NLU/LLM code paths—broadcasting a deterministic, pre-translated Class 1 alert directly to on-call `BBS+`-credentialed (`Scenario Kappa` `T2/T3_Medic`) neighborhood responders who carry stocked trauma/AED kits to reach any bedside within three minutes.
2. **Closed-Loop Analytical Pharmacopoeia (`HPLC` / `GC-MS` $\ge 9950\,\mathrm{bp}$ Gate):** Bioreactor and organic-synthesis wet-labs (`Open-Insulin`, beta-lactam/macrolide antibiotics, lidocaine, acetaminophen/salicylates, sterile IV saline, and `Scenario Eta` botanical tinctures) are mechanically locked from clinical dispensation until dual independent `HPLC` and `GC-MS` assays verify active-ingredient concentration within $\pm 200\,\mathrm{bp}$ ($2\%$), chemical purity $\ge 9950\,\mathrm{bp}$ ($99.5\%$), endotoxin $< 500\,\mathrm{mEU/mL}$, and uninterrupted $[275{,}150,\, 277{,}150]\,\mathrm{mK}$ ($2.0\text{–}4.0^\circ\mathrm{C}$) cold-chain storage. Any out-of-spec or thermally degraded batch is automatically routed to `Scenario Mu` high-temperature pyrolysis (`material_id = 99`).
3. **Comprehensive Primary, Dental, Midwifery & Preventive Clinic:** Provides routine blood/metabolic panels, vaccinations, preventative dental hygiene and atraumatic restorative dentistry, physical therapy, and reproductive/gender-affirming care at zero internal cost (`0` tokens).
4. **Non-Carceral Peer-Respite Sanctuary Suites:** Acute mental-health, grief, or neurodivergent sensory crises route strictly to calm, unrecorded peer-respite sanctuary rooms (`Scenario Zeta`/`Sigma`) staffed by trauma-informed counselors—never law enforcement.
5. **Automatic Responder Decompression & Tertiary `HL7/FHIR` Bridge:** The moment an acute trauma call concludes, `col-execd` automatically triggers a `Scenario Sigma` **Responder Decompression & Hydrotherapy Hold** for the medics; if a patient needs a Layer 7 trauma center (e.g., neurosurgery or cardiac catheterization), the responder slate compiles a one-tap, data-minimized `HL7/FHIR` clinical summary and `Scenario Omicron` chosen-kin HIPAA proxy packet for the receiving ambulance/hospital.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`EmergencyTraumaDispatchIntent` (Zero-LLM Class 1):** Triggered by physical pull-cord, fob, or `Scenario Phi` mmWave fall radar; dispatches the nearest credentialed medic ring in $< 500\,\mathrm{ms}$.
* **`ClinicalAndPharmacopoeiaIntent`:** Scheduling a primary-care, dental, midwifery, or peer-respite visit, or requesting a prescription refill from the community dispensary (`domesticCareTokenMinting: 0`).
* **`TriageAndAnalyticalAssayAttestation`:** Dual-chemist + pharmacist hardware co-sign attesting `HPLC`/`GC-MS` purity ($\ge 9950\,\mathrm{bp}$), sterility, and cold-chain integrity to unlock a compounded medicine batch.
* **`TertiaryHospitalEscalationPacket`:** Patient- or proxy-authorized export of vital signs, medications administered, and `Omicron` medical power-of-attorney credentials for external EMS/hospital handoff.

### B. Semantic Representations & Deterministic Read-Backs
* **Responder-Ring Encrypted Clinical Graph (`R0_CLINICAL_ENCLAVE`):** In `https://w3id.org/collective/v1/`, personal medical records, mental-health notes, and reproductive/gender-affirming care logs are encrypted strictly to the patient's hardware key and their designated clinical care ring—completely invisible to general neighborhood assemblies, workshop guilds, or Layer 7 brokers.
* **Hard Assay-Before-Dispense Interlock:** `col-commonsd` and Layer 2 solenoid locks physically bar a pharmacy cabinet from opening for any locally synthesized or compounded batch lacking a valid `TriageAndAnalyticalAssayAttestation` with `purityBp >= 9950` and uninterrupted `[275150, 277150] mK` cold-chain telemetry.
* **Mandatory Dual-Chemist Read-Back:** Before releasing a synthesized pharmaceutical lot, two `Kappa T3`-certified stewards review a **Deterministic Read-Back** of the `HPLC` chromatogram peak integration, `GC-MS` residual solvent check (`0` toxic carryover), and endotoxin assay before tapping their keys.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Zero-LLM Deterministic Trauma Pager & Red Pull-Cord** | Every Building Vestibule & Medic Belt (`chi:responder`) | 100% deterministic hardware/firmware path (`0` LLM/NLU code): pulling the wall cord sounds a local alert and pages on-call neighborhood medics in $< 500\,\mathrm{ms}$ with exact room coordinates and AED location. |
| **HPLC / GC-MS Open-Pharmacopoeia Assay Slate** | Analytical Wet-Lab & Dispensary (`chi:pharma`) | Displays live chromatographic purity curves ($\ge 9950\,\mathrm{bp}$), cold-chain fridge logs ($275{,}150\text{–}277{,}150\,\mathrm{mK}$), and dual-steward release sign-offs; auto-quarantines expired vials to `Scenario Mu` pyrolysis. |
| **Private $R_0$ Health & Midwifery Chart Slate** | Clinic Exam Room / Patient Slate (`chi:clinical`, `r0:local_only`) | Offline encrypted chart for primary care, dental mapping, and chronic-condition tracking; prints instant `HL7/FHIR` paper/QR transfer summaries if tertiary hospital transport is needed. |
| **Color-Coded Trauma Kit NFC Seal & Cold-Chain Tag** | First-Aid Cabinets & Insulin Fridges (`chi:responder`) | Screenless tamper-evident NFC seal on emergency bags: medics see at a glance that airway, hemorrhage, and anaphylaxis supplies are stocked, sterile, and in-date. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ homesteader}$):**
   * *Context:* A solo homesteader maintaining a stocked trauma/first-aid kit, personal essential medications in a $48\,\mathrm{V}$ battery-backed medical fridge, and a wearable emergency beacon peered with neighboring homesteads.
   * *Governance Model:* **Automated expiration & cold-chain guardian.** Layer 6 stays quiet until an epinephrine pen or antibiotic pack approaches expiration or fridge temperature drifts, while pressing the solo steward's emergency fob immediately alerts peered $R_1$ neighbors and external EMS if needed.
2. **Single / Core Medic Ring Serving a Neighborhood Circle ($N_s = 2\text{–}6\text{ street medics/nurses}, N_p > 1\text{ neighborhood pod}$):**
   * *Context:* A neighborhood paramedic responder ring and first-aid/herbal clinic serving $10\text{–}40$ households ($N_p > 1$).
   * *Governance Model:*
     * **On-Call Rotation & Post-Call Decompression (`Scenario Sigma`):** Neighborhood medics rotate on-call pager duty so no single nurse or EMT is tethered 24/7; every emergency call automatically grants the responding medics a `Scenario Sigma` rest shield and warm `Scenario Zeta` bathhouse decompression slot.
     * **Strict Clinical Scope & Escalation Discipline:** `Scenario Kappa` credentials define clear clinical boundaries (`T1` First Aid/CPR $\to$ `T2` EMT/Paramedic $\to$ `T3` RN/Midwife/MD/Pharmacist); anything beyond local scope triggers immediate tertiary `HL7/FHIR` ambulance handoff without hesitation or ego.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A regional Sanctuary Clinic, Dental Suite & Analytical Pharmacopoeia Guild ($N_s = 20\text{–}150+$ physicians, nurses, midwives, dentists, mental-health counselors, and analytical chemists) serving $500\text{–}5{,}000+$ residents across $R_2 \dots R_5$.
   * *Governance Model:*
     * **Absolute Separation of Care from Ledger Balances (`domestic_care_token_minting == 0`):** Every clinical treatment, surgery escalation, mental-health stay, dental restoration, and compounded medication is an unconditional $B_{\mathrm{baseline}} = 10{,}000\,\mathrm{bp}$ right—never billed or rationed by reputation.
     * **Independent Analytical Chemistry Verification:** The team that synthesizes a pharmaceutical batch (`Open-Insulin` or antibiotics) can **never** unilaterally clear its own batch for the dispensary; an independent `Kappa T3` analytical chemist must co-sign the `HPLC`/`GC-MS` $\ge 9950\,\mathrm{bp}$ assay (`Scenario Iota`/`Kappa` separation of duties).

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Tertiary Hospital / Surgical Escalation & Catastrophic Care Pool:** Utilizing external Layer 7 trauma centers, MRI scanners, or oncology units for complex tertiary interventions.<br>**B. Pharmaceutical Precursor, Reagent & Medical Device GPO:** Procuring USP-grade chemical precursors, `HPLC` columns, sutures, or vaccine vials not yet produced in the bioregion. | • **Pooled Catastrophic Reserve ($\Phi_t$):** The `Scenario Epsilon` cooperative carapace maintains a pooled catastrophic medical reserve (and coordinates member health-share/public coverage) so no individual resident ever faces a hospital bill alone.<br>• **Medicine Dependency Tracking ($D_{\mathrm{medicine}}$):** Every imported drug or reagent is logged on the `Scenario Omega` 5-Domain Dependency Register, prioritizing open-source bioreactor and synthesis R&D (`Scenario Iota`) for the most vital essential medicines. |
| **2. Purely Defensive Reach into Layer 7** | **A. Good Samaritan, Licensed Medical Practice & Compassionate Use Shielding:** Protecting community medics, midwives, and clinics from regulatory harassment.<br>**B. Absolute Reproductive, Gender-Affirming & Mental-Health Subpoena Immunity (`Mundane Semantic Camouflage`):** Protecting sensitive patient charts from hostile state prosecutors, insurers, or employers. | • **Licensed Clinical Cooperative & Good Samaritan Carapace:** Pairs internal `Kappa` clinical credentials with state-licensed MD/NP/RN/EMT/Midwife charters for external regulatory interface.<br>• **Zero Outbound Clinical Data (*NAACP v. Alabama* + HIPAA Shield):** Patient charts live strictly in $R_0$-encrypted enclaves; `col-adversaryd` holds zero patient identities or reproductive/mental-health logs, making external fishing subpoenas mathematically empty. |
