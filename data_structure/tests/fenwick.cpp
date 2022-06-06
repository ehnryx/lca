#include <bits/stdc++.h>
using namespace std;

%:include "utility/defines.h"
%:include "utility/fast_input.h"
%:include "utility/output.h"
%:include "ds/fenwick_tree.h"

using ll = long long;
using ld = long double;

constexpr char nl = '\n';
constexpr int MOD = 998244353;
constexpr ld EPS = 1e-9L;
random_device _rd; mt19937 rng(_rd());


int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(10);
#ifdef USING_FAST_INPUT
  fast_input cin;
#endif

  int n, m;
  cin >> n >> m;
  fenwick_tree<ll> st(n);
  for(int i=0; i<m; i++) {
    char t;
    cin >> t;
    if(t == '+') {
      int x, v;
      cin >> x >> v;
      st.insert(x, v);
    } else {
      int x;
      cin >> x;
      cout << st.query(x - 1) << nl;
      /*
      static char buf[19];
      ll res = st.query(x - 1);
      bool neg = false;
      if(res < 0) {
        neg = true;
        res = -res;
      }
      int at = 0;
      while(res) {
        buf[at++] = res % 10 + '0';
        res /= 10;
      }
      if(neg) {
        putchar_unlocked('-');
      }
      if(at == 0) {
        putchar_unlocked('0');
      } else {
        while(at--) {
          putchar_unlocked(buf[at]);
        }
      }
      putchar_unlocked('\n');
      */
    }
  }

  return 0;
}
