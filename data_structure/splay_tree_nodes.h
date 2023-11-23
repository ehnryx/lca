/* Segment Tree Nodes
 * USAGE
 *  inherit
 * STATUS
 *  doesn't compile
 */
#pragma once

#include "splay_tree_nodes_common.h"

namespace splay_tree_nodes {
template <typename derived_t, typename _key_t, typename _value_t>
struct custom_range_sum : splay_node_base<derived_t, _key_t, _value_t> {
  _value_t sum;
  custom_range_sum() : splay_node_base<derived_t, _key_t, _value_t>(), sum(0) {}
  custom_range_sum(const _key_t& k, const _value_t& v)
      : splay_node_base<derived_t, _key_t, _value_t>(k, v), sum(0) {}
  void pull() { sum = this->left->sum + this->value + this->right->sum; }
};

template <typename _key_t, typename _value_t>
struct range_sum final : custom_range_sum<range_sum<_key_t, _value_t>, _key_t, _value_t> {
  using custom_range_sum<range_sum<_key_t, _value_t>, _key_t, _value_t>::custom_range_sum;
};
}  // namespace splay_tree_nodes
