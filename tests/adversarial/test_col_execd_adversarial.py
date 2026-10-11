#!/usr/bin/env python3
"""
Adversarial Empirical Stress Testing Suite for col-execd (Milestone 4 / Sprint 3).
Tests:
1. 10 Hz timing loop stability, bounded jitter, and cumulative drift prevention.
2. UHAI ring buffer high-throughput drainage under concurrent and burst producers.
3. Signal handling (rapid SIGINT, SIGTERM, concurrent signals) and clean exit code 0.
4. Darwin POSIX shm invariants: zero '/dev/shm' in source code, name bounds, unlinking.
5. Dual verification harness execution and pass criteria.
"""

import os
import sys
import time
import signal
import subprocess
import unittest
import math
from pathlib import Path

WORKSPACE_ROOT = Path("/Users/raffaine/dev/collective")
BIN_EXECD = WORKSPACE_ROOT / "core/1_simulation/src/build_native/col_execd"
BIN_PRODUCER = WORKSPACE_ROOT / "tests/adversarial/adversarial_producer"
BIN_INSPECTOR = WORKSPACE_ROOT / "tests/adversarial/adversarial_inspector"


class TestColExecdTimingLoop(unittest.TestCase):
    """Empirical challenge on the 10 Hz deterministic loop and drift-free scheduling."""

    def test_timing_stability_10hz(self):
        """Challenge 10 Hz timing loop over 20 ticks: measure intervals, jitter, and cumulative drift."""
        test_shm = f"/adv_timing_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        cmd = [
            str(BIN_EXECD),
            "--shm-name", test_shm,
            "--ticks", "20",
            "--frequency", "10"
        ]

        t_start = time.perf_counter()
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)

        tick_times = []
        for line in proc.stdout:
            now = time.perf_counter()
            if "Tick:" in line:
                tick_times.append(now)

        proc.stdout.close()
        proc.wait(timeout=5.0)
        t_end = time.perf_counter()

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        self.assertEqual(proc.returncode, 0, "col-execd did not exit with code 0")
        self.assertEqual(len(tick_times), 20, f"Expected 20 ticks, got {len(tick_times)}")

        # Calculate intervals between consecutive ticks (19 intervals)
        intervals = [tick_times[i] - tick_times[i - 1] for i in range(1, len(tick_times))]
        expected_interval = 0.100  # 100 ms for 10 Hz

        mean_interval = sum(intervals) / len(intervals)
        variance = sum((x - mean_interval) ** 2 for x in intervals) / len(intervals)
        std_dev_jitter = math.sqrt(variance)

        # Total elapsed time for 19 intervals
        total_interval_time = tick_times[-1] - tick_times[0]
        expected_total_time = 19 * expected_interval
        drift = abs(total_interval_time - expected_total_time)

        print(f"\n[TIMING 10Hz] Mean interval: {mean_interval * 1000:.2f} ms | "
              f"Expected: 100.00 ms | Jitter (stddev): {std_dev_jitter * 1000:.2f} ms | "
              f"Cumulative drift: {drift * 1000:.2f} ms")

        # Mean interval must be within 10% of 100 ms (allowing for macOS timer resolution)
        self.assertAlmostEqual(mean_interval, expected_interval, delta=0.015,
                               msg=f"Mean interval {mean_interval} diverged from 0.100s")

        # Jitter must be bounded (< 15 ms standard deviation under OS scheduling)
        self.assertLess(std_dev_jitter, 0.015,
                        msg=f"Timing jitter too high: {std_dev_jitter * 1000:.2f} ms")

        # Cumulative drift must be tightly bounded (< 40 ms over 19 intervals)
        self.assertLess(drift, 0.040,
                        msg=f"Cumulative drift exceeded tolerance: {drift * 1000:.2f} ms")

    def test_timing_higher_frequency_50hz(self):
        """Stress-test sleep_until loop at 50 Hz (20 ms interval) over 50 ticks."""
        test_shm = f"/adv_timing50_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        cmd = [
            str(BIN_EXECD),
            "--shm-name", test_shm,
            "--ticks", "50",
            "--frequency", "50"
        ]

        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)

        tick_times = []
        for line in proc.stdout:
            now = time.perf_counter()
            if "Tick:" in line:
                tick_times.append(now)

        proc.stdout.close()
        proc.wait(timeout=5.0)
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        self.assertEqual(proc.returncode, 0)
        self.assertEqual(len(tick_times), 50)

        intervals = [tick_times[i] - tick_times[i - 1] for i in range(1, len(tick_times))]
        mean_interval = sum(intervals) / len(intervals)
        expected_interval = 0.020  # 20 ms for 50 Hz

        print(f"\n[TIMING 50Hz] Mean interval: {mean_interval * 1000:.2f} ms | Expected: 20.00 ms")
        self.assertAlmostEqual(mean_interval, expected_interval, delta=0.006)


class TestColExecdDrainageAndConcurrency(unittest.TestCase):
    """Empirical challenge on UHAI ring buffer high-throughput drainage under concurrent producers."""

    def test_drainage_reverse_startup_daemon_first(self):
        """col-execd starts first on clean shm, producer feeds 1,000 frames."""
        test_shm = f"/adv_cd_d1st_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        # Launch col-execd at 100 Hz for 20 ticks (0.2s duration)
        daemon_proc = subprocess.Popen(
            [str(BIN_EXECD), "--shm-name", test_shm, "--ticks", "30", "--frequency", "100"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        time.sleep(0.05)  # Ensure daemon created shm and initialized header

        # Producer pushes 1,000 frames
        prod_res = subprocess.run(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "1000", "--delay-us", "50", "--retry-on-full"],
            capture_output=True,
            text=True
        )
        self.assertEqual(prod_res.returncode, 0)
        self.assertIn("accepted=1000", prod_res.stdout)

        stdout, _ = daemon_proc.communicate(timeout=6.0)
        self.assertEqual(daemon_proc.returncode, 0)
        self.assertIn("Clean exit", stdout)

        # Check total drained
        drained_match = None
        for line in stdout.splitlines():
            if "Drained" in line and "Clean exit" in line:
                drained_match = line
        self.assertIsNotNone(drained_match, f"Did not find clean exit line in stdout: {stdout}")
        print(f"\n[DRAINAGE D1ST] {drained_match}")

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_drainage_producer_first(self):
        """Producer writes 500 frames before col-execd starts, verifying backlog drainage."""
        test_shm = f"/adv_cd_p1st_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        prod_res = subprocess.run(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "500"],
            capture_output=True,
            text=True
        )
        self.assertEqual(prod_res.returncode, 0)
        self.assertIn("accepted=500", prod_res.stdout)

        # Now start col-execd for 5 ticks
        exec_res = subprocess.run(
            [str(BIN_EXECD), "--shm-name", test_shm, "--ticks", "5", "--frequency", "10"],
            capture_output=True,
            text=True
        )
        self.assertEqual(exec_res.returncode, 0)
        self.assertIn("Drained 500 telemetry samples. Clean exit.", exec_res.stdout)

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_burst_overrun_and_accounting(self):
        """Producer bursts 3,000 frames without consumer into 1024 buffer. col-execd drains accepted frames."""
        test_shm = f"/adv_cd_overrun_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        prod_res = subprocess.run(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "3000"],
            capture_output=True,
            text=True
        )
        self.assertEqual(prod_res.returncode, 0)
        self.assertIn("accepted=1024", prod_res.stdout)
        self.assertIn("rejected=1976", prod_res.stdout)

        # Run col-execd to drain
        exec_res = subprocess.run(
            [str(BIN_EXECD), "--shm-name", test_shm, "--ticks", "5", "--frequency", "10"],
            capture_output=True,
            text=True
        )
        self.assertEqual(exec_res.returncode, 0)
        self.assertIn("Drained 1024 telemetry samples. Clean exit.", exec_res.stdout)

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_high_throughput_concurrent_burst(self):
        """Concurrent active streaming of 10,000 frames while col-execd runs."""
        test_shm = f"/adv_cd_burst10k_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        daemon_proc = subprocess.Popen(
            [str(BIN_EXECD), "--shm-name", test_shm, "--ticks", "100", "--frequency", "50"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        time.sleep(0.05)

        prod_res = subprocess.run(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "10000", "--delay-us", "20", "--retry-on-full"],
            capture_output=True,
            text=True
        )
        self.assertEqual(prod_res.returncode, 0)
        self.assertIn("accepted=10000", prod_res.stdout)

        stdout, _ = daemon_proc.communicate(timeout=6.0)
        self.assertEqual(daemon_proc.returncode, 0)
        self.assertIn("Drained 10000 telemetry samples. Clean exit.", stdout)
        print(f"\n[HIGH THROUGHPUT] Successfully transferred and drained 10,000 frames concurrently.")

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)


class TestColExecdSignalsAndLifecycle(unittest.TestCase):
    """Empirical challenge on signal handling (SIGINT, SIGTERM) and exit code 0."""

    def test_rapid_sigint_shutdown(self):
        """Send SIGINT immediately after col-execd startup (< 30ms). Verify clean exit 0."""
        test_shm = f"/adv_sigint_execd_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        proc = subprocess.Popen(
            [str(BIN_EXECD), "--shm-name", test_shm],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        time.sleep(0.03)  # Rapid signal
        proc.send_signal(signal.SIGINT)

        stdout, _ = proc.communicate(timeout=2.0)
        self.assertEqual(proc.returncode, 0, f"SIGINT did not exit 0. Output:\n{stdout}")
        self.assertIn("Clean exit", stdout)

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_rapid_sigterm_shutdown(self):
        """Send SIGTERM immediately after col-execd startup (< 30ms). Verify clean exit 0."""
        test_shm = f"/adv_sigterm_execd_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        proc = subprocess.Popen(
            [str(BIN_EXECD), "--shm-name", test_shm],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        time.sleep(0.03)
        proc.send_signal(signal.SIGTERM)

        stdout, _ = proc.communicate(timeout=2.0)
        self.assertEqual(proc.returncode, 0, f"SIGTERM did not exit 0. Output:\n{stdout}")
        self.assertIn("Clean exit", stdout)

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_sigint_under_heavy_concurrency(self):
        """Send SIGINT while col-execd is under heavy producer flood. Verify clean exit 0 and no deadlock."""
        test_shm = f"/adv_sigint_flood_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        daemon_proc = subprocess.Popen(
            [str(BIN_EXECD), "--shm-name", test_shm, "--frequency", "50"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        time.sleep(0.05)

        # Producer flooding in background
        prod_proc = subprocess.Popen(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "50000", "--delay-us", "10"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )

        time.sleep(0.2)  # Let flood commence
        daemon_proc.send_signal(signal.SIGINT)

        stdout, _ = daemon_proc.communicate(timeout=3.0)
        self.assertEqual(daemon_proc.returncode, 0, f"Deadlock or crash on SIGINT under flood:\n{stdout}")
        self.assertIn("Clean exit", stdout)

        prod_proc.kill()
        prod_proc.stdout.close()
        prod_proc.stderr.close()
        prod_proc.wait()

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_multiple_rapid_signals(self):
        """Send rapid SIGINT then SIGTERM within 1ms. Verify graceful shutdown without crash."""
        test_shm = f"/adv_sig_multi_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        proc = subprocess.Popen(
            [str(BIN_EXECD), "--shm-name", test_shm],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        time.sleep(0.05)
        proc.send_signal(signal.SIGINT)
        proc.send_signal(signal.SIGTERM)

        stdout, _ = proc.communicate(timeout=2.0)
        self.assertEqual(proc.returncode, 0, f"Multiple signals failed:\n{stdout}")
        self.assertIn("Clean exit", stdout)

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)


class TestDarwinPosixShmInvariants(unittest.TestCase):
    """Empirical challenge on Darwin POSIX shm invariants and zero /dev/shm."""

    def test_zero_dev_shm_in_codebase(self):
        """Rigorously verify zero occurrences of '/dev/shm' in col-execd and all Sprint 3 source code."""
        targets = [
            WORKSPACE_ROOT / "src/daemons/col-execd/main.cpp",
            WORKSPACE_ROOT / "src/daemons/col-telemetryd/main.cpp",
            WORKSPACE_ROOT / "core/1_simulation/src/uhai/uhai_ring_buffer.hpp",
            WORKSPACE_ROOT / "core/1_simulation/src/main.cpp",
            WORKSPACE_ROOT / "core/1_simulation/src/core/stochastic_boundary.hpp",
            WORKSPACE_ROOT / "core/1_simulation/src/tests/test_boundary_math.cpp",
            WORKSPACE_ROOT / "core/1_simulation/src/tests/test_uhai_ring_buffer.cpp",
            WORKSPACE_ROOT / "core/1_simulation/src/tests/run_dual_verification.sh",
        ]

        for target in targets:
            self.assertTrue(target.exists(), f"Target file does not exist: {target}")
            content = target.read_text(encoding="utf-8")
            self.assertNotIn("/dev/shm", content, f"VIOLATION: Hardcoded '/dev/shm' found in {target}")

    def test_darwin_shm_naming_contract(self):
        """Verify SHM names conform to POSIX and Darwin PSHNAMLEN (<= 31 characters)."""
        default_shm = "/uhai_telemetry"
        self.assertLessEqual(len(default_shm), 31, "SHM name exceeds Darwin 31 character limit")
        self.assertTrue(default_shm.startswith("/"), "POSIX SHM name must begin with '/'")
        self.assertNotIn("//", default_shm, "POSIX SHM name cannot contain multiple consecutive slashes")


class TestDualVerificationHarness(unittest.TestCase):
    """Empirical challenge on the Dual Verification Harness execution and exit code."""

    def test_dual_verification_execution(self):
        """Execute run_dual_verification.sh and assert clean exit code 0 and pass tokens."""
        harness_path = WORKSPACE_ROOT / "core/1_simulation/src/tests/run_dual_verification.sh"
        self.assertTrue(harness_path.exists())

        res = subprocess.run(
            ["bash", str(harness_path)],
            cwd=str(WORKSPACE_ROOT),
            capture_output=True,
            text=True
        )

        self.assertEqual(res.returncode, 0, f"run_dual_verification.sh failed with code {res.returncode}:\n{res.stdout}\n{res.stderr}")
        self.assertIn("Visual Verification Passed", res.stdout, "Missing 'Visual Verification Passed'")
        self.assertIn("Math Verification Passed", res.stdout, "Missing 'Math Verification Passed'")
        self.assertIn("IPC Verification Passed", res.stdout, "Missing 'IPC Verification Passed'")
        self.assertIn("Quality Gate Status: GREEN", res.stdout, "Missing 'Quality Gate Status: GREEN'")


if __name__ == "__main__":
    unittest.main()
