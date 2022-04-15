/* Rooted Tree (with node values)
 * USAGE
 *  rooted_tree_memo<node_t> tree(adj, root, values);
 *  tree.get(i) <- value
 * CONSTRUCTOR
 *  adj: adjacency list of an unweighted undirected tree
 *  root: the root of the tree
 *  values: node_t
 * TIME
 *  O(N) construction
 *  N = #vertices
 * STATUS
 *  untested
 */
#pragma once

#include "rooted_tree.h"

template <typename node_t>
struct rooted_tree_memo_base : rooted_tree {
  std::vector<node_t> data;
  rooted_tree_memo_base(const std::vector<std::vector<int>>& adj_list, int r):
    rooted_tree(adj_list, r), data(size()) {}
  rooted_tree_memo_base(
      const std::vector<std::vector<int>>& adj_list, int r,
      const std::vector<node_t>& init):
    rooted_tree(adj_list, r), data(init) {}

  node_t& get(int u) { return data[u]; }
  const node_t& get(int u) const { return data[u]; }
  void init(int u, const node_t& val) { data[u] = val; }
  void solve() { _solve(root); }
  node_t solve_and_return() {
    solve();
    return data[root];
  }

  virtual void _solve(int u) = 0;
};

template <typename node_t, typename temp_t = void>
struct rooted_tree_memo : rooted_tree_memo_base<node_t> {
  using base = rooted_tree_memo_base<node_t>;
  using base::rooted_tree_memo_base;
  using base::data, base::adj;

  void _solve(int u) override final {
    temp_t tmp = {};
    before_children(u, data[u], tmp);
    for(int v : adj[u]) {
      _solve(v);
      for_each_child(u, data[u], v, data[v], tmp);
    }
    after_children(u, data[u], tmp);
  }

  virtual void before_children(
      [[maybe_unused]] int u, [[maybe_unused]] node_t& cur,
      [[maybe_unused]] temp_t& tmp) {}
  virtual void for_each_child(
      [[maybe_unused]] int u, [[maybe_unused]] node_t& cur,
      [[maybe_unused]] int v, [[maybe_unused]] node_t& child,
      [[maybe_unused]] temp_t& tmp) {}
  virtual void after_children(
      [[maybe_unused]] int u, [[maybe_unused]] node_t& cur,
      [[maybe_unused]] temp_t& tmp) {}
};

template <typename node_t>
struct rooted_tree_memo<node_t, void> : rooted_tree_memo_base<node_t> {
  using base = rooted_tree_memo_base<node_t>;
  using base::rooted_tree_memo_base;
  using base::data, base::adj;

  void _solve(int u) override final {
    before_children(u, data[u]);
    for(int v : adj[u]) {
      _solve(v);
      for_each_child(u, data[u], v, data[v]);
    }
    after_children(u, data[u]);
  }

  virtual void before_children(
      [[maybe_unused]] int u, [[maybe_unused]] node_t& cur) {}
  virtual void for_each_child(
      [[maybe_unused]] int u, [[maybe_unused]] node_t& cur,
      [[maybe_unused]] int v, [[maybe_unused]] node_t& child) {}
  virtual void after_children(
      [[maybe_unused]] int u, [[maybe_unused]] node_t& cur) {}
};

