#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "../../../core/1_simulation/src/uhai/uhai_ring_buffer.hpp"

namespace collective::l5 {

/**
 * @brief Progressive Trust Ring (R6 -> R3 -> R1) Assimilation State per Migrant DID (SS-EP5-007 / EPIC-11).
 *
 * Explicitly ordered for 8-byte alignment cleanliness under strict -Wpadded.
 */
struct MigrantAssimilationState {
    std::string migrant_pairwise_did{};
    std::vector<std::string> vouching_steward_dids{};
    uint64_t migrant_did_hash{0};
    uint64_t wave_id{0};
    uint64_t last_transition_hlc{0};
    double bcc_credit_limit_ec{0.0};
    double voting_weight{0.0};
    float verified_sweat_equity_mj{0.0f};
    uint32_t unresolved_mediations{0};
    uint16_t sheltered_days_consecutive{0};
    uint16_t days_in_r3_consecutive{0};
    uint8_t current_ring{static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R6_UNVERIFIED_MIGRANT)};
    bool regression_frozen{false};
    bool restorative_triage_emitted{false};
    bool has_bt_survival_floor{true}; // Inviolable humanitarian floor B_t (NEVER revoked)
    bool bbs_plus_steward_credential_issued{false};
    [[maybe_unused]] uint8_t pad_tail_[7]{0};
};

/**
 * @brief Layer 5 KMS & Trust Ring Assimilation Engine (`col-kmsd`).
 *
 * Enforces the 3-tier thermodynamic & cryptographic assimilation pipeline:
 *  - Stage 1 (R6: Unverified Migrant): Initial cohort onboarding with inviolable B_t survival floor,
 *    zero BCC credit limit, and zero L5 voting weight.
 *  - Stage 2 (R6 -> R3: Probationary Resident): Requires >= 7 consecutive sheltered days (L3 homeostasis)
 *    AND >= 36.0 MJ (10.0 Exergy Credits) verified PoTW sweat equity (L4 ledger). Passive rest never promotes.
 *    Behavioral regression freezes R6 -> R3 promotion and emits L5 restorative triage while preserving B_t.
 *  - Stage 3 (R3 -> R1: Vouched Steward): Requires >= 30 consecutive days in R3, zero unresolved conflict
 *    mediations, and a 2-of-3 FROST threshold signing ceremony (SCN-TAU-GENESIS) by existing R1 Stewards.
 */
class KmsAssimilationEngine {
public:
    static constexpr uint16_t REQUIRED_SHELTERED_DAYS_FOR_R3 = 7;
    static constexpr float REQUIRED_SWEAT_EQUITY_MJ_FOR_R3 = 36.0f; // 10.0 Exergy Credits
    static constexpr double REQUIRED_EXERGY_CREDITS_FOR_R3 = 10.0;
    static constexpr uint16_t REQUIRED_R3_DAYS_FOR_R1 = 30;
    static constexpr size_t REQUIRED_FROST_QUORUM_SIGNERS = 2; // 2-of-3 FROST threshold ceremony
    static constexpr double R6_BCC_CREDIT_LIMIT_EC = 0.0;
    static constexpr double R3_BCC_CREDIT_LIMIT_EC = 25.0;
    static constexpr double R1_BCC_CREDIT_LIMIT_EC = 100.0;

    KmsAssimilationEngine() = default;

    [[nodiscard]] static constexpr uint64_t ComputeDidHash(std::string_view did) noexcept {
        uint64_t hash = 14695981039346656037ULL;
        for (char c : did) {
            hash ^= static_cast<uint8_t>(c);
            hash *= 1099511628211ULL;
        }
        return hash == 0 ? 1ULL : hash;
    }

    MigrantAssimilationState& RegisterMigrant(
        uint64_t migrant_did_hash,
        std::string_view pairwise_did = "",
        uint64_t wave_id = 0,
        uint64_t timestamp_hlc = 0) {
        MigrantAssimilationState state{};
        state.migrant_did_hash = migrant_did_hash;
        state.migrant_pairwise_did = pairwise_did.empty()
            ? ("did:peer:migrant:" + std::to_string(migrant_did_hash))
            : std::string(pairwise_did);
        state.wave_id = wave_id;
        state.last_transition_hlc = timestamp_hlc;
        state.current_ring = static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R6_UNVERIFIED_MIGRANT);
        state.bcc_credit_limit_ec = R6_BCC_CREDIT_LIMIT_EC;
        state.voting_weight = 0.0;
        state.has_bt_survival_floor = true;
        state.bbs_plus_steward_credential_issued = false;

        migrants_[migrant_did_hash] = state;
        return migrants_[migrant_did_hash];
    }

    /**
     * @brief Stage 1: Onboard an incoming migrant cohort into Ring R6 (Unverified Migrant).
     *
     * Regardless of external wave metadata, every newly arrived migrant is strictly onboarded
     * at Ring R6 with an inviolable B_t survival floor, 0.0 BCC credit limit, and 0.0 voting weight.
     */
    std::vector<MigrantAssimilationState> RegisterMigrantCohortFromWave(
        const oasis::uhai::MigrationWaveArrivalEvent& wave) {
        std::vector<MigrantAssimilationState> cohort;
        const uint16_t count = wave.cohort_size;
        cohort.reserve(count);

        for (uint16_t i = 0; i < count; ++i) {
            std::string did = "did:peer:migrant:wave-" + std::to_string(wave.wave_id) +
                              "-member-" + std::to_string(i);
            const uint64_t did_hash = ComputeDidHash(did);
            auto& state = RegisterMigrant(did_hash, did, wave.wave_id, wave.timestamp_hlc);
            cohort.push_back(state);
        }
        return cohort;
    }

    /**
     * @brief Stage 2: Evaluate L3 Homeostasis (>= 7 sheltered days) + L4 PoTW Sweat Equity (>= 36.0 MJ).
     *
     * Passive rest (sweat_equity_mj < 36.0f) NEVER triggers R6 -> R3 promotion even if sheltered_days >= 7.
     * Behavioral regression freeze blocks promotion until restorative mediation is resolved.
     */
    bool RecordLayer3HomeostasisAndPoTWAttestation(
        uint64_t migrant_did_hash,
        uint16_t sheltered_days,
        float sweat_equity_mj,
        uint64_t timestamp_hlc = 0,
        oasis::uhai::UhaiTrustPromotionChannel* out_channel = nullptr) {
        auto it = migrants_.find(migrant_did_hash);
        if (it == migrants_.end()) {
            RegisterMigrant(migrant_did_hash, "", 0, timestamp_hlc);
            it = migrants_.find(migrant_did_hash);
        }

        MigrantAssimilationState& state = it->second;
        state.has_bt_survival_floor = true; // Inviolable humanitarian floor B_t
        state.sheltered_days_consecutive = sheltered_days;
        state.verified_sweat_equity_mj = sweat_equity_mj;

        if (state.current_ring != static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R6_UNVERIFIED_MIGRANT)) {
            return false;
        }

        if (state.regression_frozen) {
            return false;
        }

        // Strict Anti-Passive-Promotion Guard: BOTH homeostasis duration AND verified PoTW work are required
        if (state.sheltered_days_consecutive < REQUIRED_SHELTERED_DAYS_FOR_R3 ||
            state.verified_sweat_equity_mj < REQUIRED_SWEAT_EQUITY_MJ_FOR_R3) {
            return false;
        }

        state.current_ring = static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R3_PROBATIONARY_RESIDENT);
        state.bcc_credit_limit_ec = R3_BCC_CREDIT_LIMIT_EC;
        state.voting_weight = 0.0;
        state.last_transition_hlc = timestamp_hlc;

        oasis::uhai::TrustRingPromotionEvent ev{};
        ev.timestamp_hlc = timestamp_hlc;
        ev.migrant_did_hash = state.migrant_did_hash;
        ev.verified_sweat_equity_mj = state.verified_sweat_equity_mj;
        ev.sheltered_days = state.sheltered_days_consecutive;
        ev.from_ring = static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R6_UNVERIFIED_MIGRANT);
        ev.to_ring = static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R3_PROBATIONARY_RESIDENT);
        ev.frost_quorum_signers = 0;
        ev.quality_flag = oasis::uhai::QUALITY_SIMULATED;
        ev.regression_frozen = state.regression_frozen ? 1 : 0;
        ev.has_bt_survival_floor = state.has_bt_survival_floor ? 1 : 0;
        ev.reserved_pad = 0;

        emitted_promotions_.push_back(ev);
        if (out_channel != nullptr) {
            out_channel->Push(ev);
        }
        return true;
    }

    /**
     * @brief Behavioral Regression / Conflict Guard: freezes R6 -> R3 promotion and emits L5 restorative triage.
     *
     * Strictly preserves the humanitarian survival floor B_t (`has_bt_survival_floor = true`).
     */
    bool RecordBehavioralRegressionOrConflict(
        uint64_t migrant_did_hash,
        bool is_conflict_mediation = true) {
        auto it = migrants_.find(migrant_did_hash);
        if (it == migrants_.end()) {
            RegisterMigrant(migrant_did_hash);
            it = migrants_.find(migrant_did_hash);
        }

        MigrantAssimilationState& state = it->second;
        state.regression_frozen = true;
        state.restorative_triage_emitted = true;
        state.has_bt_survival_floor = true; // Never revoke B_t survival floor
        if (is_conflict_mediation) {
            state.unresolved_mediations += 1;
        }
        if (state.current_ring == static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R6_UNVERIFIED_MIGRANT)) {
            state.bcc_credit_limit_ec = R6_BCC_CREDIT_LIMIT_EC;
        }
        return true;
    }

    /**
     * @brief Restorative Justice Resolution: clears regression freeze and unresolved conflict mediations.
     */
    bool ResolveRestorativeMediation(uint64_t migrant_did_hash) {
        auto it = migrants_.find(migrant_did_hash);
        if (it == migrants_.end()) {
            return false;
        }
        it->second.regression_frozen = false;
        it->second.unresolved_mediations = 0;
        it->second.has_bt_survival_floor = true;
        return true;
    }

    /**
     * @brief Stage 3: Execute 2-of-3 FROST Threshold Signing Ceremony (`SCN-TAU-GENESIS`) for R3 -> R1 promotion.
     *
     * Requires:
     *  - Migrant currently in Ring R3 (`RING_R3_PROBATIONARY_RESIDENT`)
     *  - >= 30 consecutive days in R3
     *  - Zero unresolved conflict mediations and not regression-frozen
     *  - >= 2 distinct non-empty R1 Steward signer DIDs (cannot self-vouch)
     */
    bool ExecuteFrostStewardVouchCeremony(
        uint64_t migrant_did_hash,
        uint16_t days_in_r3,
        const std::vector<std::string>& r1_steward_signer_dids,
        uint64_t timestamp_hlc = 0,
        oasis::uhai::UhaiTrustPromotionChannel* out_channel = nullptr) {
        auto it = migrants_.find(migrant_did_hash);
        if (it == migrants_.end()) {
            return false;
        }

        MigrantAssimilationState& state = it->second;
        state.has_bt_survival_floor = true;
        state.days_in_r3_consecutive = days_in_r3;

        if (state.current_ring != static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R3_PROBATIONARY_RESIDENT)) {
            return false;
        }

        if (state.regression_frozen || state.unresolved_mediations > 0) {
            return false;
        }

        if (state.days_in_r3_consecutive < REQUIRED_R3_DAYS_FOR_R1) {
            return false;
        }

        // Deduplicate non-empty R1 Steward DIDs and forbid self-vouching
        std::vector<std::string> unique_signers;
        for (const auto& signer_did : r1_steward_signer_dids) {
            if (signer_did.empty() || signer_did == state.migrant_pairwise_did) {
                continue;
            }
            if (std::find(unique_signers.begin(), unique_signers.end(), signer_did) == unique_signers.end()) {
                unique_signers.push_back(signer_did);
            }
        }

        if (unique_signers.size() < REQUIRED_FROST_QUORUM_SIGNERS) {
            return false;
        }

        state.current_ring = static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R1_VOUCHED_STEWARD);
        state.bbs_plus_steward_credential_issued = true;
        state.voting_weight = 1.0;
        state.bcc_credit_limit_ec = R1_BCC_CREDIT_LIMIT_EC;
        state.vouching_steward_dids = unique_signers;
        state.last_transition_hlc = timestamp_hlc;

        oasis::uhai::TrustRingPromotionEvent ev{};
        ev.timestamp_hlc = timestamp_hlc;
        ev.migrant_did_hash = state.migrant_did_hash;
        ev.verified_sweat_equity_mj = state.verified_sweat_equity_mj;
        ev.sheltered_days = state.sheltered_days_consecutive;
        ev.from_ring = static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R3_PROBATIONARY_RESIDENT);
        ev.to_ring = static_cast<uint8_t>(oasis::uhai::TrustRingTier::RING_R1_VOUCHED_STEWARD);
        ev.frost_quorum_signers = static_cast<uint8_t>(unique_signers.size());
        ev.quality_flag = oasis::uhai::QUALITY_SIMULATED;
        ev.regression_frozen = 0;
        ev.has_bt_survival_floor = 1;
        ev.reserved_pad = 0;

        emitted_promotions_.push_back(ev);
        if (out_channel != nullptr) {
            out_channel->Push(ev);
        }
        return true;
    }

    [[nodiscard]] std::optional<MigrantAssimilationState> GetMigrantState(uint64_t migrant_did_hash) const {
        auto it = migrants_.find(migrant_did_hash);
        if (it == migrants_.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    [[nodiscard]] const std::unordered_map<uint64_t, MigrantAssimilationState>& GetAllMigrants() const noexcept {
        return migrants_;
    }

    [[nodiscard]] const std::vector<oasis::uhai::TrustRingPromotionEvent>& GetEmittedPromotions() const noexcept {
        return emitted_promotions_;
    }

private:
    std::unordered_map<uint64_t, MigrantAssimilationState> migrants_{};
    std::vector<oasis::uhai::TrustRingPromotionEvent> emitted_promotions_{};
};

} // namespace collective::l5
