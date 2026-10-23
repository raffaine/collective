#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "artifact_registry.hpp"

namespace oasis::artifacts {

namespace detail {

[[nodiscard]] constexpr uint64_t HashFnv1a64(std::string_view text) noexcept {
    uint64_t hash = 14695981039346656037ULL;
    for (char c : text) {
        hash ^= static_cast<uint64_t>(static_cast<unsigned char>(c));
        hash *= 1099511628211ULL;
    }
    return (hash == 0ULL) ? 1ULL : hash;
}

[[nodiscard]] inline size_t FindKeyValueStart(std::string_view json, std::string_view key, size_t start_pos = 0u) noexcept {
    std::string quoted_key;
    quoted_key.reserve(key.size() + 2u);
    quoted_key.push_back('"');
    quoted_key.append(key.data(), key.size());
    quoted_key.push_back('"');

    size_t pos = json.find(quoted_key, start_pos);
    while (pos != std::string_view::npos) {
        size_t after_key = pos + quoted_key.size();
        while (after_key < json.size() && std::isspace(static_cast<unsigned char>(json[after_key]))) {
            ++after_key;
        }
        if (after_key < json.size() && json[after_key] == ':') {
            ++after_key;
            while (after_key < json.size() && std::isspace(static_cast<unsigned char>(json[after_key]))) {
                ++after_key;
            }
            return after_key;
        }
        pos = json.find(quoted_key, pos + quoted_key.size());
    }
    return std::string_view::npos;
}

[[nodiscard]] inline bool ExtractJsonString(
    std::string_view json,
    std::string_view key,
    std::string& out_value,
    size_t start_pos = 0u
) {
    const size_t val_pos = FindKeyValueStart(json, key, start_pos);
    if (val_pos == std::string_view::npos || val_pos >= json.size() || json[val_pos] != '"') {
        return false;
    }
    std::string result;
    size_t i = val_pos + 1u;
    while (i < json.size()) {
        const char c = json[i];
        if (c == '\\' && (i + 1u) < json.size()) {
            result.push_back(json[i + 1u]);
            i += 2u;
            continue;
        }
        if (c == '"') {
            out_value = result;
            return true;
        }
        result.push_back(c);
        ++i;
    }
    return false;
}

[[nodiscard]] inline bool ExtractJsonInt64(
    std::string_view json,
    std::string_view key,
    int64_t& out_value,
    size_t start_pos = 0u
) noexcept {
    const size_t val_pos = FindKeyValueStart(json, key, start_pos);
    if (val_pos == std::string_view::npos || val_pos >= json.size()) {
        return false;
    }
    size_t end_pos = val_pos;
    if (json[end_pos] == '-' || json[end_pos] == '+') {
        ++end_pos;
    }
    const size_t digits_start = end_pos;
    while (end_pos < json.size() && std::isdigit(static_cast<unsigned char>(json[end_pos]))) {
        ++end_pos;
    }
    if (end_pos == digits_start) {
        return false;
    }
    const char* first = json.data() + ((json[val_pos] == '+') ? (val_pos + 1u) : val_pos);
    const char* last  = json.data() + end_pos;
    int64_t parsed = 0;
    const auto [ptr, ec] = std::from_chars(first, last, parsed);
    if (ec != std::errc{} || ptr != last) {
        return false;
    }
    out_value = parsed;
    return true;
}

[[nodiscard]] inline bool ExtractJsonBool(
    std::string_view json,
    std::string_view key,
    bool& out_value,
    size_t start_pos = 0u
) noexcept {
    const size_t val_pos = FindKeyValueStart(json, key, start_pos);
    if (val_pos == std::string_view::npos || val_pos >= json.size()) {
        return false;
    }
    if (json.substr(val_pos).starts_with("true")) {
        out_value = true;
        return true;
    }
    if (json.substr(val_pos).starts_with("false")) {
        out_value = false;
        return true;
    }
    return false;
}

[[nodiscard]] inline std::string_view ExtractBracketSlice(
    std::string_view json,
    std::string_view key,
    char open_ch,
    char close_ch
) noexcept {
    const size_t val_pos = FindKeyValueStart(json, key, 0u);
    if (val_pos == std::string_view::npos || val_pos >= json.size() || json[val_pos] != open_ch) {
        return {};
    }
    int depth = 0;
    bool in_str = false;
    for (size_t i = val_pos; i < json.size(); ++i) {
        const char c = json[i];
        if (c == '\\' && in_str) {
            ++i;
            continue;
        }
        if (c == '"') {
            in_str = !in_str;
            continue;
        }
        if (!in_str) {
            if (c == open_ch) {
                ++depth;
            } else if (c == close_ch) {
                --depth;
                if (depth == 0) {
                    return json.substr(val_pos, (i - val_pos) + 1u);
                }
            }
        }
    }
    return {};
}

inline void EscapeJsonString(std::string_view input, std::string& out) {
    for (char c : input) {
        if (c == '"') {
            out += "\\\"";
        } else if (c == '\\') {
            out += "\\\\";
        } else if (c == '\n') {
            out += "\\n";
        } else if (c == '\r') {
            out += "\\r";
        } else if (c == '\t') {
            out += "\\t";
        } else if (static_cast<unsigned char>(c) >= 0x20u) {
            out.push_back(c);
        }
    }
}

} // namespace detail

// ============================================================================
// 1. Lightweight Zero-Dependency JSON-LD Blueprint & Policy Loader
// ============================================================================

inline bool LoadBlueprintArtifactFromJsonLdString(
    const std::string& json_text,
    DualFidelityBlueprintArtifact& out_def
) {
    const std::string_view view(json_text);
    if (view.find('{') == std::string_view::npos || view.find('}') == std::string_view::npos) {
        return false;
    }

    std::string bp_id;
    if (!detail::ExtractJsonString(view, "blueprintId", bp_id)) {
        if (!detail::ExtractJsonString(view, "canonical_id", bp_id)) {
            return false;
        }
    }
    if (bp_id.empty()) {
        return false;
    }

    DualFidelityBlueprintArtifact def{};

    // Pre-seed known canonical Stage-1 blueprint defaults before applying JSON-LD fields
    ArtifactRegistry default_reg{};
    BootstrapDefaultStage1ArtifactCatalog(default_reg);
    if (const auto* existing = default_reg.FindBlueprintByName(bp_id)) {
        def = *existing;
        if (def.canonical_id[0] == '\0') {
            CopyCanonicalId(def.canonical_id, bp_id);
        }
    } else {
        def.blueprint_cid_hash = detail::HashFnv1a64(bp_id);
        def.treaty_cid_hash    = detail::HashFnv1a64(bp_id + "_treaty");
        def.workflow_cid_hash  = detail::HashFnv1a64(bp_id + "_workflow");
        def.scenario_id        = 99u;
        def.min_node_stage     = 1u;
        def.max_acoustic_cb    = 550u;
        def.required_vc_id     = 0u;
        def.uunaa_section_6_shielded = true;
        def.abstraction_tier   = SimulationAbstractionTier::TIER_2_MULTI_STAGE_SUBSET;
        CopyCanonicalId(def.canonical_id, bp_id);
    }

    std::string treaty_cid;
    if (detail::ExtractJsonString(view, "treatyCid", treaty_cid) && !treaty_cid.empty()) {
        def.treaty_cid_hash = detail::HashFnv1a64(treaty_cid);
    }

    std::string workflow_id;
    if (detail::ExtractJsonString(view, "workflowId", workflow_id) && !workflow_id.empty()) {
        def.workflow_cid_hash = detail::HashFnv1a64(workflow_id);
    }

    // Scenario reference classification
    const std::string_view scenario_slice = detail::ExtractBracketSlice(view, "scenarioRef", '[', ']');
    if (!scenario_slice.empty()) {
        if (scenario_slice.find("SCN-ZERO") != std::string_view::npos) {
            def.scenario_id = 0u;
        } else if (scenario_slice.find("SCN-ALPHA") != std::string_view::npos) {
            def.scenario_id = 1u;
        } else if (scenario_slice.find("SCN-BETA") != std::string_view::npos) {
            def.scenario_id = 2u;
        } else if (scenario_slice.find("SCN-GAMMA") != std::string_view::npos) {
            def.scenario_id = 3u;
        } else if (scenario_slice.find("SCN-DELTA") != std::string_view::npos) {
            def.scenario_id = 4u;
        }
    }

    // Stage
    int64_t stage_val = 0;
    if (detail::ExtractJsonInt64(view, "stage", stage_val) ||
        detail::ExtractJsonInt64(view, "min_node_stage", stage_val)) {
        def.min_node_stage = static_cast<uint8_t>(std::clamp<int64_t>(stage_val, 1, 4));
    }

    // Layer 5 Governance Policy
    int64_t acoustic_cb = 0;
    if (detail::ExtractJsonInt64(view, "perimeterAcousticCeilingCb", acoustic_cb) ||
        detail::ExtractJsonInt64(view, "max_acoustic_cb", acoustic_cb)) {
        def.max_acoustic_cb = static_cast<uint16_t>(std::clamp<int64_t>(acoustic_cb, 0, 1200));
    }

    int64_t dt_delta = 0;
    if (detail::ExtractJsonInt64(view, "dependencyRatioDeltaBp", dt_delta) ||
        detail::ExtractJsonInt64(view, "dt_delta_bp", dt_delta)) {
        def.dt_delta_bp = static_cast<int32_t>(dt_delta);
        def.dependency_ratio_delta_bp = static_cast<int32_t>(dt_delta);
    }

    int64_t rep_credit = 0;
    if (detail::ExtractJsonInt64(view, "reputationalWealthCreditBp", rep_credit) ||
        detail::ExtractJsonInt64(view, "rep_credit_bp", rep_credit)) {
        def.rep_credit_bp = static_cast<uint16_t>(std::clamp<int64_t>(rep_credit, 0, 10000));
        def.reputational_wealth_credit_bp = def.rep_credit_bp;
    }

    int64_t explicit_vc_id = 0;
    if (detail::ExtractJsonInt64(view, "requiredVcId", explicit_vc_id) ||
        detail::ExtractJsonInt64(view, "required_vc_id", explicit_vc_id)) {
        def.required_vc_id = static_cast<uint16_t>(std::clamp<int64_t>(explicit_vc_id, 0, 15));
    }

    // Physical Transduction Specs
    int64_t pv_mw = 0;
    if (detail::ExtractJsonInt64(view, "nominalPvPowerMilliwatts", pv_mw)) {
        def.net_electric_mw = pv_mw;
    }
    int64_t compute_mw = 0;
    if (detail::ExtractJsonInt64(view, "nominalComputePowerMilliwatts", compute_mw) && pv_mw == 0) {
        def.net_electric_mw = -compute_mw;
    }
    int64_t batch_kcal = 0;
    if (detail::ExtractJsonInt64(view, "caloricYieldKcalPerBatch", batch_kcal) && batch_kcal > 0) {
        def.daily_caloric_yield_kcal = static_cast<uint64_t>(batch_kcal);
    }

    // Bill of Materials & l7FallbackCostUsdCents
    uint64_t total_l7_cost_cents = 0ULL;
    const std::string_view bom_slice = detail::ExtractBracketSlice(view, "billOfMaterials", '[', ']');
    if (!bom_slice.empty()) {
        uint8_t parsed_bom_count = 0u;
        BomComponentSpec parsed_boms[4]{};

        size_t cursor = 0u;
        while (cursor < bom_slice.size()) {
            const size_t obj_start = bom_slice.find('{', cursor);
            if (obj_start == std::string_view::npos) {
                break;
            }
            int depth = 0;
            bool in_str = false;
            size_t obj_end = std::string_view::npos;
            for (size_t i = obj_start; i < bom_slice.size(); ++i) {
                const char c = bom_slice[i];
                if (c == '\\' && in_str) {
                    ++i;
                    continue;
                }
                if (c == '"') {
                    in_str = !in_str;
                    continue;
                }
                if (!in_str) {
                    if (c == '{') {
                        ++depth;
                    } else if (c == '}') {
                        --depth;
                        if (depth == 0) {
                            obj_end = i;
                            break;
                        }
                    }
                }
            }
            if (obj_end == std::string_view::npos) {
                break;
            }

            const std::string_view item_view = bom_slice.substr(obj_start, (obj_end - obj_start) + 1u);
            int64_t item_l7_cost = 0;
            if (detail::ExtractJsonInt64(item_view, "l7FallbackCostUsdCents", item_l7_cost) && item_l7_cost > 0) {
                total_l7_cost_cents += static_cast<uint64_t>(item_l7_cost);
            }

            if (parsed_bom_count < 4u) {
                BomComponentSpec spec{};
                std::string comp_id;
                if (detail::ExtractJsonString(item_view, "componentId", comp_id) && !comp_id.empty()) {
                    spec.component_cid_hash = detail::HashFnv1a64(comp_id);
                } else {
                    spec.component_cid_hash = detail::HashFnv1a64(bp_id) + parsed_bom_count + 1ULL;
                }
                int64_t mass_mg = 0;
                if (detail::ExtractJsonInt64(item_view, "massMilligrams", mass_mg) && mass_mg > 0) {
                    spec.required_mass_mg_or_units = static_cast<uint64_t>(mass_mg);
                }
                spec.l7_unit_cost_usd_cents = (item_l7_cost > 0) ? static_cast<uint64_t>(item_l7_cost) : 0ULL;
                spec.l7_lead_time_ticks = (spec.l7_unit_cost_usd_cents > 0ULL) ? 90u : 20u;
                spec.item_def_id = static_cast<uint16_t>(100u + parsed_bom_count + 1u);

                std::string src_ring;
                (void)detail::ExtractJsonString(item_view, "sourceRing", src_ring);
                if (spec.l7_unit_cost_usd_cents > 0ULL || src_ring.find("L7") != std::string::npos) {
                    spec.preferred_tier = BomSourceTier::L7_PROCUREMENT_FALLBACK;
                    spec.is_irreducible_l7_leaf = (spec.l7_unit_cost_usd_cents > 0ULL) ? 1u : 0u;
                } else if (src_ring.find("R3") != std::string::npos ||
                           src_ring.find("PEER") != std::string::npos ||
                           src_ring.find("MESH") != std::string::npos) {
                    spec.preferred_tier = BomSourceTier::MESH_PEER_BCC;
                    spec.is_irreducible_l7_leaf = 0u;
                } else {
                    spec.preferred_tier = BomSourceTier::LOCAL_STOCK_OR_FAB;
                    spec.is_irreducible_l7_leaf = 0u;
                }
                parsed_boms[parsed_bom_count++] = spec;
            }

            cursor = obj_end + 1u;
        }

        if (parsed_bom_count > 0u) {
            def.bom_count = parsed_bom_count;
            for (uint8_t i = 0u; i < 4u; ++i) {
                def.bom_dag[i] = (i < parsed_bom_count) ? parsed_boms[i] : BomComponentSpec{};
            }
        }
    } else {
        int64_t direct_l7_cost = 0;
        if (detail::ExtractJsonInt64(view, "l7FallbackCostUsdCents", direct_l7_cost) ||
            detail::ExtractJsonInt64(view, "l7_cost_cents", direct_l7_cost)) {
            if (direct_l7_cost >= 0) {
                total_l7_cost_cents = static_cast<uint64_t>(direct_l7_cost);
                def.bom_count = 1u;
                def.bom_dag[0] = BomComponentSpec{
                    detail::HashFnv1a64(bp_id + "_bom0"),
                    1000000ULL,
                    total_l7_cost_cents,
                    60u,
                    101u,
                    (total_l7_cost_cents > 0ULL)
                        ? BomSourceTier::L7_PROCUREMENT_FALLBACK
                        : BomSourceTier::LOCAL_STOCK_OR_FAB,
                    (total_l7_cost_cents > 0ULL) ? uint8_t{1u} : uint8_t{0u}
                };
            }
        }
    }

    if (total_l7_cost_cents > 0ULL || !bom_slice.empty()) {
        def.l7_cost_cents = total_l7_cost_cents;
        def.l7_fallback_cost_usd_cents = total_l7_cost_cents;
    }

    // Oasis Simulation Projection Fields
    int64_t designator_action = 0;
    if (detail::ExtractJsonInt64(view, "designatorActionId", designator_action) ||
        detail::ExtractJsonInt64(view, "designator_action_id", designator_action)) {
        def.designator_action_id = static_cast<uint16_t>(std::max<int64_t>(0, designator_action));
    }

    const std::string_view vox_slice = detail::ExtractBracketSlice(view, "voxelMaterialIds", '[', ']');
    if (!vox_slice.empty()) {
        uint8_t mat_count = 0u;
        size_t pos = 0u;
        while (pos < vox_slice.size() && mat_count < 4u) {
            while (pos < vox_slice.size() && !std::isdigit(static_cast<unsigned char>(vox_slice[pos]))) {
                ++pos;
            }
            if (pos >= vox_slice.size()) {
                break;
            }
            size_t end_d = pos;
            while (end_d < vox_slice.size() && std::isdigit(static_cast<unsigned char>(vox_slice[end_d]))) {
                ++end_d;
            }
            uint32_t m_val = 0u;
            const auto [ptr, ec] = std::from_chars(vox_slice.data() + pos, vox_slice.data() + end_d, m_val);
            if (ec == std::errc{} && ptr == (vox_slice.data() + end_d)) {
                def.voxel_material_ids[mat_count++] = static_cast<uint8_t>(std::min<uint32_t>(255u, m_val));
            }
            pos = end_d;
        }
        if (mat_count > 0u) {
            def.voxel_material_count = mat_count;
            def.target_voxel_mat_id  = def.voxel_material_ids[0];
        }
    } else {
        int64_t single_mat = 0;
        if (detail::ExtractJsonInt64(view, "target_voxel_mat_id", single_mat) && single_mat > 0) {
            def.target_voxel_mat_id   = static_cast<uint8_t>(std::min<int64_t>(255, single_mat));
            def.voxel_material_ids[0] = def.target_voxel_mat_id;
            def.voxel_material_count  = 1u;
        }
    }

    int64_t somatic_kcal = 0;
    if (detail::ExtractJsonInt64(view, "somaticCaloricCostKcal", somatic_kcal) ||
        detail::ExtractJsonInt64(view, "somatic_kcal_cost", somatic_kcal)) {
        def.somatic_kcal_cost         = static_cast<uint32_t>(std::max<int64_t>(0, somatic_kcal));
        def.somatic_caloric_cost_kcal = def.somatic_kcal_cost;
    }

    bool scraps_elec = false;
    if (detail::ExtractJsonBool(view, "scrapsLegacyElectricUtilityBaseline", scraps_elec) ||
        detail::ExtractJsonBool(view, "scraps_electric_utility_baseline", scraps_elec)) {
        def.scraps_electric_utility_baseline        = scraps_elec;
        def.scraps_legacy_electric_utility_baseline = scraps_elec;
    }

    bool scraps_groc = false;
    if (detail::ExtractJsonBool(view, "scrapsLegacyGroceryBillBaseline", scraps_groc) ||
        detail::ExtractJsonBool(view, "scraps_grocery_bill_baseline", scraps_groc)) {
        def.scraps_grocery_bill_baseline        = scraps_groc;
        def.scraps_legacy_grocery_bill_baseline = scraps_groc;
    }

    const std::string_view modes_slice = detail::ExtractBracketSlice(view, "supportedFidelityModes", '[', ']');
    if (!modes_slice.empty()) {
        uint8_t mask = 0u;
        if (modes_slice.find("MACRO_ABSTRACTED") != std::string_view::npos) {
            mask = static_cast<uint8_t>(mask | 0x01u);
        }
        if (modes_slice.find("MULTI_STAGE_SUBSET") != std::string_view::npos) {
            mask = static_cast<uint8_t>(mask | 0x02u);
        }
        if (modes_slice.find("FULL_STOICHIOMETRIC_HIL") != std::string_view::npos) {
            mask = static_cast<uint8_t>(mask | 0x04u);
        }
        if (mask != 0u) {
            def.supported_fidelity_modes = mask;
        }
    }

    std::string default_mode;
    if (detail::ExtractJsonString(view, "defaultFidelityMode", default_mode)) {
        if (default_mode == "MACRO_ABSTRACTED") {
            def.abstraction_tier = SimulationAbstractionTier::TIER_1_MACRO_ABSTRACTED;
        } else if (default_mode == "MULTI_STAGE_SUBSET") {
            def.abstraction_tier = SimulationAbstractionTier::TIER_2_MULTI_STAGE_SUBSET;
        } else if (default_mode == "FULL_STOICHIOMETRIC_HIL") {
            def.abstraction_tier = SimulationAbstractionTier::TIER_3_FULL_STOICHIOMETRIC_HIL;
        }
    } else if (!modes_slice.empty()) {
        if ((def.supported_fidelity_modes & 0x02u) != 0u) {
            def.abstraction_tier = SimulationAbstractionTier::TIER_2_MULTI_STAGE_SUBSET;
        } else if ((def.supported_fidelity_modes & 0x04u) != 0u) {
            def.abstraction_tier = SimulationAbstractionTier::TIER_3_FULL_STOICHIOMETRIC_HIL;
        } else if ((def.supported_fidelity_modes & 0x01u) != 0u) {
            def.abstraction_tier = SimulationAbstractionTier::TIER_1_MACRO_ABSTRACTED;
        }
    }

    // Parse customizable workflow steps & align somatic caloric cost and acoustic ceiling
    const std::string_view steps_slice = detail::ExtractBracketSlice(view, "customizableSteps", '[', ']');
    if (!steps_slice.empty()) {
        uint8_t parsed_steps = 0u;
        CustomizableWorkflowStepDef step_defs[4]{};
        size_t cursor = 0u;
        while (cursor < steps_slice.size() && parsed_steps < 4u) {
            const size_t obj_start = steps_slice.find('{', cursor);
            if (obj_start == std::string_view::npos) {
                break;
            }
            int depth = 0;
            bool in_str = false;
            size_t obj_end = std::string_view::npos;
            for (size_t i = obj_start; i < steps_slice.size(); ++i) {
                const char c = steps_slice[i];
                if (c == '\\' && in_str) {
                    ++i;
                    continue;
                }
                if (c == '"') {
                    in_str = !in_str;
                    continue;
                }
                if (!in_str) {
                    if (c == '{') {
                        ++depth;
                    } else if (c == '}') {
                        --depth;
                        if (depth == 0) {
                            obj_end = i;
                            break;
                        }
                    }
                }
            }
            if (obj_end == std::string_view::npos) {
                break;
            }
            const std::string_view s_view = steps_slice.substr(obj_start, (obj_end - obj_start) + 1u);
            CustomizableWorkflowStepDef sdef{};
            std::string step_id_str;
            if (detail::ExtractJsonString(s_view, "stepId", step_id_str) && !step_id_str.empty()) {
                sdef.step_id_hash = detail::HashFnv1a64(step_id_str);
            } else {
                sdef.step_id_hash = detail::HashFnv1a64(bp_id) + 100ULL + parsed_steps;
            }
            sdef.duration_ticks       = 90u;
            sdef.focus_drain_bp       = 250u;
            sdef.acoustic_emission_cb = std::min<uint16_t>(320u, def.max_acoustic_cb);
            sdef.required_tool_def_id = static_cast<uint16_t>(parsed_steps + 1u);
            sdef.required_vc_id       = def.required_vc_id;

            std::string traversal;
            (void)detail::ExtractJsonString(s_view, "layerTraversal", traversal);
            if (traversal.find("L7") != std::string::npos) {
                sdef.actor_mode = WorkflowStepActorMode::UPWARD_L7_PROCUREMENT;
                sdef.can_automate_with_l2_kit = 1u;
            } else {
                sdef.actor_mode = WorkflowStepActorMode::HUMAN_STEWARD;
                sdef.can_automate_with_l2_kit = 0u;
            }
            step_defs[parsed_steps++] = sdef;
            cursor = obj_end + 1u;
        }
        if (parsed_steps > 0u) {
            def.step_count = parsed_steps;
            for (uint8_t i = 0u; i < 4u; ++i) {
                def.workflow_steps[i] = (i < parsed_steps) ? step_defs[i] : CustomizableWorkflowStepDef{};
            }
        }
    }

    if (def.step_count == 0u) {
        def.step_count = 2u;
        def.workflow_steps[0] = CustomizableWorkflowStepDef{
            detail::HashFnv1a64(bp_id + "_step0"),
            60u,
            def.somatic_kcal_cost / 2u,
            200u,
            std::min<uint16_t>(300u, def.max_acoustic_cb),
            1u,
            def.required_vc_id,
            WorkflowStepActorMode::UPWARD_L7_PROCUREMENT,
            1u,
            {}
        };
        def.workflow_steps[1] = CustomizableWorkflowStepDef{
            detail::HashFnv1a64(bp_id + "_step1"),
            120u,
            def.somatic_kcal_cost - (def.somatic_kcal_cost / 2u),
            300u,
            std::min<uint16_t>(340u, def.max_acoustic_cb),
            2u,
            def.required_vc_id,
            WorkflowStepActorMode::HUMAN_STEWARD,
            0u,
            {}
        };
    } else if (def.somatic_kcal_cost > 0u) {
        const uint8_t n_steps = std::min<uint8_t>(def.step_count, 4u);
        const uint32_t per_step = def.somatic_kcal_cost / n_steps;
        uint32_t remainder = def.somatic_kcal_cost - (per_step * n_steps);
        for (uint8_t i = 0u; i < n_steps; ++i) {
            def.workflow_steps[i].somatic_kcal_cost = per_step + ((i == 0u) ? remainder : 0u);
            def.workflow_steps[i].acoustic_emission_cb = std::min<uint16_t>(
                def.workflow_steps[i].acoustic_emission_cb,
                def.max_acoustic_cb
            );
        }
    }

    out_def = def;
    return true;
}

inline bool LoadBlueprintArtifactFromJsonLdString(
    ArtifactRegistry& reg,
    const std::string& json_text,
    DualFidelityBlueprintArtifact* out_def = nullptr
) {
    DualFidelityBlueprintArtifact parsed{};
    if (!LoadBlueprintArtifactFromJsonLdString(json_text, parsed)) {
        return false;
    }
    if (!RegisterBlueprintArtifact(reg, parsed)) {
        return false;
    }
    if (out_def != nullptr) {
        *out_def = parsed;
    }
    return true;
}

inline uint32_t LoadBlueprintCatalogFromDirectory(
    ArtifactRegistry& reg,
    const std::string& dir_path = "core/6_intent/blueprints"
) {
#ifdef __EMSCRIPTEN__
    (void)dir_path;
    if (reg.blueprint_count == 0u) {
        BootstrapDefaultStage1ArtifactCatalog(reg);
    }
    return reg.blueprint_count;
#else
    std::filesystem::path resolved_dir(dir_path);
    std::error_code ec;
    if (!std::filesystem::exists(resolved_dir, ec) || !std::filesystem::is_directory(resolved_dir, ec)) {
        const std::array<std::filesystem::path, 6> fallbacks = {
            std::filesystem::path("core/6_intent/blueprints"),
            std::filesystem::path("../core/6_intent/blueprints"),
            std::filesystem::path("../../core/6_intent/blueprints"),
            std::filesystem::path("../../../core/6_intent/blueprints"),
            std::filesystem::path("../../../../core/6_intent/blueprints"),
            std::filesystem::path("../") / dir_path
        };
        bool found = false;
        if (dir_path.find("6_intent/blueprints") != std::string::npos) {
            for (const auto& candidate : fallbacks) {
                if (std::filesystem::exists(candidate, ec) && std::filesystem::is_directory(candidate, ec)) {
                    resolved_dir = candidate;
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            if (reg.blueprint_count == 0u) {
                BootstrapDefaultStage1ArtifactCatalog(reg);
            }
            return reg.blueprint_count;
        }
    }

    std::vector<std::filesystem::path> bp_files;
    for (const auto& entry : std::filesystem::directory_iterator(resolved_dir, ec)) {
        if (ec || !entry.is_regular_file(ec)) {
            continue;
        }
        const std::string fname = entry.path().filename().string();
        if (fname.starts_with("bp_") && fname.ends_with(".jsonld")) {
            bp_files.push_back(entry.path());
        }
    }
    std::sort(bp_files.begin(), bp_files.end());

    uint32_t loaded_count = 0u;
    for (const auto& fpath : bp_files) {
        std::ifstream ifs(fpath);
        if (!ifs.is_open()) {
            continue;
        }
        std::ostringstream oss;
        oss << ifs.rdbuf();
        DualFidelityBlueprintArtifact bp{};
        if (LoadBlueprintArtifactFromJsonLdString(oss.str(), bp)) {
            if (RegisterBlueprintArtifact(reg, bp)) {
                ++loaded_count;
            }
        }
    }

    if (loaded_count == 0u && reg.blueprint_count == 0u) {
        BootstrapDefaultStage1ArtifactCatalog(reg);
        return reg.blueprint_count;
    }
    return loaded_count;
#endif
}

// ============================================================================
// 2. Dynamic Artifact Catalog JSON Serializer for WebGPU UI & col-gatewayd
// ============================================================================

[[nodiscard]] inline std::string SerializeArtifactCatalogToJson(const ArtifactRegistry& reg) {
    std::string out;
    out.reserve(1024u);
    out.push_back('[');

    const uint32_t count = std::min<uint32_t>(reg.blueprint_count, static_cast<uint32_t>(ArtifactRegistry::MAX_BLUEPRINTS));
    for (uint32_t i = 0u; i < count; ++i) {
        const auto& bp = reg.blueprints[i];
        if (i > 0u) {
            out.push_back(',');
        }
        const uint32_t def_id = (bp.blueprint_def_id > 0u) ? static_cast<uint32_t>(bp.blueprint_def_id) : (i + 1u);
        const uint64_t l7_cents = bp.EffectiveL7CostCents();
        const int32_t dt_bp = bp.EffectiveDtDeltaBp();
        const uint16_t rep_bp = bp.EffectiveRepCreditBp();
        const uint32_t kcal_cost = bp.EffectiveSomaticKcalCost();

        out += "{\"blueprint_def_id\":";
        out += std::to_string(def_id);
        out += ",\"canonical_id\":\"";
        detail::EscapeJsonString(std::string_view(bp.canonical_id), out);
        out += "\",\"min_node_stage\":";
        out += std::to_string(static_cast<unsigned>(bp.min_node_stage));
        out += ",\"abstraction_tier\":";
        out += std::to_string(static_cast<unsigned>(bp.abstraction_tier));
        out += ",\"l7_cost_cents\":";
        out += std::to_string(l7_cents);
        out += ",\"max_acoustic_cb\":";
        out += std::to_string(static_cast<unsigned>(bp.max_acoustic_cb));
        out += ",\"dt_delta_bp\":";
        out += std::to_string(dt_bp);
        out += ",\"rep_credit_bp\":";
        out += std::to_string(static_cast<unsigned>(rep_bp));
        out += ",\"target_voxel_mat_id\":";
        out += std::to_string(static_cast<unsigned>(bp.target_voxel_mat_id));
        out += ",\"designator_action_id\":";
        out += std::to_string(static_cast<unsigned>(bp.designator_action_id));
        out += ",\"somatic_kcal_cost\":";
        out += std::to_string(kcal_cost);
        out += ",\"scraps_electric_utility_baseline\":";
        out += (bp.scraps_electric_utility_baseline || bp.scraps_legacy_electric_utility_baseline) ? "true" : "false";
        out += ",\"scraps_grocery_bill_baseline\":";
        out += (bp.scraps_grocery_bill_baseline || bp.scraps_legacy_grocery_bill_baseline) ? "true" : "false";
        out.push_back('}');
    }

    out.push_back(']');
    return out;
}

} // namespace oasis::artifacts
