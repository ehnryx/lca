#include <bits/stdc++.h>
using namespace std;

#include "../suffix_array.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  string s, t;
  cin >> s >> t;
  suffix_array sa(s + "." + t);
  int ans = 0;
  for(int i=2; i<(int)size(sa.height); i++) {
    if(sa[i] < (int)size(s) && sa[i-1] < (int)size(s)) continue;
    if(sa[i] > (int)size(s) && sa[i-1] > (int)size(s)) continue;
    ans = max(ans, sa.height[i]);
  }
  cout << ans << nl;

  return 0;
}
