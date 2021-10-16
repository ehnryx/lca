/* For checking whether a class has a member function by name
 * USAGE
 *  MEMBER_FUNCTION_CHECKER(function_name);
 *  bool <- _has_function_name(class);
 */
#pragma once

#define MEMBER_FUNCTION_CHECKER(function) \
  template <typename T> \
  struct _has_##function { \
    using other = struct { char x[2]; }; \
    template <typename C> static char test(decltype(&C::function)); \
    template <typename C> static other test(...); \
    enum { value = sizeof(test<T>(0)) == sizeof(char) }; \
  }

