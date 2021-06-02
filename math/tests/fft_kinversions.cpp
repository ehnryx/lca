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

int main() {
  cin.tie(0)->sync_with_stdio(0);

  string s;
  cin >> s;
  int n = (int)size(s);
  vector<int> a, b;
  for (char c : s) {
    a.push_back(c == 'A');
    b.push_back(c == 'B');
  }
  reverse(begin(a), end(a));

  vector<int> c = convolve<int, double>(a, b);
  for (int i=1; i<n; i++) {
    cout << c[n-1-i] << nl;
  }

  return 0;
}
