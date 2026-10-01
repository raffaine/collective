# Bootstrap Strategy: The AI-Human Studio

Building a 7-layer thermodynamic operating system simultaneously with a C++20 voxel game engine requires a highly compressed, asymmetrical development model. Traditional software studios scale by adding human headcount, which introduces massive communication overhead and fiat burn rates. 

The Collective bootstraps itself using an **AI-Human Studio Model**. In this paradigm, Large Language Models (LLMs) act as highly specialized, parallelized execution agents, while the Human acts as the sole cryptographic authority, thermodynamic verifier, and architectural governor. 

---

## 1. The AI Agent Panorama (Execution & Orchestration)

To pulverize the workload of Scenario-Driven Development (SDD), the studio deploys specific AI personas strictly siloed by their layer responsibilities to prevent architectural bleed:

*   **The Architect (Layers 1, 2, & Engine Core):** Dedicated entirely to the C++20 engine, Vulkan compute shaders, and WebAssembly (WASM) translation. 
*   **The Orchestrator (Layers 3 & 4):** Writes the complex state machines, generating W3C-compliant BPMN 2.0 XML files, CRDT network sync logic, and defining the Thermodynamic Triangulation math.
*   **The Semantic Scribe (Layers 5 & 6):** Translates physical reality into human intent, drafting the JSON-LD schemas (Knowledge Artifacts, Verifiable Credentials) and UI/UX logic.
*   **The Legal Proxy (Layer 7):** Formulates the legacy interfaces (SPC charters, PPT deeds).

---

## 2. The Concrete Infrastructure Stack (Zero-Fiat Bootstrapping)

To maintain absolute financial sovereignty during Phase 1, the AI-Human studio operates on a "Zero-Fiat" burn rate. We leverage free-tier cloud infrastructure, specifically optimizing for Google’s ecosystem and open-source tools to build, compile, and distribute the Oasis WASM client.

### A. The AI Context Engine
*   **Google AI Studio (Gemini 1.5 Pro):** The core intelligence of the studio. Its massive context window (1M-2M tokens) is mandatory. We load the entire `arch-v2-genesis` repository documentation, all 24 Scenarios, and the evolving C++ codebase into a single context window. This ensures the Architect, Orchestrator, and Scribe agents perfectly remember the 7-Layer rules without hallucinating.

### B. The Development Workspace & CI/CD
*   **Google Project IDX / GitHub Codespaces:** Instead of configuring complex local C++ environments, we use cloud-based, ephemeral IDEs. They provide pre-configured Emscripten (C++ to WASM) toolchains. Project IDX natively integrates Gemini, allowing the Architect agent to write and compile code directly in the browser environment.
*   **GitHub Actions:** Automated CI/CD. Every time code is pushed to the `main` branch, a GitHub Action spins up an Ubuntu runner, compiles the C++20 code via CMake and Emscripten, and generates the `oasis.wasm` and `index.html` payload.

### C. Deployment & Networking (The Mesh Prototype)
*   **Firebase Hosting (Google - Free Tier):** Oasis is a local-first application, meaning the "backend" is just the user's browser. Firebase Hosting provides a free, global CDN with auto-provisioned SSL certificates to serve the static WASM bundle to players instantly.
*   **Google STUN Servers:** To simulate the Layer 3 Gossipsub mesh without central servers, players' browsers must connect directly via WebRTC. We utilize free, public Google STUN servers (`stun.l.google.com:19302`) to handle NAT traversal and IP discovery, enabling true peer-to-peer multiplayer at zero cost.

---

## 3. Immediate Execution Steps (Day 1 to Day 30)

To transition from architectural drafting to actual compiling, the AI-Human studio executes the following sequence:

### Step 1: Context Seeding (Day 1)
*   Initialize a persistent workspace in Google AI Studio. 
*   Upload all markdown files from the `docs/` folder (01 through 08).
*   **Prompt Directive:** *"You are the AI-Human Studio. You have ingested the 7-Layer architecture and the 24 Scenarios. Acknowledge your constraints: no centralized databases, no fiat logic in L3, C++20 DOD principles only."*

### Step 2: The Emscripten Hello World (Day 2-5)
*   Deploy a baseline repository via Google Project IDX.
*   Task the *Architect Agent* to write a simple C++ loop that allocates a $32 \times 32 \times 32$ array of the `Voxel` 32-bit struct.
*   Setup the `CMakeLists.txt` to compile this into WASM.
*   Validate that the memory is correctly passed from C++ to a JavaScript console log in the browser.

### Step 3: Headless Scenario Alpha (Day 6-15)
*   Bypass Vulkan rendering entirely. Build the "Headless Engine."
*   Task the *Orchestrator Agent* to write a basic C++ BPMN state machine parser.
*   Load Scenario Alpha (Fabrication Commons). Have the C++ engine take a mock JSON-LD intent, process it through the logic gates, and mathematically "deduct" raw materials from a voxel chunk's memory in the console.

### Step 4: The CI/CD Pipeline (Day 16-20)
*   Configure the `.github/workflows/deploy.yml`.
*   Connect the repository to Firebase Hosting.
*   Ensure that a git push automatically compiles the WASM blob and deploys it to a live `oasis-genesis.web.app` URL.

### Step 5: Visualizing the Chunk (Day 21-30)
*   Task the *Architect Agent* to write the WebGL2 / WebGPU translation layer for the WASM payload.
*   Render the first visual output: a simple 3D representation of the $32^3$ voxel chunk in the browser, visually updating when the headless BPMN state machine alters the voxel memory.

---

## 4. Human Supervision Gates (The Meatspace API)

AI agents hallucinate, drift from constraints, and do not possess physical bodies. They cannot know if a voxel moisture model accurately reflects the water retention of fungal loam. The Human provides the ultimate ground-truth friction. 

No AI code or schema becomes canon until it passes these strict supervision gates:

*   **Gate 1: The Architectural Freeze:** The Human sets the immutable laws. If an agent suggests a centralized Cloud SQL database to solve a state-sync issue, the Human rejects the payload and forces a CRDT peer-to-peer rewrite.
*   **Gate 2: Thermodynamic Verification:** The game engine must perfectly simulate the physical hardware it will eventually govern. The simulated logic must accurately predict the filament consumption of a physical Bambu Lab printer before it is merged.
*   **Gate 3: Cryptographic Sign-Off:** AI agents cannot hold trust. They cannot sign Verifiable Credentials. The Human is the sole cryptographic anchor for the Genesis Node, reviewing and physically signing JSON-LD payloads with their private key.
