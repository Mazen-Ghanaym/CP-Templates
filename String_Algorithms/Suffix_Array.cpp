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
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}
struct SuffixArray
{
    #define all(x) x.begin(), x.end()
    string s;
    int n, LOG;
    vector<int> p, c, lcp;
    vector<vector<int>> mn;

    // c[i] is the class of the suffix starting at i
    // p[i] is the index of the ith suffix in lexiographical sorting
    // lcp[i] is the length of the "lcp" of the "i and i+1" suffix in the suffix array

    explicit SuffixArray() = default;

    explicit SuffixArray(string s) : s(s), n(sz(s) + 1)
    {
        s += "$"; // '$' less than any char of the string
        p = c = lcp = vector<int>(n);
        build();
        build_lcp();
        build_sparse_table();
    }

    // count sort the suffix array
    inline void count_sort()
    {
        vector<int> cnt(n, 0), pos(n, 0), p_new(n);
        for (auto x : c)
            cnt[x]++;
        for (int i = 1; i < n; i++)
            pos[i] = pos[i - 1] + cnt[i - 1];
        for (auto x : p)
            p_new[pos[c[x]]++] = x;
        p = p_new;
    }

    // build the suffix array
    inline void build()
    {
        // for k = 0
        vector<pair<char, int>> a(n);
        for (int i = 0; i < n; i++)
            a[i] = {s[i], i};
        sort(all(a));
        for (int i = 0; i < n; i++)
            p[i] = a[i].second;
        c[p[0]] = 0;

        // assign equivalence classes to suffixes
        for (int i = 1; i < n; i++)
            c[p[i]] = c[p[i - 1]] + (a[i].first != a[i - 1].first);

        // for k > 0
        int k = 0;
        while (((1 << k) < n) and p.back() != n - 1)
        {
            for (int i = 0; i < n; i++)
                p[i] = (p[i] - (1 << k) + n) % n; // cyclic shift
            count_sort();
            vector<int> c_new(n);
            c_new[p[0]] = 0;
            for (int i = 1; i < n; i++)
            {
                pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]};
                pair<int, int> now = {c[p[i]], c[(p[i] + (1 << k)) % n]};
                c_new[p[i]] = c_new[p[i - 1]] + (prev != now);
            }
            c = c_new;
            k++;
        }
    }

    // build the lcp array
    inline void build_lcp()
    {
        int k = 0;
        for (int i = 0; i < n - 1; i++)
        {
            int pi = c[i];
            int j = p[pi - 1]; // previous suffix in the suffix array
            while (s[i + k] == s[j + k])
                k++;
            lcp[pi] = k;
            k = max(k - 1, 0ll);
        }
    }

    inline void build_sparse_table()
    {
        LOG = 32 - __builtin_clz(n);
        mn = vector<vector<int>>(n, vector<int>(LOG));
        for (int i = 1; i < n; i++)
            mn[i][0] = lcp[i];
        for (int j = 1; j < LOG; j++)
        {
            for (int i = 0; i + (1 << j) <= n; i++)
            {
                mn[i][j] = min(mn[i][j - 1], mn[i + (1 << (j - 1))][j - 1]);
            }
        }
    }

    // compare two suffixes starting at i and j with length l in the original string
    inline int compare(const int i, const int j, const int l)
    {
        assert(0 <= i && i < n && 0 <= j && j < n);
        pair<int, int> a = {c[i], c[(i + l - 1) % n]};
        pair<int, int> b = {c[j], c[(j + l - 1) % n]};
        return a == b ? 0 : a < b ? -1
                                  : 1;
    }

    // compare the suffix starting at i with the pattern
    inline int compare(const int i, const string &pattern)
    {
        return s.compare(i, sz(pattern), pattern);
    }

    // check if the pattern is in the string
    inline bool find(const string &pattern)
    {
        int l = 0, r = n - 1;
        while (l <= r)
        {
            int mid = (l + r) >> 1;
            int res = compare(p[mid], pattern);
            if (res == 0)
                return true;
            if (res < 0)
                l = mid + 1;
            else
                r = mid - 1;
        }
        return false;
    }

    // return the index of the first occurrence of the pattern in the string
    inline int lower(const string &pattern)
    {
        int L = lower_bound(p.begin(), p.end(), pattern, [&](int i, const string &pattern)
                            { return s.substr(i, sz(pattern)) < pattern; }) -
                p.begin();
        return L;
    }

    // return the index of the last occurrence of the pattern in the string
    inline int upper(const string &pattern)
    {
        int R = upper_bound(p.begin(), p.end(), pattern, [&](const string &pattern, int i)
                            { return s.substr(i, sz(pattern)) > pattern; }) -
                p.begin() - 1;
        return R;
    }

    // return the number of occurrences of the pattern in the string
    inline int count(const string &pattern)
    {
        return upper(pattern) - lower(pattern) + 1;
    }

    // return the number of distinct substrings in the string
    inline ll distinct_substrings()
    {
        ll ans = 0;
        for (int i = 1; i < n; i++)
        {
            // calculate how many different prefixes this suffix has
            ans += n - (1 + p[i] + lcp[i]);
        }
        return ans;
    }

    // return the longest repeated substring in the string
    inline string longest_repeated_substring()
    {
        int idx = max_element(lcp.begin(), lcp.end()) - lcp.begin();
        return s.substr(p[idx], lcp[idx]);
    }

    // longest common substring of the string and the pattern
    inline string longest_common_substring(const string &s, const string &t)
    {
        string st = s + "#" + t + "$";  // if wrong st = s + "\" + t
        SuffixArray sa(st);
        int n = sz(s), ans = 0, idx = 0;
        for (int i = 1; i < sz(st); i++)
        {
            if ((sa.p[i] < n and sa.p[i - 1] > n) or (sa.p[i] > n and sa.p[i - 1] < n))
            {
                if (ans < sa.lcp[i])
                    ans = sa.lcp[i], idx = sa.p[i];
            }
        }
        return st.substr(idx, ans);
    }

    // longest common prefix of the suffices i and j in the suffix array
    inline int longest_common_prefix(int i, int j)
    {
        if (i == j)
            return n - i - 1;
        int l = min(c[i], c[j]), r = max(c[i], c[j]), len = r - l;
        int k = 31 - __builtin_clz(len);
        return min(mn[l + 1][k], mn[r - (1 << k) + 1][k]);
    }
    // return the kth smallest substring in the string
    inline string kth_substring(ll k, string s, vector<int> p)
    {
        auto pos = p, cur = p;
        int piv = 0, n = sz(s);
        while (k)
        {
            char c = s[cur[piv]];           // the character of the pivot
            int diff = cur[piv] - pos[piv]; // the difference between the current position and the pivot
            for (int i = piv; i < sz(pos) && k; i++)
            {
                if (cur[i] < sz(s) and c == s[cur[i]])
                { // if the character is the same as the pivot
                    if (i == piv)
                        cur[i]++, k--; // if it's the pivot increment the current position
                    else if (i != piv && (cur[i] - pos[i]) == diff)
                        cur[i]++, k--;
                    else
                        break;
                }
                else
                    break;
            }
            if (!k)
                return s.substr(pos[piv], cur[piv] - pos[piv]);
            while (cur[piv] == n)
                piv++;
        }
        return "No such line.";
    }
};

void solve(int tc)
{
    string s;
    cin >> s;
    SuffixArray sa(s);
    for (auto x : sa.p)
    {
        cout << x << sp;
    }
    cout << nl;
}
signed main(void)
{
    fastio();
    int tc = 1;
    // cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}