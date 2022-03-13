#include <bits/stdc++.h>
using namespace std;

#include "../kmp_matching.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for(string s, t; getline(cin, s) && getline(cin, t); ) {
    kmp_matching kmp(s);
    vector<int> all = kmp.find_all(t);
    for(int i : all) {
      cout << i << " ";
    }
    cout << nl;
  }

  return 0;
}
