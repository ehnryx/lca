#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../push_relabel.h"
#else
#include "push_relabel.h"
#endif

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, m, s, t;
  cin >> n >> m >> s >> t;
  push_relabel<long long> graph(n);
  for(int i=0; i<m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    graph.add_edge(a, b, c);
  }

  long long flow = graph.flow(s, t, true);
  vector<int> ans;
  for(int i=0; i<n; i++) {
    if (graph.left_of_min_cut(i)) {
      ans.push_back(i);
    }
  }
  cout << size(ans) << nl;
  for(int it : ans) {
    cout << it << nl;
  }

  return 0;
}
