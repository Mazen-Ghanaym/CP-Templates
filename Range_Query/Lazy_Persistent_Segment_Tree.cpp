#include <bits/stdc++.h>
using namespace std;

template <typename T> struct PersistentSegTree {
    struct Node {
        T sum;
        T lazy;
        Node *left, *right;
        Node(T val = 0) : sum(val), lazy(0), left(nullptr), right(nullptr) {}
    };

    int n;
    // Range of indices is assumed [1, n]
    Node *root;
    vector<Node *> versions = {new Node()};
    // Build persistent seg tree from vector a (1-indexed: we use a[0..n-1] for positions 1..n)
    PersistentSegTree(const vector<T> &a) : n(a.size()) {
        root = build(a, 1, n);
        versions[0] = root;
    }

    // Build function returns a new node representing segment [l, r]
    Node *build(const vector<T> &a, int l, int r) {
        Node *node = new Node();
        if (l == r) {
            node->sum = a[l - 1]; // converting 1-indexed segment index to 0-indexed vector
        } else {
            int mid = (l + r) / 2;
            node->left = build(a, l, mid);
            node->right = build(a, mid + 1, r);
            node->sum = node->left->sum + node->right->sum;
        }
        return node;
    }

    // Propagate lazy tag to children (clones children nodes for persistence)
    void push(Node *node, int l, int r) {
        if (node->lazy != 0 && l != r) {
            int mid = (l + r) / 2;
            node->left = new Node(*node->left);
            node->right = new Node(*node->right);
            node->left->lazy += node->lazy;
            node->right->lazy += node->lazy;
            node->left->sum += node->lazy * (mid - l + 1);
            node->right->sum += node->lazy * (r - mid);
            node->lazy = 0;
        }
    }

    // Update returns a new version of the node after adding val to [ql, qr]
    Node *update(Node *prev, int l, int r, int ql, int qr, T val) {
        if (r < ql || l > qr) return prev;
        Node *cur = new Node(*prev);
        if (ql <= l && r <= qr) {
            cur->lazy += val;
            cur->sum += val * (r - l + 1);
            return cur;
        }
        push(cur, l, r);
        int mid = (l + r) / 2;
        cur->left = update(cur->left, l, mid, ql, qr, val);
        cur->right = update(cur->right, mid + 1, r, ql, qr, val);
        cur->sum = cur->left->sum + cur->right->sum;
        return cur;
    }

    // Query the sum on the interval [ql, qr] in the tree rooted at node
    T query(Node *node, int l, int r, int ql, int qr) {
        if (r < ql || l > qr) return 0;
        if (ql <= l && r <= qr) return node->sum;
        push(node, l, r);
        int mid = (l + r) / 2;
        return query(node->left, l, mid, ql, qr) + query(node->right, mid + 1, r, ql, qr);
    }

    // Wrapper for update (returns new root)
    void range_update(int l, int r, T val) {
        Node *new_root = update(versions.back(), 1, n, l, r, val);
        versions.push_back(new_root);
    }

    // Wrapper for query on the root
    T range_query(int l, int r, int cur_version = -1) {
        if (cur_version == -1) cur_version = versions.size() - 1;
        return query(versions[cur_version], 1, n, l, r);
    }

    // rollback to version v
    void rollback(int v) { versions.resize(v + 1); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // Build initial persistent segment tree version from array a
    PersistentSegTree<long long> pst(a);

    int currentVersion = 0;

    while (Q--) {
        char op;
        cin >> op;
        if (op == 'C') {
            int l, r;
            long long d;
            cin >> l >> r >> d;
            pst.range_update(l, r, d);
            currentVersion++;
        } else if (op == 'Q') {
            int l, r;
            cin >> l >> r;
            cout << pst.range_query(l, r) << "\n";
        } else if (op == 'H') {
            int l, r, t;
            cin >> l >> r >> t;
            cout << pst.range_query(l, r, t) << "\n";
        } else if (op == 'B') {
            int t;
            cin >> t;
            currentVersion = t;
            pst.rollback(t);
        }
    }

    return 0;
}
