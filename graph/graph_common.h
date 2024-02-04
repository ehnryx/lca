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
  (IN_DEGREE, ADJACENCY_MATRIX, CUSTOM_GRAPH),
);
// clang-format on

template <graph_traits traits>
concept CustomGraphType = (traits.count(graph_traits::CUSTOM_GRAPH) == 1);

template <graph_traits traits>
concept AdjacencyMatrix = (traits.count(graph_traits::ADJACENCY_MATRIX) == 1);

template <graph_traits traits>
concept AdjacencyList = !CustomGraphType<traits> && !AdjacencyMatrix<traits>;

template <graph_traits traits>
concept ValidGraphTraits =
    CustomGraphType<traits> || AdjacencyList<traits> || AdjacencyMatrix<traits>;

template <graph_traits>
struct graph_base_in_degree {
  graph_base_in_degree(size_t) {}
  void reset(size_t) {}
};

template <graph_traits traits>
  requires(traits.has_any(graph_traits::IN_DEGREE))
struct graph_base_in_degree<traits> {
  std::vector<int> in_degree;
  graph_base_in_degree(size_t n) : in_degree(n) {}
  void reset(size_t n) {
    in_degree.resize(n);
    fill(in_degree.begin(), in_degree.end(), 0);
  }
};

template <typename weight_t, typename... weight_args>
concept ValidWeightArg = [] {
  if constexpr (std::is_void_v<weight_t>) {
    return sizeof...(weight_args) == 0;
  } else {
    return sizeof...(weight_args) == 1 && (... && std::is_same_v<weight_t, weight_args>);
  }
}();

template <typename weight_t, graph_traits traits, typename extra_args_t>
  requires ValidGraphTraits<traits>
struct graph_base_edges {
  // int size() const;
  // void reset(size_t);
  // void add_arc(int from, int to, weight_t... weight);
  // std::vector<graph_edge<weight_t>> get_edges() const;
  static_assert(!std::is_same_v<weight_t, weight_t>, "Graph type not implemented");
};

namespace graph_utils {
template <typename weight_t, graph_traits>
struct default_traits {
  using type = void;
};
}  // namespace graph_utils

template <
    typename _weight_t = void, graph_traits traits = graph_traits::NONE,
    typename extra_args_t = graph_utils::default_traits<_weight_t, traits>::type>
  requires ValidGraphTraits<traits>
struct graph_t : graph_base_edges<_weight_t, traits, extra_args_t>,
                 graph_base_in_degree<traits> {
  using weight_t = _weight_t;
  using edge_t = graph_edge<weight_t>;
  using adj_t = graph_adj<weight_t>;
  using graph_base = graph_base_edges<weight_t, traits, extra_args_t>;
  static constexpr bool weighted = not std::is_void_v<weight_t>;
  graph_t(int n) : graph_base(n), graph_base_in_degree<traits>(n) {}
  void reset(int n) {
    graph_base::reset(n);
    graph_base_in_degree<traits>::reset(n);
  }
  template <typename... weight_args>
    requires ValidWeightArg<weight_t, weight_args...>
  void add_edge(int u, int v, weight_args... weight) {
    add_arc(u, v, weight...);
    if (u != v) add_arc(v, u, weight...);
  }
  template <typename... weight_args>
    requires ValidWeightArg<weight_t, weight_args...>
  void add_arc(int from, int to, weight_args... weight) {
    graph_base::add_arc(from, to, weight...);
    if constexpr (traits.has_any(graph_traits::IN_DEGREE)) {
      this->in_degree[to]++;
    }
  }
};
