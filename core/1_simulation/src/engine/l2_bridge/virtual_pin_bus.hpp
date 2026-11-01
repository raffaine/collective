#pragma once

#include <algorithm>
#include <cstdint>
#include <string_view>
#include <vector>

#include "../domains/soil_physics.hpp"
#include "../domains/thermal_vessel.hpp"
#include "../ecs/affordance_system.hpp"

namespace oasis::engine {

inline constexpr uint32_t kDefaultIrrigationFlowMlPerSec = 45u; // 2.7 L/min drip manifold flow rate

enum class PinAccessStatus : uint8_t {
    OK                          = 0,
    DENIED_NO_L2_SENSOR         = 1,
    DENIED_ACTUATOR_DISABLED    = 2,
    ENDPOINT_NOT_FOUND          = 3,
    UNKNOWN_PIN_REGISTER        = 4
};

struct alignas(8) SoilPinReadResult {
    uint64_t          entity_uid{0ULL};
    SoilCellRegisters registers{};
    PinAccessStatus   status{PinAccessStatus::DENIED_NO_L2_SENSOR};
    bool              irrigation_valve_open{false};
    uint8_t           pad_[6]{0u, 0u, 0u, 0u, 0u, 0u};
};
static_assert(sizeof(SoilPinReadResult) == 56, "SoilPinReadResult must be 56 bytes with zero implicit padding");

struct alignas(8) ThermalPinReadResult {
    uint64_t               entity_uid{0ULL};
    ThermalVesselRegisters registers{};
    PinAccessStatus        status{PinAccessStatus::DENIED_NO_L2_SENSOR};
    uint8_t                pad_[7]{0u, 0u, 0u, 0u, 0u, 0u, 0u};
};
static_assert(sizeof(ThermalPinReadResult) == 48, "ThermalPinReadResult must be 48 bytes with zero implicit padding");

struct alignas(8) SoilBusEndpoint {
    uint64_t                         entity_uid{0ULL};
    const SensorAttachmentComponent* sensor{nullptr};
    SoilCellRegisters*               soil{nullptr};
    uint32_t                         valve_flow_ml_per_s{kDefaultIrrigationFlowMlPerSec};
    bool                             irrigation_valve_open{false};
    uint8_t                          pad_[3]{0u, 0u, 0u};
};

struct alignas(8) ThermalBusEndpoint {
    uint64_t                         entity_uid{0ULL};
    const SensorAttachmentComponent* sensor{nullptr};
    ThermalVesselRegisters*          vessel{nullptr};
};

// Enforces the Layer 1b <-> Layer 2 UHAI epistemic privacy & hardware actuation boundary.
// Physical registers are readable by L2 ONLY when SensorAttachmentComponent.has_l2_sensor == true.
class VirtualPinBus {
public:
    // --- Direct component-level L2 sensor reads (epistemic privacy gate) ---

    [[nodiscard]] static constexpr PinAccessStatus ReadSoilRegisters(
        const SensorAttachmentComponent& sensor,
        const SoilCellRegisters& physical_soil,
        SoilCellRegisters& out_telemetry
    ) noexcept {
        if (!sensor.has_l2_sensor) {
            out_telemetry = SoilCellRegisters{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
            return PinAccessStatus::DENIED_NO_L2_SENSOR;
        }
        out_telemetry = physical_soil;
        return PinAccessStatus::OK;
    }

    [[nodiscard]] static constexpr PinAccessStatus ReadThermalRegisters(
        const SensorAttachmentComponent& sensor,
        const ThermalVesselRegisters& physical_vessel,
        ThermalVesselRegisters& out_telemetry
    ) noexcept {
        if (!sensor.has_l2_sensor) {
            out_telemetry = ThermalVesselRegisters{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0u};
            return PinAccessStatus::DENIED_NO_L2_SENSOR;
        }
        out_telemetry = physical_vessel;
        return PinAccessStatus::OK;
    }

    [[nodiscard]] static constexpr PinAccessStatus ReadNamedRegisterU32(
        const SensorAttachmentComponent& sensor,
        const SoilCellRegisters* soil,
        const ThermalVesselRegisters* vessel,
        std::string_view pin_name,
        uint32_t& out_value
    ) noexcept {
        out_value = 0u;
        if (!sensor.has_l2_sensor) {
            return PinAccessStatus::DENIED_NO_L2_SENSOR;
        }
        if (soil != nullptr) {
            if (pin_name == "moisture_vwc_bp")    { out_value = soil->moisture_vwc_bp; return PinAccessStatus::OK; }
            if (pin_name == "soil_temp_mk")       { out_value = soil->soil_temp_mk; return PinAccessStatus::OK; }
            if (pin_name == "nitrogen_ppm")       { out_value = soil->nitrogen_ppm; return PinAccessStatus::OK; }
            if (pin_name == "phosphorus_ppm")     { out_value = soil->phosphorus_ppm; return PinAccessStatus::OK; }
            if (pin_name == "potassium_ppm")      { out_value = soil->potassium_ppm; return PinAccessStatus::OK; }
            if (pin_name == "organic_carbon_bp")  { out_value = soil->organic_carbon_bp; return PinAccessStatus::OK; }
            if (pin_name == "ph_centi")           { out_value = soil->ph_centi; return PinAccessStatus::OK; }
            if (pin_name == "crop_growth_bp")     { out_value = soil->crop_growth_bp; return PinAccessStatus::OK; }
            if (pin_name == "is_legume_fixing_n") { out_value = soil->is_legume_fixing_n; return PinAccessStatus::OK; }
        }
        if (vessel != nullptr) {
            if (pin_name == "core_temp_mk")           { out_value = vessel->core_temp_mk; return PinAccessStatus::OK; }
            if (pin_name == "heater_power_mw")        { out_value = vessel->heater_power_mw; return PinAccessStatus::OK; }
            if (pin_name == "liquid_volume_ml")       { out_value = vessel->liquid_volume_ml; return PinAccessStatus::OK; }
            if (pin_name == "pasteurization_hold_s")  { out_value = vessel->pasteurization_hold_s; return PinAccessStatus::OK; }
            if (pin_name == "target_cutoff_temp_mk")  { out_value = vessel->target_cutoff_temp_mk; return PinAccessStatus::OK; }
            if (pin_name == "lid_closed")             { out_value = vessel->lid_closed; return PinAccessStatus::OK; }
        }
        return PinAccessStatus::UNKNOWN_PIN_REGISTER;
    }

    // --- Direct component-level L2 actuator writes ---

    [[nodiscard]] static constexpr PinAccessStatus WriteHeaterPowerPin(
        const SensorAttachmentComponent& sensor,
        ThermalVesselRegisters& physical_vessel,
        uint32_t heater_power_mw
    ) noexcept {
        if (!sensor.has_l2_sensor) {
            return PinAccessStatus::DENIED_NO_L2_SENSOR;
        }
        if (!sensor.actuator_enabled) {
            return PinAccessStatus::DENIED_ACTUATOR_DISABLED;
        }
        physical_vessel.heater_power_mw = heater_power_mw;
        return PinAccessStatus::OK;
    }

    [[nodiscard]] static constexpr PinAccessStatus WriteIrrigationValvePin(
        const SensorAttachmentComponent& sensor,
        bool irrigation_valve_open,
        bool& out_valve_state
    ) noexcept {
        if (!sensor.has_l2_sensor) {
            return PinAccessStatus::DENIED_NO_L2_SENSOR;
        }
        if (!sensor.actuator_enabled) {
            return PinAccessStatus::DENIED_ACTUATOR_DISABLED;
        }
        out_valve_state = irrigation_valve_open;
        return PinAccessStatus::OK;
    }

    // --- Endpoint-routed virtual pin bus registry ---

    void BindSoilEntity(
        uint64_t entity_uid,
        const SensorAttachmentComponent* sensor,
        SoilCellRegisters* soil,
        uint32_t valve_flow_ml_per_s = kDefaultIrrigationFlowMlPerSec
    ) {
        for (auto& ep : soil_endpoints_) {
            if (ep.entity_uid == entity_uid) {
                ep.sensor = sensor;
                ep.soil = soil;
                ep.valve_flow_ml_per_s = valve_flow_ml_per_s;
                return;
            }
        }
        soil_endpoints_.push_back(SoilBusEndpoint{entity_uid, sensor, soil, valve_flow_ml_per_s, false});
    }

    void BindThermalEntity(
        uint64_t entity_uid,
        const SensorAttachmentComponent* sensor,
        ThermalVesselRegisters* vessel
    ) {
        for (auto& ep : thermal_endpoints_) {
            if (ep.entity_uid == entity_uid) {
                ep.sensor = sensor;
                ep.vessel = vessel;
                return;
            }
        }
        thermal_endpoints_.push_back(ThermalBusEndpoint{entity_uid, sensor, vessel});
    }

    [[nodiscard]] SoilPinReadResult ReadSoilEntity(uint64_t entity_uid) const noexcept {
        SoilPinReadResult out{};
        out.entity_uid = entity_uid;
        out.registers  = SoilCellRegisters{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
        for (const auto& ep : soil_endpoints_) {
            if (ep.entity_uid == entity_uid) {
                if (ep.sensor == nullptr || ep.soil == nullptr) {
                    out.status = PinAccessStatus::DENIED_NO_L2_SENSOR;
                    return out;
                }
                out.status = ReadSoilRegisters(*ep.sensor, *ep.soil, out.registers);
                if (out.status == PinAccessStatus::OK) {
                    out.irrigation_valve_open = ep.irrigation_valve_open;
                }
                return out;
            }
        }
        out.status = PinAccessStatus::ENDPOINT_NOT_FOUND;
        return out;
    }

    [[nodiscard]] ThermalPinReadResult ReadThermalEntity(uint64_t entity_uid) const noexcept {
        ThermalPinReadResult out{};
        out.entity_uid = entity_uid;
        out.registers  = ThermalVesselRegisters{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0, 0u};
        for (const auto& ep : thermal_endpoints_) {
            if (ep.entity_uid == entity_uid) {
                if (ep.sensor == nullptr || ep.vessel == nullptr) {
                    out.status = PinAccessStatus::DENIED_NO_L2_SENSOR;
                    return out;
                }
                out.status = ReadThermalRegisters(*ep.sensor, *ep.vessel, out.registers);
                return out;
            }
        }
        out.status = PinAccessStatus::ENDPOINT_NOT_FOUND;
        return out;
    }

    PinAccessStatus WriteEntityIrrigationValve(uint64_t entity_uid, bool irrigation_valve_open) noexcept {
        for (auto& ep : soil_endpoints_) {
            if (ep.entity_uid == entity_uid) {
                if (ep.sensor == nullptr || ep.soil == nullptr) {
                    return PinAccessStatus::DENIED_NO_L2_SENSOR;
                }
                return WriteIrrigationValvePin(*ep.sensor, irrigation_valve_open, ep.irrigation_valve_open);
            }
        }
        return PinAccessStatus::ENDPOINT_NOT_FOUND;
    }

    PinAccessStatus WriteEntityHeaterPower(uint64_t entity_uid, uint32_t heater_power_mw) noexcept {
        for (auto& ep : thermal_endpoints_) {
            if (ep.entity_uid == entity_uid) {
                if (ep.sensor == nullptr || ep.vessel == nullptr) {
                    return PinAccessStatus::DENIED_NO_L2_SENSOR;
                }
                return WriteHeaterPowerPin(*ep.sensor, *ep.vessel, heater_power_mw);
            }
        }
        return PinAccessStatus::ENDPOINT_NOT_FOUND;
    }

    [[nodiscard]] bool IsIrrigationValveOpen(uint64_t entity_uid) const noexcept {
        for (const auto& ep : soil_endpoints_) {
            if (ep.entity_uid == entity_uid) {
                return ep.irrigation_valve_open;
            }
        }
        return false;
    }

    // Steps all bound physical domains forward by sim_dt_s, injecting water from open L2 irrigation valves.
    void StepBoundDomains(uint32_t ambient_temp_mk, uint32_t sim_dt_s) noexcept {
        for (auto& ep : soil_endpoints_) {
            if (ep.soil != nullptr) {
                const uint32_t water_ml = ep.irrigation_valve_open
                    ? (ep.valve_flow_ml_per_s * sim_dt_s)
                    : 0u;
                StepSoilPhysics(*ep.soil, ambient_temp_mk, water_ml, sim_dt_s);
            }
        }
        for (auto& ep : thermal_endpoints_) {
            if (ep.vessel != nullptr) {
                StepThermalVessel(*ep.vessel, ambient_temp_mk, sim_dt_s);
            }
        }
    }

private:
    std::vector<SoilBusEndpoint>    soil_endpoints_{};
    std::vector<ThermalBusEndpoint> thermal_endpoints_{};
};

} // namespace oasis::engine
