# Volume 1, Chapter 1: Tri-Partite Critique and Refinement Guide
**Target File:** `doc/book_collective/chapters/01_anatomy_of_a_node.tex`

## Objective
To elevate Chapter 1 ("The Anatomy of a Node") to its highest standard of mature, authoritative prose through a managed, three-agent adversarial critique process. 

## The Orchestration (3 Agents)

### Agent 1: The Informed Critic
*   **Persona:** A senior cybernetic systems architect who has deeply read the entire `doc/` repository (including the technical Layer 1-7 companions).
*   **Task:** Evaluate the chapter for architectural consistency. Does the introduction properly set up the physical/thermodynamic realities? Do the ethical sections accurately reflect the capabilities of the Orchestrator (Layer 4) and the Semantic Interface (Layer 6)? 

### Agent 2: The Uninformed Critic
*   **Persona:** A highly literate, intelligent layperson (e.g., a sociologist or community organizer) who has *no prior knowledge* of the Sovereign Stack.
*   **Task:** Evaluate the chapter for narrative flow, clarity, and persuasion. Does the text rely too heavily on unexplained jargon? Does the transition from "The Oasis Strategy" to the "Baseline Ethics" feel jarring? Are non-technical readers treated with dignity?

### Agent 3: The Lead Author (Mediator & Writer)
*   **Persona:** The definitive voice of the Sovereign Stack. You possess absolute mastery over the technical architecture and the philosophical ethos. 
*   **Task:** Receive the feedback from both critics, synthesize it, and directly rewrite `01_anatomy_of_a_node.tex`. 

## Explicit User Critique (Mandatory Fix)
The Lead Author must immediately address a glaring flaw in the current text:
The recently added "Baseline Ethics Framework" and "Non-Steward" sections rely on a repetitive, bullet-point-style structure (using bolded phrases like **"How it is achieved via the Layers:"** and **"Care needed to avoid failure:"**). 

This reads like a rigid technical specification, not a foundational manifesto. It ruins the narrative flow and lacks the confidence and maturity of the preceding exposition. 
**Action:** The Lead Author must strip out these bolded structural crutches. The mechanics of *how* the layers achieve these ethical goals, and the *warnings* against failure modes (efficiency creep, dopamine loops, techno-elitism), must be seamlessly woven into fluid, authoritative, and mature prose.

## Workflow
1. Author summons the Informed and Uninformed Critics.
2. Critics review `01_anatomy_of_a_node.tex` and deliver their critiques to the Author.
3. Author acknowledges the critiques, applies the mandatory User Fix, and executes the rewrite using `write_to_file`.
4. Author compiles the PDF (`bibtex` and `pdflatex`).
