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
 *  tested: boj/11657; kattis/shortestpath3
 */
#pragma once

#include "graph.h"
#include "graph_traversal.h"

template <typename weight_t>
struct bellman_ford : graph_traversal {
  static weight_t constexpr dist_floor = std::numeric_limits<weight_t>::min() / 2;
  std::vector<weight_t> dist;
  std::vector<int> parent;
  std::vector<bool> in_neg;
  bool has_neg;
  bellman_ford(graph_t<weight_t> const& graph, int source, weight_t inf,
      bool reachable_only = true):
    bellman_ford(graph.size(), graph.get_edges(), source, inf, reachable_only) {}
  bellman_ford(
      int num_nodes, std::vector<graph_edge<weight_t>> const& edges,
      int source, weight_t inf, bool reachable_only = true):
    graph_traversal(num_nodes), dist(num_nodes, inf), parent(num_nodes, -1),
    in_neg(num_nodes), has_neg(false) {
    dist[source] = 0;
    parent[source] = source;
    for (int i = 1; i < num_nodes; i++) {
      for (auto const& e : edges) {
        if ((!reachable_only || dist[e.from] != inf) &&
            (dist[e.to] == inf || dist[e.from] + e.weight < dist[e.to])) {
          dist[e.to] = std::max(dist[e.from] + e.weight, dist_floor);
          parent[e.to] = e.from;
        }
      }
    }
    for (auto const& e : edges) {
      if ((!reachable_only || dist[e.from] != inf) &&
          (dist[e.to] == dist_floor || dist[e.from] + e.weight < dist[e.to])) {
        in_neg[e.to] = true;
        has_neg = true;
      }
    }
  }
  const std::vector<weight_t>& get_dists() const { return dist; }
  const std::vector<bool>& get_negatives() const { return in_neg; }
  bool in_negative_cycle(int u) const { return in_neg[u]; }
  bool has_negative_cycle() const { return has_neg; }
};

