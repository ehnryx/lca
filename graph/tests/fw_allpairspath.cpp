#include <bits/stdc++.h>
using namespace std;

//%:include "utility/fast_input.h"
//%:include "utility/output.h"

%:include "graph/floyd_warshall.h"

using ll = long long;
using ld = long double;

constexpr char nl = '\n';
constexpr int MOD = 998244353;
constexpr ld EPS = 1e-9L;
random_device _rd; mt19937 rng(_rd());


struct smaller {
  static constexpr int value = numeric_limits<int>::min() / 2;
  int operator()(int a, int b) const {
    return max(value, min(a, b));
  }
};

//#define MULTI_TEST
void solve_main([[maybe_unused]] int testnum, [[maybe_unused]] auto& cin) {
  for(int n, m, q; cin >> n >> m >> q && n; ) {
    floyd_warshall<int, smaller> g(n, numeric_limits<int>::max());
    for(int i=0; i<m; i++) {
      int a, b, c;
      cin >> a >> b >> c;
      g.add_arc(a, b, c);
    }
    g.run();
    auto neg = g;
    neg.run();

    while(q--) {
      int a, b;
      cin >> a >> b;
      if(g[a][b] == numeric_limits<int>::max()) {
        cout << "Impossible" << nl;
      } else if(g[a][b] == neg[a][b] && g[a][b] != smaller::value) {
        cout << g[a][b] << nl;
      } else {
        cout << "-Infinity" << nl;
      }
    }
    cout << nl;
  }
}

int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(10);
#ifdef USING_FAST_INPUT
  fast_input cin;
#endif

  int T = 1;
#ifdef MULTI_TEST
  cin >> T;
#endif
  for(int testnum=1; testnum<=T; testnum++) {
    solve_main(testnum, cin);
  }

  return 0;
}
