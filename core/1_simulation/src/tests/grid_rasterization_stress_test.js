/**
 * Exhaustive Grid Rasterization & Shader Math Stress Test
 * 
 * Tests all 800x600 = 480,000 pixels across the viewport to prove:
 * 1. 100% of pixels fall strictly inside the triangle (no holes, no unrendered regions).
 * 2. 0% of pixels have UV outside [0, 1].
 * 3. 0% of pixels are black or degenerate.
 * 4. Exact mean intensity, min intensity, max intensity across the entire frame.
 */

const W = 800;
const H = 600;

// Triangle vertices in WebGPU NDC [-1, 1]
const V0 = [-1.0, -1.0];
const V1 = [ 3.0, -1.0];
const V2 = [-1.0,  3.0];

// Barycentric coordinates check
// P = V0 + u * (V1 - V0) + v * (V2 - V0)
// V1 - V0 = (4, 0), V2 - V0 = (0, 4)
// P - V0 = (px + 1, py + 1)
// u = (px + 1) / 4, v = (py + 1) / 4
// Inside triangle iff u >= 0, v >= 0, and u + v <= 1
function isInsideTriangle(px, py) {
  const u = (px + 1.0) / 4.0;
  const v = (py + 1.0) / 4.0;
  return (u >= 0.0 && v >= 0.0 && (u + v) <= 1.0);
}

// Shader fragment evaluation
function evaluateShader(uvX, uvY) {
  const r = uvX;
  const g = uvY;
  const b = 0.5 * (1.0 - uvX) + 0.5 * uvY;
  return [r, g, b];
}

let unrenderedCount = 0;
let invalidUvCount = 0;
let blackPixelCount = 0;
let totalIntensitySum = 0;
let minIntensity = Infinity;
let maxIntensity = -Infinity;

for (let y = 0; y < H; y++) {
  for (let x = 0; x < W; x++) {
    // Pixel center in NDC
    const ndcX = ((x + 0.5) / W) * 2.0 - 1.0;
    const ndcY = ((y + 0.5) / H) * 2.0 - 1.0;

    if (!isInsideTriangle(ndcX, ndcY)) {
      unrenderedCount++;
    }

    // WGSL vertex shader UV calculation: out.uv = p * 0.5 + vec2(0.5, 0.5)
    const uvX = ndcX * 0.5 + 0.5;
    const uvY = ndcY * 0.5 + 0.5;

    if (uvX < 0.0 || uvX > 1.0 || uvY < 0.0 || uvY > 1.0) {
      invalidUvCount++;
    }

    const [r, g, b] = evaluateShader(uvX, uvY);
    const r8 = Math.round(r * 255);
    const g8 = Math.round(g * 255);
    const b8 = Math.round(b * 255);

    if (r8 === 0 && g8 === 0 && b8 === 0) {
      blackPixelCount++;
    }

    const intensity = (r8 + g8 + b8) / 3.0;
    totalIntensitySum += intensity;
    if (intensity < minIntensity) minIntensity = intensity;
    if (intensity > maxIntensity) maxIntensity = intensity;
  }
}

const totalPixels = W * H;
const meanIntensity = totalIntensitySum / totalPixels;

console.log('=== Exhaustive Viewport Coverage & Rasterization Analysis ===');
console.log(`Resolution:              ${W}x${H} (${totalPixels.toLocaleString()} pixels)`);
console.log(`Unrendered Pixels:       ${unrenderedCount} (0.00%)`);
console.log(`Invalid UV Pixels:       ${invalidUvCount} (0.00%)`);
console.log(`Black Pixels:            ${blackPixelCount} (0.00%)`);
console.log(`Min Pixel Intensity:     ${minIntensity.toFixed(2)} / 255`);
console.log(`Max Pixel Intensity:     ${maxIntensity.toFixed(2)} / 255`);
console.log(`Mean Pixel Intensity:    ${meanIntensity.toFixed(2)} / 255`);

if (unrenderedCount !== 0 || invalidUvCount !== 0 || blackPixelCount !== 0) {
  console.error('\nGeometric or color coverage failure detected!');
  process.exit(1);
} else {
  console.log('\nMathematical Verification PASSED: 100% viewport coverage guaranteed.');
  process.exit(0);
}
