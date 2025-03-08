# Combinatorics

## Properties

Binomial coefficients have many different properties. Here are the simplest of them:

*   Symmetry rule:

    $$\binom n k = \binom n {n-k}$$

*   Factoring in:

    $$\binom n k = \frac n k \binom {n-1} {k-1}$$

*   Sum over $k$:

    $$\sum_{k = 0}^n \binom n k = 2 ^ n$$

*   Sum over $n$:

    $$\sum_{m = 0}^n \binom m k = \binom {n + 1} {k + 1}$$

*   Sum over $n$ and $k$:

    $$\sum_{k = 0}^m  \binom {n + k} k = \binom {n + m + 1} m$$

*   Sum of the squares:

    $${\binom n 0}^2 + {\binom n 1}^2 + \cdots + {\binom n n}^2 = \binom {2n} n$$

*   Weighted sum:

    $$1 \binom n 1 + 2 \binom n 2 + \cdots + n \binom n n = n 2^{n-1}$$

*   Connection with the **Fibonacci numbers**:

    $$\binom n 0 + \binom {n-1} 1 + \cdots + \binom {n-k} k + \cdots + \binom 0 n = F_{n+1}$$
