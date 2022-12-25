/* Graph (???)
 * USAGE
 *  ???
 * STATUS
 *  untested
 */
#pragma once

#include <ostream>
#include <type_traits>
#include <utility>
#include <vector>

template <typename> struct graph_adj;
template <typename> struct graph_edge;

template <typename weight_t>
struct graph_adj {
  int to;
  weight_t weight;
  graph_adj(int v, weight_t w): to(v), weight(w) {}
  graph_adj(const graph_edge<weight_t>& e): to(e.to), weight(e.weight) {}
};

template <typename weight_t>
struct graph_edge {
  int from, to;
  weight_t weight;
  graph_edge(int u, int v, weight_t w): from(u), to(v), weight(w) {}
  graph_edge(int u, graph_adj<weight_t> adj): from(u), to(adj.to), weight(adj.weight) {}
};

template <>
struct graph_edge<void> {
  int from, to;
  graph_edge(int u, int v): from(u), to(v) {}
  operator std::pair<int, int>() const { return std::pair(from, to); }
};

template <>
struct graph_adj<void> {
  int to;
  graph_adj(int v): to(v) {}
  graph_adj(graph_edge<void> e): to(e.to) {}
  operator int() const { return to; }
};

static_assert(sizeof(graph_adj<void>) == sizeof(int));
static_assert(sizeof(graph_edge<void>) == sizeof(std::pair<int, int>));

// Graph Adj View

template <typename graph_impl>
struct graph_adj_view {
  const graph_impl* const g;
  int f;
  graph_adj_view(const graph_impl* g_ptr, int from): g(g_ptr), f(from) {}
  typename graph_impl::adj_iterator begin() const {
    return graph_impl::adj_iterator::begin(g, f);
  }
  typename graph_impl::adj_iterator end() const {
    return graph_impl::adj_iterator::end(g, f);
  }
};

// Graph Edge View

template <typename graph_impl, typename weight_t = void>
struct graph_edge_view {
  const graph_impl* const g;
  graph_edge_view(const graph_impl* g_ptr): g(g_ptr) {}
  typename graph_impl::edge_iterator begin() const {
    return graph_impl::edge_iterator::begin(g);
  }
  typename graph_impl::edge_iterator end() const {
    return graph_impl::edge_iterator::end(g);
  }
};

// Graph

template <typename graph_impl, typename _weight_t>  // weighted
struct Graph {
  using weight_t = _weight_t;
  static constexpr bool weighted = true;
  std::vector<int> in_degree, out_degree;
  int num_edges, num_arcs;
  bool directed;
  Graph(int n, bool dir = false):
    in_degree(n), out_degree(n), num_edges(0), num_arcs(0), directed(dir) {}
  virtual ~Graph() = default;
  virtual int size() const = 0;
  virtual void add_arc_impl(int a, int b, weight_t c) = 0;
  void add_edge(int a, int b, weight_t c) {
    num_edges++;
    add_arc(a, b, c);
    if (!directed) {
      add_arc(b, a, c);
    }
  }
  void add_arc(int a, int b, weight_t c) {
    num_arcs++;
    in_degree[b]++;
    out_degree[a]++;
    add_arc_impl(a, b, c);
  }
  graph_adj_view<graph_impl> operator [] (int u) const {
    return graph_adj_view<graph_impl>((const graph_impl*)this, u);
  }
  graph_edge_view<graph_impl> get_edges() const {
    return graph_edge_view<graph_impl>((const graph_impl*)this);
  }
};

template <typename graph_impl>  // unweighted
struct Graph<graph_impl, void> {
  using weight_t = void;
  static constexpr bool weighted = false;
  std::vector<int> in_degree, out_degree;
  int num_edges, num_arcs;
  bool directed;
  Graph(int n, bool dir = false):
    in_degree(n), out_degree(n), num_edges(0), num_arcs(0), directed(dir) {}
  virtual ~Graph() = default;
  virtual int size() const = 0;
  virtual void add_arc_impl(int a, int b) = 0;
  void add_edge(int a, int b) {
    num_edges++;
    add_arc(a, b);
    if (!directed) {
      add_arc(b, a);
    }
  }
  void add_arc(int a, int b) {
    num_arcs++;
    in_degree[b]++;
    out_degree[a]++;
    add_arc_impl(a, b);
  }
  graph_adj_view<graph_impl> operator [] (int u) const {
    return graph_adj_view<graph_impl>((const graph_impl*)this, u);
  }
  graph_edge_view<graph_impl> get_edges() const {
    return graph_edge_view<graph_impl>((const graph_impl*)this);
  }
};

// Utility

template <typename impl, typename T>
std::ostream& operator << (std::ostream& os, const Graph<impl, T>& g) {
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

template <typename>
struct is_graph : std::false_type {};

template <typename graph_impl, typename weight_t>
struct is_graph<Graph<graph_impl, weight_t>> : std::true_type {};

