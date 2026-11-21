# Scenario Zeta: Decommodified Community Land Trust Shelter & Regenerative Bathhouses (`SCN-ZETA-LODGE`)

* **Identifier:** `SCN-ZETA-LODGE`
* **Horizontal Wave:** Wave C (Care, Kinship, Apprenticeship & Decommodified Shelter)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Zeta governs **how the settlement shelters its permanent residents, welcomes visiting peers and crisis guests, and provides restorative thermal hydrotherapy bathhouses**. While `Scenario Xi` handles the structural engineering and timber construction of buildings, Scenario Zeta governs the **living stewardship of decommodified shelter, domestic utility baselines, hospitality, and somatic bathhouse recovery**.

Its core principles are uncompromising:
* **Decommodified Shelter (`0` Internal Rent):** Housing is held permanently off the speculative market inside a Community Land Trust (`CLT`) / Perpetual Purpose Trust (`PPT`), guaranteeing every resident a dignified, secure home and $43.2\,\mathrm{MJ/day}$ ($12\,\mathrm{kWh/day}$) utility baseline as part of $B_{\mathrm{baseline}}$.
* **100% Screenless & Unsurveilled $R_0$ Homes:** Layer 6's most important feature inside a private dwelling is **its complete absence**—zero cameras, zero smart speakers, zero occupancy sensors, and zero nagging wall dashboards inside $R_0$ suites.
* **Regenerative Hydronic Bathhouses & Closed-Loop Sanitation:** Capturing server (`Zero`/`Epsilon`), kitchen (`Gamma`), and foundry (`Theta`) waste heat into communal soaking pools, saunas, and copper drain-water heat recovery (`DWHR`), paired with odor-free negative-pressure biochar composting vaults and constructed wetlands.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy real estate and hospitality market (Layer 7), human shelter is distorted by four structural pathologies:

1. **Rent & Mortgage Servitude ($30\text{–}60\%$ of Human Labor):** Landlords and speculative real-estate funds treat basic human shelter as an appreciative financial asset—forcing workers to surrender half their waking life just to keep a roof over their heads while millions face eviction and homelessness.
2. **"Smart Home" Domestic Surveillance:** Consumer IoT thermostats, doorbell cameras, smart TVs, and voice assistants turn the private bedroom and living room into a corporate wiretap that maps intimate family routines and sleep schedules.
3. **Extractive Short-Term Rentals (Airbnbification):** Commercial short-term rental platforms hollow out residential neighborhoods into transient tourist zones, destroying neighborly cohesion and driving up local rents.
4. **Thermodynamic & Sanitary Waste:** Conventional apartments flush potable drinking water down toilets into overloaded municipal sewers and dump hot shower drain water straight into the ground without heat recovery.

---

## 3. Wide Feature Horizon

Scenario Zeta replaces landlord rent extraction and domestic IoT surveillance with five convivial shelter loops:

1. **99-Year Decommodified `CLT` Resident Stewardship (`HousingStewardshipIntent`):** Residents hold secure, renewable `CLT` stewardship occupancy of their private $R_0$ apartment, cottage, or family suite (`Scenario Omicron`) with zero internal rent or mortgage interest, adaptive right-sizing as households grow or shrink, and an unconditional $43.2\,\mathrm{MJ/day}$ electrical/hydronic comfort baseline.
2. **Screenless $R_0$ Interior Sanctuary:** All shared Layer 6 digital coordination stops at the **Commons Vestibule Kiosk**. Inside private $R_0$ homes, residents control lighting, hydronic floor warmth, and ventilation via simple tactile brass toggle switches and mechanical thermostatic valves—with **zero interior network sensors**.
3. **Federated Guest Hospitality & Zero-ID Crisis Walk-In Suites (`LodgingIntent`):** Maintains welcoming guest rooms for visiting $R_2\text{–}R_5$ guild artisans, scholars (`Scenario Iota`), and travelers, alongside dedicated unmonitored suites for Pathway 4 crisis walk-ins (`Scenario Tau`) and $< 500\,\mathrm{ms}$ unilateral `SafeHarborSeveranceIntent` exits (`Scenario Omicron`).
4. **Waste-Heat Regenerative Bathhouses & Hydrotherapy (`DWHR` $\ge 5500\,\mathrm{bp}$):** Communal bathhouses pair high-efficiency copper drain-water heat recovery coils ($\ge 55\%$ thermal capture) with waste heat from `Scenario Zero`/`Epsilon` liquid-cooled compute racks to heat cedar saunas, steam rooms, and hot mineral soaking pools—providing daily somatic recovery for stewards (`Scenario Sigma`).
5. **72-Hour Constructed Wetlands & Negative-Pressure Biochar Vaults:** Greywater passes through subsurface planted reed beds with a verified $\ge 72\text{-hour}$ hydraulic retention time (`HRT`) for orchard irrigation, while dry/foam-flush toilets feed negative-pressure ($\le -25\,\mathrm{Pa}$, $100\%$ odor-free) biochar/struvite vaults routed to `Scenario Mu` biorefining.
6. **Primary Building & Block Enveloping Node Hypervisor & Two-Speed Jurisprudence (`D4`):** Scenario Zeta acts as the **Primary Building & Block Enveloping Node Hypervisor** (*Room/Building $\to$ Street $\to$ Bioregional Watershed*) governing all indoor child nodes (`Gamma` ground-floor bakery, `Delta` nursery wing, `Lambda` music parlor, `Alpha`/`Nu` craft studios, and $R_0$ dwelling suites). The building hypervisor remains 100% domain-blind to each child node's internal **Four Constitutional Pillars**, enforcing only physical boundary leases (`EnvelopingBoundaryLeaseV50`: acoustic `cB`, electrical `mW`, thermal `mK`, and spatial `mm`) via **Two-Speed Jurisprudence**:
   * **Fast Path (Pre-Authorized Leases & Layer 2 Hazard Preemption):** Child nodes operate autonomously inside finite-TTL SI boundary envelopes, while genuine physical emergencies (such as a burst water pipe requiring an `8,200 cB` emergency wet-vac and pump override, or a smoke/fire alarm) trigger immediate hardware-timed **Layer 2 Hazard Preemption (`priority: 999`)** without waiting for a vote.
   * **Slow Path (Precedent-Compiling Semantic Diffs):** When recurring boundary friction occurs between child nodes, `col-execd` hard-blocks the out-of-lease excursion and emits a visual **Layer 6 Semantic Diff** for the building's $N_s$ residents to ratify into a permanent compiled Layer 4 diurnal precedent (e.g., quiet-hour `cB` curves or dual-sensor flood-override rules).

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`HousingStewardshipIntent`:** Requesting a long-term $R_0$ resident dwelling allocation, an adaptive household right-sizing swap (e.g., when a child is born in `Scenario Delta` or an elder prefers a zero-step ground-floor suite in `Scenario Phi`), or external envelope maintenance.
* **`LodgingIntent`:** Booking a short-term guest room for a visiting peer-node steward or allocating a zero-ID crisis sanctuary suite (`domesticCareTokenMinting: 0`).
* **`BathhouseAndLaundrySlotIntent`:** Checking quiet hours, reserving a private/family hydrotherapy soak window, or queuing high-efficiency ozone/heat-pump community laundry.
* **`SkillOfferingBarterIntent` (Optional Cultural Hospitality):** Allowing a visiting traveler or musician (`Scenario Lambda`) to offer an optional fireside concert, lecture, or craft workshop during their stay—strictly decoupled from their right to a warm bed and meal.

### B. Semantic Representations & Deterministic Read-Backs
* **Architectural Boundary of Non-Representation (`R0_INTERIOR_DARK`):** In `https://w3id.org/collective/v1/`, `col-commonsd` enforces a hard SHACL schema constraint forbidding any sensor topic, camera stream, or occupancy state inside an active $R_0$ dwelling unit. Only aggregate building-level boundary metrics (roof solar input, main cistern level, wetland `HRT`, and compost-vault negative pressure $\le -25\,\mathrm{Pa}$) exist in the knowledge graph.
* **Mandatory Lease & Maintenance Read-Back:** When a resident signs a `HousingStewardshipIntent` or requests a `Scenario Alpha`/`Xi` repair crew to enter their home for plumbing/structural work, the Vestibule Kiosk requires a **Deterministic Read-Back** specifying the exact time window during which the resident grants temporary physical access to their $R_0$ threshold.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Commons Vestibule Hospitality & Bathhouse E-Ink Kiosk** | Courtyard / Bathhouse Entry (`zeta:hospitality`) | Calm wall-mounted E-Ink slate outside private homes: handles guest room check-in, bathhouse soak schedules, laundry completion notices, and building repair tickets. |
| ***The Enveloping Node Hypervisor Lens*** | Building Vestibule Boundary Slate (`envelope:hypervisor`) | Displays active building-wide acoustic (`cB`), electrical (`mW`), thermal (`mK`), and spatial (`mm`) boundary leases across child nodes, Fast-Path L2 hazard preemption events, and Slow-Path Semantic Diff precedent ballots. |
| **Screenless $R_0$ Brass Toggle & Hydronic Dial** | Inside Private Homes (`r0:mechanical_only`) | Purely analog/tactile domestic controls (warm brass light switches, hydronic radiator dials, and the `Scenario Sigma` wooden door latch) with zero glowing screens or microphones. |
| **Zero-ID Crisis Sanctuary Key Dispenser** | Perimeter Welcoming Vestibule (`R0_GUEST`) | Dispenses a physical brass key to a warm, stocked sanctuary room for Pathway 4 walk-ins (`Scenario Tau`) or domestic exit (`Scenario Omicron`) in $< 500\,\mathrm{ms}$ with zero identity prompt. |
| **Wetland & Vault Differential-Pressure Gauge** | Utility Basement / Greenhouse (`bpmn:review`) | Coalesces 290+ building thermal, DWHR ($\ge 5500\,\mathrm{bp}$), 72h greywater `HRT`, and $\le -25\,\mathrm{Pa}$ vault ventilation checks into a single weekly 15-minute utility walk. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Across every scale, Layer 5 governance in Scenario Zeta is anchored in the unified voting weight formula scoped strictly to **Node Membership ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)\right)$$
where every building resident holds the non-decaying **Universal Civic Baseline** $B = 1$ ($10{,}000\,\mathrm{bp}$) alongside decaying domain stewardship $R_i(t)$ ($\tau_{1/2}$). When $N_s > R_i(t) + 2$, the residents' $B=1$ civic floor mathematically prevents any high-$R_i$ building maintenance or utility engineer from becoming a de facto landlord.

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ resident}$):**
   * *Context:* A solo homesteader ($V_1(t) = 1 + R_1(t)$) living in their own cottage or suburban seed house with a solar shower/bathhouse and greywater reed bed, potentially importing external Layer 5 craft or care covenants while serving as their own house envelope.
   * *Governance Model:* **Complete domestic tranquility.** The node silently maintains the $43.2\,\mathrm{MJ/day}$ thermal/electrical reserve, monitors compost-vault fan pressure ($\le -25\,\mathrm{Pa}$), and alerts the solo steward only if a physical pump or valve needs attention.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ residents & guests}$):**
   * *Context:* A co-housing house-steward or small hospitality pod ($N=2$ dyadic mutual veto when $N_s=2$, or small core $N_s$) maintaining a multi-suite house, backyard ADU cottage, and shared sauna/bathhouse for $6\text{–}20$ residents and visiting guests ($N_p > 1$).
   * *Governance Model:*
     * **Zero Landlord Power ($B=1$ Resident Floor):** Because every resident holds $B=1$ inside the house's **Enveloping Node Hypervisor** under a non-extractive `CLT` covenant with $0$ internal rent, the maintenance steward holds **zero landlord eviction power** and **zero right of unannounced entry** into any co-resident's $R_0$ suite.
     * **Rotational Bathhouse & Common-Hall Care:** Cleaning the vestibule, restocking bathhouse towels (`Scenario Nu`), and checking reed-bed filters rotate across able-bodied housemates via `Scenario Sigma` Work-Bees (`domestic_care_token_minting == 0`).
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A settlement Housing & Hospitality Guild ($N_s = 20\text{–}120+$ building residents and stewards) governing multi-story `CLT` courtyard buildings, bathhouses, and federated guest lodges across $R_1 \dots R_4$.
   * *Governance Model:*
     * **Two-Speed Enveloping Node Jurisprudence:** The building hypervisor enforces diurnal quiet-hour (`cB`), main-breaker (`mW`), and hydronic (`mK`) boundary leases across all indoor child nodes (`Gamma`, `Delta`, `Lambda`, `Alpha`), executing **Fast-Path Layer 2 Hazard Preemption** during burst-pipe or fire events and **Slow-Path Semantic Diff Precedent Compilation** ratified by the $N_s$ residents under $V_i(t) = 1 + R_i(t)$.
     * **Need-Based Adaptive Allocation & Inter-Node Reciprocal Hospitality (`R2–R4`):** Available dwellings allocate by transparent household need rather than reputational bidding, and visiting guild members enjoy free reciprocal lodging across peered watershed nodes.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Transitional Eco-Retreat / Guest-Lodge Revenue (`policy:commons_plus_l7_zero_bill`):** In Stage 1/2, offering a small subset of perimeter guest cabins to external Layer 7 eco-travelers or workshop retreats to pay down legacy land mortgages and county property taxes.<br>**B. Specialized Plumbing & Hydronic Component Procurement:** Importing EPDM pond liners, copper heat-exchanger coils, or low-wattage DC blower fans. | • **Strict Sanctuary Reserva & $\Phi_t = 0$ Auto-Shutoff:** External guest bookings are permitted only after $100\%$ of resident housing and the $\ge 500\,\mathrm{bp}$ Safe-Harbor refuge buffer are satisfied, and **automatically close** each month once mortgage/tax bills ($\Phi_t$) hit $0\text{ cents}$ and the Property Tax Escrow Endowment (`Scenario Omega`) is funded.<br>• **VLAN 40 Guest Segregation:** External paying guests remain on isolated VLAN 40 with zero access to internal `R_0/R_1` residential courtyards. |
| **2. Purely Defensive Reach into Layer 7** | **A. Irrevocable Anti-Speculation Land Lock (`CLT` / `PPT`):** Preventing any future faction, creditor, or divorce/probate court from partitioning or selling off residential homes onto the speculative real-estate market.<br>**B. Zoning, ADU & Greywater Safe-Harbor (`Mundane Semantic Camouflage`):** Protecting co-housing suites, backyard cottages, and composting/greywater systems from restrictive municipal enforcement. | • **99-Year Community Land Trust (`CLT`) & Perpetual Purpose Trust (`PPT`) Deeds:** Locks underlying land title permanently out of commercial markets.<br>• **Statutory ADU Preemption & NSF-41 / Greywater Permitting:** Files statutory Accessory Dwelling Unit (`ADU`) and state greywater/compost variances while scrubbing internal resident rosters across `l6_l7.sock`. |

---

## 7. Horizon Council v3.2 Architectural Binding (*The Architecture of Boundless Time* — D1–D8)

| Horizon Council Dimension | Scenario Zeta (`SCN-ZETA-LODGING` / `zeta_lodging`) Binding |
| :--- | :--- |
| **Node Membership Regime ($N_s$) & Unified Voice $V_i(t) = 1 + R_i(t)$** | $N_s = 20 \to 120$ building residents ($B=1$ civic baseline per resident prevents maintenance-steward landlordism when $N_s > R_i(t) + 2$). |
| **Primary Layer 6 Lens & Need-Capture Mode** | ***The Enveloping Node Hypervisor Lens*** (building spatial, acoustic, thermal, and electrical lease allocation across child nodes, keeping $R_0$ interiors 100% dark). |
| **Enveloping Node Hypervisor Lease (`cB`, `mW`, `mK`, `mm`)** | **Primary Building & Block Enveloping Node Hypervisor** governing all indoor child nodes via domain-blind `EnvelopingBoundaryLeaseV50` allocations. |
| **L4 3-Tier Cascade & Stage I$\to$III Hardware Retrofit Archetype** | Executes **Two-Speed Jurisprudence**: Fast-Path L2 burst-pipe/fire hazard preemption (`priority: 999`) + Slow-Path diurnal quiet-hour (`cB`) precedent compilation into `col-execd`. |
