#pragma once

#include <glm/glm.hpp>
#include <flecs.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

#include "core/fabrication.hpp"
#include "core/raycaster.hpp"
#include "uhai/uhai_ring_buffer.hpp"

namespace oasis::designator {

constexpr size_t MAX_DESIGNATION_VOXELS = 256;
constexpr uint8_t MAT_BLUEPRINT_GHOST = 26u;

using oasis::fabrication::MAT_CLAY_COB_LUMP;
using oasis::fabrication::MAT_URBANITE_CHUNK;
using oasis::fabrication::LooseItemComponent;
using oasis::fabrication::StockpileZoneComponent;
using oasis::fabrication::HaulLooseItemsToStockpile;

enum class VoxelContextCategory : uint8_t {
    NONE = 0,
    TERRAIN_DIRT = 1,
    FOUNDATION_ROOF = 2,
    LOOSE_ITEM = 3,
    ROOM_CLEARING = 4,
    EXISTING_BLUEPRINT = 5
};

enum class DesignatorActionId : uint16_t {
    NONE = 0,
    DIG_SWALE = 1,
    PLANT_COVER_CROP = 2,
    DESIGNATE_HAUL_ZONE = 3,
    BUILD_WALL_COB = 4,
    BUILD_WALL_URBANITE = 5,
    INSTALL_DOOR_TIMBER = 6,
    BUILD_HEARTH = 7,
    DESIGNATE_SLEEP_ZONE = 8,
    DESIGNATE_RECREATION_ZONE = 9,
    INSTALL_INVERTER = 10,
    ERECT_SOLAR_BLUEPRINT = 11,
    BUILD_ROOF = 12,
    HARVEST_GARDEN = 13,
    COOK_MEAL_KITCHEN = 14,
    CANCEL_DESIGNATION = 99
};

struct DesignatorToolSpec {
    uint16_t action_id{0};
    std::string name{};
    double kcal_cost{0.0};
    float focus_cost{0.0f};
    uint8_t target_material_id{0};
    bool is_zero_labor_zone{false};
    oasis::fabrication::InfrastructureType infra_type{oasis::fabrication::InfrastructureType::NONE};
};

[[nodiscard]] inline DesignatorToolSpec GetDesignatorToolSpecById(uint16_t action_id) {
    using oasis::fabrication::InfrastructureType;
    switch (action_id) {
        case static_cast<uint16_t>(DesignatorActionId::DIG_SWALE):
            return {1, "[Dig Swale]", 180.0, 0.10f, 18u, false, InfrastructureType::SWALE};
        case static_cast<uint16_t>(DesignatorActionId::PLANT_COVER_CROP):
            return {2, "[Plant Cover Crop]", 90.0, 0.05f, 19u, false, InfrastructureType::COVER_CROP};
        case static_cast<uint16_t>(DesignatorActionId::DESIGNATE_HAUL_ZONE):
            return {3, "[Designate Haul Zone]", 0.0, 0.0f, 20u, true, InfrastructureType::HAUL_ZONE};
        case static_cast<uint16_t>(DesignatorActionId::BUILD_WALL_COB):
            return {4, "[Build Wall: Cob]", 220.0, 0.12f, 13u, false, InfrastructureType::WALL_COB};
        case static_cast<uint16_t>(DesignatorActionId::BUILD_WALL_URBANITE):
            return {5, "[Build Wall: Urbanite]", 240.0, 0.14f, 14u, false, InfrastructureType::WALL_URBANITE};
        case static_cast<uint16_t>(DesignatorActionId::INSTALL_DOOR_TIMBER):
            return {6, "[Install Door: Insulated Timber]", 150.0, 0.08f, 15u, false, InfrastructureType::DOOR_TIMBER};
        case static_cast<uint16_t>(DesignatorActionId::BUILD_HEARTH):
            return {7, "[Build Hearth]", 300.0, 0.15f, 16u, false, InfrastructureType::ROCKET_MASS_HEATER};
        case static_cast<uint16_t>(DesignatorActionId::DESIGNATE_SLEEP_ZONE):
            return {8, "[Designate Sleep Zone]", 0.0, 0.0f, 21u, true, InfrastructureType::SLEEP_ZONE};
        case static_cast<uint16_t>(DesignatorActionId::DESIGNATE_RECREATION_ZONE):
            return {9, "[Designate Recreation Zone]", 0.0, 0.0f, 22u, true, InfrastructureType::RECREATION_ZONE};
        case static_cast<uint16_t>(DesignatorActionId::INSTALL_INVERTER):
            return {10, "[Install Hardware: Inverter]", 250.0, 0.15f, 12u, false, InfrastructureType::RAINWATER_CATCHMENT};
        case static_cast<uint16_t>(DesignatorActionId::ERECT_SOLAR_BLUEPRINT):
            return {11, "[Erect Solar Blueprint]", 500.0, 0.20f, 11u, false, InfrastructureType::SOLAR_PANEL};
        case static_cast<uint16_t>(DesignatorActionId::BUILD_ROOF):
            return {12, "[Build Roof]", 200.0, 0.10f, 17u, false, InfrastructureType::ROOF_TIMBER};
        case static_cast<uint16_t>(DesignatorActionId::HARVEST_GARDEN):
            return {13, "[Harvest Garden]", 40.0, 0.03f, 19u, true, InfrastructureType::COVER_CROP};
        case static_cast<uint16_t>(DesignatorActionId::COOK_MEAL_KITCHEN):
            return {14, "[Cook Meal (Scenario Γ)]", 0.0, 0.02f, 24u, true, InfrastructureType::NONE};
        case static_cast<uint16_t>(DesignatorActionId::CANCEL_DESIGNATION):
            return {99, "[Cancel Designation]", 0.0, 0.0f, 0u, true, InfrastructureType::NONE};
        default:
            return {4, "[Build Wall: Cob]", 220.0, 0.12f, 13u, false, InfrastructureType::WALL_COB};
    }
}

struct VoxelContextMenu {
    bool is_open{false};
    VoxelContextCategory category{VoxelContextCategory::NONE};
    glm::ivec3 voxel_pos{0};
    uint32_t material_id{0};
    std::vector<DesignatorToolSpec> actions{};

    [[nodiscard]] bool HasAction(std::string_view action_label) const noexcept {
        for (const auto& spec : actions) {
            if (spec.name == action_label || spec.name.find(action_label) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] const DesignatorToolSpec* FindAction(std::string_view action_label) const noexcept {
        for (const auto& spec : actions) {
            if (spec.name == action_label || spec.name.find(action_label) != std::string::npos) {
                return &spec;
            }
        }
        return nullptr;
    }

    [[nodiscard]] size_t size() const noexcept { return actions.size(); }
    [[nodiscard]] bool empty() const noexcept { return actions.empty(); }
};

using ContextMenuEvaluation = VoxelContextMenu;
using VoxelContextEvaluation = VoxelContextMenu;

[[nodiscard]] constexpr bool IsInsideLot402HouseInterior(int x, int y, int z) noexcept {
    const int ax = (x < 0) ? -x : x;
    const int local_z = ((z % 16) + 16) % 16;
    return (ax >= 9 && ax <= 17) && (local_z >= 2 && local_z <= 10) && (y <= 3);
}

[[nodiscard]] constexpr bool IsInsideLot402HouseInterior(glm::ivec3 pos) noexcept {
    return IsInsideLot402HouseInterior(pos.x, pos.y, pos.z);
}

[[nodiscard]] inline VoxelContextMenu EvaluateVoxelContext(
    const oasis::graphics::RaycastHit& hit,
    bool has_active_blueprint_at_pos = false
) {
    VoxelContextMenu menu{};
    if (!hit.hit) {
        menu.is_open = false;
        menu.category = VoxelContextCategory::NONE;
        return menu;
    }

    menu.is_open = true;
    menu.voxel_pos = hit.voxel_pos;
    menu.material_id = hit.material_id;

    const uint32_t mat = hit.material_id;
    const int ax = std::abs(hit.voxel_pos.x);
    const int local_z = ((hit.voxel_pos.z % 16) + 16) % 16;
    const bool is_indoor_floor =
        (mat == 4u) && IsInsideLot402HouseInterior(hit.voxel_pos.x, hit.voxel_pos.y, hit.voxel_pos.z) &&
        (ax >= 10 && ax <= 16) && (local_z >= 3 && local_z <= 9);

    // Outdoor Dirt / Lawn / Swale / Cover Crop / Mature Crop (34u)
    if (!is_indoor_floor && (mat == 4u || mat == 18u || mat == 19u || mat == 34u)) {
        menu.category = VoxelContextCategory::TERRAIN_DIRT;
        if (mat == 34u) {
            menu.actions.push_back({
                static_cast<uint16_t>(DesignatorActionId::HARVEST_GARDEN),
                "[Harvest Garden]", 40.0, 0.03f, 19u, true,
                oasis::fabrication::InfrastructureType::COVER_CROP
            });
        }
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::DIG_SWALE),
            "[Dig Swale]", 180.0, 0.10f, 18u, false,
            oasis::fabrication::InfrastructureType::SWALE
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::PLANT_COVER_CROP),
            "[Plant Cover Crop]", 90.0, 0.05f, 19u, false,
            oasis::fabrication::InfrastructureType::COVER_CROP
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::DESIGNATE_HAUL_ZONE),
            "[Designate Haul Zone]", 0.0, 0.0f, 20u, true,
            oasis::fabrication::InfrastructureType::HAUL_ZONE
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::BUILD_WALL_COB),
            "[Build Wall: Cob]", 220.0, 0.12f, 13u, false,
            oasis::fabrication::InfrastructureType::WALL_COB
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::BUILD_WALL_URBANITE),
            "[Build Wall: Urbanite]", 240.0, 0.14f, 14u, false,
            oasis::fabrication::InfrastructureType::WALL_URBANITE
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::INSTALL_DOOR_TIMBER),
            "[Install Door: Insulated Timber]", 150.0, 0.08f, 15u, false,
            oasis::fabrication::InfrastructureType::DOOR_TIMBER
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::BUILD_HEARTH),
            "[Build Hearth]", 300.0, 0.15f, 16u, false,
            oasis::fabrication::InfrastructureType::ROCKET_MASS_HEATER
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::DESIGNATE_SLEEP_ZONE),
            "[Designate Sleep Zone]", 0.0, 0.0f, 21u, true,
            oasis::fabrication::InfrastructureType::SLEEP_ZONE
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::DESIGNATE_RECREATION_ZONE),
            "[Designate Recreation Zone]", 0.0, 0.0f, 22u, true,
            oasis::fabrication::InfrastructureType::RECREATION_ZONE
        });
    } else if (mat == 1u || mat == 2u || mat == 3u || mat == 5u || mat == 6u || mat == 13u || mat == 14u) {
        // Foundation / Concrete / Siding / Roof
        menu.category = VoxelContextCategory::FOUNDATION_ROOF;
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::INSTALL_INVERTER),
            "[Install Hardware: Inverter]", 250.0, 0.15f, 12u, false,
            oasis::fabrication::InfrastructureType::RAINWATER_CATCHMENT
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::ERECT_SOLAR_BLUEPRINT),
            "[Erect Solar Blueprint]", 500.0, 0.20f, 11u, false,
            oasis::fabrication::InfrastructureType::SOLAR_PANEL
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::BUILD_ROOF),
            "[Build Roof]", 200.0, 0.10f, 17u, false,
            oasis::fabrication::InfrastructureType::ROOF_TIMBER
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::BUILD_WALL_COB),
            "[Build Wall: Cob]", 220.0, 0.12f, 13u, false,
            oasis::fabrication::InfrastructureType::WALL_COB
        });
    } else if (mat == MAT_CLAY_COB_LUMP || mat == MAT_URBANITE_CHUNK) {
        menu.category = VoxelContextCategory::LOOSE_ITEM;
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::DESIGNATE_HAUL_ZONE),
            "[Designate Haul Zone]", 0.0, 0.0f, 20u, true,
            oasis::fabrication::InfrastructureType::HAUL_ZONE
        });
    } else {
        menu.category = VoxelContextCategory::ROOM_CLEARING;
        if (mat == 24u || is_indoor_floor) {
            menu.actions.push_back({
                static_cast<uint16_t>(DesignatorActionId::COOK_MEAL_KITCHEN),
                "[Cook Meal (Scenario Γ)]", 0.0, 0.02f, 24u, true,
                oasis::fabrication::InfrastructureType::NONE
            });
        }
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::DESIGNATE_SLEEP_ZONE),
            "[Designate Sleep Zone]", 0.0, 0.0f, 21u, true,
            oasis::fabrication::InfrastructureType::SLEEP_ZONE
        });
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::DESIGNATE_RECREATION_ZONE),
            "[Designate Recreation Zone]", 0.0, 0.0f, 22u, true,
            oasis::fabrication::InfrastructureType::RECREATION_ZONE
        });
        if (is_indoor_floor) {
            menu.actions.push_back({
                static_cast<uint16_t>(DesignatorActionId::BUILD_WALL_COB),
                "[Build Wall: Cob]", 220.0, 0.12f, 13u, false,
                oasis::fabrication::InfrastructureType::WALL_COB
            });
            menu.actions.push_back({
                static_cast<uint16_t>(DesignatorActionId::INSTALL_DOOR_TIMBER),
                "[Install Door: Insulated Timber]", 150.0, 0.08f, 15u, false,
                oasis::fabrication::InfrastructureType::DOOR_TIMBER
            });
        }
    }

    if (has_active_blueprint_at_pos) {
        if (menu.category == VoxelContextCategory::NONE) {
            menu.category = VoxelContextCategory::EXISTING_BLUEPRINT;
        }
        menu.actions.push_back({
            static_cast<uint16_t>(DesignatorActionId::CANCEL_DESIGNATION),
            "[Cancel Designation]", 0.0, 0.0f, 0u, true,
            oasis::fabrication::InfrastructureType::NONE
        });
    }

    return menu;
}

struct DesignationVolume {
    glm::ivec3 min_bound{0};
    glm::ivec3 max_bound{0};
    glm::ivec3 extents{0};
    size_t total_voxels{0};
    bool exceeds_cap{false};
    bool is_valid{false};
    std::vector<glm::ivec3> voxels{};
};

using DesignationVolumeResult = DesignationVolume;

[[nodiscard]] inline DesignationVolume ComputeDesignationVolume(glm::ivec3 start_pos, glm::ivec3 end_pos) {
    DesignationVolume vol{};
    vol.min_bound = glm::min(start_pos, end_pos);
    vol.max_bound = glm::max(start_pos, end_pos);
    vol.extents = (vol.max_bound - vol.min_bound) + glm::ivec3(1, 1, 1);

    const size_t nx = static_cast<size_t>(std::max(0, vol.extents.x));
    const size_t ny = static_cast<size_t>(std::max(0, vol.extents.y));
    const size_t nz = static_cast<size_t>(std::max(0, vol.extents.z));
    vol.total_voxels = nx * ny * nz;
    vol.exceeds_cap = (vol.total_voxels > MAX_DESIGNATION_VOXELS);
    vol.is_valid = (!vol.exceeds_cap && vol.total_voxels > 0);

    if (vol.is_valid) {
        vol.voxels.reserve(vol.total_voxels);
        for (int y = vol.min_bound.y; y <= vol.max_bound.y; ++y) {
            for (int z = vol.min_bound.z; z <= vol.max_bound.z; ++z) {
                for (int x = vol.min_bound.x; x <= vol.max_bound.x; ++x) {
                    vol.voxels.emplace_back(x, y, z);
                }
            }
        }
    }

    return vol;
}

[[nodiscard]] inline oasis::uhai::SpatialIntentPayload BuildSpatialIntentPayload(
    uint16_t action_id,
    glm::ivec3 origin,
    glm::ivec3 extents,
    uint64_t timestamp_ns = 0
) noexcept {
    oasis::uhai::SpatialIntentPayload pkt{};
    pkt.timestamp_ns = timestamp_ns;
    pkt.intent_type = static_cast<uint16_t>(oasis::uhai::IntentType::SPATIAL_AFFORDANCE);
    pkt.quality_flag = oasis::uhai::QUALITY_SIMULATED; // 0x01
    pkt.status = 1; // Validated

    pkt.spatial_origin[0] = static_cast<float>(origin.x);
    pkt.spatial_origin[1] = static_cast<float>(origin.y);
    pkt.spatial_origin[2] = static_cast<float>(origin.z);

    pkt.spatial_extents[0] = static_cast<float>(extents.x);
    pkt.spatial_extents[1] = static_cast<float>(extents.y);
    pkt.spatial_extents[2] = static_cast<float>(extents.z);

    std::strncpy(pkt.schema_uri, "uhai://schema/spatial-intent/v1", sizeof(pkt.schema_uri) - 1);
    std::strncpy(reinterpret_cast<char*>(pkt.author_did), "did:key:z6MkFounderOasisSitl01", sizeof(pkt.author_did) - 1);

    int written = std::snprintf(
        pkt.payload_jsonld,
        sizeof(pkt.payload_jsonld),
        "{\"@context\":\"https://uhai.org/v1\",\"@type\":\"SpatialAffordance\",\"action\":%u,\"origin\":[%d,%d,%d],\"extents\":[%d,%d,%d]}",
        static_cast<unsigned>(action_id),
        origin.x, origin.y, origin.z,
        extents.x, extents.y, extents.z
    );
    if (written > 0) {
        pkt.payload_bytes = static_cast<uint32_t>(std::min<size_t>(static_cast<size_t>(written), sizeof(pkt.payload_jsonld) - 1));
    }

    // Deterministic FNV-1a digest into intent_cid
    uint64_t hash = 14695981039346656037ULL;
    for (size_t i = 0; i < pkt.payload_bytes; ++i) {
        hash ^= static_cast<uint8_t>(pkt.payload_jsonld[i]);
        hash *= 1099511628211ULL;
    }
    for (size_t i = 0; i < 4; ++i) {
        uint64_t word = hash ^ (timestamp_ns + i * 0x9E3779B97F4A7C15ULL);
        std::memcpy(pkt.intent_cid + i * 8, &word, sizeof(uint64_t));
    }

    return pkt;
}

// Destroys any uncompleted BlueprintComponent entity at `pos` and refunds reserved materials
inline bool CancelBlueprintAtVoxel(flecs::world& ecs, glm::ivec3 pos) {
    std::vector<flecs::entity> to_cancel;
    uint32_t refund_cob = 0;
    uint32_t refund_urbanite = 0;

    ecs.each([&](flecs::entity e, oasis::fabrication::BlueprintComponent& bp) {
        if (!bp.is_completed && bp.x == pos.x && bp.y == pos.y && bp.z == pos.z) {
            to_cancel.push_back(e);
            refund_cob += bp.reserved_cob_lumps;
            refund_urbanite += bp.reserved_urbanite_chunks;
        }
    });

    if (to_cancel.empty()) {
        return false;
    }

    // Refund reserved stockpile materials if any
    if (refund_cob > 0 || refund_urbanite > 0) {
        bool refunded_to_zone = false;
        ecs.each([&](flecs::entity, oasis::fabrication::StockpileZoneComponent& zone) {
            if (!refunded_to_zone) {
                zone.stored_cob_lumps += refund_cob;
                zone.stored_urbanite_chunks += refund_urbanite;
                refunded_to_zone = true;
            }
        });
    }

    // Unreserve any loose items that were reserved
    ecs.each([&](flecs::entity, oasis::fabrication::LooseItemComponent& item) {
        if (item.is_reserved) {
            item.is_reserved = false;
        }
    });

    // Reset FounderCommandComponent if targeting a cancelled blueprint
    auto founder = ecs.entity("Founder");
    if (founder.is_alive() && founder.has<oasis::fabrication::FounderCommandComponent>()) {
        auto& cmd = founder.get_mut<oasis::fabrication::FounderCommandComponent>();
        for (const auto& ce : to_cancel) {
            if (cmd.target_blueprint == ce || cmd.target_voxel == pos) {
                cmd.state = oasis::fabrication::FounderActionState::IDLE;
                cmd.target_blueprint = flecs::entity{};
            }
        }
    }

    for (auto& ce : to_cancel) {
        ce.destruct();
    }

    const uint8_t cur_dyn = oasis::graphics::GetDynamicVoxel(pos.x, pos.y, pos.z);
    if (cur_dyn == MAT_BLUEPRINT_GHOST || cur_dyn == 33u) {
        oasis::graphics::SetDynamicVoxel(pos.x, pos.y, pos.z, 0u);
    }

    return true;
}

} // namespace oasis::designator

namespace oasis::fabrication {
    using oasis::designator::MAX_DESIGNATION_VOXELS;
    using oasis::designator::MAT_BLUEPRINT_GHOST;
    using oasis::designator::VoxelContextCategory;
    using oasis::designator::DesignatorActionId;
    using oasis::designator::DesignatorToolSpec;
    using oasis::designator::GetDesignatorToolSpecById;
    using oasis::designator::VoxelContextMenu;
    using oasis::designator::IsInsideLot402HouseInterior;
    using oasis::designator::EvaluateVoxelContext;
    using oasis::designator::DesignationVolume;
    using oasis::designator::ComputeDesignationVolume;
    using oasis::designator::BuildSpatialIntentPayload;
    using oasis::designator::CancelBlueprintAtVoxel;
} // namespace oasis::fabrication

namespace oasis::graphics {
    using oasis::designator::MAX_DESIGNATION_VOXELS;
    using oasis::designator::MAT_BLUEPRINT_GHOST;
    using oasis::designator::VoxelContextCategory;
    using oasis::designator::DesignatorActionId;
    using oasis::designator::DesignatorToolSpec;
    using oasis::designator::GetDesignatorToolSpecById;
    using oasis::designator::VoxelContextMenu;
    using oasis::designator::IsInsideLot402HouseInterior;
    using oasis::designator::EvaluateVoxelContext;
    using oasis::designator::DesignationVolume;
    using oasis::designator::ComputeDesignationVolume;
    using oasis::designator::BuildSpatialIntentPayload;
    using oasis::designator::CancelBlueprintAtVoxel;
} // namespace oasis::graphics

namespace oasis {
    using oasis::designator::MAX_DESIGNATION_VOXELS;
    using oasis::designator::MAT_BLUEPRINT_GHOST;
    using oasis::designator::VoxelContextCategory;
    using oasis::designator::DesignatorActionId;
    using oasis::designator::DesignatorToolSpec;
    using oasis::designator::GetDesignatorToolSpecById;
    using oasis::designator::VoxelContextMenu;
    using oasis::designator::IsInsideLot402HouseInterior;
    using oasis::designator::EvaluateVoxelContext;
    using oasis::designator::DesignationVolume;
    using oasis::designator::ComputeDesignationVolume;
    using oasis::designator::BuildSpatialIntentPayload;
    using oasis::designator::CancelBlueprintAtVoxel;
    using oasis::fabrication::MAT_CLAY_COB_LUMP;
    using oasis::fabrication::MAT_URBANITE_CHUNK;
    using oasis::fabrication::LooseItemComponent;
    using oasis::fabrication::StockpileZoneComponent;
} // namespace oasis
