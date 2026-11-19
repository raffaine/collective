# Scenario-Driven Development (SDD) Dashboard & Cross-Scenario Maturity Matrix

This dashboard serves as the master navigation index for the **twenty-five Sovereign Scenarios (`Zero` through `Omega`)**, organized across **Six Horizontal Waves (Waves A–F)** and **Four Incremental Maturity Tiers**:

1. **Stage 1A — Household Seed ($N = 1$ or Single Household, $R_0 \dots R_1$, Single Suburban Lot + $\$20\text{k}$ Reserve $\Phi_0$):** Unconditional **Baseline Right to Exist ($B_{\mathrm{baseline}} = 10{,}000\,\mathrm{bp}$ — Pea Soup, Bean Stew, Warm Shower, Basic Heating, Sanitation & Emergency Care)** + initial Dual-Fidelity Real Blueprints operating under **UUNAA Section 6** private association envelopes ($\le 550\,\mathrm{cB}$ workshop day, $\le 380\,\mathrm{cB}$ night).
2. **Stage 1B — Neighborhood Mesh ($N_s \ge 1, N_p > 1$, $R_1 \dots R_2$, 5–12 Peered Households):** Pairwise Bilateral Clearing Channels (`BCC`), multi-node DAG BOM dependencies, consolidated cargo-trike milk-runs ($\eta_{\mathrm{route}} \ge 6000\,\mathrm{bp}$), rotational care pods, and `Learn -> Work -> Reputational Wealth (R)` stewardship progression.
3. **Stage 2 — Cooperative Shell ($N_s > 1, N_p > 1$, $R_2 \dots R_3$, Leased Mixed-Use / Subchapter T Carapace):** Co-located commercial commissary, tool library, coworking carapace (`Carapace-as-a-Service`), and regional HVDC/Reticulum mesh routing with self-sustaining $\Phi_t \to 0$ bill-zeroing buffers.
4. **Stage 3 — Sovereign Campus ($N_s > 1, N_p > 1$, $R_3 \dots R_5$, Community Land Trust / Perpetual Purpose Trust):** Permanently decommodified land, full stoichiometric HIL verification, reversible *Kigumi* mass-timber architecture, terminal decoupling ($D_t \le 200\text{–}500\,\mathrm{bp}$), and the outbound **Planetary Lighthouse & Genesis Seedbed Bridge**.

---

## Two-Tier Scenario Documentation Architecture

Each scenario lives in its own dedicated directory under `docs/scenarios/<slug>/` with four structured artifacts:

* **`README.md` (Forward-Facing Condensed View — English):** Free of historical baggage and low-level C++ scaffolding, organized into six human-first sections:
  1. *Intended Scope & Domain Vision*
  2. *The Current Landscape (Layer 7) & Its Structural Problems*
  3. *Wide Feature Horizon*
  4. *The Layer 6 Surface: Needs, Intents, Semantic Representations & Constellation of Application Lenses*
  5. *Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)*
  6. *Governing the Reach into Layer 7 (Extractive/Transitional vs. Purely Defensive)*
* **`bra/README.md` (Edição Brasil & Lusofonia — PT-BR):** Complete Brazilian & Lusophone Portuguese translation and local recompilation of the 6-section specification, aligned 100% with the *Rádio Lambda Brasil* Season 1 source packs (`Livro-Contábil Termodinâmico` and the Brazilian Layer 7 legal carapace: `Art. 5º XVIII CF/88`, `Ato Cooperativo Lei 5.764/71`, `TTC Lei 13.465/17`, `RPPN Lei 9.985/00`, `PNAE Lei 11.947/09`, `LDB`, `SUS / Farmácias Vivas`, `Lei de Arbitragem 9.307/96`, `LGPD`, and `Fundo Patrimonial Lei 13.800/19`).
* **`versions/v1_full_stack_spec.md` (Versioned 7-Layer Engineering Specification):** Preserves the complete layer-by-layer daemon architecture, Mermaid state machines, JSON-LD wire payloads, Oasis (`Layer 1b`) voxel/ECS specs, and compilable C++20 verification harnesses.
* **`CHANGELOG.md` (Architectural Decision & Evolution Log):** Records why architectural changes were made across versions to prevent repeating past design mistakes.

---

## Wave A: Meta, Onboarding & Homeostasis Core
*Foundation for autopoietic software self-hosting, four-pathway inclusive Steward Genesis, and burnout-proof somatic homeostasis.*

| ID | Scenario Overview (`README.md`) | Deep Spec, PT-BR & Log | Primary Surface & Constellation Lenses | Stage 1A ($N=1$) | Stage 1B ($N_s \ge 1, N_p > 1$) | Stage 2–3 ($N_s > 1, N_p > 1$) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Zero** | [The Sovereign Software, Game & Digital Commons](./scenarios/zero_software/README.md) | [🇧🇷 PT-BR](./scenarios/zero_software/bra/README.md) · [v1 Spec](./scenarios/zero_software/versions/v1_full_stack_spec.md) · [Log](./scenarios/zero_software/CHANGELOG.md) | Forge Studio, Capability Nutrition Labels, Detox Quarantine Diode | 🟢 Verified (`col-storaged` + Canary) | 🟡 In Progress (Reticulum DTN Sync) | 🔴 Planned (Dual-Guild FROST Store) |
| **Tau** | [Steward Genesis, Multi-Pathway Onboarding & Inclusion](./scenarios/tau_genesis/README.md) | [🇧🇷 PT-BR](./scenarios/tau_genesis/bra/README.md) · [v1 Spec](./scenarios/tau_genesis/versions/v1_full_stack_spec.md) · [Log](./scenarios/tau_genesis/CHANGELOG.md) | Sensory-Calm Genesis Slate, Zero-App Walk-In (`R0_GUEST`), Shamir Helper | 🟢 Verified (Suburban $\$20\text{k}$ + $B_{\mathrm{baseline}}$) | 🟢 Verified (2-of-3 NFC Vouch) | 🔴 Planned (4-Pathway Campus) |
| **Sigma** | [Burnout Prevention, Somatic Homeostasis & Rotational Care](./scenarios/sigma_burnout/README.md) | [🇧🇷 PT-BR](./scenarios/sigma_burnout/bra/README.md) · [v1 Spec](./scenarios/sigma_burnout/versions/v1_full_stack_spec.md) · [Log](./scenarios/sigma_burnout/CHANGELOG.md) | `RestShield` Widget, Physical Brass Door Latch, Work-Bee Quest Board | 🟢 Verified (Chill Loop & $\le 300\,\mathrm{cB}$) | 🟡 In Progress (Pull-Only Relief) | 🔴 Planned ($\le 4000\,\mathrm{bp}$ Concentration Breaker) |

---

## Wave B: Suburban Survival, Energy, Fabrication & Transit Seed
*Replacing grocery, hardware, and fossil-fuel delivery bills via unconditional nourishment ($B_{\mathrm{baseline}}$), shared fabrication/tool cribs, and consolidated five-artery milk-runs.*

| ID | Scenario Overview (`README.md`) | Deep Spec, PT-BR & Log | Primary Surface & Constellation Lenses | Stage 1A ($N=1$) | Stage 1B ($N_s \ge 1, N_p > 1$) | Stage 2–3 ($N_s > 1, N_p > 1$) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Gamma** | [The Communal Hearth, Keyline Agroecology & Nourishment](./scenarios/gamma_kitchen/README.md) | [🇧🇷 PT-BR](./scenarios/gamma_kitchen/bra/README.md) · [v1 Spec](./scenarios/gamma_kitchen/versions/v1_full_stack_spec.md) · [Log](./scenarios/gamma_kitchen/CHANGELOG.md) | IP67 Hearth E-Ink, Cultivar Fieldbook, `BBS+` Allergen Tiffin Locker | 🟢 Ready (Hearth Baseline + Garden) | 🟡 In Progress (Porch Tiffin Train) | 🔴 Planned (Subchapter T Commissary) |
| **Alpha** | [Convivial Fabrication, Parametric Blueprints & Tool Library](./scenarios/alpha_fabrication/README.md) | [🇧🇷 PT-BR](./scenarios/alpha_fabrication/bra/README.md) · [v1 Spec](./scenarios/alpha_fabrication/versions/v1_full_stack_spec.md) · [Log](./scenarios/alpha_fabrication/CHANGELOG.md) | Blueprint Cost Browser ($\mathbf{C}_{\mathrm{prod}}$), Shop BOM E-Ink + $<10\,\mathrm{ms}$ LOTO | 🟢 Ready (400W Solar + Garage Bench) | 🟡 In Progress (Recursive DAG BOMs) | 🔴 Planned (5-Modality Fab Guild) |
| **Beta** | [Five-Artery Logistics, Cold-Chain Milk-Runs & `No-Transit`](./scenarios/beta_transit/README.md) | [🇧🇷 PT-BR](./scenarios/beta_transit/bra/README.md) · [v1 Spec](./scenarios/beta_transit/versions/v1_full_stack_spec.md) · [Log](./scenarios/beta_transit/CHANGELOG.md) | Handlebar Domain-B Courier Deck, Red-Lever Intertie, `No-Transit` Map | 🟢 Ready (48V Cargo Trike + Cistern) | 🟡 In Progress (CVRP $\eta \ge 6000\,\mathrm{bp}$) | 🔴 Planned (380V HVDC + Barge/Rail) |

---

## Wave C: Care, Kinship, Apprenticeship & Decommodified Shelter
*Non-tokenizable human care (`domestic_care_token_minting == 0`), 90-day postpartum cocoons, pluralistic kinship with $<500\,\mathrm{ms}$ unilateral exit, $R_0$-private mastery, and screenless $R_0$ homes.*

| ID | Scenario Overview (`README.md`) | Deep Spec, PT-BR & Log | Primary Surface & Constellation Lenses | Stage 1A ($N=1$) | Stage 1B ($N_s \ge 1, N_p > 1$) | Stage 2–3 ($N_s > 1, N_p > 1$) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Delta** | [Perinatal Sanctuary, Postpartum Cocoons & Childcare Pods](./scenarios/delta_childcare/README.md) | [🇧🇷 PT-BR](./scenarios/delta_childcare/bra/README.md) · [v1 Spec](./scenarios/delta_childcare/versions/v1_full_stack_spec.md) · [Log](./scenarios/delta_childcare/CHANGELOG.md) | Camera-Free Pod E-Ink, 2-Adult NFC Slate, 1-Sign 90d Cocoon Planner | 🟢 Ready (Living-Room Pod + Cocoon) | 🟡 In Progress (Diaper Loop + Ratios) | 🔴 Planned (Birthing & Forest School) |
| **Omicron**| [Chosen Kinship Covenants & Unilateral Safe-Harbor Exit](./scenarios/omicron_kinship/README.md) | [🇧🇷 PT-BR](./scenarios/omicron_kinship/bra/README.md) · [v1 Spec](./scenarios/omicron_kinship/versions/v1_full_stack_spec.md) · [Log](./scenarios/omicron_kinship/CHANGELOG.md) | Kinship Ceremony Slate, $R_1$ Sanctuary Switch, $<500\,\mathrm{ms}$ Safe Exit | 🟡 Next Sprint ($R_0\text{–}R_2$ Covenants) | 🔴 Planned (POA/HIPAA Armor) | 🔴 Planned (Standing Refuge Suites) |
| **Kappa** | [Debt-Free Polymathic Guild Apprenticeship & Mastery](./scenarios/kappa_apprenticeship/README.md) | [🇧🇷 PT-BR](./scenarios/kappa_apprenticeship/bra/README.md) · [v1 Spec](./scenarios/kappa_apprenticeship/versions/v1_full_stack_spec.md) · [Log](./scenarios/kappa_apprenticeship/CHANGELOG.md) | $R_0$-Private BKT Workbook, Hazard Drill Lens, $\le 2\,\mathrm{m}$ BLE Tether | 🟡 Next Sprint (Supervised Mode + ZKP) | 🔴 Planned (3-of-3 Capstone) | 🔴 Planned (Trade School Carapace) |
| **Zeta** | [Decommodified CLT Shelter & Regenerative Bathhouses](./scenarios/zeta_lodging/README.md) | [🇧🇷 PT-BR](./scenarios/zeta_lodging/bra/README.md) · [v1 Spec](./scenarios/zeta_lodging/versions/v1_full_stack_spec.md) · [Log](./scenarios/zeta_lodging/CHANGELOG.md) | Vestibule Bathhouse Kiosk, Screenless $R_0$ Brass Controls, Crisis Key | 🟡 Next Sprint ($43.2\,\mathrm{MJ/d}$ + Shower) | 🔴 Planned (DWHR + 72h Wetland) | 🔴 Planned (99-Yr CLT Courtyard) |

---

## Wave D: Regenerative Land, Extraction, Materials & Architecture
*Closing the heavy bioregional material loop across rotational coppice forestry, five-stream biorefining, solar scrap metallurgy, zero-PFAS textiles, and two-track Kigumi open building.*

| ID | Scenario Overview (`README.md`) | Deep Spec, PT-BR & Log | Primary Surface & Constellation Lenses | Stage 1A ($N=1$) | Stage 1B ($N_s \ge 1, N_p > 1$) | Stage 2–3 ($N_s > 1, N_p > 1$) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Eta** | [Regenerative Agroforestry, Coppice & Botanical Apothecary](./scenarios/eta_coppice/README.md) | [🇧🇷 PT-BR](./scenarios/eta_coppice/bra/README.md) · [v1 Spec](./scenarios/eta_coppice/versions/v1_full_stack_spec.md) · [Log](./scenarios/eta_coppice/CHANGELOG.md) | Offline GIS Fieldbook (`Mother-Tree Veto`), Dual-Assay Apothecary Slate | 🔴 Planned (Backyard Micro-Nursery) | 🔴 Planned (Coppice Lot $\le 2500\,\mathrm{bp}$) | 🔴 Planned (Solar Kiln & Forest Tax) |
| **Mu** | [Five-Stream Urban Unbuilding, Biochar & Nutrient Recovery](./scenarios/mu_extraction/README.md) | [🇧🇷 PT-BR](./scenarios/mu_extraction/bra/README.md) · [v1 Spec](./scenarios/mu_extraction/versions/v1_full_stack_spec.md) · [Log](./scenarios/mu_extraction/CHANGELOG.md) | NIR/XRF Triage Scanner, 72h Compost/Struvite Slate, Taint Lockout | 🔴 Planned (72h Hot Compost) | 🔴 Planned (TLUD Biochar Retort) | 🔴 Planned ($\ge 9800\,\mathrm{bp}$ Unbuilding) |
| **Theta** | [Secondary Foundry, XRF Scrap Metallurgy & Solar Batching](./scenarios/theta_foundry/README.md) | [🇧🇷 PT-BR](./scenarios/theta_foundry/bra/README.md) · [v1 Spec](./scenarios/theta_foundry/versions/v1_full_stack_spec.md) · [Log](./scenarios/theta_foundry/CHANGELOG.md) | Foundry Crucible Console, $\le 250\,\mathrm{ms}$ 2-Person Dead-Man Pour Gate | 🔴 Planned (Tier-1 Cold Reuse & Sort) | 🔴 Planned (2-Person Micro-Melt) | 🔴 Planned (3-Tier Foundry & Glass) |
| **Nu** | [Bioregional Fiber Shed, Zero-PFAS Textiles & Footwear](./scenarios/nu_fashion/README.md) | [🇧🇷 PT-BR](./scenarios/nu_fashion/bra/README.md) · [v1 Spec](./scenarios/nu_fashion/versions/v1_full_stack_spec.md) · [Log](./scenarios/nu_fashion/CHANGELOG.md) | Ephemeral-RAM Atelier (`OP_PURGE_R0_LINEAGE`), Sashiko Mending Board | 🔴 Planned (Mending & Diaper Repair) | 🔴 Planned (Shared Sewing Loft) | 🔴 Planned (Bast/Wool Mill & Cobbler) |
| **Xi** | [Two-Track Parametric Open Building & Kigumi Barn-Raising](./scenarios/xi_architecture/README.md) | [🇧🇷 PT-BR](./scenarios/xi_architecture/bra/README.md) · [v1 Spec](./scenarios/xi_architecture/versions/v1_full_stack_spec.md) · [Log](./scenarios/xi_architecture/CHANGELOG.md) | Kigumi Barn-Raising Tablet, Track-2 Private Sketchpad, PE Code Compiler | 🔴 Planned (Blower-Door & Insulation) | 🔴 Planned (Fence-Removal Courtyards) | 🔴 Planned (Track-1/2 Kigumi Village) |

---

## Wave E: Third Place, Epistemic R&D, Culture & Polycentric Guilds
*Uniting zero-ID civic hospitality, $N \ge 3$ empirical citizen science, unmediated acoustic/STEAM culture, and stigmergic multi-guild federation.*

| ID | Scenario Overview (`README.md`) | Deep Spec, PT-BR & Log | Primary Surface & Constellation Lenses | Stage 1A ($N=1$) | Stage 1B ($N_s \ge 1, N_p > 1$) | Stage 2–3 ($N_s > 1, N_p > 1$) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Epsilon**| [Shared Third-Place Portal, Compute & Subchapter T Carapace](./scenarios/epsilon_coworking/README.md) | [🇧🇷 PT-BR](./scenarios/epsilon_coworking/bra/README.md) · [v1 Spec](./scenarios/epsilon_coworking/versions/v1_full_stack_spec.md) · [Log](./scenarios/epsilon_coworking/CHANGELOG.md) | Zero-ID Neighbor Portal, VLAN 40/30 Diode Placard, Fiduciary Studio | 🔴 Planned (Study Nook + VLAN Switch) | 🔴 Planned (Shared Fiber & GPO Pool) | 🔴 Planned (Third-Place Café & Co-op) |
| **Iota** | [Epistemic Open Science, Metrology & Alexandria Commons](./scenarios/iota_epistemic/README.md) | [🇧🇷 PT-BR](./scenarios/iota_epistemic/bra/README.md) · [v1 Spec](./scenarios/iota_epistemic/versions/v1_full_stack_spec.md) · [Log](./scenarios/iota_epistemic/CHANGELOG.md) | Lab Notebook ($\le 50\,\mathrm{bp}$ Drift), $N \ge 3$ Workbench, Alexandria Reader | 🔴 Planned (Home Soil/Water Assay) | 🔴 Planned (Inquiry Circle & Bindery) | 🔴 Planned ($N \ge 3$ Ontology Calibration) |
| **Lambda** | [Cultural, Acoustic & Land-Rooted STEAM Commons](./scenarios/lambda_cultural/README.md) | [🇧🇷 PT-BR](./scenarios/lambda_cultural/bra/README.md) · [v1 Spec](./scenarios/lambda_cultural/versions/v1_full_stack_spec.md) · [Log](./scenarios/lambda_cultural/CHANGELOG.md) | Unmediated Mic Kill-Switch, Broadside Calendar, SPL Stage Lantern | 🔴 Planned (Acoustic Hearth & Craft) | 🔴 Planned (Parlor Salon & Luthiery) | 🔴 Planned (Amphitheater & Festivals) |
| **Pi** | [Polycentric Guild Patchwork, Demand Beacons & Treaties](./scenarios/pi_guild/README.md) | [🇧🇷 PT-BR](./scenarios/pi_guild/bra/README.md) · [v1 Spec](./scenarios/pi_guild/versions/v1_full_stack_spec.md) · [Log](./scenarios/pi_guild/CHANGELOG.md) | 4-Stage Ricardian Treaty Studio, `MeshGrowthBeacon` Map, Swarm Board | 🔴 Planned (1-of-1 `BCC` Peering) | 🔴 Planned (FROST Pod + Growth Beacons) | 🔴 Planned (Spatial $\times$ Guild Federation) |

---

## Wave F: Health, Resilience, Elder Care, Justice & Terminal Decoupling
*Hardening the commonwealth across zero-LLM clinical triage, 72-hour black-start solidarity, dignified elder sagehood, transformative justice, fault-first immunity, and post-scarcity graduation.*

| ID | Scenario Overview (`README.md`) | Deep Spec, PT-BR & Log | Primary Surface & Constellation Lenses | Stage 1A ($N=1$) | Stage 1B ($N_s \ge 1, N_p > 1$) | Stage 2–3 ($N_s > 1, N_p > 1$) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Chi** | [Zero-LLM Clinical Triage, Primary Care & Pharmacopoeia](./scenarios/chi_paramedic/README.md) | [🇧🇷 PT-BR](./scenarios/chi_paramedic/bra/README.md) · [v1 Spec](./scenarios/chi_paramedic/versions/v1_full_stack_spec.md) · [Log](./scenarios/chi_paramedic/CHANGELOG.md) | Zero-LLM `<500ms` Trauma Pager, `HPLC`/`GC-MS` $\ge 9950\,\mathrm{bp}$ Assay Slate | 🔴 Planned (Trauma Kit & Cold Fridge) | 🔴 Planned (Block Medic Ring $<180\,\mathrm{s}$) | 🔴 Planned (Clinic & Open-Insulin Lab) |
| **Upsilon**| [Ecological Shock, Black-Start Microgrids & Open Refuge](./scenarios/upsilon_shock/README.md) | [🇧🇷 PT-BR](./scenarios/upsilon_shock/bra/README.md) · [v1 Spec](./scenarios/upsilon_shock/versions/v1_full_stack_spec.md) · [Log](./scenarios/upsilon_shock/CHANGELOG.md) | 0-mW Black-Start E-Ink, Open-Hearth Refuge Board, LoRa Pager | 🔴 Planned (72h Islanded Hold) | 🔴 Planned (Open-Hearth Block Refuge) | 🔴 Planned (Keyline Sponge & Jubilee) |
| **Phi** | [Dignified Aging, Emeritus Sagehood & Hospice Sanctuary](./scenarios/phi_elder/README.md) | [🇧🇷 PT-BR](./scenarios/phi_elder/bra/README.md) · [v1 Spec](./scenarios/phi_elder/versions/v1_full_stack_spec.md) · [Log](./scenarios/phi_elder/CHANGELOG.md) | AAA Tactile/Voice Companion, Edge mmWave Fall Sensor, Vigil Switch | 🔴 Planned (Accessible Ground Suite) | 🔴 Planned ($\le 2000\,\mathrm{bp}$ Care Ring) | 🔴 Planned (Hospice & Memorial Grove) |
| **Psi** | [Transformative Justice, Restorative Circles & Fission](./scenarios/psi_mediation/README.md) | [🇧🇷 PT-BR](./scenarios/psi_mediation/bra/README.md) · [v1 Spec](./scenarios/psi_mediation/versions/v1_full_stack_spec.md) · [Log](./scenarios/psi_mediation/CHANGELOG.md) | `<500ms` Safe-Harbor Intake, Unmediated Peace Hearth, Fission Workbench | 🔴 Planned (Cooling-Off & Telemetry) | 🔴 Planned (VRF Sortition Circle) | 🔴 Planned (Graceful Fission Protocol) |
| **Rho** | [Autopoietic Immunity, Fault-Before-Malice & Shielding](./scenarios/rho_adversarial/README.md) | [🇧🇷 PT-BR](./scenarios/rho_adversarial/bra/README.md) · [v1 Spec](./scenarios/rho_adversarial/versions/v1_full_stack_spec.md) · [Log](./scenarios/rho_adversarial/CHANGELOG.md) | Fault-First Calibration Lens, VLAN 40 Quarantine Viewer, Leak Auditor | 🔴 Planned (Split-Sandbox & Leak Check) | 🔴 Planned (Two-Person Meter Check) | 🔴 Planned (Subpoena Sterility Proof) |
| **Omega** | [Terminal Decoupling, Post-Scarcity & Planetary Lighthouse](./scenarios/omega_decoupling/README.md) | [🇧🇷 PT-BR](./scenarios/omega_decoupling/bra/README.md) · [v1 Spec](./scenarios/omega_decoupling/versions/v1_full_stack_spec.md) · [Log](./scenarios/omega_decoupling/CHANGELOG.md) | 5-Domain $D_t$ Observatory, Planetary Lighthouse & Seedbed Studio | 🔴 Planned (5-Domain $D_t$ Tracker) | 🔴 Planned ($D_t \le 5000\,\mathrm{bp}$ Halfway) | 🔴 Planned ($D_t \le 200\,\mathrm{bp}$ + Lighthouse) |
