/* Li Chao Tree
 * USAGE
 *  li_chao_tree<less<>, T> segtree(query_points);
 * MEMBERS
 *  add_line(Line{m, b});  // add a line
 *  query(x);  // query answer at x
 * TIME
 *  O(logN) per query
 * STATUS
 *  tested: cf/319C
 */
#pragma once

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

template <typename Compare, typename T>
struct li_chao_tree {
  struct Line {
    T m, b;  // y = mx + b
    T eval(T x) const { return m * x + b; }
  };
  const std::vector<T> query_points;
  std::vector<Line> data;

  li_chao_tree(const std::vector<T>& qp)
      : query_points(_clean_query_points(qp)),
        data(2 * _get_capacity(query_points.size()), init_line) {}

  void add_line(Line v) { return _add_line(v, 1, 0, query_points.size() - 1); }
  void _add_line(Line v, int i, int left, int right) {
    if (left == right) {
      if (Compare()(v.eval(query_points[left]), data[i].eval(query_points[left]))) {
        data[i] = v;
      }
      return;
    }
    int mid = (left + right) / 2;
    bool left_better = Compare()(v.eval(query_points[left]), data[i].eval(query_points[left]));
    bool mid_better = Compare()(v.eval(query_points[mid]), data[i].eval(query_points[mid]));
    if (mid_better) std::swap(data[i], v);
    if (left_better == mid_better) {
      _add_line(v, 2 * i + 1, mid + 1, right);
    } else {
      _add_line(v, 2 * i, left, mid);
    }
  }

  void add_line_segment(Line v, int xl, int xr) {
    return _add_line_segment(v, xl, xr, 1, 0, query_points.size() - 1);
  }
  void _add_line_segment(Line v, int xl, int xr, int i, int left, int right) {
    if (xl <= left and right <= xr) return _add_line(v, i, left, right);
    int mid = (left + right) / 2;
    if (xl <= mid) _add_line_segment(v, xl, xr, 2 * i, left, mid);
    if (xr > mid) _add_line_segment(v, xl, xr, 2 * i + 1, mid + 1, right);
  }

  T query(T x) {
    auto it = std::lower_bound(query_points.begin(), query_points.end(), x);
    if (it == query_points.end() or *it != x)
      throw std::invalid_argument("query(x) is not at a query_point");
    return _query(std::distance(query_points.begin(), it), 1, 0, query_points.size() - 1);
  }
  T _query(int x, int i, int left, int right) {
    if (left == right) return data[i].eval(query_points[x]);
    int mid = (left + right) / 2;
    if (x <= mid) {
      return std::min(data[i].eval(query_points[x]), _query(x, 2 * i, left, mid), Compare());
    } else {
      return std::min(
          data[i].eval(query_points[x]), _query(x, 2 * i + 1, mid + 1, right), Compare());
    }
  }

 private:
  static constexpr Line _get_init_line() {
    return Line{
        T(0),
        Compare()(std::numeric_limits<T>::min(), std::numeric_limits<T>::max())
            ? std::numeric_limits<T>::max()
            : std::numeric_limits<T>::min(),
    };
  }
  static int _get_capacity(int n) { return 1 << (32 - __builtin_clz(std::max(n - 1, 1))); }
  static std::vector<T> _clean_query_points(std::vector<T> v) {
    std::sort(v.begin(), v.end());
    v.resize(std::unique(v.begin(), v.end()) - v.begin());
    return v;
  }

 public:
  static constexpr Line init_line = _get_init_line();
};
