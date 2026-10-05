# Layer 4 Orchestrator: Chapter Generation Guide

## Core Mandate
This document governs the authoring of `doc/book_layer4/`. 
**Rule 1:** Preserve Chapter 1 (Stack Interaction) and Chapter 2 (Technology Primer) as the foundational bedrock.
**Rule 2:** No grouping. Every scenario (0 through 24) must have its own dedicated, expansive chapter. Chapter 3 will be Scenario 0 (The Meta-Scenario of building the Sovereign Stack and Oasis). Chapters 4 through 27 will cover Scenarios Alpha through Omega.
**Rule 3:** No superficial summaries. The companion must be *deeper* and more methodical than the raw markdown files in `docs/scenarios/`.

## Required Chapter Structure
Every chapter must systematically explore the scenario through the following four lenses:

### 1. The Legacy Paradigm & Its Failures
Define how the current centralized, fiat-based world handles this scenario. What are the centralized databases, corporate middlemen, or subjective human biases involved? Why does this legacy approach fail under strict thermodynamic and ecological limits?

### 2. The Incremental Automation Pathway
A Sovereign Node cannot jump to full AI autonomy on Day 1. Detail the three-stage migration path for orchestrating this scenario:
*   **Phase 1: Human-in-the-Loop (Manual Orchestration):** How citizens initially route this intent manually using raw L3 ledger interactions and human-approved BPMN gateways.
*   **Phase 2: The Centaur (AI-Assisted):** How localized LLMs begin to assist the human, parsing constraints and drafting intents, but strictly requiring manual cryptographic signing before execution.
*   **Phase 3: Full Autonomy:** The final state where the Orchestrator handles the DAG and Queueing logic autonomously based on physical node homeostasis.

### 3. The Human Boundary (Limits of Automation)
Define the absolute limits of the AI's jurisdiction. What are the strict ethical, thermodynamic, and cryptographic hard-stops where a human *must* step in? (e.g., The AI can route the paramedic, but it cannot algorithmically triage who lives or dies; the AI can monitor forest density, but it cannot physically wield the chainsaw).

### 4. Orchestration Mechanics (The "How")
A deep, technical dive into the exact execution. How are the DAGs topologically sorted? What specific Queueing Theory models (e.g., M/M/c) manage the intent backlogs? How do the discrete BPMN nodes interact asynchronously with L3 state changes without violating the Strict Boundary Theorem?
