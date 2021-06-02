/* Range Query Tree (combined heavy light and euler tour)
 * USAGE
 *  range_query_tree<DS, Query_t> tree(adj, root);
 *    template: DS is a data structure, Query_t is the output type
 *    DS should have the following member functions:
 *      query_range, query_point, insert_range, insert_point
 * CONSTRUCTOR
 *  adj: adjacency list of an unweighted undirected tree
 *  root: the root of the tree
 * MEMBERS
 *  lca(a, b) returns lca
 *  query_point(v), update_point(v, args),
 *  query_subtree(v), update_subtree(v, args),
 *  query_path(a, b, include_lca?, init_value, merge_func, args),
 *  update_path(a, b, include_lca?, args),
 *  search_path(a, b, include_lca?, args), searches on the path a->b
 * TIME
 *  O(N) construction,
 *  O(logN) per point/subtree/lca query
 *  O((logN)^2) per path query
 *  N = #vertices
 * STATUS
 *  tested: kattis/tourists, cf/1528c, spoj/qtree
 */
#pragma once

#include "rooted_tree.h"

template <class DS, typename Query_t>
struct range_query_tree : rooted_tree {
  DS range_ds;
  vector<int> top; // top of heavy chains
  range_query_tree(const vector<vector<int>>& adj_list, int root):
    rooted_tree(adj_list, root), range_ds((int)adj.size()), top(adj.size()) {
    preorder.clear();
    top[root] = root;
    build(root, 0);
  }
  range_query_tree(vector<vector<int>>&& adj_list, int root):
    rooted_tree(move(adj_list), root), range_ds((int)adj.size()), top(adj.size()) {
    preorder.clear();
    top[root] = root;
    build(root, 0);
  }

  int lca(int u, int v) const {
    while (top[u] != top[v]) {
      if (depth[top[u]] < depth[top[v]]) swap(u, v);
      u = parent[top[u]];
    }
    return depth[u] < depth[v] ? u : v;
  }

  template <class... Args>
  void update_point(int u, const Args&... args) {
    range_ds.update_point(start[u], args...);
  }
  template <class... Args>
  Query_t query_point(int u, const Args&... args) {
    return range_ds.query_point(start[u], args...);
  }

  template <class... Args>
  void update_subtree(int u, const Args&... args) {
    range_ds.update_range(start[u], start[u] + subtree[u] - 1, args...);
  }
  template <class... Args>
  Query_t query_subtree(int u, const Args&... args) {
    return range_ds.query_range(start[u], start[u] + subtree[u] - 1, args...);
  }

  template <class... Args>
  int update_path(int u, int v, bool include_lca, const Args&... args) {
    while (top[u] != top[v]) {
      if (depth[top[u]] < depth[top[v]]) swap(u, v);
      range_ds.update_range(start[top[u]], start[u], args...);
      u = parent[top[u]];
    }
    if (include_lca || u != v) {
      if (depth[u] < depth[v]) swap(u, v);
      range_ds.update_range(start[v] + !include_lca, start[u], args...);
    }
    return v; // return the lowest common ancestor
  }
  template <class Combine, class... Args>
  Query_t query_path(int u, int v, bool include_lca, Query_t res,
                     const Combine& merge, const Args&... args) {
    while (top[u] != top[v]) {
      if (depth[top[u]] < depth[top[v]]) swap(u, v);
      res = merge(res, range_ds.query_range(start[top[u]], start[u], args...));
      u = parent[top[u]];
    }
    if (include_lca || u != v) {
      if (depth[u] < depth[v]) swap(u, v);
      res = merge(res, range_ds.query_range(start[v] + !include_lca, start[u], args...));
    }
    return res;
  }

  template <class... Args>
  int search_path(int u, int v, bool include_lca, Args... args) {
    bool rev = false;
    vector<pair<int, int>> down;
    while (top[u] != top[v]) {
      if (depth[top[u]] < depth[top[v]]) {
        swap(u, v);
        rev ^= 1;
      }
      int left = start[top[u]];
      int right = start[u];
      if (rev) {
        down.emplace_back(left, right);
      } else {
        int res = range_ds.search_right_mutable(left, right, args...);
        if (res != range_ds.lim) return preorder[res];
      }
      u = parent[top[u]];
    }
    if (include_lca || u != v) {
      if (depth[u] < depth[v]) {
        swap(u, v);
        rev ^= 1;
      }
      int left = start[v] + !include_lca;
      int right = start[u];
      if (rev) {
        down.emplace_back(left, right);
      } else {
        int res = range_ds.search_right_mutable(left, right, args...);
        if (res != range_ds.lim) return preorder[res];
      }
    }
    for (auto it = down.rbegin(); it != down.rend(); it++) {
      int res = range_ds.search_left_mutable(it->first, it->second, args...);
      if (res != range_ds.lim) return preorder[res];
    }
    return -1;
  }

private:
  int build(int u, int idx) {
    in[u] = idx++;
    start[u] = (int)preorder.size();
    preorder.push_back(u);
    if (!adj[u].empty()) {
      pair<int, size_t> big;
      for (size_t i = 0; i < adj[u].size(); i++) {
        big = max(big, pair(subtree[adj[u][i]], i));
      }
      if (big.second > 0) {
        swap(adj[u][0], adj[u][big.second]);
      }
      // continue heavy chain
      top[adj[u].front()] = top[u];
      idx = build(adj[u].front(), idx);
      // start new chain
      for (size_t i = 1; i < adj[u].size(); i++) {
        int v = adj[u][i];
        top[v] = v;
        idx = build(v, idx);
      }
    }
    out[u] = idx++;
    return idx;
  }
};

