/* BFS
 * USAGE
 *  dijkstra bfs(graph, source, inf);
 *  assumes weighted graph
 * OUTPUT
 *  distance from source `get_dists()`, bfs tree `get_parents()`
 *  dist = inf means unreachable
 * TIME
 *  O(V + ElogE) maybe
 *  V = #vertices, E = #edges
 * STATUS
 *  untested: cf/20c
 */
#pragma once

#include "graph.h"
#include "graph_traversal.h"
#include <queue>

template <typename weight_t>
struct dijkstra : graph_traversal {
  const graph_t<weight_t>& graph;
  std::vector<weight_t> dist;
  const weight_t infinity;
  dijkstra(const graph_t<weight_t>& g, weight_t inf):
    graph_traversal(g.size()), graph(g), dist(g.size(), inf), infinity(inf) {}
  const std::vector<weight_t>& get_dists() const { return dist; }
  dijkstra& run(int source) {
    return run(std::vector({ graph_adj<weight_t>(source, 0) }));
  }
  dijkstra& run(const std::vector<int>& sources) {
    std::vector<graph_adj<weight_t>>& hacked;
    hacked.reserve(sources.size());
    for (int s : sources) {
      hacked.emplace_back(s, 0);
    }
    return run(hacked);
  }
  dijkstra& run(const std::vector<graph_adj<weight_t>>& sources) {
    std::priority_queue<
      graph_adj<weight_t>,
      std::vector<graph_adj<weight_t>>,
      std::greater<graph_adj<weight_t>>> to_visit;
    for (auto [source, dist_to_source] : sources) {
      if (dist[source] == infinity || dist_to_source < dist[source]) {
        dist[source] = dist_to_source;
        parent[source] = source;
        to_visit.emplace(source, dist_to_source);
      }
    }
    while (!to_visit.empty()) {
      auto [u, d] = to_visit.top();
      to_visit.pop();
      if (d != dist[u]) continue;
      for (const auto& [v, c] : graph[u]) {
        if (dist[v] != infinity && dist[v] <= d + c) continue;
        dist[v] = d + c;
        parent[v] = u;
        to_visit.emplace(v, dist[v]);
      }
    }
    return *this;
  }
};

