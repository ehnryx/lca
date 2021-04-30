#include <bits/stdc++.h>
using namespace std;
#define _USE_MATH_DEFINES

#ifdef HENRYX
#include "../dinic.h"
#else
#include "dinic.h"
#endif

const char nl = '\n';
const int INF = 0x3f3f3f3f;


//#define FILEIO
int main() {
	ios::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);
	cout << fixed << setprecision(10);
#ifdef FILEIO
	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);
#endif

	int T;
	cin >> T;
	while (T--) {
		int n;
		cin >> n;
		int val[n];
		vector<int> adj[n];
		for (int i=0; i<n; i++) {
			cin >> val[i];
		}
		char t;
		for (int i=0; i<n; i++) {
			for (int j=0; j<n; j++) {
				cin >> t;
				if (t=='Y') adj[i].push_back(j);
			}
		}

		const int source = 2*n;
		const int sink = source+1;

		auto valid = [&] (int lb) {
      dinic<int> graph(sink + 1);
			int cnt = 0;
			int other = 0;
			for (int i=0; i<n; i++) {
				if (val[i]>0) {
					bool border = false;
					graph.add_edge(source, 2*i, val[i]);
					graph.add_edge(2*i, 2*i+1, INF);
					for (int j : adj[i]) {
						if (val[j]) {
							graph.add_edge(2*i, 2*j+1, INF);
						} else {
							border = true;
						}
					}
					if (border) {
						cnt++;
						graph.add_edge(2*i+1, sink, lb);
					} else {
						other++;
						graph.add_edge(2*i+1, sink, 1);
					}
				}
			}
			return graph.flow(source, sink) == lb*cnt + other;
		};

		int left = 0;
		int right = n*100;
		while (left<right) {
			int mid = (left+right+1)/2;
			if (valid(mid)) {
				left = mid;
			} else {
				right = mid-1;
			}
		}
		cout << left << nl;
	}

	return 0;
}
