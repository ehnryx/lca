/* Circumcenter
 * USAGE
 *  pt O = circumcenter(A, B, C);
 * INPUT
 *  A, B, C: vertices of a triangle, orientation does not matter
 * OUTPUT
 *  circumcenter
 * TIME
 *  O(1)
 * STATUS
 *  tested: cf/101561i
 */
#pragma once

#include "point.h"

template <typename T>
point<T> circumcenter(const point<T>& a, const point<T>& b, const point<T>& c) {
  return a + perp((b-a) * norm(c-a) - (c-a) * norm(b-a)) / cross(b-a, c-a) / 2;
}

template <typename T>
T circumradius(const point<T>& a, const point<T>& b, const point<T>& c) {
  return sqrt(norm(a-b) * norm(b-c) * norm(c-a)) / abs(cross(b-a, c-a)) / 2;
}

