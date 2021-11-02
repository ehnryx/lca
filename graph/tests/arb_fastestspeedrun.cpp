// nwerc 2018 f, cf102483f
#include <bits/stdc++.h>
using namespace std;

#include "../../misc/fast_input.h"
#include "../min_arborescence.h"

using ll = long long;
using ld = long double;
using pt = complex<ld>;

constexpr char nl = '\n';
constexpr int INF = 0x3f3f3f3f;
constexpr ll INFLL = 0x3f3f3f3f3f3f3f3f;
constexpr int MOD = 998244353;
constexpr ld EPS = 1e-9L;
random_device _rd; mt19937 rng(_rd());


int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(10);

  fast_input cin;

  int n;
  cin >> n;

  //min_arborescence<ll, less<>> arb(n + 1); // sparse
  min_arborescence<ll, less<>, false> arb(n + 1); // dense

  for(int i=1; i<=n; i++) {
    int x, s;
    cin >> x >> s;
    arb.add_edge(x, i, s);
    for(int j=0; j<=n; j++) {
      int v;
      cin >> v;
      arb.add_edge(j, i, v);
    }
  }

  arb.build();
  cout << arb.solve(0).second << nl;

  return 0;
}
