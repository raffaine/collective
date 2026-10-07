#!/usr/bin/env python3
"""
Verification Suite: EigenTrust Formulation, Sybil Graph Clustering Evasion & Centralization
Pure Python Standard Library implementation (No external dependencies).
Target: SOVEREIGN_STACK_BACKLOG.md (Story 3.2, Story 5.2)
"""

import math
import random

def calculate_local_clustering_coefficient(adj_dict, nodes):
    """
    Computes local clustering coefficient for each node.
    C(i) = 2 * (triangles on i) / (deg(i) * (deg(i) - 1))
    adj_dict: map of node -> set of neighbor nodes
    """
    coeffs = {}
    degrees = {}
    for i in nodes:
        neighbors = adj_dict.get(i, set())
        k = len(neighbors)
        degrees[i] = k
        if k < 2:
            coeffs[i] = 0.0
            continue
        
        # Count edges between neighbors
        triangles = 0
        neighbor_list = list(neighbors)
        for idx1 in range(len(neighbor_list)):
            u = neighbor_list[idx1]
            for idx2 in range(idx1 + 1, len(neighbor_list)):
                v = neighbor_list[idx2]
                if v in adj_dict.get(u, set()):
                    triangles += 1
        
        coeffs[i] = (2.0 * triangles) / (k * (k - 1))
    return coeffs, degrees


def test_sybil_bipartite_evasion():
    print("=== [TEST 1] Sybil Botnet Clustering Coefficient Filter Evasion ===")
    # Story 3.2 states:
    # "To resist Sybil attacks, the engine calculates the local clustering coefficient of peer clusters:
    #  tightly-coupled cliques lacking diversified inbound trust edges from Trust Ring 0 are dampened by > 90%."
    # "Given a rogue cluster of 50 bot nodes that fully cross-sign each other's credentials...
    #  the graph clustering coefficient algorithm flags the dense insular topology"

    n_honest = 50
    n_sybil = 50
    honest_nodes = list(range(n_honest))
    sybil_nodes = list(range(n_honest, n_honest + n_sybil))

    # 1. Topology A: Naive Sybil Clique (K_50 fully connected)
    adj_clique = {i: set() for i in sybil_nodes}
    for i in sybil_nodes:
        for j in sybil_nodes:
            if i != j:
                adj_clique[i].add(j)

    coeffs_clique, _ = calculate_local_clustering_coefficient(adj_clique, sybil_nodes)
    avg_coeff_clique = sum(coeffs_clique.values()) / len(coeffs_clique)
    print(f"Topology A (Naive Sybil Clique): Average Clustering Coeff = {avg_coeff_clique:.4f} (Flagged by filter)")

    # 2. Topology B: Adversarial Bipartite Sybil Botnet (K_25,25)
    # 25 bots in Partition X, 25 bots in Partition Y. All X connect to all Y.
    # Triangle count is strictly ZERO in any bipartite graph!
    adj_bipartite = {i: set() for i in sybil_nodes}
    x_nodes = sybil_nodes[:25]
    y_nodes = sybil_nodes[25:]

    for x in x_nodes:
        for y in y_nodes:
            adj_bipartite[x].add(y)
            adj_bipartite[y].add(x)

    coeffs_bipartite, degs_bipartite = calculate_local_clustering_coefficient(adj_bipartite, sybil_nodes)
    avg_coeff_bipartite = sum(coeffs_bipartite.values()) / len(coeffs_bipartite)
    avg_deg_bipartite = sum(degs_bipartite.values()) / len(degs_bipartite)

    print(f"Topology B (Adversarial Bipartite K_25,25):")
    print(f"  Nodes: {n_sybil}, Total Bot Edges: {25 * 25}")
    print(f"  Average Degree per Bot: {avg_deg_bipartite:.1f}")
    print(f"  Average Clustering Coeff: {avg_coeff_bipartite:.4f} (Expected: 0.0000)")

    evasion_success = (avg_coeff_bipartite == 0.0)
    print(f"Result: Sybil Filter Evasion Confirmed: {evasion_success}")
    print("Analysis: A sophisticated 50-node Sybil botnet simply forms a bipartite graph.")
    print("Because bipartite graphs contain zero triangles, every bot node has Clustering Coefficient = 0.0!")
    print("The backlog's clustering coefficient filter is completely blind to bipartite botnets, failing to dampen them.")
    return evasion_success


def test_eigentrust_rank_sink_trap():
    print("\n=== [TEST 2] Pure Power Iteration Rank-Sink Absorption Vulnerability ===")
    # Story 3.2 equation:
    # t^(k+1) = C^T t^(k)
    # Notice: No pre-trusted restart vector (1 - a) C^T t + a * p!
    
    n_honest = 10
    n_sybil = 5
    n_total = n_honest + n_sybil

    # Build trust matrix C where C[i][j] is normalized trust from i to j
    # Matrix size n_total x n_total
    C = [[0.0] * n_total for _ in range(n_total)]
    
    # Honest nodes trust each other in a ring
    for i in range(n_honest):
        next_h = (i + 1) % n_honest
        C[i][next_h] = 0.9
    
    # Honest node 0 accidentally trusts 1 Sybil node (node 10)
    C[0][n_honest] = 0.1

    # Row normalize honest nodes
    for i in range(n_honest):
        s = sum(C[i])
        if s > 0:
            C[i] = [val / s for val in C[i]]

    # Sybil nodes trust each other in a closed cycle and NEVER trust honest nodes back
    for i in range(n_honest, n_total):
        next_s = n_honest + ((i - n_honest + 1) % n_sybil)
        C[i][next_s] = 1.0

    # 1. Run Story 3.2 pure power iteration: t^(k+1) = C^T t^(k)
    t = [1.0 / n_total] * n_total
    for step in range(200):
        # t_new[j] = sum_i (C[i][j] * t[i])
        t_next = [0.0] * n_total
        for j in range(n_total):
            for i in range(n_total):
                t_next[j] += C[i][j] * t[i]
        
        norm = sum(t_next)
        if norm > 0:
            t = [val / norm for val in t_next]

    sybil_mass_pure = sum(t[n_honest:])
    honest_mass_pure = sum(t[:n_honest])

    print(f"Pure Power Iteration (Story 3.2 spec t^(k+1) = C^T t^(k)):")
    print(f"  Honest Nodes Total Trust: {honest_mass_pure * 100:.2f}%")
    print(f"  Sybil Nodes Total Trust:  {sybil_mass_pure * 100:.2f}%")

    # 2. Run Authentic EigenTrust with pre-trusted restart vector (Kamvar et al.)
    p = [0.0] * n_total
    for i in range(3):
        p[i] = 1.0 / 3.0 # Trust Ring 0 pre-trusted founders
    alpha = 0.15         # Restart probability

    t_auth = [1.0 / n_total] * n_total
    for step in range(200):
        t_next = [0.0] * n_total
        for j in range(n_total):
            sum_c_t = 0.0
            for i in range(n_total):
                sum_c_t += C[i][j] * t_auth[i]
            t_next[j] = (1.0 - alpha) * sum_c_t + alpha * p[j]
        norm = sum(t_next)
        if norm > 0:
            t_auth = [val / norm for val in t_next]

    sybil_mass_auth = sum(t_auth[n_honest:])
    honest_mass_auth = sum(t_auth[:n_honest])

    print(f"\nAuthentic EigenTrust with Pre-Trusted Restart Vector (a=0.15, p in Ring 0):")
    print(f"  Honest Nodes Total Trust: {honest_mass_auth * 100:.2f}%")
    print(f"  Sybil Nodes Total Trust:  {sybil_mass_auth * 100:.2f}%")

    rank_sink_bug = (sybil_mass_pure > 0.99)
    print(f"\nResult: Rank Sink Absorption Bug Confirmed: {rank_sink_bug}")
    print("Analysis: Without the pre-trusted restart term (1-a)*C^T*t + a*p, any absorbing Sybil cluster")
    print("sucks 100.0% of the entire network's trust score into itself under power iteration!")
    return rank_sink_bug


def test_power_iteration_convergence_iterations():
    print("\n=== [TEST 3] Power Iteration Convergence Iterations on 1,024-Node Graph ===")
    # Story 3.2 claim: "the normalized reputation vector converges within 15 iterations across 1,024 nodes"
    n = 1024
    random.seed(42)

    # 4 clusters of 256 nodes with weak inter-cluster bridge edges
    # Representing sparse trust graph: list of (target, weight) per node
    graph = [[] for _ in range(n)]
    
    for i in range(n):
        cluster_id = i // 256
        cluster_start = cluster_id * 256
        cluster_end = cluster_start + 256
        
        # 7 internal cluster edges
        internal_targets = random.sample(range(cluster_start, cluster_end), 7)
        for t in internal_targets:
            if t != i:
                graph[i].append(t)
        # 1 external bridge edge
        ext = random.randint(0, n - 1)
        graph[i].append(ext)

    # Convert to sparse transition probabilities
    sparse_C = []
    for i in range(n):
        targets = graph[i]
        weight = 1.0 / len(targets)
        sparse_C.append([(t, weight) for t in targets])

    p = [0.0] * n
    for i in range(10):
        p[i] = 0.1 # 10 seed nodes in Ring 0
    alpha = 0.15

    t = [1.0 / n] * n
    tolerance = 1e-5
    converged_iter = -1

    for it in range(1, 201):
        t_next = [alpha * p[j] for j in range(n)]
        for i in range(n):
            ti = t[i]
            for (target, w) in sparse_C[i]:
                t_next[target] += (1.0 - alpha) * ti * w

        # L1 norm diff
        diff = sum(abs(t_next[j] - t[j]) for j in range(n))
        t = t_next
        if diff < tolerance:
            converged_iter = it
            break

    print(f"Target Convergence Iterations (Story 3.2): <= 15 iterations")
    print(f"Actual Iterations Required (L1 diff < 1e-5): {converged_iter} iterations")

    exceeded_15 = (converged_iter > 15)
    print(f"Result: 15-Iteration Limit Exceeded Confirmed: {exceeded_15}")
    print(f"Analysis: Across realistic community cluster topologies with bottleneck bridges,")
    print(f"power iteration requires {converged_iter} iterations to converge, falsifying the 15-iteration DoD claim.")
    return exceeded_15


if __name__ == "__main__":
    test_sybil_bipartite_evasion()
    test_eigentrust_rank_sink_trap()
    test_power_iteration_convergence_iterations()
    print("\n=== EIGENTRUST / SYBIL SUITE EXECUTION COMPLETE ===")
