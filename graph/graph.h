/* Graph (???)
 * USAGE
 *  graph_list<edge_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

#include "graph_views.h"

#include <ostream>
#include <vector>

// AJDACENCY LIST

template <typename weight_t>  // weighted
struct graph_list {
  static constexpr bool weighted = true;
  std::vector<std::vector<graph_adj<weight_t>>> adj;
  std::vector<int> in_degree, out_degree;
  graph_list(int n): adj(n), in_degree(n), out_degree(n) {}
  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b, weight_t c) {
    adj[a].emplace_back(b, c);
    in_degree[b]++;
    out_degree[a]++;
  }
  const std::vector<graph_adj<weight_t>>& operator [] (int u) const {
    return adj[u];
  }
  list_edge_list_view<weight_t> get_edges() const {
    return list_edge_list_view(this);
  }
};

template <>  // unweighted
struct graph_list<void> {
  static constexpr bool weighted = false;
  std::vector<std::vector<int>> adj;
  std::vector<int> in_degree, out_degree;
  graph_list(int n): adj(n), in_degree(n), out_degree(n) {}
  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b) {
    adj[a].emplace_back(b);
    in_degree[b]++;
    out_degree[a]++;
  }
  const std::vector<int>& operator [] (int u) const {
    return adj[u];
  }
  list_edge_list_view<void> get_edges() const {
    return list_edge_list_view(this);
  }
};

// AJDACENCY MATRIX

template <typename weight_t>  // weighted
struct graph_matrix {
  static constexpr bool weighted = true;
  std::vector<std::vector<weight_t>> adj;
  std::vector<int> in_degree, out_degree;
  weight_t inf;
  graph_matrix(int n, weight_t _inf):
    adj(n, std::vector<weight_t>(n, _inf)), in_degree(n), out_degree(n), inf(_inf) {}
  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b, weight_t c) {
    adj[a][b] = c;
    in_degree[b]++;
    out_degree[a]++;
  }
  matrix_adj_list_view<weight_t> operator [] (int u) const {
    return matrix_adj_list_view(this, u);
  }
  matrix_edge_list_view<weight_t> get_edges() const {
    return matrix_edge_list_view(this);
  }
};

template <>  // unweighted
struct graph_matrix<void> {
  static constexpr bool weighted = false;
  std::vector<std::vector<bool>> adj;
  std::vector<int> in_degree, out_degree;
  graph_matrix(int n):
    adj(n, std::vector<bool>(n)), in_degree(n), out_degree(n) {}
  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b) {
    adj[a][b] = true;
    in_degree[b]++;
    out_degree[a]++;
  }
  matrix_adj_list_view<void> operator [] (int u) const {
    return matrix_adj_list_view(this, u);
  }
  matrix_edge_list_view<void> get_edges() const {
    return matrix_edge_list_view(this);
  }
};

// MISC

template <typename G> struct is_graph_checker {};
template <template <typename> typename G, typename T>
struct is_graph_checker<G<T>> {
  static constexpr bool is_graph_list = std::is_same_v<G<T>, graph_list<T>>;
  static constexpr bool is_graph_matrix = std::is_same_v<G<T>, graph_matrix<T>>;
  static constexpr bool value = is_graph_list || is_graph_matrix;
  using weight_t = T;
};
template <typename G>
constexpr bool is_graph_v = is_graph_checker<G>::value;
template <typename G>
constexpr bool is_graph_list_v = is_graph_checker<G>::is_graph_list;
template <typename G>
constexpr bool is_graph_matrix_v = is_graph_checker<G>::is_graph_matrix;
template <typename G>
using get_graph_weight_t = typename is_graph_checker<G>::weight_t;

template <template <typename> typename graph_t, typename T,
         std::enable_if_t<is_graph_v<graph_t<T>>, bool> = true>
std::ostream& operator << (std::ostream& os, const graph_t<T>& g) {
  for (int u = 0; u < g.size(); u++) {
    os << u << " -> [";
    if constexpr (std::is_void_v<T>) {
      for (int v : g[u]) {
        os << " " << v << ", ";
      }
    } else {
      for (auto [v, c] : g[u]) {
        os << " " << v << ":" << c << ", ";
      }
    }
    os << "]\n";
  }
  return os;
}

