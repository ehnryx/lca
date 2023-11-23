/* Rooted Tree
 * USAGE
 *  rooted_tree tree(adj, root);
 * CONSTRUCTOR
 *  adj: adjacency list of an unweighted undirected tree
 *  root: the root of the tree
 * MEMBERS
 *  depth: depth[u] = depth of u, depth[root] = 0
 *  parent: parent[u] = parent of u, parent[root] = -1
 *  subtree: subtree[u] = size of the subtree rooted at u,
 *    subtree[root] = size of tree
 *  preorder: vertices in pre-order traversal order
 *  in: in[u] = time at which we enter the subtree rooted at u
 *  out: out[u] = time at which we leave the subtree rooted at u
 * TIME
 *  O(N) construction
 *  N = #vertices
 * STATUS
 *  tested: kattis/tourists
 */
#pragma once

#include <vector>

struct rooted_tree {
  std::vector<std::vector<int>> adj;
  std::vector<int> parent, depth, subtree, start, preorder, in, out;
  int root;

  rooted_tree(const std::vector<std::vector<int>>& adj_list, int r)
      : adj(adj_list), parent(size(), -1), depth(size()), subtree(size()), start(size()),
        in(size()), out(size()), root(r) {
    preorder.reserve(size());
    build(root, -1, 0);
  }
  rooted_tree(std::vector<std::vector<int>>&& adj_list, int r)
      : adj(std::move(adj_list)), parent(size(), -1), depth(size()), subtree(size()),
        start(size()), in(size()), out(size()), root(r) {
    preorder.reserve(size());
    build(root, -1, 0);
  }

  const std::vector<int>& operator[](int i) const { return adj[i]; }
  size_t size() const { return adj.size(); }

  bool is_ancestor_of(int anc, int v, bool strict = true) const {
    if (strict) return in[anc] < in[v] && out[anc] > out[v];
    else return in[anc] <= in[v] && out[anc] >= out[v];
  }

 private:
  int build(int u, int par, int idx) {
    in[u] = idx++;
    start[u] = (int)preorder.size();
    preorder.push_back(u);
    parent[u] = par;
    subtree[u] = 1;
    if (par != -1) {
      depth[u] = depth[par] + 1;
      std::vector<int>::iterator it = find(adj[u].begin(), adj[u].end(), par);
      if (it != adj[u].end()) {
        adj[u].erase(it);
      }
    }
    for (int v : adj[u]) {
      idx = build(v, u, idx);
      subtree[u] += subtree[v];
    }
    out[u] = idx++;
    return idx;
  }
};
