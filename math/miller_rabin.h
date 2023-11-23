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

#include <array>
#include <cstdint>

namespace miller_rabin {
template <typename value_t, std::enable_if_t<std::is_unsigned_v<value_t>, bool> = true>
bool is_prime(value_t n) {
  static_assert(std::is_integral_v<value_t>);
  using larger_t =
      std::conditional_t<std::is_same_v<value_t, uint32_t>, uint64_t, unsigned __int128>;
  static constexpr auto witnesses = [] {
    if constexpr (std::is_same_v<value_t, uint32_t>) {
      return std::array{2, 7, 61};
    } else {
      return std::array{2ull, 325ull, 9375ull, 28178ull, 450775ull, 9780504ull, 1795265022ull};
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
