#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ld long double
// #define int long long
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
  MEMORY EFFICIENT EERTREE (Palindromic Tree)

  A lightweight data structure for tracking distinct palindromic substrings.
  Optimized for memory constraints.

  Core Operations:
  - init(): initialize the palindromic tree with two roots - O(1)
  - extend(c): append character to string, update tree - O(1) amortized
  - distinct_palindromes(): count distinct palindromic substrings - O(1)

  Structure:
  - Two roots:
      node 0: length = -1 (ODD root - imaginary/boundary)
      node 1: length = 0  (EVEN root - empty palindrome)
  - Each node represents ONE distinct palindrome
  - Nodes numbered 0, 1, 2, ... in order of first appearance

  Use Cases:
  - Binary alphabet (a/b, 0/1) palindrome problems
  - Strict memory limits (< 100MB)
  - Online palindrome counting (distinct only)

  You can extend to handle more characters by changing nxt[] to a map or larger array.

*/
struct Eertree {
    static const int MAXN = 5000005; // Handles strings up to 5M characters
    static const int SIGMA = 2;      // Alphabet size (2 for binary: 'a'/'b' or 0/1)

    struct Node {
        int nxt[SIGMA]; // Transitions: nxt[0] for 'a'/0, nxt[1] for 'b'/1
        int len;        // Length of the palindrome this node represents
        int link;       // Suffix link: longest proper palindromic suffix
    };

    Node t[MAXN]; // Node array (global to save memory)
    int sz;       // Total number of nodes (including 2 roots)
    int last;     // Node representing longest palindromic suffix of current string
    int n;        // Current string length (number of characters pushed)
    char s[MAXN]; // String storage: s[0..n-1] = actual string (0-indexed)

    // Initialize the palindromic tree with two structural roots
    // MUST call before processing any string
    void init() {
        sz = 2;   // Start with 2 roots
        last = 1; // Initially points to EVEN root (empty string)
        n = 0;    // Empty string (string is 0-indexed: s[0..n-1])

        // Node 0: ODD Root - imaginary palindrome (length -1)
        // Used as boundary condition and parent of single characters
        t[0].len = -1;
        t[0].link = 0;                 // Points to itself
        t[0].nxt[0] = t[0].nxt[1] = 0; // No transitions initially

        // Node 1: EVEN Root - represents empty palindrome (length 0)
        // Parent of even-length palindromes when extended
        t[1].len = 0;
        t[1].link = 0;                 // Its suffix is the ODD root (imaginary)
        t[1].nxt[0] = t[1].nxt[1] = 0; // No transitions initially
    }

    /* ---------- Core Palindrome Logic ---------- */

    // Find the longest palindromic suffix of s[0..n-1] that can be extended by s[pos]
    // Strategy: Follow suffix links until we find palindrome P where:
    //   - s[pos - 1 - len[P]] exists and equals s[pos]
    //   - This allows us to form: s[pos] + P + s[pos]
    int get_link(int v, int pos) {
        while (true) {
            int m = pos - 1 - t[v].len;             // position before the palindrome v
            if (m >= 0 && s[m] == s[pos]) return v; // found extendable palindrome
            v = t[v].link;                          // try shorter palindromic suffix
        }
    }

    // Extend the string by appending character c (must be in range [0, SIGMA-1])
    // For letters: call extend(c - 'a') where c is 'a'..'b'
    // Returns: true if a NEW distinct palindrome was created, false otherwise
    bool extend(int c) {
        assert(c >= 0 && c < SIGMA); // Ensure valid alphabet range
        s[n] = (char)c;              // Store character at s[n] (0-indexed)
        int cur = get_link(last, n); // Find extendable palindrome

        // Check if palindrome (c + cur + c) already exists
        if (!t[cur].nxt[c]) {
            // New distinct palindrome! Create a node for it
            int now = sz++;
            t[now].len = t[cur].len + 2;       // Length = old palindrome + 2 end characters
            t[now].nxt[0] = t[now].nxt[1] = 0; // No children yet

            // Set suffix link (longest proper palindromic suffix)
            if (t[now].len == 1) {
                // Single character's suffix is the empty palindrome (node 1)
                t[now].link = 1;
            } else {
                // Find the suffix: start from cur's suffix and extend by same character
                t[now].link = t[get_link(t[cur].link, n)].nxt[c];
            }

            t[cur].nxt[c] = now; // Add transition: cur --c--> now
            last = now;          // Update longest palindromic suffix
            n++;                 // Increment string length
            return true;         // NEW palindrome created
        }

        // Palindrome already exists
        last = t[cur].nxt[c]; // Update longest palindromic suffix
        n++;                  // Increment string length
        return false;         // No new palindrome
    }

    /* ---------- Query Operations ---------- */

    // Returns: number of DISTINCT palindromic substrings
    // Excludes the two structural roots (they're not actual palindromes)
    int distinct_palindromes() const { return sz - 2; }

    // Get length of palindrome represented by node i
    int get_length(int node) const { return t[node].len; }
} et; // Global instance

void solve() {
    string s;
    cin >> s;

    et.init();

    for (char c : s) {
        if (et.extend(c - 'a')) {
            cout << '1';
        } else {
            cout << '0';
        }
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