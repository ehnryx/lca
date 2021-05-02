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

template <typename T, class Compare = less<>>
struct leftist_tree {
  struct leftist_node {
    T value;
    int size;
    leftist_node *left, *right;
    leftist_node(const T& val):
      value(val), size(1), left(nullptr), right(nullptr) {}
  };
  leftist_node *root;
  leftist_tree(): root(nullptr) {}
  bool empty() const { return root == nullptr; }
  int size() const { return empty() ? 0 : root->size; }
  const T& top() const { return root->value; }
  void push(const T& v) {
    root = merge(root, new leftist_node(v));
  }
  void pop() {
    leftist_node *new_root = merge(root->left, root->right);
    delete root;
    root = new_root;
  }
  void merge(leftist_tree<T, Compare>& o) {
    root = merge(root, o.root);
    o.root = nullptr;
  }
  leftist_node* merge(leftist_node *a, leftist_node *b) {
    if (a == nullptr) return b;
    if (b == nullptr) return a;
    if (Compare()(b->value, a->value)) swap(a, b);
    a->right = merge(a->right, b);
    a->size = 1 + a->right->size + (a->left == nullptr ? 0 : a->left->size);
    if (a->left == nullptr || a->left->size < a->right->size) {
      swap(a->left, a->right);
    }
    return a;
  }
};

