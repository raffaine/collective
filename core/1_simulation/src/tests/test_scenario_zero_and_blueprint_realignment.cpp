#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "core/artifact_registry.hpp"
#include "storaged_engine.hpp"
#include "execd_self_host.hpp"
#include "kms_assimilation.hpp"

namespace {

std::filesystem::path FindBlueprintDir() {
    const std::array<std::filesystem::path, 5> candidates = {
        "core/6_intent/blueprints",
        "../core/6_intent/blueprints",
        "../../core/6_intent/blueprints",
        "../../../core/6_intent/blueprints",
        "../../../../core/6_intent/blueprints"
    };
    for (const auto& candidate : candidates) {
        if (std::filesystem::exists(candidate) && std::filesystem::is_directory(candidate)) {
            return candidate;
        }
    }
    return "core/6_intent/blueprints";
}

std::string ReadFileToString(const std::filesystem::path& path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        return {};
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

} // namespace

// =============================================================================
// LAW 1: Scenario Zero Autopoietic Self-Hosting & col-storaged (SCN-ZERO-SOFTWARE)
// =============================================================================

TEST_CASE("Law 1.1: Scenario Zero Studio Veto (Protocol of Refusal) & Recursive SBOM DAG Gate", "[scenario_zero][law1]") {
    using namespace oasis::daemons::execd;

    SoftwareCommissionSpec valid_spec{};
    valid_spec.project_id = "indie-coop-voxel-mod-v1";
    valid_spec.category = SoftwareAssetCategory::VIDEO_GAME_OR_ENGINE;
    valid_spec.contains_surveillance_telemetry = false;
    valid_spec.contains_drm_lock_in = false;
    valid_spec.contains_predatory_dark_patterns = false;
    valid_spec.vetoed_by_studio_conscience = false;
    valid_spec.total_sbom_dependencies = 12U;
    valid_spec.vendored_and_detoxed_sbom_dependencies = 12U;

    REQUIRE(EvaluateStudioVetoAndSbomDag(valid_spec));

    // Reject DRM lock-in
    {
        auto drm_spec = valid_spec;
        drm_spec.contains_drm_lock_in = true;
        REQUIRE_FALSE(EvaluateStudioVetoAndSbomDag(drm_spec));
    }

    // Reject surveillance telemetry
    {
        auto telemetry_spec = valid_spec;
        telemetry_spec.contains_surveillance_telemetry = true;
        REQUIRE_FALSE(EvaluateStudioVetoAndSbomDag(telemetry_spec));
    }

    // Reject predatory dark patterns
    {
        auto dark_pattern_spec = valid_spec;
        dark_pattern_spec.contains_predatory_dark_patterns = true;
        REQUIRE_FALSE(EvaluateStudioVetoAndSbomDag(dark_pattern_spec));
    }

    // Reject unconditional Studio Veto (Protocol of Refusal)
    {
        auto vetoed_spec = valid_spec;
        vetoed_spec.vetoed_by_studio_conscience = true;
        REQUIRE_FALSE(EvaluateStudioVetoAndSbomDag(vetoed_spec));
    }

    // Reject unvendored SBOM dependencies (live L7 cloud tethers)
    {
        auto unvendored_spec = valid_spec;
        unvendored_spec.vendored_and_detoxed_sbom_dependencies = 11U;
        REQUIRE_FALSE(EvaluateStudioVetoAndSbomDag(unvendored_spec));
    }
}

TEST_CASE("Law 1.2: Scenario Zero 3-Node Reproducible Build, 20-Year Hardware Frugality & Oasis Canary Gate", "[scenario_zero][law1]") {
    using namespace oasis::daemons::execd;

    const std::string canonical_hash = "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08";

    SoftwareVerificationReport daemon_report{};
    daemon_report.category = SoftwareAssetCategory::CONSTITUTIONAL_STACK_DAEMON;
    daemon_report.builder_1_sha256 = canonical_hash;
    daemon_report.builder_2_sha256 = canonical_hash;
    daemon_report.builder_3_sha256 = canonical_hash;
    daemon_report.external_network_calls_during_build = 0U;
    daemon_report.idle_ram_mib_on_salvaged_laptop = 240U;
    daemon_report.max_ram_budget_mib = 512U;
    daemon_report.exposes_sandboxed_mod_abi = true;
    daemon_report.floating_point_opcodes_detected = 0U;
    daemon_report.layer_skipping_imports_detected = 0U;
    daemon_report.oasis_shadow_soak_ticks = 12000U;
    daemon_report.unhandled_panics_or_deadlocks = 0U;
    daemon_report.human_steward_co_signed = true;
    daemon_report.ai_unsupervised_deploy_attempted = false;

    REQUIRE(VerifySoftwareReleasePipeline(daemon_report));

    // Reject 3-node SHA-256 mismatch
    {
        auto bad_hash = daemon_report;
        bad_hash.builder_3_sha256 = "0000000000000000000000000000000000000000000000000000000000000000";
        REQUIRE_FALSE(VerifySoftwareReleasePipeline(bad_hash));
    }

    // Reject 20-year salvaged laptop RAM budget violation (> 512 MiB)
    {
        auto bloated_ram = daemon_report;
        bloated_ram.idle_ram_mib_on_salvaged_laptop = 640U;
        REQUIRE_FALSE(VerifySoftwareReleasePipeline(bloated_ram));
    }

    // Reject game or L6 app missing sandboxed WASM mod ABI
    {
        auto closed_game = daemon_report;
        closed_game.category = SoftwareAssetCategory::VIDEO_GAME_OR_ENGINE;
        closed_game.exposes_sandboxed_mod_abi = false;
        REQUIRE_FALSE(VerifySoftwareReleasePipeline(closed_game));
    }

    // Reject Constitutional Stack Daemon with IEEE-754 float opcodes or layer-skipping imports
    {
        auto float_daemon = daemon_report;
        float_daemon.floating_point_opcodes_detected = 1U;
        REQUIRE_FALSE(VerifySoftwareReleasePipeline(float_daemon));

        auto layer_skip_daemon = daemon_report;
        layer_skip_daemon.layer_skipping_imports_detected = 1U;
        REQUIRE_FALSE(VerifySoftwareReleasePipeline(layer_skip_daemon));
    }

    // Reject Constitutional Stack Daemon with insufficient Oasis shadow soak (< 10,000 ticks) or panics
    {
        auto short_soak = daemon_report;
        short_soak.oasis_shadow_soak_ticks = 9999U;
        REQUIRE_FALSE(VerifySoftwareReleasePipeline(short_soak));

        auto panic_soak = daemon_report;
        panic_soak.unhandled_panics_or_deadlocks = 1U;
        REQUIRE_FALSE(VerifySoftwareReleasePipeline(panic_soak));
    }
}

TEST_CASE("Law 1.3: Hydronic Compute Heat Recovery (>= 8000 bp), BCC Zero-Sum Conservation & R0 Non-Tokenization", "[scenario_zero][storaged][law1]") {
    using namespace oasis::daemons::storaged;
    using namespace oasis::daemons::execd;

    PairwiseBccChannel channel{};
    channel.peer_did_hash = 0xCAFEBABE12345678ULL;
    REQUIRE((channel.balance_u_to_v_uj + channel.balance_v_to_u_uj) == 0);

    // Reject < 8000 bp (80%) server waste heat recovery (e.g., 75% recovery)
    REQUIRE_FALSE(ComputeHydronicHeatRecoveryAndBcc(
        channel,
        10'000'000ULL,
        100'000'000ULL,
        75'000'000ULL));

    // Accept >= 8000 bp (85% recovery) and verify strict zero-sum BCC clearing
    REQUIRE(ComputeHydronicHeatRecoveryAndBcc(
        channel,
        10'000'000ULL,
        100'000'000ULL,
        85'000'000ULL));
    // Net compute = 15,000,000 uJ + bounty 10,000,000 uJ = 25,000,000 uJ
    REQUIRE(channel.balance_u_to_v_uj == 25'000'000LL);
    REQUIRE(channel.balance_v_to_u_uj == -25'000'000LL);
    REQUIRE((channel.balance_u_to_v_uj + channel.balance_v_to_u_uj) == 0);

    // Direct SettlePairwiseBccTransfer also preserves strict zero-sum invariant
    REQUIRE(SettlePairwiseBccTransfer(channel, 5'000'000ULL));
    REQUIRE(channel.balance_u_to_v_uj == 30'000'000LL);
    REQUIRE(channel.balance_v_to_u_uj == -30'000'000LL);
    REQUIRE((channel.balance_u_to_v_uj + channel.balance_v_to_u_uj) == 0);

    // Layer 3 unconditionally refuses to mint tokens or charge BCC credits for R0 baseline care
    REQUIRE_FALSE(AttemptTokenizeBaselineCare(BaselineCareCategory::HUMBLE_HEARTH_PEA_SOUP_BEAN_STEW));
    REQUIRE_FALSE(AttemptTokenizeBaselineCare(BaselineCareCategory::WARM_SHOWER_AND_HYDRONIC_HEATING));
    REQUIRE_FALSE(AttemptTokenizeBaselineCare(BaselineCareCategory::SANITATION_AND_COMPOST_STEWARDSHIP));
    REQUIRE_FALSE(AttemptTokenizeBaselineCare(BaselineCareCategory::PERINATAL_AND_CHILDCARE_AFFECTION));
    REQUIRE_FALSE(AttemptTokenizeBaselineCare(BaselineCareCategory::ELDER_AND_EMERGENCY_HEALTH_CARE));
    REQUIRE_FALSE(AttemptTokenizeBaselineCare(BaselineCareCategory::OPEN_KNOWLEDGE_AGORA_LEARNING));

    // Verify StoragedEngine MerkleArtifactRecord 3-builder consensus gate
    StoragedEngine storage;
    MerkleArtifactRecord rec{};
    rec.artifact_cid_hash = 0x1122334455667788ULL;
    rec.sbom_merkle_root_hash = 0xAABBCCDDEEFF0011ULL;
    rec.agreed_sha256_prefix = 0x9988776655443322ULL;
    rec.builder_consensus_count = 2U;
    REQUIRE_FALSE(storage.CommitMerkleArtifact(rec));

    rec.builder_consensus_count = 3U;
    REQUIRE(storage.CommitMerkleArtifact(rec));
    REQUIRE(storage.GetArtifactCount() == 1U);
    REQUIRE(storage.FindArtifact(0x1122334455667788ULL) != nullptr);
}

// =============================================================================
// LAW 2: Dual-Fidelity Real Blueprints & Zero-C++-Touch Synthetic Blueprint Pipeline
// =============================================================================

TEST_CASE("Law 2.1: Canonical JSON-LD Blueprints & Stage-1 Artifact Catalog Bootstrap", "[blueprints][law2]") {
    using namespace oasis::artifacts;

    const auto bp_dir = FindBlueprintDir();

    const std::array<std::string, 5> facility_blueprints = {
        "bp_alpha_stage1_solar_array.jsonld",
        "bp_zero_stage1_homelab_workstation.jsonld",
        "bp_gamma_stage1_hearth_and_garden.jsonld",
        "bp_beta_stage1_cargo_trike_depot.jsonld",
        "bp_delta_stage1_childcare_pod.jsonld"
    };

    for (const auto& filename : facility_blueprints) {
        const auto full_path = bp_dir / filename;
        INFO("Checking blueprint file: " << full_path.string());
        REQUIRE(std::filesystem::exists(full_path));
        const std::string content = ReadFileToString(full_path);
        REQUIRE_FALSE(content.empty());
        REQUIRE(content.find("\"realWorldEngineeringSpec\"") != std::string::npos);
        REQUIRE(content.find("\"oasisSimulationProjection\"") != std::string::npos);
    }

    const auto manifest_path = bp_dir / "scn_zero_daemons_manifest.jsonld";
    REQUIRE(std::filesystem::exists(manifest_path));
    const std::string manifest_content = ReadFileToString(manifest_path);
    REQUIRE(manifest_content.find("\"registeredBinaries\"") != std::string::npos);
    REQUIRE(manifest_content.find("\"autopoieticSelfHostingLaw\"") != std::string::npos);
    REQUIRE(manifest_content.find("\"oasis_engine\"") != std::string::npos);

    // Verify BootstrapDefaultStage1ArtifactCatalog and LoadBlueprintCatalogFromDirectory
    ArtifactRegistry reg{};
    BootstrapDefaultStage1ArtifactCatalog(reg);
    const std::size_t loaded_from_disk = LoadBlueprintCatalogFromDirectory(reg, bp_dir.string());
    REQUIRE(loaded_from_disk == 5U);
    REQUIRE(reg.blueprint_count == 5U);
    REQUIRE(reg.FindBlueprintByName("Stage1_Solar_Array_400W") != nullptr);
    REQUIRE(reg.FindBlueprintByName("bp_alpha_stage1_solar_array") != nullptr);
    REQUIRE(reg.FindBlueprintByName("Stage1_Zero_Homelab_Workstation") != nullptr);
    REQUIRE(reg.FindBlueprintByName("Stage1_Gamma_Hearth_And_Garden") != nullptr);
    REQUIRE(reg.FindBlueprintByName("Stage1_Beta_Cargo_ETrike_Depot") != nullptr);
    REQUIRE(reg.FindBlueprintByName("Stage1_Delta_Childcare_Pod") != nullptr);

    const std::string serialized_json = SerializeArtifactCatalogToJson(reg);
    REQUIRE(serialized_json.find("Stage1_Solar_Array_400W") != std::string::npos);
    REQUIRE(serialized_json.find("Stage1_Delta_Childcare_Pod") != std::string::npos);
}

TEST_CASE("Law 2.2: Zero-C++-Touch Synthetic Blueprint E2E Invariant", "[blueprints][synthetic][law2]") {
    using namespace oasis::artifacts;

    ArtifactRegistry reg{};
    BootstrapDefaultStage1ArtifactCatalog(reg);

    // Create a brand-new synthetic blueprint at runtime without modifying any engine switch statements
    DualFidelityBlueprintArtifact synth_bp{};
    synth_bp.blueprint_cid_hash = 0x53594E54485F4250ULL; // "SYNTH_BP"
    synth_bp.treaty_cid_hash = 0x545254595F53594EULL;
    synth_bp.workflow_cid_hash = 0x57464C575F53594EULL;
    CopyCanonicalId(synth_bp.canonical_id, "Synthetic_Atmospheric_Water_Harvester_Stage1");
    synth_bp.scenario_id = 99U;
    synth_bp.min_node_stage = 1U;
    synth_bp.max_acoustic_cb = 480U; // 48.0 dBA perimeter acoustic cap
    synth_bp.required_vc_id = 3U;    // Requires VC bit 3
    synth_bp.uunaa_section_6_shielded = true;
    synth_bp.abstraction_tier = SimulationAbstractionTier::TIER_2_MULTI_STAGE_SUBSET;
    synth_bp.target_voxel_mat_id = 25U;
    synth_bp.net_electric_mw = 100000LL;
    synth_bp.net_thermal_mw = 50000LL;
    synth_bp.potable_water_ml_s = 120LL;
    synth_bp.daily_caloric_yield_kcal = 1500ULL;
    synth_bp.monthly_l7_fiat_revenue_cents = 50000ULL;

    synth_bp.bom_count = 2U;
    synth_bp.bom_dag[0] = BomComponentSpec{
        0x424F4D5F53594E31ULL, 5000000ULL, 42500ULL, 90U, 901U,
        BomSourceTier::L7_PROCUREMENT_FALLBACK, 1U
    };
    synth_bp.bom_dag[1] = BomComponentSpec{
        0x424F4D5F53594E32ULL, 8000000ULL, 0ULL, 20U, 902U,
        BomSourceTier::LOCAL_STOCK_OR_FAB, 0U
    };

    synth_bp.step_count = 2U;
    synth_bp.workflow_steps[0] = CustomizableWorkflowStepDef{
        0x5354505F53594E31ULL, 60U, 90U, 200U, 410U, 50U, 3U,
        WorkflowStepActorMode::UPWARD_L7_PROCUREMENT, 1U, {}
    };
    synth_bp.workflow_steps[1] = CustomizableWorkflowStepDef{
        0x5354505F53594E32ULL, 120U, 180U, 350U, 450U, 51U, 3U,
        WorkflowStepActorMode::HUMAN_STEWARD, 0U, {}
    };

    REQUIRE(RegisterBlueprintArtifact(reg, synth_bp));
    REQUIRE(reg.blueprint_count == 6U);

    AlphaFounderGenesisState founder{};
    REQUIRE(founder.legacy_fiat_reserve_cents == 2'000'000ULL); // $20,000.00 Genesis reserve
    REQUIRE(founder.dependency_ratio_bp == 10000U);

    // 1. Missing required VC (bit 3) -> rejected by L5 gate
    {
        auto rejected_vc = ExecuteBlueprintWorkflowPipeline(reg, "Synthetic_Atmospheric_Water_Harvester_Stage1", founder);
        REQUIRE_FALSE(rejected_vc.ratified_by_l5);
        REQUIRE(rejected_vc.rejection_reason_code == WORKFLOW_REJECT_MISSING_VC);
    }

    // Complete Scenario Kappa learning module for VC 3
    founder.CompleteKappaLearningModule(3U, 500U);

    // 2. Excessive acoustic emission (> 480 cB) -> rejected by L5 acoustic gate
    {
        auto noisy_bp = synth_bp;
        noisy_bp.workflow_steps[1].acoustic_emission_cb = 540U;
        RegisterBlueprintArtifact(reg, noisy_bp);

        auto rejected_noise = ExecuteBlueprintWorkflowPipeline(reg, "Synthetic_Atmospheric_Water_Harvester_Stage1", founder);
        REQUIRE_FALSE(rejected_noise.ratified_by_l5);
        REQUIRE(rejected_noise.rejection_reason_code == WORKFLOW_REJECT_ACOUSTIC_LIMIT_EXCEEDED);
    }

    // Restore compliant synthetic blueprint and execute full L6 -> L5 -> L4 -> L7 U-Turn pipeline
    RegisterBlueprintArtifact(reg, synth_bp);
    auto ok_res = ExecuteBlueprintWorkflowPipeline(reg, "Synthetic_Atmospheric_Water_Harvester_Stage1", founder);

    REQUIRE(ok_res.ratified_by_l5);
    REQUIRE(ok_res.completed_l4_workflow);
    REQUIRE(ok_res.rejection_reason_code == WORKFLOW_REJECT_NONE);
    REQUIRE(ok_res.upward_l7_orders_triggered == 1U);
    REQUIRE(ok_res.l7_fiat_deducted_cents == 42'500ULL);
    REQUIRE(founder.legacy_fiat_reserve_cents == (2'000'000ULL - 42'500ULL));
    REQUIRE(founder.dependency_ratio_bp < 10000U);
    REQUIRE(ok_res.reputation_awarded_bp > 0U);
    REQUIRE(founder.reputation_wealth_bp == (500U + ok_res.reputation_awarded_bp));
}

// =============================================================================
// LAW 3: Constitutional Baseline Right to Exist (B_baseline) & Learn -> Work -> Govern (R)
// =============================================================================

TEST_CASE("Law 3: Constitutional Baseline Right to Exist (B_baseline) & Learn -> Work -> Govern (R) Progression", "[baseline][governance][law3]") {
    using namespace collective::l5;

    BaselineExistenceEntitlement baseline{};
    REQUIRE(baseline.daily_hearth_kcal == 2200U);            // Pea soup & bean stew
    REQUIRE(baseline.warm_shower_and_heating_mk == 293150U); // 20C indoor heating / warm shower
    REQUIRE(baseline.sanitation_and_compost_access);
    REQUIRE(baseline.emergency_and_failing_health_care);
    REQUIRE(baseline.open_knowledge_agora_access);
    REQUIRE(baseline.relies_on_community_baseline);

    CitizenGovernanceStanding citizen{};
    citizen.legacy_grocery_burn_cents_mo = 45000ULL;
    citizen.legacy_utility_burn_cents_mo = 22000ULL;
    citizen.legacy_luxury_habit_cents_mo = 12500ULL; // $125.00/mo optional legacy habit
    citizen.unliberated_property_tax_or_lease_cents_mo = 115000ULL;

    // 1. When relying on community baseline, grocery (45000) & utility (22000) burn drop immediately to 0
    REQUIRE(EvaluateCitizenBurnAndGovernance(baseline, citizen));
    REQUIRE(citizen.unconditional_r0_baseline_guaranteed);
    REQUIRE(citizen.legacy_grocery_burn_cents_mo == 0ULL);
    REQUIRE(citizen.legacy_utility_burn_cents_mo == 0ULL);
    REQUIRE(citizen.net_monthly_fiat_burn_cents == (115000ULL + 12500ULL));
    REQUIRE_FALSE(citizen.can_co_sign_domain_governance);

    // 2. Complete Scenario Kappa apprenticeships and Layer 4 workflow tasks until R >= 2500 bp
    citizen.completed_kappa_apprenticeships = 1U; // +1000 bp and unlocks domain credential
    citizen.completed_l4_workflow_tasks = 2U;     // +1000 bp (total = 2000 bp < 2500 bp)
    REQUIRE(EvaluateCitizenBurnAndGovernance(baseline, citizen));
    REQUIRE(citizen.holds_domain_credential);
    REQUIRE(citizen.reputation_wealth_bp == 2000U);
    REQUIRE_FALSE(citizen.can_co_sign_domain_governance);

    // Complete 1 more L4 workflow task -> R reaches 2500 bp -> unlocks domain governance co-signing!
    citizen.completed_l4_workflow_tasks = 3U; // 1000 + 1500 = 2500 bp
    REQUIRE(EvaluateCitizenBurnAndGovernance(baseline, citizen));
    REQUIRE(citizen.reputation_wealth_bp == 2500U);
    REQUIRE(citizen.can_co_sign_domain_governance);
}

// =============================================================================
// LAW 4: Alpha Genesis ($20,000 Reserve) & 5-Node Bioregional Strategy Viability
// =============================================================================

TEST_CASE("Law 4: Isolated Care Node Deficit vs. Coupled 5-Node Cluster Solidarity", "[strategy][cluster][law4]") {
    using namespace oasis::artifacts;

    // 1. Isolated Care Node (Scenario Delta) runs a monthly fiat deficit when alone
    AlphaFounderGenesisState isolated_delta{};
    isolated_delta.strategy_path = BioregionalStrategyPath::STRATEGY_DELTA_CHILDCARE_SANCTUARY;

    const auto isolated_res = SimulateBioregionalStrategyMonth(isolated_delta, false);
    REQUIRE(isolated_res.gross_l7_revenue_cents == 0ULL);
    REQUIRE(isolated_res.ran_fiat_deficit);
    REQUIRE(isolated_res.net_fiat_delta_cents < 0LL);
    REQUIRE(isolated_delta.legacy_fiat_reserve_cents < ALPHA_GENESIS_FIAT_RESERVE_CENTS);

    // 2. Coupled 5-Node Neighborhood Cluster (Zero + Alpha + Beta + Gamma + Delta)
    std::array<AlphaFounderGenesisState, 5> cluster{};
    cluster[0].strategy_path = BioregionalStrategyPath::STRATEGY_ZERO_SOFTWARE_HOMELAB;
    cluster[1].strategy_path = BioregionalStrategyPath::STRATEGY_ALPHA_GARAGE_FABRICATION;
    cluster[2].strategy_path = BioregionalStrategyPath::STRATEGY_BETA_CARGO_LOGISTICS;
    cluster[3].strategy_path = BioregionalStrategyPath::STRATEGY_GAMMA_COMMONS_HEARTH_FOOD;
    cluster[4].strategy_path = BioregionalStrategyPath::STRATEGY_DELTA_CHILDCARE_SANCTUARY;

    const auto cluster_res = SimulateBioregionalStrategyMonth(cluster, true);
    REQUIRE(cluster_res.all_nodes_enrolled_in_baseline);
    REQUIRE(cluster_res.delta_care_node_thriving);
    REQUIRE(cluster_res.cluster_net_fiat_delta_cents > 0LL);
    REQUIRE(cluster_res.tax_solidarity_subsidy_to_care_cents > UNAVOIDABLE_HOUSING_TAX_BURN_CENTS_MO);

    for (size_t i = 0; i < 5; ++i) {
        REQUIRE(cluster_res.node_results[i].grocery_burn_cents == 0ULL);
        REQUIRE(cluster_res.node_results[i].utility_burn_cents == 0ULL);
        REQUIRE_FALSE(cluster_res.node_results[i].ran_fiat_deficit);
        REQUIRE(cluster[i].legacy_fiat_reserve_cents > ALPHA_GENESIS_FIAT_RESERVE_CENTS);
        REQUIRE(cluster[i].dependency_ratio_bp < 10000U);
    }
}
