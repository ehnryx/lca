/* For checking whether a class has a member function by name
 * USAGE
 *  MEMBER_FUNCTION_CHECKER(function_name);
 *  bool <- _has_function_name(class);
 */
#pragma once

#define MEMBER_FUNCTION_CHECKER(FUNCTION) \
  template <typename __class_t, typename... __args_t> \
  struct _has_##FUNCTION { \
    template <typename __class> \
    static constexpr auto check(__class*) -> \
      typename std::is_void<std::void_t< \
        decltype(std::declval<__class&>().get(std::declval<__args_t>()...))>>::type; \
    static constexpr auto check(...) -> std::false_type; \
    using type = decltype(check<__class_t>(0)); \
    static constexpr bool value = type::value; \
  }

