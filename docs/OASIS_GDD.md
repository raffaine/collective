# OASIS GAME DESIGN DOCUMENT (GDD)
**Document ID:** `OASIS-GDD-V4.0`  
**Milestone:** Round 3 Architectural Council Ratification  
**Classification:** Authoritative Game Design Specification  
**Engine Baseline:** C++20 / WebGPU / Flecs ECS / UHAI IPC  
**Status:** Council Approved (Unanimous Consensus Ratified)  

---

## 1. Executive Vision & Core Philosophy

### 1.1. The Trojan Horse Simulator
Oasis is not an escapist fantasy, a sandbox playground, or a gamified spreadsheet. It is the **Trojan Horse Simulator** for physical-world sovereign resilience. 

Mainstream simulation games indulge in comfortable, detached illusions:
* In *The Sims*, human beings are reduced to predictable biological machines driven by colored meters, instantly cured of grief by a slice of pizza or thirty minutes of television.
* In *Cities: Skylines*, civilization sprouts magically upon a pristine, empty green meadow connected to an infinite, benevolent municipal utility grid.
* In standard base builders (*RimWorld*, *Banished*), the player wields an omniscient, authoritarian god cursor, commanding mortal humans to march into hazardous labor or fatal combat without friction, ethical hesitation, or emotional trauma.

Oasis rejects every one of these illusions. The real Anthropocene provides no untouched virgin fields, no benevolent utility grids, and no obedient digital pawns. We live within the omnipresent, crumbling, extractive infrastructure of late-stage industrial society: cracked asphalt cul-de-sacs, failing high-voltage power lines, contaminated municipal water mains, predatory municipal code enforcement, and soaring fiat tariffs.

Oasis simulates the gritty, tactile, and exhilarating reality of **sovereign emancipation**:
1. You do not begin with an off-grid utopia; you begin as an impoverished, vulnerable enclave **leeching parasitically** from the dying legacy infrastructure.
2. You do not command mindless workers; you guide **Founder 01**, a mortal human steward with distinct ethical convictions, bounded stamina, and a deep psychological inner life.
3. You do not conquer the map through imperial expansion; you systematically **sever the legacy umbilical cord**, regenerate living soil, build mutual-aid trust circles, and connect with real-world neighbors via cryptographic mesh networks to dissolve the hostile legacy frontier.

Every gameplay system in Oasis is a compiler: player mastery over thermal mass, rocket mass heaters, solar microgrids, biochar sorption, and community governance in the game mirrors the exact cybernetic blueprints required to build resilient, sovereign physical nodes in the real world.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 THE FOUR HERITAGE PILLARS OF OASIS                               │
├────────────────────────────────┬────────────────────────────────┬────────────────────────────────┤
│ D1: Dwarf Fortress Psychology  │ D2: The Sims Indirect Founder  │ D3: Macro Legacy Leeching      │
│ - 64-byte bounded ECS state    │ - Mortal Founder 01 embodiment │ - High-voltage grid siphoning  │
│ - 32-slot episodic memory ring │ - Advisory intention queue     │ - Water & sewage interception  │
│ - Big-Five facets & 8 Values   │ - Spontaneous whims & desires  │ - Urbanite & rebar quarrying   │
│ - Focus vs Mood vs Stress      │ - Moral & stress rejection     │ - Suburban, Urban & Eco biomes │
│ - Breakdowns & Strange Moods   │ - Community social delegation  │ - Parasitic -> Sovereign arc   │
├────────────────────────────────┴────────────────────────────────┴────────────────────────────────┤
│ D4: Sovereign Stack Multiplayer & The Boundary Interface                                         │
│ - Local-first deterministic simulation (zero central game servers)                               │
│ - Stochastic Boundary Interface: Inhomogeneous Poisson process for legacy pressure shocks       │
│ - 3 Attenuation Pillars: Physical autarky, NPC integration, and cryptographic peer proximity     │
│ - Multiplayer executed EXCLUSIVELY via Sovereign Stack (Layers 2-5 & 7) over UHAI IPC            │
│ - Cryptographic speed-of-light geo-proximity proofs & shared boundary perimeter consolidation     │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## 2. Directive 1: Dwarf Fortress History & Emergent Psychology

### 2.1. The Failure of Sims-Style Need Bars
Legacy life simulation games represent human condition through superficial, linear bars (Hunger, Energy, Hygiene, Bladder, Fun, Social). When a bar dips into the red, an icon flashes; the player clicks an appliance, and the bar fills. 

This model fails to simulate genuine human beings for three fatal reasons:
1. **Zero Temporal Depth (Instant Amnesia):** An agent whose fun bar reaches 100% is instantaneously joyful, even if their home burned down thirty seconds prior or their companion died in a winter freeze. They possess no historical memory, no trauma, and no grief.
2. **Zero Value Friction:** An agent will perform any task regardless of whether it violates their lifelong convictions, so long as their motive bars are adequately serviced.
3. **Mechanical Doll Behavior:** Agents behave as state machines responding to immediate bodily triggers rather than conscious beings shaped by lived history.

In Oasis, we completely abandon this paradigm. We adopt the psychological architecture of *Dwarf Fortress*, grounding human simulation in **Personality Facets**, **Core Values**, **Needs**, **Transient Thoughts**, **Stress Accumulators**, and a **Bounded Episodic Memory Graph**.

### 2.2. The Deep Psychology Data Model
To achieve *Dwarf Fortress* psychological fidelity without heap allocation overhead or garbage collection spikes, every agent's psychological state is packed into a cache-aligned, 64-byte structure managed by the Entity Component System (ECS):

```cpp
namespace oasis::psychology {

// Fixed-point type aliases for bit-exact cross-platform determinism
using Fixed8  = int8_t;   // Quantized scalar [-100 to +100]
using Fixed16 = int16_t;  // Q8.8 fixed-point [-128.00 to +127.99]
using Fixed32 = int32_t;  // Q24.8 fixed-point [-8,388,608.00 to +8,388,607.99] for stress accumulation

// 1. Personality Facets (Derived from Big-Five OCEAN + DF Facet Model)
// Quantized as signed 8-bit integers (-100 to +100). 0 = neutral human baseline.
struct alignas(4) PersonalityFacets {
    Fixed8 assertiveness;       // Propensity to lead, command, and direct others
    Fixed8 anxiety;             // Vulnerability to external stressors and threats
    Fixed8 empathy;             // Sensitivity to others' suffering; altruistic drive
    Fixed8 self_discipline;     // Ability to sustain focus through discomfort and fatigue
    Fixed8 curiosity;           // Drive to explore, experiment, and invent blueprints
    Fixed8 pragmatism;          // Willingness to compromise ideals for sheer survival
    Fixed8 stoicism;            // Inherent buffering against immediate emotional shocks
    Fixed8 sociability;         // Craving for communal proximity vs solitary isolation
    Fixed8 discordance;         // Propensity for skepticism, friction, and conflict
    Fixed8 artistic_drive;      // Need for beauty, craftsmanship, and creative expression
    Fixed8 padding[6];          // SIMD cache-line alignment padding
};
static_assert(sizeof(PersonalityFacets) == 16, "PersonalityFacets must be 16 bytes.");

// 2. Core Values (8 Philosophical & Cultural Convictions, -100 to +100)
enum class CoreValueType : uint8_t {
    AUTONOMY     = 0, // Hatred of legacy servitude vs comfort in external rules
    STEWARDSHIP  = 1, // Regenerative permaculture stewardship vs extractive exploitation
    SOLIDARITY   = 2, // Mutual aid obligation vs individual hoarding
    INGENUITY    = 3, // Open-source fabrication and invention vs passive consumption
    TENACITY     = 4, // Perseverance through winter hardships vs fragility
    PRAGMATISM   = 5, // Willingness to compromise ideals for immediate community survival
    TRANSPARENCY = 6, // Reverence for truth and open audit vs paranoia and secrecy
    BEAUTY       = 7  // Need for craftsmanship, art, and dignity vs utilitarian sterility
};

struct alignas(4) CoreValues {
    Fixed8 autonomy;            // [-100 to +100]
    Fixed8 stewardship;         // [-100 to +100]
    Fixed8 solidarity;          // [-100 to +100]
    Fixed8 ingenuity;           // [-100 to +100]
    Fixed8 tenacity;            // [-100 to +100]
    Fixed8 pragmatism;          // [-100 to +100]
    Fixed8 transparency;        // [-100 to +100]
    Fixed8 beauty;              // [-100 to +100]
    Fixed8 padding[8];          // 16-byte alignment padding
};
static_assert(sizeof(CoreValues) == 16, "CoreValues must be 16 bytes.");

// 3. Transient Thought: Reaction to an active environmental or social stimulus
struct alignas(4) Thought {
    uint16_t thought_id;        // Categorical ID (e.g., THOUGHT_COLD_RAIN, THOUGHT_COMMUNAL_FEAST)
    Fixed16  valence;           // Emotional impact (Q8.8: -128.0 to +127.0)
    uint32_t timestamp_ticks;   // Tick when thought occurred
    uint16_t duration_ticks;    // Duration thought influences active mood
    uint16_t source_entity_id;  // Entity instigating the thought (peer, inspector, null)
};
static_assert(sizeof(Thought) == 12, "Thought must be 12 bytes.");

// 4. Bounded 32-Slot Episodic Memory Node: Permanent historical record
struct alignas(8) EpisodicMemoryNode {
    uint32_t timestamp_ticks;   // Simulation tick of event occurrence
    uint16_t event_type;        // Categorical event (MEM_CODE_RAID, MEM_FIRST_KILOWATT, etc.)
    uint16_t subject_entity_id; // Related actor (peer, inspector, or 0)
    Fixed16  emotional_valence; // Consolidated emotional weight (Q8.8)
    uint16_t salience;          // Salience score [0..65535] determining retention
    uint16_t voxel_x;           // Spatial coordinate where event occurred
    uint16_t voxel_y;
    uint16_t voxel_z;
    uint16_t flags;             // Bit 0: PERMANENT_MEMORY (>80 salience root trauma), Bits 1-15: Trigger count
};
static_assert(sizeof(EpisodicMemoryNode) == 24, "EpisodicMemoryNode must be exactly 24 bytes.");

} // namespace oasis::psychology
```

### 2.3. The Cognitive Triad: Focus, Mood, and Stress Accumulation
Every agent operates under a three-tier cognitive state machine:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                   THE COGNITIVE TRIAD ENGINE                                     │
├──────────────────────────┬──────────────────────────┬────────────────────────────────────────────┤
│ Cognitive Metric         │ Numerical Range          │ Mechanics & Behavioral Impact              │
├──────────────────────────┼──────────────────────────┼────────────────────────────────────────────┤
│ **Short-Term Focus**     │ $0 \dots 255$            │ Immediate operational clarity. Depleted by │
│                          │ (Uint8)                  │ bodily pain, cold, hunger, or unfulfilled  │
│                          │                          │ whims. High focus ($\ge 200$) yields       │
│                          │                          │ $+25\%$ build speed & masterwork chance.   │
│                          │                          │ Low focus ($\le 50$) causes tool drops,    │
│                          │                          │ miswiring, and industrial accidents.       │
├──────────────────────────┼──────────────────────────┼────────────────────────────────────────────┤
│ **Active Mood Balance**  │ $-10,000 \dots +10,000$  │ Fast-moving emotional equilibrium. The sum │
│                          │ (Fixed32)                │ of all active transient thoughts in the    │
│                          │                          │ agent's buffer, weighted by Personality    │
│                          │                          │ Facets and Core Values.                    │
├──────────────────────────┼──────────────────────────┼────────────────────────────────────────────┤
│ **Cumulative Stress**    │ $-100,000 \dots +100,000$│ Slow-moving hysteretic accumulator.        │
│                          │ (Fixed32)                │ Integrates persistent negative mood over   │
│                          │                          │ days and weeks. Drives breakdown states.   │
└──────────────────────────┴──────────────────────────┴────────────────────────────────────────────┘
```

#### Mood Calculation Formula
At each 1 Hz psychological tick, active mood $\mathcal{M}(t)$ is computed from active thoughts $\mathcal{T}(t)$:

$$\mathcal{M}(t) = \sum_{i \in \mathcal{T}(t)} \text{valence}_i \cdot \left( 1.0 + \frac{\text{FacetImpact}_i + \text{ValueImpact}_i}{100.0} \right)$$

*Example:* An agent with `CoreValues.ecological_harmony = +90` who experiences `THOUGHT_SEVERED_LEGACY_GRID` receives double positive valence, whereas an agent with `PersonalityFacets.anxiety = +80` observing `THOUGHT_MUNICIPAL_INSPECTION_NOTICE` suffers a massive negative mood shock.

#### Stress Integration & Dual-Timescale Lyapunov Homeostasis
Stress does not jump erratically; it accumulates as a continuous differential equation governed by a **Dual-Timescale Lyapunov Damping System** to prevent catastrophic stress death spirals while preserving slow-moving hysteretic breakdown memory:

1. **Fast Acute Damping ($\tau_{\text{acute}} = 30\text{s}$):**
   Transient thoughts (cold rain, dropped tool, burned soup) decay quickly towards active mood equilibrium:
   $$\dot{S}_{\text{acute}} = \begin{cases} -\frac{1}{\tau_{\text{acute}}} (\mathcal{M}(t) - \mathcal{M}_{\text{threshold}}), & \text{if } \mathcal{M}(t) > \mathcal{M}_{\text{threshold}} \\ +\frac{1}{\tau_{\text{acute}}} (\mathcal{M}_{\text{threshold}} - \mathcal{M}(t)) \left(1.0 + \frac{\text{Anxiety}}{100.0}\right), & \text{if } \mathcal{M}(t) < \mathcal{M}_{\text{threshold}} \end{cases}$$

2. **Slow Baseline Integration ($\tau_{\text{baseline}} = 7\text{ days} = 604,800\text{ s}$):**
   Cumulative stress integrates unresolved chronic trauma over long horizons, governed by a non-linear Lyapunov restoring function:
   $$\frac{d(\text{Stress})}{dt} = \dot{S}_{\text{acute}} - \kappa_{\text{resilience}} \cdot (\text{Focus} + \text{MutualAid}) - \frac{1}{\tau_{\text{baseline}}} \tanh\left(\frac{\text{Stress} - \text{Stress}_{\text{baseline}}}{S_0}\right)$$

3. **Autonomous Clinical Recovery Attractor (Elimination of Catatonia Death Spirals):**
   When stress crosses into Catatonia ($> 80,000$), unmitigated feedback cascades are halted: peers initiate autonomous clinical care (spoon-feeding warm broth, tucking into warm wool blankets, administering herbal teas). This enforces a guaranteed restorative drift ($\Delta \text{Stress}_{\text{clinical}} = -150\text{ units/hour}$), breaking absorbing death spirals and ensuring a realistic, non-linear pathway back to sanity.

### 2.4. Breakdown State Machine & Strange Moods
When cumulative stress crosses critical thresholds, the agent’s cognitive state machine transitions into a **Breakdown State**:

```
                       [Normal Operation: Stress < 25,000]
                                       │
                                       ▼ (Stress > 25,000)
                       ┌─────────────────────────────────┐
                       │   MILD BREAKDOWN: Irritable     │
                       │   - Snaps at peers in mess hall │
                       │   - Refuses non-essential tasks │
                       └───────────────┬─────────────────┘
                                       │
                                       ▼ (Stress > 50,000)
                       ┌─────────────────────────────────┐
                       │  MODERATE BREAKDOWN: Split State│
                       ├────────────────┬────────────────┤
                       │    TANTRUM     │   MELANCHOLY   │
                       │ Smashes solar  │ Sits in dirt,  │
                       │ inverters &    │ stares blankly,│
                       │ kicks tools    │ ignores orders │
                       └────────┬───────┴────────┬───────┘
                                │                │
                                └───────┬────────┘
                                        ▼ (Stress > 80,000)
                       ┌─────────────────────────────────┐
                       │  SEVERE BREAKDOWN: Catatonia    │
                       │  - Complete biological shutdown │
                       │  - Requires peers to spoon-feed │
                       │  - Clinical recovery attractor  │
                       └─────────────────────────────────┘
```

#### The Creative Burst ("Strange Mood")
Under specific inverted conditions—when an agent possesses `Craftsmanship >= 70`, `Autonomy >= 60`, moderate pressure, and a sudden spike of inspiration—they enter a **Strange Mood**:
1. **Trance State:** The agent locks themselves inside a workshop, claiming physical tools and workstations.
2. **Material Demands:** They express urgent, specific material requirements (e.g., salvaged 4-gauge copper wire, seasoned white oak, biochar, beeswax).
3. **Masterwork Artifact:** If supplied within 72 in-game hours, they fabricate a **Masterwork Sovereign Artifact** (e.g., an ultra-efficient MPPT charge controller, a high-efficiency rocket mass heater, or a peer-to-peer LoRa transceiver with hand-wound coils).
   - The artifact confers permanent $+20$ efficiency to the parcel.
   - The creator receives a permanent $+15$ boost to `Stoicism` and permanently drops stress to zero.
4. **Tragic Collapse:** If materials are denied, the trance collapses into suicidal melancholia or permanent berserk mania.

### 2.5. Bounded Episodic Memory Ring & Sensory Flashbacks
Every agent owns a **32-slot circular ring buffer** for episodic memories:
* **Salience Decay (Ebbinghaus Curve):** Memories decay on diurnal sleep cycles:
  $$S(t) = S_0 \cdot \exp\left(-\frac{\Delta t}{\tau_{\text{decay}}}\right) \cdot \left(1 + \frac{|\text{valence}|}{V_{\text{max}}}\right)$$
* **Trauma Protection & Salience-Priority Ring Replacement:** High-salience root traumas ($S(t) > 80$) are explicitly flagged as `PERMANENT_MEMORY`. When the 32-slot circular buffer reaches capacity during an event burst (e.g., 40 secondary events following a raid), the replacement algorithm evicts the entry with the lowest salience score that lacks the `PERMANENT_MEMORY` flag. Root cause traumas are permanently immune to FIFO eviction before diurnal sleep consolidation completes, eliminating "Trauma Amnesia".
* **Sleep Consolidation:** During sleep, memories in the ring buffer are consolidated into the agent's permanent 8 Core Values. Traumatic survival freezes shift `CoreValues.solidarity` and `PersonalityFacets.stoicism` upward.
* **Spatial Sensory Flashbacks:** When Founder 01 walks within 4 decimeters of a voxel where an intense trauma occurred (e.g., a violent municipal code enforcement raid at coordinate $(95, 32, 48)$), a spatial hash query triggers a **sensory retrieval**:
  > *"Founder 01 remembers the violent raid of Autumn Year 1. A cold dread settles over them."*
  A negative thought is injected into the thought buffer, and Founder 01’s focus temporarily plummets.

---

## 3. Directive 2: The Sims Indirect Founder Management

### 3.1. Rejection of the RTS God-Mode Cursor
In conventional colony sims (*RimWorld*, *Banished*), the player acts as a detached digital dictator: select five workers, click an iron vein, and watch them march robotically into backbreaking labor without hesitation or fatigue.

In Oasis, this is rejected as an authoritarian anti-pattern that violates the principles of human agency and mutual aid. You are not a god hovering over Lot 402. You are the **Guiding Steward** of **Founder 01**, a mortal founding human being.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                   INDIRECT STEWARDSHIP LOOP                                      │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                                  │
│   PLAYER (Cadastral Slate / HUD)                               FOUNDER 01 (Autonomous Agent)     │
│   ┌───────────────────────────┐                               ┌──────────────────────────────┐   │
│   │ Suggests Blueprints       │                               │ Evaluates Needs & Stamina    │   │
│   │ Enqueues Advisory Chores  │──[Advisory Intention Queue]──►│ Evaluates Moral Alignment    │   │
│   │ Honors Personal Whims     │                               │ Decides: Execute or Defy     │   │
│   └───────────────────────────┘                               └──────────────┬───────────────┘   │
│                 ▲                                                            │                   │
│                 │                                                            │ Environmental     │
│                 │               Gibsonian Affordances                        ▼ Feedback          │
│                 └───────────[Heat, Cooked Stew, Clean Water, Tools]──────────┘                   │
│                                                                                                  │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

### 3.2. Founder 01 Embodiment & Indirect Stewardship
Founder 01 is embodied within the physical voxel simulation:
* **Caloric & Fatigue Boundaries:** Founder 01 burns real calories ($2,000\text{ kcal/day}$ baseline, up to $4,500\text{ kcal/day}$ breaking asphalt or felling timber). Working through exhaustion rapidly depletes Focus and spikes Stress.
* **Advisory Intention Queue:** The player designates architectural plans and queues chores on the **Cadastral Slate** (e.g., `[Scavenge Asphalt]`, `[Turn Compost]`, `[Wire Inverter]`). Founder 01 processes this queue through an internal deliberative utility evaluator.
* **Whims & Creative Aspirations:** Founder 01 spontaneously generates personal desires (e.g., *"Wants to build a solar food dehydrator"*, *"Wants an uninterrupted afternoon to weld bicycle frames"*, *"Craves solitary rest by the woodstove"*). Fulfilling whims restores Focus ($+20$) and purges Stress ($-15$).
* **Gibsonian Environmental Affordances:** The player influences Founder 01 primarily by altering the physical environment. Lighting a rocket mass heater naturally draws shivering characters to sit nearby and converse; simmering a pot of hearty root-vegetable stew at 18:00 naturally gathers the entire community into the communal kitchen, triggering social bonding thoughts.

### 3.3. Autonomous Agency & Moral Rejection Heuristics
Founder 01 is an autonomous sovereign human being who will reject orders that violate their ethics or exceed their stress tolerance:

```
                      [Player Directive Issued on Slate]
                                       │
                                       ▼
                         ┌───────────────────────────┐
                         │ G1: Stress Breakdown Check│───[Stress > 50,000]───► [REJECT: Tantrum/Melancholy]
                         └─────────────┬─────────────┘
                                       │ Passed
                                       ▼
                         ┌───────────────────────────┐
                         │  G2: Physiological Gating │───[Hydration < 10% or]──► [DEFER: Drops tool, rushes]
                         │      (Thirst, Exhaustion) │   [Fatigue > 90%     ]    [to well or bed first     ]
                         └─────────────┬─────────────┘
                                       │ Passed
                                       ▼
                         ┌───────────────────────────┐
                         │  G3: Core Value Alignment │───[Moral Conflict    ]──► [FLAT REFUSAL: Moral      ]
                         │      Gating Heuristic     │   [ΔValue > Threshold]    [Protest Telegraph        ]
                         └─────────────┬─────────────┘
                                       │ Passed
                                       ▼
                         [DIRECTIVE ACCEPTED & QUEUED]
```

#### The Moral Rejection Mathematical Curve
The probability of rejecting an unethical directive follows a logistic sigmoid function:

$$P(\text{Rejection}) = \frac{1}{1 + \exp\left(-k \cdot (\text{MoralConflict} + \text{StressDelta} - \text{Trust})\right)}$$

Where:
* $\text{MoralConflict} = \sum (\text{DirectiveImpact}_i \cdot \text{CoreValues}_i)$.  
  *Example:* Ordering Founder 01 to dump hazardous lead-acid battery electrolyte into the communal aquifer produces a catastrophic moral conflict against `CoreValues.ecological_harmony = +85`.
* **Visual Telegraph:** When Founder 01 rejects a directive, she drops the tool, turns toward the camera, and the game displays an in-world telegraph over her Plumbob:  
  > *"Founder 01 refuses: 'I will not poison the soil we eat from.' Focus dropped by 20."*

### 3.4. Anti-Lockup & Anti-Thrashing Guarantees
To ensure robust autonomous simulation without action deadlocks:
1. **25% Utility Hysteresis ($\Theta_{\text{hysteresis}} = 0.25 \cdot U_{\text{max}}$):** An agent executing an action will only switch to a candidate task if candidate utility exceeds current utility by at least 25%, preventing erratic whim oscillation between adjacent needs.
2. **Atomic Action Commit:** Once a physical task begins (cooking, welding, digging), it cannot be interrupted by non-critical whims until the action phase completes.
3. **Distance-Adaptive Lease TTL & Heartbeat Transit Renewal:** Workbenches, tools, and beds are initially reserved with a distance-scaled Time-to-Live (TTL) lease: $\text{TTL}_{\text{init}} = \max\left(60, \left\lceil \frac{d(\text{agent}, \text{target})}{\text{speed}} \right\rceil \times 1.5\right)$ ticks. As the agent traverses waypoints toward the affordance, continuous physical forward progress automatically emits a **heartbeat transit lease renewal** every 15 ticks, resetting the TTL. If an agent ceases movement or is diverted, the lease expires cleanly, returning the resource to the commons pool without transit race condition dropouts.
4. **30-Tick Progress Watchdog with Exponential Backoff:** If an agent's physical distance to an objective fails to decrease over 30 consecutive ticks, the task transitions to `ACTION_BLOCKED`. The agent steps back 1 decimeter, releases all reservations, increments an unfulfillability failure counter $k$, and blacklists the destination with **exponential backoff**:
   $$\text{BlacklistDuration}(k) \in \{ 30\text{s}, 60\text{s}, 120\text{s}, 300\text{s} \} \quad (900, 1800, 3600, 9000\text{ ticks at } 30\text{ Hz})$$
   capped at 300 seconds ($9,000$ ticks). This completely eliminates the periodic 150-tick livelock cycle where agents fruitlessly re-target blocked resources.

### 3.5. Community Scaling: Expansion via Leadership & Delegation
As the settlement grows from a lone camper into a thriving ecovillage, the player **never gains god-mode control over incoming NPCs**. Secondary agents remain completely autonomous:
* **The Morning Circle:** At dawn (06:00), Founder 01 rings the communal bell. Layer 4 BPMN WorkTokens are displayed on the public bulletin board.
* **Voluntary Task Claiming:** NPCs evaluate open tasks against their personal facets, skills, and focus. High-empathy citizens claim childcare and meal preparation; high-craftsmanship citizens claim inverter repair and masonry.
* **Trust Rings (Layer 5 Integration):** Founder 01 delegates critical responsibilities (battery management, seed vault access) only to citizens who have achieved **Ring 1** status in the Web of Trust. If Founder 01 attempts to command a **Ring 2** newcomer without building trust, the citizen bristles, rejects the work token, and suffers negative social thoughts.

---

## 4. Directive 3: Cities: Skylines Grandiosity & Leeching the Legacy

### 4.1. The Omnipresent Legacy Reality
Traditional city builders isolate the player within an empty paradise where clean electricity and unlimited water flow from the map edge in exchange for abstract currency.

In Oasis, the player is situated within the decaying, predatory reality of late-stage industrial infrastructure. Legacy infrastructure is everywhere: cracked concrete streets, high-voltage transmission lines buzzing overhead, leaking municipal cast-iron water pipes, and abandoned telecommunications pedestals.

Our core macroscopic gameplay loop is **"Leeching the Legacy"**: you do not begin with an off-grid solar paradise; you begin as an impoverished, vulnerable enclave that must survive by parasitically tapping, siphoning, intercepting, and scavenging the dying infrastructure of the old world.

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 THE LEGACY INFRASTRUCTURE MATRIX                                 │
├─────────────────────────┬───────────────────────────────┬────────────────────────────────────────┤
│ Target Legacy System    │ Siphoning / Scavenging Method │ Catastrophic Failure / Retaliation Risk│
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ High-Voltage Power Grid │ Inductive line clamps, meter  │ Arc flash explosion, transformer fire, │
│ (12kV / 240V drops)     │ bypasses, pirate solar inject │ smart-meter automated tamper alarm     │
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Municipal Water & Sewer │ Hot-tap saddle valves on cast │ Water pressure drop alert, backflow    │
│ (Cast-iron mains)       │ iron, storm runoff diversion  │ contamination, municipal cutoff squad  │
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Asphalt, Concrete &     │ Jackhammering roads, cutting  │ Heavy fine citations, structural slab  │
│ Structural Rebar        │ bridge rebar, crushing rubble │ collapse, toxic coal-tar runoff        │
├─────────────────────────┼───────────────────────────────┼────────────────────────────────────────┤
│ Fiber & Copper Telecom  │ Inductive telephone taps,     │ Carrier line fault detection, FCC/ISP  │
│ (Roadside punch blocks) │ splicing roadside fiber lines │ signal sweep, cellular triangulation   │
└─────────────────────────┴───────────────────────────────┴────────────────────────────────────────┘
```

### 4.2. Detailed Siphoning Mechanics

#### 1. Power Grid Siphoning & Smart Meter Anomaly Counter
* **The Siphon:** Founder 01 climbs an abandoned utility pole at night with insulated lines to attach an inductive clamp or tap a 240V split-phase drop before the municipal meter.
* **The Exergy Yield:** Supplies up to $7.2\text{ kW}$ of electrical power directly into makeshift battery banks.
* **The Retaliation Risk & Anomaly Dissipation:** Drawing $> 30\text{ A}$ creates a localized thermal signature and a grid-phase imbalance. The utility company’s smart grid anomaly detection counter rises by $+1\%$ per 100 ticks. If it hits $100\%$, an armed inspection crew accompanied by municipal code enforcement raids the parcel. When current draw drops below $10\text{ A}$, the anomaly counter safely dissipates at $-0.5\%$ per 100 ticks, rewarding tactical load shedding.
* **Electrical Flow Solver Dynamics:** Physical current propagation across tapped 12kV distribution lines and 240V drops is modeled via **Successive Over-Relaxation (SOR with $\omega = 1.6$)**. Because low-impedance copper distribution lines ($R \approx 0.005\ \Omega$, $g \approx 200\text{ S}$) connect directly to high-impedance pirate battery drops ($R \approx 8.0\ \Omega$, $g \approx 0.125\text{ S}$), extreme conductance contrast causes unrelaxed solvers to stall with $>80\%$ voltage error. The SOR solver contracts spectral error rapidly, converging in $<20$ iterations. To eliminate floating singular matrices ($\det(\mathbf{G}) = 0$) when players cut legacy ties in Epoch 3 (Sovereign Severance), the microgrid incorporates a virtual earth ground reference node ($g_{\text{virtual\_earth}} = 10^{-6}\text{ S}$), backed by a direct Sparse Cholesky factorization fallback during breaker trips and topology splices.

#### 2. Municipal Water & Sewage Interception
* **The Siphon:** Excavating two meters down into clay to expose a corroded 4-inch ductile iron municipal main, installing a stealth saddle tap with a manual ball valve, or breaking open a stormwater catch basin to route runoff into bioswales.
* **The Exergy Yield:** Supplies abundant water for crops and living.
* **The Retaliation Risk:** Sudden pressure drops trigger city SCADA alarms. Siphoning untreated storm runoff brings severe chemical contaminants (motor oil, microplastics, heavy metals) that must be routed through biochar filtration beds and mushroom mycelium before watering crops.

#### 3. Scavenging Asphalt, Concrete & Rebar (Urbanite Quarrying)
* **The Siphon:** Jackhammering degraded municipal asphalt roads and parking lots; extracting high-tensile steel rebar with oxy-acetylene torches or angle grinders.
* **The Exergy Yield:** Produces **Urbanite** (broken concrete chunks) for stone foundations, rocket mass heaters, and gabion retaining walls.
* **The Retaliation Risk:** Pulverizing asphalt generates toxic silica dust and releases toxic bitumen volatiles; unmasked workers suffer rapid lung degradation thoughts and physical fatigue.

#### 4. Tapping Legacy Fiber & Copper Telecom
* **The Siphon:** Punching down into roadside punch blocks, siphoning DSL dial-tone, or stripping abandoned fiber-optic bundles.
* **The Exergy Yield:** Grants external internet connectivity to download open-source CAD blueprints, firmware, and weather forecasts into your local Layer 6 semantic cache.
* **The Retaliation Risk:** Unencrypted legacy traffic increases visibility to municipal and federal surveillance dragnets.

### 4.3. Multi-Environment Biome Profiles
Oasis supports three radically distinct environments, each presenting unique legacy geometries, legal traps, and thermodynamic challenges:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                   MULTI-BIOME SPECIFICATION MATRIX                               │
├───────────────────┬──────────────────────────┬──────────────────────────┬────────────────────────┤
│ Characteristic    │ Suburban Sprawl          │ Dense Urban Block        │ Permaculture Eco-Node  │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Spatial Envelope  │ Sprawling Horizontal     │ Deep Vertical Canyons    │ Contoured Topography   │
│                   │ ($128 \times 128 \times 16\text{ dm}$) │ ($64 \times 64 \times 64\text{ dm}$)   │ ($96 \times 96 \times 32\text{ dm}$)   │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Legacy Tethers    │ Overhead power lines,    │ High-voltage conduits,   │ Scavenged storm drains,│
│ (Layer 7 Pressure)│ shallow water mains,     │ steam tunnels, subway    │ boundary fence lines,  │
│                   │ residential meters       │ vaults, fiat property tax│ off-grid well taps     │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Primary Materials │ Asphalt, Pine Siding,    │ Structural Steel,        │ Fungal Loam, Cob,      │
│                   │ Drywall, Turf Grass      │ Reinforced Concrete      │ Biochar, Bamboo, Clay  │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Hydrology Loop    │ Lawn runoff, impervious  │ Sump pumps, stormwater   │ Keyline swales, ponds, │
│                   │ asphalt gutter routing   │ retention vaults, sewage │ greywater reed beds    │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Leeching Vector   │ Tapping grid lines for   │ Siphoning waste heat,    │ Restoring dead topsoil,│
│                   │ microgrid battery charge │ scavenging structural rebar│ capturing runoff water │
├───────────────────┼──────────────────────────┼──────────────────────────┼────────────────────────┤
│ Adversarial Focus │ HOA bylaws, drone lawn   │ Building inspectors,     │ Downstream senior water│
│                   │ inspections ($150/day)   │ FLIR thermal sweeps      │ rights enforcement     │
└───────────────────┴──────────────────────────┴──────────────────────────┴────────────────────────┘
```

#### Spatial Chunk Memory & 128-Chunk Active Visibility Window
To render and simulate expansive multi-biome sectors without thrashing WebAssembly's 256 MB linear memory ceiling, the engine maintains a **128-chunk active sector visibility window** ($128 \times 128\text{ KB} = 16.38\text{ MB}$). A full neighborhood sector ($8 \times 8 \times 4$ chunks = 256 chunks) is simulated hierarchically: the core 128 chunks closest to Founder 01 execute full-fidelity thermodynamics, hydrology, and voxel physics, while outer chunks utilize compressed macro-block representations. This provides a full-horizon sector view without chunk paging thrashing or frame drops.

### 4.4. The Macro Progression Arc: Three Epochs of Sovereignty
The journey of Lot 402 spans three distinct historical epochs:
1. **Epoch 1: Parasitic Entanglement:** Complete dependency on legacy power and water. Rapid fiat escrow drain ($1,450 starting balance). Constant vulnerability to utility rate hikes and smart-meter surveillance.
2. **Epoch 2: Systematic Scavenging:** Siphoning power, tapping water, depaving driveways into food forests, quarrying urbanite, establishing compost toilets and greywater bioswales.
3. **Epoch 3: Sovereign Severance:** Physical utility drops cut. Solar microgrid and rain catchment achieve $100\%$ autarky. The municipal meter stops spinning. Monthly fiat drain drops to zero. Lot 402 transforms from a vulnerable consumer into an emancipated producer node.

---

## 5. Directive 4: Multiplayer & The Boundary Interface

### 5.1. Local-First Simulation Philosophy
Oasis rejects the extractive, fragile cloud-server model of modern gaming:
* **Zero Centralized Game Servers:** The simulation executes $100\%$ locally on the user's hardware (desktop native or browser WASM sandbox).
* **Zero Cloud Lock-in:** World voxel state, entity psychology, and historical logs reside in local storage (native SQLite/LMDB or browser OPFS).
* **Total Offline Resilience:** If the global internet goes down, your Oasis simulation continues running deterministically without interruption.

### 5.2. The Stochastic Boundary Interface
The perimeter of the simulated parcel is not an invisible wall or skybox. It is an active **Stochastic Boundary Interface** representing the friction of the legacy fiat world pressing against the sovereign sanctuary:

```
                  ┌──────────────────────────────────────────────────┐
                  │          THE UNMANAGED LEGACY FIAT WORLD         │
                  │   - Evictions, tax liens, police code raids      │
                  │   - Utility shutoffs, EPA fines, market shocks   │
                  └─────────────────────────┬────────────────────────┘
                                            │
                                            ▼  Stochastic Pressure Vectors
                  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                  ░░░░░░░░░░░ STOCHASTIC BOUNDARY INTERFACE ░░░░░░░░░░
                  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                            │
                                            ▼  Filtered & Attenuated Stress
                  ┌──────────────────────────────────────────────────┐
                  │           SOVEREIGN OASIS SANCTUARY              │
                  │   - Regenerated living soil & micro-grid exergy  │
                  │   - Integrated, healed citizens & low stress     │
                  │   - Physical P2P Proximity Mesh to Peer Nodes    │
                  └──────────────────────────────────────────────────┘
```

#### Inhomogeneous Poisson Arrival Process
Hostile legacy events arrive according to an inhomogeneous Poisson process with time-varying intensity $\lambda(t)$:

$$\lambda(t) = \max\left(\lambda_{\min}, \lambda_{\mathcal{R}} \cdot \exp(-\Psi(t))\right)$$

Where:
* $\lambda_{\mathcal{R}}$ is the baseline arrival intensity governed by the current geopolitical regime $\mathcal{R}(t) \in \{\text{Sub-Radar}, \text{Under-Scrutiny}, \text{Adversarial}, \text{Active-Siege}\}$.
* $\lambda_{\min} = 0.001\text{ s}^{-1}$ is the **irreducible legacy threat floor** (approx. 1 event per 16.6 minutes), ensuring that gameplay tension and off-grid maintenance requirements are never completely extinguished.
* $\Psi(t)$ is the **Sovereign Attenuation Potential** driven by the three attenuation pillars.

### 5.3. The Three Attenuation Pillars
The boundary pressure weakens deterministically through three vectors:

$$\Psi(t) = \alpha \cdot \Phi_{\text{infrastructure}}(t) + \beta \cdot \Phi_{\text{social}}(t) + \gamma \cdot \tanh\left(\delta \cdot \Phi_{\text{mesh}}(t)\right)$$

1. **Physical Self-Sufficiency ($\Phi_{\text{infrastructure}}$) & Genesis Epsilon Guard:**
   $$\Phi_{\text{infrastructure}}(t) = \frac{1}{3} \left[ \min\left(1.0, \frac{E_{\text{solar\_stored}} + \epsilon}{E_{\text{consumed}} + \epsilon}\right) + \min\left(1.0, \frac{W_{\text{rain\_stored}} + \epsilon}{W_{\text{consumed}} + \epsilon}\right) + \frac{A_{\text{depaved\_loam}}}{A_{\text{total\_lot}}} \right]$$
   At Day 1 simulation start (Genesis), physical consumption is initially zero ($E_{\text{consumed}} = 0$, $W_{\text{consumed}} = 0$). To prevent catastrophic $0/0 \implies \text{NaN}$ floating-point singularities in the Poisson generator, all ratio evaluations incorporate the **Genesis Autarky Epsilon Guard** ($\epsilon = 0.001\text{ kWh/L}$):
   $$\eta = \frac{E_{\text{self}} + \epsilon}{E_{\text{legacy}} + E_{\text{self}} + \epsilon}$$
   When solar and rainwater fully cover consumption and asphalt is depaved into living loam, utility shutoff threats become physically inert.
2. **Social NPC Integration ($\Phi_{\text{social}}$):**
   $$\Phi_{\text{social}}(t) = \ln\left(1 + \sum_{k=1}^{N_{\text{agents}}} \frac{\text{Trust}_k \cdot \text{MutualAidHours}_k}{100}\right)$$
   Welcoming wandering refugees, feeding them, and integrating them into Trust Ring 1 transforms them into vigilant community defenders who patrol the perimeter.
3. **Peer Proximity Mesh Ties ($\Phi_{\text{mesh}}$) & Bounded Weighting:**
   $$\Phi_{\text{mesh}}(t) = \frac{1}{|\mathcal{P}|_{\max}} \sum_{j \in \mathcal{P}} \frac{\text{TrustScore}_{ij}}{1 + \left(\frac{d_{ij}}{d_0}\right)^2}$$
   Where $\mathcal{P}$ is the set of peer sovereign nodes connected via Reticulum radio, $d_{ij}$ is the geodesic distance (km), and $d_0 = 1.0\text{ km}$ is the characteristic coupling distance.
   * **Dimensionless Bounded Formulation:** Rather than allowing unbounded summation to produce values $> 400$ (which causes IEEE-754 single-precision float underflow of $\exp(-\Psi)$ to exact $0.0$), the mesh pillar enters the attenuation equation through a bounded hyperbolic tangent function $\tanh(\delta \cdot \Phi_{\text{mesh}}) \in [0, 1.0)$. Connecting to multiple mesh peers dramatically buffers the parcel without eliminating external gameplay tension.

### 5.4. Sovereign Stack Exclusive Multiplayer (Zero Custom P2P)
**Oasis contains zero custom P2P networking protocols, zero WebRTC socket spaghetti, and zero blockchain logic.** All multiplayer coordination routes strictly through the Sovereign Stack via the UHAI IPC boundary:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                D4 MULTIPLAYER LAYER ALLOCATION MATRIX                            │
├───────┬──────────────────────────────┬───────────────────────────────────────────────────────────┤
│ LAYER │ SOVEREIGN STACK SUBSYSTEM    │ MULTIPLAYER FUNCTIONALITY & PROTOCOL SPECIFICATION        │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L2**│ Physical/Mesh Radio &        │ Reticulum Network Stack (RNS), LoRa (SX1262), 802.11s     │
│       │ Delay-Tolerant Transport     │ Wi-Fi Mesh; Cryptographic Distance Bounding PHY           │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L3**│ Distributed State Ledger &   │ Kademlia DHT, Sphinx Onion Routing, libp2p Gossipsub;     │
│       │ Encrypted Gossip Pub/Sub     │ Automerge/Yrs CRDT state replication, IPLD blockstore     │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L4**│ Identity, Credentials &      │ W3C DID Core (`did:key`, `did:peer`), Ed25519 Keypairs;   │
│       │ Collaborative Orchestration  │ Verifiable Credentials (VCs); Metered WASM BPMN 2.0 VM    │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L5**│ Mutual Credit, Barter &      │ Zero-Sum Mutual Credit Clearing (Hawala/Ripple loops);     │
│       │ Smart Covenants              │ Bilateral/Multilateral Smart Covenants; Two-Chamber WoT   │
├───────┼──────────────────────────────┼───────────────────────────────────────────────────────────┤
│ **L7**│ Adversarial Municipal        │ Discrete Event Simulation (`col-adversaryd`); Municipal   │
│       │ Emulation & Boundary Drivers │ Code Enforcement, Zoning Raids, Utility Monopoly Strikes │
└───────┴──────────────────────────────┴───────────────────────────────────────────────────────────┘
```

### 5.5. Cryptographic Geo-Proximity & Boundary Consolidation
Traditional games use easily-spoofed GPS coordinates. The Sovereign Stack enforces **cryptographically unforgeable physical distance bounding**:

$$d_{AB} \le \frac{c \cdot (\Delta t - t_d)}{2}$$

Where $c \approx 3 \times 10^8\text{ m/s}$ (speed of light), $\Delta t$ is round-trip challenge-response time, and $t_d \le 1.0\text{ ns}$ is hardware turnaround time.

#### Isoperimetric Perimeter Consolidation
When two neighboring nodes establish verified proximity ties:
1. **Perimeter Reduction:** Two independent squares of area $A$ have a perimeter of $8\sqrt{A}$. When merged along an edge, their shared internal border is eliminated, reducing the exposed hostile perimeter to $6\sqrt{A}$—an immediate **$25\%$ reduction in hostile surface area**.
2. **Bounded Boundary Friction Coefficient ($\kappa$):**
   $$\kappa = \kappa_{\text{base}} \cdot \prod_{j \in \text{Peers}} \max\left(0.0, 1 - \alpha \cdot \frac{R_j}{\max(d_j, d_{\min})}\right) \cdot (1 - \beta \cdot P_{\text{buffer}}) \cdot \left(1 - \tanh\left(\gamma \cdot N_{\text{citizens}}\right)\right)$$
   Strictly bounded to:
   $$\kappa \in [0.05, 1.0]$$
   The bounded term $\left(1 - \tanh(\gamma \cdot N_{\text{citizens}})\right)$ prevents the critical sign inversion bug where community populations exceeding $50$ citizens ($1/\gamma$) produced negative friction coefficients ($\kappa < 0$). As $\kappa$ drops smoothly toward $0.05$, the stochastic arrival rate of municipal raids collapses by $95\%$, and the volumetric Fog of Legacy dissolves along the shared property line, creating an open **Commons Corridor**.

---

## 6. Core Gameplay Loops & Systems Progression

### 6.1. The Daily Rhythm
The core gameplay loop operates across a structured diurnal cycle:

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                   THE DAILY SIMULATION RHYTHM                                    │
├─────────────────────┬────────────────────────────────────────────────────────────────────────────┤
│ Time Window         │ Gameplay Phase & Systems Activity                                          │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **06:00 — Dawn**    │ **The Morning Circle:** Bell rings; Founder 01 convenes community. Open    │
│                     │ BPMN WorkTokens are posted on the slate; citizens claim tasks.             │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **07:00 — 12:00**   │ **Morning Production Sprint:** High focus period. Heavy physical labor     │
│                     │ (asphalt quarrying, solar mounting, ditch excavation, keyline trenching).  │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **12:00 — Midday**  │ **Communal Nourishment:** Shared lunch in the mess tent. Thermal resting.  │
│                     │ Need satiation; social thought exchange; hydration replenishment.          │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **13:00 — 17:00**   │ **Afternoon Maintenance & Craft:** Precision crafting (circuit wiring,     │
│                     │ seed sorting, biochar inoculation, tool sharpening, whim execution).       │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **17:00 — 20:00**   │ **Evening Governance & Social Bonding:** Dinner around the rocket heater;  │
│                     │ storytelling, acoustic music, reviewing boundary threat reports.           │
├─────────────────────┼────────────────────────────────────────────────────────────────────────────┤
│ **20:00 — 06:00**   │ **Night Rest & Memory Consolidation:** Sleep cycle. Diurnal memory decay;  │
│                     │ episodic ring buffer consolidation into core beliefs; stress damping.      │
└─────────────────────┴────────────────────────────────────────────────────────────────────────────┘
```

### 6.2. Seasonal Progression & Environmental Pressures
* **Spring (The Mud & Thaw):** Heavy rain tests bioswales and stormwater catchments; high risk of mudslides; rapid seedling germination; legacy code enforcement begins spring property inspections.
* **Summer (The Heat & Drought):** Solar exergy at maximum ($> 8.0\text{ kWh/m}^2/\text{day}$); high evaporation demands water conservation; extreme risk of grass fires and thermal transformer blowouts.
* **Autumn (The Harvest & Scavenging):** Intensive canning, drying, and root cellar storage; final push to scavenge structural timber and rebar before snow.
* **Winter (The Deep Freeze & Survival):** Low solar yield; high heating exergy demand; risk of pipe bursts and frostbite; testing the community’s social solidarity against altruism fatigue.

---

## 7. HUD, UI/UX Design & Telemetry

### 7.1. Dual Viewport System
Oasis incorporates a seamless dual camera system:

```
┌───────────────────────────────────┐         ┌───────────────────────────────────┐
│     DIRECT 3RD-PERSON VIEW        │         │   CADASTRAL AXONOMETRIC SLATE     │
├───────────────────────────────────┤         ├───────────────────────────────────┤
│ - Over-the-shoulder perspective   │  [TAB]  │ - Orthographic blueprint view     │
│ - Tactile kinematics & tool swing │ ◄═════► │ - Thermodynamic & water overlays  │
│ - Intimate character emotionality │ (400ms) │ - Boundary threat vectors         │
│ - Immersive lighting & weather    │         │ - Architectural planning & queuing│
└───────────────────────────────────┘         └───────────────────────────────────┘
```
Transitions between views execute smoothly over a 400ms Hermite curve, preserving player spatial orientation.

### 7.2. The Volumetric Plumbob 2.0
Hovering directly over Founder 01’s head is a 3D volumetric faceted gemstone—**The Plumbob 2.0**:
* **Luminescence & Color Mapping:**
  * *Calm Emerald Pulse (0.5 Hz):* Grounded, high focus ($\ge 180$), low stress ($< 20,000$).
  * *Warm Amber Flicker (1.5 Hz):* Strained focus ($50 \dots 100$), moderate stress, physiological fatigue.
  * *Jagged Crimson Strobing (4.0 Hz):* Acute breakdown, tantrum, melancholy, or moral defiance.
  * *Prismatic Iridescent Shimmer:* Strange Mood / Creative Burst in progress.

### 7.3. The Steward's Slate & Natural-Language Cognitive Dossier
Selecting any character opens their **Cognitive Dossier**, presented not as a spreadsheet of numbers, but as an intimate, empathetic narrative:
> *"Founder 01 feels deeply unsettled. She is haunted by the memory of the municipal code raid at the south gate last Autumn. She felt quiet pride while laying cob around the rocket stove yesterday. She values ecological harmony above all else and is frustrated by Adam's reluctance to share salvaged copper wire. Her thoughts are scattered."*

### 7.4. The Volumetric Fog of Legacy
Outside the player’s active parcel boundary, the world is shrouded in the **Fog of Legacy**—a thick, volumetric atmospheric smog illuminated by distant flashing emergency sirens, surveillance drones, and flickering sodium-vapor streetlights. As the player regenerates soil and establishes peer proximity ties, the fog physically recedes, revealing clean skies and verdant commons corridors.

---

## 8. Audio & Narrative Themes

### 8.1. Acoustic Landscape & Somatic Cues
* **The Living Hearth vs. The Dying Machine:** Inside Lot 402, soundscapes feature the gentle bubbling of bio-swales, the crackle of seasoned oak in rocket stoves, acoustic guitars, and rhythmic hand sawing. Outside the perimeter hums the oppressive, industrial drone of 60 Hz high-voltage transformers, diesel engines, and police sirens.
* **Somatic Audio Feedback:** Fatigued characters breathe heavily; high-stress agents mutter under their breath or slam tools with excessive force; harmonious community meals produce warm, resonant vocal murmurs.

### 8.2. Thematic Narrative Tone
Oasis is serious, grounded, and defiantly optimistic. It avoids both cynical post-apocalyptic despair and naive utopian fantasy. It honors the dignity of physical labor, the reality of human psychological friction, and the transformative power of organized mutual aid.
