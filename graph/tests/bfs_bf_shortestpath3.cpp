#include <bits/stdc++.h>
using namespace std;

#include "../bellman_ford.h"
#include "../breadth_first.h"

//using ll = long long;
constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for (int n, m, q, s; cin >> n >> m >> q >> s && n; ) {
    graph_list<int> graph(n);
    graph_list<void> rev_graph(n);
    for (int i = 0; i < m; i++) {
      int a, b, c;
      cin >> a >> b >> c;
      graph.add_edge(a, b, c);
      rev_graph.add_edge(b, a);
    }
    const int inf = n * 2000 + 7;
    bellman_ford bf(graph, s, inf, true);
    auto negative = bf.get_negatives();
    auto dist = bf.get_dists();
    /*auto [dist, neg_cycle] = bellman_ford(edges, n, s, inf, true);
    vector<bool> negative(n);
    if (neg_cycle) {
      for (auto [a, b, c] : edges) {
        if (dist[a] < inf && dist[a] + c < dist[b]) {
          negative[b] = true;
        }
      }
    }*/
    for (int i = 0; i < q; i++) {
      int v;
      cin >> v;
      breadth_first bfs(rev_graph, v);
      auto rev_dist = bfs.get_dists();
      bool neg = false;
      for (int j = 0; j < n; j++) {
        if (rev_dist[j] != -1 && negative[j]) {
          neg = true;
          assert(bf.has_negative_cycle());
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
