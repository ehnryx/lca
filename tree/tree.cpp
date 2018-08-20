#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1<<17; // number of nodes in the tree
const int L = 20; // log_2(N)

// General data structure to sture the tree
vector<int> adj[N];

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
};
//*/

// TODO make HLD inherit from LCA or something
////////////////////////////////////////////////////////////////////////
// Heavy Light Decomposition -- O(n) to build
// Paths on the tree go through O(log(n)) heavy chains
// * Nodes are 1-indexed
//*!
struct HLD {
	vector<int> sz, root, start;
	vector<int> parent, subsz, chain, pos;
	int hldnum, segnum;

	HLD(int n) {
		hldnum = segnum = 0;
		parent.resize(n);
		subsz.resize(n);
		chain.resize(n);
		pos.resize(n);
	}

	// 0 indexed, returns the position
	int get(int i) {
		return start[chain[i]] + pos[i];
	}

	void build(int root) {
		precmp(root); // comment this out if parent and subsz are calculated already
		build_hld(root);
	}

	// This function does not need to be in here,
	// will speed it up to compute subsz and parent in LCA dfs
	int precmp(int cur, int par = 0) {
		parent[cur] = par;
		subsz[cur] = 1;
		for (int x : adj[cur]) {
			if (x != par) {
				subsz[cur] += precmp(x, cur);
			}
		}
		return subsz[cur];
	}

	void build_hld(int cur, int par = 0) {
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
