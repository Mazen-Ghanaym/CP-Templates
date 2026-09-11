# Two Knights and Grid Placement Combinatorics

This document covers the solution, mathematical breakdown, and general framework for solving grid placement problems where objects (like chess pieces) interact or attack each other, inspired by the classic **CSES - Two Knights** problem.

---

## 1. CSES - Two Knights Problem

### Problem Statement
Count for each $k = 1, 2, \dots, n$, the number of ways to place two non-attacking knights on a $k \times k$ chessboard.

---

### Core Idea: Complementary Counting (Total - Invalid)

Counting non-attacking positions directly can be complex because of boundary conditions. Instead, we use **Complementary Counting**:

$$\text{Valid Placements} = \text{Total Placements} - \text{Attacking Placements}$$

#### 1. Total Placements
On a $k \times k$ board, there are $k^2$ squares. The number of ways to choose 2 distinct squares for 2 identical knights is:

$$\text{Total} = \binom{k^2}{2} = \frac{k^2(k^2 - 1)}{2}$$

#### 2. Attacking Placements
Two knights attack each other if and only if one knight can reach the other in a single move. 

- A knight move forms the diagonal corners of a **$2 \times 3$** or **$3 \times 2$** rectangle.
- Inside any **$2 \times 3$** sub-grid, there are **exactly 2 attacking pairs** of knights (the opposite corners).
- Inside any **$3 \times 2$** sub-grid, there are also **exactly 2 attacking pairs**.

Now, count how many such sub-grids exist on a $k \times k$ board:
- Number of $2 \times 3$ blocks: $(k - 1) \times (k - 2)$
- Number of $3 \times 2$ blocks: $(k - 2) \times (k - 1)$

Total $2 \times 3$ and $3 \times 2$ blocks:

$$\text{Total Blocks} = 2 \cdot (k - 1)(k - 2)$$

Since each block contains $2$ attacking pairs:

$$\text{Attacking Pairs} = 2 \times 2(k - 1)(k - 2) = 4(k - 1)(k - 2)$$

#### 3. Closed-Form Formula for $k \times k$

$$\boxed{\text{Ans}(k) = \frac{k^2(k^2 - 1)}{2} - 4(k - 1)(k - 2)}$$

---

## 2. General Framework for Grid Placement Problems

When solving problems involving placing 2 (or more) objects on an $N \times M$ grid:

### Framework Steps:
1. **Complementary Counting**: Express $\text{Valid} = \text{Total} - \text{Invalid}$.
2. **Sub-grid / Bounding Box Decomposition**:
   - Determine the smallest bounding box $(r \times c)$ where the invalid pattern occurs.
   - Number of $r \times c$ sub-grids in an $N \times M$ board is $(N - r + 1) \times (M - c + 1)$.
   - Multiply the number of sub-grids by the number of invalid configurations per sub-grid.
3. **Graph / Edge Counting Perspective**:
   - View grid cells as vertices in a graph ($V = N \times M$).
   - Draw an edge between two cells if placing objects on them is invalid (e.g., adjacent cells or knight-attack cells).
   - Total invalid pairs for 2 objects = Total edges $E$ in the interaction graph.
   - Valid ways = $\binom{|V|}{2} - |E|$.

---

## 3. Variations and Generalizations

### Variation 1: Two Knights on an $N \times M$ Grid
For a general $N \times M$ board:
- Total pairs: $\binom{NM}{2}$
- $2 \times 3$ sub-grids: $(N - 1)(M - 2)$, each having 2 attacking pairs.
- $3 \times 2$ sub-grids: $(N - 2)(M - 1)$, each having 2 attacking pairs.

$$\text{Ans}(N, M) = \binom{NM}{2} - 2\big[(N - 1)(M - 2) + (N - 2)(M - 1)\big]$$

---

### Variation 2: Two Kings Non-Attacking on $N \times M$ Grid
Two kings attack if they are adjacent horizontally, vertically, or diagonally (a $2 \times 2$ neighborhood).

Using the **Graph / Edge Counting** method:
- Horizontal adjacent pairs: $N(M - 1)$
- Vertical adjacent pairs: $M(N - 1)$
- Diagonal adjacent pairs: $2(N - 1)(M - 1)$

Total attacking pairs $E$:

$$E = N(M - 1) + M(N - 1) + 2(N - 1)(M - 1) = 4NM - 3N - 3M + 2$$

$$\text{Ans}(N, M) = \binom{NM}{2} - (4NM - 3N - 3M + 2)$$

---

### Variation 3: Two Rooks Non-Attacking on $N \times M$ Grid
Two rooks attack if they are in the same row or column.

#### Method 1 (Direct Combination):
- Select 2 distinct rows: $\binom{N}{2}$
- Select 2 distinct columns: $\binom{M}{2}$
- Place 2 rooks in the 4 intersection points such that no two are in the same row/col: $2$ ways.

$$\text{Ans}(N, M) = 2 \cdot \binom{N}{2} \binom{M}{2} = \frac{N(N - 1) M(M - 1)}{2}$$

#### Method 2 (Complementary):
- Total ways: $\binom{NM}{2}$
- Same row pairs: $N \cdot \binom{M}{2}$
- Same column pairs: $M \cdot \binom{N}{2}$

$$\text{Ans}(N, M) = \binom{NM}{2} - N \binom{M}{2} - M \binom{N}{2}$$

---

### Variation 4: Two Bishops Non-Attacking on $N \times M$ Grid
Two bishops attack if they are on the same diagonal.
- Partition the board into **White** and **Black** squares (bishops on different colored squares never attack each other).
- Count diagonal lengths $d_i$ for main diagonals ($r - c = \text{const}$) and anti-diagonals ($r + c = \text{const}$).
- Total attacking pairs = $\sum \binom{d_i}{2}$ across all diagonals.

---

## 4. C++ Reference Implementation

```cpp
#include <iostream>
using namespace std;

typedef long long ll;

// CSES - Two Knights on k x k board
ll two_knights_square(ll k) {
    if (k < 1) return 0;
    ll total = (k * k) * (k * k - 1) / 2;
    ll attacking = 4 * (k - 1) * (k - 2);
    return total - attacking;
}

// Two Knights on N x M board
ll two_knights_rect(ll n, ll m) {
    if (n <= 0 || m <= 0) return 0;
    ll total = (n * m) * (n * m - 1) / 2;
    ll attacking = 0;
    if (n >= 2 && m >= 3) attacking += 2 * (n - 1) * (m - 2);
    if (n >= 3 && m >= 2) attacking += 2 * (n - 2) * (m - 1);
    return total - attacking;
}

// Two Kings non-attacking on N x M board
ll two_kings_rect(ll n, ll m) {
    if (n <= 0 || m <= 0) return 0;
    ll total = (n * m) * (n * m - 1) / 2;
    ll attacking = 4 * n * m - 3 * n - 3 * m + 2;
    return total - attacking;
}

// Two Rooks non-attacking on N x M board
ll two_rooks_rect(ll n, ll m) {
    if (n < 2 || m < 2) return 0;
    return (n * (n - 1) * m * (m - 1)) / 2;
}

int main() {
    int n;
    if (cin >> n) {
        for (int k = 1; k <= n; ++k) {
            cout << two_knights_square(k) << "\n";
        }
    }
    return 0;
}
```

---

## 5. Summary Cheat Sheet

| Problem | Total Placements | Invalid Condition | Formula |
|---|---|---|---|
| **2 Knights ($k \times k$)** | $\binom{k^2}{2}$ | Form $2 \times 3$ or $3 \times 2$ blocks | $\binom{k^2}{2} - 4(k-1)(k-2)$ |
| **2 Knights ($N \times M$)** | $\binom{NM}{2}$ | Form $2 \times 3$ or $3 \times 2$ blocks | $\binom{NM}{2} - 2[(N-1)(M-2) + (N-2)(M-1)]$ |
| **2 Kings ($N \times M$)** | $\binom{NM}{2}$ | Adjacent (Horiz/Vert/Diag) | $\binom{NM}{2} - (4NM - 3N - 3M + 2)$ |
| **2 Rooks ($N \times M$)** | $\binom{NM}{2}$ | Same row or column | $2 \binom{N}{2} \binom{M}{2}$ |
