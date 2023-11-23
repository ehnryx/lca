/* Range Minimum Query
 * USAGE
 *  range_minimum_query rmq(arr);
 *  int val = rmq.query(l, r); // inclusive range [l, r]
 * CONSTRUCTOR
 *  arr: array on which to build the range minimum query
 * TIME
 *  O(NlogN) construction, O(1) query
 *  N = |array|
 * STATUS
 *  tested: kattis/tourists, cf/1454f
 */
#pragma once

#include "../utility/minmax.h"

#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename Func>
struct rmq_functional {
  std::vector<std::vector<T>> rmq;
  rmq_functional() = default;
  rmq_functional(const std::vector<T>& arr) {
    int n = (int)arr.size();
    int L = 32 - __builtin_clz(n);
    rmq.resize(L);
    rmq.front() = arr;
    build(n, L);
  }
  rmq_functional(std::vector<T>&& arr) {
    int n = (int)arr.size();
    int L = 32 - __builtin_clz(n);
    rmq.resize(L);
    rmq.front() = move(arr);
    build(n, L);
  }
  T query(int l, int r) const {
    if (l > r) throw std::invalid_argument("The range is empty, ie. l > r");
    int j = 31 - __builtin_clz(r + 1 - l);
    return Func()(rmq[j][l], rmq[j][r + 1 - (1 << j)]);
  }
  void build(int n, int L) {
    for (int j = 1; j < L; j++) {
      rmq[j].resize(n - (1 << j) + 1);
      for (int i = 0; i + (1 << j) <= n; i++) {
        rmq[j][i] = Func()(rmq[j - 1][i], rmq[j - 1][i + (1 << (j - 1))]);
      }
    }
  }
};

template <typename T, typename Compare = std::less<T>>
using range_minimum_query = rmq_functional<
    T, utility::min<T, std::conditional_t<std::is_void_v<Compare>, std::less<T>, Compare>>>;
// no alias template argument deduction

template <typename Compare = void, typename T>
auto make_rmq(std::vector<T> const& arr) -> range_minimum_query<T, Compare> {
  return {arr};
}

template <typename Compare = void, typename T>
auto make_rmq(std::vector<T>&& arr) -> range_minimum_query<T, Compare> {
  return {std::move(arr)};
}
