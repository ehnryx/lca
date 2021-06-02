#include <bits/stdc++.h>
using namespace std;

#include "../suffix_array_lcp.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int T;
  cin >> T;
  while(T--) {
    string s;
    cin >> s;
    int n = s.size();
    suffix_array_lcp sa(s);
    long long ans = 0;
    int cur = 0;
    for(int i=1; i<n; i++) {
      int nxt = sa.lcp(sa[i], sa[i+1]);
      if(nxt > cur) ans += nxt - cur;
      cur = nxt;
    }
    cout << ans << nl;
  }

  return 0;
}
