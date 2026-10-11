#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <random>
#include <vector>
#include <spdlog/spdlog.h>

#include "core/stochastic_boundary.hpp"

using namespace oasis::boundary;
using Catch::Matchers::WithinAbs;
using Catch::Matchers::WithinRel;

TEST_CASE("Genesis Autarky Epsilon Guard Day 1 NaN Prevention", "[genesis][epsilon][math]") {
    SECTION("Day 1 zero consumption resolves cleanly to 1.0 without NaN") {
        constexpr double e_solar_stored = 0.0;
        constexpr double e_consumed = 0.0;
        constexpr double w_rain_stored = 0.0;
        constexpr double w_consumed = 0.0;
        constexpr double e_legacy = 0.0;
        constexpr double e_self = 0.0;
        constexpr double area_depaved = 0.0;
        constexpr double area_total = 100.0;

        // Verify unshielded division results in NaN
        volatile double zero = 0.0;
        double unshielded = zero / zero;
        CHECK(std::isnan(unshielded));

        // Evaluate guarded sustainability metrics
        double sustainability_e = CalculateEnergySustainability(e_solar_stored, e_consumed);
        double sustainability_w = CalculateWaterSustainability(w_rain_stored, w_consumed);
        double autarky_ratio = CalculateAutarkyRatio(e_self, e_legacy);
        double phi_infra = CalculateInfrastructureAutarky(
            e_solar_stored, e_consumed,
            w_rain_stored, w_consumed,
            area_depaved, area_total
        );

        // Assert strictly finite, non-NaN, non-Inf
        REQUIRE_FALSE(std::isnan(sustainability_e));
        REQUIRE_FALSE(std::isinf(sustainability_e));
        REQUIRE(sustainability_e == 1.0);

        REQUIRE_FALSE(std::isnan(sustainability_w));
        REQUIRE_FALSE(std::isinf(sustainability_w));
        REQUIRE(sustainability_w == 1.0);

        REQUIRE_FALSE(std::isnan(autarky_ratio));
        REQUIRE_FALSE(std::isinf(autarky_ratio));
        REQUIRE(autarky_ratio == 1.0);

        REQUIRE_FALSE(std::isnan(phi_infra));
        REQUIRE_FALSE(std::isinf(phi_infra));
        REQUIRE_THAT(phi_infra, WithinAbs(2.0 / 3.0, 1e-6));

        // Component evaluation consistency
        InfrastructureAutarkyComponent autarky_comp{};
        REQUIRE_THAT(autarky_comp.EvaluatePhi(), WithinAbs(2.0 / 3.0, 1e-6));
        REQUIRE(autarky_comp.EvaluateAutarkyRatio(0.0) == 1.0);

        spdlog::info("[Catch2 Boundary Math] Genesis Epsilon Guard Day 1 passed: energy={:.4f}, water={:.4f}, autarky={:.4f}, Phi_infra={:.4f}",
                     sustainability_e, sustainability_w, autarky_ratio, phi_infra);
    }

    SECTION("Non-zero production and consumption boundary transitions") {
        // High production with zero consumption
        REQUIRE(CalculateEnergySustainability(50.0, 0.0) == 1.0);
        REQUIRE(CalculateWaterSustainability(100.0, 0.0) == 1.0);

        // Zero production with positive consumption
        double depleted_e = CalculateEnergySustainability(0.0, 50.0);
        REQUIRE_FALSE(std::isnan(depleted_e));
        REQUIRE_THAT(depleted_e, WithinAbs(0.001 / 50.001, 1e-6));
        REQUIRE(depleted_e > 0.0);
        REQUIRE(depleted_e < 0.001);

        // Partial self-sufficiency
        double partial_e = CalculateEnergySustainability(25.0, 50.0);
        REQUIRE_THAT(partial_e, WithinAbs(25.001 / 50.001, 1e-6));

        // 100% autarky with depaved lot
        double full_autarky = CalculateInfrastructureAutarky(50.0, 20.0, 100.0, 50.0, 100.0, 100.0);
        REQUIRE_THAT(full_autarky, WithinAbs(1.0, 1e-6));

        // Depleted autarky with zero depaving
        double zero_autarky = CalculateInfrastructureAutarky(0.0, 1000.0, 0.0, 1000.0, 0.0, 100.0);
        REQUIRE(zero_autarky > 0.0);
        REQUIRE(zero_autarky < 0.001);
    }
}

TEST_CASE("Sovereign Attenuation Potential and Bounded Mesh", "[attenuation][mesh]") {
    SECTION("Baseline Day 1 potential calculation") {
        double phi_infra = 2.0 / 3.0;
        double phi_social = CalculateSocialIntegration(0.0); // 0 agents
        double phi_mesh = CalculateMeshProximity(0.0);       // 0 peers

        REQUIRE(phi_social == 0.0);
        REQUIRE(phi_mesh == 0.0);

        double psi = CalculateSovereignAttenuation(phi_infra, phi_social, phi_mesh);
        REQUIRE_THAT(psi, WithinAbs(2.0 / 3.0, 1e-6));

        // Component evaluation consistency
        SocialMutualAidComponent social{};
        REQUIRE(social.EvaluatePhi() == 0.0);

        PeerMeshComponent mesh{};
        REQUIRE(mesh.EvaluatePhi() == 0.0);
    }

    SECTION("Bounded tanh behavior eliminating floating point underflow and overflow") {
        double phi_infra = 1.0;
        double phi_social = CalculateSocialIntegration(1600.0); // 10 agents, 80 trust, 200 hours
        REQUIRE_THAT(phi_social, WithinAbs(std::log(17.0), 1e-4));

        // Large mesh values bounded by tanh
        double huge_mesh = 1000.0;
        double psi = CalculateSovereignAttenuation(phi_infra, phi_social, huge_mesh, 1.0, 1.0, 1.0, 0.01);
        
        REQUIRE_FALSE(std::isnan(psi));
        REQUIRE_FALSE(std::isinf(psi));
        REQUIRE(psi > 0.0);
        // Mesh term tanh(10.0) is bounded in [0.0, 1.0)
        REQUIRE(std::tanh(0.01 * huge_mesh) < 1.0);
        REQUIRE(std::tanh(0.01 * huge_mesh) >= 0.0);

        // Extreme asymptotic upper bound check
        double extreme_mesh = 1e8;
        double psi_extreme = CalculateSovereignAttenuation(1.0, 0.0, extreme_mesh);
        REQUIRE_FALSE(std::isnan(psi_extreme));
        REQUIRE_FALSE(std::isinf(psi_extreme));
        REQUIRE_THAT(psi_extreme, WithinAbs(2.0, 1e-4));

        // PeerMeshComponent spatial inverse distance weighting
        PeerMeshComponent pcomp{};
        pcomp.count = 2;
        pcomp.peers[0] = {1.0, 0.5}; // trust=1.0, dist=0.5km -> 1.0/(1 + 0.25) = 0.8
        pcomp.peers[1] = {0.8, 1.0}; // trust=0.8, dist=1.0km -> 0.8/(1 + 1.0) = 0.4
        pcomp.max_capacity = 5.0;
        double p_phi = pcomp.EvaluatePhi(); // (0.8 + 0.4) / 5.0 = 0.24
        REQUIRE_THAT(p_phi, WithinAbs(0.24, 1e-4));
    }
}

TEST_CASE("Inhomogeneous Poisson Arrival Intensity and Regimes", "[poisson][regimes]") {
    SECTION("Base intensities for all geopolitical regimes") {
        REQUIRE(GetRegimeBaseIntensity(LegacyRegime::SubRadar) == 0.005);
        REQUIRE(GetRegimeBaseIntensity(LegacyRegime::UnderScrutiny) == 0.020);
        REQUIRE(GetRegimeBaseIntensity(LegacyRegime::Adversarial) == 0.080);
        REQUIRE(GetRegimeBaseIntensity(LegacyRegime::ActiveSiege) == 0.250);
    }

    SECTION("Irreducible legacy threat floor enforcement") {
        // Under scrutiny with zero attenuation
        double lambda_unattenuated = CalculatePoissonArrivalIntensity(LegacyRegime::UnderScrutiny, 0.0);
        REQUIRE(lambda_unattenuated == 0.020);

        // Active siege with zero attenuation
        double lambda_siege = CalculatePoissonArrivalIntensity(LegacyRegime::ActiveSiege, 0.0);
        REQUIRE(lambda_siege == 0.250);

        // Extreme attenuation (Psi = 50.0) clamped to floor
        double lambda_extreme = CalculatePoissonArrivalIntensity(LegacyRegime::ActiveSiege, 50.0);
        REQUIRE_FALSE(std::isnan(lambda_extreme));
        REQUIRE(lambda_extreme == IRREDUCIBLE_THREAT_FLOOR);
        REQUIRE(lambda_extreme >= 0.001);

        // SubRadar with high attenuation clamped to floor
        double lambda_subradar = CalculatePoissonArrivalIntensity(LegacyRegime::SubRadar, 100.0);
        REQUIRE(lambda_subradar == IRREDUCIBLE_THREAT_FLOOR);
    }

    SECTION("Over 85% attenuation criterion under high autarky (Backlog Story 4.2)") {
        double base_lambda = GetRegimeBaseIntensity(LegacyRegime::UnderScrutiny); // 0.020
        // Autarky: 100% solar (Phi_infra=1.0), 5 trusted citizens (Phi_social=ln(1+5*80*20/100)=ln(81)~4.39), 3 peers (mesh term~0.2)
        double psi = 1.0 + 4.39 + 0.2; // ~5.59
        double lambda_attenuated = CalculatePoissonArrivalIntensity(LegacyRegime::UnderScrutiny, psi);

        double reduction = (base_lambda - lambda_attenuated) / base_lambda;
        REQUIRE(reduction > 0.85);
        REQUIRE(lambda_attenuated >= IRREDUCIBLE_THREAT_FLOOR);
    }
}

TEST_CASE("Monte Carlo Poisson Arrival Distribution", "[poisson][montecarlo]") {
    std::mt19937_64 rng(1337);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    constexpr double lambda = 0.05; // 1 event per 20 seconds
    constexpr int N = 5000;

    double total_inter_arrival = 0.0;
    for (int i = 0; i < N; ++i) {
        double u = std::max(1e-12, dist(rng));
        double delta_t = -std::log(u) / lambda;
        REQUIRE_FALSE(std::isnan(delta_t));
        REQUIRE(delta_t > 0.0);
        total_inter_arrival += delta_t;
    }

    double empirical_mean = total_inter_arrival / N;
    double expected_mean = 1.0 / lambda; // 20.0 seconds

    spdlog::info("[Catch2 Boundary Math] Monte Carlo {} samples: empirical_mean={:.4f}s, expected_mean={:.4f}s (tolerance=5%)",
                 N, empirical_mean, expected_mean);

    REQUIRE_THAT(empirical_mean, WithinRel(expected_mean, 0.05));
}
