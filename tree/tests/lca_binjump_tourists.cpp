#include <bits/stdc++.h>
using namespace std;

//#ifdef HENRYX
#include "../lca_binary_jumping.h"
//#else
//#include "lca_binary_jumping.h"
//#endif

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<vector<int>> adj(n + 1);
  for (int i = 1; i < n; i++) {
    int a, b;
    cin >> a >> b;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }
  lca_binary_jumping graph(adj, 1);
  long long ans = 0;
  for (int i = 1; i <= n; i++) {
    for (int j = 2*i; j <= n; j += i) {
      int lca_ij = graph.lca(i, j);
      ans += graph.depth[i] + graph.depth[j] - 2 * graph.depth[lca_ij] + 1;
    }
  }
  cout << ans << nl;

  return 0;
}
