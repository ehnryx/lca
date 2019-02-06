#include <bits/stdc++.h>
using namespace std;

typedef pair<int,int> pii;

const int N = 1e5+1;
vector<int> adj[N];

void UPDATE(int a, int b, int c, int d, int e) {}

namespace HLD {
  const int L = 17; const int N = 1<<L; // values for 1e5

  int lg[2*N]; pii rmq[2*N][L+1]; // no RMQ if only vertical paths
  void build_rmq(int n) { for (int i=1; i<n; i++) lg[i] = 31-__builtin_clz(i);
    for (int j=1; j<=L; j++) for (int i=0; i+(1<<j)-1<n; i++)
      rmq[i][j] = min(rmq[i][j-1], rmq[i+(1<<(j-1))][j-1]); }
  int query_rmq(int l, int r) { if (l>r) swap(l,r); int j = lg[r-l+1];
    return min(rmq[l][j], rmq[r-(1<<j)+1][j]).second; }

  int d[N], sp[N], par[N], sub[N]; int lcan;
  int build_lca(int u, int p) {
    d[u] = d[par[u]=p]+1; sub[u] = 1; rmq[sp[u]=lcan++][0] = pii(d[u], u);
    for (int x:adj[u]) if (x!=p) { sub[u] += build_lca(x,u);
      rmq[lcan++][0] = pii(d[u], u); } return sub[u]; }
  int query_lca(int a, int b) { return query_rmq(sp[a], sp[b]); }

  int sz[N], root[N], ch[N], pos[N]; int dn, cn;
  void build_hld(int u, int p) { if (dn == cn) { root[cn]=u; sz[cn++]=0; }
    pos[u] = sz[ch[u]=dn]++; int b=-1, c=-1;
    for (int x:adj[u]) if (x!=p && sub[x]>b) { b = sub[c=x]; }
    if (c!=-1) build_hld(c,u);
    for (int y:adj[u]) if (y!=p && y!=c) { dn++; build_hld(y,u); } }

  int off[N], len[N];
  int length(int n) { return 1<<(32-__builtin_clz(n)); }
  void build(int r, int n=N) { d[0] = lcan = dn = cn = off[0] = 0;
    build_lca(r,0); build_rmq(2*n); build_hld(r,0);
    for (int i=0; i<cn; i++) len[i] = length(sz[i]);
    for (int i=1; i<cn; i++) off[i] = off[i-1] + 2*len[i-1]; }

  // ASSUME b IS ANCESTOR OF a, modify for insert_node, query_path, query_node
  void insert_path(int a, int b, int v) {
    while (ch[a] != ch[b]) {
      UPDATE(off[ch[a]], len[ch[a]], 0, pos[a], v); // call segtree
      a = par[root[ch[a]]];
    }
    if (pos[a] != pos[b]) {
      UPDATE(off[ch[a]], len[ch[a]], pos[b]+1, pos[a], v); // call segree
    }
  }
}

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
