#!/usr/bin/env python3
"""
Empirical Verification Suite: Anti-Bleed Purge
Round 3 Architectural Council Verification Gate
Target: docs/OASIS_ARCHITECTURE.md, docs/OASIS_GDD.md, PRODUCT_BACKLOG_V4.md, core/1_simulation/src/
"""

import os
import re

def audit_document_anti_bleed():
    print("=== [TEST 1] Anti-Bleed Purge: Specification Document Audit ===")
    
    docs = [
        "docs/OASIS_ARCHITECTURE.md",
        "docs/OASIS_GDD.md",
        "PRODUCT_BACKLOG_V4.md"
    ]
    
    thread_patterns = [
        r"\bThread\s*1\b",
        r"\bThread\s*3\b",
        r"\bT1\b.*rendering",
        r"\bT3\b.*layer"
    ]
    
    violations = []
    
    for doc in docs:
        if not os.path.exists(doc):
            print(f"File not found: {doc}")
            continue
        with open(doc, "r", encoding="utf-8") as f:
            lines = f.readlines()
        
        for idx, line in enumerate(lines):
            # Check for active Thread 1 / Thread 3 mentions (excluding purge manifests)
            if "PURGED" in line or "anti-pattern" in line.lower() or "purged" in line.lower():
                continue
            for pat in thread_patterns:
                if re.search(pat, line, re.IGNORECASE):
                    violations.append((doc, idx + 1, line.strip()))
    
    print(f"Monolithic Game-Loop Threading (Thread 1 / Thread 3) Active Violations: {len(violations)}")
    for v in violations:
        print(f"  [VIOLATION] {v[0]}:{v[1]}: {v[2]}")
    
    # Check for embedded daemons or custom P2P
    forbidden_engine_terms = [
        r"embeds?\s+col-",
        r"static_assert.*col-",
        r"libp2p\s+transport\s+in\s+engine",
        r"webrtc.*socket.*in\s+engine"
    ]
    
    daemon_violations = []
    for doc in docs:
        with open(doc, "r", encoding="utf-8") as f:
            lines = f.readlines()
        for idx, line in enumerate(lines):
            if "PURGED" in line or "zero" in line.lower():
                continue
            for pat in forbidden_engine_terms:
                if re.search(pat, line, re.IGNORECASE):
                    daemon_violations.append((doc, idx + 1, line.strip()))
    
    print(f"Embedded Daemons / Custom P2P Specification Violations: {len(daemon_violations)}")
    for v in daemon_violations:
        print(f"  [VIOLATION] {v[0]}:{v[1]}: {v[2]}")
    
    return len(violations) == 0, len(daemon_violations) == 0

def audit_legacy_source_bleed():
    print("\n=== [TEST 2] Anti-Bleed Audit: Lingering Source Code & Procurement Bleed ===")
    
    src_dir = "core/1_simulation/src/core"
    unpurged_files = []
    if os.path.exists(src_dir):
        files = os.listdir(src_dir)
        check_targets = [
            "did_crypto_generator.cpp",
            "did_crypto_generator.hpp",
            "crdt_sync_engine.hpp",
            "bpmn_parser.cpp",
            "bpmn_parser.hpp"
        ]
        for t in check_targets:
            if t in files:
                unpurged_files.append(t)
    
    print(f"Lingering V1 Source Files in core/1_simulation/src/core: {len(unpurged_files)}")
    for f in unpurged_files:
        print(f"  - Found unpurged legacy file: {f}")
    
    return unpurged_files

if __name__ == "__main__":
    threads_clean, daemons_clean = audit_document_anti_bleed()
    unpurged = audit_legacy_source_bleed()
    
    print("\n=== ANTI-BLEED AUDIT SUMMARY ===")
    print(f"Document Monolithic Thread Purge Confirmed:    {threads_clean}")
    print(f"Document Embedded Daemon/P2P Purge Confirmed:  {daemons_clean}")
    print(f"Workspace Source Code Purge Needed:           {len(unpurged) > 0}")
