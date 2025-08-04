#include <bits/stdc++.h>
using namespace std;

#define nl "\n"
/*
 • Add x y–Adds a new node to the tree as a child of node x. The newly added node and node x are
 connected with an edge of weight y. The newly added node is denoted by a number equal to the
 number of nodes that the tree consists of after its addition.
 • Query a b– Finds the longest path in a tree which starts in node a and ends in some node from
 the subtree of node b (which itself is considered to be in its own subtree). The length of the path is
 defined as exclusive or (xor) of weights of all edges that the path consists of.
*/
static const int BITS = 30;

// The Trie structure, where each node holds a set of indices.
struct Trie
{
    struct Node
    {
        Node *children[2];
        set<int> ids;
        Node() : children{nullptr, nullptr} {}
    };

    Node *root;

    Trie() { root = new Node(); }

    // Inserts the pre-calculated XOR sum for a given node `u`
    // by adding its `in_time` to the sets along the path.
    void insert(int in_time, int xor_sum)
    {
        Node *curr = root;
        for (int i = BITS - 1; i >= 0; --i)
        {
            int bit = (xor_sum >> i) & 1;
            if (!curr->children[bit])
            {
                curr->children[bit] = new Node();
            }
            curr = curr->children[bit];
            curr->ids.insert(in_time);
        }
    }

    // Finds the max XOR of `val` with any node in the subtree of `u`.
    int query_max(int val, int in_u, int out_u)
    {
        Node *curr = root;
        int best_partner_xor = 0;
        for (int i = BITS - 1; i >= 0; --i)
        {
            int bit = (val >> i) & 1;
            int required_bit = 1 - bit;

            // Check if a valid partner exists down the desired path.
            if (curr->children[required_bit])
            {
                auto &s = curr->children[required_bit]->ids;
                auto it = s.lower_bound(in_u);
                if (it != s.end() && *it <= out_u)
                {
                    // Found a partner in the subtree range. Take this path.
                    best_partner_xor |= (required_bit << i);
                    curr = curr->children[required_bit];
                    continue;
                }
            }

            // Forced to take the other path.
            best_partner_xor |= (bit << i);
            curr = curr->children[bit];
        }
        return val ^ best_partner_xor;
    }
};

struct Query
{
    int type; // 1 for Add, 2 for Query
    int u, v;
};

vector<vector<pair<int, int>>> adj;
vector<int> dist, tin, tout;
int timer;
int node_count;

// DFS on the final tree to compute distances and in/out times.
void dfs_flattener(int u, int p, int current_xor_dist)
{
    tin[u] = ++timer;
    dist[u] = current_xor_dist;
    for (const auto &edge : adj[u])
    {
        int v = edge.first;
        int w = edge.second;
        if (v != p)
        {
            dfs_flattener(v, u, current_xor_dist ^ w);
        }
    }
    tout[u] = timer;
}

void solve()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    vector<Query> queries(q);
    adj.resize(q + 2);
    node_count = 1;

    // 1. Read all queries and build the final adjacency list.
    for (int i = 0; i < q; ++i)
    {
        string type;
        cin >> type >> queries[i].u >> queries[i].v;
        if (type == "Add")
        {
            queries[i].type = 1;
            node_count++;
            adj[queries[i].u].push_back({node_count, queries[i].v});
        }
        else
        {
            queries[i].type = 2;
        }
    }

    // 2. Perform one DFS on the final tree.
    dist.resize(node_count + 1);
    tin.resize(node_count + 1);
    tout.resize(node_count + 1);
    timer = 0;
    dfs_flattener(1, 0, 0);

    // 3. Process queries chronologically, inserting into the Trie as we go.
    Trie trie;
    int current_nodes_added = 1;
    trie.insert(tin[1], dist[1]);

    for (const auto &query : queries)
    {
        if (query.type == 1)
        { // Add
            current_nodes_added++;
            trie.insert(tin[current_nodes_added], dist[current_nodes_added]);
        }
        else
        { // Query
            int a = query.u;
            int b = query.v;
            cout << trie.query_max(dist[a], tin[b], tout[b]) << nl;
        }
    }
}

int main()
{
    solve();
    return 0;
}
