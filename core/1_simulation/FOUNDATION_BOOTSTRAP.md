# Phase 0: Oasis Engine Foundation Bootstrap

## Mission
You are the **Architect Agent**. Before any ScDD scenarios can be run, you must establish the "Basilar Foundation" of the Oasis engine. We are building a dual-target (Native + WebAssembly) C++20 game engine using **SDL2** as our universal abstraction layer.

## Requirements

### 1. Dual-Target CMake Configuration
Modify `core/1_simulation/src/CMakeLists.txt` to support compiling both natively and via Emscripten (`emcmake cmake ..`).
- It must link against SDL2 (use `find_package(SDL2 REQUIRED)` for native, and `-s USE_SDL=2` for Emscripten).
- Output must be an executable for native builds, and an `.html`/`.js`/`.wasm` package for Emscripten builds.

### 2. The Universal Game Loop
Refactor `core/1_simulation/src/main.cpp` into a standard SDL2 application:
- Initialize SDL2 video.
- Create a window and a WebGL/OpenGL rendering context.
- Implement an event polling loop (catching `SDL_QUIT` or `emscripten_set_main_loop` for the browser).
- Integrate the existing `ChunkManager` and `Tick()` loop into the main game loop so the physics engine actually "ticks" alongside frame rendering.

### 3. The HTML/Browser Payload
Ensure the build outputs a functional `index.html` (either via an Emscripten template or a custom one) containing a `<canvas id="canvas">` element.

### 4. Verification
- Provide instructions on how to compile the native build.
- Provide instructions on how to compile the WASM build (assuming the user has `emsdk` installed) and how to serve it locally.

Once this is complete, the engine will have a window, an event queue, and a render context. Only then can we return to the `TDD_ORCHESTRATOR.md` to begin testing ScDD physical interactions!
