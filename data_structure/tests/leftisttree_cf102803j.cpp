#include <bits/stdc++.h>
using namespace std;

#include "../leftist_tree.h"

constexpr char nl = '\n';
using ll = long long;

struct Frac {
  int a, b;
  Frac(ll _a=0, ll _b=0): a(_a), b(_b) {}
  bool operator < (const Frac& o) const { return (ll)a*o.b < (ll)b*o.a; }
  bool operator <= (const Frac& o) const { return (ll)a*o.b <= (ll)b*o.a; }
  void operator += (const Frac& o) { a += o.a; b += o.b; }
};

const int N = 1e5 + 1;
vector<int> adj[N];
int a[N], b[N];

leftist_tree<Frac> solve(int u, ll& ans) {
  leftist_tree<Frac> subtree;
  for(int v : adj[u]) {
    leftist_tree<Frac> child = solve(v, ans);
    subtree.merge(child);
  }
  Frac cur(a[u], b[u]);
  while(!subtree.empty() && subtree.top() <= cur) {
    ans += (ll)cur.b * subtree.top().a;
    cur += subtree.top();
    subtree.pop();
  }
  subtree.push(cur);
  return subtree;
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

  int n;
  cin >> n;
  for(int i=2; i<=n; i++) {
    int parent;
    cin >> parent;
    adj[parent].push_back(i);
  }
  for(int i=1; i<=n; i++) {
    cin >> a[i] >> b[i];
  }

  ll ans = 0;
  leftist_tree<Frac> order = solve(1, ans);
  Frac cur(0, 0);
  while(!order.empty()) {
    ans += (ll)cur.b * order.top().a;
    cur += order.top();
    order.pop();
  }
  cout << ans << nl;

  return 0;
}
