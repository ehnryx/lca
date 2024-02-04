/* Fraction
 * USAGE
 *  fraction<T> f(numerator, denominator);
 *  1/0 is +inf, -1/0 is -inf, 0/0 is treated as NaN
 * STATUS
 *  mostly untested: spoj/SNGFRAC1, nadc21/e
 */
#pragma once

#include "int128.h"

#include <cmath>
#include <iostream>

template <typename T, bool overflow_guard = false>
struct fraction {
  T num, den;
  fraction(const T& n = 0, const T& d = 1) {
    T g = gcd(n, d);
    g = g < 0 ? -g : g;
    num = d < 0 ? -n / g : n / g;
    den = d < 0 ? -d / g : d / g;
  }
  fraction(const T& n, const T& d, bool) : num(n), den(d) {}  // skip gcd
  static fraction from_pair(const T& n, const T& d) { return fraction(n, d, false); }
  static fraction inf() { return from_pair(1, 0); }
  template <typename F>
  fraction(const fraction<F>& o) : num(o.num), den(o.den) {}
  const T& numerator() const { return num; }
  const T& denominator() const { return den; }
  T floor() const { return num < 0 ? (num - den + 1) / den : num / den; }
  T integer_part() const { return floor(); }
  fraction fractional_part() const {
    T rem = num % den;
    return fraction(rem < 0 ? rem + den : rem, den, false);
  }
  friend std::ostream& operator<<(std::ostream& os, const fraction& v) {
    return os << v.numerator() << '/' << v.denominator();
  }
  fraction operator-() const { return fraction(-num, den, false); }
  fraction operator+(const fraction& o) const { return fraction(*this) += o; }
  fraction operator-(const fraction& o) const { return fraction(*this) -= o; }
  fraction operator*(const fraction& o) const { return fraction(*this) *= o; }
  fraction operator/(const fraction& o) const { return fraction(*this) /= o; }
  fraction& operator+=(const fraction& o) {
    T g = gcd(den, o.den);
    den /= g;
    num = num * (o.den / g) + o.num * den;
    g = gcd(num, g);
    num /= g;
    den *= o.den / g;
    return *this;
  }
  fraction& operator-=(const fraction& o) {
    T g = gcd(den, o.den);
    den /= g;
    num = num * (o.den / g) - o.num * den;
    g = gcd(num, g);
    num /= g;
    den *= o.den / g;
    return *this;
  }
  fraction& operator*=(const fraction& o) {
    T gn = gcd(num, o.den);
    T gd = gcd(den, o.num);
    num = num / gn * o.num / gd;
    den = den / gd * o.den / gn;
    return *this;
  }
  fraction& operator/=(const fraction& o) {
    T gn = gcd(num, o.num);
    T gd = gcd(den, o.den);
    num = num / gn * (o.num < 0 ? -o.den : o.den) / gd;
    den = den / gd * (o.num < 0 ? -o.num : o.num) / gn;
    return *this;
  }
  fraction inverse() const {
    return num < 0 ? fraction(-den, -num, false) : fraction(den, num, false);
  }
  bool operator<(const fraction& o) const {
    if (den == 0 && o.den == 0) return num && o.num && num < o.num;
    if (den == 0) return num < 0;
    if (o.den == 0) return 0 < o.num;
    if constexpr (overflow_guard) return fraction(*this).compare_guard(o);
    else return num * o.den < o.num * den;
  }
  bool compare_guard(fraction o) {  // destroys *this
    while (den != o.den) {
      T y = floor();
      T oy = o.floor();
      if (y != oy) return y < oy;
      num -= den * y;
      o.num -= o.den * oy;
      if (num == 0 || o.num == 0) break;
      if ((den > o.den && 2 * num > den) || (o.den > den && 2 * o.num > o.den)) {
        num = den - num;
        o.num = o.den - o.num;
      } else {
        swap(num, den);
        swap(o.num, o.den);
      }
      swap(num, o.num);
      swap(den, o.den);
    }
    return num < o.num;
  }
  bool operator>(const fraction& o) const { return o < *this; }
  bool operator==(const fraction& o) const { return num == o.num && den == o.den; }
  bool operator<=(const fraction& o) const { return operator==(o) || operator<(o); }
  bool operator>=(const fraction& o) const { return o <= *this; }
  template <typename D>
  D value() const {
    return D(num) / D(den);
  }
  explicit operator float() const { return value<float>(); }
  explicit operator double() const { return value<double>(); }
  explicit operator long double() const { return value<long double>(); }
  fraction abs() const { return fraction(num < 0 ? -num : num, den, false); }
  template <typename D = long double>
  D sqrt() const {
    return std::sqrt(value<D>());
  }
  struct as_pair {
    bool operator()(const fraction& a, const fraction& b) const {
      return a.num < b.num || (a.num == b.num && a.den < b.den);
    }
  };
};

template <typename T, bool G>
auto abs(const fraction<T, G>& v) {
  return v.abs();
}
template <typename T, bool G>
auto sqrt(const fraction<T, G>& v) {
  return v.sqrt();
}
