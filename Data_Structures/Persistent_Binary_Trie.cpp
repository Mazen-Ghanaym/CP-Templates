#include <bits/stdc++.h>
using namespace std;
#define nl "\n"

// --- Persistent Binary Trie ---
class PersistentBinaryTrie {
private:
    static const int BITS = 19;

    struct Node {
        int children[2];
        int count;
        Node() : count(0) {
            children[0] = children[1] = 0; // Index 0 is the sentinel null node.
        }
    };

    vector<Node> node_pool;
    vector<int> roots;
    int node_idx;

    // Clones a node `u` (by index) and returns the index of the new node.
    int clone_node(int u) {
        node_pool[node_idx] = node_pool[u];
        return node_idx++;
    }

    // Inserts a value into the trie starting from a given root.
    int insert_internal(int root_idx, int val) {
        int new_root = clone_node(root_idx);
        node_pool[new_root].count++;

        int curr_new = new_root;
        int curr_old = root_idx;

        for (int i = BITS - 1; i >= 0; --i) {
            int bit = (val >> i) & 1;
            int old_child_idx = node_pool[curr_old].children[bit];
            int new_child_idx = clone_node(old_child_idx);
            
            node_pool[curr_new].children[bit] = new_child_idx;
            node_pool[new_child_idx].count++;

            curr_new = new_child_idx;
            curr_old = old_child_idx;
        }
        return new_root;
    }

    int query_kth(int l_root_idx, int r_root_idx, int k) {
        int val = 0;
        for (int i = BITS - 1; i >= 0; --i) {
            int left_count = node_pool[node_pool[r_root_idx].children[0]].count - 
                             node_pool[node_pool[l_root_idx].children[0]].count;
            
            if (k <= left_count) {
                r_root_idx = node_pool[r_root_idx].children[0];
                l_root_idx = node_pool[l_root_idx].children[0];
            } else {
                k -= left_count;
                val |= (1 << i);
                r_root_idx = node_pool[r_root_idx].children[1];
                l_root_idx = node_pool[l_root_idx].children[1];
            }
        }
        return val;
    }

    int query_count_le(int l_root_idx, int r_root_idx, int x) {
        int count = 0;
        for (int i = BITS - 1; i >= 0; --i) {
            int bit = (x >> i) & 1;
            if (bit == 1) {
                count += node_pool[node_pool[r_root_idx].children[0]].count - 
                         node_pool[node_pool[l_root_idx].children[0]].count;
            }
            r_root_idx = node_pool[r_root_idx].children[bit];
            l_root_idx = node_pool[l_root_idx].children[bit];
        }
        count += node_pool[r_root_idx].count - node_pool[l_root_idx].count;
        return count;
    }

    int query_max_xor(int l_root_idx, int r_root_idx, int x) {
        int y = 0;
        for (int i = BITS - 1; i >= 0; --i) {
            int bit = (x >> i) & 1;
            int required_bit = 1 - bit;

            int r_child_idx = node_pool[r_root_idx].children[required_bit];
            int l_child_idx = node_pool[l_root_idx].children[required_bit];
            
            if (node_pool[r_child_idx].count > node_pool[l_child_idx].count) {
                y |= (required_bit << i);
                r_root_idx = r_child_idx;
                l_root_idx = l_child_idx;
            } else {
                y |= (bit << i);
                r_root_idx = node_pool[r_root_idx].children[bit];
                l_root_idx = node_pool[l_root_idx].children[bit];
            }
        }
        return y;
    }

public:
    PersistentBinaryTrie(int max_queries) {
        node_pool.resize(max_queries * (BITS + 2));
        node_idx = 1; // node 0 is the sentinel null node
        roots.reserve(max_queries + 1);
        roots.push_back(0);
    }
    void add(int x) {
        roots.push_back(insert_internal(roots.back(), x));
    }

    void rollback(int k) {
        int count = min(k, (int)roots.size() - 1);
        roots.resize(roots.size() - count);
    }

    int get_max_xor(int l, int r, int x) {
        return query_max_xor(roots[l - 1], roots[r], x);
    }

    int get_count_le(int l, int r, int x) {
        return query_count_le(roots[l - 1], roots[r], x);
    }

    int get_kth(int l, int r, int k) {
        int total_in_range = node_pool[roots[r]].count - node_pool[roots[l-1]].count;
        if (k > total_in_range || k <= 0) return -1;
        return query_kth(roots[l - 1], roots[r], k);
    }
};


void solve() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    PersistentBinaryTrie pbt(q);

    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            int x;
            cin >> x;
            pbt.add(x);
        } else if (type == 1) { // On the interval L..R find a number y, to maximize (x xor y).
            int l, r, x;
            cin >> l >> r >> x;
            cout << pbt.get_max_xor(l, r, x) << nl;
        } else if (type == 2) { // delete the last k elements.
            int k;
            cin >> k;
            pbt.rollback(k);
        } else if (type == 3) { // On the interval L..R, count the number of integers less than or equal to x.
            int l, r, x;
            cin >> l >> r >> x;
            cout << pbt.get_count_le(l, r, x) << nl;
        } else if (type == 4) { // On the interval L..R, find the kth smallest integer (kth order statistic).
            int l, r, k;
            cin >> l >> r >> k;
            cout << pbt.get_kth(l, r, k) << nl;
        }
    }
}

int main() {
    solve();
    return 0;
}
