#!/usr/bin/env python3
"""
Verification Suite: CRDT Operational Synchronization, Kulkarni-Demir HLC & LWW Convergence
Target: SOVEREIGN_STACK_BACKLOG.md (Story 3.1, Story 5.2, Story 5.3)
"""

import struct
import time
import random

class HLC:
    """Kulkarni-Demir Hybrid Logical Clock implementation."""
    def __init__(self, node_id, initial_pt_ms=0):
        self.node_id = node_id
        self.l = initial_pt_ms # physical time component
        self.c = 0             # logical counter component

    def now_64(self):
        # Pack into uint64: 48 bits physical ms, 16 bits counter
        return ((self.l & 0xFFFFFFFFFFFF) << 16) | (self.c & 0xFFFF)

    def unpack(self, packed_64):
        l = (packed_64 >> 16) & 0xFFFFFFFFFFFF
        c = packed_64 & 0xFFFF
        return l, c

    def tick(self, pt_ms):
        # Send or local event
        l_old = self.l
        self.l = max(l_old, pt_ms)
        if self.l == l_old:
            self.c += 1
        else:
            self.c = 0
        return self.now_64()

    def update(self, msg_l, msg_c, pt_ms):
        # Receive event
        l_old = self.l
        self.l = max(l_old, msg_l, pt_ms)
        if self.l == l_old and self.l == msg_l:
            self.c = max(self.c, msg_c) + 1
        elif self.l == l_old:
            self.c += 1
        elif self.l == msg_l:
            self.c = msg_c + 1
        else:
            self.c = 0
        return self.now_64()


class VoxelMutationOp:
    # 24-byte struct as defined in section 2.2:
    # uint64_t logical_clock;     // 8 bytes
    # uint32_t voxel_index;       // 4 bytes
    # Voxel    old_value;         // 4 bytes: uint8 material_id, moisture, temperature, metadata
    # Voxel    new_value;         // 4 bytes
    # uint16_t author_entity_id;  // 2 bytes
    # uint16_t signature_slot;    // 2 bytes
    FORMAT = "<QIBBBB B BBBBH H" # Little endian packing, exactly 24 bytes

    def __init__(self, logical_clock, voxel_index, old_v, new_v, author_id, sig_slot=0):
        self.logical_clock = logical_clock
        self.voxel_index = voxel_index
        self.old_v = old_v # (mat, moist, temp, meta)
        self.new_v = new_v
        self.author_id = author_id
        self.sig_slot = sig_slot

    def pack(self):
        return struct.pack(
            "<QI4B4BHH",
            self.logical_clock,
            self.voxel_index,
            *self.old_v,
            *self.new_v,
            self.author_id,
            self.sig_slot
        )

    @classmethod
    def unpack(cls, data):
        fields = struct.unpack("<QI4B4BHH", data)
        hlc = fields[0]
        v_idx = fields[1]
        old_v = fields[2:6]
        new_v = fields[6:10]
        author = fields[10]
        sig = fields[11]
        return cls(hlc, v_idx, old_v, new_v, author, sig)


def test_struct_packing_and_size():
    print("=== [TEST 1] VoxelMutationOp 24-Byte Memory Layout & Static Assert ===")
    sample_op = VoxelMutationOp(
        logical_clock=0x123456789ABCDEF0,
        voxel_index=45000,
        old_v=(0, 0, 128, 0),
        new_v=(1, 50, 140, 1),
        author_id=42,
        sig_slot=7
    )
    packed_bytes = sample_op.pack()
    size = len(packed_bytes)
    print(f"Packed struct size: {size} bytes (Expected: 24 bytes)")
    assert size == 24, f"Struct size is {size}, expected 24 bytes!"
    unpacked_op = VoxelMutationOp.unpack(packed_bytes)
    assert unpacked_op.logical_clock == sample_op.logical_clock
    assert unpacked_op.voxel_index == sample_op.voxel_index
    assert unpacked_op.old_v == sample_op.old_v
    assert unpacked_op.new_v == sample_op.new_v
    print("Static assert verified: VoxelMutationOp is exactly 24 bytes and roundtrips perfectly.")
    return True


def test_story_3_1_rollback_divergence_bug():
    print("\n=== [TEST 2] Story 3.1 Rollback Specification Divergence Bug ===")
    print("Testing the verbatim scenario from Story 3.1 lines 823-828:")
    print(" 'When a remote mutation arrives with an earlier HLC timestamp from an authorized peer,")
    print("  Then the engine applies the 24-byte inverse log (old_value) to roll back the local voxel")
    print("  And commits the remote mutation'")
    
    # Initial state: Voxel at index 100 has Material 0 (Air)
    initial_voxel = (0, 0, 128, 0) # Air

    # Node A generates mutation A at tick 120 (HLC = (120, 0)): Material 1 (Asphalt)
    hlc_A = ((120 & 0xFFFFFFFFFFFF) << 16) | 0
    op_A = VoxelMutationOp(hlc_A, 100, initial_voxel, (1, 0, 128, 0), author_id=1)

    # Node B generates mutation B at tick 100 (HLC = (100, 0)): Material 2 (Clay)
    hlc_B = ((100 & 0xFFFFFFFFFFFF) << 16) | 0
    op_B = VoxelMutationOp(hlc_B, 100, initial_voxel, (2, 0, 128, 0), author_id=2)

    # 1. Local execution
    # Node A applies op_A optimistically:
    node_A_voxel = op_A.new_v # Material 1 (Asphalt)
    node_A_last_hlc = op_A.logical_clock

    # Node B applies op_B optimistically:
    node_B_voxel = op_B.new_v # Material 2 (Clay)
    node_B_last_hlc = op_B.logical_clock

    print(f"Initial Local State -> Node A voxel: {node_A_voxel[0]} (HLC={node_A_last_hlc >> 16}), Node B voxel: {node_B_voxel[0]} (HLC={node_B_last_hlc >> 16})")

    # 2. Exchange across mesh:
    # Node B receives op_A (HLC 120 > local HLC 100):
    # Under Last-Write-Wins: op_A is newer than op_B, so Node B adopts op_A.
    node_B_voxel = op_A.new_v # Material 1
    node_B_last_hlc = op_A.logical_clock
    print(f"Node B receives op_A (newer): Adopts op_A -> Voxel is Material {node_B_voxel[0]}")

    # Node A receives op_B (HLC 100 < local HLC 120):
    # NOW FOLLOW STORY 3.1 VERBATIM SPEC:
    # "When a remote mutation arrives with an earlier HLC timestamp from an authorized peer,
    #  Then the engine applies the 24-byte inverse log (old_value) to roll back the local voxel
    #  And commits the remote mutation"
    print("Node A executes Story 3.1 rollback specification:")
    # Roll back local voxel using old_value:
    node_A_voxel = op_A.old_v # rolled back to initial_voxel (Air)
    # Commit remote mutation op_B:
    node_A_voxel = op_B.new_v # Material 2
    node_A_last_hlc = op_B.logical_clock
    print(f"Node A applied rollback and committed op_B (older) -> Voxel is Material {node_A_voxel[0]}")

    print("\n--- FINAL CONVERGENCE CHECK ---")
    print(f"Node A final voxel material: {node_A_voxel[0]} (Clay)")
    print(f"Node B final voxel material: {node_B_voxel[0]} (Asphalt)")

    diverged = (node_A_voxel[0] != node_B_voxel[0])
    print(f"Divergence detected: {diverged}")
    if diverged:
        print("CRITICAL SPECIFICATION FLAW: Story 3.1 describes rolling back newer edits when an")
        print("earlier edit arrives, directly violating Last-Write-Wins (LWW) and producing permanent state divergence!")
        print("Correct LWW behavior: If remote HLC < local HLC, the remote write must be DROPPED/SUPERSEDED, NOT committed!")
    return diverged


def test_cascading_concurrent_mutations():
    print("\n=== [TEST 3] Empirical 1,000 Concurrent Out-of-Order Voxel Mutations ===")
    # Simulate 5 nodes concurrently editing the same lot of 1,000 voxels with simulated delays
    num_nodes = 5
    num_voxels = 1000
    ops_per_node = 200 # Total 1,000 ops

    # Ground truth LWW store per node: map voxel_idx -> (hlc, author_id, voxel_state)
    stores = [{v: (0, 0, (0, 0, 128, 0)) for v in range(num_voxels)} for _ in range(num_nodes)]
    hlcs = [HLC(node_id=i, initial_pt_ms=1000) for i in range(num_nodes)]

    # Generate operations
    all_ops = []
    for node_idx in range(num_nodes):
        for _ in range(ops_per_node):
            v_idx = random.randint(0, num_voxels - 1)
            # simulate time tick
            pt = 1000 + random.randint(0, 100)
            clock = hlcs[node_idx].tick(pt)
            mat = random.randint(1, 7)
            new_v = (mat, 100, 130, 0)
            old_v = stores[node_idx][v_idx][2]
            op = VoxelMutationOp(clock, v_idx, old_v, new_v, author_id=node_idx)
            # Local apply (correct LWW rule)
            stores[node_idx][v_idx] = (clock, node_idx, new_v)
            all_ops.append(op)

    # Shuffle all operations to simulate arbitrary out-of-order arrival across mesh
    for node_idx in range(num_nodes):
        shuffled = list(all_ops)
        random.shuffle(shuffled)
        for op in shuffled:
            v_idx = op.voxel_index
            curr_hlc, curr_author, curr_v = stores[node_idx][v_idx]
            
            # Pure deterministic LWW rule: higher HLC wins; if equal HLC, higher author_id wins
            if (op.logical_clock > curr_hlc) or (op.logical_clock == curr_hlc and op.author_id > curr_author):
                stores[node_idx][v_idx] = (op.logical_clock, op.author_id, op.new_v)

    # Check if all 5 nodes converged to bit-exact identical state
    base_store = stores[0]
    all_match = True
    for node_idx in range(1, num_nodes):
        for v_idx in range(num_voxels):
            if stores[node_idx][v_idx] != base_store[v_idx]:
                all_match = False
                break

    print(f"All 5 nodes converged to 100% identical state under pure LWW: {all_match}")
    print("Conclusion: Pure LWW converges, BUT it requires discarding earlier ops and NEVER applying")
    print("the inverted rollback specified in Story 3.1!")
    return all_match


def test_hlc_counter_overflow():
    print("\n=== [TEST 4] Kulkarni-Demir HLC 16-Bit Logical Counter Burst Overflow ===")
    # If 48 bits physical time + 16 bits counter:
    # Max counter value is 2^16 - 1 = 65,535
    hlc = HLC(node_id=1, initial_pt_ms=5000)
    
    # Burst of 70,000 events within the exact same millisecond
    pt = 5000
    overflowed = False
    for i in range(70000):
        packed = hlc.tick(pt)
        l, c = hlc.unpack(packed)
        if c == 0 and i > 0:
            print(f"Counter wrapped around at iteration {i} (16-bit overflow)!")
            overflowed = True
            break

    print(f"Result: 16-bit Counter Overflow on burst confirmed: {overflowed}")
    print("Analysis: If a batch import (e.g. sneakernet token with 10,000 deltas or high-speed automation)")
    print("ticks within < 1 ms, a 16-bit counter wraps around, violating causal ordering monotonicity!")
    return overflowed


if __name__ == "__main__":
    test_struct_packing_and_size()
    t2 = test_story_3_1_rollback_divergence_bug()
    t3 = test_cascading_concurrent_mutations()
    t4 = test_hlc_counter_overflow()
    print("\n=== CRDT / HLC SUITE EXECUTION COMPLETE ===")
