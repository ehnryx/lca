#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1<<17; // length of array

////////////////////////////////////////////////////////////////////////
// Disjoint Set Union -- O(a(n)) per query, (inverse ackermann)
// Usage: 
// 1. dsu(n) to create n disjoint sets from 0 to n-1
// 2. dsu.link(i,j) to combine sets containing i and j
// 3. dsu.find(i) to find the root of the set containing i
//*!
struct DSU {
	vector<int> root;
	vector<int> sz;

	DSU(int n) {
		root.resize(n, -1);
		sz.resize(n, 1);
	}

	int find(int i) {
		if (root[i] == -1) return i;
		return root[i] = find(root[i]);
	}

	// returns true if we combine two sets
	bool link(int i, int j) {
		i = find(i);
		j = find(j);
		if (i == j) return false;
		if (sz[i] < sz[j]) swap(i,j);
		root[j] = i;
		sz[i] += sz[j];
		return true;
	}
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
