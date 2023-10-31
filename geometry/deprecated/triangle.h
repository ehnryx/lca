/* Triangles
 *  predicates dealing with 3 points
 * STATUS
 *  mostly untested
 */
#pragma once

#include "../utility/is_any_of.h"
#include "point.h"

template <typename T, typename strict = void, std::enable_if_t<point<T>::floating, bool> = true>
bool in_circle(T eps,
    const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& v) {
  static_assert(is_any_of_v<strict, void, geo::strict>);
  const auto av = a - v;
  const auto bv = b - v;
  const auto cv = c - v;
  auto value = cross(av, bv) * norm(cv) + cross(bv, cv) * norm(av) + cross(cv, av) * norm(bv);
  if (cross(a - c, b - c) < 0) value = -value;
  if constexpr (std::is_same_v<strict, geo::strict>) {
    return value > eps;
  } else {
    return value > -eps;
  }
}

template <typename T, typename strict = void, std::enable_if_t<not point<T>::floating, bool> = true>
bool in_circle(const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& v) {
  static_assert(is_any_of_v<strict, void, geo::strict>);
  const auto av = a - v;
  const auto bv = b - v;
  const auto cv = c - v;
  auto value = cross(av, bv) * norm(cv) + cross(bv, cv) * norm(av) + cross(cv, av) * norm(bv);
  if (cross(a - c, b - c) < 0) value = -value;
  if constexpr (std::is_same_v<strict, geo::strict>) {
    return value > 0;
  } else {
    return value >= 0;
  }
}

template <typename T>
auto circumradius(const point<T>& a, const point<T>& b, const point<T>& c) {
  using return_t = geo::circle_info<T>::intersection_type;
  auto top = sqrt(
      (a - b).template norm<return_t>() *
      (b - c).template norm<return_t>() *
      (c - a).template norm<return_t>());
  auto bot = 2 * cross<return_t>(a - c, b - c);
  return top / bot;
}

// TODO circumcenter
