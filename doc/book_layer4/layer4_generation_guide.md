# Layer 4 Orchestrator: Architecture Generation Guide

## Core Mandate
This document governs the authoring of `doc/book_layer4/`. The book is no longer a catalog of scenarios. It is a deeply technical, architectural deep-dive into **how Layer 4 is engineered, computed, and stored** to support those scenarios.

**Rule 1: Preserved Foundation.** Chapter 1 (Stack Interaction) and Chapter 2 (Technology Primer: BPMN, Actor Model, Queueing) remain the bedrock.
**Rule 2: Core Technology Chapters.** Subsequent chapters (Chapters 3 through 6) must build directly upon Chapter 2. They must detail the exact technologies required to assist Layer 4 in hosting orchestrations. This includes:
*   How state is stored and computed.
*   How Layer 4 shares infrastructure with Layer 3 (The Ledger) or runs on what L3 provides.
*   How WASM runtimes, localized LLM inference, and cryptographic routing (ZK-proofs) are physically managed at the edge.
*   These tools must demonstrably support all the complex requirements of the 25 scenarios.
**Rule 3: The Scenario Appendix.** The 25 previously authored scenario chapters (Scenario 0 through Omega) are moved to the Appendix. They serve as an exhaustive reference proving that the core technology chapters can handle every possible edge case.

## Required Core Chapter Layout (To Be Authored)
*   **Chapter 3: Distributed State & Shared Ledger Infrastructure** (Detailing how L4 BPMN state machines persist memory, utilizing L3 IPFS/IPLD storage or localized DAGs without central databases).
*   **Chapter 4: The Computation Matrix** (Detailing the execution environment: WASM runtimes for deterministic BPMN logic, and thermodynamic throttling for LLM inference).
*   **Chapter 5: Cryptographic Routing & Intent Resolution** (How intents are routed across the offline mesh, utilizing ZK-proofs for privacy-preserving routing).
*   **Chapter 6: Telemetry Ingestion & Sensor Oracles** (The technical bridge between L2 hardware interrupts and L4 cognitive queues).
