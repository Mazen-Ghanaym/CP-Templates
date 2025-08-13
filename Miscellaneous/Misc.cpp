#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
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
// generate random -> use rnd() -> rnd()%n
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
inline int random(int l, int r) {
    return rnd() % (r - l + 1) + l;
    // return uniform_int_distribution<int>(l, r)(rnd);
}

// another way to generate random
mt19937 seed{random_device{}()};
uniform_int_distribution<int> rnd(INT_MIN, INT_MAX);
inline int random() { return rnd(seed); }
inline int random(int l, int r) { return uniform_int_distribution<int>(l, r)(seed); }


// GP Hash Table
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

struct custom_hash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }
  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }
};

gp_hash_table<int, int, custom_hash> mp;

