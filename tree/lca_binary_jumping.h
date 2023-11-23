/* Lowest Common Ancestor (Binary Jumping)
 * USAGE
 *  lca_binary_jumping tree(adj, root);
 *  int lca = tree.lca(a, b);
 * CONSTRUCTOR
 *  adj: adjacency list of an unweighted undirected tree
 *  root: the root of the tree
 * INPUT
 *  vertices a and b, returns the lowest common ancestor of a and b
 * TIME
 *  O(NlogN) construction, O(logN) per lca query
 *  N = #vertices
 * STATUS
 *  tested: kattis/tourists
 */
#pragma once

#include "rooted_tree.h"

struct lca_binary_jumping : rooted_tree {
  int L;
  std::vector<std::vector<int>> jump;
  lca_binary_jumping(const std::vector<std::vector<int>>& adj_list, int r)
      : rooted_tree(adj_list, r) {
    build();
  }
  lca_binary_jumping(std::vector<std::vector<int>>&& adj_list, int r)
      : rooted_tree(std::move(adj_list), r) {
    build();
  }

  int lca(int a, int b) const {
    if (depth[a] < depth[b]) std::swap(a, b);
    for (int j = L - 1; j >= 0; j--) {
      if (depth[a] - (1 << j) >= depth[b]) {
        a = jump[j][a];
      }
    }
    if (a == b) return a;
    for (int j = L - 1; j >= 0; j--) {
      if (jump[j][a] != jump[j][b]) {
        a = jump[j][a];
        b = jump[j][b];
      }
    }
    return parent[a];
  }
  int distance(int a, int b) const { return depth[a] + depth[b] - 2 * depth[lca(a, b)]; }

 private:
  void build() {
    L = 32 - __builtin_clz((int)adj.size());
    jump.resize(L, std::vector<int>(adj.size(), -1));
    for (int u : preorder) {
      jump[0][u] = parent[u];
      for (int j = 1; j < L && jump[j - 1][u] != -1; j++) {
        jump[j][u] = jump[j - 1][jump[j - 1][u]];
      }
    }
  }
};
