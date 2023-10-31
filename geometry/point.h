/* Point
 * USAGE
 *  pt<T>(x, y);
 *  T must be integral or floating
 *  intersection types are as follows
 *    float -> float
 *    double -> double
 *    long double -> long double
 *    integral -> double
 * STATUS
 *  untested
 */
#pragma once

#include <complex>
#include <tuple>

namespace geo {
struct strict {
  bool value;
  strict(bool v): value(v) {}
  operator bool() const { return value; }
};

#ifndef INTERSECTION_TYPE
#define INTERSECTION_TYPE double
#endif
template <typename T>
using intersection_type = std::conditional_t<
  std::is_floating_point_v<T>, T, INTERSECTION_TYPE>;

#ifdef NO_BIGGER_TYPE
template <typename T>
using bigger_type = T;
#else
template <typename T>
using bigger_type = std::conditional_t<
  std::is_floating_point_v<T>, T,
  std::conditional_t<
    std::is_same_v<T, long long>, INTERSECTION_TYPE, long long>>;
#endif

template <typename T, typename From>
struct is_constructible {
  static bool constexpr value = std::is_floating_point_v<T> or
    (std::is_integral_v<From> and sizeof(From) <= sizeof(T));
};
template <typename T, typename From>
bool constexpr is_constructible_v = is_constructible<T, From>::value;
}  // namespace geo

template <typename T>
struct point {
  static_assert(std::is_floating_point_v<T> or std::is_integral_v<T>);
  static bool constexpr floating = std::is_floating_point_v<T>;
  using type = T;
  using bigger_type = geo::bigger_type<T>;
  using intersection_type = geo::intersection_type<T>;

  T x, y;
  point(): x(0), y(0) {}
  point(T const& _x, T const& _y): x(_x), y(_y) {}
  point(std::complex<T> const& v): x(v.real()), y(v.imag()) {}
  template <typename U, std::enable_if_t<geo::is_constructible_v<T, U>, bool> = true>
  point(point<U> const& v): x(v.x), y(v.y) {}
  template <typename U, std::enable_if_t<not geo::is_constructible_v<T, U>, bool> = true>
  explicit point(point<U> const& v): x(v.x), y(v.y) {}
  template <typename U = T>
  explicit operator std::complex<U>() const { return std::complex<U>(x, y); }
  friend std::ostream& operator<<(std::ostream& os, point const& v) {
    return os << v.x << " " << v.y;
  }
  friend std::istream& operator>>(std::istream& is, point& v) {
    return is >> v.x >> v.y;
  }
  template <typename fast_input_t>
  void fast_read(fast_input_t& in) {
    in >> x >> y;
  }

  bool operator<(point const& v) const { return std::tie(x, y) < std::tie(v.x, v.y); }
  bool operator>(point const& v) const { return std::tie(x, y) > std::tie(v.x, v.y); }
  bool operator==(point const& v) const { return x == v.x and y == v.y; }
  bool operator!=(point const& v) const { return !operator==(v); }
  point operator-() const { return point(-x, -y); }
  point operator+(point const& v) const { return point(x + v.x, y + v.y); }
  point operator-(point const& v) const { return point(x - v.x, y - v.y); }
  point operator*(point const& v) const { return point(x*v.x - y*v.y, x*v.y + y*v.x); }
  point operator/(point const& v) const { return (*this) * v.inverse(); }
  point operator*(T const& c) const { return point(x * c, y * c); }
  point operator/(T const& c) const { return point(x / c, y / c); }
  point& operator+=(point const& v) { x += v.x; y += v.y; return *this; }
  point& operator-=(point const& v) { x -= v.x; y -= v.y; return *this; }
  point& operator*=(point const& v) { return *this = point(x*v.x - y*v.y, x*v.y + y*v.x); }
  point& operator/=(point const& v) { return operator*=(v.inverse()); }
  point& operator*=(T const& c) { x *= c; y *= c; return *this; }
  point& operator/=(T const& c) { x /= c; y /= c; return *this; }
  friend point operator*(T const& c, point const& v) { return v * c; }
  friend point operator/(T const& c, point const& v) { return v.inverse() * c; }

  point transpose() const { return point(y, x); }
  point inverse() const { return conj() / norm(); }
  point conj() const { return point(x, -y); }
  point perp() const { return point(-y, x); }

  T real() const { return x; }
  T imag() const { return y; }
  void real(T const& v) { x = v; }
  void imag(T const& v) { y = v; }

  template <typename U = bigger_type> U norm() const { return U(x)*x + U(y)*y; }
  template <typename U = bigger_type> U dot(point const& v) const { return U(x)*v.x + U(y)*v.y; }
  template <typename U = bigger_type> U cross(point const& v) const { return U(x)*v.y - U(y)*v.x; }
  template <typename U = bigger_type> auto abs() const { return sqrt(norm<U>()); }
  template <typename U = bigger_type> auto absl() const { return sqrtl(norm<U>()); }
  template <typename U = type> auto arg() const { return atan2(U(y), U(x)); }
  template <typename U = type> auto argl() const { return atan2l(U(y), U(x)); }
  template <typename U, typename V, bool F = floating>
  static std::enable_if_t<F, point> polar(U const& radius, V const& angle) {
    return point(radius * cos(angle), radius * sin(angle));
  }

  static bool by_angle(point const& a, point const& b) {
    return a.arg() < b.arg();
  }
  static bool by_x(point const& a, point const& b) {
    return a.x < b.x or (a.x == b.x && a.y < b.y);
  }
  static bool by_y(point const& a, point const& b) {
    return a.y < b.y or (a.y == b.y && a.x < b.x);
  }
  static bool by_slope(point const& a, point const& b) {
    return a.cross(b) > 0;
  }
};

template <typename T> auto real(point<T> const& v) { return v.real(); }
template <typename T> auto imag(point<T> const& v) { return v.imag(); }
template <typename T> auto conj(point<T> const& v) { return v.conj(); }
template <typename T> auto perp(point<T> const& v) { return v.perp(); }
template <typename T> auto norm(point<T> const& v) { return v.norm(); }
template <typename T> auto arg(point<T> const& v) { return v.arg(); }
template <typename T> auto abs(point<T> const& v) { return v.abs(); }
template <typename T> auto argl(point<T> const& v) { return v.argl(); }
template <typename T> auto absl(point<T> const& v) { return v.absl(); }
template <typename T> auto dot(point<T> const& a, point<T> const& b) { return a.dot(b); }
template <typename T> auto cross(point<T> const& a, point<T> const& b) { return a.cross(b); }

namespace geo {
template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool equal(T eps, point<T> const& a, point<T> const& b) { return abs(a - b) <= eps; }

template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
bool less_than(T eps, point<T> const& a, point<T> const& b) {
  return a.x + eps < b.x or (a.x <= b.x + eps && a.y + eps < b.y);
}

template <typename T>
struct epsilon {
  T value;
  explicit epsilon(T const& v): value(v) {}
  operator T const&() const { return value; }
};
template <typename T, std::enable_if_t<point<T>::floating, bool> = true>
int sign(T x, epsilon<T> eps) {
  return x < -eps ? -1 : x > eps ? 1 : 0;
}
template <typename T, std::enable_if_t<not point<T>::floating, bool> = true>
int sign(T x) {
  return x < 0 ? -1 : x > 0 ? 1 : 0;
}
}  // namespace geo

