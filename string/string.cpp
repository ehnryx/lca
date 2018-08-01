#include <bits/stdc++.h>
using namespace std;

////////////////////////////////////////////////////////////////////////
// KMP Fail Table (UBC)
// Returns the 1-indexed fail table of the input string
//*!
vector<int> build_fail(const string& s) {
	int n = s.size();
	vector<int> fail(n+1);
	for (int i = 0, j = -1; ; i++, j++) {
		fail[i] = j;
		if (i == n) return fail;
		while (j >= 0 && s[i] != s[j]) {
			j = fail[j];
		}
	}
}
//*/

////////////////////////////////////////////////////////////////////////
// KMP Algorithm (UBC)
// Returns matches ??? (Documentation needed)
//*!
vector<int> kmp_match(string t, string p) { int n=t.size(),m=p.size();
	vector<int> matches, fail = kmp_init(p);
	for (int i=0, j=0; ; ++i, ++j) {
		if (j == m) matches.push_back(i-j), j = fail[j];
		if (i == n) return matches;
		while (j>=0 && t[i]!=p[j]) j = fail[j];
	}
	return matches;
}
//*/


////////////////////////////////////////////////////////////////////////
int main() {
	return 0;
}
