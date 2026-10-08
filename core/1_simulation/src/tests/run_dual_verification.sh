#!/usr/bin/env bash
# ==============================================================================
# Dual Automated Verification & Quality Gate Suite
# 
# Executes:
# 1. Headless WebGPU Visual Verification (Suburban Sprawl Biome & Flecs Memory Logs)
#    - Verifies genuine WebGPU rendering in headless Chrome (Metal backend).
#    - Validates Suburban Sprawl procedural materials, color diversity >= 5, stddev >= 15.0.
#    - Validates Flecs ECS episodic memory logs ([Oasis Cognitive Core] Founder Memory: tick=).
#    - Confirms "Visual Verification Passed".
# 
# 2. Native Sovereign Stack IPC Verification (Catch2 Test Suite)
#    - Validates UHAI SPSC ring buffer memory layout (192B header, 64B slot, cacheline isolation).
#    - Validates monotonic 64-bit indexes with bitwise masking.
#    - Validates 1,000,000-frame concurrent throughput under contention without false sharing.
#    - Validates overrun dropped frame accounting and underrun safety.
#    - Confirms 12,463 assertions in 6 test cases pass and outputs "IPC Verification Passed".
# ==============================================================================

set -euo pipefail

# Resolve directories
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [[ -f "${SCRIPT_DIR}/../build_native/test_uhai_ring_buffer" ]]; then
    SIM_SRC_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
elif [[ -f "${SCRIPT_DIR}/build_native/test_uhai_ring_buffer" ]]; then
    SIM_SRC_DIR="${SCRIPT_DIR}"
elif [[ -f "${PWD}/build_native/test_uhai_ring_buffer" ]]; then
    SIM_SRC_DIR="${PWD}"
else
    SIM_SRC_DIR="/Users/raffaine/dev/collective/core/1_simulation/src"
fi

cd "${SIM_SRC_DIR}"

echo "================================================================================"
echo "=== Dual Automated Verification Suite: Oasis V4 & Sovereign Stack IPC ==="
echo "================================================================================"
echo "Root Simulation Directory: ${SIM_SRC_DIR}"
echo "Execution Timestamp:       $(date -u +"%Y-%m-%dT%H:%M:%SZ")"
echo ""

# -----------------------------------------------------------------------------
# Part 1: Headless WebGPU Visual Verification
# -----------------------------------------------------------------------------
echo "================================================================================"
echo "[Stage 1/2] Headless WebGPU Visual Verification (Suburban Sprawl Biome)"
echo "Command: node tests/verify_canvas.js"
echo "================================================================================"

VISUAL_OUTPUT_FILE=$(mktemp /tmp/oasis_visual_test_XXXXXX.log)

if node tests/verify_canvas.js 2>&1 | tee "${VISUAL_OUTPUT_FILE}"; then
    echo ""
    if grep -q "Visual Verification Passed" "${VISUAL_OUTPUT_FILE}"; then
        echo ">>> [STAGE 1 SUCCESS] Visual Verification Confirmed."
    else
        echo ">>> [ERROR] 'Visual Verification Passed' string missing from test output."
        rm -f "${VISUAL_OUTPUT_FILE}"
        exit 1
    fi
else
    VISUAL_EXIT=$?
    echo ""
    echo ">>> [STAGE 1 FAILED] Headless visual verification exited with code ${VISUAL_EXIT}."
    rm -f "${VISUAL_OUTPUT_FILE}"
    exit ${VISUAL_EXIT}
fi

rm -f "${VISUAL_OUTPUT_FILE}"
echo ""

# -----------------------------------------------------------------------------
# Part 2: Native IPC Catch2 Test Suite
# -----------------------------------------------------------------------------
echo "================================================================================"
echo "[Stage 2/2] Native Sovereign Stack IPC Verification (Catch2 Test Suite)"
echo "Command: ./build_native/test_uhai_ring_buffer"
echo "================================================================================"

IPC_OUTPUT_FILE=$(mktemp /tmp/oasis_ipc_test_XXXXXX.log)

if ./build_native/test_uhai_ring_buffer 2>&1 | tee "${IPC_OUTPUT_FILE}"; then
    echo ""
    if grep -q "All tests passed (12463 assertions in 6 test cases)" "${IPC_OUTPUT_FILE}"; then
        echo "IPC Verification Passed"
        echo ">>> [STAGE 2 SUCCESS] All 12,463 assertions in 6 Catch2 test cases passed."
    else
        echo ">>> [ERROR] Catch2 assertions did not match expected 12,463 assertions."
        rm -f "${IPC_OUTPUT_FILE}"
        exit 1
    fi
else
    IPC_EXIT=$?
    echo ""
    echo ">>> [STAGE 2 FAILED] Native Catch2 IPC test suite exited with code ${IPC_EXIT}."
    rm -f "${IPC_OUTPUT_FILE}"
    exit ${IPC_EXIT}
fi

rm -f "${IPC_OUTPUT_FILE}"
echo ""

# -----------------------------------------------------------------------------
# Quality Gate Final Summary
# -----------------------------------------------------------------------------
echo "================================================================================"
echo "DUAL AUTOMATED VERIFICATION PASSED"
echo "================================================================================"
echo "Visual Verification Passed"
echo "IPC Verification Passed"
echo ""
echo "Quality Gate Status: GREEN"
echo "- Procedural Legacy Biome: Suburban Sprawl (\"Lot 402 & Cul-de-sac\") Verified"
echo "- Material Diversity:      >= 5 distinct materials detected"
echo "- Spatial Variance:        StdDev >= 15.0 confirmed"
echo "- Cognitive Core Logging:  Flecs Episodic Memory verified"
echo "- UHAI SPSC Ring Buffer:   12,463 assertions across 6 test cases passed"
echo "- Concurrency Scale:       1,000,000 frames transferred without false sharing"
echo "================================================================================"

exit 0
