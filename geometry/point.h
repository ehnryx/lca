/* Point
 * USAGE
 *  pt<T>(x, y);
 *  default T = long double
 * STATUS
 *  tested somewhat
 */
#pragma once

#include <complex>

template <typename T>
struct point {
  static constexpr bool floating = std::is_floating_point_v<T>;
  using type = T;
  T x, y;
  point() = default;
  point(const T& c): x(c), y(0) {}
  point(const T& _x, const T& _y): x(_x), y(_y) {}
  point(const std::complex<T>& v): x(v.real()), y(v.imag()) {}
  template <typename U>
  point(const point<U>& v): x(v.x), y(v.y) {}
  friend std::ostream& operator << (std::ostream& os, const point& v) {
    return os << v.x << ' ' << v.y;
  }
  friend std::istream& operator >> (std::istream& is, point& v) {
    return is >> v.x >> v.y;
  }
  template <typename fast_input_t>
  void fast_read(fast_input_t& in) {
    in >> x >> y;
  }
  bool operator < (const point& v) const { return tie(x, y) < tie(v.x, v.y); }
  bool operator > (const point& v) const { return tie(x, y) > tie(v.x, v.y); }
  bool operator == (const point& v) const { return x == v.x && y == v.y; }
  bool operator != (const point& v) const { return !operator == (v); }
  point operator - () const { return point(-x, -y); }
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
  point transpose() const { return point(y, x); }
  point inverse() const { return conj() / norm(); }
  point conj() const { return point(x, -y); }
  point perp() const { return point(-y, x); }
  T real() const { return x; }
  T imag() const { return y; }
  void real(const T& v) { x = v; }
  void imag(const T& v) { y = v; }
  template <typename U = T> U norm() const { return U(x)*x + U(y)*y; }
  template <typename U = T> U dot(const point& v) const { return U(x)*v.x + U(y)*v.y; }
  template <typename U = T> U cross(const point& v) const { return U(x)*v.y - U(y)*v.x; }
  template <typename U = T> auto arg() const { return atan2(U(y), U(x)); }
  template <typename U = T> auto abs() const { return sqrt(norm<U>()); }
  template <typename U = T> auto argl() const { return atan2l(U(y), U(x)); }
  template <typename U = T> auto absl() const { return sqrtl(norm<U>()); }
  template <typename U, typename V>
  static point polar(const U& radius, const V& angle) {
    return point(radius * cos(angle), radius * sin(angle));
  }
  static bool by_angle(const point& a, const point& b) {
    return a.arg() < b.arg();
  }
  struct by_x {
    T eps;
    by_x() = default;
    by_x(T e): eps(e) {}
    bool operator () (const point& a, const point& b) const {
      if constexpr (floating) {
        return a.x + eps < b.x || (a.x <= b.x + eps && a.y < b.y);
      } else {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
      }
    }
  };
  struct by_y {
    T eps;
    by_y() = default;
    by_y(T e): eps(e) {}
    bool operator () (const point& a, const point& b) const {
      if constexpr (floating) {
        return a.y + eps < b.y || (a.y <= b.y + eps && a.x < b.x);
      } else {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
      }
    }
  };
};

template <typename T> auto real(const point<T>& v) { return v.real(); }
template <typename T> auto imag(const point<T>& v) { return v.imag(); }
template <typename T> auto conj(const point<T>& v) { return v.conj(); }
template <typename T> auto perp(const point<T>& v) { return v.perp(); }
template <typename T> auto norm(const point<T>& v) { return v.norm(); }
template <typename T> auto arg(const point<T>& v) { return v.arg(); }
template <typename T> auto abs(const point<T>& v) { return v.abs(); }
template <typename T> auto argl(const point<T>& v) { return v.argl(); }
template <typename T> auto absl(const point<T>& v) { return v.absl(); }

template <typename T> auto dot(const point<T>& a, const point<T>& b) { return a.dot(b); }
template <typename T> auto cross(const point<T>& a, const point<T>& b) { return a.cross(b); }

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool equal(T eps, const point<T>& a, const point<T>& b) { return abs(a - b) <= eps; }
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
bool equal(const point<T>& a, const point<T>& b) { return a == b; }

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool less_than(T eps, const point<T>& a, const point<T>& b) {
  return a.x + eps < b.x || (a.x <= b.x + eps && a.y + eps < b.y);
}
template <typename T, std::enable_if_t<!point<T>::floating, bool> = true>
bool less_than(const point<T>& a, const point<T>& b) { return a < b; }

namespace utility {
  template <typename T>
  int sign(const T& x, const T& eps) {
    return x < -eps ? -1 : x > eps ? 1 : 0;
  }
}

