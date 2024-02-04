/* Topological sort
 * USAGE
 *  vector<int> order = topological_sort(graph);
 *  gives a vector of vertices in toposort order (breaks if not dag)
 * TIME
 *  O(V + E) maybe
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: cf/104772l
 */
#pragma once

#include "graph.h"

#include <stack>

template <typename weight_t, graph_traits _gt, typename... CallbackFs>
  requires(_gt.has_any(graph_traits::IN_DEGREE))
std::vector<int> topological_sort(
    graph_t<weight_t, _gt> const& graph, graph_callbacks_t<CallbackFs...>&& callbacks) {
  static constexpr bool has_on_node = GRAPH_CALLBACK_EXISTS(callbacks.on_node);
  static constexpr bool has_on_edge = GRAPH_CALLBACK_EXISTS(callbacks.on_edge);
  std::vector<int> order;
  order.reserve(graph.size());
  std::vector<int> in_degree = graph.in_degree;
  std::stack<int> to_visit;
  for (int s = 0; s < graph.size(); s++) {
    if (in_degree[s] == 0) {
      to_visit.push(s);
    }
  }
  while (not to_visit.empty()) {
    int u = to_visit.top();
    to_visit.pop();
    order.push_back(u);
    if constexpr (has_on_node) callbacks.on_node(u);
    for (auto const& e : graph[u]) {
      if constexpr (has_on_edge) callbacks.on_edge(graph_edge(u, e));
      if (--in_degree[e.to] == 0) {
        to_visit.push(e.to);
      }
    }
  }
  return order;
}

template <typename weight_t, graph_traits _gt>
  requires(_gt.has_any(graph_traits::IN_DEGREE))
std::vector<int> topological_sort(graph_t<weight_t, _gt> const& graph) {
  return topological_sort(graph, graph_callbacks_t{});
}
