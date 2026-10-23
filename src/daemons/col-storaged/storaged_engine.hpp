#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace oasis::daemons::storaged {

/**
 * @brief Layer 3 Content-Addressed Merkle Artifact Record (SCN-ZERO-SOFTWARE / V3.1).
 *
 * Strictly fixed-point integer wire structure with zero IEEE-754 floats and
 * explicit padding for zero warnings under -std=c++20 -Wall -Wextra -Wpadded -Werror.
 */
struct alignas(8) MerkleArtifactRecord {
    uint64_t artifact_cid_hash{0};       // 8B (offset 0)
    uint64_t sbom_merkle_root_hash{0};   // 8B (offset 8)
    uint64_t agreed_sha256_prefix{0};    // 8B (offset 16)
    uint32_t builder_consensus_count{0}; // 4B (offset 24): >= 3 independent builders required
    uint16_t asset_category{0};          // 2B (offset 28)
    uint8_t  is_verified_active{0};      // 1B (offset 30)
    uint8_t  pad_{0};                    // 1B (offset 31 -> 32B total)
};
static_assert(sizeof(MerkleArtifactRecord) == 32, "MerkleArtifactRecord must be exactly 32 bytes.");
static_assert(alignof(MerkleArtifactRecord) == 8, "MerkleArtifactRecord must be 8-byte aligned.");
static_assert(std::is_standard_layout_v<MerkleArtifactRecord>, "MerkleArtifactRecord must be standard layout.");
static_assert(std::is_trivially_copyable_v<MerkleArtifactRecord>, "MerkleArtifactRecord must be trivially copyable.");

/**
 * @brief Layer 3 Pairwise Bilateral Clearing Channel (BCC) Ledger State.
 *
 * Enforces zero-sum thermodynamic mutual-aid clearing:
 *   B_{u->v}(t) + B_{v->u}(t) == 0, bounded by [-C_{u->v}, +C_{v->u}].
 * Strictly integer microjoules (_uj) with zero IEEE-754 floating-point fields.
 */
struct alignas(8) PairwiseBccChannel {
    int64_t  balance_u_to_v_uj{0};                    // 8B (offset 0)
    int64_t  balance_v_to_u_uj{0};                    // 8B (offset 8)
    int64_t  credit_limit_u_to_v_uj{200000000000000LL}; // 8B (offset 16): default 200 MJ credit window
    int64_t  credit_limit_v_to_u_uj{200000000000000LL}; // 8B (offset 24): default 200 MJ credit window
    uint64_t peer_did_hash{0};                        // 8B (offset 32 -> 40B total)
};
static_assert(sizeof(PairwiseBccChannel) == 40, "PairwiseBccChannel must be exactly 40 bytes.");
static_assert(alignof(PairwiseBccChannel) == 8, "PairwiseBccChannel must be 8-byte aligned.");
static_assert(std::is_standard_layout_v<PairwiseBccChannel>, "PairwiseBccChannel must be standard layout.");
static_assert(std::is_trivially_copyable_v<PairwiseBccChannel>, "PairwiseBccChannel must be trivially copyable.");

/**
 * @brief Baseline R0 Human Care Categories (Non-Commensurable / Non-Tokenizable).
 */
enum class BaselineCareCategory : uint16_t {
    HUMBLE_HEARTH_PEA_SOUP_BEAN_STEW = 1,
    WARM_SHOWER_AND_HYDRONIC_HEATING = 2,
    SANITATION_AND_COMPOST_STEWARDSHIP = 3,
    PERINATAL_AND_CHILDCARE_AFFECTION = 4,
    ELDER_AND_EMERGENCY_HEALTH_CARE = 5,
    OPEN_KNOWLEDGE_AGORA_LEARNING = 6
};

/**
 * @brief Settle a Pairwise Bilateral Clearing Channel (BCC) transfer in microjoules (_uj).
 *
 * Enforces:
 * 1. Prior and post zero-sum conservation invariant: (balance_u_to_v_uj + balance_v_to_u_uj) == 0.
 * 2. Mutual-aid credit window bounds: neither side may exceed the ratified credit limit.
 */
[[nodiscard]] constexpr bool SettlePairwiseBccTransfer(
    PairwiseBccChannel& ch,
    uint64_t transfer_uj) noexcept {
    // 1. Pre-condition: channel must satisfy zero-sum conservation
    if ((ch.balance_u_to_v_uj + ch.balance_v_to_u_uj) != 0) {
        return false;
    }

    // 2. Guard against int64_t overflow on conversion
    if (transfer_uj > static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
        return false;
    }

    // 3. Reject negative credit limits
    if (ch.credit_limit_u_to_v_uj < 0 || ch.credit_limit_v_to_u_uj < 0) {
        return false;
    }

    const int64_t delta_uj = static_cast<int64_t>(transfer_uj);
    if (delta_uj > 0 &&
        ch.balance_u_to_v_uj > (std::numeric_limits<int64_t>::max() - delta_uj)) {
        return false;
    }

    const int64_t next_u_to_v = ch.balance_u_to_v_uj + delta_uj;
    const int64_t next_v_to_u = ch.balance_v_to_u_uj - delta_uj;

    // 4. Enforce mutual-aid credit window bounds
    if (next_u_to_v > ch.credit_limit_u_to_v_uj ||
        (-next_v_to_u) > ch.credit_limit_v_to_u_uj) {
        return false;
    }

    ch.balance_u_to_v_uj = next_u_to_v;
    ch.balance_v_to_u_uj = next_v_to_u;

    // 5. Post-condition: strict zero-sum conservation invariant
    return (ch.balance_u_to_v_uj + ch.balance_v_to_u_uj) == 0;
}

/**
 * @brief Snake-case alias for SettlePairwiseBccTransfer.
 */
[[nodiscard]] constexpr bool settle_pairwise_bcc_transfer(
    PairwiseBccChannel& ch,
    uint64_t transfer_uj) noexcept {
    return SettlePairwiseBccTransfer(ch, transfer_uj);
}

/**
 * @brief Constitutional Non-Commensurability Invariant (Chapter 4 §4.4 / Scenario Sigma & Delta).
 *
 * Layer 3 (`col-storaged`) structurally refuses to mint tokens, open Dutch auctions,
 * or charge BCC exergy credits for R0 baseline human care (Humble Hearth pea soup &
 * bean stew, warm shower, indoor heating, sanitation, childcare, or emergency health care).
 * Unconditionally returns false.
 */
[[nodiscard]] constexpr bool AttemptTokenizeBaselineCare(uint16_t care_category = 0) noexcept {
    (void)care_category;
    return false;
}

[[nodiscard]] constexpr bool AttemptTokenizeBaselineCare(BaselineCareCategory care_category) noexcept {
    return AttemptTokenizeBaselineCare(static_cast<uint16_t>(care_category));
}

[[nodiscard]] constexpr bool attempt_tokenize_baseline_care(uint16_t care_category = 0) noexcept {
    return AttemptTokenizeBaselineCare(care_category);
}

/**
 * @brief Verify a MerkleArtifactRecord meets 3-node reproducible consensus before activation.
 */
[[nodiscard]] constexpr bool VerifyMerkleArtifactConsensus(MerkleArtifactRecord& rec) noexcept {
    if (rec.artifact_cid_hash == 0 ||
        rec.sbom_merkle_root_hash == 0 ||
        rec.agreed_sha256_prefix == 0 ||
        rec.builder_consensus_count < 3U) {
        rec.is_verified_active = 0;
        return false;
    }
    rec.is_verified_active = 1;
    return true;
}

/**
 * @brief Deterministic Layer 3 Content-Addressed Merkle DAG & Pairwise BCC Storage Engine.
 */
class StoragedEngine {
public:
    StoragedEngine() = default;

    bool CommitMerkleArtifact(MerkleArtifactRecord rec) {
        if (!VerifyMerkleArtifactConsensus(rec)) {
            return false;
        }
        artifacts_[rec.artifact_cid_hash] = rec;
        return true;
    }

    [[nodiscard]] const MerkleArtifactRecord* FindArtifact(uint64_t artifact_cid_hash) const noexcept {
        auto it = artifacts_.find(artifact_cid_hash);
        if (it == artifacts_.end()) {
            return nullptr;
        }
        return &it->second;
    }

    [[nodiscard]] size_t GetArtifactCount() const noexcept {
        return artifacts_.size();
    }

private:
    std::unordered_map<uint64_t, MerkleArtifactRecord> artifacts_{};
};

} // namespace oasis::daemons::storaged

namespace collective::l3 {
using namespace oasis::daemons::storaged;
} // namespace collective::l3
