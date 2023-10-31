//#define NO_BIGGER_TYPE
%:include "geometry/point.h"
%:include "geometry/lines.h"

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

constexpr char nl = '\n';
constexpr int MOD = 998244353;
constexpr ld EPS = 1e-9L;
random_device _rd; mt19937 rng(_rd());

using pt = point<int>;

int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(2);
#ifdef USING_FAST_INPUT
  fast_input cin;
#endif

  int T;
  cin >> T;
  while(T--) {
    ld a, b, x, y;
    cin >> a >> b >> x >> y;
    pt A(a, b), B(x, y);
    cin >> a >> b >> x >> y;
    pt C(a, b), D(x, y);
    if constexpr (is_same_v<pt, point<int>>) {
      if(!seg_x_seg(A, B, C, D)) {
        cout << "none" << nl;
      } else if(!parallel(A, B, C, D)) {
        cout << line_inter(A, B, C, D) << nl;
      } else {
        if(A > B) swap(A, B);
        if(C > D) swap(C, D);
        pt X = max(A, C);
        pt Y = min(B, D);
        if(X == Y) {
          cout << point<double>(X) << nl;
        } else {
          cout << point<double>(X) << " " << point<double>(Y) << nl;
        }
      }
    } else {
      //if(!seg_x_seg(1e-9, A, B, C, D)) {
      //  cout << "none" << nl;
      //} else if(!parallel(1e-9, A, B, C, D)) {
      //  cout << line_inter(A, B, C, D) << nl;
      //} else {
      //  if(A > B) swap(A, B);
      //  if(C > D) swap(C, D);
      //  pt X = max(A, C);
      //  pt Y = min(B, D);
      //  if(geo::equal(1e-9, X, Y)) {
      //    cout << point<double>(X) << nl;
      //  } else {
      //    cout << point<double>(X) << " " << point<double>(Y) << nl;
      //  }
      //}
    }
  }

  if constexpr (is_same_v<pt, point<int>>) {
    static_assert(not pt::floating);

    static_assert(is_same_v<point<long long>::bigger_type, point<long long>::intersection_type>);
#ifdef NO_BIGGER_TYPE
    static_assert(is_same_v<pt::bigger_type, int>);
#else
    static_assert(is_same_v<pt::bigger_type, long long>);
#endif

    static_assert(is_same_v<pt::intersection_type, double>);
  }

  return 0;
}
