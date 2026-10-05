# Legacy Proxy Architecture Spec (Layer 7)

## 1. The Ablative Shield (A Necessary Evil)
Layer 7 is the only layer of the Sovereign Stack that is inherently flawed, centralized, and fiat-based. It is the interface to the legacy nation-state and the fiat banking system. 

The mandate for Layer 7 is **Containment**. Legacy logic (USD conversion rates, tax withholding, legal entity names) is violently toxic to the pure thermodynamic math of Layers 1-6. Legacy logic must never bleed into the core engine. Layer 7 exists purely as an ablative shield to absorb state friction and protect the Trust Ring.

## 2. Core Responsibilities
Layer 7 acts as an automated API wrapper around the Social Purpose Corporation (SPC).
*   **Fiat Liquidity:** Managing webhook integrations with legacy banking APIs (e.g., Stripe, Plaid) to convert incoming USD from external sales into physical hardware purchases.
*   **Tax & Compliance:** Generating required fiat reports (K-1s, 1099s, sales tax) without polluting the Layer 3 thermodynamic ledger.
*   **Legal Arbitration (Scenario Psi):** Auto-generating legally binding arbitration PDFs from the outcomes of internal mesh mediation, ready for enforcement in legacy courts if a bad actor attempts to breach the Node.

## 3. The Containment Boundary
*   **Strict Isolation:** The C++ Simulation Engine (Layer 1b) and the Ledger (Layer 3) must *never* contain variables named `fiat_value`, `usd_price`, or `tax_rate`. 
*   **One-Way Translation:** Layer 7 reads the pure thermodynamic states from Layer 3 (e.g., "Alice generated 500 kWh of exergy") and translates them into fiat logic for external reporting ("Alice's exergy generation equates to a $50 fiat tax deduction under the SPC charter"). The core engine knows nothing about the $50.

## 4. The Terminal Decoupling (Scenario Omega)
Layer 7 is the only module designed to be permanently deleted.
Agents building Layer 7 must implement a `TerminalDecoupling` self-destruct function. When the Orchestrator detects that the Node has achieved complete thermodynamic autonomy (100% internal food, power, and shelter), this function is triggered. It automatically liquidates all remaining fiat, severs all banking API webhooks, and legally dissolves the SPC, permanently deleting Layer 7 from the Node's active memory.
