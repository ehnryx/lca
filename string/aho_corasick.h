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

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

template <typename T, typename to_int_t = void>
struct aho_corasick {
  static_assert(!std::is_void_v<to_int_t>);
  struct node {
    int parent, fail, match, pattern;
    std::array<int, to_int_t::size> child;
    node(int p): parent(p), fail(-1), match(-1), pattern(-1) {
      fill(begin(child), end(child), -1);
    }
    void for_each_child(auto&& func) const {
      for (int i = 0; i < to_int_t::size; i++) {
        if (child[i] != -1) {
          func(child[i], i);
        }
      }
    }
  };
  static constexpr int root = 0;
  std::vector<node> nodes;
  int num_patterns = 0;
  int size() const { return (int)nodes.size(); }
  const node& operator[](int u) const { return nodes[u]; }

  aho_corasick(int r = 0) {
    nodes.reserve(r);
    nodes.emplace_back(-1);  // root
  }
  aho_corasick(const std::vector<std::basic_string<T>>& patterns, int r = 0) {
    nodes.reserve(r);
    nodes.emplace_back(-1);  // root
    for (const std::basic_string<T>& s : patterns) {
      add(s);
    }
    build();
  }

  void add(const std::basic_string<T>& s) {
    int u = root;  // 0 is root
    for (const T c : s) {
      const int ci = to_int_t()(c);
      if (nodes[u].child[ci] == -1) {
        nodes[u].child[ci] = (int)nodes.size();
        nodes.emplace_back(u);
      }
      u = nodes[u].child[ci];
    }
    if (nodes[u].pattern != -1) {
      throw std::logic_error("aho_corasick does not support duplicates");
    }
    nodes[u].pattern = num_patterns++;
  }

  void build() {
    std::queue<std::pair<int, int>> bfs;
    bfs.emplace(-1, root);
    while (!bfs.empty()) {
      const auto [c, u] = bfs.front();
      bfs.pop();
      for (int i = 0; i < to_int_t::size; i++) {
        if (nodes[u].child[i] != -1) {
          bfs.emplace(i, nodes[u].child[i]);
        }
      }
      nodes[u].fail = nodes[u].match = root;
      if (u != root) {
        int v = nodes[nodes[u].parent].fail;
        while (v != root && nodes[v].child[c] == -1) {
          v = nodes[v].fail;
        }
        if (nodes[v].child[c] != -1 && nodes[v].child[c] != u) {
          nodes[u].fail = nodes[v].child[c];
        }
        nodes[u].match = (nodes[u].pattern != -1 ? u : nodes[nodes[u].fail].match);
      }
    }
  }

  vector<vector<int>> build_fail_tree() const {
    vector<vector<int>> fail_tree(nodes.size());
    for (int i = 1; i < nodes.size(); i++) {
      fail_tree[nodes[i].fail].push_back(i);
    }
    return fail_tree;
  }

  std::vector<int> get_patterns(int u) {
    std::vector<int> patterns;
    for (int v = nodes[u].match; v != root; v = nodes[nodes[v].fail].match) {
      patterns.push_back(nodes[v].pattern);
    }
    return patterns;
  }

  // func(pattern_id, index_of_end);
  void find_all(const std::string& s, auto&& func) {
    find_ends(s, [&](int node_id, int s_id) {
        for (int pattern : get_patterns(node_id)) {
          func(pattern, s_id);
        }
    });
  }

  // func(node_id, index_of_end);
  void find_ends(const std::string& s, auto&& func) {
    int u = root;
    for (int i = 0; i < (int)s.size(); i++) {
      const int c = to_int_t()(s[i]);
      while (u != root && nodes[u].child[c] == -1) {
        u = nodes[u].fail;
      }
      if (nodes[u].child[c] != -1) {
        u = nodes[u].child[c];
      }
      func(u, i);
    }
  }
};

template <typename T>
struct aho_corasick<T, void> {
  struct node {
    int parent, fail, match, pattern;
    std::unordered_map<T, int> child;
    node(int p): parent(p), fail(-1), match(-1), pattern(-1) {}
    void for_each_child(auto&& func) const {
      for (auto [c, v] : child) {
        func(v, c);
      }
    }
  };
  static constexpr int root = 0;
  std::vector<node> nodes;
  int num_patterns = 0;
  int size() const { return (int)nodes.size(); }
  const node& operator[](int u) const { return nodes[u]; }

  aho_corasick(int r = 0) {
    nodes.reserve(r);
    nodes.emplace_back(-1);  // root
  }
  aho_corasick(const std::vector<std::basic_string<T>>& patterns, int r = 0) {
    nodes.reserve(r);
    nodes.emplace_back(-1);  // root
    for (const std::basic_string<T>& s : patterns) {
      add(s);
    }
    build();
  }

  void add(const std::basic_string<T>& s) {
    int u = root;  // 0 is root
    for (const T c : s) {
      if (auto it = nodes[u].child.find(c); it != nodes[u].child.end()) {
        u = it->second;
      } else {
        auto nxt = nodes[u].child.emplace(c, (int)nodes.size()).first;
        nodes.emplace_back(u);
        u = nxt->second;
      }
    }
    if (nodes[u].pattern != -1) {
      throw std::logic_error("aho_corasick does not support duplicates");
    }
    nodes[u].pattern = num_patterns++;
  }

  void build() {
    std::queue<std::pair<T, int>> bfs;
    bfs.emplace(-1, root);
    while (!bfs.empty()) {
      const auto [c, u] = bfs.front();
      bfs.pop();
      for (const auto& child : nodes[u].child) {
        bfs.push(child);
      }
      nodes[u].fail = nodes[u].match = root;
      if (u != root) {
        int v = nodes[nodes[u].parent].fail;
        while (true) {
          if (auto it = nodes[v].child.find(c); it != nodes[v].child.end()) {
            if (it->second != u) {
              nodes[u].fail = it->second;
            }
            break;
          } else if (v == root) {
            break;
          }
          v = nodes[v].fail;
        }
        nodes[u].match = (nodes[u].pattern != -1 ? u : nodes[nodes[u].fail].match);
      }
    }
  }

  vector<vector<int>> build_fail_tree() const {
    vector<vector<int>> fail_tree(nodes.size());
    for (int i = 1; i < nodes.size(); i++) {
      fail_tree[nodes[i].fail].push_back(i);
    }
    return fail_tree;
  }

  std::vector<int> get_patterns(int u) {
    std::vector<int> patterns;
    for (int v = nodes[u].match; v != root; v = nodes[nodes[v].fail].match) {
      patterns.push_back(nodes[v].pattern);
    }
    return patterns;
  }

  // func(pattern_id, index_of_end);
  void find_all(const std::string& s, auto&& func) {
    find_ends(s, [&](int node_id, int s_id) {
        for (int pattern : get_patterns(node_id)) {
          func(pattern, s_id);
        }
    });
  }

  // func(node_id, index_of_end);
  void find_ends(const std::string& s, auto&& func) {
    int u = root;
    for (int i = 0; i < (int)s.size(); i++) {
      while (true) {
        if (auto it = nodes[u].child.find(s[i]); it != nodes[u].child.end()) {
          u = it->second;
          break;
        } else if (u == root) {
          break;
        }
        u = nodes[u].fail;
      }
      func(u, i);
    }
  }
};

