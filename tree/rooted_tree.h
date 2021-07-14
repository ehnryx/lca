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

struct rooted_tree {
  vector<vector<int>> adj;
  vector<int> parent, depth, subtree, start, preorder, in, out;
  const vector<int>& operator [] (int i) const { return adj[i]; }
  rooted_tree(const vector<vector<int>>& adj_list, int root): adj(adj_list),
    parent(adj.size(), -1), depth(adj.size()), subtree(adj.size()), start(adj.size()),
    in(adj.size()), out(adj.size()) {
    preorder.reserve(adj.size());
    build(root, -1, 0);
  }
  rooted_tree(vector<vector<int>>&& adj_list, int root): adj(move(adj_list)),
    parent(adj.size(), -1), depth(adj.size()), subtree(adj.size()), start(adj.size()),
    in(adj.size()), out(adj.size()) {
    preorder.reserve(adj.size());
    build(root, -1, 0);
  }

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
      vector<int>::iterator it = find(adj[u].begin(), adj[u].end(), par);
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

