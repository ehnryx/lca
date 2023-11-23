/* Mod Int
 * USAGE
 *  mod_int<mod,prime?> v(num);
 *  assumes the mod is prime by default.
 *  64 bit mods require __int128
 * NOTES
 *  division `operator/` is not recommended
 * STATUS
 *  tested: spoj/RECPWSUM, kattis/dimensionalanalysis
 */
#pragma once

#include "../math/extended_gcd.h"

#include <iostream>

template <long long mod_value, bool is_prime = true>
struct mod_int {
  static_assert(mod_value > 0);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"  // ISO C++ __int128
  static constexpr int int_threshold = 1 << 30;
  using mod_t = std::conditional_t < mod_value<int_threshold, int, long long>;
  using larger_t = std::conditional_t < mod_value<int_threshold, long long, __int128>;
#pragma GCC diagnostic pop
  static constexpr mod_t mod = mod_value;
  mod_t v;
  mod_int() : v(0) {}
  mod_int(mod_t c) : v(c) {
    if (abs(v) >= mod) v %= mod;
    if (v < 0) v += mod;
  }
  mod_int(larger_t c) {
    v = (mod_t)(abs(c) >= mod ? c % mod : c);
    if (v < 0) v += mod;
  }
  void assign(mod_t c) { v = c; }
  friend std::istream& operator>>(std::istream& is, mod_int& num) {
    is >> num.v;
    if (abs(num.v) >= mod) num.v %= mod;
    if (num.v < 0) num.v += mod;
    return is;
  }
  friend std::ostream& operator<<(std::ostream& os, const mod_int& num) { return os << num.v; }
  mod_t value() const { return v; }
  mod_t legible_value() const { return 2 * v <= mod ? v : v - mod; }
  bool operator==(const mod_int& o) const { return v == o.v; }
  bool operator!=(const mod_int& o) const { return v != o.v; }
  mod_int operator+(const mod_int& o) const { return mod_int(*this) += o; }
  mod_int operator-(const mod_int& o) const { return mod_int(*this) -= o; }
  mod_int operator*(const mod_int& o) const { return mod_int(*this) *= o; }
  mod_int operator/(const mod_int& o) const { return mod_int(*this) /= o; }
  mod_int operator-() const { return mod_int(*this).negate(); }
  mod_int& negate() {
    if (v != 0) v = mod - v;
    return *this;
  }
  mod_int& operator+=(const mod_int& o) {
    v += o.v;
    if (v >= mod) v -= mod;
    return *this;
  }
  mod_int& operator-=(const mod_int& o) {
    v -= o.v;
    if (v < 0) v += mod;
    return *this;
  }
  mod_int& operator*=(const mod_int& o) {
    v = (mod_t)((larger_t)v * o.v % mod);
    return *this;
  }
  mod_int& operator/=(const mod_int& o) { return operator*=(o.inverse()); }
  mod_int pow(long long exponent) const {
    if (exponent == 0) return mod_int(1);
    if (v == 0) {
      if (exponent < 0) {
        throw std::invalid_argument("raising zero to a negative power");
      }
      return mod_int(0);
    }
    if constexpr (is_prime) {
      if (std::abs(exponent) >= mod - 1) exponent %= mod - 1;
      if (exponent < 0) exponent += mod - 1;
      mod_int res(1), base(*this);
      for (; exponent != 0; exponent /= 2) {
        if (exponent % 2) res *= base;
        base *= base;
      }
      return res;
    } else {
      mod_int res(1), base(exponent < 0 ? inverse() : *this);
      for (; exponent != 0; exponent /= 2) {
        if (exponent % 2) res *= base;
        base *= base;
      }
      return res;
    }
  }
  mod_int inverse() const {
    auto [g, x, y] = extended_gcd(mod, v);
    if (g != 1) {
      throw std::invalid_argument("taking the inverse of a non-coprime number");
    }
    // assert(operator * (mod_int(y)) == 1);
    return mod_int(y);
  }
};

#include "../utility/fast_input_read.h"

template <typename input_t, long long mod_value, bool is_prime>
struct fast_input_read<input_t, mod_int<mod_value, is_prime>> {
  static void get(input_t& in, mod_int<mod_value, is_prime>& num) {
    in >> num.v;
    if (abs(num.v) >= mod_value) num.v %= mod_value;
    if (num.v < 0) num.v += mod_value;
  }
};
