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
  vector<vector<int>> jump;
  lca_binary_jumping(const vector<vector<int>>& adj, int root) :
      rooted_tree(adj, root) {
    L = 32 - __builtin_clz((int)size());
    jump.resize(size(), vector<int>(L, -1));
    for (int u : preorder) {
      jump[u][0] = parent[u];
      for (int j = 1; j < L && jump[u][j-1] != -1; j++) {
        jump[u][j] = jump[jump[u][j-1]][j-1];
      }
    }
  }
  int lca(int a, int b) {
    if (depth[a] < depth[b]) std::swap(a, b);
    for (int j = L - 1; j >= 0; j--) {
      if (depth[a] - (1 << j) >= depth[b]) {
        a = jump[a][j];
      }
    }
    if (a == b) return a;
    for (int j = L - 1; j >= 0; j--) {
      if (jump[a][j] != jump[b][j]) {
        a = jump[a][j];
        b = jump[b][j];
      }
    }
    return jump[a][0];
  }
};

