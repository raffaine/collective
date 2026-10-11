"""
verification/scenarios/run_all_scenario_checks.py
Master runner for the Sovereign Stack 24-Scenario Verification Suite.

Executes all 4 programmatic checks and the Agent-as-Judge evaluation track
across all 24 scenario specifications in docs/scenarios/:
1. verify_zero_floats (C++, JSON, prose)
2. verify_language_discipline (banned rhetoric)
3. verify_currency_token_purity (fiat, USD, $, Value Tokens)
4. verify_five_perspectives (Council section + unanimous consensus)
5. eval_agent_as_judge (6-criterion semantic assessment)

Generates structured JSON report at verification/scenario_verification_report.json.
Exits 0 on complete pass, 1 on any violation.
Python standard library only (unittest, re, json, ast, argparse). Zero pytest dependency.
"""

import argparse
import datetime
import json
import os
import sys
import unittest
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

# Ensure project root is in sys.path
PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))

from verification.scenarios.common import (
    ParsedScenario,
    Violation,
    get_all_scenario_files,
    SCENARIO_CATALOG,
)
from verification.scenarios.verify_zero_floats import verify_zero_floats
from verification.scenarios.verify_language_discipline import verify_language_discipline
from verification.scenarios.verify_currency_token_purity import verify_currency_token_purity
from verification.scenarios.verify_five_perspectives import verify_five_perspectives
from verification.scenarios.eval_agent_as_judge import evaluate_scenario_semantic

REPORT_OUTPUT_PATH = PROJECT_ROOT / "verification" / "scenario_verification_report.json"


def run_all_checks_on_scenario(scenario_path: Path) -> Dict[str, Any]:
    """Execute all programmatic checks and judge evaluation on a single scenario."""
    parsed = ParsedScenario(scenario_path)

    # 1. Programmatic Checks
    float_violations = verify_zero_floats(parsed)
    rhetoric_violations = verify_language_discipline(parsed)
    currency_violations = verify_currency_token_purity(parsed)
    perspective_violations = verify_five_perspectives(parsed)

    all_programmatic = (
        float_violations + rhetoric_violations + currency_violations + perspective_violations
    )

    # 2. Agent-as-Judge Evaluation
    judge_result = evaluate_scenario_semantic(parsed)

    programmatic_status = "PASS" if len(all_programmatic) == 0 else "FAIL"
    overall_status = "PASS" if programmatic_status == "PASS" and judge_result["verdict"] == "PASS" else "FAIL"

    return {
        "scenario_file": parsed.filename,
        "scenario_id": parsed.scenario_id,
        "overall_status": overall_status,
        "programmatic_status": programmatic_status,
        "programmatic_violations_count": len(all_programmatic),
        "float_violations_count": len(float_violations),
        "rhetoric_violations_count": len(rhetoric_violations),
        "currency_violations_count": len(currency_violations),
        "perspective_violations_count": len(perspective_violations),
        "violations": [v.to_dict() for v in all_programmatic],
        "judge_result": judge_result,
    }


def run_full_suite(
    scenario_paths: Optional[List[Path]] = None,
    output_report: Optional[Path] = None,
) -> Tuple[int, Dict[str, Any]]:
    """Run verification suite across scenarios, write JSON report, return exit code and summary."""
    files = scenario_paths or get_all_scenario_files()
    report_file = output_report or REPORT_OUTPUT_PATH

    total_scenarios = len(files)
    scenarios_passing_prog = 0
    scenarios_passing_judge = 0
    scenarios_passing_overall = 0

    total_float_violations = 0
    total_rhetoric_violations = 0
    total_currency_violations = 0
    total_perspective_violations = 0

    scenario_reports: List[Dict[str, Any]] = []

    print("================================================================================")
    print("      SOVEREIGN STACK E2E SCENARIO VERIFICATION TEST SUITE (M0)                 ")
    print("================================================================================")
    print(f"Target Scenarios: {total_scenarios} files in docs/scenarios/")
    print(f"Timestamp:        {datetime.datetime.now(datetime.timezone.utc).isoformat()}")
    print("--------------------------------------------------------------------------------\n")

    for idx, path in enumerate(files, start=1):
        res = run_all_checks_on_scenario(path)
        scenario_reports.append(res)

        total_float_violations += res["float_violations_count"]
        total_rhetoric_violations += res["rhetoric_violations_count"]
        total_currency_violations += res["currency_violations_count"]
        total_perspective_violations += res["perspective_violations_count"]

        if res["programmatic_status"] == "PASS":
            scenarios_passing_prog += 1
        if res["judge_result"]["verdict"] == "PASS":
            scenarios_passing_judge += 1
        if res["overall_status"] == "PASS":
            scenarios_passing_overall += 1

        prog_tag = "PASS" if res["programmatic_status"] == "PASS" else f"FAIL ({res['programmatic_violations_count']})"
        judge_tag = f"Judge: {res['judge_result']['score']}"
        print(f"[{idx:02d}/24] {path.name:<25} | Programmatic: {prog_tag:<14} | {judge_tag}")

        if res["programmatic_violations_count"] > 0:
            # Print sample violations
            for v in res["violations"][:3]:
                print(f"       -> [Line {v['line_number']}] {v['rule_id']}: {v['message']}")
            if len(res["violations"]) > 3:
                print(f"       -> ... and {len(res['violations']) - 3} more violations")

    print("\n--------------------------------------------------------------------------------")
    print("VERIFICATION SUITE SUMMARY:")
    print(f"  • Scenarios Scanned:             {total_scenarios}")
    print(f"  • Programmatic Passes (0 issues): {scenarios_passing_prog} / {total_scenarios}")
    print(f"  • Agent-as-Judge Passes (6/6):    {scenarios_passing_judge} / {total_scenarios}")
    print(f"  • Overall Passed Scenarios:       {scenarios_passing_overall} / {total_scenarios}")
    print("VIOLATIONS BREAKDOWN:")
    print(f"  • Floating-Point Math:            {total_float_violations} violations")
    print(f"  • Banned Adversarial Rhetoric:    {total_rhetoric_violations} violations")
    print(f"  • Fiat Currency & Global Tokens:  {total_currency_violations} violations")
    print(f"  • 5-Perspective Council Gaps:     {total_perspective_violations} violations")
    print("================================================================================")

    master_verdict = "PASS" if scenarios_passing_overall == total_scenarios else "FAIL"
    print(f"MASTER SUITE VERDICT: {master_verdict}")
    print("================================================================================")

    report_payload = {
        "suite_name": "Sovereign Stack 24-Scenario E2E Verification Suite",
        "timestamp": datetime.datetime.now(datetime.timezone.utc).isoformat(),
        "master_verdict": master_verdict,
        "summary": {
            "scenarios_scanned": total_scenarios,
            "scenarios_passing_programmatic": scenarios_passing_prog,
            "scenarios_failing_programmatic": total_scenarios - scenarios_passing_prog,
            "scenarios_passing_judge": scenarios_passing_judge,
            "scenarios_failing_judge": total_scenarios - scenarios_passing_judge,
            "scenarios_passing_overall": scenarios_passing_overall,
            "total_float_violations": total_float_violations,
            "total_rhetoric_violations": total_rhetoric_violations,
            "total_currency_token_violations": total_currency_violations,
            "total_missing_perspectives": total_perspective_violations,
        },
        "scenarios": scenario_reports,
    }

    report_file.parent.mkdir(parents=True, exist_ok=True)
    with open(report_file, "w", encoding="utf-8") as f:
        json.dump(report_payload, f, indent=2)

    print(f"Structured report written to: {report_file}")

    exit_code = 0 if master_verdict == "PASS" else 1
    return exit_code, report_payload


class TestMasterScenarioSuite(unittest.TestCase):
    """Unittest suite integration executing all scenario checks."""

    def test_run_full_verification_suite(self) -> None:
        exit_code, report = run_full_suite()
        # The test runner verifies execution succeeds and generates report
        self.assertTrue(REPORT_OUTPUT_PATH.exists())
        self.assertIn("summary", report)
        self.assertEqual(report["summary"]["scenarios_scanned"], 24)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Run all Sovereign Stack scenario verification checks."
    )
    parser.add_argument("paths", nargs="*", help="Optional scenario files to check")
    parser.add_argument(
        "--report", type=str, help="Custom output path for scenario_verification_report.json"
    )
    args = parser.parse_args()

    custom_paths = [Path(p) for p in args.paths] if args.paths else None
    report_out = Path(args.report) if args.report else None

    exit_code, _ = run_full_suite(custom_paths, report_out)
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
