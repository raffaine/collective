#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>

namespace oasis::artifacts {

// ============================================================================
// 1. Dual-Fidelity Real Blueprint Structures ("Feeding Oasis Feeds The Collective")
// ============================================================================

enum class SimulationAbstractionTier : uint8_t {
    TIER_1_MACRO_ABSTRACTED        = 1, // Game abstracts complex sub-steps into a clean task with exact net thermodynamic/USD/caloric math
    TIER_2_MULTI_STAGE_SUBSET      = 2, // Game models physical hauling, tool checkout, and multi-stage assembly frames
    TIER_3_FULL_STOICHIOMETRIC_HIL = 3  // Game models full chemical/thermal/HIL telemetry pins
};

enum VoxelMaterialFlags : uint32_t {
    MAT_FLAG_NONE          = 0u,
    MAT_FLAG_SOLID         = 1u << 0,
    MAT_FLAG_PASSABLE      = 1u << 1,
    MAT_FLAG_AIRTIGHT      = 1u << 2,
    MAT_FLAG_HEAT_SOURCE   = 1u << 3,
    MAT_FLAG_CLIMBABLE     = 1u << 4,
    MAT_FLAG_WALKABLE_ROOF = 1u << 5,
    MAT_FLAG_OVERLAY_F1    = 1u << 6,
    MAT_FLAG_OVERLAY_F2    = 1u << 7,
    MAT_FLAG_OVERLAY_F3    = 1u << 8,
    MAT_FLAG_OVERLAY_F4    = 1u << 9
};

struct alignas(16) MaterialVisualGpu {
    float    base_color_rgb[3]{0.5f, 0.5f, 0.5f};
    float    roughness{0.5f};
    float    emissive_rgb[3]{0.0f, 0.0f, 0.0f};
    float    metallic{0.0f};
    uint32_t material_flags{MAT_FLAG_NONE};
    uint32_t proc_shader_id{0u};
    float    exergy_kw_gen{0.0f};
    float    thermal_r_value{0.0f};
};
static_assert(sizeof(MaterialVisualGpu) == 48, "MaterialVisualGpu must be exactly 48 bytes");
static_assert(alignof(MaterialVisualGpu) == 16, "MaterialVisualGpu must be 16-byte aligned");

enum class BomSourceTier : uint8_t {
    LOCAL_STOCK_OR_FAB      = 1,
    MESH_PEER_BCC           = 2,
    L7_PROCUREMENT_FALLBACK = 3
};

struct alignas(8) BomComponentSpec {
    uint64_t      component_cid_hash{0ULL};
    uint64_t      required_mass_mg_or_units{0ULL};
    uint64_t      l7_unit_cost_usd_cents{0ULL};
    uint32_t      l7_lead_time_ticks{0u};
    uint16_t      item_def_id{0u};
    BomSourceTier preferred_tier{BomSourceTier::LOCAL_STOCK_OR_FAB};
    uint8_t       is_irreducible_l7_leaf{0u};
};
static_assert(sizeof(BomComponentSpec) == 32, "BomComponentSpec must be exactly 32 bytes");
static_assert(alignof(BomComponentSpec) == 8, "BomComponentSpec must be 8-byte aligned");

enum class WorkflowStepActorMode : uint8_t {
    HUMAN_STEWARD         = 0,
    AUTOMATED_L4_SERVICE  = 1,
    UPWARD_L7_PROCUREMENT = 2,
    MESH_BETA_TRANSIT     = 3
};

struct alignas(8) CustomizableWorkflowStepDef {
    uint64_t              step_id_hash{0ULL};
    uint32_t              duration_ticks{60u};
    uint32_t              somatic_kcal_cost{0u};
    uint16_t              focus_drain_bp{0u};
    uint16_t              acoustic_emission_cb{300u};
    uint16_t              required_tool_def_id{0u};
    uint16_t              required_vc_id{0u};
    WorkflowStepActorMode actor_mode{WorkflowStepActorMode::HUMAN_STEWARD};
    uint8_t               can_automate_with_l2_kit{0u};
    uint8_t               pad_[6]{};
};
static_assert(sizeof(CustomizableWorkflowStepDef) == 32, "CustomizableWorkflowStepDef must be exactly 32 bytes");
static_assert(alignof(CustomizableWorkflowStepDef) == 8, "CustomizableWorkflowStepDef must be 8-byte aligned");

struct alignas(8) DualFidelityBlueprintArtifact {
    // Layer 6 Identity
    uint64_t blueprint_cid_hash{0ULL};
    uint64_t treaty_cid_hash{0ULL};
    uint64_t workflow_cid_hash{0ULL};

    // Physical Transduction & Carapace Revenue (Fixed-Point SI / USD cents)
    int64_t  net_electric_mw{0LL};
    int64_t  net_thermal_mw{0LL};
    int64_t  potable_water_ml_s{0LL};
    uint64_t daily_caloric_yield_kcal{0ULL};
    uint64_t monthly_l7_fiat_revenue_cents{0ULL};

    // BOM DAG & Workflow Steps
    BomComponentSpec            bom_dag[4]{};
    CustomizableWorkflowStepDef workflow_steps[4]{};

    // Human-readable canonical label
    char canonical_id[64]{};

    // Layer 5 Governance Policy & Classification
    uint16_t                  scenario_id{0u};
    uint16_t                  max_acoustic_cb{550u};
    uint16_t                  required_vc_id{0u};
    uint8_t                   min_node_stage{1u};
    bool                      uunaa_section_6_shielded{true};
    SimulationAbstractionTier abstraction_tier{SimulationAbstractionTier::TIER_2_MULTI_STAGE_SUBSET};
    uint8_t                   target_voxel_mat_id{0u};
    uint8_t                   bom_count{0u};
    uint8_t                   step_count{0u};
    uint16_t                  designator_action_id{0u};
    uint16_t                  blueprint_def_id{0u};

    // Dynamic JSON-LD Bundle & Simulation Projection Metadata
    uint64_t                  l7_cost_cents{0ULL};
    uint64_t                  l7_fallback_cost_usd_cents{0ULL};
    uint32_t                  somatic_kcal_cost{0u};
    uint32_t                  somatic_caloric_cost_kcal{0u};
    int32_t                   dt_delta_bp{0};
    int32_t                   dependency_ratio_delta_bp{0};
    uint16_t                  rep_credit_bp{0u};
    uint16_t                  reputational_wealth_credit_bp{0u};
    uint8_t                   voxel_material_ids[4]{};
    uint8_t                   voxel_material_count{0u};
    uint8_t                   supported_fidelity_modes{0x07u};
    bool                      scraps_electric_utility_baseline{false};
    bool                      scraps_legacy_electric_utility_baseline{false};
    bool                      scraps_grocery_bill_baseline{false};
    bool                      scraps_legacy_grocery_bill_baseline{false};
    uint8_t                   pad_[2]{};

    [[nodiscard]] constexpr uint64_t EffectiveL7CostCents() const noexcept {
        uint64_t bom_sum = 0ULL;
        const uint8_t limit = (bom_count > 0u) ? std::min<uint8_t>(bom_count, 4u) : 4u;
        for (uint8_t i = 0u; i < limit; ++i) {
            if (bom_dag[i].is_irreducible_l7_leaf != 0u ||
                bom_dag[i].preferred_tier == BomSourceTier::L7_PROCUREMENT_FALLBACK) {
                bom_sum += bom_dag[i].l7_unit_cost_usd_cents;
            }
        }
        if (bom_sum > 0ULL) {
            return bom_sum;
        }
        return (l7_cost_cents > 0ULL) ? l7_cost_cents : l7_fallback_cost_usd_cents;
    }

    [[nodiscard]] constexpr int32_t EffectiveDtDeltaBp() const noexcept {
        if (dt_delta_bp != 0) {
            return dt_delta_bp;
        }
        if (dependency_ratio_delta_bp != 0) {
            return dependency_ratio_delta_bp;
        }
        int32_t delta = -800;
        if (net_electric_mw > 0LL) delta -= 600;
        if (net_thermal_mw > 0LL) delta -= 500;
        if (daily_caloric_yield_kcal > 0ULL) delta -= 900;
        if (monthly_l7_fiat_revenue_cents > 0ULL) delta -= 400;
        return delta;
    }

    [[nodiscard]] constexpr uint16_t EffectiveRepCreditBp() const noexcept {
        if (rep_credit_bp > 0u) {
            return rep_credit_bp;
        }
        if (reputational_wealth_credit_bp > 0u) {
            return reputational_wealth_credit_bp;
        }
        return 650u;
    }

    [[nodiscard]] constexpr uint32_t EffectiveSomaticKcalCost() const noexcept {
        if (somatic_kcal_cost > 0u) {
            return somatic_kcal_cost;
        }
        if (somatic_caloric_cost_kcal > 0u) {
            return somatic_caloric_cost_kcal;
        }
        uint32_t sum = 0u;
        const uint8_t limit = (step_count > 0u) ? std::min<uint8_t>(step_count, 4u) : 4u;
        for (uint8_t i = 0u; i < limit; ++i) {
            sum += workflow_steps[i].somatic_kcal_cost;
        }
        return sum;
    }
};
static_assert(sizeof(DualFidelityBlueprintArtifact) == 448, "DualFidelityBlueprintArtifact must be exactly 448 bytes");
static_assert(alignof(DualFidelityBlueprintArtifact) == 8, "DualFidelityBlueprintArtifact must be 8-byte aligned");

// ============================================================================
// 2. The Alpha Genesis Profile, Baseline Right to Exist (B_baseline),
//    & Bioregional Strategy Simulator
// ============================================================================

enum class BioregionalStrategyPath : uint8_t {
    STRATEGY_ZERO_SOFTWARE_HOMELAB    = 0, // Scenario Zero: Bedroom Indie Coder, Modder & Daemon Builder
    STRATEGY_ALPHA_GARAGE_FABRICATION = 1, // Scenario Alpha: Suburban Garage Workshop, Solar & Tool Library
    STRATEGY_BETA_CARGO_LOGISTICS     = 2, // Scenario Beta: Capillary Cargo E-Trike & Microgrid Hub
    STRATEGY_GAMMA_COMMONS_HEARTH_FOOD= 3, // Scenario Gamma: Backyard Homestead & Commons Kitchen
    STRATEGY_DELTA_CHILDCARE_SANCTUARY= 4  // Scenario Delta: Mutual-Aid Childcare Pod & Postpartum Cocoon
};

// Canonical Alpha Genesis Constants
constexpr uint64_t ALPHA_GENESIS_FIAT_RESERVE_CENTS       = 2000000ULL; // $20,000.00 USD
constexpr uint64_t LEGACY_GROCERY_BURN_CENTS_MO           = 45000ULL;   // $450.00/mo legacy grocery burn
constexpr uint64_t LEGACY_UTILITY_BURN_CENTS_MO           = 22000ULL;   // $220.00/mo baseline utility burn
constexpr uint64_t UNAVOIDABLE_HOUSING_TAX_BURN_CENTS_MO  = 115000ULL;  // $1,150.00/mo property tax/mortgage escrow
constexpr uint32_t L5_GOVERNANCE_REPUTATION_THRESHOLD_BP  = 2500u;      // R >= 2500 bp unlocks L5 stewardship

// Canonical Stage-1 Blueprint CID Hashes
constexpr uint64_t CID_STAGE1_ZERO_HOMELAB_WORKSTATION = 0x535447315F5A4552ULL; // "STG1_ZER"
constexpr uint64_t CID_STAGE1_SOLAR_ARRAY_400W         = 0x535447315F534F4CULL; // "STG1_SOL"
constexpr uint64_t CID_STAGE1_BETA_CARGO_ETRIKE_DEPOT  = 0x535447315F424554ULL; // "STG1_BET"
constexpr uint64_t CID_STAGE1_GAMMA_HEARTH_AND_GARDEN  = 0x535447315F47414DULL; // "STG1_GAM"
constexpr uint64_t CID_STAGE1_DELTA_CHILDCARE_POD      = 0x535447315F44454CULL; // "STG1_DEL"

struct alignas(8) AlphaFounderGenesisState {
    // Economic & Fiat Reserves (Suburban Lot 402, Single Adult, Recently Unemployed)
    uint64_t legacy_fiat_reserve_cents{ALPHA_GENESIS_FIAT_RESERVE_CENTS};
    uint64_t unavoidable_housing_tax_burn_cents_mo{UNAVOIDABLE_HOUSING_TAX_BURN_CENTS_MO};
    uint64_t legacy_grocery_burn_cents_mo{LEGACY_GROCERY_BURN_CENTS_MO};
    uint64_t legacy_utility_burn_cents_mo{LEGACY_UTILITY_BURN_CENTS_MO};
    uint64_t optional_legacy_luxury_burn_cents_mo{0ULL};
    uint64_t cumulative_l7_procurement_spent_cents{0ULL};
    uint64_t cumulative_l7_fiat_revenue_earned_cents{0ULL};

    // Parcel & Participation-to-Governance Progression (Learn -> Work -> Reputation R -> Govern)
    uint32_t parcel_lot_id{402u};
    uint32_t dependency_ratio_bp{10000u};               // 100% L7 dependent at Genesis
    uint32_t open_blueprints_accessible_count{5u};      // 100% of catalog visible from Day 1
    uint32_t honest_deficit_bounties_visible_count{4u}; // Honest node deficits visible from Day 1
    uint32_t completed_learning_modules{0u};            // Scenario Kappa modules completed
    uint32_t completed_workflow_contributions{0u};      // Layer 4 workflow steps completed
    uint32_t reputation_wealth_bp{0u};                  // R in [0..10000]

    // Credentials, Stage, Strategy & Community Baseline Flags
    uint16_t                earned_vc_bitmask{0x0001u}; // Bit 0 = Basic Orientation VC
    uint8_t                 current_node_stage{1u};     // Stage 1 Suburban Camouflage
    BioregionalStrategyPath strategy_path{BioregionalStrategyPath::STRATEGY_ALPHA_GARAGE_FABRICATION};
    bool                    enrolled_in_community_baseline{false};
    bool                    unlocked_l5_stewardship_governance{false};
    bool                    is_recently_unemployed_single_adult{true};
    uint8_t                 pad_[5]{};

    [[nodiscard]] constexpr uint64_t EffectiveMonthlyGroceryBurnCents() const noexcept {
        return enrolled_in_community_baseline ? 0ULL : legacy_grocery_burn_cents_mo;
    }

    [[nodiscard]] constexpr uint64_t EffectiveMonthlyUtilityBurnCents() const noexcept {
        return enrolled_in_community_baseline ? 0ULL : legacy_utility_burn_cents_mo;
    }

    [[nodiscard]] constexpr uint64_t EffectiveMonthlyTotalBurnCents() const noexcept {
        return unavoidable_housing_tax_burn_cents_mo +
               EffectiveMonthlyGroceryBurnCents() +
               EffectiveMonthlyUtilityBurnCents() +
               optional_legacy_luxury_burn_cents_mo;
    }

    constexpr void EnrollInCommunityBaseline(bool enrolled = true) noexcept {
        enrolled_in_community_baseline = enrolled;
    }

    constexpr void CompleteKappaLearningModule(uint16_t vc_id = 1u, uint32_t rep_gain_bp = 500u) noexcept {
        completed_learning_modules += 1u;
        if (vc_id < 16u) {
            earned_vc_bitmask = static_cast<uint16_t>(earned_vc_bitmask | static_cast<uint16_t>(1u << vc_id));
        }
        reputation_wealth_bp = std::min<uint32_t>(10000u, reputation_wealth_bp + rep_gain_bp);
        if (reputation_wealth_bp >= L5_GOVERNANCE_REPUTATION_THRESHOLD_BP) {
            unlocked_l5_stewardship_governance = true;
        }
    }
};
static_assert(sizeof(AlphaFounderGenesisState) == 96, "AlphaFounderGenesisState must be exactly 96 bytes");
static_assert(alignof(AlphaFounderGenesisState) == 8, "AlphaFounderGenesisState must be 8-byte aligned");

// ============================================================================
// 3. Universal Dual-Fidelity Artifact Registry & Catalog Bootstrap
// ============================================================================

[[nodiscard]] constexpr uint64_t ResolveKnownStage1CidFromName(std::string_view name) noexcept {
    if (name == "Stage1_Solar_Array_400W" || name == "bp_alpha_stage1_solar_array") {
        return CID_STAGE1_SOLAR_ARRAY_400W;
    }
    if (name == "Stage1_Zero_Homelab_Workstation" || name == "bp_zero_stage1_homelab_workstation") {
        return CID_STAGE1_ZERO_HOMELAB_WORKSTATION;
    }
    if (name == "Stage1_Gamma_Hearth_And_Garden" || name == "bp_gamma_stage1_hearth_and_garden") {
        return CID_STAGE1_GAMMA_HEARTH_AND_GARDEN;
    }
    if (name == "Stage1_Beta_Cargo_ETrike_Depot" ||
        name == "bp_beta_stage1_cargo_etrike_depot" ||
        name == "bp_beta_stage1_cargo_trike_depot") {
        return CID_STAGE1_BETA_CARGO_ETRIKE_DEPOT;
    }
    if (name == "Stage1_Delta_Childcare_Pod" || name == "bp_delta_stage1_childcare_pod") {
        return CID_STAGE1_DELTA_CHILDCARE_POD;
    }
    return 0ULL;
}

struct alignas(16) ArtifactRegistry {
    static constexpr size_t MAX_BLUEPRINTS = 32;
    static constexpr size_t MAX_MATERIALS  = 64;

    MaterialVisualGpu             materials[MAX_MATERIALS]{};
    DualFidelityBlueprintArtifact blueprints[MAX_BLUEPRINTS]{};
    uint32_t                      blueprint_count{0u};
    uint32_t                      material_count{0u};
    uint8_t                       pad_[8]{};

    [[nodiscard]] constexpr const DualFidelityBlueprintArtifact* FindBlueprintByCid(uint64_t cid_hash) const noexcept {
        for (uint32_t i = 0u; i < blueprint_count; ++i) {
            if (blueprints[i].blueprint_cid_hash == cid_hash) {
                return &blueprints[i];
            }
        }
        return nullptr;
    }

    [[nodiscard]] inline const DualFidelityBlueprintArtifact* FindBlueprintByName(std::string_view name) const noexcept {
        for (uint32_t i = 0u; i < blueprint_count; ++i) {
            if (name == std::string_view(blueprints[i].canonical_id)) {
                return &blueprints[i];
            }
        }
        const uint64_t alias_cid = ResolveKnownStage1CidFromName(name);
        if (alias_cid != 0ULL) {
            return FindBlueprintByCid(alias_cid);
        }
        return nullptr;
    }
};
static_assert(sizeof(ArtifactRegistry) == 17424, "ArtifactRegistry must be 17424 bytes");
static_assert(alignof(ArtifactRegistry) == 16, "ArtifactRegistry must be 16-byte aligned");

inline void CopyCanonicalId(char (&dest)[64], std::string_view src) noexcept {
    std::memset(dest, 0, sizeof(dest));
    const size_t n = std::min<size_t>(src.size(), sizeof(dest) - 1u);
    if (n > 0u) {
        std::memcpy(dest, src.data(), n);
    }
}

inline bool RegisterBlueprintArtifact(
    ArtifactRegistry& reg,
    const DualFidelityBlueprintArtifact& artifact
) noexcept {
    // Replace if CID or canonical_id already exists
    for (uint32_t i = 0u; i < reg.blueprint_count; ++i) {
        const bool same_cid = (reg.blueprints[i].blueprint_cid_hash == artifact.blueprint_cid_hash &&
                               artifact.blueprint_cid_hash != 0ULL);
        const bool same_name = (artifact.canonical_id[0] != '\0' &&
                                std::string_view(reg.blueprints[i].canonical_id) == std::string_view(artifact.canonical_id));
        if (same_cid || same_name) {
            const uint16_t preserved_def_id = (artifact.blueprint_def_id > 0u)
                ? artifact.blueprint_def_id
                : ((reg.blueprints[i].blueprint_def_id > 0u)
                    ? reg.blueprints[i].blueprint_def_id
                    : static_cast<uint16_t>(i + 1u));
            reg.blueprints[i] = artifact;
            reg.blueprints[i].blueprint_def_id = preserved_def_id;
            return true;
        }
    }
    if (reg.blueprint_count >= ArtifactRegistry::MAX_BLUEPRINTS) {
        return false;
    }
    DualFidelityBlueprintArtifact copy = artifact;
    if (copy.blueprint_def_id == 0u) {
        copy.blueprint_def_id = static_cast<uint16_t>(reg.blueprint_count + 1u);
    }
    reg.blueprints[reg.blueprint_count++] = copy;
    return true;
}

inline bool RegisterMaterialVisual(
    ArtifactRegistry& reg,
    uint8_t mat_id,
    const MaterialVisualGpu& visual
) noexcept {
    if (mat_id >= ArtifactRegistry::MAX_MATERIALS) {
        return false;
    }
    reg.materials[mat_id] = visual;
    if (static_cast<uint32_t>(mat_id) + 1u > reg.material_count) {
        reg.material_count = static_cast<uint32_t>(mat_id) + 1u;
    }
    return true;
}

inline void BootstrapDefaultStage1ArtifactCatalog(ArtifactRegistry& reg) noexcept {
    reg = ArtifactRegistry{};

    // Register core MaterialVisualGpu descriptors for WebGPU / Voxel semantics
    {
        MaterialVisualGpu solar_mat{};
        solar_mat.base_color_rgb[0] = 0.12f;
        solar_mat.base_color_rgb[1] = 0.24f;
        solar_mat.base_color_rgb[2] = 0.58f;
        solar_mat.roughness         = 0.20f;
        solar_mat.metallic          = 0.80f;
        solar_mat.material_flags    = MAT_FLAG_SOLID | MAT_FLAG_OVERLAY_F2;
        solar_mat.proc_shader_id    = 11u;
        solar_mat.exergy_kw_gen     = 0.40f;
        solar_mat.thermal_r_value   = 1.50f;
        RegisterMaterialVisual(reg, 11u, solar_mat);

        MaterialVisualGpu door_mat{};
        door_mat.base_color_rgb[0] = 0.55f;
        door_mat.base_color_rgb[1] = 0.36f;
        door_mat.base_color_rgb[2] = 0.18f;
        door_mat.roughness         = 0.65f;
        door_mat.material_flags    = MAT_FLAG_PASSABLE | MAT_FLAG_AIRTIGHT | MAT_FLAG_OVERLAY_F1;
        door_mat.proc_shader_id    = 15u;
        door_mat.thermal_r_value   = 4.00f;
        RegisterMaterialVisual(reg, 15u, door_mat);

        MaterialVisualGpu hearth_mat{};
        hearth_mat.base_color_rgb[0] = 0.72f;
        hearth_mat.base_color_rgb[1] = 0.28f;
        hearth_mat.base_color_rgb[2] = 0.14f;
        hearth_mat.emissive_rgb[0]   = 0.85f;
        hearth_mat.emissive_rgb[1]   = 0.30f;
        hearth_mat.emissive_rgb[2]   = 0.05f;
        hearth_mat.roughness         = 0.75f;
        hearth_mat.material_flags    = MAT_FLAG_SOLID | MAT_FLAG_AIRTIGHT | MAT_FLAG_HEAT_SOURCE | MAT_FLAG_OVERLAY_F1;
        hearth_mat.proc_shader_id    = 16u;
        hearth_mat.thermal_r_value   = 8.00f;
        RegisterMaterialVisual(reg, 16u, hearth_mat);
    }

    // 1. Stage1_Solar_Array_400W (Scenario Alpha: Suburban Garage & Roof Solar)
    {
        DualFidelityBlueprintArtifact bp{};
        bp.blueprint_cid_hash            = CID_STAGE1_SOLAR_ARRAY_400W;
        bp.treaty_cid_hash               = 0x545254595F414C50ULL;
        bp.workflow_cid_hash             = 0x57464C575F534F4CULL;
        CopyCanonicalId(bp.canonical_id, "Stage1_Solar_Array_400W");
        bp.scenario_id                   = 1u; // Alpha
        bp.min_node_stage                = 1u;
        bp.max_acoustic_cb               = 550u;
        bp.required_vc_id                = 0u;
        bp.uunaa_section_6_shielded      = true;
        bp.abstraction_tier              = SimulationAbstractionTier::TIER_2_MULTI_STAGE_SUBSET;
        bp.target_voxel_mat_id           = 11u;
        bp.designator_action_id          = 11u;
        bp.blueprint_def_id              = 1u;
        bp.l7_cost_cents                 = 142000ULL;
        bp.l7_fallback_cost_usd_cents    = 142000ULL;
        bp.somatic_kcal_cost             = 420u;
        bp.somatic_caloric_cost_kcal     = 420u;
        bp.dt_delta_bp                   = -1800;
        bp.dependency_ratio_delta_bp     = -1800;
        bp.rep_credit_bp                 = 120u;
        bp.reputational_wealth_credit_bp = 120u;
        bp.voxel_material_ids[0]         = 11u;
        bp.voxel_material_ids[1]         = 15u;
        bp.voxel_material_count          = 2u;
        bp.supported_fidelity_modes      = 0x07u;
        bp.scraps_electric_utility_baseline        = true;
        bp.scraps_legacy_electric_utility_baseline = true;
        bp.net_electric_mw               = 400000LL; // +400 W
        bp.net_thermal_mw                = 0LL;
        bp.potable_water_ml_s            = 0LL;
        bp.daily_caloric_yield_kcal      = 0ULL;
        bp.monthly_l7_fiat_revenue_cents = 80000ULL; // $800/mo fabrication & energy savings/surplus

        bp.bom_count = 2u;
        bp.bom_dag[0] = BomComponentSpec{
            0x424F4D5F50564345ULL, 38000000000ULL, 142000ULL, 120u, 101u,
            BomSourceTier::L7_PROCUREMENT_FALLBACK, 1u
        };
        bp.bom_dag[1] = BomComponentSpec{
            0x424F4D5F5241434BULL, 1850000000ULL, 0ULL, 60u, 102u,
            BomSourceTier::LOCAL_STOCK_OR_FAB, 0u
        };

        bp.step_count = 3u;
        bp.workflow_steps[0] = CustomizableWorkflowStepDef{
            0x5354505F534F4C31ULL, 60u, 60u, 200u, 300u, 1u, 0u,
            WorkflowStepActorMode::UPWARD_L7_PROCUREMENT, 1u, {}
        };
        bp.workflow_steps[1] = CustomizableWorkflowStepDef{
            0x5354505F534F4C32ULL, 180u, 220u, 500u, 460u, 2u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 0u, {}
        };
        bp.workflow_steps[2] = CustomizableWorkflowStepDef{
            0x5354505F534F4C33ULL, 120u, 140u, 300u, 380u, 3u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 1u, {}
        };
        RegisterBlueprintArtifact(reg, bp);
    }

    // 2. Stage1_Zero_Homelab_Workstation (Scenario Zero: Bedroom Indie Coder, Modder & Daemon Builder)
    {
        DualFidelityBlueprintArtifact bp{};
        bp.blueprint_cid_hash            = CID_STAGE1_ZERO_HOMELAB_WORKSTATION;
        bp.treaty_cid_hash               = 0x545254595F5A4552ULL;
        bp.workflow_cid_hash             = 0x57464C575F5A4552ULL;
        CopyCanonicalId(bp.canonical_id, "Stage1_Zero_Homelab_Workstation");
        bp.scenario_id                   = 0u; // Zero
        bp.min_node_stage                = 1u;
        bp.max_acoustic_cb               = 380u; // Quiet bedroom homelab (< 38 dBA)
        bp.required_vc_id                = 0u;
        bp.uunaa_section_6_shielded      = true;
        bp.abstraction_tier              = SimulationAbstractionTier::TIER_3_FULL_STOICHIOMETRIC_HIL;
        bp.target_voxel_mat_id           = 120u; // STUDIO_WORKSTATION_POD
        bp.designator_action_id          = 16u;
        bp.blueprint_def_id              = 2u;
        bp.l7_cost_cents                 = 35000ULL;
        bp.l7_fallback_cost_usd_cents    = 35000ULL;
        bp.somatic_kcal_cost             = 180u;
        bp.somatic_caloric_cost_kcal     = 180u;
        bp.dt_delta_bp                   = -1200;
        bp.dependency_ratio_delta_bp     = -1200;
        bp.rep_credit_bp                 = 150u;
        bp.reputational_wealth_credit_bp = 150u;
        bp.voxel_material_ids[0]         = 120u;
        bp.voxel_material_ids[1]         = 122u;
        bp.voxel_material_count          = 2u;
        bp.supported_fidelity_modes      = 0x07u;
        bp.net_electric_mw               = -65000LL; // -65 W draw
        bp.net_thermal_mw                = 52000LL;  // +52 W waste heat recovery (8000 bp = 80%)
        bp.potable_water_ml_s            = 0LL;
        bp.daily_caloric_yield_kcal      = 0ULL;
        bp.monthly_l7_fiat_revenue_cents = 140000ULL; // $1,400/mo commercial indie game/app publishing (Phi_t)

        bp.bom_count = 2u;
        bp.bom_dag[0] = BomComponentSpec{
            0x424F4D5F54484E4BULL, 2400000000ULL, 35000ULL, 90u, 201u,
            BomSourceTier::L7_PROCUREMENT_FALLBACK, 1u
        };
        bp.bom_dag[1] = BomComponentSpec{
            0x424F4D5F53544D33ULL, 350000000ULL, 0ULL, 30u, 202u,
            BomSourceTier::MESH_PEER_BCC, 0u
        };

        bp.step_count = 3u;
        bp.workflow_steps[0] = CustomizableWorkflowStepDef{
            0x5354505F5A455231ULL, 45u, 30u, 150u, 280u, 10u, 0u,
            WorkflowStepActorMode::UPWARD_L7_PROCUREMENT, 1u, {}
        };
        bp.workflow_steps[1] = CustomizableWorkflowStepDef{
            0x5354505F5A455232ULL, 150u, 80u, 400u, 320u, 11u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 1u, {}
        };
        bp.workflow_steps[2] = CustomizableWorkflowStepDef{
            0x5354505F5A455233ULL, 120u, 70u, 350u, 310u, 12u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 1u, {}
        };
        RegisterBlueprintArtifact(reg, bp);
    }

    // 3. Stage1_Gamma_Hearth_And_Garden (Scenario Gamma: Backyard Homestead & Commons Kitchen)
    {
        DualFidelityBlueprintArtifact bp{};
        bp.blueprint_cid_hash            = CID_STAGE1_GAMMA_HEARTH_AND_GARDEN;
        bp.treaty_cid_hash               = 0x545254595F47414DULL;
        bp.workflow_cid_hash             = 0x57464C575F47414DULL;
        CopyCanonicalId(bp.canonical_id, "Stage1_Gamma_Hearth_And_Garden");
        bp.scenario_id                   = 3u; // Gamma
        bp.min_node_stage                = 1u;
        bp.max_acoustic_cb               = 500u;
        bp.required_vc_id                = 0u;
        bp.uunaa_section_6_shielded      = true;
        bp.abstraction_tier              = SimulationAbstractionTier::TIER_1_MACRO_ABSTRACTED;
        bp.target_voxel_mat_id           = 22u; // Hearth & Garden
        bp.designator_action_id          = 14u;
        bp.blueprint_def_id              = 3u;
        bp.l7_cost_cents                 = 87000ULL;
        bp.l7_fallback_cost_usd_cents    = 87000ULL;
        bp.somatic_kcal_cost             = 250u;
        bp.somatic_caloric_cost_kcal     = 250u;
        bp.dt_delta_bp                   = -2200;
        bp.dependency_ratio_delta_bp     = -2200;
        bp.rep_credit_bp                 = 140u;
        bp.reputational_wealth_credit_bp = 140u;
        bp.voxel_material_ids[0]         = 22u;
        bp.voxel_material_ids[1]         = 23u;
        bp.voxel_material_count          = 2u;
        bp.supported_fidelity_modes      = 0x07u;
        bp.scraps_grocery_bill_baseline        = true;
        bp.scraps_legacy_grocery_bill_baseline = true;
        bp.net_electric_mw               = 0LL;
        bp.net_thermal_mw                = 1500000LL; // +1.5 kW hearth thermal output
        bp.potable_water_ml_s            = 50LL;
        bp.daily_caloric_yield_kcal      = 15600ULL;  // Pea soup, bean stew & garden greens for the cluster
        bp.monthly_l7_fiat_revenue_cents = 40000ULL;  // $400/mo surplus preserve/ferment guild exchange

        bp.bom_count = 2u;
        bp.bom_dag[0] = BomComponentSpec{
            0x424F4D5F50454153ULL, 125000000000ULL, 87000ULL, 60u, 301u,
            BomSourceTier::L7_PROCUREMENT_FALLBACK, 1u
        };
        bp.bom_dag[1] = BomComponentSpec{
            0x424F4D5F434F424CULL, 450000000000ULL, 0ULL, 10u, 302u,
            BomSourceTier::LOCAL_STOCK_OR_FAB, 0u
        };

        bp.step_count = 2u;
        bp.workflow_steps[0] = CustomizableWorkflowStepDef{
            0x5354505F47414D31ULL, 180u, 150u, 450u, 420u, 20u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 0u, {}
        };
        bp.workflow_steps[1] = CustomizableWorkflowStepDef{
            0x5354505F47414D32ULL, 120u, 100u, 300u, 360u, 21u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 1u, {}
        };
        RegisterBlueprintArtifact(reg, bp);
    }

    // 4. Stage1_Beta_Cargo_ETrike_Depot (Scenario Beta: Capillary Cargo E-Trike & Microgrid Hub)
    {
        DualFidelityBlueprintArtifact bp{};
        bp.blueprint_cid_hash            = CID_STAGE1_BETA_CARGO_ETRIKE_DEPOT;
        bp.treaty_cid_hash               = 0x545254595F424554ULL;
        bp.workflow_cid_hash             = 0x57464C575F424554ULL;
        CopyCanonicalId(bp.canonical_id, "Stage1_Beta_Cargo_ETrike_Depot");
        bp.scenario_id                   = 2u; // Beta
        bp.min_node_stage                = 1u;
        bp.max_acoustic_cb               = 520u;
        bp.required_vc_id                = 0u;
        bp.uunaa_section_6_shielded      = true;
        bp.abstraction_tier              = SimulationAbstractionTier::TIER_2_MULTI_STAGE_SUBSET;
        bp.target_voxel_mat_id           = 20u; // Haul / Depot Zone
        bp.designator_action_id          = 5u;
        bp.blueprint_def_id              = 4u;
        bp.l7_cost_cents                 = 115000ULL;
        bp.l7_fallback_cost_usd_cents    = 115000ULL;
        bp.somatic_kcal_cost             = 310u;
        bp.somatic_caloric_cost_kcal     = 310u;
        bp.dt_delta_bp                   = -1500;
        bp.dependency_ratio_delta_bp     = -1500;
        bp.rep_credit_bp                 = 130u;
        bp.reputational_wealth_credit_bp = 130u;
        bp.voxel_material_ids[0]         = 20u;
        bp.voxel_material_count          = 1u;
        bp.supported_fidelity_modes      = 0x07u;
        bp.net_electric_mw               = 150000LL;
        bp.net_thermal_mw                = 0LL;
        bp.potable_water_ml_s            = 25LL;
        bp.daily_caloric_yield_kcal      = 0ULL;
        bp.monthly_l7_fiat_revenue_cents = 60000ULL; // $600/mo cooperative courier & transit savings

        bp.bom_count = 2u;
        bp.bom_dag[0] = BomComponentSpec{
            0x424F4D5F4C465034ULL, 12000000ULL, 115000ULL, 150u, 401u,
            BomSourceTier::L7_PROCUREMENT_FALLBACK, 1u
        };
        bp.bom_dag[1] = BomComponentSpec{
            0x424F4D5F4652414DULL, 15000000ULL, 0ULL, 40u, 402u,
            BomSourceTier::MESH_PEER_BCC, 0u
        };

        bp.step_count = 2u;
        bp.workflow_steps[0] = CustomizableWorkflowStepDef{
            0x5354505F42455431ULL, 150u, 200u, 400u, 470u, 30u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 0u, {}
        };
        bp.workflow_steps[1] = CustomizableWorkflowStepDef{
            0x5354505F42455432ULL, 90u, 110u, 250u, 390u, 31u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 1u, {}
        };
        RegisterBlueprintArtifact(reg, bp);
    }

    // 5. Stage1_Delta_Childcare_Pod (Scenario Delta: Mutual-Aid Childcare Pod & Postpartum Cocoon)
    {
        DualFidelityBlueprintArtifact bp{};
        bp.blueprint_cid_hash            = CID_STAGE1_DELTA_CHILDCARE_POD;
        bp.treaty_cid_hash               = 0x545254595F44454CULL;
        bp.workflow_cid_hash             = 0x57464C575F44454CULL;
        CopyCanonicalId(bp.canonical_id, "Stage1_Delta_Childcare_Pod");
        bp.scenario_id                   = 4u; // Delta
        bp.min_node_stage                = 1u;
        bp.max_acoustic_cb               = 500u;
        bp.required_vc_id                = 0u;
        bp.uunaa_section_6_shielded      = true;
        bp.abstraction_tier              = SimulationAbstractionTier::TIER_1_MACRO_ABSTRACTED;
        bp.target_voxel_mat_id           = 22u; // Sanctuary / Care Zone
        bp.designator_action_id          = 4u;
        bp.blueprint_def_id              = 5u;
        bp.l7_cost_cents                 = 125000ULL;
        bp.l7_fallback_cost_usd_cents    = 125000ULL;
        bp.somatic_kcal_cost             = 290u;
        bp.somatic_caloric_cost_kcal     = 290u;
        bp.dt_delta_bp                   = -1300;
        bp.dependency_ratio_delta_bp     = -1300;
        bp.rep_credit_bp                 = 160u;
        bp.reputational_wealth_credit_bp = 160u;
        bp.voxel_material_ids[0]         = 22u;
        bp.voxel_material_count          = 1u;
        bp.supported_fidelity_modes      = 0x07u;
        bp.net_electric_mw               = -30000LL;
        bp.net_thermal_mw                = 250000LL;
        bp.potable_water_ml_s            = 15LL;
        bp.daily_caloric_yield_kcal      = 0ULL;
        bp.monthly_l7_fiat_revenue_cents = 0ULL; // Pure mutual aid: 0 external fiat monetization!

        bp.bom_count = 1u;
        bp.bom_dag[0] = BomComponentSpec{
            0x424F4D5F48455041ULL, 5000000ULL, 125000ULL, 60u, 501u,
            BomSourceTier::L7_PROCUREMENT_FALLBACK, 1u
        };

        bp.step_count = 2u;
        bp.workflow_steps[0] = CustomizableWorkflowStepDef{
            0x5354505F44454C31ULL, 120u, 150u, 300u, 380u, 40u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 0u, {}
        };
        bp.workflow_steps[1] = CustomizableWorkflowStepDef{
            0x5354505F44454C32ULL, 120u, 140u, 250u, 350u, 41u, 0u,
            WorkflowStepActorMode::HUMAN_STEWARD, 0u, {}
        };
        RegisterBlueprintArtifact(reg, bp);
    }
}

// ============================================================================
// 4. L6 -> L5 -> L4 Workflow Execution Pipeline & Upward L7 Procurement
// ============================================================================

enum WorkflowRejectionReason : uint8_t {
    WORKFLOW_REJECT_NONE                     = 0u,
    WORKFLOW_REJECT_NODE_STAGE_TOO_LOW       = 1u,
    WORKFLOW_REJECT_ACOUSTIC_LIMIT_EXCEEDED  = 2u,
    WORKFLOW_REJECT_MISSING_VC               = 3u,
    WORKFLOW_REJECT_INSUFFICIENT_L7_RESERVE  = 4u,
    WORKFLOW_REJECT_BLUEPRINT_NOT_FOUND      = 5u
};

struct alignas(8) WorkflowPipelineExecutionResult {
    uint64_t l7_fiat_deducted_cents{0ULL};
    uint32_t total_duration_ticks{0u};
    uint32_t total_somatic_kcal_burned{0u};
    uint32_t reputation_awarded_bp{0u};
    uint32_t updated_dependency_ratio_bp{10000u};
    bool     ratified_by_l5{false};
    bool     completed_l4_workflow{false};
    uint8_t  rejection_reason_code{WORKFLOW_REJECT_NONE};
    uint8_t  upward_l7_orders_triggered{0u};
    uint8_t  pad_[4]{};
};
static_assert(sizeof(WorkflowPipelineExecutionResult) == 32, "WorkflowPipelineExecutionResult must be 32 bytes");
static_assert(alignof(WorkflowPipelineExecutionResult) == 8, "WorkflowPipelineExecutionResult must be 8-byte aligned");

inline WorkflowPipelineExecutionResult ExecuteBlueprintWorkflowPipeline(
    const DualFidelityBlueprintArtifact& bp,
    AlphaFounderGenesisState& founder
) noexcept {
    WorkflowPipelineExecutionResult result{};
    result.updated_dependency_ratio_bp = founder.dependency_ratio_bp;

    // 1. Layer 5 Governance Gates: Node Stage, Verifiable Credentials & Acoustic Envelope
    if (founder.current_node_stage < bp.min_node_stage) {
        result.rejection_reason_code = WORKFLOW_REJECT_NODE_STAGE_TOO_LOW;
        return result;
    }

    if (bp.required_vc_id > 0u && bp.required_vc_id < 16u) {
        const uint16_t mask = static_cast<uint16_t>(1u << bp.required_vc_id);
        if ((founder.earned_vc_bitmask & mask) == 0u) {
            result.rejection_reason_code = WORKFLOW_REJECT_MISSING_VC;
            return result;
        }
    }

    const uint8_t step_limit = (bp.step_count > 0u) ? std::min<uint8_t>(bp.step_count, 4u) : 4u;
    for (uint8_t i = 0u; i < step_limit; ++i) {
        const auto& step = bp.workflow_steps[i];
        const bool active_step = (bp.step_count > 0u) ||
                                 (step.step_id_hash != 0ULL) ||
                                 (step.somatic_kcal_cost != 0u) ||
                                 (step.focus_drain_bp != 0u) ||
                                 (step.required_tool_def_id != 0u) ||
                                 (step.required_vc_id != 0u);
        if (!active_step) {
            continue;
        }
        if (step.acoustic_emission_cb > bp.max_acoustic_cb) {
            result.rejection_reason_code = WORKFLOW_REJECT_ACOUSTIC_LIMIT_EXCEEDED;
            return result;
        }
        if (step.required_vc_id > 0u && step.required_vc_id < 16u) {
            const uint16_t mask = static_cast<uint16_t>(1u << step.required_vc_id);
            if ((founder.earned_vc_bitmask & mask) == 0u) {
                result.rejection_reason_code = WORKFLOW_REJECT_MISSING_VC;
                return result;
            }
        }
    }

    // 2. BOM DAG Resolution & Upward L4 -> L5 -> L6 -> L7 Irreducible Leaf Procurement
    uint64_t required_l7_fiat_cents = 0ULL;
    uint8_t  l7_leaf_orders         = 0u;
    const uint8_t bom_limit = (bp.bom_count > 0u) ? std::min<uint8_t>(bp.bom_count, 4u) : 4u;
    for (uint8_t i = 0u; i < bom_limit; ++i) {
        const auto& comp = bp.bom_dag[i];
        const bool active_bom = (bp.bom_count > 0u) ||
                                (comp.component_cid_hash != 0ULL) ||
                                (comp.required_mass_mg_or_units != 0ULL) ||
                                (comp.l7_unit_cost_usd_cents != 0ULL) ||
                                (comp.is_irreducible_l7_leaf != 0u);
        if (!active_bom) {
            continue;
        }
        if (comp.is_irreducible_l7_leaf != 0u ||
            comp.preferred_tier == BomSourceTier::L7_PROCUREMENT_FALLBACK) {
            required_l7_fiat_cents += comp.l7_unit_cost_usd_cents;
            if (comp.l7_unit_cost_usd_cents > 0ULL || comp.is_irreducible_l7_leaf != 0u) {
                l7_leaf_orders += 1u;
            }
        }
    }
    if (required_l7_fiat_cents == 0ULL) {
        required_l7_fiat_cents = bp.EffectiveL7CostCents();
        if (required_l7_fiat_cents > 0ULL && l7_leaf_orders == 0u) {
            l7_leaf_orders = 1u;
        }
    }

    if (founder.legacy_fiat_reserve_cents < required_l7_fiat_cents) {
        result.rejection_reason_code = WORKFLOW_REJECT_INSUFFICIENT_L7_RESERVE;
        return result;
    }

    result.ratified_by_l5 = true;

    // Deduct exact integer USD cents for upward L7 procurement
    founder.legacy_fiat_reserve_cents             -= required_l7_fiat_cents;
    founder.cumulative_l7_procurement_spent_cents += required_l7_fiat_cents;
    result.l7_fiat_deducted_cents                  = required_l7_fiat_cents;
    result.upward_l7_orders_triggered              = l7_leaf_orders;

    // Apply baseline utility / grocery bill relief when commissioned blueprint scraps legacy dependency
    if (bp.scraps_electric_utility_baseline || bp.scraps_legacy_electric_utility_baseline) {
        founder.legacy_utility_burn_cents_mo = 0ULL;
    }
    if (bp.scraps_grocery_bill_baseline || bp.scraps_legacy_grocery_bill_baseline) {
        founder.legacy_grocery_burn_cents_mo = 0ULL;
    }

    // 3. Layer 4 Workflow Execution & Participation-to-Governance Progression
    uint32_t human_steps_completed = 0u;
    for (uint8_t i = 0u; i < step_limit; ++i) {
        const auto& step = bp.workflow_steps[i];
        const bool active_step = (bp.step_count > 0u) ||
                                 (step.step_id_hash != 0ULL) ||
                                 (step.somatic_kcal_cost != 0u) ||
                                 (step.focus_drain_bp != 0u) ||
                                 (step.required_tool_def_id != 0u) ||
                                 (i == 0u);
        if (!active_step) {
            continue;
        }
        result.total_duration_ticks      += step.duration_ticks;
        result.total_somatic_kcal_burned += step.somatic_kcal_cost;
        if (step.actor_mode == WorkflowStepActorMode::HUMAN_STEWARD) {
            human_steps_completed += 1u;
            founder.completed_workflow_contributions += 1u;
        }
    }
    if (result.total_somatic_kcal_burned == 0u) {
        result.total_somatic_kcal_burned = bp.EffectiveSomaticKcalCost();
    }

    // Credit Reputational Wealth (R) for active stewardship work
    const uint32_t rep_gain_bp = (human_steps_completed > 0u) ? (human_steps_completed * 650u) : 300u;
    founder.reputation_wealth_bp = std::min<uint32_t>(10000u, founder.reputation_wealth_bp + rep_gain_bp);
    if (founder.reputation_wealth_bp >= L5_GOVERNANCE_REPUTATION_THRESHOLD_BP) {
        founder.unlocked_l5_stewardship_governance = true;
    }

    // Reduce external Layer 7 Dependency Ratio (D_t) based on deployed sovereign capacity
    uint32_t dep_reduction_bp = 800u;
    const int32_t explicit_dt = (bp.dt_delta_bp != 0) ? bp.dt_delta_bp : bp.dependency_ratio_delta_bp;
    if (explicit_dt != 0) {
        dep_reduction_bp = static_cast<uint32_t>(explicit_dt < 0 ? -explicit_dt : explicit_dt);
    } else {
        if (bp.net_electric_mw > 0LL) {
            dep_reduction_bp += 600u;
        }
        if (bp.net_thermal_mw > 0LL) {
            dep_reduction_bp += 500u;
        }
        if (bp.daily_caloric_yield_kcal > 0ULL) {
            dep_reduction_bp += 900u;
        }
        if (bp.monthly_l7_fiat_revenue_cents > 0ULL) {
            dep_reduction_bp += 400u;
        }
    }
    if (founder.dependency_ratio_bp > dep_reduction_bp) {
        founder.dependency_ratio_bp -= dep_reduction_bp;
    } else {
        founder.dependency_ratio_bp = 0u;
    }

    result.reputation_awarded_bp       = rep_gain_bp;
    result.updated_dependency_ratio_bp = founder.dependency_ratio_bp;
    result.completed_l4_workflow       = true;
    return result;
}

inline WorkflowPipelineExecutionResult ExecuteBlueprintWorkflowPipeline(
    const ArtifactRegistry& reg,
    uint64_t blueprint_cid_hash,
    AlphaFounderGenesisState& founder
) noexcept {
    const DualFidelityBlueprintArtifact* bp = reg.FindBlueprintByCid(blueprint_cid_hash);
    if (bp == nullptr) {
        WorkflowPipelineExecutionResult not_found{};
        not_found.rejection_reason_code       = WORKFLOW_REJECT_BLUEPRINT_NOT_FOUND;
        not_found.updated_dependency_ratio_bp = founder.dependency_ratio_bp;
        return not_found;
    }
    return ExecuteBlueprintWorkflowPipeline(*bp, founder);
}

inline WorkflowPipelineExecutionResult ExecuteBlueprintWorkflowPipeline(
    const ArtifactRegistry& reg,
    std::string_view canonical_id,
    AlphaFounderGenesisState& founder
) noexcept {
    const DualFidelityBlueprintArtifact* bp = reg.FindBlueprintByName(canonical_id);
    if (bp == nullptr) {
        WorkflowPipelineExecutionResult not_found{};
        not_found.rejection_reason_code       = WORKFLOW_REJECT_BLUEPRINT_NOT_FOUND;
        not_found.updated_dependency_ratio_bp = founder.dependency_ratio_bp;
        return not_found;
    }
    return ExecuteBlueprintWorkflowPipeline(*bp, founder);
}

inline WorkflowPipelineExecutionResult ExecuteBlueprintWorkflowPipeline(
    const ArtifactRegistry& /*reg*/,
    const DualFidelityBlueprintArtifact& bp,
    AlphaFounderGenesisState& founder
) noexcept {
    return ExecuteBlueprintWorkflowPipeline(bp, founder);
}

// ============================================================================
// 5. Bioregional Strategy Simulator (Single Node vs. Coupled 5-Node Cluster)
// ============================================================================

struct alignas(8) StrategyMonthSimulationResult {
    int64_t                 net_fiat_delta_cents{0LL};
    uint64_t                gross_l7_revenue_cents{0ULL};
    uint64_t                total_fiat_burn_cents{0ULL};
    uint64_t                grocery_burn_cents{0ULL};
    uint64_t                utility_burn_cents{0ULL};
    uint64_t                housing_tax_burn_cents{0ULL};
    uint64_t                ending_fiat_reserve_cents{0ULL};
    uint32_t                ending_dependency_ratio_bp{10000u};
    BioregionalStrategyPath strategy_path{BioregionalStrategyPath::STRATEGY_ALPHA_GARAGE_FABRICATION};
    bool                    baseline_sustained{false};
    bool                    ran_fiat_deficit{false};
    uint8_t                 pad_[1]{};
};
static_assert(sizeof(StrategyMonthSimulationResult) == 64, "StrategyMonthSimulationResult must be 64 bytes");
static_assert(alignof(StrategyMonthSimulationResult) == 8, "StrategyMonthSimulationResult must be 8-byte aligned");

struct alignas(8) ClusterMonthSimulationResult {
    StrategyMonthSimulationResult node_results[5]{};
    int64_t                       cluster_net_fiat_delta_cents{0LL};
    uint64_t                      cluster_gross_l7_revenue_cents{0ULL};
    uint64_t                      cluster_total_tax_burn_cents{0ULL};
    uint64_t                      tax_solidarity_subsidy_to_care_cents{0ULL};
    uint32_t                      average_dependency_ratio_bp{0u};
    bool                          delta_care_node_thriving{false};
    bool                          all_nodes_enrolled_in_baseline{false};
    uint8_t                       pad_[2]{};
};
static_assert(sizeof(ClusterMonthSimulationResult) == 360, "ClusterMonthSimulationResult must be 360 bytes");
static_assert(alignof(ClusterMonthSimulationResult) == 8, "ClusterMonthSimulationResult must be 8-byte aligned");

[[nodiscard]] constexpr uint64_t GetMonthlyStrategyL7RevenueCents(
    BioregionalStrategyPath path,
    bool coupled_cluster_solidarity
) noexcept {
    switch (path) {
        case BioregionalStrategyPath::STRATEGY_ZERO_SOFTWARE_HOMELAB:
            // Indie game/app publishing & B2B software carapace revenue (boosted in coupled cluster)
            return coupled_cluster_solidarity ? 260000ULL : 155000ULL;
        case BioregionalStrategyPath::STRATEGY_ALPHA_GARAGE_FABRICATION:
            // Custom CNC/solar fabrication & repair carapace revenue
            return coupled_cluster_solidarity ? 165000ULL : 120000ULL;
        case BioregionalStrategyPath::STRATEGY_BETA_CARGO_LOGISTICS:
            // Last-mile cargo e-trike logistics & microgrid balancing
            return coupled_cluster_solidarity ? 120000ULL : 85000ULL;
        case BioregionalStrategyPath::STRATEGY_GAMMA_COMMONS_HEARTH_FOOD:
            // Artisanal ferments, seed library & market surplus
            return coupled_cluster_solidarity ? 90000ULL : 65000ULL;
        case BioregionalStrategyPath::STRATEGY_DELTA_CHILDCARE_SANCTUARY:
            // Pure non-monetized mutual aid & care sanctuary ($0 external L7 revenue)
            return 0ULL;
    }
    return 0ULL;
}

inline StrategyMonthSimulationResult SimulateBioregionalStrategyMonth(
    AlphaFounderGenesisState& node,
    bool coupled_cluster_solidarity = false
) noexcept {
    if (coupled_cluster_solidarity) {
        node.EnrollInCommunityBaseline(true);
    }

    StrategyMonthSimulationResult res{};
    res.strategy_path = node.strategy_path;

    // Evaluate monthly grocery & utility burn
    uint64_t grocery_burn = node.EffectiveMonthlyGroceryBurnCents();
    uint64_t utility_burn = node.EffectiveMonthlyUtilityBurnCents();

    if (!node.enrolled_in_community_baseline) {
        // Partial self-reliance for specific isolated nodes
        if (node.strategy_path == BioregionalStrategyPath::STRATEGY_GAMMA_COMMONS_HEARTH_FOOD) {
            grocery_burn = 0ULL; // Gamma feeds itself pea soup & bean stew
        } else if (node.strategy_path == BioregionalStrategyPath::STRATEGY_ALPHA_GARAGE_FABRICATION ||
                   node.strategy_path == BioregionalStrategyPath::STRATEGY_BETA_CARGO_LOGISTICS) {
            utility_burn = utility_burn / 2ULL; // Solar/microgrid halves utility bill
        }
    }

    const uint64_t housing_tax = node.unavoidable_housing_tax_burn_cents_mo;
    const uint64_t luxury_burn = node.optional_legacy_luxury_burn_cents_mo;
    uint64_t total_burn        = housing_tax + grocery_burn + utility_burn + luxury_burn;
    uint64_t gross_rev         = GetMonthlyStrategyL7RevenueCents(node.strategy_path, coupled_cluster_solidarity);

    // In coupled cluster solidarity, the neighborhood Commons pools Phi_t to cover
    // unavoidable property tax for non-monetized care infrastructure (Scenario Delta).
    if (coupled_cluster_solidarity &&
        node.strategy_path == BioregionalStrategyPath::STRATEGY_DELTA_CHILDCARE_SANCTUARY) {
        gross_rev = housing_tax + 15000ULL; // Tax escrow covered + modest cooperative stipend
    }

    node.cumulative_l7_fiat_revenue_earned_cents += gross_rev;

    const int64_t net_delta = static_cast<int64_t>(gross_rev) - static_cast<int64_t>(total_burn);
    if (net_delta >= 0LL) {
        node.legacy_fiat_reserve_cents += static_cast<uint64_t>(net_delta);
    } else {
        const uint64_t deficit = static_cast<uint64_t>(-net_delta);
        if (node.legacy_fiat_reserve_cents >= deficit) {
            node.legacy_fiat_reserve_cents -= deficit;
        } else {
            node.legacy_fiat_reserve_cents = 0ULL;
        }
    }

    // Update dependency ratio & governance progression
    const uint32_t dep_step = coupled_cluster_solidarity ? 1200u : 450u;
    if (node.dependency_ratio_bp > dep_step) {
        node.dependency_ratio_bp -= dep_step;
    } else {
        node.dependency_ratio_bp = 0u;
    }

    node.completed_workflow_contributions += 1u;
    node.reputation_wealth_bp = std::min<uint32_t>(
        10000u,
        node.reputation_wealth_bp + (coupled_cluster_solidarity ? 700u : 350u)
    );
    if (node.reputation_wealth_bp >= L5_GOVERNANCE_REPUTATION_THRESHOLD_BP) {
        node.unlocked_l5_stewardship_governance = true;
    }

    res.net_fiat_delta_cents       = net_delta;
    res.gross_l7_revenue_cents     = gross_rev;
    res.total_fiat_burn_cents      = total_burn;
    res.grocery_burn_cents         = grocery_burn;
    res.utility_burn_cents         = utility_burn;
    res.housing_tax_burn_cents     = housing_tax;
    res.ending_fiat_reserve_cents  = node.legacy_fiat_reserve_cents;
    res.ending_dependency_ratio_bp = node.dependency_ratio_bp;
    res.baseline_sustained         = node.enrolled_in_community_baseline || (grocery_burn == 0ULL);
    res.ran_fiat_deficit           = (net_delta < 0LL);
    return res;
}

inline StrategyMonthSimulationResult SimulateBioregionalStrategyMonth(
    AlphaFounderGenesisState& node,
    BioregionalStrategyPath path,
    bool coupled_cluster_solidarity = false
) noexcept {
    node.strategy_path = path;
    return SimulateBioregionalStrategyMonth(node, coupled_cluster_solidarity);
}

inline ClusterMonthSimulationResult SimulateBioregionalStrategyMonth(
    std::array<AlphaFounderGenesisState, 5>& cluster_nodes,
    bool coupled_cluster_solidarity = true
) noexcept {
    ClusterMonthSimulationResult cluster_res{};
    cluster_res.all_nodes_enrolled_in_baseline = coupled_cluster_solidarity;

    if (!coupled_cluster_solidarity) {
        uint64_t dep_sum = 0ULL;
        for (size_t i = 0u; i < 5u; ++i) {
            cluster_nodes[i].EnrollInCommunityBaseline(false);
            cluster_res.node_results[i] = SimulateBioregionalStrategyMonth(cluster_nodes[i], false);
            cluster_res.cluster_net_fiat_delta_cents   += cluster_res.node_results[i].net_fiat_delta_cents;
            cluster_res.cluster_gross_l7_revenue_cents += cluster_res.node_results[i].gross_l7_revenue_cents;
            cluster_res.cluster_total_tax_burn_cents   += cluster_res.node_results[i].housing_tax_burn_cents;
            dep_sum += cluster_res.node_results[i].ending_dependency_ratio_bp;
            if (cluster_nodes[i].strategy_path == BioregionalStrategyPath::STRATEGY_DELTA_CHILDCARE_SANCTUARY) {
                cluster_res.delta_care_node_thriving = !cluster_res.node_results[i].ran_fiat_deficit;
            }
        }
        cluster_res.average_dependency_ratio_bp = static_cast<uint32_t>(dep_sum / 5ULL);
        return cluster_res;
    }

    // Coupled 5-Node Neighborhood Cluster (Zero + Alpha + Beta + Gamma + Delta):
    // 1. All 5 nodes enroll in Community Baseline (pea soup & bean stew from Gamma,
    //    warm shower & heating from Alpha/Gamma, sanitation & logistics from Beta,
    //    mutual-aid care from Delta, and open L6 software/knowledge from Zero).
    // 2. Revenue-generating nodes pool external Phi_t surplus to cover Delta's property tax.
    uint64_t total_cluster_rev  = 0ULL;
    uint64_t total_cluster_tax  = 0ULL;
    uint64_t total_cluster_burn = 0ULL;

    for (size_t i = 0u; i < 5u; ++i) {
        cluster_nodes[i].EnrollInCommunityBaseline(true);
        const uint64_t rev = GetMonthlyStrategyL7RevenueCents(cluster_nodes[i].strategy_path, true);
        const uint64_t tax = cluster_nodes[i].unavoidable_housing_tax_burn_cents_mo;
        const uint64_t lux = cluster_nodes[i].optional_legacy_luxury_burn_cents_mo;
        total_cluster_rev  += rev;
        total_cluster_tax  += tax;
        total_cluster_burn += (tax + lux);
    }

    const int64_t cluster_net = static_cast<int64_t>(total_cluster_rev) - static_cast<int64_t>(total_cluster_burn);
    const uint64_t shared_dividend_per_node = (cluster_net > 0LL)
        ? (static_cast<uint64_t>(cluster_net) / 5ULL)
        : 0ULL;

    uint64_t dep_sum = 0ULL;
    for (size_t i = 0u; i < 5u; ++i) {
        auto& node = cluster_nodes[i];
        auto& nres = cluster_res.node_results[i];

        const uint64_t raw_rev = GetMonthlyStrategyL7RevenueCents(node.strategy_path, true);
        const uint64_t tax     = node.unavoidable_housing_tax_burn_cents_mo;
        const uint64_t lux     = node.optional_legacy_luxury_burn_cents_mo;

        node.cumulative_l7_fiat_revenue_earned_cents += raw_rev;

        if (node.strategy_path == BioregionalStrategyPath::STRATEGY_DELTA_CHILDCARE_SANCTUARY) {
            cluster_res.tax_solidarity_subsidy_to_care_cents = tax + shared_dividend_per_node;
        }

        // After cooperative tax clearing, each node receives its equal share of net cluster Phi_t
        node.legacy_fiat_reserve_cents += shared_dividend_per_node;

        if (node.dependency_ratio_bp > 1500u) {
            node.dependency_ratio_bp -= 1500u;
        } else {
            node.dependency_ratio_bp = 0u;
        }

        node.completed_workflow_contributions += 1u;
        node.reputation_wealth_bp = std::min<uint32_t>(10000u, node.reputation_wealth_bp + 750u);
        if (node.reputation_wealth_bp >= L5_GOVERNANCE_REPUTATION_THRESHOLD_BP) {
            node.unlocked_l5_stewardship_governance = true;
        }

        nres.strategy_path              = node.strategy_path;
        nres.gross_l7_revenue_cents     = raw_rev;
        nres.grocery_burn_cents         = 0ULL;
        nres.utility_burn_cents         = 0ULL;
        nres.housing_tax_burn_cents     = tax;
        nres.total_fiat_burn_cents      = tax + lux;
        nres.net_fiat_delta_cents       = static_cast<int64_t>(shared_dividend_per_node);
        nres.ending_fiat_reserve_cents  = node.legacy_fiat_reserve_cents;
        nres.ending_dependency_ratio_bp = node.dependency_ratio_bp;
        nres.baseline_sustained         = true;
        nres.ran_fiat_deficit           = false;

        if (node.strategy_path == BioregionalStrategyPath::STRATEGY_DELTA_CHILDCARE_SANCTUARY) {
            cluster_res.delta_care_node_thriving = true;
        }
        dep_sum += node.dependency_ratio_bp;
    }

    cluster_res.cluster_net_fiat_delta_cents   = cluster_net;
    cluster_res.cluster_gross_l7_revenue_cents = total_cluster_rev;
    cluster_res.cluster_total_tax_burn_cents   = total_cluster_tax;
    cluster_res.average_dependency_ratio_bp    = static_cast<uint32_t>(dep_sum / 5ULL);
    return cluster_res;
}

} // namespace oasis::artifacts

#include "blueprint_bundle_loader.hpp"

