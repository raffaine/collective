#!/bin/bash
# start_dev.sh - The Collective Dev Loop Launcher
# Usage: ./scripts/start_dev.sh

echo "========================================="
echo " 🚀 Launching The Collective (Dev Mode) "
echo "========================================="

# 1. Cleanup old processes
echo "[1/4] Cleaning up previous background processes..."
pkill -f "col-gatewayd" || true
pkill -f "col-meshd" || true
pkill -f "http.server 8080" || true
pkill -f "http.server 8081" || true

# 2. Build the engine (Native Daemons + WASM Oasis)
echo "[2/4] Compiling Native Daemons and WASM Engine..."
cd core/1_simulation/src
./build_native.sh
./build_wasm.sh

# 3. Launch the Sovereign Stack Daemons
echo "[3/4] Starting Sovereign Stack Daemons..."
cd build_native
./col_gatewayd --port 8443 &
GATEWAY_PID=$!
echo "  -> col-gatewayd running on port 8443 (PID: $GATEWAY_PID)"
cd ../../../..

# 4. Launch the UIs
echo "[4/4] Starting Local Web Servers..."
# Oasis Game (Layer 1)
cd core/1_simulation/src/build_wasm
python3 -m http.server 8080 &
OASIS_PID=$!
echo "  -> Oasis Engine (Layer 1) running at: http://localhost:8080"
cd ../../../../

# Companion App (Layer 6)
cd core/1_simulation/src/web/companion
python3 -m http.server 8081 &
COMPANION_PID=$!
echo "  -> Companion App (Layer 6) running at: http://localhost:8081"
cd ../../../../

echo "========================================="
echo " ✅ Dev Loop Active!"
echo " - Press Ctrl+C to shutdown all services."
echo "========================================="

# Wait and trap Ctrl+C to kill everything
trap "echo 'Shutting down...'; kill $GATEWAY_PID $OASIS_PID $COMPANION_PID; exit" INT
wait
