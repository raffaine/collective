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
    uint32_t    concentration_ppm{0};     // Parts per million
    uint32_t    actuator_level_bp{0};     // Basis points (0..10000)
    uint32_t    hold_duration_ms{0};      // Milliseconds at target lethality/hold
    uint8_t     is_online{0};             // 0 = absent/offline, 1 = active in L2TelemetryTable
    uint8_t     is_actuator{0};           // 0 = sensor, 1 = actuator
    uint8_t     actuator_commanded_on{0}; // 1 = open/energized by L4 controller
    uint8_t     quality_flags{1};         // 1 = QUALITY_SIMULATED, 2 = QUALITY_HARDWARE_ROOT
};
static_assert(alignof(L2SensorRegister) == 8, "L2SensorRegister must be 8-byte aligned.");

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
// Layer 4: Real-Life Multi-Workflow Orchestrator Models
// ============================================================================

struct alignas(8) L4TaskItem {
    std::string           task_id{};
    std::string           workflow_id{};
    std::string           asset_id{};
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
    uint32_t              step_index{1};
    uint32_t              oasis_action_code{0};
    uint32_t              target_temp_min_mk{0};
    uint32_t              target_moisture_max_bp{0};
    TaskCommitmentState   task_commitment_state{TaskCommitmentState::UNASSIGNED};
    TaskExecutionModality execution_modality{TaskExecutionModality::MANUAL_HUMAN_OBSERVATION};
    HaulModality          haul_modality{HaulModality::NONE};
    uint8_t               has_unmodeled_micro_detail{0};
    uint8_t               unmodeled_prompt_resolved{0};
    [[maybe_unused]] uint8_t pad_[3]{0};
};
static_assert(alignof(L4TaskItem) == 8, "L4TaskItem must be 8-byte aligned.");

struct alignas(8) L4WorkflowInstance {
    std::string             workflow_id{};
    std::string             asset_id{};
    std::string             title{};
    std::vector<L4TaskItem> tasks{};
    uint64_t                spawned_tick{0};
    uint32_t                completed_cycles{0};
    uint8_t                 is_active{1};
    [[maybe_unused]] uint8_t pad_[3]{0};
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

        l2_telemetry_ = L2TelemetryTable{};
        ledger_ = L3NodeLedger{};
        current_tick_ = 1ULL;
        custom_counter_ = 0ULL;
        sim_day_ = 1U;
        bootstrapped_from_jsonld_ = 0U;
        bootstrap_source_path_ = "builtin:pea_to_soup_canonical_v1";
        last_command_action_ = "bootstrap";
        last_command_status_ = "OK";

        InitializePeerBccChannels();
        InitializeGovernedAssets();
        InitializeCanonicalNeeds();
        TryEnrichNeedsFromJsonLd(jsonld_hint_path);
        InitializeGenesisDecisionsAndWorkflows();
        RefreshL4AutomationModalities();
    }

    void AdvanceToTick(uint64_t tick) noexcept {
        if (tick > current_tick_) {
            current_tick_ = tick;
            sim_day_ = 1U + static_cast<uint32_t>(current_tick_ / 28800ULL);
        }
    }

    [[nodiscard]] uint64_t CurrentTick() const noexcept { return current_tick_; }
    [[nodiscard]] uint32_t CurrentSimDay() const noexcept { return sim_day_; }
    [[nodiscard]] bool BootstrappedFromJsonLd() const noexcept { return bootstrapped_from_jsonld_ != 0U; }
    [[nodiscard]] const std::string& BootstrapSourcePath() const noexcept { return bootstrap_source_path_; }

    // ------------------------------------------------------------------------
    // Direct State Accessors
    // ------------------------------------------------------------------------

    [[nodiscard]] const std::vector<L6NeedRecord>& GetNeeds() const noexcept { return needs_; }
    [[nodiscard]] const std::vector<L6IntentRequest>& GetIntentRequests() const noexcept { return requests_; }
    [[nodiscard]] const std::vector<L6PendingDecision>& GetPendingDecisions() const noexcept { return pending_decisions_; }
    [[nodiscard]] const std::vector<L5GovernedAsset>& GetGovernedAssets() const noexcept { return governed_assets_; }
    [[nodiscard]] const std::vector<L4WorkflowInstance>& GetWorkflows() const noexcept { return workflows_; }
    [[nodiscard]] std::vector<L4WorkflowInstance>& GetWorkflowsMutable() noexcept { return workflows_; }
    [[nodiscard]] const L3NodeLedger& GetLedger() const noexcept { return ledger_; }
    [[nodiscard]] L3NodeLedger& GetLedgerMutable() noexcept { return ledger_; }
    [[nodiscard]] const L2TelemetryTable& GetL2TelemetryTable() const noexcept { return l2_telemetry_; }
    [[nodiscard]] L2TelemetryTable& GetL2TelemetryTableMutable() noexcept { return l2_telemetry_; }

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
                // Local R0 workflows or R1 mesh peers
                ledger_.total_exergy_accounted_uj += need->exergy_cost_uj * static_cast<uint64_t>(qty);
                EnsureWorkflowsSpawnedForAsset(need->governed_asset_id);

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

        return ApplyGovernancePolicyToAsset(
            dec->governed_asset_id,
            sourcing_mode,
            distribution_policy,
            task_release_ring
        );
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
    // L4 Multi-Workflow Task Lifecycle Operations
    // ------------------------------------------------------------------------

    bool CommitTask(const std::string& task_id) {
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr) {
            return false;
        }
        task->task_commitment_state = TaskCommitmentState::COMMITTED_BY_PLAYER;
        task->claimed_by_cluster_id.clear();
        task->last_state_change_tick = current_tick_;
        return true;
    }

    bool ReleaseTaskToR1Peers(const std::string& task_id) {
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr) {
            return false;
        }
        task->task_commitment_state = TaskCommitmentState::RELEASED_TO_R1_PEERS;
        task->claimed_by_cluster_id.clear();
        task->last_state_change_tick = current_tick_;
        return true;
    }

    bool ClaimTaskByPeerCluster(const std::string& task_id, const std::string& cluster_id) {
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr || task->task_commitment_state != TaskCommitmentState::RELEASED_TO_R1_PEERS) {
            return false;
        }
        task->task_commitment_state = TaskCommitmentState::CLAIMED_BY_PEER_CLUSTER;
        task->claimed_by_cluster_id = cluster_id;
        task->last_state_change_tick = current_tick_;
        return true;
    }

    bool CompleteTask(const std::string& task_id, const std::string& completed_by_cluster_id = "") {
        L4TaskItem* task = FindTaskMutable(task_id);
        if (task == nullptr) {
            return false;
        }

        const bool already_completed = (task->task_commitment_state == TaskCommitmentState::COMPLETED);
        task->task_commitment_state = TaskCommitmentState::COMPLETED;
        if (!completed_by_cluster_id.empty()) {
            task->claimed_by_cluster_id = completed_by_cluster_id;
        }
        task->last_state_change_tick = current_tick_;

        if (!already_completed) {
            ledger_.total_exergy_accounted_uj += task->exergy_cost_uj;
            if (completed_by_cluster_id.empty()) {
                ledger_.steward_reputation_bp =
                    std::min<uint32_t>(10000U, ledger_.steward_reputation_bp + 120U);
            }
            if (task->l7_revenue_offset_cents > 0) {
                ledger_.monthly_l7_offset_revenue_cents += task->l7_revenue_offset_cents;
                ledger_.household_reserve_cents += task->l7_revenue_offset_cents;
                ++ledger_.completed_l7_offset_orders;
                if (ledger_.dependency_ratio_bp > 300U) {
                    ledger_.dependency_ratio_bp -= 300U;
                } else {
                    ledger_.dependency_ratio_bp = 0U;
                }
            }
        }

        // Check if all tasks in parent workflow are completed
        for (auto& wf : workflows_) {
            if (wf.workflow_id == task->workflow_id) {
                bool all_done = !wf.tasks.empty();
                for (const auto& step : wf.tasks) {
                    if (step.task_commitment_state != TaskCommitmentState::COMPLETED) {
                        all_done = false;
                        break;
                    }
                }
                if (all_done && !already_completed) {
                    ++wf.completed_cycles;
                }
                break;
            }
        }
        return true;
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
                // Target Living Terra Preta moisture >= 4000 bp (40.0%); open below 3800 bp
                if (soil->moisture_bp < 3800U) {
                    valve->actuator_commanded_on = 1U;
                    valve->actuator_level_bp = 10000U;
                } else if (soil->moisture_bp >= 4200U) {
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
        RefreshL4AutomationModalities();
    }

    std::string IngestL2PinsPayload(std::string_view payload) {
        ++current_tick_;

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
            reg.actuator_commanded_on = (reg.actuator_level_bp > 0U ||
                                         json_util::ExtractBool(payload, "actuator_commanded_on", false))
                                            ? 1U
                                            : 0U;
            l2_telemetry_.UpsertRegister(reg);
        }

        l2_telemetry_.last_update_tick = current_tick_;
        RefreshL4AutomationModalities();

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

        if (resolved_action == "commit_task") {
            std::string task_id = json_util::ExtractString(args, "task_id");
            if (task_id.empty()) {
                task_id = json_util::ExtractString(args, "id");
            }
            const bool ok = CommitTask(task_id);
            last_command_status_ = ok ? "OK" : "NOT_FOUND";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "release_task_r1") {
            std::string task_id = json_util::ExtractString(args, "task_id");
            if (task_id.empty()) {
                task_id = json_util::ExtractString(args, "id");
            }
            const bool ok = ReleaseTaskToR1Peers(task_id);
            last_command_status_ = ok ? "OK" : "NOT_FOUND";
            return SerializeFullNodeSnapshotJson();
        }

        if (resolved_action == "complete_task") {
            std::string task_id = json_util::ExtractString(args, "task_id");
            if (task_id.empty()) {
                task_id = json_util::ExtractString(args, "id");
            }
            const std::string cluster_id = json_util::ExtractString(args, "cluster_id");
            const bool ok = CompleteTask(task_id, cluster_id);
            last_command_status_ = ok ? "OK" : "NOT_FOUND";
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
            << "  \"schema_version\": \"oasis.stack.snapshot.v1\",\n"
            << "  \"node_did\": \"did:mesh:node01:homestead\",\n"
            << "  \"tick\": " << current_tick_ << ",\n"
            << "  \"sim_day\": " << sim_day_ << ",\n"
            << "  \"bootstrapped_from_jsonld\": " << (bootstrapped_from_jsonld_ != 0U ? "true" : "false") << ",\n"
            << "  \"bootstrap_source\": \"" << json_util::Escape(bootstrap_source_path_) << "\",\n"
            << "  \"last_command_action\": \"" << json_util::Escape(last_command_action_) << "\",\n"
            << "  \"last_command_status\": \"" << json_util::Escape(last_command_status_) << "\",\n";

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

        // 5. L4 Workflows & Tasks
        oss << "  \"l4_workflows\": [";
        std::vector<const L4TaskItem*> committed_tasks;
        for (size_t i = 0U; i < workflows_.size(); ++i) {
            const auto& wf = workflows_[i];
            oss << (i == 0U ? "\n" : ",\n")
                << "    {\n"
                << "      \"workflow_id\": \"" << json_util::Escape(wf.workflow_id) << "\",\n"
                << "      \"asset_id\": \"" << json_util::Escape(wf.asset_id) << "\",\n"
                << "      \"title\": \"" << json_util::Escape(wf.title) << "\",\n"
                << "      \"is_active\": " << (wf.is_active != 0U ? "true" : "false") << ",\n"
                << "      \"completed_cycles\": " << wf.completed_cycles << ",\n"
                << "      \"spawned_tick\": " << wf.spawned_tick << ",\n"
                << "      \"tasks\": [";
            for (size_t j = 0U; j < wf.tasks.size(); ++j) {
                const auto& t = wf.tasks[j];
                if (t.task_commitment_state == TaskCommitmentState::COMMITTED_BY_PLAYER) {
                    committed_tasks.push_back(&t);
                }
                oss << (j == 0U ? "\n" : ",\n");
                AppendTaskJson(oss, t, 8U);
            }
            oss << (wf.tasks.empty() ? "]\n" : "\n      ]\n")
                << "    }";
        }
        oss << (workflows_.empty() ? "],\n" : "\n  ],\n");

        // 6. Flat L4 Committed Player Tasks
        oss << "  \"l4_committed_tasks\": [";
        for (size_t i = 0U; i < committed_tasks.size(); ++i) {
            oss << (i == 0U ? "\n" : ",\n");
            AppendTaskJson(oss, *committed_tasks[i], 4U);
        }
        oss << (committed_tasks.empty() ? "],\n" : "\n  ],\n");

        // 7. L3 Exergy, Pairwise BCC & Peer Weather
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

    // ------------------------------------------------------------------------
    // Initialization & L4 Lifecycle Spawning
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
            poultry_soil.task_release_ring = TaskReleaseRing::R1_PEER_CLUSTERS;
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
                                     : "Active on local L5 stewarded infrastructure ($0 L7 spend).";
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
        // Garage Fab & Water/Power start locally active on Day 1
        EnsureWorkflowsSpawnedForAsset("asset:garage_fab");
        EnsureWorkflowsSpawnedForAsset("asset:water_and_power");

        // Emit pending L5 -> L6 polycentric decisions for L7-sourced assets
        {
            L6PendingDecision dec_peas{};
            dec_peas.decision_id = "dec:pea_cultivation";
            dec_peas.title = "L5 Proposal: Cultivate Split Peas & Mirepoix Locally in Backyard Keyline Bed";
            dec_peas.governed_asset_id = "asset:pea_cultivation";
            dec_peas.rationale =
                "Split Peas and Mirepoix Vegetables are currently procured via L7 supply chains. "
                "Approving spawns the 6-stage Pea Cultivation L4 workflow (soil amendment, trellis, Rhizobium sowing, irrigation, harvest, <14% moisture check).";
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
            dec_soup.title = "L5 Proposal: Transition Pea Soup & Bone Broth from L7 Delivery to Local Hearth & R1 Tiffins";
            dec_soup.governed_asset_id = "asset:pea_soup_hearth";
            dec_soup.rationale =
                "Eliminates $14.00/batch L7 delivery drain by instantiating the 4-stage Kitchen Pea Soup L4 workflow "
                "(input gathering, 24h collagen bone broth, >=74C Bigelow pasteurization simmer, stainless tiffin distribution).";
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
            dec_poultry.title = "L5 Proposal: Commission Pasture Poultry Coop, Thermophilic Compost & TLUD Biochar Loop";
            dec_poultry.governed_asset_id = "asset:poultry_and_soil";
            dec_poultry.rationale =
                "Closes Scenario Mu nitrogen/phosphorus/calcium cycle locally for Pasture Eggs, Thermophilic Compost, and Pyrolysis Biochar.";
            dec_poultry.linked_need_ids = {"need:pasture_eggs", "need:thermophilic_compost", "need:pyrolysis_biochar"};
            dec_poultry.estimated_exergy_savings_uj = 8700000000ULL;
            dec_poultry.estimated_monthly_l7_savings_cents = 15000;
            dec_poultry.emitted_tick = current_tick_;
            dec_poultry.proposed_sourcing_mode = SourcingMode::R0_LOCAL_WORKFLOWS;
            dec_poultry.proposed_distribution_policy = DistributionPolicy::R1_TRUST_RING_COMMONS;
            dec_poultry.proposed_task_release_ring = TaskReleaseRing::R1_PEER_CLUSTERS;
            dec_poultry.status = DecisionStatus::PENDING;
            pending_decisions_.push_back(dec_poultry);
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

        // Propagate task_release_ring policy to unassigned tasks of this asset
        for (auto& wf : workflows_) {
            if (wf.asset_id == asset_id) {
                for (auto& task : wf.tasks) {
                    if (task_release_ring == TaskReleaseRing::R1_PEER_CLUSTERS &&
                        task.task_commitment_state == TaskCommitmentState::UNASSIGNED) {
                        task.task_commitment_state = TaskCommitmentState::RELEASED_TO_R1_PEERS;
                        task.last_state_change_tick = current_tick_;
                    } else if (task_release_ring == TaskReleaseRing::R0_STEWARD_ONLY &&
                               task.task_commitment_state == TaskCommitmentState::RELEASED_TO_R1_PEERS) {
                        task.task_commitment_state = TaskCommitmentState::UNASSIGNED;
                        task.last_state_change_tick = current_tick_;
                    }
                }
            }
        }
        return true;
    }

    void EnsureWorkflowsSpawnedForAsset(const std::string& asset_id) {
        L5GovernedAsset* asset = FindAssetMutable(asset_id);
        if (asset == nullptr || asset->local_workflows_instantiated != 0U) {
            return;
        }

        auto make_task = [&](const std::string& wf_id,
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
            t.workflow_id = wf_id;
            t.asset_id = asset_id;
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
            t.task_commitment_state =
                (asset->task_release_ring == TaskReleaseRing::R1_PEER_CLUSTERS)
                    ? TaskCommitmentState::RELEASED_TO_R1_PEERS
                    : TaskCommitmentState::UNASSIGNED;
            t.haul_modality = haul;
            if (micro_prompt != nullptr && micro_prompt[0] != '\0') {
                t.has_unmodeled_micro_detail = 1U;
                t.unmodeled_prompt_question = micro_prompt;
            }
            return t;
        };

        L4WorkflowInstance wf{};
        wf.asset_id = asset_id;
        wf.spawned_tick = current_tick_;
        wf.is_active = 1U;

        if (asset_id == "asset:pea_cultivation") {
            wf.workflow_id = "wf:pea_cultivation";
            wf.title = "Pea Cultivation & Keyline Bed Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, 1U, "task:pea_1_plow_amend",
                "1. Plow & Amend Soil [Granular Haul]",
                "Backyard Keyline Bed (20,1,5)", 800000000ULL, 24U, HaulModality::GRANULAR_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 2U, "task:pea_2_erect_trellis",
                "2. Erect Trellis",
                "Backyard Keyline Bed (20,1,5)", 600000000ULL, 25U, HaulModality::NONE));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 3U, "task:pea_3_rhizobium_sow",
                "3. Rhizobium Inoculation & Sow Peas [Unmodeled micro-detail included]",
                "Backyard Keyline Bed (20,1,5)", 400000000ULL, 26U, HaulModality::NONE,
                "", "", 0U, 0U,
                "Confirm pea seeds were dampened and coated with Rhizobium leguminosarum inoculant out of direct UV before sowing 3-4 cm deep?"));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 4U, "task:pea_4_irrigate_weed",
                "4. Irrigate & Weed [Bucket Liquid Haul + Manual Soil Inspection OR L2 Automated Probe/Valve]",
                "Cistern -> Keyline Bed (20,1,5)", 350000000ULL, 27U, HaulModality::BUCKET_LIQUID_HAUL,
                "soil_probe_v1", "irrigation_valve_v1", 0U, 4500U));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 5U, "task:pea_5_harvest_pods_roots",
                "5. Harvest Pods & Roots",
                "Backyard Keyline Bed (20,1,5)", 500000000ULL, 13U, HaulModality::GRANULAR_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 6U, "task:pea_6_shell_moisture_turn",
                "6. Shell, Check <14% Moisture & Turn Bed",
                "Larder Porch & Keyline Bed (20,1,5)", 450000000ULL, 28U, HaulModality::GRANULAR_HAUL,
                "soil_probe_v1", "", 0U, 1400U,
                "Confirm shelled split peas tested <1400 bp (<14%) moisture before jar sealing and nitrogenous vines turned into soil?"));
        } else if (asset_id == "asset:pea_soup_hearth") {
            wf.workflow_id = "wf:pea_soup_hearth";
            wf.title = "Pea Soup & 24h Bone Broth Culinary Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, 1U, "task:soup_1_gather_inputs",
                "1. Gather/Procure Inputs",
                "Larder -> Kitchen Prep Table (10,1,4)", 300000000ULL, 14U, HaulModality::GRANULAR_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 2U, "task:soup_2_simmer_bone_broth",
                "2. Simmer Bone Broth",
                "Kitchen Induction Hearth (11,1,4)", 1200000000ULL, 21U, HaulModality::BUCKET_LIQUID_HAUL,
                "culinary_vessel_v1", "", 347150U, 0U));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 3U, "task:soup_3_simmer_pea_soup_74c",
                "3. Simmer Pea Soup >=74C",
                "Kitchen Induction Hearth (11,1,4)", 1400000000ULL, 22U, HaulModality::NONE,
                "culinary_vessel_v1", "", 347150U, 0U,
                "Confirm core soup temperature held >= 74C (347150 mK) for >= 120s Bigelow lethality with bay leaf and iodized sea salt?"));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 4U, "task:soup_4_package_distribute_tiffins",
                "4. Package & Distribute Tiffins",
                "Hearth -> R1 Neighborhood Tiffin Rack", 500000000ULL, 23U, HaulModality::TIFFIN_CARRIER_HAUL));
        } else if (asset_id == "asset:garage_fab") {
            wf.workflow_id = "wf:garage_fab";
            wf.title = "Garage Fabrication & L7 Zero-Bill Woodwork Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, 1U, "task:fab_1_mill_raised_bed",
                "1. Mill Raised Bed Frame",
                "Garage Workbench (9,1,13)", 800000000ULL, 25U, HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 2U, "task:fab_2_assemble_compost_kiln",
                "2. Assemble Compost & Kiln [Wheelbarrow/Dolly Heavy Haul]",
                "Garage -> Backyard Kiln Pad (23,1,8)", 1100000000ULL, 24U, HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 3U, "task:fab_3_build_poultry_coop",
                "3. Build Poultry Coop [Wheelbarrow/Dolly Heavy Haul]",
                "Garage -> Backyard Coop Run (22,1,11)", 1600000000ULL, 19U, HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 4U, "task:fab_4_l7_custom_woodwork_order",
                "4. L7 Custom Woodwork Order [+$180 Bill Offset]",
                "Garage Joinery Bench (9,1,13)", 1500000000ULL, 29U, HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL,
                "", "", 0U, 0U,
                "Confirm mortise-and-tenon joinery tolerances inspected and natural linseed finish cured before L7 client pickup?",
                18000));
        } else if (asset_id == "asset:poultry_and_soil") {
            wf.workflow_id = "wf:poultry_and_soil";
            wf.title = "Pasture Poultry, Thermophilic Compost & Biochar Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, 1U, "task:mu_1_feed_bsfl_collect_eggs",
                "1. Feed BSFL Larvae & Collect Pasture Eggs [Granular Haul]",
                "Backyard Poultry Coop (22,1,11)", 400000000ULL, 20U, HaulModality::GRANULAR_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 2U, "task:mu_2_thermophilic_compost_55c",
                "2. Turn Thermophilic Compost Drum >=55C (328150 mK)",
                "Backyard Compost Bay (23,1,8)", 500000000ULL, 24U, HaulModality::GRANULAR_HAUL,
                "compost_drum_probe_v1", "", 328150U, 0U));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 3U, "task:mu_3_tlud_biochar_charge",
                "3. Run TLUD Pyrolysis Biochar Kiln & Charge Terra Preta",
                "Backyard Kiln Pad (23,1,8)", 900000000ULL, 24U, HaulModality::WHEELBARROW_DOLLY_HEAVY_HAUL,
                "kiln_thermocouple_v1", "", 773150U, 0U));
        } else if (asset_id == "asset:water_and_power") {
            wf.workflow_id = "wf:water_and_power";
            wf.title = "Rainwater Cistern & Solar Microgrid Stewardship";
            wf.tasks.push_back(make_task(
                wf.workflow_id, 1U, "task:infra_1_cistern_first_flush",
                "1. Inspect Cistern First-Flush Diverter & Ceramic Filter",
                "Cistern Pad (18,1,3)", 450000000ULL, 27U, HaulModality::BUCKET_LIQUID_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 2U, "task:infra_2_solar_bus_balance",
                "2. Dust 400W PV Array & Verify 48V LiFePO4 Cell Balance",
                "Roof Solar Array (12,4,8)", 750000000ULL, 18U, HaulModality::NONE,
                "solar_bus_meter_v1", "", 0U, 0U));
        } else {
            const std::string slug = MakeSlug(asset_id);
            wf.workflow_id = "wf:" + slug;
            wf.title = asset->title + " Local Stewardship Lifecycle";
            wf.tasks.push_back(make_task(
                wf.workflow_id, 1U, "task:" + slug + "_1_stage",
                "1. Stage Local Materials & Tooling [Granular Haul]",
                "Homestead Workspace", asset->maintenance_exergy_uj / 3ULL + 100000000ULL, 18U,
                HaulModality::GRANULAR_HAUL));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 2U, "task:" + slug + "_2_execute",
                "2. Execute Local Commons Production [Unmodeled micro-detail included]",
                "Homestead Workspace", asset->maintenance_exergy_uj / 2ULL + 200000000ULL, 18U,
                HaulModality::NONE, "", "", 0U, 0U,
                "Confirm physical quality check and local safety envelope verified by steward?"));
            wf.tasks.push_back(make_task(
                wf.workflow_id, 3U, "task:" + slug + "_3_distribute",
                "3. Package & Share Output Across Governed Ring",
                "Node Commons Rack", asset->maintenance_exergy_uj / 4ULL + 100000000ULL, 23U,
                HaulModality::TIFFIN_CARRIER_HAUL));
        }

        asset->local_workflows_instantiated = 1U;
        asset->workflow_ids.push_back(wf.workflow_id);
        workflows_.push_back(wf);
        RefreshL4AutomationModalities();
    }

    // ------------------------------------------------------------------------
    // Strictly 8-Byte Aligned Member Layout (-Wpadded Clean)
    // ------------------------------------------------------------------------
    std::vector<L6NeedRecord>       needs_{};
    std::vector<L6IntentRequest>    requests_{};
    std::vector<L6PendingDecision>  pending_decisions_{};
    std::vector<L5GovernedAsset>    governed_assets_{};
    std::vector<L4WorkflowInstance> workflows_{};
    std::vector<L6NeedRecord>       last_search_results_{};
    L2TelemetryTable                l2_telemetry_{};
    L3NodeLedger                    ledger_{};
    std::string                     bootstrap_source_path_{};
    std::string                     last_command_action_{};
    std::string                     last_command_status_{};
    uint64_t                        current_tick_{1ULL};
    uint64_t                        custom_counter_{0ULL};
    uint32_t                        sim_day_{1U};
    uint8_t                         bootstrapped_from_jsonld_{0U};
    [[maybe_unused]] uint8_t        pad_[3]{0U};
};
static_assert(alignof(SharedStackRuntime) == 8, "SharedStackRuntime must be 8-byte aligned.");

} // namespace oasis::stack
