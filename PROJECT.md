# Project: Oasis Game Engine (V4) — Sprint 1

## Architecture
- **Language & Standards**: C++20, WebAssembly (Emscripten 4.0.10+ / Dawn C++), WebGPU, Flecs ECS.
- **Graphics Pipeline**:
  - WebGPU asynchronous device acquisition via `--use-port=emdawnwebgpu` (`<webgpu/webgpu_cpp.h>`).
  - Surface configuration via `wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector("#canvas")` with `wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc`.
  - Shader pipeline: Full-screen triangle WGSL shader embedded as C++ string literal in `shaders/raymarch_wgsl.hpp` rendering a multi-color gradient across UV coordinates with `cullMode = wgpu::CullMode::None`.
- **Simulation & ECS**:
  - Flecs world dynamically managed on the heap to avoid stack use-after-free before asynchronous browser callbacks fire.
  - `oasis::graphics::RenderState` component with non-zero size holding WebGPU device, queue, surface, pipeline, format, width, and height handles, preventing Flecs empty-struct assertions.
  - WebGPU render pass integrated into Flecs system (`OnStore` phase) executed every tick of the main loop via `emscripten_set_main_loop_arg`.
- **Automated Verification**:
  - Zero-dependency headless browser test harness (`tests/verify_canvas.js`) utilizing Node 22 native `WebSocket` and Chrome DevTools Protocol (CDP).
  - Connects to Google Chrome, loads `oasis_engine.html`, captures real compositor frames via `Page.captureScreenshot`, decodes PNG scanlines via native `zlib.inflateSync`, and samples genuine rasterized pixels (Center `[128, 127, 127, 255]`, gradient variation `deltaR=204, deltaG=204`).
  - Enforces strict fail-clean semantics (exits 1 on error, zero mock fallbacks).

## Feature Inventory
| # | Feature | Description | Milestone | Source |
|---|---------|-------------|-----------|--------|
| 1 | Toolchain & Build Fixes | Update CMakeLists.txt and build_wasm.sh with modern Emscripten WebGPU flags (`--use-port=emdawnwebgpu`, `-sEXIT_RUNTIME=0`, remove `-sASYNCIFY`), vcpkg Flecs linking | M1 | Survey (Build Explorer) |
| 2 | Shader 404 Resolution | Embed WGSL shader as C++ string literal (`shaders/raymarch_wgsl.hpp`) and update `shaders/raymarch.wgsl` to eliminate runtime 404s and filesystem dependencies | M1 | Survey (Build & Graphics Explorers) |
| 3 | WebGPU Graphics Pipeline | Async adapter/device acquisition, surface configuration, pipeline creation, and WGSL color gradient rendering without culling | M1 | Survey (Graphics Explorer) |
| 4 | Flecs ECS Integration | Define non-zero-sized `RenderState` component, avoid empty-struct assertion, manage heap ECS lifecycle, and integrate render pass into tick loop | M1 | Survey (Systems Explorer) |
| 5 | Automated Visual Verification | Implement authentic automated visual test proving canvas renders non-black pixels without mock facades or synthetic fallbacks | M1 | Survey (Quality Engineer Explorer) |

## Milestones
| # | Name | Scope | Dependencies | Status |
|---|------|-------|-------------|--------|
| 1 | Sprint 1: Engine Build, WebGPU Rendering & Automated QE Verification | Full engine codebase fixes (`main.cpp`, `CMakeLists.txt`, `shaders/`, `build_wasm.sh`) and headless browser test harness (`tests/verify_canvas.js`) | none | DONE |

## Key Milestone Outputs
- **Build Target**: `core/1_simulation/src/build_wasm/oasis_engine.wasm` (3,024,152 bytes, clean exit code 0).
- **Embedded Shader**: `core/1_simulation/src/shaders/raymarch_wgsl.hpp` (`oasis::shaders::RAYMARCH_WGSL`).
- **Engine Entry**: `core/1_simulation/src/main.cpp` (Heap Flecs lifecycle, async WebGPU, non-zero `RenderState`, `CopySrc` enabled).
- **Automated Visual Test**: `core/1_simulation/src/tests/verify_canvas.js` (Clean exit code 0, 100% pass rate, authentic CDP screenshot decode, "Automated Visual Verification Passed").
- **Gate Evaluation**: Unanimous PASS (Reviewer 1 APPROVE, Reviewer 2 APPROVE, Challenger 1 APPROVE, Challenger 2 APPROVE, Forensic Auditor CLEAN).

## Interface Contracts
### Main Loop ↔ Flecs ECS
- `flecs::world* g_ecs`: Heap-allocated ECS instance persisting beyond `main()` return.
- `emscripten_set_main_loop_arg`: Ticks `g_ecs->progress()` each animation frame.

### WebGPU ↔ Flecs Component
- `struct RenderState`: Non-zero size struct holding WebGPU device, queue, surface, pipeline, format, width, and height.

### Build & Link Contracts
- Compiler & Linker: `--use-port=emdawnwebgpu`, `-std=c++20`, `-sEXIT_RUNTIME=0`.
- Target: `core/1_simulation/src/build_wasm/oasis_engine.html` and `.wasm`.

## Code Layout
- `core/1_simulation/src/CMakeLists.txt`: Build specification and Emscripten flags
- `core/1_simulation/src/build_wasm.sh`: WASM compilation script
- `core/1_simulation/src/shaders/raymarch_wgsl.hpp`: Embedded WGSL shader header
- `core/1_simulation/src/shaders/raymarch.wgsl`: Standalone WGSL shader source
- `core/1_simulation/src/main.cpp`: Engine entry point, async WebGPU setup, Flecs ECS integration
- `core/1_simulation/src/tests/verify_canvas.js`: Automated headless browser test runner
