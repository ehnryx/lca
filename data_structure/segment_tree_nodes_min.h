/* Segment Tree Nodes
 * USAGE
 *  easy
 * STATUS
 *  untested
 */
#pragma once

#include <functional>

namespace segment_tree_nodes {

  template <typename T, typename Compare = std::less<>>
  struct custom_update_range_min {
    using out_t = T;
    T min;
    custom_update_range_min() = default;
    custom_update_range_min(T v): min(v) {}
    out_t get() const { return min; }
    void pull(const custom_update_range_min& left, const custom_update_range_min& right) {
      min = std::min(left.min, right.min, Compare());
    }
    static out_t merge(out_t left, out_t right) { return std::min(left, right, Compare()); }
  };

  template <typename T, typename Compare = std::less<>>
  struct range_add_range_min : custom_update_range_min<T> {
    T lazy;
    range_add_range_min() = default;
    range_add_range_min(T v): custom_update_range_min<T>(v) {}
    void put(T v) {
      lazy += v;
      this->min += v;
    }
    void push(range_add_range_min& left, range_add_range_min& right) {
      if (lazy != T(0)) {
        left.lazy += this->lazy;
        right.lazy += this->lazy;
        left.min += this->lazy;
        right.min += this->lazy;
        this->lazy = T(0);
      }
    }
    bool contains(T v) {
      return this->min <= v;
    }
  };

}

