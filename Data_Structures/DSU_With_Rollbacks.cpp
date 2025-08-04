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
template <typename T = int, int Base = 1>
struct DSU
{

    vector<T> parent, Gsize;
    int NumberOfComponents;
    stack<array<int, 5>> stk;

    DSU() {}

    DSU(int MaxNodes)
    {
        parent = Gsize = vector<T>(MaxNodes + Base);
        for (int i = Base; i < MaxNodes + Base; i++)
        {
            parent[i] = i;
            Gsize[i] = 1;
        }
        while(sz(stk))
            stk.pop();
        NumberOfComponents = MaxNodes;
    }

    T find_leader(int node)
    {
        return (parent[node] == node ? node : find_leader(parent[node]));
    }

    bool is_same_sets(int u, int v)
    {
        return find_leader(u) == find_leader(v);
    }

    void union_sets(int u, int v)
    {
        int leader_u = find_leader(u), leader_v = find_leader(v);
        if (leader_u == leader_v)
        {
            stk.push({-1, -1, -1, -1, -1});
            return;
        }
        // make leader_u is the leader with the larger component
        if (Gsize[leader_u] < Gsize[leader_v])
            swap(leader_u, leader_v);
        stk.push({leader_u, Gsize[leader_u], leader_v, parent[leader_v], NumberOfComponents});
        Gsize[leader_u] += Gsize[leader_v];
        parent[leader_v] = leader_u;
        NumberOfComponents--;
    }

    void restore()
    {
        while (sz(stk))
            undo();
    }
    void undo(){
        auto [leader_u, size_u, leader_v, parent_v, _NumberOfComponents] = stk.top();
        stk.pop();
        if(leader_u == -1) return;
        Gsize[leader_u] = size_u;
        parent[leader_v] = parent_v;
        NumberOfComponents = _NumberOfComponents;
    }
    int get_size(int u)
    {
        return Gsize[find_leader(u)];
    }

    int get_components_number()
    {
        return NumberOfComponents;
    }
};
void solve(int tc)
{
    int n, q;
    cin >> n >> q;
    DSU<int> dsu(n);
    vector<string> persists;
    while (q--)
    {
        string op;
        cin >> op;
        if(op == "union"){
            int u, v;
            cin >> u >> v;
            dsu.union_sets(u, v);
            persists.push_back(op);
            cout << dsu.get_components_number() << nl;
        }else if(op == "rollback"){
            while(sz(persists) && persists.back() != "persist"){
                dsu.undo();
                persists.pop_back();
            }
            if(sz(persists)) persists.pop_back();
            cout << dsu.get_components_number() << nl;
        }else{
            persists.push_back(op);
        }
    }
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
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

