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

struct BinaryTrie {

    struct Node {
        Node* child[2];
        int freq;
        Node(){
            memset(child, 0, sizeof(child));
            freq = 0;
        }
    };

    Node* root;
    BinaryTrie(){
        root = new Node;
        // insert(0); // Insert 0 to handle the case of XOR with 0
    }
    
    void insert(const int n){
        // if(search(n)) return; // insert only once
        Node* curr = root;
        for (int i = 40; i >= 0; i--){
            int bit = (n >> i) & 1;
            if(!curr -> child[bit]) curr -> child[bit] = new Node;
            curr = curr -> child[bit];
            curr -> freq++;
        }
    }

    bool search(const int n){
        Node* curr = root;
        for (int i = 40; i >= 0; i--){
            int bit = (n >> i) & 1;
            if(!curr -> child[bit]) return false;
            curr = curr -> child[bit];
        }
        return true;
    }

    void erase(const int n, int idx, Node* curr){
        if(idx == -1){
            return;
        }
        int bit = (n >> idx) & 1;
        if (!curr->child[bit]) return;
        erase(n, idx - 1, curr->child[bit]);
        curr->child[bit]->freq--;
        if(curr->child[bit]->freq == 0){
            delete curr->child[bit];
            curr->child[bit] = nullptr;
        }
    }
    void erase(const int n){
        if(!search(n)) return; // erase only if it exists
        erase(n, 40, root);
    }
    int max_xor(const int n){
        Node *curr = root;
        int ans = 0;
        for (int i = 40; i >= 0; i--){
            if (!curr) break;
            int bit = (n >> i) & 1;
            if(curr -> child[!bit]){
                ans |= (1LL << i);
                curr = curr -> child[!bit];
            }else{
                curr = curr -> child[bit];
            }
        }
        return ans;
    }
    int min_xor(const int n){
        Node *curr = root;
        int ans = 0;
        for (int i = 40; i >= 0; i--){
            if (!curr) break;
            int bit = (n >> i) & 1;
            if(curr -> child[bit]){
                curr = curr -> child[bit];
            }else{
                ans |= (1LL << i);
                curr = curr -> child[!bit];
            }
        }
        return ans;
    }
    // count the number of elements with (pref ^ x) >= k
    int cnt_sub(int pref, int k){
        Node* curr = root;
        int ans = 0;
        int curr_xor = 0;
        for (int i = 40; i >= 0; i--){
            if(!curr) break;
            bool bit = (1LL << i) & pref;
            if((curr_xor | (1LL << i)) >= k){
                ans += curr -> child[bit ^ 1] ? curr -> child[bit ^ 1] -> freq : 0;
                curr = curr -> child[bit];
            }else{
                curr_xor |= (1LL << i);
                curr = curr -> child[bit ^ 1];
            }
        }
        if(curr_xor >= k) ans += curr -> freq;
        return ans;
    }
};
void solve()
{
    int n;
    cin >> n;
    BinaryTrie trie;
    while (n--)
    {
        int type, x;
        cin >> type >> x;
        if (type == 0) {
            trie.insert(x);
        } else if (type == 1) {
            trie.erase(x);
        } else if (type == 2) {
            cout << trie.min_xor(x) << nl;
        }
    }
}
signed main(void)
{
    int tc = 1;
    //cin >> tc;
    while (tc--)
    {
        solve();
    }
    return 0;
}