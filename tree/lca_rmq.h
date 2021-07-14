/* Lowest Common Ancestor (Range Minimum Query)
 * USAGE
 *  lca_rmq tree(adj, root);
 *  int lca = tree.lca(a, b);
 * CONSTRUCTOR
 *  adj: adjacency list of an unweighted undirected tree
 *  root: the root of the tree
 * INPUT
 *  vertices a and b, returns the lowest common ancestor of a and b
 * TIME
 *  O(NlogN) construction, O(1) per lca query
 *  N = #vertices
 * STATUS
 *  tested: kattis/tourists
 */
#pragma once

#include "rooted_tree.h"
#include "../data_structure/range_minimum_query.h"

struct lca_rmq : rooted_tree {
  vector<int> idx;
  range_minimum_query<pair<int, int>> rmq;
  lca_rmq(const vector<vector<int>>& adj_list, int root):
    rooted_tree(adj_list, root) {
    build();
  }
  lca_rmq(vector<vector<int>>&& adj_list, int root):
    rooted_tree(move(adj_list), root) {
    build();
  }

  int lca(int a, int b) const {
    int l = idx[a];
    int r = idx[b];
    return rmq.query(min(l, r), max(l, r) + 1).second;
  }
  int distance(int a, int b) const {
    return depth[a] + depth[b] - 2 * depth[lca(a, b)];
  }

private:
  void build() {
    idx.resize(adj.size());
    vector<pair<int, int>> arr;
    arr.reserve(2 * preorder.size());
    for (int i = 0; i < (int)preorder.size(); i++) {
      int u = preorder[i];
      if (i > 0) {
        for (int v = preorder[i-1]; depth[v] >= depth[u]; ) {
          v = parent[v];
          arr.emplace_back(depth[v], v);
        }
      }
      idx[u] = (int)arr.size();
      arr.emplace_back(depth[u], u);
    }
    rmq = range_minimum_query(move(arr));
  }
};

