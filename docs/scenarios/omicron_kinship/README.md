# Scenario Omicron: Chosen Kinship Covenants, Intimate Sanctuary & Unilateral Safe-Harbor Exit (`SCN-OMICRON-KIN`)

* **Identifier:** `SCN-OMICRON-KIN`
* **Horizontal Wave:** Wave C (Care, Kinship, Apprenticeship & Decommodified Shelter)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Omicron governs **how adults form pluralistic families, chosen-kinship households, co-parenting pacts, and intimate sanctuaries—and how any person can exit an unsafe or ended relationship instantaneously with full material security**.

It establishes three foundational guarantees:
* **Radical Kinship Pluralism ($N\text{-ary}$ Covenants):** Honoring dyadic partners, queer and polyamorous constellations, platonic co-parents, and multi-generational chosen families on equal constitutional footing—compiling their internal covenants automatically into enforceable Layer 7 legal armor (hospital visitation, medical power of attorney, guardianship, and lease survivorship).
* **The Anti-Moralism Firewall:** Protecting consenting adult intimacy, spiritual customs, and bodily/cognitive autonomy inside $R_0 / R_1$ from neighborhood busybodies or assembly moralizing.
* **Sub-500ms Unilateral Safe-Harbor Exit:** Ensuring that no human being is ever trapped in an unhappy or abusive household by economic dependence, housing scarcity, or mandatory mediation—a single private gesture rotates locks, allocates an independent refuge suite, and guarantees $100\%$ survival parity immediately.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy legal and economic systems (Layer 7), intimate relationships and household formation suffer from four structural failures:

1. **Economic Entrapment in Unsafe Relationships:** Countless survivors of domestic coercion or abuse remain trapped with an aggressor because leaving means immediate homelessness, loss of healthcare, frozen joint bank accounts, and months of adversarial court battles.
2. **State Mononormative & Blood-Kin Gatekeeping:** Legacy hospitals, probate courts, and landlords privilege state-licensed dyadic marriage and biological bloodlines—routinely denying intensive-care visitation, medical decision-making, or housing survivorship to chosen family, queer partners, and platonic co-parents.
3. **The Commune "Cult / Moralistic Busybody" Pathology:** Historically, many utopian communes failed the opposite way—dissolving private family boundaries, policing residents' consensual romantic lives, or imposing puritanical bans on bodily and cognitive expression.
4. **Coercive Mediation Mandates:** Misguided community justice models sometimes pressure a domestic abuse survivor to sit in a "mediation circle" with their abuser before granting separate housing.

---

## 3. Wide Feature Horizon

Scenario Omicron resolves both capitalist economic entrapment and commune moralism through five capabilities:

1. **$N\text{-ary}$ Sovereign Kinship Covenants (`SovereignKinshipCovenant`):** Any $N \ge 2$ consenting adults can form, amend, or expand a chosen-family covenant defining shared $R_0$ hearth space (`Scenario Zeta`/`Xi`), co-parenting care authorities (`Scenario Delta`), elder care designations (`Scenario Phi`), and personal heirloom succession (`Scenario Omega`).
2. **Automated Layer 7 Legal Armor Compilation:** When a household ratifies a `SovereignKinshipCovenant`, `col-commonsd`'s Ricardian compiler automatically generates state-enforceable Layer 7 legal instruments—reciprocal Durable Medical Powers of Attorney, HIPAA hospital visitation releases (`hipaa_visitation_grant`), co-parenting guardianship trusts, and `CLT` lease survivorship riders—ready for external hospital or court presentation.
3. **1-Tap Unilateral Safe-Harbor Severance (`SafeHarborSeveranceIntent`):** Exiting a relationship or household requires **zero partner co-signature, zero council mediation, zero waiting period, and zero justification**. A single $500\,\mathrm{ms}$ private hardware gesture rotates `ATECC608A` door-lock keys, provisions a non-gossiped private `Scenario Zeta` refuge suite (invisible even to the former household's CRDT peer set), activates a spatial `No-Transit` buffer (`Scenario Beta`/`Psi`), and preserves $B_{\mathrm{baseline}} = 10{,}000\,\mathrm{bp}$ independence from second one.
4. **The Anti-Moralism Firewall (`SCHEMA_VIOLATION_MORALISTIC_OVERREACH`):** Layer 6 and Layer 5 constitutionally reject any assembly proposal attempting to regulate consenting adult sexuality, relationship geometry, reproductive choice (`Scenario Chi`), or private spiritual/cognitive customs inside $R_0$—paired strictly with objective `Scenario Kappa` sobriety interlocks when operating hazardous $R_2$ industrial machinery.
5. **Physical $R_1$ Sanctuary Disconnect Switch & Customary Shift Swaps:** Households observing weekly Sabbaths, holy days, or family retreats (`CustomarySanctuaryAndShiftSwapIntent`) can throw a physical wall switch severing external telemetry while reciprocally swapping guild shifts with neighboring pods.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`SovereignKinshipCovenant`:** Forming or updating a pluralistic household, co-parenting, medical proxy, and heirloom-succession covenant via simultaneous multi-party NFC co-sign.
* **`SafeHarborSeveranceIntent`:** An emergency or peaceful unilateral 1-tap exit from a kinship covenant or shared dwelling, executing in $< 500\,\mathrm{ms}$ with zero co-signature required.
* **`CustomarySanctuaryAndShiftSwapIntent`:** Registering recurring household cultural/spiritual rest days and pairing reciprocal guild shift swaps across diverse neighborhood households.
* **`HeirloomSuccessionDesignation`:** Specifying how personal $R_0$ belongings (journals, instruments, handcrafted garments, keepsakes) pass to chosen kin upon death (`Scenario Phi`/`Omega`).

### B. Semantic Representations & Deterministic Read-Backs
* **Deliberate Asymmetric Friction (Solemn Entry vs. Instant Exit):** Forming a `SovereignKinshipCovenant` requires a slow, solemn **Deterministic Read-Back** where every party reviews the exact medical proxy, co-parenting, and Layer 7 legal rider terms before tapping their hardware keys together. Conversely, triggering `SafeHarborSeveranceIntent` bypasses all read-back delays and multi-party gates, executing unilaterally in $< 500\,\mathrm{ms}$.
* **Non-Gossiped Refuge Partitioning:** When a Safe-Harbor exit is triggered, `col-commonsd` spawns an isolated Domain-B CRDT document for the departing member's new refuge suite and suppresses location gossip to the former household's DIDs.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Kinship Covenant Ceremony Slate** | Hearth / Garden Ceremony (`kin:covenant`) | Guides partners or chosen kin through composing their shared covenant, reading back plain-language commitments, and tapping NFC rings/cards simultaneously; prints wet-ink Layer 7 HIPAA/POA wallet cards. |
| **1-Tap Safe-Harbor Exit Fob / Sanctuary Kiosk** | Personal Device or Bathhouse Vestibule (`kin:safe_exit`) | Single private $500\,\mathrm{ms}$ gesture (accessible even without a personal phone at any `Scenario Zeta` vestibule kiosk): immediately unlocks a private refuge suite and rotates former door locks with zero questions asked. |
| **Physical $R_1$ Household Sanctuary Disconnect Switch** | Household Entry Wall (`r0:hardware_latch`) | Mechanical knife/toggle switch physically disconnecting the home's external VLAN 30 bridge during family retreats, Sabbaths, or intimate gatherings. |
| **Hospital Wallet Card & QR Proxy Verifier** | Printed Card / Paramedic Slate (`chi:responder`) | Wallet-sized card carrying the signed Layer 7 HIPAA release and Durable Medical Power of Attorney so any chosen-family member is immediately recognized at external Layer 7 hospitals. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ solo adult}, N_p = 1\text{ person}$):**
   * *Context:* A single adult living solo by choice.
   * *Governance Model:* **Full dignity of solo life + chosen medical proxies.** Solo residents are never treated as "incomplete households" in housing allocation or assembly voice; they can designate any trusted friend or neighbor across $R_1 / R_2$ as their medical proxy and heirloom beneficiary via a lightweight `SovereignKinshipCovenant` without sharing a roof.
2. **Household / Kinship Pod Scale ($N_s = 1\text{–}6\text{ adult co-stewards}, N_p = 2\text{–}12\text{ household members}$):**
   * *Context:* A dyadic couple, polycule, multi-generational family, or platonic co-parenting household sharing a `Scenario Zeta`/`Xi` dwelling.
   * *Governance Model:*
     * **Internal Household Self-Determination:** The household governs its own interior $R_0$ space, kitchen routines, and cultural rhythms with zero interference from neighborhood assemblies (`SCHEMA_VIOLATION_MORALISTIC_OVERREACH`).
     * **Unconditional Individual Independence Inside the Household:** Unlike legacy marriage where one breadwinner controls the bank account, every individual inside the household holds their own independent hardware DID and their own unconditional $B_{\mathrm{baseline}} = 10{,}000\,\mathrm{bp}$ right to food, shelter, and healthcare—making economic coercion structurally impossible.
3. **Multi-Steward Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ settlement & peer nodes}$):**
   * *Context:* A neighborhood or campus settlement ($N_p = 100\text{–}2{,}500+$ residents).
   * *Governance Model:*
     * **Standing Refuge Buffer ($\ge 500\,\mathrm{bp}$ Reserved Suites):** The settlement's `Scenario Zeta` housing guild constitutionally maintains unallocated, welcoming sanctuary suites across the mesh so any `SafeHarborSeveranceIntent` can be fulfilled in $< 500\,\mathrm{ms}$—including across neighboring sister nodes ($R_2 / R_3$) if the departing person wants geographical distance.
     * **Cross-Cultural Shift Reciprocity:** When diverse households observe different holy days or seasonal festivals, `col-commonsd` matches reciprocal guild shift swaps automatically so every tradition is honored while critical clinical and microgrid watches stay covered.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Statutory Spousal, Survivor & Tax Optimization:** Utilizing available Layer 7 tax filing brackets, health-insurance dependent rules, or survivor pension benefits while members still interface with legacy institutions.<br>**B. External Domestic Violence Relocation Support:** Assisting survivors fleeing abuse in Layer 7 to enter the settlement via Pathway 4 (`Scenario Tau`). | • **Decoupling Internal Truth from External Tax Forms:** How two members file a legacy IRS tax return in Layer 7 never constrains their true $N\text{-ary}$ `SovereignKinshipCovenant` inside Layer 6.<br>• **Zero-Barrier Survivor Sanctuary:** External survivors arriving at the node receive immediate `R0_GUEST` refuge, food, and physical security (`Scenario Psi`/`Rho`) with zero requirement to pay rent or show ID. |
| **2. Purely Defensive Reach into Layer 7** | **A. Hospital Visitation, Guardianship & Probate Armor:** Preventing hostile biological relatives or state courts from overriding a resident's chosen family during medical emergencies, child custody, or death.<br>**B. Anti-Bigamy / Anti-Co-Living Zoning Defense (`Mundane Semantic Camouflage`):** Protecting pluralistic and multi-generational households from restrictive municipal "unrelated persons" zoning ordinances. | • **Ricardian Legal Document Synthesis:** Automatically compiles state-compliant **Durable Medical POAs, HIPAA Authorizations, Co-Parenting Trusts, and Revocable Living Trusts** for chosen kin.<br>• **CLT Lease & Private Association Shield:** Holds residential land in a **Community Land Trust (`CLT`)** and scrubs internal kinship graphs before crossing `l6_l7.sock` so municipal authorities see only compliant leaseholders/trust beneficiaries. |
