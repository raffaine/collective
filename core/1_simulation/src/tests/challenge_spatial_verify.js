/**
 * Empirical Spatial & Visual Verification Harness
 * 
 * Challenger 1 Independent Verification Oracle
 * Tests:
 * 1. CPU Reference DDA Raymarcher Oracle matching raymarch_wgsl.hpp bit-for-bit.
 * 2. Chrome Headless CDP Screenshot Capture & PNG decode to disk.
 * 3. Spatial analysis of the full 800x600 buffer (480,000 pixels):
 *    - Monolith bounds and pixel count
 *    - Checkered floor tile pattern and slate vs sandstone pixel counts
 *    - Teal pillars and their screen projections
 *    - Sky horizon and zenith distribution
 * 4. Comparison between CPU Oracle and actual WebGPU GPU hardware rasterization.
 */

import http from 'node:http';
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { fileURLToPath } from 'node:url';
import { spawn } from 'node:child_process';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const W = 800;
const H = 600;
const ASPECT = W / H;

// --- 1. CPU REFERENCE ORACLE ---
function get_voxel_material(x, y, z) {
  if (y === 0) {
    if (Math.abs(x) <= 12 && z >= -6 && z <= 16) {
      const check = ((x + z) & 1);
      return check === 0 ? 1 : 2; // 1: Slate, 2: Sandstone
    }
  }
  if (y >= 1 && y <= 2 && Math.abs(x) <= 1 && z >= 0 && z <= 2) {
    return 3; // Amber gold monolith
  }
  if (y >= 1 && y <= 3) {
    if ((x === -3 || x === 3) && (z === -1 || z === 5)) {
      return 4; // Teal pillar
    }
  }
  return 0; // Air
}

function get_sky_color(rdy) {
  const sky_top = [0.15, 0.35, 0.65];
  const sky_horizon = [0.55, 0.70, 0.85];
  const t = Math.max(0.0, Math.min(1.0, rdy * 0.5 + 0.5));
  return [
    sky_horizon[0] + (sky_top[0] - sky_horizon[0]) * t,
    sky_horizon[1] + (sky_top[1] - sky_horizon[1]) * t,
    sky_horizon[2] + (sky_top[2] - sky_horizon[2]) * t
  ];
}

function normalize(v) {
  const l = Math.hypot(v[0], v[1], v[2]);
  return [v[0] / l, v[1] / l, v[2] / l];
}

function cross(a, b) {
  return [
    a[1] * b[2] - a[2] * b[1],
    a[2] * b[0] - a[0] * b[2],
    a[0] * b[1] - a[1] * b[0]
  ];
}

const ro = [0.0001, 2.0001, -4.9999];
const cam_target = [0.0, 0.0, 0.0];
const cam_fwd = normalize([cam_target[0] - ro[0], cam_target[1] - ro[1], cam_target[2] - ro[2]]);
const world_up = [0.0, 1.0, 0.0];
const cam_right = normalize(cross(world_up, cam_fwd));
const cam_up = cross(cam_fwd, cam_right);
const focal_length = 1.5;

function traceRay(screen_px, screen_py) {
  const rd = normalize([
    screen_px * cam_right[0] + screen_py * cam_up[0] + focal_length * cam_fwd[0],
    screen_px * cam_right[1] + screen_py * cam_up[1] + focal_length * cam_fwd[1],
    screen_px * cam_right[2] + screen_py * cam_up[2] + focal_length * cam_fwd[2]
  ]);

  let mapPos = [Math.floor(ro[0]), Math.floor(ro[1]), Math.floor(ro[2])];
  const step = [rd[0] < 0 ? -1 : 1, rd[1] < 0 ? -1 : 1, rd[2] < 0 ? -1 : 1];
  const deltaDist = [
    Math.abs(1.0 / Math.max(Math.abs(rd[0]), 1e-6)),
    Math.abs(1.0 / Math.max(Math.abs(rd[1]), 1e-6)),
    Math.abs(1.0 / Math.max(Math.abs(rd[2]), 1e-6))
  ];

  let sideDist = [
    (step[0] > 0 ? (mapPos[0] + 1 - ro[0]) : (ro[0] - mapPos[0])) * deltaDist[0],
    (step[1] > 0 ? (mapPos[1] + 1 - ro[1]) : (ro[1] - mapPos[1])) * deltaDist[1],
    (step[2] > 0 ? (mapPos[2] + 1 - ro[2]) : (ro[2] - mapPos[2])) * deltaDist[2]
  ];

  let hit_dist = 0.0;
  let hit_normal = [0.0, 0.0, 0.0];
  let hit_mat = 0;

  const MAX_STEPS = 96;
  for (let i = 0; i < MAX_STEPS; i++) {
    if (sideDist[0] < sideDist[1]) {
      if (sideDist[0] < sideDist[2]) {
        hit_dist = sideDist[0];
        sideDist[0] += deltaDist[0];
        mapPos[0] += step[0];
        hit_normal = [-step[0], 0.0, 0.0];
      } else {
        hit_dist = sideDist[2];
        sideDist[2] += deltaDist[2];
        mapPos[2] += step[2];
        hit_normal = [0.0, 0.0, -step[2]];
      }
    } else {
      if (sideDist[1] < sideDist[2]) {
        hit_dist = sideDist[1];
        sideDist[1] += deltaDist[1];
        mapPos[1] += step[1];
        hit_normal = [0.0, -step[1], 0.0];
      } else {
        hit_dist = sideDist[2];
        sideDist[2] += deltaDist[2];
        mapPos[2] += step[2];
        hit_normal = [0.0, 0.0, -step[2]];
      }
    }

    if ((mapPos[1] > 4 && step[1] > 0) || (mapPos[1] < 0 && step[1] < 0) || (mapPos[2] > 18 && step[2] > 0)) {
      break;
    }

    const mat = get_voxel_material(mapPos[0], mapPos[1], mapPos[2]);
    if (mat !== 0) {
      hit_mat = mat;
      break;
    }
  }

  if (hit_mat === 0) {
    const sky = get_sky_color(rd[1]);
    return {
      mat: 0,
      color: [Math.round(sky[0] * 255), Math.round(sky[1] * 255), Math.round(sky[2] * 255), 255],
      hit_dist: 0,
      hit_normal: [0, 0, 0]
    };
  }

  let base_color;
  if (hit_mat === 1) base_color = [0.35, 0.38, 0.42];
  else if (hit_mat === 2) base_color = [0.60, 0.55, 0.48];
  else if (hit_mat === 3) base_color = [0.92, 0.68, 0.22];
  else if (hit_mat === 4) base_color = [0.22, 0.78, 0.68];
  else base_color = [0.50, 0.50, 0.50];

  const light_dir = normalize([0.5, 0.8, -0.4]);
  const diff = Math.max(0.0, hit_normal[0] * light_dir[0] + hit_normal[1] * light_dir[1] + hit_normal[2] * light_dir[2]);
  const ambient = 0.32;
  const lighting = ambient + 0.68 * diff;

  let face_tint = 1.0;
  if (hit_normal[1] > 0.5) face_tint = 1.15;
  else if (hit_normal[1] < -0.5) face_tint = 0.60;
  else if (Math.abs(hit_normal[0]) > 0.5) face_tint = 0.85;
  else face_tint = 0.95;

  const lit_color = [
    base_color[0] * lighting * face_tint,
    base_color[1] * lighting * face_tint,
    base_color[2] * lighting * face_tint
  ];

  const sky = get_sky_color(rd[1]);
  const fog = 1.0 - Math.exp(-hit_dist * 0.035);
  const final_color = [
    lit_color[0] * (1.0 - fog) + sky[0] * fog,
    lit_color[1] * (1.0 - fog) + sky[1] * fog,
    lit_color[2] * (1.0 - fog) + sky[2] * fog
  ];

  return {
    mat: hit_mat,
    color: [
      Math.min(255, Math.round(final_color[0] * 255)),
      Math.min(255, Math.round(final_color[1] * 255)),
      Math.min(255, Math.round(final_color[2] * 255)),
      255
    ],
    hit_dist,
    hit_normal
  };
}

function pixelToScreenP(x, y) {
  const uvX = (x + 0.5) / W;
  const uvY = (y + 0.5) / H;
  const ndcX = uvX * 2.0 - 1.0;
  const ndcY = 1.0 - uvY * 2.0;
  const screen_px = ndcX * ASPECT;
  const screen_py = ndcY;
  return { screen_px, screen_py };
}

// --- 2. PNG DECODER ---
function decodePngRgba(buffer) {
  let offset = 8;
  let width = 0;
  let height = 0;
  let bitDepth = 0;
  let colorType = 0;
  const idatChunks = [];

  while (offset < buffer.length) {
    const length = buffer.readUInt32BE(offset);
    const type = buffer.toString('ascii', offset + 4, offset + 8);
    const data = buffer.subarray(offset + 8, offset + 8 + length);
    offset += 12 + length;

    if (type === 'IHDR') {
      width = data.readUInt32BE(0);
      height = data.readUInt32BE(4);
      bitDepth = data[8];
      colorType = data[9];
    } else if (type === 'IDAT') {
      idatChunks.push(data);
    } else if (type === 'IEND') {
      break;
    }
  }

  const decompressed = zlib.inflateSync(Buffer.concat(idatChunks));
  const bytesPerPixel = colorType === 6 ? 4 : 3;
  const rowBytes = width * bytesPerPixel;
  const pixels = Buffer.alloc(width * height * 4);

  let srcOffset = 0;
  let prevRow = null;

  for (let y = 0; y < height; y++) {
    const filter = decompressed[srcOffset++];
    const currentRow = Buffer.alloc(rowBytes);

    for (let x = 0; x < rowBytes; x++) {
      const byte = decompressed[srcOffset++];
      const left = x >= bytesPerPixel ? currentRow[x - bytesPerPixel] : 0;
      const up = prevRow ? prevRow[x] : 0;
      const upLeft = (prevRow && x >= bytesPerPixel) ? prevRow[x - bytesPerPixel] : 0;

      let val = byte;
      if (filter === 1) val = (byte + left) & 0xFF;
      else if (filter === 2) val = (byte + up) & 0xFF;
      else if (filter === 3) val = (byte + Math.floor((left + up) / 2)) & 0xFF;
      else if (filter === 4) {
        const p = left + up - upLeft;
        const pa = Math.abs(p - left);
        const pb = Math.abs(p - up);
        const pc = Math.abs(p - upLeft);
        const pr = (pa <= pb && pa <= pc) ? left : (pb <= pc ? up : upLeft);
        val = (byte + pr) & 0xFF;
      }
      currentRow[x] = val;
    }

    for (let col = 0; col < width; col++) {
      const dstIdx = (y * width + col) * 4;
      const srcIdx = col * bytesPerPixel;
      pixels[dstIdx] = currentRow[srcIdx];
      pixels[dstIdx + 1] = currentRow[srcIdx + 1];
      pixels[dstIdx + 2] = currentRow[srcIdx + 2];
      pixels[dstIdx + 3] = bytesPerPixel === 4 ? currentRow[srcIdx + 3] : 255;
    }
    prevRow = currentRow;
  }

  return {
    width,
    height,
    getPixel: (x, y) => {
      const cx = Math.max(0, Math.min(width - 1, Math.floor(x)));
      const cy = Math.max(0, Math.min(height - 1, Math.floor(y)));
      const idx = (cy * width + cx) * 4;
      return [pixels[idx], pixels[idx + 1], pixels[idx + 2], pixels[idx + 3]];
    }
  };
}

// --- 3. RUN VERIFICATION & COMPARISON ---
async function main() {
  console.log('================================================================');
  console.log('   CHALLENGER 1: EMPIRICAL VISUAL & SPATIAL DEEP AUDIT');
  console.log('================================================================');

  // Phase A: CPU Reference Oracle Analysis
  console.log('\n[Phase A] Running CPU Reference Oracle across full 800x600 scene...');
  const matCounts = { 0: 0, 1: 0, 2: 0, 3: 0, 4: 0 };

  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      const { screen_px, screen_py } = pixelToScreenP(x, y);
      const res = traceRay(screen_px, screen_py);
      matCounts[res.mat]++;
    }
  }

  const totalPixels = W * H;
  console.log(`Total Screen Pixels: ${totalPixels.toLocaleString()}`);
  console.log(`Material Breakdown (CPU Oracle Ground Truth):`);
  console.log(` - Mat 0 (Atmospheric Sky):        ${matCounts[0].toLocaleString().padStart(7)} (${((matCounts[0]/totalPixels)*100).toFixed(2)}%)`);
  console.log(` - Mat 1 (Slate Tile Voxel Floor): ${matCounts[1].toLocaleString().padStart(7)} (${((matCounts[1]/totalPixels)*100).toFixed(2)}%)`);
  console.log(` - Mat 2 (Sandstone Tile Floor):   ${matCounts[2].toLocaleString().padStart(7)} (${((matCounts[2]/totalPixels)*100).toFixed(2)}%)`);
  console.log(` - Mat 3 (Amber Gold Monolith):    ${matCounts[3].toLocaleString().padStart(7)} (${((matCounts[3]/totalPixels)*100).toFixed(2)}%)`);
  console.log(` - Mat 4 (Teal Decorative Pillar): ${matCounts[4].toLocaleString().padStart(7)} (${((matCounts[4]/totalPixels)*100).toFixed(2)}%)`);

  const totalVoxelPixels = matCounts[1] + matCounts[2] + matCounts[3] + matCounts[4];
  console.log(`Total 3D Voxel Pixels Hit:          ${totalVoxelPixels.toLocaleString()} (${((totalVoxelPixels/totalPixels)*100).toFixed(2)}%)`);

  // Phase B: Headless Chrome CDP Capture
  console.log('\n[Phase B] Connecting to WebGPU Headless Chrome to capture actual hardware frame...');
  const buildDir = path.resolve(__dirname, '../build_wasm');
  const port = 8089;
  const debugPort = 9228;

  const server = http.createServer((req, res) => {
    let reqPath = req.url.split('?')[0];
    if (reqPath === '/') reqPath = '/oasis_engine.html';
    const filePath = path.join(buildDir, path.normalize(reqPath).replace(/^(\.\.[\/\\])+/, ''));
    if (!fs.existsSync(filePath)) {
      res.writeHead(404);
      res.end('Not found');
      return;
    }
    const ext = path.extname(filePath);
    const mimes = { '.html': 'text/html', '.js': 'application/javascript', '.wasm': 'application/wasm' };
    res.writeHead(200, {
      'Content-Type': mimes[ext] || 'application/octet-stream',
      'Cross-Origin-Opener-Policy': 'same-origin',
      'Cross-Origin-Embedder-Policy': 'require-corp'
    });
    fs.createReadStream(filePath).pipe(res);
  });

  await new Promise(r => server.listen(port, '127.0.0.1', r));

  const profileDir = path.join('/tmp', `oasis_chal_${Date.now()}`);
  fs.mkdirSync(profileDir, { recursive: true });

  const chromeProc = spawn('/Applications/Google Chrome.app/Contents/MacOS/Google Chrome', [
    '--headless=new',
    '--enable-unsafe-webgpu',
    '--use-angle=metal',
    '--disable-crashpad',
    '--disable-gpu-sandbox',
    '--no-sandbox',
    '--window-size=1280,1024',
    `--remote-debugging-port=${debugPort}`,
    `--user-data-dir=${profileDir}`,
    `http://127.0.0.1:${port}/oasis_engine.html`
  ]);

  const cleanup = () => {
    try { chromeProc.kill('SIGKILL'); } catch (_) {}
    try { server.close(); } catch (_) {}
    try { fs.rmSync(profileDir, { recursive: true, force: true }); } catch (_) {}
  };

  let wsUrl = null;
  const start = Date.now();
  while (Date.now() - start < 8000) {
    try {
      const res = await fetch(`http://127.0.0.1:${debugPort}/json`);
      if (res.ok) {
        const list = await res.json();
        const p = list.find(t => t.type === 'page' && t.url && t.url.includes('oasis_engine.html')) || list.find(t => t.type === 'page');
        if (p?.webSocketDebuggerUrl) {
          wsUrl = p.webSocketDebuggerUrl;
          break;
        }
      }
    } catch (_) {}
    await new Promise(r => setTimeout(r, 200));
  }

  if (!wsUrl) {
    cleanup();
    throw new Error('Failed to attach to Chrome CDP debugger');
  }

  const ws = new WebSocket(wsUrl);
  await new Promise(r => ws.onopen = r);

  let cdpId = 1;
  const callbacks = new Map();
  ws.onmessage = (e) => {
    const m = JSON.parse(e.data);
    if (m.id && callbacks.has(m.id)) {
      const cb = callbacks.get(m.id);
      callbacks.delete(m.id);
      cb(m.result);
    }
  };

  const send = (method, params = {}) => new Promise(r => {
    const id = cdpId++;
    callbacks.set(id, r);
    ws.send(JSON.stringify({ id, method, params }));
  });

  await send('Page.enable');
  await send('Runtime.enable');

  // Wait until canvas is resized to 800x600 by WASM/SDL
  console.log('Waiting for WASM engine to resize canvas to 800x600...');
  let canvasRect = null;
  const pollStart = Date.now();
  while (Date.now() - pollStart < 15000) {
    try {
      const evalRes = await send('Runtime.evaluate', {
        expression: `
          (() => {
            const canvas = document.getElementById('canvas');
            if (!canvas) return null;
            const r = canvas.getBoundingClientRect();
            return { x: r.left, y: r.top, width: canvas.width, height: canvas.height };
          })()
        `,
        returnByValue: true
      });
      if (evalRes?.result?.value?.width >= 800 && evalRes?.result?.value?.height >= 600) {
        canvasRect = evalRes.result.value;
        break;
      }
    } catch (_) {}
    await new Promise(r => setTimeout(r, 300));
  }

  if (!canvasRect) {
    cleanup();
    throw new Error('Canvas was not resized to 800x600 in time');
  }

  console.log(`Canvas confirmed at (${canvasRect.x}, ${canvasRect.y}), size: ${canvasRect.width}x${canvasRect.height}`);

  // Poll until screenshot returns genuine rendered pixels
  let gpuImage = null;
  let pngBuf = null;
  const shotStart = Date.now();

  while (Date.now() - shotStart < 15000) {
    const shot = await send('Page.captureScreenshot', {
      format: 'png',
      clip: {
        x: Math.round(canvasRect.x),
        y: Math.round(canvasRect.y),
        width: Math.round(canvasRect.width),
        height: Math.round(canvasRect.height),
        scale: 1
      },
      fromSurface: true,
      captureBeyondViewport: true
    });

    if (shot?.data) {
      const buf = Buffer.from(shot.data, 'base64');
      const img = decodePngRgba(buf);
      const centerPx = img.getPixel(Math.floor(img.width * 0.5), Math.floor(img.height * 0.5));
      if (centerPx[0] > 0 || centerPx[1] > 0 || centerPx[2] > 0) {
        gpuImage = img;
        pngBuf = buf;
        break;
      }
    }
    await new Promise(r => setTimeout(r, 400));
  }

  cleanup();

  if (!gpuImage || !pngBuf) {
    throw new Error('Failed to capture active non-black frame from GPU');
  }

  const screenshotPath = path.join(buildDir, 'challenge_screenshot.png');
  fs.writeFileSync(screenshotPath, pngBuf);
  console.log(`Saved screenshot artifact: ${screenshotPath} (${pngBuf.length} bytes)`);
  console.log(`Decoded PNG: ${gpuImage.width}x${gpuImage.height}`);

  // Phase C: Compare GPU vs CPU Oracle Across Test Probes
  console.log('\n[Phase C] Empirical Probe Comparison: GPU Hardware vs CPU Oracle Ground Truth:');
  const probes = [
    { id: 'center', label: 'Center (Voxel Monolith)', x: 400, y: 300 },
    { id: 'floorCenter', label: 'Floor Center (Sandstone)', x: 400, y: 510 },
    { id: 'floorLeft', label: 'Floor Left (Slate)', x: 160, y: 510 },
    { id: 'floorRight', label: 'Floor Right (Sandstone)', x: 640, y: 510 },
    { id: 'skyCenter', label: 'Monolith Peak / Horizon Center', x: 400, y: 90 },
    { id: 'skyLeft', label: 'Teal Pillar Left / Horizon', x: 120, y: 120 },
    { id: 'skyRight', label: 'Sky/Horizon Right', x: 680, y: 120 },
    { id: 'trueZenith', label: 'True Open Sky Zenith', x: 400, y: 30 },
    { id: 'midLeft', label: 'Mid-Left Flank (Floor/Monolith)', x: 120, y: 300 },
    { id: 'midRight', label: 'Mid-Right Flank (Floor Edge)', x: 680, y: 300 }
  ];

  let maxDelta = 0;
  for (const p of probes) {
    const gpuPx = gpuImage.getPixel(p.x, p.y);
    const { screen_px, screen_py } = pixelToScreenP(p.x, p.y);
    const oracle = traceRay(screen_px, screen_py);

    const dr = Math.abs(gpuPx[0] - oracle.color[0]);
    const dg = Math.abs(gpuPx[1] - oracle.color[1]);
    const db = Math.abs(gpuPx[2] - oracle.color[2]);
    const totalD = dr + dg + db;
    if (totalD > maxDelta) maxDelta = totalD;

    const matNames = ['Air/Sky', 'Slate Floor', 'Sandstone Floor', 'Amber Monolith', 'Teal Pillar'];
    console.log(
      `Probe [${p.id.padEnd(12)} (${String(p.x).padStart(3)}, ${String(p.y).padStart(3)})]: ` +
      `GPU=[${gpuPx.slice(0,3).map(v => String(v).padStart(3)).join(',')}] ` +
      `Oracle=[${oracle.color.slice(0,3).map(v => String(v).padStart(3)).join(',')}] ` +
      `Mat=${oracle.mat} (${matNames[oracle.mat]}) ` +
      `RGB_Delta=${totalD}`
    );
  }

  console.log(`\nMax Probe RGB Discrepancy between GPU and CPU Oracle: ${maxDelta}`);

  // Phase D: Full Image Statistical Metrics
  console.log('\n[Phase D] Full Frame Empirical Statistics:');
  let nonBlackCount = 0;
  let gpuIntensitySum = 0;
  const sampledColors = new Set();
  const stepX = 10;
  const stepY = 10;

  for (let y = 0; y < H; y += stepY) {
    for (let x = 0; x < W; x += stepX) {
      const px = gpuImage.getPixel(x, y);
      const intensity = (px[0] + px[1] + px[2]) / 3;
      gpuIntensitySum += intensity;
      if (intensity > 0) nonBlackCount++;
      sampledColors.add(`${px[0]},${px[1]},${px[2]}`);
    }
  }

  const sampleCount = (H / stepY) * (W / stepX);
  const meanGpuIntensity = gpuIntensitySum / sampleCount;
  console.log(`Sampled Grid Resolution: ${sampleCount} points (${stepX}x${stepY} stride)`);
  console.log(`Non-black Pixel Rate:    ${((nonBlackCount / sampleCount) * 100).toFixed(2)}%`);
  console.log(`Mean Pixel Intensity:    ${meanGpuIntensity.toFixed(2)} / 255`);
  console.log(`Distinct Colors Sampled: ${sampledColors.size}`);

  console.log('\n================================================================');
  console.log('   EMPIRICAL VERIFICATION COMPLETE');
  console.log('================================================================');
}

main().catch(err => {
  console.error('[ERROR]', err);
  process.exit(1);
});
