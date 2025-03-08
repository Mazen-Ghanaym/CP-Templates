/*
    __builtin_popcount(n) returns the number of set bits in n.
    __builtin_popcoutll(n) returns the number of set bits in n (long long).
    __builtin_ctz(n) returns the number of trailing zeros in n.
    __builtin_ctzll(n) returns the number of trailing zeros in n (long long).
    __builtin_clz(n) returns the number of leading zeros in n.
    __builtin_clzll(n) returns the number of leading zeros in n (long long).
    __builtin_ffs(n) returns the index of the least significant set bit in n.
    __builtin_ffsll(n) returns the index of the least significant set bit in n (long long).
    __builtin_parity(n) returns the parity of n (number of set bits modulo 2).
    __builtin_parityll(n) returns the parity of n (number of set bits modulo 2) (long long).
    a + b = a ^ b + (a & b) << 1
    a + b = (a | b) << 1 - (a ^ b)
*/