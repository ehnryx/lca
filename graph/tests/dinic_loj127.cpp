#include <bits/stdc++.h>
using namespace std;

#include "../dinic.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, m, s, t;
  cin >> n >> m >> s >> t;
  dinic<int> graph(n);
  for(int i=0; i<m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    graph.add_edge(a-1, b-1, c);
  }
  cout << graph.flow(s-1, t-1) << nl;

  return 0;
}
