#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1<<17; // number of nodes in the tree
const int L = 20; // log_2(N)

// General data structure to store the tree
vector<vector<int>> adj;

////////////////////////////////////////////////////////////////////////
// Lowest Common Ancestor -- O(nlog(n)) to build, O(log(n)) to query
// TESTED ON cf1023/f
// This one uses binary jumping, so adding leaves is possible
// If the tree needs not be changed, then we can use RMQ
// on the euler tour.
// Nodes are 1-indexed
//*!
struct LCA {
    vector<vector<int>> lca;
    vector<int> depth;

    LCA(int n) {
        int l = 32 - __builtin_clz(n);
        lca.resize(n, vector<int>(l));
        depth.resize(n, 0);
    }

    void build(int cur, int par = 0) {
        depth[cur] = depth[par] + 1;
        lca[cur][0] = par;
        for (int j = 1; 1<<j < depth[cur]; j++) {
            lca[cur][j] = lca[lca[cur][j-1]][j-1];
        }
        for (int x : adj[cur]) {
            if (x != par) {
                build(x, cur);
            }
        }
    }

    int query(int a, int b) {
        if (depth[a] < depth[b]) swap(a,b);
        int diff = depth[a] - depth[b];
        for (int i = 0; diff > 0; i++, diff /= 2) {
            if (diff & 1) a = lca[a][i];
        }
        if (a == b) return a;

        for (int i = 31 - __builtin_clz(depth[a]); i >= 0; i--) {
            if (1<<i >= depth[a]) continue;
            if (lca[a][i] != lca[b][i]) {
                a = lca[a][i];
                b = lca[b][i];
            }
        }
        return lca[a][0];
    }

    void add_leaf(int id, int par) {
        depth[id] = depth[par] + 1;
        lca[id][0] = par;
        for (int j = 1; 1<<j < depth[id]; j++) {
            lca[id][j] = lca[lca[id][j-1]][j-1];
        }
    }
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
