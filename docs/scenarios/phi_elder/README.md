# Scenario Phi: Dignified Aging, Emeritus Sagehood & Unmediated Hospice Sanctuary (`SCN-PHI-ELDER`)

* **Identifier:** `SCN-PHI-ELDER`
* **Horizontal Wave:** Wave F (Health, Resilience, Elder Care, Justice & Terminal Decoupling)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Phi governs **how people age with dignity, transition from heavy physical labor into honored intergenerational mentorship (`T4_Emeritus_Sage`), receive non-surveilled daily care, and experience sacred, unmediated end-of-life hospice and ecological terramation**.

It directly heals two opposite distortions of legacy society:
* **Against Capitalist Elder-Warehousing:** Refuses to segregate seniors into lonely, profit-extractive nursing facilities cut off from children, gardens, and purposeful craft; elders live in accessible ground-floor `Scenario Zeta`/`Xi` courtyard suites woven directly alongside `Scenario Delta` childcare pods, `Scenario Gamma` hearths, and `Scenario Kappa` guild benches.
* **Against Corporate Rank-ism & Gerontocracy (`coercive_command_rank_bp == 0`):** Honors lived experience and multi-decade ecological memory (`wisdom_decay_rate_bp == 0`) while ensuring elders never accumulate coercive command rank over younger stewards—replacing bureaucratic seniority with sought-after Socratic counsel.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In late-stage capitalism and hierarchical institutions (Layer 7), aging and the end of life are distorted by four systemic failures:

1. **Profit-Extractive Nursing Homes & Epidemic Loneliness:** For-profit eldercare chains drain a family's life savings ($\$8{,}000\text{–}\$15{,}000/\text{month}$) to warehouse elders in understaffed, sterile wards severed from intergenerational community life.
2. **Corporate Rank-ism vs. Throwaway Ageism:** In hierarchical corporations, bureaucratic title displaces real hands-on craft experience—or conversely, older workers are discarded as "obsolete" the moment their physical speed slows down, erasing decades of institutional and ecological memory.
3. **Invasive Optical Fall-Camera Surveillance:** Commercial "aging-in-place" tech places cloud-connected video cameras inside elders' bedrooms and bathrooms, stripping basic human privacy and bodily dignity in the name of safety.
4. **Over-Medicalized ICU Deaths, Toxic Embalming & Probate Extraction:** Dying people are routinely isolated under fluorescent hospital lights hooked to unwanted machines, embalmed with formaldehyde into steel-lined vaults, and subjected to predatory probate courts that liquidate family tools (`Scenario Omega`).

---

## 3. Wide Feature Horizon

Scenario Phi replaces institutional warehousing and invasive cameras with five full-lifecycle dignity loops:

1. **Non-Decaying `T4_Emeritus_Sage` Standing (`wisdom_decay_rate_bp == 0`, `coercive_command_rank_bp == 0`):** When an aging master carpenter, midwife, forester, or engineer chooses to step back from heavy physical shifts, their hard-won `Scenario Kappa` `T4` mastery transitions into permanent **`T4_Emeritus_Sage`** standing. Their reputational voice and mentorship credentials **never decay** when physical output slows, and long-horizon projects (`Scenario Eta` 20-year forestry, `Scenario Upsilon` 50-year flood design, `Scenario Psi` restorative circles) include a **Long-Horizon Memory Consultation** with Emeritus Sages—while keeping `coercive_command_rank_bp == 0` so counsel guides by wisdom rather than command.
2. **Zero-Optical $60\,\mathrm{GHz}$ mmWave Radar Fall Safety & Induction Auto-Shutoffs:** Elder suites ban optical cameras and microphones (`0` video/audio streams). Instead, local edge $60\,\mathrm{GHz}$ millimeter-wave radar sensors detect sudden posture drops or prolonged bathroom immobility (processing raw point-clouds strictly in volatile local MCU RAM and discarding waveforms immediately) to page the neighborhood care ring (`Scenario Chi`), paired with automatic induction-stove and bath-overflow safety shutoffs.
3. **Rotational Care Dispersion Cap ($\le 2000\,\mathrm{bp}$ / $20\%$ Max per Caregiver):** Daily companionship, meal sharing, mobility assistance, and bathing support rotate across a multi-person intergenerational care pod where no single family member or volunteer carries $> 2000\,\mathrm{bp}$ ($20\%$) of weekly care hours (`domestic_care_token_minting == 0`), preventing family caregiver collapse (`Scenario Sigma`).
4. **Accessible Intergenerational Courtyards (`Delta` + `Phi` + `Lambda`):** Zero-step ground-floor suites with roll-in showers and warm hydronic floors open directly onto sunny courtyard gardens shared with `Scenario Delta` early-childhood play pods and `Scenario Lambda` acoustic parlors—making storytelling, seed sorting, and music a natural daily rhythm.
5. **100% Unmediated Home Hospice Vigils & Ecological Terramation (`Scenario Eta` Memorial Groves):** Hardware-signed Advance Healthcare Directives (`POLST` / `DNR` / chosen-kin proxies from `Scenario Omicron`) ensure an elder's end-of-life wishes are honored at home surrounded by loved ones and `Scenario Chi` palliative comfort care in a **100% Unmediated Sanctuary** (all non-palliative telemetry severed). Following death, the body returns gently to the living earth via natural organic reduction (**terramation**) or shroud burial in a `Scenario Eta` **Memorial Coppice Grove**, while the elder's DID is enshrined as a permanent `Wisdom_Anchor` (`Scenario Omega`).

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`ElderStewardshipAndWisdomIntent`:** Transitioning to `T4_Emeritus_Sage` standing, requesting a ground-floor accessible `Scenario Zeta` suite, or configuring a rotational intergenerational care pod ($\le 2000\,\mathrm{bp}$ per caregiver cap).
* **`LongHorizonMemoryConsultation`:** Inviting Emeritus Sages to attach recorded or written historical/ecological context (e.g., where the creek flooded in 1996 or how an earlier guild solved a metallurgical flaw) to a major `Scenario Eta`, `Xi`, or `Upsilon` proposal.
* **`AdvanceDirectiveAndHospiceIntent`:** Authoring and hardware-signing an Advance Healthcare Directive, palliative comfort-care plan, `Omicron` heirloom bequest, and preferred terramation/memorial grove ceremony.
* **`ElderSafetyAttestation` (Edge mmWave Reflex):** Local 1-bit posture-drop alert emitted by the edge radar MCU to the on-call care/medic ring with zero image or audio payload.

### B. Semantic Representations & Deterministic Read-Backs
* **Zero-Decay Emeritus Invariant (`wisdom_decay_rate_bp == 0`):** In `col-kmsd` and `col-commonsd`, transitioning a steward's profile to `T4_Emeritus_Sage` permanently zeroes out time-based reputational decay while enforcing `coerciveCommandRankBp: 0` and preserving $10{,}000\,\mathrm{bp}$ $B_{\mathrm{baseline}}$ priority.
* **Mandatory Advance-Directive & Hospice Read-Back:** When an elder authors or updates an `AdvanceDirectiveAndHospiceIntent`, their slate presents an unhurried, spoken-and-large-print **Deterministic Read-Back** confirming their exact palliative preferences, designated `Omicron` medical proxies, and Layer 7 statutory `POLST` equivalence before hardware signature.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **WCAG 2.2 AAA Tactile & Offline Voice Companion** | Elder Suite Armchair / Bedside (`phi:companion`) | Physical brass rotary/push buttons, ultra-high-contrast large-print E-Ink schedule, and unhurried offline spoken TTS read-back; zero passwords, zero tiny touch targets, and zero cameras. |
| **Intergenerational Care Pod & Story Circle Board** | Courtyard Vestibule (`phi:care_pod`, `delta:pod`) | Coordinates rotating tea visits, garden walks, youth storytelling hours, and mobility help across $5\text{–}10$ neighbors while enforcing the $\le 2000\,\mathrm{bp}$ single-caregiver load cap. |
| **Edge $60\,\mathrm{GHz}$ mmWave Posture Sensor (Zero-Camera)** | Bathroom / Bedroom Ceiling (`r0:edge_reflex`) | Screenless non-optical radar MCU: processes millimeter-wave reflections locally in RAM to detect falls and trigger a gentle voice check ("Are you okay, Maria?") before paging the care pod. |
| **Unmediated Hospice Vigil Sanctuary Switch** | Suite Entryway (`r0:hardware_latch`) | Physical brass switch that silences all node alerts, dims courtyard activity via a soft `No-Transit` buffer (`Scenario Beta`), and guarantees a sacred, unrecorded space for family vigils. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Across every scale, Layer 5 governance in Scenario Phi is anchored in the single unified voting weight formula scoped strictly to **Node Membership ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)\right)$$
Scenario Phi is the human cornerstone of the **Non-Decaying Universal Civic Baseline ($B = 1$, `D3`, `D8`)**: as an elder retires from heavy physical guild labor and their active physical contribution rate drops to zero ($\dot{r}_i(\tau) \to 0$, causing $R_i(t) \to 0$ over half-life $\tau_{1/2}$), their voice $V_i(t)$ **asymptotes cleanly to $B = 1$ ($10{,}000\,\mathrm{bp}$)**—guaranteeing that retired elders never lose their full constitutional citizenship or sanctuary protections.

1. **Solo Elder / Independent Living Scale ($N = 1 \implies N_s = 1\text{ elder}, N_p = 1\text{ resident}$):**
   * *Context:* An independent elder living solo in their own ground-floor cottage or suburban house who wants full autonomy without isolation.
   * *Governance Model:* **Sovereign independence backed by silent safety nets.** The elder controls their own schedule, uses the Tactile/Voice Companion for oral-history recording or guild consultations, and relies on zero-camera mmWave fall detection, automated induction shutoffs, and daily `Scenario Gamma` porch tiffin check-ins peered with next-door neighbors.
2. **Intergenerational Household / Pod Scale ($N_s = 3\text{–}8\text{ care-ring stewards}, N_p = 1\text{–}4\text{ elders served}$):**
   * *Context:* A multi-generational family or neighborhood care circle supporting aging parents or neighbors with dementia or mobility frailty ($N_p \ge 1$).
   * *Governance Model:*
     * **Breaking the Single-Daughter / Single-Spouse Burnout Trap ($\le 2000\,\mathrm{bp}$ Cap):** In Layer 7, one exhausted spouse or adult child often bears $100\%$ of dementia or mobility care until their own health breaks. Scenario Phi structurally distributes care across at least $5+$ pod members ($\le 20\%$ each), backed by `Scenario Sigma` Dual-Freeze Care Leave for family members during intensive transition weeks.
     * **Dementia-Safe Courtyard Wandering:** Rather than locking memory-impaired elders in a room or drugging them, courtyard perimeter gates and gentle garden loop paths allow safe outdoor strolling in the sun alongside `Scenario Eta` gardeners and `Scenario Delta` play pods.
3. **Multi-Steward Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A settlement Elder Council, Palliative Care Ring (`Scenario Chi`), and Memorial Grove Guild across $R_1 \dots R_4$.
   * *Governance Model:*
     * **Experience Over Corporate Rank (`Long-Horizon Memory Consultation`):** Before the assembly ratifies a major 20-year land-use or infrastructure change, `col-commonsd` invites Emeritus Sages to contribute oral or written historical context—ensuring the community learns from past decades without granting elders a gerontocratic veto over youth.
     * **Estate Liberation & Memorial Continuity (`Scenario Omega`):** Upon passing, personal $R_0$ keepsakes go to chosen kin (`Omicron`), $R_2$ workshop tools return to `Scenario Kappa` apprentices, and the elder's life story and craft notes remain in the `Scenario Iota` Alexandria Commons.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Statutory Pension, Social Security & Medicare/Hospice Entitlement Drawdown:** Ensuring aging stewards receive $100\%$ of the external retirement pensions, Social Security, and Medicare hospice equipment benefits they paid into during their Layer 7 working years.<br>**B. Specialized Mobility & Palliative Equipment GPO:** Procuring hospital beds, ceiling hoist tracks, or hearing-aid processors. | • **Personal Pension Sovereignty:** External pension checks belong to the elder in $R_0$ (or voluntarily contribute toward their household's $\Phi_t$ tax share)—never seized by a central commune treasury.<br>• **Inviolable $R_0$ Assistive Ownership:** Any walker, power wheelchair, or hearing aid provisioned is permanently titled to the elder (`Scenario Tau`). |
| **2. Purely Defensive Reach into Layer 7** | **A. Statutory `POLST`, Advance Directive & Anti-Conservatorship Armor:** Preventing external hospitals or estranged biological relatives from overriding an elder's wish to die peacefully at home or seizing control via predatory guardianship courts.<br>**B. Green Burial & Terramation Legal Compliance:** Protecting ecological body return in `Scenario Eta` Memorial Groves. | • **Ricardian `POLST` / Durable Healthcare POA Compilation:** Generates wet-ink state-statutory **Physician Orders for Life-Sustaining Treatment (`POLST`)** and **Durable Medical POAs** co-signed with `Scenario Chi` physicians.<br>• **Licensed Conservation Cemetery / Natural Organic Reduction Easement:** Dedicates Memorial Coppice Groves under state green-burial / terramation statutes and **UCEA** conservation easements, extinguishing estate-probate land liquidation forever. |

---

## 7. Horizon Council v3.2 Architectural Binding (*The Architecture of Boundless Time* — D1–D8)

| Horizon Council Dimension | Scenario Phi (`SCN-PHI-ELDER` / `phi_elder`) Binding |
| :--- | :--- |
| **Node Membership Regime ($N_s$) & Unified Voice $V_i(t) = 1 + R_i(t)$** | $N_s = 6 \to 30$ elder-care stewards & senior residents ($B=1$ non-decaying civic voice for elders whose active $\dot{r}_i(\tau) \to 0$). |
| **Primary Layer 6 Lens & Need-Capture Mode** | ***The Pantry Lens*** (medication/nutrition Arrhenius TTL) + ***The Enveloping Node Hypervisor Lens*** (sanctuary acoustic/thermal shields). |
| **Enveloping Node Hypervisor Lease (`cB`, `mW`, `mK`, `mm`)** | **Elder Sanctuary Envelope** (`4,000 cB` quiet shield, `295,150 mK` thermal comfort floor, priority backup `mW` power for assistive devices). |
| **L4 3-Tier Cascade & Stage I$\to$III Hardware Retrofit Archetype** | Human cornerstone of the **Non-Decaying Universal Civic Baseline ($B=1$, `D3`, `D8`)**: retired elders whose physical work rate $\dot{r}_i(\tau) \to 0$ never lose their full $B=1$ constitutional voice. |
