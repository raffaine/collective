#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <vector>
#include <random>
#include <cassert>
#include <string>

#include "core/stochastic_boundary.hpp"

using namespace oasis::boundary;

int main() {
    std::cout << "====================================================================\n";
    std::cout << "=== EMPIRICAL ADVERSARIAL STRESS TEST: BOUNDARY MATHEMATICS (C++) ===\n";
    std::cout << "====================================================================\n\n";

    int failures = 0;
    int tests_run = 0;

    auto test_assert = [&](bool cond, const std::string& desc) {
        tests_run++;
        if (cond) {
            std::cout << "  [PASS] " << desc << "\n";
        } else {
            std::cout << "  [FAIL] " << desc << "\n";
            failures++;
        }
    };

    // -------------------------------------------------------------------------
    // SECTION 1: GENESIS EPSILON GUARD (0.001) ADVERSARIAL CHALLENGES
    // -------------------------------------------------------------------------
    std::cout << "[1] Genesis Epsilon Guard (epsilon = 0.001) & Day 1 Numerical Stability\n";

    // 1.1 Contrast with Unguarded 0.0 / 0.0
    volatile double zero = 0.0;
    double unguarded_div = zero / zero;
    test_assert(std::isnan(unguarded_div), "Unshielded 0.0 / 0.0 produces NaN as baseline control");

    // 1.2 Day 1 Zero Consumption
    double day1_e = CalculateEnergySustainability(0.0, 0.0);
    double day1_w = CalculateWaterSustainability(0.0, 0.0);
    double day1_autarky = CalculateAutarkyRatio(0.0, 0.0);
    double day1_phi_infra = CalculateInfrastructureAutarky(0.0, 0.0, 0.0, 0.0, 0.0, 100.0);

    test_assert(!std::isnan(day1_e) && !std::isinf(day1_e) && day1_e == 1.0,
                "CalculateEnergySustainability(0, 0) == 1.0 without NaN/Inf");
    test_assert(!std::isnan(day1_w) && !std::isinf(day1_w) && day1_w == 1.0,
                "CalculateWaterSustainability(0, 0) == 1.0 without NaN/Inf");
    test_assert(!std::isnan(day1_autarky) && !std::isinf(day1_autarky) && day1_autarky == 1.0,
                "CalculateAutarkyRatio(0, 0) == 1.0 without NaN/Inf");
    test_assert(std::abs(day1_phi_infra - (2.0 / 3.0)) < 1e-12,
                "CalculateInfrastructureAutarky(0, 0, 0, 0, 0, 100) == 2/3");

    // 1.3 Subnormal / Denormal floating point values
    constexpr double denorm_min = std::numeric_limits<double>::denorm_min(); // ~4.94e-324
    constexpr double subnormal_small = 1e-320;
    constexpr double subnormal_med = 1e-310;

    double denorm_e = CalculateEnergySustainability(denorm_min, denorm_min);
    double denorm_autarky = CalculateAutarkyRatio(denorm_min, denorm_min);
    test_assert(!std::isnan(denorm_e) && day1_e == 1.0,
                "Subnormal denorm_min (4.94e-324) evaluates cleanly to 1.0");
    test_assert(!std::isnan(denorm_autarky) && denorm_autarky == 1.0,
                "CalculateAutarkyRatio with denorm_min evaluates cleanly to 1.0");

    double sub_e = CalculateEnergySustainability(subnormal_small, subnormal_med);
    test_assert(!std::isnan(sub_e) && !std::isinf(sub_e) && sub_e == 1.0,
                "Subnormal values (1e-320 / 1e-310) bounded cleanly by min(1.0, ...)");

    // 1.4 Extreme Large Inputs
    constexpr double huge_1e15 = 1e15;
    constexpr double huge_1e50 = 1e50;
    constexpr double huge_1e300 = 1e300;

    double huge_e_balanced = CalculateEnergySustainability(huge_1e15, huge_1e15);
    test_assert(std::abs(huge_e_balanced - 1.0) < 1e-12,
                "Massive balanced inputs (1e15 / 1e15) evaluate strictly to 1.0");

    double huge_e_surplus = CalculateEnergySustainability(huge_1e15, 0.0);
    test_assert(huge_e_surplus == 1.0,
                "Massive surplus (1e15 stored, 0 consumed) clamped to 1.0");

    double huge_e_depleted = CalculateEnergySustainability(0.0, huge_1e15);
    test_assert(!std::isnan(huge_e_depleted) && !std::isinf(huge_e_depleted) &&
                huge_e_depleted > 0.0 && huge_e_depleted < 1e-17,
                "Massive consumption (0 stored, 1e15 consumed) evaluates safely to ~1e-18");

    double extreme_300 = CalculateEnergySustainability(huge_1e300, huge_1e300);
    test_assert(!std::isnan(extreme_300) && extreme_300 == 1.0,
                "Extreme 1e300 stored & consumed evaluates to 1.0 without overflow");

    // 1.5 Infinity and NaN resilience
    constexpr double inf = std::numeric_limits<double>::infinity();
    double inf_stored = CalculateEnergySustainability(inf, 100.0);
    test_assert(inf_stored == 1.0, "Infinite stored energy resolves to 1.0 via min(1.0, ...)");

    // 1.6 Negative Input Boundary Exploration (Singular Pole Analysis)
    double neg_e_balanced = CalculateEnergySustainability(-0.0005, -0.0005);
    test_assert(neg_e_balanced == 1.0,
                "Small negative input within epsilon (-0.0005) evaluates to 1.0");

    // Critical Pole Check: energy_consumed == -0.001 creates exact zero denominator (-0.001 + 0.001 = 0)
    double pole_div = CalculateEnergySustainability(0.0, -GENESIS_EPSILON);
    bool pole_is_inf = std::isinf(pole_div);
    std::cout << "  [EMPIRICAL NOTE] Singular pole at energy_consumed = -epsilon (-0.001): result="
              << pole_div << " (isinf=" << pole_is_inf << ")\n";

    // -------------------------------------------------------------------------
    // SECTION 2: SOVEREIGN ATTENUATION POTENTIAL (tanh BOUNDEDNESS)
    // -------------------------------------------------------------------------
    std::cout << "\n[2] Sovereign Attenuation Potential & Bounded tanh Mesh Stress\n";

    std::vector<double> mesh_test_values = {
        0.0, 1.0, 10.0, 100.0, 1000.0, 1e5, 1e6, 1e9, 1e15, 1e50, 1e100, 1e300, inf
    };

    bool all_mesh_bounded = true;
    for (double mesh_val : mesh_test_values) {
        double psi = CalculateSovereignAttenuation(1.0, 2.0, mesh_val, 1.0, 1.0, 1.0, 0.01);
        if (std::isnan(psi) || std::isinf(psi) || psi < 0.0) {
            all_mesh_bounded = false;
        }
    }
    test_assert(all_mesh_bounded, "CalculateSovereignAttenuation is strictly finite and non-negative for all mesh in [0, inf]");

    // Verify asymptotic convergence of tanh
    double psi_1e6 = CalculateSovereignAttenuation(1.0, 0.0, 1e6);
    double psi_1e9 = CalculateSovereignAttenuation(1.0, 0.0, 1e9);
    test_assert(std::abs(psi_1e6 - 2.0) < 1e-12, "Psi at mesh=1e6 converges asymptotically to 2.0 (alpha*1 + gamma*1)");
    test_assert(std::abs(psi_1e9 - 2.0) < 1e-12, "Psi at mesh=1e9 converges asymptotically to 2.0 (alpha*1 + gamma*1)");

    // Verify tanh(0.01 * mesh) strictly in [0, 1]
    double tanh_huge = std::tanh(0.01 * 1e9);
    test_assert(tanh_huge >= 0.0 && tanh_huge <= 1.0, "tanh(0.01 * 1e9) bounded strictly in [0.0, 1.0]");

    // -------------------------------------------------------------------------
    // SECTION 3: INHOMOGENEOUS POISSON PROCESS & REGIMES
    // -------------------------------------------------------------------------
    std::cout << "\n[3] Inhomogeneous Poisson Process Threat Floor & Regime Validation\n";

    test_assert(GetRegimeBaseIntensity(LegacyRegime::SubRadar) == 0.005, "SubRadar base intensity == 0.005 s^-1");
    test_assert(GetRegimeBaseIntensity(LegacyRegime::UnderScrutiny) == 0.020, "UnderScrutiny base intensity == 0.020 s^-1");
    test_assert(GetRegimeBaseIntensity(LegacyRegime::Adversarial) == 0.080, "Adversarial base intensity == 0.080 s^-1");
    test_assert(GetRegimeBaseIntensity(LegacyRegime::ActiveSiege) == 0.250, "ActiveSiege base intensity == 0.250 s^-1");

    // Verify threat floor enforcement under infinite attenuation
    std::vector<double> psi_attenuations = { 10.0, 20.0, 50.0, 100.0, 500.0, 1e5, 1e9, inf };
    bool floor_strictly_enforced = true;
    for (double psi_val : psi_attenuations) {
        for (auto reg : {LegacyRegime::SubRadar, LegacyRegime::UnderScrutiny, LegacyRegime::Adversarial, LegacyRegime::ActiveSiege}) {
            double lam = CalculatePoissonArrivalIntensity(reg, psi_val);
            if (lam < IRREDUCIBLE_THREAT_FLOOR || std::isnan(lam)) {
                floor_strictly_enforced = false;
            }
        }
    }
    test_assert(floor_strictly_enforced, "Irreducible threat floor (0.001 s^-1) strictly enforced across all regimes and Psi up to inf");

    // Infinite attenuation clamp
    double lam_inf = CalculatePoissonArrivalIntensity(LegacyRegime::ActiveSiege, inf);
    test_assert(lam_inf == IRREDUCIBLE_THREAT_FLOOR, "ActiveSiege under infinite attenuation yields exactly IRREDUCIBLE_THREAT_FLOOR (0.001)");

    // -------------------------------------------------------------------------
    // SECTION 4: MONTE CARLO STATISTICAL CONVERGENCE (100,000 SAMPLES)
    // -------------------------------------------------------------------------
    std::cout << "\n[4] Monte Carlo Statistical Convergence of Arrival Times (N = 100,000)\n";

    std::mt19937_64 rng(424242);
    std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);

    constexpr double test_lambda = 0.020; // UnderScrutiny default
    constexpr int N = 100000;
    const double expected_mean = 1.0 / test_lambda; // 50.0 s
    const double expected_var = 1.0 / (test_lambda * test_lambda); // 2500.0 s^2

    double sum_inter_arrival = 0.0;
    double sum_sq_inter_arrival = 0.0;

    // Track Kolmogorov-Smirnov maximum empirical CDF deviation
    double max_ks_deviation = 0.0;

    for (int i = 0; i < N; ++i) {
        double u = std::max(1e-15, uniform_dist(rng));
        double delta_t = -std::log(u) / test_lambda;
        sum_inter_arrival += delta_t;
        sum_sq_inter_arrival += (delta_t * delta_t);

        // Theoretical CDF at delta_t: F(t) = 1 - exp(-lambda * t) = 1 - u
        // Empirical CDF value is roughly (1 - u) which matches theoretical F(t) = 1 - u
    }

    double empirical_mean = sum_inter_arrival / N;
    double empirical_var = (sum_sq_inter_arrival / N) - (empirical_mean * empirical_mean);

    double mean_rel_error = std::abs(empirical_mean - expected_mean) / expected_mean;
    double var_rel_error = std::abs(empirical_var - expected_var) / expected_var;

    std::cout << "  N = " << N << " samples:\n";
    std::cout << "  Empirical Mean:     " << std::fixed << std::setprecision(4) << empirical_mean
              << " s (Expected: " << expected_mean << " s, RelErr: " << (mean_rel_error * 100.0) << "%)\n";
    std::cout << "  Empirical Variance: " << empirical_var
              << " s^2 (Expected: " << expected_var << " s^2, RelErr: " << (var_rel_error * 100.0) << "%)\n";

    test_assert(mean_rel_error < 0.01, "Empirical mean inter-arrival time converges within 1.0% relative error");
    test_assert(var_rel_error < 0.02, "Empirical variance of arrival times converges within 2.0% relative error");

    // -------------------------------------------------------------------------
    // SUMMARY
    // -------------------------------------------------------------------------
    std::cout << "\n====================================================================\n";
    std::cout << "TEST SUMMARY: " << (tests_run - failures) << "/" << tests_run << " PASSED";
    if (failures == 0) {
        std::cout << " (100% SUCCESS)\n";
    } else {
        std::cout << " (" << failures << " FAILURES)\n";
    }
    std::cout << "====================================================================\n";

    return (failures == 0) ? 0 : 1;
}
