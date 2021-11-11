/* Euler Tour Tree
 * USAGE
 *  ???
 * TIME
 *  O(logN) per operation amortized (?)
 *  N = |tree|
 * STATUS
 *  untested untested UNTESTED
 */
#pragma once

#include "../data_structure/splay_tree.h"
#include "../misc/simple_memory_pool.h"

template <class node_t>
struct euler_tour_tree : splay_tree<node_t> {
  using base = splay_tree<node_t>;
  using base::nil, base::splay, base::set_child, base::pull;
  using base::walk_left, base::walk_right, base::rank;

  using edge_node_t = splay_node<int, node_t*>;
  simple_memory_pool<node_t> memory;
  simple_memory_pool<edge_node_t> edge_memory;
  vector<node_t*> ref_node;
  vector<splay_tree_shared_memory<edge_node_t>> edges;
  node_t dummy;
  euler_tour_tree(int n): base(), memory(3 * n), edge_memory(2 * n),
    ref_node(n), edges(n), dummy(*nil) {
    for (int i = 0; i < n; i++) {
      ref_node[i] = new_ett_node(true);
      edges[i].shared_memory = &edge_memory;
    }
  }
  ~euler_tour_tree() { base::root = nil; }
  euler_tour_tree(const euler_tour_tree&) = delete;
  euler_tour_tree& operator = (const euler_tour_tree&) = delete;

  node_t* new_ett_node(bool ref) {
    node_t* x = new (memory.allocate()) node_t();
    x->set_ref(ref);
    x->size = 1;
    return x;
  }

  int size() const { return (int)edges.size(); }
  int size(int i) { return splay(ref_node[i])->ref_cnt(); }
  node_t& operator [] (int i) { return *ref_node[i]; }
  node_t* ptr(int i) { return ref_node[i]; }

  template <class... Args>
  void init(int i, const Args&... args) {
    new (ref_node[i]) node_t(args...);
    ref_node[i]->set_ref(true);
  }

  node_t* splay(int u) {
    return splay(ref_node[u]);
  }

  void reroot(int u) {
    _reroot(ref_node[u]);
  }
  node_t* _reroot(node_t* u) {
    if (splay(u)->left == nil) return u; // nothing to do
    // disconnect left side
    node_t* v = u->left;
    v->parent = u->left = nil;
    pull(u);
    // append left to back
    v = splay(walk_left(v));
    set_child(v, u, true);
    pull(v);
    return v; // root of splay tree != root of tree
  }

  void link(int u_id, int v_id) {
    node_t* u = _reroot(ref_node[u_id]);
    node_t* v = _reroot(ref_node[v_id]);
    // insert u_id to front of v
    node_t* add_u = new_ett_node(false);
    set_child(add_u, u, true);
    set_child(add_u, v, false);
    pull(add_u);
    edges[u_id][v_id] = add_u;
    // append v_id to end of v
    node_t* add_v = new_ett_node(false);
    set_child(add_v, add_u, true);
    pull(add_v);
    edges[v_id][u_id] = add_v;
  }

  void cut(int u_id, int v_id) {
    auto e_uv = edges[u_id].find(v_id);
    if (e_uv == edges[u_id].nil) return; // edge does not exist
    auto e_vu = edges[v_id].find(u_id);
    node_t* a = e_uv->value;
    node_t* b = e_vu->value;
    edges[u_id].erase(e_uv);
    edges[v_id].erase(e_vu);
    if (rank(b) < rank(a)) {
      swap(a, b);
    }
    // disconnect a
    node_t* top = splay(a)->left;
    top->parent = nil;
    a->right->parent = nil;
    memory.deallocate(a);
    // disconnect b
    node_t* bot = splay(b)->right;
    a = b->left;
    a->parent = bot->parent = nil;
    memory.deallocate(b);
    // join top and bot
    if (top != nil && bot != nil) {
      bot = splay(walk_left(bot));
      set_child(bot, top, true);
      pull(bot); // uhh this is actually root
    }
  }

  template <class... Args>
  void update_all(int x, const Args&... args) {
    splay(ref_node[x])->put(args...);
  }

  template <class... Args>
  auto query_all(int x, const Args&... args) {
    return splay(ref_node[x])->get(args...);
  }

  template <class... Args>
  void update_subtree(int x, int p, const Args&... args) {
    node_t* e_xp = edges[x][p];
    node_t* e_px = edges[p][x];
    if (rank(e_px) < rank(e_xp)) {
      base::update_range(e_px, e_xp, args...);
    } else {
      base::update_range(e_px, nil, args...);
      base::update_range(nil, e_xp, args...);
    }
  }

  template <class... Args>
  auto query_subtree(int x, int p, const Args&... args) {
    node_t* e_xp = edges[x][p];
    node_t* e_px = edges[p][x];
    if (rank(e_px) < rank(e_xp)) {
      return base::query_range(e_px, e_xp);
    } else {
      e_px = base::range(e_px, nil);
      e_xp = base::range(nil, e_xp);
      dummy.left = e_px;
      dummy.right = e_xp;
      dummy.pull();
      return dummy.get();
    }
  }
};

template <typename derived_t, typename value_t>
struct euler_tour_node : splay_node_base<derived_t, void, value_t> {
  using base = splay_node_base<derived_t, void, value_t>;
  using base::nil, base::left, base::right;
  int _ref;
  euler_tour_node(): base(), _ref(0) {}
  euler_tour_node(const value_t& v): base(v), _ref(0) {}
  void set_ref(bool r) { _ref = r ? 0b11 : 0; }
  bool ref() const { return _ref & 1; }
  int ref_cnt() const { return _ref >> 1; }
  void pull() {
    static constexpr int mask = ~(int)1;
    _ref = (_ref & 1) + ((_ref & 1) << 1)
        + (left->_ref & mask) + (right->_ref & mask);
  }
};

