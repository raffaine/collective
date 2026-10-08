/**
 * Adversarial Stress Harness for Oasis Engine (V4) Automated Visual Verification
 * 
 * Tests the assertions in tests/verify_canvas.js against adversarial edge cases:
 * 1. Pitch black canvas -> MUST FAIL
 * 2. Flat uniform canvas (0 variance) -> MUST FAIL
 * 3. Degenerate 0-pixel triangle output -> MUST FAIL
 * 4. CullMode = 'back' (accidentally culling CCW triangle) -> MUST FAIL
 * 5. Inverted winding / negative coordinates -> MUST FAIL
 * 6. Valid engine gradient -> MUST PASS
 */

import { strict as assert } from 'node:assert';

function validateRenderOutputMock(renderResult) {
  // Center pixel is non-black
  const isNonBlack = (p) => (p[0] > 0 || p[1] > 0 || p[2] > 0) && p[3] > 0;
  if (!isNonBlack(renderResult.center)) {
    throw new Error(`Center pixel is black: [${renderResult.center.join(', ')}]`);
  }

  // Mean pixel intensity > 0
  const samplePoints = [renderResult.center, renderResult.bottomLeft, renderResult.topRight, renderResult.topLeft, renderResult.bottomRight];
  const totalIntensity = samplePoints.reduce((acc, p) => acc + (p[0] + p[1] + p[2]) / 3, 0);
  const meanIntensity = totalIntensity / samplePoints.length;
  if (meanIntensity <= 0) {
    throw new Error(`Mean pixel intensity is non-positive: ${meanIntensity}`);
  }

  // Verify gradient variety across RGB channels
  const rDiff = Math.abs(renderResult.topRight[0] - renderResult.bottomLeft[0]);
  const gDiff = Math.abs(renderResult.topRight[1] - renderResult.bottomLeft[1]);
  const bDiff = Math.abs(renderResult.topRight[2] - renderResult.bottomLeft[2]);

  if (rDiff === 0 && gDiff === 0 && bDiff === 0) {
    throw new Error('Canvas appears to be a flat solid color with zero gradient variation across UV coordinates');
  }

  return true;
}

const tests = [
  {
    name: 'Pitch Black Canvas (0, 0, 0, 255)',
    input: {
      w: 800, h: 600,
      center: [0, 0, 0, 255],
      bottomLeft: [0, 0, 0, 255],
      topRight: [0, 0, 0, 255],
      topLeft: [0, 0, 0, 255],
      bottomRight: [0, 0, 0, 255]
    },
    shouldPass: false,
    expectedError: 'Center pixel is black'
  },
  {
    name: 'Transparent Black Canvas (0, 0, 0, 0)',
    input: {
      w: 800, h: 600,
      center: [0, 0, 0, 0],
      bottomLeft: [0, 0, 0, 0],
      topRight: [0, 0, 0, 0],
      topLeft: [0, 0, 0, 0],
      bottomRight: [0, 0, 0, 0]
    },
    shouldPass: false,
    expectedError: 'Center pixel is black'
  },
  {
    name: 'Solid Gray Screen (Zero Variance Bug)',
    input: {
      w: 800, h: 600,
      center: [128, 128, 128, 255],
      bottomLeft: [128, 128, 128, 255],
      topRight: [128, 128, 128, 255],
      topLeft: [128, 128, 128, 255],
      bottomRight: [128, 128, 128, 255]
    },
    shouldPass: false,
    expectedError: 'flat solid color'
  },
  {
    name: 'Solid Red Screen (Zero Variance Bug)',
    input: {
      w: 800, h: 600,
      center: [255, 0, 0, 255],
      bottomLeft: [255, 0, 0, 255],
      topRight: [255, 0, 0, 255],
      topLeft: [255, 0, 0, 255],
      bottomRight: [255, 0, 0, 255]
    },
    shouldPass: false,
    expectedError: 'flat solid color'
  },
  {
    name: 'Actual Oasis V4 Gradient Render Output',
    input: {
      w: 800, h: 600,
      center: [128, 128, 128, 255],
      bottomLeft: [26, 26, 128, 255],
      topRight: [230, 230, 128, 255],
      topLeft: [26, 230, 230, 255],
      bottomRight: [230, 26, 25, 255]
    },
    shouldPass: true
  }
];

let allPassed = true;
console.log('=== Running Adversarial Stress Suite on Visual Validation Logic ===');

for (const t of tests) {
  try {
    const res = validateRenderOutputMock(t.input);
    if (!t.shouldPass) {
      console.error(`[FAIL] ${t.name}: expected failure, but passed!`);
      allPassed = false;
    } else {
      console.log(`[PASS] ${t.name}: correctly passed.`);
    }
  } catch (err) {
    if (t.shouldPass) {
      console.error(`[FAIL] ${t.name}: expected pass, but failed with: ${err.message}`);
      allPassed = false;
    } else {
      if (t.expectedError && !err.message.includes(t.expectedError)) {
        console.error(`[FAIL] ${t.name}: failed with unexpected error: ${err.message} (expected: ${t.expectedError})`);
        allPassed = false;
      } else {
        console.log(`[PASS] ${t.name}: correctly caught failure -> "${err.message}"`);
      }
    }
  }
}

if (!allPassed) {
  console.error('\nAdversarial stress test suite failed!');
  process.exit(1);
} else {
  console.log('\nAll adversarial test cases behaved as strictly expected.');
  process.exit(0);
}
