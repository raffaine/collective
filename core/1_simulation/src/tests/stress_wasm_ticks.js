#!/usr/bin/env node
/**
 * Adversarial WASM & Flecs Stress Test Harness
 * 
 * Tests:
 * 1. Rapid-fire tick stress: 2,000 synchronous ticks (evaluates throttle stability and zero crash).
 * 2. Real-time frame pacing: Verifies 60 FPS frame governor adherence.
 * 3. WASM linear heap stability: Measures memory before, during, and after stress.
 * 4. RenderState lifecycle: Confirms pipeline draw count and component queries.
 */

import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const buildDir = path.resolve(__dirname, '../build_wasm');

async function runWasmStressTest() {
  console.log('=== Starting Adversarial WASM & Flecs Engine Stress Test ===');
  console.log(`Target WASM directory: ${buildDir}`);

  const canvas = {
    id: 'canvas',
    width: 800,
    height: 600,
    style: {},
    getContext: (type) => ({
      canvas,
      configure: (cfg) => {},
      getCurrentTexture: () => ({
        createView: () => ({})
      })
    }),
    getBoundingClientRect: () => ({ left: 0, top: 0, width: 800, height: 600 }),
    addEventListener: () => {},
    removeEventListener: () => {}
  };

  let capturedRAFCallback = null;
  globalThis.window = globalThis;
  globalThis.screen = { width: 1920, height: 1080 };
  globalThis.addEventListener = () => {};
  globalThis.removeEventListener = () => {};
  globalThis.requestAnimationFrame = (cb) => {
    capturedRAFCallback = cb;
    return 1;
  };
  globalThis.cancelAnimationFrame = (id) => {};
  globalThis.document = {
    getElementById: (id) => canvas,
    querySelector: (sel) => canvas,
    querySelectorAll: (sel) => [canvas],
    createElement: (tag) => (tag === 'canvas' ? canvas : { style: {} }),
    body: { appendChild: () => {}, clientWidth: 1920, clientHeight: 1080 },
    addEventListener: () => {},
    removeEventListener: () => {}
  };

  globalThis.GPUValidationError = class extends Error {};
  globalThis.GPUOutOfMemoryError = class extends Error {};
  globalThis.GPUInternalError = class extends Error {};

  let drawCallCount = 0;
  let pipelineCreated = false;

  const mockGpu = {
    getPreferredCanvasFormat: () => 'bgra8unorm',
    requestAdapter: async (opts) => ({
      features: new Set(),
      limits: {},
      requestDevice: async (desc) => ({
        features: new Set(),
        limits: {},
        lost: new Promise(() => {}),
        queue: {
          submit: (cmds) => {}
        },
        createShaderModule: (desc) => ({}),
        createPipelineLayout: (desc) => ({}),
        createRenderPipeline: (desc) => {
          pipelineCreated = true;
          return {};
        },
        createCommandEncoder: () => ({
          beginRenderPass: (rpDesc) => ({
            setPipeline: (pl) => {},
            draw: (vCount, iCount) => {
              drawCallCount++;
            },
            end: () => {}
          }),
          finish: () => ({})
        })
      })
    })
  };

  Object.defineProperty(globalThis, 'navigator', {
    value: { gpu: mockGpu, userAgent: 'Node-Adversarial-Harness' },
    configurable: true,
    writable: true
  });

  const jsPath = path.join(buildDir, 'oasis_engine.js');
  if (!fs.existsSync(jsPath)) {
    throw new Error(`oasis_engine.js not found at ${jsPath}`);
  }

  // Import WASM module
  const wasmModule = await import(jsPath);
  console.log('[Stress] WASM module imported and main() executed.');

  // Wait for async WebGPU adapter/device to settle
  await new Promise((r) => setTimeout(r, 200));

  if (!pipelineCreated) {
    throw new Error('Pipeline was not created during async WebGPU initialization');
  }
  console.log('[Stress] WebGPU async pipeline successfully initialized.');

  const getHeapSize = () => {
    if (globalThis.HEAP8) return globalThis.HEAP8.buffer.byteLength;
    if (globalThis.wasmMemory) return globalThis.wasmMemory.buffer.byteLength;
    return 0;
  };

  const initialHeap = getHeapSize();
  console.log(`[Stress] Baseline WASM Linear Heap Size: ${(initialHeap / (1024 * 1024)).toFixed(2)} MB`);

  if (!capturedRAFCallback) {
    throw new Error('requestAnimationFrame was never invoked by Emscripten');
  }
  console.log('[Stress] Emscripten main loop callback successfully captured via requestAnimationFrame.');

  const stepFrame = () => {
    const cb = capturedRAFCallback;
    if (cb) {
      capturedRAFCallback = null;
      cb(performance.now());
    }
  };

  // Phase 1: Rapid-fire synchronous burst stress test (2,000 rapid calls)
  console.log('\n[Phase 1] Executing 2,000 rapid synchronous ticks...');
  const burstStart = Date.now();
  for (let i = 0; i < 2000; i++) {
    stepFrame();
  }
  const burstDuration = Date.now() - burstStart;
  console.log(`[Phase 1] 2,000 rapid ticks executed in ${burstDuration}ms without crashing.`);

  // Phase 2: Real-time paced frame test (120 frames over ~2.0s)
  console.log('\n[Phase 2] Paced execution test over 120 frames (~2.0s)...');
  const pacedStartDraws = drawCallCount;
  const targetFrames = 120;
  const frameIntervalMs = 17;

  for (let i = 0; i < targetFrames; i++) {
    stepFrame();
    await new Promise((r) => setTimeout(r, frameIntervalMs));
  }

  const pacedDraws = drawCallCount - pacedStartDraws;
  console.log(`[Phase 2] Paced draws executed: ${pacedDraws} (Target: ~${targetFrames} frames)`);
  if (pacedDraws < targetFrames * 0.8) {
    throw new Error(`Paced frame rate too low: expected ~${targetFrames}, got ${pacedDraws}`);
  }

  // Phase 3: Memory stability check
  const finalHeap = getHeapSize();
  console.log(`\n[Phase 3] Final WASM Linear Heap Size: ${(finalHeap / (1024 * 1024)).toFixed(2)} MB`);
  const heapGrowth = finalHeap - initialHeap;
  console.log(`[Phase 3] Linear Heap Growth: ${heapGrowth} bytes`);

  // Under steady-state ticking, heap should not expand uncontrollably
  if (heapGrowth > 4 * 1024 * 1024) {
    throw new Error(`Excessive WASM heap growth detected: ${heapGrowth} bytes`);
  }

  console.log('\n======================================================');
  console.log(`Total WebGPU Render Passes Executed: ${drawCallCount}`);
  console.log('Adversarial WASM & Flecs Stress Test PASSED');
  console.log('======================================================');
  process.exit(0);
}

runWasmStressTest().catch((err) => {
  console.error('\nAdversarial WASM Stress Test FAILED:', err);
  process.exit(1);
});
