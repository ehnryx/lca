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
 *  tested: cf/104713d; kattis/shortestpath3
 */
#pragma once

#include "graph.h"
#include "graph_traversal.h"
#include "../utility/type_conversion_checker.h"
#include <queue>

template <typename weight_t>
struct breadth_first : graph_traversal {
  graph_t<weight_t> const& graph;
  std::vector<int> dist;
  breadth_first(graph_t<weight_t> const& g):
    graph_traversal(g.size()), graph(g), dist(g.size(), -1) {}
  std::vector<int> const& get_dists() const { return dist; }
  // run bfs while avoiding previously visited nodes
  template <typename... CallbackFs>
  breadth_first& run(std::vector<int> const& sources, callbacks_t<CallbackFs...>&& callbacks) {
    static constexpr bool has_node_func = not IS_CALLBACK_PLACEHOLDER(callbacks.node_func);
    static constexpr bool has_edge_func = not IS_CALLBACK_PLACEHOLDER(callbacks.edge_func);
    static constexpr bool has_should_visit = not IS_CALLBACK_PLACEHOLDER(callbacks.should_visit);
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
      if constexpr (has_node_func) callbacks.node_func(u);
      for (const auto& e : graph[u]) {
        if (bool should_visit = [&] {
            if constexpr (has_should_visit) return callbacks.should_visit(graph_edge(u, e));
            else return dist[e.to] == -1; }(); should_visit) {
          if constexpr (has_edge_func) callbacks.edge_func(graph_edge(u, e));
          parent[e.to] = u;
          dist[e.to] = dist[u] + 1;
          to_visit.push_back(e.to);
        }
      }
    }
    return *this;
  }
  template <typename... CallbackFs>
  breadth_first& run(int source, callbacks_t<CallbackFs...>&& callbacks) {
    return run(std::vector({ source }), std::move(callbacks));
  }
  breadth_first& run(std::vector<int> const& sources) { return run(sources, callbacks_t{}); }
  breadth_first& run(int source) { return run(std::vector({ source }), callbacks_t{}); }
};

