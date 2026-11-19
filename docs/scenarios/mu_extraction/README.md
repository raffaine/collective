# Scenario Mu: Five-Stream Urban Unbuilding, Pyrolysis Biochar & Nutrient Recovery (`SCN-MU-EXTRACTION`)

* **Identifier:** `SCN-MU-EXTRACTION`
* **Horizontal Wave:** Wave D (Regenerative Land, Extraction, Materials & Architecture)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Mu is the **circular deconstruction, urban mining, and biogeochemical nutrient-recovery engine** of The Collective. It abolishes the concept of "waste" by intercepting both internal settlement residues and external Layer 7 demolition/discard streams, sorting and refining them into five precision feedstock streams that power the rest of the bioregion:
1. **Structural Timber, Masonry & Architectural Salvage** ($\to$ `Scenario Xi` and `Scenario Alpha`).
2. **Sorted Metals, E-Waste & Rare-Earth Concentrates** ($\to$ `Scenario Theta` foundry and `Scenario Zero` hardware reuse).
3. **Thermoplastic & Elastomer Re-Granulation** ($\to$ `Scenario Alpha` filament/pellet extrusion).
4. **Smokeless Pyrolysis Biochar, Wood Vinegar & Syngas Heat** ($\to$ `Scenario Eta`/`Gamma` *Terra Preta* soils and `Scenario Beta` hydronic heating).
5. **Pathogen-Free Thermophilic Compost & Crystallized Struvite ($\mathrm{MgNH_4PO_4 \cdot 6H_2O}$)** (closing $90\%+$ of the human phosphorus and nitrogen loop).

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy linear economy ("take-make-dump"), material extraction and disposal create four catastrophic failures:

1. **Demolition Bulldozing & Landfill Entombment:** When legacy buildings are torn down, excavators crush seasoned old-growth Douglas fir beams, copper wiring, cast-iron radiators, and intact windows into mixed rubble hauled to landfills—wasting $90\%+$ of embodied energy.
2. **The Broken Planetary Phosphorus & Nitrogen Cycle:** Modern cities import food grown with mined rock phosphate and fossil-gas Haber-Bosch nitrogen, then flush human nutrient excreta into potable water sewers where it triggers toxic river algal blooms while farm soils starve.
3. **Toxic Contamination & Open Burn Pollution:** Uninspected salvage lumber often hides lead paint, asbestos, or arsenic (`CCA`), while agricultural brush burning fills valleys with carcinogenic particulate smoke instead of sequestering carbon as biochar.
4. **Global South E-Waste & Hazardous Dumping:** Wealthy municipalities routinely claim "recycling" credit by shipping toxic e-waste, contaminated plastics, and chemical sludge to informal dumpsites in developing nations.

---

## 3. Wide Feature Horizon

Scenario Mu replaces landfills, synthetic fertilizer imports, and toxic dumping with five closed-loop refinement processes:

1. **Five-Stream Precision Unbuilding ($\ge 9800\,\mathrm{bp}$ / $98\%$ Landfill Diversion):** Surgical deconstruction crews use handheld Near-Infrared (`NIR` polymer) and X-Ray Fluorescence (`XRF` heavy-metal) spectrometers to grade, de-nail, and catalog structural beams, bricks, glass, metals, and electronics at the demolition site.
2. **EPA 503 Class-A Thermophilic Composting & Struvite Crystallization:** Organic residuals and `Scenario Zeta` vault solids undergo hardware-verified thermophilic sanitation ($\ge 328{,}150\,\mathrm{mK}$ / $55.0^\circ\mathrm{C}$ continuously for $72\text{ hours}$, automatically resetting the 72h clock on any temperature dip), while source-separated urine reactors precipitate $\ge 9000\,\mathrm{bp}$ ($90\%$) of phosphorus and nitrogen as pure, odorless **struvite crystals** within $[8500,\, 9200]\,\mathrm{mpH}$ ($\mathrm{pH}\ 8.5\text{–}9.2$).
3. **Smokeless Top-Lit Updraft (`TLUD`) Biochar Retorts & Hydronic Heat Capture:** Woody slash and non-merchantable biomass are pyrolyzed at $[673{,}150,\, 823{,}150]\,\mathrm{mK}$ ($400\text{–}550^\circ\mathrm{C}$) in afterburning retorts—sequestering recalcitrant carbon into high-porosity biochar (charged with compost/struvite into *Terra Preta*) while routing $100\%$ of retort exhaust heat into `Scenario Beta` PEX-a hydronic loops.
4. **Cryptographic Brownfield Phytomining & Taint Quarantine (`TAINTED_HEAVY_METAL_ASH`):** When hyperaccumulator plants (such as *Brassica juncea* or willow) remediate lead/cadmium/nickel-contaminated urban brownfields, or when `XRF` flags painted/treated salvage wood, the batch receives an immutable cryptographic taint tag blocking it from agricultural/culinary loops and routing it strictly to enclosed `Scenario Theta` metallurgical recovery or vitrification.
5. **The Prohibited Hazardous Export Register (`Zero Dumping Covenant`):** Constitutionally forbids exporting any hazardous e-waste, chemical solvent, or tainted ash to external municipal dumps or Global South sinks.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`EcologicalDeconstructionAndTriageIntent`:** Scheduling a surgical unbuilding salvage crew or logging incoming neighborhood material streams across the five sorting channels ($\ge 9800\,\mathrm{bp}$ diversion target).
* **`PyrolysisAndTerraPretaIntent`:** Initiating a `TLUD` biochar retort burn, a 72-hour thermophilic compost sanitation hold, or a struvite precipitation batch.
* **`PhytominingAndTaintQuarantineIntent`:** Tagging contaminated brownfield biomass or lead/asbestos/CCA salvage with an immutable `TAINTED_HEAVY_METAL_ASH` lock.
* **`SolventAndPolymerRecoveryIntent`:** Routing sorted thermoplastics (`HDPE`, `PP`, `PETG`, `PLA`) to wash/shredder granulators or distilling spent botanical/clinical alcohols (`Scenario Chi`/`Eta`) for closed-loop reuse.

### B. Semantic Representations & Deterministic Read-Backs
* **Immutable Taint Lineage Propagation (`https://w3id.org/collective/v1/`):** In `col-commonsd`, any material node carrying `taintStatus: TAINTED_HEAVY_METAL_ASH` or `UNSANITIZED_BIOSOLIDS` is blocked by SHACL validation from ever being referenced as an input to `Scenario Gamma` (food), `Scenario Eta` (edible soils), `Scenario Delta` (nursery furniture), or `Scenario Zeta` (interior living space).
* **72-Hour Continuous Thermal Hold Reset Invariant:** If a compost pile's core PT100 probe drops below $328{,}150\,\mathrm{mK}$ ($55.0^\circ\mathrm{C}$) at hour 71, Layer 4 resets the sanitation timer to `0 ms`—and Layer 6 suppresses all routine temperature ticks until the full uninterrupted $259{,}200{,}000\,\mathrm{ms}$ ($72\text{h}$) window completes.
* **Mandatory Taint & Clearance Read-Back:** Clearing a batch of compost/struvite for garden application or routing tainted phytomined ash to `Scenario Theta` requires a **Deterministic Read-Back** of the complete thermal/pH log and `XRF` heavy-metal assay before hardware signature.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Handheld NIR/XRF Unbuilding Triage Scanner** | Demolition Site / Salvage Yard (`mu:triage`, `crdt:sync`) | Ruggedized offline scanner + E-Ink slate: point at a salvaged beam or pipe to identify polymer/alloy grade or flag lead/arsenic taint in 2 seconds; prints weatherproof QR stock tags on the spot. |
| **Biosolids, Struvite & TLUD Biochar Inspector** | Compost Pad / Biorefinery Shed (`mu:biorefinery`) | Coalesces 72-hour thermal holds ($\ge 328{,}150\,\mathrm{mK}$), struvite reactor pH ($[8500, 9200]\,\mathrm{mpH}$), and retort afterburner temperatures into a single daily check round. |
| **Taint Quarantine Lockout Board** | Hazardous Holding Bay (`mu:quarantine`, `rho:auditor`) | High-contrast warning slate wired to physical magnetic bin locks; tainted materials can only be unlocked by a `Scenario Theta` smelting transfer receipt. |
| **Color-Coded 5-Stream Porch & Shop Tote Tags** | Household & Workshop Bins (`courier:dtn`) | Screenless tactile/color-coded NFC tags on returnable bins so `Scenario Beta` milk-runs route food scraps, clean polymers, metal swarf, and glass effortlessly. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ homesteader}$):**
   * *Context:* A solo homesteader running a 3-bin thermophilic hot-compost bay, a backyard smokeless `TLUD` garden-biochar stove, and workshop scrap sorting.
   * *Governance Model:* **Zero meetings, automated pathogen safety.** A single wireless PT100 compost probe and `col-commonsd` track the 72-hour $\ge 328{,}150\,\mathrm{mK}$ thermal hold silently, letting the solo gardener know the exact morning their compost is Class-A safe for vegetable beds.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ neighborhood pod}$):**
   * *Context:* One or two soil/salvage stewards operating a neighborhood compost & biochar depot and sorting shed for $5\text{–}20$ households ($N_p > 1$).
   * *Governance Model:*
     * **Taint Protection Across Neighbors:** When $N_p$ households contribute yard trimmings, workshop offcuts, or salvage lumber, quick `XRF`/visual triage at intake prevents a neighbor from accidentally putting painted or `CCA`-treated wood into the shared biochar retort or garden mulch.
     * **Rotational Compost Turning & Fair Share (`Scenario Sigma`):** Heavy compost turning and wood-chip screening rotate via Work-Bees (`0` tokens) or trigger a `DrudgeryAutomationQuestIntent` to fabricate a `Scenario Alpha` tractor/auger turner, while finished *Terra Preta* and struvite distribute back across all neighborhood gardens.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A regional Unbuilding & Biorefinery Guild ($N_s = 10\text{–}60+$ deconstruction specialists, chemical technicians, and soil scientists) operating surgical demolition crews, architectural salvage warehouses, struvite reactors, and brownfield phytomining plots across $R_2 \dots R_5$.
   * *Governance Model:*
     * **Constitutional Zero-Dumping Enforcement:** Neither the guild nor any local assembly can vote to dump hazardous residue into an external landfill or stream; the **Prohibited Hazardous Export Register** is an entrenched constitutional floor enforced by `col-kmsd`.
     * **Feedstock Supply to `Xi`, `Alpha`, `Theta`, and `Eta`:** Cataloged salvage inventory (seasoned beams, steel plate, copper busbars, clean polymer pellets, pure struvite) is advertised across the bioregional mesh (`BPVP`) at pure thermodynamic recovery cost.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Tipping-Fee Inversion & Urban Unbuilding Contracts (`policy:commons_plus_l7_zero_bill`):** Contracting with legacy property owners or municipalities to surgically deconstruct retiring buildings or accept clean organic streams—getting **paid in Layer 7 fiat tipping/deconstruction fees** while harvesting high-value timber, metal, and masonry for the commons!<br>**B. Brownfield Land Acquisition at Discount:** Acquiring degraded urban/industrial lots cheaply and remediating them via multi-year phytomining and *Terra Preta* soil building (`Scenario Tau`/`Zeta`). | • **Mandatory Intake `XRF`/`NIR` Quarantine:** Every external salvage lot crosses a strict chemical/spectrometric triage gate before entering internal stock; un-remediable asbestos/PCB hazards are refused at the contract survey stage.<br>• **$\Phi_t = 0$ Revenue Routing:** Tipping-fee and deconstruction revenue routes into the `Scenario Epsilon` cooperative carapace to pay node taxes and fund the Property Tax Escrow Endowment (`Scenario Omega`). |
| **2. Purely Defensive Reach into Layer 7** | **A. Agricultural Compost & Salvage Grading Safe-Harbor:** Preventing municipal waste monopolies from blocking neighborhood composting, rainwater/greywater nutrient recovery, or structural reuse of salvaged timber.<br>**B. Environmental Audit Defense:** Proving zero hazardous leachate or smoke emissions against pretextual code complaints. | • **EPA 503 Class-A & Agricultural Nutrient Exemption Logs:** Maintains cryptographic 72-hour thermal logs and `TLUD` clean-burn attestations under statutory agricultural/on-site composting exemptions, plus licensed engineer (`Scenario Xi`) re-grading stamps for salvaged structural timber. |
