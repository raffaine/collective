#include <catch2/catch_test_macros.hpp>
#include "../core/chunk_manager.hpp"
#include "../core/crdt_sync_engine.hpp"
#include "../core/thermodynamic_engine.hpp"
#include "../core/bpmn_parser.hpp"

using namespace oasis;

class MockActuator : public IActuator {
public:
    bool actuated = false;
    std::string last_task;
    float last_value = 0.0f;

    bool Actuate(const std::string& task_id, float value) override {
        actuated = true;
        last_task = task_id;
        last_value = value;
        return true;
    }
};

TEST_CASE("Story 2.1 & 2.5: IChunkObserver and CRDTSyncEngine", "[subsystem][crdt]") {
    ChunkManager chunk;
    chunk.InitializeHeadless();
    CRDTSyncEngine crdt;
    chunk.AddObserver(&crdt);
    
    REQUIRE(crdt.GetPendingDeltaCount() == 0);
    Voxel test_voxel{15, 255, 100, 0b10101010};
    chunk.SetVoxel(0, 0, 0, test_voxel);
    REQUIRE(crdt.GetPendingDeltaCount() == 1);
    
    chunk.SetVoxel(1, 1, 1, test_voxel);
    REQUIRE(crdt.GetPendingDeltaCount() == 2);
    crdt.Flush();
    REQUIRE(crdt.GetPendingDeltaCount() == 0);
}

TEST_CASE("Story 2.2: Thermodynamic Engine Tick Physics", "[subsystem][physics]") {
    ChunkManager chunk;
    chunk.InitializeHeadless();
    ThermodynamicEngine thermo(chunk);
    
    SovereignBoundary boundary;
    boundary.radius = 100; // Big enough to encompass everything deterministically
    
    Voxel v{1, 100, 240, 0};
    chunk.SetVoxel(16, 16, 16, v);
    
    thermo.Tick(boundary);
    
    Voxel updated = chunk.GetVoxel(16, 16, 16);
    REQUIRE(updated.temperature == 239);
    REQUIRE(updated.moisture == 99);
    
    Voxel v2{1, 0, 10, 0};
    chunk.SetVoxel(5, 5, 5, v2);
    thermo.Tick(boundary);
    
    Voxel updated2 = chunk.GetVoxel(5, 5, 5);
    REQUIRE(updated2.temperature == 11);
}

TEST_CASE("Story 2.3 & 2.4: BPMN Parser and IActuator", "[subsystem][bpmn]") {
    MockActuator actuator;
    BPMNParser parser(&actuator);
    
    std::string xml = R"(
        <?xml version="1.0"?>
        <definitions>
            <bpmn2:task id="PumpWater" value="42.5" />
        </definitions>
    )";
    
    bool result = parser.ParseAndExecute(xml);
    REQUIRE(result == true);
    REQUIRE(actuator.actuated == true);
    REQUIRE(actuator.last_task == "PumpWater");
    REQUIRE(actuator.last_value == 42.5f);
}
