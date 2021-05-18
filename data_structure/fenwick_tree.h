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

template <typename T, typename = enable_if_t<is_integral_v<T>>>
struct fenwick_tree {
  int n, logn;
  vector<T> data;
  T& operator [] (int i) { return data[i]; }
  fenwick_tree(int _n): n(_n), logn(31 - __builtin_clz(n)), data(n + 1) {}
  T query(int l, int r) const { return query(r) - query(l - 1); }
  T query(int r) const {
    T res(0);
    for (r += 1; r > 0; r -= r & -r) {
      res += data[r];
    }
    return res;
  }
  void update(int i, const T& v) {
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
