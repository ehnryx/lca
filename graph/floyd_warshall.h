/* Floyd Warshall
 * USAGE
 *  floyd_warshall<T> graph;
 * OUTPUT
 *  graph.add_edge/add_arc(n)
 *  graph.run();
 * TIME
 *  O(V^3)
 *  V = #vertices
 * STATUS
 *  untested
 */
#pragma once

#include "../utility/utility.h"
#include <functional>
#include <iostream>

template <typename weight_t, class Smaller = utility::min<>>
struct floyd_warshall {
  std::vector<std::vector<weight_t>> adj;
  const weight_t inf;
  floyd_warshall(int n, weight_t _inf, bool self_zero = true):
    adj(n, std::vector<weight_t>(n, _inf)), inf(_inf) {
    if (self_zero) {
      for (int i = 0; i < n; i++) {
        adj[i][i] = weight_t();
      }
    }
  }
  void add_edge(int a, int b, weight_t c) {
    add_arc(a, b, c);
    add_arc(b, a, c);
  }
  void add_arc(int a, int b, weight_t c) {
    adj[a][b] = Smaller()(adj[a][b], c);
  }
  template <typename add_t>
  void run(const add_t& add) {
    for (size_t k = 0; k < adj.size(); k++) {
      for (size_t i = 0; i < adj.size(); i++) {
        for (size_t j = 0; j < adj.size(); j++) {
          if (adj[i][k] == inf || adj[k][j] == inf) continue;
          adj[i][j] = Smaller()(adj[i][j], add(adj[i][k], adj[k][j]));
        }
      }
    }
  }
  void run() { return run(std::plus<>()); }
  const std::vector<weight_t>& operator[](int i) const { return adj.at(i); }
  friend std::ostream& operator<<(std::ostream& os, const floyd_warshall& g) {
    for (size_t i = 0; i < g.adj.size(); i++) {
      for (size_t j = 0; j < g.adj[i].size(); j++) {
        if (j) os << " ";
        os << g[i][j];
      }
      os << "\n";
    }
    return os;
  }
};

