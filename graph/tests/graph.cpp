#include <bits/stdc++.h>
using namespace std;

#include "../graph.h"

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, m;
  cin >> n >> m;

  //cout << "instantiating graphs " << n << " ..." << endl;
  graph_list<void> unweighted_list(n);
  graph_list<int> weighted_list(n);
  graph_matrix<void> unweighted_matrix(n);
  graph_matrix<int> weighted_matrix(n, -1);
  //cout << "DONE!" << endl;

  for (int i = 0; i < m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    //cout << "adding edge " << a << " -> " << b << " weight " << c << " ..." << endl;
    unweighted_list.add_edge(a, b);
    weighted_list.add_edge(a, b, c);
    unweighted_matrix.add_edge(a, b);
    weighted_matrix.add_edge(a, b, c);
    //cout << "DONE!" << endl;
  }

  cout << "unweighted_list" << endl << unweighted_list << endl;
  cout << "weighted_list" << endl << weighted_list << endl;
  cout << "unweighted_matrix" << endl << unweighted_matrix << endl;
  cout << "weighted_matrix" << endl << weighted_matrix << endl;

  cout << "-------------------------------------------" << endl;
  cout << "edges (unweigted_list):" << endl;
  for(auto [a, b] : unweighted_list.get_edges()) cout << a << " " << b << endl;
  cout << "edges (weigted_list):" << endl;
  for(auto [a, b, c] : weighted_list.get_edges()) cout << a << " " << b << " " << c << endl;
  cout << "edges (unweigted_matrix):" << endl;
  for(auto [a, b] : unweighted_matrix.get_edges()) cout << a << " " << b << endl;
  cout << "edges (weigted_matrix):" << endl;
  for(auto [a, b, c] : weighted_matrix.get_edges()) cout << a << " " << b << " " << c << endl;

  // edge list
  cout << "\n-------------------------------------------" << endl;
  cout << "Edge list for testing iterator" << endl;
  auto it = unweighted_list.get_edges().begin();
  while (it != unweighted_list.get_edges().end()) {
    auto old = it;
    auto [a, b] = *it++;
    assert(a == old->from && b == old->to);
    cout << a << " " << b << endl;
  }

  return 0;
}
