/* Mod Int (Montgomery form)
 * USAGE
 *  montgomery_int<mod, prime?> v(num);
 *  assumes the mod is prime (must be coprime with 2)
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
struct montgomery_int {
  static_assert(mod_value > 0);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic" // ISO C++ __int128
  using mod_t = std::conditional_t < mod_value < 1LL << 31, int, long long>;
  using larger_t = std::conditional_t < mod_value < 1LL << 31, long long, __int128>;
#pragma GCC diagnostic pop
  static constexpr mod_t mod = mod_value;

  using umod_t = std::make_unsigned_t<mod_t>;
  static constexpr int shift = CHAR_BIT * sizeof(umod_t);
  static constexpr mod_t r = ((larger_t)1 << shift) % mod;
  static constexpr mod_t r2 = (larger_t)r * r % mod;
  static constexpr mod_t r3 = (larger_t)r2 * r % mod;
  static constexpr umod_t mod_inv =
    -(umod_t)get<1>(extended_gcd((larger_t)mod, (larger_t)1 << shift));

  static constexpr montgomery_int zero = montgomery_int();
  static constexpr montgomery_int one = montgomery_int(1);
  static constexpr montgomery_int two = montgomery_int(2);

  static constexpr mod_t redc(larger_t val) {
    mod_t t = (val + (larger_t)((umod_t)val * mod_inv) * mod) >> shift;
    return t < mod ? t : t - mod;
  }
  static constexpr mod_t to_montgomery(mod_t v) {
    return redc((larger_t)r2 * (v < 0 ? v + mod : v));
  }

  mod_t v;
  constexpr montgomery_int(): v(0) {}
  constexpr montgomery_int(mod_t c): v(to_montgomery(abs(c) >= mod ? c % mod : c)) {}
  constexpr montgomery_int(larger_t c): v(to_montgomery(c % mod)) {}
  constexpr montgomery_int& assign(mod_t c) {
    v = c;
    return *this;
  }

  template <typename fast_input_t>
  void fast_read(fast_input_t& in) {
    in >> v;
    v = to_montgomery(abs(v) >= mod ? v % mod : v);
  }
  friend std::istream& operator >> (std::istream& is, montgomery_int& num) {
    is >> num.v;
    num.v = to_montgomery(abs(num.v) >= mod ? num.v % mod : num.v);
    return is;
  }
  friend std::ostream& operator << (std::ostream& os, const montgomery_int& num) {
    return os << num.value();
  }
  constexpr mod_t value() const { return redc(v); }
  constexpr mod_t legible_value() const {
    mod_t val = value();
    return 2*val <= mod ? val : val - mod;
  }

  bool operator == (const montgomery_int& o) const { return v == o.v; }
  bool operator != (const montgomery_int& o) const { return v != o.v; }
  montgomery_int operator + (const montgomery_int& o) const {
    return montgomery_int().assign(v + o.v < mod ? v + o.v : v + o.v - mod);
  }
  montgomery_int operator - (const montgomery_int& o) const {
    return montgomery_int().assign(v < o.v ? v - o.v + mod : v - o.v);
  }
  montgomery_int operator * (const montgomery_int& o) const {
    return montgomery_int().assign(redc((larger_t)v * o.v));
  }
  montgomery_int operator / (const montgomery_int& o) const {
    return montgomery_int(*this) /= o;
  }
  montgomery_int operator - () const {
    return montgomery_int().assign(v ? mod - v : v);
  }
  montgomery_int& negate() {
    if (v != 0) v = mod - v;
    return *this;
  }
  montgomery_int& operator += (const montgomery_int& o) {
    v += o.v;
    if (v >= mod) v -= mod;
    return *this;
  }
  montgomery_int& operator -= (const montgomery_int& o) {
    v -= o.v;
    if (v < 0) v += mod;
    return *this;
  }
  montgomery_int& operator *= (const montgomery_int& o) {
    v = redc((larger_t)v * o.v);
    return *this;
  }
  montgomery_int& operator /= (const montgomery_int& o) {
    return operator *= (o.inverse());
  }
  montgomery_int pow(long long exponent) const {
    if (exponent == 0) return one;
    if (v == 0) {
      if (exponent < 0) {
        throw std::invalid_argument("raising zero to a negative power");
      }
      return zero;
    }
    if constexpr (is_prime) {
      if (abs(exponent) >= mod - 1) exponent %= mod - 1;
      if (exponent < 0) exponent += mod - 1;
      montgomery_int res(one), base(*this);
      for (; exponent != 0; exponent >>= 1) {
        if (exponent & 1) res *= base;
        base *= base;
      }
      return res;
    } else {
      montgomery_int res(one), base(exponent < 0 ? inverse() : *this);
      for (; exponent != 0; exponent >>= 1) {
        if (exponent & 1) res *= base;
        base *= base;
      }
      return res;
    }
  }
  montgomery_int inverse() const {
    auto [g, x, y] = extended_gcd(mod, v);
    if (g != 1) {
      throw std::invalid_argument("taking the inverse of a non-coprime number");
    }
    return montgomery_int().assign(redc((larger_t)(y + mod) * r3));
  }
};

