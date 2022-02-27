/* Graph (???)
 * USAGE
 *  graph_list<edge_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

// AJDACENCY LIST

template <typename weight_t>  // weighted
struct graph_list {
  static constexpr bool weighted = true;
  vector<vector<pair<int, weight_t>>> adj;
  graph_list(int n): adj(n) {}
  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b, weight_t c) {
    adj[a].emplace_back(b, c);
  }
  const vector<pair<int, weight_t>>& operator [] (int u) const {
    return adj[u];
  }
};

template <>  // unweighted
struct graph_list<void> {
  static constexpr bool weighted = false;
  vector<vector<int>> adj;
  graph_list(int n): adj(n) {}
  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b) {
    adj[a].emplace_back(b);
  }
  const vector<int>& operator [] (int u) const {
    return adj[u];
  }
};

// AJDACENCY MATRIX

template <typename> struct graph_matrix;

template <typename weight_t>
class adj_list_view {
  static constexpr bool _weighted = graph_matrix<weight_t>::weighted;
  const graph_matrix<weight_t>* _graph;
  int _from;
  struct iterator {
    using out_t = conditional_t<_weighted, pair<int, weight_t>, int>;
    const graph_matrix<weight_t>* graph;
    int from, to;
    iterator(): graph(nullptr), from(-1), to(-1) {}
    iterator(const graph_matrix<weight_t>* g, int f, int t): graph(g), from(f), to(t) {}
    out_t operator * () const {
      if constexpr (_weighted) {
        return pair(to, graph->adj[from][to]);
      } else {
        return to;
      }
    }
    iterator& operator ++ () {
      while (++to < (int)graph->adj[from].size()) {
        if constexpr (_weighted) {
          if (graph->adj[from][to] != graph->inf) {
            break;
          }
        } else {
          if (graph->adj[from][to]) {
            break;
          }
        }
      }
      return *this;
    }
    bool operator == (const iterator& o) const { return to == o.to; }
    bool operator != (const iterator& o) const { return to != o.to; }
  };
public:
  adj_list_view(const graph_matrix<weight_t>* g, int u):
    _graph(g), _from(u) {}
  iterator begin() const {
    return ++iterator(_graph, _from, -1);
  }
  iterator end() const {
    return iterator(_graph, _from, (int)_graph->adj[_from].size());
  }
};

template <typename weight_t>  // weighted
struct graph_matrix {
  static constexpr bool weighted = true;
  vector<vector<weight_t>> adj;
  weight_t inf;
  graph_matrix(int n, weight_t _inf):
    adj(n, vector<weight_t>(n, _inf)), inf(_inf) {}
  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b, weight_t c) {
    adj[a][b] = c;
  }
  adj_list_view<weight_t> operator [] (int u) const {
    return adj_list_view(this, u);
  }
};

template <>  // unweighted
struct graph_matrix<void> {
  static constexpr bool weighted = false;
  vector<vector<bool>> adj;
  graph_matrix(int n): adj(n, vector<bool>(n)) {}
  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b) {
    adj[a][b] = true;
  }
  adj_list_view<void> operator [] (int u) const {
    return adj_list_view(this, u);
  }
};

// MISC

template <typename G> struct is_graph_checker {};
template <template <typename> typename G, typename T>
struct is_graph_checker<G<T>> {
  static constexpr bool is_graph_list = is_same_v<G<T>, graph_list<T>>;
  static constexpr bool is_graph_matrix = is_same_v<G<T>, graph_matrix<T>>;
  static constexpr bool value = is_graph_list || is_graph_matrix;
};
template <typename G>
constexpr bool is_graph_v = is_graph_checker<G>::value;
template <typename G>
constexpr bool is_graph_list_v = is_graph_checker<G>::is_graph_list;
template <typename G>
constexpr bool is_graph_matrix_v = is_graph_checker<G>::is_graph_matrix;

template <template <typename> typename graph_t, typename T,
         enable_if_t<is_graph_v<graph_t<T>>, bool> = true>
ostream& operator << (ostream& os, const graph_t<T>& g) {
  for (int u = 0; u < g.size(); u++) {
    os << u << " -> [";
    if constexpr (is_void_v<T>) {
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
