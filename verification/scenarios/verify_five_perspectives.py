"""
verification/scenarios/verify_five_perspectives.py
Programmatic verification of the 5-Perspective Architectural Council Ratification
across all 24 scenario specifications:
1. Presence of the Council Ratification section
2. Presence of explicit unanimous consensus declaration
3. Distinct representation of all 5 mandatory personas:
   - Game Designer
   - Game Engineer
   - Sovereign Stack Specialist
   - Solarpunk Thinker
   - Scenario Specialist
4. Substantive, non-trivial contribution text for each persona.

Requirements: Python standard library only (unittest, re, json, ast, argparse).
Zero pytest dependency.
"""

import argparse
import json
import os
import re
import sys
import unittest
from pathlib import Path
from typing import Dict, List, Optional, Tuple, Union

# Ensure project root is in sys.path for direct script execution
PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))

from verification.scenarios.common import (
    ParsedScenario,
    Violation,
    get_all_scenario_files,
    SCENARIO_CATALOG,
)

# Mandatory council personas
MANDATORY_PERSONAS = [
    ("Game Designer", re.compile(r"\bGame\s+Designer\b", re.IGNORECASE)),
    ("Game Engineer", re.compile(r"\bGame\s+Engineer\b", re.IGNORECASE)),
    ("Sovereign Stack Specialist", re.compile(r"\bSovereign\s+Stack\s+Specialist\b", re.IGNORECASE)),
    ("Solarpunk Thinker", re.compile(r"\bSolarpunk\s+Thinker\b", re.IGNORECASE)),
    ("Scenario Specialist", re.compile(r"\bScenario\s+Specialist\b", re.IGNORECASE)),
]

RATIFICATION_HEADING_PATTERN = re.compile(
    r"#+\s+.*(?:Architectural\s+Council.*5-Perspective|5-Perspective.*Synthesis)",
    re.IGNORECASE,
)

UNANIMOUS_CONSENSUS_PATTERN = re.compile(
    r"\b(unanimous\s+consensus(?:\s+reached)?|ratified\s+by\s+unanimous\s+consensus)\b",
    re.IGNORECASE,
)


def verify_five_perspectives(scenario: Union[Path, str, ParsedScenario]) -> List[Violation]:
    """Verify scenario has complete 5-perspective council section and unanimous consensus."""
    if isinstance(scenario, (Path, str)):
        parsed = ParsedScenario(Path(scenario))
    else:
        parsed = scenario

    violations: List[Violation] = []

    # 1. Check for Ratification Section Heading
    heading_found = False
    heading_line = 1
    for idx, raw_line in enumerate(parsed.lines, start=1):
        if RATIFICATION_HEADING_PATTERN.search(raw_line):
            heading_found = True
            heading_line = idx
            break

    if not heading_found:
        violations.append(
            Violation(
                file_path=str(parsed.file_path),
                line_number=1,
                rule_id="RULE_COUNCIL_HEADING_MISSING",
                message="Missing '## Architectural Council Ratification & 5-Perspective Synthesis' section",
                snippet=parsed.line_at(1),
            )
        )
        # If heading is missing, can't validate council personas within it
        return violations

    # 2. Check for Unanimous Consensus Ratification Statement
    consensus_found = False
    consensus_line = heading_line
    # Search within next 30 lines of heading
    search_limit = min(len(parsed.lines), heading_line + 35)
    for idx in range(heading_line, search_limit + 1):
        line_content = parsed.line_at(idx)
        if UNANIMOUS_CONSENSUS_PATTERN.search(line_content):
            consensus_found = True
            consensus_line = idx
            break

    if not consensus_found:
        violations.append(
            Violation(
                file_path=str(parsed.file_path),
                line_number=heading_line,
                rule_id="RULE_UNANIMOUS_CONSENSUS_MISSING",
                message="Missing explicit unanimous consensus ratification statement (e.g. 'ratified by unanimous consensus')",
                snippet=parsed.line_at(heading_line),
            )
        )

    # 3. Check for each of the 5 mandatory personas in the council section
    # Find table rows following the heading up to next major section
    council_lines: List[Tuple[int, str]] = []
    in_council_section = False
    for idx, raw_line in enumerate(parsed.lines, start=1):
        if idx == heading_line:
            in_council_section = True
            continue
        if in_council_section:
            # Check if next top-level heading begins
            if raw_line.startswith("## ") and not raw_line.startswith("### "):
                break
            council_lines.append((idx, raw_line))

    council_text = "\n".join(text for _, text in council_lines)

    for persona_name, persona_pattern in MANDATORY_PERSONAS:
        match = persona_pattern.search(council_text)
        if not match:
            violations.append(
                Violation(
                    file_path=str(parsed.file_path),
                    line_number=heading_line,
                    rule_id="RULE_MISSING_COUNCIL_PERSONA",
                    message=f"Missing mandatory Council Persona '{persona_name}' in 5-Perspective Synthesis",
                    snippet=f"Council section at line {heading_line}",
                )
            )
        else:
            # Check for substantive contribution text on the matching line / table row
            # Find the specific line where this persona appears
            for idx, text in council_lines:
                if persona_pattern.search(text):
                    # Check table row length or non-whitespace content
                    cells = [c.strip() for c in text.split("|") if c.strip()]
                    # Usually table has: | Council Persona | Disciplinary Representative | Core Contribution |
                    # The contribution cell should have at least 20 chars
                    contribution = cells[-1] if len(cells) >= 3 else text
                    if len(contribution) < 20:
                        violations.append(
                            Violation(
                                file_path=str(parsed.file_path),
                                line_number=idx,
                                rule_id="RULE_INSUFFICIENT_PERSONA_CONTRIBUTION",
                                message=f"Contribution for '{persona_name}' is insufficient or empty (<20 chars)",
                                snippet=text,
                            )
                        )
                    break

    return violations


class TestFivePerspectives(unittest.TestCase):
    """Unittest test case verifying all 24 scenarios contain 5-perspective council synthesis."""

    def test_all_scenarios_have_5_perspective_consensus(self) -> None:
        files = get_all_scenario_files()
        all_violations: List[Violation] = []

        for f in files:
            v_list = verify_five_perspectives(f)
            all_violations.extend(v_list)

        if all_violations:
            summary = "\n".join(str(v) for v in all_violations[:20])
            total = len(all_violations)
            self.fail(
                f"Found {total} 5-perspective council violations across scenarios:\n{summary}"
            )


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Verify 5-perspective council ratification and unanimous consensus."
    )
    parser.add_argument("paths", nargs="*", help="Optional scenario files to check")
    parser.add_argument(
        "--json", action="store_true", help="Output results in JSON format"
    )
    args = parser.parse_args()

    files = [Path(p) for p in args.paths] if args.paths else get_all_scenario_files()
    total_violations = 0
    results = []

    for f in files:
        violations = verify_five_perspectives(f)
        total_violations += len(violations)
        results.append(
            {
                "file": f.name,
                "violations_count": len(violations),
                "violations": [v.to_dict() for v in violations],
            }
        )
        if not args.json:
            status = "PASS" if not violations else f"FAIL ({len(violations)} violations)"
            print(f"[{status}] {f.name}")
            for v in violations:
                print(f"  Line {v.line_number}: {v.message}")
                print(f"    --> {v.snippet[:80]}")

    if args.json:
        print(
            json.dumps(
                {
                    "total_violations": total_violations,
                    "scenarios_scanned": len(files),
                    "results": results,
                },
                indent=2,
            )
        )

    return 0 if total_violations == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
