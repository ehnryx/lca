#include <bits/stdc++.h>
using namespace std;

//%:include "utility/defines.h"
//%:include "utility/fast_input.h"
//%:include "utility/output.h"
%:include "geometry/point.h"
%:include "geometry/lines.h"

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
    if(!seg_x_seg(A, B, C, D)) {
      cout << "none" << nl;
    } else if(!parallel(A, B, C, D)) {
      cout << line_inter(A, B, C, D) << nl;
    } else {
      if(A > B) swap(A, B);
      if(C > D) swap(C, D);
      pt X = max(A, C);
      pt Y = min(B, D);
      if(equal(X, Y)) {
        cout << point<double>(X) << nl;
      } else {
        cout << point<double>(X) << " " << point<double>(Y) << nl;
      }
    }
  }

  return 0;
}
