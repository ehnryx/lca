/* Segment Tree Nodes
 * USAGE
 *  easy
 * STATUS
 *  untested
 */
#pragma once

namespace segment_tree_nodes {

template <typename T>
struct custom_update_range_sum {
  using out_t = T;
  T sum;
  custom_update_range_sum() = default;
  custom_update_range_sum(T v) : sum(v) {}
  out_t get() const { return sum; }
  void pull(const custom_update_range_sum& left, const custom_update_range_sum& right) {
    sum = left.sum + right.sum;
  }
  static out_t merge(out_t left, out_t right) { return left + right; }
  static constexpr out_t default_value() { return 0; }
};

template <typename T>
struct point_assign_range_sum : custom_update_range_sum<T> {
  point_assign_range_sum() = default;
  point_assign_range_sum(T v) : custom_update_range_sum<T>(v) {}
  void put(T v) { this->sum = v; }
};

template <typename T>
struct range_assign_range_sum : custom_update_range_sum<T> {
  int length;
  bool lazy;
  range_assign_range_sum() = default;
  range_assign_range_sum(T v) : custom_update_range_sum<T>(v), lazy(false) {}
  void put(T v) {
    this->sum = v * length;
    lazy = true;
  }
  void push(range_assign_range_sum& left, range_assign_range_sum& right) {
    if (lazy) {
      left.sum = right.sum = this->sum / 2;
      left.lazy = right.lazy = true;
      lazy = false;
    }
  }
};

template <typename T>
struct point_add_range_sum : custom_update_range_sum<T> {
  point_add_range_sum() = default;
  point_add_range_sum(T v) : custom_update_range_sum<T>(v) {}
  void put(T v) { this->sum += v; }
};

template <typename T>
struct range_add_range_sum : custom_update_range_sum<T> {
  T lazy;
  int length;
  range_add_range_sum() = default;
  range_add_range_sum(T v) : custom_update_range_sum<T>(v), lazy(v) {}
  void put(T v) {
    T add = v * length;
    lazy += add;
    this->sum += add;
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

}  // namespace segment_tree_nodes
