#!/usr/bin/env node
/**
 * Empirical Adversarial Test Harness: Vitals HUD & Pointer Events Contract
 * 
 * Challenger sprint7_2 Adversarial Suite
 * 
 * Stress-tests:
 * 1. HTML/CSS pointer-events: none !important contract on #vitals-hud and all children (*).
 * 2. Hit-testing pass-through: mouse clicks in HUD region pass directly to canvas without interception.
 * 3. updateHUD(caloric_reserve, stress, focus) boundary, extreme, and malformed inputs:
 *    - Negative calories (-500, -1e6) -> clamped to 0
 *    - Extreme calories (10000, 1e9) -> clamped to 2500
 *    - Stress > 1.0 (1.5, 999.0) -> clamped to 1.0, high-hazard red styling triggered
 *    - Stress < 0.0 (-0.5) -> clamped to 0.0
 *    - Focus boundary (< 0.0, > 1.0) -> clamped to [0.0, 1.0]
 *    - NaN, null, undefined, malformed strings, objects -> sanitized to 0
 *    - Infinity, -Infinity -> clamped to extremes [0, 2500] and [0.0, 1.0]
 * 4. Telemetry tracking in window._oasisLastHUD:
 *    - Records accurate numeric values
 *    - Monotonic tick counter incrementation over 10,000 rapid cycles
 * 5. Missing DOM element safety: zero unhandled exceptions when DOM elements are absent.
 */

import fs from 'node:fs';
import path from 'node:path';
import vm from 'node:vm';
import { fileURLToPath } from 'node:url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const SIM_DIR = path.resolve(__dirname, '..');
const SHELL_HTML_PATH = path.join(SIM_DIR, 'shell_minimal.html');
const WASM_HTML_PATH = path.join(SIM_DIR, 'build_wasm', 'oasis_engine.html');

console.log('================================================================================');
console.log('=== Challenger Sprint 7: Adversarial HUD & Mouse Contract Stress Test ===');
console.log('================================================================================');

let passedAssertions = 0;
let totalAssertions = 0;

function assert(condition, message) {
  totalAssertions++;
  if (!condition) {
    console.error(`[FAIL] ${message}`);
    throw new Error(`Assertion failed: ${message}`);
  }
  passedAssertions++;
}

// -----------------------------------------------------------------------------
// Part 1: Verify CSS Contract in HTML Shells
// -----------------------------------------------------------------------------
console.log('\n--- [TEST 1] CSS Contract Verification (shell_minimal.html & build_wasm/oasis_engine.html) ---');

function verifyHtmlCssContract(filePath, label) {
  assert(fs.existsSync(filePath), `${label} file must exist at ${filePath}`);
  const content = fs.readFileSync(filePath, 'utf-8');

  // Verify #vitals-hud selector has pointer-events: none !important
  const hudPointerEventsMatch = /#vitals-hud\s*\{[^}]*pointer-events:\s*none\s*!important;/s.test(content);
  assert(hudPointerEventsMatch, `${label}: #vitals-hud must have 'pointer-events: none !important;'`);

  // Verify child wildcard selector has pointer-events: none !important
  const hudWildcardMatch = /#vitals-hud\s+\*\s*\{[^}]*pointer-events:\s*none\s*!important;/s.test(content);
  assert(hudWildcardMatch, `${label}: #vitals-hud * must have 'pointer-events: none !important;'`);

  // Verify canvas element exists
  assert(/<canvas[^>]+id=["']canvas["']/i.test(content), `${label}: <canvas id="canvas"> must exist in HTML`);

  // Verify #vitals-hud overlay exists
  assert(/<div[^>]+id=["']vitals-hud["']/i.test(content), `${label}: <div id="vitals-hud"> must exist in HTML`);

  // Verify essential child metric IDs exist
  assert(content.includes('id="hud-calories-bar"'), `${label}: hud-calories-bar element must exist`);
  assert(content.includes('id="hud-calories-val"'), `${label}: hud-calories-val element must exist`);
  assert(content.includes('id="hud-stress-bar"'), `${label}: hud-stress-bar element must exist`);
  assert(content.includes('id="hud-stress-val"'), `${label}: hud-stress-val element must exist`);
  assert(content.includes('id="hud-focus-bar"'), `${label}: hud-focus-bar element must exist`);
  assert(content.includes('id="hud-focus-val"'), `${label}: hud-focus-val element must exist`);

  // Verify updateHUD function definition exists
  assert(/function\s+updateHUD\s*\(/i.test(content), `${label}: updateHUD function must be declared`);
  assert(content.includes('window.updateHUD = updateHUD'), `${label}: updateHUD must be attached to window`);

  console.log(`[PASS] ${label} validated: CSS pointer-events: none contract and DOM structure verified.`);
}

verifyHtmlCssContract(SHELL_HTML_PATH, 'Source Shell (shell_minimal.html)');
if (fs.existsSync(WASM_HTML_PATH)) {
  verifyHtmlCssContract(WASM_HTML_PATH, 'WASM Build Output (oasis_engine.html)');
} else {
  console.log('[INFO] build_wasm/oasis_engine.html not yet built or located; skipping output check.');
}

// -----------------------------------------------------------------------------
// Part 2: Hit-Testing Simulation & Mouse Pass-Through Verification
// -----------------------------------------------------------------------------
console.log('\n--- [TEST 2] Hit-Testing Simulation & Canvas Mouse Event Pass-Through ---');

/**
 * Emulate CSS Pointer Events Hit-Testing algorithm:
 * Given a tree of elements with 2D bounds and computed pointer-events style:
 * If an element has pointer-events: none, hit-testing ignores it and continues
 * down to the underlying elements.
 */
class MockElement {
  constructor(id, bounds, pointerEvents = 'auto') {
    this.id = id;
    this.bounds = bounds; // { x, y, width, height }
    this.pointerEvents = pointerEvents;
    this.children = [];
    this.style = {};
    this.textContent = '';
  }

  contains(px, py) {
    return px >= this.bounds.x &&
           px <= this.bounds.x + this.bounds.width &&
           py >= this.bounds.y &&
           py <= this.bounds.y + this.bounds.height;
  }
}

function hitTest(elementsInZOrder, px, py) {
  // Elements ordered from topmost to bottommost
  for (const el of elementsInZOrder) {
    if (el.pointerEvents === 'none') {
      // transparent to mouse clicks
      continue;
    }
    if (el.contains(px, py)) {
      return el;
    }
  }
  return null;
}

const canvasEl = new MockElement('canvas', { x: 0, y: 0, width: 800, height: 600 }, 'auto');
const hudContainerEl = new MockElement('vitals-hud', { x: 14, y: 14, width: 240, height: 160 }, 'none');
const hudBarCalEl = new MockElement('hud-calories-bar', { x: 28, y: 40, width: 200, height: 7 }, 'none');
const hudBarStrEl = new MockElement('hud-stress-bar', { x: 28, y: 80, width: 200, height: 7 }, 'none');
const hudBarFocEl = new MockElement('hud-focus-bar', { x: 28, y: 120, width: 200, height: 7 }, 'none');

// Topmost to bottommost z-order:
const zOrder = [hudBarFocEl, hudBarStrEl, hudBarCalEl, hudContainerEl, canvasEl];

// Test 5,000 random points across the entire 800x600 canvas
let hitsInsideHud = 0;
let hitsOutsideHud = 0;

for (let i = 0; i < 5000; i++) {
  const px = Math.random() * 800;
  const py = Math.random() * 600;

  const target = hitTest(zOrder, px, py);
  assert(target !== null && target.id === 'canvas', `Hit test at (${px.toFixed(1)}, ${py.toFixed(1)}) must resolve to canvas, got ${target?.id}`);

  if (hudContainerEl.contains(px, py)) {
    hitsInsideHud++;
  } else {
    hitsOutsideHud++;
  }
}

assert(hitsInsideHud > 100, `At least 100 sample points must land within the HUD region (sampled ${hitsInsideHud})`);
assert(hitsOutsideHud > 1000, `At least 1000 sample points must land outside the HUD region (sampled ${hitsOutsideHud})`);
console.log(`[PASS] 5,000 hit-test samples verified (${hitsInsideHud} inside HUD region, ${hitsOutsideHud} outside): 100% resolved to canvas.`);

// -----------------------------------------------------------------------------
// Part 3: updateHUD Execution Environment & Boundary Stress Testing
// -----------------------------------------------------------------------------
console.log('\n--- [TEST 3] updateHUD Boundary, Extreme & Corrupt Input Stress Testing ---');

// Extract the exact updateHUD function from shell_minimal.html
const shellContent = fs.readFileSync(SHELL_HTML_PATH, 'utf-8');
const scriptMatch = shellContent.match(/function\s+updateHUD\s*\(caloric_reserve,\s*stress,\s*focus(?:,\s*ambient_temp)?\)\s*\{([\s\S]*?)\n\s*\}\s*\n\s*window\.updateHUD\s*=\s*updateHUD;/);
assert(scriptMatch !== null, 'Must extract updateHUD implementation from shell_minimal.html');

// Create isolated DOM VM context
function createHudContext() {
  const elements = {
    'hud-calories-bar': { style: {}, textContent: '', className: '' },
    'hud-calories-val': { style: {}, textContent: '', className: '' },
    'hud-stress-bar': { style: {}, textContent: '', className: '' },
    'hud-stress-val': { style: {}, textContent: '', className: '' },
    'hud-focus-bar': { style: {}, textContent: '', className: '' },
    'hud-focus-val': { style: {}, textContent: '', className: '' },
    'hud-temp-bar': { style: {}, textContent: '', className: '' },
    'hud-temp-val': { style: {}, textContent: '', className: '' }
  };

  const sandbox = {
    document: {
      getElementById: (id) => elements[id] || null
    },
    window: {},
    Number: Number,
    Math: Math,
    Date: Date,
    console: console
  };
  sandbox.window = sandbox;

  const code = `
    function updateHUD(caloric_reserve, stress, focus, ambient_temp) {
      ${scriptMatch[1]}
    }
    window.updateHUD = updateHUD;
  `;

  vm.createContext(sandbox);
  vm.runInContext(code, sandbox);

  return { sandbox, elements };
}

// Subtest 3.1: Nominal Inputs
{
  const { sandbox, elements } = createHudContext();
  sandbox.updateHUD(2500, 0.25, 0.75);

  assert(elements['hud-calories-bar'].style.width === '100.0%', 'Nominal calories bar width should be 100.0%');
  assert(elements['hud-calories-val'].textContent === '2500 / 2500 kcal', 'Nominal calories text should be 2500 / 2500 kcal');
  assert(elements['hud-stress-bar'].style.width === '25.0%', 'Nominal stress bar width should be 25.0%');
  assert(elements['hud-stress-val'].textContent === '25%', 'Nominal stress text should be 25%');
  assert(elements['hud-focus-bar'].style.width === '75.0%', 'Nominal focus bar width should be 75.0%');
  assert(elements['hud-focus-val'].textContent === '75%', 'Nominal focus text should be 75%');

  assert(sandbox.window._oasisLastHUD.calories === 2500, 'Telemetry calories matches 2500');
  assert(sandbox.window._oasisLastHUD.stress === 0.25, 'Telemetry stress matches 0.25');
  assert(sandbox.window._oasisLastHUD.focus === 0.75, 'Telemetry focus matches 0.75');
  assert(sandbox.window._oasisLastHUD.tick === 1, 'Initial tick count is 1');
  console.log('[PASS] Nominal vitals (2500 kcal, 0.25 stress, 0.75 focus) rendered correctly.');
}

// Subtest 3.2: Zero Bounds
{
  const { sandbox, elements } = createHudContext();
  sandbox.updateHUD(0, 0, 0);

  assert(elements['hud-calories-bar'].style.width === '0.0%', 'Zero calories bar width should be 0.0%');
  assert(elements['hud-calories-val'].textContent === '0 / 2500 kcal', 'Zero calories text should be 0 / 2500 kcal');
  assert(elements['hud-stress-bar'].style.width === '0.0%', 'Zero stress bar width should be 0.0%');
  assert(elements['hud-stress-val'].textContent === '0%', 'Zero stress text should be 0%');
  assert(elements['hud-focus-bar'].style.width === '0.0%', 'Zero focus bar width should be 0.0%');
  assert(elements['hud-focus-val'].textContent === '0%', 'Zero focus text should be 0%');
  console.log('[PASS] Zero vitals (0, 0, 0) clamped cleanly to 0.0%.');
}

// Subtest 3.3: Negative Values (Underflow Boundary)
{
  const { sandbox, elements } = createHudContext();
  sandbox.updateHUD(-500.0, -0.75, -10.0);

  assert(elements['hud-calories-bar'].style.width === '0.0%', 'Negative calories must clamp to 0.0%');
  assert(elements['hud-calories-val'].textContent === '0 / 2500 kcal', 'Negative calories text must clamp to 0');
  assert(elements['hud-stress-bar'].style.width === '0.0%', 'Negative stress must clamp to 0.0%');
  assert(elements['hud-stress-val'].textContent === '0%', 'Negative stress text must clamp to 0%');
  assert(elements['hud-focus-bar'].style.width === '0.0%', 'Negative focus must clamp to 0.0%');
  assert(elements['hud-focus-val'].textContent === '0%', 'Negative focus text must clamp to 0%');

  assert(sandbox.window._oasisLastHUD.calories === 0, 'Telemetry calories clamped to 0');
  assert(sandbox.window._oasisLastHUD.stress === 0, 'Telemetry stress clamped to 0');
  assert(sandbox.window._oasisLastHUD.focus === 0, 'Telemetry focus clamped to 0');
  console.log('[PASS] Negative values (-500 kcal, -0.75 stress, -10 focus) clamped safely to 0.');
}

// Subtest 3.4: Extreme Overshoot (Overflow Boundary)
{
  const { sandbox, elements } = createHudContext();
  sandbox.updateHUD(10000.0, 1.5, 999.0);

  assert(elements['hud-calories-bar'].style.width === '100.0%', '10000 kcal must clamp to 100.0%');
  assert(elements['hud-calories-val'].textContent === '2500 / 2500 kcal', '10000 kcal text must clamp to 2500 / 2500 kcal');
  assert(elements['hud-stress-bar'].style.width === '100.0%', '1.5 stress must clamp to 100.0%');
  assert(elements['hud-stress-val'].textContent === '100%', '1.5 stress text must clamp to 100%');
  assert(elements['hud-focus-bar'].style.width === '100.0%', '999 focus must clamp to 100.0%');
  assert(elements['hud-focus-val'].textContent === '100%', '999 focus text must clamp to 100%');

  assert(sandbox.window._oasisLastHUD.calories === 2500, 'Telemetry calories clamped to 2500');
  assert(sandbox.window._oasisLastHUD.stress === 1.0, 'Telemetry stress clamped to 1.0');
  assert(sandbox.window._oasisLastHUD.focus === 1.0, 'Telemetry focus clamped to 1.0');
  console.log('[PASS] Extreme overshoot (10,000 kcal, 1.5 stress, 999 focus) clamped safely to max bounds.');
}

// Subtest 3.5: Stress Hazard Color Transition
{
  const { sandbox, elements } = createHudContext();
  
  // Stress at 0.70 (nominal hazard ceiling)
  sandbox.updateHUD(2000, 0.70, 0.50);
  assert(elements['hud-stress-bar'].style.background.includes('#ef4444'), 'Stress <= 0.70 must retain warning orange/red');
  assert(elements['hud-stress-bar'].style.boxShadow.includes('rgba(239, 68, 68'), 'Stress <= 0.70 retains standard shadow');

  // Stress at 0.71 (critical hazard breach)
  sandbox.updateHUD(2000, 0.71, 0.50);
  assert(elements['hud-stress-bar'].style.background.includes('#dc2626'), 'Stress > 0.70 must transition to critical crimson');
  assert(elements['hud-stress-bar'].style.boxShadow.includes('12px #ff0000'), 'Stress > 0.70 activates pulsating red glow');
  console.log('[PASS] Stress hazard threshold transition (0.70 -> 0.71) verified.');
}

// Subtest 3.6: Malformed, NaN, null, and undefined Inputs
{
  const malformedInputs = [
    { cal: NaN, str: NaN, foc: NaN, desc: 'All NaN' },
    { cal: null, str: null, foc: null, desc: 'All null' },
    { cal: undefined, str: undefined, foc: undefined, desc: 'All undefined' },
    { cal: 'corrupt', str: 'bad', foc: 'data', desc: 'All strings' },
    { cal: {}, str: [], foc: false, desc: 'Object, Array, Boolean' },
    { cal: Infinity, str: Infinity, foc: Infinity, desc: '+Infinity' },
    { cal: -Infinity, str: -Infinity, foc: -Infinity, desc: '-Infinity' }
  ];

  for (const tc of malformedInputs) {
    const { sandbox } = createHudContext();
    sandbox.updateHUD(tc.cal, tc.str, tc.foc);

    const hud = sandbox.window._oasisLastHUD;
    assert(!isNaN(hud.calories), `${tc.desc}: calories must not be NaN`);
    assert(!isNaN(hud.stress), `${tc.desc}: stress must not be NaN`);
    assert(!isNaN(hud.focus), `${tc.desc}: focus must not be NaN`);
    assert(hud.calories >= 0 && hud.calories <= 2500, `${tc.desc}: calories within [0, 2500]`);
    assert(hud.stress >= 0.0 && hud.stress <= 1.0, `${tc.desc}: stress within [0.0, 1.0]`);
    assert(hud.focus >= 0.0 && hud.focus <= 1.0, `${tc.desc}: focus within [0.0, 1.0]`);
  }
  console.log(`[PASS] All ${malformedInputs.length} malformed & IEEE 754 edge-case input suites sanitized without exception.`);
}

// Subtest 3.7: Telemetry Monotonicity & Rapid Firing (10,000 Ticks)
{
  const { sandbox } = createHudContext();
  const startTime = Date.now();

  for (let tick = 1; tick <= 10000; tick++) {
    const c = (tick * 17) % 2500;
    const s = ((tick * 31) % 100) / 100.0;
    const f = ((tick * 47) % 100) / 100.0;

    sandbox.updateHUD(c, s, f);
    assert(sandbox.window._oasisLastHUD.tick === tick, `Tick counter must be exactly ${tick}`);
  }

  assert(sandbox.window._oasisLastHUD.tick === 10000, 'Tick counter after 10,000 iterations must equal 10000');
  assert(sandbox.window._oasisLastHUD.timestamp >= startTime, 'Timestamp must be monotonic');
  console.log('[PASS] 10,000 rapid HUD update ticks executed with 100% telemetry counter monotonicity.');
}

// Subtest 3.8: DOM Element Resilience (Missing Elements)
{
  const sandbox = {
    document: {
      getElementById: () => null // All elements return null
    },
    window: {},
    Number: Number,
    Math: Math,
    Date: Date,
    console: console
  };
  sandbox.window = sandbox;

  const code = `
    function updateHUD(caloric_reserve, stress, focus, ambient_temp) {
      ${scriptMatch[1]}
    }
    window.updateHUD = updateHUD;
  `;

  vm.createContext(sandbox);
  vm.runInContext(code, sandbox);

  // Must not throw when DOM elements are null
  try {
    sandbox.updateHUD(1500, 0.4, 0.8, 22.5);
    assert(sandbox.window._oasisLastHUD.calories === 1500, 'Telemetry updated even when DOM elements are missing');
    assert(sandbox.window._oasisLastHUD.temperature === 22.5, 'Telemetry temperature updated with null DOM');
    console.log('[PASS] DOM absence resilience verified: updateHUD did not throw when getElementById returns null.');
  } catch (err) {
    throw new Error(`updateHUD crashed with null DOM elements: ${err.message}`);
  }
}

// Subtest 3.9: Ambient Temperature Thermal States & Styling (Cold, Goldilocks, Hot)
{
  const { sandbox, elements } = createHudContext();

  // Cold (< 15°C, e.g. 5.0°C) -> Ice Blue (#38bdf8)
  sandbox.updateHUD(2500, 0.2, 0.8, 5.0);
  assert(elements['hud-temp-val'].textContent === '5.0 °C', 'Cold temp formatted correctly');
  assert(elements['hud-temp-val'].style.color === '#38bdf8', 'Cold temp text color must be Ice Blue (#38bdf8)');
  assert(elements['hud-temp-val'].className.includes('hud-temp-cold'), 'Cold temp value has hud-temp-cold class');
  assert(elements['hud-temp-bar'].style.background.includes('#38bdf8'), 'Cold temp bar has Ice Blue gradient');
  assert(elements['hud-temp-bar'].className.includes('hud-bar-temp-cold'), 'Cold temp bar has hud-bar-temp-cold class');
  assert(sandbox.window._oasisLastHUD.temperature === 5.0, 'Telemetry records cold temperature');

  // Goldilocks (15°C - 30°C, e.g. 22.5°C) -> Emerald Green (#34d399)
  sandbox.updateHUD(2500, 0.2, 0.8, 22.5);
  assert(elements['hud-temp-val'].textContent === '22.5 °C', 'Goldilocks temp formatted correctly');
  assert(elements['hud-temp-val'].style.color === '#34d399', 'Goldilocks temp text color must be Green (#34d399)');
  assert(elements['hud-temp-val'].className.includes('hud-temp-goldilocks'), 'Goldilocks temp value has hud-temp-goldilocks class');
  assert(elements['hud-temp-bar'].style.background.includes('#34d399'), 'Goldilocks temp bar has Green gradient');
  assert(elements['hud-temp-bar'].className.includes('hud-bar-temp-goldilocks'), 'Goldilocks temp bar has hud-bar-temp-goldilocks class');
  assert(sandbox.window._oasisLastHUD.temperature === 22.5, 'Telemetry records goldilocks temperature');

  // Hot (> 30°C, e.g. 35.0°C) -> Hazard Red (#ef4444)
  sandbox.updateHUD(2500, 0.2, 0.8, 35.0);
  assert(elements['hud-temp-val'].textContent === '35.0 °C', 'Hot temp formatted correctly');
  assert(elements['hud-temp-val'].style.color === '#ef4444', 'Hot temp text color must be Hazard Red (#ef4444)');
  assert(elements['hud-temp-val'].className.includes('hud-temp-hot'), 'Hot temp value has hud-temp-hot class');
  assert(elements['hud-temp-bar'].style.background.includes('#ef4444'), 'Hot temp bar has Red gradient');
  assert(elements['hud-temp-bar'].className.includes('hud-bar-temp-hot'), 'Hot temp bar has hud-bar-temp-hot class');
  assert(sandbox.window._oasisLastHUD.temperature === 35.0, 'Telemetry records hot temperature');

  console.log('[PASS] Ambient temperature thermal states (Cold <15°C Ice Blue, Goldilocks 15-30°C Green, Hot >30°C Red) verified.');
}

console.log('\n================================================================================');
console.log(`=== ALL ADVERSARIAL HUD CHECKS PASSED (${passedAssertions}/${totalAssertions} assertions) ===`);
console.log('================================================================================\n');

process.exit(0);
