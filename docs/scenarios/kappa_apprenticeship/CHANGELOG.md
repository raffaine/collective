# Architectural Evolution & Decision Log — Scenario Kappa: Debt-Free Polymathic Guild Apprenticeship & Unlinkable Mastery (`SCN-KAPPA-APPRENTICE`)

This log records why architectural changes were made across versions of `SCN-KAPPA-APPRENTICE` so future stewards and engineers understand the rationale behind current invariants and avoid repeating past design mistakes.

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
  * Expanded `SCN-KAPPA-APPRENTICE` from an early narrow vignette into a complete macro-domain specification spanning Layers 1 through 7 and Layer 1b (Oasis 3D simulation).
  * Enforced strict 64-bit fixed-point SI integer arithmetic (`floats == []`) across all wire schemas and C++20 reference harnesses (`-std=c++20 -Wall -Wextra -Werror`).
  * Established 3-Stage Incremental Node Facility Blueprints (`Stage 1` household/suburban seed $\to$ `Stage 2` cooperative commercial shell $\to$ `Stage 3` CLT/PPT bioregional campus).
