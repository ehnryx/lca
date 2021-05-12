/* Fraction
 * USAGE
 *  mod_int<mod, 32 or 64 bits> v(num);
 *  64 bits requires __int128
 * NOTES
 *  division `operator/` is not recommended
 * STATUS
 *  mostly untested
 */
#pragma once

#include "extended_gcd.h"

template <long long mod_value, int num_bits = 32, bool is_prime = true>
struct mod_int {
  static_assert(num_bits == 32 || num_bits == 64);
  using mod_t = conditional_t<num_bits == 32, int, long long>;
  using larger_t = conditional_t<num_bits == 32, long long, __int128>;
  static const mod_t mod = mod_value;
  mod_t v;
  mod_int() = default;
  mod_int(const mod_t& c): v(c) {
    if(v < 0) v = v % mod + mod;
    else if(v >= mod) v = v % mod;
  }
  friend istream& operator >> (istream& is, mod_int& num) {
    is >> num.v;
    if(num.v < 0) num.v = num.v % mod + mod;
    else if(num.v >= mod) num.v = num.v % mod;
    return is;
  }
  friend ostream& operator << (ostream& os, const mod_int& num) { return os << num.v; }
  bool operator == (const mod_int& o) const { return v == o.v; }
  mod_int operator + (const mod_int& o) const { return mod_int(*this) += o; }
  mod_int operator - (const mod_int& o) const { return mod_int(*this) -= o; }
  mod_int operator * (const mod_int& o) const { return mod_int(*this) *= o; }
  mod_int operator / (const mod_int& o) const { return mod_int(*this) /= o; }
  mod_int operator - () const { return mod_int(v ? mod - v : v); }
  mod_int& operator += (const mod_int& o) {
    v += o.v;
    if(v >= mod) v -= mod;
    return *this;
  }
  mod_int& operator -= (const mod_int& o) {
    v -= o.v;
    if(v < 0) v += mod;
    return *this;
  }
  mod_int& operator *= (const mod_int& o) {
    v = (larger_t)v * o.v % mod;
    return *this;
  }
  mod_int& operator /= (const mod_int& o) {
    v = (larger_t)v * o.inverse() % mod;
    return *this;
  }
  template <typename T, typename = enable_if_t<is_integral_v<T>>>
  mod_int pow(T exponent) const {
    if constexpr (is_prime) {
      if (exponent < 0) {
        assert(v != 0);
        exponent = mod - 1 + exponent % (mod - 1);
      }
      mod_int res(1), base(*this);
      for ( ; exponent != 0; exponent /= 2) {
        if (exponent % 2) res *= base;
        base *= base;
      }
      return res;
    } else {
      mod_int res(1), base(exponent < 0 ? inverse() : *this);
      for ( ; exponent != 0; exponent /= 2) {
        if (exponent % 2) res *= base;
        base *= base;
      }
      return res;
    }
  }
  mod_int inverse() const {
    auto [g, x, y] = extended_gcd(mod, v);
    assert(g == 1);
    return mod_int(y < 0 ? y + mod : y);
  }
};

