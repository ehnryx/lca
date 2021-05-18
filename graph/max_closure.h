/* Max Weight Closure
 * A closure of a directed graph is a subgraph with no outgoing edges.
 * USAGE
 *  max_closure<flow, T> graph(weights); // vector of weights
 *  graph.add_edge(a, b); // adds an edge a -> b
 *  T weight = graph.solve();
 *  vector<int> closure = graph.get_closure();
 * TIME
 *  O(flow)
 * STATUS
 *  tested: kattis/openpitmining
 */
#pragma once

#include "push_relabel.h"

template <template <typename> class Flow, typename T>
struct max_closure : Flow<T> {
  int n;
  max_closure(const vector<T>& weights):
    Flow<T>((int)size(weights) + 2), n((int)size(weights)) {
    for (int i = 0; i < n; i++) {
      if (weights[i] > 0) {
        Flow<T>::add_edge(n, i, weights[i]);
      } else if (weights[i] < 0) {
        Flow<T>::add_edge(i, n + 1, -weights[i]);
      }
    }
  }
  void add_edge(int a, int b) {
    Flow<T>::add_edge(a, b, numeric_limits<T>::max());
  }
  vector<int> solve() {
    if constexpr (is_same_v<Flow<T>, push_relabel<T>>) {
      Flow<T>::flow(n, n + 1, true);
    } else {
      Flow<T>::flow(n, n + 1);
    }
    vector<int> res;
    for (int i = 0; i < n; i++) {
      if (Flow<T>::left_of_min_cut(i)) {
        res.push_back(i);
      }
    }
    return res;
  }
};

