#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1<<17; // length of array
const int L = 20; // log(N)

////////////////////////////////////////////////////////////////////////
// Range Minimum Query -- O(nlogn) to build, O(1) to query
// Maximum query is also possible
// The code is 1-indexed, change the loops in build to make it 0-indexed
// Query range is inclusive
//*!
struct RMQ {
	ll rmq[N][L];
	void build(int n, ll a[]) {
		for (int i = 1; i <= n; i++)
			rmq[i][0] = a[i];
		for (int j = 1; j < L; j++)
			for (int i = 1; i+(1<<j)-1 <= n; i++)
				rmq[i][j] = min(rmq[i][j-1], rmq[i+(1<<(j-1))][j-1]);
	}
	ll query(int l, int r) {
		int j = __builtin_clz(r-l+1);
		return min(rmq[l][j], rmq[r-(1<<j)+1][j]);
	}
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
	return 0;
}
