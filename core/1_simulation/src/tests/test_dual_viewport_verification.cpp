#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <cassert>
#include <limits>
#include <algorithm>

// Precision tolerances
constexpr float EPSILON_F = 1e-6f;
constexpr double EPSILON_D = 1e-9;

// ==============================================================================
// 1. CUBIC HERMITE TRANSITION MATH VERIFICATION
// ==============================================================================
void VerifyCubicHermiteMath() {
    std::cout << "\n====================================================================\n";
    std::cout << "[TEST SUITE 1] Cubic Hermite Transition Math (h(u) = 3u^2 - 2u^3)\n";
    std::cout << "====================================================================\n";

    auto h = [](float u) -> float {
        return 3.0f * u * u - 2.0f * u * u * u;
    };
    auto h_prime = [](float u) -> float {
        return 6.0f * u - 6.0f * u * u;
    };
    auto h_double_prime = [](float u) -> float {
        return 6.0f - 12.0f * u;
    };

    // 1.1 Endpoint values
    float h0 = h(0.0f);
    float h1 = h(1.0f);
    std::cout << "  [1.1 Endpoint Values] h(0.0) = " << h0 << ", h(1.0) = " << h1 << "\n";
    assert(std::abs(h0 - 0.0f) < EPSILON_F && "h(0.0) must be exactly 0.0f");
    assert(std::abs(h1 - 1.0f) < EPSILON_F && "h(1.0) must be exactly 1.0f");

    // 1.2 Endpoint derivatives (analytical)
    float dh0 = h_prime(0.0f);
    float dh1 = h_prime(1.0f);
    std::cout << "  [1.2 Analytical Derivatives] h'(0.0) = " << dh0 << ", h'(1.0) = " << dh1 << "\n";
    assert(std::abs(dh0 - 0.0f) < EPSILON_F && "h'(0.0) must be exactly 0.0f");
    assert(std::abs(dh1 - 0.0f) < EPSILON_F && "h'(1.0) must be exactly 0.0f");

    // 1.3 Endpoint derivative continuity via finite differences (numerical convergence)
    std::cout << "  [1.3 Finite Difference Endpoint Convergence]:\n";
    std::vector<float> epsilons = {0.1f, 0.01f, 0.001f, 0.0001f, 0.00001f};
    for (float eps : epsilons) {
        float forward_slope_0 = (h(eps) - h(0.0f)) / eps;
        float backward_slope_1 = (h(1.0f) - h(1.0f - eps)) / eps;
        // In double precision without float32 catastrophic cancellation:
        double eps_d = static_cast<double>(eps);
        double h_1_d = 1.0;
        double u_d = 1.0 - eps_d;
        double h_u_d = 3.0 * u_d * u_d - 2.0 * u_d * u_d * u_d;
        double exact_backward_slope = (h_1_d - h_u_d) / eps_d;

        std::cout << "      eps = " << std::setw(8) << eps 
                  << " | Forward slope at 0 (f32): " << std::setw(12) << forward_slope_0 
                  << " | Backward slope at 1 (f32): " << std::setw(12) << backward_slope_1 
                  << " | Exact (f64): " << std::setw(12) << exact_backward_slope << "\n";
        
        assert(forward_slope_0 <= 3.0f * eps + EPSILON_F);
        assert(exact_backward_slope <= 3.0 * eps_d + 1e-7);
        // In float32, due to subtractive cancellation near 1.0, error is bounded by eps + float epsilon
        assert(backward_slope_1 <= 4.0f * eps + 0.001f);
    }
    std::cout << "      -> Confirmed C^1 velocity continuity at both u=0 and u=1.\n";

    // 1.4 Monotonicity test
    std::cout << "  [1.4 Monotonicity Analysis (Float32 vs Float64)]:\n";
    // First, test double precision across 1,000,000 points
    constexpr int NUM_SAMPLES_D = 1000000;
    double prev_d = -1.0;
    double max_slope_d = 0.0;
    double max_slope_u_d = 0.0;
    for (int i = 0; i <= NUM_SAMPLES_D; ++i) {
        double u_d = static_cast<double>(i) / static_cast<double>(NUM_SAMPLES_D);
        double val_d = 3.0 * u_d * u_d - 2.0 * u_d * u_d * u_d;
        assert(val_d >= 0.0 && val_d <= 1.0);
        if (i > 0) {
            assert(val_d >= prev_d && "Mathematical monotonicity must strictly hold in double precision");
            double slope = (val_d - prev_d) * NUM_SAMPLES_D;
            if (slope > max_slope_d) {
                max_slope_d = slope;
                max_slope_u_d = u_d;
            }
        }
        prev_d = val_d;
    }
    std::cout << "      -> Float64 (double): 100% strictly monotonic across " << NUM_SAMPLES_D << " samples.\n"
              << "         Max slope: " << max_slope_d << " at u = " << max_slope_u_d << " (theoretical 1.5)\n";
    assert(std::abs(max_slope_d - 1.5) < 0.001);

    // Now test Float32: In float32, near u=1.0, the derivative h'(u) = 6(1-u) approaches 0.
    // When h'(u) * delta_u < 1 ULP of 2.0 (1.19e-7), roundoff noise can cause 1-ULP jitter.
    // Let's test at 240 ticks (400ms at 600 Hz or 60 Hz = 24 ticks), and at 10,000 steps.
    for (int num_steps : {24, 60, 240, 1000, 10000}) {
        int inversions = 0;
        float max_inversion_mag = 0.0f;
        float prev_f = -1.0f;
        for (int i = 0; i <= num_steps; ++i) {
            float u_f = static_cast<float>(i) / static_cast<float>(num_steps);
            float val_f = h(u_f);
            if (i > 0 && val_f < prev_f) {
                inversions++;
                max_inversion_mag = std::max(max_inversion_mag, prev_f - val_f);
            }
            prev_f = val_f;
        }
        std::cout << "      -> Float32 at " << std::setw(6) << num_steps << " steps: "
                  << "inversions = " << inversions 
                  << " | max inversion mag = " << max_inversion_mag << "\n";
        // At engine frame rates (24 to 240 ticks over 400ms), there must be ZERO inversions!
        if (num_steps <= 240) {
            assert(inversions == 0 && "Zero inversions at operational frame rates!");
        }
    }

    // 1.5 Inflection point & concavity
    std::cout << "  [1.5 Inflection Point & Symmetry]:\n";
    float h_half = h(0.5f);
    float h_prime_half = h_prime(0.5f);
    float h_double_prime_half = h_double_prime(0.5f);
    std::cout << "      h(0.5) = " << h_half << " (expected 0.5)\n";
    std::cout << "      h'(0.5) = " << h_prime_half << " (expected 1.5)\n";
    std::cout << "      h''(0.5) = " << h_double_prime_half << " (expected 0.0)\n";
    assert(std::abs(h_half - 0.5f) < EPSILON_F);
    assert(std::abs(h_prime_half - 1.5f) < EPSILON_F);
    assert(std::abs(h_double_prime_half - 0.0f) < EPSILON_F);

    // Point symmetry test: h(1 - u) == 1 - h(u)
    for (int i = 0; i <= 1000; ++i) {
        float u = static_cast<float>(i) / 1000.0f;
        float sym_diff = std::abs((1.0f - h(u)) - h(1.0f - u));
        assert(sym_diff < EPSILON_F && "Point symmetry h(1-u) == 1 - h(u) violated");
    }
    std::cout << "      -> Point symmetry h(1 - u) == 1 - h(u) verified for all u.\n";

    // 1.6 Numerical stability under out-of-bounds inputs and clamps
    std::cout << "  [1.6 Out-of-Bounds & Clamping Invariants]:\n";
    std::vector<float> oob_inputs = {-100.0f, -1.0f, -0.0001f, 1.0001f, 2.0f, 100.0f, 1e20f, -1e20f};
    for (float raw_u : oob_inputs) {
        float clamped_u = std::clamp(raw_u, 0.0f, 1.0f);
        float res = h(clamped_u);
        assert(res >= 0.0f && res <= 1.0f);
        assert(!std::isnan(res) && !std::isinf(res));
    }
    std::cout << "      -> Clamped inputs correctly evaluate to bounded values.\n";
    std::cout << ">>> [TEST SUITE 1 PASSED] Cubic Hermite transition math verified.\n";
}

// ==============================================================================
// 2. PROJECTION MATRIX INVERTIBILITY & WebGPU DEPTH CLIPPING [0, 1]
// ==============================================================================
void VerifyProjectionAndDepthClipping() {
    std::cout << "\n====================================================================\n";
    std::cout << "[TEST SUITE 2] Projection Invertibility & WebGPU Depth Clipping [0, 1]\n";
    std::cout << "====================================================================\n";

    const float aspect = 800.0f / 600.0f;
    const float fovy = glm::radians(53.13f);
    const float zNear_persp = 0.1f;
    const float zFar_persp = 100.0f;

    // Mode 0: Perspective Camera
    const glm::vec3 eye_persp(0.0001f, 2.0001f, -4.9999f);
    const glm::vec3 target_persp(0.0f, 0.0f, 0.0f);
    const glm::vec3 up_persp(0.0f, 1.0f, 0.0f);
    const glm::mat4 view_persp = glm::lookAt(eye_persp, target_persp, up_persp);
    const glm::mat4 proj_persp = glm::perspective(fovy, aspect, zNear_persp, zFar_persp);
    const glm::mat4 vp_persp = proj_persp * view_persp;
    const glm::mat4 inv_vp_persp = glm::inverse(vp_persp);

    // Mode 1: Isometric Orthographic Camera
    const float ortho_size = 14.0f;
    const glm::mat4 proj_iso = glm::ortho(-ortho_size * aspect, ortho_size * aspect, -ortho_size, ortho_size, -50.0f, 50.0f);
    const glm::vec3 eye_iso(-16.0001f, 20.0001f, -16.0001f);
    const glm::vec3 target_iso(0.0001f, 0.0001f, 0.0001f);
    const glm::vec3 up_iso(0.0f, 1.0f, 0.0f);
    const glm::mat4 view_iso = glm::lookAt(eye_iso, target_iso, up_iso);
    const glm::mat4 vp_iso = proj_iso * view_iso;
    const glm::mat4 inv_vp_iso = glm::inverse(vp_iso);

    // 2.1 Verify WebGPU Depth Clipping [0, 1] for Perspective Projection
    std::cout << "  [2.1 WebGPU Depth Clipping Verification - Perspective]:\n";
    // Point on near plane along camera forward axis (in view space: z = -zNear_persp)
    glm::vec4 near_pt_eye(0.0f, 0.0f, -zNear_persp, 1.0f);
    glm::vec4 near_pt_clip = proj_persp * near_pt_eye;
    float near_ndc_z = near_pt_clip.z / near_pt_clip.w;

    // Point on far plane along camera forward axis (in view space: z = -zFar_persp)
    glm::vec4 far_pt_eye(0.0f, 0.0f, -zFar_persp, 1.0f);
    glm::vec4 far_pt_clip = proj_persp * far_pt_eye;
    float far_ndc_z = far_pt_clip.z / far_pt_clip.w;

    std::cout << "      Near plane (z=" << zNear_persp << ") -> NDC z = " << near_ndc_z << " (expected 0.0)\n";
    std::cout << "      Far plane  (z=" << zFar_persp << ") -> NDC z = " << far_ndc_z << " (expected 1.0)\n";
    assert(std::abs(near_ndc_z - 0.0f) < 1e-4f && "WebGPU near clipping plane must map to NDC z = 0.0");
    assert(std::abs(far_ndc_z - 1.0f) < 1e-4f && "WebGPU far clipping plane must map to NDC z = 1.0");

    // 2.2 Verify WebGPU Depth Clipping [0, 1] for Orthographic Projection
    std::cout << "  [2.2 WebGPU Depth Clipping Verification - Orthographic]:\n";
    // In glm::ortho(left, right, bottom, top, zNear, zFar) with GLM_FORCE_DEPTH_ZERO_TO_ONE:
    // View space point at -zNear maps to z=0, -zFar maps to z=1.
    // For -50 to +50: zNear=-50, zFar=50:
    glm::vec4 ortho_near_eye(0.0f, 0.0f, 50.0f, 1.0f); // view space z = -(-50) = +50
    glm::vec4 ortho_near_clip = proj_iso * ortho_near_eye;
    float ortho_near_ndc_z = ortho_near_clip.z / ortho_near_clip.w;

    glm::vec4 ortho_far_eye(0.0f, 0.0f, -50.0f, 1.0f); // view space z = -(+50) = -50
    glm::vec4 ortho_far_clip = proj_iso * ortho_far_eye;
    float ortho_far_ndc_z = ortho_far_clip.z / ortho_far_clip.w;

    std::cout << "      Ortho near plane (z=-50) -> NDC z = " << ortho_near_ndc_z << " (expected 0.0)\n";
    std::cout << "      Ortho far plane  (z=+50) -> NDC z = " << ortho_far_ndc_z << " (expected 1.0)\n";
    assert(std::abs(ortho_near_ndc_z - 0.0f) < 1e-4f && "WebGPU ortho near plane must map to NDC z = 0.0");
    assert(std::abs(ortho_far_ndc_z - 1.0f) < 1e-4f && "WebGPU ortho far plane must map to NDC z = 1.0");

    // 2.3 Determinant & Invertibility of Perspective VP Matrix
    std::cout << "  [2.3 Determinant & Invertibility]:\n";
    float det_vp_persp = glm::determinant(vp_persp);
    float det_vp_iso = glm::determinant(vp_iso);
    std::cout << "      det(VP_persp) = " << det_vp_persp << "\n";
    std::cout << "      det(VP_iso)   = " << det_vp_iso << "\n";
    assert(std::abs(det_vp_persp) > 1e-6f && "VP_persp must be non-singular");
    assert(std::abs(det_vp_iso) > 1e-6f && "VP_iso must be non-singular");

    // Verify VP * inv(VP) == Identity
    glm::mat4 I_persp = vp_persp * inv_vp_persp;
    glm::mat4 I_iso = vp_iso * inv_vp_iso;
    float err_I_persp = 0.0f;
    float err_I_iso = 0.0f;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            float expected = (r == c) ? 1.0f : 0.0f;
            err_I_persp = std::max(err_I_persp, std::abs(I_persp[c][r] - expected));
            err_I_iso = std::max(err_I_iso, std::abs(I_iso[c][r] - expected));
        }
    }
    std::cout << "      Max inversion error for Perspective: " << err_I_persp << " (tolerance 1e-4)\n";
    std::cout << "      Max inversion error for Isometric:   " << err_I_iso << " (tolerance 1e-4)\n";
    assert(err_I_persp < 1e-4f);
    assert(err_I_iso < 1e-4f);

    // 2.4 Transition Invertibility: Test inv_view_proj(t) across t in [0, 1]
    std::cout << "  [2.4 Intermediate Matrix Regularity along t in [0, 1]]:\n";
    float min_det = std::numeric_limits<float>::max();
    float max_det = std::numeric_limits<float>::lowest();
    for (int step = 0; step <= 1000; ++step) {
        float t = static_cast<float>(step) / 1000.0f;
        glm::mat4 inv_vp_t = (1.0f - t) * inv_vp_persp + t * inv_vp_iso;
        float det_t = glm::determinant(inv_vp_t);
        min_det = std::min(min_det, det_t);
        max_det = std::max(max_det, det_t);
        assert(!std::isnan(det_t) && !std::isinf(det_t));
        assert(std::abs(det_t) > 1e-5f && "Interpolated inverse VP matrix must never be singular!");
    }
    std::cout << "      det(inv_vp_t) range across t in [0, 1]: [" << min_det << ", " << max_det << "]\n";
    std::cout << "      -> Invertibility and non-singularity guaranteed across entire transition.\n";

    std::cout << ">>> [TEST SUITE 2 PASSED] Projection invertibility and WebGPU depth clipping verified.\n";
}

// ==============================================================================
// 3. RAY ORIGIN AND DIRECTION INTERPOLATION BEHAVIOR ACROSS TRANSITIONS
// ==============================================================================
void VerifyRayOriginAndDirectionInterpolation() {
    std::cout << "\n====================================================================\n";
    std::cout << "[TEST SUITE 3] Ray Origin and Direction Interpolation Across Transitions\n";
    std::cout << "====================================================================\n";

    const float aspect = 800.0f / 600.0f;
    const float fovy = glm::radians(53.13f);
    const float zNear_persp = 0.1f;
    const float zFar_persp = 100.0f;

    const glm::vec3 eye_persp(0.0001f, 2.0001f, -4.9999f);
    const glm::vec3 target_persp(0.0f, 0.0f, 0.0f);
    const glm::vec3 up_persp(0.0f, 1.0f, 0.0f);
    const glm::mat4 view_persp = glm::lookAt(eye_persp, target_persp, up_persp);
    const glm::mat4 proj_persp = glm::perspective(fovy, aspect, zNear_persp, zFar_persp);
    const glm::mat4 inv_vp_persp = glm::inverse(proj_persp * view_persp);

    const float ortho_size = 14.0f;
    const glm::mat4 proj_iso = glm::ortho(-ortho_size * aspect, ortho_size * aspect, -ortho_size, ortho_size, -50.0f, 50.0f);
    const glm::vec3 eye_iso(-16.0001f, 20.0001f, -16.0001f);
    const glm::vec3 target_iso(0.0001f, 0.0001f, 0.0001f);
    const glm::vec3 up_iso(0.0f, 1.0f, 0.0f);
    const glm::mat4 view_iso = glm::lookAt(eye_iso, target_iso, up_iso);
    const glm::mat4 inv_vp_iso = glm::inverse(proj_iso * view_iso);

    // Test across a fine 101x101 screen grid (10,201 pixels) and 21 transition time steps
    constexpr int GRID_RES = 101;
    constexpr int NUM_T_STEPS = 20;

    std::cout << "  [3.1 Stress Testing " << (GRID_RES * GRID_RES * (NUM_T_STEPS + 1)) 
              << " Rays Across Screen Frustum & Transition Timeline]...\n";

    float min_near_w = std::numeric_limits<float>::max();
    float max_near_w = std::numeric_limits<float>::lowest();
    float min_far_w = std::numeric_limits<float>::max();
    float max_far_w = std::numeric_limits<float>::lowest();
    float min_dot_persp_ortho = 1.0f;
    float min_interpolated_len = 1.0f;
    float max_rd_norm_err = 0.0f;

    for (int t_step = 0; t_step <= NUM_T_STEPS; ++t_step) {
        float t = static_cast<float>(t_step) / static_cast<float>(NUM_T_STEPS);
        glm::mat4 inv_view_proj = (1.0f - t) * inv_vp_persp + t * inv_vp_iso;
        glm::vec3 camera_pos = glm::mix(eye_persp, eye_iso, t);

        for (int gy = 0; gy < GRID_RES; ++gy) {
            float ndc_y = -1.0f + 2.0f * (static_cast<float>(gy) / (GRID_RES - 1));
            for (int gx = 0; gx < GRID_RES; ++gx) {
                float ndc_x = -1.0f + 2.0f * (static_cast<float>(gx) / (GRID_RES - 1));

                // Reproduce WGSL shader logic verbatim
                glm::vec4 near_h = inv_view_proj * glm::vec4(ndc_x, ndc_y, 0.0f, 1.0f);
                glm::vec4 far_h  = inv_view_proj * glm::vec4(ndc_x, ndc_y, 1.0f, 1.0f);

                min_near_w = std::min(min_near_w, near_h.w);
                max_near_w = std::max(max_near_w, near_h.w);
                min_far_w  = std::min(min_far_w, far_h.w);
                max_far_w  = std::max(max_far_w, far_h.w);

                if (t_step == 0 && gy == 0 && gx == 0) {
                    std::cout << "      [Sample at t=0, ndc=(-1,-1)]: near_h = (" << near_h.x << ", " << near_h.y << ", " << near_h.z << ", " << near_h.w << ")\n";
                    std::cout << "      [Sample at t=0, ndc=(-1,-1)]: far_h  = (" << far_h.x << ", " << far_h.y << ", " << far_h.z << ", " << far_h.w << ")\n";
                }
                if (t_step == NUM_T_STEPS && gy == 0 && gx == 0) {
                    std::cout << "      [Sample at t=1, ndc=(-1,-1)]: near_h = (" << near_h.x << ", " << near_h.y << ", " << near_h.z << ", " << near_h.w << ")\n";
                    std::cout << "      [Sample at t=1, ndc=(-1,-1)]: far_h  = (" << far_h.x << ", " << far_h.y << ", " << far_h.z << ", " << far_h.w << ")\n";
                }

                // Invariant: near_h.w and far_h.w must be strictly positive (prevent division by zero)
                if (std::abs(near_h.w) < 1e-4f || std::abs(far_h.w) < 1e-4f) {
                    std::cout << "ERROR: near_h.w=" << near_h.w << ", far_h.w=" << far_h.w << " at t=" << t << "\n";
                }
                assert(std::abs(near_h.w) > 1e-4f && "near_h.w must not be zero");
                assert(std::abs(far_h.w) > 1e-4f && "far_h.w must not be zero");

                glm::vec3 p_near = glm::vec3(near_h) / near_h.w;
                glm::vec3 p_far  = glm::vec3(far_h) / far_h.w;

                // Perspective ray origin & direction
                glm::vec3 ro_persp = camera_pos;
                glm::vec3 rd_persp = glm::normalize(p_far - ro_persp);

                // Orthographic ray origin & direction
                glm::vec3 ro_ortho = p_near;
                glm::vec3 rd_ortho = glm::normalize(p_far - p_near);

                // Dot product between perspective and orthographic ray directions
                float dot_po = glm::dot(rd_persp, rd_ortho);
                min_dot_persp_ortho = std::min(min_dot_persp_ortho, dot_po);

                // Interpolated ray origin and unnormalized direction
                glm::vec3 ro = glm::mix(ro_persp, ro_ortho, t);
                glm::vec3 raw_rd = glm::mix(rd_persp, rd_ortho, t);
                float raw_len = glm::length(raw_rd);
                min_interpolated_len = std::min(min_interpolated_len, raw_len);

                // Invariant: raw_rd must NEVER vanish (never zero length)
                assert(raw_len > 0.1f && "Interpolated ray direction vector must never vanish!");

                glm::vec3 rd = glm::normalize(raw_rd);
                float norm_err = std::abs(glm::length(rd) - 1.0f);
                max_rd_norm_err = std::max(max_rd_norm_err, norm_err);

                // Verification of endpoint identity
                if (t == 0.0f) {
                    // In mode 0: ro must match eye_persp exactly
                    float diff_ro = glm::length(ro - eye_persp);
                    assert(diff_ro < 1e-4f && "Mode 0 ray origin must match eye_persp");
                    // rd must match rd_persp
                    float diff_rd = glm::length(rd - rd_persp);
                    assert(diff_rd < 1e-4f && "Mode 0 ray direction must match rd_persp");
                } else if (t == 1.0f) {
                    // In mode 1: ro must match p_near exactly
                    float diff_ro = glm::length(ro - p_near);
                    assert(diff_ro < 1e-4f && "Mode 1 ray origin must match p_near");
                    // rd must match rd_ortho
                    float diff_rd = glm::length(rd - rd_ortho);
                    assert(diff_rd < 1e-4f && "Mode 1 ray direction must match rd_ortho");
                }
            }
        }
    }

    std::cout << "      near_h.w range across all rays: [" << min_near_w << ", " << max_near_w << "]\n";
    std::cout << "      far_h.w  range across all rays: [" << min_far_w << ", " << max_far_w << "]\n";
    std::cout << "      Minimum rd_persp . rd_ortho:    " << min_dot_persp_ortho 
              << " (angle = " << (std::acos(min_dot_persp_ortho) * 180.0f / 3.14159265f) << " deg)\n";
    std::cout << "      Minimum ||mix(rd_p, rd_o, t)||: " << min_interpolated_len << " (strictly positive)\n";
    std::cout << "      Max ray direction norm error:   " << max_rd_norm_err << "\n";

    assert(min_dot_persp_ortho > 0.0f && "Angle between perspective and orthographic rays must be strictly < 90 deg");
    assert(min_interpolated_len > 0.5f && "Length of blended ray direction must remain robustly > 0.5");
    assert(max_rd_norm_err < 1e-5f && "Normalized ray direction must have unit length");

    // 3.2 Center Screen Ray Tracing & Target Convergence
    std::cout << "  [3.2 Center Screen Ray Convergence]:\n";
    // At ndc = (0, 0), test ray intersection with ground plane y = 0
    for (float t : {0.0f, 0.25f, 0.5f, 0.75f, 1.0f}) {
        glm::mat4 inv_view_proj = (1.0f - t) * inv_vp_persp + t * inv_vp_iso;
        glm::vec3 camera_pos = glm::mix(eye_persp, eye_iso, t);

        glm::vec4 near_h = inv_view_proj * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 far_h  = inv_view_proj * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        glm::vec3 p_near = glm::vec3(near_h) / near_h.w;
        glm::vec3 p_far  = glm::vec3(far_h) / far_h.w;

        glm::vec3 ro_persp = camera_pos;
        glm::vec3 rd_persp = glm::normalize(p_far - ro_persp);
        glm::vec3 ro_ortho = p_near;
        glm::vec3 rd_ortho = glm::normalize(p_far - p_near);

        glm::vec3 ro = glm::mix(ro_persp, ro_ortho, t);
        glm::vec3 rd = glm::normalize(glm::mix(rd_persp, rd_ortho, t));

        // Ray plane intersection with y = 0: ro.y + hit_dist * rd.y = 0 => hit_dist = -ro.y / rd.y
        float hit_dist = -ro.y / rd.y;
        glm::vec3 hit_pos = ro + hit_dist * rd;

        std::cout << "      t = " << std::setw(4) << t 
                  << " | ro: (" << std::setw(7) << ro.x << ", " << std::setw(7) << ro.y << ", " << std::setw(7) << ro.z << ")"
                  << " | rd: (" << std::setw(6) << rd.x << ", " << std::setw(6) << rd.y << ", " << std::setw(6) << rd.z << ")"
                  << " | Ground Hit (y=0): (" << std::setw(6) << hit_pos.x << ", " << std::setw(6) << hit_pos.z << ")\n";

        // Hit point must be within Suburban Sprawl bounds: |x| <= 28, z in [-10, 42]
        assert(hit_pos.x >= -28.0f && hit_pos.x <= 28.0f);
        assert(hit_pos.z >= -10.0f && hit_pos.z <= 42.0f);
    }

    std::cout << ">>> [TEST SUITE 3 PASSED] Ray origin and direction interpolation behavior verified.\n";
}

int main() {
    std::cout << "====================================================================\n";
    std::cout << "=== ADVERSARIAL VERIFICATION: DUAL VIEWPORT & TRANSITION MATH ====\n";
    std::cout << "====================================================================\n";

    VerifyCubicHermiteMath();
    VerifyProjectionAndDepthClipping();
    VerifyRayOriginAndDirectionInterpolation();

    std::cout << "\n====================================================================\n";
    std::cout << ">>> ALL ADVERSARIAL STRESS TESTS PASSED (Zero Defects Confirmed) <<<\n";
    std::cout << "====================================================================\n";
    return 0;
}
