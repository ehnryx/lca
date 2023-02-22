/* Monotone Dynamic Hull (Line Container)
 * USAGE
 *  monotone_hull<T> hull;
 *  maintains the upper envelope of a set of lines
 *  lines must be inserted in increasing order of slope
 *  WARMING: seems to be broken for floating point types
 * MEMBERS
 *  void insert(T m, T b); inserts the line y = mx + b
 *  T query(T x); queries for the maximum at x
 * TIME
 *  Amortized O(1) insert, query
 * STATUS
 *  tested: boj/4008
 */
#pragma once

#include <limits>
#include <set>

template <typename T>
struct monotone_hull_line {
  T m, b;
  mutable T last;
  monotone_hull_line(const T& _m, const T& _b, const T& r): m(_m), b(_b), last(r) {}
  bool operator < (const monotone_hull_line& o) const { return m < o.m; }
  bool operator < (const T& x) const { return last < x; }
};

template <typename T>
struct monotone_hull {
  std::vector<monotone_hull_line<T>> hull;
  using base_it = typename decltype(hull)::iterator;
  static constexpr T inf = std::numeric_limits<T>::has_infinity ?
    std::numeric_limits<T>::infinity() : std::numeric_limits<T>::max();
  static T div(const T& a, const T& b) {
    if constexpr (std::is_integral_v<T>) {
      return a / b - ((a ^ b) < 0 && a % b != 0);
    } else {
      return a / b;
    }
  }
  T intersect(const monotone_hull_line<T>& it, const monotone_hull_line<T>& jt) {
    if (it.m == jt.m) return it.b < jt.b ? -inf : inf;
    return div(it.b - jt.b, jt.m - it.m);
  }
  // add the line y = mx + b
  void insert(const T& m, const T& b) {
    monotone_hull_line<T> cur(m, b, inf);
    // remove lines to the left
    if (not hull.empty()) {
      hull.back().last = intersect(hull.back(), cur);
      while (hull.size() > 1 and hull[hull.size() - 2].last >= hull.back().last) {
        hull.pop_back();
        hull.back().last = intersect(hull.back(), cur);
      }
      if (hull.back().last == -inf) {
        hull.pop_back();
      }
    }
    hull.push_back(move(cur));
  }
  // query for max at x
  T query(const T& x) const {
    assert(not hull.empty());
    auto line = std::lower_bound(hull.begin(), hull.end(), x);
    return line->m * x + line->b;
  }
};

template <typename T>
using monotone_hull_max = monotone_hull<T>;

template <typename T>
struct monotone_hull_min : monotone_hull<T> {
  void insert(const T& m, const T& b) {
    return monotone_hull<T>::insert(-m, -b);
  }
  T query(const T& x) const {
    return -monotone_hull<T>::query(x);
  }
};

