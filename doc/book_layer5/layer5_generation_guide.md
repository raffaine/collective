# Layer 5 Trust & Policy: Architecture Generation Guide

## Core Mandate
This document governs the authoring of `doc/book_layer5/`. Applying the lessons from Layer 4, this companion must be a deeply technical, architectural engineering manual focusing on **how Layer 5 is constructed**. Layer 5 acts as the Sovereign Stack's legal and social layer—managing Decentralized Identifiers (DIDs), Web of Trust mechanics, Soulbound Tokens (SBTs), and Multi-signature Governance without relying on centralized state violence or fiat courts.

**Rule 1: Core Technology Chapters.** The book must be structured around core mechanisms rather than a catalog of scenarios.
**Rule 2: The Physical Reality of Trust.** Layer 5 is not an abstract social construct. Trust and cryptography require physical storage and physical computation. The core chapters MUST detail the hardware reality: Where are secrets held? (e.g., Secure Enclaves, physical smart cards, avoiding vulnerable memorized passwords). How are complex Zero-Knowledge attestations computed without draining edge batteries?
**Rule 3: The Scenario Appendix.** The 25 Sovereign Scenarios (Scenario 0 through Omega) will be moved to the Appendix. They serve as an exhaustive reference proving that the core technology chapters can handle every possible social and legal edge case.
**Rule 4: The Iterative Feedback Loop.** Scenario generation is a two-way street. If an appendix (e.g., Scenario Rho - Adversarial Mesh) uncovers a vulnerability or missing mechanism in the core architecture (like the vulnerability of passwords), the agent MUST halt, formulate a solution, and **refine the Core Chapters** to implement that solution before finishing the appendix. The core must evolve based on edge-case stress testing.

## Required Core Chapter Layout (To Be Authored)
*   **Chapter 1: The Sovereign Identity (DIDs & PKI Bypass)** (How individuals and machines are identified cryptographically. Crucially, how private keys are physically stored and computed without relying on fragile human memory or centralized Certificate Authorities).
*   **Chapter 2: The Web of Trust & Kinship Graphs** (How community trust is mapped mathematically via localized Trust Rings, and where this graph state is physically persisted on the mesh).
*   **Chapter 3: Cryptographic Attestations & Soulbound Tokens** (How skills and Proof of Physical Work are permanently bound to an identity, and the computational exergy required to verify them).
*   **Chapter 4: Decentralized Policy & Multi-sig Governance** (How $n$-of-$m$ thresholds enforce community rules physically via L2 actuators without a central judiciary).

## Authoring Guidelines for Each Appendix
When writing the 25 scenario appendices, you must strictly follow this internal structure:
1.  **The Legacy Paradigm & Its Failures:** How does the current world do this? (e.g., centralized KYC, credit scores).
2.  **The Incremental Automation Pathway:** Transitioning from manual human trust (Phase 1) to AI-assisted policy (Phase 2), to fully autonomous cryptographic enforcement (Phase 3).
3.  **The Human Boundary (Limits of Automation):** Exactly where the AI must stop.
4.  **Orchestration Mechanics (The "How"):** Deep dive into the cryptographic primitives and graph traversal algorithms. If writing this section reveals a gap in the core architecture, trigger **Rule 4 (The Iterative Feedback Loop)**.
