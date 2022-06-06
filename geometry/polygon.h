/* Lines
 * STATUS
 *  mostly untested
 */
#pragma once

#include "point.h"
#include "lines.h"
#include <vector>

template <typename out_t, typename T>
std::vector<point<out_t>> polygon(const std::vector<point<T>>& v) {
  std::vector<point<out_t>> res;
  res.reserve(v.size());
  for (const point<T>& it : v) {
    res.emplace_back(it);
  }
  return res;
}

template <typename T>
auto signed_area(const std::vector<point<T>>& v) {
  T area = 0;
  for (int i = (int)size(v) - 1, j = 0; j < (int)size(v); i = j++) {
    area += cross(v[i], v[j]);
  }
  if constexpr (std::is_integral_v<T>) {
    return area / 2.L;
  } else {
    return area / 2;
  }
}

template <typename T>
T doubled_area(const std::vector<point<T>>& v) {
  T area = 0;
  for (int i = (int)size(v) - 1, j = 0; j < (int)size(v); i = j++) {
    area += cross(v[i], v[j]);
  }
  return area;
}

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool on_boundary(const T& eps,
    const std::vector<point<T>>& v, const point<T>& c) {
  bool ok = false;
  for (int i = (int)size(v) - 1, j = 0; !ok && j < (int)size(v); i = j++) {
    ok |= on_segment(eps, v[i], v[j], c);
  }
  return ok;
}
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
bool on_boundary(const std::vector<point<T>>& v, const point<T>& c) {
  bool ok = false;
  for (int i = (int)size(v) - 1, j = 0; !ok && j < (int)size(v); i = j++) {
    ok |= on_segment(v[i], v[j], c);
  }
  return ok;
}

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool in_polygon(const T& eps,
    const std::vector<point<T>>& v, const point<T>& c, bool strict = false) {
  if (on_boundary(eps, v, c)) return !strict;
  double sum = 0;
  for (int i = (int)size(v) - 1, j = 0; j < (int)size(v); i = j++) {
    sum += atan2(cross(v[i] - c, v[j] - c), dot(v[i] - c, v[j] - c));
  }
  return abs(sum) > 1;
}
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
bool in_polygon(const std::vector<point<T>>& v, const point<T>& c, bool strict = false) {
  if (on_boundary(v, c)) return !strict;
  double sum = 0;
  for (int i = (int)size(v) - 1, j = 0; j < (int)size(v); i = j++) {
    sum += atan2(cross(v[i] - c, v[j] - c), dot(v[i] - c, v[j] - c));
  }
  return abs(sum) > 1;
}

// keep the left side
template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
std::vector<point<T>> cut_polygon(const T& eps,
    const std::vector<point<T>> &v, const point<T>& a, const point<T>& b) {
  std::vector<point<T>> res;
  res.reserve(v.size() + 1);
  for (int i = (int)v.size() - 1, j = 0; j < (int)v.size(); i = j++) {
    T normalized_eps = eps * abs(b - a);
    if (cross(b - a, v[i] - a) >= -normalized_eps) {
      res.push_back(v[i]);
    }
    if (utility::sign(cross(b - a, v[i] - a), normalized_eps) *
        utility::sign(cross(b - a, v[j] - a), normalized_eps) < 0) {
      point<T> it = line_inter(a, b, v[i], v[j]);
      if (res.empty() || !equal(eps, res.back(), it)) {
        res.push_back(it);
      }
    }
  }
  while (!res.empty() && equal(eps, res.front(), res.back())) {
    res.pop_back();
  }
  return res;
}
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
std::vector<point<T>> cut_polygon(
    const std::vector<point<T>> &v, const point<T>& a, const point<T>& b) {
  static_assert(!std::is_integral_v<T>); // doesn't make sense
  std::vector<point<T>> res;
  res.reserve(v.size() + 1);
  for (int i = (int)v.size() - 1, j = 0; j < (int)v.size(); i = j++) {
    if (cross(b - a, v[i] - a) >= 0) {
      res.push_back(v[i]);
    }
    if (utility::sign(cross(b - a, v[i] - a), T(0)) *
        utility::sign(cross(b - a, v[j] - a), T(0)) < 0) {
      point<T> it = line_inter(a, b, v[i], v[j]);
      if (res.empty() || !equal(res.back(), it)) {
        res.push_back(it);
      }
    }
  }
  while (!res.empty() && equal(res.front(), res.back())) {
    res.pop_back();
  }
  return res;
}

