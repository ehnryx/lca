/* Dinic utility for bipartite graphs
 * USAGE
 *  I forgot
 * STATUS
 *  untested
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
std::vector<int> max_independent_set(const dinic<T>& graph, int source, int sink) {
  std::vector<int> rem, all;
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
  all.resize(distance(begin(all),
        set_difference(begin(all), end(all), begin(rem), end(rem), begin(all))));
  return all;
}

