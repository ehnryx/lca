/* Persistent Li Chao Tree
 * USAGE
 *  persistent_li_chao_tree<less<>, T> segtree(query_points);
 * MEMBERS
 *  add_line(parent_id, Line{m, b});  // add a line
 *  query(id, x);  // query answer at x
 * TIME
 *  O(logN) per query
 * STATUS
 *  untested: boj/12876
 */
#pragma once

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

template <typename Compare, typename T>
struct persistent_li_chao_tree {
  struct Line {
    T m, b;  // y = mx + b
    T eval(T x) const { return m * x + b; }
  };
  struct Node {
    Line v;
    int li = -1, ri = -1;
  };
  const std::vector<T> query_points;
  std::vector<Node> data;
  std::vector<int> roots;
  std::vector<int> freei;

  persistent_li_chao_tree(const std::vector<T>& qp, int reserve = 0):
    query_points(_clean_query_points(qp)), data(1, Node{init_line}), roots(1, 0) {
    if (reserve) {
      data.reserve(reserve);
      freei.reserve(reserve);
    }
  }

  void free_nodes(std::vector<int> to_free) {
    freei.insert(freei.end(), to_free.begin(), to_free.end());
  }
  template <typename... MaybeRecord>
  int make_node(Node&& v, MaybeRecord&... version_nodes) {
    static_assert(sizeof...(MaybeRecord) <= 1);
    if (freei.empty()) {
      data.push_back(std::move(v));
      (version_nodes.push_back(data.size() - 1), ...);
      return data.size() - 1;
    }
    int i = freei.back();
    freei.pop_back();
    data[i] = std::move(v);
    (version_nodes.push_back(i), ...);
    return i;
  }

  template <typename... MaybeRecord>
  int add_line(int id, Line v, MaybeRecord&... version_nodes) {
    static_assert(sizeof...(MaybeRecord) <= 1);
    roots.push_back(_add_line(v, roots[id], 0, query_points.size() - 1, version_nodes...));
    return roots.size() - 1;
  }
  template <typename... MaybeRecord>
  int _add_line(Line v, int i, int left, int right, MaybeRecord&... version_nodes) {
    static_assert(sizeof...(MaybeRecord) <= 1);
    if (i == -1) {
      return make_node(Node{v}, version_nodes...);
    }
    if (left == right) {
      if (Compare()(v.eval(query_points[left]), data[i].v.eval(query_points[left]))) {
        return make_node(Node{v}, version_nodes...);
      }
      return make_node(Node{data[i].v}, version_nodes...);
    }
    int mid = (left + right) / 2;
    bool left_better = Compare()(v.eval(query_points[left]), data[i].v.eval(query_points[left]));
    bool mid_better = Compare()(v.eval(query_points[mid]), data[i].v.eval(query_points[mid]));
    int ni = make_node(Node{data[i]}, version_nodes...);
    if (mid_better) std::swap(data[ni].v, v);
    if (left_better == mid_better) {
      data[ni].ri = _add_line(v, data[i].ri, mid + 1, right, version_nodes...);
    } else {
      data[ni].li = _add_line(v, data[i].li, left, mid, version_nodes...);
    }
    return ni;
  }

  T query(int id, T x) {
    auto it = std::lower_bound(query_points.begin(), query_points.end(), x);
    if (it == query_points.end() or *it != x) throw std::invalid_argument("query(x) is not at a query_point");
    if (id >= roots.size()) throw std::invalid_argument("query(x) for a version that does not exist");
    return _query(std::distance(query_points.begin(), it), roots[id], 0, query_points.size() - 1);
  }
  T _query(int x, int i, int left, int right) {
    if (i == -1) return init_line.b;
    if (left == right) return data[i].v.eval(query_points[x]);
    int mid = (left + right) / 2;
    if (x <= mid) {
      return std::min(data[i].v.eval(query_points[x]), _query(x, data[i].li, left, mid), Compare());
    } else {
      return std::min(data[i].v.eval(query_points[x]), _query(x, data[i].ri, mid + 1, right), Compare());
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
  static std::vector<T> _clean_query_points(std::vector<T> v) {
    std::sort(v.begin(), v.end());
    v.resize(std::unique(v.begin(), v.end()) - v.begin());
    return v;
  }

public:
  static constexpr Line init_line = _get_init_line();
};

