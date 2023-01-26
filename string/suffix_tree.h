/* Suffix Tree (offline build from suffix array)
 * USAGE
 *  suffix_tree tree(string_s)
 * INPUT
 *  string or basic_string<int>
 *  remap values to [1,n] if using basic_string<int>. DO NOT use 0
 * OUTPUT
 *  leaves 0-indexed as suffixes, n is the root
 *  note: a leaf has an empty range if it is the prefix of another suffix
 * TIME
 *  O(N)
 *  N = |string|
 * STATUS
 *  tested: nadc21/b, kattis/jobbdna
 */
#pragma once

#include "suffix_array.h"

template <template<typename> typename container_t, typename T>
struct suffix_tree {
  container_t<T> t;
  // suffix_node ranges are [inclusive, exclusive)
  struct suffix_node_item {
    T value; int to;
    suffix_node_item(T v, int t): value(v), to(t) {}
    bool operator<(T c) const { return value < c; }
  };
  struct suffix_node : std::vector<suffix_node_item> {
    int parent, depth, left, right;
    suffix_node() = default;
    suffix_node(int p, int d, int l, int r): parent(p), depth(d), left(l), right(r) {}
    int get(const T& c) const {
      auto res = std::lower_bound(this->begin(), this->end(), c);
      return res != this->end() && res->value == c ? res->to : -1;
    }
  };
  std::vector<suffix_node> nodes;

  suffix_tree(const container_t<T>& s): t(s) { build(); }
  suffix_tree(container_t<T>&& s): t(move(s)) { build(); }

  // BEGIN suffix tree functions
  // size of suffix tree
  int size() const { return (int)nodes.size(); }
  // length of string
  int strlen() const { return (int)t.size(); }
  // the root of the suffix tree
  int root() const { return (int)t.size(); }
  // the depth of the start of a vertex (the number of characters above it)
  int depth(int u) const { return nodes[u].depth; }
  // the parent of a vertex
  int parent(int u) const { return nodes[u].parent; }
  // the length of the corresponding substring
  int length(int u) const { return nodes[u].right - nodes[u].left; };
  // whether a vertex is a leaf
  bool is_leaf(int u) const { return u < root(); }
  // the nodes in order, use .get(char) to match a character
  const suffix_node& operator [] (int i) const { return nodes.at(i); };

  // return: (node, idx in range[node]) of the past-the-end of the match.
  //         (-1, -1) if not matched
  template <template<typename> typename container_u>
  std::pair<int, int> match(const container_u<T>& s) const {
    int u = root();
    int idx = 0;
    for (int i = 0; i < (int)s.size(); i++) {
      if (idx == nodes[u].right) {
        u = nodes[u].get(s[i]);
        if (u == -1) {
          return std::pair(-1, -1);
        }
        idx = nodes[u].left;
      }
      if (s[i] == t[idx]) {
        idx++;
      } else {
        return std::pair(-1, -1);
      }
    }
    return std::pair(u, idx);
  }
  // END suffix tree functions

private:
  void build() {
    suffix_array sa(t);
    int n = (int)t.size();
    nodes.reserve(2 * n);
    nodes.resize(n + 1);
    for (int i = 1, u = n; i <= n; i++) {
      // walk up to the lca
      while (u != n && sa.height[i] <= depth(u)) {
        u = parent(u);
      }
      // split the node if necessary
      int split = sa.height[i] - depth(u);
      if (split < length(u) || u < n) {
        nodes.emplace_back(parent(u), depth(u), nodes[u].left, nodes[u].left + split);
        nodes.back().emplace_back(nodes[u].left + split == n ? 0 : t[nodes[u].left + split], u);
        nodes[u].depth += split;
        nodes[u].left += split;
        nodes[u].parent = (int)nodes.size() - 1;
        u = (int)nodes.size() - 1;
        auto edge = std::lower_bound(
            nodes[parent(u)].begin(), nodes[parent(u)].end(), t[nodes[u].left]);
        edge->to = u;  // guaranteed to exist
      }
      // add the remaining suffix as a child
      int suf = sa[i];
      nodes[suf] = suffix_node(u, depth(u) + split, suf + depth(u) + split, n);
      nodes[u].emplace_back(t[nodes[suf].left], suf);
      u = suf;
    }
  }
};

