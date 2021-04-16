/* Lines
 * STATUS
 *  mostly untested
 */
#pragma once

#include "point.h"

template <typename T>
bool collinear(const point<T>& a, const point<T>& b, const point<T>& c, const T& eps) {
  return abs(a-c) <= eps || abs(cross(a-c, b-c) / abs(a-c)) <= eps;
}

template <typename T>
point<T> line_inter(const point<T>& a, const point<T>& b, const point<T>& c, const point<T>& d) {
  return a + cross(c-a, d-c) / cross(b-a, d-c) * (b-a);
}

