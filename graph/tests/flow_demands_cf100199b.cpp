#include <bits/stdc++.h>
using namespace std;

%:include "graph/dinic.h"
%:include "graph/flow_with_demands.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);
#ifdef ONLINE_JUDGE
  freopen("cooling.in", "r", stdin);
  freopen("cooling.out", "w", stdout);
#endif

  int n, m;
  cin >> n >> m;
  flow_with_demands<int> graph(n + 1);
  vector idx(n + 1, vector<int>(n + 1, -1));
  vector<int> ans(m);
  for(int i=0; i<m; i++) {
    int a, b, d, c;
    cin >> a >> b >> d >> c;
    graph.add_edge(a, b, d, c);
    idx[a][b] = i;
    ans[i] = d;
  }
  if (graph.flowable()) {
    cout << "YES" << nl;
    for(int i=1; i<=n; i++) {
      for(const auto& e : graph.adj[i]) {
        if(e.to > n || idx[i][e.to] == -1) continue;
        ans[idx[i][e.to]] += e.flow;
      }
    }
    for(int i=0; i<m; i++) {
      cout << ans[i] << nl;
    }
  } else {
    cout << "NO" << nl;
  }

  return 0;
}
