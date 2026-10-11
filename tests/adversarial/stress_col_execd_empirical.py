#!/usr/bin/env python3
"""
Deep Stress Harness for col-execd:
1. Long-run 100-tick drift analysis (10.0 seconds).
2. CPU contention / throttle resilience.
3. Rapid signal spamming during sleep_until.
4. Concurrent multi-producer memory safety stress.
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


class DeepStressColExecd(unittest.TestCase):

    def test_long_run_100_ticks_drift_bounds(self):
        """Run col-execd for 100 ticks (10 seconds nominal) and analyze cumulative drift."""
        test_shm = f"/adv_deep_100t_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        cmd = [
            str(BIN_EXECD),
            "--shm-name", test_shm,
            "--ticks", "100",
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
        proc.wait(timeout=15.0)
        t_end = time.perf_counter()

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        self.assertEqual(proc.returncode, 0)
        # col-execd logs every 10 ticks when max_ticks > 50 and buffer is empty
        self.assertEqual(len(tick_times), 10)

        # Each interval between log lines represents 10 ticks (1.000 second at 10 Hz)
        intervals = [tick_times[i] - tick_times[i - 1] for i in range(1, len(tick_times))]
        mean_interval = sum(intervals) / len(intervals)
        variance = sum((x - mean_interval) ** 2 for x in intervals) / len(intervals)
        std_dev_jitter = math.sqrt(variance)

        # Total duration from tick 10 to tick 100 (9 intervals of 10 ticks = 90 ticks = 9.000s)
        total_interval_span = tick_times[-1] - tick_times[0]
        expected_span = 9.000  # 9.000 seconds
        cumulative_drift = abs(total_interval_span - expected_span)

        print(f"\n[100-TICK STRESS] Span (tick 10 to 100): {total_interval_span:.4f}s | "
              f"Expected: {expected_span:.4f}s | "
              f"Cumulative drift over 90 ticks: {cumulative_drift * 1000:.2f} ms | "
              f"Mean 10-tick interval: {mean_interval:.4f}s | "
              f"Jitter stddev: {std_dev_jitter * 1000:.2f} ms")

        # Over 90 ticks (9 seconds), cumulative drift MUST be bounded under 50 ms
        self.assertLess(cumulative_drift, 0.050,
                        f"Cumulative drift over 90 ticks ({cumulative_drift*1000:.2f}ms) exceeded 50ms!")
        # Mean 10-tick interval must be 1.000s +/- 0.020s
        self.assertAlmostEqual(mean_interval, 1.000, delta=0.020)

    def test_rapid_signal_spam_during_sleep(self):
        """Send rapid burst of 5 SIGINT signals to col-execd during sleep. Verify immediate clean exit."""
        test_shm = f"/adv_deep_sigspam_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        proc = subprocess.Popen(
            [str(BIN_EXECD), "--shm-name", test_shm, "--frequency", "10"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        time.sleep(0.08)  # Daemon is likely in sleep_until

        t_sig = time.perf_counter()
        for _ in range(5):
            proc.send_signal(signal.SIGINT)
            time.sleep(0.005)

        stdout, _ = proc.communicate(timeout=2.0)
        t_exit = time.perf_counter()

        elapsed_after_sig = t_exit - t_sig
        print(f"\n[SIGNAL SPAM] Exit after signal burst: {elapsed_after_sig * 1000:.2f} ms")

        self.assertEqual(proc.returncode, 0)
        self.assertIn("Clean exit", stdout)
        # Should exit quickly (< 500 ms)
        self.assertLess(elapsed_after_sig, 0.500)

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_concurrent_dual_producers_stress(self):
        """Run 2 concurrent producers pushing 5,000 frames each into same shm while col-execd drains at 100 Hz."""
        test_shm = f"/adv_deep_dualprod_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        daemon_proc = subprocess.Popen(
            [str(BIN_EXECD), "--shm-name", test_shm, "--ticks", "60", "--frequency", "100"],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        time.sleep(0.05)

        # Launch 2 producers simultaneously
        p1 = subprocess.Popen(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "5000", "--delay-us", "10"],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
        )
        p2 = subprocess.Popen(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "5000", "--delay-us", "10"],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
        )

        p1_out, _ = p1.communicate(timeout=5.0)
        p2_out, _ = p2.communicate(timeout=5.0)
        daemon_out, _ = daemon_proc.communicate(timeout=5.0)

        self.assertEqual(p1.returncode, 0)
        self.assertEqual(p2.returncode, 0)
        self.assertEqual(daemon_proc.returncode, 0)
        self.assertIn("Clean exit", daemon_out)

        print(f"\n[DUAL PRODUCERS] p1: {p1_out.strip()} | p2: {p2_out.strip()}")
        print(f"[DUAL PRODUCERS] Daemon: {[l for l in daemon_out.splitlines() if 'Clean exit' in l]}")

        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)


if __name__ == "__main__":
    unittest.main()
