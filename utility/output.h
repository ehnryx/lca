/* ostream& operator <<
 * STATUS
 *  untested ?
 */
#pragma once

#include <ostream>
#include <tuple>

template <template <typename> typename Container, typename T>
std::ostream& operator << (std::ostream& os, const Container<T>& v) {
  for (size_t i = 0; i < v.size(); i++) {
    if (i) os << " ";
    os << v[i];
  }
  return os;
}

template <typename T, typename U>
std::ostream& operator << (std::ostream& os, const std::pair<T, U>& v) {
  return os << v.first << " " << v.second;
}

namespace {
  template <typename Tuple, size_t... Is>
  auto _skip_first(const Tuple& t, std::index_sequence<Is...>) {
    return std::make_tuple(std::get<Is + 1>(t)...);
  }
}

template <typename T>
std::ostream& operator << (std::ostream& os, const std::tuple<T>& v) {
  return os << std::get<0>(v);
}

template <typename... T>
std::ostream& operator << (std::ostream& os, const std::tuple<T...>& v) {
  return os << std::get<0>(v) << " "
    << _skip_first(v, std::make_index_sequence<sizeof...(T) - 1>{});
}

