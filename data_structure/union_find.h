/* Union Find
 * USAGE
 *  union_find dsu(n);
 * MEMBERS
 *  bool link(a, b, temporary?); // link a -> b
 *  int find(int); (also operator [])
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
#include <tuple>
#include <vector>

template <bool persistent = false>
struct union_find {
  std::vector<int> parent, _size;
  union_find(int n): parent(n, -1), _size(n, 1) {}
  int operator [] (int i) { return find(i); }
  int size() const { return (int)parent.size(); }
  int size(int i) { return _size[find(i)]; }
  int find(int i) {
    if (parent[i] == -1) return i;
    return parent[i] = find(parent[i]);
  }
  bool link(int from, int to) {
    from = find(from);
    to = find(to);
    if (from == to) return false;
    parent[from] = to;
    _size[to] += _size[from];
    return true;
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

template <>
struct union_find<true> {
  std::vector<int> parent, _size;
  std::vector<std::tuple<int, int, int, int>> memo;
  union_find(int n): parent(n, -1), _size(n, 1) {}
  int operator [] (int i) { return find(i); }
  int size() const { return (int)parent.size(); }
  int size(int i) { return _size[find(i)]; }
  int find(int i) {
    if (parent[i] == -1) return i;
    if (memo.empty()) return parent[i] = find(parent[i]);
    return find(parent[i]);
  }
  bool link(int from, int to, bool temporary = false) {
    from = find(from);
    to = find(to);
    if (from == to) return false;
    if (_size[from] > _size[to]) std::swap(from, to);
    if (temporary) {
      memo.emplace_back(from, parent[from], to, _size[to]);
    }
    parent[from] = to;
    _size[to] += _size[from];
    return true;
  }
  void unlink(int k = 0) {
    while (k--) {
      const auto& [i, root_i, j, size_j] = memo.back();
      parent[i] = root_i;
      _size[j] = size_j;
      memo.pop_back();
    }
  }
};

