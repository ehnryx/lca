/* Segment Tree
 * USAGE
 *  segment_tree<Node_t, Query_t> segtree(n); initializes a segtree with >= n leaves
 *  segment_tree<Node_t, Query_t> segtree(vector); initializes a segment tree with given values
 *  template: <Node_t, Query_t, break_conditions?>
 *  Node_t is a class to be implemented, should have the members:
 *    void put(args...); update at node
 *    Query_t get(args...); gets the return value from node
 *    static Query_t merge(const Query_t& l, const Query_t& r); merges the return values
 *    static Query_t default_value(); default value for return (when node out of range)
 *    void pull(const Node_t& l, const Node_t& r); pulls values from children
 *    void push(Node_t& l, Node_t& r); pushes lazy to l and r
 *    //for searching on the segment tree
 *    bool contains(args&...); does the segment contain the value? update args if not
 *    bool found(args...); does the leaf correspond to the value that we want?
 *    //break conditions
 *    bool put_condition(args...); whether to update in segtree beats
 * MEMBERS
 *  * All ranges are inclusive
 *  update(l, r, value...); value for range update
 *  query(l, r, ...); query with optional args
 *  update_point(x, value...); point update
 *  query_point(x, ...); point query
 *  search_left(l, r, ...); search on the segtree, starting from the left
 *  search_right(l, r, ...); search on the segtree, starting from the right
 *  returns segment_tree.lim if not found (ie. n in the constructor)
 * TIME
 *  O(logN) per query
 *  N = |array|
 * STATUS
 *  tested: cf/474f,102875a,1515i, acmhdu/5306
 */
#pragma once

#include "../misc/member_function_checker.h"

template <class Node_t, typename Query_t, bool break_cond = false>
struct segment_tree {
  MEMBER_FUNCTION_CHECKER(push);
  MEMBER_FUNCTION_CHECKER(pull);
  MEMBER_FUNCTION_CHECKER(default_value);
  static constexpr bool has_push = _has_push<Node_t>::value;
  static constexpr bool has_pull = _has_pull<Node_t>::value;
  static constexpr bool has_default_value = _has_default_value<Node_t>::value;

  int lim, length;
  vector<Node_t> data;
  Node_t& operator [] (int i) { return data[i]; }

  segment_tree(int n): lim(n),
    length(1 << (lim == 1 ? 0 : 32 - __builtin_clz(lim - 1))), data(2 * length) {}
  template <class Input_t>
  segment_tree(const vector<Input_t>& a, int offset = 0): lim((int)size(a)),
    length(1 << (lim == 1 ? 0 : 32 - __builtin_clz(lim - 1))), data(2*length) {
    for (int i = offset; i < lim; i++) {
      data[length + i] = Node_t(a[i]);
    }
    build();
  }
  void build() {
    for (int i = length - 1; i > 0; i--) {
      if constexpr (has_pull) data[i].pull(data[2*i], data[2*i + 1]);
    }
  }
  void assign_lengths() {
    for (int i = 0; i < length; i++) {
      data[i + length].length = 1;
    }
    for (int i = length - 1; i > 0; i--) {
      data[i].length = data[2 * i].length + data[2 * i + 1].length;
    }
  }

  template <class... Args>
  void update_range(int l, int r, const Args&... args) {
    update(l, r, args...);
  }
  template <class... Args>
  void update(int l, int r, const Args&... args) {
    if (r < l) return;
    if (l < 0 || lim <= r) throw invalid_argument("update range out of bounds");
    __update(l, r, 1, 0, length - 1, args...);
  }
  template <class... Args>
  void __update(int l, int r, int i, int first, int last, const Args&... args) {
    if constexpr (break_cond) {
      if (data[i].break_condition(args...)) return;
      if (l <= first && last <= r && data[i].put_condition(args...)) {
        return data[i].put(args...);
      }
      if (i >= length) {
        throw invalid_argument("put_condition/break_condition is incorrect, "
                               "trying to descend past a leaf");
      }
    } else {
      if (l <= first && last <= r) {
        return data[i].put(args...);
      }
    }
    if constexpr (has_push) data[i].push(data[2*i], data[2*i + 1]);
    int mid = (first + last) / 2;
    if(l <= mid) __update(l, r, 2*i, first, mid, args...);
    if(mid < r) __update(l, r, 2*i + 1, mid + 1, last, args...);
    if constexpr (has_pull) data[i].pull(data[2*i], data[2*i + 1]);
  }

  template <class... Args>
  Query_t query_range(int l, int r, const Args&... args) {
    return query(l, r, args...);
  }
  template <class... Args>
  Query_t query(int l, int r, const Args&... args) {
    if (r < l) {
      if constexpr (has_default_value) return Node_t::default_value();
      else assert(false);
    }
    if (l < 0 || lim <= r) throw invalid_argument("query range out of bounds");
    return __query(l, r, 1, 0, length - 1, args...);
  }
  template <class... Args>
  Query_t __query(int l, int r, int i, int first, int last, const Args&... args) {
    if (l <= first && last <= r) return data[i].get(args...);
    if constexpr (has_push) data[i].push(data[2*i], data[2*i + 1]);
    int mid = (first + last) / 2;
    if(r <= mid) return __query(l, r, 2*i, first, mid, args...);
    if(mid < l) return __query(l, r, 2*i + 1, mid + 1, last, args...);
    return Node_t::merge(
        __query(l, r, 2*i, first, mid, args...),
        __query(l, r, 2*i + 1, mid + 1, last, args...));
  }

  template <class... Args>
  void update_point(int x, const Args&... args) {
    if (x < 0 || lim <= x) throw invalid_argument("update_point index out of bounds");
    __update_point(x, 1, 0, length - 1, args...);
  }
  template <class... Args>
  void __update_point(int x, int i, int first, int last, const Args&... args) {
    if (first == last) return data[i].put(args...);
    if constexpr (has_push) data[i].push(data[2*i], data[2*i + 1]);
    int mid = (first + last) / 2;
    if (x <= mid) __update_point(x, 2*i, first, mid, args...);
    else __update_point(x, 2*i + 1, mid + 1, last, args...);
    if constexpr (has_pull) data[i].pull(data[2*i], data[2*i + 1]);
  }

  template <class... Args>
  Query_t query_point(int x, const Args&... args) {
    if (x < 0 || lim <= x) throw invalid_argument("query_point index out of bounds");
    return __query_point(x, 1, 0, length - 1, args...);
  }
  template <class... Args>
  Query_t __query_point(int x, int i, int first, int last, const Args&... args) {
    if (first == last) return data[i].get(args...);
    if constexpr (has_push) data[i].push(data[2*i], data[2*i + 1]);
    int mid = (first + last) / 2;
    if (x <= mid) return __query_point(x, 2*i, first, mid, args...);
    else return __query_point(x, 2*i + 1, mid + 1, last, args...);
  }

  template <class... Args>
  void update_up(int x, const Args&... args) {
    static_assert(!has_push);
    if (x < 0 || lim <= x) throw invalid_argument("update_up index out of bounds");
    for (int i = x + length; i > 0; i /= 2) {
      data[i].put(args...);
    }
  }

  template <class... Args>
  Query_t query_up(int x, const Args&... args) {
    if (x < 0 || lim <= x) throw invalid_argument("query_up index out of bounds");
    return __query_up(x, 1, 0, length - 1, args...);
  }
  template <class... Args>
  Query_t __query_up(int x, int i, int first, int last, const Args&... args) {
    if (first == last) return data[i].get(args...);
    if constexpr (has_push) data[i].push(data[2*i], data[2*i + 1]);
    int mid = (first + last) / 2;
    if (x <= mid) {
      return Node_t::merge(data[i].get(args...), __query_up(x, 2*i, first, mid, args...));
    } else {
      return Node_t::merge(data[i].get(args...), __query_up(x, 2*i + 1, mid + 1, last, args...));
    }
  }

  template <class... Args>
  int search_left(int l, int r, Args... args) {
    if (r < l) return lim;
    if (l < 0 || lim <= r) throw invalid_argument("search_left range out of bounds");
    return __search_left(l, r, 1, 0, length - 1, forward_as_tuple(args...));
  }
  template <class... Args>
  int search_left_mutable(int l, int r, Args&... args) {
    if (r < l) return lim;
    if (l < 0 || lim <= r) throw invalid_argument("search_left range out of bounds");
    return __search_left(l, r, 1, 0, length - 1, forward_as_tuple(args...));
  }
  template <class... Args>
  int __search_left(int l, int r, int i, int first, int last, tuple<Args&...> args) {
    if (l <= first && last <= r
        && !apply(&Node_t::contains, tuple_cat(tuple(data[i]), args))) return lim;
    if (first == last) return first;
    if constexpr (has_push) data[i].push(data[2*i], data[2*i + 1]);
    int mid = (first + last) / 2;
    int res = (l <= mid ? __search_left(l, r, 2*i, first, mid, args) : lim);
    if (res == lim && mid < r) res = __search_left(l, r, 2*i + 1, mid + 1, last, args);
    return res;
  }

  template <class... Args>
  int search_right(int l, int r, Args... args) {
    if (r < l) return lim;
    if (l < 0 || lim <= r) throw invalid_argument("search_right range out of bounds");
    return __search_right(l, r, 1, 0, length - 1, forward_as_tuple(args...));
  }
  template <class... Args>
  int search_right_mutable(int l, int r, Args&... args) {
    if (r < l) return lim;
    if (l < 0 || lim <= r) throw invalid_argument("search_right range out of bounds");
    return __search_right(l, r, 1, 0, length - 1, forward_as_tuple(args...));
  }
  template <class... Args>
  int __search_right(int l, int r, int i, int first, int last, tuple<Args&...> args) {
    if (l <= first && last <= r
        && !apply(&Node_t::contains, tuple_cat(tuple(data[i]), args))) return lim;
    if (first == last) return first;
    if constexpr (has_push) data[i].push(data[2*i], data[2*i + 1]);
    int mid = (first + last) / 2;
    int res = (mid < r ? __search_right(l, r, 2*i + 1, mid + 1, last, args) : lim);
    if (res == lim && l <= mid) res = __search_right(l, r, 2*i, first, mid, args);
    return res;
  }
};

