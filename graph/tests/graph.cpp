#include "../graph_edgelist.h"
#include "../graph_list.h"
#include "../graph_matrix.h"
#include "../graph_vector.h"
#include "../floyd_warshall.h"

#include <iostream>
#include <tuple>
using namespace std;

void run_edgelist(int n, const vector<tuple<int, int, int>>& edges) {
  cout << "EDGELIST" << endl;

  GraphEdgeList<int> g(n, true);
  GraphEdgeList<void> unwg(n, true);

  for (auto [a, b, c] : edges) {
    g.add_edge(a, b, c);
    unwg.add_edge(a, b);
  }

  cout << "WEIGHTED\n" << g << endl;
  for (auto [a, b, c] : g.get_edges()) {
    cout << a << " " << b << " " << c << endl;
  }
  cout << endl;

  cout << "UNWEIGHTED\n" << unwg << endl;
  for (auto [a, b] : unwg.get_edges()) {
    cout << a << " " << b << " " << endl;
  }
  cout <<endl;
}

void run_list(int n, const vector<tuple<int, int, int>>& edges) {
  cout << "LIST" << endl;

  GraphList<int> g(n, true);
  GraphList<void> unwg(n, true);

  for (auto [a, b, c] : edges) {
    g.add_edge(a, b, c);
    unwg.add_edge(a, b);
  }

  cout << "WEIGHTED\n" << g << endl;
  for (auto [a, b, c] : g.get_edges()) {
    cout << a << " " << b << " " << c << endl;
  }
  cout << endl;

  cout << "UNWEIGHTED\n" << unwg << endl;
  for (auto [a, b] : unwg.get_edges()) {
    cout << a << " " << b << " " << endl;
  }
  cout <<endl;
}

void run_matrix(int n, const vector<tuple<int, int, int>>& edges) {
  cout << "MATRIX" << endl;

  GraphMatrix<int, utility::min<int>> g(n, -1, true);
  GraphMatrix<void> unwg(n, true);

  for (auto [a, b, c] : edges) {
    g.add_edge(a, b, c);
    unwg.add_edge(a, b);
  }

  cout << "WEIGHTED\n" << g << endl;
  for (auto [a, b, c] : g.get_edges()) {
    cout << a << " " << b << " " << c << endl;
  }
  cout << endl;

  cout << "UNWEIGHTED\n" << unwg << endl;
  for (auto [a, b] : unwg.get_edges()) {
    cout << a << " " << b << " " << endl;
  }
  cout <<endl;
}

void run_vector(int n, const vector<tuple<int, int, int>>& edges) {
  cout << "VECTOR" << endl;

  GraphVector<int> g(n);
  GraphVector<void> unwg(n);

  for (auto [a, b, c] : edges) {
    g.add_edge(a, b, c);
    unwg.add_edge(a, b);
  }

  cout << "WEIGHTED\n" << g << endl;
  for (auto [a, b, c] : g.get_edges()) {
    cout << a << " " << b << " " << c << endl;
  }
  cout << endl;

  cout << "UNWEIGHTED\n" << unwg << endl;
  for (auto [a, b] : unwg.get_edges()) {
    cout << a << " " << b << " " << endl;
  }
  cout <<endl;
}

int main() {
  int n, m;
  cin >> n >> m;

  vector<tuple<int, int, int>> edges;

  for(int i=0; i<m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    edges.emplace_back(a, b, c);
  }

  run_edgelist(n, edges);
  run_list(n, edges);
  run_matrix(n, edges);
  run_vector(n, edges);

  return 0;
}
