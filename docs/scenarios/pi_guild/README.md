# Scenario Pi: Polycentric Guild Patchwork, Demand-Capture Beacons & Ricardian Treaties (`SCN-PI-GUILD`)

* **Identifier:** `SCN-PI-GUILD`
* **Horizontal Wave:** Wave E (Third Place, Epistemic R&D, Culture & Polycentric Guilds)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Pi governs **how the entire polycentric patchwork of spatial neighborhoods, non-spatial craft guilds, and temporary inter-node swarms organizes itself without a central state or corporate hierarchy**. While `Scenario Kappa` governs individual apprenticeship and skill credentials, Scenario Pi governs **collective organizational morphogenesis, demand capture, and multi-party federation**:
* **The Three-Dimensional Organizational Patchwork:** Weaving together (1) *Spatial Polities* (contiguous households, neighborhoods, and watershed villages sharing physical land and microgrids), (2) *Non-Spatial Craft Guilds* (overlapping fellowships of bakers, machinists, paramedics, coders, and foresters spanning across nodes), and (3) *Ephemeral Work-Bee Swarms* (temporary task forces that assemble for a barn-raising or harvest and dissolve automatically upon completion).
* **Privacy-Preserving Demand Capture (`MeshGrowthBeacon`):** Aggregating individual unmet Intents, long-haul transport bottlenecks, and Layer 7 import fallbacks into stigmergic beacons that guide where new nodes and guild capacities naturally grow.
* **Superlinear Post-Scarcity Synergy & Anti-Isolationism:** Ensuring that every node added to the mesh strengthens the whole while preserving local cellular autonomy—preventing both freeloading extraction ("siphoning") and rich-node hoarding ("isolationism").

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy economic and political organization (Layer 7), coordinating production across communities is trapped between two dysfunctional extremes:

1. **Blind Market Speculation vs. Central-Planning Rigidity:** Capitalist markets rely on price gouging, advertising manipulation, and boom-bust bankruptcies to signal demand, while state central planning suffocates local initiative under distant, out-of-touch bureaucratic quotas.
2. **Corporate Monopoly & Supply-Chain Single Points of Failure:** Profit-maximizing supply chains consolidate production into single massive factories ($k = 1$), so a single fire, strike, or blockade paralyzes an entire region.
3. **Ossified Bureaucracies That Never Dissolve:** In both corporations and state agencies, temporary committees hired to solve a specific problem turn into permanent self-perpetuating fiefdoms that invent busywork to justify their rank.
4. **Fragmented Anti-Collaboration or Parasitic Free-Riding:** Decentralized networks without constitutional reciprocity guards either fragment into selfish, isolated bunkers that refuse to share surplus or get drained by extractive nodes that take without contributing.

---

## 3. Wide Feature Horizon

Scenario Pi replaces market chaos and bureaucratic hierarchy with five polycentric coordination mechanisms:

1. **Spatial $\times$ Non-Spatial $\times$ Ephemeral Swarm Patchwork:** Any steward belongs simultaneously to their physical residential neighborhood ($R_1 / R_2$ Spatial Polity) and one or more cross-cutting Craft Guilds ($R_2 \dots R_5$ Non-Spatial Guilds). When a multi-disciplinary project arises (e.g., building a new `Scenario Chi` clinic requiring `Xi` timber framers, `Alpha` machinists, `Beta` electricians, and `Gamma` hospitality), `col-commonsd` convenes an **Ephemeral Work-Bee Swarm** whose administrative authority **automatically dissolves (`autoDissolveOnCompletion: true`)** the moment the delivery receipt is signed.
2. **4-Stage Multi-Party Ricardian Treaties (`Treaty_CID` + FROST):** Inter-guild and inter-node covenants are authored in the Ricardian Treaty Studio—cryptographically binding (1) human-readable prose, (2) canonical W3C JSON-LD schemas, and (3) deterministic Layer 4 BPMN execution invariants into a single `Treaty_CID` co-signed via threshold `FROST` hardware keys.
3. **Intent-Driven Demand Capture & Stigmergic Growth Beacons (`AggregatedIntentDemandSignal` $\to$ `MeshGrowthBeacon`):** How does the mesh know what new workshops, orchards, or sister nodes to build next? Without exposing any individual's private intents, `col-commonsd` aggregates:
   * Recurring local cache misses that forced a Layer 7 `ProcurementIntent` fallback ($D_t$ hotspots),
   * High `Scenario Beta` haul-exergy routes (e.g., hauling heavy grain or castings $80\,\mathrm{km}$ every week), and
   * Single-point-of-failure guilds where regional producer redundancy drops below $k < 3$.
   It publishes a non-coercive **`MeshGrowthBeacon`** showing the exact haul-exergy savings (e.g., `7500 bp` / $75\%$ haul reduction) and pre-committed bilateral demand waiting for any steward team that seeds that missing capacity nearby (`Scenario Tau` + `Scenario Kappa` fellowships).
4. **Superlinear Mesh Synergy & Pairwise Zero-Sum `BCC` Channels:** Inter-node trade clears across Pairwise Bilateral Clearing Channels ($B_{u \to v}(t) + B_{v \to u}(t) = 0\,\mu\mathrm{J}$) where active reciprocal collaboration organically expands bilateral credit headroom ($+1000\,\mathrm{bp}$), while multi-hop chained HTLCs settle complex BOMs across valleys with zero global token or central bank.
5. **Dual Integrity Guards (Anti-Isolationism Demurrage & Anti-Siphon Throttling):**
   * *Anti-Isolationism:* A resource-rich node cannot hoard surplus capacity in selfish autarky while refusing to peer; idle unshared surplus above local $F_{\mathrm{eco}} + B_{\mathrm{baseline}}$ buffers undergoes gentle reputational/credit decay that is refreshed by fulfilling peer treaties or seeding new `Scenario Tau` nodes.
   * *Anti-Siphon:* Conversely, if an external peer chronically draws maximum credit headroom without reciprocating or sharing verified `Scenario Upsilon` shock telemetry, its bilateral credit window smoothly throttles down (while never cutting anyone's internal $R_0$ survival floor).

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`PolycentricMultiPartyRicardianTreaty`:** Proposing, simulating (`Oasis` `0x01`), and threshold-signing an inter-guild or inter-node mutual-aid, BOM supply, or watershed stewardship treaty.
* **`AggregatedIntentDemandSignal` & `MeshGrowthBeacon`:** Publishing a privacy-preserving regional demand signal inviting guilds or new Stage 1/2 nodes to stand up a needed workshop, foundry, or clinic close to where demand is concentrated.
* **`WorkBeeSwarmCharter` & `WorkBeeSwarmDeliveryReceipt`:** Convening a temporary cross-guild task force with a hard cryptographic sunset upon task completion.
* **`BPVPCapabilityAdvertisement`:** Advertising ratified guild blueprints, surplus stocks, and lead times across Trust Rings $R_1 \dots R_5$.

### B. Semantic Representations & Deterministic Read-Backs
* **Tripartite Ricardian Equivalence (`Treaty_CID`):** Ensures no party can ever sign a treaty whose human prose says one thing while the underlying BPMN state machine executes another:
  $$\mathrm{Treaty\_CID} = \mathrm{SHA256}\bigl(\mathrm{len}(\mathrm{Text}_{\mathrm{prose}}) \parallel \mathrm{Text}_{\mathrm{prose}} \parallel \mathrm{len}(\mathrm{Schema}_{\mathrm{JCS}}) \parallel \mathrm{Schema}_{\mathrm{JCS}} \parallel \mathrm{len}(\mathrm{BPMN}) \parallel \mathrm{BPMN}\bigr)$$
* **Mandatory 4-Stage Treaty & Swarm Read-Back:** Before a guild or neighborhood assembly co-signs a `Treaty_CID` via `FROST`, the Ricardian Treaty Studio renders a **Deterministic Read-Back** displaying: (1) maximum exergy/material commitment bounds, (2) proof that local $F_{\mathrm{eco}} + B_{\mathrm{baseline}}$ reserves cannot be breached by the treaty, (3) `Oasis` shadow-simulation results, and (4) the exact unilateral exit / sunset conditions.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **4-Stage Ricardian Treaty Studio** | Guild Hall / Agora Tablet (`pi:treaty_author`, `gov:ratify`) | Side-by-side prose, JSON-LD, and visual BPMN flow editor with integrated `Oasis` (`0x01`) stress-test preview and threshold `FROST` hardware signing. |
| **Stigmergic Mesh Growth Beacon Map** | Bioregional Observatory Slate (`crdt:read`, `pi:beacon`) | Visual contour map of the watershed highlighting high-haul-exergy bottlenecks, $D_t$ import-substitution opportunities, and $k < 3$ redundancy gaps so young guilds know where their craft is most needed. |
| **Ephemeral Work-Bee Swarm Board** | Barn-Raising / Harvest Field Slate (`pi:swarm`, `bpmn:review`) | Coordinates multi-guild volunteers, tool kits (`Alpha`), and hot tiffins (`Gamma`) for a weekend project, then visibly archives and dissolves the swarm structure when the last peg is driven. |
| **Pairwise `BCC` Reciprocity & Headroom Gauge** | Guild Steward Slate (`crdt:read`) | Calm thermometer view of pairwise bilateral channel balances ($B_{u \to v} + B_{v \to u} = 0$) and seasonal counter-trade commitments across neighboring valleys. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ solo node}$):**
   * *Context:* A solo homesteader or artisan node peering for the first time with 3–4 neighboring households ($R_1$).
   * *Governance Model:* **1-of-1 Sovereign Peering & Instant Demand Discovery.** The solo steward holds their own key (or a 2-of-3 social-recovery split via `Scenario Tau`), peers bilaterally with neighbors via lightweight `BCC` channels, and uses the **Mesh Growth Beacon Map** to see what local neighbors need most (e.g., discovering that five nearby households need a shared solar dehydrator or bike-repair bench).
2. **Single / Core Steward Guild Serving a Wider Circle ($N_s = 2\text{–}5\text{ guild stewards}, N_p > 1\text{ neighborhood pod}$):**
   * *Context:* A small neighborhood baking, fabrication, or agroforestry pod ($N_s = 2\text{–}5$) serving $15\text{–}50$ residents ($N_p > 1$).
   * *Governance Model:*
     * **Threshold `FROST` Co-Signing (2-of-3 / 3-of-5):** External guild commitments and `GPO` pools use threshold multi-signatures so no single steward can accidentally over-commit the pod's workshop or larder.
     * **Automatic Redundancy Seeding ($k \ge 3$):** If the neighborhood pod is the *only* producer ($k = 1$) of a vital good in the valley, `col-commonsd` automatically emits a `MeshGrowthBeacon` and `Scenario Kappa` fellowship to help a second and third pod stand up redundant capacity—preventing monopoly power and protecting the core stewards from burnout (`Scenario Sigma`).
3. **Multi-Steward Polycentric Commonwealth ($N_s > 1\text{ overlapping guilds}, N_p > 1\text{ bioregional federation}$):**
   * *Context:* Dozens of spatial neighborhoods and non-spatial craft guilds ($N_p = 500\text{–}10{,}000+$ residents across $R_2 \dots R_5$).
   * *Governance Model:*
     * **Subsidiarity & Bicameral Balance (Spatial $\times$ Non-Spatial):** Neither the spatial neighborhood nor the non-spatial craft guild rules over the other: the **Non-Spatial Guild** (Stewardship Chamber) governs *how* a craft is practiced safely and well, while the **Spatial Polity** (Commons Chamber) governs *where* physical impacts, land use, and ecological floors ($F_{\mathrm{eco}}$) sit within the local watershed.
     * **Auto-Dissolving Task Forces:** Regional infrastructure projects run through temporary **Work-Bee Swarms** with hard cryptographic expiration dates so cross-node coordination never hardens into a permanent central state bureaucracy.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Federated `GPO` Bulk Bargaining & Import Substitution:** Aggregating demand signals across multiple nodes to negotiate container-scale wholesale prices on solar cells, bearings, or grain while local `MeshGrowthBeacon` projects are under construction.<br>**B. Inter-Co-op Commercial Clearing (`policy:commons_plus_l7_zero_bill`):** Partnering with friendly external Layer 7 worker cooperatives (`Mondragon` / `Rochdale` peers) to exchange specialty goods. | • **Demand-Capture to Import-Substitution Pipeline:** Every aggregated Layer 7 `GPO` order automatically increases the gravity score of the corresponding `MeshGrowthBeacon`, channeling guild investment toward replacing the highest-volume imports first ($D_t \to 0$).<br>• **Zero Speculative Arbitrage:** External trade is strictly bounded by physical node bills ($\Phi_t$) and real material needs—forbidding commodity speculation or financialized derivatives. |
| **2. Purely Defensive Reach into Layer 7** | **A. Multi-Entity Compartmentalization (Anti-Contagion Legal Topology):** Ensuring that a frivolous lawsuit or regulatory attack against one commercial activity can never seize residential land or sister-node tools.<br>**B. Federation Graph OpSec (`Mundane Semantic Camouflage`):** Hiding the polycentric treaty topology from external corporate competitors or state surveillance. | • **Separate Legal Shells per Risk Domain:** Segregates land into unencumbered **CLTs / Perpetual Purpose Trusts (`PPTs`)**, internal sharing into **UUNAA Section 6** private associations, and external trade into capped, liability-isolated **Subchapter T** operating shells.<br>• **Pairwise Treaty Confidentiality:** `Treaty_CID` covenants replicate only across participating peer rings ($R_1 \dots R_4$) and never cross `l6_l7.sock`. |
