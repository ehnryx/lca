#include <bits/stdc++.h>
using namespace std;

typedef pair<int,int> pii;

// values for 1e5
const int L = 17;
const int N = 1<<L;

// FOR QUERIES ON TREES: 
// General data structure to store the tree
vector<int> adj[N];

// EXAMPLE FOR SEGTREE
void UPDATE(int, int, int);

////////////////////////////////////////////////////////////////////////
// Range Minimum Query -- O(nlogn) to build, O(1) to query
// Maximum query is also possible
// The code is 0-indexed, change the loops in build to make it 1-indexed
// Query range is inclusive
//*!
template<class T> struct RMQ {
  T rmq[2*N][L+1]; int lg[2*N];
  void build() { for (int j=1; j<2*N; j++) lg[j]=31-__builtin_clz(j);
    for (int j=1; j<L+1; j++) for (int i=0; i+(1<<j)-1<2*N; i++)
      rmq[i][j] = min(rmq[i][j-1], rmq[i+(1<<(j-1))][j-1]); }
  T query(int a, int b) { if (a>b) swap(a,b);
    int j=lg[b-a+1]; return min(rmq[a][j], rmq[b-(1<<j)+1][j]); }
};
//*/

////////////////////////////////////////////////////////////////////////
// Lowest Common Ancestor -- Using RMQ
// Nodes are 1-indexed
//*!
struct LCA : RMQ<pii> {
  int dh[N], sp[N];
  int par[N], subsz[N]; // These are for HLD
  int lcan; LCA() { lcan = dh[0] = 0; }
  void build(int root) { build(root,0); RMQ::build(); }
  int build(int cur, int p) {
    dh[cur] = dh[par[cur]=p]+1; sp[cur] = lcan; subsz[cur] = 1;
    rmq[lcan++][0] = pii(dh[cur],cur);
    for (int i:adj[cur]) if (i!=p) { subsz[cur] += build(i,cur);
      rmq[lcan++][0]=pii(dh[cur],cur); } return subsz[cur]; }
  int query(int a, int b) { return RMQ::query(sp[a],sp[b]).second; }
};
//*/

////////////////////////////////////////////////////////////////////////
// Heavy Light Decomposition -- O(n) to build
// TESTED ON cf1023/f
// Paths on the tree go through O(log(n)) chains
// * Nodes are 1-indexed
//*!
struct HLD : LCA {
  vector<int> sz, root, start; // indexed by chains
  int chain[N], pos[N]; // indexed by nodes
  int hldn, segn; HLD(): LCA() { hldn = segn = 0; }
  // 0 indexed, returns the position
  int get(int i) const { return start[chain[i]] + pos[i]; }
  void build(int r) { LCA::build(r); build_hld(r,0); }
  void build_hld(int cur, int p) {
    if (hldn == root.size()) {
      root.push_back(cur); start.push_back(segn); sz.push_back(0);
    } chain[cur] = hldn; pos[cur] = sz[chain[cur]]++; segn++;
    int best=-1, child=-1;
    for (int x:adj[cur])
      if (x!=p && subsz[x]>best) { best = subsz[child=x]; }
    if (child != -1) build_hld(child, cur);
    for (int x:adj[cur])
      if (x!=p && x!=child) { hldn++; build_hld(x,cur); } }
  // Inserting a path into a segtree on the chains
  // insert_path interval is [a,b), but UPDATE is [s,t]
  // path: a -> b, b is an ancestor of a
  // value: v
  void insert_path(int a, int b, int v) {
    while (chain[a] != chain[b]) {
      int s = start[chain[a]];
      UPDATE(s, s+pos[a], v); // update segtree here
      a = par[root[chain[a]]];
    }
    if (pos[a] != pos[b]) {
      int s = start[chain[a]];
      UPDATE(s+pos[b]+1, s+pos[a], v); // update segtree here
    }
  }
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
