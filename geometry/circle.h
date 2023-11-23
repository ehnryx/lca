/* Circles
 * STATUS
 *  somewhat tested: boj/15004,7019,9598,20989
 */
#pragma once

#include <variant>

#include "lines.h"
#include "point.h"

template <typename>
struct circle;

namespace geo {
template <typename T>
struct circle_intersection_type {
  using type = T;
  using one = point<T>;
  using two = std::tuple<point<T>, point<T>>;
  std::variant<bool, one, two> it;
  auto count() const -> int {
    return std::holds_alternative<bool>(it) ? (std::get<bool>(it) ? -1 : 0)
                                            : (std::holds_alternative<one>(it) ? 1 : 2);
  }
  auto get_one() const -> point<T> { return get<one>(it); }
  auto get_two() const -> std::tuple<point<T>, point<T>> { return get<two>(it); }
};

template <typename T, typename U>
struct bigger_intersection_type {
  using T_inter = intersection_type<T>;
  using U_inter = intersection_type<U>;
  using type = std::conditional_t<sizeof(T_inter) < sizeof(U_inter), U_inter, T_inter>;
};

template <typename T, typename U>
using bigger_intersection_t = typename bigger_intersection_type<T, U>::type;

template <typename T, typename U>
using circle_intersection_t = circle_intersection_type<bigger_intersection_t<T, U>>;
}  // namespace geo

template <typename T>
struct circle {
  using type = T;
  using bigger_type = geo::bigger_type<T>;
  template <typename U>
  using inter_t = geo::circle_intersection_t<T, U>;
  template <typename U>
  using intersection_type = typename inter_t<U>::type;

  point<T> center;
  T radius;

  circle(point<T> const& c, T const& r) : center(c), radius(r) {}
  circle(T x, T y, T r) : center(x, y), radius(r) {}
  template <typename U, std::enable_if_t<geo::is_constructible_v<T, U>, bool> = true>
  circle(circle<U> const& c) : center(c.center), radius(c.radius) {}
  template <typename U, std::enable_if_t<not geo::is_constructible_v<T, U>, bool> = true>
  explicit circle(circle<U> const& c) : center(c.center), radius(c.radius) {}

  bool operator==(const circle& o) const { return center == o.center and radius == o.radius; }

  bool contains(geo::epsilon<T> eps, point<T> const& o, geo::strict strict = false) const {
    if (strict) {
      return norm(o - center) < bigger_type(radius) * (radius - 2 * eps);
    } else {
      return norm(o - center) <= bigger_type(radius) * (radius + 2 * eps);
    }
  }
  bool contains(point<T> const& o, geo::strict strict = false) const {
    if (strict) {
      return norm(o - center) < bigger_type(radius) * radius;
    } else {
      return norm(o - center) <= bigger_type(radius) * radius;
    }
  }

  template <typename U>
  auto get_point(U angle) const {
    return point<U>(center) + point<U>::polar(radius, angle);
  }

  template <typename U>
  auto segment_area(U angle) const {
    return (angle - sin(angle)) * radius * radius / 2;
  }

  template <typename U>
  inter_t<U> intersect(circle<U> const& o) const {
    using I = geo::bigger_intersection_t<T, U>;
    if constexpr (not std::is_same_v<I, T> or not std::is_same_v<I, U>) {
      return circle<I>(*this).intersect(circle<I>(o));
    } else {
      return _intersect_impl(o);
    }
  }

  template <typename U, typename E>
  inter_t<U> intersect(circle<U> const& o, E const eps) const {
    using I = geo::bigger_intersection_t<T, U>;
    if constexpr (not std::is_same_v<I, T> or not std::is_same_v<I, U>) {
      return circle<I>(*this).intersect(circle<I>(o), eps);
    } else {
      return _intersect_impl(o, I(eps));
    }
  }

  template <typename U>
  inter_t<U> intersect(point<U> const& u, point<U> const& v) const {
    using I = geo::bigger_intersection_t<T, U>;
    if constexpr (not std::is_same_v<I, T> or not std::is_same_v<I, U>) {
      return circle<I>(*this).intersect(point<I>(u), point<I>(v));
    } else {
      return _intersect_impl(u, v);
    }
  }

 private:
  inter_t<T> _intersect_impl(circle<T> const& b) const {
    auto d2 = norm(b.center - center);
    if (d2 == 0) return {radius == b.radius};
    auto sum_r2 = (radius + b.radius) * (radius + b.radius);
    auto dif_r2 = (radius - b.radius) * (radius - b.radius);
    if (d2 > sum_r2 or d2 < dif_r2) return {false};
    auto d_r2 = (radius + b.radius) * (radius - b.radius);
    auto it = ((center + b.center) + (b.center - center) * (d_r2 / d2)) / 2;
    if (d2 == sum_r2 or d2 == dif_r2) return {it};
    auto h2d2 = radius < b.radius ? d2 * radius * radius - (d2 + d_r2) * (d2 + d_r2) / 4
                                  : d2 * b.radius * b.radius - (d2 - d_r2) * (d2 - d_r2) / 4;
    if (h2d2 <= 0) return {it};
    auto shift = std::sqrt(h2d2) / d2 * perp(b.center - center);
    return {std::tuple(it - shift, it + shift)};
  }

  inter_t<T> _intersect_impl(circle<T> const& b, T const eps) const {
    const auto eps2 = eps * eps;
    const auto d2 = norm(b.center - center);
    if (d2 < eps2) return {std::abs(radius - b.radius) < eps};
    const auto d_r2 = (radius + b.radius) * (radius - b.radius);
    const auto it = ((center + b.center) + (b.center - center) * (d_r2 / d2)) / 2;
    const auto h2d2 = radius < b.radius
                          ? d2 * radius * radius - (d2 + d_r2) * (d2 + d_r2) / 4
                          : d2 * b.radius * b.radius - (d2 - d_r2) * (d2 - d_r2) / 4;
    if (const auto d2eps2 = d2 * eps2; h2d2 < -d2eps2) return {false};
    else if (h2d2 < d2eps2) return {it};
    const auto shift = std::sqrt(h2d2) / d2 * perp(b.center - center);
    return {std::tuple(it - shift, it + shift)};
  }

  inter_t<T> _intersect_impl(point<T> const& u, point<T> const& v) const {
    if (std::abs(line_point_dist(u, v, center)) > radius) return {false};
    auto mid = project(u, v, center);
    auto h2 = radius * radius - norm(mid - center);
    if (h2 <= 0) return {mid};
    auto shift = std::sqrt(h2 / norm(v - u)) * (v - u);
    return {std::tuple(mid - shift, mid + shift)};
  }
};

namespace geo {
template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool equal(T eps, circle<T> const& a, circle<T> const& b) {
  return equal(eps, a.center, b.center) && std::abs(a.radius - b.radius) <= eps;
}
}  // namespace geo
