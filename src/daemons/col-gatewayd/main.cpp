#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <spdlog/spdlog.h>
#include "gateway_server.hpp"

/**
 * =============================================================================
 * Sovereign Stack Layer 6 Semantic Gateway Daemon (col-gatewayd)
 * =============================================================================
 *
 * Architecture & Responsibilities (PROJECT.md, COMPANION_APP_ARCHITECTURE.md, SS-EP6-006):
 * - Standalone POSIX daemon terminating HTTPS/WSS (TLS 1.3, port 8443, VLAN 30)
 *   and companion dashboard interactions for the Sovereign Stack.
 * - Enforces GatewaySecurityConfig:
 *   - port = 8443, vlan_id = 30, tls_1_3_enforced = true, did_auth_required = true
 * - Enforces Anti-Bleed Invariants 1-4, Core Invariant 7, & SS-EP6-006:
 *   - Read-Only Cybernetic Hood on Layers 2, 3, 4, 5, and 7:
 *     Any POST, PUT, DELETE, or PATCH to /api/v1/l2/..., /api/v1/l3/...,
 *     /api/v1/l4/..., /api/v1/l5/..., or /api/v1/l7/... returns 403 Forbidden / 405 Method Not Allowed.
 *   - Strict Intent U-Turn (Human -> L6 -> L5 -> L4 -> [upward-yield] -> L5 -> L6 -> L7):
 *     Member ratifications of pending Layer 5 [upward-yield] holds enter exclusively
 *     via POST /api/v1/l6/intent (e.g., payload containing "RATIFY_L5_YIELD_HOLD"
 *     or semantic Agora actions), which col-gatewayd routes inward L6 -> L5
 *     (GovernanceResolutionPacket).
 * - Reads UHAI shared memory ring buffers (L2 telemetry, L4 execution, L5 governance)
 *   and writes to /uhai_spatial_intent (SpatialIntentSlot) and /uhai_l5_resolutions.
 * =============================================================================
 */

static std::atomic<bool> g_running{true};

static void SignalHandler(int signum) {
    (void)signum;
    g_running.store(false, std::memory_order_release);
}

static void PrintHelp(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [options]\n"
              << "Options:\n"
              << "  --port <port>         HTTPS/WSS listen port (default: 8443, VLAN 30)\n"
              << "  --shm-name <name>     UHAI telemetry shared memory name (default: "
              << oasis::uhai::DEFAULT_GATEWAY_SHM_NAME << ")\n"
              << "  --frequency <Hz>      Telemetry polling frequency in Hz (default: 10)\n"
              << "  --ticks <N>           Run for N ticks then terminate cleanly (0 = run forever)\n"
              << "  --max-ticks <N>       Alias for --ticks\n"
              << "  --help, -h            Display this help message\n";
}

int main(int argc, char* argv[]) {
    oasis::gateway::GatewaySecurityConfig security_cfg{};
    security_cfg.port = 8443;
    security_cfg.vlan_id = 30;
    security_cfg.tls_1_3_enforced = true;
    security_cfg.did_auth_required = true;

    int port = static_cast<int>(security_cfg.port);
    std::string shm_name = oasis::uhai::DEFAULT_GATEWAY_SHM_NAME;
    int frequency_hz = 10;
    int max_ticks = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintHelp(argv[0]);
            return 0;
        } else if (arg == "--port" && i + 1 < argc) {
            port = std::atoi(argv[++i]);
            security_cfg.port = static_cast<uint16_t>(port);
        } else if (arg == "--shm-name" && i + 1 < argc) {
            shm_name = argv[++i];
        } else if (arg == "--frequency" && i + 1 < argc) {
            frequency_hz = std::atoi(argv[++i]);
        } else if ((arg == "--ticks" || arg == "--max-ticks") && i + 1 < argc) {
            max_ticks = std::atoi(argv[++i]);
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            PrintHelp(argv[0]);
            return 1;
        }
    }

    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    spdlog::info("[col-gatewayd] ========================================================");
    spdlog::info("[col-gatewayd] Sovereign Stack Layer 6 Semantic Gateway Daemon (SS-EP6-006)");
    spdlog::info("[col-gatewayd] Port: {}, VLAN: {}, TLS 1.3: {}, DID Auth: {}, ShmName: '{}', TargetHz: {}, MaxTicks: {}",
                 security_cfg.port, security_cfg.vlan_id, security_cfg.tls_1_3_enforced,
                 security_cfg.did_auth_required, shm_name, frequency_hz, max_ticks);
    spdlog::info("[col-gatewayd] Read-Only Cybernetic Hood active on /api/v1/l2/*, /api/v1/l3/*, /api/v1/l4/*, /api/v1/l5/*, /api/v1/l7/* (403/405)");
    spdlog::info("[col-gatewayd] Strict Intent U-Turn: Human -> L6 -> L5 -> L4 -> [upward-yield] -> L5 -> L6 -> L7 via POST /api/v1/l6/intent");
    spdlog::info("[col-gatewayd] ========================================================");

    oasis::gateway::GatewayServer server(shm_name);
    server.SetSecurityConfig(security_cfg);

    int exit_code = server.RunSocketServer(port, max_ticks, &g_running);

    spdlog::info("[col-gatewayd] Gateway daemon shutdown complete. Requests handled: {}. Clean exit.",
                 server.GetTotalRequestsHandled());
    return exit_code;
}
