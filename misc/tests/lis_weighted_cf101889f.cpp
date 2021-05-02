#include <bits/stdc++.h>
using namespace std;

#include "../lis_weighted.h"

using ll = long long;
const char nl = '\n';

struct Point {
  int x, y; ll v;
  Point(const pair<int,int>& p, ll _v): x(p.first), y(p.second), v(_v) {}
  bool operator < (const Point& o) const {
    if (x == o.x) return y > o.y;
    else return x < o.x;
  }
};

//#define FILEIO
int main() {
  ios::sync_with_stdio(0);
  cin.tie(0); cout.tie(0);
  cout << fixed << setprecision(10);
#ifdef FILEIO
  freopen("test.in", "r", stdin);
  freopen("test.out", "w", stdout);
#endif

  int n, a, b, c;
  cin >> n;

  map<pair<int,int>,ll> p;
  for (int i=0; i<n; i++) {
    cin >> a >> b >> c;
    p[pair(a,b)] += c;
  }

  vector<Point> arr;
  for (const auto& it : p) {
    arr.push_back(Point(it.first, it.second));
  }
  n = (int)arr.size();
  sort(arr.begin(), arr.end());

  vector<pair<int,ll>> lis;
  for (const Point& it : arr) {
    lis.push_back(pair<int,ll>(it.y, it.v));
  }
  ll ans = 0;
  for (int i : lis_weighted(lis)) {
    ans += lis[i].second;
  }
  cout << ans << nl;

  return 0;
}
