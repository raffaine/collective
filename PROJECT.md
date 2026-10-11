# Project: Sovereign Stack 24-Scenario Overhaul & Canonical Compliance

## Architecture
- **Layer 1: Physical Reality:** Pure physical matter and energy (6.4 kW PV, 14.34 kWh LiFePO4, 5.0 kW inverter, 48V DC bus, 4-stage thermal cascade). Contains zero software, zero firmware, zero keys, zero network routing.
- **Layer 2: The Cybernetic Membrane:** Zephyr RTOS zero-trust microcontrollers (3-tier taxonomy), Living Mirror state observer, lock-free `/dev/shm` UHAI ring buffer bridge. Canonical fixed-point SI integer math wire encoding (microjoules, milliwatts, millivolts, milliamperes, milliliters/s, hectopascals, millikelvin, milligrams, milliseconds). Convivial anti-panopticon hardware shield (absolute ban on body-worn sensors and workplace cameras).
- **Layer 3: The Thermodynamic Ledger:** Peer-to-peer Pairwise Bilateral Clearing Channels (BCC) with zero-sum conservation $B_{u \to v}(t) + B_{v \to u}(t) = 0$; mutual aid credit windows $[-C_{u \to v}, +C_{v \to u}]$; sensor-verified two-phase cryptographic delivery receipts $\sigma_{\mathrm{delivery}}$; multi-hop chained Hashed Timelock Contracts (HTLC) $\langle H(S), \Delta t, Q, u, v \rangle$; non-monetary settlement (reciprocal physical delivery, stewardship labor, Commons Jubilee); Proof-of-Thermodynamic-Work (PoTW; $1.0\text{ credit} \equiv 3.6\text{ MJ}$ net exergy) with physical demurrage $\mathcal{E}_0 e^{-\lambda t}$; absolute non-commensurability and non-tokenization of domestic care and human reproductive labor; zero global tokens, zero fiat conversions, zero AMMs.
- **Layer 4: The Deterministic Orchestrator:** Sandboxed BPMN 2.0 VM executing deterministic state workflows; FIFO / priority queues protecting ecological floors ($F_{\mathrm{eco}}$) and human survival baselines ($B_{\mathrm{baseline}}$); anonymous claim handles; zero task Dutch auctions or chore gig-markets. Boundary socket: `/run/collective/ipc/l4_l5.sock`.
- **Layer 5: Polycentric Governance:** Autopoietic cellular autonomy ("the parts" vs "the whole"); Protocol of Refusal protecting sacred unquantified domains; autonomous transit veto (`No-Transit`); 7-tier concentric trust rings ($R_0$ Hearth, $R_1$ Settlement, $R_2$ Commons, $R_3$ Watershed, $R_4$ Bioregional, $R_5$ Continental, $R_6$ Ecumene); Bioregional Path-Vector Protocol (BPVP / FRRP) with Capability Advertisements $\mathcal{A}$, export surplus bounds $Q_{\mathrm{avail}}$, loop checks, thermodynamic path attenuation $\mathcal{C}(\mathcal{P})$, and dynamic outward request cascading; bicameral polity; decaying and capped Reputational Wealth ($\mathcal{R}$) with Care Leave Shields.
- **Layer 6: Semantic Translation & Agora:** Tripartite Ricardian Treaties $\mathrm{Treaty\_CID} = \mathrm{SHA256}(\mathrm{Text} \parallel \mathrm{JSON-LD} \parallel \mathrm{BPMN})$; Agora intent matching; machine-readable JSON-LD schemas.
- **Layer 7: The Legacy Interface:** Dual-Membrane Doctrine; Tripartite Legal Chassis (Bilateral Mutual-Aid Agreements under UUNAA § 6, Subchapter T Shared Services Agreements under IRC §§ 1381–1388, Federated Land Trust easements); Cellular Blinding ("unremarkable local cooperatives"); subpoena immunity via data non-existence; bounded residual extraction ($R_t$) against itemized needs lists up to fiat ceiling $\Phi_t$; language discipline permanently retiring adversarial metaphors ("Trojan Ingestion", "Ecological Leeching", "Trojan Horse", "parasitism") in favor of lawful engineering interties and neighbourly reciprocity.

## Feature Inventory
| # | Feature | Description | Milestone | Source |
|---|---------|-------------|-----------|--------|
| F01 | Bilateral Clearing Channels & HTLCs | Pairwise channels, zero-sum conservation, mutual aid credit windows, sensor delivery receipts, multi-hop HTLCs, no global tokens | M1–M6 | Ch 4 §4.2, Ch 6 §6.7, Audit |
| F02 | 7-Tier Trust Rings & BPVP | Concentric rings ($R_0 \dots R_6$), capability advertisements, export surplus bounds, path vector loop checks, dynamic request cascading | M1–M6 | Ch 6 §6.5, Audit |
| F03 | Layer 5 Polycentric Governance | Cellular autonomy, Protocol of Refusal, transit vetoes, Treaty_CID, l4_l5.sock, bicameral polity, capped reputational wealth | M1–M6 | Ch 5 §5.1, Ch 6 §6.1, Ch 7 §7.3, Audit |
| F04 | Language Discipline | Purge "Trojan", "Leech", "Parasite"; adopt Dual-Membrane Doctrine, regenerative harvesting, honest dealing | M1–M6 | Ch 8 §8.9.4, Audit |
| F05 | Fixed-Point SI Integer Math | Zero IEEE 754 floats in C++, JSON schemas, and state claims; scaled integer wire encodings ($\mu\mathrm{J}, \mathrm{mW}, \mathrm{mV}, \mathrm{mA}, \mathrm{mL/s}, \mathrm{hPa}, \mathrm{mK}, \mathrm{mg}, \mathrm{ms}$) | M1–M6 | Ch 3 §3.4, Ch 4 §4.3, Audit |
| F06 | Non-Tokenization of Care & Anti-Panopticon | Exclude care/domestic labor from exergy minting; unconditional survival baselines; zero body-worn sensors or workplace cameras | M2, M5 | Ch 3 §3.7, Ch 4 §4.4, Ch 5 §5.6, Audit |
| F07 | 5-Perspective Council Consensus | Game Design, Game Engineering, Sovereign Stack, Solarpunk Philosophy, Scenario Specialist with explicit ratification in all 24 files | M1–M6 | ORIGINAL_REQUEST R3 |
| F08 | Scenario Alpha: Fabrication Commons | Rewrite `alpha_fabrication.md` with 5-perspective synthesis, PoTW exergy, strict adjacency, integer fixed-point, dual-membrane export | M1 | docs/scenarios/alpha_fabrication.md |
| F09 | Scenario Theta: Micro-Foundry | Rewrite `theta_foundry.md` with pyrometry integer telemetry, BCC metals clearing, zero L1 software | M1 | docs/scenarios/theta_foundry.md |
| F10 | Scenario Mu: Biochar Pyrolysis | Rewrite `mu_extraction.md` with biomass exergy accounting, agroforestry BCC, no parasitic rhetoric | M1 | docs/scenarios/mu_extraction.md |
| F11 | Scenario Eta: Coppice Forestry | Rewrite `eta_coppice.md` with 20-year rotation model, watershed trust rings, non-monetary treaties | M1 | docs/scenarios/eta_coppice.md |
| F12 | Scenario Gamma: Community Kitchen | Rewrite `gamma_kitchen.md` with calories in kJ/mg, zero fiat meal tokens, unconditional hearth $R_0$ food floor | M2 | docs/scenarios/gamma_kitchen.md |
| F13 | Scenario Delta: Childcare Commons | Rewrite `delta_childcare.md` with zero childcare tokenization, rotational work bees, convivial anti-surveillance | M2 | docs/scenarios/delta_childcare.md |
| F14 | Scenario Chi: Paramedic & Triage | Rewrite `chi_paramedic.md` with non-quantified triage, clinical care as unalienable right, zero biometric tracking | M2 | docs/scenarios/chi_paramedic.md |
| F15 | Scenario Phi: Elder Care | Rewrite `phi_elder.md` with unalienable elder baseline, Care Leave Shield, non-financial reputational recognition | M2 | docs/scenarios/phi_elder.md |
| F16 | Scenario Beta: Bioregional Transit | Rewrite `beta_transit.md` with multi-hop HTLC routing, transit vetoes, integer exergy transport costs | M3 | docs/scenarios/beta_transit.md |
| F17 | Scenario Epsilon: Open Coworking | Rewrite `epsilon_coworking.md` with anonymous task handles, zero workplace micro-surveillance, Subchapter T SSA | M3 | docs/scenarios/epsilon_coworking.md |
| F18 | Scenario Xi: Living Architecture | Rewrite `xi_architecture.md` with biophysical building materials, land trust easements, zero L1 daemons | M3 | docs/scenarios/xi_architecture.md |
| F19 | Scenario Zeta: Guest Lodging | Rewrite `zeta_lodging.md` with convivial sanctuary $R_0$, guest pass bounded fiat extraction, no hotel gig-market | M3 | docs/scenarios/zeta_lodging.md |
| F20 | Scenario Iota: Epistemic R&D | Rewrite `iota_epistemic.md` with open knowledge mesh, decentralized peer review, no predatory patenting | M4 | docs/scenarios/iota_epistemic.md |
| F21 | Scenario Kappa: Apprenticeship | Rewrite `kappa_apprenticeship.md` with guild mentorship, non-commercial education, decaying reputational wealth | M4 | docs/scenarios/kappa_apprenticeship.md |
| F22 | Scenario Lambda: Cultural Commons | Rewrite `lambda_cultural.md` with cultural celebrations in $R_1/R_2$, non-tokenized art, Protocol of Refusal | M4 | docs/scenarios/lambda_cultural.md |
| F23 | Scenario Nu: Circular Textiles | Rewrite `nu_fashion.md` with natural fiber processing, repair cafes, BCC materials clearance | M4 | docs/scenarios/nu_fashion.md |
| F24 | Scenario Tau: Node Genesis | Rewrite `tau_genesis.md` with Node bring-up timeline, hardware BOM alignment, Charter ratification | M5 | docs/scenarios/tau_genesis.md |
| F25 | Scenario Omicron: Kinship & Hearth | Rewrite `omicron_kinship.md` with domestic sanctuary $R_0$, relational intimacy, zero biometric sensors | M5 | docs/scenarios/omicron_kinship.md |
| F26 | Scenario Pi: Bioregional Guild | Rewrite `pi_guild.md` with cross-watershed guild federation, non-monetary treaties, mutual aid dispatch | M5 | docs/scenarios/pi_guild.md |
| F27 | Scenario Psi: Mediation & Justice | Rewrite `psi_mediation.md` with restorative circles, voluntary participation, zero algorithmic punishment | M5 | docs/scenarios/psi_mediation.md |
| F28 | Scenario Rho: Adversarial Pressure | Rewrite `rho_adversarial.md` with legacy zoning/tax defense, subpoena immunity, mundane semantic camouflage | M6 | docs/scenarios/rho_adversarial.md |
| F29 | Scenario Upsilon: Bioregional Shock | Rewrite `upsilon_shock.md` with wildfire/flood disaster cascade, Commons Jubilee, emergency request routing | M6 | docs/scenarios/upsilon_shock.md |
| F30 | Scenario Sigma: Burnout Defense | Rewrite `sigma_burnout.md` with Care Leave Shield, rotational relief, ban on task Dutch auctions | M6 | docs/scenarios/sigma_burnout.md |
| F31 | Scenario Omega: Decoupling Horizon | Rewrite `omega_decoupling.md` with dependency ratio $D_t$, obsolescence horizon, striking corporate shell | M6 | docs/scenarios/omega_decoupling.md |
| F32 | E2E Programmatic Verification Suite | Python standard library AST/regex test modules checking zero floats, zero rhetoric, zero fiat/tokens, 5 perspectives | M0 | verification/scenarios/ |
| F33 | Agent-as-Judge Suite & Rubric | 6-criterion structured JSON evaluator validating BCC, HTLC, Trust Rings, BPVP, Governance, Consensus | M0 | verification/scenarios/ |
| F34 | 100% Verification Acceptance Gate | Execution of test suite across all 24 rewritten files with 0 failures, 24/24 Judge PASS | M7 | Final Gate |

## Milestones
| # | Name | Scope | Dependencies | Status |
|---|------|-------|-------------|--------|
| M0 | E2E Testing Track Infrastructure | Build test runner, regex/AST checkers, Judge harness, TEST_READY.md | none | IN_PROGRESS |
| M1 | Physical Fabrication & Materials Commons | Rewrite Alpha, Theta, Mu, Eta (4 scenarios) | M0 (parallel) | DONE |
| M2 | Bodily Care, Nourishment & Sanctuaries | Rewrite Gamma, Delta, Chi, Phi (4 scenarios) | M0 (parallel) | DONE |
| M3 | Spatial Habitat & Cooperative Mobility | Rewrite Beta, Epsilon, Xi, Zeta (4 scenarios) | M0 (parallel) | DONE |
| M4 | Epistemic, Cultural & Soft Infrastructure | Rewrite Iota, Kappa, Lambda, Nu (4 scenarios) | M0 (parallel) | DONE |
| M5 | Identity, Governance & Restorative Justice | Rewrite Tau, Omicron, Pi, Psi (4 scenarios) | M0 (parallel) | DONE |
| M6 | Threat Modeling, Shock & Bioregional Decoupling | Rewrite Rho, Upsilon, Sigma, Omega (4 scenarios) | M0 (parallel) | DONE |
| M7 | Final Acceptance Gate & Verification | 100% E2E test pass, 24/24 Judge PASS, forensic audit, completion handoff | M0–M6 | PLANNED |

## Interface Contracts
### Scenario Markdown Files ↔ Verification Suite
- Each of the 24 scenarios in `docs/scenarios/` must export:
  1. `## Architectural Council Ratification & 5-Perspective Synthesis` featuring Game Designer, Game Engineer, Sovereign Stack Specialist, Solarpunk Thinker, Scenario Specialist, and explicit unanimous consensus.
  2. Fixed-point SI integer math in all C++ blocks (`uint64_t`, `int64_t`, `int32_t`, `uint32_t`), JSON schemas, and physical prose. Zero IEEE 754 floats (`float`, `double`, `f32`, `f64`, `\d+\.\d+f`).
  3. Strict zero-tolerance ban on retired adversarial rhetoric ("Trojan Ingestion", "Ecological Leeching", "Trojan Horse", "Leeching", "Parasite").
  4. Strict zero-tolerance ban on fiat conversions and global tokens ("Value Token", "Value Tokens", "USD", `\$`).
  5. Explicit documented presence of Pairwise Bilateral Clearing Channels ($BCC$), multi-hop Hashed Timelock Contracts (HTLC), 7-tier concentric trust rings ($R_0 \dots R_6$), Bioregional Path-Vector Protocol ($BPVP$), and Layer 5 polycentric governance (cellular autonomy, protocol of refusal, transit vetos, non-monetary treaties $\mathrm{Treaty\_CID}$).

## Code Layout
- Target Scenarios: `docs/scenarios/*.md` (24 files)
- E2E Test Suite: `verification/scenarios/*.py`, `verification/scenarios/agent_as_judge_rubric.md`
- Master Test Runner: `run_scenario_verification.sh`
- Test Output Artifacts: `verification/scenario_verification_report.json`, `TEST_READY.md`
- Orchestrator Working Directory: `.agents/teamwork/orchestrator_24/`
