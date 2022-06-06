/* Segment Tree Nodes
 * USAGE
 *  easy
 * STATUS
 *  untested
 */
#pragma once

namespace segment_node {

  template <typename T>
  struct custom_update_range_sum {
    using out_t = T;
    T sum;
    custom_update_range_sum() = default;
    custom_update_range_sum(T v): sum(v) {}
    out_t get() const { return sum; }
    void pull(const custom_update_range_sum& left, const custom_update_range_sum& right) {
      sum = left.sum + right.sum;
    }
    static out_t merge(out_t left, out_t right) { return left + right; }
  };

  template <typename T>
  struct range_assign_range_sum final : custom_update_range_sum<T> {
    using Base = custom_update_range_sum<T>;
    using Base::sum;
    int length;
    bool lazy;
    range_assign_range_sum() = default;
    range_assign_range_sum(T v): Base(v), lazy(false) {}
    void put(T v) {
      sum = v * length;
      lazy = true;
    }
    void push(range_assign_range_sum& left, range_assign_range_sum& right) {
      if (lazy) {
        left.sum = right.sum = sum / 2;
        left.lazy = right.lazy = true;
        lazy = false;
      }
    }
  };

  template <typename T>
  struct range_add_range_sum final : custom_update_range_sum<T> {
    using Base = custom_update_range_sum<T>;
    using Base::sum;
    T lazy;
    int length;
    range_add_range_sum() = default;
    range_add_range_sum(T v): Base(v), lazy(v) {}
    void put(T v) {
      T add = v * length;
      lazy += add;
      sum += add;
    }
    void push(range_add_range_sum& left, range_add_range_sum& right) {
      if (lazy != T(0)) {
        T add = lazy / 2;
        left.lazy += add;
        right.lazy += add;
        left.sum += add;
        right.sum += add;
        lazy = T(0);
      }
    }
  };

}

