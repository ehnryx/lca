/* BFS
 * USAGE
 *  breadth_first bfs(graph, source);
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

#include "../data_structure/circular_buffer.h"
#include "graph_traversal.h"

template <typename graph_t>
struct breadth_first : graph_traversal {
  vector<int> dist, parent;
  breadth_first(const graph_t& graph, int source, bool zero_one = false):
    dist(graph.size(), -1), parent(graph.size(), -1) {
    circular_buffer<int> to_visit(graph.size());
    to_visit.push_back(source);
    dist[source] = 0;
    parent[source] = source;
    while (!to_visit.empty()) {
      int u = to_visit.front();
      to_visit.pop_front();
      if constexpr (graph_t::weighted) {
        for (const auto& [v, w] : graph[u]) {
          if (dist[v] != -1) continue;
          if (zero_one) {
            parent[v] = u;
            if (w == typename graph_t::weight_t()) {
              dist[v] = dist[u];
              to_visit.push_front(v);
            } else {
              dist[v] = dist[u] + 1;
              to_visit.push_back(v);
            }
          } else {
            dist[v] = dist[u] + 1;
            parent[v] = u;
            to_visit.push_back(v);
          }
        }
      } else {
        for (int v : graph[u]) {
          if (dist[v] != -1) continue;
          dist[v] = dist[u] + 1;
          parent[v] = u;
          to_visit.push_back(v);
        }
      }
    }
  }
  const vector<int>& get_dists() const { return dist; }
  const vector<int>& get_parents() const override { return parent; }
};

