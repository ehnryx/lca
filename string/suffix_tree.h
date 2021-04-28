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
 *  tested: nadc21/b
 */
#pragma once

#include "suffix_array.h"

template <typename T>
struct suffix_tree {
  basic_string<T> t;
  struct suffix_node : vector<pair<T, int>> {
    int parent, depth, left, right;
    suffix_node() = default;
    suffix_node(int p, int d, int l, int r): parent(p), depth(d), left(l), right(r) {}
    int get(const T& c) const {
      auto res = find_if(
          vector<pair<T, int>>::begin(), vector<pair<T, int>>::end(),
          [=](const pair<T, int>& v) { return v.first == c; });
      return res == vector<pair<T, int>>::end() ? -1 : res->second;
    }
  };
  vector<suffix_node> nodes;

  suffix_tree(const basic_string<T>& s): t(s) {
    suffix_array sa(s);
    int n = (int)size(s);
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
        nodes.back().emplace_back(nodes[u].left + split == n ? 0 : s[nodes[u].left + split], u);
        nodes[u].depth += split;
        nodes[u].left += split;
        nodes[u].parent = (int)size(nodes) - 1;
        u = (int)size(nodes) - 1;
        for (auto& edge : nodes[parent(u)]) {
          if (edge.first == s[nodes[u].left]) {
            edge.second = u;
          }
        }
      }
      // add the remaining suffix as a child
      int suf = sa[i];
      nodes[suf] = suffix_node(u, depth(u) + split, suf + depth(u) + split, n);
      nodes[u].emplace_back(s[nodes[suf].left], suf);
      u = suf;
    }
  }

  int root() const { return (int)size(t); }
  int depth(int u) const { return nodes[u].depth; }
  int parent(int u) const { return nodes[u].parent; }
  int length(int u) const { return nodes[u].right - nodes[u].left; };
  bool is_leaf(int u) const { return u < root(); }
  const suffix_node& operator [] (int i) { return nodes.at(i); };

  // return: (node, idx in range[node]) of the past-the-end of the match.
  //         (-1, -1) if not matched
  pair<int, int> match(const string& s) const {
    int u = root();
    int idx = 0;
    for (int i = 0; i < (int)size(s); i++) {
      if (idx == nodes[u].right) {
        u = nodes[u].get(s[i]);
        if (u == -1) {
          return pair(-1, -1);
        }
        idx = nodes[u].left;
      }
      if (s[i] == t[idx]) {
        idx++;
      } else {
        return pair(-1, -1);
      }
    }
    return pair(u, idx);
  }
};

