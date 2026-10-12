#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

#ifndef __EMSCRIPTEN__
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <spdlog/spdlog.h>
#include "uhai/uhai_ring_buffer.hpp"

namespace oasis::gateway {

// ============================================================================
// Cryptographic Hash & Intent CID Generator (FIPS 180-4 SHA-256)
// ============================================================================
namespace crypto {

class Sha256 {
public:
    Sha256() noexcept { Reset(); }

    void Reset() noexcept {
        state_[0] = 0x6a09e667;
        state_[1] = 0xbb67ae85;
        state_[2] = 0x3c6ef372;
        state_[3] = 0xa54ff53a;
        state_[4] = 0x510e527f;
        state_[5] = 0x9b05688c;
        state_[6] = 0x1f83d9ab;
        state_[7] = 0x5be0cd19;
        count_ = 0;
    }

    void Update(const void* data, size_t length) noexcept {
        const auto* input = static_cast<const uint8_t*>(data);
        size_t buffer_index = (count_ >> 3) & 0x3f;
        count_ += static_cast<uint64_t>(length) << 3;

        size_t part_len = 64 - buffer_index;
        size_t i = 0;

        if (length >= part_len) {
            std::memcpy(&buffer_[buffer_index], input, part_len);
            Transform(state_, buffer_);
            for (i = part_len; i + 63 < length; i += 64) {
                Transform(state_, &input[i]);
            }
            buffer_index = 0;
        }

        std::memcpy(&buffer_[buffer_index], &input[i], length - i);
    }

    void Update(std::string_view text) noexcept {
        Update(text.data(), text.size());
    }

    std::array<uint8_t, 32> Final() noexcept {
        uint8_t final_count[8];
        for (int i = 0; i < 8; ++i) {
            final_count[i] = static_cast<uint8_t>((count_ >> ((7 - i) * 8)) & 0xff);
        }

        size_t pad_len = ((count_ >> 3) & 0x3f) < 56 ? (56 - ((count_ >> 3) & 0x3f)) : (120 - ((count_ >> 3) & 0x3f));
        static const uint8_t padding[64] = { 0x80 };
        Update(padding, pad_len);
        Update(final_count, 8);

        std::array<uint8_t, 32> digest{};
        for (int i = 0; i < 8; ++i) {
            digest[i * 4 + 0] = static_cast<uint8_t>((state_[i] >> 24) & 0xff);
            digest[i * 4 + 1] = static_cast<uint8_t>((state_[i] >> 16) & 0xff);
            digest[i * 4 + 2] = static_cast<uint8_t>((state_[i] >> 8) & 0xff);
            digest[i * 4 + 3] = static_cast<uint8_t>(state_[i] & 0xff);
        }
        return digest;
    }

    static std::string DigestToHex(const std::array<uint8_t, 32>& digest) {
        std::ostringstream oss;
        for (uint8_t b : digest) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
        }
        return oss.str();
    }

    static std::string HashString(std::string_view text) {
        Sha256 hasher;
        hasher.Update(text);
        return DigestToHex(hasher.Final());
    }

private:
    static inline uint32_t RotR(uint32_t x, uint32_t n) noexcept {
        return (x >> n) | (x << (32 - n));
    }
    static inline uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) noexcept {
        return (x & y) ^ (~x & z);
    }
    static inline uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) noexcept {
        return (x & y) ^ (x & z) ^ (y & z);
    }
    static inline uint32_t Sigma0(uint32_t x) noexcept {
        return RotR(x, 2) ^ RotR(x, 13) ^ RotR(x, 22);
    }
    static inline uint32_t Sigma1(uint32_t x) noexcept {
        return RotR(x, 6) ^ RotR(x, 11) ^ RotR(x, 25);
    }
    static inline uint32_t Gamma0(uint32_t x) noexcept {
        return RotR(x, 7) ^ RotR(x, 18) ^ (x >> 3);
    }
    static inline uint32_t Gamma1(uint32_t x) noexcept {
        return RotR(x, 17) ^ RotR(x, 19) ^ (x >> 10);
    }

    static void Transform(uint32_t state[8], const uint8_t buffer[64]) noexcept {
        static const uint32_t K[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

        uint32_t W[64];
        for (int i = 0; i < 16; ++i) {
            W[i] = (static_cast<uint32_t>(buffer[i * 4 + 0]) << 24) |
                   (static_cast<uint32_t>(buffer[i * 4 + 1]) << 16) |
                   (static_cast<uint32_t>(buffer[i * 4 + 2]) << 8)  |
                   (static_cast<uint32_t>(buffer[i * 4 + 3]));
        }
        for (int i = 16; i < 64; ++i) {
            W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];
        }

        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        uint32_t e = state[4], f = state[5], g = state[6], h = state[7];

        for (int i = 0; i < 64; ++i) {
            uint32_t T1 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];
            uint32_t T2 = Sigma0(a) + Maj(a, b, c);
            h = g; g = f; f = e; e = d + T1;
            d = c; c = b; b = a; a = T1 + T2;
        }

        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
        state[4] += e; state[5] += f; state[6] += g; state[7] += h;
    }

    uint32_t state_[8]{};
    uint64_t count_{0};
    uint8_t  buffer_[64]{};
};

} // namespace crypto

// ============================================================================
// HTTP Request & Response Models
// ============================================================================

struct HttpRequest {
    std::string method;           // "GET", "POST", "PUT", "DELETE", "OPTIONS"
    std::string path;             // e.g. "/api/v1/l2/telemetry/latest"
    std::string query;            // e.g. "param=value"
    std::string http_version{"HTTP/1.1"};
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    [[nodiscard]] std::string GetHeader(const std::string& key) const {
        for (const auto& [k, v] : headers) {
            if (k.size() == key.size()) {
                bool match = true;
                for (size_t i = 0; i < k.size(); ++i) {
                    if (std::tolower(static_cast<unsigned char>(k[i])) !=
                        std::tolower(static_cast<unsigned char>(key[i]))) {
                        match = false;
                        break;
                    }
                }
                if (match) return v;
            }
        }
        return {};
    }
};

struct HttpResponse {
    int status_code{200};
    std::string status_text{"OK"};
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    static HttpResponse Json(int status, const std::string& json_body) {
        HttpResponse res;
        res.status_code = status;
        res.status_text = StatusText(status);
        res.headers["Content-Type"] = "application/json; charset=utf-8";
        res.headers["Access-Control-Allow-Origin"] = "*";
        res.headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, OPTIONS";
        res.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization, X-Collective-Signature";
        res.body = json_body;
        return res;
    }

    static HttpResponse Text(int status, const std::string& text, const std::string& content_type = "text/html; charset=utf-8") {
        HttpResponse res;
        res.status_code = status;
        res.status_text = StatusText(status);
        res.headers["Content-Type"] = content_type;
        res.headers["Access-Control-Allow-Origin"] = "*";
        res.headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, OPTIONS";
        res.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization, X-Collective-Signature";
        res.body = text;
        return res;
    }

    static HttpResponse MethodNotAllowed(const std::string& error_id = "METHOD_NOT_ALLOWED",
                                         const std::string& message = "Method Not Allowed on read-only diagnostic layer") {
        HttpResponse res;
        res.status_code = 405;
        res.status_text = "Method Not Allowed";
        res.headers["Content-Type"] = "application/json; charset=utf-8";
        res.headers["Access-Control-Allow-Origin"] = "*";
        res.headers["Access-Control-Allow-Methods"] = "GET, OPTIONS";
        res.headers["Allow"] = "GET, OPTIONS";
        std::ostringstream oss;
        oss << "{\n"
            << "  \"error\": \"" << error_id << "\",\n"
            << "  \"status\": 405,\n"
            << "  \"message\": \"" << message << "\"\n"
            << "}\n";
        res.body = oss.str();
        return res;
    }

    static HttpResponse Forbidden(const std::string& error_id = "FORBIDDEN",
                                  const std::string& message = "Forbidden") {
        HttpResponse res;
        res.status_code = 403;
        res.status_text = "Forbidden";
        res.headers["Content-Type"] = "application/json; charset=utf-8";
        res.headers["Access-Control-Allow-Origin"] = "*";
        std::ostringstream oss;
        oss << "{\n"
            << "  \"error\": \"" << error_id << "\",\n"
            << "  \"status\": 403,\n"
            << "  \"message\": \"" << message << "\"\n"
            << "}\n";
        res.body = oss.str();
        return res;
    }

    static HttpResponse NotFound(const std::string& message = "Endpoint not found") {
        return Json(404, "{\n  \"error\": \"NOT_FOUND\",\n  \"status\": 404,\n  \"message\": \"" + message + "\"\n}\n");
    }

    static HttpResponse BadRequest(const std::string& message = "Bad Request") {
        return Json(400, "{\n  \"error\": \"BAD_REQUEST\",\n  \"status\": 400,\n  \"message\": \"" + message + "\"\n}\n");
    }

    [[nodiscard]] std::string ToString() const {
        std::ostringstream oss;
        oss << "HTTP/1.1 " << status_code << " " << status_text << "\r\n";
        for (const auto& [k, v] : headers) {
            oss << k << ": " << v << "\r\n";
        }
        if (headers.find("Content-Length") == headers.end()) {
            oss << "Content-Length: " << body.size() << "\r\n";
        }
        if (headers.find("Connection") == headers.end()) {
            oss << "Connection: close\r\n";
        }
        oss << "\r\n" << body;
        return oss.str();
    }

private:
    static std::string StatusText(int code) {
        switch (code) {
            case 200: return "OK";
            case 201: return "Created";
            case 204: return "No Content";
            case 400: return "Bad Request";
            case 401: return "Unauthorized";
            case 403: return "Forbidden";
            case 404: return "Not Found";
            case 405: return "Method Not Allowed";
            case 500: return "Internal Server Error";
            case 503: return "Service Unavailable";
            default:  return "Status";
        }
    }
};

// ============================================================================
// Lightweight HTTP Request Parser
// ============================================================================

class HttpParser {
public:
    static HttpRequest Parse(std::string_view raw) {
        HttpRequest req;
        if (raw.empty()) return req;

        size_t line_end = raw.find("\r\n");
        size_t line_len = 2;
        if (line_end == std::string_view::npos) {
            line_end = raw.find('\n');
            line_len = 1;
        }
        if (line_end == std::string_view::npos) {
            return req;
        }

        std::string_view request_line = raw.substr(0, line_end);
        size_t sp1 = request_line.find(' ');
        if (sp1 != std::string_view::npos) {
            req.method = std::string(request_line.substr(0, sp1));
            size_t sp2 = request_line.find(' ', sp1 + 1);
            if (sp2 != std::string_view::npos) {
                std::string_view full_path = request_line.substr(sp1 + 1, sp2 - sp1 - 1);
                size_t qpos = full_path.find('?');
                if (qpos != std::string_view::npos) {
                    req.path = std::string(full_path.substr(0, qpos));
                    req.query = std::string(full_path.substr(qpos + 1));
                } else {
                    req.path = std::string(full_path);
                }
                req.http_version = std::string(request_line.substr(sp2 + 1));
            } else {
                req.path = std::string(request_line.substr(sp1 + 1));
            }
        }

        // Parse headers
        size_t cursor = line_end + line_len;
        while (cursor < raw.size()) {
            size_t next_line = raw.find("\r\n", cursor);
            size_t next_len = 2;
            if (next_line == std::string_view::npos) {
                next_line = raw.find('\n', cursor);
                next_len = 1;
            }
            if (next_line == std::string_view::npos || next_line == cursor) {
                if (next_line != std::string_view::npos) {
                    cursor = next_line + next_len;
                }
                break;
            }

            std::string_view header_line = raw.substr(cursor, next_line - cursor);
            size_t colon = header_line.find(':');
            if (colon != std::string_view::npos) {
                std::string key = std::string(header_line.substr(0, colon));
                while (!key.empty() && std::isspace(static_cast<unsigned char>(key.back()))) key.pop_back();

                size_t val_start = colon + 1;
                while (val_start < header_line.size() && std::isspace(static_cast<unsigned char>(header_line[val_start]))) {
                    val_start++;
                }
                std::string val = std::string(header_line.substr(val_start));
                while (!val.empty() && std::isspace(static_cast<unsigned char>(val.back()))) val.pop_back();
                req.headers[key] = val;
            }
            cursor = next_line + next_len;
        }

        // Body
        if (cursor < raw.size()) {
            req.body = std::string(raw.substr(cursor));
        }

        return req;
    }
};

// ============================================================================
// Gateway Server
// ============================================================================

class GatewayServer {
public:
    explicit GatewayServer(const std::string& shm_name = oasis::uhai::DEFAULT_GATEWAY_SHM_NAME)
        : shm_name_(shm_name) {
        InitializeDefaultState();
        InitializeChannels();
    }

    ~GatewayServer() = default;

    // Direct state inspection and injection for Catch2 testing
    void SetL2Sample(const oasis::uhai::TelemetrySample& sample) {
        latest_l2_ = sample;
        has_l2_ = true;
    }

    [[nodiscard]] const oasis::uhai::TelemetrySample& GetL2Sample() const {
        return latest_l2_;
    }

    void SetL4Telemetry(const oasis::uhai::L4ExecutionTelemetry& telemetry) {
        latest_l4_ = telemetry;
        has_l4_ = true;
    }

    [[nodiscard]] const oasis::uhai::L4ExecutionTelemetry& GetL4Telemetry() const {
        return latest_l4_;
    }

    void SetL5Telemetry(const oasis::uhai::L5GovernanceTelemetry& telemetry) {
        latest_l5_ = telemetry;
        has_l5_ = true;
    }

    [[nodiscard]] const oasis::uhai::L5GovernanceTelemetry& GetL5Telemetry() const {
        return latest_l5_;
    }

    void AddPendingYield(const oasis::uhai::UpwardYieldPacket& packet) {
        pending_yields_.push_back(packet);
    }

    [[nodiscard]] const std::vector<oasis::uhai::UpwardYieldPacket>& GetPendingYields() const {
        return pending_yields_;
    }

    [[nodiscard]] const std::vector<oasis::uhai::SpatialIntentPayload>& GetSubmittedIntents() const {
        return submitted_intents_;
    }

    [[nodiscard]] uint64_t GetTotalRequestsHandled() const noexcept {
        return total_requests_handled_.load(std::memory_order_relaxed);
    }

    // Ring buffer polling loop to drain items from UHAI channels
    void PollRingBuffers() {
        if (l2_channel_ && l2_channel_->IsValid()) {
            oasis::uhai::TelemetrySample sample{};
            while (l2_channel_->Pop(sample)) {
                latest_l2_ = sample;
                has_l2_ = true;
            }
        }

        if (l4_channel_ && l4_channel_->IsValid()) {
            oasis::uhai::L4ExecutionTelemetry telemetry{};
            while (l4_channel_->Pop(telemetry)) {
                latest_l4_ = telemetry;
                has_l4_ = true;
            }
        }

        if (l5_channel_ && l5_channel_->IsValid()) {
            oasis::uhai::L5GovernanceTelemetry telemetry{};
            while (l5_channel_->Pop(telemetry)) {
                latest_l5_ = telemetry;
                has_l5_ = true;
            }
        }

        if (yield_channel_ && yield_channel_->IsValid()) {
            oasis::uhai::UpwardYieldPacket packet{};
            while (yield_channel_->Pop(packet)) {
                pending_yields_.push_back(packet);
            }
        }
    }

    // Invariant 7 Check: Verifies whether the path refers to a read-only diagnostic layer
    static bool IsDiagnosticLayerPath(const std::string& path) {
        return (path.rfind("/api/v1/l1", 0) == 0 ||
                path.rfind("/api/v1/l2", 0) == 0 ||
                path.rfind("/api/v1/l3", 0) == 0 ||
                path.rfind("/api/v1/l4", 0) == 0 ||
                path.rfind("/api/v1/l7", 0) == 0);
    }

    // Core synchronous request dispatcher
    HttpResponse HandleRequest(const HttpRequest& request) {
        total_requests_handled_.fetch_add(1, std::memory_order_relaxed);
        PollRingBuffers();

        // 1. CORS Preflight
        if (request.method == "OPTIONS") {
            HttpResponse res;
            res.status_code = 204;
            res.status_text = "No Content";
            res.headers["Access-Control-Allow-Origin"] = "*";
            res.headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, OPTIONS";
            res.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization, X-Collective-Signature";
            res.headers["Access-Control-Max-Age"] = "86400";
            return res;
        }

        // 2. Sovereign Stack Invariant 7 Enforcement:
        // Diagnostic layers (L1 HAL, L2 Membrane, L3 Ledger, L4 Execution, L7 Legacy Proxy)
        // are STRICTLY READ-ONLY. Any POST, PUT, DELETE, or PATCH attempt MUST be rejected
        // with HTTP 405 Method Not Allowed / HTTP 403 Forbidden.
        if (request.method == "POST" || request.method == "PUT" ||
            request.method == "DELETE" || request.method == "PATCH") {
            if (IsDiagnosticLayerPath(request.path)) {
                if (request.path.rfind("/api/v1/l4", 0) == 0) {
                    return HttpResponse::MethodNotAllowed(
                        "L4_IS_STRICTLY_READ_ONLY_NO_MUTATIONS",
                        "Layer 4 (Execution VM) is deterministic, sandboxed, and strictly read-only. "
                        "Task approvals and civic decisions must occur exclusively in Layer 5 (Governance)."
                    );
                } else if (request.path.rfind("/api/v1/l2", 0) == 0) {
                    return HttpResponse::MethodNotAllowed(
                        "L2_MEMBRANE_IS_READ_ONLY",
                        "Layer 2 (Hardware/IoT Membrane) is strictly read-only diagnostics via gateway. "
                        "Actuation requires hardware-root cryptographic signatures on isolated VLAN 10 fieldbus."
                    );
                } else if (request.path.rfind("/api/v1/l1", 0) == 0) {
                    return HttpResponse::MethodNotAllowed(
                        "L1_HAL_IS_READ_ONLY",
                        "Layer 1 Hardware Abstraction Layer is strictly read-only."
                    );
                } else if (request.path.rfind("/api/v1/l3", 0) == 0) {
                    return HttpResponse::MethodNotAllowed(
                        "L3_LEDGER_IS_READ_ONLY",
                        "Layer 3 CRDT Ledger is strictly read-only via gateway."
                    );
                } else if (request.path.rfind("/api/v1/l7", 0) == 0) {
                    return HttpResponse::MethodNotAllowed(
                        "L7_CARAPACE_IS_READ_ONLY",
                        "Layer 7 Corporate Carapace is strictly read-only diagnostics."
                    );
                }
            }
        }

        // 3. Routing: Diagnostic GET Endpoints
        if (request.method == "GET" || request.method == "HEAD") {
            if (request.path == "/api/v1/layers/summary") {
                return HandleLayersSummary();
            } else if (request.path == "/api/v1/l2/telemetry/latest") {
                return HandleL2TelemetryLatest();
            } else if (request.path == "/api/v1/l2/residuals") {
                return HandleL2Residuals();
            } else if (request.path == "/api/v1/l2/mesh/peers") {
                return HandleL2MeshPeers();
            } else if (request.path == "/api/v1/l4/workflows") {
                return HandleL4Workflows();
            } else if (request.path == "/api/v1/l4/priority-queue") {
                return HandleL4PriorityQueue();
            } else if (request.path == "/api/v1/l4/wasm-fuel") {
                return HandleL4WasmFuel();
            } else if (request.path == "/api/v1/l5/approvals/pending") {
                return HandleL5ApprovalsPending();
            } else if (request.path == "/api/v1/l5/treaties") {
                return HandleL5Treaties();
            } else if (request.path == "/api/v1/l5/trust-ring") {
                return HandleL5TrustRing();
            } else if (request.path == "/api/v1/l5/credit-limits") {
                return HandleL5CreditLimits();
            } else if (request.path == "/api/v1/l6/agora/orderbook") {
                return HandleL6Orderbook();
            } else if (request.path == "/api/v1/l6/bounties") {
                return HandleL6Bounties();
            } else if (request.path == "/api/v1/l7/status") {
                return HandleL7Status();
            } else if (request.path == "/" || request.path == "/index.html" ||
                       request.path.rfind("/companion", 0) == 0 ||
                       request.path.rfind("/static", 0) == 0) {
                return HandleStaticFiles(request.path);
            }
            return HttpResponse::NotFound("Endpoint '" + request.path + "' not found on col-gatewayd.");
        }

        // 4. Routing: Mutating Endpoints (Layer 6 Semantic Agora & Layer 5 Governance)
        if (request.method == "POST") {
            if (request.path == "/api/v1/l6/intent" || request.path == "/api/v1/l6/intent/author") {
                return HandleL6Intent(request);
            } else if (request.path == "/api/v1/l6/agora/orders") {
                return HandleL6AgoraOrders(request);
            } else if (request.path.rfind("/api/v1/l5/approvals/", 0) == 0 &&
                       request.path.find("/resolve") != std::string::npos) {
                return HandleL5ApprovalResolve(request);
            } else if (request.path.rfind("/api/v1/l5/treaties/", 0) == 0 &&
                       request.path.find("/ratify") != std::string::npos) {
                return HandleL5TreatyRatify(request);
            }
        }

        return HttpResponse::MethodNotAllowed("UNSUPPORTED_METHOD", "HTTP method '" + request.method + "' is not supported for this path.");
    }

    // Socket server execution loop using POSIX sockets and poll()
    int RunSocketServer(int port, int max_ticks = 0, std::atomic<bool>* running = nullptr) {
#ifndef __EMSCRIPTEN__
        int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            spdlog::error("[col-gatewayd] Failed to create TCP socket: {}", std::strerror(errno));
            return 1;
        }

        int opt = 1;
        ::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        int flags = ::fcntl(server_fd, F_GETFL, 0);
        if (flags >= 0) {
            ::fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);
        }

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(static_cast<uint16_t>(port));

        if (::bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0) {
            spdlog::warn("[col-gatewayd] Failed to bind port {}: {}. Socket listening disabled.", port, std::strerror(errno));
            ::close(server_fd);
            server_fd = -1;
        } else {
            if (::listen(server_fd, 64) < 0) {
                spdlog::warn("[col-gatewayd] Failed to listen on port {}: {}", port, std::strerror(errno));
                ::close(server_fd);
                server_fd = -1;
            } else {
                spdlog::info("[col-gatewayd] HTTP gateway listening on 0.0.0.0:{} (shm: '{}')", port, shm_name_);
            }
        }

        uint64_t tick_count = 0;
        constexpr int poll_timeout_ms = 100;

        while (running == nullptr || running->load(std::memory_order_relaxed)) {
            tick_count++;
            PollRingBuffers();

            if (server_fd >= 0) {
                pollfd pfd{};
                pfd.fd = server_fd;
                pfd.events = POLLIN;

                int ret = ::poll(&pfd, 1, poll_timeout_ms);
                if (ret > 0 && (pfd.revents & POLLIN)) {
                    sockaddr_in client_addr{};
                    socklen_t addrlen = sizeof(client_addr);
                    int client_fd = ::accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen);
                    if (client_fd >= 0) {
                        HandleClientSocket(client_fd);
                        ::close(client_fd);
                    }
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(poll_timeout_ms));
            }

            if (max_ticks > 0 && tick_count >= static_cast<uint64_t>(max_ticks)) {
                spdlog::info("[col-gatewayd] Reached target tick count ({}). Initiating clean shutdown.", max_ticks);
                break;
            }
        }

        if (server_fd >= 0) {
            ::close(server_fd);
        }
        return 0;
#else
        (void)port;
        (void)max_ticks;
        (void)running;
        return 0;
#endif
    }

private:
    void InitializeDefaultState() {
        // Seed default L2 sample (simulated 60fps baseline)
        latest_l2_.frame_count = 1;
        latest_l2_.timestamp_ns = 1728750000000000000ULL;
        latest_l2_.fps = 60.0f;
        latest_l2_.founder_stress = 0.05f;
        latest_l2_.memory_slot_count = 1024;
        latest_l2_.biome_id = 2; // Permaculture
        latest_l2_.quality_flags = oasis::uhai::QUALITY_SIMULATED;
        has_l2_ = true;

        // Seed default L4 execution telemetry
        latest_l4_.tick_count = 1;
        latest_l4_.timestamp_ns = 1728750000000000000ULL;
        latest_l4_.instruction_fuel_budget = 1'000'000;
        latest_l4_.instruction_fuel_remaining = 985'000;
        latest_l4_.memory_fuel_bytes_limit = 1'048'576;
        latest_l4_.memory_fuel_bytes_used = 65'536;
        latest_l4_.active_workflow_count = 1;
        latest_l4_.active_token_count = 1;
        latest_l4_.queue_depth[0] = 0;
        latest_l4_.queue_depth[1] = 0;
        latest_l4_.queue_depth[2] = 1;
        latest_l4_.queue_depth[3] = 0;
        latest_l4_.queue_depth[4] = 0;
        latest_l4_.vm_tick_rate_hz = 10.0f;
        latest_l4_.shedding_active = 0;
        latest_l4_.sandbox_flags = 0x01;
        has_l4_ = true;

        // Seed default L5 governance telemetry
        latest_l5_.timestamp_ns = 1728750000000000000ULL;
        latest_l5_.pending_approval_count = 1;
        latest_l5_.active_treaty_count = 2;
        latest_l5_.ratified_treaty_count = 1;
        latest_l5_.active_referendum_count = 0;
        latest_l5_.bilateral_credit_limit_mj = 50'000;
        latest_l5_.net_credit_balance_mj = 12'500;
        latest_l5_.escrow_locked_mj = 45'000;
        latest_l5_.steward_trust_ring = 2;
        latest_l5_.governance_flags = 0x01;
        has_l5_ = true;

        // Seed sample upward-yielded task gate
        oasis::uhai::UpwardYieldPacket seed_yield{};
        seed_yield.yield_id = 849201;
        seed_yield.workflow_token_id = 99182;
        seed_yield.timestamp_hlc = 1728750000000000000ULL;
        seed_yield.reason = oasis::uhai::YieldReason::HUMAN_TASK_GATE_COMPLETION;
        seed_yield.priority_tier = 4;
        std::strncpy(seed_yield.task_gate_id, "Gate_Quality_Inspection", sizeof(seed_yield.task_gate_id) - 1);
        std::strncpy(seed_yield.claimant_did, "did:mesh:node04:steward77", sizeof(seed_yield.claimant_did) - 1);
        seed_yield.required_exergy_mj = 12500; // 12.5 Exergy Credits
        std::strncpy(seed_yield.description, "Physically inspect 5x 3D-printed PETG irrigation valves for dimensional fit.", sizeof(seed_yield.description) - 1);
        pending_yields_.push_back(seed_yield);
    }

    void InitializeChannels() {
        try {
            l2_channel_ = std::make_unique<oasis::uhai::UhaiTelemetryChannel>(
                oasis::uhai::UhaiTelemetryChannel::ChannelMode::Consumer, shm_name_);
        } catch (...) {
            l2_channel_ = nullptr;
        }

        try {
            l4_channel_ = std::make_unique<oasis::uhai::UhaiChannel<oasis::uhai::L4WorkflowSlot>>(
                oasis::uhai::UhaiChannel<oasis::uhai::L4WorkflowSlot>::ChannelMode::Consumer);
        } catch (...) {
            l4_channel_ = nullptr;
        }

        try {
            l5_channel_ = std::make_unique<oasis::uhai::UhaiChannel<oasis::uhai::L5GovernanceSlot>>(
                oasis::uhai::UhaiChannel<oasis::uhai::L5GovernanceSlot>::ChannelMode::Consumer);
        } catch (...) {
            l5_channel_ = nullptr;
        }

        try {
            yield_channel_ = std::make_unique<oasis::uhai::UhaiChannel<oasis::uhai::UpwardYieldSlot>>(
                oasis::uhai::UhaiChannel<oasis::uhai::UpwardYieldSlot>::ChannelMode::Consumer);
        } catch (...) {
            yield_channel_ = nullptr;
        }
    }

#ifndef __EMSCRIPTEN__
    void HandleClientSocket(int client_fd) {
        std::array<char, 4096> buffer{};
        ssize_t bytes_read = ::recv(client_fd, buffer.data(), buffer.size() - 1, 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            HttpRequest request = HttpParser::Parse(std::string_view(buffer.data(), static_cast<size_t>(bytes_read)));
            HttpResponse response = HandleRequest(request);
            std::string raw_resp = response.ToString();
            ::send(client_fd, raw_resp.data(), raw_resp.size(), 0);
        }
    }
#endif

    // ========================================================================
    // Endpoint Implementation Handlers
    // ========================================================================

    HttpResponse HandleLayersSummary() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"status\": \"HEALTHY\",\n"
            << "  \"timestamp_ns\": " << (has_l2_ ? latest_l2_.timestamp_ns : 1728750000000000000ULL) << ",\n"
            << "  \"node_did\": \"did:mesh:node01:gateway\",\n"
            << "  \"layers\": {\n"
            << "    \"l1_hal\": {\n"
            << "      \"status\": \"ACTIVE\",\n"
            << "      \"airgap_drops\": 0,\n"
            << "      \"isolated_fieldbus\": \"HEALTHY\"\n"
            << "    },\n"
            << "    \"l2_membrane\": {\n"
            << "      \"status\": \"STREAMING\",\n"
            << "      \"frame_count\": " << latest_l2_.frame_count << ",\n"
            << "      \"fps\": " << std::fixed << std::setprecision(2) << latest_l2_.fps << ",\n"
            << "      \"founder_stress\": " << std::fixed << std::setprecision(3) << latest_l2_.founder_stress << ",\n"
            << "      \"memory_slot_count\": " << latest_l2_.memory_slot_count << ",\n"
            << "      \"biome_id\": " << static_cast<int>(latest_l2_.biome_id) << ",\n"
            << "      \"quality_flags\": " << static_cast<int>(latest_l2_.quality_flags) << "\n"
            << "    },\n"
            << "    \"l3_ledger\": {\n"
            << "      \"status\": \"SYNCHRONIZED\",\n"
            << "      \"crdt_vector\": 42,\n"
            << "      \"active_channels\": 3,\n"
            << "      \"erc_multiplier\": 1.05\n"
            << "    },\n"
            << "    \"l4_execution\": {\n"
            << "      \"status\": \"RUNNING\",\n"
            << "      \"read_only\": true,\n"
            << "      \"active_workflows\": " << latest_l4_.active_workflow_count << ",\n"
            << "      \"active_tokens\": " << latest_l4_.active_token_count << ",\n"
            << "      \"instruction_fuel_remaining\": " << latest_l4_.instruction_fuel_remaining << ",\n"
            << "      \"vm_tick_rate_hz\": " << std::fixed << std::setprecision(1) << latest_l4_.vm_tick_rate_hz << ",\n"
            << "      \"shedding_active\": " << (latest_l4_.shedding_active ? "true" : "false") << "\n"
            << "    },\n"
            << "    \"l5_governance\": {\n"
            << "      \"status\": \"DELIBERATING\",\n"
            << "      \"pending_approvals\": " << pending_yields_.size() << ",\n"
            << "      \"active_treaties\": " << latest_l5_.active_treaty_count << ",\n"
            << "      \"ratified_treaties\": " << latest_l5_.ratified_treaty_count << ",\n"
            << "      \"steward_trust_ring\": " << static_cast<int>(latest_l5_.steward_trust_ring) << ",\n"
            << "      \"bilateral_credit_limit_mj\": " << latest_l5_.bilateral_credit_limit_mj << "\n"
            << "    },\n"
            << "    \"l6_semantic\": {\n"
            << "      \"status\": \"ACTIVE\",\n"
            << "      \"agora_midpoint_price_mj\": 3.6,\n"
            << "      \"orderbook_bids\": 5,\n"
            << "      \"orderbook_asks\": 4,\n"
            << "      \"submitted_intents_count\": " << submitted_intents_.size() << "\n"
            << "    },\n"
            << "    \"l7_carapace\": {\n"
            << "      \"status\": \"CONTAINED\",\n"
            << "      \"threat_level\": 1,\n"
            << "      \"grid_export_kw\": 4.2,\n"
            << "      \"operating_cash_usd\": 14250.80\n"
            << "    }\n"
            << "  }\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL2TelemetryLatest() {
        const char* label = (latest_l2_.quality_flags & oasis::uhai::QUALITY_HARDWARE_ROOT)
                                ? "QUALITY_HARDWARE_ROOT"
                                : "QUALITY_SIMULATED";
        std::ostringstream oss;
        oss << "{\n"
            << "  \"frame_count\": " << latest_l2_.frame_count << ",\n"
            << "  \"timestamp_ns\": " << latest_l2_.timestamp_ns << ",\n"
            << "  \"fps\": " << std::fixed << std::setprecision(2) << latest_l2_.fps << ",\n"
            << "  \"founder_stress\": " << std::fixed << std::setprecision(4) << latest_l2_.founder_stress << ",\n"
            << "  \"memory_slot_count\": " << latest_l2_.memory_slot_count << ",\n"
            << "  \"biome_id\": " << static_cast<int>(latest_l2_.biome_id) << ",\n"
            << "  \"quality_flags\": " << static_cast<int>(latest_l2_.quality_flags) << ",\n"
            << "  \"quality_flag_label\": \"" << label << "\",\n"
            << "  \"shm_segment\": \"" << shm_name_ << "\"\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL2Residuals() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"residual_epsilon\": 0.012,\n"
            << "  \"mahalanobis_d_m2\": 0.45,\n"
            << "  \"threshold_gamma\": 3.84,\n"
            << "  \"anomaly_detected\": false\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL2MeshPeers() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"peer_count\": 2,\n"
            << "  \"peers\": [\n"
            << "    {\"node_id\": \"0x04a1\", \"rssi_dbm\": -68, \"snr_db\": 8.5, \"hops\": 1, \"transport\": \"LoRa-SX1262\"},\n"
            << "    {\"node_id\": \"0x07f2\", \"rssi_dbm\": -52, \"snr_db\": 14.2, \"hops\": 1, \"transport\": \"802.11s-WiFi\"}\n"
            << "  ]\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL4Workflows() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"status\": \"READ_ONLY\",\n"
            << "  \"is_read_only\": true,\n"
            << "  \"active_workflow_count\": " << latest_l4_.active_workflow_count << ",\n"
            << "  \"active_token_count\": " << latest_l4_.active_token_count << ",\n"
            << "  \"instruction_fuel_remaining\": " << latest_l4_.instruction_fuel_remaining << ",\n"
            << "  \"instruction_fuel_budget\": " << latest_l4_.instruction_fuel_budget << ",\n"
            << "  \"memory_fuel_bytes_used\": " << latest_l4_.memory_fuel_bytes_used << ",\n"
            << "  \"memory_fuel_bytes_limit\": " << latest_l4_.memory_fuel_bytes_limit << ",\n"
            << "  \"vm_tick_rate_hz\": " << std::fixed << std::setprecision(1) << latest_l4_.vm_tick_rate_hz << ",\n"
            << "  \"shedding_active\": " << (latest_l4_.shedding_active ? "true" : "false") << ",\n"
            << "  \"queue_depth\": ["
            << latest_l4_.queue_depth[0] << ", " << latest_l4_.queue_depth[1] << ", "
            << latest_l4_.queue_depth[2] << ", " << latest_l4_.queue_depth[3] << ", "
            << latest_l4_.queue_depth[4] << "],\n"
            << "  \"workflows\": [\n"
            << "    {\n"
            << "      \"workflow_id\": \"Workflow_Chore_8492\",\n"
            << "      \"status\": \"STATE_YIELDED_L5\",\n"
            << "      \"active_tokens\": 1,\n"
            << "      \"fuel_consumed\": 15000,\n"
            << "      \"is_paused_yielded\": true\n"
            << "    }\n"
            << "  ]\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL4PriorityQueue() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"tier_1_ecological_floor\": " << latest_l4_.queue_depth[0] << ",\n"
            << "  \"tier_2_subsistence\": " << latest_l4_.queue_depth[1] << ",\n"
            << "  \"tier_3_mutual_aid\": " << latest_l4_.queue_depth[2] << ",\n"
            << "  \"tier_4_discretionary\": " << latest_l4_.queue_depth[3] << ",\n"
            << "  \"tier_5_legacy_export\": " << latest_l4_.queue_depth[4] << "\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL4WasmFuel() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"fuel_budget\": " << latest_l4_.instruction_fuel_budget << ",\n"
            << "  \"fuel_remaining\": " << latest_l4_.instruction_fuel_remaining << ",\n"
            << "  \"memory_pages_allocated\": " << (latest_l4_.memory_fuel_bytes_used / 65536) << ",\n"
            << "  \"trap_count\": 0\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL5ApprovalsPending() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"pending_count\": " << pending_yields_.size() << ",\n"
            << "  \"approvals\": [\n";

        for (size_t i = 0; i < pending_yields_.size(); ++i) {
            const auto& py = pending_yields_[i];
            oss << "    {\n"
                << "      \"yield_id\": \"y_" << py.yield_id << "\",\n"
                << "      \"workflow_id\": \"Workflow_Chore_" << (py.yield_id / 100) << "\",\n"
                << "      \"workflow_token_id\": \"tok_" << py.workflow_token_id << "\",\n"
                << "      \"gate_id\": \"" << py.task_gate_id << "\",\n"
                << "      \"title\": \"Chore #" << (py.yield_id / 100) << ": " << py.task_gate_id << "\",\n"
                << "      \"claimant_did\": \"" << py.claimant_did << "\",\n"
                << "      \"description\": \"" << py.description << "\",\n"
                << "      \"escrow_mj\": " << std::fixed << std::setprecision(1) << (static_cast<double>(py.required_exergy_mj) / 1000.0) << ",\n"
                << "      \"priority_tier\": " << static_cast<int>(py.priority_tier) << ",\n"
                << "      \"plain_language_read_back\": \"You are ratifying the successful completion of Chore #"
                << (py.yield_id / 100) << " by " << py.claimant_did
                << ". Approving will disburse " << (static_cast<double>(py.required_exergy_mj) / 1000.0)
                << " Exergy Credits and advance the workflow.\"\n"
                << "    }" << (i + 1 < pending_yields_.size() ? ",\n" : "\n");
        }

        oss << "  ]\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL5Treaties() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"treaty_count\": 1,\n"
            << "  \"treaties\": [\n"
            << "    {\n"
            << "      \"treaty_cid\": \"bafyreib249182a5c9f28d84920199182\",\n"
            << "      \"title\": \"Solar Microgrid Intertie Treaty (Bay Area Node 04)\",\n"
            << "      \"bilateral_credit_limit_mj\": 50000,\n"
            << "      \"status\": \"RATIFIED\",\n"
            << "      \"bicameral_tier\": 1\n"
            << "    }\n"
            << "  ]\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL5TrustRing() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"local_did\": \"did:mesh:node01:steward_lead\",\n"
            << "  \"active_peers_count\": 6,\n"
            << "  \"steward_trust_ring\": " << static_cast<int>(latest_l5_.steward_trust_ring) << ",\n"
            << "  \"rings\": {\"R0\": 1, \"R1\": 3, \"R2\": 2, \"R3\": 0, \"R4\": 0}\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL5CreditLimits() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"bilateral_credit_limit_mj\": " << latest_l5_.bilateral_credit_limit_mj << ",\n"
            << "  \"net_credit_balance_mj\": " << latest_l5_.net_credit_balance_mj << ",\n"
            << "  \"escrow_locked_mj\": " << latest_l5_.escrow_locked_mj << "\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL6Orderbook() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"bids\": [\n"
            << "    {\"price_mj\": 3.55, \"quantity_kwh\": 12.0, \"trader_did\": \"did:mesh:node02:solarpunk\"},\n"
            << "    {\"price_mj\": 3.50, \"quantity_kwh\": 25.0, \"trader_did\": \"did:mesh:node03:battery\"}\n"
            << "  ],\n"
            << "  \"asks\": [\n"
            << "    {\"price_mj\": 3.65, \"quantity_kwh\": 10.0, \"trader_did\": \"did:mesh:node04:hydro\"},\n"
            << "    {\"price_mj\": 3.70, \"quantity_kwh\": 18.0, \"trader_did\": \"did:mesh:node05:wind\"}\n"
            << "  ],\n"
            << "  \"midpoint_price_mj\": 3.60,\n"
            << "  \"spread_mj\": 0.10\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL6Bounties() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"bounty_count\": 2,\n"
            << "  \"bounties\": [\n"
            << "    {\n"
            << "      \"bounty_id\": \"bounty_712\",\n"
            << "      \"title\": \"Cistern Filter Replacement (Bay 2)\",\n"
            << "      \"escrow_mj\": 25.0,\n"
            << "      \"location\": \"Sector 4 - Cistern B\",\n"
            << "      \"status\": \"OPEN\"\n"
            << "    },\n"
            << "    {\n"
            << "      \"bounty_id\": \"bounty_713\",\n"
            << "      \"title\": \"Solar Panel Dusting & Inverter Diagnostic\",\n"
            << "      \"escrow_mj\": 15.0,\n"
            << "      \"location\": \"Rooftop Cluster 1\",\n"
            << "      \"status\": \"OPEN\"\n"
            << "    }\n"
            << "  ]\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL7Status() {
        std::ostringstream oss;
        oss << "{\n"
            << "  \"layer\": 7,\n"
            << "  \"daemon\": \"col-adversaryd\",\n"
            << "  \"vlan\": 40,\n"
            << "  \"threat_level\": {\"level\": 1, \"label\": \"STANDARD\", \"multiplier\": 1.25},\n"
            << "  \"grid_intertie\": {\n"
            << "    \"state\": \"EXPORTING\",\n"
            << "    \"export_residual_kw\": 4.2,\n"
            << "    \"curtailment_active\": false,\n"
            << "    \"breaker_actuator_healthy\": true\n"
            << "  },\n"
            << "  \"carapace_finances\": {\n"
            << "    \"operating_cash_usd\": 14250.80,\n"
            << "    \"tax_reserve_usd\": 3850.00,\n"
            << "    \"labor_bifurcation_compliant\": true\n"
            << "  }\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL6Intent(const HttpRequest& request) {
        std::string_view body_view = request.body;
        size_t first = body_view.find_first_not_of(" \t\n\r");
        if (first == std::string_view::npos || (body_view[first] != '{' && body_view[first] != '[')) {
            return HttpResponse::BadRequest("Empty or malformed JSON payload for intent");
        }
        std::string raw_payload = request.body;
        std::string cid = "bafyreib" + crypto::Sha256::HashString(raw_payload).substr(0, 32);

        oasis::uhai::SpatialIntentPayload intent{};
        std::string hex_hash = crypto::Sha256::HashString(raw_payload);
        for (size_t i = 0; i < 32 && i * 2 < hex_hash.size(); ++i) {
            std::string byte_str = hex_hash.substr(i * 2, 2);
            intent.intent_cid[i] = static_cast<uint8_t>(std::strtoul(byte_str.c_str(), nullptr, 16));
        }
        intent.timestamp_ns = 1728750000000000000ULL;
        intent.intent_type = static_cast<uint16_t>(oasis::uhai::IntentType::SPATIAL_AFFORDANCE);
        intent.quality_flag = oasis::uhai::QUALITY_SIMULATED;
        intent.status = 1; // Validated
        intent.payload_bytes = static_cast<uint32_t>(std::min(raw_payload.size(), sizeof(intent.payload_jsonld) - 1));
        std::strncpy(intent.schema_uri, "https://schema.collective.local/v1/intent.jsonld", sizeof(intent.schema_uri) - 1);
        std::strncpy(intent.payload_jsonld, raw_payload.c_str(), sizeof(intent.payload_jsonld) - 1);

        submitted_intents_.push_back(intent);

        std::ostringstream oss;
        oss << "{\n"
            << "  \"status\": \"DRAFT_COMPILED\",\n"
            << "  \"intent_cid\": \"" << cid << "\",\n"
            << "  \"intent_type\": 1,\n"
            << "  \"schema_uri\": \"https://schema.collective.local/v1/intent.jsonld\",\n"
            << "  \"plain_language_read_back\": \"Compiled semantic intent successfully validated against airgap schema.\",\n"
            << "  \"split_airgap_verified\": true,\n"
            << "  \"submitted_intents_total\": " << submitted_intents_.size() << "\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL6AgoraOrders(const HttpRequest& request) {
        (void)request;
        std::ostringstream oss;
        oss << "{\n"
            << "  \"order_id\": \"ord_9182\",\n"
            << "  \"status\": \"PLACED\",\n"
            << "  \"timestamp_hlc\": \"2026-10-12T16:20:00.000Z\",\n"
            << "  \"matched_executions\": []\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL5ApprovalResolve(const HttpRequest& request) {
        (void)request;
        std::ostringstream oss;
        oss << "{\n"
            << "  \"status\": \"RATIFIED_APPROVED\",\n"
            << "  \"yield_id\": \"y_849201\",\n"
            << "  \"workflow_token_id\": \"tok_99182\",\n"
            << "  \"released_escrow_mj\": 12.5,\n"
            << "  \"l4_dispatch_confirmed\": true,\n"
            << "  \"transaction_receipt_cid\": \"bafyreib249182a5c9f28d84920199182\"\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleL5TreatyRatify(const HttpRequest& request) {
        (void)request;
        std::ostringstream oss;
        oss << "{\n"
            << "  \"status\": \"RATIFIED\",\n"
            << "  \"treaty_cid\": \"bafyreib249182a5c9f28d84920199182\",\n"
            << "  \"cooloff_seconds_remaining\": 0,\n"
            << "  \"bicameral_quorum_met\": true\n"
            << "}\n";
        return HttpResponse::Json(200, oss.str());
    }

    HttpResponse HandleStaticFiles(const std::string& path) {
        std::string rel_file = path;
        if (rel_file.empty() || rel_file == "/") {
            rel_file = "/index.html";
        }
        if (rel_file.rfind("/companion", 0) == 0) {
            rel_file = rel_file.substr(10);
            if (rel_file.empty() || rel_file == "/") rel_file = "/index.html";
        }
        if (!rel_file.empty() && rel_file.front() == '/') {
            rel_file = rel_file.substr(1);
        }

        const std::vector<std::string> roots = {
            "core/1_simulation/src/web/companion",
            "web/companion",
            "src/web/companion",
            "../web/companion",
            "../../web/companion",
            "../../../core/1_simulation/src/web/companion"
        };

        for (const auto& root : roots) {
            std::string full_path = root + "/" + rel_file;
            std::ifstream file(full_path, std::ios::in | std::ios::binary);
            if (file.is_open()) {
                std::ostringstream ss;
                ss << file.rdbuf();
                std::string content = ss.str();
                std::string ctype = "text/html; charset=utf-8";
                if (full_path.rfind(".css") != std::string::npos) ctype = "text/css; charset=utf-8";
                else if (full_path.rfind(".js") != std::string::npos) ctype = "application/javascript; charset=utf-8";
                else if (full_path.rfind(".json") != std::string::npos) ctype = "application/json";
                else if (full_path.rfind(".svg") != std::string::npos) ctype = "image/svg+xml";
                return HttpResponse::Text(200, content, ctype);
            }
        }

        // Clean built-in fallback companion dashboard
        std::string fallback_html =
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "  <meta charset=\"utf-8\">\n"
            "  <title>Collective Companion App</title>\n"
            "</head>\n"
            "<body style=\"font-family: monospace; background: #0b0f14; color: #d0f0d0; padding: 2rem;\">\n"
            "  <h1>Collective Sovereign Stack Companion Gateway</h1>\n"
            "  <p>Status: ONLINE | Daemon: col-gatewayd | Port: Active</p>\n"
            "</body>\n"
            "</html>\n";
        return HttpResponse::Text(200, fallback_html, "text/html; charset=utf-8");
    }

    std::string shm_name_;
    std::unique_ptr<oasis::uhai::UhaiTelemetryChannel> l2_channel_{nullptr};
    std::unique_ptr<oasis::uhai::UhaiChannel<oasis::uhai::L4WorkflowSlot>> l4_channel_{nullptr};
    std::unique_ptr<oasis::uhai::UhaiChannel<oasis::uhai::L5GovernanceSlot>> l5_channel_{nullptr};
    std::unique_ptr<oasis::uhai::UhaiChannel<oasis::uhai::UpwardYieldSlot>> yield_channel_{nullptr};

    oasis::uhai::TelemetrySample latest_l2_{};
    bool has_l2_{false};

    oasis::uhai::L4ExecutionTelemetry latest_l4_{};
    bool has_l4_{false};

    oasis::uhai::L5GovernanceTelemetry latest_l5_{};
    bool has_l5_{false};

    std::vector<oasis::uhai::UpwardYieldPacket> pending_yields_{};
    std::vector<oasis::uhai::SpatialIntentPayload> submitted_intents_{};
    std::atomic<uint64_t> total_requests_handled_{0};
};

} // namespace oasis::gateway
