/* For checking whether a class has a member variable by name
 * USAGE
 *  MEMBER_TYPE_GETTER(type_name);
 *  type <- _get_type_type_name<class, default>::type;
 */
#pragma once

#include <type_traits>

#define MEMBER_TYPE_GETTER(TYPE) \
  template <typename __class_t, typename __default_t, typename = void> \
  struct _get_type_##TYPE { \
    using type = __default_t; \
  }; \
  template <typename __class_t, typename __default_t> \
  struct _get_type_##TYPE<__class_t, __default_t, std::void_t<typename __class_t::TYPE>> { \
    using type = typename __class_t::TYPE; \
  }

