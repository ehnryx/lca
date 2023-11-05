%:include "graph/max_closure.h"
%:include "graph/dinic.h"
%:include "graph/push_relabel.h"

#include <bits/stdc++.h>
using namespace std;

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<int> weights(n);
  vector<pair<int,int>> edges;
  for(int i=0; i<n; i++) {
    int v, c, m;
    cin >> v >> c >> m;
    weights[i] = v - c;
    for(int j=0; j<m; j++) {
      int to;
      cin >> to;
      edges.emplace_back(to - 1, i);
    }
  }

  max_closure<dinic, int> dinic_graph(weights);
  max_closure<push_relabel, int> pr_graph(weights);
  for(auto [a, b] : edges) {
    dinic_graph.add_edge(a, b);
    pr_graph.add_edge(a, b);
  }
  vector<int> dinic_s = dinic_graph.solve();
  vector<int> pr_s = pr_graph.solve();
  //assert(dinic_s == pr_s);

  int ans = 0, other = 0;
  for(int i : dinic_s) {
    ans += weights[i];
  }
  for (int i : pr_s) {
    other += weights[i];
  }
  assert(ans == other);
  cout << ans << nl;
  //cout << other << nl;

  return 0;
}
