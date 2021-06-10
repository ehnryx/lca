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

template <typename T>
struct prefix_sum {
  vector<T> data;
  T operator [] (int i) const { return i < 0 ? 0 : data.at(i); }
  prefix_sum(const vector<T>& arr): data(arr) {
    partial_sum(begin(data), end(data), begin(data));
  }
  T query(int l, int r) const { return operator [] (r) - operator [] (l - 1); }
};

