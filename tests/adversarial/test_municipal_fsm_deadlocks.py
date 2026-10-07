#!/usr/bin/env python3
"""
Adversarial Verification Test Suite 1: Municipal Code Enforcement FSM
Evaluates 5-state (6-enum) FSM transitions, reachability, and administrative deadlocks.
"""

import sys
import unittest
from enum import IntEnum

class FsmViolationState(IntEnum):
    UNNOTICED = 0
    COMPLAINT_FILED = 1
    INSPECTION_SCHEDULED = 2
    NOV_POSTED = 3
    STOP_WORK_ORDER = 4
    ABATEMENT_EXECUTED = 5

class MockBpmnEngine:
    def __init__(self):
        self.locked_chunks = set()
        self.tasks = []

    def lock_chunk(self, chunk_id: int):
        self.locked_chunks.add(chunk_id)

    def unlock_chunk(self, chunk_id: int):
        self.locked_chunks.discard(chunk_id)

    def can_dispatch_work_token(self, chunk_id: int, task_type: str) -> bool:
        # If chunk is locked by Stop-Work Order, WorkTokens are mathematically blocked
        if chunk_id in self.locked_chunks:
            return False
        return True

    def execute_remediation(self, chunk_id: int) -> bool:
        if not self.can_dispatch_work_token(chunk_id, "REMEDIATION"):
            return False
        return True

class MunicipalFsmSimulator:
    def __init__(self, bpmn: MockBpmnEngine):
        self.bpmn = bpmn
        self.state = FsmViolationState.UNNOTICED
        self.chunk_id = 42
        self.notice_level = 0.0
        self.daily_fine_cents = 0
        self.escrow_balance_cents = 500000 # $5,000.00
        self.ticks_in_state = 0
        self.spc_appeal_active = False
        self.spc_appeal_ticks_remaining = 0
        self.violation_cured = False

    def accumulate_notice(self, delta: float):
        self.notice_level += delta
        if self.state == FsmViolationState.UNNOTICED and self.notice_level > 100.0:
            self.state = FsmViolationState.COMPLAINT_FILED
            self.ticks_in_state = 0

    def file_spc_appeal(self):
        # RCW 23B.25 appeal stays fines for 30 in-game days (assume 1 day = 100 ticks)
        self.spc_appeal_active = True
        self.spc_appeal_ticks_remaining = 3000

    def attempt_cure_via_bpmn(self) -> bool:
        # Player attempts to dispatch labor to dismantle or bring structure into compliance
        success = self.bpmn.execute_remediation(self.chunk_id)
        if success:
            self.violation_cured = True
            self.state = FsmViolationState.UNNOTICED
            self.bpmn.unlock_chunk(self.chunk_id)
            self.daily_fine_cents = 0
            self.ticks_in_state = 0
        return success

    def tick(self, ticks: int = 1):
        for _ in range(ticks):
            self.ticks_in_state += 1
            if self.spc_appeal_active:
                self.spc_appeal_ticks_remaining -= 1
                if self.spc_appeal_ticks_remaining <= 0:
                    self.spc_appeal_active = False

            # Daily fines apply if in State 3 or 4 and appeal not active (assume daily fine assessed every 100 ticks)
            if self.ticks_in_state % 100 == 0:
                if (self.state in (FsmViolationState.NOV_POSTED, FsmViolationState.STOP_WORK_ORDER)) and not self.spc_appeal_active:
                    self.escrow_balance_cents -= self.daily_fine_cents

            # Transition logic as defined in Backlog Story 4.3
            if self.state == FsmViolationState.COMPLAINT_FILED:
                # Timer: 3 to 7 Days (300 to 700 ticks). Let's use 500 ticks
                if self.ticks_in_state >= 500:
                    self.state = FsmViolationState.INSPECTION_SCHEDULED
                    self.ticks_in_state = 0

            elif self.state == FsmViolationState.INSPECTION_SCHEDULED:
                # Inspector conducts site visit after 1 tick
                if not self.violation_cured:
                    self.state = FsmViolationState.NOV_POSTED
                    self.daily_fine_cents = 25000 # $250.00/day
                    self.ticks_in_state = 0
                else:
                    self.state = FsmViolationState.UNNOTICED
                    self.ticks_in_state = 0

            elif self.state == FsmViolationState.NOV_POSTED:
                # Timer: 14 Days to Cure (1400 ticks)
                if self.ticks_in_state >= 1400:
                    self.state = FsmViolationState.STOP_WORK_ORDER
                    # Emit coordination lock on targeted voxel coordinates
                    self.bpmn.lock_chunk(self.chunk_id)
                    self.ticks_in_state = 0

            elif self.state == FsmViolationState.STOP_WORK_ORDER:
                # Timer: 30 Days default (3000 ticks) -> ABATEMENT_EXECUTED
                if self.ticks_in_state >= 3000:
                    self.state = FsmViolationState.ABATEMENT_EXECUTED
                    self.escrow_balance_cents -= 350000 # $3,500 demolition lien
                    self.ticks_in_state = 0

            elif self.state == FsmViolationState.ABATEMENT_EXECUTED:
                # Terminal sink state in backlog: no exit transition defined!
                pass


class TestMunicipalFsm(unittest.TestCase):
    def test_state_4_stop_work_order_remediation_deadlock(self):
        """
        STRESS-TEST 1: In State 4 (STOP_WORK_ORDER), Layer 5 emits a coordination lock.
        Does this prevent the player from ever dispatching BPMN labor to cure the violation?
        """
        bpmn = MockBpmnEngine()
        sim = MunicipalFsmSimulator(bpmn)

        # Trigger complaint and advance to State 4
        sim.accumulate_notice(150.0)
        self.assertEqual(sim.state, FsmViolationState.COMPLAINT_FILED)

        # Advance 500 ticks -> INSPECTION_SCHEDULED, then 1 tick -> NOV_POSTED
        sim.tick(500)
        self.assertEqual(sim.state, FsmViolationState.INSPECTION_SCHEDULED)
        sim.tick(1)
        self.assertEqual(sim.state, FsmViolationState.NOV_POSTED)

        # Advance past 14 days cure window (1400 ticks) into STOP_WORK_ORDER
        sim.tick(1400)
        self.assertEqual(sim.state, FsmViolationState.STOP_WORK_ORDER)
        self.assertIn(sim.chunk_id, bpmn.locked_chunks)

        # Now player attempts to dismantle or retrofit the structure to cure it
        cure_result = sim.attempt_cure_via_bpmn()
        print(f"[TEST 1] Attempting cure in State 4: success={cure_result}")

        # BUG CONFIRMATION: The coordination lock prevents physical remediation!
        self.assertFalse(cure_result, "DEADLOCK CONFIRMED: Stop-work coordination lock prevents BPMN remediation tasks")

        # Because player cannot cure, simulation inexorably advances to ABATEMENT_EXECUTED
        sim.tick(3000)
        self.assertEqual(sim.state, FsmViolationState.ABATEMENT_EXECUTED)
        print(f"[TEST 1] Inevitably reached ABATEMENT_EXECUTED due to lock deadlock!")

    def test_state_5_terminal_sink_and_coordination_lock_leak(self):
        """
        STRESS-TEST 2: When State 5 (ABATEMENT_EXECUTED) is reached, does the coordination lock
        remain permanently locked, rendering the parcel coordinates unbuildable forever?
        """
        bpmn = MockBpmnEngine()
        sim = MunicipalFsmSimulator(bpmn)
        sim.accumulate_notice(150.0)
        sim.tick(500 + 1 + 1400 + 3000) # Advance through all timers to abatement

        self.assertEqual(sim.state, FsmViolationState.ABATEMENT_EXECUTED)
        self.assertIn(sim.chunk_id, bpmn.locked_chunks,
                      "COORDINATION LOCK LEAK: Chunk remains locked after demolition by NPCs")

        # Advance 10,000 more ticks: is there any exit transition from State 5?
        sim.tick(10000)
        self.assertEqual(sim.state, FsmViolationState.ABATEMENT_EXECUTED,
                         "STATE SINK CONFIRMED: FSM has no transition out of ABATEMENT_EXECUTED")
        print(f"[TEST 2] State 5 remains permanent sink with leaked coordination lock.")

    def test_spc_appeal_fine_suspension_vs_work_token_lock(self):
        """
        STRESS-TEST 3: Story 4.5 claims SPC appeal grants 'a window to bring the kitchen into compliance'.
        However, Story 4.5 only suspends FINES ('all municipal daily fines are suspended for 30 in-game days').
        Does it clear the coordination lock on BPMN WorkTokens?
        """
        bpmn = MockBpmnEngine()
        sim = MunicipalFsmSimulator(bpmn)
        # Advance to State 4
        sim.accumulate_notice(150.0)
        sim.tick(500 + 1 + 1400)
        self.assertEqual(sim.state, FsmViolationState.STOP_WORK_ORDER)

        # File SPC appeal
        sim.file_spc_appeal()
        self.assertTrue(sim.spc_appeal_active)

        # Attempt remediation during appeal window
        cure_during_appeal = sim.attempt_cure_via_bpmn()
        print(f"[TEST 3] Remediation during SPC appeal window: success={cure_during_appeal}")
        self.assertFalse(cure_during_appeal,
                         "DEFECT CONFIRMED: SPC Appeal does not lift the BPMN coordination lock, so compliance is impossible during stay!")

    def test_negative_escrow_trap(self):
        """
        STRESS-TEST 4: Demolition lien ($3,500) and fines ($250/day) can drive escrow deeply negative.
        Assert that negative escrow prevents Scenario Omega decoupling without external fiat bailout.
        """
        bpmn = MockBpmnEngine()
        sim = MunicipalFsmSimulator(bpmn)
        sim.escrow_balance_cents = 100000 # $1,000.00 initial escrow
        sim.accumulate_notice(150.0)
        sim.tick(500 + 1 + 1400 + 3000) # Advance to Abatement

        print(f"[TEST 4] Escrow balance after abatement: ${sim.escrow_balance_cents / 100:.2f}")
        self.assertLess(sim.escrow_balance_cents, 0, "Escrow balance is negative")


if __name__ == "__main__":
    unittest.main()
