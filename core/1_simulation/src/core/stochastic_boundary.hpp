#pragma once

#include <cmath>
#include <algorithm>
#include <cstdint>
#include <cstddef>

namespace oasis {
namespace boundary {

// Genesis Epsilon Guard constant (0.001 kWh / L)
constexpr double GENESIS_EPSILON = 0.001;

// Irreducible legacy threat floor (0.001 s^-1 ~ 1 event per 16.6 min)
constexpr double IRREDUCIBLE_THREAT_FLOOR = 0.001;

// Geopolitical Legacy Regimes
enum class LegacyRegime : uint8_t {
    SubRadar = 0,      // lambda_0 = 0.005 s^-1
    UnderScrutiny = 1, // lambda_0 = 0.020 s^-1 (Default)
    Adversarial = 2,   // lambda_0 = 0.080 s^-1
    ActiveSiege = 3    // lambda_0 = 0.250 s^-1
};

// Compatibility aliases
using Regime = LegacyRegime;
using GeopoliticalRegime = LegacyRegime;

// Discrete Legacy Boundary Shock Event Types
enum class BoundaryShockType : uint8_t {
    None = 0,
    MunicipalCodeRaid = 1,
    UtilityShutoff = 2,
    TaxLien = 3,
    EpaWaterInspection = 4,

    // Backward / uppercase compatibility
    NONE = 0,
    MUNICIPAL_CODE_RAID = 1,
    UTILITY_SHUTOFF = 2,
    TAX_LIEN = 3,
    EPA_WATER_INSPECTION = 4
};

inline double GetRegimeBaseIntensity(LegacyRegime regime) noexcept {
    switch (regime) {
        case LegacyRegime::SubRadar:      return 0.005;
        case LegacyRegime::UnderScrutiny: return 0.020;
        case LegacyRegime::Adversarial:   return 0.080;
        case LegacyRegime::ActiveSiege:    return 0.250;
    }
    return 0.020;
}

// Pillar 1: Energy Self-Sufficiency / Sustainability
// Gracefully resolves to 1.0 when consumption is 0 on Day 1
inline double CalculateEnergySustainability(double solar_stored, double energy_consumed, double epsilon = GENESIS_EPSILON) noexcept {
    return std::min(1.0, (solar_stored + epsilon) / (energy_consumed + epsilon));
}

// Pillar 1: Water Self-Sufficiency / Sustainability
// Gracefully resolves to 1.0 when consumption is 0 on Day 1
inline double CalculateWaterSustainability(double rain_stored, double water_consumed, double epsilon = GENESIS_EPSILON) noexcept {
    return std::min(1.0, (rain_stored + epsilon) / (water_consumed + epsilon));
}

// Pillar 1: Autarky Ratio eta
// (energy_self + epsilon) / (energy_legacy + energy_self + epsilon)
// Gracefully resolves to 1.0 when legacy energy is 0
inline double CalculateAutarkyRatio(double energy_self, double energy_legacy, double epsilon = GENESIS_EPSILON) noexcept {
    return (energy_self + epsilon) / (energy_legacy + energy_self + epsilon);
}

// Pillar 1: Full Infrastructure Autarky Phi_infrastructure(t)
// 1/3 * (term_e + term_w + term_a)
inline double CalculateInfrastructureAutarky(
    double solar_stored, double energy_consumed,
    double rain_stored, double water_consumed,
    double area_depaved, double area_total,
    double epsilon = GENESIS_EPSILON
) noexcept {
    double term_e = CalculateEnergySustainability(solar_stored, energy_consumed, epsilon);
    double term_w = CalculateWaterSustainability(rain_stored, water_consumed, epsilon);
    double term_a = (area_total > 0.0) ? (area_depaved / area_total) : 0.0;
    return (1.0 / 3.0) * (term_e + term_w + term_a);
}

// Pillar 2: Social NPC Integration Phi_social(t)
// ln(1 + trust_hours_sum / 100.0)
inline double CalculateSocialIntegration(double trust_hours_sum) noexcept {
    return std::log(1.0 + trust_hours_sum / 100.0);
}

// Pillar 3: Peer Proximity Mesh Phi_mesh(t)
// (p_max > 0.0) ? (raw_mesh_sum / p_max) : 0.0
inline double CalculateMeshProximity(double raw_mesh_sum, double p_max = 5.0) noexcept {
    return (p_max > 0.0) ? (raw_mesh_sum / p_max) : 0.0;
}

// Sovereign Attenuation Potential Psi(t)
// alpha * Phi_infra + beta * Phi_social + gamma * tanh(delta * Phi_mesh)
// Bounded tanh weighting eliminates floating-point underflow
inline double CalculateSovereignAttenuation(
    double phi_infra,
    double phi_social,
    double phi_mesh,
    double alpha = 1.0,
    double beta = 1.0,
    double gamma = 1.0,
    double delta = 0.01
) noexcept {
    double mesh_term = std::tanh(delta * phi_mesh);
    return alpha * phi_infra + beta * phi_social + gamma * mesh_term;
}

// Inhomogeneous Poisson Arrival Intensity lambda(t)
// Clamped to irreducible threat floor lambda_min
inline double CalculatePoissonArrivalIntensity(
    LegacyRegime regime,
    double psi,
    double lambda_min = IRREDUCIBLE_THREAT_FLOOR
) noexcept {
    double lambda_regime = GetRegimeBaseIntensity(regime);
    double attenuated = lambda_regime * std::exp(-psi);
    return std::max(lambda_min, attenuated);
}

inline double CalculatePoissonArrivalIntensity(
    double lambda_regime,
    double psi,
    double lambda_min = IRREDUCIBLE_THREAT_FLOOR
) noexcept {
    double attenuated = lambda_regime * std::exp(-psi);
    return std::max(lambda_min, attenuated);
}

// Component structs for Flecs ECS (and zero-dependency testability)
struct InfrastructureAutarkyComponent {
    static constexpr double EPSILON = GENESIS_EPSILON;

    double solar_stored_kwh{0.0};
    double solar_consumed_kwh{0.0};
    double rain_stored_liters{0.0};
    double rain_consumed_liters{0.0};
    double depaved_area_sqm{0.0};
    double total_lot_area_sqm{100.0};

    [[nodiscard]] double EvaluatePhi() const noexcept {
        return CalculateInfrastructureAutarky(
            solar_stored_kwh, solar_consumed_kwh,
            rain_stored_liters, rain_consumed_liters,
            depaved_area_sqm, total_lot_area_sqm,
            EPSILON
        );
    }

    [[nodiscard]] double EvaluateAutarkyRatio(double energy_legacy = 0.0) const noexcept {
        return CalculateAutarkyRatio(solar_stored_kwh, energy_legacy, EPSILON);
    }
};

struct SocialMutualAidComponent {
    struct NpcBond {
        double trust{0.0};     // [0..100]
        double hours_aid{0.0}; // Hours of mutual aid
    };

    static constexpr size_t MAX_NPCS = 16;
    NpcBond bonds[MAX_NPCS]{};
    size_t count{0};

    [[nodiscard]] double EvaluatePhi() const noexcept {
        double sum = 0.0;
        for (size_t i = 0; i < count && i < MAX_NPCS; ++i) {
            sum += (bonds[i].trust * bonds[i].hours_aid);
        }
        return CalculateSocialIntegration(sum);
    }
};

struct PeerMeshComponent {
    struct PeerNode {
        double trust_score{0.0}; // [0..1.0] or [0..100]
        double distance_km{1.0};
    };

    static constexpr size_t MAX_PEERS = 8;
    static constexpr double D0 = 1.0; // 1.0 km reference distance
    PeerNode peers[MAX_PEERS]{};
    size_t count{0};
    double max_capacity{5.0}; // p_max = 5.0 default

    [[nodiscard]] double EvaluatePhi() const noexcept {
        if (max_capacity <= 0.0 || count == 0) return 0.0;
        double sum = 0.0;
        for (size_t i = 0; i < count && i < MAX_PEERS; ++i) {
            const double d = peers[i].distance_km / D0;
            sum += peers[i].trust_score / (1.0 + d * d);
        }
        return CalculateMeshProximity(sum, max_capacity);
    }
};

struct BoundaryInterfaceComponent {
    static constexpr double LAMBDA_MIN = IRREDUCIBLE_THREAT_FLOOR;

    LegacyRegime regime{LegacyRegime::UnderScrutiny};
    double alpha{1.0};
    double beta{1.0};
    double gamma{1.0};
    double delta{0.01};

    double psi{0.0};
    double lambda{0.02};
    double countdown_timer{50.0}; // Inter-arrival timer (seconds)
    double time_to_next_shock{50.0};
    uint64_t total_shocks{0};
    BoundaryShockType last_shock_type{BoundaryShockType::None};
    BoundaryShockType last_shock{BoundaryShockType::None};

    static constexpr double GetRegimeBaseLambda(LegacyRegime r) noexcept {
        return GetRegimeBaseIntensity(r);
    }

    static double CalculatePsi(double phi_infra, double phi_social, double phi_mesh,
                               double a = 1.0, double b = 1.0, double c = 1.0, double d = 0.01) noexcept {
        return CalculateSovereignAttenuation(phi_infra, phi_social, phi_mesh, a, b, c, d);
    }

    static double CalculateLambda(double lambda_r, double psi_val, double l_min = LAMBDA_MIN) noexcept {
        return CalculatePoissonArrivalIntensity(lambda_r, psi_val, l_min);
    }
};

} // namespace boundary
} // namespace oasis
