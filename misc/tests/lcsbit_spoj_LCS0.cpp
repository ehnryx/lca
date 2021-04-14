#include <bits/stdc++.h>
using namespace std;

#include "../lcs_bitstring.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  string a, b;
  cin >> a >> b;
  auto lcs = lcs_bitstring(
      vector<char>(begin(a), end(a)),
      vector<char>(begin(b), end(b)));
  cout << lcs << nl;

  return 0;
}
