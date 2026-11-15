# Scenario Gamma: The Communal Hearth, Keyline Agroecology & Unconditional Nourishment (`SCN-GAMMA-KITCHEN`)

* **Identifier:** `SCN-GAMMA-KITCHEN`
* **Horizontal Wave:** Wave B (Suburban Survival, Energy, Fabrication & Transit Seed)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)

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
| **IP67 Washable Hearth & Pantry E-Ink Slate** | Mounted beside Induction Stockpots (`hearth:steward`, `bpmn:review`) | Steam-proof, wet-hand-friendly high-contrast display showing step timers, live Bigelow $F_0$ thermal-death gauges, and `316L` Clean-in-Place (`CIP`) verification. |
| **Garden Keyline & Micro-Flock Fieldbook** | Offline Domain-B Tablet (`agro:cultivar`, `crdt:read`) | Coalesces soil-moisture alerts, *Fabaceae* trellis harvests, `BSFL` feedings, and 30-day pasture rest rotations into a single 20-minute morning walk. |
| **Zero-Knowledge Allergen NFC Tiffin Locker** | Porch / Hearth Vestibule (`tiffin:dispense`) | Screenless or small E-Ink solenoid latch: tap badge to unlock a safe, hot stainless tiffin; flashes a zero-LLM crimson hard-stop and stays locked if an allergen conflict exists. |
| **Waterproof QR Batch Traveler Tag** | Clipped to Fermentation Crocks & Stockpots | Stainless or washable synthetic QR tag clipped to vessels for zero-screen scanning of pH and temperature milestones. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person served}$):**
   * *Context:* A single homesteader tending a backyard mirepoix garden, a 4-hen micro-flock, and a personal pressure-canning/fermentation pantry.
   * *Governance Model:* **Zero meetings, pure culinary & food-safety companion.** Layer 6 coalesces garden watering and ferment checks into a calm daily round while enforcing hard physical safety timers on pressure canning ($\ge 394{,}150\,\mathrm{mK}$) and fermentation pH ($\le 4{,}600\,\mathrm{mpH}$) so the solo homesteader enjoys effortless year-round food security without spoilage risk.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ people served}$):**
   * *Context:* One or two passionate neighborhood cooks/gardeners batch-cooking soup and stew tiffins $2\text{–}3$ days a week for a household, an elder next door (`Scenario Phi`), a postpartum family (`Scenario Delta`), or $5\text{–}12$ peered suburban homes ($N_p > 1$).
   * *Governance Model:*
     * **Automated Allergen & Pathogen Liability Shield:** When cooking for $N_p$ neighbors, the cook does not have to memorize 30 individual dietary restrictions—the `recipe_bom` is logged once on the Hearth Slate, and the NFC tiffin latches automatically prevent any neighbor with a matching allergen from opening an unsafe tiffin.
     * **No Unpaid Kitchen Servitude:** Portion requests (`DietaryNeedIntent`) aggregate before prep begins, Dish/Tiffin washing and garden weeding rotate across the $N_p$ circle via `Scenario Sigma` Work-Bees, and if the primary cook taps `Rest Now`, `col-commonsd` seamlessly shifts the pod to pantry staples or an adjacent hearth.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A full cooperative commissary hearth and agroecology guild ($N_s = 8\text{–}50+$ stewards) nourishing $150\text{–}2{,}500+$ residents and exchanging seasonal surpluses across watershed nodes ($R_2 \dots R_4$).
   * *Governance Model:*
     * **Bicameral Ecological & Nutritional Floor:** The **Commons Chamber** ($N_p$ diners) ratifies ethical and ecological guardrails—such as the absolute ban on feeding human-edible grain to livestock (`human_edible_grain_fed_mg == 0`), zero single-use plastics, and minimum seasonal variety—while the **Stewardship Chamber** ($N_s$ cooks and growers) governs crop rotations, `316L` `CIP` sanitation schedules, and recipe craft.
     * **Inter-Node Seasonal Clearing (`BCC`):** Surplus autumn apples, dried beans, or olive oil clear across neighboring valleys via zero-sum Pairwise Bilateral Clearing Channels (`BCC`) and `Scenario Beta` cold-chain milk-runs, while internal meals remain $100\%$ free (`0` tokens).

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Bulk Dry-Good & Mineral Precursor GPO (`ProcurementIntent`):** Pooling neighborhood wholesale orders for salt, spices, olive oil, or dry legumes not yet grown at $100\%$ local self-sufficiency.<br>**B. Transitional Cottage-Food & Market Surplus Sales (`policy:commons_plus_l7_zero_bill`):** Selling surplus sourdough, ferments, or jams at local farmers' markets to pay down municipal water/property tax bills. | • **Caloric & Micronutrient Dependency Tracking ($D_t$):** Every imported food item is logged by mass and caloric exergy on the Legacy Dependency Register, guiding `Scenario Eta` orchard expansion and `Scenario Pi` agricultural growth beacons.<br>• **Strict $F_{\mathrm{eco}} + B_{\mathrm{baseline}}$ Floor & $\Phi_t = 0$ Auto-Shutoff:** Layer 5 forbids exporting a single gram of food to Layer 7 until $100\%$ of internal resident tiffin baselines are met, and **automatically halts commercial cottage-food sales** the instant monthly Layer 7 bills hit $0\text{ cents}$. |
| **2. Purely Defensive Reach into Layer 7** | **A. Private Food-Club & Right-to-Garden Shielding:** Protecting neighborhood tiffin sharing and front-yard gardens from commercial restaurant licensing harassment or HOA lawn mandates.<br>**B. Household Dietary Privacy (`Mundane Semantic Camouflage`):** Preventing grocers or insurers from profiling residents' religious diets or medical allergies. | • **UUNAA Section 6 & Cottage-Food Carapace:** Structures neighborhood meal sharing as a private, non-commercial unincorporated association food club (`0` retail sales inside the mesh) and uses **State Cottage Food / IRC Subchapter T** wrappers for external commissary sales.<br>• **Aggregated GPO Scrubbing:** Outbound wholesale orders strip all household `DietaryNeedIntent` profiles, emitting only bulk commodity purchase orders across `l6_l7.sock`. |
