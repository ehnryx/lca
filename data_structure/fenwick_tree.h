/* Fenwick Tree
 * USAGE
 *  fenwick_tree<T> range_sum(n); initializes a fenwick tree on [0, n)
 *  should satisfy n > 0
 * MEMBERS
 *  query(l, r); returns sum_{l <= i <= r} v_i
 *  query(r); returns sum_{0 <= i <= r} v_i
 *  update(i, v); updates v_i = v_i + v
 *  lower_bound(v); returns smallest r such that sum_{0 <= i <= r} v_i >= v
 *    returns n if not found.
 * TIME
 *  O(logN) per query, update, lower_bound
 *  N = |array|
 * STATUS
 *  tested: cf/1515i
 */
#pragma once

#include <vector>
#include <stdexcept>

template <typename T>
struct fenwick_tree {
  int n, logn;
  std::vector<T> data;
  T& operator[](int i) { return data[i]; }
  fenwick_tree(int _n): n(_n), logn(31 - __builtin_clz(n)), data(n + 1) {}
  fenwick_tree(const std::vector<T>& arr):
    n((int)arr.size()), logn(31 - __builtin_clz(n)), data(n + 1) {
    for (int i = 0; i < n; i++) {
      update(i, arr[i]);
    }
  }
  T query_point(int r) const { return query(r, r); }
  T query_range(int l, int r) const { return query(l, r); }
  T query(int l, int r) const { return query(r) - query(l - 1); }
  T query(int r) const {
    if (r <= -1) return 0;
    if (r >= n) throw std::invalid_argument("query index out of bounds");
    T res(0);
    for (r += 1; r > 0; r -= r & -r) {
      res += data[r];
    }
    return res;
  }
  void update_point(int i, const T& v) { update(i, v); }
  void insert(int i, const T& v = T(1)) { update(i, v); }
  void update(int i, const T& v) {
    if (i < 0 || n <= i) throw std::invalid_argument("update index out of bounds");
    for (i += 1; i <= n; i += i & -i) {
      data[i] += v;
    }
  }
  int lower_bound(T v) const {
    int res = 0;
    T prefix(0);
    for (int i = logn; i >= 0; i--) {
      if (res + (1 << i) <= n && prefix + data[res + (1 << i)] < v) {
        res += 1 << i;
        prefix += data[res];
      }
    }
    return res;
  }
};

