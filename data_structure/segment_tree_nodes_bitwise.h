/* Segment Tree Nodes
 * USAGE
 *  easy
 * STATUS
 *  untested
 */
#pragma once

namespace segment_node {

  template <typename T>
  struct custom_update_range_and {
    using out_t = T;
    T value;
    custom_update_range_and() = default;
    custom_update_range_and(T v): value(v) {}
    out_t get() const { return value; }
    void pull(const custom_update_range_and& left, const custom_update_range_and& right) {
      value = left.value & right.value;
    }
    static out_t merge(out_t left, out_t right) { return left & right; }
  };

  template <typename T>
  struct range_assign_range_and final : custom_update_range_and<T> {
    using Base = custom_update_range_and<T>;
    using Base::value;
    bool lazy;
    range_assign_range_and() = default;
    range_assign_range_and(T v): Base(v), lazy(false) {}
    void put(T v) {
      value = v;
      lazy = true;
    }
    void push(range_assign_range_and& left, range_assign_range_and& right) {
      if (lazy) {
        left.value = right.value = value;
        left.lazy = right.lazy = true;
        lazy = false;
      }
    }
  };

  template <typename T>
  struct custom_update_range_or {
    using out_t = T;
    T value;
    custom_update_range_or() = default;
    custom_update_range_or(T v): value(v) {}
    out_t get() const { return value; }
    void pull(const custom_update_range_or& left, const custom_update_range_or& right) {
      value = left.value | right.value;
    }
    static out_t merge(out_t left, out_t right) { return left | right; }
  };

  template <typename T>
  struct range_assign_range_or final : custom_update_range_or<T> {
    using Base = custom_update_range_or<T>;
    using Base::value;
    bool lazy;
    range_assign_range_or() = default;
    range_assign_range_or(T v): Base(v), lazy(false) {}
    void put(T v) {
      value = v;
      lazy = true;
    }
    void push(range_assign_range_or& left, range_assign_range_or& right) {
      if (lazy) {
        left.value = right.value = value;
        left.lazy = right.lazy = true;
        lazy = false;
      }
    }
  };

}

