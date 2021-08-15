//#pragma GCC optimize("O3,fast-math")
//#pragma GCC target("sse4,avx2")

#include <bits/stdc++.h>
using namespace std;
#define _USE_MATH_DEFINES

#include "../number_theoretic_transform.h"
#include "../../number/mod_int.h"

constexpr char nl = '\n';
using Int = mod_int<998244353>;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  string s;
  cin >> s;
  int n = (int)size(s);
  //vector<Int> a, b;
  vector<long long> a, b;
  for (char c : s) {
    a.push_back(c == 'A');
    b.push_back(c == 'B');
  }
  reverse(begin(a), end(a));

  auto c = convolve<998244353>(a, b);
  for (int i=1; i<n; i++) {
    cout << c[n-1-i] << nl;
  }

  return 0;
}
