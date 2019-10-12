#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1e5; // number of nodes
const int M = 1e5; // number of edges

////////////////////////////////////////////////////////////////////////
// Minimum Spanning Arborescence -- O(Elog(E) + V)
// Finds the minimum spanning arborescence of a STRONGLY CONNECTED graph
//
// TESTED on: nwerc2018/f
// fails on uva/11183 ???
//
// USAGE:
//  1. MinArb::add_edge(a,b,c);  // add edge a -> b with cost c
//  2. MinArb::contract(u);  // u is the vertex with highest id
//  3. MinArb::expand(r);  // builds minimum out arb rooted at r
//                            the edge ids are stored in[u] for u != r
//                            edge values are: from[e], to[e], cost[e]
//*!
namespace MinArb {
    const int N = 1e5+1; // number of vertices
    const int M = 1e5+1; // number of edges
    int in[2*N], pre[2*N], par[2*N];
    ll shift[2*N];
    vector<int> child[2*N];
    priority_queue<pair<ll,int>> p[2*N];
    int from[M], to[M];
    ll cost[M], nc[M];

    int m = 0;
    void add_edge(int a, int b, ll c=1) {
        p[b].push({c, m});
        from[m] = a;
        to[m] = b;
        nc[m] = cost[m] = c;
        m++;
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
                    for ( ; par[a] == -1; a = pre[a]) {
                        par[a] = c;
                        shift[a] -= nc[in[a]] + shift[find(to[in[a]])];
                        child[c].push_back(a);
                        merge(a, c);
                    }
                    a = c;
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
