#include <bits/stdc++.h>
using namespace std;

#include "../segment_tree.h"

constexpr char nl = '\n';

int modpow(int n, int e, int m) {
  if(n == 0) return 0;
  int r = 1;
  for(; e; e/=2, n = n*n % m) {
    if(e&1) r = r*n % m;
  }
  return r;
}

int add(int a, int b, int m) {
  return a+b < m ? a+b : a+b - m;
}

struct Node {
  static int mod;
  static vector<int> identity;
  static vector<int> new_cnt;
  vector<int> cnt, lazy;
  Node(): cnt(mod), lazy(identity) {}
  Node(int v): cnt(mod), lazy(identity) {
    cnt[v % mod] = 1;
  }

  static pair<int,int> default_value() { return pair(0, 1); }
  static pair<int,int> merge(const pair<int,int>& a, const pair<int,int>& b) {
    return pair(add(a.first, b.first, mod), a.second * b.second % mod);
  }

  void put(const vector<int>& f) {
    fill(begin(new_cnt), end(new_cnt), 0);
    for(int j=0; j<mod; j++) {
      new_cnt[f[j]] += cnt[j];
      lazy[j] = f[lazy[j]];
    }
    cnt = new_cnt;
  }

  pair<int,int> get(int k) const {
    int sum = 0;
    int prod = 1;
    for(int j=0; j<mod; j++) {
      if(cnt[j] == 0) continue;
      sum = (sum + cnt[j] * modpow(j, k, mod)) % mod;
      prod = prod * modpow(j, cnt[j], mod) % mod;
    }
    return pair(sum, prod);
  }

  void pull(const Node& a, const Node& b) {
    for(int j=0; j<mod; j++) {
      cnt[j] = a.cnt[j] + b.cnt[j];
    }
  }

  void push(Node& a, Node& b) {
    if(lazy == identity) return;
    a.put(lazy);
    b.put(lazy);
    lazy = identity;
  }
};

int Node::mod;
vector<int> Node::identity;
vector<int> Node::new_cnt;

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

  int n, p;
  cin >> n >> p;

  Node::mod = p;
  Node::identity.resize(p);
  iota(begin(Node::identity), end(Node::identity), 0);
  Node::new_cnt.resize(p);

  vector<int> a(n + 1);
  for(int i=1; i<=n; i++) {
    cin >> a[i];
  }
  segment_tree<Node, pair<int,int>> st(a, 1);

  int m;
  cin >> m;
  for(int i=0; i<m; i++) {
    int t, l, r, k;
    cin >> t >> l >> r >> k;
    if(t == 1) {
      k %= p;
      vector<int> f(p);
      for(int j=0; j<p; j++) {
        f[j] = add(j, k, p);
      }
      st.update(l, r, f);
    } else if(t == 2) {
      k %= p;
      vector<int> f(p);
      for(int j=0; j<p; j++) {
        f[j] = j * k % p;
      }
      st.update(l, r, f);
    } else if(t == 3) {
      vector<int> f(p, 1);
      if(k > 0) {
        for(int j=0; j<p; j++) {
          f[j] = modpow(j, k, p);
        }
      }
      st.update(l, r, f);
    } else if(t == 4) {
      if(k == 0) {
        cout << (r - l + 1) % p << nl;
      } else {
        cout << st.query(l, r, k).first << nl;
      }
    } else { // t == 5
      cout << st.query(l, r, k).second << nl;
    }
  }

  return 0;
}
