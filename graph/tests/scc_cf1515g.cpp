%:include "graph/strongly_connected.h"

%:include "main.h"

const int N = (int)2e5 + 1;
ll depth[N];
bool vis[N];
ll dfs(const vector<vector<pair<int, int>>>& adj, int u) {
  ll res = 0;
  for (auto [v, c] : adj[u]) {
    if (!vis[v]) {
      vis[v] = true;
      depth[v] = depth[u] + c;
      res = gcd(res, dfs(adj, v));
    } else {
      ll cyc = depth[u] + c - depth[v];
      res = gcd(res, cyc);
    }
  }
  return res;
}

SOLVE() {
  int n, m;
  cin >> n >> m;
  vector<vector<pair<int, int>>> adj(n + 1);
  graph_t tarjan(n + 1);
  for (int i = 0; i < m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    tarjan.add_arc(a, b);
    adj[a].emplace_back(b, c);
  }
  strongly_connected<scc_traits::BUILD_DAG | scc_traits::DEDUP_EDGES> scc(tarjan);

  vector<ll> unit(scc.size());
  vector<int> remap(n + 1);
  for (int i = 0; i < (int)size(scc); i++) {
    if (size(scc.group[i]) == 1) continue;
    vector<vector<pair<int, int>>> comp(size(scc.group[i]));
    for (int j = 0; j < (int)size(scc.group[i]); j++) {
      remap[scc.group[i][j]] = j;
    }
    for (int u : scc.group[i]) {
      for (auto [v, c] : adj[u]) {
        if (scc[v] != i) continue;
        comp[remap[u]].emplace_back(remap[v], c);
      }
    }
    fill(vis, vis + size(scc.group[i]), false);
    unit[i] = dfs(comp, 0);
  }

  int q;
  cin >> q;
  for (int i = 0; i < q; i++) {
    int v, s, t;
    cin >> v >> s >> t;
    ll g = unit[scc[v]];
    if (g == 0) {
      if (s == 0) {
        cout << "YES" << nl;
      } else {
        cout << "NO" << nl;
      }
    } else {
      if (gcd(s, t) % gcd(g, t) == 0) {
        cout << "YES" << nl;
      } else {
        cout << "NO" << nl;
      }
    }
  }
}
