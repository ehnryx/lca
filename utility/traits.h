/* Traits (for templates, with operators)
 * USAGE:
 *  MAKE_TRAITS(traits_name,
 *    (trait1, trait2, trait3),
 *    combination1 = trait1 | trait2,
 *    combination2 = trait1 | trait2 | trait3,
 *  );
 *  auto t = traits_name::trait1; // eg.
 * STATUS:
 *  untested
 */
#pragma once

#include "macros.h"

#include <array>
#include <bit>
#include <cassert>
#include <string_view>
#include <vector>

namespace detail {
template <size_t N>
struct get_traits_type {
  static_assert(N <= 64);
  using type = std::conditional_t<
      N <= 16, std::conditional_t<N <= 8, uint8_t, uint16_t>,
      std::conditional_t<N <= 32, uint32_t, uint64_t>>;
};
}  // namespace detail

// clang-format off
#define _ADD_TRAIT_TO_SIZE(field_name) size += 1;

#define _GET_TRAITS_SIZE(...) \
  [] { \
    size_t size = 0; \
    FOR_EACH(_ADD_TRAIT_TO_SIZE, __VA_ARGS__) \
    return size; \
  }()

#define _ADD_TRAIT_TO_ARRAY(field_name) \
  for (size_t i = 0; i < index; i++) { \
    assert(traits_array[i] != #field_name); \
  } \
  traits_array[index++] = #field_name;

#define _MAKE_TRAITS_ARRAY(...) \
  [] { \
    std::array<std::string_view, _GET_TRAITS_SIZE(__VA_ARGS__)> traits_array; \
    size_t index = 0; \
    FOR_EACH(_ADD_TRAIT_TO_ARRAY, __VA_ARGS__) \
    return traits_array; \
  }()

#define _MAKE_BINARY_OPERATOR(traits_name, op, ...) \
  constexpr traits_name& operator op##= (type other) { \
    value op##= other; return *this; \
  } \
  constexpr traits_name operator op (type other) const { \
    return traits_name{*this} &= other; \
  } \
  constexpr traits_name& operator op##= (traits_name other) { \
    value op##= other.value; return *this; \
  } \
  constexpr traits_name operator op (traits_name other) const { \
    return traits_name{*this} &= other; \
  }

#define _MAKE_FIELD_CUSTOM(field_expr) \
  static constexpr type field_expr;

#define _MAKE_FIELD_UNIT(field_name) \
  static constexpr type field_name = 1 << ( \
      find(traits_array.begin(), traits_array.end(), #field_name) - traits_array.begin());

#define _MAKE_TRAITS_FIELDS(...) \
  FOR_EACH(_MAKE_FIELD_UNIT, __VA_ARGS__)

#define MAKE_TRAITS(traits_name, traits_fields, ...) \
  struct traits_name { \
    static constexpr auto traits_array = _MAKE_TRAITS_ARRAY traits_fields; \
    static constexpr size_t size = traits_array.size(); \
    using type = detail::get_traits_type<size>::type; \
    type value; \
    constexpr traits_name(type v): value(v) {} \
    constexpr operator bool() const { return value; } \
    constexpr bool operator==(traits_name other) const { return value == other.value; } \
    constexpr bool operator==(type other) const { return value == other; } \
    _MAKE_BINARY_OPERATOR(traits_name, &) \
    _MAKE_BINARY_OPERATOR(traits_name, |) \
    constexpr bool has_all(traits_name query) const { \
      return type(value & query.value) == query.value; \
    } \
    constexpr bool has_any(traits_name query) const { \
      return type(value & query.value) != 0; \
    } \
    constexpr size_t count(traits_name query) const { \
      return std::popcount(type(value & query.value)); \
    } \
    _MAKE_TRAITS_FIELDS traits_fields \
    FOR_EACH(_MAKE_FIELD_CUSTOM, __VA_ARGS__) \
    static constexpr type NONE = 0; \
  }
// clang-format on
