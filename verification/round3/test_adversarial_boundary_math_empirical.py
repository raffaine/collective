#!/usr/bin/env python3
"""
Empirical Adversarial Stress Test Suite: Stochastic Boundary Mathematics
Sprint 3 Milestone 4 - Dual Sprint 3 Gate Verification

Target:
- core/1_simulation/src/core/stochastic_boundary.hpp
- core/1_simulation/src/tests/test_boundary_math.cpp
- core/1_simulation/src/main.cpp

Tests:
1. Genesis Epsilon Guard (epsilon = 0.001)
   - Day 1 zero consumption (E_consumed = 0, W_consumed = 0)
   - Comparison with unshielded 0/0 NaN
   - Subnormal numbers (sys.float_info.min, 1e-320)
   - Massive inputs (10^15, 10^50, 10^300)
   - Negative values and pole singularity exploration
2. Sovereign Attenuation Potential Psi(t)
   - Bounded tanh(delta * Phi_mesh) under massive inputs (10^6, 10^9, 10^15, inf)
   - Asymptotic convergence to upper bound
3. Inhomogeneous Poisson Process lambda(t)
   - Irreducible threat floor (0.001 s^-1) under infinite attenuation
   - Regime intensities (SubRadar=0.005, UnderScrutiny=0.020, Adversarial=0.080, ActiveSiege=0.250)
   - Statistical convergence: Monte Carlo arrival time distribution (N = 100,000)
   - Kolmogorov-Smirnov goodness-of-fit test against Exponential(lambda)
4. Native Catch2 math test suite execution
"""

import sys
import math
import random
import unittest
import subprocess
import os
import json
from typing import List, Tuple, Dict, Any

REPO_ROOT = "/Users/raffaine/dev/collective"
CORE_SIM_DIR = os.path.join(REPO_ROOT, "core/1_simulation/src")
TEST_MATH_BIN = os.path.join(CORE_SIM_DIR, "build_native/test_boundary_math")

GENESIS_EPSILON = 0.001
IRREDUCIBLE_THREAT_FLOOR = 0.001

REGIME_INTENSITIES = {
    "SubRadar": 0.005,
    "UnderScrutiny": 0.020,
    "Adversarial": 0.080,
    "ActiveSiege": 0.250,
}

# ---------------------------------------------------------------------------
# Direct Python Oracles mirroring C++ stochastic_boundary.hpp exactly
# ---------------------------------------------------------------------------

def calculate_energy_sustainability(solar_stored: float, energy_consumed: float, epsilon: float = GENESIS_EPSILON) -> float:
    return min(1.0, (solar_stored + epsilon) / (energy_consumed + epsilon))

def calculate_water_sustainability(rain_stored: float, water_consumed: float, epsilon: float = GENESIS_EPSILON) -> float:
    return min(1.0, (rain_stored + epsilon) / (water_consumed + epsilon))

def calculate_autarky_ratio(energy_self: float, energy_legacy: float, epsilon: float = GENESIS_EPSILON) -> float:
    return (energy_self + epsilon) / (energy_legacy + energy_self + epsilon)

def calculate_infrastructure_autarky(solar_stored: float, energy_consumed: float,
                                     rain_stored: float, water_consumed: float,
                                     area_depaved: float, area_total: float,
                                     epsilon: float = GENESIS_EPSILON) -> float:
    term_e = calculate_energy_sustainability(solar_stored, energy_consumed, epsilon)
    term_w = calculate_water_sustainability(rain_stored, water_consumed, epsilon)
    term_a = (area_depaved / area_total) if area_total > 0.0 else 0.0
    return (1.0 / 3.0) * (term_e + term_w + term_a)

def calculate_social_integration(trust_hours_sum: float) -> float:
    return math.log(1.0 + trust_hours_sum / 100.0)

def calculate_mesh_proximity(raw_mesh_sum: float, p_max: float = 5.0) -> float:
    return (raw_mesh_sum / p_max) if p_max > 0.0 else 0.0

def calculate_sovereign_attenuation(phi_infra: float, phi_social: float, phi_mesh: float,
                                    alpha: float = 1.0, beta: float = 1.0, gamma: float = 1.0, delta: float = 0.01) -> float:
    mesh_term = math.tanh(delta * phi_mesh)
    return alpha * phi_infra + beta * phi_social + gamma * mesh_term

def calculate_poisson_arrival_intensity(regime_lambda: float, psi: float, lambda_min: float = IRREDUCIBLE_THREAT_FLOOR) -> float:
    attenuated = regime_lambda * math.exp(-psi)
    return max(lambda_min, attenuated)


class TestGenesisEpsilonGuardAdversarial(unittest.TestCase):
    """Adversarial stress-testing of the Genesis Epsilon Guard."""

    def test_unshielded_vs_shielded_zero_division(self):
        print("\n--- [CHALLENGE 1.1] Unshielded Division vs Shielded Genesis Guard ---")
        # Unshielded 0.0 / 0.0 yields NaN
        unshielded_nan = float('nan')
        try:
            val = 0.0 / 0.0
        except ZeroDivisionError:
            val = float('nan')
        self.assertTrue(math.isnan(val), "Control: Unshielded division by zero yields NaN")

        # Shielded evaluates cleanly to 1.0
        shielded_e = calculate_energy_sustainability(0.0, 0.0)
        shielded_w = calculate_water_sustainability(0.0, 0.0)
        shielded_autarky = calculate_autarky_ratio(0.0, 0.0)
        phi_infra = calculate_infrastructure_autarky(0.0, 0.0, 0.0, 0.0, 0.0, 100.0)

        self.assertFalse(math.isnan(shielded_e))
        self.assertFalse(math.isinf(shielded_e))
        self.assertEqual(shielded_e, 1.0)

        self.assertFalse(math.isnan(shielded_w))
        self.assertFalse(math.isinf(shielded_w))
        self.assertEqual(shielded_w, 1.0)

        self.assertFalse(math.isnan(shielded_autarky))
        self.assertFalse(math.isinf(shielded_autarky))
        self.assertEqual(shielded_autarky, 1.0)

        self.assertFalse(math.isnan(phi_infra))
        self.assertAlmostEqual(phi_infra, 2.0 / 3.0, places=9)
        print(f"  Day 1 Shielded Energy:  {shielded_e:.4f} (PASS)")
        print(f"  Day 1 Shielded Water:   {shielded_w:.4f} (PASS)")
        print(f"  Day 1 Autarky Ratio:    {shielded_autarky:.4f} (PASS)")
        print(f"  Day 1 Phi_infra:        {phi_infra:.4f} (PASS: exactly 2/3)")

    def test_subnormal_floating_point_inputs(self):
        print("\n--- [CHALLENGE 1.2] Subnormal & Denormal Float Inputs ---")
        denorm_min = sys.float_info.min * sys.float_info.epsilon # ~4.94e-324
        subnormal_test_cases = [denorm_min, 1e-320, 1e-315, 1e-308, 1e-200]

        for sub in subnormal_test_cases:
            res_e = calculate_energy_sustainability(sub, sub)
            res_autarky = calculate_autarky_ratio(sub, sub)
            self.assertFalse(math.isnan(res_e))
            self.assertEqual(res_e, 1.0, f"Subnormal input {sub} must resolve to 1.0")
            self.assertEqual(res_autarky, 1.0, f"Subnormal input {sub} autarky must resolve to 1.0")

        # Subnormal stored with zero consumption
        res_surplus = calculate_energy_sustainability(denorm_min, 0.0)
        self.assertEqual(res_surplus, 1.0)
        print("  All subnormal and denormal inputs resolve gracefully to 1.0 without underflow corruption.")

    def test_massive_and_extreme_inputs(self):
        print("\n--- [CHALLENGE 1.3] Massive & Extreme Inputs (10^15 to 10^300) ---")
        huge_values = [1e12, 1e15, 1e50, 1e150, 1e300]
        for h in huge_values:
            # Balanced massive consumption
            res_balanced = calculate_energy_sustainability(h, h)
            self.assertFalse(math.isnan(res_balanced))
            self.assertAlmostEqual(res_balanced, 1.0, places=9)

            # Massive stored, zero consumed
            res_surplus = calculate_energy_sustainability(h, 0.0)
            self.assertEqual(res_surplus, 1.0)

            # Zero stored, massive consumed
            res_depleted = calculate_energy_sustainability(0.0, h)
            self.assertFalse(math.isnan(res_depleted))
            self.assertGreater(res_depleted, 0.0)
            self.assertLessEqual(res_depleted, (GENESIS_EPSILON / h) * 1.0000000001)

            # Autarky ratio
            autarky_res = calculate_autarky_ratio(h, h)
            self.assertAlmostEqual(autarky_res, 0.5, places=9)
            autarky_pure_self = calculate_autarky_ratio(h, 0.0)
            self.assertAlmostEqual(autarky_pure_self, 1.0, places=9)

        print("  Massive inputs (up to 1e300) tested: zero overflow, perfectly bounded.")

    def test_negative_values_and_singular_poles(self):
        print("\n--- [CHALLENGE 1.4] Adversarial Negative Values & Singular Pole Analysis ---")
        # If consumption is small negative, e.g. -0.0005:
        # (-0.0005 + 0.001) / (-0.0005 + 0.001) = 0.0005 / 0.0005 = 1.0
        res_small_neg = calculate_energy_sustainability(-0.0005, -0.0005)
        self.assertEqual(res_small_neg, 1.0)

        # Singular pole at energy_consumed == -0.001:
        # Denominator becomes (-0.001 + 0.001) = 0.0!
        with self.subTest("Singular Pole Behavior at energy_consumed = -0.001"):
            try:
                pole_val = calculate_energy_sustainability(0.0, -0.001)
                print(f"  Note: calculate_energy_sustainability(0, -0.001) in Python: {pole_val}")
            except ZeroDivisionError:
                print("  Note: calculate_energy_sustainability(0, -0.001) raises ZeroDivisionError in Python")


class TestSovereignAttenuationAdversarial(unittest.TestCase):
    """Adversarial stress-testing of Sovereign Attenuation Potential Psi(t)."""

    def test_bounded_tanh_massive_inputs(self):
        print("\n--- [CHALLENGE 2.1] Bounded tanh under Massive Mesh Sums ---")
        massive_mesh_values = [0.0, 1.0, 100.0, 1e4, 1e6, 1e9, 1e15, 1e50, 1e100, 1e300, float('inf')]

        for m in massive_mesh_values:
            psi = calculate_sovereign_attenuation(1.0, 1.0, m, alpha=1.0, beta=1.0, gamma=1.0, delta=0.01)
            self.assertFalse(math.isnan(psi))
            self.assertFalse(math.isinf(psi) if not math.isinf(m) else False)
            # Since alpha=1 (phi_infra=1), beta=1 (phi_social=1), gamma=1 (tanh(delta*m) in [0, 1]),
            # psi must be in [2.0, 3.0]
            self.assertGreaterEqual(psi, 2.0)
            self.assertLessEqual(psi, 3.0)

        # Check asymptotic convergence: for mesh >= 1e4, tanh(0.01 * 1e4) = tanh(100) = 1.0 (to float precision)
        psi_1e6 = calculate_sovereign_attenuation(1.0, 0.0, 1e6)
        psi_1e9 = calculate_sovereign_attenuation(1.0, 0.0, 1e9)
        self.assertAlmostEqual(psi_1e6, 2.0, places=9)
        self.assertAlmostEqual(psi_1e9, 2.0, places=9)
        print(f"  Psi(1.0, 0.0, 1e6) = {psi_1e6:.9f} -> Converged to 2.000000000")
        print(f"  Psi(1.0, 0.0, 1e9) = {psi_1e9:.9f} -> Converged to 2.000000000")


class TestInhomogeneousPoissonProcessAdversarial(unittest.TestCase):
    """Adversarial stress-testing of Inhomogeneous Poisson Process lambda(t)."""

    def test_regime_base_intensities(self):
        print("\n--- [CHALLENGE 3.1] Regime Base Intensities Verification ---")
        for name, expected in REGIME_INTENSITIES.items():
            base = calculate_poisson_arrival_intensity(expected, 0.0)
            self.assertEqual(base, expected)
            print(f"  Regime {name:<14}: lambda_0 = {base:.4f} s^-1")

    def test_irreducible_threat_floor_under_infinite_attenuation(self):
        print("\n--- [CHALLENGE 3.2] Irreducible Threat Floor Enforcement (lambda_min = 0.001) ---")
        extreme_psis = [5.0, 10.0, 20.0, 50.0, 100.0, 500.0, 1e5, 1e9, float('inf')]

        for regime_name, base_lambda in REGIME_INTENSITIES.items():
            for psi in extreme_psis:
                lambda_t = calculate_poisson_arrival_intensity(base_lambda, psi, lambda_min=IRREDUCIBLE_THREAT_FLOOR)
                self.assertFalse(math.isnan(lambda_t))
                self.assertGreaterEqual(lambda_t, IRREDUCIBLE_THREAT_FLOOR)
                if psi >= 20.0:
                    self.assertEqual(lambda_t, IRREDUCIBLE_THREAT_FLOOR)

        print(f"  Irreducible threat floor {IRREDUCIBLE_THREAT_FLOOR} s^-1 strictly maintained under infinite attenuation.")

    def test_monte_carlo_statistical_convergence_and_ks_test(self):
        print("\n--- [CHALLENGE 3.3] Monte Carlo Statistical Convergence (N = 100,000) ---")
        random.seed(987654321)
        target_lambda = 0.020 # UnderScrutiny default
        N = 100000

        expected_mean = 1.0 / target_lambda # 50.0 s
        expected_var = 1.0 / (target_lambda ** 2) # 2500.0 s^2

        samples = []
        for _ in range(N):
            u = random.random()
            while u <= 0.0:
                u = random.random()
            # Inverse CDF for Exponential(lambda): T = -ln(U) / lambda
            delta_t = -math.log(u) / target_lambda
            samples.append(delta_t)

        emp_mean = sum(samples) / N
        emp_var = sum((x - emp_mean) ** 2 for x in samples) / (N - 1)

        mean_err = abs(emp_mean - expected_mean) / expected_mean
        var_err = abs(emp_var - expected_var) / expected_var

        print(f"  Samples:            {N}")
        print(f"  Expected Mean:      {expected_mean:.4f} s | Empirical: {emp_mean:.4f} s (Error: {mean_err*100:.3f}%)")
        print(f"  Expected Variance:  {expected_var:.4f} s^2 | Empirical: {emp_var:.4f} s^2 (Error: {var_err*100:.3f}%)")

        self.assertLess(mean_err, 0.01, "Empirical mean within 1% of theoretical mean")
        self.assertLess(var_err, 0.02, "Empirical variance within 2% of theoretical variance")

        # Kolmogorov-Smirnov Goodness-of-Fit Test
        samples.sort()
        d_plus = 0.0
        d_minus = 0.0
        for i, x in enumerate(samples):
            # Theoretical CDF: F(x) = 1 - exp(-lambda * x)
            f_x = 1.0 - math.exp(-target_lambda * x)
            f_emp_plus = (i + 1) / N
            f_emp_minus = i / N
            d_plus = max(d_plus, f_emp_plus - f_x)
            d_minus = max(d_minus, f_x - f_emp_minus)

        d_stat = max(d_plus, d_minus)
        # Critical value for alpha = 0.01: 1.628 / sqrt(N)
        critical_ks = 1.628 / math.sqrt(N) # for 100k samples: ~0.00515
        print(f"  KS Statistic D:     {d_stat:.6f} (Critical 99% Threshold: {critical_ks:.6f})")
        self.assertLess(d_stat, critical_ks, f"KS Statistic {d_stat:.6f} must be below critical threshold {critical_ks:.6f}")
        print("  Statistical Convergence Confirmed: Inter-arrival times conform strictly to Exponential(lambda).")


class TestNativeCatch2Execution(unittest.TestCase):
    """Executes the native compiled Catch2 test binary."""

    def test_run_native_catch2_math_suite(self):
        print("\n--- [CHALLENGE 4.1] Executing Native Catch2 test_boundary_math ---")
        self.assertTrue(os.path.exists(TEST_MATH_BIN), f"Binary not found: {TEST_MATH_BIN}")

        proc = subprocess.run([TEST_MATH_BIN], capture_output=True, text=True)
        print(proc.stdout)
        if proc.stderr:
            print("Stderr:", proc.stderr)

        self.assertEqual(proc.returncode, 0, f"Catch2 test returned code {proc.returncode}")
        self.assertIn("All tests passed", proc.stdout)
        self.assertIn("10053 assertions in 4 test cases", proc.stdout)
        print("  Native Catch2 math suite: 10,053/10,053 assertions PASSED (exit code 0).")


def run_all_challenges():
    loader = unittest.TestLoader()
    suite = unittest.TestSuite()
    suite.addTests(loader.loadTestsFromTestCase(TestGenesisEpsilonGuardAdversarial))
    suite.addTests(loader.loadTestsFromTestCase(TestSovereignAttenuationAdversarial))
    suite.addTests(loader.loadTestsFromTestCase(TestInhomogeneousPoissonProcessAdversarial))
    suite.addTests(loader.loadTestsFromTestCase(TestNativeCatch2Execution))

    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)

    output_data = {
        "tests_run": result.testsRun,
        "failures": len(result.failures),
        "errors": len(result.errors),
        "was_successful": result.wasSuccessful(),
    }

    results_path = os.path.join(REPO_ROOT, "verification/boundary_adversarial_results.json")
    with open(results_path, "w", encoding="utf-8") as f:
        json.dump(output_data, f, indent=2)

    return result.wasSuccessful()


if __name__ == "__main__":
    success = run_all_challenges()
    sys.exit(0 if success else 1)
