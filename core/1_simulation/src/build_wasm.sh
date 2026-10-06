#!/bin/bash
set -e

echo "=== Building Oasis Engine for WebAssembly ==="

if ! command -v emcc &> /dev/null; then
    echo "emcc not found. Downloading emsdk locally to ./emsdk..."
    if [ ! -d "emsdk" ]; then
        git clone https://github.com/emscripten-core/emsdk.git
    fi
    cd emsdk
    ./emsdk install latest
    ./emsdk activate latest
    source ./emsdk_env.sh
    cd ..
else
    echo "Found emcc: $(emcc --version | head -n 1)"
fi

if [ ! -d "vcpkg" ]; then
    echo "vcpkg not found. Cloning vcpkg locally..."
    git clone https://github.com/microsoft/vcpkg.git
    ./vcpkg/bootstrap-vcpkg.sh
fi

mkdir -p build_wasm
cd build_wasm

rm -f CMakeCache.txt

echo "Running CMake with vcpkg chainloaded to Emscripten..."
# Use absolute paths to prevent CMake include errors
VCPKG_TOOLCHAIN="$(pwd)/../vcpkg/scripts/buildsystems/vcpkg.cmake"
EM_TOOLCHAIN="$(pwd)/../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"

cmake .. \
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_TOOLCHAIN}" \
    -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="${EM_TOOLCHAIN}" \
    -DVCPKG_TARGET_TRIPLET=wasm32-emscripten

echo "Compiling via emmake..."
emmake make -j4

echo "=== Build Complete ==="
echo "You can test the WASM build by running a local server:"
echo "python3 -m http.server 8080 --directory ."
