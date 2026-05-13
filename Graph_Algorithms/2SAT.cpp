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

/* ==============================================================================
 * EXPERIENCE POINT: 2-SAT (2-Satisfiability) via Tarjan's SCC
 * ==============================================================================
 * THE PROBLEM:
 * Given a boolean formula in Conjunctive Normal Form (CNF) where each clause 
 * has at most 2 literals, find if there is an assignment of variables that 
 * makes the formula TRUE, and if so, find one valid assignment.
 * * VARIABLE REPRESENTATION (0-indexed):
 * Variable 'i' (0 to N-1) is split into two nodes:
 * - True literal  (+i) -> Node (2 * i)
 * - False literal (-i) -> Node (2 * i + 1)
 * - The negation of ANY literal 'u' is simply 'u ^ 1'
 * * HOW IT WORKS:
 * 1. Build the Implication Graph. Ex: (A OR B) is (!A -> B) AND (!B -> A).
 * 2. Run Tarjan's SCC. 
 * 3. If any variable and its negation are in the same SCC, it's UNSATISFIABLE.
 * 4. ASSIGNMENT TRICK: Tarjan processes SCCs in reverse topological order.
 * Thus, assignment[i] = (scc_id[2i] < scc_id[2i+1]) magically gives a valid
 * boolean assignment without any extra topological sorts!
 * ============================================================================== */

//! zero-indexed variables, 0-based node IDs
struct TwoSat {
    int n;
    vector<vector<int>> adj;
    vector<int> dfsn, low, scc_id;
    vector<bool> instack;
    vector<int> st;
    int timer, scc_count;
    bool is_satisfiable;
    vector<bool> assignment; 

    TwoSat(int _n) {
        n = _n;
        adj.assign(2 * n, vector<int>());
        dfsn.assign(2 * n, -1);
        low.assign(2 * n, -1);
        scc_id.assign(2 * n, -1);
        instack.assign(2 * n, false);
        assignment.assign(n, false);
        timer = scc_count = 0;
        is_satisfiable = false;
    }

    // ==========================================
    // 1. RAW INTERNAL UTILITIES (Uses Node IDs)
    // ==========================================
    void add_implication(int node_u, int node_v) {
        adj[node_u].push_back(node_v);
        adj[node_v ^ 1].push_back(node_u ^ 1); 
    }

    void add_or(int node_u, int node_v) {
        add_implication(node_u ^ 1, node_v); 
    }

    void add_xor(int node_u, int node_v) {
        add_or(node_u, node_v);
        add_or(node_u ^ 1, node_v ^ 1);
    }

    void force_true(int node_u) {
        adj[node_u ^ 1].push_back(node_u); 
    }

    // ==========================================
    // 2. USER-FRIENDLY UTILITIES (Uses Variables & Boolean States)
    // Variables must be 0-indexed. 
    // state = true for (X), state = false for (!X)
    // ==========================================
    
    // Helper to get Internal Node ID safely
    int get_node(int u, bool state) {
        return 2 * u + (state ? 0 : 1);
    }

    // Example: IF (u is state_u) THEN (v is state_v)
    void add_implication(int u, bool state_u, int v, bool state_v) {
        add_implication(get_node(u, state_u), get_node(v, state_v));
    }

    // Example: (u is state_u) OR (v is state_v)
    void add_or(int u, bool state_u, int v, bool state_v) {
        add_or(get_node(u, state_u), get_node(v, state_v));
    }

    // Example: Exactly ONE of them must be its expected state
    void add_xor(int u, bool state_u, int v, bool state_v) {
        add_xor(get_node(u, state_u), get_node(v, state_v));
    }

    // Forces variable 'u' to be strictly 'state'
    void force_value(int u, bool state) {
        force_true(get_node(u, state)); 
    }

    // ==========================================
    // TARJAN'S SCC & SOLVER
    // ==========================================
    void dfs(int u) {
        dfsn[u] = low[u] = ++timer;
        st.push_back(u);
        instack[u] = true;

        for (int v : adj[u]) {
            if (dfsn[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (instack[v]) {
                low[u] = min(low[u], dfsn[v]);
            }
        }

        if (low[u] == dfsn[u]) {
            while (true) {
                int v = st.back();
                st.pop_back();
                instack[v] = false;
                scc_id[v] = scc_count;
                if (u == v) break;
            }
            scc_count++;
        }
    }

    bool solve() {
        for (int i = 0; i < 2 * n; i++) {
            if (dfsn[i] == -1) dfs(i);
        }

        for (int i = 0; i < n; i++) {
            if (scc_id[2 * i] == scc_id[2 * i + 1]) {
                return is_satisfiable = false; 
            }
            assignment[i] = scc_id[2 * i] < scc_id[2 * i + 1];
        }
        return is_satisfiable = true;
    }
};

void solve()
{
    int m, n;
    cin >> m >> n;

    TwoSat ts(n);

    for (int i = 0; i < m; i++) {
        char sign1, sign2;
        int top1, top2;
        cin >> sign1 >> top1 >> sign2 >> top2;
        top1--; 
        top2--;
        ts.add_or(top1, sign1 == '+', top2, sign2 == '+');
    }

    if (ts.solve()) {
        for (int i = 0; i < n; i++) {
            cout << (ts.assignment[i] ? "+ " : "- ");
        }
        cout << "\n";
    } else {
        cout << "IMPOSSIBLE\n";
    }
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    //cin >> tc;
    while (tc--)
    {
        solve();
    }
    return 0;
}

/* * ==============================================================================
 * USAGE EXAMPLE IN solve():
 * ==============================================================================
 * int n = 3; // 3 boolean variables (0, 1, 2)
 * TwoSat ts(n);
 * * ts.add_or(0, true, 1, false); // (x0 OR !x1)
 * * ts.add_implication(1, true, 2, true); // IF (x1) THEN (x2)
 * * ts.force_value(0, false); // x0 must be FALSE
 * * if (ts.solve()) {
 * cout << "SATISFIABLE\n";
 * for(int i = 0; i < n; i++) cout << (ts.assignment[i] ? "T " : "F ");
 * } else {
 * cout << "UNSATISFIABLE\n";
 * }
 * ============================================================================== */



 /* ==============================================================================
 * 🧠 2-SAT MODELING FRAMEWORK: "ACTORS vs. VICTIMS"
 * ==============================================================================
 * To convert ANY problem into 2-SAT, follow these 3 steps:
 * * STEP 1: IDENTIFY THE "ACTORS" (The Variables / N)
 * What are the independent entities you can control? They MUST have exactly 
 * TWO states (e.g., pressed/not pressed, flipped/not flipped, chosen/rejected).
 * -> These are your 'Variables' (0-indexed).
 * * STEP 2: IDENTIFY THE "VICTIMS" (The Clauses / Conditions)
 * What are the entities screaming for a specific outcome? 
 * (e.g., A door saying "I must be open", A matrix cell saying "I must match").
 * -> Each victim usually depends on exactly TWO actors.
 * * STEP 3: TRANSLATE VICTIMS INTO LOGIC (The API Calls)
 * Look at the victim's requirement and the two actors (A and B) affecting it.
 * * 💡 QUICK TRANSLATION GUIDE (CHEAT CODES):
 * * 1. "At least one of them must be chosen/true"
 * -> ts.add_or(A, true, B, true);
 * * 2. "We CANNOT choose both A and B" (They conflict)
 * Meaning: At least one must be rejected.
 * -> ts.add_or(A, false, B, false);
 * * 3. "If I choose A, I MUST choose B" (Dependency)
 * Meaning: Either I don't choose A, or I choose B.
 * -> ts.add_or(A, false, B, true);  // Or: ts.add_implication(A, true, B, true);
 * * 4. "A and B MUST do the OPPOSITE action" (One chosen, one rejected)
 * -> ts.add_xor(A, true, B, true);
 * * 5. "A and B MUST do the EXACT SAME action" (Both chosen or both rejected)
 * -> ts.add_xor(A, true, B, false); 
 * * 6. "A MUST strictly be chosen / strictly be rejected" (Fixed state)
 * -> ts.force_value(A, true); // or false
 * * ==============================================================================
 * 📚 COMMON EXAMPLES:
 * - The Door Problem (776D): 
 * Actors = Switches (Press/Don't Press). Victims = Doors.
 * If door is closed: Needs 1 switch to flip -> ts.add_xor(X, true, Y, true);
 * If door is open: Needs 0 or 2 switches to flip -> ts.add_xor(X, true, Y, false);
 * * - The Matrix Problem (1475F):
 * Actors = Rows & Cols (Flip/Don't Flip). Victims = Cells.
 * If A[i][j] != B[i][j]: Needs 1 flip -> ts.add_xor(row, true, col, true);
 * If A[i][j] == B[i][j]: Needs 0 or 2 flips -> ts.add_xor(row, true, col, false);
 * ============================================================================== */