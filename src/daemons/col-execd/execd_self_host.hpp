#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif

#if __has_include("uhai/uhai_ring_buffer.hpp")
#include "uhai/uhai_ring_buffer.hpp"
#else
#include "../../../core/1_simulation/src/uhai/uhai_ring_buffer.hpp"
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include "../col-storaged/storaged_engine.hpp"

namespace oasis::daemons::execd {

/**
 * @brief Scenario Zero Full-Spectrum Software & Digital Asset Taxonomy (SCN-ZERO-SOFTWARE).
 */
enum class SoftwareAssetCategory : uint8_t {
    OS_AND_KERNEL_DISTRO        = 1,
    L6_NATIVE_OR_DETOXED_APP    = 2,
    VIDEO_GAME_OR_ENGINE        = 3,
    COMMUNITY_WASM_MOD          = 4,
    REUSABLE_LIBRARY_OR_SDK     = 5,
    CONSTITUTIONAL_STACK_DAEMON = 6
};

/**
 * @brief Recursive BOM/SBOM Resolution Tiers (Scenario Alpha & Scenario Zero).
 */
enum class BomResolutionTier : uint8_t {
    LOCAL_STOCK_OR_FAB          = 1,
    MESH_PEER_BCC_SUBORDER      = 2,
    UPWARD_YIELD_L7_PROCUREMENT = 3,
    TIER_1_LOCAL_WORKSHOP       = 1,
    TIER_2_MESH_PEER_AND_BETA   = 2,
    TIER_3_LAYER7_LEAF_IMPORT   = 3
};
using DAGSourceTier = BomResolutionTier;
using enum BomResolutionTier;

/**
 * @brief Pillar I & II: Studio Capability Advertisement, Unconditional Studio Veto & Recursive SBOM DAG.
 *
 * Strictly 8-byte aligned and padded for -Wpadded cleanliness with zero IEEE-754 floats.
 */
struct alignas(8) SoftwareCommissionSpec {
    std::string project_id{};
    SoftwareAssetCategory category{SoftwareAssetCategory::L6_NATIVE_OR_DETOXED_APP};
    bool contains_surveillance_telemetry{false};
    bool contains_drm_lock_in{false};
    bool contains_predatory_dark_patterns{false};
    bool vetoed_by_studio_conscience{false}; // Unconditional Protocol of Refusal
    [[maybe_unused]] uint8_t pad_0_[3]{0};
    uint32_t total_sbom_dependencies{0};
    uint32_t vendored_and_detoxed_sbom_dependencies{0};
};
static_assert(alignof(SoftwareCommissionSpec) == 8, "SoftwareCommissionSpec must be 8-byte aligned.");

/**
 * @brief Evaluate Unconditional Studio Veto (`Protocol of Refusal`), Anti-Dark-Pattern Gate & SBOM DAG.
 */
[[nodiscard]] inline bool EvaluateStudioVetoAndSbomDag(const SoftwareCommissionSpec& spec) noexcept {
    // 1. Unconditional Sovereign Studio Veto (Protocol of Refusal)
    if (spec.vetoed_by_studio_conscience) {
        return false;
    }

    // 2. Anti-Surveillance, Anti-DRM & Anti-Dark-Pattern Constitutional Gate
    if (spec.contains_surveillance_telemetry ||
        spec.contains_drm_lock_in ||
        spec.contains_predatory_dark_patterns) {
        return false;
    }

    // 3. Every SBOM dependency in the DAG must be 100% vendored & detoxed (zero live L7 cloud tethers)
    return spec.vendored_and_detoxed_sbom_dependencies == spec.total_sbom_dependencies;
}

[[nodiscard]] inline bool evaluate_studio_veto_and_sbom_dag(const SoftwareCommissionSpec& spec) noexcept {
    return EvaluateStudioVetoAndSbomDag(spec);
}

/**
 * @brief Pillar III & IV: 3-Node Reproducible Build, 20-Year Hardware Frugality & Oasis Canary Gate.
 *
 * Strictly 8-byte aligned and padded for -Wpadded cleanliness with zero IEEE-754 floats.
 */
struct alignas(8) SoftwareVerificationReport {
    SoftwareAssetCategory category{SoftwareAssetCategory::VIDEO_GAME_OR_ENGINE};
    [[maybe_unused]] uint8_t pad_0_[7]{0};
    std::string builder_1_sha256{};
    std::string builder_2_sha256{};
    std::string builder_3_sha256{};
    uint32_t external_network_calls_during_build{0}; // Must be 0 (hermetic sandbox)
    union {
        uint32_t idle_ram_mib_on_salvaged_laptop{240};
        uint32_t idle_ram_mib;
    };
    uint32_t max_ram_budget_mib{512};                // 20-year hardware longevity frugality gate
    bool exposes_sandboxed_mod_abi{true};
    [[maybe_unused]] uint8_t pad_1_[3]{0};
    // Constitutional Stack / RTOS extra gates:
    uint32_t floating_point_opcodes_detected{0};
    uint32_t layer_skipping_imports_detected{0};
    uint32_t oasis_shadow_soak_ticks{12000};
    uint32_t unhandled_panics_or_deadlocks{0};
    bool human_steward_co_signed{true};
    bool ai_unsupervised_deploy_attempted{false};
    [[maybe_unused]] uint8_t pad_2_[6]{0};
};
static_assert(alignof(SoftwareVerificationReport) == 8, "SoftwareVerificationReport must be 8-byte aligned.");

/**
 * @brief Verify Software Release Pipeline across Universal Gates A & B and Constitutional Gate C.
 */
[[nodiscard]] inline bool VerifySoftwareReleasePipeline(const SoftwareVerificationReport& r) noexcept {
    if (r.ai_unsupervised_deploy_attempted || !r.human_steward_co_signed) {
        return false;
    }
    if (r.external_network_calls_during_build > 0) {
        return false;
    }

    // 3-Node Bit-for-Bit Reproducible Hash Consensus
    if (r.builder_1_sha256.empty() ||
        r.builder_1_sha256 != r.builder_2_sha256 ||
        r.builder_2_sha256 != r.builder_3_sha256) {
        return false;
    }

    // 20-Year Salvaged Hardware Frugality Gate
    if (r.idle_ram_mib_on_salvaged_laptop > r.max_ram_budget_mib) {
        return false;
    }

    // Games and Apps must expose the composable Modding/Plugin ABI
    if ((r.category == SoftwareAssetCategory::VIDEO_GAME_OR_ENGINE ||
         r.category == SoftwareAssetCategory::L6_NATIVE_OR_DETOXED_APP) &&
        !r.exposes_sandboxed_mod_abi) {
        return false;
    }

    // Constitutional Stack Daemons additionally require 0 float opcodes, 0 layer skipping & >= 10,000-tick Oasis Canary Soak
    if (r.category == SoftwareAssetCategory::CONSTITUTIONAL_STACK_DAEMON) {
        if (r.floating_point_opcodes_detected > 0 || r.layer_skipping_imports_detected > 0) {
            return false;
        }
        if (r.oasis_shadow_soak_ticks < 10000U || r.unhandled_panics_or_deadlocks > 0) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] inline bool verify_software_release_pipeline(const SoftwareVerificationReport& r) noexcept {
    return VerifySoftwareReleasePipeline(r);
}

/**
 * @brief Pillar V & VI: Thermodynamic Compute Heat Recovery (>= 8000 bp) & Pairwise BCC Bounty Settlement.
 */
struct alignas(8) BccSoftwareChannel {
    int64_t balance_studio_to_peer_uj{0};
    int64_t balance_peer_to_studio_uj{0};
    int64_t credit_limit_uj{200000000000000LL};
};
static_assert(sizeof(BccSoftwareChannel) == 24, "BccSoftwareChannel must be 24 bytes.");
static_assert(alignof(BccSoftwareChannel) == 8, "BccSoftwareChannel must be 8-byte aligned.");

/**
 * @brief Verify >= 8000 bp (80%) hydronic heat recovery and settle BCC bounty & net compute exergy.
 */
[[nodiscard]] constexpr bool ComputeHydronicHeatRecoveryAndBcc(
    BccSoftwareChannel& ch,
    uint64_t sbom_bounty_uj,
    uint64_t gross_compile_render_uj,
    uint64_t hydronic_heat_recovered_uj) noexcept {
    if (gross_compile_render_uj == 0 || hydronic_heat_recovered_uj > gross_compile_render_uj) {
        return false;
    }
    const uint64_t recovery_bp = (hydronic_heat_recovered_uj * 10000ULL) / gross_compile_render_uj;
    if (recovery_bp < 8000ULL) {
        return false; // Must recover >= 8000 bp (80%) of server heat into settlement hydronics
    }

    const uint64_t net_compute_uj = gross_compile_render_uj - hydronic_heat_recovered_uj;
    const uint64_t total_transfer_uj = sbom_bounty_uj + net_compute_uj;
    if (ch.balance_peer_to_studio_uj + static_cast<int64_t>(total_transfer_uj) > ch.credit_limit_uj) {
        return false;
    }
    ch.balance_peer_to_studio_uj += static_cast<int64_t>(total_transfer_uj);
    ch.balance_studio_to_peer_uj -= static_cast<int64_t>(total_transfer_uj);
    return (ch.balance_peer_to_studio_uj + ch.balance_studio_to_peer_uj) == 0;
}

[[nodiscard]] constexpr bool ComputeHydronicHeatRecoveryAndBcc(
    oasis::daemons::storaged::PairwiseBccChannel& ch,
    uint64_t sbom_bounty_uj,
    uint64_t gross_compile_render_uj,
    uint64_t hydronic_heat_recovered_uj) noexcept {
    if (gross_compile_render_uj == 0 || hydronic_heat_recovered_uj > gross_compile_render_uj) {
        return false;
    }
    const uint64_t recovery_bp = (hydronic_heat_recovered_uj * 10000ULL) / gross_compile_render_uj;
    if (recovery_bp < 8000ULL) {
        return false;
    }
    const uint64_t net_compute_uj = gross_compile_render_uj - hydronic_heat_recovered_uj;
    const uint64_t total_transfer_uj = sbom_bounty_uj + net_compute_uj;
    return oasis::daemons::storaged::SettlePairwiseBccTransfer(ch, total_transfer_uj);
}

[[nodiscard]] constexpr bool ComputeHydronicHeatRecoveryAndBcc(
    BccSoftwareChannel& ch,
    uint64_t gross_compile_render_uj,
    uint64_t hydronic_heat_recovered_uj) noexcept {
    return ComputeHydronicHeatRecoveryAndBcc(ch, 0ULL, gross_compile_render_uj, hydronic_heat_recovered_uj);
}

[[nodiscard]] constexpr bool ComputeHydronicHeatRecoveryAndBcc(
    oasis::daemons::storaged::PairwiseBccChannel& ch,
    uint64_t gross_compile_render_uj,
    uint64_t hydronic_heat_recovered_uj) noexcept {
    return ComputeHydronicHeatRecoveryAndBcc(ch, 0ULL, gross_compile_render_uj, hydronic_heat_recovered_uj);
}

[[nodiscard]] constexpr bool ComputeHydronicHeatRecoveryAndBcc(
    uint64_t gross_compile_render_uj,
    uint64_t hydronic_heat_recovered_uj,
    uint64_t* out_net_compute_uj = nullptr) noexcept {
    if (gross_compile_render_uj == 0 || hydronic_heat_recovered_uj > gross_compile_render_uj) {
        return false;
    }
    const uint64_t recovery_bp = (hydronic_heat_recovered_uj * 10000ULL) / gross_compile_render_uj;
    if (recovery_bp < 8000ULL) {
        return false;
    }
    if (out_net_compute_uj != nullptr) {
        *out_net_compute_uj = gross_compile_render_uj - hydronic_heat_recovered_uj;
    }
    return true;
}

[[nodiscard]] constexpr bool settle_sbom_bounty_and_compute_bcc(
    BccSoftwareChannel& ch,
    uint64_t sbom_bounty_uj,
    uint64_t gross_compile_render_uj,
    uint64_t hydronic_heat_recovered_uj) noexcept {
    return ComputeHydronicHeatRecoveryAndBcc(ch, sbom_bounty_uj, gross_compile_render_uj, hydronic_heat_recovered_uj);
}

/**
 * @brief Recursive BOM/SBOM Component Node (Scenario Alpha & Scenario Zero).
 */
struct alignas(8) RecursiveBomNode {
    uint64_t component_cid_hash{0};
    uint64_t required_mass_mg{0};
    uint64_t required_exergy_uj{3600000000ULL};
    uint64_t peer_supplier_did_hash{0};
    BomResolutionTier resolved_tier{BomResolutionTier::LOCAL_STOCK_OR_FAB};
    bool available_locally{false};
    bool available_on_mesh{false};
    bool upward_yield_emitted{false};
    [[maybe_unused]] uint8_t pad_[4]{0};
};
static_assert(sizeof(RecursiveBomNode) == 40, "RecursiveBomNode must be exactly 40 bytes.");
static_assert(alignof(RecursiveBomNode) == 8, "RecursiveBomNode must be 8-byte aligned.");
static_assert(std::is_standard_layout_v<RecursiveBomNode>, "RecursiveBomNode must be standard layout.");
static_assert(std::is_trivially_copyable_v<RecursiveBomNode>, "RecursiveBomNode must be trivially copyable.");

/**
 * @brief Resolve a blueprint BOM/SBOM item across Tier 1 (Local), Tier 2 (Mesh Peer BCC), or Tier 3 (Upward Yield to L5->L6->L7).
 */
inline BomResolutionTier ResolveRecursiveBomNode(
    uint64_t component_cid_hash,
    bool available_locally,
    bool available_on_mesh,
    oasis::uhai::UpwardYieldPacket* out_yield_packet = nullptr,
    uint64_t required_exergy_uj = 3600000000ULL,
    oasis::daemons::storaged::PairwiseBccChannel* mesh_bcc_channel = nullptr) noexcept {
    if (available_locally) {
        return BomResolutionTier::LOCAL_STOCK_OR_FAB;
    }

    if (available_on_mesh) {
        if (mesh_bcc_channel != nullptr && required_exergy_uj > 0) {
            if (!oasis::daemons::storaged::SettlePairwiseBccTransfer(*mesh_bcc_channel, required_exergy_uj)) {
                // Fall through to Tier 3 upward yield if mesh BCC credit window is exhausted
            } else {
                return BomResolutionTier::MESH_PEER_BCC_SUBORDER;
            }
        } else {
            return BomResolutionTier::MESH_PEER_BCC_SUBORDER;
        }
    }

    // Tier 3: Unresolved locally and on mesh -> emit UpwardYieldPacket to L5 -> L6 -> L7 GPO Carapace
    if (out_yield_packet != nullptr) {
        std::memset(out_yield_packet, 0, sizeof(oasis::uhai::UpwardYieldPacket));
        out_yield_packet->yield_id = (component_cid_hash != 0) ? component_cid_hash : 1ULL;
        out_yield_packet->workflow_token_id = component_cid_hash;
        out_yield_packet->timestamp_hlc = 1000ULL;
        out_yield_packet->reason = oasis::uhai::YieldReason::RESOURCE_DEFICIT_CURTAILMENT;
        out_yield_packet->priority_tier = 2U;
        out_yield_packet->required_exergy_mj = static_cast<int64_t>(required_exergy_uj / 1000000000ULL);
        std::snprintf(out_yield_packet->task_gate_id, sizeof(out_yield_packet->task_gate_id),
                      "Gate_L7_GPO_Procurement");
        std::snprintf(out_yield_packet->claimant_did, sizeof(out_yield_packet->claimant_did),
                      "did:peer:steward:l4-execd-bom-resolver");
        std::snprintf(out_yield_packet->description, sizeof(out_yield_packet->description),
                      "Tier 3 BOM leaf import required via L5->L6->L7 GPO for CID 0x%llx",
                      static_cast<unsigned long long>(component_cid_hash));
    }

    return BomResolutionTier::UPWARD_YIELD_L7_PROCUREMENT;
}

inline BomResolutionTier ResolveRecursiveBomNode(
    uint64_t component_cid_hash,
    bool available_locally,
    bool available_on_mesh,
    oasis::uhai::UpwardYieldPacket& out_yield_packet,
    uint64_t required_exergy_uj = 3600000000ULL,
    oasis::daemons::storaged::PairwiseBccChannel* mesh_bcc_channel = nullptr) noexcept {
    return ResolveRecursiveBomNode(
        component_cid_hash,
        available_locally,
        available_on_mesh,
        &out_yield_packet,
        required_exergy_uj,
        mesh_bcc_channel);
}

inline BomResolutionTier ResolveRecursiveBomNode(
    RecursiveBomNode& node,
    oasis::uhai::UpwardYieldPacket* out_yield_packet = nullptr,
    oasis::daemons::storaged::PairwiseBccChannel* mesh_bcc_channel = nullptr) noexcept {
    oasis::uhai::UpwardYieldPacket local_pkt{};
    oasis::uhai::UpwardYieldPacket* target_pkt = (out_yield_packet != nullptr) ? out_yield_packet : &local_pkt;

    node.resolved_tier = ResolveRecursiveBomNode(
        node.component_cid_hash,
        node.available_locally,
        node.available_on_mesh,
        target_pkt,
        node.required_exergy_uj,
        mesh_bcc_channel);
    node.upward_yield_emitted = (node.resolved_tier == BomResolutionTier::UPWARD_YIELD_L7_PROCUREMENT);
    return node.resolved_tier;
}

} // namespace oasis::daemons::execd

namespace collective::scenario_zero {
using namespace oasis::daemons::execd;
} // namespace collective::scenario_zero

namespace collective::l4 {
using namespace oasis::daemons::execd;
} // namespace collective::l4
