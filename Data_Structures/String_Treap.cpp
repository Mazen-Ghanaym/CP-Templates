#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
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

const int MAXN = 200005; 
ll fact[MAXN];
ll invFact[MAXN];

ll power(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

void precompute_factorials() {
    fact[0] = 1;
    invFact[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
        invFact[i] = power(fact[i], MOD - 2);
    }
}

// Implicit Treap
// Balanced Binary Search Tree with random priority and it's height is around 2 * log(n)
// can do like ordered statistics tree and more
// 0 based indexing
//! don't forget to set the seed for random number generator
//! don't forget to set the seed for random number generator
mt19937 eng(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<int> rnd{INT_MIN + 1, INT_MAX};

enum DIR { L, R };
struct node;
extern node* EMPTY; 

struct node {
    char val;
    int pri, size;
    node* ch[2];
    bool rev_lazy;
    array<int, 10> p_lazy;
    array<ll, 10> counts;

    node() : val(0), pri(INT_MIN), size(0), ch{this, this}, rev_lazy(false) {
        counts.fill(0);
    }

    node(const char& c) : val(c), pri(rnd(eng)), size(1), ch{EMPTY, EMPTY}, rev_lazy(false) {
        counts.fill(0);
        counts[c - 'a'] = 1;
        iota(p_lazy.begin(), p_lazy.end(), 0);
    }

    void upd() {
        if (this == EMPTY) return;
        size = 1 + ch[L]->size + ch[R]->size;
        for (int i = 0; i < 10; ++i) {
            counts[i] = ch[L]->counts[i] + ch[R]->counts[i];
        }
        counts[val - 'a']++;
    }

    void apply_p(const array<int, 10>& p_new) {
        if (this == EMPTY) return;
        
        val = 'a' + p_new[val - 'a'];

        array<ll, 10> next_counts{};
        for (int i = 0; i < 10; i++) {
            next_counts[p_new[i]] += counts[i];
        }
        counts = next_counts;

        array<int, 10> next_p;
        for (int i = 0; i < 10; i++) {
            next_p[i] = p_new[p_lazy[i]];
        }
        p_lazy = next_p;
    }

    void push() {
        if (this == EMPTY) return;

        if (rev_lazy) {
            swap(ch[L], ch[R]);
            if (ch[L] != EMPTY) ch[L]->rev_lazy ^= 1;
            if (ch[R] != EMPTY) ch[R]->rev_lazy ^= 1;
            rev_lazy = false;
        }

        bool is_identity = true;
        for (int i = 0; i < 10; ++i) if (p_lazy[i] != i) is_identity = false;
        if (!is_identity) {
            if (ch[L] != EMPTY) ch[L]->apply_p(p_lazy);
            if (ch[R] != EMPTY) ch[R]->apply_p(p_lazy);
            iota(p_lazy.begin(), p_lazy.end(), 0);
        }
    }
};
node* EMPTY = new node();

struct Treap {
    node* root = EMPTY;

    void link(node* p, node* c, int dir) {
        if (p != EMPTY) {
            p->ch[dir] = c;
            p->upd();
        }
    }

    pair<node*, node*> split(node* curr, int frstSz) {
        if (curr == EMPTY) return {EMPTY, EMPTY};
        curr->push();
        if (frstSz <= curr->ch[L]->size) {
            auto [ls, gr] = split(curr->ch[L], frstSz);
            link(curr, gr, L);
            return {ls, curr};
        }
        auto [ls, gr] = split(curr->ch[R], frstSz - curr->ch[L]->size - 1);
        link(curr, ls, R);
        return {curr, gr};
    }

    node* merge(node* ls, node* gr) {
        if (ls == EMPTY) return gr;
        if (gr == EMPTY) return ls;
        ls->push();
        gr->push();
        if (ls->pri > gr->pri) {
            link(ls, merge(ls->ch[R], gr), R);
            return ls;
        }
        link(gr, merge(ls, gr->ch[L]), L);
        return gr;
    }

    void insert(char val, int idx) {
        auto [bef, aft] = split(root, idx);
        root = merge(merge(bef, new node(val)), aft);
    }

    void erase(int idx) {
        node *bef, *aft, *target;
        tie(bef, aft) = split(root, idx + 1);
        tie(bef, target) = split(bef, idx);
        root = merge(bef, aft);
        delete target;
    }

    void reverse(int s, int e) {
        node *bef, *aft, *bet;
        tie(bet, aft) = split(root, e + 1);
        tie(bef, bet) = split(bet, s);
        if (bet != EMPTY) bet->rev_lazy ^= 1;
        root = merge(merge(bef, bet), aft);
    }

    void swap_chars(int s, int e, char a, char b) {
        if (a == b) return;
        node *bef, *aft, *bet;
        tie(bet, aft) = split(root, e + 1);
        tie(bef, bet) = split(bet, s);
        if (bet != EMPTY) {
            array<int, 10> p_swap;
            iota(p_swap.begin(), p_swap.end(), 0);
            swap(p_swap[a - 'a'], p_swap[b - 'a']);
            bet->apply_p(p_swap);
        }
        root = merge(merge(bef, bet), aft);
    }

    array<ll, 10> get_counts(int s, int e) {
        node *bef, *aft, *bet;
        tie(bet, aft) = split(root, e + 1);
        tie(bef, bet) = split(bet, s);
        array<ll, 10> res = (bet == EMPTY) ? array<ll, 10>{} : bet->counts;
        root = merge(merge(bef, bet), aft);
        return res;
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;

    Treap t;
    for (char c : s) {
        t.insert(c, t.root->size);
    }

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            cin >> l >> r;
            auto counts = t.get_counts(l - 1, r - 1);
            int len = r - l + 1;
            int odd_counts_num = 0;
            for (ll count : counts) {
                if (count % 2 != 0) odd_counts_num++;
            }

            if (odd_counts_num > 1) {
                cout << 0 << nl;
            } else {
                ll half_len = len / 2;
                ll res = fact[half_len];
                for (ll count : counts) {
                    res = (res * invFact[count / 2]) % MOD;
                }
                cout << res << nl;
            }
        } else if (type == 2) {
            char a;
            int idx;
            cin >> a >> idx;
            t.insert(a, idx - 1);
        } else if (type == 3) {
            int idx;
            cin >> idx;
            t.erase(idx - 1);
        } else if (type == 4) {
            char a, b;
            int l, r;
            cin >> a >> b >> l >> r;
            t.swap_chars(l - 1, r - 1, a, b);
        } else if (type == 5) {
            int l, r;
            cin >> l >> r;
            t.reverse(l - 1, r - 1);
        }
    }
}

signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    precompute_factorials();
    //cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve();
    }
    return 0;
}