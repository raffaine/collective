# Digital Twin Architecture Spec (Layer 2)

## 1. The Reality-Agnostic Abstraction
The core philosophical mandate of Layer 2 is **Reality Agnosticism**. 
The C++ Simulation Engine (Layer 1b) must never know if it is operating in the physical world or inside a test suite. It only knows that it receives a normalized data stream and outputs actuation commands. 

Layer 2 acts as the universal translator between the raw chaos of Physical Reality (Layer 1) and the strict thermodynamic math of the Engine (Layer 1b).

## 2. Core Responsibilities

*   **Ingestion (Telemetry):** Listening to physical hardware via MQTT, HTTP Webhooks, or LoRaWAN, and translating it into a standardized semantic `TwinState` object.
*   **Virtualization (Mocking):** Generating simulated telemetry for the engine to consume during testing, or when predicting future states (e.g., "If I turn on the forge, what happens to the power grid?").
*   **Actuation (Command Routing):** Taking an actuation command from the engine (e.g., "Halt 3D Printer") and routing it to the physical hardware's API (or logging it to a test console if virtual).

## 3. The Protocol Interfaces

Layer 2 must establish strict interface contracts for hardware integration. Every machine (whether an Open Source Centrifuge, a Solar Inverter, or an Eldercare Fall-Sensor) must adhere to a standard API structure:

1.  **`ReadState()`**: Returns the physical exergy, temperature, and material status.
2.  **`ApplyCommand()`**: Executes a state change.
3.  **`Heartbeat()`**: Cryptographic proof-of-liveness (Thermodynamic Anti-Cheat).

## 4. Directive for Protocol Agents
When an AI Agent is tasked with building a hardware integration (e.g., integrating a physical Bambu Lab printer), they must NOT write code directly into Layer 1b. They must write a Python/Go/JS multiplexer script in Layer 2 that translates the Bambu Lab proprietary protocol into a standard `TelemetryStream` JSON-LD payload.
