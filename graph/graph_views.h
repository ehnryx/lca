/* Graph (???)
 * USAGE
 *  graph_list<graph_edge<weight_t>> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

#include <type_traits>
#include <utility>

template <typename> struct graph_list;
template <typename> struct graph_matrix;

template <typename weight_t>
struct graph_adj {
  int to;
  weight_t weight;
  graph_adj(int v, weight_t w): to(v), weight(w) {}
};

template <>
struct graph_adj<void> {
  int to;
  graph_adj(int v): to(v) {}
  operator int() const { return to; }
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

static_assert(sizeof(graph_edge<void>) == sizeof(std::pair<int, int>));
static_assert(sizeof(graph_adj<void>) == sizeof(int));

template <typename weight_t>
class list_edge_list_view {
  const graph_list<weight_t>* _graph;
  struct iterator {
    const graph_list<weight_t>* graph;
    int from, to;
    iterator(): graph(nullptr), from(-1), to(-1) {}
    iterator(const graph_list<weight_t>* g, int f, int t): graph(g), from(f), to(t) {}
    graph_edge<weight_t> operator * () const {
      return graph_edge<weight_t>(from, graph->adj[from][to]);
    }
    iterator& operator ++ () {
      while (from < (int)graph->adj.size()) {
        if (++to == (int)graph->adj[from].size()) {
          to = -1;
          from++;
          continue;
        }
        break;
      }
      return *this;
    }
    bool operator == (const iterator& o) const {
      return to == o.to && from == o.from;
    }
    bool operator != (const iterator& o) const { return !operator == (o); } // c++17
  };
public:
  list_edge_list_view(const graph_list<weight_t>* g): _graph(g) {}
  iterator begin() const {
    return ++iterator(_graph, 0, -1);
  }
  iterator end() const {
    return iterator(_graph, (int)_graph->adj.size(), -1);
  }
};

template <typename weight_t>
class matrix_adj_list_view {
  const graph_matrix<weight_t>* _graph;
  int _from;
  struct iterator {
    const graph_matrix<weight_t>* graph;
    int from, to;
    iterator(): graph(nullptr), from(-1), to(-1) {}
    iterator(const graph_matrix<weight_t>* g, int f, int t): graph(g), from(f), to(t) {}
    graph_adj<weight_t> operator * () const {
      if constexpr (std::is_void_v<weight_t>) {
        return graph_adj<weight_t>(to);
      } else {
        return graph_adj<weight_t>(to, graph->adj[from][to]);
      }
    }
    iterator& operator ++ () {
      while (++to < (int)graph->adj[from].size()) {
        if constexpr (std::is_void_v<weight_t>) {
          if (graph->adj[from][to]) {
            break;
          }
        } else {
          if (graph->adj[from][to] != graph->inf) {
            break;
          }
        }
      }
      return *this;
    }
    bool operator == (const iterator& o) const {
      return to == o.to;
    }
    bool operator != (const iterator& o) const { return !operator == (o); } // c++17
  };
public:
  matrix_adj_list_view(const graph_matrix<weight_t>* g, int u):
    _graph(g), _from(u) {}
  iterator begin() const {
    return ++iterator(_graph, _from, -1);
  }
  iterator end() const {
    return iterator(_graph, _from, (int)_graph->adj[_from].size());
  }
};

template <typename weight_t>
class matrix_edge_list_view {
  const graph_matrix<weight_t>* _graph;
  struct iterator {
    const graph_matrix<weight_t>* graph;
    int from, to;
    iterator(): graph(nullptr), from(-1), to(-1) {}
    iterator(const graph_matrix<weight_t>* g, int f, int t): graph(g), from(f), to(t) {}
    graph_edge<weight_t> operator * () const {
      if constexpr (std::is_void_v<weight_t>) {
        return graph_edge<weight_t>(from, to);
      } else {
        return graph_edge<weight_t>(from, to, graph->adj[from][to]);
      }
    }
    iterator& operator ++ () {
      while (from < (int)graph->adj.size()) {
        if (++to == (int)graph->adj[from].size()) {
          to = -1;
          from++;
          continue;
        }
        if constexpr (std::is_void_v<weight_t>) {
          if (graph->adj[from][to]) {
            break;
          }
        } else {
          if (graph->adj[from][to] != graph->inf) {
            break;
          }
        }
      }
      return *this;
    }
    bool operator == (const iterator& o) const {
      return to == o.to && from == o.from;
    }
    bool operator != (const iterator& o) const { return !operator == (o); } // c++17
  };
public:
  matrix_edge_list_view(const graph_matrix<weight_t>* g): _graph(g) {}
  iterator begin() const {
    return ++iterator(_graph, 0, -1);
  }
  iterator end() const {
    return iterator(_graph, (int)_graph->adj.size(), -1);
  }
};

