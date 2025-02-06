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
