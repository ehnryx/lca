#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../suffix_array.h"
#else
#include "suffix_array.h"
#endif

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for(string s; getline(cin, s); ) {
    suffix_array sa(s);
    getline(cin, s);
    istringstream in(s);
    int n;
    in >> n;
    for(int i=0; i<n; i++) {
      int v;
      in >> v;
      cout << sa[v + 1] << " ";
    }
    cout << nl;
  }

  return 0;
}
