#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
 * ==============================================================================
 * CYCLE DETECTION TEMPLATE (DFS)
 * Covers both Directed and Undirected graphs.
 * Nodes from 1 to N not 0 to N-1.
 * ==============================================================================
 */

int n;
vector<vector<int>> adj;
vector<int> parent;
int cycle_start, cycle_end;

// ==========================================
// 1. DIRECTED GRAPH CYCLE DETECTION
// ==========================================
// 0 = White (Unvisited), 1 = Gray (In Stack), 2 = Black (Done)
vector<char> color; 

bool dfs_directed(int v) {
    color[v] = 1; // Mark as currently visiting (in recursion stack)
    
    for (int u : adj[v]) {
        if (color[u] == 0) { // Unvisited
            parent[u] = v;
            if (dfs_directed(u))
                return true;
        } else if (color[u] == 1) { // Visited AND currently in stack -> CYCLE!
            cycle_end = v;
            cycle_start = u;
            return true;
        }
        // If color[u] == 2 (Black), it's a cross-edge, NOT a cycle. Ignore it.
    }
    
    color[v] = 2; // Mark as completely processed
    return false;
}

void find_cycle_directed() {
    color.assign(n + 1, 0);
    parent.assign(n + 1, -1);
    cycle_start = -1;

    for (int v = 1; v <= n; v++) {
        if (color[v] == 0 && dfs_directed(v))
            break;
    }

    if (cycle_start == -1) {
        cout << "IMPOSSIBLE\n";
    } else {
        vector<int> cycle;
        cycle.push_back(cycle_start);
        for (int v = cycle_end; v != cycle_start; v = parent[v])
            cycle.push_back(v);
        cycle.push_back(cycle_start);
        reverse(cycle.begin(), cycle.end());

        cout << sz(cycle) << nl;
        for (int v : cycle)
            cout << v << " ";
        cout << "\n";
    }
}

// ==========================================
// 2. UNDIRECTED GRAPH CYCLE DETECTION
// ==========================================
vector<bool> visited;

bool dfs_undirected(int v, int par) { 
    visited[v] = true;
    
    for (int u : adj[v]) {
        if (u == par) continue; // Skip the direct edge back to the parent
        
        if (visited[u]) { // Reached ANY previously visited node -> CYCLE!
            cycle_end = v;
            cycle_start = u;
            return true;
        }
        
        parent[u] = v;
        if (dfs_undirected(u, v)) // Cleaned up: passing 'v' directly
            return true;
    }
    return false;
}

void find_cycle_undirected() {
    visited.assign(n + 1, false);
    parent.assign(n + 1, -1);
    cycle_start = -1;

    for (int v = 1; v <= n; v++) {
        // We pass -1 as the initial parent since the root has no parent
        if (!visited[v] && dfs_undirected(v, -1)) break;
    }

    if (cycle_start == -1) {
        cout << "IMPOSSIBLE\n";
    } else {
        vector<int> cycle;
        cycle.push_back(cycle_start);
        for (int v = cycle_end; v != cycle_start; v = parent[v]) cycle.push_back(v);
        cycle.push_back(cycle_start);
        reverse(cycle.begin(), cycle.end());

        cout << sz(cycle) << nl;
        for (int v : cycle) cout << v << " ";
        cout << "\n";
    }
}

/* ==============================================================================
 * EXPERIENCE POINT: Undirected Cycle Detection (The Parallel Edges Trap)
 * ==============================================================================
 * THE PROBLEM:
 * Standard undirected cycle detection uses `if (next_node == parent_node) continue;`
 * to avoid falsely detecting a cycle when looking back at the node that just called it.
 * HOWEVER, this completely fails on graphs with Multiple/Parallel Edges (e.g., Yosupo).
 * If there are two roads between city A and city B, traveling to B on road 1 and 
 * returning on road 2 is a valid cycle! But the standard code will see "City A" 
 * as the parent and skip road 2 entirely.
 *
 * THE SOLUTION (Edge IDs):
 * Instead of passing the `parent_node`, pass the `incoming_edge_id`.
 * 1. Store edges as pairs: {destination_node, edge_id}.
 * 2. In DFS: `if (current_edge_id == incoming_edge_id) continue;`
 * This ensures we only ignore the EXACT SAME EDGE we just traveled on, allowing 
 * parallel edges to correctly trigger a cycle!
 * ============================================================================== */
int last_edge_id = -1;
bool dfs_undirected(int v, int edge_id) { 
    visited[v] = true;
    
    for (auto [u, id] : adj[v]) {
        if (id == edge_id) continue;
        if (visited[u]) { 
            cycle_end = v, cycle_start = u;
            last_edge_id = id;
            return true;
        }
        parent[u] = v;
        parent_edge[u] = id;
        if (dfs_undirected(u, id))
            return true;
    }
    return false;
}

void find_cycle_undirected() {
    visited.assign(n + 1, false);
    parent.assign(n + 1, -1);
    parent_edge.assign(n + 1, -1);
    cycle_start = -1;

    for (int v = 1; v <= n; v++) 
        if (!visited[v] && dfs_undirected(v, -1)) break;
    
    if (cycle_start == -1) {
        cout << "-1\n";
    } else {
        vector<int> cycle;
        vector<int> edges_in_cycle;
        cycle.push_back(cycle_start);
        for (int v = cycle_end; v != cycle_start; v = parent[v]) {
            cycle.push_back(v);
            edges_in_cycle.push_back(parent_edge[v]);
        }
        cycle.push_back(cycle_start);
        reverse(cycle.begin(), cycle.end());
        reverse(edges_in_cycle.begin(), edges_in_cycle.end());
        edges_in_cycle.push_back(last_edge_id);
        cout << sz(cycle) - 1 << nl;
        for(int i = 0; i < sz(cycle) - 1; i++) {
            cout << cycle[i] - 1 << sp;
        }
        cout << "\n";
        for(int i = 0; i < sz(edges_in_cycle); i++) {
            cout << edges_in_cycle[i] << sp;
        }
        cout << "\n";
    }
}


/* ==============================================================================
 * EXPERIENCE POINT: Graph Girth (Shortest Cycle in UNDIRECTED UNWEIGHTED Graph)
 * ==============================================================================
 * THE PROBLEM:
 * DFS cycle detection finds ANY cycle, but there is no guarantee it's the shortest.
 * If a problem asks for the "shortest cycle" (Girth), DFS will give a Wrong Answer.
 *
 * THE SOLUTION (BFS from ALL nodes):
 * We run a BFS starting from every single node. Since BFS explores level-by-level,
 * the first time it encounters an already-visited node (that isn't its parent), 
 * it mathematically guarantees finding the shortest cycle passing through that root.
 * Cycle Length = dist[u] + dist[v] + 1.
 *
 * COMPLEXITY:
 * O(V * (V + E)). This is fast enough for N <= 3000.
 *
 * CRITICAL WARNING:
 * This logic ONLY works for UNWEIGHTED graphs! If the graph has edge weights, BFS 
 * no longer guarantees the shortest path. For weighted graphs, you must run 
 * Dijkstra from every node (or use Floyd-Warshall O(V^3) if N <= 400).
 * ============================================================================== */

const int OO = 1e9;

// Returns the length of the shortest cycle. Returns -1 if the graph is Acyclic.
// 'adj' should be 1-based indexing: vector<vector<int>> adj(n + 1);
int get_graph_girth(int n, const vector<vector<int>>& adj) {
    int min_cycle = OO;
    
    for (int start_node = 1; start_node <= n; start_node++) {
        vector<int> dist(n + 1, -1);
        vector<int> parent(n + 1, -1);
        queue<int> q;
        
        dist[start_node] = 0;
        q.push(start_node);
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                } 
                else if (v != parent[u]) {
                    // Reached an already visited node that is NOT our parent.
                    // This edge closes a cycle!
                    min_cycle = min(min_cycle, dist[u] + dist[v] + 1);
                }
            }
        }
    }
    
    return (min_cycle == OO) ? -1 : min_cycle;
}


/* ==============================================================================
 * EXPERIENCE POINT: Shortest Cycle in Weighted Undirected Graph (Girth)
 * ==============================================================================
 * THE PROBLEM:
 * Finding the shortest cycle in a weighted graph. 
 * - Bellman-Ford from all nodes is O(V^2 * E), which gives TLE.
 * - Standard Dijkstra requires deleting edges one by one O(E^2 log V), also TLE.
 *
 * THE SOLUTION (Dijkstra with Branches): O(V * E log V)
 * 1. Run Dijkstra from a starting node `S`.
 * 2. Assign each direct neighbor of `S` its own unique "Branch ID".
 * 3. As Dijkstra expands, nodes inherit the Branch ID of their parent.
 * 4. COLLISION: If we try to relax an edge between two nodes with DIFFERENT 
 * Branch IDs, it means two paths from `S` have met, forming a cycle!
 * Cycle Length = dist[u] + dist[v] + weight.
 *
 * CRITICAL OPTIMIZATIONS & FIXES:
 * - Pruning: If `dist[u] >= min_cycle`, STOP. Any cycle found will be longer.
 * - Self-Loops & Multiple Edges: Handled explicitly before Dijkstra starts.
 * - The "Absorption Flaw": If a shorter branch overtakes a longer branch, the 
 * longer branch loses its ID. We must explicitly check if an edge points back 
 * to `S` (from a non-direct branch) to avoid missing these cycles.
 * ============================================================================== */
struct Edge {
    int to;
    long long weight;
};

int n; // Number of nodes
vector<vector<Edge>> adj;

// Returns the shortest cycle passing through 'start'. Returns -1 if no cycle.
long long get_shortest_cycle_for_node(int start) {
    vector<long long> dist(n + 1, OO);
    vector<int> branch(n + 1, -1);
    
    // Priority queue stores {distance, current_node}
    priority_queue<pair<long long, int>, 
                   vector<pair<long long, int>>, 
                   greater<pair<long long, int>>> pq;
                   
    long long min_cycle = OO;
    
    // 1. Handle direct neighbors and initialize branches
    for (auto& edge : adj[start]) {
        int v = edge.to;
        long long w = edge.weight;
        
        if (v == start) { // Edge Case: Self-loop
            min_cycle = min(min_cycle, w);
            continue;
        }
        
        if (dist[v] != OO) { // Edge Case: Multiple edges between same two nodes
            min_cycle = min(min_cycle, dist[v] + w);
        }
        
        if (w < dist[v]) {
            dist[v] = w;
            branch[v] = v; // The Branch ID is the neighbor node itself
            pq.push({w, v});
        }
    }
    
    // 2. Run Dijkstra to grow branches
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        
        // PRUNING: Stop exploring if current distance exceeds the best cycle found
        if (d >= min_cycle) continue; 
        
        for (auto& edge : adj[u]) {
            int v = edge.to;
            long long w = edge.weight;

            if (v == start) {
                // If we reach the start node via a different branch's path
                if (branch[u] != u && branch[u] != -1) {
                    min_cycle = min(min_cycle, dist[u] + w);
                }
                continue;
            }
            
            // 3. COLLISION: Two different branches met. We found a valid cycle!
            if (branch[v] != -1 && branch[u] != branch[v]) {
                min_cycle = min(min_cycle, dist[u] + dist[v] + w);
            }
            
            // 4. Update path (Standard Dijkstra)
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                branch[v] = branch[u]; // Inherit the branch ID
                pq.push({dist[v], v});
            }
        }
    }
    
    return (min_cycle == OO) ? -1 : min_cycle;
}

// Global minimum cycle wrapper
long long get_global_shortest_cycle() {
    long long global_min = OO;
    for (int i = 1; i <= n; i++) {
        long long current_min = get_shortest_cycle_for_node(i);
        if (current_min != -1) {
            global_min = min(global_min, current_min);
        }
    }
    return (global_min == OO) ? -1 : global_min;
}

/* ==============================================================================
 * EXPERIENCE POINT: Shortest Cycle for EACH Edge
 * ==============================================================================
 * THE PROBLEM:
 * You need to find the length of the shortest cycle passing through EVERY 
 * specific edge in the graph. 
 * - Naive approach: Force a cycle by running Dijkstra between the endpoints (u, v) 
 * for every single edge. Complexity: O(E * E log V). This gives TLE when E is large.
 *
 * THE SOLUTION (Branch Dijkstra with Edge IDs): O(V * E log V)
 * Instead of running Dijkstra per edge, we run it per node (V times).
 * 1. Start Dijkstra from a node `S`.
 * 2. For every direct neighbor `v` of `S`, we assign its `branch[v]` to be 
 * the exact `edge_id` of the road connecting `S` and `v`.
 * 3. As Dijkstra explores, nodes inherit this `edge_id`.
 * 4. COLLISION: When two different branches meet (or a branch loops back to `S`),
 * the cycle formed explicitly contains the TWO initial edges that started these
 * branches. 
 * 5. We immediately update the answer for BOTH `edge_id`s involved!
 *
 * This beautifully computes the shortest cycle for all edges connected to `S`
 * in a single Dijkstra run. Repeating this for all V nodes covers the entire graph.
 * ============================================================================== */
struct Edge {
    int to;
    long long weight;
    int id; 
};

int n, m;
vector<vector<Edge>> adj;
vector<long long> ans;

void solve_test_case(int case_num) {
    cin >> n >> m;
    
    adj.assign(n, vector<Edge>());
    ans.assign(m, OO);

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w, i});
        adj[v].push_back({u, w, i});
    }
    // Run Branch Dijkstra from every node
    for (int start = 0; start < n; start++) {
        vector<long long> dist(n, OO);
        vector<int> branch(n, -1);
        
        priority_queue<pair<long long, int>, 
                       vector<pair<long long, int>>, 
                       greater<pair<long long, int>>> pq;
                       
        // 1. Initialize branches with Edge IDs
        for (auto& edge : adj[start]) {
            int v = edge.to;
            long long w = edge.weight;
            int id = edge.id;
            // Assuming no multiple edges or self-loops in this specific problem type
            if (w < dist[v]) {
                dist[v] = w;
                branch[v] = id; // Branch ID is the actual Edge ID
                pq.push({w, v});
            }
        }
        // 2. Dijkstra Expansion
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;
            for (auto& edge : adj[u]) {
                int v = edge.to;
                long long w = edge.weight;
                int id = edge.id;
                
                // Edge Case: Returning to the start node via a different branch
                if (v == start) {
                    if (branch[u] != -1 && branch[u] != id) {
                        long long cycle_len = dist[u] + w;
                        ans[branch[u]] = min(ans[branch[u]], cycle_len);
                        ans[id] = min(ans[id], cycle_len);
                    }
                    continue;
                }
                // COLLISION: Two different branches meet
                if (branch[v] != -1 && branch[u] != branch[v]) {
                    long long cycle_len = dist[u] + dist[v] + w;
                    // Update the answer for both edges that started these branches
                    ans[branch[u]] = min(ans[branch[u]], cycle_len);
                    ans[branch[v]] = min(ans[branch[v]], cycle_len);
                }
                // Standard Path Relaxation
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    branch[v] = branch[u]; // Inherit the Edge ID
                    pq.push({dist[v], v});
                }
            }
        }
    }
    
    // 3. Print Results
    cout << "Case " << case_num << ":" << "\n";
    for (int i = 0; i < m; i++) {
        if (ans[i] == OO) {
            cout << "impossible\n";
        } else {
            // Toll must be strictly greater than total cycle weight
            long long toll = ans[i] + 1; 
            cout << toll << "\n";
        }
    }
}

/* ==============================================================================
 * EXPERIENCE POINT: Shortest Cycle for EACH Edge (Ultimate General Version)
 * ==============================================================================
 * Handles:
 * - General Graphs
 * - Self-Loops (Cycle of length 1)
 * - Multiple Edges (Parallel roads forming 2-cycles)
 * * CORE LOGIC ADDITIONS:
 * 1. Self-loops are caught immediately at the `start` node.
 * 2. Multiple edges adjacent to `start` immediately form a cycle with each other. 
 * Both edge IDs are updated, but only the shorter edge becomes the Branch ID 
 * that goes into the Priority Queue.
 * ============================================================================== */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long OO = 1e18;

struct Edge {
    int to;
    long long weight;
    int id; 
};

int n, m;
vector<vector<Edge>> adj;
vector<long long> ans;

void solve_test_case(int case_num) {
    cin >> n >> m;
    
    adj.assign(n, vector<Edge>());
    ans.assign(m, OO);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w, i});
        adj[v].push_back({u, w, i});
    }
    
    for (int start = 0; start < n; start++) {
        vector<long long> dist(n, OO);
        vector<int> branch(n, -1);
        
        priority_queue<pair<long long, int>, 
                       vector<pair<long long, int>>, 
                       greater<pair<long long, int>>> pq;
                       
        for (auto& edge : adj[start]) {
            int v = edge.to;
            long long w = edge.weight;
            int id = edge.id;
            
            // Case A: Self-Loop (A cycle consisting of exactly 1 edge)
            if (v == start) {
                ans[id] = min(ans[id], w);
                continue;
            }
            
            // Case B: Multiple Edges (We've already seen another road to 'v')
            // This forms a 2-cycle with the previous edge!
            if (dist[v] != OO) {
                long long cycle_len = dist[v] + w;
                ans[id] = min(ans[id], cycle_len);
                ans[branch[v]] = min(ans[branch[v]], cycle_len);
            }
            
            // Only the shortest edge to 'v' gets to lead the branch
            if (w < dist[v]) {
                dist[v] = w;
                branch[v] = id; 
                pq.push({w, v});
            }
        }
        
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            
            if (d > dist[u]) continue;
            
            for (auto& edge : adj[u]) {
                int v = edge.to;
                long long w = edge.weight;
                int id = edge.id;
                
                // Edge Case: Returning to start via a different edge
                if (v == start) {
                    if (branch[u] != -1 && branch[u] != id) {
                        long long cycle_len = dist[u] + w;
                        ans[branch[u]] = min(ans[branch[u]], cycle_len);
                        ans[id] = min(ans[id], cycle_len);
                    }
                    continue;
                }
                
                // COLLISION: Two different branches meet
                if (branch[v] != -1 && branch[u] != branch[v]) {
                    long long cycle_len = dist[u] + dist[v] + w;
                    ans[branch[u]] = min(ans[branch[u]], cycle_len);
                    ans[branch[v]] = min(ans[branch[v]], cycle_len);
                }
                
                // Path Relaxation
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    branch[v] = branch[u]; 
                    pq.push({dist[v], v});
                }
            }
        }
    }
    
    // 3. Print Results (Example output)
    cout << "Case " << case_num << ":\n";
    for (int i = 0; i < m; i++) {
        if (ans[i] == OO) {
            cout << "impossible\n";
        } else {
            cout << ans[i] << "\n";
        }
    }
}


/* ==============================================================================
 * EXPERIENCE POINT: Karp's Minimum Mean Weight Cycle
 * ==============================================================================
 * THE PROBLEM:
 * Find a cycle in a DIRECTED graph such that the AVERAGE edge weight
 * (Total Weight of Cycle / Number of Edges in Cycle) is minimized.
 * Standard shortest path algorithms fail here because a cycle with a larger
 * total sum might have a smaller average if it has many edges.
 *
 * THE SOLUTION (Dynamic Programming & Karp's Formula):
 * 1. dp[k][u] = minimum cost to reach node 'u' using EXACTLY 'k' edges.
 * 2. We initialize dp[0][u] = 0 for all nodes (this acts as a dummy source
 * connected to all nodes with 0 weight, perfectly handling disconnected components).
 * 3. Calculate DP table up to V edges.
 * 4. Apply Karp's formula for each node v:
 * max_{0 <= k < V} (dp[V][v] - dp[k][v]) / (V - k)
 * 5. The minimum value across all nodes is the minimum mean weight cycle.
 *
 * COMPLEXITY:
 * Time: O(V * E)
 * Space: O(V^2) -> Notice the memory! Be careful if V > 5000.
 * ============================================================================== */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

const long long OO = 1e18; // Use a large enough infinity to avoid overflow
const double INF = 1e15;

struct Edge {
    int u, v;
    long long w;
};

int n, m;
vector<Edge> edges;

// Returns the minimum average cycle weight. Returns -1.0 if the graph is Acyclic.
double get_min_mean_weight_cycle() {
    // dp[k][u] = min weight to reach 'u' using exactly 'k' edges
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, OO));

    // Start from all nodes with 0 distance at 0 edges
    for (int i = 1; i <= n; i++) {
        dp[0][i] = 0;
    }

    // Fill the DP table (similar to Bellman-Ford, but exact edge counts)
    for (int k = 1; k <= n; k++) {
        for (const auto &edge : edges) {
            int u = edge.u;
            int v = edge.v;
            long long w = edge.w;

            if (dp[k - 1][u] != OO) {
                dp[k][v] = min(dp[k][v], dp[k - 1][u] + w);
            }
        }
    }

    double min_mean_cycle = INF;

    // Apply Karp's Formula
    for (int i = 1; i <= n; i++) {
        if (dp[n][i] == OO) continue; // Node 'i' cannot be reached with exactly n edges

        double max_val = -INF;
        for (int k = 0; k < n; k++) {
            if (dp[k][i] != OO) {
                double val = (double)(dp[n][i] - dp[k][i]) / (n - k);
                max_val = max(max_val, val);
            }
        }
        min_mean_cycle = min(min_mean_cycle, max_val);
    }

    return (min_mean_cycle == INF) ? -1.0 : min_mean_cycle;
}

void solve() {
    if (!(cin >> n >> m)) return;

    edges.clear();
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        // IMPORTANT: Karp's algorithm is for DIRECTED graphs.
        edges.push_back({u, v, w});
    }

    double ans = get_min_mean_weight_cycle();

    if (ans == -1.0) {
        cout << "No cycle found.\n";
    } else {
        cout << fixed << setprecision(2) << ans << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int tc = 1;
    cin >> tc;
    int i = 1;
    while (tc--){
        cout << "Case #" << i++ << ": ";
        solve();
    } 
    return 0;
}