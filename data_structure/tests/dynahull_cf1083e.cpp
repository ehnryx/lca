#include <bits/stdc++.h>
using namespace std;

#include "../dynamic_hull.h"

constexpr char nl = '\n';
using ll = long long;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<tuple<int,int,ll>> rects;
  for(int i=0; i<n; i++) {
    int x, y; ll a;
    cin >> x >> y >> a;
    rects.emplace_back(x, y, a);
  }
  sort(begin(rects), end(rects));

  // dp[i] = xi*yi - ai + max_{j<i} (-xj) * yi + dp[j]
  vector<ll> dp(n);
  dynamic_hull<ll> hull;
  hull.insert(0, 0);
  for(int i=0; i<n; i++) {
    auto [x, y, a] = rects[i];
    dp[i] = (ll)x*y - a + hull.query(y);
    hull.insert(-x, dp[i]);
  }
  cout << *max_element(begin(dp), end(dp)) << nl;

  return 0;
}
