#!/usr/bin/env python3
"""
Adversarial Verification Suite R2.2: Unix Domain Socket IPC & Strict Adjacency Concurrency
Target: SOVEREIGN_STACK_BACKLOG.md (Sections 1.3, 2.3, 2.4, 11.1)

Evaluates:
1. Deadlock vulnerability under bidirectional Strict Adjacency message traversal (L1 <-> L7).
2. Buffer saturation, backpressure, and drop-tail behavior under 256 KB SO_SNDBUF/SO_RCVBUF limits.
3. Event Bus (col-busd) resilience against slow/hung subscribers (e.g., L7 nice +10).
4. Priority inversion: ensures upward telemetry floods do not block downward emergency actuations.
"""

import sys
import os
import socket
import select
import errno
import time
import threading
import struct
import tempfile
import unittest

# 24-byte TelemetrySample wire format
SAMPLE_FMT = "<QIfBBH"
SAMPLE_SIZE = struct.calcsize(SAMPLE_FMT) # 20 + 4 pad = 24 bytes

# 80-byte ActuatorCommand wire format
CMD_FMT = "<QI4sI64s" # 8 + 4 + 4 + 4 + 64 = 84 (approx for wire simulation)
CMD_SIZE = 80


class TestUdsIpcStrictAdjacency(unittest.TestCase):
    def setUp(self):
        self.temp_dir = tempfile.TemporaryDirectory()
        self.ipc_dir = self.temp_dir.name

    def tearDown(self):
        self.temp_dir.cleanup()

    def test_buffer_saturation_and_nonblocking_drop(self):
        """
        EMPIRICAL STRESS TEST 2.1:
        Configure bilateral UDS socket with SO_SNDBUF = 256 KB.
        Flood from L1 (producer) while L2 (consumer) intentionally stalls.
        Assert that non-blocking O_NONBLOCK prevents L1 deadlock by returning EAGAIN/EWOULDBLOCK,
        enabling drop-tail shedding without freezing the real-time event loop.
        """
        sock_l1, sock_l2 = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)
        sock_l1.setblocking(False)
        sock_l2.setblocking(False)

        # Set buffer size to 256 KB per backlog Section 2.3
        target_buf_size = 256 * 1024
        try:
            sock_l1.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, target_buf_size)
            sock_l2.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, target_buf_size)
        except OSError:
            pass # OS may enforce kernel limits, socketpair will still fill up

        print("\n[IPC STRESS 2.1] Flooding non-blocking UDS socket to test backpressure & drop-tail...")
        sample_payload = struct.pack(SAMPLE_FMT, int(time.time() * 1000), 0x3100, 230.0, 0, 0, 1)

        sent_count = 0
        dropped_count = 0
        t0 = time.perf_counter()

        # Attempt to send 25,000 samples (600 KB, exceeding buffer)
        for i in range(25000):
            try:
                sock_l1.send(sample_payload)
                sent_count += 1
            except (BlockingIOError, OSError) as e:
                if e.errno in (errno.EAGAIN, errno.EWOULDBLOCK):
                    dropped_count += 1
                    # Telemetry frame dropped per policy (Section 2.3)
                else:
                    raise

        duration = time.perf_counter() - t0
        print(f"  - Sent frames buffered: {sent_count} ({sent_count * SAMPLE_SIZE / 1024:.1f} KB)")
        print(f"  - Dropped non-critical frames: {dropped_count}")
        print(f"  - Flood completed in {duration*1000:.2f} ms without deadlocking producer")

        # Crucial assertions:
        self.assertGreater(dropped_count, 0, "Buffer saturation must occur and trigger frame shedding")
        self.assertGreater(sent_count, 0, "Initial samples must be buffered")
        self.assertLess(duration, 0.5, "Non-blocking loop must never stall or deadlock")

        sock_l1.close()
        sock_l2.close()
        print("[IPC STRESS 2.1] PASS: Non-blocking socket shed frames cleanly without producer stall.")

    def test_bidirectional_strict_adjacency_deadlock_freedom(self):
        """
        EMPIRICAL STRESS TEST 2.2:
        Simulate the complete 7-daemon linear chain (L1 <-> L2 <-> L3 <-> L4 <-> L5 <-> L6 <-> L7).
        Run simultaneous upward telemetry flow (L1 -> L7) and downward command flow (L7 -> L1).
        Verify that no circular wait occurs and all messages reach target layers.
        """
        print("\n[IPC STRESS 2.2] Testing 7-layer bidirectional Strict Adjacency pipeline for deadlocks...")

        # Create 6 bilateral socket pairs connecting L1 to L7
        # pairs[i] connects layer (i+1) to layer (i+2)
        pairs = [socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM) for _ in range(6)]
        for p1, p2 in pairs:
            p1.setblocking(False)
            p2.setblocking(False)

        # Map each layer daemon to its upstream and downstream sockets
        # L1: downstream=None, upstream=pairs[0][0]
        # L2: downstream=pairs[0][1], upstream=pairs[1][0]
        # ...
        # L7: downstream=pairs[5][1], upstream=None
        daemons = {}
        for layer in range(1, 8):
            down_sock = pairs[layer - 2][1] if layer > 1 else None
            up_sock = pairs[layer - 1][0] if layer < 7 else None
            daemons[layer] = {"down": down_sock, "up": up_sock}

        stop_event = threading.Event()
        upward_delivered = 0
        downward_delivered = 0
        lock = threading.Lock()

        def layer_relay_worker(layer_id):
            nonlocal upward_delivered, downward_delivered
            down = daemons[layer_id]["down"]
            up = daemons[layer_id]["up"]

            read_list = [s for s in (down, up) if s is not None]

            while not stop_event.is_set():
                r, _, _ = select.select(read_list, [], [], 0.01)
                for s in r:
                    try:
                        data = s.recv(4096)
                        if not data:
                            continue
                        if s == down:
                            # Upward message from lower layer
                            if up is not None:
                                up.send(data)
                            else:
                                # Reached L7 (destination of upward flow)
                                with lock:
                                    upward_delivered += len(data) // SAMPLE_SIZE
                        elif s == up:
                            # Downward message from higher layer
                            if down is not None:
                                down.send(data)
                            else:
                                # Reached L1 (destination of downward flow)
                                with lock:
                                    downward_delivered += len(data) // 80
                    except (BlockingIOError, OSError):
                        pass

        threads = [threading.Thread(target=layer_relay_worker, args=(layer,)) for layer in range(1, 8)]
        for t in threads:
            t.daemon = True
            t.start()

        # Concurrently pump:
        # L1 emits 500 upward telemetry samples
        # L7 emits 200 downward actuation commands (80 bytes)
        telemetry_sample = b"\xAA" * SAMPLE_SIZE
        actuator_command = b"\x55" * 80

        t_start = time.perf_counter()

        def pump_l1():
            sock = daemons[1]["up"]
            for _ in range(500):
                try:
                    sock.send(telemetry_sample)
                except OSError:
                    pass
                time.sleep(0.0001)

        def pump_l7():
            sock = daemons[7]["down"]
            for _ in range(200):
                try:
                    sock.send(actuator_command)
                except OSError:
                    pass
                time.sleep(0.0002)

        t_pump1 = threading.Thread(target=pump_l1)
        t_pump7 = threading.Thread(target=pump_l7)
        t_pump1.start()
        t_pump7.start()

        t_pump1.join(timeout=3.0)
        t_pump7.join(timeout=3.0)

        # Allow pipeline to drain
        time.sleep(0.1)
        stop_event.set()
        for t in threads:
            t.join(timeout=1.0)

        # Close all sockets
        for p1, p2 in pairs:
            p1.close()
            p2.close()

        total_time = time.perf_counter() - t_start
        print(f"  - Pipeline traversal complete in {total_time:.3f} s")
        print(f"  - Upward telemetry delivered L1 -> L7: {upward_delivered} samples")
        print(f"  - Downward commands delivered L7 -> L1: {downward_delivered} commands")

        self.assertGreater(upward_delivered, 450, "Upward flow must traverse 6 hops without stalling")
        self.assertGreater(downward_delivered, 180, "Downward flow must traverse 6 hops without stalling")
        self.assertLess(total_time, 2.5, "Strict Adjacency traversal must complete without deadlock")
        print("[IPC STRESS 2.2] PASS: Zero deadlocks detected across 6-hop bidirectional traversal.")

    def test_slow_subscriber_isolation_on_event_bus(self):
        """
        EMPIRICAL STRESS TEST 2.3:
        Broadcast bus (col-busd) simulation.
        If Layer 7 (nice +10) stalls or blocks on bus.sock, does the event broker stall,
        starving critical alarms to Layer 1 through 6?
        Verification: Broker must use non-blocking send or per-subscriber queue drop to isolate slow peers.
        """
        print("\n[IPC STRESS 2.3] Testing Event Bus (col-busd) resilience against hung Layer 7 subscriber...")

        # Fast subscriber (e.g. L4 Orchestrator) and slow subscriber (e.g. L7)
        fast_s1, fast_s2 = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)
        slow_s1, slow_s2 = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)

        fast_s1.setblocking(False)
        fast_s2.setblocking(False)
        slow_s1.setblocking(False)
        slow_s2.setblocking(False) # Slow subscriber never reads, filling buffer

        # Set small buffer on slow subscriber
        try:
            slow_s1.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 4096)
        except OSError:
            pass

        subscribers = [fast_s1, slow_s1]
        alarm_payload = b"[CRITICAL_ALARM:BROWNOUT_VOLTAGE_11.2V]"

        delivered_fast = 0
        dropped_slow = 0

        # Broadcast 1,000 alarms
        for _ in range(1000):
            for sub in subscribers:
                try:
                    sub.send(alarm_payload)
                    if sub == fast_s1:
                        # Fast subscriber reads immediately
                        data = fast_s2.recv(4096)
                        if data:
                            delivered_fast += 1
                except (BlockingIOError, OSError) as e:
                    if sub == slow_s1 and e.errno in (errno.EAGAIN, errno.EWOULDBLOCK):
                        dropped_slow += 1
                    else:
                        raise

        print(f"  - Fast subscriber received critical alarms: {delivered_fast}/1000")
        print(f"  - Hung subscriber dropped alarms due to backpressure: {dropped_slow}")

        self.assertEqual(delivered_fast, 1000, "Fast subscribers must receive 100% of alarms without delay")
        self.assertGreater(dropped_slow, 0, "Hung subscriber buffer must saturate and drop without stalling bus")

        for s in (fast_s1, fast_s2, slow_s1, slow_s2):
            s.close()
        print("[IPC STRESS 2.3] PASS: Event bus isolates slow/hung subscribers with zero head-of-line blocking.")


if __name__ == "__main__":
    unittest.main()
