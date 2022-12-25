/* Graph EdgeList ?
 * USAGE
 *  same as graph.h
 * STATUS
 *  untested
 */
#pragma once

#include "graph.h"

template <typename> struct GraphEdgeList;

namespace graph_edgelist_details {
  template <typename weight_t>
  struct adj_iterator : std::vector<graph_edge<weight_t>>::const_iterator {
    using base = std::vector<graph_edge<weight_t>>::const_iterator;
    const GraphEdgeList<weight_t>* const g;
    explicit adj_iterator(base it, const GraphEdgeList<weight_t>* gptr): base(it), g(gptr) {}
    graph_adj<weight_t> operator * () const { return base::operator*(); }
    adj_iterator& operator ++ () {
      int from = (*(base*)this)->from;
      base::operator++();
      while ((*(base*)this) != g->edges.end() && (*(base*)this)->from != from) {
        base::operator++();
      }
      return *this;
    }
    static adj_iterator begin(const GraphEdgeList<weight_t>* g, int from) {
      base first = g->edges.begin();
      while (first != g->edges.end() && first->from != from) {
        ++first;
      }
      return adj_iterator(first, g);
    }
    static adj_iterator end(const GraphEdgeList<weight_t>* g, int) {
      return adj_iterator(g->edges.end(), g);
    }
  };

  template <typename weight_t>
  struct edge_iterator : std::vector<graph_edge<weight_t>>::const_iterator {
    using base = std::vector<graph_edge<weight_t>>::const_iterator;
    explicit edge_iterator(base it): base(it) {}
    static edge_iterator begin(const GraphEdgeList<weight_t>* g) {
      return edge_iterator(g->edges.begin());
    }
    static edge_iterator end(const GraphEdgeList<weight_t>* g) {
      return edge_iterator(g->edges.end());
    }
  };
}

template <typename weight_t>
struct GraphEdgeList : Graph<GraphEdgeList<weight_t>, weight_t> {
  using base = Graph<GraphEdgeList<weight_t>, weight_t>;
  std::vector<graph_edge<weight_t>> edges;
  GraphEdgeList(int n, bool dir = false): base(n, dir) {}
  int size() const override final {
    return (int)base::in_degree.size();
  }
  void add_arc_impl(int a, int b, weight_t c) override final {
    edges.emplace_back(a, b, c);
  }
  using edge_iterator = graph_edgelist_details::edge_iterator<weight_t>;
  using adj_iterator = graph_edgelist_details::adj_iterator<weight_t>;
};

template <>
struct GraphEdgeList<void> : Graph<GraphEdgeList<void>, void> {
  using base = Graph<GraphEdgeList<weight_t>, weight_t>;
  std::vector<graph_edge<weight_t>> edges;
  GraphEdgeList(int n, bool dir = false): base(n, dir) {}
  int size() const override final {
    return (int)base::in_degree.size();
  }
  void add_arc_impl(int a, int b) override final {
    edges.emplace_back(a, b);
  }
  using edge_iterator = graph_edgelist_details::edge_iterator<weight_t>;
  using adj_iterator = graph_edgelist_details::adj_iterator<weight_t>;
};

