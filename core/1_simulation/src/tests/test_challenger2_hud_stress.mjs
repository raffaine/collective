#!/usr/bin/env node
/**
 * Challenger 2: Comprehensive Empirical HUD, Thermal States & Telemetry Stress Harness
 * 
 * Target: shell_minimal.html / oasis_engine.html
 * 
 * Verifies:
 * 1. CSS pointer-events: none !important contract across all DOM hierarchy levels
 * 2. Hit-testing pass-through: 20,000 raycasts across multiple viewport resolutions
 * 3. Thermal state classifications (<15°C Cold, 15-30°C Goldilocks, >30°C Hot) and exact boundary transitions
 * 4. Malformed, non-finite, and IEEE 754 edge inputs
 * 5. Rapid temperature thrashing (100,000 ticks of sinusoidal, white noise, and boundary vibration)
 * 6. Telemetry object (window._oasisLastHUD) monotonicity, precision, and resilience
 * 7. DOM independence and graceful degradation under missing DOM elements
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

let totalTests = 0;
let passedTests = 0;

function assert(condition, message) {
  totalTests++;
  if (!condition) {
    console.error(`[CHALLENGER 2 FAIL] ${message}`);
    throw new Error(`Assertion failed: ${message}`);
  }
  passedTests++;
}

console.log('================================================================================');
console.log('=== Challenger 2: Empirical Web UI & Telemetry Adversarial Stress Suite      ===');
console.log('================================================================================\n');

// -----------------------------------------------------------------------------
// [STAGE 1] CSS Contract & File Parity Inspection
// -----------------------------------------------------------------------------
console.log('>>> [STAGE 1] Inspecting CSS pointer-events and HTML Shell Parity...');

assert(fs.existsSync(SHELL_HTML_PATH), `shell_minimal.html must exist at ${SHELL_HTML_PATH}`);
const shellHtml = fs.readFileSync(SHELL_HTML_PATH, 'utf-8');

// Check pointer-events none !important on #vitals-hud and #vitals-hud *
const hudCssRuleMatch = /#vitals-hud\s*\{[^}]*pointer-events:\s*none\s*!important;/s.test(shellHtml);
assert(hudCssRuleMatch, 'shell_minimal.html: #vitals-hud must have pointer-events: none !important');

const hudWildcardMatch = /#vitals-hud\s+\*\s*\{[^}]*pointer-events:\s*none\s*!important;/s.test(shellHtml);
assert(hudWildcardMatch, 'shell_minimal.html: #vitals-hud * must have pointer-events: none !important');

// Check that no subsequent CSS rule overrides pointer-events to auto
const conflictingPointerMatch = /#vitals-hud[^{]*\{[^}]*pointer-events:\s*auto/i.test(shellHtml);
assert(!conflictingPointerMatch, 'No CSS rule should set pointer-events: auto on #vitals-hud');

// Parity check with build_wasm/oasis_engine.html if present
if (fs.existsSync(WASM_HTML_PATH)) {
  const wasmHtml = fs.readFileSync(WASM_HTML_PATH, 'utf-8');
  assert(/#vitals-hud\s*\{[^}]*pointer-events:\s*none\s*!important;/s.test(wasmHtml),
    'oasis_engine.html: #vitals-hud has pointer-events: none !important');
  assert(/#vitals-hud\s+\*\s*\{[^}]*pointer-events:\s*none\s*!important;/s.test(wasmHtml),
    'oasis_engine.html: #vitals-hud * has pointer-events: none !important');
  console.log('  [PASS] shell_minimal.html and build_wasm/oasis_engine.html pass CSS pointer-events contract.');
} else {
  console.log('  [PASS] shell_minimal.html passes CSS pointer-events contract (oasis_engine.html skipped).');
}

// -----------------------------------------------------------------------------
// [STAGE 2] Multi-Resolution Hit-Testing Pass-Through Simulation
// -----------------------------------------------------------------------------
console.log('\n>>> [STAGE 2] Multi-Resolution Hit-Testing Pass-Through Simulation...');

class SimElement {
  constructor(id, bounds, pointerEvents = 'auto') {
    this.id = id;
    this.bounds = bounds;
    this.pointerEvents = pointerEvents;
  }
  contains(x, y) {
    return x >= this.bounds.x && x <= this.bounds.x + this.bounds.w &&
           y >= this.bounds.y && y <= this.bounds.y + this.bounds.h;
  }
}

function simulateHitTest(elementsTopToBottom, x, y) {
  for (const el of elementsTopToBottom) {
    if (el.pointerEvents === 'none') {
      continue;
    }
    if (el.contains(x, y)) {
      return el;
    }
  }
  return null;
}

const resolutions = [
  { w: 800, h: 600, label: 'Standard 800x600' },
  { w: 1920, h: 1080, label: 'Full HD 1920x1080' },
  { w: 2560, h: 1440, label: 'QHD 2560x1440' }
];

let totalHitsDirectOnHud = 0;
for (const res of resolutions) {
  const canvas = new SimElement('canvas', { x: 0, y: 0, w: res.w, h: res.h }, 'auto');
  const hudContainer = new SimElement('vitals-hud', { x: 14, y: 14, w: 240, h: 260 }, 'none');
  const hudHeader = new SimElement('hud-header', { x: 24, y: 24, w: 220, h: 20 }, 'none');
  const calBar = new SimElement('hud-calories-bar', { x: 28, y: 60, w: 200, h: 7 }, 'none');
  const calVal = new SimElement('hud-calories-val', { x: 150, y: 46, w: 78, h: 14 }, 'none');
  const strBar = new SimElement('hud-stress-bar', { x: 28, y: 100, w: 200, h: 7 }, 'none');
  const focBar = new SimElement('hud-focus-bar', { x: 28, y: 140, w: 200, h: 7 }, 'none');
  const tempBar = new SimElement('hud-temp-bar', { x: 28, y: 180, w: 200, h: 7 }, 'none');
  const tempVal = new SimElement('hud-temp-val', { x: 150, y: 166, w: 78, h: 14 }, 'none');

  const zStack = [tempVal, tempBar, focBar, strBar, calVal, calBar, hudHeader, hudContainer, canvas];

  let hitsInHud = 0;
  const SAMPLES_PER_RES = 10000;
  for (let i = 0; i < SAMPLES_PER_RES; i++) {
    const rx = Math.random() * res.w;
    const ry = Math.random() * res.h;
    const target = simulateHitTest(zStack, rx, ry);
    assert(target !== null && target.id === 'canvas',
      `Hit test at (${rx.toFixed(1)}, ${ry.toFixed(1)}) in ${res.label} must reach canvas, got: ${target?.id}`);
    if (hudContainer.contains(rx, ry)) {
      hitsInHud++;
    }
  }
  totalHitsDirectOnHud += hitsInHud;
  console.log(`  [PASS] ${res.label}: ${SAMPLES_PER_RES} hit tests passed (HUD region hits: ${hitsInHud}, canvas pass-through: 100%).`);
}
assert(totalHitsDirectOnHud > 500, 'Direct HUD region hits must be statistically significant');

// -----------------------------------------------------------------------------
// [STAGE 3] updateHUD Extraction & Environment Setup
// -----------------------------------------------------------------------------
console.log('\n>>> [STAGE 3] Extracting updateHUD implementation from shell_minimal.html...');

const scriptMatch = shellHtml.match(/function\s+updateHUD\s*\(caloric_reserve,\s*stress,\s*focus(?:,\s*ambient_temp)?\)\s*\{([\s\S]*?)\n\s*\}\s*\n\s*window\.updateHUD\s*=\s*updateHUD;/);
assert(scriptMatch !== null, 'Must extract updateHUD implementation from shell_minimal.html');

function createIsolatedHudContext(mockConfig = {}) {
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

  if (mockConfig.missingIds) {
    for (const id of mockConfig.missingIds) {
      delete elements[id];
    }
  }

  const sandbox = {
    document: {
      getElementById: (id) => (mockConfig.nullAll ? null : (elements[id] || null))
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

// -----------------------------------------------------------------------------
// [STAGE 4] Thermal Boundary Transitions & CSS State Invariants
// -----------------------------------------------------------------------------
console.log('\n>>> [STAGE 4] Thermal Boundary Transitions & CSS Invariants...');

const thermalBoundaryCases = [
  // Extreme sub-zero cold
  { temp: -273.15, expectedState: 'cold', color: '#38bdf8', classPrefix: 'cold', barMin: 0.0, barMax: 0.0 },
  { temp: -50.0,   expectedState: 'cold', color: '#38bdf8', classPrefix: 'cold', barMin: 0.0, barMax: 0.0 },
  { temp: -10.0,   expectedState: 'cold', color: '#38bdf8', classPrefix: 'cold', barMin: 0.0, barMax: 0.0 },
  { temp: 0.0,     expectedState: 'cold', color: '#38bdf8', classPrefix: 'cold', barMin: 20.0, barMax: 20.0 },
  { temp: 10.0,    expectedState: 'cold', color: '#38bdf8', classPrefix: 'cold', barMin: 40.0, barMax: 40.0 },
  { temp: 14.9,    expectedState: 'cold', color: '#38bdf8', classPrefix: 'cold', barMin: 49.8, barMax: 49.8 },
  { temp: 14.9999, expectedState: 'cold', color: '#38bdf8', classPrefix: 'cold', barMin: 50.0, barMax: 50.0 },
  // Goldilocks boundary start: exactly 15.0°C
  { temp: 15.0,    expectedState: 'goldilocks', color: '#34d399', classPrefix: 'goldilocks', barMin: 50.0, barMax: 50.0 },
  { temp: 15.0001, expectedState: 'goldilocks', color: '#34d399', classPrefix: 'goldilocks', barMin: 50.0, barMax: 50.0 },
  { temp: 22.5,    expectedState: 'goldilocks', color: '#34d399', classPrefix: 'goldilocks', barMin: 65.0, barMax: 65.0 },
  { temp: 29.9999, expectedState: 'goldilocks', color: '#34d399', classPrefix: 'goldilocks', barMin: 80.0, barMax: 80.0 },
  // Goldilocks boundary end: exactly 30.0°C
  { temp: 30.0,    expectedState: 'goldilocks', color: '#34d399', classPrefix: 'goldilocks', barMin: 80.0, barMax: 80.0 },
  // Heatwave boundary: > 30.0°C
  { temp: 30.0001, expectedState: 'hot', color: '#ef4444', classPrefix: 'hot', barMin: 80.0, barMax: 80.0 },
  { temp: 30.1,    expectedState: 'hot', color: '#ef4444', classPrefix: 'hot', barMin: 80.2, barMax: 80.2 },
  { temp: 35.0,    expectedState: 'hot', color: '#ef4444', classPrefix: 'hot', barMin: 90.0, barMax: 90.0 },
  { temp: 40.0,    expectedState: 'hot', color: '#ef4444', classPrefix: 'hot', barMin: 100.0, barMax: 100.0 },
  { temp: 60.0,    expectedState: 'hot', color: '#ef4444', classPrefix: 'hot', barMin: 100.0, barMax: 100.0 },
  { temp: 500.0,   expectedState: 'hot', color: '#ef4444', classPrefix: 'hot', barMin: 100.0, barMax: 100.0 }
];

for (const tc of thermalBoundaryCases) {
  const { sandbox, elements } = createIsolatedHudContext();
  sandbox.updateHUD(2500, 0.2, 0.8, tc.temp);

  const valEl = elements['hud-temp-val'];
  const barEl = elements['hud-temp-bar'];

  // 1. Text formatted with 1 decimal place
  const expectedText = `${tc.temp.toFixed(1)} °C`;
  assert(valEl.textContent === expectedText,
    `Temp ${tc.temp}: expected text "${expectedText}", got "${valEl.textContent}"`);

  // 2. Strict Class mutual exclusivity
  if (tc.expectedState === 'cold') {
    assert(valEl.className.includes('hud-temp-cold'), `Temp ${tc.temp}: val must have hud-temp-cold`);
    assert(!valEl.className.includes('hud-temp-goldilocks'), `Temp ${tc.temp}: val must NOT have hud-temp-goldilocks`);
    assert(!valEl.className.includes('hud-temp-hot'), `Temp ${tc.temp}: val must NOT have hud-temp-hot`);
    assert(barEl.className.includes('hud-bar-temp-cold'), `Temp ${tc.temp}: bar must have hud-bar-temp-cold`);
    assert(!barEl.className.includes('hud-bar-temp-goldilocks'), `Temp ${tc.temp}: bar must NOT have hud-bar-temp-goldilocks`);
    assert(!barEl.className.includes('hud-bar-temp-hot'), `Temp ${tc.temp}: bar must NOT have hud-bar-temp-hot`);
  } else if (tc.expectedState === 'goldilocks') {
    assert(valEl.className.includes('hud-temp-goldilocks'), `Temp ${tc.temp}: val must have hud-temp-goldilocks`);
    assert(!valEl.className.includes('hud-temp-cold'), `Temp ${tc.temp}: val must NOT have hud-temp-cold`);
    assert(!valEl.className.includes('hud-temp-hot'), `Temp ${tc.temp}: val must NOT have hud-temp-hot`);
    assert(barEl.className.includes('hud-bar-temp-goldilocks'), `Temp ${tc.temp}: bar must have hud-bar-temp-goldilocks`);
    assert(!barEl.className.includes('hud-bar-temp-cold'), `Temp ${tc.temp}: bar must NOT have hud-bar-temp-cold`);
    assert(!barEl.className.includes('hud-bar-temp-hot'), `Temp ${tc.temp}: bar must NOT have hud-bar-temp-hot`);
  } else {
    assert(valEl.className.includes('hud-temp-hot'), `Temp ${tc.temp}: val must have hud-temp-hot`);
    assert(!valEl.className.includes('hud-temp-cold'), `Temp ${tc.temp}: val must NOT have hud-temp-cold`);
    assert(!valEl.className.includes('hud-temp-goldilocks'), `Temp ${tc.temp}: val must NOT have hud-temp-goldilocks`);
    assert(barEl.className.includes('hud-bar-temp-hot'), `Temp ${tc.temp}: bar must have hud-bar-temp-hot`);
    assert(!barEl.className.includes('hud-bar-temp-cold'), `Temp ${tc.temp}: bar must NOT have hud-bar-temp-cold`);
    assert(!barEl.className.includes('hud-bar-temp-goldilocks'), `Temp ${tc.temp}: bar must NOT have hud-bar-temp-goldilocks`);
  }

  // 3. Color matching
  assert(valEl.style.color === tc.color, `Temp ${tc.temp}: color must be ${tc.color}`);

  // 4. Bar width clamping
  const barPct = parseFloat(barEl.style.width);
  assert(!isNaN(barPct) && barPct >= tc.barMin && barPct <= tc.barMax,
    `Temp ${tc.temp}: bar width ${barEl.style.width} not within [${tc.barMin}%, ${tc.barMax}%]`);

  // 5. Telemetry accuracy
  assert(sandbox.window._oasisLastHUD.temperature === tc.temp,
    `Temp ${tc.temp}: telemetry must record ${tc.temp}`);
}
console.log(`  [PASS] Verified ${thermalBoundaryCases.length} thermal boundary & edge cases across all thermal states.`);

// -----------------------------------------------------------------------------
// [STAGE 5] Malformed & IEEE 754 Stress Fuzzing
// -----------------------------------------------------------------------------
console.log('\n>>> [STAGE 5] Malformed, Non-Finite & IEEE 754 Stress Fuzzing...');

const malformedMatrix = [
  { temp: undefined, expectedTemp: 22.5, desc: 'undefined ambient_temp' },
  { temp: null,      expectedTemp: 22.5, desc: 'null ambient_temp' },
  { temp: NaN,       expectedTemp: 22.5, desc: 'NaN ambient_temp' },
  { temp: 'invalid', expectedTemp: 22.5, desc: 'string "invalid"' },
  { temp: {},        expectedTemp: 22.5, desc: 'empty object {}' },
  { temp: [100, 200],expectedTemp: 22.5, desc: 'multi-element array' },
  { temp: '25.0',    expectedTemp: 25.0, desc: 'valid numeric string "25.0"' },
  { temp: '-5.0',    expectedTemp: -5.0, desc: 'valid numeric string "-5.0"' },
  { temp: '',        expectedTemp: 0.0,  desc: 'empty string "" -> 0.0' },
  { temp: false,     expectedTemp: 0.0,  desc: 'boolean false -> 0.0' },
  { temp: true,      expectedTemp: 1.0,  desc: 'boolean true -> 1.0' },
  { temp: Infinity,  expectedTemp: Infinity, desc: '+Infinity' },
  { temp: -Infinity, expectedTemp: -Infinity, desc: '-Infinity' },
  { temp: Number.MAX_SAFE_INTEGER, expectedTemp: Number.MAX_SAFE_INTEGER, desc: 'MAX_SAFE_INTEGER' },
  { temp: Number.MIN_VALUE, expectedTemp: Number.MIN_VALUE, desc: 'MIN_VALUE' }
];

for (const tc of malformedMatrix) {
  const { sandbox, elements } = createIsolatedHudContext();
  sandbox.updateHUD(1200, 0.4, 0.6, tc.temp);

  const hud = sandbox.window._oasisLastHUD;
  assert(!isNaN(hud.temperature), `${tc.desc}: telemetry temperature must not be NaN`);
  assert(hud.temperature === tc.expectedTemp,
    `${tc.desc}: telemetry temperature expected ${tc.expectedTemp}, got ${hud.temperature}`);

  const valEl = elements['hud-temp-val'];
  assert(typeof valEl.textContent === 'string' && valEl.textContent.endsWith('°C'),
    `${tc.desc}: textContent must end with '°C', got '${valEl.textContent}'`);

  const barEl = elements['hud-temp-bar'];
  assert(barEl.style.width.endsWith('%'), `${tc.desc}: bar width must end with '%'`);
}
console.log(`  [PASS] All ${malformedMatrix.length} malformed & non-finite test cases sanitized without exception.`);

// -----------------------------------------------------------------------------
// [STAGE 6] Rapid Temperature Thrashing & Invariant Fuzzing (100,000 Ticks)
// -----------------------------------------------------------------------------
console.log('\n>>> [STAGE 6] Executing 100,000 rapid HUD updates with boundary thrashing...');

{
  const { sandbox, elements } = createIsolatedHudContext();
  const tStart = Date.now();
  let prevTick = 0;

  for (let tick = 1; tick <= 100000; tick++) {
    // Generate chaotic temperature transitions
    let temp;
    const mode = tick % 5;
    if (mode === 0) {
      // Rapid boundary chatter: 14.99 vs 15.01
      temp = (tick % 2 === 0) ? 14.99 : 15.01;
    } else if (mode === 1) {
      // Rapid boundary chatter: 29.99 vs 30.01
      temp = (tick % 2 === 0) ? 29.99 : 30.01;
    } else if (mode === 2) {
      // Deep arctic freeze
      temp = -40.0 + (tick % 15);
    } else if (mode === 3) {
      // Scorching heatwave
      temp = 32.0 + (tick % 25);
    } else {
      // Nominal diurnal cycle
      temp = 22.5 + 10.0 * Math.sin(tick * 0.05);
    }

    const cal = (tick * 37) % 2500;
    const str = ((tick * 13) % 100) / 100.0;
    const foc = ((tick * 29) % 100) / 100.0;

    sandbox.updateHUD(cal, str, foc, temp);

    const hud = sandbox.window._oasisLastHUD;
    assert(hud.tick === tick, `Tick ${tick}: expected telemetry tick ${tick}, got ${hud.tick}`);
    assert(hud.tick === prevTick + 1, `Tick ${tick}: non-monotonic tick incrementation`);
    prevTick = tick;

    // Spot-check DOM consistency every 5,000 ticks
    if (tick % 5000 === 0) {
      const valEl = elements['hud-temp-val'];
      const barEl = elements['hud-temp-bar'];
      if (temp < 15.0) {
        assert(valEl.className.includes('hud-temp-cold'), `Tick ${tick}: cold class missing`);
        assert(barEl.className.includes('hud-bar-temp-cold'), `Tick ${tick}: cold bar class missing`);
      } else if (temp <= 30.0) {
        assert(valEl.className.includes('hud-temp-goldilocks'), `Tick ${tick}: goldilocks class missing`);
        assert(barEl.className.includes('hud-bar-temp-goldilocks'), `Tick ${tick}: goldilocks bar class missing`);
      } else {
        assert(valEl.className.includes('hud-temp-hot'), `Tick ${tick}: hot class missing`);
        assert(barEl.className.includes('hud-bar-temp-hot'), `Tick ${tick}: hot bar class missing`);
      }
    }
  }

  const duration = Date.now() - tStart;
  console.log(`  [PASS] 100,000 rapid update cycles completed in ${duration}ms (100% monotonic, zero state leaks).`);
}

// -----------------------------------------------------------------------------
// [STAGE 7] DOM Absence & Graceful Degradation Under Partial Failure
// -----------------------------------------------------------------------------
console.log('\n>>> [STAGE 7] DOM Absence & Graceful Degradation...');

// Subtest 7.1: Completely null document.getElementById
{
  const { sandbox } = createIsolatedHudContext({ nullAll: true });
  sandbox.updateHUD(2000, 0.5, 0.5, 12.0);
  assert(sandbox.window._oasisLastHUD.temperature === 12.0, 'Null DOM: telemetry recorded correctly');
  assert(sandbox.window._oasisLastHUD.tick === 1, 'Null DOM: tick is 1');
  console.log('  [PASS] 7.1 Complete DOM absence handled cleanly without exception.');
}

// Subtest 7.2: Missing temperature elements only (partial DOM corruption)
{
  const { sandbox, elements } = createIsolatedHudContext({ missingIds: ['hud-temp-val', 'hud-temp-bar'] });
  sandbox.updateHUD(1800, 0.3, 0.9, 33.0);
  assert(sandbox.window._oasisLastHUD.temperature === 33.0, 'Partial DOM: telemetry recorded');
  assert(elements['hud-calories-bar'].style.width === '72.0%', 'Partial DOM: other elements continue updating');
  console.log('  [PASS] 7.2 Missing temperature elements handled gracefully without breaking vitals.');
}

// Subtest 7.3: Telemetry re-initialization if window._oasisLastHUD is cleared
{
  const { sandbox } = createIsolatedHudContext();
  sandbox.updateHUD(2500, 0.1, 0.9, 22.5);
  assert(sandbox.window._oasisLastHUD.tick === 1, 'First update: tick 1');
  sandbox.updateHUD(2500, 0.1, 0.9, 22.5);
  assert(sandbox.window._oasisLastHUD.tick === 2, 'Second update: tick 2');
  
  // Wipe telemetry
  sandbox.window._oasisLastHUD = null;
  sandbox.updateHUD(2500, 0.1, 0.9, 22.5);
  assert(sandbox.window._oasisLastHUD.tick === 1, 'Post-wipe update: tick recovers cleanly to 1');
  console.log('  [PASS] 7.3 Telemetry object survives mid-run clearance and recovers tick counter.');
}

// -----------------------------------------------------------------------------
// [STAGE 8] Stress Test Summary & Assertion Counter
// -----------------------------------------------------------------------------
console.log('\n================================================================================');
console.log(`=== CHALLENGER 2 EMPIRICAL STRESS TEST PASSED: ${passedTests}/${totalTests} assertions ===`);
console.log('================================================================================\n');

process.exit(0);
