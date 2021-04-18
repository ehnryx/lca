#pragma GCC optimize("O3")
#pragma GCC optimize("fast-math")

#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../delaunay.h"
#else
#include "delaunay.h"
#endif

constexpr char nl = '\n';
using ld = double;
// eps = 1e-9 passed for ld = long double, but failed for ld = double
// eps = 1e-8 passed with 5/7 probability (with ld = double)
// eps = 2e-8 passed with 7/8 probability (with ld = double)
//using ld = long double;
using pt = point<ld>;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<point<ld>> v;
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    v.emplace_back(x, y);
  }
  auto triangles = delaunay(v, 2e-8);
  //auto triangles = delaunay(v);
  assert(clock() < CLOCKS_PER_SEC);

  ld ans = 0;
  //set<pt> seen;
  for (auto [ci, cj, ck] : triangles) {
    pt center = circumcenter(v[ci], v[cj], v[ck]);
    if (in_polygon(v, center)) {
      ans = max(ans, circumradius(v[ci], v[cj], v[ck]));
      continue;
    }
    for (pt end : {(v[ci]+v[cj])/2, (v[cj]+v[ck])/2, (v[ck]+v[ci])/2}) {
      for (int i = n-1, j = 0; j < n; i = j++) {
        if (!seg_x_seg(v[i], v[j], center, end)) continue;
        pt it = line_inter(v[i], v[j], center, end);
        //if (!seen.insert(it).second) continue;
        ld cur = 1e42;
        for (int k = 0; k < n; k++) {
          cur = min(cur, abs(it - v[k]));
        }
        ans = max(ans, cur);
      }
    }
  }
  cout << fixed << setprecision(9) << ans << nl;

  return 0;
}
