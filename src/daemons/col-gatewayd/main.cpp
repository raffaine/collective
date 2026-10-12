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
 * Architecture & Responsibilities (PROJECT.md, COMPANION_APP_ARCHITECTURE.md):
 * - Standalone POSIX daemon terminating HTTP REST and companion dashboard
 *   interactions for the Sovereign Stack.
 * - Enforces Anti-Bleed Invariants 1-4 & Core Invariant 7:
 *   - Layers 1, 2, 3, 4, and 7 are STRICTLY READ-ONLY diagnostics.
 *   - Any mutation attempts (POST/PUT/DELETE) to /api/v1/l1/..., /api/v1/l2/...,
 *     /api/v1/l3/..., /api/v1/l4/..., or /api/v1/l7/... are rejected with HTTP 405/403.
 *   - Mutating societal actions enter strictly via Layer 6 (POST /api/v1/l6/intent)
 *     or Layer 5 Governance (POST /api/v1/l5/approvals/:id/resolve).
 * - Reads UHAI shared memory ring buffers (L2 telemetry, L4 execution, L5 governance)
 *   via dedicated consumer segment (/uhai_telemetry_gateway).
 * - Serves JSON GET diagnostic readouts and Companion App dashboard assets.
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
              << "  --port <port>         HTTP listen port (default: 8443)\n"
              << "  --shm-name <name>     UHAI telemetry shared memory name (default: "
              << oasis::uhai::DEFAULT_GATEWAY_SHM_NAME << ")\n"
              << "  --frequency <Hz>      Telemetry polling frequency in Hz (default: 10)\n"
              << "  --ticks <N>           Run for N ticks then terminate cleanly (0 = run forever)\n"
              << "  --max-ticks <N>       Alias for --ticks\n"
              << "  --help, -h            Display this help message\n";
}

int main(int argc, char* argv[]) {
    int port = 8443;
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
    spdlog::info("[col-gatewayd] Sovereign Stack Layer 6 Semantic Gateway Daemon");
    spdlog::info("[col-gatewayd] Port: {}, ShmName: '{}', TargetHz: {}, MaxTicks: {}",
                 port, shm_name, frequency_hz, max_ticks);
    spdlog::info("[col-gatewayd] Invariant 7: Read-only protection active for L1, L2, L3, L4, L7");
    spdlog::info("[col-gatewayd] ========================================================");

    oasis::gateway::GatewayServer server(shm_name);

    int exit_code = server.RunSocketServer(port, max_ticks, &g_running);

    spdlog::info("[col-gatewayd] Gateway daemon shutdown complete. Requests handled: {}. Clean exit.",
                 server.GetTotalRequestsHandled());
    return exit_code;
}
