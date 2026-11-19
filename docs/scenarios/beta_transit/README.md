# Scenario Beta: Five-Artery Logistics, Cold-Chain Milk-Runs & Autonomous `No-Transit` Veto (`SCN-BETA-TRANSIT`)

* **Identifier:** `SCN-BETA-TRANSIT`
* **Horizontal Wave:** Wave B (Suburban Survival, Energy, Fabrication & Transit Seed)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Beta governs the **five physical arteries** that connect households, workshops, hearths, and sister settlements into a living metabolic organism:
1. **People:** Accessible human mobility via pedestrian greenways, e-bikes, shared EV micro-fleets, and inter-valley shuttles.
2. **Freight & BOM Backhaul:** Consolidated cargo e-trike milk-runs, insulated cold-chain tiffin trailers, and watershed barge/rail corridors.
3. **Electrical Exergy:** $48\,\mathrm{V}$ household DC buses and $380\,\mathrm{V}$ neighborhood HVDC microgrid interties with instantaneous mechanical islanding.
4. **Cascading Water & Thermal Loops:** Rainwater cisterns, spring catchments, greywater reed-bed filtration, and ambient PEX-a hydronic heat-recovery loops.
5. **Data & Delay-Tolerant Packets:** Community dark fiber, $915\,\mathrm{MHz}$ LoRa Reticulum mesh radio, and multi-gigabyte physical sneakernet drives piggybacked on cargo trikes.

Rather than paving neighborhoods for private two-ton automobiles or relying on fragile centralized utility monopolies, Scenario Beta coordinates high-efficiency, human-scale circulation while empowering every community to draw strict **`No-Transit` sanctuary boundaries** and protect watershed river flows.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy urban and suburban infrastructure (Layer 7), the movement of people, goods, energy, water, and data suffers from four structural pathologies:

1. **Automobile Dependency & Point-to-Point Delivery Entropy:** Families are forced to own expensive, depreciating cars just to buy groceries or commute, while gig-economy vans run half-empty point-to-point errands that choke residential streets with noise, exhaust, and tire microplastics.
2. **Brittle Centralized Utility Grids:** Aging municipal AC power grids and single-use potable water mains suffer cascading blackouts and main breaks, offering households zero local buffering while treating every liter of drinking water the same whether it is used for cooking or flushing toilets.
3. **Cold-Chain Opacity & Perishable Spoilage:** Food and clinical medicines shipped through commercial couriers routinely suffer unlogged temperature excursions, either spoiling in transit or requiring wasteful single-use gel packs and styrofoam coolers.
4. **ISP Monopolies & Cloud-Dependent Routing:** Neighborhood communication depends on commercial cellular towers and fiber ISPs that fail during storms and subject all local traffic to corporate packet inspection.

---

## 3. Wide Feature Horizon

Scenario Beta replaces fragmented, fossil-fueled infrastructure with five unified circulation loops:

1. **Clarke-Wright Consolidated Milk-Runs ($\eta_{\mathrm{route}} \ge 6000\,\mathrm{bp}$):** Instead of dispatching ad-hoc single-item trips, `col-commonsd` solves a deterministic integer Capacitated Vehicle Routing Problem (`CVRP`) requiring a minimum $60\%$ load factor—bundling `Scenario Gamma` hot tiffins, `Scenario Alpha` machined brackets, `Scenario Delta` sanitized cloth diapers, and `Scenario Mu` compost backhauls into one calm morning and afternoon cargo-trike loop.
2. **Hardware-Verified Cold-Chain & Two-Phase Custody Handoffs:** Insulated cargo totes carry calibrated load cells ($\pm 5000\,\mathrm{mg}$) and PT100 temperature loggers verifying uninterrupted refrigeration within $[275{,}150,\, 277{,}150]\,\mathrm{mK}$ ($2.0\text{–}4.0^\circ\mathrm{C}$) or hot-hold $\ge 333{,}150\,\mathrm{mK}$ ($60.0^\circ\mathrm{C}$) before releasing Hash Time-Locked Contract (`HTLC`) settlement preimages upon dual-NFC tap.
3. **Piggybacked Delay-Tolerant Networking (`DTN`) Sneakernet:** Every cargo e-trike and watershed shuttle carries an encrypted solid-state `DTN` courier cache—automatically syncing software updates (`Scenario Zero`), Alexandria library mirrors (`Scenario Iota`), and Domain-B Automerge CRDT logs across radio-shadowed valleys at multi-gigabyte bandwidths.
4. **Five-Tier Ecological Water & Microgrid Contention Hierarchy:** Enforces a constitutional $\ge 8000\,\mathrm{bp}$ ($80\%$) natural instream river/spring flow floor for salmonid and riparian ecosystems before any agricultural or workshop intake opens, and cascades potable water $\to$ wash greywater $\to$ reed-bed irrigation $\to$ `Scenario Mu` struvite/biochar recovery.
5. **Spatial `No-Transit` Geofence Polygons & Mechanical Red-Lever Islanding:** Communities draw cryptographic `No-Transit` polygons around childcare pods (`Delta`), wildlife corridors (`Eta`), cultural ceremonies (`Lambda`), and quiet courtyards (`Zeta`), while every household retains a physical manual disconnect lever to island its battery or cistern instantaneously by hand.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`FreightCourierIntent`:** Scheduling a consolidated multi-stop cargo-trike or shuttle run bundling physical totes and `piggybackedDtnBundles` CIDs at $\eta_{\mathrm{route}} \ge 6000\,\mathrm{bp}$.
* **`ExergyIntertieIntent` & `HydroTransferIntent`:** Authorizing peer-to-peer DC microgrid watt-hour sharing, hydronic thermal exchange, or cistern water balancing across neighboring parcels.
* **`NoTransitPolygonAmendment`:** Drawing or updating a spatial sanctuary boundary that bars autonomous carts or heavy freight from traversing quiet or sacred zones (unless overridden by a human-steered `Scenario Chi`/`Upsilon` emergency ambulance/fire rig).
* **`CustodyHandoffReceipt`:** Dual-NFC sender/courier/receiver tap attesting mass and thermal envelope integrity at transfer.

### B. Semantic Representations & Deterministic Read-Backs
* **Multi-Artery Route & Contention Graphs (`https://w3id.org/collective/v1/`):** Models parcel nodes, dark-fiber/LoRa links, HVDC bus headroom, and river instream flow (`instream_flow_preserved_bp >= 8000`) in a unified spatial-topological graph.
* **Cold-Chain & Sanitation Custody Invariant:** `col-commonsd` blocks any `FreightCourierIntent` carrying culinary or clinical perishables from releasing its HTLC settlement preimage $S$ or marking `DELIVERED_SAFE` if the hardware-signed Layer 2 thermal log records any excursion outside $[275{,}150,\, 277{,}150]\,\mathrm{mK}$ (cold) or $< 333{,}150\,\mathrm{mK}$ (hot), automatically re-routing compromised food to `Scenario Mu` compost.
* **Mandatory Intertie & `No-Transit` Read-Back:** Before a household or assembly signs an `ExergyIntertieIntent` or `NoTransitPolygonAmendment`, the lens renders a plain-language **Deterministic Read-Back** showing exact battery `SoC_bp` reserve floors ($R_0$ survival reserve is never drained by peers) and emergency ambulance override rules.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Handlebar Cargo-Trike Domain-B Courier Deck** | Mounted on E-Trike Handlebars (`courier:dtn`, `crdt:sync`) | Sunlight-transmissive monochrome slate with $\ge 64 \times 64\,\mathrm{px}$ gloved buttons operating $100\%$ offline; coalesces dozens of neighborhood drop-offs into 1–2 calm daily milk-runs and auto-syncs DTN sneakernet bundles over Wi-Fi/NFC at each stop. |
| **Mechanical Red-Lever Intertie & Cistern Panel** | Household Utility Wall (`intertie:steward`) | Physical manual contactor lever paired with a 0-mW bistable E-Ink gauge showing local battery `SoC_bp`, cistern liters, and active peer sharing; pulling the red lever physically islands the home in $<5\,\mathrm{ms}$. |
| **Spatial `No-Transit` Sanctuary Map Editor** | Commons Tablet / Desktop (`gov:ratify`) | Lets residents sketch quiet zones around nurseries, elder courts, and nesting groves, previewing alternate cargo routes before ratification. |
| **Dual-NFC Tote Custody Reader** | Porch Drop Box / Loading Bay (`courier:dtn`) | Screenless NFC pad on insulated porch boxes: courier taps tote + badge to verify cold-chain integrity and release HTLC clearing in under one second. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person served}$):**
   * *Context:* A single homesteader managing a $48\,\mathrm{V}$ solar battery, rainwater cistern, greywater reed bed, LoRa radio node, and personal cargo e-trike.
   * *Governance Model:* **Zero meetings, automated five-tier self-protection.** Layer 5 and Layer 2 automatically guard the solo homestead's $R_0$ survival floors—shedding discretionary shop loads before survival heating/refrigeration if solar input drops, and keeping $\ge 8000\,\mathrm{bp}$ of creek flow untouched.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ people served}$):**
   * *Context:* One or two neighborhood couriers / microgrid stewards running the morning cargo-trike milk-run and maintaining the shared $48\,\mathrm{V}$/cistern intertie across $5\text{–}12$ suburban households ($N_p > 1$).
   * *Governance Model:*
     * **Batch Consolidation Against Errand Fatigue:** Rather than letting 12 households ping the courier for individual deliveries all day, the CVRP solver holds non-emergency requests until the $\eta_{\mathrm{route}} \ge 6000\,\mathrm{bp}$ morning or afternoon milk-run window.
     * **Hardware Islanding Sovereignty:** Every one of the $N_p$ connected households retains an unconditional physical Red-Lever override to disconnect its home battery or water tank at any time without asking the microgrid steward.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A watershed Logistics & Grid Guild ($N_s = 10\text{–}60+$ couriers, linemen, and hydrologists) operating $380\,\mathrm{V}$ HVDC interties, electric barges/shuttles, and multi-hop HTLC freight corridors across $R_2 \dots R_5$.
   * *Governance Model:*
     * **Constituent Spatial Veto (`No-Transit`):** Under cellular autonomy, any constituent neighborhood or village ($R_1 / R_2$) can ratify a `No-Transit` polygon over its internal paths; the regional Logistics Guild must route inter-node freight around that sanctuary (except during human-steered `Chi`/`Upsilon` life-safety emergencies).
     * **Chained Multi-Hop HTLC Clearing:** Inter-node freight and HVDC exergy transfers settle atomically across pairwise zero-sum Bilateral Clearing Channels (`BCC`) using cryptographic delivery receipts ($\sigma_{\mathrm{delivery}}$), requiring zero central clearinghouse or fiat toll booths.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Asymmetric Municipal Grid & Water Buffering:** Drawing off-peak municipal power or water during deep winter troughs in Stage 1/2 before local solar/storage reaches $100\%$ autonomy.<br>**B. Last-Mile Consolidated Freight Intake:** Receiving bulk GPO deliveries at a single perimeter depot so commercial diesel trucks never enter residential lanes. | • **One-Way Dependency Reduction ($D_{\mathrm{energy}}, D_{\mathrm{water}} \to 0$):** Layer 3 tracks every imported watt-hour and liter on the Legacy Dependency Register, prioritizing battery/cistern expansion until municipal breakers can be locked open (`Scenario Omega`).<br>• **Perimeter Break-Bulk Quarantine:** Commercial carriers drop pallets at the VLAN 40 / perimeter dock; internal distribution switches immediately to electric cargo trikes and reusable stainless totes. |
| **2. Purely Defensive Reach into Layer 7** | **A. Private Bailment & Utility Easement Shielding:** Preventing state transportation boards or utility monopolies from regulating neighbor-to-neighbor cargo trike milk-runs or cross-lot DC/water lines as commercial common carriers or public utilities.<br>**B. Packet & Route Privacy (`Mundane Semantic Camouflage`):** Protecting inter-node radio/fiber traffic from ISP surveillance. | • **UUNAA Section 6 Gratuitous Bailment & UCEA Easements:** Compiles public-road cargo manifests automatically as non-commercial **Gratuitous Neighbor Bailment Bills of Lading** ($0$ for-hire tariff) and records cross-lot microgrid/water conduits inside **Uniform Conservation Easement Act (`UCEA`)** ecological/utility easements.<br>• **End-to-End Reticulum & WireGuard Encryption:** All packets traversing public rights-of-way or DTN sneakernet drives are hardware-encrypted and padded against traffic analysis. |
