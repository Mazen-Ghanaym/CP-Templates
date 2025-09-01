#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define nl "\n"
#define sp ' '

// --- 2D Fenwick Tree with Rectangle Updates and Rectangle Queries (Corrected) ---
// This data structure allows for adding a value to all elements within a
// rectangular region and querying the sum of values in another rectangular
// region, both in O(log N * log M) time.

template <int N, int M> // Template parameters for max dimensions
struct FenwickTree2dRU {
    // Four 2D BITs are used. The prefix sum S(x,y) can be expressed as:
    // S(x,y) = (x+1)(y+1)sum(D) - (y+1)sum(p*D) - (x+1)sum(q*D) + sum(p*q*D)
    // where D is the 2D difference array.
    // We use a BIT for each of the four sum terms.
    ll bit[4][N][M];

    FenwickTree2dRU() { memset(bit, 0, sizeof(bit)); }

    // Internal helper to perform a point update on one of the four BITs.
    void internal_update(int r, int c, ll v) {
        for (int x = r; x < N; x += x & -x) {
            for (int y = c; y < M; y += y & -y) {
                bit[0][x][y] += v;
                bit[1][x][y] += v * r;
                bit[2][x][y] += v * c;
                bit[3][x][y] += v * r * c;
            }
        }
    }

    // Adds 'delta' to every element in the rectangle from (r1, c1) to (r2, c2).
    // This is done by applying the 2D difference array principle.
    void update(int r1, int c1, int r2, int c2, ll delta) {
        internal_update(r1, c1, delta);
        internal_update(r1, c2 + 1, -delta);
        internal_update(r2 + 1, c1, -delta);
        internal_update(r2 + 1, c2 + 1, delta);
    }

    // Internal helper to query a prefix sum from one of the four BITs.
    ll internal_prefix_query(int r, int c) {
        ll res = 0;
        ll v_sum = 0, vr_sum = 0, vc_sum = 0, vrc_sum = 0;
        for (int x = r; x > 0; x -= x & -x) {
            for (int y = c; y > 0; y -= y & -y) {
                v_sum += bit[0][x][y];
                vr_sum += bit[1][x][y];
                vc_sum += bit[2][x][y];
                vrc_sum += bit[3][x][y];
            }
        }
        res = v_sum * (r + 1) * (c + 1) - vr_sum * (c + 1) - vc_sum * (r + 1) + vrc_sum;
        return res;
    }

    // Calculates the sum of the rectangle from (r1, c1) to (r2, c2)
    // using the principle of inclusion-exclusion on prefix sums.
    ll query(int r1, int c1, int r2, int c2) {
        return internal_prefix_query(r2, c2) - internal_prefix_query(r1 - 1, c2) - internal_prefix_query(r2, c1 - 1) +
               internal_prefix_query(r1 - 1, c1 - 1);
    }
};

void solve() {
    int n, m;
    cin >> n >> m;

    const int MAX_N = 1001;
    FenwickTree2dRU<MAX_N, MAX_N> ft;
    while (m--) {
        int type;
        cin >> type;
        if (type == 1) {
            int r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            cout << ft.query(r1, c1, r2, c2) << nl;
        } else {
            int r1, c1, r2, c2;
            ll k;
            cin >> r1 >> c1 >> r2 >> c2 >> k;
            ft.update(r1, c1, r2, c2, k);
        }
    }
}

signed main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
