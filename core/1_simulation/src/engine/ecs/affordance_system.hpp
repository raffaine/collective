#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../content/content_registry.hpp"
#include "../domains/crop_cultivar.hpp"
#include "../domains/soil_physics.hpp"
#include "../domains/thermal_vessel.hpp"
#include "../logistics/hauling_physics.hpp"

namespace oasis::engine {

inline constexpr std::string_view kAffordanceLookId    = "affordance.look";
inline constexpr std::string_view kAffordanceInspectId = "affordance.inspect";

struct InventoryStack {
    std::string item_id{};
    uint32_t    quantity{0u};
    uint32_t    reserved_{0u};
};

struct InventoryComponent {
    std::vector<InventoryStack> stacks{};
    uint32_t                    max_capacity_mass_g{1000000u};
    uint32_t                    reserved_{0u};

    void AddItem(std::string_view item_id, uint32_t quantity) {
        if (item_id.empty() || quantity == 0u) {
            return;
        }
        for (auto& s : stacks) {
            if (s.item_id == item_id) {
                s.quantity += quantity;
                return;
            }
        }
        stacks.push_back(InventoryStack{std::string(item_id), quantity, 0u});
    }

    bool RemoveItem(std::string_view item_id, uint32_t quantity) noexcept {
        if (quantity == 0u) {
            return true;
        }
        for (size_t i = 0u; i < stacks.size(); ++i) {
            if (stacks[i].item_id == item_id) {
                if (stacks[i].quantity < quantity) {
                    return false;
                }
                stacks[i].quantity -= quantity;
                if (stacks[i].quantity == 0u) {
                    stacks.erase(stacks.begin() + static_cast<std::ptrdiff_t>(i));
                }
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] uint32_t CountItem(std::string_view item_id) const noexcept {
        for (const auto& s : stacks) {
            if (s.item_id == item_id) {
                return s.quantity;
            }
        }
        return 0u;
    }

    [[nodiscard]] bool HasItem(std::string_view item_id, uint32_t min_qty = 1u) const noexcept {
        return CountItem(item_id) >= min_qty;
    }

    [[nodiscard]] uint64_t TotalItemUnits() const noexcept {
        uint64_t sum = 0ULL;
        for (const auto& s : stacks) {
            sum += static_cast<uint64_t>(s.quantity);
        }
        return sum;
    }

    [[nodiscard]] uint64_t TotalMassGrams(const ContentRegistry& registry) const noexcept {
        uint64_t mass_g = 0ULL;
        for (const auto& s : stacks) {
            const ItemDef* def = registry.FindItem(s.item_id);
            const uint32_t unit_g = (def != nullptr) ? def->unit_mass_g : 0u;
            mass_g += static_cast<uint64_t>(unit_g) * static_cast<uint64_t>(s.quantity);
        }
        return mass_g;
    }
};

struct alignas(8) PropertyEntry {
    int64_t     value{0};
    std::string key{};
    std::string unit_hint{};
};

struct PropertyBagComponent {
    std::vector<PropertyEntry> entries{};

    void Set(std::string_view key, int64_t value, std::string_view unit_hint = "") {
        for (auto& e : entries) {
            if (e.key == key) {
                e.value = value;
                if (!unit_hint.empty()) {
                    e.unit_hint = std::string(unit_hint);
                }
                return;
            }
        }
        entries.push_back(PropertyEntry{value, std::string(key), std::string(unit_hint)});
    }

    [[nodiscard]] bool TryGet(std::string_view key, int64_t& out_value) const noexcept {
        for (const auto& e : entries) {
            if (e.key == key) {
                out_value = e.value;
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] int64_t Get(std::string_view key, int64_t fallback = 0) const noexcept {
        int64_t v = fallback;
        (void)TryGet(key, v);
        return v;
    }

    [[nodiscard]] bool Has(std::string_view key) const noexcept {
        int64_t unused = 0;
        return TryGet(key, unused);
    }

    int64_t MutateDelta(std::string_view key, int64_t delta) {
        for (auto& e : entries) {
            if (e.key == key) {
                e.value += delta;
                return e.value;
            }
        }
        entries.push_back(PropertyEntry{delta, std::string(key), ""});
        return delta;
    }
};

struct AgentCapabilitiesComponent {
    std::vector<std::string> capability_tags{};

    void GrantCapability(std::string_view tag) {
        if (tag.empty() || HasCapability(tag)) {
            return;
        }
        capability_tags.emplace_back(tag);
    }

    void RevokeCapability(std::string_view tag) noexcept {
        const auto it = std::remove(capability_tags.begin(), capability_tags.end(), tag);
        capability_tags.erase(it, capability_tags.end());
    }

    [[nodiscard]] bool HasCapability(std::string_view tag) const noexcept {
        if (tag.empty()) {
            return true;
        }
        for (const auto& t : capability_tags) {
            if (t == tag) {
                return true;
            }
        }
        return false;
    }
};

struct alignas(8) SensorAttachmentComponent {
    uint32_t sensor_node_id{0u};
    uint16_t poll_interval_sim_s{10u};
    bool     has_l2_sensor{false};
    bool     actuator_enabled{true};
};
static_assert(sizeof(SensorAttachmentComponent) == 8, "SensorAttachmentComponent must be 8 bytes with zero implicit padding");

struct EquippedHaulingToolComponent {
    std::string tool_id{};
    std::string source_rack_entity_id{};

    [[nodiscard]] const HaulingToolDef* ResolveTool(const ContentRegistry& registry) const noexcept {
        if (tool_id.empty()) {
            return nullptr;
        }
        return registry.FindHaulingTool(tool_id);
    }
};

struct ManualInspectionReport {
    std::string                headline{};
    std::string                look_description{};
    std::vector<PropertyEntry> revealed_properties{};
    std::vector<InventoryStack> revealed_inventory{};
    bool                       has_live_l2_telemetry{false};
    uint8_t                    pad_[7]{0u, 0u, 0u, 0u, 0u, 0u, 0u};
};

namespace detail {

[[nodiscard]] inline bool ResolveEntityPropertyValue(
    std::string_view key,
    const PropertyBagComponent* props,
    const SoilCellRegisters* soil,
    const ThermalVesselRegisters* vessel,
    const InventoryComponent* target_inventory,
    int64_t& out_val
) noexcept {
    if (props != nullptr && props->TryGet(key, out_val)) {
        return true;
    }
    if (soil != nullptr) {
        if (key == "moisture_vwc_bp")   { out_val = static_cast<int64_t>(soil->moisture_vwc_bp); return true; }
        if (key == "soil_temp_mk")      { out_val = static_cast<int64_t>(soil->soil_temp_mk); return true; }
        if (key == "nitrogen_ppm")      { out_val = static_cast<int64_t>(soil->nitrogen_ppm); return true; }
        if (key == "phosphorus_ppm")    { out_val = static_cast<int64_t>(soil->phosphorus_ppm); return true; }
        if (key == "potassium_ppm")     { out_val = static_cast<int64_t>(soil->potassium_ppm); return true; }
        if (key == "organic_carbon_bp") { out_val = static_cast<int64_t>(soil->organic_carbon_bp); return true; }
        if (key == "ph_centi")          { out_val = static_cast<int64_t>(soil->ph_centi); return true; }
        if (key == "crop_growth_bp")    { out_val = static_cast<int64_t>(soil->crop_growth_bp); return true; }
        if (key == "crop_planted")      { out_val = static_cast<int64_t>(soil->crop_planted); return true; }
        if (key == "is_legume_fixing_n"){ out_val = static_cast<int64_t>(soil->is_legume_fixing_n); return true; }
    }
    if (vessel != nullptr) {
        if (key == "core_temp_mk")          { out_val = static_cast<int64_t>(vessel->core_temp_mk); return true; }
        if (key == "heater_power_mw")       { out_val = static_cast<int64_t>(vessel->heater_power_mw); return true; }
        if (key == "liquid_volume_ml")      { out_val = static_cast<int64_t>(vessel->liquid_volume_ml); return true; }
        if (key == "pasteurization_hold_s") { out_val = static_cast<int64_t>(vessel->pasteurization_hold_s); return true; }
        if (key == "lid_closed")            { out_val = static_cast<int64_t>(vessel->lid_closed); return true; }
    }
    if (target_inventory != nullptr) {
        if (key == "egg_count") {
            out_val = static_cast<int64_t>(target_inventory->CountItem("fresh_egg"));
            return true;
        }
        if (key == "item_units") {
            out_val = static_cast<int64_t>(target_inventory->TotalItemUnits());
            return true;
        }
    }
    return false;
}

[[nodiscard]] inline bool PassesAffordanceHaulingCheck(
    const ContentRegistry& registry,
    const AffordanceDef& aff,
    const HaulingToolDef* equipped_tool,
    bool path_is_paved
) noexcept {
    if (!aff.input_item_id.empty() && aff.input_qty > 0u) {
        const ItemDef* in_item = registry.FindItem(aff.input_item_id);
        if (in_item == nullptr) {
            return false;
        }
        const HaulEvaluation eval = EvaluateHaulPlan(*in_item, aff.input_qty, equipped_tool, path_is_paved);
        if (!IsHaulPossible(eval.feasibility)) {
            return false;
        }
    }
    if (!aff.output_item_id.empty() && aff.output_qty > 0u) {
        const ItemDef* out_item = registry.FindItem(aff.output_item_id);
        if (out_item != nullptr &&
            (out_item->matter_phase == MatterPhase::LIQUID ||
             out_item->unit_mass_g > kBarehandMaxSolidLiftMassG)) {
            const HaulEvaluation eval = EvaluateHaulPlan(*out_item, aff.output_qty, equipped_tool, path_is_paved);
            if (!IsHaulPossible(eval.feasibility)) {
                return false;
            }
        }
    }
    return true;
}

} // namespace detail

// Queries available affordances for an entity: ALWAYS includes universal affordance.look and affordance.inspect,
// then filters EntityArchetypeDef.affordance_ids by property preconditions, agent capabilities, and hauling laws.
[[nodiscard]] inline std::vector<AffordanceDef> QueryEntityAffordances(
    const ContentRegistry& registry,
    const EntityArchetypeDef& archetype,
    const PropertyBagComponent* target_props = nullptr,
    const AgentCapabilitiesComponent* agent_caps = nullptr,
    const EquippedHaulingToolComponent* equipped_tool_comp = nullptr,
    bool path_is_paved = true,
    const SoilCellRegisters* target_soil = nullptr,
    const ThermalVesselRegisters* target_vessel = nullptr,
    const InventoryComponent* target_inventory = nullptr
) {
    std::vector<AffordanceDef> out;
    out.reserve(2u + archetype.affordance_ids.size());

    // 1. Always include universal Look and Inspect affordances first
    if (const AffordanceDef* look_def = registry.FindAffordance(kAffordanceLookId); look_def != nullptr) {
        out.push_back(*look_def);
    } else {
        AffordanceDef fallback_look{};
        fallback_look.affordance_id = std::string(kAffordanceLookId);
        fallback_look.label = "Look";
        out.push_back(std::move(fallback_look));
    }

    if (const AffordanceDef* inspect_def = registry.FindAffordance(kAffordanceInspectId); inspect_def != nullptr) {
        out.push_back(*inspect_def);
    } else {
        AffordanceDef fallback_inspect{};
        fallback_inspect.affordance_id = std::string(kAffordanceInspectId);
        fallback_inspect.label = "Inspect Closely";
        fallback_inspect.duration_sim_s = 30u;
        out.push_back(std::move(fallback_inspect));
    }

    const HaulingToolDef* equipped_tool = (equipped_tool_comp != nullptr)
        ? equipped_tool_comp->ResolveTool(registry)
        : nullptr;

    // 2. Filter archetype-specific affordances by capability, property range, and hauling tool feasibility
    for (const std::string& aff_id : archetype.affordance_ids) {
        if (aff_id == kAffordanceLookId || aff_id == kAffordanceInspectId) {
            continue;
        }
        const AffordanceDef* aff = registry.FindAffordance(aff_id);
        if (aff == nullptr) {
            continue;
        }

        if (!aff->required_capability_tag.empty()) {
            if (agent_caps == nullptr || !agent_caps->HasCapability(aff->required_capability_tag)) {
                continue;
            }
        }

        if (!aff->required_property_key.empty()) {
            int64_t current_val = 0;
            if (!detail::ResolveEntityPropertyValue(
                    aff->required_property_key, target_props, target_soil, target_vessel, target_inventory, current_val)) {
                continue;
            }
            if (current_val < aff->min_property_val || current_val > aff->max_property_val) {
                continue;
            }
        }

        if (!detail::PassesAffordanceHaulingCheck(registry, *aff, equipped_tool, path_is_paved)) {
            continue;
        }

        out.push_back(*aff);
    }

    return out;
}

// Performs manual diegetic inspection (affordance.inspect), revealing PropertyBag, SoilCellRegisters,
// ThermalVesselRegisters, and InventoryComponent even when no L2 hardware sensor is attached.
[[nodiscard]] inline ManualInspectionReport InspectEntityManual(
    const EntityArchetypeDef& archetype,
    const PropertyBagComponent* props = nullptr,
    const SoilCellRegisters* soil = nullptr,
    const ThermalVesselRegisters* vessel = nullptr,
    const InventoryComponent* inventory = nullptr,
    const SensorAttachmentComponent* sensor = nullptr
) {
    ManualInspectionReport rep{};
    rep.headline = archetype.display_name;
    rep.look_description = archetype.look_description;
    rep.has_live_l2_telemetry = (sensor != nullptr && sensor->has_l2_sensor);

    if (props != nullptr) {
        for (const auto& entry : props->entries) {
            rep.revealed_properties.push_back(entry);
        }
    }
    if (soil != nullptr) {
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil->moisture_vwc_bp), "moisture_vwc_bp", "bp"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil->soil_temp_mk), "soil_temp_mk", "mK"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil->nitrogen_ppm), "nitrogen_ppm", "ppm"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil->phosphorus_ppm), "phosphorus_ppm", "ppm"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil->potassium_ppm), "potassium_ppm", "ppm"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil->organic_carbon_bp), "organic_carbon_bp", "bp"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil->ph_centi), "ph_centi", "pH*100"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil->crop_growth_bp), "crop_growth_bp", "bp"});
    }
    if (vessel != nullptr) {
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(vessel->core_temp_mk), "core_temp_mk", "mK"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(vessel->liquid_volume_ml), "liquid_volume_ml", "ml"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(vessel->pasteurization_hold_s), "pasteurization_hold_s", "s"});
    }
    if (inventory != nullptr) {
        rep.revealed_inventory = inventory->stacks;
    }
    return rep;
}

// Skill-filtered epistemic garden inspection:
// - Without L2 soil_probe_v1, a NOVICE (Tier 1) can tell whether plants are perky, stressed/yellowing, or dead/diseased,
//   whether weeds/insects are visible, and whether the top knuckle of soil feels dry or damp,
//   but CANNOT distinguish individual N-P-K nutrient symptoms or soil organic-carbon insulation / flow bottlenecks!
// - An EXPERIENCED (Tier 2) gardener identifies foliar N/P/K symptoms and 10 cm crumb ribbon thermal/hydraulic properties.
// - L2_AUTOMATED (Tier 3, has_l2_sensor == true) reads exact fixed-point registers.
[[nodiscard]] inline ManualInspectionReport InspectGardenBedEpistemic(
    const EntityArchetypeDef&        archetype,
    const SoilCellRegisters&         soil,
    const GardenBedHealthState&      health,
    const CultivarPhysicsSpec&       cultivar,
    const SensorAttachmentComponent* sensor = nullptr
) {
    ManualInspectionReport rep{};
    rep.has_live_l2_telemetry = (sensor != nullptr && sensor->has_l2_sensor);

    const AgronomySkillTier effective_tier = rep.has_live_l2_telemetry
        ? AgronomySkillTier::L2_AUTOMATED
        : health.skill_tier;
    const uint16_t uncertainty_bp = DiagnosticUncertaintyBpForSkill(effective_tier);
    const GardenIssueCode true_issue = EvaluateGardenBedTrueIssue(
        soil, cultivar, health.crop_health_bp, health.weed_pressure_bp, health.pest_pressure_bp
    );
    const GardenIssueCode diag_issue = FilterIssueByEpistemicSkill(true_issue, effective_tier);

    rep.headline = std::string(CultivarDisplayNameFromCode(cultivar.cultivar_code)) + " — " +
                   std::string(PhenologyStageLabel(EvaluateCropPhenologyStage(
                       soil.crop_growth_bp, soil.crop_planted, cultivar.harvest_mature_bp)));

    if (effective_tier == AgronomySkillTier::NOVICE) {
        // Coarse novice visual/tactile cues only!
        const int64_t plant_condition_tier =
            (health.crop_health_bp <= 3000u) ? 1 : ((health.crop_health_bp < 7500u || true_issue != GardenIssueCode::NONE) ? 2 : 3);
        const int64_t surface_moisture_tier =
            (soil.moisture_vwc_bp < 2400u) ? 1 : ((soil.moisture_vwc_bp <= 4900u) ? 2 : 3);

        if (true_issue == GardenIssueCode::NUTRIENT_DEFICIENCY ||
            true_issue == GardenIssueCode::SOIL_INSULATION_FLOW) {
            rep.look_description =
                "Novice Inspection (±28% uncertainty): Foliage looks somewhat pale and slow-growing, "
                "but without agronomy training or an L2 soil_probe_v1 you cannot tell whether it lacks specific nutrients (N/P/K) "
                "or if the soil lacks biochar thermal insulation and water/nutrient flow.";
        } else if (diag_issue != GardenIssueCode::NONE) {
            rep.look_description =
                "Novice Inspection (±28% uncertainty): Spotted visible issue — " +
                std::string(GardenIssueDisplayName(diag_issue)) + ".";
        } else {
            rep.look_description =
                "Novice Inspection (±28% uncertainty): Vines look green and alive; top knuckle of soil feels " +
                std::string(surface_moisture_tier == 1 ? "dry." : "cool and damp.");
        }

        rep.revealed_properties.push_back(PropertyEntry{plant_condition_tier, "novice_plant_condition_tier", "1=diseased..3=healthy"});
        rep.revealed_properties.push_back(PropertyEntry{surface_moisture_tier, "novice_surface_moisture_tier", "1=dry..3=wet"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.crop_growth_bp), "crop_growth_bp", "bp"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(health.weed_pressure_bp), "weed_pressure_bp", "bp"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(health.pest_pressure_bp), "pest_pressure_bp", "bp"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(diag_issue), "diagnosed_issue_code", "code"});
        rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(uncertainty_bp), "diagnostic_uncertainty_bp", "bp"});
        return rep;
    }

    // Experienced Agronomist (Tier 2) or L2 Automated (Tier 3): reveals full nutrient & soil-insulation diagnostics!
    rep.look_description =
        (effective_tier == AgronomySkillTier::L2_AUTOMATED ? "L2 Probe Telemetry (±1.5%): " : "Experienced Agronomist Diagnosis (±9%): ") +
        std::string(GardenIssueDisplayName(diag_issue)) + " · Health " +
        std::to_string(health.crop_health_bp / 100u) + "% · Soil Carbon Insulation " +
        std::to_string(soil.organic_carbon_bp / 100u) + "%.";

    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.moisture_vwc_bp), "moisture_vwc_bp", "bp"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.soil_temp_mk), "soil_temp_mk", "mK"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.nitrogen_ppm), "nitrogen_ppm", "ppm"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.phosphorus_ppm), "phosphorus_ppm", "ppm"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.potassium_ppm), "potassium_ppm", "ppm"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.organic_carbon_bp), "organic_carbon_bp", "bp"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.ph_centi), "ph_centi", "pH*100"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(soil.crop_growth_bp), "crop_growth_bp", "bp"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(health.crop_health_bp), "crop_health_bp", "bp"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(health.care_quality_score_bp), "care_quality_score_bp", "bp"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(diag_issue), "diagnosed_issue_code", "code"});
    rep.revealed_properties.push_back(PropertyEntry{static_cast<int64_t>(uncertainty_bp), "diagnostic_uncertainty_bp", "bp"});
    (void)archetype;
    return rep;
}

} // namespace oasis::engine

