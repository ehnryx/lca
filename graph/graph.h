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
struct graph_base_t {
  using weight_t = Weight_t;
  static constexpr bool weighted = not std::is_void_v<weight_t>;
  std::vector<std::vector<graph_adj<weight_t>>> adj;
  std::vector<int> in_degree, out_degree;
  graph_base_t(int n): adj(n), in_degree(n), out_degree(n) {}
  graph_base_t(const std::vector<std::vector<graph_adj<weight_t>>>& g):
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
  friend std::ostream& operator<<(std::ostream& os, const graph_base_t& g) {
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

template <typename Weight_t = void>
struct graph_t : graph_base_t<Weight_t> {
  graph_t(int n): graph_base_t<Weight_t>(n) {}
  graph_t(const std::vector<std::vector<graph_adj<Weight_t>>>& g):
    graph_base_t<Weight_t>(g) {}
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
struct graph_t<void> : graph_base_t<void> {
  graph_t(int n): graph_base_t<void>(n) {}
  graph_t(const std::vector<std::vector<graph_adj<void>>>& g):
    graph_base_t<void>(g) {}
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

