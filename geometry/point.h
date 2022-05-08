/* Point
 * USAGE
 *  pt<T>(x, y);
 *  default T = long double
 * STATUS
 *  tested somewhat
 */
#pragma once

#include <complex>

template <typename T = long double>
struct point {
  T x, y;
  point() = default;
  point(const T& c): x(c), y(0) {}
  point(const T& _x, const T& _y): x(_x), y(_y) {}
  point(const std::complex<T>& v): x(v.real()), y(v.imag()) {}
  friend std::ostream& operator << (std::ostream& os, const point& v) {
    return os << '(' << v.x << ',' << v.y << ')';
  }
  friend std::istream& operator >> (std::istream& is, point& v) {
    return is >> v.x >> v.y;
  }
  bool operator < (const point& v) const { return tie(x, y) < tie(v.x, v.y); }
  bool operator == (const point& v) const { return x == v.x && y == v.y; }
  point operator + (const point& v) const { return point(x + v.x, y + v.y); }
  point operator - (const point& v) const { return point(x - v.x, y - v.y); }
  point operator * (const point& v) const { return point(x*v.x - y*v.y, x*v.y + y*v.x); }
  point operator / (const point& v) const { return (*this) * v.inverse(); }
  point operator * (const T& c) const { return point(x*c, y*c); }
  point operator / (const T& c) const { return point(x/c, y/c); }
  point& operator += (const point& v) { x += v.x; y += v.y; return *this; }
  point& operator -= (const point& v) { x -= v.x; y -= v.y; return *this; }
  point& operator *= (const point& v) { return *this = point(x*v.x - y*v.y, x*v.y + y*v.x); }
  point& operator /= (const point& v) { return operator *= (v.inverse()); }
  point& operator *= (const T& c) { x *= c; y *= c; return *this; }
  point& operator /= (const T& c) { x /= c; y /= c; return *this; }
  friend point operator * (const T& c, const point& v) { return v*c; }
  point inverse() const { return conj() / norm(); }
  point conj() const { return point(x, -y); }
  point perp() const { return point(-y, x); }
  T real() const { return x; }
  T imag() const { return y; }
  void real(const T& v) { x = v; }
  void imag(const T& v) { y = v; }
  template <typename D = T>
  T norm() const { return x*x + y*y; }
  T dot(const point& v) const { return x*v.x + y*v.y; }
  T cross(const point& v) const { return x*v.y - y*v.x; }
  T arg() const { return atan2(y, x); }
  T abs() const { return sqrt(norm()); }
  static point polar(const T& r, const T& angle) {
    return point<T>(r * cos(angle), r * sin(angle));
  }
  static bool by_angle(const point& a, const point& b) {
    return a.arg() < b.arg();
  }
};

template <typename T> auto real(const point<T>& v) { return v.real(); }
template <typename T> auto imag(const point<T>& v) { return v.imag(); }
template <typename T> auto conj(const point<T>& v) { return v.conj(); }
template <typename T> auto perp(const point<T>& v) { return v.perp(); }
template <typename T> auto norm(const point<T>& v) { return v.norm(); }
template <typename T> auto dot(const point<T>& a, const point<T>& b) { return a.dot(b); }
template <typename T> auto cross(const point<T>& a, const point<T>& b) { return a.cross(b); }

template <typename T> auto arg(const point<T>& v) { return v.arg(); }
template <typename T> auto abs(const point<T>& v) { return v.abs(); }

template <typename T>
bool equal(const point<T>& a, const point<T>& b, const T& eps = 1e-9) {
  return abs(a - b) <= eps;
}

