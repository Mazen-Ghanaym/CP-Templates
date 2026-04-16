#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;

vector<vector<pair<int, int>>> adj;
void dijkstra(int s, vector<int> &d, vector<int> &p) {
    int n = adj.size();
    d.assign(n, OO);
    p.assign(n, -1);

    d[s] = 0;
    using pii = pair<int, int>;
    priority_queue<pii, vector<pii>, greater<pii>> q;
    q.push({0, s});
    while (!q.empty()) {
        int v = q.top().second;
        int d_v = q.top().first;
        q.pop();
        if (d_v != d[v]) continue;

        for (auto edge : adj[v]) {
            int to = edge.first;
            int len = edge.second;

            if (d[v] + len < d[to]) {
                d[to] = d[v] + len;
                p[to] = v;
                q.push({d[to], to});
            }
        }
    }
}

vector<int> restore_path(int s, int t, vector<int> &p) {
    vector<int> path;
    for (int v = t; v != -1; v = p[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    if (path[0] == s) return path;
    path.clear();
    return path;
}

void solve() {
    int n, m, s, t;
    cin >> n >> m;
    adj.assign(n + 1, vector<pair<int, int>>());
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    vector<int> d, p;
    dijkstra(1, d, p);
    vector<int> path = restore_path(1, n, p);
    if (path.empty()) {
        cout << -1 << nl;
        return;
    }
    for (auto x : path) cout << x << sp;
    cout << nl;
}
signed main(void) {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    while (tc--) {
        solve();
    }
    return 0;
}




/* ==============================================================================
 * TRICK: The "Phantom Super Node" / Multi-Source Dijkstra
 * ==============================================================================
 * USE CASE: 
 * When a problem asks to find the shortest path/cost starting from MULTIPLE 
 * possible nodes, possibly with each node having a different "starting penalty" 
 * or "bonus".
 *
 * HOW TO IMPLEMENT:
 * Instead of physically creating a "Node 0" and adding edges to all starting 
 * nodes, achieve the exact same result by PRE-LOADING the Priority Queue.
 * Push all starting nodes into the PQ with their initial values/penalties 
 * before starting the `while(!pq.empty())` loop.
 * * ⚠️ CRITICAL DETAIL: If a node has multiple possible starting values, ONLY 
 * push the absolute best one. Filter them before pushing:
 * if (initial_dist < dist[start_node]) {
 * dist[start_node] = initial_dist;
 * pq.push({initial_dist, start_node});
 * }
 *
 * WHY IT'S BETTER: 
 * O(1) setup time, saves memory, prevents graph mutation bugs, and runs faster.
 *
 * ------------------------------------------------------------------------------
 * ⚠️ EXCEPTION: When are NEGATIVE WEIGHTS allowed in Dijkstra?
 * ------------------------------------------------------------------------------
 * STANDARD RULE: Dijkstra fails if the graph has negative edge weights because 
 * it assumes that once a node is popped from the PQ, its distance is finalized 
 * and can never get cheaper.
 *
 * THE "SAFE" EXCEPTION (Super Node Initial Values):
 * It is 100% SAFE to push NEGATIVE initial distances into the Priority Queue 
 * at the very beginning.
 *
 * WHY IT WORKS: 
 * Dijkstra only breaks if a path gets cheaper *during traversal*. As long as 
 * the negative values are ONLY used as the initial starting baselines, and ALL 
 * actual graph edges (the transitions between nodes) are >= 0, the greedy 
 * property holds perfectly. The distances will strictly increase from those 
 * negative baselines, meaning no node will ever need to be "re-evaluated."
 * ============================================================================== */

 /*
 // A reusable Dijkstra function that can take multiple starting nodes
// This helps us run both the normal Dijkstra and the "Super Node" Dijkstra
vector<long long> dijkstra(int n, const vector<vector<pair<int, long long>>>& adj, const vector<pair<int, long long>>& starts) {
    vector<long long> dist(n + 1, OO);
    // Min-heap priority queue: stores {distance, node}
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    
    // Initialize the queue with all our starting points
    for (auto& s : starts) {
        int u = s.first;
        long long d = s.second;
        if (d < dist[u]) {
            dist[u] = d;
            pq.push({d, u});
        }
    }
    
    while (!pq.empty()) {
        long long d_u = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        // Optimization: skip outdated pairs
        if (d_u > dist[u]) continue;
        
        for (auto& edge : adj[u]) {
            int v = edge.first;
            long long weight = edge.second;
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
 */


/* ==============================================================================
 * CONCEPT: Understanding the Multi-Source Dijkstra Result Array
 * ==============================================================================
 * When running Dijkstra with multiple starting nodes (S1, S2, ... Sk), each 
 * pre-loaded into the priority queue with an initial weight/penalty (a1, a2, ... ak), 
 * the resulting `dist` array represents the following:
 *
 * 1. THE MATHEMATICAL MEANING:
 * For any node `v`, `dist[v]` holds the absolute CHEAPEST total cost to reach 
 * `v` from ANY of the starting nodes. 
 * Formula: dist[v] = min( Penalty[i] + ShortestPath(Start[i] -> v) )
 *
 * 2. REAL-WORLD ANALOGY 1 (Time/Delay - "The Fire Station"):
 * - Graph: A city roadmap.
 * - Sources: Fire stations.
 * - Penalty: The time it takes a specific station to wake up and start engines.
 * - Result (dist[v]): The EARLIEST possible time a fire truck can reach house `v`,
 * automatically balancing the station's prep time against the driving distance.
 *
 * 3. REAL-WORLD ANALOGY 2 (Cost/Price - "The Supply Chain"):
 * - Graph: Trade routes between cities.
 * - Sources: Factories producing a good.
 * - Penalty: The manufacturing cost at a specific factory.
 * - Result (dist[v]): The CHEAPEST final price a customer in city `v` will pay,
 * automatically balancing cheap manufacturing against expensive shipping.
 *
 * 4. THE MENTAL PROOF (The Phantom Node 0):
 * If you ever forget what the array means, imagine a fake "Node 0". Draw a 
 * directed edge from Node 0 to every starting node `Si` with a weight of `ai`. 
 * The resulting array is simply the standard, single-source shortest path 
 * from Node 0 to everywhere else in the graph.
 * ============================================================================== */



 /* ==============================================================================
 * TRICK: State-Space Dijkstra (Multi-Dimensional Dijkstra)
 * ==============================================================================
 * USE CASE: 
 * When a problem asks to find the shortest path, but you are also carrying or 
 * collecting a SECONDARY RESOURCE (e.g., a bike's slowness, car fuel, tickets).
 * Standard 1D Dijkstra (dist[node]) will fail because it will ruthlessly discard 
 * a "slower" path that happens to carry a much better secondary resource.
 *
 * HOW TO IMPLEMENT:
 * Expand your distance array to 2D (or more): `dist[node][state_value]`
 * Expand your Priority Queue to a tuple: `pq.push({current_dist, current_state, node})`
 * * WHY IT WORKS:
 * By making the state 2D, Path A (fast time, terrible bike) and Path B (slow time, 
 * super-fast bike) no longer overwrite each other. They live in different states:
 * dist[node][terrible_bike] vs dist[node][super_fast_bike].
 *
 * ------------------------------------------------------------------------------
 * ⚠️ THE FATAL TRAP: "Throwing away the carried state"
 * ------------------------------------------------------------------------------
 * When transitioning to the next node, you must push the exact state you just 
 * used/updated into the queue, NOT the inherent property of the destination node.
 * * EXAMPLE BUG (The "Lost Bike" Trap):
 * d[to][carried_bike] = new_time;
 * q.push({new_time, node_bike[to], to}); // ❌ FATAL! You just threw away the 
 * // fast bike you rode in on and grabbed
 * // the (potentially awful) bike at 'to'!
 *
 * CORRECT:
 * d[to][carried_bike] = new_time;
 * q.push({new_time, carried_bike, to});  // ✅ CORRECT! Keep holding the bike 
 * // that you actually used for this state.
 * ============================================================================== */


 /* ==============================================================================
 * EXPERIENCE POINT: Advanced State-Space Dijkstra & Graph DP
 * ==============================================================================
 * PROBLEM: AtCoder ABC 164 E - Two Currencies
 * * KEY TAKEAWAYS:
 * * 1. IN-PLACE TRANSITIONS (The "Stay and Farm" Trick):
 * In Graph DP, transitions do NOT always require moving across an edge. 
 * You can transition from (Node, State_A) to (Node, State_B). 
 * Example: Staying in the exact same city to farm coins at the cost of time.
 * Implementation: pq.push({new_time, new_coins, SAME_NODE});
 * * 2. STATE SPACE BOUNDING (The "Cap" Trick to avoid MLE):
 * If a secondary resource (like coins, health, or fuel) can theoretically grow 
 * infinitely, calculate the ABSOLUTE MAXIMUM you could mathematically ever need.
 * Math: Max Resource Needed = (Max Nodes) * (Max Edge Cost).
 * Any resource gathered beyond this cap is strictly useless. Cap it using `min()` 
 * to keep your 2D array tiny and completely avoid Memory Limit Exceeded (MLE).
 * * 3. THE "READ THE FORMAT" METRIC (Platform Quirks):
 * Always double-check the order of variables in the problem's Input section.
 * Reversing `cin >> coins >> time` into `cin >> time >> coins` is a fatal, 
 * invisible logic bug. It will often pass the sample test cases perfectly 
 * but cause a massive Wrong Answer (WA) on the hidden tests.
 * ============================================================================== */
/*
    s = min(s, MAX_COINS);

    struct Edge {
        int to, cost_coins, cost_time;
    };
    vector<vector<Edge>> adj(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v, a, b;
        cin >> u >> v >> a >> b;
        adj[u].push_back({v, a, b});
        adj[v].push_back({u, a, b}); 
    }

    vector<int> farm_time(n + 1), farm_coins(n + 1);
    for (int i = 1; i <= n; i++) {

        cin >> farm_coins[i] >> farm_time[i];
    }

    vector<vector<int>> dist(n + 1, vector<int>(MAX_COINS + 1, OO));
    
    using State = tuple<int, int, int>; 
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[1][s] = 0;
    pq.push({0, s, 1});

    while (!pq.empty()) {
        auto [d_v, coins, v] = pq.top();
        pq.pop();

        if (d_v > dist[v][coins]) continue;

        // Transition 1: Stay and Farm
        int next_coins = min(MAX_COINS, coins + farm_coins[v]); 
        int next_time = d_v + farm_time[v];
        
        if (next_time < dist[v][next_coins]) {
            dist[v][next_coins] = next_time;
            pq.push({next_time, next_coins, v}); 
        }

        // Transition 2: Travel
        for (auto& edge : adj[v]) {
            if (coins >= edge.cost_coins) {
                int travel_coins = coins - edge.cost_coins;
                int travel_time = d_v + edge.cost_time;
                
                if (travel_time < dist[edge.to][travel_coins]) {
                    dist[edge.to][travel_coins] = travel_time;
                    pq.push({travel_time, travel_coins, edge.to});
                }
            }
        }
    }

    for (int i = 2; i <= n; i++) {
        int ans = OO;
        for (int c = 0; c <= MAX_COINS; c++) {
            ans = min(ans, dist[i][c]);
        }
        cout << ans << "\n";
    }
*/


/* ==============================================================================
 * EXPERIENCE POINT: K-Shortest Paths (K-Dijkstra)
 * ==============================================================================
 * PROBLEM TYPE: 
 * Finding the top K shortest/cheapest routes from a source to a destination, 
 * where revisiting nodes or edges is allowed.
 *
 * THE CORE TRICK (Array of Max-Heaps):
 * Standard Dijkstra uses `int dist[N]` to keep ONLY the #1 best path.
 * For K-paths, use `vector<priority_queue<int>> dist(N)` to keep the top K paths.
 * * WHY A MAX-HEAP?:
 * A Max-Heap places the LARGEST (worst) distance at `.top()`.
 * This gives you an O(1) way to check if a new path deserves to be in the Top K:
 * `if (new_path < dist[v].top())`
 * If true, you simply `pop()` the worst path, and `push()` the new one!
 *
 * QUEUE PRUNING (Crucial for avoiding Time Limit Exceeded):
 * Just like standard Dijkstra uses `if (d > dist[u]) continue;` to prune stale 
 * states, K-Dijkstra MUST prune paths that are worse than the K-th best path:
 * `if (dist[u].size() == K && d > dist[u].top()) continue;`
 *
 * EXTRACTION TRICK:
 * When the algorithm finishes, `dist[target]` contains the K shortest paths. 
 * Because it is a Max-Heap, extracting them will yield the results in DESCENDING 
 * order. Don't forget to push them to a vector and `reverse()` them!
 * 
    dist[1].push(0);
    pq.push({0, 1});
 
    while (!pq.empty()) {
        auto [d_u, u] = pq.top();
        pq.pop();
 
        // Pruning Trick: If this path is strictly worse than the K-th best path 
        // we've already found for node u, it's useless. Throw it away!
        if (dist[u].size() == k && d_u > dist[u].top()) continue;
 
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;
            int next_cost = d_u + w;
 
            // Condition 1: We haven't found K paths to v yet.
            if (dist[v].size() < k) {
                dist[v].push(next_cost);
                pq.push({next_cost, v});
            } 
            // Condition 2: We have K paths, but our new path is BETTER than the worst one.
            else if (next_cost < dist[v].top()) {
                dist[v].pop(); // Kick out the worst path
                dist[v].push(next_cost); // Add our better path
                pq.push({next_cost, v});
            }
        }
    }

 * ============================================================================== */
/* ==============================================================================
 * EXPERIENCE POINT: The "Math-to-State" Illusion (3D Dijkstra)
 * ==============================================================================
 * PROBLEM: Codeforces 1473E - Minimum Path
 * * * THE CORE TRICK (Translating Equations into States):
 * Sometimes a problem gives you a weird equation for a path's cost. 
 * Equation: Total Cost = Sum(Weights) - Max(Weight) + Min(Weight)
 * * Instead of keeping track of the actual max/min (which is impossible to do 
 * optimally in standard Dijkstra), translate the math into "Items" or "Tickets":
 * - "- Max(Weight)": This is a "Free Ticket". You can traverse ONE edge for 0 cost.
 * - "+ Min(Weight)": This is a "Penalty". You MUST traverse ONE edge for 2x cost.
 * * * THE STATE REPRESENTATION (3D Array):
 * dist[node][used_free_ticket][used_penalty]
 * Both boolean states are either 0 (unused) or 1 (used).
 * * * THE IMPLEMENTATION (The 4 Transitions):
 * For every edge you process, you must attempt all valid ticket combinations:
 * * auto [d_v, u, mx, mn] = q.top(); // mx = used_free, mn = used_penalty
 * * for (auto &[v, w] : adj[u]) {
 * // 1. NORMAL: Don't use any tickets. Pay normal weight (w).
 * if (d_v + w < dist[v][mx][mn]) {
 * dist[v][mx][mn] = d_v + w;
 * q.push({dist[v][mx][mn], v, mx, mn});
 * }
 * * // 2. PENALTY: Claim this edge as the Minimum. Pay double (2 * w).
 * if (!mn && d_v + 2 * w < dist[v][mx][1]) {
 * dist[v][mx][1] = d_v + 2 * w;
 * q.push({dist[v][mx][1], v, mx, 1});
 * }
 * * // 3. FREE TICKET: Claim this edge as the Maximum. Pay nothing (0).
 * if (!mx && d_v < dist[v][1][mn]) {
 * dist[v][1][mn] = d_v;
 * q.push({dist[v][1][mn], v, 1, mn});
 * }
 * * // 4. BOTH: Claim this edge as BOTH Max and Min. Pay normal (w).
 * // (Math: w - w + w = w). Only valid if both tickets are unused.
 * if (!mx && !mn && d_v + w < dist[v][1][1]) {
 * dist[v][1][1] = d_v + w;
 * q.push({dist[v][1][1], v, 1, 1});
 * }
 * }
 * ============================================================================== */