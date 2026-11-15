# Scenario Delta: Perinatal Sanctuary, Postpartum Cocoons & Cooperative Childcare Pods (`SCN-DELTA-CHILD`)

* **Identifier:** `SCN-DELTA-CHILD`
* **Horizontal Wave:** Wave C (Care, Kinship, Apprenticeship & Decommodified Shelter)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)

---

## 1. Intended Scope & Domain Vision

Scenario Delta governs **how a community welcomes new life, supports recovering parents during the fourth trimester, and nurtures infants, toddlers, and young children**. It replaces isolated nuclear-family exhaustion and commercial daycare commodification with a warm, multi-generational **Perinatal Sanctuary & Cooperative Childcare Pod Commons**.

Its scope covers:
* **Prenatal, Midwifery & Birth Sanctuary Support:** Coordinating clinical midwifery (`Scenario Chi`), doulas, and calm birthing suites.
* **The 90-Day Postpartum Cocoon:** Wrapping new biological, adoptive, or foster parents in three months of automated, silent cross-scenario nourishment, diaper sanitation, and rest shielding from a single pre-birth signature.
* **Camera-Free Cooperative Play Pods:** Neighborhood nurseries and forest-play gardens governed by hardware-verified two-adult safeguarding, strict age-weighted child ratios, and pure thermodynamic/environmental safety (zero optical cameras or audio bugging).
* **Non-Commensurable Care (`domestic_care_token_minting == 0`):** Treating child-rearing as a sacred shared joy of the commons, never a priced or tokenized market transaction.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy urban and suburban society (Layer 7), bringing a child into the world or raising young children is punished by four systemic failures:

1. **Fourth-Trimester Isolation & Parental Exhaustion:** New parents are discharged home to isolated apartments where they must simultaneously heal from birth, nurse around the clock, cook meals, scrub laundry, and return to wage jobs within weeks—triggering epidemic postpartum depression and burnout.
2. **Prohibitive Commercial Daycare Costs vs. Caregiver Poverty:** Families pay $25\text{–}50\%$ of their income for commercial childcare slots with long waitlists, while childcare workers themselves are paid poverty wages and overloaded with unsafe child-to-adult ratios.
3. **Surveillance "Smart Nurseries":** Commercial baby monitors and corporate daycare apps stream live video and audio of sleeping infants and playing toddlers to cloud servers, normalizing childhood surveillance and leaking intimate family biometrics.
4. **Landfill Diaper & Formula Dependency:** Millions of single-use petrochemical diapers enter landfills daily while families lack safe, pasteurized community human-milk banking and organic baby-food kitchens.

---

## 3. Wide Feature Horizon

Scenario Delta replaces parental isolation and commercial daycare with five integrated care loops:

1. **One-Sign 90-Day Postpartum Cocoon (`PostpartumCocoonIntent`):** Signing a single intent prior to birth or placement silently orchestrates **90 days of background cross-scenario workflows** with zero daily scheduling chores for the parents:
   * Daily `Scenario Gamma` nutrient-dense bone-broth, lactation-support, and stew tiffins delivered quietly to the insulated porch box.
   * Twice-weekly `Scenario Beta` cargo-trike pickup and hospital-grade thermal sanitation ($\ge 344{,}150\,\mathrm{mK}$ / $71.0^\circ\mathrm{C}$ + ozone) of reusable cotton cloth diapers and `Scenario Nu` organic infant layettes.
   * Automatic `Scenario Sigma` **Dual-Freeze Care Leave** shielding all household adults from external guild pings while preserving $100\%$ of $B_{\mathrm{baseline}}$.
2. **Two-Adult NFC Safeguarding & Weighted Age-Ratio Interlocks:** Cooperative play pods require at least $N_{\mathrm{adults}} \ge 2$ `BBS+`-vetted adult stewards (paired smoothly with `Scenario Phi` elder storytellers) and enforce a strict age-weighted child ceiling ($\sum w_i \le 40{,}000 \cdot N_{\mathrm{adults}}$, where infants $= 20{,}000\,\mathrm{bp}$, toddlers $= 10{,}000\,\mathrm{bp}$, preschoolers $= 5{,}000\,\mathrm{bp}$).
3. **Zero-Camera Thermodynamic Nursery Safety:** Bans optical cameras and microphones inside nurseries, relying instead on local Layer 2 physical protections: thermostatic water mixing valves mechanically locked $\le 311{,}150\,\mathrm{mK}$ ($38.0^\circ\mathrm{C}$) against scalding, $\mathrm{CO}_2 < 800\,\mathrm{ppm}$ fresh-air ventilation, and acoustic calm $\le 450\,\mathrm{cB}$.
4. **Assay-Verified Donor Human-Milk Banking & First-Foods Larder:** Community human-milk shares undergo verified Holder pasteurization ($335{,}650\,\mathrm{mK}$ / $62.5^\circ\mathrm{C}$ for $30\,\mathrm{min}$) and rapid cooling before dispensation alongside allergen-screened `Scenario Gamma` purees.
5. **Montessori / Forest-Play Exploration (`Scenario Eta` / `Iota` Bridge):** Toddlers and preschoolers transition naturally from cozy indoor play pods into outdoor garden, mud-kitchen, and nature-discovery circles that seed lifelong curiosity for `Scenario Iota`.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`PostpartumCocoonIntent` (or `PrenatalAndBirthSanctuaryIntent`):** Pre-registering an expected birth or adoption window to automatically chain 90 days of `Gamma` tiffin delivery, `Beta`/`Nu` cloth-diaper loops, and `Sigma` Care Leave.
* **`CarePodSessionIntent`:** Opening or checking a child into a neighborhood cooperative play pod session, verifying two-adult co-presence and age-weighted capacity headroom.
* **`HumanMilkAndInfantNutritionAttestation`:** Recording Holder pasteurization thermal holds and zero-knowledge allergen compatibility for infant feedings.
* **`EmergencyCareFlare`:** A zero-LLM Class 1 priority pager alert summoning an immediate relief adult or `Scenario Chi` pediatric medic to the pod in $< 500\,\mathrm{ms}$.

### B. Semantic Representations & Deterministic Read-Backs
* **90-Day Silent DAG Expansion (`col-commonsd` $\to$ `col-execd`):** Demonstrates Layer 6's highest form of compassionate variety attenuation: one human-authored JSON-LD document compiles into dozens of recurring kitchen, courier, laundry, and rest-shield workflows that run for three months without asking the sleep-deprived parents to tap a single button.
* **Hardware-Enforced Pediatric Invariants:** Play pod sessions cannot enter `SESSION_ACTIVE` unless $N_{\mathrm{adults}} \ge 2$ hardware NFC taps are active, weighted child load $\le 40{,}000 \cdot N_{\mathrm{adults}}$, tap water $\le 311{,}150\,\mathrm{mK}$, and `opticalAndAudioSensorsPermitted == false`.
* **Mandatory Cocoon & Custody Read-Back:** Before parents sign a `PostpartumCocoonIntent` or authorize trusted pickup adults (`Omicron` kin), the slate displays a plain-language **Deterministic Read-Back** confirming authorized pickup DIDs, allergen masks, and zero external data sharing.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Camera-Free Perinatal Vestibule E-Ink & 2-Adult NFC Slate** | Play Pod Entryway (`care:pod_steward`) | Two vetted adults tap badges to open the pod; parents tap child's passive wristband/fob at drop-off and pickup; displays live $\mathrm{CO}_2$, tap-water safety, and adult-to-child ratio with zero cameras. |
| **One-Sign Postpartum Cocoon Planner** | Expectant Parent Tablet / Hearth Slate (`intent:author`) | Calm 1-page setup before birth: choose tiffin portion counts, dietary needs, and diaper sizes once, then close the screen for 90 days. |
| **Tactile Pediatric Relief / Medic Pull-Cord** | Play Pod Wall (`chi:responder`, `care:pod_steward`) | Screenless cord and 2-button fob: Button 1 calls a backup adult from the neighborhood ring (e.g., when a toddler needs one-on-one soothing); Button 2 dispatches a `Scenario Chi` pediatric medic. |
| **Diaper & Layette NFC Tote Tag** | Porch Laundry Bin (`courier:dtn`) | Automatically flags full cloth-diaper totes for the next `Scenario Beta` cargo-trike pickup and hospital-grade thermal wash cycle. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ parent household}, N_p = 1\text{ family unit}$):**
   * *Context:* A solo parent or single household raising an infant or toddler on their homestead lot.
   * *Governance Model:* **Unconditional community wrap-around (`No Solo Parent Left Isolated`).** Even when a household has only $N_s = 1$ adult, activating a `PostpartumCocoonIntent` or `EmergencyCareFlare` mobilizes surrounding $R_1 / R_2$ hearth tiffins, diaper milk-runs, and vetted respite caregivers so a solo parent never faces illness or sleep deprivation alone.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 2\text{ pod stewards}, N_p > 1\text{ neighborhood families}$):**
   * *Context:* Two rotating parent or elder stewards hosting a living-room or backyard play pod for $4\text{–}8$ neighborhood children ($N_p > 1$).
   * *Governance Model:*
     * **Mandatory Two-Adult Co-Presence ($N_s \ge 2$):** Unlike workshop or kitchen domains where $N_s = 1$ can serve a neighborhood alone, Scenario Delta constitutionally requires **at least two vetted adults** present for any shared play pod session—protecting both children and caregivers while preventing single-caregiver overwhelm.
     * **Hard Weighted Ratio Ceiling:** If a 9th child arrives that would push the weighted ratio above $40{,}000\,\mathrm{bp}$ per adult, the vestibule slate gently holds check-in and pings the on-call parent rotation to add a third adult steward within minutes.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A settlement Early Childhood & Perinatal Guild ($N_s = 15\text{–}60+$ early-childhood educators, midwives, and rotating parents/elders) operating dedicated birthing suites, multi-room nurseries, milk banks, and forest schools across $R_1 \dots R_3$.
   * *Governance Model:*
     * **Intergenerational Pairing with `Scenario Phi` Elders:** Connects elder storytellers, musicians (`Scenario Lambda`), and gardeners (`Scenario Eta`) with early-childhood pods under non-coercive, two-adult safeguarding.
     * **Zero Care Tokenization (`domestic_care_token_minting == 0`):** Participation in childcare pods and postpartum cocoons is a universal baseline right of every child and parent—never gated by a family's ledger balance or work hours.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Specialized Pediatric & Obstetric Supplies:** Procuring hospital-grade breast-milk pasteurization flasks, car seats, or neonatal clinical consumables not yet fabricated locally.<br>**B. Statutory Child Tax Credits & Paid Parental Leave Drawdown:** Helping hybrid Stage 1/2 parents claim all state paid family leave and child benefit entitlements. | • **Pediatric Safety Priority:** Layer 5 grants top external procurement priority to certified child-safety hardware (e.g., ECE/DOT vehicle restraints) while logging reusable textile/nursery furniture needs for `Scenario Nu` and `Scenario Alpha`.<br>• **Household Benefit Retention:** External statutory child benefits remain $100\%$ with the family unit. |
| **2. Purely Defensive Reach into Layer 7** | **A. Cooperative Playgroup & Shared-Custody Statutory Exemptions:** Protecting neighborhood parent-cooperative play pods from commercial daycare licensing extortion aimed at corporate chains.<br>**B. Absolute Pediatric Privacy (`Mundane Semantic Camouflage`):** Preventing data brokers, schools, or state agencies from harvesting children's developmental logs or family structures. | • **Parent-Cooperative Reciprocal Playgroup Carapace:** Structures neighborhood pods under statutory reciprocal parent-co-op exemptions and **UUNAA Section 6** private association covenants where parents rotate stewardship with zero commercial fee.<br>• **Zero Outbound Child Telemetry:** Child names, birthdates, and pod attendance logs are strictly blocked from crossing `l6_l7.sock`. |
