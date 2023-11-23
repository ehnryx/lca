// boj/15023

#include <bits/stdc++.h>
using namespace std;
#define _USE_MATH_DEFINES

%:include "graph/dinic.h"

typedef double ld;
typedef int ll;

#define nl '\n'
const ld EPS = 1e-13;

// MAGIC IO
inline char get(void) {
  static char buf[100000], *S = buf, *T = buf;
  if (S == T) {
    T = (S = buf) + fread(buf, 1, 100000, stdin);
    if (S == T) return EOF;
  }
  return *S++;
}
inline void read(int &x) {
  static char c; x = 0; int sgn = 0;
  for (c = get(); c < '0' || c > '9'; c = get()) if (c == '-') sgn = 1;
  for (; c >= '0' && c <= '9'; c = get()) x = x * 10 + c - '0';
  if (sgn) x = -x;
}
void readchar(char& c) {
  while (isspace(c = get()));
}
// END MAGIC IO

int main() {
  ios::sync_with_stdio(0); cin.tie(0);
  cout << fixed << setprecision(10);

  int n,m;
  //cin>>n>>m;
  read(n);read(m);
  vector<int> a(n),b(n),c(m);
  for(int i=0;i<n;i++) {
    //cin>>a[i]>>b[i];
    read(a[i]);read(b[i]);
  }
  vector<pair<ld,int>> line;
  for(int i=0;i<m;i++) {
    int x,y;
    //cin>>x>>y>>c;
    read(x);read(y);read(c[i]);
    line.push_back(make_pair(atan2(y,x),i));
  }
  sort(line.begin(),line.end());

  const int S = n+m;
  const int T = S+1;
  dinic<int> graph(T + 1);
  for(int i=0;i<n;i++) {
    ld ang = atan2(b[i],a[i]);
    auto it = lower_bound(line.begin(),line.end(),make_pair(ang,-1));
    const auto& nxt = (it==line.end() ? line[0] : *it);
    const auto& pre = (it==line.begin() ? line.back() : *prev(it));
    ld to_nxt = abs(nxt.first-ang);
    if(to_nxt > M_PI) to_nxt = 2*M_PI - to_nxt;
    ld to_pre = abs(pre.first-ang);
    if(to_pre > M_PI) to_pre = 2*M_PI - to_pre;
    if(abs(to_nxt - to_pre) < EPS) {
      graph.add_edge(i, nxt.second+n, 1);
      graph.add_edge(i, pre.second+n, 1);
      if(c[nxt.second]-->0) graph.add_edge(nxt.second+n, T, 1);
      if(c[pre.second]-->0) graph.add_edge(pre.second+n, T, 1);
    } else if(to_nxt < to_pre) {
      graph.add_edge(i, nxt.second+n, 1);
      if(c[nxt.second]-->0) graph.add_edge(nxt.second+n, T, 1);
    } else {
      graph.add_edge(i, pre.second+n, 1);
      if(c[pre.second]-->0) graph.add_edge(pre.second+n, T, 1);
    }
    graph.add_edge(S, i, 1);
  }

  cout<<graph.flow(S,T)<<nl;
  for(int i=0;i<n;i++) {
    for(const dinic<int>::edge& e:graph.adj[i]) {
      if(e.flow>0) {
        cout<<i<<" "<<e.to-n<<nl;
      }
    }
  }

  return 0;
}
