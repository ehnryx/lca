/* Union Find
 * USAGE
 *  union_find dsu(n);
 * MEMBERS
 *  bool link(a, b, temporary?); // link a -> b
 *  int find(int); (also operator[])
 *  void unlink();
 *  vector<int> parent, _size;
 * TIME
 *  O(logN) find, link
 *  O(1) amortized unlink
 *  N = |dsu|
 * STATUS
 *  untested
 */
#pragma once

#include <stdexcept>
#include <vector>

struct union_find_base {
  std::vector<int> parent, _size;
  union_find_base(int n) : parent(n, -1), _size(n, 1) {}
  int operator[](int i) { return find(i); }
  int size() const { return (int)parent.size(); }
  int size(int i) { return _size[find(i)]; }
  int find(int i) {
    if (parent[i] == -1) return i;
    return parent[i] = find(parent[i]);
  }
  void reroot(int root) {
    parent[find(root)] = root;
    parent[root] = -1;
  }
  void reroot(int old, int root) {
    if (parent[old] != -1) [[unlikely]] {
      throw std::invalid_argument("parent[old root] != -1");
    }
    parent[old] = root;
    parent[root] = -1;
  }
};

template <typename data_t = void>
struct union_find : union_find_base {
  std::vector<data_t> data;
  union_find(int n) : union_find_base(n), data(n) {}
  union_find(int n, data_t init) : union_find_base(n), data(n, init) {}
  union_find(const std::vector<data_t>& d) : union_find_base(d.size()), data(d) {}
  union_find(std::vector<data_t>&& d) : union_find_base(d.size()), data(move(d)) {}
  data_t& get(int i) { return data[i]; }
  template <typename... Args>
  bool link(int from, int to, Args&&... args) {
    from = find(from);
    to = find(to);
    if (from == to) return false;
    if (_size[to] < _size[from]) {
      std::swap(to, from);
      data_t::swap_args(args...);
    }
    parent[from] = to;
    _size[to] += _size[from];
    data[to].merge(data[from], forward<Args>(args)...);
    return true;
  }
};

template <>
struct union_find<void> : union_find_base {
  using union_find_base::union_find_base;
  bool link(int from, int to) {
    from = find(from);
    to = find(to);
    if (from == to) return false;
    parent[from] = to;
    _size[to] += _size[from];
    return true;
  }
};
