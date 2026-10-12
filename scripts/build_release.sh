#!/bin/bash
# build_release.sh - The Collective Alpha Packager
# Usage: ./scripts/build_release.sh

echo "========================================="
echo " 📦 Packaging Collective Alpha Release"
echo "========================================="

RELEASE_DIR="release/collective_alpha"

# 1. Cleanup old releases
rm -rf release/
mkdir -p "$RELEASE_DIR/bin"
mkdir -p "$RELEASE_DIR/oasis"
mkdir -p "$RELEASE_DIR/companion_app"

# 2. Build the project
echo "[1/4] Compiling Release Builds..."
cd core/1_simulation/src
./build_native.sh # Ideally pass -DCMAKE_BUILD_TYPE=Release
./build_wasm.sh
cd ../../..

# 3. Copy binaries and assets
echo "[2/4] Assembling Release Directory..."
# Copy Native Daemons
cp core/1_simulation/src/build_native/col_* "$RELEASE_DIR/bin/"
cp core/1_simulation/src/build_native/oasis_engine "$RELEASE_DIR/bin/" 2>/dev/null || true

# Copy Oasis WASM
cp -r core/1_simulation/src/build_wasm/* "$RELEASE_DIR/oasis/"

# Copy Companion App
cp -r core/1_simulation/src/web/companion/* "$RELEASE_DIR/companion_app/"

# 4. Generate user-friendly launch script for the Alpha testers
echo "[3/4] Generating Launch Script..."
cat << 'EOF' > "$RELEASE_DIR/start_collective.sh"
#!/bin/bash
echo "Starting The Collective Alpha..."

# Start daemons
./bin/col_gatewayd --port 8443 &
GATEWAY_PID=$!

# Start UIs
cd oasis && python3 -m http.server 8080 &
OASIS_PID=$!

cd ../companion_app && python3 -m http.server 8081 &
COMPANION_PID=$!

echo "========================================================"
echo " 🌍 Oasis (Layer 1) available at: http://localhost:8080"
echo " 📱 Companion (Layer 6) available at: http://localhost:8081"
echo "========================================================"
echo "Press Ctrl+C to exit."

trap "kill $GATEWAY_PID $OASIS_PID $COMPANION_PID; exit" INT
wait
EOF
chmod +x "$RELEASE_DIR/start_collective.sh"

# 5. Archive it
echo "[4/4] Archiving Release..."
cd release
tar -czvf collective_alpha.tar.gz collective_alpha/
cd ..

echo "========================================="
echo " ✅ Alpha Release Packaged Successfully!"
echo " 📁 Output: release/collective_alpha.tar.gz"
echo "========================================="
