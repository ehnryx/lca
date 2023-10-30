/* Segment Tree Nodes
 * USAGE
 *  easy
 * STATUS
 *  untested
 */
#pragma once

namespace segment_tree_nodes {

  template <typename T>
  struct custom_update_point_query {
    using out_t = T;
    T value;
    custom_update_point_query() = default;
    custom_update_point_query(T v): value(v) {}
    out_t get() const { return value; }
  };

  template <typename T>
  struct range_add_point_query : custom_update_point_query<T> {
    range_add_point_query() = default;
    range_add_point_query(T v): custom_update_point_query<T>(v) {}
    void put(T v) { this->value += v; }
    static T merge(T left, T right) { return left + right; }
    static constexpr T default_value() { return 0; }
  };

}
