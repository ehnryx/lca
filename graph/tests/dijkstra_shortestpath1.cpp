#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../dijkstra.h"
#else
#include "dijkstra.h"
#endif

using ll = long long;
constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for (int n, m, q, s; cin >> n >> m >> q >> s && n; ) {
    vector<vector<pair<int, ll>>> graph(n);
    for (int i = 0; i < m; i++) {
      int a, b, c;
      cin >> a >> b >> c;
      graph[a].emplace_back(b, c);
    }
    auto dist = dijkstra(graph, s);
    for (int i = 0; i < q; i++) {
      int v;
      cin >> v;
      if (dist[v] == -1) {
        cout << "Impossible" << nl;
      } else {
        cout << dist[v] << nl;
      }
    }
    cout << nl;
  }

  return 0;
}
