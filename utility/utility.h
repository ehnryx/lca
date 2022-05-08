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

  // functional form of std::min

  template <typename T = void>
  struct min {
    T operator () (const T& a, const T& b) const {
      return std::min(a, b);
    }
  };
  template <>
  struct min<void> {
    template <typename input_t>
    input_t operator () (const input_t& a, const input_t& b) const {
      return std::min(a, b);
    }
  };

  // functional form of std::max

  template <typename T = void>
  struct max {
    T operator () (const T& a, const T& b) const {
      return std::max(a, b);
    }
  };
  template <>
  struct max<void> {
    template <typename input_t>
    input_t operator () (const input_t& a, const input_t& b) const {
      return std::max(a, b);
    }
  };

}

