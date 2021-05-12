/* 2 SAT
 * USAGE
 *  two_sat sat(n); // n = number of variables, should satisfy n > 0
 *  2*i     = x_i is true
 *  2*i + 1 = x_i is false
 *  sat.or_clause(a, b); // adds the clause a OR b is true
 *  vector<bool> sat.solve(); // empty means unsolvable
 *  sat[i] = whether x_i is true
 * TIME
 *  O(N + M)
 *  N = #number of variables, M = #number of edges
 * STATUS
 *  tested: cf/102875l
 */
#pragma once

#include "strongly_connected.h"

struct two_sat {
  int n;
  vector<bool> truth;
  vector<vector<int>> adj;
  two_sat(int _n): n(_n), truth(n), adj(2*n) {}
  void add_edge(int a, int b) { adj[a].push_back(b); }
  bool operator [] (int i) const { return truth[i]; }
  bool solve() {
    strongly_connected scc(adj);
    for (int i = 0; i < n; i++) {
      if (scc[2*i] == scc[2*i + 1]) return false;
      truth[i] = (scc[2*i] < scc[2*i + 1]);
    }
    return true;
  }

  void or_clause(int a, int b) {
    add_edge(a^1, b);
    add_edge(b^1, a);
  }
  void implies(int a, int b) {
    add_edge(a, b);
    add_edge(b^1, a^1);
  }
  void or_clause(int a, bool a_truth, int b, bool b_truth) {
    or_clause(2*a + !a_truth, 2*b + !b_truth);
  }
  void implies(int a, bool a_truth, int b, bool b_truth) {
    implies(2*a + !a_truth, 2*b + !b_truth);
  }
};

