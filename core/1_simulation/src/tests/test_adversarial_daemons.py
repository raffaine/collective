#!/usr/bin/env python3
"""
Empirical Challenger: Daemon Lifecycle, Rapid Signal & Drain Adversarial Test Suite
Tests col-telemetryd and col-commonsd under rapid signals, queue draining,
and adversarial shared memory conditions.
"""

import sys
import os
import time
import signal
import struct
import subprocess
from multiprocessing import shared_memory

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "../build_native")
TELEMETRYD_BIN = os.path.join(BUILD_DIR, "col_telemetryd")
COMMONSD_BIN = os.path.join(BUILD_DIR, "col_commonsd")

TOTAL_SHM_SIZE = 65728
RING_CAPACITY = 1024
UHAI_MAGIC = 0x55484149
UHAI_VERSION = 1
CACHE_LINE_SIZE = 64

QUALITY_SIMULATED = 0x01
QUALITY_HARDWARE_ROOT = 0x02

failures = 0

def assert_true(cond, msg):
    global failures
    if not cond:
        print(f"[FAIL] {msg}", file=sys.stderr)
        failures += 1
    else:
        print(f"[PASS] {msg}")

def test_telemetryd_flags():
    print("\n--- Test A: col-telemetryd Help & CLI Parsing ---")
    proc = subprocess.run([TELEMETRYD_BIN, "--help"], capture_output=True, text=True)
    assert_true(proc.returncode == 0, "col-telemetryd --help exits with code 0")
    assert_true("Usage: col-telemetryd" in proc.stdout, "Help output contains usage string")

def test_commonsd_flags():
    print("\n--- Test B: col-commonsd Help & CLI Parsing ---")
    proc = subprocess.run([COMMONSD_BIN, "--help"], capture_output=True, text=True)
    assert_true(proc.returncode == 0, "col-commonsd --help exits with code 0")
    assert_true("Usage: col-commonsd" in proc.stdout, "Help output contains usage string")

def test_commonsd_ticks():
    print("\n--- Test C: col-commonsd Deterministic Ticks & Treaty Verification ---")
    shm_name = f"/test_adv_c_{os.getpid()}"
    proc = subprocess.run(
        [COMMONSD_BIN, "--shm-name", shm_name, "--ticks", "10", "--frequency", "50"],
        capture_output=True, text=True, timeout=5
    )
    assert_true(proc.returncode == 0, "col-commonsd exits cleanly with code 0 after 10 ticks")
    assert_true("Ricardian Treaty validated!" in proc.stdout or "Ricardian Treaty validated!" in proc.stderr,
                "Treaty CID validation reported in logs")
    assert_true("9ec184a0368b350de54a951cd06dc8113e3ef03cb277398946ff32ee28c87802" in (proc.stdout + proc.stderr),
                "Expected Treaty_CID hash present in logs")
    assert_true("[AGORA MATCH]" in (proc.stdout + proc.stderr), "Agora double-auction match logged")

def test_telemetryd_timeout():
    print("\n--- Test D: col-telemetryd Idle Timeout ---")
    shm_name = f"/test_adv_d_{os.getpid()}"
    start = time.time()
    proc = subprocess.run(
        [TELEMETRYD_BIN, "--shm-name", shm_name, "--timeout-ms", "150"],
        capture_output=True, text=True, timeout=3
    )
    elapsed = time.time() - start
    assert_true(proc.returncode == 0, "col-telemetryd exits with code 0 on idle timeout")
    assert_true(elapsed < 1.0, f"Exited within timeout window (took {elapsed:.2f}s)")
    assert_true("Startup timeout" in (proc.stdout + proc.stderr) or "Inactivity timeout" in (proc.stdout + proc.stderr),
                "Timeout reason logged in output")

def test_rapid_signal_cycles():
    print("\n--- Test E: Rapid Signal Cycles (SIGINT & SIGTERM) on Daemons ---")
    # Test 5 rapid spawn-and-signal cycles for col-telemetryd
    for i in range(5):
        shm_name = f"/test_adv_sig_telem_{i}_{os.getpid()}"
        proc = subprocess.Popen(
            [TELEMETRYD_BIN, "--shm-name", shm_name, "--timeout-ms", "5000"],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )
        time.sleep(0.03)  # Allow daemon to initialize signal handlers & attach shm
        proc.send_signal(signal.SIGINT)
        try:
            stdout, stderr = proc.communicate(timeout=2)
            assert_true(proc.returncode == 0, f"Cycle {i+1}: col-telemetryd SIGINT handled cleanly (returncode 0)")
            assert_true("Clean shutdown complete." in (stdout.decode() + stderr.decode()),
                        f"Cycle {i+1}: Clean shutdown logged")
        except subprocess.TimeoutExpired:
            proc.kill()
            assert_true(False, f"Cycle {i+1}: col-telemetryd failed to terminate upon SIGINT within 2s")

    # Test 5 rapid spawn-and-signal cycles for col-commonsd
    for i in range(5):
        shm_name = f"/test_adv_sig_comm_{i}_{os.getpid()}"
        proc = subprocess.Popen(
            [COMMONSD_BIN, "--shm-name", shm_name, "--frequency", "50"],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )
        time.sleep(0.03)
        proc.send_signal(signal.SIGTERM if (i % 2 == 0) else signal.SIGINT)
        try:
            stdout, stderr = proc.communicate(timeout=2)
            assert_true(proc.returncode == 0, f"Cycle {i+1}: col-commonsd signal handled cleanly (returncode 0)")
            assert_true("Clean exit." in (stdout.decode() + stderr.decode()),
                        f"Cycle {i+1}: col-commonsd Clean exit logged")
        except subprocess.TimeoutExpired:
            proc.kill()
            assert_true(False, f"Cycle {i+1}: col-commonsd failed to terminate upon signal within 2s")

def test_real_shm_airgap_filtering():
    print("\n--- Test F: Real POSIX Shared Memory Airgap Filtering Proof ---")
    shm_base_name = f"test_adv_airgap_{os.getpid()}"
    shm_posix_name = f"/{shm_base_name}"

    shm = shared_memory.SharedMemory(name=shm_base_name, create=True, size=TOTAL_SHM_SIZE)
    try:
        shm.buf[:TOTAL_SHM_SIZE] = bytearray(TOTAL_SHM_SIZE)

        # Write SharedRingHeader
        struct.pack_into("<QQ", shm.buf, 0, 20, 0) # write_index = 20, dropped = 0
        struct.pack_into("<Q", shm.buf, 64, 0)     # read_index = 0
        struct.pack_into("<IIII", shm.buf, 128, RING_CAPACITY, 64, UHAI_MAGIC, UHAI_VERSION)

        # Write 20 TelemetrySlots starting at offset 192
        sample_fmt = "<QQffIBBH32s"
        assert struct.calcsize(sample_fmt) == 64, "Slot size must be 64 bytes"

        for i in range(20):
            slot_offset = 192 + i * 64
            if i < 10:
                q_flag = QUALITY_SIMULATED # 0x01
            elif i < 15:
                q_flag = 0x03              # Composite spoof attempt (0x01 | 0x02)
            else:
                q_flag = QUALITY_HARDWARE_ROOT # 0x02

            slot_data = struct.pack(
                sample_fmt,
                i,                      # frame_count
                1000000000 + i * 16000, # timestamp_ns
                60.0,                   # fps
                0.25,                   # founder_stress
                i % 16,                 # memory_slot_count
                0,                      # biome_id
                q_flag,                 # quality_flags
                0,                      # reserved
                b'\x00' * 32            # pad
            )
            shm.buf[slot_offset:slot_offset+64] = bytearray(slot_data)

        # Launch col-telemetryd to drain the 20 frames from POSIX shared memory
        proc = subprocess.run(
            [TELEMETRYD_BIN, "--shm-name", shm_posix_name, "--max-frames", "20", "--timeout-ms", "2000"],
            capture_output=True, text=True, timeout=5
        )

        output = proc.stdout + proc.stderr
        print(f"  [col-telemetryd output summary]:\n" + "\n".join("    " + line for line in output.splitlines()[-6:]))

        assert_true(proc.returncode == 0, "col-telemetryd exits with code 0 after draining 20 frames")
        assert_true("[AIRGAP SECURITY VIOLATION]" in output, "Airgap security violation logged for simulated frames")
        assert_true("[INVALID QUALITY FLAG]" in output, "Invalid quality flag logged for spoofed frames")
        assert_true("[HARDWARE ROOT]" in output, "Hardware root accepted logged for authentic frames")

        # Verify summary stats in output
        assert_true("Hardware Accepted: 5" in output, "Summary confirms exactly 5 hardware accepted")
        assert_true("Simulated Dropped: 10" in output, "Summary confirms exactly 10 simulated dropped")
        assert_true("Invalid Dropped: 5" in output, "Summary confirms exactly 5 invalid dropped")
        assert_true("Total: 20" in output, "Summary confirms total 20 inspected")

    finally:
        shm.close()
        shm.unlink()

def main():
    print("=================================================================")
    print("=== Empirical Challenger: Daemon Lifecycle & Concurrency Suite ===")
    print("=================================================================")

    if not os.path.exists(TELEMETRYD_BIN) or not os.path.exists(COMMONSD_BIN):
        print(f"ERROR: Binaries not found in {BUILD_DIR}", file=sys.stderr)
        return 1

    test_telemetryd_flags()
    test_commonsd_flags()
    test_commonsd_ticks()
    test_telemetryd_timeout()
    test_rapid_signal_cycles()
    test_real_shm_airgap_filtering()

    print("\n=================================================================")
    if failures == 0:
        print(">>> ALL DAEMON ADVERSARIAL TESTS PASSED (0 FAILURES) <<<")
        print("=================================================================")
        return 0
    else:
        print(f">>> DAEMON ADVERSARIAL TESTS FAILED: {failures} failures! <<<", file=sys.stderr)
        print("=================================================================")
        return 1

if __name__ == "__main__":
    sys.exit(main())
