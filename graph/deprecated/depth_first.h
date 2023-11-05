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

template <typename weight_t>
struct depth_first : graph_traversal {
  const graph_t<weight_t>& graph;
  depth_first(const graph_t<weight_t>& g):
    graph_traversal(g.size()), graph(g) {}
  template <typename NodeF, typename EdgeF>
  depth_first& run(int source, NodeF&& node_func, EdgeF&& edge_func) {
    static constexpr bool has_node_func = not std::is_same_v<NodeF, bool>;
    static constexpr bool has_edge_func = not std::is_same_v<EdgeF, bool>;
    if (parent[source] != -1) return *this;
    std::stack<int> to_visit;
    to_visit.push(source);
    parent[source] = source;
    while (!to_visit.empty()) {
      int u = to_visit.top();
      to_visit.pop();
      if constexpr (has_node_func) node_func(u);
      for (const auto& e : graph[u]) {
        if (parent[e.to] != -1) continue;
        parent[e.to] = u;
        to_visit.push(e.to);
        if constexpr (has_edge_func) edge_func(graph_edge(u, e));
      }
    }
    return *this;
  }
  template <typename NodeF>
  depth_first& run_node_func(int source, NodeF&& node_func) {
    return run(source, node_func, false);
  }
  template <typename EdgeF>
  depth_first& run_edge_func(int source, EdgeF&& edge_func) {
    return run(source, false, edge_func);
  }
  depth_first& run(int source) { return run(source, false, false); }
};

// TODO: this is not a dfs

