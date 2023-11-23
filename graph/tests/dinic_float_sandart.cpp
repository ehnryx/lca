// boj/11226

#include <bits/stdc++.h>
using namespace std;

%:include "graph/dinic.h"

typedef long double ld;

const char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, m, w, h;
  cin >> n >> m >> w >> h;

  vector<ld> v(m);
  for(int i=0; i<m; i++) {
    cin >> v[i];
  }

  vector<ld> len(n);
  ld pre = 0;
  for(int i=0; i<n-1; i++) {
    ld x;
    cin >> x;
    len[i] = x - pre;
    pre = x;
  }
  len[n-1] = w - pre;

  vector<ld> done(n);
  vector minv(n, vector<ld>(m));
  ld minh = 1e9;
  ld maxh = 0;
  for(int i=0; i<n; i++) {
    ld cur = 0;
    for(int j=0; j<m; j++) {
      cin >> minv[i][j];
      cur += minv[i][j];
      v[j] -= minv[i][j];
    }
    minh = min(minh, cur/len[i]);
    maxh = max(maxh, cur/len[i]);
    done[i] = cur;
  }

  vector maxv(n, vector<ld>(m));
  for(int i=0; i<n; i++) {
    for(int j=0; j<m; j++) {
      cin >> maxv[i][j];
    }
  }

  const int S = n+m;
  const int T = S+1;

  ld left = minh;
  ld right = maxh;
  const ld BS = 1e-5;
  while(right - left > BS) {
    ld mid = (left+right)/2;
    dinic<ld> graph(T + 1);
    ld goal = 0;
    for(int i=0; i<n; i++) {
      graph.add_edge(S, i, len[i] * max((ld)0, mid - done[i]/len[i]));
      goal += len[i]*max((ld)0, mid - done[i]/len[i]);
    }
    for(int i=0; i<m; i++) {
      graph.add_edge(i+n, T, v[i]);
    }
    for(int i=0; i<n; i++) {
      for(int j=0; j<m; j++) {
        graph.add_edge(i, j+n, maxv[i][j] - minv[i][j]);
      }
    }
    static const ld EPS = 1e-9;
    if(abs(graph.flow(S, T, 42) - goal) < 2*EPS) {
      left = mid;
    } else {
      right = mid;
    }
  }
  cout << fixed << setprecision(3) << maxh - left << nl;

  return 0;
}
