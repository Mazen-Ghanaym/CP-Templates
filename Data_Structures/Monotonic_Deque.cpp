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
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
}
struct monotonic_deque{
    vector<pair<int, int>> frontSt, backSt;
    void push_front(int x) {
        int m = frontSt.empty() ? x : min(x, frontSt.back().second);
        frontSt.emplace_back(x, m);
    }
    void push_back(int x) {
        int m = backSt.empty() ? x : min(x, backSt.back().second);
        backSt.emplace_back(x, m);
    }
    void pop_front() {
        if (frontSt.empty()) {
            while (!backSt.empty()) {
                int v = backSt.back().first;
                backSt.pop_back();
                int m = frontSt.empty() ? v : min(v, frontSt.back().second);
                frontSt.emplace_back(v, m);
            }
        }
        frontSt.pop_back();
    }
    void pop_back() {
        if (backSt.empty()) {
            while (!frontSt.empty()) {
                int v = frontSt.back().first;
                frontSt.pop_back();
                int m = backSt.empty() ? v : min(v, backSt.back().second);
                backSt.emplace_back(v, m);
            }
        }
        backSt.pop_back();
    }
    int get_min() {
        if (frontSt.empty() && backSt.empty()) return -1;
        if (frontSt.empty()) return backSt.back().second;
        if (backSt.empty()) return frontSt.back().second;
        return min(frontSt.back().second, backSt.back().second);
    }
};
void solve(int tc)
{
    int q;
    cin >> q;
    monotonic_deque dq;
    while (q--)
    {
        char op;
        cin >> op;
        if (op == '+')
        {
            char dir;
            int x;
            cin >> dir >> x;
            if (dir == 'L')
                dq.push_front(x);
            else
                dq.push_back(x);
        }
        else if (op == '-')
        {
            char dir;
            cin >> dir;
            if (dir == 'L')
                dq.pop_front();
            else
                dq.pop_back();
        }
        cout << dq.get_min() << nl;
    }
}
signed main(void)
{
    fastio();
    int tc = 1;
    //cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}