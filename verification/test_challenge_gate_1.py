#!/usr/bin/env python3
"""
Empirical Challenge Gate 1 Test Suite
Collective Documentation Layer 1 & 2 Integration Audit

Author: Challenger 1 (Empirical Drift & Consistency Challenger)
Scope:
1. Search target chapters (01, 09, 10, 12) for obsolete phrases and contradictory concepts.
2. Empirically verify all 38 fixes from vol1_l1_l2_impact_status.md were actually applied.
3. Empirically verify all statements in vol1_l1_l2_integration_status.md against actual files.
4. Comprehensive LaTeX syntax, environment balancing, and table column consistency checks.
"""

import os
import re
import sys
import json
import subprocess

REPO_ROOT = "/Users/raffaine/dev/collective"
DOC_DIR = os.path.join(REPO_ROOT, "doc/book_collective")
CHAPTERS_DIR = os.path.join(DOC_DIR, "chapters")

TARGET_CHAPTERS = [
    "01_anatomy_of_a_node.tex",
    "09_genesis_protocol.tex",
    "10_collective_bringup.tex",
    "12_prime_genesis.tex",
]

def load_chapter(name):
    path = os.path.join(CHAPTERS_DIR, name)
    with open(path, "r", encoding="utf-8") as f:
        return f.read()

def run_cmd(cmd, cwd=REPO_ROOT):
    p = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True, errors="replace")
    return p.returncode, p.stdout, p.stderr

def test_obsolete_phrases():
    print("=" * 70)
    print("TEST 1: Obsolete Phrases & Contradictory Concepts")
    print("=" * 70)
    results = {}
    
    # 1.1 "human actuator"
    # Should not be used affirmatively. In Ch 1, it is explicitly repudiated.
    ch1 = load_chapter("01_anatomy_of_a_node.tex")
    ch9 = load_chapter("09_genesis_protocol.tex")
    ch10 = load_chapter("10_collective_bringup.tex")
    ch12 = load_chapter("12_prime_genesis.tex")
    
    ch_map = {
        "01": ch1,
        "09": ch9,
        "10": ch10,
        "12": ch12,
    }
    
    affirmative_human_actuators = []
    for num, content in ch_map.items():
        matches = re.finditer(r'human actuator', content, re.IGNORECASE)
        for m in matches:
            # check context
            start = max(0, m.start() - 100)
            end = min(len(content), m.end() + 100)
            snippet = content[start:end]
            if "repudiates" in snippet or "mistakenly referred" in snippet:
                continue
            affirmative_human_actuators.append((num, snippet))
            
    results["affirmative_human_actuator_count"] = len(affirmative_human_actuators)
    results["human_actuator_in_ch9"] = "human actuator" in ch9.lower()
    results["human_actuator_in_ch10"] = "human actuator" in ch10.lower()
    results["human_actuator_in_ch12"] = "human actuator" in ch12.lower()
    
    print(f"  Affirmative 'human actuator' occurrences across 01, 09, 10, 12: {len(affirmative_human_actuators)}")
    assert len(affirmative_human_actuators) == 0, f"Found affirmative human actuator: {affirmative_human_actuators}"
    assert not results["human_actuator_in_ch9"], "Found human actuator in ch9"
    assert not results["human_actuator_in_ch10"], "Found human actuator in ch10"
    assert not results["human_actuator_in_ch12"], "Found human actuator in ch12"
    
    # 1.2 "100 kW photovoltaic" or "200 kWh LiFePO4" in Node 0 context
    inflation_matches = []
    for num, content in ch_map.items():
        # Match 100 kW or 200 kWh in any spacing/notation
        m = re.findall(r'(?:100\s*(?:~|\\,|\\ )?kW.*?photo|200\s*(?:~|\\,|\\ )?kWh.*?LiFePO4)', content, re.IGNORECASE)
        if m:
            inflation_matches.extend([(num, match) for match in m])
            
    results["sizing_inflation_matches"] = inflation_matches
    print(f"  Node 0 Sizing Inflation (100 kW PV / 200 kWh BESS) matches: {len(inflation_matches)}")
    assert len(inflation_matches) == 0, f"Found sizing inflation matches: {inflation_matches}"
    
    # Also verify canonical BOM numbers ARE present in Ch 12:
    assert "6.4" in ch12 and ("14.34" in ch12 or "14.34~kWh" in ch12 or "14.34kWh" in ch12), "Canonical BOM not found in Ch 12"
    assert "200A" in ch12 or "200A ATS" in ch12, "200A ATS not found in Ch 12"
    
    # 1.3 "procedural thermodynamic models switch off"
    models_switch_off_matches = []
    for num, content in ch_map.items():
        m = re.findall(r'procedural(?:\s+thermodynamic)?\s+models\s+switch\s+off', content, re.IGNORECASE)
        if m:
            models_switch_off_matches.extend([(num, match) for match in m])
            
    results["models_switch_off_matches"] = models_switch_off_matches
    print(f"  'models switch off' matches: {len(models_switch_off_matches)}")
    assert len(models_switch_off_matches) == 0, f"Found models switch off: {models_switch_off_matches}"
    # Verify that 'models do not switch off' IS present in Ch 10
    assert "models do not switch off" in ch10, "Explicit correction 'models do not switch off' missing in Ch 10"
    
    # 1.4 "col-telemetryd" assigned to Layer 1
    # Check if col-telemetryd is placed in Layer 1
    l1_telemetryd = []
    for num, content in ch_map.items():
        # match Layer 1 followed closely by col-telemetryd
        m = re.finditer(r'Layer\s*1[^.\n]*col-telemetryd|col-telemetryd[^.\n]*Layer\s*1', content, re.IGNORECASE)
        for match in m:
            l1_telemetryd.append((num, match.group(0)))
    results["l1_col_telemetryd_assignments"] = l1_telemetryd
    print(f"  col-telemetryd assigned to Layer 1 matches: {len(l1_telemetryd)}")
    assert len(l1_telemetryd) == 0, f"Found col-telemetryd assigned to Layer 1: {l1_telemetryd}"
    
    # Verify Ch 12 line 265 states: "Layer 1 contains zero software daemons"
    assert "zero software daemons" in ch12, "Layer 1 zero software daemons not found in Ch 12"
    
    # 1.5 Unescaped '&' and Table Column Consistency
    table_errors = []
    for ch_name in TARGET_CHAPTERS:
        content = load_chapter(ch_name)
        # Tabular environments
        # Match \begin{tabular}{spec} ... \end{tabular}
        pos = 0
        while True:
            beg_idx = content.find(r"\begin{tabular}", pos)
            if beg_idx == -1:
                break
            end_idx = content.find(r"\end{tabular}", beg_idx)
            if end_idx == -1:
                table_errors.append(f"{ch_name}: Unclosed tabular environment at {beg_idx}")
                break
            
            # extract spec using balanced braces
            spec_start = content.find("{", beg_idx + len(r"\begin{tabular}"))
            depth = 0
            spec_end = spec_start
            for i in range(spec_start, len(content)):
                if content[i] == '{':
                    depth += 1
                elif content[i] == '}':
                    depth -= 1
                    if depth == 0:
                        spec_end = i
                        break
            col_spec = content[spec_start+1:spec_end]
            body = content[spec_end+1:end_idx]
            
            # compute expected columns from spec
            spec_clean = re.sub(r'[pmb]\{[^}]*\}', 'c', col_spec)
            # expand *{N}{c} if present
            while '*' in spec_clean:
                m_star = re.search(r'\*\{(\d+)\}\{([^}]+)\}', spec_clean)
                if not m_star: break
                spec_clean = spec_clean[:m_star.start()] + (m_star.group(2) * int(m_star.group(1))) + spec_clean[m_star.end():]
            spec_clean = re.sub(r'[@|><\{\}\s]', '', spec_clean).strip()
            expected_cols = len(spec_clean)
            expected_amps = expected_cols - 1
            
            # audit rows
            rows = body.split(r"\\")
            for r_i, r in enumerate(rows):
                r_stripped = r.replace(r"\hline", "").replace(r"\toprule", "").replace(r"\midrule", "").replace(r"\bottomrule", "").strip()
                if not r_stripped:
                    continue
                # remove escaped \&
                r_no_escaped = r_stripped.replace(r"\&", "")
                actual_amps = r_no_escaped.count("&")
                if actual_amps != expected_amps:
                    table_errors.append(f"{ch_name}: Tabular row {r_i+1} has {actual_amps} ampersands, expected {expected_amps}. Content: {r_stripped[:60]}")
            pos = end_idx + len(r"\end{tabular}")
            
        # check unescaped & in prose
        lines = content.splitlines()
        in_tab = False
        in_align = False
        for l_num, line in enumerate(lines, 1):
            if r"\begin{tabular}" in line: in_tab = True
            if r"\end{tabular}" in line: in_tab = False; continue
            if any(env in line for env in [r"\begin{align}", r"\begin{split}", r"\begin{matrix}"]) : in_align = True
            if any(env in line for env in [r"\end{align}", r"\end{split}", r"\end{matrix}"]) : in_align = False; continue
            if not in_tab and not in_align:
                line_code = line.split("%")[0]
                # look for & not preceded by \
                unescaped = re.findall(r'(?<!\\)&', line_code)
                if unescaped:
                    table_errors.append(f"{ch_name}:{l_num}: Unescaped & in prose: {line[:60]}")
                    
    results["table_errors"] = table_errors
    print(f"  Table column and unescaped ampersand errors: {len(table_errors)}")
    for err in table_errors:
        print(f"    ! {err}")
    assert len(table_errors) == 0, f"Found table errors: {table_errors}"
    
    return results

def test_all_38_fixes():
    print("\n" + "=" * 70)
    print("TEST 2: Empirical Verification of All 38 Fixes in Source Files")
    print("=" * 70)
    
    ch1 = load_chapter("01_anatomy_of_a_node.tex")
    ch9 = load_chapter("09_genesis_protocol.tex")
    ch10 = load_chapter("10_collective_bringup.tex")
    ch12 = load_chapter("12_prime_genesis.tex")
    
    # Master dictionary of distinctive markers for every single one of the 38 fixes
    # Formulated from vol1_l1_l2_impact_status.md, PROJECT.md, and vol1_l1_l2_integration_status.md
    checks = [
        # Chapter 1 (Fixes 1-10)
        (1, "ISSUE-01-07", ch1, "autopoietic living cell", "Maturana & Varela autopoiesis in Sec 1.0"),
        (2, "ISSUE-01-03 Block 0", ch1, "Soil, water, 48V DC microgrids, compute silicon", "Table 1.1 L1 Ground row"),
        (3, "ISSUE-01-02 Block 0", ch1, "Zero-trust microcontrollers (Zephyr RTOS), galvanic isolation", "Table 1.1 L2 Membrane row"),
        (4, "ISSUE-01-03 Block 1", ch1, "4-stage thermal exergy cascade", "Sec 1.2.1 solid-state electrochemistry & thermal cascade"),
        (5, "ISSUE-01-01", ch1, "Earlier technocratic drafts mistakenly referred to people as ``human actuators,''", "Sec 1.2.1 repudiation of human actuators & Chapter 5 reference"),
        (6, "ISSUE-01-02 Block 1", ch1, r"\path{/run/collective/ipc/l2_l3.sock}", "Sec 1.2.2 Zephyr RTOS, 5 invariants, l2_l3.sock"),
        (7, "ISSUE-01-04", ch1, r"\epsilon(t) = \mathbf{y}_{\mathrm{actual}}(t) - \hat{\mathbf{y}}_{\mathrm{model}}(t)", "Sec 1.4 Living Mirror & innovation residuals"),
        (8, "ISSUE-01-06", ch1, "($R_0 \\to R_6$)", "Sec 1.4.1 Trust Rings R0 to R6 correction"),
        (9, "ISSUE-01-08", ch1, "Red Lever", "Sec 1.4.3 Illich Conviviality & Red Lever Rule"),
        (10, "ISSUE-01-05", ch1, r"\ref{sec:scdd_philosophy}", "Sec 1.7 Dual Cybernetic Compiler & ScDD philosophy reference"),
        
        # Chapter 9 (Fixes 11-18)
        (11, "ISSUE-09-02", ch9, "silicon compute, power generation, and thermodynamic storage are not ethereal abstractions---they are physical matter", "Sec 9.0 Outside-in paradox resolution"),
        (12, "ISSUE-09-01", ch9, r"\ref{tab:node0-bom}", "Sec 9.2 Node 0 Compute Seed BOM table reference"),
        (13, "ISSUE-09-06", ch9, "SCN-TAU-GENESIS", "Sec 9.2 Character Creation, did:peer & Genesis commit"),
        (14, "ISSUE-09-03", ch9, r"\ref{sec:l1_microgrid}", "Sec 9.4 48V DC Microgrid Bus & 4-Stage Thermal Cascade"),
        (15, "ISSUE-09-04", ch9, "W = \\int P\\,dt", "Sec 9.4 Isolated RS-485/CAN Zephyr transducers & work integral"),
        (16, "ISSUE-09-07", ch9, "motorized break-before-make transfer switches", "Sec 9.5b Physical conduit peering & ATS switchgear"),
        (17, "ISSUE-09-05", ch9, "Passive Thermodynamic Resilience", "Table 9.1 Passive Thermodynamic Resilience row"),
        (18, "ISSUE-09-08", ch9, "Mechanical Red Lever Rule", "Table 9.2 Operational closure & Red Lever emergency decoupling"),
        
        # Chapter 10 (Fixes 19-24)
        (19, "ISSUE-10-06", ch10, "32-Slot Episodic Memory Ring Buffer", "Sec 10.2 Dwarf Fortress 32-slot memory buffer & The Sims Founder 01"),
        (20, "ISSUE-10-01/02", ch10, "Successive Over-Relaxation (SOR) Electrical Solver", "Sec 10.2 Cities: Skylines SOR solver & Jorgensen citation"),
        (21, "ISSUE-10-03", ch10, r"\ref{chap:layer1_physical}", "Sec 10.6 Canonical Node 0 BOM provisioning & Ch 2 reference"),
        (22, "ISSUE-10-04/05", ch10, "Living Mirror Mode", "Sec 10.6 Continuous dual-stream Living Mirror & refusal protocol"),
        (23, "ISSUE-10-07", ch10, "BBS+ Zero-Knowledge selective credentials", "Sec 10.7 Purging soulbound in favor of did:peer BBS+"),
        (24, "ISSUE-10-08", ch10, "armored OS2 single-mode fiber", "Sec 10.8.6 Node Mitosis autopoietic closure & optical dark fiber"),
        
        # Chapter 12 (Fixes 25-38)
        (25, "ISSUE-12-06", ch12, "From Seed to Mature Tree", "Sec 12.2 Botanical lifecycle heading"),
        (26, "ISSUE-12-09 Block 1", ch12, r"QUALITY\_SIMULATED", "Sec 12.2.1 SITL bridge & QUALITY_SIMULATED tag"),
        (27, "ISSUE-12-01 Block 1", ch12, "14.34~kWh", "Sec 12.2.3 Canonical Node 0 BOM (14.34 kWh LiFePO4)"),
        (28, "ISSUE-12-09 Block 2", ch12, r"Scenario~$\Omega$", "Sec 12.2.4 Motorized 200A ATS & Scenario Omega singularity"),
        (29, "ISSUE-12-09 Block 3", ch12, "L2: Membrane", "Table 12.1 L2: Membrane row"),
        (30, "ISSUE-12-01 Block 2", ch12, "200A microgrid (6.4kW PV, 14.34kWh BESS)", "Table 12.1 L1: Ground row realigned to 200A / 6.4kW / 14.34kWh"),
        (31, "ISSUE-12-03", ch12, r"\ref{tab:l2-mcu-taxonomy}", "Sec 12.3.1 Three-Tier MCU Hierarchy & 2.5 kV isolation"),
        (32, "ISSUE-12-05", ch12, "EVE LF280K", "Sec 12.3.1 14.34 kWh EVE LF280K cells & 4-stage thermal cascade"),
        (33, "ISSUE-12-02", ch12, "Layer~1 contains zero software daemons", "Sec 12.3.4 Phase 0 Hardware energization & zero daemons in L1"),
        (34, "ISSUE-12-10", ch12, "64-byte cache-line alignment to eliminate false sharing", "Sec 12.3.5 UHAI Simulation Broker 64-byte cache alignment"),
        (35, "ISSUE-12-11", ch12, "Game Designers, Systems Simulators, and Bards", "Sec 12.4.1 Realigned call to Game Designers & Flecs ECS"),
        (36, "ISSUE-12-07", ch12, "Red Lever Invariant", "Sec 12.4.4 Constitutional Charter Article V Red Lever Invariant"),
        (37, "ISSUE-12-08", ch12, "canonical 6.4~kW photovoltaic array", "Sec 12.4.6 Phase D 120-Day Roadmap realigned to 6.4kW / 14.34kWh"),
        (38, "ISSUE-12-04", ch12, "200A", "Action Note delegation: 200A ATS on grid, 400A Class T fuse on battery"),
    ]
    
    passed_fixes = 0
    failed_fixes = []
    
    for fix_num, issue_id, text, marker, desc in checks:
        if marker in text:
            passed_fixes += 1
            print(f"  Fix {fix_num:2d} ({issue_id:18s}): [PASS] {desc}")
        else:
            failed_fixes.append((fix_num, issue_id, marker, desc))
            print(f"  Fix {fix_num:2d} ({issue_id:18s}): [FAIL] Marker '{marker}' NOT FOUND! ({desc})")
            
    print(f"\nFix Verification Summary: {passed_fixes}/38 PASSED ({len(failed_fixes)} failed)")
    assert len(failed_fixes) == 0, f"Failed fixes: {failed_fixes}"
    return passed_fixes

def test_integration_status_truthfulness():
    print("\n" + "=" * 70)
    print("TEST 3: Verification of vol1_l1_l2_integration_status.md Truthfulness")
    print("=" * 70)
    status_file = os.path.join(REPO_ROOT, "vol1_l1_l2_integration_status.md")
    assert os.path.exists(status_file), f"{status_file} does not exist!"
    
    with open(status_file, "r", encoding="utf-8") as f:
        status_text = f.read()
        
    # Statement 1: Git working tree changes: exactly 4 chapter files modified
    code, stdout, _ = run_cmd("git diff --name-only doc/book_collective/chapters/")
    modified_files = [line.strip() for line in stdout.splitlines() if line.strip()]
    expected_modified = [
        "doc/book_collective/chapters/01_anatomy_of_a_node.tex",
        "doc/book_collective/chapters/09_genesis_protocol.tex",
        "doc/book_collective/chapters/10_collective_bringup.tex",
        "doc/book_collective/chapters/12_prime_genesis.tex",
    ]
    print(f"  Modified chapter files: {modified_files}")
    assert sorted(modified_files) == sorted(expected_modified), f"Unexpected modified chapters: {modified_files}"
    
    # Statement 2: Compilation output is 250 pages in main.pdf
    pdf_path = os.path.join(DOC_DIR, "main.pdf")
    assert os.path.exists(pdf_path), "main.pdf does not exist"
    # Read page count via pdfinfo or python
    code, stdout, _ = run_cmd(f"pdfinfo {pdf_path}")
    if code == 0:
        page_match = re.search(r'Pages:\s+(\d+)', stdout)
        if page_match:
            pages = int(page_match.group(1))
            print(f"  main.pdf page count from pdfinfo: {pages}")
            assert pages == 250, f"Expected 250 pages, got {pages}"
    else:
        # Fallback using python pypdf or regex search in log
        log_path = os.path.join(DOC_DIR, "main.log")
        with open(log_path, "r", encoding="latin-1") as f:
            log_content = f.read()
        m = re.search(r'Output written on main\.pdf \((\d+) pages', log_content)
        assert m, "Page count not found in main.log"
        pages = int(m.group(1))
        print(f"  main.pdf page count from main.log: {pages}")
        assert pages == 250, f"Expected 250 pages, got {pages}"
        
    # Statement 3: BibTeX database log (main.blg) has 0 missing entries and 0 warnings
    blg_path = os.path.join(DOC_DIR, "main.blg")
    assert os.path.exists(blg_path), "main.blg does not exist"
    with open(blg_path, "r", encoding="utf-8") as f:
        blg_content = f.read()
    assert "missing$ -- 0" in blg_content, "BibTeX missing entries != 0"
    assert "warning$ -- 0" in blg_content, "BibTeX warnings != 0"
    print("  BibTeX log: 0 missing entries, 0 warnings confirmed.")
    
    # Statement 4: LaTeX transcript log (main.log) has 0 fatal errors and 0 undefined references
    log_path = os.path.join(DOC_DIR, "main.log")
    with open(log_path, "r", encoding="latin-1") as f:
        log_content = f.read()
    assert "! " not in log_content, "Fatal LaTeX error found in main.log"
    assert "LaTeX Warning: Reference" not in log_content, "Undefined reference found in main.log"
    assert "LaTeX Warning: Citation" not in log_content, "Undefined citation found in main.log"
    print("  LaTeX transcript log: 0 fatal errors, 0 undefined references/citations confirmed.")
    
    # Statement 5: vol1_l1_l2_integration_status.md master matrix contains all 38 fixes
    for fix_i in range(1, 39):
        assert f"| {fix_i} |" in status_text, f"Master matrix missing Fix {fix_i}"
    print("  vol1_l1_l2_integration_status.md master matrix: All 38 fixes documented.")
    
    return True

def test_full_compilation():
    print("\n" + "=" * 70)
    print("TEST 4: Independent Full Compilation Execution")
    print("=" * 70)
    build_cmd = "pdflatex -interaction=nonstopmode main.tex && bibtex main && pdflatex -interaction=nonstopmode main.tex && pdflatex -interaction=nonstopmode main.tex"
    code, stdout, stderr = run_cmd(build_cmd, cwd=DOC_DIR)
    print(f"  Compilation exit code: {code}")
    assert code == 0, f"Compilation failed with code {code}!\nStderr:\n{stderr}\nStdout tail:\n{stdout[-500:]}"
    
    # Confirm output
    pdf_path = os.path.join(DOC_DIR, "main.pdf")
    pdf_size = os.path.getsize(pdf_path)
    print(f"  Successfully compiled main.pdf ({pdf_size} bytes)")
    return True

def main():
    print("RUNNING CHALLENGER GATE 1 EMPIRICAL AUDIT SUITE\n")
    test_obsolete_phrases()
    test_all_38_fixes()
    test_integration_status_truthfulness()
    test_full_compilation()
    print("\n" + "=" * 70)
    print("ALL EMPIRICAL TESTS PASSED WITH 100% SUCCESS!")
    print("=" * 70)

if __name__ == "__main__":
    main()
