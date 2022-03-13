#include <bits/stdc++.h>
using namespace std;

#include "../aho_corasick.h"

constexpr char nl = '\n';

void sanity_check(const string& s) {
  for(char c : s) {
    assert(' ' <= c);
  }
}

struct to_int_t {
  static constexpr int size() { return 128 - 32; }
  int operator() (int c) const { return c - ' '; }
};

int main() {
  cin.tie(0)->sync_with_stdio(0);

  for(string s; getline(cin, s); ){
    int n = stoi(s);
    aho_corasick<char, to_int_t> ac;
    for(int i=0; i<n; i++) {
      getline(cin, s);
      sanity_check(s);
      ac.add(s);
    }
    ac.build();

    getline(cin, s);
    sanity_check(s);
    vector<vector<int>> ans = ac.find_all(s);
    for(int i=0; i<n; i++) {
      for(int j : ans[i]) {
        cout << j << " ";
      }
      cout << nl;
    }
  }

  return 0;
}
