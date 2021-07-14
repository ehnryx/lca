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
  int operator () (char c) const { return c - 'a' + 1; }
  static const int alpha = 27; // 0 is the terminal character
  using T = char; // type of c
};

void dfs(const ukkonen<to_int>& st, int u) {
  cerr << "AT " << u << " :: " << st[u].left << " -- " << st[u].right << " w/ parent " << st[u].parent << " and suffix " << st[u].slink << endl;
  for(int c=0; c<to_int::alpha; c++) {
    if(st[u].to[c] == -1) continue;
    cerr << u << " :: " << c << " -> " << st[u].to[c] << endl;
    dfs(st, st[u].to[c]);
  }
}

int main() {
  cin.tie(0)->sync_with_stdio(0);

  string s;
  cin >> s;
  ukkonen<to_int> st(s);
  cerr<< "\n\nDFS" << endl;

  dfs(st, 0);

  return 0;
}
