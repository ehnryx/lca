/* Splay Tree
 * USAGE
 *  pray
 * TIME
 *  O(logN) per operation amortized
 *  N = |splay tree|
 * STATUS
 *  somewhat tested
 */
#pragma once

#include "../misc/static_memory_pool.h"
#include "../misc/simple_memory_pool.h"
#include "../misc/member_function_checker.h"

template <class node_t, int max_size, typename = void>
struct splay_tree_memory_base {
  splay_tree_memory_base(simple_memory_pool<node_t>*) {}
};

template <class node_t, int max_size>
struct splay_tree_memory_base<node_t, max_size, enable_if_t<max_size == -1>> {
  simple_memory_pool<node_t>* shared_memory;
  splay_tree_memory_base(simple_memory_pool<node_t>* shm): shared_memory(shm) {}
};

template <class node_t, int max_size = 0>
struct splay_tree : splay_tree_memory_base<node_t, max_size> {
  MEMBER_FUNCTION_CHECKER(push);
  MEMBER_FUNCTION_CHECKER(pull);
  static constexpr bool has_push = _has_push<node_t>::value;
  static constexpr bool has_pull = _has_pull<node_t>::value;
  static constexpr int mem_size = max(0, max_size);
  static node_t* const nil;

  static static_memory_pool<node_t, mem_size> memory;
  using memory_base = splay_tree_memory_base<node_t, max_size>;

  template <class... Args>
  inline node_t* new_node(Args&&... args) {
    if constexpr (max_size == 0) {
      return new node_t(forward<Args>(args)...);
    } else if constexpr (max_size == -1) {
      return new (memory_base::shared_memory->allocate()) node_t(forward<Args>(args)...);
    } else {
      return new (memory.allocate()) node_t(forward<Args>(args)...);
    }
  }
  inline void del_node(node_t* x) {
    if constexpr (max_size == 0) {
      delete x;
    } else if constexpr (max_size == -1) {
      memory_base::shared_memory->deallocate(x);
    } else {
      memory.deallocate(x);
    }
  }

  node_t* root;
  splay_tree(simple_memory_pool<node_t>* shm = nullptr):
    memory_base(shm), root(node_t::_get_nil()) {}
  splay_tree(int n, simple_memory_pool<node_t>* shm = nullptr):
    memory_base(shm), root(node_t::_get_nil()) {
    for (int i = 0; i < n; i++) {
      node_t* to_add = new_node();
      to_add->size = 1;
      _push_back(to_add);
    }
  }
  ~splay_tree() { erase_tree(root); }
  void erase_tree(node_t* x) {
    if (x == nil) return;
    erase_tree(x->left);
    erase_tree(x->right);
    del_node(x);
  }

  void clear() {
    erase_tree(root);
    root = nil;
  }

  splay_tree dislodge() {
    return splay_tree(move(*this));
    /*
    node_t* ret = root;
    root = nil;
    return splay_tree(ret);
    */
  }

  friend void swap(splay_tree& a, splay_tree& b) {
    swap(a.root, b.root);
  }

  splay_tree(splay_tree&& o): memory_base(move(o)), root(o.root) {
    o.root = nil;
  }
  splay_tree(const splay_tree& o): memory_base(o), root(node_t::_get_nil()) {
    o.copy_to(*this);
  }
  splay_tree& operator = (const splay_tree& o) {
    root = nil;
    o.copy_to(*this);
    return *this;
  }

  template <int other_size>
  void copy_to(splay_tree<node_t, other_size>& other) const {
    other.erase_tree(other.root);
    if (root == nil) other.root = nil;
    else other.root = copy_to_rec(other, root);
  }
  template <int other_size>
  node_t* copy_to_rec(splay_tree<node_t, other_size>& other, node_t* x) const {
    node_t* cur = other.new_node(*x);
    if (x->left != nil) {
      cur->left = copy_to_rec(other, x->left);
      cur->left->parent = cur;
    }
    if (x->right != nil) {
      cur->right = copy_to_rec(other, x->right);
      cur->right->parent = cur;
    }
    return cur;
  }

  int size() const { return root->size; }
  bool empty() const { return root == nil; }
  int rank(node_t* x) { return splay(x) == nil ? size() : x->left->size; }

  node_t* splay(node_t* x) {
    if (x == nil) return x;
    if constexpr (has_push) push_down_to(x);
    _splay_rec(x);
    pull(x);
    return root = x; // x has no lazy
  }

  node_t* splay_to(node_t* x, node_t* to) {
    if (x == nil) return x;
    if (to == nil) return splay(x);
    if constexpr (has_push) push_down_to(x);
    _splay_to_rec(x, to);
    pull(x);
    return x; // x has no lazy
  }

  void set_child(node_t* parent, node_t* child, bool left) {
    if (child != nil) {
      child->parent = parent;
    }
    if (parent != nil) {
      if (left) parent->left = child;
      else parent->right = child;
    }
  }
  bool is_left_child(node_t* x) const {
    return x != nil && x->is_left_child();
  }

  //---------------------------------------------------------------------------
  // iterators

  template <bool forward_it>
  struct iterator {
    const splay_tree* tree_ptr;
    node_t* ptr;
    iterator(): tree_ptr(nil), ptr(nil) {}
    iterator(const splay_tree* t, node_t* x): tree_ptr(t), ptr(x) {}
    iterator(const iterator<!forward_it> rev): tree_ptr(rev.tree_ptr), ptr(rev.ptr) {}
    operator node_t* () const { return ptr; }
    node_t* operator -> () const { return ptr; }
    typename node_t::out_t operator * () const { return ptr->get_value(); }
    iterator& operator ++ () {
      ptr = forward_it ? tree_ptr->next(ptr) : tree_ptr->prev(ptr);
      return *this;
    }
    iterator& operator ++ (int) { return operator ++ (); }
    iterator& operator -- () { return *this = ++iterator<!forward_it>(*this); }
    iterator& operator -- (int) { return operator -- (); }
    template <bool dir>
    bool operator == (const iterator<dir>& other) const { return ptr == other.ptr; }
    template <bool dir>
    bool operator != (const iterator<dir>& other) const { return ptr != other.ptr; }
    bool operator == (const node_t* other) const { return ptr == other; }
    bool operator != (const node_t* other) const { return ptr != other; }
  };
  iterator<true> begin() const {
    return iterator<true>(this, root != nil ? walk_left(root) : root);
  }
  iterator<true> end() const {
    return iterator<true>(this, nil);
  }
  iterator<false> rbegin() const {
    return iterator<false>(this, root != nil ? walk_right(root) : root);
  }
  iterator<false> rend() const {
    return iterator<false>(this, nil);
  }
  template <bool dir> friend iterator<dir> next(iterator<dir> it) { return ++it; }
  template <bool dir> friend iterator<dir> prev(iterator<dir> it) { return --it; }

  //---------------------------------------------------------------------------
  // binary search tree operations

  template <typename key_t>
  bool contains(const key_t& key) {
    return find(key) != nil;
  }

  template <typename key_t>
  int count(const key_t& key) {
    return rank(upper_bound(key)) - rank(lower_bound(key));
  }

  template <typename key_t, class... Args>
  node_t* insert(const key_t& key, Args&&... args) {
    return _insert(new_node(key, forward<Args>(args)...));
  }

  template <typename key_t>
  void erase(const key_t& key) {
    if (node_t* x = find(key); x != nil) {
      erase(x);
    }
  }

  template <typename key_t, class... Args>
  node_t* insert_if_none(const key_t& key, Args&&... args) {
    if (contains(key)) return nil;
    return _insert(new_node(key, forward<Args>(args)...));
  }

  template <typename key_t, class... Args>
  node_t* overwrite(const key_t& key, Args&&... args) {
    node_t* has = find(key);
    if (has == nil) return insert(key, forward<Args>(args)...);
    if constexpr (sizeof...(args) != 0) {
      has->value = typename node_t::value_t(forward<Args>(args)...);
    }
    return has;
  }

  template <typename key_t>
  void erase_all(const key_t& key) {
    for (node_t* x; (x = find(key)) != nil; ) { // should this be next?
      erase(x);
    }
  }

  node_t* _insert(node_t* x) {
    if (node_t* u = upper_bound_path(x->key); u != nil) {
      set_child(u, x, x->key < u->key);
    }
    return splay(x);
  }

  node_t* erase(node_t* x) {
    if (x == nil) return nil;
    splay(x); // splay should push x
    bool rightmost = (x->right == nil);
    set_child(nil, x->left, true);
    if (x->right != nil) {
      set_child(nil, x->right, false);
      set_child(splay(walk_left(x->right)), x->left, true);
      pull(root);
    } else if (x->left != nil) {
      splay(x->left);
    } else {
      root = nil;
    }
    del_node(x);
    return rightmost ? nil : root;
  }

  template <bool dir> void erase(iterator<dir> it) {
    erase(it.ptr);
  }

  void erase_at(int i) {
    erase(at(i));
  }

  template <typename key_t>
  node_t* find(const key_t& key) {
    node_t* u = lower_bound(key); // lower_bound splays
    if (u == nil || u->key != key) return nil;
    return u;
  }

  // Contains: returns true if subtree contanis a valid node
  // Check:    returns true if the node is valid
  template <class Contains, class Check>
  node_t* find_first_after(node_t* x, Contains has, Check valid) {
    node_t* u = splay(x) == nil ? root : x->right;
    if (u == nil || !has(u)) return nil;
    while (true) {
      if constexpr (has_push) push(u);
      if (u->left != nil && has(u->left)) {
        u = u->left;
      } else if(valid(u)) {
        return splay(u);
      } else if(u->right != nil) {
        u = u->right;
      }
    }
  }

  // Contains: returns true if subtree contanis a valid node
  // Check:    returns true if the node is valid
  template <class Contains, class Check>
  node_t* find_first_before(node_t* x, Contains has, Check valid) {
    node_t* u = splay(x) == nil ? root : x->left;
    if (u == nil || !has(u)) return nil;
    while (true) {
      if constexpr (has_push) push(u);
      if (u->right != nil && has(u->right)) {
        u = u->right;
      } else if(valid(u)) {
        return splay(u);
      } else {
        u = u->left;
      }
    }
  }

  template <typename key_t>
  node_t* lower_bound(const key_t& key) {
    node_t* u = lower_bound_path(key);
    if (u == nil || key <= u->key) return splay(u);
    return splay(next(u));
  }

  template <typename key_t>
  node_t* upper_bound(const key_t& key) {
    node_t* u = upper_bound_path(key);
    if (u == nil || key < u->key) return splay(u);
    return splay(next(u));
  }

  // goes to a leaf, chooseing left if <= node.key
  template <typename key_t>
  node_t* lower_bound_path(const key_t& key) const {
    if (root == nil) return root;
    node_t* u = root;
    while (true) {
      if constexpr (has_push) push(u);
      if (key <= u->key) {
        if (u->left == nil) {
          return u;
        }
        u = u->left;
      } else {
        if (u->right == nil) {
          return u;
        }
        u = u->right;
      }
    }
  }

  // goes to a leaf, chooseing left if < node.key
  template <typename key_t>
  node_t* upper_bound_path(const key_t& key) const {
    if (root == nil) return root;
    node_t* u = root;
    while (true) {
      if constexpr (has_push) push(u);
      if (key < u->key) {
        if (u->left == nil) {
          return u;
        }
        u = u->left;
      } else {
        if (u->right == nil) {
          return u;
        }
        u = u->right;
      }
    }
  }

  //---------------------------------------------------------------------------
  // next / previous

  node_t* splay_next(node_t* u) { return splay(next(u)); }
  node_t* splay_prev(node_t* u) { return splay(prev(u)); }

  node_t* next(node_t* u) const {
    if (u == nil) throw invalid_argument("next(nil) is not valid");
    if (u->right != nil) {
      if constexpr (has_push) push(u);
      return walk_left(u->right);
    }
    while (u != nil && !is_left_child(u)) {
      u = u->parent;
    }
    u = u != nil ? u->parent : u;
    return u;
  }

  node_t* prev(node_t* u) const {
    if (u == nil) throw invalid_argument("prev(nil) is not valid");
    if (u->left != nil) {
      if constexpr (has_push) push(u);
      return walk_right(u->left);
    }
    while (u != nil && is_left_child(u)) {
      u = u->parent;
    }
    u = u != nil ? u->parent : u;
    return u;
  }

  node_t* walk_left(node_t* u) const {
    assert(u != nil);
    while (u->left != nil) {
      if constexpr (has_push) push(u);
      u = u->left;
    }
    return u;
  }

  node_t* walk_right(node_t* u) const {
    assert(u != nil);
    while (u->right != nil) {
      if constexpr (has_push) push(u);
      u = u->right;
    }
    return u;
  }

  //---------------------------------------------------------------------------
  // rope operations

  template <class... Args>
  node_t* push_back(Args&&... args) {
    return _push_back(new_node(forward<Args>(args)...));
  }
  template <class... Args>
  node_t* push_front(Args&&... args) {
    return _push_front(new_node(forward<Args>(args)...));
  }
  template <class... Args>
  node_t* insert_before(int ref, Args&&... args) {
    return _insert_before(at(ref), new_node(forward<Args>(args)...));
  }
  template <class... Args>
  node_t* insert_after(int ref, Args&&... args) {
    return _insert_after(at(ref), new_node(forward<Args>(args)...));
  }
  splay_tree split_before(int x) { return split_before(at(x)); }
  splay_tree split_after(int x) { return split_after(at(x)); }

  node_t* first() { return root != nil ? splay(walk_left(root)) : root; }
  node_t* last() { return root != nil ? splay(walk_right(root)) : root; }
  typename node_t::out_t front() {
    if (root == nil) throw invalid_argument("calling front() on empty splay tree");
    return *first();
  }
  typename node_t::out_t back() {
    if (root == nil) throw invalid_argument("calling back() on empty splay tree");
    return *last();
  }

  node_t* at(int i) {
    if (i < 0 || i >= size()) return nil; // do we want to throw?
    node_t* u = root;
    while (true) {
      if constexpr (has_push) push(u);
      if (int ls = u->left->size; ls > i) {
        u = u->left;
      } else if (ls < i) {
        u = u->right;
        i -= ls + 1;
      } else {
        return splay(u); // requires splaying
      }
    }
  }

  template <typename T = typename node_t::key_t, typename = enable_if_t<is_void_v<T>>>
  typename node_t::out_t operator [] (int i) {
    return at(i)->get_value();
  }

  template <typename key_t, typename value_t = typename node_t::value_t,
    typename = enable_if_t<!is_void_v<key_t> && !is_void_v<value_t>>>
  value_t& operator [] (const key_t& k) {
    node_t* x = lower_bound(k);
    if (x != nil && x->key == k) return x->value;
    return _insert_before(x, new_node(k, typename node_t::value_t()))->value;
  }

  node_t* _push_back(node_t* x) {
    if (root == nil) return root = x;
    node_t* at = last();
    if constexpr (has_push) push(at);
    set_child(at, x, false);
    return splay(x);
  }
  node_t* _push_front(node_t* x) {
    if (root == nil) return root = x;
    node_t* at = first();
    if constexpr (has_push) push(at);
    set_child(at, x, true);
    return splay(x);
  }
  void pop_back() { erase(last()); }
  void pop_front() { erase(first()); }

  node_t* _insert_before(node_t* ref, node_t* x) {
    if (splay(ref) == nil) {
      return _push_back(x);
    }
    if (ref->left == nil) {
      if constexpr (has_push) push(ref);
      set_child(ref, x, true);
    } else {
      ref = prev(ref);
      if constexpr (has_push) push(ref);
      set_child(ref, x, false);
    }
    return splay(x);
  }

  node_t* _insert_after(node_t* ref, node_t* x) {
    if (splay(ref) == nil) {
      return _push_front(x);
    }
    if(ref->right == nil) {
      if constexpr (has_push) push(ref);
      set_child(ref, x, false);
    } else {
      ref = next(ref);
      if constexpr (has_push) push(ref);
      set_child(ref, x, true);
    }
    return splay(x);
  }

  splay_tree split_before(node_t* x) {
    if (splay(x) == nil) return dislodge();
    if constexpr (has_push) push(x);
    set_child(nil, x->left, true);
    node_t* ret = x->left;
    x->left = nil;
    pull(x);
    return splay_tree(ret);
  }

  splay_tree split_after(node_t* x) {
    if (splay(x) == nil) return dislodge();
    if constexpr (has_push) push(x);
    set_child(nil, x->right, true);
    node_t* ret = x->right;
    x->right = nil;
    pull(x);
    return splay_tree(ret);
  }

  void append(splay_tree& second) {
    if (second.root == nil) return;
    if (root == nil) {
      root = second.root;
    } else {
      splay(last());
      if constexpr (has_push) push(root);
      set_child(root, second.root, false);
      pull(root);
      splay(second.root);
    }
    second.root = nil; // no more tree
  }

  //---------------------------------------------------------------------------
  // push / pull

  // range accumulate
  void pull(node_t* x) const {
    if (x == nil) return;
    x->size = x->left->size + x->right->size + 1;
    if constexpr (has_pull) x->pull();
  }

  // lazy update
  template <bool use = has_push, typename = enable_if_t<use>>
  void push(node_t* x) const {
    x->push();
  }

  template <bool use = has_push, typename = enable_if_t<use>>
  node_t* push_down_to(node_t* x) const {
    if (x != nil) {
      push_down_to(x->parent);
      push(x); // x itself gets pushed too
    }
    return x;
  }

  //---------------------------------------------------------------------------
  // range update / query

  // ranges are exclusive
  node_t* range(node_t* left, node_t* right) {
    if (left == nil) {
      if (right == nil) return root;
      else return splay(right)->left;
    } else if (right == nil) {
      return splay(left)->right;
    }
    if (left == right) return nil;
    splay(left);
    return splay_to(right, left)->left;
  }

  // should the input be checked ?
  node_t* range(int l, int r) {
    return range(at(l - 1), at(r + 1));
  }

  template <typename index_t, class... Args>
  void update_range(index_t l, index_t r, const Args&... args) {
    node_t* x = range(l, r);
    x->put(args...);
    splay(x); // pull to root
  }

  template <typename index_t, class... Args>
  auto query_range(index_t l, index_t r, const Args&... args) {
    return range(l, r)->get(args...);
  }

  //---------------------------------------------------------------------------
  // splay / rotate

private:

  void _splay_rec(node_t* x) {
    if (x->parent == nil) return;
    _rotate(is_left_child(x) == is_left_child(x->parent) ? x->parent : x);
    _rotate(x);
    return _splay_rec(x);
  }

  void _splay_to_rec(node_t* x, node_t* to) {
    if (x->parent == to) return;
    if (x->parent->parent == to) {
      return _rotate(x);
    }
    _rotate(is_left_child(x) == is_left_child(x->parent) ? x->parent : x);
    _rotate(x);
    return _splay_to_rec(x, to);
  }

  void _rotate(node_t* x) {
    if (x->parent == nil) return;
    node_t* p = x->parent;
    bool side = is_left_child(x);
    set_child(p->parent, x, is_left_child(p));
    set_child(p, x->get_child(!side), side);
    set_child(x, p, !side);
    pull(p); // now child of x
  }

  // private constructor
  splay_tree(node_t* other): root(other) {}
};

//-----------------------------------------------------------------------------
// node structs and initializtion

template <typename derived_t>
struct splay_node_base_common {
  static derived_t* const nil;
  derived_t *parent, *left, *right;
  int size;
  splay_node_base_common(int s): parent(nil), left(nil), right(nil), size(s) {}
  bool is_left_child() const { return parent == nil || this == parent->left; }
  derived_t* get_child(bool is_left) const { return is_left ? left : right; }
  static derived_t* _get_nil() {
    static derived_t _nil{};
    for (static bool once = true; once; once = false) {
      _nil.parent = _nil.left = _nil.right = &_nil;
      _nil.size = 0;
    }
    return &_nil;
  }
};

template <typename derived_t, typename _key_t, typename _value_t, typename = void>
struct splay_node_base : splay_node_base_common<derived_t> {
  using key_t = _key_t;
  using value_t = _value_t;
  using out_t = pair<key_t&, value_t&>; // store as tuple
  key_t key;
  value_t value;
  splay_node_base(): splay_node_base_common<derived_t>(0), key(), value() {}
  splay_node_base(const key_t& k, const value_t& v)
    : splay_node_base_common<derived_t>(1), key(k), value(v) {}
  bool operator < (const derived_t& other) const { return key < other.key; }
  out_t get_value() { return out_t(ref(key), ref(value)); }
};

template <typename derived_t, typename _key_t, typename _value_t>
struct splay_node_base<derived_t, _key_t, _value_t,
    enable_if_t<is_void_v<_key_t> && !is_void_v<_value_t>>>
    : splay_node_base_common<derived_t> {
  using key_t = _key_t;
  using value_t = _value_t;
  using out_t = value_t&;
  value_t value;
  splay_node_base(): splay_node_base_common<derived_t>(0), value() {}
  splay_node_base(const value_t& v): splay_node_base_common<derived_t>(1), value(v) {}
  out_t get_value() { return value; }
};

template <typename derived_t, typename _key_t, typename _value_t>
struct splay_node_base<derived_t, _key_t, _value_t,
    enable_if_t<!is_void_v<_key_t> && is_void_v<_value_t>>>
    : splay_node_base_common<derived_t> {
  using key_t = _key_t;
  using value_t = _value_t;
  using out_t = key_t&;
  key_t key;
  splay_node_base(): splay_node_base_common<derived_t>(0), key() {}
  splay_node_base(const key_t& k): splay_node_base_common<derived_t>(1), key(k) {}
  bool operator < (const derived_t& other) const { return key < other.key; }
  out_t get_value() { return key; }
};

template <typename derived_t, typename _key_t, typename _value_t>
struct splay_node_base<derived_t, _key_t, _value_t,
    enable_if_t<is_void_v<_key_t> && is_void_v<_value_t>>>
    : splay_node_base_common<derived_t> {
  using key_t = _key_t;
  using value_t = _value_t;
  using out_t = void;
  splay_node_base(): splay_node_base_common<derived_t>(1) {}
  out_t get_value() { return; }
};

template <typename _key_t, typename _value_t>
struct splay_node final : splay_node_base<splay_node<_key_t, _value_t>, _key_t, _value_t> {
  using splay_node_base<splay_node<_key_t, _value_t>, _key_t, _value_t>::splay_node_base;
};

template <typename derived_t>
derived_t* const splay_node_base_common<derived_t>::nil = derived_t::_get_nil();

template <class node_t, int max_size>
node_t* const splay_tree<node_t, max_size>::nil = node_t::nil;

template <class node_t, int max_size>
static_memory_pool<node_t, splay_tree<node_t, max_size>::mem_size>
splay_tree<node_t, max_size>::memory =
  static_memory_pool<node_t, splay_tree<node_t, max_size>::mem_size>();

template <class node_t>
using splay_tree_shared_memory = splay_tree<node_t, -1>;

