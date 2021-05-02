/* Segment Tree
 * USAGE
 *  segment_tree<Node_t, Query_t> segtree(n); initializes a segtree with >= n leaves
 *  segment_tree<Node_t, Query_t> segtree(vector); initializes a segment tree with given values
 *    requires #ifdef USE_LINEAR_BUILD
 *  Node_t is a class to be implemented, should have the members:
 *    void put(int segment_length, args...); update at node
 *    Query_t get(int segment_length, args...); gets the return value from node
 *    static Query_t merge(const Query_t& l, const Query_t& r); merges the return values
 *    static Query_t default_value(); default value for return (when node out of range)
 *    void pull(const Node_t& l, const Node_t& r); pulls values from children
 *    #ifndef USE_NO_PUSH
 *    void push(Node_t& l, Node_t& r); pushes lazy to l and r
 *    #ifdef USE_SEGMENT_TREE_BEATS
 *    bool put_condition(int segment_length, args...); whether to update in segtree beats
 *    bool get_condition(int segment_length, args...); whether to return in segtree beats
 * MEMBERS
 *  update(l, r, value); value for range update
 *  query(l, r, ...); query with optional args
 * TIME
 *  O(logN) per query
 *  N = |array|
 * STATUS
 *  tested: cf/474f,102875a
 */
#pragma once

template <class Node_t, typename Query_t>
struct segment_tree {
  int length;
  vector<Node_t> data;
  segment_tree(int n) {
    length = 1 << (n == 1 ? 0 : 32 - __builtin_clz(n - 1));
    data.resize(2 * length);
  }
  template <class Input_t>
  segment_tree(const vector<Input_t>& a, int offset = 0) {
    int n = (int)size(a);
    length = 1 << (n == 1 ? 0 : 32 - __builtin_clz(n - 1));
    data.resize(2 * length);
    for (int i = offset; i < n; i++) {
      data[length + i] = Node_t(a[i]);
    }
    for (int i = length - 1; i > 0; i--) {
      data[i].pull(data[2*i], data[2*i + 1]);
    }
  }

  template <class... Args>
  void update(int l, int r, const Args&... args) {
    update(l, r, 1, 0, length - 1, args...);
  }
  template <class... Args>
  void update(int l, int r, int i, int first, int last, const Args&... args) {
    if (last < l || r < first) return;
    if (l <= first && last <= r
#ifdef USE_SEGTREE_BEATS
        && data[i].put_condition(last - first + 1, args...)
#endif
        ) {
      return data[i].put(last - first + 1, args...);
    }
#ifndef USE_NO_PUSH
    data[i].push(data[2*i], data[2*i + 1]);
#endif
    int mid = (first + last) / 2;
    update(l, r, 2*i, first, mid, args...);
    update(l, r, 2*i + 1, mid + 1, last, args...);
    data[i].pull(data[2*i], data[2*i + 1]);
  }

  template <class... Args>
  Query_t query(int l, int r, const Args&... args) {
    return query(l, r, 1, 0, length - 1, args...);
  }
  template <class... Args>
  Query_t query(int l, int r, int i, int first, int last, const Args&... args) {
    if (last < l || r < first) return Node_t::default_value();
    if (l <= first && last <= r
#ifdef USE_SEGTREE_BEATS
        && data[i].get_condition(last - first + 1, args...)
#endif
        ) {
      return data[i].get(last - first + 1, args...);
    }
#ifndef USE_NO_PUSH
    data[i].push(data[2*i], data[2*i + 1]);
#endif
    int mid = (first + last) / 2;
    Query_t left = query(l, r, 2*i, first, mid, args...);
    Query_t right = query(l, r, 2*i + 1, mid + 1, last, args...);
    return Node_t::merge(left, right);
  }
};

