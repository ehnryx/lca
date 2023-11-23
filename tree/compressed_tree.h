/* Compressed Tree (using lca_binary_jumping)
 * USAGE
 *  compressed_tree tree(lca_binary_jumping, vector of vertices);
 *  stored as an adjacency list of pair(neighbour, #edges to neighbour)
 *  remapped 0 is always the root of the compressed tree
 * TIME
 *  O(VlogN) construction
 *  N = #vertices in lca_binary_jumping
 *  V = #vertices of interest
 * STATUS
 *  untested
 */
#pragma once

#include <map>
#include <stdexcept>
#include <vector>

struct compressed_tree {
  std::vector<int> vertices;
  std::vector<bool> given;
  std::map<int, int> remap;
  std::vector<std::vector<std::pair<int, int>>> adj;
  std::vector<std::pair<int, int>> parent;

  template <typename T>
  compressed_tree(const T& tree, const std::vector<int>& verts) {
    if (verts.empty()) {
      throw std::invalid_argument("vertices of compressed tree must not be empty");
    }
    std::vector<std::pair<int, int>> order;
    for (int v : verts) {
      order.emplace_back(tree.in[v], v);
    }
    sort(begin(order), end(order));
    for (size_t i = 1; i < verts.size(); i++) {
      int lca = tree.lca(order[i].second, order[i - 1].second);
      order.emplace_back(tree.in[lca], lca);
    }
    sort(begin(order), end(order));

    int n = (int)(unique(begin(order), end(order)) - begin(order));
    vertices.resize(n);
    adj.resize(n);
    parent.resize(n);
    parent[0] = std::pair(-1, 0);
    for (int i = 0; i < n; i++) {
      vertices[i] = order[i].second;
      remap[vertices[i]] = i;
      if (i > 0) {
        int lca = tree.lca(vertices[i], vertices[i - 1]);
        int dist = tree.distance(vertices[i], lca);
        parent[i] = std::pair(remap[lca], dist);
        adj[parent[i].first].emplace_back(i, dist);
      }
    }
    given.resize(n);
    for (int v : verts) {
      given[remap[v]] = true;
    }
  }

  const std::vector<std::pair<int, int>>& operator[](int i) const { return adj[i]; }
  int size() const { return (int)adj.size(); }
};
