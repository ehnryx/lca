#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../bellman_ford.h"
#include "../bfs.h"
#else
#include "bellman_ford.h"
#include "bfs.h"
#endif

//using ll = long long;
constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for (int n, m, q, s; cin >> n >> m >> q >> s && n; ) {
    vector<tuple<int, int, int>> edges;
    vector<vector<int>> rev_graph(n);
    for (int i = 0; i < m; i++) {
      int a, b, c;
      cin >> a >> b >> c;
      edges.emplace_back(a, b, c);
      rev_graph[b].push_back(a);
    }
    const int inf = n * 2000 + 7;
    auto [dist, neg_cycle] = bellman_ford(edges, n, s, inf, true);
    vector<bool> negative(n);
    if (neg_cycle) {
      for (auto [a, b, c] : edges) {
        if (dist[a] < inf && dist[a] + c < dist[b]) {
          negative[b] = true;
        }
      }
    }
    for (int i = 0; i < q; i++) {
      int v;
      cin >> v;
      auto rev_dist = bfs(rev_graph, v);
      bool neg = false;
      for (int j = 0; j < n; j++) {
        if (rev_dist[j] != -1 && negative[j]) {
          neg = true;
          assert(neg_cycle);
          break;
        }
      }
      if (neg) {
        cout << "-Infinity" << nl;
      } else if (dist[v] == inf) {
        cout << "Impossible" << nl;
      } else {
        cout << dist[v] << nl;
      }
    }
    cout << nl;
  }

  return 0;
}
