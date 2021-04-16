#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../convex_hull.h"
#else
#include "convex_hull.h"
#endif

constexpr char nl = '\n';
using pt = point<>;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<pt> v;
  for (int i = 0; i < n; i++) {
    int x, y; char c;
    cin >> x >> y >> c;
    if (c == 'Y') {
      v.emplace_back(x, y);
    }
  }
  auto hull = convex_hull(v, true);
  assert(size(hull) == size(v));
  cout << size(hull) << nl;
  for (auto [x, y] : hull) {
    cout << (int)x << " " << (int)y << nl;
  }

  return 0;
}
