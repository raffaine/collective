# Scenario Nu: Bioregional Fiber Shed, Zero-PFAS Textiles & Ephemeral-RAM Footwear (`SCN-NU-TEXTILES`)

* **Identifier:** `SCN-NU-TEXTILES`
* **Horizontal Wave:** Wave D (Regenerative Land, Extraction, Materials & Architecture)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)

---

## 1. Intended Scope & Domain Vision

Scenario Nu governs **how the settlement clothes its people, crafts resolable footwear, weaves household linens and technical shelter membranes, and fabricates certified Personal Protective Equipment (`PPE`)**. It closes the regional **Fiber Shed** loop—from bast-fiber crops (flax linen, industrial hemp, nettle) in `Scenario Eta`/`Gamma`, rotational pasture wool, and vegetable-tanned hides to spinning, weaving, botanical dyeing, bespoke tailoring, cobbling, and visible mending.

Crucially, Scenario Nu establishes two anti-dystopian guarantees at the heart of daily life:
* **Inviolable $R_0$ Personal Ownership & Zero-Surveillance Tailoring (`OP_PURGE_R0_LINEAGE`):** Refutes the authoritarian commune caricature of drab grey uniforms or collective requisitioning of personal clothes. Every shirt, coat, dress, or pair of custom-lasted boots crafted for a person belongs permanently to them in $R_0$, and 3D body/foot measurements used during tailoring live strictly in volatile RAM and are cryptographically wiped upon handoff.
* **Zero-PFAS, Zero-Cr(VI) & Microfiber-Free Ecology:** Eliminating synthetic microplastic shedding, "forever chemical" water repellents, and toxic hexavalent-chromium leather tanning from the watershed.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy global apparel and footwear industry (Layer 7), clothing human bodies drives four severe social and ecological harms:

1. **Fast-Fashion Sweatshops & Landfill Mountains:** Billions of garments are sewn in exploitative sweatshops from cheap polyester/acrylic blends engineered to pill, tear, or go out of style after ten washes—dumping millions of tons of non-biodegradable textile waste into Global South deserts and landfills every year.
2. **PFAS "Forever Chemicals," Cr(VI) Tanning & Azo Dye Poisoning:** Commercial waterproof jackets, stain-resistant fabrics, and chrome-tanned shoes leach perfluorinated (`PFAS`) compounds and carcinogenic hexavalent chromium (`Cr(VI)`) into rivers and human bloodstreams.
3. **Synthetic Microplastic Laundry Pollution:** Every domestic wash cycle of polyester fleece or nylon activewear releases hundreds of thousands of persistent plastic microfibers into wastewater and aquatic food webs.
4. **Cemented Unrepairable Footwear & Biometric Sizing Harvesting:** Mass-market shoes are glued together with polyurethane foams that crumble and cannot be resoled, while commercial "3D body-scan sizing apps" upload intimate customer body geometries to corporate marketing clouds.

---

## 3. Wide Feature Horizon

Scenario Nu replaces fast fashion and toxic synthetics with five regenerative textile and footwear loops:

1. **Bioregional Fiber Shed & Botanical Dye/Tanning Vats:** Processes retted flax linen, hemp bast fiber, ethical rotational shear wool, and reclaimed cotton (`Scenario Mu`), dyed with mordanted botanical pigments (`Scenario Eta` madder, weld, indigo, walnut hulls) and tanned strictly with vegetable bark tannins (`Scenario Eta` oak/mimosa gallotannins) at verified `0 mg` `PFAS` and `0 mg` `Cr(VI)`, weatherproofed with beeswax, linseed, and pine-rosin emulsions.
2. **Ephemeral-RAM Bespoke Tailoring & Goodyear-Welted Cobbling (`OP_PURGE_R0_LINEAGE`):** Parametric pattern plotters (`>= 9200 bp` / $92\%$ fabric nesting efficiency) and 3-axis CNC wood lathes mill custom orthopedic shoe lasts (`Scenario Eta` hornbeam/maple) and cut bespoke garment patterns. The moment the physical garment or stitched Goodyear-welted / stitchdown boot is handed to the wearer, `col-commonsd` executes `OP_PURGE_R0_LINEAGE`—cryptographically zeroizing all volatile RAM body/foot dimension buffers and transferring the item permanently off the public ledger into private $R_0$ ownership.
3. **First-Class Visible Mending (*Sashiko*) & Cobbler Resoling (`TextileRepairBounty`):** Celebrates visible darning, patching, zipper replacement, and boot resoling at $< 1500\,\mathrm{bp}$ ($15\%$) of the exergy of new fabrication, extending garment and boot lifespans to decades.
4. **Certified Technical `PPE` & Medical/Childcare Textiles:** Fabricates EN/ANSI-tested cut-resistant Kevlar/leather foundry aprons and welding gauntlets (`Scenario Theta`/`Alpha`), sterilized surgical drapes and scrubs (`Scenario Chi`), and organic cotton cloth diapers and infant layettes (`Scenario Delta`).
5. **Closed-Loop Laundry Filtration & End-of-Life Rag/Mycelial Cycling:** Equips all `Scenario Zeta` and `Scenario Delta` washing machines with $\ge 9800\,\mathrm{bp}$ ($98\%$ capture at $50\,\mu\mathrm{m}$) microfiber traps for legacy transitional synthetics, while worn-out natural linens cycle into paper rag stock (`Scenario Iota`) or mycelial insulation substrates (`Scenario Xi`).

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`PersonalGarmentCraftIntent`:** Requesting or self-crafting bespoke clothing, winter outerwear, or custom-lasted boots—automatically triggering `OP_PURGE_R0_LINEAGE` upon physical completion so zero body measurements or personal wardrobe inventories persist.
* **`TextileRepairBounty`:** Dropping off a torn jacket, worn heel, or frayed work apron for *Sashiko* mending, re-waxing, or Goodyear-welt resoling.
* **`TechnicalPpeAndCommonsLinenIntent`:** Commissioning shared $R_2$ safety gear (foundry aprons, welding gloves, acoustic curtains, insulated yurt/tent tarps, bathhouse towels, or `Scenario Delta` diaper sets) with full lifecycle tracking.
* **`DyeAndTanneryVatAttestation`:** Verifying `0 mg` `PFAS`, `0 mg` `Cr(VI)`, closed-loop mordant recovery, and neutral effluent pH before vat discharge.

### B. Semantic Representations & Deterministic Read-Backs
* **Hard Architectural Split Between $R_0$ Personal Attire and $R_2$ Shared Technical Gear:** In `https://w3id.org/collective/v1/`, shared $R_2$ safety `PPE` (e.g., a pair of high-voltage lineman gloves or a foundry heat-shield coat) retains an NFC lineage tag tracking inspection intervals and protective ratings. Conversely, personal $R_0$ clothing and boots carry **zero RFID/NFC chips** and **zero persistent graph nodes** once `OP_PURGE_R0_LINEAGE` fires—accounting only for the aggregate raw cloth mass (`mg`) and loom/cutter exergy (`uJ`) consumed during making.
* **Mandatory Zero-Retention Read-Back:** Before a resident steps into the Atelier scanner or enters measurements for a `PersonalGarmentCraftIntent`, the slate presents a **Deterministic Read-Back** proving that the geometry buffer is pinned to volatile `ramfs` (`0` disk write capability) and will be wiped upon pattern cut.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Ephemeral-RAM Cloth & Last Atelier Slate** | Tailoring & Cobbler Bench (`nu:atelier`, `r0:volatile_ram`) | Interactive style, fabric, and fit selector that nests patterns onto loom cloth at $\ge 9200\,\mathrm{bp}$ yield, drives the CNC cutter/shoe-last lathe, and flashes a hardware-verified `RAM ZEROIZED` confirmation upon completion. |
| **Visible Sashiko Mending & Cobbler Board** | Neighborhood Sewing Loft (`nu:repair`) | Calm drop-off and pickup board pairing mending requests with textile apprentices (`Scenario Kappa`) and community mending circles (`Scenario Lambda`). |
| **Vat Chemistry & Microfiber Filter Monitor** | Dye House / Laundry Room (`nu:vat`, `bpmn:review`) | Coalesces vegetable-tanning soak timers, indigo vat reduction potentials, and $50\,\mu\mathrm{m}$ laundry filter clean-out alerts into one daily check round. |
| **Screenless Paper Pattern & Chalk Projector** | Cutting Table (`nu:atelier`) | Projects nested cut lines directly onto linen or wool bolts with overhead LED light (or prints reusable kraft-paper patterns) so tailors work hands-free with shears and chalk. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person}$):**
   * *Context:* A solo homesteader or maker operating a sewing machine, mending kit, and small botanical dye pot.
   * *Governance Model:* **Pure creative self-expression.** Zero approvals or logs; the solo steward uses the offline pattern library and nesting tool to sew and mend their own wardrobe at will.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ household & neighbors}$):**
   * *Context:* One or two skilled tailors/cobblers stewarding a shared neighborhood sewing & shoe-repair loft for $10\text{–}30$ households ($N_p > 1$).
   * *Governance Model:*
     * **Unconditional Personal Wardrobe Sovereignty ($B_{\mathrm{baseline}}$):** Every child, elder, and adult in the $N_p$ circle is entitled to well-fitted seasonal clothing and resolable footwear as part of $B_{\mathrm{baseline}}$—with complete aesthetic freedom over colors, cuts, and cultural styles (zero uniform mandates).
     * **Mending Circles & Self-Service Access:** Neighbors with `Scenario Kappa` `L1` sewing/loom credentials can book the loft during off-peak hours (`PersonalSlotIntent`) to sew gifts or costumes with zero surveillance, while weekly social Mending Bees (`Scenario Sigma`/`Lambda`) keep the primary tailor from being buried in darning work.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A regional Fiber Shed, Textile & Cobbler Guild ($N_s = 10\text{–}60+$ spinners, weavers, dyers, tailors, and cobblers) operating flax scutching mills, multi-harness jacquard looms, vegetable tanneries, and `PPE` test benches across $R_2 \dots R_4$.
   * *Governance Model:*
     * **Strict Ecological Chemical Ban (`0 mg` PFAS / Cr(VI)):** The Commons and Stewardship Chambers enforce a hard constitutional ban on importing or applying perfluorinated treatments, hexavalent chromium salts, or toxic azo dyes anywhere in the watershed.
     * **Dual-Key `PPE` Certification:** Shared industrial `PPE` batches (welding leathers, chainsaw chaps, clinical drapes) require a `Scenario Kappa` `T3` safety sign-off verifying puncture, thermal, or sterility ratings before entering workshop cribs.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Specialty Technical Fiber & Hardware GPO (`ProcurementIntent`):** Importing aramid (`Kevlar`/`Nomex`) thread for firefighter/foundry `PPE`, surgical-grade stainless needles, or natural crepe rubber soles not yet produced locally.<br>**B. Bespoke Tailoring, Knitwear & Heritage Boot Export (`policy:commons_plus_l7_zero_bill`):** Selling high-craft custom wool coats or Goodyear-welted boots to external Layer 7 customers to fund textile mill equipment and node tax bills. | • **Fiber Dependency Reduction ($D_{\mathrm{materials}}$):** Imported cotton or technical threads are logged on the Legacy Dependency Register, driving `Scenario Eta` flax/hemp acreage and `Scenario Mu` post-consumer textile reclamation.<br>• **Strict $B_{\mathrm{baseline}}$ Priority & $\Phi_t = 0$ Shutoff:** External bespoke sales are permitted only after all resident seasonal clothing/boot needs and `Scenario Delta` infant layettes are fulfilled, shutting off automatically once monthly node fiat bills hit $0\text{ cents}$. |
| **2. Purely Defensive Reach into Layer 7** | **A. ANSI / OSHA `PPE` & Children's Sleepwear Statutory Compliance:** Protecting workshop operations and community textile distribution from regulatory liability.<br>**B. Biometric & Aesthetic Privacy (`Mundane Semantic Camouflage`):** Preventing external platforms from profiling residents' body sizes or clothing habits. | • **Certified Testing & Cooperative Atelier Carapace:** Logs destructive tensile/flame test attestations on shared `PPE` batches for insurer/OSHA audits and operates external craft sales under an **IRC Subchapter T** artisan cooperative wrapper.<br>• **Zero Outbound Sizing Data:** Because `OP_PURGE_R0_LINEAGE` wipes all body measurements in RAM at creation, zero sizing or biometric records exist to leak or subpoena across `l6_l7.sock`. |
