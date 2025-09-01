#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
// #define int long long
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
// ! avoid using #define int long long
extern struct node *const EMPTY;

struct node {
    int value, lazy;
    node *l, *r;

    node() {
        value = lazy = 0;
        l = r = this;
    }

    node(int x) {
        value = lazy = x;
        l = r = EMPTY;
    }
};

node *const EMPTY = new node();

void propagate(node *&cur, int lx, int rx) {
    if (cur->lazy) {
        cur->value = cur->lazy * (rx - lx + 1);
        if (lx != rx) {
            if (cur->l == EMPTY) cur->l = new node(0);
            if (cur->r == EMPTY) cur->r = new node(0);
            cur->l->lazy = cur->lazy;
            cur->r->lazy = cur->lazy;
        }
        cur->lazy = 0;
    }
}

void update(int l, int r, int v, node *&cur, int lx = 1, int rx = 1e9) {
    propagate(cur, lx, rx);
    if (r < lx || l > rx) return;
    if (cur == EMPTY) cur = new node(0);
    if (lx >= l && rx <= r) {
        cur->lazy = v;
        propagate(cur, lx, rx);
        return;
    }
    int mid = lx + (rx - lx) / 2;
    update(l, r, v, cur->l, lx, mid);
    update(l, r, v, cur->r, mid + 1, rx);
    cur->value = cur->l->value + cur->r->value;
}

int query(int ql, int qr, node *&cur, int lx = 1, int rx = 1e9) {
    if (ql > rx || qr < lx) return 0;
    if (cur == EMPTY) return 0;
    propagate(cur, lx, rx);
    if (ql <= lx && qr >= rx) return cur->value;
    int mid = lx + (rx - lx) / 2;
    return query(ql, qr, cur->l, lx, mid) + query(ql, qr, cur->r, mid + 1, rx);
}

void solve(int tc) {
    int q, c = 0;
    cin >> q;
    node *root = EMPTY;
    while (q--) {
        int i, l, r;
        cin >> i >> l >> r;
        l += c, r += c;
        if (i == 1) {
            c = query(l, r, root);
            cout << c << nl;
        } else
            update(l, r, 1, root);
    }
}
signed main(void) {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--) {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}