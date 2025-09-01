#include <bits/stdc++.h>
using namespace std;
#define ll long long
const int N = 1e5 + 5, SQ = 325;
int arr[N], n, q;
vector<vector<int>> blk(SQ, vector<int>(26, 0));
int lazy_seg[SQ] = {0};

void process(int blk_idx) {
    int st = blk_idx * SQ;
    int end = min((blk_idx + 1) * SQ, n);
    if (lazy_seg[blk_idx] != 0) {
        for (int i = st; i < end; i++) {
            arr[i] = (arr[i] + lazy_seg[blk_idx]) % 26;
        }
        lazy_seg[blk_idx] = 0;
    }
    blk[blk_idx] = vector<int>(26, 0);
    for (int i = st; i < end; i++) {
        blk[blk_idx][arr[i]]++;
    }
}

void update(int l, int r, int val) {
    int st = l / SQ;
    int end = r / SQ;
    if (st == end) {
        for (int i = l; i <= r; i++) {
            arr[i] = (arr[i] + val) % 26;
        }
        process(st);
        return;
    }
    for (int i = l; i < (st + 1) * SQ; i++) {
        arr[i] = (arr[i] + val) % 26;
    }
    process(st);
    for (int i = st + 1; i < end; i++) lazy_seg[i] += val;
    for (int i = end * SQ; i <= r; i++) {
        arr[i] = (arr[i] + val) % 26;
    }
    process(end);
}

ll query(int l, int r) {
    vector<int> freq(26, 0);
    while (l <= r) {
        if (l % SQ == 0 && l + SQ <= r) {
            for (int i = 0; i < 26; i++) {
                freq[(i + lazy_seg[l / SQ]) % 26] += blk[l / SQ][i];
            }
            l += SQ;
        } else {
            freq[(arr[l] + lazy_seg[l / SQ]) % 26]++;
            l++;
        }
    }
    return *max_element(freq.begin(), freq.end());
}
void init() {
    for (int i = 0; i < SQ; i++) {
        blk[i].assign(26, 0);
        lazy_seg[i] = 0;
    }
}
void solve() {
    init();
    cin >> n >> q;
    string s;
    cin >> s;
    for (int i = 0; i < n; i++) {
        arr[i] = s[i] - 'a';
    }
    for (int i = 0; i < SQ; i++) process(i);
    while (q--) {
        int l, r;
        cin >> l >> r;
        --l, --r;
        update(l, r, query(l, r));
    }
    for (int i = 0; i < SQ; i++) process(i);
    for (int i = 0; i < n; i++) {
        cout << char('a' + arr[i]);
    }
    cout << endl;
}
int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc;
    cin >> tc;
    while (tc--) {
        solve();
    }
    return 0;
}