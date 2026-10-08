#!/usr/bin/env python3
"""
Empirical Challenger Test Suite for Milestone 1:
POSIX Shared Memory IPC, macOS Darwin Conformance, col_telemetryd Drainage & Signal Handling.
"""

import os
import sys
import re
import time
import signal
import subprocess
import unittest
from pathlib import Path

WORKSPACE_ROOT = Path("/Users/raffaine/dev/collective")
BIN_TELEMETRYD = WORKSPACE_ROOT / "core/1_simulation/src/build_native/col_telemetryd"
BIN_PRODUCER = WORKSPACE_ROOT / "tests/adversarial/adversarial_producer"
BIN_INSPECTOR = WORKSPACE_ROOT / "tests/adversarial/adversarial_inspector"


def ensure_binaries():
    if not BIN_PRODUCER.exists():
        subprocess.run([
            "clang++", "-std=c++20", "-O2", "-Icore/1_simulation/src",
            str(WORKSPACE_ROOT / "tests/adversarial/adversarial_producer.cpp"),
            "-o", str(BIN_PRODUCER)
        ], check=True, cwd=str(WORKSPACE_ROOT))
    if not BIN_INSPECTOR.exists():
        subprocess.run([
            "clang++", "-std=c++20", "-O2", "-Icore/1_simulation/src",
            str(WORKSPACE_ROOT / "tests/adversarial/adversarial_inspector.cpp"),
            "-o", str(BIN_INSPECTOR)
        ], check=True, cwd=str(WORKSPACE_ROOT))


ensure_binaries()


class TestDarwinPosixShmConformance(unittest.TestCase):
    """Verifies POSIX shm_open behavior on macOS Darwin without /dev/shm."""

    def test_no_hardcoded_dev_shm_in_codebase(self):
        """Verify that the codebase does not hardcode /dev/shm in runtime IPC."""
        shm_header = WORKSPACE_ROOT / "core/1_simulation/src/uhai/uhai_ring_buffer.hpp"
        daemon_src = WORKSPACE_ROOT / "src/daemons/col-telemetryd/main.cpp"
        test_src = WORKSPACE_ROOT / "core/1_simulation/src/tests/test_uhai_ring_buffer.cpp"

        for file_path in [shm_header, daemon_src, test_src]:
            text = file_path.read_text(encoding="utf-8")
            self.assertNotIn("/dev/shm", text, f"Hardcoded '/dev/shm' found in {file_path}")

    def test_dev_shm_filesystem_not_present_on_darwin(self):
        """Verify macOS Darwin does not use or rely on /dev/shm mount."""
        if sys.platform == "darwin":
            # On macOS, /dev/shm does not exist as a directory
            self.assertFalse(os.path.isdir("/dev/shm"), "/dev/shm unexpectedly exists on Darwin")

    def test_darwin_posix_shm_creation_and_unlinking(self):
        """Verify POSIX shm creation without /dev/shm via inspector."""
        test_shm = f"/adv_darwin_ok_{os.getpid()}"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        # Producer creates and initializes
        res = subprocess.run([
            str(BIN_PRODUCER),
            "--shm-name", test_shm,
            "--frames", "10",
        ], capture_output=True, text=True)
        self.assertEqual(res.returncode, 0, f"Producer failed: {res.stderr}")
        self.assertIn("accepted=10", res.stdout)

        # Inspector attaches and reads header
        insp = subprocess.run([
            str(BIN_INSPECTOR),
            "--shm-name", test_shm,
        ], capture_output=True, text=True)
        self.assertEqual(insp.returncode, 0, f"Inspector failed: {insp.stderr}")
        self.assertIn("magic=0x55484149", insp.stdout)
        self.assertIn("capacity=1024", insp.stdout)
        self.assertIn("element_size=64", insp.stdout)
        self.assertIn("write_index=10", insp.stdout)

        # Unlink
        unl = subprocess.run([
            str(BIN_INSPECTOR),
            "--shm-name", test_shm,
            "--action", "unlink",
        ], capture_output=True, text=True)
        self.assertEqual(unl.returncode, 0)
        self.assertIn("SUCCESS", unl.stdout)

        # Verify not found after unlink
        insp2 = subprocess.run([
            str(BIN_INSPECTOR),
            "--shm-name", test_shm,
        ], capture_output=True, text=True)
        self.assertEqual(insp2.returncode, 2)
        self.assertIn("NOT_FOUND", insp2.stdout)

    def test_darwin_name_length_limit_and_default_name(self):
        """Verify DEFAULT_SHM_NAME fits in Darwin's 31-char PSHNAMLEN limit."""
        default_name = "/uhai_telemetry"
        self.assertLessEqual(len(default_name), 31, "Default SHM name exceeds Darwin limit")
        self.assertTrue(default_name.startswith("/"), "POSIX SHM name must start with '/'")
        self.assertEqual(default_name.count("/"), 1, "POSIX SHM name must contain only single leading '/'")


class TestMultiProcessTelemetryDrainage(unittest.TestCase):
    """Verifies multi-process loopback drainage via col_telemetryd."""

    def test_concurrent_drainage_normal_flow(self):
        """Run col_telemetryd and producer concurrently in separate processes."""
        test_shm = f"/adv_mp_norm_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_mp_norm_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        with open(log_file, "w") as out_f:
            daemon_proc = subprocess.Popen(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--max-frames", "1000", "--timeout-ms", "4000"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )

            time.sleep(0.05)

            # Start producer pushing 1000 frames
            prod_res = subprocess.run(
                [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "1000", "--delay-us", "50"],
                capture_output=True,
                text=True
            )
            self.assertEqual(prod_res.returncode, 0)
            self.assertIn("accepted=1000", prod_res.stdout)

            daemon_proc.wait(timeout=6.0)

        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(daemon_proc.returncode, 0, f"col_telemetryd failed:\n{logs}")
        self.assertIn("Successfully attached!", logs)
        self.assertIn("Reached target frame count (1000)", logs)
        self.assertIn("Drained 1000 total samples.", logs)
        self.assertIn("Header recorded dropped frames: 0", logs)
        self.assertIn("Clean shutdown complete.", logs)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_reverse_startup_daemon_before_producer(self):
        """Verify col_telemetryd initializes layout if launched before producer."""
        test_shm = f"/adv_daemon_first_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_df_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        with open(log_file, "w") as out_f:
            daemon_proc = subprocess.Popen(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--max-frames", "500", "--timeout-ms", "4000"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )

            # Let daemon start, detect uninitialized SHM, and initialize header
            time.sleep(0.2)

            # Check inspector
            insp = subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm], capture_output=True, text=True)
            self.assertEqual(insp.returncode, 0)
            self.assertIn("magic=0x55484149", insp.stdout)

            # Producer starts afterwards
            prod_res = subprocess.run(
                [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "500"],
                capture_output=True,
                text=True
            )
            self.assertEqual(prod_res.returncode, 0)
            self.assertIn("accepted=500", prod_res.stdout)

            daemon_proc.wait(timeout=5.0)

        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(daemon_proc.returncode, 0)
        self.assertIn("Header magic uninitialized", logs)
        self.assertIn("Drained 500 total samples.", logs)
        self.assertIn("Header recorded dropped frames: 0", logs)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_reverse_startup_producer_before_daemon(self):
        """Verify col_telemetryd drains frames pre-written by producer."""
        test_shm = f"/adv_prod_first_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_pf_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        # Producer writes 300 frames and exits
        prod_res = subprocess.run(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "300"],
            capture_output=True,
            text=True
        )
        self.assertEqual(prod_res.returncode, 0)
        self.assertIn("accepted=300", prod_res.stdout)

        # Daemon attaches and drains all 300 frames
        with open(log_file, "w") as out_f:
            daemon_proc = subprocess.run(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--max-frames", "300", "--timeout-ms", "2000"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )

        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(daemon_proc.returncode, 0)
        self.assertIn("Drained 300 total samples.", logs)
        self.assertIn("Header recorded dropped frames: 0", logs)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_buffer_overrun_and_dropped_frame_accounting(self):
        """Verify backpressure overruns are recorded in header and reported by col_telemetryd."""
        test_shm = f"/adv_overrun_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_overrun_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        # Producer pushes 2500 frames into 1024 capacity buffer without consumer
        prod_res = subprocess.run(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "2500"],
            capture_output=True,
            text=True
        )
        self.assertEqual(prod_res.returncode, 0)
        self.assertIn("accepted=1024", prod_res.stdout)
        self.assertIn("rejected=1476", prod_res.stdout)
        self.assertIn("dropped_in_header=1476", prod_res.stdout)

        # Daemon drains the 1024 accepted frames
        with open(log_file, "w") as out_f:
            daemon_res = subprocess.run(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--max-frames", "1024", "--timeout-ms", "2000"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )

        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(daemon_res.returncode, 0)
        self.assertIn("Drained 1024 total samples.", logs)
        self.assertIn("Header recorded dropped frames: 1476", logs)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_sustained_streaming_with_flow_control(self):
        """Stream 3,000 frames with flow control retry to ensure 100% drainage under high load."""
        test_shm = f"/adv_flow_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_flow_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        with open(log_file, "w") as out_f:
            daemon_proc = subprocess.Popen(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--max-frames", "3000", "--timeout-ms", "10000"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )

            time.sleep(0.05)

            prod_res = subprocess.run(
                [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "3000", "--delay-us", "50", "--retry-on-full"],
                capture_output=True,
                text=True
            )
            self.assertEqual(prod_res.returncode, 0)
            self.assertIn("accepted=3000", prod_res.stdout)

            daemon_proc.wait(timeout=10.0)

        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(daemon_proc.returncode, 0)
        self.assertIn("Reached target frame count (3000)", logs)
        self.assertIn("Drained 3000 total samples.", logs)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_rapid_burst_conservation_of_frames(self):
        """Under high-speed unthrottled burst, verify drained + dropped == total sent (exact conservation)."""
        test_shm = f"/adv_burst_cons_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_burst_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        with open(log_file, "w") as out_f:
            daemon_proc = subprocess.Popen(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--timeout-ms", "4000"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )

            time.sleep(0.05)

            # Unthrottled burst of 10,000 frames (no retry, fast push)
            prod_res = subprocess.run(
                [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "10000", "--delay-us", "10"],
                capture_output=True,
                text=True
            )
            self.assertEqual(prod_res.returncode, 0)

            daemon_proc.wait(timeout=8.0)

        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(daemon_proc.returncode, 0)
        match = re.search(r"Drained (\d+) total samples\. Header recorded dropped frames: (\d+)", logs)
        self.assertIsNotNone(match, f"Drainage summary not found in logs:\n{logs[-500:]}")
        drained = int(match.group(1))
        dropped = int(match.group(2))
        self.assertEqual(drained + dropped, 10000,
                         f"Frame conservation violated! Drained ({drained}) + Dropped ({dropped}) != 10000")
        self.assertGreater(drained, 0)
        self.assertGreaterEqual(dropped, 0)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)


class TestSignalHandlingAndUnlinking(unittest.TestCase):
    """Verifies signal handling (SIGINT/SIGTERM) and unlinking semantics."""

    def test_clean_shutdown_on_sigint(self):
        """Send SIGINT to col_telemetryd and verify clean exit with code 0."""
        test_shm = f"/adv_sigint_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_sigint_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        with open(log_file, "w") as out_f:
            daemon_proc = subprocess.Popen(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--timeout-ms", "10000"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )

            time.sleep(0.15)
            self.assertIsNone(daemon_proc.poll(), "Daemon exited prematurely before SIGINT")

            daemon_proc.send_signal(signal.SIGINT)
            daemon_proc.wait(timeout=2.0)

        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(daemon_proc.returncode, 0, f"Daemon did not exit with code 0 on SIGINT:\n{logs}")
        self.assertIn("Clean shutdown complete.", logs)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_clean_shutdown_on_sigterm(self):
        """Send SIGTERM to col_telemetryd and verify clean exit with code 0."""
        test_shm = f"/adv_sigterm_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_sigterm_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        with open(log_file, "w") as out_f:
            daemon_proc = subprocess.Popen(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--timeout-ms", "10000"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )

            time.sleep(0.15)
            self.assertIsNone(daemon_proc.poll(), "Daemon exited prematurely before SIGTERM")

            daemon_proc.send_signal(signal.SIGTERM)
            daemon_proc.wait(timeout=2.0)

        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(daemon_proc.returncode, 0, f"Daemon did not exit with code 0 on SIGTERM:\n{logs}")
        self.assertIn("Clean shutdown complete.", logs)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

    def test_inactivity_and_startup_timeouts(self):
        """Verify startup and inactivity timeouts exit cleanly with code 0."""
        test_shm = f"/adv_timeout_{os.getpid()}"
        log_file = WORKSPACE_ROOT / f"tests/adversarial/log_to_{os.getpid()}.log"
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)

        # Startup timeout: no frames produced
        t0 = time.time()
        with open(log_file, "w") as out_f:
            res = subprocess.run(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--timeout-ms", "300"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )
        elapsed = time.time() - t0
        logs = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(res.returncode, 0)
        self.assertIn("Startup timeout (300 ms) reached", logs)
        self.assertGreaterEqual(elapsed, 0.28)
        self.assertLess(elapsed, 1.5)

        # Inactivity timeout: 10 frames produced, then idle
        prod_res = subprocess.run(
            [str(BIN_PRODUCER), "--shm-name", test_shm, "--frames", "10"],
            capture_output=True,
            text=True
        )
        self.assertEqual(prod_res.returncode, 0)

        t0 = time.time()
        with open(log_file, "w") as out_f:
            res2 = subprocess.run(
                [str(BIN_TELEMETRYD), "--shm-name", test_shm, "--timeout-ms", "300"],
                stdout=out_f,
                stderr=subprocess.STDOUT,
                text=True
            )
        elapsed = time.time() - t0
        logs2 = log_file.read_text(encoding="utf-8")
        log_file.unlink(missing_ok=True)

        self.assertEqual(res2.returncode, 0)
        self.assertIn("Drained 10 total samples.", logs2)
        self.assertIn("Inactivity timeout (300 ms) reached", logs2)
        self.assertGreaterEqual(elapsed, 0.28)

        # Cleanup
        subprocess.run([str(BIN_INSPECTOR), "--shm-name", test_shm, "--action", "unlink"], capture_output=True)


if __name__ == "__main__":
    unittest.main()
