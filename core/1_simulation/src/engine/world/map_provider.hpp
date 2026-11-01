#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../content/content_registry.hpp"

namespace oasis::engine {

// Multi-strata vertical bounds: y in [-2 .. 15] (y < 0 holds subsurface earthworks & structural footings)
inline constexpr int32_t kMinStrataY   = -2;
inline constexpr int32_t kMaxStrataY   = 15;
inline constexpr int32_t kStrataCountY = (kMaxStrataY - kMinStrataY) + 1; // 18 layers

// Surface & architectural material IDs (harmonized with WebGPU diorama & suburban_lot_402.json palette)
inline constexpr uint16_t MAT_VOXEL_AIR               = 0u;
inline constexpr uint16_t MAT_ASPHALT_ROAD            = 1u;
inline constexpr uint16_t MAT_ROAD_CENTERLINE         = 2u;
inline constexpr uint16_t MAT_CONCRETE_PAVEMENT       = 3u;
inline constexpr uint16_t MAT_SURFACE_LAWN_OR_FLOOR   = 4u;
inline constexpr uint16_t MAT_CLAPBOARD_WALL          = 5u;
inline constexpr uint16_t MAT_SHINGLE_ROOF            = 6u;
inline constexpr uint16_t MAT_WINDOW_GLASS            = 7u;
inline constexpr uint16_t MAT_PERIMETER_FENCE         = 10u;
inline constexpr uint16_t MAT_INSULATED_TIMBER_DOOR   = 15u;
inline constexpr uint16_t MAT_TILLED_GARDEN_SOIL      = 19u;
inline constexpr uint16_t MAT_ROOF_ACCESS_LADDER      = 32u;
inline constexpr uint16_t MAT_GARAGE_SECTIONAL_DOOR   = 57u;

// Subsurface earthworks & structural foundation strata material IDs (aligned with suburban_lot_402.json 64..68)
inline constexpr uint16_t STRATA_CLAY_SUBSOIL           = 64u;
inline constexpr uint16_t STRATA_COMPACTED_GRAVEL_PAD   = 65u;
inline constexpr uint16_t STRATA_CONCRETE_FOOTING_SLAB  = 66u;
inline constexpr uint16_t STRATA_TOPSOIL_LOAM           = 67u;
inline constexpr uint16_t STRATA_UTILITY_CONDUIT_TRENCH = 68u;

struct alignas(8) ExcavationResult {
    std::string      yielded_item_id{};
    std::string      strata_label{};
    uint32_t         quantity_units{0u};
    uint32_t         total_mass_g{0u};
    uint16_t         previous_material{MAT_VOXEL_AIR};
    MatterPhase      matter_phase{MatterPhase::GRANULAR_PARTICULATE};
    HaulingToolClass recommended_tool_class{HaulingToolClass::WHEELBARROW};
    bool             excavated{false};
    uint8_t          pad_[3]{0u, 0u, 0u};
};

struct alignas(8) PlacedEntitySeed {
    std::string entity_id{};
    std::string archetype_id{};
    std::string room_tag{};
    std::string initial_item_id{};
    int32_t     x{0};
    int32_t     y{1};
    int32_t     z{0};
    uint32_t    initial_item_qty{0u};
    uint16_t    voxel_material_id{0u};
    bool        has_l2_sensor{false};
    uint8_t     pad_[5]{0u, 0u, 0u, 0u, 0u};
};

class WorldGrid {
public:
    WorldGrid(
        int32_t min_x = -32,
        int32_t max_x = 31,
        int32_t min_z = -16,
        int32_t max_z = 31
    )
        : min_x_(std::min(min_x, max_x)),
          max_x_(std::max(min_x, max_x)),
          min_z_(std::min(min_z, max_z)),
          max_z_(std::max(min_z, max_z)),
          size_x_((std::max(min_x, max_x) - std::min(min_x, max_x)) + 1),
          size_z_((std::max(min_z, max_z) - std::min(min_z, max_z)) + 1) {
        cells_.assign(
            static_cast<size_t>(size_x_) *
            static_cast<size_t>(kStrataCountY) *
            static_cast<size_t>(size_z_),
            MAT_VOXEL_AIR
        );
    }

    void Clear(uint16_t fill_material = MAT_VOXEL_AIR) noexcept {
        std::fill(cells_.begin(), cells_.end(), fill_material);
    }

    [[nodiscard]] constexpr bool InBounds(int32_t x, int32_t y, int32_t z) const noexcept {
        return x >= min_x_ && x <= max_x_ &&
               y >= kMinStrataY && y <= kMaxStrataY &&
               z >= min_z_ && z <= max_z_;
    }

    [[nodiscard]] uint16_t GetVoxel(int32_t x, int32_t y, int32_t z) const noexcept {
        if (!InBounds(x, y, z)) {
            return MAT_VOXEL_AIR;
        }
        return cells_[FlatIndex(x, y, z)];
    }

    bool SetVoxel(int32_t x, int32_t y, int32_t z, uint16_t material_id) noexcept {
        if (!InBounds(x, y, z)) {
            return false;
        }
        cells_[FlatIndex(x, y, z)] = material_id;
        return true;
    }

    void FillBox(
        int32_t x0, int32_t y0, int32_t z0,
        int32_t x1, int32_t y1, int32_t z1,
        uint16_t material_id
    ) noexcept {
        const int32_t xa = std::max(min_x_, std::min(x0, x1));
        const int32_t xb = std::min(max_x_, std::max(x0, x1));
        const int32_t ya = std::max(kMinStrataY, std::min(y0, y1));
        const int32_t yb = std::min(kMaxStrataY, std::max(y0, y1));
        const int32_t za = std::max(min_z_, std::min(z0, z1));
        const int32_t zb = std::min(max_z_, std::max(z0, z1));
        for (int32_t y = ya; y <= yb; ++y) {
            for (int32_t z = za; z <= zb; ++z) {
                for (int32_t x = xa; x <= xb; ++x) {
                    cells_[FlatIndex(x, y, z)] = material_id;
                }
            }
        }
    }

    void FillHollowPerimeterXZ(
        int32_t x0, int32_t y0, int32_t z0,
        int32_t x1, int32_t y1, int32_t z1,
        uint16_t material_id
    ) noexcept {
        const int32_t xa = std::min(x0, x1);
        const int32_t xb = std::max(x0, x1);
        const int32_t ya = std::min(y0, y1);
        const int32_t yb = std::max(y0, y1);
        const int32_t za = std::min(z0, z1);
        const int32_t zb = std::max(z0, z1);
        for (int32_t y = ya; y <= yb; ++y) {
            for (int32_t z = za; z <= zb; ++z) {
                for (int32_t x = xa; x <= xb; ++x) {
                    if (x == xa || x == xb || z == za || z == zb) {
                        (void)SetVoxel(x, y, z, material_id);
                    }
                }
            }
        }
    }

    [[nodiscard]] bool IsPathPavedOrGraded(int32_t x, int32_t z) const noexcept {
        const uint16_t surf = GetVoxel(x, 0, z);
        const uint16_t sub  = GetVoxel(x, -1, z);
        return surf == MAT_ASPHALT_ROAD ||
               surf == MAT_ROAD_CENTERLINE ||
               surf == MAT_CONCRETE_PAVEMENT ||
               surf == STRATA_CONCRETE_FOOTING_SLAB ||
               surf == STRATA_COMPACTED_GRAVEL_PAD ||
               sub == STRATA_COMPACTED_GRAVEL_PAD ||
               sub == STRATA_CONCRETE_FOOTING_SLAB;
    }

    // Excavates an earthwork or aggregate tile into GRANULAR_PARTICULATE spoil requiring a bucket or wheelbarrow.
    ExcavationResult ExcavateTile(int32_t x, int32_t y, int32_t z) {
        ExcavationResult res{};
        if (!InBounds(x, y, z)) {
            return res;
        }
        const uint16_t mat = GetVoxel(x, y, z);
        res.previous_material = mat;
        if (mat == STRATA_TOPSOIL_LOAM || mat == MAT_TILLED_GARDEN_SOIL || mat == MAT_SURFACE_LAWN_OR_FLOOR) {
            SetVoxel(x, y, z, MAT_VOXEL_AIR);
            res.yielded_item_id        = "item.topsoil_loam";
            res.strata_label           = "Topsoil Loam Spoil";
            res.quantity_units         = 110u; // 110 kg loose topsoil loam
            res.total_mass_g           = 110000u;
            res.matter_phase           = MatterPhase::GRANULAR_PARTICULATE;
            res.recommended_tool_class = HaulingToolClass::WHEELBARROW;
            res.excavated              = true;
            return res;
        }
        if (mat == STRATA_CLAY_SUBSOIL) {
            SetVoxel(x, y, z, MAT_VOXEL_AIR);
            res.yielded_item_id        = "item.clay_subsoil";
            res.strata_label           = "Dense Clay Subsoil Spoil";
            res.quantity_units         = 120u; // 120 kg dense clay subsoil
            res.total_mass_g           = 120000u;
            res.matter_phase           = MatterPhase::GRANULAR_PARTICULATE;
            res.recommended_tool_class = HaulingToolClass::WHEELBARROW;
            res.excavated              = true;
            return res;
        }
        if (mat == STRATA_COMPACTED_GRAVEL_PAD) {
            SetVoxel(x, y, z, MAT_VOXEL_AIR);
            res.yielded_item_id        = "item.crushed_gravel";
            res.strata_label           = "Compacted Gravel Subbase";
            res.quantity_units         = 140u; // 140 kg aggregate gravel
            res.total_mass_g           = 140000u;
            res.matter_phase           = MatterPhase::GRANULAR_PARTICULATE;
            res.recommended_tool_class = HaulingToolClass::WHEELBARROW;
            res.excavated              = true;
            return res;
        }
        return res;
    }

    [[nodiscard]] constexpr int32_t MinX() const noexcept { return min_x_; }
    [[nodiscard]] constexpr int32_t MaxX() const noexcept { return max_x_; }
    [[nodiscard]] constexpr int32_t MinZ() const noexcept { return min_z_; }
    [[nodiscard]] constexpr int32_t MaxZ() const noexcept { return max_z_; }
    [[nodiscard]] constexpr int32_t MinY() const noexcept { return kMinStrataY; }
    [[nodiscard]] constexpr int32_t MaxY() const noexcept { return kMaxStrataY; }

private:
    [[nodiscard]] constexpr size_t FlatIndex(int32_t x, int32_t y, int32_t z) const noexcept {
        const size_t ix = static_cast<size_t>(x - min_x_);
        const size_t iy = static_cast<size_t>(y - kMinStrataY);
        const size_t iz = static_cast<size_t>(z - min_z_);
        return ((iy * static_cast<size_t>(size_z_)) + iz) * static_cast<size_t>(size_x_) + ix;
    }

    std::vector<uint16_t> cells_{};
    int32_t               min_x_{-32};
    int32_t               max_x_{31};
    int32_t               min_z_{-16};
    int32_t               max_z_{31};
    int32_t               size_x_{64};
    int32_t               size_z_{48};
};

class IMapProvider {
public:
    virtual ~IMapProvider() = default;
    virtual bool PopulateWorld(WorldGrid& out_grid, std::vector<PlacedEntitySeed>& out_entities) = 0;
};

// Loads a static JSON map file (e.g. suburban_lot_402.json) or an inline JSON document into WorldGrid + out_entities.
class StaticJsonMapProvider final : public IMapProvider {
public:
    explicit StaticJsonMapProvider(std::string file_path, bool treat_as_inline_json = false)
        : source_(std::move(file_path)),
          inline_mode_(treat_as_inline_json ? 1ULL : 0ULL) {}

    [[nodiscard]] static StaticJsonMapProvider FromJsonString(std::string json_text) {
        return StaticJsonMapProvider(std::move(json_text), true);
    }

    bool PopulateWorld(WorldGrid& out_grid, std::vector<PlacedEntitySeed>& out_entities) override {
        std::string payload;
        if (inline_mode_ != 0ULL) {
            payload = source_;
        } else {
            std::ifstream ifs(source_);
            if (!ifs.is_open()) {
                return false;
            }
            std::ostringstream oss;
            oss << ifs.rdbuf();
            payload = oss.str();
        }

        const std::string_view root(payload);
        const size_t start = json_util::SkipWs(root, 0u);
        if (start >= root.size() || root[start] != '{') {
            return false;
        }

        out_grid.Clear(MAT_VOXEL_AIR);
        out_entities.clear();

        // 1. Parse suburban_lot_402.json "voxel_fill_operations" (FILL_BOX, HOLLOW_PERIMETER_XZ, VOXEL_LIST)
        for (std::string_view op_obj : json_util::SplitObjectArray(root, "voxel_fill_operations")) {
            std::string op_type;
            int64_t mat = 0;
            if (!json_util::ReadString(op_obj, "op", op_type)) continue;
            if (!json_util::ReadInt64(op_obj, "material_id", mat)) {
                if (!json_util::ReadInt64(op_obj, "material", mat)) continue;
            }
            if (mat < 0) continue;
            const uint16_t mat_u16 = static_cast<uint16_t>(mat);

            if (op_type == "FILL_BOX" || op_type == "HOLLOW_PERIMETER_XZ") {
                const auto min_v = json_util::ReadIntArray(op_obj, "min");
                const auto max_v = json_util::ReadIntArray(op_obj, "max");
                if (min_v.size() >= 3u && max_v.size() >= 3u) {
                    if (op_type == "FILL_BOX") {
                        out_grid.FillBox(
                            static_cast<int32_t>(min_v[0]), static_cast<int32_t>(min_v[1]), static_cast<int32_t>(min_v[2]),
                            static_cast<int32_t>(max_v[0]), static_cast<int32_t>(max_v[1]), static_cast<int32_t>(max_v[2]),
                            mat_u16
                        );
                    } else {
                        out_grid.FillHollowPerimeterXZ(
                            static_cast<int32_t>(min_v[0]), static_cast<int32_t>(min_v[1]), static_cast<int32_t>(min_v[2]),
                            static_cast<int32_t>(max_v[0]), static_cast<int32_t>(max_v[1]), static_cast<int32_t>(max_v[2]),
                            mat_u16
                        );
                    }
                }
            } else if (op_type == "VOXEL_LIST") {
                ApplyNestedVoxelPositions(op_obj, "positions", mat_u16, out_grid);
            }
        }

        // 2. Also support compact mod map arrays "strata_fills" & "voxels"
        for (std::string_view box : json_util::SplitObjectArray(root, "strata_fills")) {
            int64_t x0 = 0, y0 = 0, z0 = 0, x1 = 0, y1 = 0, z1 = 0, mat = 0;
            if (json_util::ReadInt64(box, "x0", x0) && json_util::ReadInt64(box, "y0", y0) &&
                json_util::ReadInt64(box, "z0", z0) && json_util::ReadInt64(box, "x1", x1) &&
                json_util::ReadInt64(box, "y1", y1) && json_util::ReadInt64(box, "z1", z1) &&
                json_util::ReadInt64(box, "material", mat) && mat >= 0) {
                out_grid.FillBox(
                    static_cast<int32_t>(x0), static_cast<int32_t>(y0), static_cast<int32_t>(z0),
                    static_cast<int32_t>(x1), static_cast<int32_t>(y1), static_cast<int32_t>(z1),
                    static_cast<uint16_t>(mat)
                );
            }
        }
        for (std::string_view vox : json_util::SplitObjectArray(root, "voxels")) {
            int64_t x = 0, y = 0, z = 0, mat = 0;
            if (json_util::ReadInt64(vox, "x", x) && json_util::ReadInt64(vox, "y", y) &&
                json_util::ReadInt64(vox, "z", z) && json_util::ReadInt64(vox, "material", mat) && mat >= 0) {
                (void)out_grid.SetVoxel(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(z), static_cast<uint16_t>(mat));
            }
        }

        // 3. Parse entities from both "placed_entities" (suburban_lot_402.json) and "entities"
        auto parse_entities = [&](std::string_view key) {
            for (std::string_view ent : json_util::SplitObjectArray(root, key)) {
                PlacedEntitySeed seed{};
                if (!json_util::ReadString(ent, "entity_id", seed.entity_id) || seed.entity_id.empty()) continue;
                (void)json_util::ReadString(ent, "archetype_id", seed.archetype_id);
                if (!json_util::ReadString(ent, "room_tag", seed.room_tag)) {
                    (void)json_util::ReadString(ent, "room_id", seed.room_tag);
                }
                if (!json_util::ReadString(ent, "initial_item_id", seed.initial_item_id)) {
                    if (json_util::ReadString(ent, "hauling_tool_id", seed.initial_item_id)) {
                        seed.initial_item_qty = 1u;
                    }
                }
                const auto pos_arr = json_util::ReadIntArray(ent, "position");
                if (pos_arr.size() >= 3u) {
                    seed.x = static_cast<int32_t>(pos_arr[0]);
                    seed.y = static_cast<int32_t>(pos_arr[1]);
                    seed.z = static_cast<int32_t>(pos_arr[2]);
                } else {
                    int64_t v = 0;
                    if (json_util::ReadInt64(ent, "x", v)) seed.x = static_cast<int32_t>(v);
                    if (json_util::ReadInt64(ent, "y", v)) seed.y = static_cast<int32_t>(v);
                    if (json_util::ReadInt64(ent, "z", v)) seed.z = static_cast<int32_t>(v);
                }
                int64_t v = 0;
                if (json_util::ReadInt64(ent, "initial_item_qty", v) && v >= 0) {
                    seed.initial_item_qty = static_cast<uint32_t>(v);
                }
                if (json_util::ReadInt64(ent, "voxel_material_id", v) && v >= 0) {
                    seed.voxel_material_id = static_cast<uint16_t>(v);
                    if (seed.voxel_material_id > 0u) {
                        ApplyNestedVoxelPositions(ent, "footprint_voxels", seed.voxel_material_id, out_grid);
                    }
                }
                (void)json_util::ReadBool(ent, "has_l2_sensor", seed.has_l2_sensor);
                out_entities.push_back(std::move(seed));
            }
        };
        parse_entities("placed_entities");
        parse_entities("entities");

        return true;
    }

private:
    static void ApplyNestedVoxelPositions(
        std::string_view obj,
        std::string_view key,
        uint16_t mat_u16,
        WorldGrid& out_grid
    ) {
        const size_t arr_pos = json_util::FindKeyValPos(obj, key);
        if (arr_pos == std::string_view::npos || arr_pos >= obj.size() || obj[arr_pos] != '[') {
            return;
        }
        const size_t arr_end = json_util::FindMatchingBracket(obj, arr_pos, '[', ']');
        if (arr_end == std::string_view::npos) {
            return;
        }
        size_t cur = arr_pos + 1u;
        while (cur < arr_end) {
            cur = json_util::SkipWs(obj, cur);
            if (cur < arr_end && obj[cur] == '[') {
                const size_t sub_end = json_util::FindMatchingBracket(obj, cur, '[', ']');
                if (sub_end == std::string_view::npos || sub_end > arr_end) break;
                const std::string wrapped = std::string("{\"p\":") + std::string(obj.substr(cur, (sub_end - cur) + 1u)) + "}";
                const auto coords = json_util::ReadIntArray(wrapped, "p");
                if (coords.size() >= 3u) {
                    (void)out_grid.SetVoxel(
                        static_cast<int32_t>(coords[0]),
                        static_cast<int32_t>(coords[1]),
                        static_cast<int32_t>(coords[2]),
                        mat_u16
                    );
                }
                cur = sub_end + 1u;
            } else {
                ++cur;
            }
        }
    }

    std::string source_{};
    uint64_t    inline_mode_{0ULL};
};

// Procedurally generates a graded suburban lot in realistic civil-engineering construction order:
// clay subsoil -> cut-and-fill compacted gravel pad + concrete perimeter footings -> road/driveway/topsoil lawn ->
// hollow 6-room house + attached garage shell -> clean-slate Day-1 interior furniture & garage hauling tools.
class ProceduralSuburbanEarthworksGenerator final : public IMapProvider {
public:
    constexpr explicit ProceduralSuburbanEarthworksGenerator(int32_t lot_number = 402) noexcept
        : lot_number_(lot_number),
          reserved_cfg_(0) {}

    bool PopulateWorld(WorldGrid& out_grid, std::vector<PlacedEntitySeed>& out_entities) override {
        (void)lot_number_;
        (void)reserved_cfg_;
        out_grid.Clear(MAT_VOXEL_AIR);
        out_entities.clear();

        const int32_t gx0 = out_grid.MinX();
        const int32_t gx1 = out_grid.MaxX();
        const int32_t gz0 = out_grid.MinZ();
        const int32_t gz1 = out_grid.MaxZ();

        // 1. Deep geological horizon (y = -2): undisturbed clay subsoil across entire parcel
        out_grid.FillBox(gx0, -2, gz0, gx1, -2, gz1, STRATA_CLAY_SUBSOIL);

        // 2. Cut-and-fill subbase (y = -1): topsoil loam beneath yards, gravel pad + concrete footings under structures
        out_grid.FillBox(gx0, -1, gz0, gx1, -1, gz1, STRATA_TOPSOIL_LOAM);
        out_grid.FillBox(-5, -1, gz0, 5, -1, gz1, STRATA_COMPACTED_GRAVEL_PAD);
        out_grid.FillBox(6, -1, 1, 8, -1, 3, STRATA_COMPACTED_GRAVEL_PAD);
        out_grid.FillBox(6, -1, 11, 7, -1, 12, STRATA_COMPACTED_GRAVEL_PAD);
        out_grid.FillBox(10, -1, 3, 16, -1, 9, STRATA_COMPACTED_GRAVEL_PAD);
        out_grid.FillBox(9, -1, 11, 11, -1, 13, STRATA_COMPACTED_GRAVEL_PAD);
        out_grid.FillHollowPerimeterXZ(9, -1, 2, 17, -1, 10, STRATA_CONCRETE_FOOTING_SLAB);
        out_grid.FillHollowPerimeterXZ(8, -1, 10, 12, -1, 14, STRATA_CONCRETE_FOOTING_SLAB);
        out_grid.FillBox(4, -1, 5, 8, -1, 5, STRATA_UTILITY_CONDUIT_TRENCH);

        // 3. Finished grade (y = 0): topsoil lawn, asphalt street, concrete sidewalks/driveway, poured slabs
        out_grid.FillBox(gx0, 0, gz0, gx1, 0, gz1, STRATA_TOPSOIL_LOAM);
        for (int32_t z = gz0; z <= gz1; ++z) {
            for (int32_t x = -3; x <= 3; ++x) {
                const bool centerline = (x == 0) && (((z % 4) + 4) % 4 >= 2);
                (void)out_grid.SetVoxel(x, 0, z, centerline ? MAT_ROAD_CENTERLINE : MAT_ASPHALT_ROAD);
            }
            (void)out_grid.SetVoxel(-5, 0, z, MAT_CONCRETE_PAVEMENT);
            (void)out_grid.SetVoxel(-4, 0, z, MAT_CONCRETE_PAVEMENT);
            (void)out_grid.SetVoxel(4, 0, z, MAT_CONCRETE_PAVEMENT);
            (void)out_grid.SetVoxel(5, 0, z, MAT_CONCRETE_PAVEMENT);
        }
        out_grid.FillBox(6, 0, 1, 8, 0, 3, MAT_CONCRETE_PAVEMENT);
        out_grid.FillBox(6, 0, 11, 7, 0, 12, MAT_CONCRETE_PAVEMENT);
        out_grid.FillBox(9, 0, 2, 17, 0, 10, STRATA_CONCRETE_FOOTING_SLAB);
        out_grid.FillBox(8, 0, 10, 12, 0, 14, STRATA_CONCRETE_FOOTING_SLAB);

        // 4. Hollow 6-room house + attached garage shell (y in [1..6], open interior rooms & corridors)
        out_grid.FillHollowPerimeterXZ(9, 1, 2, 17, 3, 10, MAT_CLAPBOARD_WALL);
        out_grid.FillBox(9, 1, 6, 9, 2, 6, MAT_INSULATED_TIMBER_DOOR);
        out_grid.FillBox(17, 1, 6, 17, 2, 6, MAT_INSULATED_TIMBER_DOOR);
        out_grid.FillBox(10, 1, 10, 10, 2, 10, MAT_VOXEL_AIR);
        (void)out_grid.SetVoxel(9, 2, 4, MAT_WINDOW_GLASS);
        (void)out_grid.SetVoxel(9, 2, 8, MAT_WINDOW_GLASS);
        (void)out_grid.SetVoxel(17, 2, 4, MAT_WINDOW_GLASS);
        (void)out_grid.SetVoxel(17, 2, 8, MAT_WINDOW_GLASS);
        for (int32_t y = 4; y <= 6; ++y) {
            const int32_t inset = y - 3;
            out_grid.FillBox(9 + inset, y, 2 + inset, 17 - inset, y, 10 - inset, MAT_SHINGLE_ROOF);
        }

        out_grid.FillBox(8, 1, 10, 8, 3, 14, MAT_CLAPBOARD_WALL);
        out_grid.FillBox(12, 1, 11, 12, 3, 14, MAT_CLAPBOARD_WALL);
        out_grid.FillBox(8, 1, 14, 12, 3, 14, MAT_CLAPBOARD_WALL);
        out_grid.FillBox(8, 1, 11, 8, 2, 12, MAT_GARAGE_SECTIONAL_DOOR);
        out_grid.FillBox(8, 4, 11, 12, 4, 14, MAT_SHINGLE_ROOF);
        out_grid.FillBox(18, 1, 3, 18, 4, 3, MAT_ROOF_ACCESS_LADDER);

        out_grid.FillBox(7, 1, 1, 25, 1, 1, MAT_PERIMETER_FENCE);
        out_grid.FillBox(7, 1, 15, 25, 1, 15, MAT_PERIMETER_FENCE);
        out_grid.FillBox(25, 1, 1, 25, 1, 7, MAT_PERIMETER_FENCE);
        out_grid.FillBox(25, 1, 9, 25, 1, 15, MAT_PERIMETER_FENCE);

        // 5. Clean-slate Day-1 interior furniture & garage hauling tools (zero pre-built backyard structures or sensors)
        out_entities.push_back(PlacedEntitySeed{"entity.kitchen_fridge", "arch.refrigerator_larder", "room.kitchen_dining", "item.potable_water", 10, 1, 3, 12u, 40u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.kitchen_induction_hearth", "arch.induction_hearth_stove", "room.kitchen_dining", "", 11, 1, 3, 0u, 41u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.dining_table", "arch.dining_table_set", "room.kitchen_dining", "", 10, 1, 5, 0u, 43u, false});
        out_entities.push_back(PlacedEntitySeed{"tool.tiffin_carrier", "arch.hauling_tool_tiffin_carrier", "room.kitchen_dining", "tiffin_carrier", 12, 1, 4, 1u, 0u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.office_workstation", "arch.homelab_cad_workstation", "room.office_homelab", "", 13, 1, 3, 0u, 120u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.office_bookshelf", "arch.reference_archive_shelf", "room.office_homelab", "", 14, 1, 3, 0u, 44u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.living_couch", "arch.living_room_couch", "room.living_space", "", 10, 1, 7, 0u, 48u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.bedroom_bed", "arch.insulated_bed_mattress", "room.bedroom", "", 15, 1, 9, 0u, 23u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.garage_workbench", "arch.woodshop_joinery_workbench", "room.attached_garage", "", 9, 1, 13, 0u, 54u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.garage_material_rack", "arch.garage_lumber_and_seed_rack", "room.attached_garage", "item.cedar_plank", 11, 1, 13, 14u, 56u, false});
        out_entities.push_back(PlacedEntitySeed{"tool.galvanized_bucket", "arch.hauling_tool_bucket", "room.attached_garage", "bucket", 9, 1, 12, 1u, 0u, false});
        out_entities.push_back(PlacedEntitySeed{"tool.watering_can", "arch.hauling_tool_watering_can", "room.attached_garage", "watering_can", 10, 1, 12, 1u, 0u, false});
        out_entities.push_back(PlacedEntitySeed{"tool.wheelbarrow", "arch.hauling_tool_wheelbarrow", "room.attached_garage", "wheelbarrow", 11, 1, 12, 1u, 0u, false});
        out_entities.push_back(PlacedEntitySeed{"tool.hand_dolly", "arch.hauling_tool_hand_dolly", "room.attached_garage", "hand_dolly", 11, 1, 11, 1u, 0u, false});
        out_entities.push_back(PlacedEntitySeed{"entity.exterior_hose_spigot", "arch.exterior_water_spigot", "zone.fenced_backyard", "", 18, 1, 5, 0u, 0u, false});
        return true;
    }

private:
    int32_t lot_number_{402};
    int32_t reserved_cfg_{0};
};

} // namespace oasis::engine
