#include <bits/stdc++.h>
using namespace std;

typedef pair<int,int> pii;

// values for 1e5
const int L = 17;
const int N = 1<<L;

// FOR QUERIES ON TREES: 
// General data structure to store the tree
vector<int> adj[N];

// EXAMPLE SegTree
struct SegTree {
  int n;
  int *segt;
  SegTree(int len) {
    n = 1 << (32-__builtin_clz(max(1,len-1)));
    segt = new int[2*n];
    fill(segt, segt+2*n, 0); // remember to initialize
  }
  void update(int l, int r, int v) { /* ... */  }
};
// END EXAMPLE

template<class T> struct RMQ {
  T rmq[2*N][L+1]; int lg[2*N];
  void build() { for (int j=1; j<2*N; j++) lg[j]=31-__builtin_clz(j);
    for (int j=1; j<L+1; j++) for (int i=0; i+(1<<j)-1<2*N; i++)
      rmq[i][j] = min(rmq[i][j-1], rmq[i+(1<<(j-1))][j-1]); }
  T query(int a, int b) { if (a>b) swap(a,b);
    int j=lg[b-a+1]; return min(rmq[a][j], rmq[b-(1<<j)+1][j]); } };

// No need to extend RMQ if HLD only has vertical paths, also remove commented
struct LCA : RMQ<pii> {
  int dh[N], sp[N], par[N], subsz[N]; int lcan; LCA() { lcan = dh[0] = 0; }
  void build(int root) { build(root,0); RMQ::build(); }
  int build(int u, int p) { dh[u]=dh[par[u]=p]+1; sp[u]=lcan; subsz[u]=1;
    rmq[lcan++][0] = pii(dh[u],u); // RMQ
    for (int i:adj[u]) if (i!=p) { subsz[u] += build(i,u);
      rmq[lcan++][0] = pii(dh[u],u); // RMQ
		} return subsz[u]; }
  int query(int a, int b) { return RMQ::query(sp[a],sp[b]).second; } // RMQ
}; // Henry X's signature LCA

struct HLD : LCA {
  vector<int> sz, root; vector<SegTree*> segt; int chain[N], pos[N];
	int hldn, segn; HLD(): LCA() { hldn = segn = 0; }
  void build(int r) { LCA::build(r); build(r,0);
    for (int it:sz) segt.push_back(new SegTree(it)); }
  void build(int cur, int p) {
    if (hldn == root.size()) { root.push_back(cur); sz.push_back(0); }
		chain[cur] = hldn; pos[cur] = sz[chain[cur]]++; segn++; int b=-1, c=-1;
    for (int x:adj[cur]) if (x!=p && subsz[x]>b) { b = subsz[c=x]; }
    if (c != -1) build(c, cur);
    for (int y:adj[cur]) if (y!=p && y!=c) { hldn++; build(y,cur); } }
  void insert_node(int a, int v) { segt[chain[a]]->update(pos[a],pos[a],v); }
  // ASSUMES b IS AN ANCESTOR OF a, UPDATES PATH [a,b), SEGTREE IS [l,r]
  void insert_path(int a, int b, int v) {
    while (chain[a] != chain[b]) {
      segt[chain[a]]->update(0, pos[a], v);
      a = par[root[chain[a]]];
    }
    if (pos[a] != pos[b]) {
      segt[chain[a]]->update(pos[b]+1, pos[a], v);
    }
  }
};


////////////////////////////////////////////////////////////////////////
int main() {
  return 0;
}
