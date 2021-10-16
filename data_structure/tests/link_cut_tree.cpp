#include <bits/stdc++.h>
using namespace std;

#include "../link_cut_tree.h"

const char nl = '\n';

struct node : link_cut_node<node, char> {};

int main() {

  int n;
  cin >> n;
  link_cut_tree<node> st(n);

  static_assert(link_cut_tree<node>::has_push);

  while(true) {
    cout << nl << "-----------------" << nl;
    for(int i=0; i<n; i++) {
      for(int j=i+1; j<n; j++) {
        st.reroot(i);
        if(st.parent(j) == i) {
          cout << i << " " << j << nl;
        }
      }
    }
    cout << "-----------------" << nl << nl;

    char t;
    cin >> t;
    if (t == '+') {
      int a, b;
      cin >> a >> b;
      st.link(a, b);
    } else if (t == '-') {
      int a, b;
      cin >> a >> b;
      st.cut(a, b);
    } else if (t == 'r') {
      int r;
      cin >> r;
      st.reroot(r);
    } else if(t == 'a') {
      int a;
      cin >> a;
      st.access(a);
    } else if (t == '!') {
      break;
    }
  }

}
