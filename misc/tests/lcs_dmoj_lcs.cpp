#include <bits/stdc++.h>
using namespace std;

#include "../lcs_bitstring.h"
#include "../longest_common_subseq.h"
#include "../lcs_hunt_szymanski.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n, m;
  cin >> n >> m;
  vector<int> a(n), b(m);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  for (int i = 0; i < m; i++) {
    cin >> b[i];
  }
  auto lcs = longest_common_subseq(a, b);
  auto hsz = lcs_hunt_szymanski(a, b);
  int len = lcs_bitstring(a, b);
  cout << len << nl;

  assert(len == size(lcs));
  assert(len == size(hsz));

  return 0;
}
