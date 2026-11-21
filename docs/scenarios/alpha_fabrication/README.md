# Scenario Alpha: Convivial Fabrication, Parametric Blueprints, Lifecycle Repair & The Tool Library (`SCN-ALPHA-FAB`)

* **Identifier:** `SCN-ALPHA-FAB`
* **Horizontal Wave:** Wave B (Suburban Survival, Energy, Fabrication & Transit Seed)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Alpha defines how a settlement **designs, manufactures, repairs, and shares physical tools, building components, electronics, and durable goods**. Rather than attempting to replicate a monolithic, vertically integrated factory inside every neighborhood—or reducing local makerspaces to hobbyist trinket shops—Scenario Alpha establishes a **federated, bioregional manufacturing commons**.

Its scope spans the full physical lifecycle of durable goods:
* **Cosmopolitan Design Commons:** Global open-hardware blueprints adapted parametrically to local bioregional materials and installed machine tools.
* **Multi-Modal Production:** Distributed fabrication across wood, metal, polymers, electronics, and technical textiles.
* **Recursive Supply-Chain Fulfillment:** Decomposing complex assemblies across local workcells, peered sister-node workshops, and bounded external leaf imports.
* **First-Class Repair & Evolution:** Diagnostic remanufacturing where empirical field durability feeds directly back into blueprint selection.
* **Shared Tool Stewardship & Creative Autonomy:** Circulating everyday and specialized tools through a tiered Tool Library while guaranteeing private, unmonitored maker access during off-peak hours.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy industrial economy (Layer 7), the design, manufacture, and maintenance of physical goods are severed from the communities that depend on them, creating four structural failures:

1. **Intellectual Monopoly & Planned Obsolescence:** Manufacturers lock CAD geometries, schematics, firmware, and service manuals behind proprietary walls. Appliances, farm equipment, and electronics are engineered with brittle sub-components, glue-sealed housings, and serialized part locks so that a single worn bearing or blown capacitor forces the disposal of the entire machine.
2. **Transoceanic Supply-Chain Fragility & Embodied Entropy:** Everyday hardware, spare parts, and building fixtures are mass-produced in distant monoculture factories and shipped across oceans in single-use plastics—embedding massive transport energy while leaving neighborhoods helpless whenever global logistics stall.
3. **Atomized Consumer Hoarding:** Because neighborhoods lack high-trust sharing infrastructure, dozens of households on the same street redundantly purchase low-duty drills, saws, pressure washers, and lawn mowers that sit idle in private garages for over $9500\,\mathrm{bp}$ ($95\%$) of their mechanical lifespan.
4. **Industrial Alienation vs. Hobbyist Impotence:** Industrial-grade machinery (multi-axis CNC mills, timber jointers, metal lathes, TIG welding bays, PCB pick-and-place lines) remains locked inside commercial factories where workers have zero voice over what they build, while community makerspaces often lack the material supply chains and scheduling coordination required for serious civic production.

---

## 3. Wide Feature Horizon

Scenario Alpha replaces those four failure modes with five complementary workshop modalities and four lifecycle loops:

* **Five Workshop Modalities:**
  1. *Woodworking & Structural Joinery:* Sawmilling, cabinet saws, planers, CNC timber routers, and steam-bending kilns producing timber frames, pea trellises, poultry coops, cabinetry, furniture, beehive boxes, and ergonomic handles.
  2. *CNC Machining & Metalworking:* 3-to-5-axis milling centers, metal lathes, plasma/laser sheet tables, press brakes, welding bays, and heat-treating furnaces producing gears, shafts, bearing housings, cargo-trike frames, root-washer jigs, and micro-hydro turbine runners.
  3. *Additive Polymer & Elastomer Forming:* High-speed filament/pellet extruders, photopolymer vats, and waste-polymer shredders producing TPU weatherproof gaskets, microfluidic valves, electrical enclosures, and replacement appliance gears.
  4. *Electronics, Winding & Mechatronics:* PCB mills, pick-and-place reflow benches, coil winders, and battery spot-welders producing sensor/actuator carrier boards, MPPT solar controllers, brushless motor stators, and LoRa mesh transceivers.
  5. *Textiles, Cordage & Biocomposites:* Heavy walking-foot sewing machines, ultrasonic welders, looms, and natural-fiber/mycelium compression presses producing insulated tent membranes, waterproof cargo tarps, rigging harnesses, and acoustic panels.
* **Four Core Lifecycle Loops:**
  * *Global-to-Local Parametric Forking:* Adapting planetary blueprints to local coppiced hardwoods (`Scenario Eta`), recycled alloys (`Scenario Theta`), or reclaimed polymers (`Scenario Mu`).
  * *Recursive DAG BOM Chaining:* Automatically splitting multi-part builds across local bays and neighboring nodes (`Scenario Beta` cargo milk-runs).
  * *Diagnostic Triage & Remanufacturing:* Scanning a product's QR/NFC lineage tag to fabricate just the worn sub-part, or routing irreparable scrap back to `Scenario Mu` / `Scenario Theta`.
  * *Tiered Tool Library & Off-Peak Maker Slots:* Circulating portable tools across concentric trust rings ($R_0 \dots R_4$) and opening machine bays for personal craft during off-peak solar windows.
  * *Open-Source Franchising Across the 3 Sovereign Libraries & 3-Tier BPMN Hardware Cascade (`D7`, `D8`):* Stores proven workshop and tool blueprints across the **Ontological Graph**, **Orchestration Registry**, and **Physics Baseline** (*Turnkey Instantiation*, *Localized Semantic Forking*, *Empirical Lineage Ranking* via `collective:thermodynamicProof`, and *Greenfield Montage*). When a blueprint meets non-standard local machinery, Layer 4 evaluates the **3-Tier BPMN Hardware Adaptation Cascade**: (Tier 1) *SI Parameter Scaling* $\to$ (Tier 2) *Macro-Activity Substitution* (e.g., splicing an `M6` bolted flange workflow in place of a laser-weld Call Activity) $\to$ (Tier 3) *True-Constraint Hard-Block* (decompiling upward into a Layer 6 **Semantic Diff**).
  * *3-Tier Hardware Fleet Progression (From Scavenged Layer 7 to Zero-Stat Liberation, `D8`):* Bootstraps on **Stage I: Scavenged Layer 7 Fleet** (overcoming the 4 shim-tax penalties: *Parasitic L2 Shims*, *Blind Mid-Batch Hard-Blocks*, *Layer 7 Tribute Escalation*, and *Layer 3 ERC Penalties*), advances through **Stage II: Right-to-Repair Hybrid Fleet** (replacing potted DRM boards with open ESP32/RISC-V Layer 2 controllers and 3D-printable metric adapters as a **first-class Layer 3 Proof of Stewardship [$\Delta V / \Delta R$] event**), and matures into **Stage III: Native Open-Spec Collective Fleet** (streaming predictive bearing/RPM wear for 5-minute preventive metric swaps that achieve **Zero-Stat Liberation**).

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
Residents and craft stewards interact with the fabrication commons through six canonical JSON-LD intents:
* **`BlueprintSpecializationIntent`:** Forking a master open-hardware design (`parentBlueprintCid`) to match local timber/metal stock dimensions or available machine toolpaths while preserving standard mounting interfaces.
* **`ManufacturingOrderIntent`:** Requesting fabrication of a part or assembly—decomposing its recursive Bill of Materials (`BOM`) into local shop tasks, sister-node sub-orders, or flagged leaf imports.
* **`ToolCheckoutIntent` & `MaintenanceIntent`:** Borrowing portable hand/power tools or mobile machinery from the shared Tool Library, and queuing sharpening, calibration, or bearing replacement when wear thresholds are crossed.
* **`PersonalSlotIntent`:** Booking an off-peak workshop bay for personal woodworking, luthiery, gifts, or invention with **zero telemetry logged on what personal object was crafted**.
* **`RepairDiagnosticIntent` & `SalvageIntent`:** Isolating a failed sub-component from a product's exploded assembly tree for targeted single-part fabrication, or routing end-of-life material to `Scenario Mu` / `Scenario Theta`.

### B. Semantic Representations & Deterministic Read-Backs
`col-commonsd` represents workshop capabilities and commitments in the W3C JSON-LD 1.1 / SHACL knowledge graph (`https://w3id.org/collective/v1/`):
* **Dual-Layer Blueprint Bundles (`BlueprintSpecification` + `fBOM`):** Links parametric 3D geometry (STEP / OpenSCAD / 3MF) and exploded repair trees to a **Functional Bill of Materials (`fBOM`)** defined by physical performance properties (e.g., shear yield, food-safety, thermal rating) rather than vendor part numbers.
* **Mandatory Total Production Cost Vector Read-Back ($\mathbf{C}_{\mathrm{prod}}$):** Before a steward signs any `ManufacturingOrderIntent` or `BlueprintSpecializationIntent` with their personal hardware key, their application lens displays a plain-language **Deterministic Read-Back** of the full physical footprint:
  $$\mathbf{C}_{\mathrm{prod}} = \bigl\langle E_{\mathrm{spindle}\,\mu\mathrm{J}},\; M_{\mathrm{stock}\,\mathrm{mg}},\; \lambda_{\mathrm{ERC}\,\mu\mathrm{J}},\; \tau_{\mathrm{machine}\,\mathrm{ms}} \bigr\rangle$$
  reporting spindle exergy (`uJ`), raw stock mass (`mg`) from local salvage versus external fallback, Asymptotic Ecological Replacement Cost ($\lambda_{\mathrm{ERC}}$), steward setup time, and acoustic boundary compliance ($\le 550\,\mathrm{cB}$ day / $\le 380\,\mathrm{cB}$ night).
* **Empirical Durability Lineage (`observed_mtbf_sec`):** Completed repairs update the field-observed mean-time-between-failure of each blueprint variant, allowing longer-lasting designs to surface automatically.

### C. Constellation of Application Lenses & Screenless Modalities
Layer 6 projects the workshop surface through ruggedized, context-specific lenses that coalesce up to $390+$ concurrent shop workflows into calm, solar-batched shifts:

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| ***The Workshop* (Durable Cycles, Custody & Retrofit Lens)** | Desktop / Tablet / Crib Kiosk (`workshop:steward`, `tool:unlock`) | Thermodynamic counterpart to *The Pantry*: ignores calendar expiration for nails, hand tools, or CNC mills, tracking discrete usage-cycle wear (`cumulativeUseMs`, `wearIndexBp`), custody leases, and `HardwareFleetTier` (`Stage I Scavenged` $\to$ `Stage II Hybrid Retrofit` $\to$ `Stage III Open-Spec`); stages 5-minute predictive metric bearing swaps. |
| **Parametric Blueprint & Cost Browser** | Desktop / Tablet (`intent:author`, `crdt:read`) | Browses the 3 Sovereign Libraries (*Ontological Graph*, *Orchestration Registry*, *Physics Baseline*), localizes legacy imperial/craft dimensions (`inches` $\to$ `um`/`mm`), previews pre-sign $\mathbf{C}_{\mathrm{prod}}$ read-backs, and batches heavy CNC milling into afternoon solar surplus (`SoC_bp`). |
| **Dust-Sealed Shop-Floor BOM E-Ink Terminal** | Mounted at CNC / Saw / Welder bays (`tool:unlock`, `bpmn:review`) | Glove-friendly slate wired beside physical **Lockout/Tagout (`LOTO`)** contactors and NFC readers. Tapping a badge verifies an unlinkable `Scenario Kappa` safety proof in $<50\,\mathrm{ms}$; opening a blade guard drops power in $<10\,\mathrm{ms}$ in hardware regardless of software state. |
| **Printable QR Paper Batch Traveler** | Zero-Screen Workcells | Paper routing slip with QR codes accompanying timber or metal stock from rough-cut to finish assembly; stewards scan stage completion with a handheld reader without touching a screen. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Scenario Alpha's governance evaluates process voice strictly over **Node Membership ($N_s$ active shop stewards)** via $V_i(t) = B + R_i(t) = 1 + R_i(t)$ ($V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)$), while serving the wider circle ($N_p$) through *The Workshop* custody leases and zero-surveillance demand capture:

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person served}$):**
   * *Context:* A single homesteader or maker operating their own garage workbench, 3D printer, and solar battery (often co-located inside a House Enveloping Node alongside a Household Hearth).
   * *Governance Model:* **Zero meetings, zero votes, zero bureaucracy ($V_1(t) = 1 + R_1(t) = 100\%$).** The solo maker can import external Layer 5 tool-safety templates and turnkey Sovereign Library blueprints while governance acts strictly as an automated personal safety and Enveloping Node boundary companion: hardware `LOTO` interlocks, automatic scheduling of heavy spindle jobs during solar peak (`SoC_bp`), Enveloping Node acoustic lease compliance ($\le 550\,\mathrm{cB}$ day / $\le 380\,\mathrm{cB}$ night), and personal `Scenario Sigma` rest shields.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{–}3\text{ core makers}, N_p > 1\text{ people served}$):**
   * *Context:* One skilled maker (or a 2–3 person micro-pod) stewarding a well-equipped garage shop or tool library that serves an extended household, a 10-home neighborhood pod, or local tool borrowers.
   * *Governance Model:*
     * **Pillar II Work Capabilities & Automated Competency Shielding:** Under Layer 5's **Four Constitutional Pillars**, when any of the $N_p$ neighbors wants to borrow a table saw or book a `PersonalSlotIntent` on the CNC mill, the primary steward never has to play gatekeeper—*The Workshop* terminal unlocks strictly upon verifying an unlinkable `Scenario Kappa` `BBS+` safety credential (`T1` dual-key mentor tether or `T2+` solo) for that tool class.
     * **Pull-Only Queue & Anti-Martyrdom Protection:** Fabrication and repair requests from the $N_p$ circle enter an asynchronous, pull-only Commons Pool rather than pinging the steward directly. If fabrication load concentrates on one steward ($>4000\,\mathrm{bp}$), `Scenario Sigma` caps queue admission and triggers `Scenario Kappa` apprenticeship pairing (where apprentices earn initial $\Delta R_i(t)$ from zero by executing **Stage II Right-to-Repair Hybrid Retrofits**).
     * **Inviolable $R_0$ Creative Privacy:** Any qualified person booking an off-peak `PersonalSlotIntent` enjoys total creative privacy: the ledger accounts only for spindle exergy and discrete tool wear in *The Workshop*, logging **zero telemetry** on what personal item was crafted.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A neighborhood or campus Fabrication Guild ($N_s = 5\text{–}40+$ craft stewards) operating multi-modal bays and fulfilling recursive BOM treaties across a federated watershed ($N_p = 150\text{–}2{,}500+$ residents).
   * *Governance Model:*
     * **Bicameral Craft vs. Ethical Envelopes ($N_s > R_i(t) + 2$):** Within the guild, $V_i(t) = 1 + R_i(t)$ pairs earned craft reputation $R_i(t)$ with a non-decaying $B = 1$ ($10{,}000\,\mathrm{bp}$) baseline that prevents single-maintainer hijack when a mature blueprint repository rests in **Dormant Equilibrium**. Simultaneously, the **Commons Chamber** ($N_p$ residents, $1\text{-person-}1\text{-vote}$) holds an unconditional **Ethical Design Veto**—blocking weapon components, covert surveillance hardware, or planned-obsolescence products—and sets ecological/acoustic floors ($F_{\mathrm{eco}}$).
     * **Workcell Conscientious Refusal & Stigmergic Evolution (`collective:thermodynamicProof`):** Neither the node nor any individual steward is a passive job shop; both retain an absolute right of conscientious refusal. Across peered nodes, completed repairs and lower per-cycle $\lambda_{\mathrm{ERC}}$ update `collective:thermodynamicProof` rankings in the Sovereign Libraries, driving voluntary convergence toward open modular standards (`M6`/`M8` metric fasteners, ISO bearings, Euro-totes).

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

When Scenario Alpha interacts with the legacy economy across `l6_l7.sock` (`col-adversaryd`), governance strictly separates **extractive/transitional transactions** from **purely defensive shielding**:

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Irreducible Leaf Procurement (`ProcurementIntent`):** When a recursive BOM hits a leaf component no mesh node can yet manufacture (e.g., SiC MOSFET ICs, carbide inserts, precision ball-screws).<br>**B. Capped Surplus Custom Fab (`policy:commons_plus_l7_zero_bill`):** Selling custom CNC/woodwork output to external buyers to pay down legacy property taxes, utility bills, or tool leases. | • **Dependency Register Logging ($D_t$):** Every imported leaf part is automatically logged on the **Legacy Dependency Register**, elevating high-frequency imports into `Scenario Theta` / `Scenario Zero` import-substitution priorities.<br>• **Automatic $\Phi_t = 0$ Shutoff:** External commercial fabrication is permitted strictly above local ecological and baseline floors ($F_{\mathrm{eco}} + B_{\mathrm{baseline}}$) and **automatically shuts off** the instant monthly Layer 7 fiat bills and escrow endowment targets hit $0\text{ cents}$. |
| **2. Purely Defensive Reach into Layer 7** | **A. Associational & Statutory Shielding:** Protecting neighborhood tool libraries and repair workshops from commercial licensing harassment or OEM intellectual-property takedowns.<br>**B. Supply-Chain OpSec (`Mundane Semantic Camouflage`):** Preventing external vendors or data brokers from inferring internal node capabilities or resident habits from parts orders. | • **Tripartite Legal Carapace:** Wraps neighborhood tool sharing in **UUNAA Section 6** private unincorporated association covenants and statutory **Right-to-Repair** exemptions, commercial micro-factories in **IRC Subchapter T** cooperative charters, and workshop land in **CLT / PPT** trusts.<br>• **Camouflage Scrubbing:** `col-commonsd` strips all internal `did:peer` identities, blueprint trees, and end-use context before crossing `l6_l7.sock`, emitting only aggregated, mundane GPO wholesale purchase orders. |

---

## 7. Horizon Council v3.2 Architectural Binding (*The Architecture of Boundless Time* — D1–D8)

| Architectural Dimension | Canonical v3.2 Binding for `SCN-ALPHA-FAB` (`alpha_fabrication`) |
| :--- | :--- |
| **1. Node Membership Regime ($N_s$) & Unified Voting Formula $V_i(t) = 1 + R_i(t)$ (`CCI-2`)** | $N_s = 1 \to 6$ active machinists/woodworkers per bay ($V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)$). Pairs the non-decaying $B = 1$ ($10{,}000\,\mathrm{bp}$) baseline with **Pillar II (`collective:WorkCapabilityMatrix`)** dual-key mentor-apprentice ladders (`Scenario Kappa`), allowing newcomers with $R_i(0) = 0$ to earn initial stewardship weight by executing **Stage II Right-to-Repair Hybrid Retrofits** (`ProofOfStewardshipRetrofitRecordV50`). |
| **2. Primary Layer 6 Application Lens & Edge SI Localization (`CCI-1`, `CCI-3`)** | ***The Workshop Lens*** (durable mechanical cycle wear, spindle hours, calibration intervals, and custody leases—explicitly contrasted against *The Pantry*'s perishable Arrhenius TTL curves). Localizes legacy imperial/craft units (`1.5 inches` $\to 38{,}100\,\mu\mathrm{m}$ / `38.1 mm`) at the human edge into canonical 64-bit SI integers before Stage 4 Read-Back, while guaranteeing `0` telemetry on off-peak `PersonalSlotIntent` creations in $R_0$. |
| **3. Enveloping Node Spatial Hypervisor Lease (`CCI-4`)** | Operates inside the **Industrial Bay / Garage Workshop Envelope** under domain-blind physical capability leases from the Enveloping Node Hypervisor: acoustic ceiling $\le 8{,}500\,\mathrm{cB}$ ($85.0\,\mathrm{dBA}$) daytime / $\le 5{,}000\,\mathrm{cB}$ ($50.0\,\mathrm{dBA}$) evening (or $\le 3{,}800\,\mathrm{cB}$ residential night curfew), and electrical feeder lease $4{,}500{,}000\,\mathrm{mW}$ ($4.5\,\mathrm{kW}$ peak spindle/welder draw). |
| **4. Open-Source Franchising, L4 3-Tier Cascade & Stage I$\to$III Fleet Progression (`CCI-5`, `CCI-6`)** | Pulls turnkey workshop blueprints from the 3 Sovereign Libraries; resolves local tool differences via **Tier 1 SI Parameter Scaling** (feed-rate/pass splitting) and **Tier 2 Macro-Activity Substitution** (splicing an `M6` bolted flange sub-workflow in place of a laser weld), halting on **Tier 3 True-Constraint Hard-Blocks** (decompiled to a Layer 6 Semantic Diff). Eliminates the 4 penalties of **Stage I Scavenged Layer 7 CNCs/Appliances** by fabricating **Stage II Open RISC-V GRBL/LinuxCNC Retrofit Kits** and **Stage III Native Open-Spec Machines** that stream continuous harmonic bearing wear for 5-minute predictive swaps (**Zero-Stat Liberation**). |
