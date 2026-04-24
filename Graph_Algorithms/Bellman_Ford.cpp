#include <iostream>
#include <vector>
#include <algorithm>
#define int long long
using namespace std;

const int OO = 1e18 + 1;

/*
 * ==============================================================================
 * Bellman-Ford Algorithm Template
 * - Finds shortest paths from a single source.
 * - Can handle negative edge weights.
 * - Detects and retrieves negative cycles.
 * ==============================================================================
 */
struct edge {
    int from, to, w;
    edge(int from, int to, int w) : from(from), to(to), w(w) {}
};

// 1. Basic Bellman-Ford with Early Termination
// Use this when you ONLY need the shortest distances and are sure there are no negative cycles.
void bellman_ford_basic(const vector<edge>& edgelist, int n, int start_node, vector<int>& dist) {
    dist.assign(n + 1, OO);
    dist[start_node] = 0;
    
    bool relaxed;
    for (int i = 0; i < n - 1; i++) {
        relaxed = false;
        for (const auto& [from, to, w] : edgelist) {
            // CRITICAL: dist[from] != OO prevents false relaxation from unreachable nodes
            if (dist[from] != OO && dist[to] > dist[from] + w) {
                dist[to] = dist[from] + w;
                relaxed = true;
            }
        }
        // Early termination if no edges were relaxed in this pass
        if (!relaxed) break;
    }
}

// 2. Bellman-Ford: Full Version
// Computes shortest paths, reconstructs paths, and returns negative cycle nodes if one exists.
vector<int> bellman_ford_full(const vector<edge>& edgelist, int n, int start_node, vector<int>& dist, vector<int>& prev) {
    dist.assign(n + 1, OO);
    prev.assign(n + 1, -1);
    dist[start_node] = 0;
    
    int last_relaxed_node = -1;

    // Run N times. Passes 0 to N-2 are for shortest paths. Pass N-1 is for cycle detection.
    for (int i = 0; i < n; i++) {
        last_relaxed_node = -1;
        for (const auto& [from, to, w] : edgelist) {
            if (dist[from] != OO && dist[to] > dist[from] + w) {
                dist[to] = dist[from] + w;
                prev[to] = from;
                last_relaxed_node = to; // Track the last updated node
            }
        }
    }

    vector<int> cycle;
    
    // If we relaxed a node in the N-th iteration, a negative cycle exists!
    if (last_relaxed_node != -1) {
        // 1. Walk back N times to guarantee we are standing strictly INSIDE the cycle.
        // This handles cases where last_relaxed_node is reachable FROM the cycle, but not IN it.
        int cycle_start = last_relaxed_node;
        for (int i = 0; i < n; i++) {
            cycle_start = prev[cycle_start];
        }

        // 2. Trace the cycle and store the nodes
        int curr = cycle_start;
        do {
            cycle.push_back(curr);
            curr = prev[curr];
        } while (curr != cycle_start);
        
        cycle.push_back(cycle_start); // Complete the loop (e.g., 1 -> 2 -> 3 -> 1)
        reverse(cycle.begin(), cycle.end()); // Reverse to get the chronological path order
    }

    return cycle;
}

// 3. Utility: Reconstruct Shortest Path to a Specific Target
// Call this after running bellman_ford_full to get the exact path.
vector<int> get_shortest_path(int target, const vector<int>& prev, int start_node) {
    vector<int> path;
    
    // If target is unreachable or it's the start node itself (with no incoming edges)
    if (prev[target] == -1 && target != start_node) return path; 
    
    for (int curr = target; curr != -1; curr = prev[curr]) {
        path.push_back(curr);
    }
    reverse(path.begin(), path.end());
    return path;
}

// 4. Utility: Find ALL nodes affected by ANY negative cycle
// Returns a boolean array where affected[i] is true if node i is in a negative cycle 
// OR reachable from a negative cycle.
vector<bool> get_all_cycle_affected_nodes(const vector<edge>& edgelist, int n, int start_node, vector<int>& dist) {
    dist.assign(n + 1, OO);
    dist[start_node] = 0;
    
    // Step 1: Standard Bellman-Ford (N - 1 passes)
    for (int i = 0; i < n - 1; i++) {
        for (const auto& [from, to, w] : edgelist) {
            if (dist[from] != OO && dist[to] > dist[from] + w) {
                dist[to] = dist[from] + w;
            }
        }
    }

    // Step 2: Propagation Phase
    vector<bool> affected(n + 1, false);
    
    // Run N passes to propagate the poison
    for (int i = 0; i < n; i++) {
        for (const auto& [from, to, w] : edgelist) {
            // Condition 1: Normal relaxation means it's caught in a cycle
            if (dist[from] != OO && dist[to] > dist[from] + w) {
                dist[to] = dist[from] + w; 
                affected[to] = true;       
            }
            // Condition 2: If 'from' is poisoned, 'to' is poisoned instantly!
            if (affected[from]) {
                affected[to] = true;
            }
        }
    }

    return affected;
}


/*
 * ==============================================================================
 * NOTE ON POSITIVE CYCLES / LONGEST PATH:
 * To find the Longest Path or detect Positive Cycles, multiply all edge weights 
 * by -1 before building the edgelist, then run this exact same algorithm.
 * ==============================================================================
 */


 /* ==============================================================================
 * EXPERIENCE POINT: Bellman-Ford Initialization Tricks (Single vs. Global)
 * ==============================================================================
 * THE DILEMMA:
 * Standard Bellman-Ford is heavily dependent on how you initialize the `dist` array.
 * Using the wrong initialization will either miss disconnected cycles or completely
 * destroy your shortest path calculations.
 *
 * TRICK 1: SINGLE-SOURCE (The "Reachability" Mode)
 * * Init: `dist[start] = 0`, `dist[others] = OO`
 * * Condition: `if (dist[from] != OO && dist[to] > dist[from] + w)`
 * * Use Case: You are asked for the shortest path from A to B, or you need to know 
 * if a specific path is affected by a negative cycle.
 * * Why: The `!= OO` check prevents the algorithm from relaxing edges in unreachable 
 * components, ensuring distances strictly represent the path from the start node.
 *
 * TRICK 2: GLOBAL DETECTION (The "Super-Source" Mode)
 * * Init: ALL `dist[i] = 0`
 * * Condition: `if (dist[to] > dist[from] + w)` (REMOVE the `!= OO` check!)
 * * Use Case: You are asked "Does ANY negative cycle exist in this graph?", and the 
 * graph might be disconnected. (e.g., CSES 1197 - Cycle Finding).
 * * Why it works: Initializing all nodes to 0 mathematically simulates adding a 
 * dummy "Super Node" connected to every city with a 0-weight teleportation edge.
 * * THE FATAL TRAP: 
 * NEVER use Trick 2 (Global Detection) if the problem asks for Shortest Path distances!
 * Starting all nodes at 0 allows the algorithm to "parachute" into any node for free,
 * completely invalidating the actual travel costs.
 * ============================================================================== */

 // Example of using the "Global Detection" mode to find ANY negative cycle in a disconnected graph:
 vector<int> find_global_negative_cycle(const vector<edge>& edgelist, int n, vector<int>& prev) {
    // Start all nodes at distance 0! This is the equivalent of adding a 
    // "Super Source" connected to all nodes with 0-weight edges.
    vector<int> dist(n + 1, 0); 
    prev.assign(n + 1, -1);
    
    int last_relaxed_node = -1;

    for (int i = 0; i < n; i++) {
        last_relaxed_node = -1;
        for (const auto& [from, to, w] : edgelist) {
            // Notice we DO NOT check dist[from] != OO here, because all dists are valid
            if (dist[to] > dist[from] + w) {
                dist[to] = dist[from] + w;
                prev[to] = from;
                last_relaxed_node = to;
            }
        }
    }

    vector<int> cycle;
    if (last_relaxed_node != -1) {
        int cycle_start = last_relaxed_node;
        for (int i = 0; i < n; i++) {
            cycle_start = prev[cycle_start];
        }

        int curr = cycle_start;
        do {
            cycle.push_back(curr);
            curr = prev[curr];
        } while (curr != cycle_start);
        
        cycle.push_back(cycle_start);
        reverse(cycle.begin(), cycle.end());
    }

    return cycle;
}



/* ==============================================================================
 * EXPERIENCE POINT: Systems of Difference Constraints
 * ==============================================================================
 * THE PROBLEM:
 * Given a set of variables and linear inequalities of the form (X_v - X_u <= W),
 * find valid values for all variables, or determine if no solution exists.
 *
 * THE GRAPH TRANSLATION:
 * The inequality X_v <= X_u + W is mathematically identical to the shortest path
 * triangle inequality: dist[v] <= dist[u] + w.
 * 1. (X_v - X_u <= W)  --> add_edge(u, v, w)
 * 2. (X_v - X_u >= W)  --> X_u - X_v <= -W  --> add_edge(v, u, -w)
 * 3. (X_v - X_u == W)  --> add_edge(u, v, w) AND add_edge(v, u, -w)
 *
 * THE SOLUTION:
 * Use the "Global Cycle Detection" variant of Bellman-Ford (initialize all dist = 0).
 * - If a Negative Cycle is found: NO SOLUTION exists (contradictory constraints).
 * - If NO Negative Cycle is found: The values in the `dist` array are a valid 
 * solution! (x_i = dist[i]).
 * ============================================================================== */