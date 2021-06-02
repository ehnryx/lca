#include <bits/stdc++.h>
using namespace std;

#include "../range_query_tree.h"
#include "../../data_structure/segment_tree.h"

constexpr char nl = '\n';

struct Node {
  int value;
  int get() const { return value; }
  void put(int v) { value = v; }
  void pull(const Node& a, const Node& b) {
    value = max(a.value, b.value);
  }
  void push(Node&, Node&) {}
  static int default_value() { assert(false); }
  static int merge(int a, int b) { return max(a, b); }
};

using hammer = range_query_tree<segment_tree<Node, int>, int>;

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    vector<tuple<int, int, int>> edges;
    for (int i = 1; i < n; i++) {
      int a, b, c;
      cin >> a >> b >> c;
      adj[a].push_back(b);
      adj[b].push_back(a);
      edges.emplace_back(a, b, c);
    }
    hammer hld(move(adj), 1);
    for (auto [a, b, c] : edges) {
      if (a == hld.parent[b]) swap(a, b);
      hld.update_point(a, c);
    }
    for (string s; cin >> s && s != "DONE"; ) {
      if (s == "QUERY") {
        int a, b;
        cin >> a >> b;
        int ans = hld.query_path(a, b, false, 0,
                                 [](int x, int y) {
                                   return max(x, y);
                                 });
        cout << ans << nl;
      } else {
        int i, v;
        cin >> i >> v;
        auto [a, b, _] = edges[i - 1];
        if (a == hld.parent[b]) swap(a, b);
        hld.update_point(a, v);
      }
    }
  }

  return 0;
}
