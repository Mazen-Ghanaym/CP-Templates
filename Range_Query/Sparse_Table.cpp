#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) int(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
// sparse table
// O(nlogn) preprocessing
// O(1) query
// O(nlogn) memory
// sparse table used for range queries
// sparse table can be used for range sum, range min, range max, range gcd, range lcm, range xor, range and, range or
// sparse table can be used for range queries on static array
// sparse table can't be used for update queries
// zero-based indexing
// zero-based query
/*
sparse table for range min query
    0 1 2 3 4 5 6 7 8 9

    0 0 0 0
    1 1 1 1
    2 2 2 2
    3 3 3
    4 4 4
    5 5 5
    6 6 6
    7 7
    8 8
    9
*/
vector<vector<int>> sparse_table;
vector<int> arr;
int LOG = 20;
int n;
void solve(int tc) {
    cin >> n;
    int q;
    cin >> q;
    LOG = log2(n) + 1;
    arr.resize(n);
    sparse_table.resize(n, vector<int>(LOG));
    for (int i = 0; i < n; ++i) cin >> arr[i];
    // build sparse table
    for (int i = 0; i < n; ++i) sparse_table[i][0] = arr[i];
    for (int j = 1; j < LOG; ++j)
        for (int i = 0; i + (1 << j) <= n; ++i)
            sparse_table[i][j] = min(sparse_table[i][j - 1], sparse_table[i + (1 << (j - 1))][j - 1]);
    // query O(1)
    auto query = [](int l, int r) {
        int k = log2(r - l + 1);
        return min(sparse_table[l][k], sparse_table[r - (1 << k) + 1][k]);
    };
    // query O(logn)
    auto query_log = [](int l, int r) {
        int res = oo;
        for (int j = LOG - 1; j >= 0; --j) {
            if (l + (1 << j) - 1 <= r) {
                res = min(res, sparse_table[l][j]);
                l += (1 << j);
            }
        }
        return res;
    };
    // query
    while (q--) {
        int l, r;
        cin >> l >> r;
        l--; // converting to 0-based index
        r--; // converting to 0-based index
        cout << query_log(l, r) << nl;
    }
}
int main(void) {
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--) {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}