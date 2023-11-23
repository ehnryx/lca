/* Leftist Tree (Min Heap)
 * USAGE
 *  leftist_tree<T> heap;
 * MEMBERS
 *  int size() const;
 *  bool empty() const;
 *  void push(T);
 *  void pop();
 *  const T& top() const;
 *  void merge(leftist_tree<T>);
 * INPUT
 *  T should be a type that supports `operator<`
 * TIME
 *  O(logN) push, pop, merge
 *  O(1) top, empty
 *  N = |heap|
 * STATUS
 *  tested: cf/102803j,1515f
 */
#pragma once

#include <functional>
#include "../utility/member_function_checker.h"

template <typename value_t>
struct leftist_node {
  leftist_node *left, *right;
  value_t value;
  int size;
  leftist_node(const value_t& val) : left(nullptr), right(nullptr), value(val), size(1) {}
};

template <typename value_t, typename lazy_t = value_t>
struct leftist_node_lazy {
  leftist_node_lazy *left, *right;
  value_t value;
  lazy_t lazy;
  int size;
  leftist_node_lazy(const value_t& val)
      : left(nullptr), right(nullptr), value(val), lazy(0), size(1) {}
  void put(lazy_t add) {
    value += add;
    lazy += add;
  }
  void push() {
    if (lazy != 0) {
      if (left != nullptr) left->put(lazy);
      if (right != nullptr) right->put(lazy);
      lazy = 0;
    }
  }
};

template <typename T, typename node_t = leftist_node<T>, class Compare = std::less<>>
struct leftist_tree {
  MEMBER_FUNCTION_CHECKER(push);
  static constexpr bool has_push = _has_push<node_t>::value;
  node_t* root;
  leftist_tree() : root(nullptr) {}
  friend void swap(leftist_tree& a, leftist_tree& b) { swap(a.root, b.root); }
  bool empty() const { return root == nullptr; }
  int size() const { return empty() ? 0 : root->size; }
  const T& top() const { return root->value; }
  void push(const T& v) { root = merge(root, new node_t(v)); }
  void pop() {
    if constexpr (has_push) root->push();
    node_t* new_root = merge(root->left, root->right);
    delete root;
    root = new_root;
  }
  void merge(leftist_tree& o) {
    root = merge(root, o.root);
    o.root = nullptr;
  }
  node_t* merge(node_t* a, node_t* b) {
    if (a == nullptr) return b;
    if (b == nullptr) return a;
    if (Compare()(b->value, a->value)) swap(a, b);
    if constexpr (has_push) a->push();
    a->right = merge(a->right, b);
    a->size = 1 + a->right->size + (a->left == nullptr ? 0 : a->left->size);
    if (a->left == nullptr || a->left->size < a->right->size) {
      swap(a->left, a->right);
    }
    return a;
  }
  template <typename U, bool can_push = has_push>
  std::enable_if_t<can_push> lazy_add(U val) {
    if (root != nullptr) {
      root->put(val);
    }
  }
};
