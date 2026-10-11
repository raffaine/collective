"""
verification/scenarios/verify_language_discipline.py
Programmatic verification of zero banned adversarial rhetoric across:
1. Markdown headings and section titles
2. Gate tables (specifically Gate G5 and Gate G6)
3. Mermaid diagrams and sequence labels
4. C++ code blocks and comments
5. Markdown prose and lists

Banned rhetoric terms per Chapter 8 §8.9:
- "Trojan Ingestion"
- "Ecological Leeching"
- "Trojan Horse" / "Trojan"
- "Leeching" / "Leech"
- "Parasite" / "Parasitic" / "Parasitism"

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
from typing import List, Union

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

BANNED_RHETORIC_PATTERN = re.compile(
    r"\b(Trojan\s+Ingestion|Ecological\s+Leeching|Trojan\s+Manufacturing|"
    r"Trojan\s+Horse|Trojan|Leech(?:ing|ed|es)?|Parasit(?:e|ic|ism)?)\b",
    re.IGNORECASE,
)


def verify_language_discipline(scenario: Union[Path, str, ParsedScenario]) -> List[Violation]:
    """Scan all lines of scenario for banned adversarial rhetoric."""
    if isinstance(scenario, (Path, str)):
        parsed = ParsedScenario(Path(scenario))
    else:
        parsed = scenario

    violations: List[Violation] = []

    for idx, raw_line in enumerate(parsed.lines, start=1):
        matches = list(BANNED_RHETORIC_PATTERN.finditer(raw_line))
        for m in matches:
            term = m.group(0)
            rule_id = "RULE_BANNED_RHETORIC"
            if "trojan" in term.lower():
                rule_id = "RULE_BANNED_RHETORIC_TROJAN"
            elif "leech" in term.lower():
                rule_id = "RULE_BANNED_RHETORIC_LEECH"
            elif "parasit" in term.lower():
                rule_id = "RULE_BANNED_RHETORIC_PARASITE"

            violations.append(
                Violation(
                    file_path=str(parsed.file_path),
                    line_number=idx,
                    rule_id=rule_id,
                    message=f"Banned adversarial rhetoric '{term}' detected",
                    snippet=raw_line,
                )
            )

    return violations


class TestLanguageDiscipline(unittest.TestCase):
    """Unittest test case verifying all 24 scenarios adhere to language discipline."""

    def test_all_scenarios_have_zero_banned_rhetoric(self) -> None:
        files = get_all_scenario_files()
        all_violations: List[Violation] = []

        for f in files:
            v_list = verify_language_discipline(f)
            all_violations.extend(v_list)

        if all_violations:
            summary = "\n".join(str(v) for v in all_violations[:20])
            total = len(all_violations)
            self.fail(
                f"Found {total} banned rhetoric violations across scenarios:\n{summary}"
            )


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Verify zero banned adversarial rhetoric across scenarios."
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
        violations = verify_language_discipline(f)
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
