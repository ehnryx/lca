#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../ukkonen.h"
#else
#include "ukkonen.h"
#endif

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

constexpr char nl = '\n';

struct to_int {
  static int toi(char c) { return c - 'a' + 1; }
  static const int alpha = 27;
  using T = char;
};

ordered_set<int> dfs(
    int slen,
    const vector<vector<pair<int, int>>>& put,
    const ukkonen<to_int>& st, int u, vector<int>& ans) {
  ordered_set<int> cur;
  for(int c = 0; c < to_int::alpha; c++) {
    int v = (int)st[u].to[c];
    if(v == -1) continue;
    ordered_set<int> child = dfs(slen, put, st, v, ans);
    if (size(cur) < size(child)) {
      cur.swap(child);
    }
    for (int it : child) {
      cur.insert(it);
    }
  }
  if (st.is_leaf(u)) {
    cur.insert(slen - (st.depth(u) + st.length(u)) + 1);
  }
  for (auto [i, it] : put[u]) {
    if (it <= (int)size(cur)) {
      ans[i] = *cur.find_by_order(it - 1);
    }
  }
  return cur;
}

int main() {
  cin.tie(0)->sync_with_stdio(0);

  string s;
  cin >> s;
  int n = (int)s.size();
  ukkonen<to_int> st(s);

  int m;
  cin >> m;
  vector<vector<pair<int, int>>> put(size(st.nodes));
  for(int i=0; i<m; i++) {
    int k;
    cin >> s >> k;
    int node = st.match(s).first;
    if (node != -1) {
      put[node].emplace_back(i, k);
    }
  }

  vector<int> ans(m, -1);
  dfs(n + 1, put, st, st.root(), ans);
  for(int i=0; i<m; i++) {
    cout << ans[i] << nl;
  }

  return 0;
}
