/* Duval's Lyndon Factorization Algorithm
 * USAGE
 *  vector<int> start_of_lyndon_words = lyndon_factors(string s);
 * TIME
 *  O(N)
 *  N = |string|
 * STATUS
 *  tested: boj/10266
 */
#pragma once

#include <vector>

template <template <typename> typename container_t, typename T>
std::vector<int> lyndon_factors(const container_t<T>& a) {
  std::vector<int> factors;
  for (int start = 0; start < (int)a.size(); ) {
    int compare = start;
    int i = start + 1;
    while (i < (int)a.size() and a[compare] <= a[i]) {
      if (a[compare] < a[i]) {
        compare = start;
      } else {
        compare++;
      }
      i++;
    }
    while (start <= compare) {
      factors.push_back(start);
      start += i - compare;
    }
  }
  return factors;
}

