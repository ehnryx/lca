/* Prefix Sum
 * USAGE
 *  prefix_sum<T> range_sum(n); initializes a fenwick tree on [0, n)
 *  should satisfy n > 0
 * MEMBERS
 *  query(l, r); returns sum_{l <= i <= r} v_i
 *  query(r); returns sum_{0 <= i <= r} v_i
 *  lower_bound(v); returns smallest r such that sum_{0 <= i <= r} v_i >= v
 *    returns n if not found.
 * TIME
 *  O(1) per query
 *  O(logN) per lower_bound
 *  N = |array|
 * STATUS
 *  untested
 */
#pragma once

#include <vector>

template <typename T, size_t N = 1>
struct prefix_sum {
  prefix_sum() = delete;
};

template <typename T>
struct prefix_sum<T, 1> {
  std::vector<T> data;
  prefix_sum(const std::vector<T>& arr) : data(arr) {
    partial_sum(begin(data), end(data), begin(data));
  }
  T at(int i) const { return i < 0 ? 0 : data.at(i); }
  T query(int l, int r) const { return at(r) - at(l - 1); }
};

template <typename T>
struct prefix_sum<T, 2> {
  std::vector<std::vector<T>> data;
  prefix_sum(const std::vector<std::vector<T>>& arr) : data(arr) {
    for (size_t i = 0; i < arr.size(); i++) {
      for (size_t j = 0; j < arr[i].size(); j++) {
        data[i][j] += at(i - 1, j) + at(i, j - 1) - at(i - 1, j - 1);
      }
    }
  }
  T at(int i, int j) const { return i < 0 || j < 0 ? 0 : data.at(i).at(j); }
  T query(int lx, int ly, int rx, int ry) const {
    return at(rx, ry) + at(lx - 1, ly - 1) - at(rx, ly - 1) - at(lx - 1, ry);
  }
};
