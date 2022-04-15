/* Extended Euclidean Algorithm
 * USAGE
 *  auto [gcd, x, y] = extended_gcd(a, b);
 *  returns gcd, x and y such that ax + by = gcd
 * STATUS
 *  untested
 */
#pragma once

#include <utility>

template <typename T>
std::tuple<T, T, T> extended_gcd(T a, T b) {
  if (b == 0) return std::tuple(a, 1, 0);
  auto [g, y, x] = extended_gcd(b, a % b);
  return std::tuple(g, x, y - x * (a / b));
}

