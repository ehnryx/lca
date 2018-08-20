#include <bits/stdc++.h>
using namespace std;

typedef pair<int,int> pii;

////////////////////////////////////////////////////////////////////////
// Range Minimum Query -- O(nlogn) to build, O(1) to query
// Maximum query is also possible
// The code is 0-indexed, change the loops in build to make it 1-indexed
// Query range is inclusive
//*!
template <class T> struct RMQ {
    int lg;
    vector<vector<T>> rmq;

    RMQ(int n = 0) {
        if (n) resize(n);
    }
    void resize(int n) {
        lg = 32 - __builtin_clz(n);
        rmq.resize(n, vector<T>(lg));
    }

    void build(const vector<T>& a) {
        for (int i = 0; i < a.size(); i++)
            rmq[i][0] = a[i];
        build();
    }
    void build() {
        for (int j = 1; j < lg; j++)
            for (int i = 0; i+(1<<j)-1 < rmq.size(); i++)
                rmq[i][j] = min(rmq[i][j-1], rmq[i+(1<<(j-1))][j-1]);
    }

    T query(int l, int r) {
        int j = 31 - __builtin_clz(r-l+1);
        return min(rmq[l][j], rmq[r-(1<<j)+1][j]);
    }
};
//*/

// FOR QUERIES ON TREES: 
// General data structure to store the tree
vector<vector<int>> adj;

////////////////////////////////////////////////////////////////////////
// Lowest Common Ancestor -- Using RMQ
// Nodes are 1-indexed
//*!
struct LCA {
    RMQ<pii> rmq;
    vector<int> depth, segpos;
    vector<int> parent, subsz; // These are for HLD
    int lcanum = 0;

    LCA(int n) {
        rmq.resize(2*n);
        lcanum = 0;
        depth.resize(n);
        segpos.resize(n);
        parent.resize(n); // for HLD
        subsz.resize(n); // for HLD
        depth[0] = 0;
    }

    void build(int root) {
        build(root, 0);
        rmq.build();
    }

    int build(int cur, int par) {
        depth[cur] = depth[par] + 1;
        segpos[cur] = lcanum;
        parent[cur] = par; // for HLD
        subsz[cur] = 1; // for HLD
        rmq.rmq[lcanum++][0] = pii(depth[cur], cur);
        for (int x : adj[cur]) {
            if (x != par) {
                subsz[cur] += build(x, cur);
                rmq.rmq[lcanum++][0] = pii(depth[cur], cur);
            }
        }
        return subsz[cur];
    }

    int query(int a, int b) {
        return rmq.query(a,b).second;
    }
};
//*/

////////////////////////////////////////////////////////////////////////
// Heavy Light Decomposition -- O(n) to build
// TESTED ON cf1023/f
// Paths on the tree go through O(log(n)) heavy chains
// * Nodes are 1-indexed
//*!
struct HLD : LCA {
    vector<int> sz, root, start; // indexed by chains
    vector<int> chain, pos; // indexed by nodes
    int hldnum, segnum;

    HLD(int n): LCA(n) {
        hldnum = segnum = 0;
        chain.resize(n);
        pos.resize(n);
    }

    // 0 indexed, returns the position
    int get(int i) const {
        return start[chain[i]] + pos[i];
    }

    void build(int root) {
        LCA::build(root);
        build_hld(root, 0);
    }

    void build_hld(int cur, int par) {
        if (hldnum == root.size()) {
            root.push_back(cur);
            start.push_back(segnum);
            sz.push_back(0);
        }
        chain[cur] = hldnum;
        pos[cur] = sz[chain[cur]]++;
        segnum++;

        int best = -1;
        int child = -1;
        for (int x : adj[cur]) {
            if (x != par && subsz[x] > best) {
                best = subsz[x];
                child = x;
            }
        }
        if (child != -1) build_hld(child, cur);

        for (int x : adj[cur]) {
            if (x != par && x != child) {
                hldnum++;
                build_hld(x, cur);
            }
        }
    }

    // Inserting a path into a segtree on the chains
    // UPDATE interval is [a,b)
    // path: a -> b, b is an ancestor of a
    // value: c
    void insert_path(int a, int b, int v) {
        while (chain[a] != chain[b]) {
            int s = start[chain[a]];
            UPDATE(s, s + pos[a], v); // update segtree here
            a = parent[root[chain[a]]];
        }
        if (pos[a] != pos[b]) {
            int s = start[chain[a]];
            UPDATE(s + pos[b] + 1, s + pos[a], v); // update segtree here
        }
    }
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
