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
  graph_t<weight_t> const& graph;
  std::vector<int> order;
  std::vector<int> rank;
  toposort(graph_t<weight_t> const& g): graph(g), order(g.size()), rank(g.size(), -1) {
    std::vector<int> degree = g.in_degree;
    std::vector<int> to_visit;
    to_visit.reserve(g.size());
    for (int s = 0; s < g.size(); s++) {
      if (degree[s] == 0) {
        to_visit.push_back(s);
      }
    }
    int uid = 0;
    while (not to_visit.empty()) {
      int u = to_visit.back();
      to_visit.pop_back();
      rank[u] = uid;
      order[uid++] = u;
      for (auto const& e : g[u]) {
        if (--degree[e.to] == 0) {
          to_visit.push_back(e.to);
        }
      }
    }
    order.resize(uid);
  }
};

