#!/usr/bin/env python3
"""
Test Suite: Web Companion Dashboard Verification
Verifies Milestone 3 deliverables in core/1_simulation/src/web/companion/
- Mobile responsiveness and viewport settings
- Universal pinned 6-layer navigation dock
- Strict Invariant 7: 100% read-only diagnostic decks (L2, L3, L4, L5, L7)
- Layer 6 exclusive societal interaction boundary
- Solarpunk/Cyberpunk CSS color tokens and glassmorphism styling
- Vanilla ES6 JavaScript syntax and API integration logic
"""

import os
import re
import sys
import json
import subprocess
import unittest

COMPANION_DIR = "/Users/raffaine/dev/collective/core/1_simulation/src/web/companion"
INDEX_HTML = os.path.join(COMPANION_DIR, "index.html")
STYLE_CSS = os.path.join(COMPANION_DIR, "style.css")
APP_JS = os.path.join(COMPANION_DIR, "app.js")

class TestCompanionWebDashboard(unittest.TestCase):

    def test_01_files_exist_and_non_empty(self):
        """Verify all three core files exist and are adequately sized."""
        for path in [INDEX_HTML, STYLE_CSS, APP_JS]:
            self.assertTrue(os.path.exists(path), f"File missing: {path}")
            size = os.path.getsize(path)
            self.assertGreater(size, 1000, f"File unexpectedly small ({size} bytes): {path}")

    def test_02_html_viewport_and_mobile_meta(self):
        """Verify mobile-responsive viewport metadata and PWA title."""
        with open(INDEX_HTML, "r", encoding="utf-8") as f:
            html = f.read()

        self.assertIn('<meta name="viewport"', html)
        self.assertIn("width=device-width", html)
        self.assertIn("viewport-fit=cover", html)
        self.assertIn("Cadastral Slate", html)

    def test_03_html_pinned_6_layer_dock(self):
        """Verify universal pinned 6-layer navigation dock layout."""
        with open(INDEX_HTML, "r", encoding="utf-8") as f:
            html = f.read()

        # Check dock presence and all 6 layer tabs
        self.assertIn('class="pinned-nav-dock"', html)
        for layer_tab in ['data-layer="l2"', 'data-layer="l3"', 'data-layer="l4"', 
                          'data-layer="l5"', 'data-layer="l6"', 'data-layer="l7"']:
            self.assertIn(layer_tab, html, f"Dock missing tab for {layer_tab}")

        # Check pointer indicator notation pattern
        self.assertIn("[►L6: INTENT◄]", html)

    def test_04_html_top_status_bar(self):
        """Verify top status bar contains DID, provenance badge, connection status, and vitals."""
        with open(INDEX_HTML, "r", encoding="utf-8") as f:
            html = f.read()

        self.assertIn('id="top-status-bar"', html)
        self.assertIn('did:peer:', html)
        self.assertIn('badge-provenance', html)
        self.assertIn('0x02 ROOT', html)
        self.assertIn('● ONLINE', html)
        self.assertIn('vital-soc', html)
        self.assertIn('vital-flux', html)
        self.assertIn('vital-cistern', html)
        self.assertIn('vital-hlc', html)

    def test_05_html_invariant_7_read_only_diagnostic_decks(self):
        """Verify Layer 2, 4, 5, 7 decks exist, have read-only notices, and ZERO mutating forms."""
        with open(INDEX_HTML, "r", encoding="utf-8") as f:
            html = f.read()

        for deck_id in ["deck-l2", "deck-l3", "deck-l4", "deck-l5", "deck-l7"]:
            self.assertIn(f'id="{deck_id}"', html, f"Missing deck {deck_id}")

        # Extract contents of each diagnostic deck and verify ZERO <form> tags
        for deck_id in ["deck-l2", "deck-l3", "deck-l4", "deck-l5", "deck-l7"]:
            # Find section block
            pattern = rf'<section[^>]*id="{deck_id}"[^>]*>(.*?)</section>'
            match = re.search(pattern, html, re.DOTALL)
            self.assertIsNotNone(match, f"Could not extract {deck_id}")
            deck_content = match.group(1)

            # Assert ZERO forms
            self.assertNotIn("<form", deck_content, f"Illegal <form> tag inside read-only {deck_id}")

            # Assert read-only badge/notice
            self.assertIn("100% READ-ONLY", deck_content, f"Missing 100% READ-ONLY tag in {deck_id}")

        # Check prominent Invariant 7 notice in Layer 4
        l4_match = re.search(r'<section[^>]*id="deck-l4"[^>]*>(.*?)</section>', html, re.DOTALL)
        self.assertIsNotNone(l4_match)
        l4_content = l4_match.group(1)
        self.assertIn("sandboxed, and read-only", l4_content)
        self.assertIn("CANNOT accept or approve tasks in Layer 4", l4_content)

    def test_06_html_layer6_exclusive_interaction_boundary(self):
        """Verify Layer 6 contains CDA orderbook, limit order form, intent builder, and chore claims."""
        with open(INDEX_HTML, "r", encoding="utf-8") as f:
            html = f.read()

        l6_match = re.search(r'<section[^>]*id="deck-l6"[^>]*>(.*?)</section>', html, re.DOTALL)
        self.assertIsNotNone(l6_match, "Missing deck-l6 section")
        l6_content = l6_match.group(1)

        # CDA Order book & Limit Order Form
        self.assertIn('id="form-agora-order"', l6_content)
        self.assertIn('id="order-qty"', l6_content)
        self.assertIn('id="order-price"', l6_content)
        self.assertIn('btn-submit-order', l6_content)

        # Semantic Intent Builder
        self.assertIn('id="form-intent-builder"', l6_content)
        self.assertIn('id="intent-type"', l6_content)
        self.assertIn('id="intent-prompt"', l6_content)
        self.assertIn('id="readback-preview"', l6_content)
        self.assertIn('btn-dispatch-intent', l6_content)

        # Chore Work Bounties with Claim Buttons
        self.assertIn('CHORE BOUNTY #8492', l6_content)
        self.assertIn('btn-claim-chore', l6_content)
        self.assertIn('data-chore-id="8492"', l6_content)

    def test_07_css_solarpunk_tokens_and_styling(self):
        """Verify CSS contains obsidian background and official layer color tokens."""
        with open(STYLE_CSS, "r", encoding="utf-8") as f:
            css = f.read()

        # Canvas substrate
        self.assertIn("--color-bg-canvas:", css)
        self.assertIn("#080c14", css)

        # Layer Chromas
        self.assertIn("--color-l2-emerald:", css)
        self.assertIn("#10b981", css)
        self.assertIn("--color-l3-cyan:", css)
        self.assertIn("#06b6d4", css)
        self.assertIn("--color-l4-indigo:", css)
        self.assertIn("#6366f1", css)
        self.assertIn("--color-l5-purple:", css)
        self.assertIn("#a855f7", css)
        self.assertIn("--color-l6-amber:", css)
        self.assertIn("#f59e0b", css)
        self.assertIn("--color-l7-slate:", css)
        self.assertIn("#64748b", css)

        # Mobile responsiveness
        self.assertIn("max-width: 480px", css)
        self.assertIn("backdrop-filter:", css)
        self.assertIn("min-height: 48px", css)

    def test_08_js_syntax_and_endpoints(self):
        """Verify JS syntax is clean ES6 and strictly targets POST /api/v1/l6/intent."""
        with open(APP_JS, "r", encoding="utf-8") as f:
            js = f.read()

        # Check that the mutating endpoint is POST /api/v1/l6/intent
        self.assertIn("/api/v1/l6/intent", js)
        self.assertIn("method: 'POST'", js)

        # Check diagnostic polling endpoints
        self.assertIn("/api/v1/layers/summary", js)

        # Check that NO mutating POST requests exist for L2, L3, L4, L5, L7
        for illegal in ["/api/v1/l2/intent", "/api/v1/l4/intent", "/api/v1/l4/workflows'",
                        "/api/v1/l7/intent", "/api/v1/l2/modify", "/api/v1/l4/approve"]:
            self.assertNotIn(illegal, js, f"Illegal endpoint found in JS: {illegal}")

        # Check Node.js syntax check
        node_check = subprocess.run(["node", "-c", APP_JS], capture_output=True, text=True)
        self.assertEqual(node_check.returncode, 0, f"JS syntax check failed: {node_check.stderr}")

    def test_09_js_headless_simulation_execution(self):
        """Execute app.js logic in Node.js with mock DOM to test simulation and state transitions."""
        test_script = f"""
        const fs = require('fs');
        const jsCode = fs.readFileSync('{APP_JS}', 'utf-8');

        // Minimal browser shim
        global.window = {{
            location: {{ port: '8080' }},
            scrollTo: () => {{}},
            addEventListener: () => {{}},
            __COLLECTIVE_COMPANION__: null
        }};
        global.document = {{
            readyState: 'complete',
            getElementById: (id) => ({{
                id,
                value: '',
                textContent: '',
                classList: {{ add: () => {{}}, remove: () => {{}}, contains: () => false }},
                style: {{}},
                setAttribute: () => {{}},
                getAttribute: () => null,
                addEventListener: () => {{}},
                appendChild: () => {{}},
                querySelector: () => ({{ innerHTML: '', value: '', textContent: '', appendChild: () => {{}}, querySelector: () => ({{ textContent: '' }}) }}),
                querySelectorAll: () => []
            }}),
            querySelectorAll: () => [],
            createElement: () => ({{ style: {{}}, classList: {{ add: () => {{}} }}, appendChild: () => {{}}, querySelector: () => ({{ onclick: null }}), remove: () => {{}} }}),
            addEventListener: () => {{}}
        }};
        global.navigator = {{ vibrate: () => {{}} }};
        global.fetch = async () => {{ throw new Error('Daemon offline'); }};
        global.setInterval = () => {{}};

        eval(jsCode);

        const companion = global.window.__COLLECTIVE_COMPANION__;
        if (!companion) {{
            console.error('ERROR: window.__COLLECTIVE_COMPANION__ not exported');
            process.exit(1);
        }}

        // Verify initial state
        console.log('Active deck:', companion.state.activeDeck);
        console.log('Provenance:', companion.state.provenance);
        console.log('Is simulation:', companion.state.isSimulation);

        // Test navigation
        companion.navigateTo('l4');
        if (companion.state.activeDeck !== 'deck-l4') {{
            console.error('Navigation to L4 failed');
            process.exit(1);
        }}

        // Test provenance toggle
        companion.toggleProvenance();
        if (companion.state.provenance !== '0x01') {{
            console.error('Provenance toggle failed');
            process.exit(1);
        }}

        // Test simulation polling step
        companion.pollDiagnostics();

        // Test L6 intent dispatch simulation
        companion.dispatchL6Intent('AGORA_ORDER', {{ side: 'BUY', quantity_kwh: 10, price_exergy_mj: 3.55 }});

        console.log('SUCCESS: Headless simulation validation completed.');
        """
        result = subprocess.run(["node", "-e", test_script], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, f"Headless execution failed:\n{result.stderr}\nOutput:\n{result.stdout}")
        self.assertIn("SUCCESS: Headless simulation validation completed", result.stdout)

if __name__ == "__main__":
    unittest.main(verbosity=2)
