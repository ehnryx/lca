#pragma GCC optimize("O3,fast-math")
#pragma GCC target("sse4,avx2")

#include <bits/stdc++.h>
using namespace std;
#define _USE_MATH_DEFINES

#ifdef HENRYX
#include "../fast_fourier_transform.h"
#else
#include "fast_fourier_transform.h"
#endif

constexpr char nl = '\n';
using ll = long long;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    vector<ll> a(n + 1);
    for (int i = 0; i <= n; i++) {
      cin >> a[i];
    }
    int m;
    cin >> m;
    vector<ll> b(m + 1);
    for (int i = 0; i <= m; i++) {
      cin >> b[i];
    }
    vector<ll> c = convolve(a, b);
    cout << n + m << nl;
    for (int i = 0; i <= n + m; i++) {
      cout << c[i] << " ";
    }
    cout << nl;
  }

  return 0;
}
