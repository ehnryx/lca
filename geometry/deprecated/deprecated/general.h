/* Lines
 * STATUS
 *  mostly untested
 */
#pragma once

#include "point.h"
#include <vector>

template <typename T>
bool equal(const point<T>& a, const point<T>& b, const T& eps = 1e-9) {
  if constexpr (std::is_integral_v<T>) {
    return a == b;
  } else {
    return abs(a - b) <= eps;
  }
}

template <typename T>
bool collinear(
    const point<T>& a, const point<T>& b, const point<T>& c,
    const T& eps = 1e-9) {
  return equal(a, c, eps) || abs(cross(a - c, b - c)) <= eps * abs(a - c);
}

template <typename T>
bool parallel(
    const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d,
    const T& eps = 1e-9) {
  return equal(a, b, eps) || abs(cross(a - b, c - d)) <= eps * abs(a - b);
}

template <typename T>
std::enable_if_t<!std::is_integral_v<T>, point<T>> line_inter(
    const point<T>& a, const point<T>& b,
    const point<T>& c, const point<T>& d) {
  return a + cross(c - a, d - c) / cross(b - a, d - c) * (b - a);
}

template <typename T>
auto line_point_dist(
  const point<T>& a, const point<T>& b, const point<T>& c) {
  return cross(b - a, c - a) / abs(b - a);
}

template <typename T>
auto segment_point_dist(
  const point<T>& a, const point<T>& b, const point<T>& c) {
  if (dot(b - a, c - a) > 0 && dot(a - b, c - b) > 0) {
    return abs(line_point_dist(a, b, c));
  } else {
    return min(abs(a - c), abs(b - c));
  }
}

template <typename T>
auto segment_closest(
  const point<T>& a, const point<T>& b, const point<T>& c,
  const T& eps = 1e-9) {
  if (dot(b - a, c - a) > 0 && dot(a - b, c - b) > 0) {
    return abs(cross(b - a, c - a)) < eps ? c : line_inter(a, b, c, c + perp(a - b));
  } else {
    return norm(a - c) < norm(b - c) ? a : b;
  }
}

template <typename T>
auto signed_area(const std::vector<point<T>>& v) {
  int n = (int)v.size();
  T area = 0;
  for (int i = n - 1, j = 0; j < n; i = j++) {
    area += cross(v[i], v[j]);
  }
  if constexpr (std::is_integral_v<T>) {
    return area / 2.L;
  } else {
    return area / 2;
  }
}

template <typename T>
bool on_segment(
    const point<T>& a, const point<T>& b, const point<T>& c,
    bool strict = false, const T& eps = 1e-9) {
  if (equal(a, c, eps) || equal(b, c, eps)) return !strict;
  return collinear(a, b, c, eps) && dot(a - b, c - b) > 0 && dot(b - a, c - a) > 0;
}

template <typename T>
bool on_boundary(const std::vector<point<T>>& v, const point<T>& c, const T& eps = 1e-9) {
  bool ok = false;
  for (int i = (int)size(v) - 1, j = 0; !ok && j < (int)size(v); i = j++) {
    ok |= on_segment(v[i], v[j], c, false, eps);
  }
  return ok;
}

template <typename T>
bool in_polygon(
    const std::vector<point<T>>& v, const point<T>& c,
    bool strict = false, const T& eps = 1e-9) {
  if (on_boundary(v, c, eps)) return !strict;
  double sum = 0;
  for (int i = (int)size(v) - 1, j = 0; j < (int)size(v); i = j++) {
    sum += atan2(cross(v[i] - c, v[j] - c), dot(v[i] - c, v[j] - c));
  }
  return abs(sum) > 1;
}

namespace {
  template <typename T, typename U>
  int sign(const T& x, const U& eps) {
    return x < -eps ? -1 : x > eps ? 1 : 0;
  }
}

template <typename T>
bool seg_x_seg(
    const point<T>& a, const point<T>& b,
    const point<T>& c, const point<T>& d,
    bool strict = false, const T& eps = 1e-9) {
  if constexpr (std::is_integral_v<T>) {
    if (equal(a, b, eps)) return on_segment(c, d, a, strict, eps);
    if (equal(c, d, eps)) return on_segment(a, b, d, strict, eps);
    T r1 = cross(a - b, a - c);
    T r2 = cross(a - b, a - d);
    if (r1 == 0 && r2 == 0) {
      return strict ? min(a, b) < max(c, d) && min(c, d) < max(a, b) :
        !(max(a, b) < min(c, d) || max(c, d) < min(a, b));
    }
    T r3 = cross(c - d, c - a);
    T r4 = cross(c - d, c - b);
    r2 = r2 < 0 ? -1 : r2 > 0 ? 1 : 0;
    r4 = r4 < 0 ? -1 : r4 > 0 ? 1 : 0;
    return strict ? r1*r2 < 0 && r3*r4 < 0 : r1*r2 <= 0 && r3*r4 <= 0;
  } else {
    auto ab = abs(a - b);
    if (ab <= eps) return on_segment(c, d, a, strict, eps);
    auto cd = abs(c - d);
    if (cd <= eps) return on_segment(a, b, d, strict, eps);
    int r1 = sign(cross(a - b, a - c), eps * ab);
    int r2 = sign(cross(a - b, a - d), eps * ab);
    if (r1 == 0 && r2 == 0) {
      return strict ? min(a, b) < max(c, d) && min(c, d) < max(a, b) :
        !(max(a, b) < min(c, d) || max(c, d) < min(a, b));
    }
    int r3 = sign(cross(c - d, c - a), eps * cd);
    int r4 = sign(cross(c - d, c - b), eps * cd);
    return strict ? r1*r2 < 0 && r3*r4 < 0 : r1*r2 <= 0 && r3*r4 <= 0;
  }
}

