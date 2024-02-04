/* Polygon
 * STATUS
 *  untested
 */
#pragma once

#include <vector>
#include "lines.h"
#include "point.h"

template <typename T>
struct polygon {
  std::vector<point<T>> p;
  polygon(std::vector<point<T>>&& _p) : p(std::move(_p)) {}
  polygon(std::vector<point<T>> const& _p) : p(_p) {}
  T& operator[](int i) { return p[i]; }
  T const& operator[](int i) const { return p[i]; }
  int size() const { return p.size(); }

  template <typename float_t = double>
  auto signed_area() const
    requires(std::is_integral_v<T>)
  {
    return doubled_area() / (float_t)2;
  }
  auto signed_area() const
    requires(not std::is_integral_v<T>)
  {
    return doubled_area() / 2;
  }
  T doubled_area() const {
    T area = 0;
    for (int i = size() - 1, j = 0; j < size(); i = j++) {
      area += cross(p[i], p[j]);
    }
    return area;
  }

  bool on_boundary(const T& eps, const point<T>& c) const
    requires(point<T>::floating)
  {
    bool ok = false;
    for (int i = size() - 1, j = 0; not ok && j < size(); i = j++) {
      ok |= on_segment(eps, p[i], p[j], c);
    }
    return ok;
  }
  bool on_boundary(const point<T>& c) const
    requires(not point<T>::floating)
  {
    bool ok = false;
    for (int i = size() - 1, j = 0; !ok && j < size(); i = j++) {
      ok |= on_segment(p[i], p[j], c);
    }
    return ok;
  }

  bool contains(const T& eps, const point<T>& c, geo::strict strict = false) const
    requires(point<T>::floating)
  {
    if (on_boundary(eps, c)) return not strict;
    double sum = 0;
    for (int i = size() - 1, j = 0; j < size(); i = j++) {
      sum += atan2(cross(p[i] - c, p[j] - c), dot(p[i] - c, p[j] - c));
    }
    return std::abs(sum) > 1;
  }
  bool contains(const point<T>& c, bool strict = false) const
    requires(not point<T>::floating)
  {
    if (on_boundary(c)) return not strict;
    double sum = 0;
    for (int i = size() - 1, j = 0; j < size(); i = j++) {
      sum += atan2(cross(p[i] - c, p[j] - c), dot(p[i] - c, p[j] - c));
    }
    return std::abs(sum) > 1;
  }
};
