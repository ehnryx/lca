/* Lines
 * STATUS
 *  mostly untested
 */
#pragma once

#include "point.h"

#ifndef USE_RATIONAL_POINTS
template <typename T>
bool collinear(
    const point<T>& a, const point<T>& b, const point<T>& c,
    const T& eps = 1e-9) {
  return abs(a-c) <= eps || abs(cross(a-c, b-c) / abs(a-c)) <= eps;
}

template <typename T>
bool parallel(
    const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d,
    const T& eps = 1e-9) {
  return abs(cross(a-b, c-d)) / abs(a-b) <= eps;
}

#else
template <typename T>
bool collinear(const point<T>& a, const point<T>& b, const point<T>& c) {
  return a == c || sign_cross(a-c, b-c) == 0;
}

template <typename T>
bool parallel(const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d) {
  return sign_cross(a-b, c-d) == 0;
}
#endif

template <typename T>
point<T> line_inter(
    const point<T>& a, const point<T>& b,
    const point<T>& c, const point<T>& d) {
  return a + cross(c-a, d-c) / cross(b-a, d-c) * (b-a);
}

#ifndef USE_RATIONAL_POINTS
template <typename T>
bool on_segment(
    const point<T>& a, const point<T>& b, const point<T>& c,
    bool strict = false, const T& eps = 1e-9) {
  if (equal(a, c, eps) || equal(b, c, eps)) return !strict;
  return collinear(a, b, c, eps) && dot(a - b, c - b) > 0 && dot(b - a, c - a) > 0;
}

template <typename T>
bool on_boundary(const vector<point<T>>& v, const point<T>& c, const T& eps = 1e-9) {
  bool ok = false;
  for (int i = (int)size(v) - 1, j = 0; !ok && j < (int)size(v); i = j++) {
    ok |= on_segment(v[i], v[j], c, false, eps);
  }
  return ok;
}

template <typename T>
bool in_polygon(
    const vector<point<T>>& v, const point<T>& c,
    bool strict = false, const T& eps = 1e-9) {
  if (on_boundary(v, c, eps)) return !strict;
  T sum = 0;
  for (int i = (int)size(v) - 1, j = 0; j < (int)size(v); i = j++) {
    sum += atan2(cross(v[i] - c, v[j] - c), dot(v[i] - c, v[j] - c));
  }
  return abs(sum) > 1;
}

template <typename T>
bool seg_x_seg(
    const point<T>& a, const point<T>& b,
    const point<T>& c, const point<T>& d,
    bool strict = false, const T& eps = 1e-9) {
  T ab = abs(a - b);
  if (ab <= eps) return on_segment(c, d, a, strict, eps);
  T cd = abs(c - d);
  if (cd <= eps) return on_segment(a, b, d, strict, eps);
  int r1 = sign(cross(a - b, a - c) / ab, eps);
  int r2 = sign(cross(a - b, a - d) / ab, eps);
  if (r1 == 0 && r2 == 0) {
    return strict ? min(a, b) < max(c, d) && min(c, d) < max(a, b) :
        !(max(a, b) < min(c, d) || max(c, d) < min(a, b));
  }
  int r3 = sign(cross(c - d, c - a) / cd, eps);
  int r4 = sign(cross(c - d, c - b) / cd, eps);
  return strict ? r1*r2 < 0 && r3*r4 < 0 : r1*r2 <= 0 && r3*r4 <= 0;
}

#else
template <typename T>
bool on_segment(
    const point<T>& a, const point<T>& b, const point<T>& c,
    bool strict = false) {
  if (a == c || b == c) return !strict;
  return collinear(a, b, c) && sign_dot(a - b, c - b) > 0 && sign_dot(b - a, c - a) > 0;
}

template <typename T>
bool on_boundary(const vector<point<T>>& v, const point<T>& c, const T& eps = 1e-9) {
  bool ok = false;
  for (int i = (int)size(v) - 1, j = 0; !ok && j < (int)size(v); i = j++) {
    ok |= on_segment(v[i], v[j], c, false);
  }
  return ok;
}

template <typename D = long double, typename T>
bool in_polygon(const vector<point<T>>& v, const point<T>& c, bool strict = false) {
  if (on_boundary(v, c)) return !strict;
  D sum = 0;
  for (int i = (int)size(v) - 1, j = 0; j < (int)size(v); i = j++) {
    sum += atan2(D(cross(v[i] - c, v[j] - c)), D(dot(v[i] - c, v[j] - c)));
  }
  return abs(sum) > 1;
}

template <typename T>
bool seg_x_seg(
    const point<T>& a, const point<T>& b,
    const point<T>& c, const point<T>& d,
    bool strict = false) {
  if (a == b) return on_segment(c, d, a, strict);
  if (c == d) return on_segment(a, b, d, strict);
  int r1 = sign_cross(a - b, a - c);
  int r2 = sign_cross(a - b, a - d);
  if (r1 == 0 && r2 == 0) {
    return strict ? min(a, b) < max(c, d) && min(c, d) < max(a, b) :
        !(max(a, b) < min(c, d) || max(c, d) < min(a, b));
  }
  int r3 = sign_cross(c - d, c - a);
  int r4 = sign_cross(c - d, c - b);
  return strict ? r1*r2 < 0 && r3*r4 < 0 : r1*r2 <= 0 && r3*r4 <= 0;
}
#endif

