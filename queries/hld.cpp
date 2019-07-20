#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1e5+1; // number of nodes
vector<int> adj[N]; // adjacency graph

////////////////////////////////////////////////////////////////////////
// Heavy Light Decomposition -- O(log(n)^2) per query (UNTESTED)
// This is a legible version of the code archive HLD code
//
// USAGE:
//  0. 1-INDEXED
//  1. Code a segment tree to solve the problem on a path
//  2. Build the adjacency list of the tree in vector<int> adj[N];
//  3. Call HLD::build(root);
//  4. Call HLD::insert_path(a, b, v) to update the path from a to b
//      with value v. This will NOT update lca(a,b).
//  5. Call HLD::insert_node(a, v) to update node a with value v
//  6. Use query_path and query_node similarly
//*!
// Segment Tree Example
struct SegTree {
    int n;
    int *segt;
    SegTree(int len) {
        n = 1 << (32 - __builtin_clz(len));
        segt = new int[2*n];
        fill(segt, segt+2*n, 0);
    }
    /* implement the rest of the segment tree */ 
    // this update should be inclusive-inclusive
    void update(int l, int r, int v) {}
    // etc...
};

namespace HLD {
    SegTree* segt[N];

    int depth[N], parent[N], subsz[N]; // tree stuff
    int chainsz[N], root[N], chain[N], pos[N]; // decomposition stuff
    int hldnum, chainnum;

    int precompute(int u, int par) {
        parent[u] = par;
        depth[u] = depth[par] + 1;
        subsz[u] = 1;
        for (int x : adj[u]) {
            if (x != par) {
                subsz[u] += precompute(x, u);
            }
        }
        return subsz[u];
    }

    void build_hld(int u, int par) {
        if (hldnum == chainnum) {
            root[chainnum] = u;
            chainsz[chainnum] = 0;
            chainnum++;
        }
        chain[u] = hldnum;
        pos[u] = chainsz[hldnum]++;
        int best = -1;
        int child = -1;
        for (int v : adj[u]) {
            if (v != par && subsz[v] > best) {
                child = v;
                best = subsz[v]; 
            }
        }
        if (child != -1) {
            build_hld(child, u);
        }
        for (int v : adj[u]) {
            if (v != par && v != child) {
                hldnum++;
                build_hld(v, u);
            }
        }
    }

    void build(int r) {
        hldnum = chainnum = 0;
        depth[0] = 0;
        precompute(r, 0);
        build_hld(r, 0);
        for (int i = 0; i < chainnum; i++) {
            segt[i] = new SegTree(chainsz[i]);
        }
    }

    int insert_path(int a, int b, int v) {
        while (chain[a] != chain[b]) {
            if (depth[root[chain[a]]] < depth[root[chain[b]]]) {
                swap(a,b);
            }
            segt[chain[a]]->update(0, pos[a], v);
            a = parent[root[chain[a]]];
        }
        if (pos[a] != pos[b]) {
            if (depth[a] < depth[b]) {
                swap(a,b);
            }
            segt[chain[a]]->update(pos[b]+1, pos[a], v);
        }
        // return the lca of the path ab, because why not
        return b;
    }

    void insert_node(int a, int v) {
        segt[chain[a]]->update(pos[a], pos[a], v);
    }
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
