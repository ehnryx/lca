/* Graph (???)
 * USAGE
 *  ???
 * STATUS
 *  untested
 */
#pragma once

#include "graph_utility.h"
#include <ostream>
#include <vector>

template <typename Weight_t>
struct vector_graph_base {
  using weight_t = Weight_t;
  static constexpr bool weighted = not std::is_void_v<weight_t>;
  std::vector<std::vector<graph_adj<weight_t>>> adj;
  std::vector<int> in_degree, out_degree;
  vector_graph_base(int n): adj(n), in_degree(n), out_degree(n) {}
  vector_graph_base(const std::vector<std::vector<graph_adj<weight_t>>>& g):
    adj(g), in_degree(g.size()), out_degree(g.size()) {
    for (int u = 0; u < size(); u++) {
      out_degree[u] = (int)g[u].size();
      for (const auto& e : g[u]) {
        in_degree[e.to]++;
      }
    }
  }
  int size() const { return (int)adj.size(); }
  const std::vector<graph_adj<weight_t>>& operator[](int u) const { return adj[u]; }
  friend std::ostream& operator<<(std::ostream& os, const vector_graph_base& g) {
    for (int u = 0; u < g.size(); u++) {
      os << u << " -> [";
      if constexpr (g.weighted) {
        for (auto [v, c] : g[u]) {
          os << " " << v << ":" << c << ", ";
        }
      } else {
        for (int v : g[u]) {
          os << " " << v << ", ";
        }
      }
      os << "]\n";
    }
    return os;
  }
};

template <typename Weight_t>
struct vector_graph : vector_graph_base<Weight_t> {
  vector_graph(int n): vector_graph_base<Weight_t>(n) {}
  vector_graph(const std::vector<std::vector<graph_adj<Weight_t>>>& g):
    vector_graph_base<Weight_t>(g) {}
  void add_edge(int a, int b, Weight_t c) {
    add_arc(a, b, c);
    if (a != b) add_arc(b, a, c);
  }
  void add_arc(int a, int b, Weight_t c) {
    this->in_degree[b]++;
    this->out_degree[a]++;
    this->adj[a].emplace_back(b, c);
  }
};

template <>
struct vector_graph<void> : vector_graph_base<void> {
  vector_graph(int n): vector_graph_base<void>(n) {}
  vector_graph(const std::vector<std::vector<graph_adj<void>>>& g):
    vector_graph_base<void>(g) {}
  void add_edge(int a, int b) {
    add_arc(a, b);
    add_arc(b, a);
  }
  void add_arc(int a, int b) {
    this->in_degree[b]++;
    this->out_degree[a]++;
    this->adj[a].emplace_back(b);
  }
};

