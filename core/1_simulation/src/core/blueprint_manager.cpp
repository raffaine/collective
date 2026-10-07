#include "blueprint_manager.hpp"

namespace oasis {

void BlueprintManager::AddZone(const Zone& zone) {
    zones_.push_back(zone);
}

const std::vector<Zone>& BlueprintManager::GetZones() const {
    return zones_;
}

bool BlueprintManager::FindNearest(ZoneType type, int x, int y, int z, int& out_x, int& out_y, int& out_z) const {
    bool found = false;
    float min_dist = 999999.0f;

    for (const auto& zone : zones_) {
        if (zone.type == type) {
            // Find center of the zone
            int cx = zone.x + (zone.w / 2);
            int cy = zone.y + (zone.h / 2);
            int cz = zone.z + (zone.d / 2);

            float dx = static_cast<float>(cx - x);
            float dy = static_cast<float>(cy - y);
            float dz = static_cast<float>(cz - z);
            float dist = std::sqrt(dx*dx + dy*dy + dz*dz);

            if (dist < min_dist) {
                min_dist = dist;
                out_x = cx;
                out_y = cy;
                out_z = cz;
                found = true;
            }
        }
    }
    return found;
}

} // namespace oasis
