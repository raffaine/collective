# Dead Ends Log

| Iteration | Approach Tried | Why It Failed | Files Touched |
|---|---|---|---|
| Iteration 1 | In-process Node fallback with `mockGpu` facade and `sampleShader(uvX, uvY)` synthetic JS pixel arithmetic in `tests/verify_canvas.js` | Forensic integrity violation: synthetic pixel calculations and mock WebGPU stubs do not exercise genuine WebGPU rasterization and constitute a self-certifying mock facade. | `core/1_simulation/src/tests/verify_canvas.js` |
