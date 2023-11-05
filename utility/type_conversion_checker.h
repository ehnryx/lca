/* For checking whether one can convert to bool
 * USAGE
 *  bool <- _conerts_to_bool<class>::value;
 */
#pragma once

#include <type_traits>

template <typename __class_t>
struct _converts_to_bool {
  template <typename __class>
  static constexpr auto check(__class*) ->
    typename std::is_void<std::void_t<decltype(bool(std::declval<__class&>()))>>::type;
  template <typename __dummy>
  static constexpr auto check(...) -> std::false_type;
  using type = decltype(check<__class_t>(0));
  static constexpr bool value = type::value;
};

