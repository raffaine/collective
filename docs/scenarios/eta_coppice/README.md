# Scenario Eta: Regenerative Agroforestry, Rotational Coppice & Botanical Apothecary (`SCN-ETA-COPPICE`)

* **Identifier:** `SCN-ETA-COPPICE`
* **Horizontal Wave:** Wave D (Regenerative Land, Extraction, Materials & Architecture)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Eta governs **how the settlement partners with living forests, perennial agroforestry guilds, rotational coppice woodlots, mycological groves, and wild botanical ecosystems**. It closes the biological feedstock loop for structural construction timber (`Scenario Xi`), workshop joinery and luthiery tonewood (`Scenario Alpha`/`Lambda`), natural bast fibers and tannins (`Scenario Nu`), perennial nut/fruit staples (`Scenario Gamma`), and verified botanical precursors (`Scenario Chi`).

Unlike industrial clear-cut forestry that treats woodland as a standing timber mine, Scenario Eta operates on **multi-decade regenerative silviculture**:
* **Rotational Coppice & Pollarding:** Harvesting black locust, hazel, chestnut, willow, and oak on $5\text{–}25\text{ year}$ rotational cycles where the living root system remains intact, sequestering carbon and sprouting vigorous new poles every cycle.
* **Mother-Tree Sanctuary Polygons:** Enforcing an unbreakable constitutional veto over old-growth groves, riparian salmon buffers, and wildlife corridors.
* **The Forest Apothecary:** Cultivating and ethically wild-harvesting medicinal herbs, tree resins, and culinary/medicinal fungi gated by dual analytical safety assays.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy forestry and botanical supply chains (Layer 7), human demand for wood, paper, and plant compounds drives four ecological disasters:

1. **Clear-Cut Monoculture & Soil Erosion:** Industrial timber companies clear-cut biodiverse watersheds, spray glyphosate to kill broadleaf regeneration, and replant single-age Douglas fir or pine plantations that strip topsoil, dry up summer creeks, and explode into crown megafires (`Scenario Upsilon`).
2. **Kiln-Dried Lumber Energy Waste & Chemical Preservatives:** Commercial lumber is hauled hundreds of kilometers to fossil-gas kilns and pressure-treated with toxic chromated copper arsenate (`CCA`) or creosote that leaches into garden soils and precludes safe end-of-life composting (`Scenario Mu`).
3. **Over-Harvesting of Wild Botanicals:** Commercial supplement and essential-oil markets strip wild stands of medicinal plants and fungi to local extinction with zero abundance monitoring.
4. **Adulterated & Unverified Herbal Products:** Consumer botanical extracts routinely contain heavy-metal contamination from polluted soils, pesticide residues, or misidentified toxic look-alike species (such as amatoxin-bearing fungi).

---

## 3. Wide Feature Horizon

Scenario Eta replaces extractive logging and unverified wildcrafting with five long-horizon ecological loops:

1. **Multi-Strata Food Forests & Rotational Coppice Stools (`HeavyTimberAndCoppiceIntent`):** Manages seven-layer agroforestry guilds (canopy nut trees, sub-canopy fruit, coppice timber stools, nitrogen-fixing shrubs, perennial herbs, root crops, and climbing vines) where rotational coppice cuts unlock strictly during winter sap dormancy ($< 278{,}150\,\mathrm{mK}$ / $5.0^\circ\mathrm{C}$ multi-day root-zone average) to maximize rot resistance and stool regrowth.
2. **Hard Ecological `Protocol of Refusal` (`Mother-Tree Sanctuary Polygons`):** Old-growth anchor trees, mycorrhizal hub groves, nesting snags, and riparian buffers are mapped as sacred sanctuary polygons carrying an absolute hardware/software veto (`ERR_MOTHER_TREE_SANCTUARY_VETO`) that blocks any timber requisition regardless of workshop demand.
3. **1-in-4 Ethical Wild-Foraging Cap (`ForagingQuotaIntent`):** Wild harvesting of ramps, berries, medicinal roots, bark, and chanterelles is hard-capped at $\le 2500\,\mathrm{bp}$ ($25\%$ of observed stand abundance), leaving $\ge 75\%$ undisturbed for wildlife, seed dispersal, and stand regeneration.
4. **Solar Dehumidification Lumber Kilns:** Cures structural timber to $\le 1200\,\mathrm{bp}$ ($12.0\%$) moisture content for `Scenario Xi` *Kigumi* joinery and specialty luthiery tonewood (spruce, maple, walnut) to $\le 800\,\mathrm{bp}$ ($8.0\%$) for `Scenario Lambda` instruments using solar thermal collectors and heat-pump dehumidification.
5. **Dual-Assay Botanical & Mycological Apothecary (`PharmacopoeiaBatchAttestation`):** Outdoor log-grown and indoor fruiting mushrooms (*Hericium*, *Trametes*, *Lentinula*) and botanical tinctures/essential oils undergo mandatory dual-steward purity assays verifying `amatoxin == 0 ng` and heavy metals `< 100 ppb` before entering `Scenario Gamma` larders or `Scenario Chi` clinical compounding.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`HeavyTimberAndCoppiceIntent`:** Requesting a winter-dormant rotational coppice harvest or windfall salvage cut, tracking every log from stump to solar kiln stack to workshop delivery.
* **`ForagingQuotaIntent`:** Logging a stand abundance survey and claiming a seasonal wild-foraging quota strictly within the $\le 2500\,\mathrm{bp}$ ($25\%$) ecological ceiling.
* **`PharmacopoeiaBatchAttestation`:** Co-signing the analytical safety and potency release of a botanical extract, tannin bath (`Scenario Nu`), or medicinal mushroom batch.
* **`MotherTreeSanctuaryAmendment`:** Expanding a permanently protected old-growth or riparian conservation polygon.

### B. Semantic Representations & Deterministic Read-Backs
* **94% Silent `DORMANT` Phenological State Machines:** Because coppice stools grow over $7\text{–}20\text{ years}$ and solar kiln stacks cure over $6\text{–}18\text{ months}$, `col-commonsd` and `col-execd` hold $94\%$ of Scenario Eta's $340+$ concurrent workflows in silent `DORMANT` state—waking a human steward only when root-zone soil probes cross winter dormancy ($< 278{,}150\,\mathrm{mK}$) or kiln moisture sensors hit target equilibrium ($\le 1200\,\mathrm{bp}$ / $\le 800\,\mathrm{bp}$).
* **Mandatory Sanctuary & Dormancy Read-Back:** Before a forester signs a `HeavyTimberAndCoppiceIntent`, the fieldbook displays a **Deterministic Read-Back** confirming zero overlap with `Mother-Tree Sanctuary Polygons`, verified multi-day soil dormancy temperature $< 278{,}150\,\mathrm{mK}$, and positive net canopy basal-area growth ($F_{\mathrm{eco}} > 0$).

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Offline Domain-B Agroforestry GIS Fieldbook** | Forest Walk / E-Ink Tablet (`agro:forestry`, `crdt:sync`) | Offline contour map displaying coppice stool age, Mother-Tree Sanctuary boundaries, and $\le 2500\,\mathrm{bp}$ wild-foraging gauges; syncs via `Scenario Beta` LoRa or barn Wi-Fi upon return. |
| **Dual-Assay Botanical Apothecary Slate** | Apothecary / Wet-Lab Bench (`pharma:assay`) | High-contrast slate requiring two independent `Scenario Kappa`-certified herbalists/chemists to read back `amatoxin == 0 ng` and heavy-metal `< 100 ppb` test strips/spectra before printing batch labels. |
| **Solar Kiln Moisture & Cure Board** | Lumber Shed (`agro:forestry`, `bpmn:review`) | 0-mW E-Ink display on each timber stack showing live core moisture basis points (`1200 bp` structural / `800 bp` tonewood) so woodworkers never cut green, warping stock. |
| **Stamped Stainless Tree & Log Lineage Tag** | Hammered into Log Ends / Coppice Stools | Screenless physical QR/number tag linking a cured timber beam in `Scenario Xi` back to the exact coppice stool and winter harvest year. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ homesteader}$):**
   * *Context:* A solo homesteader managing a backyard dwarf-orchard guild, a small willow/hazel coppice hedgerow for garden stakes and basketry, shiitake logs, and an herb drying rack.
   * *Governance Model:* **Calm seasonal calendar & botanical safety guide.** Layer 6 stays silent for months at a time, surfacing gentle phenological reminders when winter pruning dormancy arrives or when shiitake logs are ready to soak, while providing clear mycotoxin/identification checklists for home apothecary teas.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ neighborhood pod}$):**
   * *Context:* One or two neighborhood arborists/herbalists stewarding a shared 2-acre coppice woodlot, solar lumber kiln, and apothecary cabinet for $10\text{–}30$ households ($N_p > 1$).
   * *Governance Model:*
     * **Preventing Tragedy of the Commons in Wild Foraging:** When $N_p$ neighbors walk the woods for mushrooms, berries, or medicinal herbs, the shared `ForagingQuotaIntent` gauge automatically closes harvesting on a patch once cumulative picks reach $2500\,\mathrm{bp}$ ($25\%$), protecting the stand without needing forest rangers.
     * **Dual-Sign Apothecary Safety:** Before any herbal tincture or foraged mushroom batch is placed in the shared `Scenario Gamma`/`Chi` neighborhood cabinet, a second qualified peer co-signs the `PharmacopoeiaBatchAttestation`.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A watershed Silviculture & Apothecary Guild ($N_s = 15\text{–}80+$ foresters, ecologists, and mycologists) stewarding hundreds of hectares of agroforestry, coppice timber, and hydrated wildfire breaks (`Scenario Upsilon`) across $R_2 \dots R_5$.
   * *Governance Model:*
     * **Standing Legal Personhood of the Forest ($F_{\mathrm{eco}}$ Veto):** Both the Silviculture Guild and the Commons Chamber recognize `Mother-Tree Sanctuary Polygons` as inviolable; any unauthorized cutting inside a sanctuary polygon triggers an immediate `Scenario Psi` ecological restitution inquiry.
     * **Multi-Decade Intergenerational Stewardship (`Scenario Phi`):** Long-rotation timber and chestnut groves (planted by one generation to mature for the next) are co-governed with `Scenario Phi` Emeritus Sages who hold multi-decade phenological memory of the watershed.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Bare-Root Heirloom Nursery Stock & Mycelial Spawn Import:** Sourcing disease-resistant chestnut cultivars, fruit scions, or initial fungal cultures to establish diverse perennial stands.<br>**B. Certified Sustainable Tonewood / Specialty Botanical Export (`policy:commons_plus_l7_zero_bill`):** Selling a small fraction of solar-kilned specialty lumber or botanical extracts to pay down land acquisition or forestry equipment costs. | • **Local Propagation Multiplication:** Imported scions and mycelial cultures are immediately propagated in the node's own stool beds and tissue-culture labs (`Scenario Iota`) so external nursery dependence drops to $0\,\mathrm{bp}$.<br>• **Basal-Area Growth Floor & $\Phi_t = 0$ Cap:** External wood/botanical sales are blocked unless annual stand growth exceeds harvest by $\ge 2000\,\mathrm{bp}$ ($20\%$) and shut off automatically once monthly land/tax obligations hit $0\text{ cents}$. |
| **2. Purely Defensive Reach into Layer 7** | **A. Property-Tax Slash via Forest & Conservation Easements ($70\text{–}90\%$ Reduction):** Neutralizing the largest recurring fiat expense on rural and peri-urban land.<br>**B. Anti-Biopiracy & Heirloom Seed Defense:** Preventing agro-chemical corporations from patenting locally bred cultivars. | • **Designated Forest Land & UCEA Conservation Easements:** Enrolls woodland and agroforestry parcels in statutory **Designated Forest Land / Timber Open Space** tax programs and **Uniform Conservation Easement Act (`UCEA`)** covenants, legally binding the land against commercial subdivision while slashing county property taxes by $70\text{–}90\%$.<br>• **Open-Source Seed Pledge & Prior-Art Publication:** Publishes cultivar genomic/phenotypic descriptions via `Scenario Iota` defensive prior art. |
