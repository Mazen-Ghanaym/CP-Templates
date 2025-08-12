#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
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

ostream &operator<<(ostream &os, __int128_t value) {
    if (value == 0) {
        os << "0";
        return os;
    }
    if (value < 0) {
        os << "-";
        value = -value;
    }
    string s;
    while (value > 0) {
        s += char(value % 10 + '0');
        value /= 10;
    }
    reverse(s.begin(), s.end());
    os << s;
    return os;
}

istream &operator>>(istream &is, __int128_t &value) {
    string s;
    is >> s;
    value = 0;
    bool is_negative = false;
    if (s[0] == '-') {
        is_negative = true;
        s = s.substr(1);
    }
    for (char c : s) {
        value = value * 10 + (c - '0');
    }
    if (is_negative) value = -value;
    return is;
}

void solve() {
    __int128_t n, m;
    cin >> n >> m;
    cout << n + m << "\n";
}
signed main(void) {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    cin >> tc;
    while (tc--) {
        solve();
    }
    return 0;
}