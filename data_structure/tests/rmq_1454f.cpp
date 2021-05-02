#include <bits/stdc++.h>
using namespace std;

#include "../range_minimum_query.h"

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
    range_minimum_query rmq_min(a);
    range_minimum_query<int,greater<>> rmq_max(a);

    bool ok = false;
    for (int i = 1; !ok && i <= n - 2; i++) {
      int val = rmq_max.query(0, i);
      int l = i + 1;
      int r = n - 1;
      while(l + 1 < r) {
        int m = (l + r) / 2;
        if (rmq_min.query(i, m) > val) {
          l = m + 1;
        } else if (rmq_min.query(i, m) < val) {
          r = m - 1;
        } else if (rmq_max.query(m, n) < val) {
          r = m - 1;
        } else if (rmq_max.query(m, n) > val) {
          l = m + 1;
        } else {
          r = m;
        }
      }
      for (int j = l; j <= r; j++) {
        if (val == rmq_min.query(i, j) && val == rmq_max.query(j, n)) {
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
