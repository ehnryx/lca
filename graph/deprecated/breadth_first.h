/* BFS
 * USAGE
 *  breadth_first bfs(graph);
 *  bfs.run(sources, zero_one?);
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

template <typename graph_impl, typename weight_t>
struct breadth_first : graph_traversal {
  const Graph<graph_impl, weight_t>& graph;
  std::vector<int> dist, parent;
  breadth_first(const Graph<graph_impl, weight_t>& g):
    graph(g), dist(graph.size(), -1), parent(graph.size(), -1) {}
  breadth_first& run(int source, bool zero_one = false) {
    return run(std::vector<int>(1, source), zero_one);
  }
  // run bfs while avoiding previously visited nodes
  breadth_first& run(const std::vector<int>& sources, bool zero_one = false) {
    std::deque<int> to_visit;
    for (int source : sources) {
      if (dist[source] == -1) {
        dist[source] = 0;
        parent[source] = source;
      }
    }
    while (!to_visit.empty()) {
      int u = to_visit.front();
      to_visit.pop_front();
      if constexpr (graph.weighted) {
        for (const auto& [v, w] : graph[u]) {
          if (dist[v] != -1) continue;
          parent[v] = u;
          if (zero_one && w == weight_t()) {
            dist[v] = dist[u];
            to_visit.push_front(v);
          } else {
            dist[v] = dist[u] + 1;
            to_visit.push_back(v);
          }
        }
      } else {
        for (int v : graph[u]) {
          if (dist[v] != -1) continue;
          parent[v] = u;
          dist[v] = dist[u] + 1;
          to_visit.push_back(v);
        }
      }
    }
    return *this;
  }
  const std::vector<int>& get_dists() const { return dist; }
  const std::vector<int>& get_parents() const override final { return parent; }
};

