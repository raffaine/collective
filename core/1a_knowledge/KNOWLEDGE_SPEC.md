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
3.  **Biology & Ecology (`ontology_ecology.jsonld`):** The living layer. Fungal loam, moisture retention, biological metabolic rates.

## 3. Binding to Layer 1b (The Simulation Engine)
Layer 1b reads these JSON-LD files to initialize its C++ structs. When Layer 1a defines the `SpecificHeatCapacity` of `mesh:Biochar`, Layer 1b maps that exact floating-point value into the 32-bit Voxel payload for environmental temperature simulation.
