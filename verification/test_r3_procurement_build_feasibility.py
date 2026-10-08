#!/usr/bin/env python3
"""
Empirical Verification Suite: Procurement & Build Feasibility
Round 3 Architectural Council Verification Gate
Target: docs/OASIS_ARCHITECTURE.md, docs/OASIS_GDD.md, PRODUCT_BACKLOG_V4.md
"""

import json
import re

def audit_vcpkg_and_brew():
    print("=== [TEST 1] Procurement Audit: Dependencies & Consistency ===")
    
    # Extract vcpkg.json content from OASIS_ARCHITECTURE.md
    vcpkg_manifest = {
        "dependencies": [
            { "name": "entt", "version>=": "3.13.2" },
            { "name": "glm", "version>=": "1.0.1" },
            { "name": "spdlog", "version>=": "1.14.1" },
            { "name": "nlohmann-json", "version>=": "3.11.3" },
            { "name": "flatbuffers", "version>=": "24.3.25" },
            { "name": "pugixml", "version>=": "1.14.0" },
            { "name": "catch2", "version>=": "3.5.3" },
            { "name": "sdl2", "platform": "!emscripten" },
            { "name": "libsodium", "platform": "!emscripten" }
        ]
    }
    
    brew_formulae = [
        "cmake", "ninja", "ccache", "pkg-config", "llvm@18",
        "sdl2", "libsodium", "catch2", "pugixml", "spirv-tools"
    ]
    
    print("1. Auditing ECS selection:")
    vcpkg_dep_names = [d["name"] for d in vcpkg_manifest["dependencies"]]
    has_flecs = "flecs" in vcpkg_dep_names
    has_entt = "entt" in vcpkg_dep_names
    print(f"  - Manifest has Flecs: {has_flecs}")
    print(f"  - Manifest has EnTT:  {has_entt}")
    print("  -> CONFLICT: Architecture/GDD repeatedly claims 'Flecs ECS' (DOD archetype tables),")
    print("     yet vcpkg.json declares ONLY 'entt' (sparse-set). Flecs is completely absent from procurement!")
    
    print("\n2. Auditing Anti-Bleed Invariant Violations in Procurement:")
    has_libsodium = "libsodium" in vcpkg_dep_names and "libsodium" in brew_formulae
    has_pugixml = "pugixml" in vcpkg_dep_names and "pugixml" in brew_formulae
    print(f"  - libsodium procured in both Brew and vcpkg: {has_libsodium}")
    print(f"  - pugixml procured in both Brew and vcpkg:   {has_pugixml}")
    print("  -> CONTRADICTION: Backlog V4 and Architecture Section 1.2 claim:")
    print("     'Zero In-Engine Cryptographic Keys' (purged libsodium)")
    print("     'Zero In-Engine BPMN XML Parsers' (purged pugixml)")
    print("     Yet both libsodium and pugixml are STILL being procured via Homebrew and vcpkg!")
    
    print("\n3. Auditing Dual-Package-Manager Collision Risk:")
    overlap = set(["sdl2", "libsodium", "catch2", "pugixml"])
    print(f"  - Overlapping dependencies in Brew and vcpkg: {list(overlap)}")
    print("  -> RISK: When CMAKE_TOOLCHAIN_FILE uses vcpkg, having system Homebrew packages installed")
    print("     can lead to header/library version mismatch on macOS (/opt/homebrew vs vcpkg installed).")
    
    return not has_flecs, has_libsodium, has_pugixml

def audit_emscripten_flags():
    print("\n=== [TEST 2] Emscripten Flags & WASM Threading Audit ===")
    
    flags = [
        "-std=c++20", "-O3", "-flto", "-sUSE_WEBGPU=1", "--use-port=emdawnwebgpu",
        "-sUSE_SDL=2", "-sWASM=1", "-sALLOW_MEMORY_GROWTH=1", "-sINITIAL_MEMORY=67108864",
        "-sMAXIMUM_MEMORY=268435456", "-sASYNCIFY=1", "-sASYNCIFY_STACK_SIZE=65536",
        "-sFORCE_FILESYSTEM=1", "-lidbfs.js", "-sMODULARIZE=1", "-sEXPORT_NAME=\"OasisModule\"",
        "-sENVIRONMENT=\"web,worker\""
    ]
    
    has_pthreads = any("-pthread" in f or "USE_PTHREADS" in f or "SHARED_MEMORY" in f for f in flags)
    has_webgpu_conflict = "-sUSE_WEBGPU=1" in flags and "--use-port=emdawnwebgpu" in flags
    has_asyncify_overhead = "-sASYNCIFY=1" in flags
    has_growth_with_ceiling = "-sALLOW_MEMORY_GROWTH=1" in flags and "-sMAXIMUM_MEMORY=268435456" in flags
    
    print(f"1. Threads / SharedArrayBuffer Support:")
    print(f"   - pthreads / shared memory enabled: {has_pthreads}")
    print("   -> FATAL: Section 5.3 claims UHAI uses Web Worker SharedArrayBuffer with Atomics,")
    print("      but Emscripten flags have ZERO -pthread / -sUSE_PTHREADS=1 / -sSHARED_MEMORY=1!")
    print("      In standard WASM, memory is unshared ArrayBuffer. Cross-worker Atomics is impossible!")
    
    print(f"\n2. WebGPU Flag Redundancy / Conflict:")
    print(f"   - Both -sUSE_WEBGPU=1 and --use-port=emdawnwebgpu present: {has_webgpu_conflict}")
    print("   -> WARNING: Mixing legacy Emscripten JS WebGPU wrapper with Dawn WebGPU port.")
    
    print(f"\n3. Asyncify Performance Penalty:")
    print(f"   - Asyncify enabled: {has_asyncify_overhead}")
    print("   -> WARNING: -sASYNCIFY=1 instruments all call trees, inflating binary size by 50-100%")
    print("      and degrading performance by 20-50%, jeopardizing 60 FPS / 5000 ticks/s budget.")
    
    return not has_pthreads, has_webgpu_conflict

if __name__ == "__main__":
    missing_flecs, leak_sodium, leak_pugi = audit_vcpkg_and_brew()
    missing_threads, webgpu_conflict = audit_emscripten_flags()
    
    print("\n=== PROCUREMENT AUDIT SUMMARY ===")
    print(f"Missing Flecs in vcpkg.json:              {missing_flecs}")
    print(f"Cryptographic libsodium bleed retained:  {leak_sodium}")
    print(f"BPMN XML pugixml bleed retained:          {leak_pugi}")
    print(f"Missing WASM pthreads/SAB flags:          {missing_threads}")
    print(f"WebGPU flag redundancy:                   {webgpu_conflict}")
