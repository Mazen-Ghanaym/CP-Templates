#include <bits/stdc++.h>
using namespace std;

// Undirected Eulerian Circuit/ Path (Hierholzer's Algorithm)
// If the graph is connected and all vertices have even degree, then it has an Eulerian
// circuit. If exactly two vertices have odd degree, then it has an Eulerian path.
// If all vertices have even degree, then the path starts and ends at the same vertex.
// If exactly two vertices have odd degree, then the path starts at one of them and ends at the other.
// Time Complexity: O(n + m), where n is the number of vertices and m is the number of edges.


int n, m;
vector<vector<pair<int, int>>> g;
vector<int> path;
vector<bool> seen;

void dfs(int node) {
	while (!g[node].empty()) {
		auto [son, idx] = g[node].back();
		g[node].pop_back();
		if (seen[idx]) { continue; }
		seen[idx] = true;
		dfs(son);
	}
	path.push_back(node);
}
// ==============================================================================
// if Circuit
void Eulerian_Circuit() {
	cin >> n >> m;

	vector<int> degree(n, 0);
	g.resize(n);
	degree.resize(n);
	seen.resize(m);

	for (int i = 0; i < m; i++) {
		int x, y;
		cin >> x >> y;
		x--, y--;
		g[x].emplace_back(make_pair(y, i));
		g[y].emplace_back(make_pair(x, i));
		degree[x]++;
		degree[y]++;
	}

	for (int node = 0; node < n; node++) {
		if (degree[node] % 2) {
			cout << "IMPOSSIBLE" << endl;
			return;
		}
	}

	dfs(0);

	if ((int)path.size() != m + 1) {
		cout << "IMPOSSIBLE";
	} else {
		for (int node : path) { cout << node + 1 << ' '; }
	}
	cout << endl;
}
// ==============================================================================
// if Path
void Eulerian_Path() {
	cin >> n >> m;
	vector<int> degree(n, 0);
	g.resize(n);
	degree.resize(n);
	seen.resize(m);
	for (int i = 0; i < m; i++) {
		int x, y;
		cin >> x >> y;
		x--, y--;
		g[x].emplace_back(make_pair(y, i));
		g[y].emplace_back(make_pair(x, i));
		degree[x]++;
		degree[y]++;
	}
	int odd = 0;
	for (int node = 0; node < n; node++) {
		if (degree[node] % 2) {
			odd++;
		}
	}
	if (odd != 0 && odd != 2) {
		cout << "IMPOSSIBLE" << endl;
		return;
	}
	int start = 0;
	if (odd == 2) {
		for (int node = 0; node < n; node++) {
			if (degree[node] % 2) {
				start = node;
				break;
			}
		}
	}
	dfs(start);
	if ((int)path.size() != m + 1) {
		cout << "IMPOSSIBLE";
	} else {
		for (int node : path) { cout << node + 1 << ' ';
		}
	}
	cout << endl;
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