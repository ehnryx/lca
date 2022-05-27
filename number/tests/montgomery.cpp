#include <bits/stdc++.h>
using namespace std;

//%:include "utility/fast_input.h"
#include "../montgomery_int.h"

using ll = long long;
using ld = long double;
using pt = complex<ld>;

constexpr char nl = '\n';
constexpr int MOD = 998244353;
constexpr ld EPS = 1e-9L;
random_device _rd; mt19937 rng(_rd());

using Int = montgomery_int<MOD>;

int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(10);
#ifdef USING_FAST_INPUT
  fast_input cin;
#endif

  static_assert(Int::r_inv == 232013824);
  static_assert(Int::mod_inv == 3296722945);

  return 0;
}
