/* Topological sort
 * USAGE
 *  toposort t(graph);
 *  t.order; // vector of vertices in toposort order (breaks if not dag)
 *  t.rank; // (0-indexed) rank of vertex in toposort order (-1 if not dag)
 * TIME
 *  O(V + E) maybe
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: cf/104772l
 */
#pragma once

#include "graph.h"

template <typename weight_t>
struct toposort {
  std::vector<int> order;
  std::vector<int> rank;
  toposort(graph_t<weight_t> const& graph):
    order(graph.size()), rank(graph.size(), -1) {
    std::vector<int> in_degree(graph.size());
    for (int u = 0; u < graph.size(); u++) {
      for (auto const& e : graph[u]) {
        in_degree[e.to] += 1;
      }
    }
    std::vector<int> to_visit;
    to_visit.reserve(graph.size());
    for (int s = 0; s < graph.size(); s++) {
      if (in_degree[s] == 0) {
        to_visit.push_back(s);
      }
    }
    int uid = 0;
    while (not to_visit.empty()) {
      int u = to_visit.back();
      to_visit.pop_back();
      rank[u] = uid;
      order[uid++] = u;
      for (auto const& e : graph[u]) {
        if (--in_degree[e.to] == 0) {
          to_visit.push_back(e.to);
        }
      }
    }
    order.resize(uid);
  }
};

