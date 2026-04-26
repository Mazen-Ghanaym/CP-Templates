/* ==============================================================================
 * EXPERIENCE POINT: Kosaraju's Algorithm (DRY Version with Utilities)
 * ==============================================================================
 * THE PROBLEM:
 * Group mutually reachable nodes into Strongly Connected Components (SCCs).
 *
 * THE SOLUTION (Kosaraju in O(V + E) - DRY Implementation):
 * We use a SINGLE generic `dfs` function for both passes.
 * Pass 1: Run DFS on `adj`, output to `order` (gives exit-time ordering).
 * Pass 2: Run DFS on `rev_adj` in reverse `order`, output to `current_scc`.
 *
 * INCLUDED UTILITIES:
 * 1. sccs: A vector of vectors. sccs[i] contains all nodes in the i-th SCC.
 * 2. scc_id[u]: Returns the ID (0-indexed) of the SCC that node 'u' belongs to.
 * 3. build_condensation_graph(): Shrinks each SCC into a single Super-Node. 
 * Safely filters out parallel edges using a set. Resulting graph is a perfect DAG!
 * ============================================================================== */

#include <iostream>
#include <vector>
#include <set>

using namespace std;

struct Kosaraju {
    int n;
    vector<vector<int>> adj, rev_adj;
    vector<bool> visited;
    vector<int> order;
    
    // Results
    vector<vector<int>> sccs;
    vector<int> scc_id; // scc_id[u] = ID of the SCC that contains node 'u'
    int scc_count;
    
    // Condensation Graph Utilities (The DAG)
    vector<vector<int>> condensation_adj;
    vector<int> scc_sizes; // Size of each Super-Node

    // Constructor (1-based indexing for input nodes)
    Kosaraju(int _n) {
        n = _n;
        adj.assign(n + 1, vector<int>());
        rev_adj.assign(n + 1, vector<int>());
        scc_id.assign(n + 1, -1);
        scc_count = 0;
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }

    // =========================================================
    // ONE DFS TO RULE THEM ALL (The CP-Algorithms Trick)
    // =========================================================
    void dfs(int u, const vector<vector<int>>& current_adj, vector<int>& output) {
        visited[u] = true;
        for (int v : current_adj[u]) {
            if (!visited[v]) {
                dfs(v, current_adj, output);
            }
        }
        output.push_back(u); // Works perfectly for both exit-time and SCC collection
    }

    void build_sccs() {
        visited.assign(n + 1, false);
        order.clear();
        
        // Step 1: Fill 'order' using original graph
        for (int i = 1; i <= n; i++) {
            if (!visited[i]) {
                dfs(i, adj, order);
            }
        }

        visited.assign(n + 1, false);
        sccs.clear();
        scc_count = 0;

        // Step 2: Process in reverse exit order on the reversed graph
        for (int i = order.size() - 1; i >= 0; i--) {
            int u = order[i];
            if (!visited[u]) {
                vector<int> current_scc;
                dfs(u, rev_adj, current_scc);
                
                // Assign SCC IDs smoothly after the DFS finishes collecting them
                for (int node : current_scc) {
                    scc_id[node] = scc_count;
                }
                
                sccs.push_back(current_scc);
                scc_count++; // Move to the next Super-Node ID
            }
        }
    }

    // Call this ONLY AFTER calling build_sccs()
    void build_condensation_graph() {
        // The DAG nodes will be 0-indexed (from 0 to scc_count - 1)
        condensation_adj.assign(scc_count, vector<int>());
        scc_sizes.assign(scc_count, 0);

        for (int i = 0; i < scc_count; i++) {
            scc_sizes[i] = sccs[i].size();
        }

        // Use a set to avoid adding multiple parallel edges between the same two SCCs
        set<pair<int, int>> edges_set;

        for (int u = 1; u <= n; u++) {
            for (int v : adj[u]) {
                int root_u = scc_id[u];
                int root_v = scc_id[v];
                
                // If they belong to DIFFERENT SCCs, add a directed edge in the DAG
                if (root_u != root_v) {
                    if (edges_set.find({root_u, root_v}) == edges_set.end()) {
                        edges_set.insert({root_u, root_v});
                        condensation_adj[root_u].push_back(root_v);
                    }
                }
            }
        }
    }
};

/* * ==============================================================================
 * USAGE EXAMPLE IN solve():
 * ==============================================================================
 * int n, m; cin >> n >> m;
 * Kosaraju kosa(n);
 * for(int i = 0; i < m; i++) {
 * int u, v; cin >> u >> v;
 * kosa.add_edge(u, v);
 * }
 * kosa.build_sccs();
 * kosa.build_condensation_graph();
 * * // Now you can run DP on `kosa.condensation_adj` 
 * // knowing it's a perfect DAG with `kosa.scc_count` nodes!
 * ============================================================================== */