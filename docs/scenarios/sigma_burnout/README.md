# Scenario Sigma: Burnout Prevention, Somatic Homeostasis & Rotational Care (`SCN-SIGMA-BURNOUT`)

* **Identifier:** `SCN-SIGMA-BURNOUT`
* **Horizontal Wave:** Wave A (Autopoietic Meta-Core, Onboarding & Homeostasis)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Sigma is the **human and somatic immune system** of The Collective. Even the most ecologically sound community will fracture from within if conscientious members quietly burn themselves out carrying invisible maintenance, caregiving, and administrative burdens while others coast—or if well-meaning peers pressure exhausted stewards into chronic overwork.

Scenario Sigma governs the **metabolic balance between human contribution, rest, recovery, and automation**:
* **Unconditional Zero-Friction Rest (`RestShield`):** Guaranteeing that any steward can step back into rest at any moment with a single gesture, zero explanation, and zero penalty.
* **Anti-Martyrdom Circuit Breakers:** Detecting when critical guild work concentrates on a single person and automatically summoning peer relief and apprenticeship training before burnout occurs.
* **Dual-Freeze Care Leave:** Protecting stewards during illness, grief, pregnancy, caregiving, or mental-health recovery by freezing reputational and credential decay while maintaining $100\%$ survival support.
* **Fair Rotational Work-Bees & Drudgery Automation:** Sharing necessary unglamorous sanitation chores fairly without coercive token pricing, and routing repetitive physical strain into engineering automation quests (`Scenario Alpha` / `Scenario Zero`).

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

Both corporate labor markets and historical communes suffer from four destructive pathologies around work and rest:

1. **Hustle Culture & The Productivity Panopticon:** Corporate workplaces deploy keystroke loggers, productivity leaderboards, and algorithmic shift schedulers that squeeze every minute of worker attention until physical or psychological collapse.
2. **The Commune "Martyr Trap" & Invisible Emotional Labor:** In informal collectives without structural safeguards (what Jo Freeman termed *The Tyranny of Structurelessness*), a small handful of hyper-conscientious members inevitably end up scrubbing every pot, fixing every broken pump, and mediating every crisis until they burn out and leave in bitterness.
3. **Punitive Sick Leave & Care Penalties:** In Layer 7, taking months off to care for a dying parent, recover from surgery, or heal from grief risks job loss, depleted savings, and lapsed professional licenses.
4. **Commodified "Gig" Bidding for Undesirable Chores:** Platforms that use Dutch auctions or surge-pricing tokens to clear unpleasant sanitation tasks inevitably create an internal underclass—forcing newcomers or resource-poor members to do all the dirty work while senior members buy their way out.

---

## 3. Wide Feature Horizon

Scenario Sigma replaces coercive productivity and informal martyrdom with five homeostatic loops:

1. **Asymmetric Rest-vs.-Commitment Friction (`RestShield` + Physical Sanctuary Latch):** Stepping into rest requires at most **one click or sliding a physical wooden-and-brass bedroom door latch**, immediately muting $100\%$ of non-emergency notifications and enforcing bedroom sanctuary quiet ($\le 300\,\mathrm{cB}$).
2. **Local-Only $R_0$ Somatic Feel Envelopes (`ChillLoop`):** Stewards can optionally track their own subjective 1-to-5 somatic energy or fatigue purely on their personal $R_0$ device. When cumulative exertion crosses the Schmitt-trigger upper threshold ($\ge 8500\,\mathrm{bp}$), the device engages `RestShield` automatically until recovery drops below $\le 4500\,\mathrm{bp}$—without ever uploading heart-rate or biometric data to the node.
3. **The Anti-Martyrdom Concentration Breaker:** Layer 5 (`col-kmsd`) monitors task completion dispersion across essential domains. If any single steward carries $> 4000\,\mathrm{bp}$ ($40\%$) of a critical domain's load over a rolling window, the system triggers an **Anti-Martyrdom Relief Beacon**—dispatching peered `Scenario Pi` relief swarms and opening priority `Scenario Kappa` mentorship fellowships so the skill is shared.
4. **Dual-Freeze Care Leave Shield:** Filing a `CareLeaveDeclarationIntent` freezes **both** Layer 5 Reputational Wealth ($\mathcal{R}$) decay and `Scenario Kappa` safety-credential recency timers for the full duration of leave while keeping $B_{\mathrm{baseline}} = 10{,}000\,\mathrm{bp}$ intact.
5. **Rotational Work-Bees & The Drudgery-to-Automation Pipeline:** Essential unglamorous chores (compost turning, drain clearing, kitchen deep-scrubs) circulate through capability-fair rotational **Work-Bee Circles** with `domestic_care_token_minting == 0`. Whenever a task repeatedly triggers ergonomic strain or `RestIntent` drops, Layer 6 compiles a **`DrudgeryAutomationQuestIntent`** commissioning `Scenario Alpha` and `Scenario Zero` to mechanize or redesign the chore.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`RestIntent` & `UNCLAIM_GUILT_FREE`:** A blinded 1-bit signal emitted by a single tap or physical door-latch slide that returns any claimed non-safety task to the Commons Pool with zero text justification and $\Delta \mathcal{R} = 0\,\mathrm{bp}$.
* **`CareLeaveDeclarationIntent`:** Activating a multi-week or multi-month Dual-Freeze Care Leave shield for illness, perinatal recovery (`Scenario Delta`), bereavement, or family care (`Scenario Phi`).
* **`DrudgeryAutomationQuestIntent`:** Escalating a chronically straining manual workflow into a high-priority mechanical/robotics design bounty for `Scenario Alpha` and `Scenario Zero`.
* **`AntiMartyrdomReliefBeacon`:** Requesting temporary cross-pod or inter-node steward relief when a small guild faces an acute staffing bottleneck.

### B. Semantic Representations & Deterministic Read-Backs
* **1-Bit Blinded Availability Projection:** Inside `col-commonsd`, a resting steward's state appears to the rest of the node solely as `STEWARD_UNAVAILABLE_RESTING`—never distinguishing whether they are napping, grieving, ill, or on vacation, and forbidding any public "hours worked" leaderboard.
* **Schmitt-Trigger Hysteresis Bounds (`8500 bp` / `4500 bp`):** Prevents rapid on/off notification flapping by keeping a resting steward shielded until full somatic recovery is reached.
* **Mandatory Relief & Automation Read-Back:** While entering rest is instantaneous and read-back-free, broadcasting an inter-node `AntiMartyrdomReliefBeacon` or commissioning a `DrudgeryAutomationQuestIntent` uses a full **Deterministic Read-Back** showing the anonymized strain frequency and the proposed engineering or mutual-aid remedy.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Physical Wooden & Brass Sanctuary Door Latch** | Bedroom / Private Suite Door (`r0:hardware_latch`) | Screenless mechanical bolt wired to a local reed switch: sliding the latch closed physically bars entry, drops room acoustic target to $\le 300\,\mathrm{cB}$, and suppresses all Layer 6 task pings. |
| **`RestShield` Somatic Homeostat Widget** | Personal Slate / Watch / E-Ink (`sigma:rest`) | Single calm button (`Rest Now` / `Unclaim Task`) with zero guilt dialogs; holds subjective 1-to-5 somatic feel logs strictly in volatile $R_0$ memory. |
| **Work-Bee Rotation & Ergonomic Quest Board** | Hearth / Guild Hall E-Ink (`sigma:workbee`) | Displays the weekly celebratory group sanitation rotation (paired with music and tea) and lets stewards flag "This chore hurts my back—automate it!" with one tap. |
| **Hydronic Bathhouse & Decompression Scheduler** | Bathhouse Vestibule (`zeta:hospitality`, `sigma:rest`) | Automatically reserves warm waste-heat hydrotherapy soaks (`Scenario Zeta`) for paramedics (`Scenario Chi`), foundry crews (`Scenario Theta`), and stewards returning from heavy shifts. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person served}$):**
   * *Context:* A solo homesteader managing their own garden, solar array, and workshop.
   * *Governance Model:* **Self-compassionate pacing & automated load shedding.** When a solo steward slides their Sanctuary Latch or falls ill with the flu, `col-execd` automatically transitions the homestead into **Low-Maintenance Autonomic Hold**—switching irrigation and poultry feeders to automated timers, pausing non-essential workshop builds, and holding all non-critical alerts until the steward recovers.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ people served}$):**
   * *Context:* One or two primary stewards (e.g., the neighborhood cook, tool-crib maintainer, or parent caregiver) serving a household or $10\text{-person}$ pod ($N_p > 1$).
   * *Governance Model:*
     * **Protecting the Giver from the Circle:** This scale is where burnout is most dangerous in everyday life. Scenario Sigma enforces **pull-only request queues** so the $N_p$ beneficiaries cannot bombard the steward with direct demands.
     * **Automatic Circle Mobilization & Peer Relief:** When the primary steward activates `RestIntent` or `CareLeaveDeclarationIntent`, Layer 6 automatically notifies capable peers in the $R_1 / R_2$ ring (or switches the pod to simple self-serve pantry staples and neighboring hearth tiffin delivery) so the caregiver can rest without fear that their family or pod will go hungry.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* Multi-steward guilds operating kitchens, clinics, foundries, and infrastructure across a settlement ($N_p = 100\text{–}2{,}500+$).
   * *Governance Model:*
     * **Hard Single-Steward Concentration Cap ($\le 4000\,\mathrm{bp}$):** No guild is permitted to rely on a single "irreplaceable hero" for $> 40\%$ of its critical operations. Crossing that threshold automatically freezes non-essential guild expansion and funds `Scenario Kappa` apprentice stipends/rotations until at least $k \ge 3$ stewards share the competency.
     * **Constitutional Ban on Care Tokenization (`domestic_care_token_minting == 0`):** The assembly is constitutionally barred from turning sanitation, caregiving, or emotional support into paid piecework; unglamorous maintenance is shared horizontally across all able-bodied members via rotational Work-Bees until `Scenario Alpha`/`Zero` automates the drudgery away.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Ergonomic & Automation Component Procurement:** Importing mechanical actuators, lift assists, or wash-down pumps needed to fulfill a `DrudgeryAutomationQuestIntent` when local shops cannot yet fabricate them.<br>**B. Statutory Paid Family & Medical Leave (PFML / FMLA) Drawdown:** Assisting hybrid Stage 1/2 stewards who still work Layer 7 jobs in claiming every paid medical and parental leave benefit they are owed by legacy employers/states. | • **Strain-Prioritized Import Allocation:** Layer 5 grants priority in the node's external procurement budget to components that permanently eliminate repetitive human injury or high-strain manual drudgery.<br>• **Automated PFML Paperwork Generation:** `col-adversaryd` compiles state-compliant medical/family leave filings for hybrid members so they can step away from extractive Layer 7 wage jobs without losing income. |
| **2. Purely Defensive Reach into Layer 7** | **A. Volunteer & Associational Labor Safe-Harbor (*Tony and Susan Alamo Foundation*):** Preventing hostile labor regulators from misclassifying reciprocal cooperative stewardship or Work-Bees as commercial wage employment subject to payroll taxation.<br>**B. Health & Somatic Data Shielding:** Preventing external insurers or employers from accessing a steward's rest or care-leave history. | • **Non-Commercial Domestic & Associational Framing:** Enforces zero internal wage/token minting for domestic care and structures civic stewardship under **UUNAA Section 6** and cooperative member-owner bylaws.<br>• **Zero Outbound Somatic Leakage:** `RestIntent` and `CareLeaveDeclarationIntent` records never cross `l6_l7.sock`; external interfaces see zero data on individual steward availability or health. |
