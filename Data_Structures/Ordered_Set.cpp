#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
class CustomOrderedSet {
  private:
    ordered_set os;

  public:
    void insert(int x) { os.insert(x); }

    void erase(int x) { os.erase(x); }

    int size() const { return os.size(); }

    ordered_set::iterator find_by_order(int k) {
        if (k < 0 || k >= (int)os.size()) return os.end();
        return os.find_by_order(k);
    }

    int order_of_key(int x) { return os.order_of_key(x); }

    ordered_set::iterator lower_bound(int x) { return os.lower_bound(x); }

    ordered_set::iterator upper_bound(int x) { return os.upper_bound(x); }

    // Custom function to get the smallest element >= x
    int successor(int x) {
        int count_lt_x = os.order_of_key(x);
        if (count_lt_x == (int)os.size()) return -1; // No element is >= x
        return *os.find_by_order(count_lt_x);
    }

    // Custom function to get the largest element <= x
    int predecessor(int x) {
        int count_le_x = os.order_of_key(x + 1);
        if (count_le_x == 0) return -1; // No element is <= x
        return *os.find_by_order(count_le_x - 1);
    }

    int count_less_than_or_equal(int x) { return os.order_of_key(x + 1); }
    const ordered_set& get_set() const { return os; }
};
//==================================================================================
// ordered multiset
#define ordered_multiset tree<int, null_type, less_equal<>, rb_tree_tag, tree_order_statistics_node_update>
ordered_multiset::iterator lower_bound(ordered_multiset &os, int x) { return os.lower_bound(x - 1); }
ordered_multiset::iterator upper_bound(ordered_multiset &os, int x) { return os.upper_bound(x + 1); }
int count_less_or_equal(ordered_multiset &os, int x) { return os.order_of_key(x + 1); }
void erase(ordered_multiset &os, int x) {
    auto it = lower_bound(os, x);
    if (it != os.end() && *it == x) os.erase(it);
}
void solve() {}
signed main(void) {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    // cin >> tc;
    while (tc--) {
        solve();
    }
    return 0;
}