/* ostream& operator <<
 * STATUS
 *  untested ?
 */
#pragma once

#include <ostream>
#include <set>
#include <tuple>
#include <unordered_set>
#include <vector>

template <template <typename> typename Container, typename T,
          typename = std::enable_if_t<std::disjunction_v<
            std::is_same<Container<T>, std::vector<T>>,
            std::is_same<Container<T>, std::set<T>>,
            std::is_same<Container<T>, std::unordered_set<T>>>>
         >
std::ostream& operator << (std::ostream& os, const Container<T>& v) {
  if (v.begin() == v.end()) return os;
  os << *v.begin();
  for (auto it = v.begin() + 1; it != v.end(); it++) {
    os << " " << *it;
  }
  return os;
}

template <typename Iterator>//, typename = std::iter_value_t<Iterator>>
struct container_view {
  const Iterator _begin, _end;
  container_view(const Iterator s, const Iterator t):
    _begin(s), _end(t) {}
  const Iterator begin() const { return _begin; }
  const Iterator end() const { return _end; }
  friend std::ostream& operator << (std::ostream& os, container_view v) {
    if (v.begin() == v.end()) return os;
    os << *v.begin();
    for (auto it = v.begin() + 1; it != v.end(); it++) {
      os << " " << *it;
    }
    return os;
  }
};

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

