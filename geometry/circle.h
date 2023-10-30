/* Circles
 * STATUS
 *  mostly untested
 */
#pragma once

#include "../utility/is_any_of.h"
#include "lines.h"
#include "point.h"
#include <variant>

template <typename> struct circle;

namespace geo {
template <typename T, typename U>
struct circle_constructible {
  static constexpr bool value = std::is_same_v<T, U> or
    std::is_same_v<T, typename circle_info<U>::intersection_type>;
};

template <typename T>
struct intersection_t {
  using type = T;
  using one = point<T>;
  using two = std::tuple<point<T>, point<T>>;
  std::variant<bool, one, two> it;
  auto count() const -> int {
    return std::holds_alternative<bool>(it)
      ? (std::get<bool>(it) ? -1 : 0)
      : (std::holds_alternative<one>(it) ? 1 : 2);
  }
  auto get_one() const -> point<T> { return get<one>(it); }
  auto get_two() const -> tuple<point<T>, point<T>> { return get<two>(it); }
};

template <typename T, typename U>
struct intersection_info {
  using T_inter = circle_info<T>::intersection_type;
  using U_inter = circle_info<U>::intersection_type;
  using inter_t = std::conditional_t<
    std::is_floating_point_v<T_inter> and std::is_floating_point_v<U_inter>,
    std::conditional_t<sizeof(T_inter) < sizeof(U_inter), U_inter, T_inter>, void>;
  using type = intersection_t<inter_t>;
};

template <typename T, typename U>
intersection_info<T, U>::type intersect(const circle<T>&, const circle<U>&);
template <typename T, typename U, typename V>
intersection_info<T, U>::type intersect(const circle<T>&, const circle<U>&, V eps);
template <typename T, typename U>
intersection_info<T, U>::type intersect(const circle<T>&, const point<U>&, const point<U>&);
}

template <typename T>
struct circle {
  using bigger_t = point<T>::bigger_t;
  point<T> center;
  T radius;

  circle(const point<T>& c, const T& r): center(c), radius(r) {}
  circle(T x, T y, T r): center(x, y), radius(r) {}
  template <typename U, std::enable_if_t<geo::circle_constructible<T, U>::value, bool> = true>
  circle(const circle<U>& c): center(c.center), radius(c.radius) {}
  template <typename U, std::enable_if_t<not geo::circle_constructible<T, U>::value, bool> = true>
  explicit circle(const circle<U>& c): center(c.center), radius(c.radius) {}

  bool operator==(const circle& o) const { return center == o.center and radius == o.radius; }

  template <typename strict = void>
  bool contains(const point<T>& o) const {
    static_assert(is_any_of_v<strict, void, geo::strict>);
    if constexpr (std::is_same_v<strict, geo::strict>) {
      return norm(o - center) < bigger_t(radius) * radius;
    } else {
      return norm(o - center) <= bigger_t(radius) * radius;
    }
  }

  template <typename U>
  auto intersect(const circle<U>& o) const {
    return geo::intersect(*this, o);
  }
  template <typename U, typename V>
  auto intersect(const circle<U>& o, V eps) const {
    return geo::intersect(*this, o, eps);
  }
  template <typename U>
  auto intersect(const point<U>& u, const point<U>& v) const {
    return geo::intersect(*this, u, v);
  }

  template <typename U>
  auto get_point(U angle) const {
    return point<U>(center) + point<U>::polar(radius, angle);
  }

  template <typename U>
  auto segment_area(U angle) const {
    return (angle - sin(angle)) * radius * radius / 2;
  }
};

namespace geo {
template <typename T>
intersection_t<T> intersect_impl(const circle<T>& a, const circle<T>& b) {
  auto d2 = norm(b.center - a.center);
  if (d2 == 0) return {a.radius == b.radius};
  auto sum_r2 = (a.radius + b.radius) * (a.radius + b.radius);
  auto dif_r2 = (a.radius - b.radius) * (a.radius - b.radius);
  if (d2 > sum_r2 or d2 < dif_r2) return {false};
  auto d_r2 = (a.radius + b.radius) * (a.radius - b.radius);
  auto it = ((a.center + b.center) + (b.center - a.center) * (d_r2 / d2)) / 2;
  if (d2 == sum_r2 or d2 == dif_r2) return {it};
  auto h2d2 = a.radius < b.radius
    ? d2 * a.radius * a.radius - (d2 + d_r2) * (d2 + d_r2) / 4
    : d2 * b.radius * b.radius - (d2 - d_r2) * (d2 - d_r2) / 4;
  if (h2d2 <= 0) return {it};
  auto shift = sqrt(h2d2) / d2 * perp(b.center - a.center);
  return {tuple(it - shift, it + shift)};
}

template <typename T>
intersection_t<T> intersect_impl(const circle<T>& a, const circle<T>& b, const T eps) {
  const auto eps2 = eps * eps;
  const auto d2 = norm(b.center - a.center);
  if (d2 < eps2) return {abs(a.radius - b.radius) < eps};
  const auto d_r2 = (a.radius + b.radius) * (a.radius - b.radius);
  const auto it = ((a.center + b.center) + (b.center - a.center) * (d_r2 / d2)) / 2;
  const auto h2d2 = a.radius < b.radius
    ? d2 * a.radius * a.radius - (d2 + d_r2) * (d2 + d_r2) / 4
    : d2 * b.radius * b.radius - (d2 - d_r2) * (d2 - d_r2) / 4;
  if (const auto d2eps2 = d2 * eps2; h2d2 < -d2eps2) return {false};
  else if (h2d2 < d2eps2) return {it};
  const auto shift = sqrt(h2d2) / d2 * perp(b.center - a.center);
  return {tuple(it - shift, it + shift)};
}

template <typename T, typename U>
intersection_info<T, U>::type intersect(const circle<T>& a, const circle<U>& b) {
  using inter_t = intersection_info<T, U>::inter_t;
  return intersect_impl(circle<inter_t>(a), circle<inter_t>(b));
}

template <typename T, typename U, typename V>
intersection_info<T, U>::type intersect(const circle<T>& a, const circle<U>& b, V eps) {
  using inter_t = intersection_info<T, U>::inter_t;
  return intersect_impl(circle<inter_t>(a), circle<inter_t>(b), inter_t(eps));
}

template <typename T>
intersection_t<T> intersect_impl(const circle<T>& a, const point<T>& u, const point<T>& v) {
  if (abs(line_point_dist(u, v, a.center)) > a.radius) return {false};
  auto mid = project(u, v, a.center);
  auto h2 = a.radius * a.radius - norm(mid - a.center);
  if (h2 <= 0) return {mid};
  auto shift = sqrt(h2 / norm(v - u)) * (v - u);
  return {tuple(mid - shift, mid + shift)};
}

template <typename T, typename U>
intersection_info<T, U>::type intersect(const circle<T>& a, const point<U>& u, const point<U>& v) {
  using inter_t = intersection_info<T, U>::inter_t;
  return intersect_impl(circle<inter_t>(a), point<inter_t>(u), point<inter_t>(v));
}
}
