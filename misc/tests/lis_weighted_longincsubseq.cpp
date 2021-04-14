#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../lis_weighted.h"
#else
#include "lis_weighted.h"
#endif

using ll = long long;
constexpr char nl = '\n';
random_device _rd; mt19937 rng(_rd());

const int M = 1e9;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for (int n; cin >> n; ) {
    vector<pair<int,ll>> a(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i].first;
      a[i].second = (ll)(M+7)*n + rng() % M;
    }
    auto lis = lis_weighted(a);
    cout << size(lis) << nl;
    for (int it : lis) {
      cout << it << " ";
    }
    cout << nl;
  }

  return 0;
}
