/* Splay Tree
 * USAGE
 *  common nodes for splay trees
 * STATUS
 *  tested boj/2927
 */
#pragma once

#include <utility>

template <typename derived_t>
struct splay_node_base_common {
  static derived_t* const nil;
  derived_t *parent, *left, *right;
  int size;
  splay_node_base_common(int s) : parent(nil), left(nil), right(nil), size(s) {}
  bool is_left_child() const { return parent == nil || this == parent->left; }
  derived_t* get_child(bool is_left) const { return is_left ? left : right; }
  static derived_t* _get_nil() {
    static derived_t _nil{};
    for (static bool once = true; once; once = false) {
      _nil.parent = _nil.left = _nil.right = &_nil;
      _nil.size = 0;
    }
    return &_nil;
  }
};

template <typename derived_t, typename _key_t, typename _value_t, typename = void>
struct splay_node_base : splay_node_base_common<derived_t> {
  using key_t = _key_t;
  using value_t = _value_t;
  using out_t = std::pair<key_t&, value_t&>;  // store as tuple
  key_t key;
  value_t value;
  splay_node_base() : splay_node_base_common<derived_t>(0), key(), value() {}
  splay_node_base(const key_t& k, const value_t& v)
      : splay_node_base_common<derived_t>(1), key(k), value(v) {}
  bool operator<(const derived_t& other) const { return key < other.key; }
  out_t get_value() { return out_t(ref(key), ref(value)); }
};

template <typename derived_t, typename _key_t, typename _value_t>
struct splay_node_base<
    derived_t, _key_t, _value_t,
    std::enable_if_t<std::is_void_v<_key_t> && !std::is_void_v<_value_t>>>
    : splay_node_base_common<derived_t> {
  using key_t = _key_t;
  using value_t = _value_t;
  using out_t = value_t&;
  value_t value;
  splay_node_base() : splay_node_base_common<derived_t>(0), value() {}
  splay_node_base(const value_t& v) : splay_node_base_common<derived_t>(1), value(v) {}
  out_t get_value() { return value; }
};

template <typename derived_t, typename _key_t, typename _value_t>
struct splay_node_base<
    derived_t, _key_t, _value_t,
    std::enable_if_t<!std::is_void_v<_key_t> && std::is_void_v<_value_t>>>
    : splay_node_base_common<derived_t> {
  using key_t = _key_t;
  using value_t = _value_t;
  using out_t = key_t&;
  key_t key;
  splay_node_base() : splay_node_base_common<derived_t>(0), key() {}
  splay_node_base(const key_t& k) : splay_node_base_common<derived_t>(1), key(k) {}
  bool operator<(const derived_t& other) const { return key < other.key; }
  out_t get_value() { return key; }
};

template <typename derived_t, typename _key_t, typename _value_t>
struct splay_node_base<
    derived_t, _key_t, _value_t,
    std::enable_if_t<std::is_void_v<_key_t> && std::is_void_v<_value_t>>>
    : splay_node_base_common<derived_t> {
  using key_t = _key_t;
  using value_t = _value_t;
  using out_t = void;
  splay_node_base() : splay_node_base_common<derived_t>(1) {}
  out_t get_value() { return; }
};

template <typename _key_t, typename _value_t>
struct splay_node final : splay_node_base<splay_node<_key_t, _value_t>, _key_t, _value_t> {
  using splay_node_base<splay_node<_key_t, _value_t>, _key_t, _value_t>::splay_node_base;
};

template <typename derived_t>
derived_t* const splay_node_base_common<derived_t>::nil = derived_t::_get_nil();
