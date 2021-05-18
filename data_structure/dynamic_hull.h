/* Dynamic Hull (Line Container)
 * USAGE
 *  dynamic_hull<T> hull;
 *  maintains the upper envelope of a set of lines
 * MEMBERS
 *  void insert(T m, T b); inserts the line y = mx + b
 *  T query(T x); queries for the maximum at x
 * TIME
 *  O(logN) insert, query
 *  N = |hull|
 * STATUS
 *  tested: cf/455e,1083e
 */
#pragma once

template <typename T>
struct dynamic_hull_line {
  T m, b;
  mutable T last;
  dynamic_hull_line(const T& _m, const T& _b): m(_m), b(_b), last(0) {}
  bool operator < (const dynamic_hull_line& o) const { return m < o.m; }
  bool operator < (const T& x) const { return last < x; }
};

template <typename T>
struct dynamic_hull : multiset<dynamic_hull_line<T>, less<>> {
  using base = multiset<dynamic_hull_line<T>, less<>>;
  using base_it = typename multiset<dynamic_hull_line<T>, less<>>::iterator;
  static constexpr T inf = numeric_limits<T>::has_infinity ?
    numeric_limits<T>::infinity() : numeric_limits<T>::max();
  template <typename U = T>
  static enable_if_t<is_floating_point_v<U>, T> div(const T& a, const T& b) {
    return a / b;
  }
  template <typename U = T>
  static enable_if_t<!is_floating_point_v<U>, T> div(const T& a, const T& b) {
    return a / b - ((a ^ b) < 0 && a % b != 0);
  }
  T intersect(base_it it, base_it jt) {
    if (jt == base::end()) return inf;
    if (it->m == jt->m) return it->b < jt->b ? -inf : inf;
    return div(it->b - jt->b, jt->m - it->m);
  }
  // add the line y = mx + b
  void insert(const T& m, const T& b) {
    auto it = base::insert(dynamic_hull_line<T>(m, b));
    it->last = intersect(it, next(it));
    // remove if covered
    if (it != base::begin() && intersect(prev(it), it) >= it->last) {
      base::erase(it);
      return;
    }
    // remove lines to the right
    while (next(it) != base::end() && it->last >= next(it)->last) {
      base::erase(next(it));
      it->last = intersect(it, next(it));
    }
    // remove lines to the left
    if (it != base::begin()) {
      prev(it)->last = intersect(prev(it), it);
      for (it = prev(it); it != base::begin() && prev(it)->last >= it->last; it--) {
        it = base::erase(it);
        prev(it)->last = intersect(prev(it), it);
      }
    }
    // remove garbage at front
    while (base::begin()->last == -inf) {
      base::erase(base::begin());
    }
  }
  // query for max at x
  T query(const T& x) const {
    assert(!base::empty());
    auto line = base::lower_bound(x);
    return line->m * x + line->b;
  }
};

