#!/usr/bin/env python3
"""
Empirical Adversarial Validation Suite for The Sovereign Stack
Chapter 6 (Layer 5: Governance) and Full Volume Compilation

Validates:
1. LaTeX Label and Reference Resolution across main.tex and all chapters (100% resolution assertion)
2. Downstream Label Mapping and Cross-Chapter Reference Integrity
3. Citation Key Resolution against BibTeX databases (100% resolution assertion)
4. LaTeX Source Structural Integrity (balanced delimiters, environments, TikZ paths)
5. Compilation Pipeline Execution and main.log anomaly analysis (errors, broken macros, overfull boxes)
6. Status Documents (doc/book_collective/vol1_ch6_federation_status.md and workspace root copy)
"""

import os
import re
import subprocess
import hashlib
import json
import sys
from pathlib import Path

WORKSPACE = Path("/Users/raffaine/dev/collective")
BOOK_DIR = WORKSPACE / "doc/book_collective"
CHAPTERS_DIR = BOOK_DIR / "chapters"
MAIN_TEX = BOOK_DIR / "main.tex"
BIB_FILES = [BOOK_DIR / "collective.bib", BOOK_DIR / "references.bib"]

DOWNSTREAM_LABELS = [
    "tab:l5-instruments",
    "sec:l5-refusal",
    "sec:l5-memory",
    "sec:l5-neighbours",
    "sec:l5-sanctions",
    "sec:l5-dueprocess",
    "sec:l5-tiers",
    "sec:l5-matching",
]

def strip_latex_comments(content: str) -> str:
    """Strip out comments, keeping escaped percent signs."""
    lines = content.splitlines()
    clean_lines = []
    for line in lines:
        cleaned = re.sub(r'(?<!\\)%.*$', '', line)
        clean_lines.append(cleaned)
    return "\n".join(clean_lines)

def get_tex_files():
    """Extract all active TeX files included in main.tex in order."""
    content = MAIN_TEX.read_text(encoding="utf-8")
    clean = strip_latex_comments(content)
    inputs = re.findall(r'\\input\{([^}]+)\}', clean)
    tex_files = [MAIN_TEX]
    for inp in inputs:
        inp_path = inp if inp.endswith(".tex") else inp + ".tex"
        full_path = BOOK_DIR / inp_path
        if full_path.exists():
            tex_files.append(full_path)
        else:
            print(f"WARNING: Included file not found: {full_path}")
    return tex_files

def parse_labels_and_refs(tex_files):
    """Parse all label definitions and references across TeX files."""
    defined_labels = {} # label -> list of (file, line_num)
    referenced_labels = [] # list of (label, file, line_num, cmd)

    label_pat = re.compile(r'\\label\{([^}]+)\}')
    ref_pat = re.compile(r'\\(ref|pageref|autoref|eqref|subref)\{([^}]+)\}')

    for fpath in tex_files:
        lines = fpath.read_text(encoding="utf-8").splitlines()
        for idx, line in enumerate(lines, 1):
            clean = re.sub(r'(?<!\\)%.*$', '', line)
            
            for m in label_pat.finditer(clean):
                lbl = m.group(1).strip()
                if lbl not in defined_labels:
                    defined_labels[lbl] = []
                defined_labels[lbl].append((str(fpath.name), idx))
                
            for m in ref_pat.finditer(clean):
                cmd = m.group(1)
                lbl = m.group(2).strip()
                referenced_labels.append((lbl, str(fpath.name), idx, cmd))

    return defined_labels, referenced_labels

def parse_bib_keys():
    """Parse all keys in collective.bib and references.bib."""
    bib_keys = set()
    key_pat = re.compile(r'@\w+\s*\{\s*([^,\s]+)\s*,')
    for bfile in BIB_FILES:
        if bfile.exists():
            content = bfile.read_text(encoding="utf-8")
            for m in key_pat.finditer(content):
                bib_keys.add(m.group(1).strip())
    return bib_keys

def parse_citations(tex_files):
    """Parse all citation commands and keys across TeX files."""
    cite_pat = re.compile(r'\\(cite|parencite|textcite|autocite|citeauthor|citeyear|footcite|nocite)(?:\[[^\]]*\])?(?:\[[^\]]*\])?\{([^}]+)\}')
    citations = [] # list of (key, file, line_num, cmd)

    for fpath in tex_files:
        lines = fpath.read_text(encoding="utf-8").splitlines()
        for idx, line in enumerate(lines, 1):
            clean = re.sub(r'(?<!\\)%.*$', '', line)
            for m in cite_pat.finditer(clean):
                cmd = m.group(1)
                keys_str = m.group(2)
                for k in keys_str.split(','):
                    key = k.strip()
                    if key:
                        citations.append((key, str(fpath.name), idx, cmd))
    return citations

def check_downstream_labels(defined_labels, referenced_labels):
    """Verify all downstream labels specifically."""
    results = {}
    for lbl in DOWNSTREAM_LABELS:
        is_defined = lbl in defined_labels
        def_locations = defined_labels.get(lbl, [])
        references = [r for r in referenced_labels if r[0] == lbl]
        results[lbl] = {
            "defined": is_defined,
            "def_locations": def_locations,
            "defined_in_ch6": any("06_layer_5_governance" in loc[0] for loc in def_locations),
            "ref_count": len(references),
            "references": [(r[1], r[2]) for r in references]
        }
    return results

def check_latex_syntax(file_path):
    """Check balanced braces, environments, math delimiters in a file."""
    content = file_path.read_text(encoding="utf-8")
    lines = content.splitlines()
    issues = []

    # Check balanced curly braces line-by-line / block (accounting for escapes and comments)
    brace_depth = 0
    in_verbatim = False
    for idx, line in enumerate(lines, 1):
        clean = re.sub(r'(?<!\\)%.*$', '', line)
        if '\\begin{verbatim}' in clean or '\\begin{lstlisting}' in clean:
            in_verbatim = True
            continue
        if '\\end{verbatim}' in clean or '\\end{lstlisting}' in clean:
            in_verbatim = False
            continue
        if in_verbatim:
            continue
        
        # Check unescaped braces
        for i, char in enumerate(clean):
            if char == '{' and (i == 0 or clean[i-1] != '\\'):
                brace_depth += 1
            elif char == '}' and (i == 0 or clean[i-1] != '\\'):
                brace_depth -= 1
                if brace_depth < 0:
                    issues.append(f"Line {idx}: Unmatched closing brace '}}'")
                    brace_depth = 0

    if brace_depth > 0:
        issues.append(f"End of file: {brace_depth} unclosed opening braces '{{'")

    # Check balanced environments
    env_stack = []
    begin_pat = re.compile(r'\\begin\{([^}]+)\}')
    end_pat = re.compile(r'\\end\{([^}]+)\}')
    for idx, line in enumerate(lines, 1):
        clean = re.sub(r'(?<!\\)%.*$', '', line)
        begins = [(m.start(), m.group(1)) for m in begin_pat.finditer(clean)]
        ends = [(m.start(), m.group(1)) for m in end_pat.finditer(clean)]
        tokens = sorted([('begin', name, pos) for pos, name in begins] + [('end', name, pos) for pos, name in ends], key=lambda x: x[2])
        for action, name, _ in tokens:
            if action == 'begin':
                env_stack.append((name, idx))
            else:
                if not env_stack:
                    issues.append(f"Line {idx}: \\end{{{name}}} without matching \\begin")
                else:
                    top_name, top_idx = env_stack.pop()
                    if top_name != name:
                        issues.append(f"Line {idx}: Mismatched \\end{{{name}}} (expected \\end{{{top_name}}} opened at line {top_idx})")

    for env, lidx in env_stack:
        issues.append(f"Unclosed \\begin{{{env}}} from line {lidx}")

    return issues

def verify_compilation():
    """Execute full compilation sequence and parse output & logs."""
    commands = [
        ["pdflatex", "-interaction=nonstopmode", "main.tex"],
        ["bibtex", "main"],
        ["pdflatex", "-interaction=nonstopmode", "main.tex"],
        ["pdflatex", "-interaction=nonstopmode", "main.tex"],
    ]

    exec_results = []
    for cmd in commands:
        res = subprocess.run(cmd, cwd=BOOK_DIR, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        exec_results.append({
            "command": " ".join(cmd),
            "returncode": res.returncode,
            "stdout_tail": res.stdout[-1000:] if res.stdout else "",
            "stderr": res.stderr
        })
        if res.returncode != 0:
            print(f"Execution failed on: {' '.join(cmd)}")
            return False, exec_results

    return True, exec_results

def parse_main_log():
    """Analyze main.log for warnings, fatal errors, undefined references, and overfull boxes."""
    log_file = BOOK_DIR / "main.log"
    if not log_file.exists():
        return {"error": "main.log does not exist"}

    log_text = log_file.read_text(encoding="utf-8", errors="replace")

    # Fatal / Error lines
    error_lines = [line for line in log_text.splitlines() if line.startswith("! ")]

    # Undefined references
    undef_ref_pat = re.compile(r'LaTeX Warning: Reference `([^`\']+)\' on page (\d+) undefined')
    undef_refs = undef_ref_pat.findall(log_text)

    # Undefined citations
    undef_cite_pat = re.compile(r'LaTeX Warning: Citation `([^`\']+)\' on page (\d+) undefined')
    undef_cites = undef_cite_pat.findall(log_text)

    # General warning strings
    has_undef_refs_warning = "LaTeX Warning: There were undefined references." in log_text
    has_multiply_defined_warning = "LaTeX Warning: There were multiply-defined labels." in log_text

    # Overfull hbox
    overfull_hbox_pat = re.compile(r'Overfull \\hbox \(([0-9.]+)pt too wide\) in paragraph at lines (\d+)--(\d+)')
    overfull_hboxes = overfull_hbox_pat.findall(log_text)

    # Overfull vbox
    overfull_vbox_pat = re.compile(r'Overfull \\vbox \(([0-9.]+)pt too wide\)')
    overfull_vboxes = overfull_vbox_pat.findall(log_text)

    # Output pages
    pages_match = re.search(r'Output written on main\.pdf \((\d+) pages', log_text)
    page_count = int(pages_match.group(1)) if pages_match else 0

    return {
        "error_lines": error_lines,
        "undefined_references": undef_refs,
        "undefined_citations": undef_cites,
        "has_undef_refs_warning": has_undef_refs_warning,
        "has_multiply_defined_warning": has_multiply_defined_warning,
        "overfull_hboxes": [{"pts_too_wide": float(m[0]), "lines": f"{m[1]}--{m[2]}"} for m in overfull_hboxes],
        "overfull_vboxes": [float(m) for m in overfull_vboxes],
        "page_count": page_count,
    }

def verify_status_documents():
    """Verify both copies of vol1_ch6_federation_status.md exist and match acceptance criteria."""
    doc_copy = BOOK_DIR / "vol1_ch6_federation_status.md"
    root_copy = WORKSPACE / "vol1_ch6_federation_status.md"

    results = {
        "doc_exists": doc_copy.exists(),
        "root_exists": root_copy.exists(),
        "sizes": {},
        "hashes": {},
        "identical": False,
        "criteria_checks": {}
    }

    if doc_copy.exists():
        d_bytes = doc_copy.read_bytes()
        results["sizes"]["doc_copy"] = len(d_bytes)
        results["hashes"]["doc_copy"] = hashlib.sha256(d_bytes).hexdigest()

    if root_copy.exists():
        r_bytes = root_copy.read_bytes()
        results["sizes"]["root_copy"] = len(r_bytes)
        results["hashes"]["root_copy"] = hashlib.sha256(r_bytes).hexdigest()

    if doc_copy.exists() and root_copy.exists():
        results["identical"] = (results["hashes"]["doc_copy"] == results["hashes"]["root_copy"])

    # Verify content criteria
    content = doc_copy.read_text(encoding="utf-8") if doc_copy.exists() else ""
    required_keywords = [
        "Executive Summary",
        "Council Routing",
        "Bioregional Path-Vector Protocol",
        "BPVP",
        "FRRP",
        "Capability Advertisements",
        "Loop-Freedom Invariant",
        "Thermodynamic Path Attenuation",
        "Dynamic Request Cascading",
        "Bilateral Clearing Channels",
        "Council Cybernetics",
        "Viable System Model",
        "VSM",
        "Ashby's Law",
        "Requisite Variety",
        "Elinor Ostrom",
        "Council Stack",
        "col-kmsd",
        "Council Narrative",
        "The Republic of Autonomous Nodes",
        "Cross-Chapter Contract",
        "tab:l5-instruments",
        "sec:l5-refusal",
        "sec:l5-memory",
        "sec:l5-neighbours",
        "sec:l5-sanctions",
        "sec:l5-dueprocess",
        "sec:l5-tiers",
        "sec:l5-matching",
    ]

    for kw in required_keywords:
        results["criteria_checks"][kw] = kw in content

    results["all_criteria_met"] = all(results["criteria_checks"].values())
    return results

def main():
    print("=" * 80)
    print("EMPIRICAL ADVERSARIAL STRESS TEST & VALIDATION SUITE")
    print("Target: Chapter 6 (Layer 5: Governance) & Full Book Compilation")
    print("=" * 80)

    # 1. Parse TeX Files, Labels, and References
    print("\n[Step 1] Parsing TeX files, labels, and references...")
    tex_files = get_tex_files()
    print(f"Found {len(tex_files)} active TeX files included in main.tex")

    defined_labels, referenced_labels = parse_labels_and_refs(tex_files)
    print(f"Total defined labels across volume: {len(defined_labels)}")
    print(f"Total reference calls across volume: {len(referenced_labels)}")

    # Check for duplicate labels
    duplicates = {lbl: locs for lbl, locs in defined_labels.items() if len(locs) > 1}
    if duplicates:
        print(f"FAIL: Multiply-defined labels found: {duplicates}")
    else:
        print("PASS: Zero duplicate labels across volume.")

    # Check for undefined references
    missing_refs = []
    for lbl, fpath, line, cmd in referenced_labels:
        if lbl not in defined_labels:
            missing_refs.append((lbl, fpath, line, cmd))

    if missing_refs:
        print(f"FAIL: {len(missing_refs)} undefined references found:")
        for r in missing_refs:
            print(f"  - \\{r[3]}{{{r[0]}}} at {r[1]}:{r[2]}")
    else:
        print("PASS: 100% of referenced labels are defined! ZERO undefined references.")

    # 2. Downstream Label Verification
    print("\n[Step 2] Verifying 8 mandatory downstream labels in Chapter 6...")
    downstream_status = check_downstream_labels(defined_labels, referenced_labels)
    all_downstream_ok = True
    for lbl, info in downstream_status.items():
        ok = info["defined"] and info["defined_in_ch6"]
        if not ok:
            all_downstream_ok = False
        status_str = "OK" if ok else "FAIL"
        print(f"  [{status_str}] {lbl}: defined={info['defined']}, in_ch6={info['defined_in_ch6']}, references={info['ref_count']}")
        for ref_file, ref_line in info["references"]:
            print(f"        <- referenced by {ref_file}:{ref_line}")

    if all_downstream_ok:
        print("PASS: All 8 downstream labels are verified in Chapter 6 and active across chapters.")
    else:
        print("FAIL: One or more downstream labels failed verification.")

    # 3. BibTeX Citation Key Resolution
    print("\n[Step 3] Parsing BibTeX entries and citations...")
    bib_keys = parse_bib_keys()
    print(f"Total BibTeX keys across databases: {len(bib_keys)}")

    citations = parse_citations(tex_files)
    print(f"Total citation invocations across volume: {len(citations)}")

    missing_citations = []
    for key, fpath, line, cmd in citations:
        if key not in bib_keys:
            missing_citations.append((key, fpath, line, cmd))

    if missing_citations:
        print(f"FAIL: {len(missing_citations)} undefined citations found:")
        for c in missing_citations:
            print(f"  - \\{c[3]}{{{c[0]}}} at {c[1]}:{c[2]}")
    else:
        print("PASS: 100% of citations resolve to valid BibTeX entries! ZERO missing citations.")

    # 4. Source Syntax & Delimiter Balance of Chapter 6
    print("\n[Step 4] Checking structural syntax & delimiter integrity of Chapter 6...")
    ch6_path = CHAPTERS_DIR / "06_layer_5_governance.tex"
    syntax_issues = check_latex_syntax(ch6_path)
    if syntax_issues:
        print(f"FAIL: Structural syntax issues in Chapter 6: {len(syntax_issues)}")
        for iss in syntax_issues:
            print(f"  - {iss}")
    else:
        print("PASS: Chapter 6 has 100% balanced braces and environments.")

    # 5. Compilation & Log Analysis
    print("\n[Step 5] Executing full LaTeX compilation sequence (pdflatex -> bibtex -> pdflatex -> pdflatex)...")
    comp_ok, comp_results = verify_compilation()
    print(f"Compilation pipeline completed with status: {'SUCCESS' if comp_ok else 'FAILED'}")

    log_analysis = parse_main_log()
    print("\n--- main.log Analysis ---")
    print(f"Output PDF page count: {log_analysis.get('page_count')} pages")
    print(f"Fatal errors: {len(log_analysis.get('error_lines', []))}")
    print(f"Undefined references in log: {len(log_analysis.get('undefined_references', []))}")
    print(f"Undefined citations in log: {len(log_analysis.get('undefined_citations', []))}")
    print(f"Has general undefined references warning: {log_analysis.get('has_undef_refs_warning')}")
    print(f"Has multiply-defined warning: {log_analysis.get('has_multiply_defined_warning')}")
    print(f"Overfull \\hbox warnings: {len(log_analysis.get('overfull_hboxes', []))}")
    for box in log_analysis.get('overfull_hboxes', []):
        print(f"  - Overfull \\hbox ({box['pts_too_wide']}pt) lines {box['lines']}")
    print(f"Overfull \\vbox warnings: {len(log_analysis.get('overfull_vboxes', []))}")

    # 6. Status Documents Verification
    print("\n[Step 6] Verifying status documentation artifacts...")
    status_results = verify_status_documents()
    print(f"doc/book_collective/vol1_ch6_federation_status.md exists: {status_results['doc_exists']}")
    print(f"vol1_ch6_federation_status.md (root) exists: {status_results['root_exists']}")
    print(f"Files identical (SHA256 match): {status_results['identical']}")
    print(f"All required acceptance criteria sections present: {status_results['all_criteria_met']}")

    # Overall Verdict
    overall_pass = (
        len(missing_refs) == 0 and
        len(duplicates) == 0 and
        all_downstream_ok and
        len(missing_citations) == 0 and
        len(syntax_issues) == 0 and
        comp_ok and
        len(log_analysis.get("error_lines", [])) == 0 and
        len(log_analysis.get("undefined_references", [])) == 0 and
        len(log_analysis.get("undefined_citations", [])) == 0 and
        not log_analysis.get("has_undef_refs_warning") and
        not log_analysis.get("has_multiply_defined_warning") and
        status_results["doc_exists"] and
        status_results["root_exists"] and
        status_results["identical"] and
        status_results["all_criteria_met"]
    )

    verdict = "APPROVE" if overall_pass else "REJECT"
    print("\n" + "=" * 80)
    print(f"OVERALL EMPIRICAL CHALLENGE VERDICT: {verdict}")
    print("=" * 80)

    # Write results summary to json for reporting
    summary = {
        "verdict": verdict,
        "defined_labels_count": len(defined_labels),
        "referenced_labels_count": len(referenced_labels),
        "duplicate_labels": duplicates,
        "missing_references": missing_refs,
        "downstream_status": downstream_status,
        "bib_keys_count": len(bib_keys),
        "citations_count": len(citations),
        "missing_citations": missing_citations,
        "syntax_issues": syntax_issues,
        "compilation_success": comp_ok,
        "log_analysis": log_analysis,
        "status_results": status_results
    }
    
    output_path = WORKSPACE / "verification" / "ch6_validation_results.json"
    output_path.write_text(json.dumps(summary, indent=2), encoding="utf-8")
    print(f"\nDetailed verification data written to: {output_path}")

    return 0 if overall_pass else 1

if __name__ == "__main__":
    sys.exit(main())
