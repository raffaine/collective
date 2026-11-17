# Radio Lambda: The Sovereign Stack & 25 Scenarios Podcast Bible

* **Series Title:** *Radio Lambda: Dispatches from the Sovereign Stack*
* **Format:** Two-Host Conversational Deep-Dive + 3-Minute Original Lyrical Songs (*Scenario Lambda Music Commons*)
* **Production Pipeline:** Hybrid **NotebookLM Audio Overviews** (15–20 min natural two-host dialogue) + **Gemini Carbon** (3-minute lyrical songs & spoken-word radio bumpers)

---

## 1. How the Production Workflow Works (NotebookLM + Gemini Carbon)

### What is NotebookLM?
[NotebookLM](https://notebooklm.google.com/) is Google's free Gemini-powered research and audio studio:
1. You create a new Notebook (e.g., `Episode 01 — The 3 AM Nursery Bug`) and upload **only** that episode's curated **Narrative Source Pack** (`episodes/epXX_source_pack.md`).
2. Because NotebookLM is strictly **source-grounded**, the hosts will **only** discuss the facts, stories, and architecture inside the file you uploaded—with zero outside hallucinations.
3. In the Studio panel under **Audio Overview**, you click **Customize** and paste the **Director Prompt** from `episodes/epXX_director_cues.md`.
4. Within ~3 minutes, it synthesizes a remarkably lifelike 12-to-20-minute two-host conversation (complete with natural laughter, interruptions, skepticism, and warmth) that you can download as an audio file.

### How Gemini Carbon Complements It (Two Modes)
* **Mode A — 3-Minute Lyrical Songs ("Radio Lambda" Bumpers):** Every episode in `episodes/epXX_director_cues.md` includes a style prompt and full lyrics for a **3-minute Gemini Carbon song** (an acoustic/indie-folk, post-rock, or solarpunk electronic track written as if recorded inside a `Scenario Lambda` unmediated music barn) to open and close the podcast episode.
* **Mode B — 3-Minute Spoken-Word / Musical-Storytelling Sessions Directly in Gemini Carbon:** If you want to generate 3-minute voiced narrative/radio-show blocks directly inside Gemini Carbon, `episodes/epXX_director_cues.md` also slices each episode into **three 3-minute Act Prompts** designed specifically for Gemini Carbon's 3-minute session window.

---

## 2. Host Personas & Conversational Chemistry

Across every episode, our Director Prompts steer the two hosts into a dynamic, human partnership:

* **Host A — "The Grounded Neighbor & Pragmatic Skeptic":**
  * Speaks from the perspective of an everyday person living in a normal suburb or apartment in Layer 7—tired of grocery inflation, rent hikes, subscription apps, and burnout, yet deeply skeptical of utopian communes, cults, endless committee meetings, and crypto-bro jargon.
  * Constantly stress-tests the architecture with real human questions: *"Wait—does that mean I have to go to a 3-hour meeting just to use a drill?"*, *"What if someone is a lazy freeloader?"*, *"What about the guy who thinks the collective is going to confiscate the custom shirt he sewed?"*, *"What happens when two housemates break up at midnight?"*
* **Host B — "The Systems Steward & Storyteller":**
  * Warm, lucid, and allergic to dry academic jargon. Never recites raw C++ structs or socket opcodes; instead, explains every layer and scenario through **physical objects, human stories, and lived moments**:
    * The **wooden-and-brass bedroom Sanctuary Latch** (`Scenario Sigma`),
    * The **washable E-Ink slate** next to the steaming pea-soup stockpot (`Scenario Gamma`),
    * The **1-tap $<500\,\mathrm{ms}$ Safe-Harbor exit fob** (`Scenario Omicron`),
    * The **physical brass knife-switch** that severs microphones in the music parlor (`Scenario Lambda`), and
    * Why we measure percentages in **basis points (`bp`, where $10{,}000\,\mathrm{bp} = 100\%$)** so no floating-point rounding bug ever repeats the 2:38 AM nursery freeze from *System Call: Genesis*.

---

## 3. Season 1 Episode Map (11 Episodes)

| Episode | Title | Core Material Covered | Signature Story & Song Theme |
| :--- | :--- | :--- | :--- |
| **[Ep. 01](./episodes/ep01_source_pack.md)** · [Cues](./episodes/ep01_director_cues.md) | **Pry-Bar at 2:38 AM: Why the Stack Exists** | *System Call: Genesis* + Overview of the 7 Layers & Scale-Adaptive Governance ($N=1 \to N_s, N_p$) | Gabe's locked smart inhaler, Jonas smashing the freezing nursery server rack, and the birth of the $10{,}000\,\mathrm{bp}$ unconditional baseline. |
| **[Ep. 02](./episodes/ep02_source_pack.md)** · [Cues](./episodes/ep02_director_cues.md) | **No Floating Points in the Basement (Layers 1–4)** | Physical Reality (L1), Oasis Rehearsal Twin (L1b), Zephyr Safety Reflexes (L2), Zero-Sum `BCC` Ledgers (L3), and 10 Hz Deterministic Workflows (L4) | Why a blade guard drops power in $<10\,\mathrm{ms}$ in hardware, Fault-Before-Malice flowmeter checks, and why `bp` and microjoules (`uJ`) replace debt-money. |
| **[Ep. 03.02](./episodes/ep03_source_pack.md)** · [Cues](./episodes/ep03_director_cues.md) | **The Soul of the Machine: Kinship, Ostrom Commons, Indigenous Ontologies & Circular Layer 7 (Layers 5–7)** | Elinor Ostrom's 8 principles + VSM System 5 (L5), Pluralistic & Indigenous RDF Named Graphs + Stoichiometric Need DAGs (L6), and Healthy Circular Extraction + Catered GPO Sourcing (L7) | Defeating a 25x25 Sybil cartel, Te Mana o te Wai & Ayllu ontologies over shared creeks, internalizing extracted resources into a circular commons, and the Progressive Siphon. |
| **[Ep. 04](./episodes/ep04_source_pack.md)** · [Cues](./episodes/ep04_director_cues.md) | **Wave A: 20-Year Laptops, Walk-In Sanctuary & The Brass Rest Latch** | Scenarios `Zero` (Software/Games), `Tau` (4-Pathway Genesis), and `Sigma` (Burnout Homeostasis) | Heating bathhouses with compiler waste heat, joining with zero smartphone (`R0_GUEST`), the 40% Anti-Martyrdom circuit breaker, and Drudgery-to-Automation quests. |
| **[Ep. 05](./episodes/ep05_source_pack.md)** · [Cues](./episodes/ep05_director_cues.md) | **Wave B: Pea Soup, Shop-Floor LOTO & Cargo-Trike Sneakernet** | Scenarios `Gamma` (Hearth), `Alpha` (Fabrication/Tools), and `Beta` (5-Artery Transit) | Zero-knowledge peanut-allergy tiffin locks, private off-peak woodworking (`0` hobby logs), and offline milk-runs carrying `DTN` sneakernet drives. |
| **[Ep. 06](./episodes/ep06_source_pack.md)** · [Cues](./episodes/ep06_director_cues.md) | **Wave C: The 90-Day Cocoon, Chosen Kin & Screenless Homes** | Scenarios `Delta` (Childcare), `Omicron` (Kinship), `Kappa` (Apprenticeship), and `Zeta` (CLT Shelter) | One signature before birth triggering 90 days of silent soup and diapers; $<500\,\mathrm{ms}$ Safe-Harbor exit; zero cameras in bedrooms. |
| **[Ep. 07](./episodes/ep07_source_pack.md)** · [Cues](./episodes/ep07_director_cues.md) | **Wave D: Mother-Tree Vetoes, Solar Crucibles & Reversible Timber** | Scenarios `Eta` (Coppice), `Mu` (Unbuilding/Biochar), `Theta` (Foundry), `Nu` (Textiles), and `Xi` (Open Building) | Wiping body measurements from RAM the second a custom coat is handed over (`OP_PURGE_R0_LINEAGE`), and nail-free *Kigumi* barn-raising. |
| **[Ep. 08](./episodes/ep08_source_pack.md)** · [Cues](./episodes/ep08_director_cues.md) | **Wave E: Zero-ID Cafés, Null-Result Science & Unmediated Music** | Scenarios `Epsilon` (Third Place), `Iota` (Epistemic), `Lambda` (STEAM Culture), and `Pi` (Guild Patchwork) | Why negative scientific results get equal fame, flipping the mic kill-switch at a concert, and `MeshGrowthBeacon` demand capture. |
| **[Ep. 09](./episodes/ep09_source_pack.md)** · [Cues](./episodes/ep09_director_cues.md) | **Wave F (Part 1): Red Pull-Cords, Open-Hearth Storms & Emeritus Sages** | Scenarios `Chi` (Clinical/Pharma), `Upsilon` (Shock), and `Phi` (Elder Sanctuary) | Sub-$500\,\mathrm{ms}$ zero-LLM trauma pagers, `HPLC`-verified open insulin, welcoming dark suburbs in a blizzard at `$0`, and mmWave fall safety. |
| **[Ep. 10](./episodes/ep10_source_pack.md)** · [Cues](./episodes/ep10_director_cues.md) | **Wave F (Part 2): Peace Hearths, Fault-Before-Malice & The Planetary Lighthouse** | Scenarios `Psi` (Justice), `Rho` (Immunity), and `Omega` (Terminal Decoupling) | Presuming a spiderweb in the meter before accusing a neighbor, locking the Property Tax Endowment, and shipping Genesis Seed Kits to the world. |
| **[Ep. 11](./episodes/ep11_source_pack.md)** · [Cues](./episodes/ep11_director_cues.md) | **Season 1 Finale: The Minimum Viable Society & The Historical Rupture** | Full MVS Retrospective (Waves A–F, Non-Stewards) + Chapter 1's Historical Rupture & Chapter 12's Centering Arch (*Le Cintre*) | Why Marx, Lenin, Trotsky, and China's state-capitalist ascent hit a structural wall—and how The Collective uses existing society as the Medieval Centering Arch to leverage its own emancipation. |

---

## 4. Directory Structure

```text
doc/podcast/
├── README.md                           # This Series Bible & Production Guide
└── episodes/
    ├── ep01_source_pack.md             # Directed Narrative Source Pack for Episode 01
    └── ep01_director_cues.md           # NotebookLM Director Prompt + 3-Min Gemini Carbon Song/Act Prompts
```
