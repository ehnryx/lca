/* Splay Tree
 * USAGE
 *  pray
 * TIME
 *  O(logN) per operation amortized
 *  N = |splay tree|
 * STATUS
 *  untested
 */
#pragma once

template <class node_t, bool value_updates = false, bool key_updates = false>
struct splay_tree {
  static constexpr bool need_push = value_updates || key_updates;

  node_t* root;
  splay_tree(): root(nullptr) {}
  template <class InputIt>
  splay_tree(InputIt first, InputIt last, bool from_tuple = false): splay_tree() {
    if (from_tuple) {
      while (first != last) {
        insert(new node_t(make_from_tuple(*first++)));
      }
    } else {
      while (first != last) {
        insert(new node_t(*first++));
      }
    }
  }
  ~splay_tree() { if (root != nullptr) erase_tree(root); }
  void erase_tree(node_t* x) {
    if (x->left != nullptr) erase_tree(x->left);
    if (x->right != nullptr) erase_tree(x->right);
    delete x;
  }

  void clear() {
    if (root != nullptr) {
      erase_tree(root);
      root = nullptr;
    }
  }

  splay_tree dislodge() {
    node_t* ret = root;
    root = nullptr;
    return splay_tree(ret);
  }

  splay_tree(const splay_tree&) = delete;
  splay_tree& operator = (const splay_tree&) = delete;
  splay_tree make_copy() const {
    return splay_tree(make_copy_rec(root));
  }
  node_t* make_copy_rec(node_t* x) const {
    node_t* cur = new node_t(*x);
    if (x->left != nullptr) {
      cur->left = make_copy_rec(x->left);
      cur->left->parent = cur;
    }
    if (x->right != nullptr) {
      cur->right = make_copy_rec(x->right);
      cur->right->parent = cur;
    }
    return cur;
  }

  int size() const { return get_size(root); }
  int rank(node_t* x) { return splay(x) == nullptr ? size() : get_size(x->left); }
  bool empty() const { return root == nullptr; }
  int get_size(node_t* x) const { return x ? x->size : 0; }

  node_t* splay(node_t* x) {
    if (x == nullptr) return x;
    return root = _splay_rec(x);
  }

  node_t* splay_to(node_t* x, node_t* to) {
    if (x == nullptr) return x;
    if (to == nullptr) return splay(x);
    return _splay_to_rec(x, to);
  }

  void set_child(node_t* parent, node_t* child, bool left) {
    if (child != nullptr) {
      child->parent = parent;
    }
    if (parent != nullptr) {
      if (left) parent->left = child;
      else parent->right = child;
    }
  }
  bool is_left_child(node_t* x) const {
    return x != nullptr && x->is_left_child();
  }

  //---------------------------------------------------------------------------
  // iterators
  //---------------------------------------------------------------------------

  template <bool forward_it>
  struct iterator {
    const splay_tree* tree_ptr;
    node_t* ptr;
    iterator(): tree_ptr(nullptr), ptr(nullptr) {}
    iterator(const splay_tree* t, node_t* x): tree_ptr(t), ptr(x) {}
    iterator(const iterator<!forward_it> rev): tree_ptr(rev.tree_ptr), ptr(rev.ptr) {}
    operator node_t* () const { return ptr; }
    const node_t* operator -> () const { return ptr; }
    typename node_t::out_t operator * () const { return ptr->get_value(); }
    iterator& operator ++ () {
      ptr = forward_it ? tree_ptr->next(ptr) : tree_ptr->prev(ptr);
      return *this;
    }
    iterator& operator ++ (int) { return operator ++ (); }
    iterator& operator -- () { return *this = ++iterator<!forward_it>(*this); }
    iterator& operator -- (int) { return operator -- (); }
    bool operator == (const iterator& other) const { return ptr == other.ptr; }
    bool operator != (const iterator& other) const { return ptr != other.ptr; }
    bool operator == (const node_t* other) const { return ptr == other; }
    bool operator != (const node_t* other) const { return ptr != other; }
  };
  iterator<true> begin() const {
    return iterator<true>(this, root ? walk_left(root) : root);
  }
  iterator<true> end() const {
    return iterator<true>(nullptr, nullptr);
  }
  iterator<false> rbegin() const {
    return iterator<false>(this, root ? walk_right(root) : root);
  }
  iterator<false> rend() const {
    return iterator<false>(nullptr, nullptr);
  }
  template <bool dir> friend iterator<dir> next(iterator<dir> it) { return ++it; }
  template <bool dir> friend iterator<dir> prev(iterator<dir> it) { return --it; }

  //---------------------------------------------------------------------------
  // binary search tree operations
  //---------------------------------------------------------------------------

  template <typename T>
  bool contains(const T& key) {
    return find(key) != nullptr;
  }

  template <typename T>
  int count(const T& key) {
    return rank(upper_bound(key)) - rank(lower_bound(key));
  }

  template <typename T, class... Args>
  node_t* insert(const T& key, const Args&... args) {
    return insert(new node_t(key, args...));
  }

  template <typename T>
  void erase(const T& key) {
    if (node_t* x = find(key); x != nullptr) {
      erase(x);
    }
  }

  template <typename T, class... Args>
  node_t* insert_if_none(const T& key, const Args&... args) {
    if (contains(key)) return nullptr;
    return insert(new node_t(key, args...));
  }

  template <typename T>
  void erase_all(const T& key) {
    for (node_t* x; (x = find(key)) != nullptr; ) { // should this be next?
      erase(x);
    }
  }

  node_t* insert(node_t* x) {
    if (x->parent || x->left || x->right) {
      throw invalid_argument("`insert(node_t*)` expects a singleton node");
    }
    if (node_t* u = upper_bound_path(x->key); u != nullptr) {
      set_child(u, x, x->key < u->key);
    }
    return splay(x);
  }

  void erase(node_t* x) {
    if (x == nullptr) return;
    splay(x);
    if constexpr (need_push) push(x);
    set_child(nullptr, x->left, true);
    if (x->right) {
      set_child(nullptr, x->right, false);
      set_child(splay(walk_left(x->right)), x->left, true);
      pull(root);
    } else if (x->left) {
      splay(x->left);
    } else {
      root = nullptr;
    }
    delete x;
  }

  template <bool dir> void erase(iterator<dir> it) { erase(it.ptr); }

  template <typename T>
  node_t* find(const T& key) {
    node_t* u = lower_bound(key); // lower_bound splays
    if (u == nullptr || u->key != key) return nullptr;
    return u;
  }

  template <typename T>
  node_t* lower_bound(const T& key) {
    node_t* u = lower_bound_path(key);
    if (u == nullptr || key <= u->key) return splay(u);
    return splay(next(u));
  }

  template <typename T>
  node_t* upper_bound(const T& key) {
    node_t* u = upper_bound_path(key);
    if (u == nullptr || key < u->key) return splay(u);
    return splay(next(u));
  }

  // goes to a leaf, chooseing left if <= node.key
  template <typename T>
  node_t* lower_bound_path(const T& key) const {
    if (root == nullptr) return root;
    node_t* u = root;
    while (true) {
      if constexpr (need_push) push(u);
      if (key <= u->key) {
        if (u->left == nullptr) {
          return u;
        }
        u = u->left;
      } else {
        if (u->right == nullptr) {
          return u;
        }
        u = u->right;
      }
    }
  }

  // goes to a leaf, chooseing left if < node.key
  template <typename T>
  node_t* upper_bound_path(const T& key) const {
    if (root == nullptr) return root;
    node_t* u = root;
    while (true) {
      if constexpr (need_push) push(u);
      if (key < u->key) {
        if (u->left == nullptr) {
          return u;
        }
        u = u->left;
      } else {
        if (u->right == nullptr) {
          return u;
        }
        u = u->right;
      }
    }
  }

  //---------------------------------------------------------------------------
  // next / previous
  //---------------------------------------------------------------------------

  node_t* next(node_t* u) const {
    if (u == nullptr) throw invalid_argument("next(nullptr) is not valid");
    if (u->right != nullptr) {
      if constexpr (need_push) push(u);
      return walk_left(u->right);
    }
    while (u != nullptr && !is_left_child(u)) {
      u = u->parent;
      pull(u);
    }
    u = u ? u->parent : u;
    pull(u);
    return u;
  }

  node_t* prev(node_t* u) const {
    if (u == nullptr) throw invalid_argument("prev(nullptr) is not valid");
    if (u->left != nullptr) {
      if constexpr (need_push) push(u);
      return walk_right(u->left);
    }
    while (u != nullptr && is_left_child(u)) {
      u = u->parent;
      pull(u);
    }
    u = u ? u->parent : u;
    pull(u);
    return u;
  }

  node_t* walk_left(node_t* u) const {
    assert(u != nullptr);
    while (u->left != nullptr) {
      if constexpr (need_push) push(u);
      u = u->left;
    }
    if constexpr (need_push) push(u);
    return u;
  }

  node_t* walk_right(node_t* u) const {
    assert(u != nullptr);
    while (u->right != nullptr) {
      if constexpr (need_push) push(u);
      u = u->right;
    }
    if constexpr (need_push) push(u);
    return u;
  }

  //---------------------------------------------------------------------------
  // rope operations
  //---------------------------------------------------------------------------

  template <class... Args>
  void push_back(const Args&... args) { push_back(new node_t(args...)); }
  template <class... Args>
  void push_front(const Args&... args) { push_front(new node_t(args...)); }
  template <class... Args>
  void insert_before(int ref, Args&... args) {
    insert_before(at(ref), new node_t(args...));
  }
  template <class... Args>
  void insert_after(int ref, Args&... args) {
    insert_after(at(ref), new node_t(args...));
  }
  splay_tree split_before(int x) { return split_before(at(x)); }
  splay_tree split_after(int x) { return split_after(at(x)); }

  node_t* first() { return root != nullptr ? splay(walk_left(root)) : root; }
  node_t* last() { return root != nullptr ? splay(walk_right(root)) : root; }

  node_t* at(int i) {
    if (i < 0 || i >= size()) return nullptr; // do we want to throw?
    node_t* u = root;
    while (true) {
      if constexpr (need_push) push(u);
      if (int ls = get_size(u->left); ls > i) {
        u = u->left;
      } else if (ls < i) {
        u = u->right;
        i -= ls + 1;
      } else {
        return splay(u);
      }
    }
  }

  void push_back(node_t* x) {
    node_t* at = last();
    if constexpr (need_push) push(at);
    set_child(at, x, false);
    splay(x);
  }
  void push_front(node_t* x) {
    node_t* at = first();
    if constexpr (need_push) push(at);
    set_child(at, x, true);
    splay(x);
  }
  void pop_back() { erase(last()); }
  void pop_front() { erase(first()); }

  void insert_before(node_t* ref, node_t* x) {
    if (splay(ref) == nullptr) {
      return push_back(x);
    }
    if (ref->left == nullptr) {
      if constexpr (need_push) push(ref);
      set_child(ref, x, true);
    } else {
      ref = prev(ref);
      if constexpr (need_push) push(ref);
      set_child(ref, x, false);
    }
    splay(x);
  }

  void insert_after(node_t* ref, node_t* x) {
    if (splay(ref) == nullptr) {
      return push_front(x);
    }
    if(ref->right == nullptr) {
      if constexpr (need_push) push(ref);
      set_child(ref, x, false);
    } else {
      ref = next(ref);
      if constexpr (need_push) push(ref);
      set_child(ref, x, true);
    }
    splay(x);
  }

  splay_tree split_before(node_t* x) {
    if (splay(x) == nullptr) return dislodge();
    if constexpr (need_push) push(x);
    set_child(nullptr, x->left, true);
    splay_tree ret(x->left);
    x->left = nullptr;
    pull(x);
    return ret;
  }

  splay_tree split_after(node_t* x) {
    if (splay(x) == nullptr) return dislodge();
    if constexpr (need_push) push(x);
    set_child(nullptr, x->right, true);
    splay_tree ret(x->right);
    x->right = nullptr;
    pull(x);
    return ret;
  }

  void append(splay_tree& second) {
    if (second.root == nullptr) return;
    if (root == nullptr) {
      root = second.root;
    } else {
      push(splay(last()));
      set_child(root, second.root, false);
      pull(root);
      splay(second.root);
    }
    second.root = nullptr; // no more tree
  }

  //---------------------------------------------------------------------------
  // push / pull
  //---------------------------------------------------------------------------

  // range accumulate
  void pull(node_t* x) const {
    if (x == nullptr) return;
    x->size = get_size(x->left) + get_size(x->right) + 1;
    //x->pull(x->left, x->right);
  }

  // lazy update
  void push(node_t* x) const {
    assert(x != nullptr);
    //x->push(x->left, x->right);
  }

  template <bool use = need_push, typename = enable_if_t<use>>
  node_t* push_down_to(node_t* x) const {
    if (x != nullptr) {
      push_down_to(x->parent);
      push(x);
    }
    return x;
  }

  //---------------------------------------------------------------------------
  // range update / query
  //---------------------------------------------------------------------------

  // ranges are exclusive
  node_t* range(node_t* left, node_t* right) {
    if (left == nullptr) {
      if (right == nullptr) return root;
      else return splay(right)->left;
    } else if (right == nullptr) {
      return splay(left)->right;
    }
    if (left == right) return nullptr;
    splay(left);
    return splay_to(right, left)->left;
  }

  //---------------------------------------------------------------------------
  // splay / rotate
  //---------------------------------------------------------------------------

private:

  node_t* _splay_rec(node_t* x) {
    assert(x != nullptr); // not null
    if (x == nullptr || x->parent == nullptr) return x;
    _rotate(is_left_child(x) == is_left_child(x->parent) ? x->parent : x);
    _rotate(x);
    return _splay_rec(x);
  }

  node_t* _splay_to_rec(node_t* x, node_t* to) {
    assert(x != nullptr); // not null
    if (x->parent == to) return x;
    if (x->parent->parent == to) {
      _rotate(x);
      return x;
    }
    _rotate(is_left_child(x) == is_left_child(x->parent) ? x->parent : x);
    _rotate(x);
    return _splay_to_rec(x);
  }

  void _rotate(node_t* x) {
    if (x->parent == nullptr) return;
    node_t* p = x->parent;
    if constexpr (need_push) {
      push(p);
      push(x);
    }
    bool side = is_left_child(x);
    set_child(p->parent, x, is_left_child(p));
    set_child(p, x->get_child(!side), side);
    set_child(x, p, !side);
    pull(p); // now child of x
    pull(x); // now parent of p
  }

  // private constructor
  splay_tree(node_t* other): root(other) {}
};

//-----------------------------------------------------------------------------
// node structs
//-----------------------------------------------------------------------------

template <typename key_t, typename value_t, typename = void>
struct splay_node {
  using out_t = void;
  int size;
  splay_node *parent, *left, *right;
  splay_node(): size(1), parent(nullptr), left(nullptr), right(nullptr) {}
  bool is_left_child() const { return parent == nullptr || this == parent->get_child(true); }
  splay_node* get_child(bool is_left) const { return is_left ? left : right; }
  out_t get_value() const { return; }
};

template <typename key_t, typename value_t>
struct splay_node<key_t, value_t, enable_if_t<is_void_v<key_t> && !is_void_v<value_t>>> {
  using out_t = const value_t&;
  value_t value;
  int size;
  splay_node *parent, *left, *right;
  splay_node(): value(), size(1), parent(nullptr), left(nullptr), right(nullptr) {}
  splay_node(const value_t& v)
    : value(v), size(1), parent(nullptr), left(nullptr), right(nullptr) {}
  bool is_left_child() const { return parent == nullptr || this == parent->get_child(true); }
  splay_node* get_child(bool is_left) const { return is_left ? left : right; }
  out_t get_value() const { return value; }
};

template <typename key_t, typename value_t>
struct splay_node<key_t, value_t, enable_if_t<!is_void_v<key_t> && is_void_v<value_t>>> {
  using out_t = const key_t&;
  key_t key;
  int size;
  splay_node *parent, *left, *right;
  splay_node(): key(), size(1), parent(nullptr), left(nullptr), right(nullptr) {}
  splay_node(const key_t& k):
    key(k), size(1), parent(nullptr), left(nullptr), right(nullptr) {}
  bool is_left_child() const { return parent == nullptr || this == parent->get_child(true); }
  splay_node* get_child(bool is_left) const { return is_left ? left : right; }
  bool operator < (const splay_node& other) const { return key < other.key; }
  out_t get_value() const { return key; }
};

template <typename key_t, typename value_t>
struct splay_node<key_t, value_t, enable_if_t<!is_void_v<key_t> && !is_void_v<value_t>>> {
  using out_t = pair<const key_t&, const value_t&>; // store as tuple
  key_t key;
  value_t value;
  int size;
  splay_node *parent, *left, *right;
  splay_node(): key(), value(), size(1), parent(nullptr), left(nullptr), right(nullptr) {}
  splay_node(const key_t& k, const value_t& v)
    : key(k), value(v), size(1), parent(nullptr), left(nullptr), right(nullptr) {}
  bool is_left_child() const { return parent == nullptr || this == parent->get_child(true); }
  splay_node* get_child(bool is_left) const { return is_left ? left : right; }
  bool operator < (const splay_node& other) const { return key < other.key; }
  out_t get_value() const { return out_t(cref(key), cref(value)); }
};

