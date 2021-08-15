#include <bits/stdc++.h>
using namespace std;

#include "../splay_tree.h"

const char nl = '\n';

using node = splay_node<int, int>;

int main() {
  splay_tree<node> st;

  for(int i=0; ; i++) {
    cout << "SET:" << nl;
    for (const auto& [a, b] : st) {
      cout << a << "  ";
    }
    cout << "\n------" << endl;

    char t;
    cin >> t;
    if (t == '+') {
      int n;
      cin >> n;
      st.insert(new node(n, 0));
    } else if (t == '-') {
      int n;
      cin >> n;
      st.erase(st.find(n));
    } else if (t == '!') {
      break;
    }
  }
}
