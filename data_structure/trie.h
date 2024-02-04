/* Trie
 * USAGE
 *  trie<T> store(); // empty trie
 *  store.insert(value); -> pair(ptr, new?)
 *  store.find(value); -> ptr
 *  struct node {
 *    map<T, ptr> child;
 *    int count;
 *  };
 * TIME
 *  O(N) per query
 *  N = |string to insert|
 * STATUS
 *  tested: cf/474f,102875a,1515i, acmhdu/5306
 */
#pragma once

#include <map>

template <typename T>
struct trie {
  struct node {
    std::map<T, node*> child;
    int count = 0;
    int end = 0;
  };
  node* root;
  trie(): root(new node) {}
  template <template <typename> typename Container>
  std::pair<node*, bool> insert(Container<T> const& val) {
    node* u = root;
    u->count += 1;
    for (T const& v : val) {
      if (auto it = u->child.find(v); it != u->child.end()) {
        u = it->second;
      } else {
        it = u->child.emplace(v, new node).first;
        u = it->second;
      }
      u->count += 1;
    }
    u->end += 1;
    return std::pair(u, u->end == 1);
  }
  template <template <typename> typename Container>
  node* find(Container<T> const& val) const {
    node* u = root;
    for (T const& v : val) {
      if (auto it = u->child.find(v); it != u->child.end()) {
        u = it->second;
      } else {
        return nullptr;
      }
    }
    return u->end ? u : nullptr;
  }
};
