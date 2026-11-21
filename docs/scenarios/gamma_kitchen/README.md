# Scenario Gamma: The Communal Hearth, Keyline Agroecology & Unconditional Nourishment (`SCN-GAMMA-KITCHEN`)

* **Identifier:** `SCN-GAMMA-KITCHEN`
* **Horizontal Wave:** Wave B (Suburban Survival, Energy, Fabrication & Transit Seed)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Gamma governs how a settlement **grows, harvests, ferments, cooks, and shares daily nourishment**. It closes the metabolic loop from backyard mirepoix gardens, keyline market beds, rotational silvopasture micro-flocks, and mycological fruiting cellars to high-efficiency communal kitchens, lacto-fermentation larders, and insulated neighborhood tiffin delivery.

At its heart is a simple constitutional promise: **nourishment is an unconditional right of existence ($B_{\mathrm{baseline}} = 10{,}000\,\mathrm{bp}$), never a priced commodity**. Whether a resident is a master machinist, a resting new parent, a child, or a walk-in guest, hot, nutrient-dense seasonal meals (anchored on Day 1 by the *Pea Soup & Bean Stew Baseline* and scaling into full bioregional culinary arts) are shared freely with zero internal prices, zero meal tokens, and cryptographic protection against food allergies and pathogens.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy industrial food system (Layer 7), daily nutrition is trapped in four interlocking crises:

1. **Supermarket Monopoly & Caloric Precarity:** Households spend $15\text{–}30\%$ of their post-tax income on groceries shipped thousands of kilometers through fragile cold chains, where corporate price-gouging turns basic nutrition into a source of daily financial anxiety.
2. **Atomized Domestic Kitchen Drudgery & Food Waste:** Every evening, dozens of exhausted households on the same street separately buy, prep, cook, and scrub dishes on inefficient stoves—wasting $30\text{–}40\%$ of fresh produce to spoilage in individual refrigerators while multiplying thermal energy consumption tenfold.
3. **Soil Depletion & Grain-Fed Feedlot Cruelty:** Industrial monoculture strips topsoil and leaches synthetic nitrogen into rivers, while factory farms feed human-edible grain and soy to confined animals alongside prophylactic antibiotics.
4. **Opaque Allergen & Pathogen Hazards:** Industrial processing and commercial catering rely on paper clipboards and opaque ingredient supply chains where a single cross-contaminated batch or cooling excursion triggers severe anaphylaxis or foodborne illness.

---

## 3. Wide Feature Horizon

Scenario Gamma replaces industrial food dependence with six integrated agro-culinary loops:

1. **Unconditional Hearth Baseline & Pluralistic Culinary Arts:** Provides reliable daily staples (rich legume stews, sourdough breads, roasted roots, bone or mushroom broths, fresh greens, and seasonal ferments) at exact zero internal price (`domesticCareTokenMinting: 0`), while supporting diverse cultural cuisines, feast days (`Scenario Lambda`), and household self-cooking ingredients.
2. **Keyline Market Gardens & Guild Companion Planting:** Polyculture beds pairing nitrogen-fixing *Fabaceae* (peas, favas, pole beans) with root vegetables, brassicas, herbs, and perennial berry hedgerows irrigated by gravity-fed rainwater and greywater reed beds (`Scenario Beta`).
3. **Trophic Non-Competition Micro-Flocks & Silvopasture:** Pasture hens, ducks, and small ruminants forage strictly on garden trimmings, rotational orchard understory (`Scenario Eta`), Black Soldier Fly Larvae (`BSFL`) grown on pre-consumer kitchen scraps, and calcined bone-ash calcium grit—with **zero human-edible grain competition** (`human_edible_grain_fed_mg == 0`).
4. **Precision Fermentation, Canning & Root Cellaring:** Ceramic lacto-fermentation crocks (sauerkraut, kimchi, miso, tempeh), pressure-retort canning in `316L` stainless vessels, solar dehydrators, and evaporative/earth-bermed root cellars that turn autumn harvest gluts into year-round larder security.
5. **Hardware-Enforced Bigelow Pasteurization & Acidification Safety:** Physical thermal and pH gates prevent any batch from being dispensed until pathogen lethality is mathematically verified.
6. **Zero-Knowledge Allergen Tiffins & Closed-Loop Compost Return:** Reusable 304-stainless insulated tiffin carriers tagged with passive NFC chips circulate via `Scenario Beta` porch milk-runs, returning rinsed carriers and post-meal organic scraps to `Scenario Mu` thermophilic compost and `BSFL` bioconversion.
7. **Thermodynamic Split Between *The Pantry* and *The Bakery / Hearth* & The Dual-Loop Bakery (`D4`, `D5`):** Distinguishes household perishable-entropy tracking in ***The Pantry Lens*** (Arrhenius TTL decay curves, stock drawdown sensors, pre-spoilage neighbor surplus offers, and **automatic weekend-detour demand throttling** with zero calendar surveillance via short-lived auto-expiring Layer 3 pickup intent leases `AnonymousAppetiteLeaseV50`) from culinary production in ***The Bakery / Hearth Craft Lens***. While governance voting weight $V_i(t) = 1 + R_i(t)$ is scoped strictly to **Node Membership ($N_s$ active cooks/bakers)** so 500 bread consumers ($N_p$) never micromanage starter hydration, anonymous cohort appetite weather-maps guide an **$8{,}500\,\mathrm{bp}$ ($85\%$) Verified Staple / $1{,}500\,\mathrm{bp}$ ($15\%$) Compositional *Montage* Experiment Split** (allowing unrequested culinary creations whose resonance is validated by Layer 2 shelf-pickup load cells).

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`DietaryNeedIntent`:** Registering a household's daily tiffin portion count, culinary preferences, and blinded zero-knowledge allergen/ethical exclusion masks (e.g., peanut, celiac, sesame, vegan, Halal, Kosher, Jain).
* **`MealOffering`:** Publishing a cooked hearth batch or larder surplus with its recursive ingredient lineage, verified pasteurization/pH attestation, and `domesticCareTokenMinting: 0`.
* **`AgroPastoralBatchAttestation`:** Recording seed-to-harvest bed provenance, irrigation water quality, and zero-grain pasture rotation compliance.
* **`CompostRoutingIntent`:** Routing trimmings, spent broth bones, and greywater back into `BSFL` larvae bins, bone-ash calcination, and `Scenario Mu` hot compost piles.

### B. Semantic Representations & Deterministic Read-Backs
* **Stoichiometric Cultivar-to-Recipe Graphs (`https://w3id.org/collective/v1/`):** Links botanical cultivars (`fabaceae_cultivars.jsonld`) and soil moisture/nutrient budgets directly to kitchen recipe BOMs (`pea_to_soup_needs.jsonld`), allowing the hearth to plan seasonal plantings around actual neighborhood nutritional needs.
* **Bigelow $F_0$ Thermal Lethality & Botulism pH Invariants:** `col-commonsd` and Layer 4 block a batch from transitioning to `DISPENSE_READY` unless Layer 2 PT100 probes attest core hold temperature $\ge 347{,}150\,\mathrm{mK}$ ($74.0^\circ\mathrm{C}$) continuously for $\ge 120{,}000\,\mathrm{ms}$ (or $\ge 394{,}150\,\mathrm{mK}$ / $121.0^\circ\mathrm{C}$ for low-acid retort canning)—automatically resetting the timer if temperature dips—and verify shelf-stable ferments at $\le 4{,}600\,\mathrm{mpH}$ ($\mathrm{pH} \le 4.6$).
* **Zero-Knowledge `BBS+` Anaphylactic Set-Disjointness Proof:** Diners never publish medical diagnoses to a public kitchen roster. Instead, tapping a diner's NFC card against a tiffin locker evaluates a local `BBS+` set-disjointness proof against the tiffin's signed recursive `recipe_bom` ($(\text{batch\_mask} \mathbin{\&} \text{diner\_exclusion\_mask}) == 0$). Any allergen overlap triggers an immediate mechanical **Anaphylactic Hard-Stop**.

### C. Constellation of Application Lenses & Screenless Modalities
In a mature neighborhood hearth running $96\text{–}480$ concurrent biological and thermal workflows (staggered garden beds, 24-hour broth simmers, kraut pH drops, and tiffin washes), Layer 6 coalesces routine checks into **one morning garden walk and one afternoon prep shift**:

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| ***The Pantry* (Perishable Entropy & Detour Lens)** | Household Larder / Fridge Slate (`pantry:steward`, `r0:local_only`) | Tracks Arrhenius biological TTL curves and shelf drawdowns; emits ZK-stripped anonymous cohort appetite vectors and short-lived (`12h` TTL) pickup leases to Layer 3; automatically throttles bakery orders during **spontaneous weekend detours** with zero calendar/GPS surveillance and posts pre-spoilage Ring $R_1$ surplus offers. |
| ***The Bakery / Hearth* Culinary Craft Lens (IP67 E-Ink)** | Mounted beside Induction Stockpots & Ovens (`hearth:steward`, `bpmn:review`) | Steam-proof display localizing craft idioms (baker's hydration `%`, `cups`, `°F`/`°C`) into canonical 64-bit SI (`bp`, `mg`, `mK`); merges Layer 3 anonymous appetite weather-maps with Layer 2 shelf-clearance load cells to schedule **$85\%$ verified staples / $15\%$ *montage* culinary experiments** alongside Bigelow $F_0$ gauges. |
| **Garden Keyline & Micro-Flock Fieldbook** | Offline Domain-B Tablet (`agro:cultivar`, `crdt:read`) | Coalesces soil-moisture alerts, *Fabaceae* trellis harvests, `BSFL` feedings, and 30-day pasture rest rotations into a single 20-minute morning walk. |
| **Zero-Knowledge Allergen NFC Tiffin Locker** | Porch / Hearth Vestibule (`tiffin:dispense`) | Screenless or small E-Ink solenoid latch: tap badge to unlock a safe, hot stainless tiffin; flashes a zero-LLM crimson hard-stop and stays locked if an allergen conflict exists. |
| **Waterproof QR Batch Traveler Tag** | Clipped to Fermentation Crocks & Stockpots | Stainless or washable synthetic QR tag clipped to vessels for zero-screen scanning of pH and temperature milestones. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Across all scales, Layer 5 scopes voting weight $V_i(t) = B + R_i(t) = 1 + R_i(t)$ ($V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)$) strictly to **Node Membership ($N_s$ active hearth/bakery stewards)** while coupling the wider community ($N_p$ eaters) through zero-surveillance demand vectors:

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person served}$):**
   * *Context:* A single homesteader tending a backyard mirepoix garden, a 4-hen micro-flock, and a personal pressure-canning/fermentation pantry.
   * *Governance Model:* **Zero meetings, pure culinary & food-safety companion ($V_1(t) = 1 + R_1(t) = 100\%$).** Layer 6's *The Pantry* and *Hearth* lenses coalesce garden watering, Arrhenius shelf-life tracking, and ferment checks into a calm daily round while enforcing hard physical safety timers on pressure canning ($\ge 394{,}150\,\mathrm{mK}$) and fermentation pH ($\le 4{,}600\,\mathrm{mpH}$).
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{–}5\text{ cooks/bakers}, N_p > 1\text{ people served}$):**
   * *Context:* A small neighborhood hearth or bakery pod ($N_s = 1\text{–}5$) batch-cooking soup tiffins and sourdough loaves for a street or $50\text{–}500$ neighborhood eaters ($N_p > 1$).
   * *Governance Model:*
     * **The Dual-Loop Bakery ($N_s$ Craft Voice vs. $N_p$ Anonymous Appetite):** The $N_p$ bread and soup consumers never vote on starter hydration or oven administration ($V_i(t) = 1 + R_i(t)$ belongs strictly to the $N_s$ active hearth stewards). Instead, residents' local *Pantry* lenses emit BBS+ ZK-stripped cohort appetite vectors and short-lived pickup leases (`AnonymousAppetiteLeaseV50`) that auto-expire if a family takes a spontaneous weekend trip—eliminating food waste without nagging confirmation notifications.
     * **85% Verified Staples / 15% Compositional *Montage*:** Hearth stewards dedicate $8{,}500\,\mathrm{bp}$ ($85\%$) of batch capacity to verified neighborhood staples and reserve $1{,}500\,\mathrm{bp}$ ($15\%$) for unrequested culinary experiments (*montage*), validated organically by Layer 2 shelf-pickup load cells.
     * **Automated Allergen Shield & Work-Bee Dish Rotation:** The `recipe_bom` is logged once on the Hearth Slate so NFC tiffin latches enforce anaphylactic hard-stops automatically, while tiffin washing rotates across the $N_p$ circle via `Scenario Sigma` Work-Bees.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A full cooperative commissary hearth and agroecology guild ($N_s = 8\text{–}50+$ stewards) nourishing $150\text{–}2{,}500+$ residents and exchanging seasonal surpluses across watershed nodes ($R_2 \dots R_4$).
   * *Governance Model:*
     * **Bicameral Ecological & Nutritional Floor ($N_s > R_i(t) + 2$):** Within the guild, $V_i(t) = 1 + R_i(t)$ pairs active culinary leadership with a non-decaying $B = 1$ baseline that prevents single-operator capture, while the **Commons Chamber** ratifies constitutional ecological floors (`human_edible_grain_fed_mg == 0`, zero single-use plastics).
     * **Inter-Node Seasonal Clearing (`BCC`):** Surplus autumn apples, dried beans, or olive oil clear across neighboring valleys via zero-sum Pairwise Bilateral Clearing Channels (`BCC`) and `Scenario Beta` cold-chain milk-runs, while internal meals remain $100\%$ free (`0` tokens).

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Bulk Dry-Good & Mineral Precursor GPO (`ProcurementIntent`):** Pooling neighborhood wholesale orders for salt, spices, olive oil, or dry legumes not yet grown at $100\%$ local self-sufficiency.<br>**B. Transitional Cottage-Food & Market Surplus Sales (`policy:commons_plus_l7_zero_bill`):** Selling surplus sourdough, ferments, or jams at local farmers' markets to pay down municipal water/property tax bills. | • **Caloric & Micronutrient Dependency Tracking ($D_t$):** Every imported food item is logged by mass and caloric exergy on the Legacy Dependency Register, guiding `Scenario Eta` orchard expansion and `Scenario Pi` agricultural growth beacons.<br>• **Strict $F_{\mathrm{eco}} + B_{\mathrm{baseline}}$ Floor & $\Phi_t = 0$ Auto-Shutoff:** Layer 5 forbids exporting a single gram of food to Layer 7 until $100\%$ of internal resident tiffin baselines are met, and **automatically halts commercial cottage-food sales** the instant monthly Layer 7 bills hit $0\text{ cents}$. |
| **2. Purely Defensive Reach into Layer 7** | **A. Private Food-Club & Right-to-Garden Shielding:** Protecting neighborhood tiffin sharing and front-yard gardens from commercial restaurant licensing harassment or HOA lawn mandates.<br>**B. Household Dietary Privacy (`Mundane Semantic Camouflage`):** Preventing grocers or insurers from profiling residents' religious diets or medical allergies. | • **UUNAA Section 6 & Cottage-Food Carapace:** Structures neighborhood meal sharing as a private, non-commercial unincorporated association food club (`0` retail sales inside the mesh) and uses **State Cottage Food / IRC Subchapter T** wrappers for external commissary sales.<br>• **Aggregated GPO Scrubbing:** Outbound wholesale orders strip all household `DietaryNeedIntent` profiles, emitting only bulk commodity purchase orders across `l6_l7.sock`. |

---

## 7. Horizon Council v3.2 Architectural Binding (*The Architecture of Boundless Time* — D1–D8)

| Architectural Dimension | Canonical v3.2 Binding for `SCN-GAMMA-KITCHEN` (`gamma_kitchen`) |
| :--- | :--- |
| **1. Node Membership Regime ($N_s$) & Unified Voting Formula $V_i(t) = 1 + R_i(t)$ (`CCI-2`)** | $N_s = 5$ active bakers/cooks ($V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)$), strictly separated from $N_p = 500$ neighborhood diners (**The Dual-Loop Bakery**, `D4`). Loop 1 reserves internal process voice to the $N_s$ culinary stewards; Loop 2 captures $N_p$ community appetite via BBS+ zero-knowledge-stripped cohort vectors (`AnonymousAppetiteLeaseV50`), pairing **$8{,}500\,\mathrm{bp}$ ($85\%$) verified staples** with a **$1{,}500\,\mathrm{bp}$ ($15\%$) compositional *montage* reserve**. |
| **2. Primary Layer 6 Application Lenses & Edge SI Localization (`CCI-1`, `CCI-3`)** | Explicit thermodynamic split between ***The Pantry Lens*** (household Arrhenius perishable TTL decay, spontaneous **weekend-detour demand throttling** via 12-hour auto-expiring pickup leases, and pre-spoilage Ring $R_1$ surplus offers) and ***The Bakery / Hearth Craft Lens*** (translating baker's hydration `%`, `cups`, and `°F`/`°C` at the human edge into canonical 64-bit SI `bp`, `mg`, `mL`, and `mK`). |
| **3. Enveloping Node Spatial Hypervisor Lease (`CCI-4`)** | Operates inside the **Ground-Floor Hearth Envelope** wrapped by the building/street Enveloping Node Hypervisor: acoustic ceiling $\le 6{,}800\,\mathrm{cB}$ ($68.0\,\mathrm{dBA}$ during active prep), electrical feeder lease $6{,}000{,}000\,\mathrm{mW}$ ($6.0\,\mathrm{kW}$ induction/deck oven draw), and flue/hydronic thermal exhaust $+4{,}500\,\mathrm{mK}$ recovered into domestic hot-water preheat. |
| **4. Open-Source Franchising, L4 3-Tier Cascade & Hardware Fleet Progression (`CCI-5`, `CCI-6`)** | Instantiates turnkey culinary blueprints from the 3 Sovereign Libraries ("Speedee Service System Without the Rent"); resolves smaller local ovens via **Tier 1 SI Parameter Scaling** ($20\,\mathrm{kg} \to 2 \times 10\,\mathrm{kg}$ dough batches) or **Tier 2 Macro-Activity Substitution** (griddle $\leftrightarrow$ convection deck) and halts on **Tier 3 True-Constraint Hard-Blocks** if cold storage $> 277{,}150\,\mathrm{mK}$ ($4.0\,^\circ\mathrm{C}$). Layer 3 $\lambda_{\mathrm{ERC}}$ minimization drives voluntary convergence onto standard **Gastronorm GN 1/1 ($530 \times 325\,\mathrm{mm}$) stainless pans**, while appliances progress from **Stage I Scavenged Commercial Ovens** (external CT/thermocouple shims) $\to$ **Stage II Right-to-Repair Open PID Retrofits** (minting Layer 3 Proof of Stewardship) $\to$ **Stage III Native Open-Spec Induction Hearths**. |
