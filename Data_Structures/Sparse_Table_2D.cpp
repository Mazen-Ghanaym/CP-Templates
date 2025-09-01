#include <bits/stdc++.h>
using namespace std;

#define nl "\n"

// --- 2D Sparse Table for Range Minimum Query (RMQ) ---
// This data structure finds the minimum value in any rectangular sub-matrix
// of a static (unchanging) 2D array.
// Preprocessing Time: O(N * M * log(N) * log(M))
// Query Time: O(1)
//! zero-based indexing in query
template <typename T> struct SparseTable2D {
    int n, m;
    vector<vector<vector<vector<T>>>> st;
    vector<int> log_table;

    // The function to be applied (e.g., min, max, gcd)
    T func(T a, T b) { return min(a, b); }

    SparseTable2D(const vector<vector<T>> &matrix) {
        n = matrix.size();
        if (n == 0) return;
        m = matrix[0].size();
        if (m == 0) return;

        // Precompute logarithms for O(1) floor(log2(x))
        int max_dim = max(n, m);
        log_table.resize(max_dim + 1);
        log_table[1] = 0;
        for (int i = 2; i <= max_dim; i++) {
            log_table[i] = log_table[i / 2] + 1;
        }

        int max_log_n = log_table[n];
        int max_log_m = log_table[m];

        st.resize(max_log_n + 1, vector<vector<vector<T>>>(max_log_m + 1, vector<vector<T>>(n, vector<T>(m))));

        // Base case: st[0][0] is the original matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                st[0][0][i][j] = matrix[i][j];
            }
        }

        // --- Preprocessing ---
        // Step 1: Fill for k1=0 (row-wise sparse tables)
        for (int k2 = 1; k2 <= max_log_m; k2++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j + (1 << k2) <= m; j++) {
                    st[0][k2][i][j] = func(st[0][k2 - 1][i][j], st[0][k2 - 1][i][j + (1 << (k2 - 1))]);
                }
            }
        }

        // Step 2: Fill for all k1 > 0 using previous results
        for (int k1 = 1; k1 <= max_log_n; k1++) {
            for (int k2 = 0; k2 <= max_log_m; k2++) {
                for (int i = 0; i + (1 << k1) <= n; i++) {
                    for (int j = 0; j + (1 << k2) <= m; j++) {
                        st[k1][k2][i][j] = func(st[k1 - 1][k2][i][j], st[k1 - 1][k2][i + (1 << (k1 - 1))][j]);
                    }
                }
            }
        }
    }

    // O(1) query for the minimum in the rectangle (r1, c1) to (r2, c2)
    T query(int r1, int c1, int r2, int c2) {
        int k1 = log_table[r2 - r1 + 1];
        int k2 = log_table[c2 - c1 + 1];

        T res1 = func(st[k1][k2][r1][c1], st[k1][k2][r1][c2 - (1 << k2) + 1]);
        T res2 = func(st[k1][k2][r2 - (1 << k1) + 1][c1], st[k1][k2][r2 - (1 << k1) + 1][c2 - (1 << k2) + 1]);

        return func(res1, res2);
    }
};

void solve() {
    // Example Usage
    int n, m;
    cin >> n >> m;

    vector<vector<int>> matrix(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> matrix[i][j];
        }
    }

    SparseTable2D<int> st2d(matrix);

    int q;
    cin >> q;
    while (q--) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        cout << st2d.query(r1 - 1, c1 - 1, r2 - 1, c2 - 1) << nl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
