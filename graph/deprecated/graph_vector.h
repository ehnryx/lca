/* GraphVector (???)
 * USAGE
 *  ???
 * STATUS
 *  untested
 */
#pragma once

#include "graph.h"

template <typename _weight_t>
struct Graph<void, _weight_t> : std::vector<std::vector<graph_adj<_weight_t>>> {
  using base = std::vector<std::vector<graph_adj<_weight_t>>>;
  using base::base, base::at, base::operator[], base::emplace_back;
  using weight_t = _weight_t;
  static constexpr bool weighted = true;
  inline int size() const { return (int)base::size(); }
  inline void add_edge(int a, int b, weight_t c) {
    add_arc(a, b, c);
    add_arc(b, a, c);
  }
  inline void add_arc(int a, int b, weight_t c) {
    operator[](a).emplace_back(b, c);
  }
  std::vector<graph_edge<weight_t>> get_edges() const {
    std::vector<graph_edge<weight_t>> edges;
    for (int i = 0; i < size(); i++) {
      for (graph_adj<weight_t> j : at(i)) {
        edges.emplace_back(i, j);
      }
    }
    return edges;
  }
};

template <>
struct Graph<void, void> : std::vector<std::vector<int>> {
  using base = std::vector<std::vector<int>>;
  using base::base, base::at, base::operator[], base::push_back;
  using weight_t = void;
  static constexpr bool weighted = false;
  inline int size() const { return (int)base::size(); }
  inline void add_edge(int a, int b) {
    add_arc(a, b);
    add_arc(b, a);
  }
  inline void add_arc(int a, int b) {
    operator[](a).emplace_back(b);
  }
  std::vector<graph_edge<weight_t>> get_edges() const {
    std::vector<graph_edge<weight_t>> edges;
    for (int i = 0; i < size(); i++) {
      for (graph_adj<weight_t> j : at(i)) {
        edges.emplace_back(i, j);
      }
    }
    return edges;
  }
};

template <typename weight_t> using GraphVector = Graph<void, weight_t>;

