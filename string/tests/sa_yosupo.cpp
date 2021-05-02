#include <bits/stdc++.h>
using namespace std;

#include "../suffix_array.h"

int main() {
  cin.tie(0)->sync_with_stdio(0);

  string s;
  cin >> s;
  suffix_array sa(s);
  for(int i=1; i<=(int)size(s); i++) {
    cout << sa[i] << " ";
  }
  cout << '\n';

  return 0;
}
