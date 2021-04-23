#include <bits/stdc++.h>
using namespace std;

#include "../fraction.h"

constexpr char nl = '\n';

fraction<long long> read_fraction() {
  string s;
  cin >> s;
  if(s.find('/') != -1) {
    return fraction<long long>(
        stoll(s.substr(0, s.find('/'))),
        stoll(s.substr(s.find('/') + 1)));
  } else {
    return fraction<long long>(stoll(s));
  }
}

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int T;
  cin >> T;
  while(T--) {
    fraction<long long> a = read_fraction();
    fraction<long long> b = read_fraction();
    fraction<long long> ans = a + b;
    if(ans.denominator() == 1) {
      cout << ans.numerator() << nl;
    } else {
      cout << ans << nl;
    }
  }

  return 0;
}
