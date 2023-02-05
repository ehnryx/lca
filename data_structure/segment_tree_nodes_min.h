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
  struct range_add_range_min : custom_update_range_min<T, Compare> {
    T lazy;
    range_add_range_min() = default;
    range_add_range_min(T v): custom_update_range_min<T, Compare>(v), lazy(0) {}
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

  template <typename T, typename Compare = std::less<>>
  struct range_min_range_min : custom_update_range_min<T, Compare> {
    T new_min;
    bool lazy = false;
    range_min_range_min() = default;
    range_min_range_min(T v): custom_update_range_min<T, Compare>(v) {}
    void put(T v) {
      if (lazy) {
        new_min = std::min(new_min, v, Compare());
      } else {
        new_min = v;
        lazy = true;
      }
      this->min = std::min(this->min, v, Compare());
    }
    void push(range_min_range_min& left, range_min_range_min& right) {
      if (lazy) {
        left.put(new_min);
        right.put(new_min);
        this->lazy = false;
      }
    }
    bool contains(T v) {
      return this->min <= v;
    }
  };
}

