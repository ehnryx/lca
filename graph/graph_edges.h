/* Graph (???)
 * USAGE
 *  graph_edges<edge_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

#include "graph_views.h"

#include <vector>

// EDGE LIST

template <typename _weight_t>  // weighted
struct graph_edges {
  using weight_t = _weight_t;
  static constexpr bool weighted = true;
  std::vector<graph_edge<weight_t>> edges;
  std::vector<int> in_degree, out_degree;
  int num_edges, num_arcs;
  bool directed;
  graph_edges(int n, bool d = false):
    in_degree(n), out_degree(n), num_edges(0), num_arcs(0), directed(d) {}
  int size() const { return (int)in_degree.size(); }
  void add_edge(int a, int b, weight_t c) {
    num_edges++;
    add_arc(a, b, c);
    if (!directed) {
      add_arc(b, a, c);
    }
  }
  void add_arc(int a, int b, weight_t c) {
    num_arcs++;
    edges.emplace_back(a, b, c);
    in_degree[b]++;
    out_degree[a]++;
  }
  const std::vector<graph_edge<weight_t>>& get_edges() const {
    return edges;
  }
};

template <>
struct graph_edges<void> {
  using weight_t = void;
  static constexpr bool weighted = false;
  std::vector<graph_edge<weight_t>> edges;
  std::vector<int> in_degree, out_degree;
  int num_edges, num_arcs;
  bool directed;
  graph_edges(int n, bool d = false):
    in_degree(n), out_degree(n), num_edges(0), num_arcs(0), directed(d) {}
  int size() const { return (int)in_degree.size(); }
  void add_edge(int a, int b) {
    num_edges++;
    add_arc(a, b);
    if (!directed) {
      add_arc(b, a);
    }
  }
  void add_arc(int a, int b) {
    num_arcs++;
    edges.emplace_back(a, b);
    in_degree[b]++;
    out_degree[a]++;
  }
  const std::vector<graph_edge<weight_t>>& get_edges() const {
    return edges;
  }
};
