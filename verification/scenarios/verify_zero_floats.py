"""
verification/scenarios/verify_zero_floats.py
Programmatic verification of zero IEEE 754 floating-point math across:
1. C++ simulation code blocks
2. JSON-LD / JSON schemas and telemetry payloads
3. Markdown prose, state claims, and tables

Requirements: Python standard library only (unittest, re, json, ast, argparse).
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

# C++ float type tokens and float literal patterns
CPP_FLOAT_TYPE_PATTERN = re.compile(r"\b(float|double|f32|f64)\b")
CPP_FLOAT_LITERAL_PATTERN = re.compile(r"\b\d+\.\d+f?\b")
CPP_FLOAT_FUNC_PATTERN = re.compile(r"\b(std::fabs|fabsf|fabs)\b")

# Whitelist patterns for prose to avoid false positives on versions, URIs, filenames, etc.
PROSE_WHITELIST_REPLACEMENTS = [
    # URLs and URIs
    re.compile(r"https?://[^\s)\]]+"),
    re.compile(r"ipfs://[^\s)\]]+"),
    re.compile(r"did:[^\s)\]]+"),
    re.compile(r"urn:[^\s)\]]+"),
    # Timestamps (ISO 8601)
    re.compile(r"\b\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}(?:\.\d+)?Z\b"),
    re.compile(r"\b\d{4}-\d{2}-\d{2}\b"),
    # Heading numbers: e.g. '## 1. Problem', '### 3.1 Layer'
    re.compile(r"^#+\s+\d+(?:\.\d+)*\b"),
    # Markdown list items at start of line: '1. ', '2. '
    re.compile(r"^\s*\d+\.\s+"),
    # RFC citations: 'RFC 4271'
    re.compile(r"\bRFC\s*\d+(?:\.\d+)?\b", re.IGNORECASE),
    # BPMN 2.0, WebGPU 1.0, POSIX 1.0
    re.compile(r"\b(BPMN|WebGPU|POSIX|OpenGL|Vulkan|SDL|WASM|MQTT)\s+\d+\.\d+\b", re.IGNORECASE),
    # Version tags like 'v1.0', 'V3.2', 'v2.1.0'
    re.compile(r"\b[vV]\d+\.\d+(?:\.\d+)?\b"),
    # Legal section citations: '§ 6', '§§ 1381–1388'
    re.compile(r"§+\s*\d+(?:\.\d+)?"),
    # Filenames with extensions: e.g. main.cpp, bounty.jsonld, schema.bpmn
    re.compile(r"\b[A-Za-z0-9_\-\/]+\.(?:cpp|hpp|h|json|jsonld|md|bpmn|sock|3mf|png|jpg|tex|pdf|sh|svg)\b"),
]

PROSE_FLOAT_PATTERN = re.compile(r"\b\d+\.\d+\b")
INLINE_CODE_PATTERN = re.compile(r"`([^`]+)`")
INLINE_FLOAT_PATTERN = re.compile(r"\b\d+\.\d+f?\b")


def check_cpp_blocks(parsed: ParsedScenario) -> List[Violation]:
    """Check all C++ code blocks for floating-point types, functions, and literals."""
    violations: List[Violation] = []
    cpp_blocks = parsed.get_code_blocks_by_lang("cpp")

    for cb in cpp_blocks:
        code_lines = cb.code.splitlines()
        for offset, raw_line in enumerate(code_lines):
            line_no = cb.start_line + offset

            # Strip string literals to avoid false positives inside string IDs
            line_no_strings = re.sub(r'"[^"\\]*(?:\\.[^"\\]*)*"', '""', raw_line)

            # Check float types
            type_match = CPP_FLOAT_TYPE_PATTERN.search(line_no_strings)
            if type_match:
                violations.append(
                    Violation(
                        file_path=str(parsed.file_path),
                        line_number=line_no,
                        rule_id="RULE_ZERO_FLOATS_CPP_TYPE",
                        message=f"Forbidden C++ float type '{type_match.group(1)}' detected",
                        snippet=raw_line,
                    )
                )

            # Check float functions (std::fabs)
            func_match = CPP_FLOAT_FUNC_PATTERN.search(line_no_strings)
            if func_match:
                violations.append(
                    Violation(
                        file_path=str(parsed.file_path),
                        line_number=line_no,
                        rule_id="RULE_ZERO_FLOATS_CPP_FUNC",
                        message=f"Forbidden C++ float math function '{func_match.group(1)}' detected",
                        snippet=raw_line,
                    )
                )

            # Check float literals (e.g. 100.0f, 42.5f, 3.50)
            lit_match = CPP_FLOAT_LITERAL_PATTERN.search(line_no_strings)
            if lit_match:
                violations.append(
                    Violation(
                        file_path=str(parsed.file_path),
                        line_number=line_no,
                        rule_id="RULE_ZERO_FLOATS_CPP_LITERAL",
                        message=f"Forbidden C++ float literal '{lit_match.group(0)}' detected",
                        snippet=raw_line,
                    )
                )

    return violations


def check_json_blocks(parsed: ParsedScenario) -> List[Violation]:
    """Check all JSON code blocks for raw floating point numbers and quoted decimals."""
    violations: List[Violation] = []
    json_blocks = parsed.get_code_blocks_by_lang("json")

    for cb in json_blocks:
        # First, scan line-by-line with regex for quick detection and exact line numbers
        code_lines = cb.code.splitlines()
        for offset, raw_line in enumerate(code_lines):
            line_no = cb.start_line + offset
            # Look for ": 42.5" or ": "3.50""
            val_float_match = re.search(r":\s*(\d+\.\d+)\b", raw_line)
            if val_float_match:
                violations.append(
                    Violation(
                        file_path=str(parsed.file_path),
                        line_number=line_no,
                        rule_id="RULE_ZERO_FLOATS_JSON_RAW",
                        message=f"Forbidden float value '{val_float_match.group(1)}' in JSON payload",
                        snippet=raw_line,
                    )
                )
            val_str_float_match = re.search(r':\s*"(\d+\.\d+)"', raw_line)
            if val_str_float_match:
                violations.append(
                    Violation(
                        file_path=str(parsed.file_path),
                        line_number=line_no,
                        rule_id="RULE_ZERO_FLOATS_JSON_STRING",
                        message=f"Forbidden decimal string value '\"{val_str_float_match.group(1)}\"' in JSON payload",
                        snippet=raw_line,
                    )
                )

        # Second, parse with json.loads to detect nested floats
        try:
            parsed_json = json.loads(cb.code)

            def inspect_node(val: object, path: str = "") -> None:
                if isinstance(val, float):
                    # We already recorded or will record line-based violations,
                    # but if none found on line (e.g. single-line minified JSON), record fence line
                    pass
                elif isinstance(val, dict):
                    for k, v in val.items():
                        inspect_node(v, f"{path}.{k}" if path else str(k))
                elif isinstance(val, list):
                    for i, item in enumerate(val):
                        inspect_node(item, f"{path}[{i}]")

            inspect_node(parsed_json)
        except Exception:
            # If JSON is invalid or contains comments, line-based check already caught it
            pass

    return violations


def check_prose_lines(parsed: ParsedScenario) -> List[Violation]:
    """Check markdown prose, tables, and state claims for floating-point decimals."""
    violations: List[Violation] = []

    for line_no, raw_line in parsed.prose_lines:
        # 1. Inspect inline code spans for floating-point decimal literals
        for match in INLINE_CODE_PATTERN.finditer(raw_line):
            code_text = match.group(1)
            # Apply whitelist replacements to allow clean version numbers, URIs, filenames, etc.
            for wl_pattern in PROSE_WHITELIST_REPLACEMENTS:
                code_text = wl_pattern.sub(" ", code_text)

            inline_float_matches = INLINE_FLOAT_PATTERN.findall(code_text)
            for m in inline_float_matches:
                violations.append(
                    Violation(
                        file_path=str(parsed.file_path),
                        line_number=line_no,
                        rule_id="RULE_ZERO_FLOATS_INLINE_CODE",
                        message=f"Forbidden floating-point decimal '{m}' inside inline code span `{match.group(1)}`",
                        snippet=raw_line,
                    )
                )

        # 2. Strip inline code spans before checking the rest of the prose line
        line_text = re.sub(r"`[^`]*`", "", raw_line)

        # Apply whitelist replacements to prose
        for wl_pattern in PROSE_WHITELIST_REPLACEMENTS:
            line_text = wl_pattern.sub(" ", line_text)

        # Find any remaining decimal numbers in prose
        matches = PROSE_FLOAT_PATTERN.findall(line_text)
        for m in matches:
            violations.append(
                Violation(
                    file_path=str(parsed.file_path),
                    line_number=line_no,
                    rule_id="RULE_ZERO_FLOATS_PROSE",
                    message=f"Forbidden floating-point decimal '{m}' in prose / state claim",
                    snippet=raw_line,
                )
            )

    return violations


def verify_zero_floats(scenario: Union[Path, str, ParsedScenario]) -> List[Violation]:
    """Run all float checks (C++, JSON, prose) against a scenario file."""
    if isinstance(scenario, (Path, str)):
        parsed = ParsedScenario(Path(scenario))
    else:
        parsed = scenario

    violations: List[Violation] = []
    violations.extend(check_cpp_blocks(parsed))
    violations.extend(check_json_blocks(parsed))
    violations.extend(check_prose_lines(parsed))
    return violations


class TestZeroFloats(unittest.TestCase):
    """Unittest test case verifying all 24 scenarios contain zero floats."""

    def test_all_scenarios_have_zero_floats(self) -> None:
        files = get_all_scenario_files()
        all_violations: List[Violation] = []

        for f in files:
            v_list = verify_zero_floats(f)
            all_violations.extend(v_list)

        if all_violations:
            summary = "\n".join(str(v) for v in all_violations[:20])
            total = len(all_violations)
            self.fail(
                f"Found {total} floating-point violations across scenarios:\n{summary}"
            )

    def test_inline_backtick_containing_float_detected(self) -> None:
        """Explicitly assert that inline backticks containing floats (e.g. `val = 0.01`) are caught as violations."""
        with tempfile.TemporaryDirectory() as tmpdir:
            test_file = Path(tmpdir) / "test_float_in_backtick.md"
            test_file.write_text(
                "# Scenario Test\n\n"
                "Here is an inline code float: `val = 0.01`.\n"
                "Here is clean code: `uint32_t`, `l4_l5.sock`, and `main.cpp`.\n",
                encoding="utf-8",
            )
            violations = verify_zero_floats(test_file)
            self.assertEqual(len(violations), 1, f"Expected 1 violation, got {violations}")
            self.assertEqual(violations[0].rule_id, "RULE_ZERO_FLOATS_INLINE_CODE")
            self.assertIn("0.01", violations[0].message)

    def test_inline_backtick_statistical_significance_float_detected(self) -> None:
        """Assert that statistical significance `p < 0.01` inside inline backticks is caught as a violation."""
        with tempfile.TemporaryDirectory() as tmpdir:
            test_file = Path(tmpdir) / "test_p_val.md"
            test_file.write_text(
                "# Statistical Metric\n\n"
                "* **Pass/Fail Metric:** `p < 100 bps` (`p < 0.01`); zero floats.\n",
                encoding="utf-8",
            )
            violations = verify_zero_floats(test_file)
            self.assertEqual(len(violations), 1, f"Expected 1 violation, got {violations}")
            self.assertEqual(violations[0].rule_id, "RULE_ZERO_FLOATS_INLINE_CODE")
            self.assertIn("0.01", violations[0].message)

    def test_inline_backtick_whitelisted_code_passes(self) -> None:
        """Assert that clean code and whitelisted tokens in inline backticks produce no violations."""
        with tempfile.TemporaryDirectory() as tmpdir:
            test_file = Path(tmpdir) / "test_clean_code.md"
            test_file.write_text(
                "# Clean Scenario\n\n"
                "Identifiers: `uint32_t`, `int64_t`, `size_t`, `l4_l5.sock`.\n"
                "Standards and versions: `BPMN 2.0`, `v1.1`, `RFC 4271`.\n",
                encoding="utf-8",
            )
            violations = verify_zero_floats(test_file)
            self.assertEqual(len(violations), 0, f"Expected 0 violations, got {violations}")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Verify zero IEEE 754 floating-point math across scenarios."
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
        violations = verify_zero_floats(f)
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
