/* Lines
 * STATUS
 *  mostly untested
 */
#pragma once

#include "point.h"

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool collinear(const T& eps,
    const point<T>& a, const point<T>& b, const point<T>& c) {
  return equal(eps, a, c) || abs(cross(a - c, b - c)) <= eps * abs(a - c);
}
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
bool collinear(const point<T>& a, const point<T>& b, const point<T>& c) {
  return cross(a - c, b - c) == 0;
}

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool parallel(const T& eps,
    const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d) {
  return equal(eps, a, b) || abs(cross(a - b, c - d)) <= eps * abs(a - b);
}
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
bool parallel(const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d) {
  return cross(a - b, c - d) == 0;
}

template <typename T>
auto line_inter(const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d) {
  if constexpr (!std::is_integral_v<T>) {
    return a + cross(c - a, d - c) / cross(b - a, d - c) * (b - a);
  } else {
    return point<double>(a) + point<double>(b - a) * cross(c - a, d - c) / cross(b - a, d - c);
  }
}

template <typename T>
auto line_point_dist(const point<T>& a, const point<T>& b, const point<T>& c) {
  return cross(b - a, c - a) / abs(b - a);
}

template <typename T>
auto segment_point_dist(const point<T>& a, const point<T>& b, const point<T>& c) {
  if (dot(b - a, c - a) > 0 && dot(a - b, c - b) > 0) {
    return abs(line_point_dist(a, b, c));
  } else {
    return std::min(abs(a - c), abs(b - c));
  }
}

template <typename T>
auto segment_closest(const point<T>& a, const point<T>& b, const point<T>& c) {
  if (dot(b - a, c - a) > 0 && dot(a - b, c - b) > 0) {
    return line_inter(a, b, c, c + perp(a - b));
  } else {
    return norm(a - c) < norm(b - c) ? a : b;
  }
}

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool on_segment(const T& eps,
    const point<T>& a, const point<T>& b, const point<T>& c, bool strict = false) {
  if (equal(eps, a, c) || equal(eps, b, c)) return !strict;
  return collinear(eps, a, b, c) && dot(b - a, c - a) > 0 && dot(a - b, c - b) > 0;
}
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
bool on_segment(const point<T>& a, const point<T>& b, const point<T>& c, bool strict = false) {
  if (a == c || b == c) return !strict;
  return collinear(a, b, c) && dot(b - a, c - a) > 0 && dot(a - b, c - b) > 0;
}

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool seg_x_seg(const T& eps,
    const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d,
    bool strict = false) {
  auto ab = abs(a - b);
  if (ab <= eps) return !strict && on_segment(eps, c, d, a);
  auto cd = abs(c - d);
  if (cd <= eps) return !strict && on_segment(eps, a, b, d);
  int r1 = utility::sign(cross(b - a, c - a), eps * ab);
  int r2 = utility::sign(cross(b - a, d - a), eps * ab);
  if (r1 == 0 && r2 == 0) {
    return strict
      ? less_than(eps, min(a, b), max(c, d)) && less_than(eps, min(c, d), max(a, b))
      : !(less_than(eps, max(a, b), min(c, d)) || less_than(eps, max(c, d), min(a, b)));
  }
  int r3 = utility::sign(cross(d - c, a - c), eps * cd);
  int r4 = utility::sign(cross(d - c, b - c), eps * cd);
  return strict ? r1 * r2 < 0 && r3 * r4 < 0 : r1 * r2 <= 0 && r3 * r4 <= 0;
}
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
bool seg_x_seg(const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d,
    bool strict = false) {
  if (a == b) return !strict && on_segment(c, d, a);
  if (c == d) return !strict && on_segment(a, b, d);
  int r1 = utility::sign(cross(b - a, c - a), T(0));
  int r2 = utility::sign(cross(b - a, d - a), T(0));
  if (r1 == 0 && r2 == 0) {
    return strict
      ? less_than(min(a, b), max(c, d)) && less_than(min(c, d), max(a, b))
      : !(less_than(max(a, b), min(c, d)) || less_than(max(c, d), min(a, b)));
  }
  int r3 = utility::sign(cross(d - c, a - c), T(0));
  int r4 = utility::sign(cross(d - c, b - c), T(0));
  return strict ? r1 * r2 < 0 && r3 * r4 < 0 : r1 * r2 <= 0 && r3 * r4 <= 0;
}

