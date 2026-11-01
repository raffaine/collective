#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif
#include <SDL2/SDL.h>
#include <flecs.h>
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../../core/chill_loop.hpp"
#include "../../uhai/uhai_ring_buffer.hpp"
#include "../ai/utility_autonomy.hpp"
#include "../clock/simulation_clock.hpp"
#include "../content/content_registry.hpp"
#include "../domains/soil_physics.hpp"
#include "../domains/thermal_vessel.hpp"
#include "../ecs/affordance_system.hpp"
#include "../l2_bridge/unmodeled_step_adapter.hpp"
#include "../l2_bridge/virtual_pin_bus.hpp"
#include "../logistics/hauling_physics.hpp"
#include "../world/map_provider.hpp"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include "../render/webgpu_renderer.hpp"

namespace oasis::engine::app {

// Physical hauling tool voxel material IDs rendered by raymarch.wgsl SDFs (70u..72u)
inline constexpr uint16_t kVoxelMatWheelbarrow  = 70u;
inline constexpr uint16_t kVoxelMatLiquidVessel = 71u;
inline constexpr uint16_t kVoxelMatHandDolly    = 72u;
inline constexpr uint16_t kVoxelMatPoultryCoop  = 58u;
inline constexpr uint16_t kVoxelMatPastureHen   = 59u;
inline constexpr uint16_t kVoxelMatBiocharKiln  = 60u;
inline constexpr uint16_t kVoxelMatCompostDrum  = 62u;

struct alignas(8) OasisAppConfig {
    std::string              map_path{};
    std::vector<std::string> mod_paths{};
    uint64_t                 max_ticks{60ULL};
    uint32_t                 width{1024u};
    uint32_t                 height{720u};
    bool                     headless_ci{false};
    bool                     procedural_map{false};
    uint8_t                  pad_[6]{0u, 0u, 0u, 0u, 0u, 0u};
};

struct alignas(4) GridCoord {
    int32_t x{0};
    int32_t y{0};
    int32_t z{0};

    [[nodiscard]] constexpr bool operator==(const GridCoord& rhs) const noexcept {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
};
static_assert(sizeof(GridCoord) == 12u, "GridCoord must be 12 bytes.");

struct alignas(8) EntitySpatialMetaComponent {
    std::string entity_id{};
    std::string archetype_id{};
    std::string room_tag{};
    std::string hauling_tool_id{};
    uint64_t    entity_uid{0ULL};
    int32_t     x{0};
    int32_t     y{1};
    int32_t     z{0};
    int32_t     stand_x{0};
    int32_t     stand_y{1};
    int32_t     stand_z{0};
    uint16_t    parked_material_id{0u};
    bool        is_parked_in_world{true};
    bool        has_drip_valve_hardware{false};
    uint8_t     pad_[4]{0u, 0u, 0u, 0u};
};

struct alignas(8) PastureHenAgent {
    AgentDrivesComponent   drives{};
    AutonomyStateComponent autonomy{};
    int32_t                x{21};
    int32_t                y{1};
    int32_t                z{10};
    uint32_t               roam_accum_sim_ms{0u};
};
static_assert(sizeof(PastureHenAgent) == 96u, "PastureHenAgent must be 96 bytes.");

enum class TaskStage : uint8_t {
    IDLE               = 0,
    WALK_TO_FETCH_TOOL = 1,
    WALK_TO_STATION    = 2,
    PERFORM_WORK       = 3,
    WALK_TO_PARK_TOOL  = 4
};

struct alignas(8) ActiveStewardTask {
    ActivityTimer          work_timer{};
    std::string            affordance_id{};
    std::string            label{};
    std::string            target_entity_id{};
    std::string            required_tool_entity_id{};
    std::string            required_tool_id{};
    std::string            l4_task_id{};
    std::vector<GridCoord> current_path{};
    size_t                 path_cursor{0u};
    int32_t                target_x{12};
    int32_t                target_y{1};
    int32_t                target_z{6};
    uint32_t               walk_speed_bp{kNominalWalkSpeedBp};
    TaskStage                stage{TaskStage::IDLE};
    bool                     auto_park_tool_after{false};
    [[maybe_unused]] uint8_t pad_[6]{0u, 0u, 0u, 0u, 0u, 0u};
};

namespace detail {

[[nodiscard]] inline std::string JsonEscape(std::string_view s) {
    std::string out;
    out.reserve(s.size() + 8u);
    for (const char ch : s) {
        switch (ch) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (static_cast<unsigned char>(ch) >= 0x20u) {
                    out.push_back(ch);
                }
                break;
        }
    }
    return out;
}

[[nodiscard]] inline std::string StrataMaterialName(uint16_t mat) {
    switch (mat) {
        case STRATA_CLAY_SUBSOIL:           return "Dense Clay Subsoil (y-horizon)";
        case STRATA_COMPACTED_GRAVEL_PAD:   return "Compacted Crushed-Gravel Subbase";
        case STRATA_CONCRETE_FOOTING_SLAB:  return "Reinforced Concrete Footing / Slab";
        case STRATA_TOPSOIL_LOAM:           return "Native Sandy-Loam Topsoil";
        case STRATA_UTILITY_CONDUIT_TRENCH: return "Buried Utility Conduit Trench";
        case MAT_SURFACE_LAWN_OR_FLOOR:     return "Turf Lawn / Interior Finished Slab";
        case MAT_TILLED_GARDEN_SOIL:        return "Amended Raised-Bed Garden Loam";
        case MAT_CONCRETE_PAVEMENT:         return "Broomed Concrete Paving";
        case MAT_ASPHALT_ROAD:
        case MAT_ROAD_CENTERLINE:           return "Asphalt Cul-de-Sac Roadway";
        default:                            return "Structural / Mineral Substrate";
    }
}

[[nodiscard]] inline bool IsStrataExcavatable(uint16_t mat) noexcept {
    return mat == STRATA_TOPSOIL_LOAM ||
           mat == STRATA_CLAY_SUBSOIL ||
           mat == STRATA_COMPACTED_GRAVEL_PAD ||
           mat == MAT_SURFACE_LAWN_OR_FLOOR;
}

[[nodiscard]] inline bool IsVoxelPassableForAgent(uint16_t mat) noexcept {
    return mat == MAT_VOXEL_AIR ||
           mat == MAT_INSULATED_TIMBER_DOOR ||
           mat == MAT_GARAGE_SECTIONAL_DOOR ||
           mat == MAT_ROOF_ACCESS_LADDER ||
           mat == 23u ||  // Bed mattress
           mat == 120u || // Office desk chair space
           mat == kVoxelMatWheelbarrow ||
           mat == kVoxelMatLiquidVessel ||
           mat == kVoxelMatHandDolly;
}

[[nodiscard]] inline bool IsWorkflowGovernedAffordance(std::string_view aff_id) noexcept {
    return aff_id == "aff.mill_raised_bed_frame" ||
           aff_id == "aff.assemble_compost_and_kiln" ||
           aff_id == "aff.build_poultry_coop" ||
           aff_id == "aff.fulfill_l7_woodwork" ||
           aff_id == "aff.plow_and_amend_bed" ||
           aff_id == "aff.erect_pea_trellis" ||
           aff_id == "aff.inoculate_and_sow_peas" ||
           aff_id == "aff.water_garden_manual" ||
           aff_id == "aff.harvest_peas_and_roots" ||
           aff_id == "aff.shell_dry_and_turn_bed" ||
           aff_id == "aff.pyrolyze_and_quench_biochar" ||
           aff_id == "aff.turn_and_harvest_compost" ||
           aff_id == "aff.tend_hens_and_collect_eggs" ||
           aff_id == "aff.gather_soup_inputs" ||
           aff_id == "aff.simmer_bone_broth" ||
           aff_id == "aff.cook_pea_soup" ||
           aff_id == "aff.pack_and_deliver_tiffins";
}

inline void ApplyStagePreconditionsToCatalog(ContentRegistry& reg) {
    auto gate_prop = [&](std::string_view aff_id, std::string_view key, int64_t min_v, int64_t max_v) {
        const AffordanceDef* existing = reg.FindAffordance(aff_id);
        if (existing == nullptr) {
            return;
        }
        AffordanceDef copy         = *existing;
        copy.required_property_key = std::string(key);
        copy.min_property_val      = min_v;
        copy.max_property_val      = max_v;
        reg.RegisterAffordance(std::move(copy));
    };

    gate_prop("aff.plow_and_amend_bed", "bed_stage", 0, 0);
    gate_prop("aff.erect_pea_trellis", "bed_stage", 1, 1);
    gate_prop("aff.inoculate_and_sow_peas", "bed_stage", 2, 2);
    gate_prop("aff.water_garden_manual", "bed_stage", 3, 4);
    gate_prop("aff.harvest_peas_and_roots", "bed_stage", 4, 4);
    gate_prop("aff.shell_dry_and_turn_bed", "bed_stage", 5, 5);
    gate_prop("aff.install_l2_soil_probe", "l2_probe_installed", 0, 0);
    gate_prop("aff.install_l2_drip_valve", "l2_valve_installed", 0, 0);

    gate_prop("aff.gather_soup_inputs", "hearth_stage", 0, 0);
    gate_prop("aff.simmer_bone_broth", "hearth_stage", 1, 1);
    gate_prop("aff.cook_pea_soup", "hearth_stage", 2, 2);
    gate_prop("aff.pack_and_deliver_tiffins", "hearth_stage", 3, 3);
    gate_prop("aff.attach_l2_culinary_controller", "l2_culinary_installed", 0, 0);
}

// Seeds complete core_catalog.json definitions inline so WASM builds (without embedded FS) have 100% parity.
inline void SeedHomesteadCoreCatalog(ContentRegistry& reg) {
    // Items from core_catalog.json
    reg.RegisterItem(ItemDef{50000ULL, "item.potable_water", "Potable & Cistern Water", 1000u, 1000u, 0u, MatterPhase::LIQUID});
    reg.RegisterItem(ItemDef{1800000ULL, "item.bone_broth", "24-Hour Collagen Bone Broth", 515u, 500u, 210u, MatterPhase::LIQUID});
    reg.RegisterItem(ItemDef{2700000ULL, "item.pea_soup", "Communal Split-Pea & Mirepoix Soup", 540u, 500u, 650u, MatterPhase::LIQUID});
    reg.RegisterItem(ItemDef{950000ULL, "item.rhizobium_slurry", "Rhizobium Seed Inoculant Slurry", 260u, 250u, 0u, MatterPhase::LIQUID});
    reg.RegisterItem(ItemDef{300000ULL, "item.compost_tea", "Aerated Humic Compost Tea", 1005u, 1000u, 0u, MatterPhase::LIQUID});
    reg.RegisterItem(ItemDef{10000ULL, "item.topsoil_loam", "Native Sandy-Loam Topsoil", 1000u, 770u, 0u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{10000ULL, "item.clay_subsoil", "Excavated Clay Subsoil", 1000u, 625u, 0u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{15000ULL, "item.crushed_gravel", "Compacted Crushed Drainage Gravel", 1000u, 590u, 0u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{450000ULL, "item.thermophilic_compost", "EPA 503 Class-A Thermophilic Compost", 1000u, 1400u, 0u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{1200000ULL, "item.pyrolysis_biochar", "Quenched Smokeless TLUD Pyrolysis Biochar", 1000u, 2500u, 0u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{7100000ULL, "item.split_peas_dry", "Dried Shelled Green Split Peas", 500u, 620u, 1700u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{1400000ULL, "item.pea_seeds_heirloom", "Heirloom Climbing Pea & Mirepoix Seed Packet", 100u, 125u, 0u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{250000ULL, "item.bone_ash_calcium_grit", "Calcined Bone-Ash Calcium Grit", 500u, 420u, 0u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{180000ULL, "item.bsfl_organic_scraps", "Vegetable Trimmings & BSFL Grubs", 1000u, 1250u, 0u, MatterPhase::GRANULAR_PARTICULATE});
    reg.RegisterItem(ItemDef{1700000ULL, "item.fresh_pea_pods", "Unhulled Harvested Pea Pods", 500u, 900u, 410u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{1700000ULL, "item.mirepoix_roots", "Fresh Mirepoix Roots", 1000u, 1300u, 410u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{325000ULL, "item.pasture_egg", "Golden-Yolked Heritage Pasture Egg", 60u, 58u, 78u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{600000ULL, "item.poultry_bones_raw", "Collagen-Rich Pasture Poultry Frame", 850u, 1100u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{1500000ULL, "item.cedar_plank", "Seasoned Aromatic Cedar Plank (2.4 m)", 4200u, 8400u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{2200000ULL, "item.trellis_aframe_kit", "Cedar & Mesh A-Frame Pea Trellis", 18500u, 32000u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{3200000ULL, "item.raised_bed_frame_kit", "Mortised Cedar Raised Bed Frame Assembly", 32000u, 55000u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{6800000ULL, "item.tlud_biochar_kiln_assembly", "Smokeless Concentric-Draft TLUD Pyrolysis Kiln", 68000u, 110000u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{7400000ULL, "item.thermophilic_compost_drum_assembly", "Insulated Rotating Thermophilic Compost Drum", 74000u, 220000u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{11500000ULL, "item.poultry_coop_module", "Predator-Proof Elevated Poultry Coop & BSFL Hopper", 115000u, 480000u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{4800000ULL, "item.custom_joinery_commission", "Finished Hardwood Joinery Commission", 48000u, 95000u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{500000ULL, "item.l2_soil_probe", "FDR Capacitive Soil Moisture & NPK/Temp Probe", 320u, 240u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{600000ULL, "item.l2_irrigation_valve", "24V Latching Drip-Line Solenoid Valve", 480u, 350u, 0u, MatterPhase::DISCRETE_SOLID});
    reg.RegisterItem(ItemDef{700000ULL, "item.l2_culinary_rtd_controller", "PT100 RTD Probe & Induction PID Controller", 410u, 310u, 0u, MatterPhase::DISCRETE_SOLID});

    // Ensure bucket supports both LIQUID (15L) and GRANULAR (20kg) and wheelbarrow supports 140kg matching core_catalog.json
    reg.RegisterHaulingTool(HaulingToolDef{"bucket", "15L Galvanized Homestead Bucket", 20000u, 15000u, 20000u, 9200u, HaulingToolClass::GRANULAR_BUCKET_OR_SACK, false});
    reg.RegisterHaulingTool(HaulingToolDef{"watering_can", "10L Galvanized Rose-Spout Watering Can", 11000u, 10000u, 0u, 9500u, HaulingToolClass::LIQUID_VESSEL, false});
    reg.RegisterHaulingTool(HaulingToolDef{"tiffin_carrier", "4-Tier 304-Stainless Tiffin Carrier", 6000u, 4000u, 0u, 9800u, HaulingToolClass::LIQUID_VESSEL, false});
    reg.RegisterHaulingTool(HaulingToolDef{"wheelbarrow", "Pneumatic Steel Contractor Wheelbarrow", 140000u, 0u, 140000u, 8500u, HaulingToolClass::WHEELBARROW, false});
    reg.RegisterHaulingTool(HaulingToolDef{"hand_dolly", "Heavy-Duty Steel Hand Dolly", 200000u, 0u, 0u, 8200u, HaulingToolClass::HAND_DOLLY, true});

    auto reg_aff = [&](
        std::string_view id,
        std::string_view label,
        uint32_t         dur_s,
        std::string_view in_item  = "",
        uint32_t         in_qty   = 0u,
        std::string_view out_item = "",
        uint32_t         out_qty  = 0u,
        std::string_view drive    = "",
        int32_t          drive_bp = 0
    ) {
        AffordanceDef a{};
        a.affordance_id   = std::string(id);
        a.label           = std::string(label);
        a.duration_sim_s  = dur_s;
        a.input_item_id   = std::string(in_item);
        a.input_qty       = in_qty;
        a.output_item_id  = std::string(out_item);
        a.output_qty      = out_qty;
        a.drive_satisfied = std::string(drive);
        a.drive_delta_bp  = drive_bp;
        reg.RegisterAffordance(std::move(a));
    };

    reg_aff("aff.look", "Look", 0u);
    reg_aff("aff.inspect", "Inspect Closely", 30u);
    reg_aff("aff.pickup_hauling_tool", "Acquire Hauling Tool", 12u);
    reg_aff("aff.draw_potable_water", "Fill Vessel with Potable Water (5 L)", 45u, "", 0u, "item.potable_water", 5u);
    reg_aff("aff.mill_raised_bed_frame", "Mill Cedar Raised Bed & Place in Yard", 900u, "item.cedar_plank", 4u, "item.raised_bed_frame_kit", 1u);
    reg_aff("aff.assemble_compost_and_kiln", "Assemble & Wheelbarrow-Haul Compost Drum + TLUD Kiln (74 kg)", 1200u, "item.cedar_plank", 3u, "item.thermophilic_compost_drum_assembly", 1u);
    reg_aff("aff.build_poultry_coop", "Build & Haul 115 kg Elevated Poultry Coop", 1500u, "item.cedar_plank", 6u, "item.poultry_coop_module", 1u);
    reg_aff("aff.fulfill_l7_woodwork", "Mill & Dolly-Haul Custom Joinery Order (48 kg)", 1200u, "item.cedar_plank", 4u, "item.custom_joinery_commission", 1u);
    reg_aff("aff.plow_and_amend_bed", "Broadfork & Amend Soil with Compost/Biochar (15 kg)", 600u, "item.thermophilic_compost", 15u);
    reg_aff("aff.erect_pea_trellis", "Erect A-Frame Climbing Pea Trellis", 420u);
    reg_aff("aff.inoculate_and_sow_peas", "Inoculate (Rhizobium) & Sow Split Peas + Mirepoix", 540u, "item.pea_seeds_heirloom", 1u);
    reg_aff("aff.water_garden_manual", "Irrigate & Hand-Weed Bed (5 L Watering Can)", 360u, "item.potable_water", 5u);
    reg_aff("aff.install_l2_soil_probe", "Install L2 FDR Soil Probe (soil_probe_v1)", 180u);
    reg_aff("aff.install_l2_drip_valve", "Plumb L2 24V Drip Solenoid Valve (irrigation_valve_v1)", 240u);
    reg_aff("aff.harvest_peas_and_roots", "Harvest Mature Pea Pods & Mirepoix Roots", 480u, "", 0u, "item.fresh_pea_pods", 4u);
    reg_aff("aff.shell_dry_and_turn_bed", "Shell Dry Peas (<14% MC) & Cut-In Root Nodules", 540u, "", 0u, "item.split_peas_dry", 3u);
    reg_aff("aff.pyrolyze_and_quench_biochar", "Run Smokeless TLUD Pyrolysis & Quench Biochar (8 kg)", 720u, "", 0u, "item.pyrolysis_biochar", 8u);
    reg_aff("aff.turn_and_harvest_compost", "Aerate & Screen Thermophilic Humus Compost (25 kg)", 480u, "", 0u, "item.thermophilic_compost", 25u);
    reg_aff("aff.tend_hens_and_collect_eggs", "Feed BSFL Grubs, Refresh Litter & Gather Eggs", 360u, "", 0u, "item.pasture_egg", 4u, "mood", 900);
    reg_aff("aff.gather_soup_inputs", "Gather & Rinse Split Peas, Mirepoix & Broth Bones", 300u, "", 0u, "", 0u, "introspection", 600);
    reg_aff("aff.attach_l2_culinary_controller", "Clip PT100 Probe & L2 Controller to Cauldron", 180u);
    reg_aff("aff.simmer_bone_broth", "Start 24-Hour Collagen Bone Broth", 900u, "", 0u, "item.bone_broth", 4u, "hunger", 1600);
    reg_aff("aff.cook_pea_soup", "Prep Mirepoix & Simmer Communal Split-Pea Soup (>= 74 C)", 900u, "", 0u, "item.pea_soup", 6u, "hunger", 2800);
    reg_aff("aff.pack_and_deliver_tiffins", "Fill Stainless Tiffin Carrier & Share Hot Pea Soup (2 L)", 420u, "item.pea_soup", 4u, "", 0u, "social", 2200);
    reg_aff("aff.eat_nourished_meal", "Sit & Eat Warm Homestead Meal", 480u, "", 0u, "", 0u, "hunger", 3200);
    reg_aff("aff.take_warm_shower", "Take Warm Hydronic Shower", 360u, "", 0u, "", 0u, "leisure", 1400);
    reg_aff("aff.decompress_living_room", "Unwind with Music & Reading on Couch", 1200u, "", 0u, "", 0u, "leisure", 2600);
    reg_aff("aff.study_at_workstation", "Review L6 Needs, Governance & Workflow Plans", 900u, "", 0u, "", 0u, "introspection", 2200);
    reg_aff("aff.sleep_in_bed", "Sleep Circadian Rest", 3600u, "", 0u, "", 0u, "energy", 4500);
    reg_aff("aff.excavate_earthwork_tile", "Excavate Subsurface Earthwork Horizon (110 kg Spoil)", 600u, "", 0u, "item.topsoil_loam", 110u);

    ApplyStagePreconditionsToCatalog(reg);

    auto reg_arch = [&](
        std::string_view              id,
        std::string_view              name,
        std::string_view              look,
        uint16_t                      mat,
        bool                          passable,
        std::initializer_list<const char*> affs
    ) {
        EntityArchetypeDef d{};
        d.archetype_id      = std::string(id);
        d.display_name      = std::string(name);
        d.look_description  = std::string(look);
        d.voxel_material_id = mat;
        d.is_passable       = passable;
        for (const char* a : affs) {
            d.affordance_ids.emplace_back(a);
        }
        reg.RegisterArchetype(std::move(d));
    };

    reg_arch("arch.induction_hearth_stove", "Magnetic Induction Hearth & 40L Stockpot",
             "A high-efficiency magnetic induction range topped with a heavy 304-stainless communal soup stockpot.",
             41u, false, {"aff.simmer_bone_broth", "aff.cook_pea_soup", "aff.attach_l2_culinary_controller"});
    reg_arch("arch.kitchen_prep_sink_cabinet", "Stainless Prep Sink & Tiffin Counter",
             "A deep stainless washbasin and butcher-block prep counter for rinsing split peas, scrubbing roots, and filling tiffins.",
             42u, false, {"aff.draw_potable_water", "aff.gather_soup_inputs", "aff.pack_and_deliver_tiffins"});
    reg_arch("arch.refrigerator_larder", "Insulated Homestead Larder & Cold Box",
             "An insulated refrigerator-larder storing chilled bone broth jars, mirepoix roots, pasture eggs, and dry split-pea canisters.",
             40u, false, {"aff.eat_nourished_meal"});
    reg_arch("arch.dining_table_set", "Oak Dining Table & Chairs",
             "A solid oak table set for quiet meals or sharing hot bowls of split-pea soup.",
             43u, false, {"aff.eat_nourished_meal"});
    reg_arch("arch.woodshop_joinery_workbench", "Scenario Alpha Joinery Workbench & Mortising Vise",
             "A heavy laminated maple workbench fitted with a twin-screw joinery vise, hand planes, and mortising chisels.",
             54u, false, {"aff.mill_raised_bed_frame", "aff.assemble_compost_and_kiln", "aff.build_poultry_coop", "aff.fulfill_l7_woodwork"});
    reg_arch("arch.hand_tool_pegboard", "Garage Agronomic & Carpentry Tool Wall",
             "An organized pegboard holding a broadfork, dibber, spirit level, pruning shears, and chisels.",
             55u, false, {});
    reg_arch("arch.garage_lumber_and_seed_rack", "Garage Cedar Lumber, Seed & L2 Hardware Rack",
             "Stickered racks of seasoned aromatic cedar planks alongside sealed jars of heirloom pea seeds and boxed L2 probes.",
             56u, false, {});
    reg_arch("arch.raised_garden_bed", "Mortised Cedar Raised Garden Bed & Pea Trellis",
             "A 2.4 m mortised cedar raised bed filled with dark loam soil beneath an A-frame climbing pea trellis.",
             19u, false, {
                 "aff.plow_and_amend_bed", "aff.erect_pea_trellis", "aff.inoculate_and_sow_peas",
                 "aff.water_garden_manual", "aff.harvest_peas_and_roots", "aff.shell_dry_and_turn_bed",
                 "aff.install_l2_soil_probe", "aff.install_l2_drip_valve"
             });
    reg_arch("arch.thermophilic_compost_drum", "Insulated Rotating Thermophilic Compost Drum",
             "An insulated dual-chamber tumbling compost drum digesting kitchen peelings, spent pea vines, and biochar into black humus.",
             kVoxelMatCompostDrum, false, {"aff.turn_and_harvest_compost"});
    reg_arch("arch.tlud_biochar_kiln", "Smokeless Concentric-Draft TLUD Biochar Kiln",
             "A double-walled stainless Top-Lit Updraft pyrolysis kiln that burns woodgas cleanly with zero visible smoke.",
             kVoxelMatBiocharKiln, false, {"aff.pyrolyze_and_quench_biochar"});
    reg_arch("arch.poultry_coop", "Elevated Heritage Poultry Coop & BSFL Hopper",
             "An elevated timber hen house with predator-proof hardware cloth, biochar deep litter, and a self-harvesting BSFL grub ramp.",
             kVoxelMatPoultryCoop, false, {"aff.tend_hens_and_collect_eggs"});
    reg_arch("arch.exterior_water_spigot", "Backyard Brass Hose Bibb & Rainwater Manifold",
             "A frost-free red-brass garden spigot with an atmospheric RPZ backflow air-gap.",
             0u, true, {"aff.draw_potable_water"});
    reg_arch("arch.homelab_cad_workstation", "Sovereign L6 Workstation & CAD Desk",
             "A dual-monitor Linux workstation connected directly to Node 0's local col-gatewayd socket.",
             120u, true, {"aff.study_at_workstation"});
    reg_arch("arch.reference_archive_shelf", "Agronomy, Joinery & Permaculture Reference Shelf",
             "Shelves lined with soil microbiology manuals, timber joinery plates, and Sovereign Stack specifications.",
             44u, false, {});
    reg_arch("arch.sovereign_node_rack", "Node 0 Deterministic Daemon & LiFePO4 Microgrid Rack",
             "A whisper-quiet convection-cooled server and 48V LiFePO4 battery cabinet running col-telemetryd through col-commonsd.",
             30u, false, {});
    reg_arch("arch.warm_shower_stall", "Hydronic Warm Shower",
             "A tiled shower stall supplied with domestic hot water pre-warmed by the Node 0 hydronic heat-recovery loop.",
             45u, false, {"aff.take_warm_shower"});
    reg_arch("arch.sanitation_toilet", "Low-Flow Sanitation Fixture", "A clean ceramic bathroom toilet.", 46u, false, {});
    reg_arch("arch.bathroom_vanity_sink", "Bathroom Washbasin & Vanity", "A porcelain washbasin stocked with castile soap and linen towels.", 47u, false, {});
    reg_arch("arch.living_room_couch", "Linen Living Room Couch", "A deep three-seat couch facing the warm acoustic hi-fi system.", 48u, false, {"aff.decompress_living_room"});
    reg_arch("arch.media_display_console", "Living Room Media Display", "A low timber console with an OLED screen for evening films and local co-op games.", 49u, false, {"aff.decompress_living_room"});
    reg_arch("arch.acoustic_sound_system", "Bookshelf Hi-Fi Speaker Tower", "Warm birch-ply acoustic speakers calibrated within quiet-hours limits.", 50u, false, {"aff.decompress_living_room"});
    reg_arch("arch.insulated_bed_mattress", "Timber Frame Bed & Wool Mattress", "A sturdy mortised bed frame dressed with breathable organic cotton and wool blankets.", 23u, true, {"aff.sleep_in_bed"});
    reg_arch("arch.bedside_nightstand", "Cedar Bedside Nightstand", "A small nightstand with a warm reading lamp and carafe of water.", 51u, false, {});
    reg_arch("arch.timber_wardrobe", "Cedar Clothing Wardrobe", "A tall wardrobe storing canvas work aprons, rain gear, and clean clothes.", 52u, false, {});
    reg_arch("arch.standing_mirror", "Full-Length Bedroom Mirror", "A simple timber-framed standing mirror reflecting natural window light.", 53u, false, {});
    reg_arch("arch.hauling_tool_bucket", "15L Galvanized Homestead Bucket", "A riveted 15-liter galvanized steel bucket with a wooden bail grip.", kVoxelMatLiquidVessel, true, {"aff.pickup_hauling_tool"});
    reg_arch("arch.hauling_tool_watering_can", "10L Galvanized Rose-Spout Watering Can", "A balanced 10-liter galvanized watering can with a removable fine-spray brass rose.", kVoxelMatLiquidVessel, true, {"aff.pickup_hauling_tool"});
    reg_arch("arch.hauling_tool_tiffin_carrier", "4-Tier 304-Stainless Insulated Tiffin Carrier", "A gleaming 4-tier 304-stainless latching tiffin carrier tagged with an NFC return manifest.", kVoxelMatLiquidVessel, true, {"aff.pickup_hauling_tool"});
    reg_arch("arch.hauling_tool_wheelbarrow", "Pneumatic Steel Contractor Wheelbarrow", "A deep-tray steel wheelbarrow on a knobby pneumatic tire built for 140 kg loads.", kVoxelMatWheelbarrow, true, {"aff.pickup_hauling_tool"});
    reg_arch("arch.hauling_tool_hand_dolly", "Heavy-Duty Steel Hand Dolly", "An upright welded-steel appliance and joinery hand dolly rated to 200 kg.", kVoxelMatHandDolly, true, {"aff.pickup_hauling_tool"});
    reg_arch("arch.yard_earthwork_plot", "Unimproved Backyard Lawn & Topsoil Plot",
             "Open backyard grass overlying sandy-loam topsoil and deep clay subsoil horizons. Mill a raised bed frame at the garage workbench first.",
             MAT_SURFACE_LAWN_OR_FLOOR, true, {});

    reg.RegisterThoughtRule(ThoughtRuleDef{"thought.day1_clean_backyard_horizon", "introspection", "introspection",
        "Clean morning air over open backyard grass — ready to mill our first cedar raised bed whenever we choose.", 4000, 10000, 150, 900u});
    reg.RegisterThoughtRule(ThoughtRuleDef{"thought.reading_quiet", "leisure", "reading",
        "Settling in with soil microbiology notes and timber joinery drawings while the house stays quiet.", 0, 10000, 200, 900u});
    reg.RegisterThoughtRule(ThoughtRuleDef{"thought.gaming_music", "leisure", "gaming",
        "Warm hi-fi acoustics in the living room — taking an unhurried half-hour break.", 0, 10000, 220, 900u});
}

} // namespace detail

class alignas(8) OasisApplication {
public:
    explicit OasisApplication(OasisAppConfig config = {})
        : config_(std::move(config)),
          clock_(default_sim_seconds_per_real_second),
          renderer_(std::make_unique<render::WebGpuRenderer>()),
          telemetry_channel_(std::make_unique<oasis::uhai::UhaiTelemetryChannel>(
              oasis::uhai::UhaiTelemetryChannel::ChannelMode::Producer
          )),
          world_grid_(
              render::kChunkOriginX,
              render::kChunkOriginX + static_cast<int32_t>(render::kChunkSizeX) - 1,
              render::kChunkOriginZ,
              render::kChunkOriginZ + static_cast<int32_t>(render::kChunkSizeZ) - 1
          ),
          procedural_mode_active_(config_.procedural_map) {}

    bool Initialize() {
        spdlog::info(
            "[Oasis Engine] Initializing modular Phase 3B engine (headless_ci={}, procedural_map={})",
            config_.headless_ci,
            procedural_mode_active_
        );

        // 1. Register Flecs ECS components
        ecs_.component<EntitySpatialMetaComponent>();
        ecs_.component<InventoryComponent>();
        ecs_.component<PropertyBagComponent>();
        ecs_.component<SensorAttachmentComponent>();
        ecs_.component<SoilCellRegisters>();
        ecs_.component<ThermalVesselRegisters>();
        ecs_.component<AgentDrivesComponent>();
        ecs_.component<AutonomyStateComponent>();
        ecs_.component<AgentCapabilitiesComponent>();
        ecs_.component<EquippedHaulingToolComponent>();

        // 2. Bootstrap moddable ContentRegistry (inline baseline + core_catalog.json + --mod bundles)
        registry_.BootstrapCoreCatalog();
        detail::SeedHomesteadCoreCatalog(registry_);
        LoadCatalogAndModsFromDisk();
        detail::ApplyStagePreconditionsToCatalog(registry_);

        // 3. Grant Steward full homestead craft & domestic capabilities
        for (const std::string_view cap : {
                 "WOODSHOP_JOINERY", "GARAGE_ASSEMBLY", "POTABLE_WATER_TAP",
                 "SOIL_CULTIVATION_BED", "COMPOST_AND_PYROLYSIS_PAD", "POULTRY_PASTURE_RUN",
                 "CULINARY_HEAT_SOURCE", "KITCHEN_PREP_SINK", "COMMONS_TIFFIN_HANDOFF", "DINING_SEATING",
                 "COLD_LARDER_STORAGE", "HYGIENE_SHOWER", "COGNITIVE_DECOMPRESSION",
                 "L6_WORKSTATION", "SLEEP_RECOVERY", "cap.soil_amendment"}) {
            steward_caps_.GrantCapability(cap);
        }

        // 4. Register additional modeled domain URI prefixes for col-gatewayd L4 workflows
        step_adapter_.RegisterModeledDomainPrefix("col://workflow/garden/", "soil_physics", "arch.raised_garden_bed");
        step_adapter_.RegisterModeledDomainPrefix("col://workflow/soup/", "thermal_vessel", "arch.induction_hearth_stove");
        step_adapter_.RegisterModeledDomainPrefix("col://workflow/fab/", "hauling_physics", "arch.woodshop_joinery_workbench");
        step_adapter_.RegisterModeledDomainPrefix("col://workflow/mu/", "soil_physics", "arch.thermophilic_compost_drum");

        // 5. Initialize Renderer (SDL2 + 3-binding WebGPU pipeline)
        if (!renderer_->Initialize(config_.width, config_.height, config_.headless_ci)) {
            return false;
        }

        // 6. Populate WorldGrid & Flecs ECS entities from IMapProvider and upload WorldChunkGpuBuffer (@binding 2)
        RebuildWorldFromMapProvider(procedural_mode_active_);

        inner_thought_ = "Morning light across Lot 402 — open backyard turf, stocked woodshop in the garage, and calm hours ahead.";
        running_       = true;
        return true;
    }

    // Executes one 60 Hz frame: input + JS queue -> SimulationClock -> Domains & Autonomy -> WebGPU render -> L1b/UHAI telemetry.
    bool TickFrame() {
        if (!running_) {
            return false;
        }

        if (!config_.headless_ci) {
            ProcessSdlInputEvents();
        }
        if (!running_) {
            return false;
        }

#ifdef __EMSCRIPTEN__
        PollBrowserCommandQueue();
#endif

        // Advance calmly paced SimulationClock (12 sim-s / real-s at 1x -> ~200 sim_ms per 60 Hz frame at 1x)
        const uint32_t sim_dt_ms = clock_.AdvanceClampedFrameDelta(16u);
        StepSimulationSubsystems(sim_dt_ms);

        // Advance Flecs world frame
        (void)ecs_.progress(1.0f / 60.0f);

        // Sync dynamic voxels (roaming heritage pasture hens 59u once coop exists) & avatar state to WebGPU
        SyncDynamicVoxelsAndAvatarToRenderer();
        renderer_->RenderFrame(1.0f / 60.0f);

        // Emit UHAI ring buffer sample & browser L1b + L2 VirtualPinBus frame snapshot
        PushUhaiTelemetrySample();
#ifdef __EMSCRIPTEN__
        PushBrowserFrameSnapshot();
#endif

        ++tick_count_;
        if (config_.headless_ci && tick_count_ >= config_.max_ticks) {
            running_ = false;
            return false;
        }
        return running_;
    }

    int RunMainLoop() {
        while (TickFrame()) {
        }
        return 0;
    }

    static void EmscriptenMainLoopCallback(void* user_data) {
        auto* app = static_cast<OasisApplication*>(user_data);
        if (app != nullptr) {
            (void)app->TickFrame();
        }
    }

    [[nodiscard]] bool IsRunning() const noexcept { return running_; }
    [[nodiscard]] uint64_t TickCount() const noexcept { return tick_count_; }

private:
    [[nodiscard]] bool TryLoadModBundleFile(std::string_view file_path) {
        std::ifstream ifs{std::string(file_path), std::ios::in | std::ios::binary};
        if (!ifs.is_open()) {
            return false;
        }
        std::ostringstream oss;
        oss << ifs.rdbuf();
        const std::string json_text = oss.str();
        return !json_text.empty() && registry_.LoadModBundleFromJson(json_text);
    }

    void LoadCatalogAndModsFromDisk() {
        const std::array<const char*, 5> catalog_candidates = {
            "data/content/core_catalog.json",
            "../data/content/core_catalog.json",
            "../../data/content/core_catalog.json",
            "../../../core/1_simulation/data/content/core_catalog.json",
            "core/1_simulation/data/content/core_catalog.json"
        };
        for (const char* path : catalog_candidates) {
            if (TryLoadModBundleFile(path)) {
                // Re-apply fast gameplay durations / direct input-output mappings where JSON omits flat keys
                EnsureAffordanceItemMappings();
                spdlog::info("[Oasis ContentRegistry] Loaded core catalog from '{}'.", path);
                break;
            }
        }
        for (const std::string& mod_path : config_.mod_paths) {
            if (TryLoadModBundleFile(mod_path)) {
                spdlog::info("[Oasis ContentRegistry] Loaded mod bundle '{}'.", mod_path);
            } else {
                spdlog::warn("[Oasis ContentRegistry] Could not open mod bundle '{}'.", mod_path);
            }
        }
    }

    // Enriches JSON-loaded affordances with explicit item mass/phase links so HaulingPhysics evaluates every haul accurately.
    void EnsureAffordanceItemMappings() {
        auto bind_io = [&](std::string_view aff_id, std::string_view in_id, uint32_t in_q, std::string_view out_id, uint32_t out_q) {
            const AffordanceDef* existing = registry_.FindAffordance(aff_id);
            if (existing == nullptr) {
                return;
            }
            AffordanceDef copy = *existing;
            if (copy.input_item_id.empty() && !in_id.empty()) {
                copy.input_item_id = std::string(in_id);
                copy.input_qty     = in_q;
            }
            if (copy.output_item_id.empty() && !out_id.empty()) {
                copy.output_item_id = std::string(out_id);
                copy.output_qty     = out_q;
            }
            registry_.RegisterAffordance(std::move(copy));
        };

        bind_io("aff.draw_potable_water", "", 0u, "item.potable_water", 5u);
        bind_io("aff.water_garden_manual", "item.potable_water", 5u, "", 0u);
        bind_io("aff.mill_raised_bed_frame", "item.cedar_plank", 4u, "item.raised_bed_frame_kit", 1u);
        bind_io("aff.assemble_compost_and_kiln", "item.cedar_plank", 3u, "item.thermophilic_compost_drum_assembly", 1u);
        bind_io("aff.build_poultry_coop", "item.cedar_plank", 6u, "item.poultry_coop_module", 1u);
        bind_io("aff.fulfill_l7_woodwork", "item.cedar_plank", 4u, "item.custom_joinery_commission", 1u);
        bind_io("aff.plow_and_amend_bed", "item.thermophilic_compost", 15u, "", 0u);
        bind_io("aff.inoculate_and_sow_peas", "item.pea_seeds_heirloom", 1u, "", 0u);
        bind_io("aff.pyrolyze_and_quench_biochar", "", 0u, "item.pyrolysis_biochar", 8u);
        bind_io("aff.turn_and_harvest_compost", "", 0u, "item.thermophilic_compost", 25u);
        bind_io("aff.pack_and_deliver_tiffins", "item.pea_soup", 4u, "", 0u);
        detail::ApplyStagePreconditionsToCatalog(registry_);
    }

    void RebuildWorldFromMapProvider(bool use_procedural) {
        procedural_mode_active_ = use_procedural;

        // Destroy previous map entities cleanly before re-seeding
        for (flecs::entity e : map_entities_) {
            if (e.is_alive()) {
                e.destruct();
            }
        }
        map_entities_.clear();
        hens_.clear();
        pin_bus_               = VirtualPinBus{};
        active_task_           = ActiveStewardTask{};
        steward_equipped_tool_ = EquippedHaulingToolComponent{};
        steward_autonomy_.player_task_locked = false;

        std::vector<PlacedEntitySeed> seeds;
        bool loaded_static_json = false;

        if (!use_procedural) {
            std::vector<std::string> map_candidates;
            if (!config_.map_path.empty()) {
                map_candidates.push_back(config_.map_path);
            }
            map_candidates.emplace_back("data/environments/suburban_lot_402.json");
            map_candidates.emplace_back("../data/environments/suburban_lot_402.json");
            map_candidates.emplace_back("../../data/environments/suburban_lot_402.json");
            map_candidates.emplace_back("../../../core/1_simulation/data/environments/suburban_lot_402.json");
            map_candidates.emplace_back("core/1_simulation/data/environments/suburban_lot_402.json");

            for (const std::string& candidate : map_candidates) {
                StaticJsonMapProvider static_provider(candidate);
                if (static_provider.PopulateWorld(world_grid_, seeds)) {
                    loaded_static_json = true;
                    spdlog::info("[Oasis MapProvider] Loaded static JSON map from '{}'.", candidate);
                    break;
                }
            }
        }

        if (!loaded_static_json) {
            ProceduralSuburbanEarthworksGenerator proc_gen(402);
            (void)proc_gen.PopulateWorld(world_grid_, seeds);
            if (!use_procedural) {
                // Static Lot 402 fallback in WASM (without embedded FS): finished lawn/floor surface (4u) over full subsurface strata
                ConvertProceduralGradeToFinishedStaticLot402();
                spdlog::info("[Oasis MapProvider] Populated Static Lot 402 layout (WASM memory bootstrap).");
            } else {
                spdlog::info("[Oasis MapProvider] Generated Procedural Suburban Earthworks parcel.");
            }
        }

        EnsureContinuousChunkApronAndCompleteHomestead(seeds, use_procedural);
        SpawnEntitiesFromSeeds(seeds);
        RefreshVirtualPinBusEndpoints();
        renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);

        steward_pos_ = glm::vec3(12.0f, 1.0f, 6.0f);
    }

    void ConvertProceduralGradeToFinishedStaticLot402() {
        for (int32_t z = world_grid_.MinZ(); z <= world_grid_.MaxZ(); ++z) {
            for (int32_t x = world_grid_.MinX(); x <= world_grid_.MaxX(); ++x) {
                const uint16_t surf = world_grid_.GetVoxel(x, 0, z);
                if (surf == STRATA_TOPSOIL_LOAM || surf == STRATA_CONCRETE_FOOTING_SLAB) {
                    (void)world_grid_.SetVoxel(x, 0, z, MAT_SURFACE_LAWN_OR_FLOOR);
                }
            }
        }
        // Interior room partition walls with open doorways matching suburban_lot_402.json
        world_grid_.FillBox(12, 1, 3, 12, 3, 4, MAT_CLAPBOARD_WALL);
        world_grid_.FillBox(14, 1, 3, 14, 3, 4, MAT_CLAPBOARD_WALL);
        world_grid_.FillBox(13, 1, 7, 13, 3, 8, MAT_CLAPBOARD_WALL);
        (void)world_grid_.SetVoxel(11, 2, 2, MAT_WINDOW_GLASS);
        (void)world_grid_.SetVoxel(15, 2, 2, MAT_WINDOW_GLASS);
        (void)world_grid_.SetVoxel(15, 2, 10, MAT_WINDOW_GLASS);
        (void)world_grid_.SetVoxel(8, 4, 10, MAT_SHINGLE_ROOF);
    }

    // Ensures seamless subsurface/street apron across all 64x16x32 chunk cells plus all 6-room furniture,
    // trees/shrubs/mailbox, and physical garage hauling tool voxels (70u, 71u, 72u).
    void EnsureContinuousChunkApronAndCompleteHomestead(
        std::vector<PlacedEntitySeed>& seeds,
        bool                           use_procedural
    ) {
        const uint16_t yard_surface_mat = use_procedural ? STRATA_TOPSOIL_LOAM : MAT_SURFACE_LAWN_OR_FLOOR;

        for (int32_t z = world_grid_.MinZ(); z <= world_grid_.MaxZ(); ++z) {
            for (int32_t x = world_grid_.MinX(); x <= world_grid_.MaxX(); ++x) {
                if (world_grid_.GetVoxel(x, -2, z) == MAT_VOXEL_AIR) {
                    (void)world_grid_.SetVoxel(x, -2, z, STRATA_CLAY_SUBSOIL);
                }
                if (world_grid_.GetVoxel(x, -1, z) == MAT_VOXEL_AIR) {
                    const uint16_t sub_mat = (std::abs(x) <= 5) ? STRATA_COMPACTED_GRAVEL_PAD : STRATA_TOPSOIL_LOAM;
                    (void)world_grid_.SetVoxel(x, -1, z, sub_mat);
                }
                if (world_grid_.GetVoxel(x, 0, z) == MAT_VOXEL_AIR) {
                    if (x >= -3 && x <= 3) {
                        const bool cl = (x == 0) && (((z % 4) + 4) % 4 >= 2);
                        (void)world_grid_.SetVoxel(x, 0, z, cl ? MAT_ROAD_CENTERLINE : MAT_ASPHALT_ROAD);
                    } else if (std::abs(x) == 4 || std::abs(x) == 5) {
                        (void)world_grid_.SetVoxel(x, 0, z, MAT_CONCRETE_PAVEMENT);
                    } else {
                        (void)world_grid_.SetVoxel(x, 0, z, yard_surface_mat);
                    }
                }
            }
        }

        // Landscaping props: mailbox (37u), front shrubs (38u), and cedar shade trees (35u trunk, 36u crown)
        (void)world_grid_.SetVoxel(6, 1, 4, 37u);
        (void)world_grid_.SetVoxel(8, 1, 4, 38u);
        (void)world_grid_.SetVoxel(8, 1, 8, 38u);
        for (const GridCoord tree_base : {GridCoord{7, 1, 14}, GridCoord{23, 1, 3}, GridCoord{23, 1, 13}}) {
            (void)world_grid_.SetVoxel(tree_base.x, 1, tree_base.z, 35u);
            (void)world_grid_.SetVoxel(tree_base.x, 2, tree_base.z, 36u);
            (void)world_grid_.SetVoxel(tree_base.x, 3, tree_base.z, 36u);
        }

        // Multi-voxel furniture secondary cells (couch & bed)
        (void)world_grid_.SetVoxel(11, 1, 7, 48u);
        (void)world_grid_.SetVoxel(16, 1, 9, 23u);

        auto upsert_seed = [&](PlacedEntitySeed desired, bool force_override_coords = false) {
            for (auto& existing : seeds) {
                if (existing.entity_id == desired.entity_id) {
                    if (force_override_coords) {
                        existing.x = desired.x;
                        existing.y = desired.y;
                        existing.z = desired.z;
                    }
                    if (existing.voxel_material_id == 0u || force_override_coords) {
                        existing.voxel_material_id = desired.voxel_material_id;
                    }
                    if (existing.initial_item_id.empty() && !desired.initial_item_id.empty()) {
                        existing.initial_item_id  = desired.initial_item_id;
                        existing.initial_item_qty = desired.initial_item_qty;
                    }
                    return;
                }
            }
            seeds.push_back(std::move(desired));
        };

        // Ensure complete 6-room furniture set (23u, 40u..56u, 120u) across both Static and Procedural maps
        upsert_seed(PlacedEntitySeed{"entity.kitchen_fridge", "arch.refrigerator_larder", "room.kitchen_dining", "item.potable_water", 10, 1, 3, 12u, 40u, false});
        upsert_seed(PlacedEntitySeed{"entity.kitchen_induction_hearth", "arch.induction_hearth_stove", "room.kitchen_dining", "", 11, 1, 3, 0u, 41u, false});
        upsert_seed(PlacedEntitySeed{"entity.kitchen_sink_cabinet", "arch.kitchen_prep_sink_cabinet", "room.kitchen_dining", "", 12, 1, 3, 0u, 42u, false});
        upsert_seed(PlacedEntitySeed{"entity.dining_table", "arch.dining_table_set", "room.kitchen_dining", "", 10, 1, 5, 0u, 43u, false});
        upsert_seed(PlacedEntitySeed{"entity.office_workstation", "arch.homelab_cad_workstation", "room.office_homelab", "", 13, 1, 3, 0u, 120u, false});
        upsert_seed(PlacedEntitySeed{"entity.office_bookshelf", "arch.reference_archive_shelf", "room.office_homelab", "", 14, 1, 3, 0u, 44u, false});
        upsert_seed(PlacedEntitySeed{"entity.node0_server_rack", "arch.sovereign_node_rack", "zone.interior_hallways", "", 12, 1, 6, 0u, 0u, false});
        upsert_seed(PlacedEntitySeed{"entity.bathroom_shower", "arch.warm_shower_stall", "room.bathroom", "", 16, 1, 3, 0u, 45u, false});
        upsert_seed(PlacedEntitySeed{"entity.bathroom_toilet", "arch.sanitation_toilet", "room.bathroom", "", 16, 1, 4, 0u, 46u, false});
        upsert_seed(PlacedEntitySeed{"entity.bathroom_sink", "arch.bathroom_vanity_sink", "room.bathroom", "", 15, 1, 3, 0u, 47u, false});
        upsert_seed(PlacedEntitySeed{"entity.living_couch", "arch.living_room_couch", "room.living_space", "", 10, 1, 7, 0u, 48u, false});
        upsert_seed(PlacedEntitySeed{"entity.living_hifi_left", "arch.acoustic_sound_system", "room.living_space", "", 11, 1, 9, 0u, 50u, false});
        upsert_seed(PlacedEntitySeed{"entity.living_media_tv", "arch.media_display_console", "room.living_space", "", 12, 1, 9, 0u, 49u, false});
        upsert_seed(PlacedEntitySeed{"entity.living_hifi_right", "arch.acoustic_sound_system", "room.living_space", "", 13, 1, 9, 0u, 50u, false});
        upsert_seed(PlacedEntitySeed{"entity.bedroom_bed", "arch.insulated_bed_mattress", "room.bedroom", "", 15, 1, 9, 0u, 23u, false});
        upsert_seed(PlacedEntitySeed{"entity.bedroom_nightstand", "arch.bedside_nightstand", "room.bedroom", "", 16, 1, 8, 0u, 51u, false});
        upsert_seed(PlacedEntitySeed{"entity.bedroom_wardrobe", "arch.timber_wardrobe", "room.bedroom", "", 14, 1, 9, 0u, 52u, false});
        upsert_seed(PlacedEntitySeed{"entity.bedroom_mirror", "arch.standing_mirror", "room.bedroom", "", 14, 1, 8, 0u, 53u, false});
        upsert_seed(PlacedEntitySeed{"entity.garage_workbench", "arch.woodshop_joinery_workbench", "room.attached_garage", "", 9, 1, 13, 0u, 54u, false});
        upsert_seed(PlacedEntitySeed{"entity.garage_tool_wall", "arch.hand_tool_pegboard", "room.attached_garage", "", 10, 1, 13, 0u, 55u, false});
        upsert_seed(PlacedEntitySeed{"entity.garage_material_rack", "arch.garage_lumber_and_seed_rack", "room.attached_garage", "item.cedar_plank", 11, 1, 13, 14u, 56u, false});
        upsert_seed(PlacedEntitySeed{"entity.exterior_hose_spigot", "arch.exterior_water_spigot", "zone.fenced_backyard", "", 18, 1, 5, 0u, 0u, false});
        upsert_seed(PlacedEntitySeed{"tool.tiffin_carrier", "arch.hauling_tool_tiffin_carrier", "room.kitchen_dining", "tiffin_carrier", 12, 1, 4, 1u, 0u, false});

        // Physical garage hauling tools placed as 3D SDF voxels inside the garage floor bay:
        // 70u Wheelbarrow at (11, 1, 11), 71u Bucket & Watering Can at (9, 1, 12) / (10, 1, 12), 72u Hand Dolly at (11, 1, 12)
        upsert_seed(PlacedEntitySeed{"tool.wheelbarrow", "arch.hauling_tool_wheelbarrow", "room.attached_garage", "wheelbarrow", 11, 1, 11, 1u, kVoxelMatWheelbarrow, false}, true);
        upsert_seed(PlacedEntitySeed{"tool.galvanized_bucket", "arch.hauling_tool_bucket", "room.attached_garage", "bucket", 9, 1, 12, 1u, kVoxelMatLiquidVessel, false}, true);
        upsert_seed(PlacedEntitySeed{"tool.watering_can", "arch.hauling_tool_watering_can", "room.attached_garage", "watering_can", 10, 1, 12, 1u, kVoxelMatLiquidVessel, false}, true);
        upsert_seed(PlacedEntitySeed{"tool.hand_dolly", "arch.hauling_tool_hand_dolly", "room.attached_garage", "hand_dolly", 11, 1, 12, 1u, kVoxelMatHandDolly, false}, true);

        for (const auto& s : seeds) {
            if (s.voxel_material_id != 0u) {
                (void)world_grid_.SetVoxel(s.x, s.y, s.z, s.voxel_material_id);
            }
        }
    }

    void SpawnEntitiesFromSeeds(const std::vector<PlacedEntitySeed>& seeds) {
        for (const auto& seed : seeds) {
            SpawnSingleEntity(seed);
        }
    }

    flecs::entity SpawnSingleEntity(const PlacedEntitySeed& seed) {
        flecs::entity e = ecs_.entity();
        const uint64_t uid = ++next_entity_uid_;

        EntitySpatialMetaComponent meta{};
        meta.entity_id          = seed.entity_id;
        meta.archetype_id       = seed.archetype_id;
        meta.room_tag           = seed.room_tag;
        meta.entity_uid         = uid;
        meta.x                  = seed.x;
        meta.y                  = seed.y;
        meta.z                  = seed.z;
        meta.parked_material_id = seed.voxel_material_id;
        meta.is_parked_in_world = true;

        if (seed.archetype_id == "arch.hauling_tool_wheelbarrow") {
            meta.hauling_tool_id = "wheelbarrow";
        } else if (seed.archetype_id == "arch.hauling_tool_bucket") {
            meta.hauling_tool_id = "bucket";
        } else if (seed.archetype_id == "arch.hauling_tool_watering_can") {
            meta.hauling_tool_id = "watering_can";
        } else if (seed.archetype_id == "arch.hauling_tool_hand_dolly") {
            meta.hauling_tool_id = "hand_dolly";
        } else if (seed.archetype_id == "arch.hauling_tool_tiffin_carrier") {
            meta.hauling_tool_id = "tiffin_carrier";
        }

        const GridCoord stand = FindBestAdjacentStandTile(seed.x, seed.y, seed.z);
        meta.stand_x = stand.x;
        meta.stand_y = stand.y;
        meta.stand_z = stand.z;

        InventoryComponent inv{};
        if (!seed.initial_item_id.empty() && seed.initial_item_qty > 0u && meta.hauling_tool_id.empty()) {
            inv.AddItem(seed.initial_item_id, seed.initial_item_qty);
        }
        if (seed.entity_id == "entity.kitchen_fridge") {
            if (!inv.HasItem("item.potable_water")) inv.AddItem("item.potable_water", 12u);
            inv.AddItem("item.mirepoix_roots", 2u);
            inv.AddItem("item.split_peas_dry", 2u);
        } else if (seed.entity_id == "entity.garage_material_rack") {
            if (!inv.HasItem("item.cedar_plank")) inv.AddItem("item.cedar_plank", 14u);
            inv.AddItem("item.pea_seeds_heirloom", 4u);
            inv.AddItem("item.l2_soil_probe", 1u);
            inv.AddItem("item.l2_irrigation_valve", 1u);
            inv.AddItem("item.l2_culinary_rtd_controller", 1u);
        }

        PropertyBagComponent props{};
        props.Set("voxel_x", seed.x, "m");
        props.Set("voxel_y", seed.y, "m");
        props.Set("voxel_z", seed.z, "m");
        if (seed.archetype_id == "arch.raised_garden_bed") {
            props.Set("bed_stage", 0, "stage");
            props.Set("l2_probe_installed", 0, "flag");
            props.Set("l2_valve_installed", 0, "flag");
        } else if (seed.archetype_id == "arch.induction_hearth_stove" ||
                   seed.archetype_id == "arch.kitchen_prep_sink_cabinet") {
            props.Set("hearth_stage", 0, "stage");
            props.Set("l2_culinary_installed", 0, "flag");
        }

        SensorAttachmentComponent sensor{};
        sensor.sensor_node_id      = static_cast<uint32_t>(uid);
        sensor.poll_interval_sim_s = 10u;
        sensor.has_l2_sensor       = seed.has_l2_sensor; // Strictly false on Day-1 clean slate!
        sensor.actuator_enabled    = true;

        e.set<EntitySpatialMetaComponent>(std::move(meta));
        e.set<InventoryComponent>(std::move(inv));
        e.set<PropertyBagComponent>(std::move(props));
        e.set<SensorAttachmentComponent>(sensor);

        if (seed.archetype_id == "arch.induction_hearth_stove") {
            ThermalVesselRegisters vessel{};
            vessel.core_temp_mk          = clock_.AmbientTempMilliKelvin();
            vessel.target_cutoff_temp_mk = 368150u; // 95 C gentle simmer cutoff
            e.set<ThermalVesselRegisters>(vessel);
        }
        if (seed.archetype_id == "arch.raised_garden_bed" || seed.archetype_id == "arch.thermophilic_compost_drum") {
            SoilCellRegisters soil{};
            soil.moisture_vwc_bp   = 2600u;
            soil.soil_temp_mk      = clock_.AmbientTempMilliKelvin();
            soil.nitrogen_ppm      = 38u;
            soil.phosphorus_ppm    = 24u;
            soil.potassium_ppm     = 125u;
            soil.organic_carbon_bp = 1200u;
            soil.ph_centi          = 655u;
            soil.crop_planted      = 0u;
            e.set<SoilCellRegisters>(soil);
        }

        map_entities_.push_back(e);
        return e;
    }

    void SetKitchenHearthStage(int64_t stage) {
        for (flecs::entity e : map_entities_) {
            if (!e.is_alive() || !e.has<EntitySpatialMetaComponent>() || !e.has<PropertyBagComponent>()) {
                continue;
            }
            const auto& m = e.get<EntitySpatialMetaComponent>();
            if (m.archetype_id == "arch.induction_hearth_stove" ||
                m.archetype_id == "arch.kitchen_prep_sink_cabinet") {
                e.get_mut<PropertyBagComponent>().Set("hearth_stage", stage, "stage");
            }
        }
    }

    [[nodiscard]] std::string FindActiveBpmnTaskForAffordance(std::string_view aff_id) const {
        for (const auto& [mapped_aff, l4_task_id] : active_bpmn_affordance_to_task_) {
            if (mapped_aff == aff_id) {
                return l4_task_id;
            }
        }
        return {};
    }

    // Re-binds live Flecs component addresses into VirtualPinBus so table reallocations never leave stale pointers.
    void RefreshVirtualPinBusEndpoints() {
        for (flecs::entity e : map_entities_) {
            if (!e.is_alive() || !e.has<EntitySpatialMetaComponent>() || !e.has<SensorAttachmentComponent>()) {
                continue;
            }
            const auto& meta   = e.get<EntitySpatialMetaComponent>();
            const auto* sensor = &e.get<SensorAttachmentComponent>();
            if (e.has<SoilCellRegisters>()) {
                uint8_t c_code = kCultivarCodeUnset;
                if (meta.archetype_id == "arch.raised_garden_bed") {
                    c_code = active_cultivar_code_;
                    if (e.has<PropertyBagComponent>()) {
                        const int64_t stored = e.get<PropertyBagComponent>().Get(
                            "cultivar_code",
                            static_cast<int64_t>(active_cultivar_code_)
                        );
                        if (stored >= 0 && stored <= 3) {
                            c_code = static_cast<uint8_t>(stored);
                        }
                    }
                }
                pin_bus_.BindSoilEntity(
                    meta.entity_uid,
                    sensor,
                    &e.get_mut<SoilCellRegisters>(),
                    kDefaultIrrigationFlowMlPerSec,
                    c_code
                );
            }
            if (e.has<ThermalVesselRegisters>()) {
                pin_bus_.BindThermalEntity(meta.entity_uid, sensor, &e.get_mut<ThermalVesselRegisters>());
            }
        }
    }

    [[nodiscard]] GridCoord FindBestAdjacentStandTile(int32_t vx, int32_t vy, int32_t vz) const noexcept {
        if (detail::IsVoxelPassableForAgent(world_grid_.GetVoxel(vx, vy, vz))) {
            return GridCoord{vx, vy, vz};
        }
        constexpr std::array<std::pair<int32_t, int32_t>, 4> kDirs = {
            std::pair<int32_t, int32_t>{0, 1},
            std::pair<int32_t, int32_t>{0, -1},
            std::pair<int32_t, int32_t>{-1, 0},
            std::pair<int32_t, int32_t>{1, 0}
        };
        for (const auto [dx, dz] : kDirs) {
            const int32_t nx = vx + dx;
            const int32_t nz = vz + dz;
            if (world_grid_.InBounds(nx, vy, nz) &&
                detail::IsVoxelPassableForAgent(world_grid_.GetVoxel(nx, vy, nz)) &&
                world_grid_.GetVoxel(nx, 0, nz) != MAT_VOXEL_AIR) {
                return GridCoord{nx, vy, nz};
            }
        }
        return GridCoord{vx, vy, vz};
    }

    // Map-agnostic 2D/ground-floor BFS/A* pathfinder over WorldGrid (zero hardcoded room coordinates!).
    [[nodiscard]] std::vector<GridCoord> ComputePathOnWorldGrid(GridCoord start, GridCoord goal) const {
        std::vector<GridCoord> path;
        start.y = 1;
        goal.y  = 1;
        if (!world_grid_.InBounds(goal.x, 1, goal.z)) {
            return path;
        }
        if (!detail::IsVoxelPassableForAgent(world_grid_.GetVoxel(goal.x, 1, goal.z))) {
            goal = FindBestAdjacentStandTile(goal.x, 1, goal.z);
        }
        if (start == goal) {
            path.push_back(goal);
            return path;
        }

        const int32_t min_x = world_grid_.MinX();
        const int32_t min_z = world_grid_.MinZ();
        const int32_t sx    = (world_grid_.MaxX() - min_x) + 1;
        const int32_t sz    = (world_grid_.MaxZ() - min_z) + 1;
        const auto    idx_of = [&](int32_t x, int32_t z) -> size_t {
            return static_cast<size_t>((z - min_z) * sx + (x - min_x));
        };

        const size_t         total = static_cast<size_t>(sx * sz);
        std::vector<int32_t> parent(total, -1);
        std::vector<uint8_t> visited(total, 0u);
        std::queue<GridCoord> q;

        if (!world_grid_.InBounds(start.x, 1, start.z)) {
            path.push_back(goal);
            return path;
        }
        const size_t start_idx = idx_of(start.x, start.z);
        visited[start_idx]     = 1u;
        q.push(start);

        constexpr std::array<std::pair<int32_t, int32_t>, 4> kNeighbors = {
            std::pair<int32_t, int32_t>{1, 0},
            std::pair<int32_t, int32_t>{-1, 0},
            std::pair<int32_t, int32_t>{0, 1},
            std::pair<int32_t, int32_t>{0, -1}
        };

        bool found = false;
        while (!q.empty()) {
            const GridCoord cur = q.front();
            q.pop();
            if (cur.x == goal.x && cur.z == goal.z) {
                found = true;
                break;
            }
            const int32_t cur_flat = static_cast<int32_t>(idx_of(cur.x, cur.z));
            for (const auto [dx, dz] : kNeighbors) {
                const int32_t nx = cur.x + dx;
                const int32_t nz = cur.z + dz;
                if (!world_grid_.InBounds(nx, 1, nz)) {
                    continue;
                }
                const size_t n_idx = idx_of(nx, nz);
                if (visited[n_idx] != 0u) {
                    continue;
                }
                if (world_grid_.GetVoxel(nx, 0, nz) == MAT_VOXEL_AIR) {
                    continue;
                }
                if (!detail::IsVoxelPassableForAgent(world_grid_.GetVoxel(nx, 1, nz))) {
                    continue;
                }
                visited[n_idx] = 1u;
                parent[n_idx]  = cur_flat;
                q.push(GridCoord{nx, 1, nz});
            }
        }

        if (!found) {
            path.push_back(goal);
            return path;
        }

        int32_t trace = static_cast<int32_t>(idx_of(goal.x, goal.z));
        while (trace >= 0 && static_cast<size_t>(trace) != start_idx) {
            const int32_t tx = min_x + (trace % sx);
            const int32_t tz = min_z + (trace / sx);
            path.push_back(GridCoord{tx, 1, tz});
            trace = parent[static_cast<size_t>(trace)];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    [[nodiscard]] flecs::entity FindEntityById(std::string_view entity_id) const {
        if (entity_id.empty()) {
            return flecs::entity::null();
        }
        for (flecs::entity e : map_entities_) {
            if (e.is_alive() && e.has<EntitySpatialMetaComponent>()) {
                if (e.get<EntitySpatialMetaComponent>().entity_id == entity_id) {
                    return e;
                }
            }
        }
        return flecs::entity::null();
    }

    [[nodiscard]] flecs::entity FindEntityByArchetype(std::string_view archetype_id) const {
        for (flecs::entity e : map_entities_) {
            if (e.is_alive() && e.has<EntitySpatialMetaComponent>()) {
                if (e.get<EntitySpatialMetaComponent>().archetype_id == archetype_id) {
                    return e;
                }
            }
        }
        return flecs::entity::null();
    }

    [[nodiscard]] flecs::entity FindEntityAtVoxel(int32_t vx, int32_t vy, int32_t vz) const {
        for (flecs::entity e : map_entities_) {
            if (!e.is_alive() || !e.has<EntitySpatialMetaComponent>()) {
                continue;
            }
            const auto& m = e.get<EntitySpatialMetaComponent>();
            if (!m.is_parked_in_world) {
                continue;
            }
            if (m.x == vx && m.y == vy && m.z == vz) {
                return e;
            }
            // Multi-voxel couch/bed/coop secondary cell resolution
            if (m.archetype_id == "arch.living_room_couch" && vx == m.x + 1 && vy == m.y && vz == m.z) {
                return e;
            }
            if (m.archetype_id == "arch.insulated_bed_mattress" && vx == m.x + 1 && vy == m.y && vz == m.z) {
                return e;
            }
            if (m.archetype_id == "arch.poultry_coop" && vx == m.x && vy == m.y && vz == m.z + 1) {
                return e;
            }
        }
        return flecs::entity::null();
    }

    // Dynamically discovers an open unpaved yard plot in WorldGrid (no hardcoded build coordinates).
    [[nodiscard]] GridCoord FindOpenBackyardPlot(int32_t prefer_z_min = 4, int32_t prefer_z_max = 12) const noexcept {
        for (int32_t x = 19; x <= 23; ++x) {
            for (int32_t z = prefer_z_min; z <= prefer_z_max; ++z) {
                if (!world_grid_.InBounds(x, 1, z)) {
                    continue;
                }
                const uint16_t ground = world_grid_.GetVoxel(x, 0, z);
                const uint16_t above  = world_grid_.GetVoxel(x, 1, z);
                if ((ground == MAT_SURFACE_LAWN_OR_FLOOR || ground == STRATA_TOPSOIL_LOAM) &&
                    above == MAT_VOXEL_AIR) {
                    return GridCoord{x, 1, z};
                }
            }
        }
        return GridCoord{20, 1, 6};
    }

    // Consults HaulingPhysics::EvaluateHaulPlan to determine if an affordance requires a garage hauling tool.
    struct ResolvedHaulRequirement {
        std::string              tool_entity_id{};
        std::string              tool_id{};
        std::string              ui_note{};
        uint32_t                 effective_speed_bp{kNominalWalkSpeedBp};
        bool                     needs_tool_fetch{false};
        [[maybe_unused]] uint8_t pad_[3]{0u, 0u, 0u};
    };

    [[nodiscard]] ResolvedHaulRequirement EvaluateAffordanceHaulRequirement(
        const AffordanceDef& aff,
        int32_t              target_x,
        int32_t              target_z
    ) const {
        ResolvedHaulRequirement req{};
        const ItemDef*          governing_item = nullptr;
        uint32_t                governing_qty  = 0u;

        if (!aff.input_item_id.empty() && aff.input_qty > 0u) {
            governing_item = registry_.FindItem(aff.input_item_id);
            governing_qty  = aff.input_qty;
        }
        if (!aff.output_item_id.empty() && aff.output_qty > 0u) {
            const ItemDef* out_item = registry_.FindItem(aff.output_item_id);
            if (out_item != nullptr &&
                (governing_item == nullptr ||
                 out_item->matter_phase == MatterPhase::LIQUID ||
                 out_item->matter_phase == MatterPhase::GRANULAR_PARTICULATE ||
                 out_item->unit_mass_g > kBarehandMaxSolidLiftMassG)) {
                governing_item = out_item;
                governing_qty  = aff.output_qty;
            }
        }
        if (governing_item == nullptr || governing_qty == 0u) {
            return req;
        }

        const bool path_is_paved = world_grid_.IsPathPavedOrGraded(target_x, target_z);
        const HaulingToolDef* currently_equipped = steward_equipped_tool_.ResolveTool(registry_);
        const HaulEvaluation  cur_eval = EvaluateHaulPlan(*governing_item, governing_qty, currently_equipped, path_is_paved);

        if (currently_equipped != nullptr &&
            cur_eval.feasibility == HaulFeasibility::FEASIBLE) {
            req.effective_speed_bp = cur_eval.effective_speed_bp;
            req.tool_id            = currently_equipped->tool_id;
            req.ui_note            = "Equipped " + currently_equipped->display_name;
            return req;
        }

        const uint64_t total_mass_kg =
            (static_cast<uint64_t>(governing_item->unit_mass_g) * static_cast<uint64_t>(governing_qty)) / 1000ULL;

        const bool wants_tool =
            (governing_item->matter_phase == MatterPhase::LIQUID) ||
            (governing_item->matter_phase == MatterPhase::GRANULAR_PARTICULATE && total_mass_kg >= 5ULL) ||
            (governing_item->unit_mass_g > kBarehandMaxSolidLiftMassG);

        if (!wants_tool && IsHaulPossible(cur_eval.feasibility)) {
            req.effective_speed_bp = cur_eval.effective_speed_bp;
            return req;
        }

        // Search live parked hauling tool entities on the parcel for the best physical match
        for (flecs::entity e : map_entities_) {
            if (!e.is_alive() || !e.has<EntitySpatialMetaComponent>()) {
                continue;
            }
            const auto& m = e.get<EntitySpatialMetaComponent>();
            if (m.hauling_tool_id.empty() || !m.is_parked_in_world) {
                continue;
            }
            const HaulingToolDef* cand_def = registry_.FindHaulingTool(m.hauling_tool_id);
            if (cand_def == nullptr) {
                continue;
            }
            // Prefer watering_can for garden irrigation, tiffin_carrier for soup sharing, hand_dolly on paved slab, wheelbarrow on yard
            if (aff.affordance_id == "aff.water_garden_manual" && m.hauling_tool_id != "watering_can" && m.hauling_tool_id != "bucket") {
                continue;
            }
            if (aff.affordance_id == "aff.pack_and_deliver_tiffins" && m.hauling_tool_id != "tiffin_carrier") {
                continue;
            }
            const HaulEvaluation eval = EvaluateHaulPlan(*governing_item, governing_qty, cand_def, path_is_paved);
            if (eval.feasibility == HaulFeasibility::FEASIBLE) {
                req.tool_entity_id     = m.entity_id;
                req.tool_id            = m.hauling_tool_id;
                req.effective_speed_bp = eval.effective_speed_bp;
                req.needs_tool_fetch   = true;
                if (governing_item->matter_phase == MatterPhase::LIQUID) {
                    req.ui_note = "Requires " + cand_def->display_name + " (Liquid)";
                } else {
                    req.ui_note = "Uses " + cand_def->display_name + " (" + std::to_string(std::max<uint64_t>(1ULL, total_mass_kg)) + " kg)";
                }
                return req;
            }
        }
        return req;
    }

    bool StartAffordanceExecution(
        std::string_view affordance_id,
        std::string_view target_entity_id,
        int32_t          vx,
        int32_t          vy,
        int32_t          vz
    ) {
        if (affordance_id == "aff.look" || affordance_id == kAffordanceLookId) {
            flecs::entity target = !target_entity_id.empty() ? FindEntityById(target_entity_id) : FindEntityAtVoxel(vx, vy, vz);
            if (target.is_alive() && target.has<EntitySpatialMetaComponent>()) {
                const auto* arch = registry_.FindArchetype(target.get<EntitySpatialMetaComponent>().archetype_id);
                if (arch != nullptr) {
                    inner_thought_ = arch->look_description;
                }
            }
            return true;
        }

        const AffordanceDef* aff = registry_.FindAffordance(affordance_id);
        AffordanceDef        fallback_aff{};
        if (aff == nullptr) {
            fallback_aff.affordance_id  = std::string(affordance_id);
            fallback_aff.label          = std::string(affordance_id);
            fallback_aff.duration_sim_s = 240u;
            aff                         = &fallback_aff;
        }

        flecs::entity target = !target_entity_id.empty() ? FindEntityById(target_entity_id) : FindEntityAtVoxel(vx, vy, vz);
        int32_t       tx     = vx;
        int32_t       ty     = std::max<int32_t>(1, vy);
        int32_t       tz     = vz;
        std::string   resolved_target_id(target_entity_id);

        if (target.is_alive() && target.has<EntitySpatialMetaComponent>()) {
            const auto& m      = target.get<EntitySpatialMetaComponent>();
            resolved_target_id = m.entity_id;
            tx                 = m.stand_x;
            ty                 = m.stand_y;
            tz                 = m.stand_z;
        } else {
            const GridCoord stand = FindBestAdjacentStandTile(vx, 1, vz);
            tx = stand.x;
            ty = stand.y;
            tz = stand.z;
        }

        // For backyard assemblies fabricated from garage workbench, evaluate haul terrain at the backyard destination!
        int32_t haul_check_x = tx;
        int32_t haul_check_z = tz;
        if (affordance_id == "aff.assemble_compost_and_kiln" || affordance_id == "aff.build_poultry_coop") {
            const GridCoord yard_plot = FindOpenBackyardPlot(8, 13);
            haul_check_x = yard_plot.x;
            haul_check_z = yard_plot.z;
        } else if (affordance_id == "aff.fulfill_l7_woodwork") {
            haul_check_x = 7; // Paved concrete driveway apron -> selects Hand Dolly (72u)!
            haul_check_z = 12;
        }

        const ResolvedHaulRequirement haul_req = EvaluateAffordanceHaulRequirement(*aff, haul_check_x, haul_check_z);

        ActiveStewardTask task{};
        task.affordance_id    = aff->affordance_id;
        task.label            = aff->label;
        task.target_entity_id = resolved_target_id;
        task.l4_task_id       = FindActiveBpmnTaskForAffordance(aff->affordance_id);
        task.target_x         = tx;
        task.target_y         = ty;
        task.target_z         = tz;
        task.walk_speed_bp    = (haul_req.effective_speed_bp > 0u) ? haul_req.effective_speed_bp : kNominalWalkSpeedBp;

        const uint32_t capped_sim_s = std::clamp(aff->duration_sim_s, 15u, 900u);
        task.work_timer.Start(capped_sim_s * kSimMsPerSecond);

        const GridCoord cur_grid{
            static_cast<int32_t>(std::lround(steward_pos_.x)),
            1,
            static_cast<int32_t>(std::lround(steward_pos_.z))
        };

        if (haul_req.needs_tool_fetch && !haul_req.tool_entity_id.empty()) {
            flecs::entity tool_ent = FindEntityById(haul_req.tool_entity_id);
            if (tool_ent.is_alive() && tool_ent.has<EntitySpatialMetaComponent>()) {
                const auto& tm              = tool_ent.get<EntitySpatialMetaComponent>();
                task.required_tool_entity_id = tm.entity_id;
                task.required_tool_id        = tm.hauling_tool_id;
                task.auto_park_tool_after    = true;
                task.stage                   = TaskStage::WALK_TO_FETCH_TOOL;
                task.current_path            = ComputePathOnWorldGrid(cur_grid, GridCoord{tm.stand_x, 1, tm.stand_z});
                task.path_cursor             = 0u;
                inner_thought_               = "Heading to the garage bay to grab the " + tm.hauling_tool_id + " first.";
            }
        }

        if (task.stage == TaskStage::IDLE) {
            task.stage        = TaskStage::WALK_TO_STATION;
            task.current_path = ComputePathOnWorldGrid(cur_grid, GridCoord{tx, 1, tz});
            task.path_cursor  = 0u;
        }

        active_task_                         = std::move(task);
        steward_autonomy_.player_task_locked = true;
        return true;
    }

    // Routes an incoming Layer 4 workflow task / col:// URI through UnmodeledStepAdapter + HaulingPhysics!
    void ExecuteL4TaskCommand(std::string_view task_id, std::string_view uri_override) {
        L4WorkflowStepSpec spec{};
        std::string        mapped_affordance;

        if (!uri_override.empty()) {
            spec.step_action_uri = std::string(uri_override);
        }

        // Map col-gatewayd task IDs & domain URIs cleanly onto physical domain URIs + data-driven affordances
        if (task_id.starts_with("task:fab_1") || spec.step_action_uri.find("mill_raised_bed") != std::string::npos) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/fab/mill_raised_bed_frame";
            spec.preferred_station_archetype = "arch.woodshop_joinery_workbench";
            mapped_affordance                = "aff.mill_raised_bed_frame";
        } else if (task_id.starts_with("task:fab_2") || spec.step_action_uri.find("compost_and_kiln") != std::string::npos ||
                   task_id.find("compost_kiln") != std::string_view::npos) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/logistics/assemble_compost_and_kiln";
            spec.preferred_station_archetype = "arch.woodshop_joinery_workbench";
            mapped_affordance                = "aff.assemble_compost_and_kiln";
        } else if (task_id.starts_with("task:fab_3") || spec.step_action_uri.find("poultry_coop") != std::string::npos ||
                   task_id.find("poultry_coop") != std::string_view::npos) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/logistics/build_poultry_coop";
            spec.preferred_station_archetype = "arch.woodshop_joinery_workbench";
            mapped_affordance                = "aff.build_poultry_coop";
        } else if (task_id.starts_with("task:fab_4") || spec.step_action_uri.find("l7_woodwork") != std::string::npos ||
                   task_id.find("woodwork") != std::string_view::npos) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/logistics/fulfill_l7_woodwork";
            spec.preferred_station_archetype = "arch.woodshop_joinery_workbench";
            mapped_affordance                = "aff.fulfill_l7_woodwork";
        } else if (task_id.starts_with("task:pea_1")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/soil/plow_and_amend";
            spec.preferred_station_archetype = "arch.raised_garden_bed";
            spec.required_registers          = {"organic_carbon_bp", "nitrogen_ppm"};
            mapped_affordance                = "aff.plow_and_amend_bed";
        } else if (task_id.starts_with("task:pea_2")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/garden/erect_trellis";
            spec.preferred_station_archetype = "arch.raised_garden_bed";
            mapped_affordance                = "aff.erect_pea_trellis";
        } else if (task_id.starts_with("task:pea_3")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/soil/inoculate_rhizobium";
            spec.preferred_station_archetype = "arch.raised_garden_bed";
            spec.required_registers          = {"is_legume_fixing_n", "inoculant_CFU_per_ml"};
            mapped_affordance                = "aff.inoculate_and_sow_peas";
        } else if (task_id.starts_with("task:pea_4")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/soil/irrigate_bed";
            spec.preferred_station_archetype = "arch.raised_garden_bed";
            spec.required_registers          = {"moisture_vwc_bp"};
            mapped_affordance                = "aff.water_garden_manual";
        } else if (task_id.starts_with("task:pea_5")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/soil/harvest_pods";
            spec.preferred_station_archetype = "arch.raised_garden_bed";
            spec.required_registers          = {"crop_growth_bp"};
            mapped_affordance                = "aff.harvest_peas_and_roots";
        } else if (task_id.starts_with("task:pea_6")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/garden/shell_and_dry_peas";
            spec.preferred_station_archetype = "arch.raised_garden_bed";
            spec.required_registers          = {"pod_moisture_bp"};
            mapped_affordance                = "aff.shell_dry_and_turn_bed";
        } else if (task_id == "task:soup_1_gather_inputs") {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/soup/gather_inputs";
            spec.preferred_station_archetype = "arch.kitchen_prep_sink_cabinet";
            mapped_affordance                = "aff.gather_soup_inputs";
        } else if (task_id == "task:soup_1" || task_id.starts_with("task:soup_2")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/thermal/simmer_bone_broth";
            spec.preferred_station_archetype = "arch.induction_hearth_stove";
            spec.required_registers          = {"core_temp_mk", "pasteurization_hold_s"};
            mapped_affordance                = "aff.simmer_bone_broth";
        } else if (task_id.starts_with("task:soup_3")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/thermal/simmer_pea_soup";
            spec.preferred_station_archetype = "arch.induction_hearth_stove";
            spec.required_registers          = {"core_temp_mk", "pasteurization_hold_s"};
            mapped_affordance                = "aff.cook_pea_soup";
        } else if (task_id.starts_with("task:soup_4") || task_id.find("distribute") != std::string_view::npos) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/logistics/pack_and_deliver_tiffins";
            spec.preferred_station_archetype = "arch.kitchen_prep_sink_cabinet";
            mapped_affordance                = "aff.pack_and_deliver_tiffins";
        } else if (task_id.find("feed_bsfl") != std::string_view::npos ||
                   task_id.find("collect_eggs") != std::string_view::npos ||
                   task_id.starts_with("task:mu_1")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/mu/tend_pasture_hens";
            spec.preferred_station_archetype = "arch.poultry_coop";
            mapped_affordance                = "aff.tend_hens_and_collect_eggs";
        } else if (task_id.find("compost") != std::string_view::npos || task_id.starts_with("task:mu_2")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/mu/turn_compost";
            spec.preferred_station_archetype = "arch.thermophilic_compost_drum";
            mapped_affordance                = "aff.turn_and_harvest_compost";
        } else if (task_id.find("biochar") != std::string_view::npos || task_id.starts_with("task:mu_3")) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/mu/pyrolyze_biochar";
            spec.preferred_station_archetype = "arch.tlud_biochar_kiln";
            mapped_affordance                = "aff.pyrolyze_and_quench_biochar";
        } else if (task_id == "task:infra_1_cistern_first_flush") {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/soil/irrigate_bed";
            spec.preferred_station_archetype = "arch.exterior_water_spigot";
            mapped_affordance                = "aff.inspect";
        } else if (task_id.starts_with("task:infra_1") || task_id.find("soil_probe") != std::string_view::npos) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/soil/install_l2_probe";
            spec.preferred_station_archetype = "arch.raised_garden_bed";
            mapped_affordance                = "aff.install_l2_soil_probe";
        } else if (task_id == "task:infra_2_solar_bus_balance") {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://workflow/infra/solar_bus_balance";
            spec.preferred_station_archetype = "arch.homelab_cad_workstation";
            mapped_affordance                = "aff.study_at_workstation";
        } else if (task_id.starts_with("task:infra_2") || task_id.find("culinary") != std::string_view::npos) {
            if (spec.step_action_uri.empty()) spec.step_action_uri = "col://domain/thermal/install_l2_controller";
            spec.preferred_station_archetype = "arch.induction_hearth_stove";
            mapped_affordance                = "aff.attach_l2_culinary_controller";
        } else {
            if (spec.step_action_uri.empty()) {
                spec.step_action_uri = "col://workflow/custom/" + std::string(task_id);
            }
            spec.preferred_station_archetype = "arch.homelab_cad_workstation";
            mapped_affordance                = "aff.study_at_workstation";
        }

        const StepAdapterResolution res = step_adapter_.ResolveWorkflowStep(spec);
        if (res.mode == ResolutionMode::PLAYER_OBSERVATION_PROMPT) {
            inner_thought_ = res.player_prompt_text;
            return;
        }

        flecs::entity station = FindEntityByArchetype(res.station_archetype_id);
        if (!station.is_alive()) {
            // If garden bed isn't milled yet, route Steward to woodshop workbench first!
            station = FindEntityByArchetype("arch.woodshop_joinery_workbench");
            if (res.station_archetype_id == "arch.raised_garden_bed") {
                mapped_affordance = "aff.mill_raised_bed_frame";
            } else if (res.station_archetype_id == "arch.poultry_coop") {
                mapped_affordance = "aff.build_poultry_coop";
            } else if (res.station_archetype_id == "arch.tlud_biochar_kiln" ||
                       res.station_archetype_id == "arch.thermophilic_compost_drum") {
                mapped_affordance = "aff.assemble_compost_and_kiln";
            }
        }

        if (station.is_alive() && station.has<EntitySpatialMetaComponent>()) {
            const auto& m = station.get<EntitySpatialMetaComponent>();
            if (StartAffordanceExecution(mapped_affordance, m.entity_id, m.x, m.y, m.z)) {
                active_task_.l4_task_id = std::string(task_id);
            }
        }
    }

    void StepSimulationSubsystems(uint32_t sim_dt_ms) {
        if (sim_dt_ms == 0u) {
            return;
        }

        // 1. Step SoilPhysics & ThermalVessel domains at 1-sim-second granularity via VirtualPinBus
        domain_sub_sec_ms_ += sim_dt_ms;
        const uint32_t whole_sim_s = domain_sub_sec_ms_ / kSimMsPerSecond;
        domain_sub_sec_ms_ %= kSimMsPerSecond;

        if (whole_sim_s > 0u) {
            RefreshVirtualPinBusEndpoints();
            pin_bus_.StepBoundDomains(clock_.AmbientTempMilliKelvin(), whole_sim_s);

            for (flecs::entity e : map_entities_) {
                if (!e.is_alive() || !e.has<EntitySpatialMetaComponent>() ||
                    !e.has<SoilCellRegisters>() || !e.has<PropertyBagComponent>()) {
                    continue;
                }
                const auto& meta = e.get<EntitySpatialMetaComponent>();
                if (meta.archetype_id != "arch.raised_garden_bed") {
                    continue;
                }
                const auto& soil  = e.get<SoilCellRegisters>();
                auto&       props = e.get_mut<PropertyBagComponent>();
                if (soil.crop_planted != 0u && props.Get("bed_stage", 0) == 3) {
                    const uint8_t c_code = static_cast<uint8_t>(
                        std::clamp<int64_t>(props.Get("cultivar_code", static_cast<int64_t>(active_cultivar_code_)), 0, 3)
                    );
                    const CultivarPhysicsSpec spec = ResolveCultivarByCode(c_code);
                    if (soil.crop_growth_bp >= spec.harvest_mature_bp) {
                        props.Set("bed_stage", 4, "stage");
                        inner_thought_ = "The " + std::string(CultivarDisplayNameFromCode(c_code)) +
                                         " vines on the backyard trellis have reached full pod maturity — ready to harvest!";
                    }
                }
            }
        }

        // 2. Step active Steward task & hauling physics state machine
        StepStewardActiveTask(sim_dt_ms);

        // 3. Step Steward UtilityAutonomySystem (20-40 sim-min calm dwells & ThoughtRuleDef introspection)
        std::string new_thought;
        const AutonomousActivity prev_act = steward_autonomy_.current_activity;
        const AutonomousActivity cur_act  = UtilityAutonomySystem::StepHumanAutonomy(
            steward_drives_,
            steward_autonomy_,
            sim_dt_ms,
            &registry_,
            &new_thought
        );
        if (!new_thought.empty()) {
            inner_thought_ = new_thought;
        }
        if (!steward_autonomy_.player_task_locked && cur_act != prev_act) {
            RouteStewardToAutonomousHabitStation(cur_act);
        }
        if (!steward_autonomy_.player_task_locked && !autonomy_walk_path_.empty()) {
            AdvanceAgentAlongPath(steward_pos_, autonomy_walk_path_, autonomy_walk_cursor_, sim_dt_ms, kNominalWalkSpeedBp);
        }

        // 4. Step autonomous heritage pasture chickens once a poultry coop entity exists
        StepPastureChickens(sim_dt_ms);

        // 5. Emergency warp brake if physiological reserves drop critically while at >= 5x warp
        if (clock_.WarpMultiplier() >= 5u &&
            (steward_drives_.hunger_bp < kEmergencyInterruptBp ||
             steward_drives_.hydration_bp < kEmergencyInterruptBp ||
             steward_drives_.energy_bp < 1000u)) {
            clock_.EmergencyDownshiftTo1x();
            inner_thought_ = "Slowing down to 1x — need water, food, and a breather.";
        }
    }

    void RouteStewardToAutonomousHabitStation(AutonomousActivity act) {
        std::string_view target_arch = "arch.living_room_couch";
        switch (act) {
            case AutonomousActivity::HUMAN_READING:       target_arch = "arch.reference_archive_shelf"; break;
            case AutonomousActivity::HUMAN_GAMING_MEDIA:  target_arch = "arch.living_room_couch"; break;
            case AutonomousActivity::HUMAN_SOCIAL_CHAT:   target_arch = "arch.homelab_cad_workstation"; break;
            case AutonomousActivity::HUMAN_INTROSPECTION: target_arch = "arch.dining_table_set"; break;
            case AutonomousActivity::HUMAN_MEAL:          target_arch = "arch.dining_table_set"; break;
            case AutonomousActivity::HUMAN_HYDRATE:       target_arch = "arch.kitchen_prep_sink_cabinet"; break;
            case AutonomousActivity::HUMAN_SLEEP_REST:    target_arch = "arch.insulated_bed_mattress"; break;
            default:                                      return;
        }
        flecs::entity st = FindEntityByArchetype(target_arch);
        if (st.is_alive() && st.has<EntitySpatialMetaComponent>()) {
            const auto& m = st.get<EntitySpatialMetaComponent>();
            const GridCoord cur{
                static_cast<int32_t>(std::lround(steward_pos_.x)),
                1,
                static_cast<int32_t>(std::lround(steward_pos_.z))
            };
            autonomy_walk_path_   = ComputePathOnWorldGrid(cur, GridCoord{m.stand_x, 1, m.stand_z});
            autonomy_walk_cursor_ = 0u;
        }
    }

    static bool AdvanceAgentAlongPath(
        glm::vec3&                    pos,
        const std::vector<GridCoord>& path,
        size_t&                       cursor,
        uint32_t                      sim_dt_ms,
        uint32_t                      speed_bp
    ) noexcept {
        if (cursor >= path.size()) {
            return true;
        }
        // Calm, human walking speed (~1.35 m/s in simulated time, scaled smoothly for comfortable visual motion)
        const float base_m_per_sim_s = 0.55f * (static_cast<float>(speed_bp) / 10000.0f);
        float       budget_m         = std::clamp(base_m_per_sim_s * (static_cast<float>(sim_dt_ms) / 1000.0f), 0.02f, 0.85f);

        while (budget_m > 1e-4f && cursor < path.size()) {
            const glm::vec3 wp(
                static_cast<float>(path[cursor].x),
                1.0f,
                static_cast<float>(path[cursor].z)
            );
            const glm::vec3 delta = wp - pos;
            const float     dist  = glm::length(delta);
            if (dist <= budget_m) {
                pos = wp;
                budget_m -= dist;
                ++cursor;
            } else {
                pos += (delta / dist) * budget_m;
                budget_m = 0.0f;
            }
        }
        return cursor >= path.size();
    }

    void StepStewardActiveTask(uint32_t sim_dt_ms) {
        if (active_task_.stage == TaskStage::IDLE) {
            return;
        }

        if (active_task_.stage == TaskStage::WALK_TO_FETCH_TOOL) {
            if (AdvanceAgentAlongPath(
                    steward_pos_,
                    active_task_.current_path,
                    active_task_.path_cursor,
                    sim_dt_ms,
                    kNominalWalkSpeedBp)) {
                EquipParkedHaulingTool(active_task_.required_tool_entity_id);
                const GridCoord cur{
                    static_cast<int32_t>(std::lround(steward_pos_.x)),
                    1,
                    static_cast<int32_t>(std::lround(steward_pos_.z))
                };
                active_task_.current_path = ComputePathOnWorldGrid(
                    cur,
                    GridCoord{active_task_.target_x, 1, active_task_.target_z}
                );
                active_task_.path_cursor = 0u;
                active_task_.stage       = TaskStage::WALK_TO_STATION;
            }
            return;
        }

        if (active_task_.stage == TaskStage::WALK_TO_STATION) {
            if (AdvanceAgentAlongPath(
                    steward_pos_,
                    active_task_.current_path,
                    active_task_.path_cursor,
                    sim_dt_ms,
                    active_task_.walk_speed_bp)) {
                active_task_.stage = TaskStage::PERFORM_WORK;
            }
            return;
        }

        if (active_task_.stage == TaskStage::PERFORM_WORK) {
            if (active_task_.work_timer.Advance(sim_dt_ms)) {
                ApplyCompletedAffordanceEffects();
                if (active_task_.auto_park_tool_after && !steward_equipped_tool_.source_rack_entity_id.empty()) {
                    flecs::entity tool_ent = FindEntityById(steward_equipped_tool_.source_rack_entity_id);
                    if (tool_ent.is_alive() && tool_ent.has<EntitySpatialMetaComponent>()) {
                        const auto& tm = tool_ent.get<EntitySpatialMetaComponent>();
                        const GridCoord cur{
                            static_cast<int32_t>(std::lround(steward_pos_.x)),
                            1,
                            static_cast<int32_t>(std::lround(steward_pos_.z))
                        };
                        active_task_.current_path = ComputePathOnWorldGrid(
                            cur,
                            GridCoord{tm.stand_x, 1, tm.stand_z}
                        );
                        active_task_.path_cursor = 0u;
                        active_task_.stage       = TaskStage::WALK_TO_PARK_TOOL;
                        return;
                    }
                }
                FinishActiveTask();
            }
            return;
        }

        if (active_task_.stage == TaskStage::WALK_TO_PARK_TOOL) {
            if (AdvanceAgentAlongPath(
                    steward_pos_,
                    active_task_.current_path,
                    active_task_.path_cursor,
                    sim_dt_ms,
                    kNominalWalkSpeedBp)) {
                ParkEquippedHaulingTool();
                FinishActiveTask();
            }
        }
    }

    void EquipParkedHaulingTool(std::string_view tool_entity_id) {
        if (!steward_equipped_tool_.source_rack_entity_id.empty()) {
            ParkEquippedHaulingTool();
        }
        flecs::entity e = FindEntityById(tool_entity_id);
        if (!e.is_alive() || !e.has<EntitySpatialMetaComponent>()) {
            return;
        }
        auto& m                                      = e.get_mut<EntitySpatialMetaComponent>();
        steward_equipped_tool_.tool_id               = m.hauling_tool_id;
        steward_equipped_tool_.source_rack_entity_id = m.entity_id;
        m.is_parked_in_world                         = false;
        if (m.parked_material_id != 0u && world_grid_.GetVoxel(m.x, m.y, m.z) == m.parked_material_id) {
            (void)world_grid_.SetVoxel(m.x, m.y, m.z, MAT_VOXEL_AIR);
            renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
        }
    }

    void ParkEquippedHaulingTool() {
        if (steward_equipped_tool_.source_rack_entity_id.empty()) {
            steward_equipped_tool_ = EquippedHaulingToolComponent{};
            return;
        }
        flecs::entity e = FindEntityById(steward_equipped_tool_.source_rack_entity_id);
        if (e.is_alive() && e.has<EntitySpatialMetaComponent>()) {
            auto& m              = e.get_mut<EntitySpatialMetaComponent>();
            m.is_parked_in_world = true;
            if (m.parked_material_id != 0u && world_grid_.GetVoxel(m.x, m.y, m.z) == MAT_VOXEL_AIR) {
                (void)world_grid_.SetVoxel(m.x, m.y, m.z, m.parked_material_id);
                renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
            }
        }
        steward_equipped_tool_ = EquippedHaulingToolComponent{};
    }

    void FinishActiveTask() noexcept {
        if (!active_task_.l4_task_id.empty()) {
            last_completed_l4_task_id_ = active_task_.l4_task_id;
        }
        active_task_                         = ActiveStewardTask{};
        steward_autonomy_.player_task_locked = false;
    }

    void ApplyCompletedAffordanceEffects() {
        const std::string& aff_id = active_task_.affordance_id;
        flecs::entity      target = FindEntityById(active_task_.target_entity_id);

        if (aff_id == "aff.pickup_hauling_tool") {
            EquipParkedHaulingTool(active_task_.target_entity_id);
            inner_thought_ = "Picked up the " + steward_equipped_tool_.tool_id + " — ready to haul.";
            return;
        }

        if (aff_id == "aff.inspect" || aff_id == kAffordanceInspectId) {
            if (target.is_alive() && target.has<EntitySpatialMetaComponent>()) {
                const auto& m    = target.get<EntitySpatialMetaComponent>();
                const auto* arch = registry_.FindArchetype(m.archetype_id);
                if (arch != nullptr) {
                    const auto rep = InspectEntityManual(
                        *arch,
                        target.has<PropertyBagComponent>() ? &target.get<PropertyBagComponent>() : nullptr,
                        target.has<SoilCellRegisters>() ? &target.get<SoilCellRegisters>() : nullptr,
                        target.has<ThermalVesselRegisters>() ? &target.get<ThermalVesselRegisters>() : nullptr,
                        target.has<InventoryComponent>() ? &target.get<InventoryComponent>() : nullptr,
                        target.has<SensorAttachmentComponent>() ? &target.get<SensorAttachmentComponent>() : nullptr
                    );
                    inner_thought_ = "Inspected " + rep.headline + ": " + rep.look_description;
                    std::ostringstream ioss;
                    ioss << "{"
                         << "\"headline\":\"" << detail::JsonEscape(rep.headline) << "\","
                         << "\"look_description\":\"" << detail::JsonEscape(rep.look_description) << "\","
                         << "\"has_live_l2_telemetry\":" << (rep.has_live_l2_telemetry ? "true" : "false") << ","
                         << "\"revealed_properties\":[";
                    for (size_t i = 0u; i < rep.revealed_properties.size(); ++i) {
                        const auto& p = rep.revealed_properties[i];
                        if (i > 0u) ioss << ",";
                        ioss << "{\"key\":\"" << detail::JsonEscape(p.key) << "\","
                             << "\"value\":" << p.value << ","
                             << "\"unit_hint\":\"" << detail::JsonEscape(p.unit_hint) << "\"}";
                    }
                    ioss << "],\"revealed_inventory\":[";
                    for (size_t i = 0u; i < rep.revealed_inventory.size(); ++i) {
                        const auto& s = rep.revealed_inventory[i];
                        if (i > 0u) ioss << ",";
                        ioss << "{\"item_id\":\"" << detail::JsonEscape(s.item_id) << "\","
                             << "\"quantity\":" << s.quantity << "}";
                    }
                    ioss << "]}";
                    pending_inspection_report_json_ = ioss.str();
                }
            }
            return;
        }

        if (aff_id == "aff.mill_raised_bed_frame") {
            ConsumeItemFromParcel("item.cedar_plank", 4u);
            const GridCoord plot   = FindOpenBackyardPlot(4, 7);
            const std::string b_id = "entity.garden_bed_" + std::to_string(++built_bed_count_);
            (void)world_grid_.SetVoxel(plot.x, 1, plot.z, MAT_TILLED_GARDEN_SOIL);
            SpawnSingleEntity(PlacedEntitySeed{
                b_id, "arch.raised_garden_bed", "zone.fenced_backyard", "", plot.x, 1, plot.z, 0u, MAT_TILLED_GARDEN_SOIL, false
            });
            RefreshVirtualPinBusEndpoints();
            renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
            inner_thought_ = "Set a mortised cedar raised bed onto the backyard loam — ready for compost, biochar, and pea seeds.";
            return;
        }

        if (aff_id == "aff.assemble_compost_and_kiln") {
            ConsumeItemFromParcel("item.cedar_plank", 3u);
            if (!FindEntityByArchetype("arch.tlud_biochar_kiln").is_alive()) {
                const GridCoord p_kiln = FindOpenBackyardPlot(8, 10);
                (void)world_grid_.SetVoxel(p_kiln.x, 1, p_kiln.z, kVoxelMatBiocharKiln);
                SpawnSingleEntity(PlacedEntitySeed{
                    "entity.biochar_kiln", "arch.tlud_biochar_kiln", "zone.fenced_backyard", "", p_kiln.x, 1, p_kiln.z, 0u, kVoxelMatBiocharKiln, false
                });
            }
            if (!FindEntityByArchetype("arch.thermophilic_compost_drum").is_alive()) {
                const GridCoord p_drum = FindOpenBackyardPlot(8, 10);
                (void)world_grid_.SetVoxel(p_drum.x, 1, p_drum.z, kVoxelMatCompostDrum);
                SpawnSingleEntity(PlacedEntitySeed{
                    "entity.compost_drum", "arch.thermophilic_compost_drum", "zone.fenced_backyard", "", p_drum.x, 1, p_drum.z, 0u, kVoxelMatCompostDrum, false
                });
            }
            RefreshVirtualPinBusEndpoints();
            renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
            inner_thought_ = "Wheeled the smokeless TLUD kiln and insulated compost drum out to the backyard nutrient pad.";
            return;
        }

        if (aff_id == "aff.build_poultry_coop") {
            ConsumeItemFromParcel("item.cedar_plank", 6u);
            if (!FindEntityByArchetype("arch.poultry_coop").is_alive()) {
                GridCoord p_coop{22, 1, 11};
                if (world_grid_.GetVoxel(22, 1, 11) != MAT_VOXEL_AIR) {
                    p_coop = FindOpenBackyardPlot(10, 13);
                }
                (void)world_grid_.SetVoxel(p_coop.x, 1, p_coop.z, kVoxelMatPoultryCoop);
                if (world_grid_.InBounds(p_coop.x, 1, p_coop.z + 1) &&
                    world_grid_.GetVoxel(p_coop.x, 1, p_coop.z + 1) == MAT_VOXEL_AIR) {
                    (void)world_grid_.SetVoxel(p_coop.x, 1, p_coop.z + 1, kVoxelMatPoultryCoop);
                }
                SpawnSingleEntity(PlacedEntitySeed{
                    "entity.poultry_coop", "arch.poultry_coop", "zone.fenced_backyard", "", p_coop.x, 1, p_coop.z, 0u, kVoxelMatPoultryCoop, false
                });
                SpawnHeritagePastureFlock(p_coop.x, p_coop.z);
                renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
                inner_thought_ = "Hauled the 115 kg elevated poultry coop into the pasture run — three heritage hens are already foraging!";
            }
            return;
        }

        if (aff_id == "aff.plow_and_amend_bed" && target.is_alive() && target.has<SoilCellRegisters>()) {
            auto& soil = target.get_mut<SoilCellRegisters>();
            AmendSoilWithCompost(soil, 8000u);
            AmendSoilWithBiochar(soil, 3000u);
            if (target.has<PropertyBagComponent>()) {
                target.get_mut<PropertyBagComponent>().Set("bed_stage", 1, "stage");
            }
            inner_thought_ = "Worked cured compost and microporous biochar deep into the root zone — ready for the A-frame trellis.";
            return;
        }

        if (aff_id == "aff.erect_pea_trellis" && target.is_alive() && target.has<EntitySpatialMetaComponent>()) {
            const auto& m = target.get<EntitySpatialMetaComponent>();
            (void)world_grid_.SetVoxel(m.x, m.y, m.z, 34u);
            if (target.has<PropertyBagComponent>()) {
                target.get_mut<PropertyBagComponent>().Set("bed_stage", 2, "stage");
            }
            renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
            inner_thought_ = "Anchored the A-frame cedar and galvanized twine trellis over the raised bed — ready to sow inoculated peas.";
            return;
        }

        if (aff_id == "aff.inoculate_and_sow_peas" && target.is_alive() && target.has<SoilCellRegisters>()) {
            ConsumeItemFromParcel("item.pea_seeds_heirloom", 1u);
            const CultivarPhysicsSpec spec = ResolveCultivarByCode(active_cultivar_code_);
            auto& soil              = target.get_mut<SoilCellRegisters>();
            soil.crop_planted       = 1u;
            soil.is_legume_fixing_n = spec.fixes_atmospheric_n2;
            soil.crop_growth_bp     = 200u; // Newly sown seed cohort (2.0% initial radicle emergence)
            soil.growth_sub_accum   = 0u;
            if (target.has<PropertyBagComponent>()) {
                auto& props = target.get_mut<PropertyBagComponent>();
                props.Set("bed_stage", 3, "stage");
                props.Set("cultivar_code", static_cast<int64_t>(active_cultivar_code_), "code");
            }
            last_observed_garden_growth_bp_   = soil.crop_growth_bp;
            last_observed_garden_moisture_bp_ = soil.moisture_vwc_bp;
            RefreshVirtualPinBusEndpoints();
            if (target.has<EntitySpatialMetaComponent>()) {
                const auto& m = target.get<EntitySpatialMetaComponent>();
                (void)world_grid_.SetVoxel(m.x, m.y, m.z, 34u);
                renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
            }
            inner_thought_ = "Sowed " + std::string(CultivarDisplayNameFromCode(active_cultivar_code_)) +
                             " at " + std::to_string(spec.sowing_depth_mm) + " mm depth along the trellis — timed growth & moisture checks begin now.";
            return;
        }

        if (aff_id == "aff.fulfill_l7_woodwork") {
            ConsumeItemFromParcel("item.cedar_plank", 4u);
            steward_drives_.introspection_bp = std::min(kDriveMaxBp, steward_drives_.introspection_bp + 1200u);
            inner_thought_ = "Wheeled the finished custom cedar joinery piece out to the driveway pickup on the hand dolly (+$180 toward zero-bill node offset).";
            return;
        }

        if (aff_id == "aff.water_garden_manual" && target.is_alive() && target.has<SoilCellRegisters>()) {
            uint8_t c_code = active_cultivar_code_;
            if (target.has<PropertyBagComponent>()) {
                c_code = static_cast<uint8_t>(
                    std::clamp<int64_t>(target.get<PropertyBagComponent>().Get("cultivar_code", static_cast<int64_t>(c_code)), 0, 3)
                );
            }
            const CultivarPhysicsSpec spec = ResolveCultivarByCode(c_code);
            auto& soil = target.get_mut<SoilCellRegisters>();
            const uint32_t dose_ml = (soil.moisture_vwc_bp < spec.target_max_vwc_bp) ? spec.irrigation_dose_ml : 1000u;
            StepCultivarSoilPhysics(soil, spec, clock_.AmbientTempMilliKelvin(), dose_ml, 1u);

            last_observed_garden_growth_bp_   = soil.crop_growth_bp;
            last_observed_garden_moisture_bp_ = soil.moisture_vwc_bp;

            const CropPhenologyStage stage = EvaluateCropPhenologyStage(
                soil.crop_growth_bp, soil.crop_planted, spec.harvest_mature_bp
            );
            if (target.has<PropertyBagComponent>()) {
                target.get_mut<PropertyBagComponent>().Set(
                    "bed_stage",
                    (soil.crop_growth_bp >= spec.harvest_mature_bp) ? 4 : 3,
                    "stage"
                );
            }

            const std::string c_name(CultivarDisplayNameFromCode(c_code));
            const std::string s_name(PhenologyStageLabel(stage));
            inner_thought_ = "Verified " + c_name + " (" + std::to_string(soil.crop_growth_bp / 100u) + "% · " +
                             s_name + ") and watered root zone to " + std::to_string(soil.moisture_vwc_bp / 100u) + "% VWC.";

            std::ostringstream ioss;
            ioss << "{"
                 << "\"headline\":\"" << detail::JsonEscape(c_name + " — " + s_name) << "\","
                 << "\"look_description\":\"" << detail::JsonEscape(inner_thought_) << "\","
                 << "\"has_live_l2_telemetry\":false,"
                 << "\"revealed_properties\":["
                 << "{\"key\":\"crop_growth_bp\",\"value\":" << soil.crop_growth_bp << ",\"unit_hint\":\"bp\"},"
                 << "{\"key\":\"moisture_vwc_bp\",\"value\":" << soil.moisture_vwc_bp << ",\"unit_hint\":\"bp\"},"
                 << "{\"key\":\"soil_temp_mk\",\"value\":" << soil.soil_temp_mk << ",\"unit_hint\":\"mK\"},"
                 << "{\"key\":\"nitrogen_ppm\",\"value\":" << soil.nitrogen_ppm << ",\"unit_hint\":\"ppm\"}"
                 << "],\"revealed_inventory\":[]}";
            pending_inspection_report_json_ = ioss.str();
            return;
        }

        if (aff_id == "aff.install_l2_soil_probe" && target.is_alive() && target.has<SensorAttachmentComponent>()) {
            ConsumeItemFromParcel("item.l2_soil_probe", 1u);
            target.get_mut<SensorAttachmentComponent>().has_l2_sensor = true;
            if (target.has<PropertyBagComponent>()) {
                target.get_mut<PropertyBagComponent>().Set("l2_probe_installed", 1, "flag");
            }
            RefreshVirtualPinBusEndpoints();
            inner_thought_ = "Seated the FDR soil probe into the root zone — VirtualPinBus is now streaming live soil_probe_v1 telemetry.";
            return;
        }

        if (aff_id == "aff.install_l2_drip_valve" && target.is_alive() && target.has<SensorAttachmentComponent>()) {
            ConsumeItemFromParcel("item.l2_irrigation_valve", 1u);
            target.get_mut<SensorAttachmentComponent>().has_l2_sensor      = true;
            target.get_mut<EntitySpatialMetaComponent>().has_drip_valve_hardware = true;
            if (target.has<PropertyBagComponent>()) {
                target.get_mut<PropertyBagComponent>().Set("l2_valve_installed", 1, "flag");
            }
            RefreshVirtualPinBusEndpoints();
            inner_thought_ = "Plumbed the 24V latching drip solenoid valve — L2 automated moisture regulation is online.";
            return;
        }

        if (aff_id == "aff.harvest_peas_and_roots" && target.is_alive() && target.has<SoilCellRegisters>()) {
            uint8_t c_code = active_cultivar_code_;
            if (target.has<PropertyBagComponent>()) {
                c_code = static_cast<uint8_t>(
                    std::clamp<int64_t>(target.get<PropertyBagComponent>().Get("cultivar_code", static_cast<int64_t>(c_code)), 0, 3)
                );
            }
            const CultivarPhysicsSpec spec = ResolveCultivarByCode(c_code);
            auto& soil          = target.get_mut<SoilCellRegisters>();
            soil.crop_growth_bp = 1500u;
            if (target.has<PropertyBagComponent>()) {
                target.get_mut<PropertyBagComponent>().Set("bed_stage", 5, "stage");
            }
            steward_inventory_.AddItem("item.fresh_pea_pods", static_cast<uint32_t>(spec.harvest_legume_batches) * 2u);
            steward_inventory_.AddItem("item.mirepoix_roots", static_cast<uint32_t>(spec.harvest_mirepoix_batches));
            inner_thought_ = "Harvested " + std::to_string(spec.harvest_legume_batches) + " batches of mature " +
                             std::string(CultivarDisplayNameFromCode(c_code)) + " pods and sweet mirepoix roots.";
            return;
        }

        if (aff_id == "aff.shell_dry_and_turn_bed") {
            steward_inventory_.AddItem("item.split_peas_dry", 3u);
            if (target.is_alive()) {
                if (target.has<PropertyBagComponent>()) {
                    target.get_mut<PropertyBagComponent>().Set("bed_stage", 0, "stage");
                }
                if (target.has<SoilCellRegisters>()) {
                    auto& soil          = target.get_mut<SoilCellRegisters>();
                    soil.crop_planted   = 0u;
                    soil.crop_growth_bp = 0u;
                }
                if (target.has<EntitySpatialMetaComponent>()) {
                    const auto& m = target.get<EntitySpatialMetaComponent>();
                    (void)world_grid_.SetVoxel(m.x, m.y, m.z, MAT_TILLED_GARDEN_SOIL);
                    renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
                }
            }
            inner_thought_ = "Shelled dry green split peas (< 14% moisture) and turned nitrogen-rich nodules back into the bed.";
            return;
        }

        if (aff_id == "aff.pyrolyze_and_quench_biochar") {
            steward_inventory_.AddItem("item.pyrolysis_biochar", 8u);
            inner_thought_ = "Quenched a clean batch of smokeless TLUD biochar — rings like glass in the bucket.";
            return;
        }

        if (aff_id == "aff.turn_and_harvest_compost") {
            steward_inventory_.AddItem("item.thermophilic_compost", 25u);
            inner_thought_ = "Screened dark, sweet-smelling thermophilic humus into the wheelbarrow.";
            return;
        }

        if (aff_id == "aff.tend_hens_and_collect_eggs") {
            uint32_t gathered = 4u;
            if (target.is_alive() && target.has<InventoryComponent>()) {
                auto& coop_inv      = target.get_mut<InventoryComponent>();
                const uint32_t laid = coop_inv.CountItem("item.pasture_egg");
                if (laid > 0u) {
                    (void)coop_inv.RemoveItem("item.pasture_egg", laid);
                    gathered = laid;
                }
            }
            steward_inventory_.AddItem("item.pasture_egg", gathered);
            steward_drives_.leisure_bp = std::min(kDriveMaxBp, steward_drives_.leisure_bp + 900u);
            inner_thought_             = "Fed BSFL grubs to the hens and gathered warm golden-yolked eggs from the nest box.";
            return;
        }

        if (aff_id == "aff.gather_soup_inputs") {
            SetKitchenHearthStage(1);
            inner_thought_ = "Rinsed split peas, scrubbed mirepoix roots, and staged broth bones at the kitchen prep sink.";
            return;
        }

        if (aff_id == "aff.attach_l2_culinary_controller" && target.is_alive() && target.has<SensorAttachmentComponent>()) {
            ConsumeItemFromParcel("item.l2_culinary_rtd_controller", 1u);
            target.get_mut<SensorAttachmentComponent>().has_l2_sensor = true;
            if (target.has<PropertyBagComponent>()) {
                target.get_mut<PropertyBagComponent>().Set("l2_culinary_installed", 1, "flag");
            }
            RefreshVirtualPinBusEndpoints();
            inner_thought_ = "Clipped the PT100 RTD probe onto the stockpot — culinary_vessel_v1 pasteurization telemetry is live.";
            return;
        }

        if ((aff_id == "aff.simmer_bone_broth" || aff_id == "aff.cook_pea_soup") &&
            target.is_alive() && target.has<ThermalVesselRegisters>()) {
            auto& vessel                 = target.get_mut<ThermalVesselRegisters>();
            vessel.liquid_volume_ml      = 6000u;
            vessel.heater_power_mw       = 1800000u; // 1.8 kW induction simmer
            vessel.lid_closed            = 1u;
            vessel.target_cutoff_temp_mk = 362150u; // 89 C gentle simmer above 74 C Bigelow threshold
            StepThermalVessel(vessel, clock_.AmbientTempMilliKelvin(), 480u);
            if (aff_id == "aff.simmer_bone_broth") {
                steward_inventory_.AddItem("item.bone_broth", 4u);
                SetKitchenHearthStage(2);
                inner_thought_ = "Rich 24-hour collagen bone broth is simmering gently on the induction hearth.";
            } else {
                steward_inventory_.AddItem("item.pea_soup", 6u);
                SetKitchenHearthStage(3);
                steward_drives_.hunger_bp = std::min(kDriveMaxBp, steward_drives_.hunger_bp + 2400u);
                inner_thought_            = "Simmered a fragrant pot of split-pea and mirepoix soup past 74 C pasteurization.";
            }
            return;
        }

        if (aff_id == "aff.pack_and_deliver_tiffins") {
            (void)steward_inventory_.RemoveItem("item.pea_soup", 2u);
            SetKitchenHearthStage(0);
            steward_drives_.social_bp = std::min(kDriveMaxBp, steward_drives_.social_bp + 2200u);
            inner_thought_            = "Latched four hot 304-stainless tiffin tiers of split-pea soup for our R0/R1 neighbors.";
            return;
        }

        if (aff_id == "aff.excavate_earthwork_tile") {
            ExcavationResult ex = world_grid_.ExcavateTile(active_task_.target_x, 0, active_task_.target_z);
            if (!ex.excavated) {
                ex = world_grid_.ExcavateTile(active_task_.target_x, -1, active_task_.target_z);
            }
            if (ex.excavated) {
                steward_inventory_.AddItem(ex.yielded_item_id, ex.quantity_units);
                renderer_->SyncWorldChunkFromGrid(world_grid_, 1u);
                inner_thought_ = "Excavated " + ex.strata_label + " into the wheelbarrow.";
            }
            return;
        }

        if (aff_id == "aff.eat_nourished_meal") {
            steward_drives_.hunger_bp    = std::min(kDriveMaxBp, steward_drives_.hunger_bp + 3200u);
            steward_drives_.hydration_bp = std::min(kDriveMaxBp, steward_drives_.hydration_bp + 1800u);
            inner_thought_               = "Shared a warm bowl at the oak table — feeling steady and restored.";
        } else if (aff_id == "aff.take_warm_shower") {
            steward_drives_.leisure_bp = std::min(kDriveMaxBp, steward_drives_.leisure_bp + 1500u);
            inner_thought_             = "Hot hydronic shower washed off the woodshop sawdust.";
        } else if (aff_id == "aff.decompress_living_room") {
            steward_drives_.leisure_bp = std::min(kDriveMaxBp, steward_drives_.leisure_bp + 2800u);
            inner_thought_             = "Unwound on the linen couch listening to warm vinyl acoustics.";
        } else if (aff_id == "aff.study_at_workstation") {
            steward_drives_.introspection_bp = std::min(kDriveMaxBp, steward_drives_.introspection_bp + 2400u);
            inner_thought_                   = "Reviewed Node 0 sovereign workflows and seasonal agronomy notes.";
        } else if (aff_id == "aff.sleep_in_bed") {
            steward_drives_.energy_bp = kDriveMaxBp;
            inner_thought_            = "Woke rested after a deep sleep under wool blankets.";
        }
    }

    void ConsumeItemFromParcel(std::string_view item_id, uint32_t qty) noexcept {
        if (steward_inventory_.RemoveItem(item_id, qty)) {
            return;
        }
        for (flecs::entity e : map_entities_) {
            if (e.is_alive() && e.has<InventoryComponent>()) {
                if (e.get_mut<InventoryComponent>().RemoveItem(item_id, qty)) {
                    return;
                }
            }
        }
    }

    void SpawnHeritagePastureFlock(int32_t coop_x, int32_t coop_z) {
        if (!hens_.empty()) {
            return;
        }
        for (int32_t i = 0; i < 3; ++i) {
            PastureHenAgent hen{};
            hen.drives.species    = AgentSpecies::CHICKEN;
            hen.drives.hunger_bp  = 7800u;
            hen.drives.hydration_bp = 8200u;
            hen.drives.energy_bp  = 8600u;
            hen.autonomy.rng_state = 0x517CC1B727220A95ULL ^ static_cast<uint64_t>(i * 997 + 17);
            hen.x = std::clamp(coop_x - 1 - (i % 2), 19, 24);
            hen.y = 1;
            hen.z = std::clamp(coop_z - 1 + i, 8, 14);
            hens_.push_back(hen);
        }
    }

    void StepPastureChickens(uint32_t sim_dt_ms) {
        if (hens_.empty()) {
            return;
        }
        flecs::entity coop = FindEntityByArchetype("arch.poultry_coop");
        if (!coop.is_alive() || !coop.has<InventoryComponent>()) {
            return;
        }
        auto& coop_inv = coop.get_mut<InventoryComponent>();
        const auto& coop_meta = coop.get<EntitySpatialMetaComponent>();

        for (auto& hen : hens_) {
            const AutonomousActivity act = UtilityAutonomySystem::StepChickenAutonomy(
                hen.drives,
                hen.autonomy,
                coop_inv,
                sim_dt_ms,
                "item.pasture_egg"
            );
            hen.roam_accum_sim_ms += sim_dt_ms;
            if (hen.roam_accum_sim_ms >= 45u * kSimMsPerSecond) {
                hen.roam_accum_sim_ms = 0u;
                if (act == AutonomousActivity::CHICKEN_ROOSTING || act == AutonomousActivity::CHICKEN_LAYING_EGG) {
                    hen.x = std::clamp(coop_meta.x - 1, 19, 24);
                    hen.z = coop_meta.z;
                } else {
                    const uint64_t r  = UtilityAutonomySystem::NextDeterministicU64(hen.autonomy.rng_state);
                    const int32_t  dx = static_cast<int32_t>(r % 3ULL) - 1;
                    const int32_t  dz = static_cast<int32_t>((r >> 8u) % 3ULL) - 1;
                    const int32_t  nx = std::clamp(hen.x + dx, 19, 24);
                    const int32_t  nz = std::clamp(hen.z + dz, 8, 14);
                    if (world_grid_.GetVoxel(nx, 1, nz) == MAT_VOXEL_AIR) {
                        hen.x = nx;
                        hen.z = nz;
                    }
                }
            }
        }
    }

    void SyncDynamicVoxelsAndAvatarToRenderer() {
        renderer_->ClearDynamicVoxels();
        for (const auto& hen : hens_) {
            renderer_->PushDynamicVoxel(hen.x, hen.y, hen.z, kVoxelMatPastureHen);
        }

        const bool  carrying = !steward_equipped_tool_.tool_id.empty();
        float       act_code = 0.0f;
        if (active_task_.stage == TaskStage::WALK_TO_FETCH_TOOL ||
            active_task_.stage == TaskStage::WALK_TO_STATION ||
            active_task_.stage == TaskStage::WALK_TO_PARK_TOOL) {
            act_code = 1.0f;
        } else if (active_task_.stage == TaskStage::PERFORM_WORK) {
            act_code = 2.0f;
        }

        const float stress_norm = 1.0f - (static_cast<float>(steward_drives_.leisure_bp) / 10000.0f);
        const float focus_norm  = static_cast<float>(steward_drives_.introspection_bp) / 10000.0f;
        const float anim_sec    = static_cast<float>(tick_count_) / 60.0f;
        renderer_->SetAvatarState(steward_pos_, anim_sec, carrying, act_code, stress_norm, focus_norm);
    }

    [[nodiscard]] oasis::chill::BiologicalFeelTiers ComputeCurrentStewardFeelTiers() const noexcept {
        oasis::chill::SomaticVitalsComponent vitals{};
        vitals.caloric_stamina_kcal  = 2500.0f * (static_cast<float>(steward_drives_.hunger_bp) / 10000.0f);
        vitals.fatigue_factor        = 1.0f - (static_cast<float>(steward_drives_.energy_bp) / 10000.0f);
        vitals.hydration_pct         = static_cast<float>(steward_drives_.hydration_bp) / 10000.0f;
        vitals.focus_pool            = static_cast<float>(steward_drives_.introspection_bp) / 10000.0f;
        vitals.stress_accumulator    = 1.0f - (static_cast<float>(steward_drives_.leisure_bp) / 10000.0f);
        vitals.entertainment_comfort = static_cast<float>(steward_drives_.leisure_bp) / 10000.0f;
        vitals.social_comfort        = static_cast<float>(steward_drives_.social_bp) / 10000.0f;
        vitals.hygiene               = 0.90f;

        bool has_any_soil_sensor = false;
        for (flecs::entity e : map_entities_) {
            if (e.is_alive() && e.has<SoilCellRegisters>() && e.has<SensorAttachmentComponent>()) {
                if (e.get<SensorAttachmentComponent>().has_l2_sensor) {
                    has_any_soil_sensor = true;
                    break;
                }
            }
        }
        return oasis::chill::ComputeBiologicalFeelTiers(vitals, vitals.social_comfort, false, has_any_soil_sensor);
    }

    void PushUhaiTelemetrySample() {
        if (!telemetry_channel_ || !telemetry_channel_->IsValid()) {
            return;
        }
        oasis::uhai::TelemetrySample sample{};
        sample.frame_count = renderer_->FrameCount();
        sample.timestamp_ns = static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now().time_since_epoch()
            ).count()
        );
        sample.fps               = 60.0f;
        sample.founder_stress    = 1.0f - (static_cast<float>(steward_drives_.leisure_bp) / 10000.0f);
        sample.memory_slot_count = steward_autonomy_.transition_count;
        sample.biome_id          = 0u;
        sample.quality_flags     = oasis::uhai::QUALITY_SIMULATED;
        (void)telemetry_channel_->Push(sample);
    }

    void ProcessSdlInputEvents() {
        SDL_Event ev{};
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running_ = false;
                return;
            }
            if (ev.type == SDL_KEYDOWN) {
                const SDL_Keycode sym = ev.key.keysym.sym;
                if (sym == SDLK_TAB) {
                    renderer_->ToggleCameraProjectionMode();
                } else if (sym == SDLK_LSHIFT || sym == SDLK_RSHIFT) {
                    shift_down_ = true;
                } else if (sym == SDLK_PAGEUP) {
                    renderer_->AdjustRoofCutaway(+1);
                } else if (sym == SDLK_PAGEDOWN) {
                    renderer_->AdjustRoofCutaway(-1);
                } else if (sym == SDLK_PLUS || sym == SDLK_EQUALS || sym == SDLK_KP_PLUS) {
                    renderer_->Zoom(2.0f);
                } else if (sym == SDLK_MINUS || sym == SDLK_KP_MINUS) {
                    renderer_->Zoom(-2.0f);
                } else if (sym == SDLK_w || sym == SDLK_UP) {
                    renderer_->PanByKey(0.0f, 1.0f);
                } else if (sym == SDLK_s || sym == SDLK_DOWN) {
                    renderer_->PanByKey(0.0f, -1.0f);
                } else if (sym == SDLK_d || sym == SDLK_RIGHT) {
                    renderer_->PanByKey(1.0f, 0.0f);
                } else if (sym == SDLK_a || sym == SDLK_LEFT) {
                    renderer_->PanByKey(-1.0f, 0.0f);
                } else if (sym == SDLK_0) {
                    clock_.SetWarpMultiplier(0u);
                } else if (sym == SDLK_1) {
                    clock_.SetWarpMultiplier(1u);
                } else if (sym == SDLK_2) {
                    clock_.SetWarpMultiplier(2u);
                } else if (sym == SDLK_3) {
                    clock_.SetWarpMultiplier(5u);
                } else if (sym == SDLK_4) {
                    clock_.SetWarpMultiplier(10u);
                } else if (sym == SDLK_F1) {
                    renderer_->SetOverlayMode(renderer_->OverlayMode() == 1u ? 0u : 1u);
                } else if (sym == SDLK_F2) {
                    renderer_->SetOverlayMode(renderer_->OverlayMode() == 2u ? 0u : 2u);
                }
            } else if (ev.type == SDL_KEYUP) {
                if (ev.key.keysym.sym == SDLK_LSHIFT || ev.key.keysym.sym == SDLK_RSHIFT) {
                    shift_down_ = false;
                }
            } else if (ev.type == SDL_MOUSEWHEEL) {
                float wx = (ev.wheel.preciseX != 0.0f) ? ev.wheel.preciseX : static_cast<float>(ev.wheel.x);
                float wy = (ev.wheel.preciseY != 0.0f) ? ev.wheel.preciseY : static_cast<float>(ev.wheel.y);
                if (ev.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                    wx = -wx;
                    wy = -wy;
                }
                const SDL_Keymod mod = SDL_GetModState();
                const bool zoom_mod  = ((mod & (KMOD_SHIFT | KMOD_ALT | KMOD_CTRL | KMOD_GUI)) != 0) || shift_down_;
                if (zoom_mod) {
                    renderer_->Zoom(wy);
                } else {
                    renderer_->PanMouseOrWheel(wx, -wy, true);
                }
            } else if (ev.type == SDL_MOUSEMOTION) {
                if (orbiting_camera_) {
                    const float dx = static_cast<float>(ev.motion.x - last_mouse_x_);
                    const float dy = static_cast<float>(ev.motion.y - last_mouse_y_);
                    last_mouse_x_  = ev.motion.x;
                    last_mouse_y_  = ev.motion.y;
                    renderer_->Orbit(dx * 0.008f, dy * 0.008f);
                } else if (panning_camera_) {
                    const float dx = static_cast<float>(ev.motion.x - last_mouse_x_);
                    const float dy = static_cast<float>(ev.motion.y - last_mouse_y_);
                    last_mouse_x_  = ev.motion.x;
                    last_mouse_y_  = ev.motion.y;
                    renderer_->PanMouseOrWheel(dx, dy, false);
                    renderer_->SetCursorHighlight(0, 0, 0, false);
                } else {
                    const auto hit = renderer_->RaycastWorldGrid(ev.motion.x, ev.motion.y, world_grid_);
                    renderer_->SetCursorHighlight(hit.vx, hit.vy, hit.vz, hit.hit);
                }
            } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                const SDL_Keymod mod      = SDL_GetModState();
                const bool       is_shift = ((mod & KMOD_SHIFT) != 0) || shift_down_;
                last_mouse_x_             = ev.button.x;
                last_mouse_y_             = ev.button.y;

                if (ev.button.button == SDL_BUTTON_RIGHT && is_shift) {
                    orbiting_camera_ = true;
                    const auto hit   = renderer_->RaycastWorldGrid(ev.button.x, ev.button.y, world_grid_);
                    if (hit.hit) {
                        renderer_->LockPoi(hit.vx, hit.vy, hit.vz);
                    }
                } else if (ev.button.button == SDL_BUTTON_MIDDLE ||
                           (ev.button.button == SDL_BUTTON_LEFT && is_shift) ||
                           ev.button.button == SDL_BUTTON_LEFT) {
                    panning_camera_ = true;
                } else if (ev.button.button == SDL_BUTTON_RIGHT) {
                    const auto hit = renderer_->RaycastWorldGrid(ev.button.x, ev.button.y, world_grid_);
                    if (hit.hit) {
                        HandleRightClickVoxelContextMenu(ev.button.x, ev.button.y, hit);
                    }
                }
            } else if (ev.type == SDL_MOUSEBUTTONUP) {
                if (ev.button.button == SDL_BUTTON_LEFT || ev.button.button == SDL_BUTTON_MIDDLE) {
                    panning_camera_ = false;
                } else if (ev.button.button == SDL_BUTTON_RIGHT) {
                    orbiting_camera_ = false;
                }
            }
        }
    }

    // Queries AffordanceSystem + subsurface strata (y = -1, -2) + HaulingPhysics notes on right-click hit!
    void HandleRightClickVoxelContextMenu(
        int32_t                        screen_x,
        int32_t                        screen_y,
        const render::WorldRaycastHit& hit
    ) {
        flecs::entity ent = FindEntityAtVoxel(hit.vx, hit.vy, hit.vz);
        if (!ent.is_alive() && hit.vy == 0) {
            ent = FindEntityAtVoxel(hit.vx, 1, hit.vz);
        }

        const EntityArchetypeDef* arch        = nullptr;
        std::string               entity_id   = "tile." + std::to_string(hit.vx) + "_" + std::to_string(hit.vy) + "_" + std::to_string(hit.vz);
        std::string               room_tag    = "parcel";
        const PropertyBagComponent*      props  = nullptr;
        const SoilCellRegisters*         soil   = nullptr;
        const ThermalVesselRegisters*    vessel = nullptr;
        const InventoryComponent*        inv    = nullptr;
        const SensorAttachmentComponent* sensor = nullptr;

        if (ent.is_alive() && ent.has<EntitySpatialMetaComponent>()) {
            const auto& m = ent.get<EntitySpatialMetaComponent>();
            entity_id     = m.entity_id;
            room_tag      = m.room_tag;
            arch          = registry_.FindArchetype(m.archetype_id);
            if (ent.has<PropertyBagComponent>()) props = &ent.get<PropertyBagComponent>();
            if (ent.has<SoilCellRegisters>()) soil = &ent.get<SoilCellRegisters>();
            if (ent.has<ThermalVesselRegisters>()) vessel = &ent.get<ThermalVesselRegisters>();
            if (ent.has<InventoryComponent>()) inv = &ent.get<InventoryComponent>();
            if (ent.has<SensorAttachmentComponent>()) sensor = &ent.get<SensorAttachmentComponent>();
        } else {
            const bool inside_house  = (hit.vx >= 9 && hit.vx <= 17 && hit.vz >= 2 && hit.vz <= 10);
            const bool inside_garage = (hit.vx >= 8 && hit.vx <= 12 && hit.vz >= 10 && hit.vz <= 14);
            if (!inside_house && !inside_garage &&
                (hit.material_id == MAT_SURFACE_LAWN_OR_FLOOR || hit.material_id == STRATA_TOPSOIL_LOAM)) {
                arch = registry_.FindArchetype("arch.yard_earthwork_plot");
            }
        }

        EntityArchetypeDef fallback_arch{};
        if (arch == nullptr) {
            fallback_arch.archetype_id      = "arch.structural_voxel";
            fallback_arch.display_name      = detail::StrataMaterialName(hit.material_id);
            fallback_arch.look_description  = "Parcel structure voxel at (" + std::to_string(hit.vx) + ", " +
                                              std::to_string(hit.vy) + ", " + std::to_string(hit.vz) + ").";
            fallback_arch.voxel_material_id = hit.material_id;
            arch                            = &fallback_arch;
        }

        const bool path_paved = world_grid_.IsPathPavedOrGraded(hit.vx, hit.vz);

        // 1. Query AffordanceSystem with current tool, then merge affordances enabled by parked garage hauling tools
        std::vector<AffordanceDef> filtered = QueryEntityAffordances(
            registry_, *arch, props, &steward_caps_, &steward_equipped_tool_, path_paved, soil, vessel, inv
        );

        for (flecs::entity te : map_entities_) {
            if (!te.is_alive() || !te.has<EntitySpatialMetaComponent>()) {
                continue;
            }
            const auto& tm = te.get<EntitySpatialMetaComponent>();
            if (tm.hauling_tool_id.empty() || !tm.is_parked_in_world) {
                continue;
            }
            EquippedHaulingToolComponent cand_tool{tm.hauling_tool_id, tm.entity_id};
            for (const bool terrain_opt : {path_paved, true, false}) {
                const auto unlocked = QueryEntityAffordances(
                    registry_, *arch, props, &steward_caps_, &cand_tool, terrain_opt, soil, vessel, inv
                );
                for (const auto& u : unlocked) {
                    const bool already = std::any_of(filtered.begin(), filtered.end(), [&](const AffordanceDef& ex) {
                        return ex.affordance_id == u.affordance_id;
                    });
                    if (!already) {
                        filtered.push_back(u);
                    }
                }
            }
        }

        // 1b. When synced with col-gatewayd (:8090), gate workflow-governed production affordances on ACTIVE_TOKEN!
        uint32_t bpmn_blocked_count = 0u;
        if (bpmn_token_sync_active_) {
            std::vector<AffordanceDef> token_gated;
            token_gated.reserve(filtered.size());
            for (auto& aff : filtered) {
                if (detail::IsWorkflowGovernedAffordance(aff.affordance_id) &&
                    FindActiveBpmnTaskForAffordance(aff.affordance_id).empty()) {
                    ++bpmn_blocked_count;
                    continue;
                }
                token_gated.push_back(std::move(aff));
            }
            filtered = std::move(token_gated);
        }

        // 2. Inspect subsurface strata at y = -1 and y = -2 beneath clicked (vx, vz)
        const uint16_t    strata_y1      = world_grid_.GetVoxel(hit.vx, -1, hit.vz);
        const uint16_t    strata_y2      = world_grid_.GetVoxel(hit.vx, -2, hit.vz);
        const std::string strata_summary = "y=-1: " + std::string(detail::StrataMaterialName(strata_y1)) +
                                           " / y=-2: " + std::string(detail::StrataMaterialName(strata_y2));

        std::ostringstream oss;
        oss << "{"
            << "\"screen_x\":" << screen_x << ","
            << "\"screen_y\":" << screen_y << ","
            << "\"x\":" << hit.vx << ","
            << "\"y\":" << hit.vy << ","
            << "\"z\":" << hit.vz << ","
            << "\"coords\":{\"x\":" << hit.vx << ",\"y\":" << hit.vy << ",\"z\":" << hit.vz << "},"
            << "\"material_id\":" << hit.material_id << ","
            << "\"entity_id\":\"" << detail::JsonEscape(entity_id) << "\","
            << "\"archetype_id\":\"" << detail::JsonEscape(arch->archetype_id) << "\","
            << "\"display_name\":\"" << detail::JsonEscape(arch->display_name) << "\","
            << "\"entity_name\":\"" << detail::JsonEscape(arch->display_name) << "\","
            << "\"name\":\"" << detail::JsonEscape(arch->display_name) << "\","
            << "\"room_tag\":\"" << detail::JsonEscape(room_tag) << "\","
            << "\"has_l2_sensor\":" << ((sensor != nullptr && sensor->has_l2_sensor) ? "true" : "false") << ","
            << "\"bpmn_token_sync_active\":" << (bpmn_token_sync_active_ ? "true" : "false") << ","
            << "\"bpmn_blocked_count\":" << bpmn_blocked_count << ","
            << "\"look_description\":\"" << detail::JsonEscape(arch->look_description) << "\","
            << "\"look_text\":\"" << detail::JsonEscape(arch->look_description) << "\","
            << "\"strata_summary\":\"" << detail::JsonEscape(strata_summary) << "\","
            << "\"inspect_available\":true,"
            << "\"subsurface_strata\":["
            << "{\"y\":-1,\"material_id\":" << strata_y1
            << ",\"name\":\"" << detail::JsonEscape(detail::StrataMaterialName(strata_y1)) << "\""
            << ",\"excavatable\":" << (detail::IsStrataExcavatable(strata_y1) ? "true" : "false")
            << ",\"hauling_note\":\"Uses Wheelbarrow (110-140 kg Granular)\"},"
            << "{\"y\":-2,\"material_id\":" << strata_y2
            << ",\"name\":\"" << detail::JsonEscape(detail::StrataMaterialName(strata_y2)) << "\""
            << ",\"excavatable\":" << (detail::IsStrataExcavatable(strata_y2) ? "true" : "false")
            << ",\"hauling_note\":\"Uses Wheelbarrow (120 kg Clay Subsoil)\"}"
            << "],"
            << "\"affordances\":[";

        for (size_t i = 0u; i < filtered.size(); ++i) {
            const auto& aff = filtered[i];
            int32_t     hx  = hit.vx;
            int32_t     hz  = hit.vz;
            if (aff.affordance_id == "aff.fulfill_l7_woodwork") {
                hx = 7;
                hz = 12;
            } else if (aff.affordance_id == "aff.assemble_compost_and_kiln" || aff.affordance_id == "aff.build_poultry_coop") {
                hx = 21;
                hz = 10;
            }
            const auto        haul_note  = EvaluateAffordanceHaulRequirement(aff, hx, hz);
            const std::string l4_task_id = FindActiveBpmnTaskForAffordance(aff.affordance_id);
            if (i > 0u) oss << ",";
            oss << "{"
                << "\"id\":\"" << detail::JsonEscape(aff.affordance_id) << "\","
                << "\"affordance_id\":\"" << detail::JsonEscape(aff.affordance_id) << "\","
                << "\"l4_task_id\":\"" << detail::JsonEscape(l4_task_id) << "\","
                << "\"label\":\"" << detail::JsonEscape(aff.label) << "\","
                << "\"duration_sim_s\":" << aff.duration_sim_s << ","
                << "\"required_capability_tag\":\"" << detail::JsonEscape(aff.required_capability_tag) << "\","
                << "\"hauling_badge\":\"" << detail::JsonEscape(haul_note.ui_note) << "\","
                << "\"hauling_note\":\"" << detail::JsonEscape(haul_note.ui_note) << "\","
                << "\"hauling_tool_id\":\"" << detail::JsonEscape(haul_note.tool_id) << "\","
                << "\"requires_tool\":\"" << detail::JsonEscape(haul_note.tool_id) << "\""
                << "}";
        }
        oss << "]}";

        const std::string payload = oss.str();
#ifdef __EMSCRIPTEN__
        EM_ASM({
            var jsonStr = UTF8ToString($0);
            if (typeof window !== 'undefined') {
                try { window._oasisLastContextMenu = JSON.parse(jsonStr); } catch (e) {}
                if (window.OasisUI && typeof window.OasisUI.onContextMenuData === 'function') {
                    window.OasisUI.onContextMenuData(jsonStr);
                }
            }
        }, payload.c_str());
#else
        (void)payload;
#endif
    }

#ifdef __EMSCRIPTEN__
    void PollBrowserCommandQueue() {
        for (int guard = 0; guard < 16; ++guard) {
            char* raw = reinterpret_cast<char*>(EM_ASM_PTR({
                if (typeof window === 'undefined' || !Array.isArray(window._oasisEngineCommandQueue) ||
                    window._oasisEngineCommandQueue.length === 0) {
                    return 0;
                }
                var item = window._oasisEngineCommandQueue.shift();
                var s = (typeof item === 'string') ? item : JSON.stringify(item);
                var len = lengthBytesUTF8(s) + 1;
                var buf = _malloc(len);
                stringToUTF8(s, buf, len);
                return buf;
            }));
            if (raw == nullptr) {
                break;
            }
            const std::string cmd_json(raw);
            std::free(raw);
            DispatchBrowserCommandJson(cmd_json);
        }
    }

    void DispatchBrowserCommandJson(std::string_view cmd_json) {
        std::string cmd_type;
        if (!json_util::ReadString(cmd_json, "type", cmd_type)) {
            if (!json_util::ReadString(cmd_json, "cmd", cmd_type)) {
                (void)json_util::ReadString(cmd_json, "action", cmd_type);
            }
        }

        if (cmd_type == "execute_affordance") {
            std::string aff_id;
            if (!json_util::ReadString(cmd_json, "affordance_id", aff_id)) {
                (void)json_util::ReadString(cmd_json, "id", aff_id);
            }
            std::string ent_id;
            (void)json_util::ReadString(cmd_json, "entity_id", ent_id);
            int64_t vx = 12;
            int64_t vy = 1;
            int64_t vz = 6;
            (void)json_util::ReadInt64(cmd_json, "x", vx);
            (void)json_util::ReadInt64(cmd_json, "y", vy);
            (void)json_util::ReadInt64(cmd_json, "z", vz);
            if (!aff_id.empty()) {
                (void)StartAffordanceExecution(
                    aff_id, ent_id, static_cast<int32_t>(vx), static_cast<int32_t>(vy), static_cast<int32_t>(vz)
                );
            }
        } else if (cmd_type == "execute_l4_task") {
            std::string task_id;
            (void)json_util::ReadString(cmd_json, "task_id", task_id);
            std::string step_uri;
            if (!json_util::ReadString(cmd_json, "step_action_uri", step_uri)) {
                (void)json_util::ReadString(cmd_json, "uri", step_uri);
            }
            ExecuteL4TaskCommand(task_id, step_uri);
        } else if (cmd_type == "set_time_warp") {
            int64_t mult = 1;
            if (!json_util::ReadInt64(cmd_json, "multiplier", mult)) {
                if (!json_util::ReadInt64(cmd_json, "warp", mult)) {
                    (void)json_util::ReadInt64(cmd_json, "speed", mult);
                }
            }
            clock_.SetWarpMultiplier(static_cast<uint32_t>(std::max<int64_t>(0, mult)));
        } else if (cmd_type == "toggle_roof_cutaway" || cmd_type == "toggle_roof") {
            int64_t explicit_z   = -1;
            bool    roof_cutaway = true;
            if (json_util::ReadInt64(cmd_json, "z_cutaway_level", explicit_z) && explicit_z >= 1) {
                renderer_->SetRoofCutawayLevel(static_cast<int32_t>(explicit_z));
            } else if (json_util::ReadBool(cmd_json, "roof_cutaway", roof_cutaway)) {
                renderer_->SetRoofCutawayLevel(roof_cutaway ? 3 : 16);
            } else {
                renderer_->ToggleRoofCutaway();
            }
        } else if (cmd_type == "apply_l2_actuators" || cmd_type == "l2_actuators" || cmd_type == "set_l2_actuators") {
            RefreshVirtualPinBusEndpoints();
            bool valve_open = false;
            if (json_util::ReadBool(cmd_json, "valve_open", valve_open) ||
                json_util::ReadBool(cmd_json, "irrigation_valve_open", valve_open)) {
                for (flecs::entity e : map_entities_) {
                    if (e.is_alive() && e.has<SoilCellRegisters>() && e.has<EntitySpatialMetaComponent>()) {
                        (void)pin_bus_.WriteEntityIrrigationValve(
                            e.get<EntitySpatialMetaComponent>().entity_uid, valve_open
                        );
                    }
                }
            }
            int64_t heater_mw = -1;
            if (json_util::ReadInt64(cmd_json, "heater_power_mw", heater_mw) && heater_mw >= 0) {
                for (flecs::entity e : map_entities_) {
                    if (e.is_alive() && e.has<ThermalVesselRegisters>() && e.has<EntitySpatialMetaComponent>()) {
                        (void)pin_bus_.WriteEntityHeaterPower(
                            e.get<EntitySpatialMetaComponent>().entity_uid, static_cast<uint32_t>(heater_mw)
                        );
                    }
                }
            }
        } else if (cmd_type == "switch_map_generator" || cmd_type == "set_map_mode") {
            std::string mode;
            if (json_util::ReadString(cmd_json, "mode", mode) ||
                json_util::ReadString(cmd_json, "generator", mode)) {
                const bool want_proc = (mode == "procedural" || mode == "earthworks");
                RebuildWorldFromMapProvider(want_proc);
            } else {
                RebuildWorldFromMapProvider(!procedural_mode_active_);
            }
        } else if (cmd_type == "set_cultivar") {
            std::string cultivar_id;
            if (!json_util::ReadString(cmd_json, "cultivar_id", cultivar_id)) {
                (void)json_util::ReadString(cmd_json, "id", cultivar_id);
            }
            if (!cultivar_id.empty()) {
                ApplyActiveCultivarSelection(cultivar_id);
            }
        } else if (cmd_type == "sync_bpmn_tokens") {
            std::string synced_cultivar_id;
            if (json_util::ReadString(cmd_json, "cultivar_id", synced_cultivar_id) ||
                json_util::ReadString(cmd_json, "selected_cultivar_id", synced_cultivar_id)) {
                if (!synced_cultivar_id.empty()) {
                    ApplyActiveCultivarSelection(synced_cultivar_id);
                }
            }
            bool sync_active = false;
            (void)json_util::ReadBool(cmd_json, "active", sync_active);
            bpmn_token_sync_active_ = sync_active;
            active_bpmn_affordance_to_task_.clear();
            if (sync_active) {
                for (const std::string_view tok_obj : json_util::SplitObjectArray(cmd_json, "tokens")) {
                    std::string aff_id;
                    std::string task_id;
                    if (!json_util::ReadString(tok_obj, "mapped_affordance_id", aff_id)) {
                        (void)json_util::ReadString(tok_obj, "affordance_id", aff_id);
                    }
                    (void)json_util::ReadString(tok_obj, "task_id", task_id);
                    if (!aff_id.empty() && !task_id.empty()) {
                        active_bpmn_affordance_to_task_.emplace_back(std::move(aff_id), std::move(task_id));
                    }
                }
            }
        }
    }
#endif

    void ApplyActiveCultivarSelection(std::string_view cultivar_id) {
        const uint8_t next_code = ResolveCultivarCodeFromId(cultivar_id);
        active_cultivar_code_   = next_code;
        for (flecs::entity e : map_entities_) {
            if (!e.is_alive() || !e.has<EntitySpatialMetaComponent>() || !e.has<PropertyBagComponent>()) {
                continue;
            }
            if (e.get<EntitySpatialMetaComponent>().archetype_id != "arch.raised_garden_bed") {
                continue;
            }
            const bool already_sown = e.has<SoilCellRegisters>() && (e.get<SoilCellRegisters>().crop_planted != 0u);
            if (!already_sown) {
                e.get_mut<PropertyBagComponent>().Set("cultivar_code", static_cast<int64_t>(next_code), "code");
            }
        }
        RefreshVirtualPinBusEndpoints();
    }

#ifdef __EMSCRIPTEN__
    void PushBrowserFrameSnapshot() {
        RefreshVirtualPinBusEndpoints();
        const auto              tiers    = ComputeCurrentStewardFeelTiers();
        const CalendarTimestamp cal      = clock_.CurrentCalendar();
        const std::string       cal_text = clock_.FormatCalendar(false);

        uint32_t    progress_pct    = 0u;
        uint32_t    remaining_sim_s = 0u;
        std::string act_label       = "Calm Leisure / Observation";
        std::string stage_str       = "AUTONOMOUS";
        const bool  task_busy       = (active_task_.stage != TaskStage::IDLE);

        if (task_busy) {
            act_label = active_task_.label;
            if (active_task_.stage == TaskStage::WALK_TO_FETCH_TOOL) {
                stage_str       = "FETCHING_TOOL";
                progress_pct    = 10u;
                remaining_sim_s = active_task_.work_timer.RemainingSimMs() / kSimMsPerSecond;
            } else if (active_task_.stage == TaskStage::WALK_TO_STATION) {
                stage_str       = "WALKING_TO_STATION";
                progress_pct    = 20u;
                remaining_sim_s = active_task_.work_timer.RemainingSimMs() / kSimMsPerSecond;
            } else if (active_task_.stage == TaskStage::PERFORM_WORK) {
                stage_str       = "WORKING";
                progress_pct    = 20u + ((active_task_.work_timer.ProgressBasisPoints() * 75u) / 10000u);
                remaining_sim_s = active_task_.work_timer.RemainingSimMs() / kSimMsPerSecond;
            } else {
                stage_str    = "PARKING_TOOL";
                progress_pct = 97u;
            }
        } else {
            progress_pct    = steward_autonomy_.dwell_timer.ProgressBasisPoints() / 100u;
            remaining_sim_s = steward_autonomy_.dwell_timer.RemainingSimMs() / kSimMsPerSecond;
            switch (steward_autonomy_.current_activity) {
                case AutonomousActivity::HUMAN_READING:       act_label = "Reading Reference Manuals"; break;
                case AutonomousActivity::HUMAN_GAMING_MEDIA:  act_label = "Unwinding with Music & Media"; break;
                case AutonomousActivity::HUMAN_SOCIAL_CHAT:   act_label = "Checking Sovereign Mesh Notes"; break;
                case AutonomousActivity::HUMAN_INTROSPECTION: act_label = "Quiet Homestead Reflection"; break;
                case AutonomousActivity::HUMAN_MEAL:          act_label = "Eating Warm Homestead Meal"; break;
                case AutonomousActivity::HUMAN_HYDRATE:       act_label = "Drinking Cool Water"; break;
                case AutonomousActivity::HUMAN_SLEEP_REST:    act_label = "Resting in Bedroom"; break;
                default:                                      break;
            }
        }

        // Collect L2 virtual pin readings strictly through VirtualPinBus (epistemic privacy: only sensor-equipped entities emit!)
        bool                 has_soil_pin    = false;
        bool                 has_valve_pin   = false;
        bool                 has_thermal_pin = false;
        SoilPinReadResult    soil_pin{};
        ThermalPinReadResult thermal_pin{};

        for (flecs::entity e : map_entities_) {
            if (!e.is_alive() || !e.has<EntitySpatialMetaComponent>()) {
                continue;
            }
            const auto& m = e.get<EntitySpatialMetaComponent>();
            if (!has_soil_pin && e.has<SoilCellRegisters>()) {
                const auto sr = pin_bus_.ReadSoilEntity(m.entity_uid);
                if (sr.status == PinAccessStatus::OK) {
                    has_soil_pin  = true;
                    soil_pin      = sr;
                    has_valve_pin = m.has_drip_valve_hardware;
                }
            }
            if (!has_thermal_pin && e.has<ThermalVesselRegisters>()) {
                const auto tr = pin_bus_.ReadThermalEntity(m.entity_uid);
                if (tr.status == PinAccessStatus::OK) {
                    has_thermal_pin = true;
                    thermal_pin     = tr;
                }
            }
        }

        uint32_t      coop_eggs = 0u;
        flecs::entity coop      = FindEntityByArchetype("arch.poultry_coop");
        if (coop.is_alive() && coop.has<InventoryComponent>()) {
            coop_eggs = coop.get<InventoryComponent>().CountItem("item.pasture_egg");
        }

        const HaulingToolDef* equipped_def = steward_equipped_tool_.ResolveTool(registry_);
        const std::string_view equipped_label =
            (equipped_def != nullptr) ? std::string_view(equipped_def->display_name) : std::string_view{};

        std::ostringstream oss;
        oss << "{"
            << "\"tick\":" << tick_count_ << ","
            << "\"sim_clock_seconds\":" << clock_.TotalSimSeconds() << ","
            << "\"clock_label\":\"" << detail::JsonEscape(cal_text) << "\","
            << "\"sim_clock\":\"" << detail::JsonEscape(cal_text) << "\","
            << "\"calendar\":\"" << detail::JsonEscape(cal_text) << "\","
            << "\"day\":" << cal.day << ","
            << "\"hour\":" << static_cast<unsigned>(cal.hour) << ","
            << "\"minute\":" << static_cast<unsigned>(cal.minute) << ","
            << "\"warp_multiplier\":" << static_cast<unsigned>(clock_.WarpMultiplier()) << ","
            << "\"time_warp\":" << static_cast<unsigned>(clock_.WarpMultiplier()) << ","
            << "\"cutaway_z\":" << renderer_->CutawayLevel() << ","
            << "\"map_generator\":\"" << (procedural_mode_active_ ? "procedural" : "static") << "\","
            << "\"active_cultivar_id\":\"" << detail::JsonEscape(CultivarIdFromCode(active_cultivar_code_)) << "\","
            << "\"active_cultivar_name\":\"" << detail::JsonEscape(CultivarDisplayNameFromCode(active_cultivar_code_)) << "\","
            << "\"observed_garden_growth_bp\":" << last_observed_garden_growth_bp_ << ","
            << "\"observed_garden_moisture_bp\":" << last_observed_garden_moisture_bp_ << ","
            << "\"feel\":{"
            << "\"hunger\":" << static_cast<unsigned>(tiers.hunger_tier) << ","
            << "\"energy\":" << static_cast<unsigned>(tiers.energy_tier) << ","
            << "\"focus\":" << static_cast<unsigned>(tiers.focus_tier) << ","
            << "\"mood\":" << static_cast<unsigned>(tiers.mood_tier) << ","
            << "\"social\":" << static_cast<unsigned>(tiers.social_tier)
            << "},"
            << "\"feel_tiers\":{"
            << "\"hunger\":" << static_cast<unsigned>(tiers.hunger_tier) << ","
            << "\"energy\":" << static_cast<unsigned>(tiers.energy_tier) << ","
            << "\"focus\":" << static_cast<unsigned>(tiers.focus_tier) << ","
            << "\"mood\":" << static_cast<unsigned>(tiers.mood_tier) << ","
            << "\"social\":" << static_cast<unsigned>(tiers.social_tier)
            << "},"
            << "\"thought\":\"" << detail::JsonEscape(inner_thought_) << "\","
            << "\"inner_thought\":\"" << detail::JsonEscape(inner_thought_) << "\","
            << "\"activity_label\":\"" << detail::JsonEscape(act_label) << "\","
            << "\"activity_progress\":" << progress_pct << ","
            << "\"remaining_sim_s\":" << remaining_sim_s << ","
            << "\"equipped_tool_id\":\"" << detail::JsonEscape(steward_equipped_tool_.tool_id) << "\","
            << "\"equipped_tool_label\":\"" << detail::JsonEscape(equipped_label) << "\","
            << "\"active_l4_task_id\":\"" << detail::JsonEscape(active_task_.l4_task_id) << "\","
            << "\"completed_l4_task_id\":\"" << detail::JsonEscape(last_completed_l4_task_id_) << "\","
            << "\"activity\":{"
            << "\"active\":" << (task_busy ? "true" : "false") << ","
            << "\"affordance_id\":\"" << detail::JsonEscape(active_task_.affordance_id) << "\","
            << "\"label\":\"" << detail::JsonEscape(act_label) << "\","
            << "\"stage\":\"" << stage_str << "\","
            << "\"progress_pct\":" << progress_pct << ","
            << "\"equipped_tool\":\"" << detail::JsonEscape(steward_equipped_tool_.tool_id) << "\""
            << "},"
            << "\"flock\":{"
            << "\"coop_built\":" << (coop.is_alive() ? "true" : "false") << ","
            << "\"hen_count\":" << hens_.size() << ","
            << "\"eggs_in_nest\":" << coop_eggs
            << "}";

        if (!pending_inspection_report_json_.empty()) {
            oss << ",\"inspection_report\":" << pending_inspection_report_json_;
            pending_inspection_report_json_.clear();
        }
        last_completed_l4_task_id_.clear();

        oss << ",\"l2_pins\":{"
            << "\"sim_clock_seconds\":" << clock_.TotalSimSeconds() << ","
            << "\"has_active_pins\":" << ((has_soil_pin || has_thermal_pin) ? "true" : "false");

        if (has_soil_pin) {
            oss << ",\"soil_probe_v1\":{"
                << "\"is_online\":true,"
                << "\"moisture_bp\":" << soil_pin.registers.moisture_vwc_bp << ","
                << "\"temperature_mk\":" << soil_pin.registers.soil_temp_mk << ","
                << "\"nitrogen_ppm\":" << soil_pin.registers.nitrogen_ppm << ","
                << "\"phosphorus_ppm\":" << soil_pin.registers.phosphorus_ppm << ","
                << "\"potassium_ppm\":" << soil_pin.registers.potassium_ppm << ","
                << "\"organic_carbon_bp\":" << soil_pin.registers.organic_carbon_bp << ","
                << "\"ph_centi\":" << soil_pin.registers.ph_centi << ","
                << "\"crop_growth_bp\":" << soil_pin.registers.crop_growth_bp
                << "}";
        }
        if (has_valve_pin) {
            oss << ",\"irrigation_valve_v1\":{"
                << "\"is_online\":true,"
                << "\"actuator_level_bp\":" << (soil_pin.irrigation_valve_open ? 10000u : 0u)
                << "}";
        }
        if (has_thermal_pin) {
            oss << ",\"culinary_vessel_v1\":{"
                << "\"is_online\":true,"
                << "\"temperature_mk\":" << thermal_pin.registers.core_temp_mk << ","
                << "\"heater_power_mw\":" << thermal_pin.registers.heater_power_mw << ","
                << "\"liquid_volume_ml\":" << thermal_pin.registers.liquid_volume_ml << ","
                << "\"hold_duration_ms\":" << (thermal_pin.registers.pasteurization_hold_s * 1000u)
                << "}";
        }
        oss << "}}";

        const std::string frame_json = oss.str();
        EM_ASM({
            var jsonStr = UTF8ToString($0);
            if (typeof window !== 'undefined') {
                try { window._oasisLastEngineFrame = JSON.parse(jsonStr); } catch (e) {}
                if (window.OasisUI && typeof window.OasisUI.onEngineFrame === 'function') {
                    window.OasisUI.onEngineFrame(jsonStr);
                }
            }
        }, frame_json.c_str());
    }
#endif

    // 1. Strictly 8-byte aligned members first (each has sizeof % 8 == 0 on both 64-bit native and 32-bit wasm32!)
    OasisAppConfig                                       config_{};
    SimulationClock                                      clock_{};
    ContentRegistry                                      registry_{};
    AgentDrivesComponent                                 steward_drives_{};
    AutonomyStateComponent                               steward_autonomy_{};
    ActiveStewardTask                                    active_task_{};
    uint64_t                                             tick_count_{0ULL};
    uint64_t                                             next_entity_uid_{100ULL};

    // 2. Pointer / STL-container aligned members (alignof == 8 on 64-bit native, alignof == 4 on 32-bit wasm32)
    std::unique_ptr<render::WebGpuRenderer>              renderer_{};
    std::unique_ptr<oasis::uhai::UhaiTelemetryChannel>   telemetry_channel_{};
    flecs::world                                         ecs_{};
    WorldGrid                                            world_grid_;
    VirtualPinBus                                        pin_bus_{};
    UnmodeledStepAdapter                                 step_adapter_{};
    InventoryComponent                                   steward_inventory_{};
    AgentCapabilitiesComponent                           steward_caps_{};
    EquippedHaulingToolComponent                         steward_equipped_tool_{};
    std::vector<flecs::entity>                           map_entities_{};
    std::vector<PastureHenAgent>                         hens_{};
    std::vector<GridCoord>                               autonomy_walk_path_{};
    std::vector<std::pair<std::string, std::string>>     active_bpmn_affordance_to_task_{};
    size_t                                               autonomy_walk_cursor_{0u};
    std::string                                          inner_thought_{};
    std::string                                          last_completed_l4_task_id_{};
    std::string                                          pending_inspection_report_json_{};

    // 3. 4-byte aligned spatial & input fields + explicit tail padding
    glm::vec3                steward_pos_{12.0f, 1.0f, 6.0f};
    uint32_t                 domain_sub_sec_ms_{0u};
    uint32_t                 built_bed_count_{0u};
    uint32_t                 last_observed_garden_growth_bp_{0u};
    uint32_t                 last_observed_garden_moisture_bp_{0u};
    int32_t                  last_mouse_x_{0};
    int32_t                  last_mouse_y_{0};
    bool                     running_{false};
    bool                     procedural_mode_active_{false};
    bool                     shift_down_{false};
    bool                     orbiting_camera_{false};
    bool                     panning_camera_{false};
    bool                     bpmn_token_sync_active_{false};
    uint8_t                  active_cultivar_code_{kCultivarCodeChronoVirtualPea};
    [[maybe_unused]] uint8_t app_pad_[5]{0u, 0u, 0u, 0u, 0u};
};

} // namespace oasis::engine::app
