#!/usr/bin/env bash
# ==============================================================================
# Sovereign Stack Native POSIX Daemon Orchestrator (Phase 2 Modular Architecture)
# ==============================================================================
# Launches, monitors, and cleanly stops all 9 native C++20 POSIX daemons:
#   1. col_telemetryd  (L1/L2 UHAI Shared-Memory Telemetry & Airgap Filter)
#   2. col_storaged    (L3 Deterministic Exergy & Pairwise BCC CRDT Ledger)
#   3. col_meshd       (L2/L3 Peer Mesh Transport & Flow Channel Router)
#   4. col_execd       (L4 Deterministic Workflow Execution Engine)
#   5. col_kmsd        (L5 Cryptographic Trust-Ring & FROST Key Management)
#   6. col_commonsd    (L5/L6 Polycentric Commons & Agora Clearing Daemon)
#   7. col_adversaryd  (L7 Corporate Carapace & Adversarial Stress Daemon)
#   8. col_swarmd      (L3/L4/L6 Peer-Cluster Neighborhood Swarm Engine)
#   9. col_gatewayd    (L6 Live POSIX TCP/HTTP JSON-RPC & Companion Gateway)
#
# Usage:
#   ./scripts/run_native_daemons.sh {start|stop|status|restart}
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${COL_BUILD_DIR:-${REPO_ROOT}/core/1_simulation/src/build_native}"
RUN_DIR="${COL_RUN_DIR:-${BUILD_DIR}/run}"
HTTP_PORT="${COL_GATEWAY_HTTP_PORT:-8090}"

DAEMONS=(
  "col_telemetryd"
  "col_storaged"
  "col_meshd"
  "col_execd"
  "col_kmsd"
  "col_commonsd"
  "col_adversaryd"
  "col_swarmd"
  "col_gatewayd"
)

mkdir -p "${RUN_DIR}"

ensure_binaries_built() {
  mkdir -p "${BUILD_DIR}"

  local swarm_src="${REPO_ROOT}/src/daemons/col-swarmd/main.cpp"
  local swarm_hdr="${REPO_ROOT}/src/daemons/col-swarmd/swarm_engine.hpp"
  local stack_hdr="${REPO_ROOT}/src/stack/shared_stack_runtime.hpp"
  local swarm_bin="${BUILD_DIR}/col_swarmd"

  if [[ ! -x "${swarm_bin}" || "${swarm_src}" -nt "${swarm_bin}" || "${swarm_hdr}" -nt "${swarm_bin}" || "${stack_hdr}" -nt "${swarm_bin}" ]]; then
    echo "[daemons] Compiling col_swarmd with strict C++20 -Wpadded -Werror..."
    /usr/bin/c++ -std=c++20 -O2 -Wall -Wextra -Wpadded -Werror \
      "${swarm_src}" -o "${swarm_bin}"
    ln -sf col_swarmd "${BUILD_DIR}/col-swarmd"
  fi

  local gw_src="${REPO_ROOT}/src/daemons/col-gatewayd/main.cpp"
  local gw_hdr="${REPO_ROOT}/src/daemons/col-gatewayd/gateway_server.hpp"
  local gw_bin="${BUILD_DIR}/col_gatewayd"

  if [[ -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
    if [[ ! -x "${gw_bin}" || "${gw_src}" -nt "${gw_bin}" || "${gw_hdr}" -nt "${gw_bin}" || "${stack_hdr}" -nt "${gw_bin}" ]]; then
      echo "[daemons] Rebuilding col_gatewayd in ${BUILD_DIR}..."
      cmake --build "${BUILD_DIR}" --target col_gatewayd -j4
    fi
  fi
}

is_daemon_running() {
  local name="$1"
  local pid_file="${RUN_DIR}/${name}.pid"
  if [[ -f "${pid_file}" ]]; then
    local pid
    pid="$(cat "${pid_file}" 2>/dev/null || true)"
    if [[ -n "${pid}" ]] && kill -0 "${pid}" 2>/dev/null; then
      return 0
    fi
  fi
  return 1
}

daemon_args_for() {
  local name="$1"
  case "${name}" in
    col_gatewayd)
      printf -- "--http-port %s --frequency 10" "${HTTP_PORT}"
      ;;
    col_swarmd)
      printf -- "--frequency 2"
      ;;
    col_telemetryd|col_kmsd)
      printf -- ""
      ;;
    *)
      printf -- "--frequency 5"
      ;;
  esac
}

start_daemons() {
  ensure_binaries_built
  mkdir -p "${RUN_DIR}"

  local started=0
  local skipped=0

  for name in "${DAEMONS[@]}"; do
    local bin="${BUILD_DIR}/${name}"
    local pid_file="${RUN_DIR}/${name}.pid"
    local log_file="${RUN_DIR}/${name}.log"

    if [[ ! -x "${bin}" ]]; then
      echo "[daemons] WARN: Binary not found or not executable: ${bin} (skipping)"
      skipped=$((skipped + 1))
      continue
    fi

    if is_daemon_running "${name}"; then
      local existing_pid
      existing_pid="$(cat "${pid_file}")"
      echo "[daemons] ${name} already running (PID ${existing_pid})"
      continue
    fi

    rm -f "${pid_file}"
    local extra_args
    extra_args="$(daemon_args_for "${name}")"

    # Launch with REPO_ROOT as working directory and fully detached stdio so callers never block on pipe FDs
    pushd "${REPO_ROOT}" >/dev/null
    if [[ -n "${extra_args}" ]]; then
      # shellcheck disable=SC2086
      nohup "${bin}" ${extra_args} </dev/null >>"${log_file}" 2>&1 &
    else
      nohup "${bin}" </dev/null >>"${log_file}" 2>&1 &
    fi
    local new_pid=$!
    popd >/dev/null
    echo "${new_pid}" >"${pid_file}"
    echo "[daemons] Started ${name} (PID ${new_pid}) -> ${log_file}"
    started=$((started + 1))

    # Give col_telemetryd a brief moment to initialize /uhai_telemetry SHM header
    if [[ "${name}" == "col_telemetryd" ]]; then
      sleep 0.15
    fi
  done

  sleep 0.25
  echo "[daemons] Launch summary: ${started} newly started, ${skipped} missing. HTTP JSON-RPC: http://127.0.0.1:${HTTP_PORT}/api/v1/snapshot"
}

stop_daemons() {
  local stopped=0

  # Stop in reverse layer order (L6 gateway -> L1 telemetry)
  for (( idx=${#DAEMONS[@]}-1 ; idx>=0 ; idx-- )); do
    local name="${DAEMONS[idx]}"
    local pid_file="${RUN_DIR}/${name}.pid"

    if [[ -f "${pid_file}" ]]; then
      local pid
      pid="$(cat "${pid_file}" 2>/dev/null || true)"
      if [[ -n "${pid}" ]] && kill -0 "${pid}" 2>/dev/null; then
        kill -TERM "${pid}" 2>/dev/null || true
        for _ in {1..20}; do
          if ! kill -0 "${pid}" 2>/dev/null; then
            break
          fi
          sleep 0.1
        done
        if kill -0 "${pid}" 2>/dev/null; then
          kill -KILL "${pid}" 2>/dev/null || true
        fi
        echo "[daemons] Stopped ${name} (PID ${pid})"
        stopped=$((stopped + 1))
      fi
      rm -f "${pid_file}"
    fi

    # Fallback: terminate any orphaned instance of this exact binary still holding a socket
    if pgrep -f "${BUILD_DIR}/${name}" >/dev/null 2>&1; then
      pkill -TERM -f "${BUILD_DIR}/${name}" 2>/dev/null || true
      sleep 0.15
      echo "[daemons] Cleaned up previous ${name} instance"
      stopped=$((stopped + 1))
    fi
  done

  echo "[daemons] Shutdown summary: ${stopped} daemon(s) stopped."
}

status_daemons() {
  local running_count=0
  local total_count="${#DAEMONS[@]}"

  printf "%-18s %-10s %-10s %s\n" "DAEMON" "STATE" "PID" "LOG"
  printf "%-18s %-10s %-10s %s\n" "------------------" "----------" "----------" "------------------------------"

  for name in "${DAEMONS[@]}"; do
    local pid_file="${RUN_DIR}/${name}.pid"
    local log_file="${RUN_DIR}/${name}.log"
    if is_daemon_running "${name}"; then
      local pid
      pid="$(cat "${pid_file}")"
      printf "%-18s %-10s %-10s %s\n" "${name}" "RUNNING" "${pid}" "${log_file}"
      running_count=$((running_count + 1))
    else
      printf "%-18s %-10s %-10s %s\n" "${name}" "STOPPED" "-" "${log_file}"
    fi
  done

  echo "------------------------------------------------------------------------------"
  echo "[daemons] Active POSIX daemons: ${running_count}/${total_count} (Gateway HTTP port: ${HTTP_PORT})"
}

usage() {
  cat <<EOF
Usage: $0 {start|stop|status|restart}

Environment overrides:
  COL_BUILD_DIR          Path to native build binaries (default: core/1_simulation/src/build_native)
  COL_RUN_DIR            Path for PID and log files (default: \$COL_BUILD_DIR/run)
  COL_GATEWAY_HTTP_PORT  Live HTTP JSON-RPC port for col_gatewayd (default: 8090)
EOF
}

ACTION="${1:-status}"
case "${ACTION}" in
  start)
    start_daemons
    ;;
  stop)
    stop_daemons
    ;;
  status)
    status_daemons
    ;;
  restart)
    stop_daemons
    start_daemons
    ;;
  --help|-h|help)
    usage
    ;;
  *)
    echo "Unknown command: ${ACTION}" >&2
    usage >&2
    exit 1
    ;;
esac
