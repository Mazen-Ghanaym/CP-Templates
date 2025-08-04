#include <bits/stdc++.h>
#define ll long long
#define nl "\n"
using namespace std;

// --- Trie with Sorted Index Vectors ---
const int ALPHABET_SIZE = 26;

// Each node in the Trie holds sorted vectors to track indices.
struct TrieNode {
    unique_ptr<TrieNode> children[ALPHABET_SIZE];
    vector<int> prefix_indices;
    vector<int> word_indices;
};

class Trie {
private:
    unique_ptr<TrieNode> root;

    // Helper to perform insertions and removals.
    // `delta` is +1 for insert, -1 for remove.
    void modify(const string& word, int index, int delta) {
        TrieNode* current = root.get();
        for (char ch : word) {
            int char_idx = ch - 'a';
            if (!current->children[char_idx]) {
                current->children[char_idx] = make_unique<TrieNode>();
            }
            current = current->children[char_idx].get();
            
            // Add or remove the index from the prefix list
            if (delta == 1) {
                current->prefix_indices.insert(
                    lower_bound(current->prefix_indices.begin(), current->prefix_indices.end(), index),
                    index
                );
            } else { // delta == -1
                auto it = lower_bound(current->prefix_indices.begin(), current->prefix_indices.end(), index);
                if (it != current->prefix_indices.end() && *it == index) {
                    current->prefix_indices.erase(it);
                }
            }
        }
        
        // Add or remove the index from the word-end list
        if (delta == 1) {
            current->word_indices.insert(
                lower_bound(current->word_indices.begin(), current->word_indices.end(), index),
                index
            );
        } else { // delta == -1
            auto it = lower_bound(current->word_indices.begin(), current->word_indices.end(), index);
            if (it != current->word_indices.end() && *it == index) {
                current->word_indices.erase(it);
            }
        }
    }

public:
    Trie() {
        root = make_unique<TrieNode>();
    }

    // Inserts word `s` from array index `idx`.
    void insert(const string& s, int idx) {
        modify(s, idx, 1);
    }

    // Removes word `s` from array index `idx`.
    void remove(const string& s, int idx) {
        modify(s, idx, -1);
    }
    // count how many words in v[l...r] start with `s`
    int count_prefix(int l, int r, const string& s) {
        TrieNode* current = root.get();
        for (char ch : s) {
            int char_idx = ch - 'a';
            if (!current->children[char_idx]) {
                return 0; // No words with this prefix
            }
            current = current->children[char_idx].get();
        }
        // Count how many indices in the prefix list are in the range [l, r].
        return upper_bound(current->prefix_indices.begin(), current->prefix_indices.end(), r) -
               lower_bound(current->prefix_indices.begin(), current->prefix_indices.end(), l);
    }

    // Query Type 3: Is `s` a prefix of any string in v[l...r]?
    bool sprefix(int l, int r, const string& s) {
        TrieNode* current = root.get();
        for (char ch : s) {
            int char_idx = ch - 'a';
            if (!current->children[char_idx]) {
                return false;
            }
            current = current->children[char_idx].get();
        }
        // Check if any index in the prefix list is in the range [l, r].
        auto it = lower_bound(current->prefix_indices.begin(), current->prefix_indices.end(), l);
        return (it != current->prefix_indices.end() && *it <= r);
    }

    // Query Type 2: Is any string in v[l...r] a prefix of `s`?
    bool vprefix(int l, int r, const string& s) {
        TrieNode* current = root.get();
        for (char ch : s) {
            // At each prefix of `s`, check if it exists as a complete word in the range [l, r].
            auto it = lower_bound(current->word_indices.begin(), current->word_indices.end(), l);
            if (it != current->word_indices.end() && *it <= r) {
                return true;
            }
            
            int char_idx = ch - 'a';
            if (!current->children[char_idx]) {
                return false;
            }
            current = current->children[char_idx].get();
        }
        // Check the full string `s` itself.
        auto it = lower_bound(current->word_indices.begin(), current->word_indices.end(), l);
        return (it != current->word_indices.end() && *it <= r);
    }
};

void solve() {
    int n;
    cin >> n;

    vector<string> v(n + 1);
    Trie trie;

    for (int i = 1; i <= n; ++i) {
        cin >> v[i];
        trie.insert(v[i], i);
    }

    int q;
    cin >> q;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int i;
            string s;
            cin >> i >> s;
            trie.remove(v[i], i);
            v[i] = s;
            trie.insert(v[i], i);
        } else if (type == 2) {
            int l, r;
            string s;
            cin >> l >> r >> s;
            if (trie.vprefix(l, r, s)) {
                cout << "Y" << nl;
            } else {
                cout << "N" << nl;
            }
        } else { // type == 3
            int l, r;
            string s;
            cin >> l >> r >> s;
            if (trie.sprefix(l, r, s)) {
                cout << "Y" << nl;
            } else {
                cout << "N" << nl;
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
