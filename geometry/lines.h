/* Lines
 * STATUS
 *  mostly untested
 */
#pragma once

#include "point.h"

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool collinear(T const& eps,
    point<T> const& a, point<T> const& b, point<T> const& v) {
  return geo::equal(eps, a, v) or abs(cross(a - v, b - v)) <= eps * abs(a - v);
}
template <typename T, std::enable_if_t<not point<T>::floating, bool> = true>
bool collinear(point<T> const& a, point<T> const& b, point<T> const& v) {
  return cross(a - v, b - v) == 0;
}

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool parallel(T const& eps,
    point<T> const& a, point<T> const& b, point<T> const& c, point<T> const& d) {
  return geo::equal(eps, a, b) || abs(cross(a - b, c - d)) <= eps * abs(a - b);
}
template <typename T, std::enable_if_t<not point<T>::floating, bool> = true>
bool parallel(point<T> const& a, point<T> const& b, point<T> const& c, point<T> const& d) {
  return cross(a - b, c - d) == 0;
}

template <typename T>
auto project(point<T> const& a, point<T> const& b, point<T> const& v) {
  using I = typename point<T>::intersection_type;
  if constexpr (std::is_same_v<T, I>) {
    return a + dot(v - a, b - a) / norm(b - a) * (b - a);
  } else {
    return point<I>(a) + point<I>(b - a) * dot(v - a, b - a) / norm(b - a);
  }
}

template <typename T>
auto line_inter(point<T> const& a, point<T> const& b, point<T> const& c, point<T> const& d) {
  using I = typename point<T>::intersection_type;
  if constexpr (std::is_same_v<T, I>) {
    return a + cross(c - a, d - c) / cross(b - a, d - c) * (b - a);
  } else {
    return point<I>(a) + point<I>(b - a) * cross(c - a, d - c) / cross(b - a, d - c);
  }
}

template <typename T>
auto line_point_dist(point<T> const& a, point<T> const& b, point<T> const& v) {
  return cross(b - a, v - a) / abs(b - a);
}

template <typename T>
auto segment_point_dist(point<T> const& a, point<T> const& b, point<T> const& v) {
  if (dot(b - a, v - a) > 0 && dot(a - b, v - b) > 0) {
    return abs(line_point_dist(a, b, v));
  } else {
    return std::min(abs(a - v), abs(b - v));
  }
}

template <typename T>
auto segment_closest(point<T> const& a, point<T> const& b, point<T> const& v) {
  if (dot(b - a, v - a) > 0 && dot(a - b, v - b) > 0) {
    return line_inter(a, b, v, v + perp(a - b));
  } else {
    return point<typename point<T>::inter_t>(norm(a - v) < norm(b - v) ? a : b);
  }
}

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool on_segment(T const& eps,
    point<T> const& a, point<T> const& b, point<T> const& v, geo::strict strict = false) {
  if (geo::equal(eps, a, v) || geo::equal(eps, b, v)) return not strict;
  return collinear(eps, a, b, v) && dot(b - a, v - a) > 0 && dot(a - b, v - b) > 0;
}
template <typename T, std::enable_if_t<not point<T>::floating, bool> = true>
bool on_segment(point<T> const& a, point<T> const& b, point<T> const& v, geo::strict strict = false) {
  if (a == v || b == v) return not strict;
  return collinear(a, b, v) && dot(b - a, v - a) > 0 && dot(a - b, v - b) > 0;
}

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool seg_x_seg(T const& eps,
    point<T> const& a, point<T> const& b, point<T> const& c, point<T> const& d,
    geo::strict strict = false) {
  auto ab = abs(a - b);
  if (ab <= eps) return not strict && on_segment(eps, c, d, a);
  auto cd = abs(c - d);
  if (cd <= eps) return not strict && on_segment(eps, a, b, d);
  int r1 = geo::sign(cross(b - a, c - a), geo::epsilon{eps * ab});
  int r2 = geo::sign(cross(b - a, d - a), geo::epsilon{eps * ab});
  if (r1 == 0 && r2 == 0) {
    return strict
      ? geo::less_than(eps, std::min(a, b), std::max(c, d)) && geo::less_than(eps, std::min(c, d), std::max(a, b))
      : not (geo::less_than(eps, std::max(a, b), std::min(c, d)) || geo::less_than(eps, std::max(c, d), std::min(a, b)));
  }
  int r3 = geo::sign(cross(d - c, a - c), geo::epsilon{eps * cd});
  int r4 = geo::sign(cross(d - c, b - c), geo::epsilon{eps * cd});
  return strict ? r1 * r2 < 0 && r3 * r4 < 0 : r1 * r2 <= 0 && r3 * r4 <= 0;
}
template <typename T, std::enable_if_t<not point<T>::floating, bool> = true>
bool seg_x_seg(point<T> const& a, point<T> const& b, point<T> const& c, point<T> const& d,
    geo::strict strict = false) {
  if (a == b) return not strict && on_segment(c, d, a, geo::strict{false});
  if (c == d) return not strict && on_segment(a, b, d, geo::strict{false});
  int r1 = geo::sign(cross(b - a, c - a));
  int r2 = geo::sign(cross(b - a, d - a));
  if (r1 == 0 && r2 == 0) {
    return strict
      ? std::min(a, b) < std::max(c, d) && std::min(c, d) < std::max(a, b)
      : not (std::max(a, b) < std::min(c, d) || std::max(c, d) < std::min(a, b));
  }
  int r3 = geo::sign(cross(d - c, a - c));
  int r4 = geo::sign(cross(d - c, b - c));
  return strict ? r1 * r2 < 0 && r3 * r4 < 0 : r1 * r2 <= 0 && r3 * r4 <= 0;
}
