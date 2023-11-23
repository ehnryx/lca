/* Suffix Tree (online Ukkonen's Algorithm)
 * USAGE
 *  ukkonen<T> tree(string_s); or
 *  ukkonen<T> tree; tree.insert(character);
 * TIME
 *  O(N) amortized
 *  N = |string|
 * STATUS
 *  untested
 */
#pragma once

#include <array>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

template <typename to_int>
struct suffix_node {
  static constexpr size_t npos = -1;
  // array
  size_t parent;
  size_t slink;                         // suffix link from end of range
  size_t left, right;                   // substring range
  std::array<size_t, to_int::size> to;  // children
  int depth;  // depth of node, measured in number of chars before node
  size_t length() const { return right - left; }
  suffix_node() : parent(-1), slink(-1), left(-1), right(-1), depth(0) {
    fill(to.begin(), to.end(), -1);
  }
  bool has_neighbour(int c) const { return to[c] != npos; }
  template <typename T>
  size_t get(const T& c) const {
    return to[to_int::toi(c)];
  }

  struct iterator {
    suffix_node const* ref;
    int i;
    iterator(suffix_node const* _ref, int pos) : ref(_ref), i(pos) {
      while (i < to_int::size && ref->to[i] == (size_t)-1) {
        i += 1;
      }
    }
    auto operator*() const { return pair(to_int::toc(i), ref->to[i]); }
    iterator& operator++() { return *this = iterator(ref, i + 1); }
    iterator& operator++(int) { return *this = iterator(ref, i + 1); }
    bool operator==(const iterator& o) const { return i == o.i; }
    bool operator!=(const iterator& o) const { return i != o.i; }
  };
  iterator begin() const { return iterator(this, 0); }
  iterator end() const { return iterator(this, to_int::size); }
};

template <>
struct suffix_node<void> {
  static constexpr size_t npos = -1;
  // map
  size_t parent;
  size_t slink;
  size_t left, right;
  std::map<int, size_t> to;
  int depth;
  size_t length() const { return right - left; }
  suffix_node() : parent(-1), slink(-1), left(-1), right(-1), depth(0) {}
  bool has_neighbour(int c) const { return to.count(c); }
  size_t get(int c) const { return to.at(c); }
};

template <class to_int = void>
struct ukkonen {
  static constexpr size_t npos = suffix_node<to_int>::npos;

  std::vector<suffix_node<to_int>> nodes;
  size_t cur_node, cur_pos;
  std::basic_string<int> t;

  ukkonen(size_t reserve_length = 0) : nodes(1), cur_node(0), cur_pos(0) {
    nodes[0].left = nodes[0].right = 0;  // root is 0
    nodes.reserve(2 * reserve_length + 1);
    t.reserve(reserve_length);
  }

  template <typename T>
  ukkonen(const std::basic_string<T>& s) : ukkonen(s.size() + 1) {
    for (size_t i = 0; i < s.size(); i++) {
      ukk_add(convert(s[i]), i);
    }
    ukk_add(0, s.size());
    finalize(s.size());
  }

  template <typename T>
  int convert(T c) const {
    if constexpr (std::is_void_v<to_int>) {
      if (c == 0) throw std::invalid_argument("0 is reserved for the terminal");
      return (int)c;
    } else {
      int converted = to_int::toi(c);
      if (converted == 0) throw std::invalid_argument("0 is reserved for the terminal");
      return converted;
    }
  }

  std::pair<size_t, size_t> get_next_pos(size_t idx, size_t pos) {
    /*/ quick check
    if (pos == nodes[idx].right && nodes[idx].slink != npos) {
      return pair(nodes[idx].slink, nodes[nodes[idx].slink].right);
    }*/

    size_t len = pos - nodes[idx].left;
    size_t new_idx = nodes[idx].parent;
    if (new_idx == root()) {
      len -= 1;
    } else {
      new_idx = nodes[new_idx].slink;
    }

    size_t new_pos = nodes[new_idx].right;
    while (len > 0) {
      new_idx = nodes[new_idx].to[t[pos - len]];
      if (len <= nodes[new_idx].length()) {
        new_pos = nodes[new_idx].left + len;
        len = 0;
      } else {
        len -= nodes[new_idx].length();
      }
    }
    return std::pair(new_idx, new_pos);
  }

  void ukk_add(int c, size_t index) {
    t.push_back(c);

    size_t old_node = -1;
    while (true) {
      // extend a leaf
      if (cur_pos == index && cur_node != root()) {
        if (nodes[cur_node].parent == root() && cur_pos == nodes[cur_node].left) {
          break;  // single-char branch means that this phase is done
        }
        // just go to the next position
        std::tie(cur_node, cur_pos) = get_next_pos(cur_node, cur_pos);
        continue;
      }

      // in the middle of a segment, need to split
      if (cur_pos < nodes[cur_node].right) {
        // break if letter exists already
        if (t[cur_pos] == c) {
          break;
        }
        // split segment
        size_t mid = nodes.size();
        nodes.resize(nodes.size() + 1);
        size_t par = nodes[cur_node].parent;
        // connect par -> mid -> cur
        nodes[par].to[t[nodes[cur_node].left]] = mid;
        nodes[mid].to[t[cur_pos]] = cur_node;
        nodes[mid].parent = par;
        nodes[cur_node].parent = mid;
        // update substring range
        nodes[mid].left = nodes[cur_node].left;
        nodes[mid].right = nodes[cur_node].left = cur_pos;
        // update depth
        nodes[mid].depth = nodes[cur_node].depth;
        nodes[cur_node].depth += (int)nodes[mid].length();
        // update cur_node
        cur_node = mid;
      }

      // update suffix link if necessary
      if (old_node != npos) {
        nodes[old_node].slink = cur_node;
      }
      old_node = cur_node;

      // break if letter exists already
      if (nodes[cur_node].has_neighbour(c)) {
        break;
      }

      // add new leaf
      size_t leaf = nodes.size();
      nodes.resize(nodes.size() + 1);
      nodes[cur_node].to[c] = leaf;
      nodes[leaf].parent = cur_node;
      nodes[leaf].left = index;
      nodes[leaf].depth = nodes[cur_node].depth + (int)nodes[cur_node].length();

      // check if done (just added leaf to root)
      if (cur_node == root()) {
        break;
      }
      // walk to the next position
      std::tie(cur_node, cur_pos) = get_next_pos(cur_node, cur_pos);
    }

    // increment cur_pos
    if (cur_pos == nodes[cur_node].right) {
      cur_node = nodes[cur_node].to[c];
      cur_pos = nodes[cur_node].left + 1;
    } else {
      cur_pos += 1;
    }
  }

  void finalize(size_t last) {
    for (suffix_node<to_int>& u : nodes) {
      u.right = min(u.right, last + 1);
      assert(u.left <= u.right);
    }
  }

  // BEGIN suffix tree functions
  //  length of the string
  int size() const { return (int)t.size(); }
  // the root of the suffix tree
  static constexpr int root() { return 0; }
  // the depth of the start of a vertex (the number of characters above it)
  int depth(int u) const { return nodes[u].depth; }
  // the parent of a vertex
  int parent(int u) const { return nodes[u].parent; }
  // the length of the corresponding substring
  int length(int u) const { return (int)nodes[u].length(); }
  // whether a vertex is a leaf
  bool is_leaf(int u) const { return nodes[u].right >= t.size(); }
  // the nodes in order, use .get(char) to match a character
  const suffix_node<to_int>& operator[](int i) const { return nodes.at(i); }

  // return: (node, idx in range[node]) of the past-the-end of the match.
  //         (-1, -1) if not matched
  template <typename T>
  std::pair<int, int> match(const std::basic_string<T>& s) const {
    int u = root();
    int idx = 0;
    for (size_t i = 0; i < s.size(); i++) {
      if (idx == (int)nodes[u].right) {
        u = (int)nodes[u].get(s[i]);
        if (u == -1) {
          return std::pair(-1, -1);
        }
        idx = (int)nodes[u].left;
      }
      if (convert(s[i]) == t[idx]) {
        idx++;
      } else {
        return std::pair(-1, -1);
      }
    }
    return std::pair(u, idx);
  }
  // END suffix tree functions
};
