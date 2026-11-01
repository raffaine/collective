#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace oasis::engine {

enum class MatterPhase : uint8_t {
    DISCRETE_SOLID       = 0,
    GRANULAR_PARTICULATE = 1,
    LIQUID               = 2
};

enum class HaulingToolClass : uint8_t {
    NONE_BARE_HANDS         = 0,
    LIQUID_VESSEL           = 1,
    GRANULAR_BUCKET_OR_SACK = 2,
    WHEELBARROW             = 3,
    HAND_DOLLY              = 4,
    CARGO_ETRIKE            = 5,
    FORKLIFT                = 6
};

struct alignas(8) ItemDef {
    uint64_t    exergy_per_unit_uj{0ULL};
    std::string item_id{};
    std::string display_name{};
    uint32_t    unit_mass_g{0u};
    uint32_t    unit_volume_ml{0u};
    uint32_t    caloric_kcal{0u};
    MatterPhase matter_phase{MatterPhase::DISCRETE_SOLID};
    uint8_t     pad_[3]{0u, 0u, 0u};
};

struct alignas(8) HaulingToolDef {
    std::string      tool_id{};
    std::string      display_name{};
    uint32_t         max_payload_mass_g{0u};
    uint32_t         max_liquid_volume_ml{0u};
    uint32_t         max_granular_mass_g{0u};
    uint32_t         speed_multiplier_bp{10000u};
    HaulingToolClass tool_class{HaulingToolClass::NONE_BARE_HANDS};
    bool             requires_paved_or_graded_path{false};
    uint8_t          pad_[6]{0u, 0u, 0u, 0u, 0u, 0u};
};

struct alignas(8) AffordanceDef {
    int64_t     min_property_val{std::numeric_limits<int64_t>::min()};
    int64_t     max_property_val{std::numeric_limits<int64_t>::max()};
    int64_t     target_property_delta{0};
    std::string affordance_id{};
    std::string label{};
    std::string required_capability_tag{};
    std::string required_property_key{};
    std::string input_item_id{};
    std::string output_item_id{};
    std::string target_property_mut_key{};
    std::string drive_satisfied{};
    uint32_t    input_qty{0u};
    uint32_t    output_qty{0u};
    uint32_t    duration_sim_s{0u};
    int32_t     drive_delta_bp{0};
};

struct alignas(8) EntityArchetypeDef {
    std::string              archetype_id{};
    std::string              display_name{};
    std::string              look_description{};
    std::string              category_tag{};
    std::vector<std::string> inspect_property_keys{};
    std::vector<std::string> affordance_ids{};
    uint16_t                 voxel_material_id{0u};
    bool                     is_passable{false};
    bool                     is_airtight{false};
    uint8_t                  pad_[4]{0u, 0u, 0u, 0u};
};

struct alignas(8) ThoughtRuleDef {
    std::string rule_id{};
    std::string drive_key{};
    std::string activity_tag{};
    std::string thought_text{};
    int32_t     min_drive_bp{0};
    int32_t     max_drive_bp{10000};
    int32_t     mood_delta_bp{0};
    uint32_t    cooldown_sim_s{600u};
};

namespace json_util {

[[nodiscard]] inline size_t SkipWs(std::string_view s, size_t pos) noexcept {
    while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos])) != 0) {
        ++pos;
    }
    return pos;
}

[[nodiscard]] inline size_t FindMatchingBracket(std::string_view s, size_t open_pos, char open_ch, char close_ch) noexcept {
    if (open_pos >= s.size() || s[open_pos] != open_ch) {
        return std::string_view::npos;
    }
    int depth = 0;
    bool in_str = false;
    for (size_t i = open_pos; i < s.size(); ++i) {
        const char c = s[i];
        if (in_str) {
            if (c == '\\' && (i + 1u) < s.size()) {
                ++i;
                continue;
            }
            if (c == '"') {
                in_str = false;
            }
            continue;
        }
        if (c == '"') {
            in_str = true;
        } else if (c == open_ch) {
            ++depth;
        } else if (c == close_ch) {
            --depth;
            if (depth == 0) {
                return i;
            }
        }
    }
    return std::string_view::npos;
}

[[nodiscard]] inline size_t FindKeyValPos(std::string_view obj, std::string_view key) noexcept {
    size_t i = 0u;
    int depth = 0;
    bool in_str = false;
    while (i < obj.size()) {
        const char c = obj[i];
        if (in_str) {
            if (c == '\\' && (i + 1u) < obj.size()) {
                i += 2u;
                continue;
            }
            if (c == '"') {
                in_str = false;
            }
            ++i;
            continue;
        }
        if (c == '{' || c == '[') {
            ++depth;
            ++i;
            continue;
        }
        if (c == '}' || c == ']') {
            --depth;
            ++i;
            continue;
        }
        if (c == '"' && depth <= 1) {
            const size_t key_start = i + 1u;
            size_t key_end = key_start;
            while (key_end < obj.size() && obj[key_end] != '"') {
                if (obj[key_end] == '\\' && (key_end + 1u) < obj.size()) {
                    key_end += 2u;
                } else {
                    ++key_end;
                }
            }
            if (key_end >= obj.size()) {
                return std::string_view::npos;
            }
            const std::string_view candidate = obj.substr(key_start, key_end - key_start);
            const size_t colon = SkipWs(obj, key_end + 1u);
            if (colon < obj.size() && obj[colon] == ':') {
                const size_t val_pos = SkipWs(obj, colon + 1u);
                if (candidate == key) {
                    return val_pos;
                }
                i = val_pos;
                continue;
            }
            i = key_end + 1u;
            continue;
        }
        ++i;
    }
    return std::string_view::npos;
}

[[nodiscard]] inline bool ReadString(std::string_view obj, std::string_view key, std::string& out) {
    const size_t p = FindKeyValPos(obj, key);
    if (p == std::string_view::npos || p >= obj.size() || obj[p] != '"') {
        return false;
    }
    std::string res;
    size_t i = p + 1u;
    while (i < obj.size()) {
        if (obj[i] == '\\' && (i + 1u) < obj.size()) {
            res.push_back(obj[i + 1u]);
            i += 2u;
            continue;
        }
        if (obj[i] == '"') {
            out = std::move(res);
            return true;
        }
        res.push_back(obj[i]);
        ++i;
    }
    return false;
}

[[nodiscard]] inline bool ReadInt64(std::string_view obj, std::string_view key, int64_t& out) noexcept {
    const size_t p = FindKeyValPos(obj, key);
    if (p == std::string_view::npos || p >= obj.size()) {
        return false;
    }
    size_t e = p;
    if (obj[e] == '-' || obj[e] == '+') {
        ++e;
    }
    while (e < obj.size() && std::isdigit(static_cast<unsigned char>(obj[e])) != 0) {
        ++e;
    }
    int64_t val = 0;
    const auto [ptr, ec] = std::from_chars(obj.data() + p, obj.data() + e, val);
    if (ec == std::errc{} && ptr == (obj.data() + e)) {
        out = val;
        return true;
    }
    return false;
}

[[nodiscard]] inline bool ReadBool(std::string_view obj, std::string_view key, bool& out) noexcept {
    const size_t p = FindKeyValPos(obj, key);
    if (p == std::string_view::npos) {
        return false;
    }
    if (obj.substr(p, 4u) == "true") {
        out = true;
        return true;
    }
    if (obj.substr(p, 5u) == "false") {
        out = false;
        return true;
    }
    return false;
}

[[nodiscard]] inline std::vector<std::string_view> SplitObjectArray(std::string_view root, std::string_view array_key) {
    std::vector<std::string_view> objects;
    const size_t arr_pos = FindKeyValPos(root, array_key);
    if (arr_pos == std::string_view::npos || arr_pos >= root.size() || root[arr_pos] != '[') {
        return objects;
    }
    const size_t arr_end = FindMatchingBracket(root, arr_pos, '[', ']');
    if (arr_end == std::string_view::npos) {
        return objects;
    }
    size_t cur = arr_pos + 1u;
    while (cur < arr_end) {
        cur = SkipWs(root, cur);
        if (cur < arr_end && root[cur] == '{') {
            const size_t obj_end = FindMatchingBracket(root, cur, '{', '}');
            if (obj_end == std::string_view::npos || obj_end > arr_end) {
                break;
            }
            objects.push_back(root.substr(cur, (obj_end - cur) + 1u));
            cur = obj_end + 1u;
        } else {
            ++cur;
        }
    }
    return objects;
}

[[nodiscard]] inline std::vector<std::string> ReadStringArray(std::string_view obj, std::string_view key) {
    std::vector<std::string> values;
    const size_t arr_pos = FindKeyValPos(obj, key);
    if (arr_pos == std::string_view::npos || arr_pos >= obj.size() || obj[arr_pos] != '[') {
        return values;
    }
    const size_t arr_end = FindMatchingBracket(obj, arr_pos, '[', ']');
    if (arr_end == std::string_view::npos) {
        return values;
    }
    size_t i = arr_pos + 1u;
    while (i < arr_end) {
        i = SkipWs(obj, i);
        if (i < arr_end && obj[i] == '"') {
            std::string s;
            ++i;
            while (i < arr_end && obj[i] != '"') {
                if (obj[i] == '\\' && (i + 1u) < arr_end) {
                    s.push_back(obj[i + 1u]);
                    i += 2u;
                } else {
                    s.push_back(obj[i]);
                    ++i;
                }
            }
            values.push_back(std::move(s));
        }
        ++i;
    }
    return values;
}

[[nodiscard]] inline std::vector<int64_t> ReadIntArray(std::string_view obj, std::string_view key) {
    std::vector<int64_t> values;
    const size_t arr_pos = FindKeyValPos(obj, key);
    if (arr_pos == std::string_view::npos || arr_pos >= obj.size() || obj[arr_pos] != '[') {
        return values;
    }
    const size_t arr_end = FindMatchingBracket(obj, arr_pos, '[', ']');
    if (arr_end == std::string_view::npos) {
        return values;
    }
    size_t i = arr_pos + 1u;
    while (i < arr_end) {
        i = SkipWs(obj, i);
        if (i < arr_end && (obj[i] == '-' || obj[i] == '+' || std::isdigit(static_cast<unsigned char>(obj[i])) != 0)) {
            size_t e = i + 1u;
            while (e < arr_end && std::isdigit(static_cast<unsigned char>(obj[e])) != 0) {
                ++e;
            }
            int64_t v = 0;
            const auto [ptr, ec] = std::from_chars(obj.data() + i, obj.data() + e, v);
            if (ec == std::errc{} && ptr == (obj.data() + e)) {
                values.push_back(v);
            }
            i = e;
        } else {
            ++i;
        }
    }
    return values;
}

} // namespace json_util

[[nodiscard]] inline MatterPhase ParseMatterPhase(std::string_view s) noexcept {
    if (s == "LIQUID" || s == "liquid") {
        return MatterPhase::LIQUID;
    }
    if (s == "GRANULAR_PARTICULATE" || s == "granular_particulate" || s == "granular") {
        return MatterPhase::GRANULAR_PARTICULATE;
    }
    return MatterPhase::DISCRETE_SOLID;
}

[[nodiscard]] inline HaulingToolClass ParseHaulingToolClass(std::string_view s) noexcept {
    if (s == "LIQUID_VESSEL" || s == "liquid_vessel") return HaulingToolClass::LIQUID_VESSEL;
    if (s == "GRANULAR_BUCKET_OR_SACK" || s == "granular_bucket_or_sack") return HaulingToolClass::GRANULAR_BUCKET_OR_SACK;
    if (s == "WHEELBARROW" || s == "wheelbarrow") return HaulingToolClass::WHEELBARROW;
    if (s == "HAND_DOLLY" || s == "hand_dolly") return HaulingToolClass::HAND_DOLLY;
    if (s == "CARGO_ETRIKE" || s == "cargo_etrike") return HaulingToolClass::CARGO_ETRIKE;
    if (s == "FORKLIFT" || s == "forklift") return HaulingToolClass::FORKLIFT;
    return HaulingToolClass::NONE_BARE_HANDS;
}

class alignas(8) ContentRegistry {
public:
    void RegisterItem(ItemDef def) {
        if (def.item_id.empty()) return;
        const auto it = item_idx_.find(def.item_id);
        if (it != item_idx_.end()) {
            items_[it->second] = std::move(def);
            return;
        }
        item_idx_.emplace(def.item_id, items_.size());
        items_.push_back(std::move(def));
    }

    void RegisterHaulingTool(HaulingToolDef def) {
        if (def.tool_id.empty()) return;
        const auto it = tool_idx_.find(def.tool_id);
        if (it != tool_idx_.end()) {
            tools_[it->second] = std::move(def);
            return;
        }
        tool_idx_.emplace(def.tool_id, tools_.size());
        tools_.push_back(std::move(def));
    }

    void RegisterAffordance(AffordanceDef def) {
        if (def.affordance_id.empty()) return;
        const auto it = affordance_idx_.find(def.affordance_id);
        if (it != affordance_idx_.end()) {
            affordances_[it->second] = std::move(def);
            return;
        }
        affordance_idx_.emplace(def.affordance_id, affordances_.size());
        affordances_.push_back(std::move(def));
    }

    void RegisterArchetype(EntityArchetypeDef def) {
        if (def.archetype_id.empty()) return;
        const auto it = archetype_idx_.find(def.archetype_id);
        if (it != archetype_idx_.end()) {
            archetypes_[it->second] = std::move(def);
            return;
        }
        archetype_idx_.emplace(def.archetype_id, archetypes_.size());
        archetypes_.push_back(std::move(def));
    }

    void RegisterThoughtRule(ThoughtRuleDef def) {
        if (def.rule_id.empty()) return;
        const auto it = thought_idx_.find(def.rule_id);
        if (it != thought_idx_.end()) {
            thoughts_[it->second] = std::move(def);
            return;
        }
        thought_idx_.emplace(def.rule_id, thoughts_.size());
        thoughts_.push_back(std::move(def));
    }

    [[nodiscard]] const ItemDef* FindItem(std::string_view item_id) const noexcept {
        const auto it = item_idx_.find(std::string(item_id));
        return (it != item_idx_.end()) ? &items_[it->second] : nullptr;
    }

    [[nodiscard]] const HaulingToolDef* FindHaulingTool(std::string_view tool_id) const noexcept {
        const auto it = tool_idx_.find(std::string(tool_id));
        return (it != tool_idx_.end()) ? &tools_[it->second] : nullptr;
    }

    [[nodiscard]] const HaulingToolDef* FindHaulingToolByClass(HaulingToolClass cls) const noexcept {
        for (const auto& t : tools_) {
            if (t.tool_class == cls) return &t;
        }
        return nullptr;
    }

    [[nodiscard]] const AffordanceDef* FindAffordance(std::string_view affordance_id) const noexcept {
        const auto it = affordance_idx_.find(std::string(affordance_id));
        return (it != affordance_idx_.end()) ? &affordances_[it->second] : nullptr;
    }

    [[nodiscard]] const EntityArchetypeDef* FindArchetype(std::string_view archetype_id) const noexcept {
        const auto it = archetype_idx_.find(std::string(archetype_id));
        return (it != archetype_idx_.end()) ? &archetypes_[it->second] : nullptr;
    }

    [[nodiscard]] const ThoughtRuleDef* FindThoughtRule(std::string_view rule_id) const noexcept {
        const auto it = thought_idx_.find(std::string(rule_id));
        return (it != thought_idx_.end()) ? &thoughts_[it->second] : nullptr;
    }

    [[nodiscard]] const std::vector<ItemDef>& Items() const noexcept { return items_; }
    [[nodiscard]] const std::vector<HaulingToolDef>& HaulingTools() const noexcept { return tools_; }
    [[nodiscard]] const std::vector<AffordanceDef>& Affordances() const noexcept { return affordances_; }
    [[nodiscard]] const std::vector<EntityArchetypeDef>& Archetypes() const noexcept { return archetypes_; }
    [[nodiscard]] const std::vector<ThoughtRuleDef>& ThoughtRules() const noexcept { return thoughts_; }

    inline void BootstrapCoreCatalog() {
        // 1. Hauling tools spanning all physical carrier classes
        RegisterHaulingTool(HaulingToolDef{"bare_hands", "Bare Hands", 35000u, 0u, 500u, 10000u, HaulingToolClass::NONE_BARE_HANDS, false});
        RegisterHaulingTool(HaulingToolDef{"watering_can", "Galvanized Watering Can", 10000u, 10000u, 0u, 9500u, HaulingToolClass::LIQUID_VESSEL, false});
        RegisterHaulingTool(HaulingToolDef{"carboy", "20L Glass Carboy", 22000u, 20000u, 0u, 8500u, HaulingToolClass::LIQUID_VESSEL, false});
        RegisterHaulingTool(HaulingToolDef{"tiffin_carrier", "Stainless Tiffin Vessel", 4000u, 3000u, 2000u, 10000u, HaulingToolClass::LIQUID_VESSEL, false});
        RegisterHaulingTool(HaulingToolDef{"bucket", "20kg Utility Bucket", 20000u, 12000u, 20000u, 9000u, HaulingToolClass::GRANULAR_BUCKET_OR_SACK, false});
        RegisterHaulingTool(HaulingToolDef{"burlap_sack", "Woven Burlap Sack", 20000u, 0u, 20000u, 9000u, HaulingToolClass::GRANULAR_BUCKET_OR_SACK, false});
        RegisterHaulingTool(HaulingToolDef{"wheelbarrow", "Steel Yard Wheelbarrow", 120000u, 0u, 120000u, 10000u, HaulingToolClass::WHEELBARROW, false});
        RegisterHaulingTool(HaulingToolDef{"hand_dolly", "Appliance Hand Dolly", 200000u, 0u, 0u, 9000u, HaulingToolClass::HAND_DOLLY, true});
        RegisterHaulingTool(HaulingToolDef{"cargo_etrike", "Electric Cargo Trike", 250000u, 150000u, 250000u, 13500u, HaulingToolClass::CARGO_ETRIKE, true});
        RegisterHaulingTool(HaulingToolDef{"forklift", "Compact Yard Forklift", 1500000u, 1000000u, 1200000u, 12000u, HaulingToolClass::FORKLIFT, true});

        // 2. Physical items across LIQUID, GRANULAR_PARTICULATE, and DISCRETE_SOLID phases
        RegisterItem(ItemDef{50000ULL, "water_potable", "Potable Water (1L)", 1000u, 1000u, 0u, MatterPhase::LIQUID});
        RegisterItem(ItemDef{50000ULL, "item.potable_water", "Potable & Cistern Water", 1000u, 1000u, 0u, MatterPhase::LIQUID});
        RegisterItem(ItemDef{2600000ULL, "raw_milk", "Raw Whole Milk (1L)", 1030u, 1000u, 620u, MatterPhase::LIQUID});
        RegisterItem(ItemDef{2600000ULL, "pasteurized_milk", "Pasteurized Milk (1L)", 1030u, 1000u, 620u, MatterPhase::LIQUID});
        RegisterItem(ItemDef{10000ULL, "excavated_dirt", "Excavated Loam Dirt", 1000u, 750u, 0u, MatterPhase::GRANULAR_PARTICULATE});
        RegisterItem(ItemDef{10000ULL, "item.topsoil_loam", "Native Sandy-Loam Topsoil", 1000u, 770u, 0u, MatterPhase::GRANULAR_PARTICULATE});
        RegisterItem(ItemDef{10000ULL, "item.clay_subsoil", "Excavated Clay Subsoil", 1000u, 625u, 0u, MatterPhase::GRANULAR_PARTICULATE});
        RegisterItem(ItemDef{15000ULL, "crushed_gravel", "Compacted Aggregate Gravel", 1600u, 1000u, 0u, MatterPhase::GRANULAR_PARTICULATE});
        RegisterItem(ItemDef{15000ULL, "item.crushed_gravel", "Compacted Crushed Drainage Gravel", 1000u, 590u, 0u, MatterPhase::GRANULAR_PARTICULATE});
        RegisterItem(ItemDef{450000ULL, "compost_humus", "Cured Compost Humus", 800u, 1000u, 0u, MatterPhase::GRANULAR_PARTICULATE});
        RegisterItem(ItemDef{1200000ULL, "biochar_granules", "Charged Biochar Granules", 400u, 1000u, 0u, MatterPhase::GRANULAR_PARTICULATE});
        RegisterItem(ItemDef{11300000ULL, "field_peas_loose", "Field Peas (Dry Loose)", 800u, 1000u, 2700u, MatterPhase::GRANULAR_PARTICULATE});
        RegisterItem(ItemDef{325000ULL, "fresh_egg", "Pasture Hen Egg", 60u, 55u, 78u, MatterPhase::DISCRETE_SOLID});
        RegisterItem(ItemDef{3010000ULL, "cooked_meal_plate", "Warm Homestead Meal", 450u, 500u, 720u, MatterPhase::DISCRETE_SOLID});
        RegisterItem(ItemDef{800000ULL, "concrete_block", "Cinder Foundation Block", 18000u, 9000u, 0u, MatterPhase::DISCRETE_SOLID});
        RegisterItem(ItemDef{5000000ULL, "appliance_crate_heavy", "Crated Thermal Kettle (65kg)", 65000u, 120000u, 0u, MatterPhase::DISCRETE_SOLID});
        RegisterItem(ItemDef{9000000ULL, "cast_iron_kiln_core", "Cast-Iron Retort Core (165kg)", 165000u, 85000u, 0u, MatterPhase::DISCRETE_SOLID});
        RegisterItem(ItemDef{25000000ULL, "pallet_masonry", "Palletized Masonry (850kg)", 850000u, 600000u, 0u, MatterPhase::DISCRETE_SOLID});

        // 3. Universal & domain affordances
        AffordanceDef look{};
        look.affordance_id = "affordance.look";
        look.label = "Look";
        RegisterAffordance(std::move(look));

        AffordanceDef inspect{};
        inspect.affordance_id = "affordance.inspect";
        inspect.label = "Inspect Closely";
        inspect.duration_sim_s = 30u;
        RegisterAffordance(std::move(inspect));

        AffordanceDef water_bed{};
        water_bed.affordance_id = "affordance.water_soil_bed";
        water_bed.label = "Water Garden Bed (5L)";
        water_bed.input_item_id = "water_potable";
        water_bed.input_qty = 5u;
        water_bed.duration_sim_s = 180u;
        water_bed.target_property_mut_key = "moisture_vwc_bp";
        water_bed.target_property_delta = 1200;
        RegisterAffordance(std::move(water_bed));

        AffordanceDef amend_biochar{};
        amend_biochar.affordance_id = "affordance.amend_biochar";
        amend_biochar.label = "Work Biochar into Topsoil";
        amend_biochar.required_capability_tag = "cap.soil_amendment";
        amend_biochar.input_item_id = "biochar_granules";
        amend_biochar.input_qty = 5u;
        amend_biochar.duration_sim_s = 420u;
        amend_biochar.target_property_mut_key = "organic_carbon_bp";
        amend_biochar.target_property_delta = 1500;
        RegisterAffordance(std::move(amend_biochar));

        AffordanceDef harvest_crop{};
        harvest_crop.min_property_val = 10000;
        harvest_crop.max_property_val = 10000;
        harvest_crop.target_property_delta = -10000;
        harvest_crop.affordance_id = "affordance.harvest_mature_crop";
        harvest_crop.label = "Harvest Mature Peas";
        harvest_crop.required_property_key = "crop_growth_bp";
        harvest_crop.output_item_id = "field_peas_loose";
        harvest_crop.output_qty = 4u;
        harvest_crop.duration_sim_s = 480u;
        harvest_crop.target_property_mut_key = "crop_growth_bp";
        RegisterAffordance(std::move(harvest_crop));

        AffordanceDef read_book{};
        read_book.affordance_id = "affordance.read_book";
        read_book.label = "Read Quietly";
        read_book.drive_satisfied = "leisure";
        read_book.duration_sim_s = 1500u;
        read_book.drive_delta_bp = 2800;
        RegisterAffordance(std::move(read_book));

        AffordanceDef pasteurize{};
        pasteurize.affordance_id = "affordance.pasteurize_milk";
        pasteurize.label = "Pasteurize Raw Milk Batch";
        pasteurize.required_capability_tag = "cap.thermal_pasteurization";
        pasteurize.input_item_id = "raw_milk";
        pasteurize.input_qty = 4u;
        pasteurize.output_item_id = "pasteurized_milk";
        pasteurize.output_qty = 4u;
        pasteurize.duration_sim_s = 900u;
        RegisterAffordance(std::move(pasteurize));

        // 4. Core Entity Archetypes
        RegisterArchetype(EntityArchetypeDef{
            "archetype.soil_garden_bed", "Raised Garden Bed",
            "Dark crumbly loam framed by cedar boards.", "agronomy",
            {"moisture_vwc_bp", "organic_carbon_bp", "nitrogen_ppm", "crop_growth_bp"},
            {"affordance.water_soil_bed", "affordance.amend_biochar", "affordance.harvest_mature_crop"},
            19u, true, false
        });
        RegisterArchetype(EntityArchetypeDef{
            "archetype.pasteurization_kettle", "Jacketed Pasteurization Kettle",
            "Heavy stainless thermal vessel with immersion heating element and lid.", "culinary",
            {"core_temp_mk", "liquid_volume_ml", "pasteurization_hold_s"},
            {"affordance.pasteurize_milk"},
            41u, false, false
        });
        RegisterArchetype(EntityArchetypeDef{
            "archetype.bookshelf", "Oak Bookshelf",
            "Shelves lined with agronomy manuals, history volumes, and worn novels.", "furniture",
            {"book_count"},
            {"affordance.read_book"},
            44u, false, false
        });
        RegisterArchetype(EntityArchetypeDef{
            "archetype.poultry_coop", "Backyard Timber Henhouse",
            "Ventilated cedar poultry coop with straw nesting boxes.", "livestock",
            {"egg_count", "bedding_cleanliness_bp"},
            {},
            58u, false, false
        });
        RegisterArchetype(EntityArchetypeDef{
            "archetype.garage_tool_rack", "Garage Hauling & Tool Rack",
            "Pegboard and floor bay storing buckets, watering cans, wheelbarrow, and dolly.", "logistics",
            {"tools_available"},
            {},
            55u, false, false
        });

        // 5. Calm diegetic thought rules
        RegisterThoughtRule(ThoughtRuleDef{
            "thought.calm_reading", "leisure", "reading",
            "Settling into a good chapter while the house is quiet.", 2000, 9000, 350, 900u
        });
        RegisterThoughtRule(ThoughtRuleDef{
            "thought.deep_reflection", "introspection", "introspection",
            "Taking twenty minutes to sketch out tomorrow's garden layout.", 1500, 8500, 400, 1200u
        });
    }

    // Loads items, hauling_tools, affordances, archetypes/entity_archetypes, and thought_rules from JSON.
    inline bool LoadModBundleFromJson(const std::string& json_text) {
        const std::string_view root(json_text);
        const size_t first = json_util::SkipWs(root, 0u);
        if (first >= root.size() || root[first] != '{') {
            return false;
        }
        size_t loaded_count = 0u;

        for (std::string_view obj : json_util::SplitObjectArray(root, "items")) {
            ItemDef d{};
            if (!json_util::ReadString(obj, "item_id", d.item_id)) {
                (void)json_util::ReadString(obj, "id", d.item_id);
            }
            if (d.item_id.empty()) continue;
            if (!json_util::ReadString(obj, "display_name", d.display_name)) {
                (void)json_util::ReadString(obj, "name", d.display_name);
            }
            int64_t v = 0;
            if (json_util::ReadInt64(obj, "unit_mass_g", v) && v >= 0) d.unit_mass_g = static_cast<uint32_t>(v);
            if (json_util::ReadInt64(obj, "unit_volume_ml", v) && v >= 0) d.unit_volume_ml = static_cast<uint32_t>(v);
            if (json_util::ReadInt64(obj, "exergy_per_unit_uj", v) && v >= 0) d.exergy_per_unit_uj = static_cast<uint64_t>(v);
            if (json_util::ReadInt64(obj, "caloric_kcal", v) && v >= 0) d.caloric_kcal = static_cast<uint32_t>(v);
            std::string phase_str;
            if (json_util::ReadString(obj, "matter_phase", phase_str)) d.matter_phase = ParseMatterPhase(phase_str);
            RegisterItem(std::move(d));
            ++loaded_count;
        }

        for (std::string_view obj : json_util::SplitObjectArray(root, "hauling_tools")) {
            HaulingToolDef d{};
            if (!json_util::ReadString(obj, "tool_id", d.tool_id)) {
                (void)json_util::ReadString(obj, "id", d.tool_id);
            }
            if (d.tool_id.empty()) continue;
            if (!json_util::ReadString(obj, "display_name", d.display_name)) {
                (void)json_util::ReadString(obj, "name", d.display_name);
            }
            std::string cls_str;
            if (json_util::ReadString(obj, "tool_class", cls_str)) {
                d.tool_class = ParseHaulingToolClass(cls_str);
            } else {
                d.tool_class = ParseHaulingToolClass(d.tool_id);
                if (d.tool_class == HaulingToolClass::NONE_BARE_HANDS) {
                    if (d.tool_id == "bucket" || d.tool_id == "burlap_sack") {
                        d.tool_class = HaulingToolClass::GRANULAR_BUCKET_OR_SACK;
                    } else if (d.tool_id == "watering_can" || d.tool_id == "tiffin_carrier" || d.tool_id == "carboy") {
                        d.tool_class = HaulingToolClass::LIQUID_VESSEL;
                    }
                }
            }
            int64_t v = 0;
            if (json_util::ReadInt64(obj, "max_payload_mass_g", v) && v >= 0) {
                d.max_payload_mass_g = static_cast<uint32_t>(v);
            } else if (json_util::ReadInt64(obj, "max_mass_g", v) && v >= 0) {
                d.max_payload_mass_g = static_cast<uint32_t>(v);
            }
            const auto phases = json_util::ReadStringArray(obj, "supported_phases");
            bool supports_liquid = false;
            bool supports_granular = false;
            for (const auto& ph : phases) {
                if (ph == "LIQUID") supports_liquid = true;
                if (ph == "GRANULAR_PARTICULATE") supports_granular = true;
            }
            if (json_util::ReadInt64(obj, "max_liquid_volume_ml", v) && v >= 0) {
                d.max_liquid_volume_ml = static_cast<uint32_t>(v);
            } else if (supports_liquid && json_util::ReadInt64(obj, "max_volume_ml", v) && v >= 0) {
                d.max_liquid_volume_ml = static_cast<uint32_t>(v);
            }
            if (json_util::ReadInt64(obj, "max_granular_mass_g", v) && v >= 0) {
                d.max_granular_mass_g = static_cast<uint32_t>(v);
            } else if (supports_granular) {
                d.max_granular_mass_g = d.max_payload_mass_g;
            }
            if (json_util::ReadInt64(obj, "speed_multiplier_bp", v) && v >= 0) d.speed_multiplier_bp = static_cast<uint32_t>(v);
            if (!json_util::ReadBool(obj, "requires_paved_or_graded_path", d.requires_paved_or_graded_path)) {
                std::string terrain;
                if (json_util::ReadString(obj, "terrain_constraint", terrain)) {
                    d.requires_paved_or_graded_path =
                        (terrain.find("PAVED") != std::string::npos) ||
                        (terrain.find("CONCRETE") != std::string::npos) ||
                        (terrain.find("SLAB_DRIVEWAY") != std::string::npos);
                }
            }
            RegisterHaulingTool(std::move(d));
            ++loaded_count;
        }

        for (std::string_view obj : json_util::SplitObjectArray(root, "affordances")) {
            AffordanceDef d{};
            if (!json_util::ReadString(obj, "affordance_id", d.affordance_id)) {
                (void)json_util::ReadString(obj, "id", d.affordance_id);
            }
            if (d.affordance_id.empty()) continue;
            (void)json_util::ReadString(obj, "label", d.label);
            if (!json_util::ReadString(obj, "required_capability_tag", d.required_capability_tag)) {
                const auto req_caps = json_util::ReadStringArray(obj, "required_capabilities");
                if (!req_caps.empty()) d.required_capability_tag = req_caps.front();
            }
            (void)json_util::ReadString(obj, "required_property_key", d.required_property_key);
            (void)json_util::ReadInt64(obj, "min_property_val", d.min_property_val);
            (void)json_util::ReadInt64(obj, "max_property_val", d.max_property_val);
            (void)json_util::ReadString(obj, "input_item_id", d.input_item_id);
            (void)json_util::ReadString(obj, "output_item_id", d.output_item_id);
            (void)json_util::ReadString(obj, "target_property_mut_key", d.target_property_mut_key);
            (void)json_util::ReadInt64(obj, "target_property_delta", d.target_property_delta);
            (void)json_util::ReadString(obj, "drive_satisfied", d.drive_satisfied);
            int64_t v = 0;
            if (json_util::ReadInt64(obj, "input_qty", v) && v >= 0) d.input_qty = static_cast<uint32_t>(v);
            if (json_util::ReadInt64(obj, "output_qty", v) && v >= 0) d.output_qty = static_cast<uint32_t>(v);
            if (json_util::ReadInt64(obj, "duration_sim_s", v) && v >= 0) {
                d.duration_sim_s = static_cast<uint32_t>(v);
            } else if (json_util::ReadInt64(obj, "duration_sim_seconds", v) && v >= 0) {
                d.duration_sim_s = static_cast<uint32_t>(v);
            }
            if (json_util::ReadInt64(obj, "drive_delta_bp", v)) d.drive_delta_bp = static_cast<int32_t>(v);
            RegisterAffordance(std::move(d));
            ++loaded_count;
        }

        auto load_archetype_list = [&](std::string_view key) {
            for (std::string_view obj : json_util::SplitObjectArray(root, key)) {
                EntityArchetypeDef d{};
                if (!json_util::ReadString(obj, "archetype_id", d.archetype_id)) {
                    (void)json_util::ReadString(obj, "id", d.archetype_id);
                }
                if (d.archetype_id.empty()) continue;
                (void)json_util::ReadString(obj, "display_name", d.display_name);
                if (!json_util::ReadString(obj, "look_description", d.look_description)) {
                    (void)json_util::ReadString(obj, "look_text", d.look_description);
                }
                if (!json_util::ReadString(obj, "category_tag", d.category_tag)) {
                    (void)json_util::ReadString(obj, "category", d.category_tag);
                }
                int64_t v = 0;
                if (json_util::ReadInt64(obj, "voxel_material_id", v) && v >= 0) d.voxel_material_id = static_cast<uint16_t>(v);
                (void)json_util::ReadBool(obj, "is_passable", d.is_passable);
                (void)json_util::ReadBool(obj, "is_airtight", d.is_airtight);
                d.inspect_property_keys = json_util::ReadStringArray(obj, "inspect_property_keys");
                d.affordance_ids = json_util::ReadStringArray(obj, "affordance_ids");
                if (d.affordance_ids.empty()) {
                    d.affordance_ids = json_util::ReadStringArray(obj, "affordances");
                }
                RegisterArchetype(std::move(d));
                ++loaded_count;
            }
        };
        load_archetype_list("archetypes");
        load_archetype_list("entity_archetypes");

        for (std::string_view obj : json_util::SplitObjectArray(root, "thought_rules")) {
            ThoughtRuleDef d{};
            if (!json_util::ReadString(obj, "rule_id", d.rule_id)) {
                (void)json_util::ReadString(obj, "id", d.rule_id);
            }
            if (d.rule_id.empty()) continue;
            (void)json_util::ReadString(obj, "drive_key", d.drive_key);
            (void)json_util::ReadString(obj, "activity_tag", d.activity_tag);
            if (!json_util::ReadString(obj, "thought_text", d.thought_text)) {
                (void)json_util::ReadString(obj, "text", d.thought_text);
            }
            int64_t v = 0;
            if (json_util::ReadInt64(obj, "min_drive_bp", v)) d.min_drive_bp = static_cast<int32_t>(v);
            if (json_util::ReadInt64(obj, "max_drive_bp", v)) d.max_drive_bp = static_cast<int32_t>(v);
            if (json_util::ReadInt64(obj, "mood_delta_bp", v)) d.mood_delta_bp = static_cast<int32_t>(v);
            if (json_util::ReadInt64(obj, "cooldown_sim_s", v) && v >= 0) {
                d.cooldown_sim_s = static_cast<uint32_t>(v);
            } else if (json_util::ReadInt64(obj, "cooldown_sim_seconds", v) && v >= 0) {
                d.cooldown_sim_s = static_cast<uint32_t>(v);
            }
            RegisterThoughtRule(std::move(d));
            ++loaded_count;
        }

        return loaded_count > 0u || json_text.find('{') != std::string::npos;
    }

private:
    std::vector<ItemDef>                       items_{};
    std::vector<HaulingToolDef>                tools_{};
    std::vector<AffordanceDef>                 affordances_{};
    std::vector<EntityArchetypeDef>            archetypes_{};
    std::vector<ThoughtRuleDef>                thoughts_{};
    std::unordered_map<std::string, size_t>    item_idx_{};
    std::unordered_map<std::string, size_t>    tool_idx_{};
    std::unordered_map<std::string, size_t>    affordance_idx_{};
    std::unordered_map<std::string, size_t>    archetype_idx_{};
    std::unordered_map<std::string, size_t>    thought_idx_{};
};

} // namespace oasis::engine
