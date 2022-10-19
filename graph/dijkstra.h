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

template <typename graph_impl, typename weight_t>
struct dijkstra : graph_traversal {
  const Graph<graph_impl, weight_t>& graph;
  std::vector<weight_t> dist;
  std::vector<int> parent;
  const weight_t infinity;
  dijkstra(const Graph<graph_impl, weight_t>& g, weight_t inf):
    graph(g), dist(graph.size(), inf), parent(graph.size(), -1), infinity(inf) {}

  template <typename source_t>
  dijkstra& run(const std::vector<source_t>& sources) {
    std::priority_queue<Item> to_visit;
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
  const std::vector<weight_t>& get_dists() const { return dist; }
  const std::vector<int>& get_parents() const override { return parent; }
  struct Item {
    int u;
    weight_t d;
    Item(int _u, weight_t _d): u(_u), d(_d) {}
    Item(std::pair<int, weight_t> item): u(item.first), d(item.second) {}
    bool operator < (const Item& o) const { return d > o.d; }
  };
};

