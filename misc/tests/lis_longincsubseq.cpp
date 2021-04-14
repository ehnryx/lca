#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../longest_inc_subseq.h"
#else
#include "longest_inc_subseq.h"
#endif

using ll = long long;
constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for (int n; cin >> n; ) {
    vector<ll> a(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    auto lis = longest_inc_subseq(a, (ll)1e11);
    for(int i=0; i<4; i++) longest_inc_subseq(a, (ll)1e11);
    cout << size(lis) << nl;
    for (int it : lis) {
      cout << it << " ";
    }
    cout << nl;
  }

  return 0;
}
