#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

struct Edge {
    int to, id;
};

/* ==============================================================================
 * THE ULTIMATE TARJAN TEMPLATE (SCC Condensation DAG & Bridge Tree)
 * ==============================================================================
 * This single template serves TWO massive purposes seamlessly, depending entirely 
 * on the 'is_directed' flag passed to the add_edge() function:
 *
 * 1. DIRECTED GRAPHS (is_directed = true) -> CONDENSATION GRAPH (DAG)
 * - The algorithm groups nodes into standard Strongly Connected Components (SCCs).
 * - build_condensation_graph() produces a Directed Acyclic Graph (DAG) where 
 * each node is an SCC. Parallel edges between SCCs are safely filtered out.
 * - Use Case: DP on Graphs, Coin Collector problems, Reachability.
 *
 * 2. UNDIRECTED GRAPHS (is_directed = false) -> BRIDGE TREE
 * - The algorithm groups nodes into 2-Edge-Connected Components (components 
 * that cannot be disconnected by removing any single edge).
 * - build_condensation_graph() automatically produces a BRIDGE TREE! The 
 * resulting graph is an undirected tree where the edges are the exact 
 * Bridges of the original graph.
 * - Use Case: Tree DP, finding the longest path after a bridge is cut, LCA.
 *
 * THE MAGIC (Edge IDs): 
 * Passing 'p_edge_id' instead of the parent node prevents the DFS from falsely 
 * treating an undirected edge's return path as a cycle. It also makes the code 
 * 100% immune to Multiple Edges (parallel roads) in undirected graphs.
 * ============================================================================== */

// * Reverse sccs to get topological order of condensation graph

struct TarjanSCC {
    int n, timer, scc_count;
    vector<vector<Edge>> adj;
    vector<int> dfsn, low, scc_id;
    vector<bool> instack;
    vector<int> st;
    vector<vector<int>> sccs;
    
    // Condensation Graph Utilities
    vector<vector<int>> condensation_adj;
    vector<int> scc_sizes;

    TarjanSCC(int _n) {
        n = _n;
        adj.assign(n + 1, vector<Edge>());
        dfsn.assign(n + 1, -1);
        low.assign(n + 1, -1);
        scc_id.assign(n + 1, -1);
        instack.assign(n + 1, false);
        timer = scc_count = 0;
    }

    void add_edge(int u, int v, int id, bool is_directed = true) {
        adj[u].push_back({v, id});
        if (!is_directed) {
            adj[v].push_back({u, id}); // Same ID for both directions!
        }
    }

    void dfs(int u, int p_edge_id = -1) {
        dfsn[u] = low[u] = ++timer;
        st.push_back(u);
        instack[u] = true;

        for (auto& edge : adj[u]) {
            int v = edge.to;
            int id = edge.id;

            if (id == p_edge_id) continue; // The Magic Line!

            if (dfsn[v] == -1) {
                dfs(v, id);
                low[u] = min(low[u], low[v]);
            } else if (instack[v]) {
                low[u] = min(low[u], dfsn[v]);
            }
        }

        if (low[u] == dfsn[u]) {
            vector<int> current_scc;
            while (true) {
                int v = st.back();
                st.pop_back();
                instack[v] = false;
                scc_id[v] = scc_count;
                current_scc.push_back(v);
                if (u == v) break;
            }
            sccs.push_back(current_scc);
            scc_count++;
        }
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            if (dfsn[i] == -1) dfs(i);
        }
    }
    // zero based scc ids
    void build_condensation_graph() {
        condensation_adj.assign(scc_count, vector<int>());
        scc_sizes.assign(scc_count, 0);
        set<pair<int, int>> edges_set; // To prevent duplicate edges between SCCs

        for (int i = 0; i < scc_count; i++) {
            scc_sizes[i] = sccs[i].size();
        }

        for (int u = 1; u <= n; u++) {
            for (auto& edge : adj[u]) {
                int v = edge.to;
                int root_u = scc_id[u];
                int root_v = scc_id[v];

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



struct TarjanBridges {
    int n, timer;
    vector<vector<Edge>> adj;
    vector<int> dfsn, low;
    vector<pair<int, int>> bridges; // Stores the bridges

    TarjanBridges(int _n) {
        n = _n;
        adj.assign(n + 1, vector<Edge>());
        dfsn.assign(n + 1, -1);
        low.assign(n + 1, -1);
        timer = 0;
    }

    void add_edge(int u, int v, int id, bool is_directed = false) {
        adj[u].push_back({v, id});
        if (!is_directed) {
            adj[v].push_back({u, id});
        }
    }

    void dfs(int u, int p_edge_id = -1) {
        dfsn[u] = low[u] = ++timer;

        for (auto& edge : adj[u]) {
            int v = edge.to;
            int id = edge.id;

            if (id == p_edge_id) continue;

            if (dfsn[v] == -1) {
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                
                // If the lowest reachable node from 'v' is strictly below 'u'
                if (low[v] > dfsn[u]) {
                    bridges.push_back({u, v});
                }
            } else {
                low[u] = min(low[u], dfsn[v]);
            }
        }
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            if (dfsn[i] == -1) dfs(i);
        }
    }
};



struct TarjanArticulation {
    int n, timer;
    vector<vector<Edge>> adj;
    vector<int> dfsn, low;
    vector<bool> is_art; // True if node 'i' is an articulation point

    TarjanArticulation(int _n) {
        n = _n;
        adj.assign(n + 1, vector<Edge>());
        dfsn.assign(n + 1, -1);
        low.assign(n + 1, -1);
        is_art.assign(n + 1, false);
        timer = 0;
    }

    void add_edge(int u, int v, int id, bool is_directed = false) {
        adj[u].push_back({v, id});
        if (!is_directed) {
            adj[v].push_back({u, id});
        }
    }

    void dfs(int u, int p_edge_id = -1) {
        dfsn[u] = low[u] = ++timer;
        int children = 0;

        for (auto& edge : adj[u]) {
            int v = edge.to;
            int id = edge.id;

            if (id == p_edge_id) continue;

            if (dfsn[v] == -1) {
                children++;
                dfs(v, id);
                low[u] = min(low[u], low[v]);

                // Condition for non-root nodes
                if (low[v] >= dfsn[u] && p_edge_id != -1) {
                    is_art[u] = true;
                }
            } else {
                low[u] = min(low[u], dfsn[v]);
            }
        }

        // Condition for root node
        if (p_edge_id == -1 && children > 1) {
            is_art[u] = true;
        }
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            if (dfsn[i] == -1) dfs(i);
        }
    }
    
    // Utility to get them as a list if needed
    vector<int> get_articulation_points() {
        vector<int> res;
        for (int i = 1; i <= n; i++) {
            if (is_art[i]) res.push_back(i);
        }
        return res;
    }
};



/* ==============================================================================
 * EXPERIENCE POINT: Tarjan's Articulation Points, BCCs & Block-Cut Tree (BCT)
 * ==============================================================================
 * THE PROBLEM:
 * Given an UNDIRECTED graph, find its Articulation Points, group its nodes into 
 * Biconnected Components (Blocks), and build the Block-Cut Tree (BCT).
 * * THE BLOCK-CUT TREE (BCT) STRUCTURE:
 * - The BCT is a Bipartite Tree.
 * - Nodes from [1 to N] represent the original graph vertices.
 * - Nodes from [N + 1 to N + bcc_count] represent the extracted Blocks (BCCs).
 * - An edge exists between original node 'u' and block node 'B' if 'u' is inside 'B'.
 * - Perfect for Tree DP, LCA, and queries about path vulnerabilities.
 * ============================================================================== */

struct Edge {
    int to, id;
};

struct TarjanBCT {
    int n, timer, bcc_count;
    vector<vector<Edge>> adj;
    vector<int> dfsn, low;
    vector<bool> is_art; 
    
    // BCC & BCT specific structures
    vector<int> st; // Stack to hold nodes for BCC extraction
    vector<vector<int>> bccs; // Stores the actual nodes in each BCC
    vector<vector<int>> bct;  // The Block-Cut Tree Adjacency List

    TarjanBCT(int _n) {
        n = _n;
        adj.assign(n + 1, vector<Edge>());
        dfsn.assign(n + 1, -1);
        low.assign(n + 1, -1);
        is_art.assign(n + 1, false);
        timer = bcc_count = 0;
    }

    void add_edge(int u, int v, int id) {
        // Always Undirected for BCCs and Articulation Points
        adj[u].push_back({v, id});
        adj[v].push_back({u, id});
    }

    void dfs(int u, int p_edge_id = -1) {
        dfsn[u] = low[u] = ++timer;
        st.push_back(u); // Push node to stack
        int children = 0;

        for (auto& edge : adj[u]) {
            int v = edge.to;
            int id = edge.id;

            if (id == p_edge_id) continue;

            if (dfsn[v] == -1) {
                children++;
                dfs(v, id);
                low[u] = min(low[u], low[v]);

                // CUT VERTEX CONDITION FOUND!
                if (low[v] >= dfsn[u]) {
                    // Mark as Articulation Point
                    if (p_edge_id != -1 || children > 1) {
                        is_art[u] = true;
                    }

                    // Extract the Biconnected Component (BCC)
                    vector<int> current_bcc;
                    while (true) {
                        int node = st.back();
                        st.pop_back();
                        current_bcc.push_back(node);
                        if (node == v) break; // Stop after popping v
                    }
                    // The cut vertex 'u' is part of this BCC too, but it REMAINS 
                    // in the stack for other BCCs connected higher up!
                    current_bcc.push_back(u); 
                    bccs.push_back(current_bcc);
                    bcc_count++;
                }
            } else {
                low[u] = min(low[u], dfsn[v]);
            }
        }
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            if (dfsn[i] == -1) {
                // Handle isolated vertices (A BCC of size 1)
                if (adj[i].empty()) {
                    bccs.push_back({i});
                    bcc_count++;
                } else {
                    dfs(i);
                }
            }
        }
    }

    // Call this strictly AFTER calling build()
    void build_bct() {
        // Size = N (original nodes) + bcc_count (block nodes) + 1 (1-indexed)
        bct.assign(n + bcc_count + 1, vector<int>());

        for (int i = 0; i < bcc_count; i++) {
            int block_node_id = n + i + 1; // Block IDs start right after N
            
            for (int original_node : bccs[i]) {
                bct[original_node].push_back(block_node_id);
                bct[block_node_id].push_back(original_node);
            }
        }
    }

    vector<int> get_articulation_points() {
        vector<int> res;
        for (int i = 1; i <= n; i++) {
            if (is_art[i]) res.push_back(i);
        }
        return res;
    }
};

/* * ==============================================================================
 * USAGE EXAMPLE:
 * ==============================================================================
 * TarjanBCT tarjan(n);
 * for(int i = 0; i < m; i++){
 * int u, v; cin >> u >> v;
 * tarjan.add_edge(u, v, i); // Pass a unique edge ID
 * }
 * tarjan.build();
 * tarjan.build_bct();
 * * // Now tarjan.bct is your tree! 
 * // If you traverse from an original node 'u', its neighbors in 'bct' are the 
 * // Blocks it belongs to.
 * ============================================================================== */