#include <bits/stdc++.h>
using namespace std;

#include "../point.h"
#include "../circumcenter.h"

const char nl = '\n';
using ld = long double;
using pt = point<ld>;

const ld PI = acos((ld)-1);
const ld EPS = 1e-7;

int main() {
	ios::sync_with_stdio(0); cin.tie(0);

	for(ld x[3], y[3]; cin >> x[0] >> y[0] >> x[1] >> y[1] >> x[2] >> y[2]; ) {
		pt v[3];
		for(int i=0; i<3; i++) {
			v[i] = pt(x[i], y[i]);
		}
		pt c = circumcenter(v[0], v[1], v[2]);
		ld a = arg((v[1]-c) / (v[0]-c));
		ld b = arg((v[2]-c) / (v[0]-c));
		int ans = -1;
		for(int i=3; i<=1000; i++) {
			ld g = 2*PI / i;
			if(abs(a/g - round(a/g)) < EPS && abs(b/g - round(b/g)) < EPS) {
				ans = i;
				break;
			}
		}
		assert(ans != -1);
		cout << ans << nl;
	}


	return 0;
}
