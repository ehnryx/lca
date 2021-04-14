/* Point
 * USAGE
 *  pt<T>(x, y);
 *  default T = long double
 * STATUS
 *  tested
 */
#pragma once

template <typename T = long double>
struct point {
  T x, y;
  point() = default;
  point(const T& c): x(c), y(0) {}
  point(const T& _x, const T& _y): x(_x), y(_y) {}
  friend ostream& operator << (ostream& os, const point& v) {
    return os << '(' << v.x << ',' << v.y << ')';
  }
  bool operator < (const point& v) const { return tie(x, y) < tie(v.x, v.y); }
  bool operator == (const point& v) const { return x == v.x && y == v.y; }
  point operator + (const point& v) const { return point(x + v.x, y + v.y); }
  point operator - (const point& v) const { return point(x - v.x, y - v.y); }
  point operator * (const point& v) const { return point(x*v.x - y*v.y, x*v.y + y*v.x); }
  point operator / (const point& v) const { return (*this) * v.inverse(); }
  point operator * (const T& c) const { return point(x*c, y*c); }
  point operator / (const T& c) const { return point(x/c, y/c); }
  friend point operator * (const T& c, const point& v) { return v*c; }
  point inverse() const { return conj() / norm(); }
  point conj() const { return point(x, -y); }
  point perp() const { return point(-y, x); }
  T real() const { return x; }
  T imag() const { return y; }
  T arg() const { return atan2(y, x); }
  T norm() const { return x*x + y*y; }
  T abs() const { return sqrt(norm()); }
  T dot(const point& v) const { return x*v.x + y*v.y; }
  T cross(const point& v) const { return x*v.y - y*v.x; }
};

template <class T> auto real(const T& v) { return v.real(); }
template <class T> auto imag(const T& v) { return v.imag(); }
template <class T> auto arg(const T& v) { return v.arg(); }
template <class T> auto conj(const T& v) { return v.conj(); }
template <class T> auto perp(const T& v) { return v.perp(); }
template <class T> auto norm(const T& v) { return v.norm(); }
template <class T> auto abs(const T& v) { return v.abs(); }
template <class T> auto dot(const T& a, const T& b) { return a.dot(b); }
template <class T> auto cross(const T& a, const T& b) { return a.cross(b); }

