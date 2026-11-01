#!/bin/bash
set -e

# Resolve script root directory to support execution from any working directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== Building Oasis Engine for WebAssembly ==="

# 1. Fast-path EMSDK environment activation
if ! command -v emcc &> /dev/null; then
    if [ -f "$SCRIPT_DIR/emsdk/emsdk_env.sh" ]; then
        echo "Activating local emsdk from $SCRIPT_DIR/emsdk..."
        source "$SCRIPT_DIR/emsdk/emsdk_env.sh"
    elif [ -d "$HOME/dev/emsdk" ] && [ -f "$HOME/dev/emsdk/emsdk_env.sh" ]; then
        echo "Activating user emsdk from $HOME/dev/emsdk..."
        source "$HOME/dev/emsdk/emsdk_env.sh"
    else
        echo "emsdk not found. Cloning and installing emsdk..."
        if [ ! -d "emsdk" ]; then
            git clone https://github.com/emscripten-core/emsdk.git
        fi
        cd emsdk
        ./emsdk install latest
        ./emsdk activate latest
        source ./emsdk_env.sh
        cd "$SCRIPT_DIR"
    fi
else
    echo "Found emcc in environment: $(emcc --version | head -n 1)"
fi

# 2. Ensure vcpkg is available
if [ ! -d "vcpkg" ]; then
    echo "vcpkg not found. Cloning vcpkg locally..."
    git clone https://github.com/microsoft/vcpkg.git
    ./vcpkg/bootstrap-vcpkg.sh
fi

# 3. Configure and Build
mkdir -p build_wasm
cd build_wasm

rm -f CMakeCache.txt

VCPKG_TOOLCHAIN="$SCRIPT_DIR/vcpkg/scripts/buildsystems/vcpkg.cmake"
EM_TOOLCHAIN="$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"

echo "Running CMake with vcpkg chainloaded to Emscripten..."
emcmake cmake .. \
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_TOOLCHAIN}" \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="${EM_TOOLCHAIN}" \
    -DVCPKG_TARGET_TRIPLET=wasm32-emscripten

echo "Compiling via emmake..."
emmake make -j4

# Stamp cache-busting version on oasis_engine.js + modular web assets and create index.html
CACHE_VER="$(date +%s)"
CACHE_VER="$CACHE_VER" python3 - <<'PY'
import os, pathlib, re
ver = os.environ["CACHE_VER"]
p = pathlib.Path("oasis_engine.html")
txt = p.read_text()
txt = txt.replace('src="oasis_engine.js"', f'src="oasis_engine.js?v={ver}"')
txt = txt.replace('href="web/css/oasis.css"', f'href="web/css/oasis.css?v={ver}"')
txt = re.sub(r'src="(web/js/[^"]+\.js)"', rf'src="\1?v={ver}"', txt)
p.write_text(txt)
pathlib.Path("index.html").write_text(txt)
PY

# Bundle modular CSS/JS, L6 Commons Companion, L4/L5 web tools, and data/ catalogs into build_wasm/
mkdir -p web/css web/js web/companion web/orchestrator web/governance data
cp -Rf "$SCRIPT_DIR/web/css/"* web/css/ 2>/dev/null || true
cp -Rf "$SCRIPT_DIR/web/js/"* web/js/ 2>/dev/null || true
cp -Rf "$SCRIPT_DIR/web/companion/"* web/companion/ 2>/dev/null || true
if [ -d "$SCRIPT_DIR/../data" ]; then
    cp -Rf "$SCRIPT_DIR/../data/"* data/ 2>/dev/null || true
fi
cp -f "$SCRIPT_DIR/../../3_ledger/node_proxies.json" web/companion/ 2>/dev/null || true
cp -f "$SCRIPT_DIR/../../4_orchestrator/web/"* web/orchestrator/ 2>/dev/null || true
cp -f "$SCRIPT_DIR/../../4_orchestrator/workflows/"*.json web/orchestrator/ 2>/dev/null || true
cp -f "$SCRIPT_DIR/../../3_ledger/node_proxies.json" web/orchestrator/ 2>/dev/null || true
cp -f "$SCRIPT_DIR/../../5_governance/trust_gates.json" web/orchestrator/ 2>/dev/null || true
cp -f "$SCRIPT_DIR/../../5_governance/web/"* web/governance/ 2>/dev/null || true
cp -f "$SCRIPT_DIR/../../5_governance/trust_gates.json" web/governance/ 2>/dev/null || true
cp -f "$SCRIPT_DIR/../../4_orchestrator/workflows/"*.json web/governance/ 2>/dev/null || true

# Also keep build_native synchronized if it exists so servers running from either directory serve identical artifacts
if [ -d "$SCRIPT_DIR/build_native" ]; then
    cp -f oasis_engine.html oasis_engine.js oasis_engine.wasm index.html "$SCRIPT_DIR/build_native/" 2>/dev/null || true
    mkdir -p "$SCRIPT_DIR/build_native/web" "$SCRIPT_DIR/build_native/data"
    cp -Rf web/* "$SCRIPT_DIR/build_native/web/" 2>/dev/null || true
    cp -Rf data/* "$SCRIPT_DIR/build_native/data/" 2>/dev/null || true
fi

echo "=== Build Complete ==="
echo "Artifacts generated in: $(pwd)"
echo "To launch locally with zero browser caching, run from $(pwd):"
echo "python3 -m http.server 8080 --bind 127.0.0.1 --directory ."
echo "Then open in Google Chrome: http://localhost:8080/oasis_engine.html"

