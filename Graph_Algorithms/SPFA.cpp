#include <bits/stdc++.h>
#define int long long


/*
* ==============================================================================
* Shortest Path Faster Algorithm (SPFA) Template
* - An optimization of Bellman-Ford for sparse graphs.
* - Uses a queue to only relax edges from recently updated nodes.
* - Can detect negative cycles.
* ==============================================================================
*
* THE PROBLEM:
* Given a directed graph with possibly negative edge weights, find the shortest 
* path from a source node to all other nodes, or determine if a negative cycle 
* is reachable from the source.
*
* THE SOLUTION:
* SPFA maintains a queue of "active" nodes whose outgoing edges need to be relaxed.
* When a node's distance is updated, it is added to the queue if it's not already in it.
* If any node is relaxed more than N times (where N is the number of nodes), it means 
* there is a negative cycle reachable from the source.
*
* TIME COMPLEXITY:
* - Average Case: O(E)
* - Worst Case: O(N*E) (when there are many negative edges)
*
* USE CASES:
* - Use SPFA when you have a sparse graph and need to find shortest paths with negative weights.
* - It can be faster than Dijkstra's algorithm in certain cases, especially when there are many zero-weight edges.
*/


#define OO 1e18 + 1
vector<vector<pair<int, int>>> adj;

bool spfa(int s, vector<int>& d) {
    int n = adj.size();
    d.assign(n, OO);
    vector<int> cnt(n, 0);
    vector<bool> inqueue(n, false);
    queue<int> q;

    d[s] = 0;
    q.push(s);
    inqueue[s] = true;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        inqueue[v] = false;

        for (auto edge : adj[v]) {
            int to = edge.first;
            int len = edge.second;

            if (d[v] + len < d[to]) {
                d[to] = d[v] + len;
                if (!inqueue[to]) {
                    q.push(to);
                    inqueue[to] = true;
                    cnt[to]++;
                    if (cnt[to] > n)
                        return false;  // negative cycle
                }
            }
        }
    }
    return true;
}
