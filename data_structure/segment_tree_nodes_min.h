/* Segment Tree Nodes
 * USAGE
 *  easy
 * STATUS
 *  untested
 */
#pragma once

#include <functional>

namespace segment_node {

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

}

