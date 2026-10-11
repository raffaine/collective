"""
verification/scenarios/common.py
Shared parsing utilities and data structures for the Sovereign Stack
24-Scenario Verification Suite.
Python standard library only (no external dependencies).
"""

import os
import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple, NamedTuple, Any

PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
SCENARIOS_DIR = PROJECT_ROOT / "docs" / "scenarios"

# The 24 Canonical Scenarios in docs/scenarios/
SCENARIO_CATALOG: Dict[str, str] = {
    "alpha_fabrication.md": "SCN-ALPHA-FAB",
    "beta_transit.md": "SCN-BETA-TRANSIT",
    "gamma_kitchen.md": "SCN-GAMMA-KITCHEN",
    "delta_childcare.md": "SCN-DELTA-CHILD",
    "epsilon_coworking.md": "SCN-EPSILON-COWORK",
    "zeta_lodging.md": "SCN-ZETA-LODGE",
    "eta_coppice.md": "SCN-ETA-COPPICE",
    "theta_foundry.md": "SCN-THETA-FOUNDRY",
    "iota_epistemic.md": "SCN-IOTA-EPISTEMIC",
    "kappa_apprenticeship.md": "SCN-KAPPA-APPRENTICE",
    "lambda_cultural.md": "SCN-LAMBDA-CULTURE",
    "mu_extraction.md": "SCN-MU-EXTRACTION",
    "nu_fashion.md": "SCN-NU-FASHION",
    "xi_architecture.md": "SCN-XI-ARCH",
    "omicron_kinship.md": "SCN-OMICRON-KIN",
    "pi_guild.md": "SCN-PI-GUILD",
    "rho_adversarial.md": "SCN-RHO-ADVERSARY",
    "sigma_burnout.md": "SCN-SIGMA-BURNOUT",
    "tau_genesis.md": "SCN-TAU-GENESIS",
    "upsilon_shock.md": "SCN-UPSILON-SHOCK",
    "phi_elder.md": "SCN-PHI-ELDER",
    "chi_paramedic.md": "SCN-CHI-PARAMEDIC",
    "psi_mediation.md": "SCN-PSI-MEDIATION",
    "omega_decoupling.md": "SCN-OMEGA-DECOUPLE",
}


class Violation:
    """Represents a single rule violation in a scenario document."""

    def __init__(
        self,
        file_path: str,
        line_number: int,
        rule_id: str,
        message: str,
        snippet: str = "",
    ):
        self.file_path = str(file_path)
        self.line_number = int(line_number)
        self.rule_id = str(rule_id)
        self.message = str(message)
        self.snippet = str(snippet).strip()

    def to_dict(self) -> Dict[str, Any]:
        return {
            "file": os.path.basename(self.file_path),
            "file_path": self.file_path,
            "line_number": self.line_number,
            "rule_id": self.rule_id,
            "message": self.message,
            "snippet": self.snippet,
        }

    def __repr__(self) -> str:
        base = os.path.basename(self.file_path)
        return (
            f"[{self.rule_id}] {base}:{self.line_number} - {self.message} "
            f"(snippet: {self.snippet[:60]!r})"
        )


class CodeBlock(NamedTuple):
    lang: str
    code: str
    start_line: int  # 1-indexed line of first code statement
    fence_start_line: int  # 1-indexed line of opening ```
    fence_end_line: int  # 1-indexed line of closing ```


class ParsedScenario:
    """Parsed representation of a markdown scenario file."""

    def __init__(self, file_path: Path):
        self.file_path = file_path
        self.filename = file_path.name
        self.scenario_id = SCENARIO_CATALOG.get(self.filename, "SCN-UNKNOWN")
        with open(file_path, "r", encoding="utf-8") as f:
            self.raw_content = f.read()

        self.lines: List[str] = self.raw_content.splitlines()
        self.code_blocks: List[CodeBlock] = []
        self.prose_lines: List[Tuple[int, str]] = []  # (1-indexed line_no, text)
        self._parse()

    def _parse(self) -> None:
        in_code_block = False
        current_lang = ""
        current_block_lines: List[str] = []
        fence_start_line = 0
        start_line = 0

        for idx, line in enumerate(self.lines, start=1):
            stripped = line.strip()
            if stripped.startswith("```"):
                if not in_code_block:
                    in_code_block = True
                    fence_start_line = idx
                    current_lang = stripped[3:].strip().lower()
                    current_block_lines = []
                    start_line = idx + 1
                else:
                    # Closing fence
                    in_code_block = False
                    code_text = "\n".join(current_block_lines)
                    self.code_blocks.append(
                        CodeBlock(
                            lang=current_lang,
                            code=code_text,
                            start_line=start_line,
                            fence_start_line=fence_start_line,
                            fence_end_line=idx,
                        )
                    )
                    current_lang = ""
                    current_block_lines = []
            else:
                if in_code_block:
                    current_block_lines.append(line)
                else:
                    self.prose_lines.append((idx, line))

    def get_code_blocks_by_lang(self, lang: str) -> List[CodeBlock]:
        """Return all code blocks matching the specified language tag."""
        target = lang.lower().strip()
        return [cb for cb in self.code_blocks if cb.lang == target]

    def line_at(self, line_number: int) -> str:
        """Return 1-indexed line string."""
        if 1 <= line_number <= len(self.lines):
            return self.lines[line_number - 1]
        return ""


def get_all_scenario_files(scenarios_dir: Optional[Path] = None) -> List[Path]:
    """Retrieve all 24 scenario markdown files sorted canonically."""
    target_dir = scenarios_dir or SCENARIOS_DIR
    if not target_dir.exists():
        raise FileNotFoundError(f"Scenarios directory not found: {target_dir}")

    files = sorted(target_dir.glob("*.md"))
    # Prioritize catalog ordering if matched
    catalog_order = list(SCENARIO_CATALOG.keys())
    ordered = []
    remaining = []
    for f in files:
        if f.name in catalog_order:
            ordered.append(f)
        else:
            remaining.append(f)

    ordered.sort(key=lambda p: catalog_order.index(p.name) if p.name in catalog_order else 999)
    return ordered + remaining
