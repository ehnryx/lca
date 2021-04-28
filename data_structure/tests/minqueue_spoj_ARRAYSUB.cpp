#include <bits/stdc++.h>
using namespace std;

#include "../min_queue.h"

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<int> a(n);
  for(int i=0; i<n; i++) {
    cin >> a[i];
  }

  int m;
  cin >> m;
  min_queue<int, greater<>> ds;
  for(int i=0; i<m; i++) {
    ds.push(a[i]);
  }
  cout << ds.min();
  for(int i=m; i<n; i++) {
    ds.pop();
    ds.push(a[i]);
    cout << " " << ds.min();
  }
  cout << '\n';

  return 0;
}
