# Scenario Epsilon: Shared Third-Place Portal, Compute Commons & Subchapter T Carapace (`SCN-EPSILON-COWORK`)

* **Identifier:** `SCN-EPSILON-COWORK`
* **Horizontal Wave:** Wave E (Third Place, Epistemic R&D, Culture & Polycentric Guilds)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)
* **Edição Brasil & Lusofonia (PT-BR):** [bra/README.md](./bra/README.md)

---

## 1. Intended Scope & Domain Vision

Scenario Epsilon governs **the civic "Third Place" membrane, the shared liquid-cooled compute commons, and the cooperative legal/fiscal/insurance carapace (`Carapace-as-a-Service`)** that shields the settlement during its transition out of Layer 7.

Its scope bridges three vital functions:
* **The Welcoming Civic Third Place:** A neighborhood café, study hall, printshop, and meeting commons where residents and surrounding non-member neighbors gather, collaborate, and inspect public environmental metrics via a zero-ID portal.
* **Dual-VLAN Coworking & Liquid-Cooled Compute Commons:** Providing acoustically calm deep-focus workspaces ($\le 400\,\mathrm{cB}$) and shared high-density compute racks (whose waste heat warms `Scenario Zeta` bathhouses and `Scenario Gamma` greenhouses), physically segmented between VLAN 40 (external client laptops/VPNs) and VLAN 30 (internal sovereign governance).
* **Shared Cooperative Carapace (`Carapace-as-a-Service`):** Pooling bookkeeping, tax withholding, Group Purchasing Organization (`GPO`) wholesale procurement, and umbrella liability insurance inside a single role-gated legal wrapper so craft guilds (`Alpha`, `Gamma`, `Zero`, `Xi`) never have to drown in corporate paperwork.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In legacy cities and suburbs (Layer 7), remote work, small-business administration, and civic gathering suffer from four structural failures:

1. **The Disappearance of Non-Commercial Third Places:** Libraries and parks are defunded while cafés require continuous purchases and blast commercial music—leaving suburban remote workers isolated in lonely bedrooms and neighborhoods without a civic gathering hall.
2. **Crushing Administrative & Compliance Overhead for Small Makers:** An independent baker, carpenter, or indie software studio trying to operate legally in Layer 7 must separately pay for LLC filings, commercial general liability insurance, CPA tax prep, payment processors, and legal retainers—strangling small craft cooperatives before they start.
3. **Corporate Laptop / Home Network Cross-Contamination:** Remote workers plug employer-managed laptops loaded with corporate spyware/MDM agents directly into their home Wi-Fi, exposing household devices and community networks to corporate surveillance.
4. **Data-Center Thermal Waste:** Commercial cloud servers burn megawatts of air-conditioning energy to dump server heat into the sky while apartments next door burn fossil gas for hot water.

---

## 3. Wide Feature Horizon

Scenario Epsilon replaces isolated remote work and fragmented administrative bureaucracy with five integrated capabilities:

1. **The Zero-ID Open Municipal Neighbor Portal:** Mounted at the street-facing Third-Place vestibule, allowing any surrounding Layer 7 neighbor—with **zero `did:peer` identity, zero cookies, and zero tracking**—to view live boundary noise/air-quality proofs (`Scenario Lambda`/`Theta`), book community meeting tables, sign up for tool-library orientations (`Scenario Alpha`), or file a formal neighborhood concern.
2. **Hardware Dual-VLAN Segmentation (VLAN 40 DMZ vs. VLAN 30 Sovereign):** Physical Ethernet ports and Wi-Fi SSIDs at coworking desks are hardware-switched: stewards doing external Layer 7 remote jobs connect strictly to VLAN 40 (routed straight through the outbound firewall with zero visibility into VLAN 30), while sovereign node lenses run exclusively on local VLAN 30.
3. **Acoustic Zoned Third-Place Stewardship:** Combines vibrant social café/salon commons with silent deep-focus reading and coding rooms enforced at $\le 400\,\mathrm{cB}$ ($40\,\mathrm{dBA}$) via passive wood-wool/hemp acoustic panels (`Scenario Xi`/`Nu`) and E-Ink desk placards.
4. **`Carapace-as-a-Service` (Subchapter T Co-op, GPO & Umbrella Insurance Pool):** A single shared legal/accounting enclave (`col-adversaryd`) handles external invoicing, sales/payroll tax remittances, wholesale `GPO` purchasing, and master cooperative liability insurance across all node guilds—quarantining all dollar signs and tax forms strictly out of everyday Layer 6 life.
5. **Hydronic Compute Heat Recovery ($\ge 8000\,\mathrm{bp}$ Capture):** Community servers running `Scenario Zero` CI builds, `Scenario Iota` simulations, and local open-weight AI models use warm-water cold-plate cooling that feeds $320{,}000\text{–}333{,}150\,\mathrm{mK}$ ($47\text{–}60^\circ\mathrm{C}$) water directly into `Scenario Zeta` showers and radiant floors.
6. **Polycentric Overlapping Nodes in One Physical Floorplate & Domain-Blind Enveloping Node Hypervisor (`D4`):** Resolves how $K$ autonomous nodes—an $N=1$ solo artisan importing an external Layer 5 guild covenant, a `Scenario Zero` software working group pod, an electronics rework bench, and the Third-Place café—coexist inside one shared floorplate without $O(K^2)$ bilateral treaty negotiations. The **Multi-Guild Floorplate Enveloping Hypervisor** governs exclusively the shared physical boundary medium via deterministic SI leases (`EnvelopingBoundaryLeaseV50`: centibels `cB`, milliwatts `mW`, millikelvins `mK`, millimeters `mm`) while remaining 100% domain-blind to each child node's internal Layer 5 **Four Constitutional Pillars** (*Pillar I: Surplus & Contribution*, *Pillar II: Work & Capability*, *Pillar III: Internal Governance*, *Pillar IV: Meta-Law*) and enforcing **Two-Speed Jurisprudence** (*Fast Path*: finite TTLs + Layer 2 hardware hazard preemption; *Slow Path*: recurring Layer 4 hard-blocks decompiled into Layer 6 Semantic Diffs and ratified into compiled Layer 4 precedents).

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
* **`MultiDomainWorkspaceAndCarapaceIntent`:** Booking a quiet focus desk, acoustic call booth, or salon meeting table, or requesting a GPU/CPU compute batch on the local cluster.
* **`GpoWholesaleAggregationIntent`:** Pooling leaf-component procurement requests from `Alpha`, `Gamma`, `Chi`, and `Xi` into a single volume-discounted wholesale order across `l6_l7.sock`.
* **`NeighborPetitionOrInquirySubmission`:** A zero-ID intake message submitted by a non-member neighbor at the street vestibule kiosk, routed through the `Scenario Rho` intake sanitizer to the community liaison circle.
* **`CarapaceFiduciarySettlementNotice` (Role-Gated):** Authorizing monthly municipal utility, property tax, or GPO invoice settlement up to the dynamic maintenance ceiling $\Phi_t$.

### B. Semantic Representations & Deterministic Read-Backs
* **Strict Quarantine of Fiat Accounting (`Carapace Enclave Isolation`):** General Constellation lenses (`Gamma`, `Alpha`, `Delta`, `Zeta`) contain **zero dollar fields**. External fiat balances, tax schedules, and GPO invoices exist solely inside the role-gated `carapace:fiduciary` subgraph and `col-adversaryd`, where they are governed by the dynamic bill-zeroing ceiling $\Phi_t$.
* **Mandatory Fiduciary & VLAN Read-Back:** Before a fiduciary steward co-signs an external tax or GPO disbursement across `l6_l7.sock`, the Fiduciary Ledger lens presents a **Deterministic Read-Back** verifying that internal member identities (`did:peer`) have been stripped by the **Mundane Semantic Camouflage Filter** and that commercial output remains below $\Phi_t$.

### C. Constellation of Application Lenses & Screenless Modalities

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **Zero-ID Open Municipal Neighbor Portal** | Street-Facing Café Vestibule (`neighbor:public`) | Walk-up E-Ink/matte kiosk requiring zero login: shows public feast/workshop calendars (`Lambda`), live perimeter decibel/air logs, and a 1-tap "Reserve Community Table / Ask a Question" button. |
| **Acoustic Desk Placard & VLAN Diode Monitor** | Coworking Study Desks (`workspace:steward`) | 0-mW bistable E-Ink desk card showing room quiet level ($\le 400\,\mathrm{cB}$), physical hardware switch position (`VLAN 40 External Work` vs. `VLAN 30 Sovereign Commons`), and server heat-recovery output. |
| **Shared Subchapter T Carapace Fiduciary Studio** | Fiduciary Audit Desk (`carapace:fiduciary`) | Role-gated lens for rotating administrative stewards: batches dozens of weekly GPO orders and tax withholdings into one calm biweekly review and multi-sig sign-off. |
| ***The Enveloping Node Hypervisor Lens*** | Multi-Guild Floorplate Boundary Console (`envelope:hypervisor`) | Domain-blind spatial lease monitor: displays active acoustic (`cB`), electrical (`mW`), thermal (`mK`), and spatial (`mm`) boundary leases across $K$ overlapping child nodes on the floorplate, plus Fast-Path hazard overrides and Slow-Path Semantic Diff precedent proposals. |
| **Physical Brass VLAN Selector & Call-Booth Latch** | Desk / Phone Booth Wall | Mechanical rotary switch that physically air-gaps the desk Ethernet jack between VLAN 40 and VLAN 30 with an unmistakable tactile click. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

Across every scale, Layer 5 governance in Scenario Epsilon is anchored in the single unified voting weight formula scoped strictly to **Node Membership ($N_s$)**:
$$V_i(t) = B + R_i(t) = 1 + R_i(t) \qquad \left(V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)\right)$$
where $B = 1$ ($10{,}000\,\mathrm{bp}$) is the non-decaying **Universal Civic Baseline** and $R_i(t)$ is exponentially decaying Layer 3 domain stewardship ($\tau_{1/2}$). Inside an **Enveloping Node Hypervisor**, each overlapping child node retains 100% sovereign control over its internal **Four Constitutional Pillars**, while its member stewards participate in the floorplate envelope's Slow-Path boundary jurisprudence under $V_i(t) = 1 + R_i(t)$ ($N_s > R_i(t) + 2$).

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person}$):**
   * *Context:* A solo homesteader, independent crafter, or remote worker ($V_1(t) = 1 + R_1(t)$) operating a home study nook or renting/holding a bench inside a shared multi-guild floorplate while importing an external Layer 5 guild constitution (`Scenario Pi`/`Alpha`).
   * *Governance Model:* **Polycentric overlapping autonomy & automated work/life boundary.** Even when physically situated inside a multi-guild coworking hall, the $N=1$ steward's internal craft rules and creative schedule remain 100% self-governed—interacting with the floorplate's **Enveloping Node Hypervisor** solely through a domain-blind boundary lease (`cB`, `mW`, `mK`, `mm`). The physical VLAN switch separates daytime Layer 7 remote work (VLAN 40) from sovereign life (VLAN 30), while `col-commonsd` tracks the monthly household fiat bill target ($\Phi_t$).
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ neighborhood pod}$):**
   * *Context:* A shared neighborhood study porch ($N=2$ dyadic mutual veto when $N_s=2$, or small core $N_s$), garage printshop, fiber internet drop, and pooled `GPO` buying club serving $5\text{–}15$ households ($N_p > 1$).
   * *Governance Model:*
     * **Pooling Buying Power Without Bookkeeping Headaches:** All $N_p$ households drop their monthly bulk staple/hardware needs into the shared `GpoWholesaleAggregationIntent` queue; one rotating administrative steward signs the consolidated wholesale order once a month.
     * **Neighbor Quiet & Guest Wi-Fi Etiquette:** Shared study tables default to $\le 400\,\mathrm{cB}$ quiet hours during weekdays enforced via finite-TTL `EnvelopingBoundaryLeaseV50` allocations, while visiting neighbors use the isolated VLAN 40 guest Wi-Fi with zero risk to internal pod services.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A commercial-scale cooperative coworking hub, café, compute cluster, and `Carapace-as-a-Service` guild ($N_s = 15\text{–}80+$ members across $K$ overlapping child nodes) serving an entire settlement and sister nodes ($R_2 \dots R_4$).
   * *Governance Model:*
     * **Anti-Capture Baseline ($N_s > R_i(t) + 2$) & Two-Speed Floorplate Jurisprudence:** No high-$R_i$ compute administrator or fiduciary steward can override the civic baseline of the $N_s$ coworking members. When two overlapping nodes experience boundary friction (e.g., an electronics bench's solder fume extractor `cB` vs. an adjacent study pod's quiet lease), **Fast-Path Jurisprudence** enforces finite lease TTLs and Layer 2 hazard preemption (`priority: 999`), while **Slow-Path Jurisprudence** decompiles recurring Layer 4 hard-blocks into a visual **Layer 6 Semantic Diff** ratified by the $N_s$ floorplate members into a compiled Layer 4 `col-execd` precedent.
     * **Mandatory Sortition & Hard $\Phi_t$ Ceiling Against Commercial Takeover:** `carapace:fiduciary` signers rotate via staggered terms, and once monthly external revenue from VLAN 40 desks or guild exports covers $\Phi_t$, external commercial intake throttles down automatically.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Consolidated `GPO` Wholesale Procurement:** Using corporate Group Purchasing Organization volume tiers to buy raw materials, medical supplies, and electronics at $40\text{–}70\%$ below retail.<br>**B. Multi-Guild Commercial Invoicing & Desk Memberships (`policy:commons_plus_l7_zero_bill`):** Invoicing external Layer 7 clients for guild craft/software work and renting VLAN 40 coworking desks to non-member remote workers to fund $\Phi_t$ and the Property Tax Escrow Endowment (`Scenario Omega`). | • **Centralized Dependency Accounting ($D_t$):** Because all external purchases route through the shared `GPO` carapace, Layer 3 has an exact, real-time audit of the settlement's 5-domain Dependency Ratio $D_t$.<br>• **Subchapter T Patronage & $\Phi_t = 0$ Auto-Cap:** Surplus revenues are legally structured as non-taxable cooperative patronage/capital reserves dedicated to land/endowment lock and shut off once $\Phi_t$ is satisfied. |
| **2. Purely Defensive Reach into Layer 7** | **A. Tripartite Legal & Insurance Shield (`Carapace-as-a-Service`):** Providing an impenetrable statutory buffer so individual makers, cooks, and paramedics cannot be personally sued or audited.<br>**B. Individual Member Anonymity (`Mundane Semantic Camouflage`):** Hiding which internal steward requested or produced an item. | • **UUNAA Section 6 + IRC Subchapter T + CLT Chassis:** Wraps internal commons activity in unincorporated association law, external commerce in a Subchapter T cooperative, and land in a `CLT`/`PPT`.<br>• **Complete Identity Stripping at `l6_l7.sock`:** External banks, vendors, and tax agencies interact solely with the cooperative corporate entity's EIN; zero internal `did:peer` keys or member lists ever enter Layer 7. |

---

## 7. Horizon Council v3.2 Architectural Binding (*The Architecture of Boundless Time* — D1–D8)

| Horizon Council Dimension | Scenario Epsilon (`SCN-EPSILON-COWORKING` / `epsilon_coworking`) Binding |
| :--- | :--- |
| **Node Membership Regime ($N_s$) & Unified Voice $V_i(t) = 1 + R_i(t)$** | $N_s = 15 \to 80$ multi-guild coworking members ($K$ overlapping child nodes in one floorplate); $B=1$ civic floor prevents fiduciary/compute capture ($N_s > R_i(t) + 2$). |
| **Primary Layer 6 Lens & Need-Capture Mode** | ***The Enveloping Node Hypervisor Lens*** + ***The Workshop Lens*** (domain-blind spatial lease monitor & zero-ID municipal neighbor portal). |
| **Enveloping Node Hypervisor Lease (`cB`, `mW`, `mK`, `mm`)** | **Multi-Guild Floorplate Enveloping Hypervisor** (eliminating $O(K^2)$ horizontal boundary politics via `EnvelopingBoundaryLeaseV50` across `cB`, `mW`, `mK`, `mm`). |
| **L4 3-Tier Cascade & Stage I$\to$III Hardware Retrofit Archetype** | Canonical demonstration of $K$ overlapping nodes ($N=1$ crafter importing external L5 guild covenants, software pod, electronics bench) sharing one space via domain-blind `cB`/`mW`/`mK`/`mm` leases and Two-Speed Jurisprudence. |
