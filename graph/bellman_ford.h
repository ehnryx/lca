/* Bellman Ford
 * USAGE
 *  bellman_ford result(graph, source, inf, reachable);
 *  assumes weighted graph
 * INPUT
 *  source: the vertex from which to find distances
 *  inf: infinity, should satisfy n * cost < inf and inf + inf < overflow.
 *  reachable: whether negative cycles should be reachable from s
 * OUTPUT
 *  dist[i] = distance from source to i
 *  dist[i] = inf if it is impossible to reach i from source
 *  has_negative_cycle() = true if there is a negative cycle
 *    if reachable is true, then only detects negative cycles reachable from s
 *    otherwise, detects any negative cycles in the graph
 * TIME
 *  O(VE)
 *  V = #vertices, E = #edges
 * STATUS
 *  untested
 */
#pragma once

#include "graph.h"
#include "graph_traversal.h"

template <typename graph_t>
struct bellman_ford : graph_traversal {
  using weight_t = typename graph_t::weight_t;
  std::vector<weight_t> dist;
  std::vector<int> parent;
  std::vector<bool> in_neg;
  bool has_neg;
  bellman_ford(const graph_t& graph, int source, weight_t inf,
      bool reachable_only = true):
    dist(graph.size(), inf), parent(graph.size(), -1),
    in_neg(graph.size(), false), has_neg(false) {
    dist[source] = 0;
    parent[source] = source;
    auto edge_view = graph.get_edges();
    for (int i = 1; i < graph.size(); i++) {
      for (auto [a, b, cost] : edge_view) {
        if ((!reachable_only || dist[a] != inf) && dist[a] + cost < dist[b]) {
          dist[b] = dist[a] + cost;
          parent[b] = a;
        }
      }
    }
    for (auto [a, b, cost] : edge_view) {
      if ((!reachable_only || dist[a] != inf) && dist[a] + cost < dist[b]) {
        in_neg[b] = true;
        has_neg = true;
      }
    }
  }
  const std::vector<weight_t>& get_dists() const { return dist; }
  const std::vector<int>& get_parents() const override { return parent; }
  const std::vector<bool>& get_negatives() const { return in_neg; }
  bool in_negative_cycle(int u) const { return in_neg[u]; }
  bool has_negative_cycle() const { return has_neg; }
};

