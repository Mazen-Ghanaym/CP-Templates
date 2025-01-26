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
struct Number_Theory
{
    // Goldbach’s conjecture: Each even integer n > 2 can be represented as a sum n = a+ b so that both a and b are primes.

    // prime numbers are numbers that are only divisible by 1 and themselves
    /*
        If a number n is not prime, it can be represented as a product a· b, where a ≤ sqrt(n)
        or b ≤ sqrt(n), so it certainly has a factor between 2 and floor(sqrt(n)).
        Using this observation,
        we can both test if a number is prime and find the prime factorization of a number in O(sqrt(n)) time.
    */
    bool is_prime(ll n)
    {
        // O(sqrt(n))
        if (n < 2)
            return false;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
                return false;
        }
        return true;
    }

    // binary exponentiation: a^b = a^(b/2) * a^(b/2) if b is even, a^b = a * a^(b/2) * a^(b/2) if b is odd
    ll binary_exponentiation(ll a, ll b)
    {
        // O(log(b))
        ll ans = 1;
        while (b)
        {
            if (b & 1)
                ans = (ans * a);
            a = (a * a);
            b >>= 1;
        }
        return ans;
    }

    ll binary_exponentiation_mod(ll a, ll b, ll m)
    {
        // O(log(b))
        ll ans = 1;
        while (b)
        {
            if (b & 1)
                ans = (ans * a) % m;
            a = (a * a) % m;
            b >>= 1;
        }
        return ans;
    }

    // ecldiean algorithm: gcd(a, b) = gcd(b, a % b) if b != 0, gcd(a, 0) = a
    ll gcd(ll a, ll b)
    {
        // O(log(min(a, b)))
        return b == 0 ? a : gcd(b, a % b);
    }

    // lcm(a, b) = a * b / gcd(a, b)
    ll lcm(ll a, ll b)
    {
        // O(log(min(a, b)))
        return a / gcd(a, b) * b;
    }

    ll modular_inverse(ll a, ll m)
    {
        // O(log(m))
        return binary_exponentiation_mod(a, m - 2, m);
    }

    // a number can be expressed as a product of prime numbers p1^a1 * p2^a2 * p3^a3 * ... * pk^ak
    // such that p1, p2, p3, ..., pk are prime numbers and a1, a2, a3, ..., ak are their powers
    map<int, int> prime_factorization(ll n)
    {
        map<int, int> mp;
        for (int i = 2; i * i <= n; i++)
        {
            while (n % i == 0)
            {
                mp[i]++;
                n /= i;
            }
        }
        if (n > 1)
            mp[n]++;
        return mp;
    }

    // the number of divisors of a number is equal to (a1 + 1) * (a2 + 1) * (a3 + 1) * ... * (ak + 1)
    ll number_of_divisors(ll n)
    {
        // O(sqrt(n))
        ll ans = 1;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                int cnt = 0;
                while (n % i == 0)
                {
                    cnt++;
                    n /= i;
                }
                ans *= (cnt + 1);
            }
        }
        if (n > 1)
            ans *= 2;
        return ans;
    }

    // the sum of divisors of a number is equal to (p1^(a1 + 1) - 1) / (p1 - 1) * (p2^(a2 + 1) - 1) / (p2 - 1) * ... * (pk^(ak + 1) - 1) / (pk - 1)
    // the sum of divisors of a number is equal to (1 + p1 + p1^2 + ... + p1^a1) * (1 + p2 + p2^2 + ... + p2^a2) * ... * (1 + pk + pk^2 + ... + pk^ak)
    ll sum_of_divisors(ll n)
    {
        // O(sqrt(n))
        ll ans = 1;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                ll sum = 1, p = 1;
                while (n % i == 0)
                {
                    p *= i;
                    sum += p;
                    n /= i;
                }
                ans *= sum;
            }
        }
        if (n > 1)
            ans *= (n + 1);
        return ans;
    }

    ll sum_of_divisors_mod(ll n)
    {
        // O(sqrt(n))
        ll ans = 1;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                ll sum = 1, p = 1;
                while (n % i == 0)
                {
                    p = (p * i) % MOD;
                    sum = (sum + p) % MOD;
                    n /= i;
                }
                ans = (ans * sum) % MOD;
            }
        }
        if (n > 1)
            ans = (ans * (n + 1)) % MOD;
        return ans;
    }

    // the product of divisors of a number is equal to n^(number of divisors / 2)
    // because we can form τ(n)/2 pairs from the factors, each with product n
    ll product_of_divisors(ll n)
    {
        // O(sqrt(n))
        ll ans = number_of_divisors(n);
        return binary_exponentiation(n, ans / 2);
    }

    ll product_of_divisors_mod(ll n, ll m)
    {
        // O(sqrt(n))
        ll ans = number_of_divisors(n);
        return binary_exponentiation_mod(n, ans / 2, m);
    }

    // euler's totient function: φ(n) = n * (1 - 1/p1) * (1 - 1/p2) * (1 - 1/p3) * ... * (1 - 1/pk)
    // where p1, p2, p3, ..., pk are the prime factors of n
    ll euler_totient_function(ll n)
    {
        // O(sqrt(n))
        ll ans = n;
        for (int i = 2; i * i <= n; i++)
        {
            if (n % i == 0)
            {
                while (n % i == 0)
                    n /= i;
                ans = ans / i * (i - 1);
            }
        }
        if (n > 1)
            ans = ans / n * (n - 1);
        return ans;
    }

    // fermat's little theorem: a^(m - 1) ≡ 1 (mod m) if a and m are coprime
    // euler's theorem: a^φ(m) ≡ 1 (mod m) if a and m are coprime because φ(m) = m - 1 if m is prime
    // this also yields x^k mod m = x^(k % (m - 1)) mod m if x and m are coprime
    ll power_big_number_mod(ll b, ll n, ll m){
        // O(log(n))
        ll ans = 1;
        n %= m - 1;
        while(n){
            if(n & 1)
                ans = (ans * b) % m;
            b = (b * b) % m;
            n >>= 1;
        }
        return ans;
    }

    // if a^b^c % m = a^(b^c % (m - 1)) % m
    ll power_tower_mod(ll a, ll b, ll c, ll m){
        // O(log(m))
        return power_big_number_mod(a, power_big_number_mod(b, c, m - 1), m);
    }


};
struct Sieve
{
    // the sieve of Eratosthenes is an algorithm used to find all prime numbers up to a given integer
    // the time complexity of the sieve of Eratosthenes is O(n log log n)
    const int N = 1e6 + 5;
    vector<bool> is_prime(N, true);
    vector<int> prime;
    void sieve()
    {
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i < N; i++)
        {
            if (is_prime[i])
            {
                prime.push_back(i);
                for (int j = i * i; j < N; j += i)
                {
                    is_prime[j] = false;
                }
            }
        }
    }

    void linear_sieve()
    {
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i < N; i++)
        {
            if (is_prime[i])
                prime.push_back(i);
            for (int j = 0; j < prime.size() && i * prime[j] < N; j++)
            {
                is_prime[i * prime[j]] = false;
                if (i % prime[j] == 0)
                    break;
            }
        }
    }
};
struct SPF
{
    // the smallest prime factor of a number n is the smallest prime number that divides n
    // the time complexity of the SPF algorithm is O(n log log n)
    const int N = 1e6 + 5;
    vector<int> spf(N);
    void SPF()
    {
        for (int i = 2; i < N; i++)
        {
            if (spf[i] == 0)
            {
                spf[i] = i;
                for (int j = i * i; j < N; j += i)
                {
                    if (spf[j] == 0)
                        spf[j] = i;
                }
            }
        }
    }

    // the prime factorization of a number n can be found in O(log(n)) time
    map<int, int> prime_factorization(int n)
    {
        map<int, int> mp;
        while (n > 1)
        {
            mp[spf[n]]++;
            n /= spf[n];
        }
        return mp;
    }

    // the number of divisors of a number n can be found in O(log(n)) time
    int number_of_divisors(int n)
    {
        int ans = 1;
        while (n > 1)
        {
            int cnt = 1;
            int p = spf[n];
            while (n % p == 0)
            {
                cnt++;
                n /= p;
            }
            ans *= cnt;
        }
        return ans;
    }

    // the sum of divisors of a number n can be found in O(log(n)) time
    int sum_of_divisors(int n)
    {
        int ans = 1;
        while (n > 1)
        {
            int cnt = 1;
            int p = spf[n];
            int sum = 1;
            int pp = 1;
            while (n % p == 0)
            {
                cnt++;
                pp *= p;
                sum += pp;
                n /= p;
            }
            ans *= sum;
        }
        return ans;
    }

    // the product of divisors of a number n can be found in O(log(n)) time
    int product_of_divisors(int n)
    {
        ll ans = number_of_divisors(n);
        return binary_exponentiation(n, ans / 2);
    }

    // // the number of coprime numbers of a number n can be found in O(log(n)) time
    // int number_of_coprime(int n){
    //     int ans = n;
    //     while(n > 1){
    //         int p = spf[n];
    //         while(n % p == 0){
    //             n /= p;
    //         }
    //         ans = ans / p * (p - 1);
    //     }
    //     return ans;
    // }

    // // the sum of coprime numbers of a number n can be found in O(log(n)) time
    // int sum_of_coprime(int n){
    //     int ans = n;
    //     while(n > 1){
    //         int p = spf[n];
    //         while(n % p == 0){
    //             n /= p;
    //         }
    //         ans = ans / p * (p - 1);
    //     }
    //     return ans / 2;
    // }

    // // the product of coprime numbers of a number n can be found in O(log(n)) time
    // int product_of_coprime(int n){
    //     int ans = n;
    //     while(n > 1){
    //         int p = spf[n];
    //         while(n % p == 0){
    //             n /= p;
    //         }
    //         ans = ans / p * (p - 1);
    //     }
    //     return ans / 2;
    // }
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