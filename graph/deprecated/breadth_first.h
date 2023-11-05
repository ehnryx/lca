/* BFS
 * USAGE
 *  breadth_first bfs(graph);
 *  bfs.run(sources);
 * OUTPUT
 *  distance from source `get_dists()`, bfs tree `get_parents()`
 *  dist = -1 means unreachable
 * TIME
 *  O(V + E) maybe
 *  V = #vertices, E = #edges
 * STATUS
 *  untested
 */
#pragma once

#include "graph.h"
#include "graph_traversal.h"
#include <queue>

template <typename weight_t>
struct breadth_first : graph_traversal {
  const graph_t<weight_t>& graph;
  std::vector<int> dist;
  breadth_first(const graph_t<weight_t>& g):
    graph_traversal(g.size()), graph(g), dist(g.size(), -1) {}
  const std::vector<int>& get_dists() const { return dist; }
  // run bfs while avoiding previously visited nodes
  template <typename NodeF, typename EdgeF>
  breadth_first& run(const std::vector<int>& sources, NodeF&& node_func, EdgeF&& edge_func) {
    static constexpr bool has_node_func = not std::is_same_v<NodeF, bool>;
    static constexpr bool has_edge_func = not std::is_same_v<EdgeF, bool>;
    std::deque<int> to_visit;
    for (int source : sources) {
      if (dist[source] == -1) {
        dist[source] = 0;
        parent[source] = source;
        to_visit.push_back(source);
      }
    }
    while (!to_visit.empty()) {
      int u = to_visit.front();
      to_visit.pop_front();
      if constexpr (has_node_func) node_func(u);
      for (const auto& e : graph[u]) {
        if (dist[e.to] != -1) continue;
        if constexpr (has_edge_func) edge_func(graph_edge(u, e));
        parent[e.to] = u;
        dist[e.to] = dist[u] + 1;
        to_visit.push_back(e.to);
      }
    }
    return *this;
  }
  template <typename NodeF>
  breadth_first& run_node_func(const std::vector<int>& sources, NodeF&& node_func) {
    return run_node_func(sources, node_func, false);
  }
  template <typename EdgeF>
  breadth_first& run_edge_func(const std::vector<int>& sources, EdgeF&& edge_func) {
    return run(sources, false, edge_func);
  }
  template <typename NodeF>
  breadth_first& run_node_func(int source, NodeF&& node_func) {
    return run_node_func(std::vector({ source }), node_func);
  }
  template <typename EdgeF>
  breadth_first& run_edge_func(int source, EdgeF&& edge_func) {
    return run_edge_func(std::vector({ source }), edge_func);
  }
  breadth_first& run(int source) { return run(std::vector({ source }), false, false); }
};

