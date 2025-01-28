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
// Miller-Rabin primality test
// Non-deterministic algorithm
// Complexity: O(iterations * log^3(n))
struct Miller_Rabin_Non_Deterministic1e18
{
  using u64 = uint64_t;
  using u128 = __uint128_t;

  u64 binpower(u64 base, u64 e, u64 mod)
  {
    u64 result = 1;
    base %= mod;
    while (e)
    {
      if (e & 1)
        result = (u128)result * base % mod;
      base = (u128)base * base % mod;
      e >>= 1;
    }
    return result;
  }

  bool check_composite(u64 n, u64 a, u64 d, int s)
  {
    u64 x = binpower(a, d, n);
    if (x == 1 || x == n - 1)
      return false;
    for (int r = 1; r < s; r++)
    {
      x = (u128)x * x % n;
      if (x == n - 1)
        return false;
    }
    return true;
  };

  bool MillerRabin(u64 n, int iter = 5)
  { // returns true if n is probably prime, else returns false.
    if (n < 4)
      return n == 2 || n == 3;

    int s = 0;
    u64 d = n - 1;
    while ((d & 1) == 0)
    {
      d >>= 1;
      s++;
    }

    for (int i = 0; i < iter; i++)
    {
      int a = 2 + rand() % (n - 3);
      if (check_composite(n, a, d, s))
        return false;
    }
    return true;
  }
};
// Miller-Rabin primality test
// Deterministic algorithm
// O( sz(a) * log^3(n) )
struct Miller_Rabin_Deterministic1e18
{
  using u64 = uint64_t;
  using u128 = __uint128_t;

  u64 binpower(u64 base, u64 e, u64 mod)
  {
    u64 result = 1;
    base %= mod;
    while (e)
    {
      if (e & 1)
        result = (u128)result * base % mod;
      base = (u128)base * base % mod;
      e >>= 1;
    }
    return result;
  }

  bool check_composite(u64 n, u64 a, u64 d, int s)
  {
    u64 x = binpower(a, d, n);
    if (x == 1 || x == n - 1)
      return false;
    for (int r = 1; r < s; r++)
    {
      x = (u128)x * x % n;
      if (x == n - 1)
        return false;
    }
    return true;
  };

  bool MillerRabin(u64 n)
  { // returns true if n is prime, else returns false.
    if (n < 2)
      return false;

    int r = 0;
    u64 d = n - 1;
    while ((d & 1) == 0)
    {
      d >>= 1;
      r++;
    }

    for (int a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
    {
      if (n == a)
        return true;
      if (check_composite(n, a, d, r))
        return false;
    }
    return true;
  }
};
void solve(int tc)
{
}
signed main(void)
{
  ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
  int tc = 1;
  // cin >> tc;
  int i = 1;
  while (tc--)
  {
    // cout<<"Case #"<<i<<": ";
    solve(i++);
  }
  return 0;
}