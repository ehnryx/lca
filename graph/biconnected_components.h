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

#include <vector>

template <typename graph_t>
struct biconnected_components {
  std::vector<int> degree, depth, low, childcnt;
  std::vector<bool> articulation;
  biconnected_components(const graph_t& graph):
    degree(graph.size()), depth(graph.size(), -1), low(graph.size()), childcnt(graph.size()),
    articulation(graph.size()) {
    for (int i = 0; i < graph.size(); i++) {
      if (depth[i] == -1) {
        build(graph, i, -1, 0);
        articulation[i] = childcnt[i] > 1;
      }
    }
  }

  bool is_cut_vertex(int u) const {
    return articulation[u];
  }

  bool is_cut_edge(int u, int v) const {
    return (articulation[u] || degree[u] == 1) && (articulation[v] || degree[v] == 1);
  }

private:
  void build(const graph_t& graph, int u, int parent, int cur_depth) {
    depth[u] = low[u] = cur_depth;
    if constexpr (graph.weighted) {
      for (auto [v, _] : graph[u]) {
        degree[u]++;
        degree[v]++;
        if (depth[v] == -1) {
          build(graph, v, u, cur_depth + 1);
          low[u] = std::min(low[u], low[v]);
          childcnt[u]++;
          if (low[v] >= depth[u]) {
            articulation[u] = true;
          }
        } else if (v != parent) {
          low[u] = std::min(low[u], depth[v]);
        }
      }
    } else {
      for (int v : graph[u]) {
        degree[u]++;
        degree[v]++;
        if (depth[v] == -1) {
          build(graph, v, u, cur_depth + 1);
          low[u] = std::min(low[u], low[v]);
          childcnt[u]++;
          if (low[v] >= depth[u]) {
            articulation[u] = true;
          }
        } else if (v != parent) {
          low[u] = std::min(low[u], depth[v]);
        }
      }
    }
  }
};

