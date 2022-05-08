/* Graph Builder (???)
 * USAGE
 *  graph_builder<edge_t> graph_builder;
 *  fast_input >> graph_builder; // eh ?
 * STATUS
 *  untested
 */
#pragma once

#include "../utility/fast_input.h"
#include "graph.h"

// AJDACENCY LIST

template <typename weight_t, bool directed = false, int indexed = 1>
struct graph_builder {
  graph_list<weight_t> graph;
  int n, m;
  graph_builder(): graph(0, directed), n(0), m(0) {}
  template <size_t buf_size>
  void fast_read(fast_input<buf_size>& in) {
    in >> n >> m;
    graph = graph_list<weight_t>(n + indexed, directed);
    for (int i = 0; i < m; i++) {
      int a, b;
      in >> a >> b;
      if constexpr (std::is_void_v<weight_t>) {
        graph.add_edge(a, b);
      } else {
        weight_t c;
        in >> c;
        graph.add_edge(a, b, c);
      }
    }
  }
  operator const graph_list<weight_t>&() const { return graph; }
  const graph_list<weight_t>& operator * () const { return graph; }
};

