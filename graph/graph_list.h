/* Graph (???)
 * USAGE
 *  GraphList<edge_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

#include "graph.h"

template <typename> struct GraphList;

namespace graph_list_details {
  template <typename weight_t>
  struct adj_iterator : std::vector<graph_adj<weight_t>>::const_iterator {
    using base = std::vector<graph_adj<weight_t>>::const_iterator;
    explicit adj_iterator(base it): base(it) {}
    static adj_iterator begin(const GraphList<weight_t>* g, int from) {
      return adj_iterator(g->adj[from].begin());
    }
    static adj_iterator end(const GraphList<weight_t>* g, int from) {
      return adj_iterator(g->adj[from].end());
    }
  };

  template <typename weight_t>
  struct edge_iterator {
    const GraphList<weight_t>* const g;
    int from, index;
    explicit edge_iterator(const GraphList<weight_t>* gptr, int f, int i):
      g(gptr), from(f), index(i) {}
    bool operator == (const edge_iterator& o) const {
      return from == o.from && index == o.index;
    }
    graph_edge<weight_t> operator * () const {
      return graph_edge(from, g->adj[from][index]);
    }
    edge_iterator& operator ++ () {
      ++index;
      while (from != g->size() && index == (int)g->adj[from].size()) {
        ++from;
        index = 0;
      }
      return *this;
    }
    static edge_iterator begin(const GraphList<weight_t>* g) {
      return ++edge_iterator(g, 0, -1);
    }
    static edge_iterator end(const GraphList<weight_t>* g) {
      return edge_iterator(g, g->size(), 0);
    }
  };
}

template <typename weight_t>
struct GraphList : Graph<GraphList<weight_t>, weight_t> {
  using base = Graph<GraphList<weight_t>, weight_t>;
  std::vector<std::vector<graph_adj<weight_t>>> adj;
  GraphList(int n, bool dir = false): base(n, dir), adj(n) {}
  int size() const override final {
    return (int)adj.size();
  }
  void add_arc_impl(int a, int b, weight_t c) override final {
    adj[a].emplace_back(b, c);
  }
  using edge_iterator = graph_list_details::edge_iterator<weight_t>;
  using adj_iterator = graph_list_details::adj_iterator<weight_t>;
};

template <>
struct GraphList<void> : Graph<GraphList<void>, void> {
  using base = Graph<GraphList<weight_t>, weight_t>;
  std::vector<std::vector<graph_adj<weight_t>>> adj;
  GraphList(int n, bool dir = false): base(n, dir), adj(n) {}
  int size() const override final {
    return (int)adj.size();
  }
  void add_arc_impl(int a, int b) override final {
    adj[a].emplace_back(b);
  }
  using edge_iterator = graph_list_details::edge_iterator<weight_t>;
  using adj_iterator = graph_list_details::adj_iterator<weight_t>;
};

