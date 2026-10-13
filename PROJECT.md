# Project: Collective Sprint 10 (The Semantic Gateway & Companion App)

## Architecture
The Semantic Gateway (`col-gatewayd`) and Companion App provide the bridge between the user's mobile browser and the internal Sovereign Stack POSIX daemons:
- **Layer 2 (Hardware/IoT Telemetry):** SPSC ring buffer `/uhai_telemetry` and dedicated `/uhai_telemetry_gateway` providing live fixed-point SI telemetry samples.
- **Layer 4 (Deterministic Execution):** Sandboxed 10 Hz Wasmtime VM state, active BPMN 2.0 DAG tokens, fuel gauges, and 5-Tier Priority Queue loads. Strictly read-only diagnostics; zero mutation endpoints.
- **Layer 5 (Governance):** The civic brain. Upward-yielded task gates, Ricardian treaty registries, bilateral mutual credit limits, and Trust Rings.
- **Layer 6 (Semantic Agora & Intent):** Primary human interface. Continuous Double-Auction order book, natural language / JSON-LD semantic intent authoring, chore bounties. Exposes mutating `POST /api/v1/l6/intent`.
- **Layer 7 (Legacy Proxy / Carapace):** Corporate legal shield, utility grid intertie curtailment monitoring.
- **The Intent U-Turn:** Downstream ($L6 \to L5 \to L4$) and Upward Yield ($L4 \to L5 \to L6 \to L7$). No direct $L4 \to L7$ IPC (`l4_l7.sock` prohibited).

Data Flow:
`Web Companion UI (VLAN 30)` $\xleftrightarrow{\text{HTTP/WS}}$ `col-gatewayd (Port 8443)` $\xleftrightarrow{\text{POSIX SHM Ring Buffers}}$ `UHAI Shared Memory (/uhai_telemetry_gateway, /uhai_flow)`

## Feature Inventory
| # | Feature | Description | Milestone | Source |
|---|---------|-------------|-----------|--------|
| 1 | UHAI Ring Buffer L5/L6 Structs | Define `L4ExecutionTelemetry`, `L5GovernanceTelemetry`, `UpwardYieldPacket`, `GovernanceResolutionPacket` in `uhai_ring_buffer.hpp` | M1 | Survey |
| 2 | Dedicated Gateway SHM Channel | Support dedicated ring buffer name (`/uhai_telemetry_gateway`) to prevent multi-consumer SPSC cursor contention | M1 | Survey |
| 3 | col-gatewayd C++ Daemon Scaffold | Scaffold `src/daemons/col-gatewayd/main.cpp` with POSIX signal handling, drift-free loop, and CLI flags (`--port`, `--shm-name`, `--ticks`) | M2 | Survey / Request |
| 4 | col-gatewayd CMake Target | Add `col_gatewayd` executable target in `CMakeLists.txt` with symlink to `col-gatewayd` and compatibility symlinks | M2 | Survey |
| 5 | POSIX HTTP/WebSocket Dispatcher | Implement lightweight HTTP server handling REST routing and JSON serialization | M2 | Survey / Request |
| 6 | GET `/api/v1/layers/summary` | Aggregate multi-layer diagnostic snapshot across L1–L7 | M2 | Survey |
| 7 | GET `/api/v1/l2/telemetry/latest` | Read-only diagnostic retrieval of latest fixed-point SI telemetry from UHAI ring buffer | M2 | Survey / Request |
| 8 | GET `/api/v1/l4/workflows` & `/api/v1/l4/priority-queue` | Read-only BPMN DAG token state and 5-tier queue meters | M2 | Survey / Request |
| 9 | GET `/api/v1/l5/approvals/pending` & `/api/v1/l5/governance/state` | Read-only pending upward-yielded task gates and governance state | M2 | Survey / Request |
| 10 | POST `/api/v1/l6/intent` | Mutating endpoint for Semantic Agora actions, JSON-LD intent compilation, and order placement | M2 | Survey / Request |
| 11 | Diagnostic Layer Mutation Rejection | Strictly reject write attempts (`POST`, `PUT`, `DELETE`) to L1, L2, L3, L4, L7 with HTTP 405 or 403 | M2 | Survey / Invariant 7 |
| 12 | Companion App Directory Scaffolding | Create `core/1_simulation/src/web/companion/` with `index.html`, `style.css`, and `app.js` | M3 | Survey / Request |
| 13 | Mobile-Responsive Layout & Styling | Implement obsidian dark theme (`#080c14`), frosted glassmorphism, layer chroma tokens, responsive viewport | M3 | Survey / Request |
| 14 | Universal Pinned Navigation Dock | 6-layer bottom dock (`L2: HW`, `L3: LEDGER`, `L4: EXEC`, `L5: GOV`, `L6: INTENT`, `L7: PROXY`) | M3 | Survey |
| 15 | Diagnostic Layers "Pop the Hood" Views | Decks for L2, L4, L5, L7 displaying live telemetry, Wasmtime fuel, token DAGs; strictly read-only with zero mutation controls | M3 | Survey / Request |
| 16 | Layer 6 Exclusive Societal Deck | Continuous Double-Auction orderbook, Semantic Intent authoring prompt, chore bounties dispatching to `POST /api/v1/l6/intent` | M3 | Survey / Request |
| 17 | Frontend Gateway Integration & Simulation Mode | Connects to `col-gatewayd` on port 8443 with fallback simulation mode for offline UI testing | M3 | Survey |
| 18 | Catch2 Test Suite `test_col_gatewayd.cpp` | Native test suite testing ring buffer parsing, JSON serialization, diagnostic GETs, and mutating POST | M4 | Survey / Request |
| 19 | Invariant 7 Write Rejection Tests | Verify that `col-gatewayd` rejects illegal write attempts to read-only diagnostic layers with HTTP 405/403 | M4 | Survey / Request |
| 20 | E2E Integration & Deployment Confirmation | Verify all components compile, all Catch2 tests pass, and confirm "Sprint 10 Semantic Gateway Deployed" | M5 | Request |

## Milestones
| # | Name | Scope | Dependencies | Status |
|---|------|-------|-------------|--------|
| M1 | UHAI Ring Buffer L5/L6 State | Update `uhai_ring_buffer.hpp` with L4 execution, L5 governance structs, and dedicated gateway channel | none | DONE |
| M2 | col-gatewayd C++ Daemon & API | Implement `col-gatewayd` daemon, CMake target, HTTP dispatcher, GET diagnostic endpoints, POST `/api/v1/l6/intent`, and HTTP 405/403 rejection | M1 | DONE |
| M3 | Companion App Web Dashboard | Scaffold `core/1_simulation/src/web/companion/` (HTML, CSS, JS), 6-layer dock, "pop the hood" diagnostic views, and L6 societal boundary | none | DONE |
| M4 | Native Catch2 Tests | Implement `core/1_simulation/src/tests/test_col_gatewayd.cpp`, CMake test target `test_col_gatewayd`, CTest integration | M1, M2 | DONE |
| M5 | E2E Verification & Confirmation | Full compilation, all 12+ CTest test suites passing, browser/mock verification, confirm "Sprint 10 Semantic Gateway Deployed" | M1, M2, M3, M4 | DONE |

## Interface Contracts
### `uhai_ring_buffer.hpp` ↔ `col-gatewayd`
- Shared memory names:
  - Telemetry: `/uhai_telemetry_gateway` (and `/uhai_telemetry` fallback)
  - Flow: `/uhai_flow`
- Structures:
  - `TelemetrySample` (32 bytes, 8-byte aligned)
  - `TelemetrySlot` (64 bytes, cacheline aligned)
  - `FlowContract` (64 bytes, 8-byte aligned)
  - `L4ExecutionTelemetry` / `L4WorkflowSlot` (deterministic BPMN state & fuel)
  - `L5GovernanceTelemetry` / `UpwardYieldPacket` (pending civic task gates)
  - `SpatialIntentPayload` (264 bytes)

### `col-gatewayd` ↔ Web Companion App
- Port: Default `8443` (HTTP/1.1 REST, JSON)
- Diagnostic Endpoints (GET only):
  - `GET /api/v1/layers/summary` -> `{ "l2": {...}, "l4": {...}, "l5": {...}, "l6": {...}, "l7": {...} }`
  - `GET /api/v1/l2/telemetry/latest` -> `{ "frame_count": N, "fps": F, "founder_stress": S, ... }`
  - `GET /api/v1/l4/workflows` -> `{ "workflows": [...] }`
  - `GET /api/v1/l5/approvals/pending` -> `{ "pending_approvals": [...] }`
- Semantic Mutating Endpoint (POST only):
  - `POST /api/v1/l6/intent` -> Body: `{ "intent_type": "...", "payload": {...} }` -> `{ "status": "DRAFT_COMPILED", "intent_cid": "...", ... }`
- Boundary Protection:
  - Any `POST`, `PUT`, `DELETE` to `/api/v1/l1/*`, `/api/v1/l2/*`, `/api/v1/l3/*`, `/api/v1/l4/*`, `/api/v1/l7/*` -> Returns `HTTP 405 Method Not Allowed` or `HTTP 403 Forbidden`

## Code Layout
- `core/1_simulation/src/uhai/uhai_ring_buffer.hpp` — UHAI SPSC lock-free ring buffer & packet definitions
- `src/daemons/col-gatewayd/main.cpp` — Primary implementation of `col-gatewayd`
- `core/1_simulation/src/src/daemons/col-gatewayd/` — Symlink / compatibility directory
- `core/1_simulation/src/CMakeLists.txt` — CMake configuration for daemons and tests
- `core/1_simulation/src/web/companion/index.html` — Companion app dashboard markup
- `core/1_simulation/src/web/companion/style.css` — Solarpunk/Cyberpunk responsive styling
- `core/1_simulation/src/web/companion/app.js` — Frontend controller, layer switcher & API client
- `core/1_simulation/src/tests/test_col_gatewayd.cpp` — Catch2 v3 unit and boundary test suite
