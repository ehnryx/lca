#include <bits/stdc++.h>
using namespace std;

#include "../suffix_array.h"
#include "../../data_structure/min_queue.h"

constexpr char nl = '\n';

int main() {
  cin.tie(0)->sync_with_stdio(0);

  basic_string<int> cat;
  vector<int> len;
  for(string s; cin >> s; ) {
    len.push_back((int)size(s) + 1);
    cat.push_back('z' + (int)size(len));
    cat.insert(end(cat), begin(s), end(s));
  }

  int n = (int)size(len);
  if (n == 1) {
    cout << len[0] - 1 << nl;
    return 0;
  }
  partial_sum(begin(len), end(len), begin(len));
  suffix_array sa(cat);

  map<int,int> have;
  min_queue<int> window;
  int ans = 0;
  for(int i=1, j=1; i<=(int)size(cat); i++) {
    while(j <= (int)size(cat) && (int)size(have) < n) {
      if(j > i) window.push(sa.height[j]);
      have[int(upper_bound(begin(len), end(len), sa[j++]) - begin(len))] += 1;
    }
    if((int)size(have) < n) break;
    ans = max(ans, window.min());
    int rem = int(upper_bound(begin(len), end(len), sa[i]) - begin(len));
    if(--have[rem] == 0) have.erase(rem);
    window.pop();
  }
  cout << ans << nl;


  return 0;
}
