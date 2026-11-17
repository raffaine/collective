# Episode 01 Show-Runner & Audio Generation Cues

* **Episode:** `Ep. 01 — Pry-Bar at 2:38 AM: Why the Sovereign Stack Exists`
* **Companion Source File:** [ep01_source_pack.md](./ep01_source_pack.md)

---

## Option A: NotebookLM One-Shot Audio Overview Prompt (15–20 Min Two-Host Episode)

### Step-by-Step Instructions:
1. Open [NotebookLM](https://notebooklm.google.com/), create a new notebook, and upload **[ep01_source_pack.md](./ep01_source_pack.md)** as the sole source.
2. In the **Audio Overview** box, click **Customize** and paste the Director Prompt below:

```text
You are hosting Episode 1 of "Radio Lambda," an engaging, grounded, and warm podcast exploring The Collective and The Sovereign Stack.

HOST ROLES:
- Host A is the Grounded Neighbor & Pragmatic Skeptic: curious, witty, tired of modern rent/grocery/subscription traps, but allergic to cultish commune clichés, endless committee meetings, and tech-bro jargon.
- Host B is the Systems Steward & Storyteller: warm, vivid, and crystal clear—explaining every architectural layer through physical objects and human stories rather than dry code.

EPISODE STRUCTURE & MUST-HIT BEATS:
1. COLD OPEN — THE TWO ORIGIN STORIES (System Call: Genesis):
   - Open with Elara's impossible choice on the 12th floor of City Utility Enforcement and the chilling moment her boss remotely locks the smart-metered nozzle on her 8-year-old son Gabe's asthma inhaler during a sulfur inversion.
   - Contrast that corporate cruelty with the second trap: Jax's well-meaning computer ration ledger at the abandoned textile mill, which suffered a 2:38 AM priority-inversion bug during a 14°F sleet storm and shut off the upstairs toddlers' nursery heat pump to protect the database servers—until carpenter Jonas ripped the server cabinet open with a steel pry-bar and Kael threw the ration scripts into a burn barrel.
2. THE 7-LAYER SOVEREIGN STACK & WHY WE USE "bp" (BASIS POINTS):
   - Walk listeners through the 7 layers from soil and copper (Layer 1) up to the protective legal carapace (Layer 7).
   - Have Host A ask: "Wait, why do all their specs say 10,000 bp or 8,000 bp instead of percentages?" Have Host B explain basis points (10,000 bp = 100%) and why banning floating-point decimals prevents computer chips from drifting out of sync.
3. WHAT LAYER 6 FEELS LIKE (THE CONSTELLATION & ZERO-SURVEILLANCE):
   - Contrast Silicon Valley smartphone "super-apps" with Layer 6's calm Constellation: washable E-Ink soup slates, zero-knowledge peanut-allergy lunch tiffins, the physical wooden-and-brass bedroom Sanctuary Latch, the brass microphone kill-switch in the music parlor, and how Scenario Nu wipes your body measurements from volatile RAM the second your custom shirt or boots are cut ("No, Julio isn't taking your shirt!").
4. GOVERNANCE ACROSS SCALES (N=1 to Ns>1, Np>1) & LAYER 7 REACH:
   - Explain how governance scales from a solo homesteader (N=1, zero meetings!) to one cook/maker serving 10 households (protected by pull-only queues and the 40% Anti-Martyrdom circuit breaker), up to a multi-guild watershed using Demand-Capture Growth Beacons.
   - Close with the two ways a node reaches into Layer 7 (extractive/transitional up to the $0 monthly bill shutoff vs. purely defensive legal carapaces and Mundane Semantic Camouflage) on the road to Scenario Omega's Planetary Lighthouse.
```

---

## Option B: Gemini Carbon 3-MinuteLyrical Song Prompts ("Radio Lambda" Bumpers)

Use these prompts in **Gemini Carbon** to generate 3-minute lyrical tracks that bookend Episode 01 (or play as an interlude between acts).

### Song 1 (Opening / Mid-Episode Track): *"Pry-Bar at Two Thirty-Eight"*
* **Style / Sonic Direction Prompt:**
  `Warm indie-folk and atmospheric post-rock recorded live in a timber-frame barn, fingerpicked acoustic guitar, upright bass, brushed snare, cello swell, and rain on a tin roof; earnest male and female duet vocals building to a defiant, joyful polyphonic chorus, 96 BPM, 3 minutes.`
* **Lyrics:**
  ```text
  [Verse 1]
  Twelfth-floor glass and a grey plastic mouse
  Clicking the heat off a stranger's house
  Ninety-four percent on the quota screen
  To keep the boy breathing through a winter mean
  Then the sulfur fell on Fourth Avenue rain
  And a packet from the cloud locked the valve in his hand
  You can own the server, you can own the street
  But you don't own the breath where the neighbors meet

  [Chorus]
  Throw the ledger in the barrel, let the pine-scrap burn
  Every locked-up ration has a page to turn
  Ten thousand basis points of bread and heat
  No price on the soup, no lien on the seat
  Put a pry-bar to the door at two thirty-eight
  The children come first, let the database wait!

  [Verse 2]
  Fourteen degrees in the foothill mill
  The algorithm counted who had paid the bill
  Kept the silicon warm in the basement rack
  While the frost crept over the nursery crack
  Jonas didn't ask for a committee vote
  Took three feet of steel to the cabinet throat
  And Elara knelt down in the icy mud
  Splicing heavy copper like a living blood

  [Chorus]
  Throw the ledger in the barrel, let the pine-scrap burn
  Every locked-up ration has a page to turn
  Ten thousand basis points of bread and heat
  No price on the soup, no lien on the seat
  Put a pry-bar to the door at two thirty-eight
  The children come first, let the database wait!

  [Bridge]
  Slide the brass latch on the bedroom door
  No blinking screen on the kitchen floor
  Two cards together on the workshop bay
  And a lighthouse shining where the rivers play

  [Outro]
  Let the pine-scrap burn...
  Ten thousand basis points of grace
  When the interface knows how to be quiet
  And leave us the space.
  ```

---

## Option C: Three 3-Minute Spoken-Word / Radio Acts for Direct Gemini Carbon Generation

If you want to generate the spoken-word radio show directly inside **Gemini Carbon** in 3-minute sessions, run these three 3-minute prompts in sequence:

1. **Session 1 (3:00) — Act I: The Locked Inhaler & The 2:38 AM Nursery Bug:**
   * *Prompt:* `3-minute late-night public radio storytelling duet over subtle warm acoustic guitar and rain ambiance. Two expressive hosts tell the twin origin stories from System Call: Genesis—how Elara crushed the cloud-locked asthma inhaler to save her son Gabe, and how carpenter Jonas used a steel pry-bar at 2:38 AM when Jax's ration computer shut off the toddlers' nursery heat pump to save database servers, birthing the law of Unconditional Survival (10,000 basis points).`
2. **Session 2 (3:00) — Act II: The 7 Layers, Basis Points & The Calm Constellation:**
   * *Prompt:* `3-minute lively two-host radio dialogue with subtle lo-fi warm marimba underscore. Host A asks why The Sovereign Stack uses "bp" (basis points, 10,000 bp = 100%) instead of decimals, and Host B explains how banning floating-point numbers stops computer drift across 20-year-old laptops. They tour Layer 6's Constellation of physical interfaces: washable E-Ink soup slates, zero-knowledge allergy lunchboxes, wooden bedroom rest latches, and wiping body measurements from RAM the second a custom coat is sewn.`
3. **Session 3 (3:00) — Act III: From Solo Garage (N=1) to the Planetary Lighthouse (Omega):**
   * *Prompt:* `3-minute uplifting two-host radio finale building to an acoustic guitar and cello outro. The hosts explain how governance scales from one person in a suburban garage (N=1, zero meetings!) to a neighborhood pod protected by the 40% Anti-Martyrdom circuit breaker, up to a federated watershed using Demand-Capture Growth Beacons, defensive Community Land Trusts, and Scenario Omega's Planetary Lighthouse shipping Genesis Seed Kits to the world.`
