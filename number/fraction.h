/* Fraction
 * USAGE
 *  fraction<T> f(numerator, denominator);
 *  1/0 is +inf, -1/0 is -inf, 0/0 is treated as NaN
 * STATUS
 *  mostly untested: spoj/SNGFRAC1, nadc21/e
 */
#pragma once

#ifdef USE_INT128
__int128 gcd(const __int128& a, const __int128& b) {
  if(b == 0) return a < 0 ? -a : a;
  return gcd(b, a % b);
}
string to_string(const __int128& v) {
  bool neg = v < 0;
  string out;
  for(__int128 n=neg?-v:v; n; n/=10) {
    out.push_back(n % 10 + '0');
  }
  if(out.empty()) out.push_back('0');
  if(neg) out.push_back('-');
  reverse(begin(out), end(out));
  return out;
}
ostream& operator << (ostream& os, const __int128& v) {
  return os << to_string(v);
}
#endif

template <typename T, bool overflow_guard = false>
struct fraction {
  T num, den;
  explicit fraction(const T& n = 0, const T& d = 1) {
    T g = gcd(n, d);
    num = d < 0 ? -n/g : n/g;
    den = d < 0 ? -d/g : d/g;
  }
  fraction(const pair<T, T>& f): num(f.first), den(f.second) {} // skip gcd
  template <typename F> fraction(const fraction<F>& o): num(o.num), den(o.den) {}
  const T& numerator() const { return num; }
  const T& denominator() const { return den; }
  T floor() const { return num < 0 ? (num - den + 1) / den : num / den; }
  friend ostream& operator << (ostream& os, const fraction& v) {
    return os << v.numerator() << '/' << v.denominator();
  }
  fraction operator - () const { return fraction(pair(-num, den)); }
  fraction operator + (const fraction& o) const { return fraction(*this) += o; }
  fraction operator - (const fraction& o) const { return fraction(*this) -= o; }
  fraction operator * (const fraction& o) const { return fraction(*this) *= o; }
  fraction operator / (const fraction& o) const { return fraction(*this) /= o; }
  fraction& operator += (const fraction& o) {
    T g = gcd(den, o.den);
    den /= g;
    num = num * (o.den / g) + o.num * den;
    g = gcd(num, g);
    num /= g;
    den *= o.den / g;
    return *this;
  }
  fraction& operator -= (const fraction& o) {
    T g = gcd(den, o.den);
    den /= g;
    num = num * (o.den / g) - o.num * den;
    g = gcd(num, g);
    num /= g;
    den *= o.den / g;
    return *this;
  }
  fraction& operator *= (const fraction& o) {
    T gn = gcd(num, o.den);
    T gd = gcd(den, o.num);
    num = num / gn * o.num / gd;
    den = den / gd * o.den / gn;
    return *this;
  }
  fraction& operator /= (const fraction& o) {
    T gn = gcd(num, o.num);
    T gd = gcd(den, o.den);
    num = num / gn * (o.num < 0 ? -o.den : o.den) / gd;
    den = den / gd * (o.num < 0 ? -o.num : o.num) / gn;
    return *this;
  }
  bool operator < (const fraction& o) const {
    if (den == 0 && o.den == 0) return num && o.num && num < o.num;
    if (den == 0) return num < 0;
    if (o.den == 0) return 0 < o.num;
    if constexpr (overflow_guard) return fraction(*this).compare_guard(o);
    else return num * o.den < o.num * den;
  }
  bool compare_guard(fraction o) { // destroys *this
    while (den != o.den) {
      T y = floor();
      T oy = o.floor();
      if (y != oy) return y < oy;
      num -= den * y;
      o.num -= o.den * oy;
      if (num == 0 || o.num == 0) break;
      if ((den > o.den && 2*num > den) || (o.den > den && 2*o.num > o.den)) {
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
  bool operator > (const fraction& o) const { return o < *this; }
  bool operator == (const fraction& o) const { return num == o.num && den == o.den; }
  bool operator <= (const fraction& o) const { return operator == (o) || operator < (o); }
  bool operator >= (const fraction& o) const { return o <= *this; }
  template <typename D> D value() const { return D(num) / D(den); }
  explicit operator float() const { return value<float>(); }
  explicit operator double() const { return value<double>(); }
  explicit operator long double() const { return value<long double>(); }
  fraction abs() const { return fraction(pair(num < 0 ? -num : num, den)); }
  template <typename D = long double> D sqrt() const { return std::sqrt(value<D>()); }
  struct as_pair {
    bool operator () (const fraction& a, const fraction& b) const {
      return a.num < b.num || (a.num == b.num && a.den < b.den);
    }
  };
};

template <typename T, bool G> auto abs(const fraction<T,G>& v) { return v.abs(); }
template <typename T, bool G> auto sqrt(const fraction<T,G>& v) { return v.sqrt(); }

