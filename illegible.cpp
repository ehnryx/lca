#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int,int> pii;

const int INF = 0x3f3f3f3f;
const int N = 1e5+1;
const int M = 1e5+1;

////////////////////////////////////////////////////////////////////////////////
// Fast Walsh Hadamard Transform -- O(nlog(n))
// Essentially FFT with xor instead of addition
// the array must be a power of 2
// USAGE:
//  1. fwht(a); fwht(b);
//  2. for i in 0..N-1: ans[i] = a[i] * b[i]
//  3. fwht(ans, true);
//*!
template<class T> void fwht(vector<T>& p, bool inv=0) { int n = p.size();
  for (int len=1; 2*len<=n; len*=2) for (int i=0; i<n; i+=2*len)
    for (int j=0; j<len; j++) { T u = p[i+j], v = p[i+len+j];
      p[i+j] = u+v; p[i+len+j] = u-v; }
  if (inv) for (int i=0; i<n; i++) p[i] = p[i] / n;
}
//*/

////////////////////////////////////////////////////////////////////////////////
// Interval Set -- amortized O(log(n)) per operation
// Stores a set of disjoint intervals,
// interval is templated, but default is [a,b)
// insert inserts the interval
// find returns the interval or {inf,inf} if not found
// USAGE:
//  1. Define a type with operators < and ==, Also an inf for that type
//  2. IntervalSet<Type,left_inclusive,right_inclusive> iset(T.inf);
//  3. iset.insert(a,b); to insert the interval (a,b)
//  4. iset.find(x); to find to which interval point x belongs
//*!
template <class T, bool lin=true, bool rin=false> struct IntervalSet {
  T inf; set<pair<T,T>> s; IntervalSet(T i): inf(i) {}
  bool cmp(T a, T b, bool l, bool r=false) {
    if (l||r) return a < b || a == b; else return a < b; }
  void insert(T a, T b) { if (s.empty()) { s.insert({a,b}); return; }
    auto it = s.upper_bound({a,inf});
    while (it != s.end() && cmp(it->first,b,lin,rin)) {
      b = max(b, it->second); it = s.erase(it); }
    it = s.upper_bound({a,inf});
    while (it != s.begin() && cmp(a,(--it)->second,lin,rin)) {
      a = min(a, it->first); b = max(b, it->second); it = s.erase(it); }
    s.insert({a,b}); }
  pair<T,T> find(T x) { if (s.empty()) { return {inf,inf}; }
    auto it = s.upper_bound({x,inf}); if (it == s.begin()) return {inf,inf};
    if (cmp((--it)->second,x,!rin)) return {inf,inf};
    if (cmp(x,it->first,!lin)) return {inf,inf};
    return *it; }
};
//*/

////////////////////////////////////////////////////////////////////////////////
// Weighted Longest Increasing Subsequence Length -- O(nlog(n))
// Returns the weight of a weighted longest increasing subsequence
// pii: value, weight
//*!
template<class T> vector<int> weighted_lis(const vector<pair<int,T>>& a) {
  int n = a.size(); vector<int> pre(n,-1); map<pair<int,int>,T> lis;
  for (int i = 0; i < n; i++) { auto it = lis.lower_bound({a[i].first,0});
    if (it != lis.begin()) pre[i] = prev(it)->first.second;
    for (T cur = a[i].second; cur > 0 && it != lis.end(); ) {
      if (it->second <= cur) { cur -= it->second; it = lis.erase(it); }
      else { it->second -= cur; cur = 0; } }
    lis[pii(a[i].first,i)] = a[i].second; }
  vector<int> res;
  for (int i = lis.rbegin()->first.second; i!=-1; i=pre[i]) res.push_back(i);
  reverse(res.begin(),res.end()); return res;
}
//*/

////////////////////////////////////////////////////////////////////////////////
// Wavelet Tree -- Queries are O(log(A)), A = size of alphabet
// No updates are allowed! Everything is 1-indexed
// USAGE:
//  0. WaveletTree wt(s.begin(), s.end()); WARNING: s gets destroyed!
//*!
struct WaveletTree { int lo, hi; vector<int> b; WaveletTree *left, *right;
  template<class T> WaveletTree(T s, T e):
    WaveletTree(s, e, *min_element(s,e), *max_element(s,e)) {}
  template<class T> WaveletTree(T s, T e, int l, int r): lo(l), hi(r) {
    if (lo == hi || s == e) { left = right = nullptr; return; }
    int mid = (lo+hi)/2; auto f = [&](int x) { return x <= mid; };
    b.reserve(distance(s,e)+1); b.push_back(0);
    for (auto it=s; it!=e; it++) b.push_back(b.back()+f(*it));
    auto m = stable_partition(s, e, f);
    left = new WaveletTree(s, m, lo, mid);
    right = new WaveletTree(m, e, mid+1, hi);
  }
  // nth smallest element, inclusive range, must be valid
  int nth(int l, int r, int n) {
    if (l > r) return INF;
    if (lo == hi) return lo;
    int lb = b[l-1], rb = b[r];
    if (n <= rb-lb) return left->nth(lb+1, rb, n);
    else return right->nth(l-lb, r-rb, n-rb+lb);
  }
  // number of elements less than or equal to k
  int leq(int l, int r, int k) {
    if (l > r || k < lo) return 0;
    if (hi <= k) return r-l+1;
    int lb = b[l-1], rb = b[r];
    return left->leq(lb+1, rb, k) + right->leq(l-lb, r-rb, k);
  }
  // number of elements equal to k
  int count(int l, int r, int k) {
    if (l > r || k < lo || k > hi) return 0;
    if (lo == hi) return r-l+1;
    int lb = b[l-1], rb = b[r], m = (lo+hi)/2;
    if (k <= m) return left->count(lb+1, rb, k);
    else return right->count(l-lb, r-rb, k);
  }
};
//*/

////////////////////////////////////////////////////////////////////////////////
// Minimum Spanning Arborescence -- O(Elog(E) + V)  TESTED: nwerc2018f uva11183
// graph needs to be STRONGLY CONNECTED (make G strong again by adding edges)
// USAGE:
//  1. MinArb::add_edge(a,b,c);  // add edge a -> b with cost c
//  2. MinArb::contract(u);  // u is the vertex with highest id
//  3. MinArb::expand(r);  // builds minimum out arb rooted at r
//     arb edge ids are e=in[u] for u!=r, values are: from[e], to[e], cost[e]
//     If multiple expands per contract, do
//     MinArb::save(n); MinArb::expand(r); MinArb::load(n);
//  If multiple calls to contract are needed, call MinArb::init(n);
//*!
namespace MinArb {
  int in[2*N], pre[2*N], par[2*N], dsu[2*N], spar[2*N], sin[2*N]; ll sh[2*N]; 
  vector<int> child[2*N]; priority_queue<pair<ll,int>> p[2*N];
  int from[M], to[M]; ll cost[M], nc[M]; int m = 0;
  void add_edge(int a, int b, ll c=0) {
    from[m] = a; to[m] = b; nc[m] = -c; cost[m] = c; p[b].push({-c, m++}); }
  void init(int n=N) { m = 0;
    fill(p,p+2*n,priority_queue<pair<ll,int>>());
    fill(child,child+2*n,vector<int>()); }
  int find(int i) { return dsu[i] == -1 ? i : dsu[i] = find(dsu[i]); }
  void link(int i, int j) { if (find(i) != find(j)) dsu[find(i)] = find(j); }
  void contract(int n) { memset(dsu, -1, sizeof dsu);
    memset(in, -1, sizeof in); memset(sh, 0, sizeof sh);
    memset(pre, -1, sizeof pre); memset(par, -1, sizeof par);
    for (int a = n; !p[a].empty(); ) {
      int e = p[a].top().second; p[a].pop(); int u = from[e]; int b = find(u);
      if (a != b) { in[a] = e; pre[a] = b; if (in[u] == -1) a = b; else {
        for (int c = ++n; a != c; a = find(pre[a])) {
          par[a] = c; sh[a] = -nc[in[a]]; child[c].push_back(a);
          if (p[a].size()>p[c].size()) { swap(p[a],p[c]); swap(sh[a],sh[c]); }
          for (ll add = sh[a]-sh[c]; !p[a].empty(); ) { auto it = p[a].top();
            p[a].pop(); p[c].push({it.first + add, it.second});
            nc[it.second] += add; } link(a, c); } } } } }
  void expand(int r) { stack<int> roots; auto dismantle = [&](int u) {
    for ( ; par[u] != -1; u = par[u]) for (int v : child[par[u]]) {
      if (u!=v) { par[v] = -1; if (!child[v].empty()) roots.push(v); } } };
  for (dismantle(r); !roots.empty(); ) { int c = roots.top(); roots.pop();
    int v = to[in[c]]; in[v] = in[c]; dismantle(v); } }
  void save(int n=N) { for(int i=0;i<2*n;i++) { spar[i]=par[i]; sin[i]=in[i]; }}
  void load(int n=N) { for(int i=0;i<2*n;i++) { par[i]=spar[i]; in[i]=sin[i]; }}
}
//*/

////////////////////////////////////////////////////////////////////////////////
int main() { return 0; }
