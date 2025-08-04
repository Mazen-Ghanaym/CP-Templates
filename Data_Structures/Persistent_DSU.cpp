#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 7;


template <typename Type, int LOG>
struct PersistentArray {
    struct Node {
        Type data;
        Node* child[1 << LOG] = {};
        Node() {}
        Node(const Type& data_) : data(data_) {}
    };
    Node* root;

    PersistentArray() : root(nullptr) {}
    Type get(Node* t, int k) {
        if (k == 0) return t->data;
        return get(t->child[k & ((1 << LOG) - 1)], k >> LOG);
    }

    Type get(const int& k) {
        return get(root, k);
    }

    pair<Node*, Type*> mutable_get(Node* t, int k) {
        t = t ? new Node(*t) : new Node();
        if (k == 0) return {t, &t->data};
        auto p = mutable_get(t->child[k & ((1 << LOG) - 1)], k >> LOG);
        t->child[k & ((1 << LOG) - 1)] = p.first;
        return {t, p.second};
    }

    Type* mutable_get(const int& k) {
        auto ret = mutable_get(root, k);
        root = ret.first;
        return ret.second;
    }

    Node* build(Node* t, const Type& data, int k) {
        if (!t) t = new Node();
        if (k == 0) {
            t->data = data;
            return t;
        }
        auto p = build(t->child[k & ((1 << LOG) - 1)], data, k >> LOG);
        t->child[k & ((1 << LOG) - 1)] = p;
        return t;
    }

    void build(const vector<Type>& vec) {
        root = nullptr;
        for(size_t i=0; i<vec.size(); i++) {
            root = build(root, vec[i], i);
        }
    }
};

// Fully Persistent Union-Find
struct PersistentUnionFind {
    PersistentArray<int, 3> data;
    PersistentUnionFind() {}
    PersistentUnionFind(int size_) {
        data.build(vector<int>(size_, -1));
    }

    int find(int k) {
        int p = data.get(k);
        return p >= 0 ? find(p) : k;
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    int size(int k) {
        return (-data.get(find(k)));
    }

    PersistentUnionFind unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return *this;
        auto u = data.get(x);
        auto v = data.get(y);
        if (u > v) swap(u, v), swap(x, y);

        auto a = data.mutable_get(x);
        *a += v;
        auto b = data.mutable_get(y);
        *b = x;
        return *this;
    }
};

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int n, q;
    cin >> n >> q;
    vector<PersistentUnionFind> his(q + 1);
    his[0] = PersistentUnionFind(n);
    for (int i = 1; i <= q; ++i) {
        int tp, k, u, v;
        cin >> tp >> k >> u >> v;
        ++k;
        if (tp == 0) {
            his[i] = his[k];
            his[i].unite(u, v);
        }
        if (tp == 1) {
            cout << his[k].same(u, v) << '\n';
        }
    }

    return 0;
}
