#include <bits/stdc++.h>
using namespace std;

#include "../suffix_array_lcp.h"

const char nl = '\n';

// double-check correctness
// read limits carefully
// characterize valid solutions
int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(10);
#if defined(ONLINE_JUDGE) && defined(FILENAME)
  freopen(FILENAME ".in", "r", stdin);
  freopen(FILENAME ".out", "w", stdout);
#endif

  string s;
  cin >> s;
  int n = (int)size(s);
  suffix_array_lcp sa(s);

  for(int d=1; d<=n/3; d++) {
    int len1 = sa.lcp(0, d) / d * d;
    int b = d + len1;
    int len2 = sa.lcp(b, b+d) / d * d;
    int c = b+d + len2;
    int len3 = sa.lcp(c, c+d);
    if(len3 == n - (c+d)) {
      cout << s.substr(0, d) << " " << s.substr(b, d) << nl;
      return 0;
    }
  }
  assert(false);

  return 0;
}
