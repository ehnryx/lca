#include <bits/stdc++.h>
using namespace std;
#define _USE_MATH_DEFINES

#include "../strongly_connected.h"

//#define FILENAME sadcactus

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

typedef long long ll;
typedef long double ld;
typedef complex<ld> pt;

constexpr char nl = '\n';
constexpr ll INF = 0x3f3f3f3f;
constexpr ll INFLL = 0x3f3f3f3f3f3f3f3f;
constexpr ll MOD = 998244353;
constexpr ld EPS = 1e-9L;
random_device _rd; mt19937 rng(_rd());

const int N = 2e5 + 1;
ll depth[N];
bool vis[N];
ll dfs(const vector<vector<pair<int,int>>>& adj, int u) {
  ll res = 0;
  for(auto [v, c] : adj[u]) {
    if(!vis[v]) {
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

// double-check correctness
// read limits carefully
// characterize valid solutions
int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(10);
#if defined(ONLINE_JUDGE) && defined(FILENAME)
  freopen(FILENAME ".in", "r", stdin);
  freopen(FILENAME ".out", "w", stdout);
#endif

  int n, m;
  cin >> n >> m;
  vector<vector<pair<int,int>>> adj(n + 1);
  vector<vector<int>> tarjan(n + 1);
  for(int i=0; i<m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    tarjan[a].push_back(b);
    adj[a].emplace_back(b, c);
  }
  strongly_connected scc(tarjan);

  vector<ll> unit(scc.size());
  vector<int> remap(n + 1);
  for(int i=0; i<(int)size(scc); i++) {
    if(size(scc.group[i]) == 1) continue;
    vector<vector<pair<int,int>>> comp(size(scc.group[i]));
    for(int j=0; j<(int)size(scc.group[i]); j++) {
      remap[scc.group[i][j]] = j;
    }
    for(int u : scc.group[i]) {
      for(auto [v, c] : adj[u]) {
        if(scc[v] != i) continue;
        comp[remap[u]].emplace_back(remap[v], c);
      }
    }
    fill(vis, vis + size(scc.group[i]), false);
    unit[i] = dfs(comp, 0);
  }

  int q;
  cin >> q;
  for(int i=0; i<q; i++) {
    int v, s, t;
    cin >> v >> s >> t;
    ll g = unit[scc[v]];
    if(g == 0) {
      if(s == 0) {
        cout << "YES" << nl;
      } else {
        cout << "NO" << nl;
      }
    } else {
      if(gcd(s, t) % gcd(g, t) == 0) {
        cout << "YES" << nl;
      } else {
        cout << "NO" << nl;
      }
    }
  }

  return 0;
}
