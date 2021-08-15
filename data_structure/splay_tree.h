/* Splay Tree
 * USAGE
 * MEMBERS
 * TIME
 *  O(logN) per query amortized
 *  N = |splay tree|
 * STATUS
 *  untested
 */
#pragma once

template <typename key_t, typename value_t, typename = void>
struct splay_node {
  using out_t = void;
  int size;
  splay_node *parent, *left, *right;
  splay_node(): size(1), parent(nullptr), left(nullptr), right(nullptr) {}
  bool left_child() const { return parent == nullptr || this == parent->get_child(true); }
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
  bool left_child() const { return parent == nullptr || this == parent->get_child(true); }
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
  bool left_child() const { return parent == nullptr || this == parent->get_child(true); }
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
  bool left_child() const { return parent == nullptr || this == parent->get_child(true); }
  splay_node* get_child(bool is_left) const { return is_left ? left : right; }
  bool operator < (const splay_node& other) const { return key < other.key; }
  out_t get_value() const { return out_t(cref(key), cref(value)); }
};

template <class node_t>
struct splay_tree {
  node_t* root;
  splay_tree(): root(nullptr) {}
  template <class InputIt>
  splay_tree(InputIt first, InputIt last): splay_tree() {
    while (first != last) {
      insert(new node_t(*first));
      first++;
    }
  }
  ~splay_tree() {
    if (root) erase_tree(root);
  }
  void erase_tree(node_t* x) {
    if (x->left) erase_tree(x->left);
    if (x->right) erase_tree(x->right);
    delete x;
  }

  node_t* splay(node_t* x) {
    if (x == nullptr) return x;
    return root = splay_rec(x);
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
    set_child(nullptr, x->left, true);
    if (x->right) {
      set_child(nullptr, x->right, false);
      set_child(splay(walk_left(x->right)), x->left, true);
    } else if (x->left) {
      splay(x->left);
    } else {
      root = nullptr;
    }
    delete x;
  }

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

  node_t* next(node_t* u) const {
    if (u == nullptr) throw invalid_argument("next(nullptr) is not valid");
    if (u->right) return walk_left(u->right);
    while (u != nullptr && !left_child(u)) {
      u = u->parent;
    }
    return u ? u->parent : u;
  }

  node_t* prev(node_t* u) const {
    if (u == nullptr) throw invalid_argument("prev(nullptr) is not valid");
    if (u->left) return walk_right(u->left);
    while (u != nullptr && left_child(u)) {
      u = u->parent;
    }
    return u ? u->parent : u;
  }

  int size() const { return get_size(root); }
  int rank(node_t* x) { return get_size(splay(x)->left); }
  bool empty() const { return root == nullptr; }

  template <bool forward_it>
  struct iterator {
    const splay_tree* tree_ptr;
    node_t* ptr;
    iterator(): tree_ptr(nullptr), ptr(nullptr) {}
    iterator(const splay_tree* t, node_t* x): tree_ptr(t), ptr(x) {}
    const node_t* operator -> () const { return ptr; }
    typename node_t::out_t operator * () const { return ptr->get_value(); }
    iterator& operator ++ () {
      ptr = forward_it ? tree_ptr->next(ptr) : tree_ptr->prev(ptr);
      return *this;
    }
    iterator& operator ++ (int) { return operator ++ (); }
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
  template <bool forward_it>
  friend bool operator == (const node_t* a, const iterator<forward_it>& b) { return b == a; }
  template <bool forward_it>
  friend bool operator != (const node_t* a, const iterator<forward_it>& b) { return b != a; }

  void set_child(node_t* parent, node_t* child, bool left) {
    if (child != nullptr) {
      child->parent = parent;
    }
    if (parent != nullptr) {
      if (left) parent->left = child;
      else parent->right = child;
    }
  }
  bool left_child(node_t* x) const {
    return x != nullptr && x->left_child();
  }

  void pull(node_t* x) {
    x->size = get_size(x->left) + get_size(x->right) + 1;
  }

  template <typename T>
  node_t* lower_bound_path(const T& key) const {
    if (root == nullptr) return root;
    node_t* u = root;
    while (true) {
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

  template <typename T>
  node_t* upper_bound_path(const T& key) const {
    if (root == nullptr) return root;
    node_t* u = root;
    while (true) {
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

  node_t* walk_left(node_t* u) const {
    assert(u != nullptr);
    while (u->left != nullptr) {
      u = u->left;
    }
    return u;
  }

  node_t* walk_right(node_t* u) const {
    assert(u != nullptr);
    while (u->right != nullptr) {
      u = u->right;
    }
    return u;
  }

  int get_size(node_t* x) const {
    return x ? x->size : 0;
  }

private:
  node_t* splay_rec(node_t* x) {
    assert(x != nullptr); // not null
    if (x == nullptr || x->parent == nullptr) return x;
    rotate(left_child(x) == left_child(x->parent) ? x->parent : x);
    rotate(x);
    return splay_rec(x);
  }
  void rotate(node_t* x) {
    if (x->parent == nullptr) return;
    bool side = left_child(x);
    node_t* p = x->parent;
    set_child(p->parent, x, left_child(p));
    set_child(p, x->get_child(!side), side);
    set_child(x, p, !side);
    pull(x);
  }
};

