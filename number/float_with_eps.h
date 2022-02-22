/* Floating point number with epsilon comparisons
 * USAGE
 *  float_with_eps<T, eps> value;
 * STATUS
 *  untested
 */
#pragma once

template <typename T, T eps>
struct float_with_eps {
  T v;
  float_with_eps() = default;
  float_with_eps(const T& _v): v(_v) {}
  template <typename U> float_with_eps(const U& _v): v(_v) {}
  float_with_eps& operator += (const float_with_eps& o) { v += o.v; return *this; }
  float_with_eps& operator -= (const float_with_eps& o) { v -= o.v; return *this; }
  float_with_eps& operator *= (const float_with_eps& o) { v *= o.v; return *this; }
  float_with_eps& operator /= (const float_with_eps& o) { v /= o.v; return *this; }
  float_with_eps operator + (const float_with_eps& o) const { return v + o.v; }
  float_with_eps operator - (const float_with_eps& o) const { return v - o.v; }
  float_with_eps operator * (const float_with_eps& o) const { return v * o.v; }
  float_with_eps operator / (const float_with_eps& o) const { return v / o.v; }
  bool operator < (const float_with_eps& o) const { return v + eps < o.v; }
  bool operator > (const float_with_eps& o) const { return v > o.v + eps; }
  bool operator == (const float_with_eps& o) const {
    return !(operator < (o)) && !(operator > (o));
  }
  friend ostream& operator << (ostream& os, const float_with_eps& f) {
    return os << f.v;
  }
};

