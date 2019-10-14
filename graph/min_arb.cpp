#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1e5; // number of nodes
const int M = 1e5; // number of edges

////////////////////////////////////////////////////////////////////////////////
// Minimum Spanning Arborescence -- O(Elog(E) + V)  TESTED: nwerc2018f uva11183
// graph needs to be STRONGLY CONNECTED (make G strong again by adding edges)
// USAGE:
//  1. MinArb::add_edge(a,b,c);  // add edge a -> b with cost c
//  2. MinArb::contract(u);  // u is the vertex with highest id
//  3. MinArb::expand(r);  // builds minimum out arb rooted at r
//     arb edge ids are e=in[u] for u!=r, values are: from[e], to[e], cost[e]
//     If multiple expands per contract (expand kills in[] and par[]), do
//     MinArb::save(n); MinArb::expand(r); (get arb ...) MinArb::load(n);
//  If multiple calls to contract are needed, call MinArb::init(n);
//*!
namespace MinArb {
    const int N = 2500+1;
    const int M = N*N*2;
    int in[2*N], pre[2*N], par[2*N];
    ll shift[2*N];
    vector<int> child[2*N];
    priority_queue<pair<ll,int>> p[2*N];
    int from[M], to[M];
    ll cost[M], nc[M];

    int spar[2*N], sin[2*N];
    void save(int n=N) {
        for (int i = 0; i < 2*n; i++) {
            spar[i] = par[i];
            sin[i] = in[i];
        }
    }
    void load(int n=N) {
        for (int i = 0; i < 2*n; i++) {
            par[i] = spar[i];
            in[i] = sin[i];
        }
    }

    int m = 0;
    void add_edge(int a, int b, ll c=0) {
        p[b].push({-c, m});
        from[m] = a;
        to[m] = b;
        nc[m] = cost[m] = -c;
        m++;
    }
    void init(int n=N) {
        m = 0;
        fill(p,p+2*n,priority_queue<pair<ll,int>>());
        fill(child,child+2*n,vector<int>());
    }

    int dsu[2*N];
    int find(int i) {
        if (dsu[i] == -1) return i;
        return dsu[i] = find(dsu[i]);
    }
    void link(int i, int j) {
        if (find(i) != find(j)) {
            dsu[find(i)] = find(j);
        }
    }

    void merge(int s, int t) {
        assert(s==find(s)&&t==find(t));
        if(p[s].size() > p[t].size()) {
            swap(p[s], p[t]);
            swap(shift[s], shift[t]);
        }
        ll add = shift[s] - shift[t];
        while (!p[s].empty()) {
            auto it = p[s].top();
            p[s].pop();
            p[t].push({it.first + add, it.second});
            nc[it.second] += add;
        }
        link(s, t);
    }

    void contract(int n) {
        memset(dsu, -1, sizeof dsu);
        memset(in, -1, sizeof in);
        memset(shift, 0, sizeof shift);
        memset(pre, -1, sizeof pre);
        memset(par, -1, sizeof par);
        for (int a = n; !p[a].empty(); ) {
            int e = p[a].top().second;
            p[a].pop();
            int u = from[e];
            int b = find(u);
            if (a != b) {
                in[a] = e;
                pre[a] = b;
                if (in[u] == -1) {
                    a = b;
                } else {
                    int c = ++n;
                    for ( ; a != c; a = find(pre[a])) {
                        par[a] = c;
                        shift[a] = -nc[in[a]];
                        child[c].push_back(a);
                        merge(a, c);
                    }
                }
            }
        }
    }

    void expand(int r) {
        stack<int> roots;
        auto dismantle = [&](int u) {
            for ( ; par[u] != -1; u = par[u]) {
                for (int v : child[par[u]]) {
                    if (u == v) continue;
                    par[v] = -1;
                    if (!child[v].empty()) {
                        roots.push(v);
                    }
                }
            }
        };
        for (dismantle(r); !roots.empty(); ) {
            int c = roots.top();
            roots.pop();
            int v = to[in[c]];
            in[v] = in[c];
            dismantle(v);
        }
    }
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
