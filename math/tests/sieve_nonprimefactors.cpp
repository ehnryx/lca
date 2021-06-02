#pragma GCC optimize("O3")
#pragma GCC target("sse4,avx2")
#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../linear_sieve.h"
#else
#include "linear_sieve.h"
#endif

constexpr char nl = '\n';

/*
struct nonprimefactors {
  using T = pair<int,int>;
  static constexpr bool store_powers = true;
  static T one() { return pair(1, 0); }
  static T coprime(T a, T b) { return pair(a.first * b.first, a.second + b.second); }
  static T prime(int) { return pair(2, 1); }
  static T prime_power(T, int, int k) { return pair(k + 1, 1); }
};
*/

struct nonprimefactors {
  using T = pair<int,int>;
  static constexpr bool store_powers = false;
  static T one() { return pair(1, 0); }
  static T coprime(T a, T b) { return pair(a.first * b.first, a.second + b.second); }
  static T prime(int) { return pair(2, 1); }
  static T noncoprime(T a, int, int k) { return pair(a.first / k * (k + 1), a.second); }
};

int main() {
  cin.tie(0)->sync_with_stdio(0);

  const int N = 2e6 + 1;
  linear_sieve<nonprimefactors, nonprimefactors::store_powers> npf(N);

  int T;
  cin >> T;
  while(T--) {
    int n;
    cin >> n;
    cout << npf[n].first - npf[n].second << nl;
  }

  return 0;
}
