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
 * TIME
 *  O(N) construction
 *  N = #vertices
 * STATUS
 *  tested: kattis/tourists
 */
#pragma once

struct rooted_tree : vector<vector<int>> {
  vector<int> parent, depth, subtree, preorder;
  rooted_tree(const vector<vector<int>>& adj, int root) : vector<vector<int>>(adj) {
    parent.resize(size(), -1);
    depth.resize(size());
    subtree.resize(size());
    build(root, -1);
  }
  void build(int u, int par) {
    preorder.push_back(u);
    parent[u] = par;
    subtree[u] = 1;
    if (par != -1) {
      depth[u] = depth[par] + 1;
      data()[u].erase(find(data()[u].begin(), data()[u].end(), par));
    }
    for (int v : at(u)) {
      build(v, u);
      subtree[u] += subtree[v];
    }
  }
};

