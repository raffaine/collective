#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace oasis::stack {

// ============================================================================
// Sovereign Stack Enumerations (Strictly Fixed-Point / Zero IEEE-754 Floats)
// ============================================================================

enum class NeedTimingMode : uint8_t {
    NOW              = 0,
    SCHEDULED_FUTURE = 1
};

enum class DecisionStatus : uint8_t {
    PENDING              = 0,
    APPROVED             = 1,
    ADAPTED_AND_APPROVED = 2,
    DEFERRED             = 3
};

enum class SourcingMode : uint8_t {
    L7_LEGACY_CHANNEL  = 0,
    R1_MESH_PEERS      = 1,
    R0_LOCAL_WORKFLOWS = 2
};

enum class DistributionPolicy : uint8_t {
    R0_HOUSEHOLD                = 0,
    R1_TRUST_RING_COMMONS       = 1,
    COMMONS_PLUS_L7_BILL_OFFSET = 2
};

enum class TaskReleaseRing : uint8_t {
    R0_STEWARD_ONLY  = 0,
    R1_PEER_CLUSTERS = 1
};

enum class TaskCommitmentState : uint8_t {
    UNASSIGNED              = 0,
    COMMITTED_BY_PLAYER     = 1,
    RELEASED_TO_R1_PEERS    = 2,
    CLAIMED_BY_PEER_CLUSTER = 3,
    COMPLETED               = 4
};

enum class TaskExecutionModality : uint8_t {
    MANUAL_HUMAN_OBSERVATION = 0,
    AUTOMATED_L2_EXECUTION   = 1
};

enum class HaulModality : uint8_t {
    NONE                         = 0,
    GRANULAR_HAUL                = 1,
    BUCKET_LIQUID_HAUL           = 2,
    WHEELBARROW_DOLLY_HEAVY_HAUL = 3,
    TIFFIN_CARRIER_HAUL          = 4
};

enum class BpmnTokenState : uint8_t {
    WAITING_SEQUENCE_FLOW   = 0,
    BLOCKED_BY_PRECONDITION = 1,
    ACTIVE_TOKEN            = 2,
    COMPLETED               = 3,
    WAITING_TIMER           = 4
};

// ============================================================================
// String Conversion Helpers for Stack Enumerations
// ============================================================================

[[nodiscard]] constexpr const char* ToString(NeedTimingMode mode) noexcept {
    switch (mode) {
        case NeedTimingMode::NOW:              return "NOW";
        case NeedTimingMode::SCHEDULED_FUTURE: return "SCHEDULED_FUTURE";
    }
    return "NOW";
}

[[nodiscard]] constexpr const char* ToString(DecisionStatus status) noexcept {
    switch (status) {
        case DecisionStatus::PENDING:              return "PENDING";
        case DecisionStatus::APPROVED:             return "APPROVED";
        case DecisionStatus::ADAPTED_AND_APPROVED: return "ADAPTED_AND_APPROVED";
        case DecisionStatus::DEFERRED:             return "DEFERRED";
    }
    return "PENDING";
}

[[nodiscard]] constexpr const char* ToString(SourcingMode mode) noexcept {
    switch (mode) {
        case SourcingMode::L7_LEGACY_CHANNEL:  return "L7_LEGACY_CHANNEL";
        case SourcingMode::R1_MESH_PEERS:      return "R1_MESH_PEERS";
        case SourcingMode::R0_LOCAL_WORKFLOWS: return "R0_LOCAL_WORKFLOWS";
    }
    return "L7_LEGACY_CHANNEL";
}

[[nodiscard]] constexpr const char* ToString(DistributionPolicy policy) noexcept {
    switch (policy) {
        case DistributionPolicy::R0_HOUSEHOLD:                return "R0_HOUSEHOLD";
        case DistributionPolicy::R1_TRUST_RING_COMMONS:       return "R1_TRUST_RING_COMMONS";
        case DistributionPolicy::COMMONS_PLUS_L7_BILL_OFFSET: return "COMMONS_PLUS_L7_BILL_OFFSET";
    }
    return "R1_TRUST_RING_COMMONS";
}

[[nodiscard]] constexpr const char* ToString(TaskReleaseRing ring) noexcept {
    switch (ring) {
        case TaskReleaseRing::R0_STEWARD_ONLY:  return "R0_STEWARD_ONLY";
        case TaskReleaseRing::R1_PEER_CLUSTERS: return "R1_PEER_CLUSTERS";
    }
    return "R0_STEWARD_ONLY";
}

[[nodiscard]] constexpr const char* ToString(TaskCommitmentState state) noexcept {
    switch (state) {
        case TaskCommitmentState::UNASSIGNED:              return "UNASSIGNED";
        case TaskCommitmentState::COMMITTED_BY_PLAYER:     return "COMMITTED_BY_PLAYER";
        case TaskCommitmentState::RELEASED_TO_R1_PEERS:    return "RELEASED_TO_R1_PEERS";
        case TaskCommitmentState::CLAIMED_BY_PEER_CLUSTER: return "CLAIMED_BY_PEER_CLUSTER";
        case TaskCommitmentState::COMPLETED:               return "COMPLETED";
    }
    return "UNASSIGNED";
}

[[nodiscard]] constexpr const char* ToString(TaskExecutionModality modality) noexcept {
    switch (modality) {
        case TaskExecutionModality::MANUAL_HUMAN_OBSERVATION: return "MANUAL_HUMAN_OBSERVATION";
        case TaskExecutionModality::AUTOMATED_L2_EXECUTION:   return "AUTOMATED_L2_EXECUTION";
    }
    return "MANUAL_HUMAN_OBSERVATION";
}

[[nodiscard]] constexpr const char* ToString(HaulModality modality) noexcept {
    switch (modality) {
        case HaulModality::NONE:                         return "NONE";
        case HaulModality::GRANULAR_HAUL:                return "GRANULAR_HAUL";
        case HaulModality::BUCKET_LIQUID_HAUL:           return "BUCKET_LIQUID_HAUL";
        case HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL: return "WHEELBARROW_DOLLY_HEAVY_HAUL";
        case HaulModality::TIFFIN_CARRIER_HAUL:          return "TIFFIN_CARRIER_HAUL";
    }
    return "NONE";
}

[[nodiscard]] constexpr const char* ToString(BpmnTokenState state) noexcept {
    switch (state) {
        case BpmnTokenState::WAITING_SEQUENCE_FLOW:   return "WAITING_SEQUENCE_FLOW";
        case BpmnTokenState::BLOCKED_BY_PRECONDITION: return "BLOCKED_BY_PRECONDITION";
        case BpmnTokenState::ACTIVE_TOKEN:            return "ACTIVE_TOKEN";
        case BpmnTokenState::COMPLETED:               return "COMPLETED";
        case BpmnTokenState::WAITING_TIMER:           return "WAITING_TIMER";
    }
    return "WAITING_SEQUENCE_FLOW";
}

[[nodiscard]] inline NeedTimingMode ParseNeedTimingMode(std::string_view raw) noexcept {
    if (raw == "SCHEDULED_FUTURE" || raw == "TOMORROW" || raw == "FUTURE" || raw == "SCHEDULED") {
        return NeedTimingMode::SCHEDULED_FUTURE;
    }
    return NeedTimingMode::NOW;
}

[[nodiscard]] inline SourcingMode ParseSourcingMode(std::string_view raw, SourcingMode fallback) noexcept {
    if (raw == "L7_LEGACY_CHANNEL" || raw == "L7_FALLBACK" || raw == "L7") {
        return SourcingMode::L7_LEGACY_CHANNEL;
    }
    if (raw == "R1_MESH_PEERS" || raw == "R1_PEERS" || raw == "HYBRID_L5_L7") {
        return SourcingMode::R1_MESH_PEERS;
    }
    if (raw == "R0_LOCAL_WORKFLOWS" || raw == "LOCAL_L5_FIRST" || raw == "L5") {
        return SourcingMode::R0_LOCAL_WORKFLOWS;
    }
    return fallback;
}

[[nodiscard]] inline DistributionPolicy ParseDistributionPolicy(std::string_view raw, DistributionPolicy fallback) noexcept {
    if (raw == "R0_HOUSEHOLD" || raw == "R0_ONLY") {
        return DistributionPolicy::R0_HOUSEHOLD;
    }
    if (raw == "R1_TRUST_RING_COMMONS" || raw == "R0_AND_R1_TIFFINS") {
        return DistributionPolicy::R1_TRUST_RING_COMMONS;
    }
    if (raw == "COMMONS_PLUS_L7_BILL_OFFSET" || raw == "L7_SURPLUS_SALE") {
        return DistributionPolicy::COMMONS_PLUS_L7_BILL_OFFSET;
    }
    return fallback;
}

[[nodiscard]] inline TaskReleaseRing ParseTaskReleaseRing(std::string_view raw, TaskReleaseRing fallback) noexcept {
    if (raw == "R1_PEER_CLUSTERS" || raw == "R1_PEERS") {
        return TaskReleaseRing::R1_PEER_CLUSTERS;
    }
    if (raw == "R0_STEWARD_ONLY" || raw == "R0_SELF") {
        return TaskReleaseRing::R0_STEWARD_ONLY;
    }
    return fallback;
}

// ============================================================================
// Lightweight Zero-Float JSON Escaping & Extraction Utilities
// ============================================================================

namespace json_util {

[[nodiscard]] inline std::string Escape(std::string_view input) {
    std::string out;
    out.reserve(input.size() + 8U);
    for (char ch : input) {
        switch (ch) {
            case '\"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b";  break;
            case '\f': out += "\\f";  break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (static_cast<unsigned char>(ch) < 0x20U) {
                    out += ' ';
                } else {
                    out += ch;
                }
                break;
        }
    }
    return out;
}

[[nodiscard]] inline std::string ToLowerAscii(std::string_view s) {
    std::string out(s);
    for (char& c : out) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return out;
}

[[nodiscard]] inline size_t FindKeyValueStart(std::string_view json, std::string_view key, size_t start_pos = 0U) noexcept {
    std::string needle = "\"";
    needle.append(key);
    needle.push_back('\"');

    size_t pos = start_pos;
    while (pos < json.size()) {
        size_t hit = json.find(needle, pos);
        if (hit == std::string_view::npos) {
            return std::string_view::npos;
        }
        size_t after_key = hit + needle.size();
        while (after_key < json.size() && std::isspace(static_cast<unsigned char>(json[after_key]))) {
            ++after_key;
        }
        if (after_key < json.size() && json[after_key] == ':') {
            size_t val_pos = after_key + 1U;
            while (val_pos < json.size() && std::isspace(static_cast<unsigned char>(json[val_pos]))) {
                ++val_pos;
            }
            return val_pos;
        }
        pos = hit + needle.size();
    }
    return std::string_view::npos;
}

[[nodiscard]] inline std::string ExtractString(std::string_view json, std::string_view key, std::string_view default_val = {}) {
    size_t val_pos = FindKeyValueStart(json, key);
    if (val_pos == std::string_view::npos || val_pos >= json.size() || json[val_pos] != '\"') {
        return std::string(default_val);
    }
    ++val_pos;
    std::string result;
    while (val_pos < json.size()) {
        char c = json[val_pos];
        if (c == '\\' && (val_pos + 1U) < json.size()) {
            char next = json[val_pos + 1U];
            switch (next) {
                case '\"': result.push_back('\"'); break;
                case '\\': result.push_back('\\'); break;
                case '/':  result.push_back('/');  break;
                case 'n':  result.push_back('\n'); break;
                case 'r':  result.push_back('\r'); break;
                case 't':  result.push_back('\t'); break;
                default:   result.push_back(next); break;
            }
            val_pos += 2U;
            continue;
        }
        if (c == '\"') {
            break;
        }
        result.push_back(c);
        ++val_pos;
    }
    return result;
}

[[nodiscard]] inline int64_t ExtractInt64(std::string_view json, std::string_view key, int64_t default_val = 0) noexcept {
    size_t val_pos = FindKeyValueStart(json, key);
    if (val_pos == std::string_view::npos || val_pos >= json.size()) {
        return default_val;
    }
    bool negative = false;
    if (json[val_pos] == '-') {
        negative = true;
        ++val_pos;
    } else if (json[val_pos] == '+') {
        ++val_pos;
    }
    if (val_pos >= json.size() || !std::isdigit(static_cast<unsigned char>(json[val_pos]))) {
        return default_val;
    }
    uint64_t acc = 0ULL;
    while (val_pos < json.size() && std::isdigit(static_cast<unsigned char>(json[val_pos]))) {
        uint64_t digit = static_cast<uint64_t>(json[val_pos] - '0');
        if (acc <= (static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) - digit) / 10ULL) {
            acc = acc * 10ULL + digit;
        }
        ++val_pos;
    }
    return negative ? -static_cast<int64_t>(acc) : static_cast<int64_t>(acc);
}

[[nodiscard]] inline uint64_t ExtractUint64(std::string_view json, std::string_view key, uint64_t default_val = 0ULL) noexcept {
    size_t val_pos = FindKeyValueStart(json, key);
    if (val_pos == std::string_view::npos || val_pos >= json.size()) {
        return default_val;
    }
    if (!std::isdigit(static_cast<unsigned char>(json[val_pos]))) {
        return default_val;
    }
    uint64_t acc = 0ULL;
    while (val_pos < json.size() && std::isdigit(static_cast<unsigned char>(json[val_pos]))) {
        uint64_t digit = static_cast<uint64_t>(json[val_pos] - '0');
        if (acc <= (std::numeric_limits<uint64_t>::max() - digit) / 10ULL) {
            acc = acc * 10ULL + digit;
        }
        ++val_pos;
    }
    return acc;
}

[[nodiscard]] inline bool HasKey(std::string_view json, std::string_view key) noexcept {
    return FindKeyValueStart(json, key) != std::string_view::npos;
}

[[nodiscard]] inline bool ExtractBool(std::string_view json, std::string_view key, bool default_val = false) noexcept {
    size_t val_pos = FindKeyValueStart(json, key);
    if (val_pos == std::string_view::npos || val_pos >= json.size()) {
        return default_val;
    }
    if (json.substr(val_pos, 4U) == "true" || json[val_pos] == '1') {
        return true;
    }
    if (json.substr(val_pos, 5U) == "false" || json[val_pos] == '0') {
        return false;
    }
    return default_val;
}

[[nodiscard]] inline std::string_view ExtractObjectSlice(std::string_view json, std::string_view key) noexcept {
    size_t val_pos = FindKeyValueStart(json, key);
    if (val_pos == std::string_view::npos || val_pos >= json.size() || json[val_pos] != '{') {
        return {};
    }
    size_t depth = 0U;
    bool in_str = false;
    for (size_t i = val_pos; i < json.size(); ++i) {
        char c = json[i];
        if (c == '\\' && in_str) {
            ++i;
            continue;
        }
        if (c == '\"') {
            in_str = !in_str;
            continue;
        }
        if (!in_str) {
            if (c == '{') {
                ++depth;
            } else if (c == '}') {
                --depth;
                if (depth == 0U) {
                    return json.substr(val_pos, i - val_pos + 1U);
                }
            }
        }
    }
    return {};
}

} // namespace json_util

// ============================================================================
// Layer 2: Hardware Membrane Sensor & Actuator Telemetry Table
// ============================================================================

struct alignas(8) L2SensorRegister {
    std::string register_id{};
    std::string hardware_modality{};
    uint64_t    last_seen_tick{0};
    uint64_t    cumulative_exergy_uj{0};
    uint32_t    temperature_mk{293150};   // Millikelvin (293150 mK = 20.0 C)
    uint32_t    moisture_bp{0};           // Basis points (0..10000)
    uint32_t    concentration_ppm{0};     // Parts per million (e.g. mineral N ppm)
    uint32_t    actuator_level_bp{0};     // Basis points (0..10000)
    uint32_t    hold_duration_ms{0};      // Milliseconds at target lethality/hold
    uint32_t    crop_growth_bp{0};        // Phenological maturity basis points (0..10000)
    uint8_t     is_online{0};             // 0 = absent/offline, 1 = active in L2TelemetryTable
    uint8_t     is_actuator{0};           // 0 = sensor, 1 = actuator
    uint8_t     actuator_commanded_on{0}; // 1 = open/energized by L4 controller
    uint8_t     quality_flags{1};         // 1 = QUALITY_SIMULATED, 2 = QUALITY_HARDWARE_ROOT
    [[maybe_unused]] uint8_t pad_[4]{0};
};
static_assert(alignof(L2SensorRegister) == 8, "L2SensorRegister must be 8-byte aligned.");

// ============================================================================
// Parameterized Legume Cultivar Specification (Fabaceae Real & Sim-Tuned)
// ============================================================================

struct alignas(8) CultivarWorkflowSpec {
    uint64_t    seed_exergy_uj{400000000ULL};
    std::string cultivar_id{};
    std::string title{};
    std::string species_binomial{};
    std::string agronomic_notes{};
    uint32_t    maturity_sim_seconds{43200U};        // Ideal 1.0x growth duration 0 -> 10000 bp
    uint32_t    verification_interval_sim_s{10800U}; // BPMN IntermediateCatchEvent timer cadence
    uint32_t    min_growth_temp_mk{277150U};         // Lower thermal cutoff (mK)
    uint32_t    opt_low_temp_mk{285150U};            // Optimal thermal plateau start (mK)
    uint32_t    opt_high_temp_mk{302150U};           // Optimal thermal plateau end (mK)
    uint32_t    max_growth_temp_mk{313150U};         // Upper heat-stress cutoff (mK)
    uint32_t    target_min_vwc_bp{3000U};            // Verification moisture trigger (bp)
    uint32_t    target_opt_vwc_bp{4200U};            // Post-watering target root-zone VWC (bp)
    uint32_t    target_max_vwc_bp{4900U};            // Upper optimal VWC bound (bp)
    uint32_t    irrigation_dose_ml{4500U};           // Per-actuation water dose (ml)
    uint32_t    et_multiplier_bp{15000U};            // Evapotranspiration multiplier (10000 = 1.0x)
    uint32_t    n_fixation_rate_bp{14000U};          // Rhizobium N2 fixation multiplier (10000 = 1.0x)
    uint32_t    harvest_mature_bp{9500U};            // Maturity threshold to unlock Harvest step (bp)
    uint16_t    trellis_ridge_height_mm{1500U};      // Required A-frame trellis ridge height (mm)
    uint16_t    sowing_depth_mm{30U};                // Seed placement depth (mm)
    uint8_t     n_draw_per_band_ppm{0U};             // Mineral N draw per 250 bp biomass band (ppm)
    uint8_t     p_draw_per_band_ppm{1U};             // Available P draw per 250 bp biomass band (ppm)
    uint8_t     k_draw_per_band_ppm{1U};             // Exchangeable K draw per 250 bp biomass band (ppm)
    uint8_t     fixes_atmospheric_n2{1U};            // 1 = active Rhizobium symbiosis, 0 = soil-N consumer
    uint8_t     is_virtual_sim_tuned{1U};            // 1 = game-tuned virtual physics, 0 = heritage agronomy
    uint8_t     harvest_legume_batches{2U};          // Harvested legume batches per bed cycle
    uint8_t     harvest_mirepoix_batches{1U};        // Companion understory mirepoix batches per cycle
    [[maybe_unused]] uint8_t pad_{0U};
};
static_assert(alignof(CultivarWorkflowSpec) == 8, "CultivarWorkflowSpec must be 8-byte aligned.");

[[nodiscard]] inline std::string FormatSimDurationCompact(uint64_t sim_s) {
    if (sim_s == 0ULL) {
        return "due now";
    }
    if (sim_s >= 86400ULL) {
        const uint64_t days  = sim_s / 86400ULL;
        const uint64_t hours = (sim_s % 86400ULL) / 3600ULL;
        if (hours == 0ULL) {
            return std::to_string(days) + "d";
        }
        return std::to_string(days) + "d " + std::to_string(hours) + "h";
    }
    if (sim_s >= 3600ULL) {
        const uint64_t hours = sim_s / 3600ULL;
        const uint64_t mins  = (sim_s % 3600ULL) / 60ULL;
        if (mins == 0ULL) {
            return std::to_string(hours) + "h";
        }
        return std::to_string(hours) + "h " + std::to_string(mins) + "m";
    }
    const uint64_t mins = std::max<uint64_t>(1ULL, (sim_s + 59ULL) / 60ULL);
    return std::to_string(mins) + "m";
}

[[nodiscard]] constexpr const char* ComputePhenologyLabel(
    uint32_t crop_growth_bp,
    uint8_t  crop_sown,
    uint32_t harvest_mature_bp = 9500U
) noexcept {
    if (crop_sown == 0U) {
        return "Unsown Bed";
    }
    if (crop_growth_bp >= harvest_mature_bp) {
        return "Mature Pods Ready for Harvest";
    }
    if (crop_growth_bp >= 6800U) {
        return "Flowering & Pod Fill";
    }
    if (crop_growth_bp >= 2500U) {
        return "Vegetative Tendril Climb";
    }
    return "Germinating Radicle & Shoots";
}

[[nodiscard]] constexpr uint32_t EvaluateStackCultivarTempFactorBp(
    uint32_t soil_temp_mk,
    const CultivarWorkflowSpec& spec
) noexcept {
    if (soil_temp_mk <= spec.min_growth_temp_mk || soil_temp_mk >= spec.max_growth_temp_mk) {
        return 0U;
    }
    if (soil_temp_mk < spec.opt_low_temp_mk) {
        const uint32_t span = std::max(1U, spec.opt_low_temp_mk - spec.min_growth_temp_mk);
        return ((soil_temp_mk - spec.min_growth_temp_mk) * 10000U) / span;
    }
    if (soil_temp_mk <= spec.opt_high_temp_mk) {
        return 10000U;
    }
    const uint32_t span = std::max(1U, spec.max_growth_temp_mk - spec.opt_high_temp_mk);
    return ((spec.max_growth_temp_mk - soil_temp_mk) * 10000U) / span;
}

[[nodiscard]] constexpr uint32_t EvaluateStackCultivarMoistureFactorBp(
    uint32_t moisture_bp,
    const CultivarWorkflowSpec& spec
) noexcept {
    constexpr uint32_t kWiltingBp     = 1000U;
    constexpr uint32_t kWaterloggedBp = 7800U;
    if (moisture_bp <= kWiltingBp || moisture_bp >= kWaterloggedBp) {
        return 0U;
    }
    const uint32_t opt_low  = std::max(kWiltingBp + 200U, spec.target_min_vwc_bp);
    const uint32_t opt_high = std::max(opt_low + 400U, spec.target_max_vwc_bp);
    if (moisture_bp < opt_low) {
        return ((moisture_bp - kWiltingBp) * 10000U) / (opt_low - kWiltingBp);
    }
    if (moisture_bp <= opt_high) {
        return 10000U;
    }
    return ((kWaterloggedBp - moisture_bp) * 10000U) / std::max(1U, kWaterloggedBp - opt_high);
}

struct alignas(8) L2TelemetryTable {
    std::vector<L2SensorRegister> registers{};
    uint64_t                      last_update_tick{0};

    void UpsertRegister(const L2SensorRegister& incoming) {
        for (auto& existing : registers) {
            if (existing.register_id == incoming.register_id) {
                existing = incoming;
                return;
            }
        }
        registers.push_back(incoming);
    }

    [[nodiscard]] const L2SensorRegister* FindRegister(std::string_view register_id) const noexcept {
        for (const auto& reg : registers) {
            if (reg.register_id == register_id) {
                return &reg;
            }
        }
        return nullptr;
    }

    [[nodiscard]] L2SensorRegister* FindRegisterMutable(std::string_view register_id) noexcept {
        for (auto& reg : registers) {
            if (reg.register_id == register_id) {
                return &reg;
            }
        }
        return nullptr;
    }

    [[nodiscard]] bool IsRegisterOnline(std::string_view register_id) const noexcept {
        if (register_id.empty()) {
            return false;
        }
        const L2SensorRegister* reg = FindRegister(register_id);
        return (reg != nullptr) && (reg->is_online != 0U);
    }

    void SetRegisterOnline(std::string_view register_id, bool online, uint64_t tick = 0ULL) {
        L2SensorRegister* reg = FindRegisterMutable(register_id);
        if (reg != nullptr) {
            reg->is_online = online ? 1U : 0U;
            reg->last_seen_tick = tick;
            last_update_tick = tick;
            return;
        }
        if (online) {
            L2SensorRegister created{};
            created.register_id = std::string(register_id);
            created.hardware_modality = (register_id == "irrigation_valve_v1")
                                            ? "SOLENOID_VALVE_ACTUATOR"
                                            : "UHAI_FIELD_REGISTER";
            created.last_seen_tick = tick;
            created.is_online = 1U;
            created.is_actuator = (register_id == "irrigation_valve_v1") ? 1U : 0U;
            registers.push_back(created);
            last_update_tick = tick;
        }
    }

    [[nodiscard]] uint32_t CountOnlineRegisters() const noexcept {
        uint32_t count = 0U;
        for (const auto& reg : registers) {
            if (reg.is_online != 0U) {
                ++count;
            }
        }
        return count;
    }
};
static_assert(alignof(L2TelemetryTable) == 8, "L2TelemetryTable must be 8-byte aligned.");

// ============================================================================
// Layer 6: Needs & Intent Commons Records
// ============================================================================

struct alignas(8) L6NeedRecord {
    std::string  need_id{};
    std::string  title{};
    std::string  category{};
    std::string  description{};
    std::string  l7_channel_label{};
    std::string  local_channel_label{};
    std::string  governed_asset_id{};
    std::string  last_status_note{};
    uint64_t     exergy_cost_uj{0};
    int64_t      l7_unit_cost_cents{0};
    uint32_t     action_code{0};
    uint32_t     request_count{0};
    SourcingMode active_sourcing_mode{SourcingMode::L7_LEGACY_CHANNEL};
    uint8_t      is_r0_baseline_care{0};
    uint8_t      is_custom_mesh_need{0};
    [[maybe_unused]] uint8_t pad_[5]{0};
};
static_assert(alignof(L6NeedRecord) == 8, "L6NeedRecord must be 8-byte aligned.");

struct alignas(8) L6IntentRequest {
    std::string    request_id{};
    std::string    need_id{};
    std::string    need_title{};
    std::string    requester_did{};
    std::string    fulfillment_note{};
    uint64_t       exergy_budget_uj{0};
    uint64_t       created_tick{0};
    uint32_t       due_sim_day{0};
    uint32_t       quantity_units{1};
    NeedTimingMode timing_mode{NeedTimingMode::NOW};
    SourcingMode   resolved_sourcing_mode{SourcingMode::L7_LEGACY_CHANNEL};
    uint8_t        fulfilled{0};
    [[maybe_unused]] uint8_t pad_[5]{0};
};
static_assert(alignof(L6IntentRequest) == 8, "L6IntentRequest must be 8-byte aligned.");

struct alignas(8) L6PendingDecision {
    std::string              decision_id{};
    std::string              title{};
    std::string              governed_asset_id{};
    std::string              rationale{};
    std::vector<std::string> linked_need_ids{};
    uint64_t                 estimated_exergy_savings_uj{0};
    int64_t                  estimated_monthly_l7_savings_cents{0};
    uint64_t                 emitted_tick{0};
    SourcingMode             proposed_sourcing_mode{SourcingMode::R0_LOCAL_WORKFLOWS};
    DistributionPolicy       proposed_distribution_policy{DistributionPolicy::R1_TRUST_RING_COMMONS};
    TaskReleaseRing          proposed_task_release_ring{TaskReleaseRing::R0_STEWARD_ONLY};
    DecisionStatus           status{DecisionStatus::PENDING};
    [[maybe_unused]] uint8_t pad_[4]{0};
};
static_assert(alignof(L6PendingDecision) == 8, "L6PendingDecision must be 8-byte aligned.");

// ============================================================================
// Layer 5: Polycentric Node Governance Asset Model
// ============================================================================

struct alignas(8) L5GovernedAsset {
    std::string              asset_id{};
    std::string              title{};
    std::string              governance_badge{};
    std::string              l7_fallback_channel{};
    std::string              local_commons_channel{};
    std::vector<std::string> linked_need_ids{};
    std::vector<std::string> workflow_ids{};
    uint64_t                 maintenance_exergy_uj{0};
    int64_t                  monthly_l7_offset_potential_cents{0};
    uint32_t                 steward_reputation_credit_bp{150};
    SourcingMode             sourcing_mode{SourcingMode::L7_LEGACY_CHANNEL};
    DistributionPolicy       distribution_policy{DistributionPolicy::R1_TRUST_RING_COMMONS};
    TaskReleaseRing          task_release_ring{TaskReleaseRing::R0_STEWARD_ONLY};
    uint8_t                  local_workflows_instantiated{0};
};
static_assert(alignof(L5GovernedAsset) == 8, "L5GovernedAsset must be 8-byte aligned.");

// ============================================================================
// Layer 4: Real-Life Multi-Workflow Orchestrator & Personal Cluster Node Models
// ============================================================================

struct alignas(8) HomesteadClusterNode {
    std::string node_id{};
    std::string node_did{};
    std::string title{};
    std::string zone_label{};
    std::string daemons_hosted{};
    std::string role_summary{};
    std::string active_token_summary{};
    std::string inventory_summary{};
    uint64_t    accounted_exergy_uj{0};
    uint32_t    active_workflow_count{0};
    uint32_t    completed_task_count{0};
    uint32_t    online_l2_pin_count{0};
    uint8_t     is_online{1};
    [[maybe_unused]] uint8_t pad_[3]{0};
};
static_assert(alignof(HomesteadClusterNode) == 8, "HomesteadClusterNode must be 8-byte aligned.");

struct alignas(8) L4TaskItem {
    std::string           task_id{};
    std::string           workflow_id{};
    std::string           asset_id{};
    std::string           assigned_node_id{};
    std::string           mapped_affordance_id{};
    std::string           precondition_blocker_note{};
    std::string           title{};
    std::string           location_label{};
    std::string           l2_sensor_register_id{};
    std::string           l2_actuator_register_id{};
    std::string           claimed_by_cluster_id{};
    std::string           unmodeled_prompt_question{};
    std::string           unmodeled_prompt_answer{};
    uint64_t              exergy_cost_uj{0};
    int64_t               l7_revenue_offset_cents{0};
    uint64_t              last_state_change_tick{0};
    uint64_t              timer_due_sim_s{0};
    uint64_t              timer_remaining_sim_s{0};
    uint32_t              step_index{1};
    uint32_t              oasis_action_code{0};
    uint32_t              target_temp_min_mk{0};
    uint32_t              target_moisture_max_bp{0};
    uint32_t              timer_interval_sim_s{0};
    uint32_t              verification_cycle_index{0};
    TaskCommitmentState   task_commitment_state{TaskCommitmentState::UNASSIGNED};
    TaskExecutionModality execution_modality{TaskExecutionModality::MANUAL_HUMAN_OBSERVATION};
    HaulModality          haul_modality{HaulModality::NONE};
    BpmnTokenState        bpmn_token_state{BpmnTokenState::WAITING_SEQUENCE_FLOW};
    uint8_t               has_unmodeled_micro_detail{0};
    uint8_t               unmodeled_prompt_resolved{0};
    [[maybe_unused]] uint8_t pad_[2]{0};
};
static_assert(alignof(L4TaskItem) == 8, "L4TaskItem must be 8-byte aligned.");

struct alignas(8) L4WorkflowInstance {
    std::string             workflow_id{};
    std::string             asset_id{};
    std::string             assigned_node_id{};
    std::string             title{};
    std::string             input_cultivar_id{};
    std::string             input_cultivar_title{};
    std::string             species_binomial{};
    std::string             phenology_stage_label{};
    std::vector<L4TaskItem> tasks{};
    uint64_t                spawned_tick{0};
    uint64_t                crop_sown_sim_s{0};
    uint64_t                last_growth_eval_sim_s{0};
    uint64_t                next_verification_due_sim_s{0};
    uint32_t                completed_cycles{0};
    uint32_t                crop_growth_bp{0};
    uint32_t                harvest_mature_bp{9500};
    uint32_t                maturity_sim_seconds{0};
    uint32_t                verification_interval_sim_s{0};
    uint32_t                estimated_moisture_bp{3200};
    uint32_t                estimated_nitrogen_ppm{48};
    uint32_t                growth_sub_accum{0};
    uint32_t                moisture_et_sub_accum{0};
    uint32_t                nitrogen_sub_accum{0};
    uint16_t                verification_checks_completed{0};
    uint16_t                auto_l2_checks_completed{0};
    uint8_t                 is_active{1};
    uint8_t                 crop_sown{0};
    uint8_t                 is_virtual_cultivar{0};
    uint8_t                 fixes_atmospheric_n2{1};
};
static_assert(alignof(L4WorkflowInstance) == 8, "L4WorkflowInstance must be 8-byte aligned.");

// ============================================================================
// Layer 3 & Layer 7: Pairwise BCC Exergy Ledger & Zero-Bill Offset Accounting
// ============================================================================

struct alignas(8) L3PeerBccRecord {
    std::string peer_cluster_id{};
    std::string peer_cluster_title{};
    int64_t     balance_local_to_peer_uj{0};
    int64_t     balance_peer_to_local_uj{0};
    int64_t     credit_limit_uj{200000000000LL}; // 200 MJ credit window in uJ
    uint64_t    total_cleared_volume_uj{0};
    uint32_t    completed_mutual_tasks{0};
    uint32_t    household_count{4};

    [[nodiscard]] constexpr bool IsZeroSumValid() const noexcept {
        return (balance_local_to_peer_uj + balance_peer_to_local_uj) == 0;
    }

    [[nodiscard]] constexpr bool SettleSignedDelta(int64_t delta_local_to_peer_uj) noexcept {
        if (!IsZeroSumValid() || credit_limit_uj < 0) {
            return false;
        }
        const int64_t next_local = balance_local_to_peer_uj + delta_local_to_peer_uj;
        const int64_t next_peer  = balance_peer_to_local_uj - delta_local_to_peer_uj;
        if (next_local > credit_limit_uj || next_local < -credit_limit_uj ||
            next_peer > credit_limit_uj  || next_peer < -credit_limit_uj) {
            return false;
        }
        balance_local_to_peer_uj = next_local;
        balance_peer_to_local_uj = next_peer;
        const uint64_t abs_delta = (delta_local_to_peer_uj >= 0)
                                       ? static_cast<uint64_t>(delta_local_to_peer_uj)
                                       : static_cast<uint64_t>(-delta_local_to_peer_uj);
        total_cleared_volume_uj += abs_delta;
        return IsZeroSumValid();
    }
};
static_assert(alignof(L3PeerBccRecord) == 8, "L3PeerBccRecord must be 8-byte aligned.");

struct alignas(8) L3MeshWeatherEvent {
    std::string event_id{};
    std::string cluster_id{};
    std::string cluster_title{};
    std::string event_category{};
    std::string summary{};
    uint64_t    exergy_delta_uj{0};
    uint64_t    tick{0};
};
static_assert(alignof(L3MeshWeatherEvent) == 8, "L3MeshWeatherEvent must be 8-byte aligned.");

struct alignas(8) L3SurplusOffer {
    std::string offer_id{};
    std::string cluster_id{};
    std::string item_title{};
    uint64_t    exergy_value_uj{0};
    uint64_t    posted_tick{0};
    uint32_t    quantity_units{1};
    uint8_t     is_available{1};
    [[maybe_unused]] uint8_t pad_[3]{0};
};
static_assert(alignof(L3SurplusOffer) == 8, "L3SurplusOffer must be 8-byte aligned.");

struct alignas(8) L3NodeLedger {
    std::vector<L3PeerBccRecord>    peer_bcc_channels{};
    std::vector<L3MeshWeatherEvent> mesh_weather_feed{};
    std::vector<L3SurplusOffer>     peer_surplus_offers{};
    uint64_t total_exergy_accounted_uj{30600000000ULL}; // 30.6 MJ daily solar baseload
    int64_t  household_reserve_cents{2000000};          // $20,000.00 Genesis Reserve
    int64_t  monthly_legacy_bills_cents{22000};         // $220.00/mo after R0 baseline enrollment
    int64_t  monthly_l7_offset_revenue_cents{0};        // L7 custom woodwork / surplus offsets
    uint32_t dependency_ratio_bp{2200};                 // 22.00% residual L7 dependency
    uint32_t steward_reputation_bp{650};                // Steward reputational wealth (bp)
    uint32_t completed_l7_offset_orders{0};
    uint8_t  r0_baseline_enrolled{1};
    [[maybe_unused]] uint8_t pad_[3]{0};

    [[nodiscard]] constexpr int64_t NetMonthlyCashflowCents() const noexcept {
        return monthly_l7_offset_revenue_cents - monthly_legacy_bills_cents;
    }

    [[nodiscard]] constexpr bool IsZeroNetBillAchieved() const noexcept {
        return monthly_l7_offset_revenue_cents >= monthly_legacy_bills_cents;
    }

    [[nodiscard]] bool VerifyPairwiseZeroSumInvariant() const noexcept {
        int64_t pair_sum = 0;
        for (const auto& ch : peer_bcc_channels) {
            if (!ch.IsZeroSumValid()) {
                return false;
            }
            pair_sum += (ch.balance_local_to_peer_uj + ch.balance_peer_to_local_uj);
        }
        return pair_sum == 0;
    }
};
static_assert(alignof(L3NodeLedger) == 8, "L3NodeLedger must be 8-byte aligned.");

// ============================================================================
// Single-Source-of-Truth Native Sovereign Stack Runtime
// ============================================================================

class alignas(8) SharedStackRuntime {
public:
    explicit SharedStackRuntime(const std::string& jsonld_hint_path = "") {
        Bootstrap(jsonld_hint_path);
    }

    // ------------------------------------------------------------------------
    // Bootstrap & Lifecycle
    // ------------------------------------------------------------------------

    void Bootstrap(const std::string& jsonld_hint_path = "") {
        needs_.clear();
        requests_.clear();
        pending_decisions_.clear();
        governed_assets_.clear();
        workflows_.clear();
        last_search_results_.clear();
        cluster_nodes_.clear();
        cultivars_.clear();

        l2_telemetry_ = L2TelemetryTable{};
        ledger_ = L3NodeLedger{};
        current_tick_ = 1ULL;
        custom_counter_ = 0ULL;
        sim_clock_seconds_ = 28800ULL; // Day 1, 08:00:00
        sim_day_ = 1U;
        ambient_temp_override_mk_ = 0U;
        cultivars_enriched_count_ = 0U;
        raised_bed_frames_built_ = 0U;
        compost_kiln_units_built_ = 0U;
        poultry_coops_built_ = 0U;
        harvested_split_pea_batches_ = 0U;
        harvested_mirepoix_batches_ = 0U;
        poultry_bones_and_eggs_batches_ = 0U;
        thermophilic_compost_batches_ = 0U;
        biochar_batches_ = 0U;
        simmered_bone_broth_batches_ = 0U;
        served_pea_soup_tiffin_batches_ = 0U;
        bootstrapped_from_jsonld_ = 0U;
        external_r1_swarm_enabled_ = 0U; // Default: Solo Steward 4-Node Personal Homestead Cluster
        bootstrap_source_path_ = "builtin:pea_to_soup_canonical_v1";
        selected_garden_cultivar_id_ = "cultivar:chrono_virtual_pea";
        last_command_action_ = "bootstrap";
        last_command_status_ = "OK";

        InitializeHomesteadClusterNodes();
        InitializeCanonicalCultivars();
        TryEnrichCultivarsFromJsonLd();
        InitializeGovernedAssets();
        InitializeCanonicalNeeds();
        TryEnrichNeedsFromJsonLd(jsonld_hint_path);
        InitializeGenesisDecisionsAndWorkflows();
        EvaluateBpmnTokensAndDependencies();
        RefreshL4AutomationModalities();
        RefreshClusterNodeSummaries();
    }

    void AdvanceToTick(uint64_t tick) noexcept {
        if (tick > current_tick_) {
            current_tick_ = tick;
            sim_day_ = std::max(sim_day_, 1U + static_cast<uint32_t>(sim_clock_seconds_ / 86400ULL));
        }
    }

    void AdvanceSimClockSeconds(uint64_t delta_sim_s) {
        if (delta_sim_s == 0ULL) {
            EvaluateBpmnTokensAndDependencies();
            RefreshL4AutomationModalities();
            RefreshClusterNodeSummaries();
            return;
        }
        sim_clock_seconds_ += delta_sim_s;
        sim_day_ = 1U + static_cast<uint32_t>(sim_clock_seconds_ / 86400ULL);
        ++current_tick_;
        EvaluateBpmnTokensAndDependencies();
        RefreshL4AutomationModalities();
        RefreshClusterNodeSummaries();
    }

    void SyncSimClockSeconds(uint64_t total_sim_s) {
        if (total_sim_s > sim_clock_seconds_) {
            sim_clock_seconds_ = total_sim_s;
            sim_day_ = 1U + static_cast<uint32_t>(sim_clock_seconds_ / 86400ULL);
        }
        EvaluateBpmnTokensAndDependencies();
        RefreshL4AutomationModalities();
        RefreshClusterNodeSummaries();
    }

    void SetAmbientTempOverrideMk(uint32_t temp_mk) noexcept {
        ambient_temp_override_mk_ = temp_mk;
    }

    [[nodiscard]] uint32_t CurrentAmbientTempMk() const noexcept {
        if (ambient_temp_override_mk_ > 0U) {
            return ambient_temp_override_mk_;
        }
        if (const L2SensorRegister* soil = l2_telemetry_.FindRegister("soil_probe_v1")) {
            if (soil->is_online != 0U && soil->temperature_mk > 0U) {
                return soil->temperature_mk;
            }
        }
        // Pure-integer diurnal ambient temperature curve: 287150 mK (14C night) .. 296150 mK (23C afternoon)
        const uint64_t rem_day_s = sim_clock_seconds_ % 86400ULL;
        const uint64_t shifted_s = (rem_day_s + 86400ULL - 14400ULL) % 86400ULL;
        const uint64_t dist_s    = (shifted_s <= 43200ULL) ? shifted_s : (86400ULL - shifted_s);
        return 287150U + static_cast<uint32_t>((9000ULL * dist_s) / 43200ULL);
    }

    [[nodiscard]] uint64_t CurrentTick() const noexcept { return current_tick_; }
    [[nodiscard]] uint64_t SimClockSeconds() const noexcept { return sim_clock_seconds_; }
    [[nodiscard]] uint32_t CurrentSimDay() const noexcept { return sim_day_; }
    [[nodiscard]] bool BootstrappedFromJsonLd() const noexcept { return bootstrapped_from_jsonld_ != 0U; }
    [[nodiscard]] const std::string& BootstrapSourcePath() const noexcept { return bootstrap_source_path_; }
    [[nodiscard]] const std::string& SelectedGardenCultivarId() const noexcept { return selected_garden_cultivar_id_; }
    [[nodiscard]] const std::vector<CultivarWorkflowSpec>& GetAvailableCultivars() const noexcept { return cultivars_; }
    [[nodiscard]] bool IsExternalR1SwarmEnabled() const noexcept { return external_r1_swarm_enabled_ != 0U; }

    [[nodiscard]] const CultivarWorkflowSpec* FindCultivar(std::string_view cultivar_id) const noexcept {
        if (cultivar_id.empty()) {
            return nullptr;
        }
        for (const auto& c : cultivars_) {
            if (c.cultivar_id == cultivar_id ||
                (c.cultivar_id.rfind("cultivar:", 0U) == 0U &&
                 std::string_view(c.cultivar_id).substr(9U) == cultivar_id)) {
                return &c;
            }
        }
        return nullptr;
    }

    bool SetGardenCultivar(std::string_view cultivar_id) {
        const CultivarWorkflowSpec* spec = FindCultivar(cultivar_id);
        if (spec == nullptr) {
            return false;
        }
        selected_garden_cultivar_id_ = spec->cultivar_id;
        for (auto& wf : workflows_) {
            if (wf.workflow_id == "wf:pea_cultivation" && wf.crop_sown == 0U) {
                ApplyCultivarParametersToWorkflow(wf, *spec);
            }
        }
        EvaluateBpmnTokensAndDependencies();
        RefreshL4AutomationModalities();
        RefreshClusterNodeSummaries();
        return true;
    }

    void SetExternalR1SwarmEnabled(bool enabled) {
        external_r1_swarm_enabled_ = enabled ? 1U : 0U;
        if (enabled) {
            if (ledger_.peer_bcc_channels.empty()) {
                InitializePeerBccChannels();
            }
            EmitLocalClusterChoreographyEvent(
                "node-01:office-coordinator",
                "Node 01 — Office Sovereign Coordinator",
                "R1_SWARM_ENABLED",
                "External R1 neighborhood peer-cluster swarm enabled alongside the 4-node Lot 402 cluster.",
                0ULL
            );
        } else {
            // Recall any unassigned external R1 releases back to the solo Steward's R0 cluster
            for (auto& wf : workflows_) {
                for (auto& t : wf.tasks) {
                    if (t.task_commitment_state == TaskCommitmentState::RELEASED_TO_R1_PEERS) {
                        t.task_commitment_state = TaskCommitmentState::UNASSIGNED;
                        t.claimed_by_cluster_id.clear();
                        t.last_state_change_tick = current_tick_;
                    }
                }
            }
            EmitLocalClusterChoreographyEvent(
                "node-01:office-coordinator",
                "Node 01 — Office Sovereign Coordinator",
                "SOLO_CLUSTER_MODE",
                "External R1 peer swarm paused; running strictly on the solo Steward's 4-node Homestead Cluster.",
                0ULL
            );
        }
        EvaluateBpmnTokensAndDependencies();
        RefreshClusterNodeSummaries();
    }

    void StepLocalHomesteadClusterHeartbeat(uint64_t tick) {
        AdvanceToTick(tick);
        EvaluateBpmnTokensAndDependencies();
        RefreshClusterNodeSummaries();
    }

    // ------------------------------------------------------------------------
    // Direct State Accessors
    // ------------------------------------------------------------------------

    [[nodiscard]] const std::vector<L6NeedRecord>& GetNeeds() const noexcept { return needs_; }
    [[nodiscard]] const std::vector<L6IntentRequest>& GetIntentRequests() const noexcept { return requests_; }
    [[nodiscard]] const std::vector<L6PendingDecision>& GetPendingDecisions() const noexcept { return pending_decisions_; }
    [[nodiscard]] const std::vector<L5GovernedAsset>& GetGovernedAssets() const noexcept { return governed_assets_; }
    [[nodiscard]] const std::vector<L4WorkflowInstance>& GetWorkflows() const noexcept { return workflows_; }
    [[nodiscard]] std::vector<L4WorkflowInstance>& GetWorkflowsMutable() noexcept { return workflows_; }
    [[nodiscard]] const std::vector<HomesteadClusterNode>& GetClusterNodes() const noexcept { return cluster_nodes_; }
    [[nodiscard]] const L3NodeLedger& GetLedger() const noexcept { return ledger_; }
    [[nodiscard]] L3NodeLedger& GetLedgerMutable() noexcept { return ledger_; }
    [[nodiscard]] const L2TelemetryTable& GetL2TelemetryTable() const noexcept { return l2_telemetry_; }
    [[nodiscard]] L2TelemetryTable& GetL2TelemetryTableMutable() noexcept { return l2_telemetry_; }
    [[nodiscard]] uint32_t RaisedBedFramesBuilt() const noexcept { return raised_bed_frames_built_; }
    [[nodiscard]] uint32_t CompostKilnUnitsBuilt() const noexcept { return compost_kiln_units_built_; }
    [[nodiscard]] uint32_t PoultryCoopsBuilt() const noexcept { return poultry_coops_built_; }
    [[nodiscard]] uint32_t HarvestedSplitPeaBatches() const noexcept { return harvested_split_pea_batches_; }
    [[nodiscard]] uint32_t HarvestedMirepoixBatches() const noexcept { return harvested_mirepoix_batches_; }

    // ------------------------------------------------------------------------
    // L6 Needs & Intent Commons Operations
    // ------------------------------------------------------------------------

    [[nodiscard]] std::vector<L6NeedRecord> SearchNeeds(std::string_view query) const {
        if (query.empty()) {
            return needs_;
        }
        const std::string needle = json_util::ToLowerAscii(query);
        std::vector<L6NeedRecord> matches;
        for (const auto& need : needs_) {
            if (json_util::ToLowerAscii(need.need_id).find(needle) != std::string::npos ||
                json_util::ToLowerAscii(need.title).find(needle) != std::string::npos ||
                json_util::ToLowerAscii(need.category).find(needle) != std::string::npos ||
                json_util::ToLowerAscii(need.description).find(needle) != std::string::npos) {
                matches.push_back(need);
            }
        }
        return matches;
    }

    L6IntentRequest RequestNeed(
        const std::string& need_id_or_title,
        NeedTimingMode timing_mode = NeedTimingMode::NOW,
        uint32_t due_sim_day = 0U,
        uint32_t quantity_units = 1U,
        const std::string& requester_did = "did:mesh:node01:steward"
    ) {
        L6NeedRecord* need = FindNeedMutable(need_id_or_title);
        if (need == nullptr && !needs_.empty()) {
            need = &needs_.front();
        }

        const uint32_t qty = (quantity_units == 0U) ? 1U : quantity_units;
        const uint32_t resolved_due_day =
            (timing_mode == NeedTimingMode::SCHEDULED_FUTURE && due_sim_day == 0U)
                ? (sim_day_ + 1U)
                : ((due_sim_day == 0U) ? sim_day_ : due_sim_day);

        L6IntentRequest req{};
        req.request_id = "req:" + std::to_string(requests_.size() + 1U);
        req.requester_did = requester_did;
        req.created_tick = current_tick_;
        req.due_sim_day = resolved_due_day;
        req.quantity_units = qty;
        req.timing_mode = timing_mode;

        if (need != nullptr) {
            ++need->request_count;
            req.need_id = need->need_id;
            req.need_title = need->title;
            req.exergy_budget_uj = need->exergy_cost_uj * static_cast<uint64_t>(qty);
            req.resolved_sourcing_mode = need->active_sourcing_mode;

            if (need->active_sourcing_mode == SourcingMode::L7_LEGACY_CHANNEL) {
                const int64_t total_cost_cents = need->l7_unit_cost_cents * static_cast<int64_t>(qty);
                if (timing_mode == NeedTimingMode::NOW) {
                    if (requester_did.rfind("cluster:", 0U) != 0U && total_cost_cents > 0) {
                        ledger_.household_reserve_cents =
                            std::max<int64_t>(0, ledger_.household_reserve_cents - total_cost_cents);
                    }
                    req.fulfilled = 1U;
                    req.fulfillment_note = "Fulfilled via L7 fallback (" + need->l7_channel_label +
                                           "); approve L5 governance decision to transition local.";
                    need->last_status_note = req.fulfillment_note;
                } else {
                    req.fulfilled = 0U;
                    req.fulfillment_note = "Scheduled for Sim Day " + std::to_string(resolved_due_day) +
                                           " (approve L5 decision to fulfill locally at $0).";
                    need->last_status_note = req.fulfillment_note;
                }
                EnsurePendingDecisionForAsset(need->governed_asset_id, need->need_id);
            } else {
                // Local R0 workflows or R1 mesh peers — instantiate only the specific BPMN workflow needed
                ledger_.total_exergy_accounted_uj += need->exergy_cost_uj * static_cast<uint64_t>(qty);
                if (need->need_id == "need:raised_bed_frame") {
                    EnsureSpecificWorkflowSpawned("wf:fab_raised_bed");
                } else if (need->governed_asset_id == "asset:water_and_power") {
                    EnsureSpecificWorkflowSpawned("wf:water_and_power");
                } else {
                    EnsureWorkflowsSpawnedForAsset(need->governed_asset_id);
                }
                EvaluateBpmnTokensAndDependencies();
                RefreshClusterNodeSummaries();

                if (timing_mode == NeedTimingMode::SCHEDULED_FUTURE) {
                    req.fulfilled = 0U;
                    req.fulfillment_note = "Scheduled on local L4 workflow (" + need->local_channel_label +
                                           ") for Sim Day " + std::to_string(resolved_due_day) + ".";
                } else {
                    req.fulfilled = 1U;
                    req.fulfillment_note = "Dispatched to local L5/L4 commons (" + need->local_channel_label +
                                           ", $0 L7 fiat spend).";
                }
                need->last_status_note = req.fulfillment_note;
            }
        }

        requests_.push_back(req);
        return req;
    }

    L6NeedRecord DefineNewMeshNeed(
        const std::string& title,
        const std::string& category,
        const std::string& l7_channel_label,
        uint64_t exergy_cost_uj,
        int64_t l7_unit_cost_cents = 1500,
        const std::string& local_channel_label = "Local L5 Stewarded Workflow ($0.00)"
    ) {
        ++custom_counter_;
        const std::string slug = MakeSlug(title) + "_" + std::to_string(custom_counter_);
        const std::string need_id = "need:" + slug;
        const std::string asset_id = "asset:" + slug;
        const std::string decision_id = "dec:" + slug;

        L5GovernedAsset asset{};
        asset.asset_id = asset_id;
        asset.title = title + " Commons Asset";
        asset.governance_badge = "L5 Polycentric Mesh Asset";
        asset.l7_fallback_channel = l7_channel_label.empty() ? "L7 Legacy Market Fallback" : l7_channel_label;
        asset.local_commons_channel = local_channel_label;
        asset.linked_need_ids.push_back(need_id);
        asset.maintenance_exergy_uj = (exergy_cost_uj > 0ULL) ? exergy_cost_uj : 2500000000ULL;
        asset.monthly_l7_offset_potential_cents = (l7_unit_cost_cents > 0) ? (l7_unit_cost_cents * 4) : 6000;
        asset.steward_reputation_credit_bp = 180U;
        asset.sourcing_mode = SourcingMode::L7_LEGACY_CHANNEL;
        asset.distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
        asset.task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
        asset.local_workflows_instantiated = 0U;
        governed_assets_.push_back(asset);

        L6NeedRecord rec{};
        rec.need_id = need_id;
        rec.title = title.empty() ? ("Custom Mesh Need " + std::to_string(custom_counter_)) : title;
        rec.category = category.empty() ? "Community Defined Commons" : category;
        rec.description = "User-defined mesh semantic need bound initially to L7 fallback pending L5 local workflow ratification.";
        rec.l7_channel_label = asset.l7_fallback_channel;
        rec.local_channel_label = local_channel_label;
        rec.governed_asset_id = asset_id;
        rec.last_status_note = "Registered on L6 Agora with L7 fallback; L5 local production decision emitted.";
        rec.exergy_cost_uj = (exergy_cost_uj > 0ULL) ? exergy_cost_uj : 2500000000ULL;
        rec.l7_unit_cost_cents = (l7_unit_cost_cents >= 0) ? l7_unit_cost_cents : 1500;
        rec.action_code = 18U;
        rec.request_count = 0U;
        rec.active_sourcing_mode = SourcingMode::L7_LEGACY_CHANNEL;
        rec.is_r0_baseline_care = 0U;
        rec.is_custom_mesh_need = 1U;
        needs_.push_back(rec);

        L6PendingDecision dec{};
        dec.decision_id = decision_id;
        dec.title = "L5 Proposal: Instantiate Local L4 Production Workflows for " + rec.title;
        dec.governed_asset_id = asset_id;
        dec.rationale = "L5 Governance detected new mesh need '" + rec.title + "' currently bound to " +
                        rec.l7_channel_label + ". Ratify local L4 workflow instantiation to internalize production.";
        dec.linked_need_ids.push_back(need_id);
        dec.estimated_exergy_savings_uj = rec.exergy_cost_uj / 2ULL;
        dec.estimated_monthly_l7_savings_cents = asset.monthly_l7_offset_potential_cents;
        dec.emitted_tick = current_tick_;
        dec.proposed_sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
        dec.proposed_distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
        dec.proposed_task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
        dec.status = DecisionStatus::PENDING;
        pending_decisions_.push_back(dec);

        return rec;
    }

    // ------------------------------------------------------------------------
    // L5 Polycentric Node Governance Operations
    // ------------------------------------------------------------------------

    bool ApproveOrAdaptGovernanceDecision(
        const std::string& decision_id,
        bool approve = true
    ) {
        L6PendingDecision* dec = FindDecisionMutable(decision_id);
        if (dec == nullptr) {
            return false;
        }
        return ApproveOrAdaptGovernanceDecision(
            decision_id,
            approve,
            dec->proposed_sourcing_mode,
            dec->proposed_distribution_policy,
            dec->proposed_task_release_ring,
            false
        );
    }

    bool ApproveOrAdaptGovernanceDecision(
        const std::string& decision_id,
        bool approve,
        SourcingMode sourcing_mode,
        DistributionPolicy distribution_policy,
        TaskReleaseRing task_release_ring,
        bool was_adapted = true
    ) {
        L6PendingDecision* dec = FindDecisionMutable(decision_id);
        if (dec == nullptr) {
            return false;
        }

        if (!approve) {
            dec->status = DecisionStatus::DEFERRED;
            return true;
        }

        const bool params_changed =
            was_adapted ||
            (sourcing_mode != dec->proposed_sourcing_mode) ||
            (distribution_policy != dec->proposed_distribution_policy) ||
            (task_release_ring != dec->proposed_task_release_ring);

        dec->proposed_sourcing_mode = sourcing_mode;
        dec->proposed_distribution_policy = distribution_policy;
        dec->proposed_task_release_ring = task_release_ring;
        dec->status = params_changed ? DecisionStatus::ADAPTED_AND_APPROVED : DecisionStatus::APPROVED;

        const std::string target_asset_id = dec->governed_asset_id;
        const bool ok = ApplyGovernancePolicyToAsset(
            target_asset_id,
            sourcing_mode,
            distribution_policy,
            task_release_ring
        );
        if (ok && decision_id == "dec:garage_l7_woodwork" &&
            sourcing_mode != SourcingMode::L7_LEGACY_CHANNEL) {
            EnsureSpecificWorkflowSpawned("wf:fab_l7_woodwork");
            EvaluateBpmnTokensAndDependencies();
            RefreshClusterNodeSummaries();
        }
        return ok;
    }

    bool AdaptAssetGovernancePolicy(
        const std::string& asset_or_decision_id,
        SourcingMode sourcing_mode,
        DistributionPolicy distribution_policy,
        TaskReleaseRing task_release_ring
    ) {
        if (FindDecisionMutable(asset_or_decision_id) != nullptr) {
            return ApproveOrAdaptGovernanceDecision(
                asset_or_decision_id,
                true,
                sourcing_mode,
                distribution_policy,
                task_release_ring,
                true
            );
        }
        return ApplyGovernancePolicyToAsset(
            asset_or_decision_id,
            sourcing_mode,
            distribution_policy,
            task_release_ring
        );
    }

    // ------------------------------------------------------------------------
    // L4 Multi-Workflow BPMN Token Lifecycle Operations
    // ------------------------------------------------------------------------

    bool CommitTask(const std::string& task_id) {
        EvaluateBpmnTokensAndDependencies();
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr || task->bpmn_token_state != BpmnTokenState::ACTIVE_TOKEN) {
            return false;
        }
        task->task_commitment_state = TaskCommitmentState::COMMITTED_BY_PLAYER;
        task->claimed_by_cluster_id.clear();
        task->last_state_change_tick = current_tick_;
        RefreshClusterNodeSummaries();
        return true;
    }

    bool ReleaseTaskToR1Peers(const std::string& task_id) {
        if (!IsExternalR1SwarmEnabled()) {
            return false;
        }
        EvaluateBpmnTokensAndDependencies();
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr || task->bpmn_token_state != BpmnTokenState::ACTIVE_TOKEN) {
            return false;
        }
        task->task_commitment_state = TaskCommitmentState::RELEASED_TO_R1_PEERS;
        task->claimed_by_cluster_id.clear();
        task->last_state_change_tick = current_tick_;
        RefreshClusterNodeSummaries();
        return true;
    }

    bool ClaimTaskByPeerCluster(const std::string& task_id, const std::string& cluster_id) {
        if (!IsExternalR1SwarmEnabled()) {
            return false;
        }
        EvaluateBpmnTokensAndDependencies();
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr ||
            task->bpmn_token_state != BpmnTokenState::ACTIVE_TOKEN ||
            task->task_commitment_state != TaskCommitmentState::RELEASED_TO_R1_PEERS) {
            return false;
        }
        task->task_commitment_state = TaskCommitmentState::CLAIMED_BY_PEER_CLUSTER;
        task->claimed_by_cluster_id = cluster_id;
        task->last_state_change_tick = current_tick_;
        RefreshClusterNodeSummaries();
        return true;
    }

    bool CompleteTask(
        const std::string& task_id,
        const std::string& completed_by_cluster_id = "",
        uint32_t observed_growth_bp = 0U,
        uint32_t observed_moisture_bp = 0U
    ) {
        EvaluateBpmnTokensAndDependencies();
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr || task->bpmn_token_state != BpmnTokenState::ACTIVE_TOKEN) {
            return false;
        }

        const std::string parent_wf_id = task->workflow_id;
        const std::string assigned_node = task->assigned_node_id;
        const uint64_t task_exergy = task->exergy_cost_uj;
        const int64_t l7_cents = task->l7_revenue_offset_cents;

        // Special BPMN IntermediateCatchEvent + ExclusiveGateway loop for Step 4 Timed Verification & Watering:
        // Completing a check before crop_growth_bp >= harvest_mature_bp re-arms WAITING_TIMER instead of unlocking Harvest!
        if (task_id == "task:pea_4_irrigate_weed") {
            if (L4WorkflowInstance* wf = FindWorkflowMutable(parent_wf_id)) {
                const CultivarWorkflowSpec spec = ResolveCultivarSpecOrDefault(wf->input_cultivar_id);
                if (observed_growth_bp > wf->crop_growth_bp) {
                    wf->crop_growth_bp = std::min(10000U, observed_growth_bp);
                }
                wf->estimated_moisture_bp = (observed_moisture_bp > 0U)
                                                ? std::max(observed_moisture_bp, spec.target_opt_vwc_bp)
                                                : spec.target_opt_vwc_bp;
                ++wf->verification_checks_completed;
                wf->phenology_stage_label = ComputePhenologyLabel(
                    wf->crop_growth_bp, wf->crop_sown, spec.harvest_mature_bp);

                ledger_.total_exergy_accounted_uj += task_exergy;
                AccountNodeTaskCompletion(assigned_node, task_exergy);
                if (completed_by_cluster_id.empty()) {
                    ledger_.steward_reputation_bp =
                        std::min<uint32_t>(10000U, ledger_.steward_reputation_bp + 50U);
                }

                if (wf->crop_growth_bp < spec.harvest_mature_bp) {
                    wf->next_verification_due_sim_s =
                        sim_clock_seconds_ + static_cast<uint64_t>(spec.verification_interval_sim_s);
                    task->task_commitment_state = TaskCommitmentState::UNASSIGNED;
                    task->claimed_by_cluster_id.clear();
                    task->bpmn_token_state = BpmnTokenState::WAITING_TIMER;
                    task->timer_due_sim_s = wf->next_verification_due_sim_s;
                    task->timer_remaining_sim_s = spec.verification_interval_sim_s;
                    task->verification_cycle_index =
                        static_cast<uint32_t>(wf->verification_checks_completed) + 1U;
                    task->last_state_change_tick = current_tick_;

                    EmitLocalClusterChoreographyEvent(
                        "node-03:garden-agro",
                        "Node 03 — Backyard Agroecology & Soil Node",
                        "AGRO_TIMED_VERIFICATION",
                        "Completed verification & watering check #" +
                            std::to_string(wf->verification_checks_completed) + " on " + spec.title +
                            " (" + std::to_string(wf->crop_growth_bp / 100U) + "% maturity · " +
                            wf->phenology_stage_label + "); next BPMN timer check in " +
                            FormatSimDurationCompact(spec.verification_interval_sim_s) + ".",
                        task_exergy
                    );

                    EvaluateBpmnTokensAndDependencies();
                    RefreshL4AutomationModalities();
                    RefreshClusterNodeSummaries();
                    return true;
                }

                // Maturity reached (>= harvest_mature_bp): exit loop and mark Step 4 COMPLETED!
                task->task_commitment_state = TaskCommitmentState::COMPLETED;
                task->bpmn_token_state = BpmnTokenState::COMPLETED;
                task->timer_remaining_sim_s = 0ULL;
                task->precondition_blocker_note.clear();
                if (!completed_by_cluster_id.empty()) {
                    task->claimed_by_cluster_id = completed_by_cluster_id;
                }
                task->last_state_change_tick = current_tick_;
                EmitLocalClusterChoreographyEvent(
                    "node-03:garden-agro",
                    "Node 03 — Backyard Agroecology & Soil Node",
                    "AGRO_MATURITY_VERIFIED",
                    "Verified " + spec.title + " reached harvest maturity (" +
                        std::to_string(wf->crop_growth_bp / 100U) + "%); Step 5 Harvest BPMN token unlocked.",
                    task_exergy
                );
                EvaluateBpmnTokensAndDependencies();
                RefreshL4AutomationModalities();
                RefreshClusterNodeSummaries();
                return true;
            }
        }

        task->task_commitment_state = TaskCommitmentState::COMPLETED;
        task->bpmn_token_state = BpmnTokenState::COMPLETED;
        task->precondition_blocker_note.clear();
        if (!completed_by_cluster_id.empty()) {
            task->claimed_by_cluster_id = completed_by_cluster_id;
        }
        task->last_state_change_tick = current_tick_;

        ledger_.total_exergy_accounted_uj += task_exergy;
        AccountNodeTaskCompletion(assigned_node, task_exergy);

        if (completed_by_cluster_id.empty()) {
            ledger_.steward_reputation_bp =
                std::min<uint32_t>(10000U, ledger_.steward_reputation_bp + 120U);
        }
        if (l7_cents > 0) {
            ledger_.monthly_l7_offset_revenue_cents += l7_cents;
            ledger_.household_reserve_cents += l7_cents;
            ++ledger_.completed_l7_offset_orders;
            if (ledger_.dependency_ratio_bp > 300U) {
                ledger_.dependency_ratio_bp -= 300U;
            } else {
                ledger_.dependency_ratio_bp = 0U;
            }
        }

        ApplyPhysicalChoreographyOnTaskCompleted(task_id, task_exergy);

        // Check if all tasks in parent workflow are completed
        for (auto& wf : workflows_) {
            if (wf.workflow_id == parent_wf_id) {
                bool all_done = !wf.tasks.empty();
                for (const auto& step : wf.tasks) {
                    if (step.task_commitment_state != TaskCommitmentState::COMPLETED) {
                        all_done = false;
                        break;
                    }
                }
                if (all_done) {
                    ++wf.completed_cycles;
                }
                break;
            }
        }

        EvaluateBpmnTokensAndDependencies();
        RefreshL4AutomationModalities();
        RefreshClusterNodeSummaries();
        return true;
    }

    bool StartNewWorkflowCycle(const std::string& workflow_id, const std::string& optional_cultivar_id = "") {
        if (!optional_cultivar_id.empty()) {
            if (const CultivarWorkflowSpec* c = FindCultivar(optional_cultivar_id)) {
                selected_garden_cultivar_id_ = c->cultivar_id;
            }
        }
        for (auto& wf : workflows_) {
            if (wf.workflow_id != workflow_id || wf.tasks.empty()) {
                continue;
            }
            for (const auto& step : wf.tasks) {
                if (step.task_commitment_state != TaskCommitmentState::COMPLETED) {
                    return false; // Current cycle still in progress
                }
            }
            wf.crop_sown = 0U;
            wf.crop_sown_sim_s = 0ULL;
            wf.last_growth_eval_sim_s = 0ULL;
            wf.next_verification_due_sim_s = 0ULL;
            wf.crop_growth_bp = 0U;
            wf.estimated_moisture_bp = 3200U;
            wf.estimated_nitrogen_ppm = 72U;
            wf.growth_sub_accum = 0U;
            wf.moisture_et_sub_accum = 0U;
            wf.nitrogen_sub_accum = 0U;
            wf.verification_checks_completed = 0U;
            wf.auto_l2_checks_completed = 0U;
            wf.phenology_stage_label = "Unsown Bed";
            if (wf.workflow_id == "wf:pea_cultivation") {
                ApplyCultivarParametersToWorkflow(wf, ResolveCultivarSpecOrDefault(selected_garden_cultivar_id_));
            }
            const L5GovernedAsset* asset = FindAsset(wf.asset_id);
            const bool auto_release_r1 =
                IsExternalR1SwarmEnabled() &&
                (asset != nullptr) &&
                (asset->task_release_ring == TaskReleaseRing::R1_PEER_CLUSTERS);
            for (auto& step : wf.tasks) {
                step.task_commitment_state = auto_release_r1
                                                 ? TaskCommitmentState::RELEASED_TO_R1_PEERS
                                                 : TaskCommitmentState::UNASSIGNED;
                step.bpmn_token_state = BpmnTokenState::WAITING_SEQUENCE_FLOW;
                step.timer_due_sim_s = 0ULL;
                step.timer_remaining_sim_s = 0ULL;
                step.verification_cycle_index = 0U;
                step.claimed_by_cluster_id.clear();
                step.precondition_blocker_note.clear();
                step.last_state_change_tick = current_tick_;
            }
            EvaluateBpmnTokensAndDependencies();
            RefreshL4AutomationModalities();
            RefreshClusterNodeSummaries();
            return true;
        }
        return false;
    }

    bool AnswerUnmodeledMicroDetailPrompt(const std::string& task_id, const std::string& answer) {
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr || task->has_unmodeled_micro_detail == 0U || answer.empty()) {
            return false;
        }
        task->unmodeled_prompt_answer = answer;
        task->unmodeled_prompt_resolved = 1U;
        ledger_.steward_reputation_bp =
            std::min<uint32_t>(10000U, ledger_.steward_reputation_bp + 40U);
        return true;
    }

    // ------------------------------------------------------------------------
    // L2 Telemetry Ingestion & Automated L4 Modality Switching
    // ------------------------------------------------------------------------

    void RefreshL4AutomationModalities() {
        for (auto& wf : workflows_) {
            for (auto& task : wf.tasks) {
                const bool sensor_online =
                    !task.l2_sensor_register_id.empty() &&
                    l2_telemetry_.IsRegisterOnline(task.l2_sensor_register_id);
                const bool actuator_online =
                    !task.l2_actuator_register_id.empty() &&
                    l2_telemetry_.IsRegisterOnline(task.l2_actuator_register_id);

                if (sensor_online || actuator_online) {
                    task.execution_modality = TaskExecutionModality::AUTOMATED_L2_EXECUTION;
                } else {
                    task.execution_modality = TaskExecutionModality::MANUAL_HUMAN_OBSERVATION;
                }
            }
        }

        // Closed-loop L2 soil moisture -> irrigation_valve_v1 actuator control
        const L2SensorRegister* soil = l2_telemetry_.FindRegister("soil_probe_v1");
        if (soil != nullptr && soil->is_online != 0U) {
            L2SensorRegister* valve = l2_telemetry_.FindRegisterMutable("irrigation_valve_v1");
            if (valve == nullptr) {
                L2SensorRegister created_valve{};
                created_valve.register_id = "irrigation_valve_v1";
                created_valve.hardware_modality = "SOLENOID_VALVE_ACTUATOR";
                created_valve.last_seen_tick = current_tick_;
                created_valve.is_online = 1U;
                created_valve.is_actuator = 1U;
                l2_telemetry_.UpsertRegister(created_valve);
                valve = l2_telemetry_.FindRegisterMutable("irrigation_valve_v1");
            }
            if (valve != nullptr) {
                const CultivarWorkflowSpec spec = ResolveCultivarSpecOrDefault(selected_garden_cultivar_id_);
                const uint32_t open_below_bp = std::max(3200U, spec.target_min_vwc_bp + 400U);
                if (soil->moisture_bp < open_below_bp) {
                    valve->actuator_commanded_on = 1U;
                    valve->actuator_level_bp = 10000U;
                } else if (soil->moisture_bp >= spec.target_opt_vwc_bp) {
                    valve->actuator_commanded_on = 0U;
                    valve->actuator_level_bp = 0U;
                }
                valve->last_seen_tick = current_tick_;
            }
        }

        // Closed-loop Bigelow culinary lethality check (>= 347150 mK = 74.0 C for >= 120000 ms)
        const L2SensorRegister* vessel = l2_telemetry_.FindRegister("culinary_vessel_v1");
        if (vessel != nullptr && vessel->is_online != 0U &&
            vessel->temperature_mk >= 347150U && vessel->hold_duration_ms >= 120000U) {
            for (auto& wf : workflows_) {
                for (auto& task : wf.tasks) {
                    if (task.l2_sensor_register_id == "culinary_vessel_v1" &&
                        task.task_commitment_state == TaskCommitmentState::COMMITTED_BY_PLAYER) {
                        task.unmodeled_prompt_resolved = 1U;
                        if (task.unmodeled_prompt_answer.empty()) {
                            task.unmodeled_prompt_answer =
                                "Verified automatically by culinary_vessel_v1 PT100 probe (>= 347150 mK, >= 120000 ms).";
                        }
                    }
                }
            }
        }
        RefreshClusterNodeSummaries();
    }

    void IngestL2Register(const L2SensorRegister& reg) {
        L2SensorRegister copy = reg;
        if (copy.last_seen_tick == 0ULL) {
            copy.last_seen_tick = current_tick_;
        }
        l2_telemetry_.UpsertRegister(copy);
        l2_telemetry_.last_update_tick = current_tick_;
        if (copy.cumulative_exergy_uj > 0ULL) {
            ledger_.total_exergy_accounted_uj += copy.cumulative_exergy_uj;
        }
        EvaluateBpmnTokensAndDependencies();
        RefreshL4AutomationModalities();
    }

    std::string IngestL2PinsPayload(std::string_view payload) {
        ++current_tick_;

        // 0. Synchronize simulation clock seconds when forwarded by 3D engine (epistemic-safe: carries no private registers)
        if (json_util::HasKey(payload, "sim_clock_seconds")) {
            const uint64_t incoming_clock_s =
                json_util::ExtractUint64(payload, "sim_clock_seconds", sim_clock_seconds_);
            if (incoming_clock_s > sim_clock_seconds_) {
                sim_clock_seconds_ = incoming_clock_s;
                sim_day_ = 1U + static_cast<uint32_t>(sim_clock_seconds_ / 86400ULL);
            }
        }

        // 1. Nested object or flat keys for soil_probe_v1
        std::string_view soil_obj = json_util::ExtractObjectSlice(payload, "soil_probe_v1");
        if (!soil_obj.empty() || json_util::HasKey(payload, "soil_moisture_bp") ||
            json_util::ExtractString(payload, "register_id") == "soil_probe_v1") {
            std::string_view src = !soil_obj.empty() ? soil_obj : payload;
            L2SensorRegister soil{};
            if (const auto* prev = l2_telemetry_.FindRegister("soil_probe_v1")) {
                soil = *prev;
            }
            soil.register_id = "soil_probe_v1";
            soil.hardware_modality = "CAPACITIVE_SOIL_PROBE";
            soil.last_seen_tick = current_tick_;
            soil.is_actuator = 0U;
            const bool online = json_util::HasKey(src, "is_online")
                                    ? json_util::ExtractBool(src, "is_online", true)
                                    : json_util::ExtractBool(payload, "soil_probe_online", true);
            soil.is_online = online ? 1U : 0U;
            soil.moisture_bp = static_cast<uint32_t>(json_util::ExtractUint64(
                src, "moisture_bp",
                json_util::ExtractUint64(payload, "soil_moisture_bp", soil.moisture_bp)));
            soil.temperature_mk = static_cast<uint32_t>(json_util::ExtractUint64(
                src, "temperature_mk",
                json_util::ExtractUint64(payload, "soil_temp_mk",
                                         soil.temperature_mk > 0U ? soil.temperature_mk : 291150U)));
            soil.concentration_ppm = static_cast<uint32_t>(json_util::ExtractUint64(
                src, "concentration_ppm",
                json_util::ExtractUint64(src, "nitrogen_ppm", soil.concentration_ppm)));
            soil.crop_growth_bp = static_cast<uint32_t>(json_util::ExtractUint64(
                src, "crop_growth_bp",
                json_util::ExtractUint64(payload, "crop_growth_bp", soil.crop_growth_bp)));
            l2_telemetry_.UpsertRegister(soil);
        }

        // 2. Nested object or flat keys for culinary_vessel_v1
        std::string_view vessel_obj = json_util::ExtractObjectSlice(payload, "culinary_vessel_v1");
        if (!vessel_obj.empty() || json_util::HasKey(payload, "vessel_temp_mk") ||
            json_util::ExtractString(payload, "register_id") == "culinary_vessel_v1") {
            std::string_view src = !vessel_obj.empty() ? vessel_obj : payload;
            L2SensorRegister vessel{};
            if (const auto* prev = l2_telemetry_.FindRegister("culinary_vessel_v1")) {
                vessel = *prev;
            }
            vessel.register_id = "culinary_vessel_v1";
            vessel.hardware_modality = "PT100_INDUCTION_VESSEL";
            vessel.last_seen_tick = current_tick_;
            vessel.is_actuator = 0U;
            const bool online = json_util::HasKey(src, "is_online")
                                    ? json_util::ExtractBool(src, "is_online", true)
                                    : json_util::ExtractBool(payload, "culinary_vessel_online", true);
            vessel.is_online = online ? 1U : 0U;
            vessel.temperature_mk = static_cast<uint32_t>(json_util::ExtractUint64(
                src, "temperature_mk",
                json_util::ExtractUint64(payload, "vessel_temp_mk",
                                         vessel.temperature_mk > 0U ? vessel.temperature_mk : 347150U)));
            vessel.hold_duration_ms = static_cast<uint32_t>(json_util::ExtractUint64(
                src, "hold_duration_ms", vessel.hold_duration_ms));
            l2_telemetry_.UpsertRegister(vessel);
        }

        // 3. Nested object or single register_id for irrigation_valve_v1 or arbitrary register
        std::string_view valve_obj = json_util::ExtractObjectSlice(payload, "irrigation_valve_v1");
        if (!valve_obj.empty()) {
            L2SensorRegister valve{};
            if (const auto* prev = l2_telemetry_.FindRegister("irrigation_valve_v1")) {
                valve = *prev;
            }
            valve.register_id = "irrigation_valve_v1";
            valve.hardware_modality = "SOLENOID_VALVE_ACTUATOR";
            valve.last_seen_tick = current_tick_;
            valve.is_actuator = 1U;
            valve.is_online = json_util::ExtractBool(valve_obj, "is_online", true) ? 1U : 0U;
            if (json_util::HasKey(valve_obj, "actuator_level_bp")) {
                valve.actuator_level_bp =
                    static_cast<uint32_t>(json_util::ExtractUint64(valve_obj, "actuator_level_bp", 0ULL));
                valve.actuator_commanded_on = (valve.actuator_level_bp > 0U) ? 1U : 0U;
            }
            l2_telemetry_.UpsertRegister(valve);
        }

        const std::string single_reg_id = json_util::ExtractString(payload, "register_id");
        if (!single_reg_id.empty() &&
            single_reg_id != "soil_probe_v1" &&
            single_reg_id != "culinary_vessel_v1") {
            L2SensorRegister reg{};
            if (const auto* prev = l2_telemetry_.FindRegister(single_reg_id)) {
                reg = *prev;
            }
            reg.register_id = single_reg_id;
            reg.hardware_modality = json_util::ExtractString(
                payload, "hardware_modality",
                single_reg_id == "irrigation_valve_v1" ? "SOLENOID_VALVE_ACTUATOR" : "UHAI_FIELD_REGISTER");
            reg.last_seen_tick = current_tick_;
            reg.is_online = json_util::ExtractBool(payload, "is_online", true) ? 1U : 0U;
            reg.is_actuator = (single_reg_id == "irrigation_valve_v1" ||
                               json_util::ExtractBool(payload, "is_actuator", false))
                                  ? 1U
                                  : 0U;
            reg.temperature_mk = static_cast<uint32_t>(
                json_util::ExtractUint64(payload, "temperature_mk", reg.temperature_mk));
            reg.moisture_bp = static_cast<uint32_t>(
                json_util::ExtractUint64(payload, "moisture_bp", reg.moisture_bp));
            reg.concentration_ppm = static_cast<uint32_t>(
                json_util::ExtractUint64(payload, "concentration_ppm", reg.concentration_ppm));
            reg.actuator_level_bp = static_cast<uint32_t>(
                json_util::ExtractUint64(payload, "actuator_level_bp", reg.actuator_level_bp));
            reg.hold_duration_ms = static_cast<uint32_t>(
                json_util::ExtractUint64(payload, "hold_duration_ms", reg.hold_duration_ms));
            reg.crop_growth_bp = static_cast<uint32_t>(
                json_util::ExtractUint64(payload, "crop_growth_bp", reg.crop_growth_bp));
            reg.actuator_commanded_on = (reg.actuator_level_bp > 0U ||
                                         json_util::ExtractBool(payload, "actuator_commanded_on", false))
                                            ? 1U
                                            : 0U;
            l2_telemetry_.UpsertRegister(reg);
        }

        l2_telemetry_.last_update_tick = current_tick_;
        RefreshL4AutomationModalities();
        EvaluateBpmnTokensAndDependencies();
        RefreshClusterNodeSummaries();

        // Serialize L2 actuator pin command response
        const L2SensorRegister* valve = l2_telemetry_.FindRegister("irrigation_valve_v1");
        const bool valve_online = (valve != nullptr) && (valve->is_online != 0U);
        const bool valve_on = (valve != nullptr) && (valve->actuator_commanded_on != 0U);
        const uint32_t valve_bp = (valve != nullptr) ? valve->actuator_level_bp : 0U;

        std::ostringstream oss;
        oss << "{\n"
            << "  \"ok\": true,\n"
            << "  \"tick\": " << current_tick_ << ",\n"
            << "  \"irrigation_valve_v1\": {\n"
            << "    \"register_id\": \"irrigation_valve_v1\",\n"
            << "    \"is_online\": " << (valve_online ? "true" : "false") << ",\n"
            << "    \"commanded_on\": " << (valve_on ? "true" : "false") << ",\n"
            << "    \"valve_open_bp\": " << valve_bp << ",\n"
            << "    \"actuator_level_bp\": " << valve_bp << "\n"
            << "  },\n"
            << "  \"actuators\": {\n"
            << "    \"irrigation_valve_v1\": {\n"
            << "      \"register_id\": \"irrigation_valve_v1\",\n"
            << "      \"is_online\": " << (valve_online ? "true" : "false") << ",\n"
            << "      \"commanded_on\": " << (valve_on ? "true" : "false") << ",\n"
            << "      \"actuator_level_bp\": " << valve_bp << "\n"
            << "    }\n"
            << "  },\n"
            << "  \"l2_telemetry\": ";
        AppendL2TelemetryJson(oss, 2U);
        oss << "\n}\n";
        return oss.str();
    }

    // ------------------------------------------------------------------------
    // L3 Pairwise BCC & Mesh Weather Operations (for col-swarmd)
    // ------------------------------------------------------------------------

    bool SettlePeerClusterBcc(const std::string& cluster_id, int64_t delta_local_to_peer_uj, bool count_task = false) {
        for (auto& ch : ledger_.peer_bcc_channels) {
            if (ch.peer_cluster_id == cluster_id) {
                const bool ok = ch.SettleSignedDelta(delta_local_to_peer_uj);
                if (ok && count_task) {
                    ++ch.completed_mutual_tasks;
                }
                return ok;
            }
        }
        return false;
    }

    void PushMeshWeatherEvent(const L3MeshWeatherEvent& ev) {
        ledger_.mesh_weather_feed.insert(ledger_.mesh_weather_feed.begin(), ev);
        if (ledger_.mesh_weather_feed.size() > 24U) {
            ledger_.mesh_weather_feed.resize(24U);
        }
    }

    void PushPeerSurplusOffer(const L3SurplusOffer& offer) {
        ledger_.peer_surplus_offers.insert(ledger_.peer_surplus_offers.begin(), offer);
        if (ledger_.peer_surplus_offers.size() > 16U) {
            ledger_.peer_surplus_offers.resize(16U);
        }
    }

    // ------------------------------------------------------------------------
    // JSON Command Dispatcher & Full Node Snapshot Serializer
    // ------------------------------------------------------------------------

    std::string DispatchJsonCommand(const std::string& action, const std::string& payload) {
        ++current_tick_;
        const std::string resolved_action =
            !action.empty() ? action : json_util::ExtractString(payload, "action");

        std::string_view nested_payload = json_util::ExtractObjectSlice(payload, "payload");
        std::string_view args = !nested_payload.empty() ? nested_payload : std::string_view(payload);

        last_command_action_ = resolved_action;

        if (resolved_action == "search_needs") {
            const std::string query = json_util::ExtractString(args, "query");
            last_search_results_ = SearchNeeds(query);
            last_command_status_ = "OK";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "request_need") {
            std::string need_id = json_util::ExtractString(args, "need_id");
            if (need_id.empty()) {
                need_id = json_util::ExtractString(args, "id", "need:pea_soup");
            }
            const NeedTimingMode mode =
                ParseNeedTimingMode(json_util::ExtractString(args, "timing_mode", "NOW"));
            const uint32_t due_day =
                static_cast<uint32_t>(json_util::ExtractUint64(args, "due_sim_day", 0ULL));
            const uint32_t qty =
                static_cast<uint32_t>(json_util::ExtractUint64(args, "quantity_units", 1ULL));
            const std::string requester =
                json_util::ExtractString(args, "requester_did", "did:mesh:node01:steward");
            (void)RequestNeed(need_id, mode, due_day, qty, requester);
            last_command_status_ = "OK";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "define_need") {
            const std::string title = json_util::ExtractString(args, "title", "Community Mesh Need");
            const std::string category = json_util::ExtractString(args, "category", "Community Defined Commons");
            const std::string l7_label =
                json_util::ExtractString(args, "l7_channel_label", "L7 Legacy Supply Channel");
            const uint64_t exergy_uj =
                json_util::ExtractUint64(args, "exergy_cost_uj", 3000000000ULL);
            const int64_t cost_cents =
                json_util::ExtractInt64(args, "l7_unit_cost_cents", 1500);
            const std::string local_label =
                json_util::ExtractString(args, "local_channel_label", "Local L5 Stewarded Workflow ($0.00)");
            (void)DefineNewMeshNeed(title, category, l7_label, exergy_uj, cost_cents, local_label);
            last_command_status_ = "OK";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "approve_decision") {
            std::string decision_id = json_util::ExtractString(args, "decision_id");
            if (decision_id.empty()) {
                decision_id = json_util::ExtractString(args, "id");
            }
            const std::string cultivar_id = json_util::ExtractString(args, "cultivar_id");
            if (!cultivar_id.empty()) {
                (void)SetGardenCultivar(cultivar_id);
            }
            const bool approve = json_util::ExtractBool(args, "approve", true);
            bool ok = false;
            if (L6PendingDecision* dec = FindDecisionMutable(decision_id)) {
                const bool has_overrides =
                    json_util::HasKey(args, "sourcing_mode") ||
                    json_util::HasKey(args, "distribution_policy") ||
                    json_util::HasKey(args, "task_release_ring");
                if (has_overrides) {
                    const SourcingMode sm = ParseSourcingMode(
                        json_util::ExtractString(args, "sourcing_mode"), dec->proposed_sourcing_mode);
                    const DistributionPolicy dp = ParseDistributionPolicy(
                        json_util::ExtractString(args, "distribution_policy"), dec->proposed_distribution_policy);
                    const TaskReleaseRing tr = ParseTaskReleaseRing(
                        json_util::ExtractString(args, "task_release_ring"), dec->proposed_task_release_ring);
                    ok = ApproveOrAdaptGovernanceDecision(decision_id, approve, sm, dp, tr, true);
                } else {
                    ok = ApproveOrAdaptGovernanceDecision(decision_id, approve);
                }
            }
            last_command_status_ = ok ? "OK" : "NOT_FOUND";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "adapt_policy") {
            std::string target_id = json_util::ExtractString(args, "decision_id");
            if (target_id.empty()) {
                target_id = json_util::ExtractString(args, "asset_id");
            }
            if (target_id.empty()) {
                target_id = json_util::ExtractString(args, "id");
            }
            const std::string cultivar_id = json_util::ExtractString(args, "cultivar_id");
            if (!cultivar_id.empty()) {
                (void)SetGardenCultivar(cultivar_id);
            }
            SourcingMode sm_default = SourcingMode::R0_LOCAL_WORKFLOWS;
            DistributionPolicy dp_default = DistributionPolicy::R1_TRUST_RING_COMMONS;
            TaskReleaseRing tr_default = TaskReleaseRing::R0_STEWARD_ONLY;
            if (const L5GovernedAsset* asset = FindAsset(target_id)) {
                sm_default = asset->sourcing_mode;
                dp_default = asset->distribution_policy;
                tr_default = asset->task_release_ring;
            } else if (const L6PendingDecision* dec = FindDecision(target_id)) {
                sm_default = dec->proposed_sourcing_mode;
                dp_default = dec->proposed_distribution_policy;
                tr_default = dec->proposed_task_release_ring;
            }
            const SourcingMode sm =
                ParseSourcingMode(json_util::ExtractString(args, "sourcing_mode"), sm_default);
            const DistributionPolicy dp =
                ParseDistributionPolicy(json_util::ExtractString(args, "distribution_policy"), dp_default);
            const TaskReleaseRing tr =
                ParseTaskReleaseRing(json_util::ExtractString(args, "task_release_ring"), tr_default);
            const bool ok = AdaptAssetGovernancePolicy(target_id, sm, dp, tr);
            last_command_status_ = ok ? "OK" : "NOT_FOUND";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "set_cultivar") {
            std::string cultivar_id = json_util::ExtractString(args, "cultivar_id");
            if (cultivar_id.empty()) {
                cultivar_id = json_util::ExtractString(args, "id");
            }
            const bool ok = SetGardenCultivar(cultivar_id);
            last_command_status_ = ok ? "OK" : "UNKNOWN_CULTIVAR";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "advance_sim_clock") {
            if (json_util::HasKey(args, "ambient_temp_mk")) {
                SetAmbientTempOverrideMk(
                    static_cast<uint32_t>(json_util::ExtractUint64(args, "ambient_temp_mk", 0ULL)));
            }
            uint64_t delta_s = json_util::ExtractUint64(args, "delta_sim_seconds", 0ULL);
            if (delta_s == 0ULL) {
                delta_s = json_util::ExtractUint64(args, "seconds", 0ULL);
            }
            AdvanceSimClockSeconds(delta_s);
            last_command_status_ = "OK";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "commit_task") {
            std::string task_id = json_util::ExtractString(args, "task_id");
            if (task_id.empty()) {
                task_id = json_util::ExtractString(args, "id");
            }
            const bool ok = CommitTask(task_id);
            last_command_status_ = ok ? "OK" : "BLOCKED_OR_NOT_ACTIVE_TOKEN";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "release_task_r1") {
            std::string task_id = json_util::ExtractString(args, "task_id");
            if (task_id.empty()) {
                task_id = json_util::ExtractString(args, "id");
            }
            const bool ok = ReleaseTaskToR1Peers(task_id);
            last_command_status_ = ok ? "OK" : "SOLO_CLUSTER_OR_NOT_ACTIVE_TOKEN";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "complete_task") {
            std::string task_id = json_util::ExtractString(args, "task_id");
            if (task_id.empty()) {
                task_id = json_util::ExtractString(args, "id");
            }
            const std::string cluster_id = json_util::ExtractString(args, "cluster_id");
            const uint32_t obs_growth_bp = static_cast<uint32_t>(
                json_util::ExtractUint64(args, "observed_growth_bp", 0ULL));
            const uint32_t obs_moisture_bp = static_cast<uint32_t>(
                json_util::ExtractUint64(args, "observed_moisture_bp", 0ULL));
            const bool ok = CompleteTask(task_id, cluster_id, obs_growth_bp, obs_moisture_bp);
            last_command_status_ = ok ? "OK" : "BLOCKED_OR_NOT_ACTIVE_TOKEN";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "start_workflow_cycle") {
            std::string wf_id = json_util::ExtractString(args, "workflow_id");
            if (wf_id.empty()) {
                wf_id = json_util::ExtractString(args, "id");
            }
            const std::string cultivar_id = json_util::ExtractString(args, "cultivar_id");
            const bool ok = StartNewWorkflowCycle(wf_id, cultivar_id);
            last_command_status_ = ok ? "OK" : "CYCLE_IN_PROGRESS_OR_NOT_FOUND";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "set_external_swarm") {
            const bool enabled = json_util::ExtractBool(
                args, "enabled", !IsExternalR1SwarmEnabled());
            SetExternalR1SwarmEnabled(enabled);
            last_command_status_ = "OK";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "ingest_l2_telemetry") {
            (void)IngestL2PinsPayload(args);
            last_command_status_ = "OK";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "answer_unmodeled_prompt") {
            std::string task_id = json_util::ExtractString(args, "task_id");
            if (task_id.empty()) {
                task_id = json_util::ExtractString(args, "id");
            }
            std::string answer = json_util::ExtractString(args, "answer");
            if (answer.empty()) {
                answer = json_util::ExtractString(args, "response");
            }
            const bool ok = AnswerUnmodeledMicroDetailPrompt(task_id, answer);
            last_command_status_ = ok ? "OK" : "INVALID_OR_NOT_FOUND";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "reset_homestead_cluster") {
            Bootstrap();
            last_command_action_ = "reset_homestead_cluster";
            last_command_status_ = "OK";
            return SerializeFullNodeSnapshotJson();
        }

        last_command_status_ = "UNSUPPORTED_ACTION";
        std::ostringstream err;
        err << "{\n"
            << "  \"ok\": false,\n"
            << "  \"error\": \"UNSUPPORTED_ACTION\",\n"
            << "  \"action\": \"" << json_util::Escape(resolved_action) << "\"\n"
            << "}\n";
        return err.str();
    }

    [[nodiscard]] std::string SerializeFullNodeSnapshotJson() const {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"ok\": true,\n"
            << "  \"schema_version\": \"oasis.stack.snapshot.v2\",\n"
            << "  \"node_did\": \"did:mesh:lot402:homestead-cluster\",\n"
            << "  \"cluster_mode\": \"" << (IsExternalR1SwarmEnabled() ? "R1_FEDERATED_SWARM" : "SOLO_STEWARD_4_NODE_CLUSTER") << "\",\n"
            << "  \"external_r1_swarm_enabled\": " << (IsExternalR1SwarmEnabled() ? "true" : "false") << ",\n"
            << "  \"tick\": " << current_tick_ << ",\n"
            << "  \"sim_day\": " << sim_day_ << ",\n"
            << "  \"sim_clock_seconds\": " << sim_clock_seconds_ << ",\n"
            << "  \"ambient_temp_mk\": " << CurrentAmbientTempMk() << ",\n"
            << "  \"selected_cultivar_id\": \"" << json_util::Escape(selected_garden_cultivar_id_) << "\",\n"
            << "  \"bootstrapped_from_jsonld\": " << (bootstrapped_from_jsonld_ != 0U ? "true" : "false") << ",\n"
            << "  \"bootstrap_source\": \"" << json_util::Escape(bootstrap_source_path_) << "\",\n"
            << "  \"last_command_action\": \"" << json_util::Escape(last_command_action_) << "\",\n"
            << "  \"last_command_status\": \"" << json_util::Escape(last_command_status_) << "\",\n"
            << "  \"homestead_physical_state\": {\n"
            << "    \"raised_bed_frames_built\": " << raised_bed_frames_built_ << ",\n"
            << "    \"compost_kiln_units_built\": " << compost_kiln_units_built_ << ",\n"
            << "    \"poultry_coops_built\": " << poultry_coops_built_ << ",\n"
            << "    \"harvested_split_pea_batches\": " << harvested_split_pea_batches_ << ",\n"
            << "    \"harvested_mirepoix_batches\": " << harvested_mirepoix_batches_ << ",\n"
            << "    \"poultry_bones_and_eggs_batches\": " << poultry_bones_and_eggs_batches_ << ",\n"
            << "    \"thermophilic_compost_batches\": " << thermophilic_compost_batches_ << ",\n"
            << "    \"biochar_batches\": " << biochar_batches_ << ",\n"
            << "    \"simmered_bone_broth_batches\": " << simmered_bone_broth_batches_ << ",\n"
            << "    \"served_pea_soup_tiffin_batches\": " << served_pea_soup_tiffin_batches_ << "\n"
            << "  },\n";

        // 0A. Parameterized Legume Cultivars (Fabaceae Real & Sim-Tuned Virtual Varieties)
        oss << "  \"available_cultivars\": [";
        for (size_t i = 0U; i < cultivars_.size(); ++i) {
            const auto& c = cultivars_[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "    {\n"
                << "      \"cultivar_id\": \"" << json_util::Escape(c.cultivar_id) << "\",\n"
                << "      \"title\": \"" << json_util::Escape(c.title) << "\",\n"
                << "      \"species_binomial\": \"" << json_util::Escape(c.species_binomial) << "\",\n"
                << "      \"agronomic_notes\": \"" << json_util::Escape(c.agronomic_notes) << "\",\n"
                << "      \"maturity_sim_seconds\": " << c.maturity_sim_seconds << ",\n"
                << "      \"maturity_label\": \"" << json_util::Escape(FormatSimDurationCompact(c.maturity_sim_seconds)) << "\",\n"
                << "      \"verification_interval_sim_s\": " << c.verification_interval_sim_s << ",\n"
                << "      \"verification_interval_label\": \"" << json_util::Escape(FormatSimDurationCompact(c.verification_interval_sim_s)) << "\",\n"
                << "      \"min_growth_temp_mk\": " << c.min_growth_temp_mk << ",\n"
                << "      \"opt_low_temp_mk\": " << c.opt_low_temp_mk << ",\n"
                << "      \"opt_high_temp_mk\": " << c.opt_high_temp_mk << ",\n"
                << "      \"max_growth_temp_mk\": " << c.max_growth_temp_mk << ",\n"
                << "      \"target_min_vwc_bp\": " << c.target_min_vwc_bp << ",\n"
                << "      \"target_opt_vwc_bp\": " << c.target_opt_vwc_bp << ",\n"
                << "      \"target_max_vwc_bp\": " << c.target_max_vwc_bp << ",\n"
                << "      \"irrigation_dose_ml\": " << c.irrigation_dose_ml << ",\n"
                << "      \"harvest_mature_bp\": " << c.harvest_mature_bp << ",\n"
                << "      \"trellis_ridge_height_mm\": " << c.trellis_ridge_height_mm << ",\n"
                << "      \"sowing_depth_mm\": " << c.sowing_depth_mm << ",\n"
                << "      \"n_draw_per_band_ppm\": " << static_cast<uint32_t>(c.n_draw_per_band_ppm) << ",\n"
                << "      \"fixes_atmospheric_n2\": " << (c.fixes_atmospheric_n2 != 0U ? "true" : "false") << ",\n"
                << "      \"is_virtual_sim_tuned\": " << (c.is_virtual_sim_tuned != 0U ? "true" : "false") << ",\n"
                << "      \"harvest_legume_batches\": " << static_cast<uint32_t>(c.harvest_legume_batches) << ",\n"
                << "      \"harvest_mirepoix_batches\": " << static_cast<uint32_t>(c.harvest_mirepoix_batches) << "\n"
                << "    }";
        }
        oss << (cultivars_.empty() ? "],\n" : "\n  ],\n");

        // 0B. Solo Steward 4-Node Personal Homestead Cluster Topology
        oss << "  \"homestead_cluster_nodes\": [";
        for (size_t i = 0U; i < cluster_nodes_.size(); ++i) {
            const auto& cn = cluster_nodes_[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "    {\n"
                << "      \"node_id\": \"" << json_util::Escape(cn.node_id) << "\",\n"
                << "      \"node_did\": \"" << json_util::Escape(cn.node_did) << "\",\n"
                << "      \"title\": \"" << json_util::Escape(cn.title) << "\",\n"
                << "      \"zone_label\": \"" << json_util::Escape(cn.zone_label) << "\",\n"
                << "      \"daemons_hosted\": \"" << json_util::Escape(cn.daemons_hosted) << "\",\n"
                << "      \"role_summary\": \"" << json_util::Escape(cn.role_summary) << "\",\n"
                << "      \"active_token_summary\": \"" << json_util::Escape(cn.active_token_summary) << "\",\n"
                << "      \"inventory_summary\": \"" << json_util::Escape(cn.inventory_summary) << "\",\n"
                << "      \"accounted_exergy_uj\": " << cn.accounted_exergy_uj << ",\n"
                << "      \"active_workflow_count\": " << cn.active_workflow_count << ",\n"
                << "      \"completed_task_count\": " << cn.completed_task_count << ",\n"
                << "      \"online_l2_pin_count\": " << cn.online_l2_pin_count << ",\n"
                << "      \"is_online\": " << (cn.is_online != 0U ? "true" : "false") << "\n"
                << "    }";
        }
        oss << (cluster_nodes_.empty() ? "],\n" : "\n  ],\n");

        // Optional search_results from last search_needs command
        oss << "  \"search_results\": [";
        for (size_t i = 0U; i < last_search_results_.size(); ++i) {
            oss << (i == 0U ? "\n" : ",\n");
            AppendNeedJson(oss, last_search_results_[i], 4U);
        }
        oss << (last_search_results_.empty() ? "],\n" : "\n  ],\n");

        // 1. L6 Needs Catalog
        oss << "  \"l6_needs\": [";
        for (size_t i = 0U; i < needs_.size(); ++i) {
            oss << (i == 0U ? "\n" : ",\n");
            AppendNeedJson(oss, needs_[i], 4U);
        }
        oss << (needs_.empty() ? "],\n" : "\n  ],\n");

        // 2. L6 Intent Requests
        oss << "  \"l6_requests\": [";
        for (size_t i = 0U; i < requests_.size(); ++i) {
            const auto& r = requests_[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "    {\n"
                << "      \"request_id\": \"" << json_util::Escape(r.request_id) << "\",\n"
                << "      \"need_id\": \"" << json_util::Escape(r.need_id) << "\",\n"
                << "      \"need_title\": \"" << json_util::Escape(r.need_title) << "\",\n"
                << "      \"requester_did\": \"" << json_util::Escape(r.requester_did) << "\",\n"
                << "      \"timing_mode\": \"" << ToString(r.timing_mode) << "\",\n"
                << "      \"due_sim_day\": " << r.due_sim_day << ",\n"
                << "      \"quantity_units\": " << r.quantity_units << ",\n"
                << "      \"exergy_budget_uj\": " << r.exergy_budget_uj << ",\n"
                << "      \"resolved_sourcing_mode\": \"" << ToString(r.resolved_sourcing_mode) << "\",\n"
                << "      \"fulfilled\": " << (r.fulfilled != 0U ? "true" : "false") << ",\n"
                << "      \"fulfillment_note\": \"" << json_util::Escape(r.fulfillment_note) << "\",\n"
                << "      \"created_tick\": " << r.created_tick << "\n"
                << "    }";
        }
        oss << (requests_.empty() ? "],\n" : "\n  ],\n");

        // 3. L6 Pending & Resolved Governance Decisions from L5
        oss << "  \"l6_pending_decisions\": [";
        for (size_t i = 0U; i < pending_decisions_.size(); ++i) {
            const auto& d = pending_decisions_[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "    {\n"
                << "      \"decision_id\": \"" << json_util::Escape(d.decision_id) << "\",\n"
                << "      \"title\": \"" << json_util::Escape(d.title) << "\",\n"
                << "      \"governed_asset_id\": \"" << json_util::Escape(d.governed_asset_id) << "\",\n"
                << "      \"rationale\": \"" << json_util::Escape(d.rationale) << "\",\n"
                << "      \"proposed_sourcing_mode\": \"" << ToString(d.proposed_sourcing_mode) << "\",\n"
                << "      \"proposed_distribution_policy\": \"" << ToString(d.proposed_distribution_policy) << "\",\n"
                << "      \"proposed_task_release_ring\": \"" << ToString(d.proposed_task_release_ring) << "\",\n"
                << "      \"estimated_exergy_savings_uj\": " << d.estimated_exergy_savings_uj << ",\n"
                << "      \"estimated_monthly_l7_savings_cents\": " << d.estimated_monthly_l7_savings_cents << ",\n"
                << "      \"status\": \"" << ToString(d.status) << "\",\n"
                << "      \"emitted_tick\": " << d.emitted_tick << ",\n"
                << "      \"linked_need_ids\": [";
            for (size_t j = 0U; j < d.linked_need_ids.size(); ++j) {
                oss << (j == 0U ? "" : ", ") << "\"" << json_util::Escape(d.linked_need_ids[j]) << "\"";
            }
            oss << "]\n"
                << "    }";
        }
        oss << (pending_decisions_.empty() ? "],\n" : "\n  ],\n");

        // 4. L5 Governed Assets
        oss << "  \"l5_governed_assets\": [";
        for (size_t i = 0U; i < governed_assets_.size(); ++i) {
            const auto& a = governed_assets_[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "    {\n"
                << "      \"asset_id\": \"" << json_util::Escape(a.asset_id) << "\",\n"
                << "      \"title\": \"" << json_util::Escape(a.title) << "\",\n"
                << "      \"governance_badge\": \"" << json_util::Escape(a.governance_badge) << "\",\n"
                << "      \"sourcing_mode\": \"" << ToString(a.sourcing_mode) << "\",\n"
                << "      \"distribution_policy\": \"" << ToString(a.distribution_policy) << "\",\n"
                << "      \"task_release_ring\": \"" << ToString(a.task_release_ring) << "\",\n"
                << "      \"local_workflows_instantiated\": " << (a.local_workflows_instantiated != 0U ? "true" : "false") << ",\n"
                << "      \"l7_fallback_channel\": \"" << json_util::Escape(a.l7_fallback_channel) << "\",\n"
                << "      \"local_commons_channel\": \"" << json_util::Escape(a.local_commons_channel) << "\",\n"
                << "      \"maintenance_exergy_uj\": " << a.maintenance_exergy_uj << ",\n"
                << "      \"monthly_l7_offset_potential_cents\": " << a.monthly_l7_offset_potential_cents << ",\n"
                << "      \"steward_reputation_credit_bp\": " << a.steward_reputation_credit_bp << ",\n"
                << "      \"linked_need_ids\": [";
            for (size_t j = 0U; j < a.linked_need_ids.size(); ++j) {
                oss << (j == 0U ? "" : ", ") << "\"" << json_util::Escape(a.linked_need_ids[j]) << "\"";
            }
            oss << "],\n"
                << "      \"workflow_ids\": [";
            for (size_t j = 0U; j < a.workflow_ids.size(); ++j) {
                oss << (j == 0U ? "" : ", ") << "\"" << json_util::Escape(a.workflow_ids[j]) << "\"";
            }
            oss << "]\n"
                << "    }";
        }
        oss << (governed_assets_.empty() ? "],\n" : "\n  ],\n");

        // 5. L4 Workflows & Tasks (Strictly BPMN Token-Sequenced + Timed Catch Events)
        oss << "  \"l4_workflows\": [";
        std::vector<const L4TaskItem*> committed_tasks;
        std::vector<const L4TaskItem*> available_tasks;
        std::vector<const L4TaskItem*> waiting_timer_tasks;
        std::vector<const L4TaskItem*> active_token_tasks;
        for (size_t i = 0U; i < workflows_.size(); ++i) {
            const auto& wf = workflows_[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "    {\n"
                << "      \"workflow_id\": \"" << json_util::Escape(wf.workflow_id) << "\",\n"
                << "      \"asset_id\": \"" << json_util::Escape(wf.asset_id) << "\",\n"
                << "      \"assigned_node_id\": \"" << json_util::Escape(wf.assigned_node_id) << "\",\n"
                << "      \"title\": \"" << json_util::Escape(wf.title) << "\",\n"
                << "      \"input_cultivar_id\": \"" << json_util::Escape(wf.input_cultivar_id) << "\",\n"
                << "      \"input_cultivar_title\": \"" << json_util::Escape(wf.input_cultivar_title) << "\",\n"
                << "      \"species_binomial\": \"" << json_util::Escape(wf.species_binomial) << "\",\n"
                << "      \"phenology_stage_label\": \"" << json_util::Escape(wf.phenology_stage_label) << "\",\n"
                << "      \"crop_sown\": " << (wf.crop_sown != 0U ? "true" : "false") << ",\n"
                << "      \"is_virtual_cultivar\": " << (wf.is_virtual_cultivar != 0U ? "true" : "false") << ",\n"
                << "      \"fixes_atmospheric_n2\": " << (wf.fixes_atmospheric_n2 != 0U ? "true" : "false") << ",\n"
                << "      \"crop_growth_bp\": " << wf.crop_growth_bp << ",\n"
                << "      \"harvest_mature_bp\": " << wf.harvest_mature_bp << ",\n"
                << "      \"maturity_sim_seconds\": " << wf.maturity_sim_seconds << ",\n"
                << "      \"verification_interval_sim_s\": " << wf.verification_interval_sim_s << ",\n"
                << "      \"estimated_moisture_bp\": " << wf.estimated_moisture_bp << ",\n"
                << "      \"estimated_nitrogen_ppm\": " << wf.estimated_nitrogen_ppm << ",\n"
                << "      \"verification_checks_completed\": " << wf.verification_checks_completed << ",\n"
                << "      \"auto_l2_checks_completed\": " << wf.auto_l2_checks_completed << ",\n"
                << "      \"next_verification_due_sim_s\": " << wf.next_verification_due_sim_s << ",\n"
                << "      \"is_active\": " << (wf.is_active != 0U ? "true" : "false") << ",\n"
                << "      \"completed_cycles\": " << wf.completed_cycles << ",\n"
                << "      \"spawned_tick\": " << wf.spawned_tick << ",\n"
                << "      \"tasks\": [";
            for (size_t j = 0U; j < wf.tasks.size(); ++j) {
                const auto& t = wf.tasks[j];
                if (t.task_commitment_state == TaskCommitmentState::COMMITTED_BY_PLAYER &&
                    t.bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN) {
                    committed_tasks.push_back(&t);
                }
                if (t.bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN) {
                    active_token_tasks.push_back(&t);
                    if (t.task_commitment_state == TaskCommitmentState::UNASSIGNED) {
                        available_tasks.push_back(&t);
                    }
                } else if (t.bpmn_token_state == BpmnTokenState::WAITING_TIMER) {
                    waiting_timer_tasks.push_back(&t);
                }
                oss << (j == 0U ? "\n" : ",\n");
                AppendTaskJson(oss, t, 8U);
            }
            oss << (wf.tasks.empty() ? "]\n" : "\n      ]\n")
                << "    }";
        }
        oss << (workflows_.empty() ? "],\n" : "\n  ],\n");

        // 5B. Active BPMN Execution Tokens (used by 3D Spatial Context Menu & HUD)
        oss << "  \"active_bpmn_tokens\": [";
        for (size_t i = 0U; i < active_token_tasks.size(); ++i) {
            const auto& t = *active_token_tasks[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "    {\n"
                << "      \"task_id\": \"" << json_util::Escape(t.task_id) << "\",\n"
                << "      \"workflow_id\": \"" << json_util::Escape(t.workflow_id) << "\",\n"
                << "      \"assigned_node_id\": \"" << json_util::Escape(t.assigned_node_id) << "\",\n"
                << "      \"mapped_affordance_id\": \"" << json_util::Escape(t.mapped_affordance_id) << "\",\n"
                << "      \"title\": \"" << json_util::Escape(t.title) << "\",\n"
                << "      \"task_commitment_state\": \"" << ToString(t.task_commitment_state) << "\"\n"
                << "    }";
        }
        oss << (active_token_tasks.empty() ? "],\n" : "\n  ],\n");

        // 6A. Flat L4 Available Unassigned Tasks (Active Token Only — powers HUD Available Tasks window)
        oss << "  \"l4_available_tasks\": [";
        for (size_t i = 0U; i < available_tasks.size(); ++i) {
            oss << (i == 0U ? "\n" : ",\n");
            AppendTaskJson(oss, *available_tasks[i], 4U);
        }
        oss << (available_tasks.empty() ? "],\n" : "\n  ],\n");

        // 6B. Flat L4 Active BPMN Timer Catch Events (powers HUD timer countdown badges)
        oss << "  \"l4_waiting_timers\": [";
        for (size_t i = 0U; i < waiting_timer_tasks.size(); ++i) {
            oss << (i == 0U ? "\n" : ",\n");
            AppendTaskJson(oss, *waiting_timer_tasks[i], 4U);
        }
        oss << (waiting_timer_tasks.empty() ? "],\n" : "\n  ],\n");

        // 6C. Flat L4 Committed Player Tasks (Active Token Only)
        oss << "  \"l4_committed_tasks\": [";
        for (size_t i = 0U; i < committed_tasks.size(); ++i) {
            oss << (i == 0U ? "\n" : ",\n");
            AppendTaskJson(oss, *committed_tasks[i], 4U);
        }
        oss << (committed_tasks.empty() ? "],\n" : "\n  ],\n");

        // 7. L3 Exergy, Pairwise BCC & Inter-Node / Peer Weather
        oss << "  \"l3_ledger\": {\n"
            << "    \"total_exergy_accounted_uj\": " << ledger_.total_exergy_accounted_uj << ",\n"
            << "    \"bcc_zero_sum_invariant_ok\": " << (ledger_.VerifyPairwiseZeroSumInvariant() ? "true" : "false") << ",\n"
            << "    \"pairwise_bcc_channels\": [";
        for (size_t i = 0U; i < ledger_.peer_bcc_channels.size(); ++i) {
            const auto& ch = ledger_.peer_bcc_channels[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "      {\n"
                << "        \"peer_cluster_id\": \"" << json_util::Escape(ch.peer_cluster_id) << "\",\n"
                << "        \"peer_cluster_title\": \"" << json_util::Escape(ch.peer_cluster_title) << "\",\n"
                << "        \"household_count\": " << ch.household_count << ",\n"
                << "        \"balance_local_to_peer_uj\": " << ch.balance_local_to_peer_uj << ",\n"
                << "        \"balance_peer_to_local_uj\": " << ch.balance_peer_to_local_uj << ",\n"
                << "        \"credit_limit_uj\": " << ch.credit_limit_uj << ",\n"
                << "        \"total_cleared_volume_uj\": " << ch.total_cleared_volume_uj << ",\n"
                << "        \"completed_mutual_tasks\": " << ch.completed_mutual_tasks << "\n"
                << "      }";
        }
        oss << (ledger_.peer_bcc_channels.empty() ? "],\n" : "\n    ],\n");

        oss << "    \"peer_surplus_offers\": [";
        for (size_t i = 0U; i < ledger_.peer_surplus_offers.size(); ++i) {
            const auto& o = ledger_.peer_surplus_offers[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "      {\n"
                << "        \"offer_id\": \"" << json_util::Escape(o.offer_id) << "\",\n"
                << "        \"cluster_id\": \"" << json_util::Escape(o.cluster_id) << "\",\n"
                << "        \"item_title\": \"" << json_util::Escape(o.item_title) << "\",\n"
                << "        \"exergy_value_uj\": " << o.exergy_value_uj << ",\n"
                << "        \"quantity_units\": " << o.quantity_units << ",\n"
                << "        \"is_available\": " << (o.is_available != 0U ? "true" : "false") << ",\n"
                << "        \"posted_tick\": " << o.posted_tick << "\n"
                << "      }";
        }
        oss << (ledger_.peer_surplus_offers.empty() ? "],\n" : "\n    ],\n");

        oss << "    \"mesh_weather_feed\": [";
        for (size_t i = 0U; i < ledger_.mesh_weather_feed.size(); ++i) {
            const auto& ev = ledger_.mesh_weather_feed[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "      {\n"
                << "        \"event_id\": \"" << json_util::Escape(ev.event_id) << "\",\n"
                << "        \"cluster_id\": \"" << json_util::Escape(ev.cluster_id) << "\",\n"
                << "        \"cluster_title\": \"" << json_util::Escape(ev.cluster_title) << "\",\n"
                << "        \"event_category\": \"" << json_util::Escape(ev.event_category) << "\",\n"
                << "        \"summary\": \"" << json_util::Escape(ev.summary) << "\",\n"
                << "        \"exergy_delta_uj\": " << ev.exergy_delta_uj << ",\n"
                << "        \"tick\": " << ev.tick << "\n"
                << "      }";
        }
        oss << (ledger_.mesh_weather_feed.empty() ? "]\n" : "\n    ]\n")
            << "  },\n";

        // 8. L2 Active Sensor & Actuator Registers
        oss << "  \"l2_telemetry\": ";
        AppendL2TelemetryJson(oss, 2U);
        oss << ",\n";

        // 9. L7 Bill-Offset & Genesis Reserve Ledger
        oss << "  \"l7_bill_offset_ledger\": {\n"
            << "    \"household_reserve_cents\": " << ledger_.household_reserve_cents << ",\n"
            << "    \"monthly_legacy_bills_cents\": " << ledger_.monthly_legacy_bills_cents << ",\n"
            << "    \"monthly_l7_offset_revenue_cents\": " << ledger_.monthly_l7_offset_revenue_cents << ",\n"
            << "    \"net_monthly_cashflow_cents\": " << ledger_.NetMonthlyCashflowCents() << ",\n"
            << "    \"zero_net_bill_achieved\": " << (ledger_.IsZeroNetBillAchieved() ? "true" : "false") << ",\n"
            << "    \"completed_l7_offset_orders\": " << ledger_.completed_l7_offset_orders << ",\n"
            << "    \"r0_baseline_enrolled\": " << (ledger_.r0_baseline_enrolled != 0U ? "true" : "false") << ",\n"
            << "    \"dependency_ratio_bp\": " << ledger_.dependency_ratio_bp << ",\n"
            << "    \"steward_reputation_bp\": " << ledger_.steward_reputation_bp << "\n"
            << "  }\n"
            << "}\n";
        return oss.str();
    }

private:
    static std::string MakeSlug(std::string_view text) {
        std::string slug;
        bool last_underscore = false;
        for (char ch : text) {
            if (std::isalnum(static_cast<unsigned char>(ch))) {
                slug.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
                last_underscore = false;
            } else if (!last_underscore && !slug.empty()) {
                slug.push_back('_');
                last_underscore = true;
            }
            if (slug.size() >= 28U) {
                break;
            }
        }
        while (!slug.empty() && slug.back() == '_') {
            slug.pop_back();
        }
        return slug.empty() ? "mesh_need" : slug;
    }

    static void AppendIndent(std::ostringstream& oss, size_t spaces) {
        for (size_t i = 0U; i < spaces; ++i) {
            oss.put(' ');
        }
    }

    static void AppendNeedJson(std::ostringstream& oss, const L6NeedRecord& n, size_t indent) {
        const std::string pad(indent, ' ');
        oss << pad << "{\n"
            << pad << "  \"need_id\": \"" << json_util::Escape(n.need_id) << "\",\n"
            << pad << "  \"title\": \"" << json_util::Escape(n.title) << "\",\n"
            << pad << "  \"category\": \"" << json_util::Escape(n.category) << "\",\n"
            << pad << "  \"description\": \"" << json_util::Escape(n.description) << "\",\n"
            << pad << "  \"exergy_cost_uj\": " << n.exergy_cost_uj << ",\n"
            << pad << "  \"l7_unit_cost_cents\": " << n.l7_unit_cost_cents << ",\n"
            << pad << "  \"l7_channel_label\": \"" << json_util::Escape(n.l7_channel_label) << "\",\n"
            << pad << "  \"local_channel_label\": \"" << json_util::Escape(n.local_channel_label) << "\",\n"
            << pad << "  \"governed_asset_id\": \"" << json_util::Escape(n.governed_asset_id) << "\",\n"
            << pad << "  \"active_sourcing_mode\": \"" << ToString(n.active_sourcing_mode) << "\",\n"
            << pad << "  \"is_r0_baseline_care\": " << (n.is_r0_baseline_care != 0U ? "true" : "false") << ",\n"
            << pad << "  \"is_custom_mesh_need\": " << (n.is_custom_mesh_need != 0U ? "true" : "false") << ",\n"
            << pad << "  \"action_code\": " << n.action_code << ",\n"
            << pad << "  \"request_count\": " << n.request_count << ",\n"
            << pad << "  \"last_status_note\": \"" << json_util::Escape(n.last_status_note) << "\"\n"
            << pad << "}";
    }

    static void AppendTaskJson(std::ostringstream& oss, const L4TaskItem& t, size_t indent) {
        const std::string pad(indent, ' ');
        oss << pad << "{\n"
            << pad << "  \"task_id\": \"" << json_util::Escape(t.task_id) << "\",\n"
            << pad << "  \"workflow_id\": \"" << json_util::Escape(t.workflow_id) << "\",\n"
            << pad << "  \"asset_id\": \"" << json_util::Escape(t.asset_id) << "\",\n"
            << pad << "  \"assigned_node_id\": \"" << json_util::Escape(t.assigned_node_id) << "\",\n"
            << pad << "  \"mapped_affordance_id\": \"" << json_util::Escape(t.mapped_affordance_id) << "\",\n"
            << pad << "  \"bpmn_token_state\": \"" << ToString(t.bpmn_token_state) << "\",\n"
            << pad << "  \"precondition_blocker_note\": \"" << json_util::Escape(t.precondition_blocker_note) << "\",\n"
            << pad << "  \"step_index\": " << t.step_index << ",\n"
            << pad << "  \"title\": \"" << json_util::Escape(t.title) << "\",\n"
            << pad << "  \"location_label\": \"" << json_util::Escape(t.location_label) << "\",\n"
            << pad << "  \"exergy_cost_uj\": " << t.exergy_cost_uj << ",\n"
            << pad << "  \"l7_revenue_offset_cents\": " << t.l7_revenue_offset_cents << ",\n"
            << pad << "  \"oasis_action_code\": " << t.oasis_action_code << ",\n"
            << pad << "  \"task_commitment_state\": \"" << ToString(t.task_commitment_state) << "\",\n"
            << pad << "  \"execution_modality\": \"" << ToString(t.execution_modality) << "\",\n"
            << pad << "  \"haul_modality\": \"" << ToString(t.haul_modality) << "\",\n"
            << pad << "  \"l2_sensor_register_id\": \"" << json_util::Escape(t.l2_sensor_register_id) << "\",\n"
            << pad << "  \"l2_actuator_register_id\": \"" << json_util::Escape(t.l2_actuator_register_id) << "\",\n"
            << pad << "  \"target_temp_min_mk\": " << t.target_temp_min_mk << ",\n"
            << pad << "  \"target_moisture_max_bp\": " << t.target_moisture_max_bp << ",\n"
            << pad << "  \"timer_due_sim_s\": " << t.timer_due_sim_s << ",\n"
            << pad << "  \"timer_remaining_sim_s\": " << t.timer_remaining_sim_s << ",\n"
            << pad << "  \"timer_remaining_label\": \"" << json_util::Escape(FormatSimDurationCompact(t.timer_remaining_sim_s)) << "\",\n"
            << pad << "  \"timer_interval_sim_s\": " << t.timer_interval_sim_s << ",\n"
            << pad << "  \"verification_cycle_index\": " << t.verification_cycle_index << ",\n"
            << pad << "  \"claimed_by_cluster_id\": \"" << json_util::Escape(t.claimed_by_cluster_id) << "\",\n"
            << pad << "  \"has_unmodeled_micro_detail\": " << (t.has_unmodeled_micro_detail != 0U ? "true" : "false") << ",\n"
            << pad << "  \"unmodeled_prompt_resolved\": " << (t.unmodeled_prompt_resolved != 0U ? "true" : "false") << ",\n"
            << pad << "  \"unmodeled_prompt_question\": \"" << json_util::Escape(t.unmodeled_prompt_question) << "\",\n"
            << pad << "  \"unmodeled_prompt_answer\": \"" << json_util::Escape(t.unmodeled_prompt_answer) << "\"\n"
            << pad << "}";
    }

    void AppendL2TelemetryJson(std::ostringstream& oss, size_t indent) const {
        const std::string pad(indent, ' ');
        oss << "{\n"
            << pad << "  \"last_update_tick\": " << l2_telemetry_.last_update_tick << ",\n"
            << pad << "  \"active_register_count\": " << l2_telemetry_.CountOnlineRegisters() << ",\n"
            << pad << "  \"soil_probe_v1_online\": "
            << (l2_telemetry_.IsRegisterOnline("soil_probe_v1") ? "true" : "false") << ",\n"
            << pad << "  \"irrigation_valve_v1_online\": "
            << (l2_telemetry_.IsRegisterOnline("irrigation_valve_v1") ? "true" : "false") << ",\n"
            << pad << "  \"culinary_vessel_v1_online\": "
            << (l2_telemetry_.IsRegisterOnline("culinary_vessel_v1") ? "true" : "false") << ",\n"
            << pad << "  \"registers\": [";
        for (size_t i = 0U; i < l2_telemetry_.registers.size(); ++i) {
            const auto& r = l2_telemetry_.registers[i];
            oss << (i == 0U ? "\n" : ",\n")
                << pad << "    {\n"
                << pad << "      \"register_id\": \"" << json_util::Escape(r.register_id) << "\",\n"
                << pad << "      \"hardware_modality\": \"" << json_util::Escape(r.hardware_modality) << "\",\n"
                << pad << "      \"is_online\": " << (r.is_online != 0U ? "true" : "false") << ",\n"
                << pad << "      \"is_actuator\": " << (r.is_actuator != 0U ? "true" : "false") << ",\n"
                << pad << "      \"actuator_commanded_on\": " << (r.actuator_commanded_on != 0U ? "true" : "false") << ",\n"
                << pad << "      \"temperature_mk\": " << r.temperature_mk << ",\n"
                << pad << "      \"moisture_bp\": " << r.moisture_bp << ",\n"
                << pad << "      \"concentration_ppm\": " << r.concentration_ppm << ",\n"
                << pad << "      \"crop_growth_bp\": " << r.crop_growth_bp << ",\n"
                << pad << "      \"actuator_level_bp\": " << r.actuator_level_bp << ",\n"
                << pad << "      \"hold_duration_ms\": " << r.hold_duration_ms << ",\n"
                << pad << "      \"cumulative_exergy_uj\": " << r.cumulative_exergy_uj << ",\n"
                << pad << "      \"quality_flags\": " << static_cast<uint32_t>(r.quality_flags) << ",\n"
                << pad << "      \"last_seen_tick\": " << r.last_seen_tick << "\n"
                << pad << "    }";
        }
        oss << (l2_telemetry_.registers.empty() ? "]\n" : ("\n" + pad + "  ]\n"))
            << pad << "}";
    }

    // ------------------------------------------------------------------------
    // Internal Lookup Helpers
    // ------------------------------------------------------------------------

    [[nodiscard]] L6NeedRecord* FindNeedMutable(std::string_view id_or_title) noexcept {
        std::string_view canonical_target = id_or_title;
        if (id_or_title == "need:mirepoix_roots") {
            canonical_target = "need:mirepoix_vegetables";
        } else if (id_or_title == "need:bone_broth") {
            canonical_target = "need:collagen_bone_broth";
        }
        for (auto& n : needs_) {
            if (n.need_id == canonical_target || n.title == canonical_target) {
                return &n;
            }
        }
        const std::string lowered = json_util::ToLowerAscii(canonical_target);
        for (auto& n : needs_) {
            if (json_util::ToLowerAscii(n.need_id) == lowered ||
                json_util::ToLowerAscii(n.title) == lowered) {
                return &n;
            }
        }
        return nullptr;
    }

    [[nodiscard]] const L6PendingDecision* FindDecision(std::string_view decision_id) const noexcept {
        for (const auto& d : pending_decisions_) {
            if (d.decision_id == decision_id) {
                return &d;
            }
        }
        return nullptr;
    }

    [[nodiscard]] L6PendingDecision* FindDecisionMutable(std::string_view decision_id) noexcept {
        for (auto& d : pending_decisions_) {
            if (d.decision_id == decision_id) {
                return &d;
            }
        }
        return nullptr;
    }

    [[nodiscard]] const L5GovernedAsset* FindAsset(std::string_view asset_id) const noexcept {
        for (const auto& a : governed_assets_) {
            if (a.asset_id == asset_id) {
                return &a;
            }
        }
        return nullptr;
    }

    [[nodiscard]] L5GovernedAsset* FindAssetMutable(std::string_view asset_id) noexcept {
        for (auto& a : governed_assets_) {
            if (a.asset_id == asset_id) {
                return &a;
            }
        }
        return nullptr;
    }

    [[nodiscard]] L4TaskItem* FindTaskMutable(std::string_view task_id) noexcept {
        for (auto& wf : workflows_) {
            for (auto& t : wf.tasks) {
                if (t.task_id == task_id) {
                    return &t;
                }
            }
        }
        return nullptr;
    }

    [[nodiscard]] const L4WorkflowInstance* FindWorkflow(std::string_view wf_id) const noexcept {
        for (const auto& wf : workflows_) {
            if (wf.workflow_id == wf_id) {
                return &wf;
            }
        }
        return nullptr;
    }

    [[nodiscard]] L4WorkflowInstance* FindWorkflowMutable(std::string_view wf_id) noexcept {
        for (auto& wf : workflows_) {
            if (wf.workflow_id == wf_id) {
                return &wf;
            }
        }
        return nullptr;
    }

    [[nodiscard]] bool HasWorkflow(std::string_view wf_id) const noexcept {
        return FindWorkflow(wf_id) != nullptr;
    }

    [[nodiscard]] CultivarWorkflowSpec ResolveCultivarSpecOrDefault(std::string_view cultivar_id) const {
        if (const CultivarWorkflowSpec* found = FindCultivar(cultivar_id)) {
            return *found;
        }
        if (const CultivarWorkflowSpec* selected = FindCultivar(selected_garden_cultivar_id_)) {
            return *selected;
        }
        if (!cultivars_.empty()) {
            return cultivars_.front();
        }
        CultivarWorkflowSpec fallback{};
        fallback.cultivar_id = "cultivar:chrono_virtual_pea";
        fallback.title = "Chrono-Pod Virtual Pea (Sim-Tuned Fast Cycle)";
        fallback.species_binomial = "Pisum virtualis";
        return fallback;
    }

    // ------------------------------------------------------------------------
    // Solo Steward 4-Node Personal Homestead Cluster & BPMN Token Choreography
    // ------------------------------------------------------------------------

    void InitializeHomesteadClusterNodes() {
        auto add_node = [&](const char* node_id,
                            const char* node_did,
                            const char* title,
                            const char* zone_label,
                            const char* daemons_hosted,
                            const char* role_summary,
                            uint64_t initial_exergy_uj) {
            HomesteadClusterNode cn{};
            cn.node_id = node_id;
            cn.node_did = node_did;
            cn.title = title;
            cn.zone_label = zone_label;
            cn.daemons_hosted = daemons_hosted;
            cn.role_summary = role_summary;
            cn.active_token_summary = "Idle — awaiting L5 governance or BPMN token";
            cn.inventory_summary = "Baseline R0 ready";
            cn.accounted_exergy_uj = initial_exergy_uj;
            cn.is_online = 1U;
            cluster_nodes_.push_back(cn);
        };

        add_node(
            "node-01:office-coordinator",
            "did:mesh:lot402:node-01-office",
            "Node 01 — Office Sovereign Coordinator",
            "Office Workstation (13, 1, 3)",
            "col-gatewayd · col-commonsd · col-kmsd · col-adversaryd",
            "L6/L5 Policy Coordination, KMS Identity & Solar/Cistern Stewardship",
            30600000000ULL
        );
        add_node(
            "node-02:garage-fab",
            "did:mesh:lot402:node-02-garage",
            "Node 02 — Garage Mini-Fab & Joinery Node",
            "Garage Joinery Bench (9, 1, 13)",
            "col-execd (fab-lane) · col-storaged",
            "On-demand Raised Bed Milling, Compost/Kiln Hardware, Poultry Coops & L7 Woodwork",
            0ULL
        );
        add_node(
            "node-03:garden-agro",
            "did:mesh:lot402:node-03-garden",
            "Node 03 — Backyard Agroecology & Soil Node",
            "Backyard Keyline & Coop (20, 1, 5)",
            "col-telemetryd (soil_probe_v1 / irrigation_valve_v1) · col-execd (agro-lane) · col-meshd",
            "Keyline Pea Cultivation, Thermophilic Compost, TLUD Biochar & Pasture Poultry",
            0ULL
        );
        add_node(
            "node-04:hearth-galley",
            "did:mesh:lot402:node-04-hearth",
            "Node 04 — Kitchen Induction Hearth & Galley Node",
            "Kitchen Induction Hearth (11, 1, 3)",
            "col-execd (hearth-lane) · col-telemetryd (culinary_vessel_v1)",
            "24h Collagen Bone Broth, >=74C Bigelow Pea Soup & Stainless Tiffin Service",
            0ULL
        );

        EmitLocalClusterChoreographyEvent(
            "node-01:office-coordinator",
            "Node 01 — Office Sovereign Coordinator",
            "CLUSTER_BOOT",
            "Solo 4-Node Homestead Cluster booted on Lot 402 (node-01..node-04 online over local L3 mesh; external R1 peer swarm deferred).",
            0ULL
        );
    }

    void EmitLocalClusterChoreographyEvent(
        const std::string& node_id,
        const std::string& node_title,
        const std::string& category,
        const std::string& summary,
        uint64_t exergy_delta_uj
    ) {
        L3MeshWeatherEvent ev{};
        ev.event_id = "l3-local:" + std::to_string(current_tick_) + ":" +
                      std::to_string(ledger_.mesh_weather_feed.size() + 1U);
        ev.cluster_id = node_id;
        ev.cluster_title = node_title;
        ev.event_category = category;
        ev.summary = summary;
        ev.exergy_delta_uj = exergy_delta_uj;
        ev.tick = current_tick_;
        PushMeshWeatherEvent(ev);
    }

    void AccountNodeTaskCompletion(const std::string& node_id, uint64_t exergy_uj) {
        for (auto& cn : cluster_nodes_) {
            if (cn.node_id == node_id) {
                ++cn.completed_task_count;
                cn.accounted_exergy_uj += exergy_uj;
                return;
            }
        }
        if (!cluster_nodes_.empty()) {
            ++cluster_nodes_.front().completed_task_count;
            cluster_nodes_.front().accounted_exergy_uj += exergy_uj;
        }
    }

    void ApplyPhysicalChoreographyOnTaskCompleted(const std::string& task_id, uint64_t exergy_uj) {
        if (task_id == "task:fab_1_mill_raised_bed") {
            ++raised_bed_frames_built_;
            EmitLocalClusterChoreographyEvent(
                "node-02:garage-fab",
                "Node 02 (Garage Fab) -> Node 03 (Garden Agro)",
                "L3_INTERNODE_HANDOFF",
                "Milled cedar raised bed frame delivered to Backyard Keyline plot; Pea Cultivation Step 1 token unlocked.",
                exergy_uj
            );
        } else if (task_id == "task:fab_2_assemble_compost_kiln") {
            ++compost_kiln_units_built_;
            EmitLocalClusterChoreographyEvent(
                "node-02:garage-fab",
                "Node 02 (Garage Fab) -> Node 03 (Garden Agro)",
                "L3_INTERNODE_HANDOFF",
                "Insulated compost drum & TLUD pyrolysis kiln commissioned on Backyard Node 03.",
                exergy_uj
            );
        } else if (task_id == "task:fab_3_build_poultry_coop") {
            ++poultry_coops_built_;
            EmitLocalClusterChoreographyEvent(
                "node-02:garage-fab",
                "Node 02 (Garage Fab) -> Node 03 (Garden Agro)",
                "L3_INTERNODE_HANDOFF",
                "Elevated cedar poultry coop & BSFL bioconversion feeder commissioned on Backyard Node 03.",
                exergy_uj
            );
        } else if (task_id == "task:fab_4_l7_custom_woodwork_order") {
            EmitLocalClusterChoreographyEvent(
                "node-02:garage-fab",
                "Node 02 — Garage Mini-Fab & Joinery Node",
                "L7_BILL_OFFSET",
                "Completed custom mortise-and-tenon woodwork order (+$180.00 Zero-Bill offset credited to Node 01 ledger).",
                exergy_uj
            );
        } else if (task_id == "task:pea_3_rhizobium_sow") {
            if (L4WorkflowInstance* wf = FindWorkflowMutable("wf:pea_cultivation")) {
                const CultivarWorkflowSpec spec = ResolveCultivarSpecOrDefault(wf->input_cultivar_id);
                const bool full_l2_auto =
                    l2_telemetry_.IsRegisterOnline("soil_probe_v1") &&
                    l2_telemetry_.IsRegisterOnline("irrigation_valve_v1");

                wf->crop_sown = 1U;
                wf->crop_sown_sim_s = sim_clock_seconds_;
                wf->last_growth_eval_sim_s = sim_clock_seconds_;
                wf->crop_growth_bp = 200U;
                wf->estimated_moisture_bp = full_l2_auto ? spec.target_opt_vwc_bp : 2900U;
                wf->estimated_nitrogen_ppm = 72U;
                wf->growth_sub_accum = 0U;
                wf->moisture_et_sub_accum = 0U;
                wf->nitrogen_sub_accum = 0U;
                wf->verification_checks_completed = 0U;
                wf->auto_l2_checks_completed = full_l2_auto ? 1U : 0U;
                wf->next_verification_due_sim_s =
                    full_l2_auto
                        ? (sim_clock_seconds_ + static_cast<uint64_t>(spec.verification_interval_sim_s))
                        : sim_clock_seconds_;
                wf->phenology_stage_label =
                    ComputePhenologyLabel(wf->crop_growth_bp, wf->crop_sown, spec.harvest_mature_bp);

                if (L2SensorRegister* soil = l2_telemetry_.FindRegisterMutable("soil_probe_v1")) {
                    soil->crop_growth_bp = wf->crop_growth_bp;
                    if (full_l2_auto) {
                        soil->moisture_bp = wf->estimated_moisture_bp;
                    }
                }

                EmitLocalClusterChoreographyEvent(
                    "node-03:garden-agro",
                    "Node 03 — Backyard Agroecology & Soil Node",
                    "AGRO_CULTIVAR_SOWN",
                    "Sowed " + spec.title + " (" + spec.species_binomial + ", ideal cycle " +
                        FormatSimDurationCompact(spec.maturity_sim_seconds) +
                        ", check cadence " + FormatSimDurationCompact(spec.verification_interval_sim_s) + ").",
                    exergy_uj
                );
            }
        } else if (task_id == "task:pea_5_harvest_pods_roots") {
            const L4WorkflowInstance* wf = FindWorkflow("wf:pea_cultivation");
            const CultivarWorkflowSpec spec = ResolveCultivarSpecOrDefault(
                (wf != nullptr && !wf->input_cultivar_id.empty())
                    ? std::string_view(wf->input_cultivar_id)
                    : std::string_view(selected_garden_cultivar_id_));
            const uint32_t leg_yield  = std::max<uint32_t>(1U, spec.harvest_legume_batches);
            const uint32_t root_yield = std::max<uint32_t>(1U, spec.harvest_mirepoix_batches);
            harvested_split_pea_batches_ += leg_yield;
            harvested_mirepoix_batches_  += root_yield;
            EmitLocalClusterChoreographyEvent(
                "node-03:garden-agro",
                "Node 03 (Garden Agro) -> Node 04 (Hearth Galley)",
                "L3_INTERNODE_HANDOFF",
                "Harvested +" + std::to_string(leg_yield) + " legume batch(es) & +" +
                    std::to_string(root_yield) + " mirepoix batch(es) from " + spec.title +
                    "; transferred to Kitchen Hearth Node 04 (Pea Soup Step 1 unlocked).",
                exergy_uj
            );
        } else if (task_id == "task:mu_2_thermophilic_compost_55c") {
            ++thermophilic_compost_batches_;
            EmitLocalClusterChoreographyEvent(
                "node-03:garden-agro",
                "Node 03 — Backyard Agroecology & Soil Node",
                "SOIL_CLOSURE",
                "Sanitized >=55C (328150 mK) thermophilic compost batch cured for keyline Terra Preta.",
                exergy_uj
            );
        } else if (task_id == "task:mu_3_tlud_biochar_charge") {
            ++biochar_batches_;
            EmitLocalClusterChoreographyEvent(
                "node-03:garden-agro",
                "Node 03 — Backyard Agroecology & Soil Node",
                "SOIL_CLOSURE",
                "Pyrolyzed >=8000 bp fixed-carbon TLUD biochar batch and charged Terra Preta matrix.",
                exergy_uj
            );
        } else if (task_id == "task:mu_1_feed_bsfl_collect_eggs") {
            ++poultry_bones_and_eggs_batches_;
            EmitLocalClusterChoreographyEvent(
                "node-03:garden-agro",
                "Node 03 (Garden Agro) -> Node 04 (Hearth Galley)",
                "L3_INTERNODE_HANDOFF",
                "Pasture eggs & rotational poultry broth bones transferred to Kitchen Hearth Node 04.",
                exergy_uj
            );
        } else if (task_id == "task:soup_2_simmer_bone_broth") {
            ++simmered_bone_broth_batches_;
            EmitLocalClusterChoreographyEvent(
                "node-04:hearth-galley",
                "Node 04 — Kitchen Induction Hearth & Galley Node",
                "CULINARY_BATCH",
                "24-hour collagen bone broth batch simmered (>=347150 mK) with bone-ash calcium grit returned to Node 03.",
                exergy_uj
            );
        } else if (task_id == "task:soup_4_package_distribute_tiffins") {
            ++served_pea_soup_tiffin_batches_;
            EmitLocalClusterChoreographyEvent(
                "node-04:hearth-galley",
                "Node 04 — Kitchen Induction Hearth & Galley Node",
                "HEARTH_TIFFIN_BATCH",
                "Bigelow-pasteurized Pea Soup & Collagen Bone Broth packaged in 304-stainless tiffins.",
                exergy_uj
            );
        }
    }

    void StepGardenWorkflowBiologyAndTimers() {
        const L2SensorRegister* soil_reg = l2_telemetry_.FindRegister("soil_probe_v1");
        const bool probe_online = (soil_reg != nullptr) && (soil_reg->is_online != 0U);
        const bool valve_online = l2_telemetry_.IsRegisterOnline("irrigation_valve_v1");
        const bool full_l2_auto = probe_online && valve_online;

        for (auto& wf : workflows_) {
            if (wf.workflow_id != "wf:pea_cultivation" || wf.is_active == 0U || wf.crop_sown == 0U) {
                continue;
            }

            const CultivarWorkflowSpec spec = ResolveCultivarSpecOrDefault(wf.input_cultivar_id);

            L4TaskItem* step4 = nullptr;
            for (auto& t : wf.tasks) {
                if (t.task_id == "task:pea_4_irrigate_weed") {
                    step4 = &t;
                    break;
                }
            }

            if (probe_online) {
                if (soil_reg->crop_growth_bp > wf.crop_growth_bp) {
                    wf.crop_growth_bp = std::min(10000U, soil_reg->crop_growth_bp);
                }
                if (soil_reg->moisture_bp > 0U && wf.last_growth_eval_sim_s == sim_clock_seconds_) {
                    wf.estimated_moisture_bp = soil_reg->moisture_bp;
                }
            }

            if (wf.last_growth_eval_sim_s == 0ULL) {
                wf.last_growth_eval_sim_s = sim_clock_seconds_;
            }

            // If L2 came online after sowing while next_verification_due_sim_s was due, satisfy check immediately via L2
            if (full_l2_auto && step4 != nullptr &&
                step4->task_commitment_state != TaskCommitmentState::COMPLETED &&
                wf.next_verification_due_sim_s <= sim_clock_seconds_ &&
                wf.last_growth_eval_sim_s == sim_clock_seconds_) {
                wf.estimated_moisture_bp = std::max(wf.estimated_moisture_bp, spec.target_opt_vwc_bp);
                ++wf.auto_l2_checks_completed;
                wf.next_verification_due_sim_s =
                    sim_clock_seconds_ + static_cast<uint64_t>(std::max(1U, spec.verification_interval_sim_s));
            }

            // Pure-integer sub-stepping (<= 1800 s chunks) of cultivar thermal/moisture/N-P-K growth & ET
            while (wf.last_growth_eval_sim_s < sim_clock_seconds_) {
                const uint64_t remaining_s = sim_clock_seconds_ - wf.last_growth_eval_sim_s;
                const uint64_t chunk_s = std::min<uint64_t>(1800ULL, remaining_s);
                wf.last_growth_eval_sim_s += chunk_s;

                uint32_t temp_mk = ambient_temp_override_mk_;
                if (temp_mk == 0U && probe_online && soil_reg->temperature_mk > 0U) {
                    temp_mk = soil_reg->temperature_mk;
                }
                if (temp_mk == 0U) {
                    const uint64_t rem_day_s = wf.last_growth_eval_sim_s % 86400ULL;
                    const uint64_t shifted_s = (rem_day_s + 86400ULL - 14400ULL) % 86400ULL;
                    const uint64_t dist_s =
                        (shifted_s <= 43200ULL) ? shifted_s : (86400ULL - shifted_s);
                    temp_mk = 287150U + static_cast<uint32_t>((9000ULL * dist_s) / 43200ULL);
                }

                if (full_l2_auto && step4 != nullptr &&
                    step4->task_commitment_state != TaskCommitmentState::COMPLETED) {
                    if (wf.estimated_moisture_bp < spec.target_min_vwc_bp + 200U) {
                        wf.estimated_moisture_bp = spec.target_opt_vwc_bp;
                    }
                    while (wf.next_verification_due_sim_s <= wf.last_growth_eval_sim_s) {
                        ++wf.auto_l2_checks_completed;
                        wf.estimated_moisture_bp = std::max(wf.estimated_moisture_bp, spec.target_opt_vwc_bp);
                        wf.next_verification_due_sim_s +=
                            static_cast<uint64_t>(std::max(1U, spec.verification_interval_sim_s));
                    }
                }

                const uint32_t f_temp  = EvaluateStackCultivarTempFactorBp(temp_mk, spec);
                const uint32_t f_moist = EvaluateStackCultivarMoistureFactorBp(wf.estimated_moisture_bp, spec);
                const uint32_t f_npk =
                    (spec.fixes_atmospheric_n2 != 0U)
                        ? ((wf.estimated_nitrogen_ppm >= 15U) ? 10000U : 7500U)
                        : ((wf.estimated_nitrogen_ppm >= 30U)
                               ? 10000U
                               : (4000U + (wf.estimated_nitrogen_ppm * 6000U) / 30U));

                if (wf.crop_growth_bp < 10000U && f_temp > 0U && f_moist > 0U) {
                    const uint64_t combined_bp =
                        (((static_cast<uint64_t>(f_temp) * static_cast<uint64_t>(f_moist)) / 10000ULL) *
                         static_cast<uint64_t>(f_npk)) /
                        10000ULL;
                    const uint64_t mat_s =
                        static_cast<uint64_t>(std::max(1U, spec.maturity_sim_seconds));
                    const uint64_t total_num =
                        static_cast<uint64_t>(wf.growth_sub_accum) + (combined_bp * chunk_s);
                    const uint32_t delta_growth_bp = static_cast<uint32_t>(total_num / mat_s);
                    wf.growth_sub_accum = static_cast<uint32_t>(total_num % mat_s);

                    if (delta_growth_bp > 0U) {
                        const uint32_t prev_band = wf.crop_growth_bp / 250U;
                        wf.crop_growth_bp = std::min(10000U, wf.crop_growth_bp + delta_growth_bp);
                        const uint32_t next_band = wf.crop_growth_bp / 250U;
                        if (next_band > prev_band && spec.n_draw_per_band_ppm > 0U) {
                            const uint32_t draw =
                                (next_band - prev_band) * static_cast<uint32_t>(spec.n_draw_per_band_ppm);
                            wf.estimated_nitrogen_ppm =
                                (wf.estimated_nitrogen_ppm > draw + 4U)
                                    ? (wf.estimated_nitrogen_ppm - draw)
                                    : 4U;
                        }
                    }

                    if (spec.fixes_atmospheric_n2 != 0U && spec.n_fixation_rate_bp > 0U &&
                        wf.crop_growth_bp >= 1500U && wf.estimated_nitrogen_ppm < 180U) {
                        const uint64_t n_num =
                            static_cast<uint64_t>(wf.nitrogen_sub_accum) +
                            (static_cast<uint64_t>(spec.n_fixation_rate_bp) * chunk_s);
                        constexpr uint64_t kFixDenom = 10000ULL * 3600ULL;
                        const uint32_t add_n = static_cast<uint32_t>(n_num / kFixDenom);
                        wf.nitrogen_sub_accum = static_cast<uint32_t>(n_num % kFixDenom);
                        wf.estimated_nitrogen_ppm = std::min(180U, wf.estimated_nitrogen_ppm + add_n);
                    }
                }

                // Pure-integer evapotranspiration decay: 95 bp/hr * (et_multiplier_bp / 10000)
                const uint64_t et_num =
                    static_cast<uint64_t>(wf.moisture_et_sub_accum) +
                    (95ULL * static_cast<uint64_t>(spec.et_multiplier_bp) * chunk_s);
                constexpr uint64_t kEtDenom = 3600ULL * 10000ULL;
                const uint32_t et_loss_bp = static_cast<uint32_t>(et_num / kEtDenom);
                wf.moisture_et_sub_accum = static_cast<uint32_t>(et_num % kEtDenom);
                if (et_loss_bp > 0U) {
                    wf.estimated_moisture_bp =
                        (wf.estimated_moisture_bp > 700U + et_loss_bp)
                            ? (wf.estimated_moisture_bp - et_loss_bp)
                            : 700U;
                }
            }

            wf.phenology_stage_label =
                ComputePhenologyLabel(wf.crop_growth_bp, wf.crop_sown, spec.harvest_mature_bp);

            if (L2SensorRegister* mut_soil = l2_telemetry_.FindRegisterMutable("soil_probe_v1")) {
                if (mut_soil->is_online != 0U) {
                    mut_soil->crop_growth_bp = std::max(mut_soil->crop_growth_bp, wf.crop_growth_bp);
                    if (full_l2_auto) {
                        mut_soil->moisture_bp = wf.estimated_moisture_bp;
                        mut_soil->concentration_ppm = wf.estimated_nitrogen_ppm;
                    }
                }
            }

            // Closed-loop L2 maturity verification: when soil_probe_v1 + irrigation_valve_v1 are online,
            // Step 4 auto-completes once biological maturity (>= harvest_mature_bp) is reached!
            if (full_l2_auto && step4 != nullptr &&
                step4->task_commitment_state != TaskCommitmentState::COMPLETED &&
                wf.crop_growth_bp >= spec.harvest_mature_bp) {
                step4->task_commitment_state = TaskCommitmentState::COMPLETED;
                step4->bpmn_token_state = BpmnTokenState::COMPLETED;
                step4->timer_remaining_sim_s = 0ULL;
                step4->precondition_blocker_note.clear();
                step4->last_state_change_tick = current_tick_;
                ledger_.total_exergy_accounted_uj += step4->exergy_cost_uj;
                AccountNodeTaskCompletion(step4->assigned_node_id, step4->exergy_cost_uj);

                EmitLocalClusterChoreographyEvent(
                    "node-03:garden-agro",
                    "Node 03 — Backyard Agroecology & Soil Node",
                    "L2_AUTO_MATURITY_UNLOCK",
                    "Closed-loop L2 (soil_probe_v1 + irrigation_valve_v1) completed " +
                        std::to_string(wf.auto_l2_checks_completed) + " timed checks on " +
                        spec.title + " and verified " + std::to_string(wf.crop_growth_bp / 100U) +
                        "% maturity; Step 5 Harvest BPMN token unlocked.",
                    step4->exergy_cost_uj
                );
            }
        }
    }

    void EvaluateBpmnTokensAndDependencies() {
        StepGardenWorkflowBiologyAndTimers();

        for (uint32_t pass = 0U; pass < 4U; ++pass) {
            std::vector<std::string> pending_spawns{};

            for (auto& wf : workflows_) {
                if (wf.is_active == 0U || wf.tasks.empty()) {
                    continue;
                }

                // Locate the first incomplete step in BPMN sequence flow order
                size_t token_candidate_idx = wf.tasks.size();
                for (size_t i = 0U; i < wf.tasks.size(); ++i) {
                    if (wf.tasks[i].task_commitment_state == TaskCommitmentState::COMPLETED) {
                        wf.tasks[i].bpmn_token_state = BpmnTokenState::COMPLETED;
                        wf.tasks[i].timer_remaining_sim_s = 0ULL;
                        wf.tasks[i].precondition_blocker_note.clear();
                    } else if (token_candidate_idx == wf.tasks.size()) {
                        token_candidate_idx = i;
                    }
                }

                // If every step in this workflow cycle is completed, no active token remains
                if (token_candidate_idx == wf.tasks.size()) {
                    continue;
                }

                const uint32_t gating_step_number = wf.tasks[token_candidate_idx].step_index;

                // Strictly lock all downstream steps behind sequence flow
                for (size_t i = token_candidate_idx + 1U; i < wf.tasks.size(); ++i) {
                    auto& downstream = wf.tasks[i];
                    if (downstream.task_commitment_state == TaskCommitmentState::COMPLETED) {
                        downstream.bpmn_token_state = BpmnTokenState::COMPLETED;
                        downstream.timer_remaining_sim_s = 0ULL;
                        downstream.precondition_blocker_note.clear();
                    } else {
                        downstream.bpmn_token_state = BpmnTokenState::WAITING_SEQUENCE_FLOW;
                        downstream.timer_remaining_sim_s = 0ULL;
                        downstream.precondition_blocker_note =
                            "Queued behind Step " + std::to_string(gating_step_number) + " in BPMN sequence flow.";
                        if (downstream.task_commitment_state != TaskCommitmentState::UNASSIGNED) {
                            downstream.task_commitment_state = TaskCommitmentState::UNASSIGNED;
                            downstream.claimed_by_cluster_id.clear();
                        }
                    }
                }

                // Evaluate physical, cross-node, and BPMN IntermediateCatchEvent timer gates on candidate step
                auto& candidate = wf.tasks[token_candidate_idx];
                std::string blocker{};

                if (candidate.task_id == "task:pea_1_plow_amend" && raised_bed_frames_built_ == 0U) {
                    blocker = "Blocked: requires Raised Cedar Bed Frame from Node 02 (Garage Fab: Mill Raised Bed Frame).";
                    if (!HasWorkflow("wf:fab_raised_bed")) {
                        pending_spawns.emplace_back("wf:fab_raised_bed");
                    }
                } else if ((candidate.task_id == "task:mu_2_thermophilic_compost_55c" ||
                            candidate.task_id == "task:mu_3_tlud_biochar_charge") &&
                           compost_kiln_units_built_ == 0U) {
                    blocker = "Blocked: requires Compost Drum & TLUD Kiln from Node 02 (Garage Fab: Assemble Compost & Kiln).";
                    if (!HasWorkflow("wf:fab_compost_kiln")) {
                        pending_spawns.emplace_back("wf:fab_compost_kiln");
                    }
                } else if (candidate.task_id == "task:mu_1_feed_bsfl_collect_eggs" && poultry_coops_built_ == 0U) {
                    blocker = "Blocked: requires Elevated Timber Poultry Coop from Node 02 (Garage Fab: Build Poultry Coop).";
                    if (!HasWorkflow("wf:fab_poultry_coop")) {
                        pending_spawns.emplace_back("wf:fab_poultry_coop");
                    }
                } else if (candidate.task_id == "task:soup_1_gather_inputs" && harvested_split_pea_batches_ == 0U) {
                    if (HasWorkflow("wf:pea_cultivation")) {
                        blocker = "Blocked: waiting on Node 03 (Garden Agro) Step 5 — Harvest Mature Legumes & Mirepoix.";
                    } else {
                        blocker = "Blocked: no harvested Split Peas in larder — approve Pea Cultivation in L6 Decisions first.";
                        EnsurePendingDecisionForAsset("asset:pea_cultivation", "need:split_peas");
                    }
                }

                if (!blocker.empty()) {
                    candidate.bpmn_token_state = BpmnTokenState::BLOCKED_BY_PRECONDITION;
                    candidate.timer_remaining_sim_s = 0ULL;
                    candidate.precondition_blocker_note = blocker;
                    if (candidate.task_commitment_state != TaskCommitmentState::UNASSIGNED) {
                        candidate.task_commitment_state = TaskCommitmentState::UNASSIGNED;
                        candidate.claimed_by_cluster_id.clear();
                    }
                } else if (candidate.task_id == "task:pea_4_irrigate_weed" &&
                           wf.crop_sown != 0U &&
                           sim_clock_seconds_ < wf.next_verification_due_sim_s) {
                    const uint64_t rem_s = wf.next_verification_due_sim_s - sim_clock_seconds_;
                    const bool full_l2_auto =
                        l2_telemetry_.IsRegisterOnline("soil_probe_v1") &&
                        l2_telemetry_.IsRegisterOnline("irrigation_valve_v1");
                    candidate.bpmn_token_state = BpmnTokenState::WAITING_TIMER;
                    candidate.timer_due_sim_s = wf.next_verification_due_sim_s;
                    candidate.timer_remaining_sim_s = rem_s;
                    candidate.timer_interval_sim_s = wf.verification_interval_sim_s;
                    candidate.verification_cycle_index =
                        static_cast<uint32_t>(wf.verification_checks_completed + wf.auto_l2_checks_completed) + 1U;
                    candidate.precondition_blocker_note =
                        (full_l2_auto ? "L2 Automated Growth Timer: " : "BPMN Growth Timer: ") +
                        wf.input_cultivar_title + " at " + std::to_string(wf.crop_growth_bp / 100U) +
                        "% (" + wf.phenology_stage_label + ") — next check in " +
                        FormatSimDurationCompact(rem_s) + ".";
                    if (candidate.task_commitment_state != TaskCommitmentState::UNASSIGNED) {
                        candidate.task_commitment_state = TaskCommitmentState::UNASSIGNED;
                        candidate.claimed_by_cluster_id.clear();
                    }
                } else {
                    candidate.bpmn_token_state = BpmnTokenState::ACTIVE_TOKEN;
                    candidate.timer_remaining_sim_s = 0ULL;
                    if (candidate.task_id == "task:pea_4_irrigate_weed") {
                        candidate.timer_due_sim_s = wf.next_verification_due_sim_s;
                        candidate.timer_interval_sim_s = wf.verification_interval_sim_s;
                        candidate.verification_cycle_index =
                            static_cast<uint32_t>(wf.verification_checks_completed + wf.auto_l2_checks_completed) + 1U;
                    }
                    candidate.precondition_blocker_note.clear();
                    if (IsExternalR1SwarmEnabled() &&
                        candidate.task_commitment_state == TaskCommitmentState::UNASSIGNED) {
                        const L5GovernedAsset* asset = FindAsset(candidate.asset_id);
                        if (asset != nullptr && asset->task_release_ring == TaskReleaseRing::R1_PEER_CLUSTERS) {
                            candidate.task_commitment_state = TaskCommitmentState::RELEASED_TO_R1_PEERS;
                            candidate.last_state_change_tick = current_tick_;
                        }
                    }
                }
            }

            if (pending_spawns.empty()) {
                break;
            }
            for (const auto& req_wf_id : pending_spawns) {
                if (!HasWorkflow(req_wf_id)) {
                    EnsureSpecificWorkflowSpawned(req_wf_id);
                    EmitLocalClusterChoreographyEvent(
                        "node-03:garden-agro",
                        "Node 03 (Garden Agro) -> Node 02 (Garage Fab)",
                        "L4_CROSS_NODE_REQUISITION",
                        "BPMN precondition requisitioned prerequisite Garage Fab workflow '" + req_wf_id + "' on Node 02.",
                        0ULL
                    );
                }
            }
        }
    }

    void RefreshClusterNodeSummaries() {
        for (auto& cn : cluster_nodes_) {
            cn.active_workflow_count = 0U;
            cn.online_l2_pin_count = 0U;
            std::string active_tokens{};
            std::string timer_notes{};
            std::string blocked_notes{};

            for (const auto& wf : workflows_) {
                if (wf.assigned_node_id != cn.node_id || wf.is_active == 0U) {
                    continue;
                }
                bool has_incomplete = false;
                for (const auto& t : wf.tasks) {
                    if (t.task_commitment_state != TaskCommitmentState::COMPLETED) {
                        has_incomplete = true;
                    }
                    if (t.bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN) {
                        if (!active_tokens.empty()) {
                            active_tokens += " | ";
                        }
                        active_tokens += t.title;
                    } else if (t.bpmn_token_state == BpmnTokenState::WAITING_TIMER &&
                               timer_notes.empty()) {
                        timer_notes = t.precondition_blocker_note;
                    } else if (t.bpmn_token_state == BpmnTokenState::BLOCKED_BY_PRECONDITION &&
                               blocked_notes.empty()) {
                        blocked_notes = t.precondition_blocker_note;
                    }
                }
                if (has_incomplete) {
                    ++cn.active_workflow_count;
                }
            }

            if (!active_tokens.empty()) {
                cn.active_token_summary = "ACTIVE TOKEN: " + active_tokens;
            } else if (!timer_notes.empty()) {
                cn.active_token_summary = timer_notes;
            } else if (!blocked_notes.empty()) {
                cn.active_token_summary = blocked_notes;
            } else {
                cn.active_token_summary = "Idle — 0 active BPMN tokens";
            }

            if (cn.node_id == "node-01:office-coordinator") {
                if (l2_telemetry_.IsRegisterOnline("solar_bus_meter_v1")) {
                    ++cn.online_l2_pin_count;
                }
                uint32_t pending_dec_count = 0U;
                for (const auto& d : pending_decisions_) {
                    if (d.status == DecisionStatus::PENDING) {
                        ++pending_dec_count;
                    }
                }
                cn.inventory_summary =
                    std::to_string(pending_dec_count) + " pending L5->L6 decisions · " +
                    (IsExternalR1SwarmEnabled() ? "R1 Swarm ON" : "Solo 4-Node Cluster");
            } else if (cn.node_id == "node-02:garage-fab") {
                cn.inventory_summary =
                    "Built: " + std::to_string(raised_bed_frames_built_) + " Bed Frame(s) · " +
                    std::to_string(compost_kiln_units_built_) + " Compost/Kiln · " +
                    std::to_string(poultry_coops_built_) + " Coop(s)";
            } else if (cn.node_id == "node-03:garden-agro") {
                if (l2_telemetry_.IsRegisterOnline("soil_probe_v1")) {
                    ++cn.online_l2_pin_count;
                }
                if (l2_telemetry_.IsRegisterOnline("irrigation_valve_v1")) {
                    ++cn.online_l2_pin_count;
                }
                if (l2_telemetry_.IsRegisterOnline("compost_drum_probe_v1")) {
                    ++cn.online_l2_pin_count;
                }
                if (l2_telemetry_.IsRegisterOnline("kiln_thermocouple_v1")) {
                    ++cn.online_l2_pin_count;
                }
                cn.inventory_summary =
                    "Legumes: " + std::to_string(harvested_split_pea_batches_) +
                    " · Mirepoix: " + std::to_string(harvested_mirepoix_batches_) +
                    " · Compost: " + std::to_string(thermophilic_compost_batches_) +
                    " · Biochar: " + std::to_string(biochar_batches_);
            } else if (cn.node_id == "node-04:hearth-galley") {
                if (l2_telemetry_.IsRegisterOnline("culinary_vessel_v1")) {
                    ++cn.online_l2_pin_count;
                }
                cn.inventory_summary =
                    "Bone Broth: " + std::to_string(simmered_bone_broth_batches_) +
                    " batch(es) · Pea Soup Tiffins Served: " +
                    std::to_string(served_pea_soup_tiffin_batches_) + " batch(es)";
            }
        }
    }

    // ------------------------------------------------------------------------
    // Initialization & On-Demand BPMN L4 Lifecycle Spawning
    // ------------------------------------------------------------------------

    void InitializePeerBccChannels() {
        auto add_peer = [&](const char* id, const char* title, uint32_t households) {
            L3PeerBccRecord rec{};
            rec.peer_cluster_id = id;
            rec.peer_cluster_title = title;
            rec.household_count = households;
            rec.credit_limit_uj = 200000000000LL;
            ledger_.peer_bcc_channels.push_back(rec);
        };
        add_peer("cluster:north_oak_commons", "North-Oak 5-Household Garden & Kitchen Cluster", 5U);
        add_peer("cluster:creekside_guild",   "Creekside 4-Garage Tool & Timber Cluster", 4U);
        add_peer("cluster:sunnyside_care_ring", "Sunnyside 6-Family Care & Hearth Cluster", 6U);
    }

    void InitializeGovernedAssets() {
        {
            L5GovernedAsset pea_garden{};
            pea_garden.asset_id = "asset:pea_cultivation";
            pea_garden.title = "Backyard Keyline Pea & Mirepoix Garden";
            pea_garden.governance_badge = "L5 Soil & Legume Stewardship";
            pea_garden.l7_fallback_channel = "L7 Produce & Dry Legume Supply ($6.00/batch)";
            pea_garden.local_commons_channel = "Local Keyline Pea Trellis & Root Bed ($0.00)";
            pea_garden.linked_need_ids = {"need:split_peas", "need:mirepoix_vegetables"};
            pea_garden.maintenance_exergy_uj = 3100000000ULL;
            pea_garden.monthly_l7_offset_potential_cents = 18000;
            pea_garden.steward_reputation_credit_bp = 220U;
            pea_garden.sourcing_mode = SourcingMode::L7_LEGACY_CHANNEL;
            pea_garden.distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
            pea_garden.task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            pea_garden.local_workflows_instantiated = 0U;
            governed_assets_.push_back(pea_garden);
        }
        {
            L5GovernedAsset soup_hearth{};
            soup_hearth.asset_id = "asset:pea_soup_hearth";
            soup_hearth.title = "Kitchen Pea Soup & Collagen Broth Hearth";
            soup_hearth.governance_badge = "R0/R1 Unconditional Nutrition Hearth";
            soup_hearth.l7_fallback_channel = "L7 Legacy Grocery Delivery ($14.00/order)";
            soup_hearth.local_commons_channel = "Local Induction Hearth & 304-Stainless Tiffins ($0.00)";
            soup_hearth.linked_need_ids = {"need:pea_soup", "need:collagen_bone_broth"};
            soup_hearth.maintenance_exergy_uj = 3400000000ULL;
            soup_hearth.monthly_l7_offset_potential_cents = 34000;
            soup_hearth.steward_reputation_credit_bp = 260U;
            soup_hearth.sourcing_mode = SourcingMode::L7_LEGACY_CHANNEL;
            soup_hearth.distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
            soup_hearth.task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            soup_hearth.local_workflows_instantiated = 0U;
            governed_assets_.push_back(soup_hearth);
        }
        {
            L5GovernedAsset garage_fab{};
            garage_fab.asset_id = "asset:garage_fab";
            garage_fab.title = "Garage Mini-Fab & Joinery Node";
            garage_fab.governance_badge = "L5 Tool Commons & L7 Bill Offset";
            garage_fab.l7_fallback_channel = "L7 Big-Box Hardware & Contractor Retail ($85.00/kit)";
            garage_fab.local_commons_channel = "Local Garage Workbench + L7 Custom Woodwork Service";
            garage_fab.linked_need_ids = {"need:raised_bed_frame"};
            garage_fab.maintenance_exergy_uj = 5000000000ULL;
            garage_fab.monthly_l7_offset_potential_cents = 36000;
            garage_fab.steward_reputation_credit_bp = 300U;
            garage_fab.sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
            garage_fab.distribution_policy = DistributionPolicy::COMMONS_PLUS_L7_BILL_OFFSET;
            garage_fab.task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            garage_fab.local_workflows_instantiated = 0U;
            governed_assets_.push_back(garage_fab);
        }
        {
            L5GovernedAsset poultry_soil{};
            poultry_soil.asset_id = "asset:poultry_and_soil";
            poultry_soil.title = "Pasture Poultry, Thermophilic Compost & Biochar Loop";
            poultry_soil.governance_badge = "Scenario Mu Trophic & Nutrient Closure";
            poultry_soil.l7_fallback_channel = "L7 Cold-Chain Eggs & Bagged Soil Supply ($27.00/cycle)";
            poultry_soil.local_commons_channel = "Local Timber Coop, Compost Drum & TLUD Biochar Kiln ($0.00)";
            poultry_soil.linked_need_ids = {"need:pasture_eggs", "need:thermophilic_compost", "need:pyrolysis_biochar"};
            poultry_soil.maintenance_exergy_uj = 1800000000ULL;
            poultry_soil.monthly_l7_offset_potential_cents = 15000;
            poultry_soil.steward_reputation_credit_bp = 240U;
            poultry_soil.sourcing_mode = SourcingMode::L7_LEGACY_CHANNEL;
            poultry_soil.distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
            poultry_soil.task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            poultry_soil.local_workflows_instantiated = 0U;
            governed_assets_.push_back(poultry_soil);
        }
        {
            L5GovernedAsset water_power{};
            water_power.asset_id = "asset:water_and_power";
            water_power.title = "Roof Solar Microgrid & Rainwater Cistern";
            water_power.governance_badge = "Tier-1 Ecological Floor & Exergy Bus";
            water_power.l7_fallback_channel = "L7 Municipal Water & Electric Grid ($0.28/kWh)";
            water_power.local_commons_channel = "Local 48V Solar Bus & Filtered Cistern ($0.00)";
            water_power.linked_need_ids = {"need:potable_water", "need:node_electricity"};
            water_power.maintenance_exergy_uj = 1200000000ULL;
            water_power.monthly_l7_offset_potential_cents = 14000;
            water_power.steward_reputation_credit_bp = 200U;
            water_power.sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
            water_power.distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
            water_power.task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            water_power.local_workflows_instantiated = 0U;
            governed_assets_.push_back(water_power);
        }
    }

    void InitializeCanonicalNeeds() {
        auto add_need = [&](const char* id,
                            const char* title,
                            const char* category,
                            const char* desc,
                            uint64_t exergy_uj,
                            int64_t l7_cents,
                            const char* l7_label,
                            const char* local_label,
                            const char* asset_id,
                            SourcingMode mode,
                            uint8_t r0_care,
                            uint32_t action_code) {
            L6NeedRecord n{};
            n.need_id = id;
            n.title = title;
            n.category = category;
            n.description = desc;
            n.l7_channel_label = l7_label;
            n.local_channel_label = local_label;
            n.governed_asset_id = asset_id;
            n.last_status_note = (mode == SourcingMode::L7_LEGACY_CHANNEL)
                                     ? "Sourced via L7 fallback pending L5 local governance approval."
                                     : "Ready for on-demand local L5/L4 workflow instantiation ($0 L7 spend).";
            n.exergy_cost_uj = exergy_uj;
            n.l7_unit_cost_cents = l7_cents;
            n.action_code = action_code;
            n.active_sourcing_mode = mode;
            n.is_r0_baseline_care = r0_care;
            needs_.push_back(n);
        };

        add_need("need:pea_soup", "Pea Soup", "R0 Nutrition & Hearth",
                 "Bigelow-pasteurized (>= 347150 mK) split-pea soup fortified with 24h collagen bone broth & mirepoix.",
                 4200000000ULL, 1400, "L7 Legacy Grocery Delivery ($14.00/batch)",
                 "Local Induction Hearth & R1 Stainless Tiffins ($0.00)",
                 "asset:pea_soup_hearth", SourcingMode::L7_LEGACY_CHANNEL, 1U, 22U);

        add_need("need:split_peas", "Split Peas", "Agroecology & Legumes",
                 "Nitrogen-fixing climbing green split peas (Pisum sativum) dried below 1400 bp moisture.",
                 2800000000ULL, 600, "L7 Bulk Dry Legume Supply ($6.00/2kg)",
                 "Local Backyard Keyline Pea Trellis ($0.00)",
                 "asset:pea_cultivation", SourcingMode::L7_LEGACY_CHANNEL, 1U, 13U);

        add_need("need:mirepoix_vegetables", "Mirepoix Vegetables", "Agroecology & Root Crops",
                 "Keyline understory carrots, onions, celery, garlic, and culinary herbs.",
                 1900000000ULL, 550, "L7 Supermarket Produce ($5.50/bundle)",
                 "Local Keyline Understory Root Bed ($0.00)",
                 "asset:pea_cultivation", SourcingMode::L7_LEGACY_CHANNEL, 1U, 13U);

        add_need("need:pasture_eggs", "Pasture Eggs", "Trophic Poultry & Protein",
                 "Heritage micro-flock eggs raised strictly on BSFL larvae, garden trimmings, and bone-ash calcium grit.",
                 3200000000ULL, 900, "L7 Cold-Chain Supermarket ($9.00/doz)",
                 "Local Backyard Timber Poultry Coop ($0.00)",
                 "asset:poultry_and_soil", SourcingMode::L7_LEGACY_CHANNEL, 1U, 20U);

        add_need("need:collagen_bone_broth", "Collagen Bone Broth", "R0 Nutrition & Hearth",
                 "24-hour acid-extracted collagen poultry bone broth (>= 347150 mK) with closed-loop bone-ash grit.",
                 3600000000ULL, 1100, "L7 Specialty Broth Grocer ($11.00/jar)",
                 "Local Kitchen Induction Stockpot ($0.00)",
                 "asset:pea_soup_hearth", SourcingMode::L7_LEGACY_CHANNEL, 1U, 21U);

        add_need("need:raised_bed_frame", "Raised Bed Frame", "Garage Joinery & Infrastructure",
                 "Mortised salvaged cedar raised garden bed frame and A-frame wire pea trellis.",
                 5400000000ULL, 8500, "L7 Big-Box Garden Hardware ($85.00/kit)",
                 "Local Garage Mini-Fab Workbench ($0.00)",
                 "asset:garage_fab", SourcingMode::R0_LOCAL_WORKFLOWS, 0U, 25U);

        add_need("need:thermophilic_compost", "Thermophilic Compost", "Scenario Mu Soil Ecology",
                 "EPA-503 Class-A sanitized compost held >= 328150 mK (55 C) for 72 hours.",
                 2400000000ULL, 1800, "L7 Bagged Garden Compost ($18.00/bag)",
                 "Local Insulated Rotating Compost Drum ($0.00)",
                 "asset:poultry_and_soil", SourcingMode::L7_LEGACY_CHANNEL, 0U, 24U);

        add_need("need:pyrolysis_biochar", "Pyrolysis Biochar", "Scenario Mu Soil Ecology",
                 "Smokeless TLUD pyrolysis biochar (773150-873150 mK, >= 8000 bp fixed carbon) for Terra Preta.",
                 3100000000ULL, 2400, "L7 Agricultural Amendment Supply ($24.00/sack)",
                 "Local Backyard TLUD Pyrolysis Kiln ($0.00)",
                 "asset:poultry_and_soil", SourcingMode::L7_LEGACY_CHANNEL, 0U, 24U);

        add_need("need:potable_water", "Potable Water", "Ecological Floor & Hydrology",
                 "Gravity-filtered rainwater cistern supply for hearth cooking, drinking, and keyline irrigation.",
                 1500000000ULL, 320, "L7 Municipal Water Utility ($3.20/unit)",
                 "Local Filtered Rainwater Cistern ($0.00)",
                 "asset:water_and_power", SourcingMode::R0_LOCAL_WORKFLOWS, 1U, 27U);

        add_need("need:node_electricity", "Node Electricity", "Exergy & Microgrid",
                 "8.5 kWh/day (30.6 MJ) 48V DC / 120V AC rooftop solar + LiFePO4 homelab & induction baseload.",
                 30600000000ULL, 238, "L7 Municipal Electric Grid ($0.28/kWh)",
                 "Local 400W Roof Solar Array + 48V Bus ($0.00)",
                 "asset:water_and_power", SourcingMode::R0_LOCAL_WORKFLOWS, 0U, 18U);
    }

    void TryEnrichNeedsFromJsonLd(const std::string& hint_path) {
        std::vector<std::string> candidate_paths;
        if (!hint_path.empty()) {
            candidate_paths.push_back(hint_path);
        }
        candidate_paths.emplace_back("core/6_intent/needs/pea_to_soup_needs.jsonld");
        candidate_paths.emplace_back("../core/6_intent/needs/pea_to_soup_needs.jsonld");
        candidate_paths.emplace_back("../../core/6_intent/needs/pea_to_soup_needs.jsonld");
        candidate_paths.emplace_back("../../../core/6_intent/needs/pea_to_soup_needs.jsonld");
        candidate_paths.emplace_back("../../../../core/6_intent/needs/pea_to_soup_needs.jsonld");

        for (const auto& path : candidate_paths) {
            std::ifstream in(path, std::ios::in | std::ios::binary);
            if (!in.is_open()) {
                continue;
            }
            std::ostringstream buf;
            buf << in.rdbuf();
            const std::string content = buf.str();
            if (content.empty()) {
                continue;
            }
            bootstrapped_from_jsonld_ = 1U;
            bootstrap_source_path_ = path;

            // Enrich canonical needs from matching @id entries in pea_to_soup_needs.jsonld
            for (auto& need : needs_) {
                std::string lookup_key = "\"@id\": \"" + need.need_id + "\"";
                size_t hit = content.find(lookup_key);
                if (hit == std::string::npos && need.need_id == "need:mirepoix_vegetables") {
                    hit = content.find("\"@id\": \"need:mirepoix_roots\"");
                }
                if (hit == std::string::npos && need.need_id == "need:collagen_bone_broth") {
                    hit = content.find("\"@id\": \"need:bone_broth\"");
                }
                if (hit == std::string::npos) {
                    hit = content.find(need.title);
                }
                if (hit != std::string::npos) {
                    const size_t next_id = content.find("\"@id\":", hit + 6U);
                    const size_t window_end = (next_id != std::string::npos)
                                                  ? std::min(next_id, hit + 3200U)
                                                  : std::min(content.size(), hit + 3200U);
                    std::string_view slice(content.data() + hit, window_end - hit);
                    const uint64_t parsed_uj = json_util::ExtractUint64(slice, "exergy_cost_uj", 0ULL);
                    if (parsed_uj > 0ULL) {
                        need.exergy_cost_uj = parsed_uj;
                    }
                    int64_t parsed_cents = json_util::ExtractInt64(slice, "l7_unit_cost_cents", -1);
                    if (parsed_cents < 0) {
                        parsed_cents = json_util::ExtractInt64(slice, "fiat_cost_usd_cents", -1);
                    }
                    if (parsed_cents >= 0) {
                        need.l7_unit_cost_cents = parsed_cents;
                    }
                    const std::string parsed_desc = json_util::ExtractString(slice, "schema:description");
                    if (!parsed_desc.empty()) {
                        need.description = parsed_desc;
                    }
                }
            }
            break;
        }
    }

    void InitializeGenesisDecisionsAndWorkflows() {
        // Strict BPMN Clean-Slate Invariant: Day 1 starts with ZERO unsolicited L4 workflows!
        // Workflows instantiate strictly when the Steward ratifies an L5 Governance Decision or requests a Need,
        // and cross-node prerequisite fabrication workflows auto-requisition via BPMN preconditions.

        {
            L6PendingDecision dec_peas{};
            dec_peas.decision_id = "dec:pea_cultivation";
            dec_peas.title = "L5 Proposal: Cultivate Split Peas & Mirepoix Locally on Node 03 (Garden Agro)";
            dec_peas.governed_asset_id = "asset:pea_cultivation";
            dec_peas.rationale =
                "Split Peas and Mirepoix Vegetables are currently procured via L7 supply chains. "
                "Approving spawns the 6-stage Pea Cultivation BPMN workflow on Node 03 (and automatically requisitions a Raised Bed Frame from Garage Node 02 first).";
            dec_peas.linked_need_ids = {"need:split_peas", "need:mirepoix_vegetables"};
            dec_peas.estimated_exergy_savings_uj = 4700000000ULL;
            dec_peas.estimated_monthly_l7_savings_cents = 18000;
            dec_peas.emitted_tick = current_tick_;
            dec_peas.proposed_sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
            dec_peas.proposed_distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
            dec_peas.proposed_task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            dec_peas.status = DecisionStatus::PENDING;
            pending_decisions_.push_back(dec_peas);
        }
        {
            L6PendingDecision dec_soup{};
            dec_soup.decision_id = "dec:pea_soup_hearth";
            dec_soup.title = "L5 Proposal: Commission Pea Soup & 24h Bone Broth Hearth on Node 04 (Hearth Galley)";
            dec_soup.governed_asset_id = "asset:pea_soup_hearth";
            dec_soup.rationale =
                "Eliminates $14.00/batch L7 delivery drain by instantiating the 4-stage Kitchen Pea Soup BPMN workflow "
                "(waits on harvested Split Peas from Node 03 -> 24h collagen bone broth -> >=74C Bigelow simmer -> stainless tiffins).";
            dec_soup.linked_need_ids = {"need:pea_soup", "need:collagen_bone_broth"};
            dec_soup.estimated_exergy_savings_uj = 7800000000ULL;
            dec_soup.estimated_monthly_l7_savings_cents = 34000;
            dec_soup.emitted_tick = current_tick_;
            dec_soup.proposed_sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
            dec_soup.proposed_distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
            dec_soup.proposed_task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            dec_soup.status = DecisionStatus::PENDING;
            pending_decisions_.push_back(dec_soup);
        }
        {
            L6PendingDecision dec_poultry{};
            dec_poultry.decision_id = "dec:poultry_and_soil";
            dec_poultry.title = "L5 Proposal: Commission Thermophilic Compost, TLUD Biochar & Pasture Poultry Loop";
            dec_poultry.governed_asset_id = "asset:poultry_and_soil";
            dec_poultry.rationale =
                "Closes Scenario Mu nitrogen/phosphorus/calcium cycle across Node 02 (Compost/Kiln & Coop fabrication) and Node 03 (Compost, Biochar & Pasture Eggs).";
            dec_poultry.linked_need_ids = {"need:pasture_eggs", "need:thermophilic_compost", "need:pyrolysis_biochar"};
            dec_poultry.estimated_exergy_savings_uj = 8700000000ULL;
            dec_poultry.estimated_monthly_l7_savings_cents = 15000;
            dec_poultry.emitted_tick = current_tick_;
            dec_poultry.proposed_sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
            dec_poultry.proposed_distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
            dec_poultry.proposed_task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            dec_poultry.status = DecisionStatus::PENDING;
            pending_decisions_.push_back(dec_poultry);
        }
        {
            L6PendingDecision dec_woodwork{};
            dec_woodwork.decision_id = "dec:garage_l7_woodwork";
            dec_woodwork.title = "L5 Proposal: Open Node 02 (Garage Fab) for L7 Custom Woodwork Orders (+$180/order Bill Offset)";
            dec_woodwork.governed_asset_id = "asset:garage_fab";
            dec_woodwork.rationale =
                "Authorizes discretionary Tier-5 custom mortise-and-tenon woodwork commissions on Garage Node 02 to offset $220/mo legacy utility bills toward $0 net cost.";
            dec_woodwork.linked_need_ids = {"need:raised_bed_frame"};
            dec_woodwork.estimated_exergy_savings_uj = 1500000000ULL;
            dec_woodwork.estimated_monthly_l7_savings_cents = 18000;
            dec_woodwork.emitted_tick = current_tick_;
            dec_woodwork.proposed_sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
            dec_woodwork.proposed_distribution_policy = DistributionPolicy::COMMONS_PLUS_L7_BILL_OFFSET;
            dec_woodwork.proposed_task_release_ring = TaskReleaseRing::R0_STEWARD_ONLY;
            dec_woodwork.status = DecisionStatus::PENDING;
            pending_decisions_.push_back(dec_woodwork);
        }
    }

    void EnsurePendingDecisionForAsset(const std::string& asset_id, const std::string& need_id) {
        if (asset_id.empty()) {
            return;
        }
        for (const auto& dec : pending_decisions_) {
            if (dec.governed_asset_id == asset_id && dec.status == DecisionStatus::PENDING) {
                return;
            }
        }
        const L5GovernedAsset* asset = FindAsset(asset_id);
        if (asset == nullptr) {
            return;
        }
        L6PendingDecision dec{};
        dec.decision_id = "dec:auto_" + MakeSlug(asset_id) + "_" + std::to_string(pending_decisions_.size() + 1U);
        dec.title = "L5 Proposal: Instantiate Local Workflows for " + asset->title;
        dec.governed_asset_id = asset_id;
        dec.rationale = "Repeated L7 fallback demand detected. Approve to instantiate local L4 workflows.";
        if (!need_id.empty()) {
            dec.linked_need_ids.push_back(need_id);
        }
        dec.estimated_exergy_savings_uj = asset->maintenance_exergy_uj;
        dec.estimated_monthly_l7_savings_cents = asset->monthly_l7_offset_potential_cents;
        dec.emitted_tick = current_tick_;
        dec.proposed_sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
        dec.proposed_distribution_policy = asset->distribution_policy;
        dec.proposed_task_release_ring = asset->task_release_ring;
        dec.status = DecisionStatus::PENDING;
        pending_decisions_.push_back(dec);
    }

    bool ApplyGovernancePolicyToAsset(
        const std::string& asset_id,
        SourcingMode sourcing_mode,
        DistributionPolicy distribution_policy,
        TaskReleaseRing task_release_ring
    ) {
        L5GovernedAsset* asset = FindAssetMutable(asset_id);
        if (asset == nullptr) {
            return false;
        }

        asset->sourcing_mode = sourcing_mode;
        asset->distribution_policy = distribution_policy;
        asset->task_release_ring = task_release_ring;

        for (auto& need : needs_) {
            if (need.governed_asset_id == asset_id) {
                need.active_sourcing_mode = sourcing_mode;
                need.last_status_note =
                    (sourcing_mode == SourcingMode::L7_LEGACY_CHANNEL)
                        ? ("Governed via L7 fallback (" + need.l7_channel_label + ").")
                        : ("Ratified L5 policy (" + std::string(ToString(sourcing_mode)) +
                           " / " + std::string(ToString(distribution_policy)) + ").");
            }
        }

        if (sourcing_mode != SourcingMode::L7_LEGACY_CHANNEL) {
            EnsureWorkflowsSpawnedForAsset(asset_id);
            if (ledger_.dependency_ratio_bp > 250U) {
                ledger_.dependency_ratio_bp -= 250U;
            }
            ledger_.steward_reputation_bp =
                std::min<uint32_t>(10000U, ledger_.steward_reputation_bp + asset->steward_reputation_credit_bp);
        }

        EvaluateBpmnTokensAndDependencies();

        // Propagate task_release_ring policy strictly to ACTIVE_TOKEN unassigned tasks when R1 swarm is enabled
        for (auto& wf : workflows_) {
            if (wf.asset_id == asset_id) {
                for (auto& task : wf.tasks) {
                    if (IsExternalR1SwarmEnabled() &&
                        task_release_ring == TaskReleaseRing::R1_PEER_CLUSTERS &&
                        task.bpmn_token_state == BpmnTokenState::ACTIVE_TOKEN &&
                        task.task_commitment_state == TaskCommitmentState::UNASSIGNED) {
                        task.task_commitment_state = TaskCommitmentState::RELEASED_TO_R1_PEERS;
                        task.last_state_change_tick = current_tick_;
                    } else if ((!IsExternalR1SwarmEnabled() ||
                                task_release_ring == TaskReleaseRing::R0_STEWARD_ONLY) &&
                               task.task_commitment_state == TaskCommitmentState::RELEASED_TO_R1_PEERS) {
                        task.task_commitment_state = TaskCommitmentState::UNASSIGNED;
                        task.last_state_change_tick = current_tick_;
                    }
                }
            }
        }

        RefreshL4AutomationModalities();
        RefreshClusterNodeSummaries();
        return true;
    }

    void EnsureSpecificWorkflowSpawned(const std::string& wf_id) {
        if (wf_id.empty() || HasWorkflow(wf_id)) {
            return;
        }

        auto make_task = [&](const std::string& workflow_id,
                             const std::string& asset_id,
                             const std::string& assigned_node_id,
                             const std::string& mapped_affordance_id,
                             uint32_t step_idx,
                             const std::string& task_id,
                             const std::string& title,
                             const std::string& loc,
                             uint64_t exergy_uj,
                             uint32_t action_code,
                             HaulModality haul,
                             const char* l2_sensor = "",
                             const char* l2_actuator = "",
                             uint32_t min_mk = 0U,
                             uint32_t max_moisture_bp = 0U,
                             const char* micro_prompt = "",
                             int64_t l7_offset_cents = 0) {
            L4TaskItem t{};
            t.task_id = task_id;
            t.workflow_id = workflow_id;
            t.asset_id = asset_id;
            t.assigned_node_id = assigned_node_id;
            t.mapped_affordance_id = mapped_affordance_id;
            t.title = title;
            t.location_label = loc;
            t.l2_sensor_register_id = l2_sensor;
            t.l2_actuator_register_id = l2_actuator;
            t.exergy_cost_uj = exergy_uj;
            t.l7_revenue_offset_cents = l7_offset_cents;
            t.last_state_change_tick = current_tick_;
            t.step_index = step_idx;
            t.oasis_action_code = action_code;
            t.target_temp_min_mk = min_mk;
            t.target_moisture_max_bp = max_moisture_bp;
            t.task_commitment_state = TaskCommitmentState::UNASSIGNED;
            t.bpmn_token_state = BpmnTokenState::WAITING_SEQUENCE_FLOW;
            t.haul_modality = haul;
            if (micro_prompt != nullptr && micro_prompt[0] != '\0') {
                t.has_unmodeled_micro_detail = 1U;
                t.unmodeled_prompt_question = micro_prompt;
            }
            return t;
        };

        L4WorkflowInstance wf{};
        wf.workflow_id = wf_id;
        wf.spawned_tick = current_tick_;
        wf.is_active = 1U;

        if (wf_id == "wf:fab_raised_bed") {
            wf.asset_id = "asset:garage_fab";
            wf.assigned_node_id = "node-02:garage-fab";
            wf.title = "Garage Fab: Mill Raised Cedar Garden Bed Frame";
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.mill_raised_bed_frame",
                1U, "task:fab_1_mill_raised_bed",
                "1. Mill Raised Cedar Bed Frame [Wheelbarrow/Dolly Haul to Backyard]",
                "Node 02: Garage Joinery Bench (9,1,13)", 800000000ULL, 25U,
                HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL));
        } else if (wf_id == "wf:fab_compost_kiln") {
            wf.asset_id = "asset:garage_fab";
            wf.assigned_node_id = "node-02:garage-fab";
            wf.title = "Garage Fab: Assemble Thermophilic Compost Drum & TLUD Kiln";
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.assemble_compost_and_kiln",
                1U, "task:fab_2_assemble_compost_kiln",
                "1. Assemble Compost Drum & TLUD Kiln [Wheelbarrow/Dolly Heavy Haul]",
                "Node 02 -> Backyard Kiln Pad (23,1,8)", 1100000000ULL, 24U,
                HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL));
        } else if (wf_id == "wf:fab_poultry_coop") {
            wf.asset_id = "asset:garage_fab";
            wf.assigned_node_id = "node-02:garage-fab";
            wf.title = "Garage Fab: Build Elevated Timber Poultry Coop & BSFL Feeder";
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.build_poultry_coop",
                1U, "task:fab_3_build_poultry_coop",
                "1. Build Elevated Poultry Coop [Wheelbarrow/Dolly Heavy Haul]",
                "Node 02 -> Backyard Coop Run (22,1,11)", 1600000000ULL, 19U,
                HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL));
        } else if (wf_id == "wf:fab_l7_woodwork") {
            wf.asset_id = "asset:garage_fab";
            wf.assigned_node_id = "node-02:garage-fab";
            wf.title = "Garage Fab: L7 Custom Woodwork Commission (+$180 Zero-Bill Offset)";
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.fulfill_l7_woodwork",
                1U, "task:fab_4_l7_custom_woodwork_order",
                "1. Fulfill L7 Custom Woodwork Order [+$180 Bill Offset]",
                "Node 02: Garage Joinery Bench (9,1,13)", 1500000000ULL, 29U,
                HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL,
                "", "", 0U, 0U,
                "Confirm mortise-and-tenon joinery tolerances inspected and natural linseed finish cured before L7 client pickup?",
                18000));
        } else if (wf_id == "wf:pea_cultivation") {
            wf.asset_id = "asset:pea_cultivation";
            wf.assigned_node_id = "node-03:garden-agro";
            wf.title = "Legume Cultivation & Keyline Bed BPMN Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.plow_and_amend_bed",
                1U, "task:pea_1_plow_amend",
                "1. Plow & Amend Keyline Soil [Granular Haul]",
                "Node 03: Backyard Keyline Bed (20,1,5)", 800000000ULL, 24U,
                HaulModality::GRANULAR_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.erect_pea_trellis",
                2U, "task:pea_2_erect_trellis",
                "2. Erect A-Frame Wire Pea Trellis",
                "Node 03: Backyard Keyline Bed (20,1,5)", 600000000ULL, 25U,
                HaulModality::NONE));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.inoculate_and_sow_peas",
                3U, "task:pea_3_rhizobium_sow",
                "3. Rhizobium Inoculation & Sow Split Peas + Mirepoix",
                "Node 03: Backyard Keyline Bed (20,1,5)", 400000000ULL, 26U,
                HaulModality::NONE, "", "", 0U, 0U,
                "Confirm pea seeds were dampened and coated with Rhizobium leguminosarum inoculant out of direct UV before sowing 3-4 cm deep?"));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.water_garden_manual",
                4U, "task:pea_4_irrigate_weed",
                "4. Timed Verification & Irrigate [Bucket Liquid Haul OR L2 Probe/Valve]",
                "Node 03: Cistern -> Keyline Bed (20,1,5)", 350000000ULL, 27U,
                HaulModality::BUCKET_LIQUID_HAUL, "soil_probe_v1", "irrigation_valve_v1", 0U, 4500U));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.harvest_peas_and_roots",
                5U, "task:pea_5_harvest_pods_roots",
                "5. Harvest Mature Pea Pods & Mirepoix Roots",
                "Node 03: Backyard Keyline Bed (20,1,5)", 500000000ULL, 13U,
                HaulModality::GRANULAR_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.shell_dry_and_turn_bed",
                6U, "task:pea_6_shell_moisture_turn",
                "6. Shell Dry Legumes (<14% Moisture) & Turn Biomass Vines",
                "Node 03: Larder Porch & Keyline Bed (20,1,5)", 450000000ULL, 28U,
                HaulModality::GRANULAR_HAUL, "soil_probe_v1", "", 0U, 1400U,
                "Confirm shelled split legumes tested <1400 bp (<14%) moisture before jar sealing and residue vines turned into soil?"));
            ApplyCultivarParametersToWorkflow(
                wf, ResolveCultivarSpecOrDefault(selected_garden_cultivar_id_));
        } else if (wf_id == "wf:pea_soup_hearth") {
            wf.asset_id = "asset:pea_soup_hearth";
            wf.assigned_node_id = "node-04:hearth-galley";
            wf.title = "Pea Soup & 24h Collagen Bone Broth BPMN Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.gather_soup_inputs",
                1U, "task:soup_1_gather_inputs",
                "1. Gather & Rinse Split Peas, Mirepoix & Broth Bones",
                "Node 04: Kitchen Prep Sink (12,1,3)", 300000000ULL, 14U,
                HaulModality::GRANULAR_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.simmer_bone_broth",
                2U, "task:soup_2_simmer_bone_broth",
                "2. Simmer 24h Collagen Bone Broth (>=347150 mK)",
                "Node 04: Kitchen Induction Hearth (11,1,3)", 1200000000ULL, 21U,
                HaulModality::BUCKET_LIQUID_HAUL, "culinary_vessel_v1", "", 347150U, 0U));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.cook_pea_soup",
                3U, "task:soup_3_simmer_pea_soup_74c",
                "3. Simmer Split-Pea Soup >=74C (Bigelow Pasteurization)",
                "Node 04: Kitchen Induction Hearth (11,1,3)", 1400000000ULL, 22U,
                HaulModality::NONE, "culinary_vessel_v1", "", 347150U, 0U,
                "Confirm core soup temperature held >= 74C (347150 mK) for >= 120s Bigelow lethality with bay leaf and iodized sea salt?"));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.pack_and_deliver_tiffins",
                4U, "task:soup_4_package_distribute_tiffins",
                "4. Package & Serve 304-Stainless Pea Soup Tiffins",
                "Node 04: Hearth -> Commons Tiffin Rack", 500000000ULL, 23U,
                HaulModality::TIFFIN_CARRIER_HAUL));
        } else if (wf_id == "wf:poultry_and_soil") {
            wf.asset_id = "asset:poultry_and_soil";
            wf.assigned_node_id = "node-03:garden-agro";
            wf.title = "Thermophilic Compost, TLUD Biochar & Pasture Poultry BPMN Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.turn_and_harvest_compost",
                1U, "task:mu_2_thermophilic_compost_55c",
                "1. Turn Thermophilic Compost Drum >=55C (328150 mK)",
                "Node 03: Backyard Compost Bay (23,1,9)", 500000000ULL, 24U,
                HaulModality::GRANULAR_HAUL, "compost_drum_probe_v1", "", 328150U, 0U));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.pyrolyze_and_quench_biochar",
                2U, "task:mu_3_tlud_biochar_charge",
                "2. Run TLUD Pyrolysis Biochar Kiln & Charge Terra Preta",
                "Node 03: Backyard Kiln Pad (23,1,8)", 900000000ULL, 24U,
                HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL, "kiln_thermocouple_v1", "", 773150U, 0U));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.tend_hens_and_collect_eggs",
                3U, "task:mu_1_feed_bsfl_collect_eggs",
                "3. Feed BSFL Larvae & Collect Pasture Eggs [Trophic Closure]",
                "Node 03: Backyard Poultry Coop (22,1,11)", 400000000ULL, 20U,
                HaulModality::GRANULAR_HAUL));
        } else if (wf_id == "wf:water_and_power") {
            wf.asset_id = "asset:water_and_power";
            wf.assigned_node_id = "node-01:office-coordinator";
            wf.title = "Rainwater Cistern & 48V Solar Microgrid BPMN Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.draw_potable_water",
                1U, "task:infra_1_cistern_first_flush",
                "1. Inspect Cistern First-Flush Diverter & Draw Potable Water",
                "Node 01: Cistern Pad (18,1,3)", 450000000ULL, 27U,
                HaulModality::BUCKET_LIQUID_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, wf.asset_id, wf.assigned_node_id, "aff.study_at_workstation",
                2U, "task:infra_2_solar_bus_balance",
                "2. Audit 400W PV Array & Verify 48V LiFePO4 Cell Balance",
                "Node 01: Office Workstation (13,1,3)", 750000000ULL, 18U,
                HaulModality::NONE, "solar_bus_meter_v1", "", 0U, 0U));
        } else {
            return;
        }

        if (L5GovernedAsset* asset = FindAssetMutable(wf.asset_id)) {
            asset->local_workflows_instantiated = 1U;
            bool already_listed = false;
            for (const auto& existing_id : asset->workflow_ids) {
                if (existing_id == wf.workflow_id) {
                    already_listed = true;
                    break;
                }
            }
            if (!already_listed) {
                asset->workflow_ids.push_back(wf.workflow_id);
            }
        }
        workflows_.push_back(wf);
        RefreshL4AutomationModalities();
    }

    void EnsureWorkflowsSpawnedForAsset(const std::string& asset_id) {
        L5GovernedAsset* asset = FindAssetMutable(asset_id);
        if (asset == nullptr) {
            return;
        }

        if (asset_id == "asset:pea_cultivation") {
            EnsureSpecificWorkflowSpawned("wf:pea_cultivation");
            return;
        }
        if (asset_id == "asset:pea_soup_hearth") {
            EnsureSpecificWorkflowSpawned("wf:pea_soup_hearth");
            return;
        }
        if (asset_id == "asset:poultry_and_soil") {
            EnsureSpecificWorkflowSpawned("wf:poultry_and_soil");
            return;
        }
        if (asset_id == "asset:garage_fab") {
            EnsureSpecificWorkflowSpawned("wf:fab_l7_woodwork");
            return;
        }
        if (asset_id == "asset:water_and_power") {
            EnsureSpecificWorkflowSpawned("wf:water_and_power");
            return;
        }

        const std::string slug = MakeSlug(asset_id);
        const std::string custom_wf_id = "wf:" + slug;
        if (HasWorkflow(custom_wf_id)) {
            return;
        }

        L4WorkflowInstance wf{};
        wf.workflow_id = custom_wf_id;
        wf.asset_id = asset_id;
        wf.assigned_node_id = "node-01:office-coordinator";
        wf.title = asset->title + " Local Stewardship BPMN Lifecycle";
        wf.spawned_tick = current_tick_;
        wf.is_active = 1U;

        auto make_custom_step = [&](uint32_t step_idx,
                                    const std::string& suffix,
                                    const std::string& title,
                                    const char* affordance_id,
                                    uint64_t exergy_uj,
                                    uint32_t action_code,
                                    HaulModality haul,
                                    const char* prompt) {
            L4TaskItem t{};
            t.task_id = "task:" + slug + "_" + suffix;
            t.workflow_id = custom_wf_id;
            t.asset_id = asset_id;
            t.assigned_node_id = "node-01:office-coordinator";
            t.mapped_affordance_id = affordance_id;
            t.title = title;
            t.location_label = "Node 01: Homestead Commons Workspace";
            t.exergy_cost_uj = exergy_uj;
            t.last_state_change_tick = current_tick_;
            t.step_index = step_idx;
            t.oasis_action_code = action_code;
            t.task_commitment_state = TaskCommitmentState::UNASSIGNED;
            t.bpmn_token_state = BpmnTokenState::WAITING_SEQUENCE_FLOW;
            t.haul_modality = haul;
            if (prompt != nullptr && prompt[0] != '\0') {
                t.has_unmodeled_micro_detail = 1U;
                t.unmodeled_prompt_question = prompt;
            }
            return t;
        };

        wf.tasks.push_back(make_custom_step(
            1U, "1_stage",
            "1. Stage Local Materials & Tooling [Granular Haul]",
            "aff.study_at_workstation",
            asset->maintenance_exergy_uj / 3ULL + 100000000ULL, 18U,
            HaulModality::GRANULAR_HAUL, ""));
        wf.tasks.push_back(make_custom_step(
            2U, "2_execute",
            "2. Execute Local Commons Production [Unmodeled micro-detail included]",
            "aff.study_at_workstation",
            asset->maintenance_exergy_uj / 2ULL + 200000000ULL, 18U,
            HaulModality::NONE,
            "Confirm physical quality check and local safety envelope verified by steward?"));
        wf.tasks.push_back(make_custom_step(
            3U, "3_distribute",
            "3. Package & Share Output Across Governed Ring",
            "aff.pack_and_deliver_tiffins",
            asset->maintenance_exergy_uj / 4ULL + 100000000ULL, 23U,
            HaulModality::TIFFIN_CARRIER_HAUL, ""));

        asset->local_workflows_instantiated = 1U;
        asset->workflow_ids.push_back(wf.workflow_id);
        workflows_.push_back(wf);
        RefreshL4AutomationModalities();
    }

    // ------------------------------------------------------------------------
    // Fabaceae Cultivar Catalog & Reusable Workflow Parameterization
    // ------------------------------------------------------------------------

    void InitializeCanonicalCultivars() {
        cultivars_.clear();

        {
            CultivarWorkflowSpec c{};
            c.seed_exergy_uj = 400000000ULL;
            c.cultivar_id = "cultivar:chrono_virtual_pea";
            c.title = "Chrono-Pod Virtual Pea (Sim-Tuned Fast Cycle)";
            c.species_binomial = "Pisum virtualis";
            c.agronomic_notes =
                "Game-tuned virtual cultivar: 12h seed-to-pod cycle with 3h timed verification checks and 1.4x Rhizobium N2 fixation.";
            c.maturity_sim_seconds = 43200U;        // 12 sim hours
            c.verification_interval_sim_s = 10800U; // 3 sim hours
            c.min_growth_temp_mk = 277150U;
            c.opt_low_temp_mk = 285150U;
            c.opt_high_temp_mk = 302150U;
            c.max_growth_temp_mk = 313150U;
            c.target_min_vwc_bp = 3000U;
            c.target_opt_vwc_bp = 4200U;
            c.target_max_vwc_bp = 4900U;
            c.irrigation_dose_ml = 4500U;
            c.et_multiplier_bp = 15000U;
            c.n_fixation_rate_bp = 14000U;
            c.harvest_mature_bp = 9500U;
            c.trellis_ridge_height_mm = 1500U;
            c.sowing_depth_mm = 30U;
            c.n_draw_per_band_ppm = 0U;
            c.p_draw_per_band_ppm = 1U;
            c.k_draw_per_band_ppm = 1U;
            c.fixes_atmospheric_n2 = 1U;
            c.is_virtual_sim_tuned = 1U;
            c.harvest_legume_batches = 2U;
            c.harvest_mirepoix_batches = 1U;
            cultivars_.push_back(c);
        }
        {
            CultivarWorkflowSpec c{};
            c.seed_exergy_uj = 380000000ULL;
            c.cultivar_id = "cultivar:little_marvel_pea";
            c.title = "Little Marvel Bush Pea";
            c.species_binomial = "Pisum sativum var. humile";
            c.agronomic_notes =
                "Compact 900 mm cool-season dwarf pea: 48h (2-day) cycle with 6h soil verification checks and steady Rhizobium N2 fixation.";
            c.maturity_sim_seconds = 172800U;       // 48 sim hours (2 days)
            c.verification_interval_sim_s = 21600U; // 6 sim hours
            c.min_growth_temp_mk = 278150U;
            c.opt_low_temp_mk = 286150U;
            c.opt_high_temp_mk = 297150U;
            c.max_growth_temp_mk = 306150U;
            c.target_min_vwc_bp = 3000U;
            c.target_opt_vwc_bp = 4100U;
            c.target_max_vwc_bp = 4700U;
            c.irrigation_dose_ml = 4000U;
            c.et_multiplier_bp = 9500U;
            c.n_fixation_rate_bp = 10000U;
            c.harvest_mature_bp = 9500U;
            c.trellis_ridge_height_mm = 900U;
            c.sowing_depth_mm = 35U;
            c.n_draw_per_band_ppm = 0U;
            c.p_draw_per_band_ppm = 1U;
            c.k_draw_per_band_ppm = 1U;
            c.fixes_atmospheric_n2 = 1U;
            c.is_virtual_sim_tuned = 0U;
            c.harvest_legume_batches = 2U;
            c.harvest_mirepoix_batches = 1U;
            cultivars_.push_back(c);
        }
        {
            CultivarWorkflowSpec c{};
            c.seed_exergy_uj = 460000000ULL;
            c.cultivar_id = "cultivar:alderman_telephone_pea";
            c.title = "Alderman Heritage Climbing Pea";
            c.species_binomial = "Pisum sativum";
            c.agronomic_notes =
                "Tall 1.9 m heirloom shelling pea: 7-day cycle with 12h verification checks, high N2 fixation, and +3 split-pea batches.";
            c.maturity_sim_seconds = 604800U;       // 7 sim days
            c.verification_interval_sim_s = 43200U; // 12 sim hours
            c.min_growth_temp_mk = 278150U;
            c.opt_low_temp_mk = 286150U;
            c.opt_high_temp_mk = 296150U;
            c.max_growth_temp_mk = 305150U;
            c.target_min_vwc_bp = 3200U;
            c.target_opt_vwc_bp = 4300U;
            c.target_max_vwc_bp = 4900U;
            c.irrigation_dose_ml = 5000U;
            c.et_multiplier_bp = 10500U;
            c.n_fixation_rate_bp = 12000U;
            c.harvest_mature_bp = 9500U;
            c.trellis_ridge_height_mm = 1900U;
            c.sowing_depth_mm = 40U;
            c.n_draw_per_band_ppm = 0U;
            c.p_draw_per_band_ppm = 1U;
            c.k_draw_per_band_ppm = 2U;
            c.fixes_atmospheric_n2 = 1U;
            c.is_virtual_sim_tuned = 0U;
            c.harvest_legume_batches = 3U;
            c.harvest_mirepoix_batches = 2U;
            cultivars_.push_back(c);
        }
        {
            CultivarWorkflowSpec c{};
            c.seed_exergy_uj = 520000000ULL;
            c.cultivar_id = "cultivar:kentucky_wonder_bean";
            c.title = "Kentucky Wonder Heritage Pole Bean";
            c.species_binomial = "Phaseolus vulgaris";
            c.agronomic_notes =
                "Warm-season Fabaceae pole bean (requires >=12C / 285150 mK soil): 10-day cycle, 12h checks, draws soil mineral N-P-K, yields +4 legume batches.";
            c.maturity_sim_seconds = 864000U;       // 10 sim days
            c.verification_interval_sim_s = 43200U; // 12 sim hours
            c.min_growth_temp_mk = 285150U;         // 12.0 C warm-soil minimum!
            c.opt_low_temp_mk = 291150U;
            c.opt_high_temp_mk = 303150U;
            c.max_growth_temp_mk = 311150U;
            c.target_min_vwc_bp = 3300U;
            c.target_opt_vwc_bp = 4400U;
            c.target_max_vwc_bp = 5000U;
            c.irrigation_dose_ml = 5500U;
            c.et_multiplier_bp = 11500U;
            c.n_fixation_rate_bp = 0U;
            c.harvest_mature_bp = 9500U;
            c.trellis_ridge_height_mm = 2100U;
            c.sowing_depth_mm = 25U;
            c.n_draw_per_band_ppm = 1U;
            c.p_draw_per_band_ppm = 2U;
            c.k_draw_per_band_ppm = 3U;
            c.fixes_atmospheric_n2 = 0U;
            c.is_virtual_sim_tuned = 0U;
            c.harvest_legume_batches = 4U;
            c.harvest_mirepoix_batches = 2U;
            cultivars_.push_back(c);
        }
    }

    void TryEnrichCultivarsFromJsonLd() {
        const std::vector<std::string> candidate_paths = {
            "core/6_intent/cultivars/fabaceae_cultivars.jsonld",
            "../core/6_intent/cultivars/fabaceae_cultivars.jsonld",
            "../../core/6_intent/cultivars/fabaceae_cultivars.jsonld",
            "../../../core/6_intent/cultivars/fabaceae_cultivars.jsonld",
            "../../../../core/6_intent/cultivars/fabaceae_cultivars.jsonld"
        };

        for (const auto& path : candidate_paths) {
            std::ifstream in(path, std::ios::in | std::ios::binary);
            if (!in.is_open()) {
                continue;
            }
            std::ostringstream buf;
            buf << in.rdbuf();
            const std::string content = buf.str();
            if (content.empty()) {
                continue;
            }

            uint32_t enriched = 0U;
            for (auto& c : cultivars_) {
                const std::string lookup_key = "\"@id\": \"" + c.cultivar_id + "\"";
                const size_t hit = content.find(lookup_key);
                if (hit == std::string::npos) {
                    continue;
                }
                const size_t next_id = content.find("\"@id\":", hit + 6U);
                const size_t window_end = (next_id != std::string::npos)
                                              ? std::min(next_id, hit + 3200U)
                                              : std::min(content.size(), hit + 3200U);
                std::string_view slice(content.data() + hit, window_end - hit);
                const uint64_t mat_s = json_util::ExtractUint64(slice, "maturity_sim_seconds", 0ULL);
                if (mat_s > 0ULL) {
                    c.maturity_sim_seconds = static_cast<uint32_t>(mat_s);
                }
                const uint64_t chk_s = json_util::ExtractUint64(slice, "verification_interval_sim_s", 0ULL);
                if (chk_s > 0ULL) {
                    c.verification_interval_sim_s = static_cast<uint32_t>(chk_s);
                }
                const uint64_t min_mk = json_util::ExtractUint64(slice, "min_growth_temp_mk", 0ULL);
                if (min_mk > 0ULL) {
                    c.min_growth_temp_mk = static_cast<uint32_t>(min_mk);
                }
                const uint64_t batches = json_util::ExtractUint64(slice, "harvest_legume_batches", 0ULL);
                if (batches > 0ULL) {
                    c.harvest_legume_batches = static_cast<uint8_t>(batches);
                }
                const std::string desc = json_util::ExtractString(slice, "schema:description");
                if (!desc.empty()) {
                    c.agronomic_notes = desc;
                }
                ++enriched;
            }
            cultivars_enriched_count_ = enriched;
            break;
        }
    }

    static void ApplyCultivarParametersToWorkflow(
        L4WorkflowInstance& wf,
        const CultivarWorkflowSpec& spec
    ) {
        wf.input_cultivar_id = spec.cultivar_id;
        wf.input_cultivar_title = spec.title;
        wf.species_binomial = spec.species_binomial;
        wf.harvest_mature_bp = spec.harvest_mature_bp;
        wf.maturity_sim_seconds = spec.maturity_sim_seconds;
        wf.verification_interval_sim_s = spec.verification_interval_sim_s;
        wf.is_virtual_cultivar = spec.is_virtual_sim_tuned;
        wf.fixes_atmospheric_n2 = spec.fixes_atmospheric_n2;
        wf.phenology_stage_label =
            ComputePhenologyLabel(wf.crop_growth_bp, wf.crop_sown, spec.harvest_mature_bp);
        wf.title = "Legume Cultivation BPMN — " + spec.title + " (" +
                   FormatSimDurationCompact(spec.maturity_sim_seconds) + " cycle)";

        for (auto& t : wf.tasks) {
            if (t.task_id == "task:pea_2_erect_trellis") {
                t.title = "2. Erect A-Frame Wire Trellis (" +
                          std::to_string(spec.trellis_ridge_height_mm) + " mm Ridge)";
            } else if (t.task_id == "task:pea_3_rhizobium_sow") {
                t.exergy_cost_uj = spec.seed_exergy_uj;
                t.target_temp_min_mk = spec.min_growth_temp_mk;
                if (spec.fixes_atmospheric_n2 != 0U) {
                    t.title = "3. Rhizobium Inoculation & Sow " + spec.title + " (" +
                              std::to_string(spec.sowing_depth_mm) + " mm)";
                    t.unmodeled_prompt_question =
                        "Confirm " + spec.species_binomial +
                        " seeds were dampened and coated with Rhizobium leguminosarum inoculant out of direct UV before sowing " +
                        std::to_string(spec.sowing_depth_mm) + " mm deep?";
                } else {
                    t.title = "3. Warm-Soil Check (>=" +
                              std::to_string((spec.min_growth_temp_mk - 273150U) / 1000U) +
                              "C) & Sow " + spec.title + " (" +
                              std::to_string(spec.sowing_depth_mm) + " mm)";
                    t.unmodeled_prompt_question =
                        "Confirm soil bed exceeded " + std::to_string(spec.min_growth_temp_mk) +
                        " mK warm-season threshold and received compost mineral-N side-dressing for non-nodulating " +
                        spec.species_binomial + "?";
                }
            } else if (t.task_id == "task:pea_4_irrigate_weed") {
                t.timer_interval_sim_s = spec.verification_interval_sim_s;
                t.target_temp_min_mk = spec.min_growth_temp_mk;
                t.target_moisture_max_bp = spec.target_opt_vwc_bp;
                t.title = "4. Timed Verification & Irrigate [Every " +
                          FormatSimDurationCompact(spec.verification_interval_sim_s) +
                          " · Bucket OR L2]";
            } else if (t.task_id == "task:pea_5_harvest_pods_roots") {
                t.title = "5. Harvest Mature " + spec.title + " (+" +
                          std::to_string(spec.harvest_legume_batches) + " Legume / +" +
                          std::to_string(spec.harvest_mirepoix_batches) + " Root Batches)";
            }
        }
    }

    // ------------------------------------------------------------------------
    // Strictly 8-Byte Aligned Member Layout (-Wpadded Clean)
    // ------------------------------------------------------------------------
    std::vector<L6NeedRecord>         needs_{};
    std::vector<L6IntentRequest>      requests_{};
    std::vector<L6PendingDecision>    pending_decisions_{};
    std::vector<L5GovernedAsset>      governed_assets_{};
    std::vector<L4WorkflowInstance>   workflows_{};
    std::vector<L6NeedRecord>         last_search_results_{};
    std::vector<HomesteadClusterNode> cluster_nodes_{};
    std::vector<CultivarWorkflowSpec> cultivars_{};
    L2TelemetryTable                  l2_telemetry_{};
    L3NodeLedger                      ledger_{};
    std::string                       bootstrap_source_path_{};
    std::string                       selected_garden_cultivar_id_{"cultivar:chrono_virtual_pea"};
    std::string                       last_command_action_{};
    std::string                       last_command_status_{};
    uint64_t                          current_tick_{1ULL};
    uint64_t                          custom_counter_{0ULL};
    uint64_t                          sim_clock_seconds_{28800ULL};
    uint32_t                          sim_day_{1U};
    uint32_t                          ambient_temp_override_mk_{0U};
    uint32_t                          cultivars_enriched_count_{0U};
    uint32_t                          raised_bed_frames_built_{0U};
    uint32_t                          compost_kiln_units_built_{0U};
    uint32_t                          poultry_coops_built_{0U};
    uint32_t                          harvested_split_pea_batches_{0U};
    uint32_t                          harvested_mirepoix_batches_{0U};
    uint32_t                          poultry_bones_and_eggs_batches_{0U};
    uint32_t                          thermophilic_compost_batches_{0U};
    uint32_t                          biochar_batches_{0U};
    uint32_t                          simmered_bone_broth_batches_{0U};
    uint32_t                          served_pea_soup_tiffin_batches_{0U};
    uint8_t                           bootstrapped_from_jsonld_{0U};
    uint8_t                           external_r1_swarm_enabled_{0U};
    [[maybe_unused]] uint8_t          pad_[2]{0U};
};
static_assert(alignof(SharedStackRuntime) == 8, "SharedStackRuntime must be 8-byte aligned.");

} // namespace oasis::stack
