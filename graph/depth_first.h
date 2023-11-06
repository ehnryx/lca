/* DFS (recursive)
 * USAGE
 *  depth_first dfs(graph);
 *  dfs.run(sources, callbacks);
 * OUTPUT
 *  visited array
 *  dfs tree `get_parents()`
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
struct depth_first : graph_traversal {
  graph_t<weight_t> const& graph;
  std::vector<bool> visited;
  depth_first(graph_t<weight_t> const& g):
    graph_traversal(g.size()), graph(g), visited(g.size(), false) {}
  // run bfs while avoiding previously visited nodes
  template <typename... CallbackFs>
  depth_first& run(std::vector<int> const& sources, graph_callbacks_t<CallbackFs...>&& callbacks) {
    auto run_dfs = [this, callbacks = std::move(callbacks)](auto&& self, int u) -> void {
      static constexpr bool has_on_node = not IS_GRAPH_CALLBACK_PLACEHOLDER(callbacks.on_node);
      static constexpr bool has_on_edge = not IS_GRAPH_CALLBACK_PLACEHOLDER(callbacks.on_edge);
      static constexpr bool has_should_visit = not IS_GRAPH_CALLBACK_PLACEHOLDER(callbacks.should_visit);
      visited[u] = true;
      if constexpr (has_on_node) callbacks.on_node(u);
      for (auto const& e : graph[u]) {
        if (bool should_visit = [&] {
          if constexpr (has_should_visit) return callbacks.should_visit(graph_edge(u, e));
          else return not visited[e.to]; }(); should_visit) {
          if constexpr (has_on_edge) callbacks.on_edge(graph_edge(u, e));
          parent[e.to] = u;
          self(self, e.to);
        }
      }
    };
    for (int source : sources) {
      if (not visited[source]) {
        run_dfs(run_dfs, source);
      }
    }
    return *this;
  }
  template <typename... CallbackFs>
  depth_first& run(int source, graph_callbacks_t<CallbackFs...>&& callbacks) {
    return run(std::vector({ source }), std::move(callbacks));
  }
  depth_first& run(std::vector<int> const& sources) { return run(sources, graph_callbacks_t{}); }
  depth_first& run(int source) { return run(std::vector({ source }), graph_callbacks_t{}); }
};

