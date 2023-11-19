/* 2 SAT
 * USAGE
 *  two_sat sat(n); // n = number of variables, should satisfy n > 0
 *  2*i     = x_i is true
 *  2*i + 1 = x_i is false
 *  sat.or_clause(a, b); // adds the clause a OR b is true
 *  bool yes = sat.solve();
 *  sat[i] = whether x_i is true
 * TIME
 *  O(N + M)
 *  N = #number of variables, M = #number of edges
 * STATUS
 *  tested: cf/102875l,104772c
 */
#pragma once

#include "strongly_connected.h"

struct two_sat {
  graph_t<void> graph;
  std::vector<char> truth;  // no vector<bool>
  two_sat(int n): graph(2*n), truth(n) {}
  void reset(int n) {
    graph.reset(2*n);
    truth.resize(n);
  }
  bool operator[](int i) const { return truth[i]; }
  bool solve() {
    strongly_connected scc(graph);
    for (int i = 0; i < truth.size(); i++) {
      if (scc[2*i] == scc[2*i + 1]) return false;
      truth[i] = (scc[2*i] < scc[2*i + 1]);
    }
    return true;
  }

  void either(int a, int b) {
    graph.add_arc(a^1, b);
    graph.add_arc(b^1, a);
  }
  void implies(int a, int b) {
    graph.add_arc(a, b);
    graph.add_arc(b^1, a^1);
  }
  void iff(int a, int b) {
    implies(a, b);
    implies(b, a);
  }
  void either(int a, bool a_truth, int b, bool b_truth) {
    either(2*a + !a_truth, 2*b + !b_truth);
  }
  void implies(int a, bool a_truth, int b, bool b_truth) {
    implies(2*a + !a_truth, 2*b + !b_truth);
  }
  void iff(int a, bool a_truth, int b, bool b_truth) {
    iff(2*a + !a_truth, 2*b + !b_truth);
  }
};

