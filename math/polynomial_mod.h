/* Polynomial (mod)
 * USAGE
 *  polynomial_mod<T> Q(vector_of_coeffs);
 *  polynomial_mod<T> P(degree);
 *  P[i] = c_i;
 * TIME
 *  ???
 * STATUS
 *  untested
 */
#pragma once

#include "number_theoretic_transform.h"
#include <iostream>

template <long long mod, typename T = long long>
struct polynomial_mod {
  static_assert(std::is_same_v<T, long long> || !std::is_integral_v<T>);
  std::vector<T> coeffs;

  polynomial_mod() {}
  polynomial_mod(int deg, T leading = 1): coeffs(deg + 1) { coeffs[deg] = leading; }
  polynomial_mod(const polynomial_mod& v): coeffs(v.coeffs) {}
  polynomial_mod(polynomial_mod&& v): coeffs(std::move(v.coeffs)) {}
  polynomial_mod(const std::vector<T>& v): coeffs(v) { mod_all_coeffs(); }
  polynomial_mod(std::vector<T>&& v): coeffs(std::move(v)) { mod_all_coeffs(); }
  polynomial_mod& operator = (const polynomial_mod& v) { coeffs = v.coeffs; return *this; }
  void mod_all_coeffs() {
    if constexpr (std::is_integral_v<T>) {
      for (size_t i = 0; i < size(); i++) {
        coeffs[i] %= mod;
        if (coeffs[i] < 0) coeffs[i] += mod;
      }
    }
  }

  size_t size() const { return coeffs.size(); }
  polynomial_mod& resize(size_t s) { coeffs.resize(s); return *this; }
  int degree() const { return (int)coeffs.size() - 1; }
  bool zero() const { return coeffs.empty(); }
  T at(size_t i) const { return i < coeffs.size() ? coeffs[i] : T(0); }
  T operator [] (size_t i) const { return at(i); }
  T leading_coeff() const { return zero() ? T(0) : coeffs.back(); }

  friend std::ostream& operator << (std::ostream& os, const polynomial_mod& v) {
    if (v.zero()) return os << '0';
    os << v.coeffs.front();
    for (size_t i = 1; i < v.size(); i++) {
      os << " " << v[i];
    }
    return os;
  }

  polynomial_mod& make_zero() {
    coeffs.clear();
    return *this;
  }

  polynomial_mod& remove_leading_zeros() {
    while (!zero() && coeffs.back() == 0) {
      coeffs.pop_back();
    }
    return *this;
  }

  polynomial_mod& invert_variable(int deg) {
    coeffs.resize(deg + 1);
    reverse(coeffs.begin(), coeffs.end());
    return remove_leading_zeros();
  }

  polynomial_mod truncate(size_t cut) const {
    if (cut >= size()) return *this;
    polynomial_mod out;
    out.coeffs.resize(cut);
    std::copy(coeffs.begin(), coeffs.begin() + cut, out.coeffs.begin());
    return out.remove_leading_zeros();
  }

  polynomial_mod& _truncate(size_t cut) {
    if (cut < size()) coeffs.resize(cut);
    return *this;
  }

  // scalar

  polynomial_mod& operator *= (T c) {
    if (c == 0) return make_zero();
    if (c == 1) return *this;
    for (size_t i = 0; i < size(); i++) {
      coeffs[i] *= c;
      if constexpr (std::is_integral_v<T>) {
        coeffs[i] %= mod;
        if (coeffs[i] < 0) coeffs[i] += mod;
      }
    }
    return *this;
  }
  polynomial_mod& operator /= (T c) {
    return operator *= (ntt::get_inverse<mod, T>(c));
  }
  polynomial_mod operator * (T c) const {
    return c == 0 ? polynomial_mod() : polynomial_mod(*this) *= c;
  }
  polynomial_mod operator / (T c) const {
    return polynomial_mod(*this) /= c;
  }

  // negation

  polynomial_mod operator - () const {
    return polynomial_mod(*this).negate();
  }
  polynomial_mod& negate() {
    for (size_t i = 0; i < size(); i++) {
      coeffs[i] = -coeffs[i];
      if constexpr (std::is_integral_v<T>) {
        if (coeffs[i] < 0) coeffs[i] += mod;
      }
    }
    return *this;
  }

  // addition

  polynomial_mod& _add(const polynomial_mod& o) {
    if (o.size() > size()) coeffs.resize(o.size());
    for (size_t i = 0; i < o.size(); i++) {
      coeffs[i] += o[i];
      if constexpr (std::is_integral_v<T>) {
        if (coeffs[i] >= mod) coeffs[i] -= mod;
      }
    }
    return *this;
  }

  polynomial_mod& _subtract(const polynomial_mod& o) {
    if (o.size() > size()) coeffs.resize(o.size());
    for (size_t i = 0; i < o.size(); i++) {
      coeffs[i] -= o[i];
      if constexpr (std::is_integral_v<T>) {
        if (coeffs[i] < 0) coeffs[i] += mod;
      }
    }
    return *this;
  }

  polynomial_mod operator + (const polynomial_mod& o) const {
    return polynomial_mod(*this) += o;
  }
  polynomial_mod operator - (const polynomial_mod& o) const {
    return polynomial_mod(*this) -= o;
  }
  polynomial_mod& operator += (const polynomial_mod& o) {
    return _add(o).remove_leading_zeros();
  }
  polynomial_mod& operator -= (const polynomial_mod& o) {
    return _subtract(o).remove_leading_zeros();
  }

  // multiplication

  polynomial_mod operator * (const polynomial_mod& o) const {
    if (zero() || o.zero()) return polynomial_mod();
    return ntt::convolve<mod>(coeffs, o.coeffs);
  }
  polynomial_mod& operator *= (const polynomial_mod& o) {
    if (zero() || o.zero()) return make_zero();
    return *this = ntt::convolve<mod>(coeffs, o.coeffs);
  }

  // division

  polynomial_mod operator / (const polynomial_mod& o) const {
    return divide_remainder(o).first;
  }
  polynomial_mod operator % (const polynomial_mod& o) const {
    return divide_remainder(o).second;
  }
  polynomial_mod& operator /= (const polynomial_mod& o) {
    return *this = divide_remainder(o).first;
  }
  polynomial_mod& operator %= (const polynomial_mod& o) {
    return *this = divide_remainder(o).second;
  }

  std::pair<polynomial_mod, polynomial_mod> divide_remainder(const polynomial_mod& o) const {
    if (o.zero()) throw std::invalid_argument("divide by zero polynomial");
    if (degree() < o.degree()) return std::pair(polynomial_mod(), *this);
    int deg = degree() - o.degree();
    polynomial_mod a(*this), b(o);
    if (T leading = b.leading_coeff(); leading != 1) {
      T inv_leading = ntt::get_inverse<mod, T>(leading);
      a *= inv_leading;
      b *= inv_leading;
    }
    b.invert_variable(b.degree());
    a.invert_variable(a.degree());
    a *= b.reciprocal(deg + 1);
    a.invert_variable(deg);
    polynomial_mod rem = *this - a * o;
    return std::pair(std::move(a), std::move(rem));
  }

  polynomial_mod reciprocal(size_t cut) const {
    polynomial_mod res(0, -ntt::get_inverse<mod, T>(at(0)));
    for (size_t mdeg = 1; mdeg < cut; mdeg *= 2) {
      res = res * (res * this->truncate(2 * mdeg) + polynomial_mod(0, 2));
      res._truncate(2 * mdeg);
    }
    return res._truncate(cut).remove_leading_zeros().negate();
  }

  // derivative / integral

  polynomial_mod derivative() const {
    if (zero()) return *this;
    polynomial_mod res;
    res.resize(size() - 1);
    for (int i = 1; i < size(); i++) {
      res.coeffs[i - 1] = coeffs[i] * i;
      if constexpr (std::is_integral_v<T>) {
        res.coeffs[i - 1] %= mod;
        if (res.coeffs[i - 1] < 0) res.coeffs[i - 1] += mod;
      }
    }
    return res;
  }

  polynomial_mod integral() const {
    if (zero()) return *this;
    polynomial_mod res;
    res.resize(size() + 1);
    for (int i = 0; i < size(); i++) {
      res.coeffs[i + 1] = coeffs[i] * ntt::get_inverse<mod, T>(i + 1);
      if constexpr (std::is_integral_v<T>) {
        res.coeffs[i + 1] %= mod;
        if (res.coeffs[i + 1] < 0) res.coeffs[i + 1] += mod;
      }
    }
    return res;
  }

  // log, exp

  polynomial_mod log(size_t cut) const {
    return (derivative() * reciprocal(cut))._truncate(cut)
      .integral()._truncate(cut).remove_leading_zeros();
  }

  polynomial_mod exp(size_t cut) const {
    if (at(0) != 0) throw std::invalid_argument("exp(P(x)) must have P(0) = 0");
    polynomial_mod res(0, 1);
    for (size_t mdeg = 1; mdeg < cut; mdeg *= 2) {
      res = res * (this->truncate(2 * mdeg) - res.log(2 * mdeg) + polynomial_mod(0, 1));
      res._truncate(2 * mdeg);
    }
    return res._truncate(cut).remove_leading_zeros();
  }

  // evaluation

  T operator () (T x) const { return evaluate(x); }
  T evaluate(T x) const {
    T res = 0;
    T xn = 1;
    for (size_t i = 0; i < size(); i++) {
      res += xn * coeffs[i];
      xn *= x;
    }
    return res;
  }

  // multipoint
  std::vector<T> evaluate(std::vector<T> xs) const {
    if (xs.empty()) return {};
    if (xs.size() == 1) return { evaluate(xs[0]) };
    int lg = 32 - __builtin_clz(xs.size() - 1);
    int n = 1 << lg;
    std::vector<polynomial_mod> blocks(2 * n), modded(2 * n);
    for (size_t i = 0; i < xs.size(); i++) {
      blocks[i + n].coeffs = { -xs[i], 1 };
    }
    for (size_t i = n - 1; i > 0; i--) {
      blocks[i] = (blocks[2*i + 1].zero() ? blocks[2*i] : blocks[2*i] * blocks[2*i + 1]);
    }
    modded[1] = *this;
    for (size_t i = 1; i < n; i++) {
      if (!blocks[2*i].zero()) {
        modded[2*i] = modded[i] % blocks[2*i];
      }
      if (!blocks[2*i + 1].zero()) {
        modded[2*i + 1] = modded[i] % blocks[2*i + 1];
      }
    }
    std::vector<T> res(xs.size());
    for (size_t i = 0; i < xs.size(); i++) {
      res[i] = modded[i + n].at(0);
    }
    return res;
  }

};
