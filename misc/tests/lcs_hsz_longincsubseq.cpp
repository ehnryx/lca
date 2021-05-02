#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../lcs_hunt_szymanski.h"
#else
#include "lcs_hunt_szymanski.h"
#endif

using ll = long long;
constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for (int n; cin >> n; ) {
    vector<ll> a(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    vector<ll> order = a;
    sort(begin(order), end(order));
    order.resize(unique(begin(order), end(order)) - begin(order));
    auto lcs = lcs_hunt_szymanski(a, order);
    for (int i=0; i<4; i++) lcs_hunt_szymanski(a, order);

    vector<int> lis;
    for(int i=0, s=0; i<(int)size(lcs); i++) {
      auto it = find(begin(a) + s, end(a), lcs[i]);
      lis.push_back(s = (int)(it - begin(a)));
    }

    cout << size(lis) << nl;
    for (int it : lis) {
      cout << it << " ";
    }
    cout << nl;
  }

  return 0;
}
