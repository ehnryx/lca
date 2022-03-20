/* Convex Hull (Monotone Chain)
 * USAGE
 *  vector<pt> hull = convex_hull(points, keep_collinear);
 * INPUT
 *  points: vector of point<T>
 *  keep_collinear: keeps collinear points on the boundary
 * OUTPUT
 *  the convex hull in counterclockwise order
 * TIME
 *  O(NlogN)
 *  N = #points
 * STATUS
 *  tested: kattis/convexhull,convexhull2
 */
#pragma once

#include "point.h"

template <typename T>
vector<point<T>> convex_hull(vector<point<T>> points, bool keep=false) {
  sort(begin(points), end(points));
  points.resize(unique(begin(points), end(points)) - begin(points));
  vector<point<T>> hull(2 * size(points));
  int top = 0, bot = 0;
  for (int i = 0, dir = 1; i < (int)size(points) && i >= 0; i += dir) {
    while (top - bot > 1) {
      T turn = cross(hull[top-1] - hull[top-2], points[i] - hull[top-2]);
      if (keep ? turn >= 0 : turn > 0) break;
      top--; // pop
    }
    hull[top++] = points[i];
    if (i + 1 == (int)size(points)) {
      dir = -1;
      bot = top - 1;
    }
  }
  hull.resize(max(1, top - 1));
  return hull;
}

