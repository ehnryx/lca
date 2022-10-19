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
  template <typename T = void, class Compare = std::less<>>
  struct min {
    T operator () (const T& a, const T& b) const {
      return std::min<T>(a, b, Compare());
    }
  };
  template <class Compare>
  struct min<void, Compare> {
    template <typename input_t>
    input_t operator () (const input_t& a, const input_t& b) const {
      return std::min(a, b, std::less<>());
    }
  };

  // functional form of std::max
  template <typename T = void, class Compare = std::less<>>
  struct max {
    T operator () (const T& a, const T& b) const {
      return std::max<T>(a, b, Compare());
    }
  };
  template <class Compare>
  struct max<void, Compare> {
    template <typename input_t>
    input_t operator () (const input_t& a, const input_t& b) const {
      return std::max(a, b, std::less<>());
    }
  };

  // keep
  template <typename T = void>
  struct keep {
    T operator () (const T& old_v, const T&) const {
      return old_v;
    }
  };
  template <>
  struct keep<void> {
    template <typename input_t>
    input_t operator () (const input_t& old_v, const input_t&) const {
      return old_v;
    }
  };

  // replace
  template <typename T = void>
  struct replace {
    T operator () (const T&, const T& new_v) const {
      return new_v;
    }
  };
  template <>
  struct replace<void> {
    template <typename input_t>
    input_t operator () (const input_t&, const input_t& new_v) const {
      return new_v;
    }
  };
}

