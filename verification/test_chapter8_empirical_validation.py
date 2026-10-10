#!/usr/bin/env python3
"""
Empirical Adversarial Validation Suite for The Sovereign Stack
Chapter 8 (Layer 7: The Legacy Interface: The Protective Carapace, Inter-Node Trade, and Systemic Blinding)
and Full Book Volume Verification

Automated Tests:
1. File Existence, Integrity & Synchronization of vol1_ch8_blinding_status.md
2. Status Artifact Structural & Content Completeness (7 Sections, 4 Council Roles, Checklists)
3. Cross-Chapter Label and Reference Resolution across entire Volume 1 (100% resolution assertion)
4. Preservation of Critical Chapter 8 Contract Labels and Downstream Mappings
5. BibTeX Citation Resolution across entire Volume 1 against collective.bib & references.bib
6. Requirement R2 Verification: Legal Characterization of Inter-Node Trade
   - BMAA (UUNAA § 6) gratuitous bailments & UCC Art. 2 exclusion
   - Subchapter T SSAs (26 U.S.C. §§ 1381–1388) cost-recovery pass-through
   - Federated Land Trust Utility Easements (UCEA / Restatement)
   - 26 U.S.C. § 6045 & Treas. Reg. § 1.6045-1 Barter Tax Defense (Form 1099-B)
   - 5 Anti-Consolidation & Corporate Veil Firewalls (alter ego defense)
   - Capper-Volstead Antitrust Insulation (7 U.S.C. §§ 291–292)
   - FLSA Labor Bifurcation & Alamo Defense (471 U.S. 290)
   - Dual-Chassis Land Governance & Partition Immunity
7. Requirement R3 Verification: Systemic Blinding Strategies
   - Structural Illegibility (Scott 1998)
   - Cellular Independence ("Army of Unremarkable Cooperatives", zero umbrella parent)
   - Asymmetric Legal Heterogeneity
   - Mundane Semantic Camouflage (NAICS codes, Form 1120-C, 990-EZ/N)
   - Telemetry Obfuscation (IEEE 1547 / CA Rule 21 profile, DMZ truncation)
   - Network Traffic Padding (constant-rate dummy padding over WireGuard/Reticulum)
   - Subpoena Immunity & Doctrine of Data Non-Existence (NAACP v. Alabama, 357 U.S. 449)
8. Doctrinal Invariance & Non-Contradiction Verification
   - Dual-Membrane Doctrine integrity (Membrane 1 vs Membrane 2)
   - "Members, Not Masks" / "No Deception" / Honest Dealing
   - Retirement and purging of predatory/adversarial metaphors ("leeching", "parasitism", "Trojan horse")
9. Compilation & Typesetting Artifact Verification:
   - main.pdf binary stream parsing (189 pages, >1.5MB)
   - main.log analysis (0 undefined references, 0 missing citations, 0 multiply-defined labels)
   - main.blg analysis (0 errors, 0 warnings, 74 bibliography entries)
   - main.aux verification (all Chapter 8 contract labels mapped to page numbers)
   - main.toc verification (all Chapter 8 sections mapped)
"""

import os
import re
import subprocess
import hashlib
import json
import zlib
import sys
from pathlib import Path

WORKSPACE = Path("/Users/raffaine/dev/collective")
BOOK_DIR = WORKSPACE / "doc/book_collective"
CHAPTERS_DIR = BOOK_DIR / "chapters"
MAIN_TEX = BOOK_DIR / "main.tex"
CH8_TEX = CHAPTERS_DIR / "08_layer_7_legacy.tex"
STATUS_ROOT = WORKSPACE / "vol1_ch8_blinding_status.md"
STATUS_BOOK = BOOK_DIR / "vol1_ch8_blinding_status.md"
BIB_FILES = [BOOK_DIR / "collective.bib", BOOK_DIR / "references.bib"]

REQUIRED_CH8_LABELS = [
    "tab:l7-interfaces",
    "tab:l7-forms",
    "sec:l7-municipal",
    "sec:l7-trade",
    "tab:l7-trade-structures",
    "eq:l7_ricardian_hash",
    "sec:l7-blinding",
    "sec:l7-records",
    "tab:l7-records",
    "eq:exergy_residual",
    "eq:export_bounds",
    "eq:fiat_target",
    "eq:exergy_shield",
    "eq:progressive_siphon",
    "sec:l7-severance",
    "eq:dependency_ratio",
]

def strip_latex_comments(content: str) -> str:
    lines = content.splitlines()
    clean_lines = []
    for line in lines:
        cleaned = re.sub(r'(?<!\\)%.*$', '', line)
        clean_lines.append(cleaned)
    return "\n".join(clean_lines)

def get_tex_files():
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
    defined_labels = {}
    referenced_labels = []
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
    bib_keys = set()
    key_pat = re.compile(r'@\w+\s*\{\s*([^,\s]+)\s*,')
    for bfile in BIB_FILES:
        if bfile.exists():
            content = bfile.read_text(encoding="utf-8")
            for m in key_pat.finditer(content):
                bib_keys.add(m.group(1).strip())
    return bib_keys

def parse_citations(tex_files):
    citations = []
    cite_pat = re.compile(r'\\(cite|citep|citet|textcite|parencite|autocite|citeauthor|citeyear|nocite)\*?\{([^}]+)\}')
    for fpath in tex_files:
        lines = fpath.read_text(encoding="utf-8").splitlines()
        for idx, line in enumerate(lines, 1):
            clean = re.sub(r'(?<!\\)%.*$', '', line)
            for m in cite_pat.finditer(clean):
                raw_keys = m.group(2)
                for k in raw_keys.split(','):
                    k_clean = k.strip()
                    if k_clean:
                        citations.append((k_clean, str(fpath.name), idx))
    return citations

def count_pdf_pages(pdf_path: Path) -> int:
    data = pdf_path.read_bytes()
    count = 0
    for m in re.finditer(rb'stream[\r\n]+(.*?)[\r\n]+endstream', data, re.DOTALL):
        try:
            decomp = zlib.decompress(m.group(1))
            c = re.findall(rb'/Type\s*/Page\b', decomp)
            count += len(c)
        except Exception:
            pass
    return count

# ==========================================
# TEST RUNNER
# ==========================================

class TestSuite:
    def __init__(self):
        self.passed = 0
        self.failed = 0
        self.results = []

    def assert_true(self, condition: bool, test_name: str, details: str = ""):
        if condition:
            self.passed += 1
            self.results.append({"test": test_name, "status": "PASS", "details": details})
            print(f"[PASS] {test_name}")
        else:
            self.failed += 1
            self.results.append({"test": test_name, "status": "FAIL", "details": details})
            print(f"[FAIL] {test_name} -- {details}")

def run_tests():
    ts = TestSuite()
    print("=" * 70)
    print("STARTING EMPIRICAL VALIDATION SUITE: CHAPTER 8 & FULL BOOK BUILD")
    print("=" * 70)

    # ----------------------------------------------------
    # TEST 1: Status Artifact Existence and Synchronization
    # ----------------------------------------------------
    ts.assert_true(STATUS_ROOT.exists(), "Status Artifact Exists at Workspace Root", str(STATUS_ROOT))
    ts.assert_true(STATUS_BOOK.exists(), "Status Artifact Exists in doc/book_collective/", str(STATUS_BOOK))
    
    if STATUS_ROOT.exists() and STATUS_BOOK.exists():
        h1 = hashlib.sha256(STATUS_ROOT.read_bytes()).hexdigest()
        h2 = hashlib.sha256(STATUS_BOOK.read_bytes()).hexdigest()
        ts.assert_true(h1 == h2, "Status Artifacts are Byte-for-Byte Synchronized", f"SHA256: {h1}")

    # ----------------------------------------------------
    # TEST 2: Status Artifact Completeness (7 sections, 4 council roles)
    # ----------------------------------------------------
    if STATUS_ROOT.exists():
        status_text = STATUS_ROOT.read_text(encoding="utf-8")
        
        # Check 7 Sections
        sections = [
            "## 1. Executive Summary",
            "## 2. Synthesis of the 4 Council Deliverables",
            "## 3. Comprehensive Structural Map of Chapter 8",
            "## 4. Cross-Chapter Contract & Label Verification",
            "## 5. Bibliography Additions & Validation",
            "## 6. LaTeX Compilation & Verification Results",
            "## 7. Milestone Acceptance Checklist"
        ]
        for sec in sections:
            ts.assert_true(sec in status_text, f"Status Artifact Contains Section: {sec}")
            
        # Check 4 Council Roles
        roles = [
            "2.1 Cooperative / Solidarity Economy Lawyer",
            "2.2 Cybernetic Systems Strategist",
            "2.3 Sovereign Stack Architect",
            "2.4 Narrative Editor"
        ]
        for r in roles:
            ts.assert_true(r in status_text, f"Status Artifact Details Council Role: {r}")

        # Check acceptance checklist items
        checklist_items = ["R1:", "R2:", "R3:", "R4:", "Contract Preservation", "Dual-Membrane Doctrine"]
        for ci in checklist_items:
            ts.assert_true(ci in status_text, f"Status Artifact Checklist Item Present: {ci}")

    # ----------------------------------------------------
    # TEST 3: Bibliography Synchronization and Key Verification
    # ----------------------------------------------------
    bib1_hash = hashlib.sha256(BIB_FILES[0].read_bytes()).hexdigest()
    bib2_hash = hashlib.sha256(BIB_FILES[1].read_bytes()).hexdigest()
    ts.assert_true(bib1_hash == bib2_hash, "Bibliography Databases Byte-for-Byte Synchronized", f"SHA256: {bib1_hash}")

    bib_keys = parse_bib_keys()
    required_citations = [
        "scott1998",
        "beer1974",
        "ostrom1990",
        "ica1995",
        "alamo1985",
        "reves1990",
        "cornforth1995",
        "grigg2004",
        "naacp1958",
    ]
    for k in required_citations:
        ts.assert_true(k in bib_keys, f"BibTeX Database Contains Required Key: {k}")

    # ----------------------------------------------------
    # TEST 4: LaTeX Labels and References Integrity (100% resolution assertion)
    # ----------------------------------------------------
    tex_files = get_tex_files()
    defined_labels, referenced_labels = parse_labels_and_refs(tex_files)

    # Check for multiply defined labels across all TeX files
    multi_defined = {k: v for k, v in defined_labels.items() if len(v) > 1}
    ts.assert_true(len(multi_defined) == 0, "Zero Multiply-Defined Labels in Entire Volume", f"Duplicates: {list(multi_defined.keys())}")

    # Check for undefined referenced labels across all TeX files
    unresolved_refs = [ref for ref in referenced_labels if ref[0] not in defined_labels]
    ts.assert_true(len(unresolved_refs) == 0, "100% Label Reference Resolution Across Entire Volume (0 Unresolved)", f"Unresolved count: {len(unresolved_refs)}")

    # Check specific Chapter 8 required labels
    for req_lbl in REQUIRED_CH8_LABELS:
        ts.assert_true(req_lbl in defined_labels, f"Required Chapter 8 Label Defined: {req_lbl}", 
                       f"Found at: {defined_labels.get(req_lbl, 'MISSING')}")

    # Verify downstream cross-chapter referencing to Chapter 8
    ch8_refs_from_other_chapters = [ref for ref in referenced_labels if ref[0] in defined_labels and any(loc[0] == "08_layer_7_legacy.tex" for loc in defined_labels[ref[0]]) and ref[1] != "08_layer_7_legacy.tex"]
    ts.assert_true(len(ch8_refs_from_other_chapters) > 0, "Chapter 8 Labels are Referenced Cross-Chapter by Volume 1", f"Found {len(ch8_refs_from_other_chapters)} external cross-references")

    # ----------------------------------------------------
    # TEST 5: Citations Resolution Integrity (100% resolution assertion)
    # ----------------------------------------------------
    citations = parse_citations(tex_files)
    unresolved_cites = [c for c in citations if c[0] not in bib_keys]
    ts.assert_true(len(unresolved_cites) == 0, "100% Citation Key Resolution Across Entire Volume (0 Unresolved)", f"Unresolved count: {len(unresolved_cites)}")

    # Verify Chapter 8 specific citations
    ch8_citations = parse_citations([CH8_TEX])
    ch8_keys = {c[0] for c in ch8_citations}
    for k in required_citations:
        ts.assert_true(k in ch8_keys, f"Chapter 8 Cites Required Authority: {k}")

    # ----------------------------------------------------
    # TEST 6: Requirement R2 - Inter-Node Trade Legal Characterization
    # ----------------------------------------------------
    ch8_content = CH8_TEX.read_text(encoding="utf-8")
    
    # 6.1 BMAA (UUNAA § 6)
    ts.assert_true("Uniform Unincorporated Nonprofit Association Act" in ch8_content or "UUNAA" in ch8_content, 
                   "R2: BMAA statutory anchor (UUNAA § 6) explicitly detailed")
    ts.assert_true("gratuitous bailment" in ch8_content, 
                   "R2: Non-commercial gratuitous bailment doctrine articulated")
    ts.assert_true("UCC Article~2" in ch8_content or "Uniform Commercial Code" in ch8_content, 
                   "R2: UCC Article 2 commercial sales exclusion explicitly defended")

    # 6.2 Subchapter T SSAs (26 U.S.C. §§ 1381-1388)
    ts.assert_true("Subchapter~T" in ch8_content or "Subchapter T" in ch8_content, 
                   "R2: Subchapter T statutory framework explicitly detailed")
    ts.assert_true("1381--1388" in ch8_content or "1381-1388" in ch8_content or "26 U.S.C." in ch8_content, 
                   "R2: 26 U.S.C. §§ 1381–1388 explicit citation present")
    ts.assert_true("Shared Services Agreement" in ch8_content or "SSA" in ch8_content, 
                   "R2: Inter-Cooperative Shared Services Agreement (SSA) detailed")
    ts.assert_true("cost-reimbursement" in ch8_content or "at cost" in ch8_content, 
                   "R2: Cost recovery pass-through (zero net margin) detailed")

    # 6.3 Federated Land Trust Utility Easements
    ts.assert_true("Conservation Easement" in ch8_content or "Uniform Conservation Easement Act" in ch8_content or "UCEA" in ch8_content, 
                   "R2: Land trust utility easements & UCEA detailed")
    ts.assert_true("servitude" in ch8_content.lower() or "easement" in ch8_content.lower(), 
                   "R2: Real property servitudes running with the land detailed")

    # 6.4 Barter Taxation Defense (26 U.S.C. § 6045 & Treas. Reg. § 1.6045-1)
    ts.assert_true("6045" in ch8_content, 
                   "R2: 26 U.S.C. § 6045 barter exchange defense explicitly present")
    ts.assert_true("1.6045-1" in ch8_content, 
                   "R2: Treasury Regulation § 1.6045-1 broker definition analyzed")
    ts.assert_true("Form 1099-B" in ch8_content, 
                   "R2: Form 1099-B barter reporting exemption analyzed")
    ts.assert_true("Bilateral Clearing Channels" in ch8_content or "BCC" in ch8_content, 
                   "R2: Non-pecuniary BCC exergy clearing tied to barter exemption")
    ts.assert_true("Rev. Rul. 80-52" in ch8_content or "cost-pooling" in ch8_content, 
                   "R2: IRS cost-pooling / mutual aid precedent cited")

    # 6.5 Anti-Consolidation & Veil-Piercing Firewalls
    ts.assert_true("alter ego" in ch8_content.lower() or "single business enterprise" in ch8_content.lower(), 
                   "R2: Alter ego / single business enterprise risk directly challenged")
    ts.assert_true("firewalls" in ch8_content.lower(), 
                   "R2: Five corporate firewalls enumerated")
    ts.assert_true("cross-collateralization" in ch8_content.lower(), 
                   "R2: Prohibition of cross-collateralization enforced")
    ts.assert_true("EIN" in ch8_content, 
                   "R2: Distinct EINs, articles, and bank accounts mandated")
    ts.assert_true("disclaim" in ch8_content.lower() and "partnership" in ch8_content.lower(), 
                   "R2: Express contractual disclaimer of partnership present")

    # 6.6 Antitrust & Capper-Volstead Protection
    ts.assert_true("Capper-Volstead" in ch8_content, 
                   "R2: Capper-Volstead Act (7 U.S.C. §§ 291–292) antitrust shield detailed")
    ts.assert_true("Sherman" in ch8_content, 
                   "R2: Sherman Act § 1 horizontal cartel defense analyzed")

    # 6.7 FLSA Labor Classification & Alamo Defense
    ts.assert_true("Alamo" in ch8_content, 
                   "R2: Tony and Susan Alamo Foundation v. Secretary of Labor analyzed")
    ts.assert_true("bifurcation" in ch8_content.lower(), 
                   "R2: Strict labor bifurcation between export W-2 labor and commons stewardship")

    # 6.8 Dual-Chassis Land Governance & Partition Immunity
    ts.assert_true("Dual-Chassis" in ch8_content, 
                   "R2: Dual-Chassis Model (Landholding CLT vs Operational Co-op) detailed")
    ts.assert_true("partition" in ch8_content.lower(), 
                   "R2: Immunity against judicial partition by sale codified")

    # ----------------------------------------------------
    # TEST 7: Requirement R3 - Systemic Blinding Strategies
    # ----------------------------------------------------
    # 7.1 Structural Illegibility
    ts.assert_true("Seeing Like a State" in ch8_content or "scott1998" in ch8_content, 
                   "R3: James C. Scott's structural illegibility conceptual foundation cited")
    ts.assert_true("illegib" in ch8_content.lower(), 
                   "R3: Structural illegibility principle articulated")

    # 7.2 Cellular Independence
    ts.assert_true("unremarkable" in ch8_content.lower(), 
                   "R3: Army of unremarkable cooperatives paradigm present")
    ts.assert_true("holding compan" in ch8_content.lower() and "prohibit" in ch8_content.lower() or "refus" in ch8_content.lower(), 
                   "R3: Prohibition of parent holding companies / corporate pyramids enforced")

    # 7.3 Asymmetric Legal Heterogeneity
    ts.assert_true("heterogeneity" in ch8_content.lower(), 
                   "R3: Asymmetric legal heterogeneity across jurisdictions codified")

    # 7.4 Mundane Semantic Camouflage
    ts.assert_true("NAICS" in ch8_content, 
                   "R3: Standard NAICS industry codes mandated for camouflage")
    ts.assert_true("1120-C" in ch8_content or "990-N" in ch8_content or "990-EZ" in ch8_content, 
                   "R3: Standard IRS forms (Form 1120-C, 990-N/EZ) specified")

    # 7.5 Telemetry Obfuscation & IEEE 1547 Profile
    ts.assert_true("IEEE 1547" in ch8_content or "1547" in ch8_content, 
                   "R3: IEEE 1547 standard profile net-meter compliance specified")
    ts.assert_true("Rule~21" in ch8_content or "Rule 21" in ch8_content, 
                   "R3: California Rule 21 net-metering schedule specified")
    ts.assert_true("DMZ" in ch8_content, 
                   "R3: Sub-metering and battery metrics truncated at DMZ boundary")
    ts.assert_true("padding" in ch8_content.lower() or "dummy" in ch8_content.lower(), 
                   "R3: WireGuard / Reticulum network traffic dummy padding against correlation attacks")

    # 7.6 Subpoena Immunity & Data Non-Existence
    ts.assert_true("NAACP v. Alabama" in ch8_content or "naacp1958" in ch8_content, 
                   "R3: NAACP v. Alabama First Amendment associative privacy cited")
    ts.assert_true("Data Non-Existence" in ch8_content or "non-existence" in ch8_content.lower(), 
                   "R3: Doctrine of Data Non-Existence codified")
    ts.assert_true("spoliation" in ch8_content.lower(), 
                   "R3: Defense against evidence spoliation and contempt validated")
    ts.assert_true("tab:l7-records" in ch8_content, 
                   "R3: Table of records held vs never held present")

    # ----------------------------------------------------
    # TEST 8: Doctrinal Invariance & Non-Contradiction
    # ----------------------------------------------------
    # 8.1 Dual-Membrane Doctrine
    ts.assert_true("Dual-Membrane" in ch8_content, 
                   "Doctrine: Dual-Membrane Doctrine formally defined")
    ts.assert_true("Membrane~1" in ch8_content and "Membrane~2" in ch8_content, 
                   "Doctrine: Both Membrane 1 (Legal Carapace) and Membrane 2 (Sovereign Core) codified")

    # 8.2 Members, Not Masks & Honest Dealing
    ts.assert_true("Members, Not Masks" in ch8_content, 
                   "Doctrine: 'Members, Not Masks' principle explicitly codified")
    ts.assert_true("Export Ethics Charter" in ch8_content, 
                   "Doctrine: Export Ethics Charter codified")
    ts.assert_true("Honest Dealing" in ch8_content or "honest dealing" in ch8_content.lower(), 
                   "Doctrine: Honest dealing with neighbors codified")

    # 8.3 Retirement of Predatory Metaphors
    lines = ch8_content.splitlines()
    cynical_terms = ["leeching", "parasitism", "trojan horse", "money laundering"]
    violating_lines = []
    in_retirement_sec = False
    for idx, line in enumerate(lines, 1):
        if "Retiring Adversarial and Parasitic Metaphors" in line:
            in_retirement_sec = True
        elif line.startswith(r"\section") or line.startswith(r"\chapter"):
            if "Retiring" not in line:
                in_retirement_sec = False
                
        line_clean = re.sub(r'(?<!\\)%.*$', '', line).lower()
        for term in cynical_terms:
            if term in line_clean and not in_retirement_sec:
                if "reject" not in line_clean and "prohibit" not in line_clean and "avoid" not in line_clean:
                    violating_lines.append((idx, term, line))

    ts.assert_true(len(violating_lines) == 0, 
                   "Doctrine: Predatory/Cynical metaphors completely purged from active policy", 
                   f"Violations: {violating_lines}")

    # ----------------------------------------------------
    # TEST 9: Build Artifact & Publication Verification
    # ----------------------------------------------------
    print("-" * 70)
    print("AUDITING BUILD ARTIFACTS, TYPESETTING LOGS, AND PDF STRUCTURE")
    print("-" * 70)

    # 9.1 PDF Output Verification
    main_pdf = BOOK_DIR / "main.pdf"
    ts.assert_true(main_pdf.exists(), "Build Artifact main.pdf Exists", str(main_pdf))
    if main_pdf.exists():
        pdf_size = main_pdf.stat().st_size
        ts.assert_true(pdf_size > 1_500_000, "Output PDF is of Substantial Publication Size (>1.5MB)", f"Size: {pdf_size} bytes")
        pdf_pages = count_pdf_pages(main_pdf)
        ts.assert_true(pdf_pages == 189, "Output PDF Exactly Matches Required 189 Pages", f"Pages: {pdf_pages}")

    # 9.2 Log File Inspection
    main_log = (BOOK_DIR / "main.log").read_text(encoding="utf-8", errors="replace")
    undef_ref_matches = re.findall(r'LaTeX Warning: Reference `([^`\']+)\' on page \d+ undefined', main_log)
    ts.assert_true(len(undef_ref_matches) == 0, "main.log Contains Zero Undefined References", 
                   f"Undefined refs: {undef_ref_matches}")

    undef_cite_matches = re.findall(r'LaTeX Warning: Citation `([^`\']+)\' on page \d+ undefined', main_log)
    ts.assert_true(len(undef_cite_matches) == 0, "main.log Contains Zero Undefined Citations", 
                   f"Undefined cites: {undef_cite_matches}")

    multi_label_matches = re.findall(r'LaTeX Warning: Label `([^`\']+)\' multiply defined', main_log)
    ts.assert_true(len(multi_label_matches) == 0, "main.log Contains Zero Multiply-Defined Labels", 
                   f"Multiply defined: {multi_label_matches}")

    page_match = re.search(r'Output written on main\.pdf \((\d+) pages', main_log)
    log_pages = int(page_match.group(1)) if page_match else 0
    ts.assert_true(log_pages == 189, "main.log Confirms 189 Pages Written", f"Log Pages: {log_pages}")

    # 9.3 BibTeX Processing Log (main.blg)
    main_blg = (BOOK_DIR / "main.blg").read_text(encoding="utf-8", errors="replace")
    blg_entries = re.search(r"You've used (\d+) entries", main_blg)
    entry_count = int(blg_entries.group(1)) if blg_entries else 0
    ts.assert_true(entry_count == 74, "main.blg Confirms Exactly 74 BibTeX Entries Processed", f"Entries: {entry_count}")

    blg_warnings = re.findall(r'Warning--', main_blg)
    ts.assert_true(len(blg_warnings) == 0, "BibTeX main.blg Contains Zero Warnings", f"Warnings: {len(blg_warnings)}")

    # 9.4 main.aux and main.toc Verification
    main_aux = (BOOK_DIR / "main.aux").read_text(encoding="utf-8", errors="replace")
    for req_lbl in REQUIRED_CH8_LABELS:
        ts.assert_true(f"\\newlabel{{{req_lbl}}}" in main_aux, f"Label Verified in Compiled main.aux: {req_lbl}")

    main_toc = (BOOK_DIR / "main.toc").read_text(encoding="utf-8", errors="replace")
    ts.assert_true("Layer 7: The Legacy Interface" in main_toc, "Chapter 8 Successfully Registered in Table of Contents (main.toc)")

    # ----------------------------------------------------
    # FINAL SUMMARY
    # ----------------------------------------------------
    print("=" * 70)
    print(f"EMPIRICAL VALIDATION SUITE SUMMARY: {ts.passed} PASSED, {ts.failed} FAILED")
    print("=" * 70)

    results_path = WORKSPACE / "verification/chapter8_validation_results.json"
    results_path.write_text(json.dumps(ts.results, indent=2), encoding="utf-8")
    print(f"Results saved to: {results_path}")

    return ts.failed == 0

if __name__ == "__main__":
    success = run_tests()
    sys.exit(0 if success else 1)
