#include <bits/stdc++.h>
using namespace std;

//// deprecated
//#define USE_RATIONAL_POINTS

#ifdef HENRYX
#include "../convex_hull.h"
#else
#include "convex_hull.h"
#endif

constexpr char nl = '\n';
using pt = point<long long>;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for (int n; cin >> n && n; ) {
    vector<pt> v;
    for (int i = 0; i < n; i++) {
      int x, y;
      cin >> x >> y;
      v.emplace_back(x, y);
    }
    auto hull = convex_hull(v);
    cout << size(hull) << nl;
    for (auto [x, y] : hull) {
      cout << (int)x << " " << (int)y << nl;
    }
  }

  return 0;
}
