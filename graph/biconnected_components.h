/* Biconnected Components
 * USAGE
 *  biconnected bcc(graph);
 * MEMBERS
 *  is_cut_vertex
 *  is_cut_edge
 * TIME
 *  O(V + E)
 *  V = #vertices, E = #edges
 * STATUS
 *  untested
 */
#pragma once

#include "graph.h"

#include <vector>

template <typename weight_t>
struct biconnected_components {
  const graph_t<weight_t>& graph;
  std::vector<int> degree, depth, low, childcnt;
  std::vector<bool> articulation;
  biconnected_components(const graph_t<weight_t>& g)
      : graph(g), degree(graph.size()), depth(graph.size(), -1), low(graph.size()),
        childcnt(graph.size()), articulation(graph.size()) {
    for (int i = 0; i < graph.size(); i++) {
      if (depth[i] == -1) {
        build(i, -1, 0);
        articulation[i] = childcnt[i] > 1;
      }
    }
  }

  bool is_cut_vertex(int u) const { return articulation[u]; }

  bool is_cut_edge(int u, int v) const {
    return (articulation[u] || degree[u] == 1) && (articulation[v] || degree[v] == 1);
  }

 private:
  void build(int u, int parent, int cur_depth) {
    depth[u] = low[u] = cur_depth;
    for (const auto& e : graph[u]) {
      degree[u]++;
      degree[e.to]++;
      if (depth[e.to] == -1) {
        build(e.to, u, cur_depth + 1);
        low[u] = std::min(low[u], low[e.to]);
        childcnt[u]++;
        if (low[e.to] >= depth[u]) {
          articulation[u] = true;
        }
      } else if (e.to != parent) {
        low[u] = std::min(low[u], depth[e.to]);
      }
    }
  }
};
