/* DFS
 * USAGE
 *  depth_first bfs(graph, source);
 * OUTPUT
 *  distance from source `get_dists()`, dfs tree `get_parents()`
 *  parent = -1 means unreachable
 * TIME
 *  O(V + E) maybe
 *  V = #vertices, E = #edges
 * STATUS
 *  untested
 */
#pragma once

#include "../data_structure/circular_buffer.h"
#include "graph_traversal.h"

template <typename graph_t>
struct depth_first : graph_traversal {
  vector<int> parent;
  depth_first(const graph_t& graph, int source): parent(graph.size(), -1) {
    circular_buffer<int> to_visit(graph.size());
    to_visit.push_back(source);
    parent[source] = source;
    while (!to_visit.empty()) {
      int u = to_visit.back();
      to_visit.pop_back();
      if constexpr (graph_t::weighted) {
        for (const auto& [v, _] : graph[u]) {
          if (parent[v] != -1) continue;
          parent[v] = u;
          to_visit.push_back(v);
        }
      } else {
        for (int v : graph[u]) {
          if (parent[v] != -1) continue;
          parent[v] = u;
          to_visit.push_back(v);
        }
      }
    }
  }
  const vector<int>& get_parents() const override { return parent; }
};

