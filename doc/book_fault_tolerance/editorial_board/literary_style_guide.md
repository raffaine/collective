# Literary Style & Prose Guide for *Fault Tolerance*

**Purpose:** This document is a mandatory style and craft guide for any writer or agent drafting, editing, or revising chapters of *Fault Tolerance*. Every edit must adhere to these principles to prevent regression into AI-isms, melodrama, didactic lecturing, or repetitive physical tropes.

---

## 1. Show Through Perception and Action—Never Editorialize

The single biggest failure mode in earlier drafts is **telling the reader how to feel** or explaining the moral/political lesson of a scene after the fact.

### Rules:
1. **Cut the Moral Summary Paragraph:** Never end a scene or a piece of dialogue with a narrator summary explaining why the Collective's way is superior to the legacy world (e.g., *"It was a world wholly alive, stripped of corporate greed,"* or *"This was why they built the valley"*). Let the physical action or a character's quiet observation end the beat.
2. **Anchor Description in Immediate Perception:** Do not describe rooms, machines, or landscapes like an omniscient brochure. Describe only what the POV character notices *because of what they are doing right now*.
   - *Bad (Over-described & Editorialized):* "The clinic was a low, welcoming building of rammed earth and warm cedar beams, free of the cold, sterile fluorescent tubes of legacy hospitals, designed to keep the birthing experience wholly human."
   - *Good (Grounded in Perception & Action):* "Gabe pushed through the cedar doors, his boots tracking dark compost across the swept stone floor. Down the hall, a kettle was whistling on the woodstove, and the air smelled of boiled linen and sage."
3. **Underplay Emotional Highs:** When a scene is already tragic, violent, or joyful, **pull the prose back**. The higher the stakes of the event, the simpler and more restrained the sentences should be. Trust the event itself to move the reader.
   - Avoid: *"Her breath caught in her throat," "His heart slammed against his ribs," "Tears cut clean tracks through the soot on his cheeks," "A fierce, unyielding smile touched her lips."*
   - Instead, show small, specific, involuntary human behaviors: a character looking at their boots, fumbling a wrench twice, wiping their nose on a sleeve, or changing the subject because they can't trust their voice.

---

## 2. Hard Ban on Overused Physical Clichés ("The Calloused Hands Rule")

Earlier drafts relied obsessively on a handful of physical descriptors to signal "gritty working-class authenticity." These phrases have become unintentional parody and are **strictly rationed or banned** in all new and revised text:

| Overused Trope / Word | Status / Replacement Rule |
| :--- | :--- |
| **`calloused` / `scarred` (hands, palms, fingers)** | **BANNED** in new text. Do not describe anyone's hands as calloused, scarred, or grease-stained. Show what their hands are *doing* (e.g., "he pinched the hot wire without flinching," "she scraped dried clay from under her thumbnail"). |
| **`rumbled` / `gravelly rumble`** | **BANNED** as a dialogue tag for Silas or Jace. Let Silas simply *say* things, or show his actions between lines of dialogue. |
| **`broad shoulders` / `wall of granite` / `solid as oak`** | **BANNED** for Silas. Stop treating Silas like a piece of load-bearing masonry. Let his competence, hesitation, fatigue, or dry humor define him. |
| **`knuckles whitened` / `jaw set hard`** | **BANNED.** Find specific, non-cliché ways to show tension. |
| **`breath caught` / `let out a breath they didn't know they were holding`** | **BANNED.** Classic melodrama tic. |
| **`loam` / `rich dark earth`** | **RATIONED.** Use ordinary words (`dirt`, `mud`, `clay`, `topsoil`, `crust`) unless a farmer is specifically inspecting soil composition. |
| **`fierce` / `unyielding` / `solemn nod`** | **BANNED.** Characters in real life rarely exchange "solemn nods" or "fierce smiles." |

---

## 3. Punctuation & Syntax: The Em-Dash Ban

AI-generated prose relies heavily on em-dashes (`—` or LaTeX `---`) to tack explanatory clauses, lists, or dramatic punchlines onto the ends of sentences.

### Rules:
1. **Zero Narrative Em-Dashes:** Never use `—` or `---` in narrative prose, internal monologue, or parenthetical asides.
   - *Instead of an em-dash before a list:* Use a colon (`:`) or integrate the list with "such as," "from ... to ...," or "including."
   - *Instead of parenthetical em-dashes:* Use commas, or rewrite the sentence so the thought flows naturally without interruption.
   - *Instead of a dramatic em-dash at the end of a sentence:* Use a period, a semicolon, or a conjunction.
2. **Only Exception (Interrupted Dialogue):** An em-dash (`—`) is permitted **only** inside quotation marks when a speaker is physically cut off mid-sentence or breaks off in shock (e.g., `"I told you, I have my ID—"`).

---

## 4. Anti-AI-Ism & Anti-Jargon Rules

1. **No "Not X, But Y" Rhetorical Contrasts:** Avoid the repetitive AI sentence structure *"It wasn't X; it was Y"* or *"Not out of anger, but out of..."* (e.g., *"not a cry of helplessness, but of sheer strength,"* *"not a teacher checking on a student, but equal to equal"*). State what a thing *is* directly.
2. **No Technical / Architectural Jargon in Prose:**
   - Do **not** use Volume 1 textbook terms in narrative prose or dialogue (`Layer 1`–`Layer 7`, `Ricardian contract`, `CRDT`, `zero-knowledge proof`, `Sybil attack`, `exergy`, `thermodynamic floor`, `Web of Trust`, `Soulbound Token`, `Scenario Alpha/Chi`).
   - Do **not** use underscores (`_`) or explicit Greek letters in prose.
   - Translate systemic concepts into plain, lived human terms: *county courts, water boards, shared ledgers, neighborhood kitchens, trust vouchers, boundary treaties, radio relays.*
3. **Dialogue Must Sound Like Spoken Speech, Not a Manifesto:**
   - Characters should never speechify about capitalism, courts, or ecology in multi-sentence paragraphs unless they are literally addressing a formal assembly—and even then, people interrupt, argue, mumble, and use shorthand.
   - People who work together every day speak in fragments, inside jokes, practical complaints ("That bearing's running hot again"), and evasions when they are hurt or tired.

---

## 5. Emotional Realism & Moral Complexity

1. **Let People Be Tired, Petty, and Wrong:** Even in a cooperative society, human beings get irritable when they haven't slept, hold grudges when someone hurts their friend, and disagree without coming to a warm group hug by sunset.
2. **No Instant Therapy:** Trauma, addiction, grief, and political betrayal do not resolve neatly in a single conversation. Leave rough edges, awkward silences, and unresolved disagreements on the page.
3. **Format Constraints (LaTeX):**
   - Always use `\textit{...}` for italics and `\textbf{...}` for bold; never use Markdown `*...*` or `**...**` inside `.tex` files.
   - Check quotation marks in `.tex` files for consistency (`"` or `` ``...'' ``).
