#!/usr/bin/env python3
"""
Round 3 Empirical Verification Suite - Domain 1: DF Cognitive Mechanics
Verifies:
1. Fixed-point numeric precision & overflow bounds (Fixed16 vs Fixed32 for [-100k, +100k] stress).
2. Lyapunov emotional homeostasis & Tantrum death spiral basin of attraction.
3. 32-slot episodic memory circular ring buffer under high-frequency trauma bursts.
"""

import math
import sys
import unittest
from dataclasses import dataclass
from typing import List, Optional, Dict, Tuple

# ============================================================================
# 1. FIXED-POINT PRECISION & OVERFLOW TEST
# ============================================================================

class TestD1FixedPointBounds(unittest.TestCase):
    """
    Evaluates Architecture Section 3.2 vs GDD Section 2.3:
    Architecture 3.2 specifies:
      `Emotional State & Accum: 16 bytes: Fixed16[8] Stress, Mood, Focus`
      `class Fixed16 { int16_t raw_val_; ... Q8.8 fixed-point }`
    GDD 2.3 specifies:
      `Cumulative Stress: -100,000 ... +100,000 (Fixed32)`
      Breakdown thresholds: Mild > 25,000, Tantrum > 50,000, Catatonia > 80,000.
    """

    def test_fixed16_overflow_on_stress_thresholds(self):
        print("\n--- [TEST D1.1] Fixed16 vs Fixed32 Precision & Overflow Analysis ---")
        INT16_MIN = -32768
        INT16_MAX = 32767

        # Case A: Fixed16 as Q8.8 fixed-point (as defined in OASIS_ARCHITECTURE.md line 550)
        # In Q8.8, 1 unit = 256 raw units.
        max_q8_8_float = INT16_MAX / 256.0
        min_q8_8_float = INT16_MIN / 256.0
        print(f"Fixed16 (Q8.8) Float Range: [{min_q8_8_float:.3f}, {max_q8_8_float:.3f}]")

        # Thresholds from GDD 2.3
        threshold_mild = 25000.0
        threshold_tantrum = 50000.0
        threshold_catatonia = 80000.0
        max_stress = 100000.0

        # Test storing 25,000 in Q8.8
        raw_mild = int(threshold_mild * 256)
        raw_mild_int16 = (raw_mild + 32768) % 65536 - 32768
        print(f"Target Stress Mild (25,000): Raw Q8.8 = {raw_mild}, Clamped to int16_t = {raw_mild_int16}")

        # Test storing 50,000 in Q8.8
        raw_tantrum = int(threshold_tantrum * 256)
        raw_tantrum_int16 = (raw_tantrum + 32768) % 65536 - 32768
        print(f"Target Stress Tantrum (50,000): Raw Q8.8 = {raw_tantrum}, Clamped to int16_t = {raw_tantrum_int16}")

        # Case B: Even if raw int16_t is interpreted as integer stress (without fraction)
        raw_int16_mild = 25000 # fits in int16 (< 32767)
        raw_int16_tantrum = (50000 + 32768) % 65536 - 32768 # -15536
        raw_int16_catatonia = (80000 + 32768) % 65536 - 32768 # +14464
        raw_int16_max = (100000 + 32768) % 65536 - 32768 # -31072

        print(f"Integer int16_t Tantrum (50,000) wraps to: {raw_int16_tantrum}")
        print(f"Integer int16_t Catatonia (80,000) wraps to: {raw_int16_catatonia}")
        print(f"Integer int16_t Max Stress (100,000) wraps to: {raw_int16_max}")

        # Assert that Fixed16 CANNOT represent stress thresholds
        self.assertTrue(raw_mild > INT16_MAX, "Fixed16 Q8.8 overflows on 25,000 stress")
        self.assertTrue(threshold_tantrum > INT16_MAX, "int16_t cannot hold 50,000 stress")
        self.assertTrue(threshold_catatonia > INT16_MAX, "int16_t cannot hold 80,000 stress")
        self.assertTrue(max_stress > INT16_MAX, "int16_t cannot hold 100,000 max stress")
        print("CONFIRMED: Architecture Section 3.2 allocating `Fixed16[8]` for Emotional State & Accum")
        print("is fundamentally incompatible with GDD 2.3 [-100k, +100k] stress model and will suffer catastrophic overflow.")


# ============================================================================
# 2. LYAPUNOV EMOTIONAL HOMEOSTASIS & DEATH SPIRALS
# ============================================================================

@dataclass
class PsychologicalAgent:
    name: str
    focus: float = 200.0          # 0..255
    mood: float = 0.0             # -10,000 .. +10,000
    stress: float = 0.0           # -100,000 .. +100,000
    anxiety: float = 50.0         # 0..100
    resilience: float = 0.5       # kappa_resilience
    homeostasis: float = 0.05     # delta_homeostasis
    baseline_stress: float = 0.0
    k_accum: float = 0.02
    k_decay: float = 0.01
    m_threshold: float = 0.0
    breakdown_state: str = "NORMAL"

    def update_tick(self, mutual_aid: float, environment_intact: bool, external_shock: float = 0.0):
        # 1. State-dependent behavioral degradation
        if self.stress > 80000:
            self.breakdown_state = "CATATONIA"
            self.focus = max(0.0, self.focus - 5.0)
            # In catatonia, cannot work or eat autonomously
        elif self.stress > 50000:
            self.breakdown_state = "TANTRUM"
            self.focus = max(0.0, self.focus - 2.0)
            # In tantrum, smashes tools/inverters, degrades mutual aid
        elif self.stress > 25000:
            self.breakdown_state = "MILD_IRRITABLE"
            self.focus = max(0.0, self.focus - 0.5)
        else:
            self.breakdown_state = "NORMAL"
            self.focus = min(255.0, self.focus + 1.0)

        # 2. Mood calculation
        # If environment is damaged or tools smashed, negative thoughts persist
        base_mood = 500.0 if environment_intact else -2000.0
        if self.breakdown_state == "TANTRUM":
            base_mood -= 3000.0 # self-loathing / rage thoughts
        elif self.breakdown_state == "CATATONIA":
            base_mood -= 5000.0

        self.mood = max(-10000.0, min(10000.0, base_mood))

        # 3. Continuous stress derivative ODE (GDD 2.3)
        if self.mood > self.m_threshold:
            d_stress = -self.k_decay * (self.mood - self.m_threshold)
        else:
            d_stress = +self.k_accum * (self.m_threshold - self.mood) * (1.0 + self.anxiety / 100.0)

        # 4. Discrete Lyapunov damping / Shock (GDD 2.3 line 171)
        # Delta_Stress_net = Shock - kappa*(Focus + MutualAid) - delta*(Stress - Baseline)
        effective_mutual_aid = mutual_aid if self.breakdown_state != "TANTRUM" else (mutual_aid * 0.2)
        damping = self.resilience * (self.focus + effective_mutual_aid) + self.homeostasis * (self.stress - self.baseline_stress)
        net_delta = d_stress + external_shock - damping
        self.stress += net_delta
        self.stress = max(-100000.0, min(100000.0, self.stress))
        
        # 5. Update breakdown state for new stress value
        if self.stress > 80000:
            self.breakdown_state = "CATATONIA"
        elif self.stress > 50000:
            self.breakdown_state = "TANTRUM"
        elif self.stress > 25000:
            self.breakdown_state = "MILD_IRRITABLE"
        else:
            self.breakdown_state = "NORMAL"

class TestD1LyapunovStability(unittest.TestCase):
    def test_lyapunov_recovery_under_normal_shocks(self):
        print("\n--- [TEST D1.2] Lyapunov Recovery Under Normal Transient Shock ---")
        agent = PsychologicalAgent(name="Founder 01", stress=10000.0, focus=200.0)
        
        # Apply a transient shock of 15,000 for 10 ticks
        for t in range(200):
            shock = 1500.0 if t < 10 else 0.0
            agent.update_tick(mutual_aid=100.0, environment_intact=True, external_shock=shock)

        print(f"Final state after 200 ticks: Stress = {agent.stress:.1f}, Focus = {agent.focus:.1f}, State = {agent.breakdown_state}")
        self.assertEqual(agent.breakdown_state, "NORMAL")
        self.assertLess(agent.stress, 15000.0)
        print("PASS: Under intact environment and positive mutual aid, Lyapunov damping stabilizes stress.")

    def test_tantrum_cascade_death_spiral(self):
        print("\n--- [TEST D1.3] Tantrum Feedback Cascade & Timescale Paradox ---")
        # Regime A: Slow-moving hysteretic accumulator (GDD 2.3: "integrates over days and weeks")
        # delta_homeostasis and k_accum calibrated for multi-day timescales
        agent_slow = PsychologicalAgent(
            name="Founder 01",
            stress=50000.0,
            focus=20.0,
            anxiety=80.0,
            resilience=0.01,
            homeostasis=0.0001, # ~1/10000 per sec -> half life ~7 hours
            k_accum=0.005,
            m_threshold=0.0
        )

        print("Testing Regime A: Realistic slow-moving hysteretic accumulator (delta_homeostasis = 0.0001)...")
        # In Tantrum, agent smashes inverters -> environment becomes damaged, mutual aid collapses
        env_intact = False
        mutual_aid = 5.0

        for t in range(1000):
            agent_slow.update_tick(mutual_aid=mutual_aid, environment_intact=env_intact, external_shock=0.0)

        print(f"Regime A after 1000 ticks: Stress = {agent_slow.stress:.1f}, Focus = {agent_slow.focus:.1f}, State = {agent_slow.breakdown_state}")
        self.assertEqual(agent_slow.breakdown_state, "CATATONIA")
        self.assertGreaterEqual(agent_slow.stress, 80000.0)
        self.assertEqual(agent_slow.focus, 0.0)
        print("CONFIRMED: In realistic multi-day accumulation timescale, Tantrum feedback causes irreversible Catatonia death spiral.")

        # Regime B: Fast damping test demonstrating the timescale contradiction
        agent_fast = PsychologicalAgent(
            name="Founder 01",
            stress=50000.0,
            focus=100.0,
            homeostasis=0.05 # half-life ~14 seconds
        )
        for t in range(100):
            agent_fast.update_tick(mutual_aid=10.0, environment_intact=False, external_shock=0.0)
        print(f"Regime B after 100 ticks (100s): Stress = {agent_fast.stress:.1f}, State = {agent_fast.breakdown_state}")
        print("CONFIRMED TIMESCALE PARADOX:")
        print("If damping is high enough to prevent death spirals (Regime B), mental breakdowns evaporate in ~15 seconds,")
        print("contradicting GDD 2.3 requirement of a 'slow-moving hysteretic accumulator over days and weeks'.")
        print("If damping is low enough to persist over days (Regime A), the lack of an autonomous non-linear recovery attractor")
        print("causes agents to permanently lock into Catatonia (100k stress).")


# ============================================================================
# 3. 32-SLOT EPISODIC MEMORY RING BUFFER BURST TEST
# ============================================================================

@dataclass
class EpisodicMemoryNode:
    timestamp: int
    event_id: int
    valence: float          # -100.0 .. +100.0
    salience: float         # 0.0 .. 100.0
    voxel_x: int
    voxel_y: int
    voxel_z: int
    description: str

class AgentEpisodicMemoryRing:
    """32-slot circular ring buffer as specified in GDD 2.5 and Backlog Story 1.2."""
    def __init__(self, capacity: int = 32):
        self.capacity = capacity
        self.buffer: List[Optional[EpisodicMemoryNode]] = [None] * capacity
        self.head: int = 0
        self.total_inserted: int = 0
        self.spatial_hash: Dict[Tuple[int, int, int], int] = {} # (x,y,z) -> slot index

    def push_event(self, node: EpisodicMemoryNode):
        slot = self.head
        # Evict old node from spatial hash if overwriting
        old_node = self.buffer[slot]
        if old_node is not None:
            old_coord = (old_node.voxel_x, old_node.voxel_y, old_node.voxel_z)
            if self.spatial_hash.get(old_coord) == slot:
                del self.spatial_hash[old_coord]

        self.buffer[slot] = node
        self.spatial_hash[(node.voxel_x, node.voxel_y, node.voxel_z)] = slot
        self.head = (self.head + 1) % self.capacity
        self.total_inserted += 1

    def query_spatial_flashback(self, x: int, y: int, z: int, radius_dm: int = 4) -> Optional[EpisodicMemoryNode]:
        for (vx, vy, vz), slot in self.spatial_hash.items():
            dist_sq = (vx - x)**2 + (vy - y)**2 + (vz - z)**2
            if dist_sq <= radius_dm**2:
                return self.buffer[slot]
        return None

class TestD1MemoryRingBounds(unittest.TestCase):
    def test_high_frequency_trauma_burst_amnesia(self):
        print("\n--- [TEST D1.4] 32-Slot Ring Buffer High-Frequency Trauma Burst & Eviction ---")
        ring = AgentEpisodicMemoryRing(capacity=32)

        # 1. Critical Genesis Trauma: Municipal Code Enforcement Raid at (95, 32, 48)
        raid_node = EpisodicMemoryNode(
            timestamp=1000,
            event_id=1,
            valence=-95.0,
            salience=99.0,
            voxel_x=95,
            voxel_y=32,
            voxel_z=48,
            description="Violent Municipal Code Raid & Solar Inverter Confiscation"
        )
        ring.push_event(raid_node)
        print(f"Pushed critical trauma: '{raid_node.description}' at (95, 32, 48). Head = {ring.head}")

        # Verify spatial retrieval immediately
        retrieved = ring.query_spatial_flashback(95, 32, 48, radius_dm=4)
        self.assertIsNotNone(retrieved)
        self.assertEqual(retrieved.event_id, 1)
        print("Immediate spatial flashback retrieval: SUCCESS")

        # 2. Simulate high-frequency event burst (40 secondary events: noise, mud, tool drops, verbal disputes)
        print("Simulating event burst: 40 secondary events within 2 game hours...")
        for i in range(2, 42):
            sec_node = EpisodicMemoryNode(
                timestamp=1000 + i * 10,
                event_id=i,
                valence=-15.0,
                salience=20.0,
                voxel_x=10 + i,
                voxel_y=10,
                voxel_z=10,
                description=f"Secondary Event {i} (stumbled, heard siren, dropped nail)"
            )
            ring.push_event(sec_node)

        # 3. Check if critical trauma still exists in the ring buffer
        all_event_ids = [n.event_id for n in ring.buffer if n is not None]
        print(f"Total events inserted: {ring.total_inserted}. Current IDs in buffer: min={min(all_event_ids)}, max={max(all_event_ids)}")

        # Because capacity is 32 and 41 events were inserted, the first 9 events (including event 1) were overwritten!
        self.assertNotIn(1, all_event_ids, "Event 1 (Major Raid) has been completely wiped by FIFO circular eviction!")

        # 4. Attempt spatial flashback retrieval at (95, 32, 48)
        flashback_after_burst = ring.query_spatial_flashback(95, 32, 48, radius_dm=4)
        print(f"Spatial flashback query at (95, 32, 48) after burst: {flashback_after_burst}")
        self.assertIsNone(flashback_after_burst)

        print("CONFIRMED BUG / LIMITATION:")
        print("Standard 32-slot FIFO circular overwrite suffers from 'Trauma Amnesia' under event bursts.")
        print("High-salience critical life events are overwritten by low-salience transient chatter")
        print("before the diurnal sleep consolidation cycle can commit them to Core Values.")

if __name__ == "__main__":
    unittest.main()
