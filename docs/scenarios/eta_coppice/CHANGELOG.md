# Architectural Evolution & Decision Log — Scenario Eta: Regenerative Agroforestry, Rotational Coppice & Botanical Apothecary (`SCN-ETA-COPPICE`)

This log records why architectural changes were made across versions of `SCN-ETA-COPPICE` so future stewards and engineers understand the rationale behind current invariants and avoid repeating past design mistakes.

---

## `v3.2` (2026-11-20) — Horizon Council Alignment (*The Architecture of Boundless Time*: D1–D8 & CCI-1..CCI-6)
* **What Changed:** Applied the Horizon Council v3.2 architectural recommendations ([docs/COUNCIL_GUIDANCE_SOVEREIGN_STACK_AND_BOOKS.md](../../COUNCIL_GUIDANCE_SOVEREIGN_STACK_AND_BOOKS.md)) to both the global English specification ([README.md](./README.md)) and the Brazilian & Lusophone Portuguese edition ([bra/README.md](./bra/README.md)).
* **Key Architectural Bindings Recorded in `v3.2`:**
  1. **Unified Layer 5 Civic Baseline & Reputational Voice (`D3`, `D8`):** Anchored Section 5 (*Governance Across Human Scales*) explicitly in the unified Layer 5 voting weight formula $V_i(t) = B + R_i(t) = 1 + R_i(t)$ ($V_{i,\mathrm{bp}}(t) = 10{,}000 + R_{i,\mathrm{bp}}(t)$) scoped strictly to **Node Membership ($N_s$)**, proving smooth scaling across $N=1$, $N=2$ dyadic veto, $N \gg 2$ anti-capture ($N_s > R_i(t) + 2$), and **Dormant Equilibrium ("The $\TeX$ Asymptote")** when active stewardship $\dot{r}_i(\tau) \to 0$.
  2. **Layer 6 Constellation of Lenses & Edge SI Localization (`D1`, `D2`, `D6`):** Bound the scenario to its canonical Layer 6 lenses (*The Pantry Lens*, *The Workshop Lens*, *The Bakery Lens*, *Scenario Zero Forge Studio Lens*, and/or *The Enveloping Node Hypervisor Lens*) with zero-surveillance anonymous pickup leases (`AnonymousAppetiteLeaseV50`, `CCI-2`) and edge SI localization.
  3. **Domain-Blind Enveloping Node Hypervisor & Two-Speed Jurisprudence (`D4`):** Specified the scenario's physical boundary lease envelope (`EnvelopingBoundaryLeaseV50`: `cB`, `mW`, `mK`, `mm`), respecting internal Layer 5 **Four Constitutional Pillars** while coordinating shared physical space via Fast-Path Layer 2 hazard preemption and Slow-Path Semantic Diff precedent compilation.
  4. **3-Tier Layer 4 BPMN Adaptation Cascade & 3-Stage Hardware Retrofit Progression (`D5`, `D7`):** Added Section 7 (*Horizon Council v3.2 Architectural Binding*) mapping the scenario's **Open-Source Franchising** across the Three Sovereign Libraries, its Layer 4 3-Tier Cascade (Tier 1 SI Scaling, Tier 2 Macro-Substitution, Tier 3 True-Constraint Hard-Block), and its hardware transition from **Stage I Scavenged Layer 7** to **Stage II Right-to-Repair Hybrid Retrofit Kits** and **Stage III Native Open-Spec Fleet** (**Zero-Stat Liberation**).

---

## `v2.0` (2026-11-15) — Forward-Facing Layer-6-Centered Reorganization & Scale-Adaptive Governance
* **What Changed:** Separated the forward-facing scenario presentation ([README.md](./README.md)) from the deep 7-layer engineering specification ([versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)).
* **Why This Change Was Made (Avoiding Past Mistakes):**
  1. **Eliminating Indigestible Record-Keeping Baggage:** Earlier monolithic scenario documents mixed high-level human goals with 5-Persona Council review logs, historical version debates, low-level socket plumbing, and 250-line C++20 test harnesses in a single file, making it difficult for newcomers and application authors to grasp the wide feature horizon of the domain.
  2. **Centering the Layer 6 Semantic Surface & Constellation of Lenses:** Grounded the primary scenario view in the human-facing Layer 6 surface (`col-commonsd` + `col-gatewayd`)—explicitly mapping human needs, canonical JSON-LD intents, semantic representations/deterministic read-backs, and the specialized Constellation of convivial Application Lenses and screenless modalities.
  3. **Explicit Human Governance Scaling ($N=1 \to N_s \ge 1, N_p > 1 \to N_s > 1, N_p > 1$):** Replaced abstract governance jargon with an explicit progression showing how a single solo operator ($N=1$) operates with zero ceremony, how a small steward core ($N_s \ge 1$) serves a wider circle ($N_p > 1$) without burnout or gatekeeping, and how multi-steward guilds ($N_s > 1, N_p > 1$) coordinate across a settlement and bioregion.
  4. **Clarifying Extractive vs. Defensive Layer 7 Reach:** Explicitly separated outbound Layer 7 interactions into (a) *Extractive/Transitional Reach* (sourcing irreducible precursors or capped surplus exports $\Phi_t \to 0$ while logging dependencies $D_t$ for import substitution) and (b) *Purely Defensive Reach* (statutory carapaces and the Mundane Semantic Camouflage Filter).

---

## `v1.0` (2026-10-22) — Full-Stack 7-Layer Macro-Domain & Oasis Specification
* **Snapshot:** Preserved in full at [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md).
* **Key Architectural Decisions Recorded in `v1.0`:**
  * Expanded `SCN-ETA-COPPICE` from an early narrow vignette into a complete macro-domain specification spanning Layers 1 through 7 and Layer 1b (Oasis 3D simulation).
  * Enforced strict 64-bit fixed-point SI integer arithmetic (`floats == []`) across all wire schemas and C++20 reference harnesses (`-std=c++20 -Wall -Wextra -Werror`).
  * Established 3-Stage Incremental Node Facility Blueprints (`Stage 1` household/suburban seed $\to$ `Stage 2` cooperative commercial shell $\to$ `Stage 3` CLT/PPT bioregional campus).
