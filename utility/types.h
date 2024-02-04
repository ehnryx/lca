/* Type Traits
 * STATUS
 *  untested
 */
#pragma once

#include <cstdint>
#include <type_traits>

namespace utility_details {
template <typename T>
struct bigger {
  using type = T;
};
template <typename T>
  requires(std::is_integral_v<T> and std::is_signed_v<T>)
struct bigger<T> {
  using type = long long;
};
template <typename T>
  requires(std::is_integral_v<T> and std::is_unsigned_v<T>)
struct bigger<T> {
  using type = unsigned long long;
};
template <>
struct bigger<long long> {
  using type = __int128;
};
template <>
struct bigger<unsigned long long> {
  using type = unsigned __int128;
};
}  // namespace utility_details

namespace utility {
template <typename T>
using bigger_type = typename utility_details::bigger<T>::type;
}
