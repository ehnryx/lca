/* Union Find
 * USAGE
 *  union_find dsu(n);
 * MEMBERS
 *  bool link(a, b, temporary?); // link a -> b
 *  int find(int); (also operator [])
 *  void unlink();
 *  vector<int> root, size;
 * TIME
 *  O(logN) find, link
 *  O(1) amortized unlink
 *  N = |dsu|
 * STATUS
 *  untested
 */
#pragma once

template <bool by_weight = false, bool persistent = false, typename = void>
struct union_find {
  vector<int> dsu, size;
  union_find(int n): dsu(n, -1), size(n, 1) {}
  int operator [] (int i) { return find(i); }
  int find(int i) {
    if (dsu[i] == -1) return i;
    return dsu[i] = find(dsu[i]);
  }
  bool link(int from, int to) {
    from = find(from);
    to = find(to);
    if (from == to) return false;
    if constexpr (by_weight) {
      if (size[from] > size[to]) swap(from, to);
    }
    dsu[from] = to;
    size[to] += size[from];
    return true;
  }
  template <bool can_reroot = !by_weight>
  enable_if_t<can_reroot> reroot(int root) {
    dsu[find(root)] = root;
    dsu[root] = -1;
  }
  template <bool can_reroot = !by_weight>
  enable_if_t<can_reroot> reroot(int old, int root) {
    if (dsu[old] != -1) [[unlikely]] throw invalid_argument("dsu[old root] != -1");
    dsu[old] = root;
    dsu[root] = -1;
  }
};

template <bool by_weight, bool persistent>
struct union_find<by_weight, persistent, enable_if_t<persistent>> {
  static_assert(by_weight);  // needs to be by weight if persistent
  vector<int> dsu, size;
  vector<tuple<int, int, int, int>> memo;
  union_find(int n): dsu(n, -1), size(n, 1) {}
  int operator [] (int i) { return find(i); }
  int find(int i) {
    if (dsu[i] == -1) return i;
    if (memo.empty()) return dsu[i] = find(dsu[i]);
    return find(dsu[i]);
  }
  bool link(int from, int to, bool temporary = false) {
    from = find(from);
    to = find(to);
    if (from == to) return false;
    if (size[from] > size[to]) swap(from, to);
    if (temporary) {
      memo.emplace_back(from, dsu[from], to, size[to]);
    }
    dsu[from] = to;
    size[to] += size[from];
    return true;
  }
  void unlink(int k = 0) {
    while (k--) {
      const auto& [i, root_i, j, size_j] = memo.back();
      dsu[i] = root_i;
      size[j] = size_j;
      memo.pop_back();
    }
  }
};

