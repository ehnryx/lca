/* Link Cut Tree
 * USAGE
 *  ???
 * TIME
 *  O(logN) per operation amortized (?)
 *  N = |link cut tree|
 * STATUS
 *  untested untested UNTESTED
 */
#pragma once

#include "splay_tree.h"

template <class node_t>
struct link_cut_tree : splay_tree<node_t> {
  using base = splay_tree<node_t>;
  using base::nil, base::splay, base::set_child, base::walk_left, base::pull;
  vector<node_t> data;
  link_cut_tree(int n): data(n) {
    for (int i = 0; i < n; i++) {
      data[i].size = 1;
    }
  }
  ~link_cut_tree() { base::root = nil; }
  link_cut_tree(const link_cut_tree&) = delete;
  link_cut_tree& operator = (const link_cut_tree&) = delete;

  int size() const { return (int)data.size(); }
  node_t& operator [] (int i) { return data[i]; }

  template <class... Args>
  void init(int i, const Args&... args) {
    new (&data[i]) node_t(args...);
  }

  node_t* access(int u) { return access(&data[u]); }
  node_t* access(node_t* u) {
    if (node_t* x = splay(u)->chain_parent; x != nil) {
      set_child(access(x), u, false);
    }
    splay(u);
    assert(u->chain_parent == nil);
    if (u->right != nil) {
      set_child(nil, u->right, true);
      u->right->chain_parent = u;
      u->right->push();
      u->right = nil;
    }
    pull(u);
    return u;
  }

  void link(int u, int v) { return link(&data[u], &data[v]); }
  void link(node_t* u, node_t* v) {
    set_child(access(u), reroot(v), false);
  }

  void cut(int u, int v) { return cut(&data[u], &data[v]); }
  void cut(node_t* u, node_t* v) {
    reroot(u);
    if (splay(v)->chain_parent == nil) {
      set_child(nil, u, true);
      v->left = nil;
    } else {
      assert(v->chain_parent == u);
      v->chain_parent = nil;
    }
    pull(v);
  }

  int lca(int u, int v) {
    return (int)(lca(&data[u], &data[v]) - &data[0]);
  }
  node_t* lca(node_t* u, node_t* v) {
    access(u);
    access(v);
    return u->chain_parent == nil ? u : u->chain_parent;
  }

  int find_root(int u) {
    return (int)(find_root(&data[u]) - &data[0]);
  }
  node_t* find_root(node_t* u) {
    return splay(walk_left(access(u)));
  }

  node_t* reroot(int u) { return reroot(&data[u]); }
  node_t* reroot(node_t* u) {
    access(u)->reverse();
    u->push();
    return u;
  }

  int parent(int u) {
    node_t* x = parent(&data[u]);
    return x == nil ? -1 : (int)(x - &data[0]);
  }
  node_t* parent(node_t* u) {
    return base::prev(access(u));
  }

  template <class... Args>
  void update_path_to_root(int u, const Args&... args) {
    update_path_to_root(&data[u], args...);
  }
  template <class... Args>
  void update_path_to_root(node_t* u, const Args&... args) {
    access(u)->put(args...);
    splay(u);
  }

  template <class... Args>
  auto query_path_to_root(int u, const Args&... args) {
    return query_path_to_root(&data[u], args...);
  }
  template <class... Args>
  auto query_path_to_root(node_t* u, const Args&... args) {
    return access(u)->get(args...);
  }

  // will reroot
  template <class... Args>
  void update_path(int u, int v, const Args&... args) {
    update_path(&data[u], &data[v], args...);
  }
  template <class... Args>
  void update_path(node_t* u, node_t* v, const Args&... args) {
    reroot(u);
    access(v)->put(args...);
    splay(v);
  }

  // will reroot
  template <class... Args>
  auto query_path(int u, int v, const Args&... args) {
    return query_path(&data[u], &data[v], args...);
  }
  template <class... Args>
  auto query_path(node_t* u, node_t* v, const Args&... args) {
    reroot(u);
    return access(v)->get(args...);
  }
};

template <typename derived_t, typename value_t>
struct link_cut_node : splay_node_base<derived_t, void, value_t> {
  using base = splay_node_base<derived_t, void, value_t>;
  using base::nil, base::left, base::right;
  derived_t* chain_parent;
  bool rev;
  link_cut_node(): base(), chain_parent(nil), rev(false) {}
  link_cut_node(const value_t& v): base(v), chain_parent(nil), rev(false) {}
  void reverse() {
    swap(left, right);
    rev ^= 1;
  }
  void push() {
    if (left != nil) {
      left->chain_parent = chain_parent;
      if (rev) left->reverse();
    }
    if (right != nil) {
      right->chain_parent = chain_parent;
      if (rev) right->reverse();
    }
    rev = false;
  }
};

