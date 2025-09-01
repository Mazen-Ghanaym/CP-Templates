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
void solve()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> a[i][j];
            if(j) a[i][j] += a[i][j-1];
        }
    }

    int max_sub_sum = -OO;
    for (int l = 0; l < m; l++){
        for(int r = l; r < m; r++){
            int curr_sum = 0; // if empty subarray is allowed
            for (int row = 0; row < n; row++) {
                if (l) curr_sum = max(a[row][r] - a[row][l-1], curr_sum + a[row][r] - a[row][l-1]);
                else curr_sum = max(a[row][r], curr_sum + a[row][r]);
                max_sub_sum = max(max_sub_sum, curr_sum);
                if (curr_sum < 0) curr_sum = 0; // Use if empty subarray is allowed
            }
        }
    }
    cout << max_sub_sum << nl;
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    //cin >> tc;
    while (tc--)
    {
        solve();
    }
    return 0;
}