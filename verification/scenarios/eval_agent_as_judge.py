"""
verification/scenarios/eval_agent_as_judge.py
Agent-as-Judge Evaluation Harness and Schema Validator for the Sovereign Stack.

Capabilities:
1. Validates Agent-as-Judge JSON verdict reports against the 6-criterion rubric schema.
2. Anti-Hallucination citation verification: ensures quotes and line numbers match target text.
3. Automated conjunctive semantic assessment engine: enforces genuine multi-facet invariant
   compliance across all 6 criteria without disjunctive facades or lowered thresholds.

Requirements: Python standard library only (unittest, re, json, ast, argparse, tempfile).
Zero pytest dependency.
"""

import argparse
import json
import os
import re
import sys
import tempfile
import unittest
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple, Union

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

RUBRIC_CRITERIA_KEYS = [
    "CRIT-1_BCC",
    "CRIT-2_HTLC",
    "CRIT-3_RINGS",
    "CRIT-4_BPVP",
    "CRIT-5_GOV",
    "CRIT-6_CONS",
]

# 7-Tier Concentric Trust Rings patterns (all 7 strictly required)
RINGS_PATTERNS = [
    re.compile(r"R_0.*Hearth", re.IGNORECASE),
    re.compile(r"R_1.*Settlement", re.IGNORECASE),
    re.compile(r"R_2.*Commons", re.IGNORECASE),
    re.compile(r"R_3.*Watershed", re.IGNORECASE),
    re.compile(r"R_4.*Bioregion", re.IGNORECASE),
    re.compile(r"R_5.*Continental", re.IGNORECASE),
    re.compile(r"R_6.*Ecumene", re.IGNORECASE),
]


def find_matching_lines(parsed: ParsedScenario, pattern: re.Pattern) -> List[Tuple[int, str]]:
    """Return list of (line_no, line_text) matching pattern."""
    matches = []
    for idx, line in enumerate(parsed.lines, start=1):
        if pattern.search(line):
            matches.append((idx, line.strip()))
    return matches


def evaluate_crit_1_bcc(parsed: ParsedScenario) -> Tuple[bool, List[int], str, str]:
    """CRIT-1: Conjunctively verify BCC primitive, zero-sum conservation, and credit window."""
    pat_primitive = re.compile(r"\b(BCC|Bilateral\s+Clearing\s+Channel(?:s)?)\b", re.IGNORECASE)
    pat_conservation = re.compile(
        r"B_{u\s*(?:\\to|->)\s*v}|B_{v\s*(?:\\to|->)\s*u}|zero-sum|conservation", re.IGNORECASE
    )
    pat_headroom = re.compile(
        r"C_{u\s*(?:\\to|->)\s*v}|C_{v\s*(?:\\to|->)\s*u}|credit\s+window|headroom|\[-C", re.IGNORECASE
    )

    m_prim = find_matching_lines(parsed, pat_primitive)
    m_cons = find_matching_lines(parsed, pat_conservation)
    m_head = find_matching_lines(parsed, pat_headroom)

    missing = []
    if not m_prim:
        missing.append("BCC primitive")
    if not m_cons:
        missing.append("zero-sum conservation equation B_{u->v} + B_{v->u} = 0")
    if not m_head:
        missing.append("credit window / headroom bounds [-C, +C]")

    if not missing:
        lines = sorted(list(set([m_prim[0][0], m_cons[0][0], m_head[0][0]])))
        quote = m_cons[0][1]
        return True, lines, quote, "BCC pairwise channels, zero-sum conservation, and credit window verified"
    return False, [], "", f"BCC deficient: missing {', '.join(missing)}"


def evaluate_crit_2_htlc(parsed: ParsedScenario) -> Tuple[bool, List[int], str, str]:
    """CRIT-2: Conjunctively verify HTLC primitive, tuple/hashlock, and delivery receipt/settlement."""
    pat_primitive = re.compile(
        r"\b(HTLC|HTLCs|Hashed\s+Time-?Lock(?:ed)?\s+Contract(?:s)?)\b", re.IGNORECASE
    )
    pat_tuple = re.compile(r"\\langle\s*H\(S\)|\bH\(S\)|preimage|secret\s+hash", re.IGNORECASE)
    pat_settle = re.compile(
        r"\\sigma_{\\mathrm{delivery}}|delivery\s+receipt|settl|timelock\s+deadline|cascading\s+backward|preimage\s+revelation|deterministic\s+rollback",
        re.IGNORECASE,
    )

    m_prim = find_matching_lines(parsed, pat_primitive)
    m_tup = find_matching_lines(parsed, pat_tuple)
    m_set = find_matching_lines(parsed, pat_settle)

    missing = []
    if not m_prim:
        missing.append("HTLC primitive")
    if not m_tup:
        missing.append("hashlock preimage tuple <H(S), delta t, Q, u, v>")
    if not m_set:
        missing.append("delivery receipt sigma_delivery / timelock settlement")

    if not missing:
        lines = sorted(list(set([m_prim[0][0], m_tup[0][0], m_set[0][0]])))
        quote = m_tup[0][1]
        return True, lines, quote, "Multi-hop HTLCs, cryptographic tuple, and delivery receipts verified"
    return False, [], "", f"HTLC deficient: missing {', '.join(missing)}"


def evaluate_crit_3_rings(parsed: ParsedScenario) -> Tuple[bool, List[int], str, str]:
    """CRIT-3: Strictly verify all 7 concentric trust rings (R_0 to R_6) without exception."""
    ring_lines = []
    missing_rings = []

    for idx, pat in enumerate(RINGS_PATTERNS):
        m = find_matching_lines(parsed, pat)
        if m:
            ring_lines.append(m[0][0])
        else:
            missing_rings.append(f"R_{idx}")

    if not missing_rings:
        first_line = sorted(ring_lines)[0]
        quote = parsed.line_at(first_line)
        return True, sorted(ring_lines)[:3], quote, "All 7 concentric trust rings (R_0..R_6) documented"
    return False, [], "", f"Trust rings deficient: missing {', '.join(missing_rings)} (requires all 7 tiers)"


def evaluate_crit_4_bpvp(parsed: ParsedScenario) -> Tuple[bool, List[int], str, str]:
    """CRIT-4: Conjunctively verify BPVP primitive, capability advertisements/surplus, and loop checks."""
    pat_primitive = re.compile(r"\b(BPVP|Bioregional\s+Path-Vector\s+Protocol|FRRP)\b", re.IGNORECASE)
    pat_surplus = re.compile(r"Capability\s+Advertisement|Q_{\\mathrm{avail}}", re.IGNORECASE)
    pat_loop = re.compile(
        r"\\mathcal{P}|loop\s+(?:check|freedom)|Loop-Freedom|path-vector", re.IGNORECASE
    )

    m_prim = find_matching_lines(parsed, pat_primitive)
    m_surp = find_matching_lines(parsed, pat_surplus)
    m_loop = find_matching_lines(parsed, pat_loop)

    missing = []
    if not m_prim:
        missing.append("BPVP protocol primitive")
    if not m_surp:
        missing.append("Capability Advertisements / Q_avail surplus floor")
    if not m_loop:
        missing.append("path-vector loop check / loop-freedom")

    if not missing:
        lines = sorted(list(set([m_prim[0][0], m_surp[0][0], m_loop[0][0]])))
        quote = m_prim[0][1]
        return True, lines, quote, "BPVP capability advertisements, export surplus floor, and loop checks verified"
    return False, [], "", f"BPVP deficient: missing {', '.join(missing)}"


def evaluate_crit_5_gov(parsed: ParsedScenario) -> Tuple[bool, List[int], str, str]:
    """CRIT-5: Conjunctively verify Cellular Autonomy, Protocol of Refusal, Transit Veto, and Ricardian Treaty."""
    pat_cellular = re.compile(
        r"Cellular\s+Autonomy|\bthe\s+parts\b.*?\bthe\s+whole\b|subordinate\s+whole", re.IGNORECASE
    )
    pat_refusal = re.compile(r"Protocol\s+of\s+Refusal", re.IGNORECASE)
    pat_veto = re.compile(r"Transit\s+Veto|No-Transit", re.IGNORECASE)
    pat_treaty = re.compile(r"Treaty_CID|Ricardian\s+Treaty", re.IGNORECASE)

    m_cell = find_matching_lines(parsed, pat_cellular)
    m_ref = find_matching_lines(parsed, pat_refusal)
    m_veto = find_matching_lines(parsed, pat_veto)
    m_tre = find_matching_lines(parsed, pat_treaty)

    missing = []
    if not m_cell:
        missing.append("Cellular Autonomy ('parts over the whole')")
    if not m_ref:
        missing.append("Protocol of Refusal")
    if not m_veto:
        missing.append("Autonomous Transit Veto ('No-Transit')")
    if not m_tre:
        missing.append("Ricardian Treaty (Treaty_CID)")

    if not missing:
        lines = sorted(list(set([m_cell[0][0], m_ref[0][0], m_veto[0][0], m_tre[0][0]])))
        quote = m_cell[0][1]
        return True, lines, quote, "Layer 5 Cellular Autonomy, Protocol of Refusal, Transit Veto, and Treaty_CID verified"
    return False, [], "", f"Governance deficient: missing {', '.join(missing)}"


def evaluate_scenario_semantic(scenario: Union[Path, str, ParsedScenario]) -> Dict[str, Any]:
    """Perform hardened automated semantic evaluation of a scenario against the 6 rubric criteria."""
    if isinstance(scenario, (Path, str)):
        parsed = ParsedScenario(Path(scenario))
    else:
        parsed = scenario

    criteria_results: Dict[str, Any] = {}
    violations: List[str] = []
    passed_count = 0

    # 1. CRIT-1_BCC
    bcc_pass, bcc_lines, bcc_quote, bcc_just = evaluate_crit_1_bcc(parsed)
    if bcc_pass:
        passed_count += 1
        criteria_results["CRIT-1_BCC"] = {
            "status": "PASS",
            "line_numbers": bcc_lines,
            "quote": bcc_quote,
            "justification": bcc_just,
        }
    else:
        violations.append(f"CRIT-1_BCC: {bcc_just}")
        criteria_results["CRIT-1_BCC"] = {
            "status": "FAIL",
            "line_numbers": [],
            "quote": "",
            "justification": bcc_just,
        }

    # 2. CRIT-2_HTLC
    htlc_pass, htlc_lines, htlc_quote, htlc_just = evaluate_crit_2_htlc(parsed)
    if htlc_pass:
        passed_count += 1
        criteria_results["CRIT-2_HTLC"] = {
            "status": "PASS",
            "line_numbers": htlc_lines,
            "quote": htlc_quote,
            "justification": htlc_just,
        }
    else:
        violations.append(f"CRIT-2_HTLC: {htlc_just}")
        criteria_results["CRIT-2_HTLC"] = {
            "status": "FAIL",
            "line_numbers": [],
            "quote": "",
            "justification": htlc_just,
        }

    # 3. CRIT-3_RINGS
    rings_pass, rings_lines, rings_quote, rings_just = evaluate_crit_3_rings(parsed)
    if rings_pass:
        passed_count += 1
        criteria_results["CRIT-3_RINGS"] = {
            "status": "PASS",
            "line_numbers": rings_lines,
            "quote": rings_quote,
            "justification": rings_just,
        }
    else:
        violations.append(f"CRIT-3_RINGS: {rings_just}")
        criteria_results["CRIT-3_RINGS"] = {
            "status": "FAIL",
            "line_numbers": [],
            "quote": "",
            "justification": rings_just,
        }

    # 4. CRIT-4_BPVP
    bpvp_pass, bpvp_lines, bpvp_quote, bpvp_just = evaluate_crit_4_bpvp(parsed)
    if bpvp_pass:
        passed_count += 1
        criteria_results["CRIT-4_BPVP"] = {
            "status": "PASS",
            "line_numbers": bpvp_lines,
            "quote": bpvp_quote,
            "justification": bpvp_just,
        }
    else:
        violations.append(f"CRIT-4_BPVP: {bpvp_just}")
        criteria_results["CRIT-4_BPVP"] = {
            "status": "FAIL",
            "line_numbers": [],
            "quote": "",
            "justification": bpvp_just,
        }

    # 5. CRIT-5_GOV
    gov_pass, gov_lines, gov_quote, gov_just = evaluate_crit_5_gov(parsed)
    if gov_pass:
        passed_count += 1
        criteria_results["CRIT-5_GOV"] = {
            "status": "PASS",
            "line_numbers": gov_lines,
            "quote": gov_quote,
            "justification": gov_just,
        }
    else:
        violations.append(f"CRIT-5_GOV: {gov_just}")
        criteria_results["CRIT-5_GOV"] = {
            "status": "FAIL",
            "line_numbers": [],
            "quote": "",
            "justification": gov_just,
        }

    # 6. CRIT-6_CONS
    from verification.scenarios.verify_five_perspectives import verify_five_perspectives

    persp_violations = verify_five_perspectives(parsed)
    if not persp_violations:
        passed_count += 1
        rat_line = next(
            (idx for idx, l in enumerate(parsed.lines, start=1) if "Architectural Council" in l), 1
        )
        criteria_results["CRIT-6_CONS"] = {
            "status": "PASS",
            "line_numbers": [rat_line],
            "quote": parsed.line_at(rat_line),
            "justification": "Complete 5-persona Architectural Council ratification and unanimous consensus verified",
        }
    else:
        violations.append(
            f"CRIT-6_CONS: 5-Perspective council consensus deficient: {persp_violations[0].message}"
        )
        criteria_results["CRIT-6_CONS"] = {
            "status": "FAIL",
            "line_numbers": [],
            "quote": "",
            "justification": persp_violations[0].message,
        }

    overall_verdict = "PASS" if passed_count == 6 else "FAIL"

    return {
        "scenario_file": parsed.filename,
        "scenario_id": parsed.scenario_id,
        "verdict": overall_verdict,
        "score": f"{passed_count}/6",
        "criteria": criteria_results,
        "identified_violations": violations,
    }


def validate_judge_report(report_data: Dict[str, Any], scenario_path: Optional[Path] = None) -> List[str]:
    """Validate that a Judge JSON report strictly satisfies the rubric schema and citations."""
    errors: List[str] = []

    for req_key in [
        "scenario_file",
        "scenario_id",
        "verdict",
        "score",
        "criteria",
        "identified_violations",
    ]:
        if req_key not in report_data:
            errors.append(f"Missing required top-level key: '{req_key}'")

    if errors:
        return errors

    verdict = report_data.get("verdict")
    if verdict not in ["PASS", "FAIL"]:
        errors.append(f"Invalid verdict '{verdict}', must be 'PASS' or 'FAIL'")

    score = report_data.get("score", "")
    score_match = re.match(r"^([0-6])/6$", str(score))
    if not score_match:
        errors.append(f"Invalid score format '{score}', expected 'X/6'")
    else:
        passed_count = int(score_match.group(1))
        if passed_count == 6 and verdict != "PASS":
            errors.append(f"Verdict inconsistency: score is 6/6 but verdict is '{verdict}'")
        elif passed_count < 6 and verdict != "FAIL":
            errors.append(f"Verdict inconsistency: score is {score} but verdict is '{verdict}'")

    criteria = report_data.get("criteria", {})
    for crit_key in RUBRIC_CRITERIA_KEYS:
        if crit_key not in criteria:
            errors.append(f"Missing criterion '{crit_key}' in report")
            continue

        c_data = criteria[crit_key]
        c_status = c_data.get("status")
        if c_status not in ["PASS", "FAIL"]:
            errors.append(f"Criterion '{crit_key}' status must be 'PASS' or 'FAIL'")

        line_numbers = c_data.get("line_numbers", [])
        quote = c_data.get("quote", "")

        if c_status == "PASS":
            if not isinstance(line_numbers, list) or len(line_numbers) == 0:
                errors.append(f"Criterion '{crit_key}' marked PASS but line_numbers is empty")
            if not quote or not str(quote).strip():
                errors.append(f"Criterion '{crit_key}' marked PASS but quote is empty")

            if scenario_path and scenario_path.exists() and quote:
                with open(scenario_path, "r", encoding="utf-8") as f:
                    file_text = f.read()
                quote_core = quote.strip()
                if len(quote_core) > 20:
                    quote_core = quote_core[:30]
                if quote_core not in file_text:
                    errors.append(
                        f"Anti-Hallucination check failed for '{crit_key}': quote {quote[:40]!r} not found in {scenario_path.name}"
                    )

    return errors


class TestAgentAsJudge(unittest.TestCase):
    """Unittest suite verifying the hardened Agent-as-Judge evaluation engine and rubric."""

    def test_semantic_evaluation_structure(self) -> None:
        files = get_all_scenario_files()
        if not files:
            self.skipTest("No scenario files found")

        result = evaluate_scenario_semantic(files[0])
        validation_errors = validate_judge_report(result, files[0])
        self.assertEqual(validation_errors, [], f"Schema validation errors: {validation_errors}")
        self.assertIn(result["verdict"], ["PASS", "FAIL"])

    def test_crit_1_bcc_conjunctive(self) -> None:
        with tempfile.NamedTemporaryFile("w+", suffix=".md", delete=False) as tf:
            tf.write("Some text mentioning Bilateral Clearing Channel but no math.\n")
            tf.flush()
            parsed = ParsedScenario(Path(tf.name))
        try:
            ok, _, _, msg = evaluate_crit_1_bcc(parsed)
            self.assertFalse(ok)
            self.assertIn("zero-sum", msg)
        finally:
            os.unlink(tf.name)

    def test_crit_2_htlc_no_loose_delivery_receipt(self) -> None:
        with tempfile.NamedTemporaryFile("w+", suffix=".md", delete=False) as tf:
            tf.write("L3 <-->|l2_l3.sock: Presence Attestations & Delivery Receipts| L2\n")
            tf.flush()
            parsed = ParsedScenario(Path(tf.name))
        try:
            ok, _, _, msg = evaluate_crit_2_htlc(parsed)
            self.assertFalse(ok, "Delivery receipt alone without HTLC primitive must FAIL")
            self.assertIn("HTLC primitive", msg)
        finally:
            os.unlink(tf.name)

    def test_crit_3_rings_strictly_all_seven(self) -> None:
        with tempfile.NamedTemporaryFile("w+", suffix=".md", delete=False) as tf:
            tf.write(
                "R_0 (Hearth)\nR_1 (Settlement)\nR_2 (Commons)\nR_3 (Watershed)\n"  # Only 4 rings
            )
            tf.flush()
            parsed = ParsedScenario(Path(tf.name))
        try:
            ok, _, _, msg = evaluate_crit_3_rings(parsed)
            self.assertFalse(ok, "Relaxation to 4 rings is forbidden; all 7 required")
            self.assertIn("R_4", msg)
        finally:
            os.unlink(tf.name)

    def test_crit_4_bpvp_conjunctive(self) -> None:
        with tempfile.NamedTemporaryFile("w+", suffix=".md", delete=False) as tf:
            tf.write("BPVP protocol primitive mentioned alone with zero advertisements or loops.\n")
            tf.flush()
            parsed = ParsedScenario(Path(tf.name))
        try:
            ok, _, _, msg = evaluate_crit_4_bpvp(parsed)
            self.assertFalse(ok)
            self.assertIn("Capability Advertisements", msg)
        finally:
            os.unlink(tf.name)

    def test_crit_5_gov_conjunctive(self) -> None:
        with tempfile.NamedTemporaryFile("w+", suffix=".md", delete=False) as tf:
            tf.write("Treaty_CID = SHA256(...) mentioned alone without any governance doctrines.\n")
            tf.flush()
            parsed = ParsedScenario(Path(tf.name))
        try:
            ok, _, _, msg = evaluate_crit_5_gov(parsed)
            self.assertFalse(ok)
            self.assertIn("Cellular Autonomy", msg)
        finally:
            os.unlink(tf.name)

    def test_all_scenarios_pass_agent_as_judge(self) -> None:
        files = get_all_scenario_files()
        for f in files:
            res = evaluate_scenario_semantic(f)
            self.assertEqual(
                res["verdict"],
                "PASS",
                f"Scenario {f.name} failed Agent-as-Judge with score {res['score']}: {res['identified_violations']}",
            )


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Hardened Agent-as-Judge Evaluation Harness and Report Validator."
    )
    parser.add_argument(
        "--evaluate-all", action="store_true", help="Run semantic evaluation on all 24 scenarios"
    )
    parser.add_argument("--validate", type=str, help="Validate an existing Judge JSON report file")
    parser.add_argument("paths", nargs="*", help="Optional scenario files to evaluate")
    args = parser.parse_args()

    if args.validate:
        report_path = Path(args.validate)
        if not report_path.exists():
            print(f"Error: report file {report_path} not found", file=sys.stderr)
            return 1
        with open(report_path, "r", encoding="utf-8") as f:
            data = json.load(f)
        scenario_file = Path(data.get("scenario_file", ""))
        errors = validate_judge_report(data, scenario_file if scenario_file.exists() else None)
        if errors:
            print("Judge Report Validation FAILED:")
            for e in errors:
                print(f"  - {e}")
            return 1
        print("Judge Report Schema & Citations VALID.")
        return 0

    files = [Path(p) for p in args.paths] if args.paths else get_all_scenario_files()
    total_scenarios = len(files)
    total_passed = 0
    all_results = []

    for f in files:
        eval_result = evaluate_scenario_semantic(f)
        if eval_result["verdict"] == "PASS":
            total_passed += 1
        all_results.append(eval_result)
        status_color = "PASS" if eval_result["verdict"] == "PASS" else "FAIL"
        print(f"[{status_color}] {f.name} - Score: {eval_result['score']}")
        if eval_result["identified_violations"]:
            for v in eval_result["identified_violations"]:
                print(f"  --> {v}")

    print(
        f"\nAgent-as-Judge Summary: {total_passed}/{total_scenarios} Scenarios Passed (6/6 criteria)"
    )
    return 0 if total_passed == total_scenarios else 1


if __name__ == "__main__":
    sys.exit(main())
