/* Graph (AdjacencyMatrix)
 * USAGE
 *  ???
 * STATUS
 *  tested boj/11404,9205,1956,4234,10473
 */
#pragma once

#include "../utility/nd_array.h"
#include "../utility/utility.h"
#include "graph_common.h"

#include <functional>
#include <span>

template <typename weight_t>
struct graph_matrix_traits_base {
  using value_t = std::conditional_t<std::is_void_v<weight_t>, int, weight_t>;
  static constexpr value_t inf = std::numeric_limits<value_t>::max();
  using edge_update_t = utility::min<value_t>;
  //using edge_update_t = utility::floored_min<
  //    value_t, std::numeric_limits<value_t>::min() / 2, std::less<value_t>>;
  static constexpr bool self_zero = true;
};

namespace graph_utils {
template <typename weight_t, graph_traits traits>
  requires AdjacencyMatrix<traits>
struct default_traits<weight_t, traits> {
  using type = graph_matrix_traits_base<weight_t>;
};

template <typename graph_matrix_adj_view_t>
struct graph_matrix_iterator {
  using value_t = graph_matrix_adj_view_t::value_t;
  graph_matrix_adj_view_t const& adj_view;
  value_t const* it;
  graph_matrix_iterator(graph_matrix_adj_view_t const& view, size_t u)
      : adj_view(view), it(view.view_ptr + u) {
    make_valid();
  }
  bool operator==(graph_matrix_iterator const& other) const {
    return adj_view == other.adj_view && it == other.it;
  }
  graph_matrix_iterator& operator++() {
    ++it;
    make_valid();
    return *this;
  }
  graph_adj<value_t> operator*() { return graph_adj<value_t>(get_index(), *it); }
  void make_valid() {
    while (it != adj_view.view_ptr + adj_view.view_size &&
           *it == graph_matrix_adj_view_t::inf) {
      ++it;
    }
  }
  int get_index() const { return it - adj_view.view_ptr; }
};
}  // namespace graph_utils

template <typename _value_t, _value_t _inf>
struct graph_matrix_adj_view {
  using value_t = _value_t;
  static constexpr value_t inf = _inf;
  value_t const* const view_ptr;
  size_t const view_size;
  graph_matrix_adj_view(value_t const* const ptr, size_t size)
      : view_ptr(ptr), view_size(size) {}
  bool operator==(graph_matrix_adj_view const& other) const {
    return view_ptr == other.view_ptr && view_size == other.view_size;
  }
  size_t size() const { return view_size; }
  using const_iterator = graph_utils::graph_matrix_iterator<graph_matrix_adj_view>;
  const_iterator begin() const { return const_iterator(*this, 0); }
  const_iterator end() const { return const_iterator(*this, size()); }
};

template <typename weight_t, graph_traits traits, typename graph_matrix_tt>
  requires AdjacencyMatrix<traits>
struct graph_base_edges<weight_t, traits, graph_matrix_tt> {
  using value_t = graph_matrix_tt::value_t;
  using base_edge_update_t = graph_matrix_tt::edge_update_t;
  static constexpr value_t inf = graph_matrix_tt::inf;
  nd_array<value_t, 2> matrix;
  graph_base_edges(size_t n) : matrix(n, n, inf) {
    if (graph_matrix_tt::self_zero) {
      for (size_t i = 0; i < n; i++) {
        matrix(i, i) = value_t();
      }
    }
  }
  graph_matrix_adj_view<value_t, inf> operator[](int u) const {
    return graph_matrix_adj_view<value_t, inf>(matrix.data.data() + u * size(), size());
  }
  value_t& operator()(int u, int v) { return matrix(u, v); }
  value_t const& operator()(int u, int v) const { return matrix(u, v); }
  int size() const { return matrix.indexer.decltype(matrix.indexer)::nested_indexer::size(); }
  void reset(size_t n) { matrix = decltype(matrix)(n, n, inf); }
  template <typename... weight_args>
    requires ValidWeightArg<weight_t, weight_args...>
  void add_arc(int from, int to, weight_args... weight) {
    if constexpr (std::is_void_v<weight_t>) {
      matrix(from, to) = base_edge_update_t()(matrix(from, to), 1);
    } else {
      matrix(from, to) = base_edge_update_t()(matrix(from, to), (weight, ...));
    }
  }
  std::vector<graph_edge<weight_t>> get_edges() const {
    std::vector<graph_edge<weight_t>> edges;
    for (int u = 0; u < size(); u++) {
      for (int v = 0; v < size(); v++) {
        if (matrix(u, v) == inf) continue;
        if constexpr (std::is_void_v<weight_t>) {
          edges.emplace_back(u, v);
        } else {
          edges.emplace_back(u, v, matrix(u, v));
        }
      }
    }
    return edges;
  }

  // floyd warshall
  template <typename add_t = std::plus<>, typename edge_update_t = void>
  void floyd_warshall(int start_i = 0) {
    using edge_update =
        std::conditional_t<!std::is_void_v<edge_update_t>, edge_update_t, base_edge_update_t>;
    for (int k = start_i; k < size(); k++) {
      for (int i = start_i; i < size(); i++) {
        for (int j = start_i; j < size(); j++) {
          if (matrix(i, k) == inf || matrix(k, j) == inf) continue;
          matrix(i, j) = edge_update()(matrix(i, j), add_t()(matrix(i, k), matrix(k, j)));
        }
      }
    }
  }
};
