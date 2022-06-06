#include <bits/stdc++.h>
using namespace std;

#include "../range_minimum_query.h"

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, q;
  cin >> n >> q;
  vector<int> a(n);
  for(int i=0; i<n; i++) {
    cin >> a[i];
  }

  range_minimum_query rmq(a);
  for(int i=0; i<q; i++) {
    int l, r;
    cin >> l >> r;
    cout << rmq.query(l, r - 1) << '\n';
  }

  return 0;
}
