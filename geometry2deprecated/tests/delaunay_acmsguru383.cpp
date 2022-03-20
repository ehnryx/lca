#pragma GCC optimize("O3")
#pragma GCC optimize("fast-math")

#include <bits/stdc++.h>
using namespace std;

#include "../delaunay.h"

constexpr char nl = '\n';
using ld = long double;
//using ld = double; // I can't get this to pass with doubles

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<point<ld>> v;
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    v.emplace_back(x, y);
  }
  auto triangles = delaunay<ld>(v, 1e-9);

  vector<int> dsu(n, -1);
  function<int(int)> find = [&](int i) {
    if (dsu[i] == -1) return i;
    return dsu[i] = find(dsu[i]);
  };

  vector<tuple<ld, int, int>> edges;
  for (auto [i, j, k] : triangles) {
    edges.emplace_back(abs(v[i] - v[j]), i, j);
    edges.emplace_back(abs(v[j] - v[k]), j, k);
    edges.emplace_back(abs(v[k] - v[i]), k, i);
  }
  sort(begin(edges), end(edges));

  vector<vector<pair<int, ld>>> adj(n);
  for (auto [c, i, j] : edges) {
    if (find(i) == find(j)) continue;
    adj[i].emplace_back(j, c);
    adj[j].emplace_back(i, c);
    dsu[find(i)] = find(j);
  }

  const int L = 32 - __builtin_clz(n);
  vector<int> depth(n);
  vector jump(n, vector<pair<int, ld>>(L));
  function<void(int, int)> build = [&](int u, int p) {
    for (auto [w, c] : adj[u]) {
      if (w == p) continue;
      depth[w] = depth[u] + 1;
      jump[w][0] = pair(u, c);
      for (int j = 1; j < L && jump[w][j-1].first != -1; j++) {
        int mid = jump[w][j-1].first;
        jump[w][j] = pair(jump[mid][j-1].first,
            max(jump[w][j-1].second, jump[mid][j-1].second));
      }
      build(w, u);
    }
  };
  build(0, -1);

  function<ld(int, int)> query = [=](int a, int b) {
    ld res = 0;
    if (depth[a] < depth[b]) swap(a, b);
    for (int j = L-1; j >= 0; j--) {
      if (depth[a] - (1 << j) >= depth[b]) {
        res = max(res, jump[a][j].second);
        a = jump[a][j].first;
      }
    }
    if (a == b) return res;
    for (int j = L-1; j >= 0; j--) {
      if (jump[a][j].first != jump[b][j].first) {
        res = max(res, max(jump[a][j].second, jump[b][j].second));
        a = jump[a][j].first;
        b = jump[b][j].first;
      }
    }
    return max(res, max(jump[a][0].second, jump[b][0].second));
  };

  int m;
  cin >> m;
  cout << fixed << setprecision(9);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    cout << query(--a, --b) << nl;
  }

  return 0;
}
