#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
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

/*
  ROLLBACK EERTREE (Palindromic Tree with Push/Pop)

  A data structure to track all distinct palindromic substrings efficiently.
  Main advantage: Supports UNDO operation (rollback last character).

  Core Operations:
  - push(c): append character to the string - O(1) amortized
  - pop(): remove last character (rollback) - O(1)
  - distinct_palindromes(): count distinct palindromic substrings - O(1)

  Structure:
  - Two roots:
      node 0: length = -1 (imaginary/ODD root) - parent of odd-length palindromes
      node 1: length = 0  (EVEN root - empty) - parent of even-length palindromes
  - Each node represents a distinct palindrome
  - Nodes are numbered 0, 1, 2, ... in order of creation

  Key Idea: 
  - Nodes created chronologically → rollback = undo last node creation
  - Stores history of operations to enable pop() functionality
*/
struct Eertree {
    static const int MAXN = 2000005;

    struct Edge {
        char c;   // edge character
        int to;   // destination node
        int next; // next edge in linked list
    };

    // Tree data - represents the palindromic tree structure
    int len[MAXN];    // len[i] = length of palindrome represented by node i
    int link[MAXN];   // link[i] = node of longest proper palindromic suffix of node i
    int par[MAXN];    // par[i] = parent node that node i was created from in the tree
    int head[MAXN];   // head[i] = index of first edge in adjacency list for node i
    Edge edges[MAXN * 2]; // edge storage using linked list structure
    int edge_ptr;     // pointer to next available edge slot

    // String + current state
    char s[MAXN]; // the string being processed (0-indexed)
    int last;     // node representing longest palindromic suffix of current prefix
    int sz;       // total number of nodes (including two roots: 0 and 1)
    int n;        // current string length (number of characters pushed)

    // Rollback history - enables pop() operation
    bool created[MAXN];     // created[i] = true if pushing s[i] created a new node
    int parent[MAXN];       // parent[i] = which node had edge added when s[i] was pushed
    int last_history[MAXN]; // last_history[i] = value of 'last' after pushing s[i]

    // Initialize the palindromic tree with two roots
    // MUST call before using the structure
    void init() {
        edge_ptr = 0;
        sz = 2;      // start with 2 roots
        n = 0;       // empty string
        last = 1;    // longest palindromic suffix of empty string is empty (node 1)

        // Node 0: length -1 (ODD root - imaginary palindrome)
        // Used as parent for single characters and odd-length palindromes
        len[0] = -1;
        link[0] = 0;  // points to itself
        head[0] = -1; // no edges initially
        
        // Node 1: length 0 (EVEN root - empty palindrome)
        // Used as parent for even-length palindromes
        len[1] = 0;
        link[1] = 0;  // points to itself
        head[1] = -1; // no edges initially
    }

    /* ---------- Edge Management (Adjacency List via Linked List) ---------- */

    // Find edge from node u labeled with character c
    // Returns: destination node if edge exists, 0 otherwise
    int find_edge(int u, char c) {
        for (int i = head[u]; i != -1; i = edges[i].next)
            if (edges[i].c == c) return edges[i].to;
        return 0; // edge not found
    }

    // Add edge: u --c--> v
    // Adds edge to the front of u's adjacency list (constant time)
    void add_edge(int u, char c, int v) {
        edges[edge_ptr] = {c, v, head[u]}; // create new edge, point to old head
        head[u] = edge_ptr++;               // update head to new edge
    }

    // Remove edge u --c--> ? (used during rollback)
    // Like removing a node from a linked list:
    // - If edge is at head, update head pointer
    // - Otherwise, link previous edge to next edge
    void remove_edge(int u, char c) {
        int prev = -1;
        for (int i = head[u]; i != -1; i = edges[i].next) {
            if (edges[i].c == c) {
                if (prev == -1)
                    head[u] = edges[i].next;
                else
                    edges[prev].next = edges[i].next;
                return;
            }
            prev = i;
        }
    }

    /* ---------- Core Palindrome Logic ---------- */

    // Find the node representing the largest palindromic suffix that can be extended by s[pos]
    // Strategy: Keep following suffix links until we find a palindrome P where s[pos] matches
    // the character before P, allowing us to form (s[pos] + P + s[pos])
    int get_link(int v, int pos) {
        while (true) {
            int m = pos - 1 - len[v]; // position before the palindrome v
            if (m >= 0 && s[m] == s[pos]) return v; // found extendable palindrome
            v = link[v]; // try shorter palindromic suffix
        }
    }

    // Add character c to the end of the string
    // Updates the tree structure and tracks history for rollback
    void push(char c) {
        s[n] = c; // store character
        
        // Step 1: Find which palindrome we can extend
        int cur = get_link(last, n); // longest palindromic suffix we can extend
        int nxt = find_edge(cur, c); // check if palindrome (c + cur + c) already exists

        if (!nxt) {
            // New distinct palindrome found! Create a new node
            int now = sz++;
            len[now] = len[cur] + 2; // new palindrome = c + cur + c
            head[now] = -1;          // no children yet
            par[now] = cur;          // remember parent for tree structure
            
            // Step 2: Set suffix link (longest proper palindromic suffix)
            if (len[now] == 1)
                link[now] = 1; // single character's suffix is empty palindrome (node 1)
            else
                // We're extending 'cur' by character 'c' to create 'now'
                // To find the suffix link: start from cur's suffix link and find the same extension
                link[now] = find_edge(get_link(link[cur], n), c); // find suffix palindrome

            add_edge(cur, c, now);   // add edge: cur --c--> now
            parent[n] = cur;         // save for rollback
            created[n] = true;       // mark that we created a node
            nxt = now;
        } else {
            // Palindrome already exists in tree, just update pointer
            created[n] = false; // no new node created
        }

        last = nxt;              // update longest palindromic suffix
        last_history[n] = last;  // save for rollback
        n++;                     // increment string length
    }

    // Remove the last character (rollback/undo operation)
    // Reverses the effect of the last push() call
    void pop() {
        if (n == 0) return; // nothing to remove

        n--; // decrease string length
        
        if (created[n]) {
            // If pushing s[n] created a new node, undo it:
            remove_edge(parent[n], s[n]); // remove the edge that was added
            sz--;                          // decrease node count
        }
        // else: node already existed, nothing to undo
        
        // Restore 'last' pointer to previous state
        last = (n == 0 ? 1 : last_history[n - 1]);
    }

    /* ---------- Query Operations ---------- */

    // Returns: number of distinct palindromic substrings in current string
    // Excludes the two root nodes (0 and 1), which are structural, not actual palindromes
    int distinct_palindromes() const { return sz - 2; }
    
    // Optional: Get length of palindrome represented by node i
    int get_length(int node) const { return len[node]; }
} et; // Global instance

void solve() {
    string s;
    cin >> s;
    
    et.init(); // Initialize the palindromic tree
    
    // Build the tree by adding characters one by one
    for(char c : s) {
        et.push(c);
    }
    
    // Output 1: Total count of distinct palindromic substrings
    cout << et.distinct_palindromes() << nl;
    
    // Output 2: For each node (starting from node 2), print parent and suffix link
    // Nodes 0,1 are roots. Real palindromes start from node 2.
    // Format: (parent_id - 1) (suffix_link_id - 1)
    // Subtracting 1 to convert from 0-indexed to human-readable format
    for(int i = 2; i < et.sz; i++) {
        cout << et.par[i] - 1 << sp << et.link[i] - 1 << nl;
    }
    
    // Output 3: History of 'last' pointer after each character
    // Shows which node was the longest palindromic suffix after adding each character
    for(int i = 0; i < sz(s); i++) {
        cout << et.last_history[i] - 1 << " ";
    }
    cout << nl;
}

signed main(void) {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    while (tc--) {
        solve();
    }
    return 0;
}



// Counting Frequencies of Each Palindromic Substring (OFFLINE METHOD)
/*  This method counts how many times each DISTINCT palindrome appears,
  and can also compute the total count of ALL palindromic substrings (including duplicates).
  
  We maintain a frequency array `cnt[]` where:
  - cnt[i] = number of occurrences of the palindrome represented by node i
  
  After building the tree and calling compute_freq():
  - Each cnt[i] gives the frequency of palindrome i
  - Sum of all cnt[i] gives the total number of palindromic substrings (with duplicates)
  
  Note: The number of DISTINCT palindromes = sz - 2 (excluding the two roots)
*/
struct Eertree_Offline {
    // ... (rest of your existing variables) ...
    int cnt[MAXN]; // cnt[i] = how many times palindrome i appears

    void init() {
        // ... (your existing init) ...
        memset(cnt, 0, sizeof(cnt));
    }

    void push(char c) {
        // ... (your existing push logic) ...
        
        // After finding 'nxt' (the node for the current longest palindromic suffix):
        last = nxt;
        cnt[last]++; // Increment the count for the longest suffix ending here
        
        last_history[n] = last;
        n++;
    }

    void compute_freq() {
        // Iterate backwards from the last created node to the first
        // This ensures we process children (longer palindromes) before parents (suffixes)
        for (int i = sz - 1; i >= 2; i--) {
            cnt[link[i]] += cnt[i];
        }
    }
    
    // Total Number of Palindromic Substrings (including duplicates)
    // Call this AFTER compute_freq()
    int total_palindromic_substrings() {
        int total = 0;
        for (int i = 2; i < sz; i++) {
            total += cnt[i];
        }
        return total;
    }
};

// Online Counting of Duplicate Palindromic Substrings (CORRECT METHOD)
/*  To maintain the count of palindromic substrings including duplicates
  in an online manner (after each push), we use the suffix link chain depth.
  
  Key Insight: When we add a character, the number of NEW palindromic substrings
  ending at this position equals the depth of the current node in the suffix link tree.
  
  Formula: num[node] = num[link[node]] + 1
  
  Example for "ababa":
  - After 'a': creates node for "a", depth=1, total=1
  - After 'ab': creates node for "b", depth=1, total=2
  - After 'aba': creates node for "aba", depth=2 (chain: aba->a), total=4
  - After 'abab': creates node for "bab", depth=2 (chain: bab->b), total=6
  - After 'ababa': creates node for "ababa", depth=3 (chain: ababa->aba->a), total=9
*/
struct Eertree_Online {
    // ... (rest of your existing variables) ...
    int num[MAXN];  // num[i] = depth in suffix link tree = # of palindromic suffixes
    int total_count; // running total of all palindromic substrings (including duplicates)
    
    void init() {
        // ... (your existing init) ...
        num[0] = num[1] = 0; // roots have depth 0
        total_count = 0;
    }
    
    void push(char c) {
        s[n] = c;
        
        int cur = get_link(last, n);
        int nxt = find_edge(cur, c);

        if (!nxt) {
            int now = sz++;
            len[now] = len[cur] + 2;
            head[now] = -1;
            par[now] = cur;
            
            if (len[now] == 1)
                link[now] = 1;
            else
                link[now] = find_edge(get_link(link[cur], n), c);

            // CRITICAL: Calculate depth in suffix link tree
            num[now] = num[link[now]] + 1;
            
            add_edge(cur, c, now);
            parent[n] = cur;
            created[n] = true;
            nxt = now;
        } else {
            created[n] = false;
        }

        last = nxt;
        
        // Add the count of ALL palindromes ending at current position
        total_count += num[last];
        
        last_history[n] = last;
        n++;
    }
    
    void pop() {
        if (n == 0) return;
        
        // Subtract the palindromes that were added at position n-1
        total_count -= num[last];
        
        n--;
        
        if (created[n]) {
            remove_edge(parent[n], s[n]);
            sz--;
        }
        
        last = (n == 0 ? 1 : last_history[n - 1]);
    }
    
    int get_total_count() const {
        return total_count;
    }
};

/* 
Usage Examples:

1. OFFLINE - Count occurrences of each distinct palindrome AND total count:
   Eertree_Offline et;
   et.init();
   for(char c : s) et.push(c);
   et.compute_freq();
   
   Get frequency of each distinct palindrome:
   for(int i = 2; i < et.sz; i++) {
       cout << "Palindrome " << i << " appears " << et.cnt[i] << " times\n";
   }
   
   Get total palindromic substrings (including duplicates):
   cout << "Total: " << et.total_palindromic_substrings() << nl;
   
   Get count of distinct palindromes:
   cout << "Distinct: " << (et.sz - 2) << nl;

2. ONLINE - Track total palindromic substrings dynamically (including duplicates):
   Eertree_Online et;
   et.init();
   for(char c : s) {
       et.push(c);
       cout << et.get_total_count() << sp; // total palindromes (with duplicates) after each character
   }
   
   Note: Online method gives total count, not distinct count
   For distinct count at any time, use: et.sz - 2
*/