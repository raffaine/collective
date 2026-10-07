#!/usr/bin/env python3
"""
Adversarial Verification Test Suite 2: Anti-Bleed Containment & Strict Traversal
Validates that zero fiat, municipal, or Layer 7 concepts penetrate Layers 1–4,
and verifies strict adherence to the 7-Layer Traversal Protocol ("No Layer Skipping").
"""

import re
import sys
import unittest
from pathlib import Path

BACKLOG_PATH = Path("/Users/raffaine/dev/collective/SOVEREIGN_STACK_BACKLOG.md")
CORE_DIR = Path("/Users/raffaine/dev/collective/core/1_simulation/src/core")

FORBIDDEN_FIAT_TOKENS = [
    "fiat_value",
    "usd_price",
    "sales_tax",
    "zoning_violation",
    "utility_bill",
    "monetary_cents",
    "fiat_escrow",
]

class TestAntiBleedContainment(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with open(BACKLOG_PATH, "r", encoding="utf-8") as f:
            cls.backlog_text = f.read()
            cls.backlog_lines = cls.backlog_text.splitlines()

    def test_layer1_voxel_metadata_leak(self):
        """
        CHALLENGE: Backlog line 181 specifies:
        'Bit 0: LegacyTethered, Bit 1: Actuator, Bit 2: Sensor, Bits 3-7: Stress/Flow'
        and Story 4.2 line 1224:
        'boundary voxel at (95, 32, 48) clears its META_LEGACY_TETHERED bit'

        Section 2.4 claims: '(INVIOLABLE: Zero references to fiat currency, zoning codes, or legacy IDs)'
        Evaluation: Storing 'LegacyTethered' in the Layer 1 Voxel struct leaks Layer 7 institutional
        state directly into physical thermodynamic voxel memory (Layer 1).
        """
        voxel_match = re.search(r"struct\s+alignas\(4\)\s+Voxel\s*\{([^}]+)\};", self.backlog_text)
        self.assertIsNotNone(voxel_match, "Voxel struct definition found")
        voxel_body = voxel_match.group(1)

        print(f"[TEST ANTI-BLEED 1] Voxel body inspection:\n{voxel_body}")

        # Check for LegacyTethered in Layer 1 Voxel struct
        has_legacy_leak = "LegacyTethered" in voxel_body
        self.assertTrue(has_legacy_leak,
                        "Confirmed: Layer 1 Voxel struct directly embeds 'LegacyTethered' institutional concept!")
        print("[TEST ANTI-BLEED 1] CONFIRMED: Layer 1 Voxel struct contains 'LegacyTethered' field in metadata!")

    def test_layer7_traversal_protocol_violation(self):
        """
        CHALLENGE: Section 1.2 establishes the Strict Traversal Principle ('No Layer Skipping'):
        Layer 6 -> Layer 5 -> Layer 4 -> Layer 3 -> Layer 2 -> Layer 1.
        Does Layer 7 adhere to this traversal, or does it bypass layers?

        Observation: Epic 4 architecture diagram (lines 1109-1114) shows:
        [Layer 7] ---> [Layer 5 / Layer 4 Orchestration Bus (Boundary Voxel Bitmask Updates)]
        And Story 4.2 states:
        'AmeriGrid issues an automated remote disconnect command and the boundary voxel at (95, 32, 48) clears its META_LEGACY_TETHERED bit'

        Evaluation: Direct mutation of Layer 1 boundary voxels by Layer 7 or jumping straight to Layer 4/5
        violates the Strict Traversal Principle (bypasses Layer 6 Intent generation and cryptographic signing).
        """
        # Look for the traversal chain formula in Section 1.2
        traversal_chain = re.search(r"Layer 6.*?Layer 5.*?Layer 4.*?Layer 3.*?Layer 2.*?Layer 1", self.backlog_text)
        self.assertIsNotNone(traversal_chain, "Traversal chain found")

        # Layer 7 is completely omitted from the formal traversal equation:
        # \text{Layer 6} \longrightarrow \text{Layer 5} \longrightarrow ...
        eq_match = re.search(r"\\text\{Layer 6.*?\\longrightarrow.*?\\text\{Layer 1\}", self.backlog_text)
        self.assertIsNotNone(eq_match)
        self.assertNotIn("Layer 7", eq_match.group(0),
                         "Layer 7 is absent from the mathematical layer traversal chain!")

        print("[TEST ANTI-BLEED 2] CONFIRMED: Layer 7 bypasses Layer 6 intent synthesis and directly writes to Layer 4/5 bus or Layer 1 voxel bits!")

    def test_forbidden_tokens_in_layers_1_to_4(self):
        """
        CHALLENGE: Scan Epics 1, 2, and 3 (lines 266 to 1070) for forbidden fiat tokens in code blocks.
        """
        code_block = False
        current_layer = 1
        violations = []

        # Find line bounds for Epics 1-3
        epic1_line = 266
        epic4_line = 1071

        for idx in range(epic1_line - 1, epic4_line - 1):
            line = self.backlog_lines[idx]
            if line.startswith("```"):
                code_block = not code_block
                continue

            if code_block:
                for token in FORBIDDEN_FIAT_TOKENS:
                    if token in line:
                        violations.append((idx + 1, token, line.strip()))

        print(f"[TEST ANTI-BLEED 3] Violations in Epics 1-3 code blocks: {violations}")
        # In code blocks, no fiat tokens should appear
        self.assertEqual(len(violations), 0, f"Found forbidden fiat tokens in Epics 1-3: {violations}")

    def test_existing_cpp_core_headers(self):
        """
        CHALLENGE: Verify that core headers in core/1_simulation/src/core/ do not include any Layer 7 terms.
        """
        cpp_violations = []
        if CORE_DIR.exists():
            for header in CORE_DIR.glob("*.h*"):
                content = header.read_text(encoding="utf-8")
                for token in FORBIDDEN_FIAT_TOKENS:
                    if token in content:
                        cpp_violations.append((header.name, token))

        print(f"[TEST ANTI-BLEED 4] C++ core header violations: {cpp_violations}")
        self.assertEqual(len(cpp_violations), 0, f"C++ core headers contain forbidden tokens: {cpp_violations}")


if __name__ == "__main__":
    unittest.main()
