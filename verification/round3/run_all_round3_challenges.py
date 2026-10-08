#!/usr/bin/env python3
"""
Master Test Runner: Round 3 Architectural Council Verification Gate
Runs all empirical challenge harnesses across D1, D2, D3, and D4.
"""

import subprocess
import sys
import time

TESTS = [
    ("Domain 1: DF Cognitive Mechanics", "verification/round3/test_d1_cognitive_stability.py"),
    ("Domain 2: Sims Autonomy Loop", "verification/round3/test_d2_autonomy_deadlock.py"),
    ("Domain 3: Spatial Simulation & Leeching", "verification/round3/test_d3_flow_solver_scalability.py"),
    ("Domain 4: Boundary Interface Mathematics", "verification/round3/test_d4_boundary_poisson.py"),
]

def main():
    print("=" * 80)
    print("OASIS GAME ENGINE - ROUND 3 ARCHITECTURAL VERIFICATION GATE")
    print("Challenger 1: Simulation & Algorithmic Verifier")
    print("=" * 80)
    start_time = time.time()
    
    total_passed = 0
    results = []

    for name, path in TESTS:
        print(f"\n[EXEC] Running {name} ({path})...")
        res = subprocess.run([sys.executable, path], capture_output=True, text=True)
        print(res.stdout)
        if res.returncode == 0:
            print(f"--> [PASS] {name}")
            total_passed += 1
            results.append((name, "PASS", ""))
        else:
            print(f"--> [FAIL] {name}")
            print(res.stderr)
            results.append((name, "FAIL", res.stderr))

    elapsed = time.time() - start_time
    print("\n" + "=" * 80)
    print("VERIFICATION SUITE SUMMARY")
    print(f"Tests Passed: {total_passed} / {len(TESTS)} in {elapsed:.3f}s")
    for name, status, _ in results:
        print(f"  * {name:<45}: {status}")
    print("=" * 80)

    if total_passed == len(TESTS):
        print("\nAll empirical challenge harnesses executed successfully.")
        sys.exit(0)
    else:
        print("\nOne or more challenge test suites failed.")
        sys.exit(1)

if __name__ == "__main__":
    main()
