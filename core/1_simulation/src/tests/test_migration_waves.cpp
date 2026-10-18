#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "uhai/uhai_ring_buffer.hpp"
#include "adversary_engine.hpp"
#include "kms_assimilation.hpp"
#include "core/migration_assimilation.hpp"

using Catch::Approx;
using namespace oasis::uhai;
using namespace collective::l7;
using namespace collective::l5;
using namespace oasis::migration;

// ============================================================================
// SS-EP7-007 Tier 1: UHAI Binary Layout & Alignment Verification
// ============================================================================

TEST_CASE("SS-EP7-007: MigrationWaveArrivalEvent & MigrationWaveSlot Binary Layout", "[ss-ep7-007][layout]") {
    REQUIRE(sizeof(MigrationWaveArrivalEvent) == 32);
    REQUIRE(alignof(MigrationWaveArrivalEvent) == 8);
    REQUIRE(std::is_standard_layout_v<MigrationWaveArrivalEvent>);
    REQUIRE(std::is_trivially_copyable_v<MigrationWaveArrivalEvent>);

    REQUIRE(offsetof(MigrationWaveArrivalEvent, timestamp_hlc) == 0);
    REQUIRE(offsetof(MigrationWaveArrivalEvent, wave_id) == 8);
    REQUIRE(offsetof(MigrationWaveArrivalEvent, deterministic_seed) == 12);
    REQUIRE(offsetof(MigrationWaveArrivalEvent, legacy_distress_index) == 16);
    REQUIRE(offsetof(MigrationWaveArrivalEvent, initial_stress_mean) == 20);
    REQUIRE(offsetof(MigrationWaveArrivalEvent, caloric_deficit_kcal) == 24);
    REQUIRE(offsetof(MigrationWaveArrivalEvent, cohort_size) == 28);
    REQUIRE(offsetof(MigrationWaveArrivalEvent, initial_trust_ring) == 30);
    REQUIRE(offsetof(MigrationWaveArrivalEvent, trigger_reason_code) == 31);

    REQUIRE(sizeof(MigrationWaveSlot) == 64);
    REQUIRE(alignof(MigrationWaveSlot) == 64);
    REQUIRE(std::is_standard_layout_v<MigrationWaveSlot>);
    REQUIRE(std::is_trivially_copyable_v<MigrationWaveSlot>);
    REQUIRE(TOTAL_MIGRATION_WAVE_SHM_SIZE == 65728);
}

// ============================================================================
// SS-EP7-007 Tier 2: LegacyDistressIndex & Deterministic DES Scheduling
// ============================================================================

TEST_CASE("SS-EP7-007: LegacyDistressIndex & Deterministic DES Migration Wave Scheduling", "[ss-ep7-007][des]") {
    SECTION("LegacyDistressIndex scales monotonically with ThreatLevel and sanitizes NaN/Inf") {
        const float d_benign   = LegacyDistressIndex::Compute(ThreatLevel::LEVEL_0_BENIGN, 1337ULL, 10.0);
        const float d_standard = LegacyDistressIndex::Compute(ThreatLevel::LEVEL_1_STANDARD, 1337ULL, 10.0);
        const float d_hostile  = LegacyDistressIndex::Compute(ThreatLevel::LEVEL_2_HOSTILE, 1337ULL, 10.0);
        const float d_siege    = LegacyDistressIndex::Compute(ThreatLevel::LEVEL_3_SIEGE, 1337ULL, 10.0);

        REQUIRE(d_benign < MIGRATION_DISTRESS_THRESHOLD);
        REQUIRE(d_benign < d_standard);
        REQUIRE(d_standard <= d_hostile);
        REQUIRE(d_hostile <= d_siege);
        REQUIRE(d_siege >= MIGRATION_DISTRESS_THRESHOLD);

        // NaN/Inf sanitization
        const float nan_val = std::numeric_limits<float>::quiet_NaN();
        const float inf_val = std::numeric_limits<float>::infinity();
        const auto sanitized = LegacyDistressIndex::FromFactors(nan_val, inf_val, ThreatLevel::LEVEL_2_HOSTILE, -5.0f);
        REQUIRE_FALSE(std::isnan(sanitized.value));
        REQUIRE_FALSE(std::isinf(sanitized.value));
        REQUIRE(sanitized.value >= 0.0f);
        REQUIRE(sanitized.value <= 1.0f);
    }

    SECTION("AdversaryDesEngine produces bit-exact deterministic MigrationWaveArrivalEvents with Ring R6 and cohort [1..12]") {
        AdversaryConfig cfg_a = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 9001ULL, 90);
        AdversaryConfig cfg_b = AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_3_SIEGE, 9001ULL, 90);

        AdversaryDesEngine engine_a(cfg_a);
        AdversaryDesEngine engine_b(cfg_b);

        engine_a.AdvanceDays(90.0);
        engine_b.AdvanceDays(90.0);

        const auto& waves_a = engine_a.GetEmittedMigrationWaves();
        const auto& waves_b = engine_b.GetEmittedMigrationWaves();

        REQUIRE_FALSE(waves_a.empty());
        REQUIRE(waves_a.size() == waves_b.size());

        for (size_t i = 0; i < waves_a.size(); ++i) {
            REQUIRE(std::memcmp(&waves_a[i], &waves_b[i], sizeof(MigrationWaveArrivalEvent)) == 0);
            REQUIRE(waves_a[i].initial_trust_ring == 6); // Ring R6: Unverified Migrant
            REQUIRE(waves_a[i].cohort_size >= 1);
            REQUIRE(waves_a[i].cohort_size <= 12);
            REQUIRE(waves_a[i].legacy_distress_index >= MIGRATION_DISTRESS_THRESHOLD);
            REQUIRE(waves_a[i].initial_stress_mean >= 0.50f);
            REQUIRE(waves_a[i].initial_stress_mean <= 0.85f);
            REQUIRE(waves_a[i].trigger_reason_code >= 1);
            REQUIRE(waves_a[i].trigger_reason_code <= 3);
        }

        // Benign threat level emits zero migration waves
        AdversaryDesEngine engine_benign(AdversaryConfig::CreateDefault(ThreatLevel::LEVEL_0_BENIGN, 9001ULL, 90));
        engine_benign.AdvanceDays(90.0);
        REQUIRE(engine_benign.GetEmittedMigrationWaves().empty());
    }
}

// ============================================================================
// SS-EP7-007 Tier 3: Strict Layer Adjacency (L7 -> L6 -> L5 -> L4 -> L3 -> L2 -> UHAI)
// ============================================================================

TEST_CASE("SS-EP7-007: PropagateMigrationWaveInward Strict Layer Adjacency & Airgap Isolation", "[ss-ep7-007][adjacency]") {
    UhaiMigrationWaveChannel channel(UhaiMigrationWaveChannel::ChannelMode::VirtualInMemory, "/test_migration_waves_s12");
    REQUIRE(channel.IsValid());

    LegacyDistressIndex distress = LegacyDistressIndex::FromFactors(0.90f, 0.85f, ThreatLevel::LEVEL_3_SIEGE, 0.75f);
    MigrationWaveArrivalEvent wave = BuildMigrationWaveEvent(1u, 4242ULL, 14.0, distress, ThreatLevel::LEVEL_3_SIEGE);

    SECTION("Blocks direct L7 write to /uhai_telemetry or bus.sock (Anti-Bleed & Airgap Isolation)") {
        auto blocked_uhai = PropagateMigrationWaveInward(wave, &channel, "/uhai_telemetry");
        REQUIRE_FALSE(blocked_uhai.success);
        REQUIRE(blocked_uhai.direct_uhai_telemetry_blocked);
        REQUIRE(channel.GetRing()->IsEmpty());

        auto blocked_bus = PropagateMigrationWaveInward(wave, &channel, "/var/run/collective/bus.sock");
        REQUIRE_FALSE(blocked_bus.success);
        REQUIRE(blocked_bus.direct_bus_sock_blocked);
        REQUIRE(channel.GetRing()->IsEmpty());
    }

    SECTION("Routes valid MigrationWaveArrivalEvent inward L7->L6->L5->L4->L3->L2->UhaiMigrationWaveChannel with R6 tag") {
        // Attempt to spoof initial_trust_ring = 0 (R0 Steward) -> must be sanitized back to 6 (R6 Unverified Migrant)
        MigrationWaveArrivalEvent untrusted_wave = wave;
        untrusted_wave.initial_trust_ring = 0;
        untrusted_wave.cohort_size = 999; // Exceeds max 12

        auto prop = PropagateMigrationWaveInward(untrusted_wave, channel);
        REQUIRE(prop.success);
        REQUIRE(prop.enforced_trust_ring == 6);
        REQUIRE(prop.quality_flag == QUALITY_SIMULATED);

        // Verify all 5 inward hops (L7->L6, L6->L5, L5->L4, L4->L3, L3->L2)
        REQUIRE(prop.hops.size() == 5);
        REQUIRE(std::string_view(prop.hops[0].socket_name) == "l6_l7.sock");
        REQUIRE(prop.hops[0].from_layer == 7);
        REQUIRE(prop.hops[0].to_layer == 6);
        REQUIRE(prop.hops[0].verified);

        REQUIRE(std::string_view(prop.hops[1].socket_name) == "l5_l6.sock");
        REQUIRE(prop.hops[1].from_layer == 6);
        REQUIRE(prop.hops[1].to_layer == 5);
        REQUIRE(prop.hops[1].verified);

        REQUIRE(std::string_view(prop.hops[4].socket_name) == "l2_l3.sock");
        REQUIRE(prop.hops[4].from_layer == 3);
        REQUIRE(prop.hops[4].to_layer == 2);
        REQUIRE(prop.hops[4].verified);

        // Pop from UhaiMigrationWaveChannel and verify sanitized fields
        MigrationWaveArrivalEvent popped{};
        REQUIRE(channel.Pop(popped));
        REQUIRE(popped.wave_id == 1u);
        REQUIRE(popped.initial_trust_ring == 6); // Strictly R6
        REQUIRE(popped.cohort_size == 12);       // Clamped to MAX_MIGRATION_COHORT_SIZE
        REQUIRE(popped.legacy_distress_index >= MIGRATION_DISTRESS_THRESHOLD);
    }
}

// ============================================================================
// SS-EP5-007: TrustRingPromotionEvent Layout & KmsAssimilationEngine State Machine
// ============================================================================

TEST_CASE("SS-EP5-007: TrustRingPromotionEvent Binary Layout & KmsAssimilationEngine (R6 -> R3 -> R1)", "[ss-ep5-007][kms]") {
    SECTION("TrustRingPromotionEvent & TrustRingPromotionSlot binary layout and cache-line alignment") {
        REQUIRE(sizeof(TrustRingPromotionEvent) == 32);
        REQUIRE(alignof(TrustRingPromotionEvent) == 8);
        REQUIRE(std::is_standard_layout_v<TrustRingPromotionEvent>);
        REQUIRE(std::is_trivially_copyable_v<TrustRingPromotionEvent>);

        REQUIRE(offsetof(TrustRingPromotionEvent, timestamp_hlc) == 0);
        REQUIRE(offsetof(TrustRingPromotionEvent, migrant_did_hash) == 8);
        REQUIRE(offsetof(TrustRingPromotionEvent, verified_sweat_equity_mj) == 16);
        REQUIRE(offsetof(TrustRingPromotionEvent, sheltered_days) == 20);
        REQUIRE(offsetof(TrustRingPromotionEvent, from_ring) == 22);
        REQUIRE(offsetof(TrustRingPromotionEvent, to_ring) == 23);
        REQUIRE(offsetof(TrustRingPromotionEvent, frost_quorum_signers) == 24);
        REQUIRE(offsetof(TrustRingPromotionEvent, quality_flag) == 25);
        REQUIRE(offsetof(TrustRingPromotionEvent, regression_frozen) == 26);
        REQUIRE(offsetof(TrustRingPromotionEvent, has_bt_survival_floor) == 27);
        REQUIRE(offsetof(TrustRingPromotionEvent, reserved_pad) == 28);

        REQUIRE(sizeof(TrustRingPromotionSlot) == 64);
        REQUIRE(alignof(TrustRingPromotionSlot) == 64);
        REQUIRE(std::is_standard_layout_v<TrustRingPromotionSlot>);
        REQUIRE(std::is_trivially_copyable_v<TrustRingPromotionSlot>);
    }

    SECTION("Stage 1->2->3 Assimilation Pipeline, Anti-Passive-Promotion Guard, Regression Freeze & FROST Quorum") {
        UhaiTrustPromotionChannel promo_channel(
            UhaiTrustPromotionChannel::ChannelMode::VirtualInMemory,
            "/test_trust_promotions_s13");
        REQUIRE(promo_channel.IsValid());

        KmsAssimilationEngine kms;
        MigrationWaveArrivalEvent wave{};
        wave.wave_id = 42;
        wave.timestamp_hlc = 5000ULL;
        wave.cohort_size = 3;
        wave.legacy_distress_index = 0.80f;
        wave.initial_trust_ring = 6;

        // 1. Stage 1: RegisterMigrantCohortFromWave initializes at Ring R6 with B_t floor and 0 credit/vote
        auto cohort = kms.RegisterMigrantCohortFromWave(wave);
        REQUIRE(cohort.size() == 3);
        for (const auto& m : cohort) {
            REQUIRE(m.current_ring == 6);
            REQUIRE(m.bcc_credit_limit_ec == Approx(0.0));
            REQUIRE(m.voting_weight == Approx(0.0));
            REQUIRE(m.has_bt_survival_floor);
            REQUIRE_FALSE(m.bbs_plus_steward_credential_issued);
        }

        const uint64_t did_hash = cohort[0].migrant_did_hash;

        // 2. Anti-Passive-Promotion Invariant: 14 sheltered days with 0.0 MJ (or 35.9 MJ) passive rest stays at R6
        REQUIRE_FALSE(kms.RecordLayer3HomeostasisAndPoTWAttestation(did_hash, 14, 0.0f, 5100ULL, &promo_channel));
        REQUIRE_FALSE(kms.RecordLayer3HomeostasisAndPoTWAttestation(did_hash, 14, 35.9f, 5101ULL, &promo_channel));
        REQUIRE(kms.GetMigrantState(did_hash)->current_ring == 6);
        REQUIRE(promo_channel.GetRing()->IsEmpty());

        // Also 6 sheltered days with 50.0 MJ stays at R6
        REQUIRE_FALSE(kms.RecordLayer3HomeostasisAndPoTWAttestation(did_hash, 6, 50.0f, 5102ULL, &promo_channel));
        REQUIRE(kms.GetMigrantState(did_hash)->current_ring == 6);

        // 3. Behavioral Regression / Conflict freezes R6 -> R3 promotion while strictly preserving B_t survival floor
        REQUIRE(kms.RecordBehavioralRegressionOrConflict(did_hash, true));
        auto frozen_state = kms.GetMigrantState(did_hash);
        REQUIRE(frozen_state.has_value());
        REQUIRE(frozen_state->regression_frozen);
        REQUIRE(frozen_state->restorative_triage_emitted);
        REQUIRE(frozen_state->has_bt_survival_floor); // Inviolable humanitarian floor B_t preserved!
        REQUIRE_FALSE(kms.RecordLayer3HomeostasisAndPoTWAttestation(did_hash, 7, 36.0f, 5103ULL, &promo_channel));
        REQUIRE(kms.GetMigrantState(did_hash)->current_ring == 6);

        // Clear via restorative justice mediation
        REQUIRE(kms.ResolveRestorativeMediation(did_hash));
        REQUIRE_FALSE(kms.GetMigrantState(did_hash)->regression_frozen);

        // 4. Stage 2 (R6 -> R3): sheltered_days >= 7 AND sweat_equity_mj >= 36.0f promotes to R3
        REQUIRE(kms.RecordLayer3HomeostasisAndPoTWAttestation(did_hash, 7, 36.0f, 5200ULL, &promo_channel));
        auto r3_state = kms.GetMigrantState(did_hash);
        REQUIRE(r3_state.has_value());
        REQUIRE(r3_state->current_ring == 3);
        REQUIRE(r3_state->bcc_credit_limit_ec == Approx(25.0));
        REQUIRE(r3_state->voting_weight == Approx(0.0));
        REQUIRE(r3_state->has_bt_survival_floor);

        TrustRingPromotionEvent popped_r3{};
        REQUIRE(promo_channel.Pop(popped_r3));
        REQUIRE(popped_r3.migrant_did_hash == did_hash);
        REQUIRE(popped_r3.from_ring == 6);
        REQUIRE(popped_r3.to_ring == 3);
        REQUIRE(popped_r3.sheltered_days == 7);
        REQUIRE(popped_r3.verified_sweat_equity_mj == Approx(36.0f));
        REQUIRE(popped_r3.quality_flag == 0x01);
        REQUIRE(popped_r3.has_bt_survival_floor == 1);

        // 5. Stage 3 (R3 -> R1): ExecuteFrostStewardVouchCeremony validation
        // Reject < 30 days in R3
        REQUIRE_FALSE(kms.ExecuteFrostStewardVouchCeremony(
            did_hash, 29, {"did:peer:steward:1", "did:peer:steward:2"}, 5300ULL, &promo_channel));
        // Reject < 2 distinct R1 signers (single signer or duplicate signer)
        REQUIRE_FALSE(kms.ExecuteFrostStewardVouchCeremony(
            did_hash, 30, {"did:peer:steward:1"}, 5301ULL, &promo_channel));
        REQUIRE_FALSE(kms.ExecuteFrostStewardVouchCeremony(
            did_hash, 30, {"did:peer:steward:1", "did:peer:steward:1"}, 5302ULL, &promo_channel));
        REQUIRE(kms.GetMigrantState(did_hash)->current_ring == 3);

        // Promote R3 -> R1 upon valid 2-of-3 FROST quorum after 30 days
        REQUIRE(kms.ExecuteFrostStewardVouchCeremony(
            did_hash, 30, {"did:peer:steward:1", "did:peer:steward:2"}, 5400ULL, &promo_channel));
        auto r1_state = kms.GetMigrantState(did_hash);
        REQUIRE(r1_state.has_value());
        REQUIRE(r1_state->current_ring == 1);
        REQUIRE(r1_state->voting_weight == Approx(1.0));
        REQUIRE(r1_state->bcc_credit_limit_ec == Approx(100.0));
        REQUIRE(r1_state->bbs_plus_steward_credential_issued);
        REQUIRE(r1_state->has_bt_survival_floor);

        TrustRingPromotionEvent popped_r1{};
        REQUIRE(promo_channel.Pop(popped_r1));
        REQUIRE(popped_r1.from_ring == 3);
        REQUIRE(popped_r1.to_ring == 1);
        REQUIRE(popped_r1.frost_quorum_signers == 2);
        REQUIRE(popped_r1.quality_flag == 0x01);
    }
}

// ============================================================================
// STORY-11.1: Oasis Wave Ingress, Emergency Warp Brake & MAX_PARCEL_NPCS = 16 Cap
// ============================================================================

TEST_CASE("STORY-11.1: SpawnMigrationWaveInOasis Emergency Warp Brake & MAX_PARCEL_NPCS = 16 Cap", "[story-11.1][oasis-ingress]") {
    oasis::graphics::GetDynamicVoxelStore().clear();
    flecs::world ecs;
    MigrationStagingQueue staging{};
    oasis::time::TimeWarpState warp{};
    oasis::time::SetTimeWarpMultiplier(warp, 10);
    REQUIRE(warp.multiplier == 10);

    MigrationWaveArrivalEvent wave{};
    wave.wave_id = 1;
    wave.deterministic_seed = 777;
    wave.legacy_distress_index = 0.78f;
    wave.initial_stress_mean = 0.60f;
    wave.caloric_deficit_kcal = 500.0f;
    wave.cohort_size = 8;
    wave.initial_trust_ring = 6;

    // Wave 1 (8 migrants): immediately engages Emergency Warp Brake (100x -> 1x) and spawns 8 R6 NPCs
    const uint32_t spawned_1 = SpawnMigrationWaveInOasis(ecs, wave, staging, warp);
    REQUIRE(warp.multiplier == 1);
    REQUIRE(warp.emergency_brake_triggered);
    REQUIRE(spawned_1 == 8u);
    REQUIRE(CountActiveMigrantNpcs(ecs) == 8u);
    REQUIRE(staging.staged_overflow_migrants == 0u);

    // Wave 2 (8 migrants): reaches MAX_PARCEL_NPCS = 16
    wave.wave_id = 2;
    const uint32_t spawned_2 = SpawnMigrationWaveInOasis(ecs, wave, staging, warp);
    REQUIRE(spawned_2 == 8u);
    REQUIRE(CountActiveMigrantNpcs(ecs) == MAX_PARCEL_NPCS);
    REQUIRE(staging.staged_overflow_migrants == 0u);

    // Wave 3 (8 migrants): all 8 overflow into MigrationStagingQueue without exceeding MAX_PARCEL_NPCS = 16
    wave.wave_id = 3;
    const uint32_t spawned_3 = SpawnMigrationWaveInOasis(ecs, wave, staging, warp);
    REQUIRE(spawned_3 == 0u);
    REQUIRE(CountActiveMigrantNpcs(ecs) == MAX_PARCEL_NPCS);
    REQUIRE(staging.staged_overflow_migrants == 8u);
    REQUIRE(staging.staged_waves.size() == 1u);
    REQUIRE(staging.staged_waves[0].cohort_size == 8u);

    auto boundary = ecs.entity("SettlementBoundary");
    REQUIRE(boundary.is_alive());
    REQUIRE(boundary.get<oasis::boundary::SocialMutualAidComponent>().count == MAX_PARCEL_NPCS);
}

// ============================================================================
// STORY-11.2: Legacy Mindset Friction, 4-Needs Checklist & >= 75% Stress Regressions
// ============================================================================

TEST_CASE("STORY-11.2: StepLegacyFrictionSystem, 4-Needs Checklist & >= 75% Stress Regressions", "[story-11.2][friction]") {
    oasis::graphics::GetDynamicVoxelStore().clear();
    flecs::world ecs;

    auto camp_ent = oasis::chill::BootstrapGenesisSurvivalCamp(ecs);
    auto& camp_stock = camp_ent.get_mut<oasis::chill::GenesisCampStockComponent>();
    REQUIRE(camp_stock.ration_meals_remaining == 12u);

    auto boundary = ecs.entity("SettlementBoundary")
        .set<oasis::boundary::SocialMutualAidComponent>({})
        .set<oasis::boundary::InfrastructureAutarkyComponent>({20.0, 0.0, 20.0, 100.0, 0.0, 100.0});

    auto founder = ecs.entity("Founder")
        .set<oasis::psychology::PsychologyComponent>({})
        .set<oasis::fabrication::SpatialPositionComponent>({glm::vec3(4.0f, 1.0f, 13.0f), glm::vec3(0.0f), 3.0f});
    founder.get_mut<oasis::psychology::PsychologyComponent>().stress = 0.20f;

    // Spawn 1 unassimilated R6 migrant with unmet shelter (10.0 C) and low calories (1100 kcal)
    LegacyMigrantComponent mig{};
    mig.npc_index = 0;
    mig.wave_id = 1;
    mig.trust_ring_tier = 6u;
    mig.mindset_flags = SCARCITY_ANXIETY;

    oasis::fabrication::SomaticStateComponent som{};
    som.caloric_reserve = 1100.0; // < 1400.0 hungry threshold & < 1500.0 checklist threshold
    som.hydration = 0.60f;
    som.ambient_temp_c = 10.0f;   // < 18.0 C shelter unmet

    oasis::psychology::PsychologyComponent psy{};
    psy.stress = 0.65f;

    oasis::fabrication::SpatialPositionComponent pos{};
    pos.pos = glm::vec3(3.0f, 1.0f, 13.0f); // Within 4.0m of Founder

    auto npc = ecs.entity("Migrant0")
        .set<LegacyMigrantComponent>(mig)
        .set<oasis::fabrication::SomaticStateComponent>(som)
        .set<oasis::psychology::PsychologyComponent>(psy)
        .set<oasis::fabrication::SpatialPositionComponent>(pos);

    // Step 1.0 simulated hour: stress escalates 0.65 + 0.12 = 0.77 (>= 0.75 threshold!)
    StepLegacyFrictionSystem(ecs, 1.0f);

    const auto& mig_after = npc.get<LegacyMigrantComponent>();
    const auto& psy_after = npc.get<oasis::psychology::PsychologyComponent>();
    const auto& som_after = npc.get<oasis::fabrication::SomaticStateComponent>();

    REQUIRE(psy_after.stress == Approx(0.77f));
    REQUIRE_FALSE(mig_after.checklist.enclosed_shelter_ok);
    REQUIRE(mig_after.checklist.hydration_ok);
    REQUIRE((mig_after.mindset_flags & HOARDING_ACTIVE) != 0u);
    REQUIRE((mig_after.mindset_flags & CONFLICT_REGRESSION) != 0u);
    REQUIRE((mig_after.mindset_flags & RESERVE_POWER_DRAW) != 0u);

    // 1. Ration Hoarding: withdrew 3x meals (12 -> 9 meals)
    REQUIRE(camp_stock.ration_meals_remaining == 9u);
    REQUIRE(som_after.caloric_reserve > 1500.0);

    // 2. Tool Conflict + Scarcity Anxiety radiation increased Founder stress (0.20 + 0.05 + 0.15 = 0.40)
    REQUIRE(founder.get<oasis::psychology::PsychologyComponent>().stress == Approx(0.40f));

    // 3. Reserve Power Draw drained 2.5 kWh from solar_stored_kwh (20.0 -> 17.5 kWh)
    REQUIRE(boundary.get<oasis::boundary::InfrastructureAutarkyComponent>().solar_stored_kwh == Approx(17.5));
}

// ============================================================================
// STORY-11.3: Oasis Two-Stage Trust Ring Promotion (R6 -> R3 -> R1) & Voxel Stamping
// ============================================================================

TEST_CASE("STORY-11.3: Oasis Two-Stage Trust Ring Promotion (R6->R3->R1) & Avatar Voxel Stamping (27u->28u->29u)", "[story-11.3][avatar-voxels]") {
    oasis::graphics::GetDynamicVoxelStore().clear();
    flecs::world ecs;

    LegacyMigrantComponent mig{};
    mig.npc_index = 0;
    mig.trust_ring_tier = 6u;
    mig.mindset_flags = SCARCITY_ANXIETY;
    mig.sheltered_days_consecutive = 7u;
    mig.potw_sweat_equity_mj = 36.0f;

    oasis::psychology::PsychologyComponent psy{};
    psy.stress = 0.50f;
    psy.mood = 0.40f;

    oasis::fabrication::SpatialPositionComponent pos{};
    pos.pos = glm::vec3(5.0f, 1.0f, 14.0f);

    auto npc = ecs.entity("MigrantPromo")
        .set<LegacyMigrantComponent>(mig)
        .set<oasis::psychology::PsychologyComponent>(psy)
        .set<oasis::fabrication::SpatialPositionComponent>(pos);

    // Initial R6 voxel stamp (MAT_MIGRANT_R6 = 27u)
    SyncMigrantAvatarVoxels(ecs);
    REQUIRE(oasis::graphics::GetDynamicVoxel(5, 1, 14) == MAT_MIGRANT_R6);

    // Promote R6 -> R3
    auto& mig_mut = npc.get_mut<LegacyMigrantComponent>();
    auto& psy_mut = npc.get_mut<oasis::psychology::PsychologyComponent>();
    REQUIRE(TryPromoteMigrantR6ToR3(mig_mut, psy_mut));
    REQUIRE(mig_mut.trust_ring_tier == 3u);
    REQUIRE(mig_mut.mindset_flags == MINDSET_NONE);

    SyncMigrantAvatarVoxels(ecs);
    REQUIRE(oasis::graphics::GetDynamicVoxel(5, 1, 14) == MAT_RESIDENT_R3);

    // Promote R3 -> R1 (requires >= 30 days in R3 and >= 2 FROST signers)
    mig_mut.days_in_r3_consecutive = 30u;
    REQUIRE(TryPromoteMigrantR3ToR1(mig_mut, psy_mut, 2u));
    REQUIRE(mig_mut.trust_ring_tier == 1u);
    REQUIRE(mig_mut.mindset_flags == MINDSET_NONE);
    REQUIRE(psy_mut.mood == Approx(0.95f));

    SyncMigrantAvatarVoxels(ecs);
    REQUIRE(oasis::graphics::GetDynamicVoxel(5, 1, 14) == MAT_STEWARD_R1);
}
