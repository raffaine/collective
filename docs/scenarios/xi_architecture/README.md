# Scenario Xi: Two-Track Parametric Open Building & Kigumi Barn-Raising (`SCN-XI-ARCH`)

* **Identifier:** `SCN-XI-ARCH`
* **Horizontal Wave:** Wave D (Regenerative Land, Extraction, Materials & Architecture)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Xi governs **how the settlement designs, engineers, retrofits, raises, and adapts its physical buildings and civil structures**. Synthesizing cured coppice timber from `Scenario Eta`, salvaged beams and stone from `Scenario Mu`, bronze/steel connectors from `Scenario Theta`, and CNC joinery from `Scenario Alpha`, Scenario Xi replaces disposable drywall-and-glue suburban tract construction with multi-century **Parametric Open Building**.

Its architecture rests on N. John Habraken's **Two-Track Open Building Doctrine**, which cleanly separates collective structural safety from personal domestic freedom:
* **Track 1 — Communal Structural `Support` ($R_1 / R_2$):** Long-life load-bearing timber frames (reversible nail-free CNC *Kigumi* mortise-and-tenon joinery), seismic foundations, hemp-lime/rammed-earth thermal envelopes, fire-rated party walls, and shared utility chases—engineered with full integer Finite Element Analysis (`FEA` safety factor $\ge 25{,}000\,\mathrm{bp}$ / $2.5\times$) and raised by joyful community barn-raising crews.
* **Track 2 — Sovereign Household `Infill` ($R_0$):** Non-load-bearing interior room partitions, cabinetry, paint, decor, and domestic layouts inside a resident's home—which belong $100\%$ to the household with **zero assembly votes, zero aesthetic committees, and zero interior digital-twin scanning**.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy construction and suburban development industry (Layer 7), the built environment suffers from four structural pathologies:

1. **High-Entropy Portland Cement & Un-Deconstructable Glue-Tract Boxes:** Commercial builders pour carbon-intensive Portland cement slabs and staple together OSB sheathing, spray-foam insulation, and vinyl siding that trap moisture, off-gas volatile organic compounds (`VOCs`), rot within 30–50 years, and can only be demolished into landfill rubble (`Scenario Mu`).
2. **Authoritarian HOA Aesthetic Policing vs. Rigid Floorplans:** Suburban Homeowners' Associations (`HOAs`) micromanage paint colors, porch furniture, and front-yard vegetable gardens, while load-bearing interior walls and buried plumbing prevent families from adapting their space when children arrive (`Scenario Delta`) or elders move in (`Scenario Phi`).
3. **Weaponized Building Codes Against Natural Materials:** Municipal permitting offices routinely rubber-stamp toxic synthetic tract homes while throwing bureaucratic roadblocks at mass-timber joinery, hempcrete/cob thermal walls, and salvaged structural beams.
4. **Construction Site Injury & Exploitative Subcontracting:** Commercial job sites rely on fragmented, underpaid subcontracting chains cutting corners on scaffolding, fall arrest, and dust extraction.

---

## 3. Wide Feature Horizon

Scenario Xi replaces disposable tract boxes and HOA conformity with five regenerative architectural capabilities:

1. **Pragmatic Stage-1 Suburban Retrofits & Fence Unification:** Before building from scratch on raw land, Stage 1A/1B neighborhoods retrofit existing suburban houses—air-sealing envelopes (blower-door verified), blowing dense-pack cellulose/hemp insulation, adding exterior solar thermal/shading pergolas, and voluntarily removing contiguous backyard fences between peered lots to create unified `Scenario Gamma`/`Eta` parklands and `Scenario Delta` play commons.
2. **Reversible CNC *Kigumi* Timber Joinery (`Design for Deconstruction`):** Structural frames use precision CNC-milled interlocking mortise-and-tenon, dovetail, and wooden-peg *Kigumi* joints cured to $\le 1200\,\mathrm{bp}$ ($12.0\%$) moisture equilibrium. Centuries from now—or whenever a building is reconfigured—every beam can be unpegged and reused at $100\%$ structural integrity without sawing or landfilling.
3. **Biogenic Carbon-Negative Envelopes & Pragmatic Material Transitions:** Pairs mass-timber frames with breathable hemp-lime (*hempcrete*), straw-clay, or compressed earth block (`CEB`) infill walls and `Scenario Mu` urbanite/stone footings. Where local soil bearing or seismic codes require transitional steel piers or low-carbon geopolymer/pozzolan footings, Layer 5's `NodeArchitecturalPolicy` governs pragmatic Layer 7 $\leftrightarrow$ Layer 1 engineering transitions without dogmatic rigidity.
4. **Habraken Two-Track Separation (`Support` vs. `Infill`):** Track 1 structural `Support` undergoes rigorous `FEA`, LiDAR dimensional checks, and fire/seismic verification; Track 2 interior `Infill` uses modular demountable wall panels and accessible raceways that residents can rearrange over a weekend with zero node telemetry (`track2InteriorInfillTelemetryEnabled: false`).
5. **Choreographed Community Barn-Raisings:** Translates complex structural assemblies into color-coded, numbered timber kits where community work-bee crews (`Scenario Pi`/`Sigma`) raise an entire cottage or guild hall frame in a celebratory weekend using gin poles, manual/electric hoists, and `Scenario Kappa` fall-arrest safety interlocks.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`ParametricSupportBuildIntent` (Track 1):** Proposing a new communal building frame, courtyard addition, or structural envelope retrofit—binding integer `FEA` load proofs ($\ge 25{,}000\,\mathrm{bp}$ safety factor), timber moisture checks ($\le 1200\,\mathrm{bp}$), and `Scenario Alpha` CNC *Kigumi* cutting jobs.
* **`PersonalHearthInfillNotice` (Track 2):** Optional utility-interface check when a household rearranges non-load-bearing interior partitions or cabinets—verifying only that fire egress doors and shared wet-wall shutoff valves remain unobstructed, with **zero interior layout or aesthetic logging**.
* **`FenceUnificationAndRetrofitIntent`:** Coordinating contiguous suburban neighbors to open shared garden/courtyard corridors or execute a multi-home insulation/heat-pump retrofit bee.
* **`PragmaticL7PermitVariance`:** Compiling licensed Professional Engineer (`PE`) structural calculations and International Building Code (`IBC`/`IRC`) alternative-materials equivalency packets for municipal filing.

### B. Semantic Representations & Deterministic Read-Backs
* **Habraken Track-1 / Track-2 Graph Partitioning (`https://w3id.org/collective/v1/`):** In `col-commonsd`, Track 1 `Support` elements (columns, primary beams, foundation piers, roof trusses, party walls) carry full structural lineage, moisture-cure history, and integer `FEA` stress tensors. At the interior surface of the dwelling shell, the knowledge graph stops (`track2InteriorInfillTelemetryEnabled: false`), protecting $R_0$ domestic privacy (`Scenario Zeta`).
* **Mandatory Structural & Permit Read-Back:** Before an architect, `PE` steward, and neighborhood assembly co-sign a Track 1 `ParametricSupportBuildIntent`, their slates present a **Deterministic Read-Back** confirming: (1) verified safety factor $\ge 25{,}000\,\mathrm{bp}$ ($2.5\times$) under regional snow/wind/seismic loads, (2) timber moisture $\le 1200\,\mathrm{bp}$, (3) zero encroachment on `Scenario Eta` Mother-Tree Sanctuary Polygons, and (4) complete Track 2 interior privacy.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Parametric Kigumi Barn-Raising Tablet** | Construction Site / Timber Frame Yard (`xi:builder`, `bpmn:review`) | Offline 3D assembly guide showing step-by-step pegging order, hoist rigging points, and LiDAR plumb/level checks during community barn-raising bees. |
| **Habraken Track-2 Home Infill Sketchpad** | Resident's Personal Slate (`r0:local_only`) | Local-only room layout and cabinet planner that highlights where Track 1 load-bearing columns and wet-wall pipes sit so residents can freely move Track 2 interior walls without ever uploading their floorplan. |
| **Structural Code & PE Equivalency Compiler** | Guild Engineering Desk (`xi:engineer`, `carapace:fiduciary`) | Translates internal integer `FEA` proofs and salvaged-timber grading tests into standard `IBC` Section 104.11 / `IRC` alternative-materials permit drawings for `col-adversaryd`. |
| **Chiseled Roman-Numeral & QR Joinery Marks** | Milled Directly into Timber Tenons (`alpha:cnc`) | Screenless assembly on site: every CNC-milled beam carries traditional carpenter marriage marks (`I`, `II`, `III`) so barn-raising crews fit joints by eye and mallet without looking at a screen. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ resident}$):**
   * *Context:* A solo homesteader insulating their house, building a backyard solar pergola or greenhouse, or rearranging interior Track 2 partitions.
   * *Governance Model:* **Zero committee approval; automated structural safety helper.** Track 2 interior work requires zero notices; for outdoor garden sheds or pergolas, the solo builder gets instant offline span/snow-load tables and `Kappa` ladder/roof-harness safety reminders, while calling a neighborhood work-bee (`Scenario Pi`) whenever a heavy ridge beam needs four pairs of hands to lift.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ household or co-housing pod}$):**
   * *Context:* One or two skilled carpenters/builders leading a backyard fence-removal bee, a shared courtyard timber pavilion, or an Accessory Dwelling Unit (`ADU`) retrofit for $5\text{–}15$ contiguous suburban households ($N_p > 1$).
   * *Governance Model:*
     * **Unanimous Adjoining-Neighbor Consent on Boundary Changes:** Removing a fence or building a shared garden arbor between Lot 402 and Lot 404 requires the co-signature of the directly adjoining households, while interior rooms of each house remain strictly in Track 2 ($R_0$).
     * **Work-Site Safety Interlocks (`Kappa L1+`):** Volunteer neighbors joining a weekend barn-raising bee are matched to ground rigging, peg driving, or hemp-lime tamping roles matching their `Scenario Kappa` safety tier, with hard hat and fall-arrest checks verified before hoist operation.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A settlement Architecture, Timber-Framing & Civil Guild ($N_s = 15\text{–}80+$ carpenters, masons, structural engineers, and earth-builders) raising multi-story `CLT` housing (`Scenario Zeta`), guild halls (`Scenario Epsilon`), and bridges across $R_2 \dots R_4$.
   * *Governance Model:*
     * **Bicameral Track-1 Ratification vs. Track-2 Non-Interference:** Track 1 public/communal footprints, solar-access shadow envelopes (ensuring a new building never shades a neighbor's `Scenario Gamma` garden or solar array), and `FEA` safety factors ($\ge 25{,}000\,\mathrm{bp}$) require Stewardship + Commons Chamber ratification. Conversely, the assembly is **constitutionally barred from voting on Track 2 interior aesthetics or domestic layouts** (`SCHEMA_VIOLATION_MORALISTIC_OVERREACH`).
     * **Pragmatic Material Transition Policy:** The guild and assembly periodically update `NodeArchitecturalPolicy` to phase out legacy Portland cement and synthetic membranes as local `Scenario Theta` lime/pozzolan kilns and `Scenario Eta`/`Nu` biocomposites scale up.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Transitional Code-Required Materials (`ProcurementIntent`):** Procuring tempered safety glass, EPDM roof membranes, seismic hold-down bolts, or low-carbon cement where local `Theta`/`Alpha` production cannot yet satisfy municipal structural code.<br>**B. Timber-Frame & Ecological Retrofit Contracting (`policy:commons_plus_l7_zero_bill`):** Building high-efficiency timber sunrooms or blower-door retrofits for external clients to fund heavy sawmill/crane tooling. | • **Embodied-Carbon & Dependency Tracking ($D_{\mathrm{materials}}$):** Every kilogram of imported steel, glass, or cement is logged on the Legacy Dependency Register, prioritizing `Scenario Mu` urban-mining salvage and `Scenario Theta` glass/metal capacity.<br>• **$\Phi_t = 0$ Commercial Cap:** External contracting halts automatically once monthly node fiat obligations and tool-acquisition budgets hit $0\text{ cents}$. |
| **2. Purely Defensive Reach into Layer 7** | **A. Building Permit Armor (`IBC` Section 104.11 Alternative Materials & `ADU` Preemption):** Preventing hostile municipal building departments from red-tagging mass-timber *Kigumi*, hemp-lime walls, or salvaged structural beams.<br>**B. Interior Floorplan OpSec (`Mundane Semantic Camouflage`):** Protecting residents' private room uses from external tax assessors or inspectors. | • **Licensed `PE` Stamp Pool & Code-Equivalency Dossiers:** `col-commonsd` maintains a federated pool of `Kappa`-certified stewards who also hold active Layer 7 **Professional Engineer (`PE`) / Registered Architect** licenses—stamping alternative-material `FEA` calculations under `IBC` Section 104.11 and state **Accessory Dwelling Unit (`ADU`)** preemption statutes.<br>• **Track-1-Only Permit Drawings:** External permit filings show only code-required Track 1 structural shells and wet-wall egress, never exposing internal Track 2 household life. |
