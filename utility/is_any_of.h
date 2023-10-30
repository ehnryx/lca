/* stuff
 * USAGE
 *  don't
 * TIME
 *  idk
 * STATUS
 *  untested
 */
#pragma once

#include <type_traits>

template <typename T, typename... Us>
struct is_any_of {
  static constexpr bool value = (std::is_same_v<T, Us> or ...);
};

template <typename T, typename... Us>
constexpr bool is_any_of_v = is_any_of<T, Us...>::value;
