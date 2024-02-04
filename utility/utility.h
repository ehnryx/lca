/* stuff
 * USAGE
 *  don't
 * TIME
 *  idk
 * STATUS
 *  untested
 */
#pragma once

#include <algorithm>

namespace utility {
template <typename... T>
struct always_false {
  bool operator()(...) const { return false; }
};
template <typename... T>
struct always_true {
  bool operator()(...) const { return true; }
};

template <typename T = void, typename Compare = std::less<>>
struct min {
  T operator()(const T& a, const T& b) const { return std::min(a, b, Compare()); }
};
template <typename Compare>
struct min<void, Compare> {
  template <typename T>
  T operator()(const T& a, const T& b) const {
    return std::min<T>(a, b, Compare());
  }
};

template <typename T = void>
using max = min<T, std::greater<T>>;

template <typename T = void>
using keep = min<T, always_true<T>>;

template <typename T = void>
using replace = min<T, always_false<T>>;

template <typename T, T floor_v, typename Compare = std::less<>>
struct floored_min {
  T operator()(const T& a, const T& b) const {
    return std::max(std::min(a, b, Compare()), floor_v, Compare());
  }
};
}  // namespace utility
