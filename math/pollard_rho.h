/* Pollard Rho (factoring)
 * [copied from UBC]
 * USAGE
 *  pollard_rho::get_factor(n); returns a factor of n
 * TIME
 *  O(???)
 * STATUS
 *  untested
 */
#pragma once

#include "miller_rabin.h"

namespace pollard_rho {
template <int MAGIC = 0, typename value_t, std::enable_if_t<std::is_unsigned_v<value_t>, bool> = true>
value_t get_factor(value_t n) {
  assert(n > 1);
  static_assert(std::is_integral_v<value_t>);
  static mt19937 generator;
  using larger_t = std::conditional_t<
    std::is_same_v<value_t, uint32_t>, uint64_t, unsigned __int128>;
  if (n % 2 == 0) return 2;
  if (miller_rabin::is_prime(n)) return n;
  while (true) {
    value_t x = generator() % (n - 1) + 1;
    value_t c = generator() % (n - 1) + 1;
    value_t y = 2;
    value_t div = 1;
    for (int size = 2; div == 1; size *= 2) {
      if constexpr (MAGIC == 0) {
        for (int i = 0; i < size && div == 1; i++) {
          x = ((larger_t)x * x + c) % n;
          div = gcd((x < y ? y - x : x - y), n);
        }
      } else {
        value_t q = 1;
        for (int i = 0; i < size && div == 1; ) {
          value_t px = x;
          for (int j = 0; i < size && div == 1 && j < MAGIC; i++, j++) {
            x = ((larger_t)x * x + c) % n;
            q = ((larger_t)q * (x + n - y) % n);
          }
          div = gcd(q, n);
          if (div == n) {
            x = px;
            div = 1;
            for (int j = 0; div == 1 && j < MAGIC; j++) {
              x = ((larger_t)x * x + c) % n;
              div = gcd((x < y ? y - x : x - y), n);
            }
          }
        }
      }
      y = x;
    }
    if (div != 1 && div != n) {
      return div;
    }
  }
}
}

