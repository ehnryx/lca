/* Polynomial String Hashing
 * USAGE
 *  hash_multi<Hasher, prime, mod> hasher(string_s)
 *  prime, mod are ints
 *  Hasher<prime, mod> is a hasher
 * MEMBERS
 *  get(l, r); returns tuple of hashes
 * TIME
 *  get: quadratic in the number of hashes
 *  constructor: sum of times to build each hash
 * STATUS
 *  tested: cf/1326d2
 */
#pragma once

#include <string>
#include <tuple>

template <template <int, int> typename hash_single, int mod, int prime, int... others>
struct hash_multi : hash_multi<hash_single, others...>, hash_single<mod, prime> {
  hash_multi(const std::string& s)
      : hash_multi<hash_single, others...>(s), hash_single<mod, prime>(s) {}
  auto get(int i, int len) {
    return std::tuple_cat(
        std::tuple(hash_single<mod, prime>::get(i, len)),
        hash_multi<hash_single, others...>::get(i, len));
  }
};

template <template <int, int> typename hash_single, int mod, int prime>
struct hash_multi<hash_single, mod, prime> : hash_single<mod, prime> {
  hash_multi(const std::string& s) : hash_single<mod, prime>(s) {}
  auto get(int i, int len) { return std::tuple(hash_single<mod, prime>::get(i, len)); }
};
