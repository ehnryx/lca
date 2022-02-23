#include <bits/stdc++.h>
using namespace std;

#include "../splay_tree.h"

const char nl = '\n';

using node = splay_node<int, int>;

int main() {
  splay_tree<node, 200> st;

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
      st.insert(n, 0);
    } else if (t == '-') {
      int n;
      cin >> n;
      st.erase(st.find(n));
    } else if (t == '!') {
      break;
    }
  }

  //splay_tree<node> sad = st;
  //splay_tree<node> bad(st);
  //splay_tree<node> ok(move(st));
  //splay_tree<node> yes = move(st);
  //splay_tree<node> no(st.root);

  splay_tree<node> good;
  st.copy_to(good);
  for(const auto& [a, b] : good) {
    cout << a << "  ";
  }
  cout << endl;
  for(const auto& [a, b] : st) {
    cout << a << "  ";
  }
  cout << "\n------" << endl;

  //st.clear();
  //for(const auto& [a, b] : good) {
    //cout << a << "  ";
  //}
  //cout << endl;

  good.clear();
  for(const auto& [a, b] : st) {
    cout << a << "  ";
  }
  cout << endl;

  splay_tree<node, 200> allowed = st.dislodge();

  cerr << "st: ";
  for(const auto& [a, b] : st) {
    cout << a << "  ";
  }
  cout << endl;

  cerr << "allow: ";
  for(const auto& [a, b] : allowed) {
    cout << a << "  ";
  }
  cout << endl;

  auto other = move(good);
  cerr << "sad: ";
  for(const auto& [a, b] : other) {
    cout << a << "  ";
  }
  cout << endl;

  splay_tree<node, 200> other2 = allowed;
  cerr << "other2: ";
  for(const auto& [a, b] : other2) {
    cout << a << "  ";
  }
  cout << endl;

}
