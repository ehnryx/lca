#include <bits/stdc++.h>
using namespace std;

#include "../hungarian.h"
#include "../../number/float_with_eps.h"

// using Float = float_with_eps<long double, 1e-11L>;
// doesn't work ???

using Float = long double;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<complex<long double>> a, b;
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    a.emplace_back(x, y);
  }
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    b.emplace_back(x, y);
  }

  hungarian<Float> graph(n, n);
  static constexpr long double big = 1e5;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      graph.add_weight(i, j, big - abs(a[i] - b[j]));
    }
  }

  vector<int> res;
  cerr << "weight: " << Float(n) * 1e5 - graph.run() << '\n';
  for(int i=0; i<n; i++) {
    cout << i + 1 << " " << graph.match[i] + 1 << '\n';
  }

  return 0;
}
