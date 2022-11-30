/* Minimum Spanning Forest (Kruskal)
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

#include "../data_structure/union_find.h"
#include "graph_utility.h"

template <typename weight_t, typename Compare = std::less<>>
struct min_span_forest {
  using edge_t = graph_edge<weight_t>;
  union_find<> components;
  std::vector<edge_t> edges, other;
  min_span_forest(
      int num_nodes,
      const std::vector<edge_t>& graph_edges,
      const std::vector<std::pair<int, int>>& init_edges = {}):
    components(num_nodes) {
    for (auto [a, b] : init_edges) {
      components.link(a, b);
    }
    if constexpr (std::is_void_v<weight_t>) {
      for (const edge_t& e : graph_edges) {
        if (components.link(e.from, e.to)) {
          edges.push_back(e);
        } else {
          other.push_back(e);
        }
      }
    } else {
      std::vector<edge_t> order = graph_edges;
      sort(order.begin(), order.end(), [](const edge_t& a, const edge_t& b) {
            return Compare()(a.weight, b.weight); });
      for (const edge_t& e : order) {
        if (components.link(e.from, e.to)) {
          edges.push_back(e);
        } else {
          other.push_back(e);
        }
      }
    }
  }
  weight_t sum_weight() const {
    if constexpr (!std::is_void_v<weight_t>) {
      weight_t res = weight_t();
      for (const edge_t& e : edges) {
        res += e.weight;
      }
      return res;
    }
  }
  weight_t max_weight() const {
    if constexpr (!std::is_void_v<weight_t>) {
      weight_t res = weight_t();
      for (const edge_t& e : edges) {
        res = max(res, e.weight);
      }
      return res;
    }
  }
  const union_find<>& operator*() const { return components; }
};

