/* Graph (???)
 * USAGE
 *  ???
 * STATUS
 *  untested
 */
#pragma once

#include "../utility/traits.h"
#include "graph_utility.h"

#include <ostream>
#include <vector>

// clang-format off
MAKE_TRAITS(graph_traits,
  (IN_DEGREE),
);
// clang-format on

template <graph_traits traits, typename = void>
struct graph_data_base {
  static_assert(traits == graph_traits::NONE);
  graph_data_base(size_t) {}
};

template <graph_traits traits>
struct graph_data_base<traits, std::enable_if_t<traits.has_any(graph_traits::IN_DEGREE)>> {
  std::vector<int> in_degree;
  graph_data_base(size_t n) : in_degree(n) {}
};

template <typename Weight_t, graph_traits traits>
struct graph_base_t : graph_data_base<traits> {
  using weight_t = Weight_t;
  static constexpr bool weighted = not std::is_void_v<weight_t>;
  std::vector<std::vector<graph_adj<weight_t>>> adj;
  graph_base_t(int n) : graph_data_base<traits>(n), adj(n) {}
  graph_base_t(const std::vector<std::vector<graph_adj<weight_t>>>& g)
      : graph_data_base<traits>(g.size()), adj(g) {}
  void reset(int n) {
    fill(adj.begin(), adj.end(), std::vector<graph_adj<weight_t>>());
    adj.resize(n);
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
  std::vector<graph_edge<weight_t>> get_edges() const {
    std::vector<graph_edge<weight_t>> edges;
    for (int u = 0; u < size(); u++) {
      for (auto const& v : adj[u]) {
        edges.emplace_back(u, v);
      }
    }
    return edges;
  }
};

template <typename Weight_t = void, graph_traits traits = graph_traits::NONE>
struct graph_t : graph_base_t<Weight_t, traits> {
  graph_t(int n) : graph_base_t<Weight_t, traits>(n) {}
  graph_t(const std::vector<std::vector<graph_adj<Weight_t>>>& g)
      : graph_base_t<Weight_t, traits>(g) {}
  void add_edge(int a, int b, Weight_t c) {
    add_arc(a, b, c);
    if (a != b) add_arc(b, a, c);
  }
  void add_arc(int a, int b, Weight_t c) {
    this->adj[a].emplace_back(b, c);
    if constexpr (traits.has_any(graph_traits::IN_DEGREE)) {
      this->in_degree[b]++;
    }
  }
};

template <graph_traits traits>
struct graph_t<void, traits> : graph_base_t<void, traits> {
  graph_t(int n) : graph_base_t<void, traits>(n) {}
  graph_t(const std::vector<std::vector<graph_adj<void>>>& g) : graph_base_t<void, traits>(g) {}
  void add_edge(int a, int b) {
    add_arc(a, b);
    if (a != b) add_arc(b, a);
  }
  void add_arc(int a, int b) {
    this->adj[a].emplace_back(b);
    if constexpr (traits.has_any(graph_traits::IN_DEGREE)) {
      this->in_degree[b]++;
    }
  }
};
