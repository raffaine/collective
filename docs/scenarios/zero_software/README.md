# Scenario Zero: The Sovereign Software, Game & Digital Commons (`SCN-ZERO-SOFTWARE`)

* **Identifier:** `SCN-ZERO-SOFTWARE`
* **Horizontal Wave:** Wave A (Autopoietic Meta-Core, Onboarding & Homeostasis)
* **Versioned Full-Stack Specification:** [versions/v1_full_stack_spec.md](./versions/v1_full_stack_spec.md)
* **Architectural Decision Log:** [CHANGELOG.md](./CHANGELOG.md)

---

## 1. Intended Scope & Domain Vision

Scenario Zero is the **autopoietic digital meta-scenario** of The Collective. It defines how a settlement designs, compiles, verifies, distributes, and maintains its entire software stack—from real-time microcontroller firmware and operating system kernels to the seven-layer Sovereign Stack daemons (`col-*`), the open-ended **Constellation of Layer 6 Application Lenses**, collaborative creative suites, interactive 3D simulation sandboxes (**Oasis**), video games, community mods, and offline digital libraries.

Rather than treating software as a rented corporate service or an opaque administrative control panel, Scenario Zero treats computing as a **convivial, locally owned craft**. Every binary running on a node is reproducibly compiled from auditable source code, engineered to run briskly on twenty-year-old salvaged hardware, stripped of surveillance and addictive dark patterns, and thermodynamically coupled to the settlement so compiler and GPU waste heat warms domestic water and greenhouses.

---

## 2. The Current Landscape (Layer 7) & Its Structural Problems

In the legacy software and digital media economy (Layer 7), users and communities face four systemic traps:

1. **Cloud Feudalism & SaaS Rent Extraction:** Everyday productivity tools, CAD suites, agricultural controllers, and creative software are locked behind perpetual cloud subscriptions. If a vendor raises prices, goes bankrupt, or revokes an account, communities lose access to their own tools and historical records overnight.
2. **Hardware Bloat & Forced E-Waste Obsolescence:** Commercial operating systems and Electron-style web apps consume gigabytes of RAM for trivial tasks, intentionally rendering functional laptops, tablets, and thin clients obsolete every three to five years and driving a massive global e-waste stream.
3. **Surveillance Telemetry & Attention-Harvesting Dark Patterns:** Consumer apps and commercial video games are engineered around behavioral telemetry, variable-ratio compulsion loops, algorithmic feeds, and predatory microtransactions that harvest human attention and erode mental calm.
4. **Opaque Supply Chains & Single-Maintainer Burnout:** Critical open-source infrastructure often depends on unpaid, exhausted solo maintainers, while proprietary binary blobs hide backdoors and non-deterministic floating-point drift.

---

## 3. Wide Feature Horizon

Scenario Zero replaces proprietary cloud dependence with six integrated digital capabilities:

1. **Autopoietic Self-Hosting of the Sovereign Stack:** Hermetic, bit-for-bit reproducible multi-architecture builds (`x86_64`, `aarch64`, `riscv64`) of all node daemons, Zephyr RTOS microcontroller images, and deterministic WebAssembly (`WASM`) workflow modules.
2. **The Constellation SDK & Convivial Application Suite:** Authoring tools for lightweight Layer 6 Application Lenses (Hearth slates, agroecology fieldbooks, shop-floor BOM terminals, paramedic pagers) plus local-first CRDT collaborative applications (parametric CAD/CAM, BIM, digital audio workstations, raster/vector studios, and clinical viewers).
3. **The 20-Year Salvaged Hardware Mandate:** Every system daemon and Constellation lens is budgeted to run within $\le 256\,\mathrm{MiB}$ base OS RAM and $\le 512\,\mathrm{MiB}$ application arena memory so salvaged ThinkPads, Raspberry Pis, and RISC-V boards remain first-class workstations for decades.
4. **Oasis, Sovereign Game Studios & Zero-Royalty Modding:** Video games, narrative RPGs, physics sandboxes, and **Oasis** (both a recreational settlement simulator and the node's `Layer 1b` shadow digital twin) are developed as celebrated cultural art alongside `Scenario Lambda` musicians and writers, with first-class mod graphing and zero platform royalties.
5. **The Upstream Detox Quarantine Diode:** A clean-room ingestion pipeline that imports useful legacy open-source packages from Layer 7, strips telemetry SDKs, ad trackers, and proprietary blobs, replaces cloud calls with local CRDT/mesh bindings, and vendors the detoxed source into the local offline store.
6. **Thermodynamic Compute & Solar Batching:** Heavy CI compilation, 3D lightmap baking, formal verification (`Lean 4` / `TLA+`), and local open-weight AI inference are scheduled automatically into `Scenario Beta` solar microgrid peaks while capturing $\ge 8000\,\mathrm{bp}$ ($80\%$) of server rack heat into `Scenario Zeta` bathhouses and `Scenario Gamma` greenhouses.

---

## 4. The Layer 6 Surface: Needs, Intents, Semantic Representations & Application Lenses

### A. Human Needs & Canonical Intents
Developers, artists, maintainers, and residents coordinate digital production through four primary JSON-LD intents:
* **`SoftwareOrGameProductionIntent`:** Commissioning a new application lens, daemon patch, game build, or hermetic CI compilation batch, automatically binding thermal waste-heat recovery and bedroom acoustic limits ($\le 380\,\mathrm{cB}$).
* **`CommunityModPublicationIntent`:** Publishing a creative game mod, custom ontology extension, SHACL form bundle, or pedagogical simulation sandbox (`Scenario Iota`) with automatic cryptographic attribution to upstream dependencies.
* **`SoftwareAgoraPackageManifest`:** Registering a reproducibly built, multi-party threshold-signed package CID in the decentralized mesh software catalog.
* **`DrudgeryAutomationIntake` (paired with `Scenario Sigma`):** Receiving ergonomic strain quests from physical guilds and turning them into open firmware/robotics bounties.

### B. Semantic Representations & Deterministic Read-Backs
`col-commonsd` exposes the software commons through self-describing ontological graphs in `https://w3id.org/collective/v1/`:
* **Capability Nutrition Labels (`AppCapabilityManifestV1`):** Every Application Lens or software package carries a machine-enforced manifest rendered as a plain-language **Nutrition Label** before installation or signature. It declares exact memory ceilings ($\le 512\,\mathrm{MiB}$), zero-float verification (`0` IEEE-754 opcodes in deterministic modules), requested RBAC scopes (`intent:author`, `crdt:read`, etc.), network isolation guarantees, and `>= 10,000`-tick sandbox canary test results (`0x01`).
* **Content-Addressed SBOM Merkle DAGs:** Software Bills of Materials link every binary CID to its exact source tree, compiler hash, and upstream contributor DIDs, enabling non-financial Reputational Wealth ($\mathcal{R}$) refresh credits to flow automatically to underlying library maintainers and artists when their work is used.
* **Mandatory Release Read-Back:** Before maintainers co-sign a release with their hardware keys, the lens presents a **Deterministic Read-Back** highlighting any changes in capability scopes, memory footprint, or socket permissions compared to the prior version.

### C. Constellation of Application Lenses & Screenless Modalities
Layer 6 serves software and game craft through specialized lenses that suppress $91\%$ of routine background CI/test noise:

| Application Lens / Modality | Target Context & RBAC Scope | Key Human Experience & Variety Attenuation |
| :--- | :--- | :--- |
| **The Sovereign Forge Studio** | Salvaged Laptop / Workstation (`forge:author`, `bpmn:review`) | Local-first IDE, mod workbench, and SHACL/JSON-LD lens builder; shows live multi-arch Nix/Guix reproducibility status and schedules heavy compilation jobs into afternoon solar surplus. |
| **Capability Nutrition Label Registry** | Tablet / E-Ink / Desktop (`crdt:read`, `gov:ratify`) | Displays high-contrast "Nutrition Labels" for every app and mod; lets residents inspect permissions and verify zero telemetry before pairing an app via VLAN 30 `#pair_token`. |
| **Detox Quarantine Inspector** | Maintainer Workbench (`forge:detox`) | Side-by-side diff viewer highlighting stripped Layer 7 tracking endpoints, cloud licensing checks, and non-deterministic float calls before a package enters the offline mesh mirror. |
| **Offline DTN Package Courier Sync** | Screenless / CLI / E-Ink Dock (`courier:dtn`) | Syncs multi-gigabyte software releases, game assets, and OS images across `Scenario Beta` cargo-trike sneakernet drives and Reticulum radio links with zero internet connection. |

---

## 5. Governance Across Human Scales ($N = 1 \longrightarrow N_s \ge 1, N_p > 1 \longrightarrow N_s > 1, N_p > 1$)

1. **Solo Steward Scale ($N = 1 \implies N_s = 1\text{ operator}, N_p = 1\text{ person served}$):**
   * *Context:* A solo homesteader or developer running a single-node homelab (`col-storaged` + local Constellation apps) on a salvaged mini-PC.
   * *Governance Model:* **Complete personal sovereignty with zero ceremony.** The solo operator can author, fork, and deploy personal scripts, mods, and lenses immediately; automated Layer 5/Layer 6 guards still protect them by enforcing the Capability Nutrition Label check, zero-float canary verification, and bedroom quiet ceilings ($\le 380\,\mathrm{cB}$) on night builds.
2. **Single / Core Steward Serving a Wider Circle ($N_s = 1\text{ or small core}, N_p > 1\text{ people served}$):**
   * *Context:* One or two neighborhood sysadmins/developers maintaining the shared homelab server and Constellation app catalog for a $5\text{–}15$ household pod ($N_p > 1$).
   * *Governance Model:*
     * **Least-Privilege App Isolation:** Because every resident pairs their personal device via capability-scoped `AppCapabilityToken` leases over VLAN 30 TLS 1.3, the solo maintainer never has access to neighbors' private $R_0$ notes, health records, or kinship data.
     * **Canary Before Production:** Updates to shared household lenses must pass a deterministic $10{,}000\text{-tick}$ `Oasis` shadow-canary rehearsal (`0x01`) before going live, preventing a rushed late-night patch from breaking the neighborhood kitchen or tiffin latches the next morning.
     * **Anti-Martyrdom Maintainer Shield:** Bug reports and feature wishes from the $N_p$ circle queue in a calm pull-only backlog; `Scenario Sigma` blocks urgent notifications for non-critical software wishes so the neighborhood coder is never treated as an on-call IT helpdesk.
3. **Multi-Steward Guild Serving a Community & Bioregion ($N_s > 1\text{ stewards}, N_p > 1\text{ community & peer nodes}$):**
   * *Context:* A federated Software & Game Guild ($N_s = 10\text{–}100+$ developers, artists, and auditors) maintaining core stack daemons, Constellation apps, and game studios across $R_2 \dots R_5$.
   * *Governance Model:*
     * **Dual-Guild Threshold Co-Sign (`t-of-n` FROST):** Promoting a core daemon or default Constellation lens into `/var/lib/collective/apps/` requires independent hardware signatures from both the **Software Guild** (verifying reproducible build hashes and memory bounds) and the **Security/Epistemic Guild** (`Scenario Rho`/`Iota`).
     * **The Creative Studio Veto (`Protocol of Refusal`):** Any developer, artist, or playtester holds a zero-penalty architectural veto to halt any software or game feature that introduces addictive dark patterns, public productivity leaderboards, or surveillance telemetry.
     * **Fork Freedom & SBOM Reputation Propagation:** Dissenting creatives can fork any game or application lens cleanly on the CRDT graph; upstream library authors and `Scenario Lambda` artists automatically receive $\mathcal{R}$ stewardship recognition whenever downstream forks or mods are played across the mesh.

---

## 6. Governing the Reach into Layer 7 (Extractive vs. Defensive)

| Reach Mode | Governance Purpose & Trigger | Constitutional Controls & Automatic Sunsets |
| :--- | :--- | :--- |
| **1. Extractive / Transitional Reach into Layer 7** | **A. Upstream Open-Source Tarball Ingestion:** Pulling useful compilers, Linux/Zephyr kernel trees, and scientific libraries from the public internet.<br>**B. Commercial Game & App Publishing (`policy:commons_plus_l7_zero_bill`):** Selling binary editions, soundtracks, or supporter packs of **Oasis** and Collective indie games/apps on legacy storefronts (Steam, GOG, itch.io) or offering cooperative software consultancy to fund node hardware and tax endowments. | • **Mandatory Detox Quarantine Diode:** No external code crosses `l6_l7.sock` into the mesh without automated stripping of telemetry beacons, proprietary blobs, and cloud callouts, followed by local offline vendoring.<br>• **Automatic $\Phi_t = 0$ Commercial Cap:** Revenue from external game/software sales routes into the `Scenario Epsilon` cooperative carapace to zero out monthly node fiat bills ($\Phi_t \to 0$) and seed the Property Tax Escrow Endowment, while the software itself remains $100\%$ free and open inside the mesh.<br>• **Silicon Dependency Tracking ($D_t$):** Imported CPU/FPGA/SSD hardware is logged on the Legacy Dependency Register to guide long-horizon salvage (`Scenario Mu`) and open-silicon research. |
| **2. Purely Defensive Reach into Layer 7** | **A. Copyleft & Prior-Art Shielding:** Preventing corporate capture or patent trolling against Collective software, protocols, and game engines.<br>**B. Developer OpSec (`Mundane Semantic Camouflage`):** Protecting individual contributors from doxxing, employer retaliation, or DMCA harassment. | • **Strong Reciprocal Licensing & Cooperative Wrapper:** Publishes outward releases under strong copyleft (`AGPLv3` / `CERN-OHL-S v2`) held by an **IRC Subchapter T** cooperative studio so no external corporation can privatize improvements.<br>• **Author Pseudonymity & Camouflage:** `col-commonsd` strips internal steward `did:peer` commit graphs when pushing public mirrors across `l6_l7.sock`, signing external releases with a collective studio key. |
