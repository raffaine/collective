# Knowledge Representation Spec (Layer 1a: Pure Science)

## 1. Directive for Semantic Scribe Agents
**Objective:** Define the pure, objective physical and scientific reality of the Node. Layer 1a is the realm of thermodynamics, biology, materials science, and physics. Human desires (Intents) live in Layer 6; Social constructs (Trust/Law) live in Layer 5. Layer 1a is strictly what exists in the universe.
**Constraints:**
- All definitions must be valid JSON-LD 1.1.
- Represent strict mathematical and physical laws that the C++ Engine (Layer 1b) will simulate.

## 2. Core Domains of Scientific Knowledge

The knowledge base in Layer 1a is split into scientific domains:
1.  **Thermodynamics (`ontology_thermodynamics.jsonld`):** The absolute baseline. Exergy, Joules, Watts, Thermal Mass, Entropy.
2.  **Materials Science (`ontology_materials.jsonld`):** The physical building blocks. Biochar, Steel, PLA filament, Concrete. Defines structural integrity, density, and specific heat capacity.
3.  **Digital Physics (`ontology_digital_physics.jsonld`):** The physical reality of Node 0 itself. Because the codebase acts as the physical territory of the founding Node, we must quantify digital mass (Bytes), Compute Exergy (FLOPs/Tokens), and Entropy Deltas (Git Commits). This is essential for governing the thermodynamic cost of software development.

## 3. Binding to Layer 1b (The Simulation Engine)
Layer 1b reads these JSON-LD files to initialize its C++ structs. When Layer 1a defines the `SpecificHeatCapacity` of `mesh:Biochar`, Layer 1b maps that exact floating-point value into the 32-bit Voxel payload for environmental temperature simulation. Likewise, when Layer 1a defines `mesh:ComputeExergy`, the engine tracks the actual CPU cycles burned to compile itself.
4.  **Computational Models (`ontology_computational_models.jsonld`):** The mathematical equations governing exchange. How LLM Inference Tokens translate to Joules, and how cyclomatic complexity acts as friction against future thermodynamic work.
