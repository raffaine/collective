#!/bin/bash
set -e

echo "=== Building Oasis Engine natively with vcpkg ==="

# 1. Ensure vcpkg is available
if [ ! -d "vcpkg" ]; then
    echo "vcpkg not found. Cloning vcpkg locally..."
    git clone https://github.com/microsoft/vcpkg.git
    ./vcpkg/bootstrap-vcpkg.sh
fi

# 2. Build the project using the vcpkg toolchain (manifest mode)
mkdir -p build_native
cd build_native

echo "Running CMake with vcpkg toolchain..."
# CMAKE_TOOLCHAIN_FILE tells CMake to use vcpkg to resolve dependencies
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake

echo "Compiling natively..."
make -j4

echo "=== Build Complete ==="
echo "Executables built:"
echo "  ./oasis_engine"
echo "  ./col_telemetryd"
echo "  ./test_uhai_ring_buffer"

