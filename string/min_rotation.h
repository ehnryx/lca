/* Minimum Rotation of string (via Lyndon Factorization)
 * USAGE
 *  auto rotated = min_rotation(string)
 * TIME
 *  O(N)
 *  N = |string|
 * STATUS
 *  tested: boj/10266
 */
#pragma once

#include "lyndon_factors.h"

#include <cassert>
#include <ranges>

template <template <typename> typename container_t, typename T>
int min_rotation(const container_t<T>& a) {
  container_t<T> a2;
  a2.reserve(2 * a.size());
  a2.insert(a2.end(), a.begin(), a.end());
  a2.insert(a2.end(), a.begin(), a.end());
  auto starts = lyndon_factors(a2);
  for (int i : starts | std::views::reverse) {
    if (i < a.size()) {
      return i;
    }
  }
  assert(false);
}
