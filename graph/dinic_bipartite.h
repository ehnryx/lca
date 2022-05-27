/* Dinic (Max-Flow Min-Cut)
 * USAGE
 *  dinic<T> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, c, bidirectional?);
 *  T flow = graph.flow(source, sink);
 * MEMBERS
 *  add_edge(a, b, c, bidirectional?); adds an edge a -> b with capacity c.
 *    bidirectional? is a boolean specifying whether the edge is undirected.
 *    default is false
 *  flow(source, sink); returns max flow from source to sink.
 *  clear_flow(); clears the flow in the graph (untested)
 * TIME
 *  O(EV^2) in general
 *  O(E min(V^{2/3}, sqrtE)) for unit caps
 *  O(EsqrtV) for bipartite matchings (bipartite + unit caps)
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: kattis/maxflow,hubtown,risk,sandart, cf/100199b
 */
#pragma once

#include "dinic.h"
#include <algorithm>

template <typename T>
std::vector<int> min_vertex_cover(const dinic<T>& graph, int source, int sink) {
  std::vector<int> res;
  for (const auto& e : graph.adj[source]) {
    if (!graph.left_of_min_cut(e.to)) {
      res.push_back(e.to);
    }
  }
  for (const auto& e : graph.adj[sink]) {
    if (graph.left_of_min_cut(e.to)) {
      res.push_back(e.to);
    }
  }
  return res;
}

template <typename T>
std::vector<int> max_bipartite_set(const dinic<T>& graph, int source, int sink) {
  std::vector<int> rem, all, res;
  for (const auto& e : graph.adj[source]) {
    all.push_back(e.to);
    if (!graph.left_of_min_cut(e.to)) {
      rem.push_back(e.to);
    }
  }
  for (const auto& e : graph.adj[sink]) {
    all.push_back(e.to);
    if (graph.left_of_min_cut(e.to)) {
      rem.push_back(e.to);
    }
  }
  sort(begin(rem), end(rem));
  sort(begin(all), end(all));
  set_difference(begin(all), end(all), begin(rem), end(rem), back_inserter(res));
  return res;
}

