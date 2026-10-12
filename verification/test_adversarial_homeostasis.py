#!/usr/bin/env python3
"""
Adversarial Stress Harness for Homeostasis Math & Diurnal Dynamics
Challenger 1 (Homeostasis Math Stress Verifier)
"""

import math
import json
import random
import sys

GOLDILOCKS_MIN_TEMP_C = 15.0
GOLDILOCKS_MAX_TEMP_C = 30.0
MEAN_AMBIENT_TEMP_C = 22.5
DIURNAL_AMPLITUDE_C = 12.5
DIURNAL_CYCLE_TICKS = 1200

BASELINE_RESTING_BURN_RATE = 0.050
COLD_BURN_COEFF = 0.080
HOT_BURN_COEFF = 0.060

COLD_FOCUS_DRAIN_PER_SEC = 0.0020
HOT_FOCUS_DRAIN_PER_SEC = 0.0015

def calculate_diurnal_temp(sim_tick, cycle_period=DIURNAL_CYCLE_TICKS):
    if cycle_period == 0:
        cycle_period = DIURNAL_CYCLE_TICKS
    phase = (sim_tick % cycle_period) / float(cycle_period)
    k_two_pi = 6.28318530717958647692
    angle = k_two_pi * phase
    return MEAN_AMBIENT_TEMP_C + DIURNAL_AMPLITUDE_C * math.sin(angle)

def is_sheltered(x, y, z):
    abs_x = abs(int(math.floor(x)))
    lot_z = ((int(math.floor(z)) % 16) + 16) % 16
    vy = int(math.floor(y))
    return (9 <= abs_x <= 17) and (2 <= lot_z <= 10) and (1 <= vy <= 3)

def resolve_ambient_temp(x, y, z, sim_tick, cycle_period=DIURNAL_CYCLE_TICKS):
    base = calculate_diurnal_temp(sim_tick, cycle_period)
    if is_sheltered(x, y, z):
        base = 22.0 + 0.3 * (base - 22.0)
    return base

def evaluate_homeostasis(temp_c):
    if math.isnan(temp_c):
        return {
            "burn_multiplier": 1.0,
            "focus_drain_rate": 0.0,
            "in_goldilocks": True,
            "is_cold_stress": False,
            "is_heat_stress": False
        }
    if temp_c < GOLDILOCKS_MIN_TEMP_C:
        delta = GOLDILOCKS_MIN_TEMP_C - temp_c
        return {
            "burn_multiplier": 1.0 + COLD_BURN_COEFF * delta,
            "focus_drain_rate": COLD_FOCUS_DRAIN_PER_SEC * delta,
            "in_goldilocks": False,
            "is_cold_stress": True,
            "is_heat_stress": False
        }
    elif temp_c > GOLDILOCKS_MAX_TEMP_C:
        delta = temp_c - GOLDILOCKS_MAX_TEMP_C
        return {
            "burn_multiplier": 1.0 + HOT_BURN_COEFF * delta,
            "focus_drain_rate": HOT_FOCUS_DRAIN_PER_SEC * delta,
            "in_goldilocks": False,
            "is_cold_stress": False,
            "is_heat_stress": True
        }
    else:
        return {
            "burn_multiplier": 1.0,
            "focus_drain_rate": 0.0,
            "in_goldilocks": True,
            "is_cold_stress": False,
            "is_heat_stress": False
        }

def step_homeostasis(somatic, psych, ambient_temp_c, dt, base_burn_rate=BASELINE_RESTING_BURN_RATE):
    somatic["ambient_temp_c"] = ambient_temp_c
    eval_res = evaluate_homeostasis(ambient_temp_c)
    effective_burn = base_burn_rate * eval_res["burn_multiplier"]
    cal_delta = effective_burn * dt
    somatic["caloric_reserve"] = max(0.0, somatic["caloric_reserve"] - cal_delta)

    if eval_res["focus_drain_rate"] > 0.0:
        focus_delta = eval_res["focus_drain_rate"] * dt
        psych["focus"] = max(0.0, psych["focus"] - focus_delta)
        somatic["focus"] = psych["focus"]

    if somatic["caloric_reserve"] <= 500.0 or psych["focus"] <= 0.05:
        somatic["is_exhausted"] = True

def run_adversarial_validation():
    results = {}
    
    # 1. Boundary Temperatures
    b14_999 = evaluate_homeostasis(14.999)
    b15_000 = evaluate_homeostasis(15.000)
    b15_001 = evaluate_homeostasis(15.001)
    b29_999 = evaluate_homeostasis(29.999)
    b30_000 = evaluate_homeostasis(30.000)
    b30_001 = evaluate_homeostasis(30.001)

    results["boundary_tests"] = {
        "14_999_is_cold": b14_999["is_cold_stress"] and not b14_999["in_goldilocks"],
        "14_999_multiplier": round(b14_999["burn_multiplier"], 6),
        "15_000_in_goldilocks": b15_000["in_goldilocks"] and b15_000["burn_multiplier"] == 1.0,
        "15_001_in_goldilocks": b15_001["in_goldilocks"] and b15_001["burn_multiplier"] == 1.0,
        "29_999_in_goldilocks": b29_999["in_goldilocks"] and b29_999["burn_multiplier"] == 1.0,
        "30_000_in_goldilocks": b30_000["in_goldilocks"] and b30_000["burn_multiplier"] == 1.0,
        "30_001_is_hot": b30_001["is_heat_stress"] and not b30_001["in_goldilocks"],
        "30_001_multiplier": round(b30_001["burn_multiplier"], 6),
        "jump_at_15_c": abs(b14_999["burn_multiplier"] - b15_000["burn_multiplier"]) < 1e-3,
        "jump_at_30_c": abs(b30_001["burn_multiplier"] - b30_000["burn_multiplier"]) < 1e-3,
    }

    # 2. Extreme Temperatures
    e_neg50 = evaluate_homeostasis(-50.0)
    e_abszero = evaluate_homeostasis(-273.15)
    e_pos60 = evaluate_homeostasis(60.0)
    e_pos100 = evaluate_homeostasis(100.0)

    results["extreme_temperature_tests"] = {
        "neg50_burn_multiplier": round(e_neg50["burn_multiplier"], 4),
        "neg50_focus_drain_rate": round(e_neg50["focus_drain_rate"], 4),
        "abszero_burn_multiplier": round(e_abszero["burn_multiplier"], 4),
        "abszero_focus_drain_rate": round(e_abszero["focus_drain_rate"], 4),
        "pos60_burn_multiplier": round(e_pos60["burn_multiplier"], 4),
        "pos60_focus_drain_rate": round(e_pos60["focus_drain_rate"], 4),
        "pos100_burn_multiplier": round(e_pos100["burn_multiplier"], 4),
        "pos100_focus_drain_rate": round(e_pos100["focus_drain_rate"], 4),
        "all_multipliers_positive": all(e["burn_multiplier"] > 0 for e in [e_neg50, e_abszero, e_pos60, e_pos100]),
        "all_focus_drains_positive": all(e["focus_drain_rate"] > 0 for e in [e_neg50, e_abszero, e_pos60, e_pos100]),
    }

    # 3. Diurnal Cycle Stability across 1,000,000 ticks
    ticks_1m = 1000000
    temps = [calculate_diurnal_temp(t) for t in range(0, ticks_1m, 100)]
    results["diurnal_cycle_tests"] = {
        "sampled_ticks": len(temps),
        "min_temp": round(min(temps), 4),
        "max_temp": round(max(temps), 4),
        "mean_temp": round(sum(temps) / len(temps), 4),
        "bounded_in_10_to_35": all(10.0 - 1e-4 <= t <= 35.0 + 1e-4 for t in temps),
        "dawn_tick_0": round(calculate_diurnal_temp(0), 4),
        "noon_tick_300": round(calculate_diurnal_temp(300), 4),
        "dusk_tick_600": round(calculate_diurnal_temp(600), 4),
        "midnight_tick_900": round(calculate_diurnal_temp(900), 4),
    }

    # 4. Caloric Exhaustion and Zero Clamping
    somatic = {"caloric_reserve": 50.0, "focus": 1.0, "ambient_temp_c": 22.5, "is_exhausted": False}
    psych = {"focus": 1.0}
    # Run 10000 ticks in severe cold
    for _ in range(10000):
        step_homeostasis(somatic, psych, -50.0, 1.0 / 60.0)

    results["caloric_clamping_tests"] = {
        "final_caloric_reserve": somatic["caloric_reserve"],
        "clamped_exactly_to_zero": somatic["caloric_reserve"] == 0.0,
        "is_exhausted_triggered": somatic["is_exhausted"],
    }

    # 5. Focus Drain Dynamics
    somatic_foc = {"caloric_reserve": 5000.0, "focus": 1.0, "ambient_temp_c": 22.5, "is_exhausted": False}
    psych_foc = {"focus": 1.0}
    # Drain focus at -10°C (rate = 0.050/s) -> 20s to drain
    focus_series = []
    for _ in range(1500):
        step_homeostasis(somatic_foc, psych_foc, -10.0, 1.0 / 60.0)
        focus_series.append(psych_foc["focus"])

    results["focus_drain_tests"] = {
        "initial_focus": 1.0,
        "final_focus": psych_foc["focus"],
        "clamped_to_zero": psych_foc["focus"] == 0.0,
        "somatic_psych_synced": somatic_foc["focus"] == psych_foc["focus"],
        "monotonic_decrease": all(focus_series[i] >= focus_series[i+1] for i in range(len(focus_series)-1)),
    }

    # 6. Shelter Resolution
    results["shelter_tests"] = {
        "indoor_pos_sheltered": is_sheltered(12.0, 2.0, 4.0),
        "indoor_neg_x_sheltered": is_sheltered(-12.0, 2.0, 4.0),
        "outside_x_rejected": not is_sheltered(8.0, 2.0, 4.0) and not is_sheltered(18.5, 2.0, 4.0),
        "outside_y_rejected": not is_sheltered(12.0, 0.5, 4.0) and not is_sheltered(12.0, 4.5, 4.0),
        "outside_z_rejected": not is_sheltered(12.0, 2.0, 1.0) and not is_sheltered(12.0, 2.0, 11.5),
        "midnight_indoor_temp": round(resolve_ambient_temp(12.0, 2.0, 4.0, 900), 4),
        "noon_indoor_temp": round(resolve_ambient_temp(12.0, 2.0, 4.0, 300), 4),
        "indoor_always_in_goldilocks": all(
            GOLDILOCKS_MIN_TEMP_C <= resolve_ambient_temp(12.0, 2.0, 4.0, t) <= GOLDILOCKS_MAX_TEMP_C
            for t in range(0, 1200, 10)
        ),
    }

    # Global Verdict
    all_passed = (
        all(results["boundary_tests"].values()) and
        results["extreme_temperature_tests"]["all_multipliers_positive"] and
        results["extreme_temperature_tests"]["all_focus_drains_positive"] and
        results["diurnal_cycle_tests"]["bounded_in_10_to_35"] and
        results["caloric_clamping_tests"]["clamped_exactly_to_zero"] and
        results["caloric_clamping_tests"]["is_exhausted_triggered"] and
        results["focus_drain_tests"]["clamped_to_zero"] and
        results["focus_drain_tests"]["monotonic_decrease"] and
        results["shelter_tests"]["indoor_always_in_goldilocks"]
    )

    results["empirical_verdict"] = "APPROVE" if all_passed else "REJECT"

    with open("/Users/raffaine/dev/collective/verification/homeostasis_adversarial_results.json", "w") as f:
        json.dump(results, f, indent=2)

    print(json.dumps(results, indent=2))
    return all_passed

if __name__ == "__main__":
    success = run_adversarial_validation()
    sys.exit(0 if success else 1)
