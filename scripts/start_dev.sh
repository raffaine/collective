#!/bin/bash
# start_dev.sh - The Collective Dev Loop Launcher
# Usage: /Users/raffaine/dev/collective/scripts/start_dev.sh (from any directory)

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_ROOT"

echo "========================================="
echo " 🚀 Launching The Collective (Dev Mode) "
echo "========================================="

# 1. Cleanup old processes
echo "[1/4] Cleaning up previous background processes..."
pkill -f "col-gatewayd" || true
pkill -f "col_gatewayd" || true
pkill -f "col-meshd" || true
pkill -f "http.server 8080" || true
pkill -f "http.server 8081" || true

# 2. Build the engine (Native Daemons + WASM Oasis)
echo "[2/4] Compiling Native Daemons and WASM Engine..."
cd "$REPO_ROOT/core/1_simulation/src"
./build_native.sh
./build_wasm.sh

# 3. Launch the Sovereign Stack Daemons
echo "[3/4] Starting Sovereign Stack Daemons..."
cd "$REPO_ROOT/core/1_simulation/src/build_native"
./col_gatewayd --port 8443 &
GATEWAY_PID=$!
echo "  -> col-gatewayd running on port 8443 (PID: $GATEWAY_PID)"

# 4. Launch the UIs (bound to 127.0.0.1 so Chrome treats localhost as a Secure Context for WebGPU)
echo "[4/4] Starting Local Web Servers..."
python3 -m http.server 8080 --bind 127.0.0.1 --directory "$REPO_ROOT/core/1_simulation/src/build_wasm" &
OASIS_PID=$!
echo "  -> Oasis Engine (Layer 1) running at:  http://localhost:8080/oasis_engine.html"

python3 -m http.server 8081 --bind 127.0.0.1 --directory "$REPO_ROOT/core/1_simulation/src/web/companion" &
COMPANION_PID=$!
echo "  -> Companion App (Layer 6) running at: http://localhost:8081"

echo "========================================="
echo " ✅ Dev Loop Active!"
echo " - Open http://localhost:8080/oasis_engine.html in Google Chrome"
echo " - Press Ctrl+C to shutdown all services."
echo "========================================="

trap "echo 'Shutting down...'; kill $GATEWAY_PID $OASIS_PID $COMPANION_PID 2>/dev/null; exit" INT
wait
