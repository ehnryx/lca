#include <bits/stdc++.h>
using namespace std;

#include "../heap.h"

const char nl = '\n';


int main() {
  heap<int> h;
  for(int i=0; ; i++) {
    cout << "top:   " << (h.empty() ? "NONE" : to_string(h.top())) << endl;
    cout << "size:  " << h.size() << endl;
    cout << "empty: " << h.empty() << endl;
    cout << "heap:" << nl;
    for (const auto& v : h.ordered()) {
      cout << v << "  ";
    }
    cout << "\n------" << endl;

    char t;
    cin >> t;
    if (t == '+') {
      int n;
      cin >> n;
      h.push(n);
    } else if (t == '-') {
      h.pop();
    } else if (t == '!') {
      break;
    }
  }
}
