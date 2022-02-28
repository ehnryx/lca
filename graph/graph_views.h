/* Graph (???)
 * USAGE
 *  graph_list<edge_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

template <typename> struct graph_list;
template <typename> struct graph_matrix;

template <typename weight_t>
class list_edge_list_view {
  static constexpr bool _weighted = graph_list<weight_t>::weighted;
  const graph_list<weight_t>* _graph;
  struct iterator {
    using out_t = conditional_t<_weighted, tuple<int, int, weight_t>, pair<int, int>>;
    const graph_list<weight_t>* graph;
    int from, to;
    iterator(): graph(nullptr), from(-1), to(-1) {}
    iterator(const graph_list<weight_t>* g, int f, int t): graph(g), from(f), to(t) {}
    out_t operator * () const {
      if constexpr (_weighted) {
        return tuple(from, graph->adj[from][to].first, graph->adj[from][to].second);
      } else {
        return pair(from, graph->adj[from][to]);
      }
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
    bool operator == (const iterator& o) const {
      return to == o.to;
    }
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
  static constexpr bool _weighted = graph_matrix<weight_t>::weighted;
  const graph_matrix<weight_t>* _graph;
  struct iterator {
    using out_t = conditional_t<_weighted, tuple<int, int, weight_t>, pair<int, int>>;
    const graph_matrix<weight_t>* graph;
    int from, to;
    iterator(): graph(nullptr), from(-1), to(-1) {}
    iterator(const graph_matrix<weight_t>* g, int f, int t): graph(g), from(f), to(t) {}
    out_t operator * () const {
      if constexpr (_weighted) {
        return tuple(from, to, graph->adj[from][to]);
      } else {
        return pair(from, to);
      }
    }
    iterator& operator ++ () {
      while (from < (int)graph->adj.size()) {
        if (++to == (int)graph->adj[from].size()) {
          to = -1;
          from++;
          continue;
        }
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
    bool operator == (const iterator& o) const {
      return to == o.to && from == o.from;
    }
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

