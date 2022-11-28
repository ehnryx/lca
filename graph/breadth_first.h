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
  const vector_graph<weight_t>& graph;
  std::vector<int> dist;
  breadth_first(const vector_graph<weight_t>& g):
    graph_traversal(g.size()), graph(g), dist(g.size(), -1) {}
  const std::vector<int>& get_dists() const { return dist; }
  breadth_first& run(int source) { return run(std::vector({ source })); }
  // run bfs while avoiding previously visited nodes
  breadth_first& run(const std::vector<int>& sources) {
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
      for (const auto& e : graph[u]) {
        if (dist[e.to] != -1) continue;
        parent[e.to] = u;
        dist[e.to] = dist[u] + 1;
        to_visit.push_back(e.to);
      }
    }
    return *this;
  }
};

