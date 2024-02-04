/* N-Dimensional Array
 * USAGE
 *  nd_array<int, 3> arr(n1, n2, n3, initial_value);
 *  nd_array<int, 3> arr(n1, n2, n3);
 *  arr(i, j, k) for access (instead of arr[i][j][k])
 *  arr.at(i, j, k) asserts indices in bounds (SLOW)
 *  arr.at_slow(i, j, k) throws if out of bounds (VERY VERY SLOW)
 *    dimensions 0-indexed from the right
 * NOTES
 *  stored in row-major order in a flattened vector
 *  unable to resize
 * STATUS
 *  untested
 */
#pragma once

#include "nd_indexer.h"

#include <vector>

template <typename T, size_t dims>
struct nd_array {
  const nd_indexer<dims> indexer;
  std::vector<T> data;
  template <typename... Args>
  nd_array(Args... ds) : indexer(ds...), data(indexer.size(), _get_init(ds...)) {}
  template <typename... U>
  nd_array(std::tuple<U...> ds, T init = {})
      : indexer(std::make_from_tuple<nd_indexer<dims>>(ds)), data(indexer.size(), init) {}
  size_t size() const { return indexer.size(); }
  T& operator[](size_t i) { return data[i]; }
  const T& operator[](size_t i) const { return data[i]; }

  // access
  template <typename... Args>
  T& operator()(Args... is) {
    return data[indexer.get(is...)];
  }
  template <typename... Args>
  const T& operator()(Args... is) const {
    return data[indexer.get(is...)];
  }
  template <typename... Args>
  size_t index(Args... is) const {
    return indexer.get(is...);
  }

  // access with bounds checking
  template <typename... Args>
  T& at(Args... is) {
    return data[indexer.at(is...)];
  }
  template <typename... Args>
  const T& at(Args... is) const {
    return data[indexer.at(is...)];
  }
  template <typename... Args>
  T& at_slow(Args... is) {
    return data[indexer.at_slow(is...)];
  }
  template <typename... Args>
  const T& at_slow(Args... is) const {
    return data[indexer.at_slow(is...)];
  }

  // get tuple from flattened index
  auto from_index(size_t i) const { return indexer.from_index(i); }

 private:
  template <typename... Args>
  static T _get_init(Args... ds) {
    if constexpr (sizeof...(ds) == dims) {
      return T{};
    } else {
      static_assert(sizeof...(ds) == dims + 1);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
      return (ds, ...);
#pragma GCC diagnostic pop
    }
  }
};
