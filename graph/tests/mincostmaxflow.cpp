#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../min_cost_flow.h"
#else
#include "min_cost_flow.h"
#endif

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, m, s, t;
  cin >> n >> m >> s >> t;
  min_cost_flow<int> graph(n);
  //min_cost_flow<int, false> dense(n);
  for (int i = 0; i < m; i++) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    graph.add_edge(a, b, c, d);
    //dense.add_edge(a, b, c, d);
  }
  auto [f, c] = graph.flow(s, t);
  cout << f << " " << c << nl;

  //auto [fd, cd] = dense.flow(s, t);
  //cout << fd << " " << cd << nl;
  //assert(f == fd);
  //assert(c == cd);

  return 0;
}
