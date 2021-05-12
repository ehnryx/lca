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

template <bool by_weight = true>
struct union_find {
  vector<int> dsu, size;
  vector<tuple<int,int,int,int>> memo;
  union_find(int n): dsu(n, -1), size(n, 1) {}
  int operator [] (int i) { return find(i); }
  int find(int i) {
    if (dsu[i] == -1) return i;
    if (memo.empty()) return dsu[i] = find(dsu[i]);
    return find(dsu[i]);
  }
  bool link(int i, int j, bool temporary = false) {
    i = find(i);
    j = find(j);
    if (i == j) return false;
    if (by_weight && size[i] > size[j]) swap(i, j);
    if (temporary) {
      memo.emplace_back(i, dsu[i], j, size[j]);
    }
    dsu[i] = j;
    size[j] += size[i];
    return true;
  }
  void unlink() {
    for (int t = (int)memo.size(); t >= 0; t--) {
      const auto& [i, root_i, j, size_j] = memo[t];
      dsu[i] = root_i;
      size[j] = size_j;
    }
    memo.clear();
  }
};

