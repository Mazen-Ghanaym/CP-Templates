#include <bits/stdc++.h>
using namespace std;

// Directed Eulerian Circuit/ Path
// If the graph is strongly connected and all vertices have equal in-degree and out-degree, then
// it has an Eulerian circuit. If exactly two vertices have odd degree
// one has out = in + 1 (start node) and the another has in = out + 1 (end node), then it has
// an Eulerian path. If all vertices have equal in-degree and out-degree, then the path starts
// and ends at the same vertex. If exactly two vertices have odd degree, then the path
// starts at one of them and ends at the other.

	int n, m;
vector<vector<int>> g;
vector<int> in, out, path;

void dfs(int node) {
	while (!g[node].empty()) {
		int son = g[node].back();
		g[node].pop_back();
		dfs(son);
	}
	path.push_back(node);
}

void Eulerian_Path() {
    // find eulerian path from 1 to n
	cin >> n >> m;

	g.resize(n + 1);
	in.resize(n + 1);
	out.resize(n + 1);

	for (int i = 0; i < m; i++) {
		int x, y;
		cin >> x >> y;
		g[x].push_back(y);
		out[x]++;
		in[y]++;
	}

	bool flag = true;
	for (int node = 2; node < n && flag; node++) {
		if (in[node] != out[node]) { flag = false; }
	}
	if (out[1] != in[1] + 1 || out[n] != in[n] - 1 || !flag) {
		cout << "IMPOSSIBLE";
		return 0;
	}

	dfs(1);

	reverse(path.begin(), path.end());
	if (path.size() != m + 1 || path.back() != n) {
		cout << "IMPOSSIBLE";
	} else {
		for (auto node : path) { cout << node << ' '; }
	}
}
// ==============================================================================
void Eulerian_Circuit() {
    // find eulerian circuit
    cin >> n >> m;

    g.resize(n + 1);
    in.resize(n + 1);
    out.resize(n + 1);

    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        g[x].push_back(y);
        out[x]++;
        in[y]++;
    }

    bool flag = true;
    for (int node = 1; node <= n && flag; node++) {
        if (in[node] != out[node]) { flag = false; }
    }
    if (!flag) {
        cout << "IMPOSSIBLE";
        return 0;
    }

    dfs(1);

    reverse(path.begin(), path.end());
    if (path.size() != m + 1 || path.back() != 1) {
        cout << "IMPOSSIBLE";
    } else {
        for (auto node : path) { cout << node << ' '; }
    }
}
// ==============================================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // Uncomment one of the following lines to choose between Circuit and Path
    // Eulerian_Circuit();
    Eulerian_Path();
    return 0;
}
