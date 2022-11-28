/* Graph (???)
 * USAGE
 *  GraphMatrix<edge_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

#include "../utility/utility.h"
#include "graph.h"

template <typename, class> struct GraphMatrix;

namespace graph_matrix_details {
  template <typename weight_t, class Compare>
  struct adj_iterator {
    const GraphMatrix<weight_t, Compare>* const g;
    const int from;
    int to;
    explicit adj_iterator(const GraphMatrix<weight_t, Compare>* gptr, int f, int t):
      g(gptr), from(f), to(t) {}
    bool operator == (const adj_iterator& o) const {
      return from == o.from && to == o.to;
    }
    graph_adj<weight_t> operator * () const {
      if constexpr (std::is_void_v<weight_t>) {
        return graph_adj<weight_t>(to);
      } else {
        return graph_adj(to, g->mat[from][to]);
      }
    }
    adj_iterator& operator ++ () {
      increment_one_step();
      while (to != g->size() && g->mat[from][to] == g->infinity) {
        increment_one_step();
      }
      return *this;
    }
    void increment_one_step() {
      ++to;
    }
    static adj_iterator begin(const GraphMatrix<weight_t, Compare>* g, int from) {
      return ++adj_iterator(g, from, -1);
    }
    static adj_iterator end(const GraphMatrix<weight_t, Compare>* g, int from) {
      return adj_iterator(g, from, g->size());
    }
  };

  template <typename weight_t, class Compare>
  struct edge_iterator {
    const GraphMatrix<weight_t, Compare>* const g;
    int from, to;
    explicit edge_iterator(const GraphMatrix<weight_t, Compare>* gptr, int f, int t):
      g(gptr), from(f), to(t) {}
    bool operator == (const edge_iterator& o) const {
      return from == o.from && to == o.to;
    }
    graph_edge<weight_t> operator * () const {
      if constexpr (std::is_void_v<weight_t>) {
        return graph_edge<weight_t>(from, to);
      } else {
        return graph_edge(from, to, g->mat[from][to]);
      }
    }
    edge_iterator& operator ++ () {
      increment_one_step();
      while (from != g->size() && g->mat[from][to] == g->infinity) {
        increment_one_step();
      }
      return *this;
    }
    void increment_one_step() {
      if (++to == g->size()) {
        ++from;
        to = 0;
      }
    }
    static edge_iterator begin(const GraphMatrix<weight_t, Compare>* g) {
      return ++edge_iterator(g, 0, -1);
    }
    static edge_iterator end(const GraphMatrix<weight_t, Compare>* g) {
      return edge_iterator(g, g->size(), 0);
    }
  };
}

template <typename weight_t, class Compare = utility::replace<weight_t>>
struct GraphMatrix : Graph<GraphMatrix<weight_t, Compare>, weight_t> {
  using base = Graph<GraphMatrix<weight_t, Compare>, weight_t>;
  std::vector<std::vector<weight_t>> mat;
  const weight_t infinity;
  GraphMatrix(int n, weight_t inf, bool dir = false):
    base(n, dir), mat(n, std::vector<weight_t>(n, inf)), infinity(inf) {}
  int size() const override final {
    return (int)mat.size();
  }
  void add_arc_impl(int a, int b, weight_t c) override final {
    mat[a][b] = (mat[a][b] == infinity ? c : Compare()(mat[a][b], c));
  }
  using edge_iterator = graph_matrix_details::edge_iterator<weight_t, Compare>;
  using adj_iterator = graph_matrix_details::adj_iterator<weight_t, Compare>;
};

template <class Compare>
struct GraphMatrix<void, Compare> : Graph<GraphMatrix<void, Compare>, void> {
  using base = Graph<GraphMatrix<void, Compare>, void>;
  using weight_t = base::weight_t;
  std::vector<std::vector<bool>> mat;
  static constexpr bool infinity = false;
  GraphMatrix(int n, bool dir = false):
    base(n, dir), mat(n, std::vector<bool>(n, false)) {}
  int size() const override final {
    return (int)mat.size();
  }
  void add_arc_impl(int a, int b) override final {
    mat[a][b] = (mat[a][b] == infinity ? true : Compare()((bool)mat[a][b], true));
  }
  using edge_iterator = graph_matrix_details::edge_iterator<weight_t, Compare>;
  using adj_iterator = graph_matrix_details::adj_iterator<weight_t, Compare>;
};

