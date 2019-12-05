#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1e5+1;

vector<int> adj[N];

////////////////////////////////////////////////////////////////////////
// Centroid Decomposition -- O(nlog(n)) to build (UNTESTED)
// USAGE:
//  0. MUST BE 1-indexed
//  1. decompose(u); where u is some vertex in the tree
//  2. centroid tree stored as adjacency list in cent[]
//  3. centroid parents stored in cpar[]
//  4. centroid depth stored in cdepth[]
//  5. centroid subtree vertices stored in csub[] (nlogn memory)
//*!
namespace Centroid {
    vector<int> cent[N];
    int sz[N], cdepth[N], cpar[N];
    unordered_set<int> csub[N];

    int dfs(int u, int p) {
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v != p) {
                sz[u] += dfs(v, u);
            }
        }
        return sz[u];
    }

    int centroid(int u, int p) {
        for (int v : adj[u]) {
            if (cdepth[v] == -1 && sz[v] > sz[u]/2) {
                sz[u] -= sz[v];
                sz[v] += sz[u];
                return centroid(v, p);
            }
        }
        // at centroid
        cdepth[u] = cdepth[p] + 1;
        cpar[u] = p;
        if (p) cent[p].push_back(u);
        csub[u].insert(u);
        for(int v : adj[u]) {
            if (cdepth[v] == -1) {
                int cv = centroid(v, u);
                //*! to save vertices in subtree
                for (int it : csub[cv]) {
                    csub[u].insert(it);
                }
                //*/
            }
        }
        return u;
    }

    int decompose(int u) {
        memset(cdepth, -1, sizeof cdepth);
        dfs(u, 0);
        return centroid(u, 0);
    }
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
