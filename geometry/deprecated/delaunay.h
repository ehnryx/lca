/* Delaunay Triangulation (Fortune's Algorithm)
 * USAGE
 *  auto triangles = delaunay(points);
 * INPUT
 *  points: vector of point<T>, the points to triangulate
 *    should not contain any duplicate points
 *    should not be all collinear
 * OUTPUT
 *  triangles: vector of tuple(A,B,C) representing a triangle ABC. tuple of indices
 * TIME
 *  O(NlogN)
 *  N = #points
 * STATUS
 *  TODO: kattis/pandapreserve, acmsguru/383, cf/442e, 102058k, timus/1369
 */
#pragma once

#include "lines.h"
#include "point.h"
#include "triangle.h"

template <typename T>
std::vector<std::tuple<int, int, int>> delaunay(
    const std::vector<point<T>>& points, T eps = 1e-9) {
  return {};
}

