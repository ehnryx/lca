/* stuff
 * USAGE
 *  don't
 * TIME
 *  idk
 * STATUS
 *  untested
 */
#pragma once

namespace utility {
// keep
template <typename T = void>
struct keep {
  T operator()(const T& old_v, const T&) const { return old_v; }
};
template <>
struct keep<void> {
  template <typename input_t>
  input_t operator()(const input_t& old_v, const input_t&) const {
    return old_v;
  }
};

// replace
template <typename T = void>
struct replace {
  T operator()(const T&, const T& new_v) const { return new_v; }
};
template <>
struct replace<void> {
  template <typename input_t>
  input_t operator()(const input_t&, const input_t& new_v) const {
    return new_v;
  }
};
}  // namespace utility
