#include <bits/stdc++.h>
using namespace std;

#include "../graph.h"

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, m;
  cin >> n >> m;

  //cerr << "instantiating graphs " << n << " ..." << endl;
  graph_list<void> unweighted_list(n);
  graph_list<int> weighted_list(n);
  graph_matrix<void> unweighted_matrix(n);
  graph_matrix<int> weighted_matrix(n, -1);
  //cerr << "DONE!" << endl;

  for (int i = 0; i < m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    //cerr << "adding edge " << a << " -> " << b << " weight " << c << " ..." << endl;
    unweighted_list.add_edge(a, b);
    weighted_list.add_edge(a, b, c);
    unweighted_matrix.add_edge(a, b);
    weighted_matrix.add_edge(a, b, c);
    //cerr << "DONE!" << endl;
  }

  cerr << "unweighted_list" << endl << unweighted_list << endl;
  cerr << "weighted_list" << endl << weighted_list << endl;
  cerr << "unweighted_matrix" << endl << unweighted_matrix << endl;
  cerr << "weighted_matrix" << endl << weighted_matrix << endl;

  cerr << "-------------------------------------------" << endl;
  cerr << "edges (unweigted_list):" << endl;
  for(auto [a, b] : unweighted_list.get_edges()) cerr << a << " " << b << endl;
  cerr << "edges (weigted_list):" << endl;
  for(auto [a, b, c] : weighted_list.get_edges()) cerr << a << " " << b << " " << c << endl;
  cerr << "edges (unweigted_matrix):" << endl;
  for(auto [a, b] : unweighted_matrix.get_edges()) cerr << a << " " << b << endl;
  cerr << "edges (weigted_matrix):" << endl;
  for(auto [a, b, c] : weighted_matrix.get_edges()) cerr << a << " " << b << " " << c << endl;

  return 0;
}
