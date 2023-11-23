/* Topological sort
 * USAGE
 *  topological_sort t(graph);
 *  t.order; // vector of vertices in toposort order (breaks if not dag)
 * TIME
 *  O(V + E) maybe
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: cf/104772l
 */
#pragma once

#include "graph.h"

#include <stack>

template <typename weight_t>
struct topological_sort {
  std::vector<int> order;
  topological_sort(graph_t<weight_t> const& graph) {
    order.reserve(graph.size());
    std::vector<int> in_degree(graph.size());
    for (int u = 0; u < graph.size(); u++) {
      for (auto const& e : graph[u]) {
        in_degree[e.to] += 1;
      }
    }
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
      for (auto const& e : graph[u]) {
        if (--in_degree[e.to] == 0) {
          to_visit.push(e.to);
        }
      }
    }
  }
};
