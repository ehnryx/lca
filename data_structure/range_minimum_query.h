/* Range Minimum Query
 * USAGE
 *  range_minimum_query rmq(arr);
 *  int val = rmq.query(l, r); // inclusive-exclusive range [l, r)
 * CONSTRUCTOR
 *  arr: array on which to build the range minimum query
 * TIME
 *  O(NlogN) construction, O(1) query
 *  N = |array|
 * STATUS
 *  tested: kattis/tourists, cf/1454f
 */
#pragma once

#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, class Compare = std::less<>>
struct range_minimum_query {
  std::vector<std::vector<T>> rmq;
  range_minimum_query() = default;
  range_minimum_query(const std::vector<T>& arr) {
    int n = (int)arr.size();
    int L = 32 - __builtin_clz(n);
    rmq.resize(L);
    rmq.front() = arr;
    build(n, L);
  }
  range_minimum_query(std::vector<T>&& arr) {
    int n = (int)arr.size();
    int L = 32 - __builtin_clz(n);
    rmq.resize(L);
    rmq.front() = move(arr);
    build(n, L);
  }
  T query(int l, int r) const {
    if (l >= r) throw std::invalid_argument("The range is empty, ie. l >= r");
    int j = 31 - __builtin_clz(r - l);
    return std::min(rmq[j][l], rmq[j][r - (1<<j)], Compare());
  }

private:
  void build(int n, int L) {
    for (int j = 1; j < L; j++) {
      rmq[j].resize(n - (1 << j) + 1);
      for (int i = 0; i + (1 << j) <= n; i++) {
        rmq[j][i] = std::min(rmq[j-1][i], rmq[j-1][i + (1<<(j-1))], Compare());
      }
    }
  }
};

