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

echo "=== Build Complete ==="
echo "Artifacts generated in: $(pwd)"
echo "To test locally, run:"
echo "python3 -m http.server 8080 --directory ."
