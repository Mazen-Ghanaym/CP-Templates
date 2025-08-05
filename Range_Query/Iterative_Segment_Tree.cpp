#include <bits/stdc++.h>
#define ll long long
#define sz(x) (int)(x.size())
#define int long long
using namespace std;
// iterative segment tree
// zero-based indexing
const int N = 1e5; // limit for array size
int n;             // array size
int t[2 * N];

void build() { // build the tree
    for (int i = n - 1; i > 0; --i) t[i] = t[i << 1] + t[i << 1 | 1];
}

void modify(int p, int value) { // set value at position p
    for (t[p += n] = value; p > 1; p >>= 1) t[p >> 1] = t[p] + t[p ^ 1];
}

int query(int l, int r) { //! sum on interval [l, r)
    int res = 0;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
        if (l & 1) res += t[l++];
        if (r & 1) res += t[--r];
    }
    return res;
}

void solve() {
    cin >> n;
    int q;
    cin >> q;
    for (int i = 0; i < n; i++) {
        cin >> t[n + i];
    }
    build();
    while (q--) {
        int type, x, y;
        cin >> type >> x >> y;
        if (type == 1) { // update
            modify(x, y);
        } else { // query
            cout << query(x, y) << '\n';
        }
    }
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