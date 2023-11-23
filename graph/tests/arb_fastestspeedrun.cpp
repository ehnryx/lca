// nwerc 2018 f, cf102483f
// boj/16606

%:include "utility/fast_input.h"
%:include "graph/min_arborescence.h"

%:include "main.h"
SOLVE() {
  int n;
  cin >> n;

  //min_arborescence<ll, less<>> arb(n + 1); // sparse
  min_arborescence<ll, less<>, false> arb(n + 1);  // dense

  for (int i = 1; i <= n; i++) {
    int x, s;
    cin >> x >> s;
    arb.add_edge(x, i, s);
    for (int j = 0; j <= n; j++) {
      int v;
      cin >> v;
      arb.add_edge(j, i, v);
    }
  }

  arb.build();
  cout << arb.solve(0).second << nl;
}
