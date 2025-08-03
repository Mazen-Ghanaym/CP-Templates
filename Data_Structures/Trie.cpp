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
struct Trie
{
    struct Node
    {

        Node *child[26];
        bool is_word;
        int freq;

        Node()
        {
            memset(child, 0, sizeof(child));
            is_word = false;
            freq = 0;
        }
    };

    Node *root;
    char DEFAULT;

    Trie()
    {
        root = new Node;
        DEFAULT = 'a';
    }

    void insert(const string &word)
    {
        Node *curr = root;
        for (auto &c : word)
        {
            if (!curr->child[c - DEFAULT])
                curr->child[c - DEFAULT] = new Node;
            curr = curr->child[c - DEFAULT];
            curr->freq++;
        }
        curr->is_word = true;
    }

    void erase(const string &word, int idx, Node *curr)
    {
        if (idx == sz(word))
            return void(curr->is_word = curr->freq > 1);
        erase(word, idx + 1, curr->child[word[idx] - DEFAULT]);
        if (--curr->child[word[idx] - DEFAULT]->freq == 0)
        {
            delete curr->child[word[idx] - DEFAULT];
            curr->child[word[idx] - DEFAULT] = nullptr;
        }
    }

    bool search(const string &word)
    {
        Node *curr = root;
        for (auto &c : word)
        {
            if (!curr->child[c - DEFAULT])
                return false;
            curr = curr->child[c - DEFAULT];
        }
        return curr->is_word;
    }

    void erase(const string &word)
    {
        if (search(word))
            erase(word, 0, root);
    }

    bool is_prefix(const string &word)
    {
        Node *curr = root;
        for (auto &c : word)
        {
            if (!curr->child[c - DEFAULT])
                return false;
            curr = curr->child[c - DEFAULT];
        }
        return true;
    }

    int count(const string &word)
    {
        Node *curr = root;
        for (auto &c : word)
        {
            if (!curr->child[c - DEFAULT])
                return 0;
            curr = curr->child[c - DEFAULT];
        }
        return curr->freq;
    }
    ~Trie()
    {
        function<void(Node *)> deleteNode = [&](Node *node)
        {
            if (!node)
                return;
            for (int i = 0; i < 26; ++i)
            {
                deleteNode(node->child[i]);
            }
            delete node;
        };
        deleteNode(root);
    }
};
void solve(int tc)
{
    int n, q;
    cin >> n >> q;
    Trie trie;
    while (n--)
    {
        string s;
        cin >> s;
        trie.insert(s);
    }
    while (q--)
    {
        string s;
        cin >> s;
        cout << trie.count(s) << nl;
    }
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
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