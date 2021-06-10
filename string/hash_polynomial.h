/* Polynomial String Hashing
 * USAGE
 *  poly_hasher<prime, mod> hasher(string_s)
 *  prime, mod are ints
 * INPUT
 *  string or basic_string<int>
 *  maps s_i -> s_i * prime^i % mod, outputs range sum
 * MEMBERS
 *  get(l, r); returns (sum_{l <= i < r} s_i * prime^i) % mod
 * TIME
 *  O(N) precomputation, O(1) query
 *  N = |string|
 * STATUS
 *  tested: nadc21/b
 */
#pragma once

#include "../number/mod_int.h"

template <int prime, int mod>
struct hash_polynomial {
  vector<mod_int<mod>> sum, invp;
  template <typename T>
  hash_polynomial(const basic_string<T>& s): sum(s.size() + 1), invp(s.size() + 1) {
    mod_int<mod> prime_inverse = mod_int<mod>(prime).inverse();
    mod_int<mod> x = invp[0] = 1;
    for (size_t i = 0; i < s.size(); i++) {
      x *= prime;
      sum[i + 1] = sum[i] + x * s[i];
      invp[i + 1] = invp[i] * prime_inverse;
    }
  }
  int get(int l, int r) {
    //if (l > r) throw invalid_argument("invalid range: l > r");
    //if (l < 0 || r >= (int)sum.size()) throw invalid_argument("index out of bounds");
    return ((sum[r] - sum[l]) * invp[l]).value();
  }
};

