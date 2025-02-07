# Number Theory Knowledge

## Application in Euler's theorem

$\text{if } a \text{ and } m \text{ are relatively prime.}$
$$a^{\phi(m)} \equiv 1 \pmod m \quad $$

In the particular case when $m$ is prime, Euler's theorem turns into **Fermat's little theorem**:

$$a^{m - 1} \equiv 1 \pmod m$$

As immediate consequence we also get the equivalence:

$$a^n \equiv a^{n \bmod \phi(m)} \pmod m$$

This allows computing $x^n \bmod m$ efficiently for $\bf{not}$ $\bf{coprime}$ $x$ and $m$.
For arbitrary $x, m$ and $n \geq \log_2 m$:

$$x^{n}\equiv x^{\phi(m)+[n \bmod \phi(m)]} \mod m$$

> [!NOTE]
> [Problem](https://codeforces.com/gym/100975/problem/F) $(2^{2^n} + 1)\mod k$
>
> Suppose pow = $(\phi(m)+[n \bmod \phi(m)]) \mod \phi(m)$
>
> if (pow == 0) pow = $\phi(m)$

### Why Handle pow = 0?

In your code, you compute:

$$
\text{pow} = 2^n \mod \phi(k)
$$

However, when $2^n$ is a multiple of $\phi(k)$, the result of $2^n \mod \phi(k)$ will be 0. But this creates an issue when using it in modular exponentiation:

$$
2^{\text{pow}} \mod k
$$

If $\text{pow} = 0$, it means you're computing:

$$
2^0 \mod k
$$

which is always 1. But mathematically, when $n$ is large, we expect:

$$
2^n \equiv 2^{\phi(k) + (n \bmod \phi(k))} \mod k
$$

By Euler’s theorem:

$$
x^{\phi(m)} \equiv 1 \mod m
$$

so we need to adjust by replacing $0$ with $\phi(k)$ because:

$$
2^{\phi(k)} \equiv 1 \mod k
$$

Thus, when $\text{pow} = 0$, setting it to $\phi(k)$ ensures the exponentiation still produces a meaningful result.

---
## Modular Inverse

* For an arbitrary (but coprime) modulus $m$: $a ^ {\phi (m) - 1} \equiv a ^{-1} \mod m$
* For a prime modulus $m$: $a ^ {m - 2} \equiv a ^ {-1} \mod m$
* if a and m are coprime, then $a ^ {-1} \mod m$ can be found using the extended Euclidean algorithm.
