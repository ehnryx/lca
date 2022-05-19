/* For checking whether a class has a member variable by name
 * USAGE
 *  MEMBER_VARIABLE_CHECKER(variable_name);
 *  bool <- _has_variable_name<class>::value;
 */
#pragma once

#define MEMBER_VARIABLE_CHECKER(VARIABLE) \
  template <typename __class_t> \
  struct _has_##VARIABLE { \
    template <typename __class> \
    static constexpr auto check(__class*) -> \
      typename std::is_void<std::void_t< \
        decltype(std::declval<__class&>().VARIABLE)>>::type; \
    template <typename __dummy> \
    static constexpr auto check(...) -> std::false_type; \
    using type = decltype(check<__class_t>(0)); \
    static constexpr bool value = type::value; \
  }

