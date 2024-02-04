/* Miller-Rabin Deterministic up to 2^64
 * [copied from UBC]
 * USAGE
 *  miller_rabin::is_prime(n);
 * TIME
 *  O(???)
 * STATUS
 *  untested
 */
#pragma once

#include "../utility/types.h"

#include <array>
#include <cstdint>

namespace miller_rabin {
template <typename value_t>
  requires(std::is_integral_v<value_t>)
bool is_prime(value_t n) {
  using larger_t = typename utility::bigger_type<value_t>;
  static constexpr auto witnesses = [] {
    if constexpr (sizeof(value_t) <= sizeof(int32_t)) {
      return std::array{2, 7, 61};
    } else {
      return std::array{
          2ull, 325ull, 9375ull, 28178ull, 450775ull, 9780504ull, 1795265022ull,
      };
    }
  }();
  if (n < 2) return false;
  if (n % 2 == 0) return n == 2;
  int r = __builtin_ctzll(n - 1);
  value_t d = (n - 1) >> r;
  for (value_t a : witnesses) {
    a %= n;
    if (a == 0) return true;
    if (a == 1 || a == n - 1) continue;
    value_t x = 1;
    for (value_t e = d; e != 0; e /= 2) {
      if (e % 2 == 1) x = ((larger_t)x * a) % n;
      a = ((larger_t)a * a) % n;
    }
    for (int i = 0; i < r; i++) {
      value_t y = ((larger_t)x * x) % n;
      if (y == 1 && x != 1 && x != n - 1) {
        return false;
      }
      x = y;
    }
    if (x != 1) {
      return false;
    }
  }
  return true;
}
}  // namespace miller_rabin
