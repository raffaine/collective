# Scenario Tau: Steward Genesis, Multi-Pathway Onboarding & Accessible Inclusion (`SCN-TAU-GENESIS`)

* **Identifier:** `SCN-TAU-GENESIS`
* **Horizontal Wave:** Wave A (Autopoietic Meta-Core, Onboarding & Homeostasis)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)

---

## 1. Intended Scope & Domain Vision

Scenario Tau governs **how human beings enter, participate in, and are unconditionally supported by The Collective**. It replaces both the cold bureaucratic gatekeeping of state agencies and the plutocratic "buy-in" barriers of traditional intentional communities with a warm, sensory-calm, multi-pathway onboarding protocol.

Its scope covers the full spectrum of human arrival and belonging:
* **Four Distinct Onboarding Pathways:** Welcoming part-time suburban/urban neighbors, full residential co-stewards, visiting guild artisans, and zero-device crisis sanctuary guests (`R0_GUEST`).
* **Hardware-Rooted Sovereign Identity Without Crypto-Jargon:** Minting non-exportable personal keys and social-recovery guardianship through simple physical NFC ceremonies rather than fragile 24-word seed phrases.
* **Unconditional Baseline Parity ($B_{\mathrm{baseline}} = 10{,}000\,\mathrm{bp}$):** Guaranteeing meals, shelter warmth, sanitation, and healthcare from minute one, completely decoupled from work hours or financial contribution.
* **Universal Disability & Neurodivergent Sovereignty:** Treating accessibility, sensory calm, and assistive equipment permanence as foundational constitutional rights rather than afterthoughts.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy society and conventional community models (Layer 7), joining a supportive network or accessing basic survival needs is blocked by four structural failures:

1. **Coercive Bureaucratic Interrogation & Surveillance Onboarding:** State welfare agencies and corporate platforms subject newcomers to invasive means-testing, identity dossier collection, credit checks, and biometric KYC databases that strip dignity and create permanent honey-pots for data breaches.
2. **Plutocratic Ecovillage Buy-Ins:** Many historical co-housing and off-grid communities required $\$50{,}000\text{–}\$500{,}000$ in upfront fiat capital to join—turning "sustainability" into an exclusive gated retirement club while excluding working-class families, youth, and displaced neighbors.
3. **Hostile Tech & Crypto UX Barriers:** Early decentralized protocols forced ordinary people to manage raw hexadecimal keys and 24-word mnemonic phrases, where a single lost slip of paper meant permanent lock-out from one's identity and community standing.
4. **Ableist Productivity Conditioning:** Both capitalist labor markets and poorly designed work-quota communes tie a person's right to eat, rest, or vote to their physical output—marginalizing disabled, chronically ill, neurodivergent, pregnant, or aging members.

---

## 3. Wide Feature Horizon

Scenario Tau dismantles those barriers through five core capabilities:

1. **Four-Pathway Pluralistic Inclusion:**
   * *Pathway 1 — Hybrid Suburban/Urban Neighbor:* Keeps their existing apartment and Layer 7 job while pairing a mobile Constellation lens to share `Scenario Gamma` hearth soup, `Scenario Alpha` tool libraries, and neighborhood care pods.
   * *Pathway 2 — Full Residential Co-Steward:* Moves into decommodified `Scenario Zeta` Community Land Trust (`CLT`) housing with full physical key and social-recovery setup.
   * *Pathway 3 — Visiting Guild Artisan / Seasonal Scholar:* Pairs federated $R_2\text{–}R_4$ `BBS+` skill credentials (`KappaEquivalenceClaim`) to collaborate immediately without re-proving identity.
   * *Pathway 4 — Zero-Device / Crisis Sanctuary Guest (`R0_GUEST`):* Requires **zero smartphone, zero digital identity, and zero literacy test**—issuing an unmonitored physical brass room key and full unconditional access to hot meals, bathhouses, and clinical care on the spot.
2. **Zero-Inherited-Hierarchy Parity ($R_{\mathrm{init}} = 0\,\mathrm{bp}$):** Every newcomer—whether a penniless walk-in guest, a Collective-born youth coming of age (with optional *Rumspringa* exploration support), or a founder contributing a $\$20{,}000$ bootstrap reserve—starts with exact equal Commons Chamber voting parity and $R_{\mathrm{init}} = 0\,\mathrm{bp}$ reputational wealth, preventing financial contributions from ever buying political command.
3. **In-Person NFC Vouch & 3-of-5 Shamir Social Recovery:** Replaces seed phrases with a physical $\le 40\,\mathrm{mm}$ NFC tap alongside vouching stewards, splitting key recovery across five trusted peers so a lost or broken device is effortlessly restored in person.
4. **First-Frame WCAG 2.2 AAA & Neurodivergent Accessibility:** Zero flashing animations, optional `OpenDyslexic` typography, $\ge 7.0:1$ contrast, refreshable Braille display bindings, sip-and-puff and eye-gaze switch scanning, and offline spoken TTS read-back.
5. **Inviolable $R_0$ Assistive Equipment Permanence:** Personal wheelchairs, prosthetics, AAC communication tablets, hearing aids, service animals, and sensory earmuffs fabricated or provisioned through the commons are permanently titled to the individual in $R_0$ and can never be requisitioned or audited.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
Newcomers, neighbors, and welcoming stewards interact through four primary JSON-LD intents:
* **`GenesisOnboardingIntent`:** Initiating Pathway 1, 2, or 3 onboarding via an in-person NFC vouch ceremony and binding 3-of-5 Shamir social-recovery guardians.
* **`AccessibilityAndSensoryManifest`:** A purely local ($R_0$) preference profile setting display contrast, font, audio TTS cadence, switch-scan timing, and sensory/acoustic accommodations across every Constellation lens the person touches.
* **`KappaEquivalenceClaim`:** Presenting prior trade experience or federated guild credentials to schedule a fast-track `Scenario Kappa` practical check-out without repeating beginner coursework.
* **`SanctuaryWalkInGrant` (Pathway 4):** Allocating an unmonitored guest suite and tiffin baseline count without creating any individual identity record.

### B. Semantic Representations & Deterministic Read-Backs
* **Unconditional Survival Floor (`baselineCoverageBp: 10000`):** In `https://w3id.org/collective/v1/`, every active resident or guest entity carries an immutable structural guarantee of $10{,}000\,\mathrm{bp}$ ($100\%$) access to `B_baseline` (nourishment, shelter warmth, water/sanitation, and clinical care) that no workflow or governance vote can decrement.
* **Mandatory Guardianship Read-Back:** Before a newcomer signs their `GenesisOnboardingIntent` during a vouch ceremony, the slate presents a plain-language **Deterministic Read-Back** naming their chosen 3-of-5 recovery guardians, confirming `initialReputationBp: 0`, and verifying that zero biometric or legal-name records will be broadcast outside the local ceremony.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Sensory-Calm Genesis Pairing Slate** | Welcoming Hearth / Neighbor Porch (`tau:onboard`) | Warm E-Ink or matte tablet with zero timers, zero dark patterns, tactile buttons, offline TTS voice read-back, and instant QR `#pair_token` pairing over local VLAN 30. |
| **Zero-App Physical Brass Key & Tiffin Token** | Pathway 4 Walk-In / Screenless Residents (`R0_GUEST`) | Purely physical brass door latch and passive NFC meal/tool fob requiring zero screen interaction or digital literacy. |
| **Shamir Social-Recovery Ceremony Helper** | Small Group Vouch / Key Recovery (`tau:recovery`) | Guides 3 of 5 trusted friends through a simultaneous 60-second NFC tap to restore a lost hardware token onto a replacement card. |
| **Assistive Modality Configurator** | Personal $R_0$ Device (`r0:local_only`) | Configures Braille, eye-gaze, sip-and-puff, or high-contrast profiles once and applies them automatically to every shop, kitchen, or agora slate upon badge tap. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person served}$):**
   * *Context:* A solo founder bootstrapping a Stage 1A suburban homestead lot (`Lot 402`) with their own personal hardware key and initial reserve ($\Phi_0$).
   * *Governance Model:* **Single-key self-sovereignty.** The solo founder initializes the node's root keypair and personal `AccessibilityAndSensoryManifest` with zero external quorum required, establishing the local $B_{\mathrm{baseline}}$ hearth and garden while preparing the node to peer with neighbors when ready.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ people served}$):**
   * *Context:* A household or small $2\text{–}3$ steward pod welcoming family members, children, aging parents, or $5\text{–}10$ Pathway 1 hybrid suburban neighbors who drop by for communal soup and tool sharing.
   * *Governance Model:*
     * **Frictionless Neighbor & Family Inclusion:** Children, elders, and Pathway 4 guests receive full $B_{\mathrm{baseline}}$ nourishment and warmth with zero requirement to hold keys or attend meetings. Pathway 1 neighbors pair via a simple 1-of-1 or 2-of-2 local steward vouch to access the porch tiffin locker and tool crib.
     * **Boundary Protection for the Host Steward:** Welcoming $N_p$ neighbors never turns the host steward's private bedrooms ($R_0$) into public space; guest capabilities are strictly scoped to $R_1$ common areas (porch locker, garden, garage tool crib) and bounded by the host's `Scenario Sigma` rest shields.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A neighborhood or campus settlement ($N_s = 10\text{–}100+$ stewards, $N_p = 50\text{–}2{,}500+$ residents and guests).
   * *Governance Model:*
     * **Decentralized 2-of-3 Vouch Ceremonies (No Central HR Committee):** Any two established stewards across different households can co-sign a newcomer's `GenesisOnboardingIntent` in person, preventing clique gatekeeping while blocking remote automated Sybil bots (`Scenario Rho`).
     * **Anti-Plutocracy & Universal Franchise:** Regardless of whether a member works 20 hours a week in the foundry or is bedridden and supported by care pods, every resident holds an identical $1\text{-person-}1\text{-vote}$ voice in the Commons Chamber and an unassailable $10{,}000\,\mathrm{bp}$ survival floor.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Bootstrap Capital & Tool Seeding ($\Phi_0$):** Accepting voluntary initial fiat, land, or tool contributions from founding stewards (e.g., a $\$20{,}000$ suburban bootstrap reserve) or external public disability/healthcare grants that belong to a resident.<br>**B. Hybrid Neighbor Wage Bridging (Pathway 1):** Allowing part-time neighbors to retain Layer 7 employment while shifting daily food and tool needs into the commons. | • **Zero Equity / Zero Governance Purchase (`initialReputationBp == 0`):** Layer 5 mathematically forbids fiat contributions from minting voting weight, reputational wealth ($\mathcal{R}$), or priority claims over commons housing.<br>• **Personal Benefit Non-Confiscation:** If a disabled or elder steward receives external Layer 7 statutory disability or pension benefits, those funds remain their personal $R_0$ safety buffer—never forcibly confiscated by the node. |
| **2. Purely Defensive Reach into Layer 7** | **A. Membership List Non-Existence (*NAACP v. Alabama*):** Protecting Pathway 1 hybrid neighbors and vulnerable guests from employer retaliation, landlord eviction, or state fishing expeditions.<br>**B. Guest & Volunteer Safe-Harbor:** Preventing hospitality or mutual aid from being misclassified as unlicensed boarding houses or commercial employment. | • **Structural Data Non-Existence:** `col-kmsd` and `col-commonsd` never transmit member rosters, `did:peer` graphs, or guest counts across `l6_l7.sock`; `col-adversaryd` holds zero member tables to subpoena.<br>• **UUNAA Section 6 & Gratuitous Hospitality Shield:** Frames onboarding within private unincorporated association covenants and non-commercial domestic hospitality exemptions. |
