#include <bits/stdc++.h>
using namespace std;

#include "../segment_tree.h"

struct Node {
  int value, cnt;
  Node() = default;
  static Node default_value() { return Node(); }
  static Node merge(const Node& a, const Node& b) { return Node().pull(a, b); }
  Node get() const { return *this; }
  void put(int v) { value = v; cnt = 1; }
  Node& pull(const Node& a, const Node& b) {
    value = gcd(a.value, b.value);
    cnt = (a.value == value) * a.cnt + (b.value == value) * b.cnt;
    return *this;
  }
};

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  segment_tree<Node, Node, false> st(n + 1);
  for(int i=1; i<=n; i++) {
    int a;
    cin >> a;
    st.update(i, i, a);
  }

  int m;
  cin >> m;
  for(int i=0; i<m; i++) {
    int l, r;
    cin >> l >> r;
    cout << (r-l+1) - st.query(l, r).cnt << nl;
  }

  return 0;
}
