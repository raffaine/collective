#include <iostream>
#include <cstdint>
#include <cstddef>
#include <array>
#include <string_view>
#include <type_traits>

namespace collective::governance {

enum class YieldReason : uint8_t {
    HUMAN_TASK_GATE_COMPLETION = 0x01,
    RESOURCE_DEFICIT_CURTAILMENT = 0x02,
    UNRESOLVABLE_MORAL_CONFLICT = 0x03,
    TREATY_BREACH_EXCEPTION      = 0x04
};

enum class ApprovalStatus : uint8_t {
    PENDING_REVIEW = 0x00,
    RATIFIED_APPROVED = 0x01,
    REJECTED_COMPENSATED = 0x02,
    ESCALATED_ASSEMBLY = 0x03
};

// Memory-aligned packet received across l4_l5.sock
struct alignas(8) UpwardYieldPacket {
    uint64_t yield_id;                  // Monotonic yield identifier
    uint64_t workflow_token_id;         // BPMN execution token ID in col-execd
    uint64_t timestamp_hlc;             // Hybrid Logical Clock of yield event
    YieldReason reason;                 // Reason for upward yield
    uint8_t priority_tier;              // 1 to 5
    uint8_t reserved[6];                // Alignment padding
    char task_gate_id[64];              // BPMN activity ID (e.g. "Gate_Quality_Inspect")
    char claimant_did[64];              // Claiming steward's DID
    int64_t required_exergy_mj;         // Escrow amount (in 3.6 MJ Exergy Credits * 1000)
    char description[256];              // Human-readable task description
    uint8_t sha256_spec_digest[32];     // Hash of task specification
};

// Memory-aligned resolution emitted back across l4_l5.sock
struct alignas(8) GovernanceResolutionPacket {
    uint64_t yield_id;                  // Matching yield identifier
    uint64_t resolution_hlc;            // Timestamp of resolution
    ApprovalStatus status;              // RATIFIED_APPROVED or REJECTED
    uint8_t trust_ring;                 // R0 through R6
    uint8_t reserved[6];                // Alignment padding
    char steward_approver_did[64];      // DID of approving steward
    uint8_t ed25519_signature[64];      // Web Crypto / ATECC608A signature
    int64_t released_escrow_mj;         // Exergy credit amount released from escrow
    char resolution_rationale[256];     // Deliberative justification
};

// Tripartite Ricardian Treaty payload
struct alignas(8) TreatyRatificationPayload {
    char treaty_cid[65];                // SHA-256 hex string (64 chars + null)
    uint8_t trust_ring_scope;           // Maximum ring boundary (e.g. R3)
    uint8_t bicameral_tier;             // 1 = Core (80%), 2 = Meta (66%), 3 = Ordinary (50%)
    uint8_t reserved_byte;              // Natural alignment padding to offset 68
    uint32_t cooloff_seconds_remaining;// Mandatory cooloff countdown (offset 68..71)
    int64_t bilateral_credit_limit_mj;  // Credit channel bound (+/- C) (offset 72..79)
    char counterparty_node_did[64];     // Peered Node DID (offset 80..143)
    uint8_t initiator_signature[64];    // Ed25519 signature of initiating steward (offset 144..207)
    uint8_t peer_signature[64];         // Ed25519 signature of counterparty steward (offset 208..271)
    uint8_t tail_padding[8];            // 8-byte tail alignment padding (offset 272..279)
};
static_assert(sizeof(TreatyRatificationPayload) == 280, "TreatyRatificationPayload must be exactly 280 bytes.");
static_assert(alignof(TreatyRatificationPayload) == 8, "TreatyRatificationPayload must align to 8 bytes.");

} // namespace collective::governance

namespace collective::proxy {

enum class ThreatLevel : uint8_t {
    BENIGN   = 0x00,
    STANDARD = 0x01,
    HOSTILE  = 0x02,
    SIEGE    = 0x03
};

enum class MunicipalFsmState : uint8_t {
    IDLE       = 0x00,
    INSPECTION = 0x01,
    CITATION   = 0x02,
    STOP_WORK  = 0x03,
    HEARING    = 0x04,
    SEVERANCE  = 0x05
};

enum class GridIntertieState : uint8_t {
    DISCONNECTED = 0x00,
    IMPORTING    = 0x01,
    EXPORTING    = 0x02,
    CURTAILED_AUTONOMOUS = 0x03,
    EMERGENCY_LOCKOUT    = 0x04
};

// Memory-aligned payload broadcast across /ws/v1 and REST
struct alignas(8) LegacyProxyStatusPayload {
    uint64_t timestamp_hlc;             // Hybrid Logical Clock
    ThreatLevel threat_level;           // Current DES Threat Level (0..3)
    MunicipalFsmState municipal_state;   // Current 5-state municipal FSM
    GridIntertieState grid_state;       // Physical contactor state
    uint8_t records_firewall_secure;    // 1 = True (Zero DIDs/Keys stored in L7)
    uint8_t reserved[4];                // 8-byte alignment padding
    
    // Physical Grid Intertie Telemetry
    double export_residual_kw;          // Current R_t (kW)
    double grid_export_kw;              // Live exported power (0.0 when curtailed)
    double grid_import_kw;              // Live imported power
    uint32_t curtailment_count_24h;     // Number of autonomous trips in last 24h
    uint8_t pad_curtailment[4];         // 4B explicit padding (offset 44..47)
    
    // Carapace Financial & Municipal Telemetry
    double fiat_bank_balance_usd;       // Liquid operating reserve
    double tax_reserve_escrow_usd;      // Quarterly payroll/sales tax reserve (rho_t)
    double outstanding_fines_usd;       // Accumulated municipal citation fines
    uint32_t active_citations_count;    // Number of active open citations
    uint8_t pad_citations[4];           // 4B explicit padding (offset 76..79)
    
    // Severance Protocol Metrics
    double dependency_ratio_d_t;        // D_t = E_fiat / E_in (Target -> 0.0)
    uint32_t lunar_cycles_autarky;      // Consecutive cycles sustaining D_t <= 0.05
    uint8_t terminal_decoupling_armed;  // 1 = Self-destruct armed for Phase 4
    uint8_t padding[3];                 // 3B tail padding (offset 93..95) to exact 96 bytes
};
static_assert(sizeof(LegacyProxyStatusPayload) == 96, "LegacyProxyStatusPayload must be exactly 96 bytes.");
static_assert(alignof(LegacyProxyStatusPayload) == 8, "LegacyProxyStatusPayload must align to 8 bytes.");

// Inward actuation command received across l6_l7.sock
struct alignas(8) ExternalActuationCommand {
    uint64_t command_id;                // Unique command ID
    uint64_t timestamp_hlc;             // HLC timestamp
    uint8_t action_type;                // 0x01 = Grid Curtail, 0x02 = Fiat PO, 0x03 = DES Response
    uint8_t reserved[7];
    char reference_order_id[64];        // PO or Treaty reference ID
    double max_fiat_authorization_usd;  // Maximum fiat spending cap
    char vendor_endpoint[128];          // External API / Banking webhook destination
    uint8_t l6_auth_signature[64];      // Camouflage-signed Ed25519 token
};

} // namespace collective::proxy

int main() {
    using namespace collective::governance;
    using namespace collective::proxy;

    std::cout << "=== UpwardYieldPacket ===" << std::endl;
    std::cout << "sizeof: " << sizeof(UpwardYieldPacket) << " alignof: " << alignof(UpwardYieldPacket) << std::endl;
    std::cout << "yield_id offset: " << offsetof(UpwardYieldPacket, yield_id) << std::endl;
    std::cout << "workflow_token_id offset: " << offsetof(UpwardYieldPacket, workflow_token_id) << std::endl;
    std::cout << "timestamp_hlc offset: " << offsetof(UpwardYieldPacket, timestamp_hlc) << std::endl;
    std::cout << "reason offset: " << offsetof(UpwardYieldPacket, reason) << std::endl;
    std::cout << "priority_tier offset: " << offsetof(UpwardYieldPacket, priority_tier) << std::endl;
    std::cout << "reserved offset: " << offsetof(UpwardYieldPacket, reserved) << std::endl;
    std::cout << "task_gate_id offset: " << offsetof(UpwardYieldPacket, task_gate_id) << std::endl;
    std::cout << "claimant_did offset: " << offsetof(UpwardYieldPacket, claimant_did) << std::endl;
    std::cout << "required_exergy_mj offset: " << offsetof(UpwardYieldPacket, required_exergy_mj) << std::endl;
    std::cout << "description offset: " << offsetof(UpwardYieldPacket, description) << std::endl;
    std::cout << "sha256_spec_digest offset: " << offsetof(UpwardYieldPacket, sha256_spec_digest) << std::endl;

    std::cout << "\n=== GovernanceResolutionPacket ===" << std::endl;
    std::cout << "sizeof: " << sizeof(GovernanceResolutionPacket) << " alignof: " << alignof(GovernanceResolutionPacket) << std::endl;
    std::cout << "yield_id offset: " << offsetof(GovernanceResolutionPacket, yield_id) << std::endl;
    std::cout << "resolution_hlc offset: " << offsetof(GovernanceResolutionPacket, resolution_hlc) << std::endl;
    std::cout << "status offset: " << offsetof(GovernanceResolutionPacket, status) << std::endl;
    std::cout << "trust_ring offset: " << offsetof(GovernanceResolutionPacket, trust_ring) << std::endl;
    std::cout << "reserved offset: " << offsetof(GovernanceResolutionPacket, reserved) << std::endl;
    std::cout << "steward_approver_did offset: " << offsetof(GovernanceResolutionPacket, steward_approver_did) << std::endl;
    std::cout << "ed25519_signature offset: " << offsetof(GovernanceResolutionPacket, ed25519_signature) << std::endl;
    std::cout << "released_escrow_mj offset: " << offsetof(GovernanceResolutionPacket, released_escrow_mj) << std::endl;
    std::cout << "resolution_rationale offset: " << offsetof(GovernanceResolutionPacket, resolution_rationale) << std::endl;

    std::cout << "\n=== TreatyRatificationPayload ===" << std::endl;
    std::cout << "sizeof: " << sizeof(TreatyRatificationPayload) << " alignof: " << alignof(TreatyRatificationPayload) << std::endl;
    std::cout << "treaty_cid offset: " << offsetof(TreatyRatificationPayload, treaty_cid) << std::endl;
    std::cout << "trust_ring_scope offset: " << offsetof(TreatyRatificationPayload, trust_ring_scope) << std::endl;
    std::cout << "bicameral_tier offset: " << offsetof(TreatyRatificationPayload, bicameral_tier) << std::endl;
    std::cout << "reserved_byte offset: " << offsetof(TreatyRatificationPayload, reserved_byte) << std::endl;
    std::cout << "cooloff_seconds_remaining offset: " << offsetof(TreatyRatificationPayload, cooloff_seconds_remaining) << std::endl;
    std::cout << "bilateral_credit_limit_mj offset: " << offsetof(TreatyRatificationPayload, bilateral_credit_limit_mj) << std::endl;
    std::cout << "counterparty_node_did offset: " << offsetof(TreatyRatificationPayload, counterparty_node_did) << std::endl;
    std::cout << "initiator_signature offset: " << offsetof(TreatyRatificationPayload, initiator_signature) << std::endl;
    std::cout << "peer_signature offset: " << offsetof(TreatyRatificationPayload, peer_signature) << std::endl;
    std::cout << "tail_padding offset: " << offsetof(TreatyRatificationPayload, tail_padding) << std::endl;

    std::cout << "\n=== LegacyProxyStatusPayload ===" << std::endl;
    std::cout << "sizeof: " << sizeof(LegacyProxyStatusPayload) << " alignof: " << alignof(LegacyProxyStatusPayload) << std::endl;
    std::cout << "timestamp_hlc offset: " << offsetof(LegacyProxyStatusPayload, timestamp_hlc) << std::endl;
    std::cout << "threat_level offset: " << offsetof(LegacyProxyStatusPayload, threat_level) << std::endl;
    std::cout << "municipal_state offset: " << offsetof(LegacyProxyStatusPayload, municipal_state) << std::endl;
    std::cout << "grid_state offset: " << offsetof(LegacyProxyStatusPayload, grid_state) << std::endl;
    std::cout << "records_firewall_secure offset: " << offsetof(LegacyProxyStatusPayload, records_firewall_secure) << std::endl;
    std::cout << "reserved offset: " << offsetof(LegacyProxyStatusPayload, reserved) << std::endl;
    std::cout << "export_residual_kw offset: " << offsetof(LegacyProxyStatusPayload, export_residual_kw) << std::endl;
    std::cout << "grid_export_kw offset: " << offsetof(LegacyProxyStatusPayload, grid_export_kw) << std::endl;
    std::cout << "grid_import_kw offset: " << offsetof(LegacyProxyStatusPayload, grid_import_kw) << std::endl;
    std::cout << "curtailment_count_24h offset: " << offsetof(LegacyProxyStatusPayload, curtailment_count_24h) << std::endl;
    std::cout << "pad_curtailment offset: " << offsetof(LegacyProxyStatusPayload, pad_curtailment) << std::endl;
    std::cout << "fiat_bank_balance_usd offset: " << offsetof(LegacyProxyStatusPayload, fiat_bank_balance_usd) << std::endl;
    std::cout << "tax_reserve_escrow_usd offset: " << offsetof(LegacyProxyStatusPayload, tax_reserve_escrow_usd) << std::endl;
    std::cout << "outstanding_fines_usd offset: " << offsetof(LegacyProxyStatusPayload, outstanding_fines_usd) << std::endl;
    std::cout << "active_citations_count offset: " << offsetof(LegacyProxyStatusPayload, active_citations_count) << std::endl;
    std::cout << "pad_citations offset: " << offsetof(LegacyProxyStatusPayload, pad_citations) << std::endl;
    std::cout << "dependency_ratio_d_t offset: " << offsetof(LegacyProxyStatusPayload, dependency_ratio_d_t) << std::endl;
    std::cout << "lunar_cycles_autarky offset: " << offsetof(LegacyProxyStatusPayload, lunar_cycles_autarky) << std::endl;
    std::cout << "terminal_decoupling_armed offset: " << offsetof(LegacyProxyStatusPayload, terminal_decoupling_armed) << std::endl;
    std::cout << "padding offset: " << offsetof(LegacyProxyStatusPayload, padding) << std::endl;

    std::cout << "\n=== ExternalActuationCommand ===" << std::endl;
    std::cout << "sizeof: " << sizeof(ExternalActuationCommand) << " alignof: " << alignof(ExternalActuationCommand) << std::endl;
    std::cout << "command_id offset: " << offsetof(ExternalActuationCommand, command_id) << std::endl;
    std::cout << "timestamp_hlc offset: " << offsetof(ExternalActuationCommand, timestamp_hlc) << std::endl;
    std::cout << "action_type offset: " << offsetof(ExternalActuationCommand, action_type) << std::endl;
    std::cout << "reserved offset: " << offsetof(ExternalActuationCommand, reserved) << std::endl;
    std::cout << "reference_order_id offset: " << offsetof(ExternalActuationCommand, reference_order_id) << std::endl;
    std::cout << "max_fiat_authorization_usd offset: " << offsetof(ExternalActuationCommand, max_fiat_authorization_usd) << std::endl;
    std::cout << "vendor_endpoint offset: " << offsetof(ExternalActuationCommand, vendor_endpoint) << std::endl;
    std::cout << "l6_auth_signature offset: " << offsetof(ExternalActuationCommand, l6_auth_signature) << std::endl;

    return 0;
}
