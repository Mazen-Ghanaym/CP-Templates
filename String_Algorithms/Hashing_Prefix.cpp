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
// hashing
const int N = 300005;
const ll Base1 = 31, Mod1 = 1e9 + 7;
const ll Base2 = 37, Mod2 = 1e9 + 9;

ll pw1[N], pw2[N];

void init() {
    pw1[0] = 1;
    pw2[0] = 1;
    for (int i = 1; i < N; i++) {
        pw1[i] = (pw1[i - 1] * Base1) % Mod1;
        pw2[i] = (pw2[i - 1] * Base2) % Mod2;
    }
}

struct Hash {
    ll h1, h2;
};

struct Hashing {

    vector<Hash> h;
    Hashing(const string &s) {
        int n = sz(s);
        h.resize(n + 1, {0, 0}); // Use 1-based indexing

        for (int i = 0; i < n; i++) {
            h[i + 1].h1 = (h[i].h1 * Base1 + (s[i] - 'a' + 1)) % Mod1;
            h[i + 1].h2 = (h[i].h2 * Base2 + (s[i] - 'a' + 1)) % Mod2;
        }
    }

    // Queries the hash of the substring from l to r (0-based, inclusive).
    Hash query(int l, int r) {
        int len = r - l + 1;

        // Calculate hash for the first modulus
        ll hsh1 = (h[r + 1].h1 - (h[l].h1 * pw1[len]) % Mod1 + Mod1) % Mod1;

        // Calculate hash for the second modulus
        ll hsh2 = (h[r + 1].h2 - (h[l].h2 * pw2[len]) % Mod2 + Mod2) % Mod2;

        return {hsh1, hsh2};
    }

    ull get_hash(int l, int r) {
        Hash res = query(l, r);
        return (res.h1 << 32) | res.h2;
    }
};
void solve(int tc) {
    int n;
    if (!(cin >> n)) {
        exit(0);
    }
    string pattern;
    cin >> pattern;
    string text;
    cin >> text;
    Hashing hash_text(text);
    Hashing hash_pattern(pattern);
    ull pattern_hash = hash_pattern.get_hash(0, n - 1);
    for (int i = 0; i <= sz(text) - n; i++) {
        ull text_hash = hash_text.get_hash(i, i + n - 1);
        if (text_hash == pattern_hash) {
            cout << i << nl;
        }
    }
}
signed main(void) {
    init();
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (1) {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}