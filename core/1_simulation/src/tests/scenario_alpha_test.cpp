#include <cassert>
#include <iostream>
#include <string>
#include "../core/chunk_manager.hpp"

// Simple JSON parser mock for intent
bool parse_intent(const std::string& intent, float& out_mass) {
    if (intent.find("PETG") != std::string::npos) {
        out_mass = 42.5f; // Hardcoded mock for test
        return true;
    }
    return false;
}

void test_scenario_alpha_fabrication() {
    using namespace oasis;
    std::cout << "Running Alpha Scenario Test (Fabrication Commons)...\n";

    ChunkManager chunk_mgr;
    chunk_mgr.InitializeHeadless();
    
    // 1. Setup Printer Voxel (16, 8, 16)
    Voxel printer_voxel;
    printer_voxel.material_id = 15; // FAB_NODE
    printer_voxel.moisture = 5;
    printer_voxel.temperature = 22; // Ambient
    printer_voxel.metadata = 0b00000110; // Actuator + Sensor
    chunk_mgr.SetVoxel(16, 8, 16, printer_voxel);

    // 2. Setup Hopper Voxel (15, 8, 16) - Using moisture as inventory scale (1 unit = 10g)
    Voxel hopper_voxel;
    hopper_voxel.material_id = 16; // POLYMER_SPOOL
    hopper_voxel.moisture = 100;   // 1000g
    hopper_voxel.temperature = 22;
    hopper_voxel.metadata = 0;
    chunk_mgr.SetVoxel(15, 8, 16, hopper_voxel);

    // 3. Setup Locker Voxel (16, 9, 16)
    Voxel locker_voxel;
    locker_voxel.material_id = 0; // Air
    chunk_mgr.SetVoxel(16, 9, 16, locker_voxel);

    // 4. Inject Intent (Layer 4 Handoff)
    std::string json_payload = "{\"job\": \"PETG_Valve\", \"mass_g\": 42.5}";
    bool intent_accepted = chunk_mgr.InjectIntent(json_payload.c_str());
    assert(intent_accepted == true);

    // 5. Run Thermodynamic Simulation (4473 ticks/seconds)
    for(int i=0; i<4473; ++i) {
        chunk_mgr.Tick();
    }

    // 6. Assertions (G1, G4 Physical Realities)
    Voxel final_printer = chunk_mgr.GetVoxel(16, 8, 16);
    Voxel final_hopper = chunk_mgr.GetVoxel(15, 8, 16);
    Voxel final_locker = chunk_mgr.GetVoxel(16, 9, 16);

    // Temp should have reached print temp (~240C) then perhaps cooled or stabilized, 
    // but definitely higher than ambient 22C due to Exergy expenditure.
    assert(final_printer.temperature > 50);

    // Hopper should be depleted by ~42.5g (4 units)
    assert(final_hopper.moisture <= 96); 
    assert(final_hopper.moisture >= 95);

    // Locker should be secured (material 17)
    assert(final_locker.material_id == 17);

    std::cout << "Alpha Scenario Test PASSED!\n";
}

int main() {
    test_scenario_alpha_fabrication();
    return 0;
}
