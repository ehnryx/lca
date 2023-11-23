#include <bits/stdc++.h>
using namespace std;

%:include "data_structure/range_minimum_query.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    auto rmq_min = make_rmq(a);
    auto rmq_max = make_rmq<greater<>>(a);

    bool ok = false;
    for (int i = 1; !ok && i <= n - 2; i++) {
      int val = rmq_max.query(0, i - 1);
      int l = i + 1;
      int r = n - 1;
      while(l + 1 < r) {
        int m = (l + r) / 2;
        if (rmq_min.query(i, m - 1) > val) {
          l = m + 1;
        } else if (rmq_min.query(i, m - 1) < val) {
          r = m - 1;
        } else if (rmq_max.query(m, n - 1) < val) {
          r = m - 1;
        } else if (rmq_max.query(m, n - 1) > val) {
          l = m + 1;
        } else {
          r = m;
        }
      }
      for (int j = l; j <= r; j++) {
        if (val == rmq_min.query(i, j - 1) && val == rmq_max.query(j, n - 1)) {
          ok = true;
          cout << "YES" << nl;
          cout << i << " " << j - i << " " << n - j << nl;
          break;
        }
      }
    }
    if(!ok) {
      cout << "NO" << nl;
    }
  }

  return 0;
}
