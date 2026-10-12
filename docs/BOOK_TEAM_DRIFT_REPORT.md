# Layer 1 / Oasis Drift Risk Report

**Target Audience:** `doc/book_collective` Writing & Editing Team
**Source:** Dual Sprint Architecture Council (Product Owner)
**Date:** Sprint 6 Planning Phase

## Executive Summary
During the execution of Dual Sprint 5, the engineering team successfully implemented the 10Hz POSIX execution loop, the Flecs ECS thermodynamic loops, and the UHAI shared-memory IPC. 

Upon reviewing the current LaTeX manuscript for Chapter 1 (`01_anatomy_of_a_node.tex`), the Product Owner identified significant narrative drift that directly contradicts the actual C++ codebase and strict safety protocols. The book team must consult the following corrections and draft revisions for Chapter 1.

## Identified Drifts & Required Corrections

### 1. Telemetry Ingestion vs. Production
- **Current Book Narrative:** Claims Oasis "consumes live data from their real-world environment" to recalibrate the simulator.
- **Engineering Reality:** Oasis is strictly a **producer** of simulated Layer 2 telemetry. It does NOT ingest live telemetry. Sprint 4 explicitly implemented a Hardware Airgap Filter (`quality_flag = 0x01`) to mathematically drop any simulated packets from firing physical hardware.
- **Required Edit:** Rewrite Chapter 1 to explicitly establish the *outward flow* of virtual telemetry from the C++ engine into the Layer 3 protocols for stress-testing.

### 2. Context Blurring
- **Current Book Narrative:** Frames Oasis as a "game that transitions into a real-world Orchestrator."
- **Engineering Reality:** This violates fundamental UHAI safety invariants. Real and simulated contexts must remain unmistakably distinct. The game is a shadow simulator, not a transitionary UI.
- **Required Edit:** Remove the "transitioning" framing. Emphasize simulation demarcation.

### 3. Adversarial Lore
- **Current Book Narrative:** Refers to Oasis as a "Trojan Horse" and uses terms like "parasitic" against legacy systems.
- **Engineering Reality:** The Layer 7 Adversary daemon (`col-adversaryd`) is built on Discrete Event Simulation (DES) to model legacy systems in good-faith. The Sovereign Stack is cooperative, not malicious.
- **Required Edit:** Strip "Trojan Horse" and "parasitic" phrasing. Replace with cooperative, structural terminology.
