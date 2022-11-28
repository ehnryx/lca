/* Graph (???)
 * USAGE
 *  ???
 * STATUS
 *  untested
 */
#pragma once

#include <utility>

template <typename> struct graph_adj;
template <typename> struct graph_edge;

template <typename weight_t>
struct graph_adj {
  int to;
  weight_t weight;
  graph_adj(int v, weight_t w): to(v), weight(w) {}
  graph_adj(const std::pair<int, weight_t>& e): to(e.first), weight(e.second) {}
  graph_adj(const graph_edge<weight_t>& e): to(e.to), weight(e.weight) {}
  bool operator<(const graph_adj& other) const { return weight < other.weight; }
  bool operator>(const graph_adj& other) const { return weight > other.weight; }
};

template <typename weight_t>
struct graph_edge {
  int from, to;
  weight_t weight;
  graph_edge(int u, int v, weight_t w): from(u), to(v), weight(w) {}
  graph_edge(const std::tuple<int, int, weight_t>& e):
    from(get<0>(e)), to(get<1>(e)), weight(get<2>(e)) {}
  graph_edge(int u, graph_adj<weight_t> adj): from(u), to(adj.to), weight(adj.weight) {}
  bool operator<(const graph_edge& other) const { return weight < other.weight; }
  bool operator>(const graph_edge& other) const { return weight > other.weight; }
};

template <>
struct graph_edge<void> {
  int from, to;
  graph_edge(int u, int v): from(u), to(v) {}
  operator std::pair<int, int>() const { return std::pair(from, to); }
};

template <>
struct graph_adj<void> {
  int to;
  graph_adj(int v): to(v) {}
  graph_adj(graph_edge<void> e): to(e.to) {}
  operator int() const { return to; }
};

static_assert(sizeof(graph_adj<void>) == sizeof(int));
static_assert(sizeof(graph_edge<void>) == sizeof(std::pair<int, int>));

