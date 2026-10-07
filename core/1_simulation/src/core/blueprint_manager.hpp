#pragma once
#include <vector>
#include <cmath>

namespace oasis {

enum class ZoneType {
    SERVICE_KIOSK = 0, // Layer 7 Fiat Vending (Water, Wi-Fi)
    SOLAR_MESH = 1,
    GRAYWATER_ROUTING = 2
};

struct Zone {
    int x, y, z;
    int w, h, d;
    ZoneType type;
};

// Story 6.1: Blueprint Engine for macroscopic zoning
class BlueprintManager {
public:
    void AddZone(const Zone& zone);
    const std::vector<Zone>& GetZones() const;

    // Returns true if a zone of the specified type exists, mapping the nearest coordinates
    bool FindNearest(ZoneType type, int x, int y, int z, int& out_x, int& out_y, int& out_z) const;

private:
    std::vector<Zone> zones_;
};

} // namespace oasis
