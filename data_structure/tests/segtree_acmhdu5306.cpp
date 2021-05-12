// http://acm.hdu.edu.cn/showproblem.php?pid=5306
// can't compile as is :/
#include <bits/stdc++.h>
using namespace std;

#include "../segment_tree.h"

struct Node {
  using T = pair<long long, int>;
  static constexpr int inf = numeric_limits<int>::max();
  long long sum;
  int maxv, max2, lazy, maxcnt;
  Node(): sum(0), maxv(-1), max2(-1), lazy(inf), maxcnt(0) {}
  Node(int v): sum(v), maxv(v), max2(-1), lazy(inf), maxcnt(1) {}

  void put(int v) {
    assert(v > max2);
    if (v < maxv) {
      sum += (long long)maxcnt * (v - maxv);
      maxv = v;
      lazy = v;
    }
  }
  T get() const { return pair(sum, maxv); }

  void pull(const Node& a, const Node& b) {
    sum = a.sum + b.sum;
    maxv = max(a.maxv, b.maxv);
    if (a.maxv == b.maxv) {
      max2 = max(a.max2, b.max2);
      maxcnt = a.maxcnt + b.maxcnt;
    } else if (a.maxv > b.maxv) {
      max2 = max(a.max2, b.maxv);
      maxcnt = a.maxcnt;
    } else {
      max2 = max(a.maxv, b.max2);
      maxcnt = b.maxcnt;
    }
  }
  void push(Node& a, Node& b) {
    if (lazy < inf) {
      a.put(lazy);
      b.put(lazy);
      lazy = inf;
    }
  }

  bool put_condition(int v) const { return v > max2; }
  static T default_value() { return pair(0, -1); }
  static T merge(const T& a, const T& b) {
    return pair(a.first + b.first, max(a.second, b.second));
  }
};

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int T;
  cin >> T;
  while(T--) {
    int n, m;
    cin >> n >> m;
    vector<int> a(n+1);
    for(int i=1; i<=n; i++) {
      cin >> a[i];
    }
    segment_tree<Node, pair<long long, int>, true, true> st(a);
    for(int i=0; i<m; i++) {
      int t;
      cin >> t;
      if(t == 0) {
        int x, y, v;
        cin >> x >> y >> v;
        st.update(x, y, v);
      } else if(t == 1) {
        int x, y;
        cin >> x >> y;
        cout << st.query(x, y).second << nl;
      } else {
        int x, y;
        cin >> x >> y;
        cout << st.query(x, y).first << nl;
      }
    }
  }

  return 0;
}
