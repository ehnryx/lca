/* Range Query Tree (combined heavy light and euler tour)
 * USAGE
 *  range_query_tree<DS> tree(adj, root);
 *    template: DS is a data structure
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

#include <functional>

template <typename DS>
struct range_query_tree : rooted_tree {
  DS range_ds;
  std::vector<int> top; // top of heavy chains
  range_query_tree(const std::vector<std::vector<int>>& adj_list, int r):
    rooted_tree(adj_list, r), range_ds((int)adj.size()), top(adj.size()) {
    preorder.clear();
    top[root] = root;
    build_hld(root, 0);
  }
  range_query_tree(std::vector<std::vector<int>>&& adj_list, int r):
    rooted_tree(move(adj_list), r), range_ds((int)adj.size()), top(adj.size()) {
    preorder.clear();
    top[root] = root;
    build_hld(root, 0);
  }
  void build() { range_ds.build(); }
  void assign_lengths() { range_ds.assign_lengths(); }

  int ancestor(int u, int k) const {
    if (k < 0) throw std::invalid_argument("tried to find the kth ancestor where k < 0");
    if (k > depth[u]) return -1;
    while (true) {
      int len = depth[u] - depth[top[u]] + 1;
      if (k < len) {
        return preorder[start[u] - k];
      }
      u = parent[top[u]];
      k -= len;
    }
  }
  int lca(int u, int v) const {
    while (top[u] != top[v]) {
      if (depth[top[u]] < depth[top[v]]) std::swap(u, v);
      u = parent[top[u]];
    }
    return depth[u] < depth[v] ? u : v;
  }
  int distance(int u, int v) const {
    int c = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[c];
  }
  // k is 0 indexed (0th vertex on path u->v is u)
  int kth(int u, int v, int k) const {
    int c = lca(u, v);
    int up = depth[u] - depth[c];
    int down = depth[v] - depth[c];
    if (k <= up) {
      return ancestor(u, k);
    } else {
      return ancestor(v, up + down - k);
    }
  }

  template <typename... Args>
  void update_point(int u, const Args&... args) {
    range_ds.update_point(start[u], args...);
  }
  template <typename... Args>
  auto query_point(int u, const Args&... args) {
    return range_ds.query_point(start[u], args...);
  }

  template <typename... Args>
  void update_up_point(int u, const Args&... args) {
    range_ds.update_up(start[u], args...);
  }
  template <typename... Args>
  auto query_up_point(int u, const Args&... args) {
    return range_ds.query_up(start[u], args...);
  }

  template <typename... Args>
  void update_subtree(int u, const Args&... args) {
    range_ds.update_range(start[u], start[u] + subtree[u] - 1, args...);
  }
  template <typename... Args>
  auto query_subtree(int u, const Args&... args) {
    return range_ds.query_range(start[u], start[u] + subtree[u] - 1, args...);
  }
  template <typename... Args>
  int search_subtree(int u, const Args&... args) {
    int idx = range_ds.search_left(start[u], start[u] + subtree[u] - 1, args...);
    return idx < range_ds.lim ? preorder[idx] : -1;
  }

  template <typename... Args>
  void update_non_subtree(int u, const Args&... args) {
    range_ds.update_range(0, start[u] - 1, args...);
    range_ds.update_range(start[u] + subtree[u], subtree[preorder[0]] - 1, args...);
  }
  template <typename Combine, typename... Args>
  auto query_non_subtree(
    int u, Combine&& merge, const Args&... args) {
    return merge(
      range_ds.query_range(0, start[u] - 1, args...),
      range_ds.query_range(start[u] + subtree[u], subtree[preorder[0]] - 1, args...));
  }
  template <typename... Args>
  int search_non_subtree(int u, Args... args) {
    int idx = range_ds.search_left(0, start[u] - 1, args...);
    if (idx < range_ds.lim) return preorder[idx];
    idx = range_ds.search_left(start[u] + subtree[u], subtree[preorder[0]] - 1, args...);
    return idx < range_ds.lim ? preorder[idx] : -1;
  }

  struct path_params {
    int u, v;
    bool include_lca;
  };

  template <typename... Args>
  int update_path(path_params params, const Args&... args) {
    while (top[params.u] != top[params.v]) {
      if (depth[top[params.u]] < depth[top[params.v]]) std::swap(params.u, params.v);
      range_ds.update_range(start[top[params.u]], start[params.u], args...);
      params.u = parent[top[params.u]];
    }
    if (params.include_lca || params.u != params.v) {
      if (depth[params.u] < depth[params.v]) std::swap(params.u, params.v);
      range_ds.update_range(start[params.v] + !params.include_lca, start[params.u], args...);
    }
    return params.v; // return the lowest common ancestor
  }
  template <typename out_t, typename Combine, typename... Args>
  auto query_path(path_params params, out_t init, Combine&& merge, const Args&... args) {
    auto res = init;
    while (top[params.u] != top[params.v]) {
      if (depth[top[params.u]] < depth[top[params.v]]) std::swap(params.u, params.v);
      res = merge(res, range_ds.query_range(start[top[params.u]], start[params.u], args...));
      params.u = parent[top[params.u]];
    }
    if (params.include_lca || params.u != params.v) {
      if (depth[params.u] < depth[params.v]) std::swap(params.u, params.v);
      res = merge(res, range_ds.query_range(start[params.v] + !params.include_lca, start[params.u], args...));
    }
    return res;
  }

  template <typename... Args>
  int search_path(path_params params, Args... args) {
    bool rev = false;
    std::vector<std::pair<int, int>> down;
    while (top[params.u] != top[params.v]) {
      if (depth[top[params.u]] < depth[top[params.v]]) {
        std::swap(params.u, params.v);
        rev ^= 1;
      }
      int left = start[top[params.u]];
      int right = start[params.u];
      if (rev) {
        down.emplace_back(left, right);
      } else {
        int res = range_ds.search_right_mutable(left, right, args...);
        if (res != range_ds.lim) return preorder[res];
      }
      params.u = parent[top[params.u]];
    }
    if (params.include_lca || params.u != params.v) {
      if (depth[params.u] < depth[params.v]) {
        std::swap(params.u, params.v);
        rev ^= 1;
      }
      int left = start[params.v] + !params.include_lca;
      int right = start[params.u];
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
  int build_hld(int u, int idx) {
    in[u] = idx++;
    start[u] = (int)preorder.size();
    preorder.push_back(u);
    if (!adj[u].empty()) {
      std::pair<int, size_t> big;
      for (size_t i = 0; i < adj[u].size(); i++) {
        big = std::max(big, std::pair(subtree[adj[u][i]], i));
      }
      if (big.second > 0) {
        std::swap(adj[u][0], adj[u][big.second]);
      }
      // continue heavy chain
      top[adj[u].front()] = top[u];
      idx = build_hld(adj[u].front(), idx);
      // start new chain
      for (size_t i = 1; i < adj[u].size(); i++) {
        int v = adj[u][i];
        top[v] = v;
        idx = build_hld(v, idx);
      }
    }
    out[u] = idx++;
    return idx;
  }
};

