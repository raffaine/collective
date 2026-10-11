"""
verification/scenarios/verify_currency_token_purity.py
Programmatic verification of currency and token purity across all 24 scenarios:
1. Zero fiat currency references (USD, EUR, GBP, fiat conversion, etc.)
2. Zero dollar amounts ($30, $60, $1.20, \\$50, etc.)
3. Zero global token terminology ("Value Token", "Value Tokens", "ValueToken",
   "global token", "universal ledger", etc.)
4. Zero financialization of domestic care / human reproductive labor.

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

# Global tokens pattern: matches Value Token(s) or ValueToken unconditionally,
# or generic global tokens/ledgers
VALUE_TOKEN_PATTERN = re.compile(r"\b(Value\s*Token(?:s)?|ValueToken)\b", re.IGNORECASE)
GENERIC_GLOBAL_TOKEN_PATTERN = re.compile(
    r"\b(global\s+token(?:s)?|global\s+currency|universal\s+token(?:s)?|universal\s+ledger|global\s+ledger)\b",
    re.IGNORECASE,
)
NEGATION_PREFIXES = ("zero", "no", "ban on", "without", "prohibits", "prohibiting")

# Fiat currency keywords
FIAT_KEYWORD_PATTERN = re.compile(
    r"\b(USD|EUR|GBP|fiat\s+conversion|fiat\s+pricing|fiat-to-token)\b",
    re.IGNORECASE,
)

# Escaped dollar amounts: e.g. \$50, \$100, \$0.00
ESCAPED_DOLLAR_PATTERN = re.compile(r"\\\$[\s]*\d+(?:\.\d+)?")

# Domestic care financialization keywords
CARE_FINANCIALIZATION_PATTERN = re.compile(
    r"\b(Dutch\s+auction(?:\s+for\s+chores)?|chore\s+bounty|care\s+bounty|"
    r"dynamic\s+token\s+escalation\s+for\s+chores|monetiz(?:e|ing|ation)\s+of\s+childcare|"
    r"financializ(?:e|ing|ation)\s+of\s+care)\b",
    re.IGNORECASE,
)


def check_dollar_amounts(line: str) -> List[str]:
    """Find all fiat dollar amounts in a line, ignoring legitimate LaTeX math."""
    # 1. Escaped dollar amounts are always fiat references in markdown
    escaped = list(ESCAPED_DOLLAR_PATTERN.findall(line))

    # Mask escaped dollars so they do not interfere with other checks
    line_clean = ESCAPED_DOLLAR_PATTERN.sub(" " * 5, line)

    # 2. Mask balanced LaTeX math expressions containing math symbols or letters
    def mask_math(m: re.Match) -> str:
        inner = m.group(1)
        # If the math expression mentions tokens or USD, do NOT mask it
        if "token" in inner.lower() or "usd" in inner.lower():
            return m.group(0)
        if any(op in inner for op in ["^", "_", "\\", "%", "=", ">", "<"]):
            return " " * len(m.group(0))
        return m.group(0)

    masked = re.sub(r"(?<!\\)\$(.+?)(?<!\\)\$", mask_math, line_clean)
    raw_dollars = list(re.findall(r"\$\s*\d+(?:\.\d+)?", masked))
    return escaped + raw_dollars


def verify_currency_token_purity(scenario: Union[Path, str, ParsedScenario]) -> List[Violation]:
    """Verify scenario has zero global tokens, zero fiat currencies, zero dollar amounts."""
    if isinstance(scenario, (Path, str)):
        parsed = ParsedScenario(Path(scenario))
    else:
        parsed = scenario

    violations: List[Violation] = []

    for idx, raw_line in enumerate(parsed.lines, start=1):
        # 1. Check Value Tokens
        vt_matches = list(VALUE_TOKEN_PATTERN.finditer(raw_line))
        for m in vt_matches:
            violations.append(
                Violation(
                    file_path=str(parsed.file_path),
                    line_number=idx,
                    rule_id="RULE_NO_VALUE_TOKENS",
                    message=f"Forbidden Value Token reference '{m.group(0)}' detected",
                    snippet=raw_line,
                )
            )

        # 2. Check Generic Global Tokens (unless preceded by negation word like 'zero', 'no')
        for m in GENERIC_GLOBAL_TOKEN_PATTERN.finditer(raw_line):
            preceding_text = raw_line[: m.start()].strip().lower()
            if any(preceding_text.endswith(neg) for neg in NEGATION_PREFIXES):
                continue
            violations.append(
                Violation(
                    file_path=str(parsed.file_path),
                    line_number=idx,
                    rule_id="RULE_NO_GLOBAL_TOKENS",
                    message=f"Forbidden global token reference '{m.group(0)}' detected",
                    snippet=raw_line,
                )
            )

        # 3. Check Fiat Currency Keywords
        fiat_matches = list(FIAT_KEYWORD_PATTERN.finditer(raw_line))
        for m in fiat_matches:
            violations.append(
                Violation(
                    file_path=str(parsed.file_path),
                    line_number=idx,
                    rule_id="RULE_NO_FIAT_CURRENCY",
                    message=f"Forbidden fiat currency keyword '{m.group(0)}' detected",
                    snippet=raw_line,
                )
            )

        # 4. Check Dollar Amounts
        dollar_matches = check_dollar_amounts(raw_line)
        for damt in dollar_matches:
            violations.append(
                Violation(
                    file_path=str(parsed.file_path),
                    line_number=idx,
                    rule_id="RULE_NO_DOLLAR_AMOUNT",
                    message=f"Forbidden fiat dollar figure '{damt}' detected",
                    snippet=raw_line,
                )
            )

        # 5. Check Care Financialization
        care_matches = list(CARE_FINANCIALIZATION_PATTERN.finditer(raw_line))
        for cm in care_matches:
            violations.append(
                Violation(
                    file_path=str(parsed.file_path),
                    line_number=idx,
                    rule_id="RULE_NO_CARE_FINANCIALIZATION",
                    message=f"Forbidden domestic care financialization '{cm.group(0)}' detected",
                    snippet=raw_line,
                )
            )

    return violations


class TestCurrencyTokenPurity(unittest.TestCase):
    """Unittest test case verifying all 24 scenarios have zero fiat and zero global tokens."""

    def test_all_scenarios_have_zero_fiat_and_global_tokens(self) -> None:
        files = get_all_scenario_files()
        all_violations: List[Violation] = []

        for f in files:
            v_list = verify_currency_token_purity(f)
            all_violations.extend(v_list)

        if all_violations:
            summary = "\n".join(str(v) for v in all_violations[:20])
            total = len(all_violations)
            self.fail(
                f"Found {total} currency / token purity violations across scenarios:\n{summary}"
            )


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Verify zero fiat currency, zero dollar figures, and zero global tokens."
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
        violations = verify_currency_token_purity(f)
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
