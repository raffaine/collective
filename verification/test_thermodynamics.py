#!/usr/bin/env python3
"""
Verification Suite: Thermodynamic Equations, Exergy Minting & Battery Brownout
Target: SOVEREIGN_STACK_BACKLOG.md (Story 1.2, Story 1.5, Story 3.3, Story 5.2)
"""

import math
import sys
import ctypes

def test_erc_asymptote_singularity():
    print("=== [TEST 1] Ecological Replacement Cost (lambda_ERC) Asymptote & Sign Inversion ===")
    lambda_base = 1.0
    r_capacity = 100000.0  # liters
    gamma = 2.0

    def calc_erc(r_current):
        ratio = r_current / r_capacity
        denom = 1.0 - (ratio ** gamma)
        if denom == 0.0:
            return float('inf')
        return lambda_base * (1.0 / denom)

    # Test near capacity
    test_points = [
        0.0,
        50000.0,
        95000.0,
        99000.0,
        99990.0,
        100000.0,      # Exact capacity
        100001.0,      # Slight overshoot (0.001%)
        105000.0,      # 5% overshoot
    ]

    print(f"{'R_current':<12} | {'Ratio':<8} | {'Denominator':<14} | {'lambda_ERC':<16} | {'Status'}")
    print("-" * 65)

    sign_inversion_found = False
    for r in test_points:
        ratio = r / r_capacity
        denom = 1.0 - (ratio ** gamma)
        erc = calc_erc(r)
        
        status = "NORMAL"
        if math.isinf(erc):
            status = "SINGULARITY (DIV/0)"
        elif erc < 0:
            status = "CRITICAL BUG: NEGATIVE COST INVERSION"
            sign_inversion_found = True
        elif erc > 100:
            status = "HIGH THROTTLE"
            
        print(f"{r:<12.1f} | {ratio:<8.4f} | {denom:<14.6e} | {erc:<16.4f} | {status}")

    print(f"\nResult: Sign Inversion Bug Confirmed: {sign_inversion_found}")
    print("Analysis: When resource extraction overshoots carrying capacity (due to discrete simulation steps or sensor lag),")
    print("the denominator (1 - (R/R_cap)^gamma) flips to negative, transforming the cost multiplier from +infinity to NEGATIVE!")
    print("This subsidizes/incentivizes catastrophic biome destruction instead of stopping it.")
    return sign_inversion_found


def test_exergy_minting_underflow():
    print("\n=== [TEST 2] Exergy Integral Minting Underflow During Zero Irradiance ===")
    # Story 3.3 equation:
    # Delta V = integral (Phi_in(t) - Phi_out(t)) * lambda_ERC(t) dt
    # C++ interface: uint32_t ComputeMintableTokens(const ExergyTelemetry& telem, float carrying_capacity_ratio);

    # Zero solar irradiance case (extended night or brownout storm)
    phi_in = 0.0           # Joules harvested (zero solar)
    phi_out = 15000.0      # Joules dissipated (critical node compute & life support)
    lambda_erc = 1.0
    dt = 3600.0            # 1 hour tick window

    delta_v_real = (phi_in - phi_out) * lambda_erc * (dt / 3600.0) # e.g. -15000 units
    print(f"Phi_in: {phi_in} W, Phi_out: {phi_out} W")
    print(f"Calculated Delta V (floating point): {delta_v_real}")

    # Simulated C++ cast to uint32_t as defined in IValuenomicsEngine:
    # uint32_t ComputeMintableTokens(...)
    raw_int = int(delta_v_real)
    uint32_cast = ctypes.c_uint32(raw_int).value

    print(f"C++ uint32_t cast: (uint32_t)({raw_int}) = {uint32_cast} (0x{uint32_cast:08X})")

    underflow_bug = (uint32_cast > 1_000_000_000)
    print(f"Result: Unsigned Underflow Minting Exploit Confirmed: {underflow_bug}")
    print(f"Analysis: Under zero solar irradiance, a steward with negative exergy is credited {uint32_cast:,} tokens")
    print("due to uint32_t return type in IValuenomicsEngine! Instant hyperinflation exploit.")
    return underflow_bug


def test_battery_voltage_specification_contradiction():
    print("\n=== [TEST 3] Battery Pack Voltage Specification Contradiction ===")
    # Story 1.2: 16 prismatic LiFePO4 cells
    # Story 1.5: Brownout threshold V_bus <= 10.8V

    num_cells_story_1_2 = 16
    v_nominal_cell = 3.2   # V
    v_empty_cell = 2.5     # V (Standard manufacturer discharge cutoff, e.g. EVE, CATL)
    v_damage_cell = 2.0    # V (Irreversible damage / copper dissolution threshold)
    
    pack_nominal_voltage = num_cells_story_1_2 * v_nominal_cell # 51.2V
    pack_empty_voltage = num_cells_story_1_2 * v_empty_cell     # 40.0V

    brownout_threshold_v_bus = 10.8 # Specified in Story 1.5

    cell_voltage_at_brownout = brownout_threshold_v_bus / num_cells_story_1_2

    print(f"Specified Battery Chemistry: LiFePO4")
    print(f"Specified Cell Configuration (Story 1.2): {num_cells_story_1_2}S (16 cells in series)")
    print(f"Nominal 16S Pack Voltage: {pack_nominal_voltage:.1f} V")
    print(f"Standard 16S Pack Empty Cutoff (2.5V/cell): {pack_empty_voltage:.1f} V")
    print(f"Specified Brownout Threshold (Story 1.5): {brownout_threshold_v_bus:.1f} V")
    print(f"Calculated Per-Cell Voltage at Brownout: {cell_voltage_at_brownout:.3f} V / cell")
    print(f"Damage Threshold: < {v_damage_cell:.1f} V")

    fatal_mismatch = cell_voltage_at_brownout < v_damage_cell
    print(f"Result: Fatal Voltage Contradiction Confirmed: {fatal_mismatch}")
    print(f"Analysis: At 10.8V total bus voltage, each cell in a 16S pack is at {cell_voltage_at_brownout:.3f}V.")
    print("LiFePO4 cells undergo irreversible copper dissolution and internal short circuiting below 2.0V.")
    print("The 10.8V brownout trigger is from a 12V (4S) architecture, creating an irreconcilable conflict with 16S (48V).")
    return fatal_mismatch


def test_72h_zero_irradiance_battery_drain():
    print("\n=== [TEST 4] 72-Hour Zero-Irradiance Battery Drainage & Brownout Timeline ===")
    # Simulation of edge node compute and critical loads during 72h zero-solar storm
    # Test two tiers:
    # Tier A: Field node with small battery (12V 20Ah = 240 Wh or 24V 20Ah = 480 Wh)
    # Tier B: Lot 402 Hub node with 15 kWh battery pack (Story 5.4)

    # Loads:
    # Compute: CM4 (4W) + RS485/CAN/LoRa/Sensors (1.5W) + DC-DC conversion efficiency (85%) = ~6.5W
    # Inverter Tare / Standby loss: 12W
    # Baseline load without HVAC/heavy appliances: 50W (monitoring, emergency valves, mesh relay)
    total_hub_power = 6.5 + 12.0 + 50.0 # 68.5 W continuous
    field_node_power = 4.5 # 4.5 W continuous for ESP32 + sensors + LoRa + DC-DC

    field_battery_wh = 240.0 # 12V 20Ah
    hub_battery_wh = 15000.0 # 15 kWh

    hours = 72
    field_energy_consumed = field_node_power * hours
    hub_energy_consumed = total_hub_power * hours

    print(f"Zero Irradiance Duration: {hours} hours")
    print(f"Field Node Continuous Draw: {field_node_power} W | Total Required: {field_energy_consumed:.1f} Wh")
    print(f"Field Node Battery Capacity: {field_battery_wh} Wh")
    field_runtime_hours = field_battery_wh / field_node_power
    field_survives = field_runtime_hours >= hours
    print(f"Field Node Runtime until Brownout: {field_runtime_hours:.1f} hours -> Survives 72h: {field_survives}")

    print(f"\nHub Node Continuous Critical Draw: {total_hub_power} W | Total Required: {hub_energy_consumed:.1f} Wh")
    print(f"Hub Node 15 kWh Pack Energy Consumed: {hub_energy_consumed / 1000.0:.2f} kWh ({(hub_energy_consumed / hub_battery_wh)*100:.1f}% DoD)")
    
    # What if communal heating/water pump (500W) is required in storm?
    harsh_storm_hub_power = total_hub_power + 450.0 # ~518.5 W
    harsh_energy_consumed = harsh_storm_hub_power * hours
    hub_harsh_survives = (harsh_energy_consumed <= hub_battery_wh)
    print(f"Hub Node in Harsh Storm (518.5 W critical life-support): {harsh_energy_consumed / 1000.0:.2f} kWh consumed -> Survives 72h: {hub_harsh_survives}")

    # Brownout recovery hysteresis test
    print("\nBrownout Recovery Hysteresis Check:")
    print("Backlog specifies: 'triggers a brownout fail-safe when V_bus <= 10.8 V, flushing CRDT deltas to disk'")
    print("Does backlog specify a recovery threshold V_recover (e.g. 12.4V or 13.0V)? NO.")
    print("Analysis: Without hysteresis (Schmitt trigger logic), load shedding at 10.8V causes battery open-circuit")
    print("voltage to instantly bounce back to ~11.5V, clearing brownout, turning loads back on, collapsing voltage again,")
    print("and generating rapid-cycling relay chatter / continuous emergency flush loops until flash wearout!")
    
    return True


if __name__ == "__main__":
    t1 = test_erc_asymptote_singularity()
    t2 = test_exergy_minting_underflow()
    t3 = test_battery_voltage_specification_contradiction()
    t4 = test_72h_zero_irradiance_battery_drain()
    print("\n=== THERMODYNAMIC SUITE EXECUTION COMPLETE ===")
