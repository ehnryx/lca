/* Polynomial String Hashing
 * USAGE
 *  hash_polynomial<prime, mod> hasher(string_s)
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
 *  tested: nadc21/b, cf/1326d2
 */
#pragma once

#include "../number/mod_int.h"

template <int mod, int prime>
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
  int get(int i, int len) {
    return ((sum[i + len] - sum[i]) * invp[i]).value();
  }
};

