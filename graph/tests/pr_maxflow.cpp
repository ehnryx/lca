#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../push_relabel.h"
#else
#include "push_relabel.h"
#endif

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, m, s, t;
  cin >> n >> m >> s >> t;
  push_relabel<long long> graph(n);
  for(int i=0; i<m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    graph.add_edge(a, b, c);
  }
  int flow = graph.flow(s, t, true);
  vector<tuple<int,int,int>> edges;
  for(int i=0; i<n; i++) {
    for(const push_relabel<long long>::edge& e : graph.adj[i]) {
      if(e.flow > 0) {
        edges.emplace_back(i, e.to, e.flow);
      }
    }
  }
  cout << n << " " << flow << " " << size(edges) << nl;
  for(auto [a, b, c] : edges) {
    cout << a << " " << b << " " << c << nl;
  }

  return 0;
}
