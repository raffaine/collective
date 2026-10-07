# Volume 1, Chapter 5: 5-Expert Multi-Disciplinary Critique Guide
**Target File:** `doc/book_collective/chapters/05_layer_4_orchestrator.tex`

## Objective
Stress-test Chapter 5 ("Layer 4: The Orchestrator") from five directions at once. Layer 4 is the active, execution-only engine of the Node: it reads consensus state from the Thermodynamic Ledger (Layer 3), executes BPMN workflows and sandboxed WASM logic, and emits actuation intent back down through Layer 3. Because it is the layer that turns static data into kinetic action, it is where a software mistake becomes a physical one.

## Shared Context (all experts)
- **Strict Adjacency:** Layer 4 may only interface with Layer 3 (below) and Layer 5 (above). It never reads Layer 2 sensors directly, never drives Layer 1 hardware directly, and never talks to Layer 6 or 7 directly.
- **Role purity:** Layer 4 executes. It does not set policy (Layer 5: Governance), and it does not interpret human language or intent (Layer 6: Semantic Interface).
- **Prior chapters established:** Layer 2 owns hardware fail-safes and power electronics; Layer 3 is a passive ledger with demurrage, meshed spatial reasoning, partition sharding, and *Qualitative Friction* (human vetoes, enforced rest, no optimization of human fatigue).
- **Tone:** This is a mature guiding manual, not a pamphlet. Critique substance, not just style.

## The Council

### 1. Cybernetics & Control Theory Engineer (The Dampener)
Critique how digital decisions become stable physical behaviour. Naive threshold logic (`if moisture < 50% then pump`) causes chattering and actuator wear. Look for hysteresis, deadbands, rate limiting, PID / feedback control, sensor latency and biological lag (e.g., compost heating over days). Flag any place where control is described as instantaneous or binary when physics is continuous and delayed.

### 2. Distributed Edge Compute Specialist (The Halting Problem)
Critique the execution substrate. WASM on salvaged, battery-powered hardware: fuel/gas metering, timeouts, memory caps, runaway loops draining batteries, deterministic execution across heterogeneous devices, scheduling under exergy scarcity, and what happens when the local Orchestrator instance is partitioned from its peers. Computation itself spends exergy and must be accounted for.

### 3. Workflow Architect (The Master of Biological Time)
Critique the BPMN model. Biology does not run on cron. Look for event-driven vs. time-driven triggers, degree-days and phenology, long-running processes (months/years), compensation and abort paths when a frost or blight invalidates a waiting workflow ("zombie processes"), versioning of workflows mid-flight, and human tasks inside workflows (which must respect Qualitative Friction and the right to refuse).

### 4. Ecological Failsafe Auditor (The Anti-Paperclip Maximizer)
Critique unintended optimization. A workflow told to "maximize yield" can drain an aquifer. Look for single-metric optimization, missing ecological floors (aquifer recharge, soil biota, habitat), interactions between concurrent workflows that compete for the same resource, and whether the Orchestrator can be made to act against the biome's long-term resilience. Ensure the bounds are structural, not advisory.

### 5. Sovereign Stack Architect (The Adjacency Enforcer)
Critique architecture. Flag every place where Layer 4 bypasses Layer 3, touches hardware, parses natural language, invents policy, or talks to layers it is not adjacent to. Also flag the opposite failure: responsibilities that *do* belong to Layer 4 (alerts, emergency workflows, actuation intent) but are missing or misassigned.

## Lead Author Mandate
- Synthesize all five critiques into one coherent chapter.
- **Preserve depth.** Do not shrink the chapter into a summary; keep valuable existing material and add what the council finds missing.
- Mature, flowing prose. No bolded pseudo-headings or numbered bold bullets used as structural crutches; use real `\subsection`s where structure is needed.
- Keep consistency with Chapters 2–4 and the Strict Adjacency rule.
- Compile with `bibtex` + `pdflatex` (twice) outside the sandbox, verify there are no LaTeX errors, commit only the relevant files, and write `vol1_ch5_final_status.md`.
