/* Aho Corasick String Matching
 * USAGE
 *  aho_corasick<T, to_int> kmp(patterns);
 *  kmp.match(s);
 * TIME
 *  O(|s|) ish
 * STATUS
 *  untested: kattis/stringmultimatching
 */
#pragma once

template <typename T, typename to_int_t = void>
struct aho_corasick {
  static_assert(!is_void_v<to_int_t>);
  struct node {
    node* parent;
    node* fail;
    node* match;
    array<node*, to_int_t::size()> child;
    int id;
    node(node* p): parent(p), fail(nullptr), match(nullptr), id(-1) {
      fill(begin(child), end(child), nullptr);
    }
  };
  node* root;
  vector<int> pattern_length, same_pattern;
  int pattern_cnt;

  aho_corasick(): root(new node(nullptr)), pattern_cnt(0) {}
  aho_corasick(const vector<basic_string<T>>& patterns):
    root(new node(nullptr)), pattern_cnt(0) {
    for (const basic_string<T>& s : patterns) {
      add(s);
    }
    build();
  }

  void add(const basic_string<T>& s) {
    node* u = root;
    for (T c : s) {
      if (!u->child[to_int_t()(c)]) {
        u->child[to_int_t()(c)] = new node(u);
      }
      u = u->child[to_int_t()(c)];
    }
    same_pattern.push_back(u->id);
    u->id = pattern_cnt++;
    pattern_length.push_back((int)s.size());
  }

  void build() {
    queue<pair<int, node*>> bfs;
    bfs.emplace(-1, root);
    while (!bfs.empty()) {
      auto [c, u] = bfs.front();
      bfs.pop();
      for (int i = 0; i < to_int_t::size(); i++) {
        if (u->child[i]) {
          bfs.emplace(i, u->child[i]);
        }
      }
      u->fail = u->match = root;
      if (u != root) {
        node* v = u->parent->fail;
        while (v != root && !v->child[c]) {
          v = v->fail;
        }
        if (v->child[c] != nullptr && v->child[c] != u) {
          u->fail = v->child[c];
        }
        u->match = (u->id == -1 ? u->fail->match : u);
      }
    }
  }

  vector<vector<int>> find_all(const string& s) {
    vector<vector<int>> matches(pattern_cnt);
    node* u = root;
    for (int i = 0; i < (int)s.size(); i++) {
      int c = to_int_t()(s[i]);
      while (u != root && !u->child[c]) {
        u = u->fail;
      }
      if (u->child[c] != nullptr) {
        u = u->child[c];
      }
      for (node* v = u->match; v != root; v = v->fail->match) {
        for (int id = v->id; id != -1; id = same_pattern[id]) {
          matches[id].push_back(i - pattern_length[id] + 1);
        }
      }
    }
    return matches;
  }
};

template <typename T>
struct aho_corasick<T, void> {
  struct node {
    node* parent;
    node* fail;
    node* match;
    unordered_map<T, node*> child;
    int id;
    node(node* p): parent(p), fail(nullptr), match(nullptr), id(-1) {}
  };
  node* root;
  vector<int> pattern_length, same_pattern;
  int pattern_cnt;

  aho_corasick(): root(new node(nullptr)), pattern_cnt(0) {}
  aho_corasick(const vector<basic_string<T>>& patterns):
    root(new node(nullptr)), pattern_cnt(0) {
    for (const basic_string<T>& s : patterns) {
      add(s);
    }
    build();
  }

  void add(const basic_string<T>& s) {
    node* u = root;
    for (T c : s) {
      if (!u->child.count(c)) {
        u->child[c] = new node(u);
      }
      u = u->child[c];
    }
    same_pattern.push_back(u->id);
    u->id = pattern_cnt++;
    pattern_length.push_back((int)s.size());
  }

  void build() {
    queue<pair<T, node*>> bfs;
    bfs.emplace(-1, root);
    while (!bfs.empty()) {
      auto [c, u] = bfs.front();
      bfs.pop();
      for (const auto& child : u->child) {
        bfs.push(child);
      }
      u->fail = u->match = root;
      if (u != root) {
        node* v = u->parent->fail;
        while (v != root && !v->child.count(c)) {
          v = v->fail;
        }
        if (auto it = v->child.find(c); it != v->child.end() && it->second != u) {
          u->fail = it->second;
        }
        u->match = (u->id == -1 ? u->fail->match : u);
      }
    }
  }

  vector<vector<int>> find_all(const string& s) {
    vector<vector<int>> matches(pattern_cnt);
    node* u = root;
    for (int i = 0; i < (int)s.size(); i++) {
      while (u != root && !u->child.count(s[i])) {
        u = u->fail;
      }
      if (auto it = u->child.find(s[i]); it != u->child.end()) {
        u = it->second;
      }
      for (node* v = u->match; v != root; v = v->fail->match) {
        for (int id = v->id; id != -1; id = same_pattern[id]) {
          matches[id].push_back(i - pattern_length[id] + 1);
        }
      }
    }
    return matches;
  }
};

