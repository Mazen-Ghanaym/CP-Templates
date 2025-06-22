#include <bits/stdc++.h>
using namespace std;
#define sz(x) ((int)(x).size())
// Directed Eulerian Circuit/ Path (Hierholzer's Algorithm)
// If the graph is strongly connected and all vertices have equal in-degree and out-degree, then
// it has an Eulerian circuit. If exactly two vertices have odd degree
// one has out = in + 1 (start node) and the another has in = out + 1 (end node), then it has
// an Eulerian path. If all vertices have equal in-degree and out-degree, then the path starts
// and ends at the same vertex. If exactly two vertices have odd degree, then the path
// starts at one of them and ends at the other.
// Generalized Directed Eulerian Path or Circuit
// Can have self-loops and multiple edges.
// Not all n nodes need to be presented in the path
// only the nodes presented in the m edges are considered.
// check their connectivity and find the path for them.

int n, m;
vector<vector<pair<int, int>>> adj;
vector<int> deg;

// deg[u] = out[u] - in[u]
bool Eulerian_Path_or_Circuit() {
    int st = -1, en = -1, cycle_start = 1;
    for(int u = 1; u <= n; ++u) {
        if(abs(deg[u]) > 1) return false;
        if(!adj[u].empty()) cycle_start = u;

        if(deg[u] == 1) {
            if(~st) return false;
            st = u;
        }

        if(deg[u] == -1) {
            if(~en) return false;
            en = u;
        }
    }

    if(st == -1) st = cycle_start;
    vector<int> edges, nodes, vis(m);
    auto dfs = [&](auto &&dfs, int u) -> void {
        while(!adj[u].empty()) {
            auto [v, i] = adj[u].back();
            adj[u].pop_back();
            if(vis[i]) continue;
            vis[i] = 1, dfs(dfs, v);
            edges.push_back(i);
            nodes.push_back(v);
        }
    };

    dfs(dfs, st);
    if(sz(edges) != m) return false;
    nodes.push_back(st);
    reverse(nodes.begin(), nodes.end());
    reverse(edges.begin(), edges.end());
    
    cout << "Yes\n";
    for(auto &i: nodes) cout << --i << ' ';
    cout << endl;
    for(auto &i: edges) cout << i << ' ';
    cout << endl;
    
    return true;
}

// ==============================================================================

void solve(){
    cin >> n >> m;
    adj.assign(n + 1, vector<pair<int, int>>());
    deg.assign(n + 1, 0);
    
    for(int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        u++, v++;
        adj[u].emplace_back(v, i);
        deg[u]++, deg[v]--;
    }
    
    if(!Eulerian_Path_or_Circuit()) {
        cout << "No\n";
    }
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
        solve();
    return 0;
}
