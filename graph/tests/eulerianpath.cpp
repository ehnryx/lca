#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../euler_trail.h"
#else
#include "euler_trail.h"
#endif

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for(int n, m; cin >> n >> m && n; ) {
    vector<int> degree(n);
    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < m; i++) {
      int a, b;
      cin >> a >> b;
      //adj[a].emplace_back(b, i);
      adj[a].emplace_back(b, -1);
      degree[a]++;
      degree[b]--;
    }
    int source = max_element(begin(degree), end(degree)) - begin(degree);
    vector<int> trail = euler_trail(adj, m, source);
    if (empty(trail)) {
      cout << "Impossible" << nl;
    } else {
      for (int it : trail) {
        cout << it << " ";
      }
      cout << nl;
    }
  }

  return 0;
}
