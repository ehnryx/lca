/* BFS
 * USAGE
 *  depth_first bfs(graph);
 *  dfs.run(source);
 * OUTPUT
 *  distance from source `get_dists()`, bfs tree `get_parents()`
 *  dist = -1 means unreachable
 * TIME
 *  O(V + E) maybe
 *  V = #vertices, E = #edges
 * STATUS
 *  unused
 */
#pragma once

#include "graph.h"
#include "graph_traversal.h"
#include <stack>

template <typename graph_impl, typename weight_t>
struct depth_first : graph_traversal {
  const Graph<graph_impl, weight_t>& graph;
  std::vector<int> parent;
  depth_first(const Graph<graph_impl, weight_t>& g):
    graph(g), parent(graph.size(), -1) {}
  depth_first& run(int source) {
    if (parent[source] != -1) return *this;
    std::stack<int> to_visit;
    to_visit.push(source);
    parent[source] = source;
    while (!to_visit.empty()) {
      int u = to_visit.top();
      to_visit.pop();
      for (const auto& e : graph[u]) {
        if (parent[e.to] != -1) continue;
        parent[e.to] = u;
        to_visit.push(e.to);
      }
    }
    return *this;
  }
  const std::vector<int>& get_parents() const override final { return parent; }
};

