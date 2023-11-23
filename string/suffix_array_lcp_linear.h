/* Suffix Array that supports Longest Common Prefix queries
 * USAGE
 *  suffix_array_lcp sa(string_s);
 *  int length_of_lcp = sa.lcp(i, j); // 0-indexed i, j
 * INPUT
 *  string or basic_string<int>
 *  remap values to [1,n] if using basic_string<int>
 * TIME
 *  O(NlogN) construction, O(1) query
 *  N = |string|
 * STATUS
 *  tested: cf/102803c
 */
#pragma once

#include "../data_structure/range_minimum_query_linear.h"
#include "suffix_array.h"

template <typename T>
struct suffix_array_lcp_linear : suffix_array<T>, range_minimum_query_linear<int> {
  template <template <typename> typename container_t>
  suffix_array_lcp_linear(const container_t<T>& s)
      : suffix_array<T>(s), range_minimum_query_linear<int>(suffix_array<T>::height) {}
  int lcp(int i, int j) const {
    if (i == j) return (int)this->sa.size() - 1 - i;
    int l = min(this->rank[i], this->rank[j]);
    int r = max(this->rank[i], this->rank[j]);
    return range_minimum_query_linear<int>::query(l + 1, r);
  }
  int operator[](int i) { return suffix_array<T>::operator[](i); }
};
