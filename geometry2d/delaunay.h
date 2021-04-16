/* Delaunay Triangulation (Fortune's Algorithm)
 * USAGE
 *  auto triangles = delaunay(points);
 * INPUT
 *  points: vector of point<T>, the points to triangulate
 *    should not contain any duplicate points
 *    should not be all collinear
 * OUTPUT
 *  triangles: vector of tuple(A,B,C) representing a triangle ABC
 * TIME
 *  O(NlogN)
 *  N = #points
 * STATUS
 *  tested: kattis/pandapreserve, acmsguru/383 (up to |coordinate| < 1e4)
 *  fails with probability proportional to max |coordinate|
 */
#pragma once

#include "point.h"
#include "lines.h"
#include "circumcenter.h"

namespace delaunay_triangulator {
  template <typename T> struct Boundary;
  template <typename T> using b_iterator = typename set<Boundary<T>>::iterator;
  template <typename T> struct Data : vector<point<T>> {
    T x, inf, eps, random_rotation;
    Data(): inf(T(1)/T(0)) {}
    set<Boundary<T>, less<>> shore;
    set<tuple<int, int, int>> invalid;
    void clear() {
      vector<point<T>>::clear();
      shore.clear();
      invalid.clear();
    }
    void invalidate(int i, int j, int k) {
      // could skip insert if collinear
      invalid.insert(tuple(i, j, k));
    }
  };
  template <typename T> Data<T> data;

  template <typename T>
  T relative_distance(const T& l, const T& r) {
    T base = max(min(abs(l), abs(r)), T(1));
    return (r - l) / base;
  }

  template <typename T>
  struct Boundary {
    int li, ri; // indices of left+right parabolas
    Boundary() = default;
    Boundary(int l, int r): li(l), ri(r) {}
    bool operator < (const T& y) const { return y_value(data<T>.x) < y; }
    friend bool operator < (const T& y, const Boundary<T>& b) {
      return y < b.y_value(data<T>.x);
    }
    bool operator < (const Boundary& o) const {
      T y = y_value(data<T>.x);
      T oy = o.y_value(data<T>.x);
      if (abs(relative_distance(y, oy)) > data<T>.eps) return y < oy;
      return left_slope(y, data<T>.x) < o.left_slope(oy, data<T>.x);
    }
    T y_value(const T& x) const {
      if (ri == -1) return data<T>.inf;
      const point<T>& f = max(data<T>[li], data<T>[ri]);
      const point<T>& g = min(data<T>[li], data<T>[ri]);
      if (f.x == x) { // degenerate case
        return max(data<T>[li], data<T>[ri]).y;
      } else { // intersect parabolas
        T a = f.x - g.x; // need a > 0
        T b = 2 * (f.y * (g.x - x) - g.y * (f.x - x));
        T c = (x - g.x) * norm(f) - (x - f.x) * norm(g) + x * x * (g.x - f.x);
        T s = data<T>[li] < data<T>[ri] ? -1 : 1;
        return (-b + s * sqrt(b*b - 4*a*c)) / a / 2;
      }
    }
    T left_slope(const T& y, const T& x) const {
      const point<T>& f = data<T>[li];
      if (f.x == x) return data<T>.inf;
      return (f.y - y) / (x - f.x);
    }
  };

  template <typename T>
  struct Event {
    T xtime; // time of event
    int i, j, k; // ids of points
    Event() = default;
    Event(const T& x, int _i, int _j=-1, int _k=-1): xtime(x), i(_i), j(_j), k(_k) {}
    bool operator < (const Event& o) const { return o.xtime < xtime; }
  };

  template <typename T>
  void add_circle_event(int i, int j, int k, priority_queue<Event<T>>& events) {
    if (data<T>.invalid.count(tuple(i, j, k))) return;
    const auto& a = data<T>[i];
    const auto& b = data<T>[j];
    const auto& c = data<T>[k];
    if (!collinear(a, b, c, data<T>.eps) && b.x != data<T>.x) {
      T x_rem = circumcenter(a, b, c).x + circumradius(a, b, c);
      if (data<T>.x <= x_rem + data<T>.eps) {
        events.emplace(x_rem, i, j, k);
      }
    }
  }

  template <typename T>
  bool handle_circle_event(const Event<T>& ev, priority_queue<Event<T>>& events) {
    if (data<T>.invalid.count(tuple(ev.i, ev.j, ev.k))) return false;
    Boundary<T> left(ev.i, ev.j), right(ev.j, ev.k);
    T lval = left.y_value(ev.xtime + data<T>.eps);
    T rval = right.y_value(ev.xtime + data<T>.eps);
    if (relative_distance(lval, rval) > data<T>.eps) return false;

    data<T>.x = ev.xtime;
    auto it = data<T>.shore.find(left);
    assert(it != data<T>.shore.end());
    int li = it->li;
    int ri = next(it)->ri;
    // invalidate old circle events
    if (it != data<T>.shore.begin()) {
      data<T>.invalidate(prev(it)->li, it->li, it->ri);
    }
    if (next(it, 2)->ri != -1) {
      data<T>.invalidate(it->ri, next(it)->ri, next(it, 2)->ri);
    }
    data<T>.shore.erase(data<T>.shore.erase(it));

    it = data<T>.shore.insert(Boundary<T>(li, ri)).first;
    if (next(it)->ri != -1) {
      add_circle_event(it->li, it->ri, next(it)->ri, events);
    }
    if (it != data<T>.shore.begin()) {
      add_circle_event(prev(it)->li, it->li, it->ri, events);
    }
    return true;
  }

  template <typename T>
  void handle_point_event(const Event<T>& ev, priority_queue<Event<T>>& events) {
    data<T>.x = ev.xtime;
    auto it = data<T>.shore.upper_bound(data<T>[ev.i].y);
    if (it == data<T>.shore.end()) {
      data<T>.shore.insert(Boundary<T>(ev.i, -1));
    } else {
      int hit = it->li;
      if (it->ri != -1 && it != data<T>.shore.begin()) {
        data<T>.invalidate(prev(it)->li, hit, it->ri);
      }
      // add new parabola to the beach line and add new circle events
      data<T>.shore.insert(Boundary<T>(ev.i, hit));
      if (it->ri != -1) {
        add_circle_event(ev.i, hit, it->ri, events);
      }
      it = data<T>.shore.insert(Boundary<T>(hit, ev.i)).first;
      if (it != data<T>.shore.begin()) {
        add_circle_event(prev(it)->li, hit, ev.i, events);
      }
    }
  }

  template <typename T>
  void initialize(const vector<point<T>>& p, const T& e, const T& angle) {
    data<T>.eps = e;
    data<T>.random_rotation = angle;
    if (!data<T>.random_rotation) {
      random_device delaunay_rd;
      mt19937 generator(delaunay_rd());
      data<T>.random_rotation = uniform_real_distribution<T>(0, 3.14)(generator);
      cerr << "delaunay: random_rotation " << data<T>.random_rotation << '\n';
    }
    data<T>.clear();
    transform(begin(p), end(p), back_inserter(data<T>), [=](const point<T>& v) {
        return v * point<T>::polar(T(1), data<T>.random_rotation); });
  }

  template <typename T>
  vector<tuple<int, int, int>> triangulate(
      const vector<point<T>>& p,
      const T& e,
      const T& angle) {
    initialize(p, e, angle);

    vector<tuple<int, int, int>> triangles;
    priority_queue<Event<T>> events;
    for (int i = 0; i < size(data<T>); i++) {
      events.emplace(data<T>[i].x, i, i, i);
    }
    while (!events.empty()) {
      Event ev = events.top();
      events.pop();
      if (ev.i == ev.j) {
        handle_point_event(ev, events);
      } else if (handle_circle_event(ev, events)) {
        triangles.emplace_back(ev.i, ev.j, ev.k);
      }
    }
    return triangles;
  }
}

template <typename T>
vector<tuple<int, int, int>> delaunay(
    const vector<point<T>>& points,
    const T& eps = 1e-9, // this code is really sketchy
    const T& random_angle = 0) {
  return delaunay_triangulator::triangulate(points, eps, random_angle);
}

