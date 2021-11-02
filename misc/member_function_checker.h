/* For checking whether a class has a member function by name
 * USAGE
 *  MEMBER_FUNCTION_CHECKER(function_name);
 *  bool <- _has_function_name(class);
 */
#pragma once

#define MEMBER_FUNCTION_CHECKER(function) \
  template <typename _to_check> \
  struct _has_##function { \
    using other = struct { char x[2]; }; \
    template <typename __to_check> static char test(decltype(&__to_check::function)); \
    template <typename __to_check> static other test(...); \
    enum { value = sizeof(test<_to_check>(0)) == sizeof(char) }; \
  }

