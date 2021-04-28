#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../../lca/string/suffix_array.h"
#include "../../lca/data_structure/min_queue.h"
#else
#include "suffix_array.h"
#include "min_queue.h"
#endif

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  basic_string<int> cat;
  vector<int> len;
  for(int i=0; i<n; i++) {
    string s;
    cin >> s;
    len.push_back(size(s) + 1);
    cat.push_back('z' + (int)size(len));
    cat.insert(end(cat), begin(s), end(s));
  }

  if (n == 1) {
    cout << len[0] - 1 << nl;
    return 0;
  }
  partial_sum(begin(len), end(len), begin(len));
  suffix_array sa(cat);

  map<int,int> have;
  min_queue<int> window;
  int ans = 0;
  for(int i=1, j=1; i<=size(cat); i++) {
    while(j <= size(cat) && size(have) < n) {
      if(j > i) window.push(sa.height[j]);
      have[upper_bound(begin(len), end(len), sa[j++]) - begin(len)] += 1;
    }
    if(size(have) < n) break;
    ans = max(ans, window.min());
    int rem = upper_bound(begin(len), end(len), sa[i]) - begin(len);
    if(--have[rem] == 0) have.erase(rem);
    window.pop();
  }
  cout << ans << nl;

  return 0;
}
