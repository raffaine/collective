# Scenario Theta: Secondary Foundry, XRF Scrap Metallurgy & Solar Crucible Batching (`SCN-THETA-FOUNDRY`)

* **Identifier:** `SCN-THETA-FOUNDRY`
* **Horizontal Wave:** Wave D (Regenerative Land, Extraction, Materials & Architecture)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Theta is the **heavy primary-material and metallurgical backbone** of The Collective. While `Scenario Alpha` machines and assembles parts, Scenario Theta **refines, melts, casts, forges, and synthesizes the raw structural alloys, silicate glass, ceramic refractories, and bulk mineral reagents** that make `Scenario Alpha`, `Beta`, `Xi`, and `Chi` possible without relying on global mining corporations.

Because melting steel, bronze, aluminum, and glass requires high temperatures ($900\text{–}1{,}900\,\mathrm{K}$) and serious physical hazard stewardship, Scenario Theta embeds **strict thermodynamic restraint** and **multi-person hardware safety** into its core architecture:
* **The Three-Tier Material Sourcing Hierarchy:** Always exhausting (1) direct dimensional reuse of salvaged stock (`Scenario Mu`) before permitting (2) solar-batched secondary scrap remelting, and locking (3) virgin geospheric mineral extraction behind an extraordinary watershed supermajority and $10\times$ ecological replacement cost surcharge.
* **Sub-250ms Two-Person Enclave Pour Interlocks:** Ensuring no induction furnace tilt-pour, high-current electrowinning cell, or caustic reagent reactor can be actuated without simultaneous physical co-presence of two certified stewards and verified fume-scrubber negative pressure.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy metallurgical, glass, and bulk chemical industry (Layer 7), primary material production is built on four destructive pillars:

1. **Open-Pit Virgin Mining & Tailings Devastation:** Even though humanity has already mined billions of tons of iron, copper, and aluminum sitting in scrap yards and retiring structures, corporate commodities markets subsidize open-pit virgin mining that strips mountains and leaves toxic cyanide and acid-mine tailings dams.
2. **Fossil-Coal Blast Furnaces & Grid Peak Drain:** Legacy foundries burn metallurgical coal or draw continuous megawatts from dirty utility grids 24/7 rather than synchronizing high-enthalpy melts with renewable solar, wind, or micro-hydro peaks.
3. **Downcycling from Sloppy Scrap Mixing:** Conventional scrap shredders mix copper wiring with steel and tramp elements (bismuth, lead, cadmium, antimony), degrading recycled metal into brittle, low-grade rebar instead of sorting and refining scrap back into high-purity aerospace/machining alloys.
4. **Foundry Worker Injury & Unscrubbed Fume Plumes:** Commercial foundries in deregulated zones expose workers to molten metal splash, silicosis, and heavy-metal fumes while venting acid gases and particulates into working-class neighborhoods.

---

## 3. Wide Feature Horizon

Scenario Theta replaces extractive mining and dirty blast furnaces with five precision material loops:

1. **Enforced Three-Tier Material Hierarchy:**
   * *Tier 1 — Direct Dimensional & Structural Reuse (`Tier1RepurposeMatchIntent`):* Before melting a single gram of metal or glass, `col-commonsd` queries `Scenario Mu`'s salvaged I-beams, shafting, plate stock, and glass bottles for a direct dimensional match ($< 5\%$ of remelt exergy).
   * *Tier 2 — Secondary Technospheric Scrap Remelt & Electrowinning (`PyrometallurgyAndElectrowinIntent`):* Clean induction crucible melting of sorted steel, ductile iron, silicon bronze, brass, and aluminum scrap, plus hydrometallurgical copper/precious-metal electrowinning from e-waste (`Scenario Mu`)—gated at $\ge 8500\,\mathrm{bp}$ ($85\%$) crucible fill efficiency and scheduled strictly during peak solar/hydro surplus.
   * *Tier 3 — Last-Resort Geospheric Extraction (`LastResortGeosphericExtractionIntent`):* Local quarrying of abundant clays, silica sand, limestone, or iron bog ore only when Tier 1 and Tier 2 stocks are exhausted across the entire $R_4$ bioregion, carrying a $10\times \lambda_{\mathrm{ERC}}$ ecological restoration mandate.
2. **Optical/XRF Alloy Metrology & Tramp-Element Gating:** Every crucible charge and molten sample is verified via X-Ray Fluorescence (`XRF`) or Optical Emission Spectroscopy (`OES`) to ensure alloy stoichiometry and strict tramp-element ceilings (`Pb`, `Cd`, `Bi` bounds) before pouring.
3. **Silicate Glass & Technical Ceramic Kilns:** Remelting cullet glass into standardized `Scenario Gamma` canning jars, `Scenario Zeta` architectural glazing, and `Scenario Chi`/`Iota` borosilicate labware, alongside firing refractory bricks and stoneware crocks.
4. **Bulk Inorganic & Chlor-Alkali Reagent Synthesis:** Solar-powered membrane electrolysis of brine and calcination of limestone/wood-ash to produce essential bulk cleaning and refining reagents (sodium hydroxide lye for `Scenario Nu`/`Gamma` soaps, sodium hypochlorite disinfectant, calcium hydroxide lime mortar for `Scenario Xi`, and green hydrogen/oxygen for welding torches).
5. **Two-Person Dead-Man Pour Gate & Cascade Waste-Heat Recovery:** High-temperature furnace pours require a simultaneous $\le 250\,\mathrm{ms}$ two-person hardware enclave sign-off and verified baghouse/wet-scrubber differential pressure, while furnace cooling jackets dump high-grade thermal exergy into `Scenario Eta` lumber kilns and `Scenario Beta` district heating.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`Tier1RepurposeMatchIntent`:** Automatically triggered whenever a `Scenario Alpha` or `Scenario Xi` blueprint requests structural metal or glass—searching `Scenario Mu` salvage inventory for an intact beam, shaft, or vessel before authorizing any furnace melt.
* **`PyrometallurgyAndElectrowinIntent`:** Scheduling a solar-batched Tier 2 induction crucible melt, sand/investment casting run, glass-kiln firing, or copper electrowinning batch at $\ge 8500\,\mathrm{bp}$ crucible fill efficiency.
* **`AlloyAndReagentAssayAttestation`:** Recording `XRF`/`OES` elemental composition and purity bounds on cast ingots, bar stock, borosilicate glass, or bulk reagents.
* **`LastResortGeosphericExtractionIntent`:** Proposing a rare Tier 3 clay/mineral quarry draw to the $R_4$ Bioregional Assembly with full $10\times \lambda_{\mathrm{ERC}}$ land-remediation escrow.

### B. Semantic Representations & Deterministic Read-Backs
* **Three-Tier Preemption in `https://w3id.org/collective/v1/`:** `col-commonsd` refuses to validate a `PyrometallurgyAndElectrowinIntent` unless the intent includes a signed `tier1CacheMissProof` demonstrating that no suitable intact salvaged stock exists within the local or adjacent $R_2$ yard, and that cumulative batch mass meets $\ge 8500\,\mathrm{bp}$ of crucible capacity.
* **Mandatory Pour & Tier-3 Read-Back:** Before energizing an induction furnace or unlocking the tilt-pour actuator, the Foundry Console displays a plain-language **Deterministic Read-Back** of the charge mass, `XRF` tramp-element check, solar microgrid `SoC_bp` headroom, wet-scrubber pressure status, and the two co-signing stewards' `Scenario Kappa` `T3_Foundry` credentials.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Foundry Metrology & Crucible Batch Console** | Foundry Control Pulpit (`theta:foundry`, `bpmn:review`) | High-contrast, heat-shielded E-Ink/industrial slate: aggregates small `Scenario Alpha` casting requests across the week into one $\ge 8500\,\mathrm{bp}$ full-crucible solar-peak melt; displays live `XRF`/`OES` alloy spectra. |
| **Two-Person Dead-Man Pour Enclave Switches** | Furnace Tilt Station (`tool:unlock`, `theta:pour`) | Screenless dual-station physical dead-man triggers spaced $2.5\,\mathrm{m}$ apart: requires two `Kappa T3`-badged stewards to hold contact within a $\le 250\,\mathrm{ms}$ coincidence window while scrubber airflow is green. |
| **Tier-1 Salvage-Before-Melt Matcher** | Makerspace / Yard Tablet (`crdt:read`, `intent:author`) | Intercepts raw-stock requests and shows 3D overlays of how an existing `Scenario Mu` salvaged shaft or plate can be machined directly without remelting. |
| **Stamped Ingot & Crucible Heat Pedigree Tag** | Die-Stamped into Cooled Ingots | Screenless metal punch/QR mark on every cast bronze, aluminum, or steel billet linking it to its exact `XRF` alloy assay. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ homesteader}$):**
   * *Context:* A solo homesteader at Stage 1A sorting copper/aluminum/steel scrap in a garage nook and practicing Tier 1 cold/warm mechanical reuse (cutting, drilling, and bolting salvaged stock).
   * *Governance Model:* **Safety-first boundary (`No Solo Molten Pours`).** While a solo steward can freely sort scrap (`Scenario Mu`), anneal small copper workpieces, or run a small automated glass/ceramic test kiln with hardware thermal cutoffs, Scenario Theta's Layer 5 policy **forbids solo molten metal pouring ($N_s < 2$)**—routing molten casting needs to a two-person neighborhood foundry bee (`Stage 1B/2`) so no steward ever handles liquid bronze or iron alone.
2. **Small Foundry Crew Serving a Neighborhood Circle ($N_s = 2\text{–}4\text{ foundry stewards}, N_p > 1\text{ neighborhood makers}$):**
   * *Context:* A $2\text{–}4$ steward micro-foundry crew operating a $15\text{–}50\,\mathrm{kW}$ induction crucible and sand/lost-PLA casting bay for a neighborhood or town cluster ($N_p > 1$).
   * *Governance Model:*
     * **Mandatory Two-Person Co-Presence ($N_s \ge 2$, $\le 250\,\mathrm{ms}$ sync):** Every molten pour requires two certified stewards physically at the console and dead-man switches, paired with automatic `Scenario Sigma` hydration/cooling breaks and bathhouse hydrotherapy recovery after hot shifts.
     * **Crucible Batch Coalescing ($\ge 8500\,\mathrm{bp}$):** Rather than firing up the furnace whenever a single neighbor wants one bronze bushing, requests from the $N_p$ circle queue in the Crucible Batch Console until the mold tree reaches $\ge 85\%$ capacity and sunny afternoon microgrid surplus arrives.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A regional Foundry, Glass & Chemical Guild ($N_s = 15\text{–}80+$ metallurgists, glassblowers, and chemical engineers) operating heavy induction furnaces, rolling mills, glass tanks, and chlor-alkali cells for an entire watershed federation ($R_3 \dots R_5$).
   * *Governance Model:*
     * **Bioregional Specialization (Not Every Village Needs a Blast Furnace):** Under `Scenario Pi` federation treaties, a single regional Scenario Theta hub on an industrial brownfield site with high micro-hydro/solar capacity refines ingots, sheet stock, borosilicate glass, and bulk reagents for $10\text{–}30$ surrounding settlements, clearing via zero-sum `BCC` channels and `Scenario Beta` barge/rail freight.
     * **$R_4$ Supermajority Gate on Tier-3 Quarrying:** If the guild ever proposes a Tier 3 geospheric clay/mineral extraction, it must pass an $R_4$ Watershed Assembly supermajority vote with a $10\times \lambda_{\mathrm{ERC}}$ ecological restoration bond and full `Scenario Iota` groundwater monitoring.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Scrap-Yard Arbitrage & Specialty Alloy Ingot Procurement:** Buying undervalued municipal scrap metal or importing trace alloying elements (e.g., ferrosilicon, molybdenum, nickel, or virgin borosilicate tubing) not yet recovered in sufficient purity from local urban mining.<br>**B. Recycled Castings / Artistic Bronze Export (`policy:commons_plus_l7_zero_bill`):** Selling custom architectural castings or recycled glasswork to external buyers to fund foundry refractories and tax endowments. | • **Alloy Dependency Tracking ($D_{\mathrm{materials}}$):** Every imported alloying element or virgin stock is logged on the Legacy Dependency Register (`Scenario Omega`), guiding `Scenario Mu` urban-mining scanners to target those specific metals in upcoming demolition harvests.<br>• **$\Phi_t = 0$ Commercial Shutoff:** External commercial casting shuts off automatically each month once foundry maintenance and node fiat bills hit $0\text{ cents}$. |
| **2. Purely Defensive Reach into Layer 7** | **A. EPA / Clean-Air Permitting & Pretextual Complaint Defense:** Protecting the foundry from industrial zoning challenges or air-quality citations.<br>**B. Scrap-Metal Dealer & HazMat Compliance (`Mundane Semantic Camouflage`):** Complying with statutory scrap-metal anti-theft laws and chemical storage codes without exposing internal node BOMs. | • **Continuous Cryptographic Baghouse & Scrubber Proofs:** Layer 2 differential-pressure and particulate sensors log continuous signed compliance proofs demonstrating emissions far below municipal clean-air ceilings.<br>• **Licensed Scrap-Recycler & Subchapter T Wrapper:** Operates external scrap intake under a mundane licensed secondary-metals cooperative wrapper across `l6_l7.sock`. |
