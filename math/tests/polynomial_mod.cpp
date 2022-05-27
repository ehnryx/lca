#include <bits/stdc++.h>
using namespace std;

//%:include "utility/fast_input.h"
#include "../polynomial_mod.h"

using ll = long long;
using ld = long double;
using pt = complex<ld>;

constexpr char nl = '\n';
constexpr int MOD = 998244353;
constexpr ld EPS = 1e-9L;
random_device _rd; mt19937 rng(_rd());

using P = polynomial_mod<998244353>;

int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(10);
#ifdef USING_FAST_INPUT
  fast_input cin;
#endif

  P a({ 1, -3, 2 });
  P b;
  P c({ 3, 1});
  cout << a << nl;
  cout << b << nl;
  cout << a + b << nl;
  cout << a - b << nl;
  cout << -a << nl;
  cout << a << nl;
  cout << a * b << nl;
  cout << a * c << nl;

  auto [q, r] = a.divide_remainder(c);
  cout << q << nl;
  cout << r << nl;

  return 0;
}
