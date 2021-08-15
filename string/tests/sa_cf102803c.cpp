#include <bits/stdc++.h>
using namespace std;

#include "../suffix_array.h"

const char nl = '\n';

const int L = 20;
const int N = 8e5 + 1;
int rmq[L][N];

template <class T>
int query(const T& sa, int a, int b) {
  int l = min(sa.rank[a], sa.rank[b]) + 1;
  int r = max(sa.rank[a], sa.rank[b]);
  int j = 31 - __builtin_clz(r - l + 1);
  return min(rmq[j][l], rmq[j][r-(1<<j)+1]);
}

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
  suffix_array sa(s);

  for(int i=0; i<n; i++) {
    rmq[0][sa.rank[i]] = sa.height[sa.rank[i]];
  }
  for(int j=1; j<L; j++) {
    for(int i=0; i+(1<<j)<=n; i++) {
      rmq[j][i] = min(rmq[j-1][i], rmq[j-1][i+(1<<(j-1))]);
    }
  }

  for(int d=1; d<=n/3; d++) {
    int len1 = query(sa, 0, d) / d * d;
    int b = d + len1;
    int len2 = query(sa, b, b+d) / d * d;
    int c = b+d + len2;
    int len3 = query(sa, c, c+d);
    if(len3 == n - (c+d)) {
      cout << s.substr(0, d) << " " << s.substr(b, d) << nl;
      return 0;
    }
  }
  assert(false);

  return 0;
}
